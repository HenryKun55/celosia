// typed_a10.inc: round A10's typed functions (tools/typed_lift2.py, stage 10: recursion, the upper-half class). Generated: do not edit. Included inside a namespace (typed_a10.h: namespace typed), after typed_a7.inc. First the variants of earlier functions (<name>_x10: the same MIPS typed with extra outputs, called by stage-10 functions only), then stage 10's functions.
// the recursive functions (A10_REC index): 0 manager_sub_010C68F8
// iosCopyMatrix_x10 (0x1b9148): typed by tools/typed_lift2.py (round A6; leaf, extra outputs xo[0]=a3 xo[1]=t0 xo[2]=t1; VU0 in -, out h6 h7 h8 h9)
inline void iosCopyMatrix_x10(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t a0, uint32_t a1, uint64_t &io_h6, uint64_t &io_h7, uint64_t &io_h8, uint64_t &io_h9, uint64_t *xo = nullptr)
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
    { if (xo) { xo[0] = a3; xo[1] = t0; xo[2] = t1; } io_h6 = h6; io_h7 = h7; io_h8 = h8; io_h9 = h9; return; }
    __builtin_unreachable();
}

// GetMeshClass_u10 (0x109aa48): typed by tools/typed_lift2.py (round A6; leaf; VU0 in -, out h2)
inline uint64_t GetMeshClass_u10(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint64_t a0, uint64_t &io_h2)
{
    uint64_t v0 = 0;
    uint64_t v1 = 0;
    uint64_t h2 = io_h2;
    (v0 = (uint64_t)((uint64_t)a0 + (uint64_t)0ULL));
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 35))));
    (v1 = (uint64_t)(uint32_t)((uint8_t)A1_R8B((uint32_t)v0, 8)));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v0, 32))));
    (v1 = (uint64_t)(RC_U64X(v1) ^ RC_U64X(a0)));
    { if (RC_U64X(v1) != 0) do { const a4::q128 _v = (a4v::setzero_si128()); h2 = a4::hi64(_v); std::memcpy(&v0, &_v, 8); } while (0); io_h2 = h2; return v0; }
    __builtin_unreachable();
}

// GetLightClass_x10 (0x10f93f0): typed by tools/typed_lift2.py (round A6; leaf, extra outputs xo[0]=v1 xo[1]=a0; VU0 in -, out h2)
inline uint64_t GetLightClass_x10(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint64_t a0, uint64_t &io_h2, uint64_t *xo = nullptr)
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

// GetScriptAnimationClass_x10 (0x110a838): typed by tools/typed_lift2.py (round A6; leaf, extra outputs xo[0]=v1 xo[1]=a0; VU0 in -, out h2)
inline uint64_t GetScriptAnimationClass_x10(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint64_t a0, uint64_t &io_h2, uint64_t *xo = nullptr)
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

// GetScrLwsOriClass_u10 (0x110d868): typed by tools/typed_lift2.py (round A6; leaf; VU0 in -, out h2)
inline uint64_t GetScrLwsOriClass_u10(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint64_t a0, uint64_t &io_h2)
{
    uint64_t v0 = 0;
    uint64_t v1 = 0;
    uint64_t h2 = io_h2;
    (v0 = (uint64_t)((uint64_t)a0 + (uint64_t)0ULL));
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 22))));
    (v1 = (uint64_t)(uint32_t)((uint8_t)A1_R8B((uint32_t)v0, 8)));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v0, 32))));
    (v1 = (uint64_t)(RC_U64X(v1) ^ RC_U64X(a0)));
    { if (RC_U64X(v1) != 0) do { const a4::q128 _v = (a4v::setzero_si128()); h2 = a4::hi64(_v); std::memcpy(&v0, &_v, 8); } while (0); io_h2 = h2; return v0; }
    __builtin_unreachable();
}

// SpriteMenuGetMenuIdByHash_u10 (0x1112c88): typed by tools/typed_lift2.py (round A6; loop leaf; VU0 in -, out h4)
inline uint64_t SpriteMenuGetMenuIdByHash_u10(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint64_t a0, uint64_t a1, uint64_t a2, uint64_t &io_h4)
{
    uint64_t v0 = 0;
    uint32_t v1 = 0;
    uint64_t a3 = 0;
    uint64_t t0 = 0;
    uint64_t t1 = 0;
    uint64_t t2 = 0;
    uint32_t t3 = 0;
    uint64_t h4 = io_h4;
    (v0 = (uint64_t)(uint32_t)((uint8_t)A1_R8B((uint32_t)a0, 8)));
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)a0, 144))));
    t3 = 0xFFFFFFFFu;
    (t1 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL));
    (v0 = (uint64_t)(RC_U64X(v0) ^ (uint64_t)(uint16_t)33));
    if (RC_U64X(v0) != 0) do { const a4::q128 _v = (a4v::setzero_si128()); h4 = a4::hi64(_v); std::memcpy(&a0, &_v, 8); } while (0);
    (v0 = (uint64_t)((uint64_t)a0 + (uint64_t)0ULL));
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)v0, 8))));
    { const bool c = (int32_t)a0 <= 0; (t0 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL)); if (c) goto L_1112cf4; }
    (t1 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)v0, 4))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)t1, 0))));
    (v1 = (uint32_t)((int32_t)((int32_t)A1_R32B((uint32_t)v0, 4))));
    if ((int64_t)(int32_t)v1 == (int64_t)a1) { (a1 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)t1, 8)))); goto L_1112cf8; }
    (t2 = (uint64_t)((uint64_t)RC_U64X(a0) + (uint64_t)0ULL));
    (a0 = (uint64_t)((uint64_t)RC_U64X(t1) + (uint64_t)0ULL));
    (a3 = (uint64_t)((uint64_t)RC_U64X(t1) + (uint64_t)0ULL));
L_1112cd0:;
    (t0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)t0, 1))));
    (a3 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)a3, 68))));
    (v0 = (uint64_t)(((int64_t)(int64_t)RC_U64X(t0) < (int64_t)(int64_t)t2) ? 1 : 0));
    { const bool c = (int64_t)v0 == 0; (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)a0, 68)))); if (c) goto L_1112cf4; }
    (v1 = (uint32_t)((int32_t)((int32_t)A1_R32B((uint32_t)a3, 0))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B(v1, 4))));
    { const bool c = (int64_t)v0 != (int64_t)a1; (t1 = (uint64_t)((uint64_t)RC_U64X(a0) + (uint64_t)0ULL)); if (c) { A3_BACK(); goto L_1112cd0; } }
L_1112cf4:;
    (a1 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)t1, 8))));
L_1112cf8:;
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)SRL32((uint32_t)a1, 4))));
    (v0 = (uint64_t)(RC_U64X(v0) & (uint64_t)(uint16_t)1023));
    { const bool c = (int64_t)v0 == 0; (t0 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL)); if (c) goto L_1112d4c; }
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)t1, 4))));
    (a0 = (uint64_t)((uint64_t)RC_U64X(v0) + (uint64_t)0ULL));
    (v1 = (uint32_t)((int32_t)((int32_t)A1_R32B((uint32_t)a0, 0))));
    goto L_1112d3c;
L_1112d18:;
    (v1 = (uint32_t)((int32_t)((int32_t)SRL32((uint32_t)a1, 4))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)SLL32((uint32_t)t0, 3))));
    v1 = v1 & 0x3FFu;
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)SUB32((uint32_t)v0, (uint32_t)t0))));
    (v1 = (uint32_t)(((int64_t)(int64_t)t0 < (int64_t)(int64_t)a1_sx64(v1)) ? 1 : 0));
    { const bool c = v1 == 0; (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)SLL32((uint32_t)v0, 2)))); if (c) goto L_1112d4c; }
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)a0, (uint32_t)v0))));
    (v1 = (uint32_t)((int32_t)((int32_t)A1_R32B((uint32_t)v0, 0))));
L_1112d3c:;
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B(v1, 4))));
    if ((int64_t)v0 != (int64_t)a2) { (t0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)t0, 1)))); { A3_BACK(); goto L_1112d18; } }
    t3 = A1_R32B(v1, 20);
L_1112d4c:;
    { (v0 = (uint64_t)((uint64_t)a1_sx64(t3) + (uint64_t)0ULL)); io_h4 = h4; return v0; }
    __builtin_unreachable();
}

// GetSubLinkClass_u10 (0x1114490): typed by tools/typed_lift2.py (round A6; leaf; VU0 in -, out h2)
inline uint64_t GetSubLinkClass_u10(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint64_t a0, uint64_t &io_h2)
{
    uint64_t v0 = 0;
    uint64_t v1 = 0;
    uint64_t h2 = io_h2;
    (v0 = (uint64_t)((uint64_t)a0 + (uint64_t)0ULL));
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 21))));
    (v1 = (uint64_t)(uint32_t)((uint8_t)A1_R8B((uint32_t)v0, 8)));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v0, 28))));
    (v1 = (uint64_t)(RC_U64X(v1) ^ RC_U64X(a0)));
    { if (RC_U64X(v1) != 0) do { const a4::q128 _v = (a4v::setzero_si128()); h2 = a4::hi64(_v); std::memcpy(&v0, &_v, 8); } while (0); io_h2 = h2; return v0; }
    __builtin_unreachable();
}

// dynamic_cast_to_camera_obj_x10 (0x1162e08): typed by tools/typed_lift2.py (round A6; leaf, extra outputs xo[0]=v1; VU0 in -, out -, break)
inline uint64_t dynamic_cast_to_camera_obj_x10(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint64_t a0, float f0, float f1, float f2, float f3, float f4, float f5, float f6, float f7, float f8, float f9, float f10, float f11, float f12, float f13, float f14, float f15, float f16, float f17, float f18, float f19, float f20, float f21, float f22, float f23, float f24, float f25, float f26, float f27, float f28, float f29, float f30, float f31, uint64_t *xo = nullptr)
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

// _omakeSelectFlagChk_u10 (0x134fb28): typed by tools/typed_lift2.py (round A6; leaf; VU0 in h5, out h3)
inline uint64_t _omakeSelectFlagChk_u10(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint64_t a0, uint64_t &io_h3, uint64_t io_h5)
{
    uint64_t v0 = 0;
    uint64_t v1 = 0;
    uint32_t a1 = 0;
    uint64_t h3 = io_h3;
    uint64_t h5 = io_h5;
    (v0 = (uint64_t)(((int64_t)(int64_t)a0 < (int64_t)(int32_t)0) ? 1 : 0));
    (a1 = (uint32_t)((int32_t)((int32_t)ADD32((uint32_t)a0, 7))));
    (v1 = (uint64_t)((uint64_t)a0 + (uint64_t)0ULL));
    (a0 = (uint64_t)(a0 & (uint64_t)(uint16_t)7));
    if (RC_U64X(v0) != 0) do { const a4::q128 _v = (a4::mk128(h5, a1_sx64(a1))); h3 = a4::hi64(_v); std::memcpy(&v1, &_v, 8); } while (0);
    (v1 = (uint64_t)((int64_t)(int32_t)(SRA32((int32_t)v1, 3))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)286 << 16))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v0, (uint32_t)v1))));
    (v0 = (uint64_t)(uint32_t)((uint8_t)A1_R8B((uint32_t)v0, 98)));
    (v0 = (uint64_t)((int64_t)(int32_t)(SRA32((int32_t)v0, (uint32_t)a0 & 0x1F))));
    { (v0 = (uint64_t)(RC_U64X(v0) & (uint64_t)(uint16_t)1)); io_h3 = h3; return v0; }
    __builtin_unreachable();
}

// GetScriptLayoutClass_x10 (0x110d058): typed by tools/typed_lift2.py (round A6; frame calls, extra outputs xo[0]=v1 xo[1]=a0 xo[2]=a1; VU0 in -, out h2, frame 16 bytes, 0 slot accesses)
inline uint64_t GetScriptLayoutClass_x10(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint64_t a0, uint64_t &io_h2, uint64_t *xo = nullptr)
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
    { uint64_t a7c[2] = {0u, 0u}; const uint64_t a7_t = GetScriptAnimationClass_x10(ram, rt, a0, h2, a7c); v1 = a7c[0]; a0 = a7c[1]; if (xo) { xo[0] = v1; xo[1] = a0; xo[2] = a1_sx64(a1); } io_h2 = h2; return a7_t; }
L_110d090:;
    { if (xo) { xo[0] = v1; xo[2] = a1_sx64(a1); } io_h2 = h2; return v0; }
    __builtin_unreachable();
}

// GetLayoutClass_u10 (0x10f1a20): typed by tools/typed_lift2.py (round A6; frame calls, extra outputs xo[0]=v1 xo[1]=a0 xo[2]=a1; VU0 in -, out h2, jump tables 1, frame 16 bytes, 0 slot accesses)
inline uint64_t GetLayoutClass_u10(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint64_t a0, float f0, float f1, float f2, float f3, float f4, float f5, float f6, float f7, float f8, float f9, float f10, float f11, float f12, float f13, float f14, float f15, float f16, float f17, float f18, float f19, float f20, float f21, float f22, float f23, float f24, float f25, float f26, float f27, float f28, float f29, float f30, float f31, uint64_t &io_h2, uint64_t *xo = nullptr)
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
    { A3_CHARGE(); uint64_t a7c[3] = {0u, a0, 0u}; v0 = GetScriptLayoutClass_x10(ram, rt, a0, h2, a7c); v1 = a7c[0]; a0 = a7c[1]; a1 = a7c[2]; a3xw1 = true; a3xw2 = true; }
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v0, 112))));
    goto L_10f1aa4;
L_10f1a70:;
    { A3_CHARGE(); uint64_t a7c[1] = {0u}; v0 = dynamic_cast_to_camera_obj_x10(ram, rt, a0, f0, f1, f2, f3, f4, f5, f6, f7, f8, f9, f10, f11, f12, f13, f14, f15, f16, f17, f18, f19, f20, f21, f22, f23, f24, f25, f26, f27, f28, f29, f30, f31, a7c); v1 = a7c[0]; }
    (a0 = (uint64_t)((uint64_t)v0 + (uint64_t)0ULL)); a3xw1 = true;
    { if (xo) { xo[0] = v1; xo[1] = a0; } const uint32_t a4_t = get_camera_obj_layout_class(ram, rt, (uint32_t)a0); io_h2 = h2; return a1_sx64(a4_t); }
L_10f1a88:;
    { uint64_t a7c[2] = {0u, 0u}; const uint64_t a7_t = GetLightClass_x10(ram, rt, a0, h2, a7c); v1 = a7c[0]; a0 = a7c[1]; if (xo) { xo[0] = v1; xo[1] = a0; } io_h2 = h2; return a7_t; }
L_10f1a98:;
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)a0, 144))));
    goto L_10f1aa4;
L_10f1aa0:;
    (v0 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL));
L_10f1aa4:;
    { if (xo) { xo[0] = v1; if (a3xw1) xo[1] = a0; if (a3xw2) xo[2] = a1; } io_h2 = h2; return v0; }
    __builtin_unreachable();
}

// manager_sub_010C68F8 (0x10c68f8): typed by tools/typed_lift2.py (round A6; frame calls, extra outputs xo[0]=f0; VU0 in vacc, out vf14 vf15 vf16 vf17 vf24 vf25 vf26 vf27 vacc, frame 128 bytes, 8 slot accesses)
inline uint32_t manager_sub_010C68F8(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint64_t a0, uint64_t a1, uint64_t a2, uint64_t a3, uint64_t t0, uint64_t t1, float f0, a4::vec4 &io_vf14, a4::vec4 &io_vf15, a4::vec4 &io_vf16, a4::vec4 &io_vf17, a4::vec4 &io_vf24, a4::vec4 &io_vf25, a4::vec4 &io_vf26, a4::vec4 &io_vf27, a4::vec4 &io_vacc, uint64_t *xo = nullptr)
{
    A10_REC(0);
    uint32_t v0 = 0;
    uint32_t v1 = 0;
    uint64_t t2 = 0;
    uint32_t t3 = 0;
    uint64_t s0 = 0;
    uint64_t s1 = 0;
    uint32_t s2 = 0;
    uint64_t s3 = 0;
    uint64_t s4 = 0;
    uint64_t s5 = 0;
    a4::vec4 vf14 = io_vf14;
    a4::vec4 vf15 = io_vf15;
    a4::vec4 vf16 = io_vf16;
    a4::vec4 vf17 = io_vf17;
    a4::vec4 vf24 = io_vf24;
    a4::vec4 vf25 = io_vf25;
    a4::vec4 vf26 = io_vf26;
    a4::vec4 vf27 = io_vf27;
    a4::vec4 vacc = io_vacc;
    uint8_t a2fr[128];
    bool a3xw0 = false;
    (t2 = (uint64_t)((uint64_t)a3 + (uint64_t)0ULL));
    (s0 = (uint64_t)((uint64_t)t0 + (uint64_t)0ULL));
    (s1 = (uint64_t)((uint64_t)t1 + (uint64_t)0ULL));
    (s3 = (uint64_t)((uint64_t)a0 + (uint64_t)0ULL));
    (s4 = (uint64_t)((uint64_t)a1 + (uint64_t)0ULL));
    (s5 = (uint64_t)((uint64_t)a2 + (uint64_t)0ULL));
    (v0 = (uint32_t)((int32_t)((int32_t)SLL32((uint32_t)t2, 6))));
    (v1 = (uint32_t)((int32_t)((int32_t)A1_R32B((uint32_t)s5, 8))));
    v1 = v0 + v1;
    (v0 = (uint32_t)((int32_t)((int32_t)ADD32(v0, (uint32_t)s3))));
    s2 = A1_R32B(v1, 48);
    t3 = A1_R32B(v1, 52);
    vf14 = a4v::castsi128_ps(A4_R128(ADD32((uint32_t)s0, 0)));
    vf15 = a4v::castsi128_ps(A4_R128(ADD32((uint32_t)s0, 16)));
    vf16 = a4v::castsi128_ps(A4_R128(ADD32((uint32_t)s0, 32)));
    vf17 = a4v::castsi128_ps(A4_R128(ADD32((uint32_t)s0, 48)));
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
    a2::frSt<a4::q128>(a2fr, 0, a4v::castps_si128(vf24));
    a2::frSt<a4::q128>(a2fr, 16, a4v::castps_si128(vf25));
    a2::frSt<a4::q128>(a2fr, 32, a4v::castps_si128(vf26));
    a2::frSt<a4::q128>(a2fr, 48, a4v::castps_si128(vf27));
    vf14 = a4v::castsi128_ps(A4_R128(ADD32((uint32_t)s1, 0)));
    vf15 = a4v::castsi128_ps(A4_R128(ADD32((uint32_t)s1, 16)));
    vf16 = a4v::castsi128_ps(A4_R128(ADD32((uint32_t)s1, 32)));
    vf17 = a4v::castsi128_ps(A4_R128(ADD32((uint32_t)s1, 48)));
    vf24 = a4v::castsi128_ps(a2::frLd<a4::q128>(a2fr, 0));
    vf25 = a4v::castsi128_ps(a2::frLd<a4::q128>(a2fr, 16));
    vf26 = a4v::castsi128_ps(a2::frLd<a4::q128>(a2fr, 32));
    vf27 = a4v::castsi128_ps(a2::frLd<a4::q128>(a2fr, 48));
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
    A4_W128(ADD32(v0, 0), a4v::castps_si128(vf24));
    A4_W128(ADD32(v0, 16), a4v::castps_si128(vf25));
    A4_W128(ADD32(v0, 32), a4v::castps_si128(vf26));
    A4_W128(ADD32(v0, 48), a4v::castps_si128(vf27));
    (t2 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)s4, (uint32_t)t2))));
    (a3 = (uint64_t)((uint64_t)a1_sx64(t3) + (uint64_t)0ULL));
    { const bool c = (int32_t)t3 < 0; A1_W8B((uint32_t)t2, 0, (uint8_t)0u); if (c) goto L_10c6a40; }
    { A3_CHARGE(); uint64_t a7c[1] = {(uint64_t)a1_bits(f0)}; v0 = manager_sub_010C68F8(ram, rt, a0, a1, a2, a3, t0, t1, f0, vf14, vf15, vf16, vf17, vf24, vf25, vf26, vf27, vacc, a7c); f0 = a1_flt((uint32_t)a7c[0]); a3xw0 = true; }
L_10c6a40:;
    { const bool c = (int32_t)s2 < 0; (a0 = (uint64_t)((uint64_t)s3 + (uint64_t)0ULL)); if (c) goto L_10c6a60; }
    (a1 = (uint64_t)((uint64_t)s4 + (uint64_t)0ULL));
    (a2 = (uint64_t)((uint64_t)s5 + (uint64_t)0ULL));
    (a3 = (uint64_t)((uint64_t)a1_sx64(s2) + (uint64_t)0ULL));
    (t0 = (uint64_t)((uint64_t)s0 + (uint64_t)0ULL));
    { (t1 = (uint64_t)((uint64_t)s1 + (uint64_t)0ULL)); A3_CHARGE(); uint64_t a7c[1] = {(uint64_t)a1_bits(f0)}; v0 = manager_sub_010C68F8(ram, rt, a0, a1, a2, a3, t0, t1, f0, vf14, vf15, vf16, vf17, vf24, vf25, vf26, vf27, vacc, a7c); f0 = a1_flt((uint32_t)a7c[0]); a3xw0 = true; }
L_10c6a60:;
    { if (xo) { if (a3xw0) xo[0] = a1_bits(f0); } io_vf14 = vf14; io_vf15 = vf15; io_vf16 = vf16; io_vf17 = vf17; io_vf24 = vf24; io_vf25 = vf25; io_vf26 = vf26; io_vf27 = vf27; io_vacc = vacc; return v0; }
    __builtin_unreachable();
}

// gamecore_sub_013828A8 (0x13828a8): typed by tools/typed_lift2.py (round A6; loop guest frame calls, extra outputs xo[0]=f20 xo[1]=f21 xo[2]=f22 xo[3]=f23 xo[4]=a1 xo[5]=a3 xo[6]=t0 xo[7]=t1; VU0 in vf0 vf7 vf10 vacc vstat, out vf5 vf6 vf7 vf8 vf10 vf14 vf15 vf16 vf17 vacc vq vstat h2 h3 h6 h7 h8 h9, relocated calls 38)
inline a1::RetVF gamecore_sub_013828A8(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint64_t v0, uint64_t a0, uint64_t a1, uint64_t a2, uint64_t a3, uint64_t t0, uint32_t sp, float f12, float f13, float f14, float f15, float f16, float f17, float f18, float f19, float f20, float f21, float f22, float f23, const a4::vec4 &io_vf0, a4::vec4 &io_vf5, a4::vec4 &io_vf6, a4::vec4 &io_vf7, a4::vec4 &io_vf8, a4::vec4 &io_vf10, a4::vec4 &io_vf14, a4::vec4 &io_vf15, a4::vec4 &io_vf16, a4::vec4 &io_vf17, a4::vec4 &io_vacc, float &io_vq, uint16_t &io_vstat, uint64_t &io_h2, uint64_t &io_h3, uint64_t &io_h6, uint64_t &io_h7, uint64_t &io_h8, uint64_t &io_h9, uint64_t *xo = nullptr)
{
    float f0 = 0.0f;
    float f1 = 0.0f;
    uint32_t at = 0;
    uint64_t v1 = 0;
    uint64_t t1 = 0;
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
    sp = sp - 0x150u;
    (s5 = (uint64_t)((uint64_t)a1 + (uint64_t)0ULL));
    A1_W32B(sp, 192, (uint32_t)a0);
    (a0 = (uint64_t)((uint64_t)s5 + (uint64_t)0ULL));
    (s7 = (uint64_t)((uint64_t)a2 + (uint64_t)0ULL));
    { float f = f23; uint32_t bits; std::memcpy(&bits, &f, sizeof(bits)); A1_W32B(sp, 328, bits); }
    A1_W32B(sp, 196, (uint32_t)a3);
    { float f = f22; uint32_t bits; std::memcpy(&bits, &f, sizeof(bits)); A1_W32B(sp, 320, bits); }
    { float f = f21; uint32_t bits; std::memcpy(&bits, &f, sizeof(bits)); A1_W32B(sp, 312, bits); }
    { float f = f20; uint32_t bits; std::memcpy(&bits, &f, sizeof(bits)); A1_W32B(sp, 304, bits); }
    { A1_W32B(sp, 200, (uint32_t)t0); A3_CHARGE(); v0 = GetMeshClass_u10(ram, rt, a0, h2); }
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B(sp, 200))));
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B(sp, 192))));
    (a2 = (uint64_t)((int64_t)(int32_t)((int32_t)SLL32((uint32_t)v1, 4))));
    (a3 = (uint64_t)((int64_t)(int32_t)((int32_t)SLL32((uint32_t)v1, 2))));
    (a1 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)a0, 4))));
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)a0, 112))));
    A1_W32B(sp, 208, 0u);
    (s0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v1, (uint32_t)a2))));
    (at = (uint32_t)((int32_t)((int32_t)((uint32_t)17152 << 16))));
    { uint32_t bits = a7_opq(at); std::memcpy(&f23, &bits, sizeof(bits)); }
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)a1, 68))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)v0, 12))));
    (s1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)a0, (uint32_t)a3))));
    { A1_W32B(sp, 212, (uint32_t)v0); A3_CHARGE(); v0 = a1_sx64(iosgluGetCameraContext(ram, rt, f12, f13, f14, f15, f16, f17, f18, f19)); }
    (a2 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(sp, 96))));
    (a1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v0, 416))));
    { (a0 = (uint64_t)((uint64_t)RC_U64X(a2) + (uint64_t)0ULL)); A3_CHARGE(); iosCopyMatrix_x10(ram, rt, (uint32_t)a0, (uint32_t)a1, h6, h7, h8, h9); }
    { const bool c = (int64_t)s7 == 0; (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B(sp, 196)))); if (c) goto L_1382964; }
    { (a0 = (uint64_t)((uint64_t)s7 + (uint64_t)0ULL)); A3_CHARGE(); v0 = GetMeshClass_u10(ram, rt, a0, h2); }
    A1_W32B(sp, 208, (uint32_t)v0);
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B(sp, 196))));
L_1382964:;
    if ((int64_t)v0 == 0) { do { const a4::q128 _v = (A4_R128(ADD32((uint32_t)s0, 0))); h2 = a4::hi64(_v); std::memcpy(&v0, &_v, 8); } while (0); goto L_1382978; }
    { (a0 = (uint64_t)((uint64_t)v0 + (uint64_t)0ULL)); A3_CHARGE(); v0 = GetMeshClass_u10(ram, rt, a0, h2); }
    do { const a4::q128 _v = (A4_R128(ADD32((uint32_t)s0, 0))); h2 = a4::hi64(_v); std::memcpy(&v0, &_v, 8); } while (0);
L_1382978:;
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(sp, 16))));
    (at = (uint32_t)((int32_t)((int32_t)((uint32_t)16256 << 16))));
    { uint32_t bits = a7_opq(at); std::memcpy(&f0, &bits, sizeof(bits)); }
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(sp, 96))));
    A4_W128(ADD32(sp, 0), a4::mk128(h2, v0));
    A1_W32B(sp, 12, a1_bits(f0));
    vf8 = a4v::castsi128_ps(A4_R128(ADD32(sp, 0)));
    vf14 = a4v::castsi128_ps(A4_R128(ADD32((uint32_t)a0, 0)));
    vf15 = a4v::castsi128_ps(A4_R128(ADD32((uint32_t)a0, 16)));
    vf16 = a4v::castsi128_ps(A4_R128(ADD32((uint32_t)a0, 32)));
    vf17 = a4v::castsi128_ps(A4_R128(ADD32((uint32_t)a0, 48)));
    { a4::vec4 res = a4v::mul(vf14, a4v::bcast<0>(vf8)); vacc = a4v::dest<1, 1, 1, 1>(vacc, res); }
    { a4::vec4 mul_res = a4v::mul(vf15, a4v::bcast<1>(vf8)); a4::vec4 res = a4v::addK<1, 1>(vacc, mul_res); vacc = a4v::dest<1, 1, 1, 1>(vacc, res); }
    { a4::vec4 mul_res = a4v::mul(vf16, a4v::bcast<2>(vf8)); a4::vec4 res = a4v::addK<1, 1>(vacc, mul_res); vacc = a4v::dest<1, 1, 1, 1>(vacc, res); }
    { a4::vec4 mul_res = a4v::mul(vf17, a4v::bcast<3>(vf8)); a4::vec4 res = a4v::addK<1, 1>(vacc, mul_res); a4::q128 mask = a4v::set_epi32(-1, -1, -1, -1); vf10 = a4v::blendv_ps(vf10, res, a4v::castsi128_ps(mask)); }
    A4_W128(ADD32((uint32_t)v1, 0), a4v::castps_si128(vf10));
    (a1 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B(sp, 212))));
    { uint32_t bits = A1_R32B(sp, 24); float f; std::memcpy(&f, &bits, sizeof(f)); f21 = f; }
    A1_W32B(sp, 204, 0u);
    { const bool c = (int32_t)a1 <= 0; A1_W32B(sp, 24, 0u); if (c) goto L_1382f50; }
    (a2 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)a1, 4294967295))));
    A1_W32B(sp, 220, (uint32_t)s1);
    A1_W32B(sp, 216, (uint32_t)a2);
    (fp = (uint64_t)((uint64_t)s0 + (uint64_t)0ULL));
L_13829e0:;
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B(sp, 204))));
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B(sp, 216))));
    if ((int64_t)v0 != (int64_t)v1) { do { const a4::q128 _v = (A4_R128(ADD32((uint32_t)fp, 16))); h2 = a4::hi64(_v); std::memcpy(&v0, &_v, 8); } while (0); goto L_1382a30; }
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B(sp, 196))));
    if ((int64_t)a0 != 0) { do { const a4::q128 _v = (A4_R128(ADD32((uint32_t)fp, 16))); h2 = a4::hi64(_v); std::memcpy(&v0, &_v, 8); } while (0); goto L_1382a30; }
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)fp, 4294967280))));
    (a1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(sp, 160))));
    vf5 = a4v::castsi128_ps(A4_R128(ADD32((uint32_t)fp, 0)));
    vf6 = a4v::castsi128_ps(A4_R128(ADD32((uint32_t)v0, 0)));
    { a4::vec4 res = a4v::sub(vf5, vf6); a4::q128 mask = a4v::set_epi32(0, -1, -1, -1); vf5 = a4v::blend(vf5, res, a4v::castsi128_ps(mask)); }
    A4_W128(ADD32((uint32_t)a1, 0), a4v::castps_si128(vf5));
    (a2 = (uint64_t)((uint64_t)RC_U64X(a1) + (uint64_t)0ULL));
    vf5 = a4v::castsi128_ps(A4_R128(ADD32((uint32_t)fp, 0)));
    vf6 = a4v::castsi128_ps(A4_R128(ADD32((uint32_t)a2, 0)));
    { a4::vec4 res = a4v::add(vf5, vf6); a4::q128 mask = a4v::set_epi32(0, -1, -1, -1); vf5 = a4v::blend(vf5, res, a4v::castsi128_ps(mask)); }
    A4_W128(ADD32(sp, 0), a4v::castps_si128(vf5));
    goto L_1382a34;
L_1382a30:;
    A4_W128(ADD32(sp, 0), a4::mk128(h2, v0));
L_1382a34:;
    (at = (uint32_t)((int32_t)((int32_t)((uint32_t)16256 << 16))));
    { uint32_t bits = a7_opq(at); std::memcpy(&f0, &bits, sizeof(bits)); }
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(sp, 32))));
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(sp, 96))));
    A1_W32B(sp, 12, a1_bits(f0));
    vf8 = a4v::castsi128_ps(A4_R128(ADD32(sp, 0)));
    vf14 = a4v::castsi128_ps(A4_R128(ADD32((uint32_t)v1, 0)));
    vf15 = a4v::castsi128_ps(A4_R128(ADD32((uint32_t)v1, 16)));
    vf16 = a4v::castsi128_ps(A4_R128(ADD32((uint32_t)v1, 32)));
    vf17 = a4v::castsi128_ps(A4_R128(ADD32((uint32_t)v1, 48)));
    { a4::vec4 res = a4v::mul(vf14, a4v::bcast<0>(vf8)); vacc = a4v::dest<1, 1, 1, 1>(vacc, res); }
    { a4::vec4 mul_res = a4v::mul(vf15, a4v::bcast<1>(vf8)); a4::vec4 res = a4v::addK<1, 1>(vacc, mul_res); vacc = a4v::dest<1, 1, 1, 1>(vacc, res); }
    { a4::vec4 mul_res = a4v::mul(vf16, a4v::bcast<2>(vf8)); a4::vec4 res = a4v::addK<1, 1>(vacc, mul_res); vacc = a4v::dest<1, 1, 1, 1>(vacc, res); }
    { a4::vec4 mul_res = a4v::mul(vf17, a4v::bcast<3>(vf8)); a4::vec4 res = a4v::addK<1, 1>(vacc, mul_res); a4::q128 mask = a4v::set_epi32(-1, -1, -1, -1); vf10 = a4v::blendv_ps(vf10, res, a4v::castsi128_ps(mask)); }
    A4_W128(ADD32((uint32_t)v0, 0), a4v::castps_si128(vf10));
    { uint32_t bits = A1_R32B(sp, 40); float f; std::memcpy(&f, &bits, sizeof(f)); f22 = f; }
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(sp, 64))));
    A1_W32B(sp, 40, 0u);
    (a1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(sp, 16))));
    (a2 = (uint64_t)((uint64_t)RC_U64X(v0) + (uint64_t)0ULL));
    vf5 = a4v::castsi128_ps(A4_R128(ADD32((uint32_t)a1, 0)));
    vf6 = a4v::castsi128_ps(A4_R128(ADD32((uint32_t)a2, 0)));
    { a4::vec4 res = a4v::sub(vf5, vf6); a4::q128 mask = a4v::set_epi32(0, -1, -1, -1); vf5 = a4v::blend(vf5, res, a4v::castsi128_ps(mask)); }
    A4_W128(ADD32((uint32_t)a0, 0), a4v::castps_si128(vf5));
    (v0 = (uint64_t)((uint64_t)RC_U64X(a0) + (uint64_t)0ULL));
    vf5 = a4v::castsi128_ps(A4_R128(ADD32((uint32_t)v0, 0)));
    { a4::vec4 res = a4v::mul(vf5, vf5); a4::q128 mask = a4v::set_epi32(0, -1, -1, -1); vf7 = a4v::blend(vf7, res, a4v::castsi128_ps(mask)); }
    { a4::vec4 res = a4v::mulK<1, 1>(vf0, a4v::bcast<0>(vf7)); vacc = a4v::dest<0, 0, 0, 1>(vacc, res); }
    { a4::vec4 mul_res = a4v::mulK<1, 1>(vf0, a4v::bcast<1>(vf7)); a4::vec4 res = a4v::addK<1, 1>(vacc, mul_res); vacc = a4v::dest<0, 0, 0, 1>(vacc, res); }
    { a4::vec4 mul_res = a4v::mulK<1, 1>(vf0, a4v::bcast<2>(vf7)); a4::vec4 res = a4v::addK<1, 1>(vacc, mul_res); a4::q128 mask = a4v::set_epi32(-1, 0, 0, 0); vf7 = a4v::blendv_ps(vf7, res, a4v::castsi128_ps(mask)); }
    { float fs = a4v::cvtss_f32(a4v::shuffle_ps<0x03>(vf0, vf0)); float ft = a4v::cvtss_f32(a4v::shuffle_ps<0x03>(vf7, vf7)); vq = a4v::rsqrt(vstat, fs, ft); }
    { a4::vec4 res = a4v::mul(vf5, a4v::set1_ps(vq)); a4::q128 mask = a4v::set_epi32(0, -1, -1, -1); vf5 = a4v::blendv_ps(vf5, res, a4v::castsi128_ps(mask)); }
    A4_W128(ADD32((uint32_t)v0, 0), a4v::castps_si128(vf5));
    { const uint32_t _dynInsn = A1_R32(0x1382ABCu); const int32_t _dynSimm = (int16_t)(_dynInsn & 0xFFFFu); const uint32_t _dynImm = _dynInsn & 0xFFFFu; (void)_dynSimm; (void)_dynImm; (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)_dynImm << 16)))); }
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(sp, 80))));
    { const uint32_t _dynInsn = A1_R32(0x1382AC4u); const int32_t _dynSimm = (int16_t)(_dynInsn & 0xFFFFu); const uint32_t _dynImm = _dynInsn & 0xFFFFu; (void)_dynSimm; (void)_dynImm; (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v1, _dynSimm)))); }
    (a1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(sp, 64))));
    vf5 = a4v::castsi128_ps(A4_R128(ADD32((uint32_t)v0, 0)));
    vf6 = a4v::castsi128_ps(A4_R128(ADD32((uint32_t)a1, 0)));
    { a4::vec4 fs_yzx = a4v::shuffle_ps<0xC9>(vf5, vf5); a4::vec4 ft_zxy = a4v::shuffle_ps<0xD2>(vf6, vf6); a4::vec4 res = a4v::mul(fs_yzx, ft_zxy); vacc = a4v::dest<1, 1, 1, 0>(vacc, res); }
    { a4::vec4 fs_yzx = a4v::shuffle_ps<0xC9>(vf6, vf6); a4::vec4 ft_zxy = a4v::shuffle_ps<0xD2>(vf5, vf5); a4::vec4 mul_res = a4v::mul(fs_yzx, ft_zxy); a4::vec4 res = a4v::subK<1, 1>(vacc, mul_res); a4::q128 mask = a4v::set_epi32(0, -1, -1, -1); vf7 = a4v::blendv_ps(vf7, res, a4v::castsi128_ps(mask)); }
    { a4::vec4 res = a4v::sub(vf7, vf7); a4::q128 mask = a4v::set_epi32(-1, 0, 0, 0); vf7 = a4v::blend(vf7, res, a4v::castsi128_ps(mask)); }
    A4_W128(ADD32((uint32_t)a0, 0), a4v::castps_si128(vf7));
    (at = (uint32_t)((int32_t)((int32_t)((uint32_t)16128 << 16))));
    { uint32_t bits = a7_opq(at); std::memcpy(&f1, &bits, sizeof(bits)); }
    { uint32_t bits = A1_R32B((uint32_t)fp, 12); float f; std::memcpy(&f, &bits, sizeof(f)); f0 = f; }
    (a2 = (uint64_t)((uint64_t)RC_U64X(a0) + (uint64_t)0ULL));
    f0 = a1f::mul(f0, f1);
    { uint32_t bits; std::memcpy(&bits, &f0, sizeof(bits)); (v0 = (uint64_t)((int64_t)(int32_t)(bits))); }
    vf5 = a4v::castsi128_ps(A4_R128(ADD32((uint32_t)a2, 0)));
    vf6 = a4v::castsi128_ps(a4::mk128(h2, v0));
    { a4::vec4 res = a4v::mul(vf5, a4v::bcast<0>(vf6)); a4::q128 mask = a4v::set_epi32(-1, -1, -1, -1); vf5 = a4v::blendv_ps(vf5, res, a4v::castsi128_ps(mask)); }
    A4_W128(ADD32((uint32_t)a2, 0), a4v::castps_si128(vf5));
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B(sp, 204))));
    { const bool c = (int64_t)v1 != 0; (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B(sp, 204)))); if (c) goto L_1382d00; }
    { const bool c = (int64_t)s7 == 0; (s6 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL)); if (c) goto L_1382d00; }
    { uint32_t bits = 0u; std::memcpy(&f20, &bits, sizeof(bits)); }
L_1382b28:;
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B(sp, 192))));
    (a1 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B(sp, 200))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)a0, 832))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v0, 4294967294))));
    (v0 = (uint64_t)(((int64_t)(int64_t)RC_U64X(a1) < (int64_t)(int64_t)RC_U64X(v0)) ? 1 : 0));
    { const bool c = (int64_t)v0 != 0; (a2 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B(sp, 192)))); if (c) goto L_1382c00; }
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B(sp, 200))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)a2, 152))));
    (v0 = (uint64_t)(((int64_t)(int64_t)RC_U64X(v0) < (int64_t)(int64_t)RC_U64X(v1)) ? 1 : 0));
    if ((int64_t)v0 != 0) goto L_1382c00;
    { (a0 = (uint64_t)((uint64_t)s5 + (uint64_t)0ULL)); A3_CHARGE(); v0 = GetMeshClass_u10(ram, rt, a0, h2); }
    (a0 = (uint64_t)((uint64_t)s5 + (uint64_t)0ULL));
    { (s2 = (uint64_t)((uint64_t)v0 + (uint64_t)0ULL)); A3_CHARGE(); v0 = GetMeshClass_u10(ram, rt, a0, h2); }
    (a0 = (uint64_t)((uint64_t)s5 + (uint64_t)0ULL));
    { (s3 = (uint64_t)((uint64_t)v0 + (uint64_t)0ULL)); A3_CHARGE(); v0 = GetMeshClass_u10(ram, rt, a0, h2); }
    (a0 = (uint64_t)((uint64_t)s7 + (uint64_t)0ULL));
    { (s4 = (uint64_t)((uint64_t)v0 + (uint64_t)0ULL)); A3_CHARGE(); v0 = GetMeshClass_u10(ram, rt, a0, h2); }
    (a0 = (uint64_t)((uint64_t)s7 + (uint64_t)0ULL));
    { (s1 = (uint64_t)((uint64_t)v0 + (uint64_t)0ULL)); A3_CHARGE(); v0 = GetMeshClass_u10(ram, rt, a0, h2); }
    (a0 = (uint64_t)((uint64_t)s7 + (uint64_t)0ULL));
    { (s0 = (uint64_t)((uint64_t)v0 + (uint64_t)0ULL)); A3_CHARGE(); v0 = GetMeshClass_u10(ram, rt, a0, h2); }
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)v0, 12))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)s0, 16))));
    (a1 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)s4, 12))));
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)SLL32((uint32_t)a0, 6))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)SLL32((uint32_t)v0, 2))));
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)a0, 16))));
    (s1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)s1, (uint32_t)v0))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B(sp, 208))));
    (a2 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)s3, 16))));
    (a1 = (uint64_t)((int64_t)(int32_t)((int32_t)SLL32((uint32_t)a1, 6))));
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)v0, 12))));
    { int64_t result = (int64_t)(int32_t)s6 * (int64_t)(int32_t)a0; lo = (uint64_t)(int64_t)(int32_t)result; hi = (uint64_t)(int64_t)(int32_t)(result >> 32); (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)result))); }
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)s1, 0))));
    (a1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)a1, 16))));
    (a2 = (uint64_t)((int64_t)(int32_t)((int32_t)SLL32((uint32_t)a2, 2))));
    { int64_t result = (int64_t)(int32_t)s6 * (int64_t)(int32_t)a1; lo = (uint64_t)(int64_t)(int32_t)result; hi = (uint64_t)(int64_t)(int32_t)(result >> 32); (a1 = (uint64_t)((int64_t)(int32_t)((int32_t)result))); }
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)SLL32((uint32_t)v1, 6))));
    (s2 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)s2, (uint32_t)a2))));
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v1, (uint32_t)v0))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)s2, 0))));
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v1, (uint32_t)a0))));
    { uint32_t bits = A1_R32B((uint32_t)v1, 172); float f; std::memcpy(&f, &bits, sizeof(f)); f0 = f; }
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v0, (uint32_t)a1))));
    { float f = f0; uint32_t bits; std::memcpy(&bits, &f, sizeof(bits)); A1_W32B((uint32_t)v0, 236, bits); }
    goto L_1382c48;
L_1382c00:;
    { (a0 = (uint64_t)((uint64_t)s5 + (uint64_t)0ULL)); A3_CHARGE(); v0 = GetMeshClass_u10(ram, rt, a0, h2); }
    (a0 = (uint64_t)((uint64_t)s5 + (uint64_t)0ULL));
    { (s1 = (uint64_t)((uint64_t)v0 + (uint64_t)0ULL)); A3_CHARGE(); v0 = GetMeshClass_u10(ram, rt, a0, h2); }
    (a0 = (uint64_t)((uint64_t)s5 + (uint64_t)0ULL));
    { (s0 = (uint64_t)((uint64_t)v0 + (uint64_t)0ULL)); A3_CHARGE(); v0 = GetMeshClass_u10(ram, rt, a0, h2); }
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)s0, 16))));
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)v0, 12))));
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)SLL32((uint32_t)a0, 2))));
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)SLL32((uint32_t)v1, 6))));
    (s1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)s1, (uint32_t)a0))));
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v1, 16))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)s1, 0))));
    { int64_t result = (int64_t)(int32_t)s6 * (int64_t)(int32_t)v1; lo = (uint64_t)(int64_t)(int32_t)result; hi = (uint64_t)(int64_t)(int32_t)(result >> 32); (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)result))); }
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v0, (uint32_t)v1))));
    { float f = f20; uint32_t bits; std::memcpy(&bits, &f, sizeof(bits)); A1_W32B((uint32_t)v0, 236, bits); }
L_1382c48:;
    { (a0 = (uint64_t)((uint64_t)s5 + (uint64_t)0ULL)); A3_CHARGE(); v0 = GetMeshClass_u10(ram, rt, a0, h2); }
    (a0 = (uint64_t)((uint64_t)s5 + (uint64_t)0ULL));
    { (s2 = (uint64_t)((uint64_t)v0 + (uint64_t)0ULL)); A3_CHARGE(); v0 = GetMeshClass_u10(ram, rt, a0, h2); }
    (a0 = (uint64_t)((uint64_t)s5 + (uint64_t)0ULL));
    { (s3 = (uint64_t)((uint64_t)v0 + (uint64_t)0ULL)); A3_CHARGE(); v0 = GetMeshClass_u10(ram, rt, a0, h2); }
    (a0 = (uint64_t)((uint64_t)s7 + (uint64_t)0ULL));
    { (s4 = (uint64_t)((uint64_t)v0 + (uint64_t)0ULL)); A3_CHARGE(); v0 = GetMeshClass_u10(ram, rt, a0, h2); }
    (a0 = (uint64_t)((uint64_t)s7 + (uint64_t)0ULL));
    { (s1 = (uint64_t)((uint64_t)v0 + (uint64_t)0ULL)); A3_CHARGE(); v0 = GetMeshClass_u10(ram, rt, a0, h2); }
    (a0 = (uint64_t)((uint64_t)s7 + (uint64_t)0ULL));
    { (s0 = (uint64_t)((uint64_t)v0 + (uint64_t)0ULL)); A3_CHARGE(); v0 = GetMeshClass_u10(ram, rt, a0, h2); }
    (a1 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)v0, 12))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)s0, 16))));
    (a2 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)s4, 12))));
    (a1 = (uint64_t)((int64_t)(int32_t)((int32_t)SLL32((uint32_t)a1, 6))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)SLL32((uint32_t)v0, 2))));
    (a1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)a1, 16))));
    (s1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)s1, (uint32_t)v0))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B(sp, 208))));
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)s3, 16))));
    { int64_t result = (int64_t)(int32_t)s6 * (int64_t)(int32_t)a1; lo = (uint64_t)(int64_t)(int32_t)result; hi = (uint64_t)(int64_t)(int32_t)(result >> 32); (a1 = (uint64_t)((int64_t)(int32_t)((int32_t)result))); }
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)v0, 12))));
    (a2 = (uint64_t)((int64_t)(int32_t)((int32_t)SLL32((uint32_t)a2, 6))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)s1, 0))));
    (a2 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)a2, 16))));
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)SLL32((uint32_t)a0, 2))));
    { int64_t result = (int64_t)(int32_t)s6 * (int64_t)(int32_t)a2; lo = (uint64_t)(int64_t)(int32_t)result; hi = (uint64_t)(int64_t)(int32_t)(result >> 32); (a2 = (uint64_t)((int64_t)(int32_t)((int32_t)result))); }
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)SLL32((uint32_t)v1, 6))));
    (s2 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)s2, (uint32_t)a0))));
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v1, (uint32_t)v0))));
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)s2, 0))));
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v1, (uint32_t)a1))));
    (s6 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)s6, 1))));
    do { const a4::q128 _v = (A4_R128(ADD32((uint32_t)v1, 112))); h2 = a4::hi64(_v); std::memcpy(&v0, &_v, 8); } while (0);
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)a0, (uint32_t)a2))));
    (a3 = (uint64_t)(((int64_t)(int64_t)RC_U64X(s6) < (int64_t)(int32_t)2) ? 1 : 0));
    { const bool c = (int64_t)a3 != 0; A4_W128(ADD32((uint32_t)a0, 176), a4::mk128(h2, v0)); if (c) { A3_BACK(); goto L_1382b28; } }
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B(sp, 204))));
    goto L_1382f10;
L_1382d00:;
    (s6 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL));
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B(sp, 200))));
    { uint32_t bits = 0u; std::memcpy(&f20, &bits, sizeof(bits)); }
    (s2 = (uint64_t)((int64_t)(int32_t)((int32_t)SLL32((uint32_t)a0, 6))));
    (s4 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B(sp, 220))));
    (s3 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v1, (uint32_t)a0))));
L_1382d18:;
    (a1 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B(sp, 192))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)a1, 832))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v0, 4294967294))));
    (v0 = (uint64_t)(((int64_t)(int64_t)s3 < (int64_t)(int64_t)RC_U64X(v0)) ? 1 : 0));
    { const bool c = (int64_t)v0 != 0; (a2 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B(sp, 192)))); if (c) goto L_1382d98; }
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)a2, 152))));
    (v0 = (uint64_t)(((int64_t)(int64_t)RC_U64X(v0) < (int64_t)(int64_t)s3) ? 1 : 0));
    if ((int64_t)v0 != 0) goto L_1382d98;
    { (a0 = (uint64_t)((uint64_t)s5 + (uint64_t)0ULL)); A3_CHARGE(); v0 = GetMeshClass_u10(ram, rt, a0, h2); }
    (a0 = (uint64_t)((uint64_t)s5 + (uint64_t)0ULL));
    { (s1 = (uint64_t)((uint64_t)v0 + (uint64_t)0ULL)); A3_CHARGE(); v0 = GetMeshClass_u10(ram, rt, a0, h2); }
    (a0 = (uint64_t)((uint64_t)s5 + (uint64_t)0ULL));
    { (s0 = (uint64_t)((uint64_t)v0 + (uint64_t)0ULL)); A3_CHARGE(); v0 = GetMeshClass_u10(ram, rt, a0, h2); }
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)s0, 16))));
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)v0, 12))));
    { uint32_t bits = A1_R32B((uint32_t)s4, 0); float f; std::memcpy(&f, &bits, sizeof(f)); f0 = f; }
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)SLL32((uint32_t)a0, 2))));
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)SLL32((uint32_t)v1, 6))));
    (s1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)s1, (uint32_t)a0))));
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v1, 16))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)s1, 0))));
    { int64_t result = (int64_t)(int32_t)s6 * (int64_t)(int32_t)v1; lo = (uint64_t)(int64_t)(int32_t)result; hi = (uint64_t)(int64_t)(int32_t)(result >> 32); (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)result))); }
    f0 = a1f::mul(f0, f23);
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)s2, (uint32_t)v0))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v0, (uint32_t)v1))));
    { float f = f0; uint32_t bits; std::memcpy(&bits, &f, sizeof(bits)); A1_W32B((uint32_t)v0, 236, bits); }
    goto L_1382de4;
L_1382d98:;
    { (a0 = (uint64_t)((uint64_t)s5 + (uint64_t)0ULL)); A3_CHARGE(); v0 = GetMeshClass_u10(ram, rt, a0, h2); }
    (a0 = (uint64_t)((uint64_t)s5 + (uint64_t)0ULL));
    { (s1 = (uint64_t)((uint64_t)v0 + (uint64_t)0ULL)); A3_CHARGE(); v0 = GetMeshClass_u10(ram, rt, a0, h2); }
    (a0 = (uint64_t)((uint64_t)s5 + (uint64_t)0ULL));
    { (s0 = (uint64_t)((uint64_t)v0 + (uint64_t)0ULL)); A3_CHARGE(); v0 = GetMeshClass_u10(ram, rt, a0, h2); }
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)s0, 16))));
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)v0, 12))));
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)SLL32((uint32_t)a0, 2))));
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)SLL32((uint32_t)v1, 6))));
    (s1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)s1, (uint32_t)a0))));
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v1, 16))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)s1, 0))));
    { int64_t result = (int64_t)(int32_t)s6 * (int64_t)(int32_t)v1; lo = (uint64_t)(int64_t)(int32_t)result; hi = (uint64_t)(int64_t)(int32_t)(result >> 32); (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)result))); }
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)s2, (uint32_t)v0))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v0, (uint32_t)v1))));
    { float f = f20; uint32_t bits; std::memcpy(&bits, &f, sizeof(bits)); A1_W32B((uint32_t)v0, 236, bits); }
L_1382de4:;
    if ((int64_t)s6 != 0) goto L_1382e48;
    { (a0 = (uint64_t)((uint64_t)s5 + (uint64_t)0ULL)); A3_CHARGE(); v0 = GetMeshClass_u10(ram, rt, a0, h2); }
    (a0 = (uint64_t)((uint64_t)s5 + (uint64_t)0ULL));
    { (s0 = (uint64_t)((uint64_t)v0 + (uint64_t)0ULL)); A3_CHARGE(); v0 = GetMeshClass_u10(ram, rt, a0, h2); }
    (s1 = (uint64_t)((uint64_t)v0 + (uint64_t)0ULL));
    { (a0 = (uint64_t)((uint64_t)s5 + (uint64_t)0ULL)); A3_CHARGE(); v0 = GetMeshClass_u10(ram, rt, a0, h2); }
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)s1, 16))));
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(sp, 80))));
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)SLL32((uint32_t)v1, 2))));
    (s0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)s0, (uint32_t)v1))));
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(sp, 16))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)s0, 0))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)s2, (uint32_t)v0))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v0, 176))));
    vf5 = a4v::castsi128_ps(A4_R128(ADD32((uint32_t)v1, 0)));
    vf6 = a4v::castsi128_ps(A4_R128(ADD32((uint32_t)a0, 0)));
    { a4::vec4 res = a4v::add(vf5, vf6); a4::q128 mask = a4v::set_epi32(0, -1, -1, -1); vf5 = a4v::blend(vf5, res, a4v::castsi128_ps(mask)); }
    A4_W128(ADD32((uint32_t)v0, 0), a4v::castps_si128(vf5));
    goto L_1382eac;
L_1382e48:;
    { (a0 = (uint64_t)((uint64_t)s5 + (uint64_t)0ULL)); A3_CHARGE(); v0 = GetMeshClass_u10(ram, rt, a0, h2); }
    (a0 = (uint64_t)((uint64_t)s5 + (uint64_t)0ULL));
    { (s1 = (uint64_t)((uint64_t)v0 + (uint64_t)0ULL)); A3_CHARGE(); v0 = GetMeshClass_u10(ram, rt, a0, h2); }
    (a0 = (uint64_t)((uint64_t)s5 + (uint64_t)0ULL));
    { (s0 = (uint64_t)((uint64_t)v0 + (uint64_t)0ULL)); A3_CHARGE(); v0 = GetMeshClass_u10(ram, rt, a0, h2); }
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)s0, 16))));
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)v0, 12))));
    (a1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(sp, 16))));
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)SLL32((uint32_t)a0, 2))));
    (a2 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(sp, 80))));
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)SLL32((uint32_t)v1, 6))));
    (s1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)s1, (uint32_t)a0))));
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v1, 16))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)s1, 0))));
    { int64_t result = (int64_t)(int32_t)s6 * (int64_t)(int32_t)v1; lo = (uint64_t)(int64_t)(int32_t)result; hi = (uint64_t)(int64_t)(int32_t)(result >> 32); (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)result))); }
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)s2, (uint32_t)v0))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v0, (uint32_t)v1))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v0, 176))));
    vf5 = a4v::castsi128_ps(A4_R128(ADD32((uint32_t)a1, 0)));
    vf6 = a4v::castsi128_ps(A4_R128(ADD32((uint32_t)a2, 0)));
    { a4::vec4 res = a4v::sub(vf5, vf6); a4::q128 mask = a4v::set_epi32(0, -1, -1, -1); vf5 = a4v::blend(vf5, res, a4v::castsi128_ps(mask)); }
    A4_W128(ADD32((uint32_t)v0, 0), a4v::castps_si128(vf5));
L_1382eac:;
    { (a0 = (uint64_t)((uint64_t)s5 + (uint64_t)0ULL)); A3_CHARGE(); v0 = GetMeshClass_u10(ram, rt, a0, h2); }
    (a0 = (uint64_t)((uint64_t)s5 + (uint64_t)0ULL));
    { (s1 = (uint64_t)((uint64_t)v0 + (uint64_t)0ULL)); A3_CHARGE(); v0 = GetMeshClass_u10(ram, rt, a0, h2); }
    (a0 = (uint64_t)((uint64_t)s5 + (uint64_t)0ULL));
    { (s0 = (uint64_t)((uint64_t)v0 + (uint64_t)0ULL)); A3_CHARGE(); v0 = GetMeshClass_u10(ram, rt, a0, h2); }
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)s0, 16))));
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)v0, 12))));
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)SLL32((uint32_t)a0, 2))));
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)SLL32((uint32_t)v1, 6))));
    (s1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)s1, (uint32_t)a0))));
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v1, 16))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)s1, 0))));
    { int64_t result = (int64_t)(int32_t)s6 * (int64_t)(int32_t)v1; lo = (uint64_t)(int64_t)(int32_t)result; hi = (uint64_t)(int64_t)(int32_t)(result >> 32); (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)result))); }
    (s6 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)s6, 1))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)s2, (uint32_t)v0))));
    (a0 = (uint64_t)(((int64_t)(int64_t)RC_U64X(s6) < (int64_t)(int32_t)2) ? 1 : 0));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v0, (uint32_t)v1))));
    { const bool c = (int64_t)a0 != 0; { float f = f21; uint32_t bits; std::memcpy(&bits, &f, sizeof(bits)); A1_W32B((uint32_t)v0, 184, bits); } if (c) { A3_BACK(); goto L_1382d18; } }
    do { const a4::q128 _v = (A4_R128(ADD32(sp, 80))); h2 = a4::hi64(_v); std::memcpy(&v0, &_v, 8); } while (0);
    A4_W128(ADD32(sp, 176), a4::mk128(h2, v0));
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B(sp, 204))));
L_1382f10:;
    f21 = ((float)(f22));
    do { const a4::q128 _v = (A4_R128(ADD32(sp, 16))); h2 = a4::hi64(_v); std::memcpy(&v0, &_v, 8); } while (0);
    (fp = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)fp, 16))));
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v1, 1))));
    (a2 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B(sp, 212))));
    A1_W32B(sp, 204, (uint32_t)v1);
    A4_W128(ADD32(sp, 48), a4::mk128(h2, v0));
    (a1 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B(sp, 204))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B(sp, 220))));
    do { const a4::q128 _v = (A4_R128(ADD32(sp, 32))); h3 = a4::hi64(_v); std::memcpy(&v1, &_v, 8); } while (0);
    (a0 = (uint64_t)(((int64_t)(int64_t)RC_U64X(a1) < (int64_t)(int64_t)RC_U64X(a2)) ? 1 : 0));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v0, 4))));
    A4_W128(ADD32(sp, 16), a4::mk128(h3, v1));
    A1_W32B(sp, 220, (uint32_t)v0);
    { const bool c = (int64_t)a0 != 0; A1_W32B(sp, 24, 0u); if (c) { A3_BACK(); goto L_13829e0; } }
L_1382f50:;
    { (a0 = (uint64_t)((uint64_t)s5 + (uint64_t)0ULL)); A3_CHARGE(); v0 = GetMeshClass_u10(ram, rt, a0, h2); }
    (a0 = (uint64_t)((uint64_t)s5 + (uint64_t)0ULL));
    { (s0 = (uint64_t)((uint64_t)v0 + (uint64_t)0ULL)); A3_CHARGE(); v0 = GetMeshClass_u10(ram, rt, a0, h2); }
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)v0, 16))));
    { const uint32_t _dynInsn = A1_R32(0x1382F68u); const int32_t _dynSimm = (int16_t)(_dynInsn & 0xFFFFu); const uint32_t _dynImm = _dynInsn & 0xFFFFu; (void)_dynSimm; (void)_dynImm; (a1 = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)_dynImm << 16)))); }
    { const uint32_t _dynInsn = A1_R32(0x1382F6Cu); const int32_t _dynSimm = (int16_t)(_dynInsn & 0xFFFFu); const uint32_t _dynImm = _dynInsn & 0xFFFFu; (void)_dynSimm; (void)_dynImm; (a1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)a1, _dynSimm)))); }
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)SLL32((uint32_t)v1, 2))));
    (s0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)s0, (uint32_t)v1))));
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)s0, 0))));
    { (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)a0, 64)))); A3_CHARGE(); uint64_t a7c[3] = {0u, 0u, 0u}; iosCopyMatrix_x10(ram, rt, (uint32_t)a0, (uint32_t)a1, h6, h7, h8, h9, a7c); a3 = a7c[0]; t0 = a7c[1]; t1 = a7c[2]; }
    { uint32_t bits = A1_R32B(sp, 328); float f; std::memcpy(&f, &bits, sizeof(f)); f23 = f; }
    { uint32_t bits = A1_R32B(sp, 320); float f; std::memcpy(&f, &bits, sizeof(f)); f22 = f; }
    { uint32_t bits = A1_R32B(sp, 312); float f; std::memcpy(&f, &bits, sizeof(f)); f21 = f; }
    { uint32_t bits = A1_R32B(sp, 304); float f; std::memcpy(&f, &bits, sizeof(f)); f20 = f; }
    { sp = sp + 0x150u; if (xo) { xo[0] = a1_bits(f20); xo[1] = a1_bits(f21); xo[2] = a1_bits(f22); xo[3] = a1_bits(f23); xo[4] = a1; xo[5] = a3; xo[6] = t0; xo[7] = t1; } io_vf5 = vf5; io_vf6 = vf6; io_vf7 = vf7; io_vf8 = vf8; io_vf10 = vf10; io_vf14 = vf14; io_vf15 = vf15; io_vf16 = vf16; io_vf17 = vf17; io_vacc = vacc; io_vq = vq; io_vstat = vstat; io_h2 = h2; io_h3 = h3; io_h6 = h6; io_h7 = h7; io_h8 = h8; io_h9 = h9; return a1::RetVF{(uint64_t)v0, f0}; }
    __builtin_unreachable();
}

// SelectOmakeItem (0x1400a20): typed by tools/typed_lift2.py (round A6; loop frame calls; VU0 in h5, out h2 h3 h4, relocated calls 6, guest sp, frame 80 bytes, 0 slot accesses)
inline uint64_t SelectOmakeItem(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint64_t a0, uint64_t a1, uint64_t a3, uint32_t sp, float f12, float f13, float f14, float f15, float f16, float f17, float f18, float f19, uint64_t &io_h2, uint64_t &io_h3, uint64_t &io_h4, uint64_t io_h5)
{
    uint64_t v0 = 0;
    uint64_t v1 = 0;
    uint64_t a2 = 0;
    uint64_t s0 = 0;
    uint64_t s1 = 0;
    uint64_t s2 = 0;
    uint64_t s3 = 0;
    uint64_t s4 = 0;
    uint64_t s5 = 0;
    uint64_t s6 = 0;
    uint32_t s7 = 0;
    uint32_t fp = 0;
    uint64_t h2 = io_h2;
    uint64_t h3 = io_h3;
    uint64_t h4 = io_h4;
    uint64_t h5 = io_h5;
    { const uint32_t _dynInsn = A1_R32(0x1400A20u); const int32_t _dynSimm = (int16_t)(_dynInsn & 0xFFFFu); const uint32_t _dynImm = _dynInsn & 0xFFFFu; (void)_dynSimm; (void)_dynImm; (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)_dynImm << 16)))); }
    { const uint32_t _dynInsn = A1_R32(0x1400A24u); const int32_t _dynSimm = (int16_t)(_dynInsn & 0xFFFFu); const uint32_t _dynImm = _dynInsn & 0xFFFFu; (void)_dynSimm; (void)_dynImm; (a2 = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)_dynImm << 16)))); }
    { const uint32_t _dynInsn = A1_R32(0x1400A28u); const int32_t _dynSimm = (int16_t)(_dynInsn & 0xFFFFu); const uint32_t _dynImm = _dynInsn & 0xFFFFu; (void)_dynSimm; (void)_dynImm; (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32(ADD32((uint32_t)v0, _dynSimm))))); }
    { const uint32_t _dynInsn = A1_R32(0x1400A2Cu); const int32_t _dynSimm = (int16_t)(_dynInsn & 0xFFFFu); const uint32_t _dynImm = _dynInsn & 0xFFFFu; (void)_dynSimm; (void)_dynImm; (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)a2, _dynSimm)))); }
    (s4 = (uint64_t)((uint64_t)a0 + (uint64_t)0ULL));
    (v1 = (uint64_t)(RC_U64X(v1) ^ RC_U64X(v0)));
    (s6 = (uint64_t)((uint64_t)a1 + (uint64_t)0ULL));
    fp = 0x1470000u;
    if (RC_U64X(v1) == 0) do { const a4::q128 _v = (a4v::setzero_si128()); h2 = a4::hi64(_v); std::memcpy(&v0, &_v, 8); } while (0);
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(fp, 4294950200))));
    (s0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)v0, 12))));
    { (s1 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)v0, 16)))); A3_CHARGE(); v0 = MakeStrHashValue(ram, rt, v0, a0, f12, f13, f14, f15, f16, f17, f18, f19, h2); }
    { const bool c = (int32_t)s0 <= 0; (s5 = (uint64_t)((uint64_t)v0 + (uint64_t)0ULL)); if (c) goto L_1400b30; }
    (s2 = (uint64_t)((uint64_t)s1 + (uint64_t)0ULL));
    (s3 = (uint64_t)((uint64_t)s0 + (uint64_t)0ULL));
    s7 = 0x1470000u;
L_1400a98:;
    { A3_CHARGE(); v0 = a1_sx64(getGameHardMode(ram, rt)); }
    { const bool c = (int64_t)v0 == 0; (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(s7, 4294950192)))); if (c) goto L_1400ab0; }
    (s0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)s2, 8))));
    goto L_1400ab4;
L_1400ab0:;
    (s0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)s2, 4))));
L_1400ab4:;
    { A3_CHARGE(); v0 = MakeStrHashValue(ram, rt, v0, a0, f12, f13, f14, f15, f16, f17, f18, f19, h2); }
    (a0 = (uint64_t)((uint64_t)s4 + (uint64_t)0ULL));
    (a1 = (uint64_t)((uint64_t)s5 + (uint64_t)0ULL));
    { const bool c = (int64_t)s0 == (int64_t)v0; (a2 = (uint64_t)((uint64_t)s0 + (uint64_t)0ULL)); if (c) goto L_1400b20; }
    { A3_CHARGE(); v0 = SpriteMenuGetMenuIdByHash_u10(ram, rt, a0, a1, a2, h4); }
    if ((int64_t)s6 != (int64_t)v0) { (s3 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)s3, 4294967295)))); goto L_1400b24; }
    (s1 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)s2, 24))));
    { (a0 = (uint64_t)((uint64_t)RC_U64X(s1) + (uint64_t)0ULL)); A3_CHARGE(); v0 = _omakeSelectFlagChk_u10(ram, rt, a0, h3, h5); }
    { const bool c = (int64_t)v0 == 0; (a0 = (uint64_t)((uint64_t)s1 + (uint64_t)0ULL)); if (c) goto L_1400b18; }
    { (s0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)s2, 28)))); A3_CHARGE(); v0 = a1_sx64(getSelectWeapon(ram, rt)); }
    (s0 = (uint64_t)((int64_t)(int32_t)((int32_t)SRL32((uint32_t)s0, 18))));
    (s0 = (uint64_t)(RC_U64X(s0) & (uint64_t)(uint16_t)4095));
    { const bool c = (int64_t)s0 == (int64_t)v0; (a0 = (uint64_t)((uint64_t)s1 + (uint64_t)0ULL)); if (c) goto L_1400b20; }
    { (s3 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)s3, 4294967295)))); A3_CHARGE(); v0 = _omakeSelectFlagCls(ram, rt, a0, a1_sx64((uint32_t)(sp + 0xFFFFFFB0u)), h3, h5); }
    (s2 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)s2, 32))));
    goto L_1400b28;
L_1400b18:;
    { A3_CHARGE(); v0 = _omakeSelectFlagSet(ram, rt, a0, a1_sx64((uint32_t)(sp + 0xFFFFFFB0u)), h3, h5); }
L_1400b20:;
    (s3 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)s3, 4294967295))));
L_1400b24:;
    (s2 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)s2, 32))));
L_1400b28:;
    if ((int64_t)s3 != 0) { A3_BACK(); goto L_1400a98; }
L_1400b30:;
    { const uint32_t _dynInsn = A1_R32(0x1400B30u); const int32_t _dynSimm = (int16_t)(_dynInsn & 0xFFFFu); const uint32_t _dynImm = _dynInsn & 0xFFFFu; (void)_dynSimm; (void)_dynImm; (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)_dynImm << 16)))); }
    { const uint32_t _dynInsn = A1_R32(0x1400B34u); const int32_t _dynSimm = (int16_t)(_dynInsn & 0xFFFFu); const uint32_t _dynImm = _dynInsn & 0xFFFFu; (void)_dynSimm; (void)_dynImm; (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)_dynImm << 16)))); }
    { const uint32_t _dynInsn = A1_R32(0x1400B38u); const int32_t _dynSimm = (int16_t)(_dynInsn & 0xFFFFu); const uint32_t _dynImm = _dynInsn & 0xFFFFu; (void)_dynSimm; (void)_dynImm; (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32(ADD32((uint32_t)v0, _dynSimm))))); }
    { const uint32_t _dynInsn = A1_R32(0x1400B3Cu); const int32_t _dynSimm = (int16_t)(_dynInsn & 0xFFFFu); const uint32_t _dynImm = _dynInsn & 0xFFFFu; (void)_dynSimm; (void)_dynImm; (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)a0, _dynSimm)))); }
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(fp, 4294950200))));
    (v1 = (uint64_t)(RC_U64X(v1) ^ RC_U64X(v0)));
    if (RC_U64X(v1) == 0) do { const a4::q128 _v = (a4v::setzero_si128()); h2 = a4::hi64(_v); std::memcpy(&v0, &_v, 8); } while (0);
    (s0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)v0, 12))));
    { (s1 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)v0, 16)))); A3_CHARGE(); v0 = MakeStrHashValue(ram, rt, v0, a0, f12, f13, f14, f15, f16, f17, f18, f19, h2); }
    { const bool c = (int32_t)s0 <= 0; (s3 = (uint64_t)((uint64_t)v0 + (uint64_t)0ULL)); if (c) goto L_1400bc4; }
    (s1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)s1, 24))));
    (s2 = (uint64_t)((uint64_t)s0 + (uint64_t)0ULL));
    s7 = 0x1470000u;
L_1400b70:;
    { A3_CHARGE(); v0 = a1_sx64(getGameHardMode(ram, rt)); }
    { const bool c = (int64_t)v0 == 0; (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(s7, 4294950192)))); if (c) goto L_1400b88; }
    (s0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)s1, -16))));
    goto L_1400b8c;
L_1400b88:;
    (s0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)s1, -20))));
L_1400b8c:;
    { A3_CHARGE(); v0 = MakeStrHashValue(ram, rt, v0, a0, f12, f13, f14, f15, f16, f17, f18, f19, h2); }
    if ((int64_t)s0 == (int64_t)v0) { (s2 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)s2, 4294967295)))); goto L_1400bbc; }
    { (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)s1, 0)))); A3_CHARGE(); v0 = _omakeSelectFlagChk_u10(ram, rt, a0, h3, h5); }
    (a0 = (uint64_t)((uint64_t)s4 + (uint64_t)0ULL));
    (a1 = (uint64_t)((uint64_t)s3 + (uint64_t)0ULL));
    (a2 = (uint64_t)((uint64_t)s0 + (uint64_t)0ULL));
    { (a3 = (uint64_t)((uint64_t)v0 + (uint64_t)0ULL)); A3_CHARGE(); v0 = SpriteMenuSetCheckBoxByHash(ram, rt, a0, a1, a2, a3, h4); }
    (s2 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)s2, 4294967295))));
L_1400bbc:;
    { const bool c = (int64_t)s2 != 0; (s1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)s1, 32)))); if (c) { A3_BACK(); goto L_1400b70; } }
L_1400bc4:;
    { io_h2 = h2; io_h3 = h3; io_h4 = h4; return v0; }
    __builtin_unreachable();
}

// IsParentCharOrLwsOrient (0x10cc628): typed by tools/typed_lift2.py (round A6; loop frame calls, extra outputs xo[0]=a1 xo[1]=a2 xo[2]=a3; VU0 in -, out h2 h7, frame 16 bytes, 0 slot accesses)
inline uint64_t IsParentCharOrLwsOrient(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint64_t v0, uint64_t a0, uint64_t a1, float f0, float f1, float f2, float f3, float f4, float f5, float f6, float f7, float f8, float f9, float f10, float f11, float f12, float f13, float f14, float f15, float f16, float f17, float f18, float f19, float f20, float f21, float f22, float f23, float f24, float f25, float f26, float f27, float f28, float f29, float f30, float f31, uint64_t &io_h2, uint64_t &io_h7, uint64_t *xo = nullptr)
{
    uint32_t v1 = 0;
    uint32_t a2 = 0;
    uint64_t a3 = 0;
    uint64_t s0 = 0;
    uint64_t h2 = io_h2;
    uint64_t h7 = io_h7;
    bool a3xw1 = false;
    { (s0 = (uint64_t)((uint64_t)a0 + (uint64_t)0ULL)); A3_CHARGE(); v0 = GetScrLwsOriClass_u10(ram, rt, a0, h2); }
    { const bool c = (int64_t)v0 == 0; (a3 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL)); if (c) goto L_10cc6a0; }
    (a2 = (uint32_t)((int32_t)((int32_t)A1_R32B((uint32_t)v0, 4600)))); a3xw1 = true;
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)v0, 4608))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B(a2, 48))));
    { const bool c = (int32_t)v0 <= 0; (a1 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL)); if (c) goto L_10cc6b4; }
    (v1 = (uint32_t)((int32_t)((int32_t)A1_R32B((uint32_t)a0, 0))));
    goto L_10cc674;
L_10cc660:;
    (a1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)a1, 1))));
    (v0 = (uint64_t)(((int64_t)(int64_t)RC_U64X(a1) < (int64_t)(int64_t)v0) ? 1 : 0));
    { const bool c = (int64_t)v0 == 0; (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)a0, 80)))); if (c) goto L_10cc6b4; }
    (v1 = (uint32_t)((int32_t)((int32_t)A1_R32B((uint32_t)a0, 0))));
L_10cc674:;
    if (v1 == 0) { (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B(a2, 48)))); { A3_BACK(); goto L_10cc660; } }
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B(v1, 324))));
    if ((int64_t)v0 == 0) { (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B(a2, 48)))); { A3_BACK(); goto L_10cc660; } }
    (a3 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(v1, 32))));
    goto L_10cc6b4;
L_10cc6a0:;
    { (a0 = (uint64_t)((uint64_t)s0 + (uint64_t)0ULL)); A3_CHARGE(); uint64_t a7c[3] = {0u, 0u, a1}; v0 = GetLayoutClass_u10(ram, rt, a0, f0, f1, f2, f3, f4, f5, f6, f7, f8, f9, f10, f11, f12, f13, f14, f15, f16, f17, f18, f19, f20, f21, f22, f23, f24, f25, f26, f27, f28, f29, f30, f31, h2, a7c); a1 = a7c[2]; }
    (a3 = (uint64_t)((uint64_t)v0 + (uint64_t)0ULL));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)a3, 292))));
    if (RC_U64X(v0) == 0) do { const a4::q128 _v = (a4v::setzero_si128()); h7 = a4::hi64(_v); std::memcpy(&a3, &_v, 8); } while (0);
L_10cc6b4:;
    (v0 = (uint64_t)((uint64_t)a3 + (uint64_t)0ULL));
    { if (xo) { xo[0] = a1; if (a3xw1) xo[1] = a1_sx64(a2); xo[2] = a3; } io_h2 = h2; io_h7 = h7; return v0; }
    __builtin_unreachable();
}

// AddLayoutObjSubLink (0x10f2178): typed by tools/typed_lift2.py (round A6; frame calls; VU0 in -, out h2, frame 32 bytes, 0 slot accesses)
inline uint64_t AddLayoutObjSubLink(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint64_t v0, uint64_t a0, uint64_t a1, float f0, float f1, float f2, float f3, float f4, float f5, float f6, float f7, float f8, float f9, float f10, float f11, float f12, float f13, float f14, float f15, float f16, float f17, float f18, float f19, float f20, float f21, float f22, float f23, float f24, float f25, float f26, float f27, float f28, float f29, float f30, float f31, uint64_t &io_h2)
{
    uint64_t s0 = 0;
    uint64_t s1 = 0;
    uint64_t h2 = io_h2;
    { (s0 = (uint64_t)((uint64_t)a1 + (uint64_t)0ULL)); A3_CHARGE(); v0 = GetLayoutClass_u10(ram, rt, a0, f0, f1, f2, f3, f4, f5, f6, f7, f8, f9, f10, f11, f12, f13, f14, f15, f16, f17, f18, f19, f20, f21, f22, f23, f24, f25, f26, f27, f28, f29, f30, f31, h2); }
    (a0 = (uint64_t)((uint64_t)s0 + (uint64_t)0ULL));
    { (s1 = (uint64_t)((uint64_t)v0 + (uint64_t)0ULL)); A3_CHARGE(); v0 = GetSubLinkClass_u10(ram, rt, a0, h2); }
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)s1, 268))));
    (a1 = (uint64_t)((uint64_t)v0 + (uint64_t)0ULL));
    { const uint32_t a4_t = AddTwoWayLinkNext(ram, rt, (uint32_t)a0, (uint32_t)a1); io_h2 = h2; return a1_sx64(a4_t); }
    __builtin_unreachable();
}
