// typed_a11.inc: round A11's typed functions (tools/typed_lift2.py, stage 11: the matrix stack, recursion over it). Generated: do not edit. Included inside a namespace (typed_a11.h: namespace typed), after typed_a7.inc. First the variants of earlier functions (<name>_x11: the same MIPS typed with extra outputs, called by stage-11 functions only), then stage 11's functions.
// the recursive functions (A11_REC index): 0 manager_sub_010B8798, 1 manager_sub_010B8958, 2 manager_sub_010C38E8
// iosCopyMatrix_x11 (0x1b9148): typed by tools/typed_lift2.py (round A6; leaf, extra outputs xo[0]=a2 xo[1]=a3 xo[2]=t0 xo[3]=t1; VU0 in -, out h6 h7 h8 h9)
inline void iosCopyMatrix_x11(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t a0, uint32_t a1, uint64_t &io_h6, uint64_t &io_h7, uint64_t &io_h8, uint64_t &io_h9, uint64_t *xo = nullptr)
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

// sndSepSetVol_u11 (0x1cac98): typed by tools/typed_lift2.py (round A6; leaf; VU0 in h6, out h2)
inline a1::RetVF sndSepSetVol_u11(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t a0, float f12, uint64_t &io_h2, uint64_t io_h6)
{
    float f0 = 0.0f;
    float f1 = 0.0f;
    uint32_t at = 0;
    uint64_t v0 = 0;
    uint64_t v1 = 0;
    uint64_t a1 = 0;
    uint32_t a2 = 0;
    uint64_t h2 = io_h2;
    uint64_t h6 = io_h6;
    at = 0x42FE0000u;
    f1 = a1_flt(a7_opq(at));
    a2 = 0x7Fu;
    f0 = a1_flt(A1_R32B(a0, 28));
    f12 = a1f::mul(f12, f1);
    f12 = a1f::mul(f12, f0);
    { int32_t tmp = a1f::cvtw(f12); std::memcpy(&f0, &tmp, sizeof(tmp)); }
    { uint32_t bits; std::memcpy(&bits, &f0, sizeof(bits)); (v0 = (uint64_t)((int64_t)(int32_t)(bits))); }
    (v1 = (uint64_t)(((int64_t)(int64_t)RC_U64X(v0) < (int64_t)(int32_t)128) ? 1 : 0));
    (a1 = (uint64_t)(((int64_t)(int64_t)RC_U64X(v0) < (int64_t)(int32_t)0) ? 1 : 0));
    if (RC_U64X(a1) != 0) do { const a4::q128 _v = (a4v::setzero_si128()); h2 = a4::hi64(_v); std::memcpy(&v0, &_v, 8); } while (0);
    (v1 = (uint64_t)(RC_U64X(v1) ^ (uint64_t)(uint16_t)0));
    if (RC_U64X(v1) == 0) do { const a4::q128 _v = (a4::mk128(h6, a1_sx64(a2))); h2 = a4::hi64(_v); std::memcpy(&v0, &_v, 8); } while (0);
    { A1_W16B(a0, 40, (uint16_t)(uint32_t)v0); io_h2 = h2; return a1::RetVF{(uint64_t)v0, f0}; }
    __builtin_unreachable();
}

// GAMEUTIL_is_nearly_equal_zero3D_x11 (0x107ec70): typed by tools/typed_lift2.py (round A6; leaf, extra outputs xo[0]=f1; VU0 in -, out -)
inline a1::RetVF GAMEUTIL_is_nearly_equal_zero3D_x11(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t a0, float f12, uint64_t *xo = nullptr)
{
    float f0 = 0.0f;
    float f1 = 0.0f;
    uint64_t v0 = 0;
    uint32_t fcr31 = 0;
    f0 = a1_flt(0u);
    f1 = a1_flt(A1_R32(a0));
    fcr31 = (a1f::lt(f1, f0)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
    if ((fcr31 & 0x800000u) == 0u) goto L_107eca0;
    f0 = (-(float)(f1));
    fcr31 = (a1f::le(f0, f12)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
    if ((fcr31 & 0x800000u) != 0u) { f1 = a1_flt(A1_R32B(a0, 4)); goto L_107ecb0; }
    { (v0 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL)); if (xo) { xo[0] = a1_bits(f1); } return a1::RetVF{(uint64_t)v0, f0}; }
L_107eca0:;
    fcr31 = (a1f::le(f1, f12)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
    { const bool c = (fcr31 & 0x800000u) == 0u; (v0 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL)); if (c) goto L_107ed28; }
    f1 = a1_flt(A1_R32B(a0, 4));
L_107ecb0:;
    f0 = a1_flt(0u);
    fcr31 = (a1f::lt(f1, f0)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
    if ((fcr31 & 0x800000u) == 0u) goto L_107ecd8;
    f0 = (-(float)(f1));
    fcr31 = (a1f::le(f0, f12)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
    if ((fcr31 & 0x800000u) != 0u) { f1 = a1_flt(A1_R32B(a0, 8)); goto L_107ece8; }
    { (v0 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL)); if (xo) { xo[0] = a1_bits(f1); } return a1::RetVF{(uint64_t)v0, f0}; }
L_107ecd8:;
    fcr31 = (a1f::le(f1, f12)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
    { const bool c = (fcr31 & 0x800000u) == 0u; (v0 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL)); if (c) goto L_107ed28; }
    f1 = a1_flt(A1_R32B(a0, 8));
L_107ece8:;
    f0 = a1_flt(0u);
    fcr31 = (a1f::lt(f1, f0)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
    if ((fcr31 & 0x800000u) == 0u) goto L_107ed10;
    f0 = (-(float)(f1));
    fcr31 = (a1f::le(f0, f12)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
    if ((fcr31 & 0x800000u) != 0u) goto L_107ed1c;
    { (v0 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL)); if (xo) { xo[0] = a1_bits(f1); } return a1::RetVF{(uint64_t)v0, f0}; }
L_107ed10:;
    fcr31 = (a1f::le(f1, f12)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
    if ((fcr31 & 0x800000u) == 0u) { (v0 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL)); goto L_107ed28; }
L_107ed1c:;
    { (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 1)))); if (xo) { xo[0] = a1_bits(f1); } return a1::RetVF{(uint64_t)v0, f0}; }
L_107ed28:;
    { if (xo) { xo[0] = a1_bits(f1); } return a1::RetVF{(uint64_t)v0, f0}; }
    __builtin_unreachable();
}

// GetMotionMatrixOfTargetFromParent_x11 (0x10b7aa0): typed by tools/typed_lift2.py (round A6; guest frame leaf, extra outputs xo[0]=f0; VU0 in vf0 vf10 vf12 vf13 vf14 vf15 vf16 vf17 vacc vstat, out vf4 vf5 vf6 vf10 vf11 vf12 vf13 vf14 vf15 vf16 vf17 vf24 vf25 vf26 vf27 vacc vq vstat h2)
inline uint64_t GetMotionMatrixOfTargetFromParent_x11(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint64_t a0, uint32_t a1, uint32_t a2, uint32_t a3, uint32_t t0, uint32_t t1, uint64_t t2, uint32_t sp, const a4::vec4 &io_vf0, a4::vec4 &io_vf4, a4::vec4 &io_vf5, a4::vec4 &io_vf6, a4::vec4 &io_vf10, a4::vec4 &io_vf11, a4::vec4 &io_vf12, a4::vec4 &io_vf13, a4::vec4 &io_vf14, a4::vec4 &io_vf15, a4::vec4 &io_vf16, a4::vec4 &io_vf17, a4::vec4 &io_vf24, a4::vec4 &io_vf25, a4::vec4 &io_vf26, a4::vec4 &io_vf27, a4::vec4 &io_vacc, float &io_vq, uint16_t &io_vstat, uint64_t &io_h2, uint64_t *xo = nullptr)
{
    float f0 = 0.0f;
    uint32_t at = 0;
    uint64_t v0 = 0;
    uint32_t v1 = 0;
    uint32_t t3 = 0;
    uint32_t t4 = 0;
    uint32_t t5 = 0;
    uint64_t t6 = 0;
    a4::vec4 vf0 = io_vf0;
    a4::vec4 vf4 = io_vf4;
    a4::vec4 vf5 = io_vf5;
    a4::vec4 vf6 = io_vf6;
    a4::vec4 vf10 = io_vf10;
    a4::vec4 vf11 = io_vf11;
    a4::vec4 vf12 = io_vf12;
    a4::vec4 vf13 = io_vf13;
    a4::vec4 vf14 = io_vf14;
    a4::vec4 vf15 = io_vf15;
    a4::vec4 vf16 = io_vf16;
    a4::vec4 vf17 = io_vf17;
    a4::vec4 vf24 = io_vf24;
    a4::vec4 vf25 = io_vf25;
    a4::vec4 vf26 = io_vf26;
    a4::vec4 vf27 = io_vf27;
    a4::vec4 vacc = io_vacc;
    float vq = io_vq;
    uint16_t vstat = io_vstat;
    uint64_t h2 = io_h2;
    bool a3xw0 = false;
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B(a2, 8))));
    (v1 = (uint32_t)((int32_t)((int32_t)SLL32((uint32_t)t2, 6))));
    (t6 = (uint64_t)((uint64_t)a0 + (uint64_t)0ULL));
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)SLL32((uint32_t)t2, 4))));
    (t5 = (uint32_t)((int32_t)((int32_t)ADD32((uint32_t)v0, v1))));
    sp = sp - 0x50u;
    (a3 = (uint32_t)((int32_t)((int32_t)ADD32(a3, (uint32_t)a0))));
    t4 = t5 + 0x10u;
    (t3 = (uint32_t)((int32_t)((int32_t)ADD32((uint32_t)t6, v1))));
    { const bool c = (int32_t)t2 <= 0; (a2 = (uint32_t)((int32_t)((int32_t)ADD32(a1, (uint32_t)a0)))); if (c) goto L_10b7adc; }
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)1 << 16))));
    (v0 = (uint64_t)(t2 & RC_U64X(v0)));
    if ((int64_t)v0 != 0) goto L_10b7ce8;
L_10b7adc:;
    v1 = sp;
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(sp, 64))));
    vf5 = a4v::castsi128_ps(A4_R128(ADD32(t4, 0)));
    vf6 = a4v::castsi128_ps(A4_R128(ADD32(t1, 0)));
    { a4::vec4 res = a4v::mul(vf5, vf6); a4::q128 mask = a4v::set_epi32(0, -1, -1, -1); vf5 = a4v::blend(vf5, res, a4v::castsi128_ps(mask)); }
    A4_W128(ADD32((uint32_t)v0, 0), a4v::castps_si128(vf5));
    at = 0x3F800000u;
    { uint32_t bits = a7_opq(at); std::memcpy(&f0, &bits, sizeof(bits)); } a3xw0 = true;
    A1_W32B(sp, 76, a1_bits(f0));
    { a4::vec4 res = a4v::addK<1, 1>(vf0, a4v::bcast<3>(vf0)); a4::q128 mask = a4v::set_epi32(0, -1, -1, -1); vf12 = a4v::blendv_ps(vf12, res, a4v::castsi128_ps(mask)); }
    vf11 = a4v::castsi128_ps(A4_R128(ADD32(a3, 0)));
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
    A4_W128(ADD32(v1, 0), a4v::castps_si128(vf14));
    A4_W128(ADD32(v1, 16), a4v::castps_si128(vf15));
    A4_W128(ADD32(v1, 32), a4v::castps_si128(vf16));
    vf4 = a4v::castsi128_ps(A4_R128(ADD32((uint32_t)v0, 0)));
    { a4::q128 mask = a4v::set_epi32(-1, 0, 0, 0); vf4 = a4v::blendv_ps(vf4, vf0, a4v::castsi128_ps(mask)); }
    A4_W128(ADD32(v1, 48), a4v::castps_si128(vf4));
    v1 = A1_R32B(t5, 56);
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)281 << 16))));
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)a0, 4294952128))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)SLL32(v1, 6))));
    (t1 = (uint32_t)((int32_t)((int32_t)ADD32((uint32_t)a0, 4294967248))));
    { const bool c = (int32_t)v1 >= 0; (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v0, (uint32_t)t6)))); if (c) goto L_10b7c18; }
    do { const a4::q128 _v = (A4_R128(ADD32(t0, 0))); h2 = a4::hi64(_v); std::memcpy(&v0, &_v, 8); } while (0);
    A4_W128(ADD32((uint32_t)a0, 0), a4::mk128(h2, v0));
    vf14 = a4v::castsi128_ps(A4_R128(ADD32(t1, 0)));
    vf15 = a4v::castsi128_ps(A4_R128(ADD32(t1, 16)));
    vf16 = a4v::castsi128_ps(A4_R128(ADD32(t1, 32)));
    vf17 = a4v::castsi128_ps(A4_R128(ADD32(t1, 48)));
    vf24 = a4v::castsi128_ps(A4_R128(ADD32(sp, 0)));
    vf25 = a4v::castsi128_ps(A4_R128(ADD32(sp, 16)));
    vf26 = a4v::castsi128_ps(A4_R128(ADD32(sp, 32)));
    vf27 = a4v::castsi128_ps(A4_R128(ADD32(sp, 48)));
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
    A4_W128(ADD32(t3, 0), a4v::castps_si128(vf24));
    A4_W128(ADD32(t3, 16), a4v::castps_si128(vf25));
    A4_W128(ADD32(t3, 32), a4v::castps_si128(vf26));
    A4_W128(ADD32(t3, 48), a4v::castps_si128(vf27));
    do { const a4::q128 _v = (A4_R128(ADD32(a3, 0))); h2 = a4::hi64(_v); std::memcpy(&v0, &_v, 8); } while (0);
    A4_W128(ADD32(a2, 0), a4::mk128(h2, v0));
    goto L_10b7cc4;
L_10b7c18:;
    vf14 = a4v::castsi128_ps(A4_R128(ADD32((uint32_t)v0, 0)));
    vf15 = a4v::castsi128_ps(A4_R128(ADD32((uint32_t)v0, 16)));
    vf16 = a4v::castsi128_ps(A4_R128(ADD32((uint32_t)v0, 32)));
    vf17 = a4v::castsi128_ps(A4_R128(ADD32((uint32_t)v0, 48)));
    vf24 = a4v::castsi128_ps(A4_R128(ADD32(sp, 0)));
    vf25 = a4v::castsi128_ps(A4_R128(ADD32(sp, 16)));
    vf26 = a4v::castsi128_ps(A4_R128(ADD32(sp, 32)));
    vf27 = a4v::castsi128_ps(A4_R128(ADD32(sp, 48)));
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
    A4_W128(ADD32(t3, 0), a4v::castps_si128(vf24));
    A4_W128(ADD32(t3, 16), a4v::castps_si128(vf25));
    A4_W128(ADD32(t3, 32), a4v::castps_si128(vf26));
    A4_W128(ADD32(t3, 48), a4v::castps_si128(vf27));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)SLL32(v1, 4))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v0, a1))));
    vf11 = a4v::castsi128_ps(A4_R128(ADD32((uint32_t)v0, 0)));
    vf12 = a4v::castsi128_ps(A4_R128(ADD32(a3, 0)));
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
    A4_W128(ADD32(a2, 0), a4v::castps_si128(vf13));
L_10b7cc4:;
    vf14 = a4v::castsi128_ps(A4_R128(ADD32(a2, 0)));
    { a4::vec4 res = a4v::mul(vf14, vf14); a4::q128 mask = a4v::set_epi32(-1, -1, -1, -1); vf15 = a4v::blend(vf15, res, a4v::castsi128_ps(mask)); }
    { a4::vec4 res = a4v::addK<1, 1>(vf15, a4v::bcast<1>(vf15)); a4::q128 mask = a4v::set_epi32(0, 0, 0, -1); vf15 = a4v::blendv_ps(vf15, res, a4v::castsi128_ps(mask)); }
    { a4::vec4 res = a4v::addK<1, 1>(vf15, a4v::bcast<2>(vf15)); a4::q128 mask = a4v::set_epi32(0, 0, 0, -1); vf15 = a4v::blendv_ps(vf15, res, a4v::castsi128_ps(mask)); }
    { a4::vec4 res = a4v::addK<1, 1>(vf15, a4v::bcast<3>(vf15)); a4::q128 mask = a4v::set_epi32(0, 0, 0, -1); vf15 = a4v::blendv_ps(vf15, res, a4v::castsi128_ps(mask)); }
    { float fs = a4v::cvtss_f32(a4v::shuffle_ps<0x03>(vf0, vf0)); float ft = a4v::cvtss_f32(a4v::bcast<0>(vf15)); vq = a4v::rsqrt(vstat, fs, ft); }
    { a4::vec4 res = a4v::mul(vf14, a4v::set1_ps(vq)); a4::q128 mask = a4v::set_epi32(-1, -1, -1, -1); vf15 = a4v::blendv_ps(vf15, res, a4v::castsi128_ps(mask)); }
    A4_W128(ADD32(a2, 0), a4v::castps_si128(vf15));
L_10b7ce8:;
    { sp = sp + 0x50u; if (xo) { if (a3xw0) xo[0] = a1_bits(f0); } io_vf4 = vf4; io_vf5 = vf5; io_vf6 = vf6; io_vf10 = vf10; io_vf11 = vf11; io_vf12 = vf12; io_vf13 = vf13; io_vf14 = vf14; io_vf15 = vf15; io_vf16 = vf16; io_vf17 = vf17; io_vf24 = vf24; io_vf25 = vf25; io_vf26 = vf26; io_vf27 = vf27; io_vacc = vacc; io_vq = vq; io_vstat = vstat; io_h2 = h2; return v0; }
    __builtin_unreachable();
}

// GetCharacterClass_x11 (0x10c0a70): typed by tools/typed_lift2.py (round A6; leaf, extra outputs xo[0]=a0 xo[1]=a1; VU0 in h5, out h2)
inline uint64_t GetCharacterClass_x11(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint64_t a0, uint64_t &io_h2, uint64_t io_h5, uint64_t *xo = nullptr)
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

// GetLightClass_x11 (0x10f93f0): typed by tools/typed_lift2.py (round A6; leaf, extra outputs xo[0]=v1 xo[1]=a0; VU0 in -, out h2)
inline uint64_t GetLightClass_x11(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint64_t a0, uint64_t &io_h2, uint64_t *xo = nullptr)
{
    uint64_t v0 = 0;
    uint64_t v1 = 0;
    uint64_t h2 = io_h2;
    (v0 = (uint64_t)((uint64_t)a0 + (uint64_t)0ULL));
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 19))));
    (v1 = (uint64_t)(uint32_t)((uint8_t)A1_R8B((uint32_t)v0, 8)));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v0, 32))));
    (v1 = (uint64_t)(RC_U64X(v1) ^ RC_U64X(a0)));
    { if (RC_U64X(v1) != 0) do { const a4::q128 _v = (a4v::setzero_si128()); h2 = a4::hi64(_v); std::memcpy(&v0, &_v, 8); } while (0); if (xo) { xo[0] = v1; xo[1] = a0; } io_h2 = h2; return v0; }
    __builtin_unreachable();
}

// GetScriptAnimationClass_x11 (0x110a838): typed by tools/typed_lift2.py (round A6; leaf, extra outputs xo[0]=v1 xo[1]=a0; VU0 in -, out h2)
inline uint64_t GetScriptAnimationClass_x11(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint64_t a0, uint64_t &io_h2, uint64_t *xo = nullptr)
{
    uint64_t v0 = 0;
    uint64_t v1 = 0;
    uint64_t h2 = io_h2;
    (v0 = (uint64_t)((uint64_t)a0 + (uint64_t)0ULL));
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 13))));
    (v1 = (uint64_t)(uint32_t)((uint8_t)A1_R8B((uint32_t)v0, 8)));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v0, 32))));
    (v1 = (uint64_t)(RC_U64X(v1) ^ RC_U64X(a0)));
    { if (RC_U64X(v1) != 0) do { const a4::q128 _v = (a4v::setzero_si128()); h2 = a4::hi64(_v); std::memcpy(&v0, &_v, 8); } while (0); if (xo) { xo[0] = v1; xo[1] = a0; } io_h2 = h2; return v0; }
    __builtin_unreachable();
}

// EntryModifiedManager_x11 (0x11478b8): typed by tools/typed_lift2.py (round A6; leaf, extra outputs xo[0]=a0 xo[1]=a2 xo[2]=a3 xo[3]=t0 xo[4]=t1 xo[5]=t2; VU0 in -, out -)
inline uint64_t EntryModifiedManager_x11(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint64_t a0, uint32_t a1, uint64_t *xo = nullptr)
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

// dynamic_cast_to_camera_obj_x11 (0x1162e08): typed by tools/typed_lift2.py (round A6; leaf, extra outputs xo[0]=v1; VU0 in -, out -, break)
inline uint64_t dynamic_cast_to_camera_obj_x11(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint64_t a0, float f0, float f1, float f2, float f3, float f4, float f5, float f6, float f7, float f8, float f9, float f10, float f11, float f12, float f13, float f14, float f15, float f16, float f17, float f18, float f19, float f20, float f21, float f22, float f23, float f24, float f25, float f26, float f27, float f28, float f29, float f30, float f31, uint64_t *xo = nullptr)
{
    uint64_t v0 = 0;
    uint32_t v1 = 0;
    (v0 = (uint64_t)((uint64_t)a0 + (uint64_t)0ULL));
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 17))));
    (v1 = (uint32_t)((uint8_t)A1_R8B((uint32_t)v0, 8)));
    if ((int64_t)(int32_t)v1 == (int64_t)a0) goto L_1162e24;
    A6_BREAK(0x1162E1Cu);
    (v0 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL));
L_1162e24:;
    { if (xo) { xo[0] = a1_sx64(v1); } return v0; }
    __builtin_unreachable();
}

// AI_GetAccelB3XYZ_x11 (0x13d4308): typed by tools/typed_lift2.py (round A6; frame leaf, extra outputs xo[0]=f12; VU0 in vf7 h2, out vf5 vf6 vf7, frame 16 bytes, 6 slot accesses)
inline a1::RetVF AI_GetAccelB3XYZ_x11(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t a0, uint32_t a1, uint32_t a2, uint32_t a3, uint32_t t0, uint32_t t1, float f12, a4::vec4 &io_vf5, a4::vec4 &io_vf6, a4::vec4 &io_vf7, uint64_t io_h2, uint64_t *xo = nullptr)
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
    bool a3xw0 = false;
    at = 0x3D080000u;
    at = at | 0xAB85u;
    f0 = a1_flt(a7_opq(at));
    fcr31 = (a1f::lt(f12, f0)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
    if ((fcr31 & 0x800000u) == 0u) goto L_13d4324;
    f12 = ((float)(f0)); a3xw0 = true;
L_13d4324:;
    f1 = a1f::mul(f12, f12);
    vf5 = a4v::castsi128_ps(A4_R128(ADD32(t0, 0)));
    vf6 = a4v::castsi128_ps(A4_R128(ADD32(a3, 0)));
    { a4::vec4 res = a4v::sub(vf5, vf6); a4::q128 mask = a4v::set_epi32(-1, -1, -1, -1); vf7 = a4v::blend(vf7, res, a4v::castsi128_ps(mask)); }
    a2::frSt<a4::q128>(a2fr, 0, a4v::castps_si128(vf7));
    at = 0x40800000u;
    f0 = a1_flt(a7_opq(at));
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
    f0 = a1_flt(a7_opq(at));
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
    f0 = a1_flt(a7_opq(at));
    f0 = a1f::div(fcr31, f0, f1);
    v0 = a1_bits(f0);
    vf5 = a4v::castsi128_ps(A4_R128(ADD32(a0, 0)));
    vf6 = a4v::castsi128_ps(a4::mk128(h2, a1_sx64(v0)));
    { a4::vec4 res = a4v::mul(vf5, a4v::bcast<0>(vf6)); a4::q128 mask = a4v::set_epi32(-1, -1, -1, -1); vf5 = a4v::blendv_ps(vf5, res, a4v::castsi128_ps(mask)); }
    A4_W128(ADD32(a0, 0), a4v::castps_si128(vf5));
    { if (xo) { if (a3xw0) xo[0] = a1_bits(f12); } io_vf5 = vf5; io_vf6 = vf6; io_vf7 = vf7; return a1::RetVF{(uint64_t)a1_sx64(v0), f0}; }
    __builtin_unreachable();
}

// GetScriptLayoutClass_x11 (0x110d058): typed by tools/typed_lift2.py (round A6; frame calls, extra outputs xo[0]=v1 xo[1]=a0 xo[2]=a1; VU0 in -, out h2, frame 16 bytes, 0 slot accesses)
inline uint64_t GetScriptLayoutClass_x11(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint64_t a0, uint64_t &io_h2, uint64_t *xo = nullptr)
{
    uint64_t v0 = 0;
    uint64_t v1 = 0;
    uint32_t a1 = 0;
    uint64_t h2 = io_h2;
    (v0 = (uint64_t)((uint64_t)a0 + (uint64_t)0ULL));
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 11))));
    (a1 = (uint32_t)((uint8_t)A1_R8B((uint32_t)v0, 8)));
    { const bool c = (int64_t)(int32_t)a1 == (int64_t)v1; (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v0, 32)))); if (c) goto L_110d090; }
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 13))));
    if ((int64_t)(int32_t)a1 != (int64_t)v0) { (v0 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL)); goto L_110d090; }
    { uint64_t a7c[2] = {0u, 0u}; const uint64_t a7_t = GetScriptAnimationClass_x11(ram, rt, a0, h2, a7c); v1 = a7c[0]; a0 = a7c[1]; if (xo) { xo[0] = v1; xo[1] = a0; xo[2] = a1_sx64(a1); } io_h2 = h2; return a7_t; }
L_110d090:;
    { if (xo) { xo[0] = v1; xo[2] = a1_sx64(a1); } io_h2 = h2; return v0; }
    __builtin_unreachable();
}

// setSCRAttainRange_x11 (0x1456098): typed by tools/typed_lift2.py (round A6; guest frame calls, extra outputs xo[0]=f0 xo[1]=f1 xo[2]=f2 xo[3]=f3; VU0 in -, out -)
inline uint64_t setSCRAttainRange_x11(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t a0, uint64_t a1, uint32_t sp, uint64_t *xo = nullptr)
{
    float f0 = 0.0f;
    float f1 = 0.0f;
    float f2 = 0.0f;
    float f3 = 0.0f;
    uint32_t at = 0;
    uint64_t v0 = 0;
    uint32_t v1 = 0;
    uint64_t a2 = 0;
    uint32_t a3 = 0;
    uint32_t t0 = 0;
    uint64_t hi = 0;
    uint64_t lo = 0;
    bool a3xw0 = false;
    bool a3xw1 = false;
    bool a3xw2 = false;
    bool a3xw3 = false;
    sp = sp - 0x30u;
    (v0 = (uint64_t)((uint64_t)a1 + (uint64_t)0ULL));
    v1 = 0x30000u;
    v1 = v1 | 0xFFFFu;
    (a1 = (uint64_t)((uint64_t)a1_sx64(sp) + (uint64_t)0ULL));
    (t0 = (uint32_t)((int32_t)((int32_t)A1_R32B((uint32_t)v0, 0))));
    (a2 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL));
    v1 = t0 & v1;
    { const bool c = v1 == 0; (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)SUB32((uint32_t)v0, v1)))); if (c) goto L_14560c8; }
    (a2 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)v0, 0))));
L_14560c8:;
    { const bool c = (int64_t)a2 == 0; { const uint32_t _dynInsn = A1_R32(0x14560CCu); const int32_t _dynSimm = (int16_t)(_dynInsn & 0xFFFFu); const uint32_t _dynImm = _dynInsn & 0xFFFFu; (void)_dynSimm; (void)_dynImm; (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)_dynImm << 16)))); } if (c) goto L_14560f8; }
    { const uint32_t _dynInsn = A1_R32(0x14560D0u); const int32_t _dynSimm = (int16_t)(_dynInsn & 0xFFFFu); const uint32_t _dynImm = _dynInsn & 0xFFFFu; (void)_dynSimm; (void)_dynImm; (v1 = (uint32_t)((int32_t)((int32_t)A1_R32(ADD32((uint32_t)v0, _dynSimm))))); }
    { const bool c = (int64_t)a2 == (int64_t)(int32_t)v1; (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)SRL32(t0, 18)))); if (c) goto L_14560f8; }
    (a3 = (uint32_t)((int32_t)((int32_t)A1_R32B((uint32_t)a2, 8))));
    (v0 = (uint64_t)(RC_U64X(v0) & (uint64_t)(uint16_t)4095));
    (a2 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)a2, 16))));
    v1 = A1_R32B(a3, 12);
    { int64_t result = (int64_t)(int32_t)v0 * (int64_t)(int32_t)v1; lo = (uint64_t)(int64_t)(int32_t)result; hi = (uint64_t)(int64_t)(int32_t)(result >> 32); (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)result))); }
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)a2, (uint32_t)v0))));
    goto L_14560fc;
L_14560f8:;
    (v0 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL));
L_14560fc:;
    if ((int64_t)v0 == 0) goto L_1456160;
    { uint32_t bits = A1_R32B((uint32_t)v0, 0); float f; std::memcpy(&f, &bits, sizeof(f)); f1 = f; } a3xw1 = true;
    at = 0x3F800000u;
    { uint32_t bits = a7_opq(at); std::memcpy(&f2, &bits, sizeof(bits)); } a3xw2 = true;
    A1_W32(sp, a1_bits(f1));
    at = 0x3C8E0000u;
    at = at | 0xFA35u;
    f3 = a1_flt(a7_opq(at)); a3xw3 = true;
    { uint32_t bits = A1_R32B((uint32_t)v0, 4); float f; std::memcpy(&f, &bits, sizeof(f)); f0 = f; } a3xw0 = true;
    A1_W32B(sp, 4, a1_bits(f0));
    { uint32_t bits = A1_R32B((uint32_t)v0, 8); float f; std::memcpy(&f, &bits, sizeof(f)); f1 = f; } a3xw1 = true;
    { float f = f2; uint32_t bits; std::memcpy(&bits, &f, sizeof(bits)); A1_W32B(sp, 12, bits); }
    A1_W32B(sp, 8, a1_bits(f1));
    { uint32_t bits = A1_R32B((uint32_t)v0, 12); float f; std::memcpy(&f, &bits, sizeof(f)); f0 = f; } a3xw0 = true;
    A1_W32B(sp, 16, a1_bits(f0));
    { uint32_t bits = A1_R32B((uint32_t)v0, 16); float f; std::memcpy(&f, &bits, sizeof(f)); f1 = f; } a3xw1 = true;
    A1_W32B(sp, 20, a1_bits(f1));
    { uint32_t bits = A1_R32B((uint32_t)v0, 20); float f; std::memcpy(&f, &bits, sizeof(f)); f0 = f; } a3xw0 = true;
    f0 = a1f::mul(f0, f3); a3xw0 = true;
    A1_W32B(sp, 24, a1_bits(f0));
    { uint32_t bits = A1_R32B((uint32_t)v0, 24); float f; std::memcpy(&f, &bits, sizeof(f)); f1 = f; } a3xw1 = true;
    { A1_W32B(sp, 28, a1_bits(f1)); A3_CHARGE(); v0 = setAttainRangeStruct(ram, rt, a0, (uint32_t)a1); }
L_1456160:;
    { sp = sp + 0x30u; if (xo) { if (a3xw0) xo[0] = a1_bits(f0); if (a3xw1) xo[1] = a1_bits(f1); if (a3xw2) xo[2] = a1_bits(f2); if (a3xw3) xo[3] = a1_bits(f3); } return v0; }
    __builtin_unreachable();
}

// gamecore_sub_01462340_x11 (0x1462340): typed by tools/typed_lift2.py (round A6; loop guest frame calls, extra outputs xo[0]=f1 xo[1]=f2 xo[2]=f3 xo[3]=f4 xo[4]=f5 xo[5]=f6 xo[6]=f7 xo[7]=f13; VU0 in vf0 vf7 vf8 vacc vstat h3 h4 h6 h8, out vf5 vf6 vf7 vf8 vacc vq vstat h2, relocated calls 1)
inline a1::RetVF gamecore_sub_01462340_x11(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint64_t a0, uint64_t a1, uint64_t a2, uint32_t a3, uint32_t sp, float f12, const a4::vec4 &io_vf0, a4::vec4 &io_vf5, a4::vec4 &io_vf6, a4::vec4 &io_vf7, a4::vec4 &io_vf8, a4::vec4 &io_vacc, float &io_vq, uint16_t &io_vstat, uint64_t &io_h2, uint64_t io_h3, uint64_t io_h4, uint64_t io_h6, uint64_t io_h8, uint64_t *xo = nullptr)
{
    float f0 = 0.0f;
    float f1 = 0.0f;
    float f2 = 0.0f;
    float f3 = 0.0f;
    float f4 = 0.0f;
    float f5 = 0.0f;
    float f6 = 0.0f;
    float f7 = 0.0f;
    float f13 = 0.0f;
    uint32_t at = 0;
    uint64_t v0 = 0;
    uint32_t v1 = 0;
    uint32_t t0 = 0;
    uint32_t s0 = 0;
    uint64_t s1 = 0;
    uint64_t s2 = 0;
    uint64_t s3 = 0;
    uint32_t fcr31 = 0;
    a4::vec4 vf0 = io_vf0;
    a4::vec4 vf5 = io_vf5;
    a4::vec4 vf6 = io_vf6;
    a4::vec4 vf7 = io_vf7;
    a4::vec4 vf8 = io_vf8;
    a4::vec4 vacc = io_vacc;
    float vq = io_vq;
    uint16_t vstat = io_vstat;
    uint64_t h2 = io_h2;
    uint64_t h3 = io_h3;
    uint64_t h4 = io_h4;
    uint64_t h6 = io_h6;
    uint64_t h8 = io_h8;
    bool a3xw1 = false;
    bool a3xw2 = false;
    bool a3xw3 = false;
    bool a3xw4 = false;
    bool a3xw5 = false;
    bool a3xw6 = false;
    sp = sp - 0x50u;
    (s1 = (uint64_t)((uint64_t)a0 + (uint64_t)0ULL));
    (s2 = (uint64_t)((uint64_t)a2 + (uint64_t)0ULL));
    (s3 = (uint64_t)((uint64_t)a1 + (uint64_t)0ULL));
    (t0 = (uint32_t)((int32_t)((int32_t)A1_R32B((uint32_t)s2, 1540))));
    (a2 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)s2, 1544))));
    (v1 = (uint32_t)((int32_t)((int32_t)SUB32(t0, (uint32_t)a2))));
    (a1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(v1, 32))));
    v1 = v1 + 0x3Fu;
    (a0 = (uint64_t)(((int64_t)(int64_t)RC_U64X(a1) < (int64_t)(int32_t)0) ? 1 : 0));
    (v0 = (uint64_t)((uint64_t)RC_U64X(a1) + (uint64_t)0ULL));
    if (RC_U64X(a0) != 0) do { const a4::q128 _v = (a4::mk128(h3, a1_sx64(v1))); h2 = a4::hi64(_v); std::memcpy(&v0, &_v, 8); } while (0);
    (s0 = (uint32_t)((int32_t)(SRA32((int32_t)v0, 5))));
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)SLL32(s0, 5))));
    (s0 = (uint32_t)((int32_t)((int32_t)SUB32((uint32_t)a1, (uint32_t)a0))));
    v1 = s0 << 1;
    v1 = v1 + s0;
    v1 = v1 << 4;
    (v1 = (uint32_t)((int32_t)((int32_t)ADD32(v1, (uint32_t)s2))));
    vf5 = a4v::castsi128_ps(A4_R128(ADD32(v1, 0)));
    vf6 = a4v::castsi128_ps(A4_R128(ADD32(a3, 0)));
    { a4::vec4 res = a4v::sub(vf5, vf6); a4::q128 mask = a4v::set_epi32(-1, -1, -1, -1); vf7 = a4v::blend(vf7, res, a4v::castsi128_ps(mask)); }
    { a4::vec4 res = a4v::mulK<1, 1>(vf7, vf7); a4::q128 mask = a4v::set_epi32(0, -1, -1, -1); vf7 = a4v::blend(vf7, res, a4v::castsi128_ps(mask)); }
    { a4::vec4 res = a4v::mulK<1, 1>(vf0, a4v::bcast<0>(vf7)); vacc = a4v::dest<0, 0, 0, 1>(vacc, res); }
    { a4::vec4 mul_res = a4v::mulK<1, 1>(vf0, a4v::bcast<1>(vf7)); a4::vec4 res = a4v::addK<1, 1>(vacc, mul_res); vacc = a4v::dest<0, 0, 0, 1>(vacc, res); }
    { a4::vec4 mul_res = a4v::mulK<1, 1>(vf0, a4v::bcast<2>(vf7)); a4::vec4 res = a4v::addK<1, 1>(vacc, mul_res); a4::q128 mask = a4v::set_epi32(-1, 0, 0, 0); vf7 = a4v::blendv_ps(vf7, res, a4v::castsi128_ps(mask)); }
    { float ft = a4v::cvtss_f32(a4v::shuffle_ps<0x03>(vf7, vf7)); vq = a4v::sqrt(vstat, ft); }
    { uint32_t bits; std::memcpy(&bits, &vq, sizeof(bits)); (v0 = (uint64_t)((int64_t)(int32_t)(bits))); }
    { uint32_t bits = (uint32_t)v0; std::memcpy(&f0, &bits, sizeof(bits)); }
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(s0, (uint32_t)s2))));
    { uint32_t bits = A1_R32B((uint32_t)s2, 1536); float f; std::memcpy(&f, &bits, sizeof(f)); f13 = f; }
    (v1 = (uint32_t)((int32_t)((int8_t)A1_R8B((uint32_t)a0, 1549))));
    (v0 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL));
    { const bool c = v1 != 0; f1 = a1f::sub(f13, f0); if (c) goto L_146260c; }
    (v0 = (uint64_t)(((int64_t)(int64_t)RC_U64X(a2) < (int64_t)(int32_t)31) ? 1 : 0));
    { const bool c = (int64_t)v0 == 0; (v0 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL)); if (c) goto L_146260c; }
L_14623f0:;
    (v1 = (uint32_t)((int32_t)((int32_t)SUB32(t0, (uint32_t)a2))));
    fcr31 = (a1f::lt(f12, f0)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
    (a1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(v1, 32))));
    v1 = v1 + 0x3Fu;
    (a0 = (uint64_t)(((int64_t)(int64_t)RC_U64X(a1) < (int64_t)(int32_t)0) ? 1 : 0));
    (v0 = (uint64_t)((uint64_t)RC_U64X(a1) + (uint64_t)0ULL));
    if (RC_U64X(a0) != 0) do { const a4::q128 _v = (a4::mk128(h3, a1_sx64(v1))); h2 = a4::hi64(_v); std::memcpy(&v0, &_v, 8); } while (0);
    (s0 = (uint32_t)((int32_t)(SRA32((int32_t)v0, 5))));
    v1 = s0 << 5;
    { const bool c = (fcr31 & 0x800000u) == 0u; (s0 = (uint32_t)((int32_t)((int32_t)SUB32((uint32_t)a1, v1)))); if (c) goto L_14625f0; }
    { f12 = a1f::add(f1, f12); A3_CHARGE(); f0 = iosFloatModulo(ram, rt, f12, f13); }
    { uint32_t bits = A1_R32B((uint32_t)s2, 1536); float f; std::memcpy(&f, &bits, sizeof(f)); f7 = f; } a3xw6 = true;
    at = 0x3F800000u;
    { uint32_t bits = a7_opq(at); std::memcpy(&f1, &bits, sizeof(bits)); }
    f6 = a1f::div(fcr31, f0, f7); a3xw5 = true;
    f0 = ((float)(f6));
    fcr31 = (a1f::le(f0, f1)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
    if ((fcr31 & 0x800000u) == 0u) { f0 = ((float)(f1)); goto L_146244c; }
L_146244c:;
    { uint32_t bits = 0u; std::memcpy(&f1, &bits, sizeof(bits)); }
    fcr31 = (a1f::le(f1, f0)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
    if ((fcr31 & 0x800000u) == 0u) { f0 = ((float)(f1)); goto L_146245c; }
L_146245c:;
    f6 = ((float)(f0)); a3xw5 = true;
    at = 0x40400000u;
    { uint32_t bits = a7_opq(at); std::memcpy(&f3, &bits, sizeof(bits)); } a3xw2 = true;
    f2 = ((float)(f6) * (float)(f6)); a3xw1 = true;
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)s2, 1544))));
    (v1 = (uint32_t)((int32_t)((int32_t)A1_R32B((uint32_t)s2, 1540))));
    at = 0x3F800000u;
    { uint32_t bits = a7_opq(at); std::memcpy(&f5, &bits, sizeof(bits)); } a3xw4 = true;
    (v1 = (uint32_t)((int32_t)((int32_t)SUB32(v1, (uint32_t)v0))));
    f1 = ((float)(f6) * (float)(f2));
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(v1, 65))));
    f3 = ((float)(f2) * (float)(f3)); a3xw2 = true;
    (a1 = (uint64_t)(((int64_t)(int64_t)RC_U64X(a0) < (int64_t)(int32_t)0) ? 1 : 0));
    f4 = a1f::addFinite(f2, f2); a3xw3 = true;
    v1 = v1 + 0x60u;
    (v0 = (uint64_t)((uint64_t)RC_U64X(a0) + (uint64_t)0ULL));
    f0 = a1f::addFinite(f1, f1);
    if (RC_U64X(a1) != 0) do { const a4::q128 _v = (a4::mk128(h3, a1_sx64(v1))); h2 = a4::hi64(_v); std::memcpy(&v0, &_v, 8); } while (0);
    f2 = a1f::subFinite(f1, f2); a3xw1 = true;
    (v0 = (uint64_t)((int64_t)(int32_t)(SRA32((int32_t)v0, 5))));
    f1 = a1f::subFinite(f1, f4);
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)SLL32((uint32_t)v0, 5))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)SUB32((uint32_t)a0, (uint32_t)v0))));
    f0 = a1f::subFinite(f0, f3);
    (a1 = (uint64_t)((int64_t)(int32_t)((int32_t)SLL32((uint32_t)v0, 1))));
    { float f = f2; uint32_t bits; std::memcpy(&bits, &f, sizeof(bits)); A1_W32B(sp, 12, bits); }
    (a1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)a1, (uint32_t)v0))));
    f1 = a1f::addFinite(f1, f6);
    (a1 = (uint64_t)((int64_t)(int32_t)((int32_t)SLL32((uint32_t)a1, 4))));
    (a1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)a1, (uint32_t)s2))));
    f2 = (-(float)(f0)); a3xw1 = true;
    f0 = a1f::addFinite(f0, f5);
    A1_W32B(sp, 8, a1_bits(f1));
    { float f = f2; uint32_t bits; std::memcpy(&bits, &f, sizeof(bits)); A1_W32B(sp, 4, bits); }
    A1_W32(sp, a1_bits(f0));
    { uint32_t bits; std::memcpy(&bits, &f0, sizeof(bits)); (v0 = (uint64_t)((int64_t)(int32_t)(bits))); }
    vf5 = a4v::castsi128_ps(A4_R128(ADD32((uint32_t)a1, 0)));
    vf6 = a4v::castsi128_ps(a4::mk128(h2, v0));
    { a4::vec4 res = a4v::mul(vf5, a4v::bcast<0>(vf6)); a4::q128 mask = a4v::set_epi32(-1, -1, -1, -1); vf5 = a4v::blendv_ps(vf5, res, a4v::castsi128_ps(mask)); }
    A4_W128(ADD32((uint32_t)s1, 0), a4v::castps_si128(vf5));
    v1 = s0 << 1;
    f0 = a1_flt(A1_R32B(sp, 4));
    v1 = v1 + s0;
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(sp, 16))));
    v1 = v1 << 4;
    { uint32_t bits; std::memcpy(&bits, &f0, sizeof(bits)); (a0 = (uint64_t)((int64_t)(int32_t)(bits))); }
    (v1 = (uint32_t)((int32_t)((int32_t)ADD32(v1, (uint32_t)s2))));
    vf5 = a4v::castsi128_ps(A4_R128(ADD32(v1, 0)));
    vf6 = a4v::castsi128_ps(a4::mk128(h4, a0));
    { a4::vec4 res = a4v::mul(vf5, a4v::bcast<0>(vf6)); a4::q128 mask = a4v::set_epi32(-1, -1, -1, -1); vf5 = a4v::blendv_ps(vf5, res, a4v::castsi128_ps(mask)); }
    A4_W128(ADD32((uint32_t)v0, 0), a4v::castps_si128(vf5));
    vf5 = a4v::castsi128_ps(A4_R128(ADD32((uint32_t)s1, 0)));
    vf6 = a4v::castsi128_ps(A4_R128(ADD32((uint32_t)v0, 0)));
    { a4::vec4 res = a4v::add(vf5, vf6); a4::q128 mask = a4v::set_epi32(0, -1, -1, -1); vf5 = a4v::blend(vf5, res, a4v::castsi128_ps(mask)); }
    A4_W128(ADD32((uint32_t)s1, 0), a4v::castps_si128(vf5));
    f0 = a1_flt(A1_R32B(sp, 8));
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)a1, 32))));
    f0 = a1f::mul(f0, f7);
    { uint32_t bits; std::memcpy(&bits, &f0, sizeof(bits)); (a2 = (uint64_t)((int64_t)(int32_t)(bits))); }
    vf5 = a4v::castsi128_ps(A4_R128(ADD32((uint32_t)a0, 0)));
    vf6 = a4v::castsi128_ps(a4::mk128(h6, a2));
    { a4::vec4 res = a4v::mul(vf5, a4v::bcast<0>(vf6)); a4::q128 mask = a4v::set_epi32(-1, -1, -1, -1); vf5 = a4v::blendv_ps(vf5, res, a4v::castsi128_ps(mask)); }
    A4_W128(ADD32((uint32_t)v0, 0), a4v::castps_si128(vf5));
    vf5 = a4v::castsi128_ps(A4_R128(ADD32((uint32_t)s1, 0)));
    vf6 = a4v::castsi128_ps(A4_R128(ADD32((uint32_t)v0, 0)));
    { a4::vec4 res = a4v::add(vf5, vf6); a4::q128 mask = a4v::set_epi32(0, -1, -1, -1); vf5 = a4v::blend(vf5, res, a4v::castsi128_ps(mask)); }
    A4_W128(ADD32((uint32_t)s1, 0), a4v::castps_si128(vf5));
    f0 = a1_flt(A1_R32B(sp, 12));
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(v1, 32))));
    f0 = a1f::mul(f0, f7);
    { uint32_t bits; std::memcpy(&bits, &f0, sizeof(bits)); (a2 = (uint64_t)((int64_t)(int32_t)(bits))); }
    vf5 = a4v::castsi128_ps(A4_R128(ADD32((uint32_t)a0, 0)));
    vf6 = a4v::castsi128_ps(a4::mk128(h6, a2));
    { a4::vec4 res = a4v::mul(vf5, a4v::bcast<0>(vf6)); a4::q128 mask = a4v::set_epi32(-1, -1, -1, -1); vf5 = a4v::blendv_ps(vf5, res, a4v::castsi128_ps(mask)); }
    A4_W128(ADD32((uint32_t)v0, 0), a4v::castps_si128(vf5));
    vf5 = a4v::castsi128_ps(A4_R128(ADD32((uint32_t)s1, 0)));
    vf6 = a4v::castsi128_ps(A4_R128(ADD32((uint32_t)v0, 0)));
    { a4::vec4 res = a4v::add(vf5, vf6); a4::q128 mask = a4v::set_epi32(0, -1, -1, -1); vf5 = a4v::blend(vf5, res, a4v::castsi128_ps(mask)); }
    A4_W128(ADD32((uint32_t)s1, 0), a4v::castps_si128(vf5));
    { float f = f5; uint32_t bits; std::memcpy(&bits, &f, sizeof(bits)); A1_W32B((uint32_t)s1, 12, bits); }
    v1 = v1 + 0x10u;
    (a1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)a1, 16))));
    { uint32_t bits; std::memcpy(&bits, &f6, sizeof(bits)); (t0 = (uint32_t)((int32_t)(bits))); }
    vf7 = a4v::castsi128_ps(a4::mk128(h8, a1_sx64(t0)));
    vf5 = a4v::castsi128_ps(A4_R128(ADD32(v1, 0)));
    vf6 = a4v::castsi128_ps(A4_R128(ADD32((uint32_t)a1, 0)));
    { a4::vec4 res = a4v::subK<1, 0>(vf0, a4v::bcast<0>(vf7)); a4::q128 mask = a4v::set_epi32(-1, 0, 0, 0); vf8 = a4v::blendv_ps(vf8, res, a4v::castsi128_ps(mask)); }
    { a4::vec4 res = a4v::mul(vf5, a4v::bcast<0>(vf7)); vacc = a4v::dest<1, 1, 1, 0>(vacc, res); }
    { a4::vec4 mul_res = a4v::mulK<0, 1>(vf6, a4v::bcast<3>(vf8)); a4::vec4 res = a4v::addK<1, 1>(vacc, mul_res); a4::q128 mask = a4v::set_epi32(0, -1, -1, -1); vf5 = a4v::blendv_ps(vf5, res, a4v::castsi128_ps(mask)); }
    A4_W128(ADD32((uint32_t)s3, 0), a4v::castps_si128(vf5));
    vf5 = a4v::castsi128_ps(A4_R128(ADD32((uint32_t)s3, 0)));
    { a4::vec4 res = a4v::mul(vf5, vf5); a4::q128 mask = a4v::set_epi32(0, -1, -1, -1); vf7 = a4v::blend(vf7, res, a4v::castsi128_ps(mask)); }
    { a4::vec4 res = a4v::mulK<1, 1>(vf0, a4v::bcast<0>(vf7)); vacc = a4v::dest<0, 0, 0, 1>(vacc, res); }
    { a4::vec4 mul_res = a4v::mulK<1, 1>(vf0, a4v::bcast<1>(vf7)); a4::vec4 res = a4v::addK<1, 1>(vacc, mul_res); vacc = a4v::dest<0, 0, 0, 1>(vacc, res); }
    { a4::vec4 mul_res = a4v::mulK<1, 1>(vf0, a4v::bcast<2>(vf7)); a4::vec4 res = a4v::addK<1, 1>(vacc, mul_res); a4::q128 mask = a4v::set_epi32(-1, 0, 0, 0); vf7 = a4v::blendv_ps(vf7, res, a4v::castsi128_ps(mask)); }
    { float fs = a4v::cvtss_f32(a4v::shuffle_ps<0x03>(vf0, vf0)); float ft = a4v::cvtss_f32(a4v::shuffle_ps<0x03>(vf7, vf7)); vq = a4v::rsqrt(vstat, fs, ft); }
    { a4::vec4 res = a4v::mul(vf5, a4v::set1_ps(vq)); a4::q128 mask = a4v::set_epi32(0, -1, -1, -1); vf5 = a4v::blendv_ps(vf5, res, a4v::castsi128_ps(mask)); }
    A4_W128(ADD32((uint32_t)s3, 0), a4v::castps_si128(vf5));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 1))));
    goto L_146260c;
L_14625f0:;
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)a2, 1))));
    f0 = a1f::add(f0, f13);
    (v1 = (uint32_t)(((int64_t)(int64_t)RC_U64X(v0) < (int64_t)(int32_t)31) ? 1 : 0));
    (a2 = (uint64_t)((uint64_t)RC_U64X(v0) + (uint64_t)0ULL));
    { const bool c = v1 != 0; A1_W32B((uint32_t)s2, 1544, (uint32_t)v0); if (c) { A3_BACK(); goto L_14623f0; } }
    (v0 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL));
L_146260c:;
    { sp = sp + 0x50u; if (xo) { xo[0] = a1_bits(f1); if (a3xw1) xo[1] = a1_bits(f2); if (a3xw2) xo[2] = a1_bits(f3); if (a3xw3) xo[3] = a1_bits(f4); if (a3xw4) xo[4] = a1_bits(f5); if (a3xw5) xo[5] = a1_bits(f6); if (a3xw6) xo[6] = a1_bits(f7); xo[7] = a1_bits(f13); } io_vf5 = vf5; io_vf6 = vf6; io_vf7 = vf7; io_vf8 = vf8; io_vacc = vacc; io_vq = vq; io_vstat = vstat; io_h2 = h2; return a1::RetVF{(uint64_t)v0, f0}; }
    __builtin_unreachable();
}

// GetLayoutClass_u11 (0x10f1a20): typed by tools/typed_lift2.py (round A6; frame calls, extra outputs xo[0]=v1 xo[1]=a0 xo[2]=a1; VU0 in -, out h2, jump tables 1, frame 16 bytes, 0 slot accesses)
inline uint64_t GetLayoutClass_u11(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint64_t a0, float f0, float f1, float f2, float f3, float f4, float f5, float f6, float f7, float f8, float f9, float f10, float f11, float f12, float f13, float f14, float f15, float f16, float f17, float f18, float f19, float f20, float f21, float f22, float f23, float f24, float f25, float f26, float f27, float f28, float f29, float f30, float f31, uint64_t &io_h2, uint64_t *xo = nullptr)
{
    uint64_t v0 = 0;
    uint64_t v1 = 0;
    uint64_t a1 = 0;
    uint64_t h2 = io_h2;
    bool a3xw1 = false;
    bool a3xw2 = false;
    (v0 = (uint64_t)(uint32_t)((uint8_t)A1_R8B((uint32_t)a0, 8)));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v0, 4294967287))));
    (v1 = (uint64_t)(((uint64_t)RC_U64X(v0) < (uint64_t)(int64_t)(int32_t)14) ? 1 : 0));
    if ((int64_t)v1 == 0) { (v0 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL)); goto L_10f1aa4; }
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)SLL32((uint32_t)v0, 2))));
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)281 << 16))));
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v1, (uint32_t)v0))));
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)v1, 16496))));
    { const uint32_t a5_jt = (uint32_t)v1; switch (a5_jt) { case 0x10F1A58u: goto L_10f1a58; case 0x10F1A60u: goto L_10f1a60; case 0x10F1A70u: goto L_10f1a70; case 0x10F1A88u: goto L_10f1a88; case 0x10F1A98u: goto L_10f1a98; case 0x10F1AA0u: goto L_10f1aa0; default: A5_BADJUMP(0x10F1A4Cu, a5_jt); } }
L_10f1a58:;
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)a0, 32))));
    goto L_10f1aa4;
L_10f1a60:;
    { A3_CHARGE(); uint64_t a7c[3] = {0u, a0, 0u}; v0 = GetScriptLayoutClass_x11(ram, rt, a0, h2, a7c); v1 = a7c[0]; a0 = a7c[1]; a1 = a7c[2]; a3xw1 = true; a3xw2 = true; }
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v0, 112))));
    goto L_10f1aa4;
L_10f1a70:;
    { A3_CHARGE(); uint64_t a7c[1] = {0u}; v0 = dynamic_cast_to_camera_obj_x11(ram, rt, a0, f0, f1, f2, f3, f4, f5, f6, f7, f8, f9, f10, f11, f12, f13, f14, f15, f16, f17, f18, f19, f20, f21, f22, f23, f24, f25, f26, f27, f28, f29, f30, f31, a7c); v1 = a7c[0]; }
    (a0 = (uint64_t)((uint64_t)v0 + (uint64_t)0ULL)); a3xw1 = true;
    { if (xo) { xo[0] = v1; xo[1] = a0; } const uint32_t a4_t = get_camera_obj_layout_class(ram, rt, (uint32_t)a0); io_h2 = h2; return a1_sx64(a4_t); }
L_10f1a88:;
    { uint64_t a7c[2] = {0u, 0u}; const uint64_t a7_t = GetLightClass_x11(ram, rt, a0, h2, a7c); v1 = a7c[0]; a0 = a7c[1]; if (xo) { xo[0] = v1; xo[1] = a0; } io_h2 = h2; return a7_t; }
L_10f1a98:;
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)a0, 144))));
    goto L_10f1aa4;
L_10f1aa0:;
    (v0 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL));
L_10f1aa4:;
    { if (xo) { xo[0] = v1; if (a3xw1) xo[1] = a0; if (a3xw2) xo[2] = a1; } io_h2 = h2; return v0; }
    __builtin_unreachable();
}

// GetDamageClass_u11 (0x1166c10): typed by tools/typed_lift2.py (round A6; frame calls, extra outputs xo[0]=a0 xo[1]=a1 xo[2]=a2; VU0 in h5, out h2, frame 16 bytes, 0 slot accesses)
inline uint64_t GetDamageClass_u11(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint64_t a0, float f12, float f13, float f14, float f15, float f16, float f17, float f18, float f19, uint64_t &io_h2, uint64_t io_h5, uint64_t *xo = nullptr)
{
    uint64_t v0 = 0;
    uint32_t v1 = 0;
    uint64_t a1 = 0;
    uint64_t a2 = 0;
    uint64_t h2 = io_h2;
    uint64_t h5 = io_h5;
    bool a3xw0 = false;
    (a2 = (uint64_t)((uint64_t)a0 + (uint64_t)0ULL));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 11))));
    (a1 = (uint64_t)(uint32_t)((uint8_t)A1_R8B((uint32_t)a2, 8)));
    { const bool c = (int64_t)a1 == (int64_t)v0; (v0 = (uint64_t)(((int64_t)(int64_t)RC_U64X(a1) < (int64_t)(int32_t)12) ? 1 : 0)); if (c) goto L_1166c70; }
    { const bool c = (int64_t)v0 == 0; (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 13)))); if (c) goto L_1166c48; }
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 9))));
    { const bool c = (int64_t)a1 == (int64_t)v0; (v0 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL)); if (c) goto L_1166c60; }
    goto L_1166c90;
L_1166c48:;
    { const bool c = (int64_t)a1 == (int64_t)v0; v1 = 0x16u; if (c) goto L_1166c80; }
    { const bool c = (int64_t)a1 == (int64_t)(int32_t)v1; (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)a2, 656)))); if (c) goto L_1166c8c; }
    (v0 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL));
    goto L_1166c8c;
L_1166c60:;
    { A3_CHARGE(); uint64_t a7c[2] = {0u, 0u}; v0 = GetCharacterClass_x11(ram, rt, a0, h2, h5, a7c); a0 = a7c[0]; a1 = a7c[1]; a3xw0 = true; }
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v0, 512))));
    goto L_1166c8c;
L_1166c70:;
    { A3_CHARGE(); uint64_t a7c[3] = {0u, a0, 0u}; v0 = GetScriptLayoutClass_x11(ram, rt, a0, h2, a7c); a0 = a7c[1]; a1 = a7c[2]; a3xw0 = true; }
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v0, 560))));
    goto L_1166c8c;
L_1166c80:;
    { A3_CHARGE(); uint64_t a7c[2] = {0u, 0u}; v0 = GetScriptAnimationClass_x11(ram, rt, a0, h2, a7c); a0 = a7c[1]; a3xw0 = true; }
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v0, 560))));
L_1166c8c:;
L_1166c90:;
    { if (xo) { if (a3xw0) xo[0] = a0; xo[1] = a1; xo[2] = a2; } io_h2 = h2; return v0; }
    __builtin_unreachable();
}

// GAMEUTIL_get_parent_object_u11 (0x107f168): typed by tools/typed_lift2.py (round A6; frame calls, extra outputs xo[0]=a1; VU0 in -, out h2, frame 16 bytes, 0 slot accesses)
inline uint64_t GAMEUTIL_get_parent_object_u11(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint64_t v0, uint64_t a0, uint64_t a1, float f0, float f1, float f2, float f3, float f4, float f5, float f6, float f7, float f8, float f9, float f10, float f11, float f12, float f13, float f14, float f15, float f16, float f17, float f18, float f19, float f20, float f21, float f22, float f23, float f24, float f25, float f26, float f27, float f28, float f29, float f30, float f31, uint64_t &io_h2, uint64_t *xo = nullptr)
{
    uint64_t v1 = 0;
    uint64_t h2 = io_h2;
    { A3_CHARGE(); uint64_t a7c[3] = {0u, 0u, a1}; v0 = GetLayoutClass_u11(ram, rt, a0, f0, f1, f2, f3, f4, f5, f6, f7, f8, f9, f10, f11, f12, f13, f14, f15, f16, f17, f18, f19, f20, f21, f22, f23, f24, f25, f26, f27, f28, f29, f30, f31, h2, a7c); a1 = a7c[2]; }
    (v1 = (uint64_t)((uint64_t)v0 + (uint64_t)0ULL));
    { const bool c = (int64_t)v1 == 0; (a0 = (uint64_t)((uint64_t)v1 + (uint64_t)0ULL)); if (c) goto L_107f190; }
    { if (xo) { xo[0] = a1; } const uint32_t a4_t = GetLayoutParentObj(ram, rt, (uint32_t)a0, f12, f13, f14, f15, f16, f17, f18, f19); io_h2 = h2; return a1_sx64(a4_t); }
L_107f190:;
    { if (xo) { xo[0] = a1; } io_h2 = h2; return v0; }
    __builtin_unreachable();
}

// CharaCtrlSetScriptCtrlStatus_u11 (0x10cc178): typed by tools/typed_lift2.py (round A6; guest frame calls, extra outputs xo[0]=a0 xo[1]=a1; VU0 in h5, out h2)
inline uint64_t CharaCtrlSetScriptCtrlStatus_u11(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint64_t v0, uint64_t a0, uint64_t a1, uint64_t sp, float f0, float f1, float f2, float f3, float f4, float f5, float f6, float f7, float f8, float f9, float f10, float f11, float f12, float f13, float f14, float f15, float f16, float f17, float f18, float f19, float f20, float f21, float f22, float f23, float f24, float f25, float f26, float f27, float f28, float f29, float f30, float f31, uint64_t &io_h2, uint64_t io_h5, uint64_t *xo = nullptr)
{
    uint64_t v1 = 0;
    uint64_t s0 = 0;
    uint64_t s1 = 0;
    uint64_t h2 = io_h2;
    uint64_t h5 = io_h5;
    (sp = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)sp, 4294967264))));
    (s0 = (uint64_t)((uint64_t)a0 + (uint64_t)0ULL));
    { (s1 = (uint64_t)((uint64_t)a1 + (uint64_t)0ULL)); A3_CHARGE(); uint64_t a7c[2] = {0u, 0u}; v0 = GetCharacterClass_x11(ram, rt, a0, h2, h5, a7c); a1 = a7c[1]; }
    { const bool c = (int64_t)v0 != 0; (a0 = (uint64_t)((uint64_t)s0 + (uint64_t)0ULL)); if (c) goto L_10cc1e0; }
    { (s0 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL)); A3_CHARGE(); uint64_t a7c[3] = {0u, 0u, a1}; v0 = GetLayoutClass_u11(ram, rt, a0, f0, f1, f2, f3, f4, f5, f6, f7, f8, f9, f10, f11, f12, f13, f14, f15, f16, f17, f18, f19, f20, f21, f22, f23, f24, f25, f26, f27, f28, f29, f30, f31, h2, a7c); a1 = a7c[2]; }
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)v0, 292))));
    { const bool c = (int64_t)v1 == 0; (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v0, 192)))); if (c) goto L_10cc1d4; }
    { A3_CHARGE(); v0 = a1_sx64(GetGeometryPointerObject(ram, rt, (uint32_t)a0)); }
    (a1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 22))));
    (v1 = (uint64_t)(uint32_t)((uint8_t)A1_R8B((uint32_t)v0, 8)));
    { const bool c = (int64_t)v1 != (int64_t)a1; (a0 = (uint64_t)((uint64_t)v0 + (uint64_t)0ULL)); if (c) goto L_10cc1d4; }
    { A3_CHARGE(); uint64_t a7c[2] = {0u, 0u}; v0 = GetCharacterClass_x11(ram, rt, a0, h2, h5, a7c); a0 = a7c[0]; a1 = a7c[1]; }
    (s0 = (uint64_t)((uint64_t)v0 + (uint64_t)0ULL));
L_10cc1d4:;
    (v0 = (uint64_t)((uint64_t)s0 + (uint64_t)0ULL));
    if ((int64_t)v0 == 0) goto L_10cc1fc;
L_10cc1e0:;
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v0, 2336))));
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 1))));
    (v0 = (uint64_t)(A1_R64B((uint32_t)a0, 0)));
    (v1 = (uint64_t)(RC_U64X(v1) << ((uint32_t)s1 & 0x3F)));
    (v0 = (uint64_t)(v0 | v1));
    A1_W64B((uint32_t)a0, 0, v0);
L_10cc1fc:;
    { (sp = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)sp, 32)))); if (xo) { xo[0] = a0; xo[1] = a1; } io_h2 = h2; return v0; }
    __builtin_unreachable();
}

// GetCollisionDBPlane_u11 (0x11257a0): typed by tools/typed_lift2.py (round A6; guest frame calls, extra outputs xo[0]=f0 xo[1]=f20 xo[2]=a0 xo[3]=a1 xo[4]=a2 xo[5]=a3 xo[6]=t0 xo[7]=t1 xo[8]=t2 xo[9]=t3; VU0 in vf0 vf6 vf7 vf10 vf18 vf29 vf31 vacc vstat h9 h11 h29, out vf4 vf5 vf6 vf7 vf8 vf10 vf14 vf15 vf16 vf17 vf18 vf29 vf30 vf31 vacc vq vstat h2 h5 h9 h11)
inline uint64_t GetCollisionDBPlane_u11(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint64_t a0, uint64_t t0, uint64_t sp, float f0, float f1, float f2, float f3, float f4, float f5, float f6, float f7, float f8, float f9, float f10, float f11, float f12, float f13, float f14, float f15, float f16, float f17, float f18, float f19, float f20, float f21, float f22, float f23, float f24, float f25, float f26, float f27, float f28, float f29, float f30, float f31, const a4::vec4 &io_vf0, a4::vec4 &io_vf4, a4::vec4 &io_vf5, a4::vec4 &io_vf6, a4::vec4 &io_vf7, a4::vec4 &io_vf8, a4::vec4 &io_vf10, a4::vec4 &io_vf14, a4::vec4 &io_vf15, a4::vec4 &io_vf16, a4::vec4 &io_vf17, a4::vec4 &io_vf18, a4::vec4 &io_vf29, a4::vec4 &io_vf30, a4::vec4 &io_vf31, a4::vec4 &io_vacc, float &io_vq, uint16_t &io_vstat, uint64_t &io_h2, uint64_t &io_h5, uint64_t &io_h9, uint64_t &io_h11, uint64_t io_h29, uint64_t *xo = nullptr)
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
    { (s1 = (uint64_t)(a1_sx64(v1) & (uint64_t)(uint16_t)127)); A3_CHARGE(); v0 = GetLayoutClass_u11(ram, rt, a0, f0, f1, f2, f3, f4, f5, f6, f7, f8, f9, f10, f11, f12, f13, f14, f15, f16, f17, f18, f19, f20, f21, f22, f23, f24, f25, f26, f27, f28, f29, f30, f31, h2); }
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
    { A3_CHARGE(); uint64_t a7c[6] = {0u, 0u, 0u, 0u, 0u, 0u}; v0 = EntryModifiedManager_x11(ram, rt, a0, (uint32_t)a1, a7c); a0 = a7c[0]; a2 = a7c[1]; a3 = a7c[2]; t0 = a7c[3]; t1 = a7c[4]; t2 = a7c[5]; a3xw4 = true; a3xw6 = true; a3xw7 = true; a3xw8 = true; }
L_11259e8:;
    (v0 = (uint64_t)((uint64_t)s0 + (uint64_t)0ULL));
    { (sp = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)sp, 128)))); if (xo) { if (a3xw0) xo[0] = a1_bits(f0); if (a3xw1) xo[1] = a1_bits(f20); xo[2] = a0; xo[3] = a1; if (a3xw4) xo[4] = a2; xo[5] = a3; if (a3xw6) xo[6] = t0; if (a3xw7) xo[7] = t1; if (a3xw8) xo[8] = t2; if (a3xw9) xo[9] = t3; } io_vf4 = vf4; io_vf5 = vf5; io_vf6 = vf6; io_vf7 = vf7; io_vf8 = vf8; io_vf10 = vf10; io_vf14 = vf14; io_vf15 = vf15; io_vf16 = vf16; io_vf17 = vf17; io_vf18 = vf18; io_vf29 = vf29; io_vf30 = vf30; io_vf31 = vf31; io_vacc = vacc; io_vq = vq; io_vstat = vstat; io_h2 = h2; io_h5 = h5; io_h9 = h9; io_h11 = h11; return v0; }
    __builtin_unreachable();
}

// GetCollisionDBAttribute_x11 (0x1125a08): typed by tools/typed_lift2.py (round A6; frame calls, extra outputs xo[0]=a0 xo[1]=a1 xo[2]=a2 xo[3]=a3; VU0 in -, out h2 h3, frame 48 bytes, 0 slot accesses)
inline uint64_t GetCollisionDBAttribute_x11(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint64_t v0, uint64_t a0, uint64_t a1, float f0, float f1, float f2, float f3, float f4, float f5, float f6, float f7, float f8, float f9, float f10, float f11, float f12, float f13, float f14, float f15, float f16, float f17, float f18, float f19, float f20, float f21, float f22, float f23, float f24, float f25, float f26, float f27, float f28, float f29, float f30, float f31, uint64_t &io_h2, uint64_t &io_h3, uint64_t *xo = nullptr)
{
    uint64_t v1 = 0;
    uint32_t a2 = 0;
    uint32_t a3 = 0;
    uint32_t s0 = 0;
    uint64_t s1 = 0;
    uint64_t s2 = 0;
    uint32_t s3 = 0;
    uint64_t s4 = 0;
    uint64_t h2 = io_h2;
    uint64_t h3 = io_h3;
    (s1 = (uint64_t)((uint64_t)a1 + (uint64_t)0ULL));
    (s4 = (uint64_t)((uint64_t)a0 + (uint64_t)0ULL));
    { (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)s1, 0)))); A3_CHARGE(); v0 = GetLayoutClass_u11(ram, rt, a0, f0, f1, f2, f3, f4, f5, f6, f7, f8, f9, f10, f11, f12, f13, f14, f15, f16, f17, f18, f19, f20, f21, f22, f23, f24, f25, f26, f27, f28, f29, f30, f31, h2); }
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)s1, 4))));
    (a1 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)v0, 388))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)255 << 16))));
    (s0 = (uint32_t)((int32_t)((int32_t)SRL32((uint32_t)v1, 24))));
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)s1, 0))));
    (v0 = (uint64_t)(RC_U64X(v0) | (uint64_t)(uint16_t)65535));
    s0 = s0 & 0x7Fu;
    (s2 = (uint64_t)(RC_U64X(v1) & RC_U64X(v0)));
    { (s3 = (uint32_t)((int32_t)((int32_t)A1_R32B((uint32_t)a1, 32)))); A3_CHARGE(); v0 = GetLayoutClass_u11(ram, rt, a0, f0, f1, f2, f3, f4, f5, f6, f7, f8, f9, f10, f11, f12, f13, f14, f15, f16, f17, f18, f19, f20, f21, f22, f23, f24, f25, f26, f27, f28, f29, f30, f31, h2); }
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)SLL32(s0, 3))));
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v1, s0))));
    (a3 = (uint32_t)((int32_t)((int32_t)SLL32((uint32_t)s2, 2))));
    (a2 = (uint32_t)((int32_t)((int32_t)SLL32((uint32_t)v1, 4))));
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)v0, 388))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)s1, 4))));
    (a0 = (uint64_t)((uint64_t)a1_sx64(a2) + (uint64_t)0ULL));
    { const bool c = (int32_t)v0 >= 0; (a1 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)v1, 32)))); if (c) goto L_1125ad8; }
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)a1, 8))));
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v0, (uint32_t)a0))));
    (v1 = (uint64_t)(uint32_t)((uint8_t)A1_R8B((uint32_t)a0, 8)));
    { const bool c = (int64_t)v1 == 0; (v0 = (uint64_t)((uint64_t)a1_sx64(a3) + (uint64_t)0ULL)); if (c) goto L_1125ab0; }
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)a0, 20))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v0, (uint32_t)s2))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)SLL32((uint32_t)v0, 2))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v0, (uint32_t)v1))));
    (v1 = (uint64_t)(uint32_t)((uint16_t)A1_R16B((uint32_t)v0, 0)));
    goto L_1125afc;
L_1125ab0:;
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)SLL32((uint32_t)s2, 1))));
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)a0, 20))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v0, (uint32_t)s2))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)SLL32((uint32_t)v0, 2))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v0, (uint32_t)s2))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)SLL32((uint32_t)v0, 2))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v0, (uint32_t)v1))));
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)v0, 48))));
    goto L_1125afc;
L_1125ad8:;
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)a1, 8))));
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)a1, 24))));
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(a2, (uint32_t)v1))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)v1, 84))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(a3, (uint32_t)v0))));
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)v0, 0))));
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)SLL32((uint32_t)v1, 6))));
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v1, (uint32_t)a0))));
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)v1, 60))));
L_1125afc:;
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B(s3, 52))));
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)SLL32((uint32_t)v1, 4))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v0, (uint32_t)a0))));
    do { const a4::q128 _v = (A4_R128(ADD32((uint32_t)v0, 0))); h3 = a4::hi64(_v); std::memcpy(&v1, &_v, 8); } while (0);
    A4_W128(ADD32((uint32_t)s4, 0), a4::mk128(h3, v1));
    { if (xo) { xo[0] = a0; xo[1] = a1; xo[2] = a1_sx64(a2); xo[3] = a1_sx64(a3); } io_h2 = h2; io_h3 = h3; return v0; }
    __builtin_unreachable();
}

// is_have_region_camera_region_u11 (0x11638b8): typed by tools/typed_lift2.py (round A6; guest frame calls, extra outputs xo[0]=f0 xo[1]=f1 xo[2]=f12 xo[3]=a1; VU0 in -, out h2 h3)
inline uint64_t is_have_region_camera_region_u11(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint64_t a0, uint64_t a1, uint64_t sp, float f0, float f1, float f2, float f3, float f4, float f5, float f6, float f7, float f8, float f9, float f10, float f11, float f12, float f13, float f14, float f15, float f16, float f17, float f18, float f19, float f20, float f21, float f22, float f23, float f24, float f25, float f26, float f27, float f28, float f29, float f30, float f31, uint64_t &io_h2, uint64_t &io_h3, uint64_t *xo = nullptr)
{
    uint32_t at = 0;
    uint64_t v0 = 0;
    uint64_t v1 = 0;
    uint32_t fcr31 = 0;
    uint64_t h2 = io_h2;
    uint64_t h3 = io_h3;
    bool a3xw0 = false;
    bool a3xw1 = false;
    bool a3xw2 = false;
    bool a3xw3 = false;
    (sp = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)sp, 4294967264))));
    if ((int64_t)a0 == 0) goto L_11638d0;
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)a0, 20))));
    { const bool c = (int64_t)v0 != 0; (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 1)))); if (c) goto L_11638d4; }
L_11638d0:;
    (v1 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL));
L_11638d4:;
    { const bool c = (int64_t)v1 == 0; (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 4294967295)))); if (c) goto L_11638e0; }
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)a0, 0))));
L_11638e0:;
    if ((int64_t)v0 != 0) { (v0 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL)); goto L_116394c; }
    { (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)a0, 16)))); A3_CHARGE(); uint64_t a7c[3] = {0u, 0u, a1}; v0 = GetLayoutClass_u11(ram, rt, a0, f0, f1, f2, f3, f4, f5, f6, f7, f8, f9, f10, f11, f12, f13, f14, f15, f16, f17, f18, f19, f20, f21, f22, f23, f24, f25, f26, f27, f28, f29, f30, f31, h2, a7c); a1 = a7c[2]; a3xw3 = true; }
    { uint32_t bits = 0u; std::memcpy(&f1, &bits, sizeof(bits)); } a3xw1 = true;
    do { const a4::q128 _v = (A4_R128(ADD32((uint32_t)v0, 144))); h3 = a4::hi64(_v); std::memcpy(&v1, &_v, 8); } while (0);
    A4_W128(ADD32((uint32_t)sp, 0), a4::mk128(h3, v1));
    { uint32_t bits = A1_R32B((uint32_t)sp, 0); float f; std::memcpy(&f, &bits, sizeof(f)); f0 = f; } a3xw0 = true;
    fcr31 = (a1f::lt(f0, f1)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
    if ((fcr31 & 0x800000u) != 0u) { { float f = f1; uint32_t bits; std::memcpy(&bits, &f, sizeof(bits)); A1_W32B((uint32_t)sp, 0, bits); } goto L_116390c; }
L_116390c:;
    { uint32_t bits = A1_R32B((uint32_t)sp, 4); float f; std::memcpy(&f, &bits, sizeof(f)); f0 = f; } a3xw0 = true;
    fcr31 = (a1f::lt(f0, f1)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
    if ((fcr31 & 0x800000u) != 0u) { { float f = f1; uint32_t bits; std::memcpy(&bits, &f, sizeof(bits)); A1_W32B((uint32_t)sp, 4, bits); } goto L_116391c; }
L_116391c:;
    { uint32_t bits = A1_R32B((uint32_t)sp, 8); float f; std::memcpy(&f, &bits, sizeof(f)); f0 = f; } a3xw0 = true;
    fcr31 = (a1f::lt(f0, f1)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
    if ((fcr31 & 0x800000u) != 0u) { { float f = f1; uint32_t bits; std::memcpy(&bits, &f, sizeof(bits)); A1_W32B((uint32_t)sp, 8, bits); } goto L_116392c; }
L_116392c:;
    at = 0x3A830000u;
    (at = (uint32_t)(a1_sx64(at) | (uint64_t)(uint16_t)4718));
    { uint32_t bits = a7_opq(at); std::memcpy(&f12, &bits, sizeof(bits)); } a3xw2 = true;
    { (a0 = (uint64_t)((uint64_t)sp + (uint64_t)0ULL)); A3_CHARGE(); uint64_t a7c[1] = {0u}; { const a1::RetVF a2_r = GAMEUTIL_is_nearly_equal_zero3D_x11(ram, rt, (uint32_t)a0, f12, a7c); v0 = a2_r.v0; f0 = a2_r.f0; } f1 = a1_flt((uint32_t)a7c[0]); a3xw0 = true; a3xw1 = true; }
    if ((int64_t)v0 != 0) { (v0 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL)); goto L_116394c; }
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 1))));
L_116394c:;
    { (sp = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)sp, 32)))); if (xo) { if (a3xw0) xo[0] = a1_bits(f0); if (a3xw1) xo[1] = a1_bits(f1); if (a3xw2) xo[2] = a1_bits(f12); if (a3xw3) xo[3] = a1; } io_h2 = h2; io_h3 = h3; return v0; }
    __builtin_unreachable();
}

// GetGeometryPointerSurfacePlane_u11 (0x1132850): typed by tools/typed_lift2.py (round A6; frame calls, extra outputs xo[0]=f0 xo[1]=f20 xo[2]=a0 xo[3]=a1 xo[4]=a2 xo[5]=a3 xo[6]=t0 xo[7]=t1 xo[8]=t2 xo[9]=t3; VU0 in vf0 vf6 vf7 vf10 vf18 vf29 vf31 vacc vstat h9 h11 h29, out vf4 vf5 vf6 vf7 vf8 vf10 vf14 vf15 vf16 vf17 vf18 vf29 vf30 vf31 vacc vq vstat h2 h5 h9 h11, jump tables 1, guest sp, frame 16 bytes, 0 slot accesses)
inline uint64_t GetGeometryPointerSurfacePlane_u11(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint64_t a0, uint64_t a2, uint64_t t0, uint64_t t1, uint64_t t2, uint64_t t3, uint32_t sp, float f0, float f1, float f2, float f3, float f4, float f5, float f6, float f7, float f8, float f9, float f10, float f11, float f12, float f13, float f14, float f15, float f16, float f17, float f18, float f19, float f20, float f21, float f22, float f23, float f24, float f25, float f26, float f27, float f28, float f29, float f30, float f31, const a4::vec4 &io_vf0, a4::vec4 &io_vf4, a4::vec4 &io_vf5, a4::vec4 &io_vf6, a4::vec4 &io_vf7, a4::vec4 &io_vf8, a4::vec4 &io_vf10, a4::vec4 &io_vf14, a4::vec4 &io_vf15, a4::vec4 &io_vf16, a4::vec4 &io_vf17, a4::vec4 &io_vf18, a4::vec4 &io_vf29, a4::vec4 &io_vf30, a4::vec4 &io_vf31, a4::vec4 &io_vacc, float &io_vq, uint16_t &io_vstat, uint64_t &io_h2, uint64_t &io_h5, uint64_t &io_h9, uint64_t &io_h11, uint64_t io_h29, uint64_t *xo = nullptr)
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
    { A3_CHARGE(); uint64_t a7c[10] = {(uint64_t)a1_bits(f0), (uint64_t)a1_bits(f20), 0u, 0u, a2, 0u, t0, t1, t2, t3}; v0 = GetCollisionDBPlane_u11(ram, rt, a0, t0, a1_sx64((uint32_t)(sp + 0xFFFFFFF0u)), f0, f1, f2, f3, f4, f5, f6, f7, f8, f9, f10, f11, f12, f13, f14, f15, f16, f17, f18, f19, f20, f21, f22, f23, f24, f25, f26, f27, f28, f29, f30, f31, vf0, vf4, vf5, vf6, vf7, vf8, vf10, vf14, vf15, vf16, vf17, vf18, vf29, vf30, vf31, vacc, vq, vstat, h2, h5, h9, h11, h29, a7c); f0 = a1_flt((uint32_t)a7c[0]); f20 = a1_flt((uint32_t)a7c[1]); a0 = a7c[2]; a1 = a7c[3]; a2 = a7c[4]; a3 = a7c[5]; t0 = a7c[6]; t1 = a7c[7]; t2 = a7c[8]; t3 = a7c[9]; a3xw0 = true; a3xw1 = true; a3xw2 = true; a3xw3 = true; a3xw4 = true; a3xw5 = true; a3xw6 = true; a3xw7 = true; a3xw8 = true; a3xw9 = true; }
L_1132898:;
    { if (xo) { if (a3xw0) xo[0] = a1_bits(f0); if (a3xw1) xo[1] = a1_bits(f20); if (a3xw2) xo[2] = a0; if (a3xw3) xo[3] = a1; if (a3xw4) xo[4] = a2; if (a3xw5) xo[5] = a3; if (a3xw6) xo[6] = t0; if (a3xw7) xo[7] = t1; if (a3xw8) xo[8] = t2; if (a3xw9) xo[9] = t3; } io_vf4 = vf4; io_vf5 = vf5; io_vf6 = vf6; io_vf7 = vf7; io_vf8 = vf8; io_vf10 = vf10; io_vf14 = vf14; io_vf15 = vf15; io_vf16 = vf16; io_vf17 = vf17; io_vf18 = vf18; io_vf29 = vf29; io_vf30 = vf30; io_vf31 = vf31; io_vacc = vacc; io_vq = vq; io_vstat = vstat; io_h2 = h2; io_h5 = h5; io_h9 = h9; io_h11 = h11; return v0; }
    __builtin_unreachable();
}

// CompareString16 (0x1b85d0): typed by tools/typed_lift2.py (round A6; guest frame leaf; VU0 in -, out h8 h9 h10)
inline uint64_t CompareString16(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint64_t a0, uint64_t a1, uint64_t sp, uint64_t &io_h8, uint64_t &io_h9, uint64_t &io_h10)
{
    uint64_t v0 = 0;
    uint64_t v1 = 0;
    uint64_t t0 = 0;
    uint64_t t1 = 0;
    uint64_t t2 = 0;
    uint64_t h8 = io_h8;
    uint64_t h9 = io_h9;
    uint64_t h10 = io_h10;
    (sp = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)sp, 4294967280))));
    do { const a4::q128 _v = (A4_R128(ADD32((uint32_t)a0, 0))); h8 = a4::hi64(_v); std::memcpy(&t0, &_v, 8); } while (0);
    do { const a4::q128 _v = (A4_R128(ADD32((uint32_t)a1, 0))); h9 = a4::hi64(_v); std::memcpy(&t1, &_v, 8); } while (0);
    do { const a4::q128 _v = (a4m::pxor(a4::mk128(h8, t0), a4::mk128(h9, t1))); h10 = a4::hi64(_v); std::memcpy(&t2, &_v, 8); } while (0);
    A4_W128(ADD32((uint32_t)sp, 0), a4::mk128(h10, t2));
    (v0 = (uint64_t)(A1_R64B((uint32_t)sp, 0)));
    (v1 = (uint64_t)(A1_R64B((uint32_t)sp, 8)));
    (v0 = (uint64_t)(v0 | v1));
    (v0 = (uint64_t)(v0 << (32 + 0)));
    (v0 = (uint64_t)((int64_t)v0 >> (32 + 0)));
    { (sp = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)sp, 16)))); io_h8 = h8; io_h9 = h9; io_h10 = h10; return v0; }
    __builtin_unreachable();
}

// iosTransposeMatrix (0x1b91f0): typed by tools/typed_lift2.py (round A6; leaf, extra outputs xo[0]=t0 xo[1]=t1 xo[2]=t2 xo[3]=t3; VU0 in -, out h8 h9 h10 h11 h12 h13 h14 h15)
inline void iosTransposeMatrix(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint64_t a0, uint64_t a1, uint64_t &io_h8, uint64_t &io_h9, uint64_t &io_h10, uint64_t &io_h11, uint64_t &io_h12, uint64_t &io_h13, uint64_t &io_h14, uint64_t &io_h15, uint64_t *xo = nullptr)
{
    uint64_t t0 = 0;
    uint64_t t1 = 0;
    uint64_t t2 = 0;
    uint64_t t3 = 0;
    uint64_t t4 = 0;
    uint64_t t5 = 0;
    uint64_t t6 = 0;
    uint64_t t7 = 0;
    uint64_t h8 = io_h8;
    uint64_t h9 = io_h9;
    uint64_t h10 = io_h10;
    uint64_t h11 = io_h11;
    uint64_t h12 = io_h12;
    uint64_t h13 = io_h13;
    uint64_t h14 = io_h14;
    uint64_t h15 = io_h15;
    do { const a4::q128 _v = (A4_R128(ADD32((uint32_t)a1, 0))); h8 = a4::hi64(_v); std::memcpy(&t0, &_v, 8); } while (0);
    do { const a4::q128 _v = (A4_R128(ADD32((uint32_t)a1, 16))); h9 = a4::hi64(_v); std::memcpy(&t1, &_v, 8); } while (0);
    do { const a4::q128 _v = (A4_R128(ADD32((uint32_t)a1, 32))); h10 = a4::hi64(_v); std::memcpy(&t2, &_v, 8); } while (0);
    do { const a4::q128 _v = (A4_R128(ADD32((uint32_t)a1, 48))); h11 = a4::hi64(_v); std::memcpy(&t3, &_v, 8); } while (0);
    do { const a4::q128 _v = (a4m::pextlw(a4::mk128(h9, t1), a4::mk128(h8, t0))); h12 = a4::hi64(_v); std::memcpy(&t4, &_v, 8); } while (0);
    do { const a4::q128 _v = (a4m::pextuw(a4::mk128(h9, t1), a4::mk128(h8, t0))); h13 = a4::hi64(_v); std::memcpy(&t5, &_v, 8); } while (0);
    do { const a4::q128 _v = (a4m::pextlw(a4::mk128(h11, t3), a4::mk128(h10, t2))); h14 = a4::hi64(_v); std::memcpy(&t6, &_v, 8); } while (0);
    do { const a4::q128 _v = (a4m::pextuw(a4::mk128(h11, t3), a4::mk128(h10, t2))); h15 = a4::hi64(_v); std::memcpy(&t7, &_v, 8); } while (0);
    do { const a4::q128 _v = (a4m::pcpyld(a4::mk128(h14, t6), a4::mk128(h12, t4))); h8 = a4::hi64(_v); std::memcpy(&t0, &_v, 8); } while (0);
    do { const a4::q128 _v = (a4v::unpackhi_epi64(a4::mk128(h12, t4), a4::mk128(h14, t6))); h9 = a4::hi64(_v); std::memcpy(&t1, &_v, 8); } while (0);
    do { const a4::q128 _v = (a4m::pcpyld(a4::mk128(h15, t7), a4::mk128(h13, t5))); h10 = a4::hi64(_v); std::memcpy(&t2, &_v, 8); } while (0);
    do { const a4::q128 _v = (a4v::unpackhi_epi64(a4::mk128(h13, t5), a4::mk128(h15, t7))); h11 = a4::hi64(_v); std::memcpy(&t3, &_v, 8); } while (0);
    A4_W128(ADD32((uint32_t)a0, 0), a4::mk128(h8, t0));
    A4_W128(ADD32((uint32_t)a0, 16), a4::mk128(h9, t1));
    A4_W128(ADD32((uint32_t)a0, 32), a4::mk128(h10, t2));
    A4_W128(ADD32((uint32_t)a0, 48), a4::mk128(h11, t3));
    { if (xo) { xo[0] = t0; xo[1] = t1; xo[2] = t2; xo[3] = t3; } io_h8 = h8; io_h9 = h9; io_h10 = h10; io_h11 = h11; io_h12 = h12; io_h13 = h13; io_h14 = h14; io_h15 = h15; return; }
    __builtin_unreachable();
}

// iosInverseMatrix (0x1b9238): typed by tools/typed_lift2.py (round A6; leaf, extra outputs xo[0]=t0 xo[1]=t1 xo[2]=t2 xo[3]=t3; VU0 in vf2 vf9 vacc, out vf1 vf2 vf3 vf4 vf8 vf9 vacc h8 h9 h10 h11 h12 h13 h14 h15)
inline void iosInverseMatrix(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint64_t a0, uint64_t a1, a4::vec4 &io_vf1, a4::vec4 &io_vf2, a4::vec4 &io_vf3, a4::vec4 &io_vf4, a4::vec4 &io_vf8, a4::vec4 &io_vf9, a4::vec4 &io_vacc, uint64_t &io_h8, uint64_t &io_h9, uint64_t &io_h10, uint64_t &io_h11, uint64_t &io_h12, uint64_t &io_h13, uint64_t &io_h14, uint64_t &io_h15, uint64_t *xo = nullptr)
{
    uint64_t t0 = 0;
    uint64_t t1 = 0;
    uint64_t t2 = 0;
    uint64_t t3 = 0;
    uint64_t t4 = 0;
    uint64_t t5 = 0;
    uint64_t t6 = 0;
    uint64_t t7 = 0;
    a4::vec4 vf1 = io_vf1;
    a4::vec4 vf2 = io_vf2;
    a4::vec4 vf3 = io_vf3;
    a4::vec4 vf4 = io_vf4;
    a4::vec4 vf8 = io_vf8;
    a4::vec4 vf9 = io_vf9;
    a4::vec4 vacc = io_vacc;
    uint64_t h8 = io_h8;
    uint64_t h9 = io_h9;
    uint64_t h10 = io_h10;
    uint64_t h11 = io_h11;
    uint64_t h12 = io_h12;
    uint64_t h13 = io_h13;
    uint64_t h14 = io_h14;
    uint64_t h15 = io_h15;
    do { const a4::q128 _v = (A4_R128(ADD32((uint32_t)a1, 0))); h8 = a4::hi64(_v); std::memcpy(&t0, &_v, 8); } while (0);
    do { const a4::q128 _v = (A4_R128(ADD32((uint32_t)a1, 16))); h9 = a4::hi64(_v); std::memcpy(&t1, &_v, 8); } while (0);
    do { const a4::q128 _v = (A4_R128(ADD32((uint32_t)a1, 32))); h10 = a4::hi64(_v); std::memcpy(&t2, &_v, 8); } while (0);
    vf1 = a4v::castsi128_ps(A4_R128(ADD32((uint32_t)a1, 48)));
    { a4::q128 mask = a4v::set_epi32(-1, -1, -1, -1); vf2 = a4v::blendv_ps(vf2, vf1, a4v::castsi128_ps(mask)); }
    { a4::vec4 res = a4v::sub(vf1, vf1); a4::q128 mask = a4v::set_epi32(0, -1, -1, -1); vf1 = a4v::blend(vf1, res, a4v::castsi128_ps(mask)); }
    { a4::q128 mask = a4v::set_epi32(-1, -1, -1, -1); vf9 = a4v::blendv_ps(vf9, vf1, a4v::castsi128_ps(mask)); }
    do { const a4::q128 _v = (a4v::castps_si128(vf1)); h11 = a4::hi64(_v); std::memcpy(&t3, &_v, 8); } while (0);
    do { const a4::q128 _v = (a4m::pextlw(a4::mk128(h9, t1), a4::mk128(h8, t0))); h12 = a4::hi64(_v); std::memcpy(&t4, &_v, 8); } while (0);
    do { const a4::q128 _v = (a4m::pextuw(a4::mk128(h9, t1), a4::mk128(h8, t0))); h13 = a4::hi64(_v); std::memcpy(&t5, &_v, 8); } while (0);
    do { const a4::q128 _v = (a4m::pextlw(a4::mk128(h11, t3), a4::mk128(h10, t2))); h14 = a4::hi64(_v); std::memcpy(&t6, &_v, 8); } while (0);
    do { const a4::q128 _v = (a4m::pextuw(a4::mk128(h11, t3), a4::mk128(h10, t2))); h15 = a4::hi64(_v); std::memcpy(&t7, &_v, 8); } while (0);
    do { const a4::q128 _v = (a4m::pcpyld(a4::mk128(h14, t6), a4::mk128(h12, t4))); h8 = a4::hi64(_v); std::memcpy(&t0, &_v, 8); } while (0);
    do { const a4::q128 _v = (a4v::unpackhi_epi64(a4::mk128(h12, t4), a4::mk128(h14, t6))); h9 = a4::hi64(_v); std::memcpy(&t1, &_v, 8); } while (0);
    do { const a4::q128 _v = (a4m::pcpyld(a4::mk128(h15, t7), a4::mk128(h13, t5))); h10 = a4::hi64(_v); std::memcpy(&t2, &_v, 8); } while (0);
    vf3 = a4v::castsi128_ps(a4::mk128(h8, t0));
    vf4 = a4v::castsi128_ps(a4::mk128(h9, t1));
    vf8 = a4v::castsi128_ps(a4::mk128(h10, t2));
    { a4::vec4 res = a4v::mul(vf3, a4v::bcast<0>(vf2)); vacc = a4v::dest<1, 1, 1, 0>(vacc, res); }
    { a4::vec4 mul_res = a4v::mul(vf4, a4v::bcast<1>(vf2)); a4::vec4 res = a4v::addK<1, 1>(vacc, mul_res); vacc = a4v::dest<1, 1, 1, 0>(vacc, res); }
    { a4::vec4 mul_res = a4v::mul(vf8, a4v::bcast<2>(vf2)); a4::vec4 res = a4v::addK<1, 1>(vacc, mul_res); a4::q128 mask = a4v::set_epi32(0, -1, -1, -1); vf1 = a4v::blendv_ps(vf1, res, a4v::castsi128_ps(mask)); }
    { a4::vec4 res = a4v::subK<1, 1>(vf9, vf1); a4::q128 mask = a4v::set_epi32(0, -1, -1, -1); vf1 = a4v::blend(vf1, res, a4v::castsi128_ps(mask)); }
    A4_W128(ADD32((uint32_t)a0, 0), a4::mk128(h8, t0));
    A4_W128(ADD32((uint32_t)a0, 16), a4::mk128(h9, t1));
    A4_W128(ADD32((uint32_t)a0, 32), a4::mk128(h10, t2));
    A4_W128(ADD32((uint32_t)a0, 48), a4v::castps_si128(vf1));
    { if (xo) { xo[0] = t0; xo[1] = t1; xo[2] = t2; xo[3] = t3; } io_vf1 = vf1; io_vf2 = vf2; io_vf3 = vf3; io_vf4 = vf4; io_vf8 = vf8; io_vf9 = vf9; io_vacc = vacc; io_h8 = h8; io_h9 = h9; io_h10 = h10; io_h11 = h11; io_h12 = h12; io_h13 = h13; io_h14 = h14; io_h15 = h15; return; }
    __builtin_unreachable();
}

// iosSetCameraMatrix (0x1b9348): typed by tools/typed_lift2.py (round A6; guest frame leaf, extra outputs xo[0]=a2 xo[1]=t0 xo[2]=t1 xo[3]=t2 xo[4]=t3; VU0 in vf0 vf2 vf7 vf9 vf14 vf15 vf16 vf17 vacc vstat, out vf1 vf2 vf3 vf4 vf5 vf6 vf7 vf8 vf9 vf14 vf15 vf16 vf17 vacc vq vstat h2 h8 h9 h10 h11 h12 h13 h14 h15)
inline uint64_t iosSetCameraMatrix(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint64_t a0, uint64_t a1, uint64_t a2, uint64_t a3, uint64_t sp, const a4::vec4 &io_vf0, a4::vec4 &io_vf1, a4::vec4 &io_vf2, a4::vec4 &io_vf3, a4::vec4 &io_vf4, a4::vec4 &io_vf5, a4::vec4 &io_vf6, a4::vec4 &io_vf7, a4::vec4 &io_vf8, a4::vec4 &io_vf9, a4::vec4 &io_vf14, a4::vec4 &io_vf15, a4::vec4 &io_vf16, a4::vec4 &io_vf17, a4::vec4 &io_vacc, float &io_vq, uint16_t &io_vstat, uint64_t &io_h2, uint64_t &io_h8, uint64_t &io_h9, uint64_t &io_h10, uint64_t &io_h11, uint64_t &io_h12, uint64_t &io_h13, uint64_t &io_h14, uint64_t &io_h15, uint64_t *xo = nullptr)
{
    uint64_t v0 = 0;
    uint64_t v1 = 0;
    uint64_t t0 = 0;
    uint64_t t1 = 0;
    uint64_t t2 = 0;
    uint64_t t3 = 0;
    uint64_t t4 = 0;
    uint64_t t5 = 0;
    uint64_t t6 = 0;
    uint64_t t7 = 0;
    a4::vec4 vf0 = io_vf0;
    a4::vec4 vf1 = io_vf1;
    a4::vec4 vf2 = io_vf2;
    a4::vec4 vf3 = io_vf3;
    a4::vec4 vf4 = io_vf4;
    a4::vec4 vf5 = io_vf5;
    a4::vec4 vf6 = io_vf6;
    a4::vec4 vf7 = io_vf7;
    a4::vec4 vf8 = io_vf8;
    a4::vec4 vf9 = io_vf9;
    a4::vec4 vf14 = io_vf14;
    a4::vec4 vf15 = io_vf15;
    a4::vec4 vf16 = io_vf16;
    a4::vec4 vf17 = io_vf17;
    a4::vec4 vacc = io_vacc;
    float vq = io_vq;
    uint16_t vstat = io_vstat;
    uint64_t h2 = io_h2;
    uint64_t h8 = io_h8;
    uint64_t h9 = io_h9;
    uint64_t h10 = io_h10;
    uint64_t h11 = io_h11;
    uint64_t h12 = io_h12;
    uint64_t h13 = io_h13;
    uint64_t h14 = io_h14;
    uint64_t h15 = io_h15;
    (sp = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)sp, 4294967216))));
    { a4::q128 mask = a4v::set_epi32(-1, -1, -1, -1); vf17 = a4v::blendv_ps(vf17, vf0, a4v::castsi128_ps(mask)); }
    { a4::vec4 res = a4v::shuffle_ps<0x39>(vf17, vf17); a4::q128 mask = a4v::set_epi32(-1, -1, -1, -1); vf16 = a4v::blendv_ps(vf16, res, a4v::castsi128_ps(mask)); }
    { a4::vec4 res = a4v::shuffle_ps<0x39>(vf16, vf16); a4::q128 mask = a4v::set_epi32(-1, -1, -1, -1); vf15 = a4v::blendv_ps(vf15, res, a4v::castsi128_ps(mask)); }
    { a4::vec4 res = a4v::shuffle_ps<0x39>(vf15, vf15); a4::q128 mask = a4v::set_epi32(-1, -1, -1, -1); vf14 = a4v::blendv_ps(vf14, res, a4v::castsi128_ps(mask)); }
    A4_W128(ADD32((uint32_t)sp, 0), a4v::castps_si128(vf14));
    A4_W128(ADD32((uint32_t)sp, 16), a4v::castps_si128(vf15));
    A4_W128(ADD32((uint32_t)sp, 32), a4v::castps_si128(vf16));
    A4_W128(ADD32((uint32_t)sp, 48), a4v::castps_si128(vf17));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)sp, 64))));
    vf5 = a4v::castsi128_ps(A4_R128(ADD32((uint32_t)a3, 0)));
    vf6 = a4v::castsi128_ps(A4_R128(ADD32((uint32_t)a2, 0)));
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
    A4_W128(ADD32((uint32_t)sp, 0), a4v::castps_si128(vf5));
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)sp, 32))));
    vf5 = a4v::castsi128_ps(A4_R128(ADD32((uint32_t)a2, 0)));
    { a4::vec4 res = a4v::mul(vf5, vf5); a4::q128 mask = a4v::set_epi32(0, -1, -1, -1); vf7 = a4v::blend(vf7, res, a4v::castsi128_ps(mask)); }
    { a4::vec4 res = a4v::mulK<1, 1>(vf0, a4v::bcast<0>(vf7)); vacc = a4v::dest<0, 0, 0, 1>(vacc, res); }
    { a4::vec4 mul_res = a4v::mulK<1, 1>(vf0, a4v::bcast<1>(vf7)); a4::vec4 res = a4v::addK<1, 1>(vacc, mul_res); vacc = a4v::dest<0, 0, 0, 1>(vacc, res); }
    { a4::vec4 mul_res = a4v::mulK<1, 1>(vf0, a4v::bcast<2>(vf7)); a4::vec4 res = a4v::addK<1, 1>(vacc, mul_res); a4::q128 mask = a4v::set_epi32(-1, 0, 0, 0); vf7 = a4v::blendv_ps(vf7, res, a4v::castsi128_ps(mask)); }
    { float fs = a4v::cvtss_f32(a4v::shuffle_ps<0x03>(vf0, vf0)); float ft = a4v::cvtss_f32(a4v::shuffle_ps<0x03>(vf7, vf7)); vq = a4v::rsqrt(vstat, fs, ft); }
    { a4::vec4 res = a4v::mul(vf5, a4v::set1_ps(vq)); a4::q128 mask = a4v::set_epi32(0, -1, -1, -1); vf5 = a4v::blendv_ps(vf5, res, a4v::castsi128_ps(mask)); }
    A4_W128(ADD32((uint32_t)v1, 0), a4v::castps_si128(vf5));
    (a2 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)sp, 16))));
    vf5 = a4v::castsi128_ps(A4_R128(ADD32((uint32_t)v1, 0)));
    vf6 = a4v::castsi128_ps(A4_R128(ADD32((uint32_t)sp, 0)));
    { a4::vec4 fs_yzx = a4v::shuffle_ps<0xC9>(vf5, vf5); a4::vec4 ft_zxy = a4v::shuffle_ps<0xD2>(vf6, vf6); a4::vec4 res = a4v::mul(fs_yzx, ft_zxy); vacc = a4v::dest<1, 1, 1, 0>(vacc, res); }
    { a4::vec4 fs_yzx = a4v::shuffle_ps<0xC9>(vf6, vf6); a4::vec4 ft_zxy = a4v::shuffle_ps<0xD2>(vf5, vf5); a4::vec4 mul_res = a4v::mul(fs_yzx, ft_zxy); a4::vec4 res = a4v::subK<1, 1>(vacc, mul_res); a4::q128 mask = a4v::set_epi32(0, -1, -1, -1); vf7 = a4v::blendv_ps(vf7, res, a4v::castsi128_ps(mask)); }
    { a4::vec4 res = a4v::sub(vf7, vf7); a4::q128 mask = a4v::set_epi32(-1, 0, 0, 0); vf7 = a4v::blend(vf7, res, a4v::castsi128_ps(mask)); }
    A4_W128(ADD32((uint32_t)a2, 0), a4v::castps_si128(vf7));
    do { const a4::q128 _v = (A4_R128(ADD32((uint32_t)a1, 0))); h2 = a4::hi64(_v); std::memcpy(&v0, &_v, 8); } while (0);
    A4_W128(ADD32((uint32_t)sp, 48), a4::mk128(h2, v0));
    do { const a4::q128 _v = (A4_R128(ADD32((uint32_t)sp, 0))); h8 = a4::hi64(_v); std::memcpy(&t0, &_v, 8); } while (0);
    do { const a4::q128 _v = (A4_R128(ADD32((uint32_t)sp, 16))); h9 = a4::hi64(_v); std::memcpy(&t1, &_v, 8); } while (0);
    do { const a4::q128 _v = (A4_R128(ADD32((uint32_t)sp, 32))); h10 = a4::hi64(_v); std::memcpy(&t2, &_v, 8); } while (0);
    vf1 = a4v::castsi128_ps(A4_R128(ADD32((uint32_t)sp, 48)));
    { a4::q128 mask = a4v::set_epi32(-1, -1, -1, -1); vf2 = a4v::blendv_ps(vf2, vf1, a4v::castsi128_ps(mask)); }
    { a4::vec4 res = a4v::sub(vf1, vf1); a4::q128 mask = a4v::set_epi32(0, -1, -1, -1); vf1 = a4v::blend(vf1, res, a4v::castsi128_ps(mask)); }
    { a4::q128 mask = a4v::set_epi32(-1, -1, -1, -1); vf9 = a4v::blendv_ps(vf9, vf1, a4v::castsi128_ps(mask)); }
    do { const a4::q128 _v = (a4v::castps_si128(vf1)); h11 = a4::hi64(_v); std::memcpy(&t3, &_v, 8); } while (0);
    do { const a4::q128 _v = (a4m::pextlw(a4::mk128(h9, t1), a4::mk128(h8, t0))); h12 = a4::hi64(_v); std::memcpy(&t4, &_v, 8); } while (0);
    do { const a4::q128 _v = (a4m::pextuw(a4::mk128(h9, t1), a4::mk128(h8, t0))); h13 = a4::hi64(_v); std::memcpy(&t5, &_v, 8); } while (0);
    do { const a4::q128 _v = (a4m::pextlw(a4::mk128(h11, t3), a4::mk128(h10, t2))); h14 = a4::hi64(_v); std::memcpy(&t6, &_v, 8); } while (0);
    do { const a4::q128 _v = (a4m::pextuw(a4::mk128(h11, t3), a4::mk128(h10, t2))); h15 = a4::hi64(_v); std::memcpy(&t7, &_v, 8); } while (0);
    do { const a4::q128 _v = (a4m::pcpyld(a4::mk128(h14, t6), a4::mk128(h12, t4))); h8 = a4::hi64(_v); std::memcpy(&t0, &_v, 8); } while (0);
    do { const a4::q128 _v = (a4v::unpackhi_epi64(a4::mk128(h12, t4), a4::mk128(h14, t6))); h9 = a4::hi64(_v); std::memcpy(&t1, &_v, 8); } while (0);
    do { const a4::q128 _v = (a4m::pcpyld(a4::mk128(h15, t7), a4::mk128(h13, t5))); h10 = a4::hi64(_v); std::memcpy(&t2, &_v, 8); } while (0);
    vf3 = a4v::castsi128_ps(a4::mk128(h8, t0));
    vf4 = a4v::castsi128_ps(a4::mk128(h9, t1));
    vf8 = a4v::castsi128_ps(a4::mk128(h10, t2));
    { a4::vec4 res = a4v::mul(vf3, a4v::bcast<0>(vf2)); vacc = a4v::dest<1, 1, 1, 0>(vacc, res); }
    { a4::vec4 mul_res = a4v::mul(vf4, a4v::bcast<1>(vf2)); a4::vec4 res = a4v::addK<1, 1>(vacc, mul_res); vacc = a4v::dest<1, 1, 1, 0>(vacc, res); }
    { a4::vec4 mul_res = a4v::mul(vf8, a4v::bcast<2>(vf2)); a4::vec4 res = a4v::addK<1, 1>(vacc, mul_res); a4::q128 mask = a4v::set_epi32(0, -1, -1, -1); vf1 = a4v::blendv_ps(vf1, res, a4v::castsi128_ps(mask)); }
    { a4::vec4 res = a4v::subK<1, 1>(vf9, vf1); a4::q128 mask = a4v::set_epi32(0, -1, -1, -1); vf1 = a4v::blend(vf1, res, a4v::castsi128_ps(mask)); }
    A4_W128(ADD32((uint32_t)a0, 0), a4::mk128(h8, t0));
    A4_W128(ADD32((uint32_t)a0, 16), a4::mk128(h9, t1));
    A4_W128(ADD32((uint32_t)a0, 32), a4::mk128(h10, t2));
    A4_W128(ADD32((uint32_t)a0, 48), a4v::castps_si128(vf1));
    { (sp = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)sp, 80)))); if (xo) { xo[0] = a2; xo[1] = t0; xo[2] = t1; xo[3] = t2; xo[4] = t3; } io_vf1 = vf1; io_vf2 = vf2; io_vf3 = vf3; io_vf4 = vf4; io_vf5 = vf5; io_vf6 = vf6; io_vf7 = vf7; io_vf8 = vf8; io_vf9 = vf9; io_vf14 = vf14; io_vf15 = vf15; io_vf16 = vf16; io_vf17 = vf17; io_vacc = vacc; io_vq = vq; io_vstat = vstat; io_h2 = h2; io_h8 = h8; io_h9 = h9; io_h10 = h10; io_h11 = h11; io_h12 = h12; io_h13 = h13; io_h14 = h14; io_h15 = h15; return v0; }
    __builtin_unreachable();
}

// iosMvecAllOnes (0x1b9d88): typed by tools/typed_lift2.py (round A6; leaf; VU0 in -, out h2)
inline uint64_t iosMvecAllOnes(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint64_t a0, uint64_t &io_h2)
{
    uint64_t v0 = 0;
    uint64_t h2 = io_h2;
    do { const a4::q128 _v = (a4m::pceqw(a4v::setzero_si128(), a4v::setzero_si128())); h2 = a4::hi64(_v); std::memcpy(&v0, &_v, 8); } while (0);
    { A4_W128(ADD32((uint32_t)a0, 0), a4::mk128(h2, v0)); io_h2 = h2; return v0; }
    __builtin_unreachable();
}

// iosAddMvec (0x1b9da8): typed by tools/typed_lift2.py (round A6; guest frame leaf; VU0 in -, out h2 h3)
inline uint64_t iosAddMvec(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint64_t a0, uint64_t a1, uint64_t a2, uint64_t sp, uint64_t &io_h2, uint64_t &io_h3)
{
    uint64_t v0 = 0;
    uint64_t v1 = 0;
    uint64_t a3 = 0;
    uint64_t t0 = 0;
    uint64_t h2 = io_h2;
    uint64_t h3 = io_h3;
    (a3 = (uint64_t)(A1_R64B((uint32_t)a1, 8)));
    (sp = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)sp, 4294967264))));
    (t0 = (uint64_t)(A1_R64B((uint32_t)a2, 8)));
    (v0 = (uint64_t)(A1_R64B((uint32_t)a1, 0)));
    (v1 = (uint64_t)(A1_R64B((uint32_t)a2, 0)));
    A1_W64B((uint32_t)sp, 0, v0);
    A1_W64B((uint32_t)sp, 8, a3);
    A1_W64B((uint32_t)sp, 16, v1);
    A1_W64B((uint32_t)sp, 24, t0);
    do { const a4::q128 _v = (A4_R128(ADD32((uint32_t)sp, 0))); h2 = a4::hi64(_v); std::memcpy(&v0, &_v, 8); } while (0);
    do { const a4::q128 _v = (A4_R128(ADD32((uint32_t)sp, 16))); h3 = a4::hi64(_v); std::memcpy(&v1, &_v, 8); } while (0);
    do { const a4::q128 _v = (a4m::paddw(a4::mk128(h2, v0), a4::mk128(h3, v1))); h2 = a4::hi64(_v); std::memcpy(&v0, &_v, 8); } while (0);
    A4_W128(ADD32((uint32_t)a0, 0), a4::mk128(h2, v0));
    { (sp = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)sp, 32)))); io_h2 = h2; io_h3 = h3; return v0; }
    __builtin_unreachable();
}

// iosClampUW_AddMvec (0x1b9e28): typed by tools/typed_lift2.py (round A6; guest frame leaf; VU0 in -, out h2 h3)
inline uint64_t iosClampUW_AddMvec(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint64_t a0, uint64_t a1, uint64_t a2, uint64_t sp, uint64_t &io_h2, uint64_t &io_h3)
{
    uint64_t v0 = 0;
    uint64_t v1 = 0;
    uint64_t a3 = 0;
    uint64_t t0 = 0;
    uint64_t h2 = io_h2;
    uint64_t h3 = io_h3;
    (a3 = (uint64_t)(A1_R64B((uint32_t)a1, 8)));
    (sp = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)sp, 4294967264))));
    (t0 = (uint64_t)(A1_R64B((uint32_t)a2, 8)));
    (v0 = (uint64_t)(A1_R64B((uint32_t)a1, 0)));
    (v1 = (uint64_t)(A1_R64B((uint32_t)a2, 0)));
    A1_W64B((uint32_t)sp, 0, v0);
    A1_W64B((uint32_t)sp, 8, a3);
    A1_W64B((uint32_t)sp, 16, v1);
    A1_W64B((uint32_t)sp, 24, t0);
    do { const a4::q128 _v = (A4_R128(ADD32((uint32_t)sp, 0))); h2 = a4::hi64(_v); std::memcpy(&v0, &_v, 8); } while (0);
    do { const a4::q128 _v = (A4_R128(ADD32((uint32_t)sp, 16))); h3 = a4::hi64(_v); std::memcpy(&v1, &_v, 8); } while (0);
    do { const a4::q128 _v = (ps2_paddu32(a4::mk128(h2, v0), a4::mk128(h3, v1))); h2 = a4::hi64(_v); std::memcpy(&v0, &_v, 8); } while (0);
    A4_W128(ADD32((uint32_t)a0, 0), a4::mk128(h2, v0));
    { (sp = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)sp, 32)))); io_h2 = h2; io_h3 = h3; return v0; }
    __builtin_unreachable();
}

// iosSubMvec (0x1b9e68): typed by tools/typed_lift2.py (round A6; guest frame leaf; VU0 in -, out h2 h3)
inline uint64_t iosSubMvec(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint64_t a0, uint64_t a1, uint64_t a2, uint64_t sp, uint64_t &io_h2, uint64_t &io_h3)
{
    uint64_t v0 = 0;
    uint64_t v1 = 0;
    uint64_t a3 = 0;
    uint64_t t0 = 0;
    uint64_t h2 = io_h2;
    uint64_t h3 = io_h3;
    (a3 = (uint64_t)(A1_R64B((uint32_t)a1, 8)));
    (sp = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)sp, 4294967264))));
    (t0 = (uint64_t)(A1_R64B((uint32_t)a2, 8)));
    (v0 = (uint64_t)(A1_R64B((uint32_t)a1, 0)));
    (v1 = (uint64_t)(A1_R64B((uint32_t)a2, 0)));
    A1_W64B((uint32_t)sp, 0, v0);
    A1_W64B((uint32_t)sp, 8, a3);
    A1_W64B((uint32_t)sp, 16, v1);
    A1_W64B((uint32_t)sp, 24, t0);
    do { const a4::q128 _v = (A4_R128(ADD32((uint32_t)sp, 0))); h2 = a4::hi64(_v); std::memcpy(&v0, &_v, 8); } while (0);
    do { const a4::q128 _v = (A4_R128(ADD32((uint32_t)sp, 16))); h3 = a4::hi64(_v); std::memcpy(&v1, &_v, 8); } while (0);
    do { const a4::q128 _v = (a4m::psubw(a4::mk128(h2, v0), a4::mk128(h3, v1))); h2 = a4::hi64(_v); std::memcpy(&v0, &_v, 8); } while (0);
    A4_W128(ADD32((uint32_t)a0, 0), a4::mk128(h2, v0));
    { (sp = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)sp, 32)))); io_h2 = h2; io_h3 = h3; return v0; }
    __builtin_unreachable();
}

// kernel_sub_001BEC18 (0x1bec18): typed by tools/typed_lift2.py (round A6; guest frame leaf, extra outputs xo[0]=a0 xo[1]=a1 xo[2]=a2 xo[3]=a3 xo[4]=t0 xo[5]=t1 xo[6]=t2 xo[7]=t3; VU0 in -, out h2 h3)
inline uint64_t kernel_sub_001BEC18(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint64_t a0, uint64_t sp, uint64_t &io_h2, uint64_t &io_h3, uint64_t *xo = nullptr)
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
    uint64_t h2 = io_h2;
    uint64_t h3 = io_h3;
    (t8 = (uint64_t)((uint64_t)a0 + (uint64_t)0ULL));
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 10))));
    (sp = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)sp, 4294967216))));
    do { const a4::q128 _v = (a4m::por(a4v::setzero_si128(), a4v::setzero_si128())); h2 = a4::hi64(_v); std::memcpy(&v0, &_v, 8); } while (0);
    (s0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 25))));
    (s1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 64))));
    (s2 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 65))));
    (s3 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 69))));
    (s4 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 26))));
    (a2 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 4294934528))));
    (s6 = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)4352 << 16))));
    (s7 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 1))));
    (fp = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)24576 << 16))));
    A4_W128(ADD32((uint32_t)t8, 16), a4::mk128(h2, v0));
    (t0 = (uint64_t)(0ULL | (uint64_t)(uint16_t)32768));
    (a1 = (uint64_t)(uint32_t)((uint16_t)A1_R16B((uint32_t)t8, 360)));
    (t7 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 24))));
    (v1 = (uint64_t)(A1_R64B((uint32_t)t8, 16)));
    (v0 = (uint64_t)((uint64_t)RC_U64X(a1) + (uint64_t)0ULL));
    (t6 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)t8, 352))));
    (v1 = (uint64_t)(v1 & RC_U64X(a2)));
    (a2 = (uint64_t)(uint32_t)((uint16_t)A1_R16B((uint32_t)t8, 362)));
    (v1 = (uint64_t)(v1 | RC_U64X(a0)));
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 4294967295))));
    (a0 = (uint64_t)(RC_U64X(a0) >> 4));
    (v1 = (uint64_t)(v1 | RC_U64X(t0)));
    (t0 = (uint64_t)(0ULL | (uint64_t)(uint16_t)32768));
    (t0 = (uint64_t)(RC_U64X(t0) << (32 + 13)));
    (v1 = (uint64_t)(v1 & a0));
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 2048))));
    (v1 = (uint64_t)(v1 | t0));
    (t0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 4294967280))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)SRL32((uint32_t)v0, 6))));
    (t1 = (uint64_t)(v1 & (uint64_t)(uint16_t)10));
    (v0 = (uint64_t)(RC_U64X(v0) & (uint64_t)(uint16_t)63));
    (t4 = (uint64_t)(A1_R64B((uint32_t)t8, 24)));
    (t2 = (uint64_t)(A1_R64B((uint32_t)t8, 368)));
    (t9 = (uint64_t)((int64_t)(int32_t)((int32_t)SRL32((uint32_t)a1, 1))));
    (s5 = (uint64_t)((int64_t)(int32_t)((int32_t)SRL32((uint32_t)a2, 1))));
    (a2 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)a2, 4294967295))));
    (t3 = (uint64_t)(RC_U64X(t1) << (32 + 0)));
    (t3 = (uint64_t)((int64_t)t3 >> (32 + 0)));
    (t1 = (uint64_t)((int64_t)(int64_t)RC_U64X(t1) + (int64_t)(int32_t)1));
    (v0 = (uint64_t)(RC_U64X(v0) << 16));
    (a3 = (uint64_t)((int64_t)(int32_t)((int32_t)SUB32((uint32_t)a0, (uint32_t)s5))));
    (t6 = (uint64_t)(RC_U64X(t6) | v0));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)20480 << 16))));
    (t5 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)t8, 356))));
    (a1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)a1, 4294967295))));
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)SUB32((uint32_t)a0, (uint32_t)t9))));
    (t3 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)t3, 1))));
    (a3 = (uint64_t)(RC_U64X(a3) << (32 + 4)));
    (a2 = (uint64_t)(RC_U64X(a2) << (32 + 16)));
    (t4 = (uint64_t)(t4 & RC_U64X(t0)));
    (t0 = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)12544 << 16))));
    (t3 = (uint64_t)(RC_U64X(t3) | RC_U64X(v0)));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 14))));
    (a0 = (uint64_t)(RC_U64X(a0) << 4));
    (a1 = (uint64_t)(RC_U64X(a1) << 16));
    (t2 = (uint64_t)(t2 << 31));
    (t2 = (uint64_t)((int64_t)t2 >> (32 + 0)));
    (t5 = (uint64_t)(RC_U64X(t5) | RC_U64X(t0)));
    (a0 = (uint64_t)(a0 | a3));
    (a3 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 78))));
    (a1 = (uint64_t)(a1 | a2));
    (a2 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 77))));
    (t4 = (uint64_t)(t4 | RC_U64X(v0)));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 76))));
    (t2 = (uint64_t)(t2 & (uint64_t)(uint16_t)1));
    (t1 = (uint64_t)(t1 | RC_U64X(fp)));
    (t0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 79))));
    A1_W64B((uint32_t)t8, 40, RC_U64X(v0));
    A1_W64B((uint32_t)t8, 56, RC_U64X(a2));
    A1_W64B((uint32_t)t8, 32, t6);
    A1_W64B((uint32_t)t8, 72, RC_U64X(a3));
    A1_W64B((uint32_t)t8, 88, RC_U64X(t0));
    A1_W64B((uint32_t)t8, 64, RC_U64X(t5));
    A1_W64B((uint32_t)t8, 104, RC_U64X(t7));
    A1_W64B((uint32_t)t8, 120, RC_U64X(s0));
    A1_W64B((uint32_t)t8, 96, a0);
    A1_W64B((uint32_t)t8, 136, RC_U64X(s1));
    A1_W64B((uint32_t)t8, 152, RC_U64X(s2));
    A1_W64B((uint32_t)t8, 128, a1);
    A1_W64B((uint32_t)t8, 168, RC_U64X(s3));
    A1_W64B((uint32_t)t8, 160, RC_U64X(t2));
    A1_W64B((uint32_t)t8, 184, RC_U64X(s4));
    A1_W64B((uint32_t)t8, 24, t4);
    A1_W64B((uint32_t)t8, 0, t1);
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)t8, 368))));
    A1_W64B((uint32_t)t8, 48, t6);
    (v0 = (uint64_t)(RC_U64X(v0) & (uint64_t)(uint16_t)1));
    A1_W64B((uint32_t)t8, 80, RC_U64X(t5));
    A1_W64B((uint32_t)t8, 112, a0);
    A1_W64B((uint32_t)t8, 144, a1);
    A1_W64B((uint32_t)t8, 176, RC_U64X(s7));
    A1_W64B((uint32_t)t8, 16, v1);
    A1_W32B((uint32_t)t8, 8, (uint32_t)s6);
    A1_W32B((uint32_t)t8, 12, (uint32_t)t3);
    A1_W32B((uint32_t)t8, 192, 0u);
    A1_W32B((uint32_t)t8, 196, 0u);
    A1_W32B((uint32_t)t8, 200, 0u);
    { const bool c = (int64_t)v0 == 0; A1_W32B((uint32_t)t8, 204, (uint32_t)s6); if (c) goto L_1beef8; }
    do { const a4::q128 _v = (a4m::por(a4v::setzero_si128(), a4v::setzero_si128())); h3 = a4::hi64(_v); std::memcpy(&v1, &_v, 8); } while (0);
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 4294934528))));
    A4_W128(ADD32((uint32_t)t8, 224), a4::mk128(h3, v1));
    (t5 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 6))));
    (a2 = (uint64_t)(0ULL | (uint64_t)(uint16_t)32768));
    (t0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 4294967295))));
    (t0 = (uint64_t)(RC_U64X(t0) >> 4));
    (v0 = (uint64_t)(A1_R64B((uint32_t)t8, 224)));
    (t3 = (uint64_t)((int64_t)(int32_t)((int32_t)SUB32(0u, (uint32_t)t9))));
    (v1 = (uint64_t)(uint32_t)((uint8_t)A1_R8B((uint32_t)t8, 365)));
    (t2 = (uint64_t)((int64_t)(int32_t)((int32_t)SUB32(0u, (uint32_t)s5))));
    (v0 = (uint64_t)(v0 & RC_U64X(a0)));
    (a0 = (uint64_t)(uint32_t)((uint8_t)A1_R8B((uint32_t)t8, 366)));
    (v0 = (uint64_t)(v0 | RC_U64X(t5)));
    (a1 = (uint64_t)(uint32_t)((uint8_t)A1_R8B((uint32_t)t8, 367)));
    (v0 = (uint64_t)(v0 | RC_U64X(a2)));
    (a2 = (uint64_t)(0ULL | (uint64_t)(uint16_t)32768));
    (a2 = (uint64_t)(RC_U64X(a2) << (32 + 13)));
    (v0 = (uint64_t)(v0 & t0));
    (t1 = (uint64_t)(A1_R64B((uint32_t)t8, 232)));
    (v0 = (uint64_t)(v0 | a2));
    (v1 = (uint64_t)(RC_U64X(v1) << 8));
    (a3 = (uint64_t)(uint32_t)((uint8_t)A1_R8B((uint32_t)t8, 364)));
    (a0 = (uint64_t)(RC_U64X(a0) << 16));
    (a0 = (uint64_t)(a0 | v1));
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 4294967280))));
    (a1 = (uint64_t)(RC_U64X(a1) << 24));
    (a2 = (uint64_t)(v0 & (uint64_t)(uint16_t)32767));
    (t0 = (uint64_t)(RC_U64X(a2) << (32 + 0)));
    (t0 = (uint64_t)((int64_t)t0 >> (32 + 0)));
    (a2 = (uint64_t)((int64_t)(int64_t)RC_U64X(a2) + (int64_t)(int32_t)1));
    (a3 = (uint64_t)(RC_U64X(a3) | a1));
    (a1 = (uint64_t)(RC_U64X(t9) << 4));
    (t1 = (uint64_t)(t1 & RC_U64X(v1)));
    (v1 = (uint64_t)(RC_U64X(s5) << 20));
    (t2 = (uint64_t)(RC_U64X(t2) << 20));
    (a3 = (uint64_t)(a3 | a0));
    (a1 = (uint64_t)(a1 | v1));
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 14))));
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)20480 << 16))));
    (t3 = (uint64_t)(RC_U64X(t3) << 4));
    (t0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)t0, 1))));
    (t3 = (uint64_t)(t3 | t2));
    (t0 = (uint64_t)(RC_U64X(t0) | RC_U64X(a0)));
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)5 << 16))));
    (a0 = (uint64_t)(RC_U64X(a0) | (uint64_t)(uint16_t)13));
    (t1 = (uint64_t)(t1 | RC_U64X(v1)));
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)3 << 16))));
    (t2 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 71))));
    (t4 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 4))));
    (a2 = (uint64_t)(a2 | RC_U64X(fp)));
    A1_W32B((uint32_t)t8, 348, (uint32_t)s6);
    A1_W64B((uint32_t)t8, 240, RC_U64X(v1));
    A1_W64B((uint32_t)t8, 280, RC_U64X(s7));
    A1_W64B((uint32_t)t8, 272, a3);
    A1_W64B((uint32_t)t8, 288, t3);
    A1_W64B((uint32_t)t8, 312, RC_U64X(t4));
    A1_W64B((uint32_t)t8, 304, a1);
    A1_W64B((uint32_t)t8, 328, RC_U64X(t2));
    A1_W64B((uint32_t)t8, 320, RC_U64X(a0));
    A1_W64B((uint32_t)t8, 232, t1);
    A1_W64B((uint32_t)t8, 208, a2);
    A1_W32B((uint32_t)t8, 220, (uint32_t)t0);
    A1_W64B((uint32_t)t8, 248, RC_U64X(t2));
    A1_W64B((uint32_t)t8, 264, 0ULL);
    A1_W64B((uint32_t)t8, 256, RC_U64X(t5));
    A1_W64B((uint32_t)t8, 296, RC_U64X(t4));
    A1_W64B((uint32_t)t8, 224, v0);
    A1_W32B((uint32_t)t8, 216, 0u);
    A1_W32B((uint32_t)t8, 336, 0u);
    A1_W32B((uint32_t)t8, 340, 0u);
    A1_W32B((uint32_t)t8, 344, 0u);
L_1beef8:;
    { (sp = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)sp, 80)))); if (xo) { xo[0] = a0; xo[1] = a1; xo[2] = a2; xo[3] = a3; xo[4] = t0; xo[5] = t1; xo[6] = t2; xo[7] = t3; } io_h2 = h2; io_h3 = h3; return v0; }
    __builtin_unreachable();
}

// GetMultiLocalizeMatrix (0x1132e20): typed by tools/typed_lift2.py (round A6; loop leaf, extra outputs xo[0]=t2 xo[1]=t3; VU0 in vf0 vf25 vf28 vf29 vacc, out vf24 vf25 vf26 vf28 vf29 vacc h8 h9 h10 h11 h12 h13 h14 h15)
inline uint64_t GetMultiLocalizeMatrix(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint64_t a0, uint64_t a1, uint64_t a2, const a4::vec4 &io_vf0, a4::vec4 &io_vf24, a4::vec4 &io_vf25, a4::vec4 &io_vf26, a4::vec4 &io_vf28, a4::vec4 &io_vf29, a4::vec4 &io_vacc, uint64_t &io_h8, uint64_t &io_h9, uint64_t &io_h10, uint64_t &io_h11, uint64_t &io_h12, uint64_t &io_h13, uint64_t &io_h14, uint64_t &io_h15, uint64_t *xo = nullptr)
{
    uint64_t v0 = 0;
    uint64_t v1 = 0;
    uint64_t t0 = 0;
    uint64_t t1 = 0;
    uint64_t t2 = 0;
    uint64_t t3 = 0;
    uint64_t t4 = 0;
    uint64_t t5 = 0;
    uint64_t t6 = 0;
    uint64_t t7 = 0;
    a4::vec4 vf0 = io_vf0;
    a4::vec4 vf24 = io_vf24;
    a4::vec4 vf25 = io_vf25;
    a4::vec4 vf26 = io_vf26;
    a4::vec4 vf28 = io_vf28;
    a4::vec4 vf29 = io_vf29;
    a4::vec4 vacc = io_vacc;
    uint64_t h8 = io_h8;
    uint64_t h9 = io_h9;
    uint64_t h10 = io_h10;
    uint64_t h11 = io_h11;
    uint64_t h12 = io_h12;
    uint64_t h13 = io_h13;
    uint64_t h14 = io_h14;
    uint64_t h15 = io_h15;
    bool a3xw0 = false;
    bool a3xw1 = false;
    { const bool c = (int64_t)a2 == 0; (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)a2, 4294967295)))); if (c) goto L_1132ebc; }
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 4294967295))));
L_1132e30:;
    vf24 = a4v::castsi128_ps(A4_R128(ADD32((uint32_t)a1, 48)));
    { a4::vec4 res = a4v::subK<1, 1>(vf0, a4v::bcast<3>(vf0)); a4::q128 mask = a4v::set_epi32(0, 0, 0, -1); vf25 = a4v::blendv_ps(vf25, res, a4v::castsi128_ps(mask)); }
    { a4::vec4 res = a4v::mulK<0, 1>(vf24, a4v::bcast<0>(vf25)); a4::q128 mask = a4v::set_epi32(0, -1, -1, -1); vf28 = a4v::blendv_ps(vf28, res, a4v::castsi128_ps(mask)); }
    { a4::vec4 res = a4v::sub(vf28, vf28); a4::q128 mask = a4v::set_epi32(-1, 0, 0, 0); vf28 = a4v::blend(vf28, res, a4v::castsi128_ps(mask)); }
    do { const a4::q128 _v = (A4_R128(ADD32((uint32_t)a1, 0))); h8 = a4::hi64(_v); std::memcpy(&t0, &_v, 8); } while (0);
    do { const a4::q128 _v = (A4_R128(ADD32((uint32_t)a1, 16))); h9 = a4::hi64(_v); std::memcpy(&t1, &_v, 8); } while (0);
    do { const a4::q128 _v = (A4_R128(ADD32((uint32_t)a1, 32))); h10 = a4::hi64(_v); std::memcpy(&t2, &_v, 8); } while (0); a3xw0 = true;
    do { const a4::q128 _v = (a4v::castps_si128(vf0)); h11 = a4::hi64(_v); std::memcpy(&t3, &_v, 8); } while (0); a3xw1 = true;
    do { const a4::q128 _v = (a4m::pextlw(a4::mk128(h9, t1), a4::mk128(h8, t0))); h12 = a4::hi64(_v); std::memcpy(&t4, &_v, 8); } while (0);
    do { const a4::q128 _v = (a4m::pextuw(a4::mk128(h9, t1), a4::mk128(h8, t0))); h13 = a4::hi64(_v); std::memcpy(&t5, &_v, 8); } while (0);
    do { const a4::q128 _v = (a4m::pextlw(a4::mk128(h11, t3), a4::mk128(h10, t2))); h14 = a4::hi64(_v); std::memcpy(&t6, &_v, 8); } while (0);
    do { const a4::q128 _v = (a4m::pextuw(a4::mk128(h11, t3), a4::mk128(h10, t2))); h15 = a4::hi64(_v); std::memcpy(&t7, &_v, 8); } while (0);
    do { const a4::q128 _v = (a4m::pcpyld(a4::mk128(h14, t6), a4::mk128(h12, t4))); h8 = a4::hi64(_v); std::memcpy(&t0, &_v, 8); } while (0);
    do { const a4::q128 _v = (a4v::unpackhi_epi64(a4::mk128(h12, t4), a4::mk128(h14, t6))); h9 = a4::hi64(_v); std::memcpy(&t1, &_v, 8); } while (0);
    do { const a4::q128 _v = (a4m::pcpyld(a4::mk128(h15, t7), a4::mk128(h13, t5))); h10 = a4::hi64(_v); std::memcpy(&t2, &_v, 8); } while (0); a3xw0 = true;
    do { const a4::q128 _v = (a4v::unpackhi_epi64(a4::mk128(h13, t5), a4::mk128(h15, t7))); h11 = a4::hi64(_v); std::memcpy(&t3, &_v, 8); } while (0); a3xw1 = true;
    vf24 = a4v::castsi128_ps(a4::mk128(h8, t0));
    vf25 = a4v::castsi128_ps(a4::mk128(h9, t1));
    vf26 = a4v::castsi128_ps(a4::mk128(h10, t2));
    { a4::vec4 res = a4v::mulK<0, 1>(vf24, a4v::bcast<0>(vf28)); vacc = a4v::dest<1, 1, 1, 1>(vacc, res); }
    { a4::vec4 mul_res = a4v::mulK<0, 1>(vf25, a4v::bcast<1>(vf28)); a4::vec4 res = a4v::addK<1, 1>(vacc, mul_res); vacc = a4v::dest<1, 1, 1, 1>(vacc, res); }
    { a4::vec4 mul_res = a4v::mulK<0, 1>(vf26, a4v::bcast<2>(vf28)); a4::vec4 res = a4v::addK<1, 1>(vacc, mul_res); vacc = a4v::dest<1, 1, 1, 1>(vacc, res); }
    { a4::vec4 mul_res = a4v::mulK<1, 1>(vf0, a4v::bcast<3>(vf28)); a4::vec4 res = a4v::addK<1, 1>(vacc, mul_res); a4::q128 mask = a4v::set_epi32(-1, -1, -1, -1); vf29 = a4v::blendv_ps(vf29, res, a4v::castsi128_ps(mask)); }
    { a4::vec4 res = a4v::subK<1, 1>(vf29, vf29); a4::q128 mask = a4v::set_epi32(-1, 0, 0, 0); vf29 = a4v::blend(vf29, res, a4v::castsi128_ps(mask)); }
    { a4::vec4 res = a4v::addK<1, 1>(vf29, vf0); a4::q128 mask = a4v::set_epi32(-1, 0, 0, 0); vf29 = a4v::blend(vf29, res, a4v::castsi128_ps(mask)); }
    A4_W128(ADD32((uint32_t)a0, 0), a4v::castps_si128(vf24));
    A4_W128(ADD32((uint32_t)a0, 16), a4v::castps_si128(vf25));
    A4_W128(ADD32((uint32_t)a0, 32), a4v::castps_si128(vf26));
    A4_W128(ADD32((uint32_t)a0, 48), a4v::castps_si128(vf29));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v0, 4294967295))));
    (a1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)a1, 64))));
    { const bool c = (int64_t)v0 != (int64_t)v1; (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)a0, 64)))); if (c) { A3_BACK(); goto L_1132e30; } }
L_1132ebc:;
    { if (xo) { if (a3xw0) xo[0] = t2; if (a3xw1) xo[1] = t3; } io_vf24 = vf24; io_vf25 = vf25; io_vf26 = vf26; io_vf28 = vf28; io_vf29 = vf29; io_vacc = vacc; io_h8 = h8; io_h9 = h9; io_h10 = h10; io_h11 = h11; io_h12 = h12; io_h13 = h13; io_h14 = h14; io_h15 = h15; return v0; }
    __builtin_unreachable();
}

// GetTargetLocalDir (0x11359f8): typed by tools/typed_lift2.py (round A6; guest frame leaf, extra outputs xo[0]=t0 xo[1]=t1 xo[2]=t2 xo[3]=t3; VU0 in vf0 vf10 vf12 vf13 vf14 vf15 vf16 vf17 vf25 vf28 vf29 vacc, out vf8 vf10 vf11 vf12 vf13 vf14 vf15 vf16 vf17 vf24 vf25 vf26 vf28 vf29 vacc h8 h9 h10 h11 h12 h13 h14 h15)
inline a1::RetVF GetTargetLocalDir(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint64_t a0, uint64_t a1, uint64_t a2, uint64_t sp, const a4::vec4 &io_vf0, a4::vec4 &io_vf8, a4::vec4 &io_vf10, a4::vec4 &io_vf11, a4::vec4 &io_vf12, a4::vec4 &io_vf13, a4::vec4 &io_vf14, a4::vec4 &io_vf15, a4::vec4 &io_vf16, a4::vec4 &io_vf17, a4::vec4 &io_vf24, a4::vec4 &io_vf25, a4::vec4 &io_vf26, a4::vec4 &io_vf28, a4::vec4 &io_vf29, a4::vec4 &io_vacc, uint64_t &io_h8, uint64_t &io_h9, uint64_t &io_h10, uint64_t &io_h11, uint64_t &io_h12, uint64_t &io_h13, uint64_t &io_h14, uint64_t &io_h15, uint64_t *xo = nullptr)
{
    float f0 = 0.0f;
    uint64_t v0 = 0;
    uint64_t v1 = 0;
    uint64_t t0 = 0;
    uint64_t t1 = 0;
    uint64_t t2 = 0;
    uint64_t t3 = 0;
    uint64_t t4 = 0;
    uint64_t t5 = 0;
    uint64_t t6 = 0;
    uint64_t t7 = 0;
    a4::vec4 vf0 = io_vf0;
    a4::vec4 vf8 = io_vf8;
    a4::vec4 vf10 = io_vf10;
    a4::vec4 vf11 = io_vf11;
    a4::vec4 vf12 = io_vf12;
    a4::vec4 vf13 = io_vf13;
    a4::vec4 vf14 = io_vf14;
    a4::vec4 vf15 = io_vf15;
    a4::vec4 vf16 = io_vf16;
    a4::vec4 vf17 = io_vf17;
    a4::vec4 vf24 = io_vf24;
    a4::vec4 vf25 = io_vf25;
    a4::vec4 vf26 = io_vf26;
    a4::vec4 vf28 = io_vf28;
    a4::vec4 vf29 = io_vf29;
    a4::vec4 vacc = io_vacc;
    uint64_t h8 = io_h8;
    uint64_t h9 = io_h9;
    uint64_t h10 = io_h10;
    uint64_t h11 = io_h11;
    uint64_t h12 = io_h12;
    uint64_t h13 = io_h13;
    uint64_t h14 = io_h14;
    uint64_t h15 = io_h15;
    (sp = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)sp, 4294967168))));
    (v1 = (uint64_t)((uint64_t)RC_U64X(sp) + (uint64_t)0ULL));
    { a4::vec4 res = a4v::addK<1, 1>(vf0, a4v::bcast<3>(vf0)); a4::q128 mask = a4v::set_epi32(0, -1, -1, -1); vf12 = a4v::blendv_ps(vf12, res, a4v::castsi128_ps(mask)); }
    vf11 = a4v::castsi128_ps(A4_R128(ADD32((uint32_t)a1, 0)));
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
    A4_W128(ADD32((uint32_t)v1, 0), a4v::castps_si128(vf14));
    A4_W128(ADD32((uint32_t)v1, 16), a4v::castps_si128(vf15));
    A4_W128(ADD32((uint32_t)v1, 32), a4v::castps_si128(vf16));
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v1, 48))));
    A4_W128(ADD32((uint32_t)v1, 0), a4v::castps_si128(vf0));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)sp, 64))));
    vf24 = a4v::castsi128_ps(A4_R128(ADD32((uint32_t)sp, 48)));
    { a4::vec4 res = a4v::subK<1, 1>(vf0, a4v::bcast<3>(vf0)); a4::q128 mask = a4v::set_epi32(0, 0, 0, -1); vf25 = a4v::blendv_ps(vf25, res, a4v::castsi128_ps(mask)); }
    { a4::vec4 res = a4v::mulK<0, 1>(vf24, a4v::bcast<0>(vf25)); a4::q128 mask = a4v::set_epi32(0, -1, -1, -1); vf28 = a4v::blendv_ps(vf28, res, a4v::castsi128_ps(mask)); }
    { a4::vec4 res = a4v::sub(vf28, vf28); a4::q128 mask = a4v::set_epi32(-1, 0, 0, 0); vf28 = a4v::blend(vf28, res, a4v::castsi128_ps(mask)); }
    do { const a4::q128 _v = (A4_R128(ADD32((uint32_t)sp, 0))); h8 = a4::hi64(_v); std::memcpy(&t0, &_v, 8); } while (0);
    do { const a4::q128 _v = (A4_R128(ADD32((uint32_t)sp, 16))); h9 = a4::hi64(_v); std::memcpy(&t1, &_v, 8); } while (0);
    do { const a4::q128 _v = (A4_R128(ADD32((uint32_t)sp, 32))); h10 = a4::hi64(_v); std::memcpy(&t2, &_v, 8); } while (0);
    do { const a4::q128 _v = (a4v::castps_si128(vf0)); h11 = a4::hi64(_v); std::memcpy(&t3, &_v, 8); } while (0);
    do { const a4::q128 _v = (a4m::pextlw(a4::mk128(h9, t1), a4::mk128(h8, t0))); h12 = a4::hi64(_v); std::memcpy(&t4, &_v, 8); } while (0);
    do { const a4::q128 _v = (a4m::pextuw(a4::mk128(h9, t1), a4::mk128(h8, t0))); h13 = a4::hi64(_v); std::memcpy(&t5, &_v, 8); } while (0);
    do { const a4::q128 _v = (a4m::pextlw(a4::mk128(h11, t3), a4::mk128(h10, t2))); h14 = a4::hi64(_v); std::memcpy(&t6, &_v, 8); } while (0);
    do { const a4::q128 _v = (a4m::pextuw(a4::mk128(h11, t3), a4::mk128(h10, t2))); h15 = a4::hi64(_v); std::memcpy(&t7, &_v, 8); } while (0);
    do { const a4::q128 _v = (a4m::pcpyld(a4::mk128(h14, t6), a4::mk128(h12, t4))); h8 = a4::hi64(_v); std::memcpy(&t0, &_v, 8); } while (0);
    do { const a4::q128 _v = (a4v::unpackhi_epi64(a4::mk128(h12, t4), a4::mk128(h14, t6))); h9 = a4::hi64(_v); std::memcpy(&t1, &_v, 8); } while (0);
    do { const a4::q128 _v = (a4m::pcpyld(a4::mk128(h15, t7), a4::mk128(h13, t5))); h10 = a4::hi64(_v); std::memcpy(&t2, &_v, 8); } while (0);
    do { const a4::q128 _v = (a4v::unpackhi_epi64(a4::mk128(h13, t5), a4::mk128(h15, t7))); h11 = a4::hi64(_v); std::memcpy(&t3, &_v, 8); } while (0);
    vf24 = a4v::castsi128_ps(a4::mk128(h8, t0));
    vf25 = a4v::castsi128_ps(a4::mk128(h9, t1));
    vf26 = a4v::castsi128_ps(a4::mk128(h10, t2));
    { a4::vec4 res = a4v::mulK<0, 1>(vf24, a4v::bcast<0>(vf28)); vacc = a4v::dest<1, 1, 1, 1>(vacc, res); }
    { a4::vec4 mul_res = a4v::mulK<0, 1>(vf25, a4v::bcast<1>(vf28)); a4::vec4 res = a4v::addK<1, 1>(vacc, mul_res); vacc = a4v::dest<1, 1, 1, 1>(vacc, res); }
    { a4::vec4 mul_res = a4v::mulK<0, 1>(vf26, a4v::bcast<2>(vf28)); a4::vec4 res = a4v::addK<1, 1>(vacc, mul_res); vacc = a4v::dest<1, 1, 1, 1>(vacc, res); }
    { a4::vec4 mul_res = a4v::mulK<1, 1>(vf0, a4v::bcast<3>(vf28)); a4::vec4 res = a4v::addK<1, 1>(vacc, mul_res); a4::q128 mask = a4v::set_epi32(-1, -1, -1, -1); vf29 = a4v::blendv_ps(vf29, res, a4v::castsi128_ps(mask)); }
    { a4::vec4 res = a4v::subK<1, 1>(vf29, vf29); a4::q128 mask = a4v::set_epi32(-1, 0, 0, 0); vf29 = a4v::blend(vf29, res, a4v::castsi128_ps(mask)); }
    { a4::vec4 res = a4v::addK<1, 1>(vf29, vf0); a4::q128 mask = a4v::set_epi32(-1, 0, 0, 0); vf29 = a4v::blend(vf29, res, a4v::castsi128_ps(mask)); }
    A4_W128(ADD32((uint32_t)v0, 0), a4v::castps_si128(vf24));
    A4_W128(ADD32((uint32_t)v0, 16), a4v::castps_si128(vf25));
    A4_W128(ADD32((uint32_t)v0, 32), a4v::castps_si128(vf26));
    A4_W128(ADD32((uint32_t)v0, 48), a4v::castps_si128(vf29));
    vf8 = a4v::castsi128_ps(A4_R128(ADD32((uint32_t)a2, 0)));
    vf14 = a4v::castsi128_ps(A4_R128(ADD32((uint32_t)v0, 0)));
    vf15 = a4v::castsi128_ps(A4_R128(ADD32((uint32_t)v0, 16)));
    vf16 = a4v::castsi128_ps(A4_R128(ADD32((uint32_t)v0, 32)));
    vf17 = a4v::castsi128_ps(A4_R128(ADD32((uint32_t)v0, 48)));
    { a4::vec4 res = a4v::mul(vf14, a4v::bcast<0>(vf8)); vacc = a4v::dest<1, 1, 1, 1>(vacc, res); }
    { a4::vec4 mul_res = a4v::mul(vf15, a4v::bcast<1>(vf8)); a4::vec4 res = a4v::addK<1, 1>(vacc, mul_res); vacc = a4v::dest<1, 1, 1, 1>(vacc, res); }
    { a4::vec4 mul_res = a4v::mul(vf16, a4v::bcast<2>(vf8)); a4::vec4 res = a4v::addK<1, 1>(vacc, mul_res); vacc = a4v::dest<1, 1, 1, 1>(vacc, res); }
    { a4::vec4 mul_res = a4v::mul(vf17, a4v::bcast<3>(vf8)); a4::vec4 res = a4v::addK<1, 1>(vacc, mul_res); a4::q128 mask = a4v::set_epi32(-1, -1, -1, -1); vf10 = a4v::blendv_ps(vf10, res, a4v::castsi128_ps(mask)); }
    A4_W128(ADD32((uint32_t)a0, 0), a4v::castps_si128(vf10));
    { uint32_t bits = A1_R32B((uint32_t)a0, 4); float f; std::memcpy(&f, &bits, sizeof(f)); f0 = f; }
    { (sp = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)sp, 128)))); if (xo) { xo[0] = t0; xo[1] = t1; xo[2] = t2; xo[3] = t3; } io_vf8 = vf8; io_vf10 = vf10; io_vf11 = vf11; io_vf12 = vf12; io_vf13 = vf13; io_vf14 = vf14; io_vf15 = vf15; io_vf16 = vf16; io_vf17 = vf17; io_vf24 = vf24; io_vf25 = vf25; io_vf26 = vf26; io_vf28 = vf28; io_vf29 = vf29; io_vacc = vacc; io_h8 = h8; io_h9 = h9; io_h10 = h10; io_h11 = h11; io_h12 = h12; io_h13 = h13; io_h14 = h14; io_h15 = h15; return a1::RetVF{(uint64_t)v0, f0}; }
    __builtin_unreachable();
}

// sub_00101EC0 (0x101ec0): typed by tools/typed_lift2.py (round A6; loop guest frame calls, extra outputs xo[0]=a1 xo[1]=a2 xo[2]=a3 xo[3]=t0 xo[4]=t1 xo[5]=t2 xo[6]=t3; VU0 in h3, out h2)
inline uint64_t sub_00101EC0(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint64_t a0, uint64_t a1, uint64_t a2, uint64_t a3, uint64_t t0, uint64_t t1, uint64_t t2, uint64_t sp, float f12, float f13, float f14, float f15, float f16, float f17, float f18, float f19, uint64_t &io_h2, uint64_t io_h3, uint64_t *xo = nullptr)
{
    uint64_t v0 = 0;
    uint64_t v1 = 0;
    uint64_t t3 = 0;
    uint64_t s0 = 0;
    uint64_t s1 = 0;
    uint64_t s2 = 0;
    uint64_t s3 = 0;
    uint64_t s4 = 0;
    uint64_t h2 = io_h2;
    uint64_t h3 = io_h3;
    bool a3xw0 = false;
    bool a3xw1 = false;
    bool a3xw2 = false;
    bool a3xw3 = false;
    bool a3xw4 = false;
    bool a3xw5 = false;
    bool a3xw6 = false;
    (sp = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)sp, 4294967232))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 1))));
    (s0 = (uint64_t)((uint64_t)t1 + (uint64_t)0ULL));
    (s1 = (uint64_t)((uint64_t)a2 + (uint64_t)0ULL));
    (s2 = (uint64_t)((uint64_t)t0 + (uint64_t)0ULL));
    (s3 = (uint64_t)((uint64_t)a3 + (uint64_t)0ULL));
    (s4 = (uint64_t)((uint64_t)a1 + (uint64_t)0ULL));
    if ((int64_t)t2 == (int64_t)v0) goto L_101ff0;
    { const bool c = (int64_t)t2 == 0; (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)SLL32((uint32_t)a0, 3)))); if (c) goto L_101f18; }
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 2))));
    { const bool c = (int64_t)t2 == (int64_t)v0; (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)20 << 16)))); if (c) goto L_102050; }
    goto L_1020a4;
L_101f18:;
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)65472 << 16))));
    (a2 = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)19 << 16)))); a3xw1 = true;
    (a2 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)a2, (uint32_t)v0)))); a3xw1 = true;
    (a2 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)a2, 7428)))); a3xw1 = true;
    (a1 = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)19 << 16)))); a3xw0 = true;
    (a1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)a1, 26488)))); a3xw0 = true;
    { (a0 = (uint64_t)(RC_U64X(a0) | (uint64_t)(uint16_t)16384)); A3_CHARGE(); PutString(ram, rt, f12, f13, f14, f15, f16, f17, f18, f19); }
    (a1 = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)19 << 16)))); a3xw0 = true;
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)16639 << 16))));
    (a2 = (uint64_t)((uint64_t)s1 + (uint64_t)0ULL)); a3xw1 = true;
    (a1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)a1, 26504)))); a3xw0 = true;
    { (a0 = (uint64_t)(RC_U64X(a0) | (uint64_t)(uint16_t)49152)); A3_CHARGE(); PutString(ram, rt, f12, f13, f14, f15, f16, f17, f18, f19); }
    (a1 = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)19 << 16)))); a3xw0 = true;
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)16576 << 16))));
    (a3 = (uint64_t)((uint64_t)s2 + (uint64_t)0ULL)); a3xw2 = true;
    (a1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)a1, 26520)))); a3xw0 = true;
    (a2 = (uint64_t)((uint64_t)s3 + (uint64_t)0ULL)); a3xw1 = true;
    (a0 = (uint64_t)(RC_U64X(a0) | (uint64_t)(uint16_t)65280));
    { (s1 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL)); A3_CHARGE(); PutString(ram, rt, f12, f13, f14, f15, f16, f17, f18, f19); }
    (a1 = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)19 << 16)))); a3xw0 = true;
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)49344 << 16))));
    (a1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)a1, 26544)))); a3xw0 = true;
    (a2 = (uint64_t)((uint64_t)s4 + (uint64_t)0ULL)); a3xw1 = true;
    { (a0 = (uint64_t)(RC_U64X(a0) | (uint64_t)(uint16_t)49152)); A3_CHARGE(); PutString(ram, rt, f12, f13, f14, f15, f16, f17, f18, f19); }
    (a1 = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)20 << 16)))); a3xw0 = true;
    (a1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)a1, 4294942704)))); a3xw0 = true;
    { (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 4294967040)))); A3_CHARGE(); PutString(ram, rt, f12, f13, f14, f15, f16, f17, f18, f19); }
    (s2 = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)19 << 16))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)s0, 24))));
L_101fa0:;
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)32928 << 16))));
    (a2 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)s0, 4)))); a3xw1 = true;
    (a0 = (uint64_t)(RC_U64X(a0) | (uint64_t)(uint16_t)49152));
    (a3 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)s0, 0)))); a3xw2 = true;
    (a1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)s2, 26448)))); a3xw0 = true;
    (t0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)s0, 12)))); a3xw3 = true;
    (s1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)s1, 1))));
    (t1 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)s0, 8)))); a3xw4 = true;
    (t2 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)s0, 20)))); a3xw5 = true;
    (t3 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)s0, 16)))); a3xw6 = true;
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)s0, 28))));
    (s0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)s0, 32))));
    A1_W32B((uint32_t)sp, 8, (uint32_t)v0);
    { A1_W32B((uint32_t)sp, 0, (uint32_t)v1); A3_CHARGE(); PutString(ram, rt, f12, f13, f14, f15, f16, f17, f18, f19); }
    (v0 = (uint64_t)(((int64_t)(int64_t)s1 < (int64_t)(int32_t)8) ? 1 : 0));
    if ((int64_t)v0 != 0) { (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)s0, 24)))); { A3_BACK(); goto L_101fa0; } }
    goto L_1020a4;
L_101ff0:;
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)20 << 16))));
    (s2 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL));
    (s1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v0, 4294948736))));
    (s3 = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)19 << 16))));
    (v1 = (uint64_t)(uint32_t)(A1_R32B((uint32_t)s0, 0)));
L_102008:;
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 4294967040))));
    (a1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)s3, 26560)))); a3xw0 = true;
    (s2 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)s2, 1))));
    do { const a4::q128 _v = (a4m::por(a4v::setzero_si128(), a4::mk128(h3, v1))); h2 = a4::hi64(_v); std::memcpy(&v0, &_v, 8); } while (0);
    A4_W128(ADD32((uint32_t)s1, 0), a4::mk128(h2, v0));
    (a2 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)s0, 4)))); a3xw1 = true;
    (s0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)s0, 8))));
    (a3 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)s1, 12)))); a3xw2 = true;
    (t0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)s1, 8)))); a3xw3 = true;
    (t1 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)s1, 4)))); a3xw4 = true;
    { (t2 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)s1, 0)))); a3xw5 = true; A3_CHARGE(); PutString(ram, rt, f12, f13, f14, f15, f16, f17, f18, f19); }
    (v0 = (uint64_t)(((int64_t)(int64_t)s2 < (int64_t)(int32_t)16) ? 1 : 0));
    if ((int64_t)v0 != 0) { (v1 = (uint64_t)(uint32_t)(A1_R32B((uint32_t)s0, 0))); { A3_BACK(); goto L_102008; } }
    goto L_1020a4;
L_102050:;
    (s0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)s0, 128))));
    (s1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v0, 4294948736))));
    (s3 = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)19 << 16))));
    (s2 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 15))));
    (v1 = (uint64_t)(uint32_t)(A1_R32B((uint32_t)s0, 0)));
L_102068:;
    (s2 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)s2, 4294967295))));
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 4294967040))));
    (a1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)s3, 26560)))); a3xw0 = true;
    do { const a4::q128 _v = (a4m::por(a4v::setzero_si128(), a4::mk128(h3, v1))); h2 = a4::hi64(_v); std::memcpy(&v0, &_v, 8); } while (0);
    A4_W128(ADD32((uint32_t)s1, 0), a4::mk128(h2, v0));
    (a2 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)s0, 4)))); a3xw1 = true;
    (s0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)s0, 8))));
    (a3 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)s1, 12)))); a3xw2 = true;
    (t0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)s1, 8)))); a3xw3 = true;
    (t1 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)s1, 4)))); a3xw4 = true;
    { (t2 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)s1, 0)))); a3xw5 = true; A3_CHARGE(); PutString(ram, rt, f12, f13, f14, f15, f16, f17, f18, f19); }
    if ((int32_t)s2 >= 0) { (v1 = (uint64_t)(uint32_t)(A1_R32B((uint32_t)s0, 0))); { A3_BACK(); goto L_102068; } }
L_1020a4:;
    { (sp = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)sp, 64)))); if (xo) { if (a3xw0) xo[0] = a1; if (a3xw1) xo[1] = a2; if (a3xw2) xo[2] = a3; if (a3xw3) xo[3] = t0; if (a3xw4) xo[4] = t1; if (a3xw5) xo[5] = t2; if (a3xw6) xo[6] = t3; } io_h2 = h2; return v0; }
    __builtin_unreachable();
}

// iosSetDCParam (0x1bf048): typed by tools/typed_lift2.py (round A6; frame calls, extra outputs xo[0]=a0 xo[1]=a1 xo[2]=a2 xo[3]=a3 xo[4]=t0 xo[5]=t1 xo[6]=t2 xo[7]=t3; VU0 in -, out h2 h3, guest sp, frame 16 bytes, 0 slot accesses)
inline uint64_t iosSetDCParam(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint64_t a0, uint64_t a1, uint64_t a2, uint64_t a3, uint64_t t0, uint32_t sp, uint64_t &io_h2, uint64_t &io_h3, uint64_t *xo = nullptr)
{
    uint64_t v0 = 0;
    uint64_t t1 = 0;
    uint64_t t2 = 0;
    uint64_t t3 = 0;
    uint64_t h2 = io_h2;
    uint64_t h3 = io_h3;
    (v0 = (uint64_t)((uint64_t)a0 + (uint64_t)0ULL));
    A1_W16B((uint32_t)v0, 362, (uint16_t)(uint32_t)t0);
    A1_W32B((uint32_t)v0, 352, (uint32_t)a1);
    A1_W32B((uint32_t)v0, 356, (uint32_t)a2);
    A1_W16B((uint32_t)v0, 360, (uint16_t)(uint32_t)a3);
    { uint64_t a7c[8] = {0u, 0u, 0u, 0u, 0u, 0u, 0u, 0u}; const uint64_t a7_t = kernel_sub_001BEC18(ram, rt, a0, a1_sx64((uint32_t)(sp + 0x0u)), h2, h3, a7c); a0 = a7c[0]; a1 = a7c[1]; a2 = a7c[2]; a3 = a7c[3]; t0 = a7c[4]; t1 = a7c[5]; t2 = a7c[6]; t3 = a7c[7]; if (xo) { xo[0] = a0; xo[1] = a1; xo[2] = a2; xo[3] = a3; xo[4] = t0; xo[5] = t1; xo[6] = t2; xo[7] = t3; } io_h2 = h2; io_h3 = h3; return a7_t; }
    __builtin_unreachable();
}

// iosSetDCClearParam (0x1bf070): typed by tools/typed_lift2.py (round A6; frame calls, extra outputs xo[0]=a0 xo[1]=a1 xo[2]=a2 xo[3]=a3 xo[4]=t0 xo[5]=t1 xo[6]=t2 xo[7]=t3; VU0 in -, out h2 h3, guest sp, frame 16 bytes, 0 slot accesses)
inline uint64_t iosSetDCClearParam(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint64_t a0, uint64_t a1, uint64_t a2, uint64_t a3, uint64_t t0, uint64_t t1, uint32_t sp, uint64_t &io_h2, uint64_t &io_h3, uint64_t *xo = nullptr)
{
    uint64_t v0 = 0;
    uint64_t v1 = 0;
    uint64_t t2 = 0;
    uint64_t t3 = 0;
    uint64_t h2 = io_h2;
    uint64_t h3 = io_h3;
    (v1 = (uint64_t)((uint64_t)a0 + (uint64_t)0ULL));
    (t2 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 4294967294))));
    (a1 = (uint64_t)(a1 & (uint64_t)(uint16_t)1));
    A1_W8B((uint32_t)v1, 367, (uint8_t)(uint32_t)t1);
    (v0 = (uint64_t)(A1_R64B((uint32_t)v1, 368)));
    (v0 = (uint64_t)(v0 & RC_U64X(t2)));
    A1_W8B((uint32_t)v1, 364, (uint8_t)(uint32_t)a2);
    (v0 = (uint64_t)(v0 | RC_U64X(a1)));
    A1_W8B((uint32_t)v1, 365, (uint8_t)(uint32_t)a3);
    A1_W64B((uint32_t)v1, 368, v0);
    A1_W8B((uint32_t)v1, 366, (uint8_t)(uint32_t)t0);
    { uint64_t a7c[8] = {0u, 0u, 0u, 0u, 0u, 0u, 0u, 0u}; const uint64_t a7_t = kernel_sub_001BEC18(ram, rt, a0, a1_sx64((uint32_t)(sp + 0x0u)), h2, h3, a7c); a0 = a7c[0]; a1 = a7c[1]; a2 = a7c[2]; a3 = a7c[3]; t0 = a7c[4]; t1 = a7c[5]; t2 = a7c[6]; t3 = a7c[7]; if (xo) { xo[0] = a0; xo[1] = a1; xo[2] = a2; xo[3] = a3; xo[4] = t0; xo[5] = t1; xo[6] = t2; xo[7] = t3; } io_h2 = h2; io_h3 = h3; return a7_t; }
    __builtin_unreachable();
}

// manager_sub_010B8798 (0x10b8798): typed by tools/typed_lift2.py (round A6; guest frame calls, extra outputs xo[0]=a0 xo[1]=a1 xo[2]=a2 xo[3]=a3 xo[4]=t0; VU0 in vf0 vf1 vf2 vf3 vf4 vf10 vf13 vf14 vf15 vf16 vf17 vi15 vacc vstat, out vf1 vf2 vf3 vf4 vf5 vf6 vf8 vf10 vf11 vf12 vf13 vf14 vf15 vf16 vf17 vi15 vacc vq vstat, relocated calls 6)
inline a1::RetVF manager_sub_010B8798(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint64_t a0, uint64_t a1, uint64_t a2, uint64_t a3, uint64_t t0, uint64_t sp, float f0, float f1, float f2, float f3, float f4, float f5, float f6, float f7, float f8, float f9, float f10, float f11, float f12, float f13, float f14, float f15, float f16, float f17, float f18, float f19, float f20, float f21, float f22, float f23, float f24, float f25, float f26, float f27, float f28, float f29, float f30, float f31, const a4::vec4 &io_vf0, a4::vec4 &io_vf1, a4::vec4 &io_vf2, a4::vec4 &io_vf3, a4::vec4 &io_vf4, a4::vec4 &io_vf5, a4::vec4 &io_vf6, a4::vec4 &io_vf8, a4::vec4 &io_vf10, a4::vec4 &io_vf11, a4::vec4 &io_vf12, a4::vec4 &io_vf13, a4::vec4 &io_vf14, a4::vec4 &io_vf15, a4::vec4 &io_vf16, a4::vec4 &io_vf17, uint16_t &io_vi15, a4::vec4 &io_vacc, float &io_vq, uint16_t &io_vstat, uint64_t *xo = nullptr)
{
    A11_REC(0);
    uint32_t at = 0;
    uint32_t v0 = 0;
    uint64_t s0 = 0;
    uint32_t s1 = 0;
    uint64_t s2 = 0;
    uint32_t s3 = 0;
    uint64_t s4 = 0;
    uint64_t s5 = 0;
    uint64_t s6 = 0;
    uint64_t s7 = 0;
    uint64_t fp = 0;
    a4::vec4 vf0 = io_vf0;
    a4::vec4 vf1 = io_vf1;
    a4::vec4 vf2 = io_vf2;
    a4::vec4 vf3 = io_vf3;
    a4::vec4 vf4 = io_vf4;
    a4::vec4 vf5 = io_vf5;
    a4::vec4 vf6 = io_vf6;
    a4::vec4 vf8 = io_vf8;
    a4::vec4 vf10 = io_vf10;
    a4::vec4 vf11 = io_vf11;
    a4::vec4 vf12 = io_vf12;
    a4::vec4 vf13 = io_vf13;
    a4::vec4 vf14 = io_vf14;
    a4::vec4 vf15 = io_vf15;
    a4::vec4 vf16 = io_vf16;
    a4::vec4 vf17 = io_vf17;
    uint16_t vi15 = io_vi15;
    a4::vec4 vacc = io_vacc;
    float vq = io_vq;
    uint16_t vstat = io_vstat;
    (sp = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)sp, 4294967120))));
    (s2 = (uint64_t)((uint64_t)a0 + (uint64_t)0ULL));
    (v0 = (uint32_t)((int32_t)((int32_t)SLL32((uint32_t)s2, 6))));
    (s0 = (uint64_t)((int64_t)(int32_t)((int32_t)SLL32((uint32_t)s2, 4))));
    (s5 = (uint64_t)((uint64_t)t0 + (uint64_t)0ULL));
    (s6 = (uint64_t)((uint64_t)a1 + (uint64_t)0ULL));
    (s7 = (uint64_t)((uint64_t)a3 + (uint64_t)0ULL));
    (fp = (uint64_t)((uint64_t)a2 + (uint64_t)0ULL));
    (s0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)s0, (uint32_t)s7))));
    (s1 = (uint32_t)((int32_t)((int32_t)A1_R32B((uint32_t)fp, 8))));
    s1 = s1 + v0;
    v0 = A1_R32B(s1, 52);
    s3 = s1 + 0x10u;
    { A1_W32B((uint32_t)sp, 80, v0); A3_CHARGE(); a11x::pushMatrix(ram, rt, vf1, vf2, vf3, vf4, vi15); }
    (a0 = (uint64_t)((uint64_t)s0 + (uint64_t)0ULL));
    { (s4 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B(s1, 48)))); A3_CHARGE(); v0 = MultiCurrentQuaternion(ram, rt, (uint32_t)a0, vf11, vf12, vf13, vf14, vf15, vf16, vacc); }
    vf14 = a4v::castsi128_ps(A4_R128(ADD32((uint32_t)s0, 0)));
    { a4::vec4 res = a4v::mul(vf14, vf14); a4::q128 mask = a4v::set_epi32(-1, -1, -1, -1); vf15 = a4v::blend(vf15, res, a4v::castsi128_ps(mask)); }
    { a4::vec4 res = a4v::addK<1, 1>(vf15, a4v::bcast<1>(vf15)); a4::q128 mask = a4v::set_epi32(0, 0, 0, -1); vf15 = a4v::blendv_ps(vf15, res, a4v::castsi128_ps(mask)); }
    { a4::vec4 res = a4v::addK<1, 1>(vf15, a4v::bcast<2>(vf15)); a4::q128 mask = a4v::set_epi32(0, 0, 0, -1); vf15 = a4v::blendv_ps(vf15, res, a4v::castsi128_ps(mask)); }
    { a4::vec4 res = a4v::addK<1, 1>(vf15, a4v::bcast<3>(vf15)); a4::q128 mask = a4v::set_epi32(0, 0, 0, -1); vf15 = a4v::blendv_ps(vf15, res, a4v::castsi128_ps(mask)); }
    { float fs = a4v::cvtss_f32(a4v::shuffle_ps<0x03>(vf0, vf0)); float ft = a4v::cvtss_f32(a4v::bcast<0>(vf15)); vq = a4v::rsqrt(vstat, fs, ft); }
    { a4::vec4 res = a4v::mul(vf14, a4v::set1_ps(vq)); a4::q128 mask = a4v::set_epi32(-1, -1, -1, -1); vf15 = a4v::blendv_ps(vf15, res, a4v::castsi128_ps(mask)); }
    A4_W128(ADD32((uint32_t)s0, 0), a4v::castps_si128(vf15));
    { a4::vec4 res = a4v::addK<1, 1>(vf0, a4v::bcast<3>(vf0)); a4::q128 mask = a4v::set_epi32(0, -1, -1, -1); vf12 = a4v::blendv_ps(vf12, res, a4v::castsi128_ps(mask)); }
    vf11 = a4v::castsi128_ps(A4_R128(ADD32((uint32_t)s0, 0)));
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
    A4_W128(ADD32((uint32_t)sp, 0), a4v::castps_si128(vf14));
    A4_W128(ADD32((uint32_t)sp, 16), a4v::castps_si128(vf15));
    A4_W128(ADD32((uint32_t)sp, 32), a4v::castps_si128(vf16));
    (v0 = (uint32_t)((int32_t)((int32_t)ADD32((uint32_t)sp, 48))));
    A4_W128(ADD32(v0, 0), a4v::castps_si128(vf0));
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)sp, 64))));
    vf5 = a4v::castsi128_ps(A4_R128(ADD32(s3, 0)));
    vf6 = a4v::castsi128_ps(A4_R128(ADD32((uint32_t)s5, 0)));
    { a4::vec4 res = a4v::mul(vf5, vf6); a4::q128 mask = a4v::set_epi32(0, -1, -1, -1); vf5 = a4v::blend(vf5, res, a4v::castsi128_ps(mask)); }
    A4_W128(ADD32((uint32_t)a0, 0), a4v::castps_si128(vf5));
    (at = (uint32_t)((int32_t)((int32_t)((uint32_t)16256 << 16))));
    { uint32_t bits = a7_opq(at); std::memcpy(&f0, &bits, sizeof(bits)); }
    (s2 = (uint64_t)((int64_t)(int32_t)((int32_t)SLL32((uint32_t)s2, 6))));
    (s0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 4294967295))));
    { { float f = f0; uint32_t bits; std::memcpy(&bits, &f, sizeof(bits)); A1_W32B((uint32_t)sp, 76, bits); } A3_CHARGE(); iosTransCurrentMatrix(ram, rt, (uint32_t)a0, vf0, vf1, vf2, vf3, vf4, vf8, vacc); }
    { (a0 = (uint64_t)((uint64_t)sp + (uint64_t)0ULL)); A3_CHARGE(); iosMulCurrentMatrixR(ram, rt, (uint32_t)a0, vf1, vf2, vf3, vf4, vf14, vf15, vf16, vf17, vacc); }
    { (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)s6, (uint32_t)s2)))); A3_CHARGE(); iosGetCurrentMatrix(ram, rt, (uint32_t)a0, vf1, vf2, vf3, vf4); }
    (a0 = (uint64_t)((uint64_t)s4 + (uint64_t)0ULL));
    (a1 = (uint64_t)((uint64_t)s6 + (uint64_t)0ULL));
    (a2 = (uint64_t)((uint64_t)fp + (uint64_t)0ULL));
    (a3 = (uint64_t)((uint64_t)s7 + (uint64_t)0ULL));
    { const bool c = (int64_t)s4 == (int64_t)s0; (t0 = (uint64_t)((uint64_t)s5 + (uint64_t)0ULL)); if (c) goto L_10b88fc; }
    { A3_CHARGE(); uint64_t a7c[5] = {0u, 0u, 0u, 0u, 0u}; { const a1::RetVF a2_r = manager_sub_010B8798(ram, rt, a0, a1, a2, a3, t0, sp, f0, f1, f2, f3, f4, f5, f6, f7, f8, f9, f10, f11, f12, f13, f14, f15, f16, f17, f18, f19, f20, f21, f22, f23, f24, f25, f26, f27, f28, f29, f30, f31, vf0, vf1, vf2, vf3, vf4, vf5, vf6, vf8, vf10, vf11, vf12, vf13, vf14, vf15, vf16, vf17, vi15, vacc, vq, vstat, a7c); v0 = (uint32_t)a2_r.v0; f0 = a2_r.f0; } a1 = a7c[1]; a2 = a7c[2]; a3 = a7c[3]; t0 = a7c[4]; }
L_10b88fc:;
    { A3_CHARGE(); a11x::popMatrix(ram, rt, vf1, vf2, vf3, vf4, vi15); }
    (v0 = (uint32_t)((int32_t)((int32_t)A1_R32B((uint32_t)sp, 80))));
    { const bool c = (int64_t)(int32_t)v0 == (int64_t)s0; (a0 = (uint64_t)((uint64_t)a1_sx64(v0) + (uint64_t)0ULL)); if (c) goto L_10b8924; }
    (a1 = (uint64_t)((uint64_t)s6 + (uint64_t)0ULL));
    (a2 = (uint64_t)((uint64_t)fp + (uint64_t)0ULL));
    (a3 = (uint64_t)((uint64_t)s7 + (uint64_t)0ULL));
    { (t0 = (uint64_t)((uint64_t)s5 + (uint64_t)0ULL)); A3_CHARGE(); uint64_t a7c[5] = {0u, 0u, 0u, 0u, 0u}; { const a1::RetVF a2_r = manager_sub_010B8798(ram, rt, a0, a1, a2, a3, t0, sp, f0, f1, f2, f3, f4, f5, f6, f7, f8, f9, f10, f11, f12, f13, f14, f15, f16, f17, f18, f19, f20, f21, f22, f23, f24, f25, f26, f27, f28, f29, f30, f31, vf0, vf1, vf2, vf3, vf4, vf5, vf6, vf8, vf10, vf11, vf12, vf13, vf14, vf15, vf16, vf17, vi15, vacc, vq, vstat, a7c); v0 = (uint32_t)a2_r.v0; f0 = a2_r.f0; } a0 = a7c[0]; a1 = a7c[1]; a2 = a7c[2]; a3 = a7c[3]; t0 = a7c[4]; }
L_10b8924:;
    { (sp = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)sp, 176)))); if (xo) { xo[0] = a0; xo[1] = a1; xo[2] = a2; xo[3] = a3; xo[4] = t0; } io_vf1 = vf1; io_vf2 = vf2; io_vf3 = vf3; io_vf4 = vf4; io_vf5 = vf5; io_vf6 = vf6; io_vf8 = vf8; io_vf10 = vf10; io_vf11 = vf11; io_vf12 = vf12; io_vf13 = vf13; io_vf14 = vf14; io_vf15 = vf15; io_vf16 = vf16; io_vf17 = vf17; io_vi15 = vi15; io_vacc = vacc; io_vq = vq; io_vstat = vstat; return a1::RetVF{(uint64_t)a1_sx64(v0), f0}; }
    __builtin_unreachable();
}

// manager_sub_010B8958 (0x10b8958): typed by tools/typed_lift2.py (round A6; guest frame calls, extra outputs xo[0]=f0; VU0 in vf0 vf1 vf2 vf3 vf4 vf10 vf12 vf13 vf14 vf15 vf16 vf17 vi15 vacc, out vf1 vf2 vf3 vf4 vf8 vf10 vf11 vf12 vf13 vf14 vf15 vf16 vf17 vi15 vacc h2, relocated calls 5)
inline uint64_t manager_sub_010B8958(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint64_t a0, uint64_t a1, uint64_t a2, uint64_t a3, uint64_t sp, float f0, float f1, float f2, float f3, float f4, float f5, float f6, float f7, float f8, float f9, float f10, float f11, float f12, float f13, float f14, float f15, float f16, float f17, float f18, float f19, float f20, float f21, float f22, float f23, float f24, float f25, float f26, float f27, float f28, float f29, float f30, float f31, const a4::vec4 &io_vf0, a4::vec4 &io_vf1, a4::vec4 &io_vf2, a4::vec4 &io_vf3, a4::vec4 &io_vf4, a4::vec4 &io_vf8, a4::vec4 &io_vf10, a4::vec4 &io_vf11, a4::vec4 &io_vf12, a4::vec4 &io_vf13, a4::vec4 &io_vf14, a4::vec4 &io_vf15, a4::vec4 &io_vf16, a4::vec4 &io_vf17, uint16_t &io_vi15, a4::vec4 &io_vacc, uint64_t &io_h2, uint64_t *xo = nullptr)
{
    A11_REC(1);
    uint64_t v0 = 0;
    uint64_t v1 = 0;
    uint64_t s0 = 0;
    uint64_t s1 = 0;
    uint64_t s2 = 0;
    uint64_t s3 = 0;
    uint64_t s4 = 0;
    uint64_t s5 = 0;
    uint64_t s6 = 0;
    a4::vec4 vf0 = io_vf0;
    a4::vec4 vf1 = io_vf1;
    a4::vec4 vf2 = io_vf2;
    a4::vec4 vf3 = io_vf3;
    a4::vec4 vf4 = io_vf4;
    a4::vec4 vf8 = io_vf8;
    a4::vec4 vf10 = io_vf10;
    a4::vec4 vf11 = io_vf11;
    a4::vec4 vf12 = io_vf12;
    a4::vec4 vf13 = io_vf13;
    a4::vec4 vf14 = io_vf14;
    a4::vec4 vf15 = io_vf15;
    a4::vec4 vf16 = io_vf16;
    a4::vec4 vf17 = io_vf17;
    uint16_t vi15 = io_vi15;
    a4::vec4 vacc = io_vacc;
    uint64_t h2 = io_h2;
    bool a3xw0 = false;
    (sp = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)sp, 4294967168))));
    (s1 = (uint64_t)((uint64_t)a0 + (uint64_t)0ULL));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)SLL32((uint32_t)s1, 6))));
    (s3 = (uint64_t)((uint64_t)a1 + (uint64_t)0ULL));
    (s4 = (uint64_t)((uint64_t)a3 + (uint64_t)0ULL));
    (s5 = (uint64_t)((uint64_t)a2 + (uint64_t)0ULL));
    (s0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)s5, 8))));
    (s0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)s0, (uint32_t)v0))));
    { (s6 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)s0, 52)))); A3_CHARGE(); a11x::pushMatrix(ram, rt, vf1, vf2, vf3, vf4, vi15); }
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)s0, 16))));
    { (s2 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)s0, 48)))); A3_CHARGE(); iosTransCurrentMatrix(ram, rt, (uint32_t)a0, vf0, vf1, vf2, vf3, vf4, vf8, vacc); }
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)SLL32((uint32_t)s1, 4))));
    (v1 = (uint64_t)((uint64_t)sp + (uint64_t)0ULL));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v0, (uint32_t)s4))));
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
    A4_W128(ADD32((uint32_t)v1, 0), a4v::castps_si128(vf14));
    A4_W128(ADD32((uint32_t)v1, 16), a4v::castps_si128(vf15));
    A4_W128(ADD32((uint32_t)v1, 32), a4v::castps_si128(vf16));
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v1, 48))));
    A4_W128(ADD32((uint32_t)v1, 0), a4v::castps_si128(vf0));
    (a0 = (uint64_t)((uint64_t)sp + (uint64_t)0ULL));
    { (s1 = (uint64_t)((int64_t)(int32_t)((int32_t)SLL32((uint32_t)s1, 4)))); A3_CHARGE(); iosMulCurrentMatrixR(ram, rt, (uint32_t)a0, vf1, vf2, vf3, vf4, vf14, vf15, vf16, vf17, vacc); }
    (s1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)s1, (uint32_t)s3))));
    (a0 = (uint64_t)((uint64_t)sp + (uint64_t)0ULL));
    { (s0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 4294967295)))); A3_CHARGE(); iosGetCurrentMatrix(ram, rt, (uint32_t)a0, vf1, vf2, vf3, vf4); }
    (a0 = (uint64_t)((uint64_t)s2 + (uint64_t)0ULL));
    do { const a4::q128 _v = (A4_R128(ADD32((uint32_t)sp, 48))); h2 = a4::hi64(_v); std::memcpy(&v0, &_v, 8); } while (0);
    (a1 = (uint64_t)((uint64_t)s3 + (uint64_t)0ULL));
    (a2 = (uint64_t)((uint64_t)s5 + (uint64_t)0ULL));
    (a3 = (uint64_t)((uint64_t)s4 + (uint64_t)0ULL));
    { const bool c = (int64_t)s2 == (int64_t)s0; A4_W128(ADD32((uint32_t)s1, 0), a4::mk128(h2, v0)); if (c) goto L_10b8a6c; }
    { A3_CHARGE(); uint64_t a7c[1] = {(uint64_t)a1_bits(f0)}; v0 = manager_sub_010B8958(ram, rt, a0, a1, a2, a3, sp, f0, f1, f2, f3, f4, f5, f6, f7, f8, f9, f10, f11, f12, f13, f14, f15, f16, f17, f18, f19, f20, f21, f22, f23, f24, f25, f26, f27, f28, f29, f30, f31, vf0, vf1, vf2, vf3, vf4, vf8, vf10, vf11, vf12, vf13, vf14, vf15, vf16, vf17, vi15, vacc, h2, a7c); f0 = a1_flt((uint32_t)a7c[0]); a3xw0 = true; }
L_10b8a6c:;
    { A3_CHARGE(); a11x::popMatrix(ram, rt, vf1, vf2, vf3, vf4, vi15); }
    { const bool c = (int64_t)s6 == (int64_t)s0; (a0 = (uint64_t)((uint64_t)s6 + (uint64_t)0ULL)); if (c) goto L_10b8a8c; }
    (a1 = (uint64_t)((uint64_t)s3 + (uint64_t)0ULL));
    (a2 = (uint64_t)((uint64_t)s5 + (uint64_t)0ULL));
    { (a3 = (uint64_t)((uint64_t)s4 + (uint64_t)0ULL)); A3_CHARGE(); uint64_t a7c[1] = {(uint64_t)a1_bits(f0)}; v0 = manager_sub_010B8958(ram, rt, a0, a1, a2, a3, sp, f0, f1, f2, f3, f4, f5, f6, f7, f8, f9, f10, f11, f12, f13, f14, f15, f16, f17, f18, f19, f20, f21, f22, f23, f24, f25, f26, f27, f28, f29, f30, f31, vf0, vf1, vf2, vf3, vf4, vf8, vf10, vf11, vf12, vf13, vf14, vf15, vf16, vf17, vi15, vacc, h2, a7c); f0 = a1_flt((uint32_t)a7c[0]); a3xw0 = true; }
L_10b8a8c:;
    { (sp = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)sp, 128)))); if (xo) { if (a3xw0) xo[0] = a1_bits(f0); } io_vf1 = vf1; io_vf2 = vf2; io_vf3 = vf3; io_vf4 = vf4; io_vf8 = vf8; io_vf10 = vf10; io_vf11 = vf11; io_vf12 = vf12; io_vf13 = vf13; io_vf14 = vf14; io_vf15 = vf15; io_vf16 = vf16; io_vf17 = vf17; io_vi15 = vi15; io_vacc = vacc; io_h2 = h2; return v0; }
    __builtin_unreachable();
}

// GetCharacterClassMotionInterpNodeLastPosition (0x10c2558): typed by tools/typed_lift2.py (round A6; guest frame calls, extra outputs xo[0]=t0 xo[1]=t1 xo[2]=t2 xo[3]=t3; VU0 in vf0 vf25 vf28 vf29 vacc, out vf5 vf6 vf24 vf25 vf26 vf28 vf29 vacc h2 h4 h8 h9 h10 h11 h12 h13 h14 h15)
inline uint64_t GetCharacterClassMotionInterpNodeLastPosition(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint64_t a0, uint64_t a1, uint64_t a2, uint64_t sp, float f12, float f13, float f14, float f15, float f16, float f17, float f18, float f19, const a4::vec4 &io_vf0, a4::vec4 &io_vf5, a4::vec4 &io_vf6, a4::vec4 &io_vf24, a4::vec4 &io_vf25, a4::vec4 &io_vf26, a4::vec4 &io_vf28, a4::vec4 &io_vf29, a4::vec4 &io_vacc, uint64_t &io_h2, uint64_t &io_h4, uint64_t &io_h8, uint64_t &io_h9, uint64_t &io_h10, uint64_t &io_h11, uint64_t &io_h12, uint64_t &io_h13, uint64_t &io_h14, uint64_t &io_h15, uint64_t *xo = nullptr)
{
    uint64_t v0 = 0;
    uint64_t v1 = 0;
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
    a4::vec4 vf0 = io_vf0;
    a4::vec4 vf5 = io_vf5;
    a4::vec4 vf6 = io_vf6;
    a4::vec4 vf24 = io_vf24;
    a4::vec4 vf25 = io_vf25;
    a4::vec4 vf26 = io_vf26;
    a4::vec4 vf28 = io_vf28;
    a4::vec4 vf29 = io_vf29;
    a4::vec4 vacc = io_vacc;
    uint64_t h2 = io_h2;
    uint64_t h4 = io_h4;
    uint64_t h8 = io_h8;
    uint64_t h9 = io_h9;
    uint64_t h10 = io_h10;
    uint64_t h11 = io_h11;
    uint64_t h12 = io_h12;
    uint64_t h13 = io_h13;
    uint64_t h14 = io_h14;
    uint64_t h15 = io_h15;
    bool a3xw0 = false;
    bool a3xw1 = false;
    bool a3xw2 = false;
    bool a3xw3 = false;
    (sp = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)sp, 4294967200))));
    (s0 = (uint64_t)((uint64_t)a1 + (uint64_t)0ULL));
    (s2 = (uint64_t)((uint64_t)a0 + (uint64_t)0ULL));
    if ((int32_t)a2 <= 0) goto L_10c25e8;
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)1 << 16))));
    (v0 = (uint64_t)(a2 & RC_U64X(v0)));
    { const bool c = (int64_t)v0 == 0; (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 4294967294)))); if (c) goto L_10c25ec; }
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)65534 << 16))));
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)s0, 856))));
    (v0 = (uint64_t)(RC_U64X(v0) | (uint64_t)(uint16_t)65535));
    (s1 = (uint64_t)(a2 & RC_U64X(v0)));
    { (a1 = (uint64_t)((uint64_t)s1 + (uint64_t)0ULL)); A3_CHARGE(); v0 = a1_sx64(GetMotionOptionNodeParentId(ram, rt, (uint32_t)a0, (uint32_t)a1)); }
    { const bool c = (int32_t)v0 >= 0; (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)s0, 848)))); if (c) goto L_10c25d0; }
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)SLL32((uint32_t)s1, 4))));
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)s0, 312))));
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v1, (uint32_t)v0))));
    vf5 = a4v::castsi128_ps(A4_R128(ADD32((uint32_t)a0, 0)));
    vf6 = a4v::castsi128_ps(A4_R128(ADD32((uint32_t)v1, 0)));
    { a4::vec4 res = a4v::add(vf5, vf6); a4::q128 mask = a4v::set_epi32(0, -1, -1, -1); vf5 = a4v::blend(vf5, res, a4v::castsi128_ps(mask)); }
    A4_W128(ADD32((uint32_t)s2, 0), a4v::castps_si128(vf5));
    goto L_10c2698;
L_10c25d0:;
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)SLL32((uint32_t)s1, 4))));
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v1, (uint32_t)v0))));
    do { const a4::q128 _v = (A4_R128(ADD32((uint32_t)v1, 0))); h4 = a4::hi64(_v); std::memcpy(&a0, &_v, 8); } while (0);
    A4_W128(ADD32((uint32_t)s2, 0), a4::mk128(h4, a0));
    goto L_10c2694;
L_10c25e8:;
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 4294967294))));
L_10c25ec:;
    if ((int64_t)a2 == (int64_t)v0) { (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)s0, 312)))); goto L_10c2608; }
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 4294967295))));
    if ((int64_t)a2 == (int64_t)v0) { do { const a4::q128 _v = (A4_R128(ADD32((uint32_t)s0, 1120))); h2 = a4::hi64(_v); std::memcpy(&v0, &_v, 8); } while (0); goto L_10c2690; }
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)s0, 360))));
    goto L_10c2610;
L_10c2608:;
    do { const a4::q128 _v = (A4_R128(ADD32((uint32_t)v1, 0))); h2 = a4::hi64(_v); std::memcpy(&v0, &_v, 8); } while (0);
    goto L_10c2690;
L_10c2610:;
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)SLL32((uint32_t)a2, 6))));
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v1, (uint32_t)v0))));
    vf24 = a4v::castsi128_ps(A4_R128(ADD32((uint32_t)v1, 48)));
    { a4::vec4 res = a4v::subK<1, 1>(vf0, a4v::bcast<3>(vf0)); a4::q128 mask = a4v::set_epi32(0, 0, 0, -1); vf25 = a4v::blendv_ps(vf25, res, a4v::castsi128_ps(mask)); }
    { a4::vec4 res = a4v::mulK<0, 1>(vf24, a4v::bcast<0>(vf25)); a4::q128 mask = a4v::set_epi32(0, -1, -1, -1); vf28 = a4v::blendv_ps(vf28, res, a4v::castsi128_ps(mask)); }
    { a4::vec4 res = a4v::sub(vf28, vf28); a4::q128 mask = a4v::set_epi32(-1, 0, 0, 0); vf28 = a4v::blend(vf28, res, a4v::castsi128_ps(mask)); }
    do { const a4::q128 _v = (A4_R128(ADD32((uint32_t)v1, 0))); h8 = a4::hi64(_v); std::memcpy(&t0, &_v, 8); } while (0); a3xw0 = true;
    do { const a4::q128 _v = (A4_R128(ADD32((uint32_t)v1, 16))); h9 = a4::hi64(_v); std::memcpy(&t1, &_v, 8); } while (0); a3xw1 = true;
    do { const a4::q128 _v = (A4_R128(ADD32((uint32_t)v1, 32))); h10 = a4::hi64(_v); std::memcpy(&t2, &_v, 8); } while (0); a3xw2 = true;
    do { const a4::q128 _v = (a4v::castps_si128(vf0)); h11 = a4::hi64(_v); std::memcpy(&t3, &_v, 8); } while (0); a3xw3 = true;
    do { const a4::q128 _v = (a4m::pextlw(a4::mk128(h9, t1), a4::mk128(h8, t0))); h12 = a4::hi64(_v); std::memcpy(&t4, &_v, 8); } while (0);
    do { const a4::q128 _v = (a4m::pextuw(a4::mk128(h9, t1), a4::mk128(h8, t0))); h13 = a4::hi64(_v); std::memcpy(&t5, &_v, 8); } while (0);
    do { const a4::q128 _v = (a4m::pextlw(a4::mk128(h11, t3), a4::mk128(h10, t2))); h14 = a4::hi64(_v); std::memcpy(&t6, &_v, 8); } while (0);
    do { const a4::q128 _v = (a4m::pextuw(a4::mk128(h11, t3), a4::mk128(h10, t2))); h15 = a4::hi64(_v); std::memcpy(&t7, &_v, 8); } while (0);
    do { const a4::q128 _v = (a4m::pcpyld(a4::mk128(h14, t6), a4::mk128(h12, t4))); h8 = a4::hi64(_v); std::memcpy(&t0, &_v, 8); } while (0); a3xw0 = true;
    do { const a4::q128 _v = (a4v::unpackhi_epi64(a4::mk128(h12, t4), a4::mk128(h14, t6))); h9 = a4::hi64(_v); std::memcpy(&t1, &_v, 8); } while (0); a3xw1 = true;
    do { const a4::q128 _v = (a4m::pcpyld(a4::mk128(h15, t7), a4::mk128(h13, t5))); h10 = a4::hi64(_v); std::memcpy(&t2, &_v, 8); } while (0); a3xw2 = true;
    do { const a4::q128 _v = (a4v::unpackhi_epi64(a4::mk128(h13, t5), a4::mk128(h15, t7))); h11 = a4::hi64(_v); std::memcpy(&t3, &_v, 8); } while (0); a3xw3 = true;
    vf24 = a4v::castsi128_ps(a4::mk128(h8, t0));
    vf25 = a4v::castsi128_ps(a4::mk128(h9, t1));
    vf26 = a4v::castsi128_ps(a4::mk128(h10, t2));
    { a4::vec4 res = a4v::mulK<0, 1>(vf24, a4v::bcast<0>(vf28)); vacc = a4v::dest<1, 1, 1, 1>(vacc, res); }
    { a4::vec4 mul_res = a4v::mulK<0, 1>(vf25, a4v::bcast<1>(vf28)); a4::vec4 res = a4v::addK<1, 1>(vacc, mul_res); vacc = a4v::dest<1, 1, 1, 1>(vacc, res); }
    { a4::vec4 mul_res = a4v::mulK<0, 1>(vf26, a4v::bcast<2>(vf28)); a4::vec4 res = a4v::addK<1, 1>(vacc, mul_res); vacc = a4v::dest<1, 1, 1, 1>(vacc, res); }
    { a4::vec4 mul_res = a4v::mulK<1, 1>(vf0, a4v::bcast<3>(vf28)); a4::vec4 res = a4v::addK<1, 1>(vacc, mul_res); a4::q128 mask = a4v::set_epi32(-1, -1, -1, -1); vf29 = a4v::blendv_ps(vf29, res, a4v::castsi128_ps(mask)); }
    { a4::vec4 res = a4v::subK<1, 1>(vf29, vf29); a4::q128 mask = a4v::set_epi32(-1, 0, 0, 0); vf29 = a4v::blend(vf29, res, a4v::castsi128_ps(mask)); }
    { a4::vec4 res = a4v::addK<1, 1>(vf29, vf0); a4::q128 mask = a4v::set_epi32(-1, 0, 0, 0); vf29 = a4v::blend(vf29, res, a4v::castsi128_ps(mask)); }
    A4_W128(ADD32((uint32_t)sp, 0), a4v::castps_si128(vf24));
    A4_W128(ADD32((uint32_t)sp, 16), a4v::castps_si128(vf25));
    A4_W128(ADD32((uint32_t)sp, 32), a4v::castps_si128(vf26));
    A4_W128(ADD32((uint32_t)sp, 48), a4v::castps_si128(vf29));
    do { const a4::q128 _v = (A4_R128(ADD32((uint32_t)sp, 48))); h2 = a4::hi64(_v); std::memcpy(&v0, &_v, 8); } while (0);
L_10c2690:;
    A4_W128(ADD32((uint32_t)s2, 0), a4::mk128(h2, v0));
L_10c2694:;
L_10c2698:;
    { (sp = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)sp, 96)))); if (xo) { if (a3xw0) xo[0] = t0; if (a3xw1) xo[1] = t1; if (a3xw2) xo[2] = t2; if (a3xw3) xo[3] = t3; } io_vf5 = vf5; io_vf6 = vf6; io_vf24 = vf24; io_vf25 = vf25; io_vf26 = vf26; io_vf28 = vf28; io_vf29 = vf29; io_vacc = vacc; io_h2 = h2; io_h4 = h4; io_h8 = h8; io_h9 = h9; io_h10 = h10; io_h11 = h11; io_h12 = h12; io_h13 = h13; io_h14 = h14; io_h15 = h15; return v0; }
    __builtin_unreachable();
}

// manager_sub_010C38E8 (0x10c38e8): typed by tools/typed_lift2.py (round A6; guest frame calls; VU0 in vf0 vf1 vf2 vf3 vf4 vf6 vf7 vf10 vf12 vf13 vf14 vf15 vf16 vf17 vf25 vf28 vf29 vi15 vacc vstat, out vf1 vf2 vf3 vf4 vf5 vf6 vf7 vf8 vf10 vf11 vf12 vf13 vf14 vf15 vf16 vf17 vf24 vf25 vf26 vf28 vf29 vi15 vacc vq vstat h2 h8 h9 h10 h11 h12 h13 h14 h15, relocated calls 8)
inline a1::RetVF manager_sub_010C38E8(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint64_t a0, uint64_t a1, uint64_t a2, uint64_t a3, uint64_t t0, uint64_t t1, uint64_t sp, float f0, float f1, float f2, float f3, float f4, float f5, float f6, float f7, float f8, float f9, float f10, float f11, float f12, float f13, float f14, float f15, float f16, float f17, float f18, float f19, float f20, float f21, float f22, float f23, float f24, float f25, float f26, float f27, float f28, float f29, float f30, float f31, const a4::vec4 &io_vf0, a4::vec4 &io_vf1, a4::vec4 &io_vf2, a4::vec4 &io_vf3, a4::vec4 &io_vf4, a4::vec4 &io_vf5, a4::vec4 &io_vf6, a4::vec4 &io_vf7, a4::vec4 &io_vf8, a4::vec4 &io_vf10, a4::vec4 &io_vf11, a4::vec4 &io_vf12, a4::vec4 &io_vf13, a4::vec4 &io_vf14, a4::vec4 &io_vf15, a4::vec4 &io_vf16, a4::vec4 &io_vf17, a4::vec4 &io_vf24, a4::vec4 &io_vf25, a4::vec4 &io_vf26, a4::vec4 &io_vf28, a4::vec4 &io_vf29, uint16_t &io_vi15, a4::vec4 &io_vacc, float &io_vq, uint16_t &io_vstat, uint64_t &io_h2, uint64_t &io_h8, uint64_t &io_h9, uint64_t &io_h10, uint64_t &io_h11, uint64_t &io_h12, uint64_t &io_h13, uint64_t &io_h14, uint64_t &io_h15)
{
    A11_REC(2);
    uint64_t v0 = 0;
    uint64_t v1 = 0;
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
    uint64_t fp = 0;
    a4::vec4 vf0 = io_vf0;
    a4::vec4 vf1 = io_vf1;
    a4::vec4 vf2 = io_vf2;
    a4::vec4 vf3 = io_vf3;
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
    a4::vec4 vf24 = io_vf24;
    a4::vec4 vf25 = io_vf25;
    a4::vec4 vf26 = io_vf26;
    a4::vec4 vf28 = io_vf28;
    a4::vec4 vf29 = io_vf29;
    uint16_t vi15 = io_vi15;
    a4::vec4 vacc = io_vacc;
    float vq = io_vq;
    uint16_t vstat = io_vstat;
    uint64_t h2 = io_h2;
    uint64_t h8 = io_h8;
    uint64_t h9 = io_h9;
    uint64_t h10 = io_h10;
    uint64_t h11 = io_h11;
    uint64_t h12 = io_h12;
    uint64_t h13 = io_h13;
    uint64_t h14 = io_h14;
    uint64_t h15 = io_h15;
    (sp = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)sp, 4294967008))));
    (s1 = (uint64_t)((uint64_t)t1 + (uint64_t)0ULL));
    (s0 = (uint64_t)((int64_t)(int32_t)((int32_t)SLL32((uint32_t)s1, 6))));
    (s4 = (uint64_t)((uint64_t)a3 + (uint64_t)0ULL));
    (s5 = (uint64_t)((uint64_t)a0 + (uint64_t)0ULL));
    (s6 = (uint64_t)((uint64_t)a1 + (uint64_t)0ULL));
    (s7 = (uint64_t)((uint64_t)a2 + (uint64_t)0ULL));
    (fp = (uint64_t)((uint64_t)t0 + (uint64_t)0ULL));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)s4, 8))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)s0, (uint32_t)v0))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)v0, 52))));
    { A1_W32B((uint32_t)sp, 192, (uint32_t)v0); A3_CHARGE(); a11x::pushMatrix(ram, rt, vf1, vf2, vf3, vf4, vi15); }
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)s4, 8))));
    (s0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)s0, (uint32_t)v0))));
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)s0, 16))));
    { (s2 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)s0, 48)))); A3_CHARGE(); iosTransCurrentMatrix(ram, rt, (uint32_t)a0, vf0, vf1, vf2, vf3, vf4, vf8, vacc); }
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)SLL32((uint32_t)s1, 4))));
    (v1 = (uint64_t)((uint64_t)sp + (uint64_t)0ULL));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v0, (uint32_t)fp))));
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
    A4_W128(ADD32((uint32_t)v1, 0), a4v::castps_si128(vf14));
    A4_W128(ADD32((uint32_t)v1, 16), a4v::castps_si128(vf15));
    A4_W128(ADD32((uint32_t)v1, 32), a4v::castps_si128(vf16));
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v1, 48))));
    A4_W128(ADD32((uint32_t)v1, 0), a4v::castps_si128(vf0));
    { (a0 = (uint64_t)((uint64_t)sp + (uint64_t)0ULL)); A3_CHARGE(); iosMulCurrentMatrixR(ram, rt, (uint32_t)a0, vf1, vf2, vf3, vf4, vf14, vf15, vf16, vf17, vacc); }
    { (a0 = (uint64_t)((uint64_t)sp + (uint64_t)0ULL)); A3_CHARGE(); iosGetCurrentMatrix(ram, rt, (uint32_t)a0, vf1, vf2, vf3, vf4); }
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)sp, 64))));
    vf24 = a4v::castsi128_ps(A4_R128(ADD32((uint32_t)sp, 48)));
    { a4::vec4 res = a4v::subK<1, 1>(vf0, a4v::bcast<3>(vf0)); a4::q128 mask = a4v::set_epi32(0, 0, 0, -1); vf25 = a4v::blendv_ps(vf25, res, a4v::castsi128_ps(mask)); }
    { a4::vec4 res = a4v::mulK<0, 1>(vf24, a4v::bcast<0>(vf25)); a4::q128 mask = a4v::set_epi32(0, -1, -1, -1); vf28 = a4v::blendv_ps(vf28, res, a4v::castsi128_ps(mask)); }
    { a4::vec4 res = a4v::sub(vf28, vf28); a4::q128 mask = a4v::set_epi32(-1, 0, 0, 0); vf28 = a4v::blend(vf28, res, a4v::castsi128_ps(mask)); }
    do { const a4::q128 _v = (A4_R128(ADD32((uint32_t)sp, 0))); h8 = a4::hi64(_v); std::memcpy(&t0, &_v, 8); } while (0);
    do { const a4::q128 _v = (A4_R128(ADD32((uint32_t)sp, 16))); h9 = a4::hi64(_v); std::memcpy(&t1, &_v, 8); } while (0);
    do { const a4::q128 _v = (A4_R128(ADD32((uint32_t)sp, 32))); h10 = a4::hi64(_v); std::memcpy(&t2, &_v, 8); } while (0);
    do { const a4::q128 _v = (a4v::castps_si128(vf0)); h11 = a4::hi64(_v); std::memcpy(&t3, &_v, 8); } while (0);
    do { const a4::q128 _v = (a4m::pextlw(a4::mk128(h9, t1), a4::mk128(h8, t0))); h12 = a4::hi64(_v); std::memcpy(&t4, &_v, 8); } while (0);
    do { const a4::q128 _v = (a4m::pextuw(a4::mk128(h9, t1), a4::mk128(h8, t0))); h13 = a4::hi64(_v); std::memcpy(&t5, &_v, 8); } while (0);
    do { const a4::q128 _v = (a4m::pextlw(a4::mk128(h11, t3), a4::mk128(h10, t2))); h14 = a4::hi64(_v); std::memcpy(&t6, &_v, 8); } while (0);
    do { const a4::q128 _v = (a4m::pextuw(a4::mk128(h11, t3), a4::mk128(h10, t2))); h15 = a4::hi64(_v); std::memcpy(&t7, &_v, 8); } while (0);
    do { const a4::q128 _v = (a4m::pcpyld(a4::mk128(h14, t6), a4::mk128(h12, t4))); h8 = a4::hi64(_v); std::memcpy(&t0, &_v, 8); } while (0);
    do { const a4::q128 _v = (a4v::unpackhi_epi64(a4::mk128(h12, t4), a4::mk128(h14, t6))); h9 = a4::hi64(_v); std::memcpy(&t1, &_v, 8); } while (0);
    do { const a4::q128 _v = (a4m::pcpyld(a4::mk128(h15, t7), a4::mk128(h13, t5))); h10 = a4::hi64(_v); std::memcpy(&t2, &_v, 8); } while (0);
    do { const a4::q128 _v = (a4v::unpackhi_epi64(a4::mk128(h13, t5), a4::mk128(h15, t7))); h11 = a4::hi64(_v); std::memcpy(&t3, &_v, 8); } while (0);
    vf24 = a4v::castsi128_ps(a4::mk128(h8, t0));
    vf25 = a4v::castsi128_ps(a4::mk128(h9, t1));
    vf26 = a4v::castsi128_ps(a4::mk128(h10, t2));
    { a4::vec4 res = a4v::mulK<0, 1>(vf24, a4v::bcast<0>(vf28)); vacc = a4v::dest<1, 1, 1, 1>(vacc, res); }
    { a4::vec4 mul_res = a4v::mulK<0, 1>(vf25, a4v::bcast<1>(vf28)); a4::vec4 res = a4v::addK<1, 1>(vacc, mul_res); vacc = a4v::dest<1, 1, 1, 1>(vacc, res); }
    { a4::vec4 mul_res = a4v::mulK<0, 1>(vf26, a4v::bcast<2>(vf28)); a4::vec4 res = a4v::addK<1, 1>(vacc, mul_res); vacc = a4v::dest<1, 1, 1, 1>(vacc, res); }
    { a4::vec4 mul_res = a4v::mulK<1, 1>(vf0, a4v::bcast<3>(vf28)); a4::vec4 res = a4v::addK<1, 1>(vacc, mul_res); a4::q128 mask = a4v::set_epi32(-1, -1, -1, -1); vf29 = a4v::blendv_ps(vf29, res, a4v::castsi128_ps(mask)); }
    { a4::vec4 res = a4v::subK<1, 1>(vf29, vf29); a4::q128 mask = a4v::set_epi32(-1, 0, 0, 0); vf29 = a4v::blend(vf29, res, a4v::castsi128_ps(mask)); }
    { a4::vec4 res = a4v::addK<1, 1>(vf29, vf0); a4::q128 mask = a4v::set_epi32(-1, 0, 0, 0); vf29 = a4v::blend(vf29, res, a4v::castsi128_ps(mask)); }
    A4_W128(ADD32((uint32_t)v1, 0), a4v::castps_si128(vf24));
    A4_W128(ADD32((uint32_t)v1, 16), a4v::castps_si128(vf25));
    A4_W128(ADD32((uint32_t)v1, 32), a4v::castps_si128(vf26));
    A4_W128(ADD32((uint32_t)v1, 48), a4v::castps_si128(vf29));
    (s1 = (uint64_t)((int64_t)(int32_t)((int32_t)SLL32((uint32_t)s1, 4))));
    (a1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)sp, 128))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)s1, (uint32_t)s6))));
    vf8 = a4v::castsi128_ps(A4_R128(ADD32((uint32_t)v0, 0)));
    vf14 = a4v::castsi128_ps(A4_R128(ADD32((uint32_t)v1, 0)));
    vf15 = a4v::castsi128_ps(A4_R128(ADD32((uint32_t)v1, 16)));
    vf16 = a4v::castsi128_ps(A4_R128(ADD32((uint32_t)v1, 32)));
    vf17 = a4v::castsi128_ps(A4_R128(ADD32((uint32_t)v1, 48)));
    { a4::vec4 res = a4v::mul(vf14, a4v::bcast<0>(vf8)); vacc = a4v::dest<1, 1, 1, 1>(vacc, res); }
    { a4::vec4 mul_res = a4v::mul(vf15, a4v::bcast<1>(vf8)); a4::vec4 res = a4v::addK<1, 1>(vacc, mul_res); vacc = a4v::dest<1, 1, 1, 1>(vacc, res); }
    { a4::vec4 mul_res = a4v::mul(vf16, a4v::bcast<2>(vf8)); a4::vec4 res = a4v::addK<1, 1>(vacc, mul_res); vacc = a4v::dest<1, 1, 1, 1>(vacc, res); }
    { a4::vec4 mul_res = a4v::mul(vf17, a4v::bcast<3>(vf8)); a4::vec4 res = a4v::addK<1, 1>(vacc, mul_res); a4::q128 mask = a4v::set_epi32(-1, -1, -1, -1); vf10 = a4v::blendv_ps(vf10, res, a4v::castsi128_ps(mask)); }
    A4_W128(ADD32((uint32_t)a1, 0), a4v::castps_si128(vf10));
    (s3 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)sp, 144))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)s1, (uint32_t)s7))));
    vf8 = a4v::castsi128_ps(A4_R128(ADD32((uint32_t)v0, 0)));
    vf14 = a4v::castsi128_ps(A4_R128(ADD32((uint32_t)v1, 0)));
    vf15 = a4v::castsi128_ps(A4_R128(ADD32((uint32_t)v1, 16)));
    vf16 = a4v::castsi128_ps(A4_R128(ADD32((uint32_t)v1, 32)));
    vf17 = a4v::castsi128_ps(A4_R128(ADD32((uint32_t)v1, 48)));
    { a4::vec4 res = a4v::mul(vf14, a4v::bcast<0>(vf8)); vacc = a4v::dest<1, 1, 1, 1>(vacc, res); }
    { a4::vec4 mul_res = a4v::mul(vf15, a4v::bcast<1>(vf8)); a4::vec4 res = a4v::addK<1, 1>(vacc, mul_res); vacc = a4v::dest<1, 1, 1, 1>(vacc, res); }
    { a4::vec4 mul_res = a4v::mul(vf16, a4v::bcast<2>(vf8)); a4::vec4 res = a4v::addK<1, 1>(vacc, mul_res); vacc = a4v::dest<1, 1, 1, 1>(vacc, res); }
    { a4::vec4 mul_res = a4v::mul(vf17, a4v::bcast<3>(vf8)); a4::vec4 res = a4v::addK<1, 1>(vacc, mul_res); a4::q128 mask = a4v::set_epi32(-1, -1, -1, -1); vf10 = a4v::blendv_ps(vf10, res, a4v::castsi128_ps(mask)); }
    A4_W128(ADD32((uint32_t)s3, 0), a4v::castps_si128(vf10));
    vf5 = a4v::castsi128_ps(A4_R128(ADD32((uint32_t)a1, 0)));
    { a4::vec4 res = a4v::mul(vf5, vf5); a4::q128 mask = a4v::set_epi32(0, -1, -1, -1); vf7 = a4v::blend(vf7, res, a4v::castsi128_ps(mask)); }
    { a4::vec4 res = a4v::mulK<1, 1>(vf0, a4v::bcast<0>(vf7)); vacc = a4v::dest<0, 0, 0, 1>(vacc, res); }
    { a4::vec4 mul_res = a4v::mulK<1, 1>(vf0, a4v::bcast<1>(vf7)); a4::vec4 res = a4v::addK<1, 1>(vacc, mul_res); vacc = a4v::dest<0, 0, 0, 1>(vacc, res); }
    { a4::vec4 mul_res = a4v::mulK<1, 1>(vf0, a4v::bcast<2>(vf7)); a4::vec4 res = a4v::addK<1, 1>(vacc, mul_res); a4::q128 mask = a4v::set_epi32(-1, 0, 0, 0); vf7 = a4v::blendv_ps(vf7, res, a4v::castsi128_ps(mask)); }
    { float fs = a4v::cvtss_f32(a4v::shuffle_ps<0x03>(vf0, vf0)); float ft = a4v::cvtss_f32(a4v::shuffle_ps<0x03>(vf7, vf7)); vq = a4v::rsqrt(vstat, fs, ft); }
    { a4::vec4 res = a4v::mul(vf5, a4v::set1_ps(vq)); a4::q128 mask = a4v::set_epi32(0, -1, -1, -1); vf5 = a4v::blendv_ps(vf5, res, a4v::castsi128_ps(mask)); }
    A4_W128(ADD32((uint32_t)a1, 0), a4v::castps_si128(vf5));
    { const uint32_t _dynInsn = A1_R32(0x10C3AE8u); const int32_t _dynSimm = (int16_t)(_dynInsn & 0xFFFFu); const uint32_t _dynImm = _dynInsn & 0xFFFFu; (void)_dynSimm; (void)_dynImm; (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)_dynImm << 16)))); }
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)SLL32((uint32_t)s2, 6))));
    { const uint32_t _dynInsn = A1_R32(0x10C3AF0u); const int32_t _dynSimm = (int16_t)(_dynInsn & 0xFFFFu); const uint32_t _dynImm = _dynInsn & 0xFFFFu; (void)_dynSimm; (void)_dynImm; (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v0, _dynSimm)))); }
    { const bool c = (int32_t)s2 >= 0; (a2 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)sp, 160)))); if (c) goto L_10c3b08; }
    do { const a4::q128 _v = (A4_R128(ADD32((uint32_t)v0, 0))); h2 = a4::hi64(_v); std::memcpy(&v0, &_v, 8); } while (0);
    A4_W128(ADD32((uint32_t)sp, 160), a4::mk128(h2, v0));
    goto L_10c3b38;
L_10c3b08:;
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)s4, 8))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v1, (uint32_t)v0))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v0, 16))));
    vf5 = a4v::castsi128_ps(A4_R128(ADD32((uint32_t)v0, 0)));
    { a4::vec4 res = a4v::mul(vf5, vf5); a4::q128 mask = a4v::set_epi32(0, -1, -1, -1); vf7 = a4v::blend(vf7, res, a4v::castsi128_ps(mask)); }
    { a4::vec4 res = a4v::mulK<1, 1>(vf0, a4v::bcast<0>(vf7)); vacc = a4v::dest<0, 0, 0, 1>(vacc, res); }
    { a4::vec4 mul_res = a4v::mulK<1, 1>(vf0, a4v::bcast<1>(vf7)); a4::vec4 res = a4v::addK<1, 1>(vacc, mul_res); vacc = a4v::dest<0, 0, 0, 1>(vacc, res); }
    { a4::vec4 mul_res = a4v::mulK<1, 1>(vf0, a4v::bcast<2>(vf7)); a4::vec4 res = a4v::addK<1, 1>(vacc, mul_res); a4::q128 mask = a4v::set_epi32(-1, 0, 0, 0); vf7 = a4v::blendv_ps(vf7, res, a4v::castsi128_ps(mask)); }
    { float fs = a4v::cvtss_f32(a4v::shuffle_ps<0x03>(vf0, vf0)); float ft = a4v::cvtss_f32(a4v::shuffle_ps<0x03>(vf7, vf7)); vq = a4v::rsqrt(vstat, fs, ft); }
    { a4::vec4 res = a4v::mul(vf5, a4v::set1_ps(vq)); a4::q128 mask = a4v::set_epi32(0, -1, -1, -1); vf5 = a4v::blendv_ps(vf5, res, a4v::castsi128_ps(mask)); }
    A4_W128(ADD32((uint32_t)a2, 0), a4v::castps_si128(vf5));
L_10c3b38:;
    (s0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)s5, (uint32_t)s1))));
    { const uint32_t _dynInsn = A1_R32(0x10C3B3Cu); const int32_t _dynSimm = (int16_t)(_dynInsn & 0xFFFFu); const uint32_t _dynImm = _dynInsn & 0xFFFFu; (void)_dynSimm; (void)_dynImm; (a3 = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)_dynImm << 16)))); }
    { const uint32_t _dynInsn = A1_R32(0x10C3B40u); const int32_t _dynSimm = (int16_t)(_dynInsn & 0xFFFFu); const uint32_t _dynImm = _dynInsn & 0xFFFFu; (void)_dynSimm; (void)_dynImm; (a3 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)a3, _dynSimm)))); }
    { (a0 = (uint64_t)((uint64_t)RC_U64X(s0) + (uint64_t)0ULL)); A3_CHARGE(); a9x::diffQuat(ram, rt, (uint32_t)a0, (uint32_t)a1, (uint32_t)a2, (uint32_t)a3); }
    (a1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)sp, 176))));
    vf4 = a4v::castsi128_ps(A4_R128(ADD32((uint32_t)s0, 0)));
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
    A4_W128(ADD32((uint32_t)a1, 0), a4v::castps_si128(vf6));
    (a2 = (uint64_t)((uint64_t)s3 + (uint64_t)0ULL));
    { (a0 = (uint64_t)((uint64_t)s0 + (uint64_t)0ULL)); A3_CHARGE(); f0 = a9x::axisDirZY(ram, rt, (uint32_t)a0, (uint32_t)a1, (uint32_t)a2); }
    vf14 = a4v::castsi128_ps(A4_R128(ADD32((uint32_t)s0, 0)));
    { a4::vec4 res = a4v::mul(vf14, vf14); a4::q128 mask = a4v::set_epi32(-1, -1, -1, -1); vf15 = a4v::blend(vf15, res, a4v::castsi128_ps(mask)); }
    { a4::vec4 res = a4v::addK<1, 1>(vf15, a4v::bcast<1>(vf15)); a4::q128 mask = a4v::set_epi32(0, 0, 0, -1); vf15 = a4v::blendv_ps(vf15, res, a4v::castsi128_ps(mask)); }
    { a4::vec4 res = a4v::addK<1, 1>(vf15, a4v::bcast<2>(vf15)); a4::q128 mask = a4v::set_epi32(0, 0, 0, -1); vf15 = a4v::blendv_ps(vf15, res, a4v::castsi128_ps(mask)); }
    { a4::vec4 res = a4v::addK<1, 1>(vf15, a4v::bcast<3>(vf15)); a4::q128 mask = a4v::set_epi32(0, 0, 0, -1); vf15 = a4v::blendv_ps(vf15, res, a4v::castsi128_ps(mask)); }
    { float fs = a4v::cvtss_f32(a4v::shuffle_ps<0x03>(vf0, vf0)); float ft = a4v::cvtss_f32(a4v::bcast<0>(vf15)); vq = a4v::rsqrt(vstat, fs, ft); }
    { a4::vec4 res = a4v::mul(vf14, a4v::set1_ps(vq)); a4::q128 mask = a4v::set_epi32(-1, -1, -1, -1); vf15 = a4v::blendv_ps(vf15, res, a4v::castsi128_ps(mask)); }
    A4_W128(ADD32((uint32_t)s0, 0), a4v::castps_si128(vf15));
    (v0 = (uint64_t)((uint64_t)sp + (uint64_t)0ULL));
    { a4::vec4 res = a4v::addK<1, 1>(vf0, a4v::bcast<3>(vf0)); a4::q128 mask = a4v::set_epi32(0, -1, -1, -1); vf12 = a4v::blendv_ps(vf12, res, a4v::castsi128_ps(mask)); }
    vf11 = a4v::castsi128_ps(A4_R128(ADD32((uint32_t)s0, 0)));
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
    A4_W128(ADD32((uint32_t)v0, 0), a4v::castps_si128(vf14));
    A4_W128(ADD32((uint32_t)v0, 16), a4v::castps_si128(vf15));
    A4_W128(ADD32((uint32_t)v0, 32), a4v::castps_si128(vf16));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v0, 48))));
    A4_W128(ADD32((uint32_t)v0, 0), a4v::castps_si128(vf0));
    (a0 = (uint64_t)((uint64_t)sp + (uint64_t)0ULL));
    { (s0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 4294967295)))); A3_CHARGE(); iosMulCurrentMatrixR(ram, rt, (uint32_t)a0, vf1, vf2, vf3, vf4, vf14, vf15, vf16, vf17, vacc); }
    (a0 = (uint64_t)((uint64_t)s5 + (uint64_t)0ULL));
    (t1 = (uint64_t)((uint64_t)s2 + (uint64_t)0ULL));
    (a1 = (uint64_t)((uint64_t)s6 + (uint64_t)0ULL));
    (a2 = (uint64_t)((uint64_t)s7 + (uint64_t)0ULL));
    (a3 = (uint64_t)((uint64_t)s4 + (uint64_t)0ULL));
    { const bool c = (int64_t)s2 == (int64_t)s0; (t0 = (uint64_t)((uint64_t)fp + (uint64_t)0ULL)); if (c) goto L_10c3c64; }
    { A3_CHARGE(); { const a1::RetVF a2_r = manager_sub_010C38E8(ram, rt, a0, a1, a2, a3, t0, t1, sp, f0, f1, f2, f3, f4, f5, f6, f7, f8, f9, f10, f11, f12, f13, f14, f15, f16, f17, f18, f19, f20, f21, f22, f23, f24, f25, f26, f27, f28, f29, f30, f31, vf0, vf1, vf2, vf3, vf4, vf5, vf6, vf7, vf8, vf10, vf11, vf12, vf13, vf14, vf15, vf16, vf17, vf24, vf25, vf26, vf28, vf29, vi15, vacc, vq, vstat, h2, h8, h9, h10, h11, h12, h13, h14, h15); v0 = a2_r.v0; f0 = a2_r.f0; } }
L_10c3c64:;
    { A3_CHARGE(); a11x::popMatrix(ram, rt, vf1, vf2, vf3, vf4, vi15); }
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)sp, 192))));
    { const bool c = (int64_t)v0 == (int64_t)s0; (a0 = (uint64_t)((uint64_t)s5 + (uint64_t)0ULL)); if (c) goto L_10c3c90; }
    (a1 = (uint64_t)((uint64_t)s6 + (uint64_t)0ULL));
    (a2 = (uint64_t)((uint64_t)s7 + (uint64_t)0ULL));
    (a3 = (uint64_t)((uint64_t)s4 + (uint64_t)0ULL));
    (t0 = (uint64_t)((uint64_t)fp + (uint64_t)0ULL));
    { (t1 = (uint64_t)((uint64_t)RC_U64X(v0) + (uint64_t)0ULL)); A3_CHARGE(); { const a1::RetVF a2_r = manager_sub_010C38E8(ram, rt, a0, a1, a2, a3, t0, t1, sp, f0, f1, f2, f3, f4, f5, f6, f7, f8, f9, f10, f11, f12, f13, f14, f15, f16, f17, f18, f19, f20, f21, f22, f23, f24, f25, f26, f27, f28, f29, f30, f31, vf0, vf1, vf2, vf3, vf4, vf5, vf6, vf7, vf8, vf10, vf11, vf12, vf13, vf14, vf15, vf16, vf17, vf24, vf25, vf26, vf28, vf29, vi15, vacc, vq, vstat, h2, h8, h9, h10, h11, h12, h13, h14, h15); v0 = a2_r.v0; f0 = a2_r.f0; } }
L_10c3c90:;
    { (sp = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)sp, 288)))); io_vf1 = vf1; io_vf2 = vf2; io_vf3 = vf3; io_vf4 = vf4; io_vf5 = vf5; io_vf6 = vf6; io_vf7 = vf7; io_vf8 = vf8; io_vf10 = vf10; io_vf11 = vf11; io_vf12 = vf12; io_vf13 = vf13; io_vf14 = vf14; io_vf15 = vf15; io_vf16 = vf16; io_vf17 = vf17; io_vf24 = vf24; io_vf25 = vf25; io_vf26 = vf26; io_vf28 = vf28; io_vf29 = vf29; io_vi15 = vi15; io_vacc = vacc; io_vq = vq; io_vstat = vstat; io_h2 = h2; io_h8 = h8; io_h9 = h9; io_h10 = h10; io_h11 = h11; io_h12 = h12; io_h13 = h13; io_h14 = h14; io_h15 = h15; return a1::RetVF{(uint64_t)v0, f0}; }
    __builtin_unreachable();
}

// GetMoveVectorOnWV (0x1132f30): typed by tools/typed_lift2.py (round A6; guest frame calls, extra outputs xo[0]=f20 xo[1]=a1 xo[2]=a2 xo[3]=a3 xo[4]=t0 xo[5]=t1 xo[6]=t2 xo[7]=t3; VU0 in vf0 vf7 vf10 vf25 vf28 vf29 vacc vstat h2, out vf5 vf6 vf7 vf8 vf10 vf14 vf15 vf16 vf17 vf24 vf25 vf26 vf28 vf29 vacc vq vstat h6 h7 h8 h9 h10 h11 h12 h13 h14 h15, relocated calls 1)
inline a1::RetVF GetMoveVectorOnWV(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint64_t a0, uint64_t a1, uint64_t a2, uint64_t a3, uint64_t sp, float f12, float f13, float f14, float f15, float f16, float f17, float f18, float f19, float f20, const a4::vec4 &io_vf0, a4::vec4 &io_vf5, a4::vec4 &io_vf6, a4::vec4 &io_vf7, a4::vec4 &io_vf8, a4::vec4 &io_vf10, a4::vec4 &io_vf14, a4::vec4 &io_vf15, a4::vec4 &io_vf16, a4::vec4 &io_vf17, a4::vec4 &io_vf24, a4::vec4 &io_vf25, a4::vec4 &io_vf26, a4::vec4 &io_vf28, a4::vec4 &io_vf29, a4::vec4 &io_vacc, float &io_vq, uint16_t &io_vstat, uint64_t io_h2, uint64_t &io_h6, uint64_t &io_h7, uint64_t &io_h8, uint64_t &io_h9, uint64_t &io_h10, uint64_t &io_h11, uint64_t &io_h12, uint64_t &io_h13, uint64_t &io_h14, uint64_t &io_h15, uint64_t *xo = nullptr)
{
    float f0 = 0.0f;
    float f1 = 0.0f;
    float f2 = 0.0f;
    uint64_t at = 0;
    uint64_t v0 = 0;
    uint64_t v1 = 0;
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
    a4::vec4 vf24 = io_vf24;
    a4::vec4 vf25 = io_vf25;
    a4::vec4 vf26 = io_vf26;
    a4::vec4 vf28 = io_vf28;
    a4::vec4 vf29 = io_vf29;
    a4::vec4 vacc = io_vacc;
    float vq = io_vq;
    uint16_t vstat = io_vstat;
    uint64_t h2 = io_h2;
    uint64_t h6 = io_h6;
    uint64_t h7 = io_h7;
    uint64_t h8 = io_h8;
    uint64_t h9 = io_h9;
    uint64_t h10 = io_h10;
    uint64_t h11 = io_h11;
    uint64_t h12 = io_h12;
    uint64_t h13 = io_h13;
    uint64_t h14 = io_h14;
    uint64_t h15 = io_h15;
    (sp = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)sp, 4294967104))));
    { float f = f20; uint32_t bits; std::memcpy(&bits, &f, sizeof(bits)); A1_W32B((uint32_t)sp, 184, bits); }
    f20 = ((float)(f12));
    f0 = (-(float)(f20));
    (s1 = (uint64_t)((uint64_t)a1 + (uint64_t)0ULL));
    (s0 = (uint64_t)((uint64_t)a0 + (uint64_t)0ULL));
    { uint32_t bits; std::memcpy(&bits, &f0, sizeof(bits)); (v0 = (uint64_t)((int64_t)(int32_t)(bits))); }
    vf5 = a4v::castsi128_ps(A4_R128(ADD32((uint32_t)a2, 0)));
    vf6 = a4v::castsi128_ps(a4::mk128(h2, v0));
    { a4::vec4 res = a4v::mul(vf5, a4v::bcast<0>(vf6)); a4::q128 mask = a4v::set_epi32(0, -1, -1, -1); vf5 = a4v::blendv_ps(vf5, res, a4v::castsi128_ps(mask)); }
    A4_W128(ADD32((uint32_t)sp, 0), a4v::castps_si128(vf5));
    (a1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)sp, 80))));
    vf24 = a4v::castsi128_ps(A4_R128(ADD32((uint32_t)a3, 48)));
    { a4::vec4 res = a4v::subK<1, 1>(vf0, a4v::bcast<3>(vf0)); a4::q128 mask = a4v::set_epi32(0, 0, 0, -1); vf25 = a4v::blendv_ps(vf25, res, a4v::castsi128_ps(mask)); }
    { a4::vec4 res = a4v::mulK<0, 1>(vf24, a4v::bcast<0>(vf25)); a4::q128 mask = a4v::set_epi32(0, -1, -1, -1); vf28 = a4v::blendv_ps(vf28, res, a4v::castsi128_ps(mask)); }
    { a4::vec4 res = a4v::sub(vf28, vf28); a4::q128 mask = a4v::set_epi32(-1, 0, 0, 0); vf28 = a4v::blend(vf28, res, a4v::castsi128_ps(mask)); }
    do { const a4::q128 _v = (A4_R128(ADD32((uint32_t)a3, 0))); h8 = a4::hi64(_v); std::memcpy(&t0, &_v, 8); } while (0);
    do { const a4::q128 _v = (A4_R128(ADD32((uint32_t)a3, 16))); h9 = a4::hi64(_v); std::memcpy(&t1, &_v, 8); } while (0);
    do { const a4::q128 _v = (A4_R128(ADD32((uint32_t)a3, 32))); h10 = a4::hi64(_v); std::memcpy(&t2, &_v, 8); } while (0);
    do { const a4::q128 _v = (a4v::castps_si128(vf0)); h11 = a4::hi64(_v); std::memcpy(&t3, &_v, 8); } while (0);
    do { const a4::q128 _v = (a4m::pextlw(a4::mk128(h9, t1), a4::mk128(h8, t0))); h12 = a4::hi64(_v); std::memcpy(&t4, &_v, 8); } while (0);
    do { const a4::q128 _v = (a4m::pextuw(a4::mk128(h9, t1), a4::mk128(h8, t0))); h13 = a4::hi64(_v); std::memcpy(&t5, &_v, 8); } while (0);
    do { const a4::q128 _v = (a4m::pextlw(a4::mk128(h11, t3), a4::mk128(h10, t2))); h14 = a4::hi64(_v); std::memcpy(&t6, &_v, 8); } while (0);
    do { const a4::q128 _v = (a4m::pextuw(a4::mk128(h11, t3), a4::mk128(h10, t2))); h15 = a4::hi64(_v); std::memcpy(&t7, &_v, 8); } while (0);
    do { const a4::q128 _v = (a4m::pcpyld(a4::mk128(h14, t6), a4::mk128(h12, t4))); h8 = a4::hi64(_v); std::memcpy(&t0, &_v, 8); } while (0);
    do { const a4::q128 _v = (a4v::unpackhi_epi64(a4::mk128(h12, t4), a4::mk128(h14, t6))); h9 = a4::hi64(_v); std::memcpy(&t1, &_v, 8); } while (0);
    do { const a4::q128 _v = (a4m::pcpyld(a4::mk128(h15, t7), a4::mk128(h13, t5))); h10 = a4::hi64(_v); std::memcpy(&t2, &_v, 8); } while (0);
    do { const a4::q128 _v = (a4v::unpackhi_epi64(a4::mk128(h13, t5), a4::mk128(h15, t7))); h11 = a4::hi64(_v); std::memcpy(&t3, &_v, 8); } while (0);
    vf24 = a4v::castsi128_ps(a4::mk128(h8, t0));
    vf25 = a4v::castsi128_ps(a4::mk128(h9, t1));
    vf26 = a4v::castsi128_ps(a4::mk128(h10, t2));
    { a4::vec4 res = a4v::mulK<0, 1>(vf24, a4v::bcast<0>(vf28)); vacc = a4v::dest<1, 1, 1, 1>(vacc, res); }
    { a4::vec4 mul_res = a4v::mulK<0, 1>(vf25, a4v::bcast<1>(vf28)); a4::vec4 res = a4v::addK<1, 1>(vacc, mul_res); vacc = a4v::dest<1, 1, 1, 1>(vacc, res); }
    { a4::vec4 mul_res = a4v::mulK<0, 1>(vf26, a4v::bcast<2>(vf28)); a4::vec4 res = a4v::addK<1, 1>(vacc, mul_res); vacc = a4v::dest<1, 1, 1, 1>(vacc, res); }
    { a4::vec4 mul_res = a4v::mulK<1, 1>(vf0, a4v::bcast<3>(vf28)); a4::vec4 res = a4v::addK<1, 1>(vacc, mul_res); a4::q128 mask = a4v::set_epi32(-1, -1, -1, -1); vf29 = a4v::blendv_ps(vf29, res, a4v::castsi128_ps(mask)); }
    { a4::vec4 res = a4v::subK<1, 1>(vf29, vf29); a4::q128 mask = a4v::set_epi32(-1, 0, 0, 0); vf29 = a4v::blend(vf29, res, a4v::castsi128_ps(mask)); }
    { a4::vec4 res = a4v::addK<1, 1>(vf29, vf0); a4::q128 mask = a4v::set_epi32(-1, 0, 0, 0); vf29 = a4v::blend(vf29, res, a4v::castsi128_ps(mask)); }
    A4_W128(ADD32((uint32_t)a1, 0), a4v::castps_si128(vf24));
    A4_W128(ADD32((uint32_t)a1, 16), a4v::castps_si128(vf25));
    A4_W128(ADD32((uint32_t)a1, 32), a4v::castps_si128(vf26));
    A4_W128(ADD32((uint32_t)a1, 48), a4v::castps_si128(vf29));
    { const uint32_t _dynInsn = A1_R32(0x1132FE0u); const int32_t _dynSimm = (int16_t)(_dynInsn & 0xFFFFu); const uint32_t _dynImm = _dynInsn & 0xFFFFu; (void)_dynSimm; (void)_dynImm; (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)_dynImm << 16)))); }
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)sp, 144))));
    { const uint32_t _dynInsn = A1_R32(0x1132FE8u); const int32_t _dynSimm = (int16_t)(_dynInsn & 0xFFFFu); const uint32_t _dynImm = _dynInsn & 0xFFFFu; (void)_dynSimm; (void)_dynImm; (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v0, _dynSimm)))); }
    vf8 = a4v::castsi128_ps(A4_R128(ADD32((uint32_t)v0, 0)));
    vf14 = a4v::castsi128_ps(A4_R128(ADD32((uint32_t)a1, 0)));
    vf15 = a4v::castsi128_ps(A4_R128(ADD32((uint32_t)a1, 16)));
    vf16 = a4v::castsi128_ps(A4_R128(ADD32((uint32_t)a1, 32)));
    vf17 = a4v::castsi128_ps(A4_R128(ADD32((uint32_t)a1, 48)));
    { a4::vec4 res = a4v::mul(vf14, a4v::bcast<0>(vf8)); vacc = a4v::dest<1, 1, 1, 1>(vacc, res); }
    { a4::vec4 mul_res = a4v::mul(vf15, a4v::bcast<1>(vf8)); a4::vec4 res = a4v::addK<1, 1>(vacc, mul_res); vacc = a4v::dest<1, 1, 1, 1>(vacc, res); }
    { a4::vec4 mul_res = a4v::mul(vf16, a4v::bcast<2>(vf8)); a4::vec4 res = a4v::addK<1, 1>(vacc, mul_res); vacc = a4v::dest<1, 1, 1, 1>(vacc, res); }
    { a4::vec4 mul_res = a4v::mul(vf17, a4v::bcast<3>(vf8)); a4::vec4 res = a4v::addK<1, 1>(vacc, mul_res); a4::q128 mask = a4v::set_epi32(-1, -1, -1, -1); vf10 = a4v::blendv_ps(vf10, res, a4v::castsi128_ps(mask)); }
    A4_W128(ADD32((uint32_t)a0, 0), a4v::castps_si128(vf10));
    { uint32_t bits = A1_R32B((uint32_t)sp, 148); float f; std::memcpy(&f, &bits, sizeof(f)); f2 = f; }
    { const uint32_t _dynInsn = A1_R32(0x1133018u); const int32_t _dynSimm = (int16_t)(_dynInsn & 0xFFFFu); const uint32_t _dynImm = _dynInsn & 0xFFFFu; (void)_dynSimm; (void)_dynImm; (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)_dynImm << 16)))); }
    { uint32_t bits = 0u; std::memcpy(&f0, &bits, sizeof(bits)); }
    fcr31 = (a1f::lt(f2, f0)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
    { const bool c = (fcr31 & 0x800000u) == 0u; { const uint32_t _dynInsn = A1_R32(0x1133028u); const int32_t _dynSimm = (int16_t)(_dynInsn & 0xFFFFu); const uint32_t _dynImm = _dynInsn & 0xFFFFu; (void)_dynSimm; (void)_dynImm; (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v1, _dynSimm)))); } if (c) goto L_1133050; }
    (at = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)48948 << 16))));
    (at = (uint64_t)(RC_U64X(at) | (uint64_t)(uint16_t)65011));
    { uint32_t bits = (uint32_t)at; std::memcpy(&f0, &bits, sizeof(bits)); }
    fcr31 = (a1f::lt(f2, f0)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
    if ((fcr31 & 0x800000u) != 0u) goto L_1133068;
    goto L_11330c4;
L_1133050:;
    (at = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)16180 << 16))));
    (at = (uint64_t)(RC_U64X(at) | (uint64_t)(uint16_t)65011));
    { uint32_t bits = (uint32_t)at; std::memcpy(&f0, &bits, sizeof(bits)); }
    fcr31 = (a1f::lt(f0, f2)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
    if ((fcr31 & 0x800000u) == 0u) goto L_11330c4;
L_1133068:;
    vf8 = a4v::castsi128_ps(A4_R128(ADD32((uint32_t)v1, 0)));
    vf14 = a4v::castsi128_ps(A4_R128(ADD32((uint32_t)a1, 0)));
    vf15 = a4v::castsi128_ps(A4_R128(ADD32((uint32_t)a1, 16)));
    vf16 = a4v::castsi128_ps(A4_R128(ADD32((uint32_t)a1, 32)));
    vf17 = a4v::castsi128_ps(A4_R128(ADD32((uint32_t)a1, 48)));
    { a4::vec4 res = a4v::mul(vf14, a4v::bcast<0>(vf8)); vacc = a4v::dest<1, 1, 1, 1>(vacc, res); }
    { a4::vec4 mul_res = a4v::mul(vf15, a4v::bcast<1>(vf8)); a4::vec4 res = a4v::addK<1, 1>(vacc, mul_res); vacc = a4v::dest<1, 1, 1, 1>(vacc, res); }
    { a4::vec4 mul_res = a4v::mul(vf16, a4v::bcast<2>(vf8)); a4::vec4 res = a4v::addK<1, 1>(vacc, mul_res); vacc = a4v::dest<1, 1, 1, 1>(vacc, res); }
    { a4::vec4 mul_res = a4v::mul(vf17, a4v::bcast<3>(vf8)); a4::vec4 res = a4v::addK<1, 1>(vacc, mul_res); a4::q128 mask = a4v::set_epi32(-1, -1, -1, -1); vf10 = a4v::blendv_ps(vf10, res, a4v::castsi128_ps(mask)); }
    A4_W128(ADD32((uint32_t)a0, 0), a4v::castps_si128(vf10));
    { uint32_t bits = 0u; std::memcpy(&f0, &bits, sizeof(bits)); }
    (at = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)49024 << 16))));
    { uint32_t bits = (uint32_t)at; std::memcpy(&f1, &bits, sizeof(bits)); }
    fcr31 = (a1f::lt(f2, f0)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
    if ((fcr31 & 0x800000u) != 0u) goto L_11330b0;
    (at = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)16256 << 16))));
    { uint32_t bits = (uint32_t)at; std::memcpy(&f1, &bits, sizeof(bits)); }
L_11330b0:;
    { uint32_t bits; std::memcpy(&bits, &f1, sizeof(bits)); (v0 = (uint64_t)((int64_t)(int32_t)(bits))); }
    vf5 = a4v::castsi128_ps(A4_R128(ADD32((uint32_t)a0, 0)));
    vf6 = a4v::castsi128_ps(a4::mk128(h2, v0));
    { a4::vec4 res = a4v::mul(vf5, a4v::bcast<0>(vf6)); a4::q128 mask = a4v::set_epi32(0, -1, -1, -1); vf5 = a4v::blendv_ps(vf5, res, a4v::castsi128_ps(mask)); }
    A4_W128(ADD32((uint32_t)a0, 0), a4v::castps_si128(vf5));
L_11330c4:;
    (at = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)49024 << 16))));
    { uint32_t bits = (uint32_t)at; std::memcpy(&f0, &bits, sizeof(bits)); }
    A1_W32B((uint32_t)sp, 148, 0u);
    { uint32_t bits; std::memcpy(&bits, &f0, sizeof(bits)); (v0 = (uint64_t)((int64_t)(int32_t)(bits))); }
    vf5 = a4v::castsi128_ps(A4_R128(ADD32((uint32_t)a0, 0)));
    vf6 = a4v::castsi128_ps(a4::mk128(h2, v0));
    { a4::vec4 res = a4v::mul(vf5, a4v::bcast<0>(vf6)); a4::q128 mask = a4v::set_epi32(0, -1, -1, -1); vf5 = a4v::blendv_ps(vf5, res, a4v::castsi128_ps(mask)); }
    A4_W128(ADD32((uint32_t)a0, 0), a4v::castps_si128(vf5));
    vf5 = a4v::castsi128_ps(A4_R128(ADD32((uint32_t)a0, 0)));
    { a4::vec4 res = a4v::mul(vf5, vf5); a4::q128 mask = a4v::set_epi32(0, -1, -1, -1); vf7 = a4v::blend(vf7, res, a4v::castsi128_ps(mask)); }
    { a4::vec4 res = a4v::mulK<1, 1>(vf0, a4v::bcast<0>(vf7)); vacc = a4v::dest<0, 0, 0, 1>(vacc, res); }
    { a4::vec4 mul_res = a4v::mulK<1, 1>(vf0, a4v::bcast<1>(vf7)); a4::vec4 res = a4v::addK<1, 1>(vacc, mul_res); vacc = a4v::dest<0, 0, 0, 1>(vacc, res); }
    { a4::vec4 mul_res = a4v::mulK<1, 1>(vf0, a4v::bcast<2>(vf7)); a4::vec4 res = a4v::addK<1, 1>(vacc, mul_res); a4::q128 mask = a4v::set_epi32(-1, 0, 0, 0); vf7 = a4v::blendv_ps(vf7, res, a4v::castsi128_ps(mask)); }
    { float fs = a4v::cvtss_f32(a4v::shuffle_ps<0x03>(vf0, vf0)); float ft = a4v::cvtss_f32(a4v::shuffle_ps<0x03>(vf7, vf7)); vq = a4v::rsqrt(vstat, fs, ft); }
    { a4::vec4 res = a4v::mul(vf5, a4v::set1_ps(vq)); a4::q128 mask = a4v::set_epi32(0, -1, -1, -1); vf5 = a4v::blendv_ps(vf5, res, a4v::castsi128_ps(mask)); }
    A4_W128(ADD32((uint32_t)a0, 0), a4v::castps_si128(vf5));
    { const uint32_t _dynInsn = A1_R32(0x1133108u); const int32_t _dynSimm = (int16_t)(_dynInsn & 0xFFFFu); const uint32_t _dynImm = _dynInsn & 0xFFFFu; (void)_dynSimm; (void)_dynImm; (a1 = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)_dynImm << 16)))); }
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)sp, 16))));
    { { const uint32_t _dynInsn = A1_R32(0x1133114u); const int32_t _dynSimm = (int16_t)(_dynInsn & 0xFFFFu); const uint32_t _dynImm = _dynInsn & 0xFFFFu; (void)_dynSimm; (void)_dynImm; (a1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)a1, _dynSimm)))); } A3_CHARGE(); uint64_t a7c[4] = {0u, 0u, 0u, 0u}; iosCopyMatrix_x11(ram, rt, (uint32_t)a0, (uint32_t)a1, h6, h7, h8, h9, a7c); a2 = a7c[0]; a3 = a7c[1]; t0 = a7c[2]; t1 = a7c[3]; }
    { uint32_t bits = A1_R32B((uint32_t)sp, 144); float f; std::memcpy(&f, &bits, sizeof(f)); f0 = f; }
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)sp, 16))));
    { uint32_t bits = A1_R32B((uint32_t)sp, 152); float f; std::memcpy(&f, &bits, sizeof(f)); f1 = f; }
    f2 = (-(float)(f0));
    { float f = f0; uint32_t bits; std::memcpy(&bits, &f, sizeof(bits)); A1_W32B((uint32_t)sp, 48, bits); }
    { float f = f1; uint32_t bits; std::memcpy(&bits, &f, sizeof(bits)); A1_W32B((uint32_t)sp, 16, bits); }
    { float f = f1; uint32_t bits; std::memcpy(&bits, &f, sizeof(bits)); A1_W32B((uint32_t)sp, 56, bits); }
    { float f = f2; uint32_t bits; std::memcpy(&bits, &f, sizeof(bits)); A1_W32B((uint32_t)sp, 24, bits); }
    vf8 = a4v::castsi128_ps(A4_R128(ADD32((uint32_t)s1, 0)));
    vf14 = a4v::castsi128_ps(A4_R128(ADD32((uint32_t)v0, 0)));
    vf15 = a4v::castsi128_ps(A4_R128(ADD32((uint32_t)v0, 16)));
    vf16 = a4v::castsi128_ps(A4_R128(ADD32((uint32_t)v0, 32)));
    vf17 = a4v::castsi128_ps(A4_R128(ADD32((uint32_t)v0, 48)));
    { a4::vec4 res = a4v::mul(vf14, a4v::bcast<0>(vf8)); vacc = a4v::dest<1, 1, 1, 1>(vacc, res); }
    { a4::vec4 mul_res = a4v::mul(vf15, a4v::bcast<1>(vf8)); a4::vec4 res = a4v::addK<1, 1>(vacc, mul_res); vacc = a4v::dest<1, 1, 1, 1>(vacc, res); }
    { a4::vec4 mul_res = a4v::mul(vf16, a4v::bcast<2>(vf8)); a4::vec4 res = a4v::addK<1, 1>(vacc, mul_res); vacc = a4v::dest<1, 1, 1, 1>(vacc, res); }
    { a4::vec4 mul_res = a4v::mul(vf17, a4v::bcast<3>(vf8)); a4::vec4 res = a4v::addK<1, 1>(vacc, mul_res); a4::q128 mask = a4v::set_epi32(-1, -1, -1, -1); vf10 = a4v::blendv_ps(vf10, res, a4v::castsi128_ps(mask)); }
    A4_W128(ADD32((uint32_t)s0, 0), a4v::castps_si128(vf10));
    { uint32_t bits; std::memcpy(&bits, &f20, sizeof(bits)); (v0 = (uint64_t)((int64_t)(int32_t)(bits))); }
    vf5 = a4v::castsi128_ps(A4_R128(ADD32((uint32_t)s0, 0)));
    vf6 = a4v::castsi128_ps(a4::mk128(h2, v0));
    { a4::vec4 res = a4v::mul(vf5, a4v::bcast<0>(vf6)); a4::q128 mask = a4v::set_epi32(0, -1, -1, -1); vf5 = a4v::blendv_ps(vf5, res, a4v::castsi128_ps(mask)); }
    A4_W128(ADD32((uint32_t)s0, 0), a4v::castps_si128(vf5));
    { uint32_t bits = A1_R32B((uint32_t)sp, 4); float f; std::memcpy(&f, &bits, sizeof(f)); f0 = f; }
    { float f = f0; uint32_t bits; std::memcpy(&bits, &f, sizeof(bits)); A1_W32B((uint32_t)s0, 4, bits); }
    { uint32_t bits = A1_R32B((uint32_t)sp, 184); float f; std::memcpy(&f, &bits, sizeof(f)); f20 = f; }
    { (sp = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)sp, 192)))); if (xo) { xo[0] = a1_bits(f20); xo[1] = a1; xo[2] = a2; xo[3] = a3; xo[4] = t0; xo[5] = t1; xo[6] = t2; xo[7] = t3; } io_vf5 = vf5; io_vf6 = vf6; io_vf7 = vf7; io_vf8 = vf8; io_vf10 = vf10; io_vf14 = vf14; io_vf15 = vf15; io_vf16 = vf16; io_vf17 = vf17; io_vf24 = vf24; io_vf25 = vf25; io_vf26 = vf26; io_vf28 = vf28; io_vf29 = vf29; io_vacc = vacc; io_vq = vq; io_vstat = vstat; io_h6 = h6; io_h7 = h7; io_h8 = h8; io_h9 = h9; io_h10 = h10; io_h11 = h11; io_h12 = h12; io_h13 = h13; io_h14 = h14; io_h15 = h15; return a1::RetVF{(uint64_t)v0, f0}; }
    __builtin_unreachable();
}

// GetXZVectorOnWV (0x1133198): typed by tools/typed_lift2.py (round A6; guest frame calls; VU0 in vf0 vf7 vf10 vf25 vf28 vf29 vacc vstat h2, out vf5 vf6 vf7 vf8 vf10 vf14 vf15 vf16 vf17 vf24 vf25 vf26 vf28 vf29 vacc vq vstat h6 h7 h8 h9 h10 h11 h12 h13 h14 h15, relocated calls 1)
inline a1::RetVF GetXZVectorOnWV(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint64_t a0, uint64_t a1, uint64_t a2, uint64_t sp, float f12, float f13, float f14, float f15, float f16, float f17, float f18, float f19, const a4::vec4 &io_vf0, a4::vec4 &io_vf5, a4::vec4 &io_vf6, a4::vec4 &io_vf7, a4::vec4 &io_vf8, a4::vec4 &io_vf10, a4::vec4 &io_vf14, a4::vec4 &io_vf15, a4::vec4 &io_vf16, a4::vec4 &io_vf17, a4::vec4 &io_vf24, a4::vec4 &io_vf25, a4::vec4 &io_vf26, a4::vec4 &io_vf28, a4::vec4 &io_vf29, a4::vec4 &io_vacc, float &io_vq, uint16_t &io_vstat, uint64_t io_h2, uint64_t &io_h6, uint64_t &io_h7, uint64_t &io_h8, uint64_t &io_h9, uint64_t &io_h10, uint64_t &io_h11, uint64_t &io_h12, uint64_t &io_h13, uint64_t &io_h14, uint64_t &io_h15)
{
    float f0 = 0.0f;
    float f1 = 0.0f;
    float f2 = 0.0f;
    uint64_t at = 0;
    uint64_t v0 = 0;
    uint64_t v1 = 0;
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
    a4::vec4 vf24 = io_vf24;
    a4::vec4 vf25 = io_vf25;
    a4::vec4 vf26 = io_vf26;
    a4::vec4 vf28 = io_vf28;
    a4::vec4 vf29 = io_vf29;
    a4::vec4 vacc = io_vacc;
    float vq = io_vq;
    uint16_t vstat = io_vstat;
    uint64_t h2 = io_h2;
    uint64_t h6 = io_h6;
    uint64_t h7 = io_h7;
    uint64_t h8 = io_h8;
    uint64_t h9 = io_h9;
    uint64_t h10 = io_h10;
    uint64_t h11 = io_h11;
    uint64_t h12 = io_h12;
    uint64_t h13 = io_h13;
    uint64_t h14 = io_h14;
    uint64_t h15 = io_h15;
    (sp = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)sp, 4294967120))));
    (s0 = (uint64_t)((uint64_t)a2 + (uint64_t)0ULL));
    (s1 = (uint64_t)((uint64_t)a0 + (uint64_t)0ULL));
    (a2 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)sp, 64))));
    vf24 = a4v::castsi128_ps(A4_R128(ADD32((uint32_t)a1, 48)));
    { a4::vec4 res = a4v::subK<1, 1>(vf0, a4v::bcast<3>(vf0)); a4::q128 mask = a4v::set_epi32(0, 0, 0, -1); vf25 = a4v::blendv_ps(vf25, res, a4v::castsi128_ps(mask)); }
    { a4::vec4 res = a4v::mulK<0, 1>(vf24, a4v::bcast<0>(vf25)); a4::q128 mask = a4v::set_epi32(0, -1, -1, -1); vf28 = a4v::blendv_ps(vf28, res, a4v::castsi128_ps(mask)); }
    { a4::vec4 res = a4v::sub(vf28, vf28); a4::q128 mask = a4v::set_epi32(-1, 0, 0, 0); vf28 = a4v::blend(vf28, res, a4v::castsi128_ps(mask)); }
    do { const a4::q128 _v = (A4_R128(ADD32((uint32_t)a1, 0))); h8 = a4::hi64(_v); std::memcpy(&t0, &_v, 8); } while (0);
    do { const a4::q128 _v = (A4_R128(ADD32((uint32_t)a1, 16))); h9 = a4::hi64(_v); std::memcpy(&t1, &_v, 8); } while (0);
    do { const a4::q128 _v = (A4_R128(ADD32((uint32_t)a1, 32))); h10 = a4::hi64(_v); std::memcpy(&t2, &_v, 8); } while (0);
    do { const a4::q128 _v = (a4v::castps_si128(vf0)); h11 = a4::hi64(_v); std::memcpy(&t3, &_v, 8); } while (0);
    do { const a4::q128 _v = (a4m::pextlw(a4::mk128(h9, t1), a4::mk128(h8, t0))); h12 = a4::hi64(_v); std::memcpy(&t4, &_v, 8); } while (0);
    do { const a4::q128 _v = (a4m::pextuw(a4::mk128(h9, t1), a4::mk128(h8, t0))); h13 = a4::hi64(_v); std::memcpy(&t5, &_v, 8); } while (0);
    do { const a4::q128 _v = (a4m::pextlw(a4::mk128(h11, t3), a4::mk128(h10, t2))); h14 = a4::hi64(_v); std::memcpy(&t6, &_v, 8); } while (0);
    do { const a4::q128 _v = (a4m::pextuw(a4::mk128(h11, t3), a4::mk128(h10, t2))); h15 = a4::hi64(_v); std::memcpy(&t7, &_v, 8); } while (0);
    do { const a4::q128 _v = (a4m::pcpyld(a4::mk128(h14, t6), a4::mk128(h12, t4))); h8 = a4::hi64(_v); std::memcpy(&t0, &_v, 8); } while (0);
    do { const a4::q128 _v = (a4v::unpackhi_epi64(a4::mk128(h12, t4), a4::mk128(h14, t6))); h9 = a4::hi64(_v); std::memcpy(&t1, &_v, 8); } while (0);
    do { const a4::q128 _v = (a4m::pcpyld(a4::mk128(h15, t7), a4::mk128(h13, t5))); h10 = a4::hi64(_v); std::memcpy(&t2, &_v, 8); } while (0);
    do { const a4::q128 _v = (a4v::unpackhi_epi64(a4::mk128(h13, t5), a4::mk128(h15, t7))); h11 = a4::hi64(_v); std::memcpy(&t3, &_v, 8); } while (0);
    vf24 = a4v::castsi128_ps(a4::mk128(h8, t0));
    vf25 = a4v::castsi128_ps(a4::mk128(h9, t1));
    vf26 = a4v::castsi128_ps(a4::mk128(h10, t2));
    { a4::vec4 res = a4v::mulK<0, 1>(vf24, a4v::bcast<0>(vf28)); vacc = a4v::dest<1, 1, 1, 1>(vacc, res); }
    { a4::vec4 mul_res = a4v::mulK<0, 1>(vf25, a4v::bcast<1>(vf28)); a4::vec4 res = a4v::addK<1, 1>(vacc, mul_res); vacc = a4v::dest<1, 1, 1, 1>(vacc, res); }
    { a4::vec4 mul_res = a4v::mulK<0, 1>(vf26, a4v::bcast<2>(vf28)); a4::vec4 res = a4v::addK<1, 1>(vacc, mul_res); vacc = a4v::dest<1, 1, 1, 1>(vacc, res); }
    { a4::vec4 mul_res = a4v::mulK<1, 1>(vf0, a4v::bcast<3>(vf28)); a4::vec4 res = a4v::addK<1, 1>(vacc, mul_res); a4::q128 mask = a4v::set_epi32(-1, -1, -1, -1); vf29 = a4v::blendv_ps(vf29, res, a4v::castsi128_ps(mask)); }
    { a4::vec4 res = a4v::subK<1, 1>(vf29, vf29); a4::q128 mask = a4v::set_epi32(-1, 0, 0, 0); vf29 = a4v::blend(vf29, res, a4v::castsi128_ps(mask)); }
    { a4::vec4 res = a4v::addK<1, 1>(vf29, vf0); a4::q128 mask = a4v::set_epi32(-1, 0, 0, 0); vf29 = a4v::blend(vf29, res, a4v::castsi128_ps(mask)); }
    A4_W128(ADD32((uint32_t)a2, 0), a4v::castps_si128(vf24));
    A4_W128(ADD32((uint32_t)a2, 16), a4v::castps_si128(vf25));
    A4_W128(ADD32((uint32_t)a2, 32), a4v::castps_si128(vf26));
    A4_W128(ADD32((uint32_t)a2, 48), a4v::castps_si128(vf29));
    { const uint32_t _dynInsn = A1_R32(0x1133228u); const int32_t _dynSimm = (int16_t)(_dynInsn & 0xFFFFu); const uint32_t _dynImm = _dynInsn & 0xFFFFu; (void)_dynSimm; (void)_dynImm; (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)_dynImm << 16)))); }
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)sp, 128))));
    { const uint32_t _dynInsn = A1_R32(0x1133230u); const int32_t _dynSimm = (int16_t)(_dynInsn & 0xFFFFu); const uint32_t _dynImm = _dynInsn & 0xFFFFu; (void)_dynSimm; (void)_dynImm; (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v0, _dynSimm)))); }
    vf8 = a4v::castsi128_ps(A4_R128(ADD32((uint32_t)v0, 0)));
    vf14 = a4v::castsi128_ps(A4_R128(ADD32((uint32_t)a2, 0)));
    vf15 = a4v::castsi128_ps(A4_R128(ADD32((uint32_t)a2, 16)));
    vf16 = a4v::castsi128_ps(A4_R128(ADD32((uint32_t)a2, 32)));
    vf17 = a4v::castsi128_ps(A4_R128(ADD32((uint32_t)a2, 48)));
    { a4::vec4 res = a4v::mul(vf14, a4v::bcast<0>(vf8)); vacc = a4v::dest<1, 1, 1, 1>(vacc, res); }
    { a4::vec4 mul_res = a4v::mul(vf15, a4v::bcast<1>(vf8)); a4::vec4 res = a4v::addK<1, 1>(vacc, mul_res); vacc = a4v::dest<1, 1, 1, 1>(vacc, res); }
    { a4::vec4 mul_res = a4v::mul(vf16, a4v::bcast<2>(vf8)); a4::vec4 res = a4v::addK<1, 1>(vacc, mul_res); vacc = a4v::dest<1, 1, 1, 1>(vacc, res); }
    { a4::vec4 mul_res = a4v::mul(vf17, a4v::bcast<3>(vf8)); a4::vec4 res = a4v::addK<1, 1>(vacc, mul_res); a4::q128 mask = a4v::set_epi32(-1, -1, -1, -1); vf10 = a4v::blendv_ps(vf10, res, a4v::castsi128_ps(mask)); }
    A4_W128(ADD32((uint32_t)a0, 0), a4v::castps_si128(vf10));
    { const uint32_t _dynInsn = A1_R32(0x113325Cu); const int32_t _dynSimm = (int16_t)(_dynInsn & 0xFFFFu); const uint32_t _dynImm = _dynInsn & 0xFFFFu; (void)_dynSimm; (void)_dynImm; (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)_dynImm << 16)))); }
    { uint32_t bits = A1_R32B((uint32_t)sp, 132); float f; std::memcpy(&f, &bits, sizeof(f)); f2 = f; }
    { uint32_t bits = 0u; std::memcpy(&f0, &bits, sizeof(bits)); }
    fcr31 = (a1f::lt(f2, f0)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
    { const bool c = (fcr31 & 0x800000u) == 0u; { const uint32_t _dynInsn = A1_R32(0x1133270u); const int32_t _dynSimm = (int16_t)(_dynInsn & 0xFFFFu); const uint32_t _dynImm = _dynInsn & 0xFFFFu; (void)_dynSimm; (void)_dynImm; (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v1, _dynSimm)))); } if (c) goto L_1133298; }
    (at = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)48948 << 16))));
    (at = (uint64_t)(RC_U64X(at) | (uint64_t)(uint16_t)65011));
    { uint32_t bits = (uint32_t)at; std::memcpy(&f0, &bits, sizeof(bits)); }
    fcr31 = (a1f::lt(f2, f0)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
    if ((fcr31 & 0x800000u) != 0u) goto L_11332b0;
    goto L_113330c;
L_1133298:;
    (at = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)16180 << 16))));
    (at = (uint64_t)(RC_U64X(at) | (uint64_t)(uint16_t)65011));
    { uint32_t bits = (uint32_t)at; std::memcpy(&f0, &bits, sizeof(bits)); }
    fcr31 = (a1f::lt(f0, f2)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
    if ((fcr31 & 0x800000u) == 0u) goto L_113330c;
L_11332b0:;
    vf8 = a4v::castsi128_ps(A4_R128(ADD32((uint32_t)v1, 0)));
    vf14 = a4v::castsi128_ps(A4_R128(ADD32((uint32_t)a2, 0)));
    vf15 = a4v::castsi128_ps(A4_R128(ADD32((uint32_t)a2, 16)));
    vf16 = a4v::castsi128_ps(A4_R128(ADD32((uint32_t)a2, 32)));
    vf17 = a4v::castsi128_ps(A4_R128(ADD32((uint32_t)a2, 48)));
    { a4::vec4 res = a4v::mul(vf14, a4v::bcast<0>(vf8)); vacc = a4v::dest<1, 1, 1, 1>(vacc, res); }
    { a4::vec4 mul_res = a4v::mul(vf15, a4v::bcast<1>(vf8)); a4::vec4 res = a4v::addK<1, 1>(vacc, mul_res); vacc = a4v::dest<1, 1, 1, 1>(vacc, res); }
    { a4::vec4 mul_res = a4v::mul(vf16, a4v::bcast<2>(vf8)); a4::vec4 res = a4v::addK<1, 1>(vacc, mul_res); vacc = a4v::dest<1, 1, 1, 1>(vacc, res); }
    { a4::vec4 mul_res = a4v::mul(vf17, a4v::bcast<3>(vf8)); a4::vec4 res = a4v::addK<1, 1>(vacc, mul_res); a4::q128 mask = a4v::set_epi32(-1, -1, -1, -1); vf10 = a4v::blendv_ps(vf10, res, a4v::castsi128_ps(mask)); }
    A4_W128(ADD32((uint32_t)a0, 0), a4v::castps_si128(vf10));
    { uint32_t bits = 0u; std::memcpy(&f0, &bits, sizeof(bits)); }
    (at = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)49024 << 16))));
    { uint32_t bits = (uint32_t)at; std::memcpy(&f1, &bits, sizeof(bits)); }
    fcr31 = (a1f::lt(f2, f0)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
    if ((fcr31 & 0x800000u) != 0u) goto L_11332f8;
    (at = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)16256 << 16))));
    { uint32_t bits = (uint32_t)at; std::memcpy(&f1, &bits, sizeof(bits)); }
L_11332f8:;
    { uint32_t bits; std::memcpy(&bits, &f1, sizeof(bits)); (v0 = (uint64_t)((int64_t)(int32_t)(bits))); }
    vf5 = a4v::castsi128_ps(A4_R128(ADD32((uint32_t)a0, 0)));
    vf6 = a4v::castsi128_ps(a4::mk128(h2, v0));
    { a4::vec4 res = a4v::mul(vf5, a4v::bcast<0>(vf6)); a4::q128 mask = a4v::set_epi32(0, -1, -1, -1); vf5 = a4v::blendv_ps(vf5, res, a4v::castsi128_ps(mask)); }
    A4_W128(ADD32((uint32_t)a0, 0), a4v::castps_si128(vf5));
L_113330c:;
    (at = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)49024 << 16))));
    { uint32_t bits = (uint32_t)at; std::memcpy(&f0, &bits, sizeof(bits)); }
    A1_W32B((uint32_t)sp, 132, 0u);
    { uint32_t bits; std::memcpy(&bits, &f0, sizeof(bits)); (v0 = (uint64_t)((int64_t)(int32_t)(bits))); }
    vf5 = a4v::castsi128_ps(A4_R128(ADD32((uint32_t)a0, 0)));
    vf6 = a4v::castsi128_ps(a4::mk128(h2, v0));
    { a4::vec4 res = a4v::mul(vf5, a4v::bcast<0>(vf6)); a4::q128 mask = a4v::set_epi32(0, -1, -1, -1); vf5 = a4v::blendv_ps(vf5, res, a4v::castsi128_ps(mask)); }
    A4_W128(ADD32((uint32_t)a0, 0), a4v::castps_si128(vf5));
    vf5 = a4v::castsi128_ps(A4_R128(ADD32((uint32_t)a0, 0)));
    { a4::vec4 res = a4v::mul(vf5, vf5); a4::q128 mask = a4v::set_epi32(0, -1, -1, -1); vf7 = a4v::blend(vf7, res, a4v::castsi128_ps(mask)); }
    { a4::vec4 res = a4v::mulK<1, 1>(vf0, a4v::bcast<0>(vf7)); vacc = a4v::dest<0, 0, 0, 1>(vacc, res); }
    { a4::vec4 mul_res = a4v::mulK<1, 1>(vf0, a4v::bcast<1>(vf7)); a4::vec4 res = a4v::addK<1, 1>(vacc, mul_res); vacc = a4v::dest<0, 0, 0, 1>(vacc, res); }
    { a4::vec4 mul_res = a4v::mulK<1, 1>(vf0, a4v::bcast<2>(vf7)); a4::vec4 res = a4v::addK<1, 1>(vacc, mul_res); a4::q128 mask = a4v::set_epi32(-1, 0, 0, 0); vf7 = a4v::blendv_ps(vf7, res, a4v::castsi128_ps(mask)); }
    { float fs = a4v::cvtss_f32(a4v::shuffle_ps<0x03>(vf0, vf0)); float ft = a4v::cvtss_f32(a4v::shuffle_ps<0x03>(vf7, vf7)); vq = a4v::rsqrt(vstat, fs, ft); }
    { a4::vec4 res = a4v::mul(vf5, a4v::set1_ps(vq)); a4::q128 mask = a4v::set_epi32(0, -1, -1, -1); vf5 = a4v::blendv_ps(vf5, res, a4v::castsi128_ps(mask)); }
    A4_W128(ADD32((uint32_t)a0, 0), a4v::castps_si128(vf5));
    { const uint32_t _dynInsn = A1_R32(0x1133350u); const int32_t _dynSimm = (int16_t)(_dynInsn & 0xFFFFu); const uint32_t _dynImm = _dynInsn & 0xFFFFu; (void)_dynSimm; (void)_dynImm; (a1 = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)_dynImm << 16)))); }
    (a0 = (uint64_t)((uint64_t)sp + (uint64_t)0ULL));
    { { const uint32_t _dynInsn = A1_R32(0x113335Cu); const int32_t _dynSimm = (int16_t)(_dynInsn & 0xFFFFu); const uint32_t _dynImm = _dynInsn & 0xFFFFu; (void)_dynSimm; (void)_dynImm; (a1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)a1, _dynSimm)))); } A3_CHARGE(); iosCopyMatrix(ram, rt, (uint32_t)a0, (uint32_t)a1, h6, h7, h8, h9); }
    { uint32_t bits = A1_R32B((uint32_t)sp, 128); float f; std::memcpy(&f, &bits, sizeof(f)); f0 = f; }
    { uint32_t bits = A1_R32B((uint32_t)sp, 136); float f; std::memcpy(&f, &bits, sizeof(f)); f1 = f; }
    f2 = (-(float)(f0));
    { float f = f0; uint32_t bits; std::memcpy(&bits, &f, sizeof(bits)); A1_W32B((uint32_t)sp, 32, bits); }
    { float f = f1; uint32_t bits; std::memcpy(&bits, &f, sizeof(bits)); A1_W32B((uint32_t)sp, 0, bits); }
    { float f = f1; uint32_t bits; std::memcpy(&bits, &f, sizeof(bits)); A1_W32B((uint32_t)sp, 40, bits); }
    { float f = f2; uint32_t bits; std::memcpy(&bits, &f, sizeof(bits)); A1_W32B((uint32_t)sp, 8, bits); }
    vf8 = a4v::castsi128_ps(A4_R128(ADD32((uint32_t)s0, 0)));
    vf14 = a4v::castsi128_ps(A4_R128(ADD32((uint32_t)sp, 0)));
    vf15 = a4v::castsi128_ps(A4_R128(ADD32((uint32_t)sp, 16)));
    vf16 = a4v::castsi128_ps(A4_R128(ADD32((uint32_t)sp, 32)));
    vf17 = a4v::castsi128_ps(A4_R128(ADD32((uint32_t)sp, 48)));
    { a4::vec4 res = a4v::mul(vf14, a4v::bcast<0>(vf8)); vacc = a4v::dest<1, 1, 1, 1>(vacc, res); }
    { a4::vec4 mul_res = a4v::mul(vf15, a4v::bcast<1>(vf8)); a4::vec4 res = a4v::addK<1, 1>(vacc, mul_res); vacc = a4v::dest<1, 1, 1, 1>(vacc, res); }
    { a4::vec4 mul_res = a4v::mul(vf16, a4v::bcast<2>(vf8)); a4::vec4 res = a4v::addK<1, 1>(vacc, mul_res); vacc = a4v::dest<1, 1, 1, 1>(vacc, res); }
    { a4::vec4 mul_res = a4v::mul(vf17, a4v::bcast<3>(vf8)); a4::vec4 res = a4v::addK<1, 1>(vacc, mul_res); a4::q128 mask = a4v::set_epi32(-1, -1, -1, -1); vf10 = a4v::blendv_ps(vf10, res, a4v::castsi128_ps(mask)); }
    A4_W128(ADD32((uint32_t)s1, 0), a4v::castps_si128(vf10));
    { (sp = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)sp, 176)))); io_vf5 = vf5; io_vf6 = vf6; io_vf7 = vf7; io_vf8 = vf8; io_vf10 = vf10; io_vf14 = vf14; io_vf15 = vf15; io_vf16 = vf16; io_vf17 = vf17; io_vf24 = vf24; io_vf25 = vf25; io_vf26 = vf26; io_vf28 = vf28; io_vf29 = vf29; io_vacc = vacc; io_vq = vq; io_vstat = vstat; io_h6 = h6; io_h7 = h7; io_h8 = h8; io_h9 = h9; io_h10 = h10; io_h11 = h11; io_h12 = h12; io_h13 = h13; io_h14 = h14; io_h15 = h15; return a1::RetVF{(uint64_t)v0, f0}; }
    __builtin_unreachable();
}

// GetSpaceTransformMatrix (0x1133948): typed by tools/typed_lift2.py (round A6; guest frame calls; VU0 in vf0 vf10 vf12 vf13 vf14 vf15 vf16 vf17 vf25 vf28 vf29 vacc, out vf4 vf10 vf11 vf12 vf13 vf14 vf15 vf16 vf17 vf24 vf25 vf26 vf27 vf28 vf29 vacc h8 h9 h10 h11 h12 h13 h14 h15, relocated calls 2)
inline a1::RetVF GetSpaceTransformMatrix(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint64_t a0, uint64_t a1, uint64_t a2, uint64_t a3, uint64_t t0, uint64_t t1, uint64_t t2, uint64_t sp, float f12, float f13, float f14, float f15, float f16, float f17, float f18, float f19, const a4::vec4 &io_vf0, a4::vec4 &io_vf4, a4::vec4 &io_vf10, a4::vec4 &io_vf11, a4::vec4 &io_vf12, a4::vec4 &io_vf13, a4::vec4 &io_vf14, a4::vec4 &io_vf15, a4::vec4 &io_vf16, a4::vec4 &io_vf17, a4::vec4 &io_vf24, a4::vec4 &io_vf25, a4::vec4 &io_vf26, a4::vec4 &io_vf27, a4::vec4 &io_vf28, a4::vec4 &io_vf29, a4::vec4 &io_vacc, uint64_t &io_h8, uint64_t &io_h9, uint64_t &io_h10, uint64_t &io_h11, uint64_t &io_h12, uint64_t &io_h13, uint64_t &io_h14, uint64_t &io_h15)
{
    float f0 = 0.0f;
    uint64_t v0 = 0;
    uint64_t v1 = 0;
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
    a4::vec4 vf0 = io_vf0;
    a4::vec4 vf4 = io_vf4;
    a4::vec4 vf10 = io_vf10;
    a4::vec4 vf11 = io_vf11;
    a4::vec4 vf12 = io_vf12;
    a4::vec4 vf13 = io_vf13;
    a4::vec4 vf14 = io_vf14;
    a4::vec4 vf15 = io_vf15;
    a4::vec4 vf16 = io_vf16;
    a4::vec4 vf17 = io_vf17;
    a4::vec4 vf24 = io_vf24;
    a4::vec4 vf25 = io_vf25;
    a4::vec4 vf26 = io_vf26;
    a4::vec4 vf27 = io_vf27;
    a4::vec4 vf28 = io_vf28;
    a4::vec4 vf29 = io_vf29;
    a4::vec4 vacc = io_vacc;
    uint64_t h8 = io_h8;
    uint64_t h9 = io_h9;
    uint64_t h10 = io_h10;
    uint64_t h11 = io_h11;
    uint64_t h12 = io_h12;
    uint64_t h13 = io_h13;
    uint64_t h14 = io_h14;
    uint64_t h15 = io_h15;
    (sp = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)sp, 4294967008))));
    (v0 = (uint64_t)((uint64_t)a1 + (uint64_t)0ULL));
    (s5 = (uint64_t)((uint64_t)a0 + (uint64_t)0ULL));
    (a1 = (uint64_t)((uint64_t)a2 + (uint64_t)0ULL));
    (a2 = (uint64_t)((uint64_t)v0 + (uint64_t)0ULL));
    (a0 = (uint64_t)((uint64_t)RC_U64X(sp) + (uint64_t)0ULL));
    (s2 = (uint64_t)((uint64_t)t1 + (uint64_t)0ULL));
    (s3 = (uint64_t)((uint64_t)t2 + (uint64_t)0ULL));
    (s4 = (uint64_t)((uint64_t)t0 + (uint64_t)0ULL));
    { (s1 = (uint64_t)((uint64_t)a3 + (uint64_t)0ULL)); A3_CHARGE(); f0 = a9x::axisDirZY(ram, rt, (uint32_t)a0, (uint32_t)a1, (uint32_t)a2); }
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)sp, 32))));
    { a4::vec4 res = a4v::addK<1, 1>(vf0, a4v::bcast<3>(vf0)); a4::q128 mask = a4v::set_epi32(0, -1, -1, -1); vf12 = a4v::blendv_ps(vf12, res, a4v::castsi128_ps(mask)); }
    vf11 = a4v::castsi128_ps(A4_R128(ADD32((uint32_t)sp, 0)));
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
    A4_W128(ADD32((uint32_t)v0, 0), a4v::castps_si128(vf14));
    A4_W128(ADD32((uint32_t)v0, 16), a4v::castps_si128(vf15));
    A4_W128(ADD32((uint32_t)v0, 32), a4v::castps_si128(vf16));
    vf4 = a4v::castsi128_ps(A4_R128(ADD32((uint32_t)s1, 0)));
    { a4::q128 mask = a4v::set_epi32(-1, 0, 0, 0); vf4 = a4v::blendv_ps(vf4, vf0, a4v::castsi128_ps(mask)); }
    A4_W128(ADD32((uint32_t)v0, 48), a4v::castps_si128(vf4));
    (s0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)sp, 16))));
    (a1 = (uint64_t)((uint64_t)s2 + (uint64_t)0ULL));
    (a2 = (uint64_t)((uint64_t)s4 + (uint64_t)0ULL));
    { (a0 = (uint64_t)((uint64_t)RC_U64X(s0) + (uint64_t)0ULL)); A3_CHARGE(); f0 = a9x::axisDirZY(ram, rt, (uint32_t)a0, (uint32_t)a1, (uint32_t)a2); }
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)sp, 96))));
    { a4::vec4 res = a4v::addK<1, 1>(vf0, a4v::bcast<3>(vf0)); a4::q128 mask = a4v::set_epi32(0, -1, -1, -1); vf12 = a4v::blendv_ps(vf12, res, a4v::castsi128_ps(mask)); }
    vf11 = a4v::castsi128_ps(A4_R128(ADD32((uint32_t)s0, 0)));
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
    A4_W128(ADD32((uint32_t)v0, 0), a4v::castps_si128(vf14));
    A4_W128(ADD32((uint32_t)v0, 16), a4v::castps_si128(vf15));
    A4_W128(ADD32((uint32_t)v0, 32), a4v::castps_si128(vf16));
    vf4 = a4v::castsi128_ps(A4_R128(ADD32((uint32_t)s3, 0)));
    { a4::q128 mask = a4v::set_epi32(-1, 0, 0, 0); vf4 = a4v::blendv_ps(vf4, vf0, a4v::castsi128_ps(mask)); }
    A4_W128(ADD32((uint32_t)v0, 48), a4v::castps_si128(vf4));
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)sp, 160))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)sp, 32))));
    vf24 = a4v::castsi128_ps(A4_R128(ADD32((uint32_t)v0, 48)));
    { a4::vec4 res = a4v::subK<1, 1>(vf0, a4v::bcast<3>(vf0)); a4::q128 mask = a4v::set_epi32(0, 0, 0, -1); vf25 = a4v::blendv_ps(vf25, res, a4v::castsi128_ps(mask)); }
    { a4::vec4 res = a4v::mulK<0, 1>(vf24, a4v::bcast<0>(vf25)); a4::q128 mask = a4v::set_epi32(0, -1, -1, -1); vf28 = a4v::blendv_ps(vf28, res, a4v::castsi128_ps(mask)); }
    { a4::vec4 res = a4v::sub(vf28, vf28); a4::q128 mask = a4v::set_epi32(-1, 0, 0, 0); vf28 = a4v::blend(vf28, res, a4v::castsi128_ps(mask)); }
    do { const a4::q128 _v = (A4_R128(ADD32((uint32_t)v0, 0))); h8 = a4::hi64(_v); std::memcpy(&t0, &_v, 8); } while (0);
    do { const a4::q128 _v = (A4_R128(ADD32((uint32_t)v0, 16))); h9 = a4::hi64(_v); std::memcpy(&t1, &_v, 8); } while (0);
    do { const a4::q128 _v = (A4_R128(ADD32((uint32_t)v0, 32))); h10 = a4::hi64(_v); std::memcpy(&t2, &_v, 8); } while (0);
    do { const a4::q128 _v = (a4v::castps_si128(vf0)); h11 = a4::hi64(_v); std::memcpy(&t3, &_v, 8); } while (0);
    do { const a4::q128 _v = (a4m::pextlw(a4::mk128(h9, t1), a4::mk128(h8, t0))); h12 = a4::hi64(_v); std::memcpy(&t4, &_v, 8); } while (0);
    do { const a4::q128 _v = (a4m::pextuw(a4::mk128(h9, t1), a4::mk128(h8, t0))); h13 = a4::hi64(_v); std::memcpy(&t5, &_v, 8); } while (0);
    do { const a4::q128 _v = (a4m::pextlw(a4::mk128(h11, t3), a4::mk128(h10, t2))); h14 = a4::hi64(_v); std::memcpy(&t6, &_v, 8); } while (0);
    do { const a4::q128 _v = (a4m::pextuw(a4::mk128(h11, t3), a4::mk128(h10, t2))); h15 = a4::hi64(_v); std::memcpy(&t7, &_v, 8); } while (0);
    do { const a4::q128 _v = (a4m::pcpyld(a4::mk128(h14, t6), a4::mk128(h12, t4))); h8 = a4::hi64(_v); std::memcpy(&t0, &_v, 8); } while (0);
    do { const a4::q128 _v = (a4v::unpackhi_epi64(a4::mk128(h12, t4), a4::mk128(h14, t6))); h9 = a4::hi64(_v); std::memcpy(&t1, &_v, 8); } while (0);
    do { const a4::q128 _v = (a4m::pcpyld(a4::mk128(h15, t7), a4::mk128(h13, t5))); h10 = a4::hi64(_v); std::memcpy(&t2, &_v, 8); } while (0);
    do { const a4::q128 _v = (a4v::unpackhi_epi64(a4::mk128(h13, t5), a4::mk128(h15, t7))); h11 = a4::hi64(_v); std::memcpy(&t3, &_v, 8); } while (0);
    vf24 = a4v::castsi128_ps(a4::mk128(h8, t0));
    vf25 = a4v::castsi128_ps(a4::mk128(h9, t1));
    vf26 = a4v::castsi128_ps(a4::mk128(h10, t2));
    { a4::vec4 res = a4v::mulK<0, 1>(vf24, a4v::bcast<0>(vf28)); vacc = a4v::dest<1, 1, 1, 1>(vacc, res); }
    { a4::vec4 mul_res = a4v::mulK<0, 1>(vf25, a4v::bcast<1>(vf28)); a4::vec4 res = a4v::addK<1, 1>(vacc, mul_res); vacc = a4v::dest<1, 1, 1, 1>(vacc, res); }
    { a4::vec4 mul_res = a4v::mulK<0, 1>(vf26, a4v::bcast<2>(vf28)); a4::vec4 res = a4v::addK<1, 1>(vacc, mul_res); vacc = a4v::dest<1, 1, 1, 1>(vacc, res); }
    { a4::vec4 mul_res = a4v::mulK<1, 1>(vf0, a4v::bcast<3>(vf28)); a4::vec4 res = a4v::addK<1, 1>(vacc, mul_res); a4::q128 mask = a4v::set_epi32(-1, -1, -1, -1); vf29 = a4v::blendv_ps(vf29, res, a4v::castsi128_ps(mask)); }
    { a4::vec4 res = a4v::subK<1, 1>(vf29, vf29); a4::q128 mask = a4v::set_epi32(-1, 0, 0, 0); vf29 = a4v::blend(vf29, res, a4v::castsi128_ps(mask)); }
    { a4::vec4 res = a4v::addK<1, 1>(vf29, vf0); a4::q128 mask = a4v::set_epi32(-1, 0, 0, 0); vf29 = a4v::blend(vf29, res, a4v::castsi128_ps(mask)); }
    A4_W128(ADD32((uint32_t)a0, 0), a4v::castps_si128(vf24));
    A4_W128(ADD32((uint32_t)a0, 16), a4v::castps_si128(vf25));
    A4_W128(ADD32((uint32_t)a0, 32), a4v::castps_si128(vf26));
    A4_W128(ADD32((uint32_t)a0, 48), a4v::castps_si128(vf29));
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)sp, 96))));
    vf14 = a4v::castsi128_ps(A4_R128(ADD32((uint32_t)a0, 0)));
    vf15 = a4v::castsi128_ps(A4_R128(ADD32((uint32_t)a0, 16)));
    vf16 = a4v::castsi128_ps(A4_R128(ADD32((uint32_t)a0, 32)));
    vf17 = a4v::castsi128_ps(A4_R128(ADD32((uint32_t)a0, 48)));
    vf24 = a4v::castsi128_ps(A4_R128(ADD32((uint32_t)v1, 0)));
    vf25 = a4v::castsi128_ps(A4_R128(ADD32((uint32_t)v1, 16)));
    vf26 = a4v::castsi128_ps(A4_R128(ADD32((uint32_t)v1, 32)));
    vf27 = a4v::castsi128_ps(A4_R128(ADD32((uint32_t)v1, 48)));
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
    A4_W128(ADD32((uint32_t)s5, 0), a4v::castps_si128(vf24));
    A4_W128(ADD32((uint32_t)s5, 16), a4v::castps_si128(vf25));
    A4_W128(ADD32((uint32_t)s5, 32), a4v::castps_si128(vf26));
    A4_W128(ADD32((uint32_t)s5, 48), a4v::castps_si128(vf27));
    { (sp = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)sp, 288)))); io_vf4 = vf4; io_vf10 = vf10; io_vf11 = vf11; io_vf12 = vf12; io_vf13 = vf13; io_vf14 = vf14; io_vf15 = vf15; io_vf16 = vf16; io_vf17 = vf17; io_vf24 = vf24; io_vf25 = vf25; io_vf26 = vf26; io_vf27 = vf27; io_vf28 = vf28; io_vf29 = vf29; io_vacc = vacc; io_h8 = h8; io_h9 = h9; io_h10 = h10; io_h11 = h11; io_h12 = h12; io_h13 = h13; io_h14 = h14; io_h15 = h15; return a1::RetVF{(uint64_t)v0, f0}; }
    __builtin_unreachable();
}

// GetSpaceTransformRotMatrix (0x1133bb0): typed by tools/typed_lift2.py (round A6; guest frame calls; VU0 in vf0 vf10 vf12 vf13 vf14 vf15 vf16 vf17 vf25 vf28 vf29 vacc, out vf10 vf11 vf12 vf13 vf14 vf15 vf16 vf17 vf24 vf25 vf26 vf27 vf28 vf29 vacc h8 h9 h10 h11 h12 h13 h14 h15, relocated calls 2)
inline a1::RetVF GetSpaceTransformRotMatrix(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint64_t a0, uint64_t a1, uint64_t a2, uint64_t a3, uint64_t t0, uint64_t sp, float f12, float f13, float f14, float f15, float f16, float f17, float f18, float f19, const a4::vec4 &io_vf0, a4::vec4 &io_vf10, a4::vec4 &io_vf11, a4::vec4 &io_vf12, a4::vec4 &io_vf13, a4::vec4 &io_vf14, a4::vec4 &io_vf15, a4::vec4 &io_vf16, a4::vec4 &io_vf17, a4::vec4 &io_vf24, a4::vec4 &io_vf25, a4::vec4 &io_vf26, a4::vec4 &io_vf27, a4::vec4 &io_vf28, a4::vec4 &io_vf29, a4::vec4 &io_vacc, uint64_t &io_h8, uint64_t &io_h9, uint64_t &io_h10, uint64_t &io_h11, uint64_t &io_h12, uint64_t &io_h13, uint64_t &io_h14, uint64_t &io_h15)
{
    float f0 = 0.0f;
    uint64_t v0 = 0;
    uint64_t v1 = 0;
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
    a4::vec4 vf0 = io_vf0;
    a4::vec4 vf10 = io_vf10;
    a4::vec4 vf11 = io_vf11;
    a4::vec4 vf12 = io_vf12;
    a4::vec4 vf13 = io_vf13;
    a4::vec4 vf14 = io_vf14;
    a4::vec4 vf15 = io_vf15;
    a4::vec4 vf16 = io_vf16;
    a4::vec4 vf17 = io_vf17;
    a4::vec4 vf24 = io_vf24;
    a4::vec4 vf25 = io_vf25;
    a4::vec4 vf26 = io_vf26;
    a4::vec4 vf27 = io_vf27;
    a4::vec4 vf28 = io_vf28;
    a4::vec4 vf29 = io_vf29;
    a4::vec4 vacc = io_vacc;
    uint64_t h8 = io_h8;
    uint64_t h9 = io_h9;
    uint64_t h10 = io_h10;
    uint64_t h11 = io_h11;
    uint64_t h12 = io_h12;
    uint64_t h13 = io_h13;
    uint64_t h14 = io_h14;
    uint64_t h15 = io_h15;
    (sp = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)sp, 4294967024))));
    (v0 = (uint64_t)((uint64_t)a1 + (uint64_t)0ULL));
    (s3 = (uint64_t)((uint64_t)a0 + (uint64_t)0ULL));
    (a1 = (uint64_t)((uint64_t)a2 + (uint64_t)0ULL));
    (a2 = (uint64_t)((uint64_t)v0 + (uint64_t)0ULL));
    (a0 = (uint64_t)((uint64_t)RC_U64X(sp) + (uint64_t)0ULL));
    (s2 = (uint64_t)((uint64_t)a3 + (uint64_t)0ULL));
    { (s1 = (uint64_t)((uint64_t)t0 + (uint64_t)0ULL)); A3_CHARGE(); f0 = a9x::axisDirZY(ram, rt, (uint32_t)a0, (uint32_t)a1, (uint32_t)a2); }
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)sp, 32))));
    { a4::vec4 res = a4v::addK<1, 1>(vf0, a4v::bcast<3>(vf0)); a4::q128 mask = a4v::set_epi32(0, -1, -1, -1); vf12 = a4v::blendv_ps(vf12, res, a4v::castsi128_ps(mask)); }
    vf11 = a4v::castsi128_ps(A4_R128(ADD32((uint32_t)sp, 0)));
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
    A4_W128(ADD32((uint32_t)v1, 0), a4v::castps_si128(vf14));
    A4_W128(ADD32((uint32_t)v1, 16), a4v::castps_si128(vf15));
    A4_W128(ADD32((uint32_t)v1, 32), a4v::castps_si128(vf16));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)sp, 80))));
    A4_W128(ADD32((uint32_t)v0, 0), a4v::castps_si128(vf0));
    (s0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)sp, 16))));
    (a1 = (uint64_t)((uint64_t)s1 + (uint64_t)0ULL));
    (a2 = (uint64_t)((uint64_t)s2 + (uint64_t)0ULL));
    { (a0 = (uint64_t)((uint64_t)RC_U64X(s0) + (uint64_t)0ULL)); A3_CHARGE(); f0 = a9x::axisDirZY(ram, rt, (uint32_t)a0, (uint32_t)a1, (uint32_t)a2); }
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)sp, 96))));
    { a4::vec4 res = a4v::addK<1, 1>(vf0, a4v::bcast<3>(vf0)); a4::q128 mask = a4v::set_epi32(0, -1, -1, -1); vf12 = a4v::blendv_ps(vf12, res, a4v::castsi128_ps(mask)); }
    vf11 = a4v::castsi128_ps(A4_R128(ADD32((uint32_t)s0, 0)));
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
    A4_W128(ADD32((uint32_t)v1, 0), a4v::castps_si128(vf14));
    A4_W128(ADD32((uint32_t)v1, 16), a4v::castps_si128(vf15));
    A4_W128(ADD32((uint32_t)v1, 32), a4v::castps_si128(vf16));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)sp, 144))));
    A4_W128(ADD32((uint32_t)v0, 0), a4v::castps_si128(vf0));
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)sp, 160))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)sp, 32))));
    vf24 = a4v::castsi128_ps(A4_R128(ADD32((uint32_t)v0, 48)));
    { a4::vec4 res = a4v::subK<1, 1>(vf0, a4v::bcast<3>(vf0)); a4::q128 mask = a4v::set_epi32(0, 0, 0, -1); vf25 = a4v::blendv_ps(vf25, res, a4v::castsi128_ps(mask)); }
    { a4::vec4 res = a4v::mulK<0, 1>(vf24, a4v::bcast<0>(vf25)); a4::q128 mask = a4v::set_epi32(0, -1, -1, -1); vf28 = a4v::blendv_ps(vf28, res, a4v::castsi128_ps(mask)); }
    { a4::vec4 res = a4v::sub(vf28, vf28); a4::q128 mask = a4v::set_epi32(-1, 0, 0, 0); vf28 = a4v::blend(vf28, res, a4v::castsi128_ps(mask)); }
    do { const a4::q128 _v = (A4_R128(ADD32((uint32_t)v0, 0))); h8 = a4::hi64(_v); std::memcpy(&t0, &_v, 8); } while (0);
    do { const a4::q128 _v = (A4_R128(ADD32((uint32_t)v0, 16))); h9 = a4::hi64(_v); std::memcpy(&t1, &_v, 8); } while (0);
    do { const a4::q128 _v = (A4_R128(ADD32((uint32_t)v0, 32))); h10 = a4::hi64(_v); std::memcpy(&t2, &_v, 8); } while (0);
    do { const a4::q128 _v = (a4v::castps_si128(vf0)); h11 = a4::hi64(_v); std::memcpy(&t3, &_v, 8); } while (0);
    do { const a4::q128 _v = (a4m::pextlw(a4::mk128(h9, t1), a4::mk128(h8, t0))); h12 = a4::hi64(_v); std::memcpy(&t4, &_v, 8); } while (0);
    do { const a4::q128 _v = (a4m::pextuw(a4::mk128(h9, t1), a4::mk128(h8, t0))); h13 = a4::hi64(_v); std::memcpy(&t5, &_v, 8); } while (0);
    do { const a4::q128 _v = (a4m::pextlw(a4::mk128(h11, t3), a4::mk128(h10, t2))); h14 = a4::hi64(_v); std::memcpy(&t6, &_v, 8); } while (0);
    do { const a4::q128 _v = (a4m::pextuw(a4::mk128(h11, t3), a4::mk128(h10, t2))); h15 = a4::hi64(_v); std::memcpy(&t7, &_v, 8); } while (0);
    do { const a4::q128 _v = (a4m::pcpyld(a4::mk128(h14, t6), a4::mk128(h12, t4))); h8 = a4::hi64(_v); std::memcpy(&t0, &_v, 8); } while (0);
    do { const a4::q128 _v = (a4v::unpackhi_epi64(a4::mk128(h12, t4), a4::mk128(h14, t6))); h9 = a4::hi64(_v); std::memcpy(&t1, &_v, 8); } while (0);
    do { const a4::q128 _v = (a4m::pcpyld(a4::mk128(h15, t7), a4::mk128(h13, t5))); h10 = a4::hi64(_v); std::memcpy(&t2, &_v, 8); } while (0);
    do { const a4::q128 _v = (a4v::unpackhi_epi64(a4::mk128(h13, t5), a4::mk128(h15, t7))); h11 = a4::hi64(_v); std::memcpy(&t3, &_v, 8); } while (0);
    vf24 = a4v::castsi128_ps(a4::mk128(h8, t0));
    vf25 = a4v::castsi128_ps(a4::mk128(h9, t1));
    vf26 = a4v::castsi128_ps(a4::mk128(h10, t2));
    { a4::vec4 res = a4v::mulK<0, 1>(vf24, a4v::bcast<0>(vf28)); vacc = a4v::dest<1, 1, 1, 1>(vacc, res); }
    { a4::vec4 mul_res = a4v::mulK<0, 1>(vf25, a4v::bcast<1>(vf28)); a4::vec4 res = a4v::addK<1, 1>(vacc, mul_res); vacc = a4v::dest<1, 1, 1, 1>(vacc, res); }
    { a4::vec4 mul_res = a4v::mulK<0, 1>(vf26, a4v::bcast<2>(vf28)); a4::vec4 res = a4v::addK<1, 1>(vacc, mul_res); vacc = a4v::dest<1, 1, 1, 1>(vacc, res); }
    { a4::vec4 mul_res = a4v::mulK<1, 1>(vf0, a4v::bcast<3>(vf28)); a4::vec4 res = a4v::addK<1, 1>(vacc, mul_res); a4::q128 mask = a4v::set_epi32(-1, -1, -1, -1); vf29 = a4v::blendv_ps(vf29, res, a4v::castsi128_ps(mask)); }
    { a4::vec4 res = a4v::subK<1, 1>(vf29, vf29); a4::q128 mask = a4v::set_epi32(-1, 0, 0, 0); vf29 = a4v::blend(vf29, res, a4v::castsi128_ps(mask)); }
    { a4::vec4 res = a4v::addK<1, 1>(vf29, vf0); a4::q128 mask = a4v::set_epi32(-1, 0, 0, 0); vf29 = a4v::blend(vf29, res, a4v::castsi128_ps(mask)); }
    A4_W128(ADD32((uint32_t)a0, 0), a4v::castps_si128(vf24));
    A4_W128(ADD32((uint32_t)a0, 16), a4v::castps_si128(vf25));
    A4_W128(ADD32((uint32_t)a0, 32), a4v::castps_si128(vf26));
    A4_W128(ADD32((uint32_t)a0, 48), a4v::castps_si128(vf29));
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)sp, 96))));
    vf14 = a4v::castsi128_ps(A4_R128(ADD32((uint32_t)a0, 0)));
    vf15 = a4v::castsi128_ps(A4_R128(ADD32((uint32_t)a0, 16)));
    vf16 = a4v::castsi128_ps(A4_R128(ADD32((uint32_t)a0, 32)));
    vf17 = a4v::castsi128_ps(A4_R128(ADD32((uint32_t)a0, 48)));
    vf24 = a4v::castsi128_ps(A4_R128(ADD32((uint32_t)v1, 0)));
    vf25 = a4v::castsi128_ps(A4_R128(ADD32((uint32_t)v1, 16)));
    vf26 = a4v::castsi128_ps(A4_R128(ADD32((uint32_t)v1, 32)));
    vf27 = a4v::castsi128_ps(A4_R128(ADD32((uint32_t)v1, 48)));
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
    A4_W128(ADD32((uint32_t)s3, 0), a4v::castps_si128(vf24));
    A4_W128(ADD32((uint32_t)s3, 16), a4v::castps_si128(vf25));
    A4_W128(ADD32((uint32_t)s3, 32), a4v::castps_si128(vf26));
    A4_W128(ADD32((uint32_t)s3, 48), a4v::castps_si128(vf27));
    { (sp = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)sp, 272)))); io_vf10 = vf10; io_vf11 = vf11; io_vf12 = vf12; io_vf13 = vf13; io_vf14 = vf14; io_vf15 = vf15; io_vf16 = vf16; io_vf17 = vf17; io_vf24 = vf24; io_vf25 = vf25; io_vf26 = vf26; io_vf27 = vf27; io_vf28 = vf28; io_vf29 = vf29; io_vacc = vacc; io_h8 = h8; io_h9 = h9; io_h10 = h10; io_h11 = h11; io_h12 = h12; io_h13 = h13; io_h14 = h14; io_h15 = h15; return a1::RetVF{(uint64_t)v0, f0}; }
    __builtin_unreachable();
}

// GetTurnYAngleFromGlobalDir (0x1135358): typed by tools/typed_lift2.py (round A6; guest frame calls, extra outputs xo[0]=f12 xo[1]=f13 xo[2]=a0 xo[3]=a1 xo[4]=a2 xo[5]=a3 xo[6]=t0 xo[7]=t1 xo[8]=t2 xo[9]=t3; VU0 in vf0 vf7 vf10 vf12 vf13 vf14 vf15 vf16 vf17 vf25 vf28 vf29 vacc vstat, out vf5 vf7 vf8 vf10 vf11 vf12 vf13 vf14 vf15 vf16 vf17 vf24 vf25 vf26 vf28 vf29 vacc vq vstat h2 h8 h9 h10 h11 h12 h13 h14 h15, relocated calls 2)
inline a1::RetVF GetTurnYAngleFromGlobalDir(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint64_t a0, uint64_t a1, uint64_t sp, float f0, float f14, float f15, float f16, float f17, float f18, float f19, const a4::vec4 &io_vf0, a4::vec4 &io_vf5, a4::vec4 &io_vf7, a4::vec4 &io_vf8, a4::vec4 &io_vf10, a4::vec4 &io_vf11, a4::vec4 &io_vf12, a4::vec4 &io_vf13, a4::vec4 &io_vf14, a4::vec4 &io_vf15, a4::vec4 &io_vf16, a4::vec4 &io_vf17, a4::vec4 &io_vf24, a4::vec4 &io_vf25, a4::vec4 &io_vf26, a4::vec4 &io_vf28, a4::vec4 &io_vf29, a4::vec4 &io_vacc, float &io_vq, uint16_t &io_vstat, uint64_t &io_h2, uint64_t &io_h8, uint64_t &io_h9, uint64_t &io_h10, uint64_t &io_h11, uint64_t &io_h12, uint64_t &io_h13, uint64_t &io_h14, uint64_t &io_h15, uint64_t *xo = nullptr)
{
    float f1 = 0.0f;
    float f2 = 0.0f;
    float f12 = 0.0f;
    float f13 = 0.0f;
    uint64_t at = 0;
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
    uint64_t t7 = 0;
    uint32_t fcr31 = 0;
    a4::vec4 vf0 = io_vf0;
    a4::vec4 vf5 = io_vf5;
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
    a4::vec4 vf24 = io_vf24;
    a4::vec4 vf25 = io_vf25;
    a4::vec4 vf26 = io_vf26;
    a4::vec4 vf28 = io_vf28;
    a4::vec4 vf29 = io_vf29;
    a4::vec4 vacc = io_vacc;
    float vq = io_vq;
    uint16_t vstat = io_vstat;
    uint64_t h2 = io_h2;
    uint64_t h8 = io_h8;
    uint64_t h9 = io_h9;
    uint64_t h10 = io_h10;
    uint64_t h11 = io_h11;
    uint64_t h12 = io_h12;
    uint64_t h13 = io_h13;
    uint64_t h14 = io_h14;
    uint64_t h15 = io_h15;
    (sp = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)sp, 4294967120))));
    (a3 = (uint64_t)((uint64_t)RC_U64X(sp) + (uint64_t)0ULL));
    (a2 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)sp, 16))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)sp, 32))));
    { a4::vec4 res = a4v::addK<1, 1>(vf0, a4v::bcast<3>(vf0)); a4::q128 mask = a4v::set_epi32(0, -1, -1, -1); vf12 = a4v::blendv_ps(vf12, res, a4v::castsi128_ps(mask)); }
    vf11 = a4v::castsi128_ps(A4_R128(ADD32((uint32_t)a0, 0)));
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
    A4_W128(ADD32((uint32_t)v0, 0), a4v::castps_si128(vf14));
    A4_W128(ADD32((uint32_t)v0, 16), a4v::castps_si128(vf15));
    A4_W128(ADD32((uint32_t)v0, 32), a4v::castps_si128(vf16));
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)sp, 80))));
    A4_W128(ADD32((uint32_t)v1, 0), a4v::castps_si128(vf0));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)sp, 96))));
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)sp, 32))));
    vf24 = a4v::castsi128_ps(A4_R128(ADD32((uint32_t)v1, 48)));
    { a4::vec4 res = a4v::subK<1, 1>(vf0, a4v::bcast<3>(vf0)); a4::q128 mask = a4v::set_epi32(0, 0, 0, -1); vf25 = a4v::blendv_ps(vf25, res, a4v::castsi128_ps(mask)); }
    { a4::vec4 res = a4v::mulK<0, 1>(vf24, a4v::bcast<0>(vf25)); a4::q128 mask = a4v::set_epi32(0, -1, -1, -1); vf28 = a4v::blendv_ps(vf28, res, a4v::castsi128_ps(mask)); }
    { a4::vec4 res = a4v::sub(vf28, vf28); a4::q128 mask = a4v::set_epi32(-1, 0, 0, 0); vf28 = a4v::blend(vf28, res, a4v::castsi128_ps(mask)); }
    do { const a4::q128 _v = (A4_R128(ADD32((uint32_t)v1, 0))); h8 = a4::hi64(_v); std::memcpy(&t0, &_v, 8); } while (0);
    do { const a4::q128 _v = (A4_R128(ADD32((uint32_t)v1, 16))); h9 = a4::hi64(_v); std::memcpy(&t1, &_v, 8); } while (0);
    do { const a4::q128 _v = (A4_R128(ADD32((uint32_t)v1, 32))); h10 = a4::hi64(_v); std::memcpy(&t2, &_v, 8); } while (0);
    do { const a4::q128 _v = (a4v::castps_si128(vf0)); h11 = a4::hi64(_v); std::memcpy(&t3, &_v, 8); } while (0);
    do { const a4::q128 _v = (a4m::pextlw(a4::mk128(h9, t1), a4::mk128(h8, t0))); h12 = a4::hi64(_v); std::memcpy(&t4, &_v, 8); } while (0);
    do { const a4::q128 _v = (a4m::pextuw(a4::mk128(h9, t1), a4::mk128(h8, t0))); h13 = a4::hi64(_v); std::memcpy(&t5, &_v, 8); } while (0);
    do { const a4::q128 _v = (a4m::pextlw(a4::mk128(h11, t3), a4::mk128(h10, t2))); h14 = a4::hi64(_v); std::memcpy(&t6, &_v, 8); } while (0);
    do { const a4::q128 _v = (a4m::pextuw(a4::mk128(h11, t3), a4::mk128(h10, t2))); h15 = a4::hi64(_v); std::memcpy(&t7, &_v, 8); } while (0);
    do { const a4::q128 _v = (a4m::pcpyld(a4::mk128(h14, t6), a4::mk128(h12, t4))); h8 = a4::hi64(_v); std::memcpy(&t0, &_v, 8); } while (0);
    do { const a4::q128 _v = (a4v::unpackhi_epi64(a4::mk128(h12, t4), a4::mk128(h14, t6))); h9 = a4::hi64(_v); std::memcpy(&t1, &_v, 8); } while (0);
    do { const a4::q128 _v = (a4m::pcpyld(a4::mk128(h15, t7), a4::mk128(h13, t5))); h10 = a4::hi64(_v); std::memcpy(&t2, &_v, 8); } while (0);
    do { const a4::q128 _v = (a4v::unpackhi_epi64(a4::mk128(h13, t5), a4::mk128(h15, t7))); h11 = a4::hi64(_v); std::memcpy(&t3, &_v, 8); } while (0);
    vf24 = a4v::castsi128_ps(a4::mk128(h8, t0));
    vf25 = a4v::castsi128_ps(a4::mk128(h9, t1));
    vf26 = a4v::castsi128_ps(a4::mk128(h10, t2));
    { a4::vec4 res = a4v::mulK<0, 1>(vf24, a4v::bcast<0>(vf28)); vacc = a4v::dest<1, 1, 1, 1>(vacc, res); }
    { a4::vec4 mul_res = a4v::mulK<0, 1>(vf25, a4v::bcast<1>(vf28)); a4::vec4 res = a4v::addK<1, 1>(vacc, mul_res); vacc = a4v::dest<1, 1, 1, 1>(vacc, res); }
    { a4::vec4 mul_res = a4v::mulK<0, 1>(vf26, a4v::bcast<2>(vf28)); a4::vec4 res = a4v::addK<1, 1>(vacc, mul_res); vacc = a4v::dest<1, 1, 1, 1>(vacc, res); }
    { a4::vec4 mul_res = a4v::mulK<1, 1>(vf0, a4v::bcast<3>(vf28)); a4::vec4 res = a4v::addK<1, 1>(vacc, mul_res); a4::q128 mask = a4v::set_epi32(-1, -1, -1, -1); vf29 = a4v::blendv_ps(vf29, res, a4v::castsi128_ps(mask)); }
    { a4::vec4 res = a4v::subK<1, 1>(vf29, vf29); a4::q128 mask = a4v::set_epi32(-1, 0, 0, 0); vf29 = a4v::blend(vf29, res, a4v::castsi128_ps(mask)); }
    { a4::vec4 res = a4v::addK<1, 1>(vf29, vf0); a4::q128 mask = a4v::set_epi32(-1, 0, 0, 0); vf29 = a4v::blend(vf29, res, a4v::castsi128_ps(mask)); }
    A4_W128(ADD32((uint32_t)v0, 0), a4v::castps_si128(vf24));
    A4_W128(ADD32((uint32_t)v0, 16), a4v::castps_si128(vf25));
    A4_W128(ADD32((uint32_t)v0, 32), a4v::castps_si128(vf26));
    A4_W128(ADD32((uint32_t)v0, 48), a4v::castps_si128(vf29));
    vf8 = a4v::castsi128_ps(A4_R128(ADD32((uint32_t)a1, 0)));
    vf14 = a4v::castsi128_ps(A4_R128(ADD32((uint32_t)v0, 0)));
    vf15 = a4v::castsi128_ps(A4_R128(ADD32((uint32_t)v0, 16)));
    vf16 = a4v::castsi128_ps(A4_R128(ADD32((uint32_t)v0, 32)));
    vf17 = a4v::castsi128_ps(A4_R128(ADD32((uint32_t)v0, 48)));
    { a4::vec4 res = a4v::mul(vf14, a4v::bcast<0>(vf8)); vacc = a4v::dest<1, 1, 1, 1>(vacc, res); }
    { a4::vec4 mul_res = a4v::mul(vf15, a4v::bcast<1>(vf8)); a4::vec4 res = a4v::addK<1, 1>(vacc, mul_res); vacc = a4v::dest<1, 1, 1, 1>(vacc, res); }
    { a4::vec4 mul_res = a4v::mul(vf16, a4v::bcast<2>(vf8)); a4::vec4 res = a4v::addK<1, 1>(vacc, mul_res); vacc = a4v::dest<1, 1, 1, 1>(vacc, res); }
    { a4::vec4 mul_res = a4v::mul(vf17, a4v::bcast<3>(vf8)); a4::vec4 res = a4v::addK<1, 1>(vacc, mul_res); a4::q128 mask = a4v::set_epi32(-1, -1, -1, -1); vf10 = a4v::blendv_ps(vf10, res, a4v::castsi128_ps(mask)); }
    A4_W128(ADD32((uint32_t)a2, 0), a4v::castps_si128(vf10));
    (a0 = (uint64_t)((uint64_t)RC_U64X(a3) + (uint64_t)0ULL));
    { (a1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)sp, 16)))); A3_CHARGE(); v0 = iosGetStabXZVector(ram, rt, (uint32_t)a0, (uint32_t)a1, vf0, vf5, vf7, vacc, vq, vstat, h2); }
    { uint32_t bits = A1_R32B((uint32_t)sp, 0); float f; std::memcpy(&f, &bits, sizeof(f)); f12 = f; }
    { { uint32_t bits = A1_R32B((uint32_t)sp, 8); float f; std::memcpy(&f, &bits, sizeof(f)); f13 = f; } A3_CHARGE(); f0 = a9x::atan2Exact(ram, rt, f12, f13); }
    (at = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)17204 << 16))));
    { uint32_t bits = (uint32_t)at; std::memcpy(&f1, &bits, sizeof(bits)); }
    (at = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)16457 << 16))));
    (at = (uint64_t)(RC_U64X(at) | (uint64_t)(uint16_t)4058));
    { uint32_t bits = (uint32_t)at; std::memcpy(&f2, &bits, sizeof(bits)); }
    f0 = a1f::mul(f0, f1);
    f0 = a1f::div(fcr31, f0, f2);
    { (sp = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)sp, 176)))); if (xo) { xo[0] = a1_bits(f12); xo[1] = a1_bits(f13); xo[2] = a0; xo[3] = a1; xo[4] = a2; xo[5] = a3; xo[6] = t0; xo[7] = t1; xo[8] = t2; xo[9] = t3; } io_vf5 = vf5; io_vf7 = vf7; io_vf8 = vf8; io_vf10 = vf10; io_vf11 = vf11; io_vf12 = vf12; io_vf13 = vf13; io_vf14 = vf14; io_vf15 = vf15; io_vf16 = vf16; io_vf17 = vf17; io_vf24 = vf24; io_vf25 = vf25; io_vf26 = vf26; io_vf28 = vf28; io_vf29 = vf29; io_vacc = vacc; io_vq = vq; io_vstat = vstat; io_h2 = h2; io_h8 = h8; io_h9 = h9; io_h10 = h10; io_h11 = h11; io_h12 = h12; io_h13 = h13; io_h14 = h14; io_h15 = h15; return a1::RetVF{(uint64_t)v0, f0}; }
    __builtin_unreachable();
}

// GetTurnYLimitAngleFromGlobalDir (0x11354d0): typed by tools/typed_lift2.py (round A6; guest frame calls, extra outputs xo[0]=f12 xo[1]=f13 xo[2]=f20 xo[3]=a3 xo[4]=t0 xo[5]=t1 xo[6]=t2 xo[7]=t3; VU0 in vf0 vf7 vf10 vf12 vf13 vf14 vf15 vf16 vf17 vf25 vf28 vf29 vacc vstat, out vf5 vf7 vf8 vf10 vf11 vf12 vf13 vf14 vf15 vf16 vf17 vf24 vf25 vf26 vf28 vf29 vacc vq vstat h2 h8 h9 h10 h11 h12 h13 h14 h15, relocated calls 3)
inline a1::RetVF GetTurnYLimitAngleFromGlobalDir(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint64_t a0, uint64_t a1, uint64_t a2, uint64_t sp, float f0, float f12, float f14, float f15, float f16, float f17, float f18, float f19, float f20, float f21, const a4::vec4 &io_vf0, a4::vec4 &io_vf5, a4::vec4 &io_vf7, a4::vec4 &io_vf8, a4::vec4 &io_vf10, a4::vec4 &io_vf11, a4::vec4 &io_vf12, a4::vec4 &io_vf13, a4::vec4 &io_vf14, a4::vec4 &io_vf15, a4::vec4 &io_vf16, a4::vec4 &io_vf17, a4::vec4 &io_vf24, a4::vec4 &io_vf25, a4::vec4 &io_vf26, a4::vec4 &io_vf28, a4::vec4 &io_vf29, a4::vec4 &io_vacc, float &io_vq, uint16_t &io_vstat, uint64_t &io_h2, uint64_t &io_h8, uint64_t &io_h9, uint64_t &io_h10, uint64_t &io_h11, uint64_t &io_h12, uint64_t &io_h13, uint64_t &io_h14, uint64_t &io_h15, uint64_t *xo = nullptr)
{
    float f1 = 0.0f;
    float f13 = 0.0f;
    uint64_t at = 0;
    uint64_t v0 = 0;
    uint64_t v1 = 0;
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
    uint32_t fcr31 = 0;
    a4::vec4 vf0 = io_vf0;
    a4::vec4 vf5 = io_vf5;
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
    a4::vec4 vf24 = io_vf24;
    a4::vec4 vf25 = io_vf25;
    a4::vec4 vf26 = io_vf26;
    a4::vec4 vf28 = io_vf28;
    a4::vec4 vf29 = io_vf29;
    a4::vec4 vacc = io_vacc;
    float vq = io_vq;
    uint16_t vstat = io_vstat;
    uint64_t h2 = io_h2;
    uint64_t h8 = io_h8;
    uint64_t h9 = io_h9;
    uint64_t h10 = io_h10;
    uint64_t h11 = io_h11;
    uint64_t h12 = io_h12;
    uint64_t h13 = io_h13;
    uint64_t h14 = io_h14;
    uint64_t h15 = io_h15;
    (sp = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)sp, 4294967072))));
    (s1 = (uint64_t)((uint64_t)a0 + (uint64_t)0ULL));
    { float f = f21; uint32_t bits; std::memcpy(&bits, &f, sizeof(bits)); A1_W32B((uint32_t)sp, 208, bits); }
    f21 = ((float)(f12));
    (a0 = (uint64_t)((uint64_t)RC_U64X(sp) + (uint64_t)0ULL));
    (a3 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)sp, 16))));
    { float f = f20; uint32_t bits; std::memcpy(&bits, &f, sizeof(bits)); A1_W32B((uint32_t)sp, 200, bits); }
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)sp, 32))));
    { a4::vec4 res = a4v::addK<1, 1>(vf0, a4v::bcast<3>(vf0)); a4::q128 mask = a4v::set_epi32(0, -1, -1, -1); vf12 = a4v::blendv_ps(vf12, res, a4v::castsi128_ps(mask)); }
    vf11 = a4v::castsi128_ps(A4_R128(ADD32((uint32_t)a1, 0)));
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
    A4_W128(ADD32((uint32_t)v0, 0), a4v::castps_si128(vf14));
    A4_W128(ADD32((uint32_t)v0, 16), a4v::castps_si128(vf15));
    A4_W128(ADD32((uint32_t)v0, 32), a4v::castps_si128(vf16));
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)sp, 80))));
    A4_W128(ADD32((uint32_t)v1, 0), a4v::castps_si128(vf0));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)sp, 96))));
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)sp, 32))));
    vf24 = a4v::castsi128_ps(A4_R128(ADD32((uint32_t)v1, 48)));
    { a4::vec4 res = a4v::subK<1, 1>(vf0, a4v::bcast<3>(vf0)); a4::q128 mask = a4v::set_epi32(0, 0, 0, -1); vf25 = a4v::blendv_ps(vf25, res, a4v::castsi128_ps(mask)); }
    { a4::vec4 res = a4v::mulK<0, 1>(vf24, a4v::bcast<0>(vf25)); a4::q128 mask = a4v::set_epi32(0, -1, -1, -1); vf28 = a4v::blendv_ps(vf28, res, a4v::castsi128_ps(mask)); }
    { a4::vec4 res = a4v::sub(vf28, vf28); a4::q128 mask = a4v::set_epi32(-1, 0, 0, 0); vf28 = a4v::blend(vf28, res, a4v::castsi128_ps(mask)); }
    do { const a4::q128 _v = (A4_R128(ADD32((uint32_t)v1, 0))); h8 = a4::hi64(_v); std::memcpy(&t0, &_v, 8); } while (0);
    do { const a4::q128 _v = (A4_R128(ADD32((uint32_t)v1, 16))); h9 = a4::hi64(_v); std::memcpy(&t1, &_v, 8); } while (0);
    do { const a4::q128 _v = (A4_R128(ADD32((uint32_t)v1, 32))); h10 = a4::hi64(_v); std::memcpy(&t2, &_v, 8); } while (0);
    do { const a4::q128 _v = (a4v::castps_si128(vf0)); h11 = a4::hi64(_v); std::memcpy(&t3, &_v, 8); } while (0);
    do { const a4::q128 _v = (a4m::pextlw(a4::mk128(h9, t1), a4::mk128(h8, t0))); h12 = a4::hi64(_v); std::memcpy(&t4, &_v, 8); } while (0);
    do { const a4::q128 _v = (a4m::pextuw(a4::mk128(h9, t1), a4::mk128(h8, t0))); h13 = a4::hi64(_v); std::memcpy(&t5, &_v, 8); } while (0);
    do { const a4::q128 _v = (a4m::pextlw(a4::mk128(h11, t3), a4::mk128(h10, t2))); h14 = a4::hi64(_v); std::memcpy(&t6, &_v, 8); } while (0);
    do { const a4::q128 _v = (a4m::pextuw(a4::mk128(h11, t3), a4::mk128(h10, t2))); h15 = a4::hi64(_v); std::memcpy(&t7, &_v, 8); } while (0);
    do { const a4::q128 _v = (a4m::pcpyld(a4::mk128(h14, t6), a4::mk128(h12, t4))); h8 = a4::hi64(_v); std::memcpy(&t0, &_v, 8); } while (0);
    do { const a4::q128 _v = (a4v::unpackhi_epi64(a4::mk128(h12, t4), a4::mk128(h14, t6))); h9 = a4::hi64(_v); std::memcpy(&t1, &_v, 8); } while (0);
    do { const a4::q128 _v = (a4m::pcpyld(a4::mk128(h15, t7), a4::mk128(h13, t5))); h10 = a4::hi64(_v); std::memcpy(&t2, &_v, 8); } while (0);
    do { const a4::q128 _v = (a4v::unpackhi_epi64(a4::mk128(h13, t5), a4::mk128(h15, t7))); h11 = a4::hi64(_v); std::memcpy(&t3, &_v, 8); } while (0);
    vf24 = a4v::castsi128_ps(a4::mk128(h8, t0));
    vf25 = a4v::castsi128_ps(a4::mk128(h9, t1));
    vf26 = a4v::castsi128_ps(a4::mk128(h10, t2));
    { a4::vec4 res = a4v::mulK<0, 1>(vf24, a4v::bcast<0>(vf28)); vacc = a4v::dest<1, 1, 1, 1>(vacc, res); }
    { a4::vec4 mul_res = a4v::mulK<0, 1>(vf25, a4v::bcast<1>(vf28)); a4::vec4 res = a4v::addK<1, 1>(vacc, mul_res); vacc = a4v::dest<1, 1, 1, 1>(vacc, res); }
    { a4::vec4 mul_res = a4v::mulK<0, 1>(vf26, a4v::bcast<2>(vf28)); a4::vec4 res = a4v::addK<1, 1>(vacc, mul_res); vacc = a4v::dest<1, 1, 1, 1>(vacc, res); }
    { a4::vec4 mul_res = a4v::mulK<1, 1>(vf0, a4v::bcast<3>(vf28)); a4::vec4 res = a4v::addK<1, 1>(vacc, mul_res); a4::q128 mask = a4v::set_epi32(-1, -1, -1, -1); vf29 = a4v::blendv_ps(vf29, res, a4v::castsi128_ps(mask)); }
    { a4::vec4 res = a4v::subK<1, 1>(vf29, vf29); a4::q128 mask = a4v::set_epi32(-1, 0, 0, 0); vf29 = a4v::blend(vf29, res, a4v::castsi128_ps(mask)); }
    { a4::vec4 res = a4v::addK<1, 1>(vf29, vf0); a4::q128 mask = a4v::set_epi32(-1, 0, 0, 0); vf29 = a4v::blend(vf29, res, a4v::castsi128_ps(mask)); }
    A4_W128(ADD32((uint32_t)v0, 0), a4v::castps_si128(vf24));
    A4_W128(ADD32((uint32_t)v0, 16), a4v::castps_si128(vf25));
    A4_W128(ADD32((uint32_t)v0, 32), a4v::castps_si128(vf26));
    A4_W128(ADD32((uint32_t)v0, 48), a4v::castps_si128(vf29));
    vf8 = a4v::castsi128_ps(A4_R128(ADD32((uint32_t)a2, 0)));
    vf14 = a4v::castsi128_ps(A4_R128(ADD32((uint32_t)v0, 0)));
    vf15 = a4v::castsi128_ps(A4_R128(ADD32((uint32_t)v0, 16)));
    vf16 = a4v::castsi128_ps(A4_R128(ADD32((uint32_t)v0, 32)));
    vf17 = a4v::castsi128_ps(A4_R128(ADD32((uint32_t)v0, 48)));
    { a4::vec4 res = a4v::mul(vf14, a4v::bcast<0>(vf8)); vacc = a4v::dest<1, 1, 1, 1>(vacc, res); }
    { a4::vec4 mul_res = a4v::mul(vf15, a4v::bcast<1>(vf8)); a4::vec4 res = a4v::addK<1, 1>(vacc, mul_res); vacc = a4v::dest<1, 1, 1, 1>(vacc, res); }
    { a4::vec4 mul_res = a4v::mul(vf16, a4v::bcast<2>(vf8)); a4::vec4 res = a4v::addK<1, 1>(vacc, mul_res); vacc = a4v::dest<1, 1, 1, 1>(vacc, res); }
    { a4::vec4 mul_res = a4v::mul(vf17, a4v::bcast<3>(vf8)); a4::vec4 res = a4v::addK<1, 1>(vacc, mul_res); a4::q128 mask = a4v::set_epi32(-1, -1, -1, -1); vf10 = a4v::blendv_ps(vf10, res, a4v::castsi128_ps(mask)); }
    A4_W128(ADD32((uint32_t)a3, 0), a4v::castps_si128(vf10));
    { (a1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)sp, 16)))); A3_CHARGE(); v0 = iosGetStabXZVector(ram, rt, (uint32_t)a0, (uint32_t)a1, vf0, vf5, vf7, vacc, vq, vstat, h2); }
    { uint32_t bits = A1_R32B((uint32_t)sp, 0); float f; std::memcpy(&f, &bits, sizeof(f)); f12 = f; }
    { { uint32_t bits = A1_R32B((uint32_t)sp, 8); float f; std::memcpy(&f, &bits, sizeof(f)); f13 = f; } A3_CHARGE(); f0 = a9x::atan2Exact(ram, rt, f12, f13); }
    { const uint32_t _dynInsn = A1_R32(0x1135628u); const int32_t _dynSimm = (int16_t)(_dynInsn & 0xFFFFu); const uint32_t _dynImm = _dynInsn & 0xFFFFu; (void)_dynSimm; (void)_dynImm; (a2 = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)_dynImm << 16)))); }
    f20 = ((float)(f0));
    { uint32_t bits = 0u; std::memcpy(&f0, &bits, sizeof(bits)); }
    { const uint32_t _dynInsn = A1_R32(0x1135634u); const int32_t _dynSimm = (int16_t)(_dynInsn & 0xFFFFu); const uint32_t _dynImm = _dynInsn & 0xFFFFu; (void)_dynSimm; (void)_dynImm; (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)_dynImm << 16)))); }
    { const uint32_t _dynInsn = A1_R32(0x1135638u); const int32_t _dynSimm = (int16_t)(_dynInsn & 0xFFFFu); const uint32_t _dynImm = _dynInsn & 0xFFFFu; (void)_dynSimm; (void)_dynImm; (a2 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)a2, _dynSimm)))); }
    fcr31 = (a1f::lt(f20, f0)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)sp, 160))));
    { const uint32_t _dynInsn = A1_R32(0x1135644u); const int32_t _dynSimm = (int16_t)(_dynInsn & 0xFFFFu); const uint32_t _dynImm = _dynInsn & 0xFFFFu; (void)_dynSimm; (void)_dynImm; (s0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v0, _dynSimm)))); }
    { const bool c = (fcr31 & 0x800000u) == 0u; f1 = ((float)(f21)); if (c) goto L_1135668; }
    f0 = (-(float)(f20));
    fcr31 = (a1f::lt(f21, f0)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
    if ((fcr31 & 0x800000u) != 0u) goto L_1135674;
    goto L_1135688;
L_1135668:;
    fcr31 = (a1f::lt(f21, f20)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
    if ((fcr31 & 0x800000u) == 0u) goto L_1135688;
L_1135674:;
    { uint32_t bits = 0u; std::memcpy(&f0, &bits, sizeof(bits)); }
    fcr31 = (a1f::lt(f20, f0)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
    if ((fcr31 & 0x800000u) != 0u) { f1 = (-(float)(f1)); goto L_1135684; }
L_1135684:;
    f20 = ((float)(f1));
L_1135688:;
    (at = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)18176 << 16))));
    { uint32_t bits = (uint32_t)at; std::memcpy(&f0, &bits, sizeof(bits)); }
    (at = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)16457 << 16))));
    (at = (uint64_t)(RC_U64X(at) | (uint64_t)(uint16_t)4058));
    { uint32_t bits = (uint32_t)at; std::memcpy(&f1, &bits, sizeof(bits)); }
    f0 = a1f::mul(f20, f0);
    f0 = a1f::div(fcr31, f0, f1);
    { int32_t tmp = a1f::cvtw(f0); std::memcpy(&f1, &tmp, sizeof(tmp)); }
    { uint32_t bits; std::memcpy(&bits, &f1, sizeof(bits)); (a1 = (uint64_t)((int64_t)(int32_t)(bits))); }
    (a1 = (uint64_t)((int64_t)(int32_t)((int32_t)SLL32((uint32_t)a1, 16))));
    (a1 = (uint64_t)((int64_t)(int32_t)((int32_t)SUB32(0u, (uint32_t)a1))));
    { (a1 = (uint64_t)((int64_t)(int32_t)(SRA32((int32_t)a1, 16)))); A3_CHARGE(); f0 = a9x::axisRotate(ram, rt, (uint32_t)a0, (uint32_t)a1, (uint32_t)a2); }
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)sp, 160))));
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
    A4_W128(ADD32((uint32_t)s1, 0), a4v::castps_si128(vf13));
    f0 = ((float)(f20));
    { uint32_t bits = A1_R32B((uint32_t)sp, 208); float f; std::memcpy(&f, &bits, sizeof(f)); f21 = f; }
    { uint32_t bits = A1_R32B((uint32_t)sp, 200); float f; std::memcpy(&f, &bits, sizeof(f)); f20 = f; }
    { (sp = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)sp, 224)))); if (xo) { xo[0] = a1_bits(f12); xo[1] = a1_bits(f13); xo[2] = a1_bits(f20); xo[3] = a3; xo[4] = t0; xo[5] = t1; xo[6] = t2; xo[7] = t3; } io_vf5 = vf5; io_vf7 = vf7; io_vf8 = vf8; io_vf10 = vf10; io_vf11 = vf11; io_vf12 = vf12; io_vf13 = vf13; io_vf14 = vf14; io_vf15 = vf15; io_vf16 = vf16; io_vf17 = vf17; io_vf24 = vf24; io_vf25 = vf25; io_vf26 = vf26; io_vf28 = vf28; io_vf29 = vf29; io_vacc = vacc; io_vq = vq; io_vstat = vstat; io_h2 = h2; io_h8 = h8; io_h9 = h9; io_h10 = h10; io_h11 = h11; io_h12 = h12; io_h13 = h13; io_h14 = h14; io_h15 = h15; return a1::RetVF{(uint64_t)v0, f0}; }
    __builtin_unreachable();
}

// CheckCubeVolume (0x11362d0): typed by tools/typed_lift2.py (round A6; guest frame calls, extra outputs xo[0]=a1 xo[1]=t0 xo[2]=t1 xo[3]=t2 xo[4]=t3; VU0 in vf2 vf6 vf9 vf10 vacc, out vf1 vf2 vf3 vf4 vf5 vf6 vf8 vf9 vf10 vf14 vf15 vf16 vf17 vacc h8 h9 h10 h11 h12 h13 h14 h15, relocated calls 2)
inline a1::RetVF CheckCubeVolume(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint64_t a0, uint64_t a1, uint64_t a2, uint32_t sp, float f12, float f13, float f14, float f15, float f16, float f17, float f18, float f19, a4::vec4 &io_vf1, a4::vec4 &io_vf2, a4::vec4 &io_vf3, a4::vec4 &io_vf4, a4::vec4 &io_vf5, a4::vec4 &io_vf6, a4::vec4 &io_vf8, a4::vec4 &io_vf9, a4::vec4 &io_vf10, a4::vec4 &io_vf14, a4::vec4 &io_vf15, a4::vec4 &io_vf16, a4::vec4 &io_vf17, a4::vec4 &io_vacc, uint64_t &io_h8, uint64_t &io_h9, uint64_t &io_h10, uint64_t &io_h11, uint64_t &io_h12, uint64_t &io_h13, uint64_t &io_h14, uint64_t &io_h15, uint64_t *xo = nullptr)
{
    float f0 = 0.0f;
    float f1 = 0.0f;
    uint64_t v0 = 0;
    uint64_t t0 = 0;
    uint64_t t1 = 0;
    uint64_t t2 = 0;
    uint64_t t3 = 0;
    uint64_t s0 = 0;
    uint64_t s1 = 0;
    uint32_t fcr31 = 0;
    a4::vec4 vf1 = io_vf1;
    a4::vec4 vf2 = io_vf2;
    a4::vec4 vf3 = io_vf3;
    a4::vec4 vf4 = io_vf4;
    a4::vec4 vf5 = io_vf5;
    a4::vec4 vf6 = io_vf6;
    a4::vec4 vf8 = io_vf8;
    a4::vec4 vf9 = io_vf9;
    a4::vec4 vf10 = io_vf10;
    a4::vec4 vf14 = io_vf14;
    a4::vec4 vf15 = io_vf15;
    a4::vec4 vf16 = io_vf16;
    a4::vec4 vf17 = io_vf17;
    a4::vec4 vacc = io_vacc;
    uint64_t h8 = io_h8;
    uint64_t h9 = io_h9;
    uint64_t h10 = io_h10;
    uint64_t h11 = io_h11;
    uint64_t h12 = io_h12;
    uint64_t h13 = io_h13;
    uint64_t h14 = io_h14;
    uint64_t h15 = io_h15;
    sp = sp - 0x70u;
    (v0 = (uint64_t)((uint64_t)a0 + (uint64_t)0ULL));
    (s0 = (uint64_t)((uint64_t)a1 + (uint64_t)0ULL));
    (a0 = (uint64_t)((uint64_t)a1_sx64(sp) + (uint64_t)0ULL));
    (a1 = (uint64_t)((uint64_t)v0 + (uint64_t)0ULL));
    { (s1 = (uint64_t)((uint64_t)a2 + (uint64_t)0ULL)); A3_CHARGE(); uint64_t a7c[4] = {0u, 0u, 0u, 0u}; iosInverseMatrix(ram, rt, a0, a1, vf1, vf2, vf3, vf4, vf8, vf9, vacc, h8, h9, h10, h11, h12, h13, h14, h15, a7c); t0 = a7c[0]; t1 = a7c[1]; t2 = a7c[2]; t3 = a7c[3]; }
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(sp, 64))));
    vf8 = a4v::castsi128_ps(A4_R128(ADD32((uint32_t)s0, 0)));
    vf14 = a4v::castsi128_ps(A4_R128(ADD32(sp, 0)));
    vf15 = a4v::castsi128_ps(A4_R128(ADD32(sp, 16)));
    vf16 = a4v::castsi128_ps(A4_R128(ADD32(sp, 32)));
    vf17 = a4v::castsi128_ps(A4_R128(ADD32(sp, 48)));
    { a4::vec4 res = a4v::mul(vf14, a4v::bcast<0>(vf8)); vacc = a4v::dest<1, 1, 1, 1>(vacc, res); }
    { a4::vec4 mul_res = a4v::mul(vf15, a4v::bcast<1>(vf8)); a4::vec4 res = a4v::addK<1, 1>(vacc, mul_res); vacc = a4v::dest<1, 1, 1, 1>(vacc, res); }
    { a4::vec4 mul_res = a4v::mul(vf16, a4v::bcast<2>(vf8)); a4::vec4 res = a4v::addK<1, 1>(vacc, mul_res); vacc = a4v::dest<1, 1, 1, 1>(vacc, res); }
    { a4::vec4 mul_res = a4v::mul(vf17, a4v::bcast<3>(vf8)); a4::vec4 res = a4v::addK<1, 1>(vacc, mul_res); a4::q128 mask = a4v::set_epi32(-1, -1, -1, -1); vf10 = a4v::blendv_ps(vf10, res, a4v::castsi128_ps(mask)); }
    A4_W128(ADD32((uint32_t)a0, 0), a4v::castps_si128(vf10));
    { (a1 = (uint64_t)((uint64_t)RC_U64X(a0) + (uint64_t)0ULL)); A3_CHARGE(); iosAbsVector(ram, rt, (uint32_t)a0, (uint32_t)a1, vf5, vf6); }
    f1 = a1_flt(A1_R32B(sp, 64));
    { uint32_t bits = A1_R32B((uint32_t)s1, 0); float f; std::memcpy(&f, &bits, sizeof(f)); f0 = f; }
    fcr31 = (a1f::le(f1, f0)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
    { const bool c = (fcr31 & 0x800000u) == 0u; (v0 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL)); if (c) goto L_1136368; }
    f1 = a1_flt(A1_R32B(sp, 68));
    { uint32_t bits = A1_R32B((uint32_t)s1, 4); float f; std::memcpy(&f, &bits, sizeof(f)); f0 = f; }
    fcr31 = (a1f::le(f1, f0)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
    if ((fcr31 & 0x800000u) == 0u) goto L_113636c;
    { uint32_t bits = A1_R32B((uint32_t)s1, 8); float f; std::memcpy(&f, &bits, sizeof(f)); f1 = f; }
    f0 = a1_flt(A1_R32B(sp, 72));
    fcr31 = (a1f::le(f0, f1)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
    if ((fcr31 & 0x800000u) != 0u) { (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 1)))); goto L_113636c; }
L_1136368:;
L_113636c:;
    { sp = sp + 0x70u; if (xo) { xo[0] = a1; xo[1] = t0; xo[2] = t1; xo[3] = t2; xo[4] = t3; } io_vf1 = vf1; io_vf2 = vf2; io_vf3 = vf3; io_vf4 = vf4; io_vf5 = vf5; io_vf6 = vf6; io_vf8 = vf8; io_vf9 = vf9; io_vf10 = vf10; io_vf14 = vf14; io_vf15 = vf15; io_vf16 = vf16; io_vf17 = vf17; io_vacc = vacc; io_h8 = h8; io_h9 = h9; io_h10 = h10; io_h11 = h11; io_h12 = h12; io_h13 = h13; io_h14 = h14; io_h15 = h15; return a1::RetVF{(uint64_t)v0, f0}; }
    __builtin_unreachable();
}

// CheckEllipticalSphereVolume (0x1136450): typed by tools/typed_lift2.py (round A6; guest frame calls; VU0 in vf2 vf9 vf10 vacc, out vf1 vf2 vf3 vf4 vf5 vf6 vf7 vf8 vf9 vf10 vf14 vf15 vf16 vf17 vacc h2 h8 h9 h10 h11 h12 h13 h14 h15, relocated calls 1)
inline a1::RetVF CheckEllipticalSphereVolume(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint64_t a0, uint64_t a1, uint64_t a2, uint32_t sp, a4::vec4 &io_vf1, a4::vec4 &io_vf2, a4::vec4 &io_vf3, a4::vec4 &io_vf4, a4::vec4 &io_vf5, a4::vec4 &io_vf6, a4::vec4 &io_vf7, a4::vec4 &io_vf8, a4::vec4 &io_vf9, a4::vec4 &io_vf10, a4::vec4 &io_vf14, a4::vec4 &io_vf15, a4::vec4 &io_vf16, a4::vec4 &io_vf17, a4::vec4 &io_vacc, uint64_t &io_h2, uint64_t &io_h8, uint64_t &io_h9, uint64_t &io_h10, uint64_t &io_h11, uint64_t &io_h12, uint64_t &io_h13, uint64_t &io_h14, uint64_t &io_h15)
{
    float f0 = 0.0f;
    float f1 = 0.0f;
    float f2 = 0.0f;
    float f3 = 0.0f;
    uint32_t at = 0;
    uint64_t v0 = 0;
    uint32_t v1 = 0;
    uint64_t s0 = 0;
    uint64_t s1 = 0;
    uint32_t fcr31 = 0;
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
    a4::vec4 vf14 = io_vf14;
    a4::vec4 vf15 = io_vf15;
    a4::vec4 vf16 = io_vf16;
    a4::vec4 vf17 = io_vf17;
    a4::vec4 vacc = io_vacc;
    uint64_t h2 = io_h2;
    uint64_t h8 = io_h8;
    uint64_t h9 = io_h9;
    uint64_t h10 = io_h10;
    uint64_t h11 = io_h11;
    uint64_t h12 = io_h12;
    uint64_t h13 = io_h13;
    uint64_t h14 = io_h14;
    uint64_t h15 = io_h15;
    sp = sp - 0x90u;
    (v0 = (uint64_t)((uint64_t)a0 + (uint64_t)0ULL));
    (s0 = (uint64_t)((uint64_t)a2 + (uint64_t)0ULL));
    (s1 = (uint64_t)((uint64_t)a1 + (uint64_t)0ULL));
    (a0 = (uint64_t)((uint64_t)a1_sx64(sp) + (uint64_t)0ULL));
    { (a1 = (uint64_t)((uint64_t)v0 + (uint64_t)0ULL)); A3_CHARGE(); iosInverseMatrix(ram, rt, a0, a1, vf1, vf2, vf3, vf4, vf8, vf9, vacc, h8, h9, h10, h11, h12, h13, h14, h15); }
    { uint32_t bits = A1_R32B((uint32_t)s0, 8); float f; std::memcpy(&f, &bits, sizeof(f)); f0 = f; }
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(sp, 64))));
    at = 0x3F800000u;
    { uint32_t bits = a7_opq(at); std::memcpy(&f3, &bits, sizeof(bits)); }
    { uint32_t bits = A1_R32B((uint32_t)s0, 4); float f; std::memcpy(&f, &bits, sizeof(f)); f2 = f; }
    { uint32_t bits = A1_R32B((uint32_t)s0, 0); float f; std::memcpy(&f, &bits, sizeof(f)); f1 = f; }
    f0 = a1f::div(fcr31, f3, f0);
    A1_W32B(sp, 108, 0u);
    f1 = a1f::div(fcr31, f3, f1);
    A1_W32B(sp, 104, a1_bits(f0));
    (v0 = (uint64_t)(A1_R64B(sp, 104)));
    f2 = a1f::div(fcr31, f3, f2);
    A1_W64B(sp, 88, v0);
    A1_W32B(sp, 96, a1_bits(f1));
    { float f = f2; uint32_t bits; std::memcpy(&bits, &f, sizeof(bits)); A1_W32B(sp, 100, bits); }
    (v0 = (uint64_t)(A1_R64B(sp, 96)));
    A1_W64B(sp, 80, v0);
    vf8 = a4v::castsi128_ps(A4_R128(ADD32((uint32_t)s1, 0)));
    vf14 = a4v::castsi128_ps(A4_R128(ADD32(sp, 0)));
    vf15 = a4v::castsi128_ps(A4_R128(ADD32(sp, 16)));
    vf16 = a4v::castsi128_ps(A4_R128(ADD32(sp, 32)));
    vf17 = a4v::castsi128_ps(A4_R128(ADD32(sp, 48)));
    { a4::vec4 res = a4v::mul(vf14, a4v::bcast<0>(vf8)); vacc = a4v::dest<1, 1, 1, 1>(vacc, res); }
    { a4::vec4 mul_res = a4v::mul(vf15, a4v::bcast<1>(vf8)); a4::vec4 res = a4v::addK<1, 1>(vacc, mul_res); vacc = a4v::dest<1, 1, 1, 1>(vacc, res); }
    { a4::vec4 mul_res = a4v::mul(vf16, a4v::bcast<2>(vf8)); a4::vec4 res = a4v::addK<1, 1>(vacc, mul_res); vacc = a4v::dest<1, 1, 1, 1>(vacc, res); }
    { a4::vec4 mul_res = a4v::mul(vf17, a4v::bcast<3>(vf8)); a4::vec4 res = a4v::addK<1, 1>(vacc, mul_res); a4::q128 mask = a4v::set_epi32(-1, -1, -1, -1); vf10 = a4v::blendv_ps(vf10, res, a4v::castsi128_ps(mask)); }
    A4_W128(ADD32((uint32_t)a0, 0), a4v::castps_si128(vf10));
    v1 = sp + 0x50u;
    vf5 = a4v::castsi128_ps(A4_R128(ADD32((uint32_t)a0, 0)));
    vf6 = a4v::castsi128_ps(A4_R128(ADD32(v1, 0)));
    { a4::vec4 res = a4v::mul(vf5, vf6); a4::q128 mask = a4v::set_epi32(0, -1, -1, -1); vf5 = a4v::blend(vf5, res, a4v::castsi128_ps(mask)); }
    A4_W128(ADD32((uint32_t)a0, 0), a4v::castps_si128(vf5));
    vf7 = a4v::castsi128_ps(A4_R128(ADD32((uint32_t)a0, 0)));
    { a4::vec4 res = a4v::mul(vf7, vf7); a4::q128 mask = a4v::set_epi32(0, -1, -1, -1); vf7 = a4v::blend(vf7, res, a4v::castsi128_ps(mask)); }
    { a4::vec4 res = a4v::addK<1, 1>(vf7, a4v::bcast<1>(vf7)); a4::q128 mask = a4v::set_epi32(0, 0, 0, -1); vf7 = a4v::blendv_ps(vf7, res, a4v::castsi128_ps(mask)); }
    { a4::vec4 res = a4v::addK<1, 1>(vf7, a4v::bcast<2>(vf7)); a4::q128 mask = a4v::set_epi32(0, 0, 0, -1); vf7 = a4v::blendv_ps(vf7, res, a4v::castsi128_ps(mask)); }
    do { const a4::q128 _v = (a4v::castps_si128(vf7)); h2 = a4::hi64(_v); std::memcpy(&v0, &_v, 8); } while (0);
    { uint32_t bits = (uint32_t)v0; std::memcpy(&f0, &bits, sizeof(bits)); }
    fcr31 = (a1f::le(f0, f3)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
    { const bool c = (fcr31 & 0x800000u) != 0u; (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 1)))); if (c) goto L_1136538; }
    (v0 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL));
L_1136538:;
    { sp = sp + 0x90u; io_vf1 = vf1; io_vf2 = vf2; io_vf3 = vf3; io_vf4 = vf4; io_vf5 = vf5; io_vf6 = vf6; io_vf7 = vf7; io_vf8 = vf8; io_vf9 = vf9; io_vf10 = vf10; io_vf14 = vf14; io_vf15 = vf15; io_vf16 = vf16; io_vf17 = vf17; io_vacc = vacc; io_h2 = h2; io_h8 = h8; io_h9 = h9; io_h10 = h10; io_h11 = h11; io_h12 = h12; io_h13 = h13; io_h14 = h14; io_h15 = h15; return a1::RetVF{(uint64_t)v0, f0}; }
    __builtin_unreachable();
}

// CheckCylinderVolume (0x1136c68): typed by tools/typed_lift2.py (round A6; guest frame calls, extra outputs xo[0]=a1 xo[1]=t0 xo[2]=t1 xo[3]=t2 xo[4]=t3; VU0 in vf2 vf9 vf10 vacc, out vf1 vf2 vf3 vf4 vf7 vf8 vf9 vf10 vf14 vf15 vf16 vf17 vacc h2 h8 h9 h10 h11 h12 h13 h14 h15, relocated calls 1)
inline a1::RetVF CheckCylinderVolume(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint64_t a0, uint64_t a1, uint64_t a2, uint32_t sp, a4::vec4 &io_vf1, a4::vec4 &io_vf2, a4::vec4 &io_vf3, a4::vec4 &io_vf4, a4::vec4 &io_vf7, a4::vec4 &io_vf8, a4::vec4 &io_vf9, a4::vec4 &io_vf10, a4::vec4 &io_vf14, a4::vec4 &io_vf15, a4::vec4 &io_vf16, a4::vec4 &io_vf17, a4::vec4 &io_vacc, uint64_t &io_h2, uint64_t &io_h8, uint64_t &io_h9, uint64_t &io_h10, uint64_t &io_h11, uint64_t &io_h12, uint64_t &io_h13, uint64_t &io_h14, uint64_t &io_h15, uint64_t *xo = nullptr)
{
    float f0 = 0.0f;
    float f1 = 0.0f;
    uint64_t v0 = 0;
    uint32_t v1 = 0;
    uint64_t t0 = 0;
    uint64_t t1 = 0;
    uint64_t t2 = 0;
    uint64_t t3 = 0;
    uint64_t s0 = 0;
    uint64_t s1 = 0;
    uint32_t fcr31 = 0;
    a4::vec4 vf1 = io_vf1;
    a4::vec4 vf2 = io_vf2;
    a4::vec4 vf3 = io_vf3;
    a4::vec4 vf4 = io_vf4;
    a4::vec4 vf7 = io_vf7;
    a4::vec4 vf8 = io_vf8;
    a4::vec4 vf9 = io_vf9;
    a4::vec4 vf10 = io_vf10;
    a4::vec4 vf14 = io_vf14;
    a4::vec4 vf15 = io_vf15;
    a4::vec4 vf16 = io_vf16;
    a4::vec4 vf17 = io_vf17;
    a4::vec4 vacc = io_vacc;
    uint64_t h2 = io_h2;
    uint64_t h8 = io_h8;
    uint64_t h9 = io_h9;
    uint64_t h10 = io_h10;
    uint64_t h11 = io_h11;
    uint64_t h12 = io_h12;
    uint64_t h13 = io_h13;
    uint64_t h14 = io_h14;
    uint64_t h15 = io_h15;
    sp = sp - 0x70u;
    (v0 = (uint64_t)((uint64_t)a0 + (uint64_t)0ULL));
    (s0 = (uint64_t)((uint64_t)a1 + (uint64_t)0ULL));
    (a0 = (uint64_t)((uint64_t)a1_sx64(sp) + (uint64_t)0ULL));
    (a1 = (uint64_t)((uint64_t)v0 + (uint64_t)0ULL));
    { (s1 = (uint64_t)((uint64_t)a2 + (uint64_t)0ULL)); A3_CHARGE(); uint64_t a7c[4] = {0u, 0u, 0u, 0u}; iosInverseMatrix(ram, rt, a0, a1, vf1, vf2, vf3, vf4, vf8, vf9, vacc, h8, h9, h10, h11, h12, h13, h14, h15, a7c); t0 = a7c[0]; t1 = a7c[1]; t2 = a7c[2]; t3 = a7c[3]; }
    v1 = sp + 0x40u;
    vf8 = a4v::castsi128_ps(A4_R128(ADD32((uint32_t)s0, 0)));
    vf14 = a4v::castsi128_ps(A4_R128(ADD32(sp, 0)));
    vf15 = a4v::castsi128_ps(A4_R128(ADD32(sp, 16)));
    vf16 = a4v::castsi128_ps(A4_R128(ADD32(sp, 32)));
    vf17 = a4v::castsi128_ps(A4_R128(ADD32(sp, 48)));
    { a4::vec4 res = a4v::mul(vf14, a4v::bcast<0>(vf8)); vacc = a4v::dest<1, 1, 1, 1>(vacc, res); }
    { a4::vec4 mul_res = a4v::mul(vf15, a4v::bcast<1>(vf8)); a4::vec4 res = a4v::addK<1, 1>(vacc, mul_res); vacc = a4v::dest<1, 1, 1, 1>(vacc, res); }
    { a4::vec4 mul_res = a4v::mul(vf16, a4v::bcast<2>(vf8)); a4::vec4 res = a4v::addK<1, 1>(vacc, mul_res); vacc = a4v::dest<1, 1, 1, 1>(vacc, res); }
    { a4::vec4 mul_res = a4v::mul(vf17, a4v::bcast<3>(vf8)); a4::vec4 res = a4v::addK<1, 1>(vacc, mul_res); a4::q128 mask = a4v::set_epi32(-1, -1, -1, -1); vf10 = a4v::blendv_ps(vf10, res, a4v::castsi128_ps(mask)); }
    A4_W128(ADD32(v1, 0), a4v::castps_si128(vf10));
    f1 = a1_flt(A1_R32B(sp, 68));
    (a1 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL));
    { uint32_t bits = A1_R32B((uint32_t)s1, 4); float f; std::memcpy(&f, &bits, sizeof(f)); f0 = f; }
    fcr31 = (a1f::le(f0, f1)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
    { const bool c = (fcr31 & 0x800000u) == 0u; (a0 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL)); if (c) goto L_1136ce4; }
    { uint32_t bits = A1_R32B((uint32_t)s1, 8); float f; std::memcpy(&f, &bits, sizeof(f)); f0 = f; }
    fcr31 = (a1f::le(f1, f0)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
    if ((fcr31 & 0x800000u) != 0u) { (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 1)))); goto L_1136ce4; }
L_1136ce4:;
    vf7 = a4v::castsi128_ps(A4_R128(ADD32(v1, 0)));
    { a4::vec4 res = a4v::mul(vf7, vf7); a4::q128 mask = a4v::set_epi32(0, -1, 0, -1); vf7 = a4v::blend(vf7, res, a4v::castsi128_ps(mask)); }
    { a4::vec4 res = a4v::addK<1, 1>(vf7, a4v::bcast<2>(vf7)); a4::q128 mask = a4v::set_epi32(0, 0, 0, -1); vf7 = a4v::blendv_ps(vf7, res, a4v::castsi128_ps(mask)); }
    do { const a4::q128 _v = (a4v::castps_si128(vf7)); h2 = a4::hi64(_v); std::memcpy(&v0, &_v, 8); } while (0);
    { uint32_t bits = (uint32_t)v0; std::memcpy(&f1, &bits, sizeof(bits)); }
    { uint32_t bits = A1_R32B((uint32_t)s1, 0); float f; std::memcpy(&f, &bits, sizeof(f)); f0 = f; }
    f0 = a1f::mul(f0, f0);
    fcr31 = (a1f::lt(f1, f0)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
    if ((fcr31 & 0x800000u) != 0u) { (a1 = (uint64_t)(((uint64_t)0ULL < (uint64_t)RC_U64X(a0)) ? 1 : 0)); goto L_1136d0c; }
L_1136d0c:;
    (v0 = (uint64_t)((uint64_t)RC_U64X(a1) + (uint64_t)0ULL));
    { sp = sp + 0x70u; if (xo) { xo[0] = a1; xo[1] = t0; xo[2] = t1; xo[3] = t2; xo[4] = t3; } io_vf1 = vf1; io_vf2 = vf2; io_vf3 = vf3; io_vf4 = vf4; io_vf7 = vf7; io_vf8 = vf8; io_vf9 = vf9; io_vf10 = vf10; io_vf14 = vf14; io_vf15 = vf15; io_vf16 = vf16; io_vf17 = vf17; io_vacc = vacc; io_h2 = h2; io_h8 = h8; io_h9 = h9; io_h10 = h10; io_h11 = h11; io_h12 = h12; io_h13 = h13; io_h14 = h14; io_h15 = h15; return a1::RetVF{(uint64_t)v0, f0}; }
    __builtin_unreachable();
}

// GetRateInOutEllipticalSphereVolume (0x1136d28): typed by tools/typed_lift2.py (round A6; guest frame calls; VU0 in vf2 vf9 vf10 vacc, out vf1 vf2 vf3 vf4 vf5 vf6 vf7 vf8 vf9 vf10 vf14 vf15 vf16 vf17 vacc h2 h8 h9 h10 h11 h12 h13 h14 h15, relocated calls 1)
inline a1::RetVF GetRateInOutEllipticalSphereVolume(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint64_t a0, uint64_t a1, uint64_t a2, uint64_t a3, uint64_t sp, a4::vec4 &io_vf1, a4::vec4 &io_vf2, a4::vec4 &io_vf3, a4::vec4 &io_vf4, a4::vec4 &io_vf5, a4::vec4 &io_vf6, a4::vec4 &io_vf7, a4::vec4 &io_vf8, a4::vec4 &io_vf9, a4::vec4 &io_vf10, a4::vec4 &io_vf14, a4::vec4 &io_vf15, a4::vec4 &io_vf16, a4::vec4 &io_vf17, a4::vec4 &io_vacc, uint64_t &io_h2, uint64_t &io_h8, uint64_t &io_h9, uint64_t &io_h10, uint64_t &io_h11, uint64_t &io_h12, uint64_t &io_h13, uint64_t &io_h14, uint64_t &io_h15)
{
    float f0 = 0.0f;
    uint64_t v0 = 0;
    uint64_t s0 = 0;
    uint64_t s1 = 0;
    uint64_t s2 = 0;
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
    a4::vec4 vf14 = io_vf14;
    a4::vec4 vf15 = io_vf15;
    a4::vec4 vf16 = io_vf16;
    a4::vec4 vf17 = io_vf17;
    a4::vec4 vacc = io_vacc;
    uint64_t h2 = io_h2;
    uint64_t h8 = io_h8;
    uint64_t h9 = io_h9;
    uint64_t h10 = io_h10;
    uint64_t h11 = io_h11;
    uint64_t h12 = io_h12;
    uint64_t h13 = io_h13;
    uint64_t h14 = io_h14;
    uint64_t h15 = io_h15;
    (sp = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)sp, 4294967200))));
    (v0 = (uint64_t)((uint64_t)a0 + (uint64_t)0ULL));
    (s2 = (uint64_t)((uint64_t)a1 + (uint64_t)0ULL));
    (a0 = (uint64_t)((uint64_t)RC_U64X(sp) + (uint64_t)0ULL));
    (a1 = (uint64_t)((uint64_t)v0 + (uint64_t)0ULL));
    (s0 = (uint64_t)((uint64_t)a2 + (uint64_t)0ULL));
    { (s1 = (uint64_t)((uint64_t)a3 + (uint64_t)0ULL)); A3_CHARGE(); iosInverseMatrix(ram, rt, a0, a1, vf1, vf2, vf3, vf4, vf8, vf9, vacc, h8, h9, h10, h11, h12, h13, h14, h15); }
    (a0 = (uint64_t)((uint64_t)sp + (uint64_t)0ULL));
    (a1 = (uint64_t)((uint64_t)s2 + (uint64_t)0ULL));
    (a2 = (uint64_t)((uint64_t)s0 + (uint64_t)0ULL));
    { (a3 = (uint64_t)((uint64_t)s1 + (uint64_t)0ULL)); A3_CHARGE(); { const a1::RetVF a2_r = GetRateInOutEllipticalSphereVolumeWithInverseMatrix(ram, rt, (uint32_t)a0, (uint32_t)a1, (uint32_t)a2, (uint32_t)a3, (uint32_t)sp, vf5, vf6, vf7, vf8, vf10, vf14, vf15, vf16, vf17, vacc, h2); v0 = a2_r.v0; f0 = a2_r.f0; } }
    { (sp = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)sp, 96)))); io_vf1 = vf1; io_vf2 = vf2; io_vf3 = vf3; io_vf4 = vf4; io_vf5 = vf5; io_vf6 = vf6; io_vf7 = vf7; io_vf8 = vf8; io_vf9 = vf9; io_vf10 = vf10; io_vf14 = vf14; io_vf15 = vf15; io_vf16 = vf16; io_vf17 = vf17; io_vacc = vacc; io_h2 = h2; io_h8 = h8; io_h9 = h9; io_h10 = h10; io_h11 = h11; io_h12 = h12; io_h13 = h13; io_h14 = h14; io_h15 = h15; return a1::RetVF{(uint64_t)v0, f0}; }
    __builtin_unreachable();
}

// sound3DCalc (0x1157f98): typed by tools/typed_lift2.py (round A6; guest frame calls, extra outputs xo[0]=f0; VU0 in vf0 vf2 vf9 vf10 vacc vstat h6, out vf1 vf2 vf3 vf4 vf5 vf6 vf7 vf8 vf9 vf10 vf14 vf15 vf16 vacc vq vstat h2 h8 h9 h10 h11 h12 h13 h14 h15, relocated calls 5)
inline uint64_t sound3DCalc(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint64_t a0, uint64_t a1, uint64_t a2, uint64_t a3, uint64_t sp, float f13, float f14, float f15, float f16, float f17, float f18, float f19, float f20, const a4::vec4 &io_vf0, a4::vec4 &io_vf1, a4::vec4 &io_vf2, a4::vec4 &io_vf3, a4::vec4 &io_vf4, a4::vec4 &io_vf5, a4::vec4 &io_vf6, a4::vec4 &io_vf7, a4::vec4 &io_vf8, a4::vec4 &io_vf9, a4::vec4 &io_vf10, a4::vec4 &io_vf14, a4::vec4 &io_vf15, a4::vec4 &io_vf16, a4::vec4 &io_vacc, float &io_vq, uint16_t &io_vstat, uint64_t &io_h2, uint64_t io_h6, uint64_t &io_h8, uint64_t &io_h9, uint64_t &io_h10, uint64_t &io_h11, uint64_t &io_h12, uint64_t &io_h13, uint64_t &io_h14, uint64_t &io_h15, uint64_t *xo = nullptr)
{
    float f0 = 0.0f;
    float f1 = 0.0f;
    float f2 = 0.0f;
    float f3 = 0.0f;
    float f4 = 0.0f;
    float f5 = 0.0f;
    float f12 = 0.0f;
    uint32_t at = 0;
    uint64_t v0 = 0;
    uint32_t v1 = 0;
    uint64_t s0 = 0;
    uint64_t s1 = 0;
    uint64_t s2 = 0;
    uint32_t s3 = 0;
    uint64_t s4 = 0;
    uint64_t s5 = 0;
    uint32_t fcr31 = 0;
    a4::vec4 vf0 = io_vf0;
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
    a4::vec4 vf14 = io_vf14;
    a4::vec4 vf15 = io_vf15;
    a4::vec4 vf16 = io_vf16;
    a4::vec4 vacc = io_vacc;
    float vq = io_vq;
    uint16_t vstat = io_vstat;
    uint64_t h2 = io_h2;
    uint64_t h6 = io_h6;
    uint64_t h8 = io_h8;
    uint64_t h9 = io_h9;
    uint64_t h10 = io_h10;
    uint64_t h11 = io_h11;
    uint64_t h12 = io_h12;
    uint64_t h13 = io_h13;
    uint64_t h14 = io_h14;
    uint64_t h15 = io_h15;
    bool a3xw0 = false;
    (sp = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)sp, 4294967200))));
    { const uint32_t _dynInsn = A1_R32(0x1157F9Cu); const int32_t _dynSimm = (int16_t)(_dynInsn & 0xFFFFu); const uint32_t _dynImm = _dynInsn & 0xFFFFu; (void)_dynSimm; (void)_dynImm; (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)_dynImm << 16)))); }
    (s1 = (uint64_t)((uint64_t)a2 + (uint64_t)0ULL));
    (s2 = (uint64_t)((uint64_t)a3 + (uint64_t)0ULL));
    (s4 = (uint64_t)((uint64_t)a1 + (uint64_t)0ULL));
    (s5 = (uint64_t)((uint64_t)a0 + (uint64_t)0ULL));
    { float f = f20; uint32_t bits; std::memcpy(&bits, &f, sizeof(bits)); A1_W32B((uint32_t)sp, 88, bits); }
    { const uint32_t _dynInsn = A1_R32(0x1157FD0u); const int32_t _dynSimm = (int16_t)(_dynInsn & 0xFFFFu); const uint32_t _dynImm = _dynInsn & 0xFFFFu; (void)_dynSimm; (void)_dynImm; (a1 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32(ADD32((uint32_t)v0, _dynSimm))))); }
    (v1 = (uint32_t)((int32_t)((int32_t)A1_R32B((uint32_t)s5, 68))));
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)a1, 8))));
    { (s0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B(v1, 8)))); A3_CHARGE(); v0 = getEditWorkInitData(ram, rt, a0); }
    (s0 = (uint64_t)((int64_t)(int32_t)((int32_t)SRL32((uint32_t)s0, 18))));
    (s0 = (uint64_t)(RC_U64X(s0) & (uint64_t)(uint16_t)1023));
    (v1 = (uint32_t)((int32_t)((int32_t)SLL32((uint32_t)s0, 1))));
    (v1 = (uint32_t)((int32_t)((int32_t)ADD32(v1, (uint32_t)s0))));
    v1 = v1 << 2;
    { const bool c = (int64_t)s1 == 0; (s3 = (uint32_t)((int32_t)((int32_t)ADD32((uint32_t)v0, v1)))); if (c) goto L_1158158; }
    vf5 = a4v::castsi128_ps(A4_R128(ADD32((uint32_t)s4, 0)));
    vf6 = a4v::castsi128_ps(A4_R128(ADD32((uint32_t)s1, 0)));
    { a4::vec4 res = a4v::sub(vf5, vf6); a4::q128 mask = a4v::set_epi32(0, -1, -1, -1); vf5 = a4v::blend(vf5, res, a4v::castsi128_ps(mask)); }
    A4_W128(ADD32((uint32_t)sp, 0), a4v::castps_si128(vf5));
    vf7 = a4v::castsi128_ps(A4_R128(ADD32((uint32_t)sp, 0)));
    { a4::vec4 res = a4v::mul(vf7, vf7); a4::q128 mask = a4v::set_epi32(0, -1, -1, -1); vf7 = a4v::blend(vf7, res, a4v::castsi128_ps(mask)); }
    { a4::vec4 res = a4v::addK<1, 1>(vf7, a4v::bcast<1>(vf7)); a4::q128 mask = a4v::set_epi32(0, 0, 0, -1); vf7 = a4v::blendv_ps(vf7, res, a4v::castsi128_ps(mask)); }
    { a4::vec4 res = a4v::addK<1, 1>(vf7, a4v::bcast<2>(vf7)); a4::q128 mask = a4v::set_epi32(0, 0, 0, -1); vf7 = a4v::blendv_ps(vf7, res, a4v::castsi128_ps(mask)); }
    do { const a4::q128 _v = (a4v::castps_si128(vf7)); h2 = a4::hi64(_v); std::memcpy(&v0, &_v, 8); } while (0);
    { uint32_t bits = (uint32_t)v0; std::memcpy(&f20, &bits, sizeof(bits)); }
    (a0 = (uint64_t)((uint64_t)s2 + (uint64_t)0ULL));
    { (a1 = (uint64_t)((uint64_t)s2 + (uint64_t)0ULL)); A3_CHARGE(); iosInverseMatrix(ram, rt, a0, a1, vf1, vf2, vf3, vf4, vf8, vf9, vacc, h8, h9, h10, h11, h12, h13, h14, h15); }
    (a1 = (uint64_t)((uint64_t)s2 + (uint64_t)0ULL));
    (s0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)sp, 16))));
    (a2 = (uint64_t)((uint64_t)sp + (uint64_t)0ULL));
    { (a0 = (uint64_t)((uint64_t)RC_U64X(s0) + (uint64_t)0ULL)); A3_CHARGE(); iosApplyRotMatrix(ram, rt, (uint32_t)a0, (uint32_t)a1, (uint32_t)a2, vf0, vf8, vf10, vf14, vf15, vf16, vacc); }
    (a0 = (uint64_t)((uint64_t)s0 + (uint64_t)0ULL));
    { (a1 = (uint64_t)((uint64_t)a0 + (uint64_t)0ULL)); A3_CHARGE(); v0 = iosGetStabXZVector(ram, rt, (uint32_t)a0, (uint32_t)a1, vf0, vf5, vf7, vacc, vq, vstat, h2); }
    { uint32_t bits = A1_R32B(s3, 0); float f; std::memcpy(&f, &bits, sizeof(f)); f5 = f; }
    f1 = a1_flt(A1_R32B(s3, 4));
    f2 = a1f::mul(f5, f5);
    f0 = a1_flt(A1_R32B(s3, 8)); a3xw0 = true;
    f3 = a1f::mul(f1, f1);
    fcr31 = (a1f::lt(f2, f3)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
    { const bool c = (fcr31 & 0x800000u) != 0u; f4 = a1f::mul(f0, f0); if (c) goto L_1158078; }
    f2 = ((float)(f3));
L_1158078:;
    fcr31 = (a1f::lt(f20, f2)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
    (at = (uint32_t)((int32_t)((int32_t)((uint32_t)16256 << 16))));
    { uint32_t bits = a7_opq(at); std::memcpy(&f12, &bits, sizeof(bits)); }
    if ((fcr31 & 0x800000u) != 0u) goto L_1158128;
    fcr31 = (a1f::lt(f20, f3)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
    if ((fcr31 & 0x800000u) != 0u) goto L_11580a8;
    { uint32_t bits = 0u; std::memcpy(&f0, &bits, sizeof(bits)); } a3xw0 = true;
    fcr31 = (a1f::eq(f4, f0)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
    if ((fcr31 & 0x800000u) == 0u) goto L_11580d8;
L_11580a8:;
    f0 = a1f::div(fcr31, f20, f3); a3xw0 = true;
    (at = (uint32_t)((int32_t)((int32_t)((uint32_t)16256 << 16))));
    { uint32_t bits = a7_opq(at); std::memcpy(&f1, &bits, sizeof(bits)); }
    f0 = a1f::addFinite(f0, f1); a3xw0 = true;
    f12 = a1f::div(fcr31, f1, f0);
    goto L_1158128;
L_11580d8:;
    fcr31 = (a1f::lt(f20, f4)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
    { const bool c = (fcr31 & 0x800000u) == 0u; f12 = ((float)(f0)); if (c) goto L_1158128; }
    fcr31 = (a1f::le(f4, f3)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
    if ((fcr31 & 0x800000u) != 0u) goto L_1158128;
    f0 = a1f::sub(f20, f3); a3xw0 = true;
    (at = (uint32_t)((int32_t)((int32_t)((uint32_t)16256 << 16))));
    { uint32_t bits = a7_opq(at); std::memcpy(&f1, &bits, sizeof(bits)); }
    f4 = a1f::sub(f4, f3);
    (at = (uint32_t)((int32_t)((int32_t)((uint32_t)16128 << 16))));
    { uint32_t bits = a7_opq(at); std::memcpy(&f2, &bits, sizeof(bits)); }
    f0 = a1f::div(fcr31, f0, f4); a3xw0 = true;
    f0 = a1f::addFinite(f0, f1); a3xw0 = true;
    f1 = a1f::div(fcr31, f1, f0);
    f12 = a1f::subFinite(f1, f2);
L_1158128:;
    { uint32_t bits = 0u; std::memcpy(&f0, &bits, sizeof(bits)); } a3xw0 = true;
    fcr31 = (a1f::lt(f5, f0)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
    { const bool c = (fcr31 & 0x800000u) == 0u; { uint32_t bits = A1_R32B((uint32_t)sp, 16); float f; std::memcpy(&f, &bits, sizeof(f)); f20 = f; } if (c) goto L_1158144; }
    (at = (uint32_t)((int32_t)((int32_t)((uint32_t)16256 << 16))));
    { uint32_t bits = a7_opq(at); std::memcpy(&f0, &bits, sizeof(bits)); } a3xw0 = true;
    f12 = a1f::sub(f0, f12);
L_1158144:;
    { (a0 = (uint64_t)((uint64_t)s5 + (uint64_t)0ULL)); A3_CHARGE(); { const a1::RetVF a2_r = sndSepSetVol_u11(ram, rt, (uint32_t)a0, f12, h2, h6); v0 = a2_r.v0; f0 = a2_r.f0; } a3xw0 = true; }
    (a0 = (uint64_t)((uint64_t)s5 + (uint64_t)0ULL));
    { f12 = ((float)(f20)); A3_CHARGE(); { const a1::RetVF a2_r = sndSepSetPan(ram, rt, (uint32_t)a0, f12); v0 = a2_r.v0; f0 = a2_r.f0; } a3xw0 = true; }
L_1158158:;
    { uint32_t bits = A1_R32B((uint32_t)sp, 88); float f; std::memcpy(&f, &bits, sizeof(f)); f20 = f; }
    { (sp = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)sp, 96)))); if (xo) { if (a3xw0) xo[0] = a1_bits(f0); } io_vf1 = vf1; io_vf2 = vf2; io_vf3 = vf3; io_vf4 = vf4; io_vf5 = vf5; io_vf6 = vf6; io_vf7 = vf7; io_vf8 = vf8; io_vf9 = vf9; io_vf10 = vf10; io_vf14 = vf14; io_vf15 = vf15; io_vf16 = vf16; io_vacc = vacc; io_vq = vq; io_vstat = vstat; io_h2 = h2; io_h8 = h8; io_h9 = h9; io_h10 = h10; io_h11 = h11; io_h12 = h12; io_h13 = h13; io_h14 = h14; io_h15 = h15; return v0; }
    __builtin_unreachable();
}

// create_original_view_matrix (0x1162f20): typed by tools/typed_lift2.py (round A6; frame calls, extra outputs xo[0]=a2 xo[1]=t0 xo[2]=t1 xo[3]=t2 xo[4]=t3; VU0 in vf0 vf2 vf7 vf9 vf14 vf15 vf16 vf17 vacc vstat, out vf1 vf2 vf3 vf4 vf5 vf6 vf7 vf8 vf9 vf14 vf15 vf16 vf17 vacc vq vstat h2 h8 h9 h10 h11 h12 h13 h14 h15, relocated calls 1, 64-bit inputs a0 a1 a3, guest sp, frame 16 bytes, 0 slot accesses)
inline uint64_t create_original_view_matrix(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint64_t w64_a0, uint64_t w64_a1, uint64_t a2, uint64_t w64_a3, uint32_t sp, const a4::vec4 &io_vf0, a4::vec4 &io_vf1, a4::vec4 &io_vf2, a4::vec4 &io_vf3, a4::vec4 &io_vf4, a4::vec4 &io_vf5, a4::vec4 &io_vf6, a4::vec4 &io_vf7, a4::vec4 &io_vf8, a4::vec4 &io_vf9, a4::vec4 &io_vf14, a4::vec4 &io_vf15, a4::vec4 &io_vf16, a4::vec4 &io_vf17, a4::vec4 &io_vacc, float &io_vq, uint16_t &io_vstat, uint64_t &io_h2, uint64_t &io_h8, uint64_t &io_h9, uint64_t &io_h10, uint64_t &io_h11, uint64_t &io_h12, uint64_t &io_h13, uint64_t &io_h14, uint64_t &io_h15, uint64_t *xo = nullptr)
{
    uint32_t a0 = (uint32_t)w64_a0;
    uint32_t a1 = (uint32_t)w64_a1;
    uint32_t a3 = (uint32_t)w64_a3;
    uint64_t t0 = 0;
    uint64_t t1 = 0;
    uint64_t t2 = 0;
    uint64_t t3 = 0;
    a4::vec4 vf0 = io_vf0;
    a4::vec4 vf1 = io_vf1;
    a4::vec4 vf2 = io_vf2;
    a4::vec4 vf3 = io_vf3;
    a4::vec4 vf4 = io_vf4;
    a4::vec4 vf5 = io_vf5;
    a4::vec4 vf6 = io_vf6;
    a4::vec4 vf7 = io_vf7;
    a4::vec4 vf8 = io_vf8;
    a4::vec4 vf9 = io_vf9;
    a4::vec4 vf14 = io_vf14;
    a4::vec4 vf15 = io_vf15;
    a4::vec4 vf16 = io_vf16;
    a4::vec4 vf17 = io_vf17;
    a4::vec4 vacc = io_vacc;
    float vq = io_vq;
    uint16_t vstat = io_vstat;
    uint64_t h2 = io_h2;
    uint64_t h8 = io_h8;
    uint64_t h9 = io_h9;
    uint64_t h10 = io_h10;
    uint64_t h11 = io_h11;
    uint64_t h12 = io_h12;
    uint64_t h13 = io_h13;
    uint64_t h14 = io_h14;
    uint64_t h15 = io_h15;
    { uint64_t a7c[5] = {0u, 0u, 0u, 0u, 0u}; const uint64_t a7_t = iosSetCameraMatrix(ram, rt, w64_a0, w64_a1, a2, w64_a3, a1_sx64((uint32_t)(sp + 0x0u)), vf0, vf1, vf2, vf3, vf4, vf5, vf6, vf7, vf8, vf9, vf14, vf15, vf16, vf17, vacc, vq, vstat, h2, h8, h9, h10, h11, h12, h13, h14, h15, a7c); a2 = a7c[0]; t0 = a7c[1]; t1 = a7c[2]; t2 = a7c[3]; t3 = a7c[4]; if (xo) { xo[0] = a2; xo[1] = t0; xo[2] = t1; xo[3] = t2; xo[4] = t3; } io_vf1 = vf1; io_vf2 = vf2; io_vf3 = vf3; io_vf4 = vf4; io_vf5 = vf5; io_vf6 = vf6; io_vf7 = vf7; io_vf8 = vf8; io_vf9 = vf9; io_vf14 = vf14; io_vf15 = vf15; io_vf16 = vf16; io_vf17 = vf17; io_vacc = vacc; io_vq = vq; io_vstat = vstat; io_h2 = h2; io_h8 = h8; io_h9 = h9; io_h10 = h10; io_h11 = h11; io_h12 = h12; io_h13 = h13; io_h14 = h14; io_h15 = h15; return a7_t; }
    __builtin_unreachable();
}

// SCRGetTargetPosXZDirOnMyCtrlGeo (0x137c360): typed by tools/typed_lift2.py (round A6; guest frame calls; VU0 in vf0 vf7 vf10 vf12 vf13 vf14 vf15 vf16 vf17 vf25 vf28 vf29 vacc vstat h2 h5, out vf5 vf6 vf7 vf8 vf10 vf11 vf12 vf13 vf14 vf15 vf16 vf17 vf24 vf25 vf26 vf28 vf29 vacc vq vstat h2 h4 h8 h9 h10 h11 h12 h13 h14 h15, relocated calls 1)
inline a1::RetVF SCRGetTargetPosXZDirOnMyCtrlGeo(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint64_t v0, uint64_t a0, uint64_t a1, uint64_t a2, uint64_t sp, float f12, float f13, float f14, float f15, float f16, float f17, float f18, float f19, const a4::vec4 &io_vf0, a4::vec4 &io_vf5, a4::vec4 &io_vf6, a4::vec4 &io_vf7, a4::vec4 &io_vf8, a4::vec4 &io_vf10, a4::vec4 &io_vf11, a4::vec4 &io_vf12, a4::vec4 &io_vf13, a4::vec4 &io_vf14, a4::vec4 &io_vf15, a4::vec4 &io_vf16, a4::vec4 &io_vf17, a4::vec4 &io_vf24, a4::vec4 &io_vf25, a4::vec4 &io_vf26, a4::vec4 &io_vf28, a4::vec4 &io_vf29, a4::vec4 &io_vacc, float &io_vq, uint16_t &io_vstat, uint64_t &io_h2, uint64_t &io_h4, uint64_t io_h5, uint64_t &io_h8, uint64_t &io_h9, uint64_t &io_h10, uint64_t &io_h11, uint64_t &io_h12, uint64_t &io_h13, uint64_t &io_h14, uint64_t &io_h15)
{
    float f0 = 0.0f;
    float f1 = 0.0f;
    uint64_t at = 0;
    uint64_t v1 = 0;
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
    uint32_t fcr31 = 0;
    a4::vec4 vf0 = io_vf0;
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
    a4::vec4 vf24 = io_vf24;
    a4::vec4 vf25 = io_vf25;
    a4::vec4 vf26 = io_vf26;
    a4::vec4 vf28 = io_vf28;
    a4::vec4 vf29 = io_vf29;
    a4::vec4 vacc = io_vacc;
    float vq = io_vq;
    uint16_t vstat = io_vstat;
    uint64_t h2 = io_h2;
    uint64_t h4 = io_h4;
    uint64_t h5 = io_h5;
    uint64_t h8 = io_h8;
    uint64_t h9 = io_h9;
    uint64_t h10 = io_h10;
    uint64_t h11 = io_h11;
    uint64_t h12 = io_h12;
    uint64_t h13 = io_h13;
    uint64_t h14 = io_h14;
    uint64_t h15 = io_h15;
    (sp = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)sp, 4294967152))));
    (s0 = (uint64_t)((uint64_t)a0 + (uint64_t)0ULL));
    (a0 = (uint64_t)((uint64_t)a1 + (uint64_t)0ULL));
    { (s1 = (uint64_t)((uint64_t)a2 + (uint64_t)0ULL)); A3_CHARGE(); v0 = GetCharacterClass(ram, rt, a0, h2, h5); }
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)v0, 308))));
    vf5 = a4v::castsi128_ps(A4_R128(ADD32((uint32_t)s1, 0)));
    vf6 = a4v::castsi128_ps(A4_R128(ADD32((uint32_t)v1, 0)));
    { a4::vec4 res = a4v::sub(vf5, vf6); a4::q128 mask = a4v::set_epi32(-1, -1, -1, -1); vf7 = a4v::blend(vf7, res, a4v::castsi128_ps(mask)); }
    A4_W128(ADD32((uint32_t)s0, 0), a4v::castps_si128(vf7));
    do { const a4::q128 _v = (A4_R128(ADD32((uint32_t)v0, 1136))); h4 = a4::hi64(_v); std::memcpy(&a0, &_v, 8); } while (0);
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)sp, 16))));
    A4_W128(ADD32((uint32_t)sp, 0), a4::mk128(h4, a0));
    { a4::vec4 res = a4v::addK<1, 1>(vf0, a4v::bcast<3>(vf0)); a4::q128 mask = a4v::set_epi32(0, -1, -1, -1); vf12 = a4v::blendv_ps(vf12, res, a4v::castsi128_ps(mask)); }
    vf11 = a4v::castsi128_ps(A4_R128(ADD32((uint32_t)sp, 0)));
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
    A4_W128(ADD32((uint32_t)v0, 0), a4v::castps_si128(vf14));
    A4_W128(ADD32((uint32_t)v0, 16), a4v::castps_si128(vf15));
    A4_W128(ADD32((uint32_t)v0, 32), a4v::castps_si128(vf16));
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)sp, 64))));
    A4_W128(ADD32((uint32_t)v1, 0), a4v::castps_si128(vf0));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)sp, 16))));
    vf24 = a4v::castsi128_ps(A4_R128(ADD32((uint32_t)v0, 48)));
    { a4::vec4 res = a4v::subK<1, 1>(vf0, a4v::bcast<3>(vf0)); a4::q128 mask = a4v::set_epi32(0, 0, 0, -1); vf25 = a4v::blendv_ps(vf25, res, a4v::castsi128_ps(mask)); }
    { a4::vec4 res = a4v::mulK<0, 1>(vf24, a4v::bcast<0>(vf25)); a4::q128 mask = a4v::set_epi32(0, -1, -1, -1); vf28 = a4v::blendv_ps(vf28, res, a4v::castsi128_ps(mask)); }
    { a4::vec4 res = a4v::sub(vf28, vf28); a4::q128 mask = a4v::set_epi32(-1, 0, 0, 0); vf28 = a4v::blend(vf28, res, a4v::castsi128_ps(mask)); }
    do { const a4::q128 _v = (A4_R128(ADD32((uint32_t)v0, 0))); h8 = a4::hi64(_v); std::memcpy(&t0, &_v, 8); } while (0);
    do { const a4::q128 _v = (A4_R128(ADD32((uint32_t)v0, 16))); h9 = a4::hi64(_v); std::memcpy(&t1, &_v, 8); } while (0);
    do { const a4::q128 _v = (A4_R128(ADD32((uint32_t)v0, 32))); h10 = a4::hi64(_v); std::memcpy(&t2, &_v, 8); } while (0);
    do { const a4::q128 _v = (a4v::castps_si128(vf0)); h11 = a4::hi64(_v); std::memcpy(&t3, &_v, 8); } while (0);
    do { const a4::q128 _v = (a4m::pextlw(a4::mk128(h9, t1), a4::mk128(h8, t0))); h12 = a4::hi64(_v); std::memcpy(&t4, &_v, 8); } while (0);
    do { const a4::q128 _v = (a4m::pextuw(a4::mk128(h9, t1), a4::mk128(h8, t0))); h13 = a4::hi64(_v); std::memcpy(&t5, &_v, 8); } while (0);
    do { const a4::q128 _v = (a4m::pextlw(a4::mk128(h11, t3), a4::mk128(h10, t2))); h14 = a4::hi64(_v); std::memcpy(&t6, &_v, 8); } while (0);
    do { const a4::q128 _v = (a4m::pextuw(a4::mk128(h11, t3), a4::mk128(h10, t2))); h15 = a4::hi64(_v); std::memcpy(&t7, &_v, 8); } while (0);
    do { const a4::q128 _v = (a4m::pcpyld(a4::mk128(h14, t6), a4::mk128(h12, t4))); h8 = a4::hi64(_v); std::memcpy(&t0, &_v, 8); } while (0);
    do { const a4::q128 _v = (a4v::unpackhi_epi64(a4::mk128(h12, t4), a4::mk128(h14, t6))); h9 = a4::hi64(_v); std::memcpy(&t1, &_v, 8); } while (0);
    do { const a4::q128 _v = (a4m::pcpyld(a4::mk128(h15, t7), a4::mk128(h13, t5))); h10 = a4::hi64(_v); std::memcpy(&t2, &_v, 8); } while (0);
    do { const a4::q128 _v = (a4v::unpackhi_epi64(a4::mk128(h13, t5), a4::mk128(h15, t7))); h11 = a4::hi64(_v); std::memcpy(&t3, &_v, 8); } while (0);
    vf24 = a4v::castsi128_ps(a4::mk128(h8, t0));
    vf25 = a4v::castsi128_ps(a4::mk128(h9, t1));
    vf26 = a4v::castsi128_ps(a4::mk128(h10, t2));
    { a4::vec4 res = a4v::mulK<0, 1>(vf24, a4v::bcast<0>(vf28)); vacc = a4v::dest<1, 1, 1, 1>(vacc, res); }
    { a4::vec4 mul_res = a4v::mulK<0, 1>(vf25, a4v::bcast<1>(vf28)); a4::vec4 res = a4v::addK<1, 1>(vacc, mul_res); vacc = a4v::dest<1, 1, 1, 1>(vacc, res); }
    { a4::vec4 mul_res = a4v::mulK<0, 1>(vf26, a4v::bcast<2>(vf28)); a4::vec4 res = a4v::addK<1, 1>(vacc, mul_res); vacc = a4v::dest<1, 1, 1, 1>(vacc, res); }
    { a4::vec4 mul_res = a4v::mulK<1, 1>(vf0, a4v::bcast<3>(vf28)); a4::vec4 res = a4v::addK<1, 1>(vacc, mul_res); a4::q128 mask = a4v::set_epi32(-1, -1, -1, -1); vf29 = a4v::blendv_ps(vf29, res, a4v::castsi128_ps(mask)); }
    { a4::vec4 res = a4v::subK<1, 1>(vf29, vf29); a4::q128 mask = a4v::set_epi32(-1, 0, 0, 0); vf29 = a4v::blend(vf29, res, a4v::castsi128_ps(mask)); }
    { a4::vec4 res = a4v::addK<1, 1>(vf29, vf0); a4::q128 mask = a4v::set_epi32(-1, 0, 0, 0); vf29 = a4v::blend(vf29, res, a4v::castsi128_ps(mask)); }
    A4_W128(ADD32((uint32_t)v0, 0), a4v::castps_si128(vf24));
    A4_W128(ADD32((uint32_t)v0, 16), a4v::castps_si128(vf25));
    A4_W128(ADD32((uint32_t)v0, 32), a4v::castps_si128(vf26));
    A4_W128(ADD32((uint32_t)v0, 48), a4v::castps_si128(vf29));
    vf8 = a4v::castsi128_ps(A4_R128(ADD32((uint32_t)s0, 0)));
    vf14 = a4v::castsi128_ps(A4_R128(ADD32((uint32_t)v0, 0)));
    vf15 = a4v::castsi128_ps(A4_R128(ADD32((uint32_t)v0, 16)));
    vf16 = a4v::castsi128_ps(A4_R128(ADD32((uint32_t)v0, 32)));
    vf17 = a4v::castsi128_ps(A4_R128(ADD32((uint32_t)v0, 48)));
    { a4::vec4 res = a4v::mul(vf14, a4v::bcast<0>(vf8)); vacc = a4v::dest<1, 1, 1, 1>(vacc, res); }
    { a4::vec4 mul_res = a4v::mul(vf15, a4v::bcast<1>(vf8)); a4::vec4 res = a4v::addK<1, 1>(vacc, mul_res); vacc = a4v::dest<1, 1, 1, 1>(vacc, res); }
    { a4::vec4 mul_res = a4v::mul(vf16, a4v::bcast<2>(vf8)); a4::vec4 res = a4v::addK<1, 1>(vacc, mul_res); vacc = a4v::dest<1, 1, 1, 1>(vacc, res); }
    { a4::vec4 mul_res = a4v::mul(vf17, a4v::bcast<3>(vf8)); a4::vec4 res = a4v::addK<1, 1>(vacc, mul_res); a4::q128 mask = a4v::set_epi32(-1, -1, -1, -1); vf10 = a4v::blendv_ps(vf10, res, a4v::castsi128_ps(mask)); }
    A4_W128(ADD32((uint32_t)s0, 0), a4v::castps_si128(vf10));
    A1_W32B((uint32_t)s0, 4, 0u);
    vf7 = a4v::castsi128_ps(A4_R128(ADD32((uint32_t)s0, 0)));
    { a4::vec4 res = a4v::mul(vf7, vf7); a4::q128 mask = a4v::set_epi32(0, -1, -1, -1); vf7 = a4v::blend(vf7, res, a4v::castsi128_ps(mask)); }
    { a4::vec4 res = a4v::mulK<1, 1>(vf0, a4v::bcast<0>(vf7)); vacc = a4v::dest<0, 0, 0, 1>(vacc, res); }
    { a4::vec4 mul_res = a4v::mulK<1, 1>(vf0, a4v::bcast<1>(vf7)); a4::vec4 res = a4v::addK<1, 1>(vacc, mul_res); vacc = a4v::dest<0, 0, 0, 1>(vacc, res); }
    { a4::vec4 mul_res = a4v::mulK<1, 1>(vf0, a4v::bcast<2>(vf7)); a4::vec4 res = a4v::addK<1, 1>(vacc, mul_res); a4::q128 mask = a4v::set_epi32(-1, 0, 0, 0); vf7 = a4v::blendv_ps(vf7, res, a4v::castsi128_ps(mask)); }
    { float ft = a4v::cvtss_f32(a4v::shuffle_ps<0x03>(vf7, vf7)); vq = a4v::sqrt(vstat, ft); }
    { uint32_t bits; std::memcpy(&bits, &vq, sizeof(bits)); (v0 = (uint64_t)((int64_t)(int32_t)(bits))); }
    { uint32_t bits = (uint32_t)v0; std::memcpy(&f0, &bits, sizeof(bits)); }
    (at = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)16256 << 16))));
    { uint32_t bits = (uint32_t)at; std::memcpy(&f1, &bits, sizeof(bits)); }
    f1 = a1f::div(fcr31, f1, f0);
    { uint32_t bits; std::memcpy(&bits, &f1, sizeof(bits)); (v0 = (uint64_t)((int64_t)(int32_t)(bits))); }
    vf5 = a4v::castsi128_ps(A4_R128(ADD32((uint32_t)s0, 0)));
    vf6 = a4v::castsi128_ps(a4::mk128(h2, v0));
    { a4::vec4 res = a4v::mul(vf5, a4v::bcast<0>(vf6)); a4::q128 mask = a4v::set_epi32(0, -1, -1, -1); vf5 = a4v::blendv_ps(vf5, res, a4v::castsi128_ps(mask)); }
    A4_W128(ADD32((uint32_t)s0, 0), a4v::castps_si128(vf5));
    { (sp = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)sp, 144)))); io_vf5 = vf5; io_vf6 = vf6; io_vf7 = vf7; io_vf8 = vf8; io_vf10 = vf10; io_vf11 = vf11; io_vf12 = vf12; io_vf13 = vf13; io_vf14 = vf14; io_vf15 = vf15; io_vf16 = vf16; io_vf17 = vf17; io_vf24 = vf24; io_vf25 = vf25; io_vf26 = vf26; io_vf28 = vf28; io_vf29 = vf29; io_vacc = vacc; io_vq = vq; io_vstat = vstat; io_h2 = h2; io_h4 = h4; io_h8 = h8; io_h9 = h9; io_h10 = h10; io_h11 = h11; io_h12 = h12; io_h13 = h13; io_h14 = h14; io_h15 = h15; return a1::RetVF{(uint64_t)v0, f0}; }
    __builtin_unreachable();
}

// iosGsResizeRootDC (0x1c1770): typed by tools/typed_lift2.py (round A6; loop frame calls; VU0 in -, out h2 h3, guest sp, frame 48 bytes, 0 slot accesses)
inline uint64_t iosGsResizeRootDC(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t sp, uint64_t &io_h2, uint64_t &io_h3)
{
    uint64_t v0 = 0;
    uint32_t v1 = 0;
    uint32_t a0 = 0;
    uint64_t a1 = 0;
    uint64_t a2 = 0;
    uint64_t a3 = 0;
    uint32_t t0 = 0;
    uint32_t t1 = 0;
    uint32_t s0 = 0;
    uint64_t s1 = 0;
    uint32_t s2 = 0;
    uint64_t s3 = 0;
    uint64_t hi = 0;
    uint64_t lo = 0;
    uint64_t h2 = io_h2;
    uint64_t h3 = io_h3;
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)33 << 16))));
    v1 = 0x1E0000u;
    (s0 = (uint32_t)((int32_t)((int32_t)ADD32((uint32_t)v0, 14352))));
    (s1 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL));
    (s2 = (uint32_t)((int32_t)((int32_t)ADD32(v1, 4336))));
    (s3 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 1))));
    (a1 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B(s0, 8))));
L_1c17a8:;
    (s3 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)s3, 4294967295))));
    t0 = A1_R32B(s0, 12);
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B(s0, 4))));
    (a3 = (uint64_t)((uint64_t)a1 + (uint64_t)0ULL));
    (a2 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B(s0, 0))));
    { int64_t result = (int64_t)(int32_t)a1 * (int64_t)(int32_t)t0; lo = (uint64_t)(int64_t)(int32_t)result; hi = (uint64_t)(int64_t)(int32_t)(result >> 32); (a1 = (uint64_t)((int64_t)(int32_t)((int32_t)result))); }
    a0 = A1_R32B(s0, 136);
    { int64_t result = (int64_t)(int32_t)a2 * (int64_t)(int32_t)v0; lo = (uint64_t)(int64_t)(int32_t)result; hi = (uint64_t)(int64_t)(int32_t)(result >> 32); (a2 = (uint64_t)((int64_t)(int32_t)((int32_t)result))); }
    (a0 = (uint32_t)((int32_t)((int32_t)ADD32(a0, (uint32_t)s1))));
    (a1 = (uint64_t)((int64_t)(int32_t)(SRA32((int32_t)a1, 11))));
    (a2 = (uint64_t)((int64_t)(int32_t)(SRA32((int32_t)a2, 11))));
    { (a1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)a2, (uint32_t)a1)))); A3_CHARGE(); v0 = iosSetDCParam(ram, rt, a1_sx64(a0), a1, a2, a3, a1_sx64(t0), (uint32_t)(sp + 0xFFFFFFD0u), h2, h3); }
    a0 = A1_R32B(s0, 136);
    (a2 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B(s2, 0))));
    (a1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 1))));
    (a3 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B(s2, 4))));
    (a0 = (uint32_t)((int32_t)((int32_t)ADD32(a0, (uint32_t)s1))));
    t0 = A1_R32B(s2, 8);
    t1 = A1_R32B(s2, 12);
    { A3_CHARGE(); v0 = iosSetDCClearParam(ram, rt, a1_sx64(a0), a1, a2, a3, a1_sx64(t0), a1_sx64(t1), (uint32_t)(sp + 0xFFFFFFD0u), h2, h3); }
    a0 = A1_R32B(s0, 136);
    (a3 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B(s0, 8))));
    (a1 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL));
    t0 = A1_R32B(s0, 12);
    (a0 = (uint32_t)((int32_t)((int32_t)ADD32(a0, (uint32_t)s1))));
    (a2 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL));
    { (s1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)s1, 400)))); A3_CHARGE(); v0 = iosSetDCViewPort(ram, rt, a1_sx64(a0), a1, a2, a3, a1_sx64(t0)); }
    if ((int32_t)s3 >= 0) { (a1 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B(s0, 8)))); { A3_BACK(); goto L_1c17a8; } }
    { io_h2 = h2; io_h3 = h3; return v0; }
    __builtin_unreachable();
}

// GetMotionBasePos (0x10b6d18): typed by tools/typed_lift2.py (round A6; guest frame calls; VU0 in vf0 vf1 vf2 vf3 vf4 vf10 vf12 vf13 vf14 vf15 vf16 vf17 vi15 vacc, out vf1 vf2 vf3 vf4 vf8 vf10 vf11 vf12 vf13 vf14 vf15 vf16 vf17 vi15 vacc h2, relocated calls 7)
inline uint64_t GetMotionBasePos(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint64_t a0, uint64_t a1, uint64_t a2, uint64_t a3, uint64_t sp, float f0, float f1, float f2, float f3, float f4, float f5, float f6, float f7, float f8, float f9, float f10, float f11, float f12, float f13, float f14, float f15, float f16, float f17, float f18, float f19, float f20, float f21, float f22, float f23, float f24, float f25, float f26, float f27, float f28, float f29, float f30, float f31, const a4::vec4 &io_vf0, a4::vec4 &io_vf1, a4::vec4 &io_vf2, a4::vec4 &io_vf3, a4::vec4 &io_vf4, a4::vec4 &io_vf8, a4::vec4 &io_vf10, a4::vec4 &io_vf11, a4::vec4 &io_vf12, a4::vec4 &io_vf13, a4::vec4 &io_vf14, a4::vec4 &io_vf15, a4::vec4 &io_vf16, a4::vec4 &io_vf17, uint16_t &io_vi15, a4::vec4 &io_vacc, uint64_t &io_h2)
{
    uint64_t v0 = 0;
    uint64_t s0 = 0;
    uint64_t s1 = 0;
    uint64_t s2 = 0;
    uint64_t s3 = 0;
    uint64_t s4 = 0;
    uint64_t s5 = 0;
    uint64_t s6 = 0;
    uint64_t s7 = 0;
    a4::vec4 vf0 = io_vf0;
    a4::vec4 vf1 = io_vf1;
    a4::vec4 vf2 = io_vf2;
    a4::vec4 vf3 = io_vf3;
    a4::vec4 vf4 = io_vf4;
    a4::vec4 vf8 = io_vf8;
    a4::vec4 vf10 = io_vf10;
    a4::vec4 vf11 = io_vf11;
    a4::vec4 vf12 = io_vf12;
    a4::vec4 vf13 = io_vf13;
    a4::vec4 vf14 = io_vf14;
    a4::vec4 vf15 = io_vf15;
    a4::vec4 vf16 = io_vf16;
    a4::vec4 vf17 = io_vf17;
    uint16_t vi15 = io_vi15;
    a4::vec4 vacc = io_vacc;
    uint64_t h2 = io_h2;
    (sp = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)sp, 4294967152))));
    (s0 = (uint64_t)((uint64_t)a1 + (uint64_t)0ULL));
    (s1 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL));
    (s4 = (uint64_t)((uint64_t)a3 + (uint64_t)0ULL));
    (s5 = (uint64_t)((uint64_t)a0 + (uint64_t)0ULL));
    (s6 = (uint64_t)((uint64_t)a2 + (uint64_t)0ULL));
    { (s3 = (uint64_t)((uint64_t)s5 + (uint64_t)0ULL)); A3_CHARGE(); iosUnitCurrentMatrix(ram, rt, vf0, vf1, vf2, vf3, vf4); }
    { (a0 = (uint64_t)((uint64_t)s0 + (uint64_t)0ULL)); A3_CHARGE(); iosTransCurrentMatrix(ram, rt, (uint32_t)a0, vf0, vf1, vf2, vf3, vf4, vf8, vacc); }
    (s0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)s6, 8))));
    { (s7 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)s0, 52)))); A3_CHARGE(); a11x::pushMatrix(ram, rt, vf1, vf2, vf3, vf4, vi15); }
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)s0, 16))));
    { (s2 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)s0, 48)))); A3_CHARGE(); iosTransCurrentMatrix(ram, rt, (uint32_t)a0, vf0, vf1, vf2, vf3, vf4, vf8, vacc); }
    { a4::vec4 res = a4v::addK<1, 1>(vf0, a4v::bcast<3>(vf0)); a4::q128 mask = a4v::set_epi32(0, -1, -1, -1); vf12 = a4v::blendv_ps(vf12, res, a4v::castsi128_ps(mask)); }
    vf11 = a4v::castsi128_ps(A4_R128(ADD32((uint32_t)s4, 0)));
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
    A4_W128(ADD32((uint32_t)sp, 0), a4v::castps_si128(vf14));
    A4_W128(ADD32((uint32_t)sp, 16), a4v::castps_si128(vf15));
    A4_W128(ADD32((uint32_t)sp, 32), a4v::castps_si128(vf16));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)sp, 48))));
    A4_W128(ADD32((uint32_t)v0, 0), a4v::castps_si128(vf0));
    (a0 = (uint64_t)((uint64_t)sp + (uint64_t)0ULL));
    { (s1 = (uint64_t)((int64_t)(int32_t)((int32_t)SLL32((uint32_t)s1, 4)))); A3_CHARGE(); iosMulCurrentMatrixR(ram, rt, (uint32_t)a0, vf1, vf2, vf3, vf4, vf14, vf15, vf16, vf17, vacc); }
    (s1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)s1, (uint32_t)s3))));
    (a0 = (uint64_t)((uint64_t)sp + (uint64_t)0ULL));
    { (s0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 4294967295)))); A3_CHARGE(); iosGetCurrentMatrix(ram, rt, (uint32_t)a0, vf1, vf2, vf3, vf4); }
    (a0 = (uint64_t)((uint64_t)s2 + (uint64_t)0ULL));
    do { const a4::q128 _v = (A4_R128(ADD32((uint32_t)sp, 48))); h2 = a4::hi64(_v); std::memcpy(&v0, &_v, 8); } while (0);
    (a1 = (uint64_t)((uint64_t)s5 + (uint64_t)0ULL));
    (a2 = (uint64_t)((uint64_t)s6 + (uint64_t)0ULL));
    (a3 = (uint64_t)((uint64_t)s4 + (uint64_t)0ULL));
    { const bool c = (int64_t)s2 == (int64_t)s0; A4_W128(ADD32((uint32_t)s1, 0), a4::mk128(h2, v0)); if (c) goto L_10b6e30; }
    { A3_CHARGE(); uint64_t a7c[1] = {(uint64_t)a1_bits(f0)}; v0 = manager_sub_010B8958(ram, rt, a0, a1, a2, a3, sp, f0, f1, f2, f3, f4, f5, f6, f7, f8, f9, f10, f11, f12, f13, f14, f15, f16, f17, f18, f19, f20, f21, f22, f23, f24, f25, f26, f27, f28, f29, f30, f31, vf0, vf1, vf2, vf3, vf4, vf8, vf10, vf11, vf12, vf13, vf14, vf15, vf16, vf17, vi15, vacc, h2, a7c); f0 = a1_flt((uint32_t)a7c[0]); }
L_10b6e30:;
    { A3_CHARGE(); a11x::popMatrix(ram, rt, vf1, vf2, vf3, vf4, vi15); }
    { const bool c = (int64_t)s7 == (int64_t)s0; (a0 = (uint64_t)((uint64_t)s7 + (uint64_t)0ULL)); if (c) goto L_10b6e50; }
    (a1 = (uint64_t)((uint64_t)s5 + (uint64_t)0ULL));
    (a2 = (uint64_t)((uint64_t)s6 + (uint64_t)0ULL));
    { (a3 = (uint64_t)((uint64_t)s4 + (uint64_t)0ULL)); A3_CHARGE(); v0 = manager_sub_010B8958(ram, rt, a0, a1, a2, a3, sp, f0, f1, f2, f3, f4, f5, f6, f7, f8, f9, f10, f11, f12, f13, f14, f15, f16, f17, f18, f19, f20, f21, f22, f23, f24, f25, f26, f27, f28, f29, f30, f31, vf0, vf1, vf2, vf3, vf4, vf8, vf10, vf11, vf12, vf13, vf14, vf15, vf16, vf17, vi15, vacc, h2); }
L_10b6e50:;
    { (sp = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)sp, 144)))); io_vf1 = vf1; io_vf2 = vf2; io_vf3 = vf3; io_vf4 = vf4; io_vf8 = vf8; io_vf10 = vf10; io_vf11 = vf11; io_vf12 = vf12; io_vf13 = vf13; io_vf14 = vf14; io_vf15 = vf15; io_vf16 = vf16; io_vf17 = vf17; io_vi15 = vi15; io_vacc = vacc; io_h2 = h2; return v0; }
    __builtin_unreachable();
}

// gamecore_sub_0137F850 (0x137f850): typed by tools/typed_lift2.py (round A6; guest frame calls, extra outputs xo[0]=f12 xo[1]=f13 xo[2]=f21 xo[3]=f22; VU0 in vf0 vf2 vf9 vf10 vf12 vf13 vf14 vf15 vf16 vf17 vf25 vf28 vf29 vacc vstat h3 h4, out vf1 vf2 vf3 vf4 vf5 vf6 vf7 vf8 vf9 vf10 vf11 vf12 vf13 vf14 vf15 vf16 vf17 vf24 vf25 vf26 vf28 vf29 vacc vq vstat h2 h8 h9 h10 h11 h12 h13 h14 h15, relocated calls 3)
inline a1::RetVF gamecore_sub_0137F850(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint64_t a0, uint64_t a1, uint64_t a2, uint64_t sp, float f12, float f13, float f14, float f15, float f16, float f17, float f18, float f19, float f20, float f21, float f22, const a4::vec4 &io_vf0, a4::vec4 &io_vf1, a4::vec4 &io_vf2, a4::vec4 &io_vf3, a4::vec4 &io_vf4, a4::vec4 &io_vf5, a4::vec4 &io_vf6, a4::vec4 &io_vf7, a4::vec4 &io_vf8, a4::vec4 &io_vf9, a4::vec4 &io_vf10, a4::vec4 &io_vf11, a4::vec4 &io_vf12, a4::vec4 &io_vf13, a4::vec4 &io_vf14, a4::vec4 &io_vf15, a4::vec4 &io_vf16, a4::vec4 &io_vf17, a4::vec4 &io_vf24, a4::vec4 &io_vf25, a4::vec4 &io_vf26, a4::vec4 &io_vf28, a4::vec4 &io_vf29, a4::vec4 &io_vacc, float &io_vq, uint16_t &io_vstat, uint64_t &io_h2, uint64_t io_h3, uint64_t io_h4, uint64_t &io_h8, uint64_t &io_h9, uint64_t &io_h10, uint64_t &io_h11, uint64_t &io_h12, uint64_t &io_h13, uint64_t &io_h14, uint64_t &io_h15, uint64_t *xo = nullptr)
{
    float f0 = 0.0f;
    float f1 = 0.0f;
    float f2 = 0.0f;
    uint64_t at = 0;
    uint64_t v0 = 0;
    uint64_t v1 = 0;
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
    uint64_t fp = 0;
    uint32_t fcr31 = 0;
    a4::vec4 vf0 = io_vf0;
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
    a4::vec4 vf24 = io_vf24;
    a4::vec4 vf25 = io_vf25;
    a4::vec4 vf26 = io_vf26;
    a4::vec4 vf28 = io_vf28;
    a4::vec4 vf29 = io_vf29;
    a4::vec4 vacc = io_vacc;
    float vq = io_vq;
    uint16_t vstat = io_vstat;
    uint64_t h2 = io_h2;
    uint64_t h3 = io_h3;
    uint64_t h4 = io_h4;
    uint64_t h8 = io_h8;
    uint64_t h9 = io_h9;
    uint64_t h10 = io_h10;
    uint64_t h11 = io_h11;
    uint64_t h12 = io_h12;
    uint64_t h13 = io_h13;
    uint64_t h14 = io_h14;
    uint64_t h15 = io_h15;
    (sp = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)sp, 4294966880))));
    (s0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)sp, 256))));
    (s2 = (uint64_t)((uint64_t)a1 + (uint64_t)0ULL));
    (s6 = (uint64_t)((uint64_t)a2 + (uint64_t)0ULL));
    A1_W32B((uint32_t)sp, 288, (uint32_t)a0);
    (a0 = (uint64_t)((uint64_t)RC_U64X(s0) + (uint64_t)0ULL));
    (a1 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL));
    (a2 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 16))));
    { float f = f22; uint32_t bits; std::memcpy(&bits, &f, sizeof(bits)); A1_W32B((uint32_t)sp, 400, bits); }
    f22 = ((float)(f13));
    { float f = f21; uint32_t bits; std::memcpy(&bits, &f, sizeof(bits)); A1_W32B((uint32_t)sp, 392, bits); }
    f21 = ((float)(f12));
    { { float f = f20; uint32_t bits; std::memcpy(&bits, &f, sizeof(bits)); A1_W32B((uint32_t)sp, 384, bits); } A3_CHARGE(); v0 = a6x::memset(ram, rt, a0, (uint32_t)a1, (uint32_t)a2, h2, h4); }
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)sp, 288))));
    (s1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)sp, 96))));
    (at = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)16256 << 16))));
    { uint32_t bits = (uint32_t)at; std::memcpy(&f0, &bits, sizeof(bits)); }
    (fp = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v0, 800))));
    { float f = f0; uint32_t bits; std::memcpy(&bits, &f, sizeof(bits)); A1_W32B((uint32_t)sp, 268, bits); }
    vf5 = a4v::castsi128_ps(A4_R128(ADD32((uint32_t)s6, 0)));
    vf6 = a4v::castsi128_ps(A4_R128(ADD32((uint32_t)fp, 0)));
    { a4::vec4 res = a4v::sub(vf5, vf6); a4::q128 mask = a4v::set_epi32(0, -1, -1, -1); vf5 = a4v::blend(vf5, res, a4v::castsi128_ps(mask)); }
    A4_W128(ADD32((uint32_t)s1, 0), a4v::castps_si128(vf5));
    { float f = f0; uint32_t bits; std::memcpy(&bits, &f, sizeof(bits)); A1_W32B((uint32_t)sp, 108, bits); }
    vf7 = a4v::castsi128_ps(A4_R128(ADD32((uint32_t)s1, 0)));
    { a4::vec4 res = a4v::mul(vf7, vf7); a4::q128 mask = a4v::set_epi32(0, -1, -1, -1); vf7 = a4v::blend(vf7, res, a4v::castsi128_ps(mask)); }
    { a4::vec4 res = a4v::mulK<1, 1>(vf0, a4v::bcast<0>(vf7)); vacc = a4v::dest<0, 0, 0, 1>(vacc, res); }
    { a4::vec4 mul_res = a4v::mulK<1, 1>(vf0, a4v::bcast<1>(vf7)); a4::vec4 res = a4v::addK<1, 1>(vacc, mul_res); vacc = a4v::dest<0, 0, 0, 1>(vacc, res); }
    { a4::vec4 mul_res = a4v::mulK<1, 1>(vf0, a4v::bcast<2>(vf7)); a4::vec4 res = a4v::addK<1, 1>(vacc, mul_res); a4::q128 mask = a4v::set_epi32(-1, 0, 0, 0); vf7 = a4v::blendv_ps(vf7, res, a4v::castsi128_ps(mask)); }
    { float ft = a4v::cvtss_f32(a4v::shuffle_ps<0x03>(vf7, vf7)); vq = a4v::sqrt(vstat, ft); }
    { uint32_t bits; std::memcpy(&bits, &vq, sizeof(bits)); (v0 = (uint64_t)((int64_t)(int32_t)(bits))); }
    { uint32_t bits = (uint32_t)v0; std::memcpy(&f20, &bits, sizeof(bits)); }
    f0 = a1f::div(fcr31, f0, f20);
    { uint32_t bits; std::memcpy(&bits, &f0, sizeof(bits)); (v1 = (uint64_t)((int64_t)(int32_t)(bits))); }
    vf5 = a4v::castsi128_ps(A4_R128(ADD32((uint32_t)s1, 0)));
    vf6 = a4v::castsi128_ps(a4::mk128(h3, v1));
    { a4::vec4 res = a4v::mul(vf5, a4v::bcast<0>(vf6)); a4::q128 mask = a4v::set_epi32(-1, -1, -1, -1); vf5 = a4v::blendv_ps(vf5, res, a4v::castsi128_ps(mask)); }
    A4_W128(ADD32((uint32_t)sp, 0), a4v::castps_si128(vf5));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)sp, 288))));
    (a2 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)sp, 16))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v0, 816))));
    A1_W32B((uint32_t)sp, 292, (uint32_t)v0);
    vf5 = a4v::castsi128_ps(A4_R128(ADD32((uint32_t)v0, 0)));
    { a4::vec4 res = a4v::mul(vf5, vf5); a4::q128 mask = a4v::set_epi32(0, -1, -1, -1); vf7 = a4v::blend(vf7, res, a4v::castsi128_ps(mask)); }
    { a4::vec4 res = a4v::mulK<1, 1>(vf0, a4v::bcast<0>(vf7)); vacc = a4v::dest<0, 0, 0, 1>(vacc, res); }
    { a4::vec4 mul_res = a4v::mulK<1, 1>(vf0, a4v::bcast<1>(vf7)); a4::vec4 res = a4v::addK<1, 1>(vacc, mul_res); vacc = a4v::dest<0, 0, 0, 1>(vacc, res); }
    { a4::vec4 mul_res = a4v::mulK<1, 1>(vf0, a4v::bcast<2>(vf7)); a4::vec4 res = a4v::addK<1, 1>(vacc, mul_res); a4::q128 mask = a4v::set_epi32(-1, 0, 0, 0); vf7 = a4v::blendv_ps(vf7, res, a4v::castsi128_ps(mask)); }
    { float fs = a4v::cvtss_f32(a4v::shuffle_ps<0x03>(vf0, vf0)); float ft = a4v::cvtss_f32(a4v::shuffle_ps<0x03>(vf7, vf7)); vq = a4v::rsqrt(vstat, fs, ft); }
    { a4::vec4 res = a4v::mul(vf5, a4v::set1_ps(vq)); a4::q128 mask = a4v::set_epi32(0, -1, -1, -1); vf5 = a4v::blendv_ps(vf5, res, a4v::castsi128_ps(mask)); }
    A4_W128(ADD32((uint32_t)a2, 0), a4v::castps_si128(vf5));
    (a3 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)sp, 32))));
    vf5 = a4v::castsi128_ps(A4_R128(ADD32((uint32_t)a2, 0)));
    vf6 = a4v::castsi128_ps(A4_R128(ADD32((uint32_t)sp, 0)));
    { a4::vec4 fs_yzx = a4v::shuffle_ps<0xC9>(vf5, vf5); a4::vec4 ft_zxy = a4v::shuffle_ps<0xD2>(vf6, vf6); a4::vec4 res = a4v::mul(fs_yzx, ft_zxy); vacc = a4v::dest<1, 1, 1, 0>(vacc, res); }
    { a4::vec4 fs_yzx = a4v::shuffle_ps<0xC9>(vf6, vf6); a4::vec4 ft_zxy = a4v::shuffle_ps<0xD2>(vf5, vf5); a4::vec4 mul_res = a4v::mul(fs_yzx, ft_zxy); a4::vec4 res = a4v::subK<1, 1>(vacc, mul_res); a4::q128 mask = a4v::set_epi32(0, -1, -1, -1); vf7 = a4v::blendv_ps(vf7, res, a4v::castsi128_ps(mask)); }
    { a4::vec4 res = a4v::sub(vf7, vf7); a4::q128 mask = a4v::set_epi32(-1, 0, 0, 0); vf7 = a4v::blend(vf7, res, a4v::castsi128_ps(mask)); }
    A4_W128(ADD32((uint32_t)a3, 0), a4v::castps_si128(vf7));
    vf5 = a4v::castsi128_ps(A4_R128(ADD32((uint32_t)a3, 0)));
    { a4::vec4 res = a4v::mul(vf5, vf5); a4::q128 mask = a4v::set_epi32(0, -1, -1, -1); vf7 = a4v::blend(vf7, res, a4v::castsi128_ps(mask)); }
    { a4::vec4 res = a4v::mulK<1, 1>(vf0, a4v::bcast<0>(vf7)); vacc = a4v::dest<0, 0, 0, 1>(vacc, res); }
    { a4::vec4 mul_res = a4v::mulK<1, 1>(vf0, a4v::bcast<1>(vf7)); a4::vec4 res = a4v::addK<1, 1>(vacc, mul_res); vacc = a4v::dest<0, 0, 0, 1>(vacc, res); }
    { a4::vec4 mul_res = a4v::mulK<1, 1>(vf0, a4v::bcast<2>(vf7)); a4::vec4 res = a4v::addK<1, 1>(vacc, mul_res); a4::q128 mask = a4v::set_epi32(-1, 0, 0, 0); vf7 = a4v::blendv_ps(vf7, res, a4v::castsi128_ps(mask)); }
    { float fs = a4v::cvtss_f32(a4v::shuffle_ps<0x03>(vf0, vf0)); float ft = a4v::cvtss_f32(a4v::shuffle_ps<0x03>(vf7, vf7)); vq = a4v::rsqrt(vstat, fs, ft); }
    { a4::vec4 res = a4v::mul(vf5, a4v::set1_ps(vq)); a4::q128 mask = a4v::set_epi32(0, -1, -1, -1); vf5 = a4v::blendv_ps(vf5, res, a4v::castsi128_ps(mask)); }
    A4_W128(ADD32((uint32_t)a3, 0), a4v::castps_si128(vf5));
    (s4 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)sp, 48))));
    vf5 = a4v::castsi128_ps(A4_R128(ADD32((uint32_t)a3, 0)));
    vf6 = a4v::castsi128_ps(A4_R128(ADD32((uint32_t)a2, 0)));
    { a4::vec4 fs_yzx = a4v::shuffle_ps<0xC9>(vf5, vf5); a4::vec4 ft_zxy = a4v::shuffle_ps<0xD2>(vf6, vf6); a4::vec4 res = a4v::mul(fs_yzx, ft_zxy); vacc = a4v::dest<1, 1, 1, 0>(vacc, res); }
    { a4::vec4 fs_yzx = a4v::shuffle_ps<0xC9>(vf6, vf6); a4::vec4 ft_zxy = a4v::shuffle_ps<0xD2>(vf5, vf5); a4::vec4 mul_res = a4v::mul(fs_yzx, ft_zxy); a4::vec4 res = a4v::subK<1, 1>(vacc, mul_res); a4::q128 mask = a4v::set_epi32(0, -1, -1, -1); vf7 = a4v::blendv_ps(vf7, res, a4v::castsi128_ps(mask)); }
    { a4::vec4 res = a4v::sub(vf7, vf7); a4::q128 mask = a4v::set_epi32(-1, 0, 0, 0); vf7 = a4v::blend(vf7, res, a4v::castsi128_ps(mask)); }
    A4_W128(ADD32((uint32_t)s4, 0), a4v::castps_si128(vf7));
    (s3 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)sp, 128))));
    (a1 = (uint64_t)((uint64_t)s0 + (uint64_t)0ULL));
    (a0 = (uint64_t)((uint64_t)RC_U64X(s3) + (uint64_t)0ULL));
    A1_W32B((uint32_t)sp, 60, 0u);
    A1_W32B((uint32_t)sp, 44, 0u);
    { A1_W32B((uint32_t)sp, 28, 0u); A3_CHARGE(); v0 = iosSetCameraMatrix(ram, rt, a0, a1, a2, a3, sp, vf0, vf1, vf2, vf3, vf4, vf5, vf6, vf7, vf8, vf9, vf14, vf15, vf16, vf17, vacc, vq, vstat, h2, h8, h9, h10, h11, h12, h13, h14, h15); }
    (s7 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)sp, 192))));
    vf24 = a4v::castsi128_ps(A4_R128(ADD32((uint32_t)s3, 48)));
    { a4::vec4 res = a4v::subK<1, 1>(vf0, a4v::bcast<3>(vf0)); a4::q128 mask = a4v::set_epi32(0, 0, 0, -1); vf25 = a4v::blendv_ps(vf25, res, a4v::castsi128_ps(mask)); }
    { a4::vec4 res = a4v::mulK<0, 1>(vf24, a4v::bcast<0>(vf25)); a4::q128 mask = a4v::set_epi32(0, -1, -1, -1); vf28 = a4v::blendv_ps(vf28, res, a4v::castsi128_ps(mask)); }
    { a4::vec4 res = a4v::sub(vf28, vf28); a4::q128 mask = a4v::set_epi32(-1, 0, 0, 0); vf28 = a4v::blend(vf28, res, a4v::castsi128_ps(mask)); }
    do { const a4::q128 _v = (A4_R128(ADD32((uint32_t)s3, 0))); h8 = a4::hi64(_v); std::memcpy(&t0, &_v, 8); } while (0);
    do { const a4::q128 _v = (A4_R128(ADD32((uint32_t)s3, 16))); h9 = a4::hi64(_v); std::memcpy(&t1, &_v, 8); } while (0);
    do { const a4::q128 _v = (A4_R128(ADD32((uint32_t)s3, 32))); h10 = a4::hi64(_v); std::memcpy(&t2, &_v, 8); } while (0);
    do { const a4::q128 _v = (a4v::castps_si128(vf0)); h11 = a4::hi64(_v); std::memcpy(&t3, &_v, 8); } while (0);
    do { const a4::q128 _v = (a4m::pextlw(a4::mk128(h9, t1), a4::mk128(h8, t0))); h12 = a4::hi64(_v); std::memcpy(&t4, &_v, 8); } while (0);
    do { const a4::q128 _v = (a4m::pextuw(a4::mk128(h9, t1), a4::mk128(h8, t0))); h13 = a4::hi64(_v); std::memcpy(&t5, &_v, 8); } while (0);
    do { const a4::q128 _v = (a4m::pextlw(a4::mk128(h11, t3), a4::mk128(h10, t2))); h14 = a4::hi64(_v); std::memcpy(&t6, &_v, 8); } while (0);
    do { const a4::q128 _v = (a4m::pextuw(a4::mk128(h11, t3), a4::mk128(h10, t2))); h15 = a4::hi64(_v); std::memcpy(&t7, &_v, 8); } while (0);
    do { const a4::q128 _v = (a4m::pcpyld(a4::mk128(h14, t6), a4::mk128(h12, t4))); h8 = a4::hi64(_v); std::memcpy(&t0, &_v, 8); } while (0);
    do { const a4::q128 _v = (a4v::unpackhi_epi64(a4::mk128(h12, t4), a4::mk128(h14, t6))); h9 = a4::hi64(_v); std::memcpy(&t1, &_v, 8); } while (0);
    do { const a4::q128 _v = (a4m::pcpyld(a4::mk128(h15, t7), a4::mk128(h13, t5))); h10 = a4::hi64(_v); std::memcpy(&t2, &_v, 8); } while (0);
    do { const a4::q128 _v = (a4v::unpackhi_epi64(a4::mk128(h13, t5), a4::mk128(h15, t7))); h11 = a4::hi64(_v); std::memcpy(&t3, &_v, 8); } while (0);
    vf24 = a4v::castsi128_ps(a4::mk128(h8, t0));
    vf25 = a4v::castsi128_ps(a4::mk128(h9, t1));
    vf26 = a4v::castsi128_ps(a4::mk128(h10, t2));
    { a4::vec4 res = a4v::mulK<0, 1>(vf24, a4v::bcast<0>(vf28)); vacc = a4v::dest<1, 1, 1, 1>(vacc, res); }
    { a4::vec4 mul_res = a4v::mulK<0, 1>(vf25, a4v::bcast<1>(vf28)); a4::vec4 res = a4v::addK<1, 1>(vacc, mul_res); vacc = a4v::dest<1, 1, 1, 1>(vacc, res); }
    { a4::vec4 mul_res = a4v::mulK<0, 1>(vf26, a4v::bcast<2>(vf28)); a4::vec4 res = a4v::addK<1, 1>(vacc, mul_res); vacc = a4v::dest<1, 1, 1, 1>(vacc, res); }
    { a4::vec4 mul_res = a4v::mulK<1, 1>(vf0, a4v::bcast<3>(vf28)); a4::vec4 res = a4v::addK<1, 1>(vacc, mul_res); a4::q128 mask = a4v::set_epi32(-1, -1, -1, -1); vf29 = a4v::blendv_ps(vf29, res, a4v::castsi128_ps(mask)); }
    { a4::vec4 res = a4v::subK<1, 1>(vf29, vf29); a4::q128 mask = a4v::set_epi32(-1, 0, 0, 0); vf29 = a4v::blend(vf29, res, a4v::castsi128_ps(mask)); }
    { a4::vec4 res = a4v::addK<1, 1>(vf29, vf0); a4::q128 mask = a4v::set_epi32(-1, 0, 0, 0); vf29 = a4v::blend(vf29, res, a4v::castsi128_ps(mask)); }
    A4_W128(ADD32((uint32_t)s7, 0), a4v::castps_si128(vf24));
    A4_W128(ADD32((uint32_t)s7, 16), a4v::castps_si128(vf25));
    A4_W128(ADD32((uint32_t)s7, 32), a4v::castps_si128(vf26));
    A4_W128(ADD32((uint32_t)s7, 48), a4v::castps_si128(vf29));
    f0 = a1f::add(f21, f21);
    fcr31 = (a1f::lt(f20, f0)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
    { const bool c = (fcr31 & 0x800000u) == 0u; (s0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)sp, 64)))); if (c) goto L_137fa60; }
    (at = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)16128 << 16))));
    { uint32_t bits = (uint32_t)at; std::memcpy(&f0, &bits, sizeof(bits)); }
    f21 = a1f::mul(f20, f0);
L_137fa60:;
    { uint32_t bits; std::memcpy(&bits, &f21, sizeof(bits)); (v1 = (uint64_t)((int64_t)(int32_t)(bits))); }
    vf5 = a4v::castsi128_ps(A4_R128(ADD32((uint32_t)s4, 0)));
    vf6 = a4v::castsi128_ps(a4::mk128(h3, v1));
    { a4::vec4 res = a4v::mul(vf5, a4v::bcast<0>(vf6)); a4::q128 mask = a4v::set_epi32(-1, -1, -1, -1); vf5 = a4v::blendv_ps(vf5, res, a4v::castsi128_ps(mask)); }
    A4_W128(ADD32((uint32_t)s0, 0), a4v::castps_si128(vf5));
    vf5 = a4v::castsi128_ps(A4_R128(ADD32((uint32_t)fp, 0)));
    vf6 = a4v::castsi128_ps(A4_R128(ADD32((uint32_t)s0, 0)));
    { a4::vec4 res = a4v::add(vf5, vf6); a4::q128 mask = a4v::set_epi32(0, -1, -1, -1); vf5 = a4v::blend(vf5, res, a4v::castsi128_ps(mask)); }
    A4_W128(ADD32((uint32_t)s0, 0), a4v::castps_si128(vf5));
    vf5 = a4v::castsi128_ps(A4_R128(ADD32((uint32_t)s6, 0)));
    vf6 = a4v::castsi128_ps(A4_R128(ADD32((uint32_t)s0, 0)));
    { a4::vec4 res = a4v::sub(vf5, vf6); a4::q128 mask = a4v::set_epi32(0, -1, -1, -1); vf5 = a4v::blend(vf5, res, a4v::castsi128_ps(mask)); }
    A4_W128(ADD32((uint32_t)s1, 0), a4v::castps_si128(vf5));
    vf8 = a4v::castsi128_ps(A4_R128(ADD32((uint32_t)s1, 0)));
    vf14 = a4v::castsi128_ps(A4_R128(ADD32((uint32_t)s3, 0)));
    vf15 = a4v::castsi128_ps(A4_R128(ADD32((uint32_t)s3, 16)));
    vf16 = a4v::castsi128_ps(A4_R128(ADD32((uint32_t)s3, 32)));
    vf17 = a4v::castsi128_ps(A4_R128(ADD32((uint32_t)s3, 48)));
    { a4::vec4 res = a4v::mul(vf14, a4v::bcast<0>(vf8)); vacc = a4v::dest<1, 1, 1, 1>(vacc, res); }
    { a4::vec4 mul_res = a4v::mul(vf15, a4v::bcast<1>(vf8)); a4::vec4 res = a4v::addK<1, 1>(vacc, mul_res); vacc = a4v::dest<1, 1, 1, 1>(vacc, res); }
    { a4::vec4 mul_res = a4v::mul(vf16, a4v::bcast<2>(vf8)); a4::vec4 res = a4v::addK<1, 1>(vacc, mul_res); vacc = a4v::dest<1, 1, 1, 1>(vacc, res); }
    { a4::vec4 mul_res = a4v::mul(vf17, a4v::bcast<3>(vf8)); a4::vec4 res = a4v::addK<1, 1>(vacc, mul_res); a4::q128 mask = a4v::set_epi32(-1, -1, -1, -1); vf10 = a4v::blendv_ps(vf10, res, a4v::castsi128_ps(mask)); }
    A4_W128(ADD32((uint32_t)s1, 0), a4v::castps_si128(vf10));
    A1_W32B((uint32_t)sp, 100, 0u);
    vf7 = a4v::castsi128_ps(A4_R128(ADD32((uint32_t)s1, 0)));
    { a4::vec4 res = a4v::mul(vf7, vf7); a4::q128 mask = a4v::set_epi32(0, -1, -1, -1); vf7 = a4v::blend(vf7, res, a4v::castsi128_ps(mask)); }
    { a4::vec4 res = a4v::mulK<1, 1>(vf0, a4v::bcast<0>(vf7)); vacc = a4v::dest<0, 0, 0, 1>(vacc, res); }
    { a4::vec4 mul_res = a4v::mulK<1, 1>(vf0, a4v::bcast<1>(vf7)); a4::vec4 res = a4v::addK<1, 1>(vacc, mul_res); vacc = a4v::dest<0, 0, 0, 1>(vacc, res); }
    { a4::vec4 mul_res = a4v::mulK<1, 1>(vf0, a4v::bcast<2>(vf7)); a4::vec4 res = a4v::addK<1, 1>(vacc, mul_res); a4::q128 mask = a4v::set_epi32(-1, 0, 0, 0); vf7 = a4v::blendv_ps(vf7, res, a4v::castsi128_ps(mask)); }
    { float ft = a4v::cvtss_f32(a4v::shuffle_ps<0x03>(vf7, vf7)); vq = a4v::sqrt(vstat, ft); }
    { uint32_t bits; std::memcpy(&bits, &vq, sizeof(bits)); (v0 = (uint64_t)((int64_t)(int32_t)(bits))); }
    { uint32_t bits = (uint32_t)v0; std::memcpy(&f20, &bits, sizeof(bits)); }
    f13 = a1f::mul(f20, f20);
    f0 = a1f::mul(f21, f21);
    f13 = a1f::subFinite(f13, f0);
    f13 = a1f::sqrt(fcr31, f13);
    (s5 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)sp, 80))));
    { const uint32_t _dynInsn = A1_R32(0x137FAF8u); const int32_t _dynSimm = (int16_t)(_dynInsn & 0xFFFFu); const uint32_t _dynImm = _dynInsn & 0xFFFFu; (void)_dynSimm; (void)_dynImm; (a2 = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)_dynImm << 16)))); }
    { const uint32_t _dynInsn = A1_R32(0x137FAFCu); const int32_t _dynSimm = (int16_t)(_dynInsn & 0xFFFFu); const uint32_t _dynImm = _dynInsn & 0xFFFFu; (void)_dynSimm; (void)_dynImm; (a2 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)a2, _dynSimm)))); }
    (a3 = (uint64_t)((uint64_t)s1 + (uint64_t)0ULL));
    (a0 = (uint64_t)((uint64_t)RC_U64X(s5) + (uint64_t)0ULL));
    (a1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)sp, 112))));
    { f12 = ((float)(f21)); A3_CHARGE(); uint64_t a7c[5] = {0u, 0u, 0u, 0u, 0u}; { const a1::RetVF a2_r = AI_CalcIntersectionCircleXZ(ram, rt, a0, a1, a2, (uint32_t)a3, (uint32_t)sp, f12, f13, f14, f15, f16, f17, f18, f19, f20, f21, f22, vf0, vf5, vf6, vf7, vf8, vf10, vf11, vf12, vf13, vf14, vf15, vf16, vf17, vacc, vq, vstat, h2, a7c); v0 = a2_r.v0; f0 = a2_r.f0; } f21 = a1_flt((uint32_t)a7c[0]); f22 = a1_flt((uint32_t)a7c[1]); }
    { const bool c = (int32_t)v0 <= 0; (s4 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)s2, 96)))); if (c) goto L_137fbb8; }
    vf8 = a4v::castsi128_ps(A4_R128(ADD32((uint32_t)s5, 0)));
    vf14 = a4v::castsi128_ps(A4_R128(ADD32((uint32_t)s7, 0)));
    vf15 = a4v::castsi128_ps(A4_R128(ADD32((uint32_t)s7, 16)));
    vf16 = a4v::castsi128_ps(A4_R128(ADD32((uint32_t)s7, 32)));
    vf17 = a4v::castsi128_ps(A4_R128(ADD32((uint32_t)s7, 48)));
    { a4::vec4 res = a4v::mul(vf14, a4v::bcast<0>(vf8)); vacc = a4v::dest<1, 1, 1, 1>(vacc, res); }
    { a4::vec4 mul_res = a4v::mul(vf15, a4v::bcast<1>(vf8)); a4::vec4 res = a4v::addK<1, 1>(vacc, mul_res); vacc = a4v::dest<1, 1, 1, 1>(vacc, res); }
    { a4::vec4 mul_res = a4v::mul(vf16, a4v::bcast<2>(vf8)); a4::vec4 res = a4v::addK<1, 1>(vacc, mul_res); vacc = a4v::dest<1, 1, 1, 1>(vacc, res); }
    { a4::vec4 mul_res = a4v::mul(vf17, a4v::bcast<3>(vf8)); a4::vec4 res = a4v::addK<1, 1>(vacc, mul_res); a4::q128 mask = a4v::set_epi32(-1, -1, -1, -1); vf10 = a4v::blendv_ps(vf10, res, a4v::castsi128_ps(mask)); }
    A4_W128(ADD32((uint32_t)s4, 0), a4v::castps_si128(vf10));
    vf5 = a4v::castsi128_ps(A4_R128(ADD32((uint32_t)s4, 0)));
    vf6 = a4v::castsi128_ps(A4_R128(ADD32((uint32_t)s0, 0)));
    { a4::vec4 res = a4v::add(vf5, vf6); a4::q128 mask = a4v::set_epi32(0, -1, -1, -1); vf5 = a4v::blend(vf5, res, a4v::castsi128_ps(mask)); }
    A4_W128(ADD32((uint32_t)s4, 0), a4v::castps_si128(vf5));
    (s3 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)s2, 64))));
    vf5 = a4v::castsi128_ps(A4_R128(ADD32((uint32_t)s6, 0)));
    vf6 = a4v::castsi128_ps(A4_R128(ADD32((uint32_t)s4, 0)));
    { a4::vec4 res = a4v::sub(vf5, vf6); a4::q128 mask = a4v::set_epi32(0, -1, -1, -1); vf5 = a4v::blend(vf5, res, a4v::castsi128_ps(mask)); }
    A4_W128(ADD32((uint32_t)s3, 0), a4v::castps_si128(vf5));
    vf7 = a4v::castsi128_ps(A4_R128(ADD32((uint32_t)s3, 0)));
    { a4::vec4 res = a4v::mul(vf7, vf7); a4::q128 mask = a4v::set_epi32(0, -1, -1, -1); vf7 = a4v::blend(vf7, res, a4v::castsi128_ps(mask)); }
    { a4::vec4 res = a4v::mulK<1, 1>(vf0, a4v::bcast<0>(vf7)); vacc = a4v::dest<0, 0, 0, 1>(vacc, res); }
    { a4::vec4 mul_res = a4v::mulK<1, 1>(vf0, a4v::bcast<1>(vf7)); a4::vec4 res = a4v::addK<1, 1>(vacc, mul_res); vacc = a4v::dest<0, 0, 0, 1>(vacc, res); }
    { a4::vec4 mul_res = a4v::mulK<1, 1>(vf0, a4v::bcast<2>(vf7)); a4::vec4 res = a4v::addK<1, 1>(vacc, mul_res); a4::q128 mask = a4v::set_epi32(-1, 0, 0, 0); vf7 = a4v::blendv_ps(vf7, res, a4v::castsi128_ps(mask)); }
    { float ft = a4v::cvtss_f32(a4v::shuffle_ps<0x03>(vf7, vf7)); vq = a4v::sqrt(vstat, ft); }
    { uint32_t bits; std::memcpy(&bits, &vq, sizeof(bits)); (v0 = (uint64_t)((int64_t)(int32_t)(bits))); }
    { uint32_t bits = (uint32_t)v0; std::memcpy(&f0, &bits, sizeof(bits)); }
    f0 = a1f::div(fcr31, f22, f0);
    { uint32_t bits; std::memcpy(&bits, &f0, sizeof(bits)); (v0 = (uint64_t)((int64_t)(int32_t)(bits))); }
    vf5 = a4v::castsi128_ps(A4_R128(ADD32((uint32_t)s3, 0)));
    vf6 = a4v::castsi128_ps(a4::mk128(h2, v0));
    { a4::vec4 res = a4v::mul(vf5, a4v::bcast<0>(vf6)); a4::q128 mask = a4v::set_epi32(-1, -1, -1, -1); vf5 = a4v::blendv_ps(vf5, res, a4v::castsi128_ps(mask)); }
    A4_W128(ADD32((uint32_t)s3, 0), a4v::castps_si128(vf5));
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)sp, 272))));
    goto L_137fbc0;
L_137fbb8:;
    (s3 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)s2, 64))));
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)sp, 272))));
L_137fbc0:;
    vf5 = a4v::castsi128_ps(A4_R128(ADD32((uint32_t)s5, 0)));
    { a4::vec4 res = a4v::mul(vf5, vf5); a4::q128 mask = a4v::set_epi32(0, -1, -1, -1); vf7 = a4v::blend(vf7, res, a4v::castsi128_ps(mask)); }
    { a4::vec4 res = a4v::mulK<1, 1>(vf0, a4v::bcast<0>(vf7)); vacc = a4v::dest<0, 0, 0, 1>(vacc, res); }
    { a4::vec4 mul_res = a4v::mulK<1, 1>(vf0, a4v::bcast<1>(vf7)); a4::vec4 res = a4v::addK<1, 1>(vacc, mul_res); vacc = a4v::dest<0, 0, 0, 1>(vacc, res); }
    { a4::vec4 mul_res = a4v::mulK<1, 1>(vf0, a4v::bcast<2>(vf7)); a4::vec4 res = a4v::addK<1, 1>(vacc, mul_res); a4::q128 mask = a4v::set_epi32(-1, 0, 0, 0); vf7 = a4v::blendv_ps(vf7, res, a4v::castsi128_ps(mask)); }
    { float fs = a4v::cvtss_f32(a4v::shuffle_ps<0x03>(vf0, vf0)); float ft = a4v::cvtss_f32(a4v::shuffle_ps<0x03>(vf7, vf7)); vq = a4v::rsqrt(vstat, fs, ft); }
    { a4::vec4 res = a4v::mul(vf5, a4v::set1_ps(vq)); a4::q128 mask = a4v::set_epi32(0, -1, -1, -1); vf5 = a4v::blendv_ps(vf5, res, a4v::castsi128_ps(mask)); }
    A4_W128(ADD32((uint32_t)a0, 0), a4v::castps_si128(vf5));
    { const uint32_t _dynInsn = A1_R32(0x137FBE4u); const int32_t _dynSimm = (int16_t)(_dynInsn & 0xFFFFu); const uint32_t _dynImm = _dynInsn & 0xFFFFu; (void)_dynSimm; (void)_dynImm; (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)_dynImm << 16)))); }
    { const uint32_t _dynInsn = A1_R32(0x137FBE8u); const int32_t _dynSimm = (int16_t)(_dynInsn & 0xFFFFu); const uint32_t _dynImm = _dynInsn & 0xFFFFu; (void)_dynSimm; (void)_dynImm; (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v1, _dynSimm)))); }
    vf5 = a4v::castsi128_ps(A4_R128(ADD32((uint32_t)v1, 0)));
    vf6 = a4v::castsi128_ps(A4_R128(ADD32((uint32_t)a0, 0)));
    { a4::vec4 res = a4v::addK<1, 1>(vf0, a4v::bcast<3>(vf0)); a4::q128 mask = a4v::set_epi32(0, 0, 0, -1); vf7 = a4v::blendv_ps(vf7, res, a4v::castsi128_ps(mask)); }
    { a4::vec4 res = a4v::mul(vf5, vf6); a4::q128 mask = a4v::set_epi32(0, -1, -1, -1); vf6 = a4v::blend(vf6, res, a4v::castsi128_ps(mask)); }
    { a4::vec4 res = a4v::addK<1, 1>(vf0, a4v::bcast<0>(vf6)); vacc = a4v::dest<1, 0, 0, 0>(vacc, res); }
    { a4::vec4 mul_res = a4v::mulK<1, 1>(vf7, a4v::bcast<1>(vf6)); a4::vec4 res = a4v::addK<1, 1>(vacc, mul_res); vacc = a4v::dest<1, 0, 0, 0>(vacc, res); }
    { a4::vec4 mul_res = a4v::mulK<1, 1>(vf7, a4v::bcast<2>(vf6)); a4::vec4 res = a4v::addK<1, 1>(vacc, mul_res); a4::q128 mask = a4v::set_epi32(0, 0, 0, -1); vf6 = a4v::blendv_ps(vf6, res, a4v::castsi128_ps(mask)); }
    do { const a4::q128 _v = (a4v::castps_si128(vf6)); h2 = a4::hi64(_v); std::memcpy(&v0, &_v, 8); } while (0);
    { uint32_t bits = (uint32_t)v0; std::memcpy(&f12, &bits, sizeof(bits)); }
    { A3_CHARGE(); f0 = a9x::acosExact(ram, rt, f12); }
    { uint32_t bits = 0u; std::memcpy(&f1, &bits, sizeof(bits)); }
    f2 = ((float)(f0));
    { uint32_t bits = A1_R32B((uint32_t)sp, 280); float f; std::memcpy(&f, &bits, sizeof(f)); f0 = f; }
    fcr31 = (a1f::lt(f0, f1)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
    { const bool c = (fcr31 & 0x800000u) == 0u; (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)sp, 288)))); if (c) goto L_137fc40; }
    (at = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)16585 << 16))));
    (at = (uint64_t)(RC_U64X(at) | (uint64_t)(uint16_t)4058));
    { uint32_t bits = (uint32_t)at; std::memcpy(&f0, &bits, sizeof(bits)); }
    f2 = a1f::sub(f0, f2);
L_137fc40:;
    (s6 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)s2, 48))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)sp, 292))));
    { uint32_t bits = A1_R32B((uint32_t)v1, 852); float f; std::memcpy(&f, &bits, sizeof(f)); f0 = f; }
    { uint32_t bits = A1_R32B((uint32_t)v1, 848); float f; std::memcpy(&f, &bits, sizeof(f)); f1 = f; }
    f0 = a1f::div(fcr31, f0, f1);
    { uint32_t bits; std::memcpy(&bits, &f0, sizeof(bits)); (v1 = (uint64_t)((int64_t)(int32_t)(bits))); }
    vf5 = a4v::castsi128_ps(A4_R128(ADD32((uint32_t)v0, 0)));
    vf6 = a4v::castsi128_ps(a4::mk128(h3, v1));
    { a4::vec4 res = a4v::mul(vf5, a4v::bcast<0>(vf6)); a4::q128 mask = a4v::set_epi32(-1, -1, -1, -1); vf5 = a4v::blendv_ps(vf5, res, a4v::castsi128_ps(mask)); }
    A4_W128(ADD32((uint32_t)s6, 0), a4v::castps_si128(vf5));
    do { const a4::q128 _v = (A4_R128(ADD32((uint32_t)fp, 0))); h2 = a4::hi64(_v); std::memcpy(&v0, &_v, 8); } while (0);
    (s5 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)s2, 112))));
    A4_W128(ADD32((uint32_t)s2, 80), a4::mk128(h2, v0));
    A4_W128(ADD32((uint32_t)s5, 0), a4v::castps_si128(vf0));
    f2 = a1f::mul(f21, f2);
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)326 << 16))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v0, 22096))));
    f2 = a1f::div(fcr31, f2, f22);
    { float f = f2; uint32_t bits; std::memcpy(&bits, &f, sizeof(bits)); A1_W32B((uint32_t)s2, 160, bits); }
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)sp, 288))));
    { uint32_t bits = A1_R32B((uint32_t)v0, 400); float f; std::memcpy(&f, &bits, sizeof(f)); f0 = f; }
    { uint32_t bits = A1_R32B((uint32_t)v1, 848); float f; std::memcpy(&f, &bits, sizeof(f)); f1 = f; }
    f0 = a1f::mul(f0, f1);
    { float f = f0; uint32_t bits; std::memcpy(&bits, &f, sizeof(bits)); A1_W32B((uint32_t)s2, 164, bits); }
    { uint32_t bits = A1_R32B((uint32_t)v0, 400); float f; std::memcpy(&f, &bits, sizeof(f)); f1 = f; }
    { uint32_t bits = A1_R32B((uint32_t)v1, 848); float f; std::memcpy(&f, &bits, sizeof(f)); f0 = f; }
    f1 = a1f::mul(f1, f0);
    fcr31 = (a1f::lt(f2, f1)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
    if ((fcr31 & 0x800000u) != 0u) { { float f = f1; uint32_t bits; std::memcpy(&bits, &f, sizeof(bits)); A1_W32B((uint32_t)s2, 160, bits); } goto L_137fcc8; }
L_137fcc8:;
    (s1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)s2, 32))));
    (s0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)s2, 80))));
    { uint32_t bits = A1_R32B((uint32_t)s2, 160); float f; std::memcpy(&f, &bits, sizeof(f)); f12 = f; }
    (t1 = (uint64_t)((uint64_t)RC_U64X(s5) + (uint64_t)0ULL));
    (a0 = (uint64_t)((uint64_t)RC_U64X(s1) + (uint64_t)0ULL));
    (a1 = (uint64_t)((uint64_t)RC_U64X(s6) + (uint64_t)0ULL));
    (a2 = (uint64_t)((uint64_t)s3 + (uint64_t)0ULL));
    (a3 = (uint64_t)((uint64_t)RC_U64X(s0) + (uint64_t)0ULL));
    { (t0 = (uint64_t)((uint64_t)s4 + (uint64_t)0ULL)); A3_CHARGE(); { const a1::RetVF a2_r = AI_GetAccelC3XYZ(ram, rt, (uint32_t)a0, (uint32_t)a1, (uint32_t)a2, (uint32_t)a3, (uint32_t)t0, (uint32_t)t1, f12, vf5, vf6, vf7, h2); v0 = a2_r.v0; f0 = a2_r.f0; } }
    (a0 = (uint64_t)((uint64_t)s2 + (uint64_t)0ULL));
    { uint32_t bits = A1_R32B((uint32_t)s2, 160); float f; std::memcpy(&f, &bits, sizeof(f)); f12 = f; }
    (a1 = (uint64_t)((uint64_t)s6 + (uint64_t)0ULL));
    (a2 = (uint64_t)((uint64_t)s0 + (uint64_t)0ULL));
    (a3 = (uint64_t)((uint64_t)s4 + (uint64_t)0ULL));
    (t0 = (uint64_t)((uint64_t)s5 + (uint64_t)0ULL));
    { (t1 = (uint64_t)((uint64_t)s1 + (uint64_t)0ULL)); A3_CHARGE(); { const a1::RetVF a2_r = AI_GetAccelA3XYZ(ram, rt, (uint32_t)a0, (uint32_t)a1, (uint32_t)a2, (uint32_t)a3, (uint32_t)t0, (uint32_t)t1, f12, vf5, vf6, vf7, h2); v0 = a2_r.v0; f0 = a2_r.f0; } }
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)s2, 16))));
    { uint32_t bits = A1_R32B((uint32_t)s2, 160); float f; std::memcpy(&f, &bits, sizeof(f)); f12 = f; }
    (a1 = (uint64_t)((uint64_t)s6 + (uint64_t)0ULL));
    (a2 = (uint64_t)((uint64_t)s3 + (uint64_t)0ULL));
    (a3 = (uint64_t)((uint64_t)s0 + (uint64_t)0ULL));
    (t0 = (uint64_t)((uint64_t)s4 + (uint64_t)0ULL));
    { (t1 = (uint64_t)((uint64_t)s1 + (uint64_t)0ULL)); A3_CHARGE(); uint64_t a7c[1] = {(uint64_t)a1_bits(f12)}; { const a1::RetVF a2_r = AI_GetAccelB3XYZ_x11(ram, rt, (uint32_t)a0, (uint32_t)a1, (uint32_t)a2, (uint32_t)a3, (uint32_t)t0, (uint32_t)t1, f12, vf5, vf6, vf7, h2, a7c); v0 = a2_r.v0; f0 = a2_r.f0; } f12 = a1_flt((uint32_t)a7c[0]); }
    { uint32_t bits = A1_R32B((uint32_t)s2, 160); float f; std::memcpy(&f, &bits, sizeof(f)); f0 = f; }
    { uint32_t bits = A1_R32B((uint32_t)sp, 400); float f; std::memcpy(&f, &bits, sizeof(f)); f22 = f; }
    { uint32_t bits = A1_R32B((uint32_t)sp, 392); float f; std::memcpy(&f, &bits, sizeof(f)); f21 = f; }
    { uint32_t bits = A1_R32B((uint32_t)sp, 384); float f; std::memcpy(&f, &bits, sizeof(f)); f20 = f; }
    { (sp = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)sp, 416)))); if (xo) { xo[0] = a1_bits(f12); xo[1] = a1_bits(f13); xo[2] = a1_bits(f21); xo[3] = a1_bits(f22); } io_vf1 = vf1; io_vf2 = vf2; io_vf3 = vf3; io_vf4 = vf4; io_vf5 = vf5; io_vf6 = vf6; io_vf7 = vf7; io_vf8 = vf8; io_vf9 = vf9; io_vf10 = vf10; io_vf11 = vf11; io_vf12 = vf12; io_vf13 = vf13; io_vf14 = vf14; io_vf15 = vf15; io_vf16 = vf16; io_vf17 = vf17; io_vf24 = vf24; io_vf25 = vf25; io_vf26 = vf26; io_vf28 = vf28; io_vf29 = vf29; io_vacc = vacc; io_vq = vq; io_vstat = vstat; io_h2 = h2; io_h8 = h8; io_h9 = h9; io_h10 = h10; io_h11 = h11; io_h12 = h12; io_h13 = h13; io_h14 = h14; io_h15 = h15; return a1::RetVF{(uint64_t)v0, f0}; }
    __builtin_unreachable();
}

// chkAttainExecOffsetData (0x142d6b0): typed by tools/typed_lift2.py (round A6; guest frame calls, extra outputs xo[0]=f12 xo[1]=f13 xo[2]=f20 xo[3]=f21 xo[4]=f22 xo[5]=f23 xo[6]=a1 xo[7]=a2 xo[8]=a3 xo[9]=t0 xo[10]=t1 xo[11]=t2 xo[12]=t3; VU0 in vf0 vf7 vf10 vf12 vf13 vf14 vf15 vf16 vf17 vf28 vf29 vacc vstat h2, out vf4 vf5 vf6 vf7 vf8 vf10 vf11 vf12 vf13 vf14 vf15 vf16 vf17 vf24 vf25 vf26 vf27 vf28 vf29 vacc vq vstat h3 h8 h9 h10 h11 h12 h13 h14 h15, relocated calls 3)
inline a1::RetVF chkAttainExecOffsetData(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint64_t a0, uint64_t a1, uint64_t a2, uint64_t a3, uint64_t t0, uint64_t sp, float f12, float f13, float f14, float f15, float f16, float f17, float f18, float f19, float f20, float f21, float f22, float f23, const a4::vec4 &io_vf0, a4::vec4 &io_vf4, a4::vec4 &io_vf5, a4::vec4 &io_vf6, a4::vec4 &io_vf7, a4::vec4 &io_vf8, a4::vec4 &io_vf10, a4::vec4 &io_vf11, a4::vec4 &io_vf12, a4::vec4 &io_vf13, a4::vec4 &io_vf14, a4::vec4 &io_vf15, a4::vec4 &io_vf16, a4::vec4 &io_vf17, a4::vec4 &io_vf24, a4::vec4 &io_vf25, a4::vec4 &io_vf26, a4::vec4 &io_vf27, a4::vec4 &io_vf28, a4::vec4 &io_vf29, a4::vec4 &io_vacc, float &io_vq, uint16_t &io_vstat, uint64_t io_h2, uint64_t &io_h3, uint64_t &io_h8, uint64_t &io_h9, uint64_t &io_h10, uint64_t &io_h11, uint64_t &io_h12, uint64_t &io_h13, uint64_t &io_h14, uint64_t &io_h15, uint64_t *xo = nullptr)
{
    float f0 = 0.0f;
    float f1 = 0.0f;
    float f2 = 0.0f;
    float f3 = 0.0f;
    uint64_t at = 0;
    uint64_t v0 = 0;
    uint64_t v1 = 0;
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
    a4::vec4 vf24 = io_vf24;
    a4::vec4 vf25 = io_vf25;
    a4::vec4 vf26 = io_vf26;
    a4::vec4 vf27 = io_vf27;
    a4::vec4 vf28 = io_vf28;
    a4::vec4 vf29 = io_vf29;
    a4::vec4 vacc = io_vacc;
    float vq = io_vq;
    uint16_t vstat = io_vstat;
    uint64_t h2 = io_h2;
    uint64_t h3 = io_h3;
    uint64_t h8 = io_h8;
    uint64_t h9 = io_h9;
    uint64_t h10 = io_h10;
    uint64_t h11 = io_h11;
    uint64_t h12 = io_h12;
    uint64_t h13 = io_h13;
    uint64_t h14 = io_h14;
    uint64_t h15 = io_h15;
    bool a3xw6 = false;
    bool a3xw7 = false;
    bool a3xw8 = false;
    (sp = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)sp, 4294967008))));
    (s0 = (uint64_t)((uint64_t)a3 + (uint64_t)0ULL));
    (s1 = (uint64_t)((uint64_t)t0 + (uint64_t)0ULL));
    (s2 = (uint64_t)((uint64_t)a2 + (uint64_t)0ULL));
    (s3 = (uint64_t)((uint64_t)a0 + (uint64_t)0ULL));
    (s4 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)s3, 992))));
    (s5 = (uint64_t)((uint64_t)a1 + (uint64_t)0ULL));
    (s7 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)sp, 64))));
    { float f = f23; uint32_t bits; std::memcpy(&bits, &f, sizeof(bits)); A1_W32B((uint32_t)sp, 272, bits); }
    f23 = ((float)(f12));
    { float f = f22; uint32_t bits; std::memcpy(&bits, &f, sizeof(bits)); A1_W32B((uint32_t)sp, 264, bits); }
    (a0 = (uint64_t)((uint64_t)RC_U64X(s7) + (uint64_t)0ULL));
    { float f = f21; uint32_t bits; std::memcpy(&bits, &f, sizeof(bits)); A1_W32B((uint32_t)sp, 256, bits); }
    { float f = f20; uint32_t bits; std::memcpy(&bits, &f, sizeof(bits)); A1_W32B((uint32_t)sp, 248, bits); }
    { uint32_t bits = A1_R32B((uint32_t)s4, 16); float f; std::memcpy(&f, &bits, sizeof(f)); f1 = f; }
    { uint32_t bits = A1_R32B((uint32_t)s4, 20); float f; std::memcpy(&f, &bits, sizeof(f)); f0 = f; }
    f0 = a1f::sub(f0, f1);
    { f22 = a1f::add(f1, f0); A3_CHARGE(); iosUnitMatrix(ram, rt, (uint32_t)a0, vf0, vf14, vf15, vf16, vf17); }
    do { const a4::q128 _v = (A4_R128(ADD32((uint32_t)s4, 0))); h3 = a4::hi64(_v); std::memcpy(&v1, &_v, 8); } while (0);
    (s6 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)sp, 144))));
    (at = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)16256 << 16))));
    { uint32_t bits = (uint32_t)at; std::memcpy(&f0, &bits, sizeof(bits)); }
    A4_W128(ADD32((uint32_t)sp, 112), a4::mk128(h3, v1));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)s2, 308))));
    { float f = f0; uint32_t bits; std::memcpy(&bits, &f, sizeof(bits)); A1_W32B((uint32_t)sp, 124, bits); }
    vf5 = a4v::castsi128_ps(A4_R128(ADD32((uint32_t)v0, 0)));
    vf6 = a4v::castsi128_ps(A4_R128(ADD32((uint32_t)s0, 0)));
    { a4::vec4 res = a4v::add(vf5, vf6); a4::q128 mask = a4v::set_epi32(-1, -1, -1, -1); vf7 = a4v::blend(vf7, res, a4v::castsi128_ps(mask)); }
    A4_W128(ADD32((uint32_t)s6, 0), a4v::castps_si128(vf7));
    do { const a4::q128 _v = (A4_R128(ADD32((uint32_t)s1, 1136))); h3 = a4::hi64(_v); std::memcpy(&v1, &_v, 8); } while (0);
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)s3, 232))));
    { float f = f0; uint32_t bits; std::memcpy(&bits, &f, sizeof(bits)); A1_W32B((uint32_t)sp, 156, bits); }
    { const bool c = (int64_t)v0 == 0; A4_W128(ADD32((uint32_t)sp, 160), a4::mk128(h3, v1)); if (c) goto L_142d778; }
    (s0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)sp, 160))));
    { (a0 = (uint64_t)((uint64_t)RC_U64X(s0) + (uint64_t)0ULL)); A3_CHARGE(); uint64_t a7c[4] = {0u, 0u, 0u, 0u}; StabilizeToYRotQuaternion(ram, rt, a0, (uint32_t)sp, f12, f13, f14, f15, f16, f17, f18, f19, vf0, vf4, vf5, vf6, vf7, vf11, vf12, vf13, vf14, vf15, vf16, vacc, vq, vstat, a7c); a1 = a7c[1]; a2 = a7c[2]; a3 = a7c[3]; a3xw6 = true; a3xw7 = true; a3xw8 = true; }
    (v1 = (uint64_t)((uint64_t)s6 + (uint64_t)0ULL));
    goto L_142d780;
L_142d778:;
    (s0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)sp, 160))));
    (v1 = (uint64_t)((uint64_t)s6 + (uint64_t)0ULL));
L_142d780:;
    (v0 = (uint64_t)((uint64_t)sp + (uint64_t)0ULL));
    { a4::vec4 res = a4v::addK<1, 1>(vf0, a4v::bcast<3>(vf0)); a4::q128 mask = a4v::set_epi32(0, -1, -1, -1); vf12 = a4v::blendv_ps(vf12, res, a4v::castsi128_ps(mask)); }
    vf11 = a4v::castsi128_ps(A4_R128(ADD32((uint32_t)s0, 0)));
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
    A4_W128(ADD32((uint32_t)v0, 0), a4v::castps_si128(vf14));
    A4_W128(ADD32((uint32_t)v0, 16), a4v::castps_si128(vf15));
    A4_W128(ADD32((uint32_t)v0, 32), a4v::castps_si128(vf16));
    vf4 = a4v::castsi128_ps(A4_R128(ADD32((uint32_t)v1, 0)));
    { a4::q128 mask = a4v::set_epi32(-1, 0, 0, 0); vf4 = a4v::blendv_ps(vf4, vf0, a4v::castsi128_ps(mask)); }
    A4_W128(ADD32((uint32_t)v0, 48), a4v::castps_si128(vf4));
    vf14 = a4v::castsi128_ps(A4_R128(ADD32((uint32_t)sp, 0)));
    vf15 = a4v::castsi128_ps(A4_R128(ADD32((uint32_t)sp, 16)));
    vf16 = a4v::castsi128_ps(A4_R128(ADD32((uint32_t)sp, 32)));
    vf17 = a4v::castsi128_ps(A4_R128(ADD32((uint32_t)sp, 48)));
    vf24 = a4v::castsi128_ps(A4_R128(ADD32((uint32_t)s7, 0)));
    vf25 = a4v::castsi128_ps(A4_R128(ADD32((uint32_t)s7, 16)));
    vf26 = a4v::castsi128_ps(A4_R128(ADD32((uint32_t)s7, 32)));
    vf27 = a4v::castsi128_ps(A4_R128(ADD32((uint32_t)s7, 48)));
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
    A4_W128(ADD32((uint32_t)sp, 0), a4v::castps_si128(vf24));
    A4_W128(ADD32((uint32_t)sp, 16), a4v::castps_si128(vf25));
    A4_W128(ADD32((uint32_t)sp, 32), a4v::castps_si128(vf26));
    A4_W128(ADD32((uint32_t)sp, 48), a4v::castps_si128(vf27));
    vf24 = a4v::castsi128_ps(A4_R128(ADD32((uint32_t)sp, 48)));
    { a4::vec4 res = a4v::subK<1, 1>(vf0, a4v::bcast<3>(vf0)); a4::q128 mask = a4v::set_epi32(0, 0, 0, -1); vf25 = a4v::blendv_ps(vf25, res, a4v::castsi128_ps(mask)); }
    { a4::vec4 res = a4v::mulK<0, 1>(vf24, a4v::bcast<0>(vf25)); a4::q128 mask = a4v::set_epi32(0, -1, -1, -1); vf28 = a4v::blendv_ps(vf28, res, a4v::castsi128_ps(mask)); }
    { a4::vec4 res = a4v::sub(vf28, vf28); a4::q128 mask = a4v::set_epi32(-1, 0, 0, 0); vf28 = a4v::blend(vf28, res, a4v::castsi128_ps(mask)); }
    do { const a4::q128 _v = (A4_R128(ADD32((uint32_t)sp, 0))); h8 = a4::hi64(_v); std::memcpy(&t0, &_v, 8); } while (0);
    do { const a4::q128 _v = (A4_R128(ADD32((uint32_t)sp, 16))); h9 = a4::hi64(_v); std::memcpy(&t1, &_v, 8); } while (0);
    do { const a4::q128 _v = (A4_R128(ADD32((uint32_t)sp, 32))); h10 = a4::hi64(_v); std::memcpy(&t2, &_v, 8); } while (0);
    do { const a4::q128 _v = (a4v::castps_si128(vf0)); h11 = a4::hi64(_v); std::memcpy(&t3, &_v, 8); } while (0);
    do { const a4::q128 _v = (a4m::pextlw(a4::mk128(h9, t1), a4::mk128(h8, t0))); h12 = a4::hi64(_v); std::memcpy(&t4, &_v, 8); } while (0);
    do { const a4::q128 _v = (a4m::pextuw(a4::mk128(h9, t1), a4::mk128(h8, t0))); h13 = a4::hi64(_v); std::memcpy(&t5, &_v, 8); } while (0);
    do { const a4::q128 _v = (a4m::pextlw(a4::mk128(h11, t3), a4::mk128(h10, t2))); h14 = a4::hi64(_v); std::memcpy(&t6, &_v, 8); } while (0);
    do { const a4::q128 _v = (a4m::pextuw(a4::mk128(h11, t3), a4::mk128(h10, t2))); h15 = a4::hi64(_v); std::memcpy(&t7, &_v, 8); } while (0);
    do { const a4::q128 _v = (a4m::pcpyld(a4::mk128(h14, t6), a4::mk128(h12, t4))); h8 = a4::hi64(_v); std::memcpy(&t0, &_v, 8); } while (0);
    do { const a4::q128 _v = (a4v::unpackhi_epi64(a4::mk128(h12, t4), a4::mk128(h14, t6))); h9 = a4::hi64(_v); std::memcpy(&t1, &_v, 8); } while (0);
    do { const a4::q128 _v = (a4m::pcpyld(a4::mk128(h15, t7), a4::mk128(h13, t5))); h10 = a4::hi64(_v); std::memcpy(&t2, &_v, 8); } while (0);
    do { const a4::q128 _v = (a4v::unpackhi_epi64(a4::mk128(h13, t5), a4::mk128(h15, t7))); h11 = a4::hi64(_v); std::memcpy(&t3, &_v, 8); } while (0);
    vf24 = a4v::castsi128_ps(a4::mk128(h8, t0));
    vf25 = a4v::castsi128_ps(a4::mk128(h9, t1));
    vf26 = a4v::castsi128_ps(a4::mk128(h10, t2));
    { a4::vec4 res = a4v::mulK<0, 1>(vf24, a4v::bcast<0>(vf28)); vacc = a4v::dest<1, 1, 1, 1>(vacc, res); }
    { a4::vec4 mul_res = a4v::mulK<0, 1>(vf25, a4v::bcast<1>(vf28)); a4::vec4 res = a4v::addK<1, 1>(vacc, mul_res); vacc = a4v::dest<1, 1, 1, 1>(vacc, res); }
    { a4::vec4 mul_res = a4v::mulK<0, 1>(vf26, a4v::bcast<2>(vf28)); a4::vec4 res = a4v::addK<1, 1>(vacc, mul_res); vacc = a4v::dest<1, 1, 1, 1>(vacc, res); }
    { a4::vec4 mul_res = a4v::mulK<1, 1>(vf0, a4v::bcast<3>(vf28)); a4::vec4 res = a4v::addK<1, 1>(vacc, mul_res); a4::q128 mask = a4v::set_epi32(-1, -1, -1, -1); vf29 = a4v::blendv_ps(vf29, res, a4v::castsi128_ps(mask)); }
    { a4::vec4 res = a4v::subK<1, 1>(vf29, vf29); a4::q128 mask = a4v::set_epi32(-1, 0, 0, 0); vf29 = a4v::blend(vf29, res, a4v::castsi128_ps(mask)); }
    { a4::vec4 res = a4v::addK<1, 1>(vf29, vf0); a4::q128 mask = a4v::set_epi32(-1, 0, 0, 0); vf29 = a4v::blend(vf29, res, a4v::castsi128_ps(mask)); }
    A4_W128(ADD32((uint32_t)sp, 0), a4v::castps_si128(vf24));
    A4_W128(ADD32((uint32_t)sp, 16), a4v::castps_si128(vf25));
    A4_W128(ADD32((uint32_t)sp, 32), a4v::castps_si128(vf26));
    A4_W128(ADD32((uint32_t)sp, 48), a4v::castps_si128(vf29));
    (at = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)16256 << 16))));
    { uint32_t bits = (uint32_t)at; std::memcpy(&f0, &bits, sizeof(bits)); }
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)sp, 128))));
    { float f = f0; uint32_t bits; std::memcpy(&bits, &f, sizeof(bits)); A1_W32B((uint32_t)s5, 12, bits); }
    vf8 = a4v::castsi128_ps(A4_R128(ADD32((uint32_t)s5, 0)));
    vf14 = a4v::castsi128_ps(A4_R128(ADD32((uint32_t)sp, 0)));
    vf15 = a4v::castsi128_ps(A4_R128(ADD32((uint32_t)sp, 16)));
    vf16 = a4v::castsi128_ps(A4_R128(ADD32((uint32_t)sp, 32)));
    vf17 = a4v::castsi128_ps(A4_R128(ADD32((uint32_t)sp, 48)));
    { a4::vec4 res = a4v::mul(vf14, a4v::bcast<0>(vf8)); vacc = a4v::dest<1, 1, 1, 1>(vacc, res); }
    { a4::vec4 mul_res = a4v::mul(vf15, a4v::bcast<1>(vf8)); a4::vec4 res = a4v::addK<1, 1>(vacc, mul_res); vacc = a4v::dest<1, 1, 1, 1>(vacc, res); }
    { a4::vec4 mul_res = a4v::mul(vf16, a4v::bcast<2>(vf8)); a4::vec4 res = a4v::addK<1, 1>(vacc, mul_res); vacc = a4v::dest<1, 1, 1, 1>(vacc, res); }
    { a4::vec4 mul_res = a4v::mul(vf17, a4v::bcast<3>(vf8)); a4::vec4 res = a4v::addK<1, 1>(vacc, mul_res); a4::q128 mask = a4v::set_epi32(-1, -1, -1, -1); vf10 = a4v::blendv_ps(vf10, res, a4v::castsi128_ps(mask)); }
    A4_W128(ADD32((uint32_t)v1, 0), a4v::castps_si128(vf10));
    { uint32_t bits = A1_R32B((uint32_t)sp, 132); float f; std::memcpy(&f, &bits, sizeof(f)); f21 = f; }
    A1_W32B((uint32_t)sp, 132, 0u);
    vf7 = a4v::castsi128_ps(A4_R128(ADD32((uint32_t)v1, 0)));
    { a4::vec4 res = a4v::mul(vf7, vf7); a4::q128 mask = a4v::set_epi32(0, -1, -1, -1); vf7 = a4v::blend(vf7, res, a4v::castsi128_ps(mask)); }
    { a4::vec4 res = a4v::mulK<1, 1>(vf0, a4v::bcast<0>(vf7)); vacc = a4v::dest<0, 0, 0, 1>(vacc, res); }
    { a4::vec4 mul_res = a4v::mulK<1, 1>(vf0, a4v::bcast<1>(vf7)); a4::vec4 res = a4v::addK<1, 1>(vacc, mul_res); vacc = a4v::dest<0, 0, 0, 1>(vacc, res); }
    { a4::vec4 mul_res = a4v::mulK<1, 1>(vf0, a4v::bcast<2>(vf7)); a4::vec4 res = a4v::addK<1, 1>(vacc, mul_res); a4::q128 mask = a4v::set_epi32(-1, 0, 0, 0); vf7 = a4v::blendv_ps(vf7, res, a4v::castsi128_ps(mask)); }
    { float ft = a4v::cvtss_f32(a4v::shuffle_ps<0x03>(vf7, vf7)); vq = a4v::sqrt(vstat, ft); }
    { uint32_t bits; std::memcpy(&bits, &vq, sizeof(bits)); (v0 = (uint64_t)((int64_t)(int32_t)(bits))); }
    { uint32_t bits = (uint32_t)v0; std::memcpy(&f20, &bits, sizeof(bits)); }
    f0 = a1f::div(fcr31, f0, f20);
    { uint32_t bits; std::memcpy(&bits, &f0, sizeof(bits)); (v0 = (uint64_t)((int64_t)(int32_t)(bits))); }
    vf5 = a4v::castsi128_ps(A4_R128(ADD32((uint32_t)v1, 0)));
    vf6 = a4v::castsi128_ps(a4::mk128(h2, v0));
    { a4::vec4 res = a4v::mul(vf5, a4v::bcast<0>(vf6)); a4::q128 mask = a4v::set_epi32(0, -1, -1, -1); vf5 = a4v::blendv_ps(vf5, res, a4v::castsi128_ps(mask)); }
    A4_W128(ADD32((uint32_t)v1, 0), a4v::castps_si128(vf5));
    { uint32_t bits = A1_R32B((uint32_t)sp, 128); float f; std::memcpy(&f, &bits, sizeof(f)); f12 = f; }
    { { uint32_t bits = A1_R32B((uint32_t)sp, 136); float f; std::memcpy(&f, &bits, sizeof(f)); f13 = f; } A3_CHARGE(); f0 = a9x::atan2Exact(ram, rt, f12, f13); }
    { uint32_t bits = A1_R32B((uint32_t)s4, 16); float f; std::memcpy(&f, &bits, sizeof(f)); f3 = f; }
    fcr31 = (a1f::le(f3, f20)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
    { const bool c = (fcr31 & 0x800000u) == 0u; f2 = ((float)(f0)); if (c) goto L_142d9c0; }
    f0 = a1f::add(f22, f23);
    fcr31 = (a1f::le(f20, f0)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
    if ((fcr31 & 0x800000u) == 0u) { f1 = std::fabs((float)(f2)); goto L_142d9c4; }
    f1 = std::fabs((float)(f21));
    { uint32_t bits = A1_R32B((uint32_t)s4, 28); float f; std::memcpy(&f, &bits, sizeof(f)); f0 = f; }
    fcr31 = (a1f::le(f1, f0)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
    { const bool c = (fcr31 & 0x800000u) == 0u; f1 = std::fabs((float)(f2)); if (c) goto L_142d9c4; }
    { uint32_t bits = A1_R32B((uint32_t)s4, 24); float f; std::memcpy(&f, &bits, sizeof(f)); f0 = f; }
    fcr31 = (a1f::le(f1, f0)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
    { const bool c = (fcr31 & 0x800000u) == 0u; (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 5)))); if (c) goto L_142da08; }
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 1))));
    goto L_142da08;
L_142d9c0:;
    f1 = std::fabs((float)(f2));
L_142d9c4:;
    { uint32_t bits = A1_R32B((uint32_t)s4, 24); float f; std::memcpy(&f, &bits, sizeof(f)); f0 = f; }
    fcr31 = (a1f::le(f1, f0)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
    { const bool c = (fcr31 & 0x800000u) == 0u; (v0 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL)); if (c) goto L_142da08; }
    fcr31 = (a1f::lt(f20, f3)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
    { const bool c = (fcr31 & 0x800000u) != 0u; (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 3)))); if (c) goto L_142da08; }
    f0 = a1f::add(f22, f23);
    fcr31 = (a1f::lt(f0, f20)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
    { const bool c = (fcr31 & 0x800000u) != 0u; (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 2)))); if (c) goto L_142da08; }
    f1 = std::fabs((float)(f21));
    { uint32_t bits = A1_R32B((uint32_t)s4, 28); float f; std::memcpy(&f, &bits, sizeof(f)); f0 = f; }
    fcr31 = (a1f::lt(f1, f0)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
    { const bool c = (fcr31 & 0x800000u) != 0u; (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 4)))); if (c) goto L_142da08; }
    (v0 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL));
L_142da08:;
    { uint32_t bits = A1_R32B((uint32_t)sp, 272); float f; std::memcpy(&f, &bits, sizeof(f)); f23 = f; }
    { uint32_t bits = A1_R32B((uint32_t)sp, 264); float f; std::memcpy(&f, &bits, sizeof(f)); f22 = f; }
    { uint32_t bits = A1_R32B((uint32_t)sp, 256); float f; std::memcpy(&f, &bits, sizeof(f)); f21 = f; }
    { uint32_t bits = A1_R32B((uint32_t)sp, 248); float f; std::memcpy(&f, &bits, sizeof(f)); f20 = f; }
    { (sp = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)sp, 288)))); if (xo) { xo[0] = a1_bits(f12); xo[1] = a1_bits(f13); xo[2] = a1_bits(f20); xo[3] = a1_bits(f21); xo[4] = a1_bits(f22); xo[5] = a1_bits(f23); if (a3xw6) xo[6] = a1; if (a3xw7) xo[7] = a2; if (a3xw8) xo[8] = a3; xo[9] = t0; xo[10] = t1; xo[11] = t2; xo[12] = t3; } io_vf4 = vf4; io_vf5 = vf5; io_vf6 = vf6; io_vf7 = vf7; io_vf8 = vf8; io_vf10 = vf10; io_vf11 = vf11; io_vf12 = vf12; io_vf13 = vf13; io_vf14 = vf14; io_vf15 = vf15; io_vf16 = vf16; io_vf17 = vf17; io_vf24 = vf24; io_vf25 = vf25; io_vf26 = vf26; io_vf27 = vf27; io_vf28 = vf28; io_vf29 = vf29; io_vacc = vacc; io_vq = vq; io_vstat = vstat; io_h3 = h3; io_h8 = h8; io_h9 = h9; io_h10 = h10; io_h11 = h11; io_h12 = h12; io_h13 = h13; io_h14 = h14; io_h15 = h15; return a1::RetVF{(uint64_t)v0, f0}; }
    __builtin_unreachable();
}

// chkAttainExecColumn (0x1432f40): typed by tools/typed_lift2.py (round A6; guest frame calls; VU0 in vf0 vf5 vf6 vf7 vf10 vf12 vf13 vf14 vf15 vf16 vf17 vf25 vf28 vf29 vacc vstat, out vf4 vf5 vf6 vf7 vf8 vf10 vf11 vf12 vf13 vf14 vf15 vf16 vf17 vf24 vf25 vf26 vf28 vf29 vacc vq vstat h2 h8 h9 h10 h11 h12 h13 h14 h15, relocated calls 1)
inline a1::RetVF chkAttainExecColumn(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint64_t a0, uint64_t a1, uint64_t a2, uint64_t a3, uint64_t sp, float f12, float f13, float f14, float f15, float f16, float f17, float f18, float f19, const a4::vec4 &io_vf0, a4::vec4 &io_vf4, a4::vec4 &io_vf5, a4::vec4 &io_vf6, a4::vec4 &io_vf7, a4::vec4 &io_vf8, a4::vec4 &io_vf10, a4::vec4 &io_vf11, a4::vec4 &io_vf12, a4::vec4 &io_vf13, a4::vec4 &io_vf14, a4::vec4 &io_vf15, a4::vec4 &io_vf16, a4::vec4 &io_vf17, a4::vec4 &io_vf24, a4::vec4 &io_vf25, a4::vec4 &io_vf26, a4::vec4 &io_vf28, a4::vec4 &io_vf29, a4::vec4 &io_vacc, float &io_vq, uint16_t &io_vstat, uint64_t &io_h2, uint64_t &io_h8, uint64_t &io_h9, uint64_t &io_h10, uint64_t &io_h11, uint64_t &io_h12, uint64_t &io_h13, uint64_t &io_h14, uint64_t &io_h15)
{
    float f0 = 0.0f;
    float f1 = 0.0f;
    float f2 = 0.0f;
    uint64_t at = 0;
    uint64_t v0 = 0;
    uint64_t v1 = 0;
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
    a4::vec4 vf24 = io_vf24;
    a4::vec4 vf25 = io_vf25;
    a4::vec4 vf26 = io_vf26;
    a4::vec4 vf28 = io_vf28;
    a4::vec4 vf29 = io_vf29;
    a4::vec4 vacc = io_vacc;
    float vq = io_vq;
    uint16_t vstat = io_vstat;
    uint64_t h2 = io_h2;
    uint64_t h8 = io_h8;
    uint64_t h9 = io_h9;
    uint64_t h10 = io_h10;
    uint64_t h11 = io_h11;
    uint64_t h12 = io_h12;
    uint64_t h13 = io_h13;
    uint64_t h14 = io_h14;
    uint64_t h15 = io_h15;
    (sp = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)sp, 4294967168))));
    (s0 = (uint64_t)((uint64_t)a1 + (uint64_t)0ULL));
    (s1 = (uint64_t)((uint64_t)a0 + (uint64_t)0ULL));
    (s2 = (uint64_t)((uint64_t)a2 + (uint64_t)0ULL));
    (a0 = (uint64_t)((uint64_t)RC_U64X(sp) + (uint64_t)0ULL));
    do { const a4::q128 _v = (A4_R128(ADD32((uint32_t)a3, 1136))); h2 = a4::hi64(_v); std::memcpy(&v0, &_v, 8); } while (0);
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)s1, 232))));
    { const bool c = (int64_t)v1 == 0; A4_W128(ADD32((uint32_t)sp, 0), a4::mk128(h2, v0)); if (c) goto L_1432f7c; }
    { A3_CHARGE(); StabilizeToYRotQuaternion(ram, rt, a0, (uint32_t)sp, f12, f13, f14, f15, f16, f17, f18, f19, vf0, vf4, vf5, vf6, vf7, vf11, vf12, vf13, vf14, vf15, vf16, vacc, vq, vstat); }
L_1432f7c:;
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)s2, 308))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)sp, 16))));
    { a4::vec4 res = a4v::addK<1, 1>(vf0, a4v::bcast<3>(vf0)); a4::q128 mask = a4v::set_epi32(0, -1, -1, -1); vf12 = a4v::blendv_ps(vf12, res, a4v::castsi128_ps(mask)); }
    vf11 = a4v::castsi128_ps(A4_R128(ADD32((uint32_t)sp, 0)));
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
    A4_W128(ADD32((uint32_t)v0, 0), a4v::castps_si128(vf14));
    A4_W128(ADD32((uint32_t)v0, 16), a4v::castps_si128(vf15));
    A4_W128(ADD32((uint32_t)v0, 32), a4v::castps_si128(vf16));
    vf4 = a4v::castsi128_ps(A4_R128(ADD32((uint32_t)a0, 0)));
    { a4::q128 mask = a4v::set_epi32(-1, 0, 0, 0); vf4 = a4v::blendv_ps(vf4, vf0, a4v::castsi128_ps(mask)); }
    A4_W128(ADD32((uint32_t)v0, 48), a4v::castps_si128(vf4));
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)sp, 16))));
    vf24 = a4v::castsi128_ps(A4_R128(ADD32((uint32_t)v1, 48)));
    { a4::vec4 res = a4v::subK<1, 1>(vf0, a4v::bcast<3>(vf0)); a4::q128 mask = a4v::set_epi32(0, 0, 0, -1); vf25 = a4v::blendv_ps(vf25, res, a4v::castsi128_ps(mask)); }
    { a4::vec4 res = a4v::mulK<0, 1>(vf24, a4v::bcast<0>(vf25)); a4::q128 mask = a4v::set_epi32(0, -1, -1, -1); vf28 = a4v::blendv_ps(vf28, res, a4v::castsi128_ps(mask)); }
    { a4::vec4 res = a4v::sub(vf28, vf28); a4::q128 mask = a4v::set_epi32(-1, 0, 0, 0); vf28 = a4v::blend(vf28, res, a4v::castsi128_ps(mask)); }
    do { const a4::q128 _v = (A4_R128(ADD32((uint32_t)v1, 0))); h8 = a4::hi64(_v); std::memcpy(&t0, &_v, 8); } while (0);
    do { const a4::q128 _v = (A4_R128(ADD32((uint32_t)v1, 16))); h9 = a4::hi64(_v); std::memcpy(&t1, &_v, 8); } while (0);
    do { const a4::q128 _v = (A4_R128(ADD32((uint32_t)v1, 32))); h10 = a4::hi64(_v); std::memcpy(&t2, &_v, 8); } while (0);
    do { const a4::q128 _v = (a4v::castps_si128(vf0)); h11 = a4::hi64(_v); std::memcpy(&t3, &_v, 8); } while (0);
    do { const a4::q128 _v = (a4m::pextlw(a4::mk128(h9, t1), a4::mk128(h8, t0))); h12 = a4::hi64(_v); std::memcpy(&t4, &_v, 8); } while (0);
    do { const a4::q128 _v = (a4m::pextuw(a4::mk128(h9, t1), a4::mk128(h8, t0))); h13 = a4::hi64(_v); std::memcpy(&t5, &_v, 8); } while (0);
    do { const a4::q128 _v = (a4m::pextlw(a4::mk128(h11, t3), a4::mk128(h10, t2))); h14 = a4::hi64(_v); std::memcpy(&t6, &_v, 8); } while (0);
    do { const a4::q128 _v = (a4m::pextuw(a4::mk128(h11, t3), a4::mk128(h10, t2))); h15 = a4::hi64(_v); std::memcpy(&t7, &_v, 8); } while (0);
    do { const a4::q128 _v = (a4m::pcpyld(a4::mk128(h14, t6), a4::mk128(h12, t4))); h8 = a4::hi64(_v); std::memcpy(&t0, &_v, 8); } while (0);
    do { const a4::q128 _v = (a4v::unpackhi_epi64(a4::mk128(h12, t4), a4::mk128(h14, t6))); h9 = a4::hi64(_v); std::memcpy(&t1, &_v, 8); } while (0);
    do { const a4::q128 _v = (a4m::pcpyld(a4::mk128(h15, t7), a4::mk128(h13, t5))); h10 = a4::hi64(_v); std::memcpy(&t2, &_v, 8); } while (0);
    do { const a4::q128 _v = (a4v::unpackhi_epi64(a4::mk128(h13, t5), a4::mk128(h15, t7))); h11 = a4::hi64(_v); std::memcpy(&t3, &_v, 8); } while (0);
    vf24 = a4v::castsi128_ps(a4::mk128(h8, t0));
    vf25 = a4v::castsi128_ps(a4::mk128(h9, t1));
    vf26 = a4v::castsi128_ps(a4::mk128(h10, t2));
    { a4::vec4 res = a4v::mulK<0, 1>(vf24, a4v::bcast<0>(vf28)); vacc = a4v::dest<1, 1, 1, 1>(vacc, res); }
    { a4::vec4 mul_res = a4v::mulK<0, 1>(vf25, a4v::bcast<1>(vf28)); a4::vec4 res = a4v::addK<1, 1>(vacc, mul_res); vacc = a4v::dest<1, 1, 1, 1>(vacc, res); }
    { a4::vec4 mul_res = a4v::mulK<0, 1>(vf26, a4v::bcast<2>(vf28)); a4::vec4 res = a4v::addK<1, 1>(vacc, mul_res); vacc = a4v::dest<1, 1, 1, 1>(vacc, res); }
    { a4::vec4 mul_res = a4v::mulK<1, 1>(vf0, a4v::bcast<3>(vf28)); a4::vec4 res = a4v::addK<1, 1>(vacc, mul_res); a4::q128 mask = a4v::set_epi32(-1, -1, -1, -1); vf29 = a4v::blendv_ps(vf29, res, a4v::castsi128_ps(mask)); }
    { a4::vec4 res = a4v::subK<1, 1>(vf29, vf29); a4::q128 mask = a4v::set_epi32(-1, 0, 0, 0); vf29 = a4v::blend(vf29, res, a4v::castsi128_ps(mask)); }
    { a4::vec4 res = a4v::addK<1, 1>(vf29, vf0); a4::q128 mask = a4v::set_epi32(-1, 0, 0, 0); vf29 = a4v::blend(vf29, res, a4v::castsi128_ps(mask)); }
    A4_W128(ADD32((uint32_t)v1, 0), a4v::castps_si128(vf24));
    A4_W128(ADD32((uint32_t)v1, 16), a4v::castps_si128(vf25));
    A4_W128(ADD32((uint32_t)v1, 32), a4v::castps_si128(vf26));
    A4_W128(ADD32((uint32_t)v1, 48), a4v::castps_si128(vf29));
    (at = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)16256 << 16))));
    { uint32_t bits = (uint32_t)at; std::memcpy(&f0, &bits, sizeof(bits)); }
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)sp, 80))));
    { float f = f0; uint32_t bits; std::memcpy(&bits, &f, sizeof(bits)); A1_W32B((uint32_t)s0, 12, bits); }
    vf8 = a4v::castsi128_ps(A4_R128(ADD32((uint32_t)s0, 0)));
    vf14 = a4v::castsi128_ps(A4_R128(ADD32((uint32_t)v1, 0)));
    vf15 = a4v::castsi128_ps(A4_R128(ADD32((uint32_t)v1, 16)));
    vf16 = a4v::castsi128_ps(A4_R128(ADD32((uint32_t)v1, 32)));
    vf17 = a4v::castsi128_ps(A4_R128(ADD32((uint32_t)v1, 48)));
    { a4::vec4 res = a4v::mul(vf14, a4v::bcast<0>(vf8)); vacc = a4v::dest<1, 1, 1, 1>(vacc, res); }
    { a4::vec4 mul_res = a4v::mul(vf15, a4v::bcast<1>(vf8)); a4::vec4 res = a4v::addK<1, 1>(vacc, mul_res); vacc = a4v::dest<1, 1, 1, 1>(vacc, res); }
    { a4::vec4 mul_res = a4v::mul(vf16, a4v::bcast<2>(vf8)); a4::vec4 res = a4v::addK<1, 1>(vacc, mul_res); vacc = a4v::dest<1, 1, 1, 1>(vacc, res); }
    { a4::vec4 mul_res = a4v::mul(vf17, a4v::bcast<3>(vf8)); a4::vec4 res = a4v::addK<1, 1>(vacc, mul_res); a4::q128 mask = a4v::set_epi32(-1, -1, -1, -1); vf10 = a4v::blendv_ps(vf10, res, a4v::castsi128_ps(mask)); }
    A4_W128(ADD32((uint32_t)a0, 0), a4v::castps_si128(vf10));
    { uint32_t bits = A1_R32B((uint32_t)sp, 84); float f; std::memcpy(&f, &bits, sizeof(f)); f2 = f; }
    A1_W32B((uint32_t)sp, 84, 0u);
    vf5 = a4v::castsi128_ps(A4_R128(ADD32((uint32_t)a0, 0)));
    vf6 = a4v::castsi128_ps(A4_R128(ADD32((uint32_t)a0, 0)));
    { a4::vec4 res = a4v::addK<1, 1>(vf0, a4v::bcast<3>(vf0)); a4::q128 mask = a4v::set_epi32(0, 0, 0, -1); vf7 = a4v::blendv_ps(vf7, res, a4v::castsi128_ps(mask)); }
    { a4::vec4 res = a4v::mul(vf5, vf6); a4::q128 mask = a4v::set_epi32(0, -1, -1, -1); vf6 = a4v::blend(vf6, res, a4v::castsi128_ps(mask)); }
    { a4::vec4 res = a4v::addK<1, 1>(vf0, a4v::bcast<0>(vf6)); vacc = a4v::dest<1, 0, 0, 0>(vacc, res); }
    { a4::vec4 mul_res = a4v::mulK<1, 1>(vf7, a4v::bcast<1>(vf6)); a4::vec4 res = a4v::addK<1, 1>(vacc, mul_res); vacc = a4v::dest<1, 0, 0, 0>(vacc, res); }
    { a4::vec4 mul_res = a4v::mulK<1, 1>(vf7, a4v::bcast<2>(vf6)); a4::vec4 res = a4v::addK<1, 1>(vacc, mul_res); a4::q128 mask = a4v::set_epi32(0, 0, 0, -1); vf6 = a4v::blendv_ps(vf6, res, a4v::castsi128_ps(mask)); }
    do { const a4::q128 _v = (a4v::castps_si128(vf6)); h2 = a4::hi64(_v); std::memcpy(&v0, &_v, 8); } while (0);
    { uint32_t bits = (uint32_t)v0; std::memcpy(&f1, &bits, sizeof(bits)); }
    { uint32_t bits = A1_R32B((uint32_t)s1, 972); float f; std::memcpy(&f, &bits, sizeof(f)); f0 = f; }
    f0 = a1f::mul(f0, f0);
    fcr31 = (a1f::le(f1, f0)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
    { const bool c = (fcr31 & 0x800000u) == 0u; (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 1)))); if (c) goto L_1433100; }
    f1 = std::fabs((float)(f2));
    { uint32_t bits = A1_R32B((uint32_t)s1, 976); float f; std::memcpy(&f, &bits, sizeof(f)); f0 = f; }
    fcr31 = (a1f::le(f1, f0)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
    if ((fcr31 & 0x800000u) != 0u) goto L_1433108;
L_1433100:;
    (v0 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL));
L_1433108:;
    { (sp = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)sp, 128)))); io_vf4 = vf4; io_vf5 = vf5; io_vf6 = vf6; io_vf7 = vf7; io_vf8 = vf8; io_vf10 = vf10; io_vf11 = vf11; io_vf12 = vf12; io_vf13 = vf13; io_vf14 = vf14; io_vf15 = vf15; io_vf16 = vf16; io_vf17 = vf17; io_vf24 = vf24; io_vf25 = vf25; io_vf26 = vf26; io_vf28 = vf28; io_vf29 = vf29; io_vacc = vacc; io_vq = vq; io_vstat = vstat; io_h2 = h2; io_h8 = h8; io_h9 = h9; io_h10 = h10; io_h11 = h11; io_h12 = h12; io_h13 = h13; io_h14 = h14; io_h15 = h15; return a1::RetVF{(uint64_t)v0, f0}; }
    __builtin_unreachable();
}

// nico_calcTailGenealogicalNodeNext (0x1461bc0): typed by tools/typed_lift2.py (round A6; guest frame calls; VU0 in vf0 vf7 vf8 vf9 vf10 vf12 vf13 vf14 vf15 vf16 vf17 vf25 vf28 vf29 vacc vstat h2 h5 h8, out vf4 vf5 vf6 vf7 vf8 vf9 vf10 vf11 vf12 vf13 vf14 vf15 vf16 vf17 vf24 vf25 vf26 vf27 vf28 vf29 vacc vq vstat h2 h3 h4 h5 h8 h9 h10 h11 h12 h13 h14 h15, relocated calls 7)
inline uint64_t nico_calcTailGenealogicalNodeNext(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint64_t v0, uint64_t a0, uint64_t t2, uint64_t sp, float f12, float f13, float f14, float f15, float f16, float f17, float f18, float f19, float f20, const a4::vec4 &io_vf0, a4::vec4 &io_vf4, a4::vec4 &io_vf5, a4::vec4 &io_vf6, a4::vec4 &io_vf7, a4::vec4 &io_vf8, a4::vec4 &io_vf9, a4::vec4 &io_vf10, a4::vec4 &io_vf11, a4::vec4 &io_vf12, a4::vec4 &io_vf13, a4::vec4 &io_vf14, a4::vec4 &io_vf15, a4::vec4 &io_vf16, a4::vec4 &io_vf17, a4::vec4 &io_vf24, a4::vec4 &io_vf25, a4::vec4 &io_vf26, a4::vec4 &io_vf27, a4::vec4 &io_vf28, a4::vec4 &io_vf29, a4::vec4 &io_vacc, float &io_vq, uint16_t &io_vstat, uint64_t &io_h2, uint64_t &io_h3, uint64_t &io_h4, uint64_t &io_h5, uint64_t &io_h8, uint64_t &io_h9, uint64_t &io_h10, uint64_t &io_h11, uint64_t &io_h12, uint64_t &io_h13, uint64_t &io_h14, uint64_t &io_h15)
{
    float f0 = 0.0f;
    float f1 = 0.0f;
    uint64_t at = 0;
    uint64_t v1 = 0;
    uint64_t a1 = 0;
    uint64_t a2 = 0;
    uint64_t a3 = 0;
    uint64_t t0 = 0;
    uint64_t t1 = 0;
    uint64_t t3 = 0;
    uint64_t t4 = 0;
    uint64_t t5 = 0;
    uint64_t t6 = 0;
    uint64_t t7 = 0;
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
    a4::vec4 vf9 = io_vf9;
    a4::vec4 vf10 = io_vf10;
    a4::vec4 vf11 = io_vf11;
    a4::vec4 vf12 = io_vf12;
    a4::vec4 vf13 = io_vf13;
    a4::vec4 vf14 = io_vf14;
    a4::vec4 vf15 = io_vf15;
    a4::vec4 vf16 = io_vf16;
    a4::vec4 vf17 = io_vf17;
    a4::vec4 vf24 = io_vf24;
    a4::vec4 vf25 = io_vf25;
    a4::vec4 vf26 = io_vf26;
    a4::vec4 vf27 = io_vf27;
    a4::vec4 vf28 = io_vf28;
    a4::vec4 vf29 = io_vf29;
    a4::vec4 vacc = io_vacc;
    float vq = io_vq;
    uint16_t vstat = io_vstat;
    uint64_t h2 = io_h2;
    uint64_t h3 = io_h3;
    uint64_t h4 = io_h4;
    uint64_t h5 = io_h5;
    uint64_t h8 = io_h8;
    uint64_t h9 = io_h9;
    uint64_t h10 = io_h10;
    uint64_t h11 = io_h11;
    uint64_t h12 = io_h12;
    uint64_t h13 = io_h13;
    uint64_t h14 = io_h14;
    uint64_t h15 = io_h15;
    (sp = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)sp, 4294967072))));
    (s0 = (uint64_t)((uint64_t)a0 + (uint64_t)0ULL));
    { float f = f20; uint32_t bits; std::memcpy(&bits, &f, sizeof(bits)); A1_W32B((uint32_t)sp, 216, bits); }
    (s2 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)s0, 0))));
    { (a0 = (uint64_t)((uint64_t)RC_U64X(s2) + (uint64_t)0ULL)); A3_CHARGE(); v0 = GetCharacterClass(ram, rt, a0, h2, h5); }
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)s0, 24))));
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)3 << 16))));
    (s1 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)v0, 4376))));
    (a2 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)a0, 0))));
    (v1 = (uint64_t)(RC_U64X(v1) | (uint64_t)(uint16_t)65535));
    (v0 = (uint64_t)(RC_U64X(a2) & RC_U64X(v1)));
    { const bool c = (int64_t)v0 == 0; (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)SUB32((uint32_t)a0, (uint32_t)v0)))); if (c) goto L_1461c18; }
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)v0, 0))));
    goto L_1461c1c;
L_1461c18:;
    (a0 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL));
L_1461c1c:;
    { const bool c = (int64_t)a0 == 0; { const uint32_t _dynInsn = A1_R32(0x1461C20u); const int32_t _dynSimm = (int16_t)(_dynInsn & 0xFFFFu); const uint32_t _dynImm = _dynInsn & 0xFFFFu; (void)_dynSimm; (void)_dynImm; (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)_dynImm << 16)))); } if (c) goto L_1461c50; }
    { const uint32_t _dynInsn = A1_R32(0x1461C24u); const int32_t _dynSimm = (int16_t)(_dynInsn & 0xFFFFu); const uint32_t _dynImm = _dynInsn & 0xFFFFu; (void)_dynSimm; (void)_dynImm; (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32(ADD32((uint32_t)v0, _dynSimm))))); }
    { const bool c = (int64_t)a0 == (int64_t)v1; (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)SRL32((uint32_t)a2, 18)))); if (c) goto L_1461c50; }
    (a1 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)a0, 8))));
    (v0 = (uint64_t)(RC_U64X(v0) & (uint64_t)(uint16_t)4095));
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)a0, 16))));
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)a1, 12))));
    { int64_t result = (int64_t)(int32_t)v0 * (int64_t)(int32_t)v1; lo = (uint64_t)(int64_t)(int32_t)result; hi = (uint64_t)(int64_t)(int32_t)(result >> 32); (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)result))); }
    (s3 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)a0, (uint32_t)v0))));
    goto L_1461c54;
L_1461c50:;
    (s3 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL));
L_1461c54:;
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)s1, 960))));
    if ((int64_t)v0 == 0) { (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)s0, 20)))); goto L_1462110; }
    { (a0 = (uint64_t)((uint64_t)s2 + (uint64_t)0ULL)); A3_CHARGE(); v0 = GetCharacterClass(ram, rt, a0, h2, h5); }
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)v0, 448))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)v1, 104))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)SRL32((uint32_t)v0, 3))));
    (v0 = (uint64_t)(RC_U64X(v0) & (uint64_t)(uint16_t)1));
    if ((int64_t)v0 == 0) { { uint32_t bits = A1_R32B((uint32_t)s1, 968); float f; std::memcpy(&f, &bits, sizeof(f)); f1 = f; } goto L_1461f90; }
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)s0, 8))));
    (a1 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)s0, 12))));
    (a2 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)s0, 16))));
    (a3 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)s0, 4))));
    (t0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)s0, 180))));
    (t1 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)s0, 184))));
    { (t2 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)s0, 20)))); A3_CHARGE(); v0 = GetMotionMatrixOfTargetFromParent(ram, rt, a0, (uint32_t)a1, (uint32_t)a2, (uint32_t)a3, (uint32_t)t0, (uint32_t)t1, t2, (uint32_t)sp, vf0, vf4, vf5, vf6, vf10, vf11, vf12, vf13, vf14, vf15, vf16, vf17, vf24, vf25, vf26, vf27, vacc, vq, vstat, h2); }
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)326 << 16))));
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)s0, 20))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)a0, 29700))));
    (a1 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)s0, 8))));
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)SLL32((uint32_t)v1, 6))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)SLL32((uint32_t)v0, 4))));
    (at = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)16256 << 16))));
    { uint32_t bits = (uint32_t)at; std::memcpy(&f0, &bits, sizeof(bits)); }
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v1, (uint32_t)a1))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)s1, (uint32_t)v0))));
    do { const a4::q128 _v = (A4_R128(ADD32((uint32_t)v1, 48))); h5 = a4::hi64(_v); std::memcpy(&a1, &_v, 8); } while (0);
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v0, 656))));
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v0, 640))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v0, 4294967280))));
    { uint32_t bits = A1_R32B((uint32_t)s3, 0); float f; std::memcpy(&f, &bits, sizeof(f)); f20 = f; }
    A4_W128(ADD32((uint32_t)a0, 0), a4::mk128(h5, a1));
    { float f = f0; uint32_t bits; std::memcpy(&bits, &f, sizeof(bits)); A1_W32B((uint32_t)a0, 12, bits); }
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
    A4_W128(ADD32((uint32_t)sp, 0), a4v::castps_si128(vf14));
    A4_W128(ADD32((uint32_t)sp, 16), a4v::castps_si128(vf15));
    A4_W128(ADD32((uint32_t)sp, 32), a4v::castps_si128(vf16));
    vf4 = a4v::castsi128_ps(A4_R128(ADD32((uint32_t)a0, 0)));
    { a4::q128 mask = a4v::set_epi32(-1, 0, 0, 0); vf4 = a4v::blendv_ps(vf4, vf0, a4v::castsi128_ps(mask)); }
    A4_W128(ADD32((uint32_t)sp, 48), a4v::castps_si128(vf4));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)sp, 64))));
    vf24 = a4v::castsi128_ps(A4_R128(ADD32((uint32_t)sp, 48)));
    { a4::vec4 res = a4v::subK<1, 1>(vf0, a4v::bcast<3>(vf0)); a4::q128 mask = a4v::set_epi32(0, 0, 0, -1); vf25 = a4v::blendv_ps(vf25, res, a4v::castsi128_ps(mask)); }
    { a4::vec4 res = a4v::mulK<0, 1>(vf24, a4v::bcast<0>(vf25)); a4::q128 mask = a4v::set_epi32(0, -1, -1, -1); vf28 = a4v::blendv_ps(vf28, res, a4v::castsi128_ps(mask)); }
    { a4::vec4 res = a4v::sub(vf28, vf28); a4::q128 mask = a4v::set_epi32(-1, 0, 0, 0); vf28 = a4v::blend(vf28, res, a4v::castsi128_ps(mask)); }
    do { const a4::q128 _v = (A4_R128(ADD32((uint32_t)sp, 0))); h8 = a4::hi64(_v); std::memcpy(&t0, &_v, 8); } while (0);
    do { const a4::q128 _v = (A4_R128(ADD32((uint32_t)sp, 16))); h9 = a4::hi64(_v); std::memcpy(&t1, &_v, 8); } while (0);
    do { const a4::q128 _v = (A4_R128(ADD32((uint32_t)sp, 32))); h10 = a4::hi64(_v); std::memcpy(&t2, &_v, 8); } while (0);
    do { const a4::q128 _v = (a4v::castps_si128(vf0)); h11 = a4::hi64(_v); std::memcpy(&t3, &_v, 8); } while (0);
    do { const a4::q128 _v = (a4m::pextlw(a4::mk128(h9, t1), a4::mk128(h8, t0))); h12 = a4::hi64(_v); std::memcpy(&t4, &_v, 8); } while (0);
    do { const a4::q128 _v = (a4m::pextuw(a4::mk128(h9, t1), a4::mk128(h8, t0))); h13 = a4::hi64(_v); std::memcpy(&t5, &_v, 8); } while (0);
    do { const a4::q128 _v = (a4m::pextlw(a4::mk128(h11, t3), a4::mk128(h10, t2))); h14 = a4::hi64(_v); std::memcpy(&t6, &_v, 8); } while (0);
    do { const a4::q128 _v = (a4m::pextuw(a4::mk128(h11, t3), a4::mk128(h10, t2))); h15 = a4::hi64(_v); std::memcpy(&t7, &_v, 8); } while (0);
    do { const a4::q128 _v = (a4m::pcpyld(a4::mk128(h14, t6), a4::mk128(h12, t4))); h8 = a4::hi64(_v); std::memcpy(&t0, &_v, 8); } while (0);
    do { const a4::q128 _v = (a4v::unpackhi_epi64(a4::mk128(h12, t4), a4::mk128(h14, t6))); h9 = a4::hi64(_v); std::memcpy(&t1, &_v, 8); } while (0);
    do { const a4::q128 _v = (a4m::pcpyld(a4::mk128(h15, t7), a4::mk128(h13, t5))); h10 = a4::hi64(_v); std::memcpy(&t2, &_v, 8); } while (0);
    do { const a4::q128 _v = (a4v::unpackhi_epi64(a4::mk128(h13, t5), a4::mk128(h15, t7))); h11 = a4::hi64(_v); std::memcpy(&t3, &_v, 8); } while (0);
    vf24 = a4v::castsi128_ps(a4::mk128(h8, t0));
    vf25 = a4v::castsi128_ps(a4::mk128(h9, t1));
    vf26 = a4v::castsi128_ps(a4::mk128(h10, t2));
    { a4::vec4 res = a4v::mulK<0, 1>(vf24, a4v::bcast<0>(vf28)); vacc = a4v::dest<1, 1, 1, 1>(vacc, res); }
    { a4::vec4 mul_res = a4v::mulK<0, 1>(vf25, a4v::bcast<1>(vf28)); a4::vec4 res = a4v::addK<1, 1>(vacc, mul_res); vacc = a4v::dest<1, 1, 1, 1>(vacc, res); }
    { a4::vec4 mul_res = a4v::mulK<0, 1>(vf26, a4v::bcast<2>(vf28)); a4::vec4 res = a4v::addK<1, 1>(vacc, mul_res); vacc = a4v::dest<1, 1, 1, 1>(vacc, res); }
    { a4::vec4 mul_res = a4v::mulK<1, 1>(vf0, a4v::bcast<3>(vf28)); a4::vec4 res = a4v::addK<1, 1>(vacc, mul_res); a4::q128 mask = a4v::set_epi32(-1, -1, -1, -1); vf29 = a4v::blendv_ps(vf29, res, a4v::castsi128_ps(mask)); }
    { a4::vec4 res = a4v::subK<1, 1>(vf29, vf29); a4::q128 mask = a4v::set_epi32(-1, 0, 0, 0); vf29 = a4v::blend(vf29, res, a4v::castsi128_ps(mask)); }
    { a4::vec4 res = a4v::addK<1, 1>(vf29, vf0); a4::q128 mask = a4v::set_epi32(-1, 0, 0, 0); vf29 = a4v::blend(vf29, res, a4v::castsi128_ps(mask)); }
    A4_W128(ADD32((uint32_t)v0, 0), a4v::castps_si128(vf24));
    A4_W128(ADD32((uint32_t)v0, 16), a4v::castps_si128(vf25));
    A4_W128(ADD32((uint32_t)v0, 32), a4v::castps_si128(vf26));
    A4_W128(ADD32((uint32_t)v0, 48), a4v::castps_si128(vf29));
    (a1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)sp, 128))));
    vf8 = a4v::castsi128_ps(A4_R128(ADD32((uint32_t)v1, 0)));
    vf14 = a4v::castsi128_ps(A4_R128(ADD32((uint32_t)v0, 0)));
    vf15 = a4v::castsi128_ps(A4_R128(ADD32((uint32_t)v0, 16)));
    vf16 = a4v::castsi128_ps(A4_R128(ADD32((uint32_t)v0, 32)));
    vf17 = a4v::castsi128_ps(A4_R128(ADD32((uint32_t)v0, 48)));
    { a4::vec4 res = a4v::mul(vf14, a4v::bcast<0>(vf8)); vacc = a4v::dest<1, 1, 1, 1>(vacc, res); }
    { a4::vec4 mul_res = a4v::mul(vf15, a4v::bcast<1>(vf8)); a4::vec4 res = a4v::addK<1, 1>(vacc, mul_res); vacc = a4v::dest<1, 1, 1, 1>(vacc, res); }
    { a4::vec4 mul_res = a4v::mul(vf16, a4v::bcast<2>(vf8)); a4::vec4 res = a4v::addK<1, 1>(vacc, mul_res); vacc = a4v::dest<1, 1, 1, 1>(vacc, res); }
    { a4::vec4 mul_res = a4v::mul(vf17, a4v::bcast<3>(vf8)); a4::vec4 res = a4v::addK<1, 1>(vacc, mul_res); a4::q128 mask = a4v::set_epi32(-1, -1, -1, -1); vf10 = a4v::blendv_ps(vf10, res, a4v::castsi128_ps(mask)); }
    A4_W128(ADD32((uint32_t)a1, 0), a4v::castps_si128(vf10));
    vf5 = a4v::castsi128_ps(A4_R128(ADD32((uint32_t)a1, 0)));
    { a4::vec4 res = a4v::mul(vf5, vf5); a4::q128 mask = a4v::set_epi32(0, -1, -1, -1); vf7 = a4v::blend(vf7, res, a4v::castsi128_ps(mask)); }
    { a4::vec4 res = a4v::mulK<1, 1>(vf0, a4v::bcast<0>(vf7)); vacc = a4v::dest<0, 0, 0, 1>(vacc, res); }
    { a4::vec4 mul_res = a4v::mulK<1, 1>(vf0, a4v::bcast<1>(vf7)); a4::vec4 res = a4v::addK<1, 1>(vacc, mul_res); vacc = a4v::dest<0, 0, 0, 1>(vacc, res); }
    { a4::vec4 mul_res = a4v::mulK<1, 1>(vf0, a4v::bcast<2>(vf7)); a4::vec4 res = a4v::addK<1, 1>(vacc, mul_res); a4::q128 mask = a4v::set_epi32(-1, 0, 0, 0); vf7 = a4v::blendv_ps(vf7, res, a4v::castsi128_ps(mask)); }
    { float fs = a4v::cvtss_f32(a4v::shuffle_ps<0x03>(vf0, vf0)); float ft = a4v::cvtss_f32(a4v::shuffle_ps<0x03>(vf7, vf7)); vq = a4v::rsqrt(vstat, fs, ft); }
    { a4::vec4 res = a4v::mul(vf5, a4v::set1_ps(vq)); a4::q128 mask = a4v::set_epi32(0, -1, -1, -1); vf5 = a4v::blendv_ps(vf5, res, a4v::castsi128_ps(mask)); }
    A4_W128(ADD32((uint32_t)a1, 0), a4v::castps_si128(vf5));
    (at = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)49024 << 16))));
    { uint32_t bits = (uint32_t)at; std::memcpy(&f0, &bits, sizeof(bits)); }
    { uint32_t bits; std::memcpy(&bits, &f0, sizeof(bits)); (v0 = (uint64_t)((int64_t)(int32_t)(bits))); }
    vf5 = a4v::castsi128_ps(A4_R128(ADD32((uint32_t)a1, 0)));
    vf6 = a4v::castsi128_ps(a4::mk128(h2, v0));
    { a4::vec4 res = a4v::mul(vf5, a4v::bcast<0>(vf6)); a4::q128 mask = a4v::set_epi32(0, -1, -1, -1); vf5 = a4v::blendv_ps(vf5, res, a4v::castsi128_ps(mask)); }
    A4_W128(ADD32((uint32_t)a1, 0), a4v::castps_si128(vf5));
    (at = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)16256 << 16))));
    { uint32_t bits = (uint32_t)at; std::memcpy(&f1, &bits, sizeof(bits)); }
    { const uint32_t _dynInsn = A1_R32(0x1461E48u); const int32_t _dynSimm = (int16_t)(_dynInsn & 0xFFFFu); const uint32_t _dynImm = _dynInsn & 0xFFFFu; (void)_dynSimm; (void)_dynImm; (a2 = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)_dynImm << 16)))); }
    { const uint32_t _dynInsn = A1_R32(0x1461E4Cu); const int32_t _dynSimm = (int16_t)(_dynInsn & 0xFFFFu); const uint32_t _dynImm = _dynInsn & 0xFFFFu; (void)_dynSimm; (void)_dynImm; (a3 = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)_dynImm << 16)))); }
    { const uint32_t _dynInsn = A1_R32(0x1461E50u); const int32_t _dynSimm = (int16_t)(_dynInsn & 0xFFFFu); const uint32_t _dynImm = _dynInsn & 0xFFFFu; (void)_dynSimm; (void)_dynImm; (a2 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)a2, _dynSimm)))); }
    { const uint32_t _dynInsn = A1_R32(0x1461E54u); const int32_t _dynSimm = (int16_t)(_dynInsn & 0xFFFFu); const uint32_t _dynImm = _dynInsn & 0xFFFFu; (void)_dynSimm; (void)_dynImm; (a3 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)a3, _dynSimm)))); }
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)sp, 144))));
    { { float f = f1; uint32_t bits; std::memcpy(&bits, &f, sizeof(bits)); A1_W32B((uint32_t)sp, 140, bits); } A3_CHARGE(); a9x::diffQuat(ram, rt, (uint32_t)a0, (uint32_t)a1, (uint32_t)a2, (uint32_t)a3); }
    (a1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)sp, 144))));
    vf14 = a4v::castsi128_ps(A4_R128(ADD32((uint32_t)a1, 0)));
    { a4::vec4 res = a4v::mul(vf14, vf14); a4::q128 mask = a4v::set_epi32(-1, -1, -1, -1); vf15 = a4v::blend(vf15, res, a4v::castsi128_ps(mask)); }
    { a4::vec4 res = a4v::addK<1, 1>(vf15, a4v::bcast<1>(vf15)); a4::q128 mask = a4v::set_epi32(0, 0, 0, -1); vf15 = a4v::blendv_ps(vf15, res, a4v::castsi128_ps(mask)); }
    { a4::vec4 res = a4v::addK<1, 1>(vf15, a4v::bcast<2>(vf15)); a4::q128 mask = a4v::set_epi32(0, 0, 0, -1); vf15 = a4v::blendv_ps(vf15, res, a4v::castsi128_ps(mask)); }
    { a4::vec4 res = a4v::addK<1, 1>(vf15, a4v::bcast<3>(vf15)); a4::q128 mask = a4v::set_epi32(0, 0, 0, -1); vf15 = a4v::blendv_ps(vf15, res, a4v::castsi128_ps(mask)); }
    { float fs = a4v::cvtss_f32(a4v::shuffle_ps<0x03>(vf0, vf0)); float ft = a4v::cvtss_f32(a4v::bcast<0>(vf15)); vq = a4v::rsqrt(vstat, fs, ft); }
    { a4::vec4 res = a4v::mul(vf14, a4v::set1_ps(vq)); a4::q128 mask = a4v::set_epi32(-1, -1, -1, -1); vf15 = a4v::blendv_ps(vf15, res, a4v::castsi128_ps(mask)); }
    A4_W128(ADD32((uint32_t)a1, 0), a4v::castps_si128(vf15));
    (at = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)15502 << 16))));
    (at = (uint64_t)(RC_U64X(at) | (uint64_t)(uint16_t)64053));
    { uint32_t bits = (uint32_t)at; std::memcpy(&f0, &bits, sizeof(bits)); }
    (s2 = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)326 << 16))));
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)s0, 20))));
    f20 = a1f::mul(f20, f0);
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)s0, 4))));
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)SLL32((uint32_t)a0, 4))));
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v0, (uint32_t)a0))));
    { f12 = a1f::add(f20, f20); A3_CHARGE(); { const a1::RetVF a2_r = GetLimitationQuaternion(ram, rt, a0, a1, sp, f0, f12, f13, f14, f15, f16, f17, f18, f19, f20, vf0, vf5, vf6, vf7, vacc, vq, vstat, h2, h3); v0 = a2_r.v0; f0 = a2_r.f0; } }
    (a2 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)s2, 29700))));
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)a2, 0))));
    (a1 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)s0, 20))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)s0, 4))));
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)SLL32((uint32_t)v1, 4))));
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)s1, (uint32_t)v1))));
    (a1 = (uint64_t)((int64_t)(int32_t)((int32_t)SLL32((uint32_t)a1, 4))));
    (a1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)a1, (uint32_t)v0))));
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v1, 4294967280))));
    vf11 = a4v::castsi128_ps(A4_R128(ADD32((uint32_t)a0, 0)));
    vf12 = a4v::castsi128_ps(A4_R128(ADD32((uint32_t)a1, 0)));
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
    A4_W128(ADD32((uint32_t)v1, 0), a4v::castps_si128(vf13));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)a2, 0))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)SLL32((uint32_t)v0, 4))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v0, (uint32_t)s1))));
    vf14 = a4v::castsi128_ps(A4_R128(ADD32((uint32_t)v0, 0)));
    { a4::vec4 res = a4v::mul(vf14, vf14); a4::q128 mask = a4v::set_epi32(-1, -1, -1, -1); vf15 = a4v::blend(vf15, res, a4v::castsi128_ps(mask)); }
    { a4::vec4 res = a4v::addK<1, 1>(vf15, a4v::bcast<1>(vf15)); a4::q128 mask = a4v::set_epi32(0, 0, 0, -1); vf15 = a4v::blendv_ps(vf15, res, a4v::castsi128_ps(mask)); }
    { a4::vec4 res = a4v::addK<1, 1>(vf15, a4v::bcast<2>(vf15)); a4::q128 mask = a4v::set_epi32(0, 0, 0, -1); vf15 = a4v::blendv_ps(vf15, res, a4v::castsi128_ps(mask)); }
    { a4::vec4 res = a4v::addK<1, 1>(vf15, a4v::bcast<3>(vf15)); a4::q128 mask = a4v::set_epi32(0, 0, 0, -1); vf15 = a4v::blendv_ps(vf15, res, a4v::castsi128_ps(mask)); }
    { float fs = a4v::cvtss_f32(a4v::shuffle_ps<0x03>(vf0, vf0)); float ft = a4v::cvtss_f32(a4v::bcast<0>(vf15)); vq = a4v::rsqrt(vstat, fs, ft); }
    { a4::vec4 res = a4v::mul(vf14, a4v::set1_ps(vq)); a4::q128 mask = a4v::set_epi32(-1, -1, -1, -1); vf15 = a4v::blendv_ps(vf15, res, a4v::castsi128_ps(mask)); }
    A4_W128(ADD32((uint32_t)v0, 0), a4v::castps_si128(vf15));
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)s0, 20))));
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)s0, 4))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)a2, 0))));
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)SLL32((uint32_t)v1, 4))));
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v1, (uint32_t)a0))));
    do { const a4::q128 _v = (A4_R128(ADD32((uint32_t)v1, 0))); h4 = a4::hi64(_v); std::memcpy(&a0, &_v, 8); } while (0);
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)SLL32((uint32_t)v0, 4))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v0, (uint32_t)s1))));
    A4_W128(ADD32((uint32_t)v0, 320), a4::mk128(h4, a0));
    (t2 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)s0, 20))));
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)s0, 8))));
    (a1 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)s0, 12))));
    (a2 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)s0, 16))));
    (a3 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)s0, 4))));
    (t0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)s0, 180))));
    { (t1 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)s0, 184)))); A3_CHARGE(); v0 = GetMotionMatrixOfTargetFromParent(ram, rt, a0, (uint32_t)a1, (uint32_t)a2, (uint32_t)a3, (uint32_t)t0, (uint32_t)t1, t2, (uint32_t)sp, vf0, vf4, vf5, vf6, vf10, vf11, vf12, vf13, vf14, vf15, vf16, vf17, vf24, vf25, vf26, vf27, vacc, vq, vstat, h2); }
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)s2, 29700))));
    goto L_1462184;
L_1461f90:;
    { uint32_t bits = 0u; std::memcpy(&f0, &bits, sizeof(bits)); }
    fcr31 = (a1f::lt(f0, f1)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
    if ((fcr31 & 0x800000u) == 0u) { (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)s0, 20)))); goto L_1462098; }
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)s0, 20))));
    (s2 = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)326 << 16))));
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)s0, 4))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)SLL32((uint32_t)v0, 4))));
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)s2, 29700))));
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v0, (uint32_t)a0))));
    do { const a4::q128 _v = (A4_R128(ADD32((uint32_t)a0, 0))); h2 = a4::hi64(_v); std::memcpy(&v0, &_v, 8); } while (0);
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)SLL32((uint32_t)v1, 4))));
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v1, (uint32_t)s1))));
    A4_W128(ADD32((uint32_t)sp, 0), a4::mk128(h2, v0));
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v1, 320))));
    vf14 = a4v::castsi128_ps(A4_R128(ADD32((uint32_t)v1, 0)));
    vf15 = a4v::castsi128_ps(A4_R128(ADD32((uint32_t)sp, 0)));
    { a4::vec4 res = a4v::mul(vf14, vf15); a4::q128 mask = a4v::set_epi32(-1, -1, -1, -1); vf15 = a4v::blend(vf15, res, a4v::castsi128_ps(mask)); }
    { a4::vec4 res = a4v::addK<1, 1>(vf15, a4v::bcast<1>(vf15)); a4::q128 mask = a4v::set_epi32(0, 0, 0, -1); vf15 = a4v::blendv_ps(vf15, res, a4v::castsi128_ps(mask)); }
    { a4::vec4 res = a4v::addK<1, 1>(vf15, a4v::bcast<2>(vf15)); a4::q128 mask = a4v::set_epi32(0, 0, 0, -1); vf15 = a4v::blendv_ps(vf15, res, a4v::castsi128_ps(mask)); }
    { a4::vec4 res = a4v::addK<1, 1>(vf15, a4v::bcast<3>(vf15)); a4::q128 mask = a4v::set_epi32(0, 0, 0, -1); vf15 = a4v::blendv_ps(vf15, res, a4v::castsi128_ps(mask)); }
    do { const a4::q128 _v = (a4v::castps_si128(vf15)); h2 = a4::hi64(_v); std::memcpy(&v0, &_v, 8); } while (0);
    { uint32_t bits = (uint32_t)v0; std::memcpy(&f0, &bits, sizeof(bits)); }
    { float f = f0; uint32_t bits; std::memcpy(&bits, &f, sizeof(bits)); A1_W32B((uint32_t)sp, 160, bits); }
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)sp, 160))));
    if ((int32_t)v0 >= 0) goto L_1462018;
    (at = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)49024 << 16))));
    { uint32_t bits = (uint32_t)at; std::memcpy(&f0, &bits, sizeof(bits)); }
    { uint32_t bits; std::memcpy(&bits, &f0, sizeof(bits)); (v0 = (uint64_t)((int64_t)(int32_t)(bits))); }
    vf5 = a4v::castsi128_ps(A4_R128(ADD32((uint32_t)sp, 0)));
    vf6 = a4v::castsi128_ps(a4::mk128(h2, v0));
    { a4::vec4 res = a4v::mul(vf5, a4v::bcast<0>(vf6)); a4::q128 mask = a4v::set_epi32(-1, -1, -1, -1); vf5 = a4v::blendv_ps(vf5, res, a4v::castsi128_ps(mask)); }
    A4_W128(ADD32((uint32_t)sp, 0), a4v::castps_si128(vf5));
L_1462018:;
    { uint32_t bits; std::memcpy(&bits, &f1, sizeof(bits)); (t0 = (uint64_t)((int64_t)(int32_t)(bits))); }
    vf7 = a4v::castsi128_ps(a4::mk128(h8, t0));
    vf5 = a4v::castsi128_ps(A4_R128(ADD32((uint32_t)v1, 0)));
    vf6 = a4v::castsi128_ps(A4_R128(ADD32((uint32_t)sp, 0)));
    { a4::vec4 res = a4v::subK<1, 0>(vf0, a4v::bcast<0>(vf7)); a4::q128 mask = a4v::set_epi32(-1, 0, 0, 0); vf8 = a4v::blendv_ps(vf8, res, a4v::castsi128_ps(mask)); }
    { a4::vec4 res = a4v::mul(vf5, a4v::bcast<0>(vf7)); vacc = a4v::dest<1, 1, 1, 1>(vacc, res); }
    { a4::vec4 mul_res = a4v::mulK<0, 1>(vf6, a4v::bcast<3>(vf8)); a4::vec4 res = a4v::addK<1, 1>(vacc, mul_res); a4::q128 mask = a4v::set_epi32(-1, -1, -1, -1); vf9 = a4v::blendv_ps(vf9, res, a4v::castsi128_ps(mask)); }
    A4_W128(ADD32((uint32_t)a0, 0), a4v::castps_si128(vf9));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)s0, 20))));
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)s0, 4))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)SLL32((uint32_t)v0, 4))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v0, (uint32_t)v1))));
    vf14 = a4v::castsi128_ps(A4_R128(ADD32((uint32_t)v0, 0)));
    { a4::vec4 res = a4v::mul(vf14, vf14); a4::q128 mask = a4v::set_epi32(-1, -1, -1, -1); vf15 = a4v::blend(vf15, res, a4v::castsi128_ps(mask)); }
    { a4::vec4 res = a4v::addK<1, 1>(vf15, a4v::bcast<1>(vf15)); a4::q128 mask = a4v::set_epi32(0, 0, 0, -1); vf15 = a4v::blendv_ps(vf15, res, a4v::castsi128_ps(mask)); }
    { a4::vec4 res = a4v::addK<1, 1>(vf15, a4v::bcast<2>(vf15)); a4::q128 mask = a4v::set_epi32(0, 0, 0, -1); vf15 = a4v::blendv_ps(vf15, res, a4v::castsi128_ps(mask)); }
    { a4::vec4 res = a4v::addK<1, 1>(vf15, a4v::bcast<3>(vf15)); a4::q128 mask = a4v::set_epi32(0, 0, 0, -1); vf15 = a4v::blendv_ps(vf15, res, a4v::castsi128_ps(mask)); }
    { float fs = a4v::cvtss_f32(a4v::shuffle_ps<0x03>(vf0, vf0)); float ft = a4v::cvtss_f32(a4v::bcast<0>(vf15)); vq = a4v::rsqrt(vstat, fs, ft); }
    { a4::vec4 res = a4v::mul(vf14, a4v::set1_ps(vq)); a4::q128 mask = a4v::set_epi32(-1, -1, -1, -1); vf15 = a4v::blendv_ps(vf15, res, a4v::castsi128_ps(mask)); }
    A4_W128(ADD32((uint32_t)v0, 0), a4v::castps_si128(vf15));
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)s0, 8))));
    (a1 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)s0, 12))));
    (a2 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)s0, 16))));
    (a3 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)s0, 4))));
    (t0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)s0, 180))));
    (t1 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)s0, 184))));
    { (t2 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)s0, 20)))); A3_CHARGE(); v0 = GetMotionMatrixOfTargetFromParent(ram, rt, a0, (uint32_t)a1, (uint32_t)a2, (uint32_t)a3, (uint32_t)t0, (uint32_t)t1, t2, (uint32_t)sp, vf0, vf4, vf5, vf6, vf10, vf11, vf12, vf13, vf14, vf15, vf16, vf17, vf24, vf25, vf26, vf27, vacc, vq, vstat, h2); }
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)s0, 20))));
    goto L_14620c0;
L_1462098:;
    (s2 = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)326 << 16))));
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)s0, 4))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)s2, 29700))));
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)SLL32((uint32_t)a0, 4))));
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)a0, (uint32_t)v1))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)SLL32((uint32_t)v0, 4))));
    do { const a4::q128 _v = (A4_R128(ADD32((uint32_t)a0, 0))); h3 = a4::hi64(_v); std::memcpy(&v1, &_v, 8); } while (0);
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v0, (uint32_t)s1))));
    A4_W128(ADD32((uint32_t)v0, 320), a4::mk128(h3, v1));
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)s0, 20))));
L_14620c0:;
    (a2 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)s2, 29700))));
    (a1 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)s0, 8))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)a2, 0))));
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)SLL32((uint32_t)v1, 6))));
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v1, (uint32_t)a1))));
    do { const a4::q128 _v = (A4_R128(ADD32((uint32_t)v1, 48))); h4 = a4::hi64(_v); std::memcpy(&a0, &_v, 8); } while (0);
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)SLL32((uint32_t)v0, 4))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v0, (uint32_t)s1))));
    A4_W128(ADD32((uint32_t)v0, 640), a4::mk128(h4, a0));
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)s0, 20))));
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)s0, 12))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)a2, 0))));
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)SLL32((uint32_t)v1, 4))));
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v1, (uint32_t)a0))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)SLL32((uint32_t)v0, 4))));
    do { const a4::q128 _v = (A4_R128(ADD32((uint32_t)v1, 0))); h4 = a4::hi64(_v); std::memcpy(&a0, &_v, 8); } while (0);
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v0, (uint32_t)s1))));
    A4_W128(ADD32((uint32_t)v0, 0), a4::mk128(h4, a0));
    goto L_1462180;
L_1462110:;
    (s2 = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)326 << 16))));
    (a1 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)s0, 8))));
    (a2 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)s2, 29700))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)a2, 0))));
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)SLL32((uint32_t)v1, 6))));
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v1, (uint32_t)a1))));
    do { const a4::q128 _v = (A4_R128(ADD32((uint32_t)v1, 48))); h4 = a4::hi64(_v); std::memcpy(&a0, &_v, 8); } while (0);
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)SLL32((uint32_t)v0, 4))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v0, (uint32_t)s1))));
    A4_W128(ADD32((uint32_t)v0, 640), a4::mk128(h4, a0));
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)s0, 20))));
    (a1 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)s0, 12))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)a2, 0))));
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)SLL32((uint32_t)v1, 4))));
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v1, (uint32_t)a1))));
    do { const a4::q128 _v = (A4_R128(ADD32((uint32_t)v1, 0))); h4 = a4::hi64(_v); std::memcpy(&a0, &_v, 8); } while (0);
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)SLL32((uint32_t)v0, 4))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v0, (uint32_t)s1))));
    A4_W128(ADD32((uint32_t)v0, 0), a4::mk128(h4, a0));
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)s0, 20))));
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)s0, 4))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)a2, 0))));
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)SLL32((uint32_t)v1, 4))));
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v1, (uint32_t)a0))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)SLL32((uint32_t)v0, 4))));
    do { const a4::q128 _v = (A4_R128(ADD32((uint32_t)v1, 0))); h4 = a4::hi64(_v); std::memcpy(&a0, &_v, 8); } while (0);
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v0, (uint32_t)s1))));
    A4_W128(ADD32((uint32_t)v0, 320), a4::mk128(h4, a0));
L_1462180:;
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)s2, 29700))));
L_1462184:;
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)v1, 0))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v0, 1))));
    { uint32_t bits = A1_R32B((uint32_t)sp, 216); float f; std::memcpy(&f, &bits, sizeof(f)); f20 = f; }
    A1_W32B((uint32_t)v1, 0, (uint32_t)v0);
    { (sp = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)sp, 224)))); io_vf4 = vf4; io_vf5 = vf5; io_vf6 = vf6; io_vf7 = vf7; io_vf8 = vf8; io_vf9 = vf9; io_vf10 = vf10; io_vf11 = vf11; io_vf12 = vf12; io_vf13 = vf13; io_vf14 = vf14; io_vf15 = vf15; io_vf16 = vf16; io_vf17 = vf17; io_vf24 = vf24; io_vf25 = vf25; io_vf26 = vf26; io_vf27 = vf27; io_vf28 = vf28; io_vf29 = vf29; io_vacc = vacc; io_vq = vq; io_vstat = vstat; io_h2 = h2; io_h3 = h3; io_h4 = h4; io_h5 = h5; io_h8 = h8; io_h9 = h9; io_h10 = h10; io_h11 = h11; io_h12 = h12; io_h13 = h13; io_h14 = h14; io_h15 = h15; return v0; }
    __builtin_unreachable();
}

// charrootdyn_getCharaFront_PlaneTopDifferenceXZAngle (0x10de130): typed by tools/typed_lift2.py (round A6; guest frame calls; VU0 in vf0 vf5 vf6 vf7 vf8 vf10 vf12 vf13 vf14 vf15 vf16 vf17 vf25 vf28 vf29 vacc vstat, out vf4 vf5 vf6 vf7 vf8 vf10 vf11 vf12 vf13 vf14 vf15 vf16 vf17 vf24 vf25 vf26 vf28 vf29 vacc vq vstat h2 h3 h8 h9 h10 h11 h12 h13 h14 h15, 64-bit inputs a1)
inline a1::RetVF charrootdyn_getCharaFront_PlaneTopDifferenceXZAngle(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint64_t a0, uint64_t w64_a1, uint64_t sp, float f0, float f12, float f13, float f14, float f15, float f16, float f17, float f18, float f19, const a4::vec4 &io_vf0, a4::vec4 &io_vf4, a4::vec4 &io_vf5, a4::vec4 &io_vf6, a4::vec4 &io_vf7, a4::vec4 &io_vf8, a4::vec4 &io_vf10, a4::vec4 &io_vf11, a4::vec4 &io_vf12, a4::vec4 &io_vf13, a4::vec4 &io_vf14, a4::vec4 &io_vf15, a4::vec4 &io_vf16, a4::vec4 &io_vf17, a4::vec4 &io_vf24, a4::vec4 &io_vf25, a4::vec4 &io_vf26, a4::vec4 &io_vf28, a4::vec4 &io_vf29, a4::vec4 &io_vacc, float &io_vq, uint16_t &io_vstat, uint64_t &io_h2, uint64_t &io_h3, uint64_t &io_h8, uint64_t &io_h9, uint64_t &io_h10, uint64_t &io_h11, uint64_t &io_h12, uint64_t &io_h13, uint64_t &io_h14, uint64_t &io_h15)
{
    uint64_t v0 = 0;
    uint32_t a1 = (uint32_t)w64_a1;
    uint64_t s0 = 0;
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
    a4::vec4 vf24 = io_vf24;
    a4::vec4 vf25 = io_vf25;
    a4::vec4 vf26 = io_vf26;
    a4::vec4 vf28 = io_vf28;
    a4::vec4 vf29 = io_vf29;
    a4::vec4 vacc = io_vacc;
    float vq = io_vq;
    uint16_t vstat = io_vstat;
    uint64_t h2 = io_h2;
    uint64_t h3 = io_h3;
    uint64_t h8 = io_h8;
    uint64_t h9 = io_h9;
    uint64_t h10 = io_h10;
    uint64_t h11 = io_h11;
    uint64_t h12 = io_h12;
    uint64_t h13 = io_h13;
    uint64_t h14 = io_h14;
    uint64_t h15 = io_h15;
    (sp = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)sp, 4294967248))));
    (s0 = (uint64_t)((uint64_t)a0 + (uint64_t)0ULL));
    { (a0 = (uint64_t)((uint64_t)RC_U64X(sp) + (uint64_t)0ULL)); A3_CHARGE(); { const a1::RetVF a2_r = GetPlaneZTopYPlaneCoordQuaternion(ram, rt, a0, w64_a1, sp, f12, f13, f14, f15, f16, f17, f18, f19, vf0, vf5, vf6, vf7, vacc, vq, vstat, h3); v0 = a2_r.v0; f0 = a2_r.f0; } }
    (a1 = (uint32_t)((int32_t)((int32_t)ADD32((uint32_t)sp, 16))));
    vf4 = a4v::castsi128_ps(A4_R128(ADD32((uint32_t)s0, 0)));
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
    A4_W128(ADD32(a1, 0), a4v::castps_si128(vf6));
    { (a0 = (uint64_t)((uint64_t)sp + (uint64_t)0ULL)); A3_CHARGE(); { const a1::RetVF a2_r = GetTurnYAngleFromGlobalDir(ram, rt, a0, a1_sx64(a1), sp, f0, f14, f15, f16, f17, f18, f19, vf0, vf5, vf7, vf8, vf10, vf11, vf12, vf13, vf14, vf15, vf16, vf17, vf24, vf25, vf26, vf28, vf29, vacc, vq, vstat, h2, h8, h9, h10, h11, h12, h13, h14, h15); v0 = a2_r.v0; f0 = a2_r.f0; } }
    { (sp = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)sp, 48)))); io_vf4 = vf4; io_vf5 = vf5; io_vf6 = vf6; io_vf7 = vf7; io_vf8 = vf8; io_vf10 = vf10; io_vf11 = vf11; io_vf12 = vf12; io_vf13 = vf13; io_vf14 = vf14; io_vf15 = vf15; io_vf16 = vf16; io_vf17 = vf17; io_vf24 = vf24; io_vf25 = vf25; io_vf26 = vf26; io_vf28 = vf28; io_vf29 = vf29; io_vacc = vacc; io_vq = vq; io_vstat = vstat; io_h2 = h2; io_h3 = h3; io_h8 = h8; io_h9 = h9; io_h10 = h10; io_h11 = h11; io_h12 = h12; io_h13 = h13; io_h14 = h14; io_h15 = h15; return a1::RetVF{(uint64_t)v0, f0}; }
    __builtin_unreachable();
}

// IsInsideOfRegion (0x1135dd8): typed by tools/typed_lift2.py (round A6; guest frame calls, extra outputs xo[0]=f0 xo[1]=f1 xo[2]=f2 xo[3]=f12 xo[4]=f20 xo[5]=a2 xo[6]=t0 xo[7]=t1 xo[8]=t2 xo[9]=t3; VU0 in vf0 vf2 vf6 vf7 vf9 vf10 vf12 vf13 vf14 vf15 vf16 vf17 vacc, out vf1 vf2 vf3 vf4 vf5 vf6 vf7 vf8 vf9 vf10 vf11 vf12 vf13 vf14 vf15 vf16 vf17 vf24 vf25 vf26 vf27 vacc h2 h6 h7 h8 h9 h10 h11 h12 h13 h14 h15, relocated calls 6, break)
inline uint64_t IsInsideOfRegion(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint64_t a0, uint64_t a1, uint64_t a2, uint32_t a3, uint32_t sp, float f0, float f1, float f2, float f3, float f4, float f5, float f6, float f7, float f8, float f9, float f10, float f11, float f12, float f13, float f14, float f15, float f16, float f17, float f18, float f19, float f20, float f21, float f22, float f23, float f24, float f25, float f26, float f27, float f28, float f29, float f30, float f31, const a4::vec4 &io_vf0, a4::vec4 &io_vf1, a4::vec4 &io_vf2, a4::vec4 &io_vf3, a4::vec4 &io_vf4, a4::vec4 &io_vf5, a4::vec4 &io_vf6, a4::vec4 &io_vf7, a4::vec4 &io_vf8, a4::vec4 &io_vf9, a4::vec4 &io_vf10, a4::vec4 &io_vf11, a4::vec4 &io_vf12, a4::vec4 &io_vf13, a4::vec4 &io_vf14, a4::vec4 &io_vf15, a4::vec4 &io_vf16, a4::vec4 &io_vf17, a4::vec4 &io_vf24, a4::vec4 &io_vf25, a4::vec4 &io_vf26, a4::vec4 &io_vf27, a4::vec4 &io_vacc, uint64_t &io_h2, uint64_t &io_h6, uint64_t &io_h7, uint64_t &io_h8, uint64_t &io_h9, uint64_t &io_h10, uint64_t &io_h11, uint64_t &io_h12, uint64_t &io_h13, uint64_t &io_h14, uint64_t &io_h15, uint64_t *xo = nullptr)
{
    uint32_t at = 0;
    uint64_t v0 = 0;
    uint64_t v1 = 0;
    uint64_t t0 = 0;
    uint64_t t1 = 0;
    uint64_t t2 = 0;
    uint64_t t3 = 0;
    uint64_t s0 = 0;
    uint64_t s1 = 0;
    uint64_t s2 = 0;
    uint32_t fcr31 = 0;
    a4::vec4 vf0 = io_vf0;
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
    a4::vec4 vf24 = io_vf24;
    a4::vec4 vf25 = io_vf25;
    a4::vec4 vf26 = io_vf26;
    a4::vec4 vf27 = io_vf27;
    a4::vec4 vacc = io_vacc;
    uint64_t h2 = io_h2;
    uint64_t h6 = io_h6;
    uint64_t h7 = io_h7;
    uint64_t h8 = io_h8;
    uint64_t h9 = io_h9;
    uint64_t h10 = io_h10;
    uint64_t h11 = io_h11;
    uint64_t h12 = io_h12;
    uint64_t h13 = io_h13;
    uint64_t h14 = io_h14;
    uint64_t h15 = io_h15;
    bool a3xw0 = false;
    bool a3xw1 = false;
    bool a3xw2 = false;
    bool a3xw3 = false;
    bool a3xw5 = false;
    bool a3xw6 = false;
    bool a3xw7 = false;
    bool a3xw8 = false;
    bool a3xw9 = false;
    sp = sp - 0x140u;
    (s0 = (uint64_t)((uint64_t)a0 + (uint64_t)0ULL));
    (s1 = (uint64_t)((uint64_t)a1 + (uint64_t)0ULL));
    (s2 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL));
    { const bool c = (int64_t)s0 != 0; A1_W32B(sp, 304, a1_bits(f20)); if (c) goto L_1135e38; }
    vf5 = a4v::castsi128_ps(A4_R128(ADD32((uint32_t)s1, 0)));
    vf6 = a4v::castsi128_ps(A4_R128(ADD32((uint32_t)a2, 0)));
    { a4::vec4 res = a4v::sub(vf5, vf6); a4::q128 mask = a4v::set_epi32(-1, -1, -1, -1); vf7 = a4v::blend(vf7, res, a4v::castsi128_ps(mask)); }
    { a4::vec4 res = a4v::mulK<1, 1>(vf7, vf7); a4::q128 mask = a4v::set_epi32(0, -1, -1, -1); vf7 = a4v::blend(vf7, res, a4v::castsi128_ps(mask)); }
    { a4::vec4 res = a4v::addK<1, 1>(vf7, a4v::bcast<1>(vf7)); a4::q128 mask = a4v::set_epi32(0, 0, 0, -1); vf7 = a4v::blendv_ps(vf7, res, a4v::castsi128_ps(mask)); }
    { a4::vec4 res = a4v::addK<1, 1>(vf7, a4v::bcast<2>(vf7)); a4::q128 mask = a4v::set_epi32(0, 0, 0, -1); vf7 = a4v::blendv_ps(vf7, res, a4v::castsi128_ps(mask)); }
    do { const a4::q128 _v = (a4v::castps_si128(vf7)); h2 = a4::hi64(_v); std::memcpy(&v0, &_v, 8); } while (0);
    { uint32_t bits = (uint32_t)v0; std::memcpy(&f1, &bits, sizeof(bits)); } a3xw1 = true;
    f0 = a1f::mul(f12, f12); a3xw0 = true;
    fcr31 = (a1f::le(f1, f0)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
    if ((fcr31 & 0x800000u) != 0u) { (s2 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 1)))); goto L_1136220; }
    (v0 = (uint64_t)((uint64_t)RC_U64X(s2) + (uint64_t)0ULL));
    goto L_1136224;
L_1135e38:;
    (v0 = (uint64_t)((uint64_t)a1_sx64(sp) + (uint64_t)0ULL));
    { a4::vec4 res = a4v::addK<1, 1>(vf0, a4v::bcast<3>(vf0)); a4::q128 mask = a4v::set_epi32(0, -1, -1, -1); vf12 = a4v::blendv_ps(vf12, res, a4v::castsi128_ps(mask)); }
    vf11 = a4v::castsi128_ps(A4_R128(ADD32(a3, 0)));
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
    A4_W128(ADD32((uint32_t)v0, 0), a4v::castps_si128(vf14));
    A4_W128(ADD32((uint32_t)v0, 16), a4v::castps_si128(vf15));
    A4_W128(ADD32((uint32_t)v0, 32), a4v::castps_si128(vf16));
    vf4 = a4v::castsi128_ps(A4_R128(ADD32((uint32_t)a2, 0)));
    { a4::q128 mask = a4v::set_epi32(-1, 0, 0, 0); vf4 = a4v::blendv_ps(vf4, vf0, a4v::castsi128_ps(mask)); }
    A4_W128(ADD32((uint32_t)v0, 48), a4v::castps_si128(vf4));
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 1))));
    if ((int64_t)s0 != (int64_t)v1) { (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 2)))); goto L_1135fb0; }
    at = 0x3F800000u;
    { uint32_t bits = a7_opq(at); std::memcpy(&f20, &bits, sizeof(bits)); }
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(sp, 80))));
    (a1 = (uint64_t)((uint64_t)a1_sx64(sp) + (uint64_t)0ULL));
    A1_W32B(sp, 64, a1_bits(f12));
    A1_W32B(sp, 68, a1_bits(f13));
    A1_W32B(sp, 72, a1_bits(f14));
    { A1_W32B(sp, 76, a1_bits(f20)); A3_CHARGE(); uint64_t a7c[4] = {0u, 0u, 0u, 0u}; iosInverseMatrix(ram, rt, a0, a1, vf1, vf2, vf3, vf4, vf8, vf9, vacc, h8, h9, h10, h11, h12, h13, h14, h15, a7c); t0 = a7c[0]; t1 = a7c[1]; t2 = a7c[2]; t3 = a7c[3]; a3xw6 = true; a3xw7 = true; a3xw8 = true; a3xw9 = true; }
    f0 = a1_flt(A1_R32B(sp, 64)); a3xw0 = true;
    (a1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(sp, 80))));
    f1 = a1_flt(A1_R32B(sp, 68)); a3xw1 = true;
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(sp, 144))));
    { uint32_t bits = A1_R32B(sp, 72); float f; std::memcpy(&f, &bits, sizeof(f)); f2 = f; } a3xw2 = true;
    f0 = a1f::div(fcr31, f20, f0); a3xw0 = true;
    A1_W32B(sp, 188, 0u);
    f1 = a1f::div(fcr31, f20, f1); a3xw1 = true;
    A1_W32B(sp, 176, a1_bits(f0));
    f2 = a1f::div(fcr31, f20, f2); a3xw2 = true;
    A1_W32B(sp, 180, a1_bits(f1));
    (v0 = (uint64_t)(A1_R64B(sp, 176)));
    A1_W64B(sp, 160, v0);
    { float f = f2; uint32_t bits; std::memcpy(&bits, &f, sizeof(bits)); A1_W32B(sp, 184, bits); }
    (v0 = (uint64_t)(A1_R64B(sp, 184)));
    A1_W64B(sp, 168, v0);
    vf8 = a4v::castsi128_ps(A4_R128(ADD32((uint32_t)s1, 0)));
    vf14 = a4v::castsi128_ps(A4_R128(ADD32((uint32_t)a1, 0)));
    vf15 = a4v::castsi128_ps(A4_R128(ADD32((uint32_t)a1, 16)));
    vf16 = a4v::castsi128_ps(A4_R128(ADD32((uint32_t)a1, 32)));
    vf17 = a4v::castsi128_ps(A4_R128(ADD32((uint32_t)a1, 48)));
    { a4::vec4 res = a4v::mul(vf14, a4v::bcast<0>(vf8)); vacc = a4v::dest<1, 1, 1, 1>(vacc, res); }
    { a4::vec4 mul_res = a4v::mul(vf15, a4v::bcast<1>(vf8)); a4::vec4 res = a4v::addK<1, 1>(vacc, mul_res); vacc = a4v::dest<1, 1, 1, 1>(vacc, res); }
    { a4::vec4 mul_res = a4v::mul(vf16, a4v::bcast<2>(vf8)); a4::vec4 res = a4v::addK<1, 1>(vacc, mul_res); vacc = a4v::dest<1, 1, 1, 1>(vacc, res); }
    { a4::vec4 mul_res = a4v::mul(vf17, a4v::bcast<3>(vf8)); a4::vec4 res = a4v::addK<1, 1>(vacc, mul_res); a4::q128 mask = a4v::set_epi32(-1, -1, -1, -1); vf10 = a4v::blendv_ps(vf10, res, a4v::castsi128_ps(mask)); }
    A4_W128(ADD32((uint32_t)a0, 0), a4v::castps_si128(vf10));
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(sp, 160))));
    vf5 = a4v::castsi128_ps(A4_R128(ADD32((uint32_t)a0, 0)));
    vf6 = a4v::castsi128_ps(A4_R128(ADD32((uint32_t)v1, 0)));
    { a4::vec4 res = a4v::mul(vf5, vf6); a4::q128 mask = a4v::set_epi32(0, -1, -1, -1); vf5 = a4v::blend(vf5, res, a4v::castsi128_ps(mask)); }
    A4_W128(ADD32((uint32_t)a0, 0), a4v::castps_si128(vf5));
    vf7 = a4v::castsi128_ps(A4_R128(ADD32((uint32_t)a0, 0)));
    { a4::vec4 res = a4v::mul(vf7, vf7); a4::q128 mask = a4v::set_epi32(0, -1, -1, -1); vf7 = a4v::blend(vf7, res, a4v::castsi128_ps(mask)); }
    { a4::vec4 res = a4v::addK<1, 1>(vf7, a4v::bcast<1>(vf7)); a4::q128 mask = a4v::set_epi32(0, 0, 0, -1); vf7 = a4v::blendv_ps(vf7, res, a4v::castsi128_ps(mask)); }
    { a4::vec4 res = a4v::addK<1, 1>(vf7, a4v::bcast<2>(vf7)); a4::q128 mask = a4v::set_epi32(0, 0, 0, -1); vf7 = a4v::blendv_ps(vf7, res, a4v::castsi128_ps(mask)); }
    do { const a4::q128 _v = (a4v::castps_si128(vf7)); h2 = a4::hi64(_v); std::memcpy(&v0, &_v, 8); } while (0);
    { uint32_t bits = (uint32_t)v0; std::memcpy(&f0, &bits, sizeof(bits)); } a3xw0 = true;
    fcr31 = (a1f::le(f0, f20)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
    { const bool c = (fcr31 & 0x800000u) != 0u; (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 1)))); if (c) goto L_1135fa0; }
    (v0 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL));
L_1135fa0:;
    if ((int64_t)v0 == (int64_t)s0) { (s2 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 1)))); goto L_1136220; }
    (v0 = (uint64_t)((uint64_t)s2 + (uint64_t)0ULL));
    goto L_1136224;
L_1135fb0:;
    if ((int64_t)s0 != (int64_t)v0) { (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 3)))); goto L_1136150; }
    at = 0x3F800000u;
    { uint32_t bits = a7_opq(at); std::memcpy(&f0, &bits, sizeof(bits)); } a3xw0 = true;
    f1 = (-(float)(f14)); a3xw1 = true;
    A1_W32B(sp, 80, a1_bits(f13));
    fcr31 = (a1f::le(f12, f0)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
    A1_W32B(sp, 88, a1_bits(f14));
    A1_W32B(sp, 92, a1_bits(f0));
    { const bool c = (fcr31 & 0x800000u) == 0u; A1_W32B(sp, 84, a1_bits(f1)); if (c) goto L_1136000; }
    (s0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(sp, 96))));
    at = 0x3FC90000u;
    at = at | 0xFDAu;
    { uint32_t bits = a7_opq(at); std::memcpy(&f12, &bits, sizeof(bits)); } a3xw3 = true;
    { (a0 = (uint64_t)((uint64_t)RC_U64X(s0) + (uint64_t)0ULL)); A3_CHARGE(); uint64_t a7c[5] = {0u, 0u, 0u, 0u, 0u}; { const a1::RetVF a2_r = iosRotMatrixZ(ram, rt, a0, sp, f12, f13, f14, f15, f16, f17, f18, f19, h6, h7, h8, h9, a7c); v0 = a2_r.v0; f0 = a2_r.f0; } f2 = a1_flt((uint32_t)a7c[0]); a2 = a7c[1]; a3xw0 = true; a3xw2 = true; a3xw5 = true; }
    goto L_113603c;
L_1136000:;
    (at = (uint32_t)((int32_t)((int32_t)((uint32_t)16384 << 16))));
    { uint32_t bits = a7_opq(at); std::memcpy(&f0, &bits, sizeof(bits)); } a3xw0 = true;
    fcr31 = (a1f::le(f12, f0)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
    { const bool c = (fcr31 & 0x800000u) != 0u; (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(sp, 192)))); if (c) goto L_11360b0; }
    (at = (uint32_t)((int32_t)((int32_t)((uint32_t)16448 << 16))));
    { uint32_t bits = a7_opq(at); std::memcpy(&f0, &bits, sizeof(bits)); } a3xw0 = true;
    fcr31 = (a1f::le(f12, f0)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
    { const bool c = (fcr31 & 0x800000u) == 0u; (s0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(sp, 96)))); if (c) goto L_11360b0; }
    at = 0x3FC90000u;
    at = at | 0xFDAu;
    { uint32_t bits = a7_opq(at); std::memcpy(&f12, &bits, sizeof(bits)); } a3xw3 = true;
    { (a0 = (uint64_t)((uint64_t)RC_U64X(s0) + (uint64_t)0ULL)); A3_CHARGE(); uint64_t a7c[6] = {0u, 0u, 0u, 0u, 0u, 0u}; { const a1::RetVF a2_r = iosRotMatrixY(ram, rt, a0, sp, f12, f13, f14, f15, f16, f17, f18, f19, h6, h7, h8, h9, a7c); v0 = a2_r.v0; f0 = a2_r.f0; } f2 = a1_flt((uint32_t)a7c[0]); a2 = a7c[2]; a3xw0 = true; a3xw2 = true; a3xw5 = true; }
L_113603c:;
    vf14 = a4v::castsi128_ps(A4_R128(ADD32(sp, 0)));
    vf15 = a4v::castsi128_ps(A4_R128(ADD32(sp, 16)));
    vf16 = a4v::castsi128_ps(A4_R128(ADD32(sp, 32)));
    vf17 = a4v::castsi128_ps(A4_R128(ADD32(sp, 48)));
    vf24 = a4v::castsi128_ps(A4_R128(ADD32((uint32_t)s0, 0)));
    vf25 = a4v::castsi128_ps(A4_R128(ADD32((uint32_t)s0, 16)));
    vf26 = a4v::castsi128_ps(A4_R128(ADD32((uint32_t)s0, 32)));
    vf27 = a4v::castsi128_ps(A4_R128(ADD32((uint32_t)s0, 48)));
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
    A4_W128(ADD32(sp, 0), a4v::castps_si128(vf24));
    A4_W128(ADD32(sp, 16), a4v::castps_si128(vf25));
    A4_W128(ADD32(sp, 32), a4v::castps_si128(vf26));
    A4_W128(ADD32(sp, 48), a4v::castps_si128(vf27));
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(sp, 192))));
L_11360b0:;
    { (a1 = (uint64_t)((uint64_t)a1_sx64(sp) + (uint64_t)0ULL)); A3_CHARGE(); uint64_t a7c[4] = {0u, 0u, 0u, 0u}; iosInverseMatrix(ram, rt, a0, a1, vf1, vf2, vf3, vf4, vf8, vf9, vacc, h8, h9, h10, h11, h12, h13, h14, h15, a7c); t0 = a7c[0]; t1 = a7c[1]; t2 = a7c[2]; t3 = a7c[3]; a3xw6 = true; a3xw7 = true; a3xw8 = true; a3xw9 = true; }
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(sp, 192))));
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(sp, 256))));
    vf8 = a4v::castsi128_ps(A4_R128(ADD32((uint32_t)s1, 0)));
    vf14 = a4v::castsi128_ps(A4_R128(ADD32((uint32_t)v0, 0)));
    vf15 = a4v::castsi128_ps(A4_R128(ADD32((uint32_t)v0, 16)));
    vf16 = a4v::castsi128_ps(A4_R128(ADD32((uint32_t)v0, 32)));
    vf17 = a4v::castsi128_ps(A4_R128(ADD32((uint32_t)v0, 48)));
    { a4::vec4 res = a4v::mul(vf14, a4v::bcast<0>(vf8)); vacc = a4v::dest<1, 1, 1, 1>(vacc, res); }
    { a4::vec4 mul_res = a4v::mul(vf15, a4v::bcast<1>(vf8)); a4::vec4 res = a4v::addK<1, 1>(vacc, mul_res); vacc = a4v::dest<1, 1, 1, 1>(vacc, res); }
    { a4::vec4 mul_res = a4v::mul(vf16, a4v::bcast<2>(vf8)); a4::vec4 res = a4v::addK<1, 1>(vacc, mul_res); vacc = a4v::dest<1, 1, 1, 1>(vacc, res); }
    { a4::vec4 mul_res = a4v::mul(vf17, a4v::bcast<3>(vf8)); a4::vec4 res = a4v::addK<1, 1>(vacc, mul_res); a4::q128 mask = a4v::set_epi32(-1, -1, -1, -1); vf10 = a4v::blendv_ps(vf10, res, a4v::castsi128_ps(mask)); }
    A4_W128(ADD32((uint32_t)v1, 0), a4v::castps_si128(vf10));
    f0 = a1_flt(A1_R32B(sp, 84)); a3xw0 = true;
    f1 = a1_flt(A1_R32B(sp, 260)); a3xw1 = true;
    fcr31 = (a1f::le(f0, f1)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
    { const bool c = (fcr31 & 0x800000u) == 0u; (a1 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL)); if (c) goto L_113610c; }
    f0 = a1_flt(A1_R32B(sp, 88)); a3xw0 = true;
    fcr31 = (a1f::le(f1, f0)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
    if ((fcr31 & 0x800000u) != 0u) { (a1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 1)))); goto L_113610c; }
L_113610c:;
    (a0 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL));
    vf7 = a4v::castsi128_ps(A4_R128(ADD32((uint32_t)v1, 0)));
    { a4::vec4 res = a4v::mul(vf7, vf7); a4::q128 mask = a4v::set_epi32(0, -1, 0, -1); vf7 = a4v::blend(vf7, res, a4v::castsi128_ps(mask)); }
    { a4::vec4 res = a4v::addK<1, 1>(vf7, a4v::bcast<2>(vf7)); a4::q128 mask = a4v::set_epi32(0, 0, 0, -1); vf7 = a4v::blendv_ps(vf7, res, a4v::castsi128_ps(mask)); }
    do { const a4::q128 _v = (a4v::castps_si128(vf7)); h2 = a4::hi64(_v); std::memcpy(&v0, &_v, 8); } while (0);
    { uint32_t bits = (uint32_t)v0; std::memcpy(&f1, &bits, sizeof(bits)); } a3xw1 = true;
    f0 = a1_flt(A1_R32B(sp, 80)); a3xw0 = true;
    f0 = a1f::mul(f0, f0); a3xw0 = true;
    fcr31 = (a1f::lt(f1, f0)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
    if ((fcr31 & 0x800000u) != 0u) { (a0 = (uint64_t)(((uint64_t)0ULL < (uint64_t)RC_U64X(a1)) ? 1 : 0)); goto L_1136138; }
L_1136138:;
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 1))));
    if ((int64_t)a0 == (int64_t)v0) { (s2 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 1)))); goto L_1136220; }
    (v0 = (uint64_t)((uint64_t)s2 + (uint64_t)0ULL));
    goto L_1136224;
L_1136150:;
    { const bool c = (int64_t)s0 != (int64_t)v0; (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 4)))); if (c) goto L_1136208; }
    at = 0x3F800000u;
    { uint32_t bits = a7_opq(at); std::memcpy(&f0, &bits, sizeof(bits)); } a3xw0 = true;
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(sp, 176))));
    (a1 = (uint64_t)((uint64_t)a1_sx64(sp) + (uint64_t)0ULL));
    A1_W32B(sp, 160, a1_bits(f12));
    A1_W32B(sp, 164, a1_bits(f13));
    A1_W32B(sp, 168, a1_bits(f14));
    { A1_W32B(sp, 172, a1_bits(f0)); A3_CHARGE(); uint64_t a7c[4] = {0u, 0u, 0u, 0u}; iosInverseMatrix(ram, rt, a0, a1, vf1, vf2, vf3, vf4, vf8, vf9, vacc, h8, h9, h10, h11, h12, h13, h14, h15, a7c); t0 = a7c[0]; t1 = a7c[1]; t2 = a7c[2]; t3 = a7c[3]; a3xw6 = true; a3xw7 = true; a3xw8 = true; a3xw9 = true; }
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(sp, 176))));
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(sp, 240))));
    vf8 = a4v::castsi128_ps(A4_R128(ADD32((uint32_t)s1, 0)));
    vf14 = a4v::castsi128_ps(A4_R128(ADD32((uint32_t)v0, 0)));
    vf15 = a4v::castsi128_ps(A4_R128(ADD32((uint32_t)v0, 16)));
    vf16 = a4v::castsi128_ps(A4_R128(ADD32((uint32_t)v0, 32)));
    vf17 = a4v::castsi128_ps(A4_R128(ADD32((uint32_t)v0, 48)));
    { a4::vec4 res = a4v::mul(vf14, a4v::bcast<0>(vf8)); vacc = a4v::dest<1, 1, 1, 1>(vacc, res); }
    { a4::vec4 mul_res = a4v::mul(vf15, a4v::bcast<1>(vf8)); a4::vec4 res = a4v::addK<1, 1>(vacc, mul_res); vacc = a4v::dest<1, 1, 1, 1>(vacc, res); }
    { a4::vec4 mul_res = a4v::mul(vf16, a4v::bcast<2>(vf8)); a4::vec4 res = a4v::addK<1, 1>(vacc, mul_res); vacc = a4v::dest<1, 1, 1, 1>(vacc, res); }
    { a4::vec4 mul_res = a4v::mul(vf17, a4v::bcast<3>(vf8)); a4::vec4 res = a4v::addK<1, 1>(vacc, mul_res); a4::q128 mask = a4v::set_epi32(-1, -1, -1, -1); vf10 = a4v::blendv_ps(vf10, res, a4v::castsi128_ps(mask)); }
    A4_W128(ADD32((uint32_t)a0, 0), a4v::castps_si128(vf10));
    { (a1 = (uint64_t)((uint64_t)RC_U64X(a0) + (uint64_t)0ULL)); A3_CHARGE(); iosAbsVector(ram, rt, (uint32_t)a0, (uint32_t)a1, vf5, vf6); }
    f1 = a1_flt(A1_R32B(sp, 240)); a3xw1 = true;
    f0 = a1_flt(A1_R32B(sp, 160)); a3xw0 = true;
    fcr31 = (a1f::le(f1, f0)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
    { const bool c = (fcr31 & 0x800000u) == 0u; (v1 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL)); if (c) goto L_11361f0; }
    f1 = a1_flt(A1_R32B(sp, 244)); a3xw1 = true;
    f0 = a1_flt(A1_R32B(sp, 164)); a3xw0 = true;
    fcr31 = (a1f::le(f1, f0)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
    { const bool c = (fcr31 & 0x800000u) == 0u; (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 1)))); if (c) goto L_11361f4; }
    f1 = a1_flt(A1_R32B(sp, 248)); a3xw1 = true;
    f0 = a1_flt(A1_R32B(sp, 168)); a3xw0 = true;
    fcr31 = (a1f::le(f1, f0)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
    if ((fcr31 & 0x800000u) != 0u) { (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 1)))); goto L_11361f4; }
L_11361f0:;
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 1))));
L_11361f4:;
    if ((int64_t)v1 == (int64_t)v0) { (s2 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 1)))); goto L_1136220; }
    (v0 = (uint64_t)((uint64_t)s2 + (uint64_t)0ULL));
    goto L_1136224;
L_1136208:;
    { const bool c = (int64_t)s0 == (int64_t)v1; (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 1)))); if (c) goto L_1136224; }
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 5))));
    { const bool c = (int64_t)s0 == (int64_t)v1; (v0 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL)); if (c) goto L_1136224; }
    A6_BREAK(0x113621Cu);
L_1136220:;
    (v0 = (uint64_t)((uint64_t)s2 + (uint64_t)0ULL));
L_1136224:;
    f20 = a1_flt(A1_R32B(sp, 304));
    { sp = sp + 0x140u; if (xo) { if (a3xw0) xo[0] = a1_bits(f0); if (a3xw1) xo[1] = a1_bits(f1); if (a3xw2) xo[2] = a1_bits(f2); if (a3xw3) xo[3] = a1_bits(f12); xo[4] = a1_bits(f20); if (a3xw5) xo[5] = a2; if (a3xw6) xo[6] = t0; if (a3xw7) xo[7] = t1; if (a3xw8) xo[8] = t2; if (a3xw9) xo[9] = t3; } io_vf1 = vf1; io_vf2 = vf2; io_vf3 = vf3; io_vf4 = vf4; io_vf5 = vf5; io_vf6 = vf6; io_vf7 = vf7; io_vf8 = vf8; io_vf9 = vf9; io_vf10 = vf10; io_vf11 = vf11; io_vf12 = vf12; io_vf13 = vf13; io_vf14 = vf14; io_vf15 = vf15; io_vf16 = vf16; io_vf17 = vf17; io_vf24 = vf24; io_vf25 = vf25; io_vf26 = vf26; io_vf27 = vf27; io_vacc = vacc; io_h2 = h2; io_h6 = h6; io_h7 = h7; io_h8 = h8; io_h9 = h9; io_h10 = h10; io_h11 = h11; io_h12 = h12; io_h13 = h13; io_h14 = h14; io_h15 = h15; return v0; }
    __builtin_unreachable();
}

// SCRGetTargetObjXZDirOnMyCtrlGeo (0x137c518): typed by tools/typed_lift2.py (round A6; guest frame calls; VU0 in vf0 vf7 vf10 vf12 vf13 vf14 vf15 vf16 vf17 vf25 vf28 vf29 vacc vstat h2 h5, out vf5 vf6 vf7 vf8 vf10 vf11 vf12 vf13 vf14 vf15 vf16 vf17 vf24 vf25 vf26 vf28 vf29 vacc vq vstat h2 h4 h8 h9 h10 h11 h12 h13 h14 h15, relocated calls 2)
inline a1::RetVF SCRGetTargetObjXZDirOnMyCtrlGeo(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint64_t v0, uint64_t a0, uint64_t a1, uint64_t a2, uint64_t sp, float f0, float f1, float f2, float f3, float f4, float f5, float f6, float f7, float f8, float f9, float f10, float f11, float f12, float f13, float f14, float f15, float f16, float f17, float f18, float f19, float f20, float f21, float f22, float f23, float f24, float f25, float f26, float f27, float f28, float f29, float f30, float f31, const a4::vec4 &io_vf0, a4::vec4 &io_vf5, a4::vec4 &io_vf6, a4::vec4 &io_vf7, a4::vec4 &io_vf8, a4::vec4 &io_vf10, a4::vec4 &io_vf11, a4::vec4 &io_vf12, a4::vec4 &io_vf13, a4::vec4 &io_vf14, a4::vec4 &io_vf15, a4::vec4 &io_vf16, a4::vec4 &io_vf17, a4::vec4 &io_vf24, a4::vec4 &io_vf25, a4::vec4 &io_vf26, a4::vec4 &io_vf28, a4::vec4 &io_vf29, a4::vec4 &io_vacc, float &io_vq, uint16_t &io_vstat, uint64_t &io_h2, uint64_t &io_h4, uint64_t io_h5, uint64_t &io_h8, uint64_t &io_h9, uint64_t &io_h10, uint64_t &io_h11, uint64_t &io_h12, uint64_t &io_h13, uint64_t &io_h14, uint64_t &io_h15)
{
    uint64_t at = 0;
    uint64_t v1 = 0;
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
    uint32_t fcr31 = 0;
    a4::vec4 vf0 = io_vf0;
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
    a4::vec4 vf24 = io_vf24;
    a4::vec4 vf25 = io_vf25;
    a4::vec4 vf26 = io_vf26;
    a4::vec4 vf28 = io_vf28;
    a4::vec4 vf29 = io_vf29;
    a4::vec4 vacc = io_vacc;
    float vq = io_vq;
    uint16_t vstat = io_vstat;
    uint64_t h2 = io_h2;
    uint64_t h4 = io_h4;
    uint64_t h5 = io_h5;
    uint64_t h8 = io_h8;
    uint64_t h9 = io_h9;
    uint64_t h10 = io_h10;
    uint64_t h11 = io_h11;
    uint64_t h12 = io_h12;
    uint64_t h13 = io_h13;
    uint64_t h14 = io_h14;
    uint64_t h15 = io_h15;
    (sp = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)sp, 4294967152))));
    (s0 = (uint64_t)((uint64_t)a0 + (uint64_t)0ULL));
    (s2 = (uint64_t)((uint64_t)a1 + (uint64_t)0ULL));
    { (a0 = (uint64_t)((uint64_t)a2 + (uint64_t)0ULL)); A3_CHARGE(); v0 = GetLayoutClass_u11(ram, rt, a0, f0, f1, f2, f3, f4, f5, f6, f7, f8, f9, f10, f11, f12, f13, f14, f15, f16, f17, f18, f19, f20, f21, f22, f23, f24, f25, f26, f27, f28, f29, f30, f31, h2); }
    (a0 = (uint64_t)((uint64_t)s2 + (uint64_t)0ULL));
    { (s1 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)v0, 308)))); A3_CHARGE(); v0 = GetCharacterClass(ram, rt, a0, h2, h5); }
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)v0, 308))));
    vf5 = a4v::castsi128_ps(A4_R128(ADD32((uint32_t)s1, 0)));
    vf6 = a4v::castsi128_ps(A4_R128(ADD32((uint32_t)v1, 0)));
    { a4::vec4 res = a4v::sub(vf5, vf6); a4::q128 mask = a4v::set_epi32(-1, -1, -1, -1); vf7 = a4v::blend(vf7, res, a4v::castsi128_ps(mask)); }
    A4_W128(ADD32((uint32_t)s0, 0), a4v::castps_si128(vf7));
    do { const a4::q128 _v = (A4_R128(ADD32((uint32_t)v0, 1136))); h4 = a4::hi64(_v); std::memcpy(&a0, &_v, 8); } while (0);
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)sp, 16))));
    A4_W128(ADD32((uint32_t)sp, 0), a4::mk128(h4, a0));
    { a4::vec4 res = a4v::addK<1, 1>(vf0, a4v::bcast<3>(vf0)); a4::q128 mask = a4v::set_epi32(0, -1, -1, -1); vf12 = a4v::blendv_ps(vf12, res, a4v::castsi128_ps(mask)); }
    vf11 = a4v::castsi128_ps(A4_R128(ADD32((uint32_t)sp, 0)));
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
    A4_W128(ADD32((uint32_t)v0, 0), a4v::castps_si128(vf14));
    A4_W128(ADD32((uint32_t)v0, 16), a4v::castps_si128(vf15));
    A4_W128(ADD32((uint32_t)v0, 32), a4v::castps_si128(vf16));
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)sp, 64))));
    A4_W128(ADD32((uint32_t)v1, 0), a4v::castps_si128(vf0));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)sp, 16))));
    vf24 = a4v::castsi128_ps(A4_R128(ADD32((uint32_t)v0, 48)));
    { a4::vec4 res = a4v::subK<1, 1>(vf0, a4v::bcast<3>(vf0)); a4::q128 mask = a4v::set_epi32(0, 0, 0, -1); vf25 = a4v::blendv_ps(vf25, res, a4v::castsi128_ps(mask)); }
    { a4::vec4 res = a4v::mulK<0, 1>(vf24, a4v::bcast<0>(vf25)); a4::q128 mask = a4v::set_epi32(0, -1, -1, -1); vf28 = a4v::blendv_ps(vf28, res, a4v::castsi128_ps(mask)); }
    { a4::vec4 res = a4v::sub(vf28, vf28); a4::q128 mask = a4v::set_epi32(-1, 0, 0, 0); vf28 = a4v::blend(vf28, res, a4v::castsi128_ps(mask)); }
    do { const a4::q128 _v = (A4_R128(ADD32((uint32_t)v0, 0))); h8 = a4::hi64(_v); std::memcpy(&t0, &_v, 8); } while (0);
    do { const a4::q128 _v = (A4_R128(ADD32((uint32_t)v0, 16))); h9 = a4::hi64(_v); std::memcpy(&t1, &_v, 8); } while (0);
    do { const a4::q128 _v = (A4_R128(ADD32((uint32_t)v0, 32))); h10 = a4::hi64(_v); std::memcpy(&t2, &_v, 8); } while (0);
    do { const a4::q128 _v = (a4v::castps_si128(vf0)); h11 = a4::hi64(_v); std::memcpy(&t3, &_v, 8); } while (0);
    do { const a4::q128 _v = (a4m::pextlw(a4::mk128(h9, t1), a4::mk128(h8, t0))); h12 = a4::hi64(_v); std::memcpy(&t4, &_v, 8); } while (0);
    do { const a4::q128 _v = (a4m::pextuw(a4::mk128(h9, t1), a4::mk128(h8, t0))); h13 = a4::hi64(_v); std::memcpy(&t5, &_v, 8); } while (0);
    do { const a4::q128 _v = (a4m::pextlw(a4::mk128(h11, t3), a4::mk128(h10, t2))); h14 = a4::hi64(_v); std::memcpy(&t6, &_v, 8); } while (0);
    do { const a4::q128 _v = (a4m::pextuw(a4::mk128(h11, t3), a4::mk128(h10, t2))); h15 = a4::hi64(_v); std::memcpy(&t7, &_v, 8); } while (0);
    do { const a4::q128 _v = (a4m::pcpyld(a4::mk128(h14, t6), a4::mk128(h12, t4))); h8 = a4::hi64(_v); std::memcpy(&t0, &_v, 8); } while (0);
    do { const a4::q128 _v = (a4v::unpackhi_epi64(a4::mk128(h12, t4), a4::mk128(h14, t6))); h9 = a4::hi64(_v); std::memcpy(&t1, &_v, 8); } while (0);
    do { const a4::q128 _v = (a4m::pcpyld(a4::mk128(h15, t7), a4::mk128(h13, t5))); h10 = a4::hi64(_v); std::memcpy(&t2, &_v, 8); } while (0);
    do { const a4::q128 _v = (a4v::unpackhi_epi64(a4::mk128(h13, t5), a4::mk128(h15, t7))); h11 = a4::hi64(_v); std::memcpy(&t3, &_v, 8); } while (0);
    vf24 = a4v::castsi128_ps(a4::mk128(h8, t0));
    vf25 = a4v::castsi128_ps(a4::mk128(h9, t1));
    vf26 = a4v::castsi128_ps(a4::mk128(h10, t2));
    { a4::vec4 res = a4v::mulK<0, 1>(vf24, a4v::bcast<0>(vf28)); vacc = a4v::dest<1, 1, 1, 1>(vacc, res); }
    { a4::vec4 mul_res = a4v::mulK<0, 1>(vf25, a4v::bcast<1>(vf28)); a4::vec4 res = a4v::addK<1, 1>(vacc, mul_res); vacc = a4v::dest<1, 1, 1, 1>(vacc, res); }
    { a4::vec4 mul_res = a4v::mulK<0, 1>(vf26, a4v::bcast<2>(vf28)); a4::vec4 res = a4v::addK<1, 1>(vacc, mul_res); vacc = a4v::dest<1, 1, 1, 1>(vacc, res); }
    { a4::vec4 mul_res = a4v::mulK<1, 1>(vf0, a4v::bcast<3>(vf28)); a4::vec4 res = a4v::addK<1, 1>(vacc, mul_res); a4::q128 mask = a4v::set_epi32(-1, -1, -1, -1); vf29 = a4v::blendv_ps(vf29, res, a4v::castsi128_ps(mask)); }
    { a4::vec4 res = a4v::subK<1, 1>(vf29, vf29); a4::q128 mask = a4v::set_epi32(-1, 0, 0, 0); vf29 = a4v::blend(vf29, res, a4v::castsi128_ps(mask)); }
    { a4::vec4 res = a4v::addK<1, 1>(vf29, vf0); a4::q128 mask = a4v::set_epi32(-1, 0, 0, 0); vf29 = a4v::blend(vf29, res, a4v::castsi128_ps(mask)); }
    A4_W128(ADD32((uint32_t)v0, 0), a4v::castps_si128(vf24));
    A4_W128(ADD32((uint32_t)v0, 16), a4v::castps_si128(vf25));
    A4_W128(ADD32((uint32_t)v0, 32), a4v::castps_si128(vf26));
    A4_W128(ADD32((uint32_t)v0, 48), a4v::castps_si128(vf29));
    vf8 = a4v::castsi128_ps(A4_R128(ADD32((uint32_t)s0, 0)));
    vf14 = a4v::castsi128_ps(A4_R128(ADD32((uint32_t)v0, 0)));
    vf15 = a4v::castsi128_ps(A4_R128(ADD32((uint32_t)v0, 16)));
    vf16 = a4v::castsi128_ps(A4_R128(ADD32((uint32_t)v0, 32)));
    vf17 = a4v::castsi128_ps(A4_R128(ADD32((uint32_t)v0, 48)));
    { a4::vec4 res = a4v::mul(vf14, a4v::bcast<0>(vf8)); vacc = a4v::dest<1, 1, 1, 1>(vacc, res); }
    { a4::vec4 mul_res = a4v::mul(vf15, a4v::bcast<1>(vf8)); a4::vec4 res = a4v::addK<1, 1>(vacc, mul_res); vacc = a4v::dest<1, 1, 1, 1>(vacc, res); }
    { a4::vec4 mul_res = a4v::mul(vf16, a4v::bcast<2>(vf8)); a4::vec4 res = a4v::addK<1, 1>(vacc, mul_res); vacc = a4v::dest<1, 1, 1, 1>(vacc, res); }
    { a4::vec4 mul_res = a4v::mul(vf17, a4v::bcast<3>(vf8)); a4::vec4 res = a4v::addK<1, 1>(vacc, mul_res); a4::q128 mask = a4v::set_epi32(-1, -1, -1, -1); vf10 = a4v::blendv_ps(vf10, res, a4v::castsi128_ps(mask)); }
    A4_W128(ADD32((uint32_t)s0, 0), a4v::castps_si128(vf10));
    A1_W32B((uint32_t)s0, 4, 0u);
    vf7 = a4v::castsi128_ps(A4_R128(ADD32((uint32_t)s0, 0)));
    { a4::vec4 res = a4v::mul(vf7, vf7); a4::q128 mask = a4v::set_epi32(0, -1, -1, -1); vf7 = a4v::blend(vf7, res, a4v::castsi128_ps(mask)); }
    { a4::vec4 res = a4v::mulK<1, 1>(vf0, a4v::bcast<0>(vf7)); vacc = a4v::dest<0, 0, 0, 1>(vacc, res); }
    { a4::vec4 mul_res = a4v::mulK<1, 1>(vf0, a4v::bcast<1>(vf7)); a4::vec4 res = a4v::addK<1, 1>(vacc, mul_res); vacc = a4v::dest<0, 0, 0, 1>(vacc, res); }
    { a4::vec4 mul_res = a4v::mulK<1, 1>(vf0, a4v::bcast<2>(vf7)); a4::vec4 res = a4v::addK<1, 1>(vacc, mul_res); a4::q128 mask = a4v::set_epi32(-1, 0, 0, 0); vf7 = a4v::blendv_ps(vf7, res, a4v::castsi128_ps(mask)); }
    { float ft = a4v::cvtss_f32(a4v::shuffle_ps<0x03>(vf7, vf7)); vq = a4v::sqrt(vstat, ft); }
    { uint32_t bits; std::memcpy(&bits, &vq, sizeof(bits)); (v0 = (uint64_t)((int64_t)(int32_t)(bits))); }
    { uint32_t bits = (uint32_t)v0; std::memcpy(&f0, &bits, sizeof(bits)); }
    (at = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)16256 << 16))));
    { uint32_t bits = (uint32_t)at; std::memcpy(&f1, &bits, sizeof(bits)); }
    f1 = a1f::div(fcr31, f1, f0);
    { uint32_t bits; std::memcpy(&bits, &f1, sizeof(bits)); (v0 = (uint64_t)((int64_t)(int32_t)(bits))); }
    vf5 = a4v::castsi128_ps(A4_R128(ADD32((uint32_t)s0, 0)));
    vf6 = a4v::castsi128_ps(a4::mk128(h2, v0));
    { a4::vec4 res = a4v::mul(vf5, a4v::bcast<0>(vf6)); a4::q128 mask = a4v::set_epi32(0, -1, -1, -1); vf5 = a4v::blendv_ps(vf5, res, a4v::castsi128_ps(mask)); }
    A4_W128(ADD32((uint32_t)s0, 0), a4v::castps_si128(vf5));
    { (sp = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)sp, 144)))); io_vf5 = vf5; io_vf6 = vf6; io_vf7 = vf7; io_vf8 = vf8; io_vf10 = vf10; io_vf11 = vf11; io_vf12 = vf12; io_vf13 = vf13; io_vf14 = vf14; io_vf15 = vf15; io_vf16 = vf16; io_vf17 = vf17; io_vf24 = vf24; io_vf25 = vf25; io_vf26 = vf26; io_vf28 = vf28; io_vf29 = vf29; io_vacc = vacc; io_vq = vq; io_vstat = vstat; io_h2 = h2; io_h4 = h4; io_h8 = h8; io_h9 = h9; io_h10 = h10; io_h11 = h11; io_h12 = h12; io_h13 = h13; io_h14 = h14; io_h15 = h15; return a1::RetVF{(uint64_t)v0, f0}; }
    __builtin_unreachable();
}

// gamecore_sub_01395BC8 (0x1395bc8): typed by tools/typed_lift2.py (round A6; guest frame calls, extra outputs xo[0]=a0 xo[1]=a1 xo[2]=t0 xo[3]=t1 xo[4]=t2 xo[5]=t3; VU0 in vf0 vf7 vf10 vf12 vf13 vf14 vf15 vf16 vf17 vf25 vf28 vf29 vacc vstat h5, out vf5 vf6 vf7 vf8 vf10 vf11 vf12 vf13 vf14 vf15 vf16 vf17 vf24 vf25 vf26 vf28 vf29 vacc vq vstat h2 h8 h9 h10 h11 h12 h13 h14 h15, relocated calls 3)
inline a1::RetVF gamecore_sub_01395BC8(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint64_t v0, uint64_t a0, uint64_t a1, uint64_t a2, uint64_t a3, uint64_t t0, uint64_t sp, float f0, float f1, float f2, float f3, float f4, float f5, float f6, float f7, float f8, float f9, float f10, float f11, float f12, float f13, float f14, float f15, float f16, float f17, float f18, float f19, float f20, float f21, float f22, float f23, float f24, float f25, float f26, float f27, float f28, float f29, float f30, float f31, const a4::vec4 &io_vf0, a4::vec4 &io_vf5, a4::vec4 &io_vf6, a4::vec4 &io_vf7, a4::vec4 &io_vf8, a4::vec4 &io_vf10, a4::vec4 &io_vf11, a4::vec4 &io_vf12, a4::vec4 &io_vf13, a4::vec4 &io_vf14, a4::vec4 &io_vf15, a4::vec4 &io_vf16, a4::vec4 &io_vf17, a4::vec4 &io_vf24, a4::vec4 &io_vf25, a4::vec4 &io_vf26, a4::vec4 &io_vf28, a4::vec4 &io_vf29, a4::vec4 &io_vacc, float &io_vq, uint16_t &io_vstat, uint64_t &io_h2, uint64_t io_h5, uint64_t &io_h8, uint64_t &io_h9, uint64_t &io_h10, uint64_t &io_h11, uint64_t &io_h12, uint64_t &io_h13, uint64_t &io_h14, uint64_t &io_h15, uint64_t *xo = nullptr)
{
    uint64_t v1 = 0;
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
    a4::vec4 vf0 = io_vf0;
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
    a4::vec4 vf24 = io_vf24;
    a4::vec4 vf25 = io_vf25;
    a4::vec4 vf26 = io_vf26;
    a4::vec4 vf28 = io_vf28;
    a4::vec4 vf29 = io_vf29;
    a4::vec4 vacc = io_vacc;
    float vq = io_vq;
    uint16_t vstat = io_vstat;
    uint64_t h2 = io_h2;
    uint64_t h5 = io_h5;
    uint64_t h8 = io_h8;
    uint64_t h9 = io_h9;
    uint64_t h10 = io_h10;
    uint64_t h11 = io_h11;
    uint64_t h12 = io_h12;
    uint64_t h13 = io_h13;
    uint64_t h14 = io_h14;
    uint64_t h15 = io_h15;
    (sp = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)sp, 4294967152))));
    (s2 = (uint64_t)((uint64_t)a0 + (uint64_t)0ULL));
    (s1 = (uint64_t)((uint64_t)a3 + (uint64_t)0ULL));
    (s3 = (uint64_t)((uint64_t)t0 + (uint64_t)0ULL));
    (s4 = (uint64_t)((uint64_t)a1 + (uint64_t)0ULL));
    { (s5 = (uint64_t)((uint64_t)a2 + (uint64_t)0ULL)); A3_CHARGE(); uint64_t a7c[2] = {0u, 0u}; v0 = GetCharacterClass_x11(ram, rt, a0, h2, h5, a7c); a1 = a7c[1]; }
    (a0 = (uint64_t)((uint64_t)s2 + (uint64_t)0ULL));
    { (s0 = (uint64_t)((uint64_t)v0 + (uint64_t)0ULL)); A3_CHARGE(); uint64_t a7c[3] = {0u, a0, a1}; v0 = GetLayoutClass_u11(ram, rt, a0, f0, f1, f2, f3, f4, f5, f6, f7, f8, f9, f10, f11, f12, f13, f14, f15, f16, f17, f18, f19, f20, f21, f22, f23, f24, f25, f26, f27, f28, f29, f30, f31, h2, a7c); a0 = a7c[1]; a1 = a7c[2]; }
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)v0, 308))));
    vf5 = a4v::castsi128_ps(A4_R128(ADD32((uint32_t)v1, 0)));
    vf6 = a4v::castsi128_ps(A4_R128(ADD32((uint32_t)s1, 0)));
    { a4::vec4 res = a4v::sub(vf5, vf6); a4::q128 mask = a4v::set_epi32(-1, -1, -1, -1); vf7 = a4v::blend(vf7, res, a4v::castsi128_ps(mask)); }
    { a4::vec4 res = a4v::mulK<1, 1>(vf7, vf7); a4::q128 mask = a4v::set_epi32(0, -1, -1, -1); vf7 = a4v::blend(vf7, res, a4v::castsi128_ps(mask)); }
    { a4::vec4 res = a4v::mulK<1, 1>(vf0, a4v::bcast<0>(vf7)); vacc = a4v::dest<0, 0, 0, 1>(vacc, res); }
    { a4::vec4 mul_res = a4v::mulK<1, 1>(vf0, a4v::bcast<1>(vf7)); a4::vec4 res = a4v::addK<1, 1>(vacc, mul_res); vacc = a4v::dest<0, 0, 0, 1>(vacc, res); }
    { a4::vec4 mul_res = a4v::mulK<1, 1>(vf0, a4v::bcast<2>(vf7)); a4::vec4 res = a4v::addK<1, 1>(vacc, mul_res); a4::q128 mask = a4v::set_epi32(-1, 0, 0, 0); vf7 = a4v::blendv_ps(vf7, res, a4v::castsi128_ps(mask)); }
    { float ft = a4v::cvtss_f32(a4v::shuffle_ps<0x03>(vf7, vf7)); vq = a4v::sqrt(vstat, ft); }
    { uint32_t bits; std::memcpy(&bits, &vq, sizeof(bits)); (v0 = (uint64_t)((int64_t)(int32_t)(bits))); }
    { uint32_t bits = (uint32_t)v0; std::memcpy(&f0, &bits, sizeof(bits)); }
    { float f = f0; uint32_t bits; std::memcpy(&bits, &f, sizeof(bits)); A1_W32B((uint32_t)s4, 0, bits); }
    do { const a4::q128 _v = (A4_R128(ADD32((uint32_t)s3, 0))); h2 = a4::hi64(_v); std::memcpy(&v0, &_v, 8); } while (0);
    A4_W128(ADD32((uint32_t)sp, 0), a4::mk128(h2, v0));
    A1_W32B((uint32_t)sp, 4, 0u);
    vf5 = a4v::castsi128_ps(A4_R128(ADD32((uint32_t)sp, 0)));
    { a4::vec4 res = a4v::mul(vf5, vf5); a4::q128 mask = a4v::set_epi32(0, -1, -1, -1); vf7 = a4v::blend(vf7, res, a4v::castsi128_ps(mask)); }
    { a4::vec4 res = a4v::mulK<1, 1>(vf0, a4v::bcast<0>(vf7)); vacc = a4v::dest<0, 0, 0, 1>(vacc, res); }
    { a4::vec4 mul_res = a4v::mulK<1, 1>(vf0, a4v::bcast<1>(vf7)); a4::vec4 res = a4v::addK<1, 1>(vacc, mul_res); vacc = a4v::dest<0, 0, 0, 1>(vacc, res); }
    { a4::vec4 mul_res = a4v::mulK<1, 1>(vf0, a4v::bcast<2>(vf7)); a4::vec4 res = a4v::addK<1, 1>(vacc, mul_res); a4::q128 mask = a4v::set_epi32(-1, 0, 0, 0); vf7 = a4v::blendv_ps(vf7, res, a4v::castsi128_ps(mask)); }
    { float fs = a4v::cvtss_f32(a4v::shuffle_ps<0x03>(vf0, vf0)); float ft = a4v::cvtss_f32(a4v::shuffle_ps<0x03>(vf7, vf7)); vq = a4v::rsqrt(vstat, fs, ft); }
    { a4::vec4 res = a4v::mul(vf5, a4v::set1_ps(vq)); a4::q128 mask = a4v::set_epi32(0, -1, -1, -1); vf5 = a4v::blendv_ps(vf5, res, a4v::castsi128_ps(mask)); }
    A4_W128(ADD32((uint32_t)sp, 0), a4v::castps_si128(vf5));
    (s0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)s0, 1136))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)sp, 16))));
    { a4::vec4 res = a4v::addK<1, 1>(vf0, a4v::bcast<3>(vf0)); a4::q128 mask = a4v::set_epi32(0, -1, -1, -1); vf12 = a4v::blendv_ps(vf12, res, a4v::castsi128_ps(mask)); }
    vf11 = a4v::castsi128_ps(A4_R128(ADD32((uint32_t)s0, 0)));
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
    A4_W128(ADD32((uint32_t)v0, 0), a4v::castps_si128(vf14));
    A4_W128(ADD32((uint32_t)v0, 16), a4v::castps_si128(vf15));
    A4_W128(ADD32((uint32_t)v0, 32), a4v::castps_si128(vf16));
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)sp, 64))));
    A4_W128(ADD32((uint32_t)v1, 0), a4v::castps_si128(vf0));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)sp, 16))));
    vf24 = a4v::castsi128_ps(A4_R128(ADD32((uint32_t)v0, 48)));
    { a4::vec4 res = a4v::subK<1, 1>(vf0, a4v::bcast<3>(vf0)); a4::q128 mask = a4v::set_epi32(0, 0, 0, -1); vf25 = a4v::blendv_ps(vf25, res, a4v::castsi128_ps(mask)); }
    { a4::vec4 res = a4v::mulK<0, 1>(vf24, a4v::bcast<0>(vf25)); a4::q128 mask = a4v::set_epi32(0, -1, -1, -1); vf28 = a4v::blendv_ps(vf28, res, a4v::castsi128_ps(mask)); }
    { a4::vec4 res = a4v::sub(vf28, vf28); a4::q128 mask = a4v::set_epi32(-1, 0, 0, 0); vf28 = a4v::blend(vf28, res, a4v::castsi128_ps(mask)); }
    do { const a4::q128 _v = (A4_R128(ADD32((uint32_t)v0, 0))); h8 = a4::hi64(_v); std::memcpy(&t0, &_v, 8); } while (0);
    do { const a4::q128 _v = (A4_R128(ADD32((uint32_t)v0, 16))); h9 = a4::hi64(_v); std::memcpy(&t1, &_v, 8); } while (0);
    do { const a4::q128 _v = (A4_R128(ADD32((uint32_t)v0, 32))); h10 = a4::hi64(_v); std::memcpy(&t2, &_v, 8); } while (0);
    do { const a4::q128 _v = (a4v::castps_si128(vf0)); h11 = a4::hi64(_v); std::memcpy(&t3, &_v, 8); } while (0);
    do { const a4::q128 _v = (a4m::pextlw(a4::mk128(h9, t1), a4::mk128(h8, t0))); h12 = a4::hi64(_v); std::memcpy(&t4, &_v, 8); } while (0);
    do { const a4::q128 _v = (a4m::pextuw(a4::mk128(h9, t1), a4::mk128(h8, t0))); h13 = a4::hi64(_v); std::memcpy(&t5, &_v, 8); } while (0);
    do { const a4::q128 _v = (a4m::pextlw(a4::mk128(h11, t3), a4::mk128(h10, t2))); h14 = a4::hi64(_v); std::memcpy(&t6, &_v, 8); } while (0);
    do { const a4::q128 _v = (a4m::pextuw(a4::mk128(h11, t3), a4::mk128(h10, t2))); h15 = a4::hi64(_v); std::memcpy(&t7, &_v, 8); } while (0);
    do { const a4::q128 _v = (a4m::pcpyld(a4::mk128(h14, t6), a4::mk128(h12, t4))); h8 = a4::hi64(_v); std::memcpy(&t0, &_v, 8); } while (0);
    do { const a4::q128 _v = (a4v::unpackhi_epi64(a4::mk128(h12, t4), a4::mk128(h14, t6))); h9 = a4::hi64(_v); std::memcpy(&t1, &_v, 8); } while (0);
    do { const a4::q128 _v = (a4m::pcpyld(a4::mk128(h15, t7), a4::mk128(h13, t5))); h10 = a4::hi64(_v); std::memcpy(&t2, &_v, 8); } while (0);
    do { const a4::q128 _v = (a4v::unpackhi_epi64(a4::mk128(h13, t5), a4::mk128(h15, t7))); h11 = a4::hi64(_v); std::memcpy(&t3, &_v, 8); } while (0);
    vf24 = a4v::castsi128_ps(a4::mk128(h8, t0));
    vf25 = a4v::castsi128_ps(a4::mk128(h9, t1));
    vf26 = a4v::castsi128_ps(a4::mk128(h10, t2));
    { a4::vec4 res = a4v::mulK<0, 1>(vf24, a4v::bcast<0>(vf28)); vacc = a4v::dest<1, 1, 1, 1>(vacc, res); }
    { a4::vec4 mul_res = a4v::mulK<0, 1>(vf25, a4v::bcast<1>(vf28)); a4::vec4 res = a4v::addK<1, 1>(vacc, mul_res); vacc = a4v::dest<1, 1, 1, 1>(vacc, res); }
    { a4::vec4 mul_res = a4v::mulK<0, 1>(vf26, a4v::bcast<2>(vf28)); a4::vec4 res = a4v::addK<1, 1>(vacc, mul_res); vacc = a4v::dest<1, 1, 1, 1>(vacc, res); }
    { a4::vec4 mul_res = a4v::mulK<1, 1>(vf0, a4v::bcast<3>(vf28)); a4::vec4 res = a4v::addK<1, 1>(vacc, mul_res); a4::q128 mask = a4v::set_epi32(-1, -1, -1, -1); vf29 = a4v::blendv_ps(vf29, res, a4v::castsi128_ps(mask)); }
    { a4::vec4 res = a4v::subK<1, 1>(vf29, vf29); a4::q128 mask = a4v::set_epi32(-1, 0, 0, 0); vf29 = a4v::blend(vf29, res, a4v::castsi128_ps(mask)); }
    { a4::vec4 res = a4v::addK<1, 1>(vf29, vf0); a4::q128 mask = a4v::set_epi32(-1, 0, 0, 0); vf29 = a4v::blend(vf29, res, a4v::castsi128_ps(mask)); }
    A4_W128(ADD32((uint32_t)v0, 0), a4v::castps_si128(vf24));
    A4_W128(ADD32((uint32_t)v0, 16), a4v::castps_si128(vf25));
    A4_W128(ADD32((uint32_t)v0, 32), a4v::castps_si128(vf26));
    A4_W128(ADD32((uint32_t)v0, 48), a4v::castps_si128(vf29));
    vf8 = a4v::castsi128_ps(A4_R128(ADD32((uint32_t)sp, 0)));
    vf14 = a4v::castsi128_ps(A4_R128(ADD32((uint32_t)v0, 0)));
    vf15 = a4v::castsi128_ps(A4_R128(ADD32((uint32_t)v0, 16)));
    vf16 = a4v::castsi128_ps(A4_R128(ADD32((uint32_t)v0, 32)));
    vf17 = a4v::castsi128_ps(A4_R128(ADD32((uint32_t)v0, 48)));
    { a4::vec4 res = a4v::mul(vf14, a4v::bcast<0>(vf8)); vacc = a4v::dest<1, 1, 1, 1>(vacc, res); }
    { a4::vec4 mul_res = a4v::mul(vf15, a4v::bcast<1>(vf8)); a4::vec4 res = a4v::addK<1, 1>(vacc, mul_res); vacc = a4v::dest<1, 1, 1, 1>(vacc, res); }
    { a4::vec4 mul_res = a4v::mul(vf16, a4v::bcast<2>(vf8)); a4::vec4 res = a4v::addK<1, 1>(vacc, mul_res); vacc = a4v::dest<1, 1, 1, 1>(vacc, res); }
    { a4::vec4 mul_res = a4v::mul(vf17, a4v::bcast<3>(vf8)); a4::vec4 res = a4v::addK<1, 1>(vacc, mul_res); a4::q128 mask = a4v::set_epi32(-1, -1, -1, -1); vf10 = a4v::blendv_ps(vf10, res, a4v::castsi128_ps(mask)); }
    A4_W128(ADD32((uint32_t)sp, 0), a4v::castps_si128(vf10));
    { uint32_t bits = A1_R32B((uint32_t)sp, 0); float f; std::memcpy(&f, &bits, sizeof(f)); f12 = f; }
    { { uint32_t bits = A1_R32B((uint32_t)sp, 8); float f; std::memcpy(&f, &bits, sizeof(f)); f13 = f; } A3_CHARGE(); f0 = a9x::atan2Exact(ram, rt, f12, f13); }
    { float f = f0; uint32_t bits; std::memcpy(&bits, &f, sizeof(bits)); A1_W32B((uint32_t)s5, 0, bits); }
    { (sp = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)sp, 144)))); if (xo) { xo[0] = a0; xo[1] = a1; xo[2] = t0; xo[3] = t1; xo[4] = t2; xo[5] = t3; } io_vf5 = vf5; io_vf6 = vf6; io_vf7 = vf7; io_vf8 = vf8; io_vf10 = vf10; io_vf11 = vf11; io_vf12 = vf12; io_vf13 = vf13; io_vf14 = vf14; io_vf15 = vf15; io_vf16 = vf16; io_vf17 = vf17; io_vf24 = vf24; io_vf25 = vf25; io_vf26 = vf26; io_vf28 = vf28; io_vf29 = vf29; io_vacc = vacc; io_vq = vq; io_vstat = vstat; io_h2 = h2; io_h8 = h8; io_h9 = h9; io_h10 = h10; io_h11 = h11; io_h12 = h12; io_h13 = h13; io_h14 = h14; io_h15 = h15; return a1::RetVF{(uint64_t)v0, f0}; }
    __builtin_unreachable();
}

// gamecore_sub_013BAAC8 (0x13baac8): typed by tools/typed_lift2.py (round A6; guest frame calls; VU0 in vf0 vf10 vf28 vf29 vacc, out vf5 vf6 vf7 vf8 vf10 vf14 vf15 vf16 vf17 vf24 vf25 vf26 vf27 vf28 vf29 vacc h2 h6 h7 h8 h9 h10 h11 h12 h13 h14 h15, relocated calls 4)
inline a1::RetVF gamecore_sub_013BAAC8(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint64_t v0, uint64_t a0, uint64_t a1, uint64_t sp, float f0, float f1, float f2, float f3, float f4, float f5, float f6, float f7, float f8, float f9, float f10, float f11, float f12, float f13, float f14, float f15, float f16, float f17, float f18, float f19, float f20, float f21, float f22, float f23, float f24, float f25, float f26, float f27, float f28, float f29, float f30, float f31, const a4::vec4 &io_vf0, a4::vec4 &io_vf5, a4::vec4 &io_vf6, a4::vec4 &io_vf7, a4::vec4 &io_vf8, a4::vec4 &io_vf10, a4::vec4 &io_vf14, a4::vec4 &io_vf15, a4::vec4 &io_vf16, a4::vec4 &io_vf17, a4::vec4 &io_vf24, a4::vec4 &io_vf25, a4::vec4 &io_vf26, a4::vec4 &io_vf27, a4::vec4 &io_vf28, a4::vec4 &io_vf29, a4::vec4 &io_vacc, uint64_t &io_h2, uint64_t &io_h6, uint64_t &io_h7, uint64_t &io_h8, uint64_t &io_h9, uint64_t &io_h10, uint64_t &io_h11, uint64_t &io_h12, uint64_t &io_h13, uint64_t &io_h14, uint64_t &io_h15)
{
    uint64_t at = 0;
    uint64_t v1 = 0;
    uint64_t a2 = 0;
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
    a4::vec4 vf24 = io_vf24;
    a4::vec4 vf25 = io_vf25;
    a4::vec4 vf26 = io_vf26;
    a4::vec4 vf27 = io_vf27;
    a4::vec4 vf28 = io_vf28;
    a4::vec4 vf29 = io_vf29;
    a4::vec4 vacc = io_vacc;
    uint64_t h2 = io_h2;
    uint64_t h6 = io_h6;
    uint64_t h7 = io_h7;
    uint64_t h8 = io_h8;
    uint64_t h9 = io_h9;
    uint64_t h10 = io_h10;
    uint64_t h11 = io_h11;
    uint64_t h12 = io_h12;
    uint64_t h13 = io_h13;
    uint64_t h14 = io_h14;
    uint64_t h15 = io_h15;
    (sp = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)sp, 4294967104))));
    (s1 = (uint64_t)((uint64_t)a0 + (uint64_t)0ULL));
    (s2 = (uint64_t)((uint64_t)a1 + (uint64_t)0ULL));
    (s3 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL));
    { float f = f20; uint32_t bits; std::memcpy(&bits, &f, sizeof(bits)); A1_W32B((uint32_t)sp, 184, bits); }
    (at = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)16025 << 16))));
    (at = (uint64_t)(RC_U64X(at) | (uint64_t)(uint16_t)39321));
    { uint32_t bits = (uint32_t)at; std::memcpy(&f20, &bits, sizeof(bits)); }
    { (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)s2, 0)))); A3_CHARGE(); v0 = GetLayoutClass_u11(ram, rt, a0, f0, f1, f2, f3, f4, f5, f6, f7, f8, f9, f10, f11, f12, f13, f14, f15, f16, f17, f18, f19, f20, f21, f22, f23, f24, f25, f26, f27, f28, f29, f30, f31, h2); }
    (a2 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 4294959104))));
    (a1 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)v0, 344))));
    (s0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)sp, 64))));
    (a0 = (uint64_t)((uint64_t)RC_U64X(s0) + (uint64_t)0ULL));
    { (a1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)a1, 1472)))); A3_CHARGE(); uint64_t a7c[5] = {0u, 0u, 0u, 0u, 0u}; { const a1::RetVF a2_r = iosMulRotMatrixZInt16(ram, rt, a0, a1, a2, f0, f20, vf14, vf15, vf16, vf17, vf24, vf25, vf26, vf27, vacc, h6, h7, h8, h9, a7c); v0 = a2_r.v0; f0 = a2_r.f0; } f20 = a1_flt((uint32_t)a7c[0]); }
    (a0 = (uint64_t)((uint64_t)s0 + (uint64_t)0ULL));
    (a1 = (uint64_t)((uint64_t)s0 + (uint64_t)0ULL));
    { (a2 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 3640)))); A3_CHARGE(); uint64_t a7c[5] = {0u, 0u, 0u, 0u, 0u}; { const a1::RetVF a2_r = iosMulRotMatrixXInt16(ram, rt, a0, a1, a2, f0, f20, vf14, vf15, vf16, vf17, vf24, vf25, vf26, vf27, vacc, h6, h7, h8, h9, a7c); v0 = a2_r.v0; f0 = a2_r.f0; } f20 = a1_flt((uint32_t)a7c[0]); }
    { uint32_t bits = 0u; std::memcpy(&f12, &bits, sizeof(bits)); }
    (a0 = (uint64_t)((uint64_t)s0 + (uint64_t)0ULL));
    (a1 = (uint64_t)((uint64_t)s0 + (uint64_t)0ULL));
    f13 = ((float)(f20));
    { f14 = ((float)(f12)); A3_CHARGE(); { const a1::RetVF a2_r = iosTransMatrixF(ram, rt, (uint32_t)a0, (uint32_t)a1, (uint32_t)sp, f12, f13, f14, vf8, vf10, vf14, vf15, vf16, vf17, vacc, h6, h7, h8, h9); v0 = a2_r.v0; f0 = a2_r.f0; } }
    vf24 = a4v::castsi128_ps(A4_R128(ADD32((uint32_t)s0, 48)));
    { a4::vec4 res = a4v::subK<1, 1>(vf0, a4v::bcast<3>(vf0)); a4::q128 mask = a4v::set_epi32(0, 0, 0, -1); vf25 = a4v::blendv_ps(vf25, res, a4v::castsi128_ps(mask)); }
    { a4::vec4 res = a4v::mulK<0, 1>(vf24, a4v::bcast<0>(vf25)); a4::q128 mask = a4v::set_epi32(0, -1, -1, -1); vf28 = a4v::blendv_ps(vf28, res, a4v::castsi128_ps(mask)); }
    { a4::vec4 res = a4v::sub(vf28, vf28); a4::q128 mask = a4v::set_epi32(-1, 0, 0, 0); vf28 = a4v::blend(vf28, res, a4v::castsi128_ps(mask)); }
    do { const a4::q128 _v = (A4_R128(ADD32((uint32_t)s0, 0))); h8 = a4::hi64(_v); std::memcpy(&t0, &_v, 8); } while (0);
    do { const a4::q128 _v = (A4_R128(ADD32((uint32_t)s0, 16))); h9 = a4::hi64(_v); std::memcpy(&t1, &_v, 8); } while (0);
    do { const a4::q128 _v = (A4_R128(ADD32((uint32_t)s0, 32))); h10 = a4::hi64(_v); std::memcpy(&t2, &_v, 8); } while (0);
    do { const a4::q128 _v = (a4v::castps_si128(vf0)); h11 = a4::hi64(_v); std::memcpy(&t3, &_v, 8); } while (0);
    do { const a4::q128 _v = (a4m::pextlw(a4::mk128(h9, t1), a4::mk128(h8, t0))); h12 = a4::hi64(_v); std::memcpy(&t4, &_v, 8); } while (0);
    do { const a4::q128 _v = (a4m::pextuw(a4::mk128(h9, t1), a4::mk128(h8, t0))); h13 = a4::hi64(_v); std::memcpy(&t5, &_v, 8); } while (0);
    do { const a4::q128 _v = (a4m::pextlw(a4::mk128(h11, t3), a4::mk128(h10, t2))); h14 = a4::hi64(_v); std::memcpy(&t6, &_v, 8); } while (0);
    do { const a4::q128 _v = (a4m::pextuw(a4::mk128(h11, t3), a4::mk128(h10, t2))); h15 = a4::hi64(_v); std::memcpy(&t7, &_v, 8); } while (0);
    do { const a4::q128 _v = (a4m::pcpyld(a4::mk128(h14, t6), a4::mk128(h12, t4))); h8 = a4::hi64(_v); std::memcpy(&t0, &_v, 8); } while (0);
    do { const a4::q128 _v = (a4v::unpackhi_epi64(a4::mk128(h12, t4), a4::mk128(h14, t6))); h9 = a4::hi64(_v); std::memcpy(&t1, &_v, 8); } while (0);
    do { const a4::q128 _v = (a4m::pcpyld(a4::mk128(h15, t7), a4::mk128(h13, t5))); h10 = a4::hi64(_v); std::memcpy(&t2, &_v, 8); } while (0);
    do { const a4::q128 _v = (a4v::unpackhi_epi64(a4::mk128(h13, t5), a4::mk128(h15, t7))); h11 = a4::hi64(_v); std::memcpy(&t3, &_v, 8); } while (0);
    vf24 = a4v::castsi128_ps(a4::mk128(h8, t0));
    vf25 = a4v::castsi128_ps(a4::mk128(h9, t1));
    vf26 = a4v::castsi128_ps(a4::mk128(h10, t2));
    { a4::vec4 res = a4v::mulK<0, 1>(vf24, a4v::bcast<0>(vf28)); vacc = a4v::dest<1, 1, 1, 1>(vacc, res); }
    { a4::vec4 mul_res = a4v::mulK<0, 1>(vf25, a4v::bcast<1>(vf28)); a4::vec4 res = a4v::addK<1, 1>(vacc, mul_res); vacc = a4v::dest<1, 1, 1, 1>(vacc, res); }
    { a4::vec4 mul_res = a4v::mulK<0, 1>(vf26, a4v::bcast<2>(vf28)); a4::vec4 res = a4v::addK<1, 1>(vacc, mul_res); vacc = a4v::dest<1, 1, 1, 1>(vacc, res); }
    { a4::vec4 mul_res = a4v::mulK<1, 1>(vf0, a4v::bcast<3>(vf28)); a4::vec4 res = a4v::addK<1, 1>(vacc, mul_res); a4::q128 mask = a4v::set_epi32(-1, -1, -1, -1); vf29 = a4v::blendv_ps(vf29, res, a4v::castsi128_ps(mask)); }
    { a4::vec4 res = a4v::subK<1, 1>(vf29, vf29); a4::q128 mask = a4v::set_epi32(-1, 0, 0, 0); vf29 = a4v::blend(vf29, res, a4v::castsi128_ps(mask)); }
    { a4::vec4 res = a4v::addK<1, 1>(vf29, vf0); a4::q128 mask = a4v::set_epi32(-1, 0, 0, 0); vf29 = a4v::blend(vf29, res, a4v::castsi128_ps(mask)); }
    A4_W128(ADD32((uint32_t)sp, 0), a4v::castps_si128(vf24));
    A4_W128(ADD32((uint32_t)sp, 16), a4v::castps_si128(vf25));
    A4_W128(ADD32((uint32_t)sp, 32), a4v::castps_si128(vf26));
    A4_W128(ADD32((uint32_t)sp, 48), a4v::castps_si128(vf29));
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)sp, 128))));
    vf8 = a4v::castsi128_ps(A4_R128(ADD32((uint32_t)s1, 0)));
    vf14 = a4v::castsi128_ps(A4_R128(ADD32((uint32_t)sp, 0)));
    vf15 = a4v::castsi128_ps(A4_R128(ADD32((uint32_t)sp, 16)));
    vf16 = a4v::castsi128_ps(A4_R128(ADD32((uint32_t)sp, 32)));
    vf17 = a4v::castsi128_ps(A4_R128(ADD32((uint32_t)sp, 48)));
    { a4::vec4 res = a4v::mul(vf14, a4v::bcast<0>(vf8)); vacc = a4v::dest<1, 1, 1, 1>(vacc, res); }
    { a4::vec4 mul_res = a4v::mul(vf15, a4v::bcast<1>(vf8)); a4::vec4 res = a4v::addK<1, 1>(vacc, mul_res); vacc = a4v::dest<1, 1, 1, 1>(vacc, res); }
    { a4::vec4 mul_res = a4v::mul(vf16, a4v::bcast<2>(vf8)); a4::vec4 res = a4v::addK<1, 1>(vacc, mul_res); vacc = a4v::dest<1, 1, 1, 1>(vacc, res); }
    { a4::vec4 mul_res = a4v::mul(vf17, a4v::bcast<3>(vf8)); a4::vec4 res = a4v::addK<1, 1>(vacc, mul_res); a4::q128 mask = a4v::set_epi32(-1, -1, -1, -1); vf10 = a4v::blendv_ps(vf10, res, a4v::castsi128_ps(mask)); }
    A4_W128(ADD32((uint32_t)v1, 0), a4v::castps_si128(vf10));
    { uint32_t bits = A1_R32B((uint32_t)sp, 136); float f; std::memcpy(&f, &bits, sizeof(f)); f0 = f; }
    fcr31 = (a1f::lt(f0, f20)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
    { const bool c = (fcr31 & 0x800000u) == 0u; (v0 = (uint64_t)((uint64_t)s3 + (uint64_t)0ULL)); if (c) goto L_13bac74; }
    vf7 = a4v::castsi128_ps(A4_R128(ADD32((uint32_t)v1, 0)));
    { a4::vec4 res = a4v::mul(vf7, vf7); a4::q128 mask = a4v::set_epi32(0, 0, -1, -1); vf7 = a4v::blend(vf7, res, a4v::castsi128_ps(mask)); }
    { a4::vec4 res = a4v::addK<1, 1>(vf7, a4v::bcast<1>(vf7)); a4::q128 mask = a4v::set_epi32(0, 0, 0, -1); vf7 = a4v::blendv_ps(vf7, res, a4v::castsi128_ps(mask)); }
    do { const a4::q128 _v = (a4v::castps_si128(vf7)); h2 = a4::hi64(_v); std::memcpy(&v0, &_v, 8); } while (0);
    { uint32_t bits = (uint32_t)v0; std::memcpy(&f2, &bits, sizeof(bits)); }
    { uint32_t bits = A1_R32B((uint32_t)s2, 4); float f; std::memcpy(&f, &bits, sizeof(f)); f0 = f; }
    f13 = a1f::add(f0, f20);
    f1 = ((float)(f13) * (float)(f13));
    fcr31 = (a1f::lt(f2, f1)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
    { const bool c = (fcr31 & 0x800000u) == 0u; (v0 = (uint64_t)((uint64_t)s3 + (uint64_t)0ULL)); if (c) goto L_13bac74; }
    f0 = a1f::sqrt(fcr31, f2);
    f0 = a1f::div(fcr31, f13, f0);
    vf5 = a4v::castsi128_ps(A4_R128(ADD32((uint32_t)v1, 0)));
    { uint32_t bits; std::memcpy(&bits, &f0, sizeof(bits)); (t0 = (uint64_t)((int64_t)(int32_t)(bits))); }
    vf6 = a4v::castsi128_ps(a4::mk128(h8, t0));
    { a4::vec4 res = a4v::mul(vf5, a4v::bcast<0>(vf6)); a4::q128 mask = a4v::set_epi32(0, 0, -1, -1); vf5 = a4v::blendv_ps(vf5, res, a4v::castsi128_ps(mask)); }
    A4_W128(ADD32((uint32_t)v1, 0), a4v::castps_si128(vf5));
    vf8 = a4v::castsi128_ps(A4_R128(ADD32((uint32_t)v1, 0)));
    vf14 = a4v::castsi128_ps(A4_R128(ADD32((uint32_t)s0, 0)));
    vf15 = a4v::castsi128_ps(A4_R128(ADD32((uint32_t)s0, 16)));
    vf16 = a4v::castsi128_ps(A4_R128(ADD32((uint32_t)s0, 32)));
    vf17 = a4v::castsi128_ps(A4_R128(ADD32((uint32_t)s0, 48)));
    { a4::vec4 res = a4v::mul(vf14, a4v::bcast<0>(vf8)); vacc = a4v::dest<1, 1, 1, 1>(vacc, res); }
    { a4::vec4 mul_res = a4v::mul(vf15, a4v::bcast<1>(vf8)); a4::vec4 res = a4v::addK<1, 1>(vacc, mul_res); vacc = a4v::dest<1, 1, 1, 1>(vacc, res); }
    { a4::vec4 mul_res = a4v::mul(vf16, a4v::bcast<2>(vf8)); a4::vec4 res = a4v::addK<1, 1>(vacc, mul_res); vacc = a4v::dest<1, 1, 1, 1>(vacc, res); }
    { a4::vec4 mul_res = a4v::mul(vf17, a4v::bcast<3>(vf8)); a4::vec4 res = a4v::addK<1, 1>(vacc, mul_res); a4::q128 mask = a4v::set_epi32(-1, -1, -1, -1); vf10 = a4v::blendv_ps(vf10, res, a4v::castsi128_ps(mask)); }
    A4_W128(ADD32((uint32_t)s1, 0), a4v::castps_si128(vf10));
    (s3 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 1))));
    (v0 = (uint64_t)((uint64_t)RC_U64X(s3) + (uint64_t)0ULL));
L_13bac74:;
    { uint32_t bits = A1_R32B((uint32_t)sp, 184); float f; std::memcpy(&f, &bits, sizeof(f)); f20 = f; }
    { (sp = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)sp, 192)))); io_vf5 = vf5; io_vf6 = vf6; io_vf7 = vf7; io_vf8 = vf8; io_vf10 = vf10; io_vf14 = vf14; io_vf15 = vf15; io_vf16 = vf16; io_vf17 = vf17; io_vf24 = vf24; io_vf25 = vf25; io_vf26 = vf26; io_vf27 = vf27; io_vf28 = vf28; io_vf29 = vf29; io_vacc = vacc; io_h2 = h2; io_h6 = h6; io_h7 = h7; io_h8 = h8; io_h9 = h9; io_h10 = h10; io_h11 = h11; io_h12 = h12; io_h13 = h13; io_h14 = h14; io_h15 = h15; return a1::RetVF{(uint64_t)v0, f0}; }
    __builtin_unreachable();
}

// gamecore_sub_013BB4A0 (0x13bb4a0): typed by tools/typed_lift2.py (round A6; guest frame calls; VU0 in vf0 vf10 vf25 vf28 vf29 vacc, out vf5 vf6 vf7 vf8 vf10 vf14 vf15 vf16 vf17 vf24 vf25 vf26 vf28 vf29 vacc h2 h6 h7 h8 h9 h10 h11 h12 h13 h14 h15, relocated calls 2)
inline a1::RetVF gamecore_sub_013BB4A0(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint64_t v0, uint64_t a0, uint64_t a1, uint64_t sp, float f0, float f1, float f2, float f3, float f4, float f5, float f6, float f7, float f8, float f9, float f10, float f11, float f12, float f13, float f14, float f15, float f16, float f17, float f18, float f19, float f20, float f21, float f22, float f23, float f24, float f25, float f26, float f27, float f28, float f29, float f30, float f31, const a4::vec4 &io_vf0, a4::vec4 &io_vf5, a4::vec4 &io_vf6, a4::vec4 &io_vf7, a4::vec4 &io_vf8, a4::vec4 &io_vf10, a4::vec4 &io_vf14, a4::vec4 &io_vf15, a4::vec4 &io_vf16, a4::vec4 &io_vf17, a4::vec4 &io_vf24, a4::vec4 &io_vf25, a4::vec4 &io_vf26, a4::vec4 &io_vf28, a4::vec4 &io_vf29, a4::vec4 &io_vacc, uint64_t &io_h2, uint64_t &io_h6, uint64_t &io_h7, uint64_t &io_h8, uint64_t &io_h9, uint64_t &io_h10, uint64_t &io_h11, uint64_t &io_h12, uint64_t &io_h13, uint64_t &io_h14, uint64_t &io_h15)
{
    uint64_t at = 0;
    uint64_t v1 = 0;
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
    a4::vec4 vf24 = io_vf24;
    a4::vec4 vf25 = io_vf25;
    a4::vec4 vf26 = io_vf26;
    a4::vec4 vf28 = io_vf28;
    a4::vec4 vf29 = io_vf29;
    a4::vec4 vacc = io_vacc;
    uint64_t h2 = io_h2;
    uint64_t h6 = io_h6;
    uint64_t h7 = io_h7;
    uint64_t h8 = io_h8;
    uint64_t h9 = io_h9;
    uint64_t h10 = io_h10;
    uint64_t h11 = io_h11;
    uint64_t h12 = io_h12;
    uint64_t h13 = io_h13;
    uint64_t h14 = io_h14;
    uint64_t h15 = io_h15;
    (sp = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)sp, 4294967120))));
    (s0 = (uint64_t)((uint64_t)a0 + (uint64_t)0ULL));
    (a0 = (uint64_t)((uint64_t)a1 + (uint64_t)0ULL));
    { (s2 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL)); A3_CHARGE(); v0 = GetLayoutClass_u11(ram, rt, a0, f0, f1, f2, f3, f4, f5, f6, f7, f8, f9, f10, f11, f12, f13, f14, f15, f16, f17, f18, f19, f20, f21, f22, f23, f24, f25, f26, f27, f28, f29, f30, f31, h2); }
    (a1 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)v0, 344))));
    (s1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)sp, 64))));
    { uint32_t bits = 0u; std::memcpy(&f13, &bits, sizeof(bits)); }
    (a0 = (uint64_t)((uint64_t)RC_U64X(s1) + (uint64_t)0ULL));
    (at = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)15897 << 16))));
    (at = (uint64_t)(RC_U64X(at) | (uint64_t)(uint16_t)39321));
    { uint32_t bits = (uint32_t)at; std::memcpy(&f12, &bits, sizeof(bits)); }
    (a1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)a1, 1536))));
    { f14 = ((float)(f13)); A3_CHARGE(); { const a1::RetVF a2_r = iosTransMatrixF(ram, rt, (uint32_t)a0, (uint32_t)a1, (uint32_t)sp, f12, f13, f14, vf8, vf10, vf14, vf15, vf16, vf17, vacc, h6, h7, h8, h9); v0 = a2_r.v0; f0 = a2_r.f0; } }
    vf24 = a4v::castsi128_ps(A4_R128(ADD32((uint32_t)s1, 48)));
    { a4::vec4 res = a4v::subK<1, 1>(vf0, a4v::bcast<3>(vf0)); a4::q128 mask = a4v::set_epi32(0, 0, 0, -1); vf25 = a4v::blendv_ps(vf25, res, a4v::castsi128_ps(mask)); }
    { a4::vec4 res = a4v::mulK<0, 1>(vf24, a4v::bcast<0>(vf25)); a4::q128 mask = a4v::set_epi32(0, -1, -1, -1); vf28 = a4v::blendv_ps(vf28, res, a4v::castsi128_ps(mask)); }
    { a4::vec4 res = a4v::sub(vf28, vf28); a4::q128 mask = a4v::set_epi32(-1, 0, 0, 0); vf28 = a4v::blend(vf28, res, a4v::castsi128_ps(mask)); }
    do { const a4::q128 _v = (A4_R128(ADD32((uint32_t)s1, 0))); h8 = a4::hi64(_v); std::memcpy(&t0, &_v, 8); } while (0);
    do { const a4::q128 _v = (A4_R128(ADD32((uint32_t)s1, 16))); h9 = a4::hi64(_v); std::memcpy(&t1, &_v, 8); } while (0);
    do { const a4::q128 _v = (A4_R128(ADD32((uint32_t)s1, 32))); h10 = a4::hi64(_v); std::memcpy(&t2, &_v, 8); } while (0);
    do { const a4::q128 _v = (a4v::castps_si128(vf0)); h11 = a4::hi64(_v); std::memcpy(&t3, &_v, 8); } while (0);
    do { const a4::q128 _v = (a4m::pextlw(a4::mk128(h9, t1), a4::mk128(h8, t0))); h12 = a4::hi64(_v); std::memcpy(&t4, &_v, 8); } while (0);
    do { const a4::q128 _v = (a4m::pextuw(a4::mk128(h9, t1), a4::mk128(h8, t0))); h13 = a4::hi64(_v); std::memcpy(&t5, &_v, 8); } while (0);
    do { const a4::q128 _v = (a4m::pextlw(a4::mk128(h11, t3), a4::mk128(h10, t2))); h14 = a4::hi64(_v); std::memcpy(&t6, &_v, 8); } while (0);
    do { const a4::q128 _v = (a4m::pextuw(a4::mk128(h11, t3), a4::mk128(h10, t2))); h15 = a4::hi64(_v); std::memcpy(&t7, &_v, 8); } while (0);
    do { const a4::q128 _v = (a4m::pcpyld(a4::mk128(h14, t6), a4::mk128(h12, t4))); h8 = a4::hi64(_v); std::memcpy(&t0, &_v, 8); } while (0);
    do { const a4::q128 _v = (a4v::unpackhi_epi64(a4::mk128(h12, t4), a4::mk128(h14, t6))); h9 = a4::hi64(_v); std::memcpy(&t1, &_v, 8); } while (0);
    do { const a4::q128 _v = (a4m::pcpyld(a4::mk128(h15, t7), a4::mk128(h13, t5))); h10 = a4::hi64(_v); std::memcpy(&t2, &_v, 8); } while (0);
    do { const a4::q128 _v = (a4v::unpackhi_epi64(a4::mk128(h13, t5), a4::mk128(h15, t7))); h11 = a4::hi64(_v); std::memcpy(&t3, &_v, 8); } while (0);
    vf24 = a4v::castsi128_ps(a4::mk128(h8, t0));
    vf25 = a4v::castsi128_ps(a4::mk128(h9, t1));
    vf26 = a4v::castsi128_ps(a4::mk128(h10, t2));
    { a4::vec4 res = a4v::mulK<0, 1>(vf24, a4v::bcast<0>(vf28)); vacc = a4v::dest<1, 1, 1, 1>(vacc, res); }
    { a4::vec4 mul_res = a4v::mulK<0, 1>(vf25, a4v::bcast<1>(vf28)); a4::vec4 res = a4v::addK<1, 1>(vacc, mul_res); vacc = a4v::dest<1, 1, 1, 1>(vacc, res); }
    { a4::vec4 mul_res = a4v::mulK<0, 1>(vf26, a4v::bcast<2>(vf28)); a4::vec4 res = a4v::addK<1, 1>(vacc, mul_res); vacc = a4v::dest<1, 1, 1, 1>(vacc, res); }
    { a4::vec4 mul_res = a4v::mulK<1, 1>(vf0, a4v::bcast<3>(vf28)); a4::vec4 res = a4v::addK<1, 1>(vacc, mul_res); a4::q128 mask = a4v::set_epi32(-1, -1, -1, -1); vf29 = a4v::blendv_ps(vf29, res, a4v::castsi128_ps(mask)); }
    { a4::vec4 res = a4v::subK<1, 1>(vf29, vf29); a4::q128 mask = a4v::set_epi32(-1, 0, 0, 0); vf29 = a4v::blend(vf29, res, a4v::castsi128_ps(mask)); }
    { a4::vec4 res = a4v::addK<1, 1>(vf29, vf0); a4::q128 mask = a4v::set_epi32(-1, 0, 0, 0); vf29 = a4v::blend(vf29, res, a4v::castsi128_ps(mask)); }
    A4_W128(ADD32((uint32_t)sp, 0), a4v::castps_si128(vf24));
    A4_W128(ADD32((uint32_t)sp, 16), a4v::castps_si128(vf25));
    A4_W128(ADD32((uint32_t)sp, 32), a4v::castps_si128(vf26));
    A4_W128(ADD32((uint32_t)sp, 48), a4v::castps_si128(vf29));
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)sp, 128))));
    vf8 = a4v::castsi128_ps(A4_R128(ADD32((uint32_t)s0, 0)));
    vf14 = a4v::castsi128_ps(A4_R128(ADD32((uint32_t)sp, 0)));
    vf15 = a4v::castsi128_ps(A4_R128(ADD32((uint32_t)sp, 16)));
    vf16 = a4v::castsi128_ps(A4_R128(ADD32((uint32_t)sp, 32)));
    vf17 = a4v::castsi128_ps(A4_R128(ADD32((uint32_t)sp, 48)));
    { a4::vec4 res = a4v::mul(vf14, a4v::bcast<0>(vf8)); vacc = a4v::dest<1, 1, 1, 1>(vacc, res); }
    { a4::vec4 mul_res = a4v::mul(vf15, a4v::bcast<1>(vf8)); a4::vec4 res = a4v::addK<1, 1>(vacc, mul_res); vacc = a4v::dest<1, 1, 1, 1>(vacc, res); }
    { a4::vec4 mul_res = a4v::mul(vf16, a4v::bcast<2>(vf8)); a4::vec4 res = a4v::addK<1, 1>(vacc, mul_res); vacc = a4v::dest<1, 1, 1, 1>(vacc, res); }
    { a4::vec4 mul_res = a4v::mul(vf17, a4v::bcast<3>(vf8)); a4::vec4 res = a4v::addK<1, 1>(vacc, mul_res); a4::q128 mask = a4v::set_epi32(-1, -1, -1, -1); vf10 = a4v::blendv_ps(vf10, res, a4v::castsi128_ps(mask)); }
    A4_W128(ADD32((uint32_t)v1, 0), a4v::castps_si128(vf10));
    vf7 = a4v::castsi128_ps(A4_R128(ADD32((uint32_t)v1, 0)));
    { a4::vec4 res = a4v::mul(vf7, vf7); a4::q128 mask = a4v::set_epi32(0, 0, -1, -1); vf7 = a4v::blend(vf7, res, a4v::castsi128_ps(mask)); }
    { a4::vec4 res = a4v::addK<1, 1>(vf7, a4v::bcast<1>(vf7)); a4::q128 mask = a4v::set_epi32(0, 0, 0, -1); vf7 = a4v::blendv_ps(vf7, res, a4v::castsi128_ps(mask)); }
    do { const a4::q128 _v = (a4v::castps_si128(vf7)); h2 = a4::hi64(_v); std::memcpy(&v0, &_v, 8); } while (0);
    { uint32_t bits = (uint32_t)v0; std::memcpy(&f1, &bits, sizeof(bits)); }
    (at = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)15620 << 16))));
    (at = (uint64_t)(RC_U64X(at) | (uint64_t)(uint16_t)46554));
    { uint32_t bits = (uint32_t)at; std::memcpy(&f0, &bits, sizeof(bits)); }
    fcr31 = (a1f::lt(f1, f0)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
    { const bool c = (fcr31 & 0x800000u) == 0u; (v0 = (uint64_t)((uint64_t)s2 + (uint64_t)0ULL)); if (c) goto L_13bb618; }
    f1 = a1f::sqrt(fcr31, f1);
    (at = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)15928 << 16))));
    (at = (uint64_t)(RC_U64X(at) | (uint64_t)(uint16_t)20970));
    { uint32_t bits = (uint32_t)at; std::memcpy(&f0, &bits, sizeof(bits)); }
    f0 = a1f::div(fcr31, f0, f1);
    vf5 = a4v::castsi128_ps(A4_R128(ADD32((uint32_t)v1, 0)));
    { uint32_t bits; std::memcpy(&bits, &f0, sizeof(bits)); (t0 = (uint64_t)((int64_t)(int32_t)(bits))); }
    vf6 = a4v::castsi128_ps(a4::mk128(h8, t0));
    { a4::vec4 res = a4v::mul(vf5, a4v::bcast<0>(vf6)); a4::q128 mask = a4v::set_epi32(0, 0, -1, -1); vf5 = a4v::blendv_ps(vf5, res, a4v::castsi128_ps(mask)); }
    A4_W128(ADD32((uint32_t)v1, 0), a4v::castps_si128(vf5));
    vf8 = a4v::castsi128_ps(A4_R128(ADD32((uint32_t)v1, 0)));
    vf14 = a4v::castsi128_ps(A4_R128(ADD32((uint32_t)s1, 0)));
    vf15 = a4v::castsi128_ps(A4_R128(ADD32((uint32_t)s1, 16)));
    vf16 = a4v::castsi128_ps(A4_R128(ADD32((uint32_t)s1, 32)));
    vf17 = a4v::castsi128_ps(A4_R128(ADD32((uint32_t)s1, 48)));
    { a4::vec4 res = a4v::mul(vf14, a4v::bcast<0>(vf8)); vacc = a4v::dest<1, 1, 1, 1>(vacc, res); }
    { a4::vec4 mul_res = a4v::mul(vf15, a4v::bcast<1>(vf8)); a4::vec4 res = a4v::addK<1, 1>(vacc, mul_res); vacc = a4v::dest<1, 1, 1, 1>(vacc, res); }
    { a4::vec4 mul_res = a4v::mul(vf16, a4v::bcast<2>(vf8)); a4::vec4 res = a4v::addK<1, 1>(vacc, mul_res); vacc = a4v::dest<1, 1, 1, 1>(vacc, res); }
    { a4::vec4 mul_res = a4v::mul(vf17, a4v::bcast<3>(vf8)); a4::vec4 res = a4v::addK<1, 1>(vacc, mul_res); a4::q128 mask = a4v::set_epi32(-1, -1, -1, -1); vf10 = a4v::blendv_ps(vf10, res, a4v::castsi128_ps(mask)); }
    A4_W128(ADD32((uint32_t)s0, 0), a4v::castps_si128(vf10));
    (s2 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 1))));
    (v0 = (uint64_t)((uint64_t)RC_U64X(s2) + (uint64_t)0ULL));
L_13bb618:;
    { (sp = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)sp, 176)))); io_vf5 = vf5; io_vf6 = vf6; io_vf7 = vf7; io_vf8 = vf8; io_vf10 = vf10; io_vf14 = vf14; io_vf15 = vf15; io_vf16 = vf16; io_vf17 = vf17; io_vf24 = vf24; io_vf25 = vf25; io_vf26 = vf26; io_vf28 = vf28; io_vf29 = vf29; io_vacc = vacc; io_h2 = h2; io_h6 = h6; io_h7 = h7; io_h8 = h8; io_h9 = h9; io_h10 = h10; io_h11 = h11; io_h12 = h12; io_h13 = h13; io_h14 = h14; io_h15 = h15; return a1::RetVF{(uint64_t)v0, f0}; }
    __builtin_unreachable();
}

// gamecore_sub_013BB998 (0x13bb998): typed by tools/typed_lift2.py (round A6; guest frame calls; VU0 in vf0 vf10 vf25 vf28 vf29 vacc, out vf5 vf6 vf7 vf8 vf10 vf14 vf15 vf16 vf17 vf24 vf25 vf26 vf28 vf29 vacc h2 h6 h7 h8 h9 h10 h11 h12 h13 h14 h15, relocated calls 3)
inline a1::RetVF gamecore_sub_013BB998(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint64_t v0, uint64_t a0, uint64_t a1, uint64_t sp, float f0, float f1, float f2, float f3, float f4, float f5, float f6, float f7, float f8, float f9, float f10, float f11, float f12, float f13, float f14, float f15, float f16, float f17, float f18, float f19, float f20, float f21, float f22, float f23, float f24, float f25, float f26, float f27, float f28, float f29, float f30, float f31, const a4::vec4 &io_vf0, a4::vec4 &io_vf5, a4::vec4 &io_vf6, a4::vec4 &io_vf7, a4::vec4 &io_vf8, a4::vec4 &io_vf10, a4::vec4 &io_vf14, a4::vec4 &io_vf15, a4::vec4 &io_vf16, a4::vec4 &io_vf17, a4::vec4 &io_vf24, a4::vec4 &io_vf25, a4::vec4 &io_vf26, a4::vec4 &io_vf28, a4::vec4 &io_vf29, a4::vec4 &io_vacc, uint64_t &io_h2, uint64_t &io_h6, uint64_t &io_h7, uint64_t &io_h8, uint64_t &io_h9, uint64_t &io_h10, uint64_t &io_h11, uint64_t &io_h12, uint64_t &io_h13, uint64_t &io_h14, uint64_t &io_h15)
{
    uint64_t at = 0;
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
    a4::vec4 vf24 = io_vf24;
    a4::vec4 vf25 = io_vf25;
    a4::vec4 vf26 = io_vf26;
    a4::vec4 vf28 = io_vf28;
    a4::vec4 vf29 = io_vf29;
    a4::vec4 vacc = io_vacc;
    uint64_t h2 = io_h2;
    uint64_t h6 = io_h6;
    uint64_t h7 = io_h7;
    uint64_t h8 = io_h8;
    uint64_t h9 = io_h9;
    uint64_t h10 = io_h10;
    uint64_t h11 = io_h11;
    uint64_t h12 = io_h12;
    uint64_t h13 = io_h13;
    uint64_t h14 = io_h14;
    uint64_t h15 = io_h15;
    (sp = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)sp, 4294967088))));
    (s1 = (uint64_t)((uint64_t)a0 + (uint64_t)0ULL));
    (a0 = (uint64_t)((uint64_t)a1 + (uint64_t)0ULL));
    (s3 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL));
    { float f = f20; uint32_t bits; std::memcpy(&bits, &f, sizeof(bits)); A1_W32B((uint32_t)sp, 192, bits); }
    { { float f = f21; uint32_t bits; std::memcpy(&bits, &f, sizeof(bits)); A1_W32B((uint32_t)sp, 200, bits); } A3_CHARGE(); v0 = GetLayoutClass_u11(ram, rt, a0, f0, f1, f2, f3, f4, f5, f6, f7, f8, f9, f10, f11, f12, f13, f14, f15, f16, f17, f18, f19, f20, f21, f22, f23, f24, f25, f26, f27, f28, f29, f30, f31, h2); }
    (s4 = (uint64_t)((uint64_t)v0 + (uint64_t)0ULL));
    { uint32_t bits = 0u; std::memcpy(&f20, &bits, sizeof(bits)); }
    (a1 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)s4, 344))));
    (s2 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)sp, 64))));
    (at = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)15897 << 16))));
    (at = (uint64_t)(RC_U64X(at) | (uint64_t)(uint16_t)39321));
    { uint32_t bits = (uint32_t)at; std::memcpy(&f12, &bits, sizeof(bits)); }
    (a0 = (uint64_t)((uint64_t)RC_U64X(s2) + (uint64_t)0ULL));
    (a1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)a1, 1536))));
    f13 = ((float)(f20));
    { f14 = ((float)(f20)); A3_CHARGE(); { const a1::RetVF a2_r = iosTransMatrixF(ram, rt, (uint32_t)a0, (uint32_t)a1, (uint32_t)sp, f12, f13, f14, vf8, vf10, vf14, vf15, vf16, vf17, vacc, h6, h7, h8, h9); v0 = a2_r.v0; f0 = a2_r.f0; } }
    vf24 = a4v::castsi128_ps(A4_R128(ADD32((uint32_t)s2, 48)));
    { a4::vec4 res = a4v::subK<1, 1>(vf0, a4v::bcast<3>(vf0)); a4::q128 mask = a4v::set_epi32(0, 0, 0, -1); vf25 = a4v::blendv_ps(vf25, res, a4v::castsi128_ps(mask)); }
    { a4::vec4 res = a4v::mulK<0, 1>(vf24, a4v::bcast<0>(vf25)); a4::q128 mask = a4v::set_epi32(0, -1, -1, -1); vf28 = a4v::blendv_ps(vf28, res, a4v::castsi128_ps(mask)); }
    { a4::vec4 res = a4v::sub(vf28, vf28); a4::q128 mask = a4v::set_epi32(-1, 0, 0, 0); vf28 = a4v::blend(vf28, res, a4v::castsi128_ps(mask)); }
    do { const a4::q128 _v = (A4_R128(ADD32((uint32_t)s2, 0))); h8 = a4::hi64(_v); std::memcpy(&t0, &_v, 8); } while (0);
    do { const a4::q128 _v = (A4_R128(ADD32((uint32_t)s2, 16))); h9 = a4::hi64(_v); std::memcpy(&t1, &_v, 8); } while (0);
    do { const a4::q128 _v = (A4_R128(ADD32((uint32_t)s2, 32))); h10 = a4::hi64(_v); std::memcpy(&t2, &_v, 8); } while (0);
    do { const a4::q128 _v = (a4v::castps_si128(vf0)); h11 = a4::hi64(_v); std::memcpy(&t3, &_v, 8); } while (0);
    do { const a4::q128 _v = (a4m::pextlw(a4::mk128(h9, t1), a4::mk128(h8, t0))); h12 = a4::hi64(_v); std::memcpy(&t4, &_v, 8); } while (0);
    do { const a4::q128 _v = (a4m::pextuw(a4::mk128(h9, t1), a4::mk128(h8, t0))); h13 = a4::hi64(_v); std::memcpy(&t5, &_v, 8); } while (0);
    do { const a4::q128 _v = (a4m::pextlw(a4::mk128(h11, t3), a4::mk128(h10, t2))); h14 = a4::hi64(_v); std::memcpy(&t6, &_v, 8); } while (0);
    do { const a4::q128 _v = (a4m::pextuw(a4::mk128(h11, t3), a4::mk128(h10, t2))); h15 = a4::hi64(_v); std::memcpy(&t7, &_v, 8); } while (0);
    do { const a4::q128 _v = (a4m::pcpyld(a4::mk128(h14, t6), a4::mk128(h12, t4))); h8 = a4::hi64(_v); std::memcpy(&t0, &_v, 8); } while (0);
    do { const a4::q128 _v = (a4v::unpackhi_epi64(a4::mk128(h12, t4), a4::mk128(h14, t6))); h9 = a4::hi64(_v); std::memcpy(&t1, &_v, 8); } while (0);
    do { const a4::q128 _v = (a4m::pcpyld(a4::mk128(h15, t7), a4::mk128(h13, t5))); h10 = a4::hi64(_v); std::memcpy(&t2, &_v, 8); } while (0);
    do { const a4::q128 _v = (a4v::unpackhi_epi64(a4::mk128(h13, t5), a4::mk128(h15, t7))); h11 = a4::hi64(_v); std::memcpy(&t3, &_v, 8); } while (0);
    vf24 = a4v::castsi128_ps(a4::mk128(h8, t0));
    vf25 = a4v::castsi128_ps(a4::mk128(h9, t1));
    vf26 = a4v::castsi128_ps(a4::mk128(h10, t2));
    { a4::vec4 res = a4v::mulK<0, 1>(vf24, a4v::bcast<0>(vf28)); vacc = a4v::dest<1, 1, 1, 1>(vacc, res); }
    { a4::vec4 mul_res = a4v::mulK<0, 1>(vf25, a4v::bcast<1>(vf28)); a4::vec4 res = a4v::addK<1, 1>(vacc, mul_res); vacc = a4v::dest<1, 1, 1, 1>(vacc, res); }
    { a4::vec4 mul_res = a4v::mulK<0, 1>(vf26, a4v::bcast<2>(vf28)); a4::vec4 res = a4v::addK<1, 1>(vacc, mul_res); vacc = a4v::dest<1, 1, 1, 1>(vacc, res); }
    { a4::vec4 mul_res = a4v::mulK<1, 1>(vf0, a4v::bcast<3>(vf28)); a4::vec4 res = a4v::addK<1, 1>(vacc, mul_res); a4::q128 mask = a4v::set_epi32(-1, -1, -1, -1); vf29 = a4v::blendv_ps(vf29, res, a4v::castsi128_ps(mask)); }
    { a4::vec4 res = a4v::subK<1, 1>(vf29, vf29); a4::q128 mask = a4v::set_epi32(-1, 0, 0, 0); vf29 = a4v::blend(vf29, res, a4v::castsi128_ps(mask)); }
    { a4::vec4 res = a4v::addK<1, 1>(vf29, vf0); a4::q128 mask = a4v::set_epi32(-1, 0, 0, 0); vf29 = a4v::blend(vf29, res, a4v::castsi128_ps(mask)); }
    A4_W128(ADD32((uint32_t)sp, 0), a4v::castps_si128(vf24));
    A4_W128(ADD32((uint32_t)sp, 16), a4v::castps_si128(vf25));
    A4_W128(ADD32((uint32_t)sp, 32), a4v::castps_si128(vf26));
    A4_W128(ADD32((uint32_t)sp, 48), a4v::castps_si128(vf29));
    (s0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)sp, 128))));
    vf8 = a4v::castsi128_ps(A4_R128(ADD32((uint32_t)s1, 0)));
    vf14 = a4v::castsi128_ps(A4_R128(ADD32((uint32_t)sp, 0)));
    vf15 = a4v::castsi128_ps(A4_R128(ADD32((uint32_t)sp, 16)));
    vf16 = a4v::castsi128_ps(A4_R128(ADD32((uint32_t)sp, 32)));
    vf17 = a4v::castsi128_ps(A4_R128(ADD32((uint32_t)sp, 48)));
    { a4::vec4 res = a4v::mul(vf14, a4v::bcast<0>(vf8)); vacc = a4v::dest<1, 1, 1, 1>(vacc, res); }
    { a4::vec4 mul_res = a4v::mul(vf15, a4v::bcast<1>(vf8)); a4::vec4 res = a4v::addK<1, 1>(vacc, mul_res); vacc = a4v::dest<1, 1, 1, 1>(vacc, res); }
    { a4::vec4 mul_res = a4v::mul(vf16, a4v::bcast<2>(vf8)); a4::vec4 res = a4v::addK<1, 1>(vacc, mul_res); vacc = a4v::dest<1, 1, 1, 1>(vacc, res); }
    { a4::vec4 mul_res = a4v::mul(vf17, a4v::bcast<3>(vf8)); a4::vec4 res = a4v::addK<1, 1>(vacc, mul_res); a4::q128 mask = a4v::set_epi32(-1, -1, -1, -1); vf10 = a4v::blendv_ps(vf10, res, a4v::castsi128_ps(mask)); }
    A4_W128(ADD32((uint32_t)s0, 0), a4v::castps_si128(vf10));
    vf7 = a4v::castsi128_ps(A4_R128(ADD32((uint32_t)s0, 0)));
    { a4::vec4 res = a4v::mul(vf7, vf7); a4::q128 mask = a4v::set_epi32(0, 0, -1, -1); vf7 = a4v::blend(vf7, res, a4v::castsi128_ps(mask)); }
    { a4::vec4 res = a4v::addK<1, 1>(vf7, a4v::bcast<1>(vf7)); a4::q128 mask = a4v::set_epi32(0, 0, 0, -1); vf7 = a4v::blendv_ps(vf7, res, a4v::castsi128_ps(mask)); }
    do { const a4::q128 _v = (a4v::castps_si128(vf7)); h2 = a4::hi64(_v); std::memcpy(&v0, &_v, 8); } while (0);
    { uint32_t bits = (uint32_t)v0; std::memcpy(&f0, &bits, sizeof(bits)); }
    (at = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)15620 << 16))));
    (at = (uint64_t)(RC_U64X(at) | (uint64_t)(uint16_t)46554));
    { uint32_t bits = (uint32_t)at; std::memcpy(&f21, &bits, sizeof(bits)); }
    fcr31 = (a1f::lt(f0, f21)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
    if ((fcr31 & 0x800000u) == 0u) { (a1 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)s4, 344)))); goto L_13bbb28; }
    f1 = a1f::sqrt(fcr31, f0);
    (at = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)15928 << 16))));
    (at = (uint64_t)(RC_U64X(at) | (uint64_t)(uint16_t)20970));
    { uint32_t bits = (uint32_t)at; std::memcpy(&f0, &bits, sizeof(bits)); }
    f0 = a1f::div(fcr31, f0, f1);
    vf5 = a4v::castsi128_ps(A4_R128(ADD32((uint32_t)s0, 0)));
    { uint32_t bits; std::memcpy(&bits, &f0, sizeof(bits)); (t0 = (uint64_t)((int64_t)(int32_t)(bits))); }
    vf6 = a4v::castsi128_ps(a4::mk128(h8, t0));
    { a4::vec4 res = a4v::mul(vf5, a4v::bcast<0>(vf6)); a4::q128 mask = a4v::set_epi32(0, 0, -1, -1); vf5 = a4v::blendv_ps(vf5, res, a4v::castsi128_ps(mask)); }
    A4_W128(ADD32((uint32_t)s0, 0), a4v::castps_si128(vf5));
    vf8 = a4v::castsi128_ps(A4_R128(ADD32((uint32_t)s0, 0)));
    vf14 = a4v::castsi128_ps(A4_R128(ADD32((uint32_t)s2, 0)));
    vf15 = a4v::castsi128_ps(A4_R128(ADD32((uint32_t)s2, 16)));
    vf16 = a4v::castsi128_ps(A4_R128(ADD32((uint32_t)s2, 32)));
    vf17 = a4v::castsi128_ps(A4_R128(ADD32((uint32_t)s2, 48)));
    { a4::vec4 res = a4v::mul(vf14, a4v::bcast<0>(vf8)); vacc = a4v::dest<1, 1, 1, 1>(vacc, res); }
    { a4::vec4 mul_res = a4v::mul(vf15, a4v::bcast<1>(vf8)); a4::vec4 res = a4v::addK<1, 1>(vacc, mul_res); vacc = a4v::dest<1, 1, 1, 1>(vacc, res); }
    { a4::vec4 mul_res = a4v::mul(vf16, a4v::bcast<2>(vf8)); a4::vec4 res = a4v::addK<1, 1>(vacc, mul_res); vacc = a4v::dest<1, 1, 1, 1>(vacc, res); }
    { a4::vec4 mul_res = a4v::mul(vf17, a4v::bcast<3>(vf8)); a4::vec4 res = a4v::addK<1, 1>(vacc, mul_res); a4::q128 mask = a4v::set_epi32(-1, -1, -1, -1); vf10 = a4v::blendv_ps(vf10, res, a4v::castsi128_ps(mask)); }
    A4_W128(ADD32((uint32_t)s1, 0), a4v::castps_si128(vf10));
    (s3 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 1))));
    (a1 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)s4, 344))));
L_13bbb28:;
    f13 = ((float)(f20));
    (at = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)48665 << 16))));
    (at = (uint64_t)(RC_U64X(at) | (uint64_t)(uint16_t)39321));
    { uint32_t bits = (uint32_t)at; std::memcpy(&f12, &bits, sizeof(bits)); }
    (a0 = (uint64_t)((uint64_t)s2 + (uint64_t)0ULL));
    (a1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)a1, 1024))));
    { f14 = ((float)(f13)); A3_CHARGE(); { const a1::RetVF a2_r = iosTransMatrixF(ram, rt, (uint32_t)a0, (uint32_t)a1, (uint32_t)sp, f12, f13, f14, vf8, vf10, vf14, vf15, vf16, vf17, vacc, h6, h7, h8, h9); v0 = a2_r.v0; f0 = a2_r.f0; } }
    vf24 = a4v::castsi128_ps(A4_R128(ADD32((uint32_t)s2, 48)));
    { a4::vec4 res = a4v::subK<1, 1>(vf0, a4v::bcast<3>(vf0)); a4::q128 mask = a4v::set_epi32(0, 0, 0, -1); vf25 = a4v::blendv_ps(vf25, res, a4v::castsi128_ps(mask)); }
    { a4::vec4 res = a4v::mulK<0, 1>(vf24, a4v::bcast<0>(vf25)); a4::q128 mask = a4v::set_epi32(0, -1, -1, -1); vf28 = a4v::blendv_ps(vf28, res, a4v::castsi128_ps(mask)); }
    { a4::vec4 res = a4v::sub(vf28, vf28); a4::q128 mask = a4v::set_epi32(-1, 0, 0, 0); vf28 = a4v::blend(vf28, res, a4v::castsi128_ps(mask)); }
    do { const a4::q128 _v = (A4_R128(ADD32((uint32_t)s2, 0))); h8 = a4::hi64(_v); std::memcpy(&t0, &_v, 8); } while (0);
    do { const a4::q128 _v = (A4_R128(ADD32((uint32_t)s2, 16))); h9 = a4::hi64(_v); std::memcpy(&t1, &_v, 8); } while (0);
    do { const a4::q128 _v = (A4_R128(ADD32((uint32_t)s2, 32))); h10 = a4::hi64(_v); std::memcpy(&t2, &_v, 8); } while (0);
    do { const a4::q128 _v = (a4v::castps_si128(vf0)); h11 = a4::hi64(_v); std::memcpy(&t3, &_v, 8); } while (0);
    do { const a4::q128 _v = (a4m::pextlw(a4::mk128(h9, t1), a4::mk128(h8, t0))); h12 = a4::hi64(_v); std::memcpy(&t4, &_v, 8); } while (0);
    do { const a4::q128 _v = (a4m::pextuw(a4::mk128(h9, t1), a4::mk128(h8, t0))); h13 = a4::hi64(_v); std::memcpy(&t5, &_v, 8); } while (0);
    do { const a4::q128 _v = (a4m::pextlw(a4::mk128(h11, t3), a4::mk128(h10, t2))); h14 = a4::hi64(_v); std::memcpy(&t6, &_v, 8); } while (0);
    do { const a4::q128 _v = (a4m::pextuw(a4::mk128(h11, t3), a4::mk128(h10, t2))); h15 = a4::hi64(_v); std::memcpy(&t7, &_v, 8); } while (0);
    do { const a4::q128 _v = (a4m::pcpyld(a4::mk128(h14, t6), a4::mk128(h12, t4))); h8 = a4::hi64(_v); std::memcpy(&t0, &_v, 8); } while (0);
    do { const a4::q128 _v = (a4v::unpackhi_epi64(a4::mk128(h12, t4), a4::mk128(h14, t6))); h9 = a4::hi64(_v); std::memcpy(&t1, &_v, 8); } while (0);
    do { const a4::q128 _v = (a4m::pcpyld(a4::mk128(h15, t7), a4::mk128(h13, t5))); h10 = a4::hi64(_v); std::memcpy(&t2, &_v, 8); } while (0);
    do { const a4::q128 _v = (a4v::unpackhi_epi64(a4::mk128(h13, t5), a4::mk128(h15, t7))); h11 = a4::hi64(_v); std::memcpy(&t3, &_v, 8); } while (0);
    vf24 = a4v::castsi128_ps(a4::mk128(h8, t0));
    vf25 = a4v::castsi128_ps(a4::mk128(h9, t1));
    vf26 = a4v::castsi128_ps(a4::mk128(h10, t2));
    { a4::vec4 res = a4v::mulK<0, 1>(vf24, a4v::bcast<0>(vf28)); vacc = a4v::dest<1, 1, 1, 1>(vacc, res); }
    { a4::vec4 mul_res = a4v::mulK<0, 1>(vf25, a4v::bcast<1>(vf28)); a4::vec4 res = a4v::addK<1, 1>(vacc, mul_res); vacc = a4v::dest<1, 1, 1, 1>(vacc, res); }
    { a4::vec4 mul_res = a4v::mulK<0, 1>(vf26, a4v::bcast<2>(vf28)); a4::vec4 res = a4v::addK<1, 1>(vacc, mul_res); vacc = a4v::dest<1, 1, 1, 1>(vacc, res); }
    { a4::vec4 mul_res = a4v::mulK<1, 1>(vf0, a4v::bcast<3>(vf28)); a4::vec4 res = a4v::addK<1, 1>(vacc, mul_res); a4::q128 mask = a4v::set_epi32(-1, -1, -1, -1); vf29 = a4v::blendv_ps(vf29, res, a4v::castsi128_ps(mask)); }
    { a4::vec4 res = a4v::subK<1, 1>(vf29, vf29); a4::q128 mask = a4v::set_epi32(-1, 0, 0, 0); vf29 = a4v::blend(vf29, res, a4v::castsi128_ps(mask)); }
    { a4::vec4 res = a4v::addK<1, 1>(vf29, vf0); a4::q128 mask = a4v::set_epi32(-1, 0, 0, 0); vf29 = a4v::blend(vf29, res, a4v::castsi128_ps(mask)); }
    A4_W128(ADD32((uint32_t)sp, 0), a4v::castps_si128(vf24));
    A4_W128(ADD32((uint32_t)sp, 16), a4v::castps_si128(vf25));
    A4_W128(ADD32((uint32_t)sp, 32), a4v::castps_si128(vf26));
    A4_W128(ADD32((uint32_t)sp, 48), a4v::castps_si128(vf29));
    vf8 = a4v::castsi128_ps(A4_R128(ADD32((uint32_t)s1, 0)));
    vf14 = a4v::castsi128_ps(A4_R128(ADD32((uint32_t)sp, 0)));
    vf15 = a4v::castsi128_ps(A4_R128(ADD32((uint32_t)sp, 16)));
    vf16 = a4v::castsi128_ps(A4_R128(ADD32((uint32_t)sp, 32)));
    vf17 = a4v::castsi128_ps(A4_R128(ADD32((uint32_t)sp, 48)));
    { a4::vec4 res = a4v::mul(vf14, a4v::bcast<0>(vf8)); vacc = a4v::dest<1, 1, 1, 1>(vacc, res); }
    { a4::vec4 mul_res = a4v::mul(vf15, a4v::bcast<1>(vf8)); a4::vec4 res = a4v::addK<1, 1>(vacc, mul_res); vacc = a4v::dest<1, 1, 1, 1>(vacc, res); }
    { a4::vec4 mul_res = a4v::mul(vf16, a4v::bcast<2>(vf8)); a4::vec4 res = a4v::addK<1, 1>(vacc, mul_res); vacc = a4v::dest<1, 1, 1, 1>(vacc, res); }
    { a4::vec4 mul_res = a4v::mul(vf17, a4v::bcast<3>(vf8)); a4::vec4 res = a4v::addK<1, 1>(vacc, mul_res); a4::q128 mask = a4v::set_epi32(-1, -1, -1, -1); vf10 = a4v::blendv_ps(vf10, res, a4v::castsi128_ps(mask)); }
    A4_W128(ADD32((uint32_t)s0, 0), a4v::castps_si128(vf10));
    vf7 = a4v::castsi128_ps(A4_R128(ADD32((uint32_t)s0, 0)));
    { a4::vec4 res = a4v::mul(vf7, vf7); a4::q128 mask = a4v::set_epi32(0, 0, -1, -1); vf7 = a4v::blend(vf7, res, a4v::castsi128_ps(mask)); }
    { a4::vec4 res = a4v::addK<1, 1>(vf7, a4v::bcast<1>(vf7)); a4::q128 mask = a4v::set_epi32(0, 0, 0, -1); vf7 = a4v::blendv_ps(vf7, res, a4v::castsi128_ps(mask)); }
    do { const a4::q128 _v = (a4v::castps_si128(vf7)); h2 = a4::hi64(_v); std::memcpy(&v0, &_v, 8); } while (0);
    { uint32_t bits = (uint32_t)v0; std::memcpy(&f0, &bits, sizeof(bits)); }
    fcr31 = (a1f::lt(f0, f21)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
    { const bool c = (fcr31 & 0x800000u) == 0u; (v0 = (uint64_t)((uint64_t)s3 + (uint64_t)0ULL)); if (c) goto L_13bbc64; }
    f1 = a1f::sqrt(fcr31, f0);
    (at = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)15928 << 16))));
    (at = (uint64_t)(RC_U64X(at) | (uint64_t)(uint16_t)20970));
    { uint32_t bits = (uint32_t)at; std::memcpy(&f0, &bits, sizeof(bits)); }
    f0 = a1f::div(fcr31, f0, f1);
    vf5 = a4v::castsi128_ps(A4_R128(ADD32((uint32_t)s0, 0)));
    { uint32_t bits; std::memcpy(&bits, &f0, sizeof(bits)); (t0 = (uint64_t)((int64_t)(int32_t)(bits))); }
    vf6 = a4v::castsi128_ps(a4::mk128(h8, t0));
    { a4::vec4 res = a4v::mul(vf5, a4v::bcast<0>(vf6)); a4::q128 mask = a4v::set_epi32(0, 0, -1, -1); vf5 = a4v::blendv_ps(vf5, res, a4v::castsi128_ps(mask)); }
    A4_W128(ADD32((uint32_t)s0, 0), a4v::castps_si128(vf5));
    vf8 = a4v::castsi128_ps(A4_R128(ADD32((uint32_t)s0, 0)));
    vf14 = a4v::castsi128_ps(A4_R128(ADD32((uint32_t)s2, 0)));
    vf15 = a4v::castsi128_ps(A4_R128(ADD32((uint32_t)s2, 16)));
    vf16 = a4v::castsi128_ps(A4_R128(ADD32((uint32_t)s2, 32)));
    vf17 = a4v::castsi128_ps(A4_R128(ADD32((uint32_t)s2, 48)));
    { a4::vec4 res = a4v::mul(vf14, a4v::bcast<0>(vf8)); vacc = a4v::dest<1, 1, 1, 1>(vacc, res); }
    { a4::vec4 mul_res = a4v::mul(vf15, a4v::bcast<1>(vf8)); a4::vec4 res = a4v::addK<1, 1>(vacc, mul_res); vacc = a4v::dest<1, 1, 1, 1>(vacc, res); }
    { a4::vec4 mul_res = a4v::mul(vf16, a4v::bcast<2>(vf8)); a4::vec4 res = a4v::addK<1, 1>(vacc, mul_res); vacc = a4v::dest<1, 1, 1, 1>(vacc, res); }
    { a4::vec4 mul_res = a4v::mul(vf17, a4v::bcast<3>(vf8)); a4::vec4 res = a4v::addK<1, 1>(vacc, mul_res); a4::q128 mask = a4v::set_epi32(-1, -1, -1, -1); vf10 = a4v::blendv_ps(vf10, res, a4v::castsi128_ps(mask)); }
    A4_W128(ADD32((uint32_t)s1, 0), a4v::castps_si128(vf10));
    (s3 = (uint64_t)(s3 | (uint64_t)(uint16_t)1));
    (v0 = (uint64_t)((uint64_t)s3 + (uint64_t)0ULL));
L_13bbc64:;
    { uint32_t bits = A1_R32B((uint32_t)sp, 200); float f; std::memcpy(&f, &bits, sizeof(f)); f21 = f; }
    { uint32_t bits = A1_R32B((uint32_t)sp, 192); float f; std::memcpy(&f, &bits, sizeof(f)); f20 = f; }
    { (sp = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)sp, 208)))); io_vf5 = vf5; io_vf6 = vf6; io_vf7 = vf7; io_vf8 = vf8; io_vf10 = vf10; io_vf14 = vf14; io_vf15 = vf15; io_vf16 = vf16; io_vf17 = vf17; io_vf24 = vf24; io_vf25 = vf25; io_vf26 = vf26; io_vf28 = vf28; io_vf29 = vf29; io_vacc = vacc; io_h2 = h2; io_h6 = h6; io_h7 = h7; io_h8 = h8; io_h9 = h9; io_h10 = h10; io_h11 = h11; io_h12 = h12; io_h13 = h13; io_h14 = h14; io_h15 = h15; return a1::RetVF{(uint64_t)v0, f0}; }
    __builtin_unreachable();
}

// gamecore_sub_013BBC90 (0x13bbc90): typed by tools/typed_lift2.py (round A6; guest frame calls; VU0 in vf0 vf10 vf25 vf28 vf29 vacc, out vf5 vf6 vf7 vf8 vf10 vf14 vf15 vf16 vf17 vf24 vf25 vf26 vf28 vf29 vacc h2 h6 h7 h8 h9 h10 h11 h12 h13 h14 h15, relocated calls 2)
inline a1::RetVF gamecore_sub_013BBC90(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint64_t v0, uint64_t a0, uint64_t a1, uint64_t sp, float f0, float f1, float f2, float f3, float f4, float f5, float f6, float f7, float f8, float f9, float f10, float f11, float f12, float f13, float f14, float f15, float f16, float f17, float f18, float f19, float f20, float f21, float f22, float f23, float f24, float f25, float f26, float f27, float f28, float f29, float f30, float f31, const a4::vec4 &io_vf0, a4::vec4 &io_vf5, a4::vec4 &io_vf6, a4::vec4 &io_vf7, a4::vec4 &io_vf8, a4::vec4 &io_vf10, a4::vec4 &io_vf14, a4::vec4 &io_vf15, a4::vec4 &io_vf16, a4::vec4 &io_vf17, a4::vec4 &io_vf24, a4::vec4 &io_vf25, a4::vec4 &io_vf26, a4::vec4 &io_vf28, a4::vec4 &io_vf29, a4::vec4 &io_vacc, uint64_t &io_h2, uint64_t &io_h6, uint64_t &io_h7, uint64_t &io_h8, uint64_t &io_h9, uint64_t &io_h10, uint64_t &io_h11, uint64_t &io_h12, uint64_t &io_h13, uint64_t &io_h14, uint64_t &io_h15)
{
    uint64_t at = 0;
    uint64_t v1 = 0;
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
    a4::vec4 vf24 = io_vf24;
    a4::vec4 vf25 = io_vf25;
    a4::vec4 vf26 = io_vf26;
    a4::vec4 vf28 = io_vf28;
    a4::vec4 vf29 = io_vf29;
    a4::vec4 vacc = io_vacc;
    uint64_t h2 = io_h2;
    uint64_t h6 = io_h6;
    uint64_t h7 = io_h7;
    uint64_t h8 = io_h8;
    uint64_t h9 = io_h9;
    uint64_t h10 = io_h10;
    uint64_t h11 = io_h11;
    uint64_t h12 = io_h12;
    uint64_t h13 = io_h13;
    uint64_t h14 = io_h14;
    uint64_t h15 = io_h15;
    (sp = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)sp, 4294967120))));
    (s0 = (uint64_t)((uint64_t)a0 + (uint64_t)0ULL));
    (a0 = (uint64_t)((uint64_t)a1 + (uint64_t)0ULL));
    { (s2 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL)); A3_CHARGE(); v0 = GetLayoutClass_u11(ram, rt, a0, f0, f1, f2, f3, f4, f5, f6, f7, f8, f9, f10, f11, f12, f13, f14, f15, f16, f17, f18, f19, f20, f21, f22, f23, f24, f25, f26, f27, f28, f29, f30, f31, h2); }
    (a1 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)v0, 344))));
    (s1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)sp, 64))));
    { uint32_t bits = 0u; std::memcpy(&f13, &bits, sizeof(bits)); }
    (a0 = (uint64_t)((uint64_t)RC_U64X(s1) + (uint64_t)0ULL));
    (at = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)48665 << 16))));
    (at = (uint64_t)(RC_U64X(at) | (uint64_t)(uint16_t)39321));
    { uint32_t bits = (uint32_t)at; std::memcpy(&f12, &bits, sizeof(bits)); }
    (a1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)a1, 1024))));
    { f14 = ((float)(f13)); A3_CHARGE(); { const a1::RetVF a2_r = iosTransMatrixF(ram, rt, (uint32_t)a0, (uint32_t)a1, (uint32_t)sp, f12, f13, f14, vf8, vf10, vf14, vf15, vf16, vf17, vacc, h6, h7, h8, h9); v0 = a2_r.v0; f0 = a2_r.f0; } }
    vf24 = a4v::castsi128_ps(A4_R128(ADD32((uint32_t)s1, 48)));
    { a4::vec4 res = a4v::subK<1, 1>(vf0, a4v::bcast<3>(vf0)); a4::q128 mask = a4v::set_epi32(0, 0, 0, -1); vf25 = a4v::blendv_ps(vf25, res, a4v::castsi128_ps(mask)); }
    { a4::vec4 res = a4v::mulK<0, 1>(vf24, a4v::bcast<0>(vf25)); a4::q128 mask = a4v::set_epi32(0, -1, -1, -1); vf28 = a4v::blendv_ps(vf28, res, a4v::castsi128_ps(mask)); }
    { a4::vec4 res = a4v::sub(vf28, vf28); a4::q128 mask = a4v::set_epi32(-1, 0, 0, 0); vf28 = a4v::blend(vf28, res, a4v::castsi128_ps(mask)); }
    do { const a4::q128 _v = (A4_R128(ADD32((uint32_t)s1, 0))); h8 = a4::hi64(_v); std::memcpy(&t0, &_v, 8); } while (0);
    do { const a4::q128 _v = (A4_R128(ADD32((uint32_t)s1, 16))); h9 = a4::hi64(_v); std::memcpy(&t1, &_v, 8); } while (0);
    do { const a4::q128 _v = (A4_R128(ADD32((uint32_t)s1, 32))); h10 = a4::hi64(_v); std::memcpy(&t2, &_v, 8); } while (0);
    do { const a4::q128 _v = (a4v::castps_si128(vf0)); h11 = a4::hi64(_v); std::memcpy(&t3, &_v, 8); } while (0);
    do { const a4::q128 _v = (a4m::pextlw(a4::mk128(h9, t1), a4::mk128(h8, t0))); h12 = a4::hi64(_v); std::memcpy(&t4, &_v, 8); } while (0);
    do { const a4::q128 _v = (a4m::pextuw(a4::mk128(h9, t1), a4::mk128(h8, t0))); h13 = a4::hi64(_v); std::memcpy(&t5, &_v, 8); } while (0);
    do { const a4::q128 _v = (a4m::pextlw(a4::mk128(h11, t3), a4::mk128(h10, t2))); h14 = a4::hi64(_v); std::memcpy(&t6, &_v, 8); } while (0);
    do { const a4::q128 _v = (a4m::pextuw(a4::mk128(h11, t3), a4::mk128(h10, t2))); h15 = a4::hi64(_v); std::memcpy(&t7, &_v, 8); } while (0);
    do { const a4::q128 _v = (a4m::pcpyld(a4::mk128(h14, t6), a4::mk128(h12, t4))); h8 = a4::hi64(_v); std::memcpy(&t0, &_v, 8); } while (0);
    do { const a4::q128 _v = (a4v::unpackhi_epi64(a4::mk128(h12, t4), a4::mk128(h14, t6))); h9 = a4::hi64(_v); std::memcpy(&t1, &_v, 8); } while (0);
    do { const a4::q128 _v = (a4m::pcpyld(a4::mk128(h15, t7), a4::mk128(h13, t5))); h10 = a4::hi64(_v); std::memcpy(&t2, &_v, 8); } while (0);
    do { const a4::q128 _v = (a4v::unpackhi_epi64(a4::mk128(h13, t5), a4::mk128(h15, t7))); h11 = a4::hi64(_v); std::memcpy(&t3, &_v, 8); } while (0);
    vf24 = a4v::castsi128_ps(a4::mk128(h8, t0));
    vf25 = a4v::castsi128_ps(a4::mk128(h9, t1));
    vf26 = a4v::castsi128_ps(a4::mk128(h10, t2));
    { a4::vec4 res = a4v::mulK<0, 1>(vf24, a4v::bcast<0>(vf28)); vacc = a4v::dest<1, 1, 1, 1>(vacc, res); }
    { a4::vec4 mul_res = a4v::mulK<0, 1>(vf25, a4v::bcast<1>(vf28)); a4::vec4 res = a4v::addK<1, 1>(vacc, mul_res); vacc = a4v::dest<1, 1, 1, 1>(vacc, res); }
    { a4::vec4 mul_res = a4v::mulK<0, 1>(vf26, a4v::bcast<2>(vf28)); a4::vec4 res = a4v::addK<1, 1>(vacc, mul_res); vacc = a4v::dest<1, 1, 1, 1>(vacc, res); }
    { a4::vec4 mul_res = a4v::mulK<1, 1>(vf0, a4v::bcast<3>(vf28)); a4::vec4 res = a4v::addK<1, 1>(vacc, mul_res); a4::q128 mask = a4v::set_epi32(-1, -1, -1, -1); vf29 = a4v::blendv_ps(vf29, res, a4v::castsi128_ps(mask)); }
    { a4::vec4 res = a4v::subK<1, 1>(vf29, vf29); a4::q128 mask = a4v::set_epi32(-1, 0, 0, 0); vf29 = a4v::blend(vf29, res, a4v::castsi128_ps(mask)); }
    { a4::vec4 res = a4v::addK<1, 1>(vf29, vf0); a4::q128 mask = a4v::set_epi32(-1, 0, 0, 0); vf29 = a4v::blend(vf29, res, a4v::castsi128_ps(mask)); }
    A4_W128(ADD32((uint32_t)sp, 0), a4v::castps_si128(vf24));
    A4_W128(ADD32((uint32_t)sp, 16), a4v::castps_si128(vf25));
    A4_W128(ADD32((uint32_t)sp, 32), a4v::castps_si128(vf26));
    A4_W128(ADD32((uint32_t)sp, 48), a4v::castps_si128(vf29));
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)sp, 128))));
    vf8 = a4v::castsi128_ps(A4_R128(ADD32((uint32_t)s0, 0)));
    vf14 = a4v::castsi128_ps(A4_R128(ADD32((uint32_t)sp, 0)));
    vf15 = a4v::castsi128_ps(A4_R128(ADD32((uint32_t)sp, 16)));
    vf16 = a4v::castsi128_ps(A4_R128(ADD32((uint32_t)sp, 32)));
    vf17 = a4v::castsi128_ps(A4_R128(ADD32((uint32_t)sp, 48)));
    { a4::vec4 res = a4v::mul(vf14, a4v::bcast<0>(vf8)); vacc = a4v::dest<1, 1, 1, 1>(vacc, res); }
    { a4::vec4 mul_res = a4v::mul(vf15, a4v::bcast<1>(vf8)); a4::vec4 res = a4v::addK<1, 1>(vacc, mul_res); vacc = a4v::dest<1, 1, 1, 1>(vacc, res); }
    { a4::vec4 mul_res = a4v::mul(vf16, a4v::bcast<2>(vf8)); a4::vec4 res = a4v::addK<1, 1>(vacc, mul_res); vacc = a4v::dest<1, 1, 1, 1>(vacc, res); }
    { a4::vec4 mul_res = a4v::mul(vf17, a4v::bcast<3>(vf8)); a4::vec4 res = a4v::addK<1, 1>(vacc, mul_res); a4::q128 mask = a4v::set_epi32(-1, -1, -1, -1); vf10 = a4v::blendv_ps(vf10, res, a4v::castsi128_ps(mask)); }
    A4_W128(ADD32((uint32_t)v1, 0), a4v::castps_si128(vf10));
    vf7 = a4v::castsi128_ps(A4_R128(ADD32((uint32_t)v1, 0)));
    { a4::vec4 res = a4v::mul(vf7, vf7); a4::q128 mask = a4v::set_epi32(0, 0, -1, -1); vf7 = a4v::blend(vf7, res, a4v::castsi128_ps(mask)); }
    { a4::vec4 res = a4v::addK<1, 1>(vf7, a4v::bcast<1>(vf7)); a4::q128 mask = a4v::set_epi32(0, 0, 0, -1); vf7 = a4v::blendv_ps(vf7, res, a4v::castsi128_ps(mask)); }
    do { const a4::q128 _v = (a4v::castps_si128(vf7)); h2 = a4::hi64(_v); std::memcpy(&v0, &_v, 8); } while (0);
    { uint32_t bits = (uint32_t)v0; std::memcpy(&f1, &bits, sizeof(bits)); }
    (at = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)15651 << 16))));
    (at = (uint64_t)(RC_U64X(at) | (uint64_t)(uint16_t)55048));
    { uint32_t bits = (uint32_t)at; std::memcpy(&f0, &bits, sizeof(bits)); }
    fcr31 = (a1f::lt(f1, f0)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
    { const bool c = (fcr31 & 0x800000u) == 0u; (v0 = (uint64_t)((uint64_t)s2 + (uint64_t)0ULL)); if (c) goto L_13bbe08; }
    f1 = a1f::sqrt(fcr31, f1);
    (at = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)15928 << 16))));
    (at = (uint64_t)(RC_U64X(at) | (uint64_t)(uint16_t)20970));
    { uint32_t bits = (uint32_t)at; std::memcpy(&f0, &bits, sizeof(bits)); }
    f0 = a1f::div(fcr31, f0, f1);
    vf5 = a4v::castsi128_ps(A4_R128(ADD32((uint32_t)v1, 0)));
    { uint32_t bits; std::memcpy(&bits, &f0, sizeof(bits)); (t0 = (uint64_t)((int64_t)(int32_t)(bits))); }
    vf6 = a4v::castsi128_ps(a4::mk128(h8, t0));
    { a4::vec4 res = a4v::mul(vf5, a4v::bcast<0>(vf6)); a4::q128 mask = a4v::set_epi32(0, 0, -1, -1); vf5 = a4v::blendv_ps(vf5, res, a4v::castsi128_ps(mask)); }
    A4_W128(ADD32((uint32_t)v1, 0), a4v::castps_si128(vf5));
    vf8 = a4v::castsi128_ps(A4_R128(ADD32((uint32_t)v1, 0)));
    vf14 = a4v::castsi128_ps(A4_R128(ADD32((uint32_t)s1, 0)));
    vf15 = a4v::castsi128_ps(A4_R128(ADD32((uint32_t)s1, 16)));
    vf16 = a4v::castsi128_ps(A4_R128(ADD32((uint32_t)s1, 32)));
    vf17 = a4v::castsi128_ps(A4_R128(ADD32((uint32_t)s1, 48)));
    { a4::vec4 res = a4v::mul(vf14, a4v::bcast<0>(vf8)); vacc = a4v::dest<1, 1, 1, 1>(vacc, res); }
    { a4::vec4 mul_res = a4v::mul(vf15, a4v::bcast<1>(vf8)); a4::vec4 res = a4v::addK<1, 1>(vacc, mul_res); vacc = a4v::dest<1, 1, 1, 1>(vacc, res); }
    { a4::vec4 mul_res = a4v::mul(vf16, a4v::bcast<2>(vf8)); a4::vec4 res = a4v::addK<1, 1>(vacc, mul_res); vacc = a4v::dest<1, 1, 1, 1>(vacc, res); }
    { a4::vec4 mul_res = a4v::mul(vf17, a4v::bcast<3>(vf8)); a4::vec4 res = a4v::addK<1, 1>(vacc, mul_res); a4::q128 mask = a4v::set_epi32(-1, -1, -1, -1); vf10 = a4v::blendv_ps(vf10, res, a4v::castsi128_ps(mask)); }
    A4_W128(ADD32((uint32_t)s0, 0), a4v::castps_si128(vf10));
    (s2 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 1))));
    (v0 = (uint64_t)((uint64_t)RC_U64X(s2) + (uint64_t)0ULL));
L_13bbe08:;
    { (sp = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)sp, 176)))); io_vf5 = vf5; io_vf6 = vf6; io_vf7 = vf7; io_vf8 = vf8; io_vf10 = vf10; io_vf14 = vf14; io_vf15 = vf15; io_vf16 = vf16; io_vf17 = vf17; io_vf24 = vf24; io_vf25 = vf25; io_vf26 = vf26; io_vf28 = vf28; io_vf29 = vf29; io_vacc = vacc; io_h2 = h2; io_h6 = h6; io_h7 = h7; io_h8 = h8; io_h9 = h9; io_h10 = h10; io_h11 = h11; io_h12 = h12; io_h13 = h13; io_h14 = h14; io_h15 = h15; return a1::RetVF{(uint64_t)v0, f0}; }
    __builtin_unreachable();
}

// chkAttainExecLayout (0x1432ee0): typed by tools/typed_lift2.py (round A6; frame calls, extra outputs xo[0]=f12 xo[1]=f13 xo[2]=f20 xo[3]=f21 xo[4]=f22 xo[5]=f23 xo[6]=a2 xo[7]=a3 xo[8]=t0 xo[9]=t1 xo[10]=t2 xo[11]=t3; VU0 in vf0 vf7 vf10 vf12 vf13 vf14 vf15 vf16 vf17 vf28 vf29 vacc vstat h2, out vf4 vf5 vf6 vf7 vf8 vf10 vf11 vf12 vf13 vf14 vf15 vf16 vf17 vf24 vf25 vf26 vf27 vf28 vf29 vacc vq vstat h3 h8 h9 h10 h11 h12 h13 h14 h15, 64-bit inputs a0 a1, guest sp, frame 16 bytes, 0 slot accesses)
inline a1::RetVF chkAttainExecLayout(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint64_t w64_a0, uint64_t w64_a1, uint64_t a2, uint64_t a3, uint32_t sp, float f13, float f14, float f15, float f16, float f17, float f18, float f19, float f20, float f21, float f22, float f23, const a4::vec4 &io_vf0, a4::vec4 &io_vf4, a4::vec4 &io_vf5, a4::vec4 &io_vf6, a4::vec4 &io_vf7, a4::vec4 &io_vf8, a4::vec4 &io_vf10, a4::vec4 &io_vf11, a4::vec4 &io_vf12, a4::vec4 &io_vf13, a4::vec4 &io_vf14, a4::vec4 &io_vf15, a4::vec4 &io_vf16, a4::vec4 &io_vf17, a4::vec4 &io_vf24, a4::vec4 &io_vf25, a4::vec4 &io_vf26, a4::vec4 &io_vf27, a4::vec4 &io_vf28, a4::vec4 &io_vf29, a4::vec4 &io_vacc, float &io_vq, uint16_t &io_vstat, uint64_t io_h2, uint64_t &io_h3, uint64_t &io_h8, uint64_t &io_h9, uint64_t &io_h10, uint64_t &io_h11, uint64_t &io_h12, uint64_t &io_h13, uint64_t &io_h14, uint64_t &io_h15, uint64_t *xo = nullptr)
{
    float f12 = 0.0f;
    uint32_t a0 = (uint32_t)w64_a0;
    uint32_t a1 = (uint32_t)w64_a1;
    uint64_t t0 = 0;
    uint64_t t1 = 0;
    uint64_t t2 = 0;
    uint64_t t3 = 0;
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
    a4::vec4 vf24 = io_vf24;
    a4::vec4 vf25 = io_vf25;
    a4::vec4 vf26 = io_vf26;
    a4::vec4 vf27 = io_vf27;
    a4::vec4 vf28 = io_vf28;
    a4::vec4 vf29 = io_vf29;
    a4::vec4 vacc = io_vacc;
    float vq = io_vq;
    uint16_t vstat = io_vstat;
    uint64_t h2 = io_h2;
    uint64_t h3 = io_h3;
    uint64_t h8 = io_h8;
    uint64_t h9 = io_h9;
    uint64_t h10 = io_h10;
    uint64_t h11 = io_h11;
    uint64_t h12 = io_h12;
    uint64_t h13 = io_h13;
    uint64_t h14 = io_h14;
    uint64_t h15 = io_h15;
    (t0 = (uint64_t)((uint64_t)a3 + (uint64_t)0ULL));
    { const uint32_t _dynInsn = A1_R32(0x1432EECu); const int32_t _dynSimm = (int16_t)(_dynInsn & 0xFFFFu); const uint32_t _dynImm = _dynInsn & 0xFFFFu; (void)_dynSimm; (void)_dynImm; (a3 = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)_dynImm << 16)))); }
    f12 = a1_flt(0u);
    { const uint32_t _dynInsn = A1_R32(0x1432EF4u); const int32_t _dynSimm = (int16_t)(_dynInsn & 0xFFFFu); const uint32_t _dynImm = _dynInsn & 0xFFFFu; (void)_dynSimm; (void)_dynImm; (a3 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)a3, _dynSimm)))); }
    { uint64_t a7c[13] = {0u, 0u, 0u, 0u, 0u, 0u, 0u, a2, a3, 0u, 0u, 0u, 0u}; const a1::RetVF a7_t = chkAttainExecOffsetData(ram, rt, w64_a0, w64_a1, a2, a3, t0, a1_sx64((uint32_t)(sp + 0x0u)), f12, f13, f14, f15, f16, f17, f18, f19, f20, f21, f22, f23, vf0, vf4, vf5, vf6, vf7, vf8, vf10, vf11, vf12, vf13, vf14, vf15, vf16, vf17, vf24, vf25, vf26, vf27, vf28, vf29, vacc, vq, vstat, h2, h3, h8, h9, h10, h11, h12, h13, h14, h15, a7c); f12 = a1_flt((uint32_t)a7c[0]); f13 = a1_flt((uint32_t)a7c[1]); f20 = a1_flt((uint32_t)a7c[2]); f21 = a1_flt((uint32_t)a7c[3]); f22 = a1_flt((uint32_t)a7c[4]); f23 = a1_flt((uint32_t)a7c[5]); a2 = a7c[7]; a3 = a7c[8]; t0 = a7c[9]; t1 = a7c[10]; t2 = a7c[11]; t3 = a7c[12]; if (xo) { xo[0] = a1_bits(f12); xo[1] = a1_bits(f13); xo[2] = a1_bits(f20); xo[3] = a1_bits(f21); xo[4] = a1_bits(f22); xo[5] = a1_bits(f23); xo[6] = a2; xo[7] = a3; xo[8] = t0; xo[9] = t1; xo[10] = t2; xo[11] = t3; } io_vf4 = vf4; io_vf5 = vf5; io_vf6 = vf6; io_vf7 = vf7; io_vf8 = vf8; io_vf10 = vf10; io_vf11 = vf11; io_vf12 = vf12; io_vf13 = vf13; io_vf14 = vf14; io_vf15 = vf15; io_vf16 = vf16; io_vf17 = vf17; io_vf24 = vf24; io_vf25 = vf25; io_vf26 = vf26; io_vf27 = vf27; io_vf28 = vf28; io_vf29 = vf29; io_vacc = vacc; io_vq = vq; io_vstat = vstat; io_h3 = h3; io_h8 = h8; io_h9 = h9; io_h10 = h10; io_h11 = h11; io_h12 = h12; io_h13 = h13; io_h14 = h14; io_h15 = h15; return a7_t; }
    __builtin_unreachable();
}

// chkAttainExecOffsetRadius (0x1432f08): typed by tools/typed_lift2.py (round A6; frame calls, extra outputs xo[0]=f12 xo[1]=f13 xo[2]=f20 xo[3]=f21 xo[4]=f22 xo[5]=f23 xo[6]=a2 xo[7]=a3 xo[8]=t0 xo[9]=t1 xo[10]=t2 xo[11]=t3; VU0 in vf0 vf7 vf10 vf12 vf13 vf14 vf15 vf16 vf17 vf28 vf29 vacc vstat h2, out vf4 vf5 vf6 vf7 vf8 vf10 vf11 vf12 vf13 vf14 vf15 vf16 vf17 vf24 vf25 vf26 vf27 vf28 vf29 vacc vq vstat h3 h8 h9 h10 h11 h12 h13 h14 h15, 64-bit inputs a0 a1, guest sp, frame 16 bytes, 0 slot accesses)
inline a1::RetVF chkAttainExecOffsetRadius(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint64_t w64_a0, uint64_t w64_a1, uint64_t a2, uint64_t a3, uint32_t sp, float f12, float f13, float f14, float f15, float f16, float f17, float f18, float f19, float f20, float f21, float f22, float f23, const a4::vec4 &io_vf0, a4::vec4 &io_vf4, a4::vec4 &io_vf5, a4::vec4 &io_vf6, a4::vec4 &io_vf7, a4::vec4 &io_vf8, a4::vec4 &io_vf10, a4::vec4 &io_vf11, a4::vec4 &io_vf12, a4::vec4 &io_vf13, a4::vec4 &io_vf14, a4::vec4 &io_vf15, a4::vec4 &io_vf16, a4::vec4 &io_vf17, a4::vec4 &io_vf24, a4::vec4 &io_vf25, a4::vec4 &io_vf26, a4::vec4 &io_vf27, a4::vec4 &io_vf28, a4::vec4 &io_vf29, a4::vec4 &io_vacc, float &io_vq, uint16_t &io_vstat, uint64_t io_h2, uint64_t &io_h3, uint64_t &io_h8, uint64_t &io_h9, uint64_t &io_h10, uint64_t &io_h11, uint64_t &io_h12, uint64_t &io_h13, uint64_t &io_h14, uint64_t &io_h15, uint64_t *xo = nullptr)
{
    uint32_t a0 = (uint32_t)w64_a0;
    uint32_t a1 = (uint32_t)w64_a1;
    uint64_t t0 = 0;
    uint64_t t1 = 0;
    uint64_t t2 = 0;
    uint64_t t3 = 0;
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
    a4::vec4 vf24 = io_vf24;
    a4::vec4 vf25 = io_vf25;
    a4::vec4 vf26 = io_vf26;
    a4::vec4 vf27 = io_vf27;
    a4::vec4 vf28 = io_vf28;
    a4::vec4 vf29 = io_vf29;
    a4::vec4 vacc = io_vacc;
    float vq = io_vq;
    uint16_t vstat = io_vstat;
    uint64_t h2 = io_h2;
    uint64_t h3 = io_h3;
    uint64_t h8 = io_h8;
    uint64_t h9 = io_h9;
    uint64_t h10 = io_h10;
    uint64_t h11 = io_h11;
    uint64_t h12 = io_h12;
    uint64_t h13 = io_h13;
    uint64_t h14 = io_h14;
    uint64_t h15 = io_h15;
    (t0 = (uint64_t)((uint64_t)a3 + (uint64_t)0ULL));
    { const uint32_t _dynInsn = A1_R32(0x1432F14u); const int32_t _dynSimm = (int16_t)(_dynInsn & 0xFFFFu); const uint32_t _dynImm = _dynInsn & 0xFFFFu; (void)_dynSimm; (void)_dynImm; (a3 = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)_dynImm << 16)))); }
    { const uint32_t _dynInsn = A1_R32(0x1432F18u); const int32_t _dynSimm = (int16_t)(_dynInsn & 0xFFFFu); const uint32_t _dynImm = _dynInsn & 0xFFFFu; (void)_dynSimm; (void)_dynImm; (a3 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)a3, _dynSimm)))); }
    { uint64_t a7c[13] = {0u, 0u, 0u, 0u, 0u, 0u, 0u, a2, a3, 0u, 0u, 0u, 0u}; const a1::RetVF a7_t = chkAttainExecOffsetData(ram, rt, w64_a0, w64_a1, a2, a3, t0, a1_sx64((uint32_t)(sp + 0x0u)), f12, f13, f14, f15, f16, f17, f18, f19, f20, f21, f22, f23, vf0, vf4, vf5, vf6, vf7, vf8, vf10, vf11, vf12, vf13, vf14, vf15, vf16, vf17, vf24, vf25, vf26, vf27, vf28, vf29, vacc, vq, vstat, h2, h3, h8, h9, h10, h11, h12, h13, h14, h15, a7c); f12 = a1_flt((uint32_t)a7c[0]); f13 = a1_flt((uint32_t)a7c[1]); f20 = a1_flt((uint32_t)a7c[2]); f21 = a1_flt((uint32_t)a7c[3]); f22 = a1_flt((uint32_t)a7c[4]); f23 = a1_flt((uint32_t)a7c[5]); a2 = a7c[7]; a3 = a7c[8]; t0 = a7c[9]; t1 = a7c[10]; t2 = a7c[11]; t3 = a7c[12]; if (xo) { xo[0] = a1_bits(f12); xo[1] = a1_bits(f13); xo[2] = a1_bits(f20); xo[3] = a1_bits(f21); xo[4] = a1_bits(f22); xo[5] = a1_bits(f23); xo[6] = a2; xo[7] = a3; xo[8] = t0; xo[9] = t1; xo[10] = t2; xo[11] = t3; } io_vf4 = vf4; io_vf5 = vf5; io_vf6 = vf6; io_vf7 = vf7; io_vf8 = vf8; io_vf10 = vf10; io_vf11 = vf11; io_vf12 = vf12; io_vf13 = vf13; io_vf14 = vf14; io_vf15 = vf15; io_vf16 = vf16; io_vf17 = vf17; io_vf24 = vf24; io_vf25 = vf25; io_vf26 = vf26; io_vf27 = vf27; io_vf28 = vf28; io_vf29 = vf29; io_vacc = vacc; io_vq = vq; io_vstat = vstat; io_h3 = h3; io_h8 = h8; io_h9 = h9; io_h10 = h10; io_h11 = h11; io_h12 = h12; io_h13 = h13; io_h14 = h14; io_h15 = h15; return a7_t; }
    __builtin_unreachable();
}

// chkAttainExecOffsetPos (0x1432f28): typed by tools/typed_lift2.py (round A6; frame calls; VU0 in vf0 vf7 vf10 vf12 vf13 vf14 vf15 vf16 vf17 vf28 vf29 vacc vstat h2, out vf4 vf5 vf6 vf7 vf8 vf10 vf11 vf12 vf13 vf14 vf15 vf16 vf17 vf24 vf25 vf26 vf27 vf28 vf29 vacc vq vstat h3 h8 h9 h10 h11 h12 h13 h14 h15, 64-bit inputs a0 a1 a2 a3 t0, guest sp, frame 16 bytes, 0 slot accesses)
inline a1::RetVF chkAttainExecOffsetPos(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint64_t w64_a0, uint64_t w64_a1, uint64_t w64_a2, uint64_t w64_a3, uint64_t w64_t0, uint32_t sp, float f13, float f14, float f15, float f16, float f17, float f18, float f19, float f20, float f21, float f22, float f23, const a4::vec4 &io_vf0, a4::vec4 &io_vf4, a4::vec4 &io_vf5, a4::vec4 &io_vf6, a4::vec4 &io_vf7, a4::vec4 &io_vf8, a4::vec4 &io_vf10, a4::vec4 &io_vf11, a4::vec4 &io_vf12, a4::vec4 &io_vf13, a4::vec4 &io_vf14, a4::vec4 &io_vf15, a4::vec4 &io_vf16, a4::vec4 &io_vf17, a4::vec4 &io_vf24, a4::vec4 &io_vf25, a4::vec4 &io_vf26, a4::vec4 &io_vf27, a4::vec4 &io_vf28, a4::vec4 &io_vf29, a4::vec4 &io_vacc, float &io_vq, uint16_t &io_vstat, uint64_t io_h2, uint64_t &io_h3, uint64_t &io_h8, uint64_t &io_h9, uint64_t &io_h10, uint64_t &io_h11, uint64_t &io_h12, uint64_t &io_h13, uint64_t &io_h14, uint64_t &io_h15)
{
    float f12 = 0.0f;
    uint32_t a0 = (uint32_t)w64_a0;
    uint32_t a1 = (uint32_t)w64_a1;
    uint32_t a2 = (uint32_t)w64_a2;
    uint32_t a3 = (uint32_t)w64_a3;
    uint32_t t0 = (uint32_t)w64_t0;
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
    a4::vec4 vf24 = io_vf24;
    a4::vec4 vf25 = io_vf25;
    a4::vec4 vf26 = io_vf26;
    a4::vec4 vf27 = io_vf27;
    a4::vec4 vf28 = io_vf28;
    a4::vec4 vf29 = io_vf29;
    a4::vec4 vacc = io_vacc;
    float vq = io_vq;
    uint16_t vstat = io_vstat;
    uint64_t h2 = io_h2;
    uint64_t h3 = io_h3;
    uint64_t h8 = io_h8;
    uint64_t h9 = io_h9;
    uint64_t h10 = io_h10;
    uint64_t h11 = io_h11;
    uint64_t h12 = io_h12;
    uint64_t h13 = io_h13;
    uint64_t h14 = io_h14;
    uint64_t h15 = io_h15;
    f12 = a1_flt(0u);
    { const a1::RetVF a4_t = chkAttainExecOffsetData(ram, rt, w64_a0, w64_a1, w64_a2, w64_a3, w64_t0, a1_sx64((uint32_t)(sp + 0x0u)), f12, f13, f14, f15, f16, f17, f18, f19, f20, f21, f22, f23, vf0, vf4, vf5, vf6, vf7, vf8, vf10, vf11, vf12, vf13, vf14, vf15, vf16, vf17, vf24, vf25, vf26, vf27, vf28, vf29, vacc, vq, vstat, h2, h3, h8, h9, h10, h11, h12, h13, h14, h15); io_vf4 = vf4; io_vf5 = vf5; io_vf6 = vf6; io_vf7 = vf7; io_vf8 = vf8; io_vf10 = vf10; io_vf11 = vf11; io_vf12 = vf12; io_vf13 = vf13; io_vf14 = vf14; io_vf15 = vf15; io_vf16 = vf16; io_vf17 = vf17; io_vf24 = vf24; io_vf25 = vf25; io_vf26 = vf26; io_vf27 = vf27; io_vf28 = vf28; io_vf29 = vf29; io_vacc = vacc; io_vq = vq; io_vstat = vstat; io_h3 = h3; io_h8 = h8; io_h9 = h9; io_h10 = h10; io_h11 = h11; io_h12 = h12; io_h13 = h13; io_h14 = h14; io_h15 = h15; return a4_t; }
    __builtin_unreachable();
}

// SCRConditionCheckAngleToNextPoint (0x1456860): typed by tools/typed_lift2.py (round A6; guest frame calls; VU0 in vf0 vf7 vf10 vf12 vf13 vf14 vf15 vf16 vf17 vf25 vf28 vf29 vacc vstat, out vf4 vf5 vf7 vf8 vf10 vf11 vf12 vf13 vf14 vf15 vf16 vf17 vf24 vf25 vf26 vf28 vf29 vacc vq vstat h2 h8 h9 h10 h11 h12 h13 h14 h15, relocated calls 2)
inline uint64_t SCRConditionCheckAngleToNextPoint(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint64_t v0, uint64_t a0, uint64_t sp, float f0, float f1, float f2, float f3, float f4, float f5, float f6, float f7, float f8, float f9, float f10, float f11, float f12, float f13, float f14, float f15, float f16, float f17, float f18, float f19, float f20, float f21, float f22, float f23, float f24, float f25, float f26, float f27, float f28, float f29, float f30, float f31, const a4::vec4 &io_vf0, a4::vec4 &io_vf4, a4::vec4 &io_vf5, a4::vec4 &io_vf7, a4::vec4 &io_vf8, a4::vec4 &io_vf10, a4::vec4 &io_vf11, a4::vec4 &io_vf12, a4::vec4 &io_vf13, a4::vec4 &io_vf14, a4::vec4 &io_vf15, a4::vec4 &io_vf16, a4::vec4 &io_vf17, a4::vec4 &io_vf24, a4::vec4 &io_vf25, a4::vec4 &io_vf26, a4::vec4 &io_vf28, a4::vec4 &io_vf29, a4::vec4 &io_vacc, float &io_vq, uint16_t &io_vstat, uint64_t &io_h2, uint64_t &io_h8, uint64_t &io_h9, uint64_t &io_h10, uint64_t &io_h11, uint64_t &io_h12, uint64_t &io_h13, uint64_t &io_h14, uint64_t &io_h15)
{
    uint64_t at = 0;
    uint64_t v1 = 0;
    uint64_t a1 = 0;
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
    uint32_t fcr31 = 0;
    a4::vec4 vf0 = io_vf0;
    a4::vec4 vf4 = io_vf4;
    a4::vec4 vf5 = io_vf5;
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
    a4::vec4 vf24 = io_vf24;
    a4::vec4 vf25 = io_vf25;
    a4::vec4 vf26 = io_vf26;
    a4::vec4 vf28 = io_vf28;
    a4::vec4 vf29 = io_vf29;
    a4::vec4 vacc = io_vacc;
    float vq = io_vq;
    uint16_t vstat = io_vstat;
    uint64_t h2 = io_h2;
    uint64_t h8 = io_h8;
    uint64_t h9 = io_h9;
    uint64_t h10 = io_h10;
    uint64_t h11 = io_h11;
    uint64_t h12 = io_h12;
    uint64_t h13 = io_h13;
    uint64_t h14 = io_h14;
    uint64_t h15 = io_h15;
    (sp = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)sp, 4294967184))));
    (s0 = (uint64_t)((uint64_t)a0 + (uint64_t)0ULL));
    { float f = f20; uint32_t bits; std::memcpy(&bits, &f, sizeof(bits)); A1_W32B((uint32_t)sp, 104, bits); }
    { f20 = ((float)(f12)); A3_CHARGE(); v0 = getSCRDataCtrlWork(ram, rt, v0, a0, f12, f13, f14, f15, f16, f17, f18, f19, h2); }
    (s1 = (uint64_t)((uint64_t)v0 + (uint64_t)0ULL));
    { (a0 = (uint64_t)((uint64_t)s0 + (uint64_t)0ULL)); A3_CHARGE(); v0 = GetLayoutClass_u11(ram, rt, a0, f0, f1, f2, f3, f4, f5, f6, f7, f8, f9, f10, f11, f12, f13, f14, f15, f16, f17, f18, f19, f20, f21, f22, f23, f24, f25, f26, f27, f28, f29, f30, f31, h2); }
    (a0 = (uint64_t)((uint64_t)s1 + (uint64_t)0ULL));
    (a1 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL));
    { (s0 = (uint64_t)((uint64_t)v0 + (uint64_t)0ULL)); A3_CHARGE(); v0 = getPassageWork(ram, rt, (uint32_t)a0, a1); }
    (a0 = (uint64_t)((uint64_t)v0 + (uint64_t)0ULL));
    { const bool c = (int64_t)a0 == 0; (a1 = (uint64_t)((uint64_t)sp + (uint64_t)0ULL)); if (c) goto L_1456a20; }
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)s0, 308))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)s0, 316))));
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
    A4_W128(ADD32((uint32_t)a1, 0), a4v::castps_si128(vf14));
    A4_W128(ADD32((uint32_t)a1, 16), a4v::castps_si128(vf15));
    A4_W128(ADD32((uint32_t)a1, 32), a4v::castps_si128(vf16));
    vf4 = a4v::castsi128_ps(A4_R128(ADD32((uint32_t)v1, 0)));
    { a4::q128 mask = a4v::set_epi32(-1, 0, 0, 0); vf4 = a4v::blendv_ps(vf4, vf0, a4v::castsi128_ps(mask)); }
    A4_W128(ADD32((uint32_t)a1, 48), a4v::castps_si128(vf4));
    vf24 = a4v::castsi128_ps(A4_R128(ADD32((uint32_t)sp, 48)));
    { a4::vec4 res = a4v::subK<1, 1>(vf0, a4v::bcast<3>(vf0)); a4::q128 mask = a4v::set_epi32(0, 0, 0, -1); vf25 = a4v::blendv_ps(vf25, res, a4v::castsi128_ps(mask)); }
    { a4::vec4 res = a4v::mulK<0, 1>(vf24, a4v::bcast<0>(vf25)); a4::q128 mask = a4v::set_epi32(0, -1, -1, -1); vf28 = a4v::blendv_ps(vf28, res, a4v::castsi128_ps(mask)); }
    { a4::vec4 res = a4v::sub(vf28, vf28); a4::q128 mask = a4v::set_epi32(-1, 0, 0, 0); vf28 = a4v::blend(vf28, res, a4v::castsi128_ps(mask)); }
    do { const a4::q128 _v = (A4_R128(ADD32((uint32_t)sp, 0))); h8 = a4::hi64(_v); std::memcpy(&t0, &_v, 8); } while (0);
    do { const a4::q128 _v = (A4_R128(ADD32((uint32_t)sp, 16))); h9 = a4::hi64(_v); std::memcpy(&t1, &_v, 8); } while (0);
    do { const a4::q128 _v = (A4_R128(ADD32((uint32_t)sp, 32))); h10 = a4::hi64(_v); std::memcpy(&t2, &_v, 8); } while (0);
    do { const a4::q128 _v = (a4v::castps_si128(vf0)); h11 = a4::hi64(_v); std::memcpy(&t3, &_v, 8); } while (0);
    do { const a4::q128 _v = (a4m::pextlw(a4::mk128(h9, t1), a4::mk128(h8, t0))); h12 = a4::hi64(_v); std::memcpy(&t4, &_v, 8); } while (0);
    do { const a4::q128 _v = (a4m::pextuw(a4::mk128(h9, t1), a4::mk128(h8, t0))); h13 = a4::hi64(_v); std::memcpy(&t5, &_v, 8); } while (0);
    do { const a4::q128 _v = (a4m::pextlw(a4::mk128(h11, t3), a4::mk128(h10, t2))); h14 = a4::hi64(_v); std::memcpy(&t6, &_v, 8); } while (0);
    do { const a4::q128 _v = (a4m::pextuw(a4::mk128(h11, t3), a4::mk128(h10, t2))); h15 = a4::hi64(_v); std::memcpy(&t7, &_v, 8); } while (0);
    do { const a4::q128 _v = (a4m::pcpyld(a4::mk128(h14, t6), a4::mk128(h12, t4))); h8 = a4::hi64(_v); std::memcpy(&t0, &_v, 8); } while (0);
    do { const a4::q128 _v = (a4v::unpackhi_epi64(a4::mk128(h12, t4), a4::mk128(h14, t6))); h9 = a4::hi64(_v); std::memcpy(&t1, &_v, 8); } while (0);
    do { const a4::q128 _v = (a4m::pcpyld(a4::mk128(h15, t7), a4::mk128(h13, t5))); h10 = a4::hi64(_v); std::memcpy(&t2, &_v, 8); } while (0);
    do { const a4::q128 _v = (a4v::unpackhi_epi64(a4::mk128(h13, t5), a4::mk128(h15, t7))); h11 = a4::hi64(_v); std::memcpy(&t3, &_v, 8); } while (0);
    vf24 = a4v::castsi128_ps(a4::mk128(h8, t0));
    vf25 = a4v::castsi128_ps(a4::mk128(h9, t1));
    vf26 = a4v::castsi128_ps(a4::mk128(h10, t2));
    { a4::vec4 res = a4v::mulK<0, 1>(vf24, a4v::bcast<0>(vf28)); vacc = a4v::dest<1, 1, 1, 1>(vacc, res); }
    { a4::vec4 mul_res = a4v::mulK<0, 1>(vf25, a4v::bcast<1>(vf28)); a4::vec4 res = a4v::addK<1, 1>(vacc, mul_res); vacc = a4v::dest<1, 1, 1, 1>(vacc, res); }
    { a4::vec4 mul_res = a4v::mulK<0, 1>(vf26, a4v::bcast<2>(vf28)); a4::vec4 res = a4v::addK<1, 1>(vacc, mul_res); vacc = a4v::dest<1, 1, 1, 1>(vacc, res); }
    { a4::vec4 mul_res = a4v::mulK<1, 1>(vf0, a4v::bcast<3>(vf28)); a4::vec4 res = a4v::addK<1, 1>(vacc, mul_res); a4::q128 mask = a4v::set_epi32(-1, -1, -1, -1); vf29 = a4v::blendv_ps(vf29, res, a4v::castsi128_ps(mask)); }
    { a4::vec4 res = a4v::subK<1, 1>(vf29, vf29); a4::q128 mask = a4v::set_epi32(-1, 0, 0, 0); vf29 = a4v::blend(vf29, res, a4v::castsi128_ps(mask)); }
    { a4::vec4 res = a4v::addK<1, 1>(vf29, vf0); a4::q128 mask = a4v::set_epi32(-1, 0, 0, 0); vf29 = a4v::blend(vf29, res, a4v::castsi128_ps(mask)); }
    A4_W128(ADD32((uint32_t)sp, 0), a4v::castps_si128(vf24));
    A4_W128(ADD32((uint32_t)sp, 16), a4v::castps_si128(vf25));
    A4_W128(ADD32((uint32_t)sp, 32), a4v::castps_si128(vf26));
    A4_W128(ADD32((uint32_t)sp, 48), a4v::castps_si128(vf29));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)sp, 64))));
    vf8 = a4v::castsi128_ps(A4_R128(ADD32((uint32_t)a0, 0)));
    vf14 = a4v::castsi128_ps(A4_R128(ADD32((uint32_t)sp, 0)));
    vf15 = a4v::castsi128_ps(A4_R128(ADD32((uint32_t)sp, 16)));
    vf16 = a4v::castsi128_ps(A4_R128(ADD32((uint32_t)sp, 32)));
    vf17 = a4v::castsi128_ps(A4_R128(ADD32((uint32_t)sp, 48)));
    { a4::vec4 res = a4v::mul(vf14, a4v::bcast<0>(vf8)); vacc = a4v::dest<1, 1, 1, 1>(vacc, res); }
    { a4::vec4 mul_res = a4v::mul(vf15, a4v::bcast<1>(vf8)); a4::vec4 res = a4v::addK<1, 1>(vacc, mul_res); vacc = a4v::dest<1, 1, 1, 1>(vacc, res); }
    { a4::vec4 mul_res = a4v::mul(vf16, a4v::bcast<2>(vf8)); a4::vec4 res = a4v::addK<1, 1>(vacc, mul_res); vacc = a4v::dest<1, 1, 1, 1>(vacc, res); }
    { a4::vec4 mul_res = a4v::mul(vf17, a4v::bcast<3>(vf8)); a4::vec4 res = a4v::addK<1, 1>(vacc, mul_res); a4::q128 mask = a4v::set_epi32(-1, -1, -1, -1); vf10 = a4v::blendv_ps(vf10, res, a4v::castsi128_ps(mask)); }
    A4_W128(ADD32((uint32_t)v0, 0), a4v::castps_si128(vf10));
    A1_W32B((uint32_t)sp, 68, 0u);
    vf5 = a4v::castsi128_ps(A4_R128(ADD32((uint32_t)v0, 0)));
    { a4::vec4 res = a4v::mul(vf5, vf5); a4::q128 mask = a4v::set_epi32(0, -1, -1, -1); vf7 = a4v::blend(vf7, res, a4v::castsi128_ps(mask)); }
    { a4::vec4 res = a4v::mulK<1, 1>(vf0, a4v::bcast<0>(vf7)); vacc = a4v::dest<0, 0, 0, 1>(vacc, res); }
    { a4::vec4 mul_res = a4v::mulK<1, 1>(vf0, a4v::bcast<1>(vf7)); a4::vec4 res = a4v::addK<1, 1>(vacc, mul_res); vacc = a4v::dest<0, 0, 0, 1>(vacc, res); }
    { a4::vec4 mul_res = a4v::mulK<1, 1>(vf0, a4v::bcast<2>(vf7)); a4::vec4 res = a4v::addK<1, 1>(vacc, mul_res); a4::q128 mask = a4v::set_epi32(-1, 0, 0, 0); vf7 = a4v::blendv_ps(vf7, res, a4v::castsi128_ps(mask)); }
    { float fs = a4v::cvtss_f32(a4v::shuffle_ps<0x03>(vf0, vf0)); float ft = a4v::cvtss_f32(a4v::shuffle_ps<0x03>(vf7, vf7)); vq = a4v::rsqrt(vstat, fs, ft); }
    { a4::vec4 res = a4v::mul(vf5, a4v::set1_ps(vq)); a4::q128 mask = a4v::set_epi32(0, -1, -1, -1); vf5 = a4v::blendv_ps(vf5, res, a4v::castsi128_ps(mask)); }
    A4_W128(ADD32((uint32_t)v0, 0), a4v::castps_si128(vf5));
    { uint32_t bits = A1_R32B((uint32_t)sp, 64); float f; std::memcpy(&f, &bits, sizeof(f)); f12 = f; }
    { { uint32_t bits = A1_R32B((uint32_t)sp, 72); float f; std::memcpy(&f, &bits, sizeof(f)); f13 = f; } A3_CHARGE(); f0 = a9x::atan2Exact(ram, rt, f12, f13); }
    (at = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)15502 << 16))));
    (at = (uint64_t)(RC_U64X(at) | (uint64_t)(uint16_t)64053));
    { uint32_t bits = (uint32_t)at; std::memcpy(&f1, &bits, sizeof(bits)); }
    f0 = std::fabs((float)(f0));
    f1 = a1f::mul(f20, f1);
    fcr31 = (a1f::le(f0, f1)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
    { const bool c = (fcr31 & 0x800000u) != 0u; (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 1)))); if (c) goto L_1456a20; }
    (v0 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL));
L_1456a20:;
    { uint32_t bits = A1_R32B((uint32_t)sp, 104); float f; std::memcpy(&f, &bits, sizeof(f)); f20 = f; }
    { (sp = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)sp, 112)))); io_vf4 = vf4; io_vf5 = vf5; io_vf7 = vf7; io_vf8 = vf8; io_vf10 = vf10; io_vf11 = vf11; io_vf12 = vf12; io_vf13 = vf13; io_vf14 = vf14; io_vf15 = vf15; io_vf16 = vf16; io_vf17 = vf17; io_vf24 = vf24; io_vf25 = vf25; io_vf26 = vf26; io_vf28 = vf28; io_vf29 = vf29; io_vacc = vacc; io_vq = vq; io_vstat = vstat; io_h2 = h2; io_h8 = h8; io_h9 = h9; io_h10 = h10; io_h11 = h11; io_h12 = h12; io_h13 = h13; io_h14 = h14; io_h15 = h15; return v0; }
    __builtin_unreachable();
}

// nico_calcTailGenealogicalNodeTail (0x14621c8): typed by tools/typed_lift2.py (round A6; guest frame calls; VU0 in vf0 vf7 vf9 vf10 vf12 vf13 vf14 vf15 vf16 vf17 vf25 vf28 vf29 vacc vstat h2 h5 h8, out vf4 vf5 vf6 vf7 vf8 vf9 vf10 vf11 vf12 vf13 vf14 vf15 vf16 vf17 vf24 vf25 vf26 vf27 vf28 vf29 vacc vq vstat h2 h3 h4 h5 h8 h9 h10 h11 h12 h13 h14 h15, relocated calls 1, 64-bit inputs t2)
inline uint64_t nico_calcTailGenealogicalNodeTail(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint64_t v0, uint64_t a0, uint64_t w64_t2, uint64_t sp, float f12, float f13, float f14, float f15, float f16, float f17, float f18, float f19, float f20, const a4::vec4 &io_vf0, a4::vec4 &io_vf4, a4::vec4 &io_vf5, a4::vec4 &io_vf6, a4::vec4 &io_vf7, a4::vec4 &io_vf8, a4::vec4 &io_vf9, a4::vec4 &io_vf10, a4::vec4 &io_vf11, a4::vec4 &io_vf12, a4::vec4 &io_vf13, a4::vec4 &io_vf14, a4::vec4 &io_vf15, a4::vec4 &io_vf16, a4::vec4 &io_vf17, a4::vec4 &io_vf24, a4::vec4 &io_vf25, a4::vec4 &io_vf26, a4::vec4 &io_vf27, a4::vec4 &io_vf28, a4::vec4 &io_vf29, a4::vec4 &io_vacc, float &io_vq, uint16_t &io_vstat, uint64_t &io_h2, uint64_t &io_h3, uint64_t &io_h4, uint64_t &io_h5, uint64_t &io_h8, uint64_t &io_h9, uint64_t &io_h10, uint64_t &io_h11, uint64_t &io_h12, uint64_t &io_h13, uint64_t &io_h14, uint64_t &io_h15)
{
    uint32_t v1 = 0;
    uint64_t a1 = 0;
    uint32_t t2 = (uint32_t)w64_t2;
    uint32_t s0 = 0;
    uint64_t s1 = 0;
    a4::vec4 vf0 = io_vf0;
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
    a4::vec4 vf24 = io_vf24;
    a4::vec4 vf25 = io_vf25;
    a4::vec4 vf26 = io_vf26;
    a4::vec4 vf27 = io_vf27;
    a4::vec4 vf28 = io_vf28;
    a4::vec4 vf29 = io_vf29;
    a4::vec4 vacc = io_vacc;
    float vq = io_vq;
    uint16_t vstat = io_vstat;
    uint64_t h2 = io_h2;
    uint64_t h3 = io_h3;
    uint64_t h4 = io_h4;
    uint64_t h5 = io_h5;
    uint64_t h8 = io_h8;
    uint64_t h9 = io_h9;
    uint64_t h10 = io_h10;
    uint64_t h11 = io_h11;
    uint64_t h12 = io_h12;
    uint64_t h13 = io_h13;
    uint64_t h14 = io_h14;
    uint64_t h15 = io_h15;
    (sp = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)sp, 4294967184))));
    (s1 = (uint64_t)((uint64_t)a0 + (uint64_t)0ULL));
    { (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)s1, 0)))); A3_CHARGE(); v0 = GetCharacterClass(ram, rt, a0, h2, h5); }
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)327 << 16))));
    (a1 = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)326 << 16))));
    (s0 = (uint32_t)((int32_t)((int32_t)A1_R32B((uint32_t)v0, 4376))));
    (v1 = (uint32_t)((int32_t)((int32_t)A1_R32B((uint32_t)a1, 29700))));
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)a0, 4294953904))));
    (a1 = (uint64_t)(A1_R64B((uint32_t)a0, 8)));
    (v0 = (uint64_t)(A1_R64B((uint32_t)a0, 0)));
    v1 = v1 << 4;
    v1 = v1 + s0;
    A1_W64B((uint32_t)sp, 72, a1);
    A1_W64B((uint32_t)sp, 64, v0);
    (v0 = (uint64_t)((uint64_t)sp + (uint64_t)0ULL));
    (a1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(v1, 640))));
    { a4::vec4 res = a4v::addK<1, 1>(vf0, a4v::bcast<3>(vf0)); a4::q128 mask = a4v::set_epi32(0, -1, -1, -1); vf12 = a4v::blendv_ps(vf12, res, a4v::castsi128_ps(mask)); }
    vf11 = a4v::castsi128_ps(A4_R128(ADD32(v1, 0)));
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
    A4_W128(ADD32((uint32_t)v0, 0), a4v::castps_si128(vf14));
    A4_W128(ADD32((uint32_t)v0, 16), a4v::castps_si128(vf15));
    A4_W128(ADD32((uint32_t)v0, 32), a4v::castps_si128(vf16));
    vf4 = a4v::castsi128_ps(A4_R128(ADD32((uint32_t)a1, 0)));
    { a4::q128 mask = a4v::set_epi32(-1, 0, 0, 0); vf4 = a4v::blendv_ps(vf4, vf0, a4v::castsi128_ps(mask)); }
    A4_W128(ADD32((uint32_t)v0, 48), a4v::castps_si128(vf4));
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)326 << 16))));
    (a1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)sp, 64))));
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)a0, 29700))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)a0, 0))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)SLL32((uint32_t)v0, 4))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v0, s0))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v0, 656))));
    vf8 = a4v::castsi128_ps(A4_R128(ADD32((uint32_t)a1, 0)));
    vf14 = a4v::castsi128_ps(A4_R128(ADD32((uint32_t)sp, 0)));
    vf15 = a4v::castsi128_ps(A4_R128(ADD32((uint32_t)sp, 16)));
    vf16 = a4v::castsi128_ps(A4_R128(ADD32((uint32_t)sp, 32)));
    vf17 = a4v::castsi128_ps(A4_R128(ADD32((uint32_t)sp, 48)));
    { a4::vec4 res = a4v::mul(vf14, a4v::bcast<0>(vf8)); vacc = a4v::dest<1, 1, 1, 1>(vacc, res); }
    { a4::vec4 mul_res = a4v::mul(vf15, a4v::bcast<1>(vf8)); a4::vec4 res = a4v::addK<1, 1>(vacc, mul_res); vacc = a4v::dest<1, 1, 1, 1>(vacc, res); }
    { a4::vec4 mul_res = a4v::mul(vf16, a4v::bcast<2>(vf8)); a4::vec4 res = a4v::addK<1, 1>(vacc, mul_res); vacc = a4v::dest<1, 1, 1, 1>(vacc, res); }
    { a4::vec4 mul_res = a4v::mul(vf17, a4v::bcast<3>(vf8)); a4::vec4 res = a4v::addK<1, 1>(vacc, mul_res); a4::q128 mask = a4v::set_epi32(-1, -1, -1, -1); vf10 = a4v::blendv_ps(vf10, res, a4v::castsi128_ps(mask)); }
    A4_W128(ADD32((uint32_t)v0, 0), a4v::castps_si128(vf10));
    (v1 = (uint32_t)((int32_t)((int32_t)A1_R32B((uint32_t)a0, 0))));
    v1 = v1 << 4;
    v1 = v1 + s0;
    v1 = v1 + 0x10u;
    A4_W128(ADD32(v1, 0), a4v::castps_si128(vf0));
    { (a0 = (uint64_t)((uint64_t)s1 + (uint64_t)0ULL)); A3_CHARGE(); v0 = nico_calcTailGenealogicalNodeNext(ram, rt, v0, a0, w64_t2, sp, f12, f13, f14, f15, f16, f17, f18, f19, f20, vf0, vf4, vf5, vf6, vf7, vf8, vf9, vf10, vf11, vf12, vf13, vf14, vf15, vf16, vf17, vf24, vf25, vf26, vf27, vf28, vf29, vacc, vq, vstat, h2, h3, h4, h5, h8, h9, h10, h11, h12, h13, h14, h15); }
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 1))));
    A1_W32B(s0, 960, (uint32_t)v0);
    { (sp = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)sp, 112)))); io_vf4 = vf4; io_vf5 = vf5; io_vf6 = vf6; io_vf7 = vf7; io_vf8 = vf8; io_vf9 = vf9; io_vf10 = vf10; io_vf11 = vf11; io_vf12 = vf12; io_vf13 = vf13; io_vf14 = vf14; io_vf15 = vf15; io_vf16 = vf16; io_vf17 = vf17; io_vf24 = vf24; io_vf25 = vf25; io_vf26 = vf26; io_vf27 = vf27; io_vf28 = vf28; io_vf29 = vf29; io_vacc = vacc; io_vq = vq; io_vstat = vstat; io_h2 = h2; io_h3 = h3; io_h4 = h4; io_h5 = h5; io_h8 = h8; io_h9 = h9; io_h10 = h10; io_h11 = h11; io_h12 = h12; io_h13 = h13; io_h14 = h14; io_h15 = h15; return v0; }
    __builtin_unreachable();
}

// gamecore_sub_01462B30 (0x1462b30): typed by tools/typed_lift2.py (round A6; guest frame calls, extra outputs xo[0]=f20; VU0 in vf0 vf10 vf12 vf13 vf14 vf15 vf16 vf17 vf25 vf28 vf29 vacc vstat, out vf4 vf5 vf6 vf7 vf8 vf10 vf11 vf12 vf13 vf14 vf15 vf16 vf17 vf24 vf25 vf26 vf27 vf28 vf29 vacc vq vstat h2 h3 h8 h9 h10 h11 h12 h13 h14 h15, relocated calls 8)
inline a1::RetVF gamecore_sub_01462B30(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint64_t v0, uint64_t a0, uint64_t a1, uint64_t a2, uint64_t a3, uint64_t t0, uint64_t t1, uint64_t t2, uint64_t sp, float f0, float f1, float f2, float f3, float f4, float f5, float f6, float f7, float f8, float f9, float f10, float f11, float f12, float f13, float f14, float f15, float f16, float f17, float f18, float f19, float f20, float f21, float f22, float f23, float f24, float f25, float f26, float f27, float f28, float f29, float f30, float f31, const a4::vec4 &io_vf0, a4::vec4 &io_vf4, a4::vec4 &io_vf5, a4::vec4 &io_vf6, a4::vec4 &io_vf7, a4::vec4 &io_vf8, a4::vec4 &io_vf10, a4::vec4 &io_vf11, a4::vec4 &io_vf12, a4::vec4 &io_vf13, a4::vec4 &io_vf14, a4::vec4 &io_vf15, a4::vec4 &io_vf16, a4::vec4 &io_vf17, a4::vec4 &io_vf24, a4::vec4 &io_vf25, a4::vec4 &io_vf26, a4::vec4 &io_vf27, a4::vec4 &io_vf28, a4::vec4 &io_vf29, a4::vec4 &io_vacc, float &io_vq, uint16_t &io_vstat, uint64_t &io_h2, uint64_t &io_h3, uint64_t &io_h8, uint64_t &io_h9, uint64_t &io_h10, uint64_t &io_h11, uint64_t &io_h12, uint64_t &io_h13, uint64_t &io_h14, uint64_t &io_h15, uint64_t *xo = nullptr)
{
    uint64_t at = 0;
    uint64_t v1 = 0;
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
    uint64_t fp = 0;
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
    a4::vec4 vf24 = io_vf24;
    a4::vec4 vf25 = io_vf25;
    a4::vec4 vf26 = io_vf26;
    a4::vec4 vf27 = io_vf27;
    a4::vec4 vf28 = io_vf28;
    a4::vec4 vf29 = io_vf29;
    a4::vec4 vacc = io_vacc;
    float vq = io_vq;
    uint16_t vstat = io_vstat;
    uint64_t h2 = io_h2;
    uint64_t h3 = io_h3;
    uint64_t h8 = io_h8;
    uint64_t h9 = io_h9;
    uint64_t h10 = io_h10;
    uint64_t h11 = io_h11;
    uint64_t h12 = io_h12;
    uint64_t h13 = io_h13;
    uint64_t h14 = io_h14;
    uint64_t h15 = io_h15;
    (sp = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)sp, 4294966960))));
    (s0 = (uint64_t)((uint64_t)a3 + (uint64_t)0ULL));
    (s3 = (uint64_t)((uint64_t)a2 + (uint64_t)0ULL));
    (s4 = (uint64_t)((uint64_t)a0 + (uint64_t)0ULL));
    (s5 = (uint64_t)((uint64_t)t0 + (uint64_t)0ULL));
    (s6 = (uint64_t)((uint64_t)t1 + (uint64_t)0ULL));
    (s7 = (uint64_t)((uint64_t)a1 + (uint64_t)0ULL));
    (fp = (uint64_t)((uint64_t)t2 + (uint64_t)0ULL));
    { float f = f20; uint32_t bits; std::memcpy(&bits, &f, sizeof(bits)); A1_W32B((uint32_t)sp, 320, bits); }
    f20 = ((float)(f12));
    (a2 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)s7, 16))));
    (a3 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)s7, 4))));
    (t0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)s7, 180))));
    (t1 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)s7, 184))));
    (t2 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)s7, 20))));
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)s7, 8))));
    (a1 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)s7, 12))));
    { (s1 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)s7, 0)))); A3_CHARGE(); uint64_t a7c[1] = {(uint64_t)a1_bits(f0)}; v0 = GetMotionMatrixOfTargetFromParent_x11(ram, rt, a0, (uint32_t)a1, (uint32_t)a2, (uint32_t)a3, (uint32_t)t0, (uint32_t)t1, t2, (uint32_t)sp, vf0, vf4, vf5, vf6, vf10, vf11, vf12, vf13, vf14, vf15, vf16, vf17, vf24, vf25, vf26, vf27, vacc, vq, vstat, h2, a7c); f0 = a1_flt((uint32_t)a7c[0]); }
    vf5 = a4v::castsi128_ps(A4_R128(ADD32((uint32_t)s0, 0)));
    vf6 = a4v::castsi128_ps(A4_R128(ADD32((uint32_t)s3, 0)));
    { a4::vec4 res = a4v::sub(vf5, vf6); a4::q128 mask = a4v::set_epi32(0, -1, -1, -1); vf5 = a4v::blend(vf5, res, a4v::castsi128_ps(mask)); }
    A4_W128(ADD32((uint32_t)sp, 0), a4v::castps_si128(vf5));
    (s2 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)s7, 16))));
    (a0 = (uint64_t)((uint64_t)s1 + (uint64_t)0ULL));
    { (s0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)s7, 20)))); A3_CHARGE(); v0 = GetLayoutClass_u11(ram, rt, a0, f0, f1, f2, f3, f4, f5, f6, f7, f8, f9, f10, f11, f12, f13, f14, f15, f16, f17, f18, f19, f20, f21, f22, f23, f24, f25, f26, f27, f28, f29, f30, f31, h2); }
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)s2, 8))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v0, 144))));
    (s0 = (uint64_t)((int64_t)(int32_t)((int32_t)SLL32((uint32_t)s0, 6))));
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)sp, 16))));
    (s0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)s0, (uint32_t)a0))));
    (s0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)s0, 16))));
    vf5 = a4v::castsi128_ps(A4_R128(ADD32((uint32_t)s0, 0)));
    vf6 = a4v::castsi128_ps(A4_R128(ADD32((uint32_t)v0, 0)));
    { a4::vec4 res = a4v::mul(vf5, vf6); a4::q128 mask = a4v::set_epi32(0, -1, -1, -1); vf5 = a4v::blend(vf5, res, a4v::castsi128_ps(mask)); }
    A4_W128(ADD32((uint32_t)v1, 0), a4v::castps_si128(vf5));
    vf7 = a4v::castsi128_ps(A4_R128(ADD32((uint32_t)v1, 0)));
    { a4::vec4 res = a4v::mul(vf7, vf7); a4::q128 mask = a4v::set_epi32(0, -1, -1, -1); vf7 = a4v::blend(vf7, res, a4v::castsi128_ps(mask)); }
    { a4::vec4 res = a4v::addK<1, 1>(vf7, a4v::bcast<1>(vf7)); a4::q128 mask = a4v::set_epi32(0, 0, 0, -1); vf7 = a4v::blendv_ps(vf7, res, a4v::castsi128_ps(mask)); }
    { a4::vec4 res = a4v::addK<1, 1>(vf7, a4v::bcast<2>(vf7)); a4::q128 mask = a4v::set_epi32(0, 0, 0, -1); vf7 = a4v::blendv_ps(vf7, res, a4v::castsi128_ps(mask)); }
    do { const a4::q128 _v = (a4v::castps_si128(vf7)); h2 = a4::hi64(_v); std::memcpy(&v0, &_v, 8); } while (0);
    { uint32_t bits = (uint32_t)v0; std::memcpy(&f0, &bits, sizeof(bits)); }
    vf7 = a4v::castsi128_ps(A4_R128(ADD32((uint32_t)sp, 0)));
    { a4::vec4 res = a4v::mul(vf7, vf7); a4::q128 mask = a4v::set_epi32(0, -1, -1, -1); vf7 = a4v::blend(vf7, res, a4v::castsi128_ps(mask)); }
    { a4::vec4 res = a4v::addK<1, 1>(vf7, a4v::bcast<1>(vf7)); a4::q128 mask = a4v::set_epi32(0, 0, 0, -1); vf7 = a4v::blendv_ps(vf7, res, a4v::castsi128_ps(mask)); }
    { a4::vec4 res = a4v::addK<1, 1>(vf7, a4v::bcast<2>(vf7)); a4::q128 mask = a4v::set_epi32(0, 0, 0, -1); vf7 = a4v::blendv_ps(vf7, res, a4v::castsi128_ps(mask)); }
    do { const a4::q128 _v = (a4v::castps_si128(vf7)); h2 = a4::hi64(_v); std::memcpy(&v0, &_v, 8); } while (0);
    { uint32_t bits = (uint32_t)v0; std::memcpy(&f1, &bits, sizeof(bits)); }
    f0 = a1f::div(fcr31, f0, f1);
    f0 = a1f::sqrt(fcr31, f0);
    { uint32_t bits; std::memcpy(&bits, &f0, sizeof(bits)); (v0 = (uint64_t)((int64_t)(int32_t)(bits))); }
    vf5 = a4v::castsi128_ps(A4_R128(ADD32((uint32_t)sp, 0)));
    vf6 = a4v::castsi128_ps(a4::mk128(h2, v0));
    { a4::vec4 res = a4v::mul(vf5, a4v::bcast<0>(vf6)); a4::q128 mask = a4v::set_epi32(0, -1, -1, -1); vf5 = a4v::blendv_ps(vf5, res, a4v::castsi128_ps(mask)); }
    A4_W128(ADD32((uint32_t)sp, 0), a4v::castps_si128(vf5));
    vf5 = a4v::castsi128_ps(A4_R128(ADD32((uint32_t)s3, 0)));
    vf6 = a4v::castsi128_ps(A4_R128(ADD32((uint32_t)sp, 0)));
    { a4::vec4 res = a4v::add(vf5, vf6); a4::q128 mask = a4v::set_epi32(0, -1, -1, -1); vf5 = a4v::blend(vf5, res, a4v::castsi128_ps(mask)); }
    A4_W128(ADD32((uint32_t)s4, 0), a4v::castps_si128(vf5));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)sp, 32))));
    { a4::vec4 res = a4v::addK<1, 1>(vf0, a4v::bcast<3>(vf0)); a4::q128 mask = a4v::set_epi32(0, -1, -1, -1); vf12 = a4v::blendv_ps(vf12, res, a4v::castsi128_ps(mask)); }
    vf11 = a4v::castsi128_ps(A4_R128(ADD32((uint32_t)s5, 0)));
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
    A4_W128(ADD32((uint32_t)v0, 0), a4v::castps_si128(vf14));
    A4_W128(ADD32((uint32_t)v0, 16), a4v::castps_si128(vf15));
    A4_W128(ADD32((uint32_t)v0, 32), a4v::castps_si128(vf16));
    vf4 = a4v::castsi128_ps(A4_R128(ADD32((uint32_t)s4, 0)));
    { a4::q128 mask = a4v::set_epi32(-1, 0, 0, 0); vf4 = a4v::blendv_ps(vf4, vf0, a4v::castsi128_ps(mask)); }
    A4_W128(ADD32((uint32_t)v0, 48), a4v::castps_si128(vf4));
    (s1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)sp, 96))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)sp, 32))));
    vf24 = a4v::castsi128_ps(A4_R128(ADD32((uint32_t)v0, 48)));
    { a4::vec4 res = a4v::subK<1, 1>(vf0, a4v::bcast<3>(vf0)); a4::q128 mask = a4v::set_epi32(0, 0, 0, -1); vf25 = a4v::blendv_ps(vf25, res, a4v::castsi128_ps(mask)); }
    { a4::vec4 res = a4v::mulK<0, 1>(vf24, a4v::bcast<0>(vf25)); a4::q128 mask = a4v::set_epi32(0, -1, -1, -1); vf28 = a4v::blendv_ps(vf28, res, a4v::castsi128_ps(mask)); }
    { a4::vec4 res = a4v::sub(vf28, vf28); a4::q128 mask = a4v::set_epi32(-1, 0, 0, 0); vf28 = a4v::blend(vf28, res, a4v::castsi128_ps(mask)); }
    do { const a4::q128 _v = (A4_R128(ADD32((uint32_t)v0, 0))); h8 = a4::hi64(_v); std::memcpy(&t0, &_v, 8); } while (0);
    do { const a4::q128 _v = (A4_R128(ADD32((uint32_t)v0, 16))); h9 = a4::hi64(_v); std::memcpy(&t1, &_v, 8); } while (0);
    do { const a4::q128 _v = (A4_R128(ADD32((uint32_t)v0, 32))); h10 = a4::hi64(_v); std::memcpy(&t2, &_v, 8); } while (0);
    do { const a4::q128 _v = (a4v::castps_si128(vf0)); h11 = a4::hi64(_v); std::memcpy(&t3, &_v, 8); } while (0);
    do { const a4::q128 _v = (a4m::pextlw(a4::mk128(h9, t1), a4::mk128(h8, t0))); h12 = a4::hi64(_v); std::memcpy(&t4, &_v, 8); } while (0);
    do { const a4::q128 _v = (a4m::pextuw(a4::mk128(h9, t1), a4::mk128(h8, t0))); h13 = a4::hi64(_v); std::memcpy(&t5, &_v, 8); } while (0);
    do { const a4::q128 _v = (a4m::pextlw(a4::mk128(h11, t3), a4::mk128(h10, t2))); h14 = a4::hi64(_v); std::memcpy(&t6, &_v, 8); } while (0);
    do { const a4::q128 _v = (a4m::pextuw(a4::mk128(h11, t3), a4::mk128(h10, t2))); h15 = a4::hi64(_v); std::memcpy(&t7, &_v, 8); } while (0);
    do { const a4::q128 _v = (a4m::pcpyld(a4::mk128(h14, t6), a4::mk128(h12, t4))); h8 = a4::hi64(_v); std::memcpy(&t0, &_v, 8); } while (0);
    do { const a4::q128 _v = (a4v::unpackhi_epi64(a4::mk128(h12, t4), a4::mk128(h14, t6))); h9 = a4::hi64(_v); std::memcpy(&t1, &_v, 8); } while (0);
    do { const a4::q128 _v = (a4m::pcpyld(a4::mk128(h15, t7), a4::mk128(h13, t5))); h10 = a4::hi64(_v); std::memcpy(&t2, &_v, 8); } while (0);
    do { const a4::q128 _v = (a4v::unpackhi_epi64(a4::mk128(h13, t5), a4::mk128(h15, t7))); h11 = a4::hi64(_v); std::memcpy(&t3, &_v, 8); } while (0);
    vf24 = a4v::castsi128_ps(a4::mk128(h8, t0));
    vf25 = a4v::castsi128_ps(a4::mk128(h9, t1));
    vf26 = a4v::castsi128_ps(a4::mk128(h10, t2));
    { a4::vec4 res = a4v::mulK<0, 1>(vf24, a4v::bcast<0>(vf28)); vacc = a4v::dest<1, 1, 1, 1>(vacc, res); }
    { a4::vec4 mul_res = a4v::mulK<0, 1>(vf25, a4v::bcast<1>(vf28)); a4::vec4 res = a4v::addK<1, 1>(vacc, mul_res); vacc = a4v::dest<1, 1, 1, 1>(vacc, res); }
    { a4::vec4 mul_res = a4v::mulK<0, 1>(vf26, a4v::bcast<2>(vf28)); a4::vec4 res = a4v::addK<1, 1>(vacc, mul_res); vacc = a4v::dest<1, 1, 1, 1>(vacc, res); }
    { a4::vec4 mul_res = a4v::mulK<1, 1>(vf0, a4v::bcast<3>(vf28)); a4::vec4 res = a4v::addK<1, 1>(vacc, mul_res); a4::q128 mask = a4v::set_epi32(-1, -1, -1, -1); vf29 = a4v::blendv_ps(vf29, res, a4v::castsi128_ps(mask)); }
    { a4::vec4 res = a4v::subK<1, 1>(vf29, vf29); a4::q128 mask = a4v::set_epi32(-1, 0, 0, 0); vf29 = a4v::blend(vf29, res, a4v::castsi128_ps(mask)); }
    { a4::vec4 res = a4v::addK<1, 1>(vf29, vf0); a4::q128 mask = a4v::set_epi32(-1, 0, 0, 0); vf29 = a4v::blend(vf29, res, a4v::castsi128_ps(mask)); }
    A4_W128(ADD32((uint32_t)s1, 0), a4v::castps_si128(vf24));
    A4_W128(ADD32((uint32_t)s1, 16), a4v::castps_si128(vf25));
    A4_W128(ADD32((uint32_t)s1, 32), a4v::castps_si128(vf26));
    A4_W128(ADD32((uint32_t)s1, 48), a4v::castps_si128(vf29));
    (s0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)sp, 160))));
    vf8 = a4v::castsi128_ps(A4_R128(ADD32((uint32_t)s6, 0)));
    vf14 = a4v::castsi128_ps(A4_R128(ADD32((uint32_t)s1, 0)));
    vf15 = a4v::castsi128_ps(A4_R128(ADD32((uint32_t)s1, 16)));
    vf16 = a4v::castsi128_ps(A4_R128(ADD32((uint32_t)s1, 32)));
    vf17 = a4v::castsi128_ps(A4_R128(ADD32((uint32_t)s1, 48)));
    { a4::vec4 res = a4v::mul(vf14, a4v::bcast<0>(vf8)); vacc = a4v::dest<1, 1, 1, 1>(vacc, res); }
    { a4::vec4 mul_res = a4v::mul(vf15, a4v::bcast<1>(vf8)); a4::vec4 res = a4v::addK<1, 1>(vacc, mul_res); vacc = a4v::dest<1, 1, 1, 1>(vacc, res); }
    { a4::vec4 mul_res = a4v::mul(vf16, a4v::bcast<2>(vf8)); a4::vec4 res = a4v::addK<1, 1>(vacc, mul_res); vacc = a4v::dest<1, 1, 1, 1>(vacc, res); }
    { a4::vec4 mul_res = a4v::mul(vf17, a4v::bcast<3>(vf8)); a4::vec4 res = a4v::addK<1, 1>(vacc, mul_res); a4::q128 mask = a4v::set_epi32(-1, -1, -1, -1); vf10 = a4v::blendv_ps(vf10, res, a4v::castsi128_ps(mask)); }
    A4_W128(ADD32((uint32_t)s0, 0), a4v::castps_si128(vf10));
    vf5 = a4v::castsi128_ps(A4_R128(ADD32((uint32_t)s0, 0)));
    { a4::vec4 res = a4v::mul(vf5, vf5); a4::q128 mask = a4v::set_epi32(0, -1, -1, -1); vf7 = a4v::blend(vf7, res, a4v::castsi128_ps(mask)); }
    { a4::vec4 res = a4v::mulK<1, 1>(vf0, a4v::bcast<0>(vf7)); vacc = a4v::dest<0, 0, 0, 1>(vacc, res); }
    { a4::vec4 mul_res = a4v::mulK<1, 1>(vf0, a4v::bcast<1>(vf7)); a4::vec4 res = a4v::addK<1, 1>(vacc, mul_res); vacc = a4v::dest<0, 0, 0, 1>(vacc, res); }
    { a4::vec4 mul_res = a4v::mulK<1, 1>(vf0, a4v::bcast<2>(vf7)); a4::vec4 res = a4v::addK<1, 1>(vacc, mul_res); a4::q128 mask = a4v::set_epi32(-1, 0, 0, 0); vf7 = a4v::blendv_ps(vf7, res, a4v::castsi128_ps(mask)); }
    { float fs = a4v::cvtss_f32(a4v::shuffle_ps<0x03>(vf0, vf0)); float ft = a4v::cvtss_f32(a4v::shuffle_ps<0x03>(vf7, vf7)); vq = a4v::rsqrt(vstat, fs, ft); }
    { a4::vec4 res = a4v::mul(vf5, a4v::set1_ps(vq)); a4::q128 mask = a4v::set_epi32(0, -1, -1, -1); vf5 = a4v::blendv_ps(vf5, res, a4v::castsi128_ps(mask)); }
    A4_W128(ADD32((uint32_t)s0, 0), a4v::castps_si128(vf5));
    (at = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)49024 << 16))));
    { uint32_t bits = (uint32_t)at; std::memcpy(&f0, &bits, sizeof(bits)); }
    { uint32_t bits; std::memcpy(&bits, &f0, sizeof(bits)); (v0 = (uint64_t)((int64_t)(int32_t)(bits))); }
    vf5 = a4v::castsi128_ps(A4_R128(ADD32((uint32_t)s0, 0)));
    vf6 = a4v::castsi128_ps(a4::mk128(h2, v0));
    { a4::vec4 res = a4v::mul(vf5, a4v::bcast<0>(vf6)); a4::q128 mask = a4v::set_epi32(0, -1, -1, -1); vf5 = a4v::blendv_ps(vf5, res, a4v::castsi128_ps(mask)); }
    A4_W128(ADD32((uint32_t)s0, 0), a4v::castps_si128(vf5));
    { const uint32_t _dynInsn = A1_R32(0x1462DB4u); const int32_t _dynSimm = (int16_t)(_dynInsn & 0xFFFFu); const uint32_t _dynImm = _dynInsn & 0xFFFFu; (void)_dynSimm; (void)_dynImm; (a2 = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)_dynImm << 16)))); }
    (s2 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)sp, 176))));
    { const uint32_t _dynInsn = A1_R32(0x1462DBCu); const int32_t _dynSimm = (int16_t)(_dynInsn & 0xFFFFu); const uint32_t _dynImm = _dynInsn & 0xFFFFu; (void)_dynSimm; (void)_dynImm; (a2 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)a2, _dynSimm)))); }
    (a0 = (uint64_t)((uint64_t)RC_U64X(s2) + (uint64_t)0ULL));
    (a3 = (uint64_t)((uint64_t)RC_U64X(a2) + (uint64_t)0ULL));
    { (a1 = (uint64_t)((uint64_t)RC_U64X(s0) + (uint64_t)0ULL)); A3_CHARGE(); a9x::diffQuat(ram, rt, (uint32_t)a0, (uint32_t)a1, (uint32_t)a2, (uint32_t)a3); }
    (a0 = (uint64_t)((uint64_t)s2 + (uint64_t)0ULL));
    (a2 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)sp, 192))));
    { const bool c = (int64_t)fp == 0; (a1 = (uint64_t)((uint64_t)a0 + (uint64_t)0ULL)); if (c) goto L_1462e90; }
    vf8 = a4v::castsi128_ps(A4_R128(ADD32((uint32_t)fp, 0)));
    vf14 = a4v::castsi128_ps(A4_R128(ADD32((uint32_t)s1, 0)));
    vf15 = a4v::castsi128_ps(A4_R128(ADD32((uint32_t)s1, 16)));
    vf16 = a4v::castsi128_ps(A4_R128(ADD32((uint32_t)s1, 32)));
    vf17 = a4v::castsi128_ps(A4_R128(ADD32((uint32_t)s1, 48)));
    { a4::vec4 res = a4v::mul(vf14, a4v::bcast<0>(vf8)); vacc = a4v::dest<1, 1, 1, 1>(vacc, res); }
    { a4::vec4 mul_res = a4v::mul(vf15, a4v::bcast<1>(vf8)); a4::vec4 res = a4v::addK<1, 1>(vacc, mul_res); vacc = a4v::dest<1, 1, 1, 1>(vacc, res); }
    { a4::vec4 mul_res = a4v::mul(vf16, a4v::bcast<2>(vf8)); a4::vec4 res = a4v::addK<1, 1>(vacc, mul_res); vacc = a4v::dest<1, 1, 1, 1>(vacc, res); }
    { a4::vec4 mul_res = a4v::mul(vf17, a4v::bcast<3>(vf8)); a4::vec4 res = a4v::addK<1, 1>(vacc, mul_res); a4::q128 mask = a4v::set_epi32(-1, -1, -1, -1); vf10 = a4v::blendv_ps(vf10, res, a4v::castsi128_ps(mask)); }
    A4_W128(ADD32((uint32_t)a2, 0), a4v::castps_si128(vf10));
    (s1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)sp, 208))));
    (a1 = (uint64_t)((uint64_t)s0 + (uint64_t)0ULL));
    { (a0 = (uint64_t)((uint64_t)RC_U64X(s1) + (uint64_t)0ULL)); A3_CHARGE(); f0 = a9x::axisDirZY(ram, rt, (uint32_t)a0, (uint32_t)a1, (uint32_t)a2); }
    (a1 = (uint64_t)((uint64_t)s1 + (uint64_t)0ULL));
    (s0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)sp, 224))));
    (a2 = (uint64_t)((uint64_t)s2 + (uint64_t)0ULL));
    { (a0 = (uint64_t)((uint64_t)RC_U64X(s0) + (uint64_t)0ULL)); A3_CHARGE(); DivQuaternion(ram, rt, (uint32_t)a0, (uint32_t)a1, (uint32_t)a2, vf0, vf5, vf11, vf12, vf13, vf14, vf15, vf16, vacc); }
    (a0 = (uint64_t)((uint64_t)s0 + (uint64_t)0ULL));
    (at = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)16006 << 16))));
    (at = (uint64_t)(RC_U64X(at) | (uint64_t)(uint16_t)2705));
    { uint32_t bits = (uint32_t)at; std::memcpy(&f12, &bits, sizeof(bits)); }
    { (a1 = (uint64_t)((uint64_t)s0 + (uint64_t)0ULL)); A3_CHARGE(); { const a1::RetVF a2_r = GetLimitationQuaternion(ram, rt, a0, a1, sp, f0, f12, f13, f14, f15, f16, f17, f18, f19, f20, vf0, vf5, vf6, vf7, vacc, vq, vstat, h2, h3); v0 = a2_r.v0; f0 = a2_r.f0; } }
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)s7, 20))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)s7, 4))));
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)SLL32((uint32_t)v1, 4))));
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v1, (uint32_t)v0))));
    vf11 = a4v::castsi128_ps(A4_R128(ADD32((uint32_t)s2, 0)));
    vf12 = a4v::castsi128_ps(A4_R128(ADD32((uint32_t)s0, 0)));
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
    A4_W128(ADD32((uint32_t)v1, 0), a4v::castps_si128(vf13));
    (t2 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)s7, 20))));
    goto L_1462ec4;
L_1462e90:;
    (at = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)15502 << 16))));
    (at = (uint64_t)(RC_U64X(at) | (uint64_t)(uint16_t)64053));
    { uint32_t bits = (uint32_t)at; std::memcpy(&f12, &bits, sizeof(bits)); }
    f12 = a1f::mul(f20, f12);
    { f12 = a1f::add(f12, f12); A3_CHARGE(); { const a1::RetVF a2_r = GetLimitationQuaternion(ram, rt, a0, a1, sp, f0, f12, f13, f14, f15, f16, f17, f18, f19, f20, vf0, vf5, vf6, vf7, vacc, vq, vstat, h2, h3); v0 = a2_r.v0; f0 = a2_r.f0; } }
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)s7, 20))));
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)s7, 4))));
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)SLL32((uint32_t)v1, 4))));
    do { const a4::q128 _v = (A4_R128(ADD32((uint32_t)sp, 176))); h2 = a4::hi64(_v); std::memcpy(&v0, &_v, 8); } while (0);
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v1, (uint32_t)a0))));
    A4_W128(ADD32((uint32_t)v1, 0), a4::mk128(h2, v0));
    (t2 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)s7, 20))));
L_1462ec4:;
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)s7, 8))));
    (a1 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)s7, 12))));
    (a2 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)s7, 16))));
    (a3 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)s7, 4))));
    (t0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)s7, 180))));
    { (t1 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)s7, 184)))); A3_CHARGE(); uint64_t a7c[1] = {(uint64_t)a1_bits(f0)}; v0 = GetMotionMatrixOfTargetFromParent_x11(ram, rt, a0, (uint32_t)a1, (uint32_t)a2, (uint32_t)a3, (uint32_t)t0, (uint32_t)t1, t2, (uint32_t)sp, vf0, vf4, vf5, vf6, vf10, vf11, vf12, vf13, vf14, vf15, vf16, vf17, vf24, vf25, vf26, vf27, vacc, vq, vstat, h2, a7c); f0 = a1_flt((uint32_t)a7c[0]); }
    { uint32_t bits = A1_R32B((uint32_t)sp, 320); float f; std::memcpy(&f, &bits, sizeof(f)); f20 = f; }
    { (sp = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)sp, 336)))); if (xo) { xo[0] = a1_bits(f20); } io_vf4 = vf4; io_vf5 = vf5; io_vf6 = vf6; io_vf7 = vf7; io_vf8 = vf8; io_vf10 = vf10; io_vf11 = vf11; io_vf12 = vf12; io_vf13 = vf13; io_vf14 = vf14; io_vf15 = vf15; io_vf16 = vf16; io_vf17 = vf17; io_vf24 = vf24; io_vf25 = vf25; io_vf26 = vf26; io_vf27 = vf27; io_vf28 = vf28; io_vf29 = vf29; io_vacc = vacc; io_vq = vq; io_vstat = vstat; io_h2 = h2; io_h3 = h3; io_h8 = h8; io_h9 = h9; io_h10 = h10; io_h11 = h11; io_h12 = h12; io_h13 = h13; io_h14 = h14; io_h15 = h15; return a1::RetVF{(uint64_t)v0, f0}; }
    __builtin_unreachable();
}

// GetGeometryPointerSurfaceAttribute (0x1132af0): typed by tools/typed_lift2.py (round A6; guest frame calls, extra outputs xo[0]=a0 xo[1]=a1 xo[2]=a2 xo[3]=a3; VU0 in -, out h2 h3, jump tables 1)
inline uint64_t GetGeometryPointerSurfaceAttribute(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint64_t a0, uint64_t a1, uint64_t sp, float f0, float f1, float f2, float f3, float f4, float f5, float f6, float f7, float f8, float f9, float f10, float f11, float f12, float f13, float f14, float f15, float f16, float f17, float f18, float f19, float f20, float f21, float f22, float f23, float f24, float f25, float f26, float f27, float f28, float f29, float f30, float f31, uint64_t &io_h2, uint64_t &io_h3, uint64_t *xo = nullptr)
{
    uint64_t v0 = 0;
    uint64_t v1 = 0;
    uint64_t a2 = 0;
    uint64_t a3 = 0;
    uint64_t h2 = io_h2;
    uint64_t h3 = io_h3;
    bool a3xw1 = false;
    (sp = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)sp, 4294967280))));
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)a1, 48))));
    (v0 = (uint64_t)(((uint64_t)RC_U64X(v1) < (uint64_t)(int64_t)(int32_t)9) ? 1 : 0));
    if ((int64_t)v0 == 0) { (a1 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL)); a3xw1 = true; goto L_1132b24; }
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)SLL32((uint32_t)v1, 2))));
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)281 << 16))));
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v1, (uint32_t)v0))));
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)v1, 22368))));
    { const uint32_t a5_jt = (uint32_t)v1; switch (a5_jt) { case 0x1132B20u: goto L_1132b20; case 0x1132B24u: goto L_1132b24; default: A5_BADJUMP(0x1132B18u, a5_jt); } }
L_1132b20:;
    (a1 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL)); a3xw1 = true;
L_1132b24:;
    if ((int64_t)a1 == 0) goto L_1132b38;
    { (sp = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)sp, 16)))); uint64_t a7c[4] = {0u, 0u, 0u, 0u}; const uint64_t a7_t = GetCollisionDBAttribute_x11(ram, rt, v0, a0, a1, f0, f1, f2, f3, f4, f5, f6, f7, f8, f9, f10, f11, f12, f13, f14, f15, f16, f17, f18, f19, f20, f21, f22, f23, f24, f25, f26, f27, f28, f29, f30, f31, h2, h3, a7c); a0 = a7c[0]; a1 = a7c[1]; a2 = a7c[2]; a3 = a7c[3]; if (xo) { xo[0] = a0; xo[1] = a1; xo[2] = a2; xo[3] = a3; } io_h2 = h2; io_h3 = h3; return a7_t; }
L_1132b38:;
    do { const a4::q128 _v = (a4m::por(a4v::setzero_si128(), a4v::setzero_si128())); h2 = a4::hi64(_v); std::memcpy(&v0, &_v, 8); } while (0);
    A4_W128(ADD32((uint32_t)a0, 0), a4::mk128(h2, v0));
    { (sp = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)sp, 16)))); if (xo) { if (a3xw1) xo[1] = a1; } io_h2 = h2; io_h3 = h3; return v0; }
    __builtin_unreachable();
}

// manager_sub_01162FD8 (0x1162fd8): typed by tools/typed_lift2.py (round A6; guest frame calls, extra outputs xo[0]=f0 xo[1]=f1 xo[2]=f2 xo[3]=f12 xo[4]=f13 xo[5]=f14 xo[6]=f20 xo[7]=a2 xo[8]=a3 xo[9]=t0 xo[10]=t1 xo[11]=t2 xo[12]=t3; VU0 in vf0 vf2 vf6 vf7 vf9 vf10 vf12 vf13 vf14 vf15 vf16 vf17 vacc, out vf1 vf2 vf3 vf4 vf5 vf6 vf7 vf8 vf9 vf10 vf11 vf12 vf13 vf14 vf15 vf16 vf17 vf24 vf25 vf26 vf27 vacc h2 h3 h6 h7 h8 h9 h10 h11 h12 h13 h14 h15)
inline uint64_t manager_sub_01162FD8(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint64_t v0, uint64_t a0, uint64_t a1, uint64_t a2, uint64_t a3, uint64_t t0, uint64_t t1, uint64_t t2, uint64_t t3, uint32_t sp, float f0, float f1, float f2, float f3, float f4, float f5, float f6, float f7, float f8, float f9, float f10, float f11, float f12, float f13, float f14, float f15, float f16, float f17, float f18, float f19, float f20, float f21, float f22, float f23, float f24, float f25, float f26, float f27, float f28, float f29, float f30, float f31, const a4::vec4 &io_vf0, a4::vec4 &io_vf1, a4::vec4 &io_vf2, a4::vec4 &io_vf3, a4::vec4 &io_vf4, a4::vec4 &io_vf5, a4::vec4 &io_vf6, a4::vec4 &io_vf7, a4::vec4 &io_vf8, a4::vec4 &io_vf9, a4::vec4 &io_vf10, a4::vec4 &io_vf11, a4::vec4 &io_vf12, a4::vec4 &io_vf13, a4::vec4 &io_vf14, a4::vec4 &io_vf15, a4::vec4 &io_vf16, a4::vec4 &io_vf17, a4::vec4 &io_vf24, a4::vec4 &io_vf25, a4::vec4 &io_vf26, a4::vec4 &io_vf27, a4::vec4 &io_vacc, uint64_t &io_h2, uint64_t &io_h3, uint64_t &io_h6, uint64_t &io_h7, uint64_t &io_h8, uint64_t &io_h9, uint64_t &io_h10, uint64_t &io_h11, uint64_t &io_h12, uint64_t &io_h13, uint64_t &io_h14, uint64_t &io_h15, uint64_t *xo = nullptr)
{
    uint32_t at = 0;
    uint64_t v1 = 0;
    uint64_t s0 = 0;
    uint64_t s1 = 0;
    uint64_t s2 = 0;
    uint64_t s3 = 0;
    a4::vec4 vf0 = io_vf0;
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
    a4::vec4 vf24 = io_vf24;
    a4::vec4 vf25 = io_vf25;
    a4::vec4 vf26 = io_vf26;
    a4::vec4 vf27 = io_vf27;
    a4::vec4 vacc = io_vacc;
    uint64_t h2 = io_h2;
    uint64_t h3 = io_h3;
    uint64_t h6 = io_h6;
    uint64_t h7 = io_h7;
    uint64_t h8 = io_h8;
    uint64_t h9 = io_h9;
    uint64_t h10 = io_h10;
    uint64_t h11 = io_h11;
    uint64_t h12 = io_h12;
    uint64_t h13 = io_h13;
    uint64_t h14 = io_h14;
    uint64_t h15 = io_h15;
    bool a3xw2 = false;
    bool a3xw4 = false;
    bool a3xw5 = false;
    bool a3xw6 = false;
    bool a3xw7 = false;
    bool a3xw9 = false;
    bool a3xw10 = false;
    bool a3xw11 = false;
    bool a3xw12 = false;
    sp = sp - 0x60u;
    (s0 = (uint64_t)((uint64_t)a2 + (uint64_t)0ULL));
    (s3 = (uint64_t)((uint64_t)a0 + (uint64_t)0ULL));
    (a0 = (uint64_t)((uint64_t)s0 + (uint64_t)0ULL));
    (s2 = (uint64_t)((uint64_t)a1 + (uint64_t)0ULL));
    { (s1 = (uint64_t)((uint64_t)a3 + (uint64_t)0ULL)); A3_CHARGE(); uint64_t a7c[4] = {(uint64_t)a1_bits(f0), (uint64_t)a1_bits(f1), (uint64_t)a1_bits(f12), 0u}; v0 = is_have_region_camera_region_u11(ram, rt, a0, a1, a1_sx64(sp), f0, f1, f2, f3, f4, f5, f6, f7, f8, f9, f10, f11, f12, f13, f14, f15, f16, f17, f18, f19, f20, f21, f22, f23, f24, f25, f26, f27, f28, f29, f30, f31, h2, h3, a7c); f0 = a1_flt((uint32_t)a7c[0]); f1 = a1_flt((uint32_t)a7c[1]); f12 = a1_flt((uint32_t)a7c[2]); }
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 1))));
    { const bool c = (int64_t)v0 != (int64_t)v1; (a3 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL)); if (c) goto L_116314c; }
    { const bool c = (int64_t)s1 == 0; (a2 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)s0, 20)))); a3xw7 = true; if (c) goto L_1163078; }
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)s1, 16))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)s0, 16))));
    if ((int64_t)v0 != (int64_t)v1) { do { const a4::q128 _v = (A4_R128(ADD32((uint32_t)s2, 144))); h2 = a4::hi64(_v); std::memcpy(&v0, &_v, 8); } while (0); goto L_116307c; }
    { uint32_t bits = A1_R32B((uint32_t)a2, 20); float f; std::memcpy(&f, &bits, sizeof(f)); f1 = f; }
    (a1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)s2, 144))));
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(sp, 16))));
    A1_W32B(sp, 32, a1_bits(f1));
    { uint32_t bits = A1_R32B((uint32_t)a2, 20); float f; std::memcpy(&f, &bits, sizeof(f)); f0 = f; }
    A1_W32B(sp, 36, a1_bits(f0));
    { uint32_t bits = A1_R32B((uint32_t)a2, 20); float f; std::memcpy(&f, &bits, sizeof(f)); f1 = f; }
    A1_W32B(sp, 44, 0u);
    A1_W32B(sp, 40, a1_bits(f1));
    (v0 = (uint64_t)(A1_R64B(sp, 32)));
    (v1 = (uint64_t)(A1_R64B(sp, 40)));
    A1_W64B(sp, 16, v0);
    A1_W64B(sp, 24, v1);
    vf5 = a4v::castsi128_ps(A4_R128(ADD32((uint32_t)a1, 0)));
    vf6 = a4v::castsi128_ps(A4_R128(ADD32((uint32_t)a0, 0)));
    { a4::vec4 res = a4v::add(vf5, vf6); a4::q128 mask = a4v::set_epi32(0, -1, -1, -1); vf5 = a4v::blend(vf5, res, a4v::castsi128_ps(mask)); }
    A4_W128(ADD32(sp, 0), a4v::castps_si128(vf5));
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)s0, 4))));
    goto L_1163084;
L_1163078:;
    do { const a4::q128 _v = (A4_R128(ADD32((uint32_t)s2, 144))); h2 = a4::hi64(_v); std::memcpy(&v0, &_v, 8); } while (0);
L_116307c:;
    A4_W128(ADD32(sp, 0), a4::mk128(h2, v0));
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)s0, 4))));
L_1163084:;
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 40))));
    { const bool c = (int64_t)v1 == (int64_t)v0; (v0 = (uint64_t)(((int64_t)(int64_t)v1 < (int64_t)(int32_t)41) ? 1 : 0)); if (c) goto L_11630e0; }
    { const bool c = (int64_t)v0 == 0; (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 41)))); if (c) goto L_11630b0; }
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 39))));
    if ((int64_t)v1 == (int64_t)v0) { (a2 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)s2, 308)))); a3xw7 = true; goto L_11630c8; }
    goto L_1163150;
L_11630b0:;
    { const bool c = (int64_t)v1 == (int64_t)v0; (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 42)))); if (c) goto L_1163100; }
    if ((int64_t)v1 == (int64_t)v0) { (a2 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)s2, 308)))); a3xw7 = true; goto L_1163128; }
    goto L_1163150;
L_11630c8:;
    (a3 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)s0, 32))));
    f12 = a1_flt(A1_R32(sp));
    (a1 = (uint64_t)((uint64_t)s3 + (uint64_t)0ULL));
    f13 = a1_flt(A1_R32B(sp, 4)); a3xw4 = true;
    (a0 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL));
    goto L_116313c;
L_11630e0:;
    (a2 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)s2, 308)))); a3xw7 = true;
    (a3 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)s0, 32))));
    f12 = a1_flt(A1_R32(sp));
    (a1 = (uint64_t)((uint64_t)s3 + (uint64_t)0ULL));
    f13 = a1_flt(A1_R32B(sp, 4)); a3xw4 = true;
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 3))));
    goto L_116313c;
L_1163100:;
    (a2 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)s2, 308)))); a3xw7 = true;
    (a3 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)s0, 32))));
    (at = (uint32_t)((int32_t)((int32_t)((uint32_t)16384 << 16))));
    { uint32_t bits = a7_opq(at); std::memcpy(&f12, &bits, sizeof(bits)); }
    (a1 = (uint64_t)((uint64_t)s3 + (uint64_t)0ULL));
    f13 = a1_flt(A1_R32(sp)); a3xw4 = true;
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 2))));
    f14 = a1_flt(A1_R32B(sp, 4)); a3xw5 = true;
    goto L_1163140;
L_1163128:;
    (a3 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)s0, 32))));
    f12 = a1_flt(A1_R32(sp));
    (a1 = (uint64_t)((uint64_t)s3 + (uint64_t)0ULL));
    f13 = a1_flt(A1_R32B(sp, 4)); a3xw4 = true;
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 1))));
L_116313c:;
    f14 = a1_flt(A1_R32B(sp, 8)); a3xw5 = true;
L_1163140:;
    { A3_CHARGE(); uint64_t a7c[10] = {(uint64_t)a1_bits(f0), (uint64_t)a1_bits(f1), (uint64_t)a1_bits(f2), (uint64_t)a1_bits(f12), 0u, a2, t0, t1, t2, t3}; v0 = IsInsideOfRegion(ram, rt, a0, a1, a2, (uint32_t)a3, sp, f0, f1, f2, f3, f4, f5, f6, f7, f8, f9, f10, f11, f12, f13, f14, f15, f16, f17, f18, f19, f20, f21, f22, f23, f24, f25, f26, f27, f28, f29, f30, f31, vf0, vf1, vf2, vf3, vf4, vf5, vf6, vf7, vf8, vf9, vf10, vf11, vf12, vf13, vf14, vf15, vf16, vf17, vf24, vf25, vf26, vf27, vacc, h2, h6, h7, h8, h9, h10, h11, h12, h13, h14, h15, a7c); f0 = a1_flt((uint32_t)a7c[0]); f1 = a1_flt((uint32_t)a7c[1]); f2 = a1_flt((uint32_t)a7c[2]); f12 = a1_flt((uint32_t)a7c[3]); f20 = a1_flt((uint32_t)a7c[4]); a2 = a7c[5]; t0 = a7c[6]; t1 = a7c[7]; t2 = a7c[8]; t3 = a7c[9]; a3xw2 = true; a3xw6 = true; a3xw7 = true; a3xw9 = true; a3xw10 = true; a3xw11 = true; a3xw12 = true; }
    (a3 = (uint64_t)((uint64_t)v0 + (uint64_t)0ULL));
L_116314c:;
L_1163150:;
    (v0 = (uint64_t)((uint64_t)a3 + (uint64_t)0ULL));
    { sp = sp + 0x60u; if (xo) { xo[0] = a1_bits(f0); xo[1] = a1_bits(f1); if (a3xw2) xo[2] = a1_bits(f2); xo[3] = a1_bits(f12); if (a3xw4) xo[4] = a1_bits(f13); if (a3xw5) xo[5] = a1_bits(f14); if (a3xw6) xo[6] = a1_bits(f20); if (a3xw7) xo[7] = a2; xo[8] = a3; if (a3xw9) xo[9] = t0; if (a3xw10) xo[10] = t1; if (a3xw11) xo[11] = t2; if (a3xw12) xo[12] = t3; } io_vf1 = vf1; io_vf2 = vf2; io_vf3 = vf3; io_vf4 = vf4; io_vf5 = vf5; io_vf6 = vf6; io_vf7 = vf7; io_vf8 = vf8; io_vf9 = vf9; io_vf10 = vf10; io_vf11 = vf11; io_vf12 = vf12; io_vf13 = vf13; io_vf14 = vf14; io_vf15 = vf15; io_vf16 = vf16; io_vf17 = vf17; io_vf24 = vf24; io_vf25 = vf25; io_vf26 = vf26; io_vf27 = vf27; io_vacc = vacc; io_h2 = h2; io_h3 = h3; io_h6 = h6; io_h7 = h7; io_h8 = h8; io_h9 = h9; io_h10 = h10; io_h11 = h11; io_h12 = h12; io_h13 = h13; io_h14 = h14; io_h15 = h15; return v0; }
    __builtin_unreachable();
}

// SCRFuncYamoriBSendFallKey (0x140b538): typed by tools/typed_lift2.py (round A6; guest frame calls; VU0 in vf0 vf7 vf10 vf12 vf13 vf14 vf15 vf16 vf17 vf25 vf28 vf29 vacc vstat h5, out vf5 vf7 vf8 vf10 vf11 vf12 vf13 vf14 vf15 vf16 vf17 vf24 vf25 vf26 vf28 vf29 vacc vq vstat h2 h8 h9 h10 h11 h12 h13 h14 h15, relocated calls 5)
inline a1::RetVF SCRFuncYamoriBSendFallKey(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint64_t v0, uint64_t a0, uint64_t a1, uint64_t sp, float f0, float f3, float f4, float f5, float f6, float f7, float f8, float f9, float f10, float f11, float f12, float f13, float f14, float f15, float f16, float f17, float f18, float f19, float f20, float f21, float f22, float f23, float f24, float f25, float f26, float f27, float f28, float f29, float f30, float f31, const a4::vec4 &io_vf0, a4::vec4 &io_vf5, a4::vec4 &io_vf7, a4::vec4 &io_vf8, a4::vec4 &io_vf10, a4::vec4 &io_vf11, a4::vec4 &io_vf12, a4::vec4 &io_vf13, a4::vec4 &io_vf14, a4::vec4 &io_vf15, a4::vec4 &io_vf16, a4::vec4 &io_vf17, a4::vec4 &io_vf24, a4::vec4 &io_vf25, a4::vec4 &io_vf26, a4::vec4 &io_vf28, a4::vec4 &io_vf29, a4::vec4 &io_vacc, float &io_vq, uint16_t &io_vstat, uint64_t &io_h2, uint64_t io_h5, uint64_t &io_h8, uint64_t &io_h9, uint64_t &io_h10, uint64_t &io_h11, uint64_t &io_h12, uint64_t &io_h13, uint64_t &io_h14, uint64_t &io_h15)
{
    float f1 = 0.0f;
    float f2 = 0.0f;
    uint64_t at = 0;
    uint64_t v1 = 0;
    uint64_t t0 = 0;
    uint64_t t1 = 0;
    uint64_t t2 = 0;
    uint64_t t3 = 0;
    uint64_t t4 = 0;
    uint64_t t5 = 0;
    uint64_t t6 = 0;
    uint64_t t7 = 0;
    uint64_t s0 = 0;
    uint32_t fcr31 = 0;
    a4::vec4 vf0 = io_vf0;
    a4::vec4 vf5 = io_vf5;
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
    a4::vec4 vf24 = io_vf24;
    a4::vec4 vf25 = io_vf25;
    a4::vec4 vf26 = io_vf26;
    a4::vec4 vf28 = io_vf28;
    a4::vec4 vf29 = io_vf29;
    a4::vec4 vacc = io_vacc;
    float vq = io_vq;
    uint16_t vstat = io_vstat;
    uint64_t h2 = io_h2;
    uint64_t h5 = io_h5;
    uint64_t h8 = io_h8;
    uint64_t h9 = io_h9;
    uint64_t h10 = io_h10;
    uint64_t h11 = io_h11;
    uint64_t h12 = io_h12;
    uint64_t h13 = io_h13;
    uint64_t h14 = io_h14;
    uint64_t h15 = io_h15;
    (sp = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)sp, 4294967200))));
    { (s0 = (uint64_t)((uint64_t)a0 + (uint64_t)0ULL)); A3_CHARGE(); v0 = GetCharacterClass(ram, rt, a0, h2, h5); }
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v0, 1136))));
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)sp, 16))));
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
    A4_W128(ADD32((uint32_t)v1, 0), a4v::castps_si128(vf14));
    A4_W128(ADD32((uint32_t)v1, 16), a4v::castps_si128(vf15));
    A4_W128(ADD32((uint32_t)v1, 32), a4v::castps_si128(vf16));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)sp, 64))));
    A4_W128(ADD32((uint32_t)v0, 0), a4v::castps_si128(vf0));
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)sp, 16))));
    vf24 = a4v::castsi128_ps(A4_R128(ADD32((uint32_t)v1, 48)));
    { a4::vec4 res = a4v::subK<1, 1>(vf0, a4v::bcast<3>(vf0)); a4::q128 mask = a4v::set_epi32(0, 0, 0, -1); vf25 = a4v::blendv_ps(vf25, res, a4v::castsi128_ps(mask)); }
    { a4::vec4 res = a4v::mulK<0, 1>(vf24, a4v::bcast<0>(vf25)); a4::q128 mask = a4v::set_epi32(0, -1, -1, -1); vf28 = a4v::blendv_ps(vf28, res, a4v::castsi128_ps(mask)); }
    { a4::vec4 res = a4v::sub(vf28, vf28); a4::q128 mask = a4v::set_epi32(-1, 0, 0, 0); vf28 = a4v::blend(vf28, res, a4v::castsi128_ps(mask)); }
    do { const a4::q128 _v = (A4_R128(ADD32((uint32_t)v1, 0))); h8 = a4::hi64(_v); std::memcpy(&t0, &_v, 8); } while (0);
    do { const a4::q128 _v = (A4_R128(ADD32((uint32_t)v1, 16))); h9 = a4::hi64(_v); std::memcpy(&t1, &_v, 8); } while (0);
    do { const a4::q128 _v = (A4_R128(ADD32((uint32_t)v1, 32))); h10 = a4::hi64(_v); std::memcpy(&t2, &_v, 8); } while (0);
    do { const a4::q128 _v = (a4v::castps_si128(vf0)); h11 = a4::hi64(_v); std::memcpy(&t3, &_v, 8); } while (0);
    do { const a4::q128 _v = (a4m::pextlw(a4::mk128(h9, t1), a4::mk128(h8, t0))); h12 = a4::hi64(_v); std::memcpy(&t4, &_v, 8); } while (0);
    do { const a4::q128 _v = (a4m::pextuw(a4::mk128(h9, t1), a4::mk128(h8, t0))); h13 = a4::hi64(_v); std::memcpy(&t5, &_v, 8); } while (0);
    do { const a4::q128 _v = (a4m::pextlw(a4::mk128(h11, t3), a4::mk128(h10, t2))); h14 = a4::hi64(_v); std::memcpy(&t6, &_v, 8); } while (0);
    do { const a4::q128 _v = (a4m::pextuw(a4::mk128(h11, t3), a4::mk128(h10, t2))); h15 = a4::hi64(_v); std::memcpy(&t7, &_v, 8); } while (0);
    do { const a4::q128 _v = (a4m::pcpyld(a4::mk128(h14, t6), a4::mk128(h12, t4))); h8 = a4::hi64(_v); std::memcpy(&t0, &_v, 8); } while (0);
    do { const a4::q128 _v = (a4v::unpackhi_epi64(a4::mk128(h12, t4), a4::mk128(h14, t6))); h9 = a4::hi64(_v); std::memcpy(&t1, &_v, 8); } while (0);
    do { const a4::q128 _v = (a4m::pcpyld(a4::mk128(h15, t7), a4::mk128(h13, t5))); h10 = a4::hi64(_v); std::memcpy(&t2, &_v, 8); } while (0);
    do { const a4::q128 _v = (a4v::unpackhi_epi64(a4::mk128(h13, t5), a4::mk128(h15, t7))); h11 = a4::hi64(_v); std::memcpy(&t3, &_v, 8); } while (0);
    vf24 = a4v::castsi128_ps(a4::mk128(h8, t0));
    vf25 = a4v::castsi128_ps(a4::mk128(h9, t1));
    vf26 = a4v::castsi128_ps(a4::mk128(h10, t2));
    { a4::vec4 res = a4v::mulK<0, 1>(vf24, a4v::bcast<0>(vf28)); vacc = a4v::dest<1, 1, 1, 1>(vacc, res); }
    { a4::vec4 mul_res = a4v::mulK<0, 1>(vf25, a4v::bcast<1>(vf28)); a4::vec4 res = a4v::addK<1, 1>(vacc, mul_res); vacc = a4v::dest<1, 1, 1, 1>(vacc, res); }
    { a4::vec4 mul_res = a4v::mulK<0, 1>(vf26, a4v::bcast<2>(vf28)); a4::vec4 res = a4v::addK<1, 1>(vacc, mul_res); vacc = a4v::dest<1, 1, 1, 1>(vacc, res); }
    { a4::vec4 mul_res = a4v::mulK<1, 1>(vf0, a4v::bcast<3>(vf28)); a4::vec4 res = a4v::addK<1, 1>(vacc, mul_res); a4::q128 mask = a4v::set_epi32(-1, -1, -1, -1); vf29 = a4v::blendv_ps(vf29, res, a4v::castsi128_ps(mask)); }
    { a4::vec4 res = a4v::subK<1, 1>(vf29, vf29); a4::q128 mask = a4v::set_epi32(-1, 0, 0, 0); vf29 = a4v::blend(vf29, res, a4v::castsi128_ps(mask)); }
    { a4::vec4 res = a4v::addK<1, 1>(vf29, vf0); a4::q128 mask = a4v::set_epi32(-1, 0, 0, 0); vf29 = a4v::blend(vf29, res, a4v::castsi128_ps(mask)); }
    A4_W128(ADD32((uint32_t)v1, 0), a4v::castps_si128(vf24));
    A4_W128(ADD32((uint32_t)v1, 16), a4v::castps_si128(vf25));
    A4_W128(ADD32((uint32_t)v1, 32), a4v::castps_si128(vf26));
    A4_W128(ADD32((uint32_t)v1, 48), a4v::castps_si128(vf29));
    { const uint32_t _dynInsn = A1_R32(0x140B640u); const int32_t _dynSimm = (int16_t)(_dynInsn & 0xFFFFu); const uint32_t _dynImm = _dynInsn & 0xFFFFu; (void)_dynSimm; (void)_dynImm; (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)_dynImm << 16)))); }
    { const uint32_t _dynInsn = A1_R32(0x140B644u); const int32_t _dynSimm = (int16_t)(_dynInsn & 0xFFFFu); const uint32_t _dynImm = _dynInsn & 0xFFFFu; (void)_dynSimm; (void)_dynImm; (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v0, _dynSimm)))); }
    vf8 = a4v::castsi128_ps(A4_R128(ADD32((uint32_t)v0, 0)));
    vf14 = a4v::castsi128_ps(A4_R128(ADD32((uint32_t)v1, 0)));
    vf15 = a4v::castsi128_ps(A4_R128(ADD32((uint32_t)v1, 16)));
    vf16 = a4v::castsi128_ps(A4_R128(ADD32((uint32_t)v1, 32)));
    vf17 = a4v::castsi128_ps(A4_R128(ADD32((uint32_t)v1, 48)));
    { a4::vec4 res = a4v::mul(vf14, a4v::bcast<0>(vf8)); vacc = a4v::dest<1, 1, 1, 1>(vacc, res); }
    { a4::vec4 mul_res = a4v::mul(vf15, a4v::bcast<1>(vf8)); a4::vec4 res = a4v::addK<1, 1>(vacc, mul_res); vacc = a4v::dest<1, 1, 1, 1>(vacc, res); }
    { a4::vec4 mul_res = a4v::mul(vf16, a4v::bcast<2>(vf8)); a4::vec4 res = a4v::addK<1, 1>(vacc, mul_res); vacc = a4v::dest<1, 1, 1, 1>(vacc, res); }
    { a4::vec4 mul_res = a4v::mul(vf17, a4v::bcast<3>(vf8)); a4::vec4 res = a4v::addK<1, 1>(vacc, mul_res); a4::q128 mask = a4v::set_epi32(-1, -1, -1, -1); vf10 = a4v::blendv_ps(vf10, res, a4v::castsi128_ps(mask)); }
    A4_W128(ADD32((uint32_t)sp, 0), a4v::castps_si128(vf10));
    A1_W32B((uint32_t)sp, 4, 0u);
    vf5 = a4v::castsi128_ps(A4_R128(ADD32((uint32_t)sp, 0)));
    { a4::vec4 res = a4v::mul(vf5, vf5); a4::q128 mask = a4v::set_epi32(0, -1, -1, -1); vf7 = a4v::blend(vf7, res, a4v::castsi128_ps(mask)); }
    { a4::vec4 res = a4v::mulK<1, 1>(vf0, a4v::bcast<0>(vf7)); vacc = a4v::dest<0, 0, 0, 1>(vacc, res); }
    { a4::vec4 mul_res = a4v::mulK<1, 1>(vf0, a4v::bcast<1>(vf7)); a4::vec4 res = a4v::addK<1, 1>(vacc, mul_res); vacc = a4v::dest<0, 0, 0, 1>(vacc, res); }
    { a4::vec4 mul_res = a4v::mulK<1, 1>(vf0, a4v::bcast<2>(vf7)); a4::vec4 res = a4v::addK<1, 1>(vacc, mul_res); a4::q128 mask = a4v::set_epi32(-1, 0, 0, 0); vf7 = a4v::blendv_ps(vf7, res, a4v::castsi128_ps(mask)); }
    { float fs = a4v::cvtss_f32(a4v::shuffle_ps<0x03>(vf0, vf0)); float ft = a4v::cvtss_f32(a4v::shuffle_ps<0x03>(vf7, vf7)); vq = a4v::rsqrt(vstat, fs, ft); }
    { a4::vec4 res = a4v::mul(vf5, a4v::set1_ps(vq)); a4::q128 mask = a4v::set_epi32(0, -1, -1, -1); vf5 = a4v::blendv_ps(vf5, res, a4v::castsi128_ps(mask)); }
    A4_W128(ADD32((uint32_t)sp, 0), a4v::castps_si128(vf5));
    { (a0 = (uint64_t)((uint64_t)sp + (uint64_t)0ULL)); A3_CHARGE(); uint64_t a7c[2] = {0u, 0u}; f0 = SCRGetXZAngle(ram, rt, (uint32_t)a0, f14, f15, f16, f17, f18, f19, a7c); f12 = a1_flt((uint32_t)a7c[0]); f13 = a1_flt((uint32_t)a7c[1]); }
    (at = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)16997 << 16))));
    (at = (uint64_t)(RC_U64X(at) | (uint64_t)(uint16_t)12000));
    { uint32_t bits = (uint32_t)at; std::memcpy(&f1, &bits, sizeof(bits)); }
    (at = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)16820 << 16))));
    { uint32_t bits = (uint32_t)at; std::memcpy(&f2, &bits, sizeof(bits)); }
    f1 = a1f::mul(f0, f1);
    f0 = std::fabs((float)(f1));
    fcr31 = (a1f::le(f0, f2)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
    if ((fcr31 & 0x800000u) == 0u) goto L_140b708;
    { (a0 = (uint64_t)((uint64_t)s0 + (uint64_t)0ULL)); A3_CHARGE(); v0 = GetCharacterClass(ram, rt, a0, h2, h5); }
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)480 << 16))));
    (a0 = (uint64_t)(A1_R64B((uint32_t)v0, 1680)));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)96 << 16))));
    (a0 = (uint64_t)(a0 & RC_U64X(v1)));
    { const bool c = (int64_t)a0 != (int64_t)v0; { const uint32_t _dynInsn = A1_R32(0x140B6E4u); const int32_t _dynSimm = (int16_t)(_dynInsn & 0xFFFFu); const uint32_t _dynImm = _dynInsn & 0xFFFFu; (void)_dynSimm; (void)_dynImm; (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)_dynImm << 16)))); } if (c) goto L_140b6f8; }
    { const uint32_t _dynInsn = A1_R32(0x140B6E8u); const int32_t _dynSimm = (int16_t)(_dynInsn & 0xFFFFu); const uint32_t _dynImm = _dynInsn & 0xFFFFu; (void)_dynSimm; (void)_dynImm; (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)_dynImm << 16)))); }
    (a0 = (uint64_t)((uint64_t)s0 + (uint64_t)0ULL));
    { const uint32_t _dynInsn = A1_R32(0x140B6F4u); const int32_t _dynSimm = (int16_t)(_dynInsn & 0xFFFFu); const uint32_t _dynImm = _dynInsn & 0xFFFFu; (void)_dynSimm; (void)_dynImm; (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32(ADD32((uint32_t)v1, _dynSimm))))); }
    goto L_140b87c;
L_140b6f8:;
    (a0 = (uint64_t)((uint64_t)s0 + (uint64_t)0ULL));
    { const uint32_t _dynInsn = A1_R32(0x140B700u); const int32_t _dynSimm = (int16_t)(_dynInsn & 0xFFFFu); const uint32_t _dynImm = _dynInsn & 0xFFFFu; (void)_dynSimm; (void)_dynImm; (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32(ADD32((uint32_t)v1, _dynSimm))))); }
    goto L_140b87c;
L_140b708:;
    fcr31 = (a1f::le(f2, f1)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
    if ((fcr31 & 0x800000u) == 0u) goto L_140b738;
    (at = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)17031 << 16))));
    { uint32_t bits = (uint32_t)at; std::memcpy(&f0, &bits, sizeof(bits)); }
    fcr31 = (a1f::le(f1, f0)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
    { const bool c = (fcr31 & 0x800000u) == 0u; { const uint32_t _dynInsn = A1_R32(0x140B724u); const int32_t _dynSimm = (int16_t)(_dynInsn & 0xFFFFu); const uint32_t _dynImm = _dynInsn & 0xFFFFu; (void)_dynSimm; (void)_dynImm; (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)_dynImm << 16)))); } if (c) goto L_140b738; }
    (a0 = (uint64_t)((uint64_t)s0 + (uint64_t)0ULL));
    { const uint32_t _dynInsn = A1_R32(0x140B730u); const int32_t _dynSimm = (int16_t)(_dynInsn & 0xFFFFu); const uint32_t _dynImm = _dynInsn & 0xFFFFu; (void)_dynSimm; (void)_dynImm; (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32(ADD32((uint32_t)v1, _dynSimm))))); }
    goto L_140b87c;
L_140b738:;
    (at = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)49588 << 16))));
    { uint32_t bits = (uint32_t)at; std::memcpy(&f0, &bits, sizeof(bits)); }
    fcr31 = (a1f::le(f1, f0)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
    if ((fcr31 & 0x800000u) == 0u) goto L_140b770;
    (at = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)49799 << 16))));
    { uint32_t bits = (uint32_t)at; std::memcpy(&f0, &bits, sizeof(bits)); }
    fcr31 = (a1f::le(f0, f1)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
    { const bool c = (fcr31 & 0x800000u) == 0u; { const uint32_t _dynInsn = A1_R32(0x140B75Cu); const int32_t _dynSimm = (int16_t)(_dynInsn & 0xFFFFu); const uint32_t _dynImm = _dynInsn & 0xFFFFu; (void)_dynSimm; (void)_dynImm; (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)_dynImm << 16)))); } if (c) goto L_140b770; }
    (a0 = (uint64_t)((uint64_t)s0 + (uint64_t)0ULL));
    { const uint32_t _dynInsn = A1_R32(0x140B768u); const int32_t _dynSimm = (int16_t)(_dynInsn & 0xFFFFu); const uint32_t _dynImm = _dynInsn & 0xFFFFu; (void)_dynSimm; (void)_dynImm; (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32(ADD32((uint32_t)v1, _dynSimm))))); }
    goto L_140b87c;
L_140b770:;
    (at = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)17031 << 16))));
    { uint32_t bits = (uint32_t)at; std::memcpy(&f0, &bits, sizeof(bits)); }
    fcr31 = (a1f::le(f0, f1)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
    if ((fcr31 & 0x800000u) == 0u) goto L_140b7a8;
    (at = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)17121 << 16))));
    { uint32_t bits = (uint32_t)at; std::memcpy(&f0, &bits, sizeof(bits)); }
    fcr31 = (a1f::le(f1, f0)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
    { const bool c = (fcr31 & 0x800000u) == 0u; { const uint32_t _dynInsn = A1_R32(0x140B794u); const int32_t _dynSimm = (int16_t)(_dynInsn & 0xFFFFu); const uint32_t _dynImm = _dynInsn & 0xFFFFu; (void)_dynSimm; (void)_dynImm; (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)_dynImm << 16)))); } if (c) goto L_140b7a8; }
    (a0 = (uint64_t)((uint64_t)s0 + (uint64_t)0ULL));
    { const uint32_t _dynInsn = A1_R32(0x140B7A0u); const int32_t _dynSimm = (int16_t)(_dynInsn & 0xFFFFu); const uint32_t _dynImm = _dynInsn & 0xFFFFu; (void)_dynSimm; (void)_dynImm; (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32(ADD32((uint32_t)v1, _dynSimm))))); }
    goto L_140b87c;
L_140b7a8:;
    (at = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)49799 << 16))));
    { uint32_t bits = (uint32_t)at; std::memcpy(&f0, &bits, sizeof(bits)); }
    fcr31 = (a1f::le(f1, f0)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
    if ((fcr31 & 0x800000u) == 0u) goto L_140b7e0;
    (at = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)49889 << 16))));
    { uint32_t bits = (uint32_t)at; std::memcpy(&f0, &bits, sizeof(bits)); }
    fcr31 = (a1f::le(f0, f1)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
    { const bool c = (fcr31 & 0x800000u) == 0u; { const uint32_t _dynInsn = A1_R32(0x140B7CCu); const int32_t _dynSimm = (int16_t)(_dynInsn & 0xFFFFu); const uint32_t _dynImm = _dynInsn & 0xFFFFu; (void)_dynSimm; (void)_dynImm; (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)_dynImm << 16)))); } if (c) goto L_140b7e0; }
    (a0 = (uint64_t)((uint64_t)s0 + (uint64_t)0ULL));
    { const uint32_t _dynInsn = A1_R32(0x140B7D8u); const int32_t _dynSimm = (int16_t)(_dynInsn & 0xFFFFu); const uint32_t _dynImm = _dynInsn & 0xFFFFu; (void)_dynSimm; (void)_dynImm; (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32(ADD32((uint32_t)v1, _dynSimm))))); }
    goto L_140b87c;
L_140b7e0:;
    (at = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)17121 << 16))));
    { uint32_t bits = (uint32_t)at; std::memcpy(&f0, &bits, sizeof(bits)); }
    fcr31 = (a1f::le(f0, f1)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
    if ((fcr31 & 0x800000u) == 0u) goto L_140b818;
    (at = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)17181 << 16))));
    (at = (uint64_t)(RC_U64X(at) | (uint64_t)(uint16_t)32768));
    { uint32_t bits = (uint32_t)at; std::memcpy(&f0, &bits, sizeof(bits)); }
    fcr31 = (a1f::le(f1, f0)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
    { const bool c = (fcr31 & 0x800000u) == 0u; { const uint32_t _dynInsn = A1_R32(0x140B808u); const int32_t _dynSimm = (int16_t)(_dynInsn & 0xFFFFu); const uint32_t _dynImm = _dynInsn & 0xFFFFu; (void)_dynSimm; (void)_dynImm; (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)_dynImm << 16)))); } if (c) goto L_140b818; }
    (a0 = (uint64_t)((uint64_t)s0 + (uint64_t)0ULL));
    { const uint32_t _dynInsn = A1_R32(0x140B814u); const int32_t _dynSimm = (int16_t)(_dynInsn & 0xFFFFu); const uint32_t _dynImm = _dynInsn & 0xFFFFu; (void)_dynSimm; (void)_dynImm; (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32(ADD32((uint32_t)v1, _dynSimm))))); }
    goto L_140b87c;
L_140b818:;
    (at = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)49889 << 16))));
    { uint32_t bits = (uint32_t)at; std::memcpy(&f0, &bits, sizeof(bits)); }
    fcr31 = (a1f::le(f1, f0)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
    if ((fcr31 & 0x800000u) == 0u) goto L_140b850;
    (at = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)49949 << 16))));
    (at = (uint64_t)(RC_U64X(at) | (uint64_t)(uint16_t)32768));
    { uint32_t bits = (uint32_t)at; std::memcpy(&f0, &bits, sizeof(bits)); }
    fcr31 = (a1f::le(f0, f1)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
    { const bool c = (fcr31 & 0x800000u) == 0u; { const uint32_t _dynInsn = A1_R32(0x140B840u); const int32_t _dynSimm = (int16_t)(_dynInsn & 0xFFFFu); const uint32_t _dynImm = _dynInsn & 0xFFFFu; (void)_dynSimm; (void)_dynImm; (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)_dynImm << 16)))); } if (c) goto L_140b850; }
    (a0 = (uint64_t)((uint64_t)s0 + (uint64_t)0ULL));
    { const uint32_t _dynInsn = A1_R32(0x140B84Cu); const int32_t _dynSimm = (int16_t)(_dynInsn & 0xFFFFu); const uint32_t _dynImm = _dynInsn & 0xFFFFu; (void)_dynSimm; (void)_dynImm; (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32(ADD32((uint32_t)v1, _dynSimm))))); }
    goto L_140b87c;
L_140b850:;
    { (a0 = (uint64_t)((uint64_t)s0 + (uint64_t)0ULL)); A3_CHARGE(); v0 = GetCharacterClass(ram, rt, a0, h2, h5); }
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)480 << 16))));
    (a0 = (uint64_t)(A1_R64B((uint32_t)v0, 1680)));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)96 << 16))));
    (a0 = (uint64_t)(a0 & RC_U64X(v1)));
    { const bool c = (int64_t)a0 != (int64_t)v0; { const uint32_t _dynInsn = A1_R32(0x140B86Cu); const int32_t _dynSimm = (int16_t)(_dynInsn & 0xFFFFu); const uint32_t _dynImm = _dynInsn & 0xFFFFu; (void)_dynSimm; (void)_dynImm; (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)_dynImm << 16)))); } if (c) goto L_140b890; }
    { const uint32_t _dynInsn = A1_R32(0x140B870u); const int32_t _dynSimm = (int16_t)(_dynInsn & 0xFFFFu); const uint32_t _dynImm = _dynInsn & 0xFFFFu; (void)_dynSimm; (void)_dynImm; (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)_dynImm << 16)))); }
    (a0 = (uint64_t)((uint64_t)s0 + (uint64_t)0ULL));
    { const uint32_t _dynInsn = A1_R32(0x140B878u); const int32_t _dynSimm = (int16_t)(_dynInsn & 0xFFFFu); const uint32_t _dynImm = _dynInsn & 0xFFFFu; (void)_dynSimm; (void)_dynImm; (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32(ADD32((uint32_t)v1, _dynSimm))))); }
L_140b87c:;
    { (a1 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)v0, 4)))); A3_CHARGE(); v0 = CharaCtrlSetScriptCtrlStatus_u11(ram, rt, v0, a0, a1, sp, f0, f1, f2, f3, f4, f5, f6, f7, f8, f9, f10, f11, f12, f13, f14, f15, f16, f17, f18, f19, f20, f21, f22, f23, f24, f25, f26, f27, f28, f29, f30, f31, h2, h5); }
    goto L_140b8a4;
L_140b890:;
    (a0 = (uint64_t)((uint64_t)s0 + (uint64_t)0ULL));
    { const uint32_t _dynInsn = A1_R32(0x140B894u); const int32_t _dynSimm = (int16_t)(_dynInsn & 0xFFFFu); const uint32_t _dynImm = _dynInsn & 0xFFFFu; (void)_dynSimm; (void)_dynImm; (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32(ADD32((uint32_t)v1, _dynSimm))))); }
    { (a1 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)v0, 4)))); A3_CHARGE(); v0 = CharaCtrlSetScriptCtrlStatus_u11(ram, rt, v0, a0, a1, sp, f0, f1, f2, f3, f4, f5, f6, f7, f8, f9, f10, f11, f12, f13, f14, f15, f16, f17, f18, f19, f20, f21, f22, f23, f24, f25, f26, f27, f28, f29, f30, f31, h2, h5); }
L_140b8a4:;
    { (sp = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)sp, 96)))); io_vf5 = vf5; io_vf7 = vf7; io_vf8 = vf8; io_vf10 = vf10; io_vf11 = vf11; io_vf12 = vf12; io_vf13 = vf13; io_vf14 = vf14; io_vf15 = vf15; io_vf16 = vf16; io_vf17 = vf17; io_vf24 = vf24; io_vf25 = vf25; io_vf26 = vf26; io_vf28 = vf28; io_vf29 = vf29; io_vacc = vacc; io_vq = vq; io_vstat = vstat; io_h2 = h2; io_h8 = h8; io_h9 = h9; io_h10 = h10; io_h11 = h11; io_h12 = h12; io_h13 = h13; io_h14 = h14; io_h15 = h15; return a1::RetVF{(uint64_t)v0, f0}; }
    __builtin_unreachable();
}

// CAM_get_new_look_at_posi_from_camera_posi_and_rotXY (0x1419ca8): typed by tools/typed_lift2.py (round A6; guest frame calls, extra outputs xo[0]=f12 xo[1]=f13 xo[2]=f20 xo[3]=f21 xo[4]=f22 xo[5]=f23 xo[6]=a3 xo[7]=t0 xo[8]=t1 xo[9]=t2 xo[10]=t3; VU0 in vf0 vf2 vf7 vf9 vf10 vf12 vf13 vf14 vf15 vf16 vf17 vacc vstat h3 h4, out vf1 vf2 vf3 vf4 vf5 vf6 vf7 vf8 vf9 vf10 vf11 vf12 vf13 vf14 vf15 vf16 vf17 vacc vq vstat h2 h3 h6 h7 h8 h9 h10 h11 h12 h13 h14 h15, relocated calls 10)
inline a1::RetVF CAM_get_new_look_at_posi_from_camera_posi_and_rotXY(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint64_t v0, uint64_t a0, uint64_t a1, uint64_t a2, uint64_t a3, uint64_t t0, uint64_t sp, float f12, float f13, float f14, float f15, float f16, float f17, float f18, float f19, float f20, float f21, float f22, float f23, const a4::vec4 &io_vf0, a4::vec4 &io_vf1, a4::vec4 &io_vf2, a4::vec4 &io_vf3, a4::vec4 &io_vf4, a4::vec4 &io_vf5, a4::vec4 &io_vf6, a4::vec4 &io_vf7, a4::vec4 &io_vf8, a4::vec4 &io_vf9, a4::vec4 &io_vf10, a4::vec4 &io_vf11, a4::vec4 &io_vf12, a4::vec4 &io_vf13, a4::vec4 &io_vf14, a4::vec4 &io_vf15, a4::vec4 &io_vf16, a4::vec4 &io_vf17, a4::vec4 &io_vacc, float &io_vq, uint16_t &io_vstat, uint64_t &io_h2, uint64_t &io_h3, uint64_t io_h4, uint64_t &io_h6, uint64_t &io_h7, uint64_t &io_h8, uint64_t &io_h9, uint64_t &io_h10, uint64_t &io_h11, uint64_t &io_h12, uint64_t &io_h13, uint64_t &io_h14, uint64_t &io_h15, uint64_t *xo = nullptr)
{
    float f0 = 0.0f;
    uint32_t at = 0;
    uint64_t v1 = 0;
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
    a4::vec4 vf0 = io_vf0;
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
    uint64_t h10 = io_h10;
    uint64_t h11 = io_h11;
    uint64_t h12 = io_h12;
    uint64_t h13 = io_h13;
    uint64_t h14 = io_h14;
    uint64_t h15 = io_h15;
    bool a3xw6 = false;
    bool a3xw7 = false;
    bool a3xw8 = false;
    bool a3xw9 = false;
    bool a3xw10 = false;
    (sp = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)sp, 4294967088))));
    { float f = f20; uint32_t bits; std::memcpy(&bits, &f, sizeof(bits)); A1_W32B((uint32_t)sp, 176, bits); }
    (at = (uint32_t)((int32_t)((int32_t)((uint32_t)14545 << 16))));
    (at = (uint32_t)(a1_sx64(at) | (uint64_t)(uint16_t)46871));
    { uint32_t bits = a7_opq(at); std::memcpy(&f20, &bits, sizeof(bits)); }
    { float f = f21; uint32_t bits; std::memcpy(&bits, &f, sizeof(bits)); A1_W32B((uint32_t)sp, 184, bits); }
    f21 = ((float)(f13));
    f13 = ((float)(f20));
    (s0 = (uint64_t)((uint64_t)a2 + (uint64_t)0ULL));
    (s3 = (uint64_t)((uint64_t)a1 + (uint64_t)0ULL));
    (s4 = (uint64_t)((uint64_t)a0 + (uint64_t)0ULL));
    (s5 = (uint64_t)((uint64_t)a3 + (uint64_t)0ULL));
    (s6 = (uint64_t)((uint64_t)t0 + (uint64_t)0ULL));
    { float f = f23; uint32_t bits; std::memcpy(&bits, &f, sizeof(bits)); A1_W32B((uint32_t)sp, 200, bits); }
    f23 = ((float)(f14));
    { float f = f22; uint32_t bits; std::memcpy(&bits, &f, sizeof(bits)); A1_W32B((uint32_t)sp, 192, bits); }
    { f22 = ((float)(f12)); A3_CHARGE(); { const a1::RetVF a2_r = GAMEUTIL_is_nearly_equal_zero(ram, rt, f12, f13); v0 = a2_r.v0; f0 = a2_r.f0; } }
    f12 = ((float)(f21));
    f13 = ((float)(f20));
    { (s2 = (uint64_t)(((uint64_t)v0 < (uint64_t)(int64_t)(int32_t)1) ? 1 : 0)); A3_CHARGE(); { const a1::RetVF a2_r = GAMEUTIL_is_nearly_equal_zero(ram, rt, f12, f13); v0 = a2_r.v0; f0 = a2_r.f0; } }
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 1))));
    { const bool c = (int64_t)s2 == (int64_t)v1; (s1 = (uint64_t)(((uint64_t)v0 < (uint64_t)(int64_t)(int32_t)1) ? 1 : 0)); if (c) goto L_1419d3c; }
    if ((int64_t)s1 == (int64_t)v1) { (s0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)sp, 96)))); goto L_1419d40; }
    if ((int64_t)s3 != 0) { do { const a4::q128 _v = (A4_R128(ADD32((uint32_t)s0, 0))); h3 = a4::hi64(_v); std::memcpy(&v1, &_v, 8); } while (0); goto L_1419e38; }
L_1419d3c:;
    (s0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)sp, 96))));
L_1419d40:;
    (a1 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL));
    (a0 = (uint64_t)((uint64_t)s0 + (uint64_t)0ULL));
    { (a2 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 16)))); A3_CHARGE(); v0 = a6x::memset(ram, rt, a0, (uint32_t)a1, (uint32_t)a2, h2, h4); }
    (at = (uint32_t)((int32_t)((int32_t)((uint32_t)16256 << 16))));
    { uint32_t bits = a7_opq(at); std::memcpy(&f0, &bits, sizeof(bits)); }
    { const bool c = (int64_t)s1 == 0; { float f = f0; uint32_t bits; std::memcpy(&bits, &f, sizeof(bits)); A1_W32B((uint32_t)sp, 104, bits); } if (c) goto L_1419d7c; }
    (a0 = (uint64_t)((uint64_t)sp + (uint64_t)0ULL));
    { f12 = ((float)(f21)); A3_CHARGE(); { const a1::RetVF a2_r = iosRotMatrixY(ram, rt, a0, (uint32_t)sp, f12, f13, f14, f15, f16, f17, f18, f19, h6, h7, h8, h9); v0 = a2_r.v0; f0 = a2_r.f0; } }
    (a0 = (uint64_t)((uint64_t)s0 + (uint64_t)0ULL));
    (a1 = (uint64_t)((uint64_t)sp + (uint64_t)0ULL));
    { (a2 = (uint64_t)((uint64_t)s0 + (uint64_t)0ULL)); A3_CHARGE(); iosApplyRotMatrix(ram, rt, (uint32_t)a0, (uint32_t)a1, (uint32_t)a2, vf0, vf8, vf10, vf14, vf15, vf16, vacc); }
L_1419d7c:;
    { const bool c = (int64_t)s2 == 0; { const uint32_t _dynInsn = A1_R32(0x1419D80u); const int32_t _dynSimm = (int16_t)(_dynInsn & 0xFFFFu); const uint32_t _dynImm = _dynInsn & 0xFFFFu; (void)_dynSimm; (void)_dynImm; (s1 = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)_dynImm << 16)))); } if (c) goto L_1419dd0; }
    (a1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)sp, 64))));
    { const uint32_t _dynInsn = A1_R32(0x1419D88u); const int32_t _dynSimm = (int16_t)(_dynInsn & 0xFFFFu); const uint32_t _dynImm = _dynInsn & 0xFFFFu; (void)_dynSimm; (void)_dynImm; (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)s1, _dynSimm)))); }
    vf5 = a4v::castsi128_ps(A4_R128(ADD32((uint32_t)v0, 0)));
    vf6 = a4v::castsi128_ps(A4_R128(ADD32((uint32_t)s0, 0)));
    { a4::vec4 fs_yzx = a4v::shuffle_ps<0xC9>(vf5, vf5); a4::vec4 ft_zxy = a4v::shuffle_ps<0xD2>(vf6, vf6); a4::vec4 res = a4v::mul(fs_yzx, ft_zxy); vacc = a4v::dest<1, 1, 1, 0>(vacc, res); }
    { a4::vec4 fs_yzx = a4v::shuffle_ps<0xC9>(vf6, vf6); a4::vec4 ft_zxy = a4v::shuffle_ps<0xD2>(vf5, vf5); a4::vec4 mul_res = a4v::mul(fs_yzx, ft_zxy); a4::vec4 res = a4v::subK<1, 1>(vacc, mul_res); a4::q128 mask = a4v::set_epi32(0, -1, -1, -1); vf7 = a4v::blendv_ps(vf7, res, a4v::castsi128_ps(mask)); }
    { a4::vec4 res = a4v::sub(vf7, vf7); a4::q128 mask = a4v::set_epi32(-1, 0, 0, 0); vf7 = a4v::blend(vf7, res, a4v::castsi128_ps(mask)); }
    A4_W128(ADD32((uint32_t)a1, 0), a4v::castps_si128(vf7));
    (a0 = (uint64_t)((uint64_t)sp + (uint64_t)0ULL));
    f12 = ((float)(f22));
    { (a2 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL)); A3_CHARGE(); f0 = iosRotMatrixAxis(ram, rt, a0, a1, a2, (uint32_t)sp, f12, f13, f14, f15, f16, f17, f18, f19, vf0, vf5, vf7, vf10, vf11, vf12, vf13, vf14, vf15, vf16, vf17, vacc, vq, vstat); }
    (a0 = (uint64_t)((uint64_t)s0 + (uint64_t)0ULL));
    (a1 = (uint64_t)((uint64_t)sp + (uint64_t)0ULL));
    { (a2 = (uint64_t)((uint64_t)s0 + (uint64_t)0ULL)); A3_CHARGE(); iosApplyRotMatrix(ram, rt, (uint32_t)a0, (uint32_t)a1, (uint32_t)a2, vf0, vf8, vf10, vf14, vf15, vf16, vacc); }
    { const uint32_t _dynInsn = A1_R32(0x1419DC8u); const int32_t _dynSimm = (int16_t)(_dynInsn & 0xFFFFu); const uint32_t _dynImm = _dynInsn & 0xFFFFu; (void)_dynSimm; (void)_dynImm; (a1 = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)_dynImm << 16)))); }
    goto L_1419dd4;
L_1419dd0:;
    { const uint32_t _dynInsn = A1_R32(0x1419DD0u); const int32_t _dynSimm = (int16_t)(_dynInsn & 0xFFFFu); const uint32_t _dynImm = _dynInsn & 0xFFFFu; (void)_dynSimm; (void)_dynImm; (a1 = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)_dynImm << 16)))); }
L_1419dd4:;
    (a0 = (uint64_t)((uint64_t)sp + (uint64_t)0ULL));
    { const uint32_t _dynInsn = A1_R32(0x1419DD8u); const int32_t _dynSimm = (int16_t)(_dynInsn & 0xFFFFu); const uint32_t _dynImm = _dynInsn & 0xFFFFu; (void)_dynSimm; (void)_dynImm; (a3 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)s1, _dynSimm)))); } a3xw6 = true;
    (a2 = (uint64_t)((uint64_t)s6 + (uint64_t)0ULL));
    { { const uint32_t _dynInsn = A1_R32(0x1419DE4u); const int32_t _dynSimm = (int16_t)(_dynInsn & 0xFFFFu); const uint32_t _dynImm = _dynInsn & 0xFFFFu; (void)_dynSimm; (void)_dynImm; (a1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)a1, _dynSimm)))); } A3_CHARGE(); v0 = create_original_view_matrix(ram, rt, a0, a1, a2, a3, (uint32_t)sp, vf0, vf1, vf2, vf3, vf4, vf5, vf6, vf7, vf8, vf9, vf14, vf15, vf16, vf17, vacc, vq, vstat, h2, h8, h9, h10, h11, h12, h13, h14, h15); }
    (a0 = (uint64_t)((uint64_t)sp + (uint64_t)0ULL));
    { (a1 = (uint64_t)((uint64_t)sp + (uint64_t)0ULL)); A3_CHARGE(); uint64_t a7c[4] = {0u, 0u, 0u, 0u}; iosTransposeMatrix(ram, rt, a0, a1, h8, h9, h10, h11, h12, h13, h14, h15, a7c); t0 = a7c[0]; t1 = a7c[1]; t2 = a7c[2]; t3 = a7c[3]; a3xw7 = true; a3xw8 = true; a3xw9 = true; a3xw10 = true; }
    (a0 = (uint64_t)((uint64_t)s0 + (uint64_t)0ULL));
    (a1 = (uint64_t)((uint64_t)sp + (uint64_t)0ULL));
    { (a2 = (uint64_t)((uint64_t)s0 + (uint64_t)0ULL)); A3_CHARGE(); iosApplyRotMatrix(ram, rt, (uint32_t)a0, (uint32_t)a1, (uint32_t)a2, vf0, vf8, vf10, vf14, vf15, vf16, vacc); }
    { uint32_t bits; std::memcpy(&bits, &f23, sizeof(bits)); (v1 = (uint64_t)((int64_t)(int32_t)(bits))); }
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)sp, 80))));
    vf5 = a4v::castsi128_ps(A4_R128(ADD32((uint32_t)s0, 0)));
    vf6 = a4v::castsi128_ps(a4::mk128(h3, v1));
    { a4::vec4 res = a4v::mul(vf5, a4v::bcast<0>(vf6)); a4::q128 mask = a4v::set_epi32(0, -1, -1, -1); vf5 = a4v::blendv_ps(vf5, res, a4v::castsi128_ps(mask)); }
    A4_W128(ADD32((uint32_t)v0, 0), a4v::castps_si128(vf5));
    vf5 = a4v::castsi128_ps(A4_R128(ADD32((uint32_t)s5, 0)));
    vf6 = a4v::castsi128_ps(A4_R128(ADD32((uint32_t)v0, 0)));
    { a4::vec4 res = a4v::add(vf5, vf6); a4::q128 mask = a4v::set_epi32(0, -1, -1, -1); vf5 = a4v::blend(vf5, res, a4v::castsi128_ps(mask)); }
    A4_W128(ADD32((uint32_t)s4, 0), a4v::castps_si128(vf5));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 1))));
    goto L_1419e40;
L_1419e38:;
    (v0 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL));
    A4_W128(ADD32((uint32_t)s4, 0), a4::mk128(h3, v1));
L_1419e40:;
    { uint32_t bits = A1_R32B((uint32_t)sp, 200); float f; std::memcpy(&f, &bits, sizeof(f)); f23 = f; }
    { uint32_t bits = A1_R32B((uint32_t)sp, 192); float f; std::memcpy(&f, &bits, sizeof(f)); f22 = f; }
    { uint32_t bits = A1_R32B((uint32_t)sp, 184); float f; std::memcpy(&f, &bits, sizeof(f)); f21 = f; }
    { uint32_t bits = A1_R32B((uint32_t)sp, 176); float f; std::memcpy(&f, &bits, sizeof(f)); f20 = f; }
    { (sp = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)sp, 208)))); if (xo) { xo[0] = a1_bits(f12); xo[1] = a1_bits(f13); xo[2] = a1_bits(f20); xo[3] = a1_bits(f21); xo[4] = a1_bits(f22); xo[5] = a1_bits(f23); if (a3xw6) xo[6] = a3; if (a3xw7) xo[7] = t0; if (a3xw8) xo[8] = t1; if (a3xw9) xo[9] = t2; if (a3xw10) xo[10] = t3; } io_vf1 = vf1; io_vf2 = vf2; io_vf3 = vf3; io_vf4 = vf4; io_vf5 = vf5; io_vf6 = vf6; io_vf7 = vf7; io_vf8 = vf8; io_vf9 = vf9; io_vf10 = vf10; io_vf11 = vf11; io_vf12 = vf12; io_vf13 = vf13; io_vf14 = vf14; io_vf15 = vf15; io_vf16 = vf16; io_vf17 = vf17; io_vacc = vacc; io_vq = vq; io_vstat = vstat; io_h2 = h2; io_h3 = h3; io_h6 = h6; io_h7 = h7; io_h8 = h8; io_h9 = h9; io_h10 = h10; io_h11 = h11; io_h12 = h12; io_h13 = h13; io_h14 = h14; io_h15 = h15; return a1::RetVF{(uint64_t)v0, f0}; }
    __builtin_unreachable();
}

// gamecore_sub_01420930 (0x1420930): typed by tools/typed_lift2.py (round A6; guest frame calls; VU0 in vf0 vf2 vf5 vf6 vf7 vf9 vf10 vf14 vf15 vf16 vf17 vacc vstat, out vf1 vf2 vf3 vf4 vf5 vf6 vf7 vf8 vf9 vf10 vf14 vf15 vf16 vf17 vacc vq vstat h2 h8 h9 h10 h11 h12 h13 h14 h15, relocated calls 5)
inline a1::RetVF gamecore_sub_01420930(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint64_t a0, uint64_t a3, uint64_t sp, float f0, float f1, float f2, float f3, float f4, float f5, float f6, float f7, float f8, float f9, float f10, float f11, float f12, float f13, float f14, float f15, float f16, float f17, float f18, float f19, float f20, float f21, float f22, float f23, float f24, float f25, float f26, float f27, float f28, float f29, float f30, float f31, const a4::vec4 &io_vf0, a4::vec4 &io_vf1, a4::vec4 &io_vf2, a4::vec4 &io_vf3, a4::vec4 &io_vf4, a4::vec4 &io_vf5, a4::vec4 &io_vf6, a4::vec4 &io_vf7, a4::vec4 &io_vf8, a4::vec4 &io_vf9, a4::vec4 &io_vf10, a4::vec4 &io_vf14, a4::vec4 &io_vf15, a4::vec4 &io_vf16, a4::vec4 &io_vf17, a4::vec4 &io_vacc, float &io_vq, uint16_t &io_vstat, uint64_t &io_h2, uint64_t &io_h8, uint64_t &io_h9, uint64_t &io_h10, uint64_t &io_h11, uint64_t &io_h12, uint64_t &io_h13, uint64_t &io_h14, uint64_t &io_h15)
{
    uint32_t at = 0;
    uint64_t v0 = 0;
    uint32_t v1 = 0;
    uint64_t a1 = 0;
    uint64_t a2 = 0;
    uint64_t s0 = 0;
    uint64_t s1 = 0;
    uint32_t s2 = 0;
    uint32_t fcr31 = 0;
    a4::vec4 vf0 = io_vf0;
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
    a4::vec4 vf14 = io_vf14;
    a4::vec4 vf15 = io_vf15;
    a4::vec4 vf16 = io_vf16;
    a4::vec4 vf17 = io_vf17;
    a4::vec4 vacc = io_vacc;
    float vq = io_vq;
    uint16_t vstat = io_vstat;
    uint64_t h2 = io_h2;
    uint64_t h8 = io_h8;
    uint64_t h9 = io_h9;
    uint64_t h10 = io_h10;
    uint64_t h11 = io_h11;
    uint64_t h12 = io_h12;
    uint64_t h13 = io_h13;
    uint64_t h14 = io_h14;
    uint64_t h15 = io_h15;
    (sp = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)sp, 4294967136))));
    (s1 = (uint64_t)((uint64_t)a0 + (uint64_t)0ULL));
    { float f = f20; uint32_t bits; std::memcpy(&bits, &f, sizeof(bits)); A1_W32B((uint32_t)sp, 144, bits); }
    { uint32_t bits = 0u; std::memcpy(&f20, &bits, sizeof(bits)); }
    (a1 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)s1, 0))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)a1, 360))));
    { const bool c = (int64_t)v0 == 0; (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)s1, 572)))); if (c) goto L_1420aa8; }
    { A3_CHARGE(); v0 = a1_sx64(CAM_get_target_obj(ram, rt, (uint32_t)a0)); }
    v1 = 1u;
    (a1 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)s1, 0))));
    (a2 = (uint64_t)((uint64_t)v0 + (uint64_t)0ULL));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)a1, 364))));
    { const bool c = (int64_t)v0 != (int64_t)(int32_t)v1; (a0 = (uint64_t)((uint64_t)a2 + (uint64_t)0ULL)); if (c) goto L_142098c; }
    { A3_CHARGE(); v0 = GAMEUTIL_get_parent_object_u11(ram, rt, v0, a0, a1, f0, f1, f2, f3, f4, f5, f6, f7, f8, f9, f10, f11, f12, f13, f14, f15, f16, f17, f18, f19, f20, f21, f22, f23, f24, f25, f26, f27, f28, f29, f30, f31, h2); }
    (a2 = (uint64_t)((uint64_t)v0 + (uint64_t)0ULL));
L_142098c:;
    if ((int64_t)a2 == 0) { f20 = (-(float)(f20)); goto L_1420aa4; }
    { (a0 = (uint64_t)((uint64_t)a2 + (uint64_t)0ULL)); A3_CHARGE(); v0 = GetLayoutClass_u11(ram, rt, a0, f0, f1, f2, f3, f4, f5, f6, f7, f8, f9, f10, f11, f12, f13, f14, f15, f16, f17, f18, f19, f20, f21, f22, f23, f24, f25, f26, f27, f28, f29, f30, f31, h2); }
    { const bool c = (int64_t)v0 == 0; (v1 = (uint32_t)((int32_t)((int32_t)ADD32((uint32_t)sp, 32)))); if (c) goto L_1420aa0; }
    do { const a4::q128 _v = (A4_R128(ADD32((uint32_t)v0, 128))); h2 = a4::hi64(_v); std::memcpy(&v0, &_v, 8); } while (0);
    A4_W128(ADD32((uint32_t)sp, 32), a4::mk128(h2, v0));
    vf4 = a4v::castsi128_ps(A4_R128(ADD32(v1, 0)));
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
    A4_W128(ADD32((uint32_t)sp, 0), a4v::castps_si128(vf6));
    { const uint32_t _dynInsn = A1_R32(0x14209E4u); const int32_t _dynSimm = (int16_t)(_dynInsn & 0xFFFFu); const uint32_t _dynImm = _dynInsn & 0xFFFFu; (void)_dynSimm; (void)_dynImm; (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)_dynImm << 16)))); }
    (s0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)sp, 48))));
    { const uint32_t _dynInsn = A1_R32(0x14209ECu); const int32_t _dynSimm = (int16_t)(_dynInsn & 0xFFFFu); const uint32_t _dynImm = _dynInsn & 0xFFFFu; (void)_dynSimm; (void)_dynImm; (s2 = (uint32_t)((int32_t)((int32_t)ADD32((uint32_t)v0, _dynSimm)))); }
    (a0 = (uint64_t)((uint64_t)RC_U64X(s0) + (uint64_t)0ULL));
    (a1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)s1, 224))));
    (a2 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)s1, 256))));
    { (a3 = (uint64_t)((uint64_t)a1_sx64(s2) + (uint64_t)0ULL)); A3_CHARGE(); v0 = create_original_view_matrix(ram, rt, a0, a1, a2, a3, (uint32_t)sp, vf0, vf1, vf2, vf3, vf4, vf5, vf6, vf7, vf8, vf9, vf14, vf15, vf16, vf17, vacc, vq, vstat, h2, h8, h9, h10, h11, h12, h13, h14, h15); }
    (a1 = (uint64_t)((uint64_t)s0 + (uint64_t)0ULL));
    (s0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)sp, 16))));
    (a2 = (uint64_t)((uint64_t)sp + (uint64_t)0ULL));
    { (a0 = (uint64_t)((uint64_t)RC_U64X(s0) + (uint64_t)0ULL)); A3_CHARGE(); iosApplyRotMatrix(ram, rt, (uint32_t)a0, (uint32_t)a1, (uint32_t)a2, vf0, vf8, vf10, vf14, vf15, vf16, vacc); }
    (at = (uint32_t)((int32_t)((int32_t)((uint32_t)16255 << 16))));
    at = at | 0xBE76u;
    { uint32_t bits = a7_opq(at); std::memcpy(&f1, &bits, sizeof(bits)); }
    { uint32_t bits = A1_R32B((uint32_t)sp, 24); float f; std::memcpy(&f, &bits, sizeof(f)); f0 = f; }
    fcr31 = (a1f::lt(f0, f1)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
    if ((fcr31 & 0x800000u) == 0u) { f20 = (-(float)(f20)); goto L_1420aa4; }
    { float f = f20; uint32_t bits; std::memcpy(&bits, &f, sizeof(bits)); A1_W32B((uint32_t)sp, 24, bits); }
    vf5 = a4v::castsi128_ps(A4_R128(ADD32((uint32_t)s0, 0)));
    { a4::vec4 res = a4v::mul(vf5, vf5); a4::q128 mask = a4v::set_epi32(0, -1, -1, -1); vf7 = a4v::blend(vf7, res, a4v::castsi128_ps(mask)); }
    { a4::vec4 res = a4v::mulK<1, 1>(vf0, a4v::bcast<0>(vf7)); vacc = a4v::dest<0, 0, 0, 1>(vacc, res); }
    { a4::vec4 mul_res = a4v::mulK<1, 1>(vf0, a4v::bcast<1>(vf7)); a4::vec4 res = a4v::addK<1, 1>(vacc, mul_res); vacc = a4v::dest<0, 0, 0, 1>(vacc, res); }
    { a4::vec4 mul_res = a4v::mulK<1, 1>(vf0, a4v::bcast<2>(vf7)); a4::vec4 res = a4v::addK<1, 1>(vacc, mul_res); a4::q128 mask = a4v::set_epi32(-1, 0, 0, 0); vf7 = a4v::blendv_ps(vf7, res, a4v::castsi128_ps(mask)); }
    { float fs = a4v::cvtss_f32(a4v::shuffle_ps<0x03>(vf0, vf0)); float ft = a4v::cvtss_f32(a4v::shuffle_ps<0x03>(vf7, vf7)); vq = a4v::rsqrt(vstat, fs, ft); }
    { a4::vec4 res = a4v::mul(vf5, a4v::set1_ps(vq)); a4::q128 mask = a4v::set_epi32(0, -1, -1, -1); vf5 = a4v::blendv_ps(vf5, res, a4v::castsi128_ps(mask)); }
    A4_W128(ADD32((uint32_t)s0, 0), a4v::castps_si128(vf5));
    vf5 = a4v::castsi128_ps(A4_R128(ADD32((uint32_t)s0, 0)));
    vf6 = a4v::castsi128_ps(A4_R128(ADD32(s2, 0)));
    { a4::vec4 res = a4v::addK<1, 1>(vf0, a4v::bcast<3>(vf0)); a4::q128 mask = a4v::set_epi32(0, 0, 0, -1); vf7 = a4v::blendv_ps(vf7, res, a4v::castsi128_ps(mask)); }
    { a4::vec4 res = a4v::mul(vf5, vf6); a4::q128 mask = a4v::set_epi32(0, -1, -1, -1); vf6 = a4v::blend(vf6, res, a4v::castsi128_ps(mask)); }
    { a4::vec4 res = a4v::addK<1, 1>(vf0, a4v::bcast<0>(vf6)); vacc = a4v::dest<1, 0, 0, 0>(vacc, res); }
    { a4::vec4 mul_res = a4v::mulK<1, 1>(vf7, a4v::bcast<1>(vf6)); a4::vec4 res = a4v::addK<1, 1>(vacc, mul_res); vacc = a4v::dest<1, 0, 0, 0>(vacc, res); }
    { a4::vec4 mul_res = a4v::mulK<1, 1>(vf7, a4v::bcast<2>(vf6)); a4::vec4 res = a4v::addK<1, 1>(vacc, mul_res); a4::q128 mask = a4v::set_epi32(0, 0, 0, -1); vf6 = a4v::blendv_ps(vf6, res, a4v::castsi128_ps(mask)); }
    do { const a4::q128 _v = (a4v::castps_si128(vf6)); h2 = a4::hi64(_v); std::memcpy(&v0, &_v, 8); } while (0);
    { uint32_t bits = (uint32_t)v0; std::memcpy(&f12, &bits, sizeof(bits)); }
    { A3_CHARGE(); f0 = a9x::acosExact(ram, rt, f12); }
    { uint32_t bits = 0u; std::memcpy(&f1, &bits, sizeof(bits)); }
    f20 = ((float)(f0));
    { uint32_t bits = A1_R32B((uint32_t)sp, 16); float f; std::memcpy(&f, &bits, sizeof(f)); f0 = f; }
    fcr31 = (a1f::lt(f0, f1)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
    if ((fcr31 & 0x800000u) != 0u) { f20 = (-(float)(f20)); goto L_1420aa0; }
L_1420aa0:;
    f20 = (-(float)(f20));
L_1420aa4:;
    (a1 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)s1, 0))));
L_1420aa8:;
    { uint32_t bits = A1_R32B((uint32_t)a1, 368); float f; std::memcpy(&f, &bits, sizeof(f)); f0 = f; }
    f0 = a1f::mul(f0, f20);
    { uint32_t bits = A1_R32B((uint32_t)sp, 144); float f; std::memcpy(&f, &bits, sizeof(f)); f20 = f; }
    { (sp = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)sp, 160)))); io_vf1 = vf1; io_vf2 = vf2; io_vf3 = vf3; io_vf4 = vf4; io_vf5 = vf5; io_vf6 = vf6; io_vf7 = vf7; io_vf8 = vf8; io_vf9 = vf9; io_vf10 = vf10; io_vf14 = vf14; io_vf15 = vf15; io_vf16 = vf16; io_vf17 = vf17; io_vacc = vacc; io_vq = vq; io_vstat = vstat; io_h2 = h2; io_h8 = h8; io_h9 = h9; io_h10 = h10; io_h11 = h11; io_h12 = h12; io_h13 = h13; io_h14 = h14; io_h15 = h15; return a1::RetVF{(uint64_t)v0, f0}; }
    __builtin_unreachable();
}

// SCRFuncInputRotateMotionKey (0x1450c00): typed by tools/typed_lift2.py (round A6; guest frame calls; VU0 in vf0 vf7 vf10 vf12 vf13 vf14 vf15 vf16 vf17 vf25 vf28 vf29 vacc vstat h5, out vf4 vf5 vf7 vf8 vf10 vf11 vf12 vf13 vf14 vf15 vf16 vf17 vf24 vf25 vf26 vf28 vf29 vacc vq vstat h2 h8 h9 h10 h11 h12 h13 h14 h15, relocated calls 5)
inline a1::RetVF SCRFuncInputRotateMotionKey(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint64_t v0, uint64_t a0, uint64_t a1, uint64_t sp, float f0, float f1, float f2, float f3, float f4, float f5, float f6, float f7, float f8, float f9, float f10, float f11, float f12, float f13, float f14, float f15, float f16, float f17, float f18, float f19, float f20, float f21, float f22, float f23, float f24, float f25, float f26, float f27, float f28, float f29, float f30, float f31, const a4::vec4 &io_vf0, a4::vec4 &io_vf4, a4::vec4 &io_vf5, a4::vec4 &io_vf7, a4::vec4 &io_vf8, a4::vec4 &io_vf10, a4::vec4 &io_vf11, a4::vec4 &io_vf12, a4::vec4 &io_vf13, a4::vec4 &io_vf14, a4::vec4 &io_vf15, a4::vec4 &io_vf16, a4::vec4 &io_vf17, a4::vec4 &io_vf24, a4::vec4 &io_vf25, a4::vec4 &io_vf26, a4::vec4 &io_vf28, a4::vec4 &io_vf29, a4::vec4 &io_vacc, float &io_vq, uint16_t &io_vstat, uint64_t &io_h2, uint64_t io_h5, uint64_t &io_h8, uint64_t &io_h9, uint64_t &io_h10, uint64_t &io_h11, uint64_t &io_h12, uint64_t &io_h13, uint64_t &io_h14, uint64_t &io_h15)
{
    uint64_t at = 0;
    uint64_t v1 = 0;
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
    uint32_t fcr31 = 0;
    a4::vec4 vf0 = io_vf0;
    a4::vec4 vf4 = io_vf4;
    a4::vec4 vf5 = io_vf5;
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
    a4::vec4 vf24 = io_vf24;
    a4::vec4 vf25 = io_vf25;
    a4::vec4 vf26 = io_vf26;
    a4::vec4 vf28 = io_vf28;
    a4::vec4 vf29 = io_vf29;
    a4::vec4 vacc = io_vacc;
    float vq = io_vq;
    uint16_t vstat = io_vstat;
    uint64_t h2 = io_h2;
    uint64_t h5 = io_h5;
    uint64_t h8 = io_h8;
    uint64_t h9 = io_h9;
    uint64_t h10 = io_h10;
    uint64_t h11 = io_h11;
    uint64_t h12 = io_h12;
    uint64_t h13 = io_h13;
    uint64_t h14 = io_h14;
    uint64_t h15 = io_h15;
    (sp = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)sp, 4294967168))));
    (s2 = (uint64_t)((uint64_t)a0 + (uint64_t)0ULL));
    { float f = f20; uint32_t bits; std::memcpy(&bits, &f, sizeof(bits)); A1_W32B((uint32_t)sp, 112, bits); }
    { f20 = ((float)(f12)); A3_CHARGE(); v0 = getSCRDataCtrlWork(ram, rt, v0, a0, f12, f13, f14, f15, f16, f17, f18, f19, h2); }
    (a0 = (uint64_t)((uint64_t)s2 + (uint64_t)0ULL));
    { (s1 = (uint64_t)((uint64_t)v0 + (uint64_t)0ULL)); A3_CHARGE(); v0 = GetLayoutClass_u11(ram, rt, a0, f0, f1, f2, f3, f4, f5, f6, f7, f8, f9, f10, f11, f12, f13, f14, f15, f16, f17, f18, f19, f20, f21, f22, f23, f24, f25, f26, f27, f28, f29, f30, f31, h2); }
    (a0 = (uint64_t)((uint64_t)s2 + (uint64_t)0ULL));
    { (s0 = (uint64_t)((uint64_t)v0 + (uint64_t)0ULL)); A3_CHARGE(); v0 = GetCharacterClass(ram, rt, a0, h2, h5); }
    (v1 = (uint64_t)((uint64_t)sp + (uint64_t)0ULL));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v0, 1136))));
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)s0, 308))));
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
    A4_W128(ADD32((uint32_t)v1, 0), a4v::castps_si128(vf14));
    A4_W128(ADD32((uint32_t)v1, 16), a4v::castps_si128(vf15));
    A4_W128(ADD32((uint32_t)v1, 32), a4v::castps_si128(vf16));
    vf4 = a4v::castsi128_ps(A4_R128(ADD32((uint32_t)a0, 0)));
    { a4::q128 mask = a4v::set_epi32(-1, 0, 0, 0); vf4 = a4v::blendv_ps(vf4, vf0, a4v::castsi128_ps(mask)); }
    A4_W128(ADD32((uint32_t)v1, 48), a4v::castps_si128(vf4));
    vf24 = a4v::castsi128_ps(A4_R128(ADD32((uint32_t)sp, 48)));
    { a4::vec4 res = a4v::subK<1, 1>(vf0, a4v::bcast<3>(vf0)); a4::q128 mask = a4v::set_epi32(0, 0, 0, -1); vf25 = a4v::blendv_ps(vf25, res, a4v::castsi128_ps(mask)); }
    { a4::vec4 res = a4v::mulK<0, 1>(vf24, a4v::bcast<0>(vf25)); a4::q128 mask = a4v::set_epi32(0, -1, -1, -1); vf28 = a4v::blendv_ps(vf28, res, a4v::castsi128_ps(mask)); }
    { a4::vec4 res = a4v::sub(vf28, vf28); a4::q128 mask = a4v::set_epi32(-1, 0, 0, 0); vf28 = a4v::blend(vf28, res, a4v::castsi128_ps(mask)); }
    do { const a4::q128 _v = (A4_R128(ADD32((uint32_t)sp, 0))); h8 = a4::hi64(_v); std::memcpy(&t0, &_v, 8); } while (0);
    do { const a4::q128 _v = (A4_R128(ADD32((uint32_t)sp, 16))); h9 = a4::hi64(_v); std::memcpy(&t1, &_v, 8); } while (0);
    do { const a4::q128 _v = (A4_R128(ADD32((uint32_t)sp, 32))); h10 = a4::hi64(_v); std::memcpy(&t2, &_v, 8); } while (0);
    do { const a4::q128 _v = (a4v::castps_si128(vf0)); h11 = a4::hi64(_v); std::memcpy(&t3, &_v, 8); } while (0);
    do { const a4::q128 _v = (a4m::pextlw(a4::mk128(h9, t1), a4::mk128(h8, t0))); h12 = a4::hi64(_v); std::memcpy(&t4, &_v, 8); } while (0);
    do { const a4::q128 _v = (a4m::pextuw(a4::mk128(h9, t1), a4::mk128(h8, t0))); h13 = a4::hi64(_v); std::memcpy(&t5, &_v, 8); } while (0);
    do { const a4::q128 _v = (a4m::pextlw(a4::mk128(h11, t3), a4::mk128(h10, t2))); h14 = a4::hi64(_v); std::memcpy(&t6, &_v, 8); } while (0);
    do { const a4::q128 _v = (a4m::pextuw(a4::mk128(h11, t3), a4::mk128(h10, t2))); h15 = a4::hi64(_v); std::memcpy(&t7, &_v, 8); } while (0);
    do { const a4::q128 _v = (a4m::pcpyld(a4::mk128(h14, t6), a4::mk128(h12, t4))); h8 = a4::hi64(_v); std::memcpy(&t0, &_v, 8); } while (0);
    do { const a4::q128 _v = (a4v::unpackhi_epi64(a4::mk128(h12, t4), a4::mk128(h14, t6))); h9 = a4::hi64(_v); std::memcpy(&t1, &_v, 8); } while (0);
    do { const a4::q128 _v = (a4m::pcpyld(a4::mk128(h15, t7), a4::mk128(h13, t5))); h10 = a4::hi64(_v); std::memcpy(&t2, &_v, 8); } while (0);
    do { const a4::q128 _v = (a4v::unpackhi_epi64(a4::mk128(h13, t5), a4::mk128(h15, t7))); h11 = a4::hi64(_v); std::memcpy(&t3, &_v, 8); } while (0);
    vf24 = a4v::castsi128_ps(a4::mk128(h8, t0));
    vf25 = a4v::castsi128_ps(a4::mk128(h9, t1));
    vf26 = a4v::castsi128_ps(a4::mk128(h10, t2));
    { a4::vec4 res = a4v::mulK<0, 1>(vf24, a4v::bcast<0>(vf28)); vacc = a4v::dest<1, 1, 1, 1>(vacc, res); }
    { a4::vec4 mul_res = a4v::mulK<0, 1>(vf25, a4v::bcast<1>(vf28)); a4::vec4 res = a4v::addK<1, 1>(vacc, mul_res); vacc = a4v::dest<1, 1, 1, 1>(vacc, res); }
    { a4::vec4 mul_res = a4v::mulK<0, 1>(vf26, a4v::bcast<2>(vf28)); a4::vec4 res = a4v::addK<1, 1>(vacc, mul_res); vacc = a4v::dest<1, 1, 1, 1>(vacc, res); }
    { a4::vec4 mul_res = a4v::mulK<1, 1>(vf0, a4v::bcast<3>(vf28)); a4::vec4 res = a4v::addK<1, 1>(vacc, mul_res); a4::q128 mask = a4v::set_epi32(-1, -1, -1, -1); vf29 = a4v::blendv_ps(vf29, res, a4v::castsi128_ps(mask)); }
    { a4::vec4 res = a4v::subK<1, 1>(vf29, vf29); a4::q128 mask = a4v::set_epi32(-1, 0, 0, 0); vf29 = a4v::blend(vf29, res, a4v::castsi128_ps(mask)); }
    { a4::vec4 res = a4v::addK<1, 1>(vf29, vf0); a4::q128 mask = a4v::set_epi32(-1, 0, 0, 0); vf29 = a4v::blend(vf29, res, a4v::castsi128_ps(mask)); }
    A4_W128(ADD32((uint32_t)sp, 0), a4v::castps_si128(vf24));
    A4_W128(ADD32((uint32_t)sp, 16), a4v::castps_si128(vf25));
    A4_W128(ADD32((uint32_t)sp, 32), a4v::castps_si128(vf26));
    A4_W128(ADD32((uint32_t)sp, 48), a4v::castps_si128(vf29));
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)s1, 16508))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)sp, 64))));
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)a0, 308))));
    vf8 = a4v::castsi128_ps(A4_R128(ADD32((uint32_t)v1, 0)));
    vf14 = a4v::castsi128_ps(A4_R128(ADD32((uint32_t)sp, 0)));
    vf15 = a4v::castsi128_ps(A4_R128(ADD32((uint32_t)sp, 16)));
    vf16 = a4v::castsi128_ps(A4_R128(ADD32((uint32_t)sp, 32)));
    vf17 = a4v::castsi128_ps(A4_R128(ADD32((uint32_t)sp, 48)));
    { a4::vec4 res = a4v::mul(vf14, a4v::bcast<0>(vf8)); vacc = a4v::dest<1, 1, 1, 1>(vacc, res); }
    { a4::vec4 mul_res = a4v::mul(vf15, a4v::bcast<1>(vf8)); a4::vec4 res = a4v::addK<1, 1>(vacc, mul_res); vacc = a4v::dest<1, 1, 1, 1>(vacc, res); }
    { a4::vec4 mul_res = a4v::mul(vf16, a4v::bcast<2>(vf8)); a4::vec4 res = a4v::addK<1, 1>(vacc, mul_res); vacc = a4v::dest<1, 1, 1, 1>(vacc, res); }
    { a4::vec4 mul_res = a4v::mul(vf17, a4v::bcast<3>(vf8)); a4::vec4 res = a4v::addK<1, 1>(vacc, mul_res); a4::q128 mask = a4v::set_epi32(-1, -1, -1, -1); vf10 = a4v::blendv_ps(vf10, res, a4v::castsi128_ps(mask)); }
    A4_W128(ADD32((uint32_t)v0, 0), a4v::castps_si128(vf10));
    A1_W32B((uint32_t)sp, 68, 0u);
    vf5 = a4v::castsi128_ps(A4_R128(ADD32((uint32_t)v0, 0)));
    { a4::vec4 res = a4v::mul(vf5, vf5); a4::q128 mask = a4v::set_epi32(0, -1, -1, -1); vf7 = a4v::blend(vf7, res, a4v::castsi128_ps(mask)); }
    { a4::vec4 res = a4v::mulK<1, 1>(vf0, a4v::bcast<0>(vf7)); vacc = a4v::dest<0, 0, 0, 1>(vacc, res); }
    { a4::vec4 mul_res = a4v::mulK<1, 1>(vf0, a4v::bcast<1>(vf7)); a4::vec4 res = a4v::addK<1, 1>(vacc, mul_res); vacc = a4v::dest<0, 0, 0, 1>(vacc, res); }
    { a4::vec4 mul_res = a4v::mulK<1, 1>(vf0, a4v::bcast<2>(vf7)); a4::vec4 res = a4v::addK<1, 1>(vacc, mul_res); a4::q128 mask = a4v::set_epi32(-1, 0, 0, 0); vf7 = a4v::blendv_ps(vf7, res, a4v::castsi128_ps(mask)); }
    { float fs = a4v::cvtss_f32(a4v::shuffle_ps<0x03>(vf0, vf0)); float ft = a4v::cvtss_f32(a4v::shuffle_ps<0x03>(vf7, vf7)); vq = a4v::rsqrt(vstat, fs, ft); }
    { a4::vec4 res = a4v::mul(vf5, a4v::set1_ps(vq)); a4::q128 mask = a4v::set_epi32(0, -1, -1, -1); vf5 = a4v::blendv_ps(vf5, res, a4v::castsi128_ps(mask)); }
    A4_W128(ADD32((uint32_t)v0, 0), a4v::castps_si128(vf5));
    { uint32_t bits = A1_R32B((uint32_t)sp, 64); float f; std::memcpy(&f, &bits, sizeof(f)); f12 = f; }
    { { uint32_t bits = A1_R32B((uint32_t)sp, 72); float f; std::memcpy(&f, &bits, sizeof(f)); f13 = f; } A3_CHARGE(); f0 = a9x::atan2Exact(ram, rt, f12, f13); }
    f1 = ((float)(f0));
    (at = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)15502 << 16))));
    (at = (uint64_t)(RC_U64X(at) | (uint64_t)(uint16_t)64053));
    { uint32_t bits = (uint32_t)at; std::memcpy(&f0, &bits, sizeof(bits)); }
    f0 = a1f::mul(f20, f0);
    fcr31 = (a1f::lt(f0, f1)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
    { const bool c = (fcr31 & 0x800000u) == 0u; (a0 = (uint64_t)((uint64_t)s2 + (uint64_t)0ULL)); if (c) goto L_1450dd8; }
    { const uint32_t _dynInsn = A1_R32(0x1450DBCu); const int32_t _dynSimm = (int16_t)(_dynInsn & 0xFFFFu); const uint32_t _dynImm = _dynInsn & 0xFFFFu; (void)_dynSimm; (void)_dynImm; (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)_dynImm << 16)))); }
    { const uint32_t _dynInsn = A1_R32(0x1450DC0u); const int32_t _dynSimm = (int16_t)(_dynInsn & 0xFFFFu); const uint32_t _dynImm = _dynInsn & 0xFFFFu; (void)_dynSimm; (void)_dynImm; (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32(ADD32((uint32_t)v0, _dynSimm))))); }
    { (a1 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)v1, 4)))); A3_CHARGE(); v0 = CharaCtrlSetScriptCtrlStatus_u11(ram, rt, v0, a0, a1, sp, f0, f1, f2, f3, f4, f5, f6, f7, f8, f9, f10, f11, f12, f13, f14, f15, f16, f17, f18, f19, f20, f21, f22, f23, f24, f25, f26, f27, f28, f29, f30, f31, h2, h5); }
    goto L_1450e00;
L_1450dd8:;
    f0 = (-(float)(f0));
    fcr31 = (a1f::lt(f1, f0)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
    if ((fcr31 & 0x800000u) == 0u) goto L_1450e00;
    { const uint32_t _dynInsn = A1_R32(0x1450DE8u); const int32_t _dynSimm = (int16_t)(_dynInsn & 0xFFFFu); const uint32_t _dynImm = _dynInsn & 0xFFFFu; (void)_dynSimm; (void)_dynImm; (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)_dynImm << 16)))); }
    (a0 = (uint64_t)((uint64_t)s2 + (uint64_t)0ULL));
    { const uint32_t _dynInsn = A1_R32(0x1450DF0u); const int32_t _dynSimm = (int16_t)(_dynInsn & 0xFFFFu); const uint32_t _dynImm = _dynInsn & 0xFFFFu; (void)_dynSimm; (void)_dynImm; (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32(ADD32((uint32_t)v1, _dynSimm))))); }
    { (a1 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)v0, 4)))); A3_CHARGE(); v0 = CharaCtrlSetScriptCtrlStatus_u11(ram, rt, v0, a0, a1, sp, f0, f1, f2, f3, f4, f5, f6, f7, f8, f9, f10, f11, f12, f13, f14, f15, f16, f17, f18, f19, f20, f21, f22, f23, f24, f25, f26, f27, f28, f29, f30, f31, h2, h5); }
L_1450e00:;
    { uint32_t bits = A1_R32B((uint32_t)sp, 112); float f; std::memcpy(&f, &bits, sizeof(f)); f20 = f; }
    { (sp = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)sp, 128)))); io_vf4 = vf4; io_vf5 = vf5; io_vf7 = vf7; io_vf8 = vf8; io_vf10 = vf10; io_vf11 = vf11; io_vf12 = vf12; io_vf13 = vf13; io_vf14 = vf14; io_vf15 = vf15; io_vf16 = vf16; io_vf17 = vf17; io_vf24 = vf24; io_vf25 = vf25; io_vf26 = vf26; io_vf28 = vf28; io_vf29 = vf29; io_vacc = vacc; io_vq = vq; io_vstat = vstat; io_h2 = h2; io_h8 = h8; io_h9 = h9; io_h10 = h10; io_h11 = h11; io_h12 = h12; io_h13 = h13; io_h14 = h14; io_h15 = h15; return a1::RetVF{(uint64_t)v0, f0}; }
    __builtin_unreachable();
}

// SCRFuncInputRotateMotionKeyToNextPoint (0x1450e18): typed by tools/typed_lift2.py (round A6; guest frame calls; VU0 in vf0 vf7 vf10 vf12 vf13 vf14 vf15 vf16 vf17 vf25 vf28 vf29 vacc vstat h5, out vf4 vf5 vf7 vf8 vf10 vf11 vf12 vf13 vf14 vf15 vf16 vf17 vf24 vf25 vf26 vf28 vf29 vacc vq vstat h2 h8 h9 h10 h11 h12 h13 h14 h15, relocated calls 5)
inline uint64_t SCRFuncInputRotateMotionKeyToNextPoint(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint64_t v0, uint64_t a0, uint64_t sp, float f0, float f1, float f2, float f3, float f4, float f5, float f6, float f7, float f8, float f9, float f10, float f11, float f12, float f13, float f14, float f15, float f16, float f17, float f18, float f19, float f20, float f21, float f22, float f23, float f24, float f25, float f26, float f27, float f28, float f29, float f30, float f31, const a4::vec4 &io_vf0, a4::vec4 &io_vf4, a4::vec4 &io_vf5, a4::vec4 &io_vf7, a4::vec4 &io_vf8, a4::vec4 &io_vf10, a4::vec4 &io_vf11, a4::vec4 &io_vf12, a4::vec4 &io_vf13, a4::vec4 &io_vf14, a4::vec4 &io_vf15, a4::vec4 &io_vf16, a4::vec4 &io_vf17, a4::vec4 &io_vf24, a4::vec4 &io_vf25, a4::vec4 &io_vf26, a4::vec4 &io_vf28, a4::vec4 &io_vf29, a4::vec4 &io_vacc, float &io_vq, uint16_t &io_vstat, uint64_t &io_h2, uint64_t io_h5, uint64_t &io_h8, uint64_t &io_h9, uint64_t &io_h10, uint64_t &io_h11, uint64_t &io_h12, uint64_t &io_h13, uint64_t &io_h14, uint64_t &io_h15)
{
    uint64_t at = 0;
    uint64_t v1 = 0;
    uint64_t a1 = 0;
    uint64_t a2 = 0;
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
    uint32_t fcr31 = 0;
    a4::vec4 vf0 = io_vf0;
    a4::vec4 vf4 = io_vf4;
    a4::vec4 vf5 = io_vf5;
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
    a4::vec4 vf24 = io_vf24;
    a4::vec4 vf25 = io_vf25;
    a4::vec4 vf26 = io_vf26;
    a4::vec4 vf28 = io_vf28;
    a4::vec4 vf29 = io_vf29;
    a4::vec4 vacc = io_vacc;
    float vq = io_vq;
    uint16_t vstat = io_vstat;
    uint64_t h2 = io_h2;
    uint64_t h5 = io_h5;
    uint64_t h8 = io_h8;
    uint64_t h9 = io_h9;
    uint64_t h10 = io_h10;
    uint64_t h11 = io_h11;
    uint64_t h12 = io_h12;
    uint64_t h13 = io_h13;
    uint64_t h14 = io_h14;
    uint64_t h15 = io_h15;
    (sp = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)sp, 4294967168))));
    (s2 = (uint64_t)((uint64_t)a0 + (uint64_t)0ULL));
    { float f = f20; uint32_t bits; std::memcpy(&bits, &f, sizeof(bits)); A1_W32B((uint32_t)sp, 112, bits); }
    { f20 = ((float)(f12)); A3_CHARGE(); v0 = getSCRDataCtrlWork(ram, rt, v0, a0, f12, f13, f14, f15, f16, f17, f18, f19, h2); }
    (s0 = (uint64_t)((uint64_t)v0 + (uint64_t)0ULL));
    { (a0 = (uint64_t)((uint64_t)s2 + (uint64_t)0ULL)); A3_CHARGE(); v0 = GetLayoutClass_u11(ram, rt, a0, f0, f1, f2, f3, f4, f5, f6, f7, f8, f9, f10, f11, f12, f13, f14, f15, f16, f17, f18, f19, f20, f21, f22, f23, f24, f25, f26, f27, f28, f29, f30, f31, h2); }
    (a0 = (uint64_t)((uint64_t)s0 + (uint64_t)0ULL));
    (a1 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL));
    { (s1 = (uint64_t)((uint64_t)v0 + (uint64_t)0ULL)); A3_CHARGE(); v0 = getPassageWork(ram, rt, (uint32_t)a0, a1); }
    (a0 = (uint64_t)((uint64_t)s2 + (uint64_t)0ULL));
    (a2 = (uint64_t)((uint64_t)v0 + (uint64_t)0ULL));
    { const bool c = (int64_t)a2 == 0; (a1 = (uint64_t)((uint64_t)sp + (uint64_t)0ULL)); if (c) goto L_1451028; }
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)s1, 308))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)s1, 316))));
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
    A4_W128(ADD32((uint32_t)a1, 0), a4v::castps_si128(vf14));
    A4_W128(ADD32((uint32_t)a1, 16), a4v::castps_si128(vf15));
    A4_W128(ADD32((uint32_t)a1, 32), a4v::castps_si128(vf16));
    vf4 = a4v::castsi128_ps(A4_R128(ADD32((uint32_t)v1, 0)));
    { a4::q128 mask = a4v::set_epi32(-1, 0, 0, 0); vf4 = a4v::blendv_ps(vf4, vf0, a4v::castsi128_ps(mask)); }
    A4_W128(ADD32((uint32_t)a1, 48), a4v::castps_si128(vf4));
    vf24 = a4v::castsi128_ps(A4_R128(ADD32((uint32_t)sp, 48)));
    { a4::vec4 res = a4v::subK<1, 1>(vf0, a4v::bcast<3>(vf0)); a4::q128 mask = a4v::set_epi32(0, 0, 0, -1); vf25 = a4v::blendv_ps(vf25, res, a4v::castsi128_ps(mask)); }
    { a4::vec4 res = a4v::mulK<0, 1>(vf24, a4v::bcast<0>(vf25)); a4::q128 mask = a4v::set_epi32(0, -1, -1, -1); vf28 = a4v::blendv_ps(vf28, res, a4v::castsi128_ps(mask)); }
    { a4::vec4 res = a4v::sub(vf28, vf28); a4::q128 mask = a4v::set_epi32(-1, 0, 0, 0); vf28 = a4v::blend(vf28, res, a4v::castsi128_ps(mask)); }
    do { const a4::q128 _v = (A4_R128(ADD32((uint32_t)sp, 0))); h8 = a4::hi64(_v); std::memcpy(&t0, &_v, 8); } while (0);
    do { const a4::q128 _v = (A4_R128(ADD32((uint32_t)sp, 16))); h9 = a4::hi64(_v); std::memcpy(&t1, &_v, 8); } while (0);
    do { const a4::q128 _v = (A4_R128(ADD32((uint32_t)sp, 32))); h10 = a4::hi64(_v); std::memcpy(&t2, &_v, 8); } while (0);
    do { const a4::q128 _v = (a4v::castps_si128(vf0)); h11 = a4::hi64(_v); std::memcpy(&t3, &_v, 8); } while (0);
    do { const a4::q128 _v = (a4m::pextlw(a4::mk128(h9, t1), a4::mk128(h8, t0))); h12 = a4::hi64(_v); std::memcpy(&t4, &_v, 8); } while (0);
    do { const a4::q128 _v = (a4m::pextuw(a4::mk128(h9, t1), a4::mk128(h8, t0))); h13 = a4::hi64(_v); std::memcpy(&t5, &_v, 8); } while (0);
    do { const a4::q128 _v = (a4m::pextlw(a4::mk128(h11, t3), a4::mk128(h10, t2))); h14 = a4::hi64(_v); std::memcpy(&t6, &_v, 8); } while (0);
    do { const a4::q128 _v = (a4m::pextuw(a4::mk128(h11, t3), a4::mk128(h10, t2))); h15 = a4::hi64(_v); std::memcpy(&t7, &_v, 8); } while (0);
    do { const a4::q128 _v = (a4m::pcpyld(a4::mk128(h14, t6), a4::mk128(h12, t4))); h8 = a4::hi64(_v); std::memcpy(&t0, &_v, 8); } while (0);
    do { const a4::q128 _v = (a4v::unpackhi_epi64(a4::mk128(h12, t4), a4::mk128(h14, t6))); h9 = a4::hi64(_v); std::memcpy(&t1, &_v, 8); } while (0);
    do { const a4::q128 _v = (a4m::pcpyld(a4::mk128(h15, t7), a4::mk128(h13, t5))); h10 = a4::hi64(_v); std::memcpy(&t2, &_v, 8); } while (0);
    do { const a4::q128 _v = (a4v::unpackhi_epi64(a4::mk128(h13, t5), a4::mk128(h15, t7))); h11 = a4::hi64(_v); std::memcpy(&t3, &_v, 8); } while (0);
    vf24 = a4v::castsi128_ps(a4::mk128(h8, t0));
    vf25 = a4v::castsi128_ps(a4::mk128(h9, t1));
    vf26 = a4v::castsi128_ps(a4::mk128(h10, t2));
    { a4::vec4 res = a4v::mulK<0, 1>(vf24, a4v::bcast<0>(vf28)); vacc = a4v::dest<1, 1, 1, 1>(vacc, res); }
    { a4::vec4 mul_res = a4v::mulK<0, 1>(vf25, a4v::bcast<1>(vf28)); a4::vec4 res = a4v::addK<1, 1>(vacc, mul_res); vacc = a4v::dest<1, 1, 1, 1>(vacc, res); }
    { a4::vec4 mul_res = a4v::mulK<0, 1>(vf26, a4v::bcast<2>(vf28)); a4::vec4 res = a4v::addK<1, 1>(vacc, mul_res); vacc = a4v::dest<1, 1, 1, 1>(vacc, res); }
    { a4::vec4 mul_res = a4v::mulK<1, 1>(vf0, a4v::bcast<3>(vf28)); a4::vec4 res = a4v::addK<1, 1>(vacc, mul_res); a4::q128 mask = a4v::set_epi32(-1, -1, -1, -1); vf29 = a4v::blendv_ps(vf29, res, a4v::castsi128_ps(mask)); }
    { a4::vec4 res = a4v::subK<1, 1>(vf29, vf29); a4::q128 mask = a4v::set_epi32(-1, 0, 0, 0); vf29 = a4v::blend(vf29, res, a4v::castsi128_ps(mask)); }
    { a4::vec4 res = a4v::addK<1, 1>(vf29, vf0); a4::q128 mask = a4v::set_epi32(-1, 0, 0, 0); vf29 = a4v::blend(vf29, res, a4v::castsi128_ps(mask)); }
    A4_W128(ADD32((uint32_t)sp, 0), a4v::castps_si128(vf24));
    A4_W128(ADD32((uint32_t)sp, 16), a4v::castps_si128(vf25));
    A4_W128(ADD32((uint32_t)sp, 32), a4v::castps_si128(vf26));
    A4_W128(ADD32((uint32_t)sp, 48), a4v::castps_si128(vf29));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)sp, 64))));
    vf8 = a4v::castsi128_ps(A4_R128(ADD32((uint32_t)a2, 0)));
    vf14 = a4v::castsi128_ps(A4_R128(ADD32((uint32_t)sp, 0)));
    vf15 = a4v::castsi128_ps(A4_R128(ADD32((uint32_t)sp, 16)));
    vf16 = a4v::castsi128_ps(A4_R128(ADD32((uint32_t)sp, 32)));
    vf17 = a4v::castsi128_ps(A4_R128(ADD32((uint32_t)sp, 48)));
    { a4::vec4 res = a4v::mul(vf14, a4v::bcast<0>(vf8)); vacc = a4v::dest<1, 1, 1, 1>(vacc, res); }
    { a4::vec4 mul_res = a4v::mul(vf15, a4v::bcast<1>(vf8)); a4::vec4 res = a4v::addK<1, 1>(vacc, mul_res); vacc = a4v::dest<1, 1, 1, 1>(vacc, res); }
    { a4::vec4 mul_res = a4v::mul(vf16, a4v::bcast<2>(vf8)); a4::vec4 res = a4v::addK<1, 1>(vacc, mul_res); vacc = a4v::dest<1, 1, 1, 1>(vacc, res); }
    { a4::vec4 mul_res = a4v::mul(vf17, a4v::bcast<3>(vf8)); a4::vec4 res = a4v::addK<1, 1>(vacc, mul_res); a4::q128 mask = a4v::set_epi32(-1, -1, -1, -1); vf10 = a4v::blendv_ps(vf10, res, a4v::castsi128_ps(mask)); }
    A4_W128(ADD32((uint32_t)v0, 0), a4v::castps_si128(vf10));
    A1_W32B((uint32_t)sp, 68, 0u);
    vf5 = a4v::castsi128_ps(A4_R128(ADD32((uint32_t)v0, 0)));
    { a4::vec4 res = a4v::mul(vf5, vf5); a4::q128 mask = a4v::set_epi32(0, -1, -1, -1); vf7 = a4v::blend(vf7, res, a4v::castsi128_ps(mask)); }
    { a4::vec4 res = a4v::mulK<1, 1>(vf0, a4v::bcast<0>(vf7)); vacc = a4v::dest<0, 0, 0, 1>(vacc, res); }
    { a4::vec4 mul_res = a4v::mulK<1, 1>(vf0, a4v::bcast<1>(vf7)); a4::vec4 res = a4v::addK<1, 1>(vacc, mul_res); vacc = a4v::dest<0, 0, 0, 1>(vacc, res); }
    { a4::vec4 mul_res = a4v::mulK<1, 1>(vf0, a4v::bcast<2>(vf7)); a4::vec4 res = a4v::addK<1, 1>(vacc, mul_res); a4::q128 mask = a4v::set_epi32(-1, 0, 0, 0); vf7 = a4v::blendv_ps(vf7, res, a4v::castsi128_ps(mask)); }
    { float fs = a4v::cvtss_f32(a4v::shuffle_ps<0x03>(vf0, vf0)); float ft = a4v::cvtss_f32(a4v::shuffle_ps<0x03>(vf7, vf7)); vq = a4v::rsqrt(vstat, fs, ft); }
    { a4::vec4 res = a4v::mul(vf5, a4v::set1_ps(vq)); a4::q128 mask = a4v::set_epi32(0, -1, -1, -1); vf5 = a4v::blendv_ps(vf5, res, a4v::castsi128_ps(mask)); }
    A4_W128(ADD32((uint32_t)v0, 0), a4v::castps_si128(vf5));
    { uint32_t bits = A1_R32B((uint32_t)sp, 64); float f; std::memcpy(&f, &bits, sizeof(f)); f12 = f; }
    { { uint32_t bits = A1_R32B((uint32_t)sp, 72); float f; std::memcpy(&f, &bits, sizeof(f)); f13 = f; } A3_CHARGE(); f0 = a9x::atan2Exact(ram, rt, f12, f13); }
    f1 = ((float)(f0));
    (at = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)15502 << 16))));
    (at = (uint64_t)(RC_U64X(at) | (uint64_t)(uint16_t)64053));
    { uint32_t bits = (uint32_t)at; std::memcpy(&f0, &bits, sizeof(bits)); }
    f0 = a1f::mul(f20, f0);
    fcr31 = (a1f::lt(f0, f1)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
    { const bool c = (fcr31 & 0x800000u) == 0u; (a0 = (uint64_t)((uint64_t)s2 + (uint64_t)0ULL)); if (c) goto L_1450ff8; }
    { const uint32_t _dynInsn = A1_R32(0x1450FDCu); const int32_t _dynSimm = (int16_t)(_dynInsn & 0xFFFFu); const uint32_t _dynImm = _dynInsn & 0xFFFFu; (void)_dynSimm; (void)_dynImm; (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)_dynImm << 16)))); }
    { const uint32_t _dynInsn = A1_R32(0x1450FE0u); const int32_t _dynSimm = (int16_t)(_dynInsn & 0xFFFFu); const uint32_t _dynImm = _dynInsn & 0xFFFFu; (void)_dynSimm; (void)_dynImm; (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32(ADD32((uint32_t)v0, _dynSimm))))); }
    { (a1 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)v1, 4)))); A3_CHARGE(); v0 = CharaCtrlSetScriptCtrlStatus_u11(ram, rt, v0, a0, a1, sp, f0, f1, f2, f3, f4, f5, f6, f7, f8, f9, f10, f11, f12, f13, f14, f15, f16, f17, f18, f19, f20, f21, f22, f23, f24, f25, f26, f27, f28, f29, f30, f31, h2, h5); }
    goto L_145103c;
L_1450ff8:;
    f0 = (-(float)(f0));
    fcr31 = (a1f::lt(f1, f0)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
    if ((fcr31 & 0x800000u) == 0u) goto L_145103c;
    { const uint32_t _dynInsn = A1_R32(0x1451008u); const int32_t _dynSimm = (int16_t)(_dynInsn & 0xFFFFu); const uint32_t _dynImm = _dynInsn & 0xFFFFu; (void)_dynSimm; (void)_dynImm; (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)_dynImm << 16)))); }
    (a0 = (uint64_t)((uint64_t)s2 + (uint64_t)0ULL));
    { const uint32_t _dynInsn = A1_R32(0x1451010u); const int32_t _dynSimm = (int16_t)(_dynInsn & 0xFFFFu); const uint32_t _dynImm = _dynInsn & 0xFFFFu; (void)_dynSimm; (void)_dynImm; (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32(ADD32((uint32_t)v1, _dynSimm))))); }
    { (a1 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)v0, 4)))); A3_CHARGE(); v0 = CharaCtrlSetScriptCtrlStatus_u11(ram, rt, v0, a0, a1, sp, f0, f1, f2, f3, f4, f5, f6, f7, f8, f9, f10, f11, f12, f13, f14, f15, f16, f17, f18, f19, f20, f21, f22, f23, f24, f25, f26, f27, f28, f29, f30, f31, h2, h5); }
    goto L_145103c;
L_1451028:;
    { const uint32_t _dynInsn = A1_R32(0x1451028u); const int32_t _dynSimm = (int16_t)(_dynInsn & 0xFFFFu); const uint32_t _dynImm = _dynInsn & 0xFFFFu; (void)_dynSimm; (void)_dynImm; (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)_dynImm << 16)))); }
    { const uint32_t _dynInsn = A1_R32(0x145102Cu); const int32_t _dynSimm = (int16_t)(_dynInsn & 0xFFFFu); const uint32_t _dynImm = _dynInsn & 0xFFFFu; (void)_dynSimm; (void)_dynImm; (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32(ADD32((uint32_t)v0, _dynSimm))))); }
    { (a1 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)v1, 4)))); A3_CHARGE(); v0 = CharaCtrlSetScriptCtrlStatus_u11(ram, rt, v0, a0, a1, sp, f0, f1, f2, f3, f4, f5, f6, f7, f8, f9, f10, f11, f12, f13, f14, f15, f16, f17, f18, f19, f20, f21, f22, f23, f24, f25, f26, f27, f28, f29, f30, f31, h2, h5); }
L_145103c:;
    { uint32_t bits = A1_R32B((uint32_t)sp, 112); float f; std::memcpy(&f, &bits, sizeof(f)); f20 = f; }
    { (sp = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)sp, 128)))); io_vf4 = vf4; io_vf5 = vf5; io_vf7 = vf7; io_vf8 = vf8; io_vf10 = vf10; io_vf11 = vf11; io_vf12 = vf12; io_vf13 = vf13; io_vf14 = vf14; io_vf15 = vf15; io_vf16 = vf16; io_vf17 = vf17; io_vf24 = vf24; io_vf25 = vf25; io_vf26 = vf26; io_vf28 = vf28; io_vf29 = vf29; io_vacc = vacc; io_vq = vq; io_vstat = vstat; io_h2 = h2; io_h8 = h8; io_h9 = h9; io_h10 = h10; io_h11 = h11; io_h12 = h12; io_h13 = h13; io_h14 = h14; io_h15 = h15; return v0; }
    __builtin_unreachable();
}

// SCRConditionAttainRange (0x1459c80): typed by tools/typed_lift2.py (round A6; guest frame calls; VU0 in vf0 vf7 vf10 vf12 vf13 vf14 vf15 vf16 vf17 vf28 vf29 vacc vstat h5, out vf4 vf5 vf6 vf7 vf8 vf10 vf11 vf12 vf13 vf14 vf15 vf16 vf17 vf24 vf25 vf26 vf27 vf28 vf29 vacc vq vstat h2 h3 h8 h9 h10 h11 h12 h13 h14 h15, relocated calls 2)
inline a1::RetVF SCRConditionAttainRange(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint64_t v0, uint64_t a0, uint64_t sp, float f0, float f1, float f2, float f3, float f4, float f5, float f6, float f7, float f8, float f9, float f10, float f11, float f12, float f13, float f14, float f15, float f16, float f17, float f18, float f19, float f20, float f21, float f22, float f23, float f24, float f25, float f26, float f27, float f28, float f29, float f30, float f31, const a4::vec4 &io_vf0, a4::vec4 &io_vf4, a4::vec4 &io_vf5, a4::vec4 &io_vf6, a4::vec4 &io_vf7, a4::vec4 &io_vf8, a4::vec4 &io_vf10, a4::vec4 &io_vf11, a4::vec4 &io_vf12, a4::vec4 &io_vf13, a4::vec4 &io_vf14, a4::vec4 &io_vf15, a4::vec4 &io_vf16, a4::vec4 &io_vf17, a4::vec4 &io_vf24, a4::vec4 &io_vf25, a4::vec4 &io_vf26, a4::vec4 &io_vf27, a4::vec4 &io_vf28, a4::vec4 &io_vf29, a4::vec4 &io_vacc, float &io_vq, uint16_t &io_vstat, uint64_t &io_h2, uint64_t &io_h3, uint64_t io_h5, uint64_t &io_h8, uint64_t &io_h9, uint64_t &io_h10, uint64_t &io_h11, uint64_t &io_h12, uint64_t &io_h13, uint64_t &io_h14, uint64_t &io_h15)
{
    uint64_t v1 = 0;
    uint64_t a1 = 0;
    uint64_t a2 = 0;
    uint64_t a3 = 0;
    uint64_t s0 = 0;
    uint64_t s1 = 0;
    uint64_t s2 = 0;
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
    a4::vec4 vf24 = io_vf24;
    a4::vec4 vf25 = io_vf25;
    a4::vec4 vf26 = io_vf26;
    a4::vec4 vf27 = io_vf27;
    a4::vec4 vf28 = io_vf28;
    a4::vec4 vf29 = io_vf29;
    a4::vec4 vacc = io_vacc;
    float vq = io_vq;
    uint16_t vstat = io_vstat;
    uint64_t h2 = io_h2;
    uint64_t h3 = io_h3;
    uint64_t h5 = io_h5;
    uint64_t h8 = io_h8;
    uint64_t h9 = io_h9;
    uint64_t h10 = io_h10;
    uint64_t h11 = io_h11;
    uint64_t h12 = io_h12;
    uint64_t h13 = io_h13;
    uint64_t h14 = io_h14;
    uint64_t h15 = io_h15;
    (sp = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)sp, 4294967232))));
    (s0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)sp, 64))));
    { (s2 = (uint64_t)((uint64_t)a0 + (uint64_t)0ULL)); A3_CHARGE(); v0 = getSCRDataCtrlWork(ram, rt, v0, a0, f12, f13, f14, f15, f16, f17, f18, f19, h2); }
    (s1 = (uint64_t)((uint64_t)v0 + (uint64_t)0ULL));
    (a1 = (uint64_t)((uint64_t)s0 + (uint64_t)0ULL));
    (v1 = (uint64_t)(A1_R64B((uint32_t)s1, 992)));
    (a0 = (uint64_t)((uint64_t)s1 + (uint64_t)0ULL));
    A1_W64B((uint32_t)sp, 0, v1);
    (v0 = (uint64_t)(A1_R64B((uint32_t)s1, 1000)));
    A1_W64B((uint32_t)sp, 8, v0);
    (v1 = (uint64_t)(A1_R64B((uint32_t)s1, 1008)));
    A1_W64B((uint32_t)sp, 16, v1);
    (v0 = (uint64_t)(A1_R64B((uint32_t)s1, 1016)));
    { A1_W64B((uint32_t)sp, 24, v0); A3_CHARGE(); uint64_t a7c[4] = {(uint64_t)a1_bits(f0), (uint64_t)a1_bits(f1), (uint64_t)a1_bits(f2), (uint64_t)a1_bits(f3)}; v0 = setSCRAttainRange_x11(ram, rt, (uint32_t)a0, a1, (uint32_t)sp, a7c); f0 = a1_flt((uint32_t)a7c[0]); f1 = a1_flt((uint32_t)a7c[1]); f2 = a1_flt((uint32_t)a7c[2]); f3 = a1_flt((uint32_t)a7c[3]); }
    { (a0 = (uint64_t)((uint64_t)s2 + (uint64_t)0ULL)); A3_CHARGE(); v0 = GetLayoutClass_u11(ram, rt, a0, f0, f1, f2, f3, f4, f5, f6, f7, f8, f9, f10, f11, f12, f13, f14, f15, f16, f17, f18, f19, f20, f21, f22, f23, f24, f25, f26, f27, f28, f29, f30, f31, h2); }
    (a0 = (uint64_t)((uint64_t)s2 + (uint64_t)0ULL));
    { (s0 = (uint64_t)((uint64_t)v0 + (uint64_t)0ULL)); A3_CHARGE(); v0 = GetCharacterClass(ram, rt, a0, h2, h5); }
    (a3 = (uint64_t)((uint64_t)v0 + (uint64_t)0ULL));
    (a2 = (uint64_t)((uint64_t)s0 + (uint64_t)0ULL));
    (a0 = (uint64_t)((uint64_t)s1 + (uint64_t)0ULL));
    { (a1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)s1, 96)))); A3_CHARGE(); { const a1::RetVF a2_r = chkAttainExecLayout(ram, rt, a0, a1, a2, a3, (uint32_t)sp, f13, f14, f15, f16, f17, f18, f19, f20, f21, f22, f23, vf0, vf4, vf5, vf6, vf7, vf8, vf10, vf11, vf12, vf13, vf14, vf15, vf16, vf17, vf24, vf25, vf26, vf27, vf28, vf29, vacc, vq, vstat, h2, h3, h8, h9, h10, h11, h12, h13, h14, h15); v0 = a2_r.v0; f0 = a2_r.f0; } }
    (a0 = (uint64_t)((uint64_t)s1 + (uint64_t)0ULL));
    (s0 = (uint64_t)((uint64_t)v0 + (uint64_t)0ULL));
    { (a1 = (uint64_t)((uint64_t)sp + (uint64_t)0ULL)); A3_CHARGE(); v0 = setAttainRangeStruct(ram, rt, (uint32_t)a0, (uint32_t)a1); }
    (s0 = (uint64_t)(s0 ^ (uint64_t)(uint16_t)1));
    (v0 = (uint64_t)(((uint64_t)s0 < (uint64_t)(int64_t)(int32_t)1) ? 1 : 0));
    { (sp = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)sp, 64)))); io_vf4 = vf4; io_vf5 = vf5; io_vf6 = vf6; io_vf7 = vf7; io_vf8 = vf8; io_vf10 = vf10; io_vf11 = vf11; io_vf12 = vf12; io_vf13 = vf13; io_vf14 = vf14; io_vf15 = vf15; io_vf16 = vf16; io_vf17 = vf17; io_vf24 = vf24; io_vf25 = vf25; io_vf26 = vf26; io_vf27 = vf27; io_vf28 = vf28; io_vf29 = vf29; io_vacc = vacc; io_vq = vq; io_vstat = vstat; io_h2 = h2; io_h3 = h3; io_h8 = h8; io_h9 = h9; io_h10 = h10; io_h11 = h11; io_h12 = h12; io_h13 = h13; io_h14 = h14; io_h15 = h15; return a1::RetVF{(uint64_t)v0, f0}; }
    __builtin_unreachable();
}

// nico_calcWormGenealogicalNodeNextBase (0x14635e0): typed by tools/typed_lift2.py (round A6; guest frame calls, extra outputs xo[0]=f0 xo[1]=f20; VU0 in vf0 vf8 vf10 vf12 vf13 vf14 vf15 vf16 vf17 vf25 vf28 vf29 vacc vstat h3 h4 h6 h8, out vf4 vf5 vf6 vf7 vf8 vf10 vf11 vf12 vf13 vf14 vf15 vf16 vf17 vf24 vf25 vf26 vf27 vf28 vf29 vacc vq vstat h2 h3 h4 h8 h9 h10 h11 h12 h13 h14 h15, relocated calls 1)
inline uint64_t nico_calcWormGenealogicalNodeNextBase(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint64_t v0, uint64_t a0, uint64_t t1, uint64_t sp, float f0, float f1, float f2, float f3, float f4, float f5, float f6, float f7, float f8, float f9, float f10, float f11, float f12, float f13, float f14, float f15, float f16, float f17, float f18, float f19, float f20, float f21, float f22, float f23, float f24, float f25, float f26, float f27, float f28, float f29, float f30, float f31, const a4::vec4 &io_vf0, a4::vec4 &io_vf4, a4::vec4 &io_vf5, a4::vec4 &io_vf6, a4::vec4 &io_vf7, a4::vec4 &io_vf8, a4::vec4 &io_vf10, a4::vec4 &io_vf11, a4::vec4 &io_vf12, a4::vec4 &io_vf13, a4::vec4 &io_vf14, a4::vec4 &io_vf15, a4::vec4 &io_vf16, a4::vec4 &io_vf17, a4::vec4 &io_vf24, a4::vec4 &io_vf25, a4::vec4 &io_vf26, a4::vec4 &io_vf27, a4::vec4 &io_vf28, a4::vec4 &io_vf29, a4::vec4 &io_vacc, float &io_vq, uint16_t &io_vstat, uint64_t &io_h2, uint64_t &io_h3, uint64_t &io_h4, uint64_t io_h6, uint64_t &io_h8, uint64_t &io_h9, uint64_t &io_h10, uint64_t &io_h11, uint64_t &io_h12, uint64_t &io_h13, uint64_t &io_h14, uint64_t &io_h15, uint64_t *xo = nullptr)
{
    uint32_t at = 0;
    uint32_t v1 = 0;
    uint64_t a1 = 0;
    uint32_t a2 = 0;
    uint32_t a3 = 0;
    uint32_t t0 = 0;
    uint64_t t2 = 0;
    uint64_t s0 = 0;
    uint64_t s1 = 0;
    uint64_t s2 = 0;
    uint64_t s3 = 0;
    uint32_t s4 = 0;
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
    a4::vec4 vf24 = io_vf24;
    a4::vec4 vf25 = io_vf25;
    a4::vec4 vf26 = io_vf26;
    a4::vec4 vf27 = io_vf27;
    a4::vec4 vf28 = io_vf28;
    a4::vec4 vf29 = io_vf29;
    a4::vec4 vacc = io_vacc;
    float vq = io_vq;
    uint16_t vstat = io_vstat;
    uint64_t h2 = io_h2;
    uint64_t h3 = io_h3;
    uint64_t h4 = io_h4;
    uint64_t h6 = io_h6;
    uint64_t h8 = io_h8;
    uint64_t h9 = io_h9;
    uint64_t h10 = io_h10;
    uint64_t h11 = io_h11;
    uint64_t h12 = io_h12;
    uint64_t h13 = io_h13;
    uint64_t h14 = io_h14;
    uint64_t h15 = io_h15;
    bool a3xw0 = false;
    (sp = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)sp, 4294967200))));
    (s0 = (uint64_t)((uint64_t)a0 + (uint64_t)0ULL));
    { float f = f20; uint32_t bits; std::memcpy(&bits, &f, sizeof(bits)); A1_W32B((uint32_t)sp, 80, bits); }
    (s2 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)s0, 0))));
    { (a0 = (uint64_t)((uint64_t)RC_U64X(s2) + (uint64_t)0ULL)); A3_CHARGE(); v0 = GetSCRCharaAIWorkPtr(ram, rt, v0, a0, f12, f13, f14, f15, f16, f17, f18, f19, h2); }
    v1 = 0xFFFFFFF0u;
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v0, 16819))));
    (s1 = (uint64_t)(RC_U64X(v0) & a1_sx64(v1)));
    (v0 = (uint64_t)((int64_t)(int32_t)((int8_t)A1_R8B((uint32_t)s1, 4760))));
    { const bool c = (int64_t)v0 == 0; (a1 = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)326 << 16)))); if (c) goto L_14637f0; }
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)s0, 16))));
    (v1 = (uint32_t)((int32_t)((int32_t)A1_R32B((uint32_t)s0, 20))));
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)v0, 8))));
    v1 = v1 << 6;
    (v1 = (uint32_t)((int32_t)((int32_t)ADD32(v1, (uint32_t)a0))));
    v1 = v1 + 0x10u;
    vf7 = a4v::castsi128_ps(A4_R128(ADD32(v1, 0)));
    { a4::vec4 res = a4v::mul(vf7, vf7); a4::q128 mask = a4v::set_epi32(0, -1, -1, -1); vf7 = a4v::blend(vf7, res, a4v::castsi128_ps(mask)); }
    { a4::vec4 res = a4v::mulK<1, 1>(vf0, a4v::bcast<0>(vf7)); vacc = a4v::dest<0, 0, 0, 1>(vacc, res); }
    { a4::vec4 mul_res = a4v::mulK<1, 1>(vf0, a4v::bcast<1>(vf7)); a4::vec4 res = a4v::addK<1, 1>(vacc, mul_res); vacc = a4v::dest<0, 0, 0, 1>(vacc, res); }
    { a4::vec4 mul_res = a4v::mulK<1, 1>(vf0, a4v::bcast<2>(vf7)); a4::vec4 res = a4v::addK<1, 1>(vacc, mul_res); a4::q128 mask = a4v::set_epi32(-1, 0, 0, 0); vf7 = a4v::blendv_ps(vf7, res, a4v::castsi128_ps(mask)); }
    { float ft = a4v::cvtss_f32(a4v::shuffle_ps<0x03>(vf7, vf7)); vq = a4v::sqrt(vstat, ft); }
    { uint32_t bits; std::memcpy(&bits, &vq, sizeof(bits)); (v0 = (uint64_t)((int64_t)(int32_t)(bits))); }
    { uint32_t bits = (uint32_t)v0; std::memcpy(&f20, &bits, sizeof(bits)); }
    s4 = 0x1460000u;
    (a0 = (uint64_t)((uint64_t)s2 + (uint64_t)0ULL));
    { (s2 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(s4, 29704)))); A3_CHARGE(); v0 = GetLayoutClass_u11(ram, rt, a0, f0, f1, f2, f3, f4, f5, f6, f7, f8, f9, f10, f11, f12, f13, f14, f15, f16, f17, f18, f19, f20, f21, f22, f23, f24, f25, f26, f27, f28, f29, f30, f31, h2); }
    (v1 = (uint32_t)((int32_t)((int32_t)A1_R32B((uint32_t)s2, 0))));
    (s3 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)sp, 16))));
    { uint32_t bits = A1_R32B((uint32_t)v0, 152); float f; std::memcpy(&f, &bits, sizeof(f)); f12 = f; }
    (a0 = (uint64_t)((uint64_t)sp + (uint64_t)0ULL));
    a3 = v1 << 2;
    (a1 = (uint64_t)((uint64_t)RC_U64X(s3) + (uint64_t)0ULL));
    a3 = a3 + v1;
    f12 = a1f::mul(f20, f12);
    a3 = a3 << 4;
    (a2 = (uint32_t)((int32_t)((int32_t)ADD32((uint32_t)s1, 4768))));
    (a3 = (uint32_t)((int32_t)((int32_t)ADD32((uint32_t)s1, a3))));
    { a3 = a3 + 0x10u; A3_CHARGE(); uint64_t a7c[8] = {0u, (uint64_t)a1_bits(f2), (uint64_t)a1_bits(f3), (uint64_t)a1_bits(f4), (uint64_t)a1_bits(f5), (uint64_t)a1_bits(f6), (uint64_t)a1_bits(f7), 0u}; { const a1::RetVF a2_r = gamecore_sub_01462340_x11(ram, rt, a0, a1, a1_sx64(a2), a3, (uint32_t)sp, f12, vf0, vf5, vf6, vf7, vf8, vacc, vq, vstat, h2, h3, h4, h6, h8, a7c); v0 = a2_r.v0; f0 = a2_r.f0; } f1 = a1_flt((uint32_t)a7c[0]); f2 = a1_flt((uint32_t)a7c[1]); f3 = a1_flt((uint32_t)a7c[2]); f4 = a1_flt((uint32_t)a7c[3]); f5 = a1_flt((uint32_t)a7c[4]); f6 = a1_flt((uint32_t)a7c[5]); f7 = a1_flt((uint32_t)a7c[6]); f13 = a1_flt((uint32_t)a7c[7]); a3xw0 = true; }
    { const bool c = (int64_t)v0 == 0; (t2 = (uint64_t)((uint64_t)s3 + (uint64_t)0ULL)); if (c) goto L_1463708; }
    (t0 = (uint32_t)((int32_t)((int32_t)A1_R32B((uint32_t)s2, 0))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)s0, 20))));
    (a1 = (uint64_t)((uint64_t)s0 + (uint64_t)0ULL));
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)SLL32(t0, 2))));
    (a3 = (uint32_t)((int32_t)((int32_t)A1_R32B((uint32_t)s0, 8))));
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)a0, t0))));
    t0 = t0 << 4;
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)SLL32((uint32_t)v0, 6))));
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)SLL32((uint32_t)a0, 4))));
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)s1, (uint32_t)a0))));
    (a3 = (uint32_t)((int32_t)((int32_t)ADD32(a3, (uint32_t)v0))));
    (t0 = (uint32_t)((int32_t)((int32_t)ADD32((uint32_t)s1, t0))));
    at = 0x43340000u;
    { uint32_t bits = a7_opq(at); std::memcpy(&f12, &bits, sizeof(bits)); }
    (a2 = (uint32_t)((int32_t)((int32_t)ADD32((uint32_t)a0, 4294967232))));
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)a0, 16))));
    a3 = a3 + 0x30u;
    t0 = t0 + 0xA10u;
    { (t1 = (uint64_t)((uint64_t)sp + (uint64_t)0ULL)); A3_CHARGE(); { const a1::RetVF a2_r = gamecore_sub_01462B30(ram, rt, v0, a0, a1, a1_sx64(a2), a1_sx64(a3), a1_sx64(t0), t1, t2, sp, f0, f1, f2, f3, f4, f5, f6, f7, f8, f9, f10, f11, f12, f13, f14, f15, f16, f17, f18, f19, f20, f21, f22, f23, f24, f25, f26, f27, f28, f29, f30, f31, vf0, vf4, vf5, vf6, vf7, vf8, vf10, vf11, vf12, vf13, vf14, vf15, vf16, vf17, vf24, vf25, vf26, vf27, vf28, vf29, vacc, vq, vstat, h2, h3, h8, h9, h10, h11, h12, h13, h14, h15); v0 = a2_r.v0; f0 = a2_r.f0; } a3xw0 = true; }
    a2 = s4 + 0x7408u;
    goto L_146375c;
L_1463708:;
    (t0 = (uint32_t)((int32_t)((int32_t)A1_R32B((uint32_t)s2, 0))));
    (a1 = (uint64_t)((uint64_t)s0 + (uint64_t)0ULL));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)s0, 20))));
    (t2 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL));
    (t1 = (uint64_t)((int64_t)(int32_t)((int32_t)SLL32(t0, 2))));
    (a3 = (uint32_t)((int32_t)((int32_t)A1_R32B((uint32_t)s0, 8))));
    (t1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)t1, t0))));
    t0 = t0 << 4;
    (t1 = (uint64_t)((int64_t)(int32_t)((int32_t)SLL32((uint32_t)t1, 4))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)SLL32((uint32_t)v0, 6))));
    (t1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)s1, (uint32_t)t1))));
    (a3 = (uint32_t)((int32_t)((int32_t)ADD32(a3, (uint32_t)v0))));
    (t0 = (uint32_t)((int32_t)((int32_t)ADD32((uint32_t)s1, t0))));
    { uint32_t bits = A1_R32B((uint32_t)t1, 64); float f; std::memcpy(&f, &bits, sizeof(f)); f12 = f; }
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)t1, 16))));
    (a2 = (uint32_t)((int32_t)((int32_t)ADD32((uint32_t)t1, 4294967232))));
    a3 = a3 + 0x30u;
    t0 = t0 + 0xA10u;
    { (t1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)t1, 96)))); A3_CHARGE(); { const a1::RetVF a2_r = gamecore_sub_01462B30(ram, rt, v0, a0, a1, a1_sx64(a2), a1_sx64(a3), a1_sx64(t0), t1, t2, sp, f0, f1, f2, f3, f4, f5, f6, f7, f8, f9, f10, f11, f12, f13, f14, f15, f16, f17, f18, f19, f20, f21, f22, f23, f24, f25, f26, f27, f28, f29, f30, f31, vf0, vf4, vf5, vf6, vf7, vf8, vf10, vf11, vf12, vf13, vf14, vf15, vf16, vf17, vf24, vf25, vf26, vf27, vf28, vf29, vacc, vq, vstat, h2, h3, h8, h9, h10, h11, h12, h13, h14, h15); v0 = a2_r.v0; f0 = a2_r.f0; } a3xw0 = true; }
    a2 = s4 + 0x7408u;
L_146375c:;
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)s0, 20))));
    v1 = A1_R32(a2);
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)s0, 4))));
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)SLL32((uint32_t)a0, 4))));
    v1 = v1 << 4;
    (v1 = (uint32_t)((int32_t)((int32_t)ADD32(v1, (uint32_t)s1))));
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)a0, (uint32_t)v0))));
    (a1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(v1, 2576))));
    v1 = v1 + 0xA20u;
    vf11 = a4v::castsi128_ps(A4_R128(ADD32((uint32_t)a1, 0)));
    vf12 = a4v::castsi128_ps(A4_R128(ADD32((uint32_t)a0, 0)));
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
    A4_W128(ADD32(v1, 0), a4v::castps_si128(vf13));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B(a2, 0))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)SLL32((uint32_t)v0, 4))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v0, (uint32_t)s1))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v0, 2592))));
    vf14 = a4v::castsi128_ps(A4_R128(ADD32((uint32_t)v0, 0)));
    { a4::vec4 res = a4v::mul(vf14, vf14); a4::q128 mask = a4v::set_epi32(-1, -1, -1, -1); vf15 = a4v::blend(vf15, res, a4v::castsi128_ps(mask)); }
    { a4::vec4 res = a4v::addK<1, 1>(vf15, a4v::bcast<1>(vf15)); a4::q128 mask = a4v::set_epi32(0, 0, 0, -1); vf15 = a4v::blendv_ps(vf15, res, a4v::castsi128_ps(mask)); }
    { a4::vec4 res = a4v::addK<1, 1>(vf15, a4v::bcast<2>(vf15)); a4::q128 mask = a4v::set_epi32(0, 0, 0, -1); vf15 = a4v::blendv_ps(vf15, res, a4v::castsi128_ps(mask)); }
    { a4::vec4 res = a4v::addK<1, 1>(vf15, a4v::bcast<3>(vf15)); a4::q128 mask = a4v::set_epi32(0, 0, 0, -1); vf15 = a4v::blendv_ps(vf15, res, a4v::castsi128_ps(mask)); }
    { float fs = a4v::cvtss_f32(a4v::shuffle_ps<0x03>(vf0, vf0)); float ft = a4v::cvtss_f32(a4v::bcast<0>(vf15)); vq = a4v::rsqrt(vstat, fs, ft); }
    { a4::vec4 res = a4v::mul(vf14, a4v::set1_ps(vq)); a4::q128 mask = a4v::set_epi32(-1, -1, -1, -1); vf15 = a4v::blendv_ps(vf15, res, a4v::castsi128_ps(mask)); }
    A4_W128(ADD32((uint32_t)v0, 0), a4v::castps_si128(vf15));
    goto L_1463848;
L_14637f0:;
    (v1 = (uint32_t)((int32_t)((int32_t)A1_R32B((uint32_t)s0, 20))));
    (a1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)a1, 29704))));
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)s0, 8))));
    (a2 = (uint32_t)((int32_t)((int32_t)A1_R32B((uint32_t)a1, 0))));
    v1 = v1 << 6;
    (v1 = (uint32_t)((int32_t)((int32_t)ADD32(v1, (uint32_t)a0))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)SLL32(a2, 2))));
    do { const a4::q128 _v = (A4_R128(ADD32(v1, 48))); h4 = a4::hi64(_v); std::memcpy(&a0, &_v, 8); } while (0);
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v0, a2))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)SLL32((uint32_t)v0, 4))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v0, (uint32_t)s1))));
    A4_W128(ADD32((uint32_t)v0, 16), a4::mk128(h4, a0));
    (v1 = (uint32_t)((int32_t)((int32_t)A1_R32B((uint32_t)s0, 20))));
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)s0, 12))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)a1, 0))));
    v1 = v1 << 4;
    (v1 = (uint32_t)((int32_t)((int32_t)ADD32(v1, (uint32_t)a0))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)SLL32((uint32_t)v0, 4))));
    do { const a4::q128 _v = (A4_R128(ADD32(v1, 0))); h4 = a4::hi64(_v); std::memcpy(&a0, &_v, 8); } while (0);
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v0, (uint32_t)s1))));
    A4_W128(ADD32((uint32_t)v0, 2592), a4::mk128(h4, a0));
L_1463848:;
    { uint32_t bits = A1_R32B((uint32_t)sp, 80); float f; std::memcpy(&f, &bits, sizeof(f)); f20 = f; }
    { (sp = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)sp, 96)))); if (xo) { if (a3xw0) xo[0] = a1_bits(f0); xo[1] = a1_bits(f20); } io_vf4 = vf4; io_vf5 = vf5; io_vf6 = vf6; io_vf7 = vf7; io_vf8 = vf8; io_vf10 = vf10; io_vf11 = vf11; io_vf12 = vf12; io_vf13 = vf13; io_vf14 = vf14; io_vf15 = vf15; io_vf16 = vf16; io_vf17 = vf17; io_vf24 = vf24; io_vf25 = vf25; io_vf26 = vf26; io_vf27 = vf27; io_vf28 = vf28; io_vf29 = vf29; io_vacc = vacc; io_vq = vq; io_vstat = vstat; io_h2 = h2; io_h3 = h3; io_h4 = h4; io_h8 = h8; io_h9 = h9; io_h10 = h10; io_h11 = h11; io_h12 = h12; io_h13 = h13; io_h14 = h14; io_h15 = h15; return v0; }
    __builtin_unreachable();
}

// GetGeometryPointerCharColAttr (0x10c1f68): typed by tools/typed_lift2.py (round A6; guest frame calls, extra outputs xo[0]=a0 xo[1]=a1 xo[2]=a2 xo[3]=a3; VU0 in -, out h2 h3)
inline uint64_t GetGeometryPointerCharColAttr(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint64_t a0, uint64_t a2, uint64_t a3, uint32_t sp, float f0, float f1, float f2, float f3, float f4, float f5, float f6, float f7, float f8, float f9, float f10, float f11, float f12, float f13, float f14, float f15, float f16, float f17, float f18, float f19, float f20, float f21, float f22, float f23, float f24, float f25, float f26, float f27, float f28, float f29, float f30, float f31, uint64_t &io_h2, uint64_t &io_h3, uint64_t *xo = nullptr)
{
    uint64_t v0 = 0;
    uint64_t a1 = 0;
    uint64_t h2 = io_h2;
    uint64_t h3 = io_h3;
    sp = sp - 0x20u;
    (a1 = (uint64_t)((uint64_t)a0 + (uint64_t)0ULL));
    { (a0 = (uint64_t)((uint64_t)a1_sx64(sp) + (uint64_t)0ULL)); A3_CHARGE(); uint64_t a7c[4] = {a0, a1, a2, a3}; v0 = GetGeometryPointerSurfaceAttribute(ram, rt, a0, a1, a1_sx64(sp), f0, f1, f2, f3, f4, f5, f6, f7, f8, f9, f10, f11, f12, f13, f14, f15, f16, f17, f18, f19, f20, f21, f22, f23, f24, f25, f26, f27, f28, f29, f30, f31, h2, h3, a7c); a0 = a7c[0]; a1 = a7c[1]; a2 = a7c[2]; a3 = a7c[3]; }
    (v0 = (uint64_t)(A1_R64B(sp, 0)));
    { sp = sp + 0x20u; if (xo) { xo[0] = a0; xo[1] = a1; xo[2] = a2; xo[3] = a3; } io_h2 = h2; io_h3 = h3; return v0; }
    __builtin_unreachable();
}

// charrootdyn_getGPPlaneInfoWithVPlane (0x10dc338): typed by tools/typed_lift2.py (round A6; guest frame calls, extra outputs xo[0]=f12 xo[1]=f13 xo[2]=a2 xo[3]=a3 xo[4]=t0 xo[5]=t1 xo[6]=t2 xo[7]=t3; VU0 in vf0 vf5 vf6 vf7 vf8 vf10 vf12 vf13 vf14 vf15 vf16 vf17 vf25 vf28 vf29 vacc vstat, out vf4 vf5 vf6 vf7 vf8 vf10 vf11 vf12 vf13 vf14 vf15 vf16 vf17 vf24 vf25 vf26 vf28 vf29 vacc vq vstat h2 h3 h8 h9 h10 h11 h12 h13 h14 h15)
inline a1::RetVF charrootdyn_getGPPlaneInfoWithVPlane(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint64_t a0, uint64_t a1, uint64_t a2, uint64_t sp, float f0, float f1, float f2, float f3, float f4, float f5, float f6, float f7, float f8, float f9, float f10, float f11, float f12, float f13, float f14, float f15, float f16, float f17, float f18, float f19, float f20, float f21, float f22, float f23, float f24, float f25, float f26, float f27, float f28, float f29, float f30, float f31, const a4::vec4 &io_vf0, a4::vec4 &io_vf4, a4::vec4 &io_vf5, a4::vec4 &io_vf6, a4::vec4 &io_vf7, a4::vec4 &io_vf8, a4::vec4 &io_vf10, a4::vec4 &io_vf11, a4::vec4 &io_vf12, a4::vec4 &io_vf13, a4::vec4 &io_vf14, a4::vec4 &io_vf15, a4::vec4 &io_vf16, a4::vec4 &io_vf17, a4::vec4 &io_vf24, a4::vec4 &io_vf25, a4::vec4 &io_vf26, a4::vec4 &io_vf28, a4::vec4 &io_vf29, a4::vec4 &io_vacc, float &io_vq, uint16_t &io_vstat, uint64_t &io_h2, uint64_t &io_h3, uint64_t &io_h8, uint64_t &io_h9, uint64_t &io_h10, uint64_t &io_h11, uint64_t &io_h12, uint64_t &io_h13, uint64_t &io_h14, uint64_t &io_h15, uint64_t *xo = nullptr)
{
    uint64_t v0 = 0;
    uint64_t a3 = 0;
    uint64_t t0 = 0;
    uint64_t t1 = 0;
    uint64_t t2 = 0;
    uint64_t t3 = 0;
    uint64_t s0 = 0;
    uint64_t s1 = 0;
    uint32_t s2 = 0;
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
    a4::vec4 vf24 = io_vf24;
    a4::vec4 vf25 = io_vf25;
    a4::vec4 vf26 = io_vf26;
    a4::vec4 vf28 = io_vf28;
    a4::vec4 vf29 = io_vf29;
    a4::vec4 vacc = io_vacc;
    float vq = io_vq;
    uint16_t vstat = io_vstat;
    uint64_t h2 = io_h2;
    uint64_t h3 = io_h3;
    uint64_t h8 = io_h8;
    uint64_t h9 = io_h9;
    uint64_t h10 = io_h10;
    uint64_t h11 = io_h11;
    uint64_t h12 = io_h12;
    uint64_t h13 = io_h13;
    uint64_t h14 = io_h14;
    uint64_t h15 = io_h15;
    (sp = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)sp, 4294967216))));
    (s1 = (uint64_t)((uint64_t)a0 + (uint64_t)0ULL));
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)sp, 32))));
    { (s0 = (uint64_t)((uint64_t)a2 + (uint64_t)0ULL)); A3_CHARGE(); v0 = GetGeometryPointerSurfaceAttribute(ram, rt, a0, a1, sp, f0, f1, f2, f3, f4, f5, f6, f7, f8, f9, f10, f11, f12, f13, f14, f15, f16, f17, f18, f19, f20, f21, f22, f23, f24, f25, f26, f27, f28, f29, f30, f31, h2, h3); }
    (v0 = (uint64_t)(A1_R64B((uint32_t)sp, 32)));
    (a0 = (uint64_t)((uint64_t)s0 + (uint64_t)0ULL));
    A1_W64B((uint32_t)s1, 1688, v0);
    { A1_W64B((uint32_t)s1, 1680, v0); A3_CHARGE(); uint64_t a7c[1] = {0u}; f0 = GetPlaneSlopeDegree(ram, rt, (uint32_t)a0, f13, f14, f15, f16, f17, f18, f19, a7c); f12 = a1_flt((uint32_t)a7c[0]); }
    (s2 = (uint32_t)((int32_t)((int32_t)A1_R32B((uint32_t)s1, 316))));
    { float f = f0; uint32_t bits; std::memcpy(&bits, &f, sizeof(bits)); A1_W32B((uint32_t)s1, 1716, bits); }
    (a1 = (uint64_t)((uint64_t)s0 + (uint64_t)0ULL));
    (a0 = (uint64_t)((uint64_t)sp + (uint64_t)0ULL));
    do { const a4::q128 _v = (A4_R128(ADD32((uint32_t)s0, 0))); h2 = a4::hi64(_v); std::memcpy(&v0, &_v, 8); } while (0);
    { A4_W128(ADD32((uint32_t)s1, 1696), a4::mk128(h2, v0)); A3_CHARGE(); { const a1::RetVF a2_r = GetPlaneZTopYPlaneCoordQuaternion(ram, rt, a0, a1, sp, f12, f13, f14, f15, f16, f17, f18, f19, vf0, vf5, vf6, vf7, vacc, vq, vstat, h3); v0 = a2_r.v0; f0 = a2_r.f0; } }
    (a1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)sp, 16))));
    vf4 = a4v::castsi128_ps(A4_R128(ADD32(s2, 0)));
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
    A4_W128(ADD32((uint32_t)a1, 0), a4v::castps_si128(vf6));
    { (a0 = (uint64_t)((uint64_t)sp + (uint64_t)0ULL)); A3_CHARGE(); uint64_t a7c[10] = {0u, 0u, 0u, 0u, 0u, 0u, 0u, 0u, 0u, 0u}; { const a1::RetVF a2_r = GetTurnYAngleFromGlobalDir(ram, rt, a0, a1, sp, f0, f14, f15, f16, f17, f18, f19, vf0, vf5, vf7, vf8, vf10, vf11, vf12, vf13, vf14, vf15, vf16, vf17, vf24, vf25, vf26, vf28, vf29, vacc, vq, vstat, h2, h8, h9, h10, h11, h12, h13, h14, h15, a7c); v0 = a2_r.v0; f0 = a2_r.f0; } f12 = a1_flt((uint32_t)a7c[0]); f13 = a1_flt((uint32_t)a7c[1]); a2 = a7c[4]; a3 = a7c[5]; t0 = a7c[6]; t1 = a7c[7]; t2 = a7c[8]; t3 = a7c[9]; }
    { float f = f0; uint32_t bits; std::memcpy(&bits, &f, sizeof(bits)); A1_W32B((uint32_t)s1, 1724, bits); }
    { (sp = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)sp, 80)))); if (xo) { xo[0] = a1_bits(f12); xo[1] = a1_bits(f13); xo[2] = a2; xo[3] = a3; xo[4] = t0; xo[5] = t1; xo[6] = t2; xo[7] = t3; } io_vf4 = vf4; io_vf5 = vf5; io_vf6 = vf6; io_vf7 = vf7; io_vf8 = vf8; io_vf10 = vf10; io_vf11 = vf11; io_vf12 = vf12; io_vf13 = vf13; io_vf14 = vf14; io_vf15 = vf15; io_vf16 = vf16; io_vf17 = vf17; io_vf24 = vf24; io_vf25 = vf25; io_vf26 = vf26; io_vf28 = vf28; io_vf29 = vf29; io_vacc = vacc; io_vq = vq; io_vstat = vstat; io_h2 = h2; io_h3 = h3; io_h8 = h8; io_h9 = h9; io_h10 = h10; io_h11 = h11; io_h12 = h12; io_h13 = h13; io_h14 = h14; io_h15 = h15; return a1::RetVF{(uint64_t)v0, f0}; }
    __builtin_unreachable();
}

// charrootdyn_getCollisionAttribute (0x10de258): typed by tools/typed_lift2.py (round A6; guest frame calls, extra outputs xo[0]=a2 xo[1]=a3; VU0 in -, out h2 h3)
inline uint64_t charrootdyn_getCollisionAttribute(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint64_t a0, uint64_t a2, uint64_t a3, uint32_t sp, float f0, float f1, float f2, float f3, float f4, float f5, float f6, float f7, float f8, float f9, float f10, float f11, float f12, float f13, float f14, float f15, float f16, float f17, float f18, float f19, float f20, float f21, float f22, float f23, float f24, float f25, float f26, float f27, float f28, float f29, float f30, float f31, uint64_t &io_h2, uint64_t &io_h3, uint64_t *xo = nullptr)
{
    uint64_t v0 = 0;
    uint64_t a1 = 0;
    uint64_t h2 = io_h2;
    uint64_t h3 = io_h3;
    sp = sp - 0x20u;
    (a1 = (uint64_t)((uint64_t)a0 + (uint64_t)0ULL));
    { (a0 = (uint64_t)((uint64_t)a1_sx64(sp) + (uint64_t)0ULL)); A3_CHARGE(); uint64_t a7c[4] = {0u, 0u, a2, a3}; v0 = GetGeometryPointerSurfaceAttribute(ram, rt, a0, a1, a1_sx64(sp), f0, f1, f2, f3, f4, f5, f6, f7, f8, f9, f10, f11, f12, f13, f14, f15, f16, f17, f18, f19, f20, f21, f22, f23, f24, f25, f26, f27, f28, f29, f30, f31, h2, h3, a7c); a2 = a7c[2]; a3 = a7c[3]; }
    (v0 = (uint64_t)(A1_R64B(sp, 0)));
    { sp = sp + 0x20u; if (xo) { xo[0] = a2; xo[1] = a3; } io_h2 = h2; io_h3 = h3; return v0; }
    __builtin_unreachable();
}

// is_clear_camera_region_condition (0x1163618): typed by tools/typed_lift2.py (round A6; frame calls, extra outputs xo[0]=f0 xo[1]=f1 xo[2]=f2 xo[3]=f12 xo[4]=f13 xo[5]=f14 xo[6]=f20 xo[7]=a1 xo[8]=a2 xo[9]=a3 xo[10]=t0 xo[11]=t1 xo[12]=t2 xo[13]=t3; VU0 in vf0 vf2 vf6 vf7 vf9 vf10 vf12 vf13 vf14 vf15 vf16 vf17 vacc, out vf1 vf2 vf3 vf4 vf5 vf6 vf7 vf8 vf9 vf10 vf11 vf12 vf13 vf14 vf15 vf16 vf17 vf24 vf25 vf26 vf27 vacc h2 h3 h6 h7 h8 h9 h10 h11 h12 h13 h14 h15, guest sp, frame 80 bytes, 2 slot accesses)
inline uint64_t is_clear_camera_region_condition(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint64_t v0, uint64_t a0, uint64_t a1, uint64_t a2, uint64_t a3, uint64_t t0, uint64_t t1, uint64_t t2, uint64_t t3, uint32_t sp, float f0, float f1, float f2, float f3, float f4, float f5, float f6, float f7, float f8, float f9, float f10, float f11, float f12, float f13, float f14, float f15, float f16, float f17, float f18, float f19, float f20, float f21, float f22, float f23, float f24, float f25, float f26, float f27, float f28, float f29, float f30, float f31, const a4::vec4 &io_vf0, a4::vec4 &io_vf1, a4::vec4 &io_vf2, a4::vec4 &io_vf3, a4::vec4 &io_vf4, a4::vec4 &io_vf5, a4::vec4 &io_vf6, a4::vec4 &io_vf7, a4::vec4 &io_vf8, a4::vec4 &io_vf9, a4::vec4 &io_vf10, a4::vec4 &io_vf11, a4::vec4 &io_vf12, a4::vec4 &io_vf13, a4::vec4 &io_vf14, a4::vec4 &io_vf15, a4::vec4 &io_vf16, a4::vec4 &io_vf17, a4::vec4 &io_vf24, a4::vec4 &io_vf25, a4::vec4 &io_vf26, a4::vec4 &io_vf27, a4::vec4 &io_vacc, uint64_t &io_h2, uint64_t &io_h3, uint64_t &io_h6, uint64_t &io_h7, uint64_t &io_h8, uint64_t &io_h9, uint64_t &io_h10, uint64_t &io_h11, uint64_t &io_h12, uint64_t &io_h13, uint64_t &io_h14, uint64_t &io_h15, uint64_t *xo = nullptr)
{
    uint64_t v1 = 0;
    uint64_t s0 = 0;
    uint64_t s1 = 0;
    uint64_t s2 = 0;
    uint64_t s3 = 0;
    uint64_t s4 = 0;
    uint64_t s5 = 0;
    uint64_t s6 = 0;
    uint32_t fcr31 = 0;
    a4::vec4 vf0 = io_vf0;
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
    a4::vec4 vf24 = io_vf24;
    a4::vec4 vf25 = io_vf25;
    a4::vec4 vf26 = io_vf26;
    a4::vec4 vf27 = io_vf27;
    a4::vec4 vacc = io_vacc;
    uint64_t h2 = io_h2;
    uint64_t h3 = io_h3;
    uint64_t h6 = io_h6;
    uint64_t h7 = io_h7;
    uint64_t h8 = io_h8;
    uint64_t h9 = io_h9;
    uint64_t h10 = io_h10;
    uint64_t h11 = io_h11;
    uint64_t h12 = io_h12;
    uint64_t h13 = io_h13;
    uint64_t h14 = io_h14;
    uint64_t h15 = io_h15;
    uint8_t a2fr[80];
    bool a3xw0 = false;
    bool a3xw1 = false;
    bool a3xw2 = false;
    bool a3xw3 = false;
    bool a3xw4 = false;
    bool a3xw5 = false;
    bool a3xw6 = false;
    bool a3xw10 = false;
    bool a3xw11 = false;
    bool a3xw12 = false;
    bool a3xw13 = false;
    (s0 = (uint64_t)((uint64_t)a2 + (uint64_t)0ULL));
    (s1 = (uint64_t)((uint64_t)a1 + (uint64_t)0ULL));
    (s2 = (uint64_t)((uint64_t)a0 + (uint64_t)0ULL));
    (s3 = (uint64_t)((uint64_t)t0 + (uint64_t)0ULL));
    (s5 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL));
    (s6 = (uint64_t)((uint64_t)a3 + (uint64_t)0ULL));
    { (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)s1, 16)))); A3_CHARGE(); v0 = GetLayoutClass_u11(ram, rt, a0, f0, f1, f2, f3, f4, f5, f6, f7, f8, f9, f10, f11, f12, f13, f14, f15, f16, f17, f18, f19, f20, f21, f22, f23, f24, f25, f26, f27, f28, f29, f30, f31, h2); }
    (a0 = (uint64_t)((uint64_t)s1 + (uint64_t)0ULL));
    { (s4 = (uint64_t)((uint64_t)v0 + (uint64_t)0ULL)); A3_CHARGE(); v0 = a1_sx64(is_parent_obj_boss_camera_region(ram, rt, a0)); }
    (a0 = (uint64_t)((uint64_t)s2 + (uint64_t)0ULL));
    (a3 = (uint64_t)((uint64_t)s0 + (uint64_t)0ULL));
    (a1 = (uint64_t)((uint64_t)s4 + (uint64_t)0ULL));
    (a2 = (uint64_t)((uint64_t)s1 + (uint64_t)0ULL));
    { const bool c = (int64_t)v0 == 0; (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 1)))); if (c) goto L_1163688; }
    { const bool c = (int64_t)s3 != (int64_t)v1; (v0 = (uint64_t)((uint64_t)s5 + (uint64_t)0ULL)); if (c) goto L_1163728; }
L_1163688:;
    { A3_CHARGE(); uint64_t a7c[13] = {0u, 0u, (uint64_t)a1_bits(f2), 0u, (uint64_t)a1_bits(f13), (uint64_t)a1_bits(f14), (uint64_t)a1_bits(f20), a2, 0u, t0, t1, t2, t3}; v0 = manager_sub_01162FD8(ram, rt, v0, a0, a1, a2, a3, t0, t1, t2, t3, (uint32_t)(sp + 0xFFFFFFB0u), f0, f1, f2, f3, f4, f5, f6, f7, f8, f9, f10, f11, f12, f13, f14, f15, f16, f17, f18, f19, f20, f21, f22, f23, f24, f25, f26, f27, f28, f29, f30, f31, vf0, vf1, vf2, vf3, vf4, vf5, vf6, vf7, vf8, vf9, vf10, vf11, vf12, vf13, vf14, vf15, vf16, vf17, vf24, vf25, vf26, vf27, vacc, h2, h3, h6, h7, h8, h9, h10, h11, h12, h13, h14, h15, a7c); f0 = a1_flt((uint32_t)a7c[0]); f1 = a1_flt((uint32_t)a7c[1]); f2 = a1_flt((uint32_t)a7c[2]); f12 = a1_flt((uint32_t)a7c[3]); f13 = a1_flt((uint32_t)a7c[4]); f14 = a1_flt((uint32_t)a7c[5]); f20 = a1_flt((uint32_t)a7c[6]); a2 = a7c[7]; a3 = a7c[8]; t0 = a7c[9]; t1 = a7c[10]; t2 = a7c[11]; t3 = a7c[12]; a3xw0 = true; a3xw1 = true; a3xw2 = true; a3xw3 = true; a3xw4 = true; a3xw5 = true; a3xw6 = true; a3xw10 = true; a3xw11 = true; a3xw12 = true; a3xw13 = true; }
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 1))));
    { const bool c = (int64_t)v0 != (int64_t)v1; (a1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 10000)))); if (c) goto L_1163724; }
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)s1, 20))));
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)v0, 12))));
    { const bool c = (int64_t)a0 == (int64_t)a1; (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 10001)))); if (c) goto L_1163720; }
    if ((int64_t)a0 != (int64_t)v0) { (v0 = (uint64_t)((uint64_t)s5 + (uint64_t)0ULL)); goto L_1163728; }
    { (a0 = (uint64_t)((uint64_t)s4 + (uint64_t)0ULL)); A3_CHARGE(); v0 = a1_sx64(GetLayoutParentObj(ram, rt, (uint32_t)a0, f12, f13, f14, f15, f16, f17, f18, f19)); }
    { const bool c = (int64_t)v0 == 0; (a0 = (uint64_t)((uint64_t)v0 + (uint64_t)0ULL)); if (c) goto L_1163724; }
    { A3_CHARGE(); uint64_t a7c[3] = {0u, 0u, a1}; v0 = GetLayoutClass_u11(ram, rt, a0, f0, f1, f2, f3, f4, f5, f6, f7, f8, f9, f10, f11, f12, f13, f14, f15, f16, f17, f18, f19, f20, f21, f22, f23, f24, f25, f26, f27, f28, f29, f30, f31, h2, a7c); a1 = a7c[2]; }
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)v0, 308))));
    vf5 = a4v::castsi128_ps(A4_R128(ADD32((uint32_t)v1, 0)));
    vf6 = a4v::castsi128_ps(A4_R128(ADD32((uint32_t)s2, 0)));
    { a4::vec4 res = a4v::sub(vf5, vf6); a4::q128 mask = a4v::set_epi32(0, -1, -1, -1); vf5 = a4v::blend(vf5, res, a4v::castsi128_ps(mask)); }
    a2::frSt<a4::q128>(a2fr, 0, a4v::castps_si128(vf5));
    { (a0 = (uint64_t)((uint64_t)s6 + (uint64_t)0ULL)); A3_CHARGE(); v0 = a1_sx64(get_camera_direction(ram, rt, (uint32_t)a0)); }
    (v1 = (uint64_t)((uint64_t)v0 + (uint64_t)0ULL));
    vf5 = a4v::castsi128_ps(a2::frLd<a4::q128>(a2fr, 0));
    vf6 = a4v::castsi128_ps(A4_R128(ADD32((uint32_t)v1, 0)));
    { a4::vec4 res = a4v::addK<1, 1>(vf0, a4v::bcast<3>(vf0)); a4::q128 mask = a4v::set_epi32(0, 0, 0, -1); vf7 = a4v::blendv_ps(vf7, res, a4v::castsi128_ps(mask)); }
    { a4::vec4 res = a4v::mul(vf5, vf6); a4::q128 mask = a4v::set_epi32(0, -1, -1, -1); vf6 = a4v::blend(vf6, res, a4v::castsi128_ps(mask)); }
    { a4::vec4 res = a4v::addK<1, 1>(vf0, a4v::bcast<0>(vf6)); vacc = a4v::dest<1, 0, 0, 0>(vacc, res); }
    { a4::vec4 mul_res = a4v::mulK<1, 1>(vf7, a4v::bcast<1>(vf6)); a4::vec4 res = a4v::addK<1, 1>(vacc, mul_res); vacc = a4v::dest<1, 0, 0, 0>(vacc, res); }
    { a4::vec4 mul_res = a4v::mulK<1, 1>(vf7, a4v::bcast<2>(vf6)); a4::vec4 res = a4v::addK<1, 1>(vacc, mul_res); a4::q128 mask = a4v::set_epi32(0, 0, 0, -1); vf6 = a4v::blendv_ps(vf6, res, a4v::castsi128_ps(mask)); }
    do { const a4::q128 _v = (a4v::castps_si128(vf6)); h2 = a4::hi64(_v); std::memcpy(&v0, &_v, 8); } while (0);
    { uint32_t bits = (uint32_t)v0; std::memcpy(&f1, &bits, sizeof(bits)); } a3xw1 = true;
    { uint32_t bits = 0u; std::memcpy(&f0, &bits, sizeof(bits)); } a3xw0 = true;
    fcr31 = (a1f::le(f1, f0)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
    { const bool c = (fcr31 & 0x800000u) == 0u; (v0 = (uint64_t)((uint64_t)s5 + (uint64_t)0ULL)); if (c) goto L_1163728; }
L_1163720:;
    (s5 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 1))));
L_1163724:;
    (v0 = (uint64_t)((uint64_t)s5 + (uint64_t)0ULL));
L_1163728:;
    { if (xo) { if (a3xw0) xo[0] = a1_bits(f0); if (a3xw1) xo[1] = a1_bits(f1); if (a3xw2) xo[2] = a1_bits(f2); if (a3xw3) xo[3] = a1_bits(f12); if (a3xw4) xo[4] = a1_bits(f13); if (a3xw5) xo[5] = a1_bits(f14); if (a3xw6) xo[6] = a1_bits(f20); xo[7] = a1; xo[8] = a2; xo[9] = a3; if (a3xw10) xo[10] = t0; if (a3xw11) xo[11] = t1; if (a3xw12) xo[12] = t2; if (a3xw13) xo[13] = t3; } io_vf1 = vf1; io_vf2 = vf2; io_vf3 = vf3; io_vf4 = vf4; io_vf5 = vf5; io_vf6 = vf6; io_vf7 = vf7; io_vf8 = vf8; io_vf9 = vf9; io_vf10 = vf10; io_vf11 = vf11; io_vf12 = vf12; io_vf13 = vf13; io_vf14 = vf14; io_vf15 = vf15; io_vf16 = vf16; io_vf17 = vf17; io_vf24 = vf24; io_vf25 = vf25; io_vf26 = vf26; io_vf27 = vf27; io_vacc = vacc; io_h2 = h2; io_h3 = h3; io_h6 = h6; io_h7 = h7; io_h8 = h8; io_h9 = h9; io_h10 = h10; io_h11 = h11; io_h12 = h12; io_h13 = h13; io_h14 = h14; io_h15 = h15; return v0; }
    __builtin_unreachable();
}

// SCRGetBossCollisionAttribute (0x1378838): typed by tools/typed_lift2.py (round A6; guest frame calls; VU0 in h5, out h2 h3, relocated calls 2)
inline uint64_t SCRGetBossCollisionAttribute(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint64_t v0, uint64_t a0, uint64_t sp, float f0, float f1, float f2, float f3, float f4, float f5, float f6, float f7, float f8, float f9, float f10, float f11, float f12, float f13, float f14, float f15, float f16, float f17, float f18, float f19, float f20, float f21, float f22, float f23, float f24, float f25, float f26, float f27, float f28, float f29, float f30, float f31, uint64_t &io_h2, uint64_t &io_h3, uint64_t io_h5)
{
    uint32_t a1 = 0;
    uint64_t h2 = io_h2;
    uint64_t h3 = io_h3;
    uint64_t h5 = io_h5;
    (sp = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)sp, 4294967264))));
    { A3_CHARGE(); v0 = GetDamageClass_u11(ram, rt, a0, f12, f13, f14, f15, f16, f17, f18, f19, h2, h5); }
    (a1 = (uint32_t)((int32_t)((int32_t)ADD32((uint32_t)v0, 16))));
    { (a0 = (uint64_t)((uint64_t)sp + (uint64_t)0ULL)); A3_CHARGE(); v0 = GetGeometryPointerSurfaceAttribute(ram, rt, a0, a1_sx64(a1), sp, f0, f1, f2, f3, f4, f5, f6, f7, f8, f9, f10, f11, f12, f13, f14, f15, f16, f17, f18, f19, f20, f21, f22, f23, f24, f25, f26, f27, f28, f29, f30, f31, h2, h3); }
    (v0 = (uint64_t)(A1_R64B((uint32_t)sp, 0)));
    { (sp = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)sp, 32)))); io_h2 = h2; io_h3 = h3; return v0; }
    __builtin_unreachable();
}

// gamecore_sub_01394580 (0x1394580): typed by tools/typed_lift2.py (round A6; guest frame calls, extra outputs xo[0]=f0 xo[1]=f13 xo[2]=f20 xo[3]=f21 xo[4]=a2 xo[5]=t0 xo[6]=t1 xo[7]=t2 xo[8]=t3; VU0 in vf0 vf6 vf7 vf10 vf12 vf13 vf14 vf15 vf16 vf17 vf18 vf25 vf28 vf29 vf31 vacc vstat h5 h9 h11 h29, out vf4 vf5 vf6 vf7 vf8 vf10 vf11 vf12 vf13 vf14 vf15 vf16 vf17 vf18 vf24 vf25 vf26 vf28 vf29 vf30 vf31 vacc vq vstat h2 h4 h5 h8 h9 h10 h11 h12 h13 h14 h15, relocated calls 5)
inline uint64_t gamecore_sub_01394580(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint64_t v0, uint64_t a0, uint64_t a1, uint64_t a2, uint64_t a3, uint64_t t0, uint64_t t1, uint64_t t2, uint64_t t3, uint64_t sp, float f0, float f1, float f2, float f3, float f4, float f5, float f6, float f7, float f8, float f9, float f10, float f11, float f12, float f13, float f14, float f15, float f16, float f17, float f18, float f19, float f20, float f21, float f22, float f23, float f24, float f25, float f26, float f27, float f28, float f29, float f30, float f31, const a4::vec4 &io_vf0, a4::vec4 &io_vf4, a4::vec4 &io_vf5, a4::vec4 &io_vf6, a4::vec4 &io_vf7, a4::vec4 &io_vf8, a4::vec4 &io_vf10, a4::vec4 &io_vf11, a4::vec4 &io_vf12, a4::vec4 &io_vf13, a4::vec4 &io_vf14, a4::vec4 &io_vf15, a4::vec4 &io_vf16, a4::vec4 &io_vf17, a4::vec4 &io_vf18, a4::vec4 &io_vf24, a4::vec4 &io_vf25, a4::vec4 &io_vf26, a4::vec4 &io_vf28, a4::vec4 &io_vf29, a4::vec4 &io_vf30, a4::vec4 &io_vf31, a4::vec4 &io_vacc, float &io_vq, uint16_t &io_vstat, uint64_t &io_h2, uint64_t &io_h4, uint64_t &io_h5, uint64_t &io_h8, uint64_t &io_h9, uint64_t &io_h10, uint64_t &io_h11, uint64_t &io_h12, uint64_t &io_h13, uint64_t &io_h14, uint64_t &io_h15, uint64_t io_h29, uint64_t *xo = nullptr)
{
    uint64_t at = 0;
    uint64_t v1 = 0;
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
    uint64_t fp = 0;
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
    a4::vec4 vf11 = io_vf11;
    a4::vec4 vf12 = io_vf12;
    a4::vec4 vf13 = io_vf13;
    a4::vec4 vf14 = io_vf14;
    a4::vec4 vf15 = io_vf15;
    a4::vec4 vf16 = io_vf16;
    a4::vec4 vf17 = io_vf17;
    a4::vec4 vf18 = io_vf18;
    a4::vec4 vf24 = io_vf24;
    a4::vec4 vf25 = io_vf25;
    a4::vec4 vf26 = io_vf26;
    a4::vec4 vf28 = io_vf28;
    a4::vec4 vf29 = io_vf29;
    a4::vec4 vf30 = io_vf30;
    a4::vec4 vf31 = io_vf31;
    a4::vec4 vacc = io_vacc;
    float vq = io_vq;
    uint16_t vstat = io_vstat;
    uint64_t h2 = io_h2;
    uint64_t h4 = io_h4;
    uint64_t h5 = io_h5;
    uint64_t h8 = io_h8;
    uint64_t h9 = io_h9;
    uint64_t h10 = io_h10;
    uint64_t h11 = io_h11;
    uint64_t h12 = io_h12;
    uint64_t h13 = io_h13;
    uint64_t h14 = io_h14;
    uint64_t h15 = io_h15;
    uint64_t h29 = io_h29;
    bool a3xw0 = false;
    bool a3xw1 = false;
    bool a3xw4 = false;
    bool a3xw5 = false;
    bool a3xw6 = false;
    bool a3xw7 = false;
    bool a3xw8 = false;
    (sp = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)sp, 4294967088))));
    (s0 = (uint64_t)((uint64_t)a0 + (uint64_t)0ULL));
    (s1 = (uint64_t)((uint64_t)t0 + (uint64_t)0ULL));
    (s3 = (uint64_t)((uint64_t)a2 + (uint64_t)0ULL));
    (s4 = (uint64_t)((uint64_t)a1 + (uint64_t)0ULL));
    (s5 = (uint64_t)((uint64_t)a3 + (uint64_t)0ULL));
    (fp = (uint64_t)((uint64_t)t2 + (uint64_t)0ULL));
    { float f = f21; uint32_t bits; std::memcpy(&bits, &f, sizeof(bits)); A1_W32B((uint32_t)sp, 200, bits); }
    f21 = ((float)(f12));
    { float f = f20; uint32_t bits; std::memcpy(&bits, &f, sizeof(bits)); A1_W32B((uint32_t)sp, 192, bits); }
    A1_W32B((uint32_t)sp, 96, (uint32_t)t1);
    { A1_W32B((uint32_t)sp, 100, (uint32_t)t3); A3_CHARGE(); v0 = GetSCRCharaAIWorkPtr(ram, rt, v0, a0, f12, f13, f14, f15, f16, f17, f18, f19, h2); }
    (a0 = (uint64_t)((uint64_t)s0 + (uint64_t)0ULL));
    { (s2 = (uint64_t)((uint64_t)v0 + (uint64_t)0ULL)); A3_CHARGE(); v0 = GetCharacterClass(ram, rt, a0, h2, h5); }
    (a0 = (uint64_t)((uint64_t)s0 + (uint64_t)0ULL));
    { (s6 = (uint64_t)((uint64_t)v0 + (uint64_t)0ULL)); A3_CHARGE(); v0 = GetLayoutClass_u11(ram, rt, a0, f0, f1, f2, f3, f4, f5, f6, f7, f8, f9, f10, f11, f12, f13, f14, f15, f16, f17, f18, f19, f20, f21, f22, f23, f24, f25, f26, f27, f28, f29, f30, f31, h2); }
    { const bool c = (int64_t)s1 == 0; (s7 = (uint64_t)((uint64_t)v0 + (uint64_t)0ULL)); if (c) goto L_1394610; }
    do { const a4::q128 _v = (A4_R128(ADD32((uint32_t)s3, 0))); h2 = a4::hi64(_v); std::memcpy(&v0, &_v, 8); } while (0);
    (s0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)s3, 16))));
    A4_W128(ADD32((uint32_t)sp, 0), a4::mk128(h2, v0));
    goto L_1394628;
L_1394610:;
    { const bool c = (int64_t)s5 == 0; (a0 = (uint64_t)((uint64_t)s4 + (uint64_t)0ULL)); if (c) goto L_13949c8; }
    do { const a4::q128 _v = (A4_R128(ADD32((uint32_t)s4, 64))); h2 = a4::hi64(_v); std::memcpy(&v0, &_v, 8); } while (0);
    { A4_W128(ADD32((uint32_t)sp, 0), a4::mk128(h2, v0)); A3_CHARGE(); uint64_t a7c[10] = {0u, 0u, 0u, 0u, a2, 0u, t0, t1, t2, t3}; v0 = GetGeometryPointerSurfacePlane_u11(ram, rt, a0, a2, t0, t1, t2, t3, (uint32_t)sp, f0, f1, f2, f3, f4, f5, f6, f7, f8, f9, f10, f11, f12, f13, f14, f15, f16, f17, f18, f19, f20, f21, f22, f23, f24, f25, f26, f27, f28, f29, f30, f31, vf0, vf4, vf5, vf6, vf7, vf8, vf10, vf14, vf15, vf16, vf17, vf18, vf29, vf30, vf31, vacc, vq, vstat, h2, h5, h9, h11, h29, a7c); a2 = a7c[4]; t0 = a7c[6]; t1 = a7c[7]; t2 = a7c[8]; t3 = a7c[9]; a3xw4 = true; a3xw5 = true; a3xw6 = true; a3xw7 = true; a3xw8 = true; }
    (s0 = (uint64_t)((uint64_t)v0 + (uint64_t)0ULL));
L_1394628:;
    { uint32_t bits = A1_R32B((uint32_t)s6, 1024); float f; std::memcpy(&f, &bits, sizeof(f)); f20 = f; }
    (a0 = (uint64_t)((uint64_t)s0 + (uint64_t)0ULL));
    (at = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)16997 << 16))));
    (at = (uint64_t)(RC_U64X(at) | (uint64_t)(uint16_t)12000));
    { uint32_t bits = (uint32_t)at; std::memcpy(&f0, &bits, sizeof(bits)); } a3xw0 = true;
    { f20 = a1f::mul(f20, f0); A3_CHARGE(); f0 = GetPlaneSlopeDegree(ram, rt, (uint32_t)a0, f13, f14, f15, f16, f17, f18, f19); a3xw0 = true; }
    f0 = std::fabs((float)(f0)); a3xw0 = true;
    fcr31 = (a1f::le(f20, f0)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
    { const bool c = (fcr31 & 0x800000u) == 0u; (v0 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL)); if (c) goto L_13949cc; }
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)s7, 308))));
    vf5 = a4v::castsi128_ps(A4_R128(ADD32((uint32_t)v1, 0)));
    vf6 = a4v::castsi128_ps(A4_R128(ADD32((uint32_t)sp, 0)));
    { a4::vec4 res = a4v::sub(vf5, vf6); a4::q128 mask = a4v::set_epi32(-1, -1, -1, -1); vf7 = a4v::blend(vf7, res, a4v::castsi128_ps(mask)); }
    { a4::vec4 res = a4v::mulK<1, 1>(vf7, vf7); a4::q128 mask = a4v::set_epi32(0, -1, -1, -1); vf7 = a4v::blend(vf7, res, a4v::castsi128_ps(mask)); }
    { a4::vec4 res = a4v::mulK<1, 1>(vf0, a4v::bcast<0>(vf7)); vacc = a4v::dest<0, 0, 0, 1>(vacc, res); }
    { a4::vec4 mul_res = a4v::mulK<1, 1>(vf0, a4v::bcast<1>(vf7)); a4::vec4 res = a4v::addK<1, 1>(vacc, mul_res); vacc = a4v::dest<0, 0, 0, 1>(vacc, res); }
    { a4::vec4 mul_res = a4v::mulK<1, 1>(vf0, a4v::bcast<2>(vf7)); a4::vec4 res = a4v::addK<1, 1>(vacc, mul_res); a4::q128 mask = a4v::set_epi32(-1, 0, 0, 0); vf7 = a4v::blendv_ps(vf7, res, a4v::castsi128_ps(mask)); }
    { float ft = a4v::cvtss_f32(a4v::shuffle_ps<0x03>(vf7, vf7)); vq = a4v::sqrt(vstat, ft); }
    { uint32_t bits; std::memcpy(&bits, &vq, sizeof(bits)); (v0 = (uint64_t)((int64_t)(int32_t)(bits))); }
    { uint32_t bits = (uint32_t)v0; std::memcpy(&f20, &bits, sizeof(bits)); }
    do { const a4::q128 _v = (A4_R128(ADD32((uint32_t)s0, 0))); h4 = a4::hi64(_v); std::memcpy(&a0, &_v, 8); } while (0);
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)sp, 16))));
    A4_W128(ADD32((uint32_t)sp, 16), a4::mk128(h4, a0));
    A1_W32B((uint32_t)sp, 20, 0u);
    vf5 = a4v::castsi128_ps(A4_R128(ADD32((uint32_t)v0, 0)));
    { a4::vec4 res = a4v::mul(vf5, vf5); a4::q128 mask = a4v::set_epi32(0, -1, -1, -1); vf7 = a4v::blend(vf7, res, a4v::castsi128_ps(mask)); }
    { a4::vec4 res = a4v::mulK<1, 1>(vf0, a4v::bcast<0>(vf7)); vacc = a4v::dest<0, 0, 0, 1>(vacc, res); }
    { a4::vec4 mul_res = a4v::mulK<1, 1>(vf0, a4v::bcast<1>(vf7)); a4::vec4 res = a4v::addK<1, 1>(vacc, mul_res); vacc = a4v::dest<0, 0, 0, 1>(vacc, res); }
    { a4::vec4 mul_res = a4v::mulK<1, 1>(vf0, a4v::bcast<2>(vf7)); a4::vec4 res = a4v::addK<1, 1>(vacc, mul_res); a4::q128 mask = a4v::set_epi32(-1, 0, 0, 0); vf7 = a4v::blendv_ps(vf7, res, a4v::castsi128_ps(mask)); }
    { float fs = a4v::cvtss_f32(a4v::shuffle_ps<0x03>(vf0, vf0)); float ft = a4v::cvtss_f32(a4v::shuffle_ps<0x03>(vf7, vf7)); vq = a4v::rsqrt(vstat, fs, ft); }
    { a4::vec4 res = a4v::mul(vf5, a4v::set1_ps(vq)); a4::q128 mask = a4v::set_epi32(0, -1, -1, -1); vf5 = a4v::blendv_ps(vf5, res, a4v::castsi128_ps(mask)); }
    A4_W128(ADD32((uint32_t)v0, 0), a4v::castps_si128(vf5));
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)s6, 1136))));
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)sp, 32))));
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
    A4_W128(ADD32((uint32_t)a0, 0), a4v::castps_si128(vf14));
    A4_W128(ADD32((uint32_t)a0, 16), a4v::castps_si128(vf15));
    A4_W128(ADD32((uint32_t)a0, 32), a4v::castps_si128(vf16));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)sp, 80))));
    A4_W128(ADD32((uint32_t)v0, 0), a4v::castps_si128(vf0));
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)sp, 32))));
    vf24 = a4v::castsi128_ps(A4_R128(ADD32((uint32_t)v1, 48)));
    { a4::vec4 res = a4v::subK<1, 1>(vf0, a4v::bcast<3>(vf0)); a4::q128 mask = a4v::set_epi32(0, 0, 0, -1); vf25 = a4v::blendv_ps(vf25, res, a4v::castsi128_ps(mask)); }
    { a4::vec4 res = a4v::mulK<0, 1>(vf24, a4v::bcast<0>(vf25)); a4::q128 mask = a4v::set_epi32(0, -1, -1, -1); vf28 = a4v::blendv_ps(vf28, res, a4v::castsi128_ps(mask)); }
    { a4::vec4 res = a4v::sub(vf28, vf28); a4::q128 mask = a4v::set_epi32(-1, 0, 0, 0); vf28 = a4v::blend(vf28, res, a4v::castsi128_ps(mask)); }
    do { const a4::q128 _v = (A4_R128(ADD32((uint32_t)v1, 0))); h8 = a4::hi64(_v); std::memcpy(&t0, &_v, 8); } while (0); a3xw5 = true;
    do { const a4::q128 _v = (A4_R128(ADD32((uint32_t)v1, 16))); h9 = a4::hi64(_v); std::memcpy(&t1, &_v, 8); } while (0); a3xw6 = true;
    do { const a4::q128 _v = (A4_R128(ADD32((uint32_t)v1, 32))); h10 = a4::hi64(_v); std::memcpy(&t2, &_v, 8); } while (0); a3xw7 = true;
    do { const a4::q128 _v = (a4v::castps_si128(vf0)); h11 = a4::hi64(_v); std::memcpy(&t3, &_v, 8); } while (0); a3xw8 = true;
    do { const a4::q128 _v = (a4m::pextlw(a4::mk128(h9, t1), a4::mk128(h8, t0))); h12 = a4::hi64(_v); std::memcpy(&t4, &_v, 8); } while (0);
    do { const a4::q128 _v = (a4m::pextuw(a4::mk128(h9, t1), a4::mk128(h8, t0))); h13 = a4::hi64(_v); std::memcpy(&t5, &_v, 8); } while (0);
    do { const a4::q128 _v = (a4m::pextlw(a4::mk128(h11, t3), a4::mk128(h10, t2))); h14 = a4::hi64(_v); std::memcpy(&t6, &_v, 8); } while (0);
    do { const a4::q128 _v = (a4m::pextuw(a4::mk128(h11, t3), a4::mk128(h10, t2))); h15 = a4::hi64(_v); std::memcpy(&t7, &_v, 8); } while (0);
    do { const a4::q128 _v = (a4m::pcpyld(a4::mk128(h14, t6), a4::mk128(h12, t4))); h8 = a4::hi64(_v); std::memcpy(&t0, &_v, 8); } while (0); a3xw5 = true;
    do { const a4::q128 _v = (a4v::unpackhi_epi64(a4::mk128(h12, t4), a4::mk128(h14, t6))); h9 = a4::hi64(_v); std::memcpy(&t1, &_v, 8); } while (0); a3xw6 = true;
    do { const a4::q128 _v = (a4m::pcpyld(a4::mk128(h15, t7), a4::mk128(h13, t5))); h10 = a4::hi64(_v); std::memcpy(&t2, &_v, 8); } while (0); a3xw7 = true;
    do { const a4::q128 _v = (a4v::unpackhi_epi64(a4::mk128(h13, t5), a4::mk128(h15, t7))); h11 = a4::hi64(_v); std::memcpy(&t3, &_v, 8); } while (0); a3xw8 = true;
    vf24 = a4v::castsi128_ps(a4::mk128(h8, t0));
    vf25 = a4v::castsi128_ps(a4::mk128(h9, t1));
    vf26 = a4v::castsi128_ps(a4::mk128(h10, t2));
    { a4::vec4 res = a4v::mulK<0, 1>(vf24, a4v::bcast<0>(vf28)); vacc = a4v::dest<1, 1, 1, 1>(vacc, res); }
    { a4::vec4 mul_res = a4v::mulK<0, 1>(vf25, a4v::bcast<1>(vf28)); a4::vec4 res = a4v::addK<1, 1>(vacc, mul_res); vacc = a4v::dest<1, 1, 1, 1>(vacc, res); }
    { a4::vec4 mul_res = a4v::mulK<0, 1>(vf26, a4v::bcast<2>(vf28)); a4::vec4 res = a4v::addK<1, 1>(vacc, mul_res); vacc = a4v::dest<1, 1, 1, 1>(vacc, res); }
    { a4::vec4 mul_res = a4v::mulK<1, 1>(vf0, a4v::bcast<3>(vf28)); a4::vec4 res = a4v::addK<1, 1>(vacc, mul_res); a4::q128 mask = a4v::set_epi32(-1, -1, -1, -1); vf29 = a4v::blendv_ps(vf29, res, a4v::castsi128_ps(mask)); }
    { a4::vec4 res = a4v::subK<1, 1>(vf29, vf29); a4::q128 mask = a4v::set_epi32(-1, 0, 0, 0); vf29 = a4v::blend(vf29, res, a4v::castsi128_ps(mask)); }
    { a4::vec4 res = a4v::addK<1, 1>(vf29, vf0); a4::q128 mask = a4v::set_epi32(-1, 0, 0, 0); vf29 = a4v::blend(vf29, res, a4v::castsi128_ps(mask)); }
    A4_W128(ADD32((uint32_t)v1, 0), a4v::castps_si128(vf24));
    A4_W128(ADD32((uint32_t)v1, 16), a4v::castps_si128(vf25));
    A4_W128(ADD32((uint32_t)v1, 32), a4v::castps_si128(vf26));
    A4_W128(ADD32((uint32_t)v1, 48), a4v::castps_si128(vf29));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)sp, 16))));
    vf8 = a4v::castsi128_ps(A4_R128(ADD32((uint32_t)v0, 0)));
    vf14 = a4v::castsi128_ps(A4_R128(ADD32((uint32_t)v1, 0)));
    vf15 = a4v::castsi128_ps(A4_R128(ADD32((uint32_t)v1, 16)));
    vf16 = a4v::castsi128_ps(A4_R128(ADD32((uint32_t)v1, 32)));
    vf17 = a4v::castsi128_ps(A4_R128(ADD32((uint32_t)v1, 48)));
    { a4::vec4 res = a4v::mul(vf14, a4v::bcast<0>(vf8)); vacc = a4v::dest<1, 1, 1, 1>(vacc, res); }
    { a4::vec4 mul_res = a4v::mul(vf15, a4v::bcast<1>(vf8)); a4::vec4 res = a4v::addK<1, 1>(vacc, mul_res); vacc = a4v::dest<1, 1, 1, 1>(vacc, res); }
    { a4::vec4 mul_res = a4v::mul(vf16, a4v::bcast<2>(vf8)); a4::vec4 res = a4v::addK<1, 1>(vacc, mul_res); vacc = a4v::dest<1, 1, 1, 1>(vacc, res); }
    { a4::vec4 mul_res = a4v::mul(vf17, a4v::bcast<3>(vf8)); a4::vec4 res = a4v::addK<1, 1>(vacc, mul_res); a4::q128 mask = a4v::set_epi32(-1, -1, -1, -1); vf10 = a4v::blendv_ps(vf10, res, a4v::castsi128_ps(mask)); }
    A4_W128(ADD32((uint32_t)v0, 0), a4v::castps_si128(vf10));
    { uint32_t bits = A1_R32B((uint32_t)sp, 16); float f; std::memcpy(&f, &bits, sizeof(f)); f12 = f; }
    { { uint32_t bits = A1_R32B((uint32_t)sp, 24); float f; std::memcpy(&f, &bits, sizeof(f)); f13 = f; } a3xw1 = true; A3_CHARGE(); f0 = a9x::atan2Exact(ram, rt, f12, f13); a3xw0 = true; }
    fcr31 = (a1f::lt(f20, f21)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)sp, 208))));
    f3 = ((float)(f0));
    { float f = f20; uint32_t bits; std::memcpy(&bits, &f, sizeof(bits)); A1_W32B((uint32_t)v0, 0, bits); }
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)sp, 216))));
    { const bool c = (fcr31 & 0x800000u) == 0u; { float f = f3; uint32_t bits; std::memcpy(&bits, &f, sizeof(bits)); A1_W32B((uint32_t)v1, 0, bits); } if (c) goto L_13949c8; }
    A1_W32B((uint32_t)s2, 16868, 0u);
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)sp, 100))));
    { const bool c = (int64_t)v0 == 0; (t3 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v0, 4)))); a3xw8 = true; if (c) goto L_139487c; }
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)3 << 16))));
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)t3, 0))));
    (v0 = (uint64_t)(RC_U64X(v0) | (uint64_t)(uint16_t)65535));
    (v0 = (uint64_t)(RC_U64X(a0) & RC_U64X(v0)));
    { const bool c = (int64_t)v0 == 0; (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)SUB32((uint32_t)t3, (uint32_t)v0)))); if (c) goto L_1394830; }
    (t3 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)v0, 0)))); a3xw8 = true;
    goto L_1394834;
L_1394830:;
    (t3 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL)); a3xw8 = true;
L_1394834:;
    { const bool c = (int64_t)t3 == 0; { const uint32_t _dynInsn = A1_R32(0x1394838u); const int32_t _dynSimm = (int16_t)(_dynInsn & 0xFFFFu); const uint32_t _dynImm = _dynInsn & 0xFFFFu; (void)_dynSimm; (void)_dynImm; (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)_dynImm << 16)))); } if (c) goto L_1394868; }
    { const uint32_t _dynInsn = A1_R32(0x139483Cu); const int32_t _dynSimm = (int16_t)(_dynInsn & 0xFFFFu); const uint32_t _dynImm = _dynInsn & 0xFFFFu; (void)_dynSimm; (void)_dynImm; (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32(ADD32((uint32_t)v0, _dynSimm))))); }
    { const bool c = (int64_t)t3 == (int64_t)v1; (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)SRL32((uint32_t)a0, 18)))); if (c) goto L_1394868; }
    (a1 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)t3, 8))));
    (v0 = (uint64_t)(RC_U64X(v0) & (uint64_t)(uint16_t)4095));
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)t3, 16))));
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)a1, 12))));
    { int64_t result = (int64_t)(int32_t)v0 * (int64_t)(int32_t)v1; lo = (uint64_t)(int64_t)(int32_t)result; hi = (uint64_t)(int64_t)(int32_t)(result >> 32); (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)result))); }
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)a0, (uint32_t)v0))));
    goto L_139486c;
L_1394868:;
    (a0 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL));
L_139486c:;
    { const uint32_t _dynInsn = A1_R32(0x139486Cu); const int32_t _dynSimm = (int16_t)(_dynInsn & 0xFFFFu); const uint32_t _dynImm = _dynInsn & 0xFFFFu; (void)_dynSimm; (void)_dynImm; (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)_dynImm << 16)))); }
    { const uint32_t _dynInsn = A1_R32(0x1394870u); const int32_t _dynSimm = (int16_t)(_dynInsn & 0xFFFFu); const uint32_t _dynImm = _dynInsn & 0xFFFFu; (void)_dynSimm; (void)_dynImm; (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32(ADD32((uint32_t)v0, _dynSimm))))); }
    if ((int64_t)a0 == (int64_t)v1) goto L_139489c;
L_139487c:;
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)sp, 96))));
    { uint32_t bits = A1_R32B((uint32_t)v0, 8); float f; std::memcpy(&f, &bits, sizeof(f)); f2 = f; }
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)s2, 16836))));
    f1 = std::fabs((float)(f2));
    { uint32_t bits = A1_R32B((uint32_t)v0, 0); float f; std::memcpy(&f, &bits, sizeof(f)); f0 = f; } a3xw0 = true;
    fcr31 = (a1f::le(f1, f0)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
    if ((fcr31 & 0x800000u) == 0u) goto L_1394930;
L_139489c:;
    { uint32_t bits = 0u; std::memcpy(&f0, &bits, sizeof(bits)); } a3xw0 = true;
    fcr31 = (a1f::le(f0, f3)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
    { const bool c = (fcr31 & 0x800000u) == 0u; (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)s2, 16864)))); if (c) goto L_13948d8; }
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 2))));
    { const bool c = (int64_t)v1 != (int64_t)v0; (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 1)))); if (c) goto L_1394918; }
    { uint32_t bits = A1_R32B((uint32_t)s2, 16872); float f; std::memcpy(&f, &bits, sizeof(f)); f1 = f; }
    (at = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)16384 << 16))));
    { uint32_t bits = (uint32_t)at; std::memcpy(&f0, &bits, sizeof(bits)); } a3xw0 = true;
    fcr31 = (a1f::lt(f1, f0)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
    if ((fcr31 & 0x800000u) != 0u) { A1_W32B((uint32_t)s2, 16868, (uint32_t)v1); goto L_1394900; }
    A1_W32B((uint32_t)s2, 16872, 0u);
    goto L_139491c;
L_13948d8:;
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 1))));
    { const bool c = (int64_t)v1 != (int64_t)v0; (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 2)))); if (c) goto L_1394918; }
    { uint32_t bits = A1_R32B((uint32_t)s2, 16872); float f; std::memcpy(&f, &bits, sizeof(f)); f1 = f; }
    (at = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)16384 << 16))));
    { uint32_t bits = (uint32_t)at; std::memcpy(&f0, &bits, sizeof(bits)); } a3xw0 = true;
    fcr31 = (a1f::lt(f1, f0)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
    if ((fcr31 & 0x800000u) == 0u) { A1_W32B((uint32_t)s2, 16872, 0u); goto L_139491c; }
    A1_W32B((uint32_t)s2, 16868, (uint32_t)v1);
L_1394900:;
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)326 << 16))));
    { uint32_t bits = A1_R32B((uint32_t)v0, 22480); float f; std::memcpy(&f, &bits, sizeof(f)); f0 = f; } a3xw0 = true;
    f0 = a1f::add(f1, f0); a3xw0 = true;
    { float f = f0; uint32_t bits; std::memcpy(&bits, &f, sizeof(bits)); A1_W32B((uint32_t)s2, 16872, bits); }
    goto L_1394920;
L_1394918:;
    A1_W32B((uint32_t)s2, 16872, 0u);
L_139491c:;
    A1_W32B((uint32_t)s2, 16868, (uint32_t)v0);
L_1394920:;
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)sp, 96))));
    A1_W32B((uint32_t)v0, 8, 0u);
    goto L_139494c;
L_1394930:;
    { uint32_t bits = 0u; std::memcpy(&f0, &bits, sizeof(bits)); } a3xw0 = true;
    fcr31 = (a1f::lt(f0, f2)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
    if ((fcr31 & 0x800000u) == 0u) { (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 2)))); goto L_1394944; }
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 1))));
L_1394944:;
    { float f = f0; uint32_t bits; std::memcpy(&bits, &f, sizeof(bits)); A1_W32B((uint32_t)s2, 16872, bits); }
    A1_W32B((uint32_t)s2, 16868, (uint32_t)v0);
L_139494c:;
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)s2, 16868))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 1))));
    if ((int64_t)v1 != (int64_t)v0) { { const uint32_t _dynInsn = A1_R32(0x1394958u); const int32_t _dynSimm = (int16_t)(_dynInsn & 0xFFFFu); const uint32_t _dynImm = _dynInsn & 0xFFFFu; (void)_dynSimm; (void)_dynImm; (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)_dynImm << 16)))); } goto L_1394988; }
    { const uint32_t _dynInsn = A1_R32(0x139495Cu); const int32_t _dynSimm = (int16_t)(_dynInsn & 0xFFFFu); const uint32_t _dynImm = _dynInsn & 0xFFFFu; (void)_dynSimm; (void)_dynImm; (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)_dynImm << 16)))); }
    (a1 = (uint64_t)(A1_R64B((uint32_t)fp, 448)));
    { const uint32_t _dynInsn = A1_R32(0x1394964u); const int32_t _dynSimm = (int16_t)(_dynInsn & 0xFFFFu); const uint32_t _dynImm = _dynInsn & 0xFFFFu; (void)_dynSimm; (void)_dynImm; (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32(ADD32((uint32_t)v0, _dynSimm))))); }
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 1))));
    { const uint32_t _dynInsn = A1_R32(0x139496Cu); const int32_t _dynSimm = (int16_t)(_dynInsn & 0xFFFFu); const uint32_t _dynImm = _dynInsn & 0xFFFFu; (void)_dynSimm; (void)_dynImm; (a2 = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)_dynImm << 16)))); } a3xw4 = true;
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)v1, 4))));
    (v0 = (uint64_t)(RC_U64X(a0) << ((uint32_t)v0 & 0x3F)));
    (a1 = (uint64_t)(a1 | v0));
    A1_W64B((uint32_t)fp, 448, a1);
    { const uint32_t _dynInsn = A1_R32(0x1394984u); const int32_t _dynSimm = (int16_t)(_dynInsn & 0xFFFFu); const uint32_t _dynImm = _dynInsn & 0xFFFFu; (void)_dynSimm; (void)_dynImm; (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32(ADD32((uint32_t)a2, _dynSimm))))); }
    goto L_13949ac;
L_1394988:;
    (a1 = (uint64_t)(A1_R64B((uint32_t)fp, 448)));
    { const uint32_t _dynInsn = A1_R32(0x139498Cu); const int32_t _dynSimm = (int16_t)(_dynInsn & 0xFFFFu); const uint32_t _dynImm = _dynInsn & 0xFFFFu; (void)_dynSimm; (void)_dynImm; (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32(ADD32((uint32_t)v0, _dynSimm))))); }
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 1))));
    { const uint32_t _dynInsn = A1_R32(0x1394994u); const int32_t _dynSimm = (int16_t)(_dynInsn & 0xFFFFu); const uint32_t _dynImm = _dynInsn & 0xFFFFu; (void)_dynSimm; (void)_dynImm; (a2 = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)_dynImm << 16)))); } a3xw4 = true;
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)v1, 4))));
    (v0 = (uint64_t)(RC_U64X(a0) << ((uint32_t)v0 & 0x3F)));
    (a1 = (uint64_t)(a1 | v0));
    A1_W64B((uint32_t)fp, 448, a1);
    { const uint32_t _dynInsn = A1_R32(0x13949A8u); const int32_t _dynSimm = (int16_t)(_dynInsn & 0xFFFFu); const uint32_t _dynImm = _dynInsn & 0xFFFFu; (void)_dynSimm; (void)_dynImm; (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32(ADD32((uint32_t)a2, _dynSimm))))); }
L_13949ac:;
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)v0, 4))));
    (a0 = (uint64_t)(RC_U64X(a0) << ((uint32_t)v1 & 0x3F)));
    (a0 = (uint64_t)(~(0ULL | a0)));
    (a1 = (uint64_t)(a1 & a0));
    A1_W64B((uint32_t)fp, 448, a1);
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 1))));
    goto L_13949cc;
L_13949c8:;
    (v0 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL));
L_13949cc:;
    { uint32_t bits = A1_R32B((uint32_t)sp, 200); float f; std::memcpy(&f, &bits, sizeof(f)); f21 = f; }
    { uint32_t bits = A1_R32B((uint32_t)sp, 192); float f; std::memcpy(&f, &bits, sizeof(f)); f20 = f; }
    { (sp = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)sp, 208)))); if (xo) { if (a3xw0) xo[0] = a1_bits(f0); if (a3xw1) xo[1] = a1_bits(f13); xo[2] = a1_bits(f20); xo[3] = a1_bits(f21); if (a3xw4) xo[4] = a2; if (a3xw5) xo[5] = t0; if (a3xw6) xo[6] = t1; if (a3xw7) xo[7] = t2; if (a3xw8) xo[8] = t3; } io_vf4 = vf4; io_vf5 = vf5; io_vf6 = vf6; io_vf7 = vf7; io_vf8 = vf8; io_vf10 = vf10; io_vf11 = vf11; io_vf12 = vf12; io_vf13 = vf13; io_vf14 = vf14; io_vf15 = vf15; io_vf16 = vf16; io_vf17 = vf17; io_vf18 = vf18; io_vf24 = vf24; io_vf25 = vf25; io_vf26 = vf26; io_vf28 = vf28; io_vf29 = vf29; io_vf30 = vf30; io_vf31 = vf31; io_vacc = vacc; io_vq = vq; io_vstat = vstat; io_h2 = h2; io_h4 = h4; io_h5 = h5; io_h8 = h8; io_h9 = h9; io_h10 = h10; io_h11 = h11; io_h12 = h12; io_h13 = h13; io_h14 = h14; io_h15 = h15; return v0; }
    __builtin_unreachable();
}

// nico_calcWormGenealogicalNodeTailBase (0x14639a8): typed by tools/typed_lift2.py (round A6; guest frame calls; VU0 in vf0 vf7 vf10 vf12 vf13 vf14 vf15 vf16 vf17 vf25 vf28 vf29 vacc vstat h3 h4 h6 h8, out vf4 vf5 vf6 vf7 vf8 vf10 vf11 vf12 vf13 vf14 vf15 vf16 vf17 vf24 vf25 vf26 vf27 vf28 vf29 vacc vq vstat h2 h3 h4 h7 h8 h9 h10 h11 h12 h13 h14 h15, relocated calls 2, 64-bit inputs t1)
inline uint64_t nico_calcWormGenealogicalNodeTailBase(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint64_t v0, uint64_t a0, uint64_t w64_t1, uint64_t sp, float f0, float f1, float f2, float f3, float f4, float f5, float f6, float f7, float f8, float f9, float f10, float f11, float f12, float f13, float f14, float f15, float f16, float f17, float f18, float f19, float f20, float f21, float f22, float f23, float f24, float f25, float f26, float f27, float f28, float f29, float f30, float f31, const a4::vec4 &io_vf0, a4::vec4 &io_vf4, a4::vec4 &io_vf5, a4::vec4 &io_vf6, a4::vec4 &io_vf7, a4::vec4 &io_vf8, a4::vec4 &io_vf10, a4::vec4 &io_vf11, a4::vec4 &io_vf12, a4::vec4 &io_vf13, a4::vec4 &io_vf14, a4::vec4 &io_vf15, a4::vec4 &io_vf16, a4::vec4 &io_vf17, a4::vec4 &io_vf24, a4::vec4 &io_vf25, a4::vec4 &io_vf26, a4::vec4 &io_vf27, a4::vec4 &io_vf28, a4::vec4 &io_vf29, a4::vec4 &io_vacc, float &io_vq, uint16_t &io_vstat, uint64_t &io_h2, uint64_t &io_h3, uint64_t &io_h4, uint64_t io_h6, uint64_t &io_h7, uint64_t &io_h8, uint64_t &io_h9, uint64_t &io_h10, uint64_t &io_h11, uint64_t &io_h12, uint64_t &io_h13, uint64_t &io_h14, uint64_t &io_h15)
{
    uint32_t at = 0;
    uint64_t v1 = 0;
    uint64_t a1 = 0;
    uint64_t a2 = 0;
    uint64_t a3 = 0;
    uint32_t t0 = 0;
    uint32_t t1 = (uint32_t)w64_t1;
    uint32_t t2 = 0;
    uint32_t t3 = 0;
    uint64_t s0 = 0;
    uint64_t s1 = 0;
    uint64_t s2 = 0;
    uint32_t s3 = 0;
    uint64_t s4 = 0;
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
    a4::vec4 vf24 = io_vf24;
    a4::vec4 vf25 = io_vf25;
    a4::vec4 vf26 = io_vf26;
    a4::vec4 vf27 = io_vf27;
    a4::vec4 vf28 = io_vf28;
    a4::vec4 vf29 = io_vf29;
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
    uint64_t h10 = io_h10;
    uint64_t h11 = io_h11;
    uint64_t h12 = io_h12;
    uint64_t h13 = io_h13;
    uint64_t h14 = io_h14;
    uint64_t h15 = io_h15;
    (sp = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)sp, 4294967152))));
    (s1 = (uint64_t)((uint64_t)a0 + (uint64_t)0ULL));
    (s0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)s1, 0))));
    { (a0 = (uint64_t)((uint64_t)RC_U64X(s0) + (uint64_t)0ULL)); A3_CHARGE(); v0 = GetSCRCharaAIWorkPtr(ram, rt, v0, a0, f12, f13, f14, f15, f16, f17, f18, f19, h2); }
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 4294967280))));
    (a0 = (uint64_t)((uint64_t)s0 + (uint64_t)0ULL));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v0, 16819))));
    { (s2 = (uint64_t)(RC_U64X(v0) & RC_U64X(v1))); A3_CHARGE(); v0 = GetLayoutClass_u11(ram, rt, a0, f0, f1, f2, f3, f4, f5, f6, f7, f8, f9, f10, f11, f12, f13, f14, f15, f16, f17, f18, f19, f20, f21, f22, f23, f24, f25, f26, f27, f28, f29, f30, f31, h2); }
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)326 << 16))));
    (s4 = (uint64_t)((uint64_t)v0 + (uint64_t)0ULL));
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)v1, 29704))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)327 << 16))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v0, 4294953920))));
    (a3 = (uint64_t)((uint64_t)s2 + (uint64_t)0ULL));
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)SLL32((uint32_t)a0, 2))));
    (a2 = (uint64_t)(A1_R64B((uint32_t)v0, 8)));
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v1, (uint32_t)a0))));
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)SLL32((uint32_t)a0, 4))));
    (a1 = (uint64_t)(A1_R64B((uint32_t)v0, 0)));
    (v0 = (uint64_t)((uint64_t)sp + (uint64_t)0ULL));
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)SLL32((uint32_t)v1, 4))));
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)a0, (uint32_t)s2))));
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v1, (uint32_t)s2))));
    A1_W64B((uint32_t)sp, 64, a1);
    A1_W64B((uint32_t)sp, 72, a2);
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)a0, 2592))));
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v1, 16))));
    { a4::vec4 res = a4v::addK<1, 1>(vf0, a4v::bcast<3>(vf0)); a4::q128 mask = a4v::set_epi32(0, -1, -1, -1); vf12 = a4v::blendv_ps(vf12, res, a4v::castsi128_ps(mask)); }
    vf11 = a4v::castsi128_ps(A4_R128(ADD32((uint32_t)a0, 0)));
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
    A4_W128(ADD32((uint32_t)v0, 0), a4v::castps_si128(vf14));
    A4_W128(ADD32((uint32_t)v0, 16), a4v::castps_si128(vf15));
    A4_W128(ADD32((uint32_t)v0, 32), a4v::castps_si128(vf16));
    vf4 = a4v::castsi128_ps(A4_R128(ADD32((uint32_t)v1, 0)));
    { a4::q128 mask = a4v::set_epi32(-1, 0, 0, 0); vf4 = a4v::blendv_ps(vf4, vf0, a4v::castsi128_ps(mask)); }
    A4_W128(ADD32((uint32_t)v0, 48), a4v::castps_si128(vf4));
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)326 << 16))));
    (a1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)sp, 64))));
    (s3 = (uint32_t)((int32_t)((int32_t)ADD32((uint32_t)a0, 29704))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B(s3, 0))));
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)SLL32((uint32_t)v0, 2))));
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v1, (uint32_t)v0))));
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)SLL32((uint32_t)v1, 4))));
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v1, (uint32_t)a3))));
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v1, 96))));
    vf8 = a4v::castsi128_ps(A4_R128(ADD32((uint32_t)a1, 0)));
    vf14 = a4v::castsi128_ps(A4_R128(ADD32((uint32_t)sp, 0)));
    vf15 = a4v::castsi128_ps(A4_R128(ADD32((uint32_t)sp, 16)));
    vf16 = a4v::castsi128_ps(A4_R128(ADD32((uint32_t)sp, 32)));
    vf17 = a4v::castsi128_ps(A4_R128(ADD32((uint32_t)sp, 48)));
    { a4::vec4 res = a4v::mul(vf14, a4v::bcast<0>(vf8)); vacc = a4v::dest<1, 1, 1, 1>(vacc, res); }
    { a4::vec4 mul_res = a4v::mul(vf15, a4v::bcast<1>(vf8)); a4::vec4 res = a4v::addK<1, 1>(vacc, mul_res); vacc = a4v::dest<1, 1, 1, 1>(vacc, res); }
    { a4::vec4 mul_res = a4v::mul(vf16, a4v::bcast<2>(vf8)); a4::vec4 res = a4v::addK<1, 1>(vacc, mul_res); vacc = a4v::dest<1, 1, 1, 1>(vacc, res); }
    { a4::vec4 mul_res = a4v::mul(vf17, a4v::bcast<3>(vf8)); a4::vec4 res = a4v::addK<1, 1>(vacc, mul_res); a4::q128 mask = a4v::set_epi32(-1, -1, -1, -1); vf10 = a4v::blendv_ps(vf10, res, a4v::castsi128_ps(mask)); }
    A4_W128(ADD32((uint32_t)v1, 0), a4v::castps_si128(vf10));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B(s3, 0))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)SLL32((uint32_t)v0, 4))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v0, (uint32_t)s2))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v0, 2608))));
    A4_W128(ADD32((uint32_t)v0, 0), a4v::castps_si128(vf0));
    { (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)s1, 0)))); A3_CHARGE(); v0 = GetSCRCharaAIWorkPtr(ram, rt, v0, a0, f12, f13, f14, f15, f16, f17, f18, f19, h2); }
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 4294967280))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v0, 16819))));
    (a0 = (uint64_t)((uint64_t)s1 + (uint64_t)0ULL));
    { (s0 = (uint64_t)(RC_U64X(v0) & RC_U64X(v1))); A3_CHARGE(); v0 = nico_calcWormGenealogicalNodeNextBase(ram, rt, v0, a0, w64_t1, sp, f0, f1, f2, f3, f4, f5, f6, f7, f8, f9, f10, f11, f12, f13, f14, f15, f16, f17, f18, f19, f20, f21, f22, f23, f24, f25, f26, f27, f28, f29, f30, f31, vf0, vf4, vf5, vf6, vf7, vf8, vf10, vf11, vf12, vf13, vf14, vf15, vf16, vf17, vf24, vf25, vf26, vf27, vf28, vf29, vacc, vq, vstat, h2, h3, h4, h6, h8, h9, h10, h11, h12, h13, h14, h15); }
    (v1 = (uint64_t)((int64_t)(int32_t)((int8_t)A1_R8B((uint32_t)s0, 4763))));
    { const uint32_t _dynInsn = A1_R32(0x1463B2Cu); const int32_t _dynSimm = (int16_t)(_dynInsn & 0xFFFFu); const uint32_t _dynImm = _dynInsn & 0xFFFFu; (void)_dynSimm; (void)_dynImm; (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)_dynImm << 16)))); }
    (t3 = (uint32_t)((int32_t)((int32_t)ADD32((uint32_t)s2, 96))));
    (t1 = (uint32_t)((int32_t)((int32_t)ADD32((uint32_t)sp, 80))));
    (t2 = (uint32_t)((int32_t)((int32_t)ADD32((uint32_t)s2, 16))));
    { const bool c = (int64_t)v1 == 0; { const uint32_t _dynInsn = A1_R32(0x1463B40u); const int32_t _dynSimm = (int16_t)(_dynInsn & 0xFFFFu); const uint32_t _dynImm = _dynInsn & 0xFFFFu; (void)_dynSimm; (void)_dynImm; (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v0, _dynSimm)))); } if (c) goto L_1463b88; }
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B(s3, 0))));
    do { const a4::q128 _v = (A4_R128(ADD32((uint32_t)v0, 0))); h7 = a4::hi64(_v); std::memcpy(&a3, &_v, 8); } while (0);
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)SLL32((uint32_t)a0, 2))));
    (t0 = (uint32_t)((int32_t)((int32_t)ADD32((uint32_t)a0, (uint32_t)s0))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v0, (uint32_t)a0))));
    (a1 = (uint64_t)((int64_t)(int32_t)((int32_t)SLL32((uint32_t)a0, 4))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)SLL32((uint32_t)v0, 4))));
    (a2 = (uint64_t)((uint64_t)a1_sx64(t0) + (uint64_t)0ULL));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v0, (uint32_t)s0))));
    (a1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)a1, (uint32_t)s0))));
    do { const a4::q128 _v = (A4_R128(ADD32((uint32_t)v0, 16))); h3 = a4::hi64(_v); std::memcpy(&v1, &_v, 8); } while (0);
    (a0 = (uint64_t)((uint64_t)RC_U64X(a2) + (uint64_t)0ULL));
    A4_W128(ADD32((uint32_t)v0, 32), a4::mk128(h7, a3));
    A4_W128(ADD32((uint32_t)a1, 3104), a4::mk128(h3, v1));
    A1_W8B((uint32_t)a2, 3744, (uint8_t)0u);
    A1_W8B((uint32_t)a0, 3776, (uint8_t)0u);
    A1_W8B(t0, 3808, (uint8_t)0u);
L_1463b88:;
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B(s3, 0))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v0, 1))));
    A1_W32B(s3, 0, (uint32_t)v0);
    (v1 = (uint64_t)((int64_t)(int32_t)((int8_t)A1_R8B((uint32_t)s2, 4760))));
    if ((int64_t)v1 != 0) { (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)s4, 316)))); goto L_1463bf8; }
    vf5 = a4v::castsi128_ps(A4_R128(ADD32(t2, 0)));
    vf6 = a4v::castsi128_ps(A4_R128(ADD32(t3, 0)));
    { a4::vec4 res = a4v::sub(vf5, vf6); a4::q128 mask = a4v::set_epi32(-1, -1, -1, -1); vf7 = a4v::blend(vf7, res, a4v::castsi128_ps(mask)); }
    A4_W128(ADD32(t1, 0), a4v::castps_si128(vf7));
    vf5 = a4v::castsi128_ps(A4_R128(ADD32(t1, 0)));
    { a4::vec4 res = a4v::mul(vf5, vf5); a4::q128 mask = a4v::set_epi32(0, -1, -1, -1); vf7 = a4v::blend(vf7, res, a4v::castsi128_ps(mask)); }
    { a4::vec4 res = a4v::mulK<1, 1>(vf0, a4v::bcast<0>(vf7)); vacc = a4v::dest<0, 0, 0, 1>(vacc, res); }
    { a4::vec4 mul_res = a4v::mulK<1, 1>(vf0, a4v::bcast<1>(vf7)); a4::vec4 res = a4v::addK<1, 1>(vacc, mul_res); vacc = a4v::dest<0, 0, 0, 1>(vacc, res); }
    { a4::vec4 mul_res = a4v::mulK<1, 1>(vf0, a4v::bcast<2>(vf7)); a4::vec4 res = a4v::addK<1, 1>(vacc, mul_res); a4::q128 mask = a4v::set_epi32(-1, 0, 0, 0); vf7 = a4v::blendv_ps(vf7, res, a4v::castsi128_ps(mask)); }
    { float fs = a4v::cvtss_f32(a4v::shuffle_ps<0x03>(vf0, vf0)); float ft = a4v::cvtss_f32(a4v::shuffle_ps<0x03>(vf7, vf7)); vq = a4v::rsqrt(vstat, fs, ft); }
    { a4::vec4 res = a4v::mul(vf5, a4v::set1_ps(vq)); a4::q128 mask = a4v::set_epi32(0, -1, -1, -1); vf5 = a4v::blendv_ps(vf5, res, a4v::castsi128_ps(mask)); }
    A4_W128(ADD32(t1, 0), a4v::castps_si128(vf5));
    (at = (uint32_t)((int32_t)((int32_t)((uint32_t)16256 << 16))));
    { uint32_t bits = a7_opq(at); std::memcpy(&f0, &bits, sizeof(bits)); }
    { const uint32_t _dynInsn = A1_R32(0x1463BDCu); const int32_t _dynSimm = (int16_t)(_dynInsn & 0xFFFFu); const uint32_t _dynImm = _dynInsn & 0xFFFFu; (void)_dynSimm; (void)_dynImm; (a2 = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)_dynImm << 16)))); }
    (a1 = (uint64_t)((uint64_t)a1_sx64(t1) + (uint64_t)0ULL));
    { const uint32_t _dynInsn = A1_R32(0x1463BE4u); const int32_t _dynSimm = (int16_t)(_dynInsn & 0xFFFFu); const uint32_t _dynImm = _dynInsn & 0xFFFFu; (void)_dynSimm; (void)_dynImm; (a2 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)a2, _dynSimm)))); }
    { float f = f0; uint32_t bits; std::memcpy(&bits, &f, sizeof(bits)); A1_W32B((uint32_t)sp, 92, bits); }
    { (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)s4, 316)))); A3_CHARGE(); f0 = a9x::axisDirZY(ram, rt, (uint32_t)a0, (uint32_t)a1, (uint32_t)a2); }
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)s4, 316))));
L_1463bf8:;
    vf14 = a4v::castsi128_ps(A4_R128(ADD32((uint32_t)v0, 0)));
    { a4::vec4 res = a4v::mul(vf14, vf14); a4::q128 mask = a4v::set_epi32(-1, -1, -1, -1); vf15 = a4v::blend(vf15, res, a4v::castsi128_ps(mask)); }
    { a4::vec4 res = a4v::addK<1, 1>(vf15, a4v::bcast<1>(vf15)); a4::q128 mask = a4v::set_epi32(0, 0, 0, -1); vf15 = a4v::blendv_ps(vf15, res, a4v::castsi128_ps(mask)); }
    { a4::vec4 res = a4v::addK<1, 1>(vf15, a4v::bcast<2>(vf15)); a4::q128 mask = a4v::set_epi32(0, 0, 0, -1); vf15 = a4v::blendv_ps(vf15, res, a4v::castsi128_ps(mask)); }
    { a4::vec4 res = a4v::addK<1, 1>(vf15, a4v::bcast<3>(vf15)); a4::q128 mask = a4v::set_epi32(0, 0, 0, -1); vf15 = a4v::blendv_ps(vf15, res, a4v::castsi128_ps(mask)); }
    { float fs = a4v::cvtss_f32(a4v::shuffle_ps<0x03>(vf0, vf0)); float ft = a4v::cvtss_f32(a4v::bcast<0>(vf15)); vq = a4v::rsqrt(vstat, fs, ft); }
    { a4::vec4 res = a4v::mul(vf14, a4v::set1_ps(vq)); a4::q128 mask = a4v::set_epi32(-1, -1, -1, -1); vf15 = a4v::blendv_ps(vf15, res, a4v::castsi128_ps(mask)); }
    A4_W128(ADD32((uint32_t)v0, 0), a4v::castps_si128(vf15));
    { (sp = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)sp, 144)))); io_vf4 = vf4; io_vf5 = vf5; io_vf6 = vf6; io_vf7 = vf7; io_vf8 = vf8; io_vf10 = vf10; io_vf11 = vf11; io_vf12 = vf12; io_vf13 = vf13; io_vf14 = vf14; io_vf15 = vf15; io_vf16 = vf16; io_vf17 = vf17; io_vf24 = vf24; io_vf25 = vf25; io_vf26 = vf26; io_vf27 = vf27; io_vf28 = vf28; io_vf29 = vf29; io_vacc = vacc; io_vq = vq; io_vstat = vstat; io_h2 = h2; io_h3 = h3; io_h4 = h4; io_h7 = h7; io_h8 = h8; io_h9 = h9; io_h10 = h10; io_h11 = h11; io_h12 = h12; io_h13 = h13; io_h14 = h14; io_h15 = h15; return v0; }
    __builtin_unreachable();
}

// nico_calcWormGenealogicalNodeNext (0x14648a0): typed by tools/typed_lift2.py (round A6; frame calls; VU0 in vf0 vf8 vf10 vf12 vf13 vf14 vf15 vf16 vf17 vf25 vf28 vf29 vacc vstat h3 h4 h6 h8, out vf4 vf5 vf6 vf7 vf8 vf10 vf11 vf12 vf13 vf14 vf15 vf16 vf17 vf24 vf25 vf26 vf27 vf28 vf29 vacc vq vstat h2 h3 h4 h7 h8 h9 h10 h11 h12 h13 h14 h15, 64-bit inputs t1, guest sp, frame 16 bytes, 0 slot accesses)
inline uint64_t nico_calcWormGenealogicalNodeNext(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint64_t v0, uint64_t a0, uint64_t w64_t1, uint32_t sp, float f0, float f1, float f2, float f3, float f4, float f5, float f6, float f7, float f8, float f9, float f10, float f11, float f12, float f13, float f14, float f15, float f16, float f17, float f18, float f19, float f20, float f21, float f22, float f23, float f24, float f25, float f26, float f27, float f28, float f29, float f30, float f31, const a4::vec4 &io_vf0, a4::vec4 &io_vf4, a4::vec4 &io_vf5, a4::vec4 &io_vf6, a4::vec4 &io_vf7, a4::vec4 &io_vf8, a4::vec4 &io_vf10, a4::vec4 &io_vf11, a4::vec4 &io_vf12, a4::vec4 &io_vf13, a4::vec4 &io_vf14, a4::vec4 &io_vf15, a4::vec4 &io_vf16, a4::vec4 &io_vf17, a4::vec4 &io_vf24, a4::vec4 &io_vf25, a4::vec4 &io_vf26, a4::vec4 &io_vf27, a4::vec4 &io_vf28, a4::vec4 &io_vf29, a4::vec4 &io_vacc, float &io_vq, uint16_t &io_vstat, uint64_t &io_h2, uint64_t &io_h3, uint64_t &io_h4, uint64_t io_h6, uint64_t &io_h7, uint64_t &io_h8, uint64_t &io_h9, uint64_t &io_h10, uint64_t &io_h11, uint64_t &io_h12, uint64_t &io_h13, uint64_t &io_h14, uint64_t &io_h15)
{
    uint64_t v1 = 0;
    uint32_t a1 = 0;
    uint64_t a2 = 0;
    uint64_t a3 = 0;
    uint32_t t0 = 0;
    uint32_t t1 = (uint32_t)w64_t1;
    uint64_t s0 = 0;
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
    a4::vec4 vf24 = io_vf24;
    a4::vec4 vf25 = io_vf25;
    a4::vec4 vf26 = io_vf26;
    a4::vec4 vf27 = io_vf27;
    a4::vec4 vf28 = io_vf28;
    a4::vec4 vf29 = io_vf29;
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
    uint64_t h10 = io_h10;
    uint64_t h11 = io_h11;
    uint64_t h12 = io_h12;
    uint64_t h13 = io_h13;
    uint64_t h14 = io_h14;
    uint64_t h15 = io_h15;
    (s0 = (uint64_t)((uint64_t)a0 + (uint64_t)0ULL));
    { (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)s0, 0)))); A3_CHARGE(); v0 = GetSCRCharaAIWorkPtr(ram, rt, v0, a0, f12, f13, f14, f15, f16, f17, f18, f19, h2); }
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 4294967280))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v0, 16819))));
    (a0 = (uint64_t)((uint64_t)s0 + (uint64_t)0ULL));
    { (s0 = (uint64_t)(RC_U64X(v0) & RC_U64X(v1))); A3_CHARGE(); v0 = nico_calcWormGenealogicalNodeNextBase(ram, rt, v0, a0, w64_t1, a1_sx64((uint32_t)(sp + 0xFFFFFFF0u)), f0, f1, f2, f3, f4, f5, f6, f7, f8, f9, f10, f11, f12, f13, f14, f15, f16, f17, f18, f19, f20, f21, f22, f23, f24, f25, f26, f27, f28, f29, f30, f31, vf0, vf4, vf5, vf6, vf7, vf8, vf10, vf11, vf12, vf13, vf14, vf15, vf16, vf17, vf24, vf25, vf26, vf27, vf28, vf29, vacc, vq, vstat, h2, h3, h4, h6, h8, h9, h10, h11, h12, h13, h14, h15); }
    (v1 = (uint64_t)((int64_t)(int32_t)((int8_t)A1_R8B((uint32_t)s0, 4763))));
    { const uint32_t _dynInsn = A1_R32(0x14648D0u); const int32_t _dynSimm = (int16_t)(_dynInsn & 0xFFFFu); const uint32_t _dynImm = _dynInsn & 0xFFFFu; (void)_dynSimm; (void)_dynImm; (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)_dynImm << 16)))); }
    t1 = 0x1460000u;
    { const bool c = (int64_t)v1 == 0; { const uint32_t _dynInsn = A1_R32(0x14648DCu); const int32_t _dynSimm = (int16_t)(_dynInsn & 0xFFFFu); const uint32_t _dynImm = _dynInsn & 0xFFFFu; (void)_dynSimm; (void)_dynImm; (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v0, _dynSimm)))); } if (c) goto L_1464924; }
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B(t1, 29704))));
    do { const a4::q128 _v = (A4_R128(ADD32((uint32_t)v0, 0))); h7 = a4::hi64(_v); std::memcpy(&a3, &_v, 8); } while (0);
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)SLL32((uint32_t)v1, 2))));
    (t0 = (uint32_t)((int32_t)((int32_t)ADD32((uint32_t)v1, (uint32_t)s0))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v0, (uint32_t)v1))));
    (a1 = (uint32_t)((int32_t)((int32_t)SLL32((uint32_t)v1, 4))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)SLL32((uint32_t)v0, 4))));
    (a2 = (uint64_t)((uint64_t)a1_sx64(t0) + (uint64_t)0ULL));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v0, (uint32_t)s0))));
    (a1 = (uint32_t)((int32_t)((int32_t)ADD32(a1, (uint32_t)s0))));
    do { const a4::q128 _v = (A4_R128(ADD32((uint32_t)v0, 16))); h4 = a4::hi64(_v); std::memcpy(&a0, &_v, 8); } while (0);
    (v1 = (uint64_t)((uint64_t)RC_U64X(a2) + (uint64_t)0ULL));
    A4_W128(ADD32((uint32_t)v0, 32), a4::mk128(h7, a3));
    A4_W128(ADD32(a1, 3104), a4::mk128(h4, a0));
    A1_W8B((uint32_t)a2, 3744, (uint8_t)0u);
    A1_W8B((uint32_t)v1, 3776, (uint8_t)0u);
    A1_W8B(t0, 3808, (uint8_t)0u);
L_1464924:;
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(t1, 29704))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)v1, 0))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v0, 1))));
    A1_W32B((uint32_t)v1, 0, (uint32_t)v0);
    { io_vf4 = vf4; io_vf5 = vf5; io_vf6 = vf6; io_vf7 = vf7; io_vf8 = vf8; io_vf10 = vf10; io_vf11 = vf11; io_vf12 = vf12; io_vf13 = vf13; io_vf14 = vf14; io_vf15 = vf15; io_vf16 = vf16; io_vf17 = vf17; io_vf24 = vf24; io_vf25 = vf25; io_vf26 = vf26; io_vf27 = vf27; io_vf28 = vf28; io_vf29 = vf29; io_vacc = vacc; io_vq = vq; io_vstat = vstat; io_h2 = h2; io_h3 = h3; io_h4 = h4; io_h7 = h7; io_h8 = h8; io_h9 = h9; io_h10 = h10; io_h11 = h11; io_h12 = h12; io_h13 = h13; io_h14 = h14; io_h15 = h15; return v0; }
    __builtin_unreachable();
}

// nico_GetGeometryPointerAttr (0x134c400): typed by tools/typed_lift2.py (round A6; frame calls, extra outputs xo[0]=a0 xo[1]=a1 xo[2]=a2 xo[3]=a3; VU0 in -, out h2 h3, relocated calls 1, guest sp, frame 16 bytes, 0 slot accesses)
inline uint64_t nico_GetGeometryPointerAttr(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint64_t a0, uint64_t a2, uint64_t a3, uint32_t sp, float f0, float f1, float f2, float f3, float f4, float f5, float f6, float f7, float f8, float f9, float f10, float f11, float f12, float f13, float f14, float f15, float f16, float f17, float f18, float f19, float f20, float f21, float f22, float f23, float f24, float f25, float f26, float f27, float f28, float f29, float f30, float f31, uint64_t &io_h2, uint64_t &io_h3, uint64_t *xo = nullptr)
{
    uint64_t a1 = 0;
    uint64_t h2 = io_h2;
    uint64_t h3 = io_h3;
    { uint64_t a7c[4] = {0u, 0u, 0u, 0u}; const uint64_t a7_t = GetGeometryPointerCharColAttr(ram, rt, a0, a2, a3, (uint32_t)(sp + 0x0u), f0, f1, f2, f3, f4, f5, f6, f7, f8, f9, f10, f11, f12, f13, f14, f15, f16, f17, f18, f19, f20, f21, f22, f23, f24, f25, f26, f27, f28, f29, f30, f31, h2, h3, a7c); a0 = a7c[0]; a1 = a7c[1]; a2 = a7c[2]; a3 = a7c[3]; if (xo) { xo[0] = a0; xo[1] = a1; xo[2] = a2; xo[3] = a3; } io_h2 = h2; io_h3 = h3; return a7_t; }
    __builtin_unreachable();
}

// gamecore_sub_0137B400 (0x137b400): typed by tools/typed_lift2.py (round A6; frame calls; VU0 in -, out h2 h3, relocated calls 1, 64-bit inputs a0 a2 a3, guest sp, frame 16 bytes, 0 slot accesses)
inline uint64_t gamecore_sub_0137B400(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint64_t v0, uint64_t w64_a0, uint64_t w64_a2, uint64_t w64_a3, uint32_t sp, float f0, float f1, float f2, float f3, float f4, float f5, float f6, float f7, float f8, float f9, float f10, float f11, float f12, float f13, float f14, float f15, float f16, float f17, float f18, float f19, float f20, float f21, float f22, float f23, float f24, float f25, float f26, float f27, float f28, float f29, float f30, float f31, uint64_t &io_h2, uint64_t &io_h3)
{
    uint32_t a0 = (uint32_t)w64_a0;
    uint32_t a2 = (uint32_t)w64_a2;
    uint32_t a3 = (uint32_t)w64_a3;
    uint64_t h2 = io_h2;
    uint64_t h3 = io_h3;
    { A3_CHARGE(); v0 = charrootdyn_getCollisionAttribute(ram, rt, w64_a0, w64_a2, w64_a3, (uint32_t)(sp + 0xFFFFFFF0u), f0, f1, f2, f3, f4, f5, f6, f7, f8, f9, f10, f11, f12, f13, f14, f15, f16, f17, f18, f19, f20, f21, f22, f23, f24, f25, f26, f27, f28, f29, f30, f31, h2, h3); }
    (v0 = (uint64_t)(v0 & (uint64_t)(uint16_t)2));
    (v0 = (uint64_t)(((uint64_t)RC_U64X(v0) < (uint64_t)(int64_t)(int32_t)1) ? 1 : 0));
    { io_h2 = h2; io_h3 = h3; return v0; }
    __builtin_unreachable();
}

// gamecore_sub_013B5FB0 (0x13b5fb0): typed by tools/typed_lift2.py (round A6; frame calls; VU0 in -, out h2 h3, relocated calls 1, 64-bit inputs a2 a3, guest sp, frame 16 bytes, 0 slot accesses)
inline uint64_t gamecore_sub_013B5FB0(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint64_t v0, uint64_t a0, uint64_t w64_a2, uint64_t w64_a3, uint32_t sp, float f0, float f1, float f2, float f3, float f4, float f5, float f6, float f7, float f8, float f9, float f10, float f11, float f12, float f13, float f14, float f15, float f16, float f17, float f18, float f19, float f20, float f21, float f22, float f23, float f24, float f25, float f26, float f27, float f28, float f29, float f30, float f31, uint64_t &io_h2, uint64_t &io_h3)
{
    uint32_t v1 = 0;
    uint64_t a1 = 0;
    uint32_t a2 = (uint32_t)w64_a2;
    uint32_t a3 = (uint32_t)w64_a3;
    uint64_t h2 = io_h2;
    uint64_t h3 = io_h3;
    { A3_CHARGE(); v0 = charrootdyn_getCollisionAttribute(ram, rt, a0, w64_a2, w64_a3, (uint32_t)(sp + 0xFFFFFFF0u), f0, f1, f2, f3, f4, f5, f6, f7, f8, f9, f10, f11, f12, f13, f14, f15, f16, f17, f18, f19, f20, f21, f22, f23, f24, f25, f26, f27, f28, f29, f30, f31, h2, h3); }
    (a0 = (uint64_t)(v0 & (uint64_t)(uint16_t)4));
    (v0 = (uint64_t)(v0 & (uint64_t)(uint16_t)3));
    v1 = 1u;
    { const bool c = (int64_t)v0 == (int64_t)(int32_t)v1; (a1 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL)); if (c) goto L_13b5fdc; }
    if ((int64_t)a0 == 0) goto L_13b5fe4;
L_13b5fdc:;
    (a1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 1))));
L_13b5fe4:;
    (v0 = (uint64_t)((uint64_t)RC_U64X(a1) + (uint64_t)0ULL));
    { io_h2 = h2; io_h3 = h3; return v0; }
    __builtin_unreachable();
}

// nico_calcWormGenealogicalNodeTail (0x1464948): typed by tools/typed_lift2.py (round A6; frame calls; VU0 in vf0 vf7 vf10 vf12 vf13 vf14 vf15 vf16 vf17 vf25 vf28 vf29 vacc vstat h3 h4 h6 h8, out vf4 vf5 vf6 vf7 vf8 vf10 vf11 vf12 vf13 vf14 vf15 vf16 vf17 vf24 vf25 vf26 vf27 vf28 vf29 vacc vq vstat h2 h3 h4 h6 h7 h8 h9 h10 h11 h12 h13 h14 h15, 64-bit inputs t1, guest sp, frame 32 bytes, 0 slot accesses)
inline uint64_t nico_calcWormGenealogicalNodeTail(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint64_t v0, uint64_t a0, uint64_t w64_t1, uint32_t sp, float f0, float f1, float f2, float f3, float f4, float f5, float f6, float f7, float f8, float f9, float f10, float f11, float f12, float f13, float f14, float f15, float f16, float f17, float f18, float f19, float f20, float f21, float f22, float f23, float f24, float f25, float f26, float f27, float f28, float f29, float f30, float f31, const a4::vec4 &io_vf0, a4::vec4 &io_vf4, a4::vec4 &io_vf5, a4::vec4 &io_vf6, a4::vec4 &io_vf7, a4::vec4 &io_vf8, a4::vec4 &io_vf10, a4::vec4 &io_vf11, a4::vec4 &io_vf12, a4::vec4 &io_vf13, a4::vec4 &io_vf14, a4::vec4 &io_vf15, a4::vec4 &io_vf16, a4::vec4 &io_vf17, a4::vec4 &io_vf24, a4::vec4 &io_vf25, a4::vec4 &io_vf26, a4::vec4 &io_vf27, a4::vec4 &io_vf28, a4::vec4 &io_vf29, a4::vec4 &io_vacc, float &io_vq, uint16_t &io_vstat, uint64_t &io_h2, uint64_t &io_h3, uint64_t &io_h4, uint64_t &io_h6, uint64_t &io_h7, uint64_t &io_h8, uint64_t &io_h9, uint64_t &io_h10, uint64_t &io_h11, uint64_t &io_h12, uint64_t &io_h13, uint64_t &io_h14, uint64_t &io_h15)
{
    uint32_t v1 = 0;
    uint32_t a1 = 0;
    uint64_t a2 = 0;
    uint64_t a3 = 0;
    uint32_t t0 = 0;
    uint32_t t1 = (uint32_t)w64_t1;
    uint64_t s0 = 0;
    uint64_t s1 = 0;
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
    a4::vec4 vf24 = io_vf24;
    a4::vec4 vf25 = io_vf25;
    a4::vec4 vf26 = io_vf26;
    a4::vec4 vf27 = io_vf27;
    a4::vec4 vf28 = io_vf28;
    a4::vec4 vf29 = io_vf29;
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
    uint64_t h10 = io_h10;
    uint64_t h11 = io_h11;
    uint64_t h12 = io_h12;
    uint64_t h13 = io_h13;
    uint64_t h14 = io_h14;
    uint64_t h15 = io_h15;
    (s0 = (uint64_t)((uint64_t)a0 + (uint64_t)0ULL));
    { (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)s0, 0)))); A3_CHARGE(); v0 = GetSCRCharaAIWorkPtr(ram, rt, v0, a0, f12, f13, f14, f15, f16, f17, f18, f19, h2); }
    v1 = 0xFFFFFFF0u;
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v0, 16819))));
    (a0 = (uint64_t)((uint64_t)s0 + (uint64_t)0ULL));
    (s1 = (uint64_t)(RC_U64X(v0) & a1_sx64(v1)));
    { (s0 = (uint64_t)((int64_t)(int32_t)((int8_t)A1_R8B((uint32_t)s1, 4763)))); A3_CHARGE(); v0 = nico_calcWormGenealogicalNodeTailBase(ram, rt, v0, a0, w64_t1, a1_sx64((uint32_t)(sp + 0xFFFFFFE0u)), f0, f1, f2, f3, f4, f5, f6, f7, f8, f9, f10, f11, f12, f13, f14, f15, f16, f17, f18, f19, f20, f21, f22, f23, f24, f25, f26, f27, f28, f29, f30, f31, vf0, vf4, vf5, vf6, vf7, vf8, vf10, vf11, vf12, vf13, vf14, vf15, vf16, vf17, vf24, vf25, vf26, vf27, vf28, vf29, vacc, vq, vstat, h2, h3, h4, h6, h7, h8, h9, h10, h11, h12, h13, h14, h15); }
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)326 << 16))));
    { const uint32_t _dynInsn = A1_R32(0x1464980u); const int32_t _dynSimm = (int16_t)(_dynInsn & 0xFFFFu); const uint32_t _dynImm = _dynInsn & 0xFFFFu; (void)_dynSimm; (void)_dynImm; (v1 = (uint32_t)((int32_t)((int32_t)((uint32_t)_dynImm << 16)))); }
    (t1 = (uint32_t)((int32_t)((int32_t)ADD32((uint32_t)v0, 29704))));
    { const uint32_t _dynInsn = A1_R32(0x1464988u); const int32_t _dynSimm = (int16_t)(_dynInsn & 0xFFFFu); const uint32_t _dynImm = _dynInsn & 0xFFFFu; (void)_dynSimm; (void)_dynImm; (v1 = (uint32_t)((int32_t)((int32_t)ADD32(v1, _dynSimm)))); }
    { const bool c = (int64_t)s0 == 0; A1_W8B((uint32_t)s1, 4763, (uint8_t)0u); if (c) goto L_14649ec; }
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B(t1, 0))));
    do { const a4::q128 _v = (A4_R128(ADD32(v1, 0))); h6 = a4::hi64(_v); std::memcpy(&a2, &_v, 8); } while (0);
    (v1 = (uint32_t)((int32_t)((int32_t)SLL32((uint32_t)a0, 2))));
    (t0 = (uint32_t)((int32_t)((int32_t)ADD32((uint32_t)a0, (uint32_t)s1))));
    (v1 = (uint32_t)((int32_t)((int32_t)ADD32(v1, (uint32_t)a0))));
    (a1 = (uint32_t)((int32_t)((int32_t)SLL32((uint32_t)a0, 4))));
    v1 = v1 << 4;
    (a3 = (uint64_t)((uint64_t)a1_sx64(t0) + (uint64_t)0ULL));
    (v1 = (uint32_t)((int32_t)((int32_t)ADD32(v1, (uint32_t)s1))));
    (a1 = (uint32_t)((int32_t)((int32_t)ADD32(a1, (uint32_t)s1))));
    do { const a4::q128 _v = (A4_R128(ADD32(v1, 16))); h2 = a4::hi64(_v); std::memcpy(&v0, &_v, 8); } while (0);
    (a0 = (uint64_t)((uint64_t)RC_U64X(a3) + (uint64_t)0ULL));
    A4_W128(ADD32(v1, 32), a4::mk128(h6, a2));
    A4_W128(ADD32(a1, 3104), a4::mk128(h2, v0));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 1))));
    A1_W8B((uint32_t)a3, 3744, (uint8_t)0u);
    A1_W8B((uint32_t)a0, 3776, (uint8_t)0u);
    A1_W8B(t0, 3808, (uint8_t)0u);
    A1_W8B((uint32_t)s1, 4760, (uint8_t)(uint32_t)v0);
    v1 = A1_R32(t1);
    A1_W8B((uint32_t)s1, 4762, (uint8_t)0u);
    A1_W32B((uint32_t)s1, 4756, v1);
L_14649ec:;
    { io_vf4 = vf4; io_vf5 = vf5; io_vf6 = vf6; io_vf7 = vf7; io_vf8 = vf8; io_vf10 = vf10; io_vf11 = vf11; io_vf12 = vf12; io_vf13 = vf13; io_vf14 = vf14; io_vf15 = vf15; io_vf16 = vf16; io_vf17 = vf17; io_vf24 = vf24; io_vf25 = vf25; io_vf26 = vf26; io_vf27 = vf27; io_vf28 = vf28; io_vf29 = vf29; io_vacc = vacc; io_vq = vq; io_vstat = vstat; io_h2 = h2; io_h3 = h3; io_h4 = h4; io_h6 = h6; io_h7 = h7; io_h8 = h8; io_h9 = h9; io_h10 = h10; io_h11 = h11; io_h12 = h12; io_h13 = h13; io_h14 = h14; io_h15 = h15; return v0; }
    __builtin_unreachable();
}

// nico_calcWormGenealogicalNodeDownTail (0x1464a00): typed by tools/typed_lift2.py (round A6; loop frame calls; VU0 in vf0 vf7 vf10 vf12 vf13 vf14 vf15 vf16 vf17 vf25 vf28 vf29 vacc vstat h3 h4 h6 h8, out vf4 vf5 vf6 vf7 vf8 vf10 vf11 vf12 vf13 vf14 vf15 vf16 vf17 vf24 vf25 vf26 vf27 vf28 vf29 vacc vq vstat h2 h3 h4 h5 h7 h8 h9 h10 h11 h12 h13 h14 h15, 64-bit inputs t1, guest sp, frame 16 bytes, 0 slot accesses)
inline uint64_t nico_calcWormGenealogicalNodeDownTail(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint64_t v0, uint64_t a0, uint64_t w64_t1, uint32_t sp, float f0, float f1, float f2, float f3, float f4, float f5, float f6, float f7, float f8, float f9, float f10, float f11, float f12, float f13, float f14, float f15, float f16, float f17, float f18, float f19, float f20, float f21, float f22, float f23, float f24, float f25, float f26, float f27, float f28, float f29, float f30, float f31, const a4::vec4 &io_vf0, a4::vec4 &io_vf4, a4::vec4 &io_vf5, a4::vec4 &io_vf6, a4::vec4 &io_vf7, a4::vec4 &io_vf8, a4::vec4 &io_vf10, a4::vec4 &io_vf11, a4::vec4 &io_vf12, a4::vec4 &io_vf13, a4::vec4 &io_vf14, a4::vec4 &io_vf15, a4::vec4 &io_vf16, a4::vec4 &io_vf17, a4::vec4 &io_vf24, a4::vec4 &io_vf25, a4::vec4 &io_vf26, a4::vec4 &io_vf27, a4::vec4 &io_vf28, a4::vec4 &io_vf29, a4::vec4 &io_vacc, float &io_vq, uint16_t &io_vstat, uint64_t &io_h2, uint64_t &io_h3, uint64_t &io_h4, uint64_t &io_h5, uint64_t io_h6, uint64_t &io_h7, uint64_t &io_h8, uint64_t &io_h9, uint64_t &io_h10, uint64_t &io_h11, uint64_t &io_h12, uint64_t &io_h13, uint64_t &io_h14, uint64_t &io_h15)
{
    uint32_t v1 = 0;
    uint32_t a1 = 0;
    uint32_t t1 = (uint32_t)w64_t1;
    uint64_t s0 = 0;
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
    a4::vec4 vf24 = io_vf24;
    a4::vec4 vf25 = io_vf25;
    a4::vec4 vf26 = io_vf26;
    a4::vec4 vf27 = io_vf27;
    a4::vec4 vf28 = io_vf28;
    a4::vec4 vf29 = io_vf29;
    a4::vec4 vacc = io_vacc;
    float vq = io_vq;
    uint16_t vstat = io_vstat;
    uint64_t h2 = io_h2;
    uint64_t h3 = io_h3;
    uint64_t h4 = io_h4;
    uint64_t h5 = io_h5;
    uint64_t h6 = io_h6;
    uint64_t h7 = io_h7;
    uint64_t h8 = io_h8;
    uint64_t h9 = io_h9;
    uint64_t h10 = io_h10;
    uint64_t h11 = io_h11;
    uint64_t h12 = io_h12;
    uint64_t h13 = io_h13;
    uint64_t h14 = io_h14;
    uint64_t h15 = io_h15;
    (s0 = (uint64_t)((uint64_t)a0 + (uint64_t)0ULL));
    { (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)s0, 0)))); A3_CHARGE(); v0 = GetSCRCharaAIWorkPtr(ram, rt, v0, a0, f12, f13, f14, f15, f16, f17, f18, f19, h2); }
    v1 = 0xFFFFFFF0u;
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v0, 16819))));
    (a0 = (uint64_t)((uint64_t)s0 + (uint64_t)0ULL));
    { (s0 = (uint64_t)(RC_U64X(v0) & a1_sx64(v1))); A3_CHARGE(); v0 = nico_calcWormGenealogicalNodeTailBase(ram, rt, v0, a0, w64_t1, a1_sx64((uint32_t)(sp + 0xFFFFFFF0u)), f0, f1, f2, f3, f4, f5, f6, f7, f8, f9, f10, f11, f12, f13, f14, f15, f16, f17, f18, f19, f20, f21, f22, f23, f24, f25, f26, f27, f28, f29, f30, f31, vf0, vf4, vf5, vf6, vf7, vf8, vf10, vf11, vf12, vf13, vf14, vf15, vf16, vf17, vf24, vf25, vf26, vf27, vf28, vf29, vacc, vq, vstat, h2, h3, h4, h6, h7, h8, h9, h10, h11, h12, h13, h14, h15); }
    (v1 = (uint32_t)((int32_t)((int32_t)A1_R32B((uint32_t)s0, 4756))));
    a1 = 1u;
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(v1, 4294967294))));
    (v0 = (uint64_t)(((int64_t)(int64_t)a1_sx64(a1) < (int64_t)(int64_t)RC_U64X(v0)) ? 1 : 0));
    if ((int64_t)v0 == 0) goto L_1464a6c;
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(v1, 4294967293))));
    (v1 = (uint32_t)((int32_t)((int32_t)ADD32((uint32_t)s0, 3809))));
L_1464a50:;
    (v0 = (uint64_t)((int64_t)(int32_t)((int8_t)A1_R8B(v1, 0))));
    v1 = v1 + 1u;
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)a0, 4294967295))));
    { const bool c = (int64_t)a0 != 0; if (RC_U64X(v0) == 0) do { const a4::q128 _v = (a4v::setzero_si128()); h5 = a4::hi64(_v); a1 = (uint32_t)a4::lo64(_v); } while (0); if (c) { A3_BACK(); goto L_1464a50; } }
L_1464a6c:;
    { const bool c = a1 == 0; (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 1)))); if (c) goto L_1464a78; }
    A1_W8B((uint32_t)s0, 4762, (uint8_t)(uint32_t)v0);
L_1464a78:;
    { io_vf4 = vf4; io_vf5 = vf5; io_vf6 = vf6; io_vf7 = vf7; io_vf8 = vf8; io_vf10 = vf10; io_vf11 = vf11; io_vf12 = vf12; io_vf13 = vf13; io_vf14 = vf14; io_vf15 = vf15; io_vf16 = vf16; io_vf17 = vf17; io_vf24 = vf24; io_vf25 = vf25; io_vf26 = vf26; io_vf27 = vf27; io_vf28 = vf28; io_vf29 = vf29; io_vacc = vacc; io_vq = vq; io_vstat = vstat; io_h2 = h2; io_h3 = h3; io_h4 = h4; io_h5 = h5; io_h7 = h7; io_h8 = h8; io_h9 = h9; io_h10 = h10; io_h11 = h11; io_h12 = h12; io_h13 = h13; io_h14 = h14; io_h15 = h15; return v0; }
    __builtin_unreachable();
}

// IsGeoColStatus (0x139ad68): typed by tools/typed_lift2.py (round A6; frame calls, extra outputs xo[0]=a2 xo[1]=a3; VU0 in -, out h2 h3, 64-bit inputs a0, guest sp, frame 32 bytes, 0 slot accesses)
inline uint64_t IsGeoColStatus(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint64_t v0, uint64_t w64_a0, uint64_t a1, uint64_t a2, uint64_t a3, uint32_t sp, float f0, float f1, float f2, float f3, float f4, float f5, float f6, float f7, float f8, float f9, float f10, float f11, float f12, float f13, float f14, float f15, float f16, float f17, float f18, float f19, float f20, float f21, float f22, float f23, float f24, float f25, float f26, float f27, float f28, float f29, float f30, float f31, uint64_t &io_h2, uint64_t &io_h3, uint64_t *xo = nullptr)
{
    uint32_t a0 = (uint32_t)w64_a0;
    uint64_t s0 = 0;
    uint64_t s1 = 0;
    uint64_t h2 = io_h2;
    uint64_t h3 = io_h3;
    (s0 = (uint64_t)((uint64_t)a1 + (uint64_t)0ULL));
    { (s1 = (uint64_t)((uint64_t)a2 + (uint64_t)0ULL)); A3_CHARGE(); uint64_t a7c[4] = {0u, 0u, 0u, 0u}; v0 = nico_GetGeometryPointerAttr(ram, rt, w64_a0, a2, a3, (uint32_t)(sp + 0xFFFFFFE0u), f0, f1, f2, f3, f4, f5, f6, f7, f8, f9, f10, f11, f12, f13, f14, f15, f16, f17, f18, f19, f20, f21, f22, f23, f24, f25, f26, f27, f28, f29, f30, f31, h2, h3, a7c); a2 = a7c[2]; a3 = a7c[3]; }
    (v0 = (uint64_t)(v0 & s0));
    (v0 = (uint64_t)(v0 ^ s1));
    (v0 = (uint64_t)(((uint64_t)v0 < (uint64_t)(int64_t)(int32_t)1) ? 1 : 0));
    { if (xo) { xo[0] = a2; xo[1] = a3; } io_h2 = h2; io_h3 = h3; return v0; }
    __builtin_unreachable();
}

// gamecore_sub_013B5CC8 (0x13b5cc8): typed by tools/typed_lift2.py (round A6; guest frame calls; VU0 in h5, out h2 h3, relocated calls 1)
inline uint64_t gamecore_sub_013B5CC8(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint64_t v0, uint64_t a0, uint64_t a1, uint64_t a2, uint64_t a3, uint64_t sp, float f0, float f1, float f2, float f3, float f4, float f5, float f6, float f7, float f8, float f9, float f10, float f11, float f12, float f13, float f14, float f15, float f16, float f17, float f18, float f19, float f20, float f21, float f22, float f23, float f24, float f25, float f26, float f27, float f28, float f29, float f30, float f31, uint64_t &io_h2, uint64_t &io_h3, uint64_t io_h5)
{
    uint64_t v1 = 0;
    uint64_t s0 = 0;
    uint64_t h2 = io_h2;
    uint64_t h3 = io_h3;
    uint64_t h5 = io_h5;
    (sp = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)sp, 4294967280))));
    { (s0 = (uint64_t)((uint64_t)a1 + (uint64_t)0ULL)); A3_CHARGE(); v0 = GetCharacterClass(ram, rt, a0, h2, h5); }
    { const uint32_t _dynInsn = A1_R32(0x13B5CDCu); const int32_t _dynSimm = (int16_t)(_dynInsn & 0xFFFFu); const uint32_t _dynImm = _dynInsn & 0xFFFFu; (void)_dynSimm; (void)_dynImm; (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)_dynImm << 16)))); }
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)v0, 1872))));
    { const uint32_t _dynInsn = A1_R32(0x13B5CE4u); const int32_t _dynSimm = (int16_t)(_dynInsn & 0xFFFFu); const uint32_t _dynImm = _dynInsn & 0xFFFFu; (void)_dynSimm; (void)_dynImm; (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32(ADD32((uint32_t)v1, _dynSimm))))); }
    { const bool c = (int64_t)a0 == (int64_t)v0; { const uint32_t _dynInsn = A1_R32(0x13B5CECu); const int32_t _dynSimm = (int16_t)(_dynInsn & 0xFFFFu); const uint32_t _dynImm = _dynInsn & 0xFFFFu; (void)_dynSimm; (void)_dynImm; (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)_dynImm << 16)))); } if (c) goto L_13b5d44; }
    { const uint32_t _dynInsn = A1_R32(0x13B5CF0u); const int32_t _dynSimm = (int16_t)(_dynInsn & 0xFFFFu); const uint32_t _dynImm = _dynInsn & 0xFFFFu; (void)_dynSimm; (void)_dynImm; (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32(ADD32((uint32_t)v0, _dynSimm))))); }
    { const bool c = (int64_t)a0 == (int64_t)v1; { const uint32_t _dynInsn = A1_R32(0x13B5CF8u); const int32_t _dynSimm = (int16_t)(_dynInsn & 0xFFFFu); const uint32_t _dynImm = _dynInsn & 0xFFFFu; (void)_dynSimm; (void)_dynImm; (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)_dynImm << 16)))); } if (c) goto L_13b5d44; }
    { const uint32_t _dynInsn = A1_R32(0x13B5CFCu); const int32_t _dynSimm = (int16_t)(_dynInsn & 0xFFFFu); const uint32_t _dynImm = _dynInsn & 0xFFFFu; (void)_dynSimm; (void)_dynImm; (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32(ADD32((uint32_t)v0, _dynSimm))))); }
    { const bool c = (int64_t)a0 == (int64_t)v1; { const uint32_t _dynInsn = A1_R32(0x13B5D04u); const int32_t _dynSimm = (int16_t)(_dynInsn & 0xFFFFu); const uint32_t _dynImm = _dynInsn & 0xFFFFu; (void)_dynSimm; (void)_dynImm; (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)_dynImm << 16)))); } if (c) goto L_13b5d44; }
    { const uint32_t _dynInsn = A1_R32(0x13B5D08u); const int32_t _dynSimm = (int16_t)(_dynInsn & 0xFFFFu); const uint32_t _dynImm = _dynInsn & 0xFFFFu; (void)_dynSimm; (void)_dynImm; (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32(ADD32((uint32_t)v0, _dynSimm))))); }
    { const bool c = (int64_t)a0 == (int64_t)v1; { const uint32_t _dynInsn = A1_R32(0x13B5D10u); const int32_t _dynSimm = (int16_t)(_dynInsn & 0xFFFFu); const uint32_t _dynImm = _dynInsn & 0xFFFFu; (void)_dynSimm; (void)_dynImm; (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)_dynImm << 16)))); } if (c) goto L_13b5d44; }
    { const uint32_t _dynInsn = A1_R32(0x13B5D14u); const int32_t _dynSimm = (int16_t)(_dynInsn & 0xFFFFu); const uint32_t _dynImm = _dynInsn & 0xFFFFu; (void)_dynSimm; (void)_dynImm; (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32(ADD32((uint32_t)v0, _dynSimm))))); }
    { const bool c = (int64_t)a0 == (int64_t)v1; { const uint32_t _dynInsn = A1_R32(0x13B5D1Cu); const int32_t _dynSimm = (int16_t)(_dynInsn & 0xFFFFu); const uint32_t _dynImm = _dynInsn & 0xFFFFu; (void)_dynSimm; (void)_dynImm; (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)_dynImm << 16)))); } if (c) goto L_13b5d44; }
    { const uint32_t _dynInsn = A1_R32(0x13B5D20u); const int32_t _dynSimm = (int16_t)(_dynInsn & 0xFFFFu); const uint32_t _dynImm = _dynInsn & 0xFFFFu; (void)_dynSimm; (void)_dynImm; (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32(ADD32((uint32_t)v0, _dynSimm))))); }
    { const bool c = (int64_t)a0 == (int64_t)v1; { const uint32_t _dynInsn = A1_R32(0x13B5D28u); const int32_t _dynSimm = (int16_t)(_dynInsn & 0xFFFFu); const uint32_t _dynImm = _dynInsn & 0xFFFFu; (void)_dynSimm; (void)_dynImm; (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)_dynImm << 16)))); } if (c) goto L_13b5d44; }
    { const uint32_t _dynInsn = A1_R32(0x13B5D2Cu); const int32_t _dynSimm = (int16_t)(_dynInsn & 0xFFFFu); const uint32_t _dynImm = _dynInsn & 0xFFFFu; (void)_dynSimm; (void)_dynImm; (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32(ADD32((uint32_t)v0, _dynSimm))))); }
    { const bool c = (int64_t)a0 == (int64_t)v1; { const uint32_t _dynInsn = A1_R32(0x13B5D34u); const int32_t _dynSimm = (int16_t)(_dynInsn & 0xFFFFu); const uint32_t _dynImm = _dynInsn & 0xFFFFu; (void)_dynSimm; (void)_dynImm; (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)_dynImm << 16)))); } if (c) goto L_13b5d44; }
    { const uint32_t _dynInsn = A1_R32(0x13B5D38u); const int32_t _dynSimm = (int16_t)(_dynInsn & 0xFFFFu); const uint32_t _dynImm = _dynInsn & 0xFFFFu; (void)_dynSimm; (void)_dynImm; (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32(ADD32((uint32_t)v0, _dynSimm))))); }
    { const bool c = (int64_t)a0 != (int64_t)v1; (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 1)))); if (c) goto L_13b5da4; }
L_13b5d44:;
    { (a0 = (uint64_t)((uint64_t)s0 + (uint64_t)0ULL)); A3_CHARGE(); uint64_t a7c[4] = {0u, 0u, 0u, 0u}; v0 = nico_GetGeometryPointerAttr(ram, rt, a0, a2, a3, (uint32_t)sp, f0, f1, f2, f3, f4, f5, f6, f7, f8, f9, f10, f11, f12, f13, f14, f15, f16, f17, f18, f19, f20, f21, f22, f23, f24, f25, f26, f27, f28, f29, f30, f31, h2, h3, a7c); a2 = a7c[2]; a3 = a7c[3]; }
    (v0 = (uint64_t)(v0 & (uint64_t)(uint16_t)4));
    (v0 = (uint64_t)(((uint64_t)0ULL < (uint64_t)RC_U64X(v0)) ? 1 : 0));
    (v0 = (uint64_t)(RC_U64X(v0) << (32 + 0)));
    (v0 = (uint64_t)((int64_t)v0 >> (32 + 0)));
    if ((int64_t)v0 != 0) { (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 1)))); goto L_13b5da4; }
    { (a0 = (uint64_t)((uint64_t)s0 + (uint64_t)0ULL)); A3_CHARGE(); uint64_t a7c[4] = {0u, 0u, 0u, 0u}; v0 = nico_GetGeometryPointerAttr(ram, rt, a0, a2, a3, (uint32_t)sp, f0, f1, f2, f3, f4, f5, f6, f7, f8, f9, f10, f11, f12, f13, f14, f15, f16, f17, f18, f19, f20, f21, f22, f23, f24, f25, f26, f27, f28, f29, f30, f31, h2, h3, a7c); a2 = a7c[2]; a3 = a7c[3]; }
    (v0 = (uint64_t)(v0 & (uint64_t)(uint16_t)1));
    (v0 = (uint64_t)(RC_U64X(v0) << (32 + 0)));
    (v0 = (uint64_t)((int64_t)v0 >> (32 + 0)));
    if ((int64_t)v0 == 0) { (v0 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL)); goto L_13b5da4; }
    { (a0 = (uint64_t)((uint64_t)s0 + (uint64_t)0ULL)); A3_CHARGE(); v0 = nico_GetGeometryPointerAttr(ram, rt, a0, a2, a3, (uint32_t)sp, f0, f1, f2, f3, f4, f5, f6, f7, f8, f9, f10, f11, f12, f13, f14, f15, f16, f17, f18, f19, f20, f21, f22, f23, f24, f25, f26, f27, f28, f29, f30, f31, h2, h3); }
    (v0 = (uint64_t)(v0 & (uint64_t)(uint16_t)2));
    (v0 = (uint64_t)(((uint64_t)RC_U64X(v0) < (uint64_t)(int64_t)(int32_t)1) ? 1 : 0));
    (v0 = (uint64_t)(RC_U64X(v0) << (32 + 0)));
    (v0 = (uint64_t)((int64_t)v0 >> (32 + 0)));
    if ((int64_t)v0 != 0) { (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 1)))); goto L_13b5da4; }
    (v0 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL));
L_13b5da4:;
    { (sp = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)sp, 16)))); io_h2 = h2; io_h3 = h3; return v0; }
    __builtin_unreachable();
}
