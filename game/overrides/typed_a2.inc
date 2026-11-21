// typed_a2.inc: round A2's typed functions (tools/typed_lift2.py). Generated: do not edit. Included inside a namespace (typed_a2.h: namespace typed), after typed_a1.inc.
// memprintf (0x103ff8): typed by tools/typed_lift2.py (round A2; frame leaf, frame 88 bytes, 8 slot accesses)
inline void memprintf(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, float f12, float f13, float f14, float f15, float f16, float f17, float f18, float f19)
{
    uint32_t a1 = 0;
    uint32_t a2 = 0;
    uint32_t a3 = 0;
    uint32_t t0 = 0;
    uint32_t t1 = 0;
    uint32_t t2 = 0;
    uint32_t t3 = 0;
    uint8_t a2fr[88];
    a2::frSt<uint32_t>(a2fr, 28, a1_bits(f19));
    a2::frSt<uint32_t>(a2fr, 0, a1_bits(f12));
    a2::frSt<uint32_t>(a2fr, 4, a1_bits(f13));
    a2::frSt<uint32_t>(a2fr, 8, a1_bits(f14));
    a2::frSt<uint32_t>(a2fr, 12, a1_bits(f15));
    a2::frSt<uint32_t>(a2fr, 16, a1_bits(f16));
    a2::frSt<uint32_t>(a2fr, 20, a1_bits(f17));
    a2::frSt<uint32_t>(a2fr, 24, a1_bits(f18));
    { return; }
    __builtin_unreachable();
}

// imemprintf (0x104040): typed by tools/typed_lift2.py (round A2; frame leaf, frame 88 bytes, 8 slot accesses)
inline void imemprintf(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, float f12, float f13, float f14, float f15, float f16, float f17, float f18, float f19)
{
    uint32_t a1 = 0;
    uint32_t a2 = 0;
    uint32_t a3 = 0;
    uint32_t t0 = 0;
    uint32_t t1 = 0;
    uint32_t t2 = 0;
    uint32_t t3 = 0;
    uint8_t a2fr[88];
    a2::frSt<uint32_t>(a2fr, 28, a1_bits(f19));
    a2::frSt<uint32_t>(a2fr, 0, a1_bits(f12));
    a2::frSt<uint32_t>(a2fr, 4, a1_bits(f13));
    a2::frSt<uint32_t>(a2fr, 8, a1_bits(f14));
    a2::frSt<uint32_t>(a2fr, 12, a1_bits(f15));
    a2::frSt<uint32_t>(a2fr, 16, a1_bits(f16));
    a2::frSt<uint32_t>(a2fr, 20, a1_bits(f17));
    a2::frSt<uint32_t>(a2fr, 24, a1_bits(f18));
    { return; }
    __builtin_unreachable();
}

// PutString (0x105948): typed by tools/typed_lift2.py (round A2; frame leaf, frame 80 bytes, 8 slot accesses)
inline void PutString(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, float f12, float f13, float f14, float f15, float f16, float f17, float f18, float f19)
{
    uint32_t a2 = 0;
    uint32_t a3 = 0;
    uint32_t t0 = 0;
    uint32_t t1 = 0;
    uint32_t t2 = 0;
    uint32_t t3 = 0;
    uint8_t a2fr[80];
    a2::frSt<uint32_t>(a2fr, 28, a1_bits(f19));
    a2::frSt<uint32_t>(a2fr, 0, a1_bits(f12));
    a2::frSt<uint32_t>(a2fr, 4, a1_bits(f13));
    a2::frSt<uint32_t>(a2fr, 8, a1_bits(f14));
    a2::frSt<uint32_t>(a2fr, 12, a1_bits(f15));
    a2::frSt<uint32_t>(a2fr, 16, a1_bits(f16));
    a2::frSt<uint32_t>(a2fr, 20, a1_bits(f17));
    a2::frSt<uint32_t>(a2fr, 24, a1_bits(f18));
    { return; }
    __builtin_unreachable();
}

// PutStringS (0x105990): typed by tools/typed_lift2.py (round A2; frame leaf, frame 80 bytes, 8 slot accesses)
inline void PutStringS(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, float f12, float f13, float f14, float f15, float f16, float f17, float f18, float f19)
{
    uint32_t a2 = 0;
    uint32_t a3 = 0;
    uint32_t t0 = 0;
    uint32_t t1 = 0;
    uint32_t t2 = 0;
    uint32_t t3 = 0;
    uint8_t a2fr[80];
    a2::frSt<uint32_t>(a2fr, 28, a1_bits(f19));
    a2::frSt<uint32_t>(a2fr, 0, a1_bits(f12));
    a2::frSt<uint32_t>(a2fr, 4, a1_bits(f13));
    a2::frSt<uint32_t>(a2fr, 8, a1_bits(f14));
    a2::frSt<uint32_t>(a2fr, 12, a1_bits(f15));
    a2::frSt<uint32_t>(a2fr, 16, a1_bits(f16));
    a2::frSt<uint32_t>(a2fr, 20, a1_bits(f17));
    a2::frSt<uint32_t>(a2fr, 24, a1_bits(f18));
    { return; }
    __builtin_unreachable();
}

// sub_00108D10 (0x108d10): typed by tools/typed_lift2.py (round A2; leaf)
inline uint64_t sub_00108D10(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint64_t a0, uint32_t a1, uint64_t a2)
{
    uint64_t v0 = 0;
    uint32_t v1 = 0;
    uint64_t a3 = 0;
    v1 = A1_R32(a1);
    { const bool c = v1 != 0; (a3 = (uint64_t)((uint64_t)a0 + (uint64_t)0ULL)); if (c) goto L_108d24; }
    { (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 1)))); return v0; }
L_108d24:;
    (v0 = (uint64_t)(((int64_t)(int64_t)a2 < (int64_t)(int32_t)256) ? 1 : 0));
    if ((int64_t)v0 == 0) { (v1 = (uint32_t)((int32_t)((int32_t)A1_R32B((uint32_t)a3, 0)))); goto L_108d5c; }
    (v0 = (uint64_t)(a1_sx64(v1) ^ (uint64_t)(uint16_t)1));
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)a3, 0))));
    if (v0 == 0) a2 = (uint64_t)0;;
    A1_W8B((uint32_t)a0, 0, (uint8_t)(uint32_t)a2);
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 1))));
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)a0, 1))));
    v1 = A1_R32(a1);
    A1_W32B((uint32_t)a3, 0, (uint32_t)a0);
    v1 = v1 - 1u;
    { A1_W32(a1, v1); return v0; }
L_108d5c:;
    (v0 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL));
    { A1_W8(v1, (uint8_t)0u); return v0; }
    __builtin_unreachable();
}

// sub_00116C58 (0x116c58): typed by tools/typed_lift2.py (round A2; frame leaf, frame 56 bytes, 0 slot accesses)
inline void sub_00116C58(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt)
{
    uint32_t a1 = 0;
    uint32_t a2 = 0;
    uint32_t a3 = 0;
    uint32_t t0 = 0;
    uint32_t t1 = 0;
    uint32_t t2 = 0;
    uint32_t t3 = 0;
    { return; }
    __builtin_unreachable();
}

// ps2___fpcmp_parts_d (0x127270): typed by tools/typed_lift2.py (round A2; leaf)
inline uint64_t ps2___fpcmp_parts_d(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint64_t a0, uint32_t a1)
{
    uint64_t v0 = 0;
    uint64_t v1 = 0;
    uint32_t a2 = 0;
    uint32_t a3 = 0;
    (a2 = (uint32_t)((int32_t)((int32_t)A1_R32B((uint32_t)a0, 0))));
    (v0 = (uint64_t)(((uint64_t)a1_sx64(a2) < (uint64_t)(int64_t)(int32_t)2) ? 1 : 0));
    if ((int64_t)v0 != 0) goto L_127290;
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B(a1, 0))));
    (v0 = (uint64_t)(((uint64_t)RC_U64X(v1) < (uint64_t)(int64_t)(int32_t)2) ? 1 : 0));
    if ((int64_t)v0 == 0) { (v0 = (uint64_t)(a1_sx64(a2) ^ (uint64_t)(uint16_t)4)); goto L_127298; }
L_127290:;
    { (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 1)))); return v0; }
L_127298:;
    { const bool c = (int64_t)v0 != 0; (v0 = (uint64_t)(v1 ^ (uint64_t)(uint16_t)4)); if (c) goto L_1272b8; }
    if ((int64_t)v0 != 0) { (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)a0, 4)))); goto L_12730c; }
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B(a1, 4))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)a0, 4))));
    { (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)SUB32((uint32_t)v1, (uint32_t)v0)))); return v0; }
L_1272b8:;
    if ((int64_t)v0 != 0) { (v0 = (uint64_t)(a1_sx64(a2) ^ (uint64_t)(uint16_t)2)); goto L_1272d8; }
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B(a1, 4))));
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 4294967295))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 1))));
    { if (RC_U64X(a0) == 0) v0 = (uint64_t)(v1);; return v0; }
L_1272d8:;
    if ((int64_t)v0 != 0) { (v0 = (uint64_t)(v1 ^ (uint64_t)(uint16_t)2)); goto L_127300; }
    (v1 = (uint64_t)(v1 ^ (uint64_t)(uint16_t)2));
    { const bool c = (int64_t)v1 == 0; (v0 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL)); if (c) goto L_127388; }
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B(a1, 4))));
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 1))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 4294967295))));
    { if (RC_U64X(a0) != 0) v0 = (uint64_t)(v1);; return v0; }
L_127300:;
    if ((int64_t)v0 != 0) { (a3 = (uint32_t)((int32_t)((int32_t)A1_R32B((uint32_t)a0, 4)))); goto L_127320; }
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)a0, 4))));
L_12730c:;
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 1))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 4294967295))));
    { if (a0 == 0) v0 = (uint64_t)(v1);; return v0; }
L_127320:;
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B(a1, 4))));
    { const bool c = (int64_t)(int32_t)a3 != (int64_t)v0; (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 1)))); if (c) goto L_127364; }
    (a2 = (uint32_t)((int32_t)((int32_t)A1_R32B((uint32_t)a0, 8))));
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B(a1, 8))));
    (v0 = (uint64_t)(((int64_t)(int64_t)RC_U64X(v1) < (int64_t)(int64_t)a1_sx64(a2)) ? 1 : 0));
    if ((int64_t)v0 != 0) { (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 1)))); goto L_127364; }
    (v0 = (uint64_t)(((int64_t)(int64_t)a1_sx64(a2) < (int64_t)(int64_t)RC_U64X(v1)) ? 1 : 0));
    { const bool c = (int64_t)v0 != 0; (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 4294967295)))); if (c) goto L_12737c; }
    (v1 = (uint64_t)(A1_R64B((uint32_t)a0, 16)));
    (a0 = (uint64_t)(A1_R64B(a1, 16)));
    (v0 = (uint64_t)(((uint64_t)a0 < (uint64_t)v1) ? 1 : 0));
    if ((int64_t)v0 == 0) { (v0 = (uint64_t)(((uint64_t)v1 < (uint64_t)a0) ? 1 : 0)); goto L_127370; }
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 1))));
L_127364:;
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 4294967295))));
    { if (a1_sx64(a3) == 0) v0 = (uint64_t)(v1);; return v0; }
L_127370:;
    if ((int64_t)v0 == 0) { (v0 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL)); goto L_127388; }
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 4294967295))));
L_12737c:;
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 1))));
    { if (a1_sx64(a3) == 0) v0 = (uint64_t)(v1);; return v0; }
L_127388:;
    { return v0; }
    __builtin_unreachable();
}

// iosCdStSectorAlignSkip (0x1acf60): typed by tools/typed_lift2.py (round A2; leaf)
inline uint64_t iosCdStSectorAlignSkip(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt)
{
    uint64_t v0 = 0;
    uint64_t v1 = 0;
    uint64_t a0 = 0;
    uint64_t a1 = 0;
    uint64_t a2 = 0;
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)32 << 16))));
    (a2 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v0, 4294953040))));
    (a1 = (uint64_t)(A1_R64B((uint32_t)a2, 0)));
    (v1 = (uint64_t)(((int64_t)(int64_t)a1 < (int64_t)(int32_t)0) ? 1 : 0));
    (a0 = (uint64_t)((int64_t)(int64_t)a1 + (int64_t)(int32_t)2047));
    (v0 = (uint64_t)((uint64_t)a1 + (uint64_t)0ULL));
    if (RC_U64X(v1) != 0) v0 = (uint64_t)(a0);;
    (v0 = (uint64_t)((int64_t)v0 >> 11));
    (v0 = (uint64_t)(v0 << 11));
    (v0 = (uint64_t)(a1 - v0));
    (v1 = (uint64_t)(v0 << (32 + 0)));
    (v1 = (uint64_t)((int64_t)v1 >> (32 + 0)));
    { const bool c = (int64_t)v1 == 0; (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 2048)))); if (c) goto L_1acfac; }
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)SUB32((uint32_t)v0, (uint32_t)v1))));
    (v0 = (uint64_t)(RC_U64X(v0) << (32 + 0)));
    (v0 = (uint64_t)(v0 >> (32 + 0)));
    (v0 = (uint64_t)((uint64_t)a1 + (uint64_t)v0));
    A1_W64B((uint32_t)a2, 0, v0);
L_1acfac:;
    { return v0; }
    __builtin_unreachable();
}

// iosGetTCount (0x1ad510): typed by tools/typed_lift2.py (round A2; leaf)
inline uint64_t iosGetTCount(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt)
{
    uint64_t v0 = 0;
    uint32_t v1 = 0;
    v1 = 0x200000u;
    v1 = v1 - 0x2760u;
    (v0 = (uint64_t)(A1_R64B(v1, 0)));
    { return v0; }
    __builtin_unreachable();
}

// iosGetCpuRapCountPar1Int (0x1ad658): typed by tools/typed_lift2.py (round A2; leaf)
inline a1::RetVF iosGetCpuRapCountPar1Int(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt)
{
    float f0 = 0.0f;
    float f1 = 0.0f;
    uint32_t at = 0;
    uint32_t v0 = 0;
    uint32_t v1 = 0;
    uint32_t a0 = 0;
    uint32_t fcr31 = 0;
    v0 = 0x10000000u;
    v0 = v0 | 0x800u;
    v0 = A1_R32(v0);
    v1 = v0 >> 1;
    a0 = v0 & 1u;
    { const bool c = (int32_t)v0 < 0; a0 = a0 | v1; if (c) goto L_1ad688; }
    f0 = a1_flt(v0);
    { int32_t tmp; std::memcpy(&tmp, &f0, sizeof(tmp)); f0 = ((float)(int32_t)(tmp)); }
    goto L_1ad694;
L_1ad688:;
    f0 = a1_flt(a0);
    { int32_t tmp; std::memcpy(&tmp, &f0, sizeof(tmp)); f0 = ((float)(int32_t)(tmp)); }
    f0 = a1f::add(f0, f0);
L_1ad694:;
    at = 0x42700000u;
    f1 = a1_flt(at);
    v1 = 0x1E0000u;
    v0 = A1_R32B(v1, 3028);
    f1 = a1f::mul(f0, f1);
    v0 = v0 << 2;
    at = 0x1E0000u;
    at = at + v0;
    f0 = a1_flt(A1_R32B(at, 3032));
    f0 = a1f::div(fcr31, f1, f0);
    { return a1::RetVF{(uint64_t)a1_sx64(v0), f0}; }
    __builtin_unreachable();
}

// iosSprToMemDmaAdrCopy (0x1b2a80): typed by tools/typed_lift2.py (round A2; leaf)
inline uint32_t iosSprToMemDmaAdrCopy(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t a0, uint32_t a1, uint32_t a2)
{
    uint32_t v0 = 0;
    uint32_t v1 = 0;
    uint32_t a3 = 0;
    uint32_t t0 = 0;
    uint32_t t1 = 0;
    uint32_t t2 = 0;
    uint32_t t3 = 0;
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
    A1_W32(a3, v1);
    v1 = 0x10000000u;
    A1_W32(v0, t3);
    v1 = v1 | 0xD000u;
    A1_W32(t0, a0);
    A1_W32(t1, a1);
    A1_W32(t2, a2);
    v0 = A1_R32(a3);
    v0 = v0 | 0x100u;
    A1_W32(a3, v0);
    A1_W32(v1, t3);
    { return v0; }
    __builtin_unreachable();
}

// kernel_sub_001B4120 (0x1b4120): typed by tools/typed_lift2.py (round A2; frame leaf, frame 88 bytes, 8 slot accesses)
inline uint64_t kernel_sub_001B4120(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, float f12, float f13, float f14, float f15, float f16, float f17, float f18, float f19)
{
    uint64_t v0 = 0;
    uint32_t a1 = 0;
    uint32_t a2 = 0;
    uint32_t a3 = 0;
    uint32_t t0 = 0;
    uint32_t t1 = 0;
    uint32_t t2 = 0;
    uint32_t t3 = 0;
    uint8_t a2fr[88];
    (v0 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL));
    a2::frSt<uint32_t>(a2fr, 28, a1_bits(f19));
    a2::frSt<uint32_t>(a2fr, 0, a1_bits(f12));
    a2::frSt<uint32_t>(a2fr, 4, a1_bits(f13));
    a2::frSt<uint32_t>(a2fr, 8, a1_bits(f14));
    a2::frSt<uint32_t>(a2fr, 12, a1_bits(f15));
    a2::frSt<uint32_t>(a2fr, 16, a1_bits(f16));
    a2::frSt<uint32_t>(a2fr, 20, a1_bits(f17));
    a2::frSt<uint32_t>(a2fr, 24, a1_bits(f18));
    { return v0; }
    __builtin_unreachable();
}

// PopQuaternion (0x1bbc08): typed by tools/typed_lift2.py (round A2; leaf)
inline uint32_t PopQuaternion(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt)
{
    uint32_t v0 = 0;
    uint64_t v1 = 0;
    uint64_t a0 = 0;
    v0 = 0x1E0000u;
    v0 = v0 + 0xF60u;
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B(v0, 0))));
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v1, 4294967295))));
    (a0 = (uint64_t)(((int64_t)(int64_t)RC_U64X(v1) < (int64_t)(int32_t)0) ? 1 : 0));
    if (RC_U64X(a0) != 0) v1 = (uint64_t)0;;
    { A1_W32B(v0, 0, (uint32_t)v1); return v0; }
    __builtin_unreachable();
}

// iosGetACosf (0x1be260): typed by tools/typed_lift2.py (round A2; leaf)
inline float iosGetACosf(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, float f12)
{
    float f0 = 0.0f;
    float f1 = 0.0f;
    float f2 = 0.0f;
    float f3 = 0.0f;
    float f4 = 0.0f;
    float f5 = 0.0f;
    float f6 = 0.0f;
    uint32_t at = 0;
    f3 = a1f::mul(f12, f12);
    at = 0x3E2A0000u;
    at = at | 0xAAAAu;
    f1 = a1_flt(at);
    at = 0x3FC90000u;
    at = at | 0xFDAu;
    f2 = a1_flt(at);
    at = 0x3D990000u;
    at = at | 0x9999u;
    f4 = a1_flt(at);
    f2 = a1f::sub(f2, f12);
    at = 0x3D360000u;
    at = at | 0xDB6Du;
    f5 = a1_flt(at);
    f3 = a1f::mul(f3, f12);
    at = 0x3CF80000u;
    at = at | 0xE38Du;
    f6 = a1_flt(at);
    f0 = a1f::mul(f3, f12);
    f3 = a1f::mul(f3, f1);
    f0 = a1f::mul(f0, f12);
    f2 = a1f::subFinite(f2, f3);
    f1 = a1f::mul(f0, f12);
    f0 = a1f::mul(f0, f4);
    f1 = a1f::mul(f1, f12);
    f2 = a1f::subFinite(f2, f0);
    f0 = a1f::mul(f1, f12);
    f1 = a1f::mul(f1, f5);
    f0 = a1f::mul(f0, f12);
    f2 = a1f::subFinite(f2, f1);
    f0 = a1f::mul(f0, f6);
    { f0 = a1f::sub(f2, f0); return f0; }
    __builtin_unreachable();
}

// iosDlGetCreateAdrs (0x1c0458): typed by tools/typed_lift2.py (round A2; leaf)
inline uint32_t iosDlGetCreateAdrs(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt)
{
    uint32_t v0 = 0;
    uint32_t v1 = 0;
    uint32_t a0 = 0;
    v0 = 0x210000u;
    v1 = 4u;
    v0 = v0 + 0x32D8u;
    a0 = A1_R32B(v0, 24);
    v0 = v0 + 8u;
    if (a1_sx64(a0) != 0) v1 = (uint32_t)((uint64_t)0);;
    v1 = v1 + v0;
    v0 = A1_R32(v1);
    { return v0; }
    __builtin_unreachable();
}

// getVIntrCnt (0x1c7d98): typed by tools/typed_lift2.py (round A2; leaf)
inline uint32_t getVIntrCnt(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt)
{
    uint32_t v0 = 0;
    uint32_t v1 = 0;
    v1 = 0x1E0000u;
    v1 = v1 + 0x1314u;
    v0 = A1_R32(v1);
    { return v0; }
    __builtin_unreachable();
}

// setVIntrBaseTime (0x1c7db0): typed by tools/typed_lift2.py (round A2; leaf)
inline uint32_t setVIntrBaseTime(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t a0)
{
    uint32_t v0 = 0;
    uint32_t v1 = 0;
    v0 = 0x1E0000u;
    v0 = v0 + 0x1314u;
    v1 = A1_R32(v0);
    A1_W32B(a0, 4, 0u);
    { A1_W32(a0, v1); return v0; }
    __builtin_unreachable();
}

// iosGetFontDataWithSJIS (0x1c91f8): typed by tools/typed_lift2.py (round A2; leaf)
inline uint64_t iosGetFontDataWithSJIS(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint64_t a0, uint64_t a1, uint64_t a2)
{
    uint64_t v0 = 0;
    uint64_t v1 = 0;
    uint64_t a3 = 0;
    uint64_t t0 = 0;
    uint64_t t1 = 0;
    uint64_t t2 = 0;
    uint64_t t3 = 0;
    uint64_t t4 = 0;
    uint64_t hi = 0;
    uint64_t lo = 0;
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
    if (RC_U64X(v0) != 0) t0 = (uint64_t)(a0);;
    (v0 = (uint64_t)(((uint64_t)t3 < (uint64_t)(int64_t)(int32_t)63) ? 1 : 0));
    (v1 = (uint64_t)(RC_U64X(v1) ^ (uint64_t)(uint16_t)0));
    (a1 = (uint64_t)((int64_t)(int64_t)RC_U64X(t1) + (int64_t)(int32_t)4294967168));
    if (RC_U64X(v1) != 0) t0 = (uint64_t)(a3);;
    (a2 = (uint64_t)((int64_t)(int32_t)((int32_t)SLL32((uint32_t)a2, 5))));
    (a1 = (uint64_t)(((uint64_t)a1 < (uint64_t)(int64_t)(int32_t)31) ? 1 : 0));
    (t2 = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)39 << 16))));
    (t2 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)t2, 2680))));
    (t2 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)t2, (uint32_t)a2))));
    { const bool c = (int64_t)v0 == 0; (t0 = (uint64_t)(t0 << 1)); if (c) goto L_1c9258; }
    (t1 = (uint64_t)((uint64_t)t3 + (uint64_t)0ULL));
    goto L_1c927c;
L_1c9258:;
    { const bool c = (int64_t)a1 == 0; (v1 = (uint64_t)((int64_t)(int64_t)t1 + (int64_t)(int32_t)4294967137)); if (c) goto L_1c9268; }
    (t1 = (uint64_t)((int64_t)(int64_t)t1 + (int64_t)(int32_t)4294967231));
    goto L_1c927c;
L_1c9268:;
    (v0 = (uint64_t)(((uint64_t)v1 < (uint64_t)(int64_t)(int32_t)94) ? 1 : 0));
    if ((int64_t)v0 == 0) { (v1 = (uint64_t)((int64_t)(int64_t)t0 + (int64_t)(int32_t)1)); goto L_1c9280; }
    (t1 = (uint64_t)((uint64_t)v1 + (uint64_t)0ULL));
    (t0 = (uint64_t)((int64_t)(int64_t)t0 + (int64_t)(int32_t)1));
L_1c927c:;
    (v1 = (uint64_t)((int64_t)(int64_t)t0 + (int64_t)(int32_t)1));
L_1c9280:;
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)t2, 20))));
    (v1 = (uint64_t)(v1 << 8));
    (a1 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)t2, 16))));
    (v1 = (uint64_t)((uint64_t)v1 + (uint64_t)t1));
    (a2 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)t2, 8))));
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
    { const bool c = (int64_t)t4 == 0; (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)a2, (uint32_t)v0)))); if (c) goto L_1c9314; }
    { uint32_t addr = ADD32((uint32_t)t2, 7); uint32_t aligned_addr = addr & ~7u; uint32_t offset = addr & 7u; uint64_t mem = A1_R64(aligned_addr); uint32_t shift = (7u - offset) << 3; uint64_t keepMask = (shift == 0) ? 0ull : ((1ull << shift) - 1ull); (v1 = (uint64_t)((RC_U64X(v1) & keepMask) | (mem << shift))); }
    { uint32_t addr = ADD32((uint32_t)t2, 0); uint32_t aligned_addr = addr & ~7u; uint32_t offset = addr & 7u; uint64_t mem = A1_R64(aligned_addr); uint32_t shift = offset << 3; uint64_t keepMask = (offset == 0) ? 0ull : (0xFFFFFFFFFFFFFFFFull << ((8u - offset) << 3)); (v1 = (uint64_t)((v1 & keepMask) | (mem >> shift))); }
    { uint32_t addr = ADD32((uint32_t)t2, 15); uint32_t aligned_addr = addr & ~7u; uint32_t offset = addr & 7u; uint64_t mem = A1_R64(aligned_addr); uint32_t shift = (7u - offset) << 3; uint64_t keepMask = (shift == 0) ? 0ull : ((1ull << shift) - 1ull); (a0 = (uint64_t)((RC_U64X(a0) & keepMask) | (mem << shift))); }
    { uint32_t addr = ADD32((uint32_t)t2, 8); uint32_t aligned_addr = addr & ~7u; uint32_t offset = addr & 7u; uint64_t mem = A1_R64(aligned_addr); uint32_t shift = offset << 3; uint64_t keepMask = (offset == 0) ? 0ull : (0xFFFFFFFFFFFFFFFFull << ((8u - offset) << 3)); (a0 = (uint64_t)((a0 & keepMask) | (mem >> shift))); }
    { uint32_t addr = ADD32((uint32_t)t2, 23); uint32_t aligned_addr = addr & ~7u; uint32_t offset = addr & 7u; uint64_t mem = A1_R64(aligned_addr); uint32_t shift = (7u - offset) << 3; uint64_t keepMask = (shift == 0) ? 0ull : ((1ull << shift) - 1ull); (a1 = (uint64_t)((RC_U64X(a1) & keepMask) | (mem << shift))); }
    { uint32_t addr = ADD32((uint32_t)t2, 16); uint32_t aligned_addr = addr & ~7u; uint32_t offset = addr & 7u; uint64_t mem = A1_R64(aligned_addr); uint32_t shift = offset << 3; uint64_t keepMask = (offset == 0) ? 0ull : (0xFFFFFFFFFFFFFFFFull << ((8u - offset) << 3)); (a1 = (uint64_t)((a1 & keepMask) | (mem >> shift))); }
    { uint32_t addr = ADD32((uint32_t)t2, 31); uint32_t aligned_addr = addr & ~7u; uint32_t offset = addr & 7u; uint64_t mem = A1_R64(aligned_addr); uint32_t shift = (7u - offset) << 3; uint64_t keepMask = (shift == 0) ? 0ull : ((1ull << shift) - 1ull); (a2 = (uint64_t)((RC_U64X(a2) & keepMask) | (mem << shift))); }
    { uint32_t addr = ADD32((uint32_t)t2, 24); uint32_t aligned_addr = addr & ~7u; uint32_t offset = addr & 7u; uint64_t mem = A1_R64(aligned_addr); uint32_t shift = offset << 3; uint64_t keepMask = (offset == 0) ? 0ull : (0xFFFFFFFFFFFFFFFFull << ((8u - offset) << 3)); (a2 = (uint64_t)((a2 & keepMask) | (mem >> shift))); }
    { uint32_t addr = ADD32((uint32_t)t4, 7); uint32_t aligned_addr = addr & ~7u; uint32_t offset = addr & 7u; uint32_t shift = (7u - offset) << 3; uint64_t mask = 0xFFFFFFFFFFFFFFFFull >> shift; uint64_t old_data = A1_R64(aligned_addr); uint64_t val = v1; uint64_t new_data = (old_data & ~mask) | ((val >> shift) & mask); A1_W64(aligned_addr, new_data); }
    { uint32_t addr = ADD32((uint32_t)t4, 0); uint32_t aligned_addr = addr & ~7u; uint32_t offset = addr & 7u; uint32_t shift = offset << 3; uint64_t mask = 0xFFFFFFFFFFFFFFFFull << shift; uint64_t old_data = A1_R64(aligned_addr); uint64_t val = v1; uint64_t new_data = (old_data & ~mask) | ((val << shift) & mask); A1_W64(aligned_addr, new_data); }
    { uint32_t addr = ADD32((uint32_t)t4, 15); uint32_t aligned_addr = addr & ~7u; uint32_t offset = addr & 7u; uint32_t shift = (7u - offset) << 3; uint64_t mask = 0xFFFFFFFFFFFFFFFFull >> shift; uint64_t old_data = A1_R64(aligned_addr); uint64_t val = a0; uint64_t new_data = (old_data & ~mask) | ((val >> shift) & mask); A1_W64(aligned_addr, new_data); }
    { uint32_t addr = ADD32((uint32_t)t4, 8); uint32_t aligned_addr = addr & ~7u; uint32_t offset = addr & 7u; uint32_t shift = offset << 3; uint64_t mask = 0xFFFFFFFFFFFFFFFFull << shift; uint64_t old_data = A1_R64(aligned_addr); uint64_t val = a0; uint64_t new_data = (old_data & ~mask) | ((val << shift) & mask); A1_W64(aligned_addr, new_data); }
    { uint32_t addr = ADD32((uint32_t)t4, 23); uint32_t aligned_addr = addr & ~7u; uint32_t offset = addr & 7u; uint32_t shift = (7u - offset) << 3; uint64_t mask = 0xFFFFFFFFFFFFFFFFull >> shift; uint64_t old_data = A1_R64(aligned_addr); uint64_t val = a1; uint64_t new_data = (old_data & ~mask) | ((val >> shift) & mask); A1_W64(aligned_addr, new_data); }
    { uint32_t addr = ADD32((uint32_t)t4, 16); uint32_t aligned_addr = addr & ~7u; uint32_t offset = addr & 7u; uint32_t shift = offset << 3; uint64_t mask = 0xFFFFFFFFFFFFFFFFull << shift; uint64_t old_data = A1_R64(aligned_addr); uint64_t val = a1; uint64_t new_data = (old_data & ~mask) | ((val << shift) & mask); A1_W64(aligned_addr, new_data); }
    { uint32_t addr = ADD32((uint32_t)t4, 31); uint32_t aligned_addr = addr & ~7u; uint32_t offset = addr & 7u; uint32_t shift = (7u - offset) << 3; uint64_t mask = 0xFFFFFFFFFFFFFFFFull >> shift; uint64_t old_data = A1_R64(aligned_addr); uint64_t val = a2; uint64_t new_data = (old_data & ~mask) | ((val >> shift) & mask); A1_W64(aligned_addr, new_data); }
    { uint32_t addr = ADD32((uint32_t)t4, 24); uint32_t aligned_addr = addr & ~7u; uint32_t offset = addr & 7u; uint32_t shift = offset << 3; uint64_t mask = 0xFFFFFFFFFFFFFFFFull << shift; uint64_t old_data = A1_R64(aligned_addr); uint64_t val = a2; uint64_t new_data = (old_data & ~mask) | ((val << shift) & mask); A1_W64(aligned_addr, new_data); }
L_1c9314:;
    { return v0; }
    __builtin_unreachable();
}

// iosGetFontIDWithSJIS (0x1c9498): typed by tools/typed_lift2.py (round A2; leaf)
inline uint64_t iosGetFontIDWithSJIS(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint64_t a0)
{
    uint64_t v0 = 0;
    uint64_t v1 = 0;
    uint64_t a1 = 0;
    uint64_t a2 = 0;
    uint64_t a3 = 0;
    uint64_t t0 = 0;
    uint64_t t1 = 0;
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
    if (RC_U64X(v0) != 0) a3 = (uint64_t)(a0);;
    (v0 = (uint64_t)(((uint64_t)t1 < (uint64_t)(int64_t)(int32_t)63) ? 1 : 0));
    (a1 = (uint64_t)((int64_t)(int64_t)RC_U64X(t0) + (int64_t)(int32_t)4294967168));
    if (RC_U64X(v1) != 0) a3 = (uint64_t)(a2);;
    (a1 = (uint64_t)(((uint64_t)a1 < (uint64_t)(int64_t)(int32_t)31) ? 1 : 0));
    { const bool c = (int64_t)v0 == 0; (a3 = (uint64_t)(a3 << 1)); if (c) goto L_1c94e8; }
    (t0 = (uint64_t)((uint64_t)t1 + (uint64_t)0ULL));
    goto L_1c950c;
L_1c94e8:;
    if ((int64_t)a1 == 0) { (v1 = (uint64_t)((int64_t)(int64_t)t0 + (int64_t)(int32_t)4294967137)); goto L_1c94f8; }
    (t0 = (uint64_t)((int64_t)(int64_t)t0 + (int64_t)(int32_t)4294967231));
    goto L_1c950c;
L_1c94f8:;
    (v0 = (uint64_t)(((uint64_t)v1 < (uint64_t)(int64_t)(int32_t)94) ? 1 : 0));
    if ((int64_t)v0 == 0) { (v1 = (uint64_t)((int64_t)(int64_t)a3 + (int64_t)(int32_t)1)); goto L_1c9510; }
    (t0 = (uint64_t)((uint64_t)v1 + (uint64_t)0ULL));
    (a3 = (uint64_t)((int64_t)(int64_t)a3 + (int64_t)(int32_t)1));
L_1c950c:;
    (v1 = (uint64_t)((int64_t)(int64_t)a3 + (int64_t)(int32_t)1));
L_1c9510:;
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
    { (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v0, 4294964161)))); return v0; }
    __builtin_unreachable();
}

// kernel_sub_001D1420 (0x1d1420): typed by tools/typed_lift2.py (round A2; leaf)
inline uint32_t kernel_sub_001D1420(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt)
{
    uint32_t v0 = 0;
    uint64_t v1 = 0;
    uint32_t a0 = 0;
    uint32_t a1 = 0;
    uint64_t a2 = 0;
    uint32_t a3 = 0;
    v0 = 0x270000u;
    a0 = 0x270000u;
    a1 = A1_R32B(v0, 16076);
    a3 = A1_R32B(a0, 16080);
    (v1 = (uint64_t)(uint32_t)((uint8_t)A1_R8B(a1, 2)));
    a0 = A1_R8(a1);
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)SLL32((uint32_t)v1, 1))));
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v1, 4294967168))));
    a0 = a0 & 0xFu;
    (a2 = (uint64_t)(RC_U64X(v1) | (uint64_t)(uint16_t)256));
    (a1 = (uint32_t)(((int64_t)(int64_t)RC_U64X(v1) < (int64_t)(int32_t)0) ? 1 : 0));
    v0 = a0 << 3;
    if (a1_sx64(a1) != 0) v1 = (uint64_t)(a2);;
    v0 = v0 - a0;
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)SLL32((uint32_t)v1, 6))));
    v0 = v0 << 4;
    (v1 = (uint64_t)(RC_U64X(v1) & (uint64_t)(uint16_t)32704));
    v0 = v0 + a3;
    { A1_W16B(v0, 784, (uint16_t)(uint32_t)v1); return v0; }
    __builtin_unreachable();
}

// kernel_sub_001D15B8 (0x1d15b8): typed by tools/typed_lift2.py (round A2; frame leaf, frame 16 bytes, 9 slot accesses)
inline uint64_t kernel_sub_001D15B8(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt)
{
    uint64_t v0 = 0;
    uint32_t v1 = 0;
    uint64_t a0 = 0;
    uint32_t a1 = 0;
    uint64_t a2 = 0;
    uint8_t a2fr[16];
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)39 << 16))));
    (v1 = (uint32_t)((int32_t)((int32_t)A1_R32B((uint32_t)v0, 16076))));
    a1 = A1_R8B(v1, 2);
    (v0 = (uint64_t)(uint32_t)((uint8_t)A1_R8B(v1, 0)));
    (a0 = (uint64_t)(((uint64_t)a1_sx64(a1) < (uint64_t)(int64_t)(int32_t)4) ? 1 : 0));
    { const bool c = (int64_t)a0 != 0; (a2 = (uint64_t)(RC_U64X(v0) & (uint64_t)(uint16_t)15)); if (c) goto L_1d15e8; }
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 4294967295))));
    a2::frSt<uint16_t>(a2fr, 0, (uint16_t)(uint32_t)v0);
    a2::frSt<uint16_t>(a2fr, 2, (uint16_t)(uint32_t)v0);
    goto L_1d1624;
L_1d15e8:;
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 2))));
    if ((int64_t)(int32_t)a1 != (int64_t)v0) { v1 = 1u; goto L_1d1608; }
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 1))));
    v1 = 0xFFFFFFFFu;
    a2::frSt<uint16_t>(a2fr, 0, (uint16_t)(uint32_t)v0);
    goto L_1d1620;
L_1d1608:;
    if (a1 != v1) { a2::frSt<uint16_t>(a2fr, 0, (uint16_t)v1); goto L_1d1620; }
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 4294967295))));
    a2::frSt<uint16_t>(a2fr, 2, (uint16_t)v1);
    a2::frSt<uint16_t>(a2fr, 0, (uint16_t)(uint32_t)v0);
    goto L_1d1624;
L_1d1620:;
    a2::frSt<uint16_t>(a2fr, 2, (uint16_t)v1);
L_1d1624:;
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)39 << 16))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)SLL32((uint32_t)a2, 3))));
    (v1 = (uint32_t)((int32_t)((int32_t)A1_R32B((uint32_t)a0, 16080))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)SUB32((uint32_t)v0, (uint32_t)a2))));
    a1 = a2::frLd<uint16_t>(a2fr, 0);
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)SLL32((uint32_t)v0, 4))));
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v0, v1))));
    A1_W16B((uint32_t)a0, 756, (uint16_t)a1);
    (v0 = (uint64_t)((uint64_t)RC_U64X(a0) + (uint64_t)0ULL));
    v1 = a2::frLd<uint16_t>(a2fr, 2);
    A1_W16B((uint32_t)v0, 758, (uint16_t)v1);
    { return v0; }
    __builtin_unreachable();
}

// vab_get_splitchunk (0x1da330): typed by tools/typed_lift2.py (round A2; leaf)
inline uint64_t vab_get_splitchunk(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint64_t a0)
{
    uint64_t v0 = 0;
    uint64_t v1 = 0;
    (v0 = (uint64_t)((uint64_t)a0 + (uint64_t)0ULL));
    (v1 = (uint64_t)(v0 ^ (uint64_t)(uint16_t)0));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v0, 8))));
    { if (v1 == 0) v0 = (uint64_t)0;; return v0; }
    __builtin_unreachable();
}

// kernel_sub_001DB418 (0x1db418): typed by tools/typed_lift2.py (round A2; leaf)
inline uint64_t kernel_sub_001DB418(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t a0, uint32_t a1)
{
    uint64_t v0 = 0;
    uint64_t v1 = 0;
    a1 = a1 << 2;
    a1 = a1 + a0;
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B(a1, 8))));
    (a1 = (uint32_t)((int32_t)((int32_t)ADD32((uint32_t)v0, 3))));
    (v1 = (uint64_t)(((int64_t)(int64_t)RC_U64X(v0) < (int64_t)(int32_t)0) ? 1 : 0));
    if (RC_U64X(v1) != 0) v0 = (uint64_t)(a1_sx64(a1));;
    (v0 = (uint64_t)((int64_t)(int32_t)(SRA32((int32_t)v0, 2))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)SLL32((uint32_t)v0, 2))));
    { (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(a0, (uint32_t)v0)))); return v0; }
    __builtin_unreachable();
}

// GAMEUTIL_is_yaw_pitch_get_direction (0x10817c8): typed by tools/typed_lift2.py (round A2; leaf)
inline a1::RetVF GAMEUTIL_is_yaw_pitch_get_direction(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t a0)
{
    float f0 = 0.0f;
    float f1 = 0.0f;
    uint32_t at = 0;
    uint64_t v0 = 0;
    uint32_t fcr31 = 0;
    f1 = a1_flt(A1_R32B(a0, 4));
    f0 = a1_flt(0u);
    fcr31 = (a1f::lt(f1, f0)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
    if ((fcr31 & 0x800000u) == 0u) goto L_1081800;
    at = 0xBF7F0000u;
    at = at | 0xF972u;
    f0 = a1_flt(at);
    fcr31 = (a1f::le(f0, f1)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
    if ((fcr31 & 0x800000u) != 0u) goto L_1081818;
    { (v0 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL)); return a1::RetVF{(uint64_t)v0, f0}; }
L_1081800:;
    at = 0x3F7F0000u;
    at = at | 0xF972u;
    f0 = a1_flt(at);
    fcr31 = (a1f::le(f1, f0)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
    if ((fcr31 & 0x800000u) == 0u) { (v0 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL)); goto L_1081820; }
L_1081818:;
    { (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 1)))); return a1::RetVF{(uint64_t)v0, f0}; }
L_1081820:;
    { return a1::RetVF{(uint64_t)v0, f0}; }
    __builtin_unreachable();
}

// entrymanager (0x1082ed0): typed by tools/typed_lift2.py (round A2; leaf)
inline void entrymanager(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt)
{
    { return; }
    __builtin_unreachable();
}

// reset_double_spring_interp_cntrl (0x1084590): typed by tools/typed_lift2.py (round A2; leaf)
inline a1::RetVF reset_double_spring_interp_cntrl(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t a0, float f12)
{
    float f0 = 0.0f;
    uint32_t v0 = 0;
    v0 = a0 + 0x1Cu;
    f0 = ((float)(f12));
    A1_W32(v0, a1_bits(f12));
    A1_W32B(a0, 24, a1_bits(f12));
    { return a1::RetVF{(uint64_t)a1_sx64(v0), f0}; }
    __builtin_unreachable();
}

// setShadowEffect (0x10871c8): typed by tools/typed_lift2.py (round A2; leaf)
inline uint32_t setShadowEffect(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt)
{
    uint32_t v0 = 0;
    uint32_t v1 = 0;
    v1 = 0x1180000u;
    v0 = 1u;
    { A1_W32B(v1, 25236, v0); return v0; }
    __builtin_unreachable();
}

// fsEffectDefault (0x1088e58): typed by tools/typed_lift2.py (round A2; leaf)
inline uint32_t fsEffectDefault(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt)
{
    uint32_t v0 = 0;
    uint32_t v1 = 0;
    uint32_t a0 = 0;
    uint32_t a1 = 0;
    { const uint32_t _dynInsn = A1_R32(0x1088E58u); const int32_t _dynSimm = (int16_t)(_dynInsn & 0xFFFFu); const uint32_t _dynImm = _dynInsn & 0xFFFFu; (void)_dynSimm; (void)_dynImm; (v0 = (uint32_t)((int32_t)((int32_t)((uint32_t)_dynImm << 16)))); }
    { const uint32_t _dynInsn = A1_R32(0x1088E5Cu); const int32_t _dynSimm = (int16_t)(_dynInsn & 0xFFFFu); const uint32_t _dynImm = _dynInsn & 0xFFFFu; (void)_dynSimm; (void)_dynImm; (v1 = (uint32_t)((int32_t)((int32_t)((uint32_t)_dynImm << 16)))); }
    { const uint32_t _dynInsn = A1_R32(0x1088E60u); const int32_t _dynSimm = (int16_t)(_dynInsn & 0xFFFFu); const uint32_t _dynImm = _dynInsn & 0xFFFFu; (void)_dynSimm; (void)_dynImm; (a0 = (uint32_t)((int32_t)((int32_t)A1_R32(ADD32(v0, _dynSimm))))); }
    { const uint32_t _dynInsn = A1_R32(0x1088E64u); const int32_t _dynSimm = (int16_t)(_dynInsn & 0xFFFFu); const uint32_t _dynImm = _dynInsn & 0xFFFFu; (void)_dynSimm; (void)_dynImm; (v1 = (uint32_t)((int32_t)((int32_t)ADD32(v1, _dynSimm)))); }
    a0 = a0 ^ v1;
    if (a1_sx64(a0) == 0) v1 = (uint32_t)((uint64_t)0);;
    { const bool c = v1 == 0; v0 = 0x1210000u; if (c) goto L_1088eb0; }
    a1 = v0 + 0x4750u;
    { const bool c = a1 == 0; v0 = 0x1180000u; if (c) goto L_1088eb0; }
    a0 = v0 + 0x62F0u;
    v0 = A1_R32(a0);
    if (v0 == 0) { A1_W32(a0, a1); goto L_1088eb0; }
    v1 = A1_R32B(v0, 4);
    v0 = A1_R32B(a1, 4);
    v1 = v1 << 4;
    v0 = v0 << 4;
    v0 = (int32_t)v0 < (int32_t)v1 ? 1u : 0u;
    if (v0 == 0) { A1_W32(a0, a1); goto L_1088eb0; }
L_1088eb0:;
    { return v0; }
    __builtin_unreachable();
}

// deleteFsEffect (0x1089118): typed by tools/typed_lift2.py (round A2; leaf)
inline void deleteFsEffect(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt)
{
    { return; }
    __builtin_unreachable();
}

// deleteFogEffect (0x1089270): typed by tools/typed_lift2.py (round A2; leaf)
inline void deleteFogEffect(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt)
{
    { return; }
    __builtin_unreachable();
}

// manager_sub_01089288 (0x1089288): typed by tools/typed_lift2.py (round A2; leaf)
inline uint32_t manager_sub_01089288(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt)
{
    uint32_t v0 = 0;
    uint32_t v1 = 0;
    uint32_t a0 = 0;
    uint32_t a1 = 0;
    { const uint32_t _dynInsn = A1_R32(0x1089288u); const int32_t _dynSimm = (int16_t)(_dynInsn & 0xFFFFu); const uint32_t _dynImm = _dynInsn & 0xFFFFu; (void)_dynSimm; (void)_dynImm; (v0 = (uint32_t)((int32_t)((int32_t)((uint32_t)_dynImm << 16)))); }
    { const uint32_t _dynInsn = A1_R32(0x108928Cu); const int32_t _dynSimm = (int16_t)(_dynInsn & 0xFFFFu); const uint32_t _dynImm = _dynInsn & 0xFFFFu; (void)_dynSimm; (void)_dynImm; (v1 = (uint32_t)((int32_t)((int32_t)((uint32_t)_dynImm << 16)))); }
    { const uint32_t _dynInsn = A1_R32(0x1089290u); const int32_t _dynSimm = (int16_t)(_dynInsn & 0xFFFFu); const uint32_t _dynImm = _dynInsn & 0xFFFFu; (void)_dynSimm; (void)_dynImm; (a0 = (uint32_t)((int32_t)((int32_t)A1_R32(ADD32(v0, _dynSimm))))); }
    { const uint32_t _dynInsn = A1_R32(0x1089294u); const int32_t _dynSimm = (int16_t)(_dynInsn & 0xFFFFu); const uint32_t _dynImm = _dynInsn & 0xFFFFu; (void)_dynSimm; (void)_dynImm; (v1 = (uint32_t)((int32_t)((int32_t)ADD32(v1, _dynSimm)))); }
    a0 = a0 ^ v1;
    if (a1_sx64(a0) == 0) v1 = (uint32_t)((uint64_t)0);;
    { const bool c = v1 == 0; { const uint32_t _dynInsn = A1_R32(0x10892A4u); const int32_t _dynSimm = (int16_t)(_dynInsn & 0xFFFFu); const uint32_t _dynImm = _dynInsn & 0xFFFFu; (void)_dynSimm; (void)_dynImm; (v0 = (uint32_t)((int32_t)((int32_t)((uint32_t)_dynImm << 16)))); } if (c) goto L_10892e0; }
    { const uint32_t _dynInsn = A1_R32(0x10892A8u); const int32_t _dynSimm = (int16_t)(_dynInsn & 0xFFFFu); const uint32_t _dynImm = _dynInsn & 0xFFFFu; (void)_dynSimm; (void)_dynImm; (a0 = (uint32_t)((int32_t)((int32_t)A1_R32(ADD32(v0, _dynSimm))))); }
    { const bool c = a0 == 0; v0 = 0x1180000u; if (c) goto L_10892e0; }
    a1 = v0 + 0x62F0u;
    v0 = A1_R32(a1);
    if (v0 == 0) { A1_W32(a1, a0); goto L_10892e0; }
    v1 = A1_R32B(v0, 4);
    v0 = A1_R32B(a0, 4);
    v1 = v1 << 4;
    v0 = v0 << 4;
    v0 = (int32_t)v0 < (int32_t)v1 ? 1u : 0u;
    if (v0 == 0) { A1_W32(a1, a0); goto L_10892e0; }
L_10892e0:;
    { return v0; }
    __builtin_unreachable();
}

// getFogFactor0 (0x1089b70): typed by tools/typed_lift2.py (round A2; leaf)
inline a1::RetVF getFogFactor0(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt)
{
    float f0 = 0.0f;
    float f1 = 0.0f;
    float f2 = 0.0f;
    float f3 = 0.0f;
    float f4 = 0.0f;
    uint32_t v0 = 0;
    uint32_t fcr31 = 0;
    v0 = 0x1210000u;
    v0 = v0 + 0x47D8u;
    f2 = a1_flt(A1_R32B(v0, 8));
    f3 = a1_flt(A1_R32B(v0, 4));
    f4 = a1_flt(A1_R32B(v0, 16));
    f1 = a1_flt(A1_R32B(v0, 12));
    f0 = a1f::mul(f2, f3);
    f2 = a1f::sub(f2, f3);
    f1 = a1f::sub(f1, f4);
    f0 = ((float)(f0) * (float)(f1));
    f0 = a1f::div(fcr31, f0, f2);
    { return a1::RetVF{(uint64_t)a1_sx64(v0), f0}; }
    __builtin_unreachable();
}

// ModelDispGetWorkSize (0x1099a00): typed by tools/typed_lift2.py (round A2; leaf)
inline uint32_t ModelDispGetWorkSize(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt)
{
    uint32_t v0 = 0;
    { v0 = 0x2C3u; return v0; }
    __builtin_unreachable();
}

// ptclCounterInit (0x109a4c8): typed by tools/typed_lift2.py (round A2; leaf)
inline void ptclCounterInit(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt)
{
    { return; }
    __builtin_unreachable();
}

// manager_sub_0109ACF8 (0x109acf8): typed by tools/typed_lift2.py (round A2; leaf)
inline void manager_sub_0109ACF8(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt)
{
    { return; }
    __builtin_unreachable();
}

// manager_sub_0109AD00 (0x109ad00): typed by tools/typed_lift2.py (round A2; leaf)
inline void manager_sub_0109AD00(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt)
{
    { return; }
    __builtin_unreachable();
}

// shadowGetWorkSize (0x109deb0): typed by tools/typed_lift2.py (round A2; leaf)
inline uint32_t shadowGetWorkSize(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt)
{
    uint32_t v0 = 0;
    { v0 = 0x53u; return v0; }
    __builtin_unreachable();
}

// zFogInit (0x10ac470): typed by tools/typed_lift2.py (round A2; leaf)
inline void zFogInit(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt)
{
    { return; }
    __builtin_unreachable();
}

// get_type_parameter_segment_motion_ik_base (0x10b92c0): typed by tools/typed_lift2.py (round A2; leaf)
inline uint32_t get_type_parameter_segment_motion_ik_base(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t a0)
{
    uint32_t v0 = 0;
    uint32_t v1 = 0;
    uint32_t a1 = 0;
    uint32_t a2 = 0;
    uint64_t hi = 0;
    uint64_t lo = 0;
    v0 = A1_R32(a0);
    v1 = 0x30000u;
    v1 = v1 | 0xFFFFu;
    a0 = A1_R32B(v0, 24);
    a2 = A1_R32(a0);
    v1 = a2 & v1;
    { const bool c = v1 == 0; a0 = a0 - v1; if (c) goto L_10b92e8; }
    a0 = A1_R32(a0);
    goto L_10b92ec;
L_10b92e8:;
    a0 = 0u;
L_10b92ec:;
    { const bool c = a0 == 0; v0 = 0u; if (c) goto L_10b9320; }
    { const uint32_t _dynInsn = A1_R32(0x10B92F4u); const int32_t _dynSimm = (int16_t)(_dynInsn & 0xFFFFu); const uint32_t _dynImm = _dynInsn & 0xFFFFu; (void)_dynSimm; (void)_dynImm; (v0 = (uint32_t)((int32_t)((int32_t)((uint32_t)_dynImm << 16)))); }
    { const uint32_t _dynInsn = A1_R32(0x10B92F8u); const int32_t _dynSimm = (int16_t)(_dynInsn & 0xFFFFu); const uint32_t _dynImm = _dynInsn & 0xFFFFu; (void)_dynSimm; (void)_dynImm; (v1 = (uint32_t)((int32_t)((int32_t)A1_R32(ADD32(v0, _dynSimm))))); }
    if (a0 == v1) { v0 = 0u; goto L_10b9320; }
    a1 = A1_R32B(a0, 8);
    v0 = a2 >> 18;
    v0 = v0 & 0xFFFu;
    a0 = A1_R32B(a0, 16);
    v1 = A1_R32B(a1, 12);
    { int64_t result = (int64_t)(int32_t)v0 * (int64_t)(int32_t)v1; lo = (uint64_t)(int64_t)(int32_t)result; hi = (uint64_t)(int64_t)(int32_t)(result >> 32); (v0 = (uint32_t)((int32_t)((int32_t)result))); }
    v0 = a0 + v0;
L_10b9320:;
    { v0 = v0 + 0xCu; return v0; }
    __builtin_unreachable();
}

// request_end_motion_ik_interest (0x10ba120): typed by tools/typed_lift2.py (round A2; leaf)
inline uint64_t request_end_motion_ik_interest(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t a0)
{
    uint64_t v0 = 0;
    uint64_t v1 = 0;
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B(a0, 4))));
    (v1 = (uint64_t)(RC_U64X(v0) ^ (uint64_t)(uint16_t)1));
    if (RC_U64X(v1) != 0) v0 = (uint64_t)0;;
    { A1_W32B(a0, 8, (uint32_t)v0); return v0; }
    __builtin_unreachable();
}

// getSizeGazeIKWork (0x10ba7a0): typed by tools/typed_lift2.py (round A2; leaf)
inline uint32_t getSizeGazeIKWork(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt)
{
    uint32_t v0 = 0;
    { v0 = 0xC0u; return v0; }
    __builtin_unreachable();
}

// GetAnimationDefClass (0x10bb060): typed by tools/typed_lift2.py (round A2; leaf)
inline uint64_t GetAnimationDefClass(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint64_t a0)
{
    uint64_t v0 = 0;
    uint64_t v1 = 0;
    (v0 = (uint64_t)((uint64_t)a0 + (uint64_t)0ULL));
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 18))));
    (v1 = (uint64_t)(uint32_t)((uint8_t)A1_R8B((uint32_t)v0, 8)));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v0, 28))));
    (v1 = (uint64_t)(RC_U64X(v1) ^ RC_U64X(a0)));
    { if (RC_U64X(v1) != 0) v0 = (uint64_t)0;; return v0; }
    __builtin_unreachable();
}

// manager_sub_010BB208 (0x10bb208): typed by tools/typed_lift2.py (round A2; leaf)
inline void manager_sub_010BB208(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt)
{
    { return; }
    __builtin_unreachable();
}

// RequestCharacterMotionShiftFunc (0x10c1530): typed by tools/typed_lift2.py (round A2; leaf)
inline uint32_t RequestCharacterMotionShiftFunc(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint64_t a0, uint32_t a1)
{
    uint32_t v0 = 0;
    uint32_t v1 = 0;
    uint32_t a2 = 0;
    (v1 = (uint32_t)((uint8_t)A1_R8B((uint32_t)a0, 8)));
    (a2 = (uint32_t)((int32_t)((int32_t)ADD32((uint32_t)a0, 144))));
    v0 = 9u;
    { const bool c = v1 == v0; (a0 = (uint64_t)((uint64_t)a1_sx64(a2) + (uint64_t)0ULL)); if (c) goto L_10c1554; }
    v0 = 0x16u;
    (a0 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL));
    v0 = v1 ^ v0;
    if (a1_sx64(v0) == 0) a0 = (uint64_t)(a1_sx64(a2));;
L_10c1554:;
    (v0 = (uint32_t)((int32_t)((int8_t)A1_R8B((uint32_t)a0, 1341))));
    (v1 = (uint32_t)((uint8_t)A1_R8B((uint32_t)a0, 1341)));
    a2 = v0 << 2;
    v0 = (int32_t)v0 < 8 ? 1u : 0u;
    { const bool c = v0 == 0; v1 = v1 + 1u; if (c) goto L_10c1578; }
    (v0 = (uint32_t)((int32_t)((int32_t)ADD32(a2, (uint32_t)a0))));
    A1_W8B((uint32_t)a0, 1341, (uint8_t)v1);
    A1_W32B(v0, 1360, a1);
L_10c1578:;
    { return v0; }
    __builtin_unreachable();
}

// SetCharacterObjControlCallBackFunc (0x10c1d08): typed by tools/typed_lift2.py (round A2; leaf)
inline uint32_t SetCharacterObjControlCallBackFunc(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint64_t a0, uint32_t a1)
{
    uint32_t v0 = 0;
    uint32_t v1 = 0;
    uint32_t a2 = 0;
    (v1 = (uint32_t)((uint8_t)A1_R8B((uint32_t)a0, 8)));
    (a2 = (uint32_t)((int32_t)((int32_t)ADD32((uint32_t)a0, 144))));
    v0 = 9u;
    { const bool c = v1 == v0; (a0 = (uint64_t)((uint64_t)a1_sx64(a2) + (uint64_t)0ULL)); if (c) goto L_10c1d2c; }
    v0 = 0x16u;
    (a0 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL));
    v0 = v1 ^ v0;
    if (a1_sx64(v0) == 0) a0 = (uint64_t)(a1_sx64(a2));;
L_10c1d2c:;
    { A1_W32B((uint32_t)a0, 2504, a1); return v0; }
    __builtin_unreachable();
}

// SetInertiaRotInterpMode (0x10c29a0): typed by tools/typed_lift2.py (round A2; leaf)
inline uint64_t SetInertiaRotInterpMode(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint64_t a0)
{
    uint64_t v0 = 0;
    uint32_t v1 = 0;
    uint64_t a1 = 0;
    uint64_t a2 = 0;
    uint64_t a3 = 0;
    (v1 = (uint32_t)((uint8_t)A1_R8B((uint32_t)a0, 8)));
    (a1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)a0, 144))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 9))));
    { const bool c = (int64_t)(int32_t)v1 == (int64_t)v0; (a3 = (uint64_t)((uint64_t)RC_U64X(a1) + (uint64_t)0ULL)); if (c) goto L_10c29c4; }
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 22))));
    (a3 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL));
    (v0 = (uint64_t)(a1_sx64(v1) ^ RC_U64X(v0)));
    if (RC_U64X(v0) == 0) a3 = (uint64_t)(a1);;
L_10c29c4:;
    (v0 = (uint64_t)((int64_t)(int32_t)((int8_t)A1_R8B((uint32_t)a3, 2598))));
    { const bool c = (int64_t)v0 == 0; v1 = 1u; if (c) goto L_10c29f4; }
    (a2 = (uint64_t)(A1_R64B((uint32_t)a3, 2608)));
    (a0 = (uint64_t)(A1_R64B((uint32_t)a3, 2616)));
    (a1 = (uint64_t)(A1_R64B((uint32_t)a3, 2624)));
    (v0 = (uint64_t)(A1_R64B((uint32_t)a3, 2632)));
    A1_W8B((uint32_t)a3, 2601, (uint8_t)v1);
    A1_W64B((uint32_t)a3, 2664, v0);
    A1_W64B((uint32_t)a3, 2640, a2);
    A1_W64B((uint32_t)a3, 2648, a0);
    A1_W64B((uint32_t)a3, 2656, a1);
L_10c29f4:;
    { return v0; }
    __builtin_unreachable();
}

// charrootdyn_getSlideCollisionJudgeCBArg (0x10dc898): typed by tools/typed_lift2.py (round A2; leaf)
inline uint64_t charrootdyn_getSlideCollisionJudgeCBArg(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint64_t a0, uint32_t a1, uint64_t a2)
{
    uint64_t v0 = 0;
    uint32_t v1 = 0;
    v1 = A1_R32B(a1, 104);
    (a2 = (uint64_t)(((uint64_t)0ULL < (uint64_t)a2) ? 1 : 0));
    (v0 = (uint64_t)((uint64_t)a0 + (uint64_t)0ULL));
    v1 = v1 >> 11;
    v1 = v1 & 1u;
    if (a1_sx64(v1) == 0) a2 = (uint64_t)0;;
    A1_W32B((uint32_t)a0, 0, (uint32_t)a2);
    v1 = A1_R32B(a1, 104);
    v1 = v1 >> 15;
    v1 = v1 & 1u;
    { A1_W32B((uint32_t)a0, 4, v1); return v0; }
    __builtin_unreachable();
}

// manager_sub_010ED778 (0x10ed778): typed by tools/typed_lift2.py (round A2; leaf)
inline void manager_sub_010ED778(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt)
{
    { return; }
    __builtin_unreachable();
}

// FlushJudgeVolumeClass (0x10ee6f8): typed by tools/typed_lift2.py (round A2; leaf)
inline void FlushJudgeVolumeClass(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt)
{
    { return; }
    __builtin_unreachable();
}

// GetGameLightClass (0x10f74e8): typed by tools/typed_lift2.py (round A2; leaf)
inline uint64_t GetGameLightClass(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint64_t a0)
{
    uint64_t v0 = 0;
    uint64_t v1 = 0;
    (v0 = (uint64_t)((uint64_t)a0 + (uint64_t)0ULL));
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 26))));
    (v1 = (uint64_t)(uint32_t)((uint8_t)A1_R8B((uint32_t)v0, 8)));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v0, 32))));
    (v1 = (uint64_t)(RC_U64X(v1) ^ RC_U64X(a0)));
    { if (RC_U64X(v1) != 0) v0 = (uint64_t)0;; return v0; }
    __builtin_unreachable();
}

// GetGlobalLightClass (0x10f8b20): typed by tools/typed_lift2.py (round A2; leaf)
inline uint64_t GetGlobalLightClass(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint64_t a0)
{
    uint64_t v0 = 0;
    uint64_t v1 = 0;
    (v0 = (uint64_t)((uint64_t)a0 + (uint64_t)0ULL));
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 25))));
    (v1 = (uint64_t)(uint32_t)((uint8_t)A1_R8B((uint32_t)v0, 8)));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v0, 32))));
    (v1 = (uint64_t)(RC_U64X(v1) ^ RC_U64X(a0)));
    { if (RC_U64X(v1) != 0) v0 = (uint64_t)0;; return v0; }
    __builtin_unreachable();
}

// GetAmbientLightClass (0x10f9508): typed by tools/typed_lift2.py (round A2; leaf)
inline uint64_t GetAmbientLightClass(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint64_t a0)
{
    uint64_t v0 = 0;
    uint64_t v1 = 0;
    (v0 = (uint64_t)((uint64_t)a0 + (uint64_t)0ULL));
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 20))));
    (v1 = (uint64_t)(uint32_t)((uint8_t)A1_R8B((uint32_t)v0, 8)));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v0, 28))));
    (v1 = (uint64_t)(RC_U64X(v1) ^ RC_U64X(a0)));
    { if (RC_U64X(v1) != 0) v0 = (uint64_t)0;; return v0; }
    __builtin_unreachable();
}

// manager_sub_010F9528 (0x10f9528): typed by tools/typed_lift2.py (round A2; leaf)
inline void manager_sub_010F9528(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt)
{
    { return; }
    __builtin_unreachable();
}

// createBinaryMemoryInfoWindow (0x1101688): typed by tools/typed_lift2.py (round A2; leaf)
inline void createBinaryMemoryInfoWindow(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt)
{
    { return; }
    __builtin_unreachable();
}

// skelSegDel (0x11017e0): typed by tools/typed_lift2.py (round A2; leaf)
inline void skelSegDel(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt)
{
    { return; }
    __builtin_unreachable();
}

// debugCreateAnimObjSegmentDisp (0x11017e8): typed by tools/typed_lift2.py (round A2; leaf)
inline void debugCreateAnimObjSegmentDisp(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt)
{
    { return; }
    __builtin_unreachable();
}

// debugDeleteAnimObjSegmentDisp (0x11017f0): typed by tools/typed_lift2.py (round A2; leaf)
inline void debugDeleteAnimObjSegmentDisp(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt)
{
    { return; }
    __builtin_unreachable();
}

// debugCreateAnimObjSegmentDefDisp (0x11017f8): typed by tools/typed_lift2.py (round A2; leaf)
inline void debugCreateAnimObjSegmentDefDisp(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt)
{
    { return; }
    __builtin_unreachable();
}

// debugDeleteAnimObjSegmentDefDisp (0x1101800): typed by tools/typed_lift2.py (round A2; leaf)
inline void debugDeleteAnimObjSegmentDefDisp(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt)
{
    { return; }
    __builtin_unreachable();
}

// scriptIntrSegInit (0x1102fd8): typed by tools/typed_lift2.py (round A2; leaf)
inline void scriptIntrSegInit(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt)
{
    { return; }
    __builtin_unreachable();
}

// GetScriptAnimationClass (0x110a838): typed by tools/typed_lift2.py (round A2; leaf)
inline uint64_t GetScriptAnimationClass(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint64_t a0)
{
    uint64_t v0 = 0;
    uint64_t v1 = 0;
    (v0 = (uint64_t)((uint64_t)a0 + (uint64_t)0ULL));
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 13))));
    (v1 = (uint64_t)(uint32_t)((uint8_t)A1_R8B((uint32_t)v0, 8)));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v0, 32))));
    (v1 = (uint64_t)(RC_U64X(v1) ^ RC_U64X(a0)));
    { if (RC_U64X(v1) != 0) v0 = (uint64_t)0;; return v0; }
    __builtin_unreachable();
}

// SetScriptAnimationObjChildEntryGroup (0x110a8e0): typed by tools/typed_lift2.py (round A2; leaf)
inline uint32_t SetScriptAnimationObjChildEntryGroup(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t a0, uint32_t a1)
{
    uint32_t v0 = 0;
    v0 = A1_R8B(a0, 8);
    a0 = a0 + 0x20u;
    v0 = v0 ^ 0xDu;
    if (a1_sx64(v0) != 0) a0 = (uint32_t)((uint64_t)0);;
    { A1_W32B(a0, 864, a1); return v0; }
    __builtin_unreachable();
}

// SetScariptAnimationObjMotSyncSlotId (0x110bb30): typed by tools/typed_lift2.py (round A2; leaf)
inline uint32_t SetScariptAnimationObjMotSyncSlotId(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t a0, uint32_t a1)
{
    uint32_t v0 = 0;
    v0 = A1_R8B(a0, 8);
    a0 = a0 + 0x20u;
    v0 = v0 ^ 0xDu;
    if (a1_sx64(v0) != 0) a0 = (uint32_t)((uint64_t)0);;
    { A1_W32B(a0, 848, a1); return v0; }
    __builtin_unreachable();
}

// SetScriptCharacterObjReleaseCallBackFunc (0x110cc78): typed by tools/typed_lift2.py (round A2; leaf)
inline uint32_t SetScriptCharacterObjReleaseCallBackFunc(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t a0, uint32_t a1, uint32_t a2)
{
    uint32_t v0 = 0;
    v0 = A1_R8B(a0, 8);
    a0 = a0 + 0x20u;
    v0 = v0 ^ 9u;
    if (a1_sx64(v0) != 0) a0 = (uint32_t)((uint64_t)0);;
    A1_W32B(a0, 4620, a2);
    { A1_W32B(a0, 4616, a1); return v0; }
    __builtin_unreachable();
}

// GetScrLwsOriClass (0x110d868): typed by tools/typed_lift2.py (round A2; leaf)
inline uint64_t GetScrLwsOriClass(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint64_t a0)
{
    uint64_t v0 = 0;
    uint64_t v1 = 0;
    (v0 = (uint64_t)((uint64_t)a0 + (uint64_t)0ULL));
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 22))));
    (v1 = (uint64_t)(uint32_t)((uint8_t)A1_R8B((uint32_t)v0, 8)));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v0, 32))));
    (v1 = (uint64_t)(RC_U64X(v1) ^ RC_U64X(a0)));
    { if (RC_U64X(v1) != 0) v0 = (uint64_t)0;; return v0; }
    __builtin_unreachable();
}

// GetScriptLwsOrientDefinfo (0x110da90): typed by tools/typed_lift2.py (round A2; leaf)
inline uint64_t GetScriptLwsOrientDefinfo(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint64_t a0)
{
    uint64_t v0 = 0;
    uint64_t v1 = 0;
    (v0 = (uint64_t)((uint64_t)a0 + (uint64_t)0ULL));
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 23))));
    (v1 = (uint64_t)(uint32_t)((uint8_t)A1_R8B((uint32_t)v0, 8)));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v0, 32))));
    (v1 = (uint64_t)(RC_U64X(v1) ^ RC_U64X(a0)));
    { if (RC_U64X(v1) != 0) v0 = (uint64_t)0;; return v0; }
    __builtin_unreachable();
}

// manager_sub_0110DC18 (0x110dc18): typed by tools/typed_lift2.py (round A2; leaf)
inline void manager_sub_0110DC18(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt)
{
    { return; }
    __builtin_unreachable();
}

// SpriteMenuIsFadeCheckSelectLayer (0x110ecd0): typed by tools/typed_lift2.py (round A2; leaf)
inline a1::RetVF SpriteMenuIsFadeCheckSelectLayer(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t a0)
{
    float f0 = 0.0f;
    float f1 = 0.0f;
    uint32_t v0 = 0;
    uint32_t v1 = 0;
    uint32_t a1 = 0;
    uint32_t a2 = 0;
    uint32_t fcr31 = 0;
    v0 = A1_R8B(a0, 8);
    a0 = a0 + 0x90u;
    v1 = 0xFFFFFFFFu;
    v0 = v0 ^ 0x21u;
    if (a1_sx64(v0) != 0) a0 = (uint32_t)((uint64_t)0);;
    a1 = A1_R32B(a0, 12);
    { const bool c = a1 == v1; v0 = a1 << 4; if (c) goto L_110ed60; }
    a0 = A1_R32B(a0, 4);
    v0 = v0 + a1;
    a1 = 2u;
    v0 = v0 << 2;
    a0 = a0 + v0;
    v1 = A1_R32(a0);
    v1 = A1_R32B(v1, 12);
    { const bool c = v1 == a1; a2 = a0 + 0x1Cu; if (c) goto L_110ed40; }
    v0 = (int32_t)v1 < 3 ? 1u : 0u;
    if (v0 == 0) { v0 = 3u; goto L_110ed28; }
    v0 = 1u;
    goto L_110ed34;
L_110ed28:;
    { const bool c = v1 == v0; a2 = a0 + 0x1Cu; if (c) goto L_110ed40; }
    v0 = 4u;
L_110ed34:;
    { const bool c = v1 == v0; a2 = a0 + 0x20u; if (c) goto L_110ed40; }
    a2 = a0 + 0xCu;
L_110ed40:;
    f1 = a1_flt(A1_R32B(a0, 16));
    f0 = a1_flt(A1_R32(a2));
    fcr31 = (a1f::eq(f1, f0)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
    { const bool c = (fcr31 & 0x800000u) == 0u; v0 = 1u; if (c) goto L_110edc0; }
    { v0 = 0u; return a1::RetVF{(uint64_t)a1_sx64(v0), f0}; }
L_110ed60:;
    a0 = A1_R32B(a0, 4);
    v1 = 2u;
    v0 = A1_R32(a0);
    a1 = A1_R32B(v0, 12);
    { const bool c = a1 == v1; a2 = a0 + 0x1Cu; if (c) goto L_110eda8; }
    v0 = (int32_t)a1 < 3 ? 1u : 0u;
    if (v0 == 0) { v0 = 3u; goto L_110ed90; }
    v0 = 1u;
    goto L_110ed9c;
L_110ed90:;
    { const bool c = a1 == v0; a2 = a0 + 0x1Cu; if (c) goto L_110eda8; }
    v0 = 4u;
L_110ed9c:;
    { const bool c = a1 == v0; a2 = a0 + 0x20u; if (c) goto L_110eda8; }
    a2 = a0 + 0xCu;
L_110eda8:;
    f1 = a1_flt(A1_R32B(a0, 16));
    f0 = a1_flt(A1_R32(a2));
    fcr31 = (a1f::eq(f1, f0)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
    { const bool c = (fcr31 & 0x800000u) == 0u; v0 = 1u; if (c) goto L_110edc0; }
    v0 = 0u;
L_110edc0:;
    { return a1::RetVF{(uint64_t)a1_sx64(v0), f0}; }
    __builtin_unreachable();
}

// GetScriptSpriteMenuClass (0x11119b8): typed by tools/typed_lift2.py (round A2; leaf)
inline uint64_t GetScriptSpriteMenuClass(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint64_t a0)
{
    uint64_t v0 = 0;
    uint64_t v1 = 0;
    (v0 = (uint64_t)((uint64_t)a0 + (uint64_t)0ULL));
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 33))));
    (v1 = (uint64_t)(uint32_t)((uint8_t)A1_R8B((uint32_t)v0, 8)));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v0, 32))));
    (v1 = (uint64_t)(RC_U64X(v1) ^ RC_U64X(a0)));
    { if (RC_U64X(v1) != 0) v0 = (uint64_t)0;; return v0; }
    __builtin_unreachable();
}

// SpriteMenuSetLayerPriority (0x1111e98): typed by tools/typed_lift2.py (round A2; leaf)
inline uint32_t SpriteMenuSetLayerPriority(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t a0, uint32_t a1, uint32_t a2)
{
    uint32_t v0 = 0;
    uint32_t v1 = 0;
    uint32_t a3 = 0;
    v0 = A1_R8B(a0, 8);
    a0 = a0 + 0x90u;
    a3 = a1 << 4;
    a2 = a2 & 0xFu;
    v0 = v0 ^ 0x21u;
    a3 = a3 + a1;
    if (a1_sx64(v0) != 0) a0 = (uint32_t)((uint64_t)0);;
    a3 = a3 << 2;
    v1 = A1_R32B(a0, 4);
    a1 = 0xFFFC0000u;
    a1 = a1 | 0x3FFFu;
    a2 = a2 << 14;
    v1 = v1 + a3;
    v0 = A1_R32B(v1, 8);
    v0 = v0 & a1;
    v0 = v0 | a2;
    { A1_W32B(v1, 8, v0); return v0; }
    __builtin_unreachable();
}

// SpriteMenuGetSelectLayer (0x1112ba0): typed by tools/typed_lift2.py (round A2; leaf)
inline uint32_t SpriteMenuGetSelectLayer(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t a0)
{
    uint32_t v0 = 0;
    v0 = A1_R8B(a0, 8);
    a0 = a0 + 0x90u;
    v0 = v0 ^ 0x21u;
    if (a1_sx64(v0) != 0) a0 = (uint32_t)((uint64_t)0);;
    { v0 = A1_R32B(a0, 12); return v0; }
    __builtin_unreachable();
}

// SpriteMenuGetPadEvent (0x1112bb8): typed by tools/typed_lift2.py (round A2; leaf)
inline uint32_t SpriteMenuGetPadEvent(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t a0)
{
    uint32_t v0 = 0;
    v0 = A1_R8B(a0, 8);
    a0 = a0 + 0x90u;
    v0 = v0 ^ 0x21u;
    if (a1_sx64(v0) != 0) a0 = (uint32_t)((uint64_t)0);;
    { v0 = A1_R32B(a0, 16); return v0; }
    __builtin_unreachable();
}

// SpriteMenuGetSelectSubMenuId (0x1112f50): typed by tools/typed_lift2.py (round A2; leaf)
inline uint32_t SpriteMenuGetSelectSubMenuId(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t a0)
{
    uint32_t v0 = 0;
    uint32_t v1 = 0;
    uint64_t a1 = 0;
    v0 = A1_R8B(a0, 8);
    a0 = a0 + 0x90u;
    (a1 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL));
    v1 = 0xFFFFFFFFu;
    v0 = v0 ^ 0x21u;
    if (a1_sx64(v0) == 0) a1 = (uint64_t)(a1_sx64(a0));;
    (v0 = (uint32_t)((int32_t)((int32_t)A1_R32B((uint32_t)a1, 12))));
    { const bool c = v0 == v1; a0 = v0 << 4; if (c) goto L_1112f98; }
    v0 = a0 + v0;
    (v1 = (uint32_t)((int32_t)((int32_t)A1_R32B((uint32_t)a1, 4))));
    v0 = v0 << 2;
    v1 = v1 + v0;
    v0 = A1_R32B(v1, 56);
    v0 = v0 << 1;
    { v0 = (uint32_t)((int32_t)v0 >> 22); return v0; }
L_1112f98:;
    { v0 = 0xFFFFFFFFu; return v0; }
    __builtin_unreachable();
}

// SpriteMenuIsFadeCheckLayer (0x11138c8): typed by tools/typed_lift2.py (round A2; leaf)
inline a1::RetVF SpriteMenuIsFadeCheckLayer(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t a0, uint32_t a1)
{
    float f0 = 0.0f;
    float f1 = 0.0f;
    uint32_t v0 = 0;
    uint32_t v1 = 0;
    uint32_t a2 = 0;
    uint32_t fcr31 = 0;
    a2 = A1_R8B(a0, 8);
    a0 = a0 + 0x90u;
    v1 = a1 << 4;
    a2 = a2 ^ 0x21u;
    v1 = v1 + a1;
    if (a1_sx64(a2) != 0) a0 = (uint32_t)((uint64_t)0);;
    v1 = v1 << 2;
    v0 = A1_R32B(a0, 4);
    a1 = 2u;
    v1 = v0 + v1;
    a0 = A1_R32(v1);
    a2 = v1 + 0x1Cu;
    a0 = A1_R32B(a0, 12);
    { const bool c = a0 == a1; v0 = (int32_t)a0 < 3 ? 1u : 0u; if (c) goto L_1113930; }
    if (v0 == 0) { v0 = 3u; goto L_1113918; }
    v0 = 1u;
    goto L_1113924;
L_1113918:;
    { const bool c = a0 == v0; a2 = v1 + 0x1Cu; if (c) goto L_1113930; }
    v0 = 4u;
L_1113924:;
    { const bool c = a0 == v0; a2 = v1 + 0x20u; if (c) goto L_1113930; }
    a2 = v1 + 0xCu;
L_1113930:;
    f1 = a1_flt(A1_R32B(v1, 16));
    f0 = a1_flt(A1_R32(a2));
    fcr31 = (a1f::eq(f1, f0)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
    { const bool c = (fcr31 & 0x800000u) == 0u; v0 = 1u; if (c) goto L_1113948; }
    v0 = 0u;
L_1113948:;
    { return a1::RetVF{(uint64_t)a1_sx64(v0), f0}; }
    __builtin_unreachable();
}

// SpriteMenuGetFadeByLayer (0x1113950): typed by tools/typed_lift2.py (round A2; leaf)
inline a1::RetVF SpriteMenuGetFadeByLayer(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t a0, uint32_t a1)
{
    float f0 = 0.0f;
    uint32_t v0 = 0;
    uint32_t v1 = 0;
    uint32_t a2 = 0;
    a2 = A1_R8B(a0, 8);
    a0 = a0 + 0x90u;
    v1 = a1 << 4;
    a2 = a2 ^ 0x21u;
    v1 = v1 + a1;
    if (a1_sx64(a2) != 0) a0 = (uint32_t)((uint64_t)0);;
    v1 = v1 << 2;
    v0 = A1_R32B(a0, 4);
    a1 = 2u;
    a2 = v0 + v1;
    a0 = A1_R32(a2);
    v1 = a2 + 0x1Cu;
    a0 = A1_R32B(a0, 12);
    { const bool c = a0 == a1; v0 = (int32_t)a0 < 3 ? 1u : 0u; if (c) goto L_11139b8; }
    if (v0 == 0) { v0 = 3u; goto L_11139a0; }
    v0 = 1u;
    goto L_11139ac;
L_11139a0:;
    { const bool c = a0 == v0; v1 = a2 + 0x1Cu; if (c) goto L_11139b8; }
    v0 = 4u;
L_11139ac:;
    { const bool c = a0 == v0; v1 = a2 + 0x20u; if (c) goto L_11139b8; }
    v1 = a2 + 0xCu;
L_11139b8:;
    { f0 = a1_flt(A1_R32(v1)); return a1::RetVF{(uint64_t)a1_sx64(v0), f0}; }
    __builtin_unreachable();
}

// GetScriptSpriteClass (0x1114238): typed by tools/typed_lift2.py (round A2; leaf)
inline uint64_t GetScriptSpriteClass(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint64_t a0)
{
    uint64_t v0 = 0;
    uint64_t v1 = 0;
    (v0 = (uint64_t)((uint64_t)a0 + (uint64_t)0ULL));
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 14))));
    (v1 = (uint64_t)(uint32_t)((uint8_t)A1_R8B((uint32_t)v0, 8)));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v0, 32))));
    (v1 = (uint64_t)(RC_U64X(v1) ^ RC_U64X(a0)));
    { if (RC_U64X(v1) != 0) v0 = (uint64_t)0;; return v0; }
    __builtin_unreachable();
}

// GetSubLinkClass (0x1114490): typed by tools/typed_lift2.py (round A2; leaf)
inline uint64_t GetSubLinkClass(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint64_t a0)
{
    uint64_t v0 = 0;
    uint64_t v1 = 0;
    (v0 = (uint64_t)((uint64_t)a0 + (uint64_t)0ULL));
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 21))));
    (v1 = (uint64_t)(uint32_t)((uint8_t)A1_R8B((uint32_t)v0, 8)));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v0, 28))));
    (v1 = (uint64_t)(RC_U64X(v1) ^ RC_U64X(a0)));
    { if (RC_U64X(v1) != 0) v0 = (uint64_t)0;; return v0; }
    __builtin_unreachable();
}

// CheckCollisionFinderAlive (0x112a450): typed by tools/typed_lift2.py (round A2; leaf)
inline uint32_t CheckCollisionFinderAlive(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t a0)
{
    uint32_t v0 = 0;
    uint32_t v1 = 0;
    v0 = A1_R32B(a0, 12);
    { const bool c = v0 == 0; v0 = 0u; if (c) goto L_112a46c; }
    v1 = A1_R32B(a0, 16);
    { const bool c = v1 != 0; v0 = 1u; if (c) goto L_112a46c; }
    v0 = 0u;
L_112a46c:;
    { return v0; }
    __builtin_unreachable();
}

// CopyGeometryPointer (0x11327b0): typed by tools/typed_lift2.py (round A2; leaf)
inline uint64_t CopyGeometryPointer(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t a0, uint32_t a1)
{
    uint64_t v0 = 0;
    uint64_t v1 = 0;
    (v0 = (uint64_t)(A1_R64B(a1, 0)));
    A1_W64B(a0, 0, v0);
    (v1 = (uint64_t)(A1_R64B(a1, 8)));
    A1_W64B(a0, 8, v1);
    (v0 = (uint64_t)(A1_R64B(a1, 16)));
    A1_W64B(a0, 16, v0);
    (v1 = (uint64_t)(A1_R64B(a1, 24)));
    A1_W64B(a0, 24, v1);
    (v0 = (uint64_t)(A1_R64B(a1, 32)));
    A1_W64B(a0, 32, v0);
    (v1 = (uint64_t)(A1_R64B(a1, 40)));
    A1_W64B(a0, 40, v1);
    (v0 = (uint64_t)(A1_R64B(a1, 48)));
    A1_W64B(a0, 48, v0);
    (v1 = (uint64_t)(A1_R64B(a1, 56)));
    { A1_W64B(a0, 56, v1); return v0; }
    __builtin_unreachable();
}

// free_all_emitter_and_particle_particle_manager (0x113f370): typed by tools/typed_lift2.py (round A2; leaf)
inline void free_all_emitter_and_particle_particle_manager(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt)
{
    { return; }
    __builtin_unreachable();
}

// get_particle_effect_id_from_sheet (0x113fa20): typed by tools/typed_lift2.py (round A2; leaf)
inline uint64_t get_particle_effect_id_from_sheet(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint64_t a0)
{
    uint64_t v0 = 0;
    { (v0 = (uint64_t)((uint64_t)a0 + (uint64_t)0ULL)); return v0; }
    __builtin_unreachable();
}

// get_particle_emitter_from_emitter_id (0x113fa70): typed by tools/typed_lift2.py (round A2; leaf)
inline uint64_t get_particle_emitter_from_emitter_id(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint64_t a0)
{
    uint64_t v0 = 0;
    uint64_t v1 = 0;
    (v0 = (uint64_t)((uint64_t)a0 + (uint64_t)0ULL));
    (v1 = (uint64_t)(v0 ^ (uint64_t)(uint16_t)0));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v0, 16))));
    { if (v1 == 0) v0 = (uint64_t)0;; return v0; }
    __builtin_unreachable();
}

// get_main_particle_effect_lod_from_sheet (0x113fa88): typed by tools/typed_lift2.py (round A2; leaf)
inline uint32_t get_main_particle_effect_lod_from_sheet(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t a0)
{
    uint32_t v0 = 0;
    { v0 = A1_R32B(a0, 72); return v0; }
    __builtin_unreachable();
}

// get_sub_particle_effect_lod_from_sheet (0x113fa90): typed by tools/typed_lift2.py (round A2; leaf)
inline uint32_t get_sub_particle_effect_lod_from_sheet(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t a0)
{
    uint32_t v0 = 0;
    { v0 = A1_R32B(a0, 76); return v0; }
    __builtin_unreachable();
}

// get_top_particle_effect_from_lod (0x113fa98): typed by tools/typed_lift2.py (round A2; leaf)
inline uint32_t get_top_particle_effect_from_lod(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t a0)
{
    uint32_t v0 = 0;
    { v0 = A1_R32B(a0, 212); return v0; }
    __builtin_unreachable();
}

// get_radiate_range_coefficient (0x11401d8): typed by tools/typed_lift2.py (round A2; leaf)
inline float get_radiate_range_coefficient(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, float f12)
{
    float f0 = 0.0f;
    float f1 = 0.0f;
    uint32_t at = 0;
    at = 0x40000000u;
    f0 = a1_flt(at);
    at = 0x3F000000u;
    f1 = a1_flt(at);
    f0 = a1f::sub(f0, f12);
    { f0 = a1f::mul(f0, f1); return f0; }
    __builtin_unreachable();
}

// unload_npe_file_main (0x1140730): typed by tools/typed_lift2.py (round A2; leaf)
inline void unload_npe_file_main(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt)
{
    { return; }
    __builtin_unreachable();
}

// unload_npe_file_sub (0x1140738): typed by tools/typed_lift2.py (round A2; leaf)
inline void unload_npe_file_sub(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt)
{
    { return; }
    __builtin_unreachable();
}

// manager_sub_01140740 (0x1140740): typed by tools/typed_lift2.py (round A2; leaf)
inline uint64_t manager_sub_01140740(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint64_t a0)
{
    uint64_t v0 = 0;
    { (v0 = (uint64_t)((uint64_t)a0 + (uint64_t)0ULL)); return v0; }
    __builtin_unreachable();
}

// manager_sub_01141080 (0x1141080): typed by tools/typed_lift2.py (round A2; leaf)
inline uint32_t manager_sub_01141080(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t a0)
{
    uint32_t v0 = 0;
    v0 = A1_R32B(a0, 200);
    v0 = v0 ^ 2u;
    { v0 = v0 < 1u ? 1u : 0u; return v0; }
    __builtin_unreachable();
}

// manager_sub_01142030 (0x1142030): typed by tools/typed_lift2.py (round A2; leaf)
inline float manager_sub_01142030(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t a0)
{
    float f0 = 0.0f;
    float f1 = 0.0f;
    float f2 = 0.0f;
    uint32_t at = 0;
    uint32_t fcr31 = 0;
    f2 = a1_flt(A1_R32B(a0, 68));
    f0 = a1_flt(A1_R32B(a0, 88));
    fcr31 = (a1f::lt(f2, f0)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
    if ((fcr31 & 0x800000u) == 0u) { f1 = a1_flt(A1_R32B(a0, 92)); goto L_1142050; }
    f0 = a1_flt(A1_R32B(a0, 96));
    { f0 = a1f::mul(f2, f0); return f0; }
L_1142050:;
    at = 0x3F800000u;
    f0 = a1_flt(at);
    fcr31 = (a1f::lt(f2, f1)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
    if ((fcr31 & 0x800000u) != 0u) goto L_1142074;
    f0 = a1_flt(A1_R32B(a0, 64));
    f1 = a1_flt(A1_R32B(a0, 100));
    f0 = a1f::sub(f0, f2);
    f0 = a1f::mul(f0, f1);
L_1142074:;
    { return f0; }
    __builtin_unreachable();
}

// manager_sub_01144E00 (0x1144e00): typed by tools/typed_lift2.py (round A2; leaf)
inline float manager_sub_01144E00(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t a0)
{
    float f0 = 0.0f;
    float f1 = 0.0f;
    uint32_t at = 0;
    at = 0xBF800000u;
    f0 = a1_flt(at);
    at = 0x3F800000u;
    f1 = a1_flt(at);
    A1_W32B(a0, 424, a1_bits(f0));
    A1_W32B(a0, 432, a1_bits(f1));
    A1_W32B(a0, 400, a1_bits(f0));
    A1_W32B(a0, 404, 0u);
    A1_W32B(a0, 408, a1_bits(f1));
    A1_W32B(a0, 412, a1_bits(f0));
    A1_W32B(a0, 416, 0u);
    A1_W32B(a0, 420, a1_bits(f1));
    { A1_W32B(a0, 428, 0u); return f0; }
    __builtin_unreachable();
}

// manager_sub_01144E38 (0x1144e38): typed by tools/typed_lift2.py (round A2; leaf)
inline float manager_sub_01144E38(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t a0)
{
    float f0 = 0.0f;
    uint32_t at = 0;
    at = 0x3F800000u;
    f0 = a1_flt(at);
    A1_W32B(a0, 168, 0u);
    A1_W32B(a0, 164, a1_bits(f0));
    { A1_W32B(a0, 160, 0u); return f0; }
    __builtin_unreachable();
}

// manager_sub_01144E50 (0x1144e50): typed by tools/typed_lift2.py (round A2; leaf)
inline void manager_sub_01144E50(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t a0)
{
    { A1_W32B(a0, 436, 0u); return; }
    __builtin_unreachable();
}

// manager_sub_01144E58 (0x1144e58): typed by tools/typed_lift2.py (round A2; leaf)
inline float manager_sub_01144E58(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t a0)
{
    float f0 = 0.0f;
    uint32_t at = 0;
    at = 0x3F800000u;
    f0 = a1_flt(at);
    A1_W32B(a0, 184, 0u);
    A1_W32B(a0, 180, a1_bits(f0));
    { A1_W32B(a0, 176, 0u); return f0; }
    __builtin_unreachable();
}

// manager_sub_01144E70 (0x1144e70): typed by tools/typed_lift2.py (round A2; leaf)
inline uint32_t manager_sub_01144E70(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t a0)
{
    uint32_t v0 = 0;
    v0 = 3u;
    A1_W32B(a0, 200, 0u);
    A1_W32B(a0, 188, v0);
    A1_W32B(a0, 192, 0u);
    { A1_W32B(a0, 196, 0u); return v0; }
    __builtin_unreachable();
}

// manager_sub_01144E88 (0x1144e88): typed by tools/typed_lift2.py (round A2; leaf)
inline void manager_sub_01144E88(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t a0)
{
    { A1_W32B(a0, 204, 0u); return; }
    __builtin_unreachable();
}

// is_eternal_wind (0x11464c0): typed by tools/typed_lift2.py (round A2; leaf)
inline a1::RetVF is_eternal_wind(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t a0)
{
    float f0 = 0.0f;
    float f1 = 0.0f;
    uint32_t v0 = 0;
    uint32_t fcr31 = 0;
    f1 = a1_flt(A1_R32B(a0, 12));
    f0 = a1_flt(0u);
    fcr31 = (a1f::lt(f1, f0)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
    { const bool c = (fcr31 & 0x800000u) != 0u; v0 = 1u; if (c) goto L_11464d8; }
    v0 = 0u;
L_11464d8:;
    { return a1::RetVF{(uint64_t)a1_sx64(v0), f0}; }
    __builtin_unreachable();
}

// stop_wind_emitter_geometry_pointer_reference (0x1146980): typed by tools/typed_lift2.py (round A2; leaf)
inline uint32_t stop_wind_emitter_geometry_pointer_reference(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint64_t a0)
{
    uint32_t v0 = 0;
    uint32_t v1 = 0;
    (v1 = (uint32_t)((int32_t)((int32_t)SLL32((uint32_t)a0, 2))));
    (a0 = (uint64_t)(((uint64_t)a0 < (uint64_t)(int64_t)(int32_t)80) ? 1 : 0));
    v0 = 1u;
    if ((int64_t)a0 != (int64_t)(int32_t)v0) { (a0 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL)); goto L_11469a0; }
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)289 << 16))));
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)a0, v1))));
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)a0, 28400))));
L_11469a0:;
    if ((int64_t)a0 == 0) goto L_11469c8;
    (v1 = (uint32_t)((int32_t)((int32_t)A1_R32B((uint32_t)a0, 0))));
    v0 = 2u;
    { const bool c = v1 != v0; A1_W32B((uint32_t)a0, 8, 0u); if (c) goto L_11469bc; }
    A1_W32B((uint32_t)a0, 256, 0u);
L_11469bc:;
    { v0 = 1u; return v0; }
L_11469c8:;
    { v0 = 0u; return v0; }
    __builtin_unreachable();
}

// stop_wind_emitter_callback (0x11469d0): typed by tools/typed_lift2.py (round A2; leaf)
inline uint32_t stop_wind_emitter_callback(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint64_t a0)
{
    uint32_t v0 = 0;
    uint32_t v1 = 0;
    (v1 = (uint32_t)((int32_t)((int32_t)SLL32((uint32_t)a0, 2))));
    (a0 = (uint64_t)(((uint64_t)a0 < (uint64_t)(int64_t)(int32_t)80) ? 1 : 0));
    v0 = 1u;
    if ((int64_t)a0 != (int64_t)(int32_t)v0) { (a0 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL)); goto L_11469f0; }
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)289 << 16))));
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)a0, v1))));
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)a0, 28400))));
L_11469f0:;
    if ((int64_t)a0 == 0) goto L_1146a08;
    A1_W32B((uint32_t)a0, 240, 0u);
    { v0 = 1u; return v0; }
L_1146a08:;
    { v0 = 0u; return v0; }
    __builtin_unreachable();
}

// end_wind_emitter (0x1146a10): typed by tools/typed_lift2.py (round A2; leaf)
inline uint32_t end_wind_emitter(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint64_t a0)
{
    uint32_t v0 = 0;
    uint32_t v1 = 0;
    (v1 = (uint32_t)((int32_t)((int32_t)SLL32((uint32_t)a0, 2))));
    (a0 = (uint64_t)(((uint64_t)a0 < (uint64_t)(int64_t)(int32_t)80) ? 1 : 0));
    v0 = 1u;
    if ((int64_t)a0 != (int64_t)(int32_t)v0) { (a0 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL)); goto L_1146a30; }
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)289 << 16))));
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)a0, v1))));
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)a0, 28400))));
L_1146a30:;
    if ((int64_t)a0 == 0) goto L_1146a48;
    A1_W32B((uint32_t)a0, 160, 0u);
    { v0 = 1u; return v0; }
L_1146a48:;
    { v0 = 0u; return v0; }
    __builtin_unreachable();
}

// add_wind_segment (0x1147430): typed by tools/typed_lift2.py (round A2; leaf)
inline void add_wind_segment(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt)
{
    { return; }
    __builtin_unreachable();
}

// RelocateModifiedManager (0x1147950): typed by tools/typed_lift2.py (round A2; leaf)
inline uint64_t RelocateModifiedManager(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint64_t a0, uint64_t a1, uint32_t a2)
{
    uint64_t v0 = 0;
    uint64_t v1 = 0;
    uint64_t a3 = 0;
    uint64_t t0 = 0;
    uint32_t t1 = 0;
    uint64_t hi = 0;
    uint64_t lo = 0;
    (v0 = (uint64_t)(0ULL | (uint64_t)(uint16_t)65535));
    (t1 = (uint32_t)((int32_t)((int32_t)ADD32((uint32_t)a1, 15))));
    (v0 = (uint64_t)(((int64_t)(int64_t)RC_U64X(v0) < (int64_t)(int64_t)a1) ? 1 : 0));
    (a3 = (uint64_t)(((int64_t)(int64_t)a1 < (int64_t)(int32_t)256) ? 1 : 0));
    { const bool c = (int64_t)v0 != 0; (t0 = (uint64_t)((uint64_t)a0 + (uint64_t)0ULL)); if (c) goto L_1147978; }
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 1))));
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 2))));
    if (RC_U64X(a3) != 0) v1 = (uint64_t)(v0);;
    goto L_114797c;
L_1147978:;
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 4))));
L_114797c:;
    if ((int64_t)a1 == 0) goto L_11479a8;
    { int64_t result = (int64_t)(int32_t)v1 * (int64_t)(int32_t)a1; lo = (uint64_t)(int64_t)(int32_t)result; hi = (uint64_t)(int64_t)(int32_t)(result >> 32); (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)result))); }
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 4294967280))));
    A1_W32B((uint32_t)t0, 16, a2);
    (a0 = (uint64_t)(a1_sx64(t1) & RC_U64X(v1)));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v0, 15))));
    (v0 = (uint64_t)(RC_U64X(v0) & RC_U64X(v1)));
    (a2 = (uint32_t)((int32_t)((int32_t)ADD32(a2, (uint32_t)v0))));
    A1_W32B((uint32_t)t0, 8, a2);
    (a2 = (uint32_t)((int32_t)((int32_t)ADD32(a2, (uint32_t)a0))));
L_11479a8:;
    { (v0 = (uint64_t)((uint64_t)a1_sx64(a2) + (uint64_t)0ULL)); return v0; }
    __builtin_unreachable();
}

// GetSizeOfModifiedManagerWork (0x1147a28): typed by tools/typed_lift2.py (round A2; leaf)
inline uint64_t GetSizeOfModifiedManagerWork(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint64_t a0)
{
    uint64_t v0 = 0;
    uint64_t v1 = 0;
    uint64_t a1 = 0;
    uint32_t a2 = 0;
    uint64_t hi = 0;
    uint64_t lo = 0;
    (v0 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL));
    { const bool c = (int64_t)a0 == 0; (a2 = (uint32_t)((int32_t)((int32_t)ADD32((uint32_t)a0, 15)))); if (c) goto L_1147a6c; }
    (v0 = (uint64_t)(0ULL | (uint64_t)(uint16_t)65535));
    (v0 = (uint64_t)(((int64_t)(int64_t)RC_U64X(v0) < (int64_t)(int64_t)a0) ? 1 : 0));
    if ((int64_t)v0 != 0) { (a1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 4)))); goto L_1147a54; }
    (v1 = (uint64_t)(((int64_t)(int64_t)a0 < (int64_t)(int32_t)256) ? 1 : 0));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 1))));
    (a1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 2))));
    if (RC_U64X(v1) != 0) a1 = (uint64_t)(v0);;
L_1147a54:;
    { int64_t result = (int64_t)(int32_t)a1 * (int64_t)(int32_t)a0; lo = (uint64_t)(int64_t)(int32_t)result; hi = (uint64_t)(int64_t)(int32_t)(result >> 32); (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)result))); }
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 4294967280))));
    (a0 = (uint64_t)(a1_sx64(a2) & RC_U64X(v1)));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v0, 15))));
    (v0 = (uint64_t)(RC_U64X(v0) & RC_U64X(v1)));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)a0, (uint32_t)v0))));
L_1147a6c:;
    { return v0; }
    __builtin_unreachable();
}

// sheetMgrChkStageIndexResolve (0x115ee68): typed by tools/typed_lift2.py (round A2; leaf)
inline uint64_t sheetMgrChkStageIndexResolve(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint64_t a0)
{
    uint64_t v0 = 0;
    uint32_t v1 = 0;
    uint64_t a1 = 0;
    uint32_t a2 = 0;
    uint32_t a3 = 0;
    (a3 = (uint32_t)((int32_t)((int32_t)ADD32((uint32_t)a0, 7))));
    (a1 = (uint64_t)(((int64_t)(int64_t)a0 < (int64_t)(int32_t)0) ? 1 : 0));
    (v0 = (uint64_t)((uint64_t)a0 + (uint64_t)0ULL));
    (a0 = (uint64_t)(a0 & (uint64_t)(uint16_t)7));
    a2 = 0x1230000u;
    if (RC_U64X(a1) != 0) v0 = (uint64_t)(a1_sx64(a3));;
    v1 = A1_R32(a2 - 0x5718u);
    (v0 = (uint64_t)((int64_t)(int32_t)(SRA32((int32_t)v0, 3))));
    (v1 = (uint32_t)((int32_t)((int32_t)ADD32(v1, (uint32_t)v0))));
    (v0 = (uint64_t)(uint32_t)((uint8_t)A1_R8B(v1, 0)));
    (v0 = (uint64_t)((int64_t)(int32_t)(SRA32((int32_t)v0, (uint32_t)a0 & 0x1F))));
    { (v0 = (uint64_t)(RC_U64X(v0) & (uint64_t)(uint16_t)1)); return v0; }
    __builtin_unreachable();
}

// add_camera_lws_animation_segment (0x1162510): typed by tools/typed_lift2.py (round A2; leaf)
inline void add_camera_lws_animation_segment(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt)
{
    { return; }
    __builtin_unreachable();
}

// get_camera_position (0x1162ef0): typed by tools/typed_lift2.py (round A2; leaf)
inline uint32_t get_camera_position(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t a0)
{
    uint32_t v0 = 0;
    { v0 = a0 + 0x1E0u; return v0; }
    __builtin_unreachable();
}

// get_camera_direction (0x1162ef8): typed by tools/typed_lift2.py (round A2; leaf)
inline uint32_t get_camera_direction(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t a0)
{
    uint32_t v0 = 0;
    { v0 = a0 + 0x1F0u; return v0; }
    __builtin_unreachable();
}

// get_camera_up_direction (0x1162f00): typed by tools/typed_lift2.py (round A2; leaf)
inline uint32_t get_camera_up_direction(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t a0)
{
    uint32_t v0 = 0;
    { v0 = a0 + 0x200u; return v0; }
    __builtin_unreachable();
}

// get_camera_obj_layout_class (0x1162f08): typed by tools/typed_lift2.py (round A2; leaf)
inline uint32_t get_camera_obj_layout_class(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t a0)
{
    uint32_t v0 = 0;
    { v0 = a0 + 0x20u; return v0; }
    __builtin_unreachable();
}

// get_view_matrix (0x1162fd0): typed by tools/typed_lift2.py (round A2; leaf)
inline uint32_t get_view_matrix(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t a0)
{
    uint32_t v0 = 0;
    { v0 = a0 + 0x210u; return v0; }
    __builtin_unreachable();
}

// get_manage_object_id_camera_region (0x1163750): typed by tools/typed_lift2.py (round A2; leaf)
inline uint32_t get_manage_object_id_camera_region(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint64_t a0)
{
    uint32_t v0 = 0;
    uint64_t v1 = 0;
    { const bool c = (int64_t)a0 == 0; (v1 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL)); if (c) goto L_1163768; }
    (v0 = (uint32_t)((int32_t)((int32_t)A1_R32B((uint32_t)a0, 20))));
    { const bool c = v0 != 0; (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 1)))); if (c) goto L_1163768; }
    (v1 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL));
L_1163768:;
    if ((int64_t)v1 == 0) goto L_1163780;
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)a0, 20))));
    { (v0 = (uint32_t)((int32_t)((int32_t)A1_R32B((uint32_t)v1, 4)))); return v0; }
L_1163780:;
    { v0 = 0xFFFFFFFFu; return v0; }
    __builtin_unreachable();
}

// is_parent_obj_boss_camera_region (0x1163788): typed by tools/typed_lift2.py (round A2; leaf)
inline uint32_t is_parent_obj_boss_camera_region(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint64_t a0)
{
    uint32_t v0 = 0;
    uint64_t v1 = 0;
    { const bool c = (int64_t)a0 == 0; (v1 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL)); if (c) goto L_11637a0; }
    (v0 = (uint32_t)((int32_t)((int32_t)A1_R32B((uint32_t)a0, 20))));
    { const bool c = v0 != 0; (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 1)))); if (c) goto L_11637a0; }
    (v1 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL));
L_11637a0:;
    v0 = 1u;
    if ((int64_t)v1 != (int64_t)(int32_t)v0) { v0 = 0u; goto L_11637c4; }
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)a0, 20))));
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)a0, 8))));
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v1, 4294967295))));
    (v1 = (uint64_t)(((uint64_t)RC_U64X(v1) < (uint64_t)(int64_t)(int32_t)2) ? 1 : 0));
    if ((int64_t)v1 == 0) { v0 = 0u; goto L_11637c4; }
L_11637c4:;
    { return v0; }
    __builtin_unreachable();
}

// get_priority_camera_region (0x11637d0): typed by tools/typed_lift2.py (round A2; leaf)
inline uint32_t get_priority_camera_region(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint64_t a0)
{
    uint32_t v0 = 0;
    uint64_t v1 = 0;
    { const bool c = (int64_t)a0 == 0; (v1 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL)); if (c) goto L_11637e8; }
    (v0 = (uint32_t)((int32_t)((int32_t)A1_R32B((uint32_t)a0, 20))));
    { const bool c = v0 != 0; (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 1)))); if (c) goto L_11637e8; }
    (v1 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL));
L_11637e8:;
    if ((int64_t)v1 == 0) goto L_1163800;
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)a0, 20))));
    { (v0 = (uint32_t)((int32_t)((int32_t)A1_R32B((uint32_t)v1, 16)))); return v0; }
L_1163800:;
    { v0 = 0u; return v0; }
    __builtin_unreachable();
}

// get_region_type_camera_region (0x1163888): typed by tools/typed_lift2.py (round A2; leaf)
inline uint32_t get_region_type_camera_region(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint64_t a0)
{
    uint32_t v0 = 0;
    uint64_t v1 = 0;
    { const bool c = (int64_t)a0 == 0; (v1 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL)); if (c) goto L_11638a0; }
    (v0 = (uint32_t)((int32_t)((int32_t)A1_R32B((uint32_t)a0, 20))));
    { const bool c = v0 != 0; (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 1)))); if (c) goto L_11638a0; }
    (v1 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL));
L_11638a0:;
    { const bool c = (int64_t)v1 == 0; v0 = 0xFFFFFFFFu; if (c) goto L_11638ac; }
    (v0 = (uint32_t)((int32_t)((int32_t)A1_R32B((uint32_t)a0, 0))));
L_11638ac:;
    { return v0; }
    __builtin_unreachable();
}

// get_clip_matrix (0x1164c70): typed by tools/typed_lift2.py (round A2; leaf)
inline uint32_t get_clip_matrix(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t a0)
{
    uint32_t v0 = 0;
    { v0 = a0 + 0x10u; return v0; }
    __builtin_unreachable();
}

// get_clip_matrix128 (0x1164c78): typed by tools/typed_lift2.py (round A2; leaf)
inline uint32_t get_clip_matrix128(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t a0)
{
    uint32_t v0 = 0;
    { v0 = a0 + 0x50u; return v0; }
    __builtin_unreachable();
}

// get_clip_matrix256 (0x1164c80): typed by tools/typed_lift2.py (round A2; leaf)
inline uint32_t get_clip_matrix256(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t a0)
{
    uint32_t v0 = 0;
    { v0 = a0 + 0x90u; return v0; }
    __builtin_unreachable();
}

// get_clip_matrix_fovignore (0x1164c88): typed by tools/typed_lift2.py (round A2; leaf)
inline uint32_t get_clip_matrix_fovignore(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t a0)
{
    uint32_t v0 = 0;
    { v0 = a0 + 0xD0u; return v0; }
    __builtin_unreachable();
}

// get_screen_matrix (0x1164c90): typed by tools/typed_lift2.py (round A2; leaf)
inline uint32_t get_screen_matrix(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t a0)
{
    uint32_t v0 = 0;
    { v0 = a0 + 0x110u; return v0; }
    __builtin_unreachable();
}

// get_screen_matrix128 (0x1164c98): typed by tools/typed_lift2.py (round A2; leaf)
inline uint32_t get_screen_matrix128(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t a0)
{
    uint32_t v0 = 0;
    { v0 = a0 + 0x150u; return v0; }
    __builtin_unreachable();
}

// get_screen_matrix256 (0x1164ca0): typed by tools/typed_lift2.py (round A2; leaf)
inline uint32_t get_screen_matrix256(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t a0)
{
    uint32_t v0 = 0;
    { v0 = a0 + 0x190u; return v0; }
    __builtin_unreachable();
}

// get_VC_matrix (0x1164cc0): typed by tools/typed_lift2.py (round A2; leaf)
inline uint32_t get_VC_matrix(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t a0)
{
    uint32_t v0 = 0;
    { v0 = a0 + 0x40u; return v0; }
    __builtin_unreachable();
}

// get_CS_matrix (0x1164cc8): typed by tools/typed_lift2.py (round A2; leaf)
inline uint32_t get_CS_matrix(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t a0)
{
    uint32_t v0 = 0;
    { v0 = a0 + 0x80u; return v0; }
    __builtin_unreachable();
}

// get_WVC_matrix (0x1164cd0): typed by tools/typed_lift2.py (round A2; leaf)
inline uint32_t get_WVC_matrix(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t a0)
{
    uint32_t v0 = 0;
    { v0 = a0 + 0xC0u; return v0; }
    __builtin_unreachable();
}

// get_VCS_matrix (0x1164cd8): typed by tools/typed_lift2.py (round A2; leaf)
inline uint32_t get_VCS_matrix(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t a0)
{
    uint32_t v0 = 0;
    { v0 = a0 + 0x100u; return v0; }
    __builtin_unreachable();
}

// get_WVCS_matrix (0x1164ce0): typed by tools/typed_lift2.py (round A2; leaf)
inline uint32_t get_WVCS_matrix(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t a0)
{
    uint32_t v0 = 0;
    { v0 = a0 + 0x1C0u; return v0; }
    __builtin_unreachable();
}

// get_scissor_VC_matrix (0x1164ce8): typed by tools/typed_lift2.py (round A2; leaf)
inline uint32_t get_scissor_VC_matrix(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t a0)
{
    uint32_t v0 = 0;
    { v0 = a0 + 0x200u; return v0; }
    __builtin_unreachable();
}

// get_scissor_WVC_matrix (0x1164cf0): typed by tools/typed_lift2.py (round A2; leaf)
inline uint32_t get_scissor_WVC_matrix(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t a0)
{
    uint32_t v0 = 0;
    { v0 = a0 + 0x240u; return v0; }
    __builtin_unreachable();
}

// getPointDataHeader (0x1169d18): typed by tools/typed_lift2.py (round A2; leaf)
inline uint64_t getPointDataHeader(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint64_t a0)
{
    uint64_t v0 = 0;
    uint32_t v1 = 0;
    (v0 = (uint64_t)((uint64_t)a0 + (uint64_t)0ULL));
    (v1 = (uint32_t)((int32_t)((int32_t)A1_R32B((uint32_t)v0, 8))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v0, v1))));
    v1 = v1 ^ 0u;
    { if (a1_sx64(v1) == 0) v0 = (uint64_t)0;; return v0; }
    __builtin_unreachable();
}

// getLineDataHeader (0x1169d30): typed by tools/typed_lift2.py (round A2; leaf)
inline uint64_t getLineDataHeader(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint64_t a0)
{
    uint64_t v0 = 0;
    uint32_t v1 = 0;
    (v0 = (uint64_t)((uint64_t)a0 + (uint64_t)0ULL));
    (v1 = (uint32_t)((int32_t)((int32_t)A1_R32B((uint32_t)v0, 12))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v0, v1))));
    v1 = v1 ^ 0u;
    { if (a1_sx64(v1) == 0) v0 = (uint64_t)0;; return v0; }
    __builtin_unreachable();
}

// getConnectLineArray (0x1169d88): typed by tools/typed_lift2.py (round A2; leaf)
inline uint32_t getConnectLineArray(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint64_t a0, uint64_t a1)
{
    uint32_t v0 = 0;
    uint32_t v1 = 0;
    uint64_t a2 = 0;
    { const bool c = (int64_t)a0 == 0; (a2 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL)); if (c) goto L_1169db8; }
    { const bool c = (int32_t)a1 < 0; (v0 = (uint32_t)((int32_t)((int32_t)SLL32((uint32_t)a1, 1)))); if (c) goto L_1169db8; }
    (v1 = (uint32_t)((int32_t)((int32_t)A1_R32B((uint32_t)a0, 0))));
    (v0 = (uint32_t)((int32_t)((int32_t)ADD32(v0, (uint32_t)a1))));
    v0 = v0 << 4;
    (v1 = (uint32_t)(((uint64_t)a1 < (uint64_t)a1_sx64(v1)) ? 1 : 0));
    (v0 = (uint32_t)((int32_t)((int32_t)ADD32((uint32_t)a0, v0))));
    { const bool c = v1 != 0; (a2 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(v0, 16)))); if (c) goto L_1169db8; }
    (a2 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL));
L_1169db8:;
    if ((int64_t)a2 == 0) goto L_1169dd8;
    (v0 = (uint32_t)((int32_t)((int32_t)A1_R32B((uint32_t)a2, 16))));
    if (v0 == 0) goto L_1169dd8;
    (v0 = (uint32_t)((int32_t)((int32_t)A1_R32B((uint32_t)a2, 20))));
    { (v0 = (uint32_t)((int32_t)((int32_t)ADD32((uint32_t)a0, v0)))); return v0; }
L_1169dd8:;
    { v0 = 0u; return v0; }
    __builtin_unreachable();
}

// getRootDataHeader (0x1169e20): typed by tools/typed_lift2.py (round A2; leaf)
inline uint64_t getRootDataHeader(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint64_t a0)
{
    uint64_t v0 = 0;
    uint32_t v1 = 0;
    (v0 = (uint64_t)((uint64_t)a0 + (uint64_t)0ULL));
    (v1 = (uint32_t)((int32_t)((int32_t)A1_R32B((uint32_t)v0, 16))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v0, v1))));
    v1 = v1 ^ 0u;
    { if (a1_sx64(v1) == 0) v0 = (uint64_t)0;; return v0; }
    __builtin_unreachable();
}

// getGoalDataHeader (0x1169e38): typed by tools/typed_lift2.py (round A2; leaf)
inline uint64_t getGoalDataHeader(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint64_t a0)
{
    uint64_t v0 = 0;
    uint32_t v1 = 0;
    (v0 = (uint64_t)((uint64_t)a0 + (uint64_t)0ULL));
    (v1 = (uint32_t)((int32_t)((int32_t)A1_R32B((uint32_t)v0, 20))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v0, v1))));
    v1 = v1 ^ 0u;
    { if (a1_sx64(v1) == 0) v0 = (uint64_t)0;; return v0; }
    __builtin_unreachable();
}

// getSubDataHeader (0x1169e50): typed by tools/typed_lift2.py (round A2; leaf)
inline uint64_t getSubDataHeader(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint64_t a0)
{
    uint64_t v0 = 0;
    uint32_t v1 = 0;
    (v0 = (uint64_t)((uint64_t)a0 + (uint64_t)0ULL));
    (v1 = (uint32_t)((int32_t)((int32_t)A1_R32B((uint32_t)v0, 24))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v0, v1))));
    v1 = v1 ^ 0u;
    { if (a1_sx64(v1) == 0) v0 = (uint64_t)0;; return v0; }
    __builtin_unreachable();
}

// getChildGoalPointIdxArray (0x1169f28): typed by tools/typed_lift2.py (round A2; leaf)
inline uint32_t getChildGoalPointIdxArray(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint64_t a0, uint64_t a1)
{
    uint32_t v0 = 0;
    uint32_t v1 = 0;
    uint64_t a2 = 0;
    { const bool c = (int64_t)a0 == 0; (a2 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL)); if (c) goto L_1169f58; }
    { const bool c = (int32_t)a1 < 0; (v0 = (uint32_t)((int32_t)((int32_t)SLL32((uint32_t)a1, 1)))); if (c) goto L_1169f58; }
    (v1 = (uint32_t)((int32_t)((int32_t)A1_R32B((uint32_t)a0, 0))));
    (v0 = (uint32_t)((int32_t)((int32_t)ADD32(v0, (uint32_t)a1))));
    v0 = v0 << 4;
    (v1 = (uint32_t)(((uint64_t)a1 < (uint64_t)a1_sx64(v1)) ? 1 : 0));
    (v0 = (uint32_t)((int32_t)((int32_t)ADD32((uint32_t)a0, v0))));
    { const bool c = v1 != 0; (a2 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(v0, 16)))); if (c) goto L_1169f58; }
    (a2 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL));
L_1169f58:;
    if ((int64_t)a2 == 0) goto L_1169f78;
    (v0 = (uint32_t)((int32_t)((int32_t)A1_R32B((uint32_t)a2, 20))));
    if (v0 == 0) goto L_1169f78;
    (v0 = (uint32_t)((int32_t)((int32_t)A1_R32B((uint32_t)a2, 24))));
    { (v0 = (uint32_t)((int32_t)((int32_t)ADD32((uint32_t)a0, v0)))); return v0; }
L_1169f78:;
    { v0 = 0u; return v0; }
    __builtin_unreachable();
}

// getChildSubPointIdxArray (0x1169f80): typed by tools/typed_lift2.py (round A2; leaf)
inline uint32_t getChildSubPointIdxArray(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint64_t a0, uint64_t a1)
{
    uint32_t v0 = 0;
    uint32_t v1 = 0;
    uint64_t a2 = 0;
    { const bool c = (int64_t)a0 == 0; (a2 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL)); if (c) goto L_1169fb0; }
    { const bool c = (int32_t)a1 < 0; (v0 = (uint32_t)((int32_t)((int32_t)SLL32((uint32_t)a1, 1)))); if (c) goto L_1169fb0; }
    (v1 = (uint32_t)((int32_t)((int32_t)A1_R32B((uint32_t)a0, 0))));
    (v0 = (uint32_t)((int32_t)((int32_t)ADD32(v0, (uint32_t)a1))));
    v0 = v0 << 4;
    (v1 = (uint32_t)(((uint64_t)a1 < (uint64_t)a1_sx64(v1)) ? 1 : 0));
    (v0 = (uint32_t)((int32_t)((int32_t)ADD32((uint32_t)a0, v0))));
    { const bool c = v1 != 0; (a2 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(v0, 16)))); if (c) goto L_1169fb0; }
    (a2 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL));
L_1169fb0:;
    if ((int64_t)a2 == 0) goto L_1169fd0;
    (v0 = (uint32_t)((int32_t)((int32_t)A1_R32B((uint32_t)a2, 20))));
    if (v0 == 0) goto L_1169fd0;
    (v0 = (uint32_t)((int32_t)((int32_t)A1_R32B((uint32_t)a2, 24))));
    { (v0 = (uint32_t)((int32_t)((int32_t)ADD32((uint32_t)a0, v0)))); return v0; }
L_1169fd0:;
    { v0 = 0u; return v0; }
    __builtin_unreachable();
}

// getInterestDataHeader (0x1169fd8): typed by tools/typed_lift2.py (round A2; leaf)
inline uint64_t getInterestDataHeader(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint64_t a0)
{
    uint64_t v0 = 0;
    uint32_t v1 = 0;
    (v0 = (uint64_t)((uint64_t)a0 + (uint64_t)0ULL));
    (v1 = (uint32_t)((int32_t)((int32_t)A1_R32B((uint32_t)v0, 32))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v0, v1))));
    v1 = v1 ^ 0u;
    { if (a1_sx64(v1) == 0) v0 = (uint64_t)0;; return v0; }
    __builtin_unreachable();
}

// getDeadDataHeader (0x116a028): typed by tools/typed_lift2.py (round A2; leaf)
inline uint64_t getDeadDataHeader(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint64_t a0)
{
    uint64_t v0 = 0;
    uint32_t v1 = 0;
    (v0 = (uint64_t)((uint64_t)a0 + (uint64_t)0ULL));
    (v1 = (uint32_t)((int32_t)((int32_t)A1_R32B((uint32_t)v0, 36))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v0, v1))));
    v1 = v1 ^ 0u;
    { if (a1_sx64(v1) == 0) v0 = (uint64_t)0;; return v0; }
    __builtin_unreachable();
}

// getLayoutDataHeader (0x116a080): typed by tools/typed_lift2.py (round A2; leaf)
inline uint64_t getLayoutDataHeader(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint64_t a0)
{
    uint64_t v0 = 0;
    uint32_t v1 = 0;
    (v0 = (uint64_t)((uint64_t)a0 + (uint64_t)0ULL));
    (v1 = (uint32_t)((int32_t)((int32_t)A1_R32B((uint32_t)v0, 40))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v0, v1))));
    v1 = v1 ^ 0u;
    { if (a1_sx64(v1) == 0) v0 = (uint64_t)0;; return v0; }
    __builtin_unreachable();
}

// GAMEUTIL_is_push_or_down_1p_button (0x1342ef8): typed by tools/typed_lift2.py (round A2; leaf)
inline uint32_t GAMEUTIL_is_push_or_down_1p_button(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint64_t a0, uint32_t a1)
{
    uint32_t v0 = 0;
    uint64_t v1 = 0;
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 1))));
    { const bool c = (int64_t)a0 != (int64_t)v1; v0 = 2u; if (c) goto L_1342f18; }
    { const uint32_t _dynInsn = A1_R32(0x1342F04u); const int32_t _dynSimm = (int16_t)(_dynInsn & 0xFFFFu); const uint32_t _dynImm = _dynInsn & 0xFFFFu; (void)_dynSimm; (void)_dynImm; (v0 = (uint32_t)((int32_t)((int32_t)((uint32_t)_dynImm << 16)))); }
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)SLL32((uint32_t)v1, a1 & 0x1F))));
    { const uint32_t _dynInsn = A1_R32(0x1342F10u); const int32_t _dynSimm = (int16_t)(_dynInsn & 0xFFFFu); const uint32_t _dynImm = _dynInsn & 0xFFFFu; (void)_dynSimm; (void)_dynImm; (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32(ADD32(v0, _dynSimm))))); }
    goto L_1342f44;
L_1342f18:;
    { const bool c = (int64_t)a0 != (int64_t)(int32_t)v0; v0 = 3u; if (c) goto L_1342f30; }
    { const uint32_t _dynInsn = A1_R32(0x1342F20u); const int32_t _dynSimm = (int16_t)(_dynInsn & 0xFFFFu); const uint32_t _dynImm = _dynInsn & 0xFFFFu; (void)_dynSimm; (void)_dynImm; (v0 = (uint32_t)((int32_t)((int32_t)((uint32_t)_dynImm << 16)))); }
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)SLL32((uint32_t)v1, a1 & 0x1F))));
    { const uint32_t _dynInsn = A1_R32(0x1342F2Cu); const int32_t _dynSimm = (int16_t)(_dynInsn & 0xFFFFu); const uint32_t _dynImm = _dynInsn & 0xFFFFu; (void)_dynSimm; (void)_dynImm; (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32(ADD32(v0, _dynSimm))))); }
    goto L_1342f44;
L_1342f30:;
    { const bool c = (int64_t)a0 != (int64_t)(int32_t)v0; { const uint32_t _dynInsn = A1_R32(0x1342F34u); const int32_t _dynSimm = (int16_t)(_dynInsn & 0xFFFFu); const uint32_t _dynImm = _dynInsn & 0xFFFFu; (void)_dynSimm; (void)_dynImm; (v0 = (uint32_t)((int32_t)((int32_t)((uint32_t)_dynImm << 16)))); } if (c) goto L_1342f58; }
    { const uint32_t _dynInsn = A1_R32(0x1342F38u); const int32_t _dynSimm = (int16_t)(_dynInsn & 0xFFFFu); const uint32_t _dynImm = _dynInsn & 0xFFFFu; (void)_dynSimm; (void)_dynImm; (v0 = (uint32_t)((int32_t)((int32_t)((uint32_t)_dynImm << 16)))); }
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)SLL32((uint32_t)v1, a1 & 0x1F))));
    { const uint32_t _dynInsn = A1_R32(0x1342F40u); const int32_t _dynSimm = (int16_t)(_dynInsn & 0xFFFFu); const uint32_t _dynImm = _dynInsn & 0xFFFFu; (void)_dynSimm; (void)_dynImm; (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32(ADD32(v0, _dynSimm))))); }
L_1342f44:;
    (v1 = (uint64_t)(RC_U64X(v1) & RC_U64X(a0)));
    if ((int64_t)v1 != 0) goto L_1342f6c;
    { v0 = 0u; return v0; }
L_1342f58:;
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)SLL32((uint32_t)v1, a1 & 0x1F))));
    { const uint32_t _dynInsn = A1_R32(0x1342F5Cu); const int32_t _dynSimm = (int16_t)(_dynInsn & 0xFFFFu); const uint32_t _dynImm = _dynInsn & 0xFFFFu; (void)_dynSimm; (void)_dynImm; (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32(ADD32(v0, _dynSimm))))); }
    (v1 = (uint64_t)(RC_U64X(v1) & RC_U64X(a0)));
    if ((int64_t)v1 == 0) { v0 = 0u; goto L_1342f78; }
L_1342f6c:;
    { v0 = 1u; return v0; }
L_1342f78:;
    { return v0; }
    __builtin_unreachable();
}

// GAMEUTIL_get_data_record_info (0x1344588): typed by tools/typed_lift2.py (round A2; leaf)
inline uint32_t GAMEUTIL_get_data_record_info(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t a0, uint64_t a1)
{
    uint32_t v0 = 0;
    uint32_t v1 = 0;
    uint64_t hi = 0;
    uint64_t lo = 0;
    if ((int32_t)a1 < 0) goto L_13445b8;
    v0 = A1_R32B(a0, 12);
    (v0 = (uint32_t)(((int64_t)(int64_t)a1 < (int64_t)(int64_t)a1_sx64(v0)) ? 1 : 0));
    if (v0 == 0) goto L_13445b8;
    v0 = A1_R32(a0);
    v1 = A1_R32B(a0, 8);
    { int64_t result = (int64_t)(int32_t)a1 * (int64_t)(int32_t)v0; lo = (uint64_t)(int64_t)(int32_t)result; hi = (uint64_t)(int64_t)(int32_t)(result >> 32); (v0 = (uint32_t)((int32_t)((int32_t)result))); }
    { v0 = v1 + v0; return v0; }
L_13445b8:;
    { v0 = 0u; return v0; }
    __builtin_unreachable();
}

// gStatSaveDataGameFlagChk (0x1353580): typed by tools/typed_lift2.py (round A2; leaf)
inline uint64_t gStatSaveDataGameFlagChk(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint64_t a0)
{
    uint64_t v0 = 0;
    uint64_t v1 = 0;
    uint32_t a1 = 0;
    (v0 = (uint64_t)(((int64_t)(int64_t)a0 < (int64_t)(int32_t)0) ? 1 : 0));
    (a1 = (uint32_t)((int32_t)((int32_t)ADD32((uint32_t)a0, 7))));
    (v1 = (uint64_t)((uint64_t)a0 + (uint64_t)0ULL));
    (a0 = (uint64_t)(a0 & (uint64_t)(uint16_t)7));
    if (RC_U64X(v0) != 0) v1 = (uint64_t)(a1_sx64(a1));;
    (v1 = (uint64_t)((int64_t)(int32_t)(SRA32((int32_t)v1, 3))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)286 << 16))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v0, (uint32_t)v1))));
    (v0 = (uint64_t)(uint32_t)((uint8_t)A1_R8B((uint32_t)v0, 400)));
    (v0 = (uint64_t)((int64_t)(int32_t)(SRA32((int32_t)v0, (uint32_t)a0 & 0x1F))));
    { (v0 = (uint64_t)(RC_U64X(v0) & (uint64_t)(uint16_t)1)); return v0; }
    __builtin_unreachable();
}

// gStatGetGObjAdrs (0x13536d8): typed by tools/typed_lift2.py (round A2; leaf)
inline uint32_t gStatGetGObjAdrs(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t a0)
{
    uint32_t v0 = 0;
    { v0 = A1_R32B(a0, 12); return v0; }
    __builtin_unreachable();
}

// getstagenumber (0x135ff08): typed by tools/typed_lift2.py (round A2; leaf)
inline uint64_t getstagenumber(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint64_t a0)
{
    uint64_t v0 = 0;
    uint64_t v1 = 0;
    uint32_t a1 = 0;
    uint32_t a2 = 0;
    uint32_t a3 = 0;
    uint64_t t0 = 0;
    uint64_t hi = 0;
    uint64_t lo = 0;
    (t0 = (uint64_t)((uint64_t)a0 + (uint64_t)0ULL));
    a1 = 0x1460000u;
    (v1 = (uint64_t)((int64_t)(int32_t)(SRA32((int32_t)t0, 5))));
    a3 = 0x1460000u;
    (v1 = (uint64_t)(RC_U64X(v1) & (uint64_t)(uint16_t)1023));
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B(a1, 25524))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)SLL32((uint32_t)v1, 2))));
    a2 = A1_R32B(a3, 25516);
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
    (a2 = (uint32_t)((int32_t)((int32_t)A1_R32B((uint32_t)a0, 0))));
    (v0 = (uint64_t)(a1_sx64(a2) & a1_sx64(a1)));
    { const bool c = (int64_t)v0 == 0; (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)SUB32((uint32_t)a0, (uint32_t)v0)))); if (c) goto L_135ff68; }
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)v0, 0))));
    goto L_135ff6c;
L_135ff68:;
    (a0 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL));
L_135ff6c:;
    { const bool c = (int64_t)a0 == 0; { const uint32_t _dynInsn = A1_R32(0x135FF70u); const int32_t _dynSimm = (int16_t)(_dynInsn & 0xFFFFu); const uint32_t _dynImm = _dynInsn & 0xFFFFu; (void)_dynSimm; (void)_dynImm; (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)_dynImm << 16)))); } if (c) goto L_135ffa0; }
    { const uint32_t _dynInsn = A1_R32(0x135FF74u); const int32_t _dynSimm = (int16_t)(_dynInsn & 0xFFFFu); const uint32_t _dynImm = _dynInsn & 0xFFFFu; (void)_dynSimm; (void)_dynImm; (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32(ADD32((uint32_t)v0, _dynSimm))))); }
    { const bool c = (int64_t)a0 == (int64_t)v1; (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)SRL32(a2, 18)))); if (c) goto L_135ffa0; }
    (a1 = (uint32_t)((int32_t)((int32_t)A1_R32B((uint32_t)a0, 8))));
    (v0 = (uint64_t)(RC_U64X(v0) & (uint64_t)(uint16_t)4095));
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)a0, 16))));
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B(a1, 12))));
    { int64_t result = (int64_t)(int32_t)v0 * (int64_t)(int32_t)v1; lo = (uint64_t)(int64_t)(int32_t)result; hi = (uint64_t)(int64_t)(int32_t)(result >> 32); (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)result))); }
    (a1 = (uint32_t)((int32_t)((int32_t)ADD32((uint32_t)a0, (uint32_t)v0))));
    goto L_135ffa4;
L_135ffa0:;
    a1 = 0u;
L_135ffa4:;
    { const bool c = a1 == 0; (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)3 << 16)))); if (c) goto L_1360060; }
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)SLL32((uint32_t)t0, 27))));
    (v0 = (uint64_t)((int64_t)(int32_t)(SRA32((int32_t)v0, 27))));
    (a0 = (uint64_t)(RC_U64X(a0) | (uint64_t)(uint16_t)65535));
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)SLL32((uint32_t)v0, 4))));
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)SUB32((uint32_t)v1, (uint32_t)v0))));
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)SLL32((uint32_t)v1, 2))));
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v1, a1))));
    (a2 = (uint32_t)((int32_t)((int32_t)A1_R32B((uint32_t)v1, 0))));
    (v0 = (uint64_t)(a1_sx64(a2) & RC_U64X(a0)));
    { const bool c = (int64_t)v0 == 0; (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)SUB32((uint32_t)v1, (uint32_t)v0)))); if (c) goto L_135ffe0; }
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)v0, 0))));
    goto L_135ffe4;
L_135ffe0:;
    (a0 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL));
L_135ffe4:;
    { const bool c = (int64_t)a0 == 0; { const uint32_t _dynInsn = A1_R32(0x135FFE8u); const int32_t _dynSimm = (int16_t)(_dynInsn & 0xFFFFu); const uint32_t _dynImm = _dynInsn & 0xFFFFu; (void)_dynSimm; (void)_dynImm; (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)_dynImm << 16)))); } if (c) goto L_1360018; }
    { const uint32_t _dynInsn = A1_R32(0x135FFECu); const int32_t _dynSimm = (int16_t)(_dynInsn & 0xFFFFu); const uint32_t _dynImm = _dynInsn & 0xFFFFu; (void)_dynSimm; (void)_dynImm; (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32(ADD32((uint32_t)v0, _dynSimm))))); }
    { const bool c = (int64_t)a0 == (int64_t)v1; (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)SRL32(a2, 18)))); if (c) goto L_1360018; }
    (a1 = (uint32_t)((int32_t)((int32_t)A1_R32B((uint32_t)a0, 8))));
    (v0 = (uint64_t)(RC_U64X(v0) & (uint64_t)(uint16_t)4095));
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)a0, 16))));
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B(a1, 12))));
    { int64_t result = (int64_t)(int32_t)v0 * (int64_t)(int32_t)v1; lo = (uint64_t)(int64_t)(int32_t)result; hi = (uint64_t)(int64_t)(int32_t)(result >> 32); (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)result))); }
    (a1 = (uint32_t)((int32_t)((int32_t)ADD32((uint32_t)a0, (uint32_t)v0))));
    goto L_136001c;
L_1360018:;
    a1 = 0u;
L_136001c:;
    { const bool c = a1 == 0; { const uint32_t _dynInsn = A1_R32(0x1360020u); const int32_t _dynSimm = (int16_t)(_dynInsn & 0xFFFFu); const uint32_t _dynImm = _dynInsn & 0xFFFFu; (void)_dynSimm; (void)_dynImm; (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)_dynImm << 16)))); } if (c) goto L_1360060; }
    { const uint32_t _dynInsn = A1_R32(0x1360024u); const int32_t _dynSimm = (int16_t)(_dynInsn & 0xFFFFu); const uint32_t _dynImm = _dynInsn & 0xFFFFu; (void)_dynSimm; (void)_dynImm; (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)_dynImm << 16)))); }
    { const uint32_t _dynInsn = A1_R32(0x1360028u); const int32_t _dynSimm = (int16_t)(_dynInsn & 0xFFFFu); const uint32_t _dynImm = _dynInsn & 0xFFFFu; (void)_dynSimm; (void)_dynImm; (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32(ADD32((uint32_t)v0, _dynSimm))))); }
    { const uint32_t _dynInsn = A1_R32(0x136002Cu); const int32_t _dynSimm = (int16_t)(_dynInsn & 0xFFFFu); const uint32_t _dynImm = _dynInsn & 0xFFFFu; (void)_dynSimm; (void)_dynImm; (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v1, _dynSimm)))); }
    (a0 = (uint64_t)(RC_U64X(a0) ^ RC_U64X(v1)));
    if (RC_U64X(a0) == 0) v1 = (uint64_t)0;;
    { const bool c = (int64_t)v1 == 0; (v0 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL)); if (c) goto L_1360044; }
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)v1, 16))));
L_1360044:;
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)SUB32(a1, (uint32_t)v0))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)52428 << 16))));
    (v1 = (uint64_t)((int64_t)(int32_t)(SRA32((int32_t)v1, 2))));
    (v0 = (uint64_t)(RC_U64X(v0) | (uint64_t)(uint16_t)52429));
    { { int64_t result = (int64_t)(int32_t)v1 * (int64_t)(int32_t)v0; lo = (uint64_t)(int64_t)(int32_t)result; hi = (uint64_t)(int64_t)(int32_t)(result >> 32); (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)result))); } return v0; }
L_1360060:;
    { (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 4294967295)))); return v0; }
    __builtin_unreachable();
}

// initStageLayout (0x1366cf0): typed by tools/typed_lift2.py (round A2; leaf)
inline void initStageLayout(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt)
{
    { return; }
    __builtin_unreachable();
}

// deleteStageLayout (0x1366cf8): typed by tools/typed_lift2.py (round A2; leaf)
inline void deleteStageLayout(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt)
{
    { return; }
    __builtin_unreachable();
}

// initAdpcmOrient (0x136a420): typed by tools/typed_lift2.py (round A2; leaf)
inline void initAdpcmOrient(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt)
{
    { return; }
    __builtin_unreachable();
}

// bgmStartGameOver (0x136aae8): typed by tools/typed_lift2.py (round A2; leaf)
inline void bgmStartGameOver(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt)
{
    { return; }
    __builtin_unreachable();
}

// bgmStageInit (0x136aaf8): typed by tools/typed_lift2.py (round A2; leaf)
inline void bgmStageInit(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt)
{
    { return; }
    __builtin_unreachable();
}

// SCRExecBossPostProcess (0x13790a0): typed by tools/typed_lift2.py (round A2; leaf)
inline void SCRExecBossPostProcess(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt)
{
    { return; }
    __builtin_unreachable();
}

// SCRPlayerReset (0x13b6198): typed by tools/typed_lift2.py (round A2; leaf)
inline void SCRPlayerReset(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt)
{
    { return; }
    __builtin_unreachable();
}

// gamecore_sub_013B6530 (0x13b6530): typed by tools/typed_lift2.py (round A2; leaf)
inline void gamecore_sub_013B6530(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt)
{
    { return; }
    __builtin_unreachable();
}

// scrInit (0x13cfb60): typed by tools/typed_lift2.py (round A2; leaf)
inline void scrInit(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt)
{
    { return; }
    __builtin_unreachable();
}

// AI_CheckNormZero (0x13d4df8): typed by tools/typed_lift2.py (round A2; leaf)
inline a1::RetVF AI_CheckNormZero(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t a0)
{
    float f0 = 0.0f;
    float f1 = 0.0f;
    uint32_t at = 0;
    uint32_t v0 = 0;
    uint32_t fcr31 = 0;
    f0 = a1_flt(0u);
    f1 = a1_flt(A1_R32(a0));
    fcr31 = (a1f::lt(f1, f0)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
    if ((fcr31 & 0x800000u) == 0u) goto L_13d4e30;
    at = 0xB8D10000u;
    at = at | 0xB717u;
    f0 = a1_flt(at);
    fcr31 = (a1f::lt(f0, f1)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
    if ((fcr31 & 0x800000u) != 0u) { f1 = a1_flt(A1_R32B(a0, 4)); goto L_13d4e4c; }
    { v0 = 1u; return a1::RetVF{(uint64_t)a1_sx64(v0), f0}; }
L_13d4e30:;
    at = 0x38D10000u;
    at = at | 0xB717u;
    f0 = a1_flt(at);
    fcr31 = (a1f::lt(f1, f0)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
    { const bool c = (fcr31 & 0x800000u) == 0u; v0 = 1u; if (c) goto L_13d4ef0; }
    f1 = a1_flt(A1_R32B(a0, 4));
L_13d4e4c:;
    f0 = a1_flt(0u);
    fcr31 = (a1f::lt(f1, f0)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
    if ((fcr31 & 0x800000u) == 0u) goto L_13d4e80;
    at = 0xB8D10000u;
    at = at | 0xB717u;
    f0 = a1_flt(at);
    fcr31 = (a1f::lt(f0, f1)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
    if ((fcr31 & 0x800000u) != 0u) { f1 = a1_flt(A1_R32B(a0, 8)); goto L_13d4e9c; }
    { v0 = 1u; return a1::RetVF{(uint64_t)a1_sx64(v0), f0}; }
L_13d4e80:;
    at = 0x38D10000u;
    at = at | 0xB717u;
    f0 = a1_flt(at);
    fcr31 = (a1f::lt(f1, f0)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
    { const bool c = (fcr31 & 0x800000u) == 0u; v0 = 1u; if (c) goto L_13d4ef0; }
    f1 = a1_flt(A1_R32B(a0, 8));
L_13d4e9c:;
    f0 = a1_flt(0u);
    fcr31 = (a1f::lt(f1, f0)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
    if ((fcr31 & 0x800000u) == 0u) goto L_13d4ed0;
    at = 0xB8D10000u;
    at = at | 0xB717u;
    f0 = a1_flt(at);
    fcr31 = (a1f::lt(f0, f1)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
    if ((fcr31 & 0x800000u) != 0u) goto L_13d4ee8;
    { v0 = 1u; return a1::RetVF{(uint64_t)a1_sx64(v0), f0}; }
L_13d4ed0:;
    at = 0x38D10000u;
    at = at | 0xB717u;
    f0 = a1_flt(at);
    fcr31 = (a1f::lt(f1, f0)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
    if ((fcr31 & 0x800000u) == 0u) { v0 = 1u; goto L_13d4ef0; }
L_13d4ee8:;
    { v0 = 0u; return a1::RetVF{(uint64_t)a1_sx64(v0), f0}; }
L_13d4ef0:;
    { return a1::RetVF{(uint64_t)a1_sx64(v0), f0}; }
    __builtin_unreachable();
}

// initSavePoint (0x140ae18): typed by tools/typed_lift2.py (round A2; leaf)
inline void initSavePoint(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt)
{
    { return; }
    __builtin_unreachable();
}

// getSprLogWorkSize (0x140ae38): typed by tools/typed_lift2.py (round A2; leaf)
inline uint32_t getSprLogWorkSize(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt)
{
    uint32_t v0 = 0;
    { v0 = 0x2D00u; return v0; }
    __builtin_unreachable();
}

// getSprLogStatusSize (0x140ae50): typed by tools/typed_lift2.py (round A2; leaf)
inline uint32_t getSprLogStatusSize(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt)
{
    uint32_t v0 = 0;
    { v0 = 0xCu; return v0; }
    __builtin_unreachable();
}

// get_camera_move_collision_group (0x141ce38): typed by tools/typed_lift2.py (round A2; leaf)
inline uint32_t get_camera_move_collision_group(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t a0)
{
    uint32_t v0 = 0;
    { v0 = A1_R32B(a0, 12); return v0; }
    __builtin_unreachable();
}

// get_game_camera (0x141ce58): typed by tools/typed_lift2.py (round A2; leaf)
inline uint32_t get_game_camera(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t a0)
{
    uint32_t v0 = 0;
    { v0 = a0 + 0x20u; return v0; }
    __builtin_unreachable();
}

// get_cur_camera_target (0x141ce60): typed by tools/typed_lift2.py (round A2; leaf)
inline uint32_t get_cur_camera_target(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t a0)
{
    uint32_t v0 = 0;
    { v0 = A1_R32B(a0, 28); return v0; }
    __builtin_unreachable();
}

// gamecore_sub_0141D178 (0x141d178): typed by tools/typed_lift2.py (round A2; leaf)
inline void gamecore_sub_0141D178(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt)
{
    { return; }
    __builtin_unreachable();
}

// gamecore_sub_0141D180 (0x141d180): typed by tools/typed_lift2.py (round A2; leaf)
inline void gamecore_sub_0141D180(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt)
{
    { return; }
    __builtin_unreachable();
}

// gamecore_sub_0141D4F0 (0x141d4f0): typed by tools/typed_lift2.py (round A2; leaf)
inline void gamecore_sub_0141D4F0(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt)
{
    { return; }
    __builtin_unreachable();
}

// get_cur_using_lws_anime_mode_base_class (0x1424f38): typed by tools/typed_lift2.py (round A2; leaf)
inline uint64_t get_cur_using_lws_anime_mode_base_class(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint64_t a0)
{
    uint64_t v0 = 0;
    uint64_t v1 = 0;
    uint32_t a1 = 0;
    (v0 = (uint64_t)((uint64_t)a0 + (uint64_t)0ULL));
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 7))));
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)v0, 4960))));
    (a1 = (uint32_t)((int32_t)((int32_t)ADD32((uint32_t)v0, 13536))));
    { const bool c = (int64_t)a0 == (int64_t)v1; (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v0, 12912)))); if (c) goto L_1424f60; }
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 8))));
    (v0 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL));
    (v1 = (uint64_t)(RC_U64X(a0) ^ RC_U64X(v1)));
    if (RC_U64X(v1) == 0) v0 = (uint64_t)(a1_sx64(a1));;
L_1424f60:;
    { return v0; }
    __builtin_unreachable();
}

// end_object_interest_game_camera (0x1425050): typed by tools/typed_lift2.py (round A2; leaf)
inline void end_object_interest_game_camera(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt)
{
    { return; }
    __builtin_unreachable();
}

// free_camera_mode_manager_segment (0x1425660): typed by tools/typed_lift2.py (round A2; leaf)
inline void free_camera_mode_manager_segment(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt)
{
    { return; }
    __builtin_unreachable();
}

// gamecore_sub_01425D20 (0x1425d20): typed by tools/typed_lift2.py (round A2; leaf)
inline uint32_t gamecore_sub_01425D20(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint64_t a0)
{
    uint32_t v0 = 0;
    uint32_t v1 = 0;
    uint64_t a1 = 0;
    uint32_t a2 = 0;
    (v1 = (uint32_t)((int32_t)((int32_t)A1_R32B((uint32_t)a0, 4960))));
    v0 = 7u;
    (a2 = (uint32_t)((int32_t)((int32_t)ADD32((uint32_t)a0, 13536))));
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)a0, 12912))));
    { const bool c = v1 == v0; (a1 = (uint64_t)(a1_sx64(v1) ^ (uint64_t)(uint16_t)8)); if (c) goto L_1425d40; }
    (a0 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL));
    if (RC_U64X(a1) == 0) a0 = (uint64_t)(a1_sx64(a2));;
L_1425d40:;
    if (v1 == 0) goto L_1425d74;
    if ((int64_t)a0 == 0) goto L_1425d80;
    (v1 = (uint32_t)((int32_t)((int32_t)A1_R32B((uint32_t)a0, 140))));
    if (v1 == 0) goto L_1425d74;
    v0 = 2u;
    if (v1 == v0) goto L_1425d74;
    (v0 = (uint32_t)((int32_t)((int32_t)A1_R32B((uint32_t)a0, 152))));
    if (v0 == 0) goto L_1425d80;
L_1425d74:;
    { v0 = 1u; return v0; }
L_1425d80:;
    { v0 = 0u; return v0; }
    __builtin_unreachable();
}

// gamecore_sub_01426250 (0x1426250): typed by tools/typed_lift2.py (round A2; leaf)
inline void gamecore_sub_01426250(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt)
{
    { return; }
    __builtin_unreachable();
}

// gamecore_sub_014262B0 (0x14262b0): typed by tools/typed_lift2.py (round A2; leaf)
inline void gamecore_sub_014262B0(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt)
{
    { return; }
    __builtin_unreachable();
}

// gamecore_sub_014262B8 (0x14262b8): typed by tools/typed_lift2.py (round A2; leaf)
inline void gamecore_sub_014262B8(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt)
{
    { return; }
    __builtin_unreachable();
}

// gamecore_sub_01426368 (0x1426368): typed by tools/typed_lift2.py (round A2; leaf)
inline void gamecore_sub_01426368(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt)
{
    { return; }
    __builtin_unreachable();
}

// get_interp_game_camera_param_game_camera_param_manager (0x142c120): typed by tools/typed_lift2.py (round A2; leaf)
inline uint32_t get_interp_game_camera_param_game_camera_param_manager(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t a0)
{
    uint32_t v0 = 0;
    { v0 = a0 + 0xCB8u; return v0; }
    __builtin_unreachable();
}

// get_mode_common_data_filter_base_dist_to_look_at (0x142c428): typed by tools/typed_lift2.py (round A2; leaf)
inline a1::RetVF get_mode_common_data_filter_base_dist_to_look_at(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t a0)
{
    float f0 = 0.0f;
    uint32_t v0 = 0;
    v0 = A1_R32(a0);
    f0 = a1_flt(A1_R32B(v0, 336));
    { return a1::RetVF{(uint64_t)a1_sx64(v0), f0}; }
    __builtin_unreachable();
}

// delISysPathObj (0x1431c30): typed by tools/typed_lift2.py (round A2; leaf)
inline void delISysPathObj(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt)
{
    { return; }
    __builtin_unreachable();
}

// setDataHeaderFromAreaIdx (0x1431da0): typed by tools/typed_lift2.py (round A2; leaf)
inline uint32_t setDataHeaderFromAreaIdx(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt)
{
    uint32_t v0 = 0;
    { v0 = 1u; return v0; }
    __builtin_unreachable();
}

// SCRConditionTrue (0x14563b8): typed by tools/typed_lift2.py (round A2; leaf)
inline uint32_t SCRConditionTrue(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt)
{
    uint32_t v0 = 0;
    { v0 = 1u; return v0; }
    __builtin_unreachable();
}

// getSizeAimIKWork (0x145d688): typed by tools/typed_lift2.py (round A2; leaf)
inline uint32_t getSizeAimIKWork(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt)
{
    uint32_t v0 = 0;
    { v0 = 0x30u; return v0; }
    __builtin_unreachable();
}

// getSizeAttackIKWork (0x145dfc8): typed by tools/typed_lift2.py (round A2; leaf)
inline uint32_t getSizeAttackIKWork(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt)
{
    uint32_t v0 = 0;
    { v0 = 0x30u; return v0; }
    __builtin_unreachable();
}

// getSizeBeardIKWork (0x145f0e0): typed by tools/typed_lift2.py (round A2; leaf)
inline uint32_t getSizeBeardIKWork(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt)
{
    uint32_t v0 = 0;
    { v0 = 0x3D0u; return v0; }
    __builtin_unreachable();
}

// getSizeTailIKWork (0x14621b0): typed by tools/typed_lift2.py (round A2; leaf)
inline uint32_t getSizeTailIKWork(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt)
{
    uint32_t v0 = 0;
    { v0 = 0x3D0u; return v0; }
    __builtin_unreachable();
}

// getEventScriptClass (0x1464aa0): typed by tools/typed_lift2.py (round A2; leaf)
inline uint64_t getEventScriptClass(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint64_t a0)
{
    uint64_t v0 = 0;
    uint64_t v1 = 0;
    (v0 = (uint64_t)((uint64_t)a0 + (uint64_t)0ULL));
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 32))));
    (v1 = (uint64_t)(uint32_t)((uint8_t)A1_R8B((uint32_t)v0, 8)));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v0, 32))));
    (v1 = (uint64_t)(RC_U64X(v1) ^ RC_U64X(a0)));
    { if (RC_U64X(v1) != 0) v0 = (uint64_t)0;; return v0; }
    __builtin_unreachable();
}

// iosInitDC (0x1bf400): typed by tools/typed_lift2.py (round A2; frame calls, frame 16 bytes, 0 slot accesses)
inline void iosInitDC(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt)
{
    uint32_t a0 = 0;
    a0 = 0x210000u;
    a0 = a0 + 0x32C8u;
    { (void)isysInitGroup(ram, rt, a0); return; }
    __builtin_unreachable();
}

// InitSubThread (0x1c9828): typed by tools/typed_lift2.py (round A2; frame calls, frame 16 bytes, 0 slot accesses)
inline void InitSubThread(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt)
{
    uint32_t a0 = 0;
    a0 = 0x1E0000u;
    a0 = a0 + 0x1558u;
    { (void)isysInitGroup(ram, rt, a0); return; }
    __builtin_unreachable();
}

// setSoundMode (0x1cb430): typed by tools/typed_lift2.py (round A2; frame calls, frame 16 bytes, 0 slot accesses)
inline uint32_t setSoundMode(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint64_t a0)
{
    uint32_t v0 = 0;
    uint64_t v1 = 0;
    uint32_t a1 = 0;
    v0 = 2u;
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 1))));
    if (a0 == 0) v1 = (uint64_t)(a1_sx64(v0));;
    a1 = 0x1E0000u;
    (a0 = (uint64_t)((uint64_t)v1 + (uint64_t)0ULL));
    A1_W32B(a1, 5472, (uint32_t)v1);
    { return Sg2SlotSetOutputMode(ram, rt, (uint32_t)a0); }
    __builtin_unreachable();
}

// Sg2DmaTransStatus (0x1cc300): typed by tools/typed_lift2.py (round A2; frame calls, frame 16 bytes, 0 slot accesses)
inline uint32_t Sg2DmaTransStatus(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t a0)
{
    { return Sg2RemoteDmaTransStatus(ram, rt, a0); }
    __builtin_unreachable();
}

// Sg2VabBdOpen (0x1da530): typed by tools/typed_lift2.py (round A2; frame calls, frame 32 bytes, 0 slot accesses)
inline uint64_t Sg2VabBdOpen(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint64_t a0, uint64_t a1)
{
    uint64_t v0 = 0;
    uint64_t v1 = 0;
    uint32_t s0 = 0;
    uint64_t s1 = 0;
    (v1 = (uint64_t)((uint64_t)a0 + (uint64_t)0ULL));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)SLL32((uint32_t)v1, 1))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v0, (uint32_t)v1))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)SLL32((uint32_t)v0, 2))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)SUB32((uint32_t)v0, (uint32_t)v1))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)SLL32((uint32_t)v0, 2))));
    s0 = 0x290000u;
    s0 = s0 + 0x4C0u;
    (s0 = (uint32_t)((int32_t)((int32_t)ADD32(s0, (uint32_t)v0))));
    { (s1 = (uint64_t)((uint64_t)a1 + (uint64_t)0ULL)); A2_CHARGE(); v0 = vab_check_alive(ram, rt, a0); }
    if ((int64_t)v0 != 0) { A1_W32B(s0, 12, (uint32_t)s1); goto L_1da580; }
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 4294967295))));
    goto L_1da584;
L_1da580:;
    (v0 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL));
L_1da584:;
    { return v0; }
    __builtin_unreachable();
}

// GAMEUTIL_get_difference_value (0x1080220): typed by tools/typed_lift2.py (round A2; frame calls, frame 16 bytes, 0 slot accesses)
inline a1::RetVF GAMEUTIL_get_difference_value(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint64_t a0, float f12, float f13)
{
    float f0 = 0.0f;
    float f1 = 0.0f;
    uint32_t v0 = 0;
    v0 = 1u;
    f1 = ((float)(f12));
    { const bool c = (int64_t)a0 != (int64_t)(int32_t)v0; f0 = ((float)(f13)); if (c) goto L_1080248; }
    { return GAMEUTIL_get_move_radi_from_yaw1_to_yaw2(ram, rt, f12, f13); }
L_1080248:;
    f0 = a1f::sub(f0, f1);
    { return a1::RetVF{(uint64_t)a1_sx64(v0), f0}; }
    __builtin_unreachable();
}

// GetCalcNumObjWorkSize (0x10c1b90): typed by tools/typed_lift2.py (round A2; frame calls, frame 16 bytes, 0 slot accesses)
inline uint32_t GetCalcNumObjWorkSize(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint64_t a0, uint64_t a1, uint64_t a2, uint64_t a3, uint64_t t0)
{
    uint32_t v0 = 0;
    uint32_t v1 = 0;
    uint32_t t1 = 0;
    uint32_t s0 = 0;
    uint64_t hi = 0;
    uint64_t lo = 0;
    (t1 = (uint32_t)((int32_t)((int32_t)SLL32((uint32_t)a0, 4))));
    (v0 = (uint32_t)((int32_t)((int32_t)SLL32((uint32_t)a0, 5))));
    t1 = t1 + 0x80u;
    (v0 = (uint32_t)((int32_t)((int32_t)ADD32(v0, (uint32_t)a0))));
    { int64_t result = (int64_t)(int32_t)t1 * (int64_t)(int32_t)a2; lo = (uint64_t)(int64_t)(int32_t)result; hi = (uint64_t)(int64_t)(int32_t)(result >> 32); (t1 = (uint32_t)((int32_t)((int32_t)result))); }
    v0 = v0 << 3;
    (v1 = (uint32_t)((int32_t)((int32_t)SLL32((uint32_t)a2, 3))));
    s0 = v0 + 0x33u;
    v1 = s0 + v1;
    (a0 = (uint64_t)((uint64_t)a3 + (uint64_t)0ULL));
    { const bool c = (int64_t)a2 == 0; v1 = v1 + t1; if (c) goto L_10c1bd0; }
    s0 = v1 + 0x66u;
L_10c1bd0:;
    if ((int64_t)a1 == 0) goto L_10c1c14;
    { const bool c = (int64_t)t0 == 0; (v1 = (uint32_t)((int32_t)((int32_t)SLL32((uint32_t)a1, 1)))); if (c) goto L_10c1bf8; }
    (v1 = (uint32_t)((int32_t)((int32_t)ADD32(v1, (uint32_t)a1))));
    v0 = v1 << 6;
    v0 = v0 - v1;
    v0 = v0 << 1;
    goto L_10c1c0c;
L_10c1bf8:;
    (v0 = (uint32_t)((int32_t)((int32_t)SLL32((uint32_t)a1, 3))));
    (v0 = (uint32_t)((int32_t)((int32_t)ADD32(v0, (uint32_t)a1))));
    v1 = v0 << 5;
    v0 = v0 + v1;
    (v0 = (uint32_t)((int32_t)((int32_t)ADD32(v0, (uint32_t)a1))));
L_10c1c0c:;
    v0 = s0 + v0;
    s0 = v0 + 0x33u;
L_10c1c14:;
    { s0 = s0 + 0x333u; A2_CHARGE(); v0 = GetCollisionFinderWorkSize(ram, rt, (uint32_t)a0); }
    v0 = s0 + v0;
    { return v0; }
    __builtin_unreachable();
}

// GAMEUTIL_constructor_move_angle (0x1343c58): typed by tools/typed_lift2.py (round A2; frame calls, frame 16 bytes, 0 slot accesses)
inline float GAMEUTIL_constructor_move_angle(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint64_t a0)
{
    float f0 = 0.0f;
    float f1 = 0.0f;
    float f2 = 0.0f;
    float f3 = 0.0f;
    float f4 = 0.0f;
    uint32_t at = 0;
    uint64_t s0 = 0;
    { (s0 = (uint64_t)((uint64_t)a0 + (uint64_t)0ULL)); A2_CHARGE(); GAMEUTIL_reset_move_angle(ram, rt, (uint32_t)a0); }
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
    { float f = f1; uint32_t bits; std::memcpy(&bits, &f, sizeof(bits)); A1_W32B((uint32_t)s0, 8, bits); }
    { float f = f0; uint32_t bits; std::memcpy(&bits, &f, sizeof(bits)); A1_W32B((uint32_t)s0, 24, bits); }
    { float f = f4; uint32_t bits; std::memcpy(&bits, &f, sizeof(bits)); A1_W32B((uint32_t)s0, 12, bits); }
    { float f = f2; uint32_t bits; std::memcpy(&bits, &f, sizeof(bits)); A1_W32B((uint32_t)s0, 16, bits); }
    { float f = f3; uint32_t bits; std::memcpy(&bits, &f, sizeof(bits)); A1_W32B((uint32_t)s0, 20, bits); }
    { float f = f4; uint32_t bits; std::memcpy(&bits, &f, sizeof(bits)); A1_W32B((uint32_t)s0, 4, bits); }
    { return f0; }
    __builtin_unreachable();
}

// setFade (0x13461d0): typed by tools/typed_lift2.py (round A2; frame calls, frame 16 bytes, 0 slot accesses)
inline a1::RetVF setFade(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, float f12, float f13)
{
    uint32_t a0 = 0;
    uint32_t a1 = 0;
    a0 = 0x1460000u;
    a1 = 0x1Bu;
    a0 = a0 + 0x5A40u;
    { return setFadeParam(ram, rt, a0, a1, f12, f13); }
    __builtin_unreachable();
}

// getFade (0x13461f0): typed by tools/typed_lift2.py (round A2; frame calls, frame 16 bytes, 0 slot accesses)
inline a1::RetVF getFade(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt)
{
    uint32_t a0 = 0;
    a0 = 0x1460000u;
    a0 = a0 + 0x5A40u;
    { return checkFadeParam(ram, rt, a0); }
    __builtin_unreachable();
}

// gamecore_sub_013527D8 (0x13527d8): typed by tools/typed_lift2.py (round A2; frame calls, frame 16 bytes, 0 slot accesses)
inline uint32_t gamecore_sub_013527D8(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t a0)
{
    float f12 = 0.0f;
    uint32_t at = 0;
    uint64_t a1 = 0;
    at = 0x3FC00000u;
    f12 = a1_flt(at);
    (a1 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL));
    { return bgmStop(ram, rt, a0, a1, f12); }
    __builtin_unreachable();
}

// gamecore_sub_01357758 (0x1357758): typed by tools/typed_lift2.py (round A2; frame calls, frame 16 bytes, 0 slot accesses)
inline uint32_t gamecore_sub_01357758(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt)
{
    float f12 = 0.0f;
    uint32_t at = 0;
    uint32_t v0 = 0;
    uint32_t a0 = 0;
    uint64_t a1 = 0;
    v0 = 0x1460000u;
    (a1 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL));
    at = 0x42C80000u;
    f12 = a1_flt(at);
    a0 = A1_R32B(v0, 24100);
    { return bgmStop(ram, rt, a0, a1, f12); }
    __builtin_unreachable();
}

// gamecore_sub_01384EE0 (0x1384ee0): typed by tools/typed_lift2.py (round A2; frame calls, frame 16 bytes, 0 slot accesses)
inline uint32_t gamecore_sub_01384EE0(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt)
{
    uint32_t a0 = 0;
    a0 = 1u;
    { return GAMEUTIL_set_auto_get_flag_boss_interest_position(ram, rt, a0); }
    __builtin_unreachable();
}

// get_pre_type_expression (0x138be98): typed by tools/typed_lift2.py (round A2; frame calls, frame 16 bytes, 0 slot accesses)
inline uint32_t get_pre_type_expression(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t a0)
{
    a0 = a0 + 4u;
    { return GAMEUTIL_get_pre_changed_status_status_manager(ram, rt, a0); }
    __builtin_unreachable();
}

// MotSpdCtrl_Init (0x139aaf8): typed by tools/typed_lift2.py (round A2; frame calls, frame 32 bytes, 0 slot accesses)
inline void MotSpdCtrl_Init(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint64_t a0, uint64_t a1)
{
    uint64_t s0 = 0;
    uint64_t s1 = 0;
    (s0 = (uint64_t)((uint64_t)a0 + (uint64_t)0ULL));
    { (s1 = (uint64_t)((uint64_t)a1 + (uint64_t)0ULL)); A2_CHARGE(); MotSpdCtrl_Reset(ram, rt, (uint32_t)a0); }
    A1_W32B((uint32_t)s0, 20, (uint32_t)s1);
    { return; }
    __builtin_unreachable();
}

// gamecore_sub_013AE538 (0x13ae538): typed by tools/typed_lift2.py (round A2; frame calls, frame 16 bytes, 0 slot accesses)
inline uint32_t gamecore_sub_013AE538(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt)
{
    uint32_t a0 = 0;
    a0 = 1u;
    { return _optionWorkChk(ram, rt, a0); }
    __builtin_unreachable();
}

// gamecore_sub_013AE550 (0x13ae550): typed by tools/typed_lift2.py (round A2; frame calls, frame 16 bytes, 0 slot accesses)
inline uint32_t gamecore_sub_013AE550(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt)
{
    uint32_t a0 = 0;
    a0 = 4u;
    { return _optionWorkChk(ram, rt, a0); }
    __builtin_unreachable();
}

// gamecore_sub_013AE568 (0x13ae568): typed by tools/typed_lift2.py (round A2; frame calls, frame 16 bytes, 0 slot accesses)
inline uint32_t gamecore_sub_013AE568(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt)
{
    uint32_t a0 = 0;
    a0 = 5u;
    { return _optionWorkChk(ram, rt, a0); }
    __builtin_unreachable();
}

// gamecore_sub_013AE580 (0x13ae580): typed by tools/typed_lift2.py (round A2; frame calls, frame 16 bytes, 0 slot accesses)
inline uint32_t gamecore_sub_013AE580(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt)
{
    uint32_t a0 = 0;
    a0 = 2u;
    { return _optionWorkChk(ram, rt, a0); }
    __builtin_unreachable();
}

// gamecore_sub_013AE598 (0x13ae598): typed by tools/typed_lift2.py (round A2; frame calls, frame 16 bytes, 0 slot accesses)
inline uint32_t gamecore_sub_013AE598(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt)
{
    uint32_t a0 = 0;
    a0 = 6u;
    { return _optionWorkChk(ram, rt, a0); }
    __builtin_unreachable();
}

// gamecore_sub_013AE5B0 (0x13ae5b0): typed by tools/typed_lift2.py (round A2; frame calls, frame 16 bytes, 0 slot accesses)
inline uint32_t gamecore_sub_013AE5B0(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt)
{
    uint32_t a0 = 0;
    a0 = 0x11u;
    { return _optionWorkChk(ram, rt, a0); }
    __builtin_unreachable();
}

// gamecore_sub_013AE5C8 (0x13ae5c8): typed by tools/typed_lift2.py (round A2; frame calls, frame 16 bytes, 0 slot accesses)
inline uint32_t gamecore_sub_013AE5C8(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt)
{
    uint32_t a0 = 0;
    a0 = 0x16u;
    { return _optionWorkChk(ram, rt, a0); }
    __builtin_unreachable();
}

// gamecore_sub_013AE5E0 (0x13ae5e0): typed by tools/typed_lift2.py (round A2; frame calls, frame 16 bytes, 0 slot accesses)
inline uint32_t gamecore_sub_013AE5E0(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt)
{
    uint32_t a0 = 0;
    a0 = 0x15u;
    { return _optionWorkChk(ram, rt, a0); }
    __builtin_unreachable();
}

// gamecore_sub_013AE5F8 (0x13ae5f8): typed by tools/typed_lift2.py (round A2; frame calls, frame 16 bytes, 0 slot accesses)
inline uint32_t gamecore_sub_013AE5F8(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt)
{
    uint32_t a0 = 0;
    a0 = 0x13u;
    { return _optionWorkChk(ram, rt, a0); }
    __builtin_unreachable();
}

// gamecore_sub_013AE610 (0x13ae610): typed by tools/typed_lift2.py (round A2; frame calls, frame 16 bytes, 0 slot accesses)
inline uint32_t gamecore_sub_013AE610(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt)
{
    uint32_t a0 = 0;
    a0 = 0x12u;
    { return _optionWorkChk(ram, rt, a0); }
    __builtin_unreachable();
}

// gamecore_sub_013AE628 (0x13ae628): typed by tools/typed_lift2.py (round A2; frame calls, frame 16 bytes, 0 slot accesses)
inline uint32_t gamecore_sub_013AE628(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt)
{
    uint32_t a0 = 0;
    a0 = 0x14u;
    { return _optionWorkChk(ram, rt, a0); }
    __builtin_unreachable();
}

// gamecore_sub_013AE880 (0x13ae880): typed by tools/typed_lift2.py (round A2; frame calls, frame 16 bytes, 0 slot accesses)
inline uint32_t gamecore_sub_013AE880(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt)
{
    uint32_t a0 = 0;
    a0 = 3u;
    { return _optionWorkChk(ram, rt, a0); }
    __builtin_unreachable();
}

// gamecore_sub_013B77C8 (0x13b77c8): typed by tools/typed_lift2.py (round A2; frame calls, frame 16 bytes, 0 slot accesses)
inline a1::RetVF gamecore_sub_013B77C8(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt)
{
    { return setFullPower(ram, rt); }
    __builtin_unreachable();
}

// get_cur_attr_camera_default_angle (0x14109f8): typed by tools/typed_lift2.py (round A2; frame calls, frame 16 bytes, 0 slot accesses)
inline uint32_t get_cur_attr_camera_default_angle(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t a0)
{
    { return GAMEUTIL_get_cur_status_status_manager(ram, rt, a0); }
    __builtin_unreachable();
}

// gamecore_sub_01410A90 (0x1410a90): typed by tools/typed_lift2.py (round A2; frame calls, frame 16 bytes, 0 slot accesses)
inline void gamecore_sub_01410A90(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t a0)
{
    uint32_t v0 = 0;
    uint32_t a1 = 0;
    v0 = 1u;
    a1 = 1u;
    A1_W32B(a0, 888, v0);
    a0 = a0 + 0x37Cu;
    { (void)GAMEUTIL_reset_status_manager(ram, rt, a0, a1); return; }
    __builtin_unreachable();
}

// get_status_camera_target_approach (0x1414a48): typed by tools/typed_lift2.py (round A2; frame calls, frame 16 bytes, 0 slot accesses)
inline uint32_t get_status_camera_target_approach(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t a0)
{
    { return GAMEUTIL_get_cur_status_status_manager(ram, rt, a0); }
    __builtin_unreachable();
}

// CAM_init_camera_warp_cntrl (0x141aa98): typed by tools/typed_lift2.py (round A2; frame calls, frame 16 bytes, 0 slot accesses)
inline a1::RetVF CAM_init_camera_warp_cntrl(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint64_t a0, uint32_t a1, uint32_t a2, uint32_t a3)
{
    float f0 = 0.0f;
    uint32_t at = 0;
    uint64_t v0 = 0;
    at = 0x3DB20000u;
    at = at | 0xB8C2u;
    { uint32_t bits = at; std::memcpy(&f0, &bits, sizeof(bits)); }
    (v0 = (uint64_t)((uint64_t)a0 + (uint64_t)0ULL));
    A1_W32B((uint32_t)v0, 0, a1);
    { float f = f0; uint32_t bits; std::memcpy(&bits, &f, sizeof(bits)); A1_W32B((uint32_t)v0, 24, bits); }
    A1_W32B((uint32_t)v0, 12, a2);
    A1_W32B((uint32_t)v0, 28, a3);
    { float f = f0; uint32_t bits; std::memcpy(&bits, &f, sizeof(bits)); A1_W32B((uint32_t)v0, 20, bits); }
    { return CAM_reset_camera_warp_cntrl(ram, rt, (uint32_t)a0); }
    __builtin_unreachable();
}

// gamecore_sub_0141C9B8 (0x141c9b8): typed by tools/typed_lift2.py (round A2; frame calls, frame 16 bytes, 0 slot accesses)
inline a1::RetVF gamecore_sub_0141C9B8(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint64_t a0, uint32_t a1, uint64_t a2)
{
    float f0 = 0.0f;
    uint32_t v0 = 0;
    v0 = 1u;
    if ((int64_t)a0 != (int64_t)(int32_t)v0) goto L_141c9d8;
    (a0 = (uint64_t)((uint64_t)a2 + (uint64_t)0ULL));
    { return get_view_angle_lws_anime_mode_base(ram, rt, (uint32_t)a0); }
L_141c9d8:;
    f0 = a1_flt(A1_R32B(a1, 76));
    { return a1::RetVF{(uint64_t)a1_sx64(v0), f0}; }
    __builtin_unreachable();
}

// get_main_cameraman_camera_obj (0x141d390): typed by tools/typed_lift2.py (round A2; frame calls, frame 16 bytes, 0 slot accesses)
inline uint32_t get_main_cameraman_camera_obj(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt)
{
    uint32_t v0 = 0;
    uint32_t v1 = 0;
    uint32_t a0 = 0;
    v1 = 0x1200000u;
    v1 = v1 - 0x4A00u;
    v0 = A1_R32(v1);
    v0 = v0 << 2;
    v1 = v1 + v0;
    a0 = A1_R32B(v1, 8);
    { return get_camera_obj(ram, rt, a0); }
    __builtin_unreachable();
}

// request_end_pause_rise_game_camera (0x1424fe8): typed by tools/typed_lift2.py (round A2; frame calls, frame 16 bytes, 0 slot accesses)
inline uint32_t request_end_pause_rise_game_camera(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint64_t a0, uint64_t a1)
{
    uint64_t a2 = 0;
    (a2 = (uint64_t)((uint64_t)a1 + (uint64_t)0ULL));
    (a1 = (uint64_t)((uint64_t)a0 + (uint64_t)0ULL));
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)a0, 14160))));
    { return request_end_camera_pause_rise(ram, rt, (uint32_t)a0); }
    __builtin_unreachable();
}

// is_end_pause_rise_game_camera (0x1425008): typed by tools/typed_lift2.py (round A2; frame calls, frame 16 bytes, 0 slot accesses)
inline uint32_t is_end_pause_rise_game_camera(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t a0)
{
    a0 = a0 + 0x3750u;
    { return is_end_camera_pause_rise(ram, rt, a0); }
    __builtin_unreachable();
}

// getPathColiGroup (0x1431c58): typed by tools/typed_lift2.py (round A2; frame calls, frame 16 bytes, 0 slot accesses)
inline uint32_t getPathColiGroup(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t a0)
{
    a0 = A1_R32B(a0, 15440);
    { return getPathColiGroupByType(ram, rt, a1_sx64(a0)); }
    __builtin_unreachable();
}

// _resetBaseMoveWork (0x1437a88): typed by tools/typed_lift2.py (round A2; frame calls, frame 16 bytes, 0 slot accesses)
inline a1::RetVF _resetBaseMoveWork(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint64_t a1)
{
    uint64_t a0 = 0;
    (a0 = (uint64_t)((uint64_t)a1 + (uint64_t)0ULL));
    A1_W32B((uint32_t)a1, 4, 0u);
    A1_W32B((uint32_t)a1, 36, 0u);
    { return gamecore_sub_01438448(ram, rt, (uint32_t)a0); }
    __builtin_unreachable();
}

// SCRConditionSwordFlareHitColAttr (0x1459360): typed by tools/typed_lift2.py (round A2; frame calls, frame 32 bytes, 0 slot accesses)
inline uint64_t SCRConditionSwordFlareHitColAttr(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint64_t t0, uint64_t t1)
{
    uint64_t v0 = 0;
    uint64_t s0 = 0;
    uint64_t s1 = 0;
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 4294967295))));
    (s1 = (uint64_t)((uint64_t)t1 + (uint64_t)0ULL));
    (s0 = (uint64_t)((uint64_t)t0 + (uint64_t)0ULL));
    if ((int64_t)s1 == (int64_t)v0) goto L_1459398;
    { A2_CHARGE(); v0 = GetSwordFlareHitPlaneAttribute(ram, rt); }
    (v0 = (uint64_t)(v0 & s0));
    (v0 = (uint64_t)(s1 ^ v0));
    (v0 = (uint64_t)(((uint64_t)v0 < (uint64_t)(int64_t)(int32_t)1) ? 1 : 0));
    goto L_14593a8;
L_1459398:;
    { A2_CHARGE(); v0 = GetSwordFlareHitPlaneAttribute(ram, rt); }
    (v0 = (uint64_t)(v0 & s0));
    (v0 = (uint64_t)(((uint64_t)0ULL < (uint64_t)v0) ? 1 : 0));
L_14593a8:;
    { return v0; }
    __builtin_unreachable();
}

// SCRConditionHardMode (0x1459ae8): typed by tools/typed_lift2.py (round A2; frame calls, frame 16 bytes, 0 slot accesses)
inline uint32_t SCRConditionHardMode(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt)
{
    { return getGameHardMode(ram, rt); }
    __builtin_unreachable();
}

// gamecore_sub_0145B1C0 (0x145b1c0): typed by tools/typed_lift2.py (round A2; frame calls, frame 16 bytes, 0 slot accesses)
inline uint32_t gamecore_sub_0145B1C0(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint64_t a1, uint64_t t0)
{
    float f12 = 0.0f;
    uint32_t at = 0;
    uint32_t v0 = 0;
    uint64_t a0 = 0;
    uint64_t s0 = 0;
    (s0 = (uint64_t)((uint64_t)t0 + (uint64_t)0ULL));
    (v0 = (uint32_t)((int32_t)((int32_t)A1_R32B((uint32_t)s0, 24))));
    { const bool c = v0 == 0; (a0 = (uint64_t)((uint64_t)a1_sx64(v0) + (uint64_t)0ULL)); if (c) goto L_145b1f8; }
    (at = (uint32_t)((int32_t)((int32_t)((uint32_t)17096 << 16))));
    { uint32_t bits = at; std::memcpy(&f12, &bits, sizeof(bits)); }
    { (a1 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL)); A2_CHARGE(); v0 = bgmStop(ram, rt, (uint32_t)a0, a1, f12); }
    v0 = 1u;
    A1_W32B((uint32_t)s0, 28, v0);
    A1_W32B((uint32_t)s0, 24, 0u);
L_145b1f8:;
    { return v0; }
    __builtin_unreachable();
}

// is_reach_src_value_double_spring_interp_cntrl (0x10845b0): typed by tools/typed_lift2.py (round A2; frame calls, frame 16 bytes, 0 slot accesses)
inline a1::RetVF is_reach_src_value_double_spring_interp_cntrl(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint64_t a0, float f0, float f12)
{
    float f1 = 0.0f;
    float f13 = 0.0f;
    uint32_t at = 0;
    uint32_t v0 = 0;
    uint64_t s0 = 0;
    f13 = ((float)(f12));
    (s0 = (uint64_t)((uint64_t)a0 + (uint64_t)0ULL));
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)s0, 20))));
    { { uint32_t bits = A1_R32B((uint32_t)s0, 28); float f; std::memcpy(&f, &bits, sizeof(f)); f12 = f; } A2_CHARGE(); { const a1::RetVF a2_r = GAMEUTIL_get_difference_value(ram, rt, a0, f12, f13); v0 = (uint32_t)a2_r.v0; f0 = a2_r.f0; } }
    { uint32_t bits = A1_R32B((uint32_t)s0, 16); float f; std::memcpy(&f, &bits, sizeof(f)); f1 = f; }
    at = 0x40400000u;
    { uint32_t bits = at; std::memcpy(&f13, &bits, sizeof(bits)); }
    f12 = ((float)(f0));
    f13 = a1f::mul(f1, f13);
    { return GAMEUTIL_is_nearly_equal_zero(ram, rt, f12, f13); }
    __builtin_unreachable();
}

// gamecore_sub_014269D0 (0x14269d0): typed by tools/typed_lift2.py (round A2; frame calls, frame 16 bytes, 0 slot accesses)
inline a1::RetVF gamecore_sub_014269D0(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint64_t a0)
{
    uint64_t v0 = 0;
    uint64_t a1 = 0;
    uint64_t a2 = 0;
    uint64_t a3 = 0;
    (v0 = (uint64_t)((uint64_t)a0 + (uint64_t)0ULL));
    (a1 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL));
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v0, 6208))));
    (a2 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL));
    (a3 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL));
    A1_W32B((uint32_t)v0, 6192, 0u);
    A1_W32B((uint32_t)v0, 6196, 0u);
    A1_W32B((uint32_t)v0, 6200, 0u);
    { return CAM_init_camera_warp_cntrl(ram, rt, a0, (uint32_t)a1, (uint32_t)a2, (uint32_t)a3); }
    __builtin_unreachable();
}
