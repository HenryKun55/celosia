// typed_a8.inc: round A8's typed functions (tools/typed_lift2.py, stage 8: the varargs functions' spills, more MIPS-step natives). Generated: do not edit. Included inside a namespace (typed_a8.h: namespace typed), after typed_a7.inc. First the variants of earlier functions (<name>_x8: the same MIPS typed with extra outputs, called by stage-8 functions only), then stage 8's functions.
// iosCopyMatrix_x8 (0x1b9148): typed by tools/typed_lift2.py (round A6; leaf, extra outputs xo[0]=a2 xo[1]=a3 xo[2]=t0 xo[3]=t1; VU0 in -, out h6 h7 h8 h9)
inline void iosCopyMatrix_x8(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t a0, uint32_t a1, uint64_t &io_h6, uint64_t &io_h7, uint64_t &io_h8, uint64_t &io_h9, uint64_t *xo = nullptr)
{
    uint64_t a2 = 0;
    uint64_t a3 = 0;
    uint64_t t0 = 0;
    uint64_t t1 = 0;
    uint64_t h6 = io_h6;
    uint64_t h7 = io_h7;
    uint64_t h8 = io_h8;
    uint64_t h9 = io_h9;
    do { const a4::q128 _v = (A4_R128(ADD32(a1, 48))); h9 = a4::hi64(_v); std::memcpy(&t1, &_v, 8); } while (0);
    A4_W128(ADD32(a0, 48), a4::mk128(h9, t1));
    do { const a4::q128 _v = (A4_R128(ADD32(a1, 32))); h8 = a4::hi64(_v); std::memcpy(&t0, &_v, 8); } while (0);
    A4_W128(ADD32(a0, 32), a4::mk128(h8, t0));
    do { const a4::q128 _v = (A4_R128(ADD32(a1, 16))); h7 = a4::hi64(_v); std::memcpy(&a3, &_v, 8); } while (0);
    A4_W128(ADD32(a0, 16), a4::mk128(h7, a3));
    do { const a4::q128 _v = (A4_R128(ADD32(a1, 0))); h6 = a4::hi64(_v); std::memcpy(&a2, &_v, 8); } while (0);
    A4_W128(ADD32(a0, 0), a4::mk128(h6, a2));
    { if (xo) { xo[0] = a2; xo[1] = a3; xo[2] = t0; xo[3] = t1; } io_h6 = h6; io_h7 = h7; io_h8 = h8; io_h9 = h9; return; }
    __builtin_unreachable();
}

// glColor4f_x8 (0x108e8b8): typed by tools/typed_lift2.py (round A6; loop leaf, extra outputs xo[0]=f12 xo[1]=f13 xo[2]=f14 xo[3]=f15; VU0 in -, out -)
inline a1::RetVF glColor4f_x8(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, float f12, float f13, float f14, float f15, uint64_t *xo = nullptr)
{
    float f0 = 0.0f;
    float f1 = 0.0f;
    float f2 = 0.0f;
    uint32_t at = 0;
    uint32_t v0 = 0;
    uint32_t v1 = 0;
    uint32_t a0 = 0;
    uint32_t fcr31 = 0;
    at = 0x437F0000u;
    f0 = a1_flt(a7_opq(at));
    v0 = 0x1180000u;
    at = 0x43000000u;
    f1 = a1_flt(a7_opq(at));
    v0 = v0 + 0x6390u;
    f12 = a1f::mul(f12, f0);
    f2 = ((float)(f0));
    f15 = a1f::mul(f15, f1);
    v1 = v0 + 0x90u;
    f13 = a1f::mul(f13, f0);
    a0 = 3u;
    f14 = a1f::mul(f14, f0);
    A1_W32B(v0, 144, a1_bits(f12));
    A1_W32B(v0, 156, a1_bits(f15));
    A1_W32B(v0, 148, a1_bits(f13));
    A1_W32B(v0, 152, a1_bits(f14));
    f1 = a1_flt(0u);
L_108e900:;
    f0 = a1_flt(A1_R32(v1));
    fcr31 = (a1f::lt(f2, f0)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
    if ((fcr31 & 0x800000u) == 0u) goto L_108e918;
    A1_W32(v1, a1_bits(f2));
    f0 = ((float)(f2));
L_108e918:;
    fcr31 = (a1f::lt(f0, f1)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
    if ((fcr31 & 0x800000u) != 0u) { A1_W32(v1, 0u); goto L_108e924; }
L_108e924:;
    a0 = a0 - 1u;
    { const bool c = (int32_t)a0 >= 0; v1 = v1 + 4u; if (c) { A3_BACK(); goto L_108e900; } }
    { if (xo) { xo[0] = a1_bits(f12); xo[1] = a1_bits(f13); xo[2] = a1_bits(f14); xo[3] = a1_bits(f15); } return a1::RetVF{(uint64_t)a1_sx64(v0), f0}; }
    __builtin_unreachable();
}

// GetCharacterClass_x8 (0x10c0a70): typed by tools/typed_lift2.py (round A6; leaf, extra outputs xo[0]=a0 xo[1]=a1; VU0 in h5, out h2)
inline uint64_t GetCharacterClass_x8(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint64_t a0, uint64_t &io_h2, uint64_t io_h5, uint64_t *xo = nullptr)
{
    uint64_t v0 = 0;
    uint64_t v1 = 0;
    uint32_t a1 = 0;
    uint64_t h2 = io_h2;
    uint64_t h5 = io_h5;
    (v0 = (uint64_t)((uint64_t)a0 + (uint64_t)0ULL));
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 9))));
    (a0 = (uint64_t)(uint32_t)((uint8_t)A1_R8B((uint32_t)v0, 8)));
    (a1 = (uint32_t)((int32_t)((int32_t)ADD32((uint32_t)v0, 144))));
    { const bool c = (int64_t)a0 == (int64_t)v1; (v0 = (uint64_t)((uint64_t)a1_sx64(a1) + (uint64_t)0ULL)); if (c) goto L_10c0a98; }
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 22))));
    (v0 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL));
    (v1 = (uint64_t)(RC_U64X(a0) ^ RC_U64X(v1)));
    if (RC_U64X(v1) == 0) do { const a4::q128 _v = (a4::mk128(h5, a1_sx64(a1))); h2 = a4::hi64(_v); std::memcpy(&v0, &_v, 8); } while (0);
L_10c0a98:;
    { if (xo) { xo[0] = a0; xo[1] = a1_sx64(a1); } io_h2 = h2; return v0; }
    __builtin_unreachable();
}

// SpriteMenuGetLayerNoByHash_x8 (0x1112008): typed by tools/typed_lift2.py (round A6; loop leaf, extra outputs xo[0]=a2 xo[1]=a3 xo[2]=t0; VU0 in h4, out h3)
inline uint64_t SpriteMenuGetLayerNoByHash_x8(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint64_t a0, uint64_t a1, uint64_t &io_h3, uint64_t io_h4, uint64_t *xo = nullptr)
{
    uint64_t v0 = 0;
    uint64_t v1 = 0;
    uint64_t a2 = 0;
    uint32_t a3 = 0;
    uint64_t t0 = 0;
    uint64_t h3 = io_h3;
    uint64_t h4 = io_h4;
    (v0 = (uint64_t)(uint32_t)((uint8_t)A1_R8B((uint32_t)a0, 8)));
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)a0, 144))));
    (v1 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL));
    (t0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 4294967295))));
    (v0 = (uint64_t)(RC_U64X(v0) ^ (uint64_t)(uint16_t)33));
    if (RC_U64X(v0) == 0) do { const a4::q128 _v = (a4::mk128(h4, a0)); h3 = a4::hi64(_v); std::memcpy(&v1, &_v, 8); } while (0);
    (a3 = (uint32_t)((int32_t)((int32_t)A1_R32B((uint32_t)v1, 8))));
    { const bool c = (int32_t)a3 <= 0; (a2 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL)); if (c) goto L_111207c; }
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)v1, 4))));
    (a0 = (uint64_t)((uint64_t)RC_U64X(v0) + (uint64_t)0ULL));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)a0, 0))));
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)v0, 4))));
    if ((int64_t)v1 != (int64_t)a1) { (a2 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)a2, 1)))); goto L_1112050; }
    (t0 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL));
    goto L_111207c;
L_1112050:;
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)SLL32((uint32_t)a2, 4))));
    (v1 = (uint64_t)(((int64_t)(int64_t)a2 < (int64_t)(int64_t)a1_sx64(a3)) ? 1 : 0));
    { const bool c = (int64_t)v1 == 0; (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v0, (uint32_t)a2)))); if (c) goto L_111207c; }
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)SLL32((uint32_t)v0, 2))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)a0, (uint32_t)v0))));
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)v0, 0))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)v1, 4))));
    if ((int64_t)v0 != (int64_t)a1) { (a2 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)a2, 1)))); { A3_BACK(); goto L_1112050; } }
    (t0 = (uint64_t)((uint64_t)a2 + (uint64_t)0ULL));
L_111207c:;
    { (v0 = (uint64_t)((uint64_t)t0 + (uint64_t)0ULL)); if (xo) { xo[0] = a2; xo[1] = a1_sx64(a3); xo[2] = t0; } io_h3 = h3; return v0; }
    __builtin_unreachable();
}

// SpriteMenuSetNumPartsFormatByPartsParam_x8 (0x11137b8): typed by tools/typed_lift2.py (round A6; leaf, extra outputs xo[0]=a2; VU0 in -, out -)
inline uint32_t SpriteMenuSetNumPartsFormatByPartsParam_x8(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t a1, uint32_t a2, uint64_t *xo = nullptr)
{
    uint32_t v0 = 0;
    uint32_t v1 = 0;
    v0 = A1_R32B(a1, 24);
    v1 = 0xF00F0000u;
    a2 = a2 & 0xFFu;
    v1 = v1 | 0xFFFFu;
    a2 = a2 << 20;
    v0 = v0 & v1;
    v0 = v0 | a2;
    { A1_W32B(a1, 24, v0); if (xo) { xo[0] = a1_sx64(a2); } return v0; }
    __builtin_unreachable();
}

// EntryModifiedManager_x8 (0x11478b8): typed by tools/typed_lift2.py (round A6; leaf, extra outputs xo[0]=a0 xo[1]=a2 xo[2]=a3 xo[3]=t0 xo[4]=t1 xo[5]=t2; VU0 in -, out -)
inline uint64_t EntryModifiedManager_x8(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint64_t a0, uint32_t a1, uint64_t *xo = nullptr)
{
    uint64_t v0 = 0;
    uint32_t v1 = 0;
    uint32_t a2 = 0;
    uint64_t a3 = 0;
    uint32_t t0 = 0;
    uint64_t t1 = 0;
    uint32_t t2 = 0;
    (a3 = (uint64_t)((uint64_t)a0 + (uint64_t)0ULL));
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 2))));
    (t0 = (uint32_t)((int32_t)((int32_t)A1_R32B((uint32_t)a3, 0))));
    (a2 = (uint32_t)((int32_t)((int32_t)A1_R32B((uint32_t)a3, 12))));
    v1 = t0 + 1u;
    t2 = t0 << 1;
    (v0 = (uint64_t)((uint64_t)a1_sx64(t0) + (uint64_t)0ULL));
    (t1 = (uint64_t)(((int64_t)(int64_t)a1_sx64(a2) < (int64_t)(int32_t)3) ? 1 : 0));
    { const bool c = (int64_t)(int32_t)a2 == (int64_t)a0; A1_W32B((uint32_t)a3, 0, v1); if (c) goto L_1147920; }
    { const bool c = (int64_t)t1 == 0; v1 = 4u; if (c) goto L_1147900; }
    v1 = 1u;
    if (a2 == v1) { (v1 = (uint32_t)((int32_t)((int32_t)A1_R32B((uint32_t)a3, 16)))); goto L_1147910; }
    (v1 = (uint32_t)((int32_t)((int32_t)A1_R32B((uint32_t)a3, 8))));
    goto L_1147940;
L_1147900:;
    if (a2 == v1) { (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)a3, 16)))); goto L_1147930; }
    (v1 = (uint32_t)((int32_t)((int32_t)A1_R32B((uint32_t)a3, 8))));
    goto L_1147940;
L_1147910:;
    v1 = v1 + t0;
    A1_W8(v1, (uint8_t)a1);
    goto L_114793c;
L_1147920:;
    (v1 = (uint32_t)((int32_t)((int32_t)A1_R32B((uint32_t)a3, 16))));
    v1 = t2 + v1;
    A1_W16(v1, (uint16_t)a1);
    goto L_114793c;
L_1147930:;
    v1 = t0 << 2;
    (v1 = (uint32_t)((int32_t)((int32_t)ADD32(v1, (uint32_t)a0))));
    A1_W32(v1, a1);
L_114793c:;
    (v1 = (uint32_t)((int32_t)((int32_t)A1_R32B((uint32_t)a3, 8))));
L_1147940:;
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 1))));
    v1 = v1 + a1;
    { A1_W8B(v1, 0, (uint8_t)(uint32_t)a0); if (xo) { xo[0] = a0; xo[1] = a1_sx64(a2); xo[2] = a3; xo[3] = a1_sx64(t0); xo[4] = t1; xo[5] = a1_sx64(t2); } return v0; }
    __builtin_unreachable();
}

// GetScreenRadius_x8 (0x1160b00): typed by tools/typed_lift2.py (round A6; guest frame leaf, extra outputs xo[0]=a0 xo[1]=a2; VU0 in vf10 vacc, out vf8 vf10 vf14 vf15 vf16 vf17 vacc)
inline a1::RetVF GetScreenRadius_x8(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t a0, uint32_t a1, uint32_t sp, float f12, a4::vec4 &io_vf8, a4::vec4 &io_vf10, a4::vec4 &io_vf14, a4::vec4 &io_vf15, a4::vec4 &io_vf16, a4::vec4 &io_vf17, a4::vec4 &io_vacc, uint64_t *xo = nullptr)
{
    float f0 = 0.0f;
    float f1 = 0.0f;
    float f2 = 0.0f;
    uint32_t at = 0;
    uint32_t v0 = 0;
    uint32_t v1 = 0;
    uint32_t a2 = 0;
    uint32_t fcr31 = 0;
    a4::vec4 vf8 = io_vf8;
    a4::vec4 vf10 = io_vf10;
    a4::vec4 vf14 = io_vf14;
    a4::vec4 vf15 = io_vf15;
    a4::vec4 vf16 = io_vf16;
    a4::vec4 vf17 = io_vf17;
    a4::vec4 vacc = io_vacc;
    sp = sp - 0x80u;
    v1 = a0 + 0x1A0u;
    v0 = sp + 0x10u;
    vf8 = a4v::castsi128_ps(A4_R128(ADD32(a1, 0)));
    vf14 = a4v::castsi128_ps(A4_R128(ADD32(v1, 0)));
    vf15 = a4v::castsi128_ps(A4_R128(ADD32(v1, 16)));
    vf16 = a4v::castsi128_ps(A4_R128(ADD32(v1, 32)));
    vf17 = a4v::castsi128_ps(A4_R128(ADD32(v1, 48)));
    { a4::vec4 res = a4v::mul(vf14, a4v::bcast<0>(vf8)); vacc = a4v::dest<1, 1, 1, 1>(vacc, res); }
    { a4::vec4 mul_res = a4v::mul(vf15, a4v::bcast<1>(vf8)); a4::vec4 res = a4v::addK<1, 1>(vacc, mul_res); vacc = a4v::dest<1, 1, 1, 1>(vacc, res); }
    { a4::vec4 mul_res = a4v::mul(vf16, a4v::bcast<2>(vf8)); a4::vec4 res = a4v::addK<1, 1>(vacc, mul_res); vacc = a4v::dest<1, 1, 1, 1>(vacc, res); }
    { a4::vec4 mul_res = a4v::mul(vf17, a4v::bcast<3>(vf8)); a4::vec4 res = a4v::addK<1, 1>(vacc, mul_res); a4::q128 mask = a4v::set_epi32(-1, -1, -1, -1); vf10 = a4v::blendv_ps(vf10, res, a4v::castsi128_ps(mask)); }
    A4_W128(ADD32(v0, 0), a4v::castps_si128(vf10));
    f1 = a1_flt(A1_R32B(sp, 24));
    a2 = sp + 0x30u;
    f0 = a1_flt(0u);
    fcr31 = (a1f::lt(f1, f0)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
    { const bool c = (fcr31 & 0x800000u) == 0u; v0 = sp + 0x50u; if (c) goto L_1160b80; }
    vf8 = a4v::castsi128_ps(A4_R128(ADD32(a1, 0)));
    vf14 = a4v::castsi128_ps(A4_R128(ADD32(v1, 0)));
    vf15 = a4v::castsi128_ps(A4_R128(ADD32(v1, 16)));
    vf16 = a4v::castsi128_ps(A4_R128(ADD32(v1, 32)));
    vf17 = a4v::castsi128_ps(A4_R128(ADD32(v1, 48)));
    { a4::vec4 res = a4v::mul(vf14, a4v::bcast<0>(vf8)); vacc = a4v::dest<1, 1, 1, 1>(vacc, res); }
    { a4::vec4 mul_res = a4v::mul(vf15, a4v::bcast<1>(vf8)); a4::vec4 res = a4v::addK<1, 1>(vacc, mul_res); vacc = a4v::dest<1, 1, 1, 1>(vacc, res); }
    { a4::vec4 mul_res = a4v::mul(vf16, a4v::bcast<2>(vf8)); a4::vec4 res = a4v::addK<1, 1>(vacc, mul_res); vacc = a4v::dest<1, 1, 1, 1>(vacc, res); }
    { a4::vec4 mul_res = a4v::mul(vf17, a4v::bcast<3>(vf8)); a4::vec4 res = a4v::addK<1, 1>(vacc, mul_res); a4::q128 mask = a4v::set_epi32(-1, -1, -1, -1); vf10 = a4v::blendv_ps(vf10, res, a4v::castsi128_ps(mask)); }
    A4_W128(ADD32(a2, 0), a4v::castps_si128(vf10));
    f0 = a1_flt(A1_R32B(sp, 56));
    f1 = (-(float)(f0));
    goto L_1160bac;
L_1160b80:;
    vf8 = a4v::castsi128_ps(A4_R128(ADD32(a1, 0)));
    vf14 = a4v::castsi128_ps(A4_R128(ADD32(v1, 0)));
    vf15 = a4v::castsi128_ps(A4_R128(ADD32(v1, 16)));
    vf16 = a4v::castsi128_ps(A4_R128(ADD32(v1, 32)));
    vf17 = a4v::castsi128_ps(A4_R128(ADD32(v1, 48)));
    { a4::vec4 res = a4v::mul(vf14, a4v::bcast<0>(vf8)); vacc = a4v::dest<1, 1, 1, 1>(vacc, res); }
    { a4::vec4 mul_res = a4v::mul(vf15, a4v::bcast<1>(vf8)); a4::vec4 res = a4v::addK<1, 1>(vacc, mul_res); vacc = a4v::dest<1, 1, 1, 1>(vacc, res); }
    { a4::vec4 mul_res = a4v::mul(vf16, a4v::bcast<2>(vf8)); a4::vec4 res = a4v::addK<1, 1>(vacc, mul_res); vacc = a4v::dest<1, 1, 1, 1>(vacc, res); }
    { a4::vec4 mul_res = a4v::mul(vf17, a4v::bcast<3>(vf8)); a4::vec4 res = a4v::addK<1, 1>(vacc, mul_res); a4::q128 mask = a4v::set_epi32(-1, -1, -1, -1); vf10 = a4v::blendv_ps(vf10, res, a4v::castsi128_ps(mask)); }
    A4_W128(ADD32(v0, 0), a4v::castps_si128(vf10));
    f1 = a1_flt(A1_R32B(sp, 88));
L_1160bac:;
    at = 0x3F800000u;
    f0 = a1_flt(a7_opq(at));
    a0 = a0 + 0xC0u;
    v1 = sp + 0x60u;
    v0 = sp + 0x70u;
    A1_W32B(sp, 112, a1_bits(f12));
    A1_W32B(sp, 124, a1_bits(f0));
    A1_W32B(sp, 116, 0u);
    A1_W32B(sp, 120, a1_bits(f1));
    vf8 = a4v::castsi128_ps(A4_R128(ADD32(v0, 0)));
    vf14 = a4v::castsi128_ps(A4_R128(ADD32(a0, 0)));
    vf15 = a4v::castsi128_ps(A4_R128(ADD32(a0, 16)));
    vf16 = a4v::castsi128_ps(A4_R128(ADD32(a0, 32)));
    vf17 = a4v::castsi128_ps(A4_R128(ADD32(a0, 48)));
    { a4::vec4 res = a4v::mul(vf14, a4v::bcast<0>(vf8)); vacc = a4v::dest<1, 1, 1, 1>(vacc, res); }
    { a4::vec4 mul_res = a4v::mul(vf15, a4v::bcast<1>(vf8)); a4::vec4 res = a4v::addK<1, 1>(vacc, mul_res); vacc = a4v::dest<1, 1, 1, 1>(vacc, res); }
    { a4::vec4 mul_res = a4v::mul(vf16, a4v::bcast<2>(vf8)); a4::vec4 res = a4v::addK<1, 1>(vacc, mul_res); vacc = a4v::dest<1, 1, 1, 1>(vacc, res); }
    { a4::vec4 mul_res = a4v::mul(vf17, a4v::bcast<3>(vf8)); a4::vec4 res = a4v::addK<1, 1>(vacc, mul_res); a4::q128 mask = a4v::set_epi32(-1, -1, -1, -1); vf10 = a4v::blendv_ps(vf10, res, a4v::castsi128_ps(mask)); }
    A4_W128(ADD32(v1, 0), a4v::castps_si128(vf10));
    f0 = a1_flt(A1_R32B(sp, 96));
    f1 = a1_flt(A1_R32B(sp, 108));
    at = 0x45000000u;
    f2 = a1_flt(a7_opq(at));
    f0 = a1f::div(fcr31, f0, f1);
    f0 = a1f::subFinite(f0, f2);
    { sp = sp + 0x80u; if (xo) { xo[0] = a1_sx64(a0); xo[1] = a1_sx64(a2); } io_vf8 = vf8; io_vf10 = vf10; io_vf14 = vf14; io_vf15 = vf15; io_vf16 = vf16; io_vf17 = vf17; io_vacc = vacc; return a1::RetVF{(uint64_t)a1_sx64(v0), f0}; }
    __builtin_unreachable();
}

// gamecore_sub_01386B98_x8 (0x1386b98): typed by tools/typed_lift2.py (round A6; loop leaf, extra outputs xo[0]=f0 xo[1]=a3 xo[2]=t0 xo[3]=t1 xo[4]=t2 xo[5]=t3; VU0 in -, out -)
inline uint32_t gamecore_sub_01386B98_x8(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint64_t *xo = nullptr)
{
    float f0 = 0.0f;
    uint32_t v0 = 0;
    uint32_t v1 = 0;
    uint32_t a0 = 0;
    uint32_t a1 = 0;
    uint32_t a2 = 0;
    uint32_t a3 = 0;
    uint32_t t0 = 0;
    uint32_t t1 = 0;
    uint64_t t2 = 0;
    uint64_t t3 = 0;
    uint32_t t4 = 0;
    bool a3xw0 = false;
    v0 = 0x11E0000u;
    v1 = 0x1460000u;
    t4 = v0 + 0x62ACu;
    (v1 = (uint32_t)((int32_t)((int32_t)ADD32(v1, 22096))));
    (t2 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL));
    (t3 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL));
    t0 = t4 - 0x1Cu;
L_1386bb8:;
    (v0 = (uint32_t)((int32_t)((int32_t)SLL32((uint32_t)t3, 3))));
    t1 = 3u;
    a2 = v0 + t4;
L_1386bc8:;
    v0 = A1_R32(a2);
    if (v0 == 0) { A1_W32B(t0, 108, 0u); goto L_1386be4; }
    A1_W32(a2, 0u);
    A1_W32B(a2, 4, 0u);
    A1_W32B(a2, 8, 0u);
    A1_W32B(t0, 108, 0u);
L_1386be4:;
    a1 = t0;
    a3 = 0u;
    a0 = t0 + 0x1Cu;
    v0 = A1_R32(a0);
L_1386bf8:;
    { const bool c = v0 != 0; a0 = a0 + 0x10u; if (c) goto L_1386c38; }
    a3 = a3 + 1u;
    v0 = (int32_t)a3 < 4 ? 1u : 0u;
    if (v0 != 0) { v0 = A1_R32(a0); { A3_BACK(); goto L_1386bf8; } }
    f0 = a1_flt(A1_R32B(v1, 376)); a3xw0 = true;
    A1_W32B(a1, 4, 0u);
    A1_W32B(a1, 24, a1_bits(f0));
    A1_W32B(a1, 8, (uint32_t)t2);
    A1_W32B(a1, 12, 0u);
    A1_W32B(a1, 16, 0u);
    A1_W32B(a1, 20, 0u);
L_1386c38:;
    t1 = t1 - 1u;
    { const bool c = (int32_t)t1 >= 0; a2 = a2 + 0x10u; if (c) { A3_BACK(); goto L_1386bc8; } }
    (t2 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)t2, 1))));
    t0 = t0 + 0x78u;
    (v0 = (uint32_t)(((int64_t)(int64_t)RC_U64X(t2) < (int64_t)(int32_t)8) ? 1 : 0));
    { const bool c = v0 != 0; (t3 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)t3, 15)))); if (c) { A3_BACK(); goto L_1386bb8; } }
    { if (xo) { if (a3xw0) xo[0] = a1_bits(f0); xo[1] = a1_sx64(a3); xo[2] = a1_sx64(t0); xo[3] = a1_sx64(t1); xo[4] = t2; xo[5] = t3; } return v0; }
    __builtin_unreachable();
}

// getLayoutClassCollisionIndepSurfBoundary_x8 (0x1119248): typed by tools/typed_lift2.py (round A6; guest frame calls, extra outputs xo[0]=f2 xo[1]=a0 xo[2]=a1 xo[3]=a2 xo[4]=a3 xo[5]=t0 xo[6]=t1 xo[7]=t2 xo[8]=t3; VU0 in vf10 vf29 vf31 vacc, out vf8 vf10 vf14 vf15 vf16 vf17 vf29 vf30 vf31 vacc h3)
inline uint64_t getLayoutClassCollisionIndepSurfBoundary_x8(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint64_t a0, uint64_t a1, uint64_t a2, uint32_t sp, float f12, float f13, float f14, float f15, float f16, float f17, float f18, float f19, a4::vec4 &io_vf8, a4::vec4 &io_vf10, a4::vec4 &io_vf14, a4::vec4 &io_vf15, a4::vec4 &io_vf16, a4::vec4 &io_vf17, a4::vec4 &io_vf29, a4::vec4 &io_vf30, a4::vec4 &io_vf31, a4::vec4 &io_vacc, uint64_t &io_h3, uint64_t *xo = nullptr)
{
    float f0 = 0.0f;
    float f1 = 0.0f;
    float f2 = 0.0f;
    uint32_t at = 0;
    uint64_t v0 = 0;
    uint64_t v1 = 0;
    uint64_t a3 = 0;
    uint64_t t0 = 0;
    uint64_t t1 = 0;
    uint64_t t2 = 0;
    uint32_t t3 = 0;
    uint64_t s0 = 0;
    a4::vec4 vf8 = io_vf8;
    a4::vec4 vf10 = io_vf10;
    a4::vec4 vf14 = io_vf14;
    a4::vec4 vf15 = io_vf15;
    a4::vec4 vf16 = io_vf16;
    a4::vec4 vf17 = io_vf17;
    a4::vec4 vf29 = io_vf29;
    a4::vec4 vf30 = io_vf30;
    a4::vec4 vf31 = io_vf31;
    a4::vec4 vacc = io_vacc;
    uint64_t h3 = io_h3;
    bool a3xw0 = false;
    bool a3xw3 = false;
    bool a3xw8 = false;
    sp = sp - 0x40u;
    (t1 = (uint64_t)((uint64_t)a0 + (uint64_t)0ULL));
    (t0 = (uint64_t)((uint64_t)a1 + (uint64_t)0ULL));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)SLL32((uint32_t)t0, 1))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v0, (uint32_t)t0))));
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)SLL32((uint32_t)t0, 3))));
    (a1 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)t1, 372))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)SLL32((uint32_t)v0, 5))));
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v1, (uint32_t)t0))));
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)a1, 36))));
    (a1 = (uint64_t)((int64_t)(int32_t)((int32_t)SLL32((uint32_t)a2, 4))));
    (a3 = (uint64_t)((int64_t)(int32_t)((int32_t)SLL32((uint32_t)v1, 4))));
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)a0, (uint32_t)v0))));
    (t2 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)a0, 72))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)t2, 8))));
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)t2, 20))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v0, (uint32_t)a2))));
    (a0 = (uint64_t)(uint32_t)((uint8_t)A1_R8B((uint32_t)v0, 0)));
    (s0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v1, (uint32_t)a1))));
    { const bool c = (int64_t)a0 != 0; (v0 = (uint64_t)((uint64_t)RC_U64X(s0) + (uint64_t)0ULL)); if (c) goto L_11193b4; }
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)t1, 388))));
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)v0, 32))));
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)v1, 8))));
    (a3 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)a0, (uint32_t)a3))));
    (v0 = (uint64_t)(uint32_t)((uint8_t)A1_R8B((uint32_t)a3, 9)));
    { const bool c = (int64_t)v0 == 0; (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)SLL32((uint32_t)a2, 3)))); if (c) goto L_1119340; }
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)a3, 24))));
    t3 = sp + 0x10u; a3xw8 = true;
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)a0, (uint32_t)v0))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int16_t)A1_R16B((uint32_t)a0, 0))));
    A1_W32B(sp, 32, (uint32_t)v0);
    (v1 = (uint64_t)((int64_t)(int32_t)((int16_t)A1_R16B((uint32_t)a0, 2))));
    A1_W32B(sp, 36, (uint32_t)v1);
    (v0 = (uint64_t)((int64_t)(int32_t)((int16_t)A1_R16B((uint32_t)a0, 4))));
    (v1 = (uint64_t)(A1_R64B(sp, 32)));
    A1_W32B(sp, 40, (uint32_t)v0);
    (a1 = (uint64_t)(uint32_t)((uint16_t)A1_R16B((uint32_t)a0, 6)));
    A1_W64B(sp, 16, v1);
    A1_W32B(sp, 44, (uint32_t)a1);
    (v1 = (uint64_t)(A1_R64B(sp, 40)));
    A1_W64B(sp, 24, v1);
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B(sp, 28))));
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)a3, 12))));
    (v0 = (uint64_t)((int64_t)(int32_t)(SRA32((int32_t)v0, 1))));
    A1_W32B(sp, 28, (uint32_t)v0);
    vf30 = a4v::castsi128_ps(A4_R128(ADD32(t3, 0)));
    { a4::q128 src = a4v::castps_si128(vf30); a4::vec4 res = a4v::cvtepi32_ps(src); res = a4v::mul_ps(res, a4v::set1_ps(3.05175781e-05f)); a4::q128 mask = a4v::set_epi32(-1, -1, -1, -1); vf29 = a4v::blendv_ps(vf29, res, a4v::castsi128_ps(mask)); }
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v1, 16))));
    vf30 = a4v::castsi128_ps(A4_R128(ADD32((uint32_t)v0, 0)));
    { a4::vec4 res = a4v::mul(vf29, vf30); a4::q128 mask = a4v::set_epi32(-1, -1, -1, -1); vf31 = a4v::blend(vf31, res, a4v::castsi128_ps(mask)); }
    vf30 = a4v::castsi128_ps(A4_R128(ADD32((uint32_t)v1, 0)));
    { a4::q128 mask = a4v::set_epi32(-1, -1, -1, -1); vf29 = a4v::blendv_ps(vf29, vf31, a4v::castsi128_ps(mask)); }
    { a4::q128 mask = a4v::set_epi32(-1, -1, -1, -1); vf31 = a4v::blendv_ps(vf31, vf29, a4v::castsi128_ps(mask)); }
    { a4::vec4 res = a4v::addK<1, 0>(vf29, vf30); a4::q128 mask = a4v::set_epi32(0, -1, -1, -1); vf31 = a4v::blend(vf31, res, a4v::castsi128_ps(mask)); }
    A4_W128(ADD32(sp, 0), a4v::castps_si128(vf31));
    (a3 = (uint64_t)((uint64_t)a1_sx64(sp) + (uint64_t)0ULL));
    goto L_111934c;
L_1119340:;
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)a3, 24))));
    t3 = sp + 0x10u; a3xw8 = true;
    (a3 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v0, (uint32_t)a1))));
L_111934c:;
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)t1, 348))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)SLL32((uint32_t)t0, 6))));
    { uint32_t bits = A1_R32B((uint32_t)a3, 12); float f; std::memcpy(&f, &bits, sizeof(f)); f0 = f; }
    { uint32_t bits = A1_R32B((uint32_t)t1, 156); float f; std::memcpy(&f, &bits, sizeof(f)); f1 = f; }
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v0, (uint32_t)a0))));
    do { const a4::q128 _v = (A4_R128(ADD32((uint32_t)a3, 0))); h3 = a4::hi64(_v); std::memcpy(&v1, &_v, 8); } while (0);
    (at = (uint32_t)((int32_t)((int32_t)((uint32_t)16256 << 16))));
    { uint32_t bits = a7_opq(at); std::memcpy(&f2, &bits, sizeof(bits)); } a3xw0 = true;
    f0 = a1f::mul(f0, f1);
    A4_W128(ADD32(sp, 16), a4::mk128(h3, v1));
    { float f = f2; uint32_t bits; std::memcpy(&bits, &f, sizeof(bits)); A1_W32B(sp, 28, bits); }
    vf8 = a4v::castsi128_ps(A4_R128(ADD32(t3, 0)));
    vf14 = a4v::castsi128_ps(A4_R128(ADD32((uint32_t)v0, 0)));
    vf15 = a4v::castsi128_ps(A4_R128(ADD32((uint32_t)v0, 16)));
    vf16 = a4v::castsi128_ps(A4_R128(ADD32((uint32_t)v0, 32)));
    vf17 = a4v::castsi128_ps(A4_R128(ADD32((uint32_t)v0, 48)));
    { a4::vec4 res = a4v::mul(vf14, a4v::bcast<0>(vf8)); vacc = a4v::dest<1, 1, 1, 1>(vacc, res); }
    { a4::vec4 mul_res = a4v::mul(vf15, a4v::bcast<1>(vf8)); a4::vec4 res = a4v::addK<1, 1>(vacc, mul_res); vacc = a4v::dest<1, 1, 1, 1>(vacc, res); }
    { a4::vec4 mul_res = a4v::mul(vf16, a4v::bcast<2>(vf8)); a4::vec4 res = a4v::addK<1, 1>(vacc, mul_res); vacc = a4v::dest<1, 1, 1, 1>(vacc, res); }
    { a4::vec4 mul_res = a4v::mul(vf17, a4v::bcast<3>(vf8)); a4::vec4 res = a4v::addK<1, 1>(vacc, mul_res); a4::q128 mask = a4v::set_epi32(-1, -1, -1, -1); vf10 = a4v::blendv_ps(vf10, res, a4v::castsi128_ps(mask)); }
    A4_W128(ADD32((uint32_t)s0, 0), a4v::castps_si128(vf10));
    { float f = f0; uint32_t bits; std::memcpy(&bits, &f, sizeof(bits)); A1_W32B((uint32_t)s0, 12, bits); }
    (a0 = (uint64_t)((uint64_t)t2 + (uint64_t)0ULL));
    { (a1 = (uint64_t)((uint64_t)a2 + (uint64_t)0ULL)); A3_CHARGE(); uint64_t a7c[6] = {0u, 0u, 0u, 0u, 0u, 0u}; v0 = EntryModifiedManager_x8(ram, rt, a0, (uint32_t)a1, a7c); a0 = a7c[0]; a2 = a7c[1]; a3 = a7c[2]; t0 = a7c[3]; t1 = a7c[4]; t2 = a7c[5]; a3xw3 = true; }
    (v0 = (uint64_t)((uint64_t)s0 + (uint64_t)0ULL));
L_11193b4:;
    { sp = sp + 0x40u; if (xo) { if (a3xw0) xo[0] = a1_bits(f2); xo[1] = a0; xo[2] = a1; if (a3xw3) xo[3] = a2; xo[4] = a3; xo[5] = t0; xo[6] = t1; xo[7] = t2; if (a3xw8) xo[8] = a1_sx64(t3); } io_vf8 = vf8; io_vf10 = vf10; io_vf14 = vf14; io_vf15 = vf15; io_vf16 = vf16; io_vf17 = vf17; io_vf29 = vf29; io_vf30 = vf30; io_vf31 = vf31; io_vacc = vacc; io_h3 = h3; return v0; }
    __builtin_unreachable();
}

// set_npe_blend_ratio_particle_emitter_x8 (0x113f878): typed by tools/typed_lift2.py (round A6; frame calls, extra outputs xo[0]=f20 xo[1]=a0; VU0 in -, out -, frame 16 bytes, 2 slot accesses)
inline uint64_t set_npe_blend_ratio_particle_emitter_x8(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint64_t v0, uint64_t a1, float f12, float f13, float f14, float f15, float f16, float f17, float f18, float f19, float f20, uint64_t *xo = nullptr)
{
    uint64_t v1 = 0;
    uint64_t a0 = 0;
    uint8_t a2fr[16];
    (a0 = (uint64_t)((uint64_t)a1 + (uint64_t)0ULL));
    a2::frSt<uint32_t>(a2fr, 8, a1_bits(f20));
    { f20 = ((float)(f12)); A3_CHARGE(); v0 = get_particle_emitter_from_emitter_id(ram, rt, a0); }
    (v1 = (uint64_t)((uint64_t)v0 + (uint64_t)0ULL));
    if ((int64_t)v1 == 0) goto L_113f8a4;
    { float f = f20; uint32_t bits; std::memcpy(&bits, &f, sizeof(bits)); A1_W32B((uint32_t)v1, 364, bits); }
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 1))));
L_113f8a4:;
    f20 = a1_flt(a2::frLd<uint32_t>(a2fr, 8));
    { if (xo) { xo[0] = a1_bits(f20); xo[1] = a0; } return v0; }
    __builtin_unreachable();
}

// end_particle_emitter_x8 (0x113f8b0): typed by tools/typed_lift2.py (round A6; frame calls, extra outputs xo[0]=a0; VU0 in -, out -, frame 16 bytes, 0 slot accesses)
inline uint64_t end_particle_emitter_x8(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint64_t v0, uint64_t a1, uint64_t a2, float f12, float f13, float f14, float f15, float f16, float f17, float f18, float f19, uint64_t *xo = nullptr)
{
    uint64_t v1 = 0;
    uint64_t a0 = 0;
    uint64_t s0 = 0;
    (a0 = (uint64_t)((uint64_t)a1 + (uint64_t)0ULL));
    { (s0 = (uint64_t)((uint64_t)a2 + (uint64_t)0ULL)); A3_CHARGE(); v0 = get_particle_emitter_from_emitter_id(ram, rt, a0); }
    (v1 = (uint64_t)((uint64_t)v0 + (uint64_t)0ULL));
    { const bool c = (int64_t)v1 == 0; (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 1)))); if (c) goto L_113f8e4; }
    { const bool c = (int64_t)s0 != (int64_t)a0; A1_W32B((uint32_t)v1, 276, (uint32_t)a0); if (c) goto L_113f8e0; }
    A1_W32B((uint32_t)v1, 196, 0u);
L_113f8e0:;
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 1))));
L_113f8e4:;
    { if (xo) { xo[0] = a0; } return v0; }
    __builtin_unreachable();
}

// MultiGlobalQuaternionToRootSkeleton (0x10b7690): typed by tools/typed_lift2.py (round A6; loop leaf, extra outputs xo[0]=v0 xo[1]=a3 xo[2]=t0 xo[3]=t1 xo[4]=t2; VU0 in vf13 vf14 vf15 vf16 vacc, out vf11 vf12 vf13 vf14 vf15 vf16 vacc)
inline void MultiGlobalQuaternionToRootSkeleton(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint64_t a0, uint32_t a1, uint32_t a2, a4::vec4 &io_vf11, a4::vec4 &io_vf12, a4::vec4 &io_vf13, a4::vec4 &io_vf14, a4::vec4 &io_vf15, a4::vec4 &io_vf16, a4::vec4 &io_vacc, uint64_t *xo = nullptr)
{
    uint32_t v0 = 0;
    uint32_t v1 = 0;
    uint64_t a3 = 0;
    uint64_t t0 = 0;
    uint32_t t1 = 0;
    uint32_t t2 = 0;
    a4::vec4 vf11 = io_vf11;
    a4::vec4 vf12 = io_vf12;
    a4::vec4 vf13 = io_vf13;
    a4::vec4 vf14 = io_vf14;
    a4::vec4 vf15 = io_vf15;
    a4::vec4 vf16 = io_vf16;
    a4::vec4 vacc = io_vacc;
    bool a3xw0 = false;
    bool a3xw1 = false;
    bool a3xw4 = false;
    t1 = A1_R32B(a2, 4);
    { const bool c = (int32_t)t1 <= 0; (t0 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL)); if (c) goto L_10b7704; }
    (a3 = (uint64_t)((uint64_t)a0 + (uint64_t)0ULL)); a3xw1 = true;
    t2 = 0xFFFFFFFFu; a3xw4 = true;
L_10b76a8:;
    v0 = A1_R32B(a2, 8); a3xw0 = true;
    (v1 = (uint32_t)((int32_t)((int32_t)SLL32((uint32_t)t0, 6))));
    v1 = v1 + v0;
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B(v1, 56))));
    if ((int64_t)a0 != (int64_t)(int32_t)t2) { (t0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)t0, 1)))); goto L_10b76f8; }
    vf11 = a4v::castsi128_ps(A4_R128(ADD32(a1, 0)));
    vf12 = a4v::castsi128_ps(A4_R128(ADD32((uint32_t)a3, 0)));
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
    A4_W128(ADD32((uint32_t)a3, 0), a4v::castps_si128(vf13));
    (t0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)t0, 1))));
L_10b76f8:;
    (v0 = (uint32_t)(((int64_t)(int64_t)t0 < (int64_t)(int64_t)a1_sx64(t1)) ? 1 : 0)); a3xw0 = true;
    { const bool c = v0 != 0; (a3 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)a3, 16)))); a3xw1 = true; if (c) { A3_BACK(); goto L_10b76a8; } }
L_10b7704:;
    { if (xo) { if (a3xw0) xo[0] = a1_sx64(v0); if (a3xw1) xo[1] = a3; xo[2] = t0; xo[3] = a1_sx64(t1); if (a3xw4) xo[4] = a1_sx64(t2); } io_vf11 = vf11; io_vf12 = vf12; io_vf13 = vf13; io_vf14 = vf14; io_vf15 = vf15; io_vf16 = vf16; io_vacc = vacc; return; }
    __builtin_unreachable();
}

// GetGeoDispClass (0x10ed688): typed by tools/typed_lift2.py (round A6; leaf; VU0 in -, out h2)
inline uint64_t GetGeoDispClass(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint64_t a0, uint64_t &io_h2)
{
    uint64_t v0 = 0;
    uint64_t v1 = 0;
    uint64_t h2 = io_h2;
    (v0 = (uint64_t)((uint64_t)a0 + (uint64_t)0ULL));
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 36))));
    (v1 = (uint64_t)(uint32_t)((uint8_t)A1_R8B((uint32_t)v0, 8)));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v0, 32))));
    (v1 = (uint64_t)(RC_U64X(v1) ^ RC_U64X(a0)));
    { if (RC_U64X(v1) != 0) do { const a4::q128 _v = (a4v::setzero_si128()); h2 = a4::hi64(_v); std::memcpy(&v0, &_v, 8); } while (0); io_h2 = h2; return v0; }
    __builtin_unreachable();
}

// DebugDumpClothResources (0x11391b8): typed by tools/typed_lift2.py (round A6; loop leaf; VU0 in -, out -)
inline uint32_t DebugDumpClothResources(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t a0)
{
    uint32_t v0 = 0;
    uint32_t v1 = 0;
    uint32_t a1 = 0;
    uint32_t a2 = 0;
    uint32_t a3 = 0;
    uint32_t t0 = 0;
    uint32_t t1 = 0;
    uint32_t t2 = 0;
    v0 = (uint32_t)(int16_t)A1_R16(a0);
    if ((int32_t)v0 < 0) goto L_11392b4;
    v1 = A1_R32B(a0, 4);
L_11391c8:;
    { const bool c = (int32_t)v1 <= 0; t0 = 0u; if (c) goto L_11392a4; }
    v0 = A1_R32B(a0, 40);
    t2 = v1;
    t1 = A1_R32B(a0, 44);
    a3 = 0u;
    a1 = v0;
    v1 = v0 + 0x14u;
    a2 = 0u;
L_11391f0:;
    v0 = A1_R32(v1 - 8u);
    if ((int32_t)v0 <= 0) { v0 = A1_R32(v1); goto L_1139228; }
    v0 = a2 + a1;
    v0 = A1_R32B(v0, 12);
    v0 = v0 - 1u;
L_1139208:;
    { const bool c = v0 != 0; v0 = v0 - 1u; if (c) { A3_BACK(); goto L_1139208; } }
    v0 = A1_R32(v1);
L_1139228:;
    { const bool c = (int32_t)v0 <= 0; v0 = a3 + a1; if (c) goto L_1139254; }
    v0 = A1_R32B(v0, 20);
    v0 = v0 - 1u;
L_1139238:;
    { const bool c = v0 != 0; v0 = v0 - 1u; if (c) { A3_BACK(); goto L_1139238; } }
L_1139254:;
    if ((int32_t)t1 < 0) { t0 = t0 + 1u; goto L_1139290; }
    v0 = A1_R32B(a0, 8);
    if ((int32_t)v0 <= 0) { t0 = t0 + 1u; goto L_1139290; }
    v0 = v0 - 1u;
L_1139270:;
    { const bool c = v0 != 0; v0 = v0 - 1u; if (c) { A3_BACK(); goto L_1139270; } }
    t0 = t0 + 1u;
L_1139290:;
    a3 = a3 + 0x24u;
    v0 = (int32_t)t0 < (int32_t)t2 ? 1u : 0u;
    v1 = v1 + 0x24u;
    { const bool c = v0 != 0; a2 = a2 + 0x24u; if (c) { A3_BACK(); goto L_11391f0; } }
L_11392a4:;
    a0 = a0 + 0x34u;
    v0 = (uint32_t)(int16_t)A1_R16(a0);
    if ((int32_t)v0 >= 0) { v1 = A1_R32B(a0, 4); { A3_BACK(); goto L_11391c8; } }
L_11392b4:;
    { return v0; }
    __builtin_unreachable();
}

// get_cur_camera_context (0x1160f68): typed by tools/typed_lift2.py (round A6; leaf; VU0 in -, out -)
inline uint32_t get_cur_camera_context(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt)
{
    uint32_t v0 = 0;
    uint32_t v1 = 0;
    v0 = 0x1190000u;
    v1 = A1_R32(v0 - 0x1B4u);
    v1 = v1 << 2;
    v0 = 0x1230000u;
    v0 = v0 + v1;
    v0 = A1_R32(v0 - 0x5710u);
    { return v0; }
    __builtin_unreachable();
}

// gamecore_sub_01462628 (0x1462628): typed by tools/typed_lift2.py (round A6; loop guest frame leaf; VU0 in vf0 vf7 vacc vstat h4 h9 h10, out vf5 vf6 vf7 vacc vq vstat h2 h3)
inline a1::RetVF gamecore_sub_01462628(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint64_t a0, uint32_t sp, const a4::vec4 &io_vf0, a4::vec4 &io_vf5, a4::vec4 &io_vf6, a4::vec4 &io_vf7, a4::vec4 &io_vacc, float &io_vq, uint16_t &io_vstat, uint64_t &io_h2, uint64_t &io_h3, uint64_t io_h4, uint64_t io_h9, uint64_t io_h10)
{
    float f0 = 0.0f;
    float f1 = 0.0f;
    uint32_t at = 0;
    uint64_t v0 = 0;
    uint32_t v1 = 0;
    uint32_t a1 = 0;
    uint32_t a2 = 0;
    uint32_t a3 = 0;
    uint32_t t0 = 0;
    uint32_t t1 = 0;
    uint32_t t2 = 0;
    uint32_t t3 = 0;
    uint32_t t4 = 0;
    uint64_t t5 = 0;
    uint64_t t6 = 0;
    uint64_t t7 = 0;
    uint32_t t8 = 0;
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
    uint64_t h4 = io_h4;
    uint64_t h9 = io_h9;
    uint64_t h10 = io_h10;
    sp = sp - 0x20u;
    at = 0x3F800000u;
    { uint32_t bits = a7_opq(at); std::memcpy(&f1, &bits, sizeof(bits)); }
    (t5 = (uint64_t)((uint64_t)a0 + (uint64_t)0ULL));
    (t6 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL));
    (t7 = (uint64_t)((uint64_t)a1_sx64(sp) + (uint64_t)0ULL));
    t4 = sp + 0x10u;
    t8 = 0x1Fu;
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)t5, 1540))));
L_1462650:;
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)SUB32((uint32_t)a0, (uint32_t)t6))));
    (a3 = (uint32_t)((int32_t)((int32_t)ADD32((uint32_t)a0, 33))));
    (t0 = (uint32_t)((int32_t)((int32_t)ADD32((uint32_t)a0, 32))));
    (t2 = (uint32_t)((int32_t)((int32_t)ADD32((uint32_t)a0, 63))));
    (t1 = (uint32_t)((int32_t)((int32_t)ADD32((uint32_t)a0, 64))));
    a2 = (int32_t)a3 < 0 ? 1u : 0u;
    a1 = (int32_t)t0 < 0 ? 1u : 0u;
    v1 = a3;
    (v0 = (uint64_t)((uint64_t)a1_sx64(t0) + (uint64_t)0ULL));
    if (a1_sx64(a1) != 0) do { const a4::q128 _v = (a4::mk128(h10, a1_sx64(t2))); h2 = a4::hi64(_v); std::memcpy(&v0, &_v, 8); } while (0);
    a1 = (int32_t)t2 < 0 ? 1u : 0u;
    if (a1_sx64(a2) != 0) do { const a4::q128 _v = (a4::mk128(h9, a1_sx64(t1))); h3 = a4::hi64(_v); v1 = (uint32_t)a4::lo64(_v); } while (0);
    (t3 = (uint32_t)((int32_t)(SRA32((int32_t)v0, 5))));
    t1 = (uint32_t)((int32_t)v1 >> 5);
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)SLL32(t3, 5))));
    v1 = t1 << 5;
    (t3 = (uint32_t)((int32_t)((int32_t)SUB32(t0, (uint32_t)v0))));
    t1 = a3 - v1;
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)a0, 94))));
    (v0 = (uint64_t)((uint64_t)a1_sx64(t2) + (uint64_t)0ULL));
    a2 = t3 << 1;
    if (a1_sx64(a1) != 0) do { const a4::q128 _v = (a4::mk128(h4, a0)); h2 = a4::hi64(_v); std::memcpy(&v0, &_v, 8); } while (0);
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(a2, t3))));
    v1 = t1 << 1;
    (a1 = (uint32_t)((int32_t)(SRA32((int32_t)v0, 5))));
    v1 = v1 + t1;
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)SLL32(a1, 5))));
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)SLL32((uint32_t)a0, 4))));
    v1 = v1 << 4;
    (a1 = (uint32_t)((int32_t)((int32_t)SUB32(t2, (uint32_t)v0))));
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)a0, (uint32_t)t5))));
    (v1 = (uint32_t)((int32_t)((int32_t)ADD32(v1, (uint32_t)t5))));
    vf5 = a4v::castsi128_ps(A4_R128(ADD32((uint32_t)a0, 0)));
    vf6 = a4v::castsi128_ps(A4_R128(ADD32(v1, 0)));
    { a4::vec4 res = a4v::sub(vf5, vf6); a4::q128 mask = a4v::set_epi32(-1, -1, -1, -1); vf7 = a4v::blend(vf7, res, a4v::castsi128_ps(mask)); }
    A4_W128(ADD32((uint32_t)t7, 0), a4v::castps_si128(vf7));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)SLL32(a1, 1))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v0, a1))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)SLL32((uint32_t)v0, 4))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v0, (uint32_t)t5))));
    vf5 = a4v::castsi128_ps(A4_R128(ADD32((uint32_t)v0, 0)));
    vf6 = a4v::castsi128_ps(A4_R128(ADD32((uint32_t)a0, 0)));
    { a4::vec4 res = a4v::sub(vf5, vf6); a4::q128 mask = a4v::set_epi32(-1, -1, -1, -1); vf7 = a4v::blend(vf7, res, a4v::castsi128_ps(mask)); }
    A4_W128(ADD32(t4, 0), a4v::castps_si128(vf7));
    { const bool c = (int64_t)t6 != 0; v1 = a2 + t3; if (c) goto L_1462718; }
    do { const a4::q128 _v = (A4_R128(ADD32(sp, 16))); h2 = a4::hi64(_v); std::memcpy(&v0, &_v, 8); } while (0);
    A4_W128(ADD32(sp, 0), a4::mk128(h2, v0));
    goto L_1462728;
L_1462718:;
    if ((int64_t)t6 != (int64_t)(int32_t)t8) { v1 = v1 << 4; goto L_146272c; }
    do { const a4::q128 _v = (A4_R128(ADD32(sp, 0))); h2 = a4::hi64(_v); std::memcpy(&v0, &_v, 8); } while (0);
    A4_W128(ADD32(sp, 16), a4::mk128(h2, v0));
L_1462728:;
    v1 = v1 << 4;
L_146272c:;
    (v1 = (uint32_t)((int32_t)((int32_t)ADD32(v1, (uint32_t)t5))));
    v1 = v1 + 0x20u;
    vf5 = a4v::castsi128_ps(A4_R128(ADD32((uint32_t)t7, 0)));
    vf6 = a4v::castsi128_ps(A4_R128(ADD32(t4, 0)));
    { a4::vec4 res = a4v::add(vf5, vf6); a4::q128 mask = a4v::set_epi32(0, -1, -1, -1); vf5 = a4v::blend(vf5, res, a4v::castsi128_ps(mask)); }
    A4_W128(ADD32(v1, 0), a4v::castps_si128(vf5));
    vf5 = a4v::castsi128_ps(A4_R128(ADD32(v1, 0)));
    { a4::vec4 res = a4v::mul(vf5, vf5); a4::q128 mask = a4v::set_epi32(0, -1, -1, -1); vf7 = a4v::blend(vf7, res, a4v::castsi128_ps(mask)); }
    { a4::vec4 res = a4v::mulK<1, 1>(vf0, a4v::bcast<0>(vf7)); vacc = a4v::dest<0, 0, 0, 1>(vacc, res); }
    { a4::vec4 mul_res = a4v::mulK<1, 1>(vf0, a4v::bcast<1>(vf7)); a4::vec4 res = a4v::addK<1, 1>(vacc, mul_res); vacc = a4v::dest<0, 0, 0, 1>(vacc, res); }
    { a4::vec4 mul_res = a4v::mulK<1, 1>(vf0, a4v::bcast<2>(vf7)); a4::vec4 res = a4v::addK<1, 1>(vacc, mul_res); a4::q128 mask = a4v::set_epi32(-1, 0, 0, 0); vf7 = a4v::blendv_ps(vf7, res, a4v::castsi128_ps(mask)); }
    { float fs = a4v::cvtss_f32(a4v::shuffle_ps<0x03>(vf0, vf0)); float ft = a4v::cvtss_f32(a4v::shuffle_ps<0x03>(vf7, vf7)); vq = a4v::rsqrt(vstat, fs, ft); }
    { a4::vec4 res = a4v::mul(vf5, a4v::set1_ps(vq)); a4::q128 mask = a4v::set_epi32(0, -1, -1, -1); vf5 = a4v::blendv_ps(vf5, res, a4v::castsi128_ps(mask)); }
    A4_W128(ADD32(v1, 0), a4v::castps_si128(vf5));
    vf5 = a4v::castsi128_ps(A4_R128(ADD32(t4, 0)));
    { a4::vec4 res = a4v::mul(vf5, vf5); a4::q128 mask = a4v::set_epi32(0, -1, -1, -1); vf7 = a4v::blend(vf7, res, a4v::castsi128_ps(mask)); }
    { a4::vec4 res = a4v::mulK<1, 1>(vf0, a4v::bcast<0>(vf7)); vacc = a4v::dest<0, 0, 0, 1>(vacc, res); }
    { a4::vec4 mul_res = a4v::mulK<1, 1>(vf0, a4v::bcast<1>(vf7)); a4::vec4 res = a4v::addK<1, 1>(vacc, mul_res); vacc = a4v::dest<0, 0, 0, 1>(vacc, res); }
    { a4::vec4 mul_res = a4v::mulK<1, 1>(vf0, a4v::bcast<2>(vf7)); a4::vec4 res = a4v::addK<1, 1>(vacc, mul_res); a4::q128 mask = a4v::set_epi32(-1, 0, 0, 0); vf7 = a4v::blendv_ps(vf7, res, a4v::castsi128_ps(mask)); }
    { float fs = a4v::cvtss_f32(a4v::shuffle_ps<0x03>(vf0, vf0)); float ft = a4v::cvtss_f32(a4v::shuffle_ps<0x03>(vf7, vf7)); vq = a4v::rsqrt(vstat, fs, ft); }
    { a4::vec4 res = a4v::mul(vf5, a4v::set1_ps(vq)); a4::q128 mask = a4v::set_epi32(0, -1, -1, -1); vf5 = a4v::blendv_ps(vf5, res, a4v::castsi128_ps(mask)); }
    A4_W128(ADD32(t4, 0), a4v::castps_si128(vf5));
    vf5 = a4v::castsi128_ps(A4_R128(ADD32(t4, 0)));
    vf6 = a4v::castsi128_ps(A4_R128(ADD32(v1, 0)));
    { a4::vec4 res = a4v::addK<1, 1>(vf0, a4v::bcast<3>(vf0)); a4::q128 mask = a4v::set_epi32(0, 0, 0, -1); vf7 = a4v::blendv_ps(vf7, res, a4v::castsi128_ps(mask)); }
    { a4::vec4 res = a4v::mul(vf5, vf6); a4::q128 mask = a4v::set_epi32(0, -1, -1, -1); vf6 = a4v::blend(vf6, res, a4v::castsi128_ps(mask)); }
    { a4::vec4 res = a4v::addK<1, 1>(vf0, a4v::bcast<0>(vf6)); vacc = a4v::dest<1, 0, 0, 0>(vacc, res); }
    { a4::vec4 mul_res = a4v::mulK<1, 1>(vf7, a4v::bcast<1>(vf6)); a4::vec4 res = a4v::addK<1, 1>(vacc, mul_res); vacc = a4v::dest<1, 0, 0, 0>(vacc, res); }
    { a4::vec4 mul_res = a4v::mulK<1, 1>(vf7, a4v::bcast<2>(vf6)); a4::vec4 res = a4v::addK<1, 1>(vacc, mul_res); a4::q128 mask = a4v::set_epi32(0, 0, 0, -1); vf6 = a4v::blendv_ps(vf6, res, a4v::castsi128_ps(mask)); }
    do { const a4::q128 _v = (a4v::castps_si128(vf6)); h2 = a4::hi64(_v); std::memcpy(&v0, &_v, 8); } while (0);
    { uint32_t bits = (uint32_t)v0; std::memcpy(&f0, &bits, sizeof(bits)); }
    f0 = a1f::div(fcr31, f1, f0);
    { uint32_t bits; std::memcpy(&bits, &f0, sizeof(bits)); (v0 = (uint64_t)((int64_t)(int32_t)(bits))); }
    vf5 = a4v::castsi128_ps(A4_R128(ADD32(v1, 0)));
    vf6 = a4v::castsi128_ps(a4::mk128(h2, v0));
    { a4::vec4 res = a4v::mul(vf5, a4v::bcast<0>(vf6)); a4::q128 mask = a4v::set_epi32(0, -1, -1, -1); vf5 = a4v::blendv_ps(vf5, res, a4v::castsi128_ps(mask)); }
    A4_W128(ADD32(v1, 0), a4v::castps_si128(vf5));
    (t6 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)t6, 1))));
    (v0 = (uint64_t)(((int64_t)(int64_t)RC_U64X(t6) < (int64_t)(int32_t)32) ? 1 : 0));
    if ((int64_t)v0 != 0) { (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)t5, 1540)))); { A3_BACK(); goto L_1462650; } }
    { sp = sp + 0x20u; io_vf5 = vf5; io_vf6 = vf6; io_vf7 = vf7; io_vacc = vacc; io_vq = vq; io_vstat = vstat; io_h2 = h2; io_h3 = h3; return a1::RetVF{(uint64_t)v0, f0}; }
    __builtin_unreachable();
}

// LoaderSysPrintf (0x102cf8): typed by tools/typed_lift2.py (round A6; guest frame calls, extra outputs xo[0]=a0 xo[1]=a1 xo[2]=a2; VU0 in -, out h2)
inline uint64_t LoaderSysPrintf(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint64_t a0, uint64_t a1, uint64_t a2, uint64_t a3, uint64_t t0, uint64_t t1, uint64_t t2, uint64_t t3, uint64_t sp, float f12, float f13, float f14, float f15, float f16, float f17, float f18, float f19, uint64_t &io_h2, uint64_t *xo = nullptr)
{
    uint64_t v0 = 0;
    uint64_t h2 = io_h2;
    (sp = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)sp, 4294966928))));
    (v0 = (uint64_t)((uint64_t)a0 + (uint64_t)0ULL));
    A1_W64B((uint32_t)sp, 312, a1);
    (a1 = (uint64_t)((uint64_t)v0 + (uint64_t)0ULL));
    (a0 = (uint64_t)((uint64_t)RC_U64X(sp) + (uint64_t)0ULL));
    A1_W64B((uint32_t)sp, 320, a2);
    (a2 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)sp, 312))));
    A1_W64B((uint32_t)sp, 328, a3);
    A1_W64B((uint32_t)sp, 336, t0);
    A1_W64B((uint32_t)sp, 344, t1);
    A1_W64B((uint32_t)sp, 352, t2);
    A1_W64B((uint32_t)sp, 360, t3);
    { float f = f12; uint32_t bits; std::memcpy(&bits, &f, sizeof(bits)); A1_W32B((uint32_t)sp, 280, bits); }
    { float f = f13; uint32_t bits; std::memcpy(&bits, &f, sizeof(bits)); A1_W32B((uint32_t)sp, 284, bits); }
    { float f = f14; uint32_t bits; std::memcpy(&bits, &f, sizeof(bits)); A1_W32B((uint32_t)sp, 288, bits); }
    { float f = f15; uint32_t bits; std::memcpy(&bits, &f, sizeof(bits)); A1_W32B((uint32_t)sp, 292, bits); }
    { float f = f16; uint32_t bits; std::memcpy(&bits, &f, sizeof(bits)); A1_W32B((uint32_t)sp, 296, bits); }
    { float f = f17; uint32_t bits; std::memcpy(&bits, &f, sizeof(bits)); A1_W32B((uint32_t)sp, 300, bits); }
    { float f = f18; uint32_t bits; std::memcpy(&bits, &f, sizeof(bits)); A1_W32B((uint32_t)sp, 304, bits); }
    { { float f = f19; uint32_t bits; std::memcpy(&bits, &f, sizeof(bits)); A1_W32B((uint32_t)sp, 308, bits); } A3_CHARGE(); v0 = a1_sx64(a7x::vsprintf(ram, rt, (uint32_t)a0, (uint32_t)a1, (uint32_t)a2, h2)); }
    { (a0 = (uint64_t)((uint64_t)sp + (uint64_t)0ULL)); A3_CHARGE(); v0 = a1_sx64(a7x::printf(ram, rt, (uint32_t)a0, (uint32_t)a1, (uint32_t)a2, (uint32_t)a3, (uint32_t)t0, (uint32_t)t1, (uint32_t)t2, (uint32_t)t3, (uint32_t)sp, h2)); }
    { (a0 = (uint64_t)((uint64_t)sp + (uint64_t)0ULL)); A3_CHARGE(); v0 = a1_sx64(a6x::strlen(ram, rt, (uint32_t)a0, h2)); }
    { (sp = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)sp, 368)))); if (xo) { xo[0] = a0; xo[1] = a1; xo[2] = a2; } io_h2 = h2; return v0; }
    __builtin_unreachable();
}

// ps2___make_dp (0x125870): typed by tools/typed_lift2.py (round A6; guest frame calls; VU0 in -, out -, 64-bit inputs a2)
inline uint64_t ps2___make_dp(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint64_t a0, uint32_t a1, uint64_t w64_a2, uint64_t a3, uint32_t sp)
{
    uint64_t v0 = 0;
    uint32_t a2 = (uint32_t)w64_a2;
    sp = sp - 0x30u;
    A1_W32B(sp, 0, (uint32_t)a0);
    (a0 = (uint64_t)((uint64_t)a1_sx64(sp) + (uint64_t)0ULL));
    A1_W32B(sp, 4, a1);
    A1_W32B(sp, 8, a2);
    { A1_W64B(sp, 16, a3); A3_CHARGE(); v0 = ps2___pack_d(ram, rt, a0, w64_a2); }
    { sp = sp + 0x30u; return v0; }
    __builtin_unreachable();
}

// iosSPrintf (0x1b2920): typed by tools/typed_lift2.py (round A6; guest frame calls, extra outputs xo[0]=a2; VU0 in -, out h2)
inline uint32_t iosSPrintf(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t a0, uint32_t a1, uint64_t a2, uint64_t a3, uint64_t t0, uint64_t t1, uint64_t t2, uint64_t t3, uint32_t sp, float f12, float f13, float f14, float f15, float f16, float f17, float f18, float f19, uint64_t &io_h2, uint64_t *xo = nullptr)
{
    uint32_t v0 = 0;
    uint64_t h2 = io_h2;
    sp = sp - 0x60u;
    A1_W64B(sp, 48, a2);
    (a2 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(sp, 48))));
    A1_W64B(sp, 56, a3);
    A1_W64B(sp, 64, t0);
    A1_W64B(sp, 72, t1);
    A1_W64B(sp, 80, t2);
    A1_W64B(sp, 88, t3);
    A1_W32B(sp, 16, a1_bits(f12));
    A1_W32B(sp, 20, a1_bits(f13));
    A1_W32B(sp, 24, a1_bits(f14));
    A1_W32B(sp, 28, a1_bits(f15));
    A1_W32B(sp, 32, a1_bits(f16));
    A1_W32B(sp, 36, a1_bits(f17));
    A1_W32B(sp, 40, a1_bits(f18));
    { A1_W32B(sp, 44, a1_bits(f19)); A3_CHARGE(); v0 = a7x::vsprintf(ram, rt, a0, a1, (uint32_t)a2, h2); }
    { sp = sp + 0x60u; if (xo) { xo[0] = a2; } io_h2 = h2; return v0; }
    __builtin_unreachable();
}

// iosgluGetCameraContext (0x108f518): typed by tools/typed_lift2.py (round A6; frame calls; VU0 in -, out -, frame 16 bytes, 0 slot accesses)
inline uint32_t iosgluGetCameraContext(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, float f12, float f13, float f14, float f15, float f16, float f17, float f18, float f19)
{
    { return get_cur_camera_context(ram, rt); }
    __builtin_unreachable();
}

// gluGetLookAtPosition (0x108f618): typed by tools/typed_lift2.py (round A6; frame calls, extra outputs xo[0]=f20; VU0 in h4, out vf5 vf6, frame 48 bytes, 4 slot accesses)
inline uint32_t gluGetLookAtPosition(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t v0, uint64_t a0, float f12, float f13, float f14, float f15, float f16, float f17, float f18, float f19, float f20, a4::vec4 &io_vf5, a4::vec4 &io_vf6, uint64_t io_h4, uint64_t *xo = nullptr)
{
    uint32_t v1 = 0;
    uint64_t s0 = 0;
    a4::vec4 vf5 = io_vf5;
    a4::vec4 vf6 = io_vf6;
    uint64_t h4 = io_h4;
    uint8_t a2fr[48];
    (s0 = (uint64_t)((uint64_t)a0 + (uint64_t)0ULL));
    a2::frSt<uint32_t>(a2fr, 32, a1_bits(f20));
    { f20 = ((float)(f12)); A3_CHARGE(); v0 = get_cur_camera_context(ram, rt); }
    v1 = v0 + 0x150u;
    { uint32_t bits; std::memcpy(&bits, &f20, sizeof(bits)); (a0 = (uint64_t)((int64_t)(int32_t)(bits))); }
    vf5 = a4v::castsi128_ps(A4_R128(ADD32(v1, 0)));
    vf6 = a4v::castsi128_ps(a4::mk128(h4, a0));
    { a4::vec4 res = a4v::mul(vf5, a4v::bcast<0>(vf6)); a4::q128 mask = a4v::set_epi32(0, -1, -1, -1); vf5 = a4v::blendv_ps(vf5, res, a4v::castsi128_ps(mask)); }
    a2::frSt<a4::q128>(a2fr, 0, a4v::castps_si128(vf5));
    v0 = v0 + 0x140u;
    vf5 = a4v::castsi128_ps(A4_R128(ADD32(v0, 0)));
    vf6 = a4v::castsi128_ps(a2::frLd<a4::q128>(a2fr, 0));
    { a4::vec4 res = a4v::add(vf5, vf6); a4::q128 mask = a4v::set_epi32(0, -1, -1, -1); vf5 = a4v::blend(vf5, res, a4v::castsi128_ps(mask)); }
    A4_W128(ADD32((uint32_t)s0, 0), a4v::castps_si128(vf5));
    f20 = a1_flt(a2::frLd<uint32_t>(a2fr, 32));
    { if (xo) { xo[0] = a1_bits(f20); } io_vf5 = vf5; io_vf6 = vf6; return v0; }
    __builtin_unreachable();
}

// gluGetCameraSpaceXYZ (0x108f718): typed by tools/typed_lift2.py (round A6; frame calls; VU0 in vf0 vf7 vacc vstat, out vf5 vf6 vf7 vacc vq vstat h3, frame 32 bytes, 0 slot accesses)
inline uint32_t gluGetCameraSpaceXYZ(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t v0, uint64_t a0, uint64_t a1, uint64_t a2, float f12, float f13, float f14, float f15, float f16, float f17, float f18, float f19, const a4::vec4 &io_vf0, a4::vec4 &io_vf5, a4::vec4 &io_vf6, a4::vec4 &io_vf7, a4::vec4 &io_vacc, float &io_vq, uint16_t &io_vstat, uint64_t &io_h3)
{
    uint64_t v1 = 0;
    uint64_t s0 = 0;
    uint64_t s1 = 0;
    uint64_t s2 = 0;
    a4::vec4 vf0 = io_vf0;
    a4::vec4 vf5 = io_vf5;
    a4::vec4 vf6 = io_vf6;
    a4::vec4 vf7 = io_vf7;
    a4::vec4 vacc = io_vacc;
    float vq = io_vq;
    uint16_t vstat = io_vstat;
    uint64_t h3 = io_h3;
    (s0 = (uint64_t)((uint64_t)a0 + (uint64_t)0ULL));
    (s1 = (uint64_t)((uint64_t)a2 + (uint64_t)0ULL));
    { (s2 = (uint64_t)((uint64_t)a1 + (uint64_t)0ULL)); A3_CHARGE(); v0 = get_cur_camera_context(ram, rt); }
    do { const a4::q128 _v = (A4_R128(ADD32(v0, 336))); h3 = a4::hi64(_v); std::memcpy(&v1, &_v, 8); } while (0);
    { const uint32_t _dynInsn = A1_R32(0x108F740u); const int32_t _dynSimm = (int16_t)(_dynInsn & 0xFFFFu); const uint32_t _dynImm = _dynInsn & 0xFFFFu; (void)_dynSimm; (void)_dynImm; (v0 = (uint32_t)((int32_t)((int32_t)((uint32_t)_dynImm << 16)))); }
    { const uint32_t _dynInsn = A1_R32(0x108F744u); const int32_t _dynSimm = (int16_t)(_dynInsn & 0xFFFFu); const uint32_t _dynImm = _dynInsn & 0xFFFFu; (void)_dynSimm; (void)_dynImm; (v0 = (uint32_t)((int32_t)((int32_t)ADD32(v0, _dynSimm)))); }
    A4_W128(ADD32((uint32_t)s1, 0), a4::mk128(h3, v1));
    vf5 = a4v::castsi128_ps(A4_R128(ADD32(v0, 0)));
    vf6 = a4v::castsi128_ps(A4_R128(ADD32((uint32_t)s1, 0)));
    { a4::vec4 fs_yzx = a4v::shuffle_ps<0xC9>(vf5, vf5); a4::vec4 ft_zxy = a4v::shuffle_ps<0xD2>(vf6, vf6); a4::vec4 res = a4v::mul(fs_yzx, ft_zxy); vacc = a4v::dest<1, 1, 1, 0>(vacc, res); }
    { a4::vec4 fs_yzx = a4v::shuffle_ps<0xC9>(vf6, vf6); a4::vec4 ft_zxy = a4v::shuffle_ps<0xD2>(vf5, vf5); a4::vec4 mul_res = a4v::mul(fs_yzx, ft_zxy); a4::vec4 res = a4v::subK<1, 1>(vacc, mul_res); a4::q128 mask = a4v::set_epi32(0, -1, -1, -1); vf7 = a4v::blendv_ps(vf7, res, a4v::castsi128_ps(mask)); }
    { a4::vec4 res = a4v::sub(vf7, vf7); a4::q128 mask = a4v::set_epi32(-1, 0, 0, 0); vf7 = a4v::blend(vf7, res, a4v::castsi128_ps(mask)); }
    A4_W128(ADD32((uint32_t)s0, 0), a4v::castps_si128(vf7));
    vf5 = a4v::castsi128_ps(A4_R128(ADD32((uint32_t)s0, 0)));
    { a4::vec4 res = a4v::mul(vf5, vf5); a4::q128 mask = a4v::set_epi32(0, -1, -1, -1); vf7 = a4v::blend(vf7, res, a4v::castsi128_ps(mask)); }
    { a4::vec4 res = a4v::mulK<1, 1>(vf0, a4v::bcast<0>(vf7)); vacc = a4v::dest<0, 0, 0, 1>(vacc, res); }
    { a4::vec4 mul_res = a4v::mulK<1, 1>(vf0, a4v::bcast<1>(vf7)); a4::vec4 res = a4v::addK<1, 1>(vacc, mul_res); vacc = a4v::dest<0, 0, 0, 1>(vacc, res); }
    { a4::vec4 mul_res = a4v::mulK<1, 1>(vf0, a4v::bcast<2>(vf7)); a4::vec4 res = a4v::addK<1, 1>(vacc, mul_res); a4::q128 mask = a4v::set_epi32(-1, 0, 0, 0); vf7 = a4v::blendv_ps(vf7, res, a4v::castsi128_ps(mask)); }
    { float fs = a4v::cvtss_f32(a4v::shuffle_ps<0x03>(vf0, vf0)); float ft = a4v::cvtss_f32(a4v::shuffle_ps<0x03>(vf7, vf7)); vq = a4v::rsqrt(vstat, fs, ft); }
    { a4::vec4 res = a4v::mul(vf5, a4v::set1_ps(vq)); a4::q128 mask = a4v::set_epi32(0, -1, -1, -1); vf5 = a4v::blendv_ps(vf5, res, a4v::castsi128_ps(mask)); }
    A4_W128(ADD32((uint32_t)s0, 0), a4v::castps_si128(vf5));
    vf5 = a4v::castsi128_ps(A4_R128(ADD32((uint32_t)s1, 0)));
    vf6 = a4v::castsi128_ps(A4_R128(ADD32((uint32_t)s0, 0)));
    { a4::vec4 fs_yzx = a4v::shuffle_ps<0xC9>(vf5, vf5); a4::vec4 ft_zxy = a4v::shuffle_ps<0xD2>(vf6, vf6); a4::vec4 res = a4v::mul(fs_yzx, ft_zxy); vacc = a4v::dest<1, 1, 1, 0>(vacc, res); }
    { a4::vec4 fs_yzx = a4v::shuffle_ps<0xC9>(vf6, vf6); a4::vec4 ft_zxy = a4v::shuffle_ps<0xD2>(vf5, vf5); a4::vec4 mul_res = a4v::mul(fs_yzx, ft_zxy); a4::vec4 res = a4v::subK<1, 1>(vacc, mul_res); a4::q128 mask = a4v::set_epi32(0, -1, -1, -1); vf7 = a4v::blendv_ps(vf7, res, a4v::castsi128_ps(mask)); }
    { a4::vec4 res = a4v::sub(vf7, vf7); a4::q128 mask = a4v::set_epi32(-1, 0, 0, 0); vf7 = a4v::blend(vf7, res, a4v::castsi128_ps(mask)); }
    A4_W128(ADD32((uint32_t)s2, 0), a4v::castps_si128(vf7));
    { io_vf5 = vf5; io_vf6 = vf6; io_vf7 = vf7; io_vacc = vacc; io_vq = vq; io_vstat = vstat; io_h3 = h3; return v0; }
    __builtin_unreachable();
}

// gluUpdateCameraContext (0x108f7b8): typed by tools/typed_lift2.py (round A6; frame calls; VU0 in -, out -, frame 16 bytes, 0 slot accesses)
inline uint32_t gluUpdateCameraContext(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t v0, float f12, float f13, float f14, float f15, float f16, float f17, float f18, float f19)
{
    uint32_t v1 = 0;
    { A3_CHARGE(); v0 = get_cur_camera_context(ram, rt); }
    v1 = 0x1180000u;
    A1_W32B(v1, 25488, v0);
    { return v0; }
    __builtin_unreachable();
}

// PredictMotionOrientByScriptCtrl (0x10c9de8): typed by tools/typed_lift2.py (round A6; loop guest frame calls, extra outputs xo[0]=a3 xo[1]=t0 xo[2]=t1 xo[3]=t2 xo[4]=t3; VU0 in h3 h5, out h2 h5)
inline uint64_t PredictMotionOrientByScriptCtrl(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint64_t v0, uint64_t a0, uint64_t a1, uint64_t sp, float f12, float f13, float f14, float f15, float f16, float f17, float f18, float f19, uint64_t &io_h2, uint64_t io_h3, uint64_t &io_h5, uint64_t *xo = nullptr)
{
    uint64_t v1 = 0;
    uint64_t a2 = 0;
    uint64_t a3 = 0;
    uint64_t t0 = 0;
    uint64_t t1 = 0;
    uint64_t t2 = 0;
    uint64_t t3 = 0;
    uint64_t t4 = 0;
    uint64_t t5 = 0;
    uint64_t t6 = 0;
    uint64_t t7 = 0;
    uint64_t s0 = 0;
    uint64_t s1 = 0;
    uint64_t s2 = 0;
    uint64_t s3 = 0;
    uint64_t s4 = 0;
    uint64_t s5 = 0;
    uint64_t s6 = 0;
    uint64_t s7 = 0;
    uint64_t t8 = 0;
    uint64_t t9 = 0;
    uint64_t fp = 0;
    uint64_t hi = 0;
    uint64_t lo = 0;
    uint64_t h2 = io_h2;
    uint64_t h3 = io_h3;
    uint64_t h5 = io_h5;
    bool a3xw0 = false;
    bool a3xw1 = false;
    bool a3xw2 = false;
    bool a3xw3 = false;
    bool a3xw4 = false;
    (sp = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)sp, 4294967184))));
    { (s0 = (uint64_t)((uint64_t)a1 + (uint64_t)0ULL)); A3_CHARGE(); v0 = GetCharacterClass(ram, rt, a0, h2, h5); }
    if ((int64_t)v0 == 0) { (v0 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL)); goto L_10ca1fc; }
    (a1 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)v0, 1872))));
    { const bool c = (int64_t)a1 == 0; (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)3 << 16)))); if (c) goto L_10ca1f8; }
    (a2 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)a1, 0))));
    (v0 = (uint64_t)(RC_U64X(v0) | (uint64_t)(uint16_t)65535));
    (v0 = (uint64_t)(RC_U64X(a2) & RC_U64X(v0)));
    { const bool c = (int64_t)v0 == 0; (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)SUB32((uint32_t)a1, (uint32_t)v0)))); if (c) goto L_10c9e50; }
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)v0, 0))));
    goto L_10c9e54;
L_10c9e50:;
    (a0 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL));
L_10c9e54:;
    { const bool c = (int64_t)a0 == 0; { const uint32_t _dynInsn = A1_R32(0x10C9E58u); const int32_t _dynSimm = (int16_t)(_dynInsn & 0xFFFFu); const uint32_t _dynImm = _dynInsn & 0xFFFFu; (void)_dynSimm; (void)_dynImm; (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)_dynImm << 16)))); } if (c) goto L_10c9e70; }
    { const uint32_t _dynInsn = A1_R32(0x10C9E5Cu); const int32_t _dynSimm = (int16_t)(_dynInsn & 0xFFFFu); const uint32_t _dynImm = _dynInsn & 0xFFFFu; (void)_dynSimm; (void)_dynImm; (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32(ADD32((uint32_t)v0, _dynSimm))))); }
    if ((int64_t)a0 == (int64_t)v1) { (a3 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL)); a3xw0 = true; goto L_10c9e74; }
    (a3 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)a0, 12)))); a3xw0 = true;
    goto L_10c9e74;
L_10c9e70:;
    (a3 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL)); a3xw0 = true;
L_10c9e74:;
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)3 << 16))));
    (v0 = (uint64_t)(RC_U64X(v0) | (uint64_t)(uint16_t)65535));
    (v0 = (uint64_t)(a2 & RC_U64X(v0)));
    { const bool c = (int64_t)v0 == 0; (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)SUB32((uint32_t)a1, (uint32_t)v0)))); if (c) goto L_10c9e90; }
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)v0, 0))));
    goto L_10c9e94;
L_10c9e90:;
    (a0 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL));
L_10c9e94:;
    { const bool c = (int64_t)a0 == 0; { const uint32_t _dynInsn = A1_R32(0x10C9E98u); const int32_t _dynSimm = (int16_t)(_dynInsn & 0xFFFFu); const uint32_t _dynImm = _dynInsn & 0xFFFFu; (void)_dynSimm; (void)_dynImm; (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)_dynImm << 16)))); } if (c) goto L_10c9ec8; }
    { const uint32_t _dynInsn = A1_R32(0x10C9E9Cu); const int32_t _dynSimm = (int16_t)(_dynInsn & 0xFFFFu); const uint32_t _dynImm = _dynInsn & 0xFFFFu; (void)_dynSimm; (void)_dynImm; (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32(ADD32((uint32_t)v0, _dynSimm))))); }
    { const bool c = (int64_t)a0 == (int64_t)v1; (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)SRL32((uint32_t)a2, 18)))); if (c) goto L_10c9ec8; }
    (a1 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)a0, 8))));
    (v0 = (uint64_t)(RC_U64X(v0) & (uint64_t)(uint16_t)4095));
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)a0, 16))));
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)a1, 12))));
    { int64_t result = (int64_t)(int32_t)v0 * (int64_t)(int32_t)v1; lo = (uint64_t)(int64_t)(int32_t)result; hi = (uint64_t)(int64_t)(int32_t)(result >> 32); (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)result))); }
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)a0, (uint32_t)v0))));
    goto L_10c9ecc;
L_10c9ec8:;
    (a0 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL));
L_10c9ecc:;
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 1))));
    (fp = (uint64_t)((uint64_t)RC_U64X(a0) + (uint64_t)0ULL));
    (v0 = (uint64_t)(RC_U64X(v0) << ((uint32_t)s0 & 0x3F)));
    A1_W32B((uint32_t)sp, 0, 0u);
    A1_W64B((uint32_t)sp, 8, v0);
    { const bool c = (int64_t)a3 == 0; (t9 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)a3, 4294967295)))); if (c) goto L_10ca1ec; }
    (a3 = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)3 << 16)))); a3xw0 = true;
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)3 << 16))));
    (a3 = (uint64_t)(RC_U64X(a3) | (uint64_t)(uint16_t)65535)); a3xw0 = true;
    A1_W32B((uint32_t)sp, 16, (uint32_t)v0);
    { const uint32_t _dynInsn = A1_R32(0x10C9EF8u); const int32_t _dynSimm = (int16_t)(_dynInsn & 0xFFFFu); const uint32_t _dynImm = _dynInsn & 0xFFFFu; (void)_dynSimm; (void)_dynImm; (t2 = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)_dynImm << 16)))); } a3xw3 = true;
    { const uint32_t _dynInsn = A1_R32(0x10C9EFCu); const int32_t _dynSimm = (int16_t)(_dynInsn & 0xFFFFu); const uint32_t _dynImm = _dynInsn & 0xFFFFu; (void)_dynSimm; (void)_dynImm; (t3 = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)_dynImm << 16)))); } a3xw4 = true;
    { const uint32_t _dynInsn = A1_R32(0x10C9F00u); const int32_t _dynSimm = (int16_t)(_dynInsn & 0xFFFFu); const uint32_t _dynImm = _dynInsn & 0xFFFFu; (void)_dynSimm; (void)_dynImm; (t4 = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)_dynImm << 16)))); }
    { const uint32_t _dynInsn = A1_R32(0x10C9F04u); const int32_t _dynSimm = (int16_t)(_dynInsn & 0xFFFFu); const uint32_t _dynImm = _dynInsn & 0xFFFFu; (void)_dynSimm; (void)_dynImm; (t5 = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)_dynImm << 16)))); }
    { const uint32_t _dynInsn = A1_R32(0x10C9F08u); const int32_t _dynSimm = (int16_t)(_dynInsn & 0xFFFFu); const uint32_t _dynImm = _dynInsn & 0xFFFFu; (void)_dynSimm; (void)_dynImm; (t6 = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)_dynImm << 16)))); }
    { const uint32_t _dynInsn = A1_R32(0x10C9F0Cu); const int32_t _dynSimm = (int16_t)(_dynInsn & 0xFFFFu); const uint32_t _dynImm = _dynInsn & 0xFFFFu; (void)_dynSimm; (void)_dynImm; (t7 = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)_dynImm << 16)))); }
    { const uint32_t _dynInsn = A1_R32(0x10C9F10u); const int32_t _dynSimm = (int16_t)(_dynInsn & 0xFFFFu); const uint32_t _dynImm = _dynInsn & 0xFFFFu; (void)_dynSimm; (void)_dynImm; (s0 = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)_dynImm << 16)))); }
    { const uint32_t _dynInsn = A1_R32(0x10C9F14u); const int32_t _dynSimm = (int16_t)(_dynInsn & 0xFFFFu); const uint32_t _dynImm = _dynInsn & 0xFFFFu; (void)_dynSimm; (void)_dynImm; (s1 = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)_dynImm << 16)))); }
    { const uint32_t _dynInsn = A1_R32(0x10C9F18u); const int32_t _dynSimm = (int16_t)(_dynInsn & 0xFFFFu); const uint32_t _dynImm = _dynInsn & 0xFFFFu; (void)_dynSimm; (void)_dynImm; (s2 = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)_dynImm << 16)))); }
    { const uint32_t _dynInsn = A1_R32(0x10C9F1Cu); const int32_t _dynSimm = (int16_t)(_dynInsn & 0xFFFFu); const uint32_t _dynImm = _dynInsn & 0xFFFFu; (void)_dynSimm; (void)_dynImm; (s3 = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)_dynImm << 16)))); }
    { const uint32_t _dynInsn = A1_R32(0x10C9F20u); const int32_t _dynSimm = (int16_t)(_dynInsn & 0xFFFFu); const uint32_t _dynImm = _dynInsn & 0xFFFFu; (void)_dynSimm; (void)_dynImm; (s4 = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)_dynImm << 16)))); }
    { const uint32_t _dynInsn = A1_R32(0x10C9F24u); const int32_t _dynSimm = (int16_t)(_dynInsn & 0xFFFFu); const uint32_t _dynImm = _dynInsn & 0xFFFFu; (void)_dynSimm; (void)_dynImm; (s5 = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)_dynImm << 16)))); }
    { const uint32_t _dynInsn = A1_R32(0x10C9F28u); const int32_t _dynSimm = (int16_t)(_dynInsn & 0xFFFFu); const uint32_t _dynImm = _dynInsn & 0xFFFFu; (void)_dynSimm; (void)_dynImm; (s6 = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)_dynImm << 16)))); }
    { const uint32_t _dynInsn = A1_R32(0x10C9F2Cu); const int32_t _dynSimm = (int16_t)(_dynInsn & 0xFFFFu); const uint32_t _dynImm = _dynInsn & 0xFFFFu; (void)_dynSimm; (void)_dynImm; (s7 = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)_dynImm << 16)))); }
    { const uint32_t _dynInsn = A1_R32(0x10C9F30u); const int32_t _dynSimm = (int16_t)(_dynInsn & 0xFFFFu); const uint32_t _dynImm = _dynInsn & 0xFFFFu; (void)_dynSimm; (void)_dynImm; (t8 = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)_dynImm << 16)))); }
    { const uint32_t _dynInsn = A1_R32(0x10C9F34u); const int32_t _dynSimm = (int16_t)(_dynInsn & 0xFFFFu); const uint32_t _dynImm = _dynInsn & 0xFFFFu; (void)_dynSimm; (void)_dynImm; (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)_dynImm << 16)))); }
    { const uint32_t _dynInsn = A1_R32(0x10C9F38u); const int32_t _dynSimm = (int16_t)(_dynInsn & 0xFFFFu); const uint32_t _dynImm = _dynInsn & 0xFFFFu; (void)_dynSimm; (void)_dynImm; (a1 = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)_dynImm << 16)))); }
    A1_W32B((uint32_t)sp, 16, (uint32_t)a3);
    { const uint32_t _dynInsn = A1_R32(0x10C9F40u); const int32_t _dynSimm = (int16_t)(_dynInsn & 0xFFFFu); const uint32_t _dynImm = _dynInsn & 0xFFFFu; (void)_dynSimm; (void)_dynImm; (t2 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)t2, _dynSimm)))); } a3xw3 = true;
    { const uint32_t _dynInsn = A1_R32(0x10C9F44u); const int32_t _dynSimm = (int16_t)(_dynInsn & 0xFFFFu); const uint32_t _dynImm = _dynInsn & 0xFFFFu; (void)_dynSimm; (void)_dynImm; (t3 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)t3, _dynSimm)))); } a3xw4 = true;
    { const uint32_t _dynInsn = A1_R32(0x10C9F48u); const int32_t _dynSimm = (int16_t)(_dynInsn & 0xFFFFu); const uint32_t _dynImm = _dynInsn & 0xFFFFu; (void)_dynSimm; (void)_dynImm; (t4 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)t4, _dynSimm)))); }
    { const uint32_t _dynInsn = A1_R32(0x10C9F4Cu); const int32_t _dynSimm = (int16_t)(_dynInsn & 0xFFFFu); const uint32_t _dynImm = _dynInsn & 0xFFFFu; (void)_dynSimm; (void)_dynImm; (t5 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)t5, _dynSimm)))); }
    { const uint32_t _dynInsn = A1_R32(0x10C9F50u); const int32_t _dynSimm = (int16_t)(_dynInsn & 0xFFFFu); const uint32_t _dynImm = _dynInsn & 0xFFFFu; (void)_dynSimm; (void)_dynImm; (t6 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)t6, _dynSimm)))); }
    { const uint32_t _dynInsn = A1_R32(0x10C9F54u); const int32_t _dynSimm = (int16_t)(_dynInsn & 0xFFFFu); const uint32_t _dynImm = _dynInsn & 0xFFFFu; (void)_dynSimm; (void)_dynImm; (t7 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)t7, _dynSimm)))); }
    { const uint32_t _dynInsn = A1_R32(0x10C9F58u); const int32_t _dynSimm = (int16_t)(_dynInsn & 0xFFFFu); const uint32_t _dynImm = _dynInsn & 0xFFFFu; (void)_dynSimm; (void)_dynImm; (s0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)s0, _dynSimm)))); }
    { const uint32_t _dynInsn = A1_R32(0x10C9F5Cu); const int32_t _dynSimm = (int16_t)(_dynInsn & 0xFFFFu); const uint32_t _dynImm = _dynInsn & 0xFFFFu; (void)_dynSimm; (void)_dynImm; (s1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)s1, _dynSimm)))); }
    { const uint32_t _dynInsn = A1_R32(0x10C9F60u); const int32_t _dynSimm = (int16_t)(_dynInsn & 0xFFFFu); const uint32_t _dynImm = _dynInsn & 0xFFFFu; (void)_dynSimm; (void)_dynImm; (s2 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)s2, _dynSimm)))); }
    { const uint32_t _dynInsn = A1_R32(0x10C9F64u); const int32_t _dynSimm = (int16_t)(_dynInsn & 0xFFFFu); const uint32_t _dynImm = _dynInsn & 0xFFFFu; (void)_dynSimm; (void)_dynImm; (s3 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)s3, _dynSimm)))); }
    { const uint32_t _dynInsn = A1_R32(0x10C9F68u); const int32_t _dynSimm = (int16_t)(_dynInsn & 0xFFFFu); const uint32_t _dynImm = _dynInsn & 0xFFFFu; (void)_dynSimm; (void)_dynImm; (s4 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)s4, _dynSimm)))); }
    { const uint32_t _dynInsn = A1_R32(0x10C9F6Cu); const int32_t _dynSimm = (int16_t)(_dynInsn & 0xFFFFu); const uint32_t _dynImm = _dynInsn & 0xFFFFu; (void)_dynSimm; (void)_dynImm; (s5 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)s5, _dynSimm)))); }
    { const uint32_t _dynInsn = A1_R32(0x10C9F70u); const int32_t _dynSimm = (int16_t)(_dynInsn & 0xFFFFu); const uint32_t _dynImm = _dynInsn & 0xFFFFu; (void)_dynSimm; (void)_dynImm; (s6 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)s6, _dynSimm)))); }
    { const uint32_t _dynInsn = A1_R32(0x10C9F74u); const int32_t _dynSimm = (int16_t)(_dynInsn & 0xFFFFu); const uint32_t _dynImm = _dynInsn & 0xFFFFu; (void)_dynSimm; (void)_dynImm; (s7 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)s7, _dynSimm)))); }
    { const uint32_t _dynInsn = A1_R32(0x10C9F78u); const int32_t _dynSimm = (int16_t)(_dynInsn & 0xFFFFu); const uint32_t _dynImm = _dynInsn & 0xFFFFu; (void)_dynSimm; (void)_dynImm; (t8 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)t8, _dynSimm)))); }
    { const uint32_t _dynInsn = A1_R32(0x10C9F7Cu); const int32_t _dynSimm = (int16_t)(_dynInsn & 0xFFFFu); const uint32_t _dynImm = _dynInsn & 0xFFFFu; (void)_dynSimm; (void)_dynImm; (t1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)a0, _dynSimm)))); } a3xw2 = true;
    { const uint32_t _dynInsn = A1_R32(0x10C9F80u); const int32_t _dynSimm = (int16_t)(_dynInsn & 0xFFFFu); const uint32_t _dynImm = _dynInsn & 0xFFFFu; (void)_dynSimm; (void)_dynImm; (t0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)a1, _dynSimm)))); } a3xw1 = true;
    (a2 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 1))));
L_10c9f88:;
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)fp, 4))));
    (a1 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)sp, 16))));
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)a0, 0))));
    (v0 = (uint64_t)(RC_U64X(v1) & RC_U64X(a1)));
    { const bool c = (int64_t)v0 == 0; (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)SUB32((uint32_t)a0, (uint32_t)v0)))); if (c) goto L_10c9fa8; }
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)v0, 0))));
    goto L_10c9fac;
L_10c9fa8:;
    (a0 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL));
L_10c9fac:;
    { const bool c = (int64_t)a0 == 0; { const uint32_t _dynInsn = A1_R32(0x10C9FB0u); const int32_t _dynSimm = (int16_t)(_dynInsn & 0xFFFFu); const uint32_t _dynImm = _dynInsn & 0xFFFFu; (void)_dynSimm; (void)_dynImm; (a3 = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)_dynImm << 16)))); } a3xw0 = true; if (c) goto L_10c9fe0; }
    { const uint32_t _dynInsn = A1_R32(0x10C9FB4u); const int32_t _dynSimm = (int16_t)(_dynInsn & 0xFFFFu); const uint32_t _dynImm = _dynInsn & 0xFFFFu; (void)_dynSimm; (void)_dynImm; (a3 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)a3, _dynSimm)))); } a3xw0 = true;
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)a3, 0))));
    { const bool c = (int64_t)a0 == (int64_t)v0; (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)SRL32((uint32_t)v1, 18)))); if (c) goto L_10c9fe0; }
    (a1 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)a0, 8))));
    (v1 = (uint64_t)(RC_U64X(v1) & (uint64_t)(uint16_t)4095));
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)a0, 16))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)a1, 12))));
    { int64_t result = (int64_t)(int32_t)v1 * (int64_t)(int32_t)v0; lo = (uint64_t)(int64_t)(int32_t)result; hi = (uint64_t)(int64_t)(int32_t)(result >> 32); (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)result))); }
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)a0, (uint32_t)v1))));
    goto L_10c9fe4;
L_10c9fe0:;
    (v1 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL));
L_10c9fe4:;
    (a1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)fp, 8))));
    { const bool c = (int64_t)v1 == 0; (v0 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL)); if (c) goto L_10ca020; }
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)v1, 4))));
    (a3 = (uint64_t)(A1_R64B((uint32_t)sp, 8))); a3xw0 = true;
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)v1, 8))));
    (v0 = (uint64_t)(a2 << ((uint32_t)v0 & 0x3F)));
    { const bool c = (int64_t)v1 == (int64_t)a2; (a0 = (uint64_t)(a3 & v0)); if (c) goto L_10ca014; }
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 2))));
    if ((int64_t)v1 == (int64_t)v0) { (v0 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL)); goto L_10ca020; }
L_10ca014:;
    (v0 = (uint64_t)(((uint64_t)0ULL < (uint64_t)a0) ? 1 : 0));
    (v0 = (uint64_t)(RC_U64X(v0) << (32 + 0)));
    (v0 = (uint64_t)((int64_t)v0 >> (32 + 0)));
L_10ca020:;
    if ((int32_t)v0 <= 0) { (t9 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)t9, 4294967295)))); goto L_10ca1e0; }
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)a1, 0))));
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)sp, 16))));
    (v0 = (uint64_t)(RC_U64X(v1) & RC_U64X(a0)));
    { const bool c = (int64_t)v0 == 0; (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)SUB32((uint32_t)a1, (uint32_t)v0)))); if (c) goto L_10ca048; }
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)v0, 0))));
    goto L_10ca04c;
L_10ca048:;
    (a0 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL));
L_10ca04c:;
    { const bool c = (int64_t)a0 == 0; { const uint32_t _dynInsn = A1_R32(0x10CA050u); const int32_t _dynSimm = (int16_t)(_dynInsn & 0xFFFFu); const uint32_t _dynImm = _dynInsn & 0xFFFFu; (void)_dynSimm; (void)_dynImm; (a1 = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)_dynImm << 16)))); } if (c) goto L_10ca080; }
    { const uint32_t _dynInsn = A1_R32(0x10CA054u); const int32_t _dynSimm = (int16_t)(_dynInsn & 0xFFFFu); const uint32_t _dynImm = _dynInsn & 0xFFFFu; (void)_dynSimm; (void)_dynImm; (a1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)a1, _dynSimm)))); }
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)a1, 0))));
    { const bool c = (int64_t)a0 == (int64_t)v0; (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)SRL32((uint32_t)v1, 18)))); if (c) goto L_10ca080; }
    (a1 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)a0, 8))));
    (v1 = (uint64_t)(RC_U64X(v1) & (uint64_t)(uint16_t)4095));
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)a0, 16))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)a1, 12))));
    { int64_t result = (int64_t)(int32_t)v1 * (int64_t)(int32_t)v0; lo = (uint64_t)(int64_t)(int32_t)result; hi = (uint64_t)(int64_t)(int32_t)(result >> 32); (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)result))); }
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)a0, (uint32_t)v1))));
    goto L_10ca084;
L_10ca080:;
    (v1 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL));
L_10ca084:;
    { const uint32_t _dynInsn = A1_R32(0x10CA084u); const int32_t _dynSimm = (int16_t)(_dynInsn & 0xFFFFu); const uint32_t _dynImm = _dynInsn & 0xFFFFu; (void)_dynSimm; (void)_dynImm; (a3 = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)_dynImm << 16)))); } a3xw0 = true;
    { const uint32_t _dynInsn = A1_R32(0x10CA088u); const int32_t _dynSimm = (int16_t)(_dynInsn & 0xFFFFu); const uint32_t _dynImm = _dynInsn & 0xFFFFu; (void)_dynSimm; (void)_dynImm; (a3 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)a3, _dynSimm)))); } a3xw0 = true;
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)a3, 0))));
    { const bool c = (int64_t)v1 == (int64_t)v0; { const uint32_t _dynInsn = A1_R32(0x10CA094u); const int32_t _dynSimm = (int16_t)(_dynInsn & 0xFFFFu); const uint32_t _dynImm = _dynInsn & 0xFFFFu; (void)_dynSimm; (void)_dynImm; (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)_dynImm << 16)))); } if (c) goto L_10ca1dc; }
    { const uint32_t _dynInsn = A1_R32(0x10CA098u); const int32_t _dynSimm = (int16_t)(_dynInsn & 0xFFFFu); const uint32_t _dynImm = _dynInsn & 0xFFFFu; (void)_dynSimm; (void)_dynImm; (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)a0, _dynSimm)))); }
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)a0, 0))));
    { const bool c = (int64_t)v1 == (int64_t)v0; { const uint32_t _dynInsn = A1_R32(0x10CA0A4u); const int32_t _dynSimm = (int16_t)(_dynInsn & 0xFFFFu); const uint32_t _dynImm = _dynInsn & 0xFFFFu; (void)_dynSimm; (void)_dynImm; (a1 = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)_dynImm << 16)))); } if (c) goto L_10ca1dc; }
    { const uint32_t _dynInsn = A1_R32(0x10CA0A8u); const int32_t _dynSimm = (int16_t)(_dynInsn & 0xFFFFu); const uint32_t _dynImm = _dynInsn & 0xFFFFu; (void)_dynSimm; (void)_dynImm; (a1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)a1, _dynSimm)))); }
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)a1, 0))));
    { const bool c = (int64_t)v1 == (int64_t)v0; { const uint32_t _dynInsn = A1_R32(0x10CA0B4u); const int32_t _dynSimm = (int16_t)(_dynInsn & 0xFFFFu); const uint32_t _dynImm = _dynInsn & 0xFFFFu; (void)_dynSimm; (void)_dynImm; (a3 = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)_dynImm << 16)))); } a3xw0 = true; if (c) goto L_10ca1dc; }
    { const uint32_t _dynInsn = A1_R32(0x10CA0B8u); const int32_t _dynSimm = (int16_t)(_dynInsn & 0xFFFFu); const uint32_t _dynImm = _dynInsn & 0xFFFFu; (void)_dynSimm; (void)_dynImm; (a3 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)a3, _dynSimm)))); } a3xw0 = true;
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)a3, 0))));
    { const bool c = (int64_t)v1 == (int64_t)v0; { const uint32_t _dynInsn = A1_R32(0x10CA0C4u); const int32_t _dynSimm = (int16_t)(_dynInsn & 0xFFFFu); const uint32_t _dynImm = _dynInsn & 0xFFFFu; (void)_dynSimm; (void)_dynImm; (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)_dynImm << 16)))); } if (c) goto L_10ca1dc; }
    { const uint32_t _dynInsn = A1_R32(0x10CA0C8u); const int32_t _dynSimm = (int16_t)(_dynInsn & 0xFFFFu); const uint32_t _dynImm = _dynInsn & 0xFFFFu; (void)_dynSimm; (void)_dynImm; (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)a0, _dynSimm)))); }
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)a0, 0))));
    { const bool c = (int64_t)v1 == (int64_t)v0; { const uint32_t _dynInsn = A1_R32(0x10CA0D4u); const int32_t _dynSimm = (int16_t)(_dynInsn & 0xFFFFu); const uint32_t _dynImm = _dynInsn & 0xFFFFu; (void)_dynSimm; (void)_dynImm; (a1 = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)_dynImm << 16)))); } if (c) goto L_10ca1dc; }
    { const uint32_t _dynInsn = A1_R32(0x10CA0D8u); const int32_t _dynSimm = (int16_t)(_dynInsn & 0xFFFFu); const uint32_t _dynImm = _dynInsn & 0xFFFFu; (void)_dynSimm; (void)_dynImm; (a1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)a1, _dynSimm)))); }
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)a1, 0))));
    { const bool c = (int64_t)v1 == (int64_t)v0; { const uint32_t _dynInsn = A1_R32(0x10CA0E4u); const int32_t _dynSimm = (int16_t)(_dynInsn & 0xFFFFu); const uint32_t _dynImm = _dynInsn & 0xFFFFu; (void)_dynSimm; (void)_dynImm; (a3 = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)_dynImm << 16)))); } a3xw0 = true; if (c) goto L_10ca1dc; }
    { const uint32_t _dynInsn = A1_R32(0x10CA0E8u); const int32_t _dynSimm = (int16_t)(_dynInsn & 0xFFFFu); const uint32_t _dynImm = _dynInsn & 0xFFFFu; (void)_dynSimm; (void)_dynImm; (a3 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)a3, _dynSimm)))); } a3xw0 = true;
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)a3, 0))));
    if ((int64_t)v1 == (int64_t)v0) { (t9 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)t9, 4294967295)))); goto L_10ca1e0; }
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)t2, 0))));
    if ((int64_t)v1 == (int64_t)v0) { (t9 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)t9, 4294967295)))); goto L_10ca1e0; }
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)t3, 0))));
    if ((int64_t)v1 == (int64_t)v0) { (t9 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)t9, 4294967295)))); goto L_10ca1e0; }
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)t4, 0))));
    if ((int64_t)v1 == (int64_t)v0) { (t9 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)t9, 4294967295)))); goto L_10ca1e0; }
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)t5, 0))));
    if ((int64_t)v1 == (int64_t)v0) { (t9 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)t9, 4294967295)))); goto L_10ca1e0; }
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)t6, 0))));
    if ((int64_t)v1 == (int64_t)v0) { (t9 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)t9, 4294967295)))); goto L_10ca1e0; }
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)t7, 0))));
    if ((int64_t)v1 == (int64_t)v0) { (t9 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)t9, 4294967295)))); goto L_10ca1e0; }
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)s0, 0))));
    if ((int64_t)v1 == (int64_t)v0) { (t9 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)t9, 4294967295)))); goto L_10ca1e0; }
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)s1, 0))));
    if ((int64_t)v1 == (int64_t)v0) { (t9 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)t9, 4294967295)))); goto L_10ca1e0; }
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)s2, 0))));
    if ((int64_t)v1 == (int64_t)v0) { (t9 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)t9, 4294967295)))); goto L_10ca1e0; }
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)s3, 0))));
    if ((int64_t)v1 == (int64_t)v0) { (t9 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)t9, 4294967295)))); goto L_10ca1e0; }
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)s4, 0))));
    if ((int64_t)v1 == (int64_t)v0) { (t9 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)t9, 4294967295)))); goto L_10ca1e0; }
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)s5, 0))));
    if ((int64_t)v1 == (int64_t)v0) { (t9 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)t9, 4294967295)))); goto L_10ca1e0; }
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)s6, 0))));
    if ((int64_t)v1 == (int64_t)v0) { (t9 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)t9, 4294967295)))); goto L_10ca1e0; }
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)s7, 0))));
    if ((int64_t)v1 == (int64_t)v0) { (t9 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)t9, 4294967295)))); goto L_10ca1e0; }
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)t8, 0))));
    if ((int64_t)v1 == (int64_t)v0) { (t9 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)t9, 4294967295)))); goto L_10ca1e0; }
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)t1, 0))));
    if ((int64_t)v1 == (int64_t)v0) { (t9 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)t9, 4294967295)))); goto L_10ca1e0; }
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)t0, 0))));
    { const bool c = (int64_t)v1 == (int64_t)v0; { const uint32_t _dynInsn = A1_R32(0x10CA1C0u); const int32_t _dynSimm = (int16_t)(_dynInsn & 0xFFFFu); const uint32_t _dynImm = _dynInsn & 0xFFFFu; (void)_dynSimm; (void)_dynImm; (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)_dynImm << 16)))); } if (c) goto L_10ca1dc; }
    (a1 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)sp, 0))));
    { const uint32_t _dynInsn = A1_R32(0x10CA1C8u); const int32_t _dynSimm = (int16_t)(_dynInsn & 0xFFFFu); const uint32_t _dynImm = _dynInsn & 0xFFFFu; (void)_dynSimm; (void)_dynImm; (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)a0, _dynSimm)))); }
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)a0, 0))));
    (v0 = (uint64_t)(RC_U64X(v1) ^ RC_U64X(v0)));
    if (RC_U64X(v0) != 0) do { const a4::q128 _v = (a4::mk128(h3, v1)); h5 = a4::hi64(_v); std::memcpy(&a1, &_v, 8); } while (0);
    A1_W32B((uint32_t)sp, 0, (uint32_t)a1);
L_10ca1dc:;
    (t9 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)t9, 4294967295))));
L_10ca1e0:;
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 4294967295))));
    { const bool c = (int64_t)t9 != (int64_t)v0; (fp = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)fp, 16)))); if (c) { A3_BACK(); goto L_10c9f88; } }
L_10ca1ec:;
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)sp, 0))));
    goto L_10ca1fc;
L_10ca1f8:;
    (v0 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL));
L_10ca1fc:;
    { (sp = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)sp, 112)))); if (xo) { if (a3xw0) xo[0] = a3; if (a3xw1) xo[1] = t0; if (a3xw2) xo[2] = t1; if (a3xw3) xo[3] = t2; if (a3xw4) xo[4] = t3; } io_h2 = h2; io_h5 = h5; return v0; }
    __builtin_unreachable();
}

// manager_sub_01114C70 (0x1114c70): typed by tools/typed_lift2.py (round A6; loop guest frame calls, extra outputs xo[0]=f0 xo[1]=f20 xo[2]=a1 xo[3]=a2 xo[4]=a3 xo[5]=t0 xo[6]=t1 xo[7]=t2 xo[8]=t3; VU0 in vf10 vf29 vf31 vacc h9 h29, out vf8 vf10 vf14 vf15 vf16 vf17 vf29 vf30 vf31 vacc h5 h9)
inline uint64_t manager_sub_01114C70(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint64_t a0, uint64_t a1, uint64_t a2, uint64_t a3, uint64_t t0, uint32_t sp, float f12, float f13, float f14, float f15, float f16, float f17, float f18, float f19, float f20, a4::vec4 &io_vf8, a4::vec4 &io_vf10, a4::vec4 &io_vf14, a4::vec4 &io_vf15, a4::vec4 &io_vf16, a4::vec4 &io_vf17, a4::vec4 &io_vf29, a4::vec4 &io_vf30, a4::vec4 &io_vf31, a4::vec4 &io_vacc, uint64_t &io_h5, uint64_t &io_h9, uint64_t io_h29, uint64_t *xo = nullptr)
{
    float f0 = 0.0f;
    float f1 = 0.0f;
    uint32_t at = 0;
    uint64_t v0 = 0;
    uint64_t v1 = 0;
    uint64_t t1 = 0;
    uint64_t t2 = 0;
    uint32_t t3 = 0;
    uint32_t t4 = 0;
    uint32_t s0 = 0;
    uint64_t s1 = 0;
    uint32_t s2 = 0;
    uint32_t s3 = 0;
    uint32_t s4 = 0;
    uint64_t s5 = 0;
    uint64_t s6 = 0;
    uint64_t s7 = 0;
    uint64_t fp = 0;
    a4::vec4 vf8 = io_vf8;
    a4::vec4 vf10 = io_vf10;
    a4::vec4 vf14 = io_vf14;
    a4::vec4 vf15 = io_vf15;
    a4::vec4 vf16 = io_vf16;
    a4::vec4 vf17 = io_vf17;
    a4::vec4 vf29 = io_vf29;
    a4::vec4 vf30 = io_vf30;
    a4::vec4 vf31 = io_vf31;
    a4::vec4 vacc = io_vacc;
    uint64_t h5 = io_h5;
    uint64_t h9 = io_h9;
    uint64_t h29 = io_h29;
    bool a3xw0 = false;
    bool a3xw5 = false;
    sp = sp - 0xC0u;
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)SLL32((uint32_t)a3, 3))));
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v1, (uint32_t)a3))));
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)SLL32((uint32_t)v1, 4))));
    if (a1 == 0) do { const a4::q128 _v = (a4::mk128(h29, a1_sx64(sp))); h5 = a4::hi64(_v); std::memcpy(&a1, &_v, 8); } while (0);
    (s6 = (uint64_t)((uint64_t)a0 + (uint64_t)0ULL));
    A1_W32B(sp, 176, a1_bits(f20));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)a2, 388))));
    (t1 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)a2, 372))));
    (t2 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)v0, 32))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)SLL32((uint32_t)a3, 1))));
    (t4 = (uint32_t)((int32_t)((int32_t)A1_R32B((uint32_t)t1, 36))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v0, (uint32_t)a3))));
    (t3 = (uint32_t)((int32_t)((int32_t)A1_R32B((uint32_t)t2, 8))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)SLL32((uint32_t)v0, 5))));
    (t1 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)a2, 348))));
    (a3 = (uint64_t)((int64_t)(int32_t)((int32_t)SLL32((uint32_t)a3, 6))));
    (s3 = (uint32_t)((int32_t)((int32_t)ADD32(t3, (uint32_t)v1))));
    (s2 = (uint32_t)((int32_t)((int32_t)ADD32(t4, (uint32_t)v0))));
    (v1 = (uint64_t)(uint32_t)((uint8_t)A1_R8B(s3, 9)));
    (s4 = (uint32_t)((int32_t)((int32_t)ADD32((uint32_t)t1, (uint32_t)a3))));
    { const bool c = (int64_t)v1 == 0; A1_W32B(sp, 64, (uint32_t)a1); if (c) goto L_1114f80; }
    (v0 = (uint64_t)(uint32_t)((uint8_t)A1_R8B(s3, 8)));
    { const bool c = (int64_t)v0 == 0; (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)SLL32((uint32_t)t0, 2)))); if (c) goto L_1114e38; }
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B(s3, 20))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v0, (uint32_t)t0))));
    (s7 = (uint64_t)((uint64_t)s6 + (uint64_t)0ULL));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)SLL32((uint32_t)v0, 2))));
    (s5 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL));
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v1, (uint32_t)v0))));
    (s6 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(sp, 16))));
    (s1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v1, 2))));
    (t1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(sp, 32))));
    (fp = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL));
L_1114d28:;
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B(sp, 64))));
    (t0 = (uint64_t)(uint32_t)((uint16_t)A1_R16B((uint32_t)s1, 0))); a3xw5 = true;
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)fp, (uint32_t)v1))));
    A1_W32B((uint32_t)v0, 0, (uint32_t)t0);
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)SLL32((uint32_t)t0, 1))));
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v1, (uint32_t)t0))));
    (a3 = (uint64_t)((int64_t)(int32_t)((int32_t)SLL32((uint32_t)t0, 4))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B(s2, 8))));
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)SLL32((uint32_t)v1, 1))));
    (a1 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B(s2, 20))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v0, (uint32_t)t0))));
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B(s3, 4))));
    (a2 = (uint64_t)(uint32_t)((uint8_t)A1_R8B((uint32_t)v0, 0)));
    (s0 = (uint32_t)((int32_t)((int32_t)ADD32((uint32_t)a1, (uint32_t)a3))));
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)a0, (uint32_t)v1))));
    { const bool c = (int64_t)a2 != 0; (a1 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B(s3, 12)))); if (c) goto L_1114e14; }
    (v0 = (uint64_t)((int64_t)(int32_t)((int16_t)A1_R16B((uint32_t)a0, 0))));
    A1_W32B(sp, 48, (uint32_t)v0);
    (v1 = (uint64_t)((int64_t)(int32_t)((int16_t)A1_R16B((uint32_t)a0, 2))));
    A1_W32B(sp, 52, (uint32_t)v1);
    (v0 = (uint64_t)((int64_t)(int32_t)((int16_t)A1_R16B((uint32_t)a0, 4))));
    A1_W32B(sp, 60, 0u);
    A1_W32B(sp, 56, (uint32_t)v0);
    (v1 = (uint64_t)(A1_R64B(sp, 48)));
    (a0 = (uint64_t)(A1_R64B(sp, 56)));
    A1_W64B(sp, 32, v1);
    A1_W64B(sp, 40, a0);
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B(sp, 44))));
    (v0 = (uint64_t)((int64_t)(int32_t)(SRA32((int32_t)v0, 1))));
    A1_W32B(sp, 44, (uint32_t)v0);
    vf30 = a4v::castsi128_ps(A4_R128(ADD32((uint32_t)t1, 0)));
    { a4::q128 src = a4v::castps_si128(vf30); a4::vec4 res = a4v::cvtepi32_ps(src); res = a4v::mul_ps(res, a4v::set1_ps(3.05175781e-05f)); a4::q128 mask = a4v::set_epi32(-1, -1, -1, -1); vf29 = a4v::blendv_ps(vf29, res, a4v::castsi128_ps(mask)); }
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)a1, 16))));
    vf30 = a4v::castsi128_ps(A4_R128(ADD32((uint32_t)v0, 0)));
    { a4::vec4 res = a4v::mul(vf29, vf30); a4::q128 mask = a4v::set_epi32(-1, -1, -1, -1); vf31 = a4v::blend(vf31, res, a4v::castsi128_ps(mask)); }
    vf30 = a4v::castsi128_ps(A4_R128(ADD32((uint32_t)a1, 0)));
    { a4::q128 mask = a4v::set_epi32(-1, -1, -1, -1); vf29 = a4v::blendv_ps(vf29, vf31, a4v::castsi128_ps(mask)); }
    { a4::q128 mask = a4v::set_epi32(-1, -1, -1, -1); vf31 = a4v::blendv_ps(vf31, vf29, a4v::castsi128_ps(mask)); }
    { a4::vec4 res = a4v::addK<1, 0>(vf29, vf30); a4::q128 mask = a4v::set_epi32(0, -1, -1, -1); vf31 = a4v::blend(vf31, res, a4v::castsi128_ps(mask)); }
    A4_W128(ADD32((uint32_t)s6, 0), a4v::castps_si128(vf31));
    (at = (uint32_t)((int32_t)((int32_t)((uint32_t)16256 << 16))));
    { uint32_t bits = a7_opq(at); std::memcpy(&f0, &bits, sizeof(bits)); } a3xw0 = true;
    A1_W32B(sp, 28, a1_bits(f0));
    vf8 = a4v::castsi128_ps(A4_R128(ADD32((uint32_t)s6, 0)));
    vf14 = a4v::castsi128_ps(A4_R128(ADD32(s4, 0)));
    vf15 = a4v::castsi128_ps(A4_R128(ADD32(s4, 16)));
    vf16 = a4v::castsi128_ps(A4_R128(ADD32(s4, 32)));
    vf17 = a4v::castsi128_ps(A4_R128(ADD32(s4, 48)));
    { a4::vec4 res = a4v::mul(vf14, a4v::bcast<0>(vf8)); vacc = a4v::dest<1, 1, 1, 1>(vacc, res); }
    { a4::vec4 mul_res = a4v::mul(vf15, a4v::bcast<1>(vf8)); a4::vec4 res = a4v::addK<1, 1>(vacc, mul_res); vacc = a4v::dest<1, 1, 1, 1>(vacc, res); }
    { a4::vec4 mul_res = a4v::mul(vf16, a4v::bcast<2>(vf8)); a4::vec4 res = a4v::addK<1, 1>(vacc, mul_res); vacc = a4v::dest<1, 1, 1, 1>(vacc, res); }
    { a4::vec4 mul_res = a4v::mul(vf17, a4v::bcast<3>(vf8)); a4::vec4 res = a4v::addK<1, 1>(vacc, mul_res); a4::q128 mask = a4v::set_epi32(-1, -1, -1, -1); vf10 = a4v::blendv_ps(vf10, res, a4v::castsi128_ps(mask)); }
    A4_W128(ADD32(s0, 0), a4v::castps_si128(vf10));
    (a1 = (uint64_t)((uint64_t)RC_U64X(t0) + (uint64_t)0ULL));
    (a0 = (uint64_t)((uint64_t)a1_sx64(s2) + (uint64_t)0ULL));
    { A4_W128(ADD32(sp, 80), a4::mk128(h9, t1)); A3_CHARGE(); uint64_t a7c[5] = {0u, 0u, 0u, 0u, 0u}; v0 = EntryModifiedManager(ram, rt, a0, (uint32_t)a1, a7c); a2 = a7c[0]; a3 = a7c[1]; t0 = a7c[2]; t2 = a7c[4]; a3xw5 = true; }
    do { const a4::q128 _v = (A4_R128(ADD32(sp, 80))); h9 = a4::hi64(_v); std::memcpy(&t1, &_v, 8); } while (0);
L_1114e14:;
    (s5 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)s5, 1))));
    A1_W32B((uint32_t)s7, 0, s0);
    (v0 = (uint64_t)(((int64_t)(int64_t)RC_U64X(s5) < (int64_t)(int32_t)3) ? 1 : 0));
    (s1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)s1, 2))));
    (fp = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)fp, 4))));
    { const bool c = (int64_t)v0 != 0; (s7 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)s7, 4)))); if (c) { A3_BACK(); goto L_1114d28; } }
    goto L_1115128;
L_1114e38:;
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)SLL32((uint32_t)t0, 1))));
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B(s3, 20))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v0, (uint32_t)t0))));
    (s7 = (uint64_t)((uint64_t)s6 + (uint64_t)0ULL));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)SLL32((uint32_t)v0, 2))));
    (s5 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v0, (uint32_t)t0))));
    (s6 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(sp, 16))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)SLL32((uint32_t)v0, 2))));
    (fp = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(sp, 32))));
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v1, (uint32_t)v0))));
    (s1 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL));
    A1_W32B(sp, 68, (uint32_t)v1);
L_1114e70:;
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B(sp, 68))));
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B(sp, 64))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)a0, (uint32_t)s1))));
    (t0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)v0, 0)))); a3xw5 = true;
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)s1, (uint32_t)v1))));
    A1_W32B((uint32_t)a0, 0, (uint32_t)t0);
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)SLL32((uint32_t)t0, 1))));
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v1, (uint32_t)t0))));
    (a3 = (uint64_t)((int64_t)(int32_t)((int32_t)SLL32((uint32_t)t0, 4))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B(s2, 8))));
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)SLL32((uint32_t)v1, 1))));
    (a1 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B(s2, 20))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v0, (uint32_t)t0))));
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B(s3, 4))));
    (a2 = (uint64_t)(uint32_t)((uint8_t)A1_R8B((uint32_t)v0, 0)));
    (s0 = (uint32_t)((int32_t)((int32_t)ADD32((uint32_t)a1, (uint32_t)a3))));
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)a0, (uint32_t)v1))));
    { const bool c = (int64_t)a2 != 0; (a1 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B(s3, 12)))); if (c) goto L_1114f5c; }
    (v0 = (uint64_t)((int64_t)(int32_t)((int16_t)A1_R16B((uint32_t)a0, 0))));
    A1_W32B(sp, 48, (uint32_t)v0);
    (v1 = (uint64_t)((int64_t)(int32_t)((int16_t)A1_R16B((uint32_t)a0, 2))));
    A1_W32B(sp, 52, (uint32_t)v1);
    (v0 = (uint64_t)((int64_t)(int32_t)((int16_t)A1_R16B((uint32_t)a0, 4))));
    A1_W32B(sp, 60, 0u);
    A1_W32B(sp, 56, (uint32_t)v0);
    (v1 = (uint64_t)(A1_R64B(sp, 48)));
    (a0 = (uint64_t)(A1_R64B(sp, 56)));
    A1_W64B(sp, 32, v1);
    A1_W64B(sp, 40, a0);
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B(sp, 44))));
    (v0 = (uint64_t)((int64_t)(int32_t)(SRA32((int32_t)v0, 1))));
    A1_W32B(sp, 44, (uint32_t)v0);
    vf30 = a4v::castsi128_ps(A4_R128(ADD32((uint32_t)fp, 0)));
    { a4::q128 src = a4v::castps_si128(vf30); a4::vec4 res = a4v::cvtepi32_ps(src); res = a4v::mul_ps(res, a4v::set1_ps(3.05175781e-05f)); a4::q128 mask = a4v::set_epi32(-1, -1, -1, -1); vf29 = a4v::blendv_ps(vf29, res, a4v::castsi128_ps(mask)); }
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)a1, 16))));
    vf30 = a4v::castsi128_ps(A4_R128(ADD32((uint32_t)v0, 0)));
    { a4::vec4 res = a4v::mul(vf29, vf30); a4::q128 mask = a4v::set_epi32(-1, -1, -1, -1); vf31 = a4v::blend(vf31, res, a4v::castsi128_ps(mask)); }
    vf30 = a4v::castsi128_ps(A4_R128(ADD32((uint32_t)a1, 0)));
    { a4::q128 mask = a4v::set_epi32(-1, -1, -1, -1); vf29 = a4v::blendv_ps(vf29, vf31, a4v::castsi128_ps(mask)); }
    { a4::q128 mask = a4v::set_epi32(-1, -1, -1, -1); vf31 = a4v::blendv_ps(vf31, vf29, a4v::castsi128_ps(mask)); }
    { a4::vec4 res = a4v::addK<1, 0>(vf29, vf30); a4::q128 mask = a4v::set_epi32(0, -1, -1, -1); vf31 = a4v::blend(vf31, res, a4v::castsi128_ps(mask)); }
    A4_W128(ADD32((uint32_t)s6, 0), a4v::castps_si128(vf31));
    (at = (uint32_t)((int32_t)((int32_t)((uint32_t)16256 << 16))));
    { uint32_t bits = a7_opq(at); std::memcpy(&f0, &bits, sizeof(bits)); } a3xw0 = true;
    A1_W32B(sp, 28, a1_bits(f0));
    vf8 = a4v::castsi128_ps(A4_R128(ADD32((uint32_t)s6, 0)));
    vf14 = a4v::castsi128_ps(A4_R128(ADD32(s4, 0)));
    vf15 = a4v::castsi128_ps(A4_R128(ADD32(s4, 16)));
    vf16 = a4v::castsi128_ps(A4_R128(ADD32(s4, 32)));
    vf17 = a4v::castsi128_ps(A4_R128(ADD32(s4, 48)));
    { a4::vec4 res = a4v::mul(vf14, a4v::bcast<0>(vf8)); vacc = a4v::dest<1, 1, 1, 1>(vacc, res); }
    { a4::vec4 mul_res = a4v::mul(vf15, a4v::bcast<1>(vf8)); a4::vec4 res = a4v::addK<1, 1>(vacc, mul_res); vacc = a4v::dest<1, 1, 1, 1>(vacc, res); }
    { a4::vec4 mul_res = a4v::mul(vf16, a4v::bcast<2>(vf8)); a4::vec4 res = a4v::addK<1, 1>(vacc, mul_res); vacc = a4v::dest<1, 1, 1, 1>(vacc, res); }
    { a4::vec4 mul_res = a4v::mul(vf17, a4v::bcast<3>(vf8)); a4::vec4 res = a4v::addK<1, 1>(vacc, mul_res); a4::q128 mask = a4v::set_epi32(-1, -1, -1, -1); vf10 = a4v::blendv_ps(vf10, res, a4v::castsi128_ps(mask)); }
    A4_W128(ADD32(s0, 0), a4v::castps_si128(vf10));
    (a1 = (uint64_t)((uint64_t)RC_U64X(t0) + (uint64_t)0ULL));
    { (a0 = (uint64_t)((uint64_t)a1_sx64(s2) + (uint64_t)0ULL)); A3_CHARGE(); uint64_t a7c[5] = {0u, 0u, 0u, 0u, 0u}; v0 = EntryModifiedManager(ram, rt, a0, (uint32_t)a1, a7c); a2 = a7c[0]; a3 = a7c[1]; t0 = a7c[2]; t1 = a7c[3]; t2 = a7c[4]; a3xw5 = true; }
L_1114f5c:;
    (s5 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)s5, 1))));
    A1_W32B((uint32_t)s7, 0, s0);
    (v0 = (uint64_t)(((int64_t)(int64_t)RC_U64X(s5) < (int64_t)(int32_t)3) ? 1 : 0));
    (s1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)s1, 4))));
    { const bool c = (int64_t)v0 != 0; (s7 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)s7, 4)))); if (c) { A3_BACK(); goto L_1114e70; } }
    goto L_1115128;
L_1114f80:;
    (v0 = (uint64_t)(uint32_t)((uint8_t)A1_R8B(s3, 8)));
    { const bool c = (int64_t)v0 == 0; (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)SLL32((uint32_t)t0, 2)))); if (c) goto L_1115080; }
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B(s3, 20))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v0, (uint32_t)t0))));
    (at = (uint32_t)((int32_t)((int32_t)((uint32_t)16256 << 16))));
    { uint32_t bits = a7_opq(at); std::memcpy(&f20, &bits, sizeof(bits)); }
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)SLL32((uint32_t)v0, 2))));
    (s5 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL));
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v1, (uint32_t)v0))));
    (fp = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(sp, 16))));
    (s1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v1, 2))));
    (s7 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL));
L_1114fb8:;
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B(sp, 64))));
    (t0 = (uint64_t)(uint32_t)((uint16_t)A1_R16B((uint32_t)s1, 0))); a3xw5 = true;
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)s7, (uint32_t)a0))));
    A1_W32B((uint32_t)v0, 0, (uint32_t)t0);
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)SLL32((uint32_t)t0, 1))));
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v1, (uint32_t)t0))));
    (a3 = (uint64_t)((int64_t)(int32_t)((int32_t)SLL32((uint32_t)t0, 4))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B(s2, 8))));
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)SLL32((uint32_t)v1, 2))));
    (a2 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B(s2, 20))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v0, (uint32_t)t0))));
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B(s3, 4))));
    (a1 = (uint64_t)(uint32_t)((uint8_t)A1_R8B((uint32_t)v0, 0)));
    (s0 = (uint32_t)((int32_t)((int32_t)ADD32((uint32_t)a2, (uint32_t)a3))));
    { const bool c = (int64_t)a1 != 0; (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)a0, (uint32_t)v1)))); if (c) goto L_1115058; }
    { uint32_t bits = A1_R32B((uint32_t)a0, 0); float f; std::memcpy(&f, &bits, sizeof(f)); f0 = f; } a3xw0 = true;
    A1_W32B(sp, 32, a1_bits(f0));
    { uint32_t bits = A1_R32B((uint32_t)a0, 4); float f; std::memcpy(&f, &bits, sizeof(f)); f1 = f; }
    A1_W32B(sp, 36, a1_bits(f1));
    { uint32_t bits = A1_R32B((uint32_t)a0, 8); float f; std::memcpy(&f, &bits, sizeof(f)); f0 = f; } a3xw0 = true;
    A1_W32B(sp, 44, a1_bits(f20));
    A1_W32B(sp, 40, a1_bits(f0));
    (v0 = (uint64_t)(A1_R64B(sp, 32)));
    (v1 = (uint64_t)(A1_R64B(sp, 40)));
    A1_W64B(sp, 16, v0);
    A1_W64B(sp, 24, v1);
    vf8 = a4v::castsi128_ps(A4_R128(ADD32((uint32_t)fp, 0)));
    vf14 = a4v::castsi128_ps(A4_R128(ADD32(s4, 0)));
    vf15 = a4v::castsi128_ps(A4_R128(ADD32(s4, 16)));
    vf16 = a4v::castsi128_ps(A4_R128(ADD32(s4, 32)));
    vf17 = a4v::castsi128_ps(A4_R128(ADD32(s4, 48)));
    { a4::vec4 res = a4v::mul(vf14, a4v::bcast<0>(vf8)); vacc = a4v::dest<1, 1, 1, 1>(vacc, res); }
    { a4::vec4 mul_res = a4v::mul(vf15, a4v::bcast<1>(vf8)); a4::vec4 res = a4v::addK<1, 1>(vacc, mul_res); vacc = a4v::dest<1, 1, 1, 1>(vacc, res); }
    { a4::vec4 mul_res = a4v::mul(vf16, a4v::bcast<2>(vf8)); a4::vec4 res = a4v::addK<1, 1>(vacc, mul_res); vacc = a4v::dest<1, 1, 1, 1>(vacc, res); }
    { a4::vec4 mul_res = a4v::mul(vf17, a4v::bcast<3>(vf8)); a4::vec4 res = a4v::addK<1, 1>(vacc, mul_res); a4::q128 mask = a4v::set_epi32(-1, -1, -1, -1); vf10 = a4v::blendv_ps(vf10, res, a4v::castsi128_ps(mask)); }
    A4_W128(ADD32(s0, 0), a4v::castps_si128(vf10));
    (a1 = (uint64_t)((uint64_t)RC_U64X(t0) + (uint64_t)0ULL));
    { (a0 = (uint64_t)((uint64_t)a1_sx64(s2) + (uint64_t)0ULL)); A3_CHARGE(); uint64_t a7c[5] = {0u, 0u, 0u, 0u, 0u}; v0 = EntryModifiedManager(ram, rt, a0, (uint32_t)a1, a7c); a2 = a7c[0]; a3 = a7c[1]; t0 = a7c[2]; t1 = a7c[3]; t2 = a7c[4]; a3xw5 = true; }
L_1115058:;
    (s5 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)s5, 1))));
    A1_W32B((uint32_t)s6, 0, s0);
    (v0 = (uint64_t)(((int64_t)(int64_t)RC_U64X(s5) < (int64_t)(int32_t)3) ? 1 : 0));
    (s1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)s1, 2))));
    (s7 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)s7, 4))));
    { const bool c = (int64_t)v0 != 0; (s6 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)s6, 4)))); if (c) { A3_BACK(); goto L_1114fb8; } }
    goto L_1115128;
L_1115080:;
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)SLL32((uint32_t)t0, 1))));
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B(s3, 20))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v0, (uint32_t)t0))));
    (s5 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)SLL32((uint32_t)v0, 2))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v0, (uint32_t)t0))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)SLL32((uint32_t)v0, 2))));
    (s7 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v1, (uint32_t)v0))));
L_11150a0:;
    (s1 = (uint64_t)((int64_t)(int32_t)((int32_t)SLL32((uint32_t)s5, 2))));
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B(sp, 64))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)s7, (uint32_t)s1))));
    (a3 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)v0, 0))));
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)s1, (uint32_t)a0))));
    A1_W32B((uint32_t)v1, 0, (uint32_t)a3);
    (a2 = (uint64_t)((int64_t)(int32_t)((int32_t)SLL32((uint32_t)a3, 4))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B(s2, 8))));
    (a1 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B(s2, 20))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v0, (uint32_t)a3))));
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B(s3, 4))));
    (a0 = (uint64_t)(uint32_t)((uint8_t)A1_R8B((uint32_t)v0, 0)));
    (s0 = (uint32_t)((int32_t)((int32_t)ADD32((uint32_t)a1, (uint32_t)a2))));
    { const bool c = (int64_t)a0 != 0; (a2 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)a2, (uint32_t)v1)))); if (c) goto L_1115110; }
    vf8 = a4v::castsi128_ps(A4_R128(ADD32((uint32_t)a2, 0)));
    vf14 = a4v::castsi128_ps(A4_R128(ADD32(s4, 0)));
    vf15 = a4v::castsi128_ps(A4_R128(ADD32(s4, 16)));
    vf16 = a4v::castsi128_ps(A4_R128(ADD32(s4, 32)));
    vf17 = a4v::castsi128_ps(A4_R128(ADD32(s4, 48)));
    { a4::vec4 res = a4v::mul(vf14, a4v::bcast<0>(vf8)); vacc = a4v::dest<1, 1, 1, 1>(vacc, res); }
    { a4::vec4 mul_res = a4v::mul(vf15, a4v::bcast<1>(vf8)); a4::vec4 res = a4v::addK<1, 1>(vacc, mul_res); vacc = a4v::dest<1, 1, 1, 1>(vacc, res); }
    { a4::vec4 mul_res = a4v::mul(vf16, a4v::bcast<2>(vf8)); a4::vec4 res = a4v::addK<1, 1>(vacc, mul_res); vacc = a4v::dest<1, 1, 1, 1>(vacc, res); }
    { a4::vec4 mul_res = a4v::mul(vf17, a4v::bcast<3>(vf8)); a4::vec4 res = a4v::addK<1, 1>(vacc, mul_res); a4::q128 mask = a4v::set_epi32(-1, -1, -1, -1); vf10 = a4v::blendv_ps(vf10, res, a4v::castsi128_ps(mask)); }
    A4_W128(ADD32(s0, 0), a4v::castps_si128(vf10));
    (a1 = (uint64_t)((uint64_t)RC_U64X(a3) + (uint64_t)0ULL));
    { (a0 = (uint64_t)((uint64_t)a1_sx64(s2) + (uint64_t)0ULL)); A3_CHARGE(); uint64_t a7c[5] = {0u, 0u, 0u, 0u, 0u}; v0 = EntryModifiedManager(ram, rt, a0, (uint32_t)a1, a7c); a2 = a7c[0]; a3 = a7c[1]; t0 = a7c[2]; t1 = a7c[3]; t2 = a7c[4]; a3xw5 = true; }
L_1115110:;
    (s5 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)s5, 1))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)s1, (uint32_t)s6))));
    (v1 = (uint64_t)(((int64_t)(int64_t)RC_U64X(s5) < (int64_t)(int32_t)3) ? 1 : 0));
    { const bool c = (int64_t)v1 != 0; A1_W32B((uint32_t)v0, 0, s0); if (c) { A3_BACK(); goto L_11150a0; } }
L_1115128:;
    f20 = a1_flt(A1_R32B(sp, 176));
    { sp = sp + 0xC0u; if (xo) { if (a3xw0) xo[0] = a1_bits(f0); xo[1] = a1_bits(f20); xo[2] = a1; xo[3] = a2; xo[4] = a3; if (a3xw5) xo[5] = t0; xo[6] = t1; xo[7] = t2; xo[8] = a1_sx64(t3); } io_vf8 = vf8; io_vf10 = vf10; io_vf14 = vf14; io_vf15 = vf15; io_vf16 = vf16; io_vf17 = vf17; io_vf29 = vf29; io_vf30 = vf30; io_vf31 = vf31; io_vacc = vacc; io_h5 = h5; io_h9 = h9; return v0; }
    __builtin_unreachable();
}

// manager_sub_01116108 (0x1116108): typed by tools/typed_lift2.py (round A6; loop guest frame calls, extra outputs xo[0]=a1 xo[1]=a2 xo[2]=a3 xo[3]=t0 xo[4]=t1 xo[5]=t2 xo[6]=t3; VU0 in vf10 vf29 vacc h11, out vf8 vf10 vf14 vf15 vf16 vf17 vf29 vf30 vf31 vacc h11)
inline uint64_t manager_sub_01116108(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint64_t a0, uint64_t a1, uint64_t a2, uint64_t a3, uint64_t t0, uint64_t t1, uint64_t t2, uint64_t t3, uint64_t sp, float f12, float f13, float f14, float f15, float f16, float f17, float f18, float f19, a4::vec4 &io_vf8, a4::vec4 &io_vf10, a4::vec4 &io_vf14, a4::vec4 &io_vf15, a4::vec4 &io_vf16, a4::vec4 &io_vf17, a4::vec4 &io_vf29, a4::vec4 &io_vf30, a4::vec4 &io_vf31, a4::vec4 &io_vacc, uint64_t &io_h11, uint64_t *xo = nullptr)
{
    uint64_t v0 = 0;
    uint64_t v1 = 0;
    uint64_t s0 = 0;
    uint64_t s1 = 0;
    uint64_t s2 = 0;
    uint64_t s3 = 0;
    uint64_t s4 = 0;
    uint64_t s5 = 0;
    uint64_t s6 = 0;
    uint64_t s7 = 0;
    uint64_t fp = 0;
    a4::vec4 vf8 = io_vf8;
    a4::vec4 vf10 = io_vf10;
    a4::vec4 vf14 = io_vf14;
    a4::vec4 vf15 = io_vf15;
    a4::vec4 vf16 = io_vf16;
    a4::vec4 vf17 = io_vf17;
    a4::vec4 vf29 = io_vf29;
    a4::vec4 vf30 = io_vf30;
    a4::vec4 vf31 = io_vf31;
    a4::vec4 vacc = io_vacc;
    uint64_t h11 = io_h11;
    bool a3xw0 = false;
    bool a3xw1 = false;
    bool a3xw2 = false;
    bool a3xw3 = false;
    bool a3xw4 = false;
    bool a3xw5 = false;
    bool a3xw6 = false;
    (sp = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)sp, 4294967184))));
    (fp = (uint64_t)((uint64_t)a1 + (uint64_t)0ULL));
    (s4 = (uint64_t)((uint64_t)a0 + (uint64_t)0ULL));
    (s5 = (uint64_t)((uint64_t)t3 + (uint64_t)0ULL));
    (s6 = (uint64_t)((uint64_t)t1 + (uint64_t)0ULL));
    (s7 = (uint64_t)((uint64_t)t0 + (uint64_t)0ULL));
    A1_W32B((uint32_t)sp, 0, (uint32_t)a2);
    A1_W32B((uint32_t)sp, 4, (uint32_t)a3);
    { const bool c = (int64_t)fp == 0; A1_W32B((uint32_t)sp, 8, (uint32_t)t2); if (c) goto L_11163f0; }
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 2))));
    A1_W32B((uint32_t)sp, 12, (uint32_t)v0);
L_1116160:;
    { uint32_t addr = ADD32((uint32_t)s5, 7); uint32_t aligned_addr = addr & ~7u; uint32_t offset = addr & 7u; uint64_t mem = A1_R64(aligned_addr); uint32_t shift = (7u - offset) << 3; uint64_t keepMask = (shift == 0) ? 0ull : ((1ull << shift) - 1ull); (v1 = (uint64_t)((v1 & keepMask) | (mem << shift))); }
    { uint32_t addr = ADD32((uint32_t)s5, 0); uint32_t aligned_addr = addr & ~7u; uint32_t offset = addr & 7u; uint64_t mem = A1_R64(aligned_addr); uint32_t shift = offset << 3; uint64_t keepMask = (offset == 0) ? 0ull : (0xFFFFFFFFFFFFFFFFull << ((8u - offset) << 3)); (v1 = (uint64_t)((v1 & keepMask) | (mem >> shift))); }
    { uint32_t addr = ADD32((uint32_t)fp, 7); uint32_t aligned_addr = addr & ~7u; uint32_t offset = addr & 7u; uint32_t shift = (7u - offset) << 3; uint64_t mask = 0xFFFFFFFFFFFFFFFFull >> shift; uint64_t old_data = A1_R64(aligned_addr); uint64_t val = v1; uint64_t new_data = (old_data & ~mask) | ((val >> shift) & mask); A1_W64(aligned_addr, new_data); }
    { uint32_t addr = ADD32((uint32_t)fp, 0); uint32_t aligned_addr = addr & ~7u; uint32_t offset = addr & 7u; uint32_t shift = offset << 3; uint64_t mask = 0xFFFFFFFFFFFFFFFFull << shift; uint64_t old_data = A1_R64(aligned_addr); uint64_t val = v1; uint64_t new_data = (old_data & ~mask) | ((val << shift) & mask); A1_W64(aligned_addr, new_data); }
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 4294967295))));
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)s5, 0))));
    { const bool c = (int64_t)a0 == (int64_t)v1; (s3 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)s5, 4)))); if (c) goto L_1116210; }
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)SLL32((uint32_t)a0, 1))));
    (a1 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)sp, 4)))); a3xw0 = true;
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v0, (uint32_t)a0))));
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)SLL32((uint32_t)a0, 3))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)SLL32((uint32_t)v0, 5))));
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v1, (uint32_t)a0))));
    (a3 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v0, (uint32_t)a1)))); a3xw2 = true;
    (a1 = (uint64_t)((int64_t)(int32_t)((int32_t)SLL32((uint32_t)a0, 6)))); a3xw0 = true;
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)a3, 8))));
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)SLL32((uint32_t)v1, 4))));
    (a2 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)a3, 20)))); a3xw1 = true;
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v1, (uint32_t)s6))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v0, (uint32_t)s3))));
    (t0 = (uint64_t)((int64_t)(int32_t)((int32_t)SLL32((uint32_t)s3, 4)))); a3xw3 = true;
    (a0 = (uint64_t)(uint32_t)((uint8_t)A1_R8B((uint32_t)v0, 0)));
    (a1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)a1, (uint32_t)s7)))); a3xw0 = true;
    (s0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)a2, (uint32_t)t0))));
    { const bool c = (int64_t)a0 != 0; (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)v1, 4)))); if (c) goto L_1116204; }
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)t0, (uint32_t)v0))));
    vf8 = a4v::castsi128_ps(A4_R128(ADD32((uint32_t)v0, 0)));
    vf14 = a4v::castsi128_ps(A4_R128(ADD32((uint32_t)a1, 0)));
    vf15 = a4v::castsi128_ps(A4_R128(ADD32((uint32_t)a1, 16)));
    vf16 = a4v::castsi128_ps(A4_R128(ADD32((uint32_t)a1, 32)));
    vf17 = a4v::castsi128_ps(A4_R128(ADD32((uint32_t)a1, 48)));
    { a4::vec4 res = a4v::mul(vf14, a4v::bcast<0>(vf8)); vacc = a4v::dest<1, 1, 1, 1>(vacc, res); }
    { a4::vec4 mul_res = a4v::mul(vf15, a4v::bcast<1>(vf8)); a4::vec4 res = a4v::addK<1, 1>(vacc, mul_res); vacc = a4v::dest<1, 1, 1, 1>(vacc, res); }
    { a4::vec4 mul_res = a4v::mul(vf16, a4v::bcast<2>(vf8)); a4::vec4 res = a4v::addK<1, 1>(vacc, mul_res); vacc = a4v::dest<1, 1, 1, 1>(vacc, res); }
    { a4::vec4 mul_res = a4v::mul(vf17, a4v::bcast<3>(vf8)); a4::vec4 res = a4v::addK<1, 1>(vacc, mul_res); a4::q128 mask = a4v::set_epi32(-1, -1, -1, -1); vf10 = a4v::blendv_ps(vf10, res, a4v::castsi128_ps(mask)); }
    A4_W128(ADD32((uint32_t)s0, 0), a4v::castps_si128(vf10));
    (a0 = (uint64_t)((uint64_t)RC_U64X(a3) + (uint64_t)0ULL));
    { (a1 = (uint64_t)((uint64_t)RC_U64X(s3) + (uint64_t)0ULL)); a3xw0 = true; A3_CHARGE(); uint64_t a7c[5] = {0u, 0u, 0u, 0u, 0u}; v0 = EntryModifiedManager(ram, rt, a0, (uint32_t)a1, a7c); a2 = a7c[0]; a3 = a7c[1]; t0 = a7c[2]; t1 = a7c[3]; t2 = a7c[4]; a3xw1 = true; a3xw2 = true; a3xw3 = true; a3xw4 = true; a3xw5 = true; }
L_1116204:;
    A1_W32B((uint32_t)s4, 0, (uint32_t)s0);
    goto L_11163c8;
L_1116210:;
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)sp, 0))));
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)v0, 20))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)SLL32((uint32_t)s3, 4))));
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v1, (uint32_t)v0))));
    A1_W32B((uint32_t)s4, 0, (uint32_t)v1);
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)sp, 0))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)v1, 8))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v0, (uint32_t)s3))));
    (v1 = (uint64_t)(uint32_t)((uint8_t)A1_R8B((uint32_t)v0, 0)));
    { const bool c = (int64_t)v1 != 0; (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)sp, 12)))); if (c) goto L_11163cc; }
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)sp, 8))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)SLL32((uint32_t)s3, 2))));
    (a1 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)sp, 4)))); a3xw0 = true;
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v0, (uint32_t)a0))));
    (s2 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)v0, 0))));
    (s1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)s2, 4))));
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)s1, 0))));
    (t0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)s1, 4)))); a3xw3 = true;
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)SLL32((uint32_t)a0, 1))));
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)SLL32((uint32_t)a0, 3))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v0, (uint32_t)a0))));
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v1, (uint32_t)a0))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)SLL32((uint32_t)v0, 5))));
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)SLL32((uint32_t)v1, 4))));
    (a3 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v0, (uint32_t)a1)))); a3xw2 = true;
    (t1 = (uint64_t)((int64_t)(int32_t)((int32_t)SLL32((uint32_t)t0, 4)))); a3xw4 = true;
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)a3, 8))));
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v1, (uint32_t)s6))));
    (a2 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)a3, 20)))); a3xw1 = true;
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)SLL32((uint32_t)a0, 6))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v0, (uint32_t)t0))));
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)a0, (uint32_t)s7))));
    (a1 = (uint64_t)(uint32_t)((uint8_t)A1_R8B((uint32_t)v0, 0))); a3xw0 = true;
    (s0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)a2, (uint32_t)t1))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)v1, 4))));
    { const bool c = (int64_t)a1 != 0; (v1 = (uint64_t)((uint64_t)RC_U64X(s0) + (uint64_t)0ULL)); if (c) goto L_11162e4; }
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)t1, (uint32_t)v0))));
    vf8 = a4v::castsi128_ps(A4_R128(ADD32((uint32_t)v0, 0)));
    vf14 = a4v::castsi128_ps(A4_R128(ADD32((uint32_t)a0, 0)));
    vf15 = a4v::castsi128_ps(A4_R128(ADD32((uint32_t)a0, 16)));
    vf16 = a4v::castsi128_ps(A4_R128(ADD32((uint32_t)a0, 32)));
    vf17 = a4v::castsi128_ps(A4_R128(ADD32((uint32_t)a0, 48)));
    { a4::vec4 res = a4v::mul(vf14, a4v::bcast<0>(vf8)); vacc = a4v::dest<1, 1, 1, 1>(vacc, res); }
    { a4::vec4 mul_res = a4v::mul(vf15, a4v::bcast<1>(vf8)); a4::vec4 res = a4v::addK<1, 1>(vacc, mul_res); vacc = a4v::dest<1, 1, 1, 1>(vacc, res); }
    { a4::vec4 mul_res = a4v::mul(vf16, a4v::bcast<2>(vf8)); a4::vec4 res = a4v::addK<1, 1>(vacc, mul_res); vacc = a4v::dest<1, 1, 1, 1>(vacc, res); }
    { a4::vec4 mul_res = a4v::mul(vf17, a4v::bcast<3>(vf8)); a4::vec4 res = a4v::addK<1, 1>(vacc, mul_res); a4::q128 mask = a4v::set_epi32(-1, -1, -1, -1); vf10 = a4v::blendv_ps(vf10, res, a4v::castsi128_ps(mask)); }
    A4_W128(ADD32((uint32_t)s0, 0), a4v::castps_si128(vf10));
    (a0 = (uint64_t)((uint64_t)RC_U64X(a3) + (uint64_t)0ULL));
    { (a1 = (uint64_t)((uint64_t)RC_U64X(t0) + (uint64_t)0ULL)); a3xw0 = true; A3_CHARGE(); v0 = EntryModifiedManager(ram, rt, a0, (uint32_t)a1); }
    (v1 = (uint64_t)((uint64_t)s0 + (uint64_t)0ULL));
L_11162e4:;
    vf31 = a4v::castsi128_ps(A4_R128(ADD32((uint32_t)v1, 0)));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)s2, 0))));
    (s2 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v0, 4294967294))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 4294967295))));
    { const bool c = (int64_t)s2 == (int64_t)v0; (s1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)s1, 8)))); if (c) goto L_11163b4; }
    (t3 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 4294967295)))); a3xw6 = true;
L_1116300:;
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)s1, 0))));
    { a4::q128 mask = a4v::set_epi32(-1, -1, -1, -1); vf29 = a4v::blendv_ps(vf29, vf31, a4v::castsi128_ps(mask)); }
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)SLL32((uint32_t)a0, 1))));
    (a1 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)sp, 4)))); a3xw0 = true;
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v0, (uint32_t)a0))));
    (t2 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)s1, 4)))); a3xw5 = true;
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)SLL32((uint32_t)v0, 5))));
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)SLL32((uint32_t)a0, 3))));
    (t1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v0, (uint32_t)a1)))); a3xw4 = true;
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v1, (uint32_t)a0))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)t1, 8))));
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)SLL32((uint32_t)v1, 4))));
    (a1 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)t1, 20)))); a3xw0 = true;
    (t0 = (uint64_t)((int64_t)(int32_t)((int32_t)SLL32((uint32_t)t2, 4)))); a3xw3 = true;
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v0, (uint32_t)t2))));
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v1, (uint32_t)s6))));
    (a2 = (uint64_t)(uint32_t)((uint8_t)A1_R8B((uint32_t)v0, 0))); a3xw1 = true;
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)SLL32((uint32_t)a0, 6))));
    (s0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)a1, (uint32_t)t0))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)v1, 4))));
    (a3 = (uint64_t)((uint64_t)RC_U64X(s0) + (uint64_t)0ULL)); a3xw2 = true;
    { const bool c = (int64_t)a2 != 0; (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)a0, (uint32_t)s7)))); if (c) goto L_11163a0; }
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)t0, (uint32_t)v0))));
    vf8 = a4v::castsi128_ps(A4_R128(ADD32((uint32_t)v0, 0)));
    vf14 = a4v::castsi128_ps(A4_R128(ADD32((uint32_t)v1, 0)));
    vf15 = a4v::castsi128_ps(A4_R128(ADD32((uint32_t)v1, 16)));
    vf16 = a4v::castsi128_ps(A4_R128(ADD32((uint32_t)v1, 32)));
    vf17 = a4v::castsi128_ps(A4_R128(ADD32((uint32_t)v1, 48)));
    { a4::vec4 res = a4v::mul(vf14, a4v::bcast<0>(vf8)); vacc = a4v::dest<1, 1, 1, 1>(vacc, res); }
    { a4::vec4 mul_res = a4v::mul(vf15, a4v::bcast<1>(vf8)); a4::vec4 res = a4v::addK<1, 1>(vacc, mul_res); vacc = a4v::dest<1, 1, 1, 1>(vacc, res); }
    { a4::vec4 mul_res = a4v::mul(vf16, a4v::bcast<2>(vf8)); a4::vec4 res = a4v::addK<1, 1>(vacc, mul_res); vacc = a4v::dest<1, 1, 1, 1>(vacc, res); }
    { a4::vec4 mul_res = a4v::mul(vf17, a4v::bcast<3>(vf8)); a4::vec4 res = a4v::addK<1, 1>(vacc, mul_res); a4::q128 mask = a4v::set_epi32(-1, -1, -1, -1); vf10 = a4v::blendv_ps(vf10, res, a4v::castsi128_ps(mask)); }
    A4_W128(ADD32((uint32_t)s0, 0), a4v::castps_si128(vf10));
    (a0 = (uint64_t)((uint64_t)RC_U64X(t1) + (uint64_t)0ULL));
    (a1 = (uint64_t)((uint64_t)RC_U64X(t2) + (uint64_t)0ULL)); a3xw0 = true;
    { A4_W128(ADD32((uint32_t)sp, 16), a4::mk128(h11, t3)); A3_CHARGE(); v0 = EntryModifiedManager(ram, rt, a0, (uint32_t)a1); }
    do { const a4::q128 _v = (A4_R128(ADD32((uint32_t)sp, 16))); h11 = a4::hi64(_v); std::memcpy(&t3, &_v, 8); } while (0); a3xw6 = true;
    (a3 = (uint64_t)((uint64_t)s0 + (uint64_t)0ULL)); a3xw2 = true;
L_11163a0:;
    vf30 = a4v::castsi128_ps(A4_R128(ADD32((uint32_t)a3, 0)));
    { a4::vec4 res = a4v::add(vf29, vf30); a4::q128 mask = a4v::set_epi32(-1, -1, -1, -1); vf31 = a4v::blend(vf31, res, a4v::castsi128_ps(mask)); }
    (s2 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)s2, 4294967295))));
    { const bool c = (int64_t)s2 != (int64_t)t3; (s1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)s1, 8)))); if (c) { A3_BACK(); goto L_1116300; } }
L_11163b4:;
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)s4, 0))));
    A4_W128(ADD32((uint32_t)v0, 0), a4v::castps_si128(vf31));
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)sp, 0))));
    { (a1 = (uint64_t)((uint64_t)s3 + (uint64_t)0ULL)); a3xw0 = true; A3_CHARGE(); uint64_t a7c[5] = {0u, 0u, 0u, 0u, 0u}; v0 = EntryModifiedManager(ram, rt, a0, (uint32_t)a1, a7c); a2 = a7c[0]; a3 = a7c[1]; t0 = a7c[2]; t1 = a7c[3]; t2 = a7c[4]; a3xw1 = true; a3xw2 = true; a3xw3 = true; a3xw4 = true; a3xw5 = true; }
L_11163c8:;
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)sp, 12))));
L_11163cc:;
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 4294967295))));
    (s5 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)s5, 8))));
    (fp = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)fp, 8))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v0, 4294967295))));
    (s4 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)s4, 4))));
    { const bool c = (int64_t)v0 != (int64_t)v1; A1_W32B((uint32_t)sp, 12, (uint32_t)v0); if (c) { A3_BACK(); goto L_1116160; } }
    goto L_1116668;
L_11163f0:;
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 2))));
    A1_W32B((uint32_t)sp, 12, (uint32_t)a0);
L_11163f8:;
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)s5, 0))));
    (a1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 4294967295)))); a3xw0 = true;
    { const bool c = (int64_t)a0 == (int64_t)a1; (s3 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)s5, 4)))); if (c) goto L_1116498; }
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)SLL32((uint32_t)a0, 1))));
    (a1 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)sp, 4)))); a3xw0 = true;
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v0, (uint32_t)a0))));
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)SLL32((uint32_t)a0, 3))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)SLL32((uint32_t)v0, 5))));
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v1, (uint32_t)a0))));
    (a3 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v0, (uint32_t)a1)))); a3xw2 = true;
    (a1 = (uint64_t)((int64_t)(int32_t)((int32_t)SLL32((uint32_t)a0, 6)))); a3xw0 = true;
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)a3, 8))));
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)SLL32((uint32_t)v1, 4))));
    (a2 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)a3, 20)))); a3xw1 = true;
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v1, (uint32_t)s6))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v0, (uint32_t)s3))));
    (t0 = (uint64_t)((int64_t)(int32_t)((int32_t)SLL32((uint32_t)s3, 4)))); a3xw3 = true;
    (a0 = (uint64_t)(uint32_t)((uint8_t)A1_R8B((uint32_t)v0, 0)));
    (a1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)a1, (uint32_t)s7)))); a3xw0 = true;
    (s0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)a2, (uint32_t)t0))));
    { const bool c = (int64_t)a0 != 0; (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)v1, 4)))); if (c) goto L_111648c; }
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)t0, (uint32_t)v0))));
    vf8 = a4v::castsi128_ps(A4_R128(ADD32((uint32_t)v0, 0)));
    vf14 = a4v::castsi128_ps(A4_R128(ADD32((uint32_t)a1, 0)));
    vf15 = a4v::castsi128_ps(A4_R128(ADD32((uint32_t)a1, 16)));
    vf16 = a4v::castsi128_ps(A4_R128(ADD32((uint32_t)a1, 32)));
    vf17 = a4v::castsi128_ps(A4_R128(ADD32((uint32_t)a1, 48)));
    { a4::vec4 res = a4v::mul(vf14, a4v::bcast<0>(vf8)); vacc = a4v::dest<1, 1, 1, 1>(vacc, res); }
    { a4::vec4 mul_res = a4v::mul(vf15, a4v::bcast<1>(vf8)); a4::vec4 res = a4v::addK<1, 1>(vacc, mul_res); vacc = a4v::dest<1, 1, 1, 1>(vacc, res); }
    { a4::vec4 mul_res = a4v::mul(vf16, a4v::bcast<2>(vf8)); a4::vec4 res = a4v::addK<1, 1>(vacc, mul_res); vacc = a4v::dest<1, 1, 1, 1>(vacc, res); }
    { a4::vec4 mul_res = a4v::mul(vf17, a4v::bcast<3>(vf8)); a4::vec4 res = a4v::addK<1, 1>(vacc, mul_res); a4::q128 mask = a4v::set_epi32(-1, -1, -1, -1); vf10 = a4v::blendv_ps(vf10, res, a4v::castsi128_ps(mask)); }
    A4_W128(ADD32((uint32_t)s0, 0), a4v::castps_si128(vf10));
    (a0 = (uint64_t)((uint64_t)RC_U64X(a3) + (uint64_t)0ULL));
    { (a1 = (uint64_t)((uint64_t)RC_U64X(s3) + (uint64_t)0ULL)); a3xw0 = true; A3_CHARGE(); uint64_t a7c[5] = {0u, 0u, 0u, 0u, 0u}; v0 = EntryModifiedManager(ram, rt, a0, (uint32_t)a1, a7c); a2 = a7c[0]; a3 = a7c[1]; t0 = a7c[2]; t1 = a7c[3]; t2 = a7c[4]; a3xw1 = true; a3xw2 = true; a3xw3 = true; a3xw4 = true; a3xw5 = true; }
L_111648c:;
    A1_W32B((uint32_t)s4, 0, (uint32_t)s0);
    goto L_1116648;
L_1116498:;
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)sp, 0))));
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)v0, 20))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)SLL32((uint32_t)s3, 4))));
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v1, (uint32_t)v0))));
    A1_W32B((uint32_t)s4, 0, (uint32_t)v1);
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)sp, 0))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)v1, 8))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v0, (uint32_t)s3))));
    (v1 = (uint64_t)(uint32_t)((uint8_t)A1_R8B((uint32_t)v0, 0)));
    { const bool c = (int64_t)v1 != 0; (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)sp, 12)))); if (c) goto L_111664c; }
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)sp, 8))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)SLL32((uint32_t)s3, 2))));
    (a1 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)sp, 4)))); a3xw0 = true;
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v0, (uint32_t)a0))));
    (s2 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)v0, 0))));
    (s1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)s2, 4))));
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)s1, 0))));
    (t0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)s1, 4)))); a3xw3 = true;
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)SLL32((uint32_t)a0, 1))));
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)SLL32((uint32_t)a0, 3))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v0, (uint32_t)a0))));
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v1, (uint32_t)a0))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)SLL32((uint32_t)v0, 5))));
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)SLL32((uint32_t)v1, 4))));
    (a3 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v0, (uint32_t)a1)))); a3xw2 = true;
    (t1 = (uint64_t)((int64_t)(int32_t)((int32_t)SLL32((uint32_t)t0, 4)))); a3xw4 = true;
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)a3, 8))));
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v1, (uint32_t)s6))));
    (a2 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)a3, 20)))); a3xw1 = true;
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)SLL32((uint32_t)a0, 6))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v0, (uint32_t)t0))));
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)a0, (uint32_t)s7))));
    (a1 = (uint64_t)(uint32_t)((uint8_t)A1_R8B((uint32_t)v0, 0))); a3xw0 = true;
    (s0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)a2, (uint32_t)t1))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)v1, 4))));
    { const bool c = (int64_t)a1 != 0; (v1 = (uint64_t)((uint64_t)RC_U64X(s0) + (uint64_t)0ULL)); if (c) goto L_111656c; }
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)t1, (uint32_t)v0))));
    vf8 = a4v::castsi128_ps(A4_R128(ADD32((uint32_t)v0, 0)));
    vf14 = a4v::castsi128_ps(A4_R128(ADD32((uint32_t)a0, 0)));
    vf15 = a4v::castsi128_ps(A4_R128(ADD32((uint32_t)a0, 16)));
    vf16 = a4v::castsi128_ps(A4_R128(ADD32((uint32_t)a0, 32)));
    vf17 = a4v::castsi128_ps(A4_R128(ADD32((uint32_t)a0, 48)));
    { a4::vec4 res = a4v::mul(vf14, a4v::bcast<0>(vf8)); vacc = a4v::dest<1, 1, 1, 1>(vacc, res); }
    { a4::vec4 mul_res = a4v::mul(vf15, a4v::bcast<1>(vf8)); a4::vec4 res = a4v::addK<1, 1>(vacc, mul_res); vacc = a4v::dest<1, 1, 1, 1>(vacc, res); }
    { a4::vec4 mul_res = a4v::mul(vf16, a4v::bcast<2>(vf8)); a4::vec4 res = a4v::addK<1, 1>(vacc, mul_res); vacc = a4v::dest<1, 1, 1, 1>(vacc, res); }
    { a4::vec4 mul_res = a4v::mul(vf17, a4v::bcast<3>(vf8)); a4::vec4 res = a4v::addK<1, 1>(vacc, mul_res); a4::q128 mask = a4v::set_epi32(-1, -1, -1, -1); vf10 = a4v::blendv_ps(vf10, res, a4v::castsi128_ps(mask)); }
    A4_W128(ADD32((uint32_t)s0, 0), a4v::castps_si128(vf10));
    (a0 = (uint64_t)((uint64_t)RC_U64X(a3) + (uint64_t)0ULL));
    { (a1 = (uint64_t)((uint64_t)RC_U64X(t0) + (uint64_t)0ULL)); a3xw0 = true; A3_CHARGE(); v0 = EntryModifiedManager(ram, rt, a0, (uint32_t)a1); }
    (v1 = (uint64_t)((uint64_t)s0 + (uint64_t)0ULL));
L_111656c:;
    vf31 = a4v::castsi128_ps(A4_R128(ADD32((uint32_t)v1, 0)));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)s2, 0))));
    (s2 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v0, 4294967294))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 4294967295))));
    { const bool c = (int64_t)s2 == (int64_t)v0; (s1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)s1, 8)))); if (c) goto L_1116634; }
    (fp = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 4294967295))));
L_1116588:;
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)s1, 0))));
    { a4::q128 mask = a4v::set_epi32(-1, -1, -1, -1); vf29 = a4v::blendv_ps(vf29, vf31, a4v::castsi128_ps(mask)); }
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)SLL32((uint32_t)a0, 1))));
    (a1 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)sp, 4)))); a3xw0 = true;
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v0, (uint32_t)a0))));
    (t2 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)s1, 4)))); a3xw5 = true;
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)SLL32((uint32_t)v0, 5))));
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)SLL32((uint32_t)a0, 3))));
    (t1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v0, (uint32_t)a1)))); a3xw4 = true;
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v1, (uint32_t)a0))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)t1, 8))));
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)SLL32((uint32_t)v1, 4))));
    (a1 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)t1, 20)))); a3xw0 = true;
    (t0 = (uint64_t)((int64_t)(int32_t)((int32_t)SLL32((uint32_t)t2, 4)))); a3xw3 = true;
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v0, (uint32_t)t2))));
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v1, (uint32_t)s6))));
    (a2 = (uint64_t)(uint32_t)((uint8_t)A1_R8B((uint32_t)v0, 0))); a3xw1 = true;
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)SLL32((uint32_t)a0, 6))));
    (s0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)a1, (uint32_t)t0))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)v1, 4))));
    (a3 = (uint64_t)((uint64_t)RC_U64X(s0) + (uint64_t)0ULL)); a3xw2 = true;
    { const bool c = (int64_t)a2 != 0; (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)a0, (uint32_t)s7)))); if (c) goto L_1116620; }
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)t0, (uint32_t)v0))));
    vf8 = a4v::castsi128_ps(A4_R128(ADD32((uint32_t)v0, 0)));
    vf14 = a4v::castsi128_ps(A4_R128(ADD32((uint32_t)v1, 0)));
    vf15 = a4v::castsi128_ps(A4_R128(ADD32((uint32_t)v1, 16)));
    vf16 = a4v::castsi128_ps(A4_R128(ADD32((uint32_t)v1, 32)));
    vf17 = a4v::castsi128_ps(A4_R128(ADD32((uint32_t)v1, 48)));
    { a4::vec4 res = a4v::mul(vf14, a4v::bcast<0>(vf8)); vacc = a4v::dest<1, 1, 1, 1>(vacc, res); }
    { a4::vec4 mul_res = a4v::mul(vf15, a4v::bcast<1>(vf8)); a4::vec4 res = a4v::addK<1, 1>(vacc, mul_res); vacc = a4v::dest<1, 1, 1, 1>(vacc, res); }
    { a4::vec4 mul_res = a4v::mul(vf16, a4v::bcast<2>(vf8)); a4::vec4 res = a4v::addK<1, 1>(vacc, mul_res); vacc = a4v::dest<1, 1, 1, 1>(vacc, res); }
    { a4::vec4 mul_res = a4v::mul(vf17, a4v::bcast<3>(vf8)); a4::vec4 res = a4v::addK<1, 1>(vacc, mul_res); a4::q128 mask = a4v::set_epi32(-1, -1, -1, -1); vf10 = a4v::blendv_ps(vf10, res, a4v::castsi128_ps(mask)); }
    A4_W128(ADD32((uint32_t)s0, 0), a4v::castps_si128(vf10));
    (a0 = (uint64_t)((uint64_t)RC_U64X(t1) + (uint64_t)0ULL));
    { (a1 = (uint64_t)((uint64_t)RC_U64X(t2) + (uint64_t)0ULL)); a3xw0 = true; A3_CHARGE(); v0 = EntryModifiedManager(ram, rt, a0, (uint32_t)a1); }
    (a3 = (uint64_t)((uint64_t)s0 + (uint64_t)0ULL)); a3xw2 = true;
L_1116620:;
    vf30 = a4v::castsi128_ps(A4_R128(ADD32((uint32_t)a3, 0)));
    { a4::vec4 res = a4v::add(vf29, vf30); a4::q128 mask = a4v::set_epi32(-1, -1, -1, -1); vf31 = a4v::blend(vf31, res, a4v::castsi128_ps(mask)); }
    (s2 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)s2, 4294967295))));
    { const bool c = (int64_t)s2 != (int64_t)fp; (s1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)s1, 8)))); if (c) { A3_BACK(); goto L_1116588; } }
L_1116634:;
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)s4, 0))));
    A4_W128(ADD32((uint32_t)v0, 0), a4v::castps_si128(vf31));
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)sp, 0))));
    { (a1 = (uint64_t)((uint64_t)s3 + (uint64_t)0ULL)); a3xw0 = true; A3_CHARGE(); uint64_t a7c[5] = {0u, 0u, 0u, 0u, 0u}; v0 = EntryModifiedManager(ram, rt, a0, (uint32_t)a1, a7c); a2 = a7c[0]; a3 = a7c[1]; t0 = a7c[2]; t1 = a7c[3]; t2 = a7c[4]; a3xw1 = true; a3xw2 = true; a3xw3 = true; a3xw4 = true; a3xw5 = true; }
L_1116648:;
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)sp, 12))));
L_111664c:;
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 4294967295))));
    (s5 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)s5, 8))));
    (s4 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)s4, 4))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v0, 4294967295))));
    { const bool c = (int64_t)v0 != (int64_t)v1; A1_W32B((uint32_t)sp, 12, (uint32_t)v0); if (c) { A3_BACK(); goto L_11163f8; } }
L_1116668:;
    { (sp = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)sp, 112)))); if (xo) { if (a3xw0) xo[0] = a1; if (a3xw1) xo[1] = a2; if (a3xw2) xo[2] = a3; if (a3xw3) xo[3] = t0; if (a3xw4) xo[4] = t1; if (a3xw5) xo[5] = t2; if (a3xw6) xo[6] = t3; } io_vf8 = vf8; io_vf10 = vf10; io_vf14 = vf14; io_vf15 = vf15; io_vf16 = vf16; io_vf17 = vf17; io_vf29 = vf29; io_vf30 = vf30; io_vf31 = vf31; io_vacc = vacc; io_h11 = h11; return v0; }
    __builtin_unreachable();
}

// CopyMultiMatrix (0x1132ec8): typed by tools/typed_lift2.py (round A6; loop frame calls, extra outputs xo[0]=a1 xo[1]=a2 xo[2]=a3 xo[3]=t0 xo[4]=t1; VU0 in -, out h6 h7 h8 h9, relocated calls 1, frame 48 bytes, 0 slot accesses)
inline void CopyMultiMatrix(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint64_t a0, uint64_t a1, uint64_t a2, float f12, float f13, float f14, float f15, float f16, float f17, float f18, float f19, uint64_t &io_h6, uint64_t &io_h7, uint64_t &io_h8, uint64_t &io_h9, uint64_t *xo = nullptr)
{
    uint64_t a3 = 0;
    uint64_t t0 = 0;
    uint64_t t1 = 0;
    uint64_t s0 = 0;
    uint64_t s1 = 0;
    uint64_t s2 = 0;
    uint64_t s3 = 0;
    uint64_t h6 = io_h6;
    uint64_t h7 = io_h7;
    uint64_t h8 = io_h8;
    uint64_t h9 = io_h9;
    bool a3xw0 = false;
    bool a3xw1 = false;
    bool a3xw2 = false;
    bool a3xw3 = false;
    bool a3xw4 = false;
    (s0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)a2, 4294967295))));
    (s1 = (uint64_t)((uint64_t)a1 + (uint64_t)0ULL));
    (s2 = (uint64_t)((uint64_t)a0 + (uint64_t)0ULL));
    if ((int64_t)a2 == 0) goto L_1132f14;
    (s3 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 4294967295))));
    (a0 = (uint64_t)((uint64_t)s2 + (uint64_t)0ULL));
L_1132ef8:;
    (s2 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)s2, 64))));
    (a1 = (uint64_t)((uint64_t)s1 + (uint64_t)0ULL)); a3xw0 = true;
    (s1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)s1, 64))));
    { (s0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)s0, 4294967295)))); A3_CHARGE(); uint64_t a7c[4] = {0u, 0u, 0u, 0u}; iosCopyMatrix_x8(ram, rt, (uint32_t)a0, (uint32_t)a1, h6, h7, h8, h9, a7c); a2 = a7c[0]; a3 = a7c[1]; t0 = a7c[2]; t1 = a7c[3]; a3xw1 = true; a3xw2 = true; a3xw3 = true; a3xw4 = true; }
    { const bool c = (int64_t)s0 != (int64_t)s3; (a0 = (uint64_t)((uint64_t)s2 + (uint64_t)0ULL)); if (c) { A3_BACK(); goto L_1132ef8; } }
L_1132f14:;
    { if (xo) { if (a3xw0) xo[0] = a1; if (a3xw1) xo[1] = a2; if (a3xw2) xo[2] = a3; if (a3xw3) xo[3] = t0; if (a3xw4) xo[4] = t1; } io_h6 = h6; io_h7 = h7; io_h8 = h8; io_h9 = h9; return; }
    __builtin_unreachable();
}

// get_top_emitter_list_from_particle_geodisp (0x113f378): typed by tools/typed_lift2.py (round A6; frame calls; VU0 in -, out h2, frame 16 bytes, 0 slot accesses)
inline uint64_t get_top_emitter_list_from_particle_geodisp(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint64_t v0, uint64_t a0, float f12, float f13, float f14, float f15, float f16, float f17, float f18, float f19, uint64_t &io_h2)
{
    uint64_t h2 = io_h2;
    if ((int64_t)a0 == 0) goto L_113f398;
    { A3_CHARGE(); v0 = GetGeoDispClass(ram, rt, a0, h2); }
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v0, 232))));
    goto L_113f39c;
L_113f398:;
    (v0 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL));
L_113f39c:;
    { io_h2 = h2; return v0; }
    __builtin_unreachable();
}

// gamecore_sub_014627E8 (0x14627e8): typed by tools/typed_lift2.py (round A6; loop frame calls; VU0 in vf0 vf7 vacc vstat h4 h6 h9 h10, out vf5 vf6 vf7 vacc vq vstat h2 h3 h5, guest sp, frame 16 bytes, 0 slot accesses)
inline a1::RetVF gamecore_sub_014627E8(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint64_t a0, uint64_t a1, uint64_t a2, uint32_t sp, const a4::vec4 &io_vf0, a4::vec4 &io_vf5, a4::vec4 &io_vf6, a4::vec4 &io_vf7, a4::vec4 &io_vacc, float &io_vq, uint16_t &io_vstat, uint64_t &io_h2, uint64_t &io_h3, uint64_t io_h4, uint64_t &io_h5, uint64_t io_h6, uint64_t io_h9, uint64_t io_h10)
{
    float f0 = 0.0f;
    float f1 = 0.0f;
    uint64_t v0 = 0;
    uint64_t v1 = 0;
    uint32_t a3 = 0;
    uint64_t t0 = 0;
    uint64_t t1 = 0;
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
    uint64_t h4 = io_h4;
    uint64_t h5 = io_h5;
    uint64_t h6 = io_h6;
    uint64_t h9 = io_h9;
    uint64_t h10 = io_h10;
    (t0 = (uint64_t)((uint64_t)a1 + (uint64_t)0ULL));
    (s0 = (uint64_t)((uint64_t)a0 + (uint64_t)0ULL));
    (v0 = (uint64_t)((int64_t)(int32_t)((int8_t)A1_R8B((uint32_t)s0, 1548))));
    { const bool c = (int64_t)v0 == 0; (t1 = (uint64_t)((uint64_t)a2 + (uint64_t)0ULL)); if (c) goto L_1462840; }
    a3 = 1u;
    (a1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)s0, 1549))));
    (a2 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 31))));
L_1462818:;
    do { const a4::q128 _v = (A4_R128(ADD32((uint32_t)t0, 0))); h3 = a4::hi64(_v); std::memcpy(&v1, &_v, 8); } while (0);
    (a2 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)a2, 4294967295))));
    A4_W128(ADD32((uint32_t)a0, 0), a4::mk128(h3, v1));
    do { const a4::q128 _v = (A4_R128(ADD32((uint32_t)t1, 0))); h2 = a4::hi64(_v); std::memcpy(&v0, &_v, 8); } while (0);
    A4_W128(ADD32((uint32_t)a0, 16), a4::mk128(h2, v0));
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)a0, 48))));
    A1_W8B((uint32_t)a1, 0, (uint8_t)a3);
    { const bool c = (int32_t)a2 >= 0; (a1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)a1, 1)))); if (c) { A3_BACK(); goto L_1462818; } }
    A1_W8B((uint32_t)s0, 1548, (uint8_t)0u);
L_1462840:;
    (a3 = (uint32_t)((int32_t)((int32_t)A1_R32B((uint32_t)s0, 1540))));
    (a1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(a3, 31))));
    (a2 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(a3, 62))));
    (v1 = (uint64_t)(((int64_t)(int64_t)RC_U64X(a1) < (int64_t)(int32_t)0) ? 1 : 0));
    (v0 = (uint64_t)((uint64_t)RC_U64X(a1) + (uint64_t)0ULL));
    if (RC_U64X(v1) != 0) do { const a4::q128 _v = (a4::mk128(h6, a2)); h2 = a4::hi64(_v); std::memcpy(&v0, &_v, 8); } while (0);
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)SLL32(a3, 1))));
    (v0 = (uint64_t)((int64_t)(int32_t)(SRA32((int32_t)v0, 5))));
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)a0, a3))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)SLL32((uint32_t)v0, 5))));
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)SLL32((uint32_t)a0, 4))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)SUB32((uint32_t)a1, (uint32_t)v0))));
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)a0, (uint32_t)s0))));
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)SLL32((uint32_t)v0, 1))));
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v1, (uint32_t)v0))));
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)SLL32((uint32_t)v1, 4))));
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v1, (uint32_t)s0))));
    vf5 = a4v::castsi128_ps(A4_R128(ADD32((uint32_t)v1, 0)));
    vf6 = a4v::castsi128_ps(A4_R128(ADD32((uint32_t)a0, 0)));
    { a4::vec4 res = a4v::sub(vf5, vf6); a4::q128 mask = a4v::set_epi32(-1, -1, -1, -1); vf7 = a4v::blend(vf7, res, a4v::castsi128_ps(mask)); }
    { a4::vec4 res = a4v::mulK<1, 1>(vf7, vf7); a4::q128 mask = a4v::set_epi32(0, -1, -1, -1); vf7 = a4v::blend(vf7, res, a4v::castsi128_ps(mask)); }
    { a4::vec4 res = a4v::addK<1, 1>(vf7, a4v::bcast<1>(vf7)); a4::q128 mask = a4v::set_epi32(0, 0, 0, -1); vf7 = a4v::blendv_ps(vf7, res, a4v::castsi128_ps(mask)); }
    { a4::vec4 res = a4v::addK<1, 1>(vf7, a4v::bcast<2>(vf7)); a4::q128 mask = a4v::set_epi32(0, 0, 0, -1); vf7 = a4v::blendv_ps(vf7, res, a4v::castsi128_ps(mask)); }
    do { const a4::q128 _v = (a4v::castps_si128(vf7)); h2 = a4::hi64(_v); std::memcpy(&v0, &_v, 8); } while (0);
    { uint32_t bits = (uint32_t)v0; std::memcpy(&f1, &bits, sizeof(bits)); }
    { uint32_t bits = A1_R32B((uint32_t)s0, 1536); float f; std::memcpy(&f, &bits, sizeof(f)); f0 = f; }
    f0 = a1f::mul(f0, f0);
    fcr31 = (a1f::lt(f0, f1)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
    { const bool c = (fcr31 & 0x800000u) == 0u; (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)SLL32(a3, 1)))); if (c) goto L_14628e0; }
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(a3, 1))));
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 32))));
    { const bool c = (int64_t)v0 != (int64_t)v1; A1_W32B((uint32_t)s0, 1540, (uint32_t)v0); if (c) goto L_14628cc; }
    A1_W32B((uint32_t)s0, 1540, 0u);
L_14628cc:;
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)s0, 1540))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v0, (uint32_t)s0))));
    A1_W8B((uint32_t)v0, 1549, (uint8_t)0u);
    (a3 = (uint32_t)((int32_t)((int32_t)A1_R32B((uint32_t)s0, 1540))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)SLL32(a3, 1))));
L_14628e0:;
    do { const a4::q128 _v = (A4_R128(ADD32((uint32_t)t0, 0))); h5 = a4::hi64(_v); std::memcpy(&a1, &_v, 8); } while (0);
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v0, a3))));
    (a0 = (uint64_t)((uint64_t)s0 + (uint64_t)0ULL));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)SLL32((uint32_t)v0, 4))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v0, (uint32_t)s0))));
    A4_W128(ADD32((uint32_t)v0, 0), a4::mk128(h5, a1));
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)s0, 1540))));
    do { const a4::q128 _v = (A4_R128(ADD32((uint32_t)t1, 0))); h5 = a4::hi64(_v); std::memcpy(&a1, &_v, 8); } while (0);
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)SLL32((uint32_t)v1, 1))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v0, (uint32_t)v1))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)SLL32((uint32_t)v0, 4))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v0, (uint32_t)s0))));
    { A4_W128(ADD32((uint32_t)v0, 16), a4::mk128(h5, a1)); A3_CHARGE(); { const a1::RetVF a2_r = gamecore_sub_01462628(ram, rt, a0, (uint32_t)(sp + 0xFFFFFFF0u), vf0, vf5, vf6, vf7, vacc, vq, vstat, h2, h3, h4, h9, h10); v0 = a2_r.v0; f0 = a2_r.f0; } }
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 1))));
    A1_W32B((uint32_t)s0, 1544, (uint32_t)v0);
    { io_vf5 = vf5; io_vf6 = vf6; io_vf7 = vf7; io_vacc = vacc; io_vq = vq; io_vstat = vstat; io_h2 = h2; io_h3 = h3; io_h5 = h5; return a1::RetVF{(uint64_t)v0, f0}; }
    __builtin_unreachable();
}

// sub_00101B88 (0x101b88): typed by tools/typed_lift2.py (round A6; loop guest frame calls, extra outputs xo[0]=t0 xo[1]=t1; VU0 in h4, out h2)
inline uint64_t sub_00101B88(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint64_t a1, uint64_t a2, uint64_t t2, uint64_t t3, uint64_t gp, uint64_t sp, float f12, float f13, float f14, float f15, float f16, float f17, float f18, float f19, uint64_t &io_h2, uint64_t io_h4, uint64_t *xo = nullptr)
{
    uint64_t v0 = 0;
    uint64_t v1 = 0;
    uint64_t a0 = 0;
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
    uint64_t h2 = io_h2;
    uint64_t h4 = io_h4;
    (sp = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)sp, 4294967216))));
    (s6 = (uint64_t)((uint64_t)a2 + (uint64_t)0ULL));
    (s7 = (uint64_t)((uint64_t)a1 + (uint64_t)0ULL));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)s6, 0))));
    (s3 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)s6, 8))));
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)v0, 56))));
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)v0, 60))));
    (a2 = (uint64_t)((uint64_t)RC_U64X(s3) + (uint64_t)0ULL));
    (s5 = (uint64_t)((int64_t)(int32_t)(SRA32((int32_t)v1, 1))));
    (s1 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)s6, 12))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)SLL32((uint32_t)s5, 3))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)SUB32((uint32_t)v0, (uint32_t)s5))));
    (t0 = (uint64_t)((uint64_t)RC_U64X(s1) + (uint64_t)0ULL));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)SLL32((uint32_t)v0, 2))));
    { const bool c = (int32_t)s5 <= 0; (s2 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)a0, (uint32_t)v0)))); if (c) goto L_101c78; }
    (s4 = (uint64_t)((uint64_t)RC_U64X(s5) + (uint64_t)0ULL));
L_101bf0:;
    (s0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)s2, 4))));
    (a0 = (uint64_t)((uint64_t)s1 + (uint64_t)0ULL));
    { uint32_t addr = ADD32((uint32_t)s2, 7); uint32_t aligned_addr = addr & ~7u; uint32_t offset = addr & 7u; uint64_t mem = A1_R64(aligned_addr); uint32_t shift = (7u - offset) << 3; uint64_t keepMask = (shift == 0) ? 0ull : ((1ull << shift) - 1ull); (v0 = (uint64_t)((v0 & keepMask) | (mem << shift))); }
    { uint32_t addr = ADD32((uint32_t)s2, 0); uint32_t aligned_addr = addr & ~7u; uint32_t offset = addr & 7u; uint64_t mem = A1_R64(aligned_addr); uint32_t shift = offset << 3; uint64_t keepMask = (offset == 0) ? 0ull : (0xFFFFFFFFFFFFFFFFull << ((8u - offset) << 3)); (v0 = (uint64_t)((v0 & keepMask) | (mem >> shift))); }
    { uint32_t addr = ADD32((uint32_t)s2, 15); uint32_t aligned_addr = addr & ~7u; uint32_t offset = addr & 7u; uint64_t mem = A1_R64(aligned_addr); uint32_t shift = (7u - offset) << 3; uint64_t keepMask = (shift == 0) ? 0ull : ((1ull << shift) - 1ull); (v1 = (uint64_t)((v1 & keepMask) | (mem << shift))); }
    { uint32_t addr = ADD32((uint32_t)s2, 8); uint32_t aligned_addr = addr & ~7u; uint32_t offset = addr & 7u; uint64_t mem = A1_R64(aligned_addr); uint32_t shift = offset << 3; uint64_t keepMask = (offset == 0) ? 0ull : (0xFFFFFFFFFFFFFFFFull << ((8u - offset) << 3)); (v1 = (uint64_t)((v1 & keepMask) | (mem >> shift))); }
    { uint32_t addr = ADD32((uint32_t)s2, 23); uint32_t aligned_addr = addr & ~7u; uint32_t offset = addr & 7u; uint64_t mem = A1_R64(aligned_addr); uint32_t shift = (7u - offset) << 3; uint64_t keepMask = (shift == 0) ? 0ull : ((1ull << shift) - 1ull); (a1 = (uint64_t)((a1 & keepMask) | (mem << shift))); }
    { uint32_t addr = ADD32((uint32_t)s2, 16); uint32_t aligned_addr = addr & ~7u; uint32_t offset = addr & 7u; uint64_t mem = A1_R64(aligned_addr); uint32_t shift = offset << 3; uint64_t keepMask = (offset == 0) ? 0ull : (0xFFFFFFFFFFFFFFFFull << ((8u - offset) << 3)); (a1 = (uint64_t)((a1 & keepMask) | (mem >> shift))); }
    (a2 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)s2, 24))));
    { uint32_t addr = ADD32((uint32_t)s3, 7); uint32_t aligned_addr = addr & ~7u; uint32_t offset = addr & 7u; uint32_t shift = (7u - offset) << 3; uint64_t mask = 0xFFFFFFFFFFFFFFFFull >> shift; uint64_t old_data = A1_R64(aligned_addr); uint64_t val = v0; uint64_t new_data = (old_data & ~mask) | ((val >> shift) & mask); A1_W64(aligned_addr, new_data); }
    { uint32_t addr = ADD32((uint32_t)s3, 0); uint32_t aligned_addr = addr & ~7u; uint32_t offset = addr & 7u; uint32_t shift = offset << 3; uint64_t mask = 0xFFFFFFFFFFFFFFFFull << shift; uint64_t old_data = A1_R64(aligned_addr); uint64_t val = v0; uint64_t new_data = (old_data & ~mask) | ((val << shift) & mask); A1_W64(aligned_addr, new_data); }
    { uint32_t addr = ADD32((uint32_t)s3, 15); uint32_t aligned_addr = addr & ~7u; uint32_t offset = addr & 7u; uint32_t shift = (7u - offset) << 3; uint64_t mask = 0xFFFFFFFFFFFFFFFFull >> shift; uint64_t old_data = A1_R64(aligned_addr); uint64_t val = v1; uint64_t new_data = (old_data & ~mask) | ((val >> shift) & mask); A1_W64(aligned_addr, new_data); }
    { uint32_t addr = ADD32((uint32_t)s3, 8); uint32_t aligned_addr = addr & ~7u; uint32_t offset = addr & 7u; uint32_t shift = offset << 3; uint64_t mask = 0xFFFFFFFFFFFFFFFFull << shift; uint64_t old_data = A1_R64(aligned_addr); uint64_t val = v1; uint64_t new_data = (old_data & ~mask) | ((val << shift) & mask); A1_W64(aligned_addr, new_data); }
    { uint32_t addr = ADD32((uint32_t)s3, 23); uint32_t aligned_addr = addr & ~7u; uint32_t offset = addr & 7u; uint32_t shift = (7u - offset) << 3; uint64_t mask = 0xFFFFFFFFFFFFFFFFull >> shift; uint64_t old_data = A1_R64(aligned_addr); uint64_t val = a1; uint64_t new_data = (old_data & ~mask) | ((val >> shift) & mask); A1_W64(aligned_addr, new_data); }
    { uint32_t addr = ADD32((uint32_t)s3, 16); uint32_t aligned_addr = addr & ~7u; uint32_t offset = addr & 7u; uint32_t shift = offset << 3; uint64_t mask = 0xFFFFFFFFFFFFFFFFull << shift; uint64_t old_data = A1_R64(aligned_addr); uint64_t val = a1; uint64_t new_data = (old_data & ~mask) | ((val << shift) & mask); A1_W64(aligned_addr, new_data); }
    (s4 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)s4, 4294967295))));
    (s0 = (uint64_t)((int64_t)(int32_t)((int32_t)SLL32((uint32_t)s0, 3))));
    A1_W32B((uint32_t)s3, 24, (uint32_t)a2);
    (a2 = (uint64_t)((uint64_t)RC_U64X(s0) + (uint64_t)0ULL));
    { (a1 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)s2, 12)))); A3_CHARGE(); v0 = a6x::memcpy(ram, rt, a0, (uint32_t)a1, (uint32_t)a2, h2, h4); }
    A1_W32B((uint32_t)s3, 12, (uint32_t)s1);
    (a1 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)s2, 16))));
    (s2 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)s2, 28))));
    (s1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)s1, (uint32_t)s0))));
    (a2 = (uint64_t)((uint64_t)s0 + (uint64_t)0ULL));
    { (a0 = (uint64_t)((uint64_t)RC_U64X(s1) + (uint64_t)0ULL)); A3_CHARGE(); v0 = a6x::memcpy(ram, rt, a0, (uint32_t)a1, (uint32_t)a2, h2, h4); }
    A1_W32B((uint32_t)s3, 16, (uint32_t)s1);
    (s1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)s1, (uint32_t)s0))));
    { const bool c = (int64_t)s4 != 0; (s3 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)s3, 28)))); if (c) { A3_BACK(); goto L_101bf0; } }
    (a2 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)s6, 8))));
    (t0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)s6, 12))));
L_101c78:;
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)19 << 16))));
    (a3 = (uint64_t)((uint64_t)s3 + (uint64_t)0ULL));
    (t1 = (uint64_t)((uint64_t)s1 + (uint64_t)0ULL));
    (a1 = (uint64_t)((uint64_t)s7 + (uint64_t)0ULL));
    { (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)a0, 25600)))); A3_CHARGE(); v0 = LoaderSysPrintf(ram, rt, a0, a1, a2, a3, t0, t1, t2, t3, sp, f12, f13, f14, f15, f16, f17, f18, f19, h2); }
    A1_W32B((uint32_t)s6, 4, (uint32_t)s5);
    (v0 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL));
    A1_W32B((uint32_t)gp, -32620, (uint32_t)s7);
    { (sp = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)sp, 80)))); if (xo) { xo[0] = t0; xo[1] = t1; } io_h2 = h2; return v0; }
    __builtin_unreachable();
}

// loaderSetResetCallback (0x103f98): typed by tools/typed_lift2.py (round A6; frame calls; VU0 in -, out h2, 64-bit inputs a1 a2 a3 t0 t1 t2 t3, guest sp, frame 16 bytes, 0 slot accesses)
inline uint64_t loaderSetResetCallback(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint64_t a0, uint64_t w64_a1, uint64_t w64_a2, uint64_t w64_a3, uint64_t w64_t0, uint64_t w64_t1, uint64_t w64_t2, uint64_t w64_t3, uint32_t gp, uint32_t sp, float f12, float f13, float f14, float f15, float f16, float f17, float f18, float f19, uint64_t &io_h2)
{
    uint32_t at = 0;
    uint64_t v0 = 0;
    uint32_t v1 = 0;
    uint32_t a1 = (uint32_t)w64_a1;
    uint32_t a2 = (uint32_t)w64_a2;
    uint32_t a3 = (uint32_t)w64_a3;
    uint32_t t0 = (uint32_t)w64_t0;
    uint32_t t1 = (uint32_t)w64_t1;
    uint32_t t2 = (uint32_t)w64_t2;
    uint32_t t3 = (uint32_t)w64_t3;
    uint64_t s0 = 0;
    uint64_t h2 = io_h2;
    (v1 = (uint32_t)((int32_t)((int32_t)A1_R32B(gp, -31988))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 10))));
    { const bool c = (int64_t)(int32_t)v1 != (int64_t)v0; (s0 = (uint64_t)((uint64_t)a0 + (uint64_t)0ULL)); if (c) goto L_103fc4; }
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)19 << 16))));
    { (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)a0, 27952)))); A3_CHARGE(); v0 = LoaderSysPrintf(ram, rt, a0, w64_a1, w64_a2, w64_a3, w64_t0, w64_t1, w64_t2, w64_t3, a1_sx64((uint32_t)(sp + 0xFFFFFFF0u)), f12, f13, f14, f15, f16, f17, f18, f19, h2); }
    A1_W32B(gp, -31988, 0u);
L_103fc4:;
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(gp, 4294935308))));
    (v1 = (uint32_t)((int32_t)((int32_t)A1_R32B((uint32_t)v0, 0))));
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)SLL32(v1, 2))));
    v1 = v1 + 1u;
    at = 0x140000u;
    (at = (uint32_t)((int32_t)((int32_t)ADD32(at, (uint32_t)a0))));
    A1_W32B(at, -12384, (uint32_t)s0);
    A1_W32B((uint32_t)v0, 0, v1);
    { io_h2 = h2; return v0; }
    __builtin_unreachable();
}

// _putString (0x1056d8): typed by tools/typed_lift2.py (round A6; loop guest frame calls; VU0 in h4, out h2)
inline uint64_t _putString(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint64_t v0, uint64_t a0, uint64_t a1, uint64_t a2, uint64_t a3, uint64_t t0, uint64_t t1, uint64_t t2, uint64_t t3, uint64_t gp, uint64_t sp, float f12, float f13, float f14, float f15, float f16, float f17, float f18, float f19, uint64_t &io_h2, uint64_t io_h4)
{
    uint64_t v1 = 0;
    uint64_t s0 = 0;
    uint64_t s1 = 0;
    uint64_t s2 = 0;
    uint64_t s3 = 0;
    uint64_t s4 = 0;
    uint64_t s5 = 0;
    uint64_t s6 = 0;
    uint64_t s7 = 0;
    uint64_t h2 = io_h2;
    uint64_t h4 = io_h4;
    (sp = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)sp, 4294967216))));
    (s2 = (uint64_t)((uint64_t)a1 + (uint64_t)0ULL));
    (s5 = (uint64_t)((uint64_t)a0 + (uint64_t)0ULL));
    (a0 = (uint64_t)((uint64_t)s2 + (uint64_t)0ULL));
    (s0 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL));
    (s3 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 32))));
    { (s4 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 4294967040)))); A3_CHARGE(); v0 = LoaderSysPrintf(ram, rt, a0, a1, a2, a3, t0, t1, t2, t3, sp, f12, f13, f14, f15, f16, f17, f18, f19, h2); }
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)20 << 16))));
    (s1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v1, 4294962576))));
    (s7 = (uint64_t)((uint64_t)RC_U64X(v1) + (uint64_t)0ULL));
    (s6 = (uint64_t)((uint64_t)RC_U64X(s1) + (uint64_t)0ULL));
    goto L_105904;
L_105730:;
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 9))));
    (v1 = (uint64_t)((int64_t)(int32_t)((int8_t)A1_R8B((uint32_t)a3, 0))));
    { const bool c = (int64_t)v1 != (int64_t)v0; (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 10)))); if (c) goto L_105778; }
    (a2 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)gp, -31744))));
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)gp, -31748))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)SLL32((uint32_t)a2, 4))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)SUB32((uint32_t)v0, (uint32_t)a2))));
    (a1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v1, 2))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)SLL32((uint32_t)v0, 2))));
    A1_W32B((uint32_t)gp, -31748, (uint32_t)a1);
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v0, (uint32_t)v1))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)SLL32((uint32_t)v0, 3))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v0, (uint32_t)s1))));
    A1_W8B((uint32_t)v0, 4, (uint8_t)(uint32_t)s3);
    A1_W32B((uint32_t)v0, 0, (uint32_t)s4);
    goto L_105858;
L_105778:;
    if ((int64_t)v1 != (int64_t)v0) { (a2 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)gp, -31744)))); goto L_1057c0; }
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)gp, -31744))));
    (a1 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL));
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)gp, -31748))));
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)SLL32((uint32_t)v0, 4))));
    (a2 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v0, 1))));
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)SUB32((uint32_t)v1, (uint32_t)v0))));
    A1_W32B((uint32_t)gp, -31744, (uint32_t)a2);
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)SLL32((uint32_t)v1, 2))));
    A1_W32B((uint32_t)gp, -31748, 0u);
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v1, (uint32_t)a0))));
    A1_W32B((uint32_t)gp, -31740, 0u);
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)SLL32((uint32_t)v1, 3))));
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v1, (uint32_t)s1))));
    A1_W8B((uint32_t)v1, 4, (uint8_t)(uint32_t)s3);
    A1_W32B((uint32_t)v1, 0, (uint32_t)s4);
    goto L_105858;
L_1057c0:;
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 39))));
    (a1 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)gp, -31748))));
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)SLL32((uint32_t)a2, 4))));
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)SUB32((uint32_t)v1, (uint32_t)a2))));
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)SLL32((uint32_t)v1, 2))));
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v1, (uint32_t)a1))));
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)SLL32((uint32_t)v1, 3))));
    (t0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v1, (uint32_t)s1))));
    A1_W32B((uint32_t)t0, 0, (uint32_t)s5);
    (v1 = (uint64_t)((int64_t)(int32_t)((int8_t)A1_R8B((uint32_t)a3, 0))));
    { const bool c = (int64_t)v1 != (int64_t)v0; (a0 = (uint64_t)(uint32_t)((uint8_t)A1_R8B((uint32_t)a3, 0))); if (c) goto L_105800; }
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)gp, -31736))));
    (v0 = (uint64_t)(((uint64_t)RC_U64X(v0) < (uint64_t)(int64_t)(int32_t)1) ? 1 : 0));
    A1_W32B((uint32_t)gp, -31736, (uint32_t)v0);
    (a0 = (uint64_t)(uint32_t)((uint8_t)A1_R8B((uint32_t)a3, 0)));
L_105800:;
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)SLL32((uint32_t)a0, 24))));
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 34))));
    (v0 = (uint64_t)((int64_t)(int32_t)(SRA32((int32_t)v0, 24))));
    { const bool c = (int64_t)v0 != (int64_t)v1; (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)gp, -31732)))); if (c) goto L_105820; }
    (v0 = (uint64_t)(((uint64_t)RC_U64X(v0) < (uint64_t)(int64_t)(int32_t)1) ? 1 : 0));
    A1_W32B((uint32_t)gp, -31732, (uint32_t)v0);
    (a0 = (uint64_t)(uint32_t)((uint8_t)A1_R8B((uint32_t)a3, 0)));
L_105820:;
    { const bool c = (int64_t)v0 != 0; (v1 = (uint64_t)((uint64_t)RC_U64X(a0) + (uint64_t)0ULL)); if (c) goto L_10584c; }
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)SLL32((uint32_t)a0, 24))));
    (v0 = (uint64_t)((int64_t)(int32_t)(SRA32((int32_t)v0, 24))));
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)20 << 16))));
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v1, (uint32_t)v0))));
    (v1 = (uint64_t)(uint32_t)((uint8_t)A1_R8B((uint32_t)v1, -27871)));
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v0, 4294967264))));
    (v1 = (uint64_t)(RC_U64X(v1) & (uint64_t)(uint16_t)2));
    if (RC_U64X(v1) != 0) do { const a4::q128 _v = (a4::mk128(h4, a0)); h2 = a4::hi64(_v); std::memcpy(&v0, &_v, 8); } while (0);
    (v1 = (uint64_t)((uint64_t)v0 + (uint64_t)0ULL));
L_10584c:;
    (a1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)a1, 1))));
    A1_W8B((uint32_t)t0, 4, (uint8_t)(uint32_t)v1);
    A1_W32B((uint32_t)gp, -31748, (uint32_t)a1);
L_105858:;
    (v0 = (uint64_t)(((int64_t)(int64_t)RC_U64X(a1) < (int64_t)(int32_t)60) ? 1 : 0));
    { const bool c = (int64_t)v0 != 0; (v0 = (uint64_t)(((int64_t)(int64_t)RC_U64X(a2) < (int64_t)(int32_t)40) ? 1 : 0)); if (c) goto L_10586c; }
    (a1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 59))));
    A1_W32B((uint32_t)gp, -31748, (uint32_t)a1);
L_10586c:;
    if ((int64_t)v0 != 0) { (s0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)s0, 1)))); goto L_105904; }
    (a1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 1))));
    (a2 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)s7, 4294962576))));
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 15))));
L_105880:;
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)SLL32((uint32_t)a0, 5))));
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 59))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v0, (uint32_t)a2))));
L_105890:;
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v1, 4294967295))));
    { uint32_t addr = ADD32((uint32_t)v0, 7); uint32_t aligned_addr = addr & ~7u; uint32_t offset = addr & 7u; uint64_t mem = A1_R64(aligned_addr); uint32_t shift = (7u - offset) << 3; uint64_t keepMask = (shift == 0) ? 0ull : ((1ull << shift) - 1ull); (a3 = (uint64_t)((a3 & keepMask) | (mem << shift))); }
    { uint32_t addr = ADD32((uint32_t)v0, 0); uint32_t aligned_addr = addr & ~7u; uint32_t offset = addr & 7u; uint64_t mem = A1_R64(aligned_addr); uint32_t shift = offset << 3; uint64_t keepMask = (offset == 0) ? 0ull : (0xFFFFFFFFFFFFFFFFull << ((8u - offset) << 3)); (a3 = (uint64_t)((a3 & keepMask) | (mem >> shift))); }
    { uint32_t addr = ADD32((uint32_t)v0, 4294966823); uint32_t aligned_addr = addr & ~7u; uint32_t offset = addr & 7u; uint32_t shift = (7u - offset) << 3; uint64_t mask = 0xFFFFFFFFFFFFFFFFull >> shift; uint64_t old_data = A1_R64(aligned_addr); uint64_t val = a3; uint64_t new_data = (old_data & ~mask) | ((val >> shift) & mask); A1_W64(aligned_addr, new_data); }
    { uint32_t addr = ADD32((uint32_t)v0, 4294966816); uint32_t aligned_addr = addr & ~7u; uint32_t offset = addr & 7u; uint32_t shift = offset << 3; uint64_t mask = 0xFFFFFFFFFFFFFFFFull << shift; uint64_t old_data = A1_R64(aligned_addr); uint64_t val = a3; uint64_t new_data = (old_data & ~mask) | ((val << shift) & mask); A1_W64(aligned_addr, new_data); }
    { const bool c = (int32_t)v1 >= 0; (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v0, 8)))); if (c) { A3_BACK(); goto L_105890; } }
    (a1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)a1, 1))));
    (v0 = (uint64_t)(((int64_t)(int64_t)RC_U64X(a1) < (int64_t)(int32_t)40) ? 1 : 0));
    { const bool c = (int64_t)v0 != 0; (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)a0, 15)))); if (c) { A3_BACK(); goto L_105880; } }
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)s6, 18720))));
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 59))));
L_1058d0:;
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v1, 4294967295))));
    { uint32_t addr = ADD32((uint32_t)gp, 4294935543); uint32_t aligned_addr = addr & ~7u; uint32_t offset = addr & 7u; uint64_t mem = A1_R64(aligned_addr); uint32_t shift = (7u - offset) << 3; uint64_t keepMask = (shift == 0) ? 0ull : ((1ull << shift) - 1ull); (a0 = (uint64_t)((a0 & keepMask) | (mem << shift))); }
    { uint32_t addr = ADD32((uint32_t)gp, 4294935536); uint32_t aligned_addr = addr & ~7u; uint32_t offset = addr & 7u; uint64_t mem = A1_R64(aligned_addr); uint32_t shift = offset << 3; uint64_t keepMask = (offset == 0) ? 0ull : (0xFFFFFFFFFFFFFFFFull << ((8u - offset) << 3)); (a0 = (uint64_t)((a0 & keepMask) | (mem >> shift))); }
    { uint32_t addr = ADD32((uint32_t)v0, 7); uint32_t aligned_addr = addr & ~7u; uint32_t offset = addr & 7u; uint32_t shift = (7u - offset) << 3; uint64_t mask = 0xFFFFFFFFFFFFFFFFull >> shift; uint64_t old_data = A1_R64(aligned_addr); uint64_t val = a0; uint64_t new_data = (old_data & ~mask) | ((val >> shift) & mask); A1_W64(aligned_addr, new_data); }
    { uint32_t addr = ADD32((uint32_t)v0, 0); uint32_t aligned_addr = addr & ~7u; uint32_t offset = addr & 7u; uint32_t shift = offset << 3; uint64_t mask = 0xFFFFFFFFFFFFFFFFull << shift; uint64_t old_data = A1_R64(aligned_addr); uint64_t val = a0; uint64_t new_data = (old_data & ~mask) | ((val << shift) & mask); A1_W64(aligned_addr, new_data); }
    { const bool c = (int32_t)v1 >= 0; (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v0, 8)))); if (c) { A3_BACK(); goto L_1058d0; } }
    (a2 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 39))));
    A1_W32B((uint32_t)gp, -31744, (uint32_t)a2);
    (s0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)s0, 1))));
L_105904:;
    { (a0 = (uint64_t)((uint64_t)s2 + (uint64_t)0ULL)); A3_CHARGE(); v0 = a1_sx64(a6x::strlen(ram, rt, (uint32_t)a0, h2)); }
    (v0 = (uint64_t)(((uint64_t)s0 < (uint64_t)v0) ? 1 : 0));
    { const bool c = (int64_t)v0 != 0; (a3 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)s2, (uint32_t)s0)))); if (c) { A3_BACK(); goto L_105730; } }
    { (sp = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)sp, 80)))); io_h2 = h2; return v0; }
    __builtin_unreachable();
}

// kernel_sub_001AF8E0 (0x1af8e0): typed by tools/typed_lift2.py (round A6; guest frame calls, extra outputs xo[0]=a2; VU0 in -, out h2)
inline uint64_t kernel_sub_001AF8E0(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint64_t v0, uint64_t a0, uint64_t a1, uint64_t a2, uint64_t a3, uint64_t t0, uint64_t t1, uint64_t t2, uint64_t t3, uint64_t sp, float f12, float f13, float f14, float f15, float f16, float f17, float f18, float f19, uint64_t &io_h2, uint64_t *xo = nullptr)
{
    uint64_t s0 = 0;
    uint64_t h2 = io_h2;
    (sp = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)sp, 4294966944))));
    (s0 = (uint64_t)((uint64_t)a0 + (uint64_t)0ULL));
    A1_W64B((uint32_t)sp, 304, a2);
    (a2 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)sp, 304))));
    (a0 = (uint64_t)((uint64_t)RC_U64X(sp) + (uint64_t)0ULL));
    A1_W64B((uint32_t)sp, 312, a3);
    A1_W64B((uint32_t)sp, 320, t0);
    A1_W64B((uint32_t)sp, 328, t1);
    A1_W64B((uint32_t)sp, 336, t2);
    A1_W64B((uint32_t)sp, 344, t3);
    { float f = f12; uint32_t bits; std::memcpy(&bits, &f, sizeof(bits)); A1_W32B((uint32_t)sp, 272, bits); }
    { float f = f13; uint32_t bits; std::memcpy(&bits, &f, sizeof(bits)); A1_W32B((uint32_t)sp, 276, bits); }
    { float f = f14; uint32_t bits; std::memcpy(&bits, &f, sizeof(bits)); A1_W32B((uint32_t)sp, 280, bits); }
    { float f = f15; uint32_t bits; std::memcpy(&bits, &f, sizeof(bits)); A1_W32B((uint32_t)sp, 284, bits); }
    { float f = f16; uint32_t bits; std::memcpy(&bits, &f, sizeof(bits)); A1_W32B((uint32_t)sp, 288, bits); }
    { float f = f17; uint32_t bits; std::memcpy(&bits, &f, sizeof(bits)); A1_W32B((uint32_t)sp, 292, bits); }
    { float f = f18; uint32_t bits; std::memcpy(&bits, &f, sizeof(bits)); A1_W32B((uint32_t)sp, 296, bits); }
    { { float f = f19; uint32_t bits; std::memcpy(&bits, &f, sizeof(bits)); A1_W32B((uint32_t)sp, 300, bits); } A3_CHARGE(); v0 = a1_sx64(a7x::vsprintf(ram, rt, (uint32_t)a0, (uint32_t)a1, (uint32_t)a2, h2)); }
    (a0 = (uint64_t)((uint64_t)sp + (uint64_t)0ULL));
    { const bool c = (int32_t)s0 >= 0; (a1 = (uint64_t)((uint64_t)sp + (uint64_t)0ULL)); if (c) goto L_1af958; }
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)30 << 16))));
    { (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)a0, 7504)))); A3_CHARGE(); uint64_t a7c[3] = {0u, 0u, 0u}; v0 = LoaderSysPrintf(ram, rt, a0, a1, a2, a3, t0, t1, t2, t3, sp, f12, f13, f14, f15, f16, f17, f18, f19, h2, a7c); a2 = a7c[2]; }
    goto L_1af970;
L_1af958:;
    { A3_CHARGE(); v0 = a1_sx64(a6x::strlen(ram, rt, (uint32_t)a0, h2)); }
    (a0 = (uint64_t)((uint64_t)s0 + (uint64_t)0ULL));
    (a1 = (uint64_t)((uint64_t)sp + (uint64_t)0ULL));
    { (a2 = (uint64_t)((uint64_t)v0 + (uint64_t)0ULL)); A3_CHARGE(); v0 = a1_sx64(LoaderSysFWrite(ram, rt, (uint32_t)a0, (uint32_t)a1, (uint32_t)a2, f12, f13, f14, f15, f16, f17, f18, f19, h2)); }
L_1af970:;
    { (a0 = (uint64_t)((uint64_t)sp + (uint64_t)0ULL)); A3_CHARGE(); v0 = a1_sx64(a6x::strlen(ram, rt, (uint32_t)a0, h2)); }
    { (sp = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)sp, 352)))); if (xo) { xo[0] = a2; } io_h2 = h2; return v0; }
    __builtin_unreachable();
}

// iosDlSetLayer (0x1c01b0): typed by tools/typed_lift2.py (round A6; frame calls; VU0 in -, out h2, 64-bit inputs a1 a2 a3 t0 t1 t2 t3, guest sp, frame 16 bytes, 0 slot accesses)
inline uint64_t iosDlSetLayer(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint64_t a0, uint64_t w64_a1, uint64_t w64_a2, uint64_t w64_a3, uint64_t w64_t0, uint64_t w64_t1, uint64_t w64_t2, uint64_t w64_t3, uint32_t sp, float f12, float f13, float f14, float f15, float f16, float f17, float f18, float f19, uint64_t &io_h2)
{
    uint64_t v0 = 0;
    uint32_t v1 = 0;
    uint32_t a1 = (uint32_t)w64_a1;
    uint32_t a2 = (uint32_t)w64_a2;
    uint32_t a3 = (uint32_t)w64_a3;
    uint32_t t0 = (uint32_t)w64_t0;
    uint32_t t1 = (uint32_t)w64_t1;
    uint32_t t2 = (uint32_t)w64_t2;
    uint32_t t3 = (uint32_t)w64_t3;
    uint64_t s0 = 0;
    uint64_t h2 = io_h2;
    (s0 = (uint64_t)((uint64_t)a0 + (uint64_t)0ULL));
    if ((int32_t)s0 >= 0) goto L_1c01d8;
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)30 << 16))));
    { (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)a0, 11264)))); A3_CHARGE(); v0 = LoaderSysPrintf(ram, rt, a0, w64_a1, w64_a2, w64_a3, w64_t0, w64_t1, w64_t2, w64_t3, a1_sx64((uint32_t)(sp + 0xFFFFFFF0u)), f12, f13, f14, f15, f16, f17, f18, f19, h2); }
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)33 << 16))));
    goto L_1c01f8;
L_1c01d8:;
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)33 << 16))));
    (v1 = (uint32_t)((int32_t)((int32_t)ADD32((uint32_t)a0, 13016))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B(v1, 20))));
    (v0 = (uint64_t)(((int64_t)(int64_t)s0 < (int64_t)(int64_t)RC_U64X(v0)) ? 1 : 0));
    { const bool c = (int64_t)v0 != 0; (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)a0, 13016)))); if (c) goto L_1c01fc; }
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B(v1, 20))));
    (s0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v0, 4294967295))));
L_1c01f8:;
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)a0, 13016))));
L_1c01fc:;
    A1_W32B((uint32_t)v0, 28, (uint32_t)s0);
    { io_h2 = h2; return v0; }
    __builtin_unreachable();
}

// iosDumpDMAStatus (0x1c0508): typed by tools/typed_lift2.py (round A6; guest frame calls, extra outputs xo[0]=a1 xo[1]=a2 xo[2]=a3 xo[3]=t0 xo[4]=t1 xo[5]=t2 xo[6]=t3; VU0 in -, out h2)
inline uint64_t iosDumpDMAStatus(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint64_t a0, uint64_t sp, float f12, float f13, float f14, float f15, float f16, float f17, float f18, float f19, uint64_t &io_h2, uint64_t *xo = nullptr)
{
    uint64_t v0 = 0;
    uint64_t v1 = 0;
    uint64_t a1 = 0;
    uint64_t a2 = 0;
    uint64_t a3 = 0;
    uint64_t t0 = 0;
    uint64_t t1 = 0;
    uint64_t t2 = 0;
    uint64_t t3 = 0;
    uint64_t t4 = 0;
    uint64_t t5 = 0;
    uint64_t t6 = 0;
    uint64_t s0 = 0;
    uint64_t s1 = 0;
    uint64_t s2 = 0;
    uint64_t s3 = 0;
    uint64_t s4 = 0;
    uint64_t s5 = 0;
    uint64_t s6 = 0;
    uint64_t s7 = 0;
    uint64_t fp = 0;
    uint64_t h2 = io_h2;
    (sp = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)sp, 4294967104))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)4096 << 16))));
    (s0 = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)30 << 16))));
    (s1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)sp, 16))));
    (s2 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)sp, 32))));
    (v0 = (uint64_t)(RC_U64X(v0) | (uint64_t)(uint16_t)36864));
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)4096 << 16))));
    (s5 = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)32767 << 16))));
    (s6 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)sp, 48))));
    (s7 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)sp, 64))));
    (fp = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)sp, 80))));
    (v1 = (uint64_t)(RC_U64X(v1) | (uint64_t)(uint16_t)40960));
    (a2 = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)4096 << 16))));
    (a3 = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)4096 << 16))));
    (a1 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)v0, 0))));
    (a2 = (uint64_t)(RC_U64X(a2) | (uint64_t)(uint16_t)36880));
    (a3 = (uint64_t)(RC_U64X(a3) | (uint64_t)(uint16_t)36912));
    (t5 = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)4096 << 16))));
    A1_W32B((uint32_t)sp, 16, (uint32_t)a1);
    (a1 = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)4096 << 16))));
    (a1 = (uint64_t)(RC_U64X(a1) | (uint64_t)(uint16_t)40976));
    (t5 = (uint64_t)(RC_U64X(t5) | (uint64_t)(uint16_t)41008));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)v1, 0))));
    (t3 = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)4096 << 16))));
    (t3 = (uint64_t)(RC_U64X(t3) | (uint64_t)(uint16_t)36928));
    (t4 = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)4096 << 16))));
    A1_W32B((uint32_t)sp, 20, (uint32_t)v0);
    (t4 = (uint64_t)(RC_U64X(t4) | (uint64_t)(uint16_t)41024));
    (t2 = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)4096 << 16))));
    (s5 = (uint64_t)(RC_U64X(s5) | (uint64_t)(uint16_t)65535));
    { uint32_t addr = ADD32((uint32_t)s1, 7); uint32_t aligned_addr = addr & ~7u; uint32_t offset = addr & 7u; uint64_t mem = A1_R64(aligned_addr); uint32_t shift = (7u - offset) << 3; uint64_t keepMask = (shift == 0) ? 0ull : ((1ull << shift) - 1ull); (v0 = (uint64_t)((RC_U64X(v0) & keepMask) | (mem << shift))); }
    { uint32_t addr = ADD32((uint32_t)s1, 0); uint32_t aligned_addr = addr & ~7u; uint32_t offset = addr & 7u; uint64_t mem = A1_R64(aligned_addr); uint32_t shift = offset << 3; uint64_t keepMask = (offset == 0) ? 0ull : (0xFFFFFFFFFFFFFFFFull << ((8u - offset) << 3)); (v0 = (uint64_t)((v0 & keepMask) | (mem >> shift))); }
    { uint32_t addr = ADD32((uint32_t)sp, 7); uint32_t aligned_addr = addr & ~7u; uint32_t offset = addr & 7u; uint32_t shift = (7u - offset) << 3; uint64_t mask = 0xFFFFFFFFFFFFFFFFull >> shift; uint64_t old_data = A1_R64(aligned_addr); uint64_t val = v0; uint64_t new_data = (old_data & ~mask) | ((val >> shift) & mask); A1_W64(aligned_addr, new_data); }
    (t2 = (uint64_t)(RC_U64X(t2) | (uint64_t)(uint16_t)36896));
    (s0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)s0, 11760))));
    { uint32_t addr = ADD32((uint32_t)sp, 0); uint32_t aligned_addr = addr & ~7u; uint32_t offset = addr & 7u; uint32_t shift = offset << 3; uint64_t mask = 0xFFFFFFFFFFFFFFFFull << shift; uint64_t old_data = A1_R64(aligned_addr); uint64_t val = v0; uint64_t new_data = (old_data & ~mask) | ((val << shift) & mask); A1_W64(aligned_addr, new_data); }
    (t0 = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)4096 << 16))));
    (v1 = (uint64_t)((uint64_t)a0 + (uint64_t)0ULL));
    (t0 = (uint64_t)(RC_U64X(t0) | (uint64_t)(uint16_t)36944));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)a2, 0))));
    (a2 = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)4096 << 16))));
    (s4 = (uint64_t)((int64_t)(int32_t)((int32_t)SLL32((uint32_t)v1, 2))));
    (t1 = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)4096 << 16))));
    A1_W32B((uint32_t)sp, 32, (uint32_t)v0);
    (t1 = (uint64_t)(RC_U64X(t1) | (uint64_t)(uint16_t)41040));
    (a2 = (uint64_t)(RC_U64X(a2) | (uint64_t)(uint16_t)40992));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)a1, 0))));
    A1_W32B((uint32_t)sp, 36, (uint32_t)v0);
    { uint32_t addr = ADD32((uint32_t)s2, 7); uint32_t aligned_addr = addr & ~7u; uint32_t offset = addr & 7u; uint64_t mem = A1_R64(aligned_addr); uint32_t shift = (7u - offset) << 3; uint64_t keepMask = (shift == 0) ? 0ull : ((1ull << shift) - 1ull); (t6 = (uint64_t)((t6 & keepMask) | (mem << shift))); }
    { uint32_t addr = ADD32((uint32_t)s2, 0); uint32_t aligned_addr = addr & ~7u; uint32_t offset = addr & 7u; uint64_t mem = A1_R64(aligned_addr); uint32_t shift = offset << 3; uint64_t keepMask = (offset == 0) ? 0ull : (0xFFFFFFFFFFFFFFFFull << ((8u - offset) << 3)); (t6 = (uint64_t)((t6 & keepMask) | (mem >> shift))); }
    { uint32_t addr = ADD32((uint32_t)s1, 7); uint32_t aligned_addr = addr & ~7u; uint32_t offset = addr & 7u; uint32_t shift = (7u - offset) << 3; uint64_t mask = 0xFFFFFFFFFFFFFFFFull >> shift; uint64_t old_data = A1_R64(aligned_addr); uint64_t val = t6; uint64_t new_data = (old_data & ~mask) | ((val >> shift) & mask); A1_W64(aligned_addr, new_data); }
    { uint32_t addr = ADD32((uint32_t)s1, 0); uint32_t aligned_addr = addr & ~7u; uint32_t offset = addr & 7u; uint32_t shift = offset << 3; uint64_t mask = 0xFFFFFFFFFFFFFFFFull << shift; uint64_t old_data = A1_R64(aligned_addr); uint64_t val = t6; uint64_t new_data = (old_data & ~mask) | ((val << shift) & mask); A1_W64(aligned_addr, new_data); }
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)30 << 16))));
    (a1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v1, 1))));
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)a0, 11448))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)a3, 0))));
    (s1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)s1, (uint32_t)s4))));
    A1_W32B((uint32_t)sp, 48, (uint32_t)v0);
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)t5, 0))));
    A1_W32B((uint32_t)sp, 52, (uint32_t)v0);
    { uint32_t addr = ADD32((uint32_t)s6, 7); uint32_t aligned_addr = addr & ~7u; uint32_t offset = addr & 7u; uint64_t mem = A1_R64(aligned_addr); uint32_t shift = (7u - offset) << 3; uint64_t keepMask = (shift == 0) ? 0ull : ((1ull << shift) - 1ull); (a3 = (uint64_t)((RC_U64X(a3) & keepMask) | (mem << shift))); }
    { uint32_t addr = ADD32((uint32_t)s6, 0); uint32_t aligned_addr = addr & ~7u; uint32_t offset = addr & 7u; uint64_t mem = A1_R64(aligned_addr); uint32_t shift = offset << 3; uint64_t keepMask = (offset == 0) ? 0ull : (0xFFFFFFFFFFFFFFFFull << ((8u - offset) << 3)); (a3 = (uint64_t)((a3 & keepMask) | (mem >> shift))); }
    { uint32_t addr = ADD32((uint32_t)s2, 7); uint32_t aligned_addr = addr & ~7u; uint32_t offset = addr & 7u; uint32_t shift = (7u - offset) << 3; uint64_t mask = 0xFFFFFFFFFFFFFFFFull >> shift; uint64_t old_data = A1_R64(aligned_addr); uint64_t val = a3; uint64_t new_data = (old_data & ~mask) | ((val >> shift) & mask); A1_W64(aligned_addr, new_data); }
    { uint32_t addr = ADD32((uint32_t)s2, 0); uint32_t aligned_addr = addr & ~7u; uint32_t offset = addr & 7u; uint32_t shift = offset << 3; uint64_t mask = 0xFFFFFFFFFFFFFFFFull << shift; uint64_t old_data = A1_R64(aligned_addr); uint64_t val = a3; uint64_t new_data = (old_data & ~mask) | ((val << shift) & mask); A1_W64(aligned_addr, new_data); }
    (s2 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)s2, (uint32_t)s4))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)t3, 0))));
    A1_W32B((uint32_t)sp, 64, (uint32_t)v0);
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)t4, 0))));
    A1_W32B((uint32_t)sp, 68, (uint32_t)v1);
    { uint32_t addr = ADD32((uint32_t)s7, 7); uint32_t aligned_addr = addr & ~7u; uint32_t offset = addr & 7u; uint64_t mem = A1_R64(aligned_addr); uint32_t shift = (7u - offset) << 3; uint64_t keepMask = (shift == 0) ? 0ull : ((1ull << shift) - 1ull); (v1 = (uint64_t)((RC_U64X(v1) & keepMask) | (mem << shift))); }
    { uint32_t addr = ADD32((uint32_t)s7, 0); uint32_t aligned_addr = addr & ~7u; uint32_t offset = addr & 7u; uint64_t mem = A1_R64(aligned_addr); uint32_t shift = offset << 3; uint64_t keepMask = (offset == 0) ? 0ull : (0xFFFFFFFFFFFFFFFFull << ((8u - offset) << 3)); (v1 = (uint64_t)((v1 & keepMask) | (mem >> shift))); }
    { uint32_t addr = ADD32((uint32_t)s6, 7); uint32_t aligned_addr = addr & ~7u; uint32_t offset = addr & 7u; uint32_t shift = (7u - offset) << 3; uint64_t mask = 0xFFFFFFFFFFFFFFFFull >> shift; uint64_t old_data = A1_R64(aligned_addr); uint64_t val = v1; uint64_t new_data = (old_data & ~mask) | ((val >> shift) & mask); A1_W64(aligned_addr, new_data); }
    { uint32_t addr = ADD32((uint32_t)s6, 0); uint32_t aligned_addr = addr & ~7u; uint32_t offset = addr & 7u; uint32_t shift = offset << 3; uint64_t mask = 0xFFFFFFFFFFFFFFFFull << shift; uint64_t old_data = A1_R64(aligned_addr); uint64_t val = v1; uint64_t new_data = (old_data & ~mask) | ((val << shift) & mask); A1_W64(aligned_addr, new_data); }
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)t0, 0))));
    A1_W32B((uint32_t)sp, 80, (uint32_t)v0);
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)t1, 0))));
    A1_W32B((uint32_t)sp, 84, (uint32_t)v1);
    { uint32_t addr = ADD32((uint32_t)fp, 7); uint32_t aligned_addr = addr & ~7u; uint32_t offset = addr & 7u; uint64_t mem = A1_R64(aligned_addr); uint32_t shift = (7u - offset) << 3; uint64_t keepMask = (shift == 0) ? 0ull : ((1ull << shift) - 1ull); (t6 = (uint64_t)((t6 & keepMask) | (mem << shift))); }
    { uint32_t addr = ADD32((uint32_t)fp, 0); uint32_t aligned_addr = addr & ~7u; uint32_t offset = addr & 7u; uint64_t mem = A1_R64(aligned_addr); uint32_t shift = offset << 3; uint64_t keepMask = (offset == 0) ? 0ull : (0xFFFFFFFFFFFFFFFFull << ((8u - offset) << 3)); (t6 = (uint64_t)((t6 & keepMask) | (mem >> shift))); }
    { uint32_t addr = ADD32((uint32_t)s7, 7); uint32_t aligned_addr = addr & ~7u; uint32_t offset = addr & 7u; uint32_t shift = (7u - offset) << 3; uint64_t mask = 0xFFFFFFFFFFFFFFFFull >> shift; uint64_t old_data = A1_R64(aligned_addr); uint64_t val = t6; uint64_t new_data = (old_data & ~mask) | ((val >> shift) & mask); A1_W64(aligned_addr, new_data); }
    { uint32_t addr = ADD32((uint32_t)s7, 0); uint32_t aligned_addr = addr & ~7u; uint32_t offset = addr & 7u; uint32_t shift = offset << 3; uint64_t mask = 0xFFFFFFFFFFFFFFFFull << shift; uint64_t old_data = A1_R64(aligned_addr); uint64_t val = t6; uint64_t new_data = (old_data & ~mask) | ((val << shift) & mask); A1_W64(aligned_addr, new_data); }
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)t2, 0))));
    A1_W32B((uint32_t)sp, 96, (uint32_t)v0);
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)a2, 0))));
    A1_W32B((uint32_t)sp, 100, (uint32_t)v1);
    { uint32_t addr = ADD32((uint32_t)sp, 103); uint32_t aligned_addr = addr & ~7u; uint32_t offset = addr & 7u; uint64_t mem = A1_R64(aligned_addr); uint32_t shift = (7u - offset) << 3; uint64_t keepMask = (shift == 0) ? 0ull : ((1ull << shift) - 1ull); (t0 = (uint64_t)((RC_U64X(t0) & keepMask) | (mem << shift))); }
    { uint32_t addr = ADD32((uint32_t)sp, 96); uint32_t aligned_addr = addr & ~7u; uint32_t offset = addr & 7u; uint64_t mem = A1_R64(aligned_addr); uint32_t shift = offset << 3; uint64_t keepMask = (offset == 0) ? 0ull : (0xFFFFFFFFFFFFFFFFull << ((8u - offset) << 3)); (t0 = (uint64_t)((t0 & keepMask) | (mem >> shift))); }
    { uint32_t addr = ADD32((uint32_t)fp, 7); uint32_t aligned_addr = addr & ~7u; uint32_t offset = addr & 7u; uint32_t shift = (7u - offset) << 3; uint64_t mask = 0xFFFFFFFFFFFFFFFFull >> shift; uint64_t old_data = A1_R64(aligned_addr); uint64_t val = t0; uint64_t new_data = (old_data & ~mask) | ((val >> shift) & mask); A1_W64(aligned_addr, new_data); }
    { uint32_t addr = ADD32((uint32_t)fp, 0); uint32_t aligned_addr = addr & ~7u; uint32_t offset = addr & 7u; uint32_t shift = offset << 3; uint64_t mask = 0xFFFFFFFFFFFFFFFFull << shift; uint64_t old_data = A1_R64(aligned_addr); uint64_t val = t0; uint64_t new_data = (old_data & ~mask) | ((val << shift) & mask); A1_W64(aligned_addr, new_data); }
    { A3_CHARGE(); uint64_t a7c[3] = {0u, 0u, 0u}; v0 = LoaderSysPrintf(ram, rt, a0, a1, a2, a3, t0, t1, t2, t3, sp, f12, f13, f14, f15, f16, f17, f18, f19, h2, a7c); a1 = a7c[1]; a2 = a7c[2]; }
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)30 << 16))));
    { (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)a0, 11512)))); A3_CHARGE(); v0 = LoaderSysPrintf(ram, rt, a0, a1, a2, a3, t0, t1, t2, t3, sp, f12, f13, f14, f15, f16, f17, f18, f19, h2); }
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)30 << 16))));
    (s3 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)sp, (uint32_t)s4))));
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)a0, 11552))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)s3, 0))));
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)s3, 0))));
    (v0 = (uint64_t)(RC_U64X(v0) & (uint64_t)(uint16_t)1));
    (a3 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)s3, 0))));
    (v1 = (uint64_t)(RC_U64X(v1) & (uint64_t)(uint16_t)12));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)SLL32((uint32_t)v0, 2))));
    (a2 = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)30 << 16))));
    (a2 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)a2, (uint32_t)v1))));
    (a2 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)a2, 4400))));
    (a3 = (uint64_t)((int64_t)(int32_t)((int32_t)SRL32((uint32_t)a3, 4))));
    (a1 = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)30 << 16))));
    (a1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)a1, (uint32_t)v0))));
    (a1 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)a1, 4392))));
    { (a3 = (uint64_t)(RC_U64X(a3) & (uint64_t)(uint16_t)3)); A3_CHARGE(); v0 = LoaderSysPrintf(ram, rt, a0, a1, a2, a3, t0, t1, t2, t3, sp, f12, f13, f14, f15, f16, f17, f18, f19, h2); }
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)30 << 16))));
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)s3, 0))));
    (a1 = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)30 << 16))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)s3, 0))));
    (a1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)a1, 4416))));
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)SRL32((uint32_t)v1, 4))));
    (a3 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)s3, 0))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)SRL32((uint32_t)v0, 5))));
    (v1 = (uint64_t)(RC_U64X(v1) & (uint64_t)(uint16_t)4));
    (v0 = (uint64_t)(RC_U64X(v0) & (uint64_t)(uint16_t)4));
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v1, (uint32_t)a1))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v0, (uint32_t)a1))));
    (a1 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)v1, 0))));
    (t0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)s3, 0))));
    (a3 = (uint64_t)((int64_t)(int32_t)((int32_t)SRL32((uint32_t)a3, 8))));
    (a2 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)v0, 0))));
    (a3 = (uint64_t)(RC_U64X(a3) & (uint64_t)(uint16_t)1));
    (t0 = (uint64_t)((int64_t)(int32_t)((int32_t)SRL32((uint32_t)t0, 16))));
    { (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)a0, 11624)))); A3_CHARGE(); uint64_t a7c[3] = {0u, 0u, 0u}; v0 = LoaderSysPrintf(ram, rt, a0, a1, a2, a3, t0, t1, t2, t3, sp, f12, f13, f14, f15, f16, f17, f18, f19, h2, a7c); a1 = a7c[1]; a2 = a7c[2]; }
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)30 << 16))));
    { (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)a0, 11720)))); A3_CHARGE(); uint64_t a7c[3] = {0u, 0u, 0u}; v0 = LoaderSysPrintf(ram, rt, a0, a1, a2, a3, t0, t1, t2, t3, sp, f12, f13, f14, f15, f16, f17, f18, f19, h2, a7c); a2 = a7c[2]; }
    (a1 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)s1, 0))));
    (a0 = (uint64_t)((uint64_t)s0 + (uint64_t)0ULL));
    { (a1 = (uint64_t)(RC_U64X(a1) & s5)); A3_CHARGE(); uint64_t a7c[3] = {0u, 0u, 0u}; v0 = LoaderSysPrintf(ram, rt, a0, a1, a2, a3, t0, t1, t2, t3, sp, f12, f13, f14, f15, f16, f17, f18, f19, h2, a7c); a1 = a7c[1]; a2 = a7c[2]; }
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)30 << 16))));
    { (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)a0, 11792)))); A3_CHARGE(); uint64_t a7c[3] = {0u, 0u, 0u}; v0 = LoaderSysPrintf(ram, rt, a0, a1, a2, a3, t0, t1, t2, t3, sp, f12, f13, f14, f15, f16, f17, f18, f19, h2, a7c); a2 = a7c[2]; }
    (a1 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)s2, 0))));
    (a0 = (uint64_t)((uint64_t)s0 + (uint64_t)0ULL));
    { (a1 = (uint64_t)(RC_U64X(a1) & s5)); A3_CHARGE(); uint64_t a7c[3] = {0u, 0u, 0u}; v0 = LoaderSysPrintf(ram, rt, a0, a1, a2, a3, t0, t1, t2, t3, sp, f12, f13, f14, f15, f16, f17, f18, f19, h2, a7c); a1 = a7c[1]; a2 = a7c[2]; }
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)s3, 0))));
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)SRL32((uint32_t)v1, 4))));
    (v1 = (uint64_t)(RC_U64X(v1) & (uint64_t)(uint16_t)3));
    if ((int64_t)v1 == 0) { (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)30 << 16)))); goto L_1c07f0; }
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)30 << 16))));
    { (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)a0, 11832)))); A3_CHARGE(); uint64_t a7c[3] = {0u, 0u, 0u}; v0 = LoaderSysPrintf(ram, rt, a0, a1, a2, a3, t0, t1, t2, t3, sp, f12, f13, f14, f15, f16, f17, f18, f19, h2, a7c); a2 = a7c[2]; }
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)s3, 0))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 2))));
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)SRL32((uint32_t)v1, 4))));
    (v1 = (uint64_t)(RC_U64X(v1) & (uint64_t)(uint16_t)3));
    { const bool c = (int64_t)v1 != (int64_t)v0; (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)s6, (uint32_t)s4)))); if (c) goto L_1c07d8; }
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)s7, (uint32_t)s4))));
    (a1 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)v0, 0))));
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)30 << 16))));
    (a2 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)v1, 0))));
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)a0, 11888))));
    (a1 = (uint64_t)(RC_U64X(a1) & s5));
    { (a2 = (uint64_t)(RC_U64X(a2) & s5)); A3_CHARGE(); uint64_t a7c[3] = {0u, 0u, 0u}; v0 = LoaderSysPrintf(ram, rt, a0, a1, a2, a3, t0, t1, t2, t3, sp, f12, f13, f14, f15, f16, f17, f18, f19, h2, a7c); a1 = a7c[1]; a2 = a7c[2]; }
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)30 << 16))));
    goto L_1c07f0;
L_1c07d8:;
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)30 << 16))));
    (a1 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)v0, 0))));
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)a0, 11944))));
    { (a1 = (uint64_t)(RC_U64X(a1) & s5)); A3_CHARGE(); uint64_t a7c[3] = {0u, 0u, 0u}; v0 = LoaderSysPrintf(ram, rt, a0, a1, a2, a3, t0, t1, t2, t3, sp, f12, f13, f14, f15, f16, f17, f18, f19, h2, a7c); a1 = a7c[1]; a2 = a7c[2]; }
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)30 << 16))));
L_1c07f0:;
    { (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)a0, 11976)))); A3_CHARGE(); v0 = LoaderSysPrintf(ram, rt, a0, a1, a2, a3, t0, t1, t2, t3, sp, f12, f13, f14, f15, f16, f17, f18, f19, h2); }
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)30 << 16))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)fp, (uint32_t)s4))));
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)a0, 12016))));
    (a1 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)v0, 0))));
    (a2 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)v0, 0))));
    { (a2 = (uint64_t)((int64_t)(int32_t)((int32_t)SLL32((uint32_t)a2, 4)))); A3_CHARGE(); uint64_t a7c[3] = {0u, 0u, 0u}; v0 = LoaderSysPrintf(ram, rt, a0, a1, a2, a3, t0, t1, t2, t3, sp, f12, f13, f14, f15, f16, f17, f18, f19, h2, a7c); a1 = a7c[1]; a2 = a7c[2]; }
    { (sp = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)sp, 192)))); if (xo) { xo[0] = a1; xo[1] = a2; xo[2] = a3; xo[3] = t0; xo[4] = t1; xo[5] = t2; xo[6] = t3; } io_h2 = h2; return v0; }
    __builtin_unreachable();
}

// iosDumpVIF1Status (0x1c0848): typed by tools/typed_lift2.py (round A6; guest frame calls, extra outputs xo[0]=a1 xo[1]=a2 xo[2]=a3 xo[3]=t0 xo[4]=t1 xo[5]=t3; VU0 in -, out h2)
inline uint64_t iosDumpVIF1Status(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint64_t a1, uint64_t a2, uint64_t a3, uint64_t t0, uint64_t t1, uint64_t t2, uint64_t t3, uint64_t sp, float f12, float f13, float f14, float f15, float f16, float f17, float f18, float f19, uint64_t &io_h2, uint64_t *xo = nullptr)
{
    uint64_t v0 = 0;
    uint64_t v1 = 0;
    uint64_t a0 = 0;
    uint64_t t4 = 0;
    uint64_t s0 = 0;
    uint64_t s1 = 0;
    uint64_t s2 = 0;
    uint64_t s3 = 0;
    uint64_t h2 = io_h2;
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)30 << 16))));
    (sp = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)sp, 4294967168))));
    { (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)a0, 12048)))); A3_CHARGE(); v0 = LoaderSysPrintf(ram, rt, a0, a1, a2, a3, t0, t1, t2, t3, sp, f12, f13, f14, f15, f16, f17, f18, f19, h2); }
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)4096 << 16))));
    (a2 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)sp, 16))));
    (v1 = (uint64_t)(RC_U64X(v1) | (uint64_t)(uint16_t)15360));
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)30 << 16))));
    (s0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)v1, 0))));
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)30 << 16))));
    (t4 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v1, 12120))));
    { uint32_t addr = ADD32((uint32_t)t4, 7); uint32_t aligned_addr = addr & ~7u; uint32_t offset = addr & 7u; uint64_t mem = A1_R64(aligned_addr); uint32_t shift = (7u - offset) << 3; uint64_t keepMask = (shift == 0) ? 0ull : ((1ull << shift) - 1ull); (t1 = (uint64_t)((t1 & keepMask) | (mem << shift))); }
    { uint32_t addr = ADD32((uint32_t)t4, 0); uint32_t aligned_addr = addr & ~7u; uint32_t offset = addr & 7u; uint64_t mem = A1_R64(aligned_addr); uint32_t shift = offset << 3; uint64_t keepMask = (offset == 0) ? 0ull : (0xFFFFFFFFFFFFFFFFull << ((8u - offset) << 3)); (t1 = (uint64_t)((t1 & keepMask) | (mem >> shift))); }
    { uint32_t addr = ADD32((uint32_t)sp, 7); uint32_t aligned_addr = addr & ~7u; uint32_t offset = addr & 7u; uint32_t shift = (7u - offset) << 3; uint64_t mask = 0xFFFFFFFFFFFFFFFFull >> shift; uint64_t old_data = A1_R64(aligned_addr); uint64_t val = t1; uint64_t new_data = (old_data & ~mask) | ((val >> shift) & mask); A1_W64(aligned_addr, new_data); }
    { uint32_t addr = ADD32((uint32_t)sp, 0); uint32_t aligned_addr = addr & ~7u; uint32_t offset = addr & 7u; uint32_t shift = offset << 3; uint64_t mask = 0xFFFFFFFFFFFFFFFFull << shift; uint64_t old_data = A1_R64(aligned_addr); uint64_t val = t1; uint64_t new_data = (old_data & ~mask) | ((val << shift) & mask); A1_W64(aligned_addr, new_data); }
    (s1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)sp, 32))));
    (t3 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)a0, 12192))));
    { uint32_t addr = ADD32((uint32_t)t3, 7); uint32_t aligned_addr = addr & ~7u; uint32_t offset = addr & 7u; uint64_t mem = A1_R64(aligned_addr); uint32_t shift = (7u - offset) << 3; uint64_t keepMask = (shift == 0) ? 0ull : ((1ull << shift) - 1ull); (v1 = (uint64_t)((RC_U64X(v1) & keepMask) | (mem << shift))); }
    { uint32_t addr = ADD32((uint32_t)t3, 0); uint32_t aligned_addr = addr & ~7u; uint32_t offset = addr & 7u; uint64_t mem = A1_R64(aligned_addr); uint32_t shift = offset << 3; uint64_t keepMask = (offset == 0) ? 0ull : (0xFFFFFFFFFFFFFFFFull << ((8u - offset) << 3)); (v1 = (uint64_t)((v1 & keepMask) | (mem >> shift))); }
    { uint32_t addr = ADD32((uint32_t)t3, 15); uint32_t aligned_addr = addr & ~7u; uint32_t offset = addr & 7u; uint64_t mem = A1_R64(aligned_addr); uint32_t shift = (7u - offset) << 3; uint64_t keepMask = (shift == 0) ? 0ull : ((1ull << shift) - 1ull); (t1 = (uint64_t)((t1 & keepMask) | (mem << shift))); }
    { uint32_t addr = ADD32((uint32_t)t3, 8); uint32_t aligned_addr = addr & ~7u; uint32_t offset = addr & 7u; uint64_t mem = A1_R64(aligned_addr); uint32_t shift = offset << 3; uint64_t keepMask = (offset == 0) ? 0ull : (0xFFFFFFFFFFFFFFFFull << ((8u - offset) << 3)); (t1 = (uint64_t)((t1 & keepMask) | (mem >> shift))); }
    { uint32_t addr = ADD32((uint32_t)a2, 7); uint32_t aligned_addr = addr & ~7u; uint32_t offset = addr & 7u; uint32_t shift = (7u - offset) << 3; uint64_t mask = 0xFFFFFFFFFFFFFFFFull >> shift; uint64_t old_data = A1_R64(aligned_addr); uint64_t val = v1; uint64_t new_data = (old_data & ~mask) | ((val >> shift) & mask); A1_W64(aligned_addr, new_data); }
    { uint32_t addr = ADD32((uint32_t)a2, 0); uint32_t aligned_addr = addr & ~7u; uint32_t offset = addr & 7u; uint32_t shift = offset << 3; uint64_t mask = 0xFFFFFFFFFFFFFFFFull << shift; uint64_t old_data = A1_R64(aligned_addr); uint64_t val = v1; uint64_t new_data = (old_data & ~mask) | ((val << shift) & mask); A1_W64(aligned_addr, new_data); }
    { uint32_t addr = ADD32((uint32_t)a2, 15); uint32_t aligned_addr = addr & ~7u; uint32_t offset = addr & 7u; uint32_t shift = (7u - offset) << 3; uint64_t mask = 0xFFFFFFFFFFFFFFFFull >> shift; uint64_t old_data = A1_R64(aligned_addr); uint64_t val = t1; uint64_t new_data = (old_data & ~mask) | ((val >> shift) & mask); A1_W64(aligned_addr, new_data); }
    { uint32_t addr = ADD32((uint32_t)a2, 8); uint32_t aligned_addr = addr & ~7u; uint32_t offset = addr & 7u; uint32_t shift = offset << 3; uint64_t mask = 0xFFFFFFFFFFFFFFFFull << shift; uint64_t old_data = A1_R64(aligned_addr); uint64_t val = t1; uint64_t new_data = (old_data & ~mask) | ((val << shift) & mask); A1_W64(aligned_addr, new_data); }
    (v0 = (uint64_t)(RC_U64X(s0) & (uint64_t)(uint16_t)3));
    (a1 = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)30 << 16))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)SLL32((uint32_t)v0, 2))));
    (a2 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)a2, (uint32_t)v0))));
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)a1, 12240))));
    { uint32_t addr = ADD32((uint32_t)a0, 7); uint32_t aligned_addr = addr & ~7u; uint32_t offset = addr & 7u; uint64_t mem = A1_R64(aligned_addr); uint32_t shift = (7u - offset) << 3; uint64_t keepMask = (shift == 0) ? 0ull : ((1ull << shift) - 1ull); (t4 = (uint64_t)((RC_U64X(t4) & keepMask) | (mem << shift))); }
    { uint32_t addr = ADD32((uint32_t)a0, 0); uint32_t aligned_addr = addr & ~7u; uint32_t offset = addr & 7u; uint64_t mem = A1_R64(aligned_addr); uint32_t shift = offset << 3; uint64_t keepMask = (offset == 0) ? 0ull : (0xFFFFFFFFFFFFFFFFull << ((8u - offset) << 3)); (t4 = (uint64_t)((t4 & keepMask) | (mem >> shift))); }
    { uint32_t addr = ADD32((uint32_t)s1, 7); uint32_t aligned_addr = addr & ~7u; uint32_t offset = addr & 7u; uint32_t shift = (7u - offset) << 3; uint64_t mask = 0xFFFFFFFFFFFFFFFFull >> shift; uint64_t old_data = A1_R64(aligned_addr); uint64_t val = t4; uint64_t new_data = (old_data & ~mask) | ((val >> shift) & mask); A1_W64(aligned_addr, new_data); }
    { uint32_t addr = ADD32((uint32_t)s1, 0); uint32_t aligned_addr = addr & ~7u; uint32_t offset = addr & 7u; uint32_t shift = offset << 3; uint64_t mask = 0xFFFFFFFFFFFFFFFFull << shift; uint64_t old_data = A1_R64(aligned_addr); uint64_t val = t4; uint64_t new_data = (old_data & ~mask) | ((val << shift) & mask); A1_W64(aligned_addr, new_data); }
    (s2 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)sp, 48))));
    (a3 = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)30 << 16))));
    (t3 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)a3, 12280))));
    { uint32_t addr = ADD32((uint32_t)t3, 7); uint32_t aligned_addr = addr & ~7u; uint32_t offset = addr & 7u; uint64_t mem = A1_R64(aligned_addr); uint32_t shift = (7u - offset) << 3; uint64_t keepMask = (shift == 0) ? 0ull : ((1ull << shift) - 1ull); (a1 = (uint64_t)((RC_U64X(a1) & keepMask) | (mem << shift))); }
    { uint32_t addr = ADD32((uint32_t)t3, 0); uint32_t aligned_addr = addr & ~7u; uint32_t offset = addr & 7u; uint64_t mem = A1_R64(aligned_addr); uint32_t shift = offset << 3; uint64_t keepMask = (offset == 0) ? 0ull : (0xFFFFFFFFFFFFFFFFull << ((8u - offset) << 3)); (a1 = (uint64_t)((a1 & keepMask) | (mem >> shift))); }
    { uint32_t addr = ADD32((uint32_t)s2, 7); uint32_t aligned_addr = addr & ~7u; uint32_t offset = addr & 7u; uint32_t shift = (7u - offset) << 3; uint64_t mask = 0xFFFFFFFFFFFFFFFFull >> shift; uint64_t old_data = A1_R64(aligned_addr); uint64_t val = a1; uint64_t new_data = (old_data & ~mask) | ((val >> shift) & mask); A1_W64(aligned_addr, new_data); }
    { uint32_t addr = ADD32((uint32_t)s2, 0); uint32_t aligned_addr = addr & ~7u; uint32_t offset = addr & 7u; uint32_t shift = offset << 3; uint64_t mask = 0xFFFFFFFFFFFFFFFFull << shift; uint64_t old_data = A1_R64(aligned_addr); uint64_t val = a1; uint64_t new_data = (old_data & ~mask) | ((val << shift) & mask); A1_W64(aligned_addr, new_data); }
    (s3 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)sp, 64))));
    (t0 = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)30 << 16))));
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)30 << 16))));
    (a1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)t0, 12320))));
    { uint32_t addr = ADD32((uint32_t)a1, 7); uint32_t aligned_addr = addr & ~7u; uint32_t offset = addr & 7u; uint64_t mem = A1_R64(aligned_addr); uint32_t shift = (7u - offset) << 3; uint64_t keepMask = (shift == 0) ? 0ull : ((1ull << shift) - 1ull); (t4 = (uint64_t)((t4 & keepMask) | (mem << shift))); }
    { uint32_t addr = ADD32((uint32_t)a1, 0); uint32_t aligned_addr = addr & ~7u; uint32_t offset = addr & 7u; uint64_t mem = A1_R64(aligned_addr); uint32_t shift = offset << 3; uint64_t keepMask = (offset == 0) ? 0ull : (0xFFFFFFFFFFFFFFFFull << ((8u - offset) << 3)); (t4 = (uint64_t)((t4 & keepMask) | (mem >> shift))); }
    { uint32_t addr = ADD32((uint32_t)s3, 7); uint32_t aligned_addr = addr & ~7u; uint32_t offset = addr & 7u; uint32_t shift = (7u - offset) << 3; uint64_t mask = 0xFFFFFFFFFFFFFFFFull >> shift; uint64_t old_data = A1_R64(aligned_addr); uint64_t val = t4; uint64_t new_data = (old_data & ~mask) | ((val >> shift) & mask); A1_W64(aligned_addr, new_data); }
    { uint32_t addr = ADD32((uint32_t)s3, 0); uint32_t aligned_addr = addr & ~7u; uint32_t offset = addr & 7u; uint32_t shift = offset << 3; uint64_t mask = 0xFFFFFFFFFFFFFFFFull << shift; uint64_t old_data = A1_R64(aligned_addr); uint64_t val = t4; uint64_t new_data = (old_data & ~mask) | ((val << shift) & mask); A1_W64(aligned_addr, new_data); }
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)a0, 12328))));
    { (a1 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)a2, 0)))); A3_CHARGE(); uint64_t a7c[3] = {0u, 0u, 0u}; v0 = LoaderSysPrintf(ram, rt, a0, a1, a2, a3, t0, t1, t2, t3, sp, f12, f13, f14, f15, f16, f17, f18, f19, h2, a7c); a2 = a7c[2]; }
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)30 << 16))));
    (v0 = (uint64_t)(s0 & (uint64_t)(uint16_t)4));
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)a0, 12352))));
    (s1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)s1, (uint32_t)v0))));
    { (a1 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)s1, 0)))); A3_CHARGE(); uint64_t a7c[3] = {0u, 0u, 0u}; v0 = LoaderSysPrintf(ram, rt, a0, a1, a2, a3, t0, t1, t2, t3, sp, f12, f13, f14, f15, f16, f17, f18, f19, h2, a7c); a2 = a7c[2]; }
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)30 << 16))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)SRL32((uint32_t)s0, 1))));
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)a0, 12376))));
    (v0 = (uint64_t)(RC_U64X(v0) & (uint64_t)(uint16_t)4));
    (s2 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)s2, (uint32_t)v0))));
    { (a1 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)s2, 0)))); A3_CHARGE(); uint64_t a7c[3] = {0u, 0u, 0u}; v0 = LoaderSysPrintf(ram, rt, a0, a1, a2, a3, t0, t1, t2, t3, sp, f12, f13, f14, f15, f16, f17, f18, f19, h2, a7c); a2 = a7c[2]; }
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)30 << 16))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)SRL32((uint32_t)s0, 4))));
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)a0, 12400))));
    (v0 = (uint64_t)(RC_U64X(v0) & (uint64_t)(uint16_t)4));
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)sp, (uint32_t)v0))));
    { (a1 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)v1, 0)))); A3_CHARGE(); uint64_t a7c[3] = {0u, 0u, 0u}; v0 = LoaderSysPrintf(ram, rt, a0, a1, a2, a3, t0, t1, t2, t3, sp, f12, f13, f14, f15, f16, f17, f18, f19, h2, a7c); a2 = a7c[2]; }
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)30 << 16))));
    (a1 = (uint64_t)((int64_t)(int32_t)((int32_t)SRL32((uint32_t)s0, 7))));
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)a0, 12424))));
    { (a1 = (uint64_t)(RC_U64X(a1) & (uint64_t)(uint16_t)1)); A3_CHARGE(); uint64_t a7c[3] = {0u, 0u, 0u}; v0 = LoaderSysPrintf(ram, rt, a0, a1, a2, a3, t0, t1, t2, t3, sp, f12, f13, f14, f15, f16, f17, f18, f19, h2, a7c); a2 = a7c[2]; }
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)30 << 16))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)SRL32((uint32_t)s0, 6))));
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)a0, 12448))));
    (v0 = (uint64_t)(RC_U64X(v0) & (uint64_t)(uint16_t)4));
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)sp, (uint32_t)v0))));
    { (a1 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)v1, 0)))); A3_CHARGE(); uint64_t a7c[3] = {0u, 0u, 0u}; v0 = LoaderSysPrintf(ram, rt, a0, a1, a2, a3, t0, t1, t2, t3, sp, f12, f13, f14, f15, f16, f17, f18, f19, h2, a7c); a2 = a7c[2]; }
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)30 << 16))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)SRL32((uint32_t)s0, 7))));
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)a0, 12472))));
    (v0 = (uint64_t)(RC_U64X(v0) & (uint64_t)(uint16_t)4));
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)sp, (uint32_t)v0))));
    { (a1 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)v1, 0)))); A3_CHARGE(); uint64_t a7c[3] = {0u, 0u, 0u}; v0 = LoaderSysPrintf(ram, rt, a0, a1, a2, a3, t0, t1, t2, t3, sp, f12, f13, f14, f15, f16, f17, f18, f19, h2, a7c); a2 = a7c[2]; }
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)30 << 16))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)SRL32((uint32_t)s0, 8))));
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)a0, 12496))));
    (v0 = (uint64_t)(RC_U64X(v0) & (uint64_t)(uint16_t)4));
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)sp, (uint32_t)v0))));
    { (a1 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)v1, 0)))); A3_CHARGE(); uint64_t a7c[3] = {0u, 0u, 0u}; v0 = LoaderSysPrintf(ram, rt, a0, a1, a2, a3, t0, t1, t2, t3, sp, f12, f13, f14, f15, f16, f17, f18, f19, h2, a7c); a2 = a7c[2]; }
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)30 << 16))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)SRL32((uint32_t)s0, 9))));
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)a0, 12520))));
    (v0 = (uint64_t)(RC_U64X(v0) & (uint64_t)(uint16_t)4));
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)sp, (uint32_t)v0))));
    { (a1 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)v1, 0)))); A3_CHARGE(); uint64_t a7c[3] = {0u, 0u, 0u}; v0 = LoaderSysPrintf(ram, rt, a0, a1, a2, a3, t0, t1, t2, t3, sp, f12, f13, f14, f15, f16, f17, f18, f19, h2, a7c); a2 = a7c[2]; }
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)30 << 16))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)SRL32((uint32_t)s0, 10))));
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)a0, 12544))));
    (v0 = (uint64_t)(RC_U64X(v0) & (uint64_t)(uint16_t)4));
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)sp, (uint32_t)v0))));
    { (a1 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)v1, 0)))); A3_CHARGE(); uint64_t a7c[3] = {0u, 0u, 0u}; v0 = LoaderSysPrintf(ram, rt, a0, a1, a2, a3, t0, t1, t2, t3, sp, f12, f13, f14, f15, f16, f17, f18, f19, h2, a7c); a2 = a7c[2]; }
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)30 << 16))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)SRL32((uint32_t)s0, 11))));
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)a0, 12568))));
    (v0 = (uint64_t)(RC_U64X(v0) & (uint64_t)(uint16_t)4));
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)sp, (uint32_t)v0))));
    { (a1 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)v1, 0)))); A3_CHARGE(); uint64_t a7c[3] = {0u, 0u, 0u}; v0 = LoaderSysPrintf(ram, rt, a0, a1, a2, a3, t0, t1, t2, t3, sp, f12, f13, f14, f15, f16, f17, f18, f19, h2, a7c); a2 = a7c[2]; }
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)30 << 16))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)SRL32((uint32_t)s0, 21))));
    (s0 = (uint64_t)((int64_t)(int32_t)((int32_t)SRL32((uint32_t)s0, 24))));
    (v0 = (uint64_t)(RC_U64X(v0) & (uint64_t)(uint16_t)4));
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)a0, 12592))));
    (s3 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)s3, (uint32_t)v0))));
    (s0 = (uint64_t)(RC_U64X(s0) & (uint64_t)(uint16_t)31));
    { (a1 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)s3, 0)))); A3_CHARGE(); v0 = LoaderSysPrintf(ram, rt, a0, a1, a2, a3, t0, t1, t2, t3, sp, f12, f13, f14, f15, f16, f17, f18, f19, h2); }
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)30 << 16))));
    (a1 = (uint64_t)((uint64_t)s0 + (uint64_t)0ULL));
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)a0, 12616))));
    { (a2 = (uint64_t)((uint64_t)a1 + (uint64_t)0ULL)); A3_CHARGE(); uint64_t a7c[3] = {0u, 0u, 0u}; v0 = LoaderSysPrintf(ram, rt, a0, a1, a2, a3, t0, t1, t2, t3, sp, f12, f13, f14, f15, f16, f17, f18, f19, h2, a7c); a1 = a7c[1]; a2 = a7c[2]; }
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)30 << 16))));
    { (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)a0, 12648)))); A3_CHARGE(); uint64_t a7c[3] = {0u, 0u, 0u}; v0 = LoaderSysPrintf(ram, rt, a0, a1, a2, a3, t0, t1, t2, t3, sp, f12, f13, f14, f15, f16, f17, f18, f19, h2, a7c); a2 = a7c[2]; }
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)30 << 16))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)4096 << 16))));
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)a0, 12680))));
    (v0 = (uint64_t)(RC_U64X(v0) | (uint64_t)(uint16_t)15408));
    (a1 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)v0, 0))));
    { A3_CHARGE(); v0 = LoaderSysPrintf(ram, rt, a0, a1, a2, a3, t0, t1, t2, t3, sp, f12, f13, f14, f15, f16, f17, f18, f19, h2); }
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)30 << 16))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)4096 << 16))));
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)4096 << 16))));
    (t0 = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)4096 << 16))));
    (t1 = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)4096 << 16))));
    (v1 = (uint64_t)(RC_U64X(v1) | (uint64_t)(uint16_t)15632));
    (t1 = (uint64_t)(RC_U64X(t1) | (uint64_t)(uint16_t)15664));
    (v0 = (uint64_t)(RC_U64X(v0) | (uint64_t)(uint16_t)15616));
    (t0 = (uint64_t)(RC_U64X(t0) | (uint64_t)(uint16_t)15648));
    (a1 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)v0, 0))));
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)a0, 12704))));
    (a2 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)v1, 0))));
    (a3 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)t0, 0))));
    (t0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)t1, 0))));
    { A3_CHARGE(); v0 = LoaderSysPrintf(ram, rt, a0, a1, a2, a3, t0, t1, t2, t3, sp, f12, f13, f14, f15, f16, f17, f18, f19, h2); }
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)30 << 16))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)4096 << 16))));
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)4096 << 16))));
    (t0 = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)4096 << 16))));
    (t1 = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)4096 << 16))));
    (t1 = (uint64_t)(RC_U64X(t1) | (uint64_t)(uint16_t)15728));
    (v1 = (uint64_t)(RC_U64X(v1) | (uint64_t)(uint16_t)15696));
    (v0 = (uint64_t)(RC_U64X(v0) | (uint64_t)(uint16_t)15680));
    (t0 = (uint64_t)(RC_U64X(t0) | (uint64_t)(uint16_t)15712));
    (a1 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)v0, 0))));
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)a0, 12800))));
    (a2 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)v1, 0))));
    (a3 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)t0, 0))));
    (t0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)t1, 0))));
    { A3_CHARGE(); v0 = LoaderSysPrintf(ram, rt, a0, a1, a2, a3, t0, t1, t2, t3, sp, f12, f13, f14, f15, f16, f17, f18, f19, h2); }
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)30 << 16))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)4096 << 16))));
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)4096 << 16))));
    (t0 = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)4096 << 16))));
    (v1 = (uint64_t)(RC_U64X(v1) | (uint64_t)(uint16_t)15472));
    (t0 = (uint64_t)(RC_U64X(t0) | (uint64_t)(uint16_t)15440));
    (v0 = (uint64_t)(RC_U64X(v0) | (uint64_t)(uint16_t)15424));
    (a1 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)v0, 0))));
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)a0, 12896))));
    (a2 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)v1, 0))));
    (a3 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)t0, 0))));
    { A3_CHARGE(); v0 = LoaderSysPrintf(ram, rt, a0, a1, a2, a3, t0, t1, t2, t3, sp, f12, f13, f14, f15, f16, f17, f18, f19, h2); }
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)30 << 16))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)4096 << 16))));
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)4096 << 16))));
    (t0 = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)4096 << 16))));
    (t1 = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)4096 << 16))));
    (v1 = (uint64_t)(RC_U64X(v1) | (uint64_t)(uint16_t)15504));
    (t1 = (uint64_t)(RC_U64X(t1) | (uint64_t)(uint16_t)15536));
    (v0 = (uint64_t)(RC_U64X(v0) | (uint64_t)(uint16_t)15568));
    (t0 = (uint64_t)(RC_U64X(t0) | (uint64_t)(uint16_t)15520));
    (a1 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)v0, 0))));
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)a0, 12968))));
    (a2 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)v1, 0))));
    (a3 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)t0, 0))));
    (t0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)t1, 0))));
    { A3_CHARGE(); v0 = LoaderSysPrintf(ram, rt, a0, a1, a2, a3, t0, t1, t2, t3, sp, f12, f13, f14, f15, f16, f17, f18, f19, h2); }
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)30 << 16))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)4096 << 16))));
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)4096 << 16))));
    (t0 = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)4096 << 16))));
    (t1 = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)4096 << 16))));
    (v1 = (uint64_t)(RC_U64X(v1) | (uint64_t)(uint16_t)15552));
    (t1 = (uint64_t)(RC_U64X(t1) | (uint64_t)(uint16_t)15488));
    (v0 = (uint64_t)(RC_U64X(v0) | (uint64_t)(uint16_t)15584));
    (t0 = (uint64_t)(RC_U64X(t0) | (uint64_t)(uint16_t)15456));
    (a1 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)v0, 0))));
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)a0, 13064))));
    (a2 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)v1, 0))));
    (a3 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)t0, 0))));
    (t0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)t1, 0))));
    { A3_CHARGE(); uint64_t a7c[3] = {0u, 0u, 0u}; v0 = LoaderSysPrintf(ram, rt, a0, a1, a2, a3, t0, t1, t2, t3, sp, f12, f13, f14, f15, f16, f17, f18, f19, h2, a7c); a1 = a7c[1]; a2 = a7c[2]; }
    { (sp = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)sp, 128)))); if (xo) { xo[0] = a1; xo[1] = a2; xo[2] = a3; xo[3] = t0; xo[4] = t1; xo[5] = t3; } io_h2 = h2; return v0; }
    __builtin_unreachable();
}

// iosDumpGIFStatus (0x1c0be8): typed by tools/typed_lift2.py (round A6; guest frame calls, extra outputs xo[0]=a1 xo[1]=a2 xo[2]=a3 xo[3]=t0 xo[4]=t1 xo[5]=t2 xo[6]=t3; VU0 in -, out h2)
inline uint64_t iosDumpGIFStatus(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint64_t sp, float f12, float f13, float f14, float f15, float f16, float f17, float f18, float f19, uint64_t &io_h2, uint64_t *xo = nullptr)
{
    uint64_t v0 = 0;
    uint64_t v1 = 0;
    uint64_t a0 = 0;
    uint64_t a1 = 0;
    uint64_t a2 = 0;
    uint64_t a3 = 0;
    uint64_t t0 = 0;
    uint64_t t1 = 0;
    uint64_t t2 = 0;
    uint64_t t3 = 0;
    uint64_t t4 = 0;
    uint64_t t5 = 0;
    uint64_t s0 = 0;
    uint64_t s1 = 0;
    uint64_t s2 = 0;
    uint64_t s3 = 0;
    uint64_t s4 = 0;
    uint64_t s5 = 0;
    uint64_t s6 = 0;
    uint64_t s7 = 0;
    uint64_t fp = 0;
    uint64_t h2 = io_h2;
    (sp = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)sp, 4294967072))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)30 << 16))));
    (t0 = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)30 << 16))));
    (t1 = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)30 << 16))));
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)30 << 16))));
    (a1 = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)30 << 16))));
    (a2 = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)30 << 16))));
    (a3 = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)30 << 16))));
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)30 << 16))));
    (t5 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v0, 13176))));
    { uint32_t addr = ADD32((uint32_t)t5, 7); uint32_t aligned_addr = addr & ~7u; uint32_t offset = addr & 7u; uint64_t mem = A1_R64(aligned_addr); uint32_t shift = (7u - offset) << 3; uint64_t keepMask = (shift == 0) ? 0ull : ((1ull << shift) - 1ull); (t2 = (uint64_t)((t2 & keepMask) | (mem << shift))); }
    { uint32_t addr = ADD32((uint32_t)t5, 0); uint32_t aligned_addr = addr & ~7u; uint32_t offset = addr & 7u; uint64_t mem = A1_R64(aligned_addr); uint32_t shift = offset << 3; uint64_t keepMask = (offset == 0) ? 0ull : (0xFFFFFFFFFFFFFFFFull << ((8u - offset) << 3)); (t2 = (uint64_t)((t2 & keepMask) | (mem >> shift))); }
    { uint32_t addr = ADD32((uint32_t)sp, 7); uint32_t aligned_addr = addr & ~7u; uint32_t offset = addr & 7u; uint32_t shift = (7u - offset) << 3; uint64_t mask = 0xFFFFFFFFFFFFFFFFull >> shift; uint64_t old_data = A1_R64(aligned_addr); uint64_t val = t2; uint64_t new_data = (old_data & ~mask) | ((val >> shift) & mask); A1_W64(aligned_addr, new_data); }
    { uint32_t addr = ADD32((uint32_t)sp, 0); uint32_t aligned_addr = addr & ~7u; uint32_t offset = addr & 7u; uint32_t shift = offset << 3; uint64_t mask = 0xFFFFFFFFFFFFFFFFull << shift; uint64_t old_data = A1_R64(aligned_addr); uint64_t val = t2; uint64_t new_data = (old_data & ~mask) | ((val << shift) & mask); A1_W64(aligned_addr, new_data); }
    (t5 = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)4096 << 16))));
    (t5 = (uint64_t)(RC_U64X(t5) | (uint64_t)(uint16_t)12320));
    (t4 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v1, 13216))));
    { uint32_t addr = ADD32((uint32_t)t4, 7); uint32_t aligned_addr = addr & ~7u; uint32_t offset = addr & 7u; uint64_t mem = A1_R64(aligned_addr); uint32_t shift = (7u - offset) << 3; uint64_t keepMask = (shift == 0) ? 0ull : ((1ull << shift) - 1ull); (v0 = (uint64_t)((RC_U64X(v0) & keepMask) | (mem << shift))); }
    { uint32_t addr = ADD32((uint32_t)t4, 0); uint32_t aligned_addr = addr & ~7u; uint32_t offset = addr & 7u; uint64_t mem = A1_R64(aligned_addr); uint32_t shift = offset << 3; uint64_t keepMask = (offset == 0) ? 0ull : (0xFFFFFFFFFFFFFFFFull << ((8u - offset) << 3)); (v0 = (uint64_t)((v0 & keepMask) | (mem >> shift))); }
    { uint32_t addr = ADD32((uint32_t)sp, 23); uint32_t aligned_addr = addr & ~7u; uint32_t offset = addr & 7u; uint32_t shift = (7u - offset) << 3; uint64_t mask = 0xFFFFFFFFFFFFFFFFull >> shift; uint64_t old_data = A1_R64(aligned_addr); uint64_t val = v0; uint64_t new_data = (old_data & ~mask) | ((val >> shift) & mask); A1_W64(aligned_addr, new_data); }
    { uint32_t addr = ADD32((uint32_t)sp, 16); uint32_t aligned_addr = addr & ~7u; uint32_t offset = addr & 7u; uint32_t shift = offset << 3; uint64_t mask = 0xFFFFFFFFFFFFFFFFull << shift; uint64_t old_data = A1_R64(aligned_addr); uint64_t val = v0; uint64_t new_data = (old_data & ~mask) | ((val << shift) & mask); A1_W64(aligned_addr, new_data); }
    (t3 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)a0, 13248))));
    { uint32_t addr = ADD32((uint32_t)t3, 7); uint32_t aligned_addr = addr & ~7u; uint32_t offset = addr & 7u; uint64_t mem = A1_R64(aligned_addr); uint32_t shift = (7u - offset) << 3; uint64_t keepMask = (shift == 0) ? 0ull : ((1ull << shift) - 1ull); (v0 = (uint64_t)((v0 & keepMask) | (mem << shift))); }
    { uint32_t addr = ADD32((uint32_t)t3, 0); uint32_t aligned_addr = addr & ~7u; uint32_t offset = addr & 7u; uint64_t mem = A1_R64(aligned_addr); uint32_t shift = offset << 3; uint64_t keepMask = (offset == 0) ? 0ull : (0xFFFFFFFFFFFFFFFFull << ((8u - offset) << 3)); (v0 = (uint64_t)((v0 & keepMask) | (mem >> shift))); }
    { uint32_t addr = ADD32((uint32_t)sp, 39); uint32_t aligned_addr = addr & ~7u; uint32_t offset = addr & 7u; uint32_t shift = (7u - offset) << 3; uint64_t mask = 0xFFFFFFFFFFFFFFFFull >> shift; uint64_t old_data = A1_R64(aligned_addr); uint64_t val = v0; uint64_t new_data = (old_data & ~mask) | ((val >> shift) & mask); A1_W64(aligned_addr, new_data); }
    { uint32_t addr = ADD32((uint32_t)sp, 32); uint32_t aligned_addr = addr & ~7u; uint32_t offset = addr & 7u; uint32_t shift = offset << 3; uint64_t mask = 0xFFFFFFFFFFFFFFFFull << shift; uint64_t old_data = A1_R64(aligned_addr); uint64_t val = v0; uint64_t new_data = (old_data & ~mask) | ((val << shift) & mask); A1_W64(aligned_addr, new_data); }
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)30 << 16))));
    A1_W32B((uint32_t)sp, 128, (uint32_t)t5);
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)a0, 13432))));
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)a1, 13256))));
    { uint32_t addr = ADD32((uint32_t)v1, 7); uint32_t aligned_addr = addr & ~7u; uint32_t offset = addr & 7u; uint64_t mem = A1_R64(aligned_addr); uint32_t shift = (7u - offset) << 3; uint64_t keepMask = (shift == 0) ? 0ull : ((1ull << shift) - 1ull); (t4 = (uint64_t)((RC_U64X(t4) & keepMask) | (mem << shift))); }
    { uint32_t addr = ADD32((uint32_t)v1, 0); uint32_t aligned_addr = addr & ~7u; uint32_t offset = addr & 7u; uint64_t mem = A1_R64(aligned_addr); uint32_t shift = offset << 3; uint64_t keepMask = (offset == 0) ? 0ull : (0xFFFFFFFFFFFFFFFFull << ((8u - offset) << 3)); (t4 = (uint64_t)((t4 & keepMask) | (mem >> shift))); }
    { uint32_t addr = ADD32((uint32_t)sp, 55); uint32_t aligned_addr = addr & ~7u; uint32_t offset = addr & 7u; uint32_t shift = (7u - offset) << 3; uint64_t mask = 0xFFFFFFFFFFFFFFFFull >> shift; uint64_t old_data = A1_R64(aligned_addr); uint64_t val = t4; uint64_t new_data = (old_data & ~mask) | ((val >> shift) & mask); A1_W64(aligned_addr, new_data); }
    { uint32_t addr = ADD32((uint32_t)sp, 48); uint32_t aligned_addr = addr & ~7u; uint32_t offset = addr & 7u; uint32_t shift = offset << 3; uint64_t mask = 0xFFFFFFFFFFFFFFFFull << shift; uint64_t old_data = A1_R64(aligned_addr); uint64_t val = t4; uint64_t new_data = (old_data & ~mask) | ((val << shift) & mask); A1_W64(aligned_addr, new_data); }
    (t4 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)a2, 13280))));
    { uint32_t addr = ADD32((uint32_t)t4, 7); uint32_t aligned_addr = addr & ~7u; uint32_t offset = addr & 7u; uint64_t mem = A1_R64(aligned_addr); uint32_t shift = (7u - offset) << 3; uint64_t keepMask = (shift == 0) ? 0ull : ((1ull << shift) - 1ull); (a1 = (uint64_t)((RC_U64X(a1) & keepMask) | (mem << shift))); }
    { uint32_t addr = ADD32((uint32_t)t4, 0); uint32_t aligned_addr = addr & ~7u; uint32_t offset = addr & 7u; uint64_t mem = A1_R64(aligned_addr); uint32_t shift = offset << 3; uint64_t keepMask = (offset == 0) ? 0ull : (0xFFFFFFFFFFFFFFFFull << ((8u - offset) << 3)); (a1 = (uint64_t)((a1 & keepMask) | (mem >> shift))); }
    { uint32_t addr = ADD32((uint32_t)sp, 71); uint32_t aligned_addr = addr & ~7u; uint32_t offset = addr & 7u; uint32_t shift = (7u - offset) << 3; uint64_t mask = 0xFFFFFFFFFFFFFFFFull >> shift; uint64_t old_data = A1_R64(aligned_addr); uint64_t val = a1; uint64_t new_data = (old_data & ~mask) | ((val >> shift) & mask); A1_W64(aligned_addr, new_data); }
    { uint32_t addr = ADD32((uint32_t)sp, 64); uint32_t aligned_addr = addr & ~7u; uint32_t offset = addr & 7u; uint32_t shift = offset << 3; uint64_t mask = 0xFFFFFFFFFFFFFFFFull << shift; uint64_t old_data = A1_R64(aligned_addr); uint64_t val = a1; uint64_t new_data = (old_data & ~mask) | ((val << shift) & mask); A1_W64(aligned_addr, new_data); }
    (a1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)a3, 13312))));
    { uint32_t addr = ADD32((uint32_t)a1, 7); uint32_t aligned_addr = addr & ~7u; uint32_t offset = addr & 7u; uint64_t mem = A1_R64(aligned_addr); uint32_t shift = (7u - offset) << 3; uint64_t keepMask = (shift == 0) ? 0ull : ((1ull << shift) - 1ull); (t5 = (uint64_t)((RC_U64X(t5) & keepMask) | (mem << shift))); }
    { uint32_t addr = ADD32((uint32_t)a1, 0); uint32_t aligned_addr = addr & ~7u; uint32_t offset = addr & 7u; uint64_t mem = A1_R64(aligned_addr); uint32_t shift = offset << 3; uint64_t keepMask = (offset == 0) ? 0ull : (0xFFFFFFFFFFFFFFFFull << ((8u - offset) << 3)); (t5 = (uint64_t)((t5 & keepMask) | (mem >> shift))); }
    { uint32_t addr = ADD32((uint32_t)a1, 15); uint32_t aligned_addr = addr & ~7u; uint32_t offset = addr & 7u; uint64_t mem = A1_R64(aligned_addr); uint32_t shift = (7u - offset) << 3; uint64_t keepMask = (shift == 0) ? 0ull : ((1ull << shift) - 1ull); (v0 = (uint64_t)((v0 & keepMask) | (mem << shift))); }
    { uint32_t addr = ADD32((uint32_t)a1, 8); uint32_t aligned_addr = addr & ~7u; uint32_t offset = addr & 7u; uint64_t mem = A1_R64(aligned_addr); uint32_t shift = offset << 3; uint64_t keepMask = (offset == 0) ? 0ull : (0xFFFFFFFFFFFFFFFFull << ((8u - offset) << 3)); (v0 = (uint64_t)((v0 & keepMask) | (mem >> shift))); }
    { uint32_t addr = ADD32((uint32_t)sp, 87); uint32_t aligned_addr = addr & ~7u; uint32_t offset = addr & 7u; uint32_t shift = (7u - offset) << 3; uint64_t mask = 0xFFFFFFFFFFFFFFFFull >> shift; uint64_t old_data = A1_R64(aligned_addr); uint64_t val = t5; uint64_t new_data = (old_data & ~mask) | ((val >> shift) & mask); A1_W64(aligned_addr, new_data); }
    { uint32_t addr = ADD32((uint32_t)sp, 80); uint32_t aligned_addr = addr & ~7u; uint32_t offset = addr & 7u; uint32_t shift = offset << 3; uint64_t mask = 0xFFFFFFFFFFFFFFFFull << shift; uint64_t old_data = A1_R64(aligned_addr); uint64_t val = t5; uint64_t new_data = (old_data & ~mask) | ((val << shift) & mask); A1_W64(aligned_addr, new_data); }
    { uint32_t addr = ADD32((uint32_t)sp, 95); uint32_t aligned_addr = addr & ~7u; uint32_t offset = addr & 7u; uint32_t shift = (7u - offset) << 3; uint64_t mask = 0xFFFFFFFFFFFFFFFFull >> shift; uint64_t old_data = A1_R64(aligned_addr); uint64_t val = v0; uint64_t new_data = (old_data & ~mask) | ((val >> shift) & mask); A1_W64(aligned_addr, new_data); }
    { uint32_t addr = ADD32((uint32_t)sp, 88); uint32_t aligned_addr = addr & ~7u; uint32_t offset = addr & 7u; uint32_t shift = offset << 3; uint64_t mask = 0xFFFFFFFFFFFFFFFFull << shift; uint64_t old_data = A1_R64(aligned_addr); uint64_t val = v0; uint64_t new_data = (old_data & ~mask) | ((val << shift) & mask); A1_W64(aligned_addr, new_data); }
    (t4 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)t0, 13344))));
    { uint32_t addr = ADD32((uint32_t)t4, 7); uint32_t aligned_addr = addr & ~7u; uint32_t offset = addr & 7u; uint64_t mem = A1_R64(aligned_addr); uint32_t shift = (7u - offset) << 3; uint64_t keepMask = (shift == 0) ? 0ull : ((1ull << shift) - 1ull); (a2 = (uint64_t)((RC_U64X(a2) & keepMask) | (mem << shift))); }
    { uint32_t addr = ADD32((uint32_t)t4, 0); uint32_t aligned_addr = addr & ~7u; uint32_t offset = addr & 7u; uint64_t mem = A1_R64(aligned_addr); uint32_t shift = offset << 3; uint64_t keepMask = (offset == 0) ? 0ull : (0xFFFFFFFFFFFFFFFFull << ((8u - offset) << 3)); (a2 = (uint64_t)((a2 & keepMask) | (mem >> shift))); }
    { uint32_t addr = ADD32((uint32_t)sp, 103); uint32_t aligned_addr = addr & ~7u; uint32_t offset = addr & 7u; uint32_t shift = (7u - offset) << 3; uint64_t mask = 0xFFFFFFFFFFFFFFFFull >> shift; uint64_t old_data = A1_R64(aligned_addr); uint64_t val = a2; uint64_t new_data = (old_data & ~mask) | ((val >> shift) & mask); A1_W64(aligned_addr, new_data); }
    { uint32_t addr = ADD32((uint32_t)sp, 96); uint32_t aligned_addr = addr & ~7u; uint32_t offset = addr & 7u; uint32_t shift = offset << 3; uint64_t mask = 0xFFFFFFFFFFFFFFFFull << shift; uint64_t old_data = A1_R64(aligned_addr); uint64_t val = a2; uint64_t new_data = (old_data & ~mask) | ((val << shift) & mask); A1_W64(aligned_addr, new_data); }
    (a1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)t1, 13416))));
    { uint32_t addr = ADD32((uint32_t)a1, 7); uint32_t aligned_addr = addr & ~7u; uint32_t offset = addr & 7u; uint64_t mem = A1_R64(aligned_addr); uint32_t shift = (7u - offset) << 3; uint64_t keepMask = (shift == 0) ? 0ull : ((1ull << shift) - 1ull); (t5 = (uint64_t)((t5 & keepMask) | (mem << shift))); }
    { uint32_t addr = ADD32((uint32_t)a1, 0); uint32_t aligned_addr = addr & ~7u; uint32_t offset = addr & 7u; uint64_t mem = A1_R64(aligned_addr); uint32_t shift = offset << 3; uint64_t keepMask = (offset == 0) ? 0ull : (0xFFFFFFFFFFFFFFFFull << ((8u - offset) << 3)); (t5 = (uint64_t)((t5 & keepMask) | (mem >> shift))); }
    { uint32_t addr = ADD32((uint32_t)a1, 15); uint32_t aligned_addr = addr & ~7u; uint32_t offset = addr & 7u; uint64_t mem = A1_R64(aligned_addr); uint32_t shift = (7u - offset) << 3; uint64_t keepMask = (shift == 0) ? 0ull : ((1ull << shift) - 1ull); (v0 = (uint64_t)((v0 & keepMask) | (mem << shift))); }
    { uint32_t addr = ADD32((uint32_t)a1, 8); uint32_t aligned_addr = addr & ~7u; uint32_t offset = addr & 7u; uint64_t mem = A1_R64(aligned_addr); uint32_t shift = offset << 3; uint64_t keepMask = (offset == 0) ? 0ull : (0xFFFFFFFFFFFFFFFFull << ((8u - offset) << 3)); (v0 = (uint64_t)((v0 & keepMask) | (mem >> shift))); }
    { uint32_t addr = ADD32((uint32_t)sp, 119); uint32_t aligned_addr = addr & ~7u; uint32_t offset = addr & 7u; uint32_t shift = (7u - offset) << 3; uint64_t mask = 0xFFFFFFFFFFFFFFFFull >> shift; uint64_t old_data = A1_R64(aligned_addr); uint64_t val = t5; uint64_t new_data = (old_data & ~mask) | ((val >> shift) & mask); A1_W64(aligned_addr, new_data); }
    { uint32_t addr = ADD32((uint32_t)sp, 112); uint32_t aligned_addr = addr & ~7u; uint32_t offset = addr & 7u; uint32_t shift = offset << 3; uint64_t mask = 0xFFFFFFFFFFFFFFFFull << shift; uint64_t old_data = A1_R64(aligned_addr); uint64_t val = t5; uint64_t new_data = (old_data & ~mask) | ((val << shift) & mask); A1_W64(aligned_addr, new_data); }
    { uint32_t addr = ADD32((uint32_t)sp, 127); uint32_t aligned_addr = addr & ~7u; uint32_t offset = addr & 7u; uint32_t shift = (7u - offset) << 3; uint64_t mask = 0xFFFFFFFFFFFFFFFFull >> shift; uint64_t old_data = A1_R64(aligned_addr); uint64_t val = v0; uint64_t new_data = (old_data & ~mask) | ((val >> shift) & mask); A1_W64(aligned_addr, new_data); }
    { uint32_t addr = ADD32((uint32_t)sp, 120); uint32_t aligned_addr = addr & ~7u; uint32_t offset = addr & 7u; uint32_t shift = offset << 3; uint64_t mask = 0xFFFFFFFFFFFFFFFFull << shift; uint64_t old_data = A1_R64(aligned_addr); uint64_t val = v0; uint64_t new_data = (old_data & ~mask) | ((val << shift) & mask); A1_W64(aligned_addr, new_data); }
    (a2 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)sp, 128))));
    (s0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)a2, 0))));
    { (s4 = (uint64_t)((int64_t)(int32_t)((int32_t)SRL32((uint32_t)s0, 1)))); A3_CHARGE(); v0 = LoaderSysPrintf(ram, rt, a0, a1, a2, a3, t0, t1, t2, t3, sp, f12, f13, f14, f15, f16, f17, f18, f19, h2); }
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)30 << 16))));
    (v0 = (uint64_t)(s0 & (uint64_t)(uint16_t)1));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)SLL32((uint32_t)v0, 2))));
    (a3 = (uint64_t)((int64_t)(int32_t)((int32_t)SRL32((uint32_t)s0, 24))));
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)sp, (uint32_t)v0))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)SRL32((uint32_t)s0, 8))));
    (a1 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)v1, 0))));
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)SRL32((uint32_t)s0, 9))));
    (a2 = (uint64_t)((int64_t)(int32_t)((int32_t)SRL32((uint32_t)s0, 10))));
    (a3 = (uint64_t)(RC_U64X(a3) & (uint64_t)(uint16_t)31));
    (a2 = (uint64_t)(RC_U64X(a2) & (uint64_t)(uint16_t)3));
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)a0, 13488))));
    (s4 = (uint64_t)(s4 & (uint64_t)(uint16_t)1));
    A1_W32B((uint32_t)sp, 132, (uint32_t)a2);
    A1_W32B((uint32_t)sp, 136, (uint32_t)a3);
    (s7 = (uint64_t)(RC_U64X(v1) & (uint64_t)(uint16_t)1));
    { (fp = (uint64_t)(RC_U64X(v0) & (uint64_t)(uint16_t)1)); A3_CHARGE(); uint64_t a7c[3] = {0u, 0u, 0u}; v0 = LoaderSysPrintf(ram, rt, a0, a1, a2, a3, t0, t1, t2, t3, sp, f12, f13, f14, f15, f16, f17, f18, f19, h2, a7c); a2 = a7c[2]; }
    (s4 = (uint64_t)((int64_t)(int32_t)((int32_t)SLL32((uint32_t)s4, 2))));
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)30 << 16))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)sp, (uint32_t)s4))));
    (s5 = (uint64_t)((int64_t)(int32_t)((int32_t)SRL32((uint32_t)s0, 2))));
    (a1 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)v0, 0))));
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)a0, 13512))));
    (s5 = (uint64_t)(RC_U64X(s5) & (uint64_t)(uint16_t)1));
    { (s6 = (uint64_t)((int64_t)(int32_t)((int32_t)SRL32((uint32_t)s0, 3)))); A3_CHARGE(); uint64_t a7c[3] = {0u, 0u, 0u}; v0 = LoaderSysPrintf(ram, rt, a0, a1, a2, a3, t0, t1, t2, t3, sp, f12, f13, f14, f15, f16, f17, f18, f19, h2, a7c); a2 = a7c[2]; }
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)30 << 16))));
    (t2 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)sp, 16))));
    (s5 = (uint64_t)((int64_t)(int32_t)((int32_t)SLL32((uint32_t)s5, 2))));
    (a1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)t2, (uint32_t)s5))));
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)a0, 13536))));
    (a1 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)a1, 0))));
    (s6 = (uint64_t)(s6 & (uint64_t)(uint16_t)1));
    (s6 = (uint64_t)((int64_t)(int32_t)((int32_t)SLL32((uint32_t)s6, 2))));
    { (s1 = (uint64_t)((int64_t)(int32_t)((int32_t)SRL32((uint32_t)s0, 5)))); A3_CHARGE(); uint64_t a7c[3] = {0u, 0u, 0u}; v0 = LoaderSysPrintf(ram, rt, a0, a1, a2, a3, t0, t1, t2, t3, sp, f12, f13, f14, f15, f16, f17, f18, f19, h2, a7c); a2 = a7c[2]; }
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)30 << 16))));
    (t3 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)sp, 32))));
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)a0, 13560))));
    (a1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)t3, (uint32_t)s6))));
    (s1 = (uint64_t)(s1 & (uint64_t)(uint16_t)1));
    (a1 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)a1, 0))));
    (s1 = (uint64_t)((int64_t)(int32_t)((int32_t)SLL32((uint32_t)s1, 2))));
    (s2 = (uint64_t)((int64_t)(int32_t)((int32_t)SRL32((uint32_t)s0, 6))));
    { (s3 = (uint64_t)((int64_t)(int32_t)((int32_t)SRL32((uint32_t)s0, 7)))); A3_CHARGE(); uint64_t a7c[3] = {0u, 0u, 0u}; v0 = LoaderSysPrintf(ram, rt, a0, a1, a2, a3, t0, t1, t2, t3, sp, f12, f13, f14, f15, f16, f17, f18, f19, h2, a7c); a2 = a7c[2]; }
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)30 << 16))));
    (t4 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)sp, 48))));
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)a0, 13584))));
    (s1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)t4, (uint32_t)s1))));
    (s2 = (uint64_t)(s2 & (uint64_t)(uint16_t)1));
    (a1 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)s1, 0))));
    (s2 = (uint64_t)((int64_t)(int32_t)((int32_t)SLL32((uint32_t)s2, 2))));
    (s3 = (uint64_t)(s3 & (uint64_t)(uint16_t)1));
    { (s0 = (uint64_t)((int64_t)(int32_t)((int32_t)SRL32((uint32_t)s0, 12)))); A3_CHARGE(); uint64_t a7c[3] = {0u, 0u, 0u}; v0 = LoaderSysPrintf(ram, rt, a0, a1, a2, a3, t0, t1, t2, t3, sp, f12, f13, f14, f15, f16, f17, f18, f19, h2, a7c); a2 = a7c[2]; }
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)30 << 16))));
    (t5 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)sp, 48))));
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)a0, 13608))));
    (s2 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)t5, (uint32_t)s2))));
    (s3 = (uint64_t)((int64_t)(int32_t)((int32_t)SLL32((uint32_t)s3, 2))));
    (a1 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)s2, 0))));
    (s0 = (uint64_t)(s0 & (uint64_t)(uint16_t)1));
    { (s0 = (uint64_t)((int64_t)(int32_t)((int32_t)SLL32((uint32_t)s0, 2)))); A3_CHARGE(); uint64_t a7c[3] = {0u, 0u, 0u}; v0 = LoaderSysPrintf(ram, rt, a0, a1, a2, a3, t0, t1, t2, t3, sp, f12, f13, f14, f15, f16, f17, f18, f19, h2, a7c); a2 = a7c[2]; }
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)30 << 16))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)sp, 48))));
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)a0, 13632))));
    (s3 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v0, (uint32_t)s3))));
    { (a1 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)s3, 0)))); A3_CHARGE(); uint64_t a7c[3] = {0u, 0u, 0u}; v0 = LoaderSysPrintf(ram, rt, a0, a1, a2, a3, t0, t1, t2, t3, sp, f12, f13, f14, f15, f16, f17, f18, f19, h2, a7c); a2 = a7c[2]; }
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)30 << 16))));
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)sp, 48))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)SLL32((uint32_t)fp, 2))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v1, (uint32_t)v0))));
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)a0, 13656))));
    { (a1 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)v0, 0)))); A3_CHARGE(); v0 = LoaderSysPrintf(ram, rt, a0, a1, a2, a3, t0, t1, t2, t3, sp, f12, f13, f14, f15, f16, f17, f18, f19, h2); }
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)30 << 16))));
    (a2 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)sp, 64))));
    (a1 = (uint64_t)((int64_t)(int32_t)((int32_t)SLL32((uint32_t)s7, 2))));
    (a1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)a2, (uint32_t)a1))));
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)a0, 13680))));
    { (a1 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)a1, 0)))); A3_CHARGE(); uint64_t a7c[3] = {0u, 0u, 0u}; v0 = LoaderSysPrintf(ram, rt, a0, a1, a2, a3, t0, t1, t2, t3, sp, f12, f13, f14, f15, f16, f17, f18, f19, h2, a7c); a2 = a7c[2]; }
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)30 << 16))));
    (t2 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)sp, 132))));
    (t3 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)sp, 80))));
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)a0, 13704))));
    (a1 = (uint64_t)((int64_t)(int32_t)((int32_t)SLL32((uint32_t)t2, 2))));
    (a1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)t3, (uint32_t)a1))));
    { (a1 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)a1, 0)))); A3_CHARGE(); uint64_t a7c[3] = {0u, 0u, 0u}; v0 = LoaderSysPrintf(ram, rt, a0, a1, a2, a3, t0, t1, t2, t3, sp, f12, f13, f14, f15, f16, f17, f18, f19, h2, a7c); a2 = a7c[2]; }
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)30 << 16))));
    (t4 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)sp, 96))));
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)a0, 13728))));
    (a1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)t4, (uint32_t)s0))));
    { (a1 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)a1, 0)))); A3_CHARGE(); v0 = LoaderSysPrintf(ram, rt, a0, a1, a2, a3, t0, t1, t2, t3, sp, f12, f13, f14, f15, f16, f17, f18, f19, h2); }
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)30 << 16))));
    (a1 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)sp, 136))));
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)a0, 13752))));
    { (a2 = (uint64_t)((uint64_t)RC_U64X(a1) + (uint64_t)0ULL)); A3_CHARGE(); uint64_t a7c[3] = {0u, 0u, 0u}; v0 = LoaderSysPrintf(ram, rt, a0, a1, a2, a3, t0, t1, t2, t3, sp, f12, f13, f14, f15, f16, f17, f18, f19, h2, a7c); a1 = a7c[1]; a2 = a7c[2]; }
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)30 << 16))));
    { (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)a0, 12648)))); A3_CHARGE(); v0 = LoaderSysPrintf(ram, rt, a0, a1, a2, a3, t0, t1, t2, t3, sp, f12, f13, f14, f15, f16, f17, f18, f19, h2); }
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)30 << 16))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)4096 << 16))));
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)4096 << 16))));
    (v1 = (uint64_t)(RC_U64X(v1) | (uint64_t)(uint16_t)12384));
    (v0 = (uint64_t)(RC_U64X(v0) | (uint64_t)(uint16_t)12400));
    (a1 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)v0, 0))));
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)a0, 13784))));
    (a2 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)v1, 0))));
    { A3_CHARGE(); v0 = LoaderSysPrintf(ram, rt, a0, a1, a2, a3, t0, t1, t2, t3, sp, f12, f13, f14, f15, f16, f17, f18, f19, h2); }
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)30 << 16))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)4096 << 16))));
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)4096 << 16))));
    (v1 = (uint64_t)(RC_U64X(v1) | (uint64_t)(uint16_t)12368));
    (v0 = (uint64_t)(RC_U64X(v0) | (uint64_t)(uint16_t)12352));
    (a1 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)v0, 0))));
    (t5 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)sp, 112))));
    (a2 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)v0, 0))));
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)a0, 13816))));
    (a3 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)v0, 0))));
    (a1 = (uint64_t)(RC_U64X(a1) & (uint64_t)(uint16_t)32767));
    (t0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)v1, 0))));
    (a2 = (uint64_t)(RC_U64X(a2) & (uint64_t)(uint16_t)32767));
    (t1 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)v1, 0))));
    (a3 = (uint64_t)((int64_t)(int32_t)((int32_t)SRL32((uint32_t)a3, 15))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)v1, 0))));
    (t0 = (uint64_t)((int64_t)(int32_t)((int32_t)SRL32((uint32_t)t0, 14))));
    (t3 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)v1, 0))));
    (t1 = (uint64_t)((int64_t)(int32_t)((int32_t)SRL32((uint32_t)t1, 15))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)SRL32((uint32_t)v0, 24))));
    (t0 = (uint64_t)(RC_U64X(t0) & (uint64_t)(uint16_t)1));
    (v0 = (uint64_t)(RC_U64X(v0) & (uint64_t)(uint16_t)12));
    (t1 = (uint64_t)(RC_U64X(t1) & (uint64_t)(uint16_t)2047));
    (t2 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)t5, (uint32_t)v0))));
    (t3 = (uint64_t)((int64_t)(int32_t)((int32_t)SRL32((uint32_t)t3, 28))));
    (t2 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)t2, 0))));
    { (a3 = (uint64_t)(RC_U64X(a3) & (uint64_t)(uint16_t)1)); A3_CHARGE(); v0 = LoaderSysPrintf(ram, rt, a0, a1, a2, a3, t0, t1, t2, t3, sp, f12, f13, f14, f15, f16, f17, f18, f19, h2); }
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)30 << 16))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)4096 << 16))));
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)a0, 13880))));
    (v0 = (uint64_t)(RC_U64X(v0) | (uint64_t)(uint16_t)12416));
    (a1 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)v0, 0))));
    (a2 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)v0, 0))));
    (a3 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)v0, 0))));
    (a1 = (uint64_t)(RC_U64X(a1) & (uint64_t)(uint16_t)32767));
    (a2 = (uint64_t)((int64_t)(int32_t)((int32_t)SRL32((uint32_t)a2, 16))));
    (a3 = (uint64_t)((int64_t)(int32_t)((int32_t)SRL32((uint32_t)a3, 20))));
    (a2 = (uint64_t)(RC_U64X(a2) & (uint64_t)(uint16_t)15));
    { (a3 = (uint64_t)(RC_U64X(a3) & (uint64_t)(uint16_t)1023)); A3_CHARGE(); uint64_t a7c[3] = {0u, 0u, 0u}; v0 = LoaderSysPrintf(ram, rt, a0, a1, a2, a3, t0, t1, t2, t3, sp, f12, f13, f14, f15, f16, f17, f18, f19, h2, a7c); a2 = a7c[2]; }
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)30 << 16))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)sp, 128))));
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)a0, 13952))));
    (a1 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)v0, 0))));
    { A3_CHARGE(); uint64_t a7c[3] = {0u, 0u, 0u}; v0 = LoaderSysPrintf(ram, rt, a0, a1, a2, a3, t0, t1, t2, t3, sp, f12, f13, f14, f15, f16, f17, f18, f19, h2, a7c); a1 = a7c[1]; a2 = a7c[2]; }
    { (sp = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)sp, 224)))); if (xo) { xo[0] = a1; xo[1] = a2; xo[2] = a3; xo[3] = t0; xo[4] = t1; xo[5] = t2; xo[6] = t3; } io_h2 = h2; return v0; }
    __builtin_unreachable();
}

// cameraBlurClearIntensityArray (0x108b9c8): typed by tools/typed_lift2.py (round A6; loop guest frame calls, extra outputs xo[0]=f12 xo[1]=f20; VU0 in h4, out vf5 vf6 h3)
inline uint32_t cameraBlurClearIntensityArray(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t sp, float f13, float f14, float f15, float f16, float f17, float f18, float f19, float f20, a4::vec4 &io_vf5, a4::vec4 &io_vf6, uint64_t &io_h3, uint64_t io_h4, uint64_t *xo = nullptr)
{
    float f12 = 0.0f;
    uint32_t v0 = 0;
    uint64_t v1 = 0;
    uint64_t a0 = 0;
    a4::vec4 vf5 = io_vf5;
    a4::vec4 vf6 = io_vf6;
    uint64_t h3 = io_h3;
    uint64_t h4 = io_h4;
    sp = sp - 0x20u;
    v0 = 0x1210000u;
    v0 = v0 + 0x4EC0u;
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 7))));
    v0 = v0 + 0x1Cu;
L_108b9e0:;
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v1, 4294967295))));
    A1_W32(v0, 0u);
    { const bool c = (int32_t)v1 >= 0; v0 = v0 - 4u; if (c) { A3_BACK(); goto L_108b9e0; } }
    v0 = 0x1180000u;
    (a0 = (uint64_t)((uint64_t)a1_sx64(sp) + (uint64_t)0ULL));
    { f12 = a1_flt(A1_R32B(v0, 25448)); A3_CHARGE(); uint64_t a7c[1] = {0u}; v0 = gluGetLookAtPosition(ram, rt, v0, a0, f12, f13, f14, f15, f16, f17, f18, f19, f20, vf5, vf6, h4, a7c); f20 = a1_flt((uint32_t)a7c[0]); }
    v0 = 0x1210000u;
    do { const a4::q128 _v = (A4_R128(ADD32(sp, 0))); h3 = a4::hi64(_v); std::memcpy(&v1, &_v, 8); } while (0);
    v0 = v0 + 0x4EB0u;
    A4_W128(ADD32(v0, 0), a4::mk128(h3, v1));
    { sp = sp + 0x20u; if (xo) { xo[0] = a1_bits(f12); xo[1] = a1_bits(f20); } io_vf5 = vf5; io_vf6 = vf6; io_h3 = h3; return v0; }
    __builtin_unreachable();
}

// manager_sub_01097F88 (0x1097f88): typed by tools/typed_lift2.py (round A6; guest frame calls, extra outputs xo[0]=f20 xo[1]=f21 xo[2]=a3 xo[3]=t0 xo[4]=t1; VU0 in vf0 vf7 vf10 vacc vstat h4, out vf5 vf6 vf7 vf8 vf10 vf14 vf15 vf16 vf17 vacc vq vstat h2 h3 h6 h7 h8 h9, jump tables 1, relocated calls 3)
inline a1::RetVF manager_sub_01097F88(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint64_t a0, uint64_t a1, uint64_t a2, uint64_t sp, float f12, float f13, float f14, float f15, float f16, float f17, float f18, float f19, float f20, float f21, const a4::vec4 &io_vf0, a4::vec4 &io_vf5, a4::vec4 &io_vf6, a4::vec4 &io_vf7, a4::vec4 &io_vf8, a4::vec4 &io_vf10, a4::vec4 &io_vf14, a4::vec4 &io_vf15, a4::vec4 &io_vf16, a4::vec4 &io_vf17, a4::vec4 &io_vacc, float &io_vq, uint16_t &io_vstat, uint64_t &io_h2, uint64_t &io_h3, uint64_t io_h4, uint64_t &io_h6, uint64_t &io_h7, uint64_t &io_h8, uint64_t &io_h9, uint64_t *xo = nullptr)
{
    float f0 = 0.0f;
    float f1 = 0.0f;
    float f2 = 0.0f;
    float f3 = 0.0f;
    float f4 = 0.0f;
    uint64_t at = 0;
    uint64_t v0 = 0;
    uint64_t v1 = 0;
    uint64_t a3 = 0;
    uint64_t t0 = 0;
    uint64_t t1 = 0;
    uint64_t s0 = 0;
    uint64_t s1 = 0;
    uint64_t s2 = 0;
    uint64_t s3 = 0;
    uint64_t s4 = 0;
    uint32_t fcr31 = 0;
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
    float vq = io_vq;
    uint16_t vstat = io_vstat;
    uint64_t h2 = io_h2;
    uint64_t h3 = io_h3;
    uint64_t h4 = io_h4;
    uint64_t h6 = io_h6;
    uint64_t h7 = io_h7;
    uint64_t h8 = io_h8;
    uint64_t h9 = io_h9;
    (sp = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)sp, 4294967136))));
    (s1 = (uint64_t)((uint64_t)a0 + (uint64_t)0ULL));
    (s3 = (uint64_t)((uint64_t)a2 + (uint64_t)0ULL));
    (s4 = (uint64_t)((uint64_t)a1 + (uint64_t)0ULL));
    { float f = f21; uint32_t bits; std::memcpy(&bits, &f, sizeof(bits)); A1_W32B((uint32_t)sp, 152, bits); }
    f21 = ((float)(f12));
    { float f = f20; uint32_t bits; std::memcpy(&bits, &f, sizeof(bits)); A1_W32B((uint32_t)sp, 144, bits); }
    (a0 = (uint64_t)((uint64_t)RC_U64X(sp) + (uint64_t)0ULL));
    (a1 = (uint64_t)((uint64_t)s3 + (uint64_t)0ULL));
    (at = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)49024 << 16))));
    { uint32_t bits = (uint32_t)at; std::memcpy(&f20, &bits, sizeof(bits)); }
    { (s2 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)s1, 12)))); A3_CHARGE(); uint64_t a7c[4] = {0u, 0u, 0u, 0u}; iosCopyMatrix_x8(ram, rt, (uint32_t)a0, (uint32_t)a1, h6, h7, h8, h9, a7c); a3 = a7c[1]; t0 = a7c[2]; t1 = a7c[3]; }
    (a0 = (uint64_t)(A1_R64B((uint32_t)s1, 24)));
    (v0 = (uint64_t)(a0 << 29));
    (v0 = (uint64_t)((int64_t)v0 >> (32 + 0)));
    (v0 = (uint64_t)(v0 & (uint64_t)(uint16_t)7));
    (v1 = (uint64_t)(((uint64_t)RC_U64X(v0) < (uint64_t)(int64_t)(int32_t)7) ? 1 : 0));
    { const bool c = (int64_t)v1 == 0; (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)SLL32((uint32_t)v0, 2)))); if (c) goto L_10982f0; }
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)281 << 16))));
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v1, (uint32_t)v0))));
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)v1, 8096))));
    { const uint32_t a5_jt = (uint32_t)v1; switch (a5_jt) { case 0x1098008u: goto L_1098008; case 0x1098010u: goto L_1098010; case 0x10980A0u: goto L_10980a0; case 0x1098120u: goto L_1098120; case 0x10981B8u: goto L_10981b8; case 0x1098250u: goto L_1098250; case 0x1098424u: goto L_1098424; default: A5_BADJUMP(0x1097FFCu, a5_jt); } }
L_1098008:;
    { uint32_t bits = A1_R32B((uint32_t)s4, 564); float f; std::memcpy(&f, &bits, sizeof(f)); f20 = f; }
    goto L_10982f0;
L_1098010:;
    do { const a4::q128 _v = (A4_R128(ADD32((uint32_t)s1, 32))); h2 = a4::hi64(_v); std::memcpy(&v0, &_v, 8); } while (0);
    (s0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)sp, 64))));
    (at = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)16256 << 16))));
    { uint32_t bits = (uint32_t)at; std::memcpy(&f0, &bits, sizeof(bits)); }
    A4_W128(ADD32((uint32_t)sp, 64), a4::mk128(h2, v0));
    { float f = f0; uint32_t bits; std::memcpy(&bits, &f, sizeof(bits)); A1_W32B((uint32_t)sp, 76, bits); }
    vf8 = a4v::castsi128_ps(A4_R128(ADD32((uint32_t)s0, 0)));
    vf14 = a4v::castsi128_ps(A4_R128(ADD32((uint32_t)sp, 0)));
    vf15 = a4v::castsi128_ps(A4_R128(ADD32((uint32_t)sp, 16)));
    vf16 = a4v::castsi128_ps(A4_R128(ADD32((uint32_t)sp, 32)));
    vf17 = a4v::castsi128_ps(A4_R128(ADD32((uint32_t)sp, 48)));
    { a4::vec4 res = a4v::mul(vf14, a4v::bcast<0>(vf8)); vacc = a4v::dest<1, 1, 1, 1>(vacc, res); }
    { a4::vec4 mul_res = a4v::mul(vf15, a4v::bcast<1>(vf8)); a4::vec4 res = a4v::addK<1, 1>(vacc, mul_res); vacc = a4v::dest<1, 1, 1, 1>(vacc, res); }
    { a4::vec4 mul_res = a4v::mul(vf16, a4v::bcast<2>(vf8)); a4::vec4 res = a4v::addK<1, 1>(vacc, mul_res); vacc = a4v::dest<1, 1, 1, 1>(vacc, res); }
    { a4::vec4 mul_res = a4v::mul(vf17, a4v::bcast<3>(vf8)); a4::vec4 res = a4v::addK<1, 1>(vacc, mul_res); a4::q128 mask = a4v::set_epi32(-1, -1, -1, -1); vf10 = a4v::blendv_ps(vf10, res, a4v::castsi128_ps(mask)); }
    A4_W128(ADD32((uint32_t)s0, 0), a4v::castps_si128(vf10));
    { A3_CHARGE(); v0 = a1_sx64(iosgluGetCameraContext(ram, rt, f12, f13, f14, f15, f16, f17, f18, f19)); }
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v0, 320))));
    vf5 = a4v::castsi128_ps(A4_R128(ADD32((uint32_t)s0, 0)));
    vf6 = a4v::castsi128_ps(A4_R128(ADD32((uint32_t)v0, 0)));
    { a4::vec4 res = a4v::sub(vf5, vf6); a4::q128 mask = a4v::set_epi32(0, -1, -1, -1); vf5 = a4v::blend(vf5, res, a4v::castsi128_ps(mask)); }
    A4_W128(ADD32((uint32_t)s0, 0), a4v::castps_si128(vf5));
    A1_W32B((uint32_t)sp, 68, 0u);
    vf7 = a4v::castsi128_ps(A4_R128(ADD32((uint32_t)s0, 0)));
    { a4::vec4 res = a4v::mul(vf7, vf7); a4::q128 mask = a4v::set_epi32(0, -1, -1, -1); vf7 = a4v::blend(vf7, res, a4v::castsi128_ps(mask)); }
    { a4::vec4 res = a4v::mulK<1, 1>(vf0, a4v::bcast<0>(vf7)); vacc = a4v::dest<0, 0, 0, 1>(vacc, res); }
    { a4::vec4 mul_res = a4v::mulK<1, 1>(vf0, a4v::bcast<1>(vf7)); a4::vec4 res = a4v::addK<1, 1>(vacc, mul_res); vacc = a4v::dest<0, 0, 0, 1>(vacc, res); }
    { a4::vec4 mul_res = a4v::mulK<1, 1>(vf0, a4v::bcast<2>(vf7)); a4::vec4 res = a4v::addK<1, 1>(vacc, mul_res); a4::q128 mask = a4v::set_epi32(-1, 0, 0, 0); vf7 = a4v::blendv_ps(vf7, res, a4v::castsi128_ps(mask)); }
    { float ft = a4v::cvtss_f32(a4v::shuffle_ps<0x03>(vf7, vf7)); vq = a4v::sqrt(vstat, ft); }
    { uint32_t bits; std::memcpy(&bits, &vq, sizeof(bits)); (v0 = (uint64_t)((int64_t)(int32_t)(bits))); }
    { uint32_t bits = (uint32_t)v0; std::memcpy(&f20, &bits, sizeof(bits)); }
    (a0 = (uint64_t)(A1_R64B((uint32_t)s1, 24)));
    goto L_10982f0;
L_10980a0:;
    do { const a4::q128 _v = (A4_R128(ADD32((uint32_t)s1, 32))); h2 = a4::hi64(_v); std::memcpy(&v0, &_v, 8); } while (0);
    (s0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)sp, 64))));
    (at = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)16256 << 16))));
    { uint32_t bits = (uint32_t)at; std::memcpy(&f0, &bits, sizeof(bits)); }
    A4_W128(ADD32((uint32_t)sp, 64), a4::mk128(h2, v0));
    { float f = f0; uint32_t bits; std::memcpy(&bits, &f, sizeof(bits)); A1_W32B((uint32_t)sp, 76, bits); }
    vf8 = a4v::castsi128_ps(A4_R128(ADD32((uint32_t)s0, 0)));
    vf14 = a4v::castsi128_ps(A4_R128(ADD32((uint32_t)sp, 0)));
    vf15 = a4v::castsi128_ps(A4_R128(ADD32((uint32_t)sp, 16)));
    vf16 = a4v::castsi128_ps(A4_R128(ADD32((uint32_t)sp, 32)));
    vf17 = a4v::castsi128_ps(A4_R128(ADD32((uint32_t)sp, 48)));
    { a4::vec4 res = a4v::mul(vf14, a4v::bcast<0>(vf8)); vacc = a4v::dest<1, 1, 1, 1>(vacc, res); }
    { a4::vec4 mul_res = a4v::mul(vf15, a4v::bcast<1>(vf8)); a4::vec4 res = a4v::addK<1, 1>(vacc, mul_res); vacc = a4v::dest<1, 1, 1, 1>(vacc, res); }
    { a4::vec4 mul_res = a4v::mul(vf16, a4v::bcast<2>(vf8)); a4::vec4 res = a4v::addK<1, 1>(vacc, mul_res); vacc = a4v::dest<1, 1, 1, 1>(vacc, res); }
    { a4::vec4 mul_res = a4v::mul(vf17, a4v::bcast<3>(vf8)); a4::vec4 res = a4v::addK<1, 1>(vacc, mul_res); a4::q128 mask = a4v::set_epi32(-1, -1, -1, -1); vf10 = a4v::blendv_ps(vf10, res, a4v::castsi128_ps(mask)); }
    A4_W128(ADD32((uint32_t)s0, 0), a4v::castps_si128(vf10));
    { A3_CHARGE(); v0 = a1_sx64(iosgluGetCameraContext(ram, rt, f12, f13, f14, f15, f16, f17, f18, f19)); }
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v0, 320))));
    vf5 = a4v::castsi128_ps(A4_R128(ADD32((uint32_t)s0, 0)));
    vf6 = a4v::castsi128_ps(A4_R128(ADD32((uint32_t)v1, 0)));
    { a4::vec4 res = a4v::sub(vf5, vf6); a4::q128 mask = a4v::set_epi32(-1, -1, -1, -1); vf7 = a4v::blend(vf7, res, a4v::castsi128_ps(mask)); }
    { a4::vec4 res = a4v::mulK<1, 1>(vf7, vf7); a4::q128 mask = a4v::set_epi32(0, -1, -1, -1); vf7 = a4v::blend(vf7, res, a4v::castsi128_ps(mask)); }
    { a4::vec4 res = a4v::mulK<1, 1>(vf0, a4v::bcast<0>(vf7)); vacc = a4v::dest<0, 0, 0, 1>(vacc, res); }
    { a4::vec4 mul_res = a4v::mulK<1, 1>(vf0, a4v::bcast<1>(vf7)); a4::vec4 res = a4v::addK<1, 1>(vacc, mul_res); vacc = a4v::dest<0, 0, 0, 1>(vacc, res); }
    { a4::vec4 mul_res = a4v::mulK<1, 1>(vf0, a4v::bcast<2>(vf7)); a4::vec4 res = a4v::addK<1, 1>(vacc, mul_res); a4::q128 mask = a4v::set_epi32(-1, 0, 0, 0); vf7 = a4v::blendv_ps(vf7, res, a4v::castsi128_ps(mask)); }
    { float ft = a4v::cvtss_f32(a4v::shuffle_ps<0x03>(vf7, vf7)); vq = a4v::sqrt(vstat, ft); }
    { uint32_t bits; std::memcpy(&bits, &vq, sizeof(bits)); (v0 = (uint64_t)((int64_t)(int32_t)(bits))); }
    { uint32_t bits = (uint32_t)v0; std::memcpy(&f20, &bits, sizeof(bits)); }
    (a0 = (uint64_t)(A1_R64B((uint32_t)s1, 24)));
    goto L_10982f0;
L_1098120:;
    do { const a4::q128 _v = (A4_R128(ADD32((uint32_t)s1, 32))); h3 = a4::hi64(_v); std::memcpy(&v1, &_v, 8); } while (0);
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)sp, 64))));
    (at = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)16256 << 16))));
    { uint32_t bits = (uint32_t)at; std::memcpy(&f0, &bits, sizeof(bits)); }
    A4_W128(ADD32((uint32_t)sp, 64), a4::mk128(h3, v1));
    { float f = f0; uint32_t bits; std::memcpy(&bits, &f, sizeof(bits)); A1_W32B((uint32_t)sp, 76, bits); }
    vf8 = a4v::castsi128_ps(A4_R128(ADD32((uint32_t)v0, 0)));
    vf14 = a4v::castsi128_ps(A4_R128(ADD32((uint32_t)sp, 0)));
    vf15 = a4v::castsi128_ps(A4_R128(ADD32((uint32_t)sp, 16)));
    vf16 = a4v::castsi128_ps(A4_R128(ADD32((uint32_t)sp, 32)));
    vf17 = a4v::castsi128_ps(A4_R128(ADD32((uint32_t)sp, 48)));
    { a4::vec4 res = a4v::mul(vf14, a4v::bcast<0>(vf8)); vacc = a4v::dest<1, 1, 1, 1>(vacc, res); }
    { a4::vec4 mul_res = a4v::mul(vf15, a4v::bcast<1>(vf8)); a4::vec4 res = a4v::addK<1, 1>(vacc, mul_res); vacc = a4v::dest<1, 1, 1, 1>(vacc, res); }
    { a4::vec4 mul_res = a4v::mul(vf16, a4v::bcast<2>(vf8)); a4::vec4 res = a4v::addK<1, 1>(vacc, mul_res); vacc = a4v::dest<1, 1, 1, 1>(vacc, res); }
    { a4::vec4 mul_res = a4v::mul(vf17, a4v::bcast<3>(vf8)); a4::vec4 res = a4v::addK<1, 1>(vacc, mul_res); a4::q128 mask = a4v::set_epi32(-1, -1, -1, -1); vf10 = a4v::blendv_ps(vf10, res, a4v::castsi128_ps(mask)); }
    A4_W128(ADD32((uint32_t)v0, 0), a4v::castps_si128(vf10));
    { A3_CHARGE(); v0 = a1_sx64(iosgluGetCameraContext(ram, rt, f12, f13, f14, f15, f16, f17, f18, f19)); }
    { uint32_t bits = 0u; std::memcpy(&f2, &bits, sizeof(bits)); }
    { uint32_t bits = A1_R32B((uint32_t)v0, 320); float f; std::memcpy(&f, &bits, sizeof(f)); f1 = f; }
    { uint32_t bits = A1_R32B((uint32_t)sp, 64); float f; std::memcpy(&f, &bits, sizeof(f)); f0 = f; }
    f0 = a1f::sub(f0, f1);
    fcr31 = (a1f::lt(f0, f2)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
    if ((fcr31 & 0x800000u) != 0u) goto L_10981a0;
    { A3_CHARGE(); v0 = a1_sx64(iosgluGetCameraContext(ram, rt, f12, f13, f14, f15, f16, f17, f18, f19)); }
    (a0 = (uint64_t)(A1_R64B((uint32_t)s1, 24)));
    { uint32_t bits = A1_R32B((uint32_t)v0, 320); float f; std::memcpy(&f, &bits, sizeof(f)); f1 = f; }
    { uint32_t bits = A1_R32B((uint32_t)sp, 64); float f; std::memcpy(&f, &bits, sizeof(f)); f0 = f; }
    goto L_10982ec;
L_10981a0:;
    { A3_CHARGE(); v0 = a1_sx64(iosgluGetCameraContext(ram, rt, f12, f13, f14, f15, f16, f17, f18, f19)); }
    (a0 = (uint64_t)(A1_R64B((uint32_t)s1, 24)));
    { uint32_t bits = A1_R32B((uint32_t)v0, 320); float f; std::memcpy(&f, &bits, sizeof(f)); f1 = f; }
    { uint32_t bits = A1_R32B((uint32_t)sp, 64); float f; std::memcpy(&f, &bits, sizeof(f)); f0 = f; }
    goto L_10982c8;
L_10981b8:;
    do { const a4::q128 _v = (A4_R128(ADD32((uint32_t)s1, 32))); h3 = a4::hi64(_v); std::memcpy(&v1, &_v, 8); } while (0);
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)sp, 64))));
    (at = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)16256 << 16))));
    { uint32_t bits = (uint32_t)at; std::memcpy(&f0, &bits, sizeof(bits)); }
    A4_W128(ADD32((uint32_t)sp, 64), a4::mk128(h3, v1));
    { float f = f0; uint32_t bits; std::memcpy(&bits, &f, sizeof(bits)); A1_W32B((uint32_t)sp, 76, bits); }
    vf8 = a4v::castsi128_ps(A4_R128(ADD32((uint32_t)v0, 0)));
    vf14 = a4v::castsi128_ps(A4_R128(ADD32((uint32_t)sp, 0)));
    vf15 = a4v::castsi128_ps(A4_R128(ADD32((uint32_t)sp, 16)));
    vf16 = a4v::castsi128_ps(A4_R128(ADD32((uint32_t)sp, 32)));
    vf17 = a4v::castsi128_ps(A4_R128(ADD32((uint32_t)sp, 48)));
    { a4::vec4 res = a4v::mul(vf14, a4v::bcast<0>(vf8)); vacc = a4v::dest<1, 1, 1, 1>(vacc, res); }
    { a4::vec4 mul_res = a4v::mul(vf15, a4v::bcast<1>(vf8)); a4::vec4 res = a4v::addK<1, 1>(vacc, mul_res); vacc = a4v::dest<1, 1, 1, 1>(vacc, res); }
    { a4::vec4 mul_res = a4v::mul(vf16, a4v::bcast<2>(vf8)); a4::vec4 res = a4v::addK<1, 1>(vacc, mul_res); vacc = a4v::dest<1, 1, 1, 1>(vacc, res); }
    { a4::vec4 mul_res = a4v::mul(vf17, a4v::bcast<3>(vf8)); a4::vec4 res = a4v::addK<1, 1>(vacc, mul_res); a4::q128 mask = a4v::set_epi32(-1, -1, -1, -1); vf10 = a4v::blendv_ps(vf10, res, a4v::castsi128_ps(mask)); }
    A4_W128(ADD32((uint32_t)v0, 0), a4v::castps_si128(vf10));
    { A3_CHARGE(); v0 = a1_sx64(iosgluGetCameraContext(ram, rt, f12, f13, f14, f15, f16, f17, f18, f19)); }
    { uint32_t bits = 0u; std::memcpy(&f2, &bits, sizeof(bits)); }
    { uint32_t bits = A1_R32B((uint32_t)v0, 324); float f; std::memcpy(&f, &bits, sizeof(f)); f1 = f; }
    { uint32_t bits = A1_R32B((uint32_t)sp, 68); float f; std::memcpy(&f, &bits, sizeof(f)); f0 = f; }
    f0 = a1f::sub(f0, f1);
    fcr31 = (a1f::lt(f0, f2)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
    if ((fcr31 & 0x800000u) != 0u) goto L_1098238;
    { A3_CHARGE(); v0 = a1_sx64(iosgluGetCameraContext(ram, rt, f12, f13, f14, f15, f16, f17, f18, f19)); }
    (a0 = (uint64_t)(A1_R64B((uint32_t)s1, 24)));
    { uint32_t bits = A1_R32B((uint32_t)v0, 324); float f; std::memcpy(&f, &bits, sizeof(f)); f1 = f; }
    { uint32_t bits = A1_R32B((uint32_t)sp, 68); float f; std::memcpy(&f, &bits, sizeof(f)); f0 = f; }
    goto L_10982ec;
L_1098238:;
    { A3_CHARGE(); v0 = a1_sx64(iosgluGetCameraContext(ram, rt, f12, f13, f14, f15, f16, f17, f18, f19)); }
    (a0 = (uint64_t)(A1_R64B((uint32_t)s1, 24)));
    { uint32_t bits = A1_R32B((uint32_t)v0, 324); float f; std::memcpy(&f, &bits, sizeof(f)); f1 = f; }
    { uint32_t bits = A1_R32B((uint32_t)sp, 68); float f; std::memcpy(&f, &bits, sizeof(f)); f0 = f; }
    goto L_10982c8;
L_1098250:;
    do { const a4::q128 _v = (A4_R128(ADD32((uint32_t)s1, 32))); h3 = a4::hi64(_v); std::memcpy(&v1, &_v, 8); } while (0);
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)sp, 64))));
    (at = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)16256 << 16))));
    { uint32_t bits = (uint32_t)at; std::memcpy(&f0, &bits, sizeof(bits)); }
    A4_W128(ADD32((uint32_t)sp, 64), a4::mk128(h3, v1));
    { float f = f0; uint32_t bits; std::memcpy(&bits, &f, sizeof(bits)); A1_W32B((uint32_t)sp, 76, bits); }
    vf8 = a4v::castsi128_ps(A4_R128(ADD32((uint32_t)v0, 0)));
    vf14 = a4v::castsi128_ps(A4_R128(ADD32((uint32_t)sp, 0)));
    vf15 = a4v::castsi128_ps(A4_R128(ADD32((uint32_t)sp, 16)));
    vf16 = a4v::castsi128_ps(A4_R128(ADD32((uint32_t)sp, 32)));
    vf17 = a4v::castsi128_ps(A4_R128(ADD32((uint32_t)sp, 48)));
    { a4::vec4 res = a4v::mul(vf14, a4v::bcast<0>(vf8)); vacc = a4v::dest<1, 1, 1, 1>(vacc, res); }
    { a4::vec4 mul_res = a4v::mul(vf15, a4v::bcast<1>(vf8)); a4::vec4 res = a4v::addK<1, 1>(vacc, mul_res); vacc = a4v::dest<1, 1, 1, 1>(vacc, res); }
    { a4::vec4 mul_res = a4v::mul(vf16, a4v::bcast<2>(vf8)); a4::vec4 res = a4v::addK<1, 1>(vacc, mul_res); vacc = a4v::dest<1, 1, 1, 1>(vacc, res); }
    { a4::vec4 mul_res = a4v::mul(vf17, a4v::bcast<3>(vf8)); a4::vec4 res = a4v::addK<1, 1>(vacc, mul_res); a4::q128 mask = a4v::set_epi32(-1, -1, -1, -1); vf10 = a4v::blendv_ps(vf10, res, a4v::castsi128_ps(mask)); }
    A4_W128(ADD32((uint32_t)v0, 0), a4v::castps_si128(vf10));
    { A3_CHARGE(); v0 = a1_sx64(iosgluGetCameraContext(ram, rt, f12, f13, f14, f15, f16, f17, f18, f19)); }
    { uint32_t bits = 0u; std::memcpy(&f2, &bits, sizeof(bits)); }
    { uint32_t bits = A1_R32B((uint32_t)v0, 328); float f; std::memcpy(&f, &bits, sizeof(f)); f1 = f; }
    { uint32_t bits = A1_R32B((uint32_t)sp, 72); float f; std::memcpy(&f, &bits, sizeof(f)); f0 = f; }
    f0 = a1f::sub(f0, f1);
    fcr31 = (a1f::lt(f0, f2)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
    if ((fcr31 & 0x800000u) == 0u) goto L_10982d8;
    { A3_CHARGE(); v0 = a1_sx64(iosgluGetCameraContext(ram, rt, f12, f13, f14, f15, f16, f17, f18, f19)); }
    (a0 = (uint64_t)(A1_R64B((uint32_t)s1, 24)));
    { uint32_t bits = A1_R32B((uint32_t)v0, 328); float f; std::memcpy(&f, &bits, sizeof(f)); f1 = f; }
    { uint32_t bits = A1_R32B((uint32_t)sp, 72); float f; std::memcpy(&f, &bits, sizeof(f)); f0 = f; }
L_10982c8:;
    f0 = a1f::sub(f0, f1);
    f20 = (-(float)(f0));
    goto L_10982f0;
L_10982d8:;
    { A3_CHARGE(); v0 = a1_sx64(iosgluGetCameraContext(ram, rt, f12, f13, f14, f15, f16, f17, f18, f19)); }
    (a0 = (uint64_t)(A1_R64B((uint32_t)s1, 24)));
    { uint32_t bits = A1_R32B((uint32_t)v0, 328); float f; std::memcpy(&f, &bits, sizeof(f)); f1 = f; }
    { uint32_t bits = A1_R32B((uint32_t)sp, 72); float f; std::memcpy(&f, &bits, sizeof(f)); f0 = f; }
L_10982ec:;
    f20 = a1f::sub(f0, f1);
L_10982f0:;
    (v0 = (uint64_t)(a0 << 26));
    (v0 = (uint64_t)((int64_t)v0 >> (32 + 0)));
    (v0 = (uint64_t)(v0 & (uint64_t)(uint16_t)1));
    { const bool c = (int64_t)v0 == 0; (s0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)sp, 80)))); if (c) goto L_1098378; }
    (a1 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL));
    (a0 = (uint64_t)((uint64_t)RC_U64X(s0) + (uint64_t)0ULL));
    { (a2 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 16)))); A3_CHARGE(); v0 = a6x::memset(ram, rt, a0, (uint32_t)a1, (uint32_t)a2, h2, h4); }
    (at = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)49024 << 16))));
    { uint32_t bits = (uint32_t)at; std::memcpy(&f0, &bits, sizeof(bits)); }
    (a1 = (uint64_t)((uint64_t)s3 + (uint64_t)0ULL));
    (a0 = (uint64_t)((uint64_t)s0 + (uint64_t)0ULL));
    (a2 = (uint64_t)((uint64_t)s0 + (uint64_t)0ULL));
    { { float f = f0; uint32_t bits; std::memcpy(&bits, &f, sizeof(bits)); A1_W32B((uint32_t)sp, 88, bits); } A3_CHARGE(); iosApplyRotMatrix(ram, rt, (uint32_t)a0, (uint32_t)a1, (uint32_t)a2, vf0, vf8, vf10, vf14, vf15, vf16, vacc); }
    { A3_CHARGE(); v0 = a1_sx64(iosgluGetCameraContext(ram, rt, f12, f13, f14, f15, f16, f17, f18, f19)); }
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v0, 336))));
    vf5 = a4v::castsi128_ps(A4_R128(ADD32((uint32_t)s0, 0)));
    vf6 = a4v::castsi128_ps(A4_R128(ADD32((uint32_t)v1, 0)));
    { a4::vec4 res = a4v::addK<1, 1>(vf0, a4v::bcast<3>(vf0)); a4::q128 mask = a4v::set_epi32(0, 0, 0, -1); vf7 = a4v::blendv_ps(vf7, res, a4v::castsi128_ps(mask)); }
    { a4::vec4 res = a4v::mul(vf5, vf6); a4::q128 mask = a4v::set_epi32(0, -1, -1, -1); vf6 = a4v::blend(vf6, res, a4v::castsi128_ps(mask)); }
    { a4::vec4 res = a4v::addK<1, 1>(vf0, a4v::bcast<0>(vf6)); vacc = a4v::dest<1, 0, 0, 0>(vacc, res); }
    { a4::vec4 mul_res = a4v::mulK<1, 1>(vf7, a4v::bcast<1>(vf6)); a4::vec4 res = a4v::addK<1, 1>(vacc, mul_res); vacc = a4v::dest<1, 0, 0, 0>(vacc, res); }
    { a4::vec4 mul_res = a4v::mulK<1, 1>(vf7, a4v::bcast<2>(vf6)); a4::vec4 res = a4v::addK<1, 1>(vacc, mul_res); a4::q128 mask = a4v::set_epi32(0, 0, 0, -1); vf6 = a4v::blendv_ps(vf6, res, a4v::castsi128_ps(mask)); }
    do { const a4::q128 _v = (a4v::castps_si128(vf6)); h2 = a4::hi64(_v); std::memcpy(&v0, &_v, 8); } while (0);
    { uint32_t bits = (uint32_t)v0; std::memcpy(&f2, &bits, sizeof(bits)); }
    { uint32_t bits = 0u; std::memcpy(&f1, &bits, sizeof(bits)); }
    (at = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)49216 << 16))));
    { uint32_t bits = (uint32_t)at; std::memcpy(&f0, &bits, sizeof(bits)); }
    fcr31 = (a1f::lt(f2, f1)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
    if ((fcr31 & 0x800000u) != 0u) goto L_109842c;
L_1098378:;
    { uint32_t bits = A1_R32B((uint32_t)s2, 0); float f; std::memcpy(&f, &bits, sizeof(f)); f1 = f; }
    fcr31 = (a1f::lt(f20, f1)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
    if ((fcr31 & 0x800000u) == 0u) { { uint32_t bits = A1_R32B((uint32_t)s2, 8); float f; std::memcpy(&f, &bits, sizeof(f)); f4 = f; } goto L_1098390; }
    { uint32_t bits = A1_R32B((uint32_t)s2, 4); float f; std::memcpy(&f, &bits, sizeof(f)); f1 = f; }
    goto L_1098404;
L_1098390:;
    fcr31 = (a1f::lt(f20, f4)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
    if ((fcr31 & 0x800000u) == 0u) { { uint32_t bits = A1_R32B((uint32_t)s2, 16); float f; std::memcpy(&f, &bits, sizeof(f)); f1 = f; } goto L_10983b0; }
    { uint32_t bits = A1_R32B((uint32_t)s2, 4); float f; std::memcpy(&f, &bits, sizeof(f)); f3 = f; }
    f2 = a1f::sub(f20, f1);
    { uint32_t bits = A1_R32B((uint32_t)s2, 12); float f; std::memcpy(&f, &bits, sizeof(f)); f0 = f; }
    goto L_10983e8;
L_10983b0:;
    fcr31 = (a1f::lt(f20, f1)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
    if ((fcr31 & 0x800000u) == 0u) { { uint32_t bits = A1_R32B((uint32_t)s2, 24); float f; std::memcpy(&f, &bits, sizeof(f)); f4 = f; } goto L_10983d0; }
    { uint32_t bits = A1_R32B((uint32_t)s2, 12); float f; std::memcpy(&f, &bits, sizeof(f)); f3 = f; }
    f2 = a1f::sub(f20, f4);
    { uint32_t bits = A1_R32B((uint32_t)s2, 20); float f; std::memcpy(&f, &bits, sizeof(f)); f0 = f; }
    f1 = a1f::sub(f1, f4);
    goto L_10983ec;
L_10983d0:;
    fcr31 = (a1f::lt(f20, f4)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
    if ((fcr31 & 0x800000u) == 0u) { { uint32_t bits = A1_R32B((uint32_t)s2, 28); float f; std::memcpy(&f, &bits, sizeof(f)); f1 = f; } goto L_1098404; }
    { uint32_t bits = A1_R32B((uint32_t)s2, 20); float f; std::memcpy(&f, &bits, sizeof(f)); f3 = f; }
    f2 = a1f::sub(f20, f1);
    { uint32_t bits = A1_R32B((uint32_t)s2, 28); float f; std::memcpy(&f, &bits, sizeof(f)); f0 = f; }
L_10983e8:;
    f1 = a1f::sub(f4, f1);
L_10983ec:;
    f0 = a1f::sub(f0, f3);
    f0 = ((float)(f0) * (float)(f2));
    f0 = a1f::div(fcr31, f0, f1);
    f1 = a1f::add(f3, f0);
L_1098404:;
    (at = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)16256 << 16))));
    { uint32_t bits = (uint32_t)at; std::memcpy(&f2, &bits, sizeof(bits)); }
    (at = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)49216 << 16))));
    { uint32_t bits = (uint32_t)at; std::memcpy(&f0, &bits, sizeof(bits)); }
    f1 = a1f::sub(f2, f1);
    fcr31 = (a1f::eq(f1, f2)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
    { const bool c = (fcr31 & 0x800000u) != 0u; { float f = f1; uint32_t bits; std::memcpy(&bits, &f, sizeof(bits)); A1_W32B((uint32_t)s4, 584, bits); } if (c) goto L_1098428; }
L_1098424:;
    f0 = ((float)(f21));
L_1098428:;
L_109842c:;
    { uint32_t bits = A1_R32B((uint32_t)sp, 152); float f; std::memcpy(&f, &bits, sizeof(f)); f21 = f; }
    { uint32_t bits = A1_R32B((uint32_t)sp, 144); float f; std::memcpy(&f, &bits, sizeof(f)); f20 = f; }
    { (sp = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)sp, 160)))); if (xo) { xo[0] = a1_bits(f20); xo[1] = a1_bits(f21); xo[2] = a3; xo[3] = t0; xo[4] = t1; } io_vf5 = vf5; io_vf6 = vf6; io_vf7 = vf7; io_vf8 = vf8; io_vf10 = vf10; io_vf14 = vf14; io_vf15 = vf15; io_vf16 = vf16; io_vf17 = vf17; io_vacc = vacc; io_vq = vq; io_vstat = vstat; io_h2 = h2; io_h3 = h3; io_h6 = h6; io_h7 = h7; io_h8 = h8; io_h9 = h9; return a1::RetVF{(uint64_t)v0, f0}; }
    __builtin_unreachable();
}

// manager_sub_01098E20 (0x1098e20): typed by tools/typed_lift2.py (round A6; guest frame calls, extra outputs xo[0]=f20 xo[1]=f21 xo[2]=f22 xo[3]=f23 xo[4]=f24 xo[5]=f25 xo[6]=f26 xo[7]=f27; VU0 in vf0 vf14 vf15 vf16 vf17, out vf14 vf15 vf16 vf17 h2 h6 h7 h8 h9, relocated calls 36)
inline a1::RetVF manager_sub_01098E20(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint64_t v0, uint64_t a1, uint64_t sp, float f0, float f12, float f13, float f14, float f15, float f16, float f17, float f18, float f19, float f20, float f21, float f22, float f23, float f24, float f25, float f26, float f27, const a4::vec4 &io_vf0, a4::vec4 &io_vf14, a4::vec4 &io_vf15, a4::vec4 &io_vf16, a4::vec4 &io_vf17, uint64_t &io_h2, uint64_t &io_h6, uint64_t &io_h7, uint64_t &io_h8, uint64_t &io_h9, uint64_t *xo = nullptr)
{
    float f1 = 0.0f;
    float f2 = 0.0f;
    float f3 = 0.0f;
    uint32_t at = 0;
    uint32_t v1 = 0;
    uint64_t a0 = 0;
    uint64_t s0 = 0;
    uint64_t s1 = 0;
    uint64_t s2 = 0;
    uint64_t s3 = 0;
    uint64_t s4 = 0;
    uint64_t s5 = 0;
    uint64_t s6 = 0;
    uint32_t s7 = 0;
    uint64_t fp = 0;
    uint32_t fcr31 = 0;
    a4::vec4 vf0 = io_vf0;
    a4::vec4 vf14 = io_vf14;
    a4::vec4 vf15 = io_vf15;
    a4::vec4 vf16 = io_vf16;
    a4::vec4 vf17 = io_vf17;
    uint64_t h2 = io_h2;
    uint64_t h6 = io_h6;
    uint64_t h7 = io_h7;
    uint64_t h8 = io_h8;
    uint64_t h9 = io_h9;
    (sp = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)sp, 4294967072))));
    { float f = f27; uint32_t bits; std::memcpy(&bits, &f, sizeof(bits)); A1_W32B((uint32_t)sp, 216, bits); }
    { float f = f26; uint32_t bits; std::memcpy(&bits, &f, sizeof(bits)); A1_W32B((uint32_t)sp, 208, bits); }
    { float f = f25; uint32_t bits; std::memcpy(&bits, &f, sizeof(bits)); A1_W32B((uint32_t)sp, 200, bits); }
    { float f = f24; uint32_t bits; std::memcpy(&bits, &f, sizeof(bits)); A1_W32B((uint32_t)sp, 192, bits); }
    { float f = f23; uint32_t bits; std::memcpy(&bits, &f, sizeof(bits)); A1_W32B((uint32_t)sp, 184, bits); }
    { float f = f22; uint32_t bits; std::memcpy(&bits, &f, sizeof(bits)); A1_W32B((uint32_t)sp, 176, bits); }
    { float f = f21; uint32_t bits; std::memcpy(&bits, &f, sizeof(bits)); A1_W32B((uint32_t)sp, 168, bits); }
    { float f = f20; uint32_t bits; std::memcpy(&bits, &f, sizeof(bits)); A1_W32B((uint32_t)sp, 160, bits); }
    { f25 = ((float)(f12)); A3_CHARGE(); v0 = a1_sx64(iosgluGetCameraContext(ram, rt, f12, f13, f14, f15, f16, f17, f18, f19)); }
    { (s2 = (uint64_t)((uint64_t)v0 + (uint64_t)0ULL)); A3_CHARGE(); v0 = a1_sx64(iosgluGetCameraContext(ram, rt, f12, f13, f14, f15, f16, f17, f18, f19)); }
    { (s3 = (uint64_t)((uint64_t)v0 + (uint64_t)0ULL)); A3_CHARGE(); v0 = a1_sx64(getDrawScreenWidth(ram, rt)); }
    { uint32_t bits = (uint32_t)v0; std::memcpy(&f20, &bits, sizeof(bits)); }
    { int32_t tmp; std::memcpy(&tmp, &f20, sizeof(tmp)); f20 = ((float)(int32_t)(tmp)); }
    { (s5 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)s2, 544)))); A3_CHARGE(); v0 = a1_sx64(getDrawScreenHeight(ram, rt)); }
    v1 = 0x1000000u;
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 336))));
    A1_W32B((uint32_t)sp, 68, v1);
    v1 = 0x6C130000u;
    { uint32_t bits = (uint32_t)v0; std::memcpy(&f21, &bits, sizeof(bits)); }
    { int32_t tmp; std::memcpy(&tmp, &f21, sizeof(tmp)); f21 = ((float)(int32_t)(tmp)); }
    { A1_W32B((uint32_t)sp, 72, v1); A3_CHARGE(); v0 = a1_sx64(iosAllocPacketPtrPath1(ram, rt, (uint32_t)a0)); }
    (s1 = (uint64_t)((uint64_t)v0 + (uint64_t)0ULL));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)sp, 68))));
    (v1 = (uint32_t)((int32_t)((int32_t)A1_R32B((uint32_t)sp, 72))));
    (s0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)s1, 16))));
    (v0 = (uint64_t)(RC_U64X(v0) | (uint64_t)(uint16_t)257));
    (a0 = (uint64_t)((uint64_t)RC_U64X(s0) + (uint64_t)0ULL));
    A1_W32B((uint32_t)sp, 68, (uint32_t)v0);
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)24576 << 16))));
    (v0 = (uint64_t)(RC_U64X(v0) | (uint64_t)(uint16_t)20));
    v1 = v1 | 0x3C0u;
    (s4 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)s2, 608))));
    A1_W32B((uint32_t)sp, 72, v1);
    (fp = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)s2, 416))));
    A1_W64B((uint32_t)s1, 0, RC_U64X(v0));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)s2, 1184))));
    at = 0x3F000000u;
    { uint32_t bits = a7_opq(at); std::memcpy(&f24, &bits, sizeof(bits)); }
    (s6 = (uint64_t)((uint64_t)s1 + (uint64_t)0ULL));
    (v1 = (uint32_t)((int32_t)((int32_t)A1_R32B((uint32_t)sp, 68))));
    (s2 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)s2, 1312))));
    at = 0x3F800000u;
    { uint32_t bits = a7_opq(at); std::memcpy(&f26, &bits, sizeof(bits)); }
    A1_W32B((uint32_t)s1, 8, v1);
    A1_W32B((uint32_t)sp, 64, (uint32_t)v0);
    (v1 = (uint32_t)((int32_t)((int32_t)A1_R32B((uint32_t)sp, 72))));
    at = 0x45000000u;
    { uint32_t bits = a7_opq(at); std::memcpy(&f27, &bits, sizeof(bits)); }
    A1_W32B((uint32_t)s1, 12, v1);
    { (a1 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)sp, 64)))); A3_CHARGE(); iosCopyMatrix(ram, rt, (uint32_t)a0, (uint32_t)a1, h6, h7, h8, h9); }
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)s1, 80))));
    { (a1 = (uint64_t)((uint64_t)s5 + (uint64_t)0ULL)); A3_CHARGE(); iosCopyMatrix(ram, rt, (uint32_t)a0, (uint32_t)a1, h6, h7, h8, h9); }
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)s1, 144))));
    { (a1 = (uint64_t)((uint64_t)s4 + (uint64_t)0ULL)); A3_CHARGE(); iosCopyMatrix(ram, rt, (uint32_t)a0, (uint32_t)a1, h6, h7, h8, h9); }
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)s1, 208))));
    { (a1 = (uint64_t)((uint64_t)fp + (uint64_t)0ULL)); A3_CHARGE(); iosCopyMatrix(ram, rt, (uint32_t)a0, (uint32_t)a1, h6, h7, h8, h9); }
    { A3_CHARGE(); { const a1::RetVF a2_r = getFogFactor0(ram, rt); v0 = a2_r.v0; f0 = a2_r.f0; } }
    { float f = f0; uint32_t bits; std::memcpy(&bits, &f, sizeof(bits)); A1_W32B((uint32_t)s0, 256, bits); }
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)5120 << 16))));
    (v0 = (uint64_t)(RC_U64X(v0) | (uint64_t)(uint16_t)2));
    { A1_W32B((uint32_t)sp, 76, (uint32_t)v0); A3_CHARGE(); { const a1::RetVF a2_r = getFogFactor1(ram, rt); v0 = a2_r.v0; f0 = a2_r.f0; } }
    { float f = f0; uint32_t bits; std::memcpy(&bits, &f, sizeof(bits)); A1_W32B((uint32_t)s0, 268, bits); }
    f0 = a1f::mul(f21, f24);
    f1 = a1f::div(fcr31, f26, f20);
    { float f = f25; uint32_t bits; std::memcpy(&bits, &f, sizeof(bits)); A1_W32B((uint32_t)s0, 260, bits); }
    f20 = a1f::mul(f20, f24);
    { float f = f25; uint32_t bits; std::memcpy(&bits, &f, sizeof(bits)); A1_W32B((uint32_t)s0, 264, bits); }
    A1_W32B((uint32_t)s0, 316, 0u);
    (a1 = (uint64_t)((uint64_t)s6 + (uint64_t)0ULL));
    f0 = a1f::sub(f27, f0);
    do { const a4::q128 _v = (A4_R128(ADD32((uint32_t)s0, 256))); h2 = a4::hi64(_v); std::memcpy(&v0, &_v, 8); } while (0);
    (v1 = (uint32_t)((int32_t)((int32_t)A1_R32B((uint32_t)sp, 76))));
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 2))));
    f20 = a1f::sub(f27, f20);
    A4_W128(ADD32((uint32_t)s0, 272), a4::mk128(h2, v0));
    f21 = a1f::div(fcr31, f26, f21);
    A1_W32B((uint32_t)s0, 304, v1);
    { float f = f0; uint32_t bits; std::memcpy(&bits, &f, sizeof(bits)); A1_W32B((uint32_t)s0, 292, bits); }
    { float f = f1; uint32_t bits; std::memcpy(&bits, &f, sizeof(bits)); A1_W32B((uint32_t)s0, 296, bits); }
    { float f = f20; uint32_t bits; std::memcpy(&bits, &f, sizeof(bits)); A1_W32B((uint32_t)s0, 288, bits); }
    A1_W32B((uint32_t)s0, 308, 0u);
    A1_W32B((uint32_t)s0, 312, 0u);
    { { float f = f21; uint32_t bits; std::memcpy(&bits, &f, sizeof(bits)); A1_W32B((uint32_t)s0, 300, bits); } A3_CHARGE(); v0 = iosDlChainTail(ram, rt, a0, a1); }
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)281 << 16))));
    (s7 = (uint32_t)((int32_t)((int32_t)ADD32((uint32_t)v0, 4294950860))));
    (a1 = (uint64_t)((uint64_t)s6 + (uint64_t)0ULL));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B(s7, 0))));
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 3))));
    { A1_W32B((uint32_t)v0, 8, (uint32_t)s6); A3_CHARGE(); v0 = iosDlChainTail(ram, rt, a0, a1); }
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B(s7, 0))));
    (a1 = (uint64_t)((uint64_t)s6 + (uint64_t)0ULL));
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 5))));
    { A1_W32B((uint32_t)v0, 12, (uint32_t)s6); A3_CHARGE(); v0 = iosDlChainTail(ram, rt, a0, a1); }
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B(s7, 0))));
    (a1 = (uint64_t)((uint64_t)s6 + (uint64_t)0ULL));
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 6))));
    { A1_W32B((uint32_t)v0, 20, (uint32_t)s6); A3_CHARGE(); v0 = iosDlChainTail(ram, rt, a0, a1); }
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B(s7, 0))));
    (a1 = (uint64_t)((uint64_t)s6 + (uint64_t)0ULL));
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 10))));
    { A1_W32B((uint32_t)v0, 24, (uint32_t)s6); A3_CHARGE(); v0 = iosDlChainTail(ram, rt, a0, a1); }
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B(s7, 0))));
    (a1 = (uint64_t)((uint64_t)s6 + (uint64_t)0ULL));
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 8))));
    { A1_W32B((uint32_t)v0, 40, (uint32_t)s6); A3_CHARGE(); v0 = iosDlChainTail(ram, rt, a0, a1); }
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B(s7, 0))));
    (a1 = (uint64_t)((uint64_t)s6 + (uint64_t)0ULL));
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 18))));
    { A1_W32B((uint32_t)v0, 32, (uint32_t)s6); A3_CHARGE(); v0 = iosDlChainTail(ram, rt, a0, a1); }
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B(s7, 0))));
    (a1 = (uint64_t)((uint64_t)s6 + (uint64_t)0ULL));
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 9))));
    { A1_W32B((uint32_t)v0, 72, (uint32_t)s6); A3_CHARGE(); v0 = iosDlChainTail(ram, rt, a0, a1); }
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B(s7, 0))));
    (a1 = (uint64_t)((uint64_t)s6 + (uint64_t)0ULL));
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 17))));
    { A1_W32B((uint32_t)v0, 36, (uint32_t)s6); A3_CHARGE(); v0 = iosDlChainTail(ram, rt, a0, a1); }
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B(s7, 0))));
    (a1 = (uint64_t)((uint64_t)s6 + (uint64_t)0ULL));
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 15))));
    { A1_W32B((uint32_t)v0, 68, (uint32_t)s6); A3_CHARGE(); v0 = iosDlChainTail(ram, rt, a0, a1); }
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B(s7, 0))));
    (a1 = (uint64_t)((uint64_t)s6 + (uint64_t)0ULL));
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 14))));
    { A1_W32B((uint32_t)v0, 60, (uint32_t)s6); A3_CHARGE(); v0 = iosDlChainTail(ram, rt, a0, a1); }
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B(s7, 0))));
    { A1_W32B((uint32_t)v0, 56, (uint32_t)s6); A3_CHARGE(); v0 = a1_sx64(getDrawScreenWidth(ram, rt)); }
    { uint32_t bits = (uint32_t)v0; std::memcpy(&f20, &bits, sizeof(bits)); }
    { int32_t tmp; std::memcpy(&tmp, &f20, sizeof(tmp)); f20 = ((float)(int32_t)(tmp)); }
    { A3_CHARGE(); v0 = a1_sx64(getDrawScreenHeight(ram, rt)); }
    { uint32_t bits = (uint32_t)v0; std::memcpy(&f21, &bits, sizeof(bits)); }
    { int32_t tmp; std::memcpy(&tmp, &f21, sizeof(tmp)); f21 = ((float)(int32_t)(tmp)); }
    { (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 336)))); A3_CHARGE(); v0 = a1_sx64(iosAllocPacketPtrPath1(ram, rt, (uint32_t)a0)); }
    (a1 = (uint64_t)((uint64_t)s2 + (uint64_t)0ULL));
    (s0 = (uint64_t)((uint64_t)v0 + (uint64_t)0ULL));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)24576 << 16))));
    (v0 = (uint64_t)(RC_U64X(v0) | (uint64_t)(uint16_t)20));
    A1_W64B((uint32_t)s0, 0, RC_U64X(v0));
    (s1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)s0, 16))));
    (a0 = (uint64_t)((uint64_t)RC_U64X(s1) + (uint64_t)0ULL));
    (s6 = (uint64_t)((uint64_t)s0 + (uint64_t)0ULL));
    (v1 = (uint32_t)((int32_t)((int32_t)A1_R32B((uint32_t)sp, 68))));
    A1_W32B((uint32_t)s0, 8, v1);
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)sp, 72))));
    { A1_W32B((uint32_t)s0, 12, (uint32_t)v0); A3_CHARGE(); iosCopyMatrix(ram, rt, (uint32_t)a0, (uint32_t)a1, h6, h7, h8, h9); }
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)s0, 80))));
    { (a1 = (uint64_t)((uint64_t)s5 + (uint64_t)0ULL)); A3_CHARGE(); iosCopyMatrix(ram, rt, (uint32_t)a0, (uint32_t)a1, h6, h7, h8, h9); }
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)s0, 144))));
    { (a1 = (uint64_t)((uint64_t)s4 + (uint64_t)0ULL)); A3_CHARGE(); iosCopyMatrix(ram, rt, (uint32_t)a0, (uint32_t)a1, h6, h7, h8, h9); }
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)s0, 208))));
    { (a1 = (uint64_t)((uint64_t)fp + (uint64_t)0ULL)); A3_CHARGE(); iosCopyMatrix(ram, rt, (uint32_t)a0, (uint32_t)a1, h6, h7, h8, h9); }
    { A3_CHARGE(); { const a1::RetVF a2_r = getFogFactor0(ram, rt); v0 = a2_r.v0; f0 = a2_r.f0; } }
    { { float f = f0; uint32_t bits; std::memcpy(&bits, &f, sizeof(bits)); A1_W32B((uint32_t)s1, 256, bits); } A3_CHARGE(); { const a1::RetVF a2_r = getFogFactor1(ram, rt); v0 = a2_r.v0; f0 = a2_r.f0; } }
    { float f = f25; uint32_t bits; std::memcpy(&bits, &f, sizeof(bits)); A1_W32B((uint32_t)s1, 260, bits); }
    f1 = a1f::div(fcr31, f26, f20);
    { float f = f0; uint32_t bits; std::memcpy(&bits, &f, sizeof(bits)); A1_W32B((uint32_t)s1, 268, bits); }
    f20 = a1f::mul(f20, f24);
    { float f = f25; uint32_t bits; std::memcpy(&bits, &f, sizeof(bits)); A1_W32B((uint32_t)s1, 264, bits); }
    f0 = a1f::mul(f21, f24);
    A1_W32B((uint32_t)s1, 316, 0u);
    do { const a4::q128 _v = (A4_R128(ADD32((uint32_t)s1, 256))); h2 = a4::hi64(_v); std::memcpy(&v0, &_v, 8); } while (0);
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 12))));
    (v1 = (uint32_t)((int32_t)((int32_t)A1_R32B((uint32_t)sp, 76))));
    (a1 = (uint64_t)((uint64_t)s6 + (uint64_t)0ULL));
    f20 = a1f::sub(f27, f20);
    A4_W128(ADD32((uint32_t)s1, 272), a4::mk128(h2, v0));
    f21 = a1f::div(fcr31, f26, f21);
    A1_W32B((uint32_t)s1, 304, v1);
    f0 = a1f::sub(f27, f0);
    A1_W32B((uint32_t)s1, 308, 0u);
    { float f = f1; uint32_t bits; std::memcpy(&bits, &f, sizeof(bits)); A1_W32B((uint32_t)s1, 296, bits); }
    { float f = f20; uint32_t bits; std::memcpy(&bits, &f, sizeof(bits)); A1_W32B((uint32_t)s1, 288, bits); }
    A1_W32B((uint32_t)s1, 312, 0u);
    { float f = f0; uint32_t bits; std::memcpy(&bits, &f, sizeof(bits)); A1_W32B((uint32_t)s1, 292, bits); }
    { { float f = f21; uint32_t bits; std::memcpy(&bits, &f, sizeof(bits)); A1_W32B((uint32_t)s1, 300, bits); } A3_CHARGE(); v0 = iosDlChainTail(ram, rt, a0, a1); }
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B(s7, 0))));
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 13))));
    (a1 = (uint64_t)((uint64_t)s6 + (uint64_t)0ULL));
    { A1_W32B((uint32_t)v0, 48, (uint32_t)s6); A3_CHARGE(); v0 = iosDlChainTail(ram, rt, a0, a1); }
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B(s7, 0))));
    A1_W32B((uint32_t)v0, 52, (uint32_t)s6);
    { uint32_t bits = A1_R32B((uint32_t)s3, 0); float f; std::memcpy(&f, &bits, sizeof(f)); f1 = f; }
    { int32_t tmp; std::memcpy(&tmp, &f1, sizeof(tmp)); f1 = ((float)(int32_t)(tmp)); }
    { uint32_t bits = A1_R32B((uint32_t)s3, 12); float f; std::memcpy(&f, &bits, sizeof(f)); f2 = f; }
    { uint32_t bits = A1_R32B((uint32_t)s3, 4); float f; std::memcpy(&f, &bits, sizeof(f)); f0 = f; }
    { int32_t tmp; std::memcpy(&tmp, &f0, sizeof(tmp)); f0 = ((float)(int32_t)(tmp)); }
    f1 = a1f::mul(f1, f24);
    f0 = a1f::mul(f0, f24);
    f22 = a1f::div(fcr31, f2, f1);
    f23 = a1f::div(fcr31, f2, f0);
    { A3_CHARGE(); { const a1::RetVF a2_r = getZFogNear(ram, rt); v0 = a2_r.v0; f0 = a2_r.f0; } }
    { f20 = ((float)(f0)); A3_CHARGE(); v0 = a1_sx64(getZFogScissorMargin(ram, rt)); }
    { uint32_t bits = A1_R32B((uint32_t)v0, 0); float f; std::memcpy(&f, &bits, sizeof(f)); f0 = f; }
    at = 0x41200000u;
    { uint32_t bits = a7_opq(at); std::memcpy(&f1, &bits, sizeof(bits)); }
    f20 = a1f::sub(f20, f0);
    fcr31 = (a1f::lt(f20, f1)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
    { const bool c = (fcr31 & 0x800000u) == 0u; { uint32_t bits = A1_R32B((uint32_t)s3, 48); float f; std::memcpy(&f, &bits, sizeof(f)); f21 = f; } if (c) goto L_1099228; }
    f20 = ((float)(f1));
L_1099228:;
    { (a0 = (uint64_t)((uint64_t)sp + (uint64_t)0ULL)); A3_CHARGE(); iosUnitMatrix(ram, rt, (uint32_t)a0, vf0, vf14, vf15, vf16, vf17); }
    f1 = a1f::add(f21, f20);
    f0 = a1f::mul(f21, f20);
    at = 0xC0000000u;
    { uint32_t bits = a7_opq(at); std::memcpy(&f3, &bits, sizeof(bits)); }
    f2 = a1f::sub(f21, f20);
    { float f = f22; uint32_t bits; std::memcpy(&bits, &f, sizeof(bits)); A1_W32B((uint32_t)sp, 0, bits); }
    { float f = f23; uint32_t bits; std::memcpy(&bits, &f, sizeof(bits)); A1_W32B((uint32_t)sp, 20, bits); }
    { float f = f26; uint32_t bits; std::memcpy(&bits, &f, sizeof(bits)); A1_W32B((uint32_t)sp, 44, bits); }
    f0 = ((float)(f0) * (float)(f3));
    A1_W32B((uint32_t)sp, 60, 0u);
    f1 = a1f::div(fcr31, f1, f2);
    f0 = a1f::div(fcr31, f0, f2);
    { float f = f1; uint32_t bits; std::memcpy(&bits, &f, sizeof(bits)); A1_W32B((uint32_t)sp, 40, bits); }
    { { float f = f0; uint32_t bits; std::memcpy(&bits, &f, sizeof(bits)); A1_W32B((uint32_t)sp, 56, bits); } A3_CHARGE(); v0 = a1_sx64(getDrawScreenWidth(ram, rt)); }
    { uint32_t bits = (uint32_t)v0; std::memcpy(&f21, &bits, sizeof(bits)); }
    { int32_t tmp; std::memcpy(&tmp, &f21, sizeof(tmp)); f21 = ((float)(int32_t)(tmp)); }
    { A3_CHARGE(); v0 = a1_sx64(getDrawScreenHeight(ram, rt)); }
    { uint32_t bits = (uint32_t)v0; std::memcpy(&f20, &bits, sizeof(bits)); }
    { int32_t tmp; std::memcpy(&tmp, &f20, sizeof(tmp)); f20 = ((float)(int32_t)(tmp)); }
    { (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 336)))); A3_CHARGE(); v0 = a1_sx64(iosAllocPacketPtrPath1(ram, rt, (uint32_t)a0)); }
    (s0 = (uint64_t)((uint64_t)v0 + (uint64_t)0ULL));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)24576 << 16))));
    (v0 = (uint64_t)(RC_U64X(v0) | (uint64_t)(uint16_t)20));
    (a1 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)sp, 64))));
    (s1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)s0, 16))));
    A1_W64B((uint32_t)s0, 0, RC_U64X(v0));
    (a0 = (uint64_t)((uint64_t)RC_U64X(s1) + (uint64_t)0ULL));
    (s6 = (uint64_t)((uint64_t)s0 + (uint64_t)0ULL));
    (v1 = (uint32_t)((int32_t)((int32_t)A1_R32B((uint32_t)sp, 68))));
    A1_W32B((uint32_t)s0, 8, v1);
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)sp, 72))));
    { A1_W32B((uint32_t)s0, 12, (uint32_t)v0); A3_CHARGE(); iosCopyMatrix(ram, rt, (uint32_t)a0, (uint32_t)a1, h6, h7, h8, h9); }
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)s0, 80))));
    { (a1 = (uint64_t)((uint64_t)sp + (uint64_t)0ULL)); A3_CHARGE(); iosCopyMatrix(ram, rt, (uint32_t)a0, (uint32_t)a1, h6, h7, h8, h9); }
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)s0, 144))));
    { (a1 = (uint64_t)((uint64_t)sp + (uint64_t)0ULL)); A3_CHARGE(); iosCopyMatrix(ram, rt, (uint32_t)a0, (uint32_t)a1, h6, h7, h8, h9); }
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)s0, 208))));
    { (a1 = (uint64_t)((uint64_t)fp + (uint64_t)0ULL)); A3_CHARGE(); iosCopyMatrix(ram, rt, (uint32_t)a0, (uint32_t)a1, h6, h7, h8, h9); }
    { A3_CHARGE(); { const a1::RetVF a2_r = getFogFactor0(ram, rt); v0 = a2_r.v0; f0 = a2_r.f0; } }
    { { float f = f0; uint32_t bits; std::memcpy(&bits, &f, sizeof(bits)); A1_W32B((uint32_t)s1, 256, bits); } A3_CHARGE(); { const a1::RetVF a2_r = getFogFactor1(ram, rt); v0 = a2_r.v0; f0 = a2_r.f0; } }
    { float f = f25; uint32_t bits; std::memcpy(&bits, &f, sizeof(bits)); A1_W32B((uint32_t)s1, 260, bits); }
    f1 = a1f::div(fcr31, f26, f20);
    { float f = f0; uint32_t bits; std::memcpy(&bits, &f, sizeof(bits)); A1_W32B((uint32_t)s1, 268, bits); }
    f20 = a1f::mul(f20, f24);
    { float f = f25; uint32_t bits; std::memcpy(&bits, &f, sizeof(bits)); A1_W32B((uint32_t)s1, 264, bits); }
    f0 = a1f::mul(f21, f24);
    (a1 = (uint64_t)((uint64_t)s6 + (uint64_t)0ULL));
    (v1 = (uint32_t)((int32_t)((int32_t)A1_R32B((uint32_t)sp, 76))));
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 1))));
    do { const a4::q128 _v = (A4_R128(ADD32((uint32_t)s1, 256))); h2 = a4::hi64(_v); std::memcpy(&v0, &_v, 8); } while (0);
    f20 = a1f::sub(f27, f20);
    A1_W32B((uint32_t)s1, 304, v1);
    f21 = a1f::div(fcr31, f26, f21);
    A4_W128(ADD32((uint32_t)s1, 272), a4::mk128(h2, v0));
    f0 = a1f::sub(f27, f0);
    A1_W32B((uint32_t)s1, 316, 0u);
    { float f = f1; uint32_t bits; std::memcpy(&bits, &f, sizeof(bits)); A1_W32B((uint32_t)s1, 300, bits); }
    { float f = f20; uint32_t bits; std::memcpy(&bits, &f, sizeof(bits)); A1_W32B((uint32_t)s1, 292, bits); }
    A1_W32B((uint32_t)s1, 308, 0u);
    { float f = f0; uint32_t bits; std::memcpy(&bits, &f, sizeof(bits)); A1_W32B((uint32_t)s1, 288, bits); }
    A1_W32B((uint32_t)s1, 312, 0u);
    { { float f = f21; uint32_t bits; std::memcpy(&bits, &f, sizeof(bits)); A1_W32B((uint32_t)s1, 296, bits); } A3_CHARGE(); v0 = iosDlChainTail(ram, rt, a0, a1); }
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B(s7, 0))));
    A1_W32B((uint32_t)v0, 4, (uint32_t)s6);
    { uint32_t bits = A1_R32B((uint32_t)sp, 216); float f; std::memcpy(&f, &bits, sizeof(f)); f27 = f; }
    { uint32_t bits = A1_R32B((uint32_t)sp, 208); float f; std::memcpy(&f, &bits, sizeof(f)); f26 = f; }
    { uint32_t bits = A1_R32B((uint32_t)sp, 200); float f; std::memcpy(&f, &bits, sizeof(f)); f25 = f; }
    { uint32_t bits = A1_R32B((uint32_t)sp, 192); float f; std::memcpy(&f, &bits, sizeof(f)); f24 = f; }
    { uint32_t bits = A1_R32B((uint32_t)sp, 184); float f; std::memcpy(&f, &bits, sizeof(f)); f23 = f; }
    { uint32_t bits = A1_R32B((uint32_t)sp, 176); float f; std::memcpy(&f, &bits, sizeof(f)); f22 = f; }
    { uint32_t bits = A1_R32B((uint32_t)sp, 168); float f; std::memcpy(&f, &bits, sizeof(f)); f21 = f; }
    { uint32_t bits = A1_R32B((uint32_t)sp, 160); float f; std::memcpy(&f, &bits, sizeof(f)); f20 = f; }
    { (sp = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)sp, 224)))); if (xo) { xo[0] = a1_bits(f20); xo[1] = a1_bits(f21); xo[2] = a1_bits(f22); xo[3] = a1_bits(f23); xo[4] = a1_bits(f24); xo[5] = a1_bits(f25); xo[6] = a1_bits(f26); xo[7] = a1_bits(f27); } io_vf14 = vf14; io_vf15 = vf15; io_vf16 = vf16; io_vf17 = vf17; io_h2 = h2; io_h6 = h6; io_h7 = h7; io_h8 = h8; io_h9 = h9; return a1::RetVF{(uint64_t)v0, f0}; }
    __builtin_unreachable();
}

// ptclmanFrameInit (0x109a410): typed by tools/typed_lift2.py (round A6; frame calls, extra outputs xo[0]=a0 xo[1]=a1 xo[2]=a2 xo[3]=a3 xo[4]=t0 xo[5]=t1; VU0 in -, out h2 h6 h7 h8 h9, relocated calls 4, frame 32 bytes, 0 slot accesses)
inline uint64_t ptclmanFrameInit(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint64_t v0, float f12, float f13, float f14, float f15, float f16, float f17, float f18, float f19, uint64_t &io_h2, uint64_t &io_h6, uint64_t &io_h7, uint64_t &io_h8, uint64_t &io_h9, uint64_t *xo = nullptr)
{
    uint32_t at = 0;
    uint32_t v1 = 0;
    uint64_t a0 = 0;
    uint32_t a1 = 0;
    uint64_t a2 = 0;
    uint64_t a3 = 0;
    uint64_t t0 = 0;
    uint64_t t1 = 0;
    uint32_t s0 = 0;
    uint64_t s1 = 0;
    uint64_t h2 = io_h2;
    uint64_t h6 = io_h6;
    uint64_t h7 = io_h7;
    uint64_t h8 = io_h8;
    uint64_t h9 = io_h9;
    s0 = 0x1190000u;
    { s0 = s0 - 0x400Cu; A3_CHARGE(); v0 = a1_sx64(iosgluGetCameraContext(ram, rt, f12, f13, f14, f15, f16, f17, f18, f19)); }
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)281 << 16))));
    (s1 = (uint64_t)((uint64_t)v0 + (uint64_t)0ULL));
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)a0, 4294950896))));
    (a1 = (uint32_t)((int32_t)((int32_t)ADD32((uint32_t)s1, 416))));
    (v1 = (uint32_t)((int32_t)((int32_t)A1_R32B((uint32_t)a0, 0))));
    v1 = v1 ^ 1u;
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)SLL32(v1, 3))));
    A1_W32B((uint32_t)a0, 0, v1);
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v0, v1))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)SLL32((uint32_t)v0, 5))));
    at = 0x1210000u;
    at = at + 0x5C00u;
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(at, (uint32_t)v0))));
    A1_W32B(s0, 0, (uint32_t)v0);
    { (a0 = (uint64_t)((uint64_t)RC_U64X(v0) + (uint64_t)0ULL)); A3_CHARGE(); iosCopyMatrix_x8(ram, rt, (uint32_t)a0, a1, h6, h7, h8, h9); }
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B(s0, 0))));
    (a1 = (uint32_t)((int32_t)((int32_t)ADD32((uint32_t)s1, 1184))));
    { (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)a0, 64)))); A3_CHARGE(); iosCopyMatrix_x8(ram, rt, (uint32_t)a0, a1, h6, h7, h8, h9); }
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B(s0, 0))));
    (a1 = (uint32_t)((int32_t)((int32_t)ADD32((uint32_t)s1, 1632))));
    { (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)a0, 128)))); A3_CHARGE(); iosCopyMatrix_x8(ram, rt, (uint32_t)a0, a1, h6, h7, h8, h9); }
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B(s0, 0))));
    (a1 = (uint32_t)((int32_t)((int32_t)ADD32((uint32_t)s1, 1440))));
    { (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)a0, 192)))); A3_CHARGE(); uint64_t a7c[4] = {0u, 0u, 0u, 0u}; iosCopyMatrix_x8(ram, rt, (uint32_t)a0, a1, h6, h7, h8, h9, a7c); a2 = a7c[0]; a3 = a7c[1]; t0 = a7c[2]; t1 = a7c[3]; }
    v1 = A1_R32(s0);
    do { const a4::q128 _v = (A4_R128(ADD32((uint32_t)s1, 320))); h2 = a4::hi64(_v); std::memcpy(&v0, &_v, 8); } while (0);
    A4_W128(ADD32(v1, 256), a4::mk128(h2, v0));
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B(s0, 0))));
    do { const a4::q128 _v = (A4_R128(ADD32((uint32_t)s1, 336))); h2 = a4::hi64(_v); std::memcpy(&v0, &_v, 8); } while (0);
    A4_W128(ADD32((uint32_t)a0, 272), a4::mk128(h2, v0));
    { if (xo) { xo[0] = a0; xo[1] = a1_sx64(a1); xo[2] = a2; xo[3] = a3; xo[4] = t0; xo[5] = t1; } io_h2 = h2; io_h6 = h6; io_h7 = h7; io_h8 = h8; io_h9 = h9; return v0; }
    __builtin_unreachable();
}

// manager_sub_010B2318 (0x10b2318): typed by tools/typed_lift2.py (round A6; guest frame calls, extra outputs xo[0]=f12 xo[1]=f13 xo[2]=f14 xo[3]=f15 xo[4]=f20 xo[5]=f21 xo[6]=a2 xo[7]=a3 xo[8]=t0 xo[9]=t1; VU0 in vf1 vf2 vf3 vf4 vf10 vacc h4, out vf8 vf10 vf14 vf15 vf16 vf17 vacc h2 h6 h7 h8 h9, relocated calls 3)
inline a1::RetVF manager_sub_010B2318(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint64_t a0, uint64_t a1, uint64_t sp, float f12, float f13, float f14, float f15, float f16, float f17, float f18, float f19, float f20, float f21, const a4::vec4 &io_vf1, const a4::vec4 &io_vf2, const a4::vec4 &io_vf3, const a4::vec4 &io_vf4, a4::vec4 &io_vf8, a4::vec4 &io_vf10, a4::vec4 &io_vf14, a4::vec4 &io_vf15, a4::vec4 &io_vf16, a4::vec4 &io_vf17, a4::vec4 &io_vacc, uint64_t &io_h2, uint64_t io_h4, uint64_t &io_h6, uint64_t &io_h7, uint64_t &io_h8, uint64_t &io_h9, uint64_t *xo = nullptr)
{
    float f0 = 0.0f;
    uint32_t at = 0;
    uint64_t v0 = 0;
    uint64_t a2 = 0;
    uint64_t a3 = 0;
    uint64_t t0 = 0;
    uint64_t t1 = 0;
    uint64_t s0 = 0;
    uint64_t s1 = 0;
    a4::vec4 vf1 = io_vf1;
    a4::vec4 vf2 = io_vf2;
    a4::vec4 vf3 = io_vf3;
    a4::vec4 vf4 = io_vf4;
    a4::vec4 vf8 = io_vf8;
    a4::vec4 vf10 = io_vf10;
    a4::vec4 vf14 = io_vf14;
    a4::vec4 vf15 = io_vf15;
    a4::vec4 vf16 = io_vf16;
    a4::vec4 vf17 = io_vf17;
    a4::vec4 vacc = io_vacc;
    uint64_t h2 = io_h2;
    uint64_t h4 = io_h4;
    uint64_t h6 = io_h6;
    uint64_t h7 = io_h7;
    uint64_t h8 = io_h8;
    uint64_t h9 = io_h9;
    (sp = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)sp, 4294967168))));
    (a2 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 16))));
    (s0 = (uint64_t)((uint64_t)a0 + (uint64_t)0ULL));
    (s1 = (uint64_t)((uint64_t)a1 + (uint64_t)0ULL));
    (a0 = (uint64_t)((uint64_t)RC_U64X(sp) + (uint64_t)0ULL));
    (a1 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL));
    { float f = f21; uint32_t bits; std::memcpy(&bits, &f, sizeof(bits)); A1_W32B((uint32_t)sp, 112, bits); }
    f21 = ((float)(f12));
    { float f = f20; uint32_t bits; std::memcpy(&bits, &f, sizeof(bits)); A1_W32B((uint32_t)sp, 104, bits); }
    { f20 = ((float)(f13)); A3_CHARGE(); v0 = a6x::memset(ram, rt, a0, (uint32_t)a1, (uint32_t)a2, h2, h4); }
    (at = (uint32_t)((int32_t)((int32_t)((uint32_t)16256 << 16))));
    { uint32_t bits = a7_opq(at); std::memcpy(&f12, &bits, sizeof(bits)); }
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 2))));
    (at = (uint32_t)((int32_t)((int32_t)((uint32_t)15820 << 16))));
    at = at | 0xCCCCu;
    { uint32_t bits = a7_opq(at); std::memcpy(&f15, &bits, sizeof(bits)); }
    { float f = f20; uint32_t bits; std::memcpy(&bits, &f, sizeof(bits)); A1_W32B((uint32_t)sp, 8, bits); }
    f13 = ((float)(f12));
    f14 = ((float)(f12));
    { float f = f21; uint32_t bits; std::memcpy(&bits, &f, sizeof(bits)); A1_W32B((uint32_t)sp, 0, bits); }
    { const bool c = (int64_t)s0 != (int64_t)v0; { float f = f21; uint32_t bits; std::memcpy(&bits, &f, sizeof(bits)); A1_W32B((uint32_t)sp, 4, bits); } if (c) goto L_10b2390; }
    { A3_CHARGE(); uint64_t a7c[4] = {0u, 0u, 0u, 0u}; { const a1::RetVF a2_r = glColor4f_x8(ram, rt, f12, f13, f14, f15, a7c); v0 = a2_r.v0; f0 = a2_r.f0; } f12 = a1_flt((uint32_t)a7c[0]); f13 = a1_flt((uint32_t)a7c[1]); f14 = a1_flt((uint32_t)a7c[2]); f15 = a1_flt((uint32_t)a7c[3]); }
    (s0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)sp, 16))));
    goto L_10b23e4;
L_10b2390:;
    if ((int64_t)s1 == 0) goto L_10b23c8;
    { uint32_t bits = 0u; std::memcpy(&f12, &bits, sizeof(bits)); }
    (at = (uint32_t)((int32_t)((int32_t)((uint32_t)15948 << 16))));
    at = at | 0xCCCCu;
    { uint32_t bits = a7_opq(at); std::memcpy(&f13, &bits, sizeof(bits)); }
    (at = (uint32_t)((int32_t)((int32_t)((uint32_t)16128 << 16))));
    { uint32_t bits = a7_opq(at); std::memcpy(&f14, &bits, sizeof(bits)); }
    (at = (uint32_t)((int32_t)((int32_t)((uint32_t)16256 << 16))));
    { uint32_t bits = a7_opq(at); std::memcpy(&f15, &bits, sizeof(bits)); }
    { A3_CHARGE(); uint64_t a7c[4] = {0u, 0u, 0u, 0u}; { const a1::RetVF a2_r = glColor4f_x8(ram, rt, f12, f13, f14, f15, a7c); v0 = a2_r.v0; f0 = a2_r.f0; } f12 = a1_flt((uint32_t)a7c[0]); f13 = a1_flt((uint32_t)a7c[1]); f14 = a1_flt((uint32_t)a7c[2]); f15 = a1_flt((uint32_t)a7c[3]); }
    (s0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)sp, 16))));
    goto L_10b23e4;
L_10b23c8:;
    (at = (uint32_t)((int32_t)((int32_t)((uint32_t)16256 << 16))));
    { uint32_t bits = a7_opq(at); std::memcpy(&f12, &bits, sizeof(bits)); }
    f13 = ((float)(f12));
    f14 = ((float)(f12));
    { f15 = ((float)(f12)); A3_CHARGE(); uint64_t a7c[4] = {0u, 0u, 0u, 0u}; { const a1::RetVF a2_r = glColor4f_x8(ram, rt, f12, f13, f14, f15, a7c); v0 = a2_r.v0; f0 = a2_r.f0; } f12 = a1_flt((uint32_t)a7c[0]); f13 = a1_flt((uint32_t)a7c[1]); f14 = a1_flt((uint32_t)a7c[2]); f15 = a1_flt((uint32_t)a7c[3]); }
    (s0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)sp, 16))));
L_10b23e4:;
    { (a0 = (uint64_t)((uint64_t)s0 + (uint64_t)0ULL)); A3_CHARGE(); iosGetCurrentMatrix(ram, rt, (uint32_t)a0, vf1, vf2, vf3, vf4); }
    (a0 = (uint64_t)((uint64_t)s0 + (uint64_t)0ULL));
    (a1 = (uint64_t)((uint64_t)s0 + (uint64_t)0ULL));
    { (a2 = (uint64_t)((uint64_t)sp + (uint64_t)0ULL)); A3_CHARGE(); iosScaleMatrixV(ram, rt, (uint32_t)a0, (uint32_t)a1, (uint32_t)a2, vf8, vf14, vf15, vf16, vf17); }
    { (a0 = (uint64_t)((uint64_t)s0 + (uint64_t)0ULL)); A3_CHARGE(); uint64_t a7c[7] = {0u, 0u, 0u, 0u, 0u, 0u, 0u}; glLoadMatrixf(ram, rt, a0, f12, f13, f14, f15, f16, f17, f18, f19, h6, h7, h8, h9, a7c); a3 = a7c[4]; t0 = a7c[5]; t1 = a7c[6]; }
    { A3_CHARGE(); v0 = a1_sx64(iosgluGetCameraContext(ram, rt, f12, f13, f14, f15, f16, f17, f18, f19)); }
    (a0 = (uint64_t)((uint64_t)v0 + (uint64_t)0ULL));
    (a1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)sp, 64))));
    { f12 = ((float)(f21)); A3_CHARGE(); uint64_t a7c[2] = {0u, 0u}; { const a1::RetVF a2_r = GetScreenRadius_x8(ram, rt, (uint32_t)a0, (uint32_t)a1, (uint32_t)sp, f12, vf8, vf10, vf14, vf15, vf16, vf17, vacc, a7c); v0 = a2_r.v0; f0 = a2_r.f0; } a2 = a7c[1]; }
    { uint32_t bits = A1_R32B((uint32_t)sp, 112); float f; std::memcpy(&f, &bits, sizeof(f)); f21 = f; }
    { uint32_t bits = A1_R32B((uint32_t)sp, 104); float f; std::memcpy(&f, &bits, sizeof(f)); f20 = f; }
    { (sp = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)sp, 128)))); if (xo) { xo[0] = a1_bits(f12); xo[1] = a1_bits(f13); xo[2] = a1_bits(f14); xo[3] = a1_bits(f15); xo[4] = a1_bits(f20); xo[5] = a1_bits(f21); xo[6] = a2; xo[7] = a3; xo[8] = t0; xo[9] = t1; } io_vf8 = vf8; io_vf10 = vf10; io_vf14 = vf14; io_vf15 = vf15; io_vf16 = vf16; io_vf17 = vf17; io_vacc = vacc; io_h2 = h2; io_h6 = h6; io_h7 = h7; io_h8 = h8; io_h9 = h9; return a1::RetVF{(uint64_t)v0, f0}; }
    __builtin_unreachable();
}

// UpdateLayoutClassPastSimpleGeometry (0x10f24d8): typed by tools/typed_lift2.py (round A6; frame calls; VU0 in -, out h2 h6 h7 h8 h9, frame 16 bytes, 0 slot accesses)
inline void UpdateLayoutClassPastSimpleGeometry(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint64_t a0, float f12, float f13, float f14, float f15, float f16, float f17, float f18, float f19, uint64_t &io_h2, uint64_t &io_h6, uint64_t &io_h7, uint64_t &io_h8, uint64_t &io_h9)
{
    uint64_t v0 = 0;
    uint32_t v1 = 0;
    uint64_t a1 = 0;
    uint32_t a2 = 0;
    uint64_t h2 = io_h2;
    uint64_t h6 = io_h6;
    uint64_t h7 = io_h7;
    uint64_t h8 = io_h8;
    uint64_t h9 = io_h9;
    (a1 = (uint64_t)((uint64_t)a0 + (uint64_t)0ULL));
    (v1 = (uint32_t)((int32_t)((int32_t)A1_R32B((uint32_t)a1, 308))));
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)a1, 312))));
    do { const a4::q128 _v = (A4_R128(ADD32(v1, 0))); h2 = a4::hi64(_v); std::memcpy(&v0, &_v, 8); } while (0);
    A4_W128(ADD32((uint32_t)a0, 0), a4::mk128(h2, v0));
    (v1 = (uint32_t)((uint8_t)A1_R8B((uint32_t)a1, 444)));
    do { const a4::q128 _v = (A4_R128(ADD32((uint32_t)a1, 160))); h2 = a4::hi64(_v); std::memcpy(&v0, &_v, 8); } while (0);
    v1 = v1 & 0x20u;
    { const bool c = v1 == 0; A4_W128(ADD32((uint32_t)a1, 176), a4::mk128(h2, v0)); if (c) goto L_10f2520; }
    (a2 = (uint32_t)((int32_t)((int16_t)A1_R16B((uint32_t)a1, 436))));
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)a1, 360))));
    (a1 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)a1, 356))));
    { CopyMultiMatrix(ram, rt, a0, a1, a1_sx64(a2), f12, f13, f14, f15, f16, f17, f18, f19, h6, h7, h8, h9); io_h2 = h2; io_h6 = h6; io_h7 = h7; io_h8 = h8; io_h9 = h9; return; }
L_10f2520:;
    { io_h2 = h2; io_h6 = h6; io_h7 = h7; io_h8 = h8; io_h9 = h9; return; }
    __builtin_unreachable();
}

// UpdateLayoutClassPastGeometry (0x10f2b50): typed by tools/typed_lift2.py (round A6; frame calls, extra outputs xo[0]=v0 xo[1]=a1 xo[2]=a2 xo[3]=a3 xo[4]=t0 xo[5]=t1; VU0 in -, out h2 h6 h7 h8 h9, frame 16 bytes, 0 slot accesses)
inline void UpdateLayoutClassPastGeometry(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint64_t a0, uint64_t a3, uint64_t t0, uint64_t t1, float f12, float f13, float f14, float f15, float f16, float f17, float f18, float f19, uint64_t &io_h2, uint64_t &io_h6, uint64_t &io_h7, uint64_t &io_h8, uint64_t &io_h9, uint64_t *xo = nullptr)
{
    uint64_t v0 = 0;
    uint32_t v1 = 0;
    uint64_t a1 = 0;
    uint64_t a2 = 0;
    uint64_t h2 = io_h2;
    uint64_t h6 = io_h6;
    uint64_t h7 = io_h7;
    uint64_t h8 = io_h8;
    uint64_t h9 = io_h9;
    (a1 = (uint64_t)((uint64_t)a0 + (uint64_t)0ULL));
    (v1 = (uint32_t)((int32_t)((int32_t)A1_R32B((uint32_t)a1, 308))));
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)a1, 312))));
    do { const a4::q128 _v = (A4_R128(ADD32(v1, 0))); h2 = a4::hi64(_v); std::memcpy(&v0, &_v, 8); } while (0);
    A4_W128(ADD32((uint32_t)a0, 0), a4::mk128(h2, v0));
    (v1 = (uint32_t)((uint8_t)A1_R8B((uint32_t)a1, 444)));
    do { const a4::q128 _v = (A4_R128(ADD32((uint32_t)a1, 160))); h2 = a4::hi64(_v); std::memcpy(&v0, &_v, 8); } while (0);
    v1 = v1 & 0x20u;
    { const bool c = v1 == 0; A4_W128(ADD32((uint32_t)a1, 176), a4::mk128(h2, v0)); if (c) goto L_10f2b98; }
    (a2 = (uint64_t)((int64_t)(int32_t)((int16_t)A1_R16B((uint32_t)a1, 436))));
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)a1, 360))));
    (a1 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)a1, 356))));
    { uint64_t a7c[5] = {a1, a2, a3, t0, t1}; CopyMultiMatrix(ram, rt, a0, a1, a2, f12, f13, f14, f15, f16, f17, f18, f19, h6, h7, h8, h9, a7c); a1 = a7c[0]; a2 = a7c[1]; a3 = a7c[2]; t0 = a7c[3]; t1 = a7c[4]; if (xo) { xo[0] = v0; xo[1] = a1; xo[2] = a2; xo[3] = a3; xo[4] = t0; xo[5] = t1; } io_h2 = h2; io_h6 = h6; io_h7 = h7; io_h8 = h8; io_h9 = h9; return; }
L_10f2b98:;
    { if (xo) { xo[0] = v0; xo[1] = a1; } io_h2 = h2; io_h6 = h6; io_h7 = h7; io_h8 = h8; io_h9 = h9; return; }
    __builtin_unreachable();
}

// ps2__CalcCollisionMultiDepSurfBoundary (0x1124368): typed by tools/typed_lift2.py (round A6; guest frame calls, extra outputs xo[0]=a2 xo[1]=a3 xo[2]=t0 xo[3]=t1 xo[4]=t2 xo[5]=t3; VU0 in vf0 vf7 vf10 vf29 vacc h8 h11, out vf4 vf5 vf6 vf7 vf8 vf10 vf14 vf15 vf16 vf17 vf29 vf30 vf31 vacc h11, 64-bit inputs t0)
inline a1::RetVF ps2__CalcCollisionMultiDepSurfBoundary(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint64_t a0, uint64_t a2, uint64_t a3, uint64_t w64_t0, uint64_t t1, uint64_t t2, uint64_t t3, uint32_t sp, float f12, float f13, float f14, float f15, float f16, float f17, float f18, float f19, const a4::vec4 &io_vf0, a4::vec4 &io_vf4, a4::vec4 &io_vf5, a4::vec4 &io_vf6, a4::vec4 &io_vf7, a4::vec4 &io_vf8, a4::vec4 &io_vf10, a4::vec4 &io_vf14, a4::vec4 &io_vf15, a4::vec4 &io_vf16, a4::vec4 &io_vf17, a4::vec4 &io_vf29, a4::vec4 &io_vf30, a4::vec4 &io_vf31, a4::vec4 &io_vacc, uint64_t io_h8, uint64_t &io_h11, uint64_t *xo = nullptr)
{
    float f0 = 0.0f;
    uint32_t at = 0;
    uint64_t v0 = 0;
    uint32_t v1 = 0;
    uint64_t a1 = 0;
    uint32_t t0 = (uint32_t)w64_t0;
    uint64_t s0 = 0;
    a4::vec4 vf0 = io_vf0;
    a4::vec4 vf4 = io_vf4;
    a4::vec4 vf5 = io_vf5;
    a4::vec4 vf6 = io_vf6;
    a4::vec4 vf7 = io_vf7;
    a4::vec4 vf8 = io_vf8;
    a4::vec4 vf10 = io_vf10;
    a4::vec4 vf14 = io_vf14;
    a4::vec4 vf15 = io_vf15;
    a4::vec4 vf16 = io_vf16;
    a4::vec4 vf17 = io_vf17;
    a4::vec4 vf29 = io_vf29;
    a4::vec4 vf30 = io_vf30;
    a4::vec4 vf31 = io_vf31;
    a4::vec4 vacc = io_vacc;
    uint64_t h8 = io_h8;
    uint64_t h11 = io_h11;
    sp = sp - 0x20u;
    (a1 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL));
    (s0 = (uint64_t)((uint64_t)a0 + (uint64_t)0ULL));
    { (a0 = (uint64_t)((uint64_t)a1_sx64(sp) + (uint64_t)0ULL)); A3_CHARGE(); uint64_t a7c[7] = {0u, a2, a3, 0u, t1, t2, t3}; v0 = manager_sub_01116108(ram, rt, a0, a1, a2, a3, w64_t0, t1, t2, t3, a1_sx64(sp), f12, f13, f14, f15, f16, f17, f18, f19, vf8, vf10, vf14, vf15, vf16, vf17, vf29, vf30, vf31, vacc, h11, a7c); a2 = a7c[1]; a3 = a7c[2]; t1 = a7c[4]; t2 = a7c[5]; t3 = a7c[6]; }
    (at = (uint32_t)((int32_t)((int32_t)((uint32_t)16042 << 16))));
    at = at | 0xAAAAu;
    { uint32_t bits = a7_opq(at); std::memcpy(&f0, &bits, sizeof(bits)); }
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B(sp, 0))));
    v1 = A1_R32B(sp, 4);
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B(sp, 8))));
    vf4 = a4v::castsi128_ps(A4_R128(ADD32((uint32_t)a0, 0)));
    vf5 = a4v::castsi128_ps(A4_R128(ADD32(v1, 0)));
    vf6 = a4v::castsi128_ps(A4_R128(ADD32((uint32_t)v0, 0)));
    t0 = a1_bits(f0);
    vf8 = a4v::castsi128_ps(a4::mk128(h8, a1_sx64(t0)));
    { a4::vec4 res = a4v::add(vf4, vf5); a4::q128 mask = a4v::set_epi32(0, -1, -1, -1); vf7 = a4v::blend(vf7, res, a4v::castsi128_ps(mask)); }
    { a4::vec4 res = a4v::addK<1, 0>(vf7, vf6); a4::q128 mask = a4v::set_epi32(0, -1, -1, -1); vf7 = a4v::blend(vf7, res, a4v::castsi128_ps(mask)); }
    { a4::vec4 res = a4v::mulK<1, 0>(vf7, a4v::bcast<0>(vf8)); a4::q128 mask = a4v::set_epi32(0, -1, -1, -1); vf7 = a4v::blendv_ps(vf7, res, a4v::castsi128_ps(mask)); }
    { a4::vec4 res = a4v::subK<0, 1>(vf4, vf7); a4::q128 mask = a4v::set_epi32(0, -1, -1, -1); vf4 = a4v::blend(vf4, res, a4v::castsi128_ps(mask)); }
    { a4::vec4 res = a4v::mulK<1, 1>(vf4, vf4); a4::q128 mask = a4v::set_epi32(0, -1, -1, -1); vf4 = a4v::blend(vf4, res, a4v::castsi128_ps(mask)); }
    { a4::vec4 res = a4v::addK<1, 1>(vf4, a4v::bcast<1>(vf4)); a4::q128 mask = a4v::set_epi32(0, 0, 0, -1); vf4 = a4v::blendv_ps(vf4, res, a4v::castsi128_ps(mask)); }
    { a4::vec4 res = a4v::addK<1, 1>(vf4, a4v::bcast<2>(vf4)); a4::q128 mask = a4v::set_epi32(0, 0, 0, -1); vf4 = a4v::blendv_ps(vf4, res, a4v::castsi128_ps(mask)); }
    { a4::vec4 res = a4v::subK<0, 1>(vf5, vf7); a4::q128 mask = a4v::set_epi32(0, -1, -1, -1); vf5 = a4v::blend(vf5, res, a4v::castsi128_ps(mask)); }
    { a4::vec4 res = a4v::mulK<1, 1>(vf5, vf5); a4::q128 mask = a4v::set_epi32(0, -1, -1, -1); vf5 = a4v::blend(vf5, res, a4v::castsi128_ps(mask)); }
    { a4::vec4 res = a4v::addK<1, 1>(vf5, a4v::bcast<1>(vf5)); a4::q128 mask = a4v::set_epi32(0, 0, 0, -1); vf5 = a4v::blendv_ps(vf5, res, a4v::castsi128_ps(mask)); }
    { a4::vec4 res = a4v::addK<1, 1>(vf5, a4v::bcast<2>(vf5)); a4::q128 mask = a4v::set_epi32(0, 0, 0, -1); vf5 = a4v::blendv_ps(vf5, res, a4v::castsi128_ps(mask)); }
    { a4::vec4 res = a4v::max_ps(vf4, a4v::bcast<0>(vf5)); a4::q128 mask = a4v::set_epi32(0, 0, 0, -1); vf8 = a4v::blendv_ps(vf8, res, a4v::castsi128_ps(mask)); }
    { a4::vec4 res = a4v::subK<0, 1>(vf6, vf7); a4::q128 mask = a4v::set_epi32(0, -1, -1, -1); vf6 = a4v::blend(vf6, res, a4v::castsi128_ps(mask)); }
    { a4::vec4 res = a4v::mulK<1, 1>(vf6, vf6); a4::q128 mask = a4v::set_epi32(0, -1, -1, -1); vf6 = a4v::blend(vf6, res, a4v::castsi128_ps(mask)); }
    { a4::vec4 res = a4v::addK<1, 1>(vf6, a4v::bcast<1>(vf6)); a4::q128 mask = a4v::set_epi32(0, 0, 0, -1); vf6 = a4v::blendv_ps(vf6, res, a4v::castsi128_ps(mask)); }
    { a4::vec4 res = a4v::addK<1, 1>(vf6, a4v::bcast<2>(vf6)); a4::q128 mask = a4v::set_epi32(0, 0, 0, -1); vf6 = a4v::blendv_ps(vf6, res, a4v::castsi128_ps(mask)); }
    { a4::vec4 res = a4v::max_ps(vf8, a4v::bcast<0>(vf6)); a4::q128 mask = a4v::set_epi32(0, 0, 0, -1); vf8 = a4v::blendv_ps(vf8, res, a4v::castsi128_ps(mask)); }
    { a4::vec4 res = a4v::mulK<1, 0>(vf0, a4v::bcast<0>(vf8)); a4::q128 mask = a4v::set_epi32(-1, 0, 0, 0); vf7 = a4v::blendv_ps(vf7, res, a4v::castsi128_ps(mask)); }
    A4_W128(ADD32((uint32_t)s0, 0), a4v::castps_si128(vf7));
    { sp = sp + 0x20u; if (xo) { xo[0] = a2; xo[1] = a3; xo[2] = a1_sx64(t0); xo[3] = t1; xo[4] = t2; xo[5] = t3; } io_vf4 = vf4; io_vf5 = vf5; io_vf6 = vf6; io_vf7 = vf7; io_vf8 = vf8; io_vf10 = vf10; io_vf14 = vf14; io_vf15 = vf15; io_vf16 = vf16; io_vf17 = vf17; io_vf29 = vf29; io_vf30 = vf30; io_vf31 = vf31; io_vacc = vacc; io_h11 = h11; return a1::RetVF{(uint64_t)v0, f0}; }
    __builtin_unreachable();
}

// CalcCollisionMultiDepSurfBoundary (0x1129170): typed by tools/typed_lift2.py (round A6; guest frame calls; VU0 in vf0 vf7 vf10 vf29 vacc h8 h11, out vf4 vf5 vf6 vf7 vf8 vf10 vf14 vf15 vf16 vf17 vf29 vf30 vf31 vacc h11)
inline a1::RetVF CalcCollisionMultiDepSurfBoundary(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint64_t a0, uint64_t a1, uint32_t a2, uint32_t sp, float f12, float f13, float f14, float f15, float f16, float f17, float f18, float f19, const a4::vec4 &io_vf0, a4::vec4 &io_vf4, a4::vec4 &io_vf5, a4::vec4 &io_vf6, a4::vec4 &io_vf7, a4::vec4 &io_vf8, a4::vec4 &io_vf10, a4::vec4 &io_vf14, a4::vec4 &io_vf15, a4::vec4 &io_vf16, a4::vec4 &io_vf17, a4::vec4 &io_vf29, a4::vec4 &io_vf30, a4::vec4 &io_vf31, a4::vec4 &io_vacc, uint64_t io_h8, uint64_t &io_h11)
{
    float f0 = 0.0f;
    uint32_t at = 0;
    uint64_t v0 = 0;
    uint32_t v1 = 0;
    uint32_t a3 = 0;
    uint32_t t0 = 0;
    uint32_t t1 = 0;
    uint32_t t2 = 0;
    uint32_t t3 = 0;
    uint64_t s0 = 0;
    a4::vec4 vf0 = io_vf0;
    a4::vec4 vf4 = io_vf4;
    a4::vec4 vf5 = io_vf5;
    a4::vec4 vf6 = io_vf6;
    a4::vec4 vf7 = io_vf7;
    a4::vec4 vf8 = io_vf8;
    a4::vec4 vf10 = io_vf10;
    a4::vec4 vf14 = io_vf14;
    a4::vec4 vf15 = io_vf15;
    a4::vec4 vf16 = io_vf16;
    a4::vec4 vf17 = io_vf17;
    a4::vec4 vf29 = io_vf29;
    a4::vec4 vf30 = io_vf30;
    a4::vec4 vf31 = io_vf31;
    a4::vec4 vacc = io_vacc;
    uint64_t h8 = io_h8;
    uint64_t h11 = io_h11;
    sp = sp - 0x20u;
    (v0 = (uint64_t)((uint64_t)a1 + (uint64_t)0ULL));
    (s0 = (uint64_t)((uint64_t)a0 + (uint64_t)0ULL));
    t1 = a2 << 6;
    (a0 = (uint64_t)((uint64_t)a1_sx64(sp) + (uint64_t)0ULL));
    (a1 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL));
    (a3 = (uint32_t)((int32_t)((int32_t)A1_R32B((uint32_t)v0, 388))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v0, 340))));
    (t0 = (uint32_t)((int32_t)((int32_t)A1_R32B((uint32_t)v0, 32))));
    v1 = A1_R32B(a3, 32);
    a2 = t0 + 0x28u;
    a3 = A1_R32B(t0, 36);
    t3 = A1_R32B(v1, 24);
    (t0 = (uint32_t)((int32_t)((int32_t)A1_R32B((uint32_t)v0, 8))));
    t2 = A1_R32B(v1, 20);
    t3 = t3 + t1;
    { t1 = A1_R32B(v1, 8); A3_CHARGE(); v0 = manager_sub_01116108(ram, rt, a0, a1, a1_sx64(a2), a1_sx64(a3), a1_sx64(t0), a1_sx64(t1), a1_sx64(t2), a1_sx64(t3), a1_sx64(sp), f12, f13, f14, f15, f16, f17, f18, f19, vf8, vf10, vf14, vf15, vf16, vf17, vf29, vf30, vf31, vacc, h11); }
    at = 0x3EAA0000u;
    at = at | 0xAAAAu;
    { uint32_t bits = a7_opq(at); std::memcpy(&f0, &bits, sizeof(bits)); }
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B(sp, 0))));
    v1 = A1_R32B(sp, 4);
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B(sp, 8))));
    vf4 = a4v::castsi128_ps(A4_R128(ADD32((uint32_t)a0, 0)));
    vf5 = a4v::castsi128_ps(A4_R128(ADD32(v1, 0)));
    vf6 = a4v::castsi128_ps(A4_R128(ADD32((uint32_t)v0, 0)));
    t0 = a1_bits(f0);
    vf8 = a4v::castsi128_ps(a4::mk128(h8, a1_sx64(t0)));
    { a4::vec4 res = a4v::add(vf4, vf5); a4::q128 mask = a4v::set_epi32(0, -1, -1, -1); vf7 = a4v::blend(vf7, res, a4v::castsi128_ps(mask)); }
    { a4::vec4 res = a4v::addK<1, 0>(vf7, vf6); a4::q128 mask = a4v::set_epi32(0, -1, -1, -1); vf7 = a4v::blend(vf7, res, a4v::castsi128_ps(mask)); }
    { a4::vec4 res = a4v::mulK<1, 0>(vf7, a4v::bcast<0>(vf8)); a4::q128 mask = a4v::set_epi32(0, -1, -1, -1); vf7 = a4v::blendv_ps(vf7, res, a4v::castsi128_ps(mask)); }
    { a4::vec4 res = a4v::subK<0, 1>(vf4, vf7); a4::q128 mask = a4v::set_epi32(0, -1, -1, -1); vf4 = a4v::blend(vf4, res, a4v::castsi128_ps(mask)); }
    { a4::vec4 res = a4v::mulK<1, 1>(vf4, vf4); a4::q128 mask = a4v::set_epi32(0, -1, -1, -1); vf4 = a4v::blend(vf4, res, a4v::castsi128_ps(mask)); }
    { a4::vec4 res = a4v::addK<1, 1>(vf4, a4v::bcast<1>(vf4)); a4::q128 mask = a4v::set_epi32(0, 0, 0, -1); vf4 = a4v::blendv_ps(vf4, res, a4v::castsi128_ps(mask)); }
    { a4::vec4 res = a4v::addK<1, 1>(vf4, a4v::bcast<2>(vf4)); a4::q128 mask = a4v::set_epi32(0, 0, 0, -1); vf4 = a4v::blendv_ps(vf4, res, a4v::castsi128_ps(mask)); }
    { a4::vec4 res = a4v::subK<0, 1>(vf5, vf7); a4::q128 mask = a4v::set_epi32(0, -1, -1, -1); vf5 = a4v::blend(vf5, res, a4v::castsi128_ps(mask)); }
    { a4::vec4 res = a4v::mulK<1, 1>(vf5, vf5); a4::q128 mask = a4v::set_epi32(0, -1, -1, -1); vf5 = a4v::blend(vf5, res, a4v::castsi128_ps(mask)); }
    { a4::vec4 res = a4v::addK<1, 1>(vf5, a4v::bcast<1>(vf5)); a4::q128 mask = a4v::set_epi32(0, 0, 0, -1); vf5 = a4v::blendv_ps(vf5, res, a4v::castsi128_ps(mask)); }
    { a4::vec4 res = a4v::addK<1, 1>(vf5, a4v::bcast<2>(vf5)); a4::q128 mask = a4v::set_epi32(0, 0, 0, -1); vf5 = a4v::blendv_ps(vf5, res, a4v::castsi128_ps(mask)); }
    { a4::vec4 res = a4v::max_ps(vf4, a4v::bcast<0>(vf5)); a4::q128 mask = a4v::set_epi32(0, 0, 0, -1); vf8 = a4v::blendv_ps(vf8, res, a4v::castsi128_ps(mask)); }
    { a4::vec4 res = a4v::subK<0, 1>(vf6, vf7); a4::q128 mask = a4v::set_epi32(0, -1, -1, -1); vf6 = a4v::blend(vf6, res, a4v::castsi128_ps(mask)); }
    { a4::vec4 res = a4v::mulK<1, 1>(vf6, vf6); a4::q128 mask = a4v::set_epi32(0, -1, -1, -1); vf6 = a4v::blend(vf6, res, a4v::castsi128_ps(mask)); }
    { a4::vec4 res = a4v::addK<1, 1>(vf6, a4v::bcast<1>(vf6)); a4::q128 mask = a4v::set_epi32(0, 0, 0, -1); vf6 = a4v::blendv_ps(vf6, res, a4v::castsi128_ps(mask)); }
    { a4::vec4 res = a4v::addK<1, 1>(vf6, a4v::bcast<2>(vf6)); a4::q128 mask = a4v::set_epi32(0, 0, 0, -1); vf6 = a4v::blendv_ps(vf6, res, a4v::castsi128_ps(mask)); }
    { a4::vec4 res = a4v::max_ps(vf8, a4v::bcast<0>(vf6)); a4::q128 mask = a4v::set_epi32(0, 0, 0, -1); vf8 = a4v::blendv_ps(vf8, res, a4v::castsi128_ps(mask)); }
    { a4::vec4 res = a4v::mulK<1, 0>(vf0, a4v::bcast<0>(vf8)); a4::q128 mask = a4v::set_epi32(-1, 0, 0, 0); vf7 = a4v::blendv_ps(vf7, res, a4v::castsi128_ps(mask)); }
    A4_W128(ADD32((uint32_t)s0, 0), a4v::castps_si128(vf7));
    { sp = sp + 0x20u; io_vf4 = vf4; io_vf5 = vf5; io_vf6 = vf6; io_vf7 = vf7; io_vf8 = vf8; io_vf10 = vf10; io_vf14 = vf14; io_vf15 = vf15; io_vf16 = vf16; io_vf17 = vf17; io_vf29 = vf29; io_vf30 = vf30; io_vf31 = vf31; io_vacc = vacc; io_h11 = h11; return a1::RetVF{(uint64_t)v0, f0}; }
    __builtin_unreachable();
}

// disp_particle_manager (0x113f220): typed by tools/typed_lift2.py (round A6; frame calls, extra outputs xo[0]=a0; VU0 in vf0 vf14 vf15 vf16 vf17 vacc, out vf14 vf15 vf16 vf17 vf24 vf25 vf26 vf27 vacc, relocated calls 5, frame 48 bytes, 6 slot accesses)
inline a1::RetVF disp_particle_manager(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t v0, uint64_t a0, float f12, float f13, float f14, float f15, float f16, float f17, float f18, float f19, float f20, float f21, float f22, const a4::vec4 &io_vf0, a4::vec4 &io_vf14, a4::vec4 &io_vf15, a4::vec4 &io_vf16, a4::vec4 &io_vf17, a4::vec4 &io_vf24, a4::vec4 &io_vf25, a4::vec4 &io_vf26, a4::vec4 &io_vf27, a4::vec4 &io_vacc, uint64_t *xo = nullptr)
{
    float f0 = 0.0f;
    uint32_t at = 0;
    uint32_t s0 = 0;
    uint32_t fcr31 = 0;
    a4::vec4 vf0 = io_vf0;
    a4::vec4 vf14 = io_vf14;
    a4::vec4 vf15 = io_vf15;
    a4::vec4 vf16 = io_vf16;
    a4::vec4 vf17 = io_vf17;
    a4::vec4 vf24 = io_vf24;
    a4::vec4 vf25 = io_vf25;
    a4::vec4 vf26 = io_vf26;
    a4::vec4 vf27 = io_vf27;
    a4::vec4 vacc = io_vacc;
    uint8_t a2fr[48];
    s0 = 0x1190000u;
    s0 = s0 - 0x3000u;
    a2::frSt<uint32_t>(a2fr, 32, a1_bits(f22));
    a2::frSt<uint32_t>(a2fr, 24, a1_bits(f21));
    a2::frSt<uint32_t>(a2fr, 16, a1_bits(f20));
    { (a0 = (uint64_t)((uint64_t)a1_sx64(s0) + (uint64_t)0ULL)); A3_CHARGE(); iosUnitMatrix(ram, rt, (uint32_t)a0, vf0, vf14, vf15, vf16, vf17); }
    { A3_CHARGE(); v0 = getDrawScreenWidth(ram, rt); }
    at = 0x3F800000u;
    { uint32_t bits = a7_opq(at); std::memcpy(&f20, &bits, sizeof(bits)); }
    { uint32_t bits = a7_opq(v0); std::memcpy(&f0, &bits, sizeof(bits)); }
    { int32_t tmp; std::memcpy(&tmp, &f0, sizeof(tmp)); f0 = ((float)(int32_t)(tmp)); }
    at = 0xC5000000u;
    { uint32_t bits = a7_opq(at); std::memcpy(&f21, &bits, sizeof(bits)); }
    at = 0x3F000000u;
    { uint32_t bits = a7_opq(at); std::memcpy(&f22, &bits, sizeof(bits)); }
    f0 = a1f::div(fcr31, f20, f0);
    { A1_W32(s0, a1_bits(f0)); A3_CHARGE(); v0 = getDrawScreenHeight(ram, rt); }
    { uint32_t bits = a7_opq(v0); std::memcpy(&f0, &bits, sizeof(bits)); }
    { int32_t tmp; std::memcpy(&tmp, &f0, sizeof(tmp)); f0 = ((float)(int32_t)(tmp)); }
    f20 = a1f::div(fcr31, f20, f0);
    { A1_W32B(s0, 20, a1_bits(f20)); A3_CHARGE(); v0 = getDrawScreenWidth(ram, rt); }
    { uint32_t bits = a7_opq(v0); std::memcpy(&f0, &bits, sizeof(bits)); }
    { int32_t tmp; std::memcpy(&tmp, &f0, sizeof(tmp)); f0 = ((float)(int32_t)(tmp)); }
    f0 = a1f::div(fcr31, f21, f0);
    f0 = a1f::add(f0, f22);
    { A1_W32B(s0, 48, a1_bits(f0)); A3_CHARGE(); v0 = getDrawScreenHeight(ram, rt); }
    { uint32_t bits = a7_opq(v0); std::memcpy(&f0, &bits, sizeof(bits)); }
    { int32_t tmp; std::memcpy(&tmp, &f0, sizeof(tmp)); f0 = ((float)(int32_t)(tmp)); }
    f21 = a1f::div(fcr31, f21, f0);
    f21 = a1f::add(f21, f22);
    { A1_W32B(s0, 52, a1_bits(f21)); A3_CHARGE(); v0 = iosgluGetCameraContext(ram, rt, f12, f13, f14, f15, f16, f17, f18, f19); }
    v0 = v0 + 0x660u;
    vf14 = a4v::castsi128_ps(A4_R128(ADD32(s0, 0)));
    vf15 = a4v::castsi128_ps(A4_R128(ADD32(s0, 16)));
    vf16 = a4v::castsi128_ps(A4_R128(ADD32(s0, 32)));
    vf17 = a4v::castsi128_ps(A4_R128(ADD32(s0, 48)));
    vf24 = a4v::castsi128_ps(A4_R128(ADD32(v0, 0)));
    vf25 = a4v::castsi128_ps(A4_R128(ADD32(v0, 16)));
    vf26 = a4v::castsi128_ps(A4_R128(ADD32(v0, 32)));
    vf27 = a4v::castsi128_ps(A4_R128(ADD32(v0, 48)));
    { a4::vec4 res = a4v::mul(vf14, a4v::bcast<0>(vf24)); vacc = a4v::dest<1, 1, 1, 1>(vacc, res); }
    { a4::vec4 mul_res = a4v::mul(vf15, a4v::bcast<1>(vf24)); a4::vec4 res = a4v::addK<1, 1>(vacc, mul_res); vacc = a4v::dest<1, 1, 1, 1>(vacc, res); }
    { a4::vec4 mul_res = a4v::mul(vf16, a4v::bcast<2>(vf24)); a4::vec4 res = a4v::addK<1, 1>(vacc, mul_res); vacc = a4v::dest<1, 1, 1, 1>(vacc, res); }
    { a4::vec4 mul_res = a4v::mul(vf17, a4v::bcast<3>(vf24)); a4::vec4 res = a4v::addK<1, 1>(vacc, mul_res); a4::q128 mask = a4v::set_epi32(-1, -1, -1, -1); vf24 = a4v::blendv_ps(vf24, res, a4v::castsi128_ps(mask)); }
    { a4::vec4 res = a4v::mul(vf14, a4v::bcast<0>(vf25)); vacc = a4v::dest<1, 1, 1, 1>(vacc, res); }
    { a4::vec4 mul_res = a4v::mul(vf15, a4v::bcast<1>(vf25)); a4::vec4 res = a4v::addK<1, 1>(vacc, mul_res); vacc = a4v::dest<1, 1, 1, 1>(vacc, res); }
    { a4::vec4 mul_res = a4v::mul(vf16, a4v::bcast<2>(vf25)); a4::vec4 res = a4v::addK<1, 1>(vacc, mul_res); vacc = a4v::dest<1, 1, 1, 1>(vacc, res); }
    { a4::vec4 mul_res = a4v::mul(vf17, a4v::bcast<3>(vf25)); a4::vec4 res = a4v::addK<1, 1>(vacc, mul_res); a4::q128 mask = a4v::set_epi32(-1, -1, -1, -1); vf25 = a4v::blendv_ps(vf25, res, a4v::castsi128_ps(mask)); }
    { a4::vec4 res = a4v::mul(vf14, a4v::bcast<0>(vf26)); vacc = a4v::dest<1, 1, 1, 1>(vacc, res); }
    { a4::vec4 mul_res = a4v::mul(vf15, a4v::bcast<1>(vf26)); a4::vec4 res = a4v::addK<1, 1>(vacc, mul_res); vacc = a4v::dest<1, 1, 1, 1>(vacc, res); }
    { a4::vec4 mul_res = a4v::mul(vf16, a4v::bcast<2>(vf26)); a4::vec4 res = a4v::addK<1, 1>(vacc, mul_res); vacc = a4v::dest<1, 1, 1, 1>(vacc, res); }
    { a4::vec4 mul_res = a4v::mul(vf17, a4v::bcast<3>(vf26)); a4::vec4 res = a4v::addK<1, 1>(vacc, mul_res); a4::q128 mask = a4v::set_epi32(-1, -1, -1, -1); vf26 = a4v::blendv_ps(vf26, res, a4v::castsi128_ps(mask)); }
    { a4::vec4 res = a4v::mul(vf14, a4v::bcast<0>(vf27)); vacc = a4v::dest<1, 1, 1, 1>(vacc, res); }
    { a4::vec4 mul_res = a4v::mul(vf15, a4v::bcast<1>(vf27)); a4::vec4 res = a4v::addK<1, 1>(vacc, mul_res); vacc = a4v::dest<1, 1, 1, 1>(vacc, res); }
    { a4::vec4 mul_res = a4v::mul(vf16, a4v::bcast<2>(vf27)); a4::vec4 res = a4v::addK<1, 1>(vacc, mul_res); vacc = a4v::dest<1, 1, 1, 1>(vacc, res); }
    { a4::vec4 mul_res = a4v::mul(vf17, a4v::bcast<3>(vf27)); a4::vec4 res = a4v::addK<1, 1>(vacc, mul_res); a4::q128 mask = a4v::set_epi32(-1, -1, -1, -1); vf27 = a4v::blendv_ps(vf27, res, a4v::castsi128_ps(mask)); }
    A4_W128(ADD32(s0, 0), a4v::castps_si128(vf24));
    A4_W128(ADD32(s0, 16), a4v::castps_si128(vf25));
    A4_W128(ADD32(s0, 32), a4v::castps_si128(vf26));
    A4_W128(ADD32(s0, 48), a4v::castps_si128(vf27));
    f22 = a1_flt(a2::frLd<uint32_t>(a2fr, 32));
    f21 = a1_flt(a2::frLd<uint32_t>(a2fr, 24));
    f20 = a1_flt(a2::frLd<uint32_t>(a2fr, 16));
    { if (xo) { xo[0] = a0; } io_vf14 = vf14; io_vf15 = vf15; io_vf16 = vf16; io_vf17 = vf17; io_vf24 = vf24; io_vf25 = vf25; io_vf26 = vf26; io_vf27 = vf27; io_vacc = vacc; return a1::RetVF{(uint64_t)a1_sx64(v0), f0}; }
    __builtin_unreachable();
}

// get_particle_lod_base_size (0x11403c0): typed by tools/typed_lift2.py (round A6; frame calls, extra outputs xo[0]=f12 xo[1]=f13; VU0 in vf10 vacc, out vf8 vf10 vf14 vf15 vf16 vf17 vacc, guest sp, frame 16 bytes, 0 slot accesses)
inline a1::RetVF get_particle_lod_base_size(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint64_t v0, uint64_t a0, uint32_t sp, float f12, float f13, float f14, float f15, float f16, float f17, float f18, float f19, a4::vec4 &io_vf8, a4::vec4 &io_vf10, a4::vec4 &io_vf14, a4::vec4 &io_vf15, a4::vec4 &io_vf16, a4::vec4 &io_vf17, a4::vec4 &io_vacc, uint64_t *xo = nullptr)
{
    uint32_t at = 0;
    uint64_t s0 = 0;
    a4::vec4 vf8 = io_vf8;
    a4::vec4 vf10 = io_vf10;
    a4::vec4 vf14 = io_vf14;
    a4::vec4 vf15 = io_vf15;
    a4::vec4 vf16 = io_vf16;
    a4::vec4 vf17 = io_vf17;
    a4::vec4 vacc = io_vacc;
    { (s0 = (uint64_t)((uint64_t)a0 + (uint64_t)0ULL)); A3_CHARGE(); v0 = a1_sx64(iosgluGetCameraContext(ram, rt, f12, f13, f14, f15, f16, f17, f18, f19)); }
    (a0 = (uint64_t)((uint64_t)v0 + (uint64_t)0ULL));
    { uint32_t bits = A1_R32B((uint32_t)s0, 304); float f; std::memcpy(&f, &bits, sizeof(f)); f12 = f; }
    at = 0x3F800000u;
    { uint32_t bits = a7_opq(at); std::memcpy(&f13, &bits, sizeof(bits)); }
    { if (xo) { xo[0] = a1_bits(f12); xo[1] = a1_bits(f13); } const a1::RetVF a4_t = GetScreenRadiusParticle(ram, rt, (uint32_t)a0, (uint32_t)(sp + 0x0u), f12, f13, vf8, vf10, vf14, vf15, vf16, vf17, vacc); io_vf8 = vf8; io_vf10 = vf10; io_vf14 = vf14; io_vf15 = vf15; io_vf16 = vf16; io_vf17 = vf17; io_vacc = vacc; return a4_t; }
    __builtin_unreachable();
}

// gamecore_sub_01382388 (0x1382388): typed by tools/typed_lift2.py (round A6; loop guest frame calls, extra outputs xo[0]=a2 xo[1]=a3 xo[2]=t0 xo[3]=t1 xo[4]=t2 xo[5]=t3; VU0 in vf0 vf7 vf10 vacc vstat, out vf5 vf6 vf7 vf8 vf10 vf14 vf15 vf16 vf17 vacc vq vstat h2 h3 h6 h7 h8 h9, relocated calls 2)
inline a1::RetVF gamecore_sub_01382388(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint64_t v0, uint64_t a0, uint64_t a1, uint64_t sp, float f12, float f13, float f14, float f15, float f16, float f17, float f18, float f19, const a4::vec4 &io_vf0, a4::vec4 &io_vf5, a4::vec4 &io_vf6, a4::vec4 &io_vf7, a4::vec4 &io_vf8, a4::vec4 &io_vf10, a4::vec4 &io_vf14, a4::vec4 &io_vf15, a4::vec4 &io_vf16, a4::vec4 &io_vf17, a4::vec4 &io_vacc, float &io_vq, uint16_t &io_vstat, uint64_t &io_h2, uint64_t &io_h3, uint64_t &io_h6, uint64_t &io_h7, uint64_t &io_h8, uint64_t &io_h9, uint64_t *xo = nullptr)
{
    float f0 = 0.0f;
    float f1 = 0.0f;
    float f2 = 0.0f;
    float f3 = 0.0f;
    float f4 = 0.0f;
    uint32_t at = 0;
    uint64_t v1 = 0;
    uint64_t a2 = 0;
    uint64_t a3 = 0;
    uint64_t t0 = 0;
    uint64_t t1 = 0;
    uint32_t t2 = 0;
    uint64_t t3 = 0;
    uint32_t t4 = 0;
    uint64_t t5 = 0;
    uint64_t t6 = 0;
    uint32_t t7 = 0;
    uint64_t s0 = 0;
    uint64_t s1 = 0;
    uint64_t s2 = 0;
    uint64_t s3 = 0;
    uint32_t s4 = 0;
    uint64_t s5 = 0;
    uint32_t s6 = 0;
    uint32_t s7 = 0;
    uint64_t t8 = 0;
    uint32_t t9 = 0;
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
    float vq = io_vq;
    uint16_t vstat = io_vstat;
    uint64_t h2 = io_h2;
    uint64_t h3 = io_h3;
    uint64_t h6 = io_h6;
    uint64_t h7 = io_h7;
    uint64_t h8 = io_h8;
    uint64_t h9 = io_h9;
    bool a3xw4 = false;
    bool a3xw5 = false;
    (sp = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)sp, 4294967040))));
    (s3 = (uint64_t)((uint64_t)a1 + (uint64_t)0ULL));
    (s1 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)a0, 112))));
    { (s2 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)a0, 132)))); A3_CHARGE(); v0 = a1_sx64(iosgluGetCameraContext(ram, rt, f12, f13, f14, f15, f16, f17, f18, f19)); }
    (s0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)sp, 96))));
    (a1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v0, 416))));
    { (a0 = (uint64_t)((uint64_t)RC_U64X(s0) + (uint64_t)0ULL)); A3_CHARGE(); uint64_t a7c[4] = {0u, 0u, 0u, 0u}; iosCopyMatrix_x8(ram, rt, (uint32_t)a0, (uint32_t)a1, h6, h7, h8, h9, a7c); a2 = a7c[0]; a3 = a7c[1]; t0 = a7c[2]; t1 = a7c[3]; }
    do { const a4::q128 _v = (A4_R128(ADD32((uint32_t)s1, 0))); h2 = a4::hi64(_v); std::memcpy(&v0, &_v, 8); } while (0);
    (at = (uint32_t)((int32_t)((int32_t)((uint32_t)16256 << 16))));
    { uint32_t bits = a7_opq(at); std::memcpy(&f0, &bits, sizeof(bits)); }
    (s4 = (uint32_t)((int32_t)((int32_t)ADD32((uint32_t)sp, 16))));
    A4_W128(ADD32((uint32_t)sp, 0), a4::mk128(h2, v0));
    { float f = f0; uint32_t bits; std::memcpy(&bits, &f, sizeof(bits)); A1_W32B((uint32_t)sp, 12, bits); }
    vf8 = a4v::castsi128_ps(A4_R128(ADD32((uint32_t)sp, 0)));
    vf14 = a4v::castsi128_ps(A4_R128(ADD32((uint32_t)s0, 0)));
    vf15 = a4v::castsi128_ps(A4_R128(ADD32((uint32_t)s0, 16)));
    vf16 = a4v::castsi128_ps(A4_R128(ADD32((uint32_t)s0, 32)));
    vf17 = a4v::castsi128_ps(A4_R128(ADD32((uint32_t)s0, 48)));
    { a4::vec4 res = a4v::mul(vf14, a4v::bcast<0>(vf8)); vacc = a4v::dest<1, 1, 1, 1>(vacc, res); }
    { a4::vec4 mul_res = a4v::mul(vf15, a4v::bcast<1>(vf8)); a4::vec4 res = a4v::addK<1, 1>(vacc, mul_res); vacc = a4v::dest<1, 1, 1, 1>(vacc, res); }
    { a4::vec4 mul_res = a4v::mul(vf16, a4v::bcast<2>(vf8)); a4::vec4 res = a4v::addK<1, 1>(vacc, mul_res); vacc = a4v::dest<1, 1, 1, 1>(vacc, res); }
    { a4::vec4 mul_res = a4v::mul(vf17, a4v::bcast<3>(vf8)); a4::vec4 res = a4v::addK<1, 1>(vacc, mul_res); a4::q128 mask = a4v::set_epi32(-1, -1, -1, -1); vf10 = a4v::blendv_ps(vf10, res, a4v::castsi128_ps(mask)); }
    A4_W128(ADD32(s4, 0), a4v::castps_si128(vf10));
    { uint32_t bits = A1_R32B((uint32_t)sp, 24); float f; std::memcpy(&f, &bits, sizeof(f)); f2 = f; }
    (t6 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL));
    { const bool c = (int32_t)s2 <= 0; A1_W32B((uint32_t)sp, 24, 0u); if (c) goto L_13825e0; }
    (s6 = (uint32_t)((int32_t)((int32_t)ADD32((uint32_t)sp, 80))));
    { const uint32_t _dynInsn = A1_R32(0x1382424u); const int32_t _dynSimm = (int16_t)(_dynInsn & 0xFFFFu); const uint32_t _dynImm = _dynInsn & 0xFFFFu; (void)_dynSimm; (void)_dynImm; (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)_dynImm << 16)))); }
    (at = (uint32_t)((int32_t)((int32_t)((uint32_t)16128 << 16))));
    { uint32_t bits = a7_opq(at); std::memcpy(&f4, &bits, sizeof(bits)); }
    (s5 = (uint64_t)((uint64_t)s0 + (uint64_t)0ULL));
    (t1 = (uint64_t)((uint64_t)s1 + (uint64_t)0ULL));
    (s1 = (uint64_t)((uint64_t)a1_sx64(s4) + (uint64_t)0ULL));
    f3 = ((float)(f0));
    { const uint32_t _dynInsn = A1_R32(0x1382440u); const int32_t _dynSimm = (int16_t)(_dynInsn & 0xFFFFu); const uint32_t _dynImm = _dynInsn & 0xFFFFu; (void)_dynSimm; (void)_dynImm; (t9 = (uint32_t)((int32_t)((int32_t)ADD32((uint32_t)v0, _dynSimm)))); }
    (s7 = (uint32_t)((int32_t)((int32_t)ADD32((uint32_t)s2, 4294967295))));
    (t7 = (uint32_t)((int32_t)((int32_t)ADD32((uint32_t)sp, 160))));
    (t5 = (uint64_t)((uint64_t)sp + (uint64_t)0ULL));
    (t4 = (uint32_t)((int32_t)((int32_t)ADD32((uint32_t)sp, 48))));
    (t2 = (uint32_t)((int32_t)((int32_t)ADD32((uint32_t)sp, 64)))); a3xw4 = true;
    (t3 = (uint64_t)((uint64_t)a1_sx64(s6) + (uint64_t)0ULL)); a3xw5 = true;
    (t8 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL));
    (s0 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL));
L_1382468:;
    if ((int64_t)t6 != (int64_t)(int32_t)s7) { do { const a4::q128 _v = (A4_R128(ADD32((uint32_t)t1, 16))); h2 = a4::hi64(_v); std::memcpy(&v0, &_v, 8); } while (0); goto L_13824a0; }
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)t1, 4294967280))));
    vf5 = a4v::castsi128_ps(A4_R128(ADD32((uint32_t)t1, 0)));
    vf6 = a4v::castsi128_ps(A4_R128(ADD32((uint32_t)v0, 0)));
    { a4::vec4 res = a4v::sub(vf5, vf6); a4::q128 mask = a4v::set_epi32(0, -1, -1, -1); vf5 = a4v::blend(vf5, res, a4v::castsi128_ps(mask)); }
    A4_W128(ADD32(t7, 0), a4v::castps_si128(vf5));
    vf5 = a4v::castsi128_ps(A4_R128(ADD32((uint32_t)t1, 0)));
    vf6 = a4v::castsi128_ps(A4_R128(ADD32(t7, 0)));
    { a4::vec4 res = a4v::add(vf5, vf6); a4::q128 mask = a4v::set_epi32(0, -1, -1, -1); vf5 = a4v::blend(vf5, res, a4v::castsi128_ps(mask)); }
    A4_W128(ADD32((uint32_t)t5, 0), a4v::castps_si128(vf5));
    { float f = f3; uint32_t bits; std::memcpy(&bits, &f, sizeof(bits)); A1_W32B((uint32_t)sp, 12, bits); }
    goto L_13824a8;
L_13824a0:;
    A4_W128(ADD32((uint32_t)sp, 0), a4::mk128(h2, v0));
    { float f = f3; uint32_t bits; std::memcpy(&bits, &f, sizeof(bits)); A1_W32B((uint32_t)sp, 12, bits); }
L_13824a8:;
    vf8 = a4v::castsi128_ps(A4_R128(ADD32((uint32_t)t5, 0)));
    vf14 = a4v::castsi128_ps(A4_R128(ADD32((uint32_t)s5, 0)));
    vf15 = a4v::castsi128_ps(A4_R128(ADD32((uint32_t)s5, 16)));
    vf16 = a4v::castsi128_ps(A4_R128(ADD32((uint32_t)s5, 32)));
    vf17 = a4v::castsi128_ps(A4_R128(ADD32((uint32_t)s5, 48)));
    { a4::vec4 res = a4v::mul(vf14, a4v::bcast<0>(vf8)); vacc = a4v::dest<1, 1, 1, 1>(vacc, res); }
    { a4::vec4 mul_res = a4v::mul(vf15, a4v::bcast<1>(vf8)); a4::vec4 res = a4v::addK<1, 1>(vacc, mul_res); vacc = a4v::dest<1, 1, 1, 1>(vacc, res); }
    { a4::vec4 mul_res = a4v::mul(vf16, a4v::bcast<2>(vf8)); a4::vec4 res = a4v::addK<1, 1>(vacc, mul_res); vacc = a4v::dest<1, 1, 1, 1>(vacc, res); }
    { a4::vec4 mul_res = a4v::mul(vf17, a4v::bcast<3>(vf8)); a4::vec4 res = a4v::addK<1, 1>(vacc, mul_res); a4::q128 mask = a4v::set_epi32(-1, -1, -1, -1); vf10 = a4v::blendv_ps(vf10, res, a4v::castsi128_ps(mask)); }
    A4_W128(ADD32(t4, 0), a4v::castps_si128(vf10));
    { uint32_t bits = A1_R32B((uint32_t)sp, 56); float f; std::memcpy(&f, &bits, sizeof(f)); f1 = f; }
    A1_W32B((uint32_t)sp, 56, 0u);
    vf5 = a4v::castsi128_ps(A4_R128(ADD32((uint32_t)s1, 0)));
    vf6 = a4v::castsi128_ps(A4_R128(ADD32(t4, 0)));
    { a4::vec4 res = a4v::sub(vf5, vf6); a4::q128 mask = a4v::set_epi32(0, -1, -1, -1); vf5 = a4v::blend(vf5, res, a4v::castsi128_ps(mask)); }
    A4_W128(ADD32(t2, 0), a4v::castps_si128(vf5));
    vf5 = a4v::castsi128_ps(A4_R128(ADD32(t2, 0)));
    { a4::vec4 res = a4v::mul(vf5, vf5); a4::q128 mask = a4v::set_epi32(0, -1, -1, -1); vf7 = a4v::blend(vf7, res, a4v::castsi128_ps(mask)); }
    { a4::vec4 res = a4v::mulK<1, 1>(vf0, a4v::bcast<0>(vf7)); vacc = a4v::dest<0, 0, 0, 1>(vacc, res); }
    { a4::vec4 mul_res = a4v::mulK<1, 1>(vf0, a4v::bcast<1>(vf7)); a4::vec4 res = a4v::addK<1, 1>(vacc, mul_res); vacc = a4v::dest<0, 0, 0, 1>(vacc, res); }
    { a4::vec4 mul_res = a4v::mulK<1, 1>(vf0, a4v::bcast<2>(vf7)); a4::vec4 res = a4v::addK<1, 1>(vacc, mul_res); a4::q128 mask = a4v::set_epi32(-1, 0, 0, 0); vf7 = a4v::blendv_ps(vf7, res, a4v::castsi128_ps(mask)); }
    { float fs = a4v::cvtss_f32(a4v::shuffle_ps<0x03>(vf0, vf0)); float ft = a4v::cvtss_f32(a4v::shuffle_ps<0x03>(vf7, vf7)); vq = a4v::rsqrt(vstat, fs, ft); }
    { a4::vec4 res = a4v::mul(vf5, a4v::set1_ps(vq)); a4::q128 mask = a4v::set_epi32(0, -1, -1, -1); vf5 = a4v::blendv_ps(vf5, res, a4v::castsi128_ps(mask)); }
    A4_W128(ADD32(t2, 0), a4v::castps_si128(vf5));
    vf5 = a4v::castsi128_ps(A4_R128(ADD32(t9, 0)));
    vf6 = a4v::castsi128_ps(A4_R128(ADD32(t2, 0)));
    { a4::vec4 fs_yzx = a4v::shuffle_ps<0xC9>(vf5, vf5); a4::vec4 ft_zxy = a4v::shuffle_ps<0xD2>(vf6, vf6); a4::vec4 res = a4v::mul(fs_yzx, ft_zxy); vacc = a4v::dest<1, 1, 1, 0>(vacc, res); }
    { a4::vec4 fs_yzx = a4v::shuffle_ps<0xC9>(vf6, vf6); a4::vec4 ft_zxy = a4v::shuffle_ps<0xD2>(vf5, vf5); a4::vec4 mul_res = a4v::mul(fs_yzx, ft_zxy); a4::vec4 res = a4v::subK<1, 1>(vacc, mul_res); a4::q128 mask = a4v::set_epi32(0, -1, -1, -1); vf7 = a4v::blendv_ps(vf7, res, a4v::castsi128_ps(mask)); }
    { a4::vec4 res = a4v::sub(vf7, vf7); a4::q128 mask = a4v::set_epi32(-1, 0, 0, 0); vf7 = a4v::blend(vf7, res, a4v::castsi128_ps(mask)); }
    A4_W128(ADD32((uint32_t)t3, 0), a4v::castps_si128(vf7));
    { uint32_t bits = A1_R32B((uint32_t)t1, 12); float f; std::memcpy(&f, &bits, sizeof(f)); f0 = f; }
    f0 = a1f::mul(f0, f4);
    { uint32_t bits; std::memcpy(&bits, &f0, sizeof(bits)); (v0 = (uint64_t)((int64_t)(int32_t)(bits))); }
    vf5 = a4v::castsi128_ps(A4_R128(ADD32((uint32_t)t3, 0)));
    vf6 = a4v::castsi128_ps(a4::mk128(h2, v0));
    { a4::vec4 res = a4v::mul(vf5, a4v::bcast<0>(vf6)); a4::q128 mask = a4v::set_epi32(-1, -1, -1, -1); vf5 = a4v::blendv_ps(vf5, res, a4v::castsi128_ps(mask)); }
    A4_W128(ADD32((uint32_t)t3, 0), a4v::castps_si128(vf5));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)s0, (uint32_t)t6))));
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)SLL32((uint32_t)t8, 4))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)SLL32((uint32_t)v0, 4))));
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v1, (uint32_t)s3))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v0, (uint32_t)s3))));
    (t0 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL));
    (a2 = (uint64_t)((uint64_t)RC_U64X(v0) + (uint64_t)0ULL));
    (a1 = (uint64_t)((uint64_t)a1_sx64(s4) + (uint64_t)0ULL));
    (a0 = (uint64_t)((uint64_t)a1_sx64(s6) + (uint64_t)0ULL));
    (a3 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v0, 8))));
L_1382568:;
    if ((int64_t)t0 != 0) goto L_1382588;
    vf5 = a4v::castsi128_ps(A4_R128(ADD32((uint32_t)a1, 0)));
    vf6 = a4v::castsi128_ps(A4_R128(ADD32((uint32_t)a0, 0)));
    { a4::vec4 res = a4v::add(vf5, vf6); a4::q128 mask = a4v::set_epi32(0, -1, -1, -1); vf5 = a4v::blend(vf5, res, a4v::castsi128_ps(mask)); }
    A4_W128(ADD32((uint32_t)v1, 0), a4v::castps_si128(vf5));
    (t0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)t0, 1))));
    goto L_138259c;
L_1382588:;
    vf5 = a4v::castsi128_ps(A4_R128(ADD32((uint32_t)a1, 0)));
    vf6 = a4v::castsi128_ps(A4_R128(ADD32((uint32_t)a0, 0)));
    { a4::vec4 res = a4v::sub(vf5, vf6); a4::q128 mask = a4v::set_epi32(0, -1, -1, -1); vf5 = a4v::blend(vf5, res, a4v::castsi128_ps(mask)); }
    A4_W128(ADD32((uint32_t)a2, 0), a4v::castps_si128(vf5));
    (t0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)t0, 1))));
L_138259c:;
    { float f = f2; uint32_t bits; std::memcpy(&bits, &f, sizeof(bits)); A1_W32B((uint32_t)a3, 0, bits); }
    (v0 = (uint64_t)(((int64_t)(int64_t)t0 < (int64_t)(int32_t)2) ? 1 : 0));
    (a2 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)a2, 16))));
    { const bool c = (int64_t)v0 != 0; (a3 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)a3, 16)))); if (c) { A3_BACK(); goto L_1382568; } }
    do { const a4::q128 _v = (A4_R128(ADD32((uint32_t)sp, 16))); h2 = a4::hi64(_v); std::memcpy(&v0, &_v, 8); } while (0);
    (t6 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)t6, 1))));
    do { const a4::q128 _v = (A4_R128(ADD32((uint32_t)sp, 48))); h3 = a4::hi64(_v); std::memcpy(&v1, &_v, 8); } while (0);
    (a0 = (uint64_t)(((int64_t)(int64_t)RC_U64X(t6) < (int64_t)(int64_t)s2) ? 1 : 0));
    A4_W128(ADD32((uint32_t)sp, 32), a4::mk128(h2, v0));
    f2 = ((float)(f1));
    A4_W128(ADD32((uint32_t)sp, 16), a4::mk128(h3, v1));
    (t8 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)t8, 3))));
    A1_W32B((uint32_t)sp, 40, 0u);
    (s0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)s0, 2))));
    { const bool c = (int64_t)a0 != 0; (t1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)t1, 16)))); if (c) { A3_BACK(); goto L_1382468; } }
L_13825e0:;
    { (sp = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)sp, 256)))); if (xo) { xo[0] = a2; xo[1] = a3; xo[2] = t0; xo[3] = t1; if (a3xw4) xo[4] = a1_sx64(t2); if (a3xw5) xo[5] = t3; } io_vf5 = vf5; io_vf6 = vf6; io_vf7 = vf7; io_vf8 = vf8; io_vf10 = vf10; io_vf14 = vf14; io_vf15 = vf15; io_vf16 = vf16; io_vf17 = vf17; io_vacc = vacc; io_vq = vq; io_vstat = vstat; io_h2 = h2; io_h3 = h3; io_h6 = h6; io_h7 = h7; io_h8 = h8; io_h9 = h9; return a1::RetVF{(uint64_t)v0, f0}; }
    __builtin_unreachable();
}

// gamecore_sub_01386EF8 (0x1386ef8): typed by tools/typed_lift2.py (round A6; loop guest frame calls, extra outputs xo[0]=a0 xo[1]=a1 xo[2]=a2 xo[3]=a3 xo[4]=t0 xo[5]=t1 xo[6]=t2 xo[7]=t3; VU0 in -, out h2 h3, relocated calls 5)
inline uint64_t gamecore_sub_01386EF8(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint64_t a0, uint64_t a1, uint64_t sp, float f12, float f13, float f14, float f15, float f16, float f17, float f18, float f19, uint64_t &io_h2, uint64_t &io_h3, uint64_t *xo = nullptr)
{
    uint64_t v0 = 0;
    uint64_t v1 = 0;
    uint64_t a2 = 0;
    uint64_t a3 = 0;
    uint64_t t0 = 0;
    uint64_t t1 = 0;
    uint64_t t2 = 0;
    uint64_t t3 = 0;
    uint64_t t4 = 0;
    uint64_t t5 = 0;
    uint64_t t6 = 0;
    uint64_t s0 = 0;
    uint64_t s1 = 0;
    uint64_t s2 = 0;
    uint64_t s3 = 0;
    uint64_t s4 = 0;
    uint64_t s5 = 0;
    uint64_t s6 = 0;
    uint64_t h2 = io_h2;
    uint64_t h3 = io_h3;
    (sp = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)sp, 4294967168))));
    { const uint32_t _dynInsn = A1_R32(0x1386EFCu); const int32_t _dynSimm = (int16_t)(_dynInsn & 0xFFFFu); const uint32_t _dynImm = _dynInsn & 0xFFFFu; (void)_dynSimm; (void)_dynImm; (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)_dynImm << 16)))); }
    { const uint32_t _dynInsn = A1_R32(0x1386F00u); const int32_t _dynSimm = (int16_t)(_dynInsn & 0xFFFFu); const uint32_t _dynImm = _dynInsn & 0xFFFFu; (void)_dynSimm; (void)_dynImm; (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)_dynImm << 16)))); }
    { const uint32_t _dynInsn = A1_R32(0x1386F04u); const int32_t _dynSimm = (int16_t)(_dynInsn & 0xFFFFu); const uint32_t _dynImm = _dynInsn & 0xFFFFu; (void)_dynSimm; (void)_dynImm; (a2 = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)_dynImm << 16)))); }
    { const uint32_t _dynInsn = A1_R32(0x1386F08u); const int32_t _dynSimm = (int16_t)(_dynInsn & 0xFFFFu); const uint32_t _dynImm = _dynInsn & 0xFFFFu; (void)_dynSimm; (void)_dynImm; (a3 = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)_dynImm << 16)))); }
    { const uint32_t _dynInsn = A1_R32(0x1386F0Cu); const int32_t _dynSimm = (int16_t)(_dynInsn & 0xFFFFu); const uint32_t _dynImm = _dynInsn & 0xFFFFu; (void)_dynSimm; (void)_dynImm; (t0 = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)_dynImm << 16)))); }
    { const uint32_t _dynInsn = A1_R32(0x1386F10u); const int32_t _dynSimm = (int16_t)(_dynInsn & 0xFFFFu); const uint32_t _dynImm = _dynInsn & 0xFFFFu; (void)_dynSimm; (void)_dynImm; (t1 = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)_dynImm << 16)))); }
    { const uint32_t _dynInsn = A1_R32(0x1386F14u); const int32_t _dynSimm = (int16_t)(_dynInsn & 0xFFFFu); const uint32_t _dynImm = _dynInsn & 0xFFFFu; (void)_dynSimm; (void)_dynImm; (t2 = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)_dynImm << 16)))); }
    { const uint32_t _dynInsn = A1_R32(0x1386F18u); const int32_t _dynSimm = (int16_t)(_dynInsn & 0xFFFFu); const uint32_t _dynImm = _dynInsn & 0xFFFFu; (void)_dynSimm; (void)_dynImm; (t3 = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)_dynImm << 16)))); }
    (s3 = (uint64_t)((uint64_t)a1 + (uint64_t)0ULL));
    { const uint32_t _dynInsn = A1_R32(0x1386F28u); const int32_t _dynSimm = (int16_t)(_dynInsn & 0xFFFFu); const uint32_t _dynImm = _dynInsn & 0xFFFFu; (void)_dynSimm; (void)_dynImm; (t5 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32(ADD32((uint32_t)v0, _dynSimm))))); }
    (s1 = (uint64_t)((uint64_t)a0 + (uint64_t)0ULL));
    { const uint32_t _dynInsn = A1_R32(0x1386F30u); const int32_t _dynSimm = (int16_t)(_dynInsn & 0xFFFFu); const uint32_t _dynImm = _dynInsn & 0xFFFFu; (void)_dynSimm; (void)_dynImm; (t6 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32(ADD32((uint32_t)v1, _dynSimm))))); }
    { const uint32_t _dynInsn = A1_R32(0x1386F34u); const int32_t _dynSimm = (int16_t)(_dynInsn & 0xFFFFu); const uint32_t _dynImm = _dynInsn & 0xFFFFu; (void)_dynSimm; (void)_dynImm; (t4 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32(ADD32((uint32_t)a2, _dynSimm))))); }
    { const uint32_t _dynInsn = A1_R32(0x1386F38u); const int32_t _dynSimm = (int16_t)(_dynInsn & 0xFFFFu); const uint32_t _dynImm = _dynInsn & 0xFFFFu; (void)_dynSimm; (void)_dynImm; (a1 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32(ADD32((uint32_t)t0, _dynSimm))))); }
    { const uint32_t _dynInsn = A1_R32(0x1386F3Cu); const int32_t _dynSimm = (int16_t)(_dynInsn & 0xFFFFu); const uint32_t _dynImm = _dynInsn & 0xFFFFu; (void)_dynSimm; (void)_dynImm; (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32(ADD32((uint32_t)t1, _dynSimm))))); }
    { const uint32_t _dynInsn = A1_R32(0x1386F40u); const int32_t _dynSimm = (int16_t)(_dynInsn & 0xFFFFu); const uint32_t _dynImm = _dynInsn & 0xFFFFu; (void)_dynSimm; (void)_dynImm; (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32(ADD32((uint32_t)t2, _dynSimm))))); }
    { const uint32_t _dynInsn = A1_R32(0x1386F44u); const int32_t _dynSimm = (int16_t)(_dynInsn & 0xFFFFu); const uint32_t _dynImm = _dynInsn & 0xFFFFu; (void)_dynSimm; (void)_dynImm; (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32(ADD32((uint32_t)t3, _dynSimm))))); }
    { const uint32_t _dynInsn = A1_R32(0x1386F48u); const int32_t _dynSimm = (int16_t)(_dynInsn & 0xFFFFu); const uint32_t _dynImm = _dynInsn & 0xFFFFu; (void)_dynSimm; (void)_dynImm; (a2 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32(ADD32((uint32_t)a3, _dynSimm))))); }
    (s0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)s1, 28))));
    (s2 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL));
    (s4 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 3))));
    { const uint32_t _dynInsn = A1_R32(0x1386F68u); const int32_t _dynSimm = (int16_t)(_dynInsn & 0xFFFFu); const uint32_t _dynImm = _dynInsn & 0xFFFFu; (void)_dynSimm; (void)_dynImm; (s5 = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)_dynImm << 16)))); }
    { const uint32_t _dynInsn = A1_R32(0x1386F70u); const int32_t _dynSimm = (int16_t)(_dynInsn & 0xFFFFu); const uint32_t _dynImm = _dynInsn & 0xFFFFu; (void)_dynSimm; (void)_dynImm; (s6 = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)_dynImm << 16)))); }
    A1_W32B((uint32_t)sp, 32, (uint32_t)t5);
    A1_W32B((uint32_t)sp, 36, (uint32_t)t6);
    A1_W32B((uint32_t)sp, 40, (uint32_t)t4);
    A1_W32B((uint32_t)sp, 44, (uint32_t)a2);
    A1_W32B((uint32_t)sp, 48, (uint32_t)a1);
    A1_W32B((uint32_t)sp, 52, (uint32_t)a0);
    A1_W32B((uint32_t)sp, 56, (uint32_t)v1);
    A1_W32B((uint32_t)sp, 60, (uint32_t)v0);
    { uint32_t addr = ADD32((uint32_t)sp, 39); uint32_t aligned_addr = addr & ~7u; uint32_t offset = addr & 7u; uint64_t mem = A1_R64(aligned_addr); uint32_t shift = (7u - offset) << 3; uint64_t keepMask = (shift == 0) ? 0ull : ((1ull << shift) - 1ull); (v0 = (uint64_t)((RC_U64X(v0) & keepMask) | (mem << shift))); }
    { uint32_t addr = ADD32((uint32_t)sp, 32); uint32_t aligned_addr = addr & ~7u; uint32_t offset = addr & 7u; uint64_t mem = A1_R64(aligned_addr); uint32_t shift = offset << 3; uint64_t keepMask = (offset == 0) ? 0ull : (0xFFFFFFFFFFFFFFFFull << ((8u - offset) << 3)); (v0 = (uint64_t)((v0 & keepMask) | (mem >> shift))); }
    { uint32_t addr = ADD32((uint32_t)sp, 47); uint32_t aligned_addr = addr & ~7u; uint32_t offset = addr & 7u; uint64_t mem = A1_R64(aligned_addr); uint32_t shift = (7u - offset) << 3; uint64_t keepMask = (shift == 0) ? 0ull : ((1ull << shift) - 1ull); (v1 = (uint64_t)((RC_U64X(v1) & keepMask) | (mem << shift))); }
    { uint32_t addr = ADD32((uint32_t)sp, 40); uint32_t aligned_addr = addr & ~7u; uint32_t offset = addr & 7u; uint64_t mem = A1_R64(aligned_addr); uint32_t shift = offset << 3; uint64_t keepMask = (offset == 0) ? 0ull : (0xFFFFFFFFFFFFFFFFull << ((8u - offset) << 3)); (v1 = (uint64_t)((v1 & keepMask) | (mem >> shift))); }
    { uint32_t addr = ADD32((uint32_t)sp, 55); uint32_t aligned_addr = addr & ~7u; uint32_t offset = addr & 7u; uint64_t mem = A1_R64(aligned_addr); uint32_t shift = (7u - offset) << 3; uint64_t keepMask = (shift == 0) ? 0ull : ((1ull << shift) - 1ull); (a0 = (uint64_t)((RC_U64X(a0) & keepMask) | (mem << shift))); }
    { uint32_t addr = ADD32((uint32_t)sp, 48); uint32_t aligned_addr = addr & ~7u; uint32_t offset = addr & 7u; uint64_t mem = A1_R64(aligned_addr); uint32_t shift = offset << 3; uint64_t keepMask = (offset == 0) ? 0ull : (0xFFFFFFFFFFFFFFFFull << ((8u - offset) << 3)); (a0 = (uint64_t)((a0 & keepMask) | (mem >> shift))); }
    { uint32_t addr = ADD32((uint32_t)sp, 63); uint32_t aligned_addr = addr & ~7u; uint32_t offset = addr & 7u; uint64_t mem = A1_R64(aligned_addr); uint32_t shift = (7u - offset) << 3; uint64_t keepMask = (shift == 0) ? 0ull : ((1ull << shift) - 1ull); (a1 = (uint64_t)((RC_U64X(a1) & keepMask) | (mem << shift))); }
    { uint32_t addr = ADD32((uint32_t)sp, 56); uint32_t aligned_addr = addr & ~7u; uint32_t offset = addr & 7u; uint64_t mem = A1_R64(aligned_addr); uint32_t shift = offset << 3; uint64_t keepMask = (offset == 0) ? 0ull : (0xFFFFFFFFFFFFFFFFull << ((8u - offset) << 3)); (a1 = (uint64_t)((a1 & keepMask) | (mem >> shift))); }
    { uint32_t addr = ADD32((uint32_t)sp, 7); uint32_t aligned_addr = addr & ~7u; uint32_t offset = addr & 7u; uint32_t shift = (7u - offset) << 3; uint64_t mask = 0xFFFFFFFFFFFFFFFFull >> shift; uint64_t old_data = A1_R64(aligned_addr); uint64_t val = v0; uint64_t new_data = (old_data & ~mask) | ((val >> shift) & mask); A1_W64(aligned_addr, new_data); }
    { uint32_t addr = ADD32((uint32_t)sp, 0); uint32_t aligned_addr = addr & ~7u; uint32_t offset = addr & 7u; uint32_t shift = offset << 3; uint64_t mask = 0xFFFFFFFFFFFFFFFFull << shift; uint64_t old_data = A1_R64(aligned_addr); uint64_t val = v0; uint64_t new_data = (old_data & ~mask) | ((val << shift) & mask); A1_W64(aligned_addr, new_data); }
    { uint32_t addr = ADD32((uint32_t)sp, 15); uint32_t aligned_addr = addr & ~7u; uint32_t offset = addr & 7u; uint32_t shift = (7u - offset) << 3; uint64_t mask = 0xFFFFFFFFFFFFFFFFull >> shift; uint64_t old_data = A1_R64(aligned_addr); uint64_t val = v1; uint64_t new_data = (old_data & ~mask) | ((val >> shift) & mask); A1_W64(aligned_addr, new_data); }
    { uint32_t addr = ADD32((uint32_t)sp, 8); uint32_t aligned_addr = addr & ~7u; uint32_t offset = addr & 7u; uint32_t shift = offset << 3; uint64_t mask = 0xFFFFFFFFFFFFFFFFull << shift; uint64_t old_data = A1_R64(aligned_addr); uint64_t val = v1; uint64_t new_data = (old_data & ~mask) | ((val << shift) & mask); A1_W64(aligned_addr, new_data); }
    { uint32_t addr = ADD32((uint32_t)sp, 23); uint32_t aligned_addr = addr & ~7u; uint32_t offset = addr & 7u; uint32_t shift = (7u - offset) << 3; uint64_t mask = 0xFFFFFFFFFFFFFFFFull >> shift; uint64_t old_data = A1_R64(aligned_addr); uint64_t val = a0; uint64_t new_data = (old_data & ~mask) | ((val >> shift) & mask); A1_W64(aligned_addr, new_data); }
    { uint32_t addr = ADD32((uint32_t)sp, 16); uint32_t aligned_addr = addr & ~7u; uint32_t offset = addr & 7u; uint32_t shift = offset << 3; uint64_t mask = 0xFFFFFFFFFFFFFFFFull << shift; uint64_t old_data = A1_R64(aligned_addr); uint64_t val = a0; uint64_t new_data = (old_data & ~mask) | ((val << shift) & mask); A1_W64(aligned_addr, new_data); }
    { uint32_t addr = ADD32((uint32_t)sp, 31); uint32_t aligned_addr = addr & ~7u; uint32_t offset = addr & 7u; uint32_t shift = (7u - offset) << 3; uint64_t mask = 0xFFFFFFFFFFFFFFFFull >> shift; uint64_t old_data = A1_R64(aligned_addr); uint64_t val = a1; uint64_t new_data = (old_data & ~mask) | ((val >> shift) & mask); A1_W64(aligned_addr, new_data); }
    { uint32_t addr = ADD32((uint32_t)sp, 24); uint32_t aligned_addr = addr & ~7u; uint32_t offset = addr & 7u; uint32_t shift = offset << 3; uint64_t mask = 0xFFFFFFFFFFFFFFFFull << shift; uint64_t old_data = A1_R64(aligned_addr); uint64_t val = a1; uint64_t new_data = (old_data & ~mask) | ((val << shift) & mask); A1_W64(aligned_addr, new_data); }
L_1386fd8:;
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)s0, 0))));
    if ((int64_t)v0 == 0) { (s2 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)s2, 1)))); goto L_138705c; }
    { const bool c = (int64_t)s3 == 0; { const uint32_t _dynInsn = A1_R32(0x1386FE8u); const int32_t _dynSimm = (int16_t)(_dynInsn & 0xFFFFu); const uint32_t _dynImm = _dynInsn & 0xFFFFu; (void)_dynSimm; (void)_dynImm; (a1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)s6, _dynSimm)))); } if (c) goto L_1387024; }
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)s0, 4))));
    { const uint32_t _dynInsn = A1_R32(0x1386FF0u); const int32_t _dynSimm = (int16_t)(_dynInsn & 0xFFFFu); const uint32_t _dynImm = _dynInsn & 0xFFFFu; (void)_dynSimm; (void)_dynImm; (a2 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)s5, _dynSimm)))); }
    { (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)a0, 208)))); A3_CHARGE(); v0 = SetGeometryPointerGlobalState(ram, rt, (uint32_t)a0, (uint32_t)a1, (uint32_t)a2, h2, h3); }
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)s1, 0))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)SLL32((uint32_t)v0, 2))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v0, (uint32_t)s2))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)SLL32((uint32_t)v0, 2))));
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)sp, (uint32_t)v0))));
    { (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)v1, 0)))); A3_CHARGE(); v0 = get_particle_effect_id_from_sheet(ram, rt, a0); }
    (a1 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)s0, 0))));
    { (a0 = (uint64_t)((uint64_t)v0 + (uint64_t)0ULL)); A3_CHARGE(); v0 = stop_particle_emitter_geometry_pointer_reference(ram, rt, v0, a0, a1, f12, f13, f14, f15, f16, f17, f18, f19); }
L_1387024:;
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)s1, 0))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)SLL32((uint32_t)v0, 2))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v0, (uint32_t)s2))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)SLL32((uint32_t)v0, 2))));
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)sp, (uint32_t)v0))));
    { (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)v1, 0)))); A3_CHARGE(); v0 = get_particle_effect_id_from_sheet(ram, rt, a0); }
    (a1 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)s0, 0))));
    (a0 = (uint64_t)((uint64_t)v0 + (uint64_t)0ULL));
    { (a2 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL)); A3_CHARGE(); uint64_t a7c[1] = {0u}; v0 = end_particle_emitter_x8(ram, rt, v0, a1, a2, f12, f13, f14, f15, f16, f17, f18, f19, a7c); a0 = a7c[0]; }
    A1_W32B((uint32_t)s0, 8, (uint32_t)s4);
    A1_W32B((uint32_t)s0, 12, (uint32_t)s1);
    (s2 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)s2, 1))));
L_138705c:;
    (v0 = (uint64_t)(((int64_t)(int64_t)s2 < (int64_t)(int32_t)4) ? 1 : 0));
    { const bool c = (int64_t)v0 != 0; (s0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)s0, 16)))); if (c) { A3_BACK(); goto L_1386fd8; } }
    { (sp = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)sp, 128)))); if (xo) { xo[0] = a0; xo[1] = a1; xo[2] = a2; xo[3] = a3; xo[4] = t0; xo[5] = t1; xo[6] = t2; xo[7] = t3; } io_h2 = h2; io_h3 = h3; return v0; }
    __builtin_unreachable();
}

// gamecore_sub_01387090 (0x1387090): typed by tools/typed_lift2.py (round A6; loop guest frame calls, extra outputs xo[0]=f12 xo[1]=f20 xo[2]=a0 xo[3]=a1 xo[4]=a2 xo[5]=a3 xo[6]=t0 xo[7]=t1 xo[8]=t2 xo[9]=t3; VU0 in -, out -, relocated calls 1)
inline a1::RetVF gamecore_sub_01387090(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint64_t a0, uint64_t sp, float f13, float f14, float f15, float f16, float f17, float f18, float f19, float f20, uint64_t *xo = nullptr)
{
    float f0 = 0.0f;
    float f1 = 0.0f;
    float f2 = 0.0f;
    float f3 = 0.0f;
    float f4 = 0.0f;
    float f12 = 0.0f;
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
    uint64_t t4 = 0;
    uint64_t s0 = 0;
    uint64_t s1 = 0;
    uint64_t s2 = 0;
    uint64_t s3 = 0;
    uint64_t s4 = 0;
    uint32_t fcr31 = 0;
    bool a3xw0 = false;
    (sp = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)sp, 4294967168))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)326 << 16))));
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)326 << 16))));
    (s1 = (uint64_t)((uint64_t)a0 + (uint64_t)0ULL));
    (s2 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL));
    (s3 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 2))));
    (s4 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL));
    { float f = f20; uint32_t bits; std::memcpy(&bits, &f, sizeof(bits)); A1_W32B((uint32_t)sp, 112, bits); }
    { const uint32_t _dynInsn = A1_R32(0x13870C4u); const int32_t _dynSimm = (int16_t)(_dynInsn & 0xFFFFu); const uint32_t _dynImm = _dynInsn & 0xFFFFu; (void)_dynSimm; (void)_dynImm; (t0 = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)_dynImm << 16)))); }
    { const uint32_t _dynInsn = A1_R32(0x13870CCu); const int32_t _dynSimm = (int16_t)(_dynInsn & 0xFFFFu); const uint32_t _dynImm = _dynInsn & 0xFFFFu; (void)_dynSimm; (void)_dynImm; (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)_dynImm << 16)))); }
    { uint32_t bits = A1_R32B((uint32_t)v0, 22480); float f; std::memcpy(&f, &bits, sizeof(f)); f2 = f; }
    { const uint32_t _dynInsn = A1_R32(0x13870D4u); const int32_t _dynSimm = (int16_t)(_dynInsn & 0xFFFFu); const uint32_t _dynImm = _dynInsn & 0xFFFFu; (void)_dynSimm; (void)_dynImm; (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)_dynImm << 16)))); }
    { uint32_t bits = A1_R32B((uint32_t)s1, 20); float f; std::memcpy(&f, &bits, sizeof(f)); f4 = f; }
    { const uint32_t _dynInsn = A1_R32(0x13870DCu); const int32_t _dynSimm = (int16_t)(_dynInsn & 0xFFFFu); const uint32_t _dynImm = _dynInsn & 0xFFFFu; (void)_dynSimm; (void)_dynImm; (a1 = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)_dynImm << 16)))); }
    { uint32_t bits = A1_R32B((uint32_t)s1, 16); float f; std::memcpy(&f, &bits, sizeof(f)); f0 = f; }
    { const uint32_t _dynInsn = A1_R32(0x13870E4u); const int32_t _dynSimm = (int16_t)(_dynInsn & 0xFFFFu); const uint32_t _dynImm = _dynInsn & 0xFFFFu; (void)_dynSimm; (void)_dynImm; (a2 = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)_dynImm << 16)))); }
    { uint32_t bits = A1_R32B((uint32_t)v1, 26340); float f; std::memcpy(&f, &bits, sizeof(f)); f3 = f; }
    { const uint32_t _dynInsn = A1_R32(0x13870ECu); const int32_t _dynSimm = (int16_t)(_dynInsn & 0xFFFFu); const uint32_t _dynImm = _dynInsn & 0xFFFFu; (void)_dynSimm; (void)_dynImm; (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)_dynImm << 16)))); }
    f0 = a1f::sub(f0, f4);
    (at = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)16880 << 16))));
    { uint32_t bits = (uint32_t)at; std::memcpy(&f1, &bits, sizeof(bits)); }
    { const uint32_t _dynInsn = A1_R32(0x13870FCu); const int32_t _dynSimm = (int16_t)(_dynInsn & 0xFFFFu); const uint32_t _dynImm = _dynInsn & 0xFFFFu; (void)_dynSimm; (void)_dynImm; (a3 = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)_dynImm << 16)))); }
    { const uint32_t _dynInsn = A1_R32(0x1387100u); const int32_t _dynSimm = (int16_t)(_dynInsn & 0xFFFFu); const uint32_t _dynImm = _dynInsn & 0xFFFFu; (void)_dynSimm; (void)_dynImm; (t1 = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)_dynImm << 16)))); }
    f2 = a1f::mul(f2, f1);
    { const uint32_t _dynInsn = A1_R32(0x1387108u); const int32_t _dynSimm = (int16_t)(_dynInsn & 0xFFFFu); const uint32_t _dynImm = _dynInsn & 0xFFFFu; (void)_dynSimm; (void)_dynImm; (t2 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32(ADD32((uint32_t)t0, _dynSimm))))); }
    { const uint32_t _dynInsn = A1_R32(0x138710Cu); const int32_t _dynSimm = (int16_t)(_dynInsn & 0xFFFFu); const uint32_t _dynImm = _dynInsn & 0xFFFFu; (void)_dynSimm; (void)_dynImm; (t4 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32(ADD32((uint32_t)a0, _dynSimm))))); }
    (s0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)s1, 28))));
    f0 = a1f::mul(f0, f3);
    { const uint32_t _dynInsn = A1_R32(0x1387118u); const int32_t _dynSimm = (int16_t)(_dynInsn & 0xFFFFu); const uint32_t _dynImm = _dynInsn & 0xFFFFu; (void)_dynSimm; (void)_dynImm; (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32(ADD32((uint32_t)v1, _dynSimm))))); }
    { const uint32_t _dynInsn = A1_R32(0x138711Cu); const int32_t _dynSimm = (int16_t)(_dynInsn & 0xFFFFu); const uint32_t _dynImm = _dynInsn & 0xFFFFu; (void)_dynSimm; (void)_dynImm; (t3 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32(ADD32((uint32_t)v0, _dynSimm))))); }
    { const uint32_t _dynInsn = A1_R32(0x1387120u); const int32_t _dynSimm = (int16_t)(_dynInsn & 0xFFFFu); const uint32_t _dynImm = _dynInsn & 0xFFFFu; (void)_dynSimm; (void)_dynImm; (t0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32(ADD32((uint32_t)a1, _dynSimm))))); }
    { const uint32_t _dynInsn = A1_R32(0x1387124u); const int32_t _dynSimm = (int16_t)(_dynInsn & 0xFFFFu); const uint32_t _dynImm = _dynInsn & 0xFFFFu; (void)_dynSimm; (void)_dynImm; (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32(ADD32((uint32_t)a3, _dynSimm))))); }
    f0 = ((float)(f0) * (float)(f2));
    { const uint32_t _dynInsn = A1_R32(0x138712Cu); const int32_t _dynSimm = (int16_t)(_dynInsn & 0xFFFFu); const uint32_t _dynImm = _dynInsn & 0xFFFFu; (void)_dynSimm; (void)_dynImm; (a1 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32(ADD32((uint32_t)a2, _dynSimm))))); }
    { const uint32_t _dynInsn = A1_R32(0x1387130u); const int32_t _dynSimm = (int16_t)(_dynInsn & 0xFFFFu); const uint32_t _dynImm = _dynInsn & 0xFFFFu; (void)_dynSimm; (void)_dynImm; (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32(ADD32((uint32_t)t1, _dynSimm))))); }
    A1_W32B((uint32_t)sp, 32, (uint32_t)t2);
    A1_W32B((uint32_t)sp, 36, (uint32_t)t4);
    f4 = a1f::add(f4, f0);
    A1_W32B((uint32_t)sp, 40, (uint32_t)t3);
    A1_W32B((uint32_t)sp, 44, (uint32_t)a0);
    A1_W32B((uint32_t)sp, 48, (uint32_t)t0);
    A1_W32B((uint32_t)sp, 52, (uint32_t)a1);
    A1_W32B((uint32_t)sp, 56, (uint32_t)v1);
    A1_W32B((uint32_t)sp, 60, (uint32_t)v0);
    { float f = f4; uint32_t bits; std::memcpy(&bits, &f, sizeof(bits)); A1_W32B((uint32_t)s1, 20, bits); }
    (at = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)16256 << 16))));
    { uint32_t bits = (uint32_t)at; std::memcpy(&f20, &bits, sizeof(bits)); }
    { uint32_t addr = ADD32((uint32_t)sp, 39); uint32_t aligned_addr = addr & ~7u; uint32_t offset = addr & 7u; uint64_t mem = A1_R64(aligned_addr); uint32_t shift = (7u - offset) << 3; uint64_t keepMask = (shift == 0) ? 0ull : ((1ull << shift) - 1ull); (v0 = (uint64_t)((RC_U64X(v0) & keepMask) | (mem << shift))); }
    { uint32_t addr = ADD32((uint32_t)sp, 32); uint32_t aligned_addr = addr & ~7u; uint32_t offset = addr & 7u; uint64_t mem = A1_R64(aligned_addr); uint32_t shift = offset << 3; uint64_t keepMask = (offset == 0) ? 0ull : (0xFFFFFFFFFFFFFFFFull << ((8u - offset) << 3)); (v0 = (uint64_t)((v0 & keepMask) | (mem >> shift))); }
    { uint32_t addr = ADD32((uint32_t)sp, 47); uint32_t aligned_addr = addr & ~7u; uint32_t offset = addr & 7u; uint64_t mem = A1_R64(aligned_addr); uint32_t shift = (7u - offset) << 3; uint64_t keepMask = (shift == 0) ? 0ull : ((1ull << shift) - 1ull); (v1 = (uint64_t)((RC_U64X(v1) & keepMask) | (mem << shift))); }
    { uint32_t addr = ADD32((uint32_t)sp, 40); uint32_t aligned_addr = addr & ~7u; uint32_t offset = addr & 7u; uint64_t mem = A1_R64(aligned_addr); uint32_t shift = offset << 3; uint64_t keepMask = (offset == 0) ? 0ull : (0xFFFFFFFFFFFFFFFFull << ((8u - offset) << 3)); (v1 = (uint64_t)((v1 & keepMask) | (mem >> shift))); }
    { uint32_t addr = ADD32((uint32_t)sp, 55); uint32_t aligned_addr = addr & ~7u; uint32_t offset = addr & 7u; uint64_t mem = A1_R64(aligned_addr); uint32_t shift = (7u - offset) << 3; uint64_t keepMask = (shift == 0) ? 0ull : ((1ull << shift) - 1ull); (a0 = (uint64_t)((RC_U64X(a0) & keepMask) | (mem << shift))); }
    { uint32_t addr = ADD32((uint32_t)sp, 48); uint32_t aligned_addr = addr & ~7u; uint32_t offset = addr & 7u; uint64_t mem = A1_R64(aligned_addr); uint32_t shift = offset << 3; uint64_t keepMask = (offset == 0) ? 0ull : (0xFFFFFFFFFFFFFFFFull << ((8u - offset) << 3)); (a0 = (uint64_t)((a0 & keepMask) | (mem >> shift))); }
    { uint32_t addr = ADD32((uint32_t)sp, 63); uint32_t aligned_addr = addr & ~7u; uint32_t offset = addr & 7u; uint64_t mem = A1_R64(aligned_addr); uint32_t shift = (7u - offset) << 3; uint64_t keepMask = (shift == 0) ? 0ull : ((1ull << shift) - 1ull); (a1 = (uint64_t)((RC_U64X(a1) & keepMask) | (mem << shift))); }
    { uint32_t addr = ADD32((uint32_t)sp, 56); uint32_t aligned_addr = addr & ~7u; uint32_t offset = addr & 7u; uint64_t mem = A1_R64(aligned_addr); uint32_t shift = offset << 3; uint64_t keepMask = (offset == 0) ? 0ull : (0xFFFFFFFFFFFFFFFFull << ((8u - offset) << 3)); (a1 = (uint64_t)((a1 & keepMask) | (mem >> shift))); }
    { uint32_t addr = ADD32((uint32_t)sp, 7); uint32_t aligned_addr = addr & ~7u; uint32_t offset = addr & 7u; uint32_t shift = (7u - offset) << 3; uint64_t mask = 0xFFFFFFFFFFFFFFFFull >> shift; uint64_t old_data = A1_R64(aligned_addr); uint64_t val = v0; uint64_t new_data = (old_data & ~mask) | ((val >> shift) & mask); A1_W64(aligned_addr, new_data); }
    { uint32_t addr = ADD32((uint32_t)sp, 0); uint32_t aligned_addr = addr & ~7u; uint32_t offset = addr & 7u; uint32_t shift = offset << 3; uint64_t mask = 0xFFFFFFFFFFFFFFFFull << shift; uint64_t old_data = A1_R64(aligned_addr); uint64_t val = v0; uint64_t new_data = (old_data & ~mask) | ((val << shift) & mask); A1_W64(aligned_addr, new_data); }
    { uint32_t addr = ADD32((uint32_t)sp, 15); uint32_t aligned_addr = addr & ~7u; uint32_t offset = addr & 7u; uint32_t shift = (7u - offset) << 3; uint64_t mask = 0xFFFFFFFFFFFFFFFFull >> shift; uint64_t old_data = A1_R64(aligned_addr); uint64_t val = v1; uint64_t new_data = (old_data & ~mask) | ((val >> shift) & mask); A1_W64(aligned_addr, new_data); }
    { uint32_t addr = ADD32((uint32_t)sp, 8); uint32_t aligned_addr = addr & ~7u; uint32_t offset = addr & 7u; uint32_t shift = offset << 3; uint64_t mask = 0xFFFFFFFFFFFFFFFFull << shift; uint64_t old_data = A1_R64(aligned_addr); uint64_t val = v1; uint64_t new_data = (old_data & ~mask) | ((val << shift) & mask); A1_W64(aligned_addr, new_data); }
    { uint32_t addr = ADD32((uint32_t)sp, 23); uint32_t aligned_addr = addr & ~7u; uint32_t offset = addr & 7u; uint32_t shift = (7u - offset) << 3; uint64_t mask = 0xFFFFFFFFFFFFFFFFull >> shift; uint64_t old_data = A1_R64(aligned_addr); uint64_t val = a0; uint64_t new_data = (old_data & ~mask) | ((val >> shift) & mask); A1_W64(aligned_addr, new_data); }
    { uint32_t addr = ADD32((uint32_t)sp, 16); uint32_t aligned_addr = addr & ~7u; uint32_t offset = addr & 7u; uint32_t shift = offset << 3; uint64_t mask = 0xFFFFFFFFFFFFFFFFull << shift; uint64_t old_data = A1_R64(aligned_addr); uint64_t val = a0; uint64_t new_data = (old_data & ~mask) | ((val << shift) & mask); A1_W64(aligned_addr, new_data); }
    { uint32_t addr = ADD32((uint32_t)sp, 31); uint32_t aligned_addr = addr & ~7u; uint32_t offset = addr & 7u; uint32_t shift = (7u - offset) << 3; uint64_t mask = 0xFFFFFFFFFFFFFFFFull >> shift; uint64_t old_data = A1_R64(aligned_addr); uint64_t val = a1; uint64_t new_data = (old_data & ~mask) | ((val >> shift) & mask); A1_W64(aligned_addr, new_data); }
    { uint32_t addr = ADD32((uint32_t)sp, 24); uint32_t aligned_addr = addr & ~7u; uint32_t offset = addr & 7u; uint32_t shift = offset << 3; uint64_t mask = 0xFFFFFFFFFFFFFFFFull << shift; uint64_t old_data = A1_R64(aligned_addr); uint64_t val = a1; uint64_t new_data = (old_data & ~mask) | ((val << shift) & mask); A1_W64(aligned_addr, new_data); }
L_13871a8:;
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)s0, 0))));
    { const bool c = (int64_t)v0 == 0; (a1 = (uint64_t)((uint64_t)RC_U64X(v0) + (uint64_t)0ULL)); if (c) goto L_13871dc; }
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)s1, 0))));
    { uint32_t bits = A1_R32B((uint32_t)s1, 20); float f; std::memcpy(&f, &bits, sizeof(f)); f12 = f; } a3xw0 = true;
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)SLL32((uint32_t)v0, 2))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v0, (uint32_t)s2))));
    f12 = a1f::sub(f20, f12); a3xw0 = true;
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)SLL32((uint32_t)v0, 2))));
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)sp, (uint32_t)v0))));
    { (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)v1, 0)))); A3_CHARGE(); uint64_t a7c[2] = {0u, 0u}; v0 = set_npe_blend_ratio_particle_emitter_x8(ram, rt, v0, a1, f12, f13, f14, f15, f16, f17, f18, f19, f20, a7c); f20 = a1_flt((uint32_t)a7c[0]); a0 = a7c[1]; }
    A1_W32B((uint32_t)s0, 8, (uint32_t)s3);
L_13871dc:;
    (s2 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)s2, 1))));
    (v0 = (uint64_t)(((int64_t)(int64_t)RC_U64X(s2) < (int64_t)(int32_t)4) ? 1 : 0));
    { const bool c = (int64_t)v0 != 0; (s0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)s0, 16)))); if (c) { A3_BACK(); goto L_13871a8; } }
    { uint32_t bits = A1_R32B((uint32_t)s1, 20); float f; std::memcpy(&f, &bits, sizeof(f)); f1 = f; }
    { uint32_t bits = A1_R32B((uint32_t)s1, 16); float f; std::memcpy(&f, &bits, sizeof(f)); f2 = f; }
    { uint32_t bits = 0u; std::memcpy(&f0, &bits, sizeof(bits)); }
    f1 = a1f::sub(f1, f2);
    fcr31 = (a1f::lt(f1, f0)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
    if ((fcr31 & 0x800000u) == 0u) goto L_1387228;
    (at = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)48163 << 16))));
    (at = (uint64_t)(RC_U64X(at) | (uint64_t)(uint16_t)55050));
    { uint32_t bits = (uint32_t)at; std::memcpy(&f0, &bits, sizeof(bits)); }
    fcr31 = (a1f::lt(f0, f1)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
    if ((fcr31 & 0x800000u) != 0u) { { float f = f2; uint32_t bits; std::memcpy(&bits, &f, sizeof(bits)); A1_W32B((uint32_t)s1, 20, bits); } goto L_1387244; }
    (v0 = (uint64_t)((uint64_t)s4 + (uint64_t)0ULL));
    goto L_138724c;
L_1387228:;
    (at = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)15395 << 16))));
    (at = (uint64_t)(RC_U64X(at) | (uint64_t)(uint16_t)55050));
    { uint32_t bits = (uint32_t)at; std::memcpy(&f0, &bits, sizeof(bits)); }
    fcr31 = (a1f::lt(f1, f0)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
    { const bool c = (fcr31 & 0x800000u) == 0u; (v0 = (uint64_t)((uint64_t)s4 + (uint64_t)0ULL)); if (c) goto L_138724c; }
    { float f = f2; uint32_t bits; std::memcpy(&bits, &f, sizeof(bits)); A1_W32B((uint32_t)s1, 20, bits); }
L_1387244:;
    (s4 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 1))));
    (v0 = (uint64_t)((uint64_t)RC_U64X(s4) + (uint64_t)0ULL));
L_138724c:;
    { uint32_t bits = A1_R32B((uint32_t)sp, 112); float f; std::memcpy(&f, &bits, sizeof(f)); f20 = f; }
    { (sp = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)sp, 128)))); if (xo) { if (a3xw0) xo[0] = a1_bits(f12); xo[1] = a1_bits(f20); xo[2] = a0; xo[3] = a1; xo[4] = a2; xo[5] = a3; xo[6] = t0; xo[7] = t1; xo[8] = t2; xo[9] = t3; } return a1::RetVF{(uint64_t)v0, f0}; }
    __builtin_unreachable();
}

// gamecore_sub_0139F470 (0x139f470): typed by tools/typed_lift2.py (round A6; loop frame calls, extra outputs xo[0]=f0 xo[1]=f20 xo[2]=f21 xo[3]=f22 xo[4]=a1 xo[5]=a2 xo[6]=a3 xo[7]=t0 xo[8]=t1; VU0 in h2 h4 h9, out h2 h3 h6, relocated calls 12, frame 144 bytes, 38 slot accesses)
inline uint64_t gamecore_sub_0139F470(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint64_t v0, uint64_t a0, uint64_t a1, uint64_t a2, uint64_t a3, uint64_t t0, float f12, float f13, float f14, float f15, float f16, float f17, float f18, float f19, float f20, float f21, float f22, uint64_t &io_h2, uint64_t &io_h3, uint64_t io_h4, uint64_t &io_h6, uint64_t io_h9, uint64_t *xo = nullptr)
{
    float f0 = 0.0f;
    uint32_t at = 0;
    uint64_t v1 = 0;
    uint32_t t1 = 0;
    uint32_t s0 = 0;
    uint64_t s1 = 0;
    uint64_t s2 = 0;
    uint32_t s3 = 0;
    uint32_t s4 = 0;
    uint64_t s5 = 0;
    uint64_t s6 = 0;
    uint64_t s7 = 0;
    uint64_t fp = 0;
    uint64_t h2 = io_h2;
    uint64_t h3 = io_h3;
    uint64_t h4 = io_h4;
    uint64_t h6 = io_h6;
    uint64_t h9 = io_h9;
    uint8_t a2fr[144];
    bool a3xw0 = false;
    bool a3xw5 = false;
    bool a3xw6 = false;
    bool a3xw7 = false;
    bool a3xw8 = false;
    (s2 = (uint64_t)((uint64_t)a2 + (uint64_t)0ULL));
    (fp = (uint64_t)((uint64_t)a0 + (uint64_t)0ULL));
    a2::frSt<uint32_t>(a2fr, 112, a1_bits(f20));
    f20 = ((float)(f12));
    { float f = f22; uint32_t bits; std::memcpy(&bits, &f, sizeof(bits)); a2::frSt<uint32_t>(a2fr, 128, bits); }
    { float f = f21; uint32_t bits; std::memcpy(&bits, &f, sizeof(bits)); a2::frSt<uint32_t>(a2fr, 120, bits); }
    a2::frSt<uint32_t>(a2fr, 4, (uint32_t)a3);
    a2::frSt<uint32_t>(a2fr, 0, (uint32_t)a1);
    { const bool c = (int64_t)a3 == 0; a2::frSt<uint32_t>(a2fr, 8, (uint32_t)t0); if (c) goto L_139f4e0; }
    { (a0 = (uint64_t)((uint64_t)a3 + (uint64_t)0ULL)); A3_CHARGE(); v0 = MakeStrHashValue(ram, rt, v0, a0, f12, f13, f14, f15, f16, f17, f18, f19, h2); }
    (a0 = (uint64_t)((uint64_t)fp + (uint64_t)0ULL));
    { (a1 = (uint64_t)((uint64_t)v0 + (uint64_t)0ULL)); A3_CHARGE(); uint64_t a7c[3] = {0u, 0u, 0u}; v0 = SpriteMenuGetLayerNoByHash_x8(ram, rt, a0, a1, h3, h4, a7c); a2 = a7c[0]; a3 = a7c[1]; t0 = a7c[2]; a3xw5 = true; a3xw6 = true; a3xw7 = true; }
    (v1 = (uint64_t)((uint64_t)v0 + (uint64_t)0ULL));
    goto L_139f4e4;
L_139f4e0:;
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 4294967295))));
L_139f4e4:;
    (a1 = (uint64_t)((int64_t)(int32_t)((int32_t)a2::frLd<uint32_t>(a2fr, 0))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)SLL32((uint32_t)v1, 4))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v0, (uint32_t)v1))));
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)a1, 4))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)SLL32((uint32_t)v0, 2))));
    a2::frSt<uint32_t>(a2fr, 16, 0u);
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)a0, (uint32_t)v0))));
    a2::frSt<uint32_t>(a2fr, 12, (uint32_t)a0);
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)a0, 8))));
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)SRL32((uint32_t)v1, 4))));
    (v1 = (uint64_t)(RC_U64X(v1) & (uint64_t)(uint16_t)1023));
    { const bool c = (int64_t)v1 == 0; (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)a2::frLd<uint32_t>(a2fr, 4)))); if (c) goto L_139fa04; }
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)16383 << 16))));
    (at = (uint32_t)((int32_t)((int32_t)((uint32_t)16128 << 16))));
    { uint32_t bits = a7_opq(at); std::memcpy(&f22, &bits, sizeof(bits)); }
    (v0 = (uint64_t)(RC_U64X(v0) | (uint64_t)(uint16_t)65535));
    a2::frSt<uint32_t>(a2fr, 28, 0u);
    a2::frSt<uint32_t>(a2fr, 20, (uint32_t)v0);
L_139f530:;
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)a2::frLd<uint32_t>(a2fr, 12))));
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)a2::frLd<uint32_t>(a2fr, 28))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)v1, 4))));
    (s1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v0, (uint32_t)a0))));
    (s0 = (uint32_t)((int32_t)((int32_t)A1_R32B((uint32_t)s1, 0))));
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B(s0, 16))));
    (v0 = (uint64_t)(RC_U64X(v1) & (uint64_t)(uint16_t)16));
    { const bool c = (int64_t)v0 == 0; (v0 = (uint64_t)(RC_U64X(v1) & (uint64_t)(uint16_t)64)); if (c) goto L_139f5b8; }
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B(s0, 20))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)SLL32((uint32_t)v1, 2))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v0, (uint32_t)v1))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)SLL32((uint32_t)v0, 2))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)SUB32((uint32_t)v0, (uint32_t)v1))));
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)SLL32((uint32_t)v0, 3))));
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)a0, (uint32_t)s2))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)v1, 284))));
    (v0 = (uint64_t)(RC_U64X(v0) ^ (uint64_t)(uint16_t)1));
    if ((int64_t)v0 != 0) { (v0 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL)); goto L_139f588; }
    (v0 = (uint64_t)((uint64_t)RC_U64X(v1) + (uint64_t)0ULL));
    (v0 = (uint64_t)((int64_t)(int32_t)((int8_t)A1_R8B((uint32_t)v0, 296))));
L_139f588:;
    A1_W32B(s0, 12, (uint32_t)v0);
    { const bool c = (int64_t)v0 != 0; { float f = f20; uint32_t bits; std::memcpy(&bits, &f, sizeof(bits)); A1_W32B((uint32_t)s1, 20, bits); } if (c) goto L_139f9d0; }
    f0 = a1f::mul(f20, f22); a3xw0 = true;
    { float f = f0; uint32_t bits; std::memcpy(&bits, &f, sizeof(bits)); A1_W32B((uint32_t)s1, 20, bits); }
    (a1 = (uint64_t)((int64_t)(int32_t)((int32_t)a2::frLd<uint32_t>(a2fr, 8))));
    { const bool c = (int64_t)a1 != 0; (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)a2::frLd<uint32_t>(a2fr, 12)))); if (c) goto L_139f9d4; }
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)s1, 24))));
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)a2::frLd<uint32_t>(a2fr, 20))));
    (v0 = (uint64_t)(RC_U64X(v0) & RC_U64X(v1)));
    goto L_139f9cc;
L_139f5b8:;
    { const bool c = (int64_t)v0 == 0; (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B(s0, 20)))); if (c) goto L_139f850; }
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)SLL32((uint32_t)v1, 2))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v0, (uint32_t)v1))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)SLL32((uint32_t)v0, 2))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)SUB32((uint32_t)v0, (uint32_t)v1))));
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)SLL32((uint32_t)v0, 3))));
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)a0, (uint32_t)s2))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)v1, 284))));
    (v0 = (uint64_t)(RC_U64X(v0) ^ (uint64_t)(uint16_t)1));
    { const bool c = (int64_t)v0 != 0; (s5 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)s2, 272)))); if (c) goto L_139f5f8; }
    (v0 = (uint64_t)((uint64_t)RC_U64X(v1) + (uint64_t)0ULL));
    (v0 = (uint64_t)((int64_t)(int32_t)((int8_t)A1_R8B((uint32_t)v0, 296))));
    a2::frSt<uint32_t>(a2fr, 24, (uint32_t)v0);
    goto L_139f5fc;
L_139f5f8:;
    a2::frSt<uint32_t>(a2fr, 24, 0u);
L_139f5fc:;
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)326 << 16))));
    s3 = 0u;
    (s4 = (uint32_t)((int32_t)((int32_t)ADD32((uint32_t)v0, 26504))));
    (s7 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 16))));
    (s6 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 4294967295))));
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B(s4, 0))));
L_139f618:;
    { s4 = s4 + 4u; A3_CHARGE(); v0 = MakeStrHashValue(ram, rt, v0, a0, f12, f13, f14, f15, f16, f17, f18, f19, h2); }
    (a0 = (uint64_t)((uint64_t)fp + (uint64_t)0ULL));
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B(s0, 4))));
    (a1 = (uint64_t)((uint64_t)s1 + (uint64_t)0ULL));
    { const bool c = (int64_t)v1 != (int64_t)v0; (a2 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 2)))); a3xw5 = true; if (c) goto L_139f6ac; }
    (a3 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B(s0, 20)))); a3xw6 = true;
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)SLL32((uint32_t)a3, 2))));
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v1, (uint32_t)a3))));
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)SLL32((uint32_t)v1, 2))));
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)SUB32((uint32_t)v1, (uint32_t)a3))));
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)SLL32((uint32_t)v1, 3))));
    (v0 = (uint64_t)((uint64_t)RC_U64X(v1) + (uint64_t)0ULL));
    (t0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v1, (uint32_t)s5)))); a3xw7 = true;
    (a3 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v0, (uint32_t)s2)))); a3xw6 = true;
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)a3, 284))));
    (v1 = (uint64_t)(RC_U64X(v1) ^ (uint64_t)(uint16_t)1));
    { const bool c = (int64_t)v1 != 0; (v0 = (uint64_t)((uint64_t)RC_U64X(a3) + (uint64_t)0ULL)); if (c) goto L_139f670; }
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)v0, 316))));
    goto L_139f674;
L_139f670:;
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 4294967295))));
L_139f674:;
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)t0, 48))));
    (t0 = (uint64_t)((uint64_t)RC_U64X(v1) + (uint64_t)0ULL)); a3xw7 = true;
    (v1 = (uint64_t)(((int64_t)(int64_t)RC_U64X(v0) < (int64_t)(int32_t)0) ? 1 : 0));
    if (RC_U64X(v1) != 0) do { const a4::q128 _v = (a4v::setzero_si128()); h2 = a4::hi64(_v); std::memcpy(&v0, &_v, 8); } while (0);
    (a3 = (uint64_t)(((int64_t)(int64_t)v0 < (int64_t)(int32_t)17) ? 1 : 0)); a3xw6 = true;
    { const bool c = (int64_t)a3 != 0; A1_W32B(s0, 12, (uint32_t)v0); if (c) goto L_139f694; }
    A1_W32B(s0, 12, (uint32_t)s7);
L_139f694:;
    { const bool c = (int64_t)t0 != (int64_t)s6; { float f = f20; uint32_t bits; std::memcpy(&bits, &f, sizeof(bits)); A1_W32B((uint32_t)s1, 20, bits); } if (c) goto L_139f6a4; }
    f0 = a1f::mul(f20, f22); a3xw0 = true;
    { float f = f0; uint32_t bits; std::memcpy(&bits, &f, sizeof(bits)); A1_W32B((uint32_t)s1, 20, bits); }
L_139f6a4:;
    { A3_CHARGE(); v0 = a1_sx64(SpriteMenuSetNumPartsFormatByPartsParam_x8(ram, rt, (uint32_t)a1, (uint32_t)a2)); }
L_139f6ac:;
    s3 = s3 + 1u;
    (v0 = (uint64_t)(((uint64_t)a1_sx64(s3) < (uint64_t)(int64_t)(int32_t)10) ? 1 : 0));
    if ((int64_t)v0 != 0) { (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B(s4, 0)))); { A3_BACK(); goto L_139f618; } }
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)326 << 16))));
    (at = (uint32_t)((int32_t)((int32_t)((uint32_t)16128 << 16))));
    { uint32_t bits = a7_opq(at); std::memcpy(&f21, &bits, sizeof(bits)); }
    (s4 = (uint32_t)((int32_t)((int32_t)ADD32((uint32_t)v0, 26544))));
    s3 = 0u;
    (s6 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 4294967295))));
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B(s4, 0))));
L_139f6d8:;
    { s4 = s4 + 4u; A3_CHARGE(); v0 = MakeStrHashValue(ram, rt, v0, a0, f12, f13, f14, f15, f16, f17, f18, f19, h2); }
    (a0 = (uint64_t)((uint64_t)fp + (uint64_t)0ULL));
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B(s0, 4))));
    (a1 = (uint64_t)((uint64_t)s1 + (uint64_t)0ULL));
    { const bool c = (int64_t)v1 != (int64_t)v0; (a2 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL)); a3xw5 = true; if (c) goto L_139f74c; }
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B(s0, 20))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)SLL32((uint32_t)v1, 2))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v0, (uint32_t)v1))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)SLL32((uint32_t)v0, 2))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)SUB32((uint32_t)v0, (uint32_t)v1))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)SLL32((uint32_t)v0, 3))));
    (a3 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v0, (uint32_t)s2)))); a3xw6 = true;
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)a3, 284))));
    (v1 = (uint64_t)(RC_U64X(v1) ^ (uint64_t)(uint16_t)1));
    { const bool c = (int64_t)v1 != 0; (v0 = (uint64_t)((uint64_t)RC_U64X(a3) + (uint64_t)0ULL)); if (c) goto L_139f728; }
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)v0, 316))));
    goto L_139f72c;
L_139f728:;
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 4294967295))));
L_139f72c:;
    (t0 = (uint64_t)((uint64_t)RC_U64X(v0) + (uint64_t)0ULL)); a3xw7 = true;
    A1_W32B(s0, 12, (uint32_t)t0);
    { const bool c = (int64_t)t0 != (int64_t)s6; { float f = f20; uint32_t bits; std::memcpy(&bits, &f, sizeof(bits)); A1_W32B((uint32_t)s1, 20, bits); } if (c) goto L_139f744; }
    f0 = a1f::mul(f20, f21); a3xw0 = true;
    { float f = f0; uint32_t bits; std::memcpy(&bits, &f, sizeof(bits)); A1_W32B((uint32_t)s1, 20, bits); }
L_139f744:;
    { A3_CHARGE(); uint64_t a7c[1] = {0u}; v0 = a1_sx64(SpriteMenuSetNumPartsFormatByPartsParam_x8(ram, rt, (uint32_t)a1, (uint32_t)a2, a7c)); a2 = a7c[0]; a3xw5 = true; }
L_139f74c:;
    s3 = s3 + 1u;
    (v0 = (uint64_t)(((uint64_t)a1_sx64(s3) < (uint64_t)(int64_t)(int32_t)10) ? 1 : 0));
    if ((int64_t)v0 != 0) { (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B(s4, 0)))); { A3_BACK(); goto L_139f6d8; } }
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)326 << 16))));
    s3 = 0u;
    (s4 = (uint32_t)((int32_t)((int32_t)ADD32((uint32_t)v0, 26584))));
    (s7 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 99))));
    (s6 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 4294967295))));
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B(s4, 0))));
L_139f778:;
    { s4 = s4 + 4u; A3_CHARGE(); v0 = MakeStrHashValue(ram, rt, v0, a0, f12, f13, f14, f15, f16, f17, f18, f19, h2); }
    t1 = 2u; a3xw8 = true;
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B(s0, 4))));
    (a0 = (uint64_t)((uint64_t)fp + (uint64_t)0ULL));
    { const bool c = (int64_t)v1 != (int64_t)v0; (a1 = (uint64_t)((uint64_t)s1 + (uint64_t)0ULL)); if (c) goto L_139f818; }
    (a2 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B(s0, 20)))); a3xw5 = true;
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)SLL32((uint32_t)a2, 2))));
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v1, (uint32_t)a2))));
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)SLL32((uint32_t)v1, 2))));
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)SUB32((uint32_t)v1, (uint32_t)a2))));
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)SLL32((uint32_t)v1, 3))));
    (v0 = (uint64_t)((uint64_t)RC_U64X(v1) + (uint64_t)0ULL));
    (a3 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v1, (uint32_t)s5)))); a3xw6 = true;
    (a2 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v0, (uint32_t)s2)))); a3xw5 = true;
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)a2, 284))));
    (v0 = (uint64_t)((uint64_t)RC_U64X(a2) + (uint64_t)0ULL));
    (v1 = (uint64_t)(RC_U64X(v1) ^ (uint64_t)(uint16_t)1));
    { const bool c = (int64_t)v1 != 0; (a2 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 4294967295)))); a3xw5 = true; if (c) goto L_139f7d0; }
    (a2 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)v0, 316)))); a3xw5 = true;
L_139f7d0:;
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)a3, 52))));
    (t0 = (uint64_t)((uint64_t)RC_U64X(a2) + (uint64_t)0ULL)); a3xw7 = true;
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v0, 1))));
    (v1 = (uint64_t)(((int64_t)(int64_t)RC_U64X(v0) < (int64_t)(int32_t)0) ? 1 : 0));
    if (RC_U64X(v1) != 0) do { const a4::q128 _v = (a4v::setzero_si128()); h2 = a4::hi64(_v); std::memcpy(&v0, &_v, 8); } while (0);
    (a2 = (uint64_t)(((int64_t)(int64_t)v0 < (int64_t)(int32_t)100) ? 1 : 0)); a3xw5 = true;
    { const bool c = (int64_t)a2 != 0; A1_W32B(s0, 12, (uint32_t)v0); if (c) goto L_139f7f4; }
    A1_W32B(s0, 12, (uint32_t)s7);
L_139f7f4:;
    { const bool c = (int64_t)t0 != (int64_t)s6; { float f = f20; uint32_t bits; std::memcpy(&bits, &f, sizeof(bits)); A1_W32B((uint32_t)s1, 20, bits); } if (c) goto L_139f804; }
    f0 = a1f::mul(f20, f22); a3xw0 = true;
    { float f = f0; uint32_t bits; std::memcpy(&bits, &f, sizeof(bits)); A1_W32B((uint32_t)s1, 20, bits); }
L_139f804:;
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B(s0, 12))));
    (a2 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL)); a3xw5 = true;
    (v0 = (uint64_t)(((int64_t)(int64_t)RC_U64X(v0) < (int64_t)(int32_t)2) ? 1 : 0));
    { if (RC_U64X(v0) == 0) do { const a4::q128 _v = (a4::mk128(h9, a1_sx64(t1))); h6 = a4::hi64(_v); std::memcpy(&a2, &_v, 8); } while (0); a3xw5 = true; A3_CHARGE(); uint64_t a7c[1] = {0u}; v0 = a1_sx64(SpriteMenuSetNumPartsFormatByPartsParam_x8(ram, rt, (uint32_t)a1, (uint32_t)a2, a7c)); a2 = a7c[0]; a3xw5 = true; }
L_139f818:;
    s3 = s3 + 1u;
    (v0 = (uint64_t)(((uint64_t)a1_sx64(s3) < (uint64_t)(int64_t)(int32_t)10) ? 1 : 0));
    if ((int64_t)v0 != 0) { (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B(s4, 0)))); { A3_BACK(); goto L_139f778; } }
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)a2::frLd<uint32_t>(a2fr, 24))));
    { const bool c = (int64_t)a0 != 0; (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)a2::frLd<uint32_t>(a2fr, 12)))); if (c) goto L_139f9d4; }
    (a1 = (uint64_t)((int64_t)(int32_t)((int32_t)a2::frLd<uint32_t>(a2fr, 8))));
    { const bool c = (int64_t)a1 != 0; (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)a2::frLd<uint32_t>(a2fr, 16)))); if (c) goto L_139f9d8; }
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)s1, 24))));
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)a2::frLd<uint32_t>(a2fr, 20))));
    (v0 = (uint64_t)(RC_U64X(v0) & RC_U64X(v1)));
    goto L_139f9cc;
L_139f850:;
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)SLL32((uint32_t)v1, 2))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v0, (uint32_t)v1))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)SLL32((uint32_t)v0, 2))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)SUB32((uint32_t)v0, (uint32_t)v1))));
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)SLL32((uint32_t)v0, 3))));
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)a0, (uint32_t)s2))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)v1, 284))));
    (v0 = (uint64_t)(RC_U64X(v0) ^ (uint64_t)(uint16_t)1));
    { const bool c = (int64_t)v0 != 0; (s5 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)s2, 272)))); if (c) goto L_139f888; }
    (v0 = (uint64_t)((uint64_t)RC_U64X(v1) + (uint64_t)0ULL));
    (s6 = (uint64_t)((int64_t)(int32_t)((int8_t)A1_R8B((uint32_t)v0, 296))));
    goto L_139f88c;
L_139f888:;
    (s6 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL));
L_139f88c:;
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)326 << 16))));
    (at = (uint32_t)((int32_t)((int32_t)((uint32_t)16128 << 16))));
    { uint32_t bits = a7_opq(at); std::memcpy(&f21, &bits, sizeof(bits)); }
    (s4 = (uint32_t)((int32_t)((int32_t)ADD32((uint32_t)v0, 26624))));
    s3 = 0u;
    (s7 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 4294967295))));
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B(s4, 0))));
L_139f8a8:;
    { s4 = s4 + 4u; A3_CHARGE(); v0 = MakeStrHashValue(ram, rt, v0, a0, f12, f13, f14, f15, f16, f17, f18, f19, h2); }
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B(s0, 4))));
    if ((int64_t)v1 != (int64_t)v0) { s3 = s3 + 1u; goto L_139f914; }
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B(s0, 20))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)SLL32((uint32_t)v1, 2))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v0, (uint32_t)v1))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)SLL32((uint32_t)v0, 2))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)SUB32((uint32_t)v0, (uint32_t)v1))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)SLL32((uint32_t)v0, 3))));
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v0, (uint32_t)s2))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v0, (uint32_t)s5))));
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)a0, 284))));
    { uint32_t bits = A1_R32B((uint32_t)v0, 60); float f; std::memcpy(&f, &bits, sizeof(f)); f0 = f; } a3xw0 = true;
    { int32_t tmp; std::memcpy(&tmp, &f0, sizeof(tmp)); f0 = ((float)(int32_t)(tmp)); } a3xw0 = true;
    (a1 = (uint64_t)((uint64_t)RC_U64X(a0) + (uint64_t)0ULL));
    (v1 = (uint64_t)(RC_U64X(v1) ^ (uint64_t)(uint16_t)1));
    { const bool c = (int64_t)v1 != 0; (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 4294967295)))); if (c) goto L_139f8fc; }
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)a1, 316))));
L_139f8fc:;
    f0 = a1f::mul(f0, f20); a3xw0 = true;
    { const bool c = (int64_t)v0 != (int64_t)s7; { float f = f0; uint32_t bits; std::memcpy(&bits, &f, sizeof(bits)); A1_W32B((uint32_t)s1, 20, bits); } if (c) goto L_139f910; }
    f0 = a1f::mul(f0, f21); a3xw0 = true;
    { float f = f0; uint32_t bits; std::memcpy(&bits, &f, sizeof(bits)); A1_W32B((uint32_t)s1, 20, bits); }
L_139f910:;
    s3 = s3 + 1u;
L_139f914:;
    (v0 = (uint64_t)(((uint64_t)a1_sx64(s3) < (uint64_t)(int64_t)(int32_t)10) ? 1 : 0));
    if ((int64_t)v0 != 0) { (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B(s4, 0)))); { A3_BACK(); goto L_139f8a8; } }
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)326 << 16))));
    (at = (uint32_t)((int32_t)((int32_t)((uint32_t)16128 << 16))));
    { uint32_t bits = a7_opq(at); std::memcpy(&f21, &bits, sizeof(bits)); }
    (s4 = (uint32_t)((int32_t)((int32_t)ADD32((uint32_t)v0, 26664))));
    s3 = 0u;
    (s5 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 4294967295))));
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B(s4, 0))));
L_139f940:;
    { s4 = s4 + 4u; A3_CHARGE(); v0 = MakeStrHashValue(ram, rt, v0, a0, f12, f13, f14, f15, f16, f17, f18, f19, h2); }
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B(s0, 4))));
    if ((int64_t)v1 != (int64_t)v0) { s3 = s3 + 1u; goto L_139f9a0; }
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B(s0, 20))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)SLL32((uint32_t)v1, 2))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v0, (uint32_t)v1))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)SLL32((uint32_t)v0, 2))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)SUB32((uint32_t)v0, (uint32_t)v1))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)SLL32((uint32_t)v0, 3))));
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v0, (uint32_t)s2))));
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)a0, 284))));
    (v1 = (uint64_t)(RC_U64X(v1) ^ (uint64_t)(uint16_t)1));
    { const bool c = (int64_t)v1 != 0; (v0 = (uint64_t)((uint64_t)RC_U64X(a0) + (uint64_t)0ULL)); if (c) goto L_139f988; }
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)v0, 316))));
    goto L_139f98c;
L_139f988:;
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 4294967295))));
L_139f98c:;
    { const bool c = (int64_t)v0 != (int64_t)s5; { float f = f20; uint32_t bits; std::memcpy(&bits, &f, sizeof(bits)); A1_W32B((uint32_t)s1, 20, bits); } if (c) goto L_139f99c; }
    f0 = a1f::mul(f20, f21); a3xw0 = true;
    { float f = f0; uint32_t bits; std::memcpy(&bits, &f, sizeof(bits)); A1_W32B((uint32_t)s1, 20, bits); }
L_139f99c:;
    s3 = s3 + 1u;
L_139f9a0:;
    (v0 = (uint64_t)(((uint64_t)a1_sx64(s3) < (uint64_t)(int64_t)(int32_t)10) ? 1 : 0));
    if ((int64_t)v0 != 0) { (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B(s4, 0)))); { A3_BACK(); goto L_139f940; } }
    { const bool c = (int64_t)s6 != 0; (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)a2::frLd<uint32_t>(a2fr, 12)))); if (c) goto L_139f9d4; }
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)a2::frLd<uint32_t>(a2fr, 8))));
    { const bool c = (int64_t)a0 != 0; (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)a2::frLd<uint32_t>(a2fr, 16)))); if (c) goto L_139f9d8; }
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)s1, 24))));
    (a1 = (uint64_t)((int64_t)(int32_t)((int32_t)a2::frLd<uint32_t>(a2fr, 20))));
    (v0 = (uint64_t)(RC_U64X(v0) & RC_U64X(a1)));
L_139f9cc:;
    A1_W32B((uint32_t)s1, 24, (uint32_t)v0);
L_139f9d0:;
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)a2::frLd<uint32_t>(a2fr, 12))));
L_139f9d4:;
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)a2::frLd<uint32_t>(a2fr, 16))));
L_139f9d8:;
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)v1, 8))));
    (a1 = (uint64_t)((int64_t)(int32_t)((int32_t)a2::frLd<uint32_t>(a2fr, 28))));
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)a0, 1))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)SRL32((uint32_t)v0, 4))));
    a2::frSt<uint32_t>(a2fr, 16, (uint32_t)a0);
    (v0 = (uint64_t)(RC_U64X(v0) & (uint64_t)(uint16_t)1023));
    (a1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)a1, 28))));
    (v0 = (uint64_t)(((int64_t)(int64_t)RC_U64X(a0) < (int64_t)(int64_t)RC_U64X(v0)) ? 1 : 0));
    { const bool c = (int64_t)v0 != 0; a2::frSt<uint32_t>(a2fr, 28, (uint32_t)a1); if (c) { A3_BACK(); goto L_139f530; } }
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)a2::frLd<uint32_t>(a2fr, 4))));
L_139fa04:;
    { const bool c = (int64_t)v0 == 0; (s0 = (uint32_t)((int32_t)((int32_t)A1_R32B((uint32_t)s2, 40)))); if (c) goto L_139fa28; }
    { (a0 = (uint64_t)((uint64_t)v0 + (uint64_t)0ULL)); A3_CHARGE(); v0 = MakeStrHashValue(ram, rt, v0, a0, f12, f13, f14, f15, f16, f17, f18, f19, h2); }
    (a0 = (uint64_t)((uint64_t)fp + (uint64_t)0ULL));
    { (a1 = (uint64_t)((uint64_t)v0 + (uint64_t)0ULL)); A3_CHARGE(); uint64_t a7c[3] = {0u, 0u, 0u}; v0 = SpriteMenuGetLayerNoByHash_x8(ram, rt, a0, a1, h3, h4, a7c); a2 = a7c[0]; a3 = a7c[1]; t0 = a7c[2]; a3xw5 = true; a3xw6 = true; a3xw7 = true; }
    (v1 = (uint64_t)((uint64_t)v0 + (uint64_t)0ULL));
    goto L_139fa2c;
L_139fa28:;
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 4294967295))));
L_139fa2c:;
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)a2::frLd<uint32_t>(a2fr, 0))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)SLL32((uint32_t)v1, 4))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v0, (uint32_t)v1))));
    (v1 = (uint64_t)(a1_sx64(s0) & (uint64_t)(uint16_t)1023));
    (a1 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)a0, 4))));
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)65504 << 16))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)SLL32((uint32_t)v0, 2))));
    (a0 = (uint64_t)(RC_U64X(a0) | (uint64_t)(uint16_t)2047));
    (a1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)a1, (uint32_t)v0))));
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)SLL32((uint32_t)v1, 11))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)a1, 56))));
    (v0 = (uint64_t)(RC_U64X(v0) & RC_U64X(a0)));
    (v0 = (uint64_t)(RC_U64X(v0) | RC_U64X(v1)));
    { uint32_t bits = a2::frLd<uint32_t>(a2fr, 128); float f; std::memcpy(&f, &bits, sizeof(f)); f22 = f; }
    { uint32_t bits = a2::frLd<uint32_t>(a2fr, 120); float f; std::memcpy(&f, &bits, sizeof(f)); f21 = f; }
    f20 = a1_flt(a2::frLd<uint32_t>(a2fr, 112));
    A1_W32B((uint32_t)a1, 56, (uint32_t)v0);
    { if (xo) { if (a3xw0) xo[0] = a1_bits(f0); xo[1] = a1_bits(f20); xo[2] = a1_bits(f21); xo[3] = a1_bits(f22); xo[4] = a1; if (a3xw5) xo[5] = a2; if (a3xw6) xo[6] = a3; if (a3xw7) xo[7] = t0; if (a3xw8) xo[8] = a1_sx64(t1); } io_h2 = h2; io_h3 = h3; io_h6 = h6; return v0; }
    __builtin_unreachable();
}

// gamecore_sub_01441AD0 (0x1441ad0): typed by tools/typed_lift2.py (round A6; frame calls; VU0 in h5, out vf5 vf6, relocated calls 1, frame 16 bytes, 0 slot accesses)
inline uint32_t gamecore_sub_01441AD0(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, float f12, float f13, float f14, float f15, float f16, float f17, float f18, float f19, a4::vec4 &io_vf5, a4::vec4 &io_vf6, uint64_t io_h5)
{
    float f0 = 0.0f;
    uint32_t at = 0;
    uint32_t v0 = 0;
    uint32_t v1 = 0;
    uint32_t a0 = 0;
    uint32_t a1 = 0;
    a4::vec4 vf5 = io_vf5;
    a4::vec4 vf6 = io_vf6;
    uint64_t h5 = io_h5;
    { const uint32_t _dynInsn = A1_R32(0x1441AD0u); const int32_t _dynSimm = (int16_t)(_dynInsn & 0xFFFFu); const uint32_t _dynImm = _dynInsn & 0xFFFFu; (void)_dynSimm; (void)_dynImm; (v1 = (uint32_t)((int32_t)((int32_t)((uint32_t)_dynImm << 16)))); }
    { const uint32_t _dynInsn = A1_R32(0x1441AD8u); const int32_t _dynSimm = (int16_t)(_dynInsn & 0xFFFFu); const uint32_t _dynImm = _dynInsn & 0xFFFFu; (void)_dynSimm; (void)_dynImm; (v0 = (uint32_t)((int32_t)((int32_t)A1_R32(ADD32(v1, _dynSimm))))); }
    v0 = v0 & 0x20u;
    if (v0 == 0) goto L_1441b38;
    { A3_CHARGE(); v0 = iosgluGetCameraContext(ram, rt, f12, f13, f14, f15, f16, f17, f18, f19); }
    v1 = 0x1200000u;
    at = 0x41700000u;
    f0 = a1_flt(a7_opq(at));
    v1 = v1 + 0x1330u;
    a0 = v0 + 0x150u;
    a1 = a1_bits(f0);
    vf5 = a4v::castsi128_ps(A4_R128(ADD32(a0, 0)));
    vf6 = a4v::castsi128_ps(a4::mk128(h5, a1_sx64(a1)));
    { a4::vec4 res = a4v::mul(vf5, a4v::bcast<0>(vf6)); a4::q128 mask = a4v::set_epi32(0, -1, -1, -1); vf5 = a4v::blendv_ps(vf5, res, a4v::castsi128_ps(mask)); }
    A4_W128(ADD32(v1, 0), a4v::castps_si128(vf5));
    v0 = v0 + 0x140u;
    vf5 = a4v::castsi128_ps(A4_R128(ADD32(v0, 0)));
    vf6 = a4v::castsi128_ps(A4_R128(ADD32(v1, 0)));
    { a4::vec4 res = a4v::add(vf5, vf6); a4::q128 mask = a4v::set_epi32(0, -1, -1, -1); vf5 = a4v::blend(vf5, res, a4v::castsi128_ps(mask)); }
    A4_W128(ADD32(v1, 0), a4v::castps_si128(vf5));
    at = 0x3F800000u;
    f0 = a1_flt(a7_opq(at));
    A1_W32B(v1, 12, a1_bits(f0));
L_1441b38:;
    { io_vf5 = vf5; io_vf6 = vf6; return v0; }
    __builtin_unreachable();
}

// iosLoaderResetEntry (0x1ad7b8): typed by tools/typed_lift2.py (round A6; loop guest frame calls, extra outputs xo[0]=a1 xo[1]=a2; VU0 in -, out h2, 64-bit inputs a1 a3 t0 t1 t2 t3)
inline uint64_t iosLoaderResetEntry(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint64_t a0, uint64_t w64_a1, uint64_t a2, uint64_t w64_a3, uint64_t w64_t0, uint64_t w64_t1, uint64_t w64_t2, uint64_t w64_t3, uint32_t gp, uint32_t sp, float f12, float f13, float f14, float f15, float f16, float f17, float f18, float f19, uint64_t &io_h2, uint64_t *xo = nullptr)
{
    uint64_t v0 = 0;
    uint32_t v1 = 0;
    uint32_t a1 = (uint32_t)w64_a1;
    uint32_t a3 = (uint32_t)w64_a3;
    uint32_t t0 = (uint32_t)w64_t0;
    uint32_t t1 = (uint32_t)w64_t1;
    uint32_t t2 = (uint32_t)w64_t2;
    uint32_t t3 = (uint32_t)w64_t3;
    uint32_t s0 = 0;
    uint64_t s1 = 0;
    uint32_t s2 = 0;
    uint64_t h2 = io_h2;
    sp = sp - 0x20u;
    s2 = 0x200000u;
    s0 = s2 - 0x2750u;
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B(s0, 0))));
    { const bool c = (int64_t)v0 != 0; (s1 = (uint64_t)((uint64_t)a0 + (uint64_t)0ULL)); if (c) goto L_1ad7ec; }
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)27 << 16))));
    { (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)a0, 4294957192)))); A3_CHARGE(); v0 = loaderSetResetCallback(ram, rt, a0, w64_a1, a2, w64_a3, w64_t0, w64_t1, w64_t2, w64_t3, gp, sp, f12, f13, f14, f15, f16, f17, f18, f19, h2); }
L_1ad7ec:;
    (a2 = (uint64_t)((uint64_t)a1_sx64(s0) + (uint64_t)0ULL));
    s0 = 0u;
    a1 = s0 << 2;
L_1ad7f8:;
    s0 = s0 + 1u;
    (v1 = (uint32_t)((int32_t)((int32_t)ADD32(a1, (uint32_t)a2))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B(v1, 0))));
    { const bool c = (int64_t)v0 == 0; (a0 = (uint64_t)(((int64_t)(int64_t)a1_sx64(s0) < (int64_t)(int32_t)2) ? 1 : 0)); if (c) goto L_1ad82c; }
    if ((int64_t)a0 != 0) { a1 = s0 << 2; { A3_BACK(); goto L_1ad7f8; } }
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)30 << 16))));
    { (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)a0, 7232)))); A3_CHARGE(); v0 = a1_sx64(a7x::iosJumpRecoverPoint(ram, rt, (uint32_t)a0, a1, (uint32_t)a2, a3, t0, t1, t2, t3, sp, h2)); }
    a1 = s0 << 2;
L_1ad82c:;
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(s2, 4294957232))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(a1, (uint32_t)v0))));
    A1_W32B((uint32_t)v0, 0, (uint32_t)s1);
    { sp = sp + 0x20u; if (xo) { xo[0] = a1_sx64(a1); xo[1] = a2; } io_h2 = h2; return v0; }
    __builtin_unreachable();
}

// manager_sub_010B8D28 (0x10b8d28): typed by tools/typed_lift2.py (round A6; frame calls; VU0 in vf1 vf2 vf3 vf4 vf10 vacc h4, out vf8 vf10 vf14 vf15 vf16 vf17 vacc h2 h6 h7 h8 h9, 64-bit inputs a0 a1, guest sp, frame 48 bytes, 6 slot accesses)
inline a1::RetVF manager_sub_010B8D28(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint64_t w64_a0, uint64_t w64_a1, uint32_t sp, float f0, float f12, float f13, float f14, float f15, float f16, float f17, float f18, float f19, float f20, float f21, float f22, const a4::vec4 &io_vf1, const a4::vec4 &io_vf2, const a4::vec4 &io_vf3, const a4::vec4 &io_vf4, a4::vec4 &io_vf8, a4::vec4 &io_vf10, a4::vec4 &io_vf14, a4::vec4 &io_vf15, a4::vec4 &io_vf16, a4::vec4 &io_vf17, a4::vec4 &io_vacc, uint64_t &io_h2, uint64_t io_h4, uint64_t &io_h6, uint64_t &io_h7, uint64_t &io_h8, uint64_t &io_h9)
{
    uint32_t at = 0;
    uint64_t v0 = 0;
    uint32_t a0 = (uint32_t)w64_a0;
    uint32_t a1 = (uint32_t)w64_a1;
    uint64_t s0 = 0;
    uint32_t fcr31 = 0;
    a4::vec4 vf1 = io_vf1;
    a4::vec4 vf2 = io_vf2;
    a4::vec4 vf3 = io_vf3;
    a4::vec4 vf4 = io_vf4;
    a4::vec4 vf8 = io_vf8;
    a4::vec4 vf10 = io_vf10;
    a4::vec4 vf14 = io_vf14;
    a4::vec4 vf15 = io_vf15;
    a4::vec4 vf16 = io_vf16;
    a4::vec4 vf17 = io_vf17;
    a4::vec4 vacc = io_vacc;
    uint64_t h2 = io_h2;
    uint64_t h4 = io_h4;
    uint64_t h6 = io_h6;
    uint64_t h7 = io_h7;
    uint64_t h8 = io_h8;
    uint64_t h9 = io_h9;
    uint8_t a2fr[48];
    (s0 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL));
    a2::frSt<uint32_t>(a2fr, 32, a1_bits(f22));
    a2::frSt<uint32_t>(a2fr, 24, a1_bits(f21));
    { a2::frSt<uint32_t>(a2fr, 16, a1_bits(f20)); A3_CHARGE(); { const a1::RetVF a2_r = manager_sub_010B2318(ram, rt, w64_a0, w64_a1, a1_sx64((uint32_t)(sp + 0xFFFFFFD0u)), f12, f13, f14, f15, f16, f17, f18, f19, f20, f21, vf1, vf2, vf3, vf4, vf8, vf10, vf14, vf15, vf16, vf17, vacc, h2, h4, h6, h7, h8, h9); v0 = a2_r.v0; f0 = a2_r.f0; } }
    at = 0x3F800000u;
    f22 = a1_flt(a7_opq(at));
    f21 = ((float)(f0));
    fcr31 = (a1f::lt(f22, f21)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
    { const bool c = (fcr31 & 0x800000u) != 0u; f15 = ((float)(f21)); if (c) goto L_10b8d8c; }
    f20 = a1_flt(0u);
    (s0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 1))));
    f12 = ((float)(f20));
    f13 = ((float)(f20));
    { f14 = ((float)(f20)); A3_CHARGE(); setv3f(ram, rt); }
    f12 = ((float)(f20));
    f13 = ((float)(f12));
    f14 = ((float)(f22));
    { f15 = ((float)(f21)); A3_CHARGE(); setv3f(ram, rt); }
L_10b8d8c:;
    (v0 = (uint64_t)((uint64_t)s0 + (uint64_t)0ULL));
    f22 = a1_flt(a2::frLd<uint32_t>(a2fr, 32));
    f21 = a1_flt(a2::frLd<uint32_t>(a2fr, 24));
    f20 = a1_flt(a2::frLd<uint32_t>(a2fr, 16));
    { io_vf8 = vf8; io_vf10 = vf10; io_vf14 = vf14; io_vf15 = vf15; io_vf16 = vf16; io_vf17 = vf17; io_vacc = vacc; io_h2 = h2; io_h6 = h6; io_h7 = h7; io_h8 = h8; io_h9 = h9; return a1::RetVF{(uint64_t)v0, f0}; }
    __builtin_unreachable();
}

// UpdateCharacterClassPastGeometry (0x10c14f8): typed by tools/typed_lift2.py (round A6; frame calls, extra outputs xo[0]=a1 xo[1]=a2 xo[2]=a3 xo[3]=t0 xo[4]=t1; VU0 in -, out h2 h6 h7 h8 h9, frame 16 bytes, 0 slot accesses)
inline uint64_t UpdateCharacterClassPastGeometry(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint64_t a0, uint64_t a2, uint64_t a3, uint64_t t0, uint64_t t1, float f12, float f13, float f14, float f15, float f16, float f17, float f18, float f19, uint64_t &io_h2, uint64_t &io_h6, uint64_t &io_h7, uint64_t &io_h8, uint64_t &io_h9, uint64_t *xo = nullptr)
{
    uint64_t v0 = 0;
    uint32_t v1 = 0;
    uint64_t a1 = 0;
    uint64_t s0 = 0;
    uint64_t h2 = io_h2;
    uint64_t h6 = io_h6;
    uint64_t h7 = io_h7;
    uint64_t h8 = io_h8;
    uint64_t h9 = io_h9;
    (s0 = (uint64_t)((uint64_t)a0 + (uint64_t)0ULL));
    (v1 = (uint32_t)((int32_t)((int32_t)A1_R32B((uint32_t)s0, 312))));
    do { const a4::q128 _v = (A4_R128(ADD32(v1, 0))); h2 = a4::hi64(_v); std::memcpy(&v0, &_v, 8); } while (0);
    { A4_W128(ADD32((uint32_t)s0, 1280), a4::mk128(h2, v0)); A3_CHARGE(); uint64_t a7c[6] = {0u, 0u, a2, a3, t0, t1}; UpdateLayoutClassPastGeometry(ram, rt, a0, a3, t0, t1, f12, f13, f14, f15, f16, f17, f18, f19, h2, h6, h7, h8, h9, a7c); a1 = a7c[1]; a2 = a7c[2]; a3 = a7c[3]; t0 = a7c[4]; t1 = a7c[5]; }
    do { const a4::q128 _v = (A4_R128(ADD32((uint32_t)s0, 1216))); h2 = a4::hi64(_v); std::memcpy(&v0, &_v, 8); } while (0);
    A4_W128(ADD32((uint32_t)s0, 1232), a4::mk128(h2, v0));
    { if (xo) { xo[0] = a1; xo[1] = a2; xo[2] = a3; xo[3] = t0; xo[4] = t1; } io_h2 = h2; io_h6 = h6; io_h7 = h7; io_h8 = h8; io_h9 = h9; return v0; }
    __builtin_unreachable();
}

// manager_sub_010D01A8 (0x10d01a8): typed by tools/typed_lift2.py (round A6; loop frame calls, extra outputs xo[0]=f0 xo[1]=f20; VU0 in h30, out h2, guest sp, frame 96 bytes, 2 slot accesses)
inline uint64_t manager_sub_010D01A8(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint64_t a0, uint64_t a1, uint64_t a2, uint64_t a3, uint32_t t3, uint32_t sp, float f12, float f13, float f14, float f15, float f16, float f17, float f18, float f19, float f20, uint64_t &io_h2, uint64_t io_h30, uint64_t *xo = nullptr)
{
    float f0 = 0.0f;
    float f1 = 0.0f;
    float f2 = 0.0f;
    float f3 = 0.0f;
    float f4 = 0.0f;
    float f5 = 0.0f;
    float f6 = 0.0f;
    uint32_t at = 0;
    uint64_t v0 = 0;
    uint64_t v1 = 0;
    uint64_t t0 = 0;
    uint64_t t1 = 0;
    uint32_t t2 = 0;
    uint64_t s0 = 0;
    uint64_t s1 = 0;
    uint32_t s2 = 0;
    uint64_t s3 = 0;
    uint32_t s4 = 0;
    uint64_t s5 = 0;
    uint64_t s6 = 0;
    uint32_t s7 = 0;
    uint32_t fp = 0;
    uint64_t hi = 0;
    uint64_t lo = 0;
    uint32_t fcr31 = 0;
    uint64_t h2 = io_h2;
    uint64_t h30 = io_h30;
    uint8_t a2fr[96];
    bool a3xw0 = false;
    (s0 = (uint64_t)((uint64_t)a0 + (uint64_t)0ULL));
    (s3 = (uint64_t)((uint64_t)a2 + (uint64_t)0ULL));
    (s5 = (uint64_t)((uint64_t)a1 + (uint64_t)0ULL));
    { const bool c = (int64_t)a3 == 0; a2::frSt<uint32_t>(a2fr, 80, a1_bits(f20)); if (c) goto L_10d0864; }
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)a3, 212))));
    if ((int64_t)v1 == 0) { (a1 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)s5, 4336)))); goto L_10d0218; }
    (t0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)a3, 216))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)SLL32((uint32_t)t0, 3))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)SUB32((uint32_t)v0, (uint32_t)t0))));
    (t0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 1))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)SLL32((uint32_t)v0, 2))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v1, (uint32_t)v0))));
    (s1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v0, 4294967268))));
    goto L_10d02a8;
L_10d0218:;
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)3 << 16))));
    (v0 = (uint64_t)(RC_U64X(v0) | (uint64_t)(uint16_t)65535));
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)a1, 0))));
    (v1 = (uint64_t)(RC_U64X(v1) & RC_U64X(v0)));
    { const bool c = (int64_t)v1 == 0; (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)SUB32((uint32_t)a1, (uint32_t)v1)))); if (c) goto L_10d0238; }
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)v0, 0))));
    goto L_10d023c;
L_10d0238:;
    (a0 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL));
L_10d023c:;
    { const bool c = (int64_t)a0 == 0; { const uint32_t _dynInsn = A1_R32(0x10D0240u); const int32_t _dynSimm = (int16_t)(_dynInsn & 0xFFFFu); const uint32_t _dynImm = _dynInsn & 0xFFFFu; (void)_dynSimm; (void)_dynImm; (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)_dynImm << 16)))); } if (c) goto L_10d0258; }
    { const uint32_t _dynInsn = A1_R32(0x10D0244u); const int32_t _dynSimm = (int16_t)(_dynInsn & 0xFFFFu); const uint32_t _dynImm = _dynInsn & 0xFFFFu; (void)_dynSimm; (void)_dynImm; (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32(ADD32((uint32_t)v0, _dynSimm))))); }
    if ((int64_t)a0 == (int64_t)v1) { (a0 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL)); goto L_10d025c; }
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)a0, 12))));
    goto L_10d025c;
L_10d0258:;
    (a0 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL));
L_10d025c:;
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)a1, 0))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)3 << 16))));
    (v0 = (uint64_t)(RC_U64X(v0) | (uint64_t)(uint16_t)65535));
    (v0 = (uint64_t)(RC_U64X(v1) & RC_U64X(v0)));
    { const bool c = (int64_t)v0 == 0; (t0 = (uint64_t)((uint64_t)RC_U64X(a0) + (uint64_t)0ULL)); if (c) goto L_10d0280; }
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)SUB32((uint32_t)a1, (uint32_t)v0))));
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)v0, 0))));
    goto L_10d0284;
L_10d0280:;
    (a0 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL));
L_10d0284:;
    { const bool c = (int64_t)a0 == 0; { const uint32_t _dynInsn = A1_R32(0x10D0288u); const int32_t _dynSimm = (int16_t)(_dynInsn & 0xFFFFu); const uint32_t _dynImm = _dynInsn & 0xFFFFu; (void)_dynSimm; (void)_dynImm; (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)_dynImm << 16)))); } if (c) goto L_10d02a0; }
    { const uint32_t _dynInsn = A1_R32(0x10D028Cu); const int32_t _dynSimm = (int16_t)(_dynInsn & 0xFFFFu); const uint32_t _dynImm = _dynInsn & 0xFFFFu; (void)_dynSimm; (void)_dynImm; (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32(ADD32((uint32_t)v0, _dynSimm))))); }
    if ((int64_t)a0 == (int64_t)v1) { (v0 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL)); goto L_10d02a4; }
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)a0, 16))));
    goto L_10d02a4;
L_10d02a0:;
    (v0 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL));
L_10d02a4:;
    (s1 = (uint64_t)((uint64_t)RC_U64X(v0) + (uint64_t)0ULL));
L_10d02a8:;
    { const uint32_t _dynInsn = A1_R32(0x10D02A8u); const int32_t _dynSimm = (int16_t)(_dynInsn & 0xFFFFu); const uint32_t _dynImm = _dynInsn & 0xFFFFu; (void)_dynSimm; (void)_dynImm; (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)_dynImm << 16)))); }
    (t1 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL));
    { const bool c = (int32_t)t0 <= 0; { const uint32_t _dynInsn = A1_R32(0x10D02B4u); const int32_t _dynSimm = (int16_t)(_dynInsn & 0xFFFFu); const uint32_t _dynImm = _dynInsn & 0xFFFFu; (void)_dynSimm; (void)_dynImm; (t2 = (uint32_t)((int32_t)((int32_t)A1_R32(ADD32((uint32_t)v0, _dynSimm))))); } if (c) goto L_10d0864; }
    (s6 = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)3 << 16))));
    (at = (uint32_t)((int32_t)((int32_t)((uint32_t)16256 << 16))));
    { uint32_t bits = a7_opq(at); std::memcpy(&f20, &bits, sizeof(bits)); }
    (s6 = (uint64_t)(RC_U64X(s6) | (uint64_t)(uint16_t)65535));
    { const uint32_t _dynInsn = A1_R32(0x10D02C8u); const int32_t _dynSimm = (int16_t)(_dynInsn & 0xFFFFu); const uint32_t _dynImm = _dynInsn & 0xFFFFu; (void)_dynSimm; (void)_dynImm; (s7 = (uint32_t)((int32_t)((int32_t)((uint32_t)_dynImm << 16)))); }
    fp = 1u;
L_10d02d0:;
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)s1, 0))));
    (v0 = (uint64_t)(RC_U64X(v1) & s6));
    { const bool c = (int64_t)v0 == 0; (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)SUB32((uint32_t)s1, (uint32_t)v0)))); if (c) goto L_10d02e8; }
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)v0, 0))));
    goto L_10d02ec;
L_10d02e8:;
    (a0 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL));
L_10d02ec:;
    { const bool c = (int64_t)a0 == 0; { const uint32_t _dynInsn = A1_R32(0x10D02F0u); const int32_t _dynSimm = (int16_t)(_dynInsn & 0xFFFFu); const uint32_t _dynImm = _dynInsn & 0xFFFFu; (void)_dynSimm; (void)_dynImm; (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32(ADD32(s7, _dynSimm))))); } if (c) goto L_10d0318; }
    { const bool c = (int64_t)a0 == (int64_t)v0; (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)SRL32((uint32_t)v1, 18)))); if (c) goto L_10d0318; }
    (a1 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)a0, 8))));
    (v1 = (uint64_t)(RC_U64X(v1) & (uint64_t)(uint16_t)4095));
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)a0, 16))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)a1, 12))));
    { int64_t result = (int64_t)(int32_t)v1 * (int64_t)(int32_t)v0; lo = (uint64_t)(int64_t)(int32_t)result; hi = (uint64_t)(int64_t)(int32_t)(result >> 32); (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)result))); }
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)a0, (uint32_t)v1))));
    goto L_10d031c;
L_10d0318:;
    (v0 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL));
L_10d031c:;
    if ((int64_t)v0 != (int64_t)a3) { (t1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)t1, 1)))); goto L_10d0858; }
    (s2 = (uint32_t)((int32_t)((int32_t)ADD32((uint32_t)s1, 4))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B(s2, 0))));
    (v0 = (uint64_t)(RC_U64X(v0) & s6));
    { const bool c = (int64_t)v0 == 0; (a2 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL)); if (c) goto L_10d0348; }
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)SUB32(s2, (uint32_t)v0))));
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)v0, 0))));
    goto L_10d034c;
L_10d0348:;
    (v1 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL));
L_10d034c:;
    { const bool c = (int64_t)v1 == 0; { const uint32_t _dynInsn = A1_R32(0x10D0350u); const int32_t _dynSimm = (int16_t)(_dynInsn & 0xFFFFu); const uint32_t _dynImm = _dynInsn & 0xFFFFu; (void)_dynSimm; (void)_dynImm; (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32(ADD32(s7, _dynSimm))))); } if (c) goto L_10d035c; }
    { const bool c = (int64_t)v1 != (int64_t)v0; (a0 = (uint64_t)((uint64_t)RC_U64X(v1) + (uint64_t)0ULL)); if (c) goto L_10d0360; }
L_10d035c:;
    (a0 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL));
L_10d0360:;
    { const uint32_t _dynInsn = A1_R32(0x10D0360u); const int32_t _dynSimm = (int16_t)(_dynInsn & 0xFFFFu); const uint32_t _dynImm = _dynInsn & 0xFFFFu; (void)_dynSimm; (void)_dynImm; (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)_dynImm << 16)))); }
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)a0, 8))));
    { const uint32_t _dynInsn = A1_R32(0x10D0368u); const int32_t _dynSimm = (int16_t)(_dynInsn & 0xFFFFu); const uint32_t _dynImm = _dynInsn & 0xFFFFu; (void)_dynSimm; (void)_dynImm; (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v0, _dynSimm)))); }
    { const bool c = (int64_t)v1 != (int64_t)v0; (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B(s2, 0)))); if (c) goto L_10d0418; }
    (v0 = (uint64_t)(RC_U64X(v1) & s6));
    { const bool c = (int64_t)v0 == 0; (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)SUB32(s2, (uint32_t)v0)))); if (c) goto L_10d0388; }
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)v0, 0))));
    goto L_10d038c;
L_10d0388:;
    (a0 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL));
L_10d038c:;
    { const bool c = (int64_t)a0 == 0; { const uint32_t _dynInsn = A1_R32(0x10D0390u); const int32_t _dynSimm = (int16_t)(_dynInsn & 0xFFFFu); const uint32_t _dynImm = _dynInsn & 0xFFFFu; (void)_dynSimm; (void)_dynImm; (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32(ADD32(s7, _dynSimm))))); } if (c) goto L_10d03b8; }
    { const bool c = (int64_t)a0 == (int64_t)v0; (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)SRL32((uint32_t)v1, 18)))); if (c) goto L_10d03b8; }
    (a1 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)a0, 8))));
    (v1 = (uint64_t)(RC_U64X(v1) & (uint64_t)(uint16_t)4095));
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)a0, 16))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)a1, 12))));
    { int64_t result = (int64_t)(int32_t)v1 * (int64_t)(int32_t)v0; lo = (uint64_t)(int64_t)(int32_t)result; hi = (uint64_t)(int64_t)(int32_t)(result >> 32); (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)result))); }
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)a0, (uint32_t)v1))));
    goto L_10d03bc;
L_10d03b8:;
    (v0 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL));
L_10d03bc:;
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v0, 4))));
    (a2 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)v1, 0))));
    (v0 = (uint64_t)(RC_U64X(a2) & s6));
    { const bool c = (int64_t)v0 == 0; (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)SUB32((uint32_t)v1, (uint32_t)v0)))); if (c) goto L_10d03d8; }
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)v0, 0))));
    goto L_10d03dc;
L_10d03d8:;
    (a0 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL));
L_10d03dc:;
    { const bool c = (int64_t)a0 == 0; { const uint32_t _dynInsn = A1_R32(0x10D03E0u); const int32_t _dynSimm = (int16_t)(_dynInsn & 0xFFFFu); const uint32_t _dynImm = _dynInsn & 0xFFFFu; (void)_dynSimm; (void)_dynImm; (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32(ADD32(s7, _dynSimm))))); } if (c) goto L_10d0408; }
    { const bool c = (int64_t)a0 == (int64_t)v0; (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)SRL32((uint32_t)a2, 18)))); if (c) goto L_10d0408; }
    (a1 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)a0, 8))));
    (v1 = (uint64_t)(RC_U64X(v1) & (uint64_t)(uint16_t)4095));
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)a0, 16))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)a1, 12))));
    { int64_t result = (int64_t)(int32_t)v1 * (int64_t)(int32_t)v0; lo = (uint64_t)(int64_t)(int32_t)result; hi = (uint64_t)(int64_t)(int32_t)(result >> 32); (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)result))); }
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)a0, (uint32_t)v1))));
    goto L_10d040c;
L_10d0408:;
    (v0 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL));
L_10d040c:;
    A1_W32B((uint32_t)s0, 4, (uint32_t)v0);
    (a2 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 1))));
    goto L_10d0478;
L_10d0418:;
    (v0 = (uint64_t)(RC_U64X(v1) & s6));
    { const bool c = (int64_t)v0 == 0; (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)SUB32(s2, (uint32_t)v0)))); if (c) goto L_10d0430; }
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)v0, 0))));
    goto L_10d0434;
L_10d0430:;
    (a0 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL));
L_10d0434:;
    { const bool c = (int64_t)a0 == 0; { const uint32_t _dynInsn = A1_R32(0x10D0438u); const int32_t _dynSimm = (int16_t)(_dynInsn & 0xFFFFu); const uint32_t _dynImm = _dynInsn & 0xFFFFu; (void)_dynSimm; (void)_dynImm; (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32(ADD32(s7, _dynSimm))))); } if (c) goto L_10d0460; }
    { const bool c = (int64_t)a0 == (int64_t)v0; (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)SRL32((uint32_t)v1, 18)))); if (c) goto L_10d0460; }
    (a1 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)a0, 8))));
    (v1 = (uint64_t)(RC_U64X(v1) & (uint64_t)(uint16_t)4095));
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)a0, 16))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)a1, 12))));
    { int64_t result = (int64_t)(int32_t)v1 * (int64_t)(int32_t)v0; lo = (uint64_t)(int64_t)(int32_t)result; hi = (uint64_t)(int64_t)(int32_t)(result >> 32); (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)result))); }
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)a0, (uint32_t)v1))));
    goto L_10d0464;
L_10d0460:;
    (v0 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL));
L_10d0464:;
    if ((int64_t)v0 != (int64_t)(int32_t)t2) goto L_10d0478;
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)s5, 1872))));
    (a2 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 1))));
    A1_W32B((uint32_t)s0, 4, (uint32_t)v0);
L_10d0478:;
    if ((int64_t)a2 == 0) { (t1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)t1, 1)))); goto L_10d0858; }
    (s4 = (uint32_t)((int32_t)((int32_t)A1_R32B((uint32_t)s3, 0))));
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(s4, 80))));
    if ((int64_t)a0 == 0) { A1_W32B((uint32_t)s0, 8, fp); goto L_10d0770; }
    { A3_CHARGE(); v0 = GetMotionResourceFrames(ram, rt, v0, a0, t0, t1, t2, t3, (uint32_t)(sp + 0xFFFFFFA0u), f12, f13, f14, f15, f16, f17, f18, f19, h2); }
    { uint32_t bits = A1_R32B((uint32_t)s1, 12); float f; std::memcpy(&f, &bits, sizeof(f)); f3 = f; }
    (at = (uint32_t)((int32_t)((int32_t)((uint32_t)18371 << 16))));
    at = at | 0x5000u;
    { uint32_t bits = a7_opq(at); std::memcpy(&f0, &bits, sizeof(bits)); } a3xw0 = true;
    fcr31 = (a1f::lt(f3, f0)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
    { const bool c = (fcr31 & 0x800000u) == 0u; (v1 = (uint64_t)((uint64_t)v0 + (uint64_t)0ULL)); if (c) goto L_10d06d0; }
    { uint32_t bits = 0u; std::memcpy(&f1, &bits, sizeof(bits)); }
    fcr31 = (a1f::lt(f3, f1)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
    if ((fcr31 & 0x800000u) != 0u) goto L_10d04d0;
    { uint32_t bits = A1_R32B((uint32_t)s3, 40); float f; std::memcpy(&f, &bits, sizeof(f)); f0 = f; } a3xw0 = true;
    f3 = a1f::mul(f3, f0);
    fcr31 = (a1f::lt(f3, f1)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
L_10d04d0:;
    if ((fcr31 & 0x800000u) == 0u) { { uint32_t bits = A1_R32B((uint32_t)s3, 24); float f; std::memcpy(&f, &bits, sizeof(f)); f2 = f; } goto L_10d0600; }
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)s3, 32))));
    { const bool c = (int64_t)v0 == 0; f4 = ((float)(f1)); if (c) goto L_10d0848; }
    f0 = a1_flt(A1_R32B(s4, 92)); a3xw0 = true;
    { int32_t tmp; std::memcpy(&tmp, &f0, sizeof(tmp)); f0 = ((float)(int32_t)(tmp)); } a3xw0 = true;
    { uint32_t bits = A1_R32B((uint32_t)s5, 884); float f; std::memcpy(&f, &bits, sizeof(f)); f2 = f; }
    { uint32_t bits = A1_R32B((uint32_t)s3, 28); float f; std::memcpy(&f, &bits, sizeof(f)); f3 = f; }
    { uint32_t bits = A1_R32B((uint32_t)s5, 1316); float f; std::memcpy(&f, &bits, sizeof(f)); f1 = f; }
    f0 = a1f::mul(f0, f2); a3xw0 = true;
    f0 = a1f::mul(f0, f3); a3xw0 = true;
    f0 = a1f::mul(f0, f1); a3xw0 = true;
    fcr31 = (a1f::eq(f0, f4)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
    { const bool c = (fcr31 & 0x800000u) != 0u; A1_W32B((uint32_t)s0, 8, fp); if (c) goto L_10d051c; }
    f4 = a1f::div(fcr31, f20, f0);
L_10d051c:;
    { uint32_t bits = (uint32_t)v1; std::memcpy(&f1, &bits, sizeof(bits)); }
    { int32_t tmp; std::memcpy(&tmp, &f1, sizeof(tmp)); f1 = ((float)(int32_t)(tmp)); }
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)s1, 8))));
    { uint32_t bits = A1_R32B((uint32_t)s3, 8); float f; std::memcpy(&f, &bits, sizeof(f)); f0 = f; } a3xw0 = true;
    f0 = a1f::mul(f0, f4); a3xw0 = true;
    { float f = f1; uint32_t bits; std::memcpy(&bits, &f, sizeof(bits)); A1_W32B((uint32_t)s0, 16, bits); }
    { float f = f0; uint32_t bits; std::memcpy(&bits, &f, sizeof(bits)); A1_W32B((uint32_t)s0, 12, bits); }
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)v1, 0))));
    (v0 = (uint64_t)(RC_U64X(a0) & s6));
    { const bool c = (int64_t)v0 == 0; (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)SUB32((uint32_t)v1, (uint32_t)v0)))); if (c) goto L_10d0550; }
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)v0, 0))));
    goto L_10d0554;
L_10d0550:;
    (v1 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL));
L_10d0554:;
    { const bool c = (int64_t)v1 == 0; { const uint32_t _dynInsn = A1_R32(0x10D0558u); const int32_t _dynSimm = (int16_t)(_dynInsn & 0xFFFFu); const uint32_t _dynImm = _dynInsn & 0xFFFFu; (void)_dynSimm; (void)_dynImm; (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32(ADD32(s7, _dynSimm))))); } if (c) goto L_10d0580; }
    { const bool c = (int64_t)v1 == (int64_t)v0; (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)SRL32((uint32_t)a0, 18)))); if (c) goto L_10d0580; }
    (a1 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)v1, 8))));
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)v1, 16))));
    (v0 = (uint64_t)(RC_U64X(v0) & (uint64_t)(uint16_t)4095));
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)a1, 12))));
    { int64_t result = (int64_t)(int32_t)v0 * (int64_t)(int32_t)v1; lo = (uint64_t)(int64_t)(int32_t)result; hi = (uint64_t)(int64_t)(int32_t)(result >> 32); (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)result))); }
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)a0, (uint32_t)v0))));
    goto L_10d0584;
L_10d0580:;
    (v0 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL));
L_10d0584:;
    A1_W32B((uint32_t)s0, 20, (uint32_t)v0);
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B(s2, 0))));
    (v0 = (uint64_t)(RC_U64X(a0) & s6));
    { const bool c = (int64_t)v0 == 0; (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)SUB32(s2, (uint32_t)v0)))); if (c) goto L_10d05a0; }
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)v0, 0))));
    goto L_10d05a4;
L_10d05a0:;
    (v1 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL));
L_10d05a4:;
    { const bool c = (int64_t)v1 == 0; (v0 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL)); if (c) goto L_10d05d4; }
    { const uint32_t _dynInsn = A1_R32(0x10D05ACu); const int32_t _dynSimm = (int16_t)(_dynInsn & 0xFFFFu); const uint32_t _dynImm = _dynInsn & 0xFFFFu; (void)_dynSimm; (void)_dynImm; (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32(ADD32(s7, _dynSimm))))); }
    if ((int64_t)v1 == (int64_t)v0) { (v0 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL)); goto L_10d05d4; }
    (a1 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)v1, 8))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)SRL32((uint32_t)a0, 18))));
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)v1, 16))));
    (v0 = (uint64_t)(RC_U64X(v0) & (uint64_t)(uint16_t)4095));
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)a1, 12))));
    { int64_t result = (int64_t)(int32_t)v0 * (int64_t)(int32_t)v1; lo = (uint64_t)(int64_t)(int32_t)result; hi = (uint64_t)(int64_t)(int32_t)(result >> 32); (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)result))); }
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)a0, (uint32_t)v0))));
L_10d05d4:;
    A1_W32B((uint32_t)s0, 24, (uint32_t)v0);
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)s1, 24))));
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 1))));
    { uint32_t bits = A1_R32B((uint32_t)s1, 16); float f; std::memcpy(&f, &bits, sizeof(f)); f0 = f; } a3xw0 = true;
    { float f = f0; uint32_t bits; std::memcpy(&bits, &f, sizeof(bits)); A1_W32B((uint32_t)s0, 28, bits); }
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)s1, 20))));
    A1_W32B((uint32_t)s0, 36, (uint32_t)v0);
    A1_W32B((uint32_t)s0, 32, (uint32_t)v1);
    A1_W32B((uint32_t)s0, 40, 0u);
    goto L_10d084c;
L_10d0600:;
    fcr31 = (a1f::le(f3, f2)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
    { const bool c = (fcr31 & 0x800000u) == 0u; (a0 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL)); if (c) goto L_10d084c; }
    fcr31 = (a1f::eq(f3, f1)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
    if ((fcr31 & 0x800000u) == 0u) { { uint32_t bits = A1_R32B((uint32_t)s3, 12); float f; std::memcpy(&f, &bits, sizeof(f)); f1 = f; } goto L_10d06a8; }
    f0 = a1_flt(A1_R32B(s4, 92)); a3xw0 = true;
    { int32_t tmp; std::memcpy(&tmp, &f0, sizeof(tmp)); f0 = ((float)(int32_t)(tmp)); } a3xw0 = true;
    { uint32_t bits = A1_R32B((uint32_t)s5, 884); float f; std::memcpy(&f, &bits, sizeof(f)); f2 = f; }
    { uint32_t bits = A1_R32B((uint32_t)s3, 28); float f; std::memcpy(&f, &bits, sizeof(f)); f3 = f; }
    f4 = ((float)(f1));
    { uint32_t bits = A1_R32B((uint32_t)s5, 1316); float f; std::memcpy(&f, &bits, sizeof(f)); f1 = f; }
    f0 = a1f::mul(f0, f2); a3xw0 = true;
    f0 = a1f::mul(f0, f3); a3xw0 = true;
    f0 = a1f::mul(f0, f1); a3xw0 = true;
    fcr31 = (a1f::eq(f0, f4)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
    { const bool c = (fcr31 & 0x800000u) != 0u; { uint32_t bits = A1_R32B((uint32_t)s3, 8); float f; std::memcpy(&f, &bits, sizeof(f)); f1 = f; } if (c) goto L_10d0654; }
    f4 = a1f::div(fcr31, f20, f0);
L_10d0654:;
    { uint32_t bits = A1_R32B((uint32_t)s3, 12); float f; std::memcpy(&f, &bits, sizeof(f)); f0 = f; } a3xw0 = true;
    { uint32_t bits = 0u; std::memcpy(&f2, &bits, sizeof(bits)); }
    f1 = a1f::sub(f1, f0);
    fcr31 = (a1f::lt(f1, f2)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
    if ((fcr31 & 0x800000u) == 0u) { f1 = a1f::add(f1, f2); goto L_10d0678; }
    { uint32_t bits = (uint32_t)v1; std::memcpy(&f0, &bits, sizeof(bits)); } a3xw0 = true;
    { int32_t tmp; std::memcpy(&tmp, &f0, sizeof(tmp)); f0 = ((float)(int32_t)(tmp)); } a3xw0 = true;
    f1 = a1f::add(f1, f0);
L_10d0678:;
    { uint32_t bits = 0u; std::memcpy(&f0, &bits, sizeof(bits)); } a3xw0 = true;
    fcr31 = (a1f::lt(f1, f0)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
    if ((fcr31 & 0x800000u) == 0u) { f0 = ((float)(f1)); a3xw0 = true; goto L_10d0688; }
L_10d0688:;
    f1 = a1f::mul(f0, f4);
    A1_W32B((uint32_t)s0, 8, fp);
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)s1, 8))));
    { float f = f1; uint32_t bits; std::memcpy(&bits, &f, sizeof(bits)); A1_W32B((uint32_t)s0, 12, bits); }
    { uint32_t bits = A1_R32B((uint32_t)s3, 12); float f; std::memcpy(&f, &bits, sizeof(f)); f0 = f; } a3xw0 = true;
    { float f = f0; uint32_t bits; std::memcpy(&bits, &f, sizeof(bits)); A1_W32B((uint32_t)s0, 16, bits); }
    goto L_10d077c;
L_10d06a8:;
    f0 = a1f::sub(f2, f1); a3xw0 = true;
    f0 = a1f::sub(f2, f0); a3xw0 = true;
    fcr31 = (a1f::lt(f3, f0)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
    if ((fcr31 & 0x800000u) == 0u) { f1 = ((float)(f3)); goto L_10d06bc; }
L_10d06bc:;
    { float f = f1; uint32_t bits; std::memcpy(&bits, &f, sizeof(bits)); A1_W32B((uint32_t)s0, 16, bits); }
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)s1, 8))));
    A1_W32B((uint32_t)s0, 8, fp);
    A1_W32B((uint32_t)s0, 12, 0u);
    goto L_10d077c;
L_10d06d0:;
    f3 = a1f::sub(f3, f0);
    { uint32_t bits = A1_R32B((uint32_t)s5, 1076); float f; std::memcpy(&f, &bits, sizeof(f)); f0 = f; } a3xw0 = true;
    { uint32_t bits = A1_R32B((uint32_t)s3, 40); float f; std::memcpy(&f, &bits, sizeof(f)); f1 = f; }
    { uint32_t bits = A1_R32B((uint32_t)s3, 24); float f; std::memcpy(&f, &bits, sizeof(f)); f2 = f; }
    { uint32_t bits = A1_R32B((uint32_t)s3, 8); float f; std::memcpy(&f, &bits, sizeof(f)); f4 = f; }
    f3 = a1f::mul(f3, f1);
    f2 = a1f::sub(f2, f0);
    { uint32_t bits = 0u; std::memcpy(&f0, &bits, sizeof(bits)); } a3xw0 = true;
    f6 = a1f::subFinite(f2, f3);
    f1 = a1f::sub(f4, f6);
    fcr31 = (a1f::lt(f1, f0)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
    if ((fcr31 & 0x800000u) == 0u) { f5 = a1f::add(f1, f0); goto L_10d0710; }
    { uint32_t bits = (uint32_t)v1; std::memcpy(&f0, &bits, sizeof(bits)); } a3xw0 = true;
    { int32_t tmp; std::memcpy(&tmp, &f0, sizeof(tmp)); f0 = ((float)(int32_t)(tmp)); } a3xw0 = true;
    f5 = a1f::add(f1, f0);
L_10d0710:;
    fcr31 = (a1f::le(f3, f2)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
    { const bool c = (fcr31 & 0x800000u) == 0u; (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 2)))); if (c) goto L_10d084c; }
    f0 = a1_flt(A1_R32B(s4, 92)); a3xw0 = true;
    { int32_t tmp; std::memcpy(&tmp, &f0, sizeof(tmp)); f0 = ((float)(int32_t)(tmp)); } a3xw0 = true;
    { uint32_t bits = A1_R32B((uint32_t)s5, 884); float f; std::memcpy(&f, &bits, sizeof(f)); f2 = f; }
    { uint32_t bits = A1_R32B((uint32_t)s3, 28); float f; std::memcpy(&f, &bits, sizeof(f)); f3 = f; }
    { uint32_t bits = A1_R32B((uint32_t)s5, 1316); float f; std::memcpy(&f, &bits, sizeof(f)); f4 = f; }
    { uint32_t bits = 0u; std::memcpy(&f1, &bits, sizeof(bits)); }
    f0 = a1f::mul(f0, f2); a3xw0 = true;
    f0 = a1f::mul(f0, f3); a3xw0 = true;
    f0 = a1f::mul(f0, f4); a3xw0 = true;
    fcr31 = (a1f::eq(f0, f1)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
    if ((fcr31 & 0x800000u) != 0u) { f0 = a1f::mul(f6, f1); a3xw0 = true; goto L_10d075c; }
    f1 = a1f::div(fcr31, f20, f0);
    f0 = ((float)(f6) * (float)(f1)); a3xw0 = true;
L_10d075c:;
    { float f = f5; uint32_t bits; std::memcpy(&bits, &f, sizeof(bits)); A1_W32B((uint32_t)s0, 16, bits); }
    A1_W32B((uint32_t)s0, 8, fp);
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)s1, 8))));
    { float f = f0; uint32_t bits; std::memcpy(&bits, &f, sizeof(bits)); A1_W32B((uint32_t)s0, 12, bits); }
    goto L_10d077c;
L_10d0770:;
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)s1, 8))));
    A1_W32B((uint32_t)s0, 12, 0u);
    A1_W32B((uint32_t)s0, 16, 0u);
L_10d077c:;
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)v1, 0))));
    (v0 = (uint64_t)(RC_U64X(a0) & s6));
    { const bool c = (int64_t)v0 == 0; (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)SUB32((uint32_t)v1, (uint32_t)v0)))); if (c) goto L_10d0798; }
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)v0, 0))));
    goto L_10d079c;
L_10d0798:;
    (v1 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL));
L_10d079c:;
    { const bool c = (int64_t)v1 == 0; { const uint32_t _dynInsn = A1_R32(0x10D07A0u); const int32_t _dynSimm = (int16_t)(_dynInsn & 0xFFFFu); const uint32_t _dynImm = _dynInsn & 0xFFFFu; (void)_dynSimm; (void)_dynImm; (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32(ADD32(s7, _dynSimm))))); } if (c) goto L_10d07c8; }
    { const bool c = (int64_t)v1 == (int64_t)v0; (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)SRL32((uint32_t)a0, 18)))); if (c) goto L_10d07c8; }
    (a1 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)v1, 8))));
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)v1, 16))));
    (v0 = (uint64_t)(RC_U64X(v0) & (uint64_t)(uint16_t)4095));
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)a1, 12))));
    { int64_t result = (int64_t)(int32_t)v0 * (int64_t)(int32_t)v1; lo = (uint64_t)(int64_t)(int32_t)result; hi = (uint64_t)(int64_t)(int32_t)(result >> 32); (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)result))); }
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)a0, (uint32_t)v0))));
    goto L_10d07cc;
L_10d07c8:;
    (v0 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL));
L_10d07cc:;
    A1_W32B((uint32_t)s0, 20, (uint32_t)v0);
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B(s2, 0))));
    (v0 = (uint64_t)(RC_U64X(a0) & s6));
    { const bool c = (int64_t)v0 == 0; (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)SUB32(s2, (uint32_t)v0)))); if (c) goto L_10d07e8; }
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)v0, 0))));
    goto L_10d07ec;
L_10d07e8:;
    (v1 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL));
L_10d07ec:;
    { const bool c = (int64_t)v1 == 0; (v0 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL)); if (c) goto L_10d081c; }
    { const uint32_t _dynInsn = A1_R32(0x10D07F4u); const int32_t _dynSimm = (int16_t)(_dynInsn & 0xFFFFu); const uint32_t _dynImm = _dynInsn & 0xFFFFu; (void)_dynSimm; (void)_dynImm; (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32(ADD32(s7, _dynSimm))))); }
    if ((int64_t)v1 == (int64_t)v0) { (v0 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL)); goto L_10d081c; }
    (a1 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)v1, 8))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)SRL32((uint32_t)a0, 18))));
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)v1, 16))));
    (v0 = (uint64_t)(RC_U64X(v0) & (uint64_t)(uint16_t)4095));
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)a1, 12))));
    { int64_t result = (int64_t)(int32_t)v0 * (int64_t)(int32_t)v1; lo = (uint64_t)(int64_t)(int32_t)result; hi = (uint64_t)(int64_t)(int32_t)(result >> 32); (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)result))); }
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)a0, (uint32_t)v0))));
L_10d081c:;
    A1_W32B((uint32_t)s0, 24, (uint32_t)v0);
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)s1, 24))));
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 1))));
    { uint32_t bits = A1_R32B((uint32_t)s1, 16); float f; std::memcpy(&f, &bits, sizeof(f)); f0 = f; } a3xw0 = true;
    { float f = f0; uint32_t bits; std::memcpy(&bits, &f, sizeof(bits)); A1_W32B((uint32_t)s0, 28, bits); }
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)s1, 20))));
    A1_W32B((uint32_t)s0, 36, (uint32_t)v0);
    A1_W32B((uint32_t)s0, 32, (uint32_t)v1);
    A1_W32B((uint32_t)s0, 40, fp);
    goto L_10d084c;
L_10d0848:;
    (a0 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL));
L_10d084c:;
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 2))));
    if (RC_U64X(a0) == 0) do { const a4::q128 _v = (a4::mk128(h30, a1_sx64(fp))); h2 = a4::hi64(_v); std::memcpy(&v0, &_v, 8); } while (0);
    goto L_10d0868;
L_10d0858:;
    (v0 = (uint64_t)(((int64_t)(int64_t)t1 < (int64_t)(int64_t)t0) ? 1 : 0));
    { const bool c = (int64_t)v0 != 0; (s1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)s1, 28)))); if (c) { A3_BACK(); goto L_10d02d0; } }
L_10d0864:;
    (v0 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL));
L_10d0868:;
    f20 = a1_flt(a2::frLd<uint32_t>(a2fr, 80));
    { if (xo) { if (a3xw0) xo[0] = a1_bits(f0); xo[1] = a1_bits(f20); } io_h2 = h2; return v0; }
    __builtin_unreachable();
}

// ViewIntervalClip (0x1114858): typed by tools/typed_lift2.py (round A6; guest frame calls, extra outputs xo[0]=f2 xo[1]=f3 xo[2]=f12 xo[3]=f20 xo[4]=f21 xo[5]=a0 xo[6]=a1 xo[7]=a2; VU0 in vf0 vf6 vf7 vf8 vf10 vacc h5, out vf4 vf5 vf6 vf7 vf8 vf10 vf14 vf15 vf16 vf17 vacc h2)
inline a1::RetVF ViewIntervalClip(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint64_t v0, uint64_t a0, uint64_t a1, uint32_t sp, float f12, float f13, float f14, float f15, float f16, float f17, float f18, float f19, float f20, float f21, const a4::vec4 &io_vf0, a4::vec4 &io_vf4, a4::vec4 &io_vf5, a4::vec4 &io_vf6, a4::vec4 &io_vf7, a4::vec4 &io_vf8, a4::vec4 &io_vf10, a4::vec4 &io_vf14, a4::vec4 &io_vf15, a4::vec4 &io_vf16, a4::vec4 &io_vf17, a4::vec4 &io_vacc, uint64_t &io_h2, uint64_t io_h5, uint64_t *xo = nullptr)
{
    float f0 = 0.0f;
    float f1 = 0.0f;
    float f2 = 0.0f;
    float f3 = 0.0f;
    uint32_t at = 0;
    uint32_t v1 = 0;
    uint64_t a2 = 0;
    uint64_t s0 = 0;
    uint64_t s1 = 0;
    uint64_t s2 = 0;
    uint64_t s3 = 0;
    uint64_t hi = 0;
    uint64_t lo = 0;
    uint32_t fcr31 = 0;
    a4::vec4 vf0 = io_vf0;
    a4::vec4 vf4 = io_vf4;
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
    uint64_t h5 = io_h5;
    bool a3xw0 = false;
    bool a3xw1 = false;
    bool a3xw2 = false;
    bool a3xw7 = false;
    sp = sp - 0x60u;
    (s2 = (uint64_t)((uint64_t)a0 + (uint64_t)0ULL));
    (s3 = (uint64_t)((uint64_t)a1 + (uint64_t)0ULL));
    A1_W32B(sp, 80, a1_bits(f21));
    f21 = ((float)(f12));
    { A1_W32B(sp, 72, a1_bits(f20)); A3_CHARGE(); uint64_t a7c[2] = {0u, 0u}; v0 = GetCharacterClass_x8(ram, rt, a0, h2, h5, a7c); a0 = a7c[0]; a1 = a7c[1]; }
    (s0 = (uint64_t)((uint64_t)v0 + (uint64_t)0ULL));
    at = 0xBF800000u;
    { uint32_t bits = a7_opq(at); std::memcpy(&f0, &bits, sizeof(bits)); }
    { uint32_t bits = A1_R32B((uint32_t)s0, 4460); float f; std::memcpy(&f, &bits, sizeof(f)); f1 = f; }
    fcr31 = (a1f::eq(f1, f0)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
    if ((fcr31 & 0x800000u) == 0u) { { uint32_t bits = A1_R32B((uint32_t)s0, 4456); float f; std::memcpy(&f, &bits, sizeof(f)); f3 = f; } a3xw1 = true; goto L_11148c8; }
    { uint32_t bits = A1_R32B((uint32_t)s0, 4456); float f; std::memcpy(&f, &bits, sizeof(f)); f1 = f; }
    { uint32_t bits = A1_R32B((uint32_t)s0, 4452); float f; std::memcpy(&f, &bits, sizeof(f)); f0 = f; }
    f1 = a1f::sub(f21, f1);
    { float f = f21; uint32_t bits; std::memcpy(&bits, &f, sizeof(bits)); A1_W32B((uint32_t)s0, 4456, bits); }
    A1_W32B((uint32_t)s0, 4452, 0u);
    f0 = a1f::add(f0, f1);
    { float f = f0; uint32_t bits; std::memcpy(&bits, &f, sizeof(bits)); A1_W32B((uint32_t)s0, 4460, bits); }
    goto L_1114acc;
L_11148c8:;
    fcr31 = (a1f::lt(f3, f21)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
    { const bool c = (fcr31 & 0x800000u) == 0u; f0 = ((float)(f1)); if (c) goto L_1114acc; }
    (v1 = (uint32_t)((int32_t)((int32_t)A1_R32B((uint32_t)s0, 4416))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)3 << 16))));
    (v0 = (uint64_t)(RC_U64X(v0) | (uint64_t)(uint16_t)65535));
    { uint32_t bits = 0u; std::memcpy(&f2, &bits, sizeof(bits)); } a3xw0 = true;
    v1 = v1 + 0x12Cu;
    (a2 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B(v1, 0)))); a3xw7 = true;
    (v0 = (uint64_t)(RC_U64X(a2) & RC_U64X(v0)));
    { const bool c = (int64_t)v0 == 0; (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)SUB32(v1, (uint32_t)v0)))); if (c) goto L_1114900; }
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)v0, 0))));
    goto L_1114904;
L_1114900:;
    (a0 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL));
L_1114904:;
    { const bool c = (int64_t)a0 == 0; { const uint32_t _dynInsn = A1_R32(0x1114908u); const int32_t _dynSimm = (int16_t)(_dynInsn & 0xFFFFu); const uint32_t _dynImm = _dynInsn & 0xFFFFu; (void)_dynSimm; (void)_dynImm; (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)_dynImm << 16)))); } if (c) goto L_1114938; }
    { const uint32_t _dynInsn = A1_R32(0x111490Cu); const int32_t _dynSimm = (int16_t)(_dynInsn & 0xFFFFu); const uint32_t _dynImm = _dynInsn & 0xFFFFu; (void)_dynSimm; (void)_dynImm; (v1 = (uint32_t)((int32_t)((int32_t)A1_R32(ADD32((uint32_t)v0, _dynSimm))))); }
    { const bool c = (int64_t)a0 == (int64_t)(int32_t)v1; (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)SRL32((uint32_t)a2, 18)))); if (c) goto L_1114938; }
    (a1 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)a0, 8))));
    (v0 = (uint64_t)(RC_U64X(v0) & (uint64_t)(uint16_t)4095));
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)a0, 16))));
    (v1 = (uint32_t)((int32_t)((int32_t)A1_R32B((uint32_t)a1, 12))));
    { int64_t result = (int64_t)(int32_t)v0 * (int64_t)(int32_t)v1; lo = (uint64_t)(int64_t)(int32_t)result; hi = (uint64_t)(int64_t)(int32_t)(result >> 32); (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)result))); }
    (s1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)a0, (uint32_t)v0))));
    goto L_111493c;
L_1114938:;
    (s1 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL));
L_111493c:;
    if ((int64_t)s1 == 0) { f1 = a1f::sub(f21, f3); goto L_1114a98; }
    { (a0 = (uint64_t)((uint64_t)s2 + (uint64_t)0ULL)); A3_CHARGE(); v0 = GetLayoutClass(ram, rt, a0, f12, f13, f14, f15, f16, f17, f18, f19, h2); }
    (a0 = (uint64_t)((uint64_t)s3 + (uint64_t)0ULL));
    { (s2 = (uint64_t)((uint64_t)v0 + (uint64_t)0ULL)); A3_CHARGE(); v0 = GetLayoutClass(ram, rt, a0, f12, f13, f14, f15, f16, f17, f18, f19, h2); }
    (a1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(sp, 16))));
    (v1 = (uint32_t)((int32_t)((int32_t)A1_R32B((uint32_t)v0, 308))));
    (a2 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)s2, 308)))); a3xw7 = true;
    vf4 = a4v::castsi128_ps(A4_R128(ADD32((uint32_t)a2, 0)));
    vf5 = a4v::castsi128_ps(A4_R128(ADD32(v1, 0)));
    { a4::vec4 res = a4v::sub(vf4, vf5); a4::q128 mask = a4v::set_epi32(-1, -1, -1, -1); vf6 = a4v::blend(vf6, res, a4v::castsi128_ps(mask)); }
    A4_W128(ADD32((uint32_t)a1, 0), a4v::castps_si128(vf6));
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)v0, 316))));
    vf4 = a4v::castsi128_ps(A4_R128(ADD32((uint32_t)a0, 0)));
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
    A4_W128(ADD32(sp, 0), a4v::castps_si128(vf6));
    vf4 = a4v::castsi128_ps(A4_R128(ADD32(sp, 0)));
    vf5 = a4v::castsi128_ps(A4_R128(ADD32((uint32_t)a1, 0)));
    { a4::vec4 res = a4v::mul(vf4, vf5); a4::q128 mask = a4v::set_epi32(0, -1, -1, -1); vf5 = a4v::blend(vf5, res, a4v::castsi128_ps(mask)); }
    { a4::vec4 res = a4v::addK<1, 1>(vf5, a4v::bcast<1>(vf5)); a4::q128 mask = a4v::set_epi32(0, 0, 0, -1); vf5 = a4v::blendv_ps(vf5, res, a4v::castsi128_ps(mask)); }
    { a4::vec4 res = a4v::addK<1, 1>(vf5, a4v::bcast<2>(vf5)); a4::q128 mask = a4v::set_epi32(0, 0, 0, -1); vf5 = a4v::blendv_ps(vf5, res, a4v::castsi128_ps(mask)); }
    do { const a4::q128 _v = (a4v::castps_si128(vf5)); h2 = a4::hi64(_v); std::memcpy(&v0, &_v, 8); } while (0);
    { uint32_t bits = (uint32_t)v0; std::memcpy(&f2, &bits, sizeof(bits)); } a3xw0 = true;
    vf4 = a4v::castsi128_ps(A4_R128(ADD32((uint32_t)a1, 0)));
    { a4::vec4 res = a4v::mul(vf4, vf4); a4::q128 mask = a4v::set_epi32(0, -1, -1, -1); vf5 = a4v::blend(vf5, res, a4v::castsi128_ps(mask)); }
    { a4::vec4 res = a4v::addK<1, 1>(vf5, a4v::bcast<1>(vf5)); a4::q128 mask = a4v::set_epi32(0, 0, 0, -1); vf5 = a4v::blendv_ps(vf5, res, a4v::castsi128_ps(mask)); }
    { a4::vec4 res = a4v::addK<1, 1>(vf5, a4v::bcast<2>(vf5)); a4::q128 mask = a4v::set_epi32(0, 0, 0, -1); vf5 = a4v::blendv_ps(vf5, res, a4v::castsi128_ps(mask)); }
    do { const a4::q128 _v = (a4v::castps_si128(vf5)); h2 = a4::hi64(_v); std::memcpy(&v0, &_v, 8); } while (0);
    { uint32_t bits = (uint32_t)v0; std::memcpy(&f0, &bits, sizeof(bits)); }
    f0 = a1f::div(fcr31, f2, f0);
    f0 = a1f::sqrt(fcr31, f0);
    { uint32_t bits = 0u; std::memcpy(&f1, &bits, sizeof(bits)); }
    fcr31 = (a1f::lt(f2, f1)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
    { const bool c = (fcr31 & 0x800000u) == 0u; f20 = ((float)(f0)); if (c) goto L_1114a18; }
    f20 = (-(float)(f0));
L_1114a18:;
    { A3_CHARGE(); v0 = a1_sx64(iosgluGetCameraContext(ram, rt, f12, f13, f14, f15, f16, f17, f18, f19)); }
    (a1 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)s2, 308))));
    at = 0x3F800000u;
    { uint32_t bits = a7_opq(at); std::memcpy(&f12, &bits, sizeof(bits)); } a3xw2 = true;
    { (a0 = (uint64_t)((uint64_t)v0 + (uint64_t)0ULL)); A3_CHARGE(); uint64_t a7c[2] = {0u, 0u}; { const a1::RetVF a2_r = GetScreenRadius_x8(ram, rt, (uint32_t)a0, (uint32_t)a1, sp, f12, vf8, vf10, vf14, vf15, vf16, vf17, vacc, a7c); v0 = a2_r.v0; f0 = a2_r.f0; } a0 = a7c[0]; a2 = a7c[1]; a3xw7 = true; }
    { uint32_t bits = A1_R32B((uint32_t)s1, 4); float f; std::memcpy(&f, &bits, sizeof(f)); f3 = f; } a3xw1 = true;
    fcr31 = (a1f::le(f0, f3)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
    if ((fcr31 & 0x800000u) != 0u) { { uint32_t bits = A1_R32B((uint32_t)s1, 12); float f; std::memcpy(&f, &bits, sizeof(f)); f2 = f; } a3xw0 = true; goto L_1114a58; }
    { uint32_t bits = 0u; std::memcpy(&f1, &bits, sizeof(bits)); }
    fcr31 = (a1f::lt(f20, f1)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
    if ((fcr31 & 0x800000u) == 0u) { { uint32_t bits = A1_R32B((uint32_t)s1, 8); float f; std::memcpy(&f, &bits, sizeof(f)); f2 = f; } a3xw0 = true; goto L_1114a60; }
    { uint32_t bits = A1_R32B((uint32_t)s1, 12); float f; std::memcpy(&f, &bits, sizeof(f)); f2 = f; } a3xw0 = true;
L_1114a58:;
    { uint32_t bits = A1_R32B((uint32_t)s0, 4456); float f; std::memcpy(&f, &bits, sizeof(f)); f3 = f; } a3xw1 = true;
    goto L_1114a94;
L_1114a60:;
    fcr31 = (a1f::le(f2, f0)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
    if ((fcr31 & 0x800000u) == 0u) { f0 = a1f::sub(f2, f0); goto L_1114a78; }
    f2 = ((float)(f1)); a3xw0 = true;
    { uint32_t bits = A1_R32B((uint32_t)s0, 4456); float f; std::memcpy(&f, &bits, sizeof(f)); f3 = f; } a3xw1 = true;
    goto L_1114a94;
L_1114a78:;
    { uint32_t bits = A1_R32B((uint32_t)s1, 12); float f; std::memcpy(&f, &bits, sizeof(f)); f1 = f; }
    f2 = a1f::sub(f2, f3); a3xw0 = true;
    { uint32_t bits = A1_R32B((uint32_t)s0, 4456); float f; std::memcpy(&f, &bits, sizeof(f)); f3 = f; } a3xw1 = true;
    f1 = a1f::mul(f1, f0);
    f2 = a1f::div(fcr31, f1, f2); a3xw0 = true;
L_1114a94:;
    f1 = a1f::sub(f21, f3);
L_1114a98:;
    { uint32_t bits = A1_R32B((uint32_t)s0, 4452); float f; std::memcpy(&f, &bits, sizeof(f)); f0 = f; }
    { float f = f21; uint32_t bits; std::memcpy(&bits, &f, sizeof(bits)); A1_W32B((uint32_t)s0, 4456, bits); }
    f0 = a1f::add(f0, f1);
    fcr31 = (a1f::le(f2, f0)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
    { const bool c = (fcr31 & 0x800000u) == 0u; { float f = f0; uint32_t bits; std::memcpy(&bits, &f, sizeof(bits)); A1_W32B((uint32_t)s0, 4452, bits); } if (c) goto L_1114ac0; }
    A1_W32B((uint32_t)s0, 4452, 0u);
    f1 = ((float)(f0));
    { float f = f0; uint32_t bits; std::memcpy(&bits, &f, sizeof(bits)); A1_W32B((uint32_t)s0, 4460, bits); }
    goto L_1114ac8;
L_1114ac0:;
    A1_W32B((uint32_t)s0, 4460, 0u);
    { uint32_t bits = A1_R32B((uint32_t)s0, 4460); float f; std::memcpy(&f, &bits, sizeof(f)); f1 = f; }
L_1114ac8:;
    f0 = ((float)(f1));
L_1114acc:;
    f21 = a1_flt(A1_R32B(sp, 80));
    f20 = a1_flt(A1_R32B(sp, 72));
    { sp = sp + 0x60u; if (xo) { if (a3xw0) xo[0] = a1_bits(f2); if (a3xw1) xo[1] = a1_bits(f3); if (a3xw2) xo[2] = a1_bits(f12); xo[3] = a1_bits(f20); xo[4] = a1_bits(f21); xo[5] = a0; xo[6] = a1; if (a3xw7) xo[7] = a2; } io_vf4 = vf4; io_vf5 = vf5; io_vf6 = vf6; io_vf7 = vf7; io_vf8 = vf8; io_vf10 = vf10; io_vf14 = vf14; io_vf15 = vf15; io_vf16 = vf16; io_vf17 = vf17; io_vacc = vacc; io_h2 = h2; return a1::RetVF{(uint64_t)v0, f0}; }
    __builtin_unreachable();
}

// GetCollisionDBSurfBoundary (0x11193c8): typed by tools/typed_lift2.py (round A6; guest frame calls, extra outputs xo[0]=f0 xo[1]=f2 xo[2]=a0 xo[3]=a1 xo[4]=a2 xo[5]=a3 xo[6]=t0 xo[7]=t1 xo[8]=t2 xo[9]=t3; VU0 in vf0 vf7 vf10 vf29 vf31 vacc h8 h11, out vf4 vf5 vf6 vf7 vf8 vf10 vf14 vf15 vf16 vf17 vf29 vf30 vf31 vacc h2 h3 h11)
inline uint64_t GetCollisionDBSurfBoundary(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint64_t a0, uint64_t a1, uint64_t t3, uint64_t sp, float f2, float f12, float f13, float f14, float f15, float f16, float f17, float f18, float f19, const a4::vec4 &io_vf0, a4::vec4 &io_vf4, a4::vec4 &io_vf5, a4::vec4 &io_vf6, a4::vec4 &io_vf7, a4::vec4 &io_vf8, a4::vec4 &io_vf10, a4::vec4 &io_vf14, a4::vec4 &io_vf15, a4::vec4 &io_vf16, a4::vec4 &io_vf17, a4::vec4 &io_vf29, a4::vec4 &io_vf30, a4::vec4 &io_vf31, a4::vec4 &io_vacc, uint64_t &io_h2, uint64_t &io_h3, uint64_t io_h8, uint64_t &io_h11, uint64_t *xo = nullptr)
{
    float f0 = 0.0f;
    uint32_t at = 0;
    uint64_t v0 = 0;
    uint64_t v1 = 0;
    uint64_t a2 = 0;
    uint64_t a3 = 0;
    uint64_t t0 = 0;
    uint64_t t1 = 0;
    uint64_t t2 = 0;
    uint64_t s0 = 0;
    uint64_t s1 = 0;
    uint64_t s2 = 0;
    uint64_t s3 = 0;
    a4::vec4 vf0 = io_vf0;
    a4::vec4 vf4 = io_vf4;
    a4::vec4 vf5 = io_vf5;
    a4::vec4 vf6 = io_vf6;
    a4::vec4 vf7 = io_vf7;
    a4::vec4 vf8 = io_vf8;
    a4::vec4 vf10 = io_vf10;
    a4::vec4 vf14 = io_vf14;
    a4::vec4 vf15 = io_vf15;
    a4::vec4 vf16 = io_vf16;
    a4::vec4 vf17 = io_vf17;
    a4::vec4 vf29 = io_vf29;
    a4::vec4 vf30 = io_vf30;
    a4::vec4 vf31 = io_vf31;
    a4::vec4 vacc = io_vacc;
    uint64_t h2 = io_h2;
    uint64_t h3 = io_h3;
    uint64_t h8 = io_h8;
    uint64_t h11 = io_h11;
    bool a3xw0 = false;
    bool a3xw1 = false;
    bool a3xw8 = false;
    bool a3xw9 = false;
    (sp = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)sp, 4294967232))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)255 << 16))));
    (v0 = (uint64_t)(RC_U64X(v0) | (uint64_t)(uint16_t)65535));
    (s2 = (uint64_t)((uint64_t)a1 + (uint64_t)0ULL));
    (s3 = (uint64_t)((uint64_t)a0 + (uint64_t)0ULL));
    (s1 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)s2, 4))));
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)s2, 0))));
    (s0 = (uint64_t)((int64_t)(int32_t)((int32_t)SRL32((uint32_t)s1, 24))));
    (s1 = (uint64_t)(RC_U64X(s1) & RC_U64X(v0)));
    { (s0 = (uint64_t)(RC_U64X(s0) & (uint64_t)(uint16_t)127)); A3_CHARGE(); v0 = GetLayoutClass(ram, rt, a0, f12, f13, f14, f15, f16, f17, f18, f19, h2); }
    (a3 = (uint64_t)((uint64_t)v0 + (uint64_t)0ULL));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)SLL32((uint32_t)s0, 3))));
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)s2, 4))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v0, (uint32_t)s0))));
    (a2 = (uint64_t)((uint64_t)s1 + (uint64_t)0ULL));
    (s1 = (uint64_t)((int64_t)(int32_t)((int32_t)SLL32((uint32_t)s1, 2))));
    (a0 = (uint64_t)((uint64_t)a3 + (uint64_t)0ULL));
    (a1 = (uint64_t)((uint64_t)s0 + (uint64_t)0ULL));
    { const bool c = (int32_t)v1 >= 0; (t0 = (uint64_t)((int64_t)(int32_t)((int32_t)SLL32((uint32_t)v0, 4)))); if (c) goto L_1119450; }
    { A3_CHARGE(); uint64_t a7c[9] = {(uint64_t)a1_bits(f2), 0u, 0u, a2, 0u, 0u, 0u, 0u, t3}; v0 = getLayoutClassCollisionIndepSurfBoundary_x8(ram, rt, a0, a1, a2, (uint32_t)sp, f12, f13, f14, f15, f16, f17, f18, f19, vf8, vf10, vf14, vf15, vf16, vf17, vf29, vf30, vf31, vacc, h3, a7c); f2 = a1_flt((uint32_t)a7c[0]); a0 = a7c[1]; a1 = a7c[2]; a2 = a7c[3]; a3 = a7c[4]; t0 = a7c[5]; t1 = a7c[6]; t2 = a7c[7]; t3 = a7c[8]; a3xw1 = true; a3xw8 = true; a3xw9 = true; }
    do { const a4::q128 _v = (A4_R128(ADD32((uint32_t)v0, 0))); h3 = a4::hi64(_v); std::memcpy(&v1, &_v, 8); } while (0);
    A4_W128(ADD32((uint32_t)s3, 0), a4::mk128(h3, v1));
    { uint32_t bits = A1_R32B((uint32_t)s3, 12); float f; std::memcpy(&f, &bits, sizeof(f)); f0 = f; } a3xw0 = true;
    f0 = a1f::mul(f0, f0); a3xw0 = true;
    { float f = f0; uint32_t bits; std::memcpy(&bits, &f, sizeof(bits)); A1_W32B((uint32_t)s3, 12, bits); }
    goto L_111954c;
L_1119450:;
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)a3, 388))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)a3, 372))));
    (a2 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)v1, 32))));
    (s2 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v0, 112))));
    (t1 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)a2, 8))));
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)s2, 8))));
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)t0, (uint32_t)t1))));
    (a1 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)s2, 20))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)v1, 84))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)s1, (uint32_t)v0))));
    (s1 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)v0, 0))));
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)a0, (uint32_t)s1))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)SLL32((uint32_t)s1, 4))));
    (v1 = (uint64_t)(uint32_t)((uint8_t)A1_R8B((uint32_t)a0, 0)));
    (t0 = (uint64_t)((int64_t)(int32_t)((int32_t)SLL32((uint32_t)s1, 6))));
    { const bool c = (int64_t)v1 != 0; (s0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)a1, (uint32_t)v0)))); if (c) goto L_1119544; }
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)a3, 340))));
    (t3 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)a2, 24)))); a3xw9 = true;
    (t2 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)a2, 20)))); a3xw8 = true;
    (a0 = (uint64_t)((uint64_t)sp + (uint64_t)0ULL));
    (a2 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)v0, 32))));
    (t3 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)t3, (uint32_t)t0)))); a3xw9 = true;
    (t0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)v0, 8))));
    (a1 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL));
    (a3 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)a2, 36))));
    { (a2 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)a2, 40)))); A3_CHARGE(); uint64_t a7c[7] = {0u, 0u, 0u, 0u, 0u, 0u, t3}; v0 = manager_sub_01116108(ram, rt, a0, a1, a2, a3, t0, t1, t2, t3, sp, f12, f13, f14, f15, f16, f17, f18, f19, vf8, vf10, vf14, vf15, vf16, vf17, vf29, vf30, vf31, vacc, h11, a7c); t3 = a7c[6]; a3xw9 = true; }
    (at = (uint32_t)((int32_t)((int32_t)((uint32_t)16042 << 16))));
    at = at | 0xAAAAu;
    { uint32_t bits = a7_opq(at); std::memcpy(&f0, &bits, sizeof(bits)); } a3xw0 = true;
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)sp, 0))));
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)sp, 4))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)sp, 8))));
    vf4 = a4v::castsi128_ps(A4_R128(ADD32((uint32_t)a0, 0)));
    vf5 = a4v::castsi128_ps(A4_R128(ADD32((uint32_t)v1, 0)));
    vf6 = a4v::castsi128_ps(A4_R128(ADD32((uint32_t)v0, 0)));
    { uint32_t bits; std::memcpy(&bits, &f0, sizeof(bits)); (t0 = (uint64_t)((int64_t)(int32_t)(bits))); }
    vf8 = a4v::castsi128_ps(a4::mk128(h8, t0));
    { a4::vec4 res = a4v::add(vf4, vf5); a4::q128 mask = a4v::set_epi32(0, -1, -1, -1); vf7 = a4v::blend(vf7, res, a4v::castsi128_ps(mask)); }
    { a4::vec4 res = a4v::addK<1, 0>(vf7, vf6); a4::q128 mask = a4v::set_epi32(0, -1, -1, -1); vf7 = a4v::blend(vf7, res, a4v::castsi128_ps(mask)); }
    { a4::vec4 res = a4v::mulK<1, 0>(vf7, a4v::bcast<0>(vf8)); a4::q128 mask = a4v::set_epi32(0, -1, -1, -1); vf7 = a4v::blendv_ps(vf7, res, a4v::castsi128_ps(mask)); }
    { a4::vec4 res = a4v::subK<0, 1>(vf4, vf7); a4::q128 mask = a4v::set_epi32(0, -1, -1, -1); vf4 = a4v::blend(vf4, res, a4v::castsi128_ps(mask)); }
    { a4::vec4 res = a4v::mulK<1, 1>(vf4, vf4); a4::q128 mask = a4v::set_epi32(0, -1, -1, -1); vf4 = a4v::blend(vf4, res, a4v::castsi128_ps(mask)); }
    { a4::vec4 res = a4v::addK<1, 1>(vf4, a4v::bcast<1>(vf4)); a4::q128 mask = a4v::set_epi32(0, 0, 0, -1); vf4 = a4v::blendv_ps(vf4, res, a4v::castsi128_ps(mask)); }
    { a4::vec4 res = a4v::addK<1, 1>(vf4, a4v::bcast<2>(vf4)); a4::q128 mask = a4v::set_epi32(0, 0, 0, -1); vf4 = a4v::blendv_ps(vf4, res, a4v::castsi128_ps(mask)); }
    { a4::vec4 res = a4v::subK<0, 1>(vf5, vf7); a4::q128 mask = a4v::set_epi32(0, -1, -1, -1); vf5 = a4v::blend(vf5, res, a4v::castsi128_ps(mask)); }
    { a4::vec4 res = a4v::mulK<1, 1>(vf5, vf5); a4::q128 mask = a4v::set_epi32(0, -1, -1, -1); vf5 = a4v::blend(vf5, res, a4v::castsi128_ps(mask)); }
    { a4::vec4 res = a4v::addK<1, 1>(vf5, a4v::bcast<1>(vf5)); a4::q128 mask = a4v::set_epi32(0, 0, 0, -1); vf5 = a4v::blendv_ps(vf5, res, a4v::castsi128_ps(mask)); }
    { a4::vec4 res = a4v::addK<1, 1>(vf5, a4v::bcast<2>(vf5)); a4::q128 mask = a4v::set_epi32(0, 0, 0, -1); vf5 = a4v::blendv_ps(vf5, res, a4v::castsi128_ps(mask)); }
    { a4::vec4 res = a4v::max_ps(vf4, a4v::bcast<0>(vf5)); a4::q128 mask = a4v::set_epi32(0, 0, 0, -1); vf8 = a4v::blendv_ps(vf8, res, a4v::castsi128_ps(mask)); }
    { a4::vec4 res = a4v::subK<0, 1>(vf6, vf7); a4::q128 mask = a4v::set_epi32(0, -1, -1, -1); vf6 = a4v::blend(vf6, res, a4v::castsi128_ps(mask)); }
    { a4::vec4 res = a4v::mulK<1, 1>(vf6, vf6); a4::q128 mask = a4v::set_epi32(0, -1, -1, -1); vf6 = a4v::blend(vf6, res, a4v::castsi128_ps(mask)); }
    { a4::vec4 res = a4v::addK<1, 1>(vf6, a4v::bcast<1>(vf6)); a4::q128 mask = a4v::set_epi32(0, 0, 0, -1); vf6 = a4v::blendv_ps(vf6, res, a4v::castsi128_ps(mask)); }
    { a4::vec4 res = a4v::addK<1, 1>(vf6, a4v::bcast<2>(vf6)); a4::q128 mask = a4v::set_epi32(0, 0, 0, -1); vf6 = a4v::blendv_ps(vf6, res, a4v::castsi128_ps(mask)); }
    { a4::vec4 res = a4v::max_ps(vf8, a4v::bcast<0>(vf6)); a4::q128 mask = a4v::set_epi32(0, 0, 0, -1); vf8 = a4v::blendv_ps(vf8, res, a4v::castsi128_ps(mask)); }
    { a4::vec4 res = a4v::mulK<1, 0>(vf0, a4v::bcast<0>(vf8)); a4::q128 mask = a4v::set_epi32(-1, 0, 0, 0); vf7 = a4v::blendv_ps(vf7, res, a4v::castsi128_ps(mask)); }
    A4_W128(ADD32((uint32_t)s0, 0), a4v::castps_si128(vf7));
    (a0 = (uint64_t)((uint64_t)s2 + (uint64_t)0ULL));
    { (a1 = (uint64_t)((uint64_t)s1 + (uint64_t)0ULL)); A3_CHARGE(); uint64_t a7c[6] = {0u, 0u, 0u, 0u, 0u, 0u}; v0 = EntryModifiedManager_x8(ram, rt, a0, (uint32_t)a1, a7c); a0 = a7c[0]; a2 = a7c[1]; a3 = a7c[2]; t0 = a7c[3]; t1 = a7c[4]; t2 = a7c[5]; a3xw8 = true; }
L_1119544:;
    do { const a4::q128 _v = (A4_R128(ADD32((uint32_t)s0, 0))); h2 = a4::hi64(_v); std::memcpy(&v0, &_v, 8); } while (0);
    A4_W128(ADD32((uint32_t)s3, 0), a4::mk128(h2, v0));
L_111954c:;
    { (sp = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)sp, 64)))); if (xo) { if (a3xw0) xo[0] = a1_bits(f0); if (a3xw1) xo[1] = a1_bits(f2); xo[2] = a0; xo[3] = a1; xo[4] = a2; xo[5] = a3; xo[6] = t0; xo[7] = t1; if (a3xw8) xo[8] = t2; if (a3xw9) xo[9] = t3; } io_vf4 = vf4; io_vf5 = vf5; io_vf6 = vf6; io_vf7 = vf7; io_vf8 = vf8; io_vf10 = vf10; io_vf14 = vf14; io_vf15 = vf15; io_vf16 = vf16; io_vf17 = vf17; io_vf29 = vf29; io_vf30 = vf30; io_vf31 = vf31; io_vacc = vacc; io_h2 = h2; io_h3 = h3; io_h11 = h11; return v0; }
    __builtin_unreachable();
}

// GetCollisionEdgeHangQuaternion (0x1119c08): typed by tools/typed_lift2.py (round A6; guest frame calls; VU0 in vf0 vf7 vf10 vf29 vf31 vacc vstat h2 h3 h9 h11 h29, out vf5 vf6 vf7 vf8 vf10 vf14 vf15 vf16 vf17 vf29 vf30 vf31 vacc vq vstat h2 h5 h9 h11, relocated calls 1)
inline a1::RetVF GetCollisionEdgeHangQuaternion(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint64_t a0, uint64_t a1, uint64_t a2, uint32_t sp, float f12, float f13, float f14, float f15, float f16, float f17, float f18, float f19, float f20, const a4::vec4 &io_vf0, a4::vec4 &io_vf5, a4::vec4 &io_vf6, a4::vec4 &io_vf7, a4::vec4 &io_vf8, a4::vec4 &io_vf10, a4::vec4 &io_vf14, a4::vec4 &io_vf15, a4::vec4 &io_vf16, a4::vec4 &io_vf17, a4::vec4 &io_vf29, a4::vec4 &io_vf30, a4::vec4 &io_vf31, a4::vec4 &io_vacc, float &io_vq, uint16_t &io_vstat, uint64_t &io_h2, uint64_t io_h3, uint64_t &io_h5, uint64_t &io_h9, uint64_t &io_h11, uint64_t io_h29)
{
    float f0 = 0.0f;
    uint32_t at = 0;
    uint64_t v0 = 0;
    uint32_t v1 = 0;
    uint64_t a3 = 0;
    uint64_t t0 = 0;
    uint64_t t1 = 0;
    uint32_t t2 = 0;
    uint32_t t3 = 0;
    uint32_t t4 = 0;
    uint32_t t5 = 0;
    uint64_t s0 = 0;
    uint64_t s1 = 0;
    uint64_t s2 = 0;
    uint64_t s3 = 0;
    uint32_t s4 = 0;
    uint64_t s5 = 0;
    uint64_t s6 = 0;
    uint64_t s7 = 0;
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
    a4::vec4 vf29 = io_vf29;
    a4::vec4 vf30 = io_vf30;
    a4::vec4 vf31 = io_vf31;
    a4::vec4 vacc = io_vacc;
    float vq = io_vq;
    uint16_t vstat = io_vstat;
    uint64_t h2 = io_h2;
    uint64_t h3 = io_h3;
    uint64_t h5 = io_h5;
    uint64_t h9 = io_h9;
    uint64_t h11 = io_h11;
    uint64_t h29 = io_h29;
    sp = sp - 0x90u;
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)255 << 16))));
    (v0 = (uint64_t)(RC_U64X(v0) | (uint64_t)(uint16_t)65535));
    (s2 = (uint64_t)((uint64_t)a1 + (uint64_t)0ULL));
    (s3 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(sp, 48))));
    (s5 = (uint64_t)((uint64_t)a2 + (uint64_t)0ULL));
    (s6 = (uint64_t)((uint64_t)a1_sx64(sp) + (uint64_t)0ULL));
    (s7 = (uint64_t)((uint64_t)a0 + (uint64_t)0ULL));
    (s1 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)s2, 4))));
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)s2, 0))));
    (s0 = (uint64_t)((int64_t)(int32_t)((int32_t)SRL32((uint32_t)s1, 24))));
    (s1 = (uint64_t)(RC_U64X(s1) & RC_U64X(v0)));
    (s0 = (uint64_t)(RC_U64X(s0) & (uint64_t)(uint16_t)127));
    { (s4 = (uint32_t)((int32_t)((int32_t)A1_R32B((uint32_t)s2, 8)))); A3_CHARGE(); v0 = GetLayoutClass(ram, rt, a0, f12, f13, f14, f15, f16, f17, f18, f19, h2); }
    (t5 = (uint32_t)((int32_t)((int32_t)SLL32((uint32_t)s1, 2))));
    (t1 = (uint64_t)((uint64_t)v0 + (uint64_t)0ULL));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)SLL32((uint32_t)s0, 3))));
    (v1 = (uint32_t)((int32_t)((int32_t)A1_R32B((uint32_t)s2, 4))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v0, (uint32_t)s0))));
    (t2 = (uint32_t)((int32_t)((int32_t)ADD32((uint32_t)t1, 340))));
    (t3 = (uint32_t)((int32_t)((int32_t)SLL32((uint32_t)v0, 4))));
    (a0 = (uint64_t)((uint64_t)s3 + (uint64_t)0ULL));
    (a1 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL));
    (a2 = (uint64_t)((uint64_t)t1 + (uint64_t)0ULL));
    (a3 = (uint64_t)((uint64_t)s0 + (uint64_t)0ULL));
    { const bool c = (int32_t)v1 >= 0; (t0 = (uint64_t)((uint64_t)s1 + (uint64_t)0ULL)); if (c) goto L_1119cb0; }
    { A3_CHARGE(); v0 = manager_sub_01114C70(ram, rt, a0, a1, a2, a3, t0, sp, f12, f13, f14, f15, f16, f17, f18, f19, f20, vf8, vf10, vf14, vf15, vf16, vf17, vf29, vf30, vf31, vacc, h5, h9, h29); }
    v1 = s4 << 2;
    goto L_1119cf4;
L_1119cb0:;
    (a3 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)t1, 388))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B(t2, 32))));
    (v1 = (uint32_t)((int32_t)((int32_t)A1_R32B((uint32_t)a3, 32))));
    (a2 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v0, 40))));
    (a3 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)v0, 36))));
    (t1 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B(v1, 8))));
    t4 = A1_R32B(v1, 24);
    (t3 = (uint32_t)((int32_t)((int32_t)ADD32(t3, (uint32_t)t1))));
    (t0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B(t2, 8))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B(t3, 84))));
    t2 = A1_R32B(v1, 20);
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(t5, (uint32_t)v0))));
    (t3 = (uint32_t)((int32_t)((int32_t)A1_R32B((uint32_t)v0, 0))));
    t3 = t3 << 6;
    { t3 = t4 + t3; A3_CHARGE(); v0 = manager_sub_01116108(ram, rt, a0, a1, a2, a3, t0, t1, a1_sx64(t2), a1_sx64(t3), a1_sx64(sp), f12, f13, f14, f15, f16, f17, f18, f19, vf8, vf10, vf14, vf15, vf16, vf17, vf29, vf30, vf31, vacc, h11); }
    v1 = s4 << 2;
L_1119cf4:;
    (a2 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(sp, 16))));
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)s3, v1))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)281 << 16))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v0, v1))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)v0, 21656))));
    (a1 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)a0, 0))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)SLL32((uint32_t)v0, 2))));
    A1_W32B((uint32_t)s6, 0, (uint32_t)a1);
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)s3, (uint32_t)v0))));
    (v1 = (uint32_t)((int32_t)((int32_t)A1_R32B((uint32_t)v0, 0))));
    A1_W32B((uint32_t)s6, 4, v1);
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B(sp, 4))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B(sp, 0))));
    vf5 = a4v::castsi128_ps(A4_R128(ADD32((uint32_t)a0, 0)));
    vf6 = a4v::castsi128_ps(A4_R128(ADD32((uint32_t)v0, 0)));
    { a4::vec4 res = a4v::sub(vf5, vf6); a4::q128 mask = a4v::set_epi32(0, -1, -1, -1); vf5 = a4v::blend(vf5, res, a4v::castsi128_ps(mask)); }
    A4_W128(ADD32((uint32_t)a2, 0), a4v::castps_si128(vf5));
    { const uint32_t _dynInsn = A1_R32(0x1119D38u); const int32_t _dynSimm = (int16_t)(_dynInsn & 0xFFFFu); const uint32_t _dynImm = _dynInsn & 0xFFFFu; (void)_dynSimm; (void)_dynImm; (v1 = (uint32_t)((int32_t)((int32_t)((uint32_t)_dynImm << 16)))); }
    A1_W32B(sp, 20, 0u);
    { const uint32_t _dynInsn = A1_R32(0x1119D40u); const int32_t _dynSimm = (int16_t)(_dynInsn & 0xFFFFu); const uint32_t _dynImm = _dynInsn & 0xFFFFu; (void)_dynSimm; (void)_dynImm; (v1 = (uint32_t)((int32_t)((int32_t)ADD32(v1, _dynSimm)))); }
    (a1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(sp, 32))));
    vf5 = a4v::castsi128_ps(A4_R128(ADD32((uint32_t)a2, 0)));
    vf6 = a4v::castsi128_ps(A4_R128(ADD32(v1, 0)));
    { a4::vec4 fs_yzx = a4v::shuffle_ps<0xC9>(vf5, vf5); a4::vec4 ft_zxy = a4v::shuffle_ps<0xD2>(vf6, vf6); a4::vec4 res = a4v::mul(fs_yzx, ft_zxy); vacc = a4v::dest<1, 1, 1, 0>(vacc, res); }
    { a4::vec4 fs_yzx = a4v::shuffle_ps<0xC9>(vf6, vf6); a4::vec4 ft_zxy = a4v::shuffle_ps<0xD2>(vf5, vf5); a4::vec4 mul_res = a4v::mul(fs_yzx, ft_zxy); a4::vec4 res = a4v::subK<1, 1>(vacc, mul_res); a4::q128 mask = a4v::set_epi32(0, -1, -1, -1); vf7 = a4v::blendv_ps(vf7, res, a4v::castsi128_ps(mask)); }
    { a4::vec4 res = a4v::sub(vf7, vf7); a4::q128 mask = a4v::set_epi32(-1, 0, 0, 0); vf7 = a4v::blend(vf7, res, a4v::castsi128_ps(mask)); }
    A4_W128(ADD32((uint32_t)a1, 0), a4v::castps_si128(vf7));
    (at = (uint32_t)((int32_t)((int32_t)((uint32_t)49024 << 16))));
    { uint32_t bits = a7_opq(at); std::memcpy(&f0, &bits, sizeof(bits)); }
    if ((int64_t)s5 == 0) goto L_1119d84;
    { uint32_t bits; std::memcpy(&bits, &f0, sizeof(bits)); (v0 = (uint64_t)((int64_t)(int32_t)(bits))); }
    vf5 = a4v::castsi128_ps(A4_R128(ADD32((uint32_t)a1, 0)));
    vf6 = a4v::castsi128_ps(a4::mk128(h2, v0));
    { a4::vec4 res = a4v::mul(vf5, a4v::bcast<0>(vf6)); a4::q128 mask = a4v::set_epi32(0, -1, -1, -1); vf5 = a4v::blendv_ps(vf5, res, a4v::castsi128_ps(mask)); }
    A4_W128(ADD32((uint32_t)a1, 0), a4v::castps_si128(vf5));
L_1119d84:;
    vf5 = a4v::castsi128_ps(A4_R128(ADD32((uint32_t)a1, 0)));
    { a4::vec4 res = a4v::mul(vf5, vf5); a4::q128 mask = a4v::set_epi32(0, -1, -1, -1); vf7 = a4v::blend(vf7, res, a4v::castsi128_ps(mask)); }
    { a4::vec4 res = a4v::mulK<1, 1>(vf0, a4v::bcast<0>(vf7)); vacc = a4v::dest<0, 0, 0, 1>(vacc, res); }
    { a4::vec4 mul_res = a4v::mulK<1, 1>(vf0, a4v::bcast<1>(vf7)); a4::vec4 res = a4v::addK<1, 1>(vacc, mul_res); vacc = a4v::dest<0, 0, 0, 1>(vacc, res); }
    { a4::vec4 mul_res = a4v::mulK<1, 1>(vf0, a4v::bcast<2>(vf7)); a4::vec4 res = a4v::addK<1, 1>(vacc, mul_res); a4::q128 mask = a4v::set_epi32(-1, 0, 0, 0); vf7 = a4v::blendv_ps(vf7, res, a4v::castsi128_ps(mask)); }
    { float fs = a4v::cvtss_f32(a4v::shuffle_ps<0x03>(vf0, vf0)); float ft = a4v::cvtss_f32(a4v::shuffle_ps<0x03>(vf7, vf7)); vq = a4v::rsqrt(vstat, fs, ft); }
    { a4::vec4 res = a4v::mul(vf5, a4v::set1_ps(vq)); a4::q128 mask = a4v::set_epi32(0, -1, -1, -1); vf5 = a4v::blendv_ps(vf5, res, a4v::castsi128_ps(mask)); }
    A4_W128(ADD32((uint32_t)a1, 0), a4v::castps_si128(vf5));
    { (a0 = (uint64_t)((uint64_t)s7 + (uint64_t)0ULL)); A3_CHARGE(); { const a1::RetVF a2_r = GetYRotQuaternionFromDir(ram, rt, (uint32_t)a0, (uint32_t)a1, vf5, vf6, h3); v0 = a2_r.v0; f0 = a2_r.f0; } }
    { sp = sp + 0x90u; io_vf5 = vf5; io_vf6 = vf6; io_vf7 = vf7; io_vf8 = vf8; io_vf10 = vf10; io_vf14 = vf14; io_vf15 = vf15; io_vf16 = vf16; io_vf17 = vf17; io_vf29 = vf29; io_vf30 = vf30; io_vf31 = vf31; io_vacc = vacc; io_vq = vq; io_vstat = vstat; io_h2 = h2; io_h5 = h5; io_h9 = h9; io_h11 = h11; return a1::RetVF{(uint64_t)v0, f0}; }
    __builtin_unreachable();
}

// GetCollisionEdgePointerGlobalVerticalPlane (0x1119de0): typed by tools/typed_lift2.py (round A6; guest frame calls, extra outputs xo[0]=f20 xo[1]=a1 xo[2]=a2 xo[3]=a3 xo[4]=t0 xo[5]=t1 xo[6]=t2 xo[7]=t3; VU0 in vf0 vf6 vf7 vf10 vf18 vf29 vf31 vacc vstat h9 h11 h29, out vf4 vf5 vf6 vf7 vf8 vf10 vf14 vf15 vf16 vf17 vf18 vf29 vf30 vf31 vacc vq vstat h2 h5 h9 h11, relocated calls 1)
inline a1::RetVF GetCollisionEdgePointerGlobalVerticalPlane(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint64_t a0, uint64_t a1, uint64_t a2, uint64_t a3, uint64_t sp, float f12, float f13, float f14, float f15, float f16, float f17, float f18, float f19, float f20, const a4::vec4 &io_vf0, a4::vec4 &io_vf4, a4::vec4 &io_vf5, a4::vec4 &io_vf6, a4::vec4 &io_vf7, a4::vec4 &io_vf8, a4::vec4 &io_vf10, a4::vec4 &io_vf14, a4::vec4 &io_vf15, a4::vec4 &io_vf16, a4::vec4 &io_vf17, a4::vec4 &io_vf18, a4::vec4 &io_vf29, a4::vec4 &io_vf30, a4::vec4 &io_vf31, a4::vec4 &io_vacc, float &io_vq, uint16_t &io_vstat, uint64_t &io_h2, uint64_t &io_h5, uint64_t &io_h9, uint64_t &io_h11, uint64_t io_h29, uint64_t *xo = nullptr)
{
    float f0 = 0.0f;
    float f1 = 0.0f;
    float f2 = 0.0f;
    float f3 = 0.0f;
    uint32_t at = 0;
    uint64_t v0 = 0;
    uint32_t v1 = 0;
    uint64_t t0 = 0;
    uint64_t t1 = 0;
    uint64_t t2 = 0;
    uint64_t t3 = 0;
    uint32_t t4 = 0;
    uint32_t t5 = 0;
    uint64_t s0 = 0;
    uint64_t s1 = 0;
    uint64_t s2 = 0;
    uint64_t s3 = 0;
    uint64_t s4 = 0;
    uint64_t s5 = 0;
    uint64_t s6 = 0;
    uint64_t s7 = 0;
    uint32_t fcr31 = 0;
    a4::vec4 vf0 = io_vf0;
    a4::vec4 vf4 = io_vf4;
    a4::vec4 vf5 = io_vf5;
    a4::vec4 vf6 = io_vf6;
    a4::vec4 vf7 = io_vf7;
    a4::vec4 vf8 = io_vf8;
    a4::vec4 vf10 = io_vf10;
    a4::vec4 vf14 = io_vf14;
    a4::vec4 vf15 = io_vf15;
    a4::vec4 vf16 = io_vf16;
    a4::vec4 vf17 = io_vf17;
    a4::vec4 vf18 = io_vf18;
    a4::vec4 vf29 = io_vf29;
    a4::vec4 vf30 = io_vf30;
    a4::vec4 vf31 = io_vf31;
    a4::vec4 vacc = io_vacc;
    float vq = io_vq;
    uint16_t vstat = io_vstat;
    uint64_t h2 = io_h2;
    uint64_t h5 = io_h5;
    uint64_t h9 = io_h9;
    uint64_t h11 = io_h11;
    uint64_t h29 = io_h29;
    bool a3xw0 = false;
    (sp = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)sp, 4294967088))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)255 << 16))));
    (v0 = (uint64_t)(RC_U64X(v0) | (uint64_t)(uint16_t)65535));
    (s2 = (uint64_t)((uint64_t)a2 + (uint64_t)0ULL));
    (s3 = (uint64_t)((uint64_t)a3 + (uint64_t)0ULL));
    (s5 = (uint64_t)((uint64_t)a0 + (uint64_t)0ULL));
    (s6 = (uint64_t)((uint64_t)a1 + (uint64_t)0ULL));
    (a3 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)s3, 4))));
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)s3, 0))));
    (v1 = (uint32_t)((int32_t)((int32_t)SRL32((uint32_t)a3, 24))));
    (s1 = (uint64_t)(RC_U64X(a3) & RC_U64X(v0)));
    { (s0 = (uint64_t)(a1_sx64(v1) & (uint64_t)(uint16_t)127)); A3_CHARGE(); v0 = GetLayoutClass(ram, rt, a0, f12, f13, f14, f15, f16, f17, f18, f19, h2); }
    (a2 = (uint64_t)((uint64_t)v0 + (uint64_t)0ULL));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)s3, 4))));
    { const bool c = (int32_t)v0 >= 0; (a3 = (uint64_t)((uint64_t)s0 + (uint64_t)0ULL)); if (c) goto L_1119e60; }
    (t0 = (uint64_t)((uint64_t)s1 + (uint64_t)0ULL));
    (a0 = (uint64_t)((uint64_t)sp + (uint64_t)0ULL));
    { (a1 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL)); A3_CHARGE(); uint64_t a7c[9] = {0u, 0u, 0u, 0u, 0u, 0u, 0u, 0u, 0u}; v0 = manager_sub_01114C70(ram, rt, a0, a1, a2, a3, t0, (uint32_t)sp, f12, f13, f14, f15, f16, f17, f18, f19, f20, vf8, vf10, vf14, vf15, vf16, vf17, vf29, vf30, vf31, vacc, h5, h9, h29, a7c); f20 = a1_flt((uint32_t)a7c[1]); t1 = a7c[6]; t2 = a7c[7]; t3 = a7c[8]; a3xw0 = true; }
    (v1 = (uint32_t)((int32_t)((int32_t)A1_R32B((uint32_t)s3, 8))));
    goto L_1119ec0;
L_1119e60:;
    (v1 = (uint32_t)((int32_t)((int32_t)A1_R32B((uint32_t)a2, 388))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)SLL32((uint32_t)s0, 3))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v0, (uint32_t)s0))));
    (t2 = (uint64_t)((int64_t)(int32_t)((int32_t)SLL32((uint32_t)s1, 2))));
    t4 = A1_R32B(v1, 32);
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)SLL32((uint32_t)v0, 4))));
    (t3 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)a2, 340))));
    (a0 = (uint64_t)((uint64_t)sp + (uint64_t)0ULL));
    (t1 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B(t4, 8))));
    (a1 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL));
    (a2 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)t3, 32))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v0, (uint32_t)t1))));
    (t0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)t3, 8))));
    (v1 = (uint32_t)((int32_t)((int32_t)A1_R32B((uint32_t)v0, 84))));
    t5 = A1_R32B(t4, 24);
    (t2 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)t2, v1))));
    (a3 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)a2, 36))));
    (t3 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)t2, 0))));
    (a2 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)a2, 40))));
    (t2 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B(t4, 20))));
    (t3 = (uint64_t)((int64_t)(int32_t)((int32_t)SLL32((uint32_t)t3, 6))));
    { (t3 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(t5, (uint32_t)t3)))); A3_CHARGE(); uint64_t a7c[7] = {0u, 0u, 0u, 0u, t1, t2, t3}; v0 = manager_sub_01116108(ram, rt, a0, a1, a2, a3, t0, t1, t2, t3, sp, f12, f13, f14, f15, f16, f17, f18, f19, vf8, vf10, vf14, vf15, vf16, vf17, vf29, vf30, vf31, vacc, h11, a7c); t1 = a7c[4]; t2 = a7c[5]; t3 = a7c[6]; }
    (v1 = (uint32_t)((int32_t)((int32_t)A1_R32B((uint32_t)s3, 8))));
L_1119ec0:;
    (a2 = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)281 << 16))));
    (a2 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)a2, 21656))));
    (a3 = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)255 << 16))));
    v1 = v1 << 2;
    (a3 = (uint64_t)(RC_U64X(a3) | (uint64_t)(uint16_t)65535));
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(v1, (uint32_t)a2))));
    (a1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)sp, v1))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)a0, 0))));
    (t0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)a1, 0))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)SLL32((uint32_t)v0, 2))));
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)sp, (uint32_t)v0))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v0, (uint32_t)a2))));
    (a1 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)a0, 0))));
    (v1 = (uint32_t)((int32_t)((int32_t)A1_R32B((uint32_t)v0, 0))));
    A1_W32B((uint32_t)s5, 4, (uint32_t)a1);
    A1_W32B((uint32_t)s5, 0, (uint32_t)t0);
    v1 = v1 << 2;
    (a1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)sp, v1))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)s3, 4))));
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)s3, 0))));
    (v1 = (uint32_t)((int32_t)((int32_t)SRL32((uint32_t)v0, 24))));
    (s4 = (uint64_t)(RC_U64X(v0) & RC_U64X(a3)));
    (s7 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)a1, 0))));
    { (s1 = (uint64_t)(a1_sx64(v1) & (uint64_t)(uint16_t)127)); A3_CHARGE(); v0 = GetLayoutClass(ram, rt, a0, f12, f13, f14, f15, f16, f17, f18, f19, h2); }
    (a3 = (uint64_t)((uint64_t)v0 + (uint64_t)0ULL));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)s3, 4))));
    { const bool c = (int32_t)v0 >= 0; (v1 = (uint32_t)((int32_t)((int32_t)SLL32((uint32_t)s1, 1)))); if (c) goto L_111a018; }
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)a3, 372))));
    (v1 = (uint32_t)((int32_t)((int32_t)ADD32(v1, (uint32_t)s1))));
    (a1 = (uint64_t)((int64_t)(int32_t)((int32_t)SLL32((uint32_t)s4, 4))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)a0, 36))));
    v1 = v1 << 5;
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v0, v1))));
    (s3 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v0, 48))));
    (v1 = (uint32_t)((int32_t)((int32_t)A1_R32B((uint32_t)s3, 8))));
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)s3, 20))));
    (v1 = (uint32_t)((int32_t)((int32_t)ADD32(v1, (uint32_t)s4))));
    (v0 = (uint64_t)(uint32_t)((uint8_t)A1_R8B(v1, 0)));
    { const bool c = (int64_t)v0 != 0; (s0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)a0, (uint32_t)a1)))); if (c) goto L_111a130; }
    (a2 = (uint64_t)((uint64_t)a3 + (uint64_t)0ULL));
    (a3 = (uint64_t)((uint64_t)s1 + (uint64_t)0ULL));
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)sp, 48))));
    (a1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)sp, 64))));
    { (t0 = (uint64_t)((uint64_t)s4 + (uint64_t)0ULL)); A3_CHARGE(); uint64_t a7c[9] = {0u, 0u, 0u, 0u, 0u, 0u, 0u, 0u, 0u}; v0 = manager_sub_01114C70(ram, rt, a0, a1, a2, a3, t0, (uint32_t)sp, f12, f13, f14, f15, f16, f17, f18, f19, f20, vf8, vf10, vf14, vf15, vf16, vf17, vf29, vf30, vf31, vacc, h5, h9, h29, a7c); f20 = a1_flt((uint32_t)a7c[1]); t3 = a7c[8]; a3xw0 = true; }
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)sp, 48))));
    vf29 = a4v::castsi128_ps(A4_R128(ADD32((uint32_t)v0, 0)));
    (v1 = (uint32_t)((int32_t)((int32_t)A1_R32B((uint32_t)sp, 52))));
    vf30 = a4v::castsi128_ps(A4_R128(ADD32(v1, 0)));
    { a4::vec4 res = a4v::sub(vf29, vf30); a4::q128 mask = a4v::set_epi32(-1, -1, -1, -1); vf31 = a4v::blend(vf31, res, a4v::castsi128_ps(mask)); }
    { a4::q128 mask = a4v::set_epi32(-1, -1, -1, -1); vf18 = a4v::blendv_ps(vf18, vf31, a4v::castsi128_ps(mask)); }
    { a4::q128 mask = a4v::set_epi32(-1, -1, -1, -1); vf29 = a4v::blendv_ps(vf29, vf30, a4v::castsi128_ps(mask)); }
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)sp, 56))));
    vf30 = a4v::castsi128_ps(A4_R128(ADD32((uint32_t)v0, 0)));
    { a4::vec4 res = a4v::sub(vf29, vf30); a4::q128 mask = a4v::set_epi32(-1, -1, -1, -1); vf31 = a4v::blend(vf31, res, a4v::castsi128_ps(mask)); }
    { a4::q128 mask = a4v::set_epi32(-1, -1, -1, -1); vf29 = a4v::blendv_ps(vf29, vf18, a4v::castsi128_ps(mask)); }
    { a4::q128 mask = a4v::set_epi32(-1, -1, -1, -1); vf30 = a4v::blendv_ps(vf30, vf31, a4v::castsi128_ps(mask)); }
    { a4::vec4 fs_yzx = a4v::shuffle_ps<0xC9>(vf29, vf29); a4::vec4 ft_zxy = a4v::shuffle_ps<0xD2>(vf30, vf30); a4::vec4 res = a4v::mulK<1, 1>(fs_yzx, ft_zxy); vacc = a4v::dest<1, 1, 1, 0>(vacc, res); }
    { a4::vec4 fs_yzx = a4v::shuffle_ps<0xC9>(vf30, vf30); a4::vec4 ft_zxy = a4v::shuffle_ps<0xD2>(vf29, vf29); a4::vec4 mul_res = a4v::mulK<1, 1>(fs_yzx, ft_zxy); a4::vec4 res = a4v::subK<1, 1>(vacc, mul_res); a4::q128 mask = a4v::set_epi32(0, -1, -1, -1); vf31 = a4v::blendv_ps(vf31, res, a4v::castsi128_ps(mask)); }
    { a4::vec4 res = a4v::subK<1, 1>(vf31, vf31); a4::q128 mask = a4v::set_epi32(-1, 0, 0, 0); vf31 = a4v::blend(vf31, res, a4v::castsi128_ps(mask)); }
    { a4::q128 mask = a4v::set_epi32(-1, -1, -1, -1); vf29 = a4v::blendv_ps(vf29, vf31, a4v::castsi128_ps(mask)); }
    { a4::vec4 res = a4v::mulK<1, 1>(vf29, vf29); a4::q128 mask = a4v::set_epi32(0, -1, -1, -1); vf7 = a4v::blend(vf7, res, a4v::castsi128_ps(mask)); }
    { a4::vec4 res = a4v::mulK<1, 1>(vf0, a4v::bcast<0>(vf7)); vacc = a4v::dest<0, 0, 0, 1>(vacc, res); }
    { a4::vec4 mul_res = a4v::mulK<1, 1>(vf0, a4v::bcast<1>(vf7)); a4::vec4 res = a4v::addK<1, 1>(vacc, mul_res); vacc = a4v::dest<0, 0, 0, 1>(vacc, res); }
    { a4::vec4 mul_res = a4v::mulK<1, 1>(vf0, a4v::bcast<2>(vf7)); a4::vec4 res = a4v::addK<1, 1>(vacc, mul_res); a4::q128 mask = a4v::set_epi32(-1, 0, 0, 0); vf7 = a4v::blendv_ps(vf7, res, a4v::castsi128_ps(mask)); }
    { float fs = a4v::cvtss_f32(a4v::shuffle_ps<0x03>(vf0, vf0)); float ft = a4v::cvtss_f32(a4v::shuffle_ps<0x03>(vf7, vf7)); vq = a4v::rsqrt(vstat, fs, ft); }
    { a4::vec4 res = a4v::mul(vf29, a4v::set1_ps(vq)); a4::q128 mask = a4v::set_epi32(0, -1, -1, -1); vf29 = a4v::blendv_ps(vf29, res, a4v::castsi128_ps(mask)); }
    { a4::q128 mask = a4v::set_epi32(-1, -1, -1, -1); vf31 = a4v::blendv_ps(vf31, vf29, a4v::castsi128_ps(mask)); }
    A4_W128(ADD32((uint32_t)s0, 0), a4v::castps_si128(vf31));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)sp, 48))));
    vf4 = a4v::castsi128_ps(A4_R128(ADD32((uint32_t)s0, 0)));
    vf5 = a4v::castsi128_ps(A4_R128(ADD32((uint32_t)v0, 0)));
    { a4::vec4 res = a4v::mul(vf4, vf5); a4::q128 mask = a4v::set_epi32(0, -1, -1, -1); vf5 = a4v::blend(vf5, res, a4v::castsi128_ps(mask)); }
    { a4::vec4 res = a4v::addK<1, 1>(vf5, a4v::bcast<1>(vf5)); a4::q128 mask = a4v::set_epi32(0, 0, 0, -1); vf5 = a4v::blendv_ps(vf5, res, a4v::castsi128_ps(mask)); }
    { a4::vec4 res = a4v::addK<1, 1>(vf5, a4v::bcast<2>(vf5)); a4::q128 mask = a4v::set_epi32(0, 0, 0, -1); vf5 = a4v::blendv_ps(vf5, res, a4v::castsi128_ps(mask)); }
    { a4::vec4 res = a4v::subK<1, 1>(vf0, a4v::bcast<3>(vf0)); a4::q128 mask = a4v::set_epi32(-1, -1, -1, -1); vf6 = a4v::blendv_ps(vf6, res, a4v::castsi128_ps(mask)); }
    { a4::vec4 res = a4v::subK<1, 1>(vf6, a4v::bcast<0>(vf5)); a4::q128 mask = a4v::set_epi32(-1, 0, 0, 0); vf4 = a4v::blendv_ps(vf4, res, a4v::castsi128_ps(mask)); }
    A4_W128(ADD32((uint32_t)s0, 0), a4v::castps_si128(vf4));
    (a0 = (uint64_t)((uint64_t)s3 + (uint64_t)0ULL));
    (a1 = (uint64_t)((uint64_t)s4 + (uint64_t)0ULL));
    goto L_111a128;
L_111a018:;
    (v1 = (uint32_t)((int32_t)((int32_t)A1_R32B((uint32_t)a3, 388))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)SLL32((uint32_t)s1, 3))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v0, (uint32_t)s1))));
    (a1 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)a3, 372))));
    (t2 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B(v1, 32))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)SLL32((uint32_t)v0, 4))));
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)SLL32((uint32_t)s4, 2))));
    (s3 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)a1, 88))));
    (t1 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)t2, 8))));
    (a1 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)s3, 8))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v0, (uint32_t)t1))));
    (a2 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)s3, 20))));
    (v1 = (uint32_t)((int32_t)((int32_t)A1_R32B((uint32_t)v0, 84))));
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)a0, v1))));
    (s1 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)a0, 0))));
    (a1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)a1, (uint32_t)s1))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)SLL32((uint32_t)s1, 4))));
    (v1 = (uint32_t)((uint8_t)A1_R8B((uint32_t)a1, 0)));
    { const bool c = v1 != 0; (s0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)a2, (uint32_t)v0)))); if (c) goto L_111a130; }
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)a3, 340))));
    (v1 = (uint32_t)((int32_t)((int32_t)A1_R32B((uint32_t)t2, 24))));
    (a2 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)v0, 32))));
    (t3 = (uint64_t)((int64_t)(int32_t)((int32_t)SLL32((uint32_t)s1, 6))));
    (t0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)v0, 8))));
    (t3 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(v1, (uint32_t)t3))));
    (a3 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)a2, 36))));
    (a2 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)a2, 40))));
    (t2 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)t2, 20))));
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)sp, 80))));
    { (a1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)sp, 96)))); A3_CHARGE(); uint64_t a7c[7] = {0u, 0u, 0u, 0u, 0u, 0u, t3}; v0 = manager_sub_01116108(ram, rt, a0, a1, a2, a3, t0, t1, t2, t3, sp, f12, f13, f14, f15, f16, f17, f18, f19, vf8, vf10, vf14, vf15, vf16, vf17, vf29, vf30, vf31, vacc, h11, a7c); t3 = a7c[6]; }
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)sp, 80))));
    vf29 = a4v::castsi128_ps(A4_R128(ADD32((uint32_t)v0, 0)));
    (v1 = (uint32_t)((int32_t)((int32_t)A1_R32B((uint32_t)sp, 84))));
    vf30 = a4v::castsi128_ps(A4_R128(ADD32(v1, 0)));
    { a4::vec4 res = a4v::sub(vf29, vf30); a4::q128 mask = a4v::set_epi32(-1, -1, -1, -1); vf31 = a4v::blend(vf31, res, a4v::castsi128_ps(mask)); }
    { a4::q128 mask = a4v::set_epi32(-1, -1, -1, -1); vf18 = a4v::blendv_ps(vf18, vf31, a4v::castsi128_ps(mask)); }
    { a4::q128 mask = a4v::set_epi32(-1, -1, -1, -1); vf29 = a4v::blendv_ps(vf29, vf30, a4v::castsi128_ps(mask)); }
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)sp, 88))));
    vf30 = a4v::castsi128_ps(A4_R128(ADD32((uint32_t)v0, 0)));
    { a4::vec4 res = a4v::sub(vf29, vf30); a4::q128 mask = a4v::set_epi32(-1, -1, -1, -1); vf31 = a4v::blend(vf31, res, a4v::castsi128_ps(mask)); }
    { a4::q128 mask = a4v::set_epi32(-1, -1, -1, -1); vf29 = a4v::blendv_ps(vf29, vf18, a4v::castsi128_ps(mask)); }
    { a4::q128 mask = a4v::set_epi32(-1, -1, -1, -1); vf30 = a4v::blendv_ps(vf30, vf31, a4v::castsi128_ps(mask)); }
    { a4::vec4 fs_yzx = a4v::shuffle_ps<0xC9>(vf29, vf29); a4::vec4 ft_zxy = a4v::shuffle_ps<0xD2>(vf30, vf30); a4::vec4 res = a4v::mulK<1, 1>(fs_yzx, ft_zxy); vacc = a4v::dest<1, 1, 1, 0>(vacc, res); }
    { a4::vec4 fs_yzx = a4v::shuffle_ps<0xC9>(vf30, vf30); a4::vec4 ft_zxy = a4v::shuffle_ps<0xD2>(vf29, vf29); a4::vec4 mul_res = a4v::mulK<1, 1>(fs_yzx, ft_zxy); a4::vec4 res = a4v::subK<1, 1>(vacc, mul_res); a4::q128 mask = a4v::set_epi32(0, -1, -1, -1); vf31 = a4v::blendv_ps(vf31, res, a4v::castsi128_ps(mask)); }
    { a4::vec4 res = a4v::subK<1, 1>(vf31, vf31); a4::q128 mask = a4v::set_epi32(-1, 0, 0, 0); vf31 = a4v::blend(vf31, res, a4v::castsi128_ps(mask)); }
    { a4::q128 mask = a4v::set_epi32(-1, -1, -1, -1); vf29 = a4v::blendv_ps(vf29, vf31, a4v::castsi128_ps(mask)); }
    { a4::vec4 res = a4v::mulK<1, 1>(vf29, vf29); a4::q128 mask = a4v::set_epi32(0, -1, -1, -1); vf7 = a4v::blend(vf7, res, a4v::castsi128_ps(mask)); }
    { a4::vec4 res = a4v::mulK<1, 1>(vf0, a4v::bcast<0>(vf7)); vacc = a4v::dest<0, 0, 0, 1>(vacc, res); }
    { a4::vec4 mul_res = a4v::mulK<1, 1>(vf0, a4v::bcast<1>(vf7)); a4::vec4 res = a4v::addK<1, 1>(vacc, mul_res); vacc = a4v::dest<0, 0, 0, 1>(vacc, res); }
    { a4::vec4 mul_res = a4v::mulK<1, 1>(vf0, a4v::bcast<2>(vf7)); a4::vec4 res = a4v::addK<1, 1>(vacc, mul_res); a4::q128 mask = a4v::set_epi32(-1, 0, 0, 0); vf7 = a4v::blendv_ps(vf7, res, a4v::castsi128_ps(mask)); }
    { float fs = a4v::cvtss_f32(a4v::shuffle_ps<0x03>(vf0, vf0)); float ft = a4v::cvtss_f32(a4v::shuffle_ps<0x03>(vf7, vf7)); vq = a4v::rsqrt(vstat, fs, ft); }
    { a4::vec4 res = a4v::mul(vf29, a4v::set1_ps(vq)); a4::q128 mask = a4v::set_epi32(0, -1, -1, -1); vf29 = a4v::blendv_ps(vf29, res, a4v::castsi128_ps(mask)); }
    { a4::q128 mask = a4v::set_epi32(-1, -1, -1, -1); vf31 = a4v::blendv_ps(vf31, vf29, a4v::castsi128_ps(mask)); }
    A4_W128(ADD32((uint32_t)s0, 0), a4v::castps_si128(vf31));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)sp, 80))));
    vf4 = a4v::castsi128_ps(A4_R128(ADD32((uint32_t)s0, 0)));
    vf5 = a4v::castsi128_ps(A4_R128(ADD32((uint32_t)v0, 0)));
    { a4::vec4 res = a4v::mul(vf4, vf5); a4::q128 mask = a4v::set_epi32(0, -1, -1, -1); vf5 = a4v::blend(vf5, res, a4v::castsi128_ps(mask)); }
    { a4::vec4 res = a4v::addK<1, 1>(vf5, a4v::bcast<1>(vf5)); a4::q128 mask = a4v::set_epi32(0, 0, 0, -1); vf5 = a4v::blendv_ps(vf5, res, a4v::castsi128_ps(mask)); }
    { a4::vec4 res = a4v::addK<1, 1>(vf5, a4v::bcast<2>(vf5)); a4::q128 mask = a4v::set_epi32(0, 0, 0, -1); vf5 = a4v::blendv_ps(vf5, res, a4v::castsi128_ps(mask)); }
    { a4::vec4 res = a4v::subK<1, 1>(vf0, a4v::bcast<3>(vf0)); a4::q128 mask = a4v::set_epi32(-1, -1, -1, -1); vf6 = a4v::blendv_ps(vf6, res, a4v::castsi128_ps(mask)); }
    { a4::vec4 res = a4v::subK<1, 1>(vf6, a4v::bcast<0>(vf5)); a4::q128 mask = a4v::set_epi32(-1, 0, 0, 0); vf4 = a4v::blendv_ps(vf4, res, a4v::castsi128_ps(mask)); }
    A4_W128(ADD32((uint32_t)s0, 0), a4v::castps_si128(vf4));
    (a0 = (uint64_t)((uint64_t)s3 + (uint64_t)0ULL));
    (a1 = (uint64_t)((uint64_t)s1 + (uint64_t)0ULL));
L_111a128:;
    { A3_CHARGE(); uint64_t a7c[5] = {0u, 0u, 0u, 0u, 0u}; v0 = EntryModifiedManager(ram, rt, a0, (uint32_t)a1, a7c); a3 = a7c[1]; t0 = a7c[2]; t1 = a7c[3]; t2 = a7c[4]; }
L_111a130:;
    (a1 = (uint64_t)((uint64_t)s0 + (uint64_t)0ULL));
    (s1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)sp, 32))));
    { (a0 = (uint64_t)((uint64_t)RC_U64X(s1) + (uint64_t)0ULL)); A3_CHARGE(); v0 = iosGetStabXZVector(ram, rt, (uint32_t)a0, (uint32_t)a1, vf0, vf5, vf7, vacc, vq, vstat, h2); }
    (a0 = (uint64_t)((uint64_t)s1 + (uint64_t)0ULL));
    { (a1 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)s5, 0)))); A3_CHARGE(); CompletePlaneD(ram, rt, (uint32_t)a0, (uint32_t)a1, vf0, vf4, vf5, vf6); }
    (a1 = (uint64_t)((uint64_t)s1 + (uint64_t)0ULL));
    (s3 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)sp, 16))));
    (a2 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)s5, 4))));
    { (a0 = (uint64_t)((uint64_t)RC_U64X(s3) + (uint64_t)0ULL)); A3_CHARGE(); { const a1::RetVF a2_r = GetPlaneFittingPoint(ram, rt, (uint32_t)a0, (uint32_t)a1, (uint32_t)a2, (uint32_t)sp, vf0, vf5, vf6, vf7, vacc, h2); v0 = a2_r.v0; f0 = a2_r.f0; } }
    (v1 = (uint32_t)((int32_t)((int32_t)A1_R32B((uint32_t)s5, 0))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)sp, 48))));
    vf5 = a4v::castsi128_ps(A4_R128(ADD32(v1, 0)));
    vf6 = a4v::castsi128_ps(A4_R128(ADD32((uint32_t)s3, 0)));
    { a4::vec4 res = a4v::sub(vf5, vf6); a4::q128 mask = a4v::set_epi32(0, -1, -1, -1); vf5 = a4v::blend(vf5, res, a4v::castsi128_ps(mask)); }
    A4_W128(ADD32((uint32_t)v0, 0), a4v::castps_si128(vf5));
    { uint32_t bits = A1_R32B((uint32_t)sp, 48); float f; std::memcpy(&f, &bits, sizeof(f)); f1 = f; }
    { uint32_t bits = A1_R32B((uint32_t)sp, 56); float f; std::memcpy(&f, &bits, sizeof(f)); f0 = f; }
    f1 = a1f::mul(f1, f1);
    (at = (uint32_t)((int32_t)((int32_t)((uint32_t)15395 << 16))));
    at = at | 0xD708u;
    { uint32_t bits = a7_opq(at); std::memcpy(&f2, &bits, sizeof(bits)); }
    f0 = a1f::mul(f0, f0);
    f3 = a1f::addFinite(f1, f0);
    fcr31 = (a1f::lt(f3, f2)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
    { const bool c = (fcr31 & 0x800000u) != 0u; (v0 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL)); if (c) goto L_111a2ac; }
    { uint32_t bits = A1_R32B((uint32_t)sp, 52); float f; std::memcpy(&f, &bits, sizeof(f)); f0 = f; }
    (at = (uint32_t)((int32_t)((int32_t)((uint32_t)16890 << 16))));
    at = at | 0xE147u;
    { uint32_t bits = a7_opq(at); std::memcpy(&f1, &bits, sizeof(bits)); }
    f0 = a1f::mul(f0, f0);
    f0 = a1f::div(fcr31, f0, f3);
    fcr31 = (a1f::lt(f1, f0)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
    if ((fcr31 & 0x800000u) == 0u) { (v1 = (uint32_t)((int32_t)((int32_t)A1_R32B((uint32_t)s5, 4)))); goto L_111a1d8; }
    goto L_111a2b0;
L_111a1d8:;
    (s0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)s5, 0))));
    vf5 = a4v::castsi128_ps(A4_R128(ADD32(v1, 0)));
    vf6 = a4v::castsi128_ps(A4_R128(ADD32((uint32_t)s0, 0)));
    { a4::vec4 res = a4v::sub(vf5, vf6); a4::q128 mask = a4v::set_epi32(0, -1, -1, -1); vf5 = a4v::blend(vf5, res, a4v::castsi128_ps(mask)); }
    A4_W128(ADD32((uint32_t)s6, 0), a4v::castps_si128(vf5));
    { const uint32_t _dynInsn = A1_R32(0x111A1ECu); const int32_t _dynSimm = (int16_t)(_dynInsn & 0xFFFFu); const uint32_t _dynImm = _dynInsn & 0xFFFFu; (void)_dynSimm; (void)_dynImm; (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)_dynImm << 16)))); }
    { const uint32_t _dynInsn = A1_R32(0x111A1F0u); const int32_t _dynSimm = (int16_t)(_dynInsn & 0xFFFFu); const uint32_t _dynImm = _dynInsn & 0xFFFFu; (void)_dynSimm; (void)_dynImm; (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v0, _dynSimm)))); }
    vf5 = a4v::castsi128_ps(A4_R128(ADD32((uint32_t)s6, 0)));
    vf6 = a4v::castsi128_ps(A4_R128(ADD32((uint32_t)v0, 0)));
    { a4::vec4 fs_yzx = a4v::shuffle_ps<0xC9>(vf5, vf5); a4::vec4 ft_zxy = a4v::shuffle_ps<0xD2>(vf6, vf6); a4::vec4 res = a4v::mul(fs_yzx, ft_zxy); vacc = a4v::dest<1, 1, 1, 0>(vacc, res); }
    { a4::vec4 fs_yzx = a4v::shuffle_ps<0xC9>(vf6, vf6); a4::vec4 ft_zxy = a4v::shuffle_ps<0xD2>(vf5, vf5); a4::vec4 mul_res = a4v::mul(fs_yzx, ft_zxy); a4::vec4 res = a4v::subK<1, 1>(vacc, mul_res); a4::q128 mask = a4v::set_epi32(0, -1, -1, -1); vf7 = a4v::blendv_ps(vf7, res, a4v::castsi128_ps(mask)); }
    { a4::vec4 res = a4v::sub(vf7, vf7); a4::q128 mask = a4v::set_epi32(-1, 0, 0, 0); vf7 = a4v::blend(vf7, res, a4v::castsi128_ps(mask)); }
    A4_W128(ADD32((uint32_t)s2, 0), a4v::castps_si128(vf7));
    vf4 = a4v::castsi128_ps(A4_R128(ADD32((uint32_t)s2, 0)));
    vf5 = a4v::castsi128_ps(A4_R128(ADD32((uint32_t)s0, 0)));
    { a4::vec4 res = a4v::mul(vf4, vf5); a4::q128 mask = a4v::set_epi32(0, -1, -1, -1); vf5 = a4v::blend(vf5, res, a4v::castsi128_ps(mask)); }
    { a4::vec4 res = a4v::addK<1, 1>(vf5, a4v::bcast<1>(vf5)); a4::q128 mask = a4v::set_epi32(0, 0, 0, -1); vf5 = a4v::blendv_ps(vf5, res, a4v::castsi128_ps(mask)); }
    { a4::vec4 res = a4v::addK<1, 1>(vf5, a4v::bcast<2>(vf5)); a4::q128 mask = a4v::set_epi32(0, 0, 0, -1); vf5 = a4v::blendv_ps(vf5, res, a4v::castsi128_ps(mask)); }
    { a4::vec4 res = a4v::subK<1, 1>(vf0, a4v::bcast<3>(vf0)); a4::q128 mask = a4v::set_epi32(-1, -1, -1, -1); vf6 = a4v::blendv_ps(vf6, res, a4v::castsi128_ps(mask)); }
    { a4::vec4 res = a4v::subK<1, 1>(vf6, a4v::bcast<0>(vf5)); a4::q128 mask = a4v::set_epi32(-1, 0, 0, 0); vf4 = a4v::blendv_ps(vf4, res, a4v::castsi128_ps(mask)); }
    A4_W128(ADD32((uint32_t)s2, 0), a4v::castps_si128(vf4));
    (a2 = (uint64_t)((uint64_t)s7 + (uint64_t)0ULL));
    (a0 = (uint64_t)((uint64_t)s3 + (uint64_t)0ULL));
    { (a1 = (uint64_t)((uint64_t)s2 + (uint64_t)0ULL)); A3_CHARGE(); { const a1::RetVF a2_r = GetPlaneFittingPointForUnnormalizedPlane(ram, rt, (uint32_t)a0, (uint32_t)a1, (uint32_t)a2, vf0, vf5, vf6, vf7, vacc, h2); v0 = a2_r.v0; f0 = a2_r.f0; } }
    vf5 = a4v::castsi128_ps(A4_R128(ADD32((uint32_t)s3, 0)));
    vf6 = a4v::castsi128_ps(A4_R128(ADD32((uint32_t)s0, 0)));
    { a4::vec4 res = a4v::sub(vf5, vf6); a4::q128 mask = a4v::set_epi32(0, -1, -1, -1); vf5 = a4v::blend(vf5, res, a4v::castsi128_ps(mask)); }
    A4_W128(ADD32((uint32_t)s1, 0), a4v::castps_si128(vf5));
    vf5 = a4v::castsi128_ps(A4_R128(ADD32((uint32_t)s6, 0)));
    vf6 = a4v::castsi128_ps(A4_R128(ADD32((uint32_t)s1, 0)));
    { a4::vec4 fs_yzx = a4v::shuffle_ps<0xC9>(vf5, vf5); a4::vec4 ft_zxy = a4v::shuffle_ps<0xD2>(vf6, vf6); a4::vec4 res = a4v::mul(fs_yzx, ft_zxy); vacc = a4v::dest<1, 1, 1, 0>(vacc, res); }
    { a4::vec4 fs_yzx = a4v::shuffle_ps<0xC9>(vf6, vf6); a4::vec4 ft_zxy = a4v::shuffle_ps<0xD2>(vf5, vf5); a4::vec4 mul_res = a4v::mul(fs_yzx, ft_zxy); a4::vec4 res = a4v::subK<1, 1>(vacc, mul_res); a4::q128 mask = a4v::set_epi32(0, -1, -1, -1); vf7 = a4v::blendv_ps(vf7, res, a4v::castsi128_ps(mask)); }
    { a4::vec4 res = a4v::sub(vf7, vf7); a4::q128 mask = a4v::set_epi32(-1, 0, 0, 0); vf7 = a4v::blend(vf7, res, a4v::castsi128_ps(mask)); }
    A4_W128(ADD32((uint32_t)s2, 0), a4v::castps_si128(vf7));
    vf5 = a4v::castsi128_ps(A4_R128(ADD32((uint32_t)s2, 0)));
    { a4::vec4 res = a4v::mul(vf5, vf5); a4::q128 mask = a4v::set_epi32(0, -1, -1, -1); vf7 = a4v::blend(vf7, res, a4v::castsi128_ps(mask)); }
    { a4::vec4 res = a4v::mulK<1, 1>(vf0, a4v::bcast<0>(vf7)); vacc = a4v::dest<0, 0, 0, 1>(vacc, res); }
    { a4::vec4 mul_res = a4v::mulK<1, 1>(vf0, a4v::bcast<1>(vf7)); a4::vec4 res = a4v::addK<1, 1>(vacc, mul_res); vacc = a4v::dest<0, 0, 0, 1>(vacc, res); }
    { a4::vec4 mul_res = a4v::mulK<1, 1>(vf0, a4v::bcast<2>(vf7)); a4::vec4 res = a4v::addK<1, 1>(vacc, mul_res); a4::q128 mask = a4v::set_epi32(-1, 0, 0, 0); vf7 = a4v::blendv_ps(vf7, res, a4v::castsi128_ps(mask)); }
    { float fs = a4v::cvtss_f32(a4v::shuffle_ps<0x03>(vf0, vf0)); float ft = a4v::cvtss_f32(a4v::shuffle_ps<0x03>(vf7, vf7)); vq = a4v::rsqrt(vstat, fs, ft); }
    { a4::vec4 res = a4v::mul(vf5, a4v::set1_ps(vq)); a4::q128 mask = a4v::set_epi32(0, -1, -1, -1); vf5 = a4v::blendv_ps(vf5, res, a4v::castsi128_ps(mask)); }
    A4_W128(ADD32((uint32_t)s2, 0), a4v::castps_si128(vf5));
    vf4 = a4v::castsi128_ps(A4_R128(ADD32((uint32_t)s2, 0)));
    vf5 = a4v::castsi128_ps(A4_R128(ADD32((uint32_t)s0, 0)));
    { a4::vec4 res = a4v::mul(vf4, vf5); a4::q128 mask = a4v::set_epi32(0, -1, -1, -1); vf5 = a4v::blend(vf5, res, a4v::castsi128_ps(mask)); }
    { a4::vec4 res = a4v::addK<1, 1>(vf5, a4v::bcast<1>(vf5)); a4::q128 mask = a4v::set_epi32(0, 0, 0, -1); vf5 = a4v::blendv_ps(vf5, res, a4v::castsi128_ps(mask)); }
    { a4::vec4 res = a4v::addK<1, 1>(vf5, a4v::bcast<2>(vf5)); a4::q128 mask = a4v::set_epi32(0, 0, 0, -1); vf5 = a4v::blendv_ps(vf5, res, a4v::castsi128_ps(mask)); }
    { a4::vec4 res = a4v::subK<1, 1>(vf0, a4v::bcast<3>(vf0)); a4::q128 mask = a4v::set_epi32(-1, -1, -1, -1); vf6 = a4v::blendv_ps(vf6, res, a4v::castsi128_ps(mask)); }
    { a4::vec4 res = a4v::subK<1, 1>(vf6, a4v::bcast<0>(vf5)); a4::q128 mask = a4v::set_epi32(-1, 0, 0, 0); vf4 = a4v::blendv_ps(vf4, res, a4v::castsi128_ps(mask)); }
    A4_W128(ADD32((uint32_t)s2, 0), a4v::castps_si128(vf4));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 1))));
L_111a2ac:;
L_111a2b0:;
    { (sp = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)sp, 208)))); if (xo) { if (a3xw0) xo[0] = a1_bits(f20); xo[1] = a1; xo[2] = a2; xo[3] = a3; xo[4] = t0; xo[5] = t1; xo[6] = t2; xo[7] = t3; } io_vf4 = vf4; io_vf5 = vf5; io_vf6 = vf6; io_vf7 = vf7; io_vf8 = vf8; io_vf10 = vf10; io_vf14 = vf14; io_vf15 = vf15; io_vf16 = vf16; io_vf17 = vf17; io_vf18 = vf18; io_vf29 = vf29; io_vf30 = vf30; io_vf31 = vf31; io_vacc = vacc; io_vq = vq; io_vstat = vstat; io_h2 = h2; io_h5 = h5; io_h9 = h9; io_h11 = h11; return a1::RetVF{(uint64_t)v0, f0}; }
    __builtin_unreachable();
}

// GetGeometryPointerWithCollisionDBDataGlobalPoint (0x111b410): typed by tools/typed_lift2.py (round A6; guest frame calls, extra outputs xo[0]=f20 xo[1]=a2 xo[2]=a3 xo[3]=t0 xo[4]=t1 xo[5]=t2 xo[6]=t3; VU0 in vf0 vf7 vf8 vf9 vf10 vf11 vf29 vf31 vacc h9 h11 h29, out vf5 vf6 vf7 vf8 vf9 vf10 vf11 vf14 vf15 vf16 vf17 vf29 vf30 vf31 vacc h2 h5 h9 h11)
inline a1::RetVF GetGeometryPointerWithCollisionDBDataGlobalPoint(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint64_t a0, uint64_t a1, uint64_t a2, uint32_t sp, float f12, float f13, float f14, float f15, float f16, float f17, float f18, float f19, float f20, const a4::vec4 &io_vf0, a4::vec4 &io_vf5, a4::vec4 &io_vf6, a4::vec4 &io_vf7, a4::vec4 &io_vf8, a4::vec4 &io_vf9, a4::vec4 &io_vf10, a4::vec4 &io_vf11, a4::vec4 &io_vf14, a4::vec4 &io_vf15, a4::vec4 &io_vf16, a4::vec4 &io_vf17, a4::vec4 &io_vf29, a4::vec4 &io_vf30, a4::vec4 &io_vf31, a4::vec4 &io_vacc, uint64_t &io_h2, uint64_t &io_h5, uint64_t &io_h9, uint64_t &io_h11, uint64_t io_h29, uint64_t *xo = nullptr)
{
    float f0 = 0.0f;
    float f1 = 0.0f;
    float f2 = 0.0f;
    float f3 = 0.0f;
    uint32_t at = 0;
    uint64_t v0 = 0;
    uint64_t v1 = 0;
    uint64_t a3 = 0;
    uint64_t t0 = 0;
    uint64_t t1 = 0;
    uint64_t t2 = 0;
    uint64_t t3 = 0;
    uint32_t t4 = 0;
    uint32_t t5 = 0;
    uint64_t s0 = 0;
    uint64_t s1 = 0;
    uint64_t s2 = 0;
    uint64_t s3 = 0;
    uint64_t s4 = 0;
    uint32_t s5 = 0;
    uint64_t s6 = 0;
    uint64_t s7 = 0;
    uint32_t fp = 0;
    uint32_t fcr31 = 0;
    a4::vec4 vf0 = io_vf0;
    a4::vec4 vf5 = io_vf5;
    a4::vec4 vf6 = io_vf6;
    a4::vec4 vf7 = io_vf7;
    a4::vec4 vf8 = io_vf8;
    a4::vec4 vf9 = io_vf9;
    a4::vec4 vf10 = io_vf10;
    a4::vec4 vf11 = io_vf11;
    a4::vec4 vf14 = io_vf14;
    a4::vec4 vf15 = io_vf15;
    a4::vec4 vf16 = io_vf16;
    a4::vec4 vf17 = io_vf17;
    a4::vec4 vf29 = io_vf29;
    a4::vec4 vf30 = io_vf30;
    a4::vec4 vf31 = io_vf31;
    a4::vec4 vacc = io_vacc;
    uint64_t h2 = io_h2;
    uint64_t h5 = io_h5;
    uint64_t h9 = io_h9;
    uint64_t h11 = io_h11;
    uint64_t h29 = io_h29;
    bool a3xw0 = false;
    sp = sp - 0xA0u;
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)255 << 16))));
    (v0 = (uint64_t)(RC_U64X(v0) | (uint64_t)(uint16_t)65535));
    (s2 = (uint64_t)((uint64_t)a1 + (uint64_t)0ULL));
    (s3 = (uint64_t)((uint64_t)a0 + (uint64_t)0ULL));
    (s4 = (uint64_t)((uint64_t)a2 + (uint64_t)0ULL));
    s5 = sp + 0x10u;
    (s6 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(sp, 48))));
    (s7 = (uint64_t)((uint64_t)a1_sx64(sp) + (uint64_t)0ULL));
    fp = sp + 0x40u;
    (s1 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)s2, 4))));
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)s2, 0))));
    (s0 = (uint64_t)((int64_t)(int32_t)((int32_t)SRL32((uint32_t)s1, 24))));
    (s1 = (uint64_t)(RC_U64X(s1) & RC_U64X(v0)));
    { (s0 = (uint64_t)(RC_U64X(s0) & (uint64_t)(uint16_t)127)); A3_CHARGE(); v0 = GetLayoutClass(ram, rt, a0, f12, f13, f14, f15, f16, f17, f18, f19, h2); }
    (v1 = (uint64_t)((uint64_t)v0 + (uint64_t)0ULL));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)SLL32((uint32_t)s0, 3))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v0, (uint32_t)s0))));
    (t5 = (uint32_t)((int32_t)((int32_t)SLL32((uint32_t)s1, 2))));
    (t3 = (uint64_t)((int64_t)(int32_t)((int32_t)SLL32((uint32_t)v0, 4))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)s2, 4))));
    (t1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v1, 340))));
    (a0 = (uint64_t)((uint64_t)s6 + (uint64_t)0ULL));
    (a1 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL));
    (a2 = (uint64_t)((uint64_t)v1 + (uint64_t)0ULL));
    (a3 = (uint64_t)((uint64_t)s0 + (uint64_t)0ULL));
    { const bool c = (int32_t)v0 >= 0; (t0 = (uint64_t)((uint64_t)s1 + (uint64_t)0ULL)); if (c) goto L_111b4c0; }
    { A3_CHARGE(); uint64_t a7c[9] = {0u, 0u, 0u, 0u, 0u, t0, 0u, 0u, 0u}; v0 = manager_sub_01114C70(ram, rt, a0, a1, a2, a3, t0, sp, f12, f13, f14, f15, f16, f17, f18, f19, f20, vf8, vf10, vf14, vf15, vf16, vf17, vf29, vf30, vf31, vacc, h5, h9, h29, a7c); f20 = a1_flt((uint32_t)a7c[1]); a2 = a7c[3]; a3 = a7c[4]; t0 = a7c[5]; t1 = a7c[6]; t2 = a7c[7]; t3 = a7c[8]; a3xw0 = true; }
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B(sp, 52))));
    goto L_111b504;
L_111b4c0:;
    (a3 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)v1, 388))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)t1, 32))));
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)a3, 32))));
    (t0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)t1, 8))));
    (a2 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v0, 40))));
    (t1 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)v1, 8))));
    (a3 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)v0, 36))));
    (t3 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)t3, (uint32_t)t1))));
    (t4 = (uint32_t)((int32_t)((int32_t)A1_R32B((uint32_t)v1, 24))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)t3, 84))));
    (t2 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)v1, 20))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(t5, (uint32_t)v0))));
    (t3 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)v0, 0))));
    (t3 = (uint64_t)((int64_t)(int32_t)((int32_t)SLL32((uint32_t)t3, 6))));
    { (t3 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(t4, (uint32_t)t3)))); A3_CHARGE(); uint64_t a7c[7] = {0u, a2, a3, t0, t1, t2, t3}; v0 = manager_sub_01116108(ram, rt, a0, a1, a2, a3, t0, t1, t2, t3, a1_sx64(sp), f12, f13, f14, f15, f16, f17, f18, f19, vf8, vf10, vf14, vf15, vf16, vf17, vf29, vf30, vf31, vacc, h11, a7c); a2 = a7c[1]; a3 = a7c[2]; t0 = a7c[3]; t1 = a7c[4]; t2 = a7c[5]; t3 = a7c[6]; }
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B(sp, 52))));
L_111b504:;
    vf29 = a4v::castsi128_ps(A4_R128(ADD32((uint32_t)v0, 0)));
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B(sp, 48))));
    vf30 = a4v::castsi128_ps(A4_R128(ADD32((uint32_t)v1, 0)));
    { a4::vec4 res = a4v::sub(vf29, vf30); a4::q128 mask = a4v::set_epi32(-1, -1, -1, -1); vf31 = a4v::blend(vf31, res, a4v::castsi128_ps(mask)); }
    A4_W128(ADD32((uint32_t)s7, 0), a4v::castps_si128(vf31));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B(sp, 56))));
    vf29 = a4v::castsi128_ps(A4_R128(ADD32((uint32_t)v0, 0)));
    { a4::vec4 res = a4v::sub(vf29, vf30); a4::q128 mask = a4v::set_epi32(-1, -1, -1, -1); vf31 = a4v::blend(vf31, res, a4v::castsi128_ps(mask)); }
    A4_W128(ADD32(s5, 0), a4v::castps_si128(vf31));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B(sp, 48))));
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(sp, 32))));
    vf5 = a4v::castsi128_ps(A4_R128(ADD32((uint32_t)s4, 0)));
    vf6 = a4v::castsi128_ps(A4_R128(ADD32((uint32_t)v0, 0)));
    { a4::vec4 res = a4v::sub(vf5, vf6); a4::q128 mask = a4v::set_epi32(-1, -1, -1, -1); vf7 = a4v::blend(vf7, res, a4v::castsi128_ps(mask)); }
    A4_W128(ADD32((uint32_t)v1, 0), a4v::castps_si128(vf7));
    vf5 = a4v::castsi128_ps(A4_R128(ADD32(sp, 0)));
    vf6 = a4v::castsi128_ps(A4_R128(ADD32(s5, 0)));
    vf7 = a4v::castsi128_ps(A4_R128(ADD32((uint32_t)v1, 0)));
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
    (at = (uint32_t)((int32_t)((int32_t)((uint32_t)47015 << 16))));
    at = at | 0xC5ACu;
    { uint32_t bits = a7_opq(at); std::memcpy(&f2, &bits, sizeof(bits)); }
    A1_W32(fp, a1_bits(f0));
    A1_W32B(sp, 68, a1_bits(f1));
    f0 = a1_flt(A1_R32(fp));
    fcr31 = (a1f::lt(f0, f2)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
    f0 = a1_flt(A1_R32B(sp, 64));
    f1 = a1_flt(A1_R32B(sp, 68));
    { float f = f0; uint32_t bits; std::memcpy(&bits, &f, sizeof(bits)); A1_W32B((uint32_t)s3, 8, bits); }
    { float f = f1; uint32_t bits; std::memcpy(&bits, &f, sizeof(bits)); A1_W32B((uint32_t)s3, 12, bits); }
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)s2, 0))));
    A1_W32B((uint32_t)s3, 0, (uint32_t)v0);
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)s2, 4))));
    A1_W32B((uint32_t)s3, 4, (uint32_t)v1);
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)s2, 4))));
    if ((int32_t)v0 >= 0) { (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 3)))); goto L_111b6a0; }
    { (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)s2, 0)))); A3_CHARGE(); v0 = GetLayoutClass(ram, rt, a0, f12, f13, f14, f15, f16, f17, f18, f19, h2); }
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 2))));
    A1_W32B((uint32_t)s3, 48, (uint32_t)v1);
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)s3, 16))));
    (v1 = (uint64_t)(uint32_t)((uint8_t)A1_R8B((uint32_t)s2, 7)));
    (a1 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)v0, 356))));
    (v1 = (uint64_t)(RC_U64X(v1) & (uint64_t)(uint16_t)127));
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)SLL32((uint32_t)v1, 6))));
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v1, (uint32_t)a1))));
    vf8 = a4v::castsi128_ps(A4_R128(ADD32((uint32_t)s4, 0)));
    vf14 = a4v::castsi128_ps(A4_R128(ADD32((uint32_t)v1, 0)));
    vf15 = a4v::castsi128_ps(A4_R128(ADD32((uint32_t)v1, 16)));
    vf16 = a4v::castsi128_ps(A4_R128(ADD32((uint32_t)v1, 32)));
    vf17 = a4v::castsi128_ps(A4_R128(ADD32((uint32_t)v1, 48)));
    { a4::vec4 res = a4v::mul(vf14, a4v::bcast<0>(vf8)); vacc = a4v::dest<1, 1, 1, 1>(vacc, res); }
    { a4::vec4 mul_res = a4v::mul(vf15, a4v::bcast<1>(vf8)); a4::vec4 res = a4v::addK<1, 1>(vacc, mul_res); vacc = a4v::dest<1, 1, 1, 1>(vacc, res); }
    { a4::vec4 mul_res = a4v::mul(vf16, a4v::bcast<2>(vf8)); a4::vec4 res = a4v::addK<1, 1>(vacc, mul_res); vacc = a4v::dest<1, 1, 1, 1>(vacc, res); }
    { a4::vec4 mul_res = a4v::mul(vf17, a4v::bcast<3>(vf8)); a4::vec4 res = a4v::addK<1, 1>(vacc, mul_res); a4::q128 mask = a4v::set_epi32(-1, -1, -1, -1); vf10 = a4v::blendv_ps(vf10, res, a4v::castsi128_ps(mask)); }
    A4_W128(ADD32((uint32_t)a0, 0), a4v::castps_si128(vf10));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v0, 160))));
    vf5 = a4v::castsi128_ps(A4_R128(ADD32((uint32_t)a0, 0)));
    vf6 = a4v::castsi128_ps(A4_R128(ADD32((uint32_t)v0, 0)));
    { a4::vec4 res = a4v::mul(vf5, vf6); a4::q128 mask = a4v::set_epi32(0, -1, -1, -1); vf5 = a4v::blend(vf5, res, a4v::castsi128_ps(mask)); }
    A4_W128(ADD32((uint32_t)a0, 0), a4v::castps_si128(vf5));
    goto L_111b6a8;
L_111b6a0:;
    A1_W32B((uint32_t)s3, 48, (uint32_t)v0);
L_111b6a8:;
    { sp = sp + 0xA0u; if (xo) { if (a3xw0) xo[0] = a1_bits(f20); xo[1] = a2; xo[2] = a3; xo[3] = t0; xo[4] = t1; xo[5] = t2; xo[6] = t3; } io_vf5 = vf5; io_vf6 = vf6; io_vf7 = vf7; io_vf8 = vf8; io_vf9 = vf9; io_vf10 = vf10; io_vf11 = vf11; io_vf14 = vf14; io_vf15 = vf15; io_vf16 = vf16; io_vf17 = vf17; io_vf29 = vf29; io_vf30 = vf30; io_vf31 = vf31; io_vacc = vacc; io_h2 = h2; io_h5 = h5; io_h9 = h9; io_h11 = h11; return a1::RetVF{(uint64_t)v0, f0}; }
    __builtin_unreachable();
}

// GetGeometryPointerWithCollisionEdgeGlobalPoint (0x111b6d8): typed by tools/typed_lift2.py (round A6; guest frame calls, extra outputs xo[0]=a2 xo[1]=a3 xo[2]=t0 xo[3]=t1 xo[4]=t2 xo[5]=t3; VU0 in vf10 vf29 vf31 vacc h9 h11 h29, out vf8 vf10 vf14 vf15 vf16 vf17 vf29 vf30 vf31 vacc h2 h5 h9 h11)
inline a1::RetVF GetGeometryPointerWithCollisionEdgeGlobalPoint(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint64_t a0, uint64_t a1, uint64_t a2, uint64_t a3, uint32_t sp, float f12, float f13, float f14, float f15, float f16, float f17, float f18, float f19, float f20, a4::vec4 &io_vf8, a4::vec4 &io_vf10, a4::vec4 &io_vf14, a4::vec4 &io_vf15, a4::vec4 &io_vf16, a4::vec4 &io_vf17, a4::vec4 &io_vf29, a4::vec4 &io_vf30, a4::vec4 &io_vf31, a4::vec4 &io_vacc, uint64_t &io_h2, uint64_t &io_h5, uint64_t &io_h9, uint64_t &io_h11, uint64_t io_h29, uint64_t *xo = nullptr)
{
    float f0 = 0.0f;
    float f1 = 0.0f;
    float f2 = 0.0f;
    float f3 = 0.0f;
    float f4 = 0.0f;
    float f5 = 0.0f;
    float f6 = 0.0f;
    uint64_t v0 = 0;
    uint32_t v1 = 0;
    uint64_t t0 = 0;
    uint64_t t1 = 0;
    uint64_t t2 = 0;
    uint64_t t3 = 0;
    uint32_t t4 = 0;
    uint32_t t5 = 0;
    uint64_t s0 = 0;
    uint64_t s1 = 0;
    uint64_t s2 = 0;
    uint64_t s3 = 0;
    uint64_t s4 = 0;
    uint64_t s5 = 0;
    uint32_t s6 = 0;
    uint64_t s7 = 0;
    uint64_t fp = 0;
    uint32_t fcr31 = 0;
    a4::vec4 vf8 = io_vf8;
    a4::vec4 vf10 = io_vf10;
    a4::vec4 vf14 = io_vf14;
    a4::vec4 vf15 = io_vf15;
    a4::vec4 vf16 = io_vf16;
    a4::vec4 vf17 = io_vf17;
    a4::vec4 vf29 = io_vf29;
    a4::vec4 vf30 = io_vf30;
    a4::vec4 vf31 = io_vf31;
    a4::vec4 vacc = io_vacc;
    uint64_t h2 = io_h2;
    uint64_t h5 = io_h5;
    uint64_t h9 = io_h9;
    uint64_t h11 = io_h11;
    uint64_t h29 = io_h29;
    sp = sp - 0x70u;
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)255 << 16))));
    (v0 = (uint64_t)(RC_U64X(v0) | (uint64_t)(uint16_t)65535));
    (s2 = (uint64_t)((uint64_t)a1 + (uint64_t)0ULL));
    (s3 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(sp, 16))));
    (s4 = (uint64_t)((uint64_t)a0 + (uint64_t)0ULL));
    (s5 = (uint64_t)((uint64_t)a2 + (uint64_t)0ULL));
    (s7 = (uint64_t)((uint64_t)a1_sx64(sp) + (uint64_t)0ULL));
    (fp = (uint64_t)((uint64_t)a3 + (uint64_t)0ULL));
    (a1 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)s2, 4))));
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)s2, 0))));
    (v1 = (uint32_t)((int32_t)((int32_t)SRL32((uint32_t)a1, 24))));
    (s1 = (uint64_t)(RC_U64X(a1) & RC_U64X(v0)));
    (s0 = (uint64_t)(a1_sx64(v1) & (uint64_t)(uint16_t)127));
    { (s6 = (uint32_t)((int32_t)((int32_t)A1_R32B((uint32_t)s2, 8)))); A3_CHARGE(); v0 = GetLayoutClass(ram, rt, a0, f12, f13, f14, f15, f16, f17, f18, f19, h2); }
    (a2 = (uint64_t)((uint64_t)v0 + (uint64_t)0ULL));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)s2, 4))));
    { const bool c = (int32_t)v0 >= 0; (a3 = (uint64_t)((uint64_t)s0 + (uint64_t)0ULL)); if (c) goto L_111b768; }
    (t0 = (uint64_t)((uint64_t)s1 + (uint64_t)0ULL));
    (a0 = (uint64_t)((uint64_t)s3 + (uint64_t)0ULL));
    { (a1 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL)); A3_CHARGE(); uint64_t a7c[9] = {0u, 0u, 0u, 0u, 0u, t0, 0u, 0u, 0u}; v0 = manager_sub_01114C70(ram, rt, a0, a1, a2, a3, t0, sp, f12, f13, f14, f15, f16, f17, f18, f19, f20, vf8, vf10, vf14, vf15, vf16, vf17, vf29, vf30, vf31, vacc, h5, h9, h29, a7c); a2 = a7c[3]; a3 = a7c[4]; t0 = a7c[5]; t1 = a7c[6]; t2 = a7c[7]; t3 = a7c[8]; }
    v1 = s6 << 2;
    goto L_111b7c8;
L_111b768:;
    (v1 = (uint32_t)((int32_t)((int32_t)A1_R32B((uint32_t)a2, 388))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)SLL32((uint32_t)s0, 3))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v0, (uint32_t)s0))));
    (t2 = (uint64_t)((int64_t)(int32_t)((int32_t)SLL32((uint32_t)s1, 2))));
    t4 = A1_R32B(v1, 32);
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)SLL32((uint32_t)v0, 4))));
    (t3 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)a2, 340))));
    (a0 = (uint64_t)((uint64_t)s3 + (uint64_t)0ULL));
    (t1 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B(t4, 8))));
    (a1 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL));
    (a2 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)t3, 32))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v0, (uint32_t)t1))));
    (t0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)t3, 8))));
    (v1 = (uint32_t)((int32_t)((int32_t)A1_R32B((uint32_t)v0, 84))));
    t5 = A1_R32B(t4, 24);
    (t2 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)t2, v1))));
    (a3 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)a2, 36))));
    (t3 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)t2, 0))));
    (a2 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)a2, 40))));
    (t2 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B(t4, 20))));
    (t3 = (uint64_t)((int64_t)(int32_t)((int32_t)SLL32((uint32_t)t3, 6))));
    { (t3 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(t5, (uint32_t)t3)))); A3_CHARGE(); uint64_t a7c[7] = {0u, a2, a3, t0, t1, t2, t3}; v0 = manager_sub_01116108(ram, rt, a0, a1, a2, a3, t0, t1, t2, t3, a1_sx64(sp), f12, f13, f14, f15, f16, f17, f18, f19, vf8, vf10, vf14, vf15, vf16, vf17, vf29, vf30, vf31, vacc, h11, a7c); a2 = a7c[1]; a3 = a7c[2]; t0 = a7c[3]; t1 = a7c[4]; t2 = a7c[5]; t3 = a7c[6]; }
    v1 = s6 << 2;
L_111b7c8:;
    { uint32_t bits = 0u; std::memcpy(&f6, &bits, sizeof(bits)); }
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)s3, v1))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)281 << 16))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v0, v1))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)v0, 21656))));
    (a1 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)a0, 0))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)SLL32((uint32_t)v0, 2))));
    A1_W32B((uint32_t)s7, 0, (uint32_t)a1);
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)s3, (uint32_t)v0))));
    (v1 = (uint32_t)((int32_t)((int32_t)A1_R32B((uint32_t)v0, 0))));
    A1_W32B((uint32_t)s7, 4, v1);
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B(sp, 0))));
    v1 = A1_R32B(sp, 4);
    { uint32_t bits = A1_R32B((uint32_t)v0, 0); float f; std::memcpy(&f, &bits, sizeof(f)); f1 = f; }
    f0 = a1_flt(A1_R32(v1));
    { uint32_t bits = A1_R32B((uint32_t)v0, 8); float f; std::memcpy(&f, &bits, sizeof(f)); f3 = f; }
    f5 = a1f::sub(f1, f0);
    f0 = a1_flt(A1_R32B(v1, 4));
    { uint32_t bits = A1_R32B((uint32_t)v0, 4); float f; std::memcpy(&f, &bits, sizeof(f)); f2 = f; }
    f1 = a1_flt(A1_R32B(v1, 8));
    f2 = a1f::sub(f2, f0);
    fcr31 = (a1f::lt(f5, f6)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
    f4 = ((float)(f5));
    { const bool c = (fcr31 & 0x800000u) == 0u; f3 = a1f::sub(f3, f1); if (c) goto L_111b830; }
    f4 = (-(float)(f5));
L_111b830:;
    fcr31 = (a1f::lt(f2, f6)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
    if ((fcr31 & 0x800000u) == 0u) goto L_111b858;
    f0 = (-(float)(f2));
    fcr31 = (a1f::lt(f0, f4)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
    if ((fcr31 & 0x800000u) != 0u) goto L_111b864;
    goto L_111b8d0;
L_111b858:;
    fcr31 = (a1f::lt(f2, f4)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
    if ((fcr31 & 0x800000u) == 0u) goto L_111b8d0;
L_111b864:;
    { uint32_t bits = 0u; std::memcpy(&f0, &bits, sizeof(bits)); }
    fcr31 = (a1f::lt(f5, f0)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
    { const bool c = (fcr31 & 0x800000u) == 0u; f1 = ((float)(f5)); if (c) goto L_111b878; }
    f1 = (-(float)(f5));
L_111b878:;
    fcr31 = (a1f::lt(f3, f0)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
    if ((fcr31 & 0x800000u) == 0u) goto L_111b8a0;
    f0 = (-(float)(f3));
    fcr31 = (a1f::lt(f0, f1)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
    if ((fcr31 & 0x800000u) != 0u) { { uint32_t bits = A1_R32B((uint32_t)s5, 0); float f; std::memcpy(&f, &bits, sizeof(f)); f0 = f; } goto L_111b8b0; }
    goto L_111b8d0;
L_111b8a0:;
    fcr31 = (a1f::lt(f3, f1)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
    if ((fcr31 & 0x800000u) == 0u) goto L_111b8d0;
    { uint32_t bits = A1_R32B((uint32_t)s5, 0); float f; std::memcpy(&f, &bits, sizeof(f)); f0 = f; }
L_111b8b0:;
    f1 = a1_flt(A1_R32(v1));
    f0 = a1f::sub(f0, f1);
    f0 = a1f::div(fcr31, f0, f5);
    (v1 = (uint32_t)((int32_t)((int32_t)A1_R32B((uint32_t)s2, 0))));
    goto L_111b998;
L_111b8d0:;
    { uint32_t bits = 0u; std::memcpy(&f0, &bits, sizeof(bits)); }
    fcr31 = (a1f::lt(f2, f0)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
    { const bool c = (fcr31 & 0x800000u) == 0u; f1 = ((float)(f2)); if (c) goto L_111b8e4; }
    f1 = (-(float)(f2));
L_111b8e4:;
    fcr31 = (a1f::lt(f5, f0)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
    if ((fcr31 & 0x800000u) == 0u) goto L_111b908;
    f0 = (-(float)(f5));
    fcr31 = (a1f::lt(f0, f1)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
    if ((fcr31 & 0x800000u) != 0u) goto L_111b914;
    { uint32_t bits = A1_R32B((uint32_t)s5, 8); float f; std::memcpy(&f, &bits, sizeof(f)); f0 = f; }
    goto L_111b980;
L_111b908:;
    fcr31 = (a1f::lt(f5, f1)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
    if ((fcr31 & 0x800000u) == 0u) { { uint32_t bits = A1_R32B((uint32_t)s5, 8); float f; std::memcpy(&f, &bits, sizeof(f)); f0 = f; } goto L_111b980; }
L_111b914:;
    { uint32_t bits = 0u; std::memcpy(&f0, &bits, sizeof(bits)); }
    fcr31 = (a1f::lt(f2, f0)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
    { const bool c = (fcr31 & 0x800000u) == 0u; f1 = ((float)(f2)); if (c) goto L_111b928; }
    f1 = (-(float)(f2));
L_111b928:;
    fcr31 = (a1f::lt(f3, f0)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
    if ((fcr31 & 0x800000u) == 0u) goto L_111b950;
    f0 = (-(float)(f3));
    fcr31 = (a1f::lt(f0, f1)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
    if ((fcr31 & 0x800000u) != 0u) { { uint32_t bits = A1_R32B((uint32_t)s5, 4); float f; std::memcpy(&f, &bits, sizeof(f)); f0 = f; } goto L_111b960; }
    { uint32_t bits = A1_R32B((uint32_t)s5, 8); float f; std::memcpy(&f, &bits, sizeof(f)); f0 = f; }
    goto L_111b980;
L_111b950:;
    fcr31 = (a1f::lt(f3, f1)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
    if ((fcr31 & 0x800000u) == 0u) { { uint32_t bits = A1_R32B((uint32_t)s5, 8); float f; std::memcpy(&f, &bits, sizeof(f)); f0 = f; } goto L_111b980; }
    { uint32_t bits = A1_R32B((uint32_t)s5, 4); float f; std::memcpy(&f, &bits, sizeof(f)); f0 = f; }
L_111b960:;
    f1 = a1_flt(A1_R32B(v1, 4));
    f0 = a1f::sub(f0, f1);
    f0 = a1f::div(fcr31, f0, f2);
    (v1 = (uint32_t)((int32_t)((int32_t)A1_R32B((uint32_t)s2, 0))));
    goto L_111b998;
L_111b980:;
    f1 = a1_flt(A1_R32B(v1, 8));
    f0 = a1f::sub(f0, f1);
    f0 = a1f::div(fcr31, f0, f3);
    (v1 = (uint32_t)((int32_t)((int32_t)A1_R32B((uint32_t)s2, 0))));
L_111b998:;
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 4))));
    A1_W32B((uint32_t)s4, 0, v1);
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)s2, 4))));
    A1_W32B((uint32_t)s4, 4, (uint32_t)v0);
    (v1 = (uint32_t)((int32_t)((int32_t)A1_R32B((uint32_t)s2, 8))));
    A1_W32B((uint32_t)s4, 16, (uint32_t)fp);
    A1_W32B((uint32_t)s4, 48, (uint32_t)a0);
    A1_W32B((uint32_t)s4, 8, v1);
    { float f = f0; uint32_t bits; std::memcpy(&bits, &f, sizeof(bits)); A1_W32B((uint32_t)s4, 12, bits); }
    { sp = sp + 0x70u; if (xo) { xo[0] = a2; xo[1] = a3; xo[2] = t0; xo[3] = t1; xo[4] = t2; xo[5] = t3; } io_vf8 = vf8; io_vf10 = vf10; io_vf14 = vf14; io_vf15 = vf15; io_vf16 = vf16; io_vf17 = vf17; io_vf29 = vf29; io_vf30 = vf30; io_vf31 = vf31; io_vacc = vacc; io_h2 = h2; io_h5 = h5; io_h9 = h9; io_h11 = h11; return a1::RetVF{(uint64_t)v0, f0}; }
    __builtin_unreachable();
}

// dispVuCollisionDB (0x11241c8): typed by tools/typed_lift2.py (round A6; guest frame calls, extra outputs xo[0]=f0 xo[1]=a3 xo[2]=t0 xo[3]=t1 xo[4]=t2 xo[5]=t3; VU0 in vf10 vf29 vf31 vacc h9 h11 h29, out vf8 vf10 vf14 vf15 vf16 vf17 vf29 vf30 vf31 vacc h2 h3 h5 h9 h11)
inline uint64_t dispVuCollisionDB(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint64_t a0, uint64_t a1, uint64_t sp, float f0, float f12, float f13, float f14, float f15, float f16, float f17, float f18, float f19, float f20, a4::vec4 &io_vf8, a4::vec4 &io_vf10, a4::vec4 &io_vf14, a4::vec4 &io_vf15, a4::vec4 &io_vf16, a4::vec4 &io_vf17, a4::vec4 &io_vf29, a4::vec4 &io_vf30, a4::vec4 &io_vf31, a4::vec4 &io_vacc, uint64_t &io_h2, uint64_t &io_h3, uint64_t &io_h5, uint64_t &io_h9, uint64_t &io_h11, uint64_t io_h29, uint64_t *xo = nullptr)
{
    uint64_t v0 = 0;
    uint64_t v1 = 0;
    uint64_t a2 = 0;
    uint64_t a3 = 0;
    uint64_t t0 = 0;
    uint64_t t1 = 0;
    uint64_t t2 = 0;
    uint64_t t3 = 0;
    uint32_t t4 = 0;
    uint32_t t5 = 0;
    uint64_t s0 = 0;
    uint64_t s1 = 0;
    uint64_t s2 = 0;
    uint64_t s3 = 0;
    a4::vec4 vf8 = io_vf8;
    a4::vec4 vf10 = io_vf10;
    a4::vec4 vf14 = io_vf14;
    a4::vec4 vf15 = io_vf15;
    a4::vec4 vf16 = io_vf16;
    a4::vec4 vf17 = io_vf17;
    a4::vec4 vf29 = io_vf29;
    a4::vec4 vf30 = io_vf30;
    a4::vec4 vf31 = io_vf31;
    a4::vec4 vacc = io_vacc;
    uint64_t h2 = io_h2;
    uint64_t h3 = io_h3;
    uint64_t h5 = io_h5;
    uint64_t h9 = io_h9;
    uint64_t h11 = io_h11;
    uint64_t h29 = io_h29;
    bool a3xw0 = false;
    (sp = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)sp, 4294967232))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)255 << 16))));
    (v0 = (uint64_t)(RC_U64X(v0) | (uint64_t)(uint16_t)65535));
    (s2 = (uint64_t)((uint64_t)a0 + (uint64_t)0ULL));
    (s3 = (uint64_t)((uint64_t)a1 + (uint64_t)0ULL));
    (s1 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)s2, 4))));
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)s2, 0))));
    (s0 = (uint64_t)((int64_t)(int32_t)((int32_t)SRL32((uint32_t)s1, 24))));
    (s1 = (uint64_t)(RC_U64X(s1) & RC_U64X(v0)));
    { (s0 = (uint64_t)(RC_U64X(s0) & (uint64_t)(uint16_t)127)); A3_CHARGE(); v0 = GetLayoutClass(ram, rt, a0, f12, f13, f14, f15, f16, f17, f18, f19, h2); }
    (t1 = (uint64_t)((uint64_t)v0 + (uint64_t)0ULL));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)SLL32((uint32_t)s0, 3))));
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)s2, 4))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v0, (uint32_t)s0))));
    (t5 = (uint32_t)((int32_t)((int32_t)SLL32((uint32_t)s1, 2))));
    (t2 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)t1, 340))));
    (t3 = (uint64_t)((int64_t)(int32_t)((int32_t)SLL32((uint32_t)v0, 4))));
    (a0 = (uint64_t)((uint64_t)sp + (uint64_t)0ULL));
    (a1 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL));
    (a2 = (uint64_t)((uint64_t)t1 + (uint64_t)0ULL));
    (a3 = (uint64_t)((uint64_t)s0 + (uint64_t)0ULL));
    { const bool c = (int32_t)v1 >= 0; (t0 = (uint64_t)((uint64_t)s1 + (uint64_t)0ULL)); if (c) goto L_1124250; }
    { A3_CHARGE(); uint64_t a7c[9] = {(uint64_t)a1_bits(f0), 0u, 0u, 0u, 0u, t0, 0u, 0u, 0u}; v0 = manager_sub_01114C70(ram, rt, a0, a1, a2, a3, t0, (uint32_t)sp, f12, f13, f14, f15, f16, f17, f18, f19, f20, vf8, vf10, vf14, vf15, vf16, vf17, vf29, vf30, vf31, vacc, h5, h9, h29, a7c); f0 = a1_flt((uint32_t)a7c[0]); a3 = a7c[4]; t0 = a7c[5]; t1 = a7c[6]; t2 = a7c[7]; t3 = a7c[8]; a3xw0 = true; }
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)sp, 0))));
    goto L_1124294;
L_1124250:;
    (a3 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)t1, 388))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)t2, 32))));
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)a3, 32))));
    (a2 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v0, 40))));
    (a3 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)v0, 36))));
    (t1 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)v1, 8))));
    (t4 = (uint32_t)((int32_t)((int32_t)A1_R32B((uint32_t)v1, 24))));
    (t3 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)t3, (uint32_t)t1))));
    (t0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)t2, 8))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)t3, 84))));
    (t2 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)v1, 20))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(t5, (uint32_t)v0))));
    (t3 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)v0, 0))));
    (t3 = (uint64_t)((int64_t)(int32_t)((int32_t)SLL32((uint32_t)t3, 6))));
    { (t3 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(t4, (uint32_t)t3)))); A3_CHARGE(); uint64_t a7c[7] = {0u, 0u, a3, t0, t1, t2, t3}; v0 = manager_sub_01116108(ram, rt, a0, a1, a2, a3, t0, t1, t2, t3, sp, f12, f13, f14, f15, f16, f17, f18, f19, vf8, vf10, vf14, vf15, vf16, vf17, vf29, vf30, vf31, vacc, h11, a7c); a3 = a7c[2]; t0 = a7c[3]; t1 = a7c[4]; t2 = a7c[5]; t3 = a7c[6]; }
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)sp, 0))));
L_1124294:;
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)sp, 4))));
    do { const a4::q128 _v = (A4_R128(ADD32((uint32_t)v1, 0))); h2 = a4::hi64(_v); std::memcpy(&v0, &_v, 8); } while (0);
    (a1 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)sp, 8))));
    A4_W128(ADD32((uint32_t)s3, 0), a4::mk128(h2, v0));
    do { const a4::q128 _v = (A4_R128(ADD32((uint32_t)a0, 0))); h3 = a4::hi64(_v); std::memcpy(&v1, &_v, 8); } while (0);
    A4_W128(ADD32((uint32_t)s3, 16), a4::mk128(h3, v1));
    do { const a4::q128 _v = (A4_R128(ADD32((uint32_t)a1, 0))); h2 = a4::hi64(_v); std::memcpy(&v0, &_v, 8); } while (0);
    A4_W128(ADD32((uint32_t)s3, 32), a4::mk128(h2, v0));
    { (sp = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)sp, 64)))); if (xo) { if (a3xw0) xo[0] = a1_bits(f0); xo[1] = a3; xo[2] = t0; xo[3] = t1; xo[4] = t2; xo[5] = t3; } io_vf8 = vf8; io_vf10 = vf10; io_vf14 = vf14; io_vf15 = vf15; io_vf16 = vf16; io_vf17 = vf17; io_vf29 = vf29; io_vf30 = vf30; io_vf31 = vf31; io_vacc = vacc; io_h2 = h2; io_h3 = h3; io_h5 = h5; io_h9 = h9; io_h11 = h11; return v0; }
    __builtin_unreachable();
}

// GetCollisionDBVertexPoint (0x1125190): typed by tools/typed_lift2.py (round A6; frame calls, extra outputs xo[0]=f20 xo[1]=a1 xo[2]=a2 xo[3]=a3 xo[4]=t0 xo[5]=t1 xo[6]=t2 xo[7]=t3; VU0 in vf10 vf29 vf31 vacc h9 h11 h29, out vf8 vf10 vf14 vf15 vf16 vf17 vf29 vf30 vf31 vacc h2 h5 h9 h11, guest sp, frame 48 bytes, 0 slot accesses)
inline uint64_t GetCollisionDBVertexPoint(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint64_t a0, uint64_t a1, uint32_t sp, float f12, float f13, float f14, float f15, float f16, float f17, float f18, float f19, float f20, a4::vec4 &io_vf8, a4::vec4 &io_vf10, a4::vec4 &io_vf14, a4::vec4 &io_vf15, a4::vec4 &io_vf16, a4::vec4 &io_vf17, a4::vec4 &io_vf29, a4::vec4 &io_vf30, a4::vec4 &io_vf31, a4::vec4 &io_vacc, uint64_t &io_h2, uint64_t &io_h5, uint64_t &io_h9, uint64_t &io_h11, uint64_t io_h29, uint64_t *xo = nullptr)
{
    uint64_t v0 = 0;
    uint32_t v1 = 0;
    uint64_t a2 = 0;
    uint64_t a3 = 0;
    uint64_t t0 = 0;
    uint64_t t1 = 0;
    uint64_t t2 = 0;
    uint64_t t3 = 0;
    uint32_t t4 = 0;
    uint32_t t5 = 0;
    uint64_t s0 = 0;
    uint64_t s1 = 0;
    uint64_t s2 = 0;
    uint64_t s3 = 0;
    a4::vec4 vf8 = io_vf8;
    a4::vec4 vf10 = io_vf10;
    a4::vec4 vf14 = io_vf14;
    a4::vec4 vf15 = io_vf15;
    a4::vec4 vf16 = io_vf16;
    a4::vec4 vf17 = io_vf17;
    a4::vec4 vf29 = io_vf29;
    a4::vec4 vf30 = io_vf30;
    a4::vec4 vf31 = io_vf31;
    a4::vec4 vacc = io_vacc;
    uint64_t h2 = io_h2;
    uint64_t h5 = io_h5;
    uint64_t h9 = io_h9;
    uint64_t h11 = io_h11;
    uint64_t h29 = io_h29;
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)255 << 16))));
    (v0 = (uint64_t)(RC_U64X(v0) | (uint64_t)(uint16_t)65535));
    (s2 = (uint64_t)((uint64_t)a1 + (uint64_t)0ULL));
    (s3 = (uint64_t)((uint64_t)a0 + (uint64_t)0ULL));
    (s1 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)s2, 4))));
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)s2, 0))));
    (s0 = (uint64_t)((int64_t)(int32_t)((int32_t)SRL32((uint32_t)s1, 24))));
    (s1 = (uint64_t)(RC_U64X(s1) & RC_U64X(v0)));
    { (s0 = (uint64_t)(RC_U64X(s0) & (uint64_t)(uint16_t)127)); A3_CHARGE(); v0 = GetLayoutClass(ram, rt, a0, f12, f13, f14, f15, f16, f17, f18, f19, h2); }
    (t1 = (uint64_t)((uint64_t)v0 + (uint64_t)0ULL));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)SLL32((uint32_t)s0, 3))));
    (v1 = (uint32_t)((int32_t)((int32_t)A1_R32B((uint32_t)s2, 4))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v0, (uint32_t)s0))));
    (t5 = (uint32_t)((int32_t)((int32_t)SLL32((uint32_t)s1, 2))));
    (t2 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)t1, 340))));
    (t3 = (uint64_t)((int64_t)(int32_t)((int32_t)SLL32((uint32_t)v0, 4))));
    (a0 = (uint64_t)((uint64_t)s3 + (uint64_t)0ULL));
    (a2 = (uint64_t)((uint64_t)t1 + (uint64_t)0ULL));
    (a3 = (uint64_t)((uint64_t)s0 + (uint64_t)0ULL));
    (t0 = (uint64_t)((uint64_t)s1 + (uint64_t)0ULL));
    { const bool c = (int32_t)v1 >= 0; (a1 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL)); if (c) goto L_1125220; }
    { uint64_t a7c[9] = {0u, 0u, 0u, 0u, 0u, t0, 0u, 0u, 0u}; const uint64_t a7_t = manager_sub_01114C70(ram, rt, a0, a1, a2, a3, t0, (uint32_t)(sp + 0x0u), f12, f13, f14, f15, f16, f17, f18, f19, f20, vf8, vf10, vf14, vf15, vf16, vf17, vf29, vf30, vf31, vacc, h5, h9, h29, a7c); f20 = a1_flt((uint32_t)a7c[1]); a1 = a7c[2]; a2 = a7c[3]; a3 = a7c[4]; t0 = a7c[5]; t1 = a7c[6]; t2 = a7c[7]; t3 = a7c[8]; if (xo) { xo[0] = a1_bits(f20); xo[1] = a1; xo[2] = a2; xo[3] = a3; xo[4] = t0; xo[5] = t1; xo[6] = t2; xo[7] = t3; } io_vf8 = vf8; io_vf10 = vf10; io_vf14 = vf14; io_vf15 = vf15; io_vf16 = vf16; io_vf17 = vf17; io_vf29 = vf29; io_vf30 = vf30; io_vf31 = vf31; io_vacc = vacc; io_h2 = h2; io_h5 = h5; io_h9 = h9; io_h11 = h11; return a7_t; }
L_1125220:;
    (a3 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)t1, 388))));
    (a0 = (uint64_t)((uint64_t)s3 + (uint64_t)0ULL));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)t2, 32))));
    (v1 = (uint32_t)((int32_t)((int32_t)A1_R32B((uint32_t)a3, 32))));
    (a2 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v0, 40))));
    (a3 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)v0, 36))));
    (t1 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B(v1, 8))));
    t4 = A1_R32B(v1, 24);
    (t3 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)t3, (uint32_t)t1))));
    (t0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)t2, 8))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)t3, 84))));
    (t2 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B(v1, 20))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(t5, (uint32_t)v0))));
    (t3 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)v0, 0))));
    (t3 = (uint64_t)((int64_t)(int32_t)((int32_t)SLL32((uint32_t)t3, 6))));
    (t3 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(t4, (uint32_t)t3))));
    { uint64_t a7c[7] = {a1, a2, a3, t0, t1, t2, t3}; const uint64_t a7_t = manager_sub_01116108(ram, rt, a0, a1, a2, a3, t0, t1, t2, t3, a1_sx64((uint32_t)(sp + 0x0u)), f12, f13, f14, f15, f16, f17, f18, f19, vf8, vf10, vf14, vf15, vf16, vf17, vf29, vf30, vf31, vacc, h11, a7c); a1 = a7c[0]; a2 = a7c[1]; a3 = a7c[2]; t0 = a7c[3]; t1 = a7c[4]; t2 = a7c[5]; t3 = a7c[6]; if (xo) { xo[1] = a1; xo[2] = a2; xo[3] = a3; xo[4] = t0; xo[5] = t1; xo[6] = t2; xo[7] = t3; } io_vf8 = vf8; io_vf10 = vf10; io_vf14 = vf14; io_vf15 = vf15; io_vf16 = vf16; io_vf17 = vf17; io_vf29 = vf29; io_vf30 = vf30; io_vf31 = vf31; io_vacc = vacc; io_h2 = h2; io_h5 = h5; io_h9 = h9; io_h11 = h11; return a7_t; }
    __builtin_unreachable();
}

// GetCollisionDBPositionWithST (0x11252c0): typed by tools/typed_lift2.py (round A6; guest frame calls, extra outputs xo[0]=f0 xo[1]=f20 xo[2]=f21 xo[3]=a2 xo[4]=a3 xo[5]=t0 xo[6]=t1 xo[7]=t2 xo[8]=t3; VU0 in vf10 vf29 vf31 vacc h2 h5 h9 h11 h29, out vf5 vf6 vf8 vf10 vf14 vf15 vf16 vf17 vf29 vf30 vf31 vacc h2 h5 h9 h11)
inline uint64_t GetCollisionDBPositionWithST(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint64_t a0, uint64_t a1, uint32_t sp, float f0, float f12, float f13, float f14, float f15, float f16, float f17, float f18, float f19, float f20, float f21, a4::vec4 &io_vf5, a4::vec4 &io_vf6, a4::vec4 &io_vf8, a4::vec4 &io_vf10, a4::vec4 &io_vf14, a4::vec4 &io_vf15, a4::vec4 &io_vf16, a4::vec4 &io_vf17, a4::vec4 &io_vf29, a4::vec4 &io_vf30, a4::vec4 &io_vf31, a4::vec4 &io_vacc, uint64_t &io_h2, uint64_t &io_h5, uint64_t &io_h9, uint64_t &io_h11, uint64_t io_h29, uint64_t *xo = nullptr)
{
    uint64_t v0 = 0;
    uint64_t v1 = 0;
    uint64_t a2 = 0;
    uint64_t a3 = 0;
    uint64_t t0 = 0;
    uint64_t t1 = 0;
    uint64_t t2 = 0;
    uint64_t t3 = 0;
    uint32_t t4 = 0;
    uint32_t t5 = 0;
    uint64_t s0 = 0;
    uint64_t s1 = 0;
    uint64_t s2 = 0;
    uint64_t s3 = 0;
    uint32_t s4 = 0;
    uint64_t s5 = 0;
    uint64_t s6 = 0;
    a4::vec4 vf5 = io_vf5;
    a4::vec4 vf6 = io_vf6;
    a4::vec4 vf8 = io_vf8;
    a4::vec4 vf10 = io_vf10;
    a4::vec4 vf14 = io_vf14;
    a4::vec4 vf15 = io_vf15;
    a4::vec4 vf16 = io_vf16;
    a4::vec4 vf17 = io_vf17;
    a4::vec4 vf29 = io_vf29;
    a4::vec4 vf30 = io_vf30;
    a4::vec4 vf31 = io_vf31;
    a4::vec4 vacc = io_vacc;
    uint64_t h2 = io_h2;
    uint64_t h5 = io_h5;
    uint64_t h9 = io_h9;
    uint64_t h11 = io_h11;
    uint64_t h29 = io_h29;
    bool a3xw0 = false;
    sp = sp - 0xA0u;
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)255 << 16))));
    (v0 = (uint64_t)(RC_U64X(v0) | (uint64_t)(uint16_t)65535));
    (s2 = (uint64_t)((uint64_t)a1 + (uint64_t)0ULL));
    (s3 = (uint64_t)((uint64_t)a0 + (uint64_t)0ULL));
    s4 = sp + 0x10u;
    (s5 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(sp, 64))));
    (s6 = (uint64_t)((uint64_t)a1_sx64(sp) + (uint64_t)0ULL));
    { float f = f21; uint32_t bits; std::memcpy(&bits, &f, sizeof(bits)); A1_W32B(sp, 152, bits); }
    f21 = ((float)(f13));
    A1_W32B(sp, 144, a1_bits(f20));
    f20 = ((float)(f12));
    (s1 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)s2, 4))));
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)s2, 0))));
    (s0 = (uint64_t)((int64_t)(int32_t)((int32_t)SRL32((uint32_t)s1, 24))));
    (s1 = (uint64_t)(RC_U64X(s1) & RC_U64X(v0)));
    { (s0 = (uint64_t)(RC_U64X(s0) & (uint64_t)(uint16_t)127)); A3_CHARGE(); v0 = GetLayoutClass(ram, rt, a0, f12, f13, f14, f15, f16, f17, f18, f19, h2); }
    (v1 = (uint64_t)((uint64_t)v0 + (uint64_t)0ULL));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)SLL32((uint32_t)s0, 3))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v0, (uint32_t)s0))));
    (t5 = (uint32_t)((int32_t)((int32_t)SLL32((uint32_t)s1, 2))));
    (t3 = (uint64_t)((int64_t)(int32_t)((int32_t)SLL32((uint32_t)v0, 4))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)s2, 4))));
    (t1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v1, 340))));
    (a0 = (uint64_t)((uint64_t)s5 + (uint64_t)0ULL));
    (a1 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL));
    (a2 = (uint64_t)((uint64_t)v1 + (uint64_t)0ULL));
    (a3 = (uint64_t)((uint64_t)s0 + (uint64_t)0ULL));
    { const bool c = (int32_t)v0 >= 0; (t0 = (uint64_t)((uint64_t)s1 + (uint64_t)0ULL)); if (c) goto L_1125370; }
    { A3_CHARGE(); uint64_t a7c[9] = {(uint64_t)a1_bits(f0), 0u, 0u, 0u, 0u, t0, 0u, 0u, 0u}; v0 = manager_sub_01114C70(ram, rt, a0, a1, a2, a3, t0, sp, f12, f13, f14, f15, f16, f17, f18, f19, f20, vf8, vf10, vf14, vf15, vf16, vf17, vf29, vf30, vf31, vacc, h5, h9, h29, a7c); f0 = a1_flt((uint32_t)a7c[0]); f20 = a1_flt((uint32_t)a7c[1]); a2 = a7c[3]; a3 = a7c[4]; t0 = a7c[5]; t1 = a7c[6]; t2 = a7c[7]; t3 = a7c[8]; a3xw0 = true; }
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B(sp, 68))));
    goto L_11253b4;
L_1125370:;
    (a3 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)v1, 388))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)t1, 32))));
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)a3, 32))));
    (t0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)t1, 8))));
    (a2 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v0, 40))));
    (t1 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)v1, 8))));
    (a3 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)v0, 36))));
    (t3 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)t3, (uint32_t)t1))));
    (t4 = (uint32_t)((int32_t)((int32_t)A1_R32B((uint32_t)v1, 24))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)t3, 84))));
    (t2 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)v1, 20))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(t5, (uint32_t)v0))));
    (t3 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)v0, 0))));
    (t3 = (uint64_t)((int64_t)(int32_t)((int32_t)SLL32((uint32_t)t3, 6))));
    { (t3 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(t4, (uint32_t)t3)))); A3_CHARGE(); uint64_t a7c[7] = {0u, a2, a3, t0, t1, t2, t3}; v0 = manager_sub_01116108(ram, rt, a0, a1, a2, a3, t0, t1, t2, t3, a1_sx64(sp), f12, f13, f14, f15, f16, f17, f18, f19, vf8, vf10, vf14, vf15, vf16, vf17, vf29, vf30, vf31, vacc, h11, a7c); a2 = a7c[1]; a3 = a7c[2]; t0 = a7c[3]; t1 = a7c[4]; t2 = a7c[5]; t3 = a7c[6]; }
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B(sp, 68))));
L_11253b4:;
    vf29 = a4v::castsi128_ps(A4_R128(ADD32((uint32_t)v0, 0)));
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B(sp, 64))));
    vf30 = a4v::castsi128_ps(A4_R128(ADD32((uint32_t)v1, 0)));
    { a4::vec4 res = a4v::sub(vf29, vf30); a4::q128 mask = a4v::set_epi32(-1, -1, -1, -1); vf31 = a4v::blend(vf31, res, a4v::castsi128_ps(mask)); }
    A4_W128(ADD32((uint32_t)s6, 0), a4v::castps_si128(vf31));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B(sp, 72))));
    vf29 = a4v::castsi128_ps(A4_R128(ADD32((uint32_t)v0, 0)));
    { a4::vec4 res = a4v::sub(vf29, vf30); a4::q128 mask = a4v::set_epi32(-1, -1, -1, -1); vf31 = a4v::blend(vf31, res, a4v::castsi128_ps(mask)); }
    A4_W128(ADD32(s4, 0), a4v::castps_si128(vf31));
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B(sp, 64))));
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(sp, 32))));
    { uint32_t bits; std::memcpy(&bits, &f20, sizeof(bits)); (v0 = (uint64_t)((int64_t)(int32_t)(bits))); }
    vf5 = a4v::castsi128_ps(A4_R128(ADD32(sp, 0)));
    vf6 = a4v::castsi128_ps(a4::mk128(h2, v0));
    { a4::vec4 res = a4v::mul(vf5, a4v::bcast<0>(vf6)); a4::q128 mask = a4v::set_epi32(0, -1, -1, -1); vf5 = a4v::blendv_ps(vf5, res, a4v::castsi128_ps(mask)); }
    A4_W128(ADD32((uint32_t)v1, 0), a4v::castps_si128(vf5));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(sp, 48))));
    { uint32_t bits; std::memcpy(&bits, &f21, sizeof(bits)); (a1 = (uint64_t)((int64_t)(int32_t)(bits))); }
    vf5 = a4v::castsi128_ps(A4_R128(ADD32(s4, 0)));
    vf6 = a4v::castsi128_ps(a4::mk128(h5, a1));
    { a4::vec4 res = a4v::mul(vf5, a4v::bcast<0>(vf6)); a4::q128 mask = a4v::set_epi32(0, -1, -1, -1); vf5 = a4v::blendv_ps(vf5, res, a4v::castsi128_ps(mask)); }
    A4_W128(ADD32((uint32_t)v0, 0), a4v::castps_si128(vf5));
    vf5 = a4v::castsi128_ps(A4_R128(ADD32((uint32_t)v1, 0)));
    vf6 = a4v::castsi128_ps(A4_R128(ADD32((uint32_t)v0, 0)));
    { a4::vec4 res = a4v::add(vf5, vf6); a4::q128 mask = a4v::set_epi32(0, -1, -1, -1); vf5 = a4v::blend(vf5, res, a4v::castsi128_ps(mask)); }
    A4_W128(ADD32((uint32_t)s3, 0), a4v::castps_si128(vf5));
    vf5 = a4v::castsi128_ps(A4_R128(ADD32((uint32_t)a0, 0)));
    vf6 = a4v::castsi128_ps(A4_R128(ADD32((uint32_t)s3, 0)));
    { a4::vec4 res = a4v::add(vf5, vf6); a4::q128 mask = a4v::set_epi32(0, -1, -1, -1); vf5 = a4v::blend(vf5, res, a4v::castsi128_ps(mask)); }
    A4_W128(ADD32((uint32_t)s3, 0), a4v::castps_si128(vf5));
    { uint32_t bits = A1_R32B(sp, 152); float f; std::memcpy(&f, &bits, sizeof(f)); f21 = f; }
    f20 = a1_flt(A1_R32B(sp, 144));
    { sp = sp + 0xA0u; if (xo) { if (a3xw0) xo[0] = a1_bits(f0); xo[1] = a1_bits(f20); xo[2] = a1_bits(f21); xo[3] = a2; xo[4] = a3; xo[5] = t0; xo[6] = t1; xo[7] = t2; xo[8] = t3; } io_vf5 = vf5; io_vf6 = vf6; io_vf8 = vf8; io_vf10 = vf10; io_vf14 = vf14; io_vf15 = vf15; io_vf16 = vf16; io_vf17 = vf17; io_vf29 = vf29; io_vf30 = vf30; io_vf31 = vf31; io_vacc = vacc; io_h2 = h2; io_h5 = h5; io_h9 = h9; io_h11 = h11; return v0; }
    __builtin_unreachable();
}

// GetCollisionDBVectorPlane (0x1125680): typed by tools/typed_lift2.py (round A6; guest frame calls; VU0 in vf10 vf29 vf31 vacc h9 h11 h29, out vf8 vf10 vf14 vf15 vf16 vf17 vf29 vf30 vf31 vacc h2 h5 h9 h11)
inline uint64_t GetCollisionDBVectorPlane(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint64_t a0, uint64_t a1, uint64_t a2, uint64_t sp, float f12, float f13, float f14, float f15, float f16, float f17, float f18, float f19, float f20, a4::vec4 &io_vf8, a4::vec4 &io_vf10, a4::vec4 &io_vf14, a4::vec4 &io_vf15, a4::vec4 &io_vf16, a4::vec4 &io_vf17, a4::vec4 &io_vf29, a4::vec4 &io_vf30, a4::vec4 &io_vf31, a4::vec4 &io_vacc, uint64_t &io_h2, uint64_t &io_h5, uint64_t &io_h9, uint64_t &io_h11, uint64_t io_h29)
{
    uint64_t v0 = 0;
    uint64_t v1 = 0;
    uint64_t a3 = 0;
    uint64_t t0 = 0;
    uint32_t t1 = 0;
    uint32_t t2 = 0;
    uint32_t t3 = 0;
    uint32_t t4 = 0;
    uint32_t t5 = 0;
    uint64_t s0 = 0;
    uint64_t s1 = 0;
    uint64_t s2 = 0;
    uint64_t s3 = 0;
    uint64_t s4 = 0;
    a4::vec4 vf8 = io_vf8;
    a4::vec4 vf10 = io_vf10;
    a4::vec4 vf14 = io_vf14;
    a4::vec4 vf15 = io_vf15;
    a4::vec4 vf16 = io_vf16;
    a4::vec4 vf17 = io_vf17;
    a4::vec4 vf29 = io_vf29;
    a4::vec4 vf30 = io_vf30;
    a4::vec4 vf31 = io_vf31;
    a4::vec4 vacc = io_vacc;
    uint64_t h2 = io_h2;
    uint64_t h5 = io_h5;
    uint64_t h9 = io_h9;
    uint64_t h11 = io_h11;
    uint64_t h29 = io_h29;
    (sp = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)sp, 4294967232))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)255 << 16))));
    (v0 = (uint64_t)(RC_U64X(v0) | (uint64_t)(uint16_t)65535));
    (s2 = (uint64_t)((uint64_t)a2 + (uint64_t)0ULL));
    (s3 = (uint64_t)((uint64_t)a1 + (uint64_t)0ULL));
    (s4 = (uint64_t)((uint64_t)a0 + (uint64_t)0ULL));
    (s1 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)s2, 4))));
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)s2, 0))));
    (s0 = (uint64_t)((int64_t)(int32_t)((int32_t)SRL32((uint32_t)s1, 24))));
    (s1 = (uint64_t)(RC_U64X(s1) & RC_U64X(v0)));
    { (s0 = (uint64_t)(RC_U64X(s0) & (uint64_t)(uint16_t)127)); A3_CHARGE(); v0 = GetLayoutClass(ram, rt, a0, f12, f13, f14, f15, f16, f17, f18, f19, h2); }
    (v1 = (uint64_t)((uint64_t)v0 + (uint64_t)0ULL));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)SLL32((uint32_t)s0, 3))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v0, (uint32_t)s0))));
    (t5 = (uint32_t)((int32_t)((int32_t)SLL32((uint32_t)s1, 2))));
    (t3 = (uint32_t)((int32_t)((int32_t)SLL32((uint32_t)v0, 4))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)s2, 4))));
    (t1 = (uint32_t)((int32_t)((int32_t)ADD32((uint32_t)v1, 340))));
    (a0 = (uint64_t)((uint64_t)sp + (uint64_t)0ULL));
    (a1 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL));
    (a2 = (uint64_t)((uint64_t)v1 + (uint64_t)0ULL));
    (a3 = (uint64_t)((uint64_t)s0 + (uint64_t)0ULL));
    { const bool c = (int32_t)v0 >= 0; (t0 = (uint64_t)((uint64_t)s1 + (uint64_t)0ULL)); if (c) goto L_1125710; }
    { A3_CHARGE(); v0 = manager_sub_01114C70(ram, rt, a0, a1, a2, a3, t0, (uint32_t)sp, f12, f13, f14, f15, f16, f17, f18, f19, f20, vf8, vf10, vf14, vf15, vf16, vf17, vf29, vf30, vf31, vacc, h5, h9, h29); }
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)sp, 4))));
    goto L_1125754;
L_1125710:;
    (a3 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)v1, 388))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B(t1, 32))));
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)a3, 32))));
    (t0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B(t1, 8))));
    (a2 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v0, 40))));
    (t1 = (uint32_t)((int32_t)((int32_t)A1_R32B((uint32_t)v1, 8))));
    (a3 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)v0, 36))));
    t3 = t3 + t1;
    (t4 = (uint32_t)((int32_t)((int32_t)A1_R32B((uint32_t)v1, 24))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B(t3, 84))));
    (t2 = (uint32_t)((int32_t)((int32_t)A1_R32B((uint32_t)v1, 20))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(t5, (uint32_t)v0))));
    (t3 = (uint32_t)((int32_t)((int32_t)A1_R32B((uint32_t)v0, 0))));
    t3 = t3 << 6;
    { t3 = t4 + t3; A3_CHARGE(); v0 = manager_sub_01116108(ram, rt, a0, a1, a2, a3, t0, a1_sx64(t1), a1_sx64(t2), a1_sx64(t3), sp, f12, f13, f14, f15, f16, f17, f18, f19, vf8, vf10, vf14, vf15, vf16, vf17, vf29, vf30, vf31, vacc, h11); }
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)sp, 4))));
L_1125754:;
    vf29 = a4v::castsi128_ps(A4_R128(ADD32((uint32_t)v0, 0)));
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)sp, 0))));
    vf30 = a4v::castsi128_ps(A4_R128(ADD32((uint32_t)v1, 0)));
    { a4::vec4 res = a4v::sub(vf29, vf30); a4::q128 mask = a4v::set_epi32(-1, -1, -1, -1); vf31 = a4v::blend(vf31, res, a4v::castsi128_ps(mask)); }
    A4_W128(ADD32((uint32_t)s4, 0), a4v::castps_si128(vf31));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)sp, 8))));
    vf29 = a4v::castsi128_ps(A4_R128(ADD32((uint32_t)v0, 0)));
    { a4::vec4 res = a4v::sub(vf29, vf30); a4::q128 mask = a4v::set_epi32(-1, -1, -1, -1); vf31 = a4v::blend(vf31, res, a4v::castsi128_ps(mask)); }
    A4_W128(ADD32((uint32_t)s3, 0), a4v::castps_si128(vf31));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)sp, 0))));
    { (sp = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)sp, 64)))); io_vf8 = vf8; io_vf10 = vf10; io_vf14 = vf14; io_vf15 = vf15; io_vf16 = vf16; io_vf17 = vf17; io_vf29 = vf29; io_vf30 = vf30; io_vf31 = vf31; io_vacc = vacc; io_h2 = h2; io_h5 = h5; io_h9 = h9; io_h11 = h11; return v0; }
    __builtin_unreachable();
}

// GetCollisionDBPlane (0x11257a0): typed by tools/typed_lift2.py (round A6; guest frame calls, extra outputs xo[0]=f0 xo[1]=f20 xo[2]=a0 xo[3]=a1 xo[4]=a2 xo[5]=a3 xo[6]=t0 xo[7]=t1 xo[8]=t2 xo[9]=t3; VU0 in vf0 vf6 vf7 vf10 vf18 vf29 vf31 vacc vstat h9 h11 h29, out vf4 vf5 vf6 vf7 vf8 vf10 vf14 vf15 vf16 vf17 vf18 vf29 vf30 vf31 vacc vq vstat h2 h5 h9 h11)
inline uint64_t GetCollisionDBPlane(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint64_t a0, uint64_t t0, uint64_t sp, float f0, float f12, float f13, float f14, float f15, float f16, float f17, float f18, float f19, float f20, const a4::vec4 &io_vf0, a4::vec4 &io_vf4, a4::vec4 &io_vf5, a4::vec4 &io_vf6, a4::vec4 &io_vf7, a4::vec4 &io_vf8, a4::vec4 &io_vf10, a4::vec4 &io_vf14, a4::vec4 &io_vf15, a4::vec4 &io_vf16, a4::vec4 &io_vf17, a4::vec4 &io_vf18, a4::vec4 &io_vf29, a4::vec4 &io_vf30, a4::vec4 &io_vf31, a4::vec4 &io_vacc, float &io_vq, uint16_t &io_vstat, uint64_t &io_h2, uint64_t &io_h5, uint64_t &io_h9, uint64_t &io_h11, uint64_t io_h29, uint64_t *xo = nullptr)
{
    uint64_t v0 = 0;
    uint32_t v1 = 0;
    uint64_t a1 = 0;
    uint64_t a2 = 0;
    uint64_t a3 = 0;
    uint64_t t1 = 0;
    uint64_t t2 = 0;
    uint64_t t3 = 0;
    uint64_t s0 = 0;
    uint64_t s1 = 0;
    uint64_t s2 = 0;
    uint64_t s3 = 0;
    a4::vec4 vf0 = io_vf0;
    a4::vec4 vf4 = io_vf4;
    a4::vec4 vf5 = io_vf5;
    a4::vec4 vf6 = io_vf6;
    a4::vec4 vf7 = io_vf7;
    a4::vec4 vf8 = io_vf8;
    a4::vec4 vf10 = io_vf10;
    a4::vec4 vf14 = io_vf14;
    a4::vec4 vf15 = io_vf15;
    a4::vec4 vf16 = io_vf16;
    a4::vec4 vf17 = io_vf17;
    a4::vec4 vf18 = io_vf18;
    a4::vec4 vf29 = io_vf29;
    a4::vec4 vf30 = io_vf30;
    a4::vec4 vf31 = io_vf31;
    a4::vec4 vacc = io_vacc;
    float vq = io_vq;
    uint16_t vstat = io_vstat;
    uint64_t h2 = io_h2;
    uint64_t h5 = io_h5;
    uint64_t h9 = io_h9;
    uint64_t h11 = io_h11;
    uint64_t h29 = io_h29;
    bool a3xw0 = false;
    bool a3xw1 = false;
    bool a3xw4 = false;
    bool a3xw6 = false;
    bool a3xw7 = false;
    bool a3xw8 = false;
    bool a3xw9 = false;
    (sp = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)sp, 4294967168))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)255 << 16))));
    (s0 = (uint64_t)((uint64_t)a0 + (uint64_t)0ULL));
    (v0 = (uint64_t)(RC_U64X(v0) | (uint64_t)(uint16_t)65535));
    (a1 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)s0, 4))));
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)s0, 0))));
    (v1 = (uint32_t)((int32_t)((int32_t)SRL32((uint32_t)a1, 24))));
    (s3 = (uint64_t)(RC_U64X(a1) & RC_U64X(v0)));
    { (s1 = (uint64_t)(a1_sx64(v1) & (uint64_t)(uint16_t)127)); A3_CHARGE(); v0 = GetLayoutClass(ram, rt, a0, f12, f13, f14, f15, f16, f17, f18, f19, h2); }
    (a3 = (uint64_t)((uint64_t)v0 + (uint64_t)0ULL));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)s0, 4))));
    { const bool c = (int32_t)v0 >= 0; (v1 = (uint32_t)((int32_t)((int32_t)SLL32((uint32_t)s1, 1)))); if (c) goto L_11258d0; }
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)a3, 372))));
    (v1 = (uint32_t)((int32_t)((int32_t)ADD32(v1, (uint32_t)s1))));
    (a1 = (uint64_t)((int64_t)(int32_t)((int32_t)SLL32((uint32_t)s3, 4))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)a0, 36))));
    v1 = v1 << 5;
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v0, v1))));
    (s2 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v0, 48))));
    (v1 = (uint32_t)((int32_t)((int32_t)A1_R32B((uint32_t)s2, 8))));
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)s2, 20))));
    (v1 = (uint32_t)((int32_t)((int32_t)ADD32(v1, (uint32_t)s3))));
    (v0 = (uint64_t)(uint32_t)((uint8_t)A1_R8B(v1, 0)));
    { const bool c = (int64_t)v0 != 0; (s0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)a0, (uint32_t)a1)))); if (c) goto L_11259e8; }
    (a2 = (uint64_t)((uint64_t)a3 + (uint64_t)0ULL)); a3xw4 = true;
    (a3 = (uint64_t)((uint64_t)s1 + (uint64_t)0ULL));
    (a0 = (uint64_t)((uint64_t)sp + (uint64_t)0ULL));
    (a1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)sp, 16))));
    { (t0 = (uint64_t)((uint64_t)s3 + (uint64_t)0ULL)); a3xw6 = true; A3_CHARGE(); uint64_t a7c[9] = {(uint64_t)a1_bits(f0), 0u, 0u, 0u, 0u, 0u, 0u, 0u, 0u}; v0 = manager_sub_01114C70(ram, rt, a0, a1, a2, a3, t0, (uint32_t)sp, f12, f13, f14, f15, f16, f17, f18, f19, f20, vf8, vf10, vf14, vf15, vf16, vf17, vf29, vf30, vf31, vacc, h5, h9, h29, a7c); f0 = a1_flt((uint32_t)a7c[0]); f20 = a1_flt((uint32_t)a7c[1]); t3 = a7c[8]; a3xw0 = true; a3xw1 = true; a3xw9 = true; }
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)sp, 0))));
    vf29 = a4v::castsi128_ps(A4_R128(ADD32((uint32_t)v0, 0)));
    (v1 = (uint32_t)((int32_t)((int32_t)A1_R32B((uint32_t)sp, 4))));
    vf30 = a4v::castsi128_ps(A4_R128(ADD32(v1, 0)));
    { a4::vec4 res = a4v::sub(vf29, vf30); a4::q128 mask = a4v::set_epi32(-1, -1, -1, -1); vf31 = a4v::blend(vf31, res, a4v::castsi128_ps(mask)); }
    { a4::q128 mask = a4v::set_epi32(-1, -1, -1, -1); vf18 = a4v::blendv_ps(vf18, vf31, a4v::castsi128_ps(mask)); }
    { a4::q128 mask = a4v::set_epi32(-1, -1, -1, -1); vf29 = a4v::blendv_ps(vf29, vf30, a4v::castsi128_ps(mask)); }
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)sp, 8))));
    vf30 = a4v::castsi128_ps(A4_R128(ADD32((uint32_t)v0, 0)));
    { a4::vec4 res = a4v::sub(vf29, vf30); a4::q128 mask = a4v::set_epi32(-1, -1, -1, -1); vf31 = a4v::blend(vf31, res, a4v::castsi128_ps(mask)); }
    { a4::q128 mask = a4v::set_epi32(-1, -1, -1, -1); vf29 = a4v::blendv_ps(vf29, vf18, a4v::castsi128_ps(mask)); }
    { a4::q128 mask = a4v::set_epi32(-1, -1, -1, -1); vf30 = a4v::blendv_ps(vf30, vf31, a4v::castsi128_ps(mask)); }
    { a4::vec4 fs_yzx = a4v::shuffle_ps<0xC9>(vf29, vf29); a4::vec4 ft_zxy = a4v::shuffle_ps<0xD2>(vf30, vf30); a4::vec4 res = a4v::mulK<1, 1>(fs_yzx, ft_zxy); vacc = a4v::dest<1, 1, 1, 0>(vacc, res); }
    { a4::vec4 fs_yzx = a4v::shuffle_ps<0xC9>(vf30, vf30); a4::vec4 ft_zxy = a4v::shuffle_ps<0xD2>(vf29, vf29); a4::vec4 mul_res = a4v::mulK<1, 1>(fs_yzx, ft_zxy); a4::vec4 res = a4v::subK<1, 1>(vacc, mul_res); a4::q128 mask = a4v::set_epi32(0, -1, -1, -1); vf31 = a4v::blendv_ps(vf31, res, a4v::castsi128_ps(mask)); }
    { a4::vec4 res = a4v::subK<1, 1>(vf31, vf31); a4::q128 mask = a4v::set_epi32(-1, 0, 0, 0); vf31 = a4v::blend(vf31, res, a4v::castsi128_ps(mask)); }
    { a4::q128 mask = a4v::set_epi32(-1, -1, -1, -1); vf29 = a4v::blendv_ps(vf29, vf31, a4v::castsi128_ps(mask)); }
    { a4::vec4 res = a4v::mulK<1, 1>(vf29, vf29); a4::q128 mask = a4v::set_epi32(0, -1, -1, -1); vf7 = a4v::blend(vf7, res, a4v::castsi128_ps(mask)); }
    { a4::vec4 res = a4v::mulK<1, 1>(vf0, a4v::bcast<0>(vf7)); vacc = a4v::dest<0, 0, 0, 1>(vacc, res); }
    { a4::vec4 mul_res = a4v::mulK<1, 1>(vf0, a4v::bcast<1>(vf7)); a4::vec4 res = a4v::addK<1, 1>(vacc, mul_res); vacc = a4v::dest<0, 0, 0, 1>(vacc, res); }
    { a4::vec4 mul_res = a4v::mulK<1, 1>(vf0, a4v::bcast<2>(vf7)); a4::vec4 res = a4v::addK<1, 1>(vacc, mul_res); a4::q128 mask = a4v::set_epi32(-1, 0, 0, 0); vf7 = a4v::blendv_ps(vf7, res, a4v::castsi128_ps(mask)); }
    { float fs = a4v::cvtss_f32(a4v::shuffle_ps<0x03>(vf0, vf0)); float ft = a4v::cvtss_f32(a4v::shuffle_ps<0x03>(vf7, vf7)); vq = a4v::rsqrt(vstat, fs, ft); }
    { a4::vec4 res = a4v::mul(vf29, a4v::set1_ps(vq)); a4::q128 mask = a4v::set_epi32(0, -1, -1, -1); vf29 = a4v::blendv_ps(vf29, res, a4v::castsi128_ps(mask)); }
    { a4::q128 mask = a4v::set_epi32(-1, -1, -1, -1); vf31 = a4v::blendv_ps(vf31, vf29, a4v::castsi128_ps(mask)); }
    A4_W128(ADD32((uint32_t)s0, 0), a4v::castps_si128(vf31));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)sp, 0))));
    vf4 = a4v::castsi128_ps(A4_R128(ADD32((uint32_t)s0, 0)));
    vf5 = a4v::castsi128_ps(A4_R128(ADD32((uint32_t)v0, 0)));
    { a4::vec4 res = a4v::mul(vf4, vf5); a4::q128 mask = a4v::set_epi32(0, -1, -1, -1); vf5 = a4v::blend(vf5, res, a4v::castsi128_ps(mask)); }
    { a4::vec4 res = a4v::addK<1, 1>(vf5, a4v::bcast<1>(vf5)); a4::q128 mask = a4v::set_epi32(0, 0, 0, -1); vf5 = a4v::blendv_ps(vf5, res, a4v::castsi128_ps(mask)); }
    { a4::vec4 res = a4v::addK<1, 1>(vf5, a4v::bcast<2>(vf5)); a4::q128 mask = a4v::set_epi32(0, 0, 0, -1); vf5 = a4v::blendv_ps(vf5, res, a4v::castsi128_ps(mask)); }
    { a4::vec4 res = a4v::subK<1, 1>(vf0, a4v::bcast<3>(vf0)); a4::q128 mask = a4v::set_epi32(-1, -1, -1, -1); vf6 = a4v::blendv_ps(vf6, res, a4v::castsi128_ps(mask)); }
    { a4::vec4 res = a4v::subK<1, 1>(vf6, a4v::bcast<0>(vf5)); a4::q128 mask = a4v::set_epi32(-1, 0, 0, 0); vf4 = a4v::blendv_ps(vf4, res, a4v::castsi128_ps(mask)); }
    A4_W128(ADD32((uint32_t)s0, 0), a4v::castps_si128(vf4));
    (a0 = (uint64_t)((uint64_t)s2 + (uint64_t)0ULL));
    (a1 = (uint64_t)((uint64_t)s3 + (uint64_t)0ULL));
    goto L_11259e0;
L_11258d0:;
    (v1 = (uint32_t)((int32_t)((int32_t)A1_R32B((uint32_t)a3, 388))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)SLL32((uint32_t)s1, 3))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v0, (uint32_t)s1))));
    (a1 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)a3, 372))));
    (t2 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B(v1, 32)))); a3xw8 = true;
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)SLL32((uint32_t)v0, 4))));
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)SLL32((uint32_t)s3, 2))));
    (s2 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)a1, 88))));
    (t1 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)t2, 8)))); a3xw7 = true;
    (a1 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)s2, 8))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v0, (uint32_t)t1))));
    (a2 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)s2, 20)))); a3xw4 = true;
    (v1 = (uint32_t)((int32_t)((int32_t)A1_R32B((uint32_t)v0, 84))));
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)a0, v1))));
    (s1 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)a0, 0))));
    (a1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)a1, (uint32_t)s1))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)SLL32((uint32_t)s1, 4))));
    (v1 = (uint32_t)((uint8_t)A1_R8B((uint32_t)a1, 0)));
    { const bool c = v1 != 0; (s0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)a2, (uint32_t)v0)))); if (c) goto L_11259e8; }
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)a3, 340))));
    (v1 = (uint32_t)((int32_t)((int32_t)A1_R32B((uint32_t)t2, 24))));
    (a2 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)v0, 32)))); a3xw4 = true;
    (t3 = (uint64_t)((int64_t)(int32_t)((int32_t)SLL32((uint32_t)s1, 6)))); a3xw9 = true;
    (t0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)v0, 8)))); a3xw6 = true;
    (t3 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(v1, (uint32_t)t3)))); a3xw9 = true;
    (a3 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)a2, 36))));
    (a2 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)a2, 40)))); a3xw4 = true;
    (t2 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)t2, 20)))); a3xw8 = true;
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)sp, 32))));
    { (a1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)sp, 48)))); A3_CHARGE(); uint64_t a7c[7] = {0u, 0u, 0u, 0u, 0u, 0u, t3}; v0 = manager_sub_01116108(ram, rt, a0, a1, a2, a3, t0, t1, t2, t3, sp, f12, f13, f14, f15, f16, f17, f18, f19, vf8, vf10, vf14, vf15, vf16, vf17, vf29, vf30, vf31, vacc, h11, a7c); t3 = a7c[6]; a3xw9 = true; }
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)sp, 32))));
    vf29 = a4v::castsi128_ps(A4_R128(ADD32((uint32_t)v0, 0)));
    (v1 = (uint32_t)((int32_t)((int32_t)A1_R32B((uint32_t)sp, 36))));
    vf30 = a4v::castsi128_ps(A4_R128(ADD32(v1, 0)));
    { a4::vec4 res = a4v::sub(vf29, vf30); a4::q128 mask = a4v::set_epi32(-1, -1, -1, -1); vf31 = a4v::blend(vf31, res, a4v::castsi128_ps(mask)); }
    { a4::q128 mask = a4v::set_epi32(-1, -1, -1, -1); vf18 = a4v::blendv_ps(vf18, vf31, a4v::castsi128_ps(mask)); }
    { a4::q128 mask = a4v::set_epi32(-1, -1, -1, -1); vf29 = a4v::blendv_ps(vf29, vf30, a4v::castsi128_ps(mask)); }
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)sp, 40))));
    vf30 = a4v::castsi128_ps(A4_R128(ADD32((uint32_t)v0, 0)));
    { a4::vec4 res = a4v::sub(vf29, vf30); a4::q128 mask = a4v::set_epi32(-1, -1, -1, -1); vf31 = a4v::blend(vf31, res, a4v::castsi128_ps(mask)); }
    { a4::q128 mask = a4v::set_epi32(-1, -1, -1, -1); vf29 = a4v::blendv_ps(vf29, vf18, a4v::castsi128_ps(mask)); }
    { a4::q128 mask = a4v::set_epi32(-1, -1, -1, -1); vf30 = a4v::blendv_ps(vf30, vf31, a4v::castsi128_ps(mask)); }
    { a4::vec4 fs_yzx = a4v::shuffle_ps<0xC9>(vf29, vf29); a4::vec4 ft_zxy = a4v::shuffle_ps<0xD2>(vf30, vf30); a4::vec4 res = a4v::mulK<1, 1>(fs_yzx, ft_zxy); vacc = a4v::dest<1, 1, 1, 0>(vacc, res); }
    { a4::vec4 fs_yzx = a4v::shuffle_ps<0xC9>(vf30, vf30); a4::vec4 ft_zxy = a4v::shuffle_ps<0xD2>(vf29, vf29); a4::vec4 mul_res = a4v::mulK<1, 1>(fs_yzx, ft_zxy); a4::vec4 res = a4v::subK<1, 1>(vacc, mul_res); a4::q128 mask = a4v::set_epi32(0, -1, -1, -1); vf31 = a4v::blendv_ps(vf31, res, a4v::castsi128_ps(mask)); }
    { a4::vec4 res = a4v::subK<1, 1>(vf31, vf31); a4::q128 mask = a4v::set_epi32(-1, 0, 0, 0); vf31 = a4v::blend(vf31, res, a4v::castsi128_ps(mask)); }
    { a4::q128 mask = a4v::set_epi32(-1, -1, -1, -1); vf29 = a4v::blendv_ps(vf29, vf31, a4v::castsi128_ps(mask)); }
    { a4::vec4 res = a4v::mulK<1, 1>(vf29, vf29); a4::q128 mask = a4v::set_epi32(0, -1, -1, -1); vf7 = a4v::blend(vf7, res, a4v::castsi128_ps(mask)); }
    { a4::vec4 res = a4v::mulK<1, 1>(vf0, a4v::bcast<0>(vf7)); vacc = a4v::dest<0, 0, 0, 1>(vacc, res); }
    { a4::vec4 mul_res = a4v::mulK<1, 1>(vf0, a4v::bcast<1>(vf7)); a4::vec4 res = a4v::addK<1, 1>(vacc, mul_res); vacc = a4v::dest<0, 0, 0, 1>(vacc, res); }
    { a4::vec4 mul_res = a4v::mulK<1, 1>(vf0, a4v::bcast<2>(vf7)); a4::vec4 res = a4v::addK<1, 1>(vacc, mul_res); a4::q128 mask = a4v::set_epi32(-1, 0, 0, 0); vf7 = a4v::blendv_ps(vf7, res, a4v::castsi128_ps(mask)); }
    { float fs = a4v::cvtss_f32(a4v::shuffle_ps<0x03>(vf0, vf0)); float ft = a4v::cvtss_f32(a4v::shuffle_ps<0x03>(vf7, vf7)); vq = a4v::rsqrt(vstat, fs, ft); }
    { a4::vec4 res = a4v::mul(vf29, a4v::set1_ps(vq)); a4::q128 mask = a4v::set_epi32(0, -1, -1, -1); vf29 = a4v::blendv_ps(vf29, res, a4v::castsi128_ps(mask)); }
    { a4::q128 mask = a4v::set_epi32(-1, -1, -1, -1); vf31 = a4v::blendv_ps(vf31, vf29, a4v::castsi128_ps(mask)); }
    A4_W128(ADD32((uint32_t)s0, 0), a4v::castps_si128(vf31));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)sp, 32))));
    vf4 = a4v::castsi128_ps(A4_R128(ADD32((uint32_t)s0, 0)));
    vf5 = a4v::castsi128_ps(A4_R128(ADD32((uint32_t)v0, 0)));
    { a4::vec4 res = a4v::mul(vf4, vf5); a4::q128 mask = a4v::set_epi32(0, -1, -1, -1); vf5 = a4v::blend(vf5, res, a4v::castsi128_ps(mask)); }
    { a4::vec4 res = a4v::addK<1, 1>(vf5, a4v::bcast<1>(vf5)); a4::q128 mask = a4v::set_epi32(0, 0, 0, -1); vf5 = a4v::blendv_ps(vf5, res, a4v::castsi128_ps(mask)); }
    { a4::vec4 res = a4v::addK<1, 1>(vf5, a4v::bcast<2>(vf5)); a4::q128 mask = a4v::set_epi32(0, 0, 0, -1); vf5 = a4v::blendv_ps(vf5, res, a4v::castsi128_ps(mask)); }
    { a4::vec4 res = a4v::subK<1, 1>(vf0, a4v::bcast<3>(vf0)); a4::q128 mask = a4v::set_epi32(-1, -1, -1, -1); vf6 = a4v::blendv_ps(vf6, res, a4v::castsi128_ps(mask)); }
    { a4::vec4 res = a4v::subK<1, 1>(vf6, a4v::bcast<0>(vf5)); a4::q128 mask = a4v::set_epi32(-1, 0, 0, 0); vf4 = a4v::blendv_ps(vf4, res, a4v::castsi128_ps(mask)); }
    A4_W128(ADD32((uint32_t)s0, 0), a4v::castps_si128(vf4));
    (a0 = (uint64_t)((uint64_t)s2 + (uint64_t)0ULL));
    (a1 = (uint64_t)((uint64_t)s1 + (uint64_t)0ULL));
L_11259e0:;
    { A3_CHARGE(); uint64_t a7c[6] = {0u, 0u, 0u, 0u, 0u, 0u}; v0 = EntryModifiedManager_x8(ram, rt, a0, (uint32_t)a1, a7c); a0 = a7c[0]; a2 = a7c[1]; a3 = a7c[2]; t0 = a7c[3]; t1 = a7c[4]; t2 = a7c[5]; a3xw4 = true; a3xw6 = true; a3xw7 = true; a3xw8 = true; }
L_11259e8:;
    (v0 = (uint64_t)((uint64_t)s0 + (uint64_t)0ULL));
    { (sp = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)sp, 128)))); if (xo) { if (a3xw0) xo[0] = a1_bits(f0); if (a3xw1) xo[1] = a1_bits(f20); xo[2] = a0; xo[3] = a1; if (a3xw4) xo[4] = a2; xo[5] = a3; if (a3xw6) xo[6] = t0; if (a3xw7) xo[7] = t1; if (a3xw8) xo[8] = t2; if (a3xw9) xo[9] = t3; } io_vf4 = vf4; io_vf5 = vf5; io_vf6 = vf6; io_vf7 = vf7; io_vf8 = vf8; io_vf10 = vf10; io_vf14 = vf14; io_vf15 = vf15; io_vf16 = vf16; io_vf17 = vf17; io_vf18 = vf18; io_vf29 = vf29; io_vf30 = vf30; io_vf31 = vf31; io_vacc = vacc; io_vq = vq; io_vstat = vstat; io_h2 = h2; io_h5 = h5; io_h9 = h9; io_h11 = h11; return v0; }
    __builtin_unreachable();
}

// GetCollisionEdgePointerVertexPoint (0x1127b28): typed by tools/typed_lift2.py (round A6; guest frame calls, extra outputs xo[0]=f0 xo[1]=f20 xo[2]=a1 xo[3]=a2 xo[4]=a3 xo[5]=t0 xo[6]=t1 xo[7]=t2 xo[8]=t3; VU0 in vf10 vf29 vf31 vacc h9 h11 h29, out vf8 vf10 vf14 vf15 vf16 vf17 vf29 vf30 vf31 vacc h2 h5 h9 h11)
inline uint64_t GetCollisionEdgePointerVertexPoint(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint64_t a0, uint64_t a1, uint64_t sp, float f0, float f12, float f13, float f14, float f15, float f16, float f17, float f18, float f19, float f20, a4::vec4 &io_vf8, a4::vec4 &io_vf10, a4::vec4 &io_vf14, a4::vec4 &io_vf15, a4::vec4 &io_vf16, a4::vec4 &io_vf17, a4::vec4 &io_vf29, a4::vec4 &io_vf30, a4::vec4 &io_vf31, a4::vec4 &io_vacc, uint64_t &io_h2, uint64_t &io_h5, uint64_t &io_h9, uint64_t &io_h11, uint64_t io_h29, uint64_t *xo = nullptr)
{
    uint64_t v0 = 0;
    uint64_t v1 = 0;
    uint64_t a2 = 0;
    uint64_t a3 = 0;
    uint64_t t0 = 0;
    uint64_t t1 = 0;
    uint64_t t2 = 0;
    uint64_t t3 = 0;
    uint32_t t4 = 0;
    uint32_t t5 = 0;
    uint64_t s0 = 0;
    uint64_t s1 = 0;
    uint64_t s2 = 0;
    uint64_t s3 = 0;
    uint32_t s4 = 0;
    a4::vec4 vf8 = io_vf8;
    a4::vec4 vf10 = io_vf10;
    a4::vec4 vf14 = io_vf14;
    a4::vec4 vf15 = io_vf15;
    a4::vec4 vf16 = io_vf16;
    a4::vec4 vf17 = io_vf17;
    a4::vec4 vf29 = io_vf29;
    a4::vec4 vf30 = io_vf30;
    a4::vec4 vf31 = io_vf31;
    a4::vec4 vacc = io_vacc;
    uint64_t h2 = io_h2;
    uint64_t h5 = io_h5;
    uint64_t h9 = io_h9;
    uint64_t h11 = io_h11;
    uint64_t h29 = io_h29;
    bool a3xw0 = false;
    bool a3xw1 = false;
    (sp = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)sp, 4294967232))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)255 << 16))));
    (v0 = (uint64_t)(RC_U64X(v0) | (uint64_t)(uint16_t)65535));
    (s2 = (uint64_t)((uint64_t)a1 + (uint64_t)0ULL));
    (s3 = (uint64_t)((uint64_t)a0 + (uint64_t)0ULL));
    (s1 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)s2, 4))));
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)s2, 0))));
    (s0 = (uint64_t)((int64_t)(int32_t)((int32_t)SRL32((uint32_t)s1, 24))));
    (s1 = (uint64_t)(RC_U64X(s1) & RC_U64X(v0)));
    (s0 = (uint64_t)(RC_U64X(s0) & (uint64_t)(uint16_t)127));
    { (s4 = (uint32_t)((int32_t)((int32_t)A1_R32B((uint32_t)s2, 8)))); A3_CHARGE(); v0 = GetLayoutClass(ram, rt, a0, f12, f13, f14, f15, f16, f17, f18, f19, h2); }
    (t5 = (uint32_t)((int32_t)((int32_t)SLL32((uint32_t)s1, 2))));
    (v1 = (uint64_t)((uint64_t)v0 + (uint64_t)0ULL));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)SLL32((uint32_t)s0, 3))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v0, (uint32_t)s0))));
    (t1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v1, 340))));
    (t3 = (uint64_t)((int64_t)(int32_t)((int32_t)SLL32((uint32_t)v0, 4))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)s2, 4))));
    (a0 = (uint64_t)((uint64_t)sp + (uint64_t)0ULL));
    (a1 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL));
    (a2 = (uint64_t)((uint64_t)v1 + (uint64_t)0ULL));
    (a3 = (uint64_t)((uint64_t)s0 + (uint64_t)0ULL));
    { const bool c = (int32_t)v0 >= 0; (t0 = (uint64_t)((uint64_t)s1 + (uint64_t)0ULL)); if (c) goto L_1127bb8; }
    { A3_CHARGE(); uint64_t a7c[9] = {(uint64_t)a1_bits(f0), 0u, 0u, 0u, 0u, t0, 0u, 0u, 0u}; v0 = manager_sub_01114C70(ram, rt, a0, a1, a2, a3, t0, (uint32_t)sp, f12, f13, f14, f15, f16, f17, f18, f19, f20, vf8, vf10, vf14, vf15, vf16, vf17, vf29, vf30, vf31, vacc, h5, h9, h29, a7c); f0 = a1_flt((uint32_t)a7c[0]); f20 = a1_flt((uint32_t)a7c[1]); a1 = a7c[2]; a2 = a7c[3]; a3 = a7c[4]; t0 = a7c[5]; t1 = a7c[6]; t2 = a7c[7]; t3 = a7c[8]; a3xw0 = true; a3xw1 = true; }
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)SLL32(s4, 2))));
    goto L_1127bfc;
L_1127bb8:;
    (a3 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)v1, 388))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)t1, 32))));
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)a3, 32))));
    (t0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)t1, 8))));
    (a2 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v0, 40))));
    (t1 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)v1, 8))));
    (a3 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)v0, 36))));
    (t3 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)t3, (uint32_t)t1))));
    (t4 = (uint32_t)((int32_t)((int32_t)A1_R32B((uint32_t)v1, 24))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)t3, 84))));
    (t2 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)v1, 20))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(t5, (uint32_t)v0))));
    (t3 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)v0, 0))));
    (t3 = (uint64_t)((int64_t)(int32_t)((int32_t)SLL32((uint32_t)t3, 6))));
    { (t3 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(t4, (uint32_t)t3)))); A3_CHARGE(); uint64_t a7c[7] = {a1, a2, a3, t0, t1, t2, t3}; v0 = manager_sub_01116108(ram, rt, a0, a1, a2, a3, t0, t1, t2, t3, sp, f12, f13, f14, f15, f16, f17, f18, f19, vf8, vf10, vf14, vf15, vf16, vf17, vf29, vf30, vf31, vacc, h11, a7c); a1 = a7c[0]; a2 = a7c[1]; a3 = a7c[2]; t0 = a7c[3]; t1 = a7c[4]; t2 = a7c[5]; t3 = a7c[6]; }
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)SLL32(s4, 2))));
L_1127bfc:;
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)sp, (uint32_t)a0))));
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)v0, 0))));
    A1_W32B((uint32_t)s3, 0, (uint32_t)v1);
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)281 << 16))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v0, (uint32_t)a0))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)v0, 21656))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)SLL32((uint32_t)v0, 2))));
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)sp, (uint32_t)v0))));
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)v1, 0))));
    A1_W32B((uint32_t)s3, 4, (uint32_t)a0);
    { (sp = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)sp, 64)))); if (xo) { if (a3xw0) xo[0] = a1_bits(f0); if (a3xw1) xo[1] = a1_bits(f20); xo[2] = a1; xo[3] = a2; xo[4] = a3; xo[5] = t0; xo[6] = t1; xo[7] = t2; xo[8] = t3; } io_vf8 = vf8; io_vf10 = vf10; io_vf14 = vf14; io_vf15 = vf15; io_vf16 = vf16; io_vf17 = vf17; io_vf29 = vf29; io_vf30 = vf30; io_vf31 = vf31; io_vacc = vacc; io_h2 = h2; io_h5 = h5; io_h9 = h9; io_h11 = h11; return v0; }
    __builtin_unreachable();
}

// GetCollisionEdgePointerVertexPoint3 (0x1127c48): typed by tools/typed_lift2.py (round A6; guest frame calls, extra outputs xo[0]=f0 xo[1]=f20 xo[2]=a1 xo[3]=a2 xo[4]=a3 xo[5]=t0 xo[6]=t1 xo[7]=t2 xo[8]=t3; VU0 in vf10 vf29 vf31 vacc h9 h11 h29, out vf8 vf10 vf14 vf15 vf16 vf17 vf29 vf30 vf31 vacc h2 h5 h9 h11)
inline uint64_t GetCollisionEdgePointerVertexPoint3(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint64_t a0, uint64_t a1, uint64_t sp, float f0, float f12, float f13, float f14, float f15, float f16, float f17, float f18, float f19, float f20, a4::vec4 &io_vf8, a4::vec4 &io_vf10, a4::vec4 &io_vf14, a4::vec4 &io_vf15, a4::vec4 &io_vf16, a4::vec4 &io_vf17, a4::vec4 &io_vf29, a4::vec4 &io_vf30, a4::vec4 &io_vf31, a4::vec4 &io_vacc, uint64_t &io_h2, uint64_t &io_h5, uint64_t &io_h9, uint64_t &io_h11, uint64_t io_h29, uint64_t *xo = nullptr)
{
    uint64_t v0 = 0;
    uint64_t v1 = 0;
    uint64_t a2 = 0;
    uint64_t a3 = 0;
    uint64_t t0 = 0;
    uint64_t t1 = 0;
    uint64_t t2 = 0;
    uint64_t t3 = 0;
    uint32_t t4 = 0;
    uint32_t t5 = 0;
    uint64_t s0 = 0;
    uint64_t s1 = 0;
    uint64_t s2 = 0;
    uint64_t s3 = 0;
    uint32_t s4 = 0;
    a4::vec4 vf8 = io_vf8;
    a4::vec4 vf10 = io_vf10;
    a4::vec4 vf14 = io_vf14;
    a4::vec4 vf15 = io_vf15;
    a4::vec4 vf16 = io_vf16;
    a4::vec4 vf17 = io_vf17;
    a4::vec4 vf29 = io_vf29;
    a4::vec4 vf30 = io_vf30;
    a4::vec4 vf31 = io_vf31;
    a4::vec4 vacc = io_vacc;
    uint64_t h2 = io_h2;
    uint64_t h5 = io_h5;
    uint64_t h9 = io_h9;
    uint64_t h11 = io_h11;
    uint64_t h29 = io_h29;
    bool a3xw0 = false;
    bool a3xw1 = false;
    (sp = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)sp, 4294967232))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)255 << 16))));
    (v0 = (uint64_t)(RC_U64X(v0) | (uint64_t)(uint16_t)65535));
    (s2 = (uint64_t)((uint64_t)a1 + (uint64_t)0ULL));
    (s3 = (uint64_t)((uint64_t)a0 + (uint64_t)0ULL));
    (s1 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)s2, 4))));
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)s2, 0))));
    (s0 = (uint64_t)((int64_t)(int32_t)((int32_t)SRL32((uint32_t)s1, 24))));
    (s1 = (uint64_t)(RC_U64X(s1) & RC_U64X(v0)));
    (s0 = (uint64_t)(RC_U64X(s0) & (uint64_t)(uint16_t)127));
    { (s4 = (uint32_t)((int32_t)((int32_t)A1_R32B((uint32_t)s2, 8)))); A3_CHARGE(); v0 = GetLayoutClass(ram, rt, a0, f12, f13, f14, f15, f16, f17, f18, f19, h2); }
    (t5 = (uint32_t)((int32_t)((int32_t)SLL32((uint32_t)s1, 2))));
    (v1 = (uint64_t)((uint64_t)v0 + (uint64_t)0ULL));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)SLL32((uint32_t)s0, 3))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v0, (uint32_t)s0))));
    (t1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v1, 340))));
    (t3 = (uint64_t)((int64_t)(int32_t)((int32_t)SLL32((uint32_t)v0, 4))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)s2, 4))));
    (a0 = (uint64_t)((uint64_t)sp + (uint64_t)0ULL));
    (a1 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL));
    (a2 = (uint64_t)((uint64_t)v1 + (uint64_t)0ULL));
    (a3 = (uint64_t)((uint64_t)s0 + (uint64_t)0ULL));
    { const bool c = (int32_t)v0 >= 0; (t0 = (uint64_t)((uint64_t)s1 + (uint64_t)0ULL)); if (c) goto L_1127cd8; }
    { A3_CHARGE(); uint64_t a7c[9] = {(uint64_t)a1_bits(f0), 0u, 0u, 0u, 0u, t0, 0u, 0u, 0u}; v0 = manager_sub_01114C70(ram, rt, a0, a1, a2, a3, t0, (uint32_t)sp, f12, f13, f14, f15, f16, f17, f18, f19, f20, vf8, vf10, vf14, vf15, vf16, vf17, vf29, vf30, vf31, vacc, h5, h9, h29, a7c); f0 = a1_flt((uint32_t)a7c[0]); f20 = a1_flt((uint32_t)a7c[1]); a2 = a7c[3]; a3 = a7c[4]; t0 = a7c[5]; t1 = a7c[6]; t2 = a7c[7]; t3 = a7c[8]; a3xw0 = true; a3xw1 = true; }
    (a1 = (uint64_t)((int64_t)(int32_t)((int32_t)SLL32(s4, 2))));
    goto L_1127d1c;
L_1127cd8:;
    (a3 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)v1, 388))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)t1, 32))));
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)a3, 32))));
    (t0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)t1, 8))));
    (a2 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v0, 40))));
    (t1 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)v1, 8))));
    (a3 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)v0, 36))));
    (t3 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)t3, (uint32_t)t1))));
    (t4 = (uint32_t)((int32_t)((int32_t)A1_R32B((uint32_t)v1, 24))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)t3, 84))));
    (t2 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)v1, 20))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(t5, (uint32_t)v0))));
    (t3 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)v0, 0))));
    (t3 = (uint64_t)((int64_t)(int32_t)((int32_t)SLL32((uint32_t)t3, 6))));
    { (t3 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(t4, (uint32_t)t3)))); A3_CHARGE(); uint64_t a7c[7] = {0u, a2, a3, t0, t1, t2, t3}; v0 = manager_sub_01116108(ram, rt, a0, a1, a2, a3, t0, t1, t2, t3, sp, f12, f13, f14, f15, f16, f17, f18, f19, vf8, vf10, vf14, vf15, vf16, vf17, vf29, vf30, vf31, vacc, h11, a7c); a2 = a7c[1]; a3 = a7c[2]; t0 = a7c[3]; t1 = a7c[4]; t2 = a7c[5]; t3 = a7c[6]; }
    (a1 = (uint64_t)((int64_t)(int32_t)((int32_t)SLL32(s4, 2))));
L_1127d1c:;
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)sp, (uint32_t)a1))));
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)v0, 0))));
    A1_W32B((uint32_t)s3, 0, (uint32_t)v1);
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)281 << 16))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v0, (uint32_t)a1))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)v0, 21656))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)SLL32((uint32_t)v0, 2))));
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)sp, (uint32_t)v0))));
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)v1, 0))));
    A1_W32B((uint32_t)s3, 4, (uint32_t)a0);
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)281 << 16))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v0, (uint32_t)a1))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)v0, 21672))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)SLL32((uint32_t)v0, 2))));
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)sp, (uint32_t)v0))));
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)v1, 0))));
    A1_W32B((uint32_t)s3, 8, (uint32_t)a0);
    { (sp = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)sp, 64)))); if (xo) { if (a3xw0) xo[0] = a1_bits(f0); if (a3xw1) xo[1] = a1_bits(f20); xo[2] = a1; xo[3] = a2; xo[4] = a3; xo[5] = t0; xo[6] = t1; xo[7] = t2; xo[8] = t3; } io_vf8 = vf8; io_vf10 = vf10; io_vf14 = vf14; io_vf15 = vf15; io_vf16 = vf16; io_vf17 = vf17; io_vf29 = vf29; io_vf30 = vf30; io_vf31 = vf31; io_vacc = vacc; io_h2 = h2; io_h5 = h5; io_h9 = h9; io_h11 = h11; return v0; }
    __builtin_unreachable();
}

// GetCollisionDBEdgeVertexPoint (0x1127e68): typed by tools/typed_lift2.py (round A6; guest frame calls, extra outputs xo[0]=f0 xo[1]=f20 xo[2]=a2 xo[3]=a3 xo[4]=t0 xo[5]=t1 xo[6]=t2 xo[7]=t3; VU0 in vf10 vf29 vf31 vacc h9 h11 h29, out vf8 vf10 vf14 vf15 vf16 vf17 vf29 vf30 vf31 vacc h2 h5 h9 h11)
inline uint64_t GetCollisionDBEdgeVertexPoint(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint64_t a0, uint64_t a1, uint64_t a2, uint64_t sp, float f0, float f12, float f13, float f14, float f15, float f16, float f17, float f18, float f19, float f20, a4::vec4 &io_vf8, a4::vec4 &io_vf10, a4::vec4 &io_vf14, a4::vec4 &io_vf15, a4::vec4 &io_vf16, a4::vec4 &io_vf17, a4::vec4 &io_vf29, a4::vec4 &io_vf30, a4::vec4 &io_vf31, a4::vec4 &io_vacc, uint64_t &io_h2, uint64_t &io_h5, uint64_t &io_h9, uint64_t &io_h11, uint64_t io_h29, uint64_t *xo = nullptr)
{
    uint64_t v0 = 0;
    uint64_t v1 = 0;
    uint64_t a3 = 0;
    uint64_t t0 = 0;
    uint64_t t1 = 0;
    uint64_t t2 = 0;
    uint64_t t3 = 0;
    uint32_t t4 = 0;
    uint32_t t5 = 0;
    uint64_t s0 = 0;
    uint64_t s1 = 0;
    uint64_t s2 = 0;
    uint64_t s3 = 0;
    uint64_t s4 = 0;
    a4::vec4 vf8 = io_vf8;
    a4::vec4 vf10 = io_vf10;
    a4::vec4 vf14 = io_vf14;
    a4::vec4 vf15 = io_vf15;
    a4::vec4 vf16 = io_vf16;
    a4::vec4 vf17 = io_vf17;
    a4::vec4 vf29 = io_vf29;
    a4::vec4 vf30 = io_vf30;
    a4::vec4 vf31 = io_vf31;
    a4::vec4 vacc = io_vacc;
    uint64_t h2 = io_h2;
    uint64_t h5 = io_h5;
    uint64_t h9 = io_h9;
    uint64_t h11 = io_h11;
    uint64_t h29 = io_h29;
    bool a3xw0 = false;
    bool a3xw1 = false;
    (sp = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)sp, 4294967232))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)255 << 16))));
    (v0 = (uint64_t)(RC_U64X(v0) | (uint64_t)(uint16_t)65535));
    (s2 = (uint64_t)((uint64_t)a1 + (uint64_t)0ULL));
    (s3 = (uint64_t)((uint64_t)a0 + (uint64_t)0ULL));
    (s4 = (uint64_t)((uint64_t)a2 + (uint64_t)0ULL));
    (s1 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)s2, 4))));
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)s2, 0))));
    (s0 = (uint64_t)((int64_t)(int32_t)((int32_t)SRL32((uint32_t)s1, 24))));
    (s1 = (uint64_t)(RC_U64X(s1) & RC_U64X(v0)));
    { (s0 = (uint64_t)(RC_U64X(s0) & (uint64_t)(uint16_t)127)); A3_CHARGE(); v0 = GetLayoutClass(ram, rt, a0, f12, f13, f14, f15, f16, f17, f18, f19, h2); }
    (v1 = (uint64_t)((uint64_t)v0 + (uint64_t)0ULL));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)SLL32((uint32_t)s0, 3))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v0, (uint32_t)s0))));
    (t5 = (uint32_t)((int32_t)((int32_t)SLL32((uint32_t)s1, 2))));
    (t3 = (uint64_t)((int64_t)(int32_t)((int32_t)SLL32((uint32_t)v0, 4))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)s2, 4))));
    (t1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v1, 340))));
    (a0 = (uint64_t)((uint64_t)sp + (uint64_t)0ULL));
    (a1 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL));
    (a2 = (uint64_t)((uint64_t)v1 + (uint64_t)0ULL));
    (a3 = (uint64_t)((uint64_t)s0 + (uint64_t)0ULL));
    { const bool c = (int32_t)v0 >= 0; (t0 = (uint64_t)((uint64_t)s1 + (uint64_t)0ULL)); if (c) goto L_1127ef8; }
    { A3_CHARGE(); uint64_t a7c[9] = {(uint64_t)a1_bits(f0), 0u, 0u, 0u, 0u, t0, 0u, 0u, 0u}; v0 = manager_sub_01114C70(ram, rt, a0, a1, a2, a3, t0, (uint32_t)sp, f12, f13, f14, f15, f16, f17, f18, f19, f20, vf8, vf10, vf14, vf15, vf16, vf17, vf29, vf30, vf31, vacc, h5, h9, h29, a7c); f0 = a1_flt((uint32_t)a7c[0]); f20 = a1_flt((uint32_t)a7c[1]); a2 = a7c[3]; a3 = a7c[4]; t0 = a7c[5]; t1 = a7c[6]; t2 = a7c[7]; t3 = a7c[8]; a3xw0 = true; a3xw1 = true; }
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)SLL32((uint32_t)s4, 2))));
    goto L_1127f3c;
L_1127ef8:;
    (a3 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)v1, 388))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)t1, 32))));
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)a3, 32))));
    (t0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)t1, 8))));
    (a2 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v0, 40))));
    (t1 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)v1, 8))));
    (a3 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)v0, 36))));
    (t3 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)t3, (uint32_t)t1))));
    (t4 = (uint32_t)((int32_t)((int32_t)A1_R32B((uint32_t)v1, 24))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)t3, 84))));
    (t2 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)v1, 20))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(t5, (uint32_t)v0))));
    (t3 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)v0, 0))));
    (t3 = (uint64_t)((int64_t)(int32_t)((int32_t)SLL32((uint32_t)t3, 6))));
    { (t3 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(t4, (uint32_t)t3)))); A3_CHARGE(); uint64_t a7c[7] = {0u, a2, a3, t0, t1, t2, t3}; v0 = manager_sub_01116108(ram, rt, a0, a1, a2, a3, t0, t1, t2, t3, sp, f12, f13, f14, f15, f16, f17, f18, f19, vf8, vf10, vf14, vf15, vf16, vf17, vf29, vf30, vf31, vacc, h11, a7c); a2 = a7c[1]; a3 = a7c[2]; t0 = a7c[3]; t1 = a7c[4]; t2 = a7c[5]; t3 = a7c[6]; }
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)SLL32((uint32_t)s4, 2))));
L_1127f3c:;
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)sp, (uint32_t)a0))));
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)v0, 0))));
    A1_W32B((uint32_t)s3, 0, (uint32_t)v1);
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)281 << 16))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v0, (uint32_t)a0))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)v0, 21656))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)SLL32((uint32_t)v0, 2))));
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)sp, (uint32_t)v0))));
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)v1, 0))));
    A1_W32B((uint32_t)s3, 4, (uint32_t)a0);
    { (sp = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)sp, 64)))); if (xo) { if (a3xw0) xo[0] = a1_bits(f0); if (a3xw1) xo[1] = a1_bits(f20); xo[2] = a2; xo[3] = a3; xo[4] = t0; xo[5] = t1; xo[6] = t2; xo[7] = t3; } io_vf8 = vf8; io_vf10 = vf10; io_vf14 = vf14; io_vf15 = vf15; io_vf16 = vf16; io_vf17 = vf17; io_vf29 = vf29; io_vf30 = vf30; io_vf31 = vf31; io_vacc = vacc; io_h2 = h2; io_h5 = h5; io_h9 = h9; io_h11 = h11; return v0; }
    __builtin_unreachable();
}

// GetCollisionDBEdgeVertexPoint3 (0x1127f88): typed by tools/typed_lift2.py (round A6; guest frame calls; VU0 in vf10 vf29 vf31 vacc h9 h11 h29, out vf8 vf10 vf14 vf15 vf16 vf17 vf29 vf30 vf31 vacc h2 h5 h9 h11)
inline uint64_t GetCollisionDBEdgeVertexPoint3(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint64_t a0, uint64_t a1, uint64_t a2, uint64_t sp, float f12, float f13, float f14, float f15, float f16, float f17, float f18, float f19, float f20, a4::vec4 &io_vf8, a4::vec4 &io_vf10, a4::vec4 &io_vf14, a4::vec4 &io_vf15, a4::vec4 &io_vf16, a4::vec4 &io_vf17, a4::vec4 &io_vf29, a4::vec4 &io_vf30, a4::vec4 &io_vf31, a4::vec4 &io_vacc, uint64_t &io_h2, uint64_t &io_h5, uint64_t &io_h9, uint64_t &io_h11, uint64_t io_h29)
{
    uint64_t v0 = 0;
    uint64_t v1 = 0;
    uint64_t a3 = 0;
    uint64_t t0 = 0;
    uint32_t t1 = 0;
    uint32_t t2 = 0;
    uint32_t t3 = 0;
    uint32_t t4 = 0;
    uint32_t t5 = 0;
    uint64_t s0 = 0;
    uint64_t s1 = 0;
    uint64_t s2 = 0;
    uint64_t s3 = 0;
    uint64_t s4 = 0;
    a4::vec4 vf8 = io_vf8;
    a4::vec4 vf10 = io_vf10;
    a4::vec4 vf14 = io_vf14;
    a4::vec4 vf15 = io_vf15;
    a4::vec4 vf16 = io_vf16;
    a4::vec4 vf17 = io_vf17;
    a4::vec4 vf29 = io_vf29;
    a4::vec4 vf30 = io_vf30;
    a4::vec4 vf31 = io_vf31;
    a4::vec4 vacc = io_vacc;
    uint64_t h2 = io_h2;
    uint64_t h5 = io_h5;
    uint64_t h9 = io_h9;
    uint64_t h11 = io_h11;
    uint64_t h29 = io_h29;
    (sp = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)sp, 4294967232))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)255 << 16))));
    (v0 = (uint64_t)(RC_U64X(v0) | (uint64_t)(uint16_t)65535));
    (s2 = (uint64_t)((uint64_t)a1 + (uint64_t)0ULL));
    (s3 = (uint64_t)((uint64_t)a0 + (uint64_t)0ULL));
    (s4 = (uint64_t)((uint64_t)a2 + (uint64_t)0ULL));
    (s1 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)s2, 4))));
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)s2, 0))));
    (s0 = (uint64_t)((int64_t)(int32_t)((int32_t)SRL32((uint32_t)s1, 24))));
    (s1 = (uint64_t)(RC_U64X(s1) & RC_U64X(v0)));
    { (s0 = (uint64_t)(RC_U64X(s0) & (uint64_t)(uint16_t)127)); A3_CHARGE(); v0 = GetLayoutClass(ram, rt, a0, f12, f13, f14, f15, f16, f17, f18, f19, h2); }
    (v1 = (uint64_t)((uint64_t)v0 + (uint64_t)0ULL));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)SLL32((uint32_t)s0, 3))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v0, (uint32_t)s0))));
    (t5 = (uint32_t)((int32_t)((int32_t)SLL32((uint32_t)s1, 2))));
    (t3 = (uint32_t)((int32_t)((int32_t)SLL32((uint32_t)v0, 4))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)s2, 4))));
    (t1 = (uint32_t)((int32_t)((int32_t)ADD32((uint32_t)v1, 340))));
    (a0 = (uint64_t)((uint64_t)sp + (uint64_t)0ULL));
    (a1 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL));
    (a2 = (uint64_t)((uint64_t)v1 + (uint64_t)0ULL));
    (a3 = (uint64_t)((uint64_t)s0 + (uint64_t)0ULL));
    { const bool c = (int32_t)v0 >= 0; (t0 = (uint64_t)((uint64_t)s1 + (uint64_t)0ULL)); if (c) goto L_1128018; }
    { A3_CHARGE(); v0 = manager_sub_01114C70(ram, rt, a0, a1, a2, a3, t0, (uint32_t)sp, f12, f13, f14, f15, f16, f17, f18, f19, f20, vf8, vf10, vf14, vf15, vf16, vf17, vf29, vf30, vf31, vacc, h5, h9, h29); }
    (a1 = (uint64_t)((int64_t)(int32_t)((int32_t)SLL32((uint32_t)s4, 2))));
    goto L_112805c;
L_1128018:;
    (a3 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)v1, 388))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B(t1, 32))));
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)a3, 32))));
    (t0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B(t1, 8))));
    (a2 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v0, 40))));
    (t1 = (uint32_t)((int32_t)((int32_t)A1_R32B((uint32_t)v1, 8))));
    (a3 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)v0, 36))));
    t3 = t3 + t1;
    (t4 = (uint32_t)((int32_t)((int32_t)A1_R32B((uint32_t)v1, 24))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B(t3, 84))));
    (t2 = (uint32_t)((int32_t)((int32_t)A1_R32B((uint32_t)v1, 20))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(t5, (uint32_t)v0))));
    (t3 = (uint32_t)((int32_t)((int32_t)A1_R32B((uint32_t)v0, 0))));
    t3 = t3 << 6;
    { t3 = t4 + t3; A3_CHARGE(); v0 = manager_sub_01116108(ram, rt, a0, a1, a2, a3, t0, a1_sx64(t1), a1_sx64(t2), a1_sx64(t3), sp, f12, f13, f14, f15, f16, f17, f18, f19, vf8, vf10, vf14, vf15, vf16, vf17, vf29, vf30, vf31, vacc, h11); }
    (a1 = (uint64_t)((int64_t)(int32_t)((int32_t)SLL32((uint32_t)s4, 2))));
L_112805c:;
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)sp, (uint32_t)a1))));
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)v0, 0))));
    A1_W32B((uint32_t)s3, 0, (uint32_t)v1);
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)281 << 16))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v0, (uint32_t)a1))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)v0, 21656))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)SLL32((uint32_t)v0, 2))));
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)sp, (uint32_t)v0))));
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)v1, 0))));
    A1_W32B((uint32_t)s3, 4, (uint32_t)a0);
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)281 << 16))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v0, (uint32_t)a1))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)v0, 21672))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)SLL32((uint32_t)v0, 2))));
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)sp, (uint32_t)v0))));
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)v1, 0))));
    A1_W32B((uint32_t)s3, 8, (uint32_t)a0);
    { (sp = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)sp, 64)))); io_vf8 = vf8; io_vf10 = vf10; io_vf14 = vf14; io_vf15 = vf15; io_vf16 = vf16; io_vf17 = vf17; io_vf29 = vf29; io_vf30 = vf30; io_vf31 = vf31; io_vacc = vacc; io_h2 = h2; io_h5 = h5; io_h9 = h9; io_h11 = h11; return v0; }
    __builtin_unreachable();
}

// GetCollisionEdgePositionWithS (0x1128108): typed by tools/typed_lift2.py (round A6; guest frame calls, extra outputs xo[0]=f0 xo[1]=f20 xo[2]=a2 xo[3]=a3 xo[4]=t0 xo[5]=t1 xo[6]=t2 xo[7]=t3; VU0 in vf0 vf8 vf9 vf10 vf29 vf31 vacc h8 h9 h11 h29, out vf5 vf6 vf7 vf8 vf9 vf10 vf14 vf15 vf16 vf17 vf29 vf30 vf31 vacc h2 h5 h9 h11)
inline uint64_t GetCollisionEdgePositionWithS(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint64_t a0, uint64_t a1, uint32_t sp, float f0, float f12, float f13, float f14, float f15, float f16, float f17, float f18, float f19, float f20, const a4::vec4 &io_vf0, a4::vec4 &io_vf5, a4::vec4 &io_vf6, a4::vec4 &io_vf7, a4::vec4 &io_vf8, a4::vec4 &io_vf9, a4::vec4 &io_vf10, a4::vec4 &io_vf14, a4::vec4 &io_vf15, a4::vec4 &io_vf16, a4::vec4 &io_vf17, a4::vec4 &io_vf29, a4::vec4 &io_vf30, a4::vec4 &io_vf31, a4::vec4 &io_vacc, uint64_t &io_h2, uint64_t &io_h5, uint64_t io_h8, uint64_t &io_h9, uint64_t &io_h11, uint64_t io_h29, uint64_t *xo = nullptr)
{
    uint64_t v0 = 0;
    uint32_t v1 = 0;
    uint64_t a2 = 0;
    uint64_t a3 = 0;
    uint64_t t0 = 0;
    uint64_t t1 = 0;
    uint64_t t2 = 0;
    uint64_t t3 = 0;
    uint32_t t4 = 0;
    uint32_t t5 = 0;
    uint64_t s0 = 0;
    uint64_t s1 = 0;
    uint64_t s2 = 0;
    uint64_t s3 = 0;
    uint32_t s4 = 0;
    uint64_t s5 = 0;
    uint64_t s6 = 0;
    a4::vec4 vf0 = io_vf0;
    a4::vec4 vf5 = io_vf5;
    a4::vec4 vf6 = io_vf6;
    a4::vec4 vf7 = io_vf7;
    a4::vec4 vf8 = io_vf8;
    a4::vec4 vf9 = io_vf9;
    a4::vec4 vf10 = io_vf10;
    a4::vec4 vf14 = io_vf14;
    a4::vec4 vf15 = io_vf15;
    a4::vec4 vf16 = io_vf16;
    a4::vec4 vf17 = io_vf17;
    a4::vec4 vf29 = io_vf29;
    a4::vec4 vf30 = io_vf30;
    a4::vec4 vf31 = io_vf31;
    a4::vec4 vacc = io_vacc;
    uint64_t h2 = io_h2;
    uint64_t h5 = io_h5;
    uint64_t h8 = io_h8;
    uint64_t h9 = io_h9;
    uint64_t h11 = io_h11;
    uint64_t h29 = io_h29;
    bool a3xw0 = false;
    sp = sp - 0x70u;
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)255 << 16))));
    (v0 = (uint64_t)(RC_U64X(v0) | (uint64_t)(uint16_t)65535));
    (s2 = (uint64_t)((uint64_t)a1 + (uint64_t)0ULL));
    (s3 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(sp, 16))));
    (s5 = (uint64_t)((uint64_t)a0 + (uint64_t)0ULL));
    (s6 = (uint64_t)((uint64_t)a1_sx64(sp) + (uint64_t)0ULL));
    A1_W32B(sp, 96, a1_bits(f20));
    f20 = ((float)(f12));
    (s1 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)s2, 4))));
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)s2, 0))));
    (s0 = (uint64_t)((int64_t)(int32_t)((int32_t)SRL32((uint32_t)s1, 24))));
    (s1 = (uint64_t)(RC_U64X(s1) & RC_U64X(v0)));
    (s0 = (uint64_t)(RC_U64X(s0) & (uint64_t)(uint16_t)127));
    { (s4 = (uint32_t)((int32_t)((int32_t)A1_R32B((uint32_t)s2, 8)))); A3_CHARGE(); v0 = GetLayoutClass(ram, rt, a0, f12, f13, f14, f15, f16, f17, f18, f19, h2); }
    (t5 = (uint32_t)((int32_t)((int32_t)SLL32((uint32_t)s1, 2))));
    (t1 = (uint64_t)((uint64_t)v0 + (uint64_t)0ULL));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)SLL32((uint32_t)s0, 3))));
    (v1 = (uint32_t)((int32_t)((int32_t)A1_R32B((uint32_t)s2, 4))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v0, (uint32_t)s0))));
    (t2 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)t1, 340))));
    (t3 = (uint64_t)((int64_t)(int32_t)((int32_t)SLL32((uint32_t)v0, 4))));
    (a0 = (uint64_t)((uint64_t)s3 + (uint64_t)0ULL));
    (a1 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL));
    (a2 = (uint64_t)((uint64_t)t1 + (uint64_t)0ULL));
    (a3 = (uint64_t)((uint64_t)s0 + (uint64_t)0ULL));
    { const bool c = (int32_t)v1 >= 0; (t0 = (uint64_t)((uint64_t)s1 + (uint64_t)0ULL)); if (c) goto L_11281b0; }
    { A3_CHARGE(); uint64_t a7c[9] = {(uint64_t)a1_bits(f0), 0u, 0u, 0u, 0u, 0u, 0u, 0u, 0u}; v0 = manager_sub_01114C70(ram, rt, a0, a1, a2, a3, t0, sp, f12, f13, f14, f15, f16, f17, f18, f19, f20, vf8, vf10, vf14, vf15, vf16, vf17, vf29, vf30, vf31, vacc, h5, h9, h29, a7c); f0 = a1_flt((uint32_t)a7c[0]); f20 = a1_flt((uint32_t)a7c[1]); a2 = a7c[3]; a3 = a7c[4]; t1 = a7c[6]; t2 = a7c[7]; t3 = a7c[8]; a3xw0 = true; }
    v1 = s4 << 2;
    goto L_11281f4;
L_11281b0:;
    (a3 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)t1, 388))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)t2, 32))));
    (v1 = (uint32_t)((int32_t)((int32_t)A1_R32B((uint32_t)a3, 32))));
    (a2 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v0, 40))));
    (a3 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)v0, 36))));
    (t1 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B(v1, 8))));
    t4 = A1_R32B(v1, 24);
    (t3 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)t3, (uint32_t)t1))));
    (t0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)t2, 8))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)t3, 84))));
    (t2 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B(v1, 20))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(t5, (uint32_t)v0))));
    (t3 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)v0, 0))));
    (t3 = (uint64_t)((int64_t)(int32_t)((int32_t)SLL32((uint32_t)t3, 6))));
    { (t3 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(t4, (uint32_t)t3)))); A3_CHARGE(); uint64_t a7c[7] = {0u, a2, a3, 0u, t1, t2, t3}; v0 = manager_sub_01116108(ram, rt, a0, a1, a2, a3, t0, t1, t2, t3, a1_sx64(sp), f12, f13, f14, f15, f16, f17, f18, f19, vf8, vf10, vf14, vf15, vf16, vf17, vf29, vf30, vf31, vacc, h11, a7c); a2 = a7c[1]; a3 = a7c[2]; t1 = a7c[4]; t2 = a7c[5]; t3 = a7c[6]; }
    v1 = s4 << 2;
L_11281f4:;
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)s3, v1))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)281 << 16))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v0, v1))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)v0, 21656))));
    (a1 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)a0, 0))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)SLL32((uint32_t)v0, 2))));
    A1_W32B((uint32_t)s6, 0, (uint32_t)a1);
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)s3, (uint32_t)v0))));
    (v1 = (uint32_t)((int32_t)((int32_t)A1_R32B((uint32_t)v0, 0))));
    A1_W32B((uint32_t)s6, 4, v1);
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B(sp, 0))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B(sp, 4))));
    { uint32_t bits; std::memcpy(&bits, &f20, sizeof(bits)); (t0 = (uint64_t)((int64_t)(int32_t)(bits))); }
    vf7 = a4v::castsi128_ps(a4::mk128(h8, t0));
    vf5 = a4v::castsi128_ps(A4_R128(ADD32((uint32_t)a0, 0)));
    vf6 = a4v::castsi128_ps(A4_R128(ADD32((uint32_t)v0, 0)));
    { a4::vec4 res = a4v::subK<1, 0>(vf0, a4v::bcast<0>(vf7)); a4::q128 mask = a4v::set_epi32(-1, 0, 0, 0); vf8 = a4v::blendv_ps(vf8, res, a4v::castsi128_ps(mask)); }
    { a4::vec4 res = a4v::mul(vf5, a4v::bcast<0>(vf7)); vacc = a4v::dest<1, 1, 1, 1>(vacc, res); }
    { a4::vec4 mul_res = a4v::mulK<0, 1>(vf6, a4v::bcast<3>(vf8)); a4::vec4 res = a4v::addK<1, 1>(vacc, mul_res); a4::q128 mask = a4v::set_epi32(-1, -1, -1, -1); vf9 = a4v::blendv_ps(vf9, res, a4v::castsi128_ps(mask)); }
    A4_W128(ADD32((uint32_t)s5, 0), a4v::castps_si128(vf9));
    f20 = a1_flt(A1_R32B(sp, 96));
    { sp = sp + 0x70u; if (xo) { if (a3xw0) xo[0] = a1_bits(f0); xo[1] = a1_bits(f20); xo[2] = a2; xo[3] = a3; xo[4] = t0; xo[5] = t1; xo[6] = t2; xo[7] = t3; } io_vf5 = vf5; io_vf6 = vf6; io_vf7 = vf7; io_vf8 = vf8; io_vf9 = vf9; io_vf10 = vf10; io_vf14 = vf14; io_vf15 = vf15; io_vf16 = vf16; io_vf17 = vf17; io_vf29 = vf29; io_vf30 = vf30; io_vf31 = vf31; io_vacc = vacc; io_h2 = h2; io_h5 = h5; io_h9 = h9; io_h11 = h11; return v0; }
    __builtin_unreachable();
}

// GetSTPositionOfCollisionDB (0x1129630): typed by tools/typed_lift2.py (round A6; guest frame calls; VU0 in vf0 vf7 vf8 vf9 vf10 vf11 vf29 vf31 vacc h9 h11 h29, out vf5 vf6 vf7 vf8 vf9 vf10 vf11 vf14 vf15 vf16 vf17 vf29 vf30 vf31 vacc h2 h5 h9 h11)
inline a1::RetVF GetSTPositionOfCollisionDB(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint64_t a0, uint64_t a1, uint64_t a2, uint64_t a3, uint32_t sp, float f12, float f13, float f14, float f15, float f16, float f17, float f18, float f19, float f20, const a4::vec4 &io_vf0, a4::vec4 &io_vf5, a4::vec4 &io_vf6, a4::vec4 &io_vf7, a4::vec4 &io_vf8, a4::vec4 &io_vf9, a4::vec4 &io_vf10, a4::vec4 &io_vf11, a4::vec4 &io_vf14, a4::vec4 &io_vf15, a4::vec4 &io_vf16, a4::vec4 &io_vf17, a4::vec4 &io_vf29, a4::vec4 &io_vf30, a4::vec4 &io_vf31, a4::vec4 &io_vacc, uint64_t &io_h2, uint64_t &io_h5, uint64_t &io_h9, uint64_t &io_h11, uint64_t io_h29)
{
    float f0 = 0.0f;
    float f1 = 0.0f;
    float f2 = 0.0f;
    float f3 = 0.0f;
    uint32_t at = 0;
    uint64_t v0 = 0;
    uint64_t v1 = 0;
    uint64_t t0 = 0;
    uint32_t t1 = 0;
    uint32_t t2 = 0;
    uint32_t t3 = 0;
    uint32_t t4 = 0;
    uint32_t t5 = 0;
    uint64_t s0 = 0;
    uint64_t s1 = 0;
    uint64_t s2 = 0;
    uint32_t s3 = 0;
    uint64_t s4 = 0;
    uint64_t s5 = 0;
    uint64_t s6 = 0;
    uint64_t s7 = 0;
    uint64_t fp = 0;
    uint32_t fcr31 = 0;
    a4::vec4 vf0 = io_vf0;
    a4::vec4 vf5 = io_vf5;
    a4::vec4 vf6 = io_vf6;
    a4::vec4 vf7 = io_vf7;
    a4::vec4 vf8 = io_vf8;
    a4::vec4 vf9 = io_vf9;
    a4::vec4 vf10 = io_vf10;
    a4::vec4 vf11 = io_vf11;
    a4::vec4 vf14 = io_vf14;
    a4::vec4 vf15 = io_vf15;
    a4::vec4 vf16 = io_vf16;
    a4::vec4 vf17 = io_vf17;
    a4::vec4 vf29 = io_vf29;
    a4::vec4 vf30 = io_vf30;
    a4::vec4 vf31 = io_vf31;
    a4::vec4 vacc = io_vacc;
    uint64_t h2 = io_h2;
    uint64_t h5 = io_h5;
    uint64_t h9 = io_h9;
    uint64_t h11 = io_h11;
    uint64_t h29 = io_h29;
    sp = sp - 0x90u;
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)255 << 16))));
    (v0 = (uint64_t)(RC_U64X(v0) | (uint64_t)(uint16_t)65535));
    (s2 = (uint64_t)((uint64_t)a2 + (uint64_t)0ULL));
    s3 = sp + 0x10u;
    (s4 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(sp, 48))));
    (s5 = (uint64_t)((uint64_t)a3 + (uint64_t)0ULL));
    (s6 = (uint64_t)((uint64_t)a0 + (uint64_t)0ULL));
    (s7 = (uint64_t)((uint64_t)a1_sx64(sp) + (uint64_t)0ULL));
    (fp = (uint64_t)((uint64_t)a1 + (uint64_t)0ULL));
    (s1 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)s2, 4))));
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)s2, 0))));
    (s0 = (uint64_t)((int64_t)(int32_t)((int32_t)SRL32((uint32_t)s1, 24))));
    (s1 = (uint64_t)(RC_U64X(s1) & RC_U64X(v0)));
    { (s0 = (uint64_t)(RC_U64X(s0) & (uint64_t)(uint16_t)127)); A3_CHARGE(); v0 = GetLayoutClass(ram, rt, a0, f12, f13, f14, f15, f16, f17, f18, f19, h2); }
    (v1 = (uint64_t)((uint64_t)v0 + (uint64_t)0ULL));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)SLL32((uint32_t)s0, 3))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v0, (uint32_t)s0))));
    (t5 = (uint32_t)((int32_t)((int32_t)SLL32((uint32_t)s1, 2))));
    (t3 = (uint32_t)((int32_t)((int32_t)SLL32((uint32_t)v0, 4))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)s2, 4))));
    (t1 = (uint32_t)((int32_t)((int32_t)ADD32((uint32_t)v1, 340))));
    (a0 = (uint64_t)((uint64_t)s4 + (uint64_t)0ULL));
    (a1 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL));
    (a2 = (uint64_t)((uint64_t)v1 + (uint64_t)0ULL));
    (a3 = (uint64_t)((uint64_t)s0 + (uint64_t)0ULL));
    { const bool c = (int32_t)v0 >= 0; (t0 = (uint64_t)((uint64_t)s1 + (uint64_t)0ULL)); if (c) goto L_11296e0; }
    { A3_CHARGE(); v0 = manager_sub_01114C70(ram, rt, a0, a1, a2, a3, t0, sp, f12, f13, f14, f15, f16, f17, f18, f19, f20, vf8, vf10, vf14, vf15, vf16, vf17, vf29, vf30, vf31, vacc, h5, h9, h29); }
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B(sp, 52))));
    goto L_1129724;
L_11296e0:;
    (a3 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)v1, 388))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B(t1, 32))));
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)a3, 32))));
    (t0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B(t1, 8))));
    (a2 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v0, 40))));
    (t1 = (uint32_t)((int32_t)((int32_t)A1_R32B((uint32_t)v1, 8))));
    (a3 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)v0, 36))));
    t3 = t3 + t1;
    (t4 = (uint32_t)((int32_t)((int32_t)A1_R32B((uint32_t)v1, 24))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B(t3, 84))));
    (t2 = (uint32_t)((int32_t)((int32_t)A1_R32B((uint32_t)v1, 20))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(t5, (uint32_t)v0))));
    (t3 = (uint32_t)((int32_t)((int32_t)A1_R32B((uint32_t)v0, 0))));
    t3 = t3 << 6;
    { t3 = t4 + t3; A3_CHARGE(); v0 = manager_sub_01116108(ram, rt, a0, a1, a2, a3, t0, a1_sx64(t1), a1_sx64(t2), a1_sx64(t3), a1_sx64(sp), f12, f13, f14, f15, f16, f17, f18, f19, vf8, vf10, vf14, vf15, vf16, vf17, vf29, vf30, vf31, vacc, h11); }
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B(sp, 52))));
L_1129724:;
    vf29 = a4v::castsi128_ps(A4_R128(ADD32((uint32_t)v0, 0)));
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B(sp, 48))));
    vf30 = a4v::castsi128_ps(A4_R128(ADD32((uint32_t)v1, 0)));
    { a4::vec4 res = a4v::sub(vf29, vf30); a4::q128 mask = a4v::set_epi32(-1, -1, -1, -1); vf31 = a4v::blend(vf31, res, a4v::castsi128_ps(mask)); }
    A4_W128(ADD32((uint32_t)s7, 0), a4v::castps_si128(vf31));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B(sp, 56))));
    vf29 = a4v::castsi128_ps(A4_R128(ADD32((uint32_t)v0, 0)));
    { a4::vec4 res = a4v::sub(vf29, vf30); a4::q128 mask = a4v::set_epi32(-1, -1, -1, -1); vf31 = a4v::blend(vf31, res, a4v::castsi128_ps(mask)); }
    A4_W128(ADD32(s3, 0), a4v::castps_si128(vf31));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B(sp, 48))));
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(sp, 32))));
    vf5 = a4v::castsi128_ps(A4_R128(ADD32((uint32_t)s5, 0)));
    vf6 = a4v::castsi128_ps(A4_R128(ADD32((uint32_t)v0, 0)));
    { a4::vec4 res = a4v::sub(vf5, vf6); a4::q128 mask = a4v::set_epi32(-1, -1, -1, -1); vf7 = a4v::blend(vf7, res, a4v::castsi128_ps(mask)); }
    A4_W128(ADD32((uint32_t)v1, 0), a4v::castps_si128(vf7));
    vf5 = a4v::castsi128_ps(A4_R128(ADD32(sp, 0)));
    vf6 = a4v::castsi128_ps(A4_R128(ADD32(s3, 0)));
    vf7 = a4v::castsi128_ps(A4_R128(ADD32((uint32_t)v1, 0)));
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
    (at = (uint32_t)((int32_t)((int32_t)((uint32_t)47015 << 16))));
    at = at | 0xC5ACu;
    { uint32_t bits = a7_opq(at); std::memcpy(&f2, &bits, sizeof(bits)); }
    { float f = f0; uint32_t bits; std::memcpy(&bits, &f, sizeof(bits)); A1_W32B((uint32_t)s6, 0, bits); }
    { float f = f1; uint32_t bits; std::memcpy(&bits, &f, sizeof(bits)); A1_W32B((uint32_t)fp, 0, bits); }
    { uint32_t bits = A1_R32B((uint32_t)s6, 0); float f; std::memcpy(&f, &bits, sizeof(f)); f0 = f; }
    fcr31 = (a1f::lt(f0, f2)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
    { const bool c = (fcr31 & 0x800000u) != 0u; (v0 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL)); if (c) goto L_1129864; }
    fcr31 = (a1f::lt(f1, f2)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
    if ((fcr31 & 0x800000u) != 0u) goto L_1129868;
    f0 = a1f::add(f0, f1);
    (at = (uint32_t)((int32_t)((int32_t)((uint32_t)16256 << 16))));
    { uint32_t bits = a7_opq(at); std::memcpy(&f1, &bits, sizeof(bits)); }
    (at = (uint32_t)((int32_t)((int32_t)((uint32_t)14247 << 16))));
    at = at | 0xC5ACu;
    { uint32_t bits = a7_opq(at); std::memcpy(&f2, &bits, sizeof(bits)); }
    f0 = a1f::subFinite(f0, f1);
    fcr31 = (a1f::lt(f2, f0)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
    { const bool c = (fcr31 & 0x800000u) == 0u; (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 1)))); if (c) goto L_1129868; }
    (v0 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL));
L_1129864:;
L_1129868:;
    { sp = sp + 0x90u; io_vf5 = vf5; io_vf6 = vf6; io_vf7 = vf7; io_vf8 = vf8; io_vf9 = vf9; io_vf10 = vf10; io_vf11 = vf11; io_vf14 = vf14; io_vf15 = vf15; io_vf16 = vf16; io_vf17 = vf17; io_vf29 = vf29; io_vf30 = vf30; io_vf31 = vf31; io_vacc = vacc; io_h2 = h2; io_h5 = h5; io_h9 = h9; io_h11 = h11; return a1::RetVF{(uint64_t)v0, f0}; }
    __builtin_unreachable();
}

// manager_sub_01141150 (0x1141150): typed by tools/typed_lift2.py (round A6; frame calls, extra outputs xo[0]=f0 xo[1]=f12 xo[2]=f13; VU0 in vf10 vacc, out vf8 vf10 vf14 vf15 vf16 vf17 vacc, guest sp, frame 32 bytes, 0 slot accesses)
inline uint64_t manager_sub_01141150(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint64_t a0, uint64_t a1, uint32_t sp, float f0, float f12, float f13, float f14, float f15, float f16, float f17, float f18, float f19, a4::vec4 &io_vf8, a4::vec4 &io_vf10, a4::vec4 &io_vf14, a4::vec4 &io_vf15, a4::vec4 &io_vf16, a4::vec4 &io_vf17, a4::vec4 &io_vacc, uint64_t *xo = nullptr)
{
    float f1 = 0.0f;
    uint64_t v0 = 0;
    uint32_t v1 = 0;
    uint64_t s0 = 0;
    uint64_t s1 = 0;
    uint32_t fcr31 = 0;
    a4::vec4 vf8 = io_vf8;
    a4::vec4 vf10 = io_vf10;
    a4::vec4 vf14 = io_vf14;
    a4::vec4 vf15 = io_vf15;
    a4::vec4 vf16 = io_vf16;
    a4::vec4 vf17 = io_vf17;
    a4::vec4 vacc = io_vacc;
    bool a3xw0 = false;
    bool a3xw1 = false;
    bool a3xw2 = false;
    (s0 = (uint64_t)((uint64_t)a1 + (uint64_t)0ULL));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)s0, 8))));
    { const bool c = (int64_t)v0 == 0; (s1 = (uint64_t)((uint64_t)a0 + (uint64_t)0ULL)); if (c) goto L_1141218; }
    { A3_CHARGE(); uint64_t a7c[2] = {0u, 0u}; { const a1::RetVF a2_r = get_particle_lod_base_size(ram, rt, v0, a0, (uint32_t)(sp + 0xFFFFFFE0u), f12, f13, f14, f15, f16, f17, f18, f19, vf8, vf10, vf14, vf15, vf16, vf17, vacc, a7c); v0 = a2_r.v0; f0 = a2_r.f0; } f12 = a1_flt((uint32_t)a7c[0]); f13 = a1_flt((uint32_t)a7c[1]); a3xw0 = true; a3xw1 = true; a3xw2 = true; }
    (v1 = (uint32_t)((int32_t)((int32_t)A1_R32B((uint32_t)s1, 280))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)s0, 8))));
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 1))));
    { const bool c = (int64_t)v0 != (int64_t)a0; f1 = ((float)(f0)); if (c) goto L_11411b8; }
    { uint32_t bits = A1_R32B((uint32_t)s0, 20); float f; std::memcpy(&f, &bits, sizeof(f)); f0 = f; } a3xw0 = true;
    fcr31 = (a1f::lt(f1, f0)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
    if ((fcr31 & 0x800000u) != 0u) { A1_W32B((uint32_t)s1, 280, (uint32_t)a0); goto L_114121c; }
    { uint32_t bits = A1_R32B((uint32_t)s0, 24); float f; std::memcpy(&f, &bits, sizeof(f)); f0 = f; } a3xw0 = true;
    fcr31 = (a1f::lt(f1, f0)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
    if ((fcr31 & 0x800000u) != 0u) { A1_W32B((uint32_t)s1, 280, v1); goto L_114121c; }
    A1_W32B((uint32_t)s1, 280, 0u);
    goto L_114121c;
L_11411b8:;
    { uint32_t bits = A1_R32B((uint32_t)s0, 12); float f; std::memcpy(&f, &bits, sizeof(f)); f0 = f; } a3xw0 = true;
    fcr31 = (a1f::lt(f1, f0)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
    if ((fcr31 & 0x800000u) == 0u) { { uint32_t bits = A1_R32B((uint32_t)s0, 16); float f; std::memcpy(&f, &bits, sizeof(f)); f0 = f; } a3xw0 = true; goto L_11411d8; }
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 2))));
    A1_W32B((uint32_t)s1, 280, (uint32_t)v0);
    goto L_114121c;
L_11411d8:;
    fcr31 = (a1f::lt(f1, f0)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
    if ((fcr31 & 0x800000u) != 0u) { A1_W32B((uint32_t)s1, 280, v1); goto L_114121c; }
    { uint32_t bits = A1_R32B((uint32_t)s0, 20); float f; std::memcpy(&f, &bits, sizeof(f)); f0 = f; } a3xw0 = true;
    fcr31 = (a1f::lt(f1, f0)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
    if ((fcr31 & 0x800000u) == 0u) { { uint32_t bits = A1_R32B((uint32_t)s0, 24); float f; std::memcpy(&f, &bits, sizeof(f)); f0 = f; } a3xw0 = true; goto L_1141200; }
    A1_W32B((uint32_t)s1, 280, (uint32_t)a0);
    goto L_114121c;
L_1141200:;
    fcr31 = (a1f::lt(f1, f0)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
    if ((fcr31 & 0x800000u) == 0u) { A1_W32B((uint32_t)s1, 280, 0u); goto L_114121c; }
    A1_W32B((uint32_t)s1, 280, v1);
    goto L_114121c;
L_1141218:;
    A1_W32B((uint32_t)s1, 280, 0u);
L_114121c:;
    { if (xo) { if (a3xw0) xo[0] = a1_bits(f0); if (a3xw1) xo[1] = a1_bits(f12); if (a3xw2) xo[2] = a1_bits(f13); } io_vf8 = vf8; io_vf10 = vf10; io_vf14 = vf14; io_vf15 = vf15; io_vf16 = vf16; io_vf17 = vf17; io_vacc = vacc; return v0; }
    __builtin_unreachable();
}

// gamecore_sub_0136E5F8 (0x136e5f8): typed by tools/typed_lift2.py (round A6; frame calls, extra outputs xo[0]=a1 xo[1]=a2 xo[2]=a3 xo[3]=t1; VU0 in -, out h2 h6 h7 h8 h9, relocated calls 2, 64-bit inputs t0, frame 16 bytes, 0 slot accesses)
inline void gamecore_sub_0136E5F8(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint64_t v0, uint64_t a0, uint64_t a2, uint64_t a3, uint64_t w64_t0, uint64_t t1, float f12, float f13, float f14, float f15, float f16, float f17, float f18, float f19, uint64_t &io_h2, uint64_t &io_h6, uint64_t &io_h7, uint64_t &io_h8, uint64_t &io_h9, uint64_t *xo = nullptr)
{
    uint64_t a1 = 0;
    uint32_t t0 = (uint32_t)w64_t0;
    uint64_t h2 = io_h2;
    uint64_t h6 = io_h6;
    uint64_t h7 = io_h7;
    uint64_t h8 = io_h8;
    uint64_t h9 = io_h9;
    { (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)a0, 148)))); A3_CHARGE(); v0 = GetLayoutClass(ram, rt, a0, f12, f13, f14, f15, f16, f17, f18, f19, h2); }
    (a0 = (uint64_t)((uint64_t)v0 + (uint64_t)0ULL));
    { uint64_t a7c[6] = {0u, 0u, a2, a3, 0u, t1}; UpdateLayoutClassPastGeometry(ram, rt, a0, a3, w64_t0, t1, f12, f13, f14, f15, f16, f17, f18, f19, h2, h6, h7, h8, h9, a7c); a1 = a7c[1]; a2 = a7c[2]; a3 = a7c[3]; t1 = a7c[5]; if (xo) { xo[0] = a1; xo[1] = a2; xo[2] = a3; xo[3] = t1; } io_h2 = h2; io_h6 = h6; io_h7 = h7; io_h8 = h8; io_h9 = h9; return; }
    __builtin_unreachable();
}

// init_weakpoint_effect (0x1388288): typed by tools/typed_lift2.py (round A6; loop guest frame calls, extra outputs xo[0]=f0 xo[1]=a3 xo[2]=t0 xo[3]=t1 xo[4]=t2 xo[5]=t3; VU0 in -, out h2 h3, relocated calls 5)
inline uint64_t init_weakpoint_effect(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint64_t a1, uint64_t sp, float f0, float f12, float f13, float f14, float f15, float f16, float f17, float f18, float f19, uint64_t &io_h2, uint64_t &io_h3, uint64_t *xo = nullptr)
{
    uint64_t v0 = 0;
    uint64_t v1 = 0;
    uint64_t a0 = 0;
    uint64_t a2 = 0;
    uint64_t a3 = 0;
    uint64_t t0 = 0;
    uint64_t t1 = 0;
    uint64_t t2 = 0;
    uint64_t t3 = 0;
    uint64_t s0 = 0;
    uint64_t s1 = 0;
    uint64_t s2 = 0;
    uint64_t s3 = 0;
    uint64_t s4 = 0;
    uint64_t s5 = 0;
    uint64_t s6 = 0;
    uint64_t h2 = io_h2;
    uint64_t h3 = io_h3;
    (sp = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)sp, 4294967200))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)286 << 16))));
    (s0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v0, 25232))));
    (s1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 7))));
    (s2 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 3))));
    { const uint32_t _dynInsn = A1_R32(0x13882B0u); const int32_t _dynSimm = (int16_t)(_dynInsn & 0xFFFFu); const uint32_t _dynImm = _dynInsn & 0xFFFFu; (void)_dynSimm; (void)_dynImm; (s3 = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)_dynImm << 16)))); }
    { const uint32_t _dynInsn = A1_R32(0x13882B8u); const int32_t _dynSimm = (int16_t)(_dynInsn & 0xFFFFu); const uint32_t _dynImm = _dynInsn & 0xFFFFu; (void)_dynSimm; (void)_dynImm; (s4 = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)_dynImm << 16)))); }
    { const uint32_t _dynInsn = A1_R32(0x13882C0u); const int32_t _dynSimm = (int16_t)(_dynInsn & 0xFFFFu); const uint32_t _dynImm = _dynInsn & 0xFFFFu; (void)_dynSimm; (void)_dynImm; (s5 = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)_dynImm << 16)))); }
    { const uint32_t _dynInsn = A1_R32(0x13882C8u); const int32_t _dynSimm = (int16_t)(_dynInsn & 0xFFFFu); const uint32_t _dynImm = _dynInsn & 0xFFFFu; (void)_dynSimm; (void)_dynImm; (s6 = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)_dynImm << 16)))); }
L_13882d0:;
    (a0 = (uint64_t)((uint64_t)s0 + (uint64_t)0ULL));
    { (a1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 1)))); A3_CHARGE(); v0 = gamecore_sub_01386EF8(ram, rt, a0, a1, sp, f12, f13, f14, f15, f16, f17, f18, f19, h2, h3); }
    { const uint32_t _dynInsn = A1_R32(0x13882DCu); const int32_t _dynSimm = (int16_t)(_dynInsn & 0xFFFFu); const uint32_t _dynImm = _dynInsn & 0xFFFFu; (void)_dynSimm; (void)_dynImm; (a1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)s4, _dynSimm)))); }
    { const uint32_t _dynInsn = A1_R32(0x13882E0u); const int32_t _dynSimm = (int16_t)(_dynInsn & 0xFFFFu); const uint32_t _dynImm = _dynInsn & 0xFFFFu; (void)_dynSimm; (void)_dynImm; (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32(ADD32((uint32_t)s6, _dynSimm))))); }
    { const uint32_t _dynInsn = A1_R32(0x13882E4u); const int32_t _dynSimm = (int16_t)(_dynInsn & 0xFFFFu); const uint32_t _dynImm = _dynInsn & 0xFFFFu; (void)_dynSimm; (void)_dynImm; (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32(ADD32((uint32_t)s5, _dynSimm))))); }
    A1_W32B((uint32_t)sp, 16, (uint32_t)v0);
    A1_W32B((uint32_t)sp, 20, (uint32_t)v1);
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)s0, 92))));
    { uint32_t addr = ADD32((uint32_t)sp, 23); uint32_t aligned_addr = addr & ~7u; uint32_t offset = addr & 7u; uint64_t mem = A1_R64(aligned_addr); uint32_t shift = (7u - offset) << 3; uint64_t keepMask = (shift == 0) ? 0ull : ((1ull << shift) - 1ull); (v1 = (uint64_t)((RC_U64X(v1) & keepMask) | (mem << shift))); }
    { uint32_t addr = ADD32((uint32_t)sp, 16); uint32_t aligned_addr = addr & ~7u; uint32_t offset = addr & 7u; uint64_t mem = A1_R64(aligned_addr); uint32_t shift = offset << 3; uint64_t keepMask = (offset == 0) ? 0ull : (0xFFFFFFFFFFFFFFFFull << ((8u - offset) << 3)); (v1 = (uint64_t)((v1 & keepMask) | (mem >> shift))); }
    { uint32_t addr = ADD32((uint32_t)sp, 7); uint32_t aligned_addr = addr & ~7u; uint32_t offset = addr & 7u; uint32_t shift = (7u - offset) << 3; uint64_t mask = 0xFFFFFFFFFFFFFFFFull >> shift; uint64_t old_data = A1_R64(aligned_addr); uint64_t val = v1; uint64_t new_data = (old_data & ~mask) | ((val >> shift) & mask); A1_W64(aligned_addr, new_data); }
    { uint32_t addr = ADD32((uint32_t)sp, 0); uint32_t aligned_addr = addr & ~7u; uint32_t offset = addr & 7u; uint32_t shift = offset << 3; uint64_t mask = 0xFFFFFFFFFFFFFFFFull << shift; uint64_t old_data = A1_R64(aligned_addr); uint64_t val = v1; uint64_t new_data = (old_data & ~mask) | ((val << shift) & mask); A1_W64(aligned_addr, new_data); }
    { const bool c = (int64_t)v0 == 0; { const uint32_t _dynInsn = A1_R32(0x1388308u); const int32_t _dynSimm = (int16_t)(_dynInsn & 0xFFFFu); const uint32_t _dynImm = _dynInsn & 0xFFFFu; (void)_dynSimm; (void)_dynImm; (a2 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)s3, _dynSimm)))); } if (c) goto L_1388364; }
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)s0, 96))));
    { (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)a0, 208)))); A3_CHARGE(); v0 = SetGeometryPointerGlobalState(ram, rt, (uint32_t)a0, (uint32_t)a1, (uint32_t)a2, h2, h3); }
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)s0, 0))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)SLL32((uint32_t)v0, 2))));
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)sp, (uint32_t)v0))));
    { (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)v1, 0)))); A3_CHARGE(); v0 = get_particle_effect_id_from_sheet(ram, rt, a0); }
    (a1 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)s0, 92))));
    { (a0 = (uint64_t)((uint64_t)v0 + (uint64_t)0ULL)); A3_CHARGE(); v0 = stop_particle_emitter_geometry_pointer_reference(ram, rt, v0, a0, a1, f12, f13, f14, f15, f16, f17, f18, f19); }
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)s0, 0))));
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)SLL32((uint32_t)v1, 2))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)sp, (uint32_t)v1))));
    { (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)v0, 0)))); A3_CHARGE(); v0 = get_particle_effect_id_from_sheet(ram, rt, a0); }
    (a1 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)s0, 92))));
    (a0 = (uint64_t)((uint64_t)v0 + (uint64_t)0ULL));
    { (a2 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL)); A3_CHARGE(); v0 = end_particle_emitter(ram, rt, v0, a1, a2, f12, f13, f14, f15, f16, f17, f18, f19); }
    A1_W32B((uint32_t)s0, 100, (uint32_t)s2);
    A1_W32B((uint32_t)s0, 104, (uint32_t)s0);
L_1388364:;
    (s1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)s1, 4294967295))));
    { const bool c = (int32_t)s1 >= 0; (s0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)s0, 120)))); if (c) { A3_BACK(); goto L_13882d0; } }
    { A3_CHARGE(); uint64_t a7c[6] = {(uint64_t)a1_bits(f0), 0u, 0u, 0u, 0u, 0u}; v0 = a1_sx64(gamecore_sub_01386B98_x8(ram, rt, a7c)); f0 = a1_flt((uint32_t)a7c[0]); a3 = a7c[1]; t0 = a7c[2]; t1 = a7c[3]; t2 = a7c[4]; t3 = a7c[5]; }
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)326 << 16))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 1))));
    A1_W32B((uint32_t)v1, 26352, (uint32_t)v0);
    { (sp = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)sp, 96)))); if (xo) { xo[0] = a1_bits(f0); xo[1] = a3; xo[2] = t0; xo[3] = t1; xo[4] = t2; xo[5] = t3; } io_h2 = h2; io_h3 = h3; return v0; }
    __builtin_unreachable();
}

// end_weakpoint_effect (0x1388440): typed by tools/typed_lift2.py (round A6; loop guest frame calls, extra outputs xo[0]=a2 xo[1]=a3 xo[2]=t0 xo[3]=t1 xo[4]=t2 xo[5]=t3; VU0 in -, out h2 h3, relocated calls 5)
inline uint64_t end_weakpoint_effect(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint64_t a1, uint64_t sp, float f12, float f13, float f14, float f15, float f16, float f17, float f18, float f19, uint64_t &io_h2, uint64_t &io_h3, uint64_t *xo = nullptr)
{
    uint64_t v0 = 0;
    uint64_t v1 = 0;
    uint64_t a0 = 0;
    uint64_t a2 = 0;
    uint64_t a3 = 0;
    uint64_t t0 = 0;
    uint64_t t1 = 0;
    uint64_t t2 = 0;
    uint64_t t3 = 0;
    uint64_t s0 = 0;
    uint64_t s1 = 0;
    uint64_t s2 = 0;
    uint64_t s3 = 0;
    uint64_t s4 = 0;
    uint64_t s5 = 0;
    uint64_t s6 = 0;
    uint64_t s7 = 0;
    uint64_t h2 = io_h2;
    uint64_t h3 = io_h3;
    bool a3xw0 = false;
    bool a3xw1 = false;
    bool a3xw2 = false;
    bool a3xw3 = false;
    bool a3xw4 = false;
    bool a3xw5 = false;
    (sp = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)sp, 4294967184))));
    (s7 = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)326 << 16))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)s7, 26352))));
    if ((int64_t)v0 == 0) goto L_138853c;
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)286 << 16))));
    { const uint32_t _dynInsn = A1_R32(0x1388478u); const int32_t _dynSimm = (int16_t)(_dynInsn & 0xFFFFu); const uint32_t _dynImm = _dynInsn & 0xFFFFu; (void)_dynSimm; (void)_dynImm; (s6 = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)_dynImm << 16)))); }
    (s0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v0, 25232))));
    { const uint32_t _dynInsn = A1_R32(0x1388480u); const int32_t _dynSimm = (int16_t)(_dynInsn & 0xFFFFu); const uint32_t _dynImm = _dynInsn & 0xFFFFu; (void)_dynSimm; (void)_dynImm; (s5 = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)_dynImm << 16)))); }
    { const uint32_t _dynInsn = A1_R32(0x1388484u); const int32_t _dynSimm = (int16_t)(_dynInsn & 0xFFFFu); const uint32_t _dynImm = _dynInsn & 0xFFFFu; (void)_dynSimm; (void)_dynImm; (s4 = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)_dynImm << 16)))); }
    { const uint32_t _dynInsn = A1_R32(0x1388488u); const int32_t _dynSimm = (int16_t)(_dynInsn & 0xFFFFu); const uint32_t _dynImm = _dynInsn & 0xFFFFu; (void)_dynSimm; (void)_dynImm; (s3 = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)_dynImm << 16)))); }
    (s2 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 3))));
    (s1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 7))));
L_1388498:;
    (a0 = (uint64_t)((uint64_t)s0 + (uint64_t)0ULL));
    { (a1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 1)))); A3_CHARGE(); uint64_t a7c[8] = {0u, 0u, 0u, 0u, 0u, 0u, 0u, 0u}; v0 = gamecore_sub_01386EF8(ram, rt, a0, a1, sp, f12, f13, f14, f15, f16, f17, f18, f19, h2, h3, a7c); a3 = a7c[3]; t0 = a7c[4]; t1 = a7c[5]; t2 = a7c[6]; t3 = a7c[7]; a3xw1 = true; a3xw2 = true; a3xw3 = true; a3xw4 = true; a3xw5 = true; }
    { const uint32_t _dynInsn = A1_R32(0x13884A4u); const int32_t _dynSimm = (int16_t)(_dynInsn & 0xFFFFu); const uint32_t _dynImm = _dynInsn & 0xFFFFu; (void)_dynSimm; (void)_dynImm; (a1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)s4, _dynSimm)))); }
    { const uint32_t _dynInsn = A1_R32(0x13884A8u); const int32_t _dynSimm = (int16_t)(_dynInsn & 0xFFFFu); const uint32_t _dynImm = _dynInsn & 0xFFFFu; (void)_dynSimm; (void)_dynImm; (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32(ADD32((uint32_t)s6, _dynSimm))))); }
    { const uint32_t _dynInsn = A1_R32(0x13884ACu); const int32_t _dynSimm = (int16_t)(_dynInsn & 0xFFFFu); const uint32_t _dynImm = _dynInsn & 0xFFFFu; (void)_dynSimm; (void)_dynImm; (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32(ADD32((uint32_t)s5, _dynSimm))))); }
    A1_W32B((uint32_t)sp, 16, (uint32_t)v0);
    A1_W32B((uint32_t)sp, 20, (uint32_t)v1);
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)s0, 92))));
    { uint32_t addr = ADD32((uint32_t)sp, 23); uint32_t aligned_addr = addr & ~7u; uint32_t offset = addr & 7u; uint64_t mem = A1_R64(aligned_addr); uint32_t shift = (7u - offset) << 3; uint64_t keepMask = (shift == 0) ? 0ull : ((1ull << shift) - 1ull); (v1 = (uint64_t)((RC_U64X(v1) & keepMask) | (mem << shift))); }
    { uint32_t addr = ADD32((uint32_t)sp, 16); uint32_t aligned_addr = addr & ~7u; uint32_t offset = addr & 7u; uint64_t mem = A1_R64(aligned_addr); uint32_t shift = offset << 3; uint64_t keepMask = (offset == 0) ? 0ull : (0xFFFFFFFFFFFFFFFFull << ((8u - offset) << 3)); (v1 = (uint64_t)((v1 & keepMask) | (mem >> shift))); }
    { uint32_t addr = ADD32((uint32_t)sp, 7); uint32_t aligned_addr = addr & ~7u; uint32_t offset = addr & 7u; uint32_t shift = (7u - offset) << 3; uint64_t mask = 0xFFFFFFFFFFFFFFFFull >> shift; uint64_t old_data = A1_R64(aligned_addr); uint64_t val = v1; uint64_t new_data = (old_data & ~mask) | ((val >> shift) & mask); A1_W64(aligned_addr, new_data); }
    { uint32_t addr = ADD32((uint32_t)sp, 0); uint32_t aligned_addr = addr & ~7u; uint32_t offset = addr & 7u; uint32_t shift = offset << 3; uint64_t mask = 0xFFFFFFFFFFFFFFFFull << shift; uint64_t old_data = A1_R64(aligned_addr); uint64_t val = v1; uint64_t new_data = (old_data & ~mask) | ((val << shift) & mask); A1_W64(aligned_addr, new_data); }
    { const bool c = (int64_t)v0 == 0; { const uint32_t _dynInsn = A1_R32(0x13884D0u); const int32_t _dynSimm = (int16_t)(_dynInsn & 0xFFFFu); const uint32_t _dynImm = _dynInsn & 0xFFFFu; (void)_dynSimm; (void)_dynImm; (a2 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)s3, _dynSimm)))); } a3xw0 = true; if (c) goto L_138852c; }
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)s0, 96))));
    { (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)a0, 208)))); A3_CHARGE(); v0 = SetGeometryPointerGlobalState(ram, rt, (uint32_t)a0, (uint32_t)a1, (uint32_t)a2, h2, h3); }
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)s0, 0))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)SLL32((uint32_t)v0, 2))));
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)sp, (uint32_t)v0))));
    { (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)v1, 0)))); A3_CHARGE(); v0 = get_particle_effect_id_from_sheet(ram, rt, a0); }
    (a1 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)s0, 92))));
    { (a0 = (uint64_t)((uint64_t)v0 + (uint64_t)0ULL)); A3_CHARGE(); v0 = stop_particle_emitter_geometry_pointer_reference(ram, rt, v0, a0, a1, f12, f13, f14, f15, f16, f17, f18, f19); }
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)s0, 0))));
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)SLL32((uint32_t)v1, 2))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)sp, (uint32_t)v1))));
    { (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)v0, 0)))); A3_CHARGE(); v0 = get_particle_effect_id_from_sheet(ram, rt, a0); }
    (a1 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)s0, 92))));
    (a0 = (uint64_t)((uint64_t)v0 + (uint64_t)0ULL));
    { (a2 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL)); a3xw0 = true; A3_CHARGE(); v0 = end_particle_emitter(ram, rt, v0, a1, a2, f12, f13, f14, f15, f16, f17, f18, f19); }
    A1_W32B((uint32_t)s0, 100, (uint32_t)s2);
    A1_W32B((uint32_t)s0, 104, (uint32_t)s0);
L_138852c:;
    (s1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)s1, 4294967295))));
    { const bool c = (int32_t)s1 >= 0; (s0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)s0, 120)))); if (c) { A3_BACK(); goto L_1388498; } }
    A1_W32B((uint32_t)s7, 26352, 0u);
L_138853c:;
    { (sp = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)sp, 112)))); if (xo) { if (a3xw0) xo[0] = a2; if (a3xw1) xo[1] = a3; if (a3xw2) xo[2] = t0; if (a3xw3) xo[3] = t1; if (a3xw4) xo[4] = t2; if (a3xw5) xo[5] = t3; } io_h2 = h2; io_h3 = h3; return v0; }
    __builtin_unreachable();
}

// LinkSecureParentInfoByDBPos (0x10c2c88): typed by tools/typed_lift2.py (round A6; frame calls, extra outputs xo[0]=f0 xo[1]=a2 xo[2]=a3 xo[3]=t0 xo[4]=t1 xo[5]=t2 xo[6]=t3; VU0 in vf0 vf7 vf8 vf9 vf10 vf11 vf29 vf31 vacc h9 h11 h29, out vf5 vf6 vf7 vf8 vf9 vf10 vf11 vf14 vf15 vf16 vf17 vf29 vf30 vf31 vacc h2 h5 h9 h11, guest sp, frame 48 bytes, 0 slot accesses)
inline uint64_t LinkSecureParentInfoByDBPos(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint64_t v0, uint64_t a0, uint64_t a1, uint64_t a2, uint32_t sp, float f12, float f13, float f14, float f15, float f16, float f17, float f18, float f19, float f20, const a4::vec4 &io_vf0, a4::vec4 &io_vf5, a4::vec4 &io_vf6, a4::vec4 &io_vf7, a4::vec4 &io_vf8, a4::vec4 &io_vf9, a4::vec4 &io_vf10, a4::vec4 &io_vf11, a4::vec4 &io_vf14, a4::vec4 &io_vf15, a4::vec4 &io_vf16, a4::vec4 &io_vf17, a4::vec4 &io_vf29, a4::vec4 &io_vf30, a4::vec4 &io_vf31, a4::vec4 &io_vacc, uint64_t &io_h2, uint64_t &io_h5, uint64_t &io_h9, uint64_t &io_h11, uint64_t io_h29, uint64_t *xo = nullptr)
{
    float f0 = 0.0f;
    uint64_t a3 = 0;
    uint64_t t0 = 0;
    uint64_t t1 = 0;
    uint64_t t2 = 0;
    uint64_t t3 = 0;
    uint64_t s0 = 0;
    uint64_t s1 = 0;
    uint64_t s2 = 0;
    uint64_t s3 = 0;
    a4::vec4 vf0 = io_vf0;
    a4::vec4 vf5 = io_vf5;
    a4::vec4 vf6 = io_vf6;
    a4::vec4 vf7 = io_vf7;
    a4::vec4 vf8 = io_vf8;
    a4::vec4 vf9 = io_vf9;
    a4::vec4 vf10 = io_vf10;
    a4::vec4 vf11 = io_vf11;
    a4::vec4 vf14 = io_vf14;
    a4::vec4 vf15 = io_vf15;
    a4::vec4 vf16 = io_vf16;
    a4::vec4 vf17 = io_vf17;
    a4::vec4 vf29 = io_vf29;
    a4::vec4 vf30 = io_vf30;
    a4::vec4 vf31 = io_vf31;
    a4::vec4 vacc = io_vacc;
    uint64_t h2 = io_h2;
    uint64_t h5 = io_h5;
    uint64_t h9 = io_h9;
    uint64_t h11 = io_h11;
    uint64_t h29 = io_h29;
    (s0 = (uint64_t)((uint64_t)a0 + (uint64_t)0ULL));
    (s1 = (uint64_t)((uint64_t)a1 + (uint64_t)0ULL));
    (s2 = (uint64_t)((uint64_t)a2 + (uint64_t)0ULL));
    { (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)s1, 0)))); A3_CHARGE(); v0 = GetLayoutClass(ram, rt, a0, f12, f13, f14, f15, f16, f17, f18, f19, h2); }
    (a0 = (uint64_t)((uint64_t)s0 + (uint64_t)0ULL));
    (a1 = (uint64_t)((uint64_t)s1 + (uint64_t)0ULL));
    (a2 = (uint64_t)((uint64_t)s2 + (uint64_t)0ULL));
    { (s3 = (uint64_t)((uint64_t)v0 + (uint64_t)0ULL)); A3_CHARGE(); uint64_t a7c[7] = {0u, 0u, 0u, 0u, 0u, 0u, 0u}; { const a1::RetVF a2_r = GetGeometryPointerWithCollisionDBDataGlobalPoint(ram, rt, a0, a1, a2, (uint32_t)(sp + 0xFFFFFFD0u), f12, f13, f14, f15, f16, f17, f18, f19, f20, vf0, vf5, vf6, vf7, vf8, vf9, vf10, vf11, vf14, vf15, vf16, vf17, vf29, vf30, vf31, vacc, h2, h5, h9, h11, h29, a7c); v0 = a2_r.v0; f0 = a2_r.f0; } a2 = a7c[1]; a3 = a7c[2]; t0 = a7c[3]; t1 = a7c[4]; t2 = a7c[5]; t3 = a7c[6]; }
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 1))));
    A1_W32B((uint32_t)s0, 76, (uint32_t)v0);
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)s3, 280))));
    (a1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)s0, 64))));
    { if (xo) { xo[0] = a1_bits(f0); xo[1] = a2; xo[2] = a3; xo[3] = t0; xo[4] = t1; xo[5] = t2; xo[6] = t3; } const uint32_t a4_t = AddTwoWayLinkNext(ram, rt, (uint32_t)a0, (uint32_t)a1); io_vf5 = vf5; io_vf6 = vf6; io_vf7 = vf7; io_vf8 = vf8; io_vf9 = vf9; io_vf10 = vf10; io_vf11 = vf11; io_vf14 = vf14; io_vf15 = vf15; io_vf16 = vf16; io_vf17 = vf17; io_vf29 = vf29; io_vf30 = vf30; io_vf31 = vf31; io_vacc = vacc; io_h2 = h2; io_h5 = h5; io_h9 = h9; io_h11 = h11; return a1_sx64(a4_t); }
    __builtin_unreachable();
}

// manager_sub_010D41E0 (0x10d41e0): typed by tools/typed_lift2.py (round A6; guest frame calls; VU0 in vf0 vf6 vf7 vf10 vf18 vf29 vf31 vacc vstat h9 h11 h29, out vf4 vf5 vf6 vf7 vf8 vf10 vf14 vf15 vf16 vf17 vf18 vf29 vf30 vf31 vacc vq vstat h2 h5 h9 h11)
inline uint64_t manager_sub_010D41E0(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint64_t v0, uint64_t a0, uint64_t a1, uint64_t t0, uint64_t sp, float f0, float f12, float f13, float f14, float f15, float f16, float f17, float f18, float f19, float f20, const a4::vec4 &io_vf0, a4::vec4 &io_vf4, a4::vec4 &io_vf5, a4::vec4 &io_vf6, a4::vec4 &io_vf7, a4::vec4 &io_vf8, a4::vec4 &io_vf10, a4::vec4 &io_vf14, a4::vec4 &io_vf15, a4::vec4 &io_vf16, a4::vec4 &io_vf17, a4::vec4 &io_vf18, a4::vec4 &io_vf29, a4::vec4 &io_vf30, a4::vec4 &io_vf31, a4::vec4 &io_vacc, float &io_vq, uint16_t &io_vstat, uint64_t &io_h2, uint64_t &io_h5, uint64_t &io_h9, uint64_t &io_h11, uint64_t io_h29)
{
    float f1 = 0.0f;
    uint64_t v1 = 0;
    uint64_t s0 = 0;
    uint64_t s1 = 0;
    uint64_t s2 = 0;
    uint64_t s3 = 0;
    uint32_t fcr31 = 0;
    a4::vec4 vf0 = io_vf0;
    a4::vec4 vf4 = io_vf4;
    a4::vec4 vf5 = io_vf5;
    a4::vec4 vf6 = io_vf6;
    a4::vec4 vf7 = io_vf7;
    a4::vec4 vf8 = io_vf8;
    a4::vec4 vf10 = io_vf10;
    a4::vec4 vf14 = io_vf14;
    a4::vec4 vf15 = io_vf15;
    a4::vec4 vf16 = io_vf16;
    a4::vec4 vf17 = io_vf17;
    a4::vec4 vf18 = io_vf18;
    a4::vec4 vf29 = io_vf29;
    a4::vec4 vf30 = io_vf30;
    a4::vec4 vf31 = io_vf31;
    a4::vec4 vacc = io_vacc;
    float vq = io_vq;
    uint16_t vstat = io_vstat;
    uint64_t h2 = io_h2;
    uint64_t h5 = io_h5;
    uint64_t h9 = io_h9;
    uint64_t h11 = io_h11;
    uint64_t h29 = io_h29;
    (sp = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)sp, 4294967248))));
    (s0 = (uint64_t)((uint64_t)a1 + (uint64_t)0ULL));
    { (s3 = (uint64_t)((uint64_t)a0 + (uint64_t)0ULL)); A3_CHARGE(); v0 = GetCollisionDBAttributeAdr(ram, rt, v0, a0, f12, f13, f14, f15, f16, f17, f18, f19, h2); }
    (s2 = (uint64_t)(A1_R64B((uint32_t)v0, 0)));
    (v0 = (uint64_t)(0ULL | (uint64_t)(uint16_t)49152));
    (v0 = (uint64_t)(RC_U64X(v0) << 18));
    (v1 = (uint64_t)(0ULL | (uint64_t)(uint16_t)63488));
    (v1 = (uint64_t)(RC_U64X(v1) << 20));
    (a0 = (uint64_t)(s2 & v1));
    { const bool c = (int64_t)a0 == (int64_t)v0; (v0 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL)); if (c) goto L_10d43c8; }
    (v0 = (uint64_t)(0ULL | (uint64_t)(uint16_t)40960));
    (v0 = (uint64_t)(RC_U64X(v0) << 19));
    { const bool c = (int64_t)a0 == (int64_t)v0; (v0 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL)); if (c) goto L_10d43c8; }
    (v0 = (uint64_t)(0ULL | (uint64_t)(uint16_t)40960));
    (v0 = (uint64_t)(RC_U64X(v0) << 18));
    { const bool c = (int64_t)a0 == (int64_t)v0; (v0 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL)); if (c) goto L_10d43c8; }
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)s0, 0))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)31744 << 16))));
    { const bool c = (int64_t)v1 != 0; (s1 = (uint64_t)(s2 & RC_U64X(v0))); if (c) goto L_10d4260; }
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)12288 << 16))));
    { const bool c = (int64_t)s1 == (int64_t)v1; (v0 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL)); if (c) goto L_10d43c8; }
L_10d4260:;
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)s0, 4))));
    { const bool c = (int64_t)v0 != 0; (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)27648 << 16)))); if (c) goto L_10d427c; }
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)13312 << 16))));
    { const bool c = (int64_t)s1 == (int64_t)v1; (v0 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL)); if (c) goto L_10d43c8; }
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)27648 << 16))));
L_10d427c:;
    { const bool c = (int64_t)s1 == (int64_t)v1; (v0 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL)); if (c) goto L_10d43c8; }
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)s0, 8))));
    if ((int64_t)v0 != 0) { (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)s0, 12)))); goto L_10d42a0; }
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)9216 << 16))));
    { const bool c = (int64_t)s1 == (int64_t)v1; (v0 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL)); if (c) goto L_10d43c8; }
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)s0, 12))));
L_10d42a0:;
    if ((int64_t)v0 != 0) { (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)s0, 24)))); goto L_10d42cc; }
    (v0 = (uint64_t)(0ULL | (uint64_t)(uint16_t)32768));
    (v0 = (uint64_t)(RC_U64X(v0) << 18));
    { const bool c = (int64_t)a0 == (int64_t)v0; (v0 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL)); if (c) goto L_10d43c8; }
    (v0 = (uint64_t)(0ULL | (uint64_t)(uint16_t)47104));
    (v0 = (uint64_t)(RC_U64X(v0) << 20));
    { const bool c = (int64_t)a0 == (int64_t)v0; (v0 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL)); if (c) goto L_10d43c8; }
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)s0, 24))));
L_10d42cc:;
    if ((int64_t)v0 != 0) { (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)s0, 28)))); goto L_10d42e4; }
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)1024 << 16))));
    { const bool c = (int64_t)s1 == (int64_t)v1; (v0 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL)); if (c) goto L_10d43c8; }
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)s0, 28))));
L_10d42e4:;
    { const bool c = (int64_t)v0 != 0; (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)25600 << 16)))); if (c) goto L_10d42fc; }
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)18432 << 16))));
    { const bool c = (int64_t)s1 == (int64_t)v1; (v0 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL)); if (c) goto L_10d43c8; }
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)25600 << 16))));
L_10d42fc:;
    { const bool c = (int64_t)s1 != (int64_t)v0; (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)26624 << 16)))); if (c) goto L_10d4330; }
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)s0, 16))));
    if ((int64_t)v0 == 0) { (v0 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL)); goto L_10d43c8; }
    { (a0 = (uint64_t)((uint64_t)s3 + (uint64_t)0ULL)); A3_CHARGE(); uint64_t a7c[10] = {0u, (uint64_t)a1_bits(f20), 0u, 0u, 0u, 0u, t0, 0u, 0u, 0u}; v0 = GetCollisionDBPlane(ram, rt, a0, t0, sp, f0, f12, f13, f14, f15, f16, f17, f18, f19, f20, vf0, vf4, vf5, vf6, vf7, vf8, vf10, vf14, vf15, vf16, vf17, vf18, vf29, vf30, vf31, vacc, vq, vstat, h2, h5, h9, h11, h29, a7c); f20 = a1_flt((uint32_t)a7c[1]); t0 = a7c[6]; }
    { uint32_t bits = A1_R32B((uint32_t)s0, 36); float f; std::memcpy(&f, &bits, sizeof(f)); f1 = f; }
    { uint32_t bits = A1_R32B((uint32_t)v0, 4); float f; std::memcpy(&f, &bits, sizeof(f)); f0 = f; }
    fcr31 = (a1f::lt(f1, f0)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
    { const bool c = (fcr31 & 0x800000u) != 0u; (v0 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL)); if (c) goto L_10d43c8; }
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)26624 << 16))));
L_10d4330:;
    { const bool c = (int64_t)s1 != (int64_t)v0; (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)15360 << 16)))); if (c) goto L_10d4398; }
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)s0, 20))));
    if ((int64_t)v0 == 0) { (v0 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL)); goto L_10d43c8; }
    { (a0 = (uint64_t)((uint64_t)s3 + (uint64_t)0ULL)); A3_CHARGE(); v0 = GetCollisionDBPlane(ram, rt, a0, t0, sp, f0, f12, f13, f14, f15, f16, f17, f18, f19, f20, vf0, vf4, vf5, vf6, vf7, vf8, vf10, vf14, vf15, vf16, vf17, vf18, vf29, vf30, vf31, vacc, vq, vstat, h2, h5, h9, h11, h29); }
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)s0, 48))));
    (a0 = (uint64_t)((uint64_t)v0 + (uint64_t)0ULL));
    vf5 = a4v::castsi128_ps(A4_R128(ADD32((uint32_t)a0, 0)));
    vf6 = a4v::castsi128_ps(A4_R128(ADD32((uint32_t)v1, 0)));
    { a4::vec4 res = a4v::addK<1, 1>(vf0, a4v::bcast<3>(vf0)); a4::q128 mask = a4v::set_epi32(0, 0, 0, -1); vf7 = a4v::blendv_ps(vf7, res, a4v::castsi128_ps(mask)); }
    { a4::vec4 res = a4v::mul(vf5, vf6); a4::q128 mask = a4v::set_epi32(0, -1, -1, -1); vf6 = a4v::blend(vf6, res, a4v::castsi128_ps(mask)); }
    { a4::vec4 res = a4v::addK<1, 1>(vf0, a4v::bcast<0>(vf6)); vacc = a4v::dest<1, 0, 0, 0>(vacc, res); }
    { a4::vec4 mul_res = a4v::mulK<1, 1>(vf7, a4v::bcast<1>(vf6)); a4::vec4 res = a4v::addK<1, 1>(vacc, mul_res); vacc = a4v::dest<1, 0, 0, 0>(vacc, res); }
    { a4::vec4 mul_res = a4v::mulK<1, 1>(vf7, a4v::bcast<2>(vf6)); a4::vec4 res = a4v::addK<1, 1>(vacc, mul_res); a4::q128 mask = a4v::set_epi32(0, 0, 0, -1); vf6 = a4v::blendv_ps(vf6, res, a4v::castsi128_ps(mask)); }
    do { const a4::q128 _v = (a4v::castps_si128(vf6)); h2 = a4::hi64(_v); std::memcpy(&v0, &_v, 8); } while (0);
    { uint32_t bits = (uint32_t)v0; std::memcpy(&f0, &bits, sizeof(bits)); }
    f0 = (-(float)(f0));
    { uint32_t bits = A1_R32B((uint32_t)s0, 40); float f; std::memcpy(&f, &bits, sizeof(f)); f1 = f; }
    fcr31 = (a1f::lt(f1, f0)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
    if ((fcr31 & 0x800000u) == 0u) { (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)15360 << 16)))); goto L_10d4398; }
    (v0 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL));
    goto L_10d43c8;
L_10d4398:;
    if ((int64_t)s1 != (int64_t)v0) goto L_10d43ac;
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)s0, 32))));
    { const bool c = (int64_t)v1 == 0; (v0 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL)); if (c) goto L_10d43c8; }
L_10d43ac:;
    (v0 = (uint64_t)(0ULL | (uint64_t)(uint16_t)61440));
    (v0 = (uint64_t)(RC_U64X(v0) << 30));
    (v1 = (uint64_t)(0ULL | (uint64_t)(uint16_t)32768));
    (v1 = (uint64_t)(RC_U64X(v1) << 28));
    (v0 = (uint64_t)(s2 & v0));
    (v0 = (uint64_t)(v0 ^ v1));
    (v0 = (uint64_t)(((uint64_t)0ULL < (uint64_t)v0) ? 1 : 0));
L_10d43c8:;
    { (sp = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)sp, 48)))); io_vf4 = vf4; io_vf5 = vf5; io_vf6 = vf6; io_vf7 = vf7; io_vf8 = vf8; io_vf10 = vf10; io_vf14 = vf14; io_vf15 = vf15; io_vf16 = vf16; io_vf17 = vf17; io_vf18 = vf18; io_vf29 = vf29; io_vf30 = vf30; io_vf31 = vf31; io_vacc = vacc; io_vq = vq; io_vstat = vstat; io_h2 = h2; io_h5 = h5; io_h9 = h9; io_h11 = h11; return v0; }
    __builtin_unreachable();
}

// GetCollisionEdgePointerHVPlane (0x111a2d8): typed by tools/typed_lift2.py (round A6; guest frame calls, extra outputs xo[0]=f20 xo[1]=a1 xo[2]=a2 xo[3]=a3 xo[4]=t0 xo[5]=t1 xo[6]=t2 xo[7]=t3; VU0 in vf0 vf6 vf7 vf10 vf18 vf29 vf31 vacc vstat h9 h11 h29, out vf4 vf5 vf6 vf7 vf8 vf10 vf14 vf15 vf16 vf17 vf18 vf29 vf30 vf31 vacc vq vstat h2 h5 h9 h11)
inline a1::RetVF GetCollisionEdgePointerHVPlane(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint64_t a0, uint64_t a1, uint64_t a2, uint64_t a3, uint64_t t0, uint64_t sp, float f12, float f13, float f14, float f15, float f16, float f17, float f18, float f19, float f20, const a4::vec4 &io_vf0, a4::vec4 &io_vf4, a4::vec4 &io_vf5, a4::vec4 &io_vf6, a4::vec4 &io_vf7, a4::vec4 &io_vf8, a4::vec4 &io_vf10, a4::vec4 &io_vf14, a4::vec4 &io_vf15, a4::vec4 &io_vf16, a4::vec4 &io_vf17, a4::vec4 &io_vf18, a4::vec4 &io_vf29, a4::vec4 &io_vf30, a4::vec4 &io_vf31, a4::vec4 &io_vacc, float &io_vq, uint16_t &io_vstat, uint64_t &io_h2, uint64_t &io_h5, uint64_t &io_h9, uint64_t &io_h11, uint64_t io_h29, uint64_t *xo = nullptr)
{
    float f0 = 0.0f;
    float f1 = 0.0f;
    uint32_t at = 0;
    uint64_t v0 = 0;
    uint32_t v1 = 0;
    uint64_t t1 = 0;
    uint64_t t2 = 0;
    uint64_t t3 = 0;
    uint32_t s0 = 0;
    uint64_t s1 = 0;
    uint64_t s2 = 0;
    uint64_t s3 = 0;
    uint64_t s4 = 0;
    uint64_t s5 = 0;
    uint64_t s6 = 0;
    uint64_t s7 = 0;
    uint64_t fp = 0;
    uint32_t fcr31 = 0;
    a4::vec4 vf0 = io_vf0;
    a4::vec4 vf4 = io_vf4;
    a4::vec4 vf5 = io_vf5;
    a4::vec4 vf6 = io_vf6;
    a4::vec4 vf7 = io_vf7;
    a4::vec4 vf8 = io_vf8;
    a4::vec4 vf10 = io_vf10;
    a4::vec4 vf14 = io_vf14;
    a4::vec4 vf15 = io_vf15;
    a4::vec4 vf16 = io_vf16;
    a4::vec4 vf17 = io_vf17;
    a4::vec4 vf18 = io_vf18;
    a4::vec4 vf29 = io_vf29;
    a4::vec4 vf30 = io_vf30;
    a4::vec4 vf31 = io_vf31;
    a4::vec4 vacc = io_vacc;
    float vq = io_vq;
    uint16_t vstat = io_vstat;
    uint64_t h2 = io_h2;
    uint64_t h5 = io_h5;
    uint64_t h9 = io_h9;
    uint64_t h11 = io_h11;
    uint64_t h29 = io_h29;
    bool a3xw0 = false;
    bool a3xw2 = false;
    bool a3xw5 = false;
    bool a3xw6 = false;
    bool a3xw7 = false;
    (sp = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)sp, 4294967136))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)255 << 16))));
    (v0 = (uint64_t)(RC_U64X(v0) | (uint64_t)(uint16_t)65535));
    (s4 = (uint64_t)((uint64_t)a2 + (uint64_t)0ULL));
    (s5 = (uint64_t)((uint64_t)a0 + (uint64_t)0ULL));
    (s6 = (uint64_t)((uint64_t)a3 + (uint64_t)0ULL));
    (s7 = (uint64_t)((uint64_t)t0 + (uint64_t)0ULL));
    (fp = (uint64_t)((uint64_t)a1 + (uint64_t)0ULL));
    (t0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)s7, 4))));
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)s7, 0))));
    (v1 = (uint32_t)((int32_t)((int32_t)SRL32((uint32_t)t0, 24))));
    (s3 = (uint64_t)(RC_U64X(t0) & RC_U64X(v0)));
    { (s1 = (uint64_t)(a1_sx64(v1) & (uint64_t)(uint16_t)127)); A3_CHARGE(); v0 = GetLayoutClass(ram, rt, a0, f12, f13, f14, f15, f16, f17, f18, f19, h2); }
    (a3 = (uint64_t)((uint64_t)v0 + (uint64_t)0ULL));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)s7, 4))));
    { const bool c = (int32_t)v0 >= 0; (v1 = (uint32_t)((int32_t)((int32_t)SLL32((uint32_t)s1, 1)))); if (c) goto L_111a428; }
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)a3, 372))));
    (v1 = (uint32_t)((int32_t)((int32_t)ADD32(v1, (uint32_t)s1))));
    (a1 = (uint64_t)((int64_t)(int32_t)((int32_t)SLL32((uint32_t)s3, 4))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)a0, 36))));
    v1 = v1 << 5;
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v0, v1))));
    (s2 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v0, 48))));
    (v1 = (uint32_t)((int32_t)((int32_t)A1_R32B((uint32_t)s2, 8))));
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)s2, 20))));
    (v1 = (uint32_t)((int32_t)((int32_t)ADD32(v1, (uint32_t)s3))));
    (v0 = (uint64_t)(uint32_t)((uint8_t)A1_R8B(v1, 0)));
    { const bool c = (int64_t)v0 != 0; (s0 = (uint32_t)((int32_t)((int32_t)ADD32((uint32_t)a0, (uint32_t)a1)))); if (c) goto L_111a540; }
    (a2 = (uint64_t)((uint64_t)a3 + (uint64_t)0ULL)); a3xw2 = true;
    (a3 = (uint64_t)((uint64_t)s1 + (uint64_t)0ULL));
    (a0 = (uint64_t)((uint64_t)sp + (uint64_t)0ULL));
    (a1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)sp, 16))));
    { (t0 = (uint64_t)((uint64_t)s3 + (uint64_t)0ULL)); A3_CHARGE(); uint64_t a7c[9] = {0u, 0u, 0u, 0u, 0u, 0u, 0u, 0u, 0u}; v0 = manager_sub_01114C70(ram, rt, a0, a1, a2, a3, t0, (uint32_t)sp, f12, f13, f14, f15, f16, f17, f18, f19, f20, vf8, vf10, vf14, vf15, vf16, vf17, vf29, vf30, vf31, vacc, h5, h9, h29, a7c); f20 = a1_flt((uint32_t)a7c[1]); t3 = a7c[8]; a3xw0 = true; a3xw7 = true; }
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)sp, 0))));
    vf29 = a4v::castsi128_ps(A4_R128(ADD32((uint32_t)v0, 0)));
    (v1 = (uint32_t)((int32_t)((int32_t)A1_R32B((uint32_t)sp, 4))));
    vf30 = a4v::castsi128_ps(A4_R128(ADD32(v1, 0)));
    { a4::vec4 res = a4v::sub(vf29, vf30); a4::q128 mask = a4v::set_epi32(-1, -1, -1, -1); vf31 = a4v::blend(vf31, res, a4v::castsi128_ps(mask)); }
    { a4::q128 mask = a4v::set_epi32(-1, -1, -1, -1); vf18 = a4v::blendv_ps(vf18, vf31, a4v::castsi128_ps(mask)); }
    { a4::q128 mask = a4v::set_epi32(-1, -1, -1, -1); vf29 = a4v::blendv_ps(vf29, vf30, a4v::castsi128_ps(mask)); }
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)sp, 8))));
    vf30 = a4v::castsi128_ps(A4_R128(ADD32((uint32_t)v0, 0)));
    { a4::vec4 res = a4v::sub(vf29, vf30); a4::q128 mask = a4v::set_epi32(-1, -1, -1, -1); vf31 = a4v::blend(vf31, res, a4v::castsi128_ps(mask)); }
    { a4::q128 mask = a4v::set_epi32(-1, -1, -1, -1); vf29 = a4v::blendv_ps(vf29, vf18, a4v::castsi128_ps(mask)); }
    { a4::q128 mask = a4v::set_epi32(-1, -1, -1, -1); vf30 = a4v::blendv_ps(vf30, vf31, a4v::castsi128_ps(mask)); }
    { a4::vec4 fs_yzx = a4v::shuffle_ps<0xC9>(vf29, vf29); a4::vec4 ft_zxy = a4v::shuffle_ps<0xD2>(vf30, vf30); a4::vec4 res = a4v::mulK<1, 1>(fs_yzx, ft_zxy); vacc = a4v::dest<1, 1, 1, 0>(vacc, res); }
    { a4::vec4 fs_yzx = a4v::shuffle_ps<0xC9>(vf30, vf30); a4::vec4 ft_zxy = a4v::shuffle_ps<0xD2>(vf29, vf29); a4::vec4 mul_res = a4v::mulK<1, 1>(fs_yzx, ft_zxy); a4::vec4 res = a4v::subK<1, 1>(vacc, mul_res); a4::q128 mask = a4v::set_epi32(0, -1, -1, -1); vf31 = a4v::blendv_ps(vf31, res, a4v::castsi128_ps(mask)); }
    { a4::vec4 res = a4v::subK<1, 1>(vf31, vf31); a4::q128 mask = a4v::set_epi32(-1, 0, 0, 0); vf31 = a4v::blend(vf31, res, a4v::castsi128_ps(mask)); }
    { a4::q128 mask = a4v::set_epi32(-1, -1, -1, -1); vf29 = a4v::blendv_ps(vf29, vf31, a4v::castsi128_ps(mask)); }
    { a4::vec4 res = a4v::mulK<1, 1>(vf29, vf29); a4::q128 mask = a4v::set_epi32(0, -1, -1, -1); vf7 = a4v::blend(vf7, res, a4v::castsi128_ps(mask)); }
    { a4::vec4 res = a4v::mulK<1, 1>(vf0, a4v::bcast<0>(vf7)); vacc = a4v::dest<0, 0, 0, 1>(vacc, res); }
    { a4::vec4 mul_res = a4v::mulK<1, 1>(vf0, a4v::bcast<1>(vf7)); a4::vec4 res = a4v::addK<1, 1>(vacc, mul_res); vacc = a4v::dest<0, 0, 0, 1>(vacc, res); }
    { a4::vec4 mul_res = a4v::mulK<1, 1>(vf0, a4v::bcast<2>(vf7)); a4::vec4 res = a4v::addK<1, 1>(vacc, mul_res); a4::q128 mask = a4v::set_epi32(-1, 0, 0, 0); vf7 = a4v::blendv_ps(vf7, res, a4v::castsi128_ps(mask)); }
    { float fs = a4v::cvtss_f32(a4v::shuffle_ps<0x03>(vf0, vf0)); float ft = a4v::cvtss_f32(a4v::shuffle_ps<0x03>(vf7, vf7)); vq = a4v::rsqrt(vstat, fs, ft); }
    { a4::vec4 res = a4v::mul(vf29, a4v::set1_ps(vq)); a4::q128 mask = a4v::set_epi32(0, -1, -1, -1); vf29 = a4v::blendv_ps(vf29, res, a4v::castsi128_ps(mask)); }
    { a4::q128 mask = a4v::set_epi32(-1, -1, -1, -1); vf31 = a4v::blendv_ps(vf31, vf29, a4v::castsi128_ps(mask)); }
    A4_W128(ADD32(s0, 0), a4v::castps_si128(vf31));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)sp, 0))));
    vf4 = a4v::castsi128_ps(A4_R128(ADD32(s0, 0)));
    vf5 = a4v::castsi128_ps(A4_R128(ADD32((uint32_t)v0, 0)));
    { a4::vec4 res = a4v::mul(vf4, vf5); a4::q128 mask = a4v::set_epi32(0, -1, -1, -1); vf5 = a4v::blend(vf5, res, a4v::castsi128_ps(mask)); }
    { a4::vec4 res = a4v::addK<1, 1>(vf5, a4v::bcast<1>(vf5)); a4::q128 mask = a4v::set_epi32(0, 0, 0, -1); vf5 = a4v::blendv_ps(vf5, res, a4v::castsi128_ps(mask)); }
    { a4::vec4 res = a4v::addK<1, 1>(vf5, a4v::bcast<2>(vf5)); a4::q128 mask = a4v::set_epi32(0, 0, 0, -1); vf5 = a4v::blendv_ps(vf5, res, a4v::castsi128_ps(mask)); }
    { a4::vec4 res = a4v::subK<1, 1>(vf0, a4v::bcast<3>(vf0)); a4::q128 mask = a4v::set_epi32(-1, -1, -1, -1); vf6 = a4v::blendv_ps(vf6, res, a4v::castsi128_ps(mask)); }
    { a4::vec4 res = a4v::subK<1, 1>(vf6, a4v::bcast<0>(vf5)); a4::q128 mask = a4v::set_epi32(-1, 0, 0, 0); vf4 = a4v::blendv_ps(vf4, res, a4v::castsi128_ps(mask)); }
    A4_W128(ADD32(s0, 0), a4v::castps_si128(vf4));
    (a0 = (uint64_t)((uint64_t)s2 + (uint64_t)0ULL));
    (a1 = (uint64_t)((uint64_t)s3 + (uint64_t)0ULL));
    goto L_111a538;
L_111a428:;
    (v1 = (uint32_t)((int32_t)((int32_t)A1_R32B((uint32_t)a3, 388))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)SLL32((uint32_t)s1, 3))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v0, (uint32_t)s1))));
    (a1 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)a3, 372))));
    (t2 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B(v1, 32)))); a3xw6 = true;
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)SLL32((uint32_t)v0, 4))));
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)SLL32((uint32_t)s3, 2))));
    (s2 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)a1, 88))));
    (t1 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)t2, 8)))); a3xw5 = true;
    (a1 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)s2, 8))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v0, (uint32_t)t1))));
    (a2 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)s2, 20)))); a3xw2 = true;
    (v1 = (uint32_t)((int32_t)((int32_t)A1_R32B((uint32_t)v0, 84))));
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)a0, v1))));
    (s1 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)a0, 0))));
    (a1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)a1, (uint32_t)s1))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)SLL32((uint32_t)s1, 4))));
    (v1 = (uint32_t)((uint8_t)A1_R8B((uint32_t)a1, 0)));
    { const bool c = v1 != 0; (s0 = (uint32_t)((int32_t)((int32_t)ADD32((uint32_t)a2, (uint32_t)v0)))); if (c) goto L_111a540; }
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)a3, 340))));
    (v1 = (uint32_t)((int32_t)((int32_t)A1_R32B((uint32_t)t2, 24))));
    (a2 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)v0, 32)))); a3xw2 = true;
    (t3 = (uint64_t)((int64_t)(int32_t)((int32_t)SLL32((uint32_t)s1, 6)))); a3xw7 = true;
    (t0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)v0, 8))));
    (t3 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(v1, (uint32_t)t3)))); a3xw7 = true;
    (a3 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)a2, 36))));
    (a2 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)a2, 40)))); a3xw2 = true;
    (t2 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)t2, 20)))); a3xw6 = true;
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)sp, 32))));
    { (a1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)sp, 48)))); A3_CHARGE(); uint64_t a7c[7] = {0u, 0u, 0u, 0u, 0u, 0u, t3}; v0 = manager_sub_01116108(ram, rt, a0, a1, a2, a3, t0, t1, t2, t3, sp, f12, f13, f14, f15, f16, f17, f18, f19, vf8, vf10, vf14, vf15, vf16, vf17, vf29, vf30, vf31, vacc, h11, a7c); t3 = a7c[6]; a3xw7 = true; }
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)sp, 32))));
    vf29 = a4v::castsi128_ps(A4_R128(ADD32((uint32_t)v0, 0)));
    (v1 = (uint32_t)((int32_t)((int32_t)A1_R32B((uint32_t)sp, 36))));
    vf30 = a4v::castsi128_ps(A4_R128(ADD32(v1, 0)));
    { a4::vec4 res = a4v::sub(vf29, vf30); a4::q128 mask = a4v::set_epi32(-1, -1, -1, -1); vf31 = a4v::blend(vf31, res, a4v::castsi128_ps(mask)); }
    { a4::q128 mask = a4v::set_epi32(-1, -1, -1, -1); vf18 = a4v::blendv_ps(vf18, vf31, a4v::castsi128_ps(mask)); }
    { a4::q128 mask = a4v::set_epi32(-1, -1, -1, -1); vf29 = a4v::blendv_ps(vf29, vf30, a4v::castsi128_ps(mask)); }
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)sp, 40))));
    vf30 = a4v::castsi128_ps(A4_R128(ADD32((uint32_t)v0, 0)));
    { a4::vec4 res = a4v::sub(vf29, vf30); a4::q128 mask = a4v::set_epi32(-1, -1, -1, -1); vf31 = a4v::blend(vf31, res, a4v::castsi128_ps(mask)); }
    { a4::q128 mask = a4v::set_epi32(-1, -1, -1, -1); vf29 = a4v::blendv_ps(vf29, vf18, a4v::castsi128_ps(mask)); }
    { a4::q128 mask = a4v::set_epi32(-1, -1, -1, -1); vf30 = a4v::blendv_ps(vf30, vf31, a4v::castsi128_ps(mask)); }
    { a4::vec4 fs_yzx = a4v::shuffle_ps<0xC9>(vf29, vf29); a4::vec4 ft_zxy = a4v::shuffle_ps<0xD2>(vf30, vf30); a4::vec4 res = a4v::mulK<1, 1>(fs_yzx, ft_zxy); vacc = a4v::dest<1, 1, 1, 0>(vacc, res); }
    { a4::vec4 fs_yzx = a4v::shuffle_ps<0xC9>(vf30, vf30); a4::vec4 ft_zxy = a4v::shuffle_ps<0xD2>(vf29, vf29); a4::vec4 mul_res = a4v::mulK<1, 1>(fs_yzx, ft_zxy); a4::vec4 res = a4v::subK<1, 1>(vacc, mul_res); a4::q128 mask = a4v::set_epi32(0, -1, -1, -1); vf31 = a4v::blendv_ps(vf31, res, a4v::castsi128_ps(mask)); }
    { a4::vec4 res = a4v::subK<1, 1>(vf31, vf31); a4::q128 mask = a4v::set_epi32(-1, 0, 0, 0); vf31 = a4v::blend(vf31, res, a4v::castsi128_ps(mask)); }
    { a4::q128 mask = a4v::set_epi32(-1, -1, -1, -1); vf29 = a4v::blendv_ps(vf29, vf31, a4v::castsi128_ps(mask)); }
    { a4::vec4 res = a4v::mulK<1, 1>(vf29, vf29); a4::q128 mask = a4v::set_epi32(0, -1, -1, -1); vf7 = a4v::blend(vf7, res, a4v::castsi128_ps(mask)); }
    { a4::vec4 res = a4v::mulK<1, 1>(vf0, a4v::bcast<0>(vf7)); vacc = a4v::dest<0, 0, 0, 1>(vacc, res); }
    { a4::vec4 mul_res = a4v::mulK<1, 1>(vf0, a4v::bcast<1>(vf7)); a4::vec4 res = a4v::addK<1, 1>(vacc, mul_res); vacc = a4v::dest<0, 0, 0, 1>(vacc, res); }
    { a4::vec4 mul_res = a4v::mulK<1, 1>(vf0, a4v::bcast<2>(vf7)); a4::vec4 res = a4v::addK<1, 1>(vacc, mul_res); a4::q128 mask = a4v::set_epi32(-1, 0, 0, 0); vf7 = a4v::blendv_ps(vf7, res, a4v::castsi128_ps(mask)); }
    { float fs = a4v::cvtss_f32(a4v::shuffle_ps<0x03>(vf0, vf0)); float ft = a4v::cvtss_f32(a4v::shuffle_ps<0x03>(vf7, vf7)); vq = a4v::rsqrt(vstat, fs, ft); }
    { a4::vec4 res = a4v::mul(vf29, a4v::set1_ps(vq)); a4::q128 mask = a4v::set_epi32(0, -1, -1, -1); vf29 = a4v::blendv_ps(vf29, res, a4v::castsi128_ps(mask)); }
    { a4::q128 mask = a4v::set_epi32(-1, -1, -1, -1); vf31 = a4v::blendv_ps(vf31, vf29, a4v::castsi128_ps(mask)); }
    A4_W128(ADD32(s0, 0), a4v::castps_si128(vf31));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)sp, 32))));
    vf4 = a4v::castsi128_ps(A4_R128(ADD32(s0, 0)));
    vf5 = a4v::castsi128_ps(A4_R128(ADD32((uint32_t)v0, 0)));
    { a4::vec4 res = a4v::mul(vf4, vf5); a4::q128 mask = a4v::set_epi32(0, -1, -1, -1); vf5 = a4v::blend(vf5, res, a4v::castsi128_ps(mask)); }
    { a4::vec4 res = a4v::addK<1, 1>(vf5, a4v::bcast<1>(vf5)); a4::q128 mask = a4v::set_epi32(0, 0, 0, -1); vf5 = a4v::blendv_ps(vf5, res, a4v::castsi128_ps(mask)); }
    { a4::vec4 res = a4v::addK<1, 1>(vf5, a4v::bcast<2>(vf5)); a4::q128 mask = a4v::set_epi32(0, 0, 0, -1); vf5 = a4v::blendv_ps(vf5, res, a4v::castsi128_ps(mask)); }
    { a4::vec4 res = a4v::subK<1, 1>(vf0, a4v::bcast<3>(vf0)); a4::q128 mask = a4v::set_epi32(-1, -1, -1, -1); vf6 = a4v::blendv_ps(vf6, res, a4v::castsi128_ps(mask)); }
    { a4::vec4 res = a4v::subK<1, 1>(vf6, a4v::bcast<0>(vf5)); a4::q128 mask = a4v::set_epi32(-1, 0, 0, 0); vf4 = a4v::blendv_ps(vf4, res, a4v::castsi128_ps(mask)); }
    A4_W128(ADD32(s0, 0), a4v::castps_si128(vf4));
    (a0 = (uint64_t)((uint64_t)s2 + (uint64_t)0ULL));
    (a1 = (uint64_t)((uint64_t)s1 + (uint64_t)0ULL));
L_111a538:;
    { A3_CHARGE(); uint64_t a7c[5] = {0u, 0u, 0u, 0u, 0u}; v0 = EntryModifiedManager(ram, rt, a0, (uint32_t)a1, a7c); a2 = a7c[0]; a3 = a7c[1]; t0 = a7c[2]; t1 = a7c[3]; t2 = a7c[4]; a3xw2 = true; a3xw5 = true; a3xw6 = true; }
L_111a540:;
    (a0 = (uint64_t)((uint64_t)a1_sx64(s0) + (uint64_t)0ULL));
    { uint32_t bits = A1_R32B((uint32_t)a0, 4); float f; std::memcpy(&f, &bits, sizeof(f)); f1 = f; }
    { uint32_t bits = 0u; std::memcpy(&f0, &bits, sizeof(bits)); }
    fcr31 = (a1f::lt(f1, f0)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
    if ((fcr31 & 0x800000u) == 0u) goto L_111a578;
    (at = (uint32_t)((int32_t)((int32_t)((uint32_t)49011 << 16))));
    (at = (uint32_t)(a1_sx64(at) | (uint64_t)(uint16_t)13107));
    { uint32_t bits = a7_opq(at); std::memcpy(&f0, &bits, sizeof(bits)); }
    fcr31 = (a1f::lt(f1, f0)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
    { const bool c = (fcr31 & 0x800000u) != 0u; (v0 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL)); if (c) goto L_111a708; }
    (s0 = (uint32_t)((int32_t)((int32_t)ADD32((uint32_t)sp, 16))));
    goto L_111a598;
L_111a578:;
    (at = (uint32_t)((int32_t)((int32_t)((uint32_t)16243 << 16))));
    (at = (uint32_t)(a1_sx64(at) | (uint64_t)(uint16_t)13107));
    { uint32_t bits = a7_opq(at); std::memcpy(&f0, &bits, sizeof(bits)); }
    fcr31 = (a1f::lt(f0, f1)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
    { const bool c = (fcr31 & 0x800000u) == 0u; (s0 = (uint32_t)((int32_t)((int32_t)ADD32((uint32_t)sp, 16)))); if (c) goto L_111a598; }
    (v0 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL));
    goto L_111a708;
L_111a598:;
    (s1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)sp, 32))));
    (a3 = (uint64_t)((uint64_t)s7 + (uint64_t)0ULL));
    (a0 = (uint64_t)((uint64_t)sp + (uint64_t)0ULL));
    (a1 = (uint64_t)((uint64_t)a1_sx64(s0) + (uint64_t)0ULL));
    (a2 = (uint64_t)((uint64_t)RC_U64X(s1) + (uint64_t)0ULL)); a3xw2 = true;
    { (s2 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 1)))); A3_CHARGE(); uint64_t a7c[8] = {(uint64_t)a1_bits(f20), 0u, 0u, 0u, 0u, 0u, 0u, 0u}; { const a1::RetVF a2_r = GetCollisionEdgePointerGlobalVerticalPlane(ram, rt, a0, a1, a2, a3, sp, f12, f13, f14, f15, f16, f17, f18, f19, f20, vf0, vf4, vf5, vf6, vf7, vf8, vf10, vf14, vf15, vf16, vf17, vf18, vf29, vf30, vf31, vacc, vq, vstat, h2, h5, h9, h11, h29, a7c); v0 = a2_r.v0; f0 = a2_r.f0; } f20 = a1_flt((uint32_t)a7c[0]); a1 = a7c[1]; a2 = a7c[2]; a3 = a7c[3]; t0 = a7c[4]; t1 = a7c[5]; t2 = a7c[6]; t3 = a7c[7]; a3xw0 = true; a3xw2 = true; a3xw5 = true; a3xw6 = true; a3xw7 = true; }
    { const bool c = (int64_t)v0 == 0; (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)sp, 0)))); if (c) goto L_111a708; }
    { const bool c = (int64_t)fp == 0; (a1 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)sp, 4)))); if (c) goto L_111a5cc; }
    do { const a4::q128 _v = (A4_R128(ADD32((uint32_t)sp, 32))); h2 = a4::hi64(_v); std::memcpy(&v0, &_v, 8); } while (0);
    A4_W128(ADD32((uint32_t)fp, 0), a4::mk128(h2, v0));
L_111a5cc:;
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)sp, 48))));
    vf5 = a4v::castsi128_ps(A4_R128(ADD32(s0, 0)));
    vf6 = a4v::castsi128_ps(A4_R128(ADD32((uint32_t)s1, 0)));
    { a4::vec4 fs_yzx = a4v::shuffle_ps<0xC9>(vf5, vf5); a4::vec4 ft_zxy = a4v::shuffle_ps<0xD2>(vf6, vf6); a4::vec4 res = a4v::mul(fs_yzx, ft_zxy); vacc = a4v::dest<1, 1, 1, 0>(vacc, res); }
    { a4::vec4 fs_yzx = a4v::shuffle_ps<0xC9>(vf6, vf6); a4::vec4 ft_zxy = a4v::shuffle_ps<0xD2>(vf5, vf5); a4::vec4 mul_res = a4v::mul(fs_yzx, ft_zxy); a4::vec4 res = a4v::subK<1, 1>(vacc, mul_res); a4::q128 mask = a4v::set_epi32(0, -1, -1, -1); vf7 = a4v::blendv_ps(vf7, res, a4v::castsi128_ps(mask)); }
    { a4::vec4 res = a4v::sub(vf7, vf7); a4::q128 mask = a4v::set_epi32(-1, 0, 0, 0); vf7 = a4v::blend(vf7, res, a4v::castsi128_ps(mask)); }
    A4_W128(ADD32((uint32_t)v0, 0), a4v::castps_si128(vf7));
    vf5 = a4v::castsi128_ps(A4_R128(ADD32((uint32_t)v0, 0)));
    { a4::vec4 res = a4v::mul(vf5, vf5); a4::q128 mask = a4v::set_epi32(0, -1, -1, -1); vf7 = a4v::blend(vf7, res, a4v::castsi128_ps(mask)); }
    { a4::vec4 res = a4v::mulK<1, 1>(vf0, a4v::bcast<0>(vf7)); vacc = a4v::dest<0, 0, 0, 1>(vacc, res); }
    { a4::vec4 mul_res = a4v::mulK<1, 1>(vf0, a4v::bcast<1>(vf7)); a4::vec4 res = a4v::addK<1, 1>(vacc, mul_res); vacc = a4v::dest<0, 0, 0, 1>(vacc, res); }
    { a4::vec4 mul_res = a4v::mulK<1, 1>(vf0, a4v::bcast<2>(vf7)); a4::vec4 res = a4v::addK<1, 1>(vacc, mul_res); a4::q128 mask = a4v::set_epi32(-1, 0, 0, 0); vf7 = a4v::blendv_ps(vf7, res, a4v::castsi128_ps(mask)); }
    { float fs = a4v::cvtss_f32(a4v::shuffle_ps<0x03>(vf0, vf0)); float ft = a4v::cvtss_f32(a4v::shuffle_ps<0x03>(vf7, vf7)); vq = a4v::rsqrt(vstat, fs, ft); }
    { a4::vec4 res = a4v::mul(vf5, a4v::set1_ps(vq)); a4::q128 mask = a4v::set_epi32(0, -1, -1, -1); vf5 = a4v::blendv_ps(vf5, res, a4v::castsi128_ps(mask)); }
    A4_W128(ADD32((uint32_t)v0, 0), a4v::castps_si128(vf5));
    { uint32_t bits = 0u; std::memcpy(&f1, &bits, sizeof(bits)); }
    { uint32_t bits = A1_R32B((uint32_t)sp, 52); float f; std::memcpy(&f, &bits, sizeof(f)); f0 = f; }
    fcr31 = (a1f::lt(f0, f1)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
    if ((fcr31 & 0x800000u) != 0u) { (s2 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 4294967295)))); goto L_111a620; }
L_111a620:;
    { const bool c = (int64_t)s5 == 0; do { const a4::q128 _v = (A4_R128(ADD32((uint32_t)sp, 48))); h2 = a4::hi64(_v); std::memcpy(&v0, &_v, 8); } while (0); if (c) goto L_111a64c; }
    A4_W128(ADD32((uint32_t)s5, 0), a4::mk128(h2, v0));
    vf4 = a4v::castsi128_ps(A4_R128(ADD32((uint32_t)s5, 0)));
    vf5 = a4v::castsi128_ps(A4_R128(ADD32((uint32_t)a0, 0)));
    { a4::vec4 res = a4v::mul(vf4, vf5); a4::q128 mask = a4v::set_epi32(0, -1, -1, -1); vf5 = a4v::blend(vf5, res, a4v::castsi128_ps(mask)); }
    { a4::vec4 res = a4v::addK<1, 1>(vf5, a4v::bcast<1>(vf5)); a4::q128 mask = a4v::set_epi32(0, 0, 0, -1); vf5 = a4v::blendv_ps(vf5, res, a4v::castsi128_ps(mask)); }
    { a4::vec4 res = a4v::addK<1, 1>(vf5, a4v::bcast<2>(vf5)); a4::q128 mask = a4v::set_epi32(0, 0, 0, -1); vf5 = a4v::blendv_ps(vf5, res, a4v::castsi128_ps(mask)); }
    { a4::vec4 res = a4v::subK<1, 1>(vf0, a4v::bcast<3>(vf0)); a4::q128 mask = a4v::set_epi32(-1, -1, -1, -1); vf6 = a4v::blendv_ps(vf6, res, a4v::castsi128_ps(mask)); }
    { a4::vec4 res = a4v::subK<1, 1>(vf6, a4v::bcast<0>(vf5)); a4::q128 mask = a4v::set_epi32(-1, 0, 0, 0); vf4 = a4v::blendv_ps(vf4, res, a4v::castsi128_ps(mask)); }
    A4_W128(ADD32((uint32_t)s5, 0), a4v::castps_si128(vf4));
L_111a64c:;
    { const uint32_t _dynInsn = A1_R32(0x111A64Cu); const int32_t _dynSimm = (int16_t)(_dynInsn & 0xFFFFu); const uint32_t _dynImm = _dynInsn & 0xFFFFu; (void)_dynSimm; (void)_dynImm; (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)_dynImm << 16)))); }
    (v1 = (uint32_t)((int32_t)((int32_t)ADD32((uint32_t)sp, 64))));
    { const uint32_t _dynInsn = A1_R32(0x111A654u); const int32_t _dynSimm = (int16_t)(_dynInsn & 0xFFFFu); const uint32_t _dynImm = _dynInsn & 0xFFFFu; (void)_dynSimm; (void)_dynImm; (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v0, _dynSimm)))); }
    vf5 = a4v::castsi128_ps(A4_R128(ADD32((uint32_t)v0, 0)));
    vf6 = a4v::castsi128_ps(A4_R128(ADD32((uint32_t)s1, 0)));
    { a4::vec4 fs_yzx = a4v::shuffle_ps<0xC9>(vf5, vf5); a4::vec4 ft_zxy = a4v::shuffle_ps<0xD2>(vf6, vf6); a4::vec4 res = a4v::mul(fs_yzx, ft_zxy); vacc = a4v::dest<1, 1, 1, 0>(vacc, res); }
    { a4::vec4 fs_yzx = a4v::shuffle_ps<0xC9>(vf6, vf6); a4::vec4 ft_zxy = a4v::shuffle_ps<0xD2>(vf5, vf5); a4::vec4 mul_res = a4v::mul(fs_yzx, ft_zxy); a4::vec4 res = a4v::subK<1, 1>(vacc, mul_res); a4::q128 mask = a4v::set_epi32(0, -1, -1, -1); vf7 = a4v::blendv_ps(vf7, res, a4v::castsi128_ps(mask)); }
    { a4::vec4 res = a4v::sub(vf7, vf7); a4::q128 mask = a4v::set_epi32(-1, 0, 0, 0); vf7 = a4v::blend(vf7, res, a4v::castsi128_ps(mask)); }
    A4_W128(ADD32(v1, 0), a4v::castps_si128(vf7));
    vf5 = a4v::castsi128_ps(A4_R128(ADD32(v1, 0)));
    { a4::vec4 res = a4v::mul(vf5, vf5); a4::q128 mask = a4v::set_epi32(0, -1, -1, -1); vf7 = a4v::blend(vf7, res, a4v::castsi128_ps(mask)); }
    { a4::vec4 res = a4v::mulK<1, 1>(vf0, a4v::bcast<0>(vf7)); vacc = a4v::dest<0, 0, 0, 1>(vacc, res); }
    { a4::vec4 mul_res = a4v::mulK<1, 1>(vf0, a4v::bcast<1>(vf7)); a4::vec4 res = a4v::addK<1, 1>(vacc, mul_res); vacc = a4v::dest<0, 0, 0, 1>(vacc, res); }
    { a4::vec4 mul_res = a4v::mulK<1, 1>(vf0, a4v::bcast<2>(vf7)); a4::vec4 res = a4v::addK<1, 1>(vacc, mul_res); a4::q128 mask = a4v::set_epi32(-1, 0, 0, 0); vf7 = a4v::blendv_ps(vf7, res, a4v::castsi128_ps(mask)); }
    { float fs = a4v::cvtss_f32(a4v::shuffle_ps<0x03>(vf0, vf0)); float ft = a4v::cvtss_f32(a4v::shuffle_ps<0x03>(vf7, vf7)); vq = a4v::rsqrt(vstat, fs, ft); }
    { a4::vec4 res = a4v::mul(vf5, a4v::set1_ps(vq)); a4::q128 mask = a4v::set_epi32(0, -1, -1, -1); vf5 = a4v::blendv_ps(vf5, res, a4v::castsi128_ps(mask)); }
    A4_W128(ADD32(v1, 0), a4v::castps_si128(vf5));
    { const bool c = (int64_t)s6 == 0; do { const a4::q128 _v = (A4_R128(ADD32((uint32_t)sp, 64))); h2 = a4::hi64(_v); std::memcpy(&v0, &_v, 8); } while (0); if (c) goto L_111a6c0; }
    A4_W128(ADD32((uint32_t)s6, 0), a4::mk128(h2, v0));
    vf4 = a4v::castsi128_ps(A4_R128(ADD32((uint32_t)s6, 0)));
    vf5 = a4v::castsi128_ps(A4_R128(ADD32((uint32_t)a1, 0)));
    { a4::vec4 res = a4v::mul(vf4, vf5); a4::q128 mask = a4v::set_epi32(0, -1, -1, -1); vf5 = a4v::blend(vf5, res, a4v::castsi128_ps(mask)); }
    { a4::vec4 res = a4v::addK<1, 1>(vf5, a4v::bcast<1>(vf5)); a4::q128 mask = a4v::set_epi32(0, 0, 0, -1); vf5 = a4v::blendv_ps(vf5, res, a4v::castsi128_ps(mask)); }
    { a4::vec4 res = a4v::addK<1, 1>(vf5, a4v::bcast<2>(vf5)); a4::q128 mask = a4v::set_epi32(0, 0, 0, -1); vf5 = a4v::blendv_ps(vf5, res, a4v::castsi128_ps(mask)); }
    { a4::vec4 res = a4v::subK<1, 1>(vf0, a4v::bcast<3>(vf0)); a4::q128 mask = a4v::set_epi32(-1, -1, -1, -1); vf6 = a4v::blendv_ps(vf6, res, a4v::castsi128_ps(mask)); }
    { a4::vec4 res = a4v::subK<1, 1>(vf6, a4v::bcast<0>(vf5)); a4::q128 mask = a4v::set_epi32(-1, 0, 0, 0); vf4 = a4v::blendv_ps(vf4, res, a4v::castsi128_ps(mask)); }
    A4_W128(ADD32((uint32_t)s6, 0), a4v::castps_si128(vf4));
L_111a6c0:;
    { const bool c = (int64_t)s4 == 0; (v0 = (uint64_t)((uint64_t)s2 + (uint64_t)0ULL)); if (c) goto L_111a708; }
    (at = (uint32_t)((int32_t)((int32_t)((uint32_t)49024 << 16))));
    { uint32_t bits = a7_opq(at); std::memcpy(&f0, &bits, sizeof(bits)); }
    { uint32_t bits; std::memcpy(&bits, &f0, sizeof(bits)); (v0 = (uint64_t)((int64_t)(int32_t)(bits))); }
    vf5 = a4v::castsi128_ps(A4_R128(ADD32(v1, 0)));
    vf6 = a4v::castsi128_ps(a4::mk128(h2, v0));
    { a4::vec4 res = a4v::mul(vf5, a4v::bcast<0>(vf6)); a4::q128 mask = a4v::set_epi32(0, -1, -1, -1); vf5 = a4v::blendv_ps(vf5, res, a4v::castsi128_ps(mask)); }
    A4_W128(ADD32((uint32_t)s4, 0), a4v::castps_si128(vf5));
    vf4 = a4v::castsi128_ps(A4_R128(ADD32((uint32_t)s4, 0)));
    vf5 = a4v::castsi128_ps(A4_R128(ADD32((uint32_t)a0, 0)));
    { a4::vec4 res = a4v::mul(vf4, vf5); a4::q128 mask = a4v::set_epi32(0, -1, -1, -1); vf5 = a4v::blend(vf5, res, a4v::castsi128_ps(mask)); }
    { a4::vec4 res = a4v::addK<1, 1>(vf5, a4v::bcast<1>(vf5)); a4::q128 mask = a4v::set_epi32(0, 0, 0, -1); vf5 = a4v::blendv_ps(vf5, res, a4v::castsi128_ps(mask)); }
    { a4::vec4 res = a4v::addK<1, 1>(vf5, a4v::bcast<2>(vf5)); a4::q128 mask = a4v::set_epi32(0, 0, 0, -1); vf5 = a4v::blendv_ps(vf5, res, a4v::castsi128_ps(mask)); }
    { a4::vec4 res = a4v::subK<1, 1>(vf0, a4v::bcast<3>(vf0)); a4::q128 mask = a4v::set_epi32(-1, -1, -1, -1); vf6 = a4v::blendv_ps(vf6, res, a4v::castsi128_ps(mask)); }
    { a4::vec4 res = a4v::subK<1, 1>(vf6, a4v::bcast<0>(vf5)); a4::q128 mask = a4v::set_epi32(-1, 0, 0, 0); vf4 = a4v::blendv_ps(vf4, res, a4v::castsi128_ps(mask)); }
    A4_W128(ADD32((uint32_t)s4, 0), a4v::castps_si128(vf4));
    (v0 = (uint64_t)((uint64_t)s2 + (uint64_t)0ULL));
L_111a708:;
    { (sp = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)sp, 160)))); if (xo) { if (a3xw0) xo[0] = a1_bits(f20); xo[1] = a1; if (a3xw2) xo[2] = a2; xo[3] = a3; xo[4] = t0; if (a3xw5) xo[5] = t1; if (a3xw6) xo[6] = t2; if (a3xw7) xo[7] = t3; } io_vf4 = vf4; io_vf5 = vf5; io_vf6 = vf6; io_vf7 = vf7; io_vf8 = vf8; io_vf10 = vf10; io_vf14 = vf14; io_vf15 = vf15; io_vf16 = vf16; io_vf17 = vf17; io_vf18 = vf18; io_vf29 = vf29; io_vf30 = vf30; io_vf31 = vf31; io_vacc = vacc; io_vq = vq; io_vstat = vstat; io_h2 = h2; io_h5 = h5; io_h9 = h9; io_h11 = h11; return a1::RetVF{(uint64_t)v0, f0}; }
    __builtin_unreachable();
}

// ClipCollisionDB (0x111b9f0): typed by tools/typed_lift2.py (round A6; guest frame calls, extra outputs xo[0]=f20 xo[1]=a1 xo[2]=a2 xo[3]=a3 xo[4]=t0 xo[5]=t1 xo[6]=t2 xo[7]=t3; VU0 in vf0 vf6 vf7 vf8 vf9 vf10 vf11 vf18 vf29 vf31 vacc vstat h8 h9 h11 h29, out vf4 vf5 vf6 vf7 vf8 vf9 vf10 vf11 vf14 vf15 vf16 vf17 vf18 vf29 vf30 vf31 vacc vq vstat h2 h3 h5 h9 h11)
inline a1::RetVF ClipCollisionDB(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint64_t a0, uint64_t a1, uint64_t a2, uint64_t a3, uint64_t t0, uint64_t t2, uint64_t sp, float f12, float f13, float f14, float f15, float f16, float f17, float f18, float f19, float f20, const a4::vec4 &io_vf0, a4::vec4 &io_vf4, a4::vec4 &io_vf5, a4::vec4 &io_vf6, a4::vec4 &io_vf7, a4::vec4 &io_vf8, a4::vec4 &io_vf9, a4::vec4 &io_vf10, a4::vec4 &io_vf11, a4::vec4 &io_vf14, a4::vec4 &io_vf15, a4::vec4 &io_vf16, a4::vec4 &io_vf17, a4::vec4 &io_vf18, a4::vec4 &io_vf29, a4::vec4 &io_vf30, a4::vec4 &io_vf31, a4::vec4 &io_vacc, float &io_vq, uint16_t &io_vstat, uint64_t &io_h2, uint64_t &io_h3, uint64_t &io_h5, uint64_t io_h8, uint64_t &io_h9, uint64_t &io_h11, uint64_t io_h29, uint64_t *xo = nullptr)
{
    float f0 = 0.0f;
    float f1 = 0.0f;
    float f2 = 0.0f;
    float f3 = 0.0f;
    uint32_t at = 0;
    uint64_t v0 = 0;
    uint32_t v1 = 0;
    uint64_t t1 = 0;
    uint64_t t3 = 0;
    uint32_t t4 = 0;
    uint32_t t5 = 0;
    uint64_t s0 = 0;
    uint64_t s1 = 0;
    uint64_t s2 = 0;
    uint64_t s3 = 0;
    uint64_t s4 = 0;
    uint64_t s5 = 0;
    uint64_t s6 = 0;
    uint64_t s7 = 0;
    uint32_t fp = 0;
    uint32_t fcr31 = 0;
    a4::vec4 vf0 = io_vf0;
    a4::vec4 vf4 = io_vf4;
    a4::vec4 vf5 = io_vf5;
    a4::vec4 vf6 = io_vf6;
    a4::vec4 vf7 = io_vf7;
    a4::vec4 vf8 = io_vf8;
    a4::vec4 vf9 = io_vf9;
    a4::vec4 vf10 = io_vf10;
    a4::vec4 vf11 = io_vf11;
    a4::vec4 vf14 = io_vf14;
    a4::vec4 vf15 = io_vf15;
    a4::vec4 vf16 = io_vf16;
    a4::vec4 vf17 = io_vf17;
    a4::vec4 vf18 = io_vf18;
    a4::vec4 vf29 = io_vf29;
    a4::vec4 vf30 = io_vf30;
    a4::vec4 vf31 = io_vf31;
    a4::vec4 vacc = io_vacc;
    float vq = io_vq;
    uint16_t vstat = io_vstat;
    uint64_t h2 = io_h2;
    uint64_t h3 = io_h3;
    uint64_t h5 = io_h5;
    uint64_t h8 = io_h8;
    uint64_t h9 = io_h9;
    uint64_t h11 = io_h11;
    uint64_t h29 = io_h29;
    bool a3xw0 = false;
    bool a3xw2 = false;
    bool a3xw4 = false;
    bool a3xw5 = false;
    bool a3xw6 = false;
    (sp = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)sp, 4294967056))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)255 << 16))));
    (v0 = (uint64_t)(RC_U64X(v0) | (uint64_t)(uint16_t)65535));
    (s4 = (uint64_t)((uint64_t)a3 + (uint64_t)0ULL));
    (s5 = (uint64_t)((uint64_t)a0 + (uint64_t)0ULL));
    (s6 = (uint64_t)((uint64_t)a1 + (uint64_t)0ULL));
    (s7 = (uint64_t)((uint64_t)a2 + (uint64_t)0ULL));
    (a3 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)s4, 4))));
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)s4, 0))));
    (v1 = (uint32_t)((int32_t)((int32_t)SRL32((uint32_t)a3, 24))));
    (s3 = (uint64_t)(RC_U64X(a3) & RC_U64X(v0)));
    { (s1 = (uint64_t)(a1_sx64(v1) & (uint64_t)(uint16_t)127)); A3_CHARGE(); v0 = GetLayoutClass(ram, rt, a0, f12, f13, f14, f15, f16, f17, f18, f19, h2); }
    (t3 = (uint64_t)((uint64_t)v0 + (uint64_t)0ULL));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)s4, 4))));
    { const bool c = (int32_t)v0 >= 0; (v1 = (uint32_t)((int32_t)((int32_t)SLL32((uint32_t)s1, 1)))); if (c) goto L_111bb40; }
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)t3, 372))));
    (v1 = (uint32_t)((int32_t)((int32_t)ADD32(v1, (uint32_t)s1))));
    (a1 = (uint64_t)((int64_t)(int32_t)((int32_t)SLL32((uint32_t)s3, 4))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)a0, 36))));
    v1 = v1 << 5;
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v0, v1))));
    (s2 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v0, 48))));
    (v1 = (uint32_t)((int32_t)((int32_t)A1_R32B((uint32_t)s2, 8))));
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)s2, 20))));
    (v1 = (uint32_t)((int32_t)((int32_t)ADD32(v1, (uint32_t)s3))));
    (v0 = (uint64_t)(uint32_t)((uint8_t)A1_R8B(v1, 0)));
    { const bool c = (int64_t)v0 != 0; (s0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)a0, (uint32_t)a1)))); if (c) goto L_111bc58; }
    (a2 = (uint64_t)((uint64_t)t3 + (uint64_t)0ULL)); a3xw2 = true;
    (a3 = (uint64_t)((uint64_t)s1 + (uint64_t)0ULL));
    (a0 = (uint64_t)((uint64_t)sp + (uint64_t)0ULL));
    (a1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)sp, 16))));
    { (t0 = (uint64_t)((uint64_t)s3 + (uint64_t)0ULL)); a3xw4 = true; A3_CHARGE(); uint64_t a7c[9] = {0u, 0u, 0u, 0u, 0u, 0u, 0u, 0u, 0u}; v0 = manager_sub_01114C70(ram, rt, a0, a1, a2, a3, t0, (uint32_t)sp, f12, f13, f14, f15, f16, f17, f18, f19, f20, vf8, vf10, vf14, vf15, vf16, vf17, vf29, vf30, vf31, vacc, h5, h9, h29, a7c); f20 = a1_flt((uint32_t)a7c[1]); t3 = a7c[8]; a3xw0 = true; }
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)sp, 0))));
    vf29 = a4v::castsi128_ps(A4_R128(ADD32((uint32_t)v0, 0)));
    (v1 = (uint32_t)((int32_t)((int32_t)A1_R32B((uint32_t)sp, 4))));
    vf30 = a4v::castsi128_ps(A4_R128(ADD32(v1, 0)));
    { a4::vec4 res = a4v::sub(vf29, vf30); a4::q128 mask = a4v::set_epi32(-1, -1, -1, -1); vf31 = a4v::blend(vf31, res, a4v::castsi128_ps(mask)); }
    { a4::q128 mask = a4v::set_epi32(-1, -1, -1, -1); vf18 = a4v::blendv_ps(vf18, vf31, a4v::castsi128_ps(mask)); }
    { a4::q128 mask = a4v::set_epi32(-1, -1, -1, -1); vf29 = a4v::blendv_ps(vf29, vf30, a4v::castsi128_ps(mask)); }
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)sp, 8))));
    vf30 = a4v::castsi128_ps(A4_R128(ADD32((uint32_t)v0, 0)));
    { a4::vec4 res = a4v::sub(vf29, vf30); a4::q128 mask = a4v::set_epi32(-1, -1, -1, -1); vf31 = a4v::blend(vf31, res, a4v::castsi128_ps(mask)); }
    { a4::q128 mask = a4v::set_epi32(-1, -1, -1, -1); vf29 = a4v::blendv_ps(vf29, vf18, a4v::castsi128_ps(mask)); }
    { a4::q128 mask = a4v::set_epi32(-1, -1, -1, -1); vf30 = a4v::blendv_ps(vf30, vf31, a4v::castsi128_ps(mask)); }
    { a4::vec4 fs_yzx = a4v::shuffle_ps<0xC9>(vf29, vf29); a4::vec4 ft_zxy = a4v::shuffle_ps<0xD2>(vf30, vf30); a4::vec4 res = a4v::mulK<1, 1>(fs_yzx, ft_zxy); vacc = a4v::dest<1, 1, 1, 0>(vacc, res); }
    { a4::vec4 fs_yzx = a4v::shuffle_ps<0xC9>(vf30, vf30); a4::vec4 ft_zxy = a4v::shuffle_ps<0xD2>(vf29, vf29); a4::vec4 mul_res = a4v::mulK<1, 1>(fs_yzx, ft_zxy); a4::vec4 res = a4v::subK<1, 1>(vacc, mul_res); a4::q128 mask = a4v::set_epi32(0, -1, -1, -1); vf31 = a4v::blendv_ps(vf31, res, a4v::castsi128_ps(mask)); }
    { a4::vec4 res = a4v::subK<1, 1>(vf31, vf31); a4::q128 mask = a4v::set_epi32(-1, 0, 0, 0); vf31 = a4v::blend(vf31, res, a4v::castsi128_ps(mask)); }
    { a4::q128 mask = a4v::set_epi32(-1, -1, -1, -1); vf29 = a4v::blendv_ps(vf29, vf31, a4v::castsi128_ps(mask)); }
    { a4::vec4 res = a4v::mulK<1, 1>(vf29, vf29); a4::q128 mask = a4v::set_epi32(0, -1, -1, -1); vf7 = a4v::blend(vf7, res, a4v::castsi128_ps(mask)); }
    { a4::vec4 res = a4v::mulK<1, 1>(vf0, a4v::bcast<0>(vf7)); vacc = a4v::dest<0, 0, 0, 1>(vacc, res); }
    { a4::vec4 mul_res = a4v::mulK<1, 1>(vf0, a4v::bcast<1>(vf7)); a4::vec4 res = a4v::addK<1, 1>(vacc, mul_res); vacc = a4v::dest<0, 0, 0, 1>(vacc, res); }
    { a4::vec4 mul_res = a4v::mulK<1, 1>(vf0, a4v::bcast<2>(vf7)); a4::vec4 res = a4v::addK<1, 1>(vacc, mul_res); a4::q128 mask = a4v::set_epi32(-1, 0, 0, 0); vf7 = a4v::blendv_ps(vf7, res, a4v::castsi128_ps(mask)); }
    { float fs = a4v::cvtss_f32(a4v::shuffle_ps<0x03>(vf0, vf0)); float ft = a4v::cvtss_f32(a4v::shuffle_ps<0x03>(vf7, vf7)); vq = a4v::rsqrt(vstat, fs, ft); }
    { a4::vec4 res = a4v::mul(vf29, a4v::set1_ps(vq)); a4::q128 mask = a4v::set_epi32(0, -1, -1, -1); vf29 = a4v::blendv_ps(vf29, res, a4v::castsi128_ps(mask)); }
    { a4::q128 mask = a4v::set_epi32(-1, -1, -1, -1); vf31 = a4v::blendv_ps(vf31, vf29, a4v::castsi128_ps(mask)); }
    A4_W128(ADD32((uint32_t)s0, 0), a4v::castps_si128(vf31));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)sp, 0))));
    vf4 = a4v::castsi128_ps(A4_R128(ADD32((uint32_t)s0, 0)));
    vf5 = a4v::castsi128_ps(A4_R128(ADD32((uint32_t)v0, 0)));
    { a4::vec4 res = a4v::mul(vf4, vf5); a4::q128 mask = a4v::set_epi32(0, -1, -1, -1); vf5 = a4v::blend(vf5, res, a4v::castsi128_ps(mask)); }
    { a4::vec4 res = a4v::addK<1, 1>(vf5, a4v::bcast<1>(vf5)); a4::q128 mask = a4v::set_epi32(0, 0, 0, -1); vf5 = a4v::blendv_ps(vf5, res, a4v::castsi128_ps(mask)); }
    { a4::vec4 res = a4v::addK<1, 1>(vf5, a4v::bcast<2>(vf5)); a4::q128 mask = a4v::set_epi32(0, 0, 0, -1); vf5 = a4v::blendv_ps(vf5, res, a4v::castsi128_ps(mask)); }
    { a4::vec4 res = a4v::subK<1, 1>(vf0, a4v::bcast<3>(vf0)); a4::q128 mask = a4v::set_epi32(-1, -1, -1, -1); vf6 = a4v::blendv_ps(vf6, res, a4v::castsi128_ps(mask)); }
    { a4::vec4 res = a4v::subK<1, 1>(vf6, a4v::bcast<0>(vf5)); a4::q128 mask = a4v::set_epi32(-1, 0, 0, 0); vf4 = a4v::blendv_ps(vf4, res, a4v::castsi128_ps(mask)); }
    A4_W128(ADD32((uint32_t)s0, 0), a4v::castps_si128(vf4));
    (a0 = (uint64_t)((uint64_t)s2 + (uint64_t)0ULL));
    (a1 = (uint64_t)((uint64_t)s3 + (uint64_t)0ULL));
    goto L_111bc50;
L_111bb40:;
    (v1 = (uint32_t)((int32_t)((int32_t)A1_R32B((uint32_t)t3, 388))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)SLL32((uint32_t)s1, 3))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v0, (uint32_t)s1))));
    (a1 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)t3, 372))));
    (t2 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B(v1, 32)))); a3xw6 = true;
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)SLL32((uint32_t)v0, 4))));
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)SLL32((uint32_t)s3, 2))));
    (s2 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)a1, 88))));
    (t1 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)t2, 8)))); a3xw5 = true;
    (a1 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)s2, 8))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v0, (uint32_t)t1))));
    (a2 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)s2, 20)))); a3xw2 = true;
    (v1 = (uint32_t)((int32_t)((int32_t)A1_R32B((uint32_t)v0, 84))));
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)a0, v1))));
    (s1 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)a0, 0))));
    (a1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)a1, (uint32_t)s1))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)SLL32((uint32_t)s1, 4))));
    (v1 = (uint32_t)((uint8_t)A1_R8B((uint32_t)a1, 0)));
    { const bool c = v1 != 0; (s0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)a2, (uint32_t)v0)))); if (c) goto L_111bc58; }
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)t3, 340))));
    (t3 = (uint64_t)((int64_t)(int32_t)((int32_t)SLL32((uint32_t)s1, 6))));
    (a2 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)v0, 32)))); a3xw2 = true;
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)sp, 32))));
    (v1 = (uint32_t)((int32_t)((int32_t)A1_R32B((uint32_t)t2, 24))));
    (a1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)sp, 48))));
    (t0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)v0, 8)))); a3xw4 = true;
    (a3 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)a2, 36))));
    (a2 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)a2, 40)))); a3xw2 = true;
    (t2 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)t2, 20)))); a3xw6 = true;
    { (t3 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(v1, (uint32_t)t3)))); A3_CHARGE(); uint64_t a7c[7] = {0u, 0u, 0u, 0u, 0u, 0u, t3}; v0 = manager_sub_01116108(ram, rt, a0, a1, a2, a3, t0, t1, t2, t3, sp, f12, f13, f14, f15, f16, f17, f18, f19, vf8, vf10, vf14, vf15, vf16, vf17, vf29, vf30, vf31, vacc, h11, a7c); t3 = a7c[6]; }
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)sp, 32))));
    vf29 = a4v::castsi128_ps(A4_R128(ADD32((uint32_t)v0, 0)));
    (v1 = (uint32_t)((int32_t)((int32_t)A1_R32B((uint32_t)sp, 36))));
    vf30 = a4v::castsi128_ps(A4_R128(ADD32(v1, 0)));
    { a4::vec4 res = a4v::sub(vf29, vf30); a4::q128 mask = a4v::set_epi32(-1, -1, -1, -1); vf31 = a4v::blend(vf31, res, a4v::castsi128_ps(mask)); }
    { a4::q128 mask = a4v::set_epi32(-1, -1, -1, -1); vf18 = a4v::blendv_ps(vf18, vf31, a4v::castsi128_ps(mask)); }
    { a4::q128 mask = a4v::set_epi32(-1, -1, -1, -1); vf29 = a4v::blendv_ps(vf29, vf30, a4v::castsi128_ps(mask)); }
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)sp, 40))));
    vf30 = a4v::castsi128_ps(A4_R128(ADD32((uint32_t)v0, 0)));
    { a4::vec4 res = a4v::sub(vf29, vf30); a4::q128 mask = a4v::set_epi32(-1, -1, -1, -1); vf31 = a4v::blend(vf31, res, a4v::castsi128_ps(mask)); }
    { a4::q128 mask = a4v::set_epi32(-1, -1, -1, -1); vf29 = a4v::blendv_ps(vf29, vf18, a4v::castsi128_ps(mask)); }
    { a4::q128 mask = a4v::set_epi32(-1, -1, -1, -1); vf30 = a4v::blendv_ps(vf30, vf31, a4v::castsi128_ps(mask)); }
    { a4::vec4 fs_yzx = a4v::shuffle_ps<0xC9>(vf29, vf29); a4::vec4 ft_zxy = a4v::shuffle_ps<0xD2>(vf30, vf30); a4::vec4 res = a4v::mulK<1, 1>(fs_yzx, ft_zxy); vacc = a4v::dest<1, 1, 1, 0>(vacc, res); }
    { a4::vec4 fs_yzx = a4v::shuffle_ps<0xC9>(vf30, vf30); a4::vec4 ft_zxy = a4v::shuffle_ps<0xD2>(vf29, vf29); a4::vec4 mul_res = a4v::mulK<1, 1>(fs_yzx, ft_zxy); a4::vec4 res = a4v::subK<1, 1>(vacc, mul_res); a4::q128 mask = a4v::set_epi32(0, -1, -1, -1); vf31 = a4v::blendv_ps(vf31, res, a4v::castsi128_ps(mask)); }
    { a4::vec4 res = a4v::subK<1, 1>(vf31, vf31); a4::q128 mask = a4v::set_epi32(-1, 0, 0, 0); vf31 = a4v::blend(vf31, res, a4v::castsi128_ps(mask)); }
    { a4::q128 mask = a4v::set_epi32(-1, -1, -1, -1); vf29 = a4v::blendv_ps(vf29, vf31, a4v::castsi128_ps(mask)); }
    { a4::vec4 res = a4v::mulK<1, 1>(vf29, vf29); a4::q128 mask = a4v::set_epi32(0, -1, -1, -1); vf7 = a4v::blend(vf7, res, a4v::castsi128_ps(mask)); }
    { a4::vec4 res = a4v::mulK<1, 1>(vf0, a4v::bcast<0>(vf7)); vacc = a4v::dest<0, 0, 0, 1>(vacc, res); }
    { a4::vec4 mul_res = a4v::mulK<1, 1>(vf0, a4v::bcast<1>(vf7)); a4::vec4 res = a4v::addK<1, 1>(vacc, mul_res); vacc = a4v::dest<0, 0, 0, 1>(vacc, res); }
    { a4::vec4 mul_res = a4v::mulK<1, 1>(vf0, a4v::bcast<2>(vf7)); a4::vec4 res = a4v::addK<1, 1>(vacc, mul_res); a4::q128 mask = a4v::set_epi32(-1, 0, 0, 0); vf7 = a4v::blendv_ps(vf7, res, a4v::castsi128_ps(mask)); }
    { float fs = a4v::cvtss_f32(a4v::shuffle_ps<0x03>(vf0, vf0)); float ft = a4v::cvtss_f32(a4v::shuffle_ps<0x03>(vf7, vf7)); vq = a4v::rsqrt(vstat, fs, ft); }
    { a4::vec4 res = a4v::mul(vf29, a4v::set1_ps(vq)); a4::q128 mask = a4v::set_epi32(0, -1, -1, -1); vf29 = a4v::blendv_ps(vf29, res, a4v::castsi128_ps(mask)); }
    { a4::q128 mask = a4v::set_epi32(-1, -1, -1, -1); vf31 = a4v::blendv_ps(vf31, vf29, a4v::castsi128_ps(mask)); }
    A4_W128(ADD32((uint32_t)s0, 0), a4v::castps_si128(vf31));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)sp, 32))));
    vf4 = a4v::castsi128_ps(A4_R128(ADD32((uint32_t)s0, 0)));
    vf5 = a4v::castsi128_ps(A4_R128(ADD32((uint32_t)v0, 0)));
    { a4::vec4 res = a4v::mul(vf4, vf5); a4::q128 mask = a4v::set_epi32(0, -1, -1, -1); vf5 = a4v::blend(vf5, res, a4v::castsi128_ps(mask)); }
    { a4::vec4 res = a4v::addK<1, 1>(vf5, a4v::bcast<1>(vf5)); a4::q128 mask = a4v::set_epi32(0, 0, 0, -1); vf5 = a4v::blendv_ps(vf5, res, a4v::castsi128_ps(mask)); }
    { a4::vec4 res = a4v::addK<1, 1>(vf5, a4v::bcast<2>(vf5)); a4::q128 mask = a4v::set_epi32(0, 0, 0, -1); vf5 = a4v::blendv_ps(vf5, res, a4v::castsi128_ps(mask)); }
    { a4::vec4 res = a4v::subK<1, 1>(vf0, a4v::bcast<3>(vf0)); a4::q128 mask = a4v::set_epi32(-1, -1, -1, -1); vf6 = a4v::blendv_ps(vf6, res, a4v::castsi128_ps(mask)); }
    { a4::vec4 res = a4v::subK<1, 1>(vf6, a4v::bcast<0>(vf5)); a4::q128 mask = a4v::set_epi32(-1, 0, 0, 0); vf4 = a4v::blendv_ps(vf4, res, a4v::castsi128_ps(mask)); }
    A4_W128(ADD32((uint32_t)s0, 0), a4v::castps_si128(vf4));
    (a0 = (uint64_t)((uint64_t)s2 + (uint64_t)0ULL));
    (a1 = (uint64_t)((uint64_t)s1 + (uint64_t)0ULL));
L_111bc50:;
    { A3_CHARGE(); uint64_t a7c[5] = {0u, 0u, 0u, 0u, 0u}; v0 = EntryModifiedManager(ram, rt, a0, (uint32_t)a1, a7c); a2 = a7c[0]; a3 = a7c[1]; t0 = a7c[2]; t1 = a7c[3]; t2 = a7c[4]; a3xw2 = true; a3xw4 = true; a3xw5 = true; a3xw6 = true; }
L_111bc58:;
    (a0 = (uint64_t)((uint64_t)s0 + (uint64_t)0ULL));
    vf5 = a4v::castsi128_ps(A4_R128(ADD32((uint32_t)a0, 0)));
    vf6 = a4v::castsi128_ps(A4_R128(ADD32((uint32_t)s6, 0)));
    { a4::vec4 res = a4v::addK<1, 1>(vf0, a4v::bcast<3>(vf0)); a4::q128 mask = a4v::set_epi32(0, 0, 0, -1); vf7 = a4v::blendv_ps(vf7, res, a4v::castsi128_ps(mask)); }
    { a4::vec4 res = a4v::mul(vf5, vf6); a4::q128 mask = a4v::set_epi32(0, -1, -1, -1); vf6 = a4v::blend(vf6, res, a4v::castsi128_ps(mask)); }
    { a4::vec4 res = a4v::addK<1, 1>(vf0, a4v::bcast<0>(vf6)); vacc = a4v::dest<1, 0, 0, 0>(vacc, res); }
    { a4::vec4 mul_res = a4v::mulK<1, 1>(vf7, a4v::bcast<1>(vf6)); a4::vec4 res = a4v::addK<1, 1>(vacc, mul_res); vacc = a4v::dest<1, 0, 0, 0>(vacc, res); }
    { a4::vec4 mul_res = a4v::mulK<1, 1>(vf7, a4v::bcast<2>(vf6)); a4::vec4 res = a4v::addK<1, 1>(vacc, mul_res); a4::q128 mask = a4v::set_epi32(0, 0, 0, -1); vf6 = a4v::blendv_ps(vf6, res, a4v::castsi128_ps(mask)); }
    { a4::vec4 res = a4v::addK<1, 0>(vf6, a4v::bcast<3>(vf5)); a4::q128 mask = a4v::set_epi32(0, 0, 0, -1); vf6 = a4v::blendv_ps(vf6, res, a4v::castsi128_ps(mask)); }
    do { const a4::q128 _v = (a4v::castps_si128(vf6)); h2 = a4::hi64(_v); std::memcpy(&v0, &_v, 8); } while (0);
    { uint32_t bits = (uint32_t)v0; std::memcpy(&f1, &bits, sizeof(bits)); }
    { uint32_t bits = 0u; std::memcpy(&f0, &bits, sizeof(bits)); }
    fcr31 = (a1f::lt(f1, f0)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
    { const bool c = (fcr31 & 0x800000u) != 0u; (v0 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL)); if (c) goto L_111c044; }
    vf5 = a4v::castsi128_ps(A4_R128(ADD32((uint32_t)a0, 0)));
    vf6 = a4v::castsi128_ps(A4_R128(ADD32((uint32_t)s7, 0)));
    { a4::vec4 res = a4v::addK<1, 1>(vf0, a4v::bcast<3>(vf0)); a4::q128 mask = a4v::set_epi32(0, 0, 0, -1); vf7 = a4v::blendv_ps(vf7, res, a4v::castsi128_ps(mask)); }
    { a4::vec4 res = a4v::mul(vf5, vf6); a4::q128 mask = a4v::set_epi32(0, -1, -1, -1); vf6 = a4v::blend(vf6, res, a4v::castsi128_ps(mask)); }
    { a4::vec4 res = a4v::addK<1, 1>(vf0, a4v::bcast<0>(vf6)); vacc = a4v::dest<1, 0, 0, 0>(vacc, res); }
    { a4::vec4 mul_res = a4v::mulK<1, 1>(vf7, a4v::bcast<1>(vf6)); a4::vec4 res = a4v::addK<1, 1>(vacc, mul_res); vacc = a4v::dest<1, 0, 0, 0>(vacc, res); }
    { a4::vec4 mul_res = a4v::mulK<1, 1>(vf7, a4v::bcast<2>(vf6)); a4::vec4 res = a4v::addK<1, 1>(vacc, mul_res); a4::q128 mask = a4v::set_epi32(0, 0, 0, -1); vf6 = a4v::blendv_ps(vf6, res, a4v::castsi128_ps(mask)); }
    { a4::vec4 res = a4v::addK<1, 0>(vf6, a4v::bcast<3>(vf5)); a4::q128 mask = a4v::set_epi32(0, 0, 0, -1); vf6 = a4v::blendv_ps(vf6, res, a4v::castsi128_ps(mask)); }
    do { const a4::q128 _v = (a4v::castps_si128(vf6)); h2 = a4::hi64(_v); std::memcpy(&v0, &_v, 8); } while (0);
    { uint32_t bits = (uint32_t)v0; std::memcpy(&f2, &bits, sizeof(bits)); }
    { uint32_t bits = 0u; std::memcpy(&f0, &bits, sizeof(bits)); }
    fcr31 = (a1f::lt(f0, f2)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
    { const bool c = (fcr31 & 0x800000u) != 0u; (v0 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL)); if (c) goto L_111c044; }
    f1 = a1f::sub(f2, f1);
    fcr31 = (a1f::lt(f1, f0)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
    if ((fcr31 & 0x800000u) == 0u) goto L_111bd00;
    (at = (uint32_t)((int32_t)((int32_t)((uint32_t)47619 << 16))));
    (at = (uint32_t)(a1_sx64(at) | (uint64_t)(uint16_t)4717));
    { uint32_t bits = a7_opq(at); std::memcpy(&f0, &bits, sizeof(bits)); }
    fcr31 = (a1f::lt(f0, f1)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
    if ((fcr31 & 0x800000u) != 0u) { do { const a4::q128 _v = (A4_R128(ADD32((uint32_t)s6, 0))); h2 = a4::hi64(_v); std::memcpy(&v0, &_v, 8); } while (0); goto L_111bd1c; }
    goto L_111bd28;
L_111bd00:;
    (at = (uint32_t)((int32_t)((int32_t)((uint32_t)14851 << 16))));
    (at = (uint32_t)(a1_sx64(at) | (uint64_t)(uint16_t)4717));
    { uint32_t bits = a7_opq(at); std::memcpy(&f0, &bits, sizeof(bits)); }
    fcr31 = (a1f::lt(f1, f0)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
    if ((fcr31 & 0x800000u) == 0u) goto L_111bd28;
    do { const a4::q128 _v = (A4_R128(ADD32((uint32_t)s6, 0))); h2 = a4::hi64(_v); std::memcpy(&v0, &_v, 8); } while (0);
L_111bd1c:;
    A4_W128(ADD32((uint32_t)sp, 0), a4::mk128(h2, v0));
    goto L_111bd54;
L_111bd28:;
    f0 = a1f::div(fcr31, f2, f1);
    { uint32_t bits; std::memcpy(&bits, &f0, sizeof(bits)); (t0 = (uint64_t)((int64_t)(int32_t)(bits))); } a3xw4 = true;
    vf7 = a4v::castsi128_ps(a4::mk128(h8, t0));
    vf5 = a4v::castsi128_ps(A4_R128(ADD32((uint32_t)s6, 0)));
    vf6 = a4v::castsi128_ps(A4_R128(ADD32((uint32_t)s7, 0)));
    { a4::vec4 res = a4v::subK<1, 0>(vf0, a4v::bcast<0>(vf7)); a4::q128 mask = a4v::set_epi32(-1, 0, 0, 0); vf8 = a4v::blendv_ps(vf8, res, a4v::castsi128_ps(mask)); }
    { a4::vec4 res = a4v::mul(vf5, a4v::bcast<0>(vf7)); vacc = a4v::dest<1, 1, 1, 0>(vacc, res); }
    { a4::vec4 mul_res = a4v::mulK<0, 1>(vf6, a4v::bcast<3>(vf8)); a4::vec4 res = a4v::addK<1, 1>(vacc, mul_res); a4::q128 mask = a4v::set_epi32(0, -1, -1, -1); vf5 = a4v::blendv_ps(vf5, res, a4v::castsi128_ps(mask)); }
    A4_W128(ADD32((uint32_t)sp, 0), a4v::castps_si128(vf5));
L_111bd54:;
    (s0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)sp, 16))));
    (a1 = (uint64_t)((uint64_t)s4 + (uint64_t)0ULL));
    { (a0 = (uint64_t)((uint64_t)RC_U64X(s0) + (uint64_t)0ULL)); A3_CHARGE(); uint64_t a7c[10] = {0u, 0u, 0u, 0u, 0u, 0u, 0u, 0u, t2, t3}; v0 = GetCollisionDBSurfBoundary(ram, rt, a0, a1, t3, sp, f2, f12, f13, f14, f15, f16, f17, f18, f19, vf0, vf4, vf5, vf6, vf7, vf8, vf10, vf14, vf15, vf16, vf17, vf29, vf30, vf31, vacc, h2, h3, h8, h11, a7c); a1 = a7c[3]; a2 = a7c[4]; a3 = a7c[5]; t0 = a7c[6]; t1 = a7c[7]; t2 = a7c[8]; t3 = a7c[9]; a3xw2 = true; a3xw4 = true; a3xw5 = true; a3xw6 = true; }
    vf5 = a4v::castsi128_ps(A4_R128(ADD32((uint32_t)s0, 0)));
    vf6 = a4v::castsi128_ps(A4_R128(ADD32((uint32_t)sp, 0)));
    { a4::vec4 res = a4v::sub(vf5, vf6); a4::q128 mask = a4v::set_epi32(-1, -1, -1, -1); vf7 = a4v::blend(vf7, res, a4v::castsi128_ps(mask)); }
    { a4::vec4 res = a4v::mulK<1, 1>(vf7, vf7); a4::q128 mask = a4v::set_epi32(0, -1, -1, -1); vf7 = a4v::blend(vf7, res, a4v::castsi128_ps(mask)); }
    { a4::vec4 res = a4v::addK<1, 1>(vf7, a4v::bcast<1>(vf7)); a4::q128 mask = a4v::set_epi32(0, 0, 0, -1); vf7 = a4v::blendv_ps(vf7, res, a4v::castsi128_ps(mask)); }
    { a4::vec4 res = a4v::addK<1, 1>(vf7, a4v::bcast<2>(vf7)); a4::q128 mask = a4v::set_epi32(0, 0, 0, -1); vf7 = a4v::blendv_ps(vf7, res, a4v::castsi128_ps(mask)); }
    do { const a4::q128 _v = (a4v::castps_si128(vf7)); h2 = a4::hi64(_v); std::memcpy(&v0, &_v, 8); } while (0);
    { uint32_t bits = (uint32_t)v0; std::memcpy(&f1, &bits, sizeof(bits)); }
    { uint32_t bits = A1_R32B((uint32_t)sp, 28); float f; std::memcpy(&f, &bits, sizeof(f)); f0 = f; }
    fcr31 = (a1f::lt(f0, f1)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
    { const bool c = (fcr31 & 0x800000u) != 0u; (v0 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL)); if (c) goto L_111c044; }
    (a1 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)s4, 4))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)255 << 16))));
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)s4, 0))));
    (v0 = (uint64_t)(RC_U64X(v0) | (uint64_t)(uint16_t)65535));
    (v1 = (uint32_t)((int32_t)((int32_t)SRL32((uint32_t)a1, 24))));
    (s1 = (uint64_t)(RC_U64X(a1) & RC_U64X(v0)));
    (s0 = (uint64_t)(a1_sx64(v1) & (uint64_t)(uint16_t)127));
    (fp = (uint32_t)((int32_t)((int32_t)ADD32((uint32_t)sp, 144))));
    (s7 = (uint64_t)((uint64_t)sp + (uint64_t)0ULL));
    (s6 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)sp, 80))));
    (s3 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)sp, 96))));
    { (s2 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)sp, 128)))); A3_CHARGE(); v0 = GetLayoutClass(ram, rt, a0, f12, f13, f14, f15, f16, f17, f18, f19, h2); }
    (a2 = (uint64_t)((uint64_t)v0 + (uint64_t)0ULL)); a3xw2 = true;
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)s4, 4))));
    { const bool c = (int32_t)v0 >= 0; (a0 = (uint64_t)((uint64_t)s2 + (uint64_t)0ULL)); if (c) goto L_111bdf0; }
    (a3 = (uint64_t)((uint64_t)s0 + (uint64_t)0ULL));
    (t0 = (uint64_t)((uint64_t)s1 + (uint64_t)0ULL)); a3xw4 = true;
    { (a1 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL)); A3_CHARGE(); uint64_t a7c[9] = {0u, 0u, 0u, 0u, 0u, t0, 0u, 0u, 0u}; v0 = manager_sub_01114C70(ram, rt, a0, a1, a2, a3, t0, (uint32_t)sp, f12, f13, f14, f15, f16, f17, f18, f19, f20, vf8, vf10, vf14, vf15, vf16, vf17, vf29, vf30, vf31, vacc, h5, h9, h29, a7c); f20 = a1_flt((uint32_t)a7c[1]); a1 = a7c[2]; a2 = a7c[3]; a3 = a7c[4]; t0 = a7c[5]; t1 = a7c[6]; t2 = a7c[7]; t3 = a7c[8]; a3xw0 = true; a3xw2 = true; a3xw4 = true; a3xw5 = true; a3xw6 = true; }
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)sp, 132))));
    goto L_111be4c;
L_111bdf0:;
    (v1 = (uint32_t)((int32_t)((int32_t)A1_R32B((uint32_t)a2, 388))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)SLL32((uint32_t)s0, 3))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v0, (uint32_t)s0))));
    (t2 = (uint64_t)((int64_t)(int32_t)((int32_t)SLL32((uint32_t)s1, 2)))); a3xw6 = true;
    t4 = A1_R32B(v1, 32);
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)SLL32((uint32_t)v0, 4))));
    (t3 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)a2, 340))));
    (t1 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B(t4, 8)))); a3xw5 = true;
    (a1 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL));
    (a2 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)t3, 32)))); a3xw2 = true;
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v0, (uint32_t)t1))));
    (t0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)t3, 8)))); a3xw4 = true;
    (v1 = (uint32_t)((int32_t)((int32_t)A1_R32B((uint32_t)v0, 84))));
    t5 = A1_R32B(t4, 24);
    (t2 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)t2, v1)))); a3xw6 = true;
    (a3 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)a2, 36))));
    (t3 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)t2, 0))));
    (a2 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)a2, 40)))); a3xw2 = true;
    (t2 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B(t4, 20)))); a3xw6 = true;
    (t3 = (uint64_t)((int64_t)(int32_t)((int32_t)SLL32((uint32_t)t3, 6))));
    { (t3 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(t5, (uint32_t)t3)))); A3_CHARGE(); uint64_t a7c[7] = {a1, a2, a3, t0, t1, t2, t3}; v0 = manager_sub_01116108(ram, rt, a0, a1, a2, a3, t0, t1, t2, t3, sp, f12, f13, f14, f15, f16, f17, f18, f19, vf8, vf10, vf14, vf15, vf16, vf17, vf29, vf30, vf31, vacc, h11, a7c); a1 = a7c[0]; a2 = a7c[1]; a3 = a7c[2]; t0 = a7c[3]; t1 = a7c[4]; t2 = a7c[5]; t3 = a7c[6]; a3xw2 = true; a3xw4 = true; a3xw5 = true; a3xw6 = true; }
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)sp, 132))));
L_111be4c:;
    vf29 = a4v::castsi128_ps(A4_R128(ADD32((uint32_t)v0, 0)));
    (v1 = (uint32_t)((int32_t)((int32_t)A1_R32B((uint32_t)sp, 128))));
    vf30 = a4v::castsi128_ps(A4_R128(ADD32(v1, 0)));
    { a4::vec4 res = a4v::sub(vf29, vf30); a4::q128 mask = a4v::set_epi32(-1, -1, -1, -1); vf31 = a4v::blend(vf31, res, a4v::castsi128_ps(mask)); }
    A4_W128(ADD32((uint32_t)s6, 0), a4v::castps_si128(vf31));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)sp, 136))));
    vf29 = a4v::castsi128_ps(A4_R128(ADD32((uint32_t)v0, 0)));
    { a4::vec4 res = a4v::sub(vf29, vf30); a4::q128 mask = a4v::set_epi32(-1, -1, -1, -1); vf31 = a4v::blend(vf31, res, a4v::castsi128_ps(mask)); }
    A4_W128(ADD32((uint32_t)s3, 0), a4v::castps_si128(vf31));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)sp, 128))));
    (v1 = (uint32_t)((int32_t)((int32_t)ADD32((uint32_t)sp, 112))));
    vf5 = a4v::castsi128_ps(A4_R128(ADD32((uint32_t)s7, 0)));
    vf6 = a4v::castsi128_ps(A4_R128(ADD32((uint32_t)v0, 0)));
    { a4::vec4 res = a4v::sub(vf5, vf6); a4::q128 mask = a4v::set_epi32(-1, -1, -1, -1); vf7 = a4v::blend(vf7, res, a4v::castsi128_ps(mask)); }
    A4_W128(ADD32(v1, 0), a4v::castps_si128(vf7));
    vf5 = a4v::castsi128_ps(A4_R128(ADD32((uint32_t)s6, 0)));
    vf6 = a4v::castsi128_ps(A4_R128(ADD32((uint32_t)s3, 0)));
    vf7 = a4v::castsi128_ps(A4_R128(ADD32(v1, 0)));
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
    (at = (uint32_t)((int32_t)((int32_t)((uint32_t)47015 << 16))));
    at = at | 0xC5ACu;
    { uint32_t bits = a7_opq(at); std::memcpy(&f2, &bits, sizeof(bits)); }
    A1_W32(fp, a1_bits(f0));
    { float f = f1; uint32_t bits; std::memcpy(&bits, &f, sizeof(bits)); A1_W32B((uint32_t)sp, 148, bits); }
    f0 = a1_flt(A1_R32(fp));
    fcr31 = (a1f::lt(f0, f2)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
    { const bool c = (fcr31 & 0x800000u) != 0u; v1 = 0u; if (c) goto L_111bf8c; }
    fcr31 = (a1f::lt(f1, f2)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
    if ((fcr31 & 0x800000u) != 0u) goto L_111bf8c;
    f0 = a1f::add(f0, f1);
    (at = (uint32_t)((int32_t)((int32_t)((uint32_t)16256 << 16))));
    { uint32_t bits = a7_opq(at); std::memcpy(&f1, &bits, sizeof(bits)); }
    (at = (uint32_t)((int32_t)((int32_t)((uint32_t)14247 << 16))));
    at = at | 0xC5ACu;
    { uint32_t bits = a7_opq(at); std::memcpy(&f2, &bits, sizeof(bits)); }
    f0 = a1f::subFinite(f0, f1);
    fcr31 = (a1f::lt(f2, f0)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
    { const bool c = (fcr31 & 0x800000u) == 0u; v1 = 1u; if (c) goto L_111bf8c; }
    v1 = 0u;
L_111bf8c:;
    { const bool c = v1 == 0; (v0 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL)); if (c) goto L_111c044; }
    do { const a4::q128 _v = (A4_R128(ADD32((uint32_t)sp, 0))); h2 = a4::hi64(_v); std::memcpy(&v0, &_v, 8); } while (0);
    { uint32_t bits = A1_R32B((uint32_t)sp, 144); float f; std::memcpy(&f, &bits, sizeof(f)); f0 = f; }
    { uint32_t bits = A1_R32B((uint32_t)sp, 148); float f; std::memcpy(&f, &bits, sizeof(f)); f1 = f; }
    A4_W128(ADD32((uint32_t)s5, 64), a4::mk128(h2, v0));
    { float f = f0; uint32_t bits; std::memcpy(&bits, &f, sizeof(bits)); A1_W32B((uint32_t)s5, 8, bits); }
    { float f = f1; uint32_t bits; std::memcpy(&bits, &f, sizeof(bits)); A1_W32B((uint32_t)s5, 12, bits); }
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)s4, 0))));
    A1_W32B((uint32_t)s5, 0, (uint32_t)v0);
    (v1 = (uint32_t)((int32_t)((int32_t)A1_R32B((uint32_t)s4, 4))));
    A1_W32B((uint32_t)s5, 4, v1);
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)s4, 4))));
    { const bool c = (int32_t)v0 >= 0; (s0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)s5, 64)))); if (c) goto L_111c038; }
    { (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)s4, 0)))); A3_CHARGE(); v0 = GetLayoutClass(ram, rt, a0, f12, f13, f14, f15, f16, f17, f18, f19, h2); }
    v1 = 2u;
    A1_W32B((uint32_t)s5, 48, v1);
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)s5, 16))));
    (v1 = (uint32_t)((uint8_t)A1_R8B((uint32_t)s4, 7)));
    (a1 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)v0, 356))));
    v1 = v1 & 0x7Fu;
    v1 = v1 << 6;
    (v1 = (uint32_t)((int32_t)((int32_t)ADD32(v1, (uint32_t)a1))));
    vf8 = a4v::castsi128_ps(A4_R128(ADD32((uint32_t)s0, 0)));
    vf14 = a4v::castsi128_ps(A4_R128(ADD32(v1, 0)));
    vf15 = a4v::castsi128_ps(A4_R128(ADD32(v1, 16)));
    vf16 = a4v::castsi128_ps(A4_R128(ADD32(v1, 32)));
    vf17 = a4v::castsi128_ps(A4_R128(ADD32(v1, 48)));
    { a4::vec4 res = a4v::mul(vf14, a4v::bcast<0>(vf8)); vacc = a4v::dest<1, 1, 1, 1>(vacc, res); }
    { a4::vec4 mul_res = a4v::mul(vf15, a4v::bcast<1>(vf8)); a4::vec4 res = a4v::addK<1, 1>(vacc, mul_res); vacc = a4v::dest<1, 1, 1, 1>(vacc, res); }
    { a4::vec4 mul_res = a4v::mul(vf16, a4v::bcast<2>(vf8)); a4::vec4 res = a4v::addK<1, 1>(vacc, mul_res); vacc = a4v::dest<1, 1, 1, 1>(vacc, res); }
    { a4::vec4 mul_res = a4v::mul(vf17, a4v::bcast<3>(vf8)); a4::vec4 res = a4v::addK<1, 1>(vacc, mul_res); a4::q128 mask = a4v::set_epi32(-1, -1, -1, -1); vf10 = a4v::blendv_ps(vf10, res, a4v::castsi128_ps(mask)); }
    A4_W128(ADD32((uint32_t)a0, 0), a4v::castps_si128(vf10));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v0, 160))));
    vf5 = a4v::castsi128_ps(A4_R128(ADD32((uint32_t)a0, 0)));
    vf6 = a4v::castsi128_ps(A4_R128(ADD32((uint32_t)v0, 0)));
    { a4::vec4 res = a4v::mul(vf5, vf6); a4::q128 mask = a4v::set_epi32(0, -1, -1, -1); vf5 = a4v::blend(vf5, res, a4v::castsi128_ps(mask)); }
    A4_W128(ADD32((uint32_t)a0, 0), a4v::castps_si128(vf5));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 1))));
    goto L_111c044;
L_111c038:;
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 3))));
    A1_W32B((uint32_t)s5, 48, (uint32_t)v0);
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 1))));
L_111c044:;
    { (sp = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)sp, 240)))); if (xo) { if (a3xw0) xo[0] = a1_bits(f20); xo[1] = a1; if (a3xw2) xo[2] = a2; xo[3] = a3; if (a3xw4) xo[4] = t0; if (a3xw5) xo[5] = t1; if (a3xw6) xo[6] = t2; xo[7] = t3; } io_vf4 = vf4; io_vf5 = vf5; io_vf6 = vf6; io_vf7 = vf7; io_vf8 = vf8; io_vf9 = vf9; io_vf10 = vf10; io_vf11 = vf11; io_vf14 = vf14; io_vf15 = vf15; io_vf16 = vf16; io_vf17 = vf17; io_vf18 = vf18; io_vf29 = vf29; io_vf30 = vf30; io_vf31 = vf31; io_vacc = vacc; io_vq = vq; io_vstat = vstat; io_h2 = h2; io_h3 = h3; io_h5 = h5; io_h9 = h9; io_h11 = h11; return a1::RetVF{(uint64_t)v0, f0}; }
    __builtin_unreachable();
}

// GetGeometryPointerSurfacePlane (0x1132850): typed by tools/typed_lift2.py (round A6; frame calls, extra outputs xo[0]=f0 xo[1]=f20 xo[2]=a0 xo[3]=a1 xo[4]=a2 xo[5]=a3 xo[6]=t0 xo[7]=t1 xo[8]=t2 xo[9]=t3; VU0 in vf0 vf6 vf7 vf10 vf18 vf29 vf31 vacc vstat h9 h11 h29, out vf4 vf5 vf6 vf7 vf8 vf10 vf14 vf15 vf16 vf17 vf18 vf29 vf30 vf31 vacc vq vstat h2 h5 h9 h11, jump tables 1, guest sp, frame 16 bytes, 0 slot accesses)
inline uint64_t GetGeometryPointerSurfacePlane(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint64_t a0, uint64_t a2, uint64_t t0, uint64_t t1, uint64_t t2, uint64_t t3, uint32_t sp, float f0, float f12, float f13, float f14, float f15, float f16, float f17, float f18, float f19, float f20, const a4::vec4 &io_vf0, a4::vec4 &io_vf4, a4::vec4 &io_vf5, a4::vec4 &io_vf6, a4::vec4 &io_vf7, a4::vec4 &io_vf8, a4::vec4 &io_vf10, a4::vec4 &io_vf14, a4::vec4 &io_vf15, a4::vec4 &io_vf16, a4::vec4 &io_vf17, a4::vec4 &io_vf18, a4::vec4 &io_vf29, a4::vec4 &io_vf30, a4::vec4 &io_vf31, a4::vec4 &io_vacc, float &io_vq, uint16_t &io_vstat, uint64_t &io_h2, uint64_t &io_h5, uint64_t &io_h9, uint64_t &io_h11, uint64_t io_h29, uint64_t *xo = nullptr)
{
    uint64_t v0 = 0;
    uint32_t v1 = 0;
    uint64_t a1 = 0;
    uint64_t a3 = 0;
    a4::vec4 vf0 = io_vf0;
    a4::vec4 vf4 = io_vf4;
    a4::vec4 vf5 = io_vf5;
    a4::vec4 vf6 = io_vf6;
    a4::vec4 vf7 = io_vf7;
    a4::vec4 vf8 = io_vf8;
    a4::vec4 vf10 = io_vf10;
    a4::vec4 vf14 = io_vf14;
    a4::vec4 vf15 = io_vf15;
    a4::vec4 vf16 = io_vf16;
    a4::vec4 vf17 = io_vf17;
    a4::vec4 vf18 = io_vf18;
    a4::vec4 vf29 = io_vf29;
    a4::vec4 vf30 = io_vf30;
    a4::vec4 vf31 = io_vf31;
    a4::vec4 vacc = io_vacc;
    float vq = io_vq;
    uint16_t vstat = io_vstat;
    uint64_t h2 = io_h2;
    uint64_t h5 = io_h5;
    uint64_t h9 = io_h9;
    uint64_t h11 = io_h11;
    uint64_t h29 = io_h29;
    bool a3xw0 = false;
    bool a3xw1 = false;
    bool a3xw2 = false;
    bool a3xw3 = false;
    bool a3xw4 = false;
    bool a3xw5 = false;
    bool a3xw6 = false;
    bool a3xw7 = false;
    bool a3xw8 = false;
    bool a3xw9 = false;
    (v1 = (uint32_t)((int32_t)((int32_t)A1_R32B((uint32_t)a0, 48))));
    (v0 = (uint64_t)(((uint64_t)a1_sx64(v1) < (uint64_t)(int64_t)(int32_t)9) ? 1 : 0));
    if ((int64_t)v0 == 0) { (a0 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL)); a3xw2 = true; goto L_1132884; }
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)SLL32(v1, 2))));
    v1 = 0x1190000u;
    (v1 = (uint32_t)((int32_t)((int32_t)ADD32(v1, (uint32_t)v0))));
    (v1 = (uint32_t)((int32_t)((int32_t)A1_R32B(v1, 22224))));
    { const uint32_t a5_jt = v1; switch (a5_jt) { case 0x1132880u: goto L_1132880; case 0x1132884u: goto L_1132884; default: A5_BADJUMP(0x1132878u, a5_jt); } }
L_1132880:;
    (a0 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL)); a3xw2 = true;
L_1132884:;
    { const uint32_t _dynInsn = A1_R32(0x1132884u); const int32_t _dynSimm = (int16_t)(_dynInsn & 0xFFFFu); const uint32_t _dynImm = _dynInsn & 0xFFFFu; (void)_dynSimm; (void)_dynImm; (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)_dynImm << 16)))); }
    { const bool c = (int64_t)a0 == 0; { const uint32_t _dynInsn = A1_R32(0x113288Cu); const int32_t _dynSimm = (int16_t)(_dynInsn & 0xFFFFu); const uint32_t _dynImm = _dynInsn & 0xFFFFu; (void)_dynSimm; (void)_dynImm; (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v0, _dynSimm)))); } if (c) goto L_1132898; }
    { A3_CHARGE(); uint64_t a7c[10] = {(uint64_t)a1_bits(f0), (uint64_t)a1_bits(f20), 0u, 0u, a2, 0u, t0, t1, t2, t3}; v0 = GetCollisionDBPlane(ram, rt, a0, t0, a1_sx64((uint32_t)(sp + 0xFFFFFFF0u)), f0, f12, f13, f14, f15, f16, f17, f18, f19, f20, vf0, vf4, vf5, vf6, vf7, vf8, vf10, vf14, vf15, vf16, vf17, vf18, vf29, vf30, vf31, vacc, vq, vstat, h2, h5, h9, h11, h29, a7c); f0 = a1_flt((uint32_t)a7c[0]); f20 = a1_flt((uint32_t)a7c[1]); a0 = a7c[2]; a1 = a7c[3]; a2 = a7c[4]; a3 = a7c[5]; t0 = a7c[6]; t1 = a7c[7]; t2 = a7c[8]; t3 = a7c[9]; a3xw0 = true; a3xw1 = true; a3xw2 = true; a3xw3 = true; a3xw4 = true; a3xw5 = true; a3xw6 = true; a3xw7 = true; a3xw8 = true; a3xw9 = true; }
L_1132898:;
    { if (xo) { if (a3xw0) xo[0] = a1_bits(f0); if (a3xw1) xo[1] = a1_bits(f20); if (a3xw2) xo[2] = a0; if (a3xw3) xo[3] = a1; if (a3xw4) xo[4] = a2; if (a3xw5) xo[5] = a3; if (a3xw6) xo[6] = t0; if (a3xw7) xo[7] = t1; if (a3xw8) xo[8] = t2; if (a3xw9) xo[9] = t3; } io_vf4 = vf4; io_vf5 = vf5; io_vf6 = vf6; io_vf7 = vf7; io_vf8 = vf8; io_vf10 = vf10; io_vf14 = vf14; io_vf15 = vf15; io_vf16 = vf16; io_vf17 = vf17; io_vf18 = vf18; io_vf29 = vf29; io_vf30 = vf30; io_vf31 = vf31; io_vacc = vacc; io_vq = vq; io_vstat = vstat; io_h2 = h2; io_h5 = h5; io_h9 = h9; io_h11 = h11; return v0; }
    __builtin_unreachable();
}

// gamecore_sub_013F16F8 (0x13f16f8): typed by tools/typed_lift2.py (round A6; frame calls, extra outputs xo[0]=f20; VU0 in vf0 vf7 vf10 vf12 vf13 vf14 vf15 vf16 vf17 vf18 vf29 vf31 vacc vstat h5 h9 h11 h29, out vf4 vf5 vf6 vf7 vf8 vf10 vf11 vf12 vf13 vf14 vf15 vf16 vf17 vf18 vf29 vf30 vf31 vacc vq vstat h2 h3 h5 h9 h11, relocated calls 4, 64-bit inputs t0, guest sp, frame 48 bytes, 2 slot accesses)
inline a1::RetVF gamecore_sub_013F16F8(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint64_t v0, uint64_t a0, uint64_t w64_t0, uint32_t sp, float f12, float f13, float f14, float f15, float f16, float f17, float f18, float f19, float f20, const a4::vec4 &io_vf0, a4::vec4 &io_vf4, a4::vec4 &io_vf5, a4::vec4 &io_vf6, a4::vec4 &io_vf7, a4::vec4 &io_vf8, a4::vec4 &io_vf10, a4::vec4 &io_vf11, a4::vec4 &io_vf12, a4::vec4 &io_vf13, a4::vec4 &io_vf14, a4::vec4 &io_vf15, a4::vec4 &io_vf16, a4::vec4 &io_vf17, a4::vec4 &io_vf18, a4::vec4 &io_vf29, a4::vec4 &io_vf30, a4::vec4 &io_vf31, a4::vec4 &io_vacc, float &io_vq, uint16_t &io_vstat, uint64_t &io_h2, uint64_t &io_h3, uint64_t &io_h5, uint64_t &io_h9, uint64_t &io_h11, uint64_t io_h29, uint64_t *xo = nullptr)
{
    float f0 = 0.0f;
    float f1 = 0.0f;
    float f2 = 0.0f;
    uint64_t v1 = 0;
    uint32_t a1 = 0;
    uint64_t a2 = 0;
    uint32_t t0 = (uint32_t)w64_t0;
    uint64_t s0 = 0;
    uint64_t s1 = 0;
    uint64_t s2 = 0;
    uint32_t fcr31 = 0;
    a4::vec4 vf0 = io_vf0;
    a4::vec4 vf4 = io_vf4;
    a4::vec4 vf5 = io_vf5;
    a4::vec4 vf6 = io_vf6;
    a4::vec4 vf7 = io_vf7;
    a4::vec4 vf8 = io_vf8;
    a4::vec4 vf10 = io_vf10;
    a4::vec4 vf11 = io_vf11;
    a4::vec4 vf12 = io_vf12;
    a4::vec4 vf13 = io_vf13;
    a4::vec4 vf14 = io_vf14;
    a4::vec4 vf15 = io_vf15;
    a4::vec4 vf16 = io_vf16;
    a4::vec4 vf17 = io_vf17;
    a4::vec4 vf18 = io_vf18;
    a4::vec4 vf29 = io_vf29;
    a4::vec4 vf30 = io_vf30;
    a4::vec4 vf31 = io_vf31;
    a4::vec4 vacc = io_vacc;
    float vq = io_vq;
    uint16_t vstat = io_vstat;
    uint64_t h2 = io_h2;
    uint64_t h3 = io_h3;
    uint64_t h5 = io_h5;
    uint64_t h9 = io_h9;
    uint64_t h11 = io_h11;
    uint64_t h29 = io_h29;
    uint8_t a2fr[48];
    bool a3xw0 = false;
    { (s1 = (uint64_t)((uint64_t)a0 + (uint64_t)0ULL)); A3_CHARGE(); v0 = a1_sx64(isysGetThisSubThread(ram, rt)); }
    (s0 = (uint64_t)((uint64_t)v0 + (uint64_t)0ULL));
    { (a0 = (uint64_t)((uint64_t)s0 + (uint64_t)0ULL)); A3_CHARGE(); v0 = GetCharacterClass(ram, rt, a0, h2, h5); }
    (a0 = (uint64_t)((uint64_t)s0 + (uint64_t)0ULL));
    { (s2 = (uint64_t)((uint64_t)v0 + (uint64_t)0ULL)); A3_CHARGE(); v0 = GetLayoutClass(ram, rt, a0, f12, f13, f14, f15, f16, f17, f18, f19, h2); }
    { uint32_t bits = A1_R32B((uint32_t)s1, 420); float f; std::memcpy(&f, &bits, sizeof(f)); f1 = f; }
    (a2 = (uint64_t)((uint64_t)v0 + (uint64_t)0ULL));
    { uint32_t bits = A1_R32B((uint32_t)s1, 588); float f; std::memcpy(&f, &bits, sizeof(f)); f0 = f; }
    { uint32_t bits = A1_R32B((uint32_t)s1, 96); float f; std::memcpy(&f, &bits, sizeof(f)); f2 = f; }
    f0 = a1f::sub(f0, f1);
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)s1, 100))));
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)s1, 584))));
    { uint32_t bits = A1_R32B((uint32_t)v0, 8); float f; std::memcpy(&f, &bits, sizeof(f)); f1 = f; }
    f0 = a1f::sub(f0, f2);
    { const bool c = (int64_t)v1 == 0; f1 = a1f::add(f0, f1); if (c) goto L_13f1838; }
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)a2, 308))));
    { uint32_t bits = A1_R32B((uint32_t)v0, 4); float f; std::memcpy(&f, &bits, sizeof(f)); f0 = f; }
    fcr31 = (a1f::le(f1, f0)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
    if ((fcr31 & 0x800000u) == 0u) { (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)s1, 580)))); goto L_13f183c; }
    { const uint32_t _dynInsn = A1_R32(0x13F176Cu); const int32_t _dynSimm = (int16_t)(_dynInsn & 0xFFFFu); const uint32_t _dynImm = _dynInsn & 0xFFFFu; (void)_dynSimm; (void)_dynImm; (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)_dynImm << 16)))); }
    (a1 = (uint32_t)((int32_t)((int32_t)ADD32((uint32_t)s1, 448))));
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)s1, 432))));
    { const uint32_t _dynInsn = A1_R32(0x13F1778u); const int32_t _dynSimm = (int16_t)(_dynInsn & 0xFFFFu); const uint32_t _dynImm = _dynInsn & 0xFFFFu; (void)_dynSimm; (void)_dynImm; (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v0, _dynSimm)))); }
    vf8 = a4v::castsi128_ps(A4_R128(ADD32((uint32_t)v0, 0)));
    { a4::vec4 res = a4v::addK<1, 1>(vf0, a4v::bcast<3>(vf0)); a4::q128 mask = a4v::set_epi32(0, -1, -1, -1); vf12 = a4v::blendv_ps(vf12, res, a4v::castsi128_ps(mask)); }
    vf11 = a4v::castsi128_ps(A4_R128(ADD32((uint32_t)v1, 0)));
    { a4::vec4 res = a4v::shuffle_ps<0x39>(vf0, vf0); a4::q128 mask = a4v::set_epi32(-1, 0, 0, 0); vf14 = a4v::blendv_ps(vf14, res, a4v::castsi128_ps(mask)); }
    { a4::vec4 res = a4v::shuffle_ps<0x39>(vf0, vf0); a4::q128 mask = a4v::set_epi32(-1, 0, 0, 0); vf15 = a4v::blendv_ps(vf15, res, a4v::castsi128_ps(mask)); }
    { a4::vec4 res = a4v::shuffle_ps<0x39>(vf0, vf0); a4::q128 mask = a4v::set_epi32(-1, 0, 0, 0); vf16 = a4v::blendv_ps(vf16, res, a4v::castsi128_ps(mask)); }
    { a4::vec4 fs_yzx = a4v::shuffle_ps<0xC9>(vf11, vf11); a4::vec4 ft_zxy = a4v::shuffle_ps<0xD2>(vf12, vf12); a4::vec4 res = a4v::mulK<0, 1>(fs_yzx, ft_zxy); vacc = a4v::dest<1, 1, 1, 0>(vacc, res); }
    { a4::vec4 mul_res = a4v::mulK<1, 1>(vf0, vf0); a4::vec4 res = a4v::addK<1, 1>(vacc, mul_res); a4::q128 mask = a4v::set_epi32(0, -1, -1, -1); vf11 = a4v::blendv_ps(vf11, res, a4v::castsi128_ps(mask)); }
    { a4::vec4 mul_res = a4v::mulK<1, 1>(vf11, vf12); a4::vec4 res = a4v::addK<1, 1>(vacc, mul_res); a4::q128 mask = a4v::set_epi32(0, -1, -1, -1); vf10 = a4v::blendv_ps(vf10, res, a4v::castsi128_ps(mask)); }
    { a4::vec4 res = a4v::mulK<1, 1>(vf11, vf10); a4::q128 mask = a4v::set_epi32(0, -1, -1, -1); vf13 = a4v::blend(vf13, res, a4v::castsi128_ps(mask)); }
    { a4::vec4 fs_yzx = a4v::shuffle_ps<0xC9>(vf11, vf11); a4::vec4 ft_zxy = a4v::shuffle_ps<0xD2>(vf10, vf10); a4::vec4 res = a4v::mulK<1, 1>(fs_yzx, ft_zxy); vacc = a4v::dest<1, 1, 1, 0>(vacc, res); }
    { a4::vec4 mul_res = a4v::mulK<1, 0>(vf10, a4v::bcast<3>(vf11)); a4::vec4 res = a4v::addK<1, 1>(vacc, mul_res); a4::q128 mask = a4v::set_epi32(0, -1, -1, -1); vf15 = a4v::blendv_ps(vf15, res, a4v::castsi128_ps(mask)); }
    { a4::vec4 mul_res = a4v::mulK<1, 0>(vf10, a4v::bcast<3>(vf11)); a4::vec4 res = a4v::subK<1, 1>(vacc, mul_res); a4::q128 mask = a4v::set_epi32(0, -1, -1, -1); vf16 = a4v::blendv_ps(vf16, res, a4v::castsi128_ps(mask)); }
    { a4::vec4 fs_yzx = a4v::shuffle_ps<0xC9>(vf13, vf13); a4::vec4 ft_zxy = a4v::shuffle_ps<0xD2>(vf12, vf12); a4::vec4 res = a4v::mulK<1, 1>(fs_yzx, ft_zxy); vacc = a4v::dest<1, 1, 1, 0>(vacc, res); }
    { a4::vec4 mul_res = a4v::mulK<1, 1>(vf13, vf12); a4::vec4 res = a4v::addK<1, 1>(vacc, mul_res); a4::q128 mask = a4v::set_epi32(0, -1, -1, -1); vf17 = a4v::blendv_ps(vf17, res, a4v::castsi128_ps(mask)); }
    { a4::vec4 fs_yzx = a4v::shuffle_ps<0xC9>(vf15, vf15); a4::vec4 ft_zxy = a4v::shuffle_ps<0xD2>(vf12, vf12); a4::vec4 res = a4v::mulK<1, 1>(fs_yzx, ft_zxy); vacc = a4v::dest<1, 1, 1, 0>(vacc, res); }
    { a4::vec4 mul_res = a4v::mulK<1, 1>(vf0, vf0); a4::vec4 res = a4v::addK<1, 1>(vacc, mul_res); a4::q128 mask = a4v::set_epi32(0, -1, -1, -1); vf15 = a4v::blendv_ps(vf15, res, a4v::castsi128_ps(mask)); }
    { a4::vec4 res = a4v::subK<1, 1>(vf12, vf17); a4::q128 mask = a4v::set_epi32(0, -1, -1, -1); vf14 = a4v::blend(vf14, res, a4v::castsi128_ps(mask)); }
    { a4::q128 mask = a4v::set_epi32(0, -1, -1, 0); vf17 = a4v::blendv_ps(vf17, vf14, a4v::castsi128_ps(mask)); }
    { a4::q128 mask = a4v::set_epi32(0, 0, -1, 0); vf14 = a4v::blendv_ps(vf14, vf16, a4v::castsi128_ps(mask)); }
    { a4::q128 mask = a4v::set_epi32(0, 0, -1, 0); vf16 = a4v::blendv_ps(vf16, vf15, a4v::castsi128_ps(mask)); }
    { a4::q128 mask = a4v::set_epi32(0, 0, -1, 0); vf15 = a4v::blendv_ps(vf15, vf17, a4v::castsi128_ps(mask)); }
    { a4::q128 mask = a4v::set_epi32(0, -1, 0, 0); vf14 = a4v::blendv_ps(vf14, vf15, a4v::castsi128_ps(mask)); }
    { a4::q128 mask = a4v::set_epi32(0, -1, 0, 0); vf15 = a4v::blendv_ps(vf15, vf16, a4v::castsi128_ps(mask)); }
    { a4::q128 mask = a4v::set_epi32(0, -1, 0, 0); vf16 = a4v::blendv_ps(vf16, vf17, a4v::castsi128_ps(mask)); }
    { a4::vec4 res = a4v::mulK<1, 0>(vf14, a4v::bcast<0>(vf8)); vacc = a4v::dest<1, 1, 1, 1>(vacc, res); }
    { a4::vec4 mul_res = a4v::mulK<1, 0>(vf15, a4v::bcast<1>(vf8)); a4::vec4 res = a4v::addK<1, 1>(vacc, mul_res); vacc = a4v::dest<1, 1, 1, 1>(vacc, res); }
    { a4::vec4 mul_res = a4v::mulK<1, 0>(vf16, a4v::bcast<2>(vf8)); a4::vec4 res = a4v::addK<1, 1>(vacc, mul_res); vacc = a4v::dest<1, 1, 1, 1>(vacc, res); }
    { a4::vec4 mul_res = a4v::mulK<1, 0>(vf0, a4v::bcast<3>(vf8)); a4::vec4 res = a4v::addK<1, 1>(vacc, mul_res); a4::q128 mask = a4v::set_epi32(-1, -1, -1, -1); vf10 = a4v::blendv_ps(vf10, res, a4v::castsi128_ps(mask)); }
    A4_W128(ADD32(a1, 0), a4v::castps_si128(vf10));
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 1))));
    do { const a4::q128 _v = (A4_R128(ADD32((uint32_t)s1, 448))); h2 = a4::hi64(_v); std::memcpy(&v0, &_v, 8); } while (0);
    { const uint32_t _dynInsn = A1_R32(0x13F17FCu); const int32_t _dynSimm = (int16_t)(_dynInsn & 0xFFFFu); const uint32_t _dynImm = _dynInsn & 0xFFFFu; (void)_dynSimm; (void)_dynImm; (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)_dynImm << 16)))); }
    A1_W32B((uint32_t)s1, 580, (uint32_t)a0);
    { const uint32_t _dynInsn = A1_R32(0x13F1804u); const int32_t _dynSimm = (int16_t)(_dynInsn & 0xFFFFu); const uint32_t _dynImm = _dynInsn & 0xFFFFu; (void)_dynSimm; (void)_dynImm; (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v1, _dynSimm)))); }
    A4_W128(ADD32((uint32_t)s1, 464), a4::mk128(h2, v0));
    { uint32_t bits = A1_R32B((uint32_t)s1, 588); float f; std::memcpy(&f, &bits, sizeof(f)); f1 = f; }
    do { const a4::q128 _v = (A4_R128(ADD32((uint32_t)v1, 0))); h2 = a4::hi64(_v); std::memcpy(&v0, &_v, 8); } while (0);
    A4_W128(ADD32((uint32_t)s1, 496), a4::mk128(h2, v0));
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)a2, 308))));
    { uint32_t bits = A1_R32B((uint32_t)v1, 0); float f; std::memcpy(&f, &bits, sizeof(f)); f0 = f; }
    { float f = f1; uint32_t bits; std::memcpy(&bits, &f, sizeof(bits)); A1_W32B((uint32_t)s1, 484, bits); }
    { float f = f0; uint32_t bits; std::memcpy(&bits, &f, sizeof(bits)); A1_W32B((uint32_t)s1, 480, bits); }
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)a2, 308))));
    { uint32_t bits = A1_R32B((uint32_t)v0, 8); float f; std::memcpy(&f, &bits, sizeof(f)); f0 = f; }
    { float f = f0; uint32_t bits; std::memcpy(&bits, &f, sizeof(bits)); A1_W32B((uint32_t)s1, 488, bits); }
    goto L_13f191c;
L_13f1838:;
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)s1, 580))));
L_13f183c:;
    { const bool c = (int64_t)v0 == 0; (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)s2, 4096)))); if (c) goto L_13f191c; }
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)a2, 308))));
    vf5 = a4v::castsi128_ps(A4_R128(ADD32((uint32_t)v0, 0)));
    vf6 = a4v::castsi128_ps(A4_R128(ADD32((uint32_t)v1, 0)));
    { a4::vec4 res = a4v::sub(vf5, vf6); a4::q128 mask = a4v::set_epi32(-1, -1, -1, -1); vf7 = a4v::blend(vf7, res, a4v::castsi128_ps(mask)); }
    a2::frSt<a4::q128>(a2fr, 0, a4v::castps_si128(vf7));
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)s1, 448))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)s1, 432))));
    vf8 = a4v::castsi128_ps(a2::frLd<a4::q128>(a2fr, 0));
    { a4::vec4 res = a4v::addK<1, 1>(vf0, a4v::bcast<3>(vf0)); a4::q128 mask = a4v::set_epi32(0, -1, -1, -1); vf12 = a4v::blendv_ps(vf12, res, a4v::castsi128_ps(mask)); }
    vf11 = a4v::castsi128_ps(A4_R128(ADD32((uint32_t)v0, 0)));
    { a4::vec4 res = a4v::shuffle_ps<0x39>(vf0, vf0); a4::q128 mask = a4v::set_epi32(-1, 0, 0, 0); vf14 = a4v::blendv_ps(vf14, res, a4v::castsi128_ps(mask)); }
    { a4::vec4 res = a4v::shuffle_ps<0x39>(vf0, vf0); a4::q128 mask = a4v::set_epi32(-1, 0, 0, 0); vf15 = a4v::blendv_ps(vf15, res, a4v::castsi128_ps(mask)); }
    { a4::vec4 res = a4v::shuffle_ps<0x39>(vf0, vf0); a4::q128 mask = a4v::set_epi32(-1, 0, 0, 0); vf16 = a4v::blendv_ps(vf16, res, a4v::castsi128_ps(mask)); }
    { a4::vec4 fs_yzx = a4v::shuffle_ps<0xC9>(vf11, vf11); a4::vec4 ft_zxy = a4v::shuffle_ps<0xD2>(vf12, vf12); a4::vec4 res = a4v::mulK<0, 1>(fs_yzx, ft_zxy); vacc = a4v::dest<1, 1, 1, 0>(vacc, res); }
    { a4::vec4 mul_res = a4v::mulK<1, 1>(vf0, vf0); a4::vec4 res = a4v::addK<1, 1>(vacc, mul_res); a4::q128 mask = a4v::set_epi32(0, -1, -1, -1); vf11 = a4v::blendv_ps(vf11, res, a4v::castsi128_ps(mask)); }
    { a4::vec4 mul_res = a4v::mulK<1, 1>(vf11, vf12); a4::vec4 res = a4v::addK<1, 1>(vacc, mul_res); a4::q128 mask = a4v::set_epi32(0, -1, -1, -1); vf10 = a4v::blendv_ps(vf10, res, a4v::castsi128_ps(mask)); }
    { a4::vec4 res = a4v::mulK<1, 1>(vf11, vf10); a4::q128 mask = a4v::set_epi32(0, -1, -1, -1); vf13 = a4v::blend(vf13, res, a4v::castsi128_ps(mask)); }
    { a4::vec4 fs_yzx = a4v::shuffle_ps<0xC9>(vf11, vf11); a4::vec4 ft_zxy = a4v::shuffle_ps<0xD2>(vf10, vf10); a4::vec4 res = a4v::mulK<1, 1>(fs_yzx, ft_zxy); vacc = a4v::dest<1, 1, 1, 0>(vacc, res); }
    { a4::vec4 mul_res = a4v::mulK<1, 0>(vf10, a4v::bcast<3>(vf11)); a4::vec4 res = a4v::addK<1, 1>(vacc, mul_res); a4::q128 mask = a4v::set_epi32(0, -1, -1, -1); vf15 = a4v::blendv_ps(vf15, res, a4v::castsi128_ps(mask)); }
    { a4::vec4 mul_res = a4v::mulK<1, 0>(vf10, a4v::bcast<3>(vf11)); a4::vec4 res = a4v::subK<1, 1>(vacc, mul_res); a4::q128 mask = a4v::set_epi32(0, -1, -1, -1); vf16 = a4v::blendv_ps(vf16, res, a4v::castsi128_ps(mask)); }
    { a4::vec4 fs_yzx = a4v::shuffle_ps<0xC9>(vf13, vf13); a4::vec4 ft_zxy = a4v::shuffle_ps<0xD2>(vf12, vf12); a4::vec4 res = a4v::mulK<1, 1>(fs_yzx, ft_zxy); vacc = a4v::dest<1, 1, 1, 0>(vacc, res); }
    { a4::vec4 mul_res = a4v::mulK<1, 1>(vf13, vf12); a4::vec4 res = a4v::addK<1, 1>(vacc, mul_res); a4::q128 mask = a4v::set_epi32(0, -1, -1, -1); vf17 = a4v::blendv_ps(vf17, res, a4v::castsi128_ps(mask)); }
    { a4::vec4 fs_yzx = a4v::shuffle_ps<0xC9>(vf15, vf15); a4::vec4 ft_zxy = a4v::shuffle_ps<0xD2>(vf12, vf12); a4::vec4 res = a4v::mulK<1, 1>(fs_yzx, ft_zxy); vacc = a4v::dest<1, 1, 1, 0>(vacc, res); }
    { a4::vec4 mul_res = a4v::mulK<1, 1>(vf0, vf0); a4::vec4 res = a4v::addK<1, 1>(vacc, mul_res); a4::q128 mask = a4v::set_epi32(0, -1, -1, -1); vf15 = a4v::blendv_ps(vf15, res, a4v::castsi128_ps(mask)); }
    { a4::vec4 res = a4v::subK<1, 1>(vf12, vf17); a4::q128 mask = a4v::set_epi32(0, -1, -1, -1); vf14 = a4v::blend(vf14, res, a4v::castsi128_ps(mask)); }
    { a4::q128 mask = a4v::set_epi32(0, -1, -1, 0); vf17 = a4v::blendv_ps(vf17, vf14, a4v::castsi128_ps(mask)); }
    { a4::q128 mask = a4v::set_epi32(0, 0, -1, 0); vf14 = a4v::blendv_ps(vf14, vf16, a4v::castsi128_ps(mask)); }
    { a4::q128 mask = a4v::set_epi32(0, 0, -1, 0); vf16 = a4v::blendv_ps(vf16, vf15, a4v::castsi128_ps(mask)); }
    { a4::q128 mask = a4v::set_epi32(0, 0, -1, 0); vf15 = a4v::blendv_ps(vf15, vf17, a4v::castsi128_ps(mask)); }
    { a4::q128 mask = a4v::set_epi32(0, -1, 0, 0); vf14 = a4v::blendv_ps(vf14, vf15, a4v::castsi128_ps(mask)); }
    { a4::q128 mask = a4v::set_epi32(0, -1, 0, 0); vf15 = a4v::blendv_ps(vf15, vf16, a4v::castsi128_ps(mask)); }
    { a4::q128 mask = a4v::set_epi32(0, -1, 0, 0); vf16 = a4v::blendv_ps(vf16, vf17, a4v::castsi128_ps(mask)); }
    { a4::vec4 res = a4v::mulK<1, 0>(vf14, a4v::bcast<0>(vf8)); vacc = a4v::dest<1, 1, 1, 1>(vacc, res); }
    { a4::vec4 mul_res = a4v::mulK<1, 0>(vf15, a4v::bcast<1>(vf8)); a4::vec4 res = a4v::addK<1, 1>(vacc, mul_res); vacc = a4v::dest<1, 1, 1, 1>(vacc, res); }
    { a4::vec4 mul_res = a4v::mulK<1, 0>(vf16, a4v::bcast<2>(vf8)); a4::vec4 res = a4v::addK<1, 1>(vacc, mul_res); vacc = a4v::dest<1, 1, 1, 1>(vacc, res); }
    { a4::vec4 mul_res = a4v::mulK<1, 0>(vf0, a4v::bcast<3>(vf8)); a4::vec4 res = a4v::addK<1, 1>(vacc, mul_res); a4::q128 mask = a4v::set_epi32(-1, -1, -1, -1); vf10 = a4v::blendv_ps(vf10, res, a4v::castsi128_ps(mask)); }
    A4_W128(ADD32((uint32_t)a0, 0), a4v::castps_si128(vf10));
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)s1, 464))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)s1, 448))));
    vf5 = a4v::castsi128_ps(A4_R128(ADD32((uint32_t)v0, 0)));
    { a4::vec4 res = a4v::mul(vf5, vf5); a4::q128 mask = a4v::set_epi32(0, -1, -1, -1); vf7 = a4v::blend(vf7, res, a4v::castsi128_ps(mask)); }
    { a4::vec4 res = a4v::mulK<1, 1>(vf0, a4v::bcast<0>(vf7)); vacc = a4v::dest<0, 0, 0, 1>(vacc, res); }
    { a4::vec4 mul_res = a4v::mulK<1, 1>(vf0, a4v::bcast<1>(vf7)); a4::vec4 res = a4v::addK<1, 1>(vacc, mul_res); vacc = a4v::dest<0, 0, 0, 1>(vacc, res); }
    { a4::vec4 mul_res = a4v::mulK<1, 1>(vf0, a4v::bcast<2>(vf7)); a4::vec4 res = a4v::addK<1, 1>(vacc, mul_res); a4::q128 mask = a4v::set_epi32(-1, 0, 0, 0); vf7 = a4v::blendv_ps(vf7, res, a4v::castsi128_ps(mask)); }
    { float fs = a4v::cvtss_f32(a4v::shuffle_ps<0x03>(vf0, vf0)); float ft = a4v::cvtss_f32(a4v::shuffle_ps<0x03>(vf7, vf7)); vq = a4v::rsqrt(vstat, fs, ft); }
    { a4::vec4 res = a4v::mul(vf5, a4v::set1_ps(vq)); a4::q128 mask = a4v::set_epi32(0, -1, -1, -1); vf5 = a4v::blendv_ps(vf5, res, a4v::castsi128_ps(mask)); }
    A4_W128(ADD32((uint32_t)v1, 0), a4v::castps_si128(vf5));
    { (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)s2, 4088)))); A3_CHARGE(); uint64_t a7c[10] = {(uint64_t)a1_bits(f0), (uint64_t)a1_bits(f20), 0u, 0u, 0u, 0u, 0u, 0u, 0u, 0u}; v0 = GetCollisionDBPlane(ram, rt, a0, w64_t0, a1_sx64((uint32_t)(sp + 0xFFFFFFD0u)), f0, f12, f13, f14, f15, f16, f17, f18, f19, f20, vf0, vf4, vf5, vf6, vf7, vf8, vf10, vf14, vf15, vf16, vf17, vf18, vf29, vf30, vf31, vacc, vq, vstat, h2, h5, h9, h11, h29, a7c); f0 = a1_flt((uint32_t)a7c[0]); f20 = a1_flt((uint32_t)a7c[1]); a3xw0 = true; }
    do { const a4::q128 _v = (A4_R128(ADD32((uint32_t)v0, 0))); h3 = a4::hi64(_v); std::memcpy(&v1, &_v, 8); } while (0);
    A4_W128(ADD32((uint32_t)s1, 496), a4::mk128(h3, v1));
    do { const a4::q128 _v = (A4_R128(ADD32((uint32_t)s2, 4096))); h2 = a4::hi64(_v); std::memcpy(&v0, &_v, 8); } while (0);
    A4_W128(ADD32((uint32_t)s1, 480), a4::mk128(h2, v0));
L_13f191c:;
    { if (xo) { if (a3xw0) xo[0] = a1_bits(f20); } io_vf4 = vf4; io_vf5 = vf5; io_vf6 = vf6; io_vf7 = vf7; io_vf8 = vf8; io_vf10 = vf10; io_vf11 = vf11; io_vf12 = vf12; io_vf13 = vf13; io_vf14 = vf14; io_vf15 = vf15; io_vf16 = vf16; io_vf17 = vf17; io_vf18 = vf18; io_vf29 = vf29; io_vf30 = vf30; io_vf31 = vf31; io_vacc = vacc; io_vq = vq; io_vstat = vstat; io_h2 = h2; io_h3 = h3; io_h5 = h5; io_h9 = h9; io_h11 = h11; return a1::RetVF{(uint64_t)v0, f0}; }
    __builtin_unreachable();
}

// charrootdyn_memoryposition (0x10df928): typed by tools/typed_lift2.py (round A6; guest frame calls; VU0 in vf0 vf7 vf8 vf9 vf10 vf12 vf13 vf14 vf15 vf16 vf17 vf29 vf31 vacc h4 h5 h8 h9 h11 h29, out vf5 vf6 vf7 vf8 vf9 vf10 vf11 vf12 vf13 vf14 vf15 vf16 vf17 vf18 vf29 vf30 vf31 vacc h2 h3 h5 h9 h11)
inline a1::RetVF charrootdyn_memoryposition(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint64_t v0, uint64_t a0, uint64_t a1, uint64_t a2, uint64_t a3, uint64_t sp, float f12, float f13, float f14, float f15, float f16, float f17, float f18, float f19, float f20, const a4::vec4 &io_vf0, a4::vec4 &io_vf5, a4::vec4 &io_vf6, a4::vec4 &io_vf7, a4::vec4 &io_vf8, a4::vec4 &io_vf9, a4::vec4 &io_vf10, a4::vec4 &io_vf11, a4::vec4 &io_vf12, a4::vec4 &io_vf13, a4::vec4 &io_vf14, a4::vec4 &io_vf15, a4::vec4 &io_vf16, a4::vec4 &io_vf17, a4::vec4 &io_vf18, a4::vec4 &io_vf29, a4::vec4 &io_vf30, a4::vec4 &io_vf31, a4::vec4 &io_vacc, uint64_t &io_h2, uint64_t &io_h3, uint64_t io_h4, uint64_t &io_h5, uint64_t io_h8, uint64_t &io_h9, uint64_t &io_h11, uint64_t io_h29)
{
    float f0 = 0.0f;
    uint32_t at = 0;
    uint64_t v1 = 0;
    uint64_t s0 = 0;
    uint64_t s1 = 0;
    uint64_t s2 = 0;
    uint64_t s3 = 0;
    uint64_t s4 = 0;
    uint64_t s5 = 0;
    uint32_t fcr31 = 0;
    a4::vec4 vf0 = io_vf0;
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
    a4::vec4 vf29 = io_vf29;
    a4::vec4 vf30 = io_vf30;
    a4::vec4 vf31 = io_vf31;
    a4::vec4 vacc = io_vacc;
    uint64_t h2 = io_h2;
    uint64_t h3 = io_h3;
    uint64_t h4 = io_h4;
    uint64_t h5 = io_h5;
    uint64_t h8 = io_h8;
    uint64_t h9 = io_h9;
    uint64_t h11 = io_h11;
    uint64_t h29 = io_h29;
    (sp = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)sp, 4294967200))));
    (s2 = (uint64_t)((uint64_t)a3 + (uint64_t)0ULL));
    (s3 = (uint64_t)((uint64_t)a1 + (uint64_t)0ULL));
    (s4 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL));
    (s5 = (uint64_t)((uint64_t)a2 + (uint64_t)0ULL));
    { float f = f20; uint32_t bits; std::memcpy(&bits, &f, sizeof(bits)); A1_W32B((uint32_t)sp, 88, bits); }
    { f20 = ((float)(f12)); A3_CHARGE(); v0 = GetCharacterClass(ram, rt, a0, h2, h5); }
    (s1 = (uint64_t)((uint64_t)v0 + (uint64_t)0ULL));
    (a1 = (uint64_t)((uint64_t)s2 + (uint64_t)0ULL));
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)s1, 1792))));
    (s0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)s1, 448))));
    (a2 = (uint64_t)((uint64_t)RC_U64X(a0) + (uint64_t)0ULL));
    { f12 = ((float)(f20)); A3_CHARGE(); f0 = charrootdyn_getInterpPoint(ram, rt, (uint32_t)a0, (uint32_t)a1, (uint32_t)a2, f12, vf0, vf5, vf6, vf7, vf8, vf9, vacc, h2, h8); }
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)s0, 0))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)v1, 104))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)SRL32((uint32_t)v0, 25))));
    (v0 = (uint64_t)(RC_U64X(v0) & (uint64_t)(uint16_t)1));
    { const bool c = (int64_t)v0 == 0; (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)s1, 4128)))); if (c) goto L_10dfac8; }
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)s1, 4204))));
    { const bool c = (int64_t)v0 != 0; (v0 = (uint64_t)((uint64_t)s4 + (uint64_t)0ULL)); if (c) goto L_10dfad4; }
    (at = (uint32_t)((int32_t)((int32_t)((uint32_t)16256 << 16))));
    { uint32_t bits = a7_opq(at); std::memcpy(&f0, &bits, sizeof(bits)); }
    fcr31 = (a1f::le(f0, f20)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
    if ((fcr31 & 0x800000u) == 0u) { goto L_10dfad8; }
    at = 0xBF800000u;
    { uint32_t bits = a7_opq(at); std::memcpy(&f0, &bits, sizeof(bits)); }
    (v1 = (uint64_t)((uint64_t)sp + (uint64_t)0ULL));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)sp, 16))));
    { uint32_t bits; std::memcpy(&bits, &f0, sizeof(bits)); (a0 = (uint64_t)((int64_t)(int32_t)(bits))); }
    vf5 = a4v::castsi128_ps(A4_R128(ADD32((uint32_t)s2, 0)));
    vf6 = a4v::castsi128_ps(a4::mk128(h4, a0));
    { a4::vec4 res = a4v::mul(vf5, a4v::bcast<0>(vf6)); a4::q128 mask = a4v::set_epi32(0, -1, -1, -1); vf5 = a4v::blendv_ps(vf5, res, a4v::castsi128_ps(mask)); }
    A4_W128(ADD32((uint32_t)v0, 0), a4v::castps_si128(vf5));
    vf8 = a4v::castsi128_ps(A4_R128(ADD32((uint32_t)v0, 0)));
    vf18 = a4v::castsi128_ps(A4_R128(ADD32((uint32_t)s5, 0)));
    { a4::q128 mask = a4v::set_epi32(-1, 0, 0, 0); vf18 = a4v::blendv_ps(vf18, vf0, a4v::castsi128_ps(mask)); }
    { a4::vec4 res = a4v::addK<1, 1>(vf0, a4v::bcast<3>(vf0)); a4::q128 mask = a4v::set_epi32(0, -1, -1, -1); vf12 = a4v::blendv_ps(vf12, res, a4v::castsi128_ps(mask)); }
    vf11 = a4v::castsi128_ps(A4_R128(ADD32((uint32_t)s3, 0)));
    { a4::vec4 res = a4v::shuffle_ps<0x39>(vf0, vf0); a4::q128 mask = a4v::set_epi32(-1, 0, 0, 0); vf14 = a4v::blendv_ps(vf14, res, a4v::castsi128_ps(mask)); }
    { a4::vec4 res = a4v::shuffle_ps<0x39>(vf0, vf0); a4::q128 mask = a4v::set_epi32(-1, 0, 0, 0); vf15 = a4v::blendv_ps(vf15, res, a4v::castsi128_ps(mask)); }
    { a4::vec4 res = a4v::shuffle_ps<0x39>(vf0, vf0); a4::q128 mask = a4v::set_epi32(-1, 0, 0, 0); vf16 = a4v::blendv_ps(vf16, res, a4v::castsi128_ps(mask)); }
    { a4::vec4 fs_yzx = a4v::shuffle_ps<0xC9>(vf11, vf11); a4::vec4 ft_zxy = a4v::shuffle_ps<0xD2>(vf12, vf12); a4::vec4 res = a4v::mulK<0, 1>(fs_yzx, ft_zxy); vacc = a4v::dest<1, 1, 1, 0>(vacc, res); }
    { a4::vec4 mul_res = a4v::mulK<1, 1>(vf0, vf0); a4::vec4 res = a4v::addK<1, 1>(vacc, mul_res); a4::q128 mask = a4v::set_epi32(0, -1, -1, -1); vf11 = a4v::blendv_ps(vf11, res, a4v::castsi128_ps(mask)); }
    { a4::vec4 mul_res = a4v::mulK<1, 1>(vf11, vf12); a4::vec4 res = a4v::addK<1, 1>(vacc, mul_res); a4::q128 mask = a4v::set_epi32(0, -1, -1, -1); vf10 = a4v::blendv_ps(vf10, res, a4v::castsi128_ps(mask)); }
    { a4::vec4 res = a4v::mulK<1, 1>(vf11, vf10); a4::q128 mask = a4v::set_epi32(0, -1, -1, -1); vf13 = a4v::blend(vf13, res, a4v::castsi128_ps(mask)); }
    { a4::vec4 fs_yzx = a4v::shuffle_ps<0xC9>(vf11, vf11); a4::vec4 ft_zxy = a4v::shuffle_ps<0xD2>(vf10, vf10); a4::vec4 res = a4v::mulK<1, 1>(fs_yzx, ft_zxy); vacc = a4v::dest<1, 1, 1, 0>(vacc, res); }
    { a4::vec4 mul_res = a4v::mulK<1, 0>(vf10, a4v::bcast<3>(vf11)); a4::vec4 res = a4v::addK<1, 1>(vacc, mul_res); a4::q128 mask = a4v::set_epi32(0, -1, -1, -1); vf15 = a4v::blendv_ps(vf15, res, a4v::castsi128_ps(mask)); }
    { a4::vec4 mul_res = a4v::mulK<1, 0>(vf10, a4v::bcast<3>(vf11)); a4::vec4 res = a4v::subK<1, 1>(vacc, mul_res); a4::q128 mask = a4v::set_epi32(0, -1, -1, -1); vf16 = a4v::blendv_ps(vf16, res, a4v::castsi128_ps(mask)); }
    { a4::vec4 fs_yzx = a4v::shuffle_ps<0xC9>(vf13, vf13); a4::vec4 ft_zxy = a4v::shuffle_ps<0xD2>(vf12, vf12); a4::vec4 res = a4v::mulK<1, 1>(fs_yzx, ft_zxy); vacc = a4v::dest<1, 1, 1, 0>(vacc, res); }
    { a4::vec4 mul_res = a4v::mulK<1, 1>(vf13, vf12); a4::vec4 res = a4v::addK<1, 1>(vacc, mul_res); a4::q128 mask = a4v::set_epi32(0, -1, -1, -1); vf17 = a4v::blendv_ps(vf17, res, a4v::castsi128_ps(mask)); }
    { a4::vec4 fs_yzx = a4v::shuffle_ps<0xC9>(vf15, vf15); a4::vec4 ft_zxy = a4v::shuffle_ps<0xD2>(vf12, vf12); a4::vec4 res = a4v::mulK<1, 1>(fs_yzx, ft_zxy); vacc = a4v::dest<1, 1, 1, 0>(vacc, res); }
    { a4::vec4 mul_res = a4v::mulK<1, 1>(vf0, vf0); a4::vec4 res = a4v::addK<1, 1>(vacc, mul_res); a4::q128 mask = a4v::set_epi32(0, -1, -1, -1); vf15 = a4v::blendv_ps(vf15, res, a4v::castsi128_ps(mask)); }
    { a4::vec4 res = a4v::subK<1, 1>(vf12, vf17); a4::q128 mask = a4v::set_epi32(0, -1, -1, -1); vf14 = a4v::blend(vf14, res, a4v::castsi128_ps(mask)); }
    { a4::q128 mask = a4v::set_epi32(0, -1, -1, 0); vf17 = a4v::blendv_ps(vf17, vf14, a4v::castsi128_ps(mask)); }
    { a4::q128 mask = a4v::set_epi32(0, 0, -1, 0); vf14 = a4v::blendv_ps(vf14, vf16, a4v::castsi128_ps(mask)); }
    { a4::q128 mask = a4v::set_epi32(0, 0, -1, 0); vf16 = a4v::blendv_ps(vf16, vf15, a4v::castsi128_ps(mask)); }
    { a4::q128 mask = a4v::set_epi32(0, 0, -1, 0); vf15 = a4v::blendv_ps(vf15, vf17, a4v::castsi128_ps(mask)); }
    { a4::q128 mask = a4v::set_epi32(0, -1, 0, 0); vf14 = a4v::blendv_ps(vf14, vf15, a4v::castsi128_ps(mask)); }
    { a4::q128 mask = a4v::set_epi32(0, -1, 0, 0); vf15 = a4v::blendv_ps(vf15, vf16, a4v::castsi128_ps(mask)); }
    { a4::q128 mask = a4v::set_epi32(0, -1, 0, 0); vf16 = a4v::blendv_ps(vf16, vf17, a4v::castsi128_ps(mask)); }
    { a4::vec4 res = a4v::mulK<1, 0>(vf14, a4v::bcast<0>(vf8)); vacc = a4v::dest<1, 1, 1, 1>(vacc, res); }
    { a4::vec4 mul_res = a4v::mulK<1, 0>(vf15, a4v::bcast<1>(vf8)); a4::vec4 res = a4v::addK<1, 1>(vacc, mul_res); vacc = a4v::dest<1, 1, 1, 1>(vacc, res); }
    { a4::vec4 mul_res = a4v::mulK<1, 0>(vf16, a4v::bcast<2>(vf8)); a4::vec4 res = a4v::addK<1, 1>(vacc, mul_res); vacc = a4v::dest<1, 1, 1, 1>(vacc, res); }
    { a4::vec4 mul_res = a4v::mul(vf18, a4v::bcast<3>(vf8)); a4::vec4 res = a4v::addK<1, 1>(vacc, mul_res); a4::q128 mask = a4v::set_epi32(-1, -1, -1, -1); vf10 = a4v::blendv_ps(vf10, res, a4v::castsi128_ps(mask)); }
    A4_W128(ADD32((uint32_t)v1, 0), a4v::castps_si128(vf10));
    (s4 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 1))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)s1, 292))));
    (a1 = (uint64_t)((uint64_t)s3 + (uint64_t)0ULL));
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)s1, 4128))));
    { const bool c = (int64_t)v0 == 0; (a2 = (uint64_t)((uint64_t)sp + (uint64_t)0ULL)); if (c) goto L_10dfab0; }
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)s1, 240))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v0, 4294967294))));
    (v0 = (uint64_t)(((uint64_t)RC_U64X(v0) < (uint64_t)(int64_t)(int32_t)2) ? 1 : 0));
    if ((int64_t)v0 == 0) goto L_10dfab0;
    { (s0 = (uint64_t)((uint64_t)RC_U64X(a0) + (uint64_t)0ULL)); A3_CHARGE(); v0 = a1_sx64(UnlinkSecureParentInfo(ram, rt, a0, f12, f13, f14, f15, f16, f17, f18, f19)); }
    (s1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)s1, 192))));
    (a2 = (uint64_t)((uint64_t)sp + (uint64_t)0ULL));
    (a0 = (uint64_t)((uint64_t)s0 + (uint64_t)0ULL));
    { (a1 = (uint64_t)((uint64_t)RC_U64X(s1) + (uint64_t)0ULL)); A3_CHARGE(); uint64_t a7c[7] = {0u, 0u, 0u, 0u, 0u, 0u, 0u}; v0 = LinkSecureParentInfoByDBPos(ram, rt, v0, a0, a1, a2, (uint32_t)sp, f12, f13, f14, f15, f16, f17, f18, f19, f20, vf0, vf5, vf6, vf7, vf8, vf9, vf10, vf11, vf14, vf15, vf16, vf17, vf29, vf30, vf31, vacc, h2, h5, h9, h11, h29, a7c); f0 = a1_flt((uint32_t)a7c[0]); }
    (v0 = (uint64_t)((uint64_t)s4 + (uint64_t)0ULL));
    goto L_10dfad4;
L_10dfab0:;
    { A3_CHARGE(); v0 = SetGeometryPointerGlobalState(ram, rt, (uint32_t)a0, (uint32_t)a1, (uint32_t)a2, h2, h3); }
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 1))));
    A1_W32B((uint32_t)s1, 4204, (uint32_t)v0);
    goto L_10dfad0;
L_10dfac8:;
    { A3_CHARGE(); v0 = a1_sx64(UnlinkSecureParentInfo(ram, rt, a0, f12, f13, f14, f15, f16, f17, f18, f19)); }
L_10dfad0:;
    (v0 = (uint64_t)((uint64_t)s4 + (uint64_t)0ULL));
L_10dfad4:;
L_10dfad8:;
    { uint32_t bits = A1_R32B((uint32_t)sp, 88); float f; std::memcpy(&f, &bits, sizeof(f)); f20 = f; }
    { (sp = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)sp, 96)))); io_vf5 = vf5; io_vf6 = vf6; io_vf7 = vf7; io_vf8 = vf8; io_vf9 = vf9; io_vf10 = vf10; io_vf11 = vf11; io_vf12 = vf12; io_vf13 = vf13; io_vf14 = vf14; io_vf15 = vf15; io_vf16 = vf16; io_vf17 = vf17; io_vf18 = vf18; io_vf29 = vf29; io_vf30 = vf30; io_vf31 = vf31; io_vacc = vacc; io_h2 = h2; io_h3 = h3; io_h5 = h5; io_h9 = h9; io_h11 = h11; return a1::RetVF{(uint64_t)v0, f0}; }
    __builtin_unreachable();
}

// manager_sub_0111F190 (0x111f190): typed by tools/typed_lift2.py (round A6; guest frame calls, extra outputs xo[0]=f20 xo[1]=a2 xo[2]=a3 xo[3]=t0 xo[4]=t1 xo[5]=t2 xo[6]=t3; VU0 in vf0 vf6 vf7 vf8 vf9 vf10 vf11 vf18 vf29 vf31 vacc vstat h8 h9 h11 h29, out vf4 vf5 vf6 vf7 vf8 vf9 vf10 vf11 vf14 vf15 vf16 vf17 vf18 vf29 vf30 vf31 vacc vq vstat h2 h3 h5 h9 h11)
inline a1::RetVF manager_sub_0111F190(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint64_t v0, uint64_t a0, uint64_t a1, uint64_t a2, uint64_t a3, uint64_t t0, uint64_t t1, uint64_t t2, uint64_t sp, float f12, float f13, float f14, float f15, float f16, float f17, float f18, float f19, float f20, const a4::vec4 &io_vf0, a4::vec4 &io_vf4, a4::vec4 &io_vf5, a4::vec4 &io_vf6, a4::vec4 &io_vf7, a4::vec4 &io_vf8, a4::vec4 &io_vf9, a4::vec4 &io_vf10, a4::vec4 &io_vf11, a4::vec4 &io_vf14, a4::vec4 &io_vf15, a4::vec4 &io_vf16, a4::vec4 &io_vf17, a4::vec4 &io_vf18, a4::vec4 &io_vf29, a4::vec4 &io_vf30, a4::vec4 &io_vf31, a4::vec4 &io_vacc, float &io_vq, uint16_t &io_vstat, uint64_t &io_h2, uint64_t &io_h3, uint64_t &io_h5, uint64_t io_h8, uint64_t &io_h9, uint64_t &io_h11, uint64_t io_h29, uint64_t *xo = nullptr)
{
    float f0 = 0.0f;
    uint32_t v1 = 0;
    uint64_t t3 = 0;
    uint64_t s0 = 0;
    uint64_t s1 = 0;
    uint64_t s2 = 0;
    uint64_t s3 = 0;
    uint64_t s4 = 0;
    uint64_t s5 = 0;
    a4::vec4 vf0 = io_vf0;
    a4::vec4 vf4 = io_vf4;
    a4::vec4 vf5 = io_vf5;
    a4::vec4 vf6 = io_vf6;
    a4::vec4 vf7 = io_vf7;
    a4::vec4 vf8 = io_vf8;
    a4::vec4 vf9 = io_vf9;
    a4::vec4 vf10 = io_vf10;
    a4::vec4 vf11 = io_vf11;
    a4::vec4 vf14 = io_vf14;
    a4::vec4 vf15 = io_vf15;
    a4::vec4 vf16 = io_vf16;
    a4::vec4 vf17 = io_vf17;
    a4::vec4 vf18 = io_vf18;
    a4::vec4 vf29 = io_vf29;
    a4::vec4 vf30 = io_vf30;
    a4::vec4 vf31 = io_vf31;
    a4::vec4 vacc = io_vacc;
    float vq = io_vq;
    uint16_t vstat = io_vstat;
    uint64_t h2 = io_h2;
    uint64_t h3 = io_h3;
    uint64_t h5 = io_h5;
    uint64_t h8 = io_h8;
    uint64_t h9 = io_h9;
    uint64_t h11 = io_h11;
    uint64_t h29 = io_h29;
    (sp = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)sp, 4294967152))));
    (s0 = (uint64_t)((uint64_t)t0 + (uint64_t)0ULL));
    (s5 = (uint64_t)((uint64_t)a1 + (uint64_t)0ULL));
    (a1 = (uint64_t)((uint64_t)a2 + (uint64_t)0ULL));
    (a2 = (uint64_t)((uint64_t)a3 + (uint64_t)0ULL));
    { (a3 = (uint64_t)((uint64_t)s0 + (uint64_t)0ULL)); A3_CHARGE(); uint64_t a7c[8] = {(uint64_t)a1_bits(f20), 0u, a2, 0u, t0, t1, t2, 0u}; { const a1::RetVF a2_r = ClipCollisionDB(ram, rt, a0, a1, a2, a3, t0, t2, sp, f12, f13, f14, f15, f16, f17, f18, f19, f20, vf0, vf4, vf5, vf6, vf7, vf8, vf9, vf10, vf11, vf14, vf15, vf16, vf17, vf18, vf29, vf30, vf31, vacc, vq, vstat, h2, h3, h5, h8, h9, h11, h29, a7c); v0 = a2_r.v0; f0 = a2_r.f0; } f20 = a1_flt((uint32_t)a7c[0]); a2 = a7c[2]; a3 = a7c[3]; t0 = a7c[4]; t1 = a7c[5]; t2 = a7c[6]; t3 = a7c[7]; }
    (s4 = (uint64_t)((uint64_t)v0 + (uint64_t)0ULL));
    { const bool c = (int64_t)s4 == 0; (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)255 << 16)))); if (c) goto L_111f404; }
    (a1 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)s0, 4))));
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)s0, 0))));
    (v0 = (uint64_t)(RC_U64X(v0) | (uint64_t)(uint16_t)65535));
    (v1 = (uint32_t)((int32_t)((int32_t)SRL32((uint32_t)a1, 24))));
    (s3 = (uint64_t)(RC_U64X(a1) & RC_U64X(v0)));
    { (s1 = (uint64_t)(a1_sx64(v1) & (uint64_t)(uint16_t)127)); A3_CHARGE(); v0 = GetLayoutClass(ram, rt, a0, f12, f13, f14, f15, f16, f17, f18, f19, h2); }
    (a3 = (uint64_t)((uint64_t)v0 + (uint64_t)0ULL));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)s0, 4))));
    { const bool c = (int32_t)v0 >= 0; (v1 = (uint32_t)((int32_t)((int32_t)SLL32((uint32_t)s1, 1)))); if (c) goto L_111f2e0; }
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)a3, 372))));
    (v1 = (uint32_t)((int32_t)((int32_t)ADD32(v1, (uint32_t)s1))));
    (a1 = (uint64_t)((int64_t)(int32_t)((int32_t)SLL32((uint32_t)s3, 4))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)a0, 36))));
    v1 = v1 << 5;
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v0, v1))));
    (s2 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v0, 48))));
    (v1 = (uint32_t)((int32_t)((int32_t)A1_R32B((uint32_t)s2, 8))));
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)s2, 20))));
    (v1 = (uint32_t)((int32_t)((int32_t)ADD32(v1, (uint32_t)s3))));
    (v0 = (uint64_t)(uint32_t)((uint8_t)A1_R8B(v1, 0)));
    { const bool c = (int64_t)v0 != 0; (s0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)a0, (uint32_t)a1)))); if (c) goto L_111f3f8; }
    (a2 = (uint64_t)((uint64_t)a3 + (uint64_t)0ULL));
    (a3 = (uint64_t)((uint64_t)s1 + (uint64_t)0ULL));
    (a0 = (uint64_t)((uint64_t)sp + (uint64_t)0ULL));
    (a1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)sp, 16))));
    { (t0 = (uint64_t)((uint64_t)s3 + (uint64_t)0ULL)); A3_CHARGE(); uint64_t a7c[9] = {(uint64_t)a1_bits(f0), 0u, 0u, 0u, 0u, 0u, 0u, 0u, 0u}; v0 = manager_sub_01114C70(ram, rt, a0, a1, a2, a3, t0, (uint32_t)sp, f12, f13, f14, f15, f16, f17, f18, f19, f20, vf8, vf10, vf14, vf15, vf16, vf17, vf29, vf30, vf31, vacc, h5, h9, h29, a7c); f0 = a1_flt((uint32_t)a7c[0]); f20 = a1_flt((uint32_t)a7c[1]); t3 = a7c[8]; }
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)sp, 0))));
    vf29 = a4v::castsi128_ps(A4_R128(ADD32((uint32_t)v0, 0)));
    (v1 = (uint32_t)((int32_t)((int32_t)A1_R32B((uint32_t)sp, 4))));
    vf30 = a4v::castsi128_ps(A4_R128(ADD32(v1, 0)));
    { a4::vec4 res = a4v::sub(vf29, vf30); a4::q128 mask = a4v::set_epi32(-1, -1, -1, -1); vf31 = a4v::blend(vf31, res, a4v::castsi128_ps(mask)); }
    { a4::q128 mask = a4v::set_epi32(-1, -1, -1, -1); vf18 = a4v::blendv_ps(vf18, vf31, a4v::castsi128_ps(mask)); }
    { a4::q128 mask = a4v::set_epi32(-1, -1, -1, -1); vf29 = a4v::blendv_ps(vf29, vf30, a4v::castsi128_ps(mask)); }
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)sp, 8))));
    vf30 = a4v::castsi128_ps(A4_R128(ADD32((uint32_t)v0, 0)));
    { a4::vec4 res = a4v::sub(vf29, vf30); a4::q128 mask = a4v::set_epi32(-1, -1, -1, -1); vf31 = a4v::blend(vf31, res, a4v::castsi128_ps(mask)); }
    { a4::q128 mask = a4v::set_epi32(-1, -1, -1, -1); vf29 = a4v::blendv_ps(vf29, vf18, a4v::castsi128_ps(mask)); }
    { a4::q128 mask = a4v::set_epi32(-1, -1, -1, -1); vf30 = a4v::blendv_ps(vf30, vf31, a4v::castsi128_ps(mask)); }
    { a4::vec4 fs_yzx = a4v::shuffle_ps<0xC9>(vf29, vf29); a4::vec4 ft_zxy = a4v::shuffle_ps<0xD2>(vf30, vf30); a4::vec4 res = a4v::mulK<1, 1>(fs_yzx, ft_zxy); vacc = a4v::dest<1, 1, 1, 0>(vacc, res); }
    { a4::vec4 fs_yzx = a4v::shuffle_ps<0xC9>(vf30, vf30); a4::vec4 ft_zxy = a4v::shuffle_ps<0xD2>(vf29, vf29); a4::vec4 mul_res = a4v::mulK<1, 1>(fs_yzx, ft_zxy); a4::vec4 res = a4v::subK<1, 1>(vacc, mul_res); a4::q128 mask = a4v::set_epi32(0, -1, -1, -1); vf31 = a4v::blendv_ps(vf31, res, a4v::castsi128_ps(mask)); }
    { a4::vec4 res = a4v::subK<1, 1>(vf31, vf31); a4::q128 mask = a4v::set_epi32(-1, 0, 0, 0); vf31 = a4v::blend(vf31, res, a4v::castsi128_ps(mask)); }
    { a4::q128 mask = a4v::set_epi32(-1, -1, -1, -1); vf29 = a4v::blendv_ps(vf29, vf31, a4v::castsi128_ps(mask)); }
    { a4::vec4 res = a4v::mulK<1, 1>(vf29, vf29); a4::q128 mask = a4v::set_epi32(0, -1, -1, -1); vf7 = a4v::blend(vf7, res, a4v::castsi128_ps(mask)); }
    { a4::vec4 res = a4v::mulK<1, 1>(vf0, a4v::bcast<0>(vf7)); vacc = a4v::dest<0, 0, 0, 1>(vacc, res); }
    { a4::vec4 mul_res = a4v::mulK<1, 1>(vf0, a4v::bcast<1>(vf7)); a4::vec4 res = a4v::addK<1, 1>(vacc, mul_res); vacc = a4v::dest<0, 0, 0, 1>(vacc, res); }
    { a4::vec4 mul_res = a4v::mulK<1, 1>(vf0, a4v::bcast<2>(vf7)); a4::vec4 res = a4v::addK<1, 1>(vacc, mul_res); a4::q128 mask = a4v::set_epi32(-1, 0, 0, 0); vf7 = a4v::blendv_ps(vf7, res, a4v::castsi128_ps(mask)); }
    { float fs = a4v::cvtss_f32(a4v::shuffle_ps<0x03>(vf0, vf0)); float ft = a4v::cvtss_f32(a4v::shuffle_ps<0x03>(vf7, vf7)); vq = a4v::rsqrt(vstat, fs, ft); }
    { a4::vec4 res = a4v::mul(vf29, a4v::set1_ps(vq)); a4::q128 mask = a4v::set_epi32(0, -1, -1, -1); vf29 = a4v::blendv_ps(vf29, res, a4v::castsi128_ps(mask)); }
    { a4::q128 mask = a4v::set_epi32(-1, -1, -1, -1); vf31 = a4v::blendv_ps(vf31, vf29, a4v::castsi128_ps(mask)); }
    A4_W128(ADD32((uint32_t)s0, 0), a4v::castps_si128(vf31));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)sp, 0))));
    vf4 = a4v::castsi128_ps(A4_R128(ADD32((uint32_t)s0, 0)));
    vf5 = a4v::castsi128_ps(A4_R128(ADD32((uint32_t)v0, 0)));
    { a4::vec4 res = a4v::mul(vf4, vf5); a4::q128 mask = a4v::set_epi32(0, -1, -1, -1); vf5 = a4v::blend(vf5, res, a4v::castsi128_ps(mask)); }
    { a4::vec4 res = a4v::addK<1, 1>(vf5, a4v::bcast<1>(vf5)); a4::q128 mask = a4v::set_epi32(0, 0, 0, -1); vf5 = a4v::blendv_ps(vf5, res, a4v::castsi128_ps(mask)); }
    { a4::vec4 res = a4v::addK<1, 1>(vf5, a4v::bcast<2>(vf5)); a4::q128 mask = a4v::set_epi32(0, 0, 0, -1); vf5 = a4v::blendv_ps(vf5, res, a4v::castsi128_ps(mask)); }
    { a4::vec4 res = a4v::subK<1, 1>(vf0, a4v::bcast<3>(vf0)); a4::q128 mask = a4v::set_epi32(-1, -1, -1, -1); vf6 = a4v::blendv_ps(vf6, res, a4v::castsi128_ps(mask)); }
    { a4::vec4 res = a4v::subK<1, 1>(vf6, a4v::bcast<0>(vf5)); a4::q128 mask = a4v::set_epi32(-1, 0, 0, 0); vf4 = a4v::blendv_ps(vf4, res, a4v::castsi128_ps(mask)); }
    A4_W128(ADD32((uint32_t)s0, 0), a4v::castps_si128(vf4));
    (a0 = (uint64_t)((uint64_t)s2 + (uint64_t)0ULL));
    (a1 = (uint64_t)((uint64_t)s3 + (uint64_t)0ULL));
    goto L_111f3f0;
L_111f2e0:;
    (v1 = (uint32_t)((int32_t)((int32_t)A1_R32B((uint32_t)a3, 388))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)SLL32((uint32_t)s1, 3))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v0, (uint32_t)s1))));
    (a1 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)a3, 372))));
    (t2 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B(v1, 32))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)SLL32((uint32_t)v0, 4))));
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)SLL32((uint32_t)s3, 2))));
    (s2 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)a1, 88))));
    (t1 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)t2, 8))));
    (a1 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)s2, 8))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v0, (uint32_t)t1))));
    (a2 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)s2, 20))));
    (v1 = (uint32_t)((int32_t)((int32_t)A1_R32B((uint32_t)v0, 84))));
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)a0, v1))));
    (s1 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)a0, 0))));
    (a1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)a1, (uint32_t)s1))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)SLL32((uint32_t)s1, 4))));
    (v1 = (uint32_t)((uint8_t)A1_R8B((uint32_t)a1, 0)));
    { const bool c = v1 != 0; (s0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)a2, (uint32_t)v0)))); if (c) goto L_111f3f8; }
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)a3, 340))));
    (v1 = (uint32_t)((int32_t)((int32_t)A1_R32B((uint32_t)t2, 24))));
    (a2 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)v0, 32))));
    (t3 = (uint64_t)((int64_t)(int32_t)((int32_t)SLL32((uint32_t)s1, 6))));
    (t0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)v0, 8))));
    (t3 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(v1, (uint32_t)t3))));
    (a3 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)a2, 36))));
    (a2 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)a2, 40))));
    (t2 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)t2, 20))));
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)sp, 32))));
    { (a1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)sp, 48)))); A3_CHARGE(); uint64_t a7c[7] = {0u, 0u, 0u, 0u, 0u, 0u, t3}; v0 = manager_sub_01116108(ram, rt, a0, a1, a2, a3, t0, t1, t2, t3, sp, f12, f13, f14, f15, f16, f17, f18, f19, vf8, vf10, vf14, vf15, vf16, vf17, vf29, vf30, vf31, vacc, h11, a7c); t3 = a7c[6]; }
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)sp, 32))));
    vf29 = a4v::castsi128_ps(A4_R128(ADD32((uint32_t)v0, 0)));
    (v1 = (uint32_t)((int32_t)((int32_t)A1_R32B((uint32_t)sp, 36))));
    vf30 = a4v::castsi128_ps(A4_R128(ADD32(v1, 0)));
    { a4::vec4 res = a4v::sub(vf29, vf30); a4::q128 mask = a4v::set_epi32(-1, -1, -1, -1); vf31 = a4v::blend(vf31, res, a4v::castsi128_ps(mask)); }
    { a4::q128 mask = a4v::set_epi32(-1, -1, -1, -1); vf18 = a4v::blendv_ps(vf18, vf31, a4v::castsi128_ps(mask)); }
    { a4::q128 mask = a4v::set_epi32(-1, -1, -1, -1); vf29 = a4v::blendv_ps(vf29, vf30, a4v::castsi128_ps(mask)); }
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)sp, 40))));
    vf30 = a4v::castsi128_ps(A4_R128(ADD32((uint32_t)v0, 0)));
    { a4::vec4 res = a4v::sub(vf29, vf30); a4::q128 mask = a4v::set_epi32(-1, -1, -1, -1); vf31 = a4v::blend(vf31, res, a4v::castsi128_ps(mask)); }
    { a4::q128 mask = a4v::set_epi32(-1, -1, -1, -1); vf29 = a4v::blendv_ps(vf29, vf18, a4v::castsi128_ps(mask)); }
    { a4::q128 mask = a4v::set_epi32(-1, -1, -1, -1); vf30 = a4v::blendv_ps(vf30, vf31, a4v::castsi128_ps(mask)); }
    { a4::vec4 fs_yzx = a4v::shuffle_ps<0xC9>(vf29, vf29); a4::vec4 ft_zxy = a4v::shuffle_ps<0xD2>(vf30, vf30); a4::vec4 res = a4v::mulK<1, 1>(fs_yzx, ft_zxy); vacc = a4v::dest<1, 1, 1, 0>(vacc, res); }
    { a4::vec4 fs_yzx = a4v::shuffle_ps<0xC9>(vf30, vf30); a4::vec4 ft_zxy = a4v::shuffle_ps<0xD2>(vf29, vf29); a4::vec4 mul_res = a4v::mulK<1, 1>(fs_yzx, ft_zxy); a4::vec4 res = a4v::subK<1, 1>(vacc, mul_res); a4::q128 mask = a4v::set_epi32(0, -1, -1, -1); vf31 = a4v::blendv_ps(vf31, res, a4v::castsi128_ps(mask)); }
    { a4::vec4 res = a4v::subK<1, 1>(vf31, vf31); a4::q128 mask = a4v::set_epi32(-1, 0, 0, 0); vf31 = a4v::blend(vf31, res, a4v::castsi128_ps(mask)); }
    { a4::q128 mask = a4v::set_epi32(-1, -1, -1, -1); vf29 = a4v::blendv_ps(vf29, vf31, a4v::castsi128_ps(mask)); }
    { a4::vec4 res = a4v::mulK<1, 1>(vf29, vf29); a4::q128 mask = a4v::set_epi32(0, -1, -1, -1); vf7 = a4v::blend(vf7, res, a4v::castsi128_ps(mask)); }
    { a4::vec4 res = a4v::mulK<1, 1>(vf0, a4v::bcast<0>(vf7)); vacc = a4v::dest<0, 0, 0, 1>(vacc, res); }
    { a4::vec4 mul_res = a4v::mulK<1, 1>(vf0, a4v::bcast<1>(vf7)); a4::vec4 res = a4v::addK<1, 1>(vacc, mul_res); vacc = a4v::dest<0, 0, 0, 1>(vacc, res); }
    { a4::vec4 mul_res = a4v::mulK<1, 1>(vf0, a4v::bcast<2>(vf7)); a4::vec4 res = a4v::addK<1, 1>(vacc, mul_res); a4::q128 mask = a4v::set_epi32(-1, 0, 0, 0); vf7 = a4v::blendv_ps(vf7, res, a4v::castsi128_ps(mask)); }
    { float fs = a4v::cvtss_f32(a4v::shuffle_ps<0x03>(vf0, vf0)); float ft = a4v::cvtss_f32(a4v::shuffle_ps<0x03>(vf7, vf7)); vq = a4v::rsqrt(vstat, fs, ft); }
    { a4::vec4 res = a4v::mul(vf29, a4v::set1_ps(vq)); a4::q128 mask = a4v::set_epi32(0, -1, -1, -1); vf29 = a4v::blendv_ps(vf29, res, a4v::castsi128_ps(mask)); }
    { a4::q128 mask = a4v::set_epi32(-1, -1, -1, -1); vf31 = a4v::blendv_ps(vf31, vf29, a4v::castsi128_ps(mask)); }
    A4_W128(ADD32((uint32_t)s0, 0), a4v::castps_si128(vf31));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)sp, 32))));
    vf4 = a4v::castsi128_ps(A4_R128(ADD32((uint32_t)s0, 0)));
    vf5 = a4v::castsi128_ps(A4_R128(ADD32((uint32_t)v0, 0)));
    { a4::vec4 res = a4v::mul(vf4, vf5); a4::q128 mask = a4v::set_epi32(0, -1, -1, -1); vf5 = a4v::blend(vf5, res, a4v::castsi128_ps(mask)); }
    { a4::vec4 res = a4v::addK<1, 1>(vf5, a4v::bcast<1>(vf5)); a4::q128 mask = a4v::set_epi32(0, 0, 0, -1); vf5 = a4v::blendv_ps(vf5, res, a4v::castsi128_ps(mask)); }
    { a4::vec4 res = a4v::addK<1, 1>(vf5, a4v::bcast<2>(vf5)); a4::q128 mask = a4v::set_epi32(0, 0, 0, -1); vf5 = a4v::blendv_ps(vf5, res, a4v::castsi128_ps(mask)); }
    { a4::vec4 res = a4v::subK<1, 1>(vf0, a4v::bcast<3>(vf0)); a4::q128 mask = a4v::set_epi32(-1, -1, -1, -1); vf6 = a4v::blendv_ps(vf6, res, a4v::castsi128_ps(mask)); }
    { a4::vec4 res = a4v::subK<1, 1>(vf6, a4v::bcast<0>(vf5)); a4::q128 mask = a4v::set_epi32(-1, 0, 0, 0); vf4 = a4v::blendv_ps(vf4, res, a4v::castsi128_ps(mask)); }
    A4_W128(ADD32((uint32_t)s0, 0), a4v::castps_si128(vf4));
    (a0 = (uint64_t)((uint64_t)s2 + (uint64_t)0ULL));
    (a1 = (uint64_t)((uint64_t)s1 + (uint64_t)0ULL));
L_111f3f0:;
    { A3_CHARGE(); uint64_t a7c[5] = {0u, 0u, 0u, 0u, 0u}; v0 = EntryModifiedManager(ram, rt, a0, (uint32_t)a1, a7c); a2 = a7c[0]; a3 = a7c[1]; t0 = a7c[2]; t1 = a7c[3]; t2 = a7c[4]; }
L_111f3f8:;
    (v0 = (uint64_t)((uint64_t)s0 + (uint64_t)0ULL));
    do { const a4::q128 _v = (A4_R128(ADD32((uint32_t)v0, 0))); h2 = a4::hi64(_v); std::memcpy(&v0, &_v, 8); } while (0);
    A4_W128(ADD32((uint32_t)s5, 0), a4::mk128(h2, v0));
L_111f404:;
    (v0 = (uint64_t)((uint64_t)s4 + (uint64_t)0ULL));
    { (sp = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)sp, 144)))); if (xo) { xo[0] = a1_bits(f20); xo[1] = a2; xo[2] = a3; xo[3] = t0; xo[4] = t1; xo[5] = t2; xo[6] = t3; } io_vf4 = vf4; io_vf5 = vf5; io_vf6 = vf6; io_vf7 = vf7; io_vf8 = vf8; io_vf9 = vf9; io_vf10 = vf10; io_vf11 = vf11; io_vf14 = vf14; io_vf15 = vf15; io_vf16 = vf16; io_vf17 = vf17; io_vf18 = vf18; io_vf29 = vf29; io_vf30 = vf30; io_vf31 = vf31; io_vacc = vacc; io_vq = vq; io_vstat = vstat; io_h2 = h2; io_h3 = h3; io_h5 = h5; io_h9 = h9; io_h11 = h11; return a1::RetVF{(uint64_t)v0, f0}; }
    __builtin_unreachable();
}

// ConvertFromCollisionPointToDynamics (0x1124048): typed by tools/typed_lift2.py (round A6; frame calls, extra outputs xo[0]=f0 xo[1]=f20 xo[2]=a1 xo[3]=a2 xo[4]=a3 xo[5]=t0 xo[6]=t1 xo[7]=t2 xo[8]=t3; VU0 in vf0 vf6 vf7 vf10 vf18 vf29 vf31 vacc vstat h9 h11 h29, out vf4 vf5 vf6 vf7 vf8 vf10 vf14 vf15 vf16 vf17 vf18 vf29 vf30 vf31 vacc vq vstat h2 h3 h5 h9 h11, guest sp, frame 32 bytes, 0 slot accesses)
inline uint64_t ConvertFromCollisionPointToDynamics(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint64_t a0, uint64_t a1, uint64_t a2, uint64_t a3, uint64_t t0, uint64_t t1, uint64_t t2, uint64_t t3, uint32_t sp, float f0, float f12, float f13, float f14, float f15, float f16, float f17, float f18, float f19, float f20, const a4::vec4 &io_vf0, a4::vec4 &io_vf4, a4::vec4 &io_vf5, a4::vec4 &io_vf6, a4::vec4 &io_vf7, a4::vec4 &io_vf8, a4::vec4 &io_vf10, a4::vec4 &io_vf14, a4::vec4 &io_vf15, a4::vec4 &io_vf16, a4::vec4 &io_vf17, a4::vec4 &io_vf18, a4::vec4 &io_vf29, a4::vec4 &io_vf30, a4::vec4 &io_vf31, a4::vec4 &io_vacc, float &io_vq, uint16_t &io_vstat, uint64_t &io_h2, uint64_t &io_h3, uint64_t &io_h5, uint64_t &io_h9, uint64_t &io_h11, uint64_t io_h29, uint64_t *xo = nullptr)
{
    uint64_t v0 = 0;
    uint64_t v1 = 0;
    uint64_t s0 = 0;
    uint64_t s1 = 0;
    a4::vec4 vf0 = io_vf0;
    a4::vec4 vf4 = io_vf4;
    a4::vec4 vf5 = io_vf5;
    a4::vec4 vf6 = io_vf6;
    a4::vec4 vf7 = io_vf7;
    a4::vec4 vf8 = io_vf8;
    a4::vec4 vf10 = io_vf10;
    a4::vec4 vf14 = io_vf14;
    a4::vec4 vf15 = io_vf15;
    a4::vec4 vf16 = io_vf16;
    a4::vec4 vf17 = io_vf17;
    a4::vec4 vf18 = io_vf18;
    a4::vec4 vf29 = io_vf29;
    a4::vec4 vf30 = io_vf30;
    a4::vec4 vf31 = io_vf31;
    a4::vec4 vacc = io_vacc;
    float vq = io_vq;
    uint16_t vstat = io_vstat;
    uint64_t h2 = io_h2;
    uint64_t h3 = io_h3;
    uint64_t h5 = io_h5;
    uint64_t h9 = io_h9;
    uint64_t h11 = io_h11;
    uint64_t h29 = io_h29;
    (s0 = (uint64_t)((uint64_t)a0 + (uint64_t)0ULL));
    (s1 = (uint64_t)((uint64_t)a1 + (uint64_t)0ULL));
    (a0 = (uint64_t)((uint64_t)s1 + (uint64_t)0ULL));
    do { const a4::q128 _v = (A4_R128(ADD32((uint32_t)s1, 64))); h2 = a4::hi64(_v); std::memcpy(&v0, &_v, 8); } while (0);
    { A4_W128(ADD32((uint32_t)s0, 0), a4::mk128(h2, v0)); A3_CHARGE(); uint64_t a7c[10] = {(uint64_t)a1_bits(f0), (uint64_t)a1_bits(f20), 0u, a1, a2, a3, t0, t1, t2, t3}; v0 = GetGeometryPointerSurfacePlane(ram, rt, a0, a2, t0, t1, t2, t3, (uint32_t)(sp + 0xFFFFFFE0u), f0, f12, f13, f14, f15, f16, f17, f18, f19, f20, vf0, vf4, vf5, vf6, vf7, vf8, vf10, vf14, vf15, vf16, vf17, vf18, vf29, vf30, vf31, vacc, vq, vstat, h2, h5, h9, h11, h29, a7c); f0 = a1_flt((uint32_t)a7c[0]); f20 = a1_flt((uint32_t)a7c[1]); a1 = a7c[3]; a2 = a7c[4]; a3 = a7c[5]; t0 = a7c[6]; t1 = a7c[7]; t2 = a7c[8]; t3 = a7c[9]; }
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)s0, 32))));
    do { const a4::q128 _v = (A4_R128(ADD32((uint32_t)v0, 0))); h3 = a4::hi64(_v); std::memcpy(&v1, &_v, 8); } while (0);
    A4_W128(ADD32((uint32_t)s0, 16), a4::mk128(h3, v1));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)s1, 0))));
    A1_W32B((uint32_t)a0, 0, (uint32_t)v0);
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)s1, 4))));
    A1_W32B((uint32_t)a0, 4, (uint32_t)v1);
    { if (xo) { xo[0] = a1_bits(f0); xo[1] = a1_bits(f20); xo[2] = a1; xo[3] = a2; xo[4] = a3; xo[5] = t0; xo[6] = t1; xo[7] = t2; xo[8] = t3; } io_vf4 = vf4; io_vf5 = vf5; io_vf6 = vf6; io_vf7 = vf7; io_vf8 = vf8; io_vf10 = vf10; io_vf14 = vf14; io_vf15 = vf15; io_vf16 = vf16; io_vf17 = vf17; io_vf18 = vf18; io_vf29 = vf29; io_vf30 = vf30; io_vf31 = vf31; io_vacc = vacc; io_vq = vq; io_vstat = vstat; io_h2 = h2; io_h3 = h3; io_h5 = h5; io_h9 = h9; io_h11 = h11; return v0; }
    __builtin_unreachable();
}

// manager_sub_01141CE0 (0x1141ce0): typed by tools/typed_lift2.py (round A6; frame calls, extra outputs xo[0]=f0 xo[1]=a3 xo[2]=t0 xo[3]=t1 xo[4]=t2 xo[5]=t3; VU0 in vf0 vf6 vf7 vf10 vf18 vf29 vf31 vacc vstat h9 h11 h29, out vf4 vf5 vf6 vf7 vf8 vf10 vf14 vf15 vf16 vf17 vf18 vf29 vf30 vf31 vacc vq vstat h2 h3 h5 h9 h11, guest sp, frame 16 bytes, 0 slot accesses)
inline uint64_t manager_sub_01141CE0(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint64_t a0, uint64_t a1, uint64_t a2, uint64_t a3, uint64_t t0, uint64_t t1, uint64_t t2, uint64_t t3, uint32_t sp, float f0, float f12, float f13, float f14, float f15, float f16, float f17, float f18, float f19, float f20, const a4::vec4 &io_vf0, a4::vec4 &io_vf4, a4::vec4 &io_vf5, a4::vec4 &io_vf6, a4::vec4 &io_vf7, a4::vec4 &io_vf8, a4::vec4 &io_vf10, a4::vec4 &io_vf14, a4::vec4 &io_vf15, a4::vec4 &io_vf16, a4::vec4 &io_vf17, a4::vec4 &io_vf18, a4::vec4 &io_vf29, a4::vec4 &io_vf30, a4::vec4 &io_vf31, a4::vec4 &io_vacc, float &io_vq, uint16_t &io_vstat, uint64_t &io_h2, uint64_t &io_h3, uint64_t &io_h5, uint64_t &io_h9, uint64_t &io_h11, uint64_t io_h29, uint64_t *xo = nullptr)
{
    uint64_t v0 = 0;
    uint64_t v1 = 0;
    uint64_t s0 = 0;
    a4::vec4 vf0 = io_vf0;
    a4::vec4 vf4 = io_vf4;
    a4::vec4 vf5 = io_vf5;
    a4::vec4 vf6 = io_vf6;
    a4::vec4 vf7 = io_vf7;
    a4::vec4 vf8 = io_vf8;
    a4::vec4 vf10 = io_vf10;
    a4::vec4 vf14 = io_vf14;
    a4::vec4 vf15 = io_vf15;
    a4::vec4 vf16 = io_vf16;
    a4::vec4 vf17 = io_vf17;
    a4::vec4 vf18 = io_vf18;
    a4::vec4 vf29 = io_vf29;
    a4::vec4 vf30 = io_vf30;
    a4::vec4 vf31 = io_vf31;
    a4::vec4 vacc = io_vacc;
    float vq = io_vq;
    uint16_t vstat = io_vstat;
    uint64_t h2 = io_h2;
    uint64_t h3 = io_h3;
    uint64_t h5 = io_h5;
    uint64_t h9 = io_h9;
    uint64_t h11 = io_h11;
    uint64_t h29 = io_h29;
    (s0 = (uint64_t)((uint64_t)a1 + (uint64_t)0ULL));
    do { const a4::q128 _v = (A4_R128(ADD32((uint32_t)a2, 64))); h2 = a4::hi64(_v); std::memcpy(&v0, &_v, 8); } while (0);
    A4_W128(ADD32((uint32_t)a0, 0), a4::mk128(h2, v0));
    { (a0 = (uint64_t)((uint64_t)a2 + (uint64_t)0ULL)); A3_CHARGE(); uint64_t a7c[10] = {(uint64_t)a1_bits(f0), 0u, 0u, 0u, 0u, a3, t0, t1, t2, t3}; v0 = GetGeometryPointerSurfacePlane(ram, rt, a0, a2, t0, t1, t2, t3, (uint32_t)(sp + 0xFFFFFFF0u), f0, f12, f13, f14, f15, f16, f17, f18, f19, f20, vf0, vf4, vf5, vf6, vf7, vf8, vf10, vf14, vf15, vf16, vf17, vf18, vf29, vf30, vf31, vacc, vq, vstat, h2, h5, h9, h11, h29, a7c); f0 = a1_flt((uint32_t)a7c[0]); a3 = a7c[5]; t0 = a7c[6]; t1 = a7c[7]; t2 = a7c[8]; t3 = a7c[9]; }
    do { const a4::q128 _v = (A4_R128(ADD32((uint32_t)v0, 0))); h3 = a4::hi64(_v); std::memcpy(&v1, &_v, 8); } while (0);
    A4_W128(ADD32((uint32_t)s0, 0), a4::mk128(h3, v1));
    A1_W32B((uint32_t)s0, 12, 0u);
    { if (xo) { xo[0] = a1_bits(f0); xo[1] = a3; xo[2] = t0; xo[3] = t1; xo[4] = t2; xo[5] = t3; } io_vf4 = vf4; io_vf5 = vf5; io_vf6 = vf6; io_vf7 = vf7; io_vf8 = vf8; io_vf10 = vf10; io_vf14 = vf14; io_vf15 = vf15; io_vf16 = vf16; io_vf17 = vf17; io_vf18 = vf18; io_vf29 = vf29; io_vf30 = vf30; io_vf31 = vf31; io_vacc = vacc; io_vq = vq; io_vstat = vstat; io_h2 = h2; io_h3 = h3; io_h5 = h5; io_h9 = h9; io_h11 = h11; return v0; }
    __builtin_unreachable();
}
