// typed_a3.inc: round A3's typed functions (tools/typed_lift2.py, stage 3). Generated: do not edit. Included inside a namespace (typed_a3.h: namespace typed), after typed_a2.inc.
// RelocateElfInfoHeader (0x1004b8): typed by tools/typed_lift2.py (round A3; loop leaf, extra outputs xo[0]=a3 xo[1]=t0 xo[2]=t1 xo[3]=t2)
inline uint32_t RelocateElfInfoHeader(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint64_t a0, uint64_t *xo = nullptr)
{
    uint32_t v0 = 0;
    uint64_t v1 = 0;
    uint64_t a1 = 0;
    uint32_t a2 = 0;
    uint32_t a3 = 0;
    uint32_t t0 = 0;
    uint64_t t1 = 0;
    uint32_t t2 = 0;
    (t1 = (uint64_t)((uint64_t)a0 + (uint64_t)0ULL));
    (v0 = (uint32_t)((int32_t)((int32_t)A1_R32B((uint32_t)t1, 92))));
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)t1, 64))));
    (v0 = (uint32_t)((int32_t)((int32_t)ADD32((uint32_t)t1, v0))));
    { const bool c = (int32_t)a0 <= 0; A1_W32B((uint32_t)t1, 48, v0); if (c) goto L_1004f4; }
    (v1 = (uint64_t)((uint64_t)a1_sx64(v0) + (uint64_t)0ULL));
    (a1 = (uint64_t)((uint64_t)RC_U64X(a0) + (uint64_t)0ULL));
L_1004d8:;
    (v0 = (uint32_t)((int32_t)((int32_t)A1_R32B((uint32_t)v1, 28))));
    (a1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)a1, 4294967295))));
    (v0 = (uint32_t)((int32_t)((int32_t)ADD32((uint32_t)t1, v0))));
    A1_W32B((uint32_t)v1, 4, v0);
    { const bool c = (int64_t)a1 != 0; (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v1, 32)))); if (c) { A3_BACK(); goto L_1004d8; } }
L_1004f4:;
    (a1 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)t1, 84))));
    (a2 = (uint32_t)((int32_t)((int32_t)A1_R32B((uint32_t)t1, 96))));
    (a3 = (uint32_t)((int32_t)((int32_t)A1_R32B((uint32_t)t1, 88))));
    (a1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)t1, (uint32_t)a1))));
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)t1, 80))));
    (a2 = (uint32_t)((int32_t)((int32_t)ADD32((uint32_t)t1, a2))));
    (t0 = (uint32_t)((int32_t)((int32_t)A1_R32B((uint32_t)t1, 100))));
    (a3 = (uint32_t)((int32_t)((int32_t)ADD32((uint32_t)t1, a3))));
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)t1, 108))));
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)t1, (uint32_t)a0))));
    (v0 = (uint32_t)((int32_t)((int32_t)A1_R32B((uint32_t)t1, 104))));
    (t0 = (uint32_t)((int32_t)((int32_t)ADD32((uint32_t)t1, t0))));
    (t2 = (uint32_t)((int32_t)((int32_t)A1_R32B((uint32_t)t1, 56))));
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)t1, (uint32_t)v1))));
    (v0 = (uint32_t)((int32_t)((int32_t)ADD32((uint32_t)t1, v0))));
    A1_W32B((uint32_t)t1, 40, (uint32_t)a1);
    A1_W32B((uint32_t)t1, 52, a2);
    A1_W32B((uint32_t)t1, 44, a3);
    A1_W32B((uint32_t)t1, 32, (uint32_t)a0);
    A1_W32B((uint32_t)t1, 72, (uint32_t)v1);
    A1_W32B((uint32_t)t1, 68, v0);
    { const bool c = (int32_t)t2 <= 0; A1_W32B((uint32_t)t1, 60, t0); if (c) goto L_10057c; }
    (a0 = (uint64_t)((uint64_t)a1_sx64(t0) + (uint64_t)0ULL));
    (a1 = (uint64_t)((uint64_t)a1_sx64(t2) + (uint64_t)0ULL));
L_100558:;
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)a0, 20))));
    (a1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)a1, 4294967295))));
    (v0 = (uint32_t)((int32_t)((int32_t)A1_R32B((uint32_t)a0, 24))));
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)t1, (uint32_t)v1))));
    (v0 = (uint32_t)((int32_t)((int32_t)ADD32((uint32_t)t1, v0))));
    A1_W32B((uint32_t)a0, 12, (uint32_t)v1);
    A1_W32B((uint32_t)a0, 16, v0);
    { const bool c = (int64_t)a1 != 0; (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)a0, 28)))); if (c) { A3_BACK(); goto L_100558; } }
L_10057c:;
    (v0 = (uint32_t)((int32_t)((int32_t)A1_R32B((uint32_t)t1, 20))));
    (v0 = (uint32_t)((int32_t)((int32_t)ADD32((uint32_t)t1, v0))));
    { A1_W32B((uint32_t)t1, 24, v0); if (xo) { xo[0] = a1_sx64(a3); xo[1] = a1_sx64(t0); xo[2] = t1; xo[3] = a1_sx64(t2); } return v0; }
    __builtin_unreachable();
}

// LoaderSysEntryExternalIntcHandlerList (0x102730): typed by tools/typed_lift2.py (round A3; loop leaf, extra outputs xo[0]=a2 xo[1]=a3 xo[2]=t0 xo[3]=t1)
inline uint32_t LoaderSysEntryExternalIntcHandlerList(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint64_t a0, uint32_t a1, uint64_t *xo = nullptr)
{
    uint32_t v0 = 0;
    uint64_t v1 = 0;
    uint64_t a2 = 0;
    uint32_t a3 = 0;
    uint64_t t0 = 0;
    uint32_t t1 = 0;
    t1 = 0x140000u;
    (t0 = (uint64_t)((uint64_t)a0 + (uint64_t)0ULL));
    v0 = t1 - 0x4070u;
    (a2 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL));
    a3 = v0 + 4u;
    v0 = A1_R32(a3);
L_102748:;
    a3 = a3 + 8u;
    (v1 = (uint64_t)((uint64_t)a2 + (uint64_t)0ULL));
    (a2 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)a2, 1))));
    { const bool c = (int32_t)v0 < 0; (a0 = (uint64_t)(((int64_t)(int64_t)RC_U64X(a2) < (int64_t)(int32_t)256) ? 1 : 0)); if (c) goto L_102770; }
    if ((int64_t)a0 != 0) { v0 = A1_R32(a3); { A3_BACK(); goto L_102748; } }
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 4294967295))));
L_102770:;
    v0 = t1 - 0x4070u;
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)SLL32((uint32_t)v1, 3))));
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v1, v0))));
    (v0 = (uint32_t)((int32_t)((int32_t)ADD32(v0, (uint32_t)v1))));
    A1_W32B(v0, 4, (uint32_t)t0);
    { A1_W32B((uint32_t)a0, 0, a1); if (xo) { xo[0] = a2; xo[1] = a1_sx64(a3); xo[2] = t0; xo[3] = a1_sx64(t1); } return v0; }
    __builtin_unreachable();
}

// LoaderSysEntryExternalThreadList (0x102790): typed by tools/typed_lift2.py (round A3; loop leaf, extra outputs xo[0]=a1 xo[1]=a2 xo[2]=a3 xo[3]=t0)
inline uint32_t LoaderSysEntryExternalThreadList(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t a0, uint64_t *xo = nullptr)
{
    uint32_t v0 = 0;
    uint32_t v1 = 0;
    uint32_t a1 = 0;
    uint64_t a2 = 0;
    uint64_t a3 = 0;
    uint32_t t0 = 0;
    v1 = 0x140000u;
    (a2 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL));
    t0 = v1 - 0x4870u;
    v0 = A1_R32(t0);
L_1027a0:;
    t0 = t0 + 4u;
    (a3 = (uint64_t)((uint64_t)a2 + (uint64_t)0ULL));
    (a2 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)a2, 1))));
    { const bool c = (int32_t)v0 < 0; (a1 = (uint32_t)(((int64_t)(int64_t)RC_U64X(a2) < (int64_t)(int32_t)256) ? 1 : 0)); if (c) goto L_1027c8; }
    if (a1 != 0) { v0 = A1_R32(t0); { A3_BACK(); goto L_1027a0; } }
    (a3 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 4294967295))));
L_1027c8:;
    v1 = v1 - 0x4870u;
    (v0 = (uint32_t)((int32_t)((int32_t)SLL32((uint32_t)a3, 2))));
    v0 = v0 + v1;
    { A1_W32(v0, a0); if (xo) { xo[0] = a1_sx64(a1); xo[1] = a2; xo[2] = a3; xo[3] = a1_sx64(t0); } return v0; }
    __builtin_unreachable();
}

// LoaderSysEntryExternalSemaList (0x1027e0): typed by tools/typed_lift2.py (round A3; loop leaf)
inline uint32_t LoaderSysEntryExternalSemaList(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t a0)
{
    uint32_t v0 = 0;
    uint32_t v1 = 0;
    uint32_t a1 = 0;
    uint64_t a2 = 0;
    uint64_t a3 = 0;
    uint32_t t0 = 0;
    v1 = 0x140000u;
    (a2 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL));
    t0 = v1 - 0x4470u;
    v0 = A1_R32(t0);
L_1027f0:;
    t0 = t0 + 4u;
    (a3 = (uint64_t)((uint64_t)a2 + (uint64_t)0ULL));
    (a2 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)a2, 1))));
    { const bool c = (int32_t)v0 < 0; (a1 = (uint32_t)(((int64_t)(int64_t)RC_U64X(a2) < (int64_t)(int32_t)256) ? 1 : 0)); if (c) goto L_102818; }
    if (a1 != 0) { v0 = A1_R32(t0); { A3_BACK(); goto L_1027f0; } }
    (a3 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 4294967295))));
L_102818:;
    v1 = v1 - 0x4470u;
    (v0 = (uint32_t)((int32_t)((int32_t)SLL32((uint32_t)a3, 2))));
    v0 = v0 + v1;
    { A1_W32(v0, a0); return v0; }
    __builtin_unreachable();
}

// LoaderSysEntryExternalIopMemoryList (0x102830): typed by tools/typed_lift2.py (round A3; loop leaf, extra outputs xo[0]=a1 xo[1]=a2)
inline uint64_t LoaderSysEntryExternalIopMemoryList(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint64_t a0, uint64_t *xo = nullptr)
{
    uint64_t v0 = 0;
    uint64_t v1 = 0;
    uint64_t a1 = 0;
    uint32_t a2 = 0;
    bool a3xw0 = false;
    { const bool c = (int64_t)a0 == 0; a2 = 0x140000u; if (c) goto L_102898; }
    (v1 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL));
    (a1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(a2, 4294952848)))); a3xw0 = true;
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)a1, 0))));
L_102848:;
    { const bool c = (int64_t)v0 == 0; (a1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)a1, 4)))); a3xw0 = true; if (c) goto L_102890; }
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v1, 1))));
    (v0 = (uint64_t)(((int64_t)(int64_t)RC_U64X(v1) < (int64_t)(int32_t)256) ? 1 : 0));
    if ((int64_t)v0 != 0) { (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)a1, 0)))); { A3_BACK(); goto L_102848; } }
    (a1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 4294967295)))); a3xw0 = true;
L_102870:;
    { const bool c = (int32_t)a1 < 0; (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)SLL32((uint32_t)a1, 2)))); if (c) goto L_102898; }
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(a2, 4294952848))));
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v1, (uint32_t)v0))));
    (v0 = (uint64_t)((uint64_t)a1 + (uint64_t)0ULL));
    { A1_W32B((uint32_t)v1, 0, (uint32_t)a0); if (xo) { xo[0] = a1; xo[1] = a1_sx64(a2); } return v0; }
L_102890:;
    (a1 = (uint64_t)((uint64_t)v1 + (uint64_t)0ULL)); a3xw0 = true;
    { A3_BACK(); goto L_102870; }
L_102898:;
    { (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 4294967295)))); if (xo) { if (a3xw0) xo[0] = a1; xo[1] = a1_sx64(a2); } return v0; }
    __builtin_unreachable();
}

// LoaderSysDeleteExternalIntcHandlerList (0x1028a0): typed by tools/typed_lift2.py (round A3; loop leaf)
inline uint64_t LoaderSysDeleteExternalIntcHandlerList(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint64_t a0)
{
    uint64_t v0 = 0;
    uint32_t v1 = 0;
    uint64_t a1 = 0;
    uint32_t a2 = 0;
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)20 << 16))));
    (a1 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v0, 4294950800))));
    a2 = 0xFFFFFFFFu;
    (v1 = (uint32_t)((int32_t)((int32_t)ADD32((uint32_t)v0, 4))));
L_1028b8:;
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B(v1, 0))));
    if ((int64_t)v0 != (int64_t)a0) { (a1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)a1, 1)))); goto L_1028d8; }
    A1_W32(v1 - 4u, a2);
    (v0 = (uint64_t)((uint64_t)a0 + (uint64_t)0ULL));
    { A1_W32(v1, a2); return v0; }
L_1028d8:;
    (v0 = (uint64_t)(((int64_t)(int64_t)a1 < (int64_t)(int32_t)256) ? 1 : 0));
    { const bool c = (int64_t)v0 != 0; v1 = v1 + 8u; if (c) { A3_BACK(); goto L_1028b8; } }
    { (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 4294967295)))); return v0; }
    __builtin_unreachable();
}

// LoaderSysDeleteExternalThreadList (0x1028f0): typed by tools/typed_lift2.py (round A3; loop leaf, extra outputs xo[0]=a1 xo[1]=a2)
inline uint64_t LoaderSysDeleteExternalThreadList(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint64_t a0, uint64_t *xo = nullptr)
{
    uint64_t v0 = 0;
    uint32_t v1 = 0;
    uint64_t a1 = 0;
    uint32_t a2 = 0;
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)20 << 16))));
    (a1 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL));
    (v1 = (uint32_t)((int32_t)((int32_t)ADD32((uint32_t)v0, 4294948752))));
    a2 = 0xFFFFFFFFu;
L_102900:;
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B(v1, 0))));
    if ((int64_t)v0 != (int64_t)a0) { (a1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)a1, 1)))); goto L_102918; }
    A1_W32(v1, a2);
    { (v0 = (uint64_t)((uint64_t)a0 + (uint64_t)0ULL)); if (xo) { xo[0] = a1; xo[1] = a1_sx64(a2); } return v0; }
L_102918:;
    (v0 = (uint64_t)(((int64_t)(int64_t)a1 < (int64_t)(int32_t)256) ? 1 : 0));
    { const bool c = (int64_t)v0 != 0; v1 = v1 + 4u; if (c) { A3_BACK(); goto L_102900; } }
    { (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 4294967295)))); if (xo) { xo[0] = a1; xo[1] = a1_sx64(a2); } return v0; }
    __builtin_unreachable();
}

// LoaderSysDeleteExternalSemaList (0x102930): typed by tools/typed_lift2.py (round A3; loop leaf, extra outputs xo[0]=a1 xo[1]=a2)
inline uint64_t LoaderSysDeleteExternalSemaList(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint64_t a0, uint64_t *xo = nullptr)
{
    uint64_t v0 = 0;
    uint32_t v1 = 0;
    uint64_t a1 = 0;
    uint32_t a2 = 0;
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)20 << 16))));
    (a1 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL));
    (v1 = (uint32_t)((int32_t)((int32_t)ADD32((uint32_t)v0, 4294949776))));
    a2 = 0xFFFFFFFFu;
L_102940:;
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B(v1, 0))));
    if ((int64_t)v0 != (int64_t)a0) { (a1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)a1, 1)))); goto L_102958; }
    A1_W32(v1, a2);
    { (v0 = (uint64_t)((uint64_t)a0 + (uint64_t)0ULL)); if (xo) { xo[0] = a1; xo[1] = a1_sx64(a2); } return v0; }
L_102958:;
    (v0 = (uint64_t)(((int64_t)(int64_t)a1 < (int64_t)(int32_t)256) ? 1 : 0));
    { const bool c = (int64_t)v0 != 0; v1 = v1 + 4u; if (c) { A3_BACK(); goto L_102940; } }
    { (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 4294967295)))); if (xo) { xo[0] = a1; xo[1] = a1_sx64(a2); } return v0; }
    __builtin_unreachable();
}

// LoaderSysDeleteExternalIopMemoryList (0x102970): typed by tools/typed_lift2.py (round A3; loop leaf, extra outputs xo[0]=a1 xo[1]=a2 xo[2]=a3 xo[3]=t0)
inline uint64_t LoaderSysDeleteExternalIopMemoryList(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint64_t a0, uint64_t *xo = nullptr)
{
    uint64_t v0 = 0;
    uint32_t v1 = 0;
    uint32_t a1 = 0;
    uint64_t a2 = 0;
    uint64_t a3 = 0;
    uint32_t t0 = 0;
    v1 = 0x140000u;
    (a2 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL));
    t0 = v1 - 0x3870u;
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B(t0, 0))));
L_102980:;
    t0 = t0 + 4u;
    (a3 = (uint64_t)((uint64_t)a2 + (uint64_t)0ULL));
    (a2 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)a2, 1))));
    { const bool c = (int64_t)v0 == (int64_t)a0; (a1 = (uint32_t)(((int64_t)(int64_t)RC_U64X(a2) < (int64_t)(int32_t)256) ? 1 : 0)); if (c) goto L_1029a8; }
    if (a1 != 0) { (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B(t0, 0)))); { A3_BACK(); goto L_102980; } }
    (a3 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 4294967295))));
L_1029a8:;
    { const bool c = (int32_t)a3 < 0; v1 = v1 - 0x3870u; if (c) goto L_1029bc; }
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)SLL32((uint32_t)a3, 2))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v0, v1))));
    A1_W32B((uint32_t)v0, 0, 0u);
L_1029bc:;
    { (v0 = (uint64_t)((uint64_t)a3 + (uint64_t)0ULL)); if (xo) { xo[0] = a1_sx64(a1); xo[1] = a2; xo[2] = a3; xo[3] = a1_sx64(t0); } return v0; }
    __builtin_unreachable();
}

// LoaderSysInitExternalIntcHandlerList (0x1029c8): typed by tools/typed_lift2.py (round A3; loop leaf)
inline uint32_t LoaderSysInitExternalIntcHandlerList(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt)
{
    uint32_t v0 = 0;
    uint32_t v1 = 0;
    uint32_t a0 = 0;
    v0 = 0x140000u;
    a0 = 0xFFFFFFFFu;
    v0 = v0 - 0x4070u;
    v1 = 0xFFu;
L_1029d8:;
    v1 = v1 - 1u;
    A1_W32B(v0, 4, a0);
    A1_W32(v0, a0);
    { const bool c = (int32_t)v1 >= 0; v0 = v0 + 8u; if (c) { A3_BACK(); goto L_1029d8; } }
    { return v0; }
    __builtin_unreachable();
}

// LoaderSysInitExternalSemaList (0x102a00): typed by tools/typed_lift2.py (round A3; loop leaf)
inline uint32_t LoaderSysInitExternalSemaList(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt)
{
    uint32_t v0 = 0;
    uint32_t v1 = 0;
    uint32_t a0 = 0;
    v0 = 0x140000u;
    a0 = 0xFFFFFFFFu;
    v0 = v0 - 0x4470u;
    v1 = 0xFFu;
    v0 = v0 + 0x3FCu;
L_102a18:;
    v1 = v1 - 1u;
    A1_W32(v0, a0);
    { const bool c = (int32_t)v1 >= 0; v0 = v0 - 4u; if (c) { A3_BACK(); goto L_102a18; } }
    { return v0; }
    __builtin_unreachable();
}

// LoaderSysInitExternalThreadList (0x102a40): typed by tools/typed_lift2.py (round A3; loop leaf)
inline uint32_t LoaderSysInitExternalThreadList(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt)
{
    uint32_t v0 = 0;
    uint32_t v1 = 0;
    uint32_t a0 = 0;
    v0 = 0x140000u;
    a0 = 0xFFFFFFFFu;
    v0 = v0 - 0x4870u;
    v1 = 0xFFu;
    v0 = v0 + 0x3FCu;
L_102a58:;
    v1 = v1 - 1u;
    A1_W32(v0, a0);
    { const bool c = (int32_t)v1 >= 0; v0 = v0 - 4u; if (c) { A3_BACK(); goto L_102a58; } }
    { return v0; }
    __builtin_unreachable();
}

// LoaderSysInitExternalIopMemoryList (0x102c58): typed by tools/typed_lift2.py (round A3; loop leaf)
inline uint32_t LoaderSysInitExternalIopMemoryList(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt)
{
    uint32_t v0 = 0;
    uint32_t v1 = 0;
    v0 = 0x140000u;
    v1 = 0xFFu;
    v0 = v0 - 0x3870u;
    v0 = v0 + 0x3FCu;
L_102c68:;
    v1 = v1 - 1u;
    A1_W32(v0, 0u);
    { const bool c = (int32_t)v1 >= 0; v0 = v0 - 4u; if (c) { A3_BACK(); goto L_102c68; } }
    { return v0; }
    __builtin_unreachable();
}

// LoaderSysGetMemoryInfo (0x103110): typed by tools/typed_lift2.py (round A3; loop leaf, extra outputs xo[0]=a0 xo[1]=a1 xo[2]=a2 xo[3]=a3 xo[4]=t0 xo[5]=t1 xo[6]=t2 xo[7]=t3)
inline uint64_t LoaderSysGetMemoryInfo(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint64_t a0, uint64_t gp, uint64_t *xo = nullptr)
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
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)gp, -32624))));
    A1_W32B((uint32_t)a0, 24, (uint32_t)v0);
    (t0 = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)510 << 16))));
    (t0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)t0, 0))));
    A1_W32B((uint32_t)a0, 0, (uint32_t)t0);
    (a3 = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)2 << 16))));
    (a3 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)a3, 0))));
    (a2 = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)23 << 16))));
    (a2 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)a2, 4294943640))));
    A1_W32B((uint32_t)a0, 8, (uint32_t)a2);
    (t2 = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)4 << 16))));
    (t2 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)t2, 0))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)19 << 16))));
    (t4 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)a0, 32))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v0, 7168))));
    (t1 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)gp, -32092))));
    (t3 = (uint64_t)((uint64_t)RC_U64X(v0) + (uint64_t)0ULL));
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)v0, 64))));
    (a1 = (uint64_t)(RC_U64X(t4) | RC_U64X(t3)));
    (a3 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)t0, (uint32_t)a3))));
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)t1, (uint32_t)v1))));
    (a2 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)a2, (uint32_t)t2))));
    (a1 = (uint64_t)(RC_U64X(a1) & (uint64_t)(uint16_t)7));
    A1_W32B((uint32_t)a0, 20, (uint32_t)v1);
    A1_W32B((uint32_t)a0, 4, (uint32_t)a3);
    A1_W32B((uint32_t)a0, 12, (uint32_t)a2);
    A1_W32B((uint32_t)a0, 16, (uint32_t)t1);
    { const bool c = (int64_t)a1 == 0; A1_W32B((uint32_t)a0, 28, (uint32_t)t0); if (c) goto L_103208; }
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)t3, 64))));
L_1031a8:;
    { uint32_t addr = ADD32((uint32_t)t3, 7); uint32_t aligned_addr = addr & ~7u; uint32_t offset = addr & 7u; uint64_t mem = A1_R64(aligned_addr); uint32_t shift = (7u - offset) << 3; uint64_t keepMask = (shift == 0) ? 0ull : ((1ull << shift) - 1ull); (v1 = (uint64_t)((v1 & keepMask) | (mem << shift))); }
    { uint32_t addr = ADD32((uint32_t)t3, 0); uint32_t aligned_addr = addr & ~7u; uint32_t offset = addr & 7u; uint64_t mem = A1_R64(aligned_addr); uint32_t shift = offset << 3; uint64_t keepMask = (offset == 0) ? 0ull : (0xFFFFFFFFFFFFFFFFull << ((8u - offset) << 3)); (v1 = (uint64_t)((v1 & keepMask) | (mem >> shift))); }
    { uint32_t addr = ADD32((uint32_t)t3, 15); uint32_t aligned_addr = addr & ~7u; uint32_t offset = addr & 7u; uint64_t mem = A1_R64(aligned_addr); uint32_t shift = (7u - offset) << 3; uint64_t keepMask = (shift == 0) ? 0ull : ((1ull << shift) - 1ull); (a0 = (uint64_t)((a0 & keepMask) | (mem << shift))); }
    { uint32_t addr = ADD32((uint32_t)t3, 8); uint32_t aligned_addr = addr & ~7u; uint32_t offset = addr & 7u; uint64_t mem = A1_R64(aligned_addr); uint32_t shift = offset << 3; uint64_t keepMask = (offset == 0) ? 0ull : (0xFFFFFFFFFFFFFFFFull << ((8u - offset) << 3)); (a0 = (uint64_t)((a0 & keepMask) | (mem >> shift))); }
    { uint32_t addr = ADD32((uint32_t)t3, 23); uint32_t aligned_addr = addr & ~7u; uint32_t offset = addr & 7u; uint64_t mem = A1_R64(aligned_addr); uint32_t shift = (7u - offset) << 3; uint64_t keepMask = (shift == 0) ? 0ull : ((1ull << shift) - 1ull); (a1 = (uint64_t)((a1 & keepMask) | (mem << shift))); }
    { uint32_t addr = ADD32((uint32_t)t3, 16); uint32_t aligned_addr = addr & ~7u; uint32_t offset = addr & 7u; uint64_t mem = A1_R64(aligned_addr); uint32_t shift = offset << 3; uint64_t keepMask = (offset == 0) ? 0ull : (0xFFFFFFFFFFFFFFFFull << ((8u - offset) << 3)); (a1 = (uint64_t)((a1 & keepMask) | (mem >> shift))); }
    { uint32_t addr = ADD32((uint32_t)t3, 31); uint32_t aligned_addr = addr & ~7u; uint32_t offset = addr & 7u; uint64_t mem = A1_R64(aligned_addr); uint32_t shift = (7u - offset) << 3; uint64_t keepMask = (shift == 0) ? 0ull : ((1ull << shift) - 1ull); (a2 = (uint64_t)((a2 & keepMask) | (mem << shift))); }
    { uint32_t addr = ADD32((uint32_t)t3, 24); uint32_t aligned_addr = addr & ~7u; uint32_t offset = addr & 7u; uint64_t mem = A1_R64(aligned_addr); uint32_t shift = offset << 3; uint64_t keepMask = (offset == 0) ? 0ull : (0xFFFFFFFFFFFFFFFFull << ((8u - offset) << 3)); (a2 = (uint64_t)((a2 & keepMask) | (mem >> shift))); }
    { uint32_t addr = ADD32((uint32_t)t4, 7); uint32_t aligned_addr = addr & ~7u; uint32_t offset = addr & 7u; uint32_t shift = (7u - offset) << 3; uint64_t mask = 0xFFFFFFFFFFFFFFFFull >> shift; uint64_t old_data = A1_R64(aligned_addr); uint64_t val = v1; uint64_t new_data = (old_data & ~mask) | ((val >> shift) & mask); A1_W64(aligned_addr, new_data); }
    { uint32_t addr = ADD32((uint32_t)t4, 0); uint32_t aligned_addr = addr & ~7u; uint32_t offset = addr & 7u; uint32_t shift = offset << 3; uint64_t mask = 0xFFFFFFFFFFFFFFFFull << shift; uint64_t old_data = A1_R64(aligned_addr); uint64_t val = v1; uint64_t new_data = (old_data & ~mask) | ((val << shift) & mask); A1_W64(aligned_addr, new_data); }
    { uint32_t addr = ADD32((uint32_t)t4, 15); uint32_t aligned_addr = addr & ~7u; uint32_t offset = addr & 7u; uint32_t shift = (7u - offset) << 3; uint64_t mask = 0xFFFFFFFFFFFFFFFFull >> shift; uint64_t old_data = A1_R64(aligned_addr); uint64_t val = a0; uint64_t new_data = (old_data & ~mask) | ((val >> shift) & mask); A1_W64(aligned_addr, new_data); }
    { uint32_t addr = ADD32((uint32_t)t4, 8); uint32_t aligned_addr = addr & ~7u; uint32_t offset = addr & 7u; uint32_t shift = offset << 3; uint64_t mask = 0xFFFFFFFFFFFFFFFFull << shift; uint64_t old_data = A1_R64(aligned_addr); uint64_t val = a0; uint64_t new_data = (old_data & ~mask) | ((val << shift) & mask); A1_W64(aligned_addr, new_data); }
    { uint32_t addr = ADD32((uint32_t)t4, 23); uint32_t aligned_addr = addr & ~7u; uint32_t offset = addr & 7u; uint32_t shift = (7u - offset) << 3; uint64_t mask = 0xFFFFFFFFFFFFFFFFull >> shift; uint64_t old_data = A1_R64(aligned_addr); uint64_t val = a1; uint64_t new_data = (old_data & ~mask) | ((val >> shift) & mask); A1_W64(aligned_addr, new_data); }
    { uint32_t addr = ADD32((uint32_t)t4, 16); uint32_t aligned_addr = addr & ~7u; uint32_t offset = addr & 7u; uint32_t shift = offset << 3; uint64_t mask = 0xFFFFFFFFFFFFFFFFull << shift; uint64_t old_data = A1_R64(aligned_addr); uint64_t val = a1; uint64_t new_data = (old_data & ~mask) | ((val << shift) & mask); A1_W64(aligned_addr, new_data); }
    { uint32_t addr = ADD32((uint32_t)t4, 31); uint32_t aligned_addr = addr & ~7u; uint32_t offset = addr & 7u; uint32_t shift = (7u - offset) << 3; uint64_t mask = 0xFFFFFFFFFFFFFFFFull >> shift; uint64_t old_data = A1_R64(aligned_addr); uint64_t val = a2; uint64_t new_data = (old_data & ~mask) | ((val >> shift) & mask); A1_W64(aligned_addr, new_data); }
    { uint32_t addr = ADD32((uint32_t)t4, 24); uint32_t aligned_addr = addr & ~7u; uint32_t offset = addr & 7u; uint32_t shift = offset << 3; uint64_t mask = 0xFFFFFFFFFFFFFFFFull << shift; uint64_t old_data = A1_R64(aligned_addr); uint64_t val = a2; uint64_t new_data = (old_data & ~mask) | ((val << shift) & mask); A1_W64(aligned_addr, new_data); }
    (t3 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)t3, 32))));
    { const bool c = (int64_t)t3 != (int64_t)v0; (t4 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)t4, 32)))); if (c) { A3_BACK(); goto L_1031a8; } }
    goto L_103240;
L_103208:;
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)t3, 64))));
L_10320c:;
    (v1 = (uint64_t)(A1_R64B((uint32_t)t3, 0)));
    (a0 = (uint64_t)(A1_R64B((uint32_t)t3, 8)));
    (a1 = (uint64_t)(A1_R64B((uint32_t)t3, 16)));
    (a2 = (uint64_t)(A1_R64B((uint32_t)t3, 24)));
    A1_W64B((uint32_t)t4, 0, v1);
    A1_W64B((uint32_t)t4, 8, a0);
    A1_W64B((uint32_t)t4, 16, a1);
    A1_W64B((uint32_t)t4, 24, a2);
    (t3 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)t3, 32))));
    { const bool c = (int64_t)t3 != (int64_t)v0; (t4 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)t4, 32)))); if (c) { A3_BACK(); goto L_10320c; } }
L_103240:;
    { uint32_t addr = ADD32((uint32_t)t3, 7); uint32_t aligned_addr = addr & ~7u; uint32_t offset = addr & 7u; uint64_t mem = A1_R64(aligned_addr); uint32_t shift = (7u - offset) << 3; uint64_t keepMask = (shift == 0) ? 0ull : ((1ull << shift) - 1ull); (v0 = (uint64_t)((v0 & keepMask) | (mem << shift))); }
    { uint32_t addr = ADD32((uint32_t)t3, 0); uint32_t aligned_addr = addr & ~7u; uint32_t offset = addr & 7u; uint64_t mem = A1_R64(aligned_addr); uint32_t shift = offset << 3; uint64_t keepMask = (offset == 0) ? 0ull : (0xFFFFFFFFFFFFFFFFull << ((8u - offset) << 3)); (v0 = (uint64_t)((v0 & keepMask) | (mem >> shift))); }
    { uint32_t addr = ADD32((uint32_t)t4, 7); uint32_t aligned_addr = addr & ~7u; uint32_t offset = addr & 7u; uint32_t shift = (7u - offset) << 3; uint64_t mask = 0xFFFFFFFFFFFFFFFFull >> shift; uint64_t old_data = A1_R64(aligned_addr); uint64_t val = v0; uint64_t new_data = (old_data & ~mask) | ((val >> shift) & mask); A1_W64(aligned_addr, new_data); }
    { uint32_t addr = ADD32((uint32_t)t4, 0); uint32_t aligned_addr = addr & ~7u; uint32_t offset = addr & 7u; uint32_t shift = offset << 3; uint64_t mask = 0xFFFFFFFFFFFFFFFFull << shift; uint64_t old_data = A1_R64(aligned_addr); uint64_t val = v0; uint64_t new_data = (old_data & ~mask) | ((val << shift) & mask); A1_W64(aligned_addr, new_data); }
    { (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 1)))); if (xo) { xo[0] = a0; xo[1] = a1; xo[2] = a2; xo[3] = a3; xo[4] = t0; xo[5] = t1; xo[6] = t2; xo[7] = t3; } return v0; }
    __builtin_unreachable();
}

// ScrollDisplay (0x106390): typed by tools/typed_lift2.py (round A3; loop leaf)
inline uint64_t ScrollDisplay(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint64_t gp)
{
    uint64_t v0 = 0;
    uint64_t v1 = 0;
    uint64_t a0 = 0;
    uint64_t a1 = 0;
    uint64_t a2 = 0;
    uint64_t a3 = 0;
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)20 << 16))));
    (a1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 1))));
    (a2 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v0, 4294962576))));
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 15))));
L_1063a0:;
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)SLL32((uint32_t)a0, 5))));
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 59))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v0, (uint32_t)a2))));
L_1063b0:;
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v1, 4294967295))));
    { uint32_t addr = ADD32((uint32_t)v0, 7); uint32_t aligned_addr = addr & ~7u; uint32_t offset = addr & 7u; uint64_t mem = A1_R64(aligned_addr); uint32_t shift = (7u - offset) << 3; uint64_t keepMask = (shift == 0) ? 0ull : ((1ull << shift) - 1ull); (a3 = (uint64_t)((a3 & keepMask) | (mem << shift))); }
    { uint32_t addr = ADD32((uint32_t)v0, 0); uint32_t aligned_addr = addr & ~7u; uint32_t offset = addr & 7u; uint64_t mem = A1_R64(aligned_addr); uint32_t shift = offset << 3; uint64_t keepMask = (offset == 0) ? 0ull : (0xFFFFFFFFFFFFFFFFull << ((8u - offset) << 3)); (a3 = (uint64_t)((a3 & keepMask) | (mem >> shift))); }
    { uint32_t addr = ADD32((uint32_t)v0, 4294966823); uint32_t aligned_addr = addr & ~7u; uint32_t offset = addr & 7u; uint32_t shift = (7u - offset) << 3; uint64_t mask = 0xFFFFFFFFFFFFFFFFull >> shift; uint64_t old_data = A1_R64(aligned_addr); uint64_t val = a3; uint64_t new_data = (old_data & ~mask) | ((val >> shift) & mask); A1_W64(aligned_addr, new_data); }
    { uint32_t addr = ADD32((uint32_t)v0, 4294966816); uint32_t aligned_addr = addr & ~7u; uint32_t offset = addr & 7u; uint32_t shift = offset << 3; uint64_t mask = 0xFFFFFFFFFFFFFFFFull << shift; uint64_t old_data = A1_R64(aligned_addr); uint64_t val = a3; uint64_t new_data = (old_data & ~mask) | ((val << shift) & mask); A1_W64(aligned_addr, new_data); }
    { const bool c = (int32_t)v1 >= 0; (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v0, 8)))); if (c) { A3_BACK(); goto L_1063b0; } }
    (a1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)a1, 1))));
    (v0 = (uint64_t)(((int64_t)(int64_t)RC_U64X(a1) < (int64_t)(int32_t)40) ? 1 : 0));
    { const bool c = (int64_t)v0 != 0; (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)a0, 15)))); if (c) { A3_BACK(); goto L_1063a0; } }
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)20 << 16))));
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)gp, 4294935536))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v0, 4294962576))));
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 59))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v0, 18720))));
L_106400:;
    { uint32_t addr = ADD32((uint32_t)a0, 7); uint32_t aligned_addr = addr & ~7u; uint32_t offset = addr & 7u; uint64_t mem = A1_R64(aligned_addr); uint32_t shift = (7u - offset) << 3; uint64_t keepMask = (shift == 0) ? 0ull : ((1ull << shift) - 1ull); (a1 = (uint64_t)((a1 & keepMask) | (mem << shift))); }
    { uint32_t addr = ADD32((uint32_t)a0, 0); uint32_t aligned_addr = addr & ~7u; uint32_t offset = addr & 7u; uint64_t mem = A1_R64(aligned_addr); uint32_t shift = offset << 3; uint64_t keepMask = (offset == 0) ? 0ull : (0xFFFFFFFFFFFFFFFFull << ((8u - offset) << 3)); (a1 = (uint64_t)((a1 & keepMask) | (mem >> shift))); }
    { uint32_t addr = ADD32((uint32_t)v0, 7); uint32_t aligned_addr = addr & ~7u; uint32_t offset = addr & 7u; uint32_t shift = (7u - offset) << 3; uint64_t mask = 0xFFFFFFFFFFFFFFFFull >> shift; uint64_t old_data = A1_R64(aligned_addr); uint64_t val = a1; uint64_t new_data = (old_data & ~mask) | ((val >> shift) & mask); A1_W64(aligned_addr, new_data); }
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v1, 4294967295))));
    { uint32_t addr = ADD32((uint32_t)v0, 0); uint32_t aligned_addr = addr & ~7u; uint32_t offset = addr & 7u; uint32_t shift = offset << 3; uint64_t mask = 0xFFFFFFFFFFFFFFFFull << shift; uint64_t old_data = A1_R64(aligned_addr); uint64_t val = a1; uint64_t new_data = (old_data & ~mask) | ((val << shift) & mask); A1_W64(aligned_addr, new_data); }
    { const bool c = (int32_t)v1 >= 0; (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v0, 8)))); if (c) { A3_BACK(); goto L_106400; } }
    { return v0; }
    __builtin_unreachable();
}

// ClearDisplay (0x106430): typed by tools/typed_lift2.py (round A3; loop leaf)
inline uint64_t ClearDisplay(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint64_t gp)
{
    uint64_t v0 = 0;
    uint64_t v1 = 0;
    uint64_t a0 = 0;
    uint64_t a1 = 0;
    uint64_t a2 = 0;
    uint64_t a3 = 0;
    uint64_t t0 = 0;
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)20 << 16))));
    (a1 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL));
    (a3 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v0, 4294962576))));
    (a2 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)gp, 4294935536))));
    (a0 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL));
L_106448:;
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)SLL32((uint32_t)a0, 5))));
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 59))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v0, (uint32_t)a3))));
L_106458:;
    { uint32_t addr = ADD32((uint32_t)a2, 7); uint32_t aligned_addr = addr & ~7u; uint32_t offset = addr & 7u; uint64_t mem = A1_R64(aligned_addr); uint32_t shift = (7u - offset) << 3; uint64_t keepMask = (shift == 0) ? 0ull : ((1ull << shift) - 1ull); (t0 = (uint64_t)((t0 & keepMask) | (mem << shift))); }
    { uint32_t addr = ADD32((uint32_t)a2, 0); uint32_t aligned_addr = addr & ~7u; uint32_t offset = addr & 7u; uint64_t mem = A1_R64(aligned_addr); uint32_t shift = offset << 3; uint64_t keepMask = (offset == 0) ? 0ull : (0xFFFFFFFFFFFFFFFFull << ((8u - offset) << 3)); (t0 = (uint64_t)((t0 & keepMask) | (mem >> shift))); }
    { uint32_t addr = ADD32((uint32_t)v0, 7); uint32_t aligned_addr = addr & ~7u; uint32_t offset = addr & 7u; uint32_t shift = (7u - offset) << 3; uint64_t mask = 0xFFFFFFFFFFFFFFFFull >> shift; uint64_t old_data = A1_R64(aligned_addr); uint64_t val = t0; uint64_t new_data = (old_data & ~mask) | ((val >> shift) & mask); A1_W64(aligned_addr, new_data); }
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v1, 4294967295))));
    { uint32_t addr = ADD32((uint32_t)v0, 0); uint32_t aligned_addr = addr & ~7u; uint32_t offset = addr & 7u; uint32_t shift = offset << 3; uint64_t mask = 0xFFFFFFFFFFFFFFFFull << shift; uint64_t old_data = A1_R64(aligned_addr); uint64_t val = t0; uint64_t new_data = (old_data & ~mask) | ((val << shift) & mask); A1_W64(aligned_addr, new_data); }
    { const bool c = (int32_t)v1 >= 0; (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v0, 8)))); if (c) { A3_BACK(); goto L_106458; } }
    (a1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)a1, 1))));
    (v0 = (uint64_t)(((int64_t)(int64_t)RC_U64X(a1) < (int64_t)(int32_t)40) ? 1 : 0));
    { const bool c = (int64_t)v0 != 0; (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)a0, 15)))); if (c) { A3_BACK(); goto L_106448; } }
    { return v0; }
    __builtin_unreachable();
}

// sub_00109640 (0x109640): typed by tools/typed_lift2.py (round A3; loop leaf)
inline uint32_t sub_00109640(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint64_t a0, uint32_t a1)
{
    uint32_t v0 = 0;
    uint32_t v1 = 0;
    a1 = A1_R32B(a1, 40);
    if (a1 == 0) goto L_109684;
    v1 = A1_R32B(a1, 8);
L_109650:;
    if (v1 == 0) { a1 = A1_R32B(a1, 20); goto L_10967c; }
    v0 = A1_R32(v1);
L_109660:;
    if ((int64_t)(int32_t)v0 != (int64_t)a0) { v1 = A1_R32B(v1, 56); goto L_109670; }
    { v0 = v1; return v0; }
L_109670:;
    if (v1 != 0) { v0 = A1_R32(v1); { A3_BACK(); goto L_109660; } }
    a1 = A1_R32B(a1, 20);
L_10967c:;
    if (a1 != 0) { v1 = A1_R32B(a1, 8); { A3_BACK(); goto L_109650; } }
L_109684:;
    { v0 = 0u; return v0; }
    __builtin_unreachable();
}

// sub_0010F370 (0x10f370): typed by tools/typed_lift2.py (round A3; loop leaf)
inline uint32_t sub_0010F370(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t a0, uint32_t a1, uint32_t a2)
{
    uint32_t v0 = 0;
    uint32_t v1 = 0;
    uint64_t a3 = 0;
    a2 = a2 >> 2;
    { const bool c = a2 == 0; (a3 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL)); if (c) goto L_10f3a0; }
L_10f380:;
    v1 = A1_R32(a1);
    (a3 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)a3, 1))));
    a1 = a1 + 4u;
    (v0 = (uint32_t)(((uint64_t)RC_U64X(a3) < (uint64_t)a1_sx64(a2)) ? 1 : 0));
    A1_W32(a0, v1);
    a0 = a0 + 4u;
    if (v0 != 0) { A3_BACK(); goto L_10f380; }
L_10f3a0:;
    { v0 = 0u; return v0; }
    __builtin_unreachable();
}

// sub_0010FC08 (0x10fc08): typed by tools/typed_lift2.py (round A3; loop leaf)
inline uint32_t sub_0010FC08(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t a0, uint32_t a1, uint32_t a2)
{
    uint32_t v0 = 0;
    uint32_t v1 = 0;
    uint64_t a3 = 0;
    a2 = a2 >> 2;
    { const bool c = a2 == 0; (a3 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL)); if (c) goto L_10fc38; }
L_10fc18:;
    v1 = A1_R32(a1);
    (a3 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)a3, 1))));
    a1 = a1 + 4u;
    (v0 = (uint32_t)(((uint64_t)RC_U64X(a3) < (uint64_t)a1_sx64(a2)) ? 1 : 0));
    A1_W32(a0, v1);
    a0 = a0 + 4u;
    if (v0 != 0) { A3_BACK(); goto L_10fc18; }
L_10fc38:;
    { v0 = 0u; return v0; }
    __builtin_unreachable();
}

// sub_0010FC40 (0x10fc40): typed by tools/typed_lift2.py (round A3; loop leaf)
inline uint64_t sub_0010FC40(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint64_t a0, uint64_t a1, uint64_t a2)
{
    uint64_t v0 = 0;
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)a0, 0))));
    { const bool c = (int64_t)v0 == (int64_t)a2; (v0 = (uint64_t)(((uint64_t)a0 < (uint64_t)a1) ? 1 : 0)); if (c) goto L_10fc74; }
    if ((int64_t)v0 == 0) { if (RC_U64X(v0) == 0) a0 = (uint64_t)0;; goto L_10fc78; }
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)a0, 4))));
L_10fc58:;
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)a0, 0))));
    { const bool c = (int64_t)v0 == (int64_t)a2; (v0 = (uint64_t)(((uint64_t)a0 < (uint64_t)a1) ? 1 : 0)); if (c) goto L_10fc74; }
    if ((int64_t)v0 != 0) { (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)a0, 4)))); { A3_BACK(); goto L_10fc58; } }
    if (RC_U64X(v0) == 0) a0 = (uint64_t)0;;
    goto L_10fc78;
L_10fc74:;
    if (v0 == 0) a0 = (uint64_t)0;;
L_10fc78:;
    { (v0 = (uint64_t)((uint64_t)a0 + (uint64_t)0ULL)); return v0; }
    __builtin_unreachable();
}

// sub_0010FE50 (0x10fe50): typed by tools/typed_lift2.py (round A3; loop leaf)
inline uint32_t sub_0010FE50(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t a0, uint32_t a1, uint32_t a2)
{
    uint32_t v0 = 0;
    uint32_t v1 = 0;
    uint64_t a3 = 0;
    a2 = a2 >> 2;
    { const bool c = a2 == 0; (a3 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL)); if (c) goto L_10fe80; }
L_10fe60:;
    v1 = A1_R32(a1);
    (a3 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)a3, 1))));
    a1 = a1 + 4u;
    (v0 = (uint32_t)(((uint64_t)RC_U64X(a3) < (uint64_t)a1_sx64(a2)) ? 1 : 0));
    A1_W32(a0, v1);
    a0 = a0 + 4u;
    if (v0 != 0) { A3_BACK(); goto L_10fe60; }
L_10fe80:;
    { v0 = 0u; return v0; }
    __builtin_unreachable();
}

// sub_0010FFC0 (0x10ffc0): typed by tools/typed_lift2.py (round A3; loop leaf)
inline uint32_t sub_0010FFC0(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t a0, uint32_t a1, uint64_t a2)
{
    uint32_t v0 = 0;
    uint32_t v1 = 0;
    uint64_t a3 = 0;
    { const bool c = (int64_t)a2 == 0; (a3 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL)); if (c) goto L_10ffe8; }
L_10ffc8:;
    v1 = A1_R8(a1);
    (a3 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)a3, 1))));
    a1 = a1 + 1u;
    (v0 = (uint32_t)(((uint64_t)RC_U64X(a3) < (uint64_t)a2) ? 1 : 0));
    A1_W8(a0, (uint8_t)v1);
    a0 = a0 + 1u;
    if (v0 != 0) { A3_BACK(); goto L_10ffc8; }
L_10ffe8:;
    { v0 = 0u; return v0; }
    __builtin_unreachable();
}

// sub_00110270 (0x110270): typed by tools/typed_lift2.py (round A3; loop leaf)
inline uint32_t sub_00110270(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t a0, uint32_t a1, uint32_t a2)
{
    uint32_t v0 = 0;
    uint32_t v1 = 0;
    uint64_t a3 = 0;
    a2 = a2 >> 2;
    { const bool c = a2 == 0; (a3 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL)); if (c) goto L_1102a0; }
L_110280:;
    v1 = A1_R32(a1);
    (a3 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)a3, 1))));
    a1 = a1 + 4u;
    (v0 = (uint32_t)(((uint64_t)RC_U64X(a3) < (uint64_t)a1_sx64(a2)) ? 1 : 0));
    A1_W32(a0, v1);
    a0 = a0 + 4u;
    if (v0 != 0) { A3_BACK(); goto L_110280; }
L_1102a0:;
    { v0 = 0u; return v0; }
    __builtin_unreachable();
}

// sub_00110910 (0x110910): typed by tools/typed_lift2.py (round A3; loop leaf, extra outputs xo[0]=a0 xo[1]=a1 xo[2]=a2 xo[3]=a3 xo[4]=t0 xo[5]=t1)
inline uint64_t sub_00110910(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint64_t a0, uint64_t *xo = nullptr)
{
    uint64_t v0 = 0;
    uint64_t v1 = 0;
    uint64_t a1 = 0;
    uint64_t a2 = 0;
    uint64_t a3 = 0;
    uint64_t t0 = 0;
    uint64_t t1 = 0;
    (a2 = (uint64_t)((uint64_t)a0 + (uint64_t)0ULL));
    (t1 = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)19 << 16))));
    (v0 = (uint64_t)(A1_R64B((uint32_t)a2, 32)));
    (a1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)t1, 14616))));
    (v1 = (uint64_t)(A1_R64B((uint32_t)a2, 16)));
    (t0 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL));
    (a0 = (uint64_t)(A1_R64B((uint32_t)a2, 24)));
    (v0 = (uint64_t)((uint64_t)v0 + (uint64_t)v1));
    (a1 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)a1, 24))));
    (a3 = (uint64_t)(v0 - a0));
    goto L_110944;
L_110940:;
    (a1 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)t0, 0))));
L_110944:;
    if ((int64_t)a1 == 0) { A1_W32B((uint32_t)a2, 4, (uint32_t)t0); goto L_110970; }
    (v0 = (uint64_t)(A1_R64B((uint32_t)a1, 32)));
    (v1 = (uint64_t)(A1_R64B((uint32_t)a1, 16)));
    (a0 = (uint64_t)(A1_R64B((uint32_t)a1, 24)));
    (v0 = (uint64_t)((uint64_t)v0 + (uint64_t)v1));
    (v0 = (uint64_t)(v0 - a0));
    (v0 = (uint64_t)(((uint64_t)a3 < (uint64_t)v0) ? 1 : 0));
    if ((int64_t)v0 == 0) { (t0 = (uint64_t)((uint64_t)RC_U64X(a1) + (uint64_t)0ULL)); { A3_BACK(); goto L_110940; } }
    A1_W32B((uint32_t)a2, 4, (uint32_t)t0);
L_110970:;
    { const bool c = (int64_t)a1 == 0; A1_W32B((uint32_t)a2, 0, (uint32_t)a1); if (c) goto L_11097c; }
    A1_W32B((uint32_t)a1, 4, (uint32_t)a2);
L_11097c:;
    { const bool c = (int64_t)t0 == 0; (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)t1, 14616)))); if (c) goto L_11098c; }
    { A1_W32B((uint32_t)t0, 0, (uint32_t)a2); if (xo) { xo[0] = a0; xo[1] = a1; xo[2] = a2; xo[3] = a3; xo[4] = t0; xo[5] = t1; } return v0; }
L_11098c:;
    { A1_W32B((uint32_t)v0, 24, (uint32_t)a2); if (xo) { xo[0] = a0; xo[1] = a1; xo[2] = a2; xo[3] = a3; xo[4] = t0; xo[5] = t1; } return v0; }
    __builtin_unreachable();
}

// iGetTimerSystemTime (0x110c78): typed by tools/typed_lift2.py (round A3; leaf, extra outputs xo[0]=a0 xo[1]=t0 xo[2]=t1 xo[3]=t2)
inline uint64_t iGetTimerSystemTime(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint64_t *xo = nullptr)
{
    uint64_t v0 = 0;
    uint64_t v1 = 0;
    uint64_t a0 = 0;
    uint64_t t0 = 0;
    uint64_t t1 = 0;
    uint64_t t2 = 0;
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)4096 << 16))));
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)19 << 16))));
    (v1 = (uint64_t)(RC_U64X(v1) | (uint64_t)(uint16_t)4096));
    (v0 = (uint64_t)(uint32_t)(A1_R32B((uint32_t)v1, 0)));
    (t0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)v1, 16))));
    (t2 = (uint64_t)(A1_R64B((uint32_t)a0, 14616)));
    (t1 = (uint64_t)(RC_U64X(t0) & (uint64_t)(uint16_t)2048));
    { const bool c = (int64_t)t1 == 0; (t1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 2)))); if (c) goto L_110ca4; }
    (v0 = (uint64_t)(uint32_t)(A1_R32B((uint32_t)v1, 0)));
    (t2 = (uint64_t)((int64_t)(int64_t)t2 + (int64_t)(int32_t)1));
L_110ca4:;
    (t0 = (uint64_t)(RC_U64X(t0) & (uint64_t)(uint16_t)3));
    (t2 = (uint64_t)(t2 << 16));
    if (RC_U64X(t0) == 0) t1 = (uint64_t)0;;
    (v0 = (uint64_t)(v0 | t2));
    (t1 = (uint64_t)(t1 << ((uint32_t)t0 & 0x3F)));
    (v0 = (uint64_t)(v0 << ((uint32_t)t1 & 0x3F)));
    { if (xo) { xo[0] = a0; xo[1] = t0; xo[2] = t1; xo[3] = t2; } return v0; }
    __builtin_unreachable();
}

// iAllocTimerCounter (0x110d48): typed by tools/typed_lift2.py (round A3; leaf, extra outputs xo[0]=a0 xo[1]=a1 xo[2]=a2 xo[3]=a3)
inline uint32_t iAllocTimerCounter(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint64_t *xo = nullptr)
{
    uint32_t v0 = 0;
    uint32_t v1 = 0;
    uint32_t a0 = 0;
    uint32_t a1 = 0;
    uint32_t a2 = 0;
    uint32_t a3 = 0;
    v0 = 0x130000u;
    a3 = v0 + 0x3918u;
    a2 = A1_R32B(a3, 20);
    if (a2 != 0) goto L_110d68;
    v0 = 0x80000000u;
    { v0 = v0 | 0x8005u; if (xo) { xo[2] = a1_sx64(a2); xo[3] = a1_sx64(a3); } return v0; }
L_110d68:;
    v0 = A1_R32B(a3, 16);
    a1 = a2 << 4;
    a0 = A1_R32(a2);
    v0 = v0 + 1u;
    A1_W32B(a2, 40, 0u);
    A1_W32B(a3, 16, v0);
    A1_W32B(a2, 12, 0u);
    A1_W32B(a3, 20, a0);
    v1 = A1_R32B(a3, 12);
    A1_W64B(a2, 24, 0ULL);
    v1 = v1 + 1u;
    A1_W32B(a3, 12, v1);
    v0 = A1_R32B(a3, 12);
    v0 = v0 << 1;
    v0 = v0 & 0x3FEu;
    v0 = v0 | 1u;
    A1_W32B(a2, 8, v0);
    v0 = A1_R32B(a2, 8);
    { v0 = a1 | v0; if (xo) { xo[0] = a1_sx64(a0); xo[1] = a1_sx64(a1); xo[2] = a1_sx64(a2); xo[3] = a1_sx64(a3); } return v0; }
    __builtin_unreachable();
}

// iGetTimerBaseTime (0x111288): typed by tools/typed_lift2.py (round A3; leaf, extra outputs xo[0]=a1)
inline uint64_t iGetTimerBaseTime(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint64_t a0, uint64_t *xo = nullptr)
{
    uint64_t v0 = 0;
    uint64_t v1 = 0;
    uint64_t a1 = 0;
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)SRL32((uint32_t)a0, 10))));
    { const bool c = (int32_t)a0 < 0; (a1 = (uint64_t)((int64_t)(int32_t)((int32_t)SLL32((uint32_t)v0, 6)))); if (c) goto L_1112a4; }
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)a1, 8))));
    (v0 = (uint64_t)(a0 & (uint64_t)(uint16_t)1023));
    if ((int64_t)v0 == (int64_t)v1) { (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)a1, 12)))); goto L_1112ac; }
L_1112a4:;
    { (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 4294967295)))); if (xo) { xo[0] = a1; } return v0; }
L_1112ac:;
    (v0 = (uint64_t)(v0 & (uint64_t)(uint16_t)1));
    if ((int64_t)v0 == 0) goto L_1112c8;
    (v1 = (uint64_t)(A1_R64B((uint32_t)a1, 24)));
    (v0 = (uint64_t)(A1_R64B((uint32_t)a1, 16)));
    { (v0 = (uint64_t)(v0 - v1)); if (xo) { xo[0] = a1; } return v0; }
L_1112c8:;
    { (v0 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL)); if (xo) { xo[0] = a1; } return v0; }
    __builtin_unreachable();
}

// ps2__InitAlarm (0x111700): typed by tools/typed_lift2.py (round A3; loop leaf)
inline uint32_t ps2__InitAlarm(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt)
{
    uint32_t v0 = 0;
    uint32_t v1 = 0;
    uint32_t a0 = 0;
    uint32_t a1 = 0;
    a1 = 0x150000u;
    v1 = 0x150000u;
    v0 = a1 - 0x5780u;
    a0 = 0x3Fu;
    A1_W32(v1 - 0x5380u, v0);
    v1 = v0 + 0x400u;
    v0 = v0 + 0x3F0u;
L_111720:;
    A1_W32(v0, v1);
    a0 = a0 - 1u;
    v0 = v0 - 0x10u;
    v1 = v1 - 0x10u;
    if ((int32_t)a0 >= 0) { A3_BACK(); goto L_111720; }
    v1 = a1 - 0x5780u;
    v0 = 0u;
    { A1_W32B(v1, 1008, 0u); return v0; }
    __builtin_unreachable();
}

// sceVif1PkAddGsData (0x113368): typed by tools/typed_lift2.py (round A3; leaf, extra outputs xo[0]=a1)
inline uint64_t sceVif1PkAddGsData(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint64_t a0, uint64_t a1, uint64_t *xo = nullptr)
{
    uint64_t v0 = 0;
    uint64_t v1 = 0;
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)a0, 0))));
    (v1 = (uint64_t)(a1 << (32 + 0)));
    (v1 = (uint64_t)((int64_t)v1 >> (32 + 0)));
    (a1 = (uint64_t)((int64_t)a1 >> (32 + 0)));
    A1_W32B((uint32_t)v0, 0, (uint32_t)v1);
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v0, 4))));
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v0, 4))));
    A1_W32B((uint32_t)v0, 0, (uint32_t)a1);
    { A1_W32B((uint32_t)a0, 0, (uint32_t)v1); if (xo) { xo[0] = a1; } return v0; }
    __builtin_unreachable();
}

// sceGsSetDefAlphaEnv (0x114020): typed by tools/typed_lift2.py (round A3; leaf, extra outputs xo[0]=a2 xo[1]=a3 xo[2]=t0 xo[3]=t1)
inline uint64_t sceGsSetDefAlphaEnv(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint64_t a0, uint64_t a1, uint64_t *xo = nullptr)
{
    uint64_t v0 = 0;
    uint64_t v1 = 0;
    uint64_t a2 = 0;
    uint64_t a3 = 0;
    uint64_t t0 = 0;
    uint64_t t1 = 0;
    (a1 = (uint64_t)((int64_t)(int32_t)((int32_t)SLL32((uint32_t)a1, 16))));
    (t1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 66))));
    (a1 = (uint64_t)((int64_t)(int32_t)(SRA32((int32_t)a1, 16))));
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 68))));
    (a1 = (uint64_t)(RC_U64X(a1) << (32 + 16)));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 73))));
    (a1 = (uint64_t)((int64_t)a1 >> (32 + 16)));
    (a2 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 59))));
    (a3 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 129))));
    (a3 = (uint64_t)(RC_U64X(a3) << (32 + 0)));
    (a3 = (uint64_t)(a3 | (uint64_t)(uint16_t)32895));
    (t0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 74))));
    A1_W64B((uint32_t)a0, 8, RC_U64X(t1));
    A1_W64B((uint32_t)a0, 0, RC_U64X(v1));
    A1_W64B((uint32_t)a0, 24, RC_U64X(v0));
    A1_W64B((uint32_t)a0, 16, a1);
    A1_W64B((uint32_t)a0, 40, RC_U64X(a2));
    A1_W64B((uint32_t)a0, 32, a3);
    A1_W64B((uint32_t)a0, 56, RC_U64X(t0));
    A1_W64B((uint32_t)a0, 48, 0ULL);
    // SYNC instruction - memory barrier // In recompiled code, we don't need explicit memory barriers
    { (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 4)))); if (xo) { xo[0] = a2; xo[1] = a3; xo[2] = t0; xo[3] = t1; } return v0; }
    __builtin_unreachable();
}

// sceGsSetHalfOffset (0x114080): typed by tools/typed_lift2.py (round A3; leaf, extra outputs xo[0]=a1 xo[1]=a2 xo[2]=a3)
inline uint64_t sceGsSetHalfOffset(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint64_t a0, uint64_t a1, uint64_t a2, uint64_t a3, uint64_t *xo = nullptr)
{
    uint64_t v0 = 0;
    uint64_t v1 = 0;
    (v0 = (uint64_t)(A1_R64B((uint32_t)a0, 48)));
    (a1 = (uint64_t)((int64_t)(int32_t)((int32_t)SLL32((uint32_t)a1, 16))));
    (a2 = (uint64_t)((int64_t)(int32_t)((int32_t)SLL32((uint32_t)a2, 16))));
    (a1 = (uint64_t)((int64_t)(int32_t)(SRA32((int32_t)a1, 16))));
    (v1 = (uint64_t)(v0 >> 16));
    (a2 = (uint64_t)((int64_t)(int32_t)(SRA32((int32_t)a2, 16))));
    (v0 = (uint64_t)(v0 >> (32 + 16)));
    (v1 = (uint64_t)(v1 & (uint64_t)(uint16_t)2047));
    (v0 = (uint64_t)(v0 & (uint64_t)(uint16_t)2047));
    (v1 = (uint64_t)(RC_U64X(v1) << (32 + 0)));
    (v1 = (uint64_t)((int64_t)v1 >> (32 + 0)));
    (v0 = (uint64_t)(RC_U64X(v0) << (32 + 0)));
    (v0 = (uint64_t)((int64_t)v0 >> (32 + 0)));
    (a2 = (uint64_t)(RC_U64X(a2) << (32 + 16)));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v0, 1))));
    (a1 = (uint64_t)(RC_U64X(a1) << (32 + 16)));
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v1, 1))));
    (v0 = (uint64_t)(RC_U64X(v0) >> 1));
    (a2 = (uint64_t)((int64_t)a2 >> (32 + 16)));
    (a1 = (uint64_t)((int64_t)a1 >> (32 + 16)));
    (v1 = (uint64_t)(RC_U64X(v1) >> 1));
    (a2 = (uint64_t)(a2 - v0));
    (a1 = (uint64_t)(a1 - v1));
    (v0 = (uint64_t)(a2 << 4));
    (a3 = (uint64_t)((int64_t)(int32_t)((int32_t)SLL32((uint32_t)a3, 16))));
    { const bool c = (int64_t)a3 == 0; (a1 = (uint64_t)(a1 << 4)); if (c) goto L_1140f8; }
    (v0 = (uint64_t)((int64_t)(int64_t)v0 + (int64_t)(int32_t)8));
    (v0 = (uint64_t)(v0 << (32 + 0)));
    goto L_1140fc;
L_1140f8:;
    (v0 = (uint64_t)(a2 << (32 + 4)));
L_1140fc:;
    (v0 = (uint64_t)(a1 | v0));
    { A1_W64B((uint32_t)a0, 32, v0); if (xo) { xo[0] = a1; xo[1] = a2; xo[2] = a3; } return v0; }
    __builtin_unreachable();
}

// sub_001157D8 (0x1157d8): typed by tools/typed_lift2.py (round A3; loop leaf)
inline uint64_t sub_001157D8(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t a0, uint32_t a1)
{
    uint64_t v0 = 0;
    uint32_t v1 = 0;
    uint64_t a2 = 0;
    uint64_t a3 = 0;
    uint64_t t0 = 0;
    uint64_t t1 = 0;
    uint32_t t2 = 0;
    (t0 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL));
    (t1 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL));
    (a3 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL));
    (a2 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL));
    t2 = 8u;
L_1157f0:;
    (v0 = (uint64_t)(uint32_t)((uint8_t)A1_R8B(a0, 0)));
    (v0 = (uint64_t)((int64_t)(int32_t)(SRA32((int32_t)v0, (uint32_t)t0 & 0x1F))));
    (v0 = (uint64_t)(RC_U64X(v0) & (uint64_t)(uint16_t)1));
    { const bool c = (int64_t)v0 == 0; v1 = 1u; if (c) goto L_115850; }
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)a2, 4294967280))));
    (v0 = (uint64_t)(((uint64_t)RC_U64X(v0) < (uint64_t)(int64_t)(int32_t)16) ? 1 : 0));
    A1_W32(a1, v1);
    A1_W32B(a1, 8, (uint32_t)t1);
    { const bool c = (int64_t)v0 != 0; A1_W32B(a1, 12, (uint32_t)a3); if (c) goto L_11582c; }
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)a2, 4294967261))));
    (v0 = (uint64_t)(((uint64_t)RC_U64X(v0) < (uint64_t)(int64_t)(int32_t)4) ? 1 : 0));
    if ((int64_t)v0 == 0) { (a3 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)a3, 1)))); goto L_115838; }
L_11582c:;
    A1_W32B(a1, 4, t2);
    (t1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)t1, 1))));
    goto L_11585c;
L_115838:;
    (v0 = (uint64_t)(a3 & (uint64_t)(uint16_t)7));
    { const bool c = (int64_t)v0 != 0; A1_W32B(a1, 4, v1); if (c) goto L_11585c; }
    (t1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)t1, 1))));
    (a3 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL));
    goto L_11585c;
L_115850:;
    A1_W32(a1, 0u);
    A1_W32B(a1, 8, 0u);
    A1_W32B(a1, 12, 0u);
L_11585c:;
    (t0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)t0, 1))));
    (v0 = (uint64_t)(RC_U64X(t0) & (uint64_t)(uint16_t)7));
    { const bool c = (int64_t)v0 != 0; (a2 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)a2, 1)))); if (c) goto L_115874; }
    a0 = a0 + 1u;
    (t0 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL));
L_115874:;
    (v0 = (uint64_t)(((int64_t)(int64_t)RC_U64X(a2) < (int64_t)(int32_t)40) ? 1 : 0));
    { const bool c = (int64_t)v0 != 0; a1 = a1 + 0x10u; if (c) { A3_BACK(); goto L_1157f0; } }
    { (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 1)))); return v0; }
    __builtin_unreachable();
}

// sub_0011BAE8 (0x11bae8): typed by tools/typed_lift2.py (round A3; loop leaf, extra outputs xo[0]=a2)
inline uint32_t sub_0011BAE8(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint64_t a0, uint32_t a1, uint64_t *xo = nullptr)
{
    uint32_t v0 = 0;
    uint32_t v1 = 0;
    uint64_t a2 = 0;
    a1 = a1 << 24;
    (v1 = (uint32_t)((int32_t)((int8_t)A1_R8B((uint32_t)a0, 0))));
    (v0 = (uint32_t)((uint8_t)A1_R8B((uint32_t)a0, 0)));
    a1 = (uint32_t)((int32_t)a1 >> 24);
    { const bool c = v1 == 0; (a2 = (uint64_t)((uint64_t)a0 + (uint64_t)0ULL)); if (c) goto L_11bb24; }
    if (v1 == a1) goto L_11bb34;
    (a2 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)a2, 1))));
L_11bb10:;
    (v1 = (uint32_t)((int32_t)((int8_t)A1_R8B((uint32_t)a2, 0))));
    { const bool c = v1 == 0; (v0 = (uint32_t)((uint8_t)A1_R8B((uint32_t)a2, 0))); if (c) goto L_11bb24; }
    if (v1 != a1) { (a2 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)a2, 1)))); { A3_BACK(); goto L_11bb10; } }
L_11bb24:;
    v0 = v0 << 24;
    v0 = (uint32_t)((int32_t)v0 >> 24);
    if (v0 != a1) goto L_11bb3c;
L_11bb34:;
    { (v0 = (uint32_t)((int32_t)((int32_t)SUB32((uint32_t)a2, (uint32_t)a0)))); if (xo) { xo[0] = a2; } return v0; }
L_11bb3c:;
    { v0 = 0xFFFFFFFFu; if (xo) { xo[0] = a2; } return v0; }
    __builtin_unreachable();
}

// sub_0011BBB8 (0x11bbb8): typed by tools/typed_lift2.py (round A3; loop leaf, extra outputs xo[0]=a2)
inline uint32_t sub_0011BBB8(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t a0, uint64_t *xo = nullptr)
{
    uint32_t v0 = 0;
    uint32_t v1 = 0;
    uint32_t a1 = 0;
    uint32_t a2 = 0;
    bool a3xw0 = false;
    v0 = (uint32_t)(int8_t)A1_R8(a0);
    { const bool c = v0 == 0; v1 = A1_R8(a0); if (c) goto L_11bc04; }
    a2 = 0x3Fu; a3xw0 = true;
    a1 = 0x2Au;
L_11bbd0:;
    v0 = v1 << 24;
    v0 = (uint32_t)((int32_t)v0 >> 24);
    if (v0 == a2) goto L_11bbf0;
    { const bool c = v0 == a1; v0 = (int32_t)v0 < 32 ? 1u : 0u; if (c) goto L_11bbf0; }
    { const bool c = v0 == 0; a0 = a0 + 1u; if (c) goto L_11bbf8; }
L_11bbf0:;
    { v0 = 0u; if (xo) { xo[0] = a1_sx64(a2); } return v0; }
L_11bbf8:;
    v0 = (uint32_t)(int8_t)A1_R8(a0);
    { const bool c = v0 != 0; v1 = A1_R8(a0); if (c) { A3_BACK(); goto L_11bbd0; } }
L_11bc04:;
    { v0 = 1u; if (xo) { if (a3xw0) xo[0] = a1_sx64(a2); } return v0; }
    __builtin_unreachable();
}

// sub_0011BF68 (0x11bf68): typed by tools/typed_lift2.py (round A3; loop leaf)
inline uint64_t sub_0011BF68(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint64_t a0)
{
    uint64_t v0 = 0;
    uint32_t v1 = 0;
    uint64_t a1 = 0;
    uint64_t a2 = 0;
    (a1 = (uint64_t)((uint64_t)a0 + (uint64_t)0ULL));
    (a2 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL));
    (v0 = (uint64_t)((int64_t)(int32_t)((int8_t)A1_R8B((uint32_t)a1, 0))));
    { const bool c = (int64_t)v0 == 0; (v1 = (uint32_t)((uint8_t)A1_R8B((uint32_t)a1, 0))); if (c) goto L_11bfa4; }
L_11bf80:;
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)SLL32(v1, 24))));
    (a1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)a1, 1))));
    (v0 = (uint64_t)((int64_t)(int32_t)(SRA32((int32_t)v0, 24))));
    (v1 = (uint32_t)((int32_t)((int32_t)ADD32((uint32_t)a2, 1))));
    (v0 = (uint64_t)(RC_U64X(v0) ^ (uint64_t)(uint16_t)47));
    (a0 = (uint64_t)((int64_t)(int32_t)((int8_t)A1_R8B((uint32_t)a1, 0))));
    if (RC_U64X(v0) == 0) a2 = (uint64_t)(a1_sx64(v1));;
    { const bool c = (int64_t)a0 != 0; (v1 = (uint32_t)((uint8_t)A1_R8B((uint32_t)a1, 0))); if (c) { A3_BACK(); goto L_11bf80; } }
L_11bfa4:;
    { (v0 = (uint64_t)((uint64_t)a2 + (uint64_t)0ULL)); return v0; }
    __builtin_unreachable();
}

// _sceMcFatInit (0x11c138): typed by tools/typed_lift2.py (round A3; loop leaf, extra outputs xo[0]=a2 xo[1]=a3 xo[2]=t0 xo[3]=t2)
inline uint32_t _sceMcFatInit(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint64_t *xo = nullptr)
{
    uint32_t v0 = 0;
    uint32_t v1 = 0;
    uint32_t a0 = 0;
    uint32_t a1 = 0;
    uint32_t a2 = 0;
    uint32_t a3 = 0;
    uint32_t t0 = 0;
    uint32_t t2 = 0;
    v0 = 0x160000u;
    a0 = 0xFFFFFFFFu;
    v0 = v0 - 0x20B8u;
    a2 = 1u;
    v0 = v0 + 0x808u;
    a1 = 0x140000u;
    v1 = 1u;
L_11c158:;
    A1_W32(v0 - 8u, a0);
    v1 = v1 - 1u;
    A1_W32(v0 - 4u, a0);
    A1_W32B(v0, 140, 0u);
    A1_W32B(v0, 128, a2);
    A1_W32B(v0, 132, 0u);
    A1_W32B(v0, 136, 0u);
    t2 = a1 - 0x72F8u;
    a3 = (uint32_t)(int8_t)A1_R8(t2);
    t0 = (uint32_t)(int8_t)A1_R8B(t2, 1);
    A1_W8(v0, (uint8_t)a3);
    A1_W8B(v0, 1, (uint8_t)t0);
    { const bool c = (int32_t)v1 >= 0; v0 = v0 + 0x898u; if (c) { A3_BACK(); goto L_11c158; } }
    { v0 = 1u; if (xo) { xo[0] = a1_sx64(a2); xo[1] = a1_sx64(a3); xo[2] = a1_sx64(t0); xo[3] = a1_sx64(t2); } return v0; }
    __builtin_unreachable();
}

// _sceMcFatInit2 (0x11c198): typed by tools/typed_lift2.py (round A3; leaf, extra outputs xo[0]=a1 xo[1]=a2 xo[2]=a3 xo[3]=t0 xo[4]=t1 xo[5]=t2 xo[6]=t3)
inline uint32_t _sceMcFatInit2(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t a0, uint64_t *xo = nullptr)
{
    uint32_t v0 = 0;
    uint32_t v1 = 0;
    uint32_t a1 = 0;
    uint64_t a2 = 0;
    uint64_t a3 = 0;
    uint32_t t0 = 0;
    uint32_t t1 = 0;
    uint32_t t2 = 0;
    uint32_t t3 = 0;
    uint64_t hi = 0;
    uint64_t lo = 0;
    v0 = 0x898u;
    v1 = 0x160000u;
    { int64_t result = (int64_t)(int32_t)a0 * (int64_t)(int32_t)v0; lo = (uint64_t)(int64_t)(int32_t)result; hi = (uint64_t)(int64_t)(int32_t)(result >> 32); (a0 = (uint32_t)((int32_t)((int32_t)result))); }
    v1 = v1 - 0x20B8u;
    t0 = 0xFFFFFFFFu;
    t1 = 1u;
    t2 = v1 + 0x808u;
    t3 = 0x140000u;
    a1 = v1 + a0;
    A1_W32B(a1, 2048, t0);
    (a2 = (uint64_t)((uint64_t)a1_sx64(a1) + (uint64_t)0ULL));
    A1_W32B((uint32_t)a2, 2052, t0);
    (a3 = (uint64_t)((uint64_t)a1_sx64(a1) + (uint64_t)0ULL));
    A1_W32B((uint32_t)a3, 2196, 0u);
    v1 = a1;
    A1_W32B(a1, 2184, t1);
    a0 = a0 + t2;
    A1_W32B((uint32_t)a2, 2188, 0u);
    A1_W32B(v1, 2192, 0u);
    (a3 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(t3, 4294937864))));
    (v1 = (uint32_t)((int32_t)((int8_t)A1_R8B((uint32_t)a3, 0))));
    (a1 = (uint32_t)((int32_t)((int8_t)A1_R8B((uint32_t)a3, 1))));
    A1_W8(a0, (uint8_t)v1);
    A1_W8B(a0, 1, (uint8_t)a1);
    { v0 = 1u; if (xo) { xo[0] = a1_sx64(a1); xo[1] = a2; xo[2] = a3; xo[3] = a1_sx64(t0); xo[4] = a1_sx64(t1); xo[5] = a1_sx64(t2); xo[6] = a1_sx64(t3); } return v0; }
    __builtin_unreachable();
}

// _sceMcpInit (0x120438): typed by tools/typed_lift2.py (round A3; loop leaf)
inline uint32_t _sceMcpInit(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt)
{
    uint32_t v0 = 0;
    uint32_t v1 = 0;
    uint32_t a0 = 0;
    uint32_t a1 = 0;
    v0 = 0x160000u;
    a1 = 0xFFFFFFFFu;
    v1 = v0 - 0xB80u;
    a0 = v1 + 0x4020u;
L_120448:;
    A1_W32(v1, a1);
    A1_W32B(v1, 12, 0u);
    v1 = v1 + 0x2010u;
    v0 = (int32_t)v1 < (int32_t)a0 ? 1u : 0u;
    if (v0 != 0) { A3_BACK(); goto L_120448; }
    { v0 = 1u; return v0; }
    __builtin_unreachable();
}

// _sceMcpIsSpeckOutBlock (0x120d10): typed by tools/typed_lift2.py (round A3; loop leaf)
inline uint32_t _sceMcpIsSpeckOutBlock(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t a0, uint64_t a1)
{
    uint32_t v0 = 0;
    uint32_t v1 = 0;
    uint64_t hi = 0;
    uint64_t lo = 0;
    v1 = 0x184u;
    v0 = 0x130000u;
    { int64_t result = (int64_t)(int32_t)a0 * (int64_t)(int32_t)v1; lo = (uint64_t)(int64_t)(int32_t)result; hi = (uint64_t)(int64_t)(int32_t)(result >> 32); (a0 = (uint32_t)((int32_t)((int32_t)result))); }
    v0 = v0 + 0x5638u;
    v1 = 0u;
    a0 = a0 + v0;
    a0 = a0 + 0xD0u;
L_120d30:;
    v0 = A1_R32(a0);
    { const bool c = (int64_t)(int32_t)v0 != (int64_t)a1; v1 = v1 + 1u; if (c) goto L_120d44; }
    { v0 = 1u; return v0; }
L_120d44:;
    v0 = (int32_t)v1 < 32 ? 1u : 0u;
    { const bool c = v0 != 0; a0 = a0 + 4u; if (c) { A3_BACK(); goto L_120d30; } }
    { v0 = 0u; return v0; }
    __builtin_unreachable();
}

// sub_00121EC0 (0x121ec0): typed by tools/typed_lift2.py (round A3; loop leaf)
inline uint64_t sub_00121EC0(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt)
{
    uint64_t v0 = 0;
    uint64_t v1 = 0;
    uint32_t a0 = 0;
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)22 << 16))));
    (v1 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL));
    (a0 = (uint32_t)((int32_t)((int32_t)ADD32((uint32_t)v0, 15560))));
L_121ed0:;
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B(a0, 0))));
    if ((int64_t)v0 != 0) { (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v1, 1)))); goto L_121ee4; }
    { (v0 = (uint64_t)((uint64_t)v1 + (uint64_t)0ULL)); return v0; }
L_121ee4:;
    (v0 = (uint64_t)(((int64_t)(int64_t)v1 < (int64_t)(int32_t)2) ? 1 : 0));
    { const bool c = (int64_t)v0 != 0; a0 = a0 + 0x24u; if (c) { A3_BACK(); goto L_121ed0; } }
    { (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 4294967295)))); return v0; }
    __builtin_unreachable();
}

// sceNetcnfifGetResult (0x122378): typed by tools/typed_lift2.py (round A3; loop leaf, extra outputs xo[0]=a1 xo[1]=a2 xo[2]=a3 xo[3]=t0)
inline uint64_t sceNetcnfifGetResult(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint64_t a0, uint64_t *xo = nullptr)
{
    uint64_t v0 = 0;
    uint64_t v1 = 0;
    uint64_t a1 = 0;
    uint64_t a2 = 0;
    uint64_t a3 = 0;
    uint64_t t0 = 0;
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)22 << 16))));
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v1, 32512))));
    (v0 = (uint64_t)(a0 | RC_U64X(v1)));
    (v0 = (uint64_t)(v0 & (uint64_t)(uint16_t)7));
    { const bool c = (int64_t)v0 == 0; (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v1, 768)))); if (c) goto L_1223ec; }
L_122390:;
    { uint32_t addr = ADD32((uint32_t)v1, 7); uint32_t aligned_addr = addr & ~7u; uint32_t offset = addr & 7u; uint64_t mem = A1_R64(aligned_addr); uint32_t shift = (7u - offset) << 3; uint64_t keepMask = (shift == 0) ? 0ull : ((1ull << shift) - 1ull); (a1 = (uint64_t)((a1 & keepMask) | (mem << shift))); }
    { uint32_t addr = ADD32((uint32_t)v1, 0); uint32_t aligned_addr = addr & ~7u; uint32_t offset = addr & 7u; uint64_t mem = A1_R64(aligned_addr); uint32_t shift = offset << 3; uint64_t keepMask = (offset == 0) ? 0ull : (0xFFFFFFFFFFFFFFFFull << ((8u - offset) << 3)); (a1 = (uint64_t)((a1 & keepMask) | (mem >> shift))); }
    { uint32_t addr = ADD32((uint32_t)v1, 15); uint32_t aligned_addr = addr & ~7u; uint32_t offset = addr & 7u; uint64_t mem = A1_R64(aligned_addr); uint32_t shift = (7u - offset) << 3; uint64_t keepMask = (shift == 0) ? 0ull : ((1ull << shift) - 1ull); (a2 = (uint64_t)((a2 & keepMask) | (mem << shift))); }
    { uint32_t addr = ADD32((uint32_t)v1, 8); uint32_t aligned_addr = addr & ~7u; uint32_t offset = addr & 7u; uint64_t mem = A1_R64(aligned_addr); uint32_t shift = offset << 3; uint64_t keepMask = (offset == 0) ? 0ull : (0xFFFFFFFFFFFFFFFFull << ((8u - offset) << 3)); (a2 = (uint64_t)((a2 & keepMask) | (mem >> shift))); }
    { uint32_t addr = ADD32((uint32_t)v1, 23); uint32_t aligned_addr = addr & ~7u; uint32_t offset = addr & 7u; uint64_t mem = A1_R64(aligned_addr); uint32_t shift = (7u - offset) << 3; uint64_t keepMask = (shift == 0) ? 0ull : ((1ull << shift) - 1ull); (a3 = (uint64_t)((a3 & keepMask) | (mem << shift))); }
    { uint32_t addr = ADD32((uint32_t)v1, 16); uint32_t aligned_addr = addr & ~7u; uint32_t offset = addr & 7u; uint64_t mem = A1_R64(aligned_addr); uint32_t shift = offset << 3; uint64_t keepMask = (offset == 0) ? 0ull : (0xFFFFFFFFFFFFFFFFull << ((8u - offset) << 3)); (a3 = (uint64_t)((a3 & keepMask) | (mem >> shift))); }
    { uint32_t addr = ADD32((uint32_t)v1, 31); uint32_t aligned_addr = addr & ~7u; uint32_t offset = addr & 7u; uint64_t mem = A1_R64(aligned_addr); uint32_t shift = (7u - offset) << 3; uint64_t keepMask = (shift == 0) ? 0ull : ((1ull << shift) - 1ull); (t0 = (uint64_t)((t0 & keepMask) | (mem << shift))); }
    { uint32_t addr = ADD32((uint32_t)v1, 24); uint32_t aligned_addr = addr & ~7u; uint32_t offset = addr & 7u; uint64_t mem = A1_R64(aligned_addr); uint32_t shift = offset << 3; uint64_t keepMask = (offset == 0) ? 0ull : (0xFFFFFFFFFFFFFFFFull << ((8u - offset) << 3)); (t0 = (uint64_t)((t0 & keepMask) | (mem >> shift))); }
    { uint32_t addr = ADD32((uint32_t)a0, 7); uint32_t aligned_addr = addr & ~7u; uint32_t offset = addr & 7u; uint32_t shift = (7u - offset) << 3; uint64_t mask = 0xFFFFFFFFFFFFFFFFull >> shift; uint64_t old_data = A1_R64(aligned_addr); uint64_t val = a1; uint64_t new_data = (old_data & ~mask) | ((val >> shift) & mask); A1_W64(aligned_addr, new_data); }
    { uint32_t addr = ADD32((uint32_t)a0, 0); uint32_t aligned_addr = addr & ~7u; uint32_t offset = addr & 7u; uint32_t shift = offset << 3; uint64_t mask = 0xFFFFFFFFFFFFFFFFull << shift; uint64_t old_data = A1_R64(aligned_addr); uint64_t val = a1; uint64_t new_data = (old_data & ~mask) | ((val << shift) & mask); A1_W64(aligned_addr, new_data); }
    { uint32_t addr = ADD32((uint32_t)a0, 15); uint32_t aligned_addr = addr & ~7u; uint32_t offset = addr & 7u; uint32_t shift = (7u - offset) << 3; uint64_t mask = 0xFFFFFFFFFFFFFFFFull >> shift; uint64_t old_data = A1_R64(aligned_addr); uint64_t val = a2; uint64_t new_data = (old_data & ~mask) | ((val >> shift) & mask); A1_W64(aligned_addr, new_data); }
    { uint32_t addr = ADD32((uint32_t)a0, 8); uint32_t aligned_addr = addr & ~7u; uint32_t offset = addr & 7u; uint32_t shift = offset << 3; uint64_t mask = 0xFFFFFFFFFFFFFFFFull << shift; uint64_t old_data = A1_R64(aligned_addr); uint64_t val = a2; uint64_t new_data = (old_data & ~mask) | ((val << shift) & mask); A1_W64(aligned_addr, new_data); }
    { uint32_t addr = ADD32((uint32_t)a0, 23); uint32_t aligned_addr = addr & ~7u; uint32_t offset = addr & 7u; uint32_t shift = (7u - offset) << 3; uint64_t mask = 0xFFFFFFFFFFFFFFFFull >> shift; uint64_t old_data = A1_R64(aligned_addr); uint64_t val = a3; uint64_t new_data = (old_data & ~mask) | ((val >> shift) & mask); A1_W64(aligned_addr, new_data); }
    { uint32_t addr = ADD32((uint32_t)a0, 16); uint32_t aligned_addr = addr & ~7u; uint32_t offset = addr & 7u; uint32_t shift = offset << 3; uint64_t mask = 0xFFFFFFFFFFFFFFFFull << shift; uint64_t old_data = A1_R64(aligned_addr); uint64_t val = a3; uint64_t new_data = (old_data & ~mask) | ((val << shift) & mask); A1_W64(aligned_addr, new_data); }
    { uint32_t addr = ADD32((uint32_t)a0, 31); uint32_t aligned_addr = addr & ~7u; uint32_t offset = addr & 7u; uint32_t shift = (7u - offset) << 3; uint64_t mask = 0xFFFFFFFFFFFFFFFFull >> shift; uint64_t old_data = A1_R64(aligned_addr); uint64_t val = t0; uint64_t new_data = (old_data & ~mask) | ((val >> shift) & mask); A1_W64(aligned_addr, new_data); }
    { uint32_t addr = ADD32((uint32_t)a0, 24); uint32_t aligned_addr = addr & ~7u; uint32_t offset = addr & 7u; uint32_t shift = offset << 3; uint64_t mask = 0xFFFFFFFFFFFFFFFFull << shift; uint64_t old_data = A1_R64(aligned_addr); uint64_t val = t0; uint64_t new_data = (old_data & ~mask) | ((val << shift) & mask); A1_W64(aligned_addr, new_data); }
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v1, 32))));
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)a0, 32))));
    if ((int64_t)v1 != (int64_t)v0) { A3_BACK(); goto L_122390; }
    goto L_122420;
L_1223ec:;
    (a1 = (uint64_t)(A1_R64B((uint32_t)v1, 0)));
    (a2 = (uint64_t)(A1_R64B((uint32_t)v1, 8)));
    (a3 = (uint64_t)(A1_R64B((uint32_t)v1, 16)));
    (t0 = (uint64_t)(A1_R64B((uint32_t)v1, 24)));
    A1_W64B((uint32_t)a0, 0, a1);
    A1_W64B((uint32_t)a0, 8, a2);
    A1_W64B((uint32_t)a0, 16, a3);
    A1_W64B((uint32_t)a0, 24, t0);
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v1, 32))));
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)a0, 32))));
    if ((int64_t)v1 != (int64_t)v0) { A3_BACK(); goto L_1223ec; }
L_122420:;
    { uint32_t addr = ADD32((uint32_t)v1, 7); uint32_t aligned_addr = addr & ~7u; uint32_t offset = addr & 7u; uint64_t mem = A1_R64(aligned_addr); uint32_t shift = (7u - offset) << 3; uint64_t keepMask = (shift == 0) ? 0ull : ((1ull << shift) - 1ull); (a1 = (uint64_t)((a1 & keepMask) | (mem << shift))); }
    { uint32_t addr = ADD32((uint32_t)v1, 0); uint32_t aligned_addr = addr & ~7u; uint32_t offset = addr & 7u; uint64_t mem = A1_R64(aligned_addr); uint32_t shift = offset << 3; uint64_t keepMask = (offset == 0) ? 0ull : (0xFFFFFFFFFFFFFFFFull << ((8u - offset) << 3)); (a1 = (uint64_t)((a1 & keepMask) | (mem >> shift))); }
    { uint32_t addr = ADD32((uint32_t)v1, 15); uint32_t aligned_addr = addr & ~7u; uint32_t offset = addr & 7u; uint64_t mem = A1_R64(aligned_addr); uint32_t shift = (7u - offset) << 3; uint64_t keepMask = (shift == 0) ? 0ull : ((1ull << shift) - 1ull); (a2 = (uint64_t)((a2 & keepMask) | (mem << shift))); }
    { uint32_t addr = ADD32((uint32_t)v1, 8); uint32_t aligned_addr = addr & ~7u; uint32_t offset = addr & 7u; uint64_t mem = A1_R64(aligned_addr); uint32_t shift = offset << 3; uint64_t keepMask = (offset == 0) ? 0ull : (0xFFFFFFFFFFFFFFFFull << ((8u - offset) << 3)); (a2 = (uint64_t)((a2 & keepMask) | (mem >> shift))); }
    { uint32_t addr = ADD32((uint32_t)a0, 7); uint32_t aligned_addr = addr & ~7u; uint32_t offset = addr & 7u; uint32_t shift = (7u - offset) << 3; uint64_t mask = 0xFFFFFFFFFFFFFFFFull >> shift; uint64_t old_data = A1_R64(aligned_addr); uint64_t val = a1; uint64_t new_data = (old_data & ~mask) | ((val >> shift) & mask); A1_W64(aligned_addr, new_data); }
    { uint32_t addr = ADD32((uint32_t)a0, 0); uint32_t aligned_addr = addr & ~7u; uint32_t offset = addr & 7u; uint32_t shift = offset << 3; uint64_t mask = 0xFFFFFFFFFFFFFFFFull << shift; uint64_t old_data = A1_R64(aligned_addr); uint64_t val = a1; uint64_t new_data = (old_data & ~mask) | ((val << shift) & mask); A1_W64(aligned_addr, new_data); }
    { uint32_t addr = ADD32((uint32_t)a0, 15); uint32_t aligned_addr = addr & ~7u; uint32_t offset = addr & 7u; uint32_t shift = (7u - offset) << 3; uint64_t mask = 0xFFFFFFFFFFFFFFFFull >> shift; uint64_t old_data = A1_R64(aligned_addr); uint64_t val = a2; uint64_t new_data = (old_data & ~mask) | ((val >> shift) & mask); A1_W64(aligned_addr, new_data); }
    (v0 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL));
    { { uint32_t addr = ADD32((uint32_t)a0, 8); uint32_t aligned_addr = addr & ~7u; uint32_t offset = addr & 7u; uint32_t shift = offset << 3; uint64_t mask = 0xFFFFFFFFFFFFFFFFull << shift; uint64_t old_data = A1_R64(aligned_addr); uint64_t val = a2; uint64_t new_data = (old_data & ~mask) | ((val << shift) & mask); A1_W64(aligned_addr, new_data); } if (xo) { xo[0] = a1; xo[1] = a2; xo[2] = a3; xo[3] = t0; } return v0; }
    __builtin_unreachable();
}

// sub_00125080 (0x125080): typed by tools/typed_lift2.py (round A3; loop leaf)
inline uint64_t sub_00125080(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint64_t a0, uint64_t a1, uint64_t a2)
{
    uint64_t v0 = 0;
    uint64_t v1 = 0;
    uint64_t a3 = 0;
    uint64_t t0 = 0;
    uint64_t t1 = 0;
    uint64_t t2 = 0;
    uint64_t t3 = 0;
    uint64_t t4 = 0;
    (t3 = (uint64_t)((uint64_t)a0 + (uint64_t)0ULL));
    (a3 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)t3, 0))));
    (v1 = (uint64_t)(((uint64_t)RC_U64X(a3) < (uint64_t)(int64_t)(int32_t)2) ? 1 : 0));
    { const bool c = (int64_t)v1 != 0; (v0 = (uint64_t)((uint64_t)t3 + (uint64_t)0ULL)); if (c) goto L_1252d4; }
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)a1, 0))));
    (v1 = (uint64_t)(((uint64_t)RC_U64X(a0) < (uint64_t)(int64_t)(int32_t)2) ? 1 : 0));
    { const bool c = (int64_t)v1 != 0; (v0 = (uint64_t)((uint64_t)a1 + (uint64_t)0ULL)); if (c) goto L_1252d4; }
    (v0 = (uint64_t)(RC_U64X(a3) ^ (uint64_t)(uint16_t)4));
    { const bool c = (int64_t)v0 != 0; (v1 = (uint64_t)(RC_U64X(a0) ^ (uint64_t)(uint16_t)4)); if (c) goto L_1250d8; }
    (v0 = (uint64_t)(RC_U64X(a0) ^ (uint64_t)(uint16_t)4));
    if ((int64_t)v0 != 0) goto L_125128;
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)a1, 4))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)t3, 4))));
    if ((int64_t)v0 == (int64_t)v1) goto L_125128;
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)20 << 16))));
    { (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v0, 4294939192)))); return v0; }
L_1250d8:;
    { const bool c = (int64_t)v1 == 0; (v0 = (uint64_t)((uint64_t)a1 + (uint64_t)0ULL)); if (c) goto L_1252d4; }
    (v0 = (uint64_t)(a0 ^ (uint64_t)(uint16_t)2));
    { const bool c = (int64_t)v0 != 0; (v1 = (uint64_t)(a3 ^ (uint64_t)(uint16_t)2)); if (c) goto L_125130; }
    (v0 = (uint64_t)(a3 ^ (uint64_t)(uint16_t)2));
    if ((int64_t)v0 != 0) goto L_125128;
    (a0 = (uint64_t)(A1_R64B((uint32_t)t3, 0)));
    (v0 = (uint64_t)((uint64_t)a2 + (uint64_t)0ULL));
    A1_W64B((uint32_t)a2, 0, a0);
    (v1 = (uint64_t)(A1_R64B((uint32_t)t3, 8)));
    A1_W64B((uint32_t)a2, 8, v1);
    (a0 = (uint64_t)(A1_R64B((uint32_t)t3, 16)));
    A1_W64B((uint32_t)a2, 16, a0);
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)t3, 4))));
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)a1, 4))));
    (v1 = (uint64_t)(RC_U64X(v1) & RC_U64X(a0)));
    { A1_W32B((uint32_t)a2, 4, (uint32_t)v1); return v0; }
L_125128:;
    { (v0 = (uint64_t)((uint64_t)t3 + (uint64_t)0ULL)); return v0; }
L_125130:;
    { const bool c = (int64_t)v1 == 0; (v0 = (uint64_t)((uint64_t)a1 + (uint64_t)0ULL)); if (c) goto L_1252d4; }
    (t0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)t3, 8))));
    (a3 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)a1, 8))));
    (t2 = (uint64_t)(A1_R64B((uint32_t)t3, 16)));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)SUB32((uint32_t)t0, (uint32_t)a3))));
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)SUB32(0u, (uint32_t)v0))));
    (v1 = (uint64_t)(((int64_t)(int64_t)RC_U64X(v0) < (int64_t)(int32_t)0) ? 1 : 0));
    if (RC_U64X(v1) != 0) v0 = (uint64_t)(a0);;
    (v0 = (uint64_t)(((int64_t)(int64_t)v0 < (int64_t)(int32_t)64) ? 1 : 0));
    { const bool c = (int64_t)v0 == 0; (t1 = (uint64_t)(A1_R64B((uint32_t)a1, 16))); if (c) goto L_1251d0; }
    (v0 = (uint64_t)(((int64_t)(int64_t)RC_U64X(a3) < (int64_t)(int64_t)RC_U64X(t0)) ? 1 : 0));
    { const bool c = (int64_t)v0 == 0; (v0 = (uint64_t)(((int64_t)(int64_t)RC_U64X(t0) < (int64_t)(int64_t)RC_U64X(a3)) ? 1 : 0)); if (c) goto L_12519c; }
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 1))));
    (a3 = (uint64_t)((int64_t)(int32_t)((int32_t)SUB32((uint32_t)t0, (uint32_t)a3))));
L_125178:;
    (v1 = (uint64_t)(t1 >> 1));
    (v0 = (uint64_t)(t1 & a0));
    (a3 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)a3, 4294967295))));
    { const bool c = (int64_t)a3 != 0; (t1 = (uint64_t)(v0 | v1)); if (c) { A3_BACK(); goto L_125178; } }
    (a3 = (uint64_t)((uint64_t)t0 + (uint64_t)0ULL));
    (v0 = (uint64_t)(((int64_t)(int64_t)t0 < (int64_t)(int64_t)a3) ? 1 : 0));
L_12519c:;
    if ((int64_t)v0 == 0) { (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)t3, 4)))); goto L_1251f0; }
    (t4 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 1))));
L_1251a8:;
    (t0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)t0, 1))));
    (v0 = (uint64_t)(t2 >> 1));
    (v1 = (uint64_t)(t2 & t4));
    (a0 = (uint64_t)(((int64_t)(int64_t)RC_U64X(t0) < (int64_t)(int64_t)a3) ? 1 : 0));
    { const bool c = (int64_t)a0 != 0; (t2 = (uint64_t)(v1 | v0)); if (c) { A3_BACK(); goto L_1251a8; } }
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)t3, 4))));
    goto L_1251f0;
L_1251d0:;
    (v0 = (uint64_t)(((int64_t)(int64_t)a3 < (int64_t)(int64_t)t0) ? 1 : 0));
    if ((int64_t)v0 == 0) { (t0 = (uint64_t)((uint64_t)a3 + (uint64_t)0ULL)); goto L_1251e8; }
    (t1 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL));
    goto L_1251ec;
L_1251e8:;
    (t2 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL));
L_1251ec:;
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)t3, 4))));
L_1251f0:;
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)a1, 4))));
    { const bool c = (int64_t)a0 == (int64_t)v0; (v0 = (uint64_t)(t2 - t1)); if (c) goto L_125288; }
    (v1 = (uint64_t)(t1 - t2));
    if (a0 == 0) v1 = (uint64_t)(v0);;
    if ((int32_t)v1 < 0) { (v1 = (uint64_t)(0ULL - v1)); goto L_125220; }
    A1_W32B((uint32_t)a2, 8, (uint32_t)t0);
    A1_W64B((uint32_t)a2, 16, v1);
    A1_W32B((uint32_t)a2, 4, 0u);
    goto L_125230;
L_125220:;
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 1))));
    A1_W32B((uint32_t)a2, 4, (uint32_t)v0);
    A1_W32B((uint32_t)a2, 8, (uint32_t)t0);
    A1_W64B((uint32_t)a2, 16, v1);
L_125230:;
    (a3 = (uint64_t)(A1_R64B((uint32_t)a2, 16)));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 4294967295))));
    (v0 = (uint64_t)(RC_U64X(v0) << 5));
    (v0 = (uint64_t)(v0 >> 4));
    (v1 = (uint64_t)((int64_t)(int64_t)a3 + (int64_t)(int32_t)4294967295));
    (v0 = (uint64_t)(((uint64_t)v0 < (uint64_t)v1) ? 1 : 0));
    if ((int64_t)v0 != 0) goto L_12529c;
L_125250:;
    (a1 = (uint64_t)(a3 << 1));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)a2, 8))));
    (v1 = (uint64_t)((int64_t)(int64_t)a1 + (int64_t)(int32_t)4294967295));
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 4294967295))));
    (a0 = (uint64_t)(RC_U64X(a0) << 5));
    (a0 = (uint64_t)(a0 >> 4));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v0, 4294967295))));
    (a0 = (uint64_t)(((uint64_t)a0 < (uint64_t)v1) ? 1 : 0));
    A1_W32B((uint32_t)a2, 8, (uint32_t)v0);
    (a3 = (uint64_t)((uint64_t)a1 + (uint64_t)0ULL));
    { const bool c = (int64_t)a0 == 0; A1_W64B((uint32_t)a2, 16, a1); if (c) { A3_BACK(); goto L_125250; } }
    goto L_12529c;
L_125288:;
    (v0 = (uint64_t)((uint64_t)t2 + (uint64_t)t1));
    A1_W32B((uint32_t)a2, 4, (uint32_t)a0);
    A1_W32B((uint32_t)a2, 8, (uint32_t)t0);
    (a3 = (uint64_t)((uint64_t)v0 + (uint64_t)0ULL));
    A1_W64B((uint32_t)a2, 16, v0);
L_12529c:;
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 4294967295))));
    (v0 = (uint64_t)(RC_U64X(v0) >> 3));
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 3))));
    (v0 = (uint64_t)(((uint64_t)v0 < (uint64_t)a3) ? 1 : 0));
    { const bool c = (int64_t)v0 == 0; A1_W32B((uint32_t)a2, 0, (uint32_t)v1); if (c) goto L_1252d0; }
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)a2, 8))));
    (a0 = (uint64_t)(a3 >> 1));
    (v1 = (uint64_t)(a3 & (uint64_t)(uint16_t)1));
    (v1 = (uint64_t)(RC_U64X(v1) | a0));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v0, 1))));
    A1_W64B((uint32_t)a2, 16, v1);
    A1_W32B((uint32_t)a2, 8, (uint32_t)v0);
L_1252d0:;
    (v0 = (uint64_t)((uint64_t)a2 + (uint64_t)0ULL));
L_1252d4:;
    { return v0; }
    __builtin_unreachable();
}

// sub_00125998 (0x125998): typed by tools/typed_lift2.py (round A3; loop leaf, extra outputs xo[0]=a2 xo[1]=a3)
inline uint32_t sub_00125998(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t a0, uint32_t a1, uint64_t *xo = nullptr)
{
    uint32_t v0 = 0;
    uint32_t v1 = 0;
    uint32_t a2 = 0;
    uint64_t a3 = 0;
    v1 = A1_R8(a0);
    a0 = a0 + 1u;
    (a3 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL));
    v0 = v1 & 0x80u;
    { const bool c = v0 == 0; a2 = v1 & 0x7Fu; if (c) goto L_1259d0; }
L_1259b0:;
    v1 = A1_R8(a0);
    a0 = a0 + 1u;
    (a3 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)a3, 7))));
    v0 = v1 & 0x7Fu;
    v1 = v1 & 0x80u;
    (v0 = (uint32_t)((int32_t)((int32_t)SLL32(v0, (uint32_t)a3 & 0x1F))));
    { const bool c = v1 != 0; a2 = a2 | v0; if (c) { A3_BACK(); goto L_1259b0; } }
L_1259d0:;
    A1_W32(a1, a2);
    { v0 = a0; if (xo) { xo[0] = a1_sx64(a2); xo[1] = a3; } return v0; }
    __builtin_unreachable();
}

// sub_001259E0 (0x1259e0): typed by tools/typed_lift2.py (round A3; loop leaf)
inline uint32_t sub_001259E0(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t a0, uint32_t a1)
{
    uint32_t v0 = 0;
    uint32_t v1 = 0;
    uint32_t a2 = 0;
    uint64_t a3 = 0;
    uint64_t t0 = 0;
    (a3 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL));
    (t0 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL));
L_1259e8:;
    a2 = A1_R8(a0);
    a0 = a0 + 1u;
    v0 = a2 & 0x7Fu;
    v1 = a2 & 0x80u;
    (v0 = (uint32_t)((int32_t)((int32_t)SLL32(v0, (uint32_t)a3 & 0x1F))));
    (a3 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)a3, 7))));
    { const bool c = v1 != 0; (t0 = (uint64_t)(t0 | a1_sx64(v0))); if (c) { A3_BACK(); goto L_1259e8; } }
    (v0 = (uint32_t)(((uint64_t)RC_U64X(a3) < (uint64_t)(int64_t)(int32_t)32) ? 1 : 0));
    if (v0 == 0) { A1_W32B(a1, 0, (uint32_t)t0); goto L_125a30; }
    v0 = a2 & 0x40u;
    if (v0 == 0) { A1_W32B(a1, 0, (uint32_t)t0); goto L_125a30; }
    v0 = 0xFFFFFFFFu;
    (v0 = (uint32_t)((int32_t)((int32_t)SLL32(v0, (uint32_t)a3 & 0x1F))));
    (t0 = (uint64_t)(t0 | a1_sx64(v0)));
    A1_W32B(a1, 0, (uint32_t)t0);
L_125a30:;
    { v0 = a0; return v0; }
    __builtin_unreachable();
}

// sub_00125A38 (0x125a38): typed by tools/typed_lift2.py (round A3; loop frame leaf, extra outputs xo[0]=a1 xo[1]=a2 xo[2]=a3 xo[3]=t0 xo[4]=t1 xo[5]=t2 xo[6]=t3, frame 16 bytes, 0 slot accesses)
inline uint64_t sub_00125A38(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint64_t a0, uint64_t a1, uint64_t *xo = nullptr)
{
    uint64_t v0 = 0;
    uint32_t v1 = 0;
    uint64_t a2 = 0;
    uint32_t a3 = 0;
    uint32_t t0 = 0;
    uint32_t t1 = 0;
    uint64_t t2 = 0;
    uint32_t t3 = 0;
    uint64_t t4 = 0;
    uint64_t t5 = 0;
    uint32_t t6 = 0;
    uint32_t t7 = 0;
    uint32_t s0 = 0;
    uint64_t s1 = 0;
    uint32_t t8 = 0;
    uint32_t t9 = 0;
    bool a3xw0 = false;
    bool a3xw1 = false;
    bool a3xw2 = false;
    bool a3xw3 = false;
    bool a3xw4 = false;
    bool a3xw5 = false;
    bool a3xw6 = false;
    (v0 = (uint64_t)((uint64_t)a1 + (uint64_t)0ULL));
    (s1 = (uint64_t)((uint64_t)a0 + (uint64_t)0ULL));
    (t9 = (uint32_t)((int32_t)((int32_t)A1_R32B((uint32_t)v0, 4))));
    { const bool c = t9 == 0; (t4 = (uint64_t)((uint64_t)a1_sx64(t9) + (uint64_t)0ULL)); if (c) goto L_125b20; }
    (s0 = (uint32_t)((int32_t)((int32_t)A1_R32B((uint32_t)s1, 4))));
    (a1 = (uint64_t)((int64_t)(int32_t)((int32_t)SLL32(t9, 2)))); a3xw0 = true;
    (t7 = (uint32_t)((int32_t)((int32_t)A1_R32B((uint32_t)s1, 0))));
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)v0, 0))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)SLL32(s0, 2))));
    (v1 = (uint32_t)((int32_t)((int32_t)ADD32((uint32_t)v0, t7))));
    (t5 = (uint64_t)((uint64_t)RC_U64X(v0) + (uint64_t)0ULL));
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)a1, (uint32_t)a0))));
    t3 = v1 - 4u; a3xw6 = true;
    (t2 = (uint64_t)((uint64_t)a1_sx64(s0) + (uint64_t)0ULL)); a3xw5 = true;
L_125a80:;
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)a0, 4294967292))));
    (t4 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)t4, 4294967295))));
    { const bool c = (int64_t)t2 == 0; (t8 = (uint32_t)((int32_t)((int32_t)A1_R32B((uint32_t)a0, 0)))); if (c) goto L_125b04; }
    v1 = A1_R32(t3);
    t6 = A1_R32B(t8, 8);
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B(v1, 8))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)SUB32((uint32_t)v0, t6))));
    { const bool c = (int32_t)v0 <= 0; (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)t2, (uint32_t)t4)))); if (c) goto L_125b08; }
    (a2 = (uint64_t)((uint64_t)t5 + (uint64_t)0ULL)); a3xw1 = true;
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)SLL32((uint32_t)t4, 2))));
    (v1 = (uint32_t)((int32_t)((int32_t)ADD32((uint32_t)a2, t7))));
    (t0 = (uint32_t)((int32_t)((int32_t)ADD32((uint32_t)a2, (uint32_t)v0)))); a3xw3 = true;
    a3 = v1 - 4u; a3xw2 = true;
    t1 = t7; a3xw4 = true;
    v1 = A1_R32(a3);
L_125ac8:;
    a3 = a3 - 4u; a3xw2 = true;
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(t0, t1))));
    t0 = t0 - 4u; a3xw3 = true;
    (a2 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)a2, 4294967292)))); a3xw1 = true;
    (t2 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)t2, 4294967295)))); a3xw5 = true;
    A1_W32B((uint32_t)v0, 0, v1);
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(t1, (uint32_t)a2))));
    t3 = t3 - 4u; a3xw6 = true;
    { const bool c = (int64_t)t2 == 0; (t5 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)t5, 4294967292)))); if (c) goto L_125b04; }
    (v1 = (uint32_t)((int32_t)((int32_t)A1_R32B((uint32_t)v0, -4))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B(v1, 8))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)SUB32((uint32_t)v0, t6))));
    if ((int32_t)v0 > 0) { v1 = A1_R32(a3); { A3_BACK(); goto L_125ac8; } }
L_125b04:;
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)t2, (uint32_t)t4))));
L_125b08:;
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)SLL32((uint32_t)v0, 2))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v0, t7))));
    { const bool c = (int64_t)t4 != 0; A1_W32B((uint32_t)v0, 0, t8); if (c) { A3_BACK(); goto L_125a80; } }
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(s0, t9))));
    A1_W32B((uint32_t)s1, 4, (uint32_t)v0);
L_125b20:;
    { if (xo) { if (a3xw0) xo[0] = a1; if (a3xw1) xo[1] = a2; if (a3xw2) xo[2] = a1_sx64(a3); if (a3xw3) xo[3] = a1_sx64(t0); if (a3xw4) xo[4] = a1_sx64(t1); if (a3xw5) xo[5] = t2; if (a3xw6) xo[6] = a1_sx64(t3); } return v0; }
    __builtin_unreachable();
}

// sub_001262B8 (0x1262b8): typed by tools/typed_lift2.py (round A3; loop leaf, extra outputs xo[0]=a1 xo[1]=a2)
inline uint64_t sub_001262B8(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t a0, uint64_t *xo = nullptr)
{
    uint64_t v0 = 0;
    uint32_t v1 = 0;
    uint32_t a1 = 0;
    uint64_t a2 = 0;
    a1 = A1_R32(a0);
    { const bool c = a1 == 0; (a2 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL)); if (c) goto L_1262f0; }
L_1262c8:;
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B(a0, 4))));
    { const bool c = (int64_t)v0 == 0; (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)a2, 1)))); if (c) goto L_1262dc; }
    v1 = A1_R32B(a0, 8);
    if (a1_sx64(v1) != 0) a2 = (uint64_t)(v0);;
L_1262dc:;
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(a0, a1))));
    (a0 = (uint32_t)((int32_t)((int32_t)ADD32((uint32_t)v0, 4))));
    v1 = A1_R32(a0);
    { const bool c = v1 != 0; a1 = v1; if (c) { A3_BACK(); goto L_1262c8; } }
L_1262f0:;
    { (v0 = (uint64_t)((uint64_t)a2 + (uint64_t)0ULL)); if (xo) { xo[0] = a1_sx64(a1); xo[1] = a2; } return v0; }
    __builtin_unreachable();
}

// sub_00126388 (0x126388): typed by tools/typed_lift2.py (round A3; loop leaf, extra outputs xo[0]=a2 xo[1]=a3)
inline uint64_t sub_00126388(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t a0, uint32_t a1, uint64_t *xo = nullptr)
{
    uint64_t v0 = 0;
    uint32_t v1 = 0;
    uint32_t a2 = 0;
    uint32_t a3 = 0;
    bool a3xw0 = false;
    a3 = A1_R32(a0);
    { const bool c = a3 == 0; (v0 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL)); if (c) goto L_1263d8; }
L_126398:;
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B(a0, 4))));
    { const bool c = (int64_t)v0 == 0; (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(a0, a3)))); if (c) goto L_1263c4; }
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B(a0, 8))));
    { const bool c = (int64_t)v0 == 0; (a2 = (uint32_t)((int32_t)((int32_t)SUB32(a1, (uint32_t)v0)))); a3xw0 = true; if (c) goto L_1263c0; }
    v1 = A1_R32B(a0, 12);
    v1 = a2 < v1 ? 1u : 0u;
    { const bool c = v1 != 0; (v0 = (uint64_t)((uint64_t)a1_sx64(a0) + (uint64_t)0ULL)); if (c) goto L_1263d8; }
L_1263c0:;
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(a0, a3))));
L_1263c4:;
    (a0 = (uint32_t)((int32_t)((int32_t)ADD32((uint32_t)v0, 4))));
    v1 = A1_R32(a0);
    { const bool c = v1 != 0; a3 = v1; if (c) { A3_BACK(); goto L_126398; } }
    (v0 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL));
L_1263d8:;
    { if (xo) { if (a3xw0) xo[0] = a1_sx64(a2); xo[1] = a1_sx64(a3); } return v0; }
    __builtin_unreachable();
}

// ps2___unpack_d (0x1271b0): typed by tools/typed_lift2.py (round A3; leaf, extra outputs xo[0]=a2 xo[1]=a3)
inline uint64_t ps2___unpack_d(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint64_t a0, uint64_t a1, uint64_t *xo = nullptr)
{
    uint64_t v0 = 0;
    uint64_t v1 = 0;
    uint64_t a2 = 0;
    uint64_t a3 = 0;
    (v0 = (uint64_t)(A1_R64B((uint32_t)a0, 0)));
    (a2 = (uint64_t)((uint64_t)a1 + (uint64_t)0ULL));
    (a1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 4294967295))));
    (a1 = (uint64_t)(RC_U64X(a1) >> 12));
    (v1 = (uint64_t)(v0 >> (32 + 20)));
    (a0 = (uint64_t)(v0 >> (32 + 31)));
    (a3 = (uint64_t)(v1 & (uint64_t)(uint16_t)2047));
    (v1 = (uint64_t)(v0 & a1));
    { const bool c = (int64_t)a3 != 0; A1_W32B((uint32_t)a2, 4, (uint32_t)a0); if (c) goto L_1271e8; }
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 2))));
    { A1_W32B((uint32_t)a2, 0, (uint32_t)v0); if (xo) { xo[0] = a2; xo[1] = a3; } return v0; }
L_1271e8:;
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 2047))));
    if ((int64_t)a3 != (int64_t)v0) { (v1 = (uint64_t)(v1 << 8)); goto L_127248; }
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 1023))));
    { const bool c = (int64_t)v1 != 0; A1_W32B((uint32_t)a2, 8, (uint32_t)v0); if (c) goto L_127218; }
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 4))));
    (v0 = (uint64_t)(0ULL | (uint64_t)(uint16_t)32768));
    (v0 = (uint64_t)(RC_U64X(v0) << (32 + 13)));
    A1_W64B((uint32_t)a2, 16, v0);
    { A1_W32B((uint32_t)a2, 0, (uint32_t)v1); if (xo) { xo[0] = a2; xo[1] = a3; } return v0; }
L_127218:;
    (v0 = (uint64_t)(0ULL | (uint64_t)(uint16_t)32768));
    (v0 = (uint64_t)(RC_U64X(v0) << (32 + 4)));
    (v0 = (uint64_t)(v1 & v0));
    { const bool c = (int64_t)v0 == 0; (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 1)))); if (c) goto L_127238; }
    A1_W32B((uint32_t)a2, 0, (uint32_t)v0);
    goto L_12723c;
L_127238:;
    A1_W32B((uint32_t)a2, 0, 0u);
L_12723c:;
    { A1_W64B((uint32_t)a2, 16, v1); if (xo) { xo[0] = a2; xo[1] = a3; } return v0; }
L_127248:;
    (v0 = (uint64_t)(0ULL | (uint64_t)(uint16_t)32768));
    (v0 = (uint64_t)(RC_U64X(v0) << (32 + 13)));
    (v1 = (uint64_t)(v1 | v0));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 3))));
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)a3, 4294966273))));
    A1_W64B((uint32_t)a2, 16, v1);
    A1_W32B((uint32_t)a2, 8, (uint32_t)a0);
    { A1_W32B((uint32_t)a2, 0, (uint32_t)v0); if (xo) { xo[0] = a2; xo[1] = a3; } return v0; }
    __builtin_unreachable();
}

// ps2__Bfree (0x12e890): typed by tools/typed_lift2.py (round A3; leaf, extra outputs xo[0]=v0)
inline void ps2__Bfree(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t a0, uint64_t a1, uint64_t *xo = nullptr)
{
    uint32_t v0 = 0;
    uint32_t v1 = 0;
    bool a3xw0 = false;
    if ((int64_t)a1 == 0) goto L_12e8b4;
    (v0 = (uint32_t)((int32_t)((int32_t)A1_R32B((uint32_t)a1, 4)))); a3xw0 = true;
    a0 = A1_R32B(a0, 76);
    v0 = v0 << 2; a3xw0 = true;
    v0 = v0 + a0; a3xw0 = true;
    v1 = A1_R32(v0);
    A1_W32B((uint32_t)a1, 0, v1);
    A1_W32B(v0, 0, (uint32_t)a1);
L_12e8b4:;
    { if (xo) { if (a3xw0) xo[0] = a1_sx64(v0); } return; }
    __builtin_unreachable();
}

// ps2___mcmp (0x12f0d8): typed by tools/typed_lift2.py (round A3; loop leaf, extra outputs xo[0]=a2 xo[1]=a3 xo[2]=t0 xo[3]=t1 xo[4]=t2)
inline uint32_t ps2___mcmp(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t a0, uint32_t a1, uint64_t *xo = nullptr)
{
    uint32_t v0 = 0;
    uint32_t v1 = 0;
    uint32_t a2 = 0;
    uint32_t a3 = 0;
    uint32_t t0 = 0;
    uint32_t t1 = 0;
    uint32_t t2 = 0;
    bool a3xw0 = false;
    bool a3xw1 = false;
    bool a3xw2 = false;
    bool a3xw3 = false;
    bool a3xw4 = false;
    v0 = A1_R32B(a0, 16);
    v1 = A1_R32B(a1, 16);
    v0 = v0 - v1;
    if (v0 != 0) goto L_12f138;
    v1 = v1 << 2;
    t2 = a0 + 0x14u; a3xw4 = true;
    v0 = a1 + 0x14u;
    a3 = t2 + v1; a3xw1 = true;
    t1 = v0 + v1; a3xw3 = true;
    a3 = a3 - 4u; a3xw1 = true;
L_12f108:;
    t1 = t1 - 4u; a3xw3 = true;
    a1 = A1_R32(a3);
    a2 = 1u; a3xw0 = true;
    a0 = A1_R32(t1);
    v0 = 0xFFFFFFFFu;
    t0 = t2 < a3 ? 1u : 0u; a3xw2 = true;
    v1 = a1 < a0 ? 1u : 0u;
    { const bool c = a1 != a0; if (a1_sx64(v1) == 0) v0 = (uint32_t)((uint64_t)(a1_sx64(a2)));; if (c) goto L_12f138; }
    if (t0 != 0) { a3 = a3 - 4u; a3xw1 = true; { A3_BACK(); goto L_12f108; } }
    v0 = 0u;
L_12f138:;
    { if (xo) { if (a3xw0) xo[0] = a1_sx64(a2); if (a3xw1) xo[1] = a1_sx64(a3); if (a3xw2) xo[2] = a1_sx64(t0); if (a3xw3) xo[3] = a1_sx64(t1); if (a3xw4) xo[4] = a1_sx64(t2); } return v0; }
    __builtin_unreachable();
}

// _ulp (0x12f2d0): typed by tools/typed_lift2.py (round A3; leaf, extra outputs xo[0]=a2)
inline uint64_t _ulp(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint64_t a0, uint64_t a1, uint64_t *xo = nullptr)
{
    uint64_t v0 = 0;
    uint64_t v1 = 0;
    uint64_t a2 = 0;
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)32752 << 16))));
    (a0 = (uint64_t)((int64_t)a0 >> (32 + 0)));
    (a0 = (uint64_t)(a0 & RC_U64X(v0)));
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)64704 << 16))));
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)a0, (uint32_t)v1))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)SUB32(0u, (uint32_t)a0))));
    { const bool c = (int32_t)a0 <= 0; (a2 = (uint64_t)(RC_U64X(a0) << (32 + 0))); if (c) goto L_12f310; }
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)65535 << 16))));
    (v0 = (uint64_t)(RC_U64X(v0) >> (32 + 0)));
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 4294967295))));
    (v1 = (uint64_t)(RC_U64X(v1) << (32 + 0)));
    (a1 = (uint64_t)(a1 & v0));
    (a1 = (uint64_t)(a1 | a2));
    (a1 = (uint64_t)(a1 & v1));
    goto L_12f394;
L_12f310:;
    (a0 = (uint64_t)((int64_t)(int32_t)(SRA32((int32_t)v0, 20))));
    (v0 = (uint64_t)(((int64_t)(int64_t)RC_U64X(a0) < (int64_t)(int32_t)20) ? 1 : 0));
    if ((int64_t)v0 == 0) { (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)a0, 4294967276)))); goto L_12f350; }
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)8 << 16))));
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)65535 << 16))));
    (v1 = (uint64_t)(RC_U64X(v1) >> (32 + 0)));
    (v0 = (uint64_t)((int64_t)(int32_t)(SRA32((int32_t)v0, (uint32_t)a0 & 0x1F))));
    (a1 = (uint64_t)(a1 & v1));
    (v0 = (uint64_t)(RC_U64X(v0) << (32 + 0)));
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 4294967295))));
    (v1 = (uint64_t)(RC_U64X(v1) << (32 + 0)));
    (a1 = (uint64_t)(a1 | v0));
    (a1 = (uint64_t)(a1 & v1));
    goto L_12f394;
L_12f350:;
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)65535 << 16))));
    (v1 = (uint64_t)(RC_U64X(v1) >> (32 + 0)));
    (v0 = (uint64_t)(((int64_t)(int64_t)a0 < (int64_t)(int32_t)31) ? 1 : 0));
    (a2 = (uint64_t)(~(0ULL | a0)));
    { const bool c = (int64_t)v0 == 0; (a1 = (uint64_t)(a1 & v1)); if (c) goto L_12f378; }
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 1))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)SLL32((uint32_t)v0, (uint32_t)a2 & 0x1F))));
    goto L_12f37c;
L_12f378:;
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 1))));
L_12f37c:;
    (v0 = (uint64_t)(RC_U64X(v0) << (32 + 0)));
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 4294967295))));
    (v1 = (uint64_t)(RC_U64X(v1) << (32 + 0)));
    (v0 = (uint64_t)(v0 >> (32 + 0)));
    (a1 = (uint64_t)(a1 & v1));
    (a1 = (uint64_t)(a1 | v0));
L_12f394:;
    (v0 = (uint64_t)((uint64_t)a1 + (uint64_t)0ULL));
    { if (xo) { xo[0] = a2; } return v0; }
    __builtin_unreachable();
}

// iosCdStStart (0x1acdf8): typed by tools/typed_lift2.py (round A3; leaf, extra outputs xo[0]=a2 xo[1]=a3)
inline uint64_t iosCdStStart(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint64_t a0, uint64_t *xo = nullptr)
{
    uint64_t v0 = 0;
    uint64_t v1 = 0;
    uint64_t a1 = 0;
    uint64_t a2 = 0;
    uint64_t a3 = 0;
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)31 << 16))));
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)31 << 16))));
    (a1 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)v0, 18452))));
    (a3 = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)31 << 16))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)v1, 18448))));
    (a2 = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)32 << 16))));
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)a0, (uint32_t)a1))));
    (a0 = (uint64_t)(RC_U64X(a0) << 11));
    A1_W32B((uint32_t)a3, 18444, (uint32_t)v0);
    { A1_W64B((uint32_t)a2, -14256, a0); if (xo) { xo[0] = a2; xo[1] = a3; } return v0; }
    __builtin_unreachable();
}

// iosGetTCountExtension (0x1ad528): typed by tools/typed_lift2.py (round A3; leaf, extra outputs xo[0]=a0)
inline uint64_t iosGetTCountExtension(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint64_t *xo = nullptr)
{
    uint64_t v0 = 0;
    uint64_t v1 = 0;
    uint64_t a0 = 0;
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)4096 << 16))));
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)32 << 16))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)a0, 0))));
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v1, 4294957216))));
    (a0 = (uint64_t)(A1_R64B((uint32_t)v1, 0)));
    (v0 = (uint64_t)(RC_U64X(v0) << (32 + 0)));
    (v0 = (uint64_t)(v0 >> (32 + 0)));
    { (v0 = (uint64_t)(a0 | v0)); if (xo) { xo[0] = a0; } return v0; }
    __builtin_unreachable();
}

// iosResetCpuRapCounter (0x1ad628): typed by tools/typed_lift2.py (round A3; leaf, extra outputs xo[0]=a1 xo[1]=a2)
inline uint32_t iosResetCpuRapCounter(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint64_t a0, uint64_t *xo = nullptr)
{
    uint32_t v0 = 0;
    uint32_t v1 = 0;
    uint32_t a1 = 0;
    uint64_t a2 = 0;
    v0 = 0x10000000u;
    v1 = 0x10000000u;
    (a2 = (uint64_t)(a0 | (uint64_t)(uint16_t)128));
    a1 = 0x1E0000u;
    v0 = v0 | 0x810u;
    v1 = v1 | 0x800u;
    A1_W32B(a1, 3028, (uint32_t)a0);
    A1_W32B(v0, 0, (uint32_t)a2);
    A1_W32(v1, 0u);
    { if (xo) { xo[0] = a1_sx64(a1); xo[1] = a2; } return v0; }
    __builtin_unreachable();
}

// iosPadRead (0x1b27b0): typed by tools/typed_lift2.py (round A3; loop leaf, extra outputs xo[0]=a1 xo[1]=a2 xo[2]=a3 xo[3]=t0)
inline uint64_t iosPadRead(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint64_t a0, uint64_t a1, uint64_t *xo = nullptr)
{
    uint64_t at = 0;
    uint64_t v0 = 0;
    uint64_t v1 = 0;
    uint64_t a2 = 0;
    uint64_t a3 = 0;
    uint64_t t0 = 0;
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)SLL32((uint32_t)a0, 4))));
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)SUB32((uint32_t)v1, (uint32_t)a0))));
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)SLL32((uint32_t)v1, 3))));
    (at = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)30 << 16))));
    (at = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)at, 3504))));
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)at, (uint32_t)v1))));
    (v0 = (uint64_t)(RC_U64X(v1) | a1));
    (v0 = (uint64_t)(v0 & (uint64_t)(uint16_t)7));
    { const bool c = (int64_t)v0 == 0; (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v1, 96)))); if (c) goto L_1b2838; }
L_1b27d8:;
    { uint32_t addr = ADD32((uint32_t)v1, 7); uint32_t aligned_addr = addr & ~7u; uint32_t offset = addr & 7u; uint64_t mem = A1_R64(aligned_addr); uint32_t shift = (7u - offset) << 3; uint64_t keepMask = (shift == 0) ? 0ull : ((1ull << shift) - 1ull); (a0 = (uint64_t)((a0 & keepMask) | (mem << shift))); }
    { uint32_t addr = ADD32((uint32_t)v1, 0); uint32_t aligned_addr = addr & ~7u; uint32_t offset = addr & 7u; uint64_t mem = A1_R64(aligned_addr); uint32_t shift = offset << 3; uint64_t keepMask = (offset == 0) ? 0ull : (0xFFFFFFFFFFFFFFFFull << ((8u - offset) << 3)); (a0 = (uint64_t)((a0 & keepMask) | (mem >> shift))); }
    { uint32_t addr = ADD32((uint32_t)v1, 15); uint32_t aligned_addr = addr & ~7u; uint32_t offset = addr & 7u; uint64_t mem = A1_R64(aligned_addr); uint32_t shift = (7u - offset) << 3; uint64_t keepMask = (shift == 0) ? 0ull : ((1ull << shift) - 1ull); (a2 = (uint64_t)((a2 & keepMask) | (mem << shift))); }
    { uint32_t addr = ADD32((uint32_t)v1, 8); uint32_t aligned_addr = addr & ~7u; uint32_t offset = addr & 7u; uint64_t mem = A1_R64(aligned_addr); uint32_t shift = offset << 3; uint64_t keepMask = (offset == 0) ? 0ull : (0xFFFFFFFFFFFFFFFFull << ((8u - offset) << 3)); (a2 = (uint64_t)((a2 & keepMask) | (mem >> shift))); }
    { uint32_t addr = ADD32((uint32_t)v1, 23); uint32_t aligned_addr = addr & ~7u; uint32_t offset = addr & 7u; uint64_t mem = A1_R64(aligned_addr); uint32_t shift = (7u - offset) << 3; uint64_t keepMask = (shift == 0) ? 0ull : ((1ull << shift) - 1ull); (a3 = (uint64_t)((a3 & keepMask) | (mem << shift))); }
    { uint32_t addr = ADD32((uint32_t)v1, 16); uint32_t aligned_addr = addr & ~7u; uint32_t offset = addr & 7u; uint64_t mem = A1_R64(aligned_addr); uint32_t shift = offset << 3; uint64_t keepMask = (offset == 0) ? 0ull : (0xFFFFFFFFFFFFFFFFull << ((8u - offset) << 3)); (a3 = (uint64_t)((a3 & keepMask) | (mem >> shift))); }
    { uint32_t addr = ADD32((uint32_t)v1, 31); uint32_t aligned_addr = addr & ~7u; uint32_t offset = addr & 7u; uint64_t mem = A1_R64(aligned_addr); uint32_t shift = (7u - offset) << 3; uint64_t keepMask = (shift == 0) ? 0ull : ((1ull << shift) - 1ull); (t0 = (uint64_t)((t0 & keepMask) | (mem << shift))); }
    { uint32_t addr = ADD32((uint32_t)v1, 24); uint32_t aligned_addr = addr & ~7u; uint32_t offset = addr & 7u; uint64_t mem = A1_R64(aligned_addr); uint32_t shift = offset << 3; uint64_t keepMask = (offset == 0) ? 0ull : (0xFFFFFFFFFFFFFFFFull << ((8u - offset) << 3)); (t0 = (uint64_t)((t0 & keepMask) | (mem >> shift))); }
    { uint32_t addr = ADD32((uint32_t)a1, 7); uint32_t aligned_addr = addr & ~7u; uint32_t offset = addr & 7u; uint32_t shift = (7u - offset) << 3; uint64_t mask = 0xFFFFFFFFFFFFFFFFull >> shift; uint64_t old_data = A1_R64(aligned_addr); uint64_t val = a0; uint64_t new_data = (old_data & ~mask) | ((val >> shift) & mask); A1_W64(aligned_addr, new_data); }
    { uint32_t addr = ADD32((uint32_t)a1, 0); uint32_t aligned_addr = addr & ~7u; uint32_t offset = addr & 7u; uint32_t shift = offset << 3; uint64_t mask = 0xFFFFFFFFFFFFFFFFull << shift; uint64_t old_data = A1_R64(aligned_addr); uint64_t val = a0; uint64_t new_data = (old_data & ~mask) | ((val << shift) & mask); A1_W64(aligned_addr, new_data); }
    { uint32_t addr = ADD32((uint32_t)a1, 15); uint32_t aligned_addr = addr & ~7u; uint32_t offset = addr & 7u; uint32_t shift = (7u - offset) << 3; uint64_t mask = 0xFFFFFFFFFFFFFFFFull >> shift; uint64_t old_data = A1_R64(aligned_addr); uint64_t val = a2; uint64_t new_data = (old_data & ~mask) | ((val >> shift) & mask); A1_W64(aligned_addr, new_data); }
    { uint32_t addr = ADD32((uint32_t)a1, 8); uint32_t aligned_addr = addr & ~7u; uint32_t offset = addr & 7u; uint32_t shift = offset << 3; uint64_t mask = 0xFFFFFFFFFFFFFFFFull << shift; uint64_t old_data = A1_R64(aligned_addr); uint64_t val = a2; uint64_t new_data = (old_data & ~mask) | ((val << shift) & mask); A1_W64(aligned_addr, new_data); }
    { uint32_t addr = ADD32((uint32_t)a1, 23); uint32_t aligned_addr = addr & ~7u; uint32_t offset = addr & 7u; uint32_t shift = (7u - offset) << 3; uint64_t mask = 0xFFFFFFFFFFFFFFFFull >> shift; uint64_t old_data = A1_R64(aligned_addr); uint64_t val = a3; uint64_t new_data = (old_data & ~mask) | ((val >> shift) & mask); A1_W64(aligned_addr, new_data); }
    { uint32_t addr = ADD32((uint32_t)a1, 16); uint32_t aligned_addr = addr & ~7u; uint32_t offset = addr & 7u; uint32_t shift = offset << 3; uint64_t mask = 0xFFFFFFFFFFFFFFFFull << shift; uint64_t old_data = A1_R64(aligned_addr); uint64_t val = a3; uint64_t new_data = (old_data & ~mask) | ((val << shift) & mask); A1_W64(aligned_addr, new_data); }
    { uint32_t addr = ADD32((uint32_t)a1, 31); uint32_t aligned_addr = addr & ~7u; uint32_t offset = addr & 7u; uint32_t shift = (7u - offset) << 3; uint64_t mask = 0xFFFFFFFFFFFFFFFFull >> shift; uint64_t old_data = A1_R64(aligned_addr); uint64_t val = t0; uint64_t new_data = (old_data & ~mask) | ((val >> shift) & mask); A1_W64(aligned_addr, new_data); }
    { uint32_t addr = ADD32((uint32_t)a1, 24); uint32_t aligned_addr = addr & ~7u; uint32_t offset = addr & 7u; uint32_t shift = offset << 3; uint64_t mask = 0xFFFFFFFFFFFFFFFFull << shift; uint64_t old_data = A1_R64(aligned_addr); uint64_t val = t0; uint64_t new_data = (old_data & ~mask) | ((val << shift) & mask); A1_W64(aligned_addr, new_data); }
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v1, 32))));
    { const bool c = (int64_t)v1 != (int64_t)v0; (a1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)a1, 32)))); if (c) { A3_BACK(); goto L_1b27d8; } }
    goto L_1b286c;
L_1b2838:;
    (a0 = (uint64_t)(A1_R64B((uint32_t)v1, 0)));
    (a2 = (uint64_t)(A1_R64B((uint32_t)v1, 8)));
    (a3 = (uint64_t)(A1_R64B((uint32_t)v1, 16)));
    (t0 = (uint64_t)(A1_R64B((uint32_t)v1, 24)));
    A1_W64B((uint32_t)a1, 0, a0);
    A1_W64B((uint32_t)a1, 8, a2);
    A1_W64B((uint32_t)a1, 16, a3);
    A1_W64B((uint32_t)a1, 24, t0);
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v1, 32))));
    { const bool c = (int64_t)v1 != (int64_t)v0; (a1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)a1, 32)))); if (c) { A3_BACK(); goto L_1b2838; } }
L_1b286c:;
    { uint32_t addr = ADD32((uint32_t)v1, 7); uint32_t aligned_addr = addr & ~7u; uint32_t offset = addr & 7u; uint64_t mem = A1_R64(aligned_addr); uint32_t shift = (7u - offset) << 3; uint64_t keepMask = (shift == 0) ? 0ull : ((1ull << shift) - 1ull); (v0 = (uint64_t)((v0 & keepMask) | (mem << shift))); }
    { uint32_t addr = ADD32((uint32_t)v1, 0); uint32_t aligned_addr = addr & ~7u; uint32_t offset = addr & 7u; uint64_t mem = A1_R64(aligned_addr); uint32_t shift = offset << 3; uint64_t keepMask = (offset == 0) ? 0ull : (0xFFFFFFFFFFFFFFFFull << ((8u - offset) << 3)); (v0 = (uint64_t)((v0 & keepMask) | (mem >> shift))); }
    { uint32_t addr = ADD32((uint32_t)v1, 15); uint32_t aligned_addr = addr & ~7u; uint32_t offset = addr & 7u; uint64_t mem = A1_R64(aligned_addr); uint32_t shift = (7u - offset) << 3; uint64_t keepMask = (shift == 0) ? 0ull : ((1ull << shift) - 1ull); (a0 = (uint64_t)((a0 & keepMask) | (mem << shift))); }
    { uint32_t addr = ADD32((uint32_t)v1, 8); uint32_t aligned_addr = addr & ~7u; uint32_t offset = addr & 7u; uint64_t mem = A1_R64(aligned_addr); uint32_t shift = offset << 3; uint64_t keepMask = (offset == 0) ? 0ull : (0xFFFFFFFFFFFFFFFFull << ((8u - offset) << 3)); (a0 = (uint64_t)((a0 & keepMask) | (mem >> shift))); }
    { uint32_t addr = ADD32((uint32_t)v1, 23); uint32_t aligned_addr = addr & ~7u; uint32_t offset = addr & 7u; uint64_t mem = A1_R64(aligned_addr); uint32_t shift = (7u - offset) << 3; uint64_t keepMask = (shift == 0) ? 0ull : ((1ull << shift) - 1ull); (a2 = (uint64_t)((a2 & keepMask) | (mem << shift))); }
    { uint32_t addr = ADD32((uint32_t)v1, 16); uint32_t aligned_addr = addr & ~7u; uint32_t offset = addr & 7u; uint64_t mem = A1_R64(aligned_addr); uint32_t shift = offset << 3; uint64_t keepMask = (offset == 0) ? 0ull : (0xFFFFFFFFFFFFFFFFull << ((8u - offset) << 3)); (a2 = (uint64_t)((a2 & keepMask) | (mem >> shift))); }
    { uint32_t addr = ADD32((uint32_t)a1, 7); uint32_t aligned_addr = addr & ~7u; uint32_t offset = addr & 7u; uint32_t shift = (7u - offset) << 3; uint64_t mask = 0xFFFFFFFFFFFFFFFFull >> shift; uint64_t old_data = A1_R64(aligned_addr); uint64_t val = v0; uint64_t new_data = (old_data & ~mask) | ((val >> shift) & mask); A1_W64(aligned_addr, new_data); }
    { uint32_t addr = ADD32((uint32_t)a1, 0); uint32_t aligned_addr = addr & ~7u; uint32_t offset = addr & 7u; uint32_t shift = offset << 3; uint64_t mask = 0xFFFFFFFFFFFFFFFFull << shift; uint64_t old_data = A1_R64(aligned_addr); uint64_t val = v0; uint64_t new_data = (old_data & ~mask) | ((val << shift) & mask); A1_W64(aligned_addr, new_data); }
    { uint32_t addr = ADD32((uint32_t)a1, 15); uint32_t aligned_addr = addr & ~7u; uint32_t offset = addr & 7u; uint32_t shift = (7u - offset) << 3; uint64_t mask = 0xFFFFFFFFFFFFFFFFull >> shift; uint64_t old_data = A1_R64(aligned_addr); uint64_t val = a0; uint64_t new_data = (old_data & ~mask) | ((val >> shift) & mask); A1_W64(aligned_addr, new_data); }
    { uint32_t addr = ADD32((uint32_t)a1, 8); uint32_t aligned_addr = addr & ~7u; uint32_t offset = addr & 7u; uint32_t shift = offset << 3; uint64_t mask = 0xFFFFFFFFFFFFFFFFull << shift; uint64_t old_data = A1_R64(aligned_addr); uint64_t val = a0; uint64_t new_data = (old_data & ~mask) | ((val << shift) & mask); A1_W64(aligned_addr, new_data); }
    { uint32_t addr = ADD32((uint32_t)a1, 23); uint32_t aligned_addr = addr & ~7u; uint32_t offset = addr & 7u; uint32_t shift = (7u - offset) << 3; uint64_t mask = 0xFFFFFFFFFFFFFFFFull >> shift; uint64_t old_data = A1_R64(aligned_addr); uint64_t val = a2; uint64_t new_data = (old_data & ~mask) | ((val >> shift) & mask); A1_W64(aligned_addr, new_data); }
    { { uint32_t addr = ADD32((uint32_t)a1, 16); uint32_t aligned_addr = addr & ~7u; uint32_t offset = addr & 7u; uint32_t shift = offset << 3; uint64_t mask = 0xFFFFFFFFFFFFFFFFull << shift; uint64_t old_data = A1_R64(aligned_addr); uint64_t val = a2; uint64_t new_data = (old_data & ~mask) | ((val << shift) & mask); A1_W64(aligned_addr, new_data); } if (xo) { xo[0] = a1; xo[1] = a2; xo[2] = a3; xo[3] = t0; } return v0; }
    __builtin_unreachable();
}

// iosMemToSprDmaAdrCopy (0x1b2a18): typed by tools/typed_lift2.py (round A3; leaf, extra outputs xo[0]=a3 xo[1]=t0 xo[2]=t1 xo[3]=t2 xo[4]=t3)
inline uint32_t iosMemToSprDmaAdrCopy(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t a0, uint32_t a1, uint32_t a2, uint64_t *xo = nullptr)
{
    uint32_t v0 = 0;
    uint32_t v1 = 0;
    uint32_t a3 = 0;
    uint32_t t0 = 0;
    uint32_t t1 = 0;
    uint32_t t2 = 0;
    uint32_t t3 = 0;
    t0 = 0x10000000u;
    v0 = 0x10000000u;
    a3 = 0x10000000u;
    t1 = 0x10000000u;
    t2 = 0x10000000u;
    t0 = t0 | 0xE020u;
    v1 = 0xFFFFFC00u;
    v0 = v0 | 0xE010u;
    t3 = 0x200u;
    a3 = a3 | 0xD410u;
    t1 = t1 | 0xD420u;
    t2 = t2 | 0xD480u;
    A1_W32(t0, v1);
    v1 = 0x10000000u;
    A1_W32(v0, t3);
    v1 = v1 | 0xD400u;
    A1_W32(a3, a0);
    a0 = 0x101u;
    A1_W32(t1, a1);
    A1_W32(t2, a2);
    v0 = A1_R32(t0);
    v0 = v0 | 0x200u;
    A1_W32(t0, v0);
    A1_W32(v1, a0);
    { if (xo) { xo[0] = a1_sx64(a3); xo[1] = a1_sx64(t0); xo[2] = a1_sx64(t1); xo[3] = a1_sx64(t2); xo[4] = a1_sx64(t3); } return v0; }
    __builtin_unreachable();
}

// Init_Shock (0x1b3c50): typed by tools/typed_lift2.py (round A3; loop leaf)
inline uint32_t Init_Shock(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt)
{
    uint32_t v0 = 0;
    uint32_t v1 = 0;
    uint32_t a0 = 0;
    uint32_t a1 = 0;
    uint64_t a2 = 0;
    uint64_t a3 = 0;
    uint32_t t0 = 0;
    v0 = 0x200000u;
    v1 = 0x200000u;
    a1 = v0 + 0x21D0u;
    { const bool c = a1 == 0; v1 = v1 + 0x21E0u; if (c) goto L_1b3cb0; }
    { const bool c = v1 == 0; t0 = 0x1E0000u; if (c) goto L_1b3cb0; }
    A1_W32B(a1, 4, v1);
    v0 = 2u;
    v1 = t0 + 0xEA8u;
    A1_W32(a1, v0);
    (a3 = (uint64_t)((uint64_t)a1_sx64(v1) + (uint64_t)0ULL));
    A1_W32(v1, a1);
    (a2 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL));
L_1b3c88:;
    (a0 = (uint32_t)((int32_t)((int32_t)A1_R32B((uint32_t)a3, 0))));
    (v0 = (uint32_t)((int32_t)((int32_t)SLL32((uint32_t)a2, 2))));
    (a2 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)a2, 1))));
    v1 = A1_R32B(a0, 4);
    (a1 = (uint32_t)(((int64_t)(int64_t)RC_U64X(a2) < (int64_t)(int32_t)2) ? 1 : 0));
    v0 = v0 + v1;
    { const bool c = a1 != 0; A1_W32(v0, 0u); if (c) { A3_BACK(); goto L_1b3c88; } }
    v0 = A1_R32B(t0, 3752);
    A1_W32B(v0, 8, 0u);
L_1b3cb0:;
    v0 = 0x200000u;
    v1 = 0x200000u;
    a1 = v0 + 0x21E8u;
    { const bool c = a1 == 0; v1 = v1 + 0x21F0u; if (c) goto L_1b3d08; }
    { const bool c = v1 == 0; v0 = 0x10u; if (c) goto L_1b3d08; }
    a0 = v1;
    A1_W32(a1, v0);
    A1_W32B(a1, 4, v1);
    v1 = 0u;
L_1b3ce0:;
    v1 = v1 + 1u;
    A1_W8(a0, (uint8_t)0u);
    v0 = (int32_t)v1 < 16 ? 1u : 0u;
    { const bool c = v0 != 0; a0 = a0 + 0x40u; if (c) { A3_BACK(); goto L_1b3ce0; } }
    { return v0; }
L_1b3d08:;
    { A1_W32(a1, 0u); return v0; }
    __builtin_unreachable();
}

// kernel_sub_001B40D0 (0x1b40d0): typed by tools/typed_lift2.py (round A3; loop leaf)
inline uint32_t kernel_sub_001B40D0(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint64_t a0)
{
    uint32_t v0 = 0;
    uint32_t v1 = 0;
    uint32_t a1 = 0;
    uint64_t a2 = 0;
    (v0 = (uint32_t)((int32_t)((int32_t)A1_R32B((uint32_t)a0, 0))));
    (a2 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL));
    { const bool c = (int32_t)v0 <= 0; (a1 = (uint32_t)((int32_t)((int32_t)A1_R32B((uint32_t)a0, 4)))); if (c) goto L_1b410c; }
    (a0 = (uint64_t)((uint64_t)a1_sx64(v0) + (uint64_t)0ULL));
    v1 = A1_R8(a1);
L_1b40e8:;
    v0 = a1;
    { const bool c = v1 == 0; a1 = a1 + 0x40u; if (c) goto L_1b4110; }
    (a2 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)a2, 1))));
    (v0 = (uint32_t)(((int64_t)(int64_t)RC_U64X(a2) < (int64_t)(int64_t)a0) ? 1 : 0));
    if (v0 != 0) { v1 = A1_R8(a1); { A3_BACK(); goto L_1b40e8; } }
L_1b410c:;
    v0 = 0u;
L_1b4110:;
    { return v0; }
    __builtin_unreachable();
}

// iosLoaderGetFileSize (0x1b7500): typed by tools/typed_lift2.py (round A3; loop leaf, extra outputs xo[0]=a3)
inline uint32_t iosLoaderGetFileSize(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint64_t a0, uint64_t *xo = nullptr)
{
    uint32_t v0 = 0;
    uint64_t v1 = 0;
    uint32_t a1 = 0;
    uint64_t a2 = 0;
    uint64_t a3 = 0;
    v0 = 0x200000u;
    (a3 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL));
    (a1 = (uint32_t)((int32_t)((int32_t)ADD32(v0, 9832))));
    (a2 = (uint64_t)((uint64_t)a1_sx64(a1) + (uint64_t)0ULL));
L_1b7510:;
    v0 = A1_R32(a1);
    a1 = a1 + 8u;
    { const bool c = (int64_t)(int32_t)v0 == (int64_t)a0; (v1 = (uint64_t)((uint64_t)a2 + (uint64_t)0ULL)); if (c) goto L_1b7538; }
    (a3 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)a3, 1))));
    (v0 = (uint32_t)(((int64_t)(int64_t)RC_U64X(a3) < (int64_t)(int32_t)8) ? 1 : 0));
    { const bool c = v0 != 0; (a2 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v1, 8)))); if (c) { A3_BACK(); goto L_1b7510; } }
    (v1 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL));
L_1b7538:;
    { const bool c = (int64_t)v1 == 0; v0 = 0u; if (c) goto L_1b7544; }
    (v0 = (uint32_t)((int32_t)((int32_t)A1_R32B((uint32_t)v1, 4))));
L_1b7544:;
    { if (xo) { xo[0] = a3; } return v0; }
    __builtin_unreachable();
}

// initDldSys (0x1b7550): typed by tools/typed_lift2.py (round A3; loop leaf, extra outputs xo[0]=a0)
inline uint32_t initDldSys(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint64_t *xo = nullptr)
{
    uint32_t v0 = 0;
    uint32_t v1 = 0;
    uint32_t a0 = 0;
    v0 = 0x200000u;
    a0 = 0xFFFFFFFFu;
    v0 = v0 + 0x2668u;
    v1 = 7u;
    v0 = v0 + 0x38u;
L_1b7568:;
    v1 = v1 - 1u;
    A1_W32(v0, a0);
    { const bool c = (int32_t)v1 >= 0; v0 = v0 - 8u; if (c) { A3_BACK(); goto L_1b7568; } }
    { if (xo) { xo[0] = a1_sx64(a0); } return v0; }
    __builtin_unreachable();
}

// kernel_sub_001BA690 (0x1ba690): typed by tools/typed_lift2.py (round A3; leaf, extra outputs xo[0]=v0)
inline float kernel_sub_001BA690(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint64_t a0, uint64_t a1, uint64_t *xo = nullptr)
{
    float f0 = 0.0f;
    float f1 = 0.0f;
    float f2 = 0.0f;
    float f3 = 0.0f;
    float f4 = 0.0f;
    float f5 = 0.0f;
    uint32_t at = 0;
    uint32_t v0 = 0;
    uint32_t v1 = 0;
    uint32_t a2 = 0;
    uint32_t a3 = 0;
    uint64_t t0 = 0;
    uint32_t t1 = 0;
    uint64_t t2 = 0;
    uint32_t t3 = 0;
    uint64_t t4 = 0;
    uint32_t fcr31 = 0;
    (t0 = (uint64_t)((uint64_t)a1 + (uint64_t)0ULL));
    { uint32_t bits = 0u; std::memcpy(&f5, &bits, sizeof(bits)); }
    { uint32_t bits = A1_R32B((uint32_t)t0, 0); float f; std::memcpy(&f, &bits, sizeof(f)); f3 = f; }
    { uint32_t bits = A1_R32B((uint32_t)t0, 20); float f; std::memcpy(&f, &bits, sizeof(f)); f2 = f; }
    { uint32_t bits = A1_R32B((uint32_t)t0, 40); float f; std::memcpy(&f, &bits, sizeof(f)); f4 = f; }
    f0 = a1f::add(f3, f2);
    f1 = a1f::add(f0, f4);
    fcr31 = (a1f::lt(f5, f1)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
    { const bool c = (fcr31 & 0x800000u) == 0u; (t2 = (uint64_t)((uint64_t)a0 + (uint64_t)0ULL)); if (c) goto L_1ba728; }
    at = 0x3F800000u;
    { uint32_t bits = at; std::memcpy(&f0, &bits, sizeof(bits)); }
    f0 = a1f::addFinite(f1, f0);
    f3 = a1f::sqrt(fcr31, f0);
    at = 0x3F000000u;
    { uint32_t bits = at; std::memcpy(&f1, &bits, sizeof(bits)); }
    f2 = ((float)(f3) * (float)(f1));
    f4 = a1f::div(fcr31, f1, f3);
    { float f = f2; uint32_t bits; std::memcpy(&bits, &f, sizeof(bits)); A1_W32B((uint32_t)t2, 12, bits); }
    { uint32_t bits = A1_R32B((uint32_t)t0, 36); float f; std::memcpy(&f, &bits, sizeof(f)); f1 = f; }
    { uint32_t bits = A1_R32B((uint32_t)t0, 24); float f; std::memcpy(&f, &bits, sizeof(f)); f0 = f; }
    f0 = a1f::sub(f0, f1);
    f0 = ((float)(f0) * (float)(f4));
    { float f = f0; uint32_t bits; std::memcpy(&bits, &f, sizeof(bits)); A1_W32B((uint32_t)t2, 0, bits); }
    { uint32_t bits = A1_R32B((uint32_t)t0, 8); float f; std::memcpy(&f, &bits, sizeof(f)); f0 = f; }
    { uint32_t bits = A1_R32B((uint32_t)t0, 32); float f; std::memcpy(&f, &bits, sizeof(f)); f1 = f; }
    f1 = a1f::sub(f1, f0);
    f1 = ((float)(f1) * (float)(f4));
    { float f = f1; uint32_t bits; std::memcpy(&bits, &f, sizeof(bits)); A1_W32B((uint32_t)t2, 4, bits); }
    { uint32_t bits = A1_R32B((uint32_t)t0, 16); float f; std::memcpy(&f, &bits, sizeof(f)); f2 = f; }
    { uint32_t bits = A1_R32B((uint32_t)t0, 4); float f; std::memcpy(&f, &bits, sizeof(f)); f0 = f; }
    f0 = a1f::sub(f0, f2);
    f0 = ((float)(f0) * (float)(f4));
    { { float f = f0; uint32_t bits; std::memcpy(&bits, &f, sizeof(bits)); A1_W32B((uint32_t)t2, 8, bits); } return f0; }
L_1ba728:;
    fcr31 = (a1f::lt(f3, f2)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
    { const bool c = (fcr31 & 0x800000u) == 0u; (t4 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL)); if (c) goto L_1ba738; }
    (t4 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 1))));
L_1ba738:;
    (t1 = (uint32_t)((int32_t)((int32_t)SLL32((uint32_t)t4, 2))));
    (v0 = (uint32_t)((int32_t)((int32_t)ADD32(t1, (uint32_t)t4))));
    v0 = v0 << 2;
    (v0 = (uint32_t)((int32_t)((int32_t)ADD32(v0, (uint32_t)t0))));
    f0 = a1_flt(A1_R32(v0));
    fcr31 = (a1f::lt(f0, f4)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
    { const bool c = (fcr31 & 0x800000u) == 0u; (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)30 << 16)))); if (c) goto L_1ba760; }
    (t4 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 2))));
    t1 = 8u;
L_1ba760:;
    (v0 = (uint32_t)((int32_t)((int32_t)ADD32(t1, (uint32_t)t4))));
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)a0, 3944))));
    v0 = v0 << 2;
    (v1 = (uint32_t)((int32_t)((int32_t)ADD32(t1, (uint32_t)a0))));
    (v0 = (uint32_t)((int32_t)((int32_t)ADD32(v0, (uint32_t)t0))));
    (a1 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B(v1, 0))));
    f2 = a1_flt(A1_R32(v0));
    (a3 = (uint32_t)((int32_t)((int32_t)SLL32((uint32_t)a1, 2))));
    at = 0x3F800000u;
    { uint32_t bits = at; std::memcpy(&f3, &bits, sizeof(bits)); }
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(a3, (uint32_t)a0))));
    (v1 = (uint32_t)((int32_t)((int32_t)ADD32(a3, (uint32_t)a1))));
    (t3 = (uint32_t)((int32_t)((int32_t)A1_R32B((uint32_t)a0, 0))));
    v1 = v1 << 2;
    (v1 = (uint32_t)((int32_t)((int32_t)ADD32(v1, (uint32_t)t0))));
    a2 = t3 << 2;
    f1 = a1_flt(A1_R32(v1));
    v0 = a2 + t3;
    v0 = v0 << 2;
    (v0 = (uint32_t)((int32_t)((int32_t)ADD32(v0, (uint32_t)t0))));
    f0 = a1_flt(A1_R32(v0));
    f1 = a1f::add(f1, f0);
    f2 = a1f::sub(f2, f1);
    f2 = a1f::addFinite(f2, f3);
    f3 = a1f::sqrt(fcr31, f2);
    at = 0x3F000000u;
    { uint32_t bits = at; std::memcpy(&f1, &bits, sizeof(bits)); }
    f4 = ((float)(f5));
    fcr31 = (a1f::eq(f3, f4)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
    (v0 = (uint32_t)((int32_t)((int32_t)ADD32(t1, (uint32_t)t2))));
    f0 = ((float)(f3) * (float)(f1));
    { const bool c = (fcr31 & 0x800000u) != 0u; A1_W32(v0, a1_bits(f0)); if (c) goto L_1ba7f0; }
    f4 = a1f::div(fcr31, f1, f3);
L_1ba7f0:;
    (v1 = (uint32_t)((int32_t)((int32_t)ADD32(a2, (uint32_t)a1))));
    (a1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(t1, (uint32_t)a1))));
    v0 = a3 + t3;
    v1 = v1 << 2;
    v0 = v0 << 2;
    (v1 = (uint32_t)((int32_t)((int32_t)ADD32(v1, (uint32_t)t0))));
    (v0 = (uint32_t)((int32_t)((int32_t)ADD32(v0, (uint32_t)t0))));
    f0 = a1_flt(A1_R32(v1));
    f1 = a1_flt(A1_R32(v0));
    v0 = t1 + t3;
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(a3, (uint32_t)t4))));
    (a3 = (uint32_t)((int32_t)((int32_t)ADD32(a3, (uint32_t)t2))));
    f1 = a1f::sub(f1, f0);
    (a1 = (uint64_t)((int64_t)(int32_t)((int32_t)SLL32((uint32_t)a1, 2))));
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)SLL32((uint32_t)a0, 2))));
    (a1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)a1, (uint32_t)t0))));
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)a0, (uint32_t)t0))));
    (v1 = (uint32_t)((int32_t)((int32_t)ADD32(a2, (uint32_t)t4))));
    v0 = v0 << 2;
    v1 = v1 << 2;
    f1 = a1f::mul(f1, f4);
    (v1 = (uint32_t)((int32_t)((int32_t)ADD32(v1, (uint32_t)t0))));
    (v0 = (uint32_t)((int32_t)((int32_t)ADD32(v0, (uint32_t)t0))));
    (a2 = (uint32_t)((int32_t)((int32_t)ADD32(a2, (uint32_t)t2))));
    { float f = f1; uint32_t bits; std::memcpy(&bits, &f, sizeof(bits)); A1_W32B((uint32_t)t2, 12, bits); }
    { uint32_t bits = A1_R32B((uint32_t)a0, 0); float f; std::memcpy(&f, &bits, sizeof(f)); f1 = f; }
    { uint32_t bits = A1_R32B((uint32_t)a1, 0); float f; std::memcpy(&f, &bits, sizeof(f)); f0 = f; }
    f0 = a1f::add(f0, f1);
    f0 = a1f::mul(f0, f4);
    A1_W32(a3, a1_bits(f0));
    f1 = a1_flt(A1_R32(v0));
    f0 = a1_flt(A1_R32(v1));
    f1 = a1f::add(f1, f0);
    f1 = a1f::mul(f1, f4);
    { A1_W32(a2, a1_bits(f1)); if (xo) { xo[0] = a1_sx64(v0); } return f0; }
    __builtin_unreachable();
}

// iosGetTableCos (0x1be570): typed by tools/typed_lift2.py (round A3; leaf, extra outputs xo[0]=a0)
inline a1::RetVF iosGetTableCos(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t a0, uint64_t *xo = nullptr)
{
    float f0 = 0.0f;
    uint32_t at = 0;
    uint64_t v0 = 0;
    uint64_t v1 = 0;
    a0 = a0 << 16;
    a0 = (uint32_t)((int32_t)a0 >> 16);
    if ((int32_t)a0 < 0) { a0 = 0u - a0; goto L_1be580; }
L_1be580:;
    (v0 = (uint64_t)(((int64_t)(int64_t)a1_sx64(a0) < (int64_t)(int32_t)16384) ? 1 : 0));
    { const bool c = (int64_t)v0 != 0; (v1 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL)); if (c) goto L_1be598; }
    (v0 = (uint64_t)(0ULL | (uint64_t)(uint16_t)32768));
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 1))));
    (a0 = (uint32_t)((int32_t)((int32_t)SUB32((uint32_t)v0, a0))));
L_1be598:;
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)SLL32(a0, 2))));
    at = 0x200000u;
    (at = (uint32_t)((int32_t)((int32_t)ADD32(at, (uint32_t)v0))));
    f0 = a1_flt(A1_R32B(at, 11968));
    if ((int64_t)v1 != 0) { f0 = (-(float)(f0)); goto L_1be5b0; }
L_1be5b0:;
    { if (xo) { xo[0] = a1_sx64(a0); } return a1::RetVF{(uint64_t)v0, f0}; }
    __builtin_unreachable();
}

// iosSetDCViewPortCenter (0x1bf160): typed by tools/typed_lift2.py (round A3; leaf, extra outputs xo[0]=t1 xo[1]=t2 xo[2]=t3)
inline uint64_t iosSetDCViewPortCenter(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint64_t a0, uint64_t a1, uint64_t a2, uint64_t a3, uint64_t t0, uint64_t t1, uint64_t t2, uint64_t *xo = nullptr)
{
    uint64_t v0 = 0;
    uint64_t v1 = 0;
    uint64_t t3 = 0;
    uint64_t t4 = 0;
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)a1, (uint32_t)a3))));
    (t3 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)a2, (uint32_t)t0))));
    (t2 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)t2, 2048))));
    (t3 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)t3, 4294967295))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v0, 4294967295))));
    (t1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)t1, 2048))));
    (v0 = (uint64_t)(RC_U64X(v0) << 16));
    (t2 = (uint64_t)((int64_t)(int32_t)((int32_t)SLL32((uint32_t)t2, 4))));
    (t3 = (uint64_t)(RC_U64X(t3) << (32 + 16)));
    (v1 = (uint64_t)(a2 << (32 + 0)));
    (v1 = (uint64_t)(v1 | v0));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 24))));
    (t1 = (uint64_t)((int64_t)(int32_t)((int32_t)SLL32((uint32_t)t1, 4))));
    (t2 = (uint64_t)(RC_U64X(t2) << (32 + 0)));
    (t3 = (uint64_t)(a1 | t3));
    (t2 = (uint64_t)(RC_U64X(t1) | t2));
    (t3 = (uint64_t)(t3 | v1));
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 25))));
    (t1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 64))));
    (t4 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 65))));
    A1_W64B((uint32_t)a0, 128, t3);
    A1_W64B((uint32_t)a0, 104, RC_U64X(v0));
    A1_W64B((uint32_t)a0, 120, RC_U64X(v1));
    A1_W64B((uint32_t)a0, 96, t2);
    A1_W64B((uint32_t)a0, 136, RC_U64X(t1));
    A1_W64B((uint32_t)a0, 152, RC_U64X(t4));
    A1_W32B((uint32_t)a0, 372, (uint32_t)a1);
    A1_W32B((uint32_t)a0, 376, (uint32_t)a2);
    A1_W32B((uint32_t)a0, 380, (uint32_t)a3);
    A1_W32B((uint32_t)a0, 384, (uint32_t)t0);
    A1_W64B((uint32_t)a0, 112, t2);
    { A1_W64B((uint32_t)a0, 144, t3); if (xo) { xo[0] = t1; xo[1] = t2; xo[2] = t3; } return v0; }
    __builtin_unreachable();
}

// iosUsedPacketBufferSize (0x1bfad8): typed by tools/typed_lift2.py (round A3; leaf, extra outputs xo[0]=a0 xo[1]=a1)
inline a1::RetVF iosUsedPacketBufferSize(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint64_t *xo = nullptr)
{
    float f0 = 0.0f;
    float f1 = 0.0f;
    uint32_t v0 = 0;
    uint32_t v1 = 0;
    uint32_t a0 = 0;
    uint32_t a1 = 0;
    uint32_t fcr31 = 0;
    v1 = 0x210000u;
    v1 = v1 + 0x32D8u;
    v0 = A1_R32B(v1, 24);
    a1 = A1_R32B(v1, 16);
    v0 = v0 << 2;
    v0 = v0 + v1;
    a0 = A1_R32(v0);
    a1 = a1 - a0;
    v0 = a1 >> 1;
    v1 = a1 & 1u;
    { const bool c = (int32_t)a1 < 0; v1 = v1 | v0; if (c) goto L_1bfb18; }
    f1 = a1_flt(a1);
    { int32_t tmp; std::memcpy(&tmp, &f1, sizeof(tmp)); f1 = ((float)(int32_t)(tmp)); }
    v0 = 0x1E0000u;
    goto L_1bfb28;
L_1bfb18:;
    f1 = a1_flt(v1);
    { int32_t tmp; std::memcpy(&tmp, &f1, sizeof(tmp)); f1 = ((float)(int32_t)(tmp)); }
    f1 = a1f::add(f1, f1);
    v0 = 0x1E0000u;
L_1bfb28:;
    v1 = A1_R32B(v0, 4268);
    v1 = v1 << 4;
    f0 = a1_flt(v1);
    { int32_t tmp; std::memcpy(&tmp, &f0, sizeof(tmp)); f0 = ((float)(int32_t)(tmp)); }
    f0 = a1f::div(fcr31, f1, f0);
    { if (xo) { xo[0] = a1_sx64(a0); xo[1] = a1_sx64(a1); } return a1::RetVF{(uint64_t)a1_sx64(v0), f0}; }
    __builtin_unreachable();
}

// iosAllocPacketPtrPath1 (0x1bfc08): typed by tools/typed_lift2.py (round A3; leaf, extra outputs xo[0]=a1)
inline uint32_t iosAllocPacketPtrPath1(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t a0, uint64_t *xo = nullptr)
{
    uint32_t v0 = 0;
    uint32_t v1 = 0;
    uint32_t a1 = 0;
    v1 = 0x210000u;
    v1 = v1 + 0x32D8u;
    v0 = A1_R32B(v1, 16);
    a1 = A1_R32B(v1, 16);
    a1 = a1 + a0;
    A1_W32B(v1, 16, a1);
    // PREF instruction (ignored)
    { if (xo) { xo[0] = a1_sx64(a1); } return v0; }
    __builtin_unreachable();
}

// iosDlChainTail (0x1bfcf8): typed by tools/typed_lift2.py (round A3; leaf, extra outputs xo[0]=a2 xo[1]=a3 xo[2]=t0 xo[3]=t1)
inline uint64_t iosDlChainTail(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint64_t a0, uint64_t a1, uint64_t *xo = nullptr)
{
    uint64_t v0 = 0;
    uint64_t v1 = 0;
    uint64_t a2 = 0;
    uint64_t a3 = 0;
    uint64_t t0 = 0;
    uint64_t t1 = 0;
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)33 << 16))));
    (a2 = (uint64_t)((int64_t)(int32_t)((int32_t)SLL32((uint32_t)a0, 1))));
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v1, 13016))));
    (a2 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)a2, (uint32_t)a0))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)v1, 24))));
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v1, 8))));
    (a2 = (uint64_t)((int64_t)(int32_t)((int32_t)SLL32((uint32_t)a2, 4))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)SLL32((uint32_t)v0, 2))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v0, (uint32_t)a0))));
    (a3 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)v0, 0))));
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)v1, 16))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)v1, 16))));
    (a3 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)a3, (uint32_t)a2))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v0, 32))));
    A1_W32B((uint32_t)v1, 16, (uint32_t)v0);
    // PREF instruction (ignored)
    (a1 = (uint64_t)(a1 << (32 + 0)));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)20480 << 16))));
    (a1 = (uint64_t)(a1 | RC_U64X(v0)));
    A1_W32B((uint32_t)a0, 28, 0u);
    A1_W64B((uint32_t)a0, 0, a1);
    (a2 = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)32767 << 16))));
    (a2 = (uint64_t)(RC_U64X(a2) | (uint64_t)(uint16_t)65535));
    A1_W32B((uint32_t)a0, 24, 0u);
    (t0 = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)8192 << 16))));
    A1_W32B((uint32_t)a0, 12, 0u);
    (a1 = (uint64_t)(RC_U64X(a0) & RC_U64X(a2)));
    A1_W32B((uint32_t)a0, 8, 0u);
    (t1 = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)32768 << 16))));
    (t1 = (uint64_t)(RC_U64X(t1) << 16));
    (t1 = (uint64_t)(t1 | (uint64_t)(uint16_t)65535));
    (t1 = (uint64_t)(t1 << 16));
    (t1 = (uint64_t)(t1 | (uint64_t)(uint16_t)65535));
    (a1 = (uint64_t)(RC_U64X(a1) << (32 + 0)));
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)a3, 36))));
    (v0 = (uint64_t)(uint32_t)(A1_R32B((uint32_t)v1, 20)));
    (v0 = (uint64_t)(v0 & RC_U64X(a2)));
    (v0 = (uint64_t)(v0 << (32 + 0)));
    (v0 = (uint64_t)((int64_t)v0 >> (32 + 0)));
    (v0 = (uint64_t)(v0 << (32 + 0)));
    (v0 = (uint64_t)(v0 | RC_U64X(t0)));
    A1_W64B((uint32_t)a0, 16, v0);
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)a3, 36))));
    (v0 = (uint64_t)(A1_R64B((uint32_t)v1, 16)));
    (v0 = (uint64_t)(v0 & t1));
    (v0 = (uint64_t)(v0 | a1));
    A1_W64B((uint32_t)v1, 16, v0);
    { A1_W32B((uint32_t)a3, 36, (uint32_t)a0); if (xo) { xo[0] = a2; xo[1] = a3; xo[2] = t0; xo[3] = t1; } return v0; }
    __builtin_unreachable();
}

// kernel_sub_001C1538 (0x1c1538): typed by tools/typed_lift2.py (round A3; frame leaf, extra outputs xo[0]=t2 xo[1]=t3 xo[2]=t4 xo[3]=t5 xo[4]=t6 xo[5]=t7, frame 16 bytes, 0 slot accesses)
inline uint64_t kernel_sub_001C1538(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint64_t *xo = nullptr)
{
    uint64_t v0 = 0;
    uint64_t v1 = 0;
    uint64_t a0 = 0;
    uint32_t a1 = 0;
    uint64_t a2 = 0;
    uint32_t a3 = 0;
    uint32_t t0 = 0;
    uint32_t t1 = 0;
    uint32_t t2 = 0;
    uint32_t t3 = 0;
    uint32_t t4 = 0;
    uint64_t t5 = 0;
    uint64_t t6 = 0;
    uint64_t t7 = 0;
    uint64_t s0 = 0;
    a1 = 0x210000u;
    a1 = a1 + 0x3810u;
    (a3 = (uint32_t)((int32_t)((int32_t)((uint32_t)4608 << 16))));
    (t1 = (uint32_t)((int32_t)((int32_t)((uint32_t)4608 << 16))));
    t4 = A1_R32B(a1, 140);
    t1 = t1 | 0x20u;
    (t2 = (uint32_t)((int32_t)((int32_t)((uint32_t)4608 << 16))));
    (t3 = (uint32_t)((int32_t)((int32_t)((uint32_t)4608 << 16))));
    (v0 = (uint64_t)(a1_sx64(t4) & (uint64_t)(uint16_t)1));
    t4 = t4 ^ 1u;
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)SLL32((uint32_t)v0, 3))));
    t2 = t2 | 0x90u;
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)SUB32((uint32_t)a0, (uint32_t)v0))));
    t3 = t3 | 0x70u;
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)SLL32((uint32_t)a0, 3))));
    (t0 = (uint32_t)((int32_t)((int32_t)((uint32_t)4608 << 16))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(a1, (uint32_t)a0))));
    t0 = t0 | 0xA0u;
    (v1 = (uint64_t)(A1_R64B((uint32_t)v0, 24)));
    (a2 = (uint64_t)((uint64_t)RC_U64X(v0) + (uint64_t)0ULL));
    (t5 = (uint64_t)((uint64_t)RC_U64X(a2) + (uint64_t)0ULL));
    A1_W64B(a3, 0, v1);
    a3 = a3 | 0x80u;
    (t6 = (uint64_t)((uint64_t)RC_U64X(t5) + (uint64_t)0ULL));
    (v0 = (uint64_t)(A1_R64B((uint32_t)a2, 32)));
    (a2 = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)4608 << 16))));
    (t7 = (uint64_t)((uint64_t)RC_U64X(t6) + (uint64_t)0ULL));
    (a2 = (uint64_t)(RC_U64X(a2) | (uint64_t)(uint16_t)224));
    A1_W64B(t1, 0, v0);
    (s0 = (uint64_t)((uint64_t)RC_U64X(t7) + (uint64_t)0ULL));
    (a0 = (uint64_t)((uint64_t)RC_U64X(s0) + (uint64_t)0ULL));
    (v0 = (uint64_t)(A1_R64B((uint32_t)t5, 40)));
    A1_W64B(t2, 0, v0);
    (v1 = (uint64_t)(A1_R64B((uint32_t)t6, 56)));
    A1_W64B(t3, 0, v1);
    (v0 = (uint64_t)(A1_R64B((uint32_t)t7, 48)));
    A1_W64B(t0, 0, v0);
    (v1 = (uint64_t)(A1_R64B((uint32_t)s0, 64)));
    A1_W64B(a3, 0, v1);
    (v0 = (uint64_t)(A1_R64B((uint32_t)a0, 72)));
    A1_W64B((uint32_t)a2, 0, v0);
    A1_W32B(a1, 140, t4);
    { if (xo) { xo[0] = a1_sx64(t2); xo[1] = a1_sx64(t3); xo[2] = a1_sx64(t4); xo[3] = t5; xo[4] = t6; xo[5] = t7; } return v0; }
    __builtin_unreachable();
}

// iosGsSetMode (0x1c15f8): typed by tools/typed_lift2.py (round A3; leaf, extra outputs xo[0]=t0 xo[1]=t1 xo[2]=t2 xo[3]=t3)
inline uint32_t iosGsSetMode(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint64_t *xo = nullptr)
{
    uint32_t v0 = 0;
    uint32_t v1 = 0;
    uint32_t a0 = 0;
    uint32_t a1 = 0;
    uint32_t a2 = 0;
    uint32_t a3 = 0;
    uint32_t t0 = 0;
    uint32_t t1 = 0;
    uint32_t t2 = 0;
    uint32_t t3 = 0;
    v0 = 0x210000u;
    t3 = v0 + 0x3810u;
    v1 = A1_R32B(t3, 164);
    { const bool c = v1 == 0; t0 = 0x200u; if (c) goto L_1c1650; }
    t1 = 0x1A0u;
    t2 = 0x280u;
    a2 = 0x1C0u;
    a3 = 0x1E0000u;
    a1 = 0x1E0000u;
    v1 = 0x50u;
    a0 = 0x1E0000u;
    v0 = 1u;
    A1_W32B(t3, 4, a2);
    A1_W32B(a1, 4324, v1);
    A1_W32B(a0, 4328, v0);
    A1_W32B(t3, 8, t0);
    A1_W32B(t3, 12, t1);
    A1_W32(t3, t2);
    A1_W32B(a3, 4320, 0u);
    { A1_W32B(t3, 160, 0u); if (xo) { xo[0] = a1_sx64(t0); xo[1] = a1_sx64(t1); xo[2] = a1_sx64(t2); xo[3] = a1_sx64(t3); } return v0; }
L_1c1650:;
    a2 = 1u;
    a3 = 0x200u;
    t0 = 0x1C0u;
    v1 = 0x1E0000u;
    a0 = 0x1E0000u;
    v0 = 2u;
    a1 = 0x1E0000u;
    A1_W32B(t3, 4, t0);
    A1_W32B(v1, 4320, a2);
    A1_W32B(a0, 4324, v0);
    A1_W32(t3, a3);
    A1_W32B(a1, 4328, 0u);
    A1_W32B(t3, 160, a2);
    A1_W32B(t3, 8, a3);
    { A1_W32B(t3, 12, t0); if (xo) { xo[0] = a1_sx64(t0); xo[3] = a1_sx64(t3); } return v0; }
    __builtin_unreachable();
}

// iosChkGpDlTransExec (0x1c35e8): typed by tools/typed_lift2.py (round A3; leaf, extra outputs xo[0]=a0)
inline uint64_t iosChkGpDlTransExec(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint64_t *xo = nullptr)
{
    uint64_t v0 = 0;
    uint32_t v1 = 0;
    uint64_t a0 = 0;
    (v1 = (uint32_t)((int32_t)((int32_t)((uint32_t)4096 << 16))));
    (a0 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL));
    v1 = v1 | 0x9000u;
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B(v1, 0))));
    (v0 = (uint64_t)(RC_U64X(v0) & (uint64_t)(uint16_t)256));
    if ((int64_t)v0 != 0) { (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 1)))); goto L_1c361c; }
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)4096 << 16))));
    (v0 = (uint64_t)(RC_U64X(v0) | (uint64_t)(uint16_t)40960));
    (v1 = (uint32_t)((int32_t)((int32_t)A1_R32B((uint32_t)v0, 0))));
    v1 = v1 & 0x100u;
    if (v1 != 0) { (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 1)))); goto L_1c361c; }
L_1c361c:;
    { (v0 = (uint64_t)((uint64_t)RC_U64X(a0) + (uint64_t)0ULL)); if (xo) { xo[0] = a0; } return v0; }
    __builtin_unreachable();
}

// getTWH (0x1c5e00): typed by tools/typed_lift2.py (round A3; loop leaf)
inline uint64_t getTWH(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint64_t a0)
{
    uint64_t v0 = 0;
    uint64_t v1 = 0;
    uint64_t a1 = 0;
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 1))));
    (a1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 4294967295))));
    (v0 = (uint64_t)(((int64_t)(int64_t)RC_U64X(v0) < (int64_t)(int64_t)a0) ? 1 : 0));
    { const bool c = (int64_t)v0 != 0; (v1 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL)); if (c) goto L_1c5e20; }
    (a1 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL));
    goto L_1c5e4c;
L_1c5e20:;
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v1, 1))));
L_1c5e24:;
    (v0 = (uint64_t)(((int64_t)(int64_t)v1 < (int64_t)(int32_t)11) ? 1 : 0));
    { const bool c = (int64_t)v0 == 0; (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 1)))); if (c) goto L_1c5e50; }
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)SLL32((uint32_t)v0, (uint32_t)v1 & 0x1F))));
    (v0 = (uint64_t)(((int64_t)(int64_t)RC_U64X(v0) < (int64_t)(int64_t)a0) ? 1 : 0));
    if ((int64_t)v0 != 0) { (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v1, 1)))); { A3_BACK(); goto L_1c5e24; } }
    (a1 = (uint64_t)((uint64_t)v1 + (uint64_t)0ULL));
L_1c5e4c:;
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 1))));
L_1c5e50:;
    (v1 = (uint64_t)(((int64_t)0LL < (int64_t)(int64_t)a1) ? 1 : 0));
    { if (RC_U64X(v1) != 0) v0 = (uint64_t)(a1);; return v0; }
    __builtin_unreachable();
}

// isysGetLinkNum (0x1c7948): typed by tools/typed_lift2.py (round A3; loop leaf)
inline uint32_t isysGetLinkNum(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt)
{
    uint32_t v0 = 0;
    uint32_t v1 = 0;
    uint64_t a0 = 0;
    uint32_t a1 = 0;
    v0 = 0x270000u;
    v1 = A1_R32(v0 - 0x2680u);
    { const bool c = v1 == 0; (a0 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL)); if (c) goto L_1c7998; }
    v0 = 0x220000u;
    a1 = v0 - 0x4700u;
    v0 = A1_R16B(v1, 14);
L_1c7968:;
    { const bool c = v0 == 0; (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)a0, 1)))); if (c) goto L_1c7988; }
    v1 = v0;
    v0 = v1 << 1;
    v0 = v0 + v1;
    v0 = v0 << 3;
    v0 = v0 + a1;
    goto L_1c798c;
L_1c7988:;
    v0 = 0u;
L_1c798c:;
    v1 = v0;
    if (v1 != 0) { v0 = A1_R16B(v1, 14); { A3_BACK(); goto L_1c7968; } }
L_1c7998:;
    v0 = 0x36B0u;
    { (v0 = (uint32_t)((int32_t)((int32_t)SUB32(v0, (uint32_t)a0)))); return v0; }
    __builtin_unreachable();
}

// getVIntrDiffTime (0x1c7dc8): typed by tools/typed_lift2.py (round A3; leaf, extra outputs xo[0]=a1)
inline a1::RetVF getVIntrDiffTime(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t a0, uint64_t *xo = nullptr)
{
    float f0 = 0.0f;
    float f1 = 0.0f;
    float f2 = 0.0f;
    float f3 = 0.0f;
    float f4 = 0.0f;
    float f5 = 0.0f;
    uint32_t at = 0;
    uint32_t v0 = 0;
    uint32_t v1 = 0;
    uint32_t a1 = 0;
    v0 = 0x1E0000u;
    a1 = A1_R32(a0);
    v0 = v0 + 0x1314u;
    at = 0x3C880000u;
    at = at | 0xAB85u;
    f3 = a1_flt(at);
    v1 = A1_R32(v0);
    at = 0x426F0000u;
    at = at | 0xC28Fu;
    f2 = a1_flt(at);
    v1 = v1 - a1;
    f4 = a1_flt(A1_R32B(a0, 4));
    f1 = a1_flt(v1);
    { int32_t tmp; std::memcpy(&tmp, &f1, sizeof(tmp)); f1 = ((float)(int32_t)(tmp)); }
    f0 = a1f::mul(f1, f3);
    { int32_t tmp = a1f::cvtw(f0); std::memcpy(&f5, &tmp, sizeof(tmp)); }
    v0 = a1_bits(f5);
    f0 = a1_flt(v0);
    { int32_t tmp; std::memcpy(&tmp, &f0, sizeof(tmp)); f0 = ((float)(int32_t)(tmp)); }
    f2 = a1f::mul(f0, f2);
    f1 = a1f::sub(f1, f2);
    f1 = a1f::mul(f1, f3);
    f0 = a1f::add(f0, f1);
    { f0 = a1f::add(f0, f4); if (xo) { xo[0] = a1_sx64(a1); } return a1::RetVF{(uint64_t)a1_sx64(v0), f0}; }
    __builtin_unreachable();
}

// addVIntrTime (0x1c7e30): typed by tools/typed_lift2.py (round A3; leaf, extra outputs xo[0]=f12 xo[1]=a1)
inline a1::RetVF addVIntrTime(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t a0, float f12, uint64_t *xo = nullptr)
{
    float f0 = 0.0f;
    float f1 = 0.0f;
    float f2 = 0.0f;
    float f3 = 0.0f;
    float f4 = 0.0f;
    uint32_t at = 0;
    uint32_t v0 = 0;
    uint32_t v1 = 0;
    uint32_t a1 = 0;
    v0 = 0x1E0000u;
    a1 = A1_R32(a0);
    v0 = v0 + 0x1314u;
    at = 0x3C880000u;
    at = at | 0xAB85u;
    f4 = a1_flt(at);
    v1 = A1_R32(v0);
    at = 0x426F0000u;
    at = at | 0xC28Fu;
    f3 = a1_flt(at);
    v1 = v1 - a1;
    f0 = a1_flt(v1);
    { int32_t tmp; std::memcpy(&tmp, &f0, sizeof(tmp)); f0 = ((float)(int32_t)(tmp)); }
    f1 = a1f::mul(f0, f4);
    { int32_t tmp = a1f::cvtw(f1); std::memcpy(&f2, &tmp, sizeof(tmp)); }
    v0 = a1_bits(f2);
    f2 = a1_flt(v0);
    { int32_t tmp; std::memcpy(&tmp, &f2, sizeof(tmp)); f2 = ((float)(int32_t)(tmp)); }
    f3 = a1f::mul(f2, f3);
    f0 = a1f::sub(f0, f3);
    f0 = a1f::mul(f0, f4);
    f2 = a1f::add(f2, f0);
    f12 = a1f::sub(f12, f2);
    { A1_W32B(a0, 4, a1_bits(f12)); if (xo) { xo[0] = a1_bits(f12); xo[1] = a1_sx64(a1); } return a1::RetVF{(uint64_t)a1_sx64(v0), f0}; }
    __builtin_unreachable();
}

// ischedStop (0x1c7fb0): typed by tools/typed_lift2.py (round A3; leaf, extra outputs xo[0]=a1)
inline uint32_t ischedStop(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint64_t *xo = nullptr)
{
    uint32_t v0 = 0;
    uint32_t v1 = 0;
    uint32_t a0 = 0;
    uint32_t a1 = 0;
    v0 = 0x270000u;
    a1 = v0 - 0x2660u;
    v1 = A1_R32(a1);
    { const bool c = v1 != 0; a0 = 0xFFFFFF7Fu; if (c) goto L_1c7fd8; }
    v0 = 0x10000000u;
    v0 = v0 | 0x10u;
    v1 = A1_R32(v0);
    v1 = v1 & a0;
    A1_W32(v0, v1);
L_1c7fd8:;
    v0 = A1_R32(a1);
    v0 = v0 + 1u;
    A1_W32(a1, v0);
    { if (xo) { xo[0] = a1_sx64(a1); } return v0; }
    __builtin_unreachable();
}

// get_next_letter_in_post (0x1c8910): typed by tools/typed_lift2.py (round A3; leaf, extra outputs xo[0]=v1)
inline uint32_t get_next_letter_in_post(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t a0, uint64_t *xo = nullptr)
{
    uint32_t v0 = 0;
    uint32_t v1 = 0;
    v1 = A1_R32B(a0, 16);
    if (v1 == 0) { v1 = 0u; goto L_1c892c; }
    A1_W32B(a0, 12, v1);
    v0 = A1_R32B(v1, 4);
    v1 = A1_R32(v1);
    A1_W32B(a0, 16, v0);
L_1c892c:;
    { v0 = v1; if (xo) { xo[0] = a1_sx64(v1); } return v0; }
    __builtin_unreachable();
}

// entry_new_letter_type (0x1c8af0): typed by tools/typed_lift2.py (round A3; loop leaf)
inline uint64_t entry_new_letter_type(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t a0)
{
    uint64_t v0 = 0;
    uint64_t v1 = 0;
    uint32_t a1 = 0;
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)39 << 16))));
    (v1 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL));
    (a1 = (uint32_t)((int32_t)((int32_t)ADD32((uint32_t)v0, 2584))));
L_1c8b00:;
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B(a1, 0))));
    if ((int64_t)v0 != 0) { (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v1, 1)))); goto L_1c8b18; }
    A1_W32(a1, a0);
    { (v0 = (uint64_t)((uint64_t)v1 + (uint64_t)0ULL)); return v0; }
L_1c8b18:;
    (v0 = (uint64_t)(((int64_t)(int64_t)v1 < (int64_t)(int32_t)20) ? 1 : 0));
    { const bool c = (int64_t)v0 != 0; a1 = a1 + 4u; if (c) { A3_BACK(); goto L_1c8b00; } }
    { (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 4294967295)))); return v0; }
    __builtin_unreachable();
}

// init_letter_type_store (0x1c8b30): typed by tools/typed_lift2.py (round A3; loop leaf)
inline uint32_t init_letter_type_store(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt)
{
    uint32_t v0 = 0;
    uint32_t v1 = 0;
    v0 = 0x270000u;
    v1 = 0x13u;
    v0 = v0 + 0xA18u;
    v0 = v0 + 0x4Cu;
L_1c8b40:;
    v1 = v1 - 1u;
    A1_W32(v0, 0u);
    { const bool c = (int32_t)v1 >= 0; v0 = v0 - 4u; if (c) { A3_BACK(); goto L_1c8b40; } }
    { return v0; }
    __builtin_unreachable();
}

// kernel_sub_001C8B98 (0x1c8b98): typed by tools/typed_lift2.py (round A3; loop leaf)
inline uint32_t kernel_sub_001C8B98(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t a0)
{
    uint32_t v0 = 0;
    uint32_t v1 = 0;
    uint32_t a1 = 0;
    uint32_t a2 = 0;
    uint32_t a3 = 0;
    uint32_t t0 = 0;
    uint32_t t1 = 0;
    uint32_t t2 = 0;
    uint64_t t3 = 0;
    uint64_t t4 = 0;
    uint32_t t5 = 0;
    uint64_t t6 = 0;
    t5 = 0x270000u;
    a3 = 0x270000u;
    v1 = t5 + 0xA6Cu;
    v0 = (uint32_t)((int32_t)a0 >> 8);
    a1 = a3 + 0xA68u;
    a2 = 0x270000u;
    A1_W32(a1, v0);
    a0 = a0 & 0xFFu;
    A1_W32(v1, 0u);
    (t3 = (uint64_t)((uint64_t)a1_sx64(v1) + (uint64_t)0ULL));
    a2 = a2 + 0xA70u;
    (t4 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL));
    (t6 = (uint64_t)((uint64_t)a1_sx64(t5) + (uint64_t)0ULL));
    v0 = (int32_t)a0 < 128 ? 1u : 0u;
L_1c8bd0:;
    if (v0 != 0) { (v0 = (uint32_t)((int32_t)((int32_t)A1_R32B((uint32_t)t3, 0)))); goto L_1c8d3c; }
    v0 = a0 - 0xA1u;
    v0 = v0 < 0x5Eu ? 1u : 0u;
    { const bool c = v0 == 0; v0 = 0x8Eu; if (c) goto L_1c8cb8; }
    v1 = A1_R32(a1);
    v0 = (uint32_t)((int32_t)v1 >> 8);
    t0 = v1 & 0xFFu;
    { const bool c = t0 != 0; A1_W32(a1, v0); if (c) goto L_1c8c10; }
    (v0 = (uint32_t)((int32_t)((int32_t)A1_R32B((uint32_t)t3, 0))));
    v0 = v0 << 8;
    v0 = v0 | a0;
    { A1_W32B((uint32_t)t3, 0, v0); return v0; }
L_1c8c10:;
    v0 = t0 - 0xA1u;
    v0 = v0 < 0x5Eu ? 1u : 0u;
    if (v0 == 0) { (v0 = (uint32_t)((int32_t)((int32_t)A1_R32B((uint32_t)t3, 0)))); goto L_1c8ca0; }
    a0 = a0 & 0x7Fu;
    v0 = a0 & 1u;
    v0 = v0 & 0xFFu;
    { const bool c = v0 == 0; t0 = v1 & 0x7Fu; if (c) goto L_1c8c58; }
    t0 = t0 + 0x1Fu;
    v0 = a0 >> 1;
    v1 = t0 < 0x7Fu ? 1u : 0u;
    { const bool c = v1 != 0; a0 = v0 + 0x71u; if (c) goto L_1c8c68; }
    v0 = t0 + 1u;
    t0 = v0 & 0xFFu;
    goto L_1c8c68;
L_1c8c58:;
    v0 = a0 >> 1;
    v1 = t0 + 0x7Eu;
    a0 = v0 + 0x70u;
    t0 = v1 & 0xFFu;
L_1c8c68:;
    v0 = a0 < 0xA0u ? 1u : 0u;
    if (v0 != 0) { (v0 = (uint32_t)((int32_t)((int32_t)A1_R32B((uint32_t)t3, 0)))); goto L_1c8c80; }
    v0 = a0 + 0x40u;
    a0 = v0 & 0xFFu;
    (v0 = (uint32_t)((int32_t)((int32_t)A1_R32B((uint32_t)t3, 0))));
L_1c8c80:;
    a0 = a0 & 0xFFu;
    A1_W32(a2, t0);
    v0 = v0 << 8;
    v0 = v0 | a0;
    v0 = v0 << 8;
    v0 = v0 | t0;
    goto L_1c8d44;
L_1c8ca0:;
    v0 = v0 << 8;
    v0 = v0 | a0;
    v0 = v0 << 8;
    v0 = v0 | t0;
    goto L_1c8d44;
L_1c8cb8:;
    { const bool c = a0 != v0; v0 = 0x8Fu; if (c) goto L_1c8ce0; }
    v0 = A1_R32(a1);
    a0 = v0 & 0xFFu;
    v0 = (uint32_t)((int32_t)v0 >> 8);
    { const bool c = a0 != 0; A1_W32(a1, v0); if (c) goto L_1c8d38; }
    { (v0 = (uint32_t)((int32_t)((int32_t)A1_R32B((uint32_t)t3, 0)))); return v0; }
L_1c8ce0:;
    if (a0 != v0) { (v0 = (uint32_t)((int32_t)((int32_t)A1_R32B((uint32_t)t3, 0)))); goto L_1c8d3c; }
    t0 = a3 + 0xA68u;
    v1 = A1_R32(t0);
    t1 = (uint32_t)((int32_t)v1 >> 8);
    a0 = v1 & 0xFFu;
    { const bool c = a0 == 0; A1_W32(t0, t1); if (c) goto L_1c8d5c; }
    (t2 = (uint32_t)((int32_t)((int32_t)ADD32((uint32_t)t6, 2668))));
    v1 = (uint32_t)((int32_t)v1 >> 16);
    v0 = A1_R32(t2);
    A1_W32(t0, v1);
    v0 = v0 << 8;
    v1 = v0 | a0;
    a0 = t1 & 0xFFu;
    A1_W32(t2, v1);
    { const bool c = a0 == 0; v0 = v1; if (c) goto L_1c8d7c; }
    v0 = v1 << 8;
    v0 = v0 | a0;
    A1_W32(t2, v0);
    goto L_1c8d48;
L_1c8d38:;
    (v0 = (uint32_t)((int32_t)((int32_t)A1_R32B((uint32_t)t3, 0))));
L_1c8d3c:;
    v0 = v0 << 8;
    v0 = v0 | a0;
L_1c8d44:;
    A1_W32B((uint32_t)t3, 0, v0);
L_1c8d48:;
    v0 = A1_R32(a1);
    a0 = v0 & 0xFFu;
    v0 = (uint32_t)((int32_t)v0 >> 8);
    { const bool c = a0 != 0; A1_W32(a1, v0); if (c) goto L_1c8d68; }
L_1c8d5c:;
    { (v0 = (uint32_t)((int32_t)((int32_t)A1_R32B((uint32_t)t3, 0)))); return v0; }
L_1c8d68:;
    (t4 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)t4, 1))));
    (v0 = (uint32_t)(((int64_t)(int64_t)RC_U64X(t4) < (int64_t)(int32_t)2) ? 1 : 0));
    if (v0 != 0) { v0 = (int32_t)a0 < 128 ? 1u : 0u; { A3_BACK(); goto L_1c8bd0; } }
    v0 = A1_R32B(t5, 2668);
L_1c8d7c:;
    { return v0; }
    __builtin_unreachable();
}

// iosGetFontInfo (0x1c9678): typed by tools/typed_lift2.py (round A3; leaf, extra outputs xo[0]=a1 xo[1]=a2 xo[2]=a3)
inline uint64_t iosGetFontInfo(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint64_t a0, uint64_t a1, uint64_t *xo = nullptr)
{
    uint64_t at = 0;
    uint64_t v0 = 0;
    uint64_t v1 = 0;
    uint64_t a2 = 0;
    uint64_t a3 = 0;
    (a1 = (uint64_t)((int64_t)(int32_t)((int32_t)SLL32((uint32_t)a1, 5))));
    (at = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)39 << 16))));
    (at = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)at, (uint32_t)a1))));
    { uint32_t addr = ADD32((uint32_t)at, 2687); uint32_t aligned_addr = addr & ~7u; uint32_t offset = addr & 7u; uint64_t mem = A1_R64(aligned_addr); uint32_t shift = (7u - offset) << 3; uint64_t keepMask = (shift == 0) ? 0ull : ((1ull << shift) - 1ull); (v0 = (uint64_t)((v0 & keepMask) | (mem << shift))); }
    (at = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)39 << 16))));
    (at = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)at, (uint32_t)a1))));
    { uint32_t addr = ADD32((uint32_t)at, 2680); uint32_t aligned_addr = addr & ~7u; uint32_t offset = addr & 7u; uint64_t mem = A1_R64(aligned_addr); uint32_t shift = offset << 3; uint64_t keepMask = (offset == 0) ? 0ull : (0xFFFFFFFFFFFFFFFFull << ((8u - offset) << 3)); (v0 = (uint64_t)((v0 & keepMask) | (mem >> shift))); }
    (at = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)39 << 16))));
    (at = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)at, (uint32_t)a1))));
    { uint32_t addr = ADD32((uint32_t)at, 2695); uint32_t aligned_addr = addr & ~7u; uint32_t offset = addr & 7u; uint64_t mem = A1_R64(aligned_addr); uint32_t shift = (7u - offset) << 3; uint64_t keepMask = (shift == 0) ? 0ull : ((1ull << shift) - 1ull); (v1 = (uint64_t)((v1 & keepMask) | (mem << shift))); }
    (at = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)39 << 16))));
    (at = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)at, (uint32_t)a1))));
    { uint32_t addr = ADD32((uint32_t)at, 2688); uint32_t aligned_addr = addr & ~7u; uint32_t offset = addr & 7u; uint64_t mem = A1_R64(aligned_addr); uint32_t shift = offset << 3; uint64_t keepMask = (offset == 0) ? 0ull : (0xFFFFFFFFFFFFFFFFull << ((8u - offset) << 3)); (v1 = (uint64_t)((v1 & keepMask) | (mem >> shift))); }
    (at = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)39 << 16))));
    (at = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)at, (uint32_t)a1))));
    { uint32_t addr = ADD32((uint32_t)at, 2703); uint32_t aligned_addr = addr & ~7u; uint32_t offset = addr & 7u; uint64_t mem = A1_R64(aligned_addr); uint32_t shift = (7u - offset) << 3; uint64_t keepMask = (shift == 0) ? 0ull : ((1ull << shift) - 1ull); (a2 = (uint64_t)((a2 & keepMask) | (mem << shift))); }
    (at = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)39 << 16))));
    (at = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)at, (uint32_t)a1))));
    { uint32_t addr = ADD32((uint32_t)at, 2696); uint32_t aligned_addr = addr & ~7u; uint32_t offset = addr & 7u; uint64_t mem = A1_R64(aligned_addr); uint32_t shift = offset << 3; uint64_t keepMask = (offset == 0) ? 0ull : (0xFFFFFFFFFFFFFFFFull << ((8u - offset) << 3)); (a2 = (uint64_t)((a2 & keepMask) | (mem >> shift))); }
    (at = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)39 << 16))));
    (at = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)at, (uint32_t)a1))));
    { uint32_t addr = ADD32((uint32_t)at, 2711); uint32_t aligned_addr = addr & ~7u; uint32_t offset = addr & 7u; uint64_t mem = A1_R64(aligned_addr); uint32_t shift = (7u - offset) << 3; uint64_t keepMask = (shift == 0) ? 0ull : ((1ull << shift) - 1ull); (a3 = (uint64_t)((a3 & keepMask) | (mem << shift))); }
    (at = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)39 << 16))));
    (at = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)at, (uint32_t)a1))));
    { uint32_t addr = ADD32((uint32_t)at, 2704); uint32_t aligned_addr = addr & ~7u; uint32_t offset = addr & 7u; uint64_t mem = A1_R64(aligned_addr); uint32_t shift = offset << 3; uint64_t keepMask = (offset == 0) ? 0ull : (0xFFFFFFFFFFFFFFFFull << ((8u - offset) << 3)); (a3 = (uint64_t)((a3 & keepMask) | (mem >> shift))); }
    { uint32_t addr = ADD32((uint32_t)a0, 7); uint32_t aligned_addr = addr & ~7u; uint32_t offset = addr & 7u; uint32_t shift = (7u - offset) << 3; uint64_t mask = 0xFFFFFFFFFFFFFFFFull >> shift; uint64_t old_data = A1_R64(aligned_addr); uint64_t val = v0; uint64_t new_data = (old_data & ~mask) | ((val >> shift) & mask); A1_W64(aligned_addr, new_data); }
    { uint32_t addr = ADD32((uint32_t)a0, 0); uint32_t aligned_addr = addr & ~7u; uint32_t offset = addr & 7u; uint32_t shift = offset << 3; uint64_t mask = 0xFFFFFFFFFFFFFFFFull << shift; uint64_t old_data = A1_R64(aligned_addr); uint64_t val = v0; uint64_t new_data = (old_data & ~mask) | ((val << shift) & mask); A1_W64(aligned_addr, new_data); }
    { uint32_t addr = ADD32((uint32_t)a0, 15); uint32_t aligned_addr = addr & ~7u; uint32_t offset = addr & 7u; uint32_t shift = (7u - offset) << 3; uint64_t mask = 0xFFFFFFFFFFFFFFFFull >> shift; uint64_t old_data = A1_R64(aligned_addr); uint64_t val = v1; uint64_t new_data = (old_data & ~mask) | ((val >> shift) & mask); A1_W64(aligned_addr, new_data); }
    { uint32_t addr = ADD32((uint32_t)a0, 8); uint32_t aligned_addr = addr & ~7u; uint32_t offset = addr & 7u; uint32_t shift = offset << 3; uint64_t mask = 0xFFFFFFFFFFFFFFFFull << shift; uint64_t old_data = A1_R64(aligned_addr); uint64_t val = v1; uint64_t new_data = (old_data & ~mask) | ((val << shift) & mask); A1_W64(aligned_addr, new_data); }
    { uint32_t addr = ADD32((uint32_t)a0, 23); uint32_t aligned_addr = addr & ~7u; uint32_t offset = addr & 7u; uint32_t shift = (7u - offset) << 3; uint64_t mask = 0xFFFFFFFFFFFFFFFFull >> shift; uint64_t old_data = A1_R64(aligned_addr); uint64_t val = a2; uint64_t new_data = (old_data & ~mask) | ((val >> shift) & mask); A1_W64(aligned_addr, new_data); }
    { uint32_t addr = ADD32((uint32_t)a0, 16); uint32_t aligned_addr = addr & ~7u; uint32_t offset = addr & 7u; uint32_t shift = offset << 3; uint64_t mask = 0xFFFFFFFFFFFFFFFFull << shift; uint64_t old_data = A1_R64(aligned_addr); uint64_t val = a2; uint64_t new_data = (old_data & ~mask) | ((val << shift) & mask); A1_W64(aligned_addr, new_data); }
    { uint32_t addr = ADD32((uint32_t)a0, 31); uint32_t aligned_addr = addr & ~7u; uint32_t offset = addr & 7u; uint32_t shift = (7u - offset) << 3; uint64_t mask = 0xFFFFFFFFFFFFFFFFull >> shift; uint64_t old_data = A1_R64(aligned_addr); uint64_t val = a3; uint64_t new_data = (old_data & ~mask) | ((val >> shift) & mask); A1_W64(aligned_addr, new_data); }
    { uint32_t addr = ADD32((uint32_t)a0, 24); uint32_t aligned_addr = addr & ~7u; uint32_t offset = addr & 7u; uint32_t shift = offset << 3; uint64_t mask = 0xFFFFFFFFFFFFFFFFull << shift; uint64_t old_data = A1_R64(aligned_addr); uint64_t val = a3; uint64_t new_data = (old_data & ~mask) | ((val << shift) & mask); A1_W64(aligned_addr, new_data); }
    { if (xo) { xo[0] = a1; xo[1] = a2; xo[2] = a3; } return v0; }
    __builtin_unreachable();
}

// sndSepPlayerSrh (0x1cab80): typed by tools/typed_lift2.py (round A3; loop leaf, extra outputs xo[0]=a3 xo[1]=t0 xo[2]=t1 xo[3]=t2)
inline uint64_t sndSepPlayerSrh(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint64_t a0, uint64_t a1, uint64_t a2, uint64_t *xo = nullptr)
{
    uint64_t v0 = 0;
    uint32_t v1 = 0;
    uint64_t a3 = 0;
    uint32_t t0 = 0;
    uint64_t t1 = 0;
    uint64_t t2 = 0;
    t0 = 0x270000u;
    (t1 = (uint64_t)((uint64_t)a0 + (uint64_t)0ULL));
    v1 = t0 + 0xBD0u;
    (t2 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B(v1, 0))));
    { const bool c = (int64_t)v0 != 0; (a3 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL)); if (c) goto L_1cabc8; }
    (a3 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)a3, 1))));
L_1caba0:;
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)SLL32((uint32_t)a3, 3))));
    (v1 = (uint32_t)(((int64_t)(int64_t)a3 < (int64_t)(int32_t)32) ? 1 : 0));
    { const bool c = v1 == 0; (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)SUB32((uint32_t)v0, (uint32_t)a3)))); if (c) goto L_1cabfc; }
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)SLL32((uint32_t)v0, 4))));
    v1 = t0 + 0xBD0u;
    (v1 = (uint32_t)((int32_t)((int32_t)ADD32((uint32_t)v0, v1))));
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B(v1, 0))));
    if ((int64_t)a0 == 0) { (a3 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)a3, 1)))); { A3_BACK(); goto L_1caba0; } }
L_1cabc8:;
    (v0 = (uint64_t)((int64_t)(int32_t)((int16_t)A1_R16B(v1, 20))));
    if ((int64_t)v0 != (int64_t)a1) { (a3 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)a3, 1)))); { A3_BACK(); goto L_1caba0; } }
    (v0 = (uint64_t)((int64_t)(int32_t)((int16_t)A1_R16B(v1, 22))));
    if ((int64_t)v0 != (int64_t)a2) { (a3 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)a3, 1)))); { A3_BACK(); goto L_1caba0; } }
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B(v1, 16))));
    if ((int64_t)v0 == 0) { (a3 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)a3, 1)))); { A3_BACK(); goto L_1caba0; } }
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)v0, 4))));
    if ((int64_t)v0 != (int64_t)t1) { (a3 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)a3, 1)))); { A3_BACK(); goto L_1caba0; } }
    (t2 = (uint64_t)((uint64_t)a1_sx64(v1) + (uint64_t)0ULL));
L_1cabfc:;
    { (v0 = (uint64_t)((uint64_t)t2 + (uint64_t)0ULL)); if (xo) { xo[0] = a3; xo[1] = a1_sx64(t0); xo[2] = t1; xo[3] = t2; } return v0; }
    __builtin_unreachable();
}

// sndSepPlayerSrh2 (0x1cac08): typed by tools/typed_lift2.py (round A3; loop leaf, extra outputs xo[0]=t1)
inline uint64_t sndSepPlayerSrh2(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint64_t a0, uint64_t a1, uint64_t *xo = nullptr)
{
    uint64_t v0 = 0;
    uint32_t v1 = 0;
    uint64_t a2 = 0;
    uint64_t a3 = 0;
    uint32_t t0 = 0;
    uint64_t t1 = 0;
    t0 = 0x270000u;
    (a3 = (uint64_t)((uint64_t)a0 + (uint64_t)0ULL));
    v1 = t0 + 0xBD0u;
    (t1 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B(v1, 0))));
    { const bool c = (int64_t)v0 != 0; (a2 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL)); if (c) goto L_1cac50; }
    (a2 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)a2, 1))));
L_1cac28:;
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)SLL32((uint32_t)a2, 3))));
    (v1 = (uint32_t)(((int64_t)(int64_t)a2 < (int64_t)(int32_t)32) ? 1 : 0));
    { const bool c = v1 == 0; (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)SUB32((uint32_t)v0, (uint32_t)a2)))); if (c) goto L_1cac78; }
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)SLL32((uint32_t)v0, 4))));
    v1 = t0 + 0xBD0u;
    (v1 = (uint32_t)((int32_t)((int32_t)ADD32((uint32_t)v0, v1))));
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B(v1, 0))));
    if ((int64_t)a0 == 0) { (a2 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)a2, 1)))); { A3_BACK(); goto L_1cac28; } }
L_1cac50:;
    (v0 = (uint64_t)((int64_t)(int32_t)((int16_t)A1_R16B(v1, 20))));
    if ((int64_t)v0 != (int64_t)a1) { (a2 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)a2, 1)))); { A3_BACK(); goto L_1cac28; } }
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B(v1, 16))));
    if ((int64_t)v0 == 0) { (a2 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)a2, 1)))); { A3_BACK(); goto L_1cac28; } }
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)v0, 4))));
    if ((int64_t)v0 != (int64_t)a3) { (a2 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)a2, 1)))); { A3_BACK(); goto L_1cac28; } }
    (t1 = (uint64_t)((uint64_t)a1_sx64(v1) + (uint64_t)0ULL));
L_1cac78:;
    { (v0 = (uint64_t)((uint64_t)t1 + (uint64_t)0ULL)); if (xo) { xo[0] = t1; } return v0; }
    __builtin_unreachable();
}

// sndSepSetVol (0x1cac98): typed by tools/typed_lift2.py (round A3; leaf, extra outputs xo[0]=a1 xo[1]=a2)
inline a1::RetVF sndSepSetVol(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t a0, float f12, uint64_t *xo = nullptr)
{
    float f0 = 0.0f;
    float f1 = 0.0f;
    uint32_t at = 0;
    uint64_t v0 = 0;
    uint64_t v1 = 0;
    uint64_t a1 = 0;
    uint32_t a2 = 0;
    at = 0x42FE0000u;
    f1 = a1_flt(at);
    a2 = 0x7Fu;
    f0 = a1_flt(A1_R32B(a0, 28));
    f12 = a1f::mul(f12, f1);
    f12 = a1f::mul(f12, f0);
    { int32_t tmp = a1f::cvtw(f12); std::memcpy(&f0, &tmp, sizeof(tmp)); }
    { uint32_t bits; std::memcpy(&bits, &f0, sizeof(bits)); (v0 = (uint64_t)((int64_t)(int32_t)(bits))); }
    (v1 = (uint64_t)(((int64_t)(int64_t)RC_U64X(v0) < (int64_t)(int32_t)128) ? 1 : 0));
    (a1 = (uint64_t)(((int64_t)(int64_t)RC_U64X(v0) < (int64_t)(int32_t)0) ? 1 : 0));
    if (RC_U64X(a1) != 0) v0 = (uint64_t)0;;
    (v1 = (uint64_t)(RC_U64X(v1) ^ (uint64_t)(uint16_t)0));
    if (RC_U64X(v1) == 0) v0 = (uint64_t)(a1_sx64(a2));;
    { A1_W16B(a0, 40, (uint16_t)(uint32_t)v0); if (xo) { xo[0] = a1; xo[1] = a1_sx64(a2); } return a1::RetVF{(uint64_t)v0, f0}; }
    __builtin_unreachable();
}

// sndSepSetPan (0x1cacd8): typed by tools/typed_lift2.py (round A3; leaf)
inline a1::RetVF sndSepSetPan(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t a0, float f12)
{
    float f0 = 0.0f;
    float f1 = 0.0f;
    uint32_t at = 0;
    uint32_t v0 = 0;
    uint32_t fcr31 = 0;
    at = 0x3F800000u;
    f0 = a1_flt(at);
    fcr31 = (a1f::lt(f0, f12)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
    if ((fcr31 & 0x800000u) != 0u) { f12 = ((float)(f0)); goto L_1cad00; }
    at = 0xBF800000u;
    f0 = a1_flt(at);
    fcr31 = (a1f::lt(f12, f0)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
    if ((fcr31 & 0x800000u) != 0u) { f12 = ((float)(f0)); goto L_1cad00; }
L_1cad00:;
    at = 0x427C0000u;
    f0 = a1_flt(at);
    at = 0x42800000u;
    f1 = a1_flt(at);
    f0 = a1f::mul(f12, f0);
    f0 = a1f::addFinite(f0, f1);
    { int32_t tmp = a1f::cvtw(f0); std::memcpy(&f1, &tmp, sizeof(tmp)); }
    v0 = a1_bits(f1);
    f1 = a1_flt(v0);
    { int32_t tmp; std::memcpy(&tmp, &f1, sizeof(tmp)); f1 = ((float)(int32_t)(tmp)); }
    { A1_W32B(a0, 36, a1_bits(f1)); return a1::RetVF{(uint64_t)a1_sx64(v0), f0}; }
    __builtin_unreachable();
}

// sndSepStopTypeAll (0x1cadf8): typed by tools/typed_lift2.py (round A3; loop leaf)
inline uint64_t sndSepStopTypeAll(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint64_t a0)
{
    uint64_t v0 = 0;
    uint64_t v1 = 0;
    uint64_t a1 = 0;
    uint64_t a2 = 0;
    uint64_t a3 = 0;
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)39 << 16))));
    (a3 = (uint64_t)(0ULL | (uint64_t)(uint16_t)32768));
    (a3 = (uint64_t)(RC_U64X(a3) << 17));
    (a1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v0, 3024))));
    (a2 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 31))));
L_1cae10:;
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)a1, 0))));
    if ((int64_t)v0 == 0) { (a2 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)a2, 4294967295)))); goto L_1cae38; }
    (v1 = (uint64_t)(A1_R64B((uint32_t)a1, 8)));
    (v0 = (uint64_t)(v1 >> (32 + 2)));
    (v0 = (uint64_t)(v0 & (uint64_t)(uint16_t)7));
    { const bool c = (int64_t)v0 != (int64_t)a0; (v1 = (uint64_t)(v1 | a3)); if (c) goto L_1cae34; }
    A1_W64B((uint32_t)a1, 8, v1);
L_1cae34:;
    (a2 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)a2, 4294967295))));
L_1cae38:;
    { const bool c = (int32_t)a2 >= 0; (a1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)a1, 112)))); if (c) { A3_BACK(); goto L_1cae10; } }
    { return v0; }
    __builtin_unreachable();
}

// sndVabCreate (0x1caf10): typed by tools/typed_lift2.py (round A3; loop leaf, extra outputs xo[0]=a3 xo[1]=t0 xo[2]=t1 xo[3]=t2)
inline uint64_t sndVabCreate(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint64_t *xo = nullptr)
{
    uint64_t v0 = 0;
    uint64_t v1 = 0;
    uint64_t a0 = 0;
    uint64_t a1 = 0;
    uint32_t a2 = 0;
    uint64_t a3 = 0;
    uint64_t t0 = 0;
    uint64_t t1 = 0;
    uint32_t t2 = 0;
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)39 << 16))));
    (t0 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL));
    (t2 = (uint32_t)((int32_t)((int32_t)ADD32((uint32_t)v0, 6616))));
    (a0 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL));
    (a3 = (uint64_t)((uint64_t)a1_sx64(t2) + (uint64_t)0ULL));
    (t1 = (uint64_t)((uint64_t)a1_sx64(t2) + (uint64_t)0ULL));
L_1caf28:;
    (a2 = (uint32_t)((int32_t)((int32_t)A1_R32B((uint32_t)a3, 0))));
    (a3 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)a3, 28))));
    (a1 = (uint64_t)((uint64_t)a0 + (uint64_t)0ULL));
    (v0 = (uint64_t)(a1_sx64(a2) & (uint64_t)(uint16_t)1));
    { const bool c = (int64_t)v0 == 0; (v1 = (uint64_t)((uint64_t)t1 + (uint64_t)0ULL)); if (c) goto L_1caf60; }
    (t0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)t0, 1))));
    (t1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v1, 28))));
    (v0 = (uint64_t)(((int64_t)(int64_t)RC_U64X(t0) < (int64_t)(int32_t)20) ? 1 : 0));
    { const bool c = (int64_t)v0 != 0; (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)a1, 28)))); if (c) { A3_BACK(); goto L_1caf28; } }
    { (v0 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL)); if (xo) { xo[0] = a3; xo[1] = t0; xo[2] = t1; xo[3] = a1_sx64(t2); } return v0; }
L_1caf60:;
    (a1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)a0, t2))));
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(t2, (uint32_t)a0))));
    (v0 = (uint64_t)(a1_sx64(a2) | (uint64_t)(uint16_t)1));
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 4294967295))));
    A1_W32B((uint32_t)t1, 0, (uint32_t)v0);
    (v0 = (uint64_t)((uint64_t)t1 + (uint64_t)0ULL));
    A1_W32B((uint32_t)a0, 4, (uint32_t)v1);
    { A1_W32B((uint32_t)a1, 16, 0u); if (xo) { xo[0] = a3; xo[1] = t0; xo[2] = t1; xo[3] = a1_sx64(t2); } return v0; }
    __builtin_unreachable();
}

// sndVabInit (0x1cb170): typed by tools/typed_lift2.py (round A3; loop leaf)
inline uint32_t sndVabInit(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt)
{
    uint32_t v0 = 0;
    uint32_t v1 = 0;
    uint32_t a0 = 0;
    uint32_t a1 = 0;
    v0 = 0x270000u;
    a1 = 0xFFFFFFFEu;
    v1 = v0 + 0x19D8u;
    a0 = 0x13u;
L_1cb180:;
    v0 = A1_R32(v1);
    a0 = a0 - 1u;
    v0 = v0 & a1;
    A1_W32(v1, v0);
    { const bool c = (int32_t)a0 >= 0; v1 = v1 + 0x1Cu; if (c) { A3_BACK(); goto L_1cb180; } }
    { return v0; }
    __builtin_unreachable();
}

// AddStrHashKey (0x1cbd70): typed by tools/typed_lift2.py (round A3; leaf, extra outputs xo[0]=a2 xo[1]=a3)
inline uint32_t AddStrHashKey(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t a0, uint32_t a1, uint64_t *xo = nullptr)
{
    uint32_t v0 = 0;
    uint32_t v1 = 0;
    uint32_t a2 = 0;
    uint32_t a3 = 0;
    v0 = A1_R32(a0);
    a2 = A1_R32(a1);
    v0 = v0 - 1u;
    a3 = A1_R32B(a0, 4);
    v0 = a2 & v0;
    v0 = v0 << 2;
    v0 = v0 + a3;
    v1 = A1_R32(v0);
    A1_W32B(a1, 12, v1);
    v0 = A1_R32(a0);
    v1 = A1_R32B(a0, 4);
    v0 = v0 - 1u;
    a2 = a2 & v0;
    a2 = a2 << 2;
    a2 = a2 + v1;
    { A1_W32(a2, a1); if (xo) { xo[0] = a1_sx64(a2); xo[1] = a1_sx64(a3); } return v0; }
    __builtin_unreachable();
}

// Sg2CtrlEntryTickProcById (0x1cc0c8): typed by tools/typed_lift2.py (round A3; leaf, extra outputs xo[0]=a0 xo[1]=a2 xo[2]=a3 xo[3]=t0)
inline uint64_t Sg2CtrlEntryTickProcById(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint64_t a0, uint32_t a1, uint64_t *xo = nullptr)
{
    uint64_t v0 = 0;
    uint32_t v1 = 0;
    uint32_t a2 = 0;
    uint64_t a3 = 0;
    uint32_t t0 = 0;
    (a3 = (uint64_t)((uint64_t)a0 + (uint64_t)0ULL));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)39 << 16))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v0, 15680))));
    (v1 = (uint32_t)((int32_t)((int32_t)SLL32((uint32_t)a3, 3))));
    (a2 = (uint32_t)((int32_t)((int32_t)ADD32(v1, (uint32_t)v0))));
    (t0 = (uint32_t)((int32_t)((int32_t)ADD32((uint32_t)v0, v1))));
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B(a2, 0))));
    v1 = 0x1D0000u;
    v1 = v1 - 0x3FD8u;
    { const bool c = (int64_t)a0 != (int64_t)(int32_t)v1; (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 4294967295)))); if (c) goto L_1cc100; }
    A1_W32(a2, a1);
    (v0 = (uint64_t)((uint64_t)a3 + (uint64_t)0ULL));
    A1_W32B(t0, 4, 0u);
L_1cc100:;
    { if (xo) { xo[0] = a0; xo[1] = a1_sx64(a2); xo[2] = a3; xo[3] = a1_sx64(t0); } return v0; }
    __builtin_unreachable();
}

// Sg2DspInit (0x1cc7f8): typed by tools/typed_lift2.py (round A3; loop leaf)
inline uint32_t Sg2DspInit(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt)
{
    uint32_t v0 = 0;
    uint32_t v1 = 0;
    v0 = 0x270000u;
    v1 = 1u;
    v0 = v0 + 0x3DC8u;
L_1cc808:;
    v1 = v1 - 1u;
    A1_W32(v0, 0u);
    A1_W32B(v0, 4, 0u);
    A1_W16B(v0, 12, (uint16_t)0u);
    A1_W16B(v0, 14, (uint16_t)0u);
    A1_W32B(v0, 16, 0u);
    A1_W32B(v0, 20, 0u);
    { const bool c = (int32_t)v1 >= 0; v0 = v0 + 0x18u; if (c) { A3_BACK(); goto L_1cc808; } }
    { v0 = 0u; return v0; }
    __builtin_unreachable();
}

// ps2_____VolPerBackup (0x1cd448): typed by tools/typed_lift2.py (round A3; loop leaf)
inline uint32_t ps2_____VolPerBackup(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t a0, uint64_t a1)
{
    uint32_t v0 = 0;
    uint32_t v1 = 0;
    if ((int64_t)a1 == 0) { v1 = a0 + 0x24Cu; goto L_1cd460; }
    v1 = a0 + 0x28Cu;
    a0 = a0 + 0x24Cu;
    goto L_1cd464;
L_1cd460:;
    a0 = a0 + 0x28Cu;
L_1cd464:;
    (a1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 7))));
L_1cd468:;
    v0 = A1_R32(a0);
    a0 = a0 + 4u;
    (a1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)a1, 4294967295))));
    A1_W32(v1, v0);
    v1 = v1 + 4u;
    v0 = A1_R32(a0);
    a0 = a0 + 4u;
    A1_W32(v1, v0);
    { const bool c = (int32_t)a1 >= 0; v1 = v1 + 4u; if (c) { A3_BACK(); goto L_1cd468; } }
    { return v0; }
    __builtin_unreachable();
}

// kernel_sub_001CDDE8 (0x1cdde8): typed by tools/typed_lift2.py (round A3; loop leaf, extra outputs xo[0]=a3 xo[1]=t0 xo[2]=t1 xo[3]=t2 xo[4]=t3)
inline uint64_t kernel_sub_001CDDE8(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint64_t a0, uint64_t *xo = nullptr)
{
    uint32_t at = 0;
    uint64_t v0 = 0;
    uint64_t v1 = 0;
    uint32_t a1 = 0;
    uint32_t a2 = 0;
    uint32_t a3 = 0;
    uint32_t t0 = 0;
    uint32_t t1 = 0;
    uint64_t t2 = 0;
    uint64_t t3 = 0;
    uint32_t t4 = 0;
    uint32_t t5 = 0;
    uint32_t t6 = 0;
    uint32_t t7 = 0;
    uint32_t t8 = 0;
    (t3 = (uint64_t)((uint64_t)a0 + (uint64_t)0ULL));
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 4294967168))));
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)SLL32((uint32_t)t3, 2))));
    a3 = 1u;
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v1, (uint32_t)t3))));
    t0 = 0x7Fu;
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)SLL32((uint32_t)v1, 5))));
    a1 = 0x40u;
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)SUB32((uint32_t)v0, (uint32_t)v1))));
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 4096))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)SLL32((uint32_t)v0, 4))));
    a2 = 0xFFFFFFFFu;
    at = 0x270000u;
    at = at + 0x3ED8u;
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(at, (uint32_t)v0))));
    (t2 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL));
    A1_W8B((uint32_t)v0, 100, (uint8_t)(uint32_t)a0);
    (t1 = (uint32_t)((int32_t)((int32_t)ADD32((uint32_t)v0, 688))));
    A1_W32B((uint32_t)v0, 20, a3);
    t8 = 0xFFu;
    A1_W32B((uint32_t)v0, 56, t0);
    t5 = 0x40u;
    A1_W32B((uint32_t)v0, 68, (uint32_t)v1);
    t4 = 0x7Fu;
    A1_W32B((uint32_t)v0, 48, 0u);
    (t7 = (uint32_t)((int32_t)((int32_t)ADD32(0u, 4096))));
    A1_W32B((uint32_t)v0, 684, a3);
    t6 = 9u;
    A1_W32B((uint32_t)v0, 52, t0);
    A1_W32B((uint32_t)v0, 44, 0u);
    A1_W32B((uint32_t)v0, 36, 0u);
    A1_W32B((uint32_t)v0, 40, 0u);
    A1_W32B((uint32_t)v0, 28, 0u);
    A1_W32B((uint32_t)v0, 32, 0u);
    A1_W32B((uint32_t)v0, 64, 0u);
    A1_W32B((uint32_t)v0, 96, 0u);
    A1_W32B((uint32_t)v0, 16, 0u);
    A1_W32B((uint32_t)v0, 60, a1);
    a1 = 1u;
    A1_W32B((uint32_t)v0, 0, a2);
    a2 = 1u;
L_1cde90:;
    (v0 = (uint64_t)(t2 ^ a1_sx64(t6)));
    (t2 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)t2, 1))));
    (v1 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL));
    (a0 = (uint64_t)(((int64_t)(int64_t)RC_U64X(t2) < (int64_t)(int32_t)16) ? 1 : 0));
    if (v0 == 0) v1 = (uint64_t)(a1_sx64(a1));;
    A1_W32B(t1, 0, (uint32_t)t3);
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)SUB32(a1, (uint32_t)v1))));
    A1_W32B(t1, 4, 0u);
    A1_W32B(t1, 44, (uint32_t)v0);
    A1_W32B(t1, 8, t8);
    A1_W32B(t1, 12, t5);
    A1_W32B(t1, 16, 0u);
    A1_W32B(t1, 20, t4);
    A1_W32B(t1, 32, t4);
    A1_W32B(t1, 36, 0u);
    A1_W32B(t1, 48, 0u);
    A1_W32B(t1, 52, t5);
    A1_W8B(t1, 100, (uint8_t)0u);
    A1_W8B(t1, 101, (uint8_t)0u);
    A1_W8B(t1, 102, (uint8_t)0u);
    A1_W8B(t1, 103, (uint8_t)0u);
    A1_W32B(t1, 72, t7);
    A1_W32B(t1, 104, 0u);
    A1_W32B(t1, 108, 0u);
    A1_W16B(t1, 92, (uint16_t)0u);
    A1_W16B(t1, 94, (uint16_t)0u);
    A1_W16B(t1, 96, (uint16_t)0u);
    A1_W16B(t1, 98, (uint16_t)0u);
    A1_W32B(t1, 76, 0u);
    A1_W32B(t1, 80, 0u);
    A1_W32B(t1, 84, 0u);
    A1_W32B(t1, 88, 0u);
    A1_W32B(t1, 56, 0u);
    A1_W32B(t1, 60, 0u);
    A1_W16B(t1, 70, (uint16_t)a2);
    A1_W16B(t1, 68, (uint16_t)a2);
    A1_W32B(t1, 40, (uint32_t)v1);
    { const bool c = (int64_t)a0 != 0; t1 = t1 + 0x70u; if (c) { A3_BACK(); goto L_1cde90; } }
    { if (xo) { xo[0] = a1_sx64(a3); xo[1] = a1_sx64(t0); xo[2] = a1_sx64(t1); xo[3] = t2; xo[4] = t3; } return v0; }
    __builtin_unreachable();
}

// kernel_sub_001CDF38 (0x1cdf38): typed by tools/typed_lift2.py (round A3; loop leaf)
inline uint32_t kernel_sub_001CDF38(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint64_t a0)
{
    uint32_t at = 0;
    uint32_t v0 = 0;
    uint32_t v1 = 0;
    uint32_t a1 = 0;
    uint32_t a2 = 0;
    uint32_t a3 = 0;
    uint32_t t0 = 0;
    uint32_t t1 = 0;
    uint64_t t2 = 0;
    uint32_t t3 = 0;
    uint32_t t4 = 0;
    uint32_t t5 = 0;
    (t2 = (uint64_t)((uint64_t)a0 + (uint64_t)0ULL));
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 1))));
    (v1 = (uint32_t)((int32_t)((int32_t)SLL32((uint32_t)t2, 2))));
    t1 = 0x7Fu;
    (v1 = (uint32_t)((int32_t)((int32_t)ADD32(v1, (uint32_t)t2))));
    t0 = 0x1000u;
    v0 = v1 << 5;
    a1 = 0xFFFFFF80u;
    v0 = v0 - v1;
    v1 = 0xFFFFFFFFu;
    v0 = v0 << 4;
    a2 = 2u;
    at = 0x270000u;
    at = at + 0x3ED8u;
    v0 = at + v0;
    a3 = 0x40u;
    A1_W32B(v0, 56, t1);
    t5 = 0xFFu;
    A1_W32B(v0, 68, t0);
    t4 = 0x64u;
    A1_W32B(v0, 48, 0u);
    t3 = 0x7Fu;
    A1_W32B(v0, 52, t1);
    A1_W32B(v0, 44, 0u);
    A1_W32B(v0, 36, 0u);
    A1_W32B(v0, 40, 0u);
    A1_W32B(v0, 28, 0u);
    A1_W32B(v0, 32, 0u);
    A1_W32B(v0, 64, 0u);
    A1_W32B(v0, 96, 0u);
    A1_W32B(v0, 16, 0u);
    A1_W32B(v0, 684, (uint32_t)a0);
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(v0, 688))));
    A1_W8B(v0, 100, (uint8_t)a1);
    a1 = 1u;
    A1_W32B(v0, 20, a2);
    a2 = 0x40u;
    A1_W32B(v0, 60, a3);
    a3 = 0x1000u;
    A1_W32(v0, v1);
    v1 = 0xFu;
L_1cdfe0:;
    v1 = v1 - 1u;
    A1_W32B((uint32_t)a0, 0, (uint32_t)t2);
    A1_W32B((uint32_t)a0, 4, 0u);
    A1_W32B((uint32_t)a0, 8, t5);
    A1_W32B((uint32_t)a0, 12, a2);
    A1_W32B((uint32_t)a0, 16, 0u);
    A1_W32B((uint32_t)a0, 20, t4);
    A1_W32B((uint32_t)a0, 32, t3);
    A1_W32B((uint32_t)a0, 36, 0u);
    A1_W32B((uint32_t)a0, 48, 0u);
    A1_W32B((uint32_t)a0, 52, a2);
    A1_W8B((uint32_t)a0, 100, (uint8_t)0u);
    A1_W8B((uint32_t)a0, 101, (uint8_t)0u);
    A1_W8B((uint32_t)a0, 102, (uint8_t)0u);
    A1_W8B((uint32_t)a0, 103, (uint8_t)0u);
    A1_W32B((uint32_t)a0, 72, a3);
    A1_W32B((uint32_t)a0, 104, 0u);
    A1_W32B((uint32_t)a0, 108, 0u);
    A1_W16B((uint32_t)a0, 92, (uint16_t)0u);
    A1_W16B((uint32_t)a0, 94, (uint16_t)0u);
    A1_W16B((uint32_t)a0, 96, (uint16_t)0u);
    A1_W16B((uint32_t)a0, 98, (uint16_t)0u);
    A1_W32B((uint32_t)a0, 56, 0u);
    A1_W32B((uint32_t)a0, 60, 0u);
    A1_W32B((uint32_t)a0, 76, 0u);
    A1_W32B((uint32_t)a0, 80, 0u);
    A1_W32B((uint32_t)a0, 84, 0u);
    A1_W32B((uint32_t)a0, 88, 0u);
    A1_W16B((uint32_t)a0, 70, (uint16_t)a1);
    A1_W16B((uint32_t)a0, 68, (uint16_t)a1);
    { const bool c = (int32_t)v1 >= 0; (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)a0, 112)))); if (c) { A3_BACK(); goto L_1cdfe0; } }
    { return v0; }
    __builtin_unreachable();
}

// Sg2SepMidiOut (0x1d2da0): typed by tools/typed_lift2.py (round A3; loop leaf)
inline uint64_t Sg2SepMidiOut(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint64_t a0, uint64_t a1, uint64_t a2)
{
    uint64_t v0 = 0;
    uint32_t v1 = 0;
    uint64_t a3 = 0;
    uint64_t t0 = 0;
    uint32_t t1 = 0;
    uint32_t t2 = 0;
    uint64_t t3 = 0;
    (v0 = (uint64_t)(((uint64_t)a0 < (uint64_t)(int64_t)(int32_t)32) ? 1 : 0));
    (t3 = (uint64_t)((uint64_t)a1 + (uint64_t)0ULL));
    { const bool c = (int64_t)v0 == 0; (a3 = (uint64_t)((uint64_t)a2 + (uint64_t)0ULL)); if (c) goto L_1d2e54; }
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)SLL32((uint32_t)a0, 2))));
    v1 = 0x290000u;
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v0, (uint32_t)a0))));
    (a1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(v1, 4294949312))));
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)SLL32((uint32_t)v0, 4))));
    (v1 = (uint32_t)((int32_t)((int32_t)ADD32((uint32_t)a0, (uint32_t)a1))));
    v1 = A1_R32(v1);
    if (v1 == 0) goto L_1d2e20;
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 2))));
    { const bool c = (int64_t)(int32_t)v1 == (int64_t)v0; t2 = 0x290000u; if (c) goto L_1d2e20; }
    t1 = 0x290000u;
    (a2 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(t2, 4294952416))));
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)a0, (uint32_t)a1))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)a2, 0))));
    v1 = t1 - 0x3B20u;
    (a1 = (uint64_t)(uint32_t)((uint8_t)A1_R8B((uint32_t)a0, 16)));
    (t0 = (uint64_t)((uint64_t)a3 + (uint64_t)0ULL));
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v0, v1))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v0, 1))));
    (v1 = (uint32_t)((int32_t)((int32_t)ADD32((uint32_t)v0, v1))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v0, 1))));
    A1_W8B((uint32_t)a0, 0, (uint8_t)(uint32_t)a1);
    (a3 = (uint64_t)((uint64_t)t3 + (uint64_t)0ULL));
    A1_W32B((uint32_t)a2, 0, (uint32_t)v0);
    A1_W8B(v1, 0, (uint8_t)(uint32_t)t3);
    goto L_1d2e4c;
L_1d2e20:;
    { (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 4294967295)))); return v0; }
L_1d2e28:;
    (a1 = (uint64_t)(uint32_t)((uint8_t)A1_R8B((uint32_t)t0, 0)));
    (v1 = (uint32_t)((int32_t)((int32_t)A1_R32B((uint32_t)a0, 0))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(t1, 4294952160))));
    (t0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)t0, 1))));
    (a3 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)a3, 4294967295))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(v1, (uint32_t)v0))));
    v1 = v1 + 1u;
    A1_W8B((uint32_t)v0, 0, (uint8_t)(uint32_t)a1);
    A1_W32B((uint32_t)a0, 0, v1);
L_1d2e4c:;
    { const bool c = (int64_t)a3 != 0; (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(t2, 4294952416)))); if (c) { A3_BACK(); goto L_1d2e28; } }
L_1d2e54:;
    { (v0 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL)); return v0; }
    __builtin_unreachable();
}

// Sg2SlotGetSlotClassIdx (0x1d3b08): typed by tools/typed_lift2.py (round A3; leaf, extra outputs xo[0]=a0)
inline uint64_t Sg2SlotGetSlotClassIdx(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint64_t *xo = nullptr)
{
    uint64_t v0 = 0;
    uint32_t v1 = 0;
    uint32_t a0 = 0;
    v1 = 0x290000u;
    v1 = v1 - 0x39DCu;
    a0 = A1_R32(v1);
    (v0 = (uint64_t)((uint64_t)a1_sx64(a0) + (uint64_t)0ULL));
    a0 = a0 + 1u;
    { A1_W32(v1, a0); if (xo) { xo[0] = a1_sx64(a0); } return v0; }
    __builtin_unreachable();
}

// Sg2SlotBlock (0x1d5cb8): typed by tools/typed_lift2.py (round A3; leaf, extra outputs xo[0]=a1 xo[1]=a2 xo[2]=a3 xo[3]=t0 xo[4]=t1 xo[5]=t2 xo[6]=t3)
inline uint32_t Sg2SlotBlock(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t a0, uint32_t a1, uint64_t *xo = nullptr)
{
    uint32_t v0 = 0;
    uint32_t v1 = 0;
    uint32_t a2 = 0;
    uint32_t a3 = 0;
    uint32_t t0 = 0;
    uint32_t t1 = 0;
    uint32_t t2 = 0;
    uint32_t t3 = 0;
    uint32_t t4 = 0;
    uint32_t t5 = 0;
    v0 = a1 << 3;
    t0 = 0x290000u;
    v0 = v0 - a1;
    t0 = t0 - 0x39E0u;
    v1 = 0x290000u;
    t4 = A1_R32(t0);
    v0 = v0 << 5;
    v1 = v1 - 0x39D0u;
    a1 = v1 + 8u;
    a2 = v1 + 4u;
    a3 = v0 + 0x10u;
    t5 = v0 + a1;
    t3 = a3 + a2;
    a3 = a3 + a1;
    t1 = v0 + v1;
    v1 = v1 + v0;
    a1 = 0xFFFFFFFFu;
    t2 = t4 + 1u;
    a2 = v0 + a2;
    A1_W32(t0, t2);
    A1_W32(t1, 0u);
    A1_W32(a2, a0);
    A1_W32(a3, a1);
    A1_W32(t3, a1);
    A1_W32B(v1, 12, a1);
    { A1_W32(t5, t4); if (xo) { xo[0] = a1_sx64(a1); xo[1] = a1_sx64(a2); xo[2] = a1_sx64(a3); xo[3] = a1_sx64(t0); xo[4] = a1_sx64(t1); xo[5] = a1_sx64(t2); xo[6] = a1_sx64(t3); } return v0; }
    __builtin_unreachable();
}

// Sg2SlotCheckSlotClass (0x1d5d58): typed by tools/typed_lift2.py (round A3; loop leaf, extra outputs xo[0]=a2)
inline uint64_t Sg2SlotCheckSlotClass(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint64_t a0, uint64_t *xo = nullptr)
{
    uint64_t v0 = 0;
    uint32_t v1 = 0;
    uint64_t a1 = 0;
    uint64_t a2 = 0;
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)41 << 16))));
    (a2 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL));
    (v1 = (uint32_t)((int32_t)((int32_t)ADD32((uint32_t)v0, 4294952496))));
    (a1 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL));
    goto L_1d5d7c;
L_1d5d70:;
    (v0 = (uint64_t)(((int64_t)(int64_t)a1 < (int64_t)(int32_t)48) ? 1 : 0));
    { const bool c = (int64_t)v0 == 0; v1 = v1 + 0xE0u; if (c) goto L_1d5da8; }
L_1d5d7c:;
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B(v1, 0))));
    (v0 = (uint64_t)(RC_U64X(v0) & (uint64_t)(uint16_t)1));
    if ((int64_t)v0 == 0) { (a1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)a1, 1)))); { A3_BACK(); goto L_1d5d70; } }
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B(v1, 12))));
    if ((int64_t)v0 != (int64_t)a0) { (a1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)a1, 1)))); { A3_BACK(); goto L_1d5d70; } }
    (a2 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 1))));
L_1d5da8:;
    { (v0 = (uint64_t)((uint64_t)a2 + (uint64_t)0ULL)); if (xo) { xo[0] = a2; } return v0; }
    __builtin_unreachable();
}

// Sg2SlotFindLoop (0x1d5db0): typed by tools/typed_lift2.py (round A3; loop leaf)
inline uint64_t Sg2SlotFindLoop(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t a0)
{
    uint64_t v0 = 0;
    uint64_t v1 = 0;
    uint32_t a1 = 0;
    uint64_t a2 = 0;
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)41 << 16))));
    (a2 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL));
    (a1 = (uint32_t)((int32_t)((int32_t)ADD32((uint32_t)v0, 4294952496))));
    (v1 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B(a1, 0))));
L_1d5dc8:;
    (v0 = (uint64_t)(v0 & (uint64_t)(uint16_t)4));
    { const bool c = (int64_t)v0 == 0; a1 = a1 + 0xE0u; if (c) goto L_1d5de0; }
    A1_W32B(a0, 0, (uint32_t)v1);
    a0 = a0 + 4u;
    (a2 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)a2, 1))));
L_1d5de0:;
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v1, 1))));
    (v0 = (uint64_t)(((int64_t)(int64_t)RC_U64X(v1) < (int64_t)(int32_t)48) ? 1 : 0));
    if ((int64_t)v0 != 0) { (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B(a1, 0)))); { A3_BACK(); goto L_1d5dc8; } }
    { (v0 = (uint64_t)((uint64_t)a2 + (uint64_t)0ULL)); return v0; }
    __builtin_unreachable();
}

// Sg2SlotFindVab (0x1d5df8): typed by tools/typed_lift2.py (round A3; loop leaf)
inline uint64_t Sg2SlotFindVab(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t a0, uint64_t a1)
{
    uint64_t v0 = 0;
    uint64_t v1 = 0;
    uint32_t a2 = 0;
    uint64_t a3 = 0;
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)41 << 16))));
    (a3 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL));
    (a2 = (uint32_t)((int32_t)((int32_t)ADD32((uint32_t)v0, 4294952496))));
    (v1 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL));
L_1d5e08:;
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B(a2, 0))));
    (v0 = (uint64_t)(RC_U64X(v0) & (uint64_t)(uint16_t)1));
    if ((int64_t)v0 == 0) { (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v1, 1)))); goto L_1d5e34; }
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B(a2, 16))));
    if ((int64_t)v0 != (int64_t)a1) { (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v1, 1)))); goto L_1d5e34; }
    A1_W32B(a0, 0, (uint32_t)v1);
    a0 = a0 + 4u;
    (a3 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)a3, 1))));
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v1, 1))));
L_1d5e34:;
    (v0 = (uint64_t)(((int64_t)(int64_t)RC_U64X(v1) < (int64_t)(int32_t)48) ? 1 : 0));
    { const bool c = (int64_t)v0 != 0; a2 = a2 + 0xE0u; if (c) { A3_BACK(); goto L_1d5e08; } }
    { (v0 = (uint64_t)((uint64_t)a3 + (uint64_t)0ULL)); return v0; }
    __builtin_unreachable();
}

// Sg2SlotFindVabCh (0x1d5e48): typed by tools/typed_lift2.py (round A3; loop leaf)
inline uint64_t Sg2SlotFindVabCh(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t a0, uint64_t a1, uint64_t a2)
{
    uint64_t v0 = 0;
    uint32_t v1 = 0;
    uint64_t a3 = 0;
    uint64_t t0 = 0;
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)41 << 16))));
    (t0 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL));
    (v1 = (uint32_t)((int32_t)((int32_t)ADD32((uint32_t)v0, 4294952496))));
    (a3 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL));
L_1d5e58:;
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B(v1, 0))));
    (v0 = (uint64_t)(RC_U64X(v0) & (uint64_t)(uint16_t)1));
    if ((int64_t)v0 == 0) { (a3 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)a3, 1)))); goto L_1d5e90; }
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B(v1, 16))));
    if ((int64_t)v0 != (int64_t)a1) { (a3 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)a3, 1)))); goto L_1d5e90; }
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B(v1, 20))));
    if ((int64_t)v0 != (int64_t)a2) { (a3 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)a3, 1)))); goto L_1d5e90; }
    A1_W32B(a0, 0, (uint32_t)a3);
    a0 = a0 + 4u;
    (t0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)t0, 1))));
    (a3 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)a3, 1))));
L_1d5e90:;
    (v0 = (uint64_t)(((int64_t)(int64_t)RC_U64X(a3) < (int64_t)(int32_t)48) ? 1 : 0));
    { const bool c = (int64_t)v0 != 0; v1 = v1 + 0xE0u; if (c) { A3_BACK(); goto L_1d5e58; } }
    { (v0 = (uint64_t)((uint64_t)t0 + (uint64_t)0ULL)); return v0; }
    __builtin_unreachable();
}

// Sg2SlotFindVabChNote (0x1d5ea8): typed by tools/typed_lift2.py (round A3; loop leaf)
inline uint64_t Sg2SlotFindVabChNote(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t a0, uint64_t a1, uint64_t a2, uint64_t a3)
{
    uint64_t v0 = 0;
    uint32_t v1 = 0;
    uint64_t t0 = 0;
    uint64_t t1 = 0;
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)41 << 16))));
    (t1 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL));
    (v1 = (uint32_t)((int32_t)((int32_t)ADD32((uint32_t)v0, 4294952496))));
    (t0 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL));
L_1d5eb8:;
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B(v1, 0))));
    (v0 = (uint64_t)(RC_U64X(v0) & (uint64_t)(uint16_t)1));
    if ((int64_t)v0 == 0) { (t0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)t0, 1)))); goto L_1d5efc; }
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B(v1, 16))));
    if ((int64_t)v0 != (int64_t)a1) { (t0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)t0, 1)))); goto L_1d5efc; }
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B(v1, 20))));
    if ((int64_t)v0 != (int64_t)a2) { (t0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)t0, 1)))); goto L_1d5efc; }
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B(v1, 36))));
    if ((int64_t)v0 != (int64_t)a3) { (t0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)t0, 1)))); goto L_1d5efc; }
    A1_W32B(a0, 0, (uint32_t)t0);
    a0 = a0 + 4u;
    (t1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)t1, 1))));
    (t0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)t0, 1))));
L_1d5efc:;
    (v0 = (uint64_t)(((int64_t)(int64_t)RC_U64X(t0) < (int64_t)(int32_t)48) ? 1 : 0));
    { const bool c = (int64_t)v0 != 0; v1 = v1 + 0xE0u; if (c) { A3_BACK(); goto L_1d5eb8; } }
    { (v0 = (uint64_t)((uint64_t)t1 + (uint64_t)0ULL)); return v0; }
    __builtin_unreachable();
}

// Sg2SlotFindVabChSsaBaseNote (0x1d5f10): typed by tools/typed_lift2.py (round A3; loop leaf)
inline uint64_t Sg2SlotFindVabChSsaBaseNote(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t a0, uint64_t a1, uint64_t a2, uint64_t a3, uint64_t t0)
{
    uint64_t v0 = 0;
    uint32_t v1 = 0;
    uint64_t t1 = 0;
    uint64_t t2 = 0;
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)41 << 16))));
    (t2 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL));
    (v1 = (uint32_t)((int32_t)((int32_t)ADD32((uint32_t)v0, 4294952496))));
    (t1 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL));
L_1d5f20:;
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B(v1, 0))));
    (v0 = (uint64_t)(RC_U64X(v0) & (uint64_t)(uint16_t)1));
    if ((int64_t)v0 == 0) { (t1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)t1, 1)))); goto L_1d5f70; }
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B(v1, 16))));
    if ((int64_t)v0 != (int64_t)a1) { (t1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)t1, 1)))); goto L_1d5f70; }
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B(v1, 20))));
    if ((int64_t)v0 != (int64_t)a2) { (t1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)t1, 1)))); goto L_1d5f70; }
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B(v1, 96))));
    if ((int64_t)v0 != (int64_t)a3) { (t1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)t1, 1)))); goto L_1d5f70; }
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B(v1, 40))));
    if ((int64_t)v0 != (int64_t)t0) { (t1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)t1, 1)))); goto L_1d5f70; }
    A1_W32B(a0, 0, (uint32_t)t1);
    a0 = a0 + 4u;
    (t2 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)t2, 1))));
    (t1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)t1, 1))));
L_1d5f70:;
    (v0 = (uint64_t)(((int64_t)(int64_t)RC_U64X(t1) < (int64_t)(int32_t)48) ? 1 : 0));
    { const bool c = (int64_t)v0 != 0; v1 = v1 + 0xE0u; if (c) { A3_BACK(); goto L_1d5f20; } }
    { (v0 = (uint64_t)((uint64_t)t2 + (uint64_t)0ULL)); return v0; }
    __builtin_unreachable();
}

// Sg2SlotFindSlotClass (0x1d5f88): typed by tools/typed_lift2.py (round A3; loop leaf, extra outputs xo[0]=a0 xo[1]=a2 xo[2]=a3)
inline uint64_t Sg2SlotFindSlotClass(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t a0, uint64_t a1, uint64_t *xo = nullptr)
{
    uint64_t v0 = 0;
    uint64_t v1 = 0;
    uint32_t a2 = 0;
    uint64_t a3 = 0;
    bool a3xw0 = false;
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)41 << 16))));
    (a3 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL));
    (a2 = (uint32_t)((int32_t)((int32_t)ADD32((uint32_t)v0, 4294952496))));
    (v1 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL));
L_1d5f98:;
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B(a2, 0))));
    (v0 = (uint64_t)(RC_U64X(v0) & (uint64_t)(uint16_t)1));
    if ((int64_t)v0 == 0) { (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v1, 1)))); goto L_1d5fc4; }
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B(a2, 12))));
    if ((int64_t)v0 != (int64_t)a1) { (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v1, 1)))); goto L_1d5fc4; }
    A1_W32B(a0, 0, (uint32_t)v1);
    a0 = a0 + 4u; a3xw0 = true;
    (a3 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)a3, 1))));
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v1, 1))));
L_1d5fc4:;
    (v0 = (uint64_t)(((int64_t)(int64_t)RC_U64X(v1) < (int64_t)(int32_t)48) ? 1 : 0));
    { const bool c = (int64_t)v0 != 0; a2 = a2 + 0xE0u; if (c) { A3_BACK(); goto L_1d5f98; } }
    { (v0 = (uint64_t)((uint64_t)a3 + (uint64_t)0ULL)); if (xo) { if (a3xw0) xo[0] = a1_sx64(a0); xo[1] = a1_sx64(a2); xo[2] = a3; } return v0; }
    __builtin_unreachable();
}

// Sg2SlotFindSlotClassCh (0x1d5fd8): typed by tools/typed_lift2.py (round A3; loop leaf, extra outputs xo[0]=a0 xo[1]=a3 xo[2]=t0)
inline uint64_t Sg2SlotFindSlotClassCh(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t a0, uint64_t a1, uint64_t a2, uint64_t *xo = nullptr)
{
    uint64_t v0 = 0;
    uint32_t v1 = 0;
    uint64_t a3 = 0;
    uint64_t t0 = 0;
    bool a3xw0 = false;
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)41 << 16))));
    (t0 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL));
    (v1 = (uint32_t)((int32_t)((int32_t)ADD32((uint32_t)v0, 4294952496))));
    (a3 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL));
L_1d5fe8:;
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B(v1, 0))));
    (v0 = (uint64_t)(RC_U64X(v0) & (uint64_t)(uint16_t)1));
    if ((int64_t)v0 == 0) { (a3 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)a3, 1)))); goto L_1d6020; }
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B(v1, 12))));
    if ((int64_t)v0 != (int64_t)a1) { (a3 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)a3, 1)))); goto L_1d6020; }
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B(v1, 20))));
    if ((int64_t)v0 != (int64_t)a2) { (a3 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)a3, 1)))); goto L_1d6020; }
    A1_W32B(a0, 0, (uint32_t)a3);
    a0 = a0 + 4u; a3xw0 = true;
    (t0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)t0, 1))));
    (a3 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)a3, 1))));
L_1d6020:;
    (v0 = (uint64_t)(((int64_t)(int64_t)RC_U64X(a3) < (int64_t)(int32_t)48) ? 1 : 0));
    { const bool c = (int64_t)v0 != 0; v1 = v1 + 0xE0u; if (c) { A3_BACK(); goto L_1d5fe8; } }
    { (v0 = (uint64_t)((uint64_t)t0 + (uint64_t)0ULL)); if (xo) { if (a3xw0) xo[0] = a1_sx64(a0); xo[1] = a3; xo[2] = t0; } return v0; }
    __builtin_unreachable();
}

// Sg2SlotFindSlotClassChNote (0x1d6038): typed by tools/typed_lift2.py (round A3; loop leaf)
inline uint64_t Sg2SlotFindSlotClassChNote(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t a0, uint64_t a1, uint64_t a2, uint64_t a3)
{
    uint64_t v0 = 0;
    uint32_t v1 = 0;
    uint64_t t0 = 0;
    uint64_t t1 = 0;
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)41 << 16))));
    (t1 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL));
    (v1 = (uint32_t)((int32_t)((int32_t)ADD32((uint32_t)v0, 4294952496))));
    (t0 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL));
L_1d6048:;
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B(v1, 0))));
    (v0 = (uint64_t)(RC_U64X(v0) & (uint64_t)(uint16_t)1));
    if ((int64_t)v0 == 0) { (t0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)t0, 1)))); goto L_1d608c; }
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B(v1, 12))));
    if ((int64_t)v0 != (int64_t)a1) { (t0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)t0, 1)))); goto L_1d608c; }
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B(v1, 20))));
    if ((int64_t)v0 != (int64_t)a2) { (t0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)t0, 1)))); goto L_1d608c; }
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B(v1, 36))));
    if ((int64_t)v0 != (int64_t)a3) { (t0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)t0, 1)))); goto L_1d608c; }
    A1_W32B(a0, 0, (uint32_t)t0);
    a0 = a0 + 4u;
    (t1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)t1, 1))));
    (t0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)t0, 1))));
L_1d608c:;
    (v0 = (uint64_t)(((int64_t)(int64_t)RC_U64X(t0) < (int64_t)(int32_t)48) ? 1 : 0));
    { const bool c = (int64_t)v0 != 0; v1 = v1 + 0xE0u; if (c) { A3_BACK(); goto L_1d6048; } }
    { (v0 = (uint64_t)((uint64_t)t1 + (uint64_t)0ULL)); return v0; }
    __builtin_unreachable();
}

// Sg2SlotFindSlotClassChSsaBaseNote (0x1d60a0): typed by tools/typed_lift2.py (round A3; loop leaf, extra outputs xo[0]=a0 xo[1]=t1 xo[2]=t2)
inline uint64_t Sg2SlotFindSlotClassChSsaBaseNote(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t a0, uint64_t a1, uint64_t a2, uint64_t a3, uint64_t t0, uint64_t *xo = nullptr)
{
    uint64_t v0 = 0;
    uint32_t v1 = 0;
    uint64_t t1 = 0;
    uint64_t t2 = 0;
    bool a3xw0 = false;
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)41 << 16))));
    (t2 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL));
    (v1 = (uint32_t)((int32_t)((int32_t)ADD32((uint32_t)v0, 4294952496))));
    (t1 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL));
L_1d60b0:;
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B(v1, 0))));
    (v0 = (uint64_t)(RC_U64X(v0) & (uint64_t)(uint16_t)1));
    if ((int64_t)v0 == 0) { (t1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)t1, 1)))); goto L_1d6100; }
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B(v1, 12))));
    if ((int64_t)v0 != (int64_t)a1) { (t1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)t1, 1)))); goto L_1d6100; }
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B(v1, 20))));
    if ((int64_t)v0 != (int64_t)a2) { (t1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)t1, 1)))); goto L_1d6100; }
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B(v1, 96))));
    if ((int64_t)v0 != (int64_t)a3) { (t1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)t1, 1)))); goto L_1d6100; }
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B(v1, 40))));
    if ((int64_t)v0 != (int64_t)t0) { (t1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)t1, 1)))); goto L_1d6100; }
    A1_W32B(a0, 0, (uint32_t)t1);
    a0 = a0 + 4u; a3xw0 = true;
    (t2 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)t2, 1))));
    (t1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)t1, 1))));
L_1d6100:;
    (v0 = (uint64_t)(((int64_t)(int64_t)RC_U64X(t1) < (int64_t)(int32_t)48) ? 1 : 0));
    { const bool c = (int64_t)v0 != 0; v1 = v1 + 0xE0u; if (c) { A3_BACK(); goto L_1d60b0; } }
    { (v0 = (uint64_t)((uint64_t)t2 + (uint64_t)0ULL)); if (xo) { if (a3xw0) xo[0] = a1_sx64(a0); xo[1] = t1; xo[2] = t2; } return v0; }
    __builtin_unreachable();
}

// Sg2SlotSetMasVol (0x1d62f0): typed by tools/typed_lift2.py (round A3; leaf, extra outputs xo[0]=a0 xo[1]=a1 xo[2]=a3 xo[3]=t0 xo[4]=t1 xo[5]=t2)
inline uint32_t Sg2SlotSetMasVol(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t a0, uint32_t a1, uint32_t a2, uint64_t *xo = nullptr)
{
    uint32_t v0 = 0;
    uint32_t v1 = 0;
    uint32_t a3 = 0;
    uint32_t t0 = 0;
    uint32_t t1 = 0;
    uint32_t t2 = 0;
    v0 = a0 << 3;
    v1 = 0x290000u;
    v0 = v0 - a0;
    t2 = v1 - 0x39D0u;
    t1 = v0 << 5;
    t0 = t2 + 8u;
    v0 = t1 + 0x30u;
    a0 = t2 + 0xCu;
    a3 = v0 + a0;
    v0 = v0 + t0;
    A1_W32(v0, a1);
    a1 = 0xFFFFFFFFu;
    v1 = t1 + 0x60u;
    A1_W32(a3, a2);
    a0 = v1 + a0;
    v1 = v1 + t0;
    v0 = A1_R32(v1);
    if (v0 == a1) goto L_1d6354;
    v0 = A1_R32(a0);
    { const bool c = v0 == a1; v1 = t1 + t2; if (c) goto L_1d6354; }
    v0 = A1_R32(v1);
    v0 = v0 | 0x1000u;
    A1_W32(v1, v0);
L_1d6354:;
    { if (xo) { xo[0] = a1_sx64(a0); xo[1] = a1_sx64(a1); xo[2] = a1_sx64(a3); xo[3] = a1_sx64(t0); xo[4] = a1_sx64(t1); xo[5] = a1_sx64(t2); } return v0; }
    __builtin_unreachable();
}

// Sg2SlotSetSeMasVol (0x1d6360): typed by tools/typed_lift2.py (round A3; leaf, extra outputs xo[0]=a0 xo[1]=a1 xo[2]=a2 xo[3]=a3)
inline uint32_t Sg2SlotSetSeMasVol(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t a0, uint32_t a1, uint64_t *xo = nullptr)
{
    uint32_t v0 = 0;
    uint32_t v1 = 0;
    uint32_t a2 = 0;
    uint32_t a3 = 0;
    v0 = a0 << 3;
    v1 = 0x290000u;
    v0 = v0 - a0;
    a2 = v1 - 0x39D0u;
    a0 = v0 << 5;
    a3 = 0xFFFFFFFFu;
    v1 = a0 + a2;
    v0 = a0 + 0x60u;
    A1_W32B(v1, 64, a1);
    a1 = a2 + v0;
    v0 = a1;
    v1 = A1_R32B(v0, 8);
    if (v1 == a3) goto L_1d63b0;
    v0 = A1_R32B(a1, 12);
    { const bool c = v0 == a3; v1 = a0 + a2; if (c) goto L_1d63b0; }
    v0 = A1_R32(v1);
    v0 = v0 | 0x1000u;
    A1_W32(v1, v0);
L_1d63b0:;
    { if (xo) { xo[0] = a1_sx64(a0); xo[1] = a1_sx64(a1); xo[2] = a1_sx64(a2); xo[3] = a1_sx64(a3); } return v0; }
    __builtin_unreachable();
}

// Sg2SlotSetBaseVol (0x1d63b8): typed by tools/typed_lift2.py (round A3; leaf, extra outputs xo[0]=a3)
inline uint32_t Sg2SlotSetBaseVol(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t a0, uint32_t a1, uint64_t *xo = nullptr)
{
    uint32_t v0 = 0;
    uint32_t v1 = 0;
    uint32_t a2 = 0;
    uint32_t a3 = 0;
    v0 = a0 << 3;
    v1 = 0x290000u;
    v0 = v0 - a0;
    a2 = v1 - 0x39D0u;
    a0 = v0 << 5;
    a3 = 0xFFFFFFFFu;
    v1 = a0 + a2;
    v0 = a0 + 0x60u;
    A1_W32B(v1, 68, a1);
    a1 = a2 + v0;
    v0 = a1;
    v1 = A1_R32B(v0, 8);
    if (v1 == a3) goto L_1d6408;
    v0 = A1_R32B(a1, 12);
    { const bool c = v0 == a3; v1 = a0 + a2; if (c) goto L_1d6408; }
    v0 = A1_R32(v1);
    v0 = v0 | 0x1000u;
    A1_W32(v1, v0);
L_1d6408:;
    { if (xo) { xo[0] = a1_sx64(a3); } return v0; }
    __builtin_unreachable();
}

// Sg2SlotSetVol (0x1d6410): typed by tools/typed_lift2.py (round A3; leaf, extra outputs xo[0]=a0)
inline uint32_t Sg2SlotSetVol(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t a0, uint32_t a1, uint64_t *xo = nullptr)
{
    uint32_t v0 = 0;
    uint32_t v1 = 0;
    v1 = a0 << 3;
    v0 = 0x290000u;
    v1 = v1 - a0;
    v0 = v0 - 0x39D0u;
    v1 = v1 << 5;
    a0 = v1 + v0;
    A1_W32B(a0, 72, a1);
    v1 = a0;
    v0 = A1_R32(v1);
    v0 = v0 | 0x1000u;
    { A1_W32(v1, v0); if (xo) { xo[0] = a1_sx64(a0); } return v0; }
    __builtin_unreachable();
}

// Sg2SlotSetMasPan (0x1d6440): typed by tools/typed_lift2.py (round A3; leaf, extra outputs xo[0]=a0 xo[1]=a1 xo[2]=a2 xo[3]=a3)
inline uint32_t Sg2SlotSetMasPan(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t a0, uint32_t a1, uint64_t *xo = nullptr)
{
    uint32_t v0 = 0;
    uint32_t v1 = 0;
    uint32_t a2 = 0;
    uint32_t a3 = 0;
    v1 = 0x290000u;
    v0 = a0 << 3;
    v0 = v0 - a0;
    a3 = v1 - 0x39D0u;
    a2 = v0 << 5;
    a0 = a3 + 0xCu;
    v1 = a2 + a0;
    v0 = a2 + 0x60u;
    A1_W32B(v1, 64, a1);
    a1 = 0xFFFFFFFFu;
    a0 = v0 + a0;
    v0 = a3 + v0;
    v1 = A1_R32B(v0, 8);
    if (v1 == a1) goto L_1d6494;
    v0 = A1_R32(a0);
    { const bool c = v0 == a1; v1 = a2 + a3; if (c) goto L_1d6494; }
    v0 = A1_R32(v1);
    v0 = v0 | 0x2000u;
    A1_W32(v1, v0);
L_1d6494:;
    { if (xo) { xo[0] = a1_sx64(a0); xo[1] = a1_sx64(a1); xo[2] = a1_sx64(a2); xo[3] = a1_sx64(a3); } return v0; }
    __builtin_unreachable();
}

// Sg2SlotSetBasePan (0x1d64a0): typed by tools/typed_lift2.py (round A3; leaf, extra outputs xo[0]=a3)
inline uint32_t Sg2SlotSetBasePan(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t a0, uint32_t a1, uint64_t *xo = nullptr)
{
    uint32_t v0 = 0;
    uint32_t v1 = 0;
    uint32_t a2 = 0;
    uint32_t a3 = 0;
    v0 = a0 << 3;
    v1 = 0x290000u;
    v0 = v0 - a0;
    a2 = v1 - 0x39D0u;
    a0 = v0 << 5;
    a3 = 0xFFFFFFFFu;
    v1 = a0 + a2;
    v0 = a0 + 0x60u;
    A1_W32B(v1, 80, a1);
    a1 = a2 + v0;
    v0 = a1;
    v1 = A1_R32B(v0, 8);
    if (v1 == a3) goto L_1d64f0;
    v0 = A1_R32B(a1, 12);
    { const bool c = v0 == a3; v1 = a0 + a2; if (c) goto L_1d64f0; }
    v0 = A1_R32(v1);
    v0 = v0 | 0x2000u;
    A1_W32(v1, v0);
L_1d64f0:;
    { if (xo) { xo[0] = a1_sx64(a3); } return v0; }
    __builtin_unreachable();
}

// Sg2SlotSetPan (0x1d64f8): typed by tools/typed_lift2.py (round A3; leaf, extra outputs xo[0]=a0)
inline uint32_t Sg2SlotSetPan(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t a0, uint32_t a1, uint64_t *xo = nullptr)
{
    uint32_t v0 = 0;
    uint32_t v1 = 0;
    v1 = a0 << 3;
    v0 = 0x290000u;
    v1 = v1 - a0;
    v0 = v0 - 0x39D0u;
    v1 = v1 << 5;
    a0 = v1 + v0;
    A1_W32B(a0, 84, a1);
    v1 = a0;
    v0 = A1_R32(v1);
    v0 = v0 | 0x2000u;
    { A1_W32(v1, v0); if (xo) { xo[0] = a1_sx64(a0); } return v0; }
    __builtin_unreachable();
}

// Sg2SlotSetMasTune (0x1d66a0): typed by tools/typed_lift2.py (round A3; leaf, extra outputs xo[0]=a0)
inline uint32_t Sg2SlotSetMasTune(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t a0, uint32_t a1, uint64_t *xo = nullptr)
{
    uint32_t v0 = 0;
    uint32_t v1 = 0;
    v1 = a0 << 3;
    v0 = 0x290000u;
    v1 = v1 - a0;
    v0 = v0 - 0x39D0u;
    v1 = v1 << 5;
    a0 = v1 + v0;
    A1_W32B(a0, 52, a1);
    v1 = a0;
    v0 = A1_R32(v1);
    v0 = v0 | 0x80u;
    { A1_W32(v1, v0); if (xo) { xo[0] = a1_sx64(a0); } return v0; }
    __builtin_unreachable();
}

// Sg2SlotSetVolPhase (0x1d6720): typed by tools/typed_lift2.py (round A3; leaf, extra outputs xo[0]=a0 xo[1]=a3 xo[2]=t0)
inline uint64_t Sg2SlotSetVolPhase(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t a0, uint32_t a1, uint32_t a2, uint64_t *xo = nullptr)
{
    uint64_t v0 = 0;
    uint32_t v1 = 0;
    uint64_t a3 = 0;
    uint32_t t0 = 0;
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)SLL32(a0, 3))));
    v1 = 0x290000u;
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)SUB32((uint32_t)v0, a0))));
    v1 = v1 - 0x39D0u;
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)SLL32((uint32_t)v0, 5))));
    (t0 = (uint32_t)((int32_t)((int32_t)ADD32((uint32_t)v0, v1))));
    a0 = A1_R32(t0);
    (a3 = (uint64_t)((uint64_t)a1_sx64(t0) + (uint64_t)0ULL));
    (v0 = (uint64_t)((uint64_t)RC_U64X(a3) + (uint64_t)0ULL));
    A1_W16B((uint32_t)a3, 116, (uint16_t)a1);
    a0 = a0 | 0x100u;
    A1_W16B((uint32_t)v0, 118, (uint16_t)a2);
    { A1_W32(t0, a0); if (xo) { xo[0] = a1_sx64(a0); xo[1] = a3; xo[2] = a1_sx64(t0); } return v0; }
    __builtin_unreachable();
}

// Sg2SlotSetLfoPitchTbl (0x1d67a8): typed by tools/typed_lift2.py (round A3; leaf, extra outputs xo[0]=a2 xo[1]=a3)
inline uint64_t Sg2SlotSetLfoPitchTbl(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t a0, uint64_t a1, uint64_t *xo = nullptr)
{
    uint64_t v0 = 0;
    uint32_t v1 = 0;
    uint64_t a2 = 0;
    uint32_t a3 = 0;
    (a2 = (uint64_t)((uint64_t)a1 + (uint64_t)0ULL));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)SLL32(a0, 3))));
    (a3 = (uint32_t)((int32_t)((int32_t)ADD32((uint32_t)a2, 60))));
    { const bool c = (int64_t)a2 == 0; (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)SUB32((uint32_t)v0, a0)))); if (c) goto L_1d67e4; }
    (v1 = (uint32_t)((int32_t)((int32_t)SLL32((uint32_t)v0, 5))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)41 << 16))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v0, 4294952496))));
    a0 = v1 + 0x80u;
    (a1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v0, a0))));
    (v1 = (uint32_t)((int32_t)((int32_t)ADD32(v1, (uint32_t)v0))));
    (v0 = (uint64_t)((uint64_t)RC_U64X(a1) + (uint64_t)0ULL));
    A1_W32B(v1, 132, (uint32_t)a2);
    A1_W32B((uint32_t)v0, 8, (uint32_t)a2);
    A1_W32B((uint32_t)a1, 12, a3);
L_1d67e4:;
    { if (xo) { xo[0] = a2; xo[1] = a1_sx64(a3); } return v0; }
    __builtin_unreachable();
}

// Sg2SlotSetLfoPitchDepth (0x1d68a8): typed by tools/typed_lift2.py (round A3; leaf, extra outputs xo[0]=a2 xo[1]=a3 xo[2]=t0 xo[3]=t1 xo[4]=t2)
inline uint64_t Sg2SlotSetLfoPitchDepth(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint64_t a0, uint64_t a1, uint64_t *xo = nullptr)
{
    uint64_t v0 = 0;
    uint32_t v1 = 0;
    uint64_t a2 = 0;
    uint64_t a3 = 0;
    uint32_t t0 = 0;
    uint32_t t1 = 0;
    uint32_t t2 = 0;
    (a3 = (uint64_t)((uint64_t)a0 + (uint64_t)0ULL));
    { const bool c = (int64_t)a1 == 0; (t0 = (uint32_t)((int32_t)((int32_t)SLL32((uint32_t)a3, 3)))); if (c) goto L_1d68e0; }
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)SUB32(t0, (uint32_t)a3))));
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 1))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)SLL32((uint32_t)v0, 5))));
    (a2 = (uint64_t)((uint64_t)a1 + (uint64_t)0ULL));
    v1 = 0x290000u;
    (v1 = (uint32_t)((int32_t)((int32_t)ADD32(v1, (uint32_t)v0))));
    v1 = A1_R32(v1 - 0x39CCu);
    { const bool c = (int64_t)(int32_t)v1 != (int64_t)a0; t2 = 0x290000u; if (c) goto L_1d68e8; }
    (a2 = (uint64_t)((int64_t)(int32_t)((int32_t)SLL32((uint32_t)a1, 1))));
    goto L_1d68e8;
L_1d68e0:;
    t2 = 0x290000u;
    (a2 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL));
L_1d68e8:;
    (v1 = (uint32_t)((int32_t)((int32_t)SUB32(t0, (uint32_t)a3))));
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(t2, 4294952496))));
    v1 = v1 << 5;
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(v1, 112))));
    (t1 = (uint32_t)((int32_t)((int32_t)ADD32((uint32_t)a0, (uint32_t)v0))));
    (v0 = (uint64_t)((uint64_t)a1_sx64(t1) + (uint64_t)0ULL));
    { const bool c = (int64_t)a1 == 0; A1_W32B((uint32_t)v0, 12, (uint32_t)a2); if (c) goto L_1d6928; }
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B(t1, 8))));
    { const bool c = (int64_t)v0 == 0; (v1 = (uint32_t)((int32_t)((int32_t)ADD32(v1, (uint32_t)a0)))); if (c) goto L_1d6928; }
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B(v1, 0))));
    (v0 = (uint64_t)(RC_U64X(v0) | (uint64_t)(uint16_t)1024));
    { A1_W32B(v1, 0, (uint32_t)v0); if (xo) { xo[0] = a2; xo[1] = a3; xo[2] = a1_sx64(t0); xo[3] = a1_sx64(t1); xo[4] = a1_sx64(t2); } return v0; }
L_1d6928:;
    (v1 = (uint32_t)((int32_t)((int32_t)SUB32(t0, (uint32_t)a3))));
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(t2, 4294952496))));
    v1 = v1 << 5;
    (a1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 4294966271))));
    (v1 = (uint32_t)((int32_t)((int32_t)ADD32(v1, (uint32_t)a0))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B(v1, 0))));
    (v0 = (uint64_t)(RC_U64X(v0) & RC_U64X(a1)));
    { A1_W32B(v1, 0, (uint32_t)v0); if (xo) { xo[0] = a2; xo[1] = a3; xo[2] = a1_sx64(t0); xo[3] = a1_sx64(t1); xo[4] = a1_sx64(t2); } return v0; }
    __builtin_unreachable();
}

// Sg2SlotSetPoltamentParam (0x1d6cf0): typed by tools/typed_lift2.py (round A3; leaf, extra outputs xo[0]=f12)
inline uint32_t Sg2SlotSetPoltamentParam(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint64_t a0, uint64_t a1, float f12, uint64_t *xo = nullptr)
{
    uint32_t at = 0;
    uint32_t v0 = 0;
    uint32_t v1 = 0;
    bool a3xw0 = false;
    (v1 = (uint32_t)((int32_t)((int32_t)SLL32((uint32_t)a0, 3))));
    (v1 = (uint32_t)((int32_t)((int32_t)SUB32(v1, (uint32_t)a0))));
    v1 = v1 << 5;
    at = 0x290000u;
    at = at - 0x39D0u;
    v1 = at + v1;
    v0 = A1_R32(v1);
    (a0 = (uint64_t)(a1_sx64(v0) | (uint64_t)(uint16_t)512));
    v0 = v0 & 0x200u;
    if (v0 != 0) { v0 = A1_R32B(v1, 208); goto L_1d6d34; }
    v0 = A1_R32B(v1, 36);
    A1_W32B(v1, 0, (uint32_t)a0);
    A1_W32B(v1, 220, 0u);
    A1_W32B(v1, 204, v0);
    A1_W32B(v1, 208, v0);
    v0 = A1_R32B(v1, 208);
L_1d6d34:;
    (v0 = (uint32_t)(((int64_t)(int64_t)a1 < (int64_t)(int64_t)a1_sx64(v0)) ? 1 : 0));
    { const bool c = v0 == 0; A1_W32B(v1, 212, (uint32_t)a1); if (c) goto L_1d6d44; }
    f12 = (-(float)(f12)); a3xw0 = true;
L_1d6d44:;
    { A1_W32B(v1, 216, a1_bits(f12)); if (xo) { if (a3xw0) xo[0] = a1_bits(f12); } return v0; }
    __builtin_unreachable();
}

// Sg2SlotTbl (0x1d8000): typed by tools/typed_lift2.py (round A3; loop leaf)
inline uint32_t Sg2SlotTbl(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t a0, uint64_t a1)
{
    uint32_t v0 = 0;
    uint32_t v1 = 0;
    uint32_t a2 = 0;
    uint32_t a3 = 0;
    uint64_t t0 = 0;
    uint64_t t1 = 0;
    uint32_t t2 = 0;
    uint32_t t3 = 0;
    uint32_t t4 = 0;
    v0 = a0 << 1;
    (t0 = (uint64_t)((uint64_t)a1 + (uint64_t)0ULL));
    a0 = v0 + a0;
    a2 = a0 << 3;
    v1 = a2 + 0x18u;
    v0 = (int32_t)a2 < (int32_t)v1 ? 1u : 0u;
    { const bool c = v0 == 0; (a1 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL)); if (c) goto L_1d80b8; }
    v0 = a0 << 6;
    (t1 = (uint64_t)((uint64_t)a1_sx64(v1) + (uint64_t)0ULL));
    v0 = v0 - a2;
    t4 = 0x18u;
    v0 = v0 << 5;
    t3 = 3u;
    a3 = 0x290000u;
    a3 = a3 - 0x39CCu;
    a3 = a3 + v0;
    t2 = 0x20u;
L_1d8048:;
    v0 = A1_R32(a3);
    { const bool c = v0 == 0; v1 = 0x2Du; if (c) goto L_1d8064; }
    v0 = A1_R8(a3);
    v0 = v0 + 0x30u;
    v0 = v0 << 24;
    v1 = (uint32_t)((int32_t)v0 >> 24);
L_1d8064:;
    (v0 = (uint32_t)((int32_t)((int32_t)ADD32((uint32_t)t0, (uint32_t)a1))));
    (a1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)a1, 1))));
    { const bool c = a2 == 0; A1_W8(v0, (uint8_t)v1); if (c) goto L_1d80a8; }
    { const bool c = a2 == t4; a0 = a2 + 3u; if (c) goto L_1d80a8; }
    v1 = (int32_t)a2 < 0 ? 1u : 0u;
    v0 = a2;
    if (a1_sx64(v1) != 0) v0 = (uint32_t)((uint64_t)(a1_sx64(a0)));;
    v0 = (uint32_t)((int32_t)v0 >> 2);
    v0 = v0 << 2;
    v0 = a2 - v0;
    if (v0 != t3) { a2 = a2 + 1u; goto L_1d80ac; }
    (v0 = (uint32_t)((int32_t)((int32_t)ADD32((uint32_t)t0, (uint32_t)a1))));
    (a1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)a1, 1))));
    A1_W8(v0, (uint8_t)t2);
L_1d80a8:;
    a2 = a2 + 1u;
L_1d80ac:;
    (v0 = (uint32_t)(((int64_t)(int64_t)a1_sx64(a2) < (int64_t)(int64_t)t1) ? 1 : 0));
    { const bool c = v0 != 0; a3 = a3 + 0xE0u; if (c) { A3_BACK(); goto L_1d8048; } }
L_1d80b8:;
    (v0 = (uint32_t)((int32_t)((int32_t)ADD32((uint32_t)t0, (uint32_t)a1))));
    { A1_W8(v0, (uint8_t)0u); return v0; }
    __builtin_unreachable();
}

// Sg2SmfpMidiOut (0x1d8de0): typed by tools/typed_lift2.py (round A3; loop leaf)
inline uint32_t Sg2SmfpMidiOut(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint64_t a0, uint64_t a1, uint32_t a2)
{
    uint32_t at = 0;
    uint32_t v0 = 0;
    uint32_t v1 = 0;
    uint64_t a3 = 0;
    uint32_t t0 = 0;
    uint32_t t1 = 0;
    uint64_t t2 = 0;
    { const bool c = (int32_t)a0 < 0; (t2 = (uint64_t)((uint64_t)a1 + (uint64_t)0ULL)); if (c) goto L_1d8e6c; }
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)SLL32((uint32_t)a0, 7))));
    at = 0x290000u;
    at = at - 0xF80u;
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(at, (uint32_t)a0))));
    (v1 = (uint32_t)((int32_t)((int32_t)A1_R32B((uint32_t)a0, 0))));
    { const bool c = v1 == 0; v0 = 0xFFFFFFFFu; if (c) goto L_1d8e70; }
    v0 = 0x290000u;
    v1 = 0x290000u;
    t1 = v0 + 0x4A0u;
    t0 = v1 - 0x360u;
    v0 = A1_R32(t1);
    (a3 = (uint64_t)((uint64_t)t2 + (uint64_t)0ULL));
    (a1 = (uint64_t)(uint32_t)((uint8_t)A1_R8B((uint32_t)a0, 8)));
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(v0, t0))));
    v0 = v0 + 1u;
    v1 = v0 + 1u;
    v0 = v0 + t0;
    A1_W8B((uint32_t)a0, 0, (uint8_t)(uint32_t)a1);
    A1_W32(t1, v1);
    { const bool c = (int64_t)a3 == 0; A1_W8B(v0, 0, (uint8_t)(uint32_t)t2); if (c) goto L_1d8e6c; }
    (a1 = (uint64_t)((uint64_t)a1_sx64(t1) + (uint64_t)0ULL));
L_1d8e48:;
    (v0 = (uint32_t)((int32_t)((int32_t)A1_R32B((uint32_t)a1, 0))));
    (a3 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)a3, 4294967295))));
    (a0 = (uint64_t)(uint32_t)((uint8_t)A1_R8B(a2, 0)));
    a2 = a2 + 1u;
    v1 = v0 + t0;
    v0 = v0 + 1u;
    A1_W8B(v1, 0, (uint8_t)(uint32_t)a0);
    { const bool c = (int64_t)a3 != 0; A1_W32B((uint32_t)a1, 0, v0); if (c) { A3_BACK(); goto L_1d8e48; } }
L_1d8e6c:;
    v0 = 0u;
L_1d8e70:;
    { return v0; }
    __builtin_unreachable();
}

// get_vabid (0x1d9ec8): typed by tools/typed_lift2.py (round A3; loop leaf, extra outputs xo[0]=a2)
inline uint64_t get_vabid(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint64_t *xo = nullptr)
{
    uint64_t v0 = 0;
    uint32_t v1 = 0;
    uint32_t a0 = 0;
    uint64_t a1 = 0;
    uint32_t a2 = 0;
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)41 << 16))));
    (a1 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL));
    (a2 = (uint32_t)((int32_t)((int32_t)ADD32((uint32_t)v0, 1216))));
    v1 = A1_R32(a2);
L_1d9ed8:;
    a2 = a2 + 0x2Cu;
    (v0 = (uint64_t)((uint64_t)a1 + (uint64_t)0ULL));
    (a1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)a1, 1))));
    { const bool c = v1 == 0; (a0 = (uint32_t)(((int64_t)(int64_t)RC_U64X(a1) < (int64_t)(int32_t)8) ? 1 : 0)); if (c) goto L_1d9f00; }
    if (a0 != 0) { v1 = A1_R32(a2); { A3_BACK(); goto L_1d9ed8; } }
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 4294967295))));
L_1d9f00:;
    { if (xo) { xo[0] = a1_sx64(a2); } return v0; }
    __builtin_unreachable();
}

// kernel_sub_001D9F08 (0x1d9f08): typed by tools/typed_lift2.py (round A3; leaf, extra outputs xo[0]=a1 xo[1]=a2)
inline uint32_t kernel_sub_001D9F08(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t a0, uint64_t a1, uint64_t *xo = nullptr)
{
    uint32_t v0 = 0;
    uint32_t v1 = 0;
    uint64_t a2 = 0;
    (a2 = (uint64_t)((uint64_t)a1 + (uint64_t)0ULL));
    v1 = 0x64680000u;
    (a1 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)a2, 12))));
    v1 = v1 | 0x5353u;
    { const bool c = (int64_t)a1 != (int64_t)(int32_t)v1; v0 = 0xFFFFFFFFu; if (c) goto L_1da078; }
    v0 = A1_R32(a0);
    { const bool c = v0 != 0; v0 = 0xFFFFFFFFu; if (c) goto L_1da078; }
    (v1 = (uint32_t)((int32_t)((int32_t)A1_R32B((uint32_t)a2, 124))));
    if (v1 != v0) { v0 = 4u; goto L_1d9f3c; }
    v0 = 3u;
L_1d9f3c:;
    A1_W32(a0, v0);
    A1_W32B(a0, 4, (uint32_t)a2);
    v0 = 0xFFFFFFFFu;
    (v1 = (uint32_t)((int32_t)((int32_t)A1_R32B((uint32_t)a2, 16))));
    { const bool c = v1 == v0; (v0 = (uint32_t)((int32_t)((int32_t)ADD32(v1, (uint32_t)a2)))); if (c) goto L_1d9f60; }
    A1_W32B((uint32_t)a2, 48, v0);
    goto L_1d9f64;
L_1d9f60:;
    A1_W32B((uint32_t)a2, 48, 0u);
L_1d9f64:;
    (v1 = (uint32_t)((int32_t)((int32_t)A1_R32B((uint32_t)a2, 20))));
    v0 = 0xFFFFFFFFu;
    { const bool c = v1 == v0; (v0 = (uint32_t)((int32_t)((int32_t)ADD32(v1, (uint32_t)a2)))); if (c) goto L_1d9f80; }
    A1_W32B((uint32_t)a2, 52, v0);
    goto L_1d9f84;
L_1d9f80:;
    A1_W32B((uint32_t)a2, 52, 0u);
L_1d9f84:;
    (v1 = (uint32_t)((int32_t)((int32_t)A1_R32B((uint32_t)a2, 24))));
    v0 = 0xFFFFFFFFu;
    { const bool c = v1 == v0; (v0 = (uint32_t)((int32_t)((int32_t)ADD32(v1, (uint32_t)a2)))); if (c) goto L_1d9fa0; }
    A1_W32B((uint32_t)a2, 56, v0);
    goto L_1d9fa4;
L_1d9fa0:;
    A1_W32B((uint32_t)a2, 56, 0u);
L_1d9fa4:;
    (v1 = (uint32_t)((int32_t)((int32_t)A1_R32B((uint32_t)a2, 28))));
    v0 = 0xFFFFFFFFu;
    { const bool c = v1 == v0; (v0 = (uint32_t)((int32_t)((int32_t)ADD32(v1, (uint32_t)a2)))); if (c) goto L_1d9fc0; }
    A1_W32B((uint32_t)a2, 60, v0);
    goto L_1d9fc4;
L_1d9fc0:;
    A1_W32B((uint32_t)a2, 60, 0u);
L_1d9fc4:;
    (v1 = (uint32_t)((int32_t)((int32_t)A1_R32B((uint32_t)a2, 32))));
    v0 = 0xFFFFFFFFu;
    { const bool c = v1 == v0; (v0 = (uint32_t)((int32_t)((int32_t)ADD32(v1, (uint32_t)a2)))); if (c) goto L_1d9fe0; }
    A1_W32B((uint32_t)a2, 64, v0);
    goto L_1d9fe4;
L_1d9fe0:;
    A1_W32B((uint32_t)a2, 64, 0u);
L_1d9fe4:;
    (v1 = (uint32_t)((int32_t)((int32_t)A1_R32B((uint32_t)a2, 36))));
    v0 = 0xFFFFFFFFu;
    { const bool c = v1 == v0; (v0 = (uint32_t)((int32_t)((int32_t)ADD32(v1, (uint32_t)a2)))); if (c) goto L_1da000; }
    A1_W32B((uint32_t)a2, 68, v0);
    goto L_1da004;
L_1da000:;
    A1_W32B((uint32_t)a2, 68, 0u);
L_1da004:;
    (v0 = (uint32_t)((int32_t)((int32_t)A1_R32B((uint32_t)a2, 52))));
    { const bool c = v0 == 0; A1_W32B(a0, 20, (uint32_t)a2); if (c) goto L_1da028; }
    v1 = A1_R16(v0);
    A1_W32B(a0, 24, v1);
    (v0 = (uint32_t)((int32_t)((int32_t)A1_R32B((uint32_t)a2, 52))));
    v0 = v0 + 2u;
    A1_W32B(a0, 28, v0);
    goto L_1da034;
L_1da028:;
    v0 = 0xFFFFFFFFu;
    A1_W32B(a0, 28, 0u);
    A1_W32B(a0, 24, v0);
L_1da034:;
    (v0 = (uint32_t)((int32_t)((int32_t)A1_R32B((uint32_t)a2, 56))));
    if (v0 == 0) { v0 = 0xFFFFFFFFu; goto L_1da060; }
    v0 = A1_R16(v0);
    A1_W32B(a0, 32, v0);
    (v1 = (uint32_t)((int32_t)((int32_t)A1_R32B((uint32_t)a2, 56))));
    v0 = v1 + 2u;
    A1_W32B(a0, 36, v1);
    A1_W32B(a0, 40, v0);
    goto L_1da068;
L_1da060:;
    A1_W32B(a0, 40, 0u);
    A1_W32B(a0, 32, v0);
L_1da068:;
    v1 = 0x7Fu;
    v0 = 0u;
    { A1_W32B(a0, 16, v1); if (xo) { xo[0] = a1; xo[1] = a2; } return v0; }
L_1da078:;
    { if (xo) { xo[0] = a1; xo[1] = a2; } return v0; }
    __builtin_unreachable();
}

// kernel_sub_001DAAB0 (0x1daab0): typed by tools/typed_lift2.py (round A3; loop leaf, extra outputs xo[0]=f0 xo[1]=v0 xo[2]=a2 xo[3]=a3 xo[4]=t0)
inline void kernel_sub_001DAAB0(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint64_t a0, uint64_t a1, uint64_t *xo = nullptr)
{
    float f0 = 0.0f;
    float f1 = 0.0f;
    float f2 = 0.0f;
    uint32_t at = 0;
    uint32_t v0 = 0;
    uint32_t v1 = 0;
    uint64_t a2 = 0;
    uint64_t a3 = 0;
    uint32_t t0 = 0;
    bool a3xw0 = false;
    bool a3xw1 = false;
    (a3 = (uint64_t)((uint64_t)a1 + (uint64_t)0ULL));
    (t0 = (uint32_t)((int32_t)((int32_t)A1_R32B((uint32_t)a0, 4))));
    { const bool c = (int32_t)a3 <= 0; (a2 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL)); if (c) goto L_1dab04; }
    (at = (uint32_t)((int32_t)((int32_t)((uint32_t)16128 << 16))));
    { uint32_t bits = at; std::memcpy(&f2, &bits, sizeof(bits)); }
L_1daac8:;
    (v1 = (uint32_t)((int32_t)((int32_t)SLL32((uint32_t)a2, 2))));
    (a2 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)a2, 1))));
    v1 = v1 + t0;
    (v0 = (uint32_t)((int32_t)((int32_t)ADD32((uint32_t)a2, 256)))); a3xw1 = true;
    (a1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(v1, 1024))));
    f0 = a1_flt(A1_R32(v1)); a3xw0 = true;
    { uint32_t bits = A1_R32B((uint32_t)a1, 0); float f; std::memcpy(&f, &bits, sizeof(f)); f1 = f; }
    (a0 = (uint64_t)(RC_U64X(a2) & (uint64_t)(uint16_t)256));
    if (RC_U64X(a0) != 0) a2 = (uint64_t)(a1_sx64(v0));;
    f0 = a1f::add(f0, f1); a3xw0 = true;
    (v0 = (uint32_t)(((int64_t)(int64_t)a2 < (int64_t)(int64_t)a3) ? 1 : 0)); a3xw1 = true;
    f0 = a1f::mul(f0, f2); a3xw0 = true;
    { float f = f0; uint32_t bits; std::memcpy(&bits, &f, sizeof(bits)); A1_W32B((uint32_t)a1, 0, bits); }
    { const bool c = v0 != 0; A1_W32(v1, a1_bits(f0)); if (c) { A3_BACK(); goto L_1daac8; } }
L_1dab04:;
    { if (xo) { if (a3xw0) xo[0] = a1_bits(f0); if (a3xw1) xo[1] = a1_sx64(v0); xo[2] = a2; xo[3] = a3; xo[4] = a1_sx64(t0); } return; }
    __builtin_unreachable();
}

// kernel_sub_001DAC18 (0x1dac18): typed by tools/typed_lift2.py (round A3; loop leaf, extra outputs xo[0]=f0 xo[1]=f1 xo[2]=a2)
inline uint32_t kernel_sub_001DAC18(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t a0, uint32_t a1, uint64_t a2, uint64_t *xo = nullptr)
{
    float f0 = 0.0f;
    float f1 = 0.0f;
    uint32_t v0 = 0;
    uint32_t v1 = 0;
    bool a3xw0 = false;
    bool a3xw1 = false;
    bool a3xw2 = false;
    v0 = A1_R32B(a0, 4);
    { const bool c = (int32_t)a2 <= 0; v1 = A1_R32B(a1, 4); if (c) goto L_1dac48; }
L_1dac28:;
    f0 = a1_flt(A1_R32(v0)); a3xw0 = true;
    (a2 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)a2, 4294967295)))); a3xw2 = true;
    f1 = a1_flt(A1_R32(v1)); a3xw1 = true;
    v1 = v1 + 4u;
    f0 = a1f::add(f0, f1); a3xw0 = true;
    A1_W32(v0, a1_bits(f0));
    { const bool c = (int64_t)a2 != 0; v0 = v0 + 4u; if (c) { A3_BACK(); goto L_1dac28; } }
L_1dac48:;
    { if (xo) { if (a3xw0) xo[0] = a1_bits(f0); if (a3xw1) xo[1] = a1_bits(f1); if (a3xw2) xo[2] = a2; } return v0; }
    __builtin_unreachable();
}

// kernel_sub_001DB678 (0x1db678): typed by tools/typed_lift2.py (round A3; loop leaf, extra outputs xo[0]=a2)
inline uint64_t kernel_sub_001DB678(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint64_t *xo = nullptr)
{
    uint64_t v0 = 0;
    uint32_t v1 = 0;
    uint32_t a0 = 0;
    uint64_t a1 = 0;
    uint32_t a2 = 0;
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)41 << 16))));
    (a1 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v0, 2008))));
    (a2 = (uint32_t)((int32_t)((int32_t)ADD32((uint32_t)v0, 4))));
    v1 = A1_R32(a2);
L_1db690:;
    a2 = a2 + 0x1D4u;
    (v0 = (uint64_t)((uint64_t)a1 + (uint64_t)0ULL));
    (a1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)a1, 1))));
    { const bool c = v1 == 0; (a0 = (uint32_t)(((int64_t)(int64_t)RC_U64X(a1) < (int64_t)(int32_t)4) ? 1 : 0)); if (c) goto L_1db6b8; }
    if (a0 != 0) { v1 = A1_R32(a2); { A3_BACK(); goto L_1db690; } }
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 4294967295))));
L_1db6b8:;
    { if (xo) { xo[0] = a1_sx64(a2); } return v0; }
    __builtin_unreachable();
}

// IirFilter (0x1dcb48): typed by tools/typed_lift2.py (round A3; loop frame leaf, frame 16 bytes, 8 slot accesses)
inline a1::RetVF IirFilter(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t a0, uint64_t a1, uint32_t a2)
{
    float f0 = 0.0f;
    float f1 = 0.0f;
    float f2 = 0.0f;
    float f3 = 0.0f;
    float f4 = 0.0f;
    float f5 = 0.0f;
    float f6 = 0.0f;
    float f7 = 0.0f;
    float f8 = 0.0f;
    float f9 = 0.0f;
    float f10 = 0.0f;
    float f11 = 0.0f;
    float f12 = 0.0f;
    float f13 = 0.0f;
    float f14 = 0.0f;
    uint32_t v0 = 0;
    uint32_t v1 = 0;
    uint8_t a2fr[16];
    f13 = a1_flt(A1_R32B(a0, 52));
    f12 = a1_flt(A1_R32B(a0, 56));
    v0 = 2u;
    f0 = a1_flt(A1_R32B(a0, 32));
    (v0 = (uint32_t)(((int64_t)(int64_t)a1_sx64(v0) < (int64_t)(int64_t)a1) ? 1 : 0));
    f8 = a1_flt(A1_R32(a2));
    f0 = a1f::mul(f12, f0);
    f11 = a1_flt(A1_R32B(a0, 60));
    f3 = a1f::mul(f13, f8);
    f4 = a1_flt(A1_R32B(a0, 28));
    f10 = a1_flt(A1_R32B(a0, 64));
    f6 = a1f::mul(f12, f8);
    f4 = a1f::mul(f11, f4);
    { uint32_t bits = A1_R32B(a0, 40); float f; std::memcpy(&f, &bits, sizeof(f)); f5 = f; }
    f9 = a1_flt(A1_R32B(a0, 68));
    f3 = a1f::addFinite(f3, f0);
    f2 = a1_flt(A1_R32B(a0, 36));
    f5 = a1f::mul(f10, f5);
    f7 = a1_flt(A1_R32B(a2, 4));
    f2 = a1f::mul(f9, f2);
    f14 = a1_flt(A1_R32B(a2, 8));
    f1 = a1f::mul(f13, f7);
    a2::frSt<uint32_t>(a2fr, 0, a1_bits(f8));
    f3 = a1f::addFinite(f3, f4);
    a2::frSt<uint32_t>(a2fr, 4, a1_bits(f7));
    a2::frSt<uint32_t>(a2fr, 8, a1_bits(f14));
    f1 = a1f::addFinite(f1, f6);
    f3 = a1f::subFinite(f3, f5);
    f3 = a1f::subFinite(f3, f2);
    A1_W32(a2, a1_bits(f3));
    f3 = a1f::mul(f10, f3);
    f0 = a1_flt(A1_R32B(a0, 32));
    f2 = a1_flt(A1_R32B(a0, 40));
    f0 = a1f::mul(f11, f0);
    f2 = a1f::mul(f9, f2);
    f1 = a1f::addFinite(f1, f0);
    f1 = a1f::subFinite(f1, f3);
    f1 = a1f::subFinite(f1, f2);
    { const bool c = v0 == 0; A1_W32B(a2, 4, a1_bits(f1)); if (c) goto L_1dcc54; }
    f5 = ((float)(f7));
    f7 = ((float)(f8));
    f6 = ((float)(f14));
    v1 = a2 + 8u;
    (v0 = (uint32_t)((int32_t)((int32_t)ADD32((uint32_t)a1, 4294967294))));
L_1dcc00:;
    f0 = a1f::mul(f13, f6);
    f2 = a1_flt(A1_R32(v1 - 4u));
    f3 = a1f::mul(f12, f5);
    f1 = a1_flt(A1_R32(v1 - 8u));
    f4 = a1f::mul(f11, f7);
    f7 = ((float)(f5));
    f2 = a1f::mul(f10, f2);
    v0 = v0 - 1u;
    f1 = a1f::mul(f9, f1);
    f5 = ((float)(f6));
    f0 = a1f::addFinite(f0, f3);
    f6 = a1_flt(A1_R32B(v1, 4));
    f0 = a1f::addFinite(f0, f4);
    f0 = a1f::subFinite(f0, f2);
    f0 = a1f::subFinite(f0, f1);
    A1_W32(v1, a1_bits(f0));
    { const bool c = v0 != 0; v1 = v1 + 4u; if (c) { A3_BACK(); goto L_1dcc00; } }
    a2::frSt<uint32_t>(a2fr, 0, a1_bits(f7));
    { float f = f5; uint32_t bits; std::memcpy(&bits, &f, sizeof(bits)); a2::frSt<uint32_t>(a2fr, 4, bits); }
    a2::frSt<uint32_t>(a2fr, 8, a1_bits(f6));
L_1dcc54:;
    f0 = a1_flt(a2::frLd<uint32_t>(a2fr, 0));
    (v0 = (uint32_t)((int32_t)((int32_t)SLL32((uint32_t)a1, 2))));
    f1 = a1_flt(a2::frLd<uint32_t>(a2fr, 4));
    v0 = a2 + v0;
    A1_W32B(a0, 28, a1_bits(f0));
    A1_W32B(a0, 32, a1_bits(f1));
    f0 = a1_flt(A1_R32(v0 - 8u));
    A1_W32B(a0, 36, a1_bits(f0));
    f1 = a1_flt(A1_R32(v0 - 4u));
    A1_W32B(a0, 40, a1_bits(f1));
    { return a1::RetVF{(uint64_t)a1_sx64(v0), f0}; }
    __builtin_unreachable();
}

// ps2___ieee754_fmodf (0x1de720): typed by tools/typed_lift2.py (round A3; loop frame leaf, frame 16 bytes, 2 slot accesses)
inline a1::RetVF ps2___ieee754_fmodf(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, float f12, float f13)
{
    float f0 = 0.0f;
    float f1 = 0.0f;
    uint32_t at = 0;
    uint64_t v0 = 0;
    uint64_t v1 = 0;
    uint32_t a0 = 0;
    uint64_t a1 = 0;
    uint64_t a2 = 0;
    uint32_t a3 = 0;
    uint32_t t0 = 0;
    uint64_t t1 = 0;
    uint64_t t2 = 0;
    uint32_t fcr31 = 0;
    uint8_t a2fr[16];
    f0 = ((float)(f12));
    { uint32_t bits; std::memcpy(&bits, &f0, sizeof(bits)); (v0 = (uint64_t)((int64_t)(int32_t)(bits))); }
    (a1 = (uint64_t)((uint64_t)RC_U64X(v0) + (uint64_t)0ULL));
    f1 = ((float)(f13));
    a2::frSt<uint32_t>(a2fr, 0, a1_bits(f1));
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)32768 << 16))));
    a3 = a2::frLd<uint32_t>(a2fr, 0);
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)32767 << 16))));
    (v0 = (uint64_t)(RC_U64X(v0) | (uint64_t)(uint16_t)65535));
    a0 = 0x7F0000u;
    (a2 = (uint64_t)(a1_sx64(a3) & RC_U64X(v0)));
    a0 = a0 | 0xFFFFu;
    (t2 = (uint64_t)(RC_U64X(a1) & RC_U64X(v1)));
    (v0 = (uint64_t)(((int64_t)(int64_t)a1_sx64(a0) < (int64_t)(int64_t)RC_U64X(a2)) ? 1 : 0));
    { const bool c = (int64_t)v0 != 0; (a1 = (uint64_t)(RC_U64X(a1) ^ RC_U64X(t2))); if (c) goto L_1de780; }
    f0 = a1f::mul(f0, f13);
    f0 = a1f::div(fcr31, f0, f0);
    goto L_1de8e8;
L_1de780:;
    (v0 = (uint64_t)(((int64_t)(int64_t)a1 < (int64_t)(int64_t)a2) ? 1 : 0));
    if ((int64_t)v0 != 0) goto L_1de8e8;
    { const bool c = (int64_t)a1 == (int64_t)a2; (v0 = (uint64_t)((int64_t)(int32_t)(SRA32((int32_t)a1, 23)))); if (c) goto L_1de850; }
    (t0 = (uint32_t)((int32_t)((int32_t)ADD32((uint32_t)v0, 4294967169))));
    (v1 = (uint64_t)((int64_t)(int32_t)(SRA32((int32_t)a2, 23))));
    (v0 = (uint64_t)(((int64_t)(int64_t)a1_sx64(t0) < (int64_t)(int32_t)4294967170) ? 1 : 0));
    { const bool c = (int64_t)v0 != 0; (a3 = (uint32_t)((int32_t)((int32_t)ADD32((uint32_t)v1, 4294967169)))); if (c) goto L_1de7b8; }
    (v1 = (uint64_t)(a1 & a1_sx64(a0)));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)128 << 16))));
    (a1 = (uint64_t)(v1 | RC_U64X(v0)));
    goto L_1de7c4;
L_1de7b8:;
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 4294967170))));
    (a0 = (uint32_t)((int32_t)((int32_t)SUB32((uint32_t)v0, t0))));
    (a1 = (uint64_t)((int64_t)(int32_t)((int32_t)SLL32((uint32_t)a1, a0 & 0x1F))));
L_1de7c4:;
    (t1 = (uint64_t)(((int64_t)(int64_t)a1_sx64(a3) < (int64_t)(int32_t)4294967170) ? 1 : 0));
    { const bool c = (int64_t)t1 != 0; (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 4294967170)))); if (c) goto L_1de7e8; }
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)127 << 16))));
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)128 << 16))));
    (v0 = (uint64_t)(RC_U64X(v0) | (uint64_t)(uint16_t)65535));
    (v0 = (uint64_t)(a2 & RC_U64X(v0)));
    (a2 = (uint64_t)(v0 | RC_U64X(v1)));
    goto L_1de7f0;
L_1de7e8:;
    (a0 = (uint32_t)((int32_t)((int32_t)SUB32((uint32_t)v0, a3))));
    (a2 = (uint64_t)((int64_t)(int32_t)((int32_t)SLL32((uint32_t)a2, a0 & 0x1F))));
L_1de7f0:;
    a0 = t0 - a3;
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 4294967295))));
L_1de7f8:;
    a0 = a0 - 1u;
    { const bool c = (int64_t)(int32_t)a0 == (int64_t)v0; (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)SUB32((uint32_t)a1, (uint32_t)a2)))); if (c) goto L_1de840; }
    { const bool c = (int32_t)v1 < 0; (a1 = (uint64_t)((int64_t)(int32_t)((int32_t)SLL32((uint32_t)a1, 1)))); if (c) { A3_BACK(); goto L_1de7f8; } }
    if ((int64_t)v1 == 0) { (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)SRL32((uint32_t)t2, 31)))); goto L_1de854; }
    (a1 = (uint64_t)((int64_t)(int32_t)((int32_t)SLL32((uint32_t)v1, 1))));
    { A3_BACK(); goto L_1de7f8; }
L_1de840:;
    (v0 = (uint64_t)(((int64_t)(int64_t)v1 < (int64_t)(int32_t)0) ? 1 : 0));
    if (RC_U64X(v0) == 0) a1 = (uint64_t)(v1);;
    { const bool c = (int64_t)a1 != 0; (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)127 << 16)))); if (c) goto L_1de870; }
L_1de850:;
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)SRL32((uint32_t)t2, 31))));
L_1de854:;
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)SLL32((uint32_t)v0, 2))));
    at = 0x1E0000u;
    (at = (uint32_t)((int32_t)((int32_t)ADD32(at, (uint32_t)v0))));
    { uint32_t bits = A1_R32B(at, 19304); float f; std::memcpy(&f, &bits, sizeof(f)); f0 = f; }
    goto L_1de8e8;
L_1de870:;
    (v0 = (uint64_t)(v0 | (uint64_t)(uint16_t)65535));
    (v0 = (uint64_t)(((int64_t)(int64_t)v0 < (int64_t)(int64_t)a1) ? 1 : 0));
    if ((int64_t)v0 != 0) goto L_1de8a8;
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)127 << 16))));
    (v1 = (uint64_t)(RC_U64X(v1) | (uint64_t)(uint16_t)65535));
L_1de888:;
    (a1 = (uint64_t)((int64_t)(int32_t)((int32_t)SLL32((uint32_t)a1, 1))));
    (v0 = (uint64_t)(((int64_t)(int64_t)v1 < (int64_t)(int64_t)RC_U64X(a1)) ? 1 : 0));
    { const bool c = (int64_t)v0 == 0; a3 = a3 - 1u; if (c) { A3_BACK(); goto L_1de888; } }
    (t1 = (uint64_t)(((int64_t)(int64_t)a1_sx64(a3) < (int64_t)(int32_t)4294967170) ? 1 : 0));
L_1de8a8:;
    { const bool c = (int64_t)t1 != 0; (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 4294967170)))); if (c) goto L_1de8d8; }
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(a3, 127))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)65408 << 16))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)a1, (uint32_t)v0))));
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)SLL32((uint32_t)v1, 23))));
    (a1 = (uint64_t)(RC_U64X(v0) | RC_U64X(v1)));
    (a1 = (uint64_t)(RC_U64X(a1) | t2));
    { uint32_t bits = (uint32_t)a1; std::memcpy(&f0, &bits, sizeof(bits)); }
    goto L_1de8e8;
L_1de8d8:;
    (a0 = (uint32_t)((int32_t)((int32_t)SUB32((uint32_t)v0, a3))));
    (a1 = (uint64_t)((int64_t)(int32_t)(SRA32((int32_t)a1, a0 & 0x1F))));
    (a1 = (uint64_t)(RC_U64X(a1) | t2));
    { uint32_t bits = (uint32_t)a1; std::memcpy(&f0, &bits, sizeof(bits)); }
L_1de8e8:;
    { return a1::RetVF{(uint64_t)v0, f0}; }
    __builtin_unreachable();
}

// GAMEUTIL_inc_or_dec_for_near_to_aim_pitch (0x107da28): typed by tools/typed_lift2.py (round A3; leaf, extra outputs xo[0]=f0)
inline uint64_t GAMEUTIL_inc_or_dec_for_near_to_aim_pitch(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint64_t a0, uint64_t a1, float f12, float f13, float f14, float f15, float f16, uint64_t *xo = nullptr)
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
    uint32_t fcr31 = 0;
    bool a3xw0 = false;
    f1 = ((float)(f12));
    fcr31 = (a1f::eq(f1, f14)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
    { const bool c = (fcr31 & 0x800000u) != 0u; (v0 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL)); if (c) goto L_107dd70; }
    { uint32_t bits = 0u; std::memcpy(&f0, &bits, sizeof(bits)); } a3xw0 = true;
    at = 0x38D10000u;
    at = at | 0xB717u;
    { uint32_t bits = at; std::memcpy(&f1, &bits, sizeof(bits)); }
    fcr31 = (a1f::lt(f13, f0)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
    if ((fcr31 & 0x800000u) == 0u) goto L_107da70;
    f0 = (-(float)(f13)); a3xw0 = true;
    fcr31 = (a1f::le(f0, f1)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
    { const bool c = (fcr31 & 0x800000u) != 0u; (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 1)))); if (c) goto L_107da80; }
    (v0 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL));
    goto L_107da80;
L_107da70:;
    fcr31 = (a1f::le(f13, f1)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
    if ((fcr31 & 0x800000u) == 0u) { (v0 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL)); goto L_107da80; }
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 1))));
L_107da80:;
    at = 0x3F800000u;
    { uint32_t bits = at; std::memcpy(&f6, &bits, sizeof(bits)); }
    if ((int64_t)v0 == 0) { f6 = ((float)(f13)); goto L_107da90; }
L_107da90:;
    { uint32_t bits = 0u; std::memcpy(&f0, &bits, sizeof(bits)); } a3xw0 = true;
    f1 = ((float)(f15));
    f3 = ((float)(f16));
    fcr31 = (a1f::lt(f6, f0)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
    at = 0x38D10000u;
    at = at | 0xB717u;
    { uint32_t bits = at; std::memcpy(&f0, &bits, sizeof(bits)); } a3xw0 = true;
    { const bool c = (fcr31 & 0x800000u) == 0u; f4 = ((float)(f12)); if (c) goto L_107dad0; }
    f2 = (-(float)(f6));
    fcr31 = (a1f::le(f2, f0)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
    if ((fcr31 & 0x800000u) != 0u) { (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 1)))); goto L_107daec; }
    (v0 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL));
    goto L_107daec;
L_107dad0:;
    fcr31 = (a1f::le(f6, f0)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
    { const bool c = (fcr31 & 0x800000u) == 0u; f2 = (-(float)(f6)); if (c) goto L_107dae8; }
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 1))));
    goto L_107daec;
L_107dae8:;
    (v0 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL));
L_107daec:;
    if ((int64_t)v0 != 0) goto L_107dbc8;
    at = 0x3FBD0000u;
    at = at | 0xE44Eu;
    { uint32_t bits = at; std::memcpy(&f0, &bits, sizeof(bits)); } a3xw0 = true;
    fcr31 = (a1f::lt(f0, f15)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
    if ((fcr31 & 0x800000u) == 0u) goto L_107db18;
    f1 = ((float)(f0));
    goto L_107db30;
L_107db18:;
    at = 0xBFBD0000u;
    at = at | 0xE44Eu;
    { uint32_t bits = at; std::memcpy(&f0, &bits, sizeof(bits)); } a3xw0 = true;
    fcr31 = (a1f::lt(f16, f0)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
    if ((fcr31 & 0x800000u) != 0u) { f3 = ((float)(f0)); goto L_107db30; }
L_107db30:;
    fcr31 = (a1f::lt(f1, f12)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
    if ((fcr31 & 0x800000u) == 0u) goto L_107db70;
    { uint32_t bits = 0u; std::memcpy(&f0, &bits, sizeof(bits)); } a3xw0 = true;
    fcr31 = (a1f::lt(f6, f0)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
    { const bool c = (fcr31 & 0x800000u) == 0u; f3 = ((float)(f15)); if (c) goto L_107dbd0; }
    f4 = a1f::add(f12, f6);
    fcr31 = (a1f::lt(f4, f1)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
    { const bool c = (fcr31 & 0x800000u) == 0u; f5 = ((float)(f16)); if (c) goto L_107dbd4; }
    if ((int64_t)a1 == 0) goto L_107dbd4;
    f4 = ((float)(f1));
    goto L_107dbd4;
L_107db70:;
    fcr31 = (a1f::lt(f12, f3)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
    if ((fcr31 & 0x800000u) == 0u) { f4 = a1f::add(f12, f6); goto L_107dbb0; }
    { uint32_t bits = 0u; std::memcpy(&f0, &bits, sizeof(bits)); } a3xw0 = true;
    fcr31 = (a1f::lt(f0, f6)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
    if ((fcr31 & 0x800000u) == 0u) { f3 = ((float)(f15)); goto L_107dbd0; }
    f4 = a1f::add(f12, f6);
    fcr31 = (a1f::lt(f3, f4)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
    if ((fcr31 & 0x800000u) == 0u) { f3 = ((float)(f15)); goto L_107dbd0; }
    if ((int64_t)a1 == 0) { f3 = ((float)(f15)); goto L_107dbd0; }
    f4 = ((float)(f3));
    goto L_107dbcc;
L_107dbb0:;
    fcr31 = (a1f::lt(f1, f4)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
    if ((fcr31 & 0x800000u) != 0u) { f4 = ((float)(f1)); goto L_107dbc8; }
    fcr31 = (a1f::lt(f4, f3)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
    if ((fcr31 & 0x800000u) != 0u) { f4 = ((float)(f3)); goto L_107dbc8; }
L_107dbc8:;
    { uint32_t bits = 0u; std::memcpy(&f0, &bits, sizeof(bits)); } a3xw0 = true;
L_107dbcc:;
    f3 = ((float)(f15));
L_107dbd0:;
    f5 = ((float)(f16));
L_107dbd4:;
    fcr31 = (a1f::lt(f2, f0)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
    at = 0x38D10000u;
    at = at | 0xB717u;
    { uint32_t bits = at; std::memcpy(&f0, &bits, sizeof(bits)); } a3xw0 = true;
    { const bool c = (fcr31 & 0x800000u) == 0u; f1 = ((float)(f12)); if (c) goto L_107dc00; }
    fcr31 = (a1f::le(f6, f0)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
    if ((fcr31 & 0x800000u) != 0u) { (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 1)))); goto L_107dc10; }
    (v0 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL));
    goto L_107dc10;
L_107dc00:;
    fcr31 = (a1f::le(f2, f0)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
    if ((fcr31 & 0x800000u) == 0u) { (v0 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL)); goto L_107dc10; }
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 1))));
L_107dc10:;
    if ((int64_t)v0 != 0) { f0 = a1f::sub(f14, f4); a3xw0 = true; goto L_107dcec; }
    at = 0x3FBD0000u;
    at = at | 0xE44Eu;
    { uint32_t bits = at; std::memcpy(&f0, &bits, sizeof(bits)); } a3xw0 = true;
    fcr31 = (a1f::lt(f0, f15)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
    if ((fcr31 & 0x800000u) == 0u) goto L_107dc38;
    f3 = ((float)(f0));
    goto L_107dc50;
L_107dc38:;
    at = 0xBFBD0000u;
    at = at | 0xE44Eu;
    { uint32_t bits = at; std::memcpy(&f0, &bits, sizeof(bits)); } a3xw0 = true;
    fcr31 = (a1f::lt(f16, f0)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
    if ((fcr31 & 0x800000u) != 0u) { f5 = ((float)(f0)); goto L_107dc50; }
L_107dc50:;
    fcr31 = (a1f::lt(f3, f12)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
    if ((fcr31 & 0x800000u) == 0u) goto L_107dc90;
    { uint32_t bits = 0u; std::memcpy(&f0, &bits, sizeof(bits)); } a3xw0 = true;
    fcr31 = (a1f::lt(f2, f0)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
    if ((fcr31 & 0x800000u) == 0u) { f0 = a1f::sub(f14, f4); a3xw0 = true; goto L_107dcec; }
    f1 = a1f::add(f12, f2);
    fcr31 = (a1f::lt(f1, f3)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
    if ((fcr31 & 0x800000u) == 0u) { f0 = a1f::sub(f14, f4); a3xw0 = true; goto L_107dcec; }
    if ((int64_t)a1 == 0) { f0 = a1f::sub(f14, f4); a3xw0 = true; goto L_107dcec; }
    f1 = ((float)(f3));
    goto L_107dce8;
L_107dc90:;
    fcr31 = (a1f::lt(f12, f5)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
    if ((fcr31 & 0x800000u) == 0u) { f1 = a1f::add(f12, f2); goto L_107dcd0; }
    { uint32_t bits = 0u; std::memcpy(&f0, &bits, sizeof(bits)); } a3xw0 = true;
    fcr31 = (a1f::lt(f0, f2)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
    if ((fcr31 & 0x800000u) == 0u) { f0 = a1f::sub(f14, f4); a3xw0 = true; goto L_107dcec; }
    f1 = a1f::add(f12, f2);
    fcr31 = (a1f::lt(f5, f1)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
    if ((fcr31 & 0x800000u) == 0u) { f0 = a1f::sub(f14, f4); a3xw0 = true; goto L_107dcec; }
    if ((int64_t)a1 == 0) { f0 = a1f::sub(f14, f4); a3xw0 = true; goto L_107dcec; }
    f1 = ((float)(f5));
    goto L_107dce8;
L_107dcd0:;
    fcr31 = (a1f::lt(f3, f1)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
    if ((fcr31 & 0x800000u) != 0u) { f1 = ((float)(f3)); goto L_107dce8; }
    fcr31 = (a1f::lt(f1, f5)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
    if ((fcr31 & 0x800000u) != 0u) { f1 = ((float)(f5)); goto L_107dce8; }
L_107dce8:;
    f0 = a1f::sub(f14, f4); a3xw0 = true;
L_107dcec:;
    { uint32_t bits = 0u; std::memcpy(&f2, &bits, sizeof(bits)); }
    fcr31 = (a1f::lt(f0, f2)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
    if ((fcr31 & 0x800000u) != 0u) { f0 = (-(float)(f0)); a3xw0 = true; goto L_107dcfc; }
L_107dcfc:;
    f14 = a1f::sub(f14, f1);
    fcr31 = (a1f::lt(f14, f2)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
    if ((fcr31 & 0x800000u) != 0u) { f14 = (-(float)(f14)); goto L_107dd0c; }
L_107dd0c:;
    fcr31 = (a1f::lt(f0, f14)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
    if ((fcr31 & 0x800000u) == 0u) { (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 4294967295)))); goto L_107dd20; }
    f1 = ((float)(f4));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 1))));
L_107dd20:;
    { uint32_t bits = 0u; std::memcpy(&f0, &bits, sizeof(bits)); } a3xw0 = true;
    at = 0x38D10000u;
    at = at | 0xB717u;
    { uint32_t bits = at; std::memcpy(&f2, &bits, sizeof(bits)); }
    fcr31 = (a1f::lt(f13, f0)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
    if ((fcr31 & 0x800000u) == 0u) goto L_107dd58;
    f0 = (-(float)(f13)); a3xw0 = true;
    fcr31 = (a1f::le(f0, f2)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
    if ((fcr31 & 0x800000u) != 0u) { (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 1)))); goto L_107dd68; }
    (v1 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL));
    goto L_107dd68;
L_107dd58:;
    fcr31 = (a1f::le(f13, f2)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
    if ((fcr31 & 0x800000u) == 0u) { (v1 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL)); goto L_107dd68; }
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 1))));
L_107dd68:;
    if ((int64_t)v1 != 0) { f1 = ((float)(f12)); goto L_107dd70; }
L_107dd70:;
    if ((int64_t)a0 != 0) { { float f = f1; uint32_t bits; std::memcpy(&bits, &f, sizeof(bits)); A1_W32B((uint32_t)a0, 0, bits); } goto L_107dd78; }
L_107dd78:;
    { if (xo) { if (a3xw0) xo[0] = a1_bits(f0); } return v0; }
    __builtin_unreachable();
}

// GAMEUTIL_get_interp_pitch (0x107dd80): typed by tools/typed_lift2.py (round A3; leaf, extra outputs xo[0]=f13 xo[1]=v0)
inline float GAMEUTIL_get_interp_pitch(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, float f12, float f13, float f14, uint64_t *xo = nullptr)
{
    float f0 = 0.0f;
    float f1 = 0.0f;
    float f2 = 0.0f;
    float f3 = 0.0f;
    float f4 = 0.0f;
    float f5 = 0.0f;
    uint32_t at = 0;
    uint64_t v0 = 0;
    uint32_t fcr31 = 0;
    bool a3xw0 = false;
    bool a3xw1 = false;
    f5 = a1_flt(0u);
    fcr31 = (a1f::le(f14, f5)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
    { const bool c = (fcr31 & 0x800000u) != 0u; f0 = ((float)(f12)); if (c) goto L_107deb8; }
    at = 0x3F800000u;
    f1 = a1_flt(at);
    fcr31 = (a1f::le(f1, f14)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
    { const bool c = (fcr31 & 0x800000u) != 0u; f0 = ((float)(f13)); if (c) goto L_107deb8; }
    f0 = a1f::sub(f13, f12);
    at = 0x3FBD0000u;
    at = at | 0xE44Eu;
    f3 = a1_flt(at);
    at = 0xBFBD0000u;
    at = at | 0xE44Eu;
    f4 = a1_flt(at);
    at = 0x38D10000u;
    at = at | 0xB717u;
    { uint32_t bits = at; std::memcpy(&f2, &bits, sizeof(bits)); }
    f13 = a1f::mul(f14, f0); a3xw0 = true;
    fcr31 = (a1f::lt(f13, f5)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
    { const bool c = (fcr31 & 0x800000u) == 0u; f1 = ((float)(f12)); if (c) goto L_107ddf8; }
    f0 = (-(float)(f13));
    fcr31 = (a1f::le(f0, f2)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
    if ((fcr31 & 0x800000u) != 0u) { (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 1)))); a3xw1 = true; goto L_107de08; }
    (v0 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL)); a3xw1 = true;
    goto L_107de08;
L_107ddf8:;
    fcr31 = (a1f::le(f13, f2)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
    if ((fcr31 & 0x800000u) == 0u) { (v0 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL)); a3xw1 = true; goto L_107de08; }
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 1)))); a3xw1 = true;
L_107de08:;
    { const bool c = (int64_t)v0 != 0; f0 = ((float)(f1)); if (c) goto L_107deb8; }
    at = 0x3FBD0000u;
    at = at | 0xE44Eu;
    f0 = a1_flt(at);
    fcr31 = (a1f::lt(f0, f3)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
    if ((fcr31 & 0x800000u) == 0u) goto L_107de30;
    f3 = ((float)(f0));
    goto L_107de48;
L_107de30:;
    at = 0xBFBD0000u;
    at = at | 0xE44Eu;
    f0 = a1_flt(at);
    fcr31 = (a1f::lt(f4, f0)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
    if ((fcr31 & 0x800000u) != 0u) { f4 = ((float)(f0)); goto L_107de48; }
L_107de48:;
    fcr31 = (a1f::lt(f3, f12)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
    if ((fcr31 & 0x800000u) == 0u) goto L_107de68;
    f0 = a1_flt(0u);
    fcr31 = (a1f::lt(f13, f0)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
    goto L_107de7c;
L_107de68:;
    fcr31 = (a1f::lt(f12, f4)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
    if ((fcr31 & 0x800000u) == 0u) { f1 = a1f::add(f12, f13); goto L_107de90; }
    f0 = a1_flt(0u);
    fcr31 = (a1f::lt(f0, f13)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
L_107de7c:;
    if ((fcr31 & 0x800000u) != 0u) { f1 = a1f::add(f12, f13); goto L_107deb4; }
    { f0 = ((float)(f1)); if (xo) { xo[0] = a1_bits(f13); xo[1] = v0; } return f0; }
L_107de90:;
    fcr31 = (a1f::lt(f3, f1)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
    if ((fcr31 & 0x800000u) == 0u) goto L_107dea8;
    f1 = ((float)(f3));
    goto L_107deb4;
L_107dea8:;
    fcr31 = (a1f::lt(f1, f4)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
    if ((fcr31 & 0x800000u) != 0u) { f1 = ((float)(f4)); goto L_107deb4; }
L_107deb4:;
    f0 = ((float)(f1));
L_107deb8:;
    { if (xo) { if (a3xw0) xo[0] = a1_bits(f13); if (a3xw1) xo[1] = v0; } return f0; }
    __builtin_unreachable();
}

// GAMEUTIL_is_diff_nearly_equal_zero (0x107ed30): typed by tools/typed_lift2.py (round A3; leaf, extra outputs xo[0]=f12)
inline a1::RetVF GAMEUTIL_is_diff_nearly_equal_zero(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, float f12, float f13, float f14, uint64_t *xo = nullptr)
{
    float f0 = 0.0f;
    uint64_t v0 = 0;
    uint32_t fcr31 = 0;
    f12 = a1f::sub(f12, f13);
    f0 = a1_flt(0u);
    fcr31 = (a1f::lt(f12, f0)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
    if ((fcr31 & 0x800000u) == 0u) goto L_107ed60;
    f0 = (-(float)(f12));
    fcr31 = (a1f::le(f0, f14)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
    if ((fcr31 & 0x800000u) != 0u) goto L_107ed6c;
    { (v0 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL)); if (xo) { xo[0] = a1_bits(f12); } return a1::RetVF{(uint64_t)v0, f0}; }
L_107ed60:;
    fcr31 = (a1f::le(f12, f14)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
    if ((fcr31 & 0x800000u) == 0u) { (v0 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL)); goto L_107ed78; }
L_107ed6c:;
    { (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 1)))); if (xo) { xo[0] = a1_bits(f12); } return a1::RetVF{(uint64_t)v0, f0}; }
L_107ed78:;
    { if (xo) { xo[0] = a1_bits(f12); } return a1::RetVF{(uint64_t)v0, f0}; }
    __builtin_unreachable();
}

// GAMEUTIL_get_cross_position_coefficient_t_by_dist_factor (0x107eec8): typed by tools/typed_lift2.py (round A3; loop leaf, extra outputs xo[0]=t1 xo[1]=t2)
inline a1::RetVF GAMEUTIL_get_cross_position_coefficient_t_by_dist_factor(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint64_t a0, uint32_t a1, uint32_t a2, uint32_t a3, uint32_t t0, float f12, uint64_t *xo = nullptr)
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
    uint32_t v1 = 0;
    uint64_t t1 = 0;
    uint64_t t2 = 0;
    uint32_t fcr31 = 0;
    { uint32_t bits = 0u; std::memcpy(&f6, &bits, sizeof(bits)); }
    (t2 = (uint64_t)((uint64_t)a0 + (uint64_t)0ULL));
    (t1 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL));
    f5 = ((float)(f6));
    f4 = ((float)(f6));
L_107eee0:;
    (v1 = (uint32_t)((int32_t)((int32_t)SLL32((uint32_t)t1, 2))));
    (t1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)t1, 1))));
    v0 = v1 + a3;
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(v1, t0))));
    f1 = a1_flt(A1_R32(v0));
    (v0 = (uint32_t)(((int64_t)(int64_t)RC_U64X(t1) < (int64_t)(int32_t)3) ? 1 : 0));
    { uint32_t bits = A1_R32B((uint32_t)a0, 0); float f; std::memcpy(&f, &bits, sizeof(f)); f0 = f; }
    v1 = v1 + a2;
    f2 = a1_flt(A1_R32(v1));
    f0 = a1f::sub(f0, f1);
    f1 = a1f::sub(f1, f2);
    f3 = a1f::addFinite(f0, f0);
    f2 = ((float)(f1) * (float)(f1));
    f0 = ((float)(f0) * (float)(f0));
    f3 = ((float)(f3) * (float)(f1));
    f4 = a1f::add(f4, f2);
    f6 = a1f::add(f6, f0);
    { const bool c = v0 != 0; f5 = a1f::add(f5, f3); if (c) { A3_BACK(); goto L_107eee0; } }
    f2 = a1f::mul(f12, f12);
    at = 0x40800000u;
    { uint32_t bits = at; std::memcpy(&f0, &bits, sizeof(bits)); }
    f3 = a1f::mul(f5, f5);
    f0 = ((float)(f6) * (float)(f0));
    { uint32_t bits = 0u; std::memcpy(&f1, &bits, sizeof(bits)); }
    f4 = a1f::subFinite(f4, f2);
    f0 = ((float)(f0) * (float)(f4));
    f3 = a1f::subFinite(f3, f0);
    fcr31 = (a1f::lt(f3, f1)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
    { const bool c = (fcr31 & 0x800000u) == 0u; v0 = 1u; if (c) goto L_107ef60; }
    v0 = 0u;
L_107ef60:;
    f3 = a1f::sqrt(fcr31, f3);
    f0 = (-(float)(f5));
    f2 = a1f::addFinite(f6, f6);
    f1 = a1f::sub(f3, f5);
    f0 = a1f::sub(f0, f3);
    f1 = a1f::div(fcr31, f1, f2);
    f0 = a1f::div(fcr31, f0, f2);
    { float f = f1; uint32_t bits; std::memcpy(&bits, &f, sizeof(bits)); A1_W32B((uint32_t)t2, 0, bits); }
    A1_W32(a1, a1_bits(f0));
    { uint32_t bits = A1_R32B((uint32_t)t2, 0); float f; std::memcpy(&f, &bits, sizeof(f)); f1 = f; }
    fcr31 = (a1f::lt(f1, f0)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
    if ((fcr31 & 0x800000u) == 0u) goto L_107efac;
    { float f = f0; uint32_t bits; std::memcpy(&bits, &f, sizeof(bits)); A1_W32B((uint32_t)t2, 0, bits); }
    A1_W32(a1, a1_bits(f1));
L_107efac:;
    { if (xo) { xo[0] = t1; xo[1] = t2; } return a1::RetVF{(uint64_t)a1_sx64(v0), f0}; }
    __builtin_unreachable();
}

// GAMEUTIL_get_segment_num_and_data (0x107f078): typed by tools/typed_lift2.py (round A3; leaf, extra outputs xo[0]=a1 xo[1]=a2)
inline uint32_t GAMEUTIL_get_segment_num_and_data(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint64_t a0, uint64_t a1, uint64_t *xo = nullptr)
{
    uint32_t v0 = 0;
    uint32_t v1 = 0;
    uint64_t a2 = 0;
    bool a3xw1 = false;
    { const bool c = (int64_t)a0 == 0; v0 = 0x30000u; if (c) goto L_107f0c8; }
    (v1 = (uint32_t)((int32_t)((int32_t)A1_R32B((uint32_t)a1, 0))));
    v0 = v0 | 0xFFFFu;
    v1 = v1 & v0;
    { const bool c = v1 == 0; (v0 = (uint32_t)((int32_t)((int32_t)SUB32((uint32_t)a1, v1)))); if (c) goto L_107f0a0; }
    (a2 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B(v0, 0)))); a3xw1 = true;
    goto L_107f0a4;
L_107f0a0:;
    (a2 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL)); a3xw1 = true;
L_107f0a4:;
    { const bool c = (int64_t)a2 == 0; { const uint32_t _dynInsn = A1_R32(0x107F0A8u); const int32_t _dynSimm = (int16_t)(_dynInsn & 0xFFFFu); const uint32_t _dynImm = _dynInsn & 0xFFFFu; (void)_dynSimm; (void)_dynImm; (v0 = (uint32_t)((int32_t)((int32_t)((uint32_t)_dynImm << 16)))); } if (c) goto L_107f0c0; }
    { const uint32_t _dynInsn = A1_R32(0x107F0ACu); const int32_t _dynSimm = (int16_t)(_dynInsn & 0xFFFFu); const uint32_t _dynImm = _dynInsn & 0xFFFFu; (void)_dynSimm; (void)_dynImm; (v1 = (uint32_t)((int32_t)((int32_t)A1_R32(ADD32(v0, _dynSimm))))); }
    if ((int64_t)a2 == (int64_t)(int32_t)v1) { v0 = 0u; goto L_107f0c4; }
    (v0 = (uint32_t)((int32_t)((int32_t)A1_R32B((uint32_t)a2, 12))));
    goto L_107f0c4;
L_107f0c0:;
    v0 = 0u;
L_107f0c4:;
    A1_W32B((uint32_t)a0, 0, v0);
L_107f0c8:;
    (v1 = (uint32_t)((int32_t)((int32_t)A1_R32B((uint32_t)a1, 0))));
    v0 = 0x30000u;
    v0 = v0 | 0xFFFFu;
    v0 = v1 & v0;
    { const bool c = v0 == 0; (v0 = (uint32_t)((int32_t)((int32_t)SUB32((uint32_t)a1, v0)))); if (c) goto L_107f0e8; }
    (a1 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B(v0, 0))));
    goto L_107f0ec;
L_107f0e8:;
    (a1 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL));
L_107f0ec:;
    if ((int64_t)a1 == 0) goto L_107f110;
    { const uint32_t _dynInsn = A1_R32(0x107F0F4u); const int32_t _dynSimm = (int16_t)(_dynInsn & 0xFFFFu); const uint32_t _dynImm = _dynInsn & 0xFFFFu; (void)_dynSimm; (void)_dynImm; (v0 = (uint32_t)((int32_t)((int32_t)((uint32_t)_dynImm << 16)))); }
    { const uint32_t _dynInsn = A1_R32(0x107F0F8u); const int32_t _dynSimm = (int16_t)(_dynInsn & 0xFFFFu); const uint32_t _dynImm = _dynInsn & 0xFFFFu; (void)_dynSimm; (void)_dynImm; (v1 = (uint32_t)((int32_t)((int32_t)A1_R32(ADD32(v0, _dynSimm))))); }
    if ((int64_t)a1 == (int64_t)(int32_t)v1) goto L_107f110;
    { (v0 = (uint32_t)((int32_t)((int32_t)A1_R32B((uint32_t)a1, 16)))); if (xo) { xo[0] = a1; if (a3xw1) xo[1] = a2; } return v0; }
L_107f110:;
    { v0 = 0u; if (xo) { xo[0] = a1; if (a3xw1) xo[1] = a2; } return v0; }
    __builtin_unreachable();
}

// GAMEUTIL_get_new_velocity (0x107fa60): typed by tools/typed_lift2.py (round A3; leaf, extra outputs xo[0]=v0)
inline float GAMEUTIL_get_new_velocity(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, float f12, float f13, float f14, float f15, float f16, uint64_t *xo = nullptr)
{
    float f0 = 0.0f;
    float f1 = 0.0f;
    uint32_t at = 0;
    uint32_t v0 = 0;
    uint64_t v1 = 0;
    uint32_t fcr31 = 0;
    bool a3xw0 = false;
    f0 = a1f::sub(f12, f13);
    f1 = a1_flt(0u);
    fcr31 = (a1f::le(f1, f0)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
    if ((fcr31 & 0x800000u) != 0u) { f0 = a1f::mul(f14, f16); goto L_107fac8; }
    fcr31 = (a1f::lt(f0, f1)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
    at = 0x3C230000u;
    at = at | 0xD70Au;
    f1 = a1_flt(at);
    if ((fcr31 & 0x800000u) == 0u) goto L_107faa8;
    f0 = (-(float)(f0));
    fcr31 = (a1f::le(f0, f1)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
    if ((fcr31 & 0x800000u) != 0u) { (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 1)))); goto L_107fab8; }
    (v1 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL));
    goto L_107fab8;
L_107faa8:;
    fcr31 = (a1f::le(f0, f1)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
    if ((fcr31 & 0x800000u) == 0u) { (v1 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL)); goto L_107fab8; }
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 1))));
L_107fab8:;
    v0 = 1u; a3xw0 = true;
    if ((int64_t)v1 != (int64_t)(int32_t)v0) goto L_107facc;
    f0 = a1f::mul(f14, f16);
L_107fac8:;
    f13 = a1f::add(f12, f0);
L_107facc:;
    fcr31 = (a1f::lt(f15, f13)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
    if ((fcr31 & 0x800000u) != 0u) { f13 = ((float)(f15)); goto L_107fad8; }
L_107fad8:;
    { f0 = ((float)(f13)); if (xo) { if (a3xw0) xo[0] = a1_sx64(v0); } return f0; }
    __builtin_unreachable();
}

// GAMEUTIL_increase_pitch (0x10806a8): typed by tools/typed_lift2.py (round A3; leaf, extra outputs xo[0]=f14 xo[1]=f15)
inline a1::RetVF GAMEUTIL_increase_pitch(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint64_t a0, uint64_t a1, float f12, float f13, float f14, float f15, uint64_t *xo = nullptr)
{
    float f0 = 0.0f;
    float f1 = 0.0f;
    float f2 = 0.0f;
    uint32_t at = 0;
    uint64_t v0 = 0;
    uint64_t v1 = 0;
    uint32_t fcr31 = 0;
    bool a3xw0 = false;
    bool a3xw1 = false;
    { uint32_t bits = 0u; std::memcpy(&f0, &bits, sizeof(bits)); }
    (v1 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL));
    at = 0x38D10000u;
    at = at | 0xB717u;
    { uint32_t bits = at; std::memcpy(&f2, &bits, sizeof(bits)); }
    fcr31 = (a1f::lt(f13, f0)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
    { const bool c = (fcr31 & 0x800000u) == 0u; f1 = ((float)(f12)); if (c) goto L_10806e0; }
    f0 = (-(float)(f13));
    fcr31 = (a1f::le(f0, f2)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
    if ((fcr31 & 0x800000u) != 0u) { (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 1)))); goto L_10806f0; }
    (v0 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL));
    goto L_10806f0;
L_10806e0:;
    fcr31 = (a1f::le(f13, f2)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
    if ((fcr31 & 0x800000u) == 0u) { (v0 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL)); goto L_10806f0; }
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 1))));
L_10806f0:;
    if ((int64_t)v0 != 0) goto L_10807cc;
    at = 0x3FBD0000u;
    at = at | 0xE44Eu;
    { uint32_t bits = at; std::memcpy(&f0, &bits, sizeof(bits)); }
    fcr31 = (a1f::lt(f0, f14)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
    if ((fcr31 & 0x800000u) == 0u) goto L_1080718;
    f14 = ((float)(f0)); a3xw0 = true;
    goto L_1080730;
L_1080718:;
    at = 0xBFBD0000u;
    at = at | 0xE44Eu;
    { uint32_t bits = at; std::memcpy(&f0, &bits, sizeof(bits)); }
    fcr31 = (a1f::lt(f15, f0)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
    if ((fcr31 & 0x800000u) != 0u) { f15 = ((float)(f0)); a3xw1 = true; goto L_1080730; }
L_1080730:;
    fcr31 = (a1f::lt(f14, f12)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
    if ((fcr31 & 0x800000u) == 0u) goto L_1080770;
    { uint32_t bits = 0u; std::memcpy(&f0, &bits, sizeof(bits)); }
    fcr31 = (a1f::lt(f13, f0)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
    if ((fcr31 & 0x800000u) == 0u) goto L_10807cc;
    f1 = a1f::add(f12, f13);
    fcr31 = (a1f::lt(f1, f14)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
    { const bool c = (fcr31 & 0x800000u) == 0u; (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 1)))); if (c) goto L_10807cc; }
    if ((int64_t)a1 == 0) goto L_10807cc;
    f1 = ((float)(f14));
    goto L_10807cc;
L_1080770:;
    fcr31 = (a1f::lt(f12, f15)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
    if ((fcr31 & 0x800000u) == 0u) { f1 = a1f::add(f12, f13); goto L_10807b0; }
    { uint32_t bits = 0u; std::memcpy(&f0, &bits, sizeof(bits)); }
    fcr31 = (a1f::lt(f0, f13)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
    if ((fcr31 & 0x800000u) == 0u) goto L_10807cc;
    f1 = a1f::add(f12, f13);
    fcr31 = (a1f::lt(f15, f1)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
    { const bool c = (fcr31 & 0x800000u) == 0u; (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 1)))); if (c) goto L_10807cc; }
    if ((int64_t)a1 == 0) goto L_10807cc;
    f1 = ((float)(f15));
    goto L_10807cc;
L_10807b0:;
    fcr31 = (a1f::lt(f14, f1)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
    if ((fcr31 & 0x800000u) != 0u) { f1 = ((float)(f14)); goto L_10807c8; }
    fcr31 = (a1f::lt(f1, f15)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
    if ((fcr31 & 0x800000u) != 0u) { f1 = ((float)(f15)); goto L_10807c8; }
L_10807c8:;
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 1))));
L_10807cc:;
    if ((int64_t)a0 != 0) { A1_W32B((uint32_t)a0, 0, (uint32_t)v1); goto L_10807d4; }
L_10807d4:;
    { f0 = ((float)(f1)); if (xo) { if (a3xw0) xo[0] = a1_bits(f14); if (a3xw1) xo[1] = a1_bits(f15); } return a1::RetVF{(uint64_t)v0, f0}; }
    __builtin_unreachable();
}

// GAMEUTIL_get_difference_yaw_and_yaw (0x1080968): typed by tools/typed_lift2.py (round A3; leaf, extra outputs xo[0]=f13)
inline float GAMEUTIL_get_difference_yaw_and_yaw(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, float f12, float f13, uint64_t *xo = nullptr)
{
    float f0 = 0.0f;
    uint32_t at = 0;
    uint32_t fcr31 = 0;
    f13 = a1f::sub(f13, f12);
    f0 = a1_flt(0u);
    fcr31 = (a1f::lt(f13, f0)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
    if ((fcr31 & 0x800000u) != 0u) { f13 = (-(float)(f13)); goto L_108097c; }
L_108097c:;
    at = 0x40490000u;
    at = at | 0xFDAu;
    f0 = a1_flt(at);
    fcr31 = (a1f::lt(f0, f13)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
    if ((fcr31 & 0x800000u) == 0u) goto L_10809a4;
    at = 0x40C90000u;
    at = at | 0xFDAu;
    f0 = a1_flt(at);
    f13 = a1f::sub(f0, f13);
L_10809a4:;
    { f0 = ((float)(f13)); if (xo) { xo[0] = a1_bits(f13); } return f0; }
    __builtin_unreachable();
}

// GAMEUTIL_is_reach_aim_pitch_angle (0x1081090): typed by tools/typed_lift2.py (round A3; leaf, extra outputs xo[0]=f0)
inline uint32_t GAMEUTIL_is_reach_aim_pitch_angle(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint64_t a0, float f12, float f13, float f14, uint64_t *xo = nullptr)
{
    float f0 = 0.0f;
    float f1 = 0.0f;
    uint32_t at = 0;
    uint32_t v0 = 0;
    uint32_t v1 = 0;
    uint32_t fcr31 = 0;
    bool a3xw0 = false;
    fcr31 = (a1f::le(f12, f14)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
    if ((fcr31 & 0x800000u) == 0u) goto L_10810a8;
    fcr31 = (a1f::le(f14, f13)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
    { const bool c = (fcr31 & 0x800000u) != 0u; v1 = 1u; if (c) goto L_10810c8; }
L_10810a8:;
    fcr31 = (a1f::le(f13, f14)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
    { const bool c = (fcr31 & 0x800000u) == 0u; v1 = 0u; if (c) goto L_10810c8; }
    fcr31 = (a1f::le(f14, f12)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
    { const bool c = (fcr31 & 0x800000u) == 0u; v0 = 1u; if (c) goto L_10810cc; }
    v1 = 1u;
    goto L_10810cc;
L_10810c8:;
    v0 = 1u;
L_10810cc:;
    if (v1 == v0) goto L_1081134;
    f0 = a1f::sub(f14, f13); a3xw0 = true;
    { uint32_t bits = 0u; std::memcpy(&f1, &bits, sizeof(bits)); }
    fcr31 = (a1f::lt(f0, f1)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
    if ((fcr31 & 0x800000u) != 0u) { f0 = (-(float)(f0)); a3xw0 = true; goto L_10810e8; }
L_10810e8:;
    fcr31 = (a1f::lt(f0, f1)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
    at = 0x38D10000u;
    at = at | 0xB717u;
    { uint32_t bits = at; std::memcpy(&f1, &bits, sizeof(bits)); }
    if ((fcr31 & 0x800000u) == 0u) goto L_1081118;
    f0 = (-(float)(f0)); a3xw0 = true;
    fcr31 = (a1f::le(f0, f1)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
    if ((fcr31 & 0x800000u) != 0u) { v1 = 1u; goto L_1081128; }
    v1 = 0u;
    goto L_1081128;
L_1081118:;
    fcr31 = (a1f::le(f0, f1)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
    if ((fcr31 & 0x800000u) == 0u) { v1 = 0u; goto L_1081128; }
    v1 = 1u;
L_1081128:;
    v0 = 1u;
    if (v1 != v0) goto L_1081148;
L_1081134:;
    if ((int64_t)a0 != 0) { { float f = f14; uint32_t bits; std::memcpy(&bits, &f, sizeof(bits)); A1_W32B((uint32_t)a0, 0, bits); } goto L_108113c; }
L_108113c:;
    { v0 = 1u; if (xo) { if (a3xw0) xo[0] = a1_bits(f0); } return v0; }
L_1081148:;
    if ((int64_t)a0 != 0) { { float f = f13; uint32_t bits; std::memcpy(&bits, &f, sizeof(bits)); A1_W32B((uint32_t)a0, 0, bits); } goto L_1081150; }
L_1081150:;
    { v0 = 0u; if (xo) { xo[0] = a1_bits(f0); } return v0; }
    __builtin_unreachable();
}

// GAMEUTIL_reset_interp_ratio_cntrl (0x10842a0): typed by tools/typed_lift2.py (round A3; leaf, extra outputs xo[0]=f0)
inline uint32_t GAMEUTIL_reset_interp_ratio_cntrl(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t a0, uint64_t *xo = nullptr)
{
    float f0 = 0.0f;
    uint32_t at = 0;
    uint32_t v0 = 0;
    uint32_t v1 = 0;
    v1 = A1_R32(a0);
    v0 = 1u;
    A1_W32B(a0, 8, v0);
    { const bool c = v1 != v0; A1_W32B(a0, 12, 0u); if (c) goto L_10842c0; }
    { A1_W32B(a0, 16, 0u); return v0; }
L_10842c0:;
    at = 0x3F800000u;
    f0 = a1_flt(at);
    { A1_W32B(a0, 16, a1_bits(f0)); if (xo) { xo[0] = a1_bits(f0); } return v0; }
    __builtin_unreachable();
}

// reset_double_spring_interp_cntrl3D (0x10846a8): typed by tools/typed_lift2.py (round A3; loop leaf, extra outputs xo[0]=a2 xo[1]=a3 xo[2]=t0)
inline a1::RetVF reset_double_spring_interp_cntrl3D(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t a0, uint32_t a1, uint64_t *xo = nullptr)
{
    float f0 = 0.0f;
    uint32_t v0 = 0;
    uint32_t v1 = 0;
    uint64_t a2 = 0;
    uint32_t a3 = 0;
    uint32_t t0 = 0;
    a3 = a0 + 0x30u;
    t0 = a0 + 0x20u;
    (a2 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL));
L_10846b8:;
    (v0 = (uint32_t)((int32_t)((int32_t)SLL32((uint32_t)a2, 2))));
    (a2 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)a2, 1))));
    v1 = v0 + a1;
    a0 = v0 + t0;
    f0 = a1_flt(A1_R32(v1));
    (v1 = (uint32_t)(((int64_t)(int64_t)RC_U64X(a2) < (int64_t)(int32_t)3) ? 1 : 0));
    v0 = v0 + a3;
    A1_W32(v0, a1_bits(f0));
    { const bool c = v1 != 0; A1_W32(a0, a1_bits(f0)); if (c) { A3_BACK(); goto L_10846b8; } }
    { v0 = a3; if (xo) { xo[0] = a2; xo[1] = a1_sx64(a3); xo[2] = a1_sx64(t0); } return a1::RetVF{(uint64_t)a1_sx64(v0), f0}; }
    __builtin_unreachable();
}

// endBossDeathEffect (0x10871d8): typed by tools/typed_lift2.py (round A3; leaf, extra outputs xo[0]=a1)
inline uint32_t endBossDeathEffect(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint64_t *xo = nullptr)
{
    uint32_t v0 = 0;
    uint32_t v1 = 0;
    uint32_t a0 = 0;
    uint32_t a1 = 0;
    v1 = 0x1180000u;
    a1 = 0xFFFFFFFEu;
    v1 = v1 + 0x6290u;
    a0 = 0x1180000u;
    v0 = A1_R32(v1);
    A1_W32B(a0, 25240, 0u);
    v0 = v0 & a1;
    { A1_W32(v1, v0); if (xo) { xo[0] = a1_sx64(a1); } return v0; }
    __builtin_unreachable();
}

// fsEffectSetParam (0x1088a38): typed by tools/typed_lift2.py (round A3; leaf, extra outputs xo[0]=a1)
inline uint32_t fsEffectSetParam(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint64_t a0, uint64_t *xo = nullptr)
{
    uint32_t v0 = 0;
    uint32_t v1 = 0;
    uint32_t a1 = 0;
    bool a3xw0 = false;
    { const bool c = (int64_t)a0 == 0; v0 = 0x1180000u; if (c) goto L_1088a6c; }
    a1 = v0 + 0x62F0u; a3xw0 = true;
    v0 = A1_R32(a1);
    if (v0 == 0) { A1_W32B(a1, 0, (uint32_t)a0); goto L_1088a6c; }
    v1 = A1_R32B(v0, 4);
    (v0 = (uint32_t)((int32_t)((int32_t)A1_R32B((uint32_t)a0, 4))));
    v1 = v1 << 4;
    v0 = v0 << 4;
    v0 = (int32_t)v0 < (int32_t)v1 ? 1u : 0u;
    if (v0 == 0) { A1_W32B(a1, 0, (uint32_t)a0); goto L_1088a6c; }
L_1088a6c:;
    { if (xo) { if (a3xw0) xo[0] = a1_sx64(a1); } return v0; }
    __builtin_unreachable();
}

// fsEffectManagerSetDemoParam (0x1088d60): typed by tools/typed_lift2.py (round A3; leaf, extra outputs xo[0]=a0 xo[1]=a1 xo[2]=a2 xo[3]=a3)
inline uint32_t fsEffectManagerSetDemoParam(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t a0, uint64_t *xo = nullptr)
{
    uint32_t v0 = 0;
    uint32_t v1 = 0;
    uint32_t a1 = 0;
    uint32_t a2 = 0;
    uint32_t a3 = 0;
    bool a3xw2 = false;
    a1 = a0 + 4u;
    v0 = 0x30000u;
    v1 = A1_R32(a1);
    v0 = v0 | 0xFFFFu;
    v0 = v1 & v0;
    { const bool c = v0 == 0; v0 = a1 - v0; if (c) goto L_1088d88; }
    a1 = A1_R32(v0);
    goto L_1088d8c;
L_1088d88:;
    a1 = 0u;
L_1088d8c:;
    { const bool c = a1 == 0; { const uint32_t _dynInsn = A1_R32(0x1088D90u); const int32_t _dynSimm = (int16_t)(_dynInsn & 0xFFFFu); const uint32_t _dynImm = _dynInsn & 0xFFFFu; (void)_dynSimm; (void)_dynImm; (v0 = (uint32_t)((int32_t)((int32_t)((uint32_t)_dynImm << 16)))); } if (c) goto L_1088da8; }
    { const uint32_t _dynInsn = A1_R32(0x1088D94u); const int32_t _dynSimm = (int16_t)(_dynInsn & 0xFFFFu); const uint32_t _dynImm = _dynInsn & 0xFFFFu; (void)_dynSimm; (void)_dynImm; (v1 = (uint32_t)((int32_t)((int32_t)A1_R32(ADD32(v0, _dynSimm))))); }
    if (a1 == v1) { a1 = 0u; goto L_1088dac; }
    a1 = A1_R32B(a1, 16);
    goto L_1088dac;
L_1088da8:;
    a1 = 0u;
L_1088dac:;
    a0 = a0 + 8u;
    v0 = 0x30000u;
    v1 = A1_R32(a0);
    v0 = v0 | 0xFFFFu;
    v0 = v1 & v0;
    { const bool c = v0 == 0; v0 = a0 - v0; if (c) goto L_1088dd0; }
    a0 = A1_R32(v0);
    goto L_1088dd4;
L_1088dd0:;
    a0 = 0u;
L_1088dd4:;
    { const bool c = a0 == 0; { const uint32_t _dynInsn = A1_R32(0x1088DD8u); const int32_t _dynSimm = (int16_t)(_dynInsn & 0xFFFFu); const uint32_t _dynImm = _dynInsn & 0xFFFFu; (void)_dynSimm; (void)_dynImm; (v0 = (uint32_t)((int32_t)((int32_t)((uint32_t)_dynImm << 16)))); } if (c) goto L_1088df0; }
    { const uint32_t _dynInsn = A1_R32(0x1088DDCu); const int32_t _dynSimm = (int16_t)(_dynInsn & 0xFFFFu); const uint32_t _dynImm = _dynInsn & 0xFFFFu; (void)_dynSimm; (void)_dynImm; (v1 = (uint32_t)((int32_t)((int32_t)A1_R32(ADD32(v0, _dynSimm))))); }
    if (a0 == v1) { a0 = 0u; goto L_1088df4; }
    a0 = A1_R32B(a0, 16);
    goto L_1088df4;
L_1088df0:;
    a0 = 0u;
L_1088df4:;
    { const bool c = a1 == 0; a3 = 0x1180000u; if (c) goto L_1088e2c; }
    a2 = a3 + 0x62F0u; a3xw2 = true;
    v0 = A1_R32(a2);
    if (v0 == 0) { A1_W32(a2, a1); goto L_1088e2c; }
    v1 = A1_R32B(v0, 4);
    v0 = A1_R32B(a1, 4);
    v1 = v1 << 4;
    v0 = v0 << 4;
    v0 = (int32_t)v0 < (int32_t)v1 ? 1u : 0u;
    if (v0 != 0) { v0 = a3 + 0x62F0u; goto L_1088e30; }
    A1_W32(a2, a1);
L_1088e2c:;
    v0 = a3 + 0x62F0u;
L_1088e30:;
    { A1_W32B(v0, 4, a0); if (xo) { xo[0] = a1_sx64(a0); xo[1] = a1_sx64(a1); if (a3xw2) xo[2] = a1_sx64(a2); xo[3] = a1_sx64(a3); } return v0; }
    __builtin_unreachable();
}

// fsEffectManagerInit (0x1088eb8): typed by tools/typed_lift2.py (round A3; loop leaf)
inline uint64_t fsEffectManagerInit(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt)
{
    uint64_t v0 = 0;
    uint64_t v1 = 0;
    uint64_t a0 = 0;
    uint64_t a1 = 0;
    uint64_t a2 = 0;
    uint64_t a3 = 0;
    uint64_t t0 = 0;
    { const uint32_t _dynInsn = A1_R32(0x1088EB8u); const int32_t _dynSimm = (int16_t)(_dynInsn & 0xFFFFu); const uint32_t _dynImm = _dynInsn & 0xFFFFu; (void)_dynSimm; (void)_dynImm; (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)_dynImm << 16)))); }
    { const uint32_t _dynInsn = A1_R32(0x1088EBCu); const int32_t _dynSimm = (int16_t)(_dynInsn & 0xFFFFu); const uint32_t _dynImm = _dynInsn & 0xFFFFu; (void)_dynSimm; (void)_dynImm; (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32(ADD32((uint32_t)v0, _dynSimm))))); }
    if ((int64_t)v0 == 0) goto L_1088f74;
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)289 << 16))));
    (a0 = (uint64_t)((uint64_t)RC_U64X(v0) + (uint64_t)0ULL));
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v1, 18256))));
    (v0 = (uint64_t)(RC_U64X(a0) | RC_U64X(v1)));
    (v0 = (uint64_t)(RC_U64X(v0) & (uint64_t)(uint16_t)7));
    { const bool c = (int64_t)v0 == 0; (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)a0, 96)))); if (c) goto L_1088f40; }
L_1088ee4:;
    { uint32_t addr = ADD32((uint32_t)a0, 7); uint32_t aligned_addr = addr & ~7u; uint32_t offset = addr & 7u; uint64_t mem = A1_R64(aligned_addr); uint32_t shift = (7u - offset) << 3; uint64_t keepMask = (shift == 0) ? 0ull : ((1ull << shift) - 1ull); (a1 = (uint64_t)((a1 & keepMask) | (mem << shift))); }
    { uint32_t addr = ADD32((uint32_t)a0, 0); uint32_t aligned_addr = addr & ~7u; uint32_t offset = addr & 7u; uint64_t mem = A1_R64(aligned_addr); uint32_t shift = offset << 3; uint64_t keepMask = (offset == 0) ? 0ull : (0xFFFFFFFFFFFFFFFFull << ((8u - offset) << 3)); (a1 = (uint64_t)((a1 & keepMask) | (mem >> shift))); }
    { uint32_t addr = ADD32((uint32_t)a0, 15); uint32_t aligned_addr = addr & ~7u; uint32_t offset = addr & 7u; uint64_t mem = A1_R64(aligned_addr); uint32_t shift = (7u - offset) << 3; uint64_t keepMask = (shift == 0) ? 0ull : ((1ull << shift) - 1ull); (a2 = (uint64_t)((a2 & keepMask) | (mem << shift))); }
    { uint32_t addr = ADD32((uint32_t)a0, 8); uint32_t aligned_addr = addr & ~7u; uint32_t offset = addr & 7u; uint64_t mem = A1_R64(aligned_addr); uint32_t shift = offset << 3; uint64_t keepMask = (offset == 0) ? 0ull : (0xFFFFFFFFFFFFFFFFull << ((8u - offset) << 3)); (a2 = (uint64_t)((a2 & keepMask) | (mem >> shift))); }
    { uint32_t addr = ADD32((uint32_t)a0, 23); uint32_t aligned_addr = addr & ~7u; uint32_t offset = addr & 7u; uint64_t mem = A1_R64(aligned_addr); uint32_t shift = (7u - offset) << 3; uint64_t keepMask = (shift == 0) ? 0ull : ((1ull << shift) - 1ull); (a3 = (uint64_t)((a3 & keepMask) | (mem << shift))); }
    { uint32_t addr = ADD32((uint32_t)a0, 16); uint32_t aligned_addr = addr & ~7u; uint32_t offset = addr & 7u; uint64_t mem = A1_R64(aligned_addr); uint32_t shift = offset << 3; uint64_t keepMask = (offset == 0) ? 0ull : (0xFFFFFFFFFFFFFFFFull << ((8u - offset) << 3)); (a3 = (uint64_t)((a3 & keepMask) | (mem >> shift))); }
    { uint32_t addr = ADD32((uint32_t)a0, 31); uint32_t aligned_addr = addr & ~7u; uint32_t offset = addr & 7u; uint64_t mem = A1_R64(aligned_addr); uint32_t shift = (7u - offset) << 3; uint64_t keepMask = (shift == 0) ? 0ull : ((1ull << shift) - 1ull); (t0 = (uint64_t)((t0 & keepMask) | (mem << shift))); }
    { uint32_t addr = ADD32((uint32_t)a0, 24); uint32_t aligned_addr = addr & ~7u; uint32_t offset = addr & 7u; uint64_t mem = A1_R64(aligned_addr); uint32_t shift = offset << 3; uint64_t keepMask = (offset == 0) ? 0ull : (0xFFFFFFFFFFFFFFFFull << ((8u - offset) << 3)); (t0 = (uint64_t)((t0 & keepMask) | (mem >> shift))); }
    { uint32_t addr = ADD32((uint32_t)v1, 7); uint32_t aligned_addr = addr & ~7u; uint32_t offset = addr & 7u; uint32_t shift = (7u - offset) << 3; uint64_t mask = 0xFFFFFFFFFFFFFFFFull >> shift; uint64_t old_data = A1_R64(aligned_addr); uint64_t val = a1; uint64_t new_data = (old_data & ~mask) | ((val >> shift) & mask); A1_W64(aligned_addr, new_data); }
    { uint32_t addr = ADD32((uint32_t)v1, 0); uint32_t aligned_addr = addr & ~7u; uint32_t offset = addr & 7u; uint32_t shift = offset << 3; uint64_t mask = 0xFFFFFFFFFFFFFFFFull << shift; uint64_t old_data = A1_R64(aligned_addr); uint64_t val = a1; uint64_t new_data = (old_data & ~mask) | ((val << shift) & mask); A1_W64(aligned_addr, new_data); }
    { uint32_t addr = ADD32((uint32_t)v1, 15); uint32_t aligned_addr = addr & ~7u; uint32_t offset = addr & 7u; uint32_t shift = (7u - offset) << 3; uint64_t mask = 0xFFFFFFFFFFFFFFFFull >> shift; uint64_t old_data = A1_R64(aligned_addr); uint64_t val = a2; uint64_t new_data = (old_data & ~mask) | ((val >> shift) & mask); A1_W64(aligned_addr, new_data); }
    { uint32_t addr = ADD32((uint32_t)v1, 8); uint32_t aligned_addr = addr & ~7u; uint32_t offset = addr & 7u; uint32_t shift = offset << 3; uint64_t mask = 0xFFFFFFFFFFFFFFFFull << shift; uint64_t old_data = A1_R64(aligned_addr); uint64_t val = a2; uint64_t new_data = (old_data & ~mask) | ((val << shift) & mask); A1_W64(aligned_addr, new_data); }
    { uint32_t addr = ADD32((uint32_t)v1, 23); uint32_t aligned_addr = addr & ~7u; uint32_t offset = addr & 7u; uint32_t shift = (7u - offset) << 3; uint64_t mask = 0xFFFFFFFFFFFFFFFFull >> shift; uint64_t old_data = A1_R64(aligned_addr); uint64_t val = a3; uint64_t new_data = (old_data & ~mask) | ((val >> shift) & mask); A1_W64(aligned_addr, new_data); }
    { uint32_t addr = ADD32((uint32_t)v1, 16); uint32_t aligned_addr = addr & ~7u; uint32_t offset = addr & 7u; uint32_t shift = offset << 3; uint64_t mask = 0xFFFFFFFFFFFFFFFFull << shift; uint64_t old_data = A1_R64(aligned_addr); uint64_t val = a3; uint64_t new_data = (old_data & ~mask) | ((val << shift) & mask); A1_W64(aligned_addr, new_data); }
    { uint32_t addr = ADD32((uint32_t)v1, 31); uint32_t aligned_addr = addr & ~7u; uint32_t offset = addr & 7u; uint32_t shift = (7u - offset) << 3; uint64_t mask = 0xFFFFFFFFFFFFFFFFull >> shift; uint64_t old_data = A1_R64(aligned_addr); uint64_t val = t0; uint64_t new_data = (old_data & ~mask) | ((val >> shift) & mask); A1_W64(aligned_addr, new_data); }
    { uint32_t addr = ADD32((uint32_t)v1, 24); uint32_t aligned_addr = addr & ~7u; uint32_t offset = addr & 7u; uint32_t shift = offset << 3; uint64_t mask = 0xFFFFFFFFFFFFFFFFull << shift; uint64_t old_data = A1_R64(aligned_addr); uint64_t val = t0; uint64_t new_data = (old_data & ~mask) | ((val << shift) & mask); A1_W64(aligned_addr, new_data); }
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)a0, 32))));
    { const bool c = (int64_t)a0 != (int64_t)v0; (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v1, 32)))); if (c) { A3_BACK(); goto L_1088ee4; } }
    { return v0; }
L_1088f40:;
    (a1 = (uint64_t)(A1_R64B((uint32_t)a0, 0)));
    (a2 = (uint64_t)(A1_R64B((uint32_t)a0, 8)));
    (a3 = (uint64_t)(A1_R64B((uint32_t)a0, 16)));
    (t0 = (uint64_t)(A1_R64B((uint32_t)a0, 24)));
    A1_W64B((uint32_t)v1, 0, a1);
    A1_W64B((uint32_t)v1, 8, a2);
    A1_W64B((uint32_t)v1, 16, a3);
    A1_W64B((uint32_t)v1, 24, t0);
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)a0, 32))));
    { const bool c = (int64_t)a0 != (int64_t)v0; (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v1, 32)))); if (c) { A3_BACK(); goto L_1088f40; } }
L_1088f74:;
    { return v0; }
    __builtin_unreachable();
}

// initFsEffect (0x1088f80): typed by tools/typed_lift2.py (round A3; loop leaf)
inline uint64_t initFsEffect(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint64_t a0)
{
    uint64_t v0 = 0;
    uint64_t v1 = 0;
    uint64_t a1 = 0;
    uint64_t a2 = 0;
    uint64_t a3 = 0;
    uint64_t t0 = 0;
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)a0, 16))));
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)61440 << 16))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)a0, 4))));
    (v0 = (uint64_t)(RC_U64X(v0) & RC_U64X(v1)));
    { const bool c = (int64_t)v0 == 0; { const uint32_t _dynInsn = A1_R32(0x1088F94u); const int32_t _dynSimm = (int16_t)(_dynInsn & 0xFFFFu); const uint32_t _dynImm = _dynInsn & 0xFFFFu; (void)_dynSimm; (void)_dynImm; (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)_dynImm << 16)))); } if (c) goto L_1089050; }
    { const bool c = (int64_t)a0 == 0; { const uint32_t _dynInsn = A1_R32(0x1088F9Cu); const int32_t _dynSimm = (int16_t)(_dynInsn & 0xFFFFu); const uint32_t _dynImm = _dynInsn & 0xFFFFu; (void)_dynSimm; (void)_dynImm; (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32(ADD32((uint32_t)v0, _dynSimm))))); } if (c) goto L_1089054; }
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)289 << 16))));
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v1, 18256))));
    (v0 = (uint64_t)(RC_U64X(a0) | RC_U64X(v1)));
    (v0 = (uint64_t)(RC_U64X(v0) & (uint64_t)(uint16_t)7));
    { const bool c = (int64_t)v0 == 0; (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)a0, 96)))); if (c) goto L_1089018; }
L_1088fb8:;
    { uint32_t addr = ADD32((uint32_t)a0, 7); uint32_t aligned_addr = addr & ~7u; uint32_t offset = addr & 7u; uint64_t mem = A1_R64(aligned_addr); uint32_t shift = (7u - offset) << 3; uint64_t keepMask = (shift == 0) ? 0ull : ((1ull << shift) - 1ull); (a1 = (uint64_t)((a1 & keepMask) | (mem << shift))); }
    { uint32_t addr = ADD32((uint32_t)a0, 0); uint32_t aligned_addr = addr & ~7u; uint32_t offset = addr & 7u; uint64_t mem = A1_R64(aligned_addr); uint32_t shift = offset << 3; uint64_t keepMask = (offset == 0) ? 0ull : (0xFFFFFFFFFFFFFFFFull << ((8u - offset) << 3)); (a1 = (uint64_t)((a1 & keepMask) | (mem >> shift))); }
    { uint32_t addr = ADD32((uint32_t)a0, 15); uint32_t aligned_addr = addr & ~7u; uint32_t offset = addr & 7u; uint64_t mem = A1_R64(aligned_addr); uint32_t shift = (7u - offset) << 3; uint64_t keepMask = (shift == 0) ? 0ull : ((1ull << shift) - 1ull); (a2 = (uint64_t)((a2 & keepMask) | (mem << shift))); }
    { uint32_t addr = ADD32((uint32_t)a0, 8); uint32_t aligned_addr = addr & ~7u; uint32_t offset = addr & 7u; uint64_t mem = A1_R64(aligned_addr); uint32_t shift = offset << 3; uint64_t keepMask = (offset == 0) ? 0ull : (0xFFFFFFFFFFFFFFFFull << ((8u - offset) << 3)); (a2 = (uint64_t)((a2 & keepMask) | (mem >> shift))); }
    { uint32_t addr = ADD32((uint32_t)a0, 23); uint32_t aligned_addr = addr & ~7u; uint32_t offset = addr & 7u; uint64_t mem = A1_R64(aligned_addr); uint32_t shift = (7u - offset) << 3; uint64_t keepMask = (shift == 0) ? 0ull : ((1ull << shift) - 1ull); (a3 = (uint64_t)((a3 & keepMask) | (mem << shift))); }
    { uint32_t addr = ADD32((uint32_t)a0, 16); uint32_t aligned_addr = addr & ~7u; uint32_t offset = addr & 7u; uint64_t mem = A1_R64(aligned_addr); uint32_t shift = offset << 3; uint64_t keepMask = (offset == 0) ? 0ull : (0xFFFFFFFFFFFFFFFFull << ((8u - offset) << 3)); (a3 = (uint64_t)((a3 & keepMask) | (mem >> shift))); }
    { uint32_t addr = ADD32((uint32_t)a0, 31); uint32_t aligned_addr = addr & ~7u; uint32_t offset = addr & 7u; uint64_t mem = A1_R64(aligned_addr); uint32_t shift = (7u - offset) << 3; uint64_t keepMask = (shift == 0) ? 0ull : ((1ull << shift) - 1ull); (t0 = (uint64_t)((t0 & keepMask) | (mem << shift))); }
    { uint32_t addr = ADD32((uint32_t)a0, 24); uint32_t aligned_addr = addr & ~7u; uint32_t offset = addr & 7u; uint64_t mem = A1_R64(aligned_addr); uint32_t shift = offset << 3; uint64_t keepMask = (offset == 0) ? 0ull : (0xFFFFFFFFFFFFFFFFull << ((8u - offset) << 3)); (t0 = (uint64_t)((t0 & keepMask) | (mem >> shift))); }
    { uint32_t addr = ADD32((uint32_t)v1, 7); uint32_t aligned_addr = addr & ~7u; uint32_t offset = addr & 7u; uint32_t shift = (7u - offset) << 3; uint64_t mask = 0xFFFFFFFFFFFFFFFFull >> shift; uint64_t old_data = A1_R64(aligned_addr); uint64_t val = a1; uint64_t new_data = (old_data & ~mask) | ((val >> shift) & mask); A1_W64(aligned_addr, new_data); }
    { uint32_t addr = ADD32((uint32_t)v1, 0); uint32_t aligned_addr = addr & ~7u; uint32_t offset = addr & 7u; uint32_t shift = offset << 3; uint64_t mask = 0xFFFFFFFFFFFFFFFFull << shift; uint64_t old_data = A1_R64(aligned_addr); uint64_t val = a1; uint64_t new_data = (old_data & ~mask) | ((val << shift) & mask); A1_W64(aligned_addr, new_data); }
    { uint32_t addr = ADD32((uint32_t)v1, 15); uint32_t aligned_addr = addr & ~7u; uint32_t offset = addr & 7u; uint32_t shift = (7u - offset) << 3; uint64_t mask = 0xFFFFFFFFFFFFFFFFull >> shift; uint64_t old_data = A1_R64(aligned_addr); uint64_t val = a2; uint64_t new_data = (old_data & ~mask) | ((val >> shift) & mask); A1_W64(aligned_addr, new_data); }
    { uint32_t addr = ADD32((uint32_t)v1, 8); uint32_t aligned_addr = addr & ~7u; uint32_t offset = addr & 7u; uint32_t shift = offset << 3; uint64_t mask = 0xFFFFFFFFFFFFFFFFull << shift; uint64_t old_data = A1_R64(aligned_addr); uint64_t val = a2; uint64_t new_data = (old_data & ~mask) | ((val << shift) & mask); A1_W64(aligned_addr, new_data); }
    { uint32_t addr = ADD32((uint32_t)v1, 23); uint32_t aligned_addr = addr & ~7u; uint32_t offset = addr & 7u; uint32_t shift = (7u - offset) << 3; uint64_t mask = 0xFFFFFFFFFFFFFFFFull >> shift; uint64_t old_data = A1_R64(aligned_addr); uint64_t val = a3; uint64_t new_data = (old_data & ~mask) | ((val >> shift) & mask); A1_W64(aligned_addr, new_data); }
    { uint32_t addr = ADD32((uint32_t)v1, 16); uint32_t aligned_addr = addr & ~7u; uint32_t offset = addr & 7u; uint32_t shift = offset << 3; uint64_t mask = 0xFFFFFFFFFFFFFFFFull << shift; uint64_t old_data = A1_R64(aligned_addr); uint64_t val = a3; uint64_t new_data = (old_data & ~mask) | ((val << shift) & mask); A1_W64(aligned_addr, new_data); }
    { uint32_t addr = ADD32((uint32_t)v1, 31); uint32_t aligned_addr = addr & ~7u; uint32_t offset = addr & 7u; uint32_t shift = (7u - offset) << 3; uint64_t mask = 0xFFFFFFFFFFFFFFFFull >> shift; uint64_t old_data = A1_R64(aligned_addr); uint64_t val = t0; uint64_t new_data = (old_data & ~mask) | ((val >> shift) & mask); A1_W64(aligned_addr, new_data); }
    { uint32_t addr = ADD32((uint32_t)v1, 24); uint32_t aligned_addr = addr & ~7u; uint32_t offset = addr & 7u; uint32_t shift = offset << 3; uint64_t mask = 0xFFFFFFFFFFFFFFFFull << shift; uint64_t old_data = A1_R64(aligned_addr); uint64_t val = t0; uint64_t new_data = (old_data & ~mask) | ((val << shift) & mask); A1_W64(aligned_addr, new_data); }
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)a0, 32))));
    { const bool c = (int64_t)a0 != (int64_t)v0; (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v1, 32)))); if (c) { A3_BACK(); goto L_1088fb8; } }
    { const uint32_t _dynInsn = A1_R32(0x1089010u); const int32_t _dynSimm = (int16_t)(_dynInsn & 0xFFFFu); const uint32_t _dynImm = _dynInsn & 0xFFFFu; (void)_dynSimm; (void)_dynImm; (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)_dynImm << 16)))); }
    goto L_1089050;
L_1089018:;
    (a1 = (uint64_t)(A1_R64B((uint32_t)a0, 0)));
    (a2 = (uint64_t)(A1_R64B((uint32_t)a0, 8)));
    (a3 = (uint64_t)(A1_R64B((uint32_t)a0, 16)));
    (t0 = (uint64_t)(A1_R64B((uint32_t)a0, 24)));
    A1_W64B((uint32_t)v1, 0, a1);
    A1_W64B((uint32_t)v1, 8, a2);
    A1_W64B((uint32_t)v1, 16, a3);
    A1_W64B((uint32_t)v1, 24, t0);
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)a0, 32))));
    { const bool c = (int64_t)a0 != (int64_t)v0; (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v1, 32)))); if (c) { A3_BACK(); goto L_1089018; } }
    { const uint32_t _dynInsn = A1_R32(0x108904Cu); const int32_t _dynSimm = (int16_t)(_dynInsn & 0xFFFFu); const uint32_t _dynImm = _dynInsn & 0xFFFFu; (void)_dynSimm; (void)_dynImm; (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)_dynImm << 16)))); }
L_1089050:;
    { const uint32_t _dynInsn = A1_R32(0x1089050u); const int32_t _dynSimm = (int16_t)(_dynInsn & 0xFFFFu); const uint32_t _dynImm = _dynInsn & 0xFFFFu; (void)_dynSimm; (void)_dynImm; (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32(ADD32((uint32_t)v0, _dynSimm))))); }
L_1089054:;
    if ((int64_t)v0 == 0) goto L_108910c;
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)289 << 16))));
    (a0 = (uint64_t)((uint64_t)v0 + (uint64_t)0ULL));
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v1, 18256))));
    (v0 = (uint64_t)(a0 | RC_U64X(v1)));
    (v0 = (uint64_t)(v0 & (uint64_t)(uint16_t)7));
    { const bool c = (int64_t)v0 == 0; (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)a0, 96)))); if (c) goto L_10890d8; }
L_1089078:;
    { uint32_t addr = ADD32((uint32_t)a0, 7); uint32_t aligned_addr = addr & ~7u; uint32_t offset = addr & 7u; uint64_t mem = A1_R64(aligned_addr); uint32_t shift = (7u - offset) << 3; uint64_t keepMask = (shift == 0) ? 0ull : ((1ull << shift) - 1ull); (a1 = (uint64_t)((a1 & keepMask) | (mem << shift))); }
    { uint32_t addr = ADD32((uint32_t)a0, 0); uint32_t aligned_addr = addr & ~7u; uint32_t offset = addr & 7u; uint64_t mem = A1_R64(aligned_addr); uint32_t shift = offset << 3; uint64_t keepMask = (offset == 0) ? 0ull : (0xFFFFFFFFFFFFFFFFull << ((8u - offset) << 3)); (a1 = (uint64_t)((a1 & keepMask) | (mem >> shift))); }
    { uint32_t addr = ADD32((uint32_t)a0, 15); uint32_t aligned_addr = addr & ~7u; uint32_t offset = addr & 7u; uint64_t mem = A1_R64(aligned_addr); uint32_t shift = (7u - offset) << 3; uint64_t keepMask = (shift == 0) ? 0ull : ((1ull << shift) - 1ull); (a2 = (uint64_t)((a2 & keepMask) | (mem << shift))); }
    { uint32_t addr = ADD32((uint32_t)a0, 8); uint32_t aligned_addr = addr & ~7u; uint32_t offset = addr & 7u; uint64_t mem = A1_R64(aligned_addr); uint32_t shift = offset << 3; uint64_t keepMask = (offset == 0) ? 0ull : (0xFFFFFFFFFFFFFFFFull << ((8u - offset) << 3)); (a2 = (uint64_t)((a2 & keepMask) | (mem >> shift))); }
    { uint32_t addr = ADD32((uint32_t)a0, 23); uint32_t aligned_addr = addr & ~7u; uint32_t offset = addr & 7u; uint64_t mem = A1_R64(aligned_addr); uint32_t shift = (7u - offset) << 3; uint64_t keepMask = (shift == 0) ? 0ull : ((1ull << shift) - 1ull); (a3 = (uint64_t)((a3 & keepMask) | (mem << shift))); }
    { uint32_t addr = ADD32((uint32_t)a0, 16); uint32_t aligned_addr = addr & ~7u; uint32_t offset = addr & 7u; uint64_t mem = A1_R64(aligned_addr); uint32_t shift = offset << 3; uint64_t keepMask = (offset == 0) ? 0ull : (0xFFFFFFFFFFFFFFFFull << ((8u - offset) << 3)); (a3 = (uint64_t)((a3 & keepMask) | (mem >> shift))); }
    { uint32_t addr = ADD32((uint32_t)a0, 31); uint32_t aligned_addr = addr & ~7u; uint32_t offset = addr & 7u; uint64_t mem = A1_R64(aligned_addr); uint32_t shift = (7u - offset) << 3; uint64_t keepMask = (shift == 0) ? 0ull : ((1ull << shift) - 1ull); (t0 = (uint64_t)((t0 & keepMask) | (mem << shift))); }
    { uint32_t addr = ADD32((uint32_t)a0, 24); uint32_t aligned_addr = addr & ~7u; uint32_t offset = addr & 7u; uint64_t mem = A1_R64(aligned_addr); uint32_t shift = offset << 3; uint64_t keepMask = (offset == 0) ? 0ull : (0xFFFFFFFFFFFFFFFFull << ((8u - offset) << 3)); (t0 = (uint64_t)((t0 & keepMask) | (mem >> shift))); }
    { uint32_t addr = ADD32((uint32_t)v1, 7); uint32_t aligned_addr = addr & ~7u; uint32_t offset = addr & 7u; uint32_t shift = (7u - offset) << 3; uint64_t mask = 0xFFFFFFFFFFFFFFFFull >> shift; uint64_t old_data = A1_R64(aligned_addr); uint64_t val = a1; uint64_t new_data = (old_data & ~mask) | ((val >> shift) & mask); A1_W64(aligned_addr, new_data); }
    { uint32_t addr = ADD32((uint32_t)v1, 0); uint32_t aligned_addr = addr & ~7u; uint32_t offset = addr & 7u; uint32_t shift = offset << 3; uint64_t mask = 0xFFFFFFFFFFFFFFFFull << shift; uint64_t old_data = A1_R64(aligned_addr); uint64_t val = a1; uint64_t new_data = (old_data & ~mask) | ((val << shift) & mask); A1_W64(aligned_addr, new_data); }
    { uint32_t addr = ADD32((uint32_t)v1, 15); uint32_t aligned_addr = addr & ~7u; uint32_t offset = addr & 7u; uint32_t shift = (7u - offset) << 3; uint64_t mask = 0xFFFFFFFFFFFFFFFFull >> shift; uint64_t old_data = A1_R64(aligned_addr); uint64_t val = a2; uint64_t new_data = (old_data & ~mask) | ((val >> shift) & mask); A1_W64(aligned_addr, new_data); }
    { uint32_t addr = ADD32((uint32_t)v1, 8); uint32_t aligned_addr = addr & ~7u; uint32_t offset = addr & 7u; uint32_t shift = offset << 3; uint64_t mask = 0xFFFFFFFFFFFFFFFFull << shift; uint64_t old_data = A1_R64(aligned_addr); uint64_t val = a2; uint64_t new_data = (old_data & ~mask) | ((val << shift) & mask); A1_W64(aligned_addr, new_data); }
    { uint32_t addr = ADD32((uint32_t)v1, 23); uint32_t aligned_addr = addr & ~7u; uint32_t offset = addr & 7u; uint32_t shift = (7u - offset) << 3; uint64_t mask = 0xFFFFFFFFFFFFFFFFull >> shift; uint64_t old_data = A1_R64(aligned_addr); uint64_t val = a3; uint64_t new_data = (old_data & ~mask) | ((val >> shift) & mask); A1_W64(aligned_addr, new_data); }
    { uint32_t addr = ADD32((uint32_t)v1, 16); uint32_t aligned_addr = addr & ~7u; uint32_t offset = addr & 7u; uint32_t shift = offset << 3; uint64_t mask = 0xFFFFFFFFFFFFFFFFull << shift; uint64_t old_data = A1_R64(aligned_addr); uint64_t val = a3; uint64_t new_data = (old_data & ~mask) | ((val << shift) & mask); A1_W64(aligned_addr, new_data); }
    { uint32_t addr = ADD32((uint32_t)v1, 31); uint32_t aligned_addr = addr & ~7u; uint32_t offset = addr & 7u; uint32_t shift = (7u - offset) << 3; uint64_t mask = 0xFFFFFFFFFFFFFFFFull >> shift; uint64_t old_data = A1_R64(aligned_addr); uint64_t val = t0; uint64_t new_data = (old_data & ~mask) | ((val >> shift) & mask); A1_W64(aligned_addr, new_data); }
    { uint32_t addr = ADD32((uint32_t)v1, 24); uint32_t aligned_addr = addr & ~7u; uint32_t offset = addr & 7u; uint32_t shift = offset << 3; uint64_t mask = 0xFFFFFFFFFFFFFFFFull << shift; uint64_t old_data = A1_R64(aligned_addr); uint64_t val = t0; uint64_t new_data = (old_data & ~mask) | ((val << shift) & mask); A1_W64(aligned_addr, new_data); }
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)a0, 32))));
    { const bool c = (int64_t)a0 != (int64_t)v0; (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v1, 32)))); if (c) { A3_BACK(); goto L_1089078; } }
    { return v0; }
L_10890d8:;
    (a1 = (uint64_t)(A1_R64B((uint32_t)a0, 0)));
    (a2 = (uint64_t)(A1_R64B((uint32_t)a0, 8)));
    (a3 = (uint64_t)(A1_R64B((uint32_t)a0, 16)));
    (t0 = (uint64_t)(A1_R64B((uint32_t)a0, 24)));
    A1_W64B((uint32_t)v1, 0, a1);
    A1_W64B((uint32_t)v1, 8, a2);
    A1_W64B((uint32_t)v1, 16, a3);
    A1_W64B((uint32_t)v1, 24, t0);
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)a0, 32))));
    { const bool c = (int64_t)a0 != (int64_t)v0; (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v1, 32)))); if (c) { A3_BACK(); goto L_10890d8; } }
L_108910c:;
    { return v0; }
    __builtin_unreachable();
}

// setFogParamDefault (0x1089aa0): typed by tools/typed_lift2.py (round A3; leaf, extra outputs xo[0]=a1 xo[1]=a2 xo[2]=a3 xo[3]=t0)
inline uint64_t setFogParamDefault(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint64_t *xo = nullptr)
{
    uint64_t v0 = 0;
    uint64_t v1 = 0;
    uint64_t a0 = 0;
    uint64_t a1 = 0;
    uint64_t a2 = 0;
    uint64_t a3 = 0;
    uint64_t t0 = 0;
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)289 << 16))));
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)280 << 16))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v0, 18392))));
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)289 << 16))));
    (t0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)a0, 25336))));
    { uint32_t addr = ADD32((uint32_t)t0, 7); uint32_t aligned_addr = addr & ~7u; uint32_t offset = addr & 7u; uint64_t mem = A1_R64(aligned_addr); uint32_t shift = (7u - offset) << 3; uint64_t keepMask = (shift == 0) ? 0ull : ((1ull << shift) - 1ull); (a1 = (uint64_t)((a1 & keepMask) | (mem << shift))); }
    { uint32_t addr = ADD32((uint32_t)t0, 0); uint32_t aligned_addr = addr & ~7u; uint32_t offset = addr & 7u; uint64_t mem = A1_R64(aligned_addr); uint32_t shift = offset << 3; uint64_t keepMask = (offset == 0) ? 0ull : (0xFFFFFFFFFFFFFFFFull << ((8u - offset) << 3)); (a1 = (uint64_t)((a1 & keepMask) | (mem >> shift))); }
    { uint32_t addr = ADD32((uint32_t)t0, 15); uint32_t aligned_addr = addr & ~7u; uint32_t offset = addr & 7u; uint64_t mem = A1_R64(aligned_addr); uint32_t shift = (7u - offset) << 3; uint64_t keepMask = (shift == 0) ? 0ull : ((1ull << shift) - 1ull); (a2 = (uint64_t)((a2 & keepMask) | (mem << shift))); }
    { uint32_t addr = ADD32((uint32_t)t0, 8); uint32_t aligned_addr = addr & ~7u; uint32_t offset = addr & 7u; uint64_t mem = A1_R64(aligned_addr); uint32_t shift = offset << 3; uint64_t keepMask = (offset == 0) ? 0ull : (0xFFFFFFFFFFFFFFFFull << ((8u - offset) << 3)); (a2 = (uint64_t)((a2 & keepMask) | (mem >> shift))); }
    { uint32_t addr = ADD32((uint32_t)t0, 23); uint32_t aligned_addr = addr & ~7u; uint32_t offset = addr & 7u; uint64_t mem = A1_R64(aligned_addr); uint32_t shift = (7u - offset) << 3; uint64_t keepMask = (shift == 0) ? 0ull : ((1ull << shift) - 1ull); (a3 = (uint64_t)((a3 & keepMask) | (mem << shift))); }
    { uint32_t addr = ADD32((uint32_t)t0, 16); uint32_t aligned_addr = addr & ~7u; uint32_t offset = addr & 7u; uint64_t mem = A1_R64(aligned_addr); uint32_t shift = offset << 3; uint64_t keepMask = (offset == 0) ? 0ull : (0xFFFFFFFFFFFFFFFFull << ((8u - offset) << 3)); (a3 = (uint64_t)((a3 & keepMask) | (mem >> shift))); }
    { uint32_t addr = ADD32((uint32_t)v0, 7); uint32_t aligned_addr = addr & ~7u; uint32_t offset = addr & 7u; uint32_t shift = (7u - offset) << 3; uint64_t mask = 0xFFFFFFFFFFFFFFFFull >> shift; uint64_t old_data = A1_R64(aligned_addr); uint64_t val = a1; uint64_t new_data = (old_data & ~mask) | ((val >> shift) & mask); A1_W64(aligned_addr, new_data); }
    { uint32_t addr = ADD32((uint32_t)v0, 0); uint32_t aligned_addr = addr & ~7u; uint32_t offset = addr & 7u; uint32_t shift = offset << 3; uint64_t mask = 0xFFFFFFFFFFFFFFFFull << shift; uint64_t old_data = A1_R64(aligned_addr); uint64_t val = a1; uint64_t new_data = (old_data & ~mask) | ((val << shift) & mask); A1_W64(aligned_addr, new_data); }
    { uint32_t addr = ADD32((uint32_t)v0, 15); uint32_t aligned_addr = addr & ~7u; uint32_t offset = addr & 7u; uint32_t shift = (7u - offset) << 3; uint64_t mask = 0xFFFFFFFFFFFFFFFFull >> shift; uint64_t old_data = A1_R64(aligned_addr); uint64_t val = a2; uint64_t new_data = (old_data & ~mask) | ((val >> shift) & mask); A1_W64(aligned_addr, new_data); }
    { uint32_t addr = ADD32((uint32_t)v0, 8); uint32_t aligned_addr = addr & ~7u; uint32_t offset = addr & 7u; uint32_t shift = offset << 3; uint64_t mask = 0xFFFFFFFFFFFFFFFFull << shift; uint64_t old_data = A1_R64(aligned_addr); uint64_t val = a2; uint64_t new_data = (old_data & ~mask) | ((val << shift) & mask); A1_W64(aligned_addr, new_data); }
    { uint32_t addr = ADD32((uint32_t)v0, 23); uint32_t aligned_addr = addr & ~7u; uint32_t offset = addr & 7u; uint32_t shift = (7u - offset) << 3; uint64_t mask = 0xFFFFFFFFFFFFFFFFull >> shift; uint64_t old_data = A1_R64(aligned_addr); uint64_t val = a3; uint64_t new_data = (old_data & ~mask) | ((val >> shift) & mask); A1_W64(aligned_addr, new_data); }
    { uint32_t addr = ADD32((uint32_t)v0, 16); uint32_t aligned_addr = addr & ~7u; uint32_t offset = addr & 7u; uint32_t shift = offset << 3; uint64_t mask = 0xFFFFFFFFFFFFFFFFull << shift; uint64_t old_data = A1_R64(aligned_addr); uint64_t val = a3; uint64_t new_data = (old_data & ~mask) | ((val << shift) & mask); A1_W64(aligned_addr, new_data); }
    { uint32_t addr = ADD32((uint32_t)t0, 31); uint32_t aligned_addr = addr & ~7u; uint32_t offset = addr & 7u; uint64_t mem = A1_R64(aligned_addr); uint32_t shift = (7u - offset) << 3; uint64_t keepMask = (shift == 0) ? 0ull : ((1ull << shift) - 1ull); (a1 = (uint64_t)((a1 & keepMask) | (mem << shift))); }
    { uint32_t addr = ADD32((uint32_t)t0, 24); uint32_t aligned_addr = addr & ~7u; uint32_t offset = addr & 7u; uint64_t mem = A1_R64(aligned_addr); uint32_t shift = offset << 3; uint64_t keepMask = (offset == 0) ? 0ull : (0xFFFFFFFFFFFFFFFFull << ((8u - offset) << 3)); (a1 = (uint64_t)((a1 & keepMask) | (mem >> shift))); }
    { uint32_t addr = ADD32((uint32_t)t0, 39); uint32_t aligned_addr = addr & ~7u; uint32_t offset = addr & 7u; uint64_t mem = A1_R64(aligned_addr); uint32_t shift = (7u - offset) << 3; uint64_t keepMask = (shift == 0) ? 0ull : ((1ull << shift) - 1ull); (a2 = (uint64_t)((a2 & keepMask) | (mem << shift))); }
    { uint32_t addr = ADD32((uint32_t)t0, 32); uint32_t aligned_addr = addr & ~7u; uint32_t offset = addr & 7u; uint64_t mem = A1_R64(aligned_addr); uint32_t shift = offset << 3; uint64_t keepMask = (offset == 0) ? 0ull : (0xFFFFFFFFFFFFFFFFull << ((8u - offset) << 3)); (a2 = (uint64_t)((a2 & keepMask) | (mem >> shift))); }
    { uint32_t addr = ADD32((uint32_t)v0, 31); uint32_t aligned_addr = addr & ~7u; uint32_t offset = addr & 7u; uint32_t shift = (7u - offset) << 3; uint64_t mask = 0xFFFFFFFFFFFFFFFFull >> shift; uint64_t old_data = A1_R64(aligned_addr); uint64_t val = a1; uint64_t new_data = (old_data & ~mask) | ((val >> shift) & mask); A1_W64(aligned_addr, new_data); }
    { uint32_t addr = ADD32((uint32_t)v0, 24); uint32_t aligned_addr = addr & ~7u; uint32_t offset = addr & 7u; uint32_t shift = offset << 3; uint64_t mask = 0xFFFFFFFFFFFFFFFFull << shift; uint64_t old_data = A1_R64(aligned_addr); uint64_t val = a1; uint64_t new_data = (old_data & ~mask) | ((val << shift) & mask); A1_W64(aligned_addr, new_data); }
    { uint32_t addr = ADD32((uint32_t)v0, 39); uint32_t aligned_addr = addr & ~7u; uint32_t offset = addr & 7u; uint32_t shift = (7u - offset) << 3; uint64_t mask = 0xFFFFFFFFFFFFFFFFull >> shift; uint64_t old_data = A1_R64(aligned_addr); uint64_t val = a2; uint64_t new_data = (old_data & ~mask) | ((val >> shift) & mask); A1_W64(aligned_addr, new_data); }
    { uint32_t addr = ADD32((uint32_t)v0, 32); uint32_t aligned_addr = addr & ~7u; uint32_t offset = addr & 7u; uint32_t shift = offset << 3; uint64_t mask = 0xFFFFFFFFFFFFFFFFull << shift; uint64_t old_data = A1_R64(aligned_addr); uint64_t val = a2; uint64_t new_data = (old_data & ~mask) | ((val << shift) & mask); A1_W64(aligned_addr, new_data); }
    (a3 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v1, 18352))));
    { uint32_t addr = ADD32((uint32_t)v0, 7); uint32_t aligned_addr = addr & ~7u; uint32_t offset = addr & 7u; uint64_t mem = A1_R64(aligned_addr); uint32_t shift = (7u - offset) << 3; uint64_t keepMask = (shift == 0) ? 0ull : ((1ull << shift) - 1ull); (a0 = (uint64_t)((RC_U64X(a0) & keepMask) | (mem << shift))); }
    { uint32_t addr = ADD32((uint32_t)v0, 0); uint32_t aligned_addr = addr & ~7u; uint32_t offset = addr & 7u; uint64_t mem = A1_R64(aligned_addr); uint32_t shift = offset << 3; uint64_t keepMask = (offset == 0) ? 0ull : (0xFFFFFFFFFFFFFFFFull << ((8u - offset) << 3)); (a0 = (uint64_t)((a0 & keepMask) | (mem >> shift))); }
    { uint32_t addr = ADD32((uint32_t)v0, 15); uint32_t aligned_addr = addr & ~7u; uint32_t offset = addr & 7u; uint64_t mem = A1_R64(aligned_addr); uint32_t shift = (7u - offset) << 3; uint64_t keepMask = (shift == 0) ? 0ull : ((1ull << shift) - 1ull); (a1 = (uint64_t)((a1 & keepMask) | (mem << shift))); }
    { uint32_t addr = ADD32((uint32_t)v0, 8); uint32_t aligned_addr = addr & ~7u; uint32_t offset = addr & 7u; uint64_t mem = A1_R64(aligned_addr); uint32_t shift = offset << 3; uint64_t keepMask = (offset == 0) ? 0ull : (0xFFFFFFFFFFFFFFFFull << ((8u - offset) << 3)); (a1 = (uint64_t)((a1 & keepMask) | (mem >> shift))); }
    { uint32_t addr = ADD32((uint32_t)v0, 23); uint32_t aligned_addr = addr & ~7u; uint32_t offset = addr & 7u; uint64_t mem = A1_R64(aligned_addr); uint32_t shift = (7u - offset) << 3; uint64_t keepMask = (shift == 0) ? 0ull : ((1ull << shift) - 1ull); (a2 = (uint64_t)((a2 & keepMask) | (mem << shift))); }
    { uint32_t addr = ADD32((uint32_t)v0, 16); uint32_t aligned_addr = addr & ~7u; uint32_t offset = addr & 7u; uint64_t mem = A1_R64(aligned_addr); uint32_t shift = offset << 3; uint64_t keepMask = (offset == 0) ? 0ull : (0xFFFFFFFFFFFFFFFFull << ((8u - offset) << 3)); (a2 = (uint64_t)((a2 & keepMask) | (mem >> shift))); }
    { uint32_t addr = ADD32((uint32_t)a3, 7); uint32_t aligned_addr = addr & ~7u; uint32_t offset = addr & 7u; uint32_t shift = (7u - offset) << 3; uint64_t mask = 0xFFFFFFFFFFFFFFFFull >> shift; uint64_t old_data = A1_R64(aligned_addr); uint64_t val = a0; uint64_t new_data = (old_data & ~mask) | ((val >> shift) & mask); A1_W64(aligned_addr, new_data); }
    { uint32_t addr = ADD32((uint32_t)a3, 0); uint32_t aligned_addr = addr & ~7u; uint32_t offset = addr & 7u; uint32_t shift = offset << 3; uint64_t mask = 0xFFFFFFFFFFFFFFFFull << shift; uint64_t old_data = A1_R64(aligned_addr); uint64_t val = a0; uint64_t new_data = (old_data & ~mask) | ((val << shift) & mask); A1_W64(aligned_addr, new_data); }
    { uint32_t addr = ADD32((uint32_t)a3, 15); uint32_t aligned_addr = addr & ~7u; uint32_t offset = addr & 7u; uint32_t shift = (7u - offset) << 3; uint64_t mask = 0xFFFFFFFFFFFFFFFFull >> shift; uint64_t old_data = A1_R64(aligned_addr); uint64_t val = a1; uint64_t new_data = (old_data & ~mask) | ((val >> shift) & mask); A1_W64(aligned_addr, new_data); }
    { uint32_t addr = ADD32((uint32_t)a3, 8); uint32_t aligned_addr = addr & ~7u; uint32_t offset = addr & 7u; uint32_t shift = offset << 3; uint64_t mask = 0xFFFFFFFFFFFFFFFFull << shift; uint64_t old_data = A1_R64(aligned_addr); uint64_t val = a1; uint64_t new_data = (old_data & ~mask) | ((val << shift) & mask); A1_W64(aligned_addr, new_data); }
    { uint32_t addr = ADD32((uint32_t)a3, 23); uint32_t aligned_addr = addr & ~7u; uint32_t offset = addr & 7u; uint32_t shift = (7u - offset) << 3; uint64_t mask = 0xFFFFFFFFFFFFFFFFull >> shift; uint64_t old_data = A1_R64(aligned_addr); uint64_t val = a2; uint64_t new_data = (old_data & ~mask) | ((val >> shift) & mask); A1_W64(aligned_addr, new_data); }
    { uint32_t addr = ADD32((uint32_t)a3, 16); uint32_t aligned_addr = addr & ~7u; uint32_t offset = addr & 7u; uint32_t shift = offset << 3; uint64_t mask = 0xFFFFFFFFFFFFFFFFull << shift; uint64_t old_data = A1_R64(aligned_addr); uint64_t val = a2; uint64_t new_data = (old_data & ~mask) | ((val << shift) & mask); A1_W64(aligned_addr, new_data); }
    { uint32_t addr = ADD32((uint32_t)v0, 31); uint32_t aligned_addr = addr & ~7u; uint32_t offset = addr & 7u; uint64_t mem = A1_R64(aligned_addr); uint32_t shift = (7u - offset) << 3; uint64_t keepMask = (shift == 0) ? 0ull : ((1ull << shift) - 1ull); (a0 = (uint64_t)((a0 & keepMask) | (mem << shift))); }
    { uint32_t addr = ADD32((uint32_t)v0, 24); uint32_t aligned_addr = addr & ~7u; uint32_t offset = addr & 7u; uint64_t mem = A1_R64(aligned_addr); uint32_t shift = offset << 3; uint64_t keepMask = (offset == 0) ? 0ull : (0xFFFFFFFFFFFFFFFFull << ((8u - offset) << 3)); (a0 = (uint64_t)((a0 & keepMask) | (mem >> shift))); }
    { uint32_t addr = ADD32((uint32_t)v0, 39); uint32_t aligned_addr = addr & ~7u; uint32_t offset = addr & 7u; uint64_t mem = A1_R64(aligned_addr); uint32_t shift = (7u - offset) << 3; uint64_t keepMask = (shift == 0) ? 0ull : ((1ull << shift) - 1ull); (a1 = (uint64_t)((a1 & keepMask) | (mem << shift))); }
    { uint32_t addr = ADD32((uint32_t)v0, 32); uint32_t aligned_addr = addr & ~7u; uint32_t offset = addr & 7u; uint64_t mem = A1_R64(aligned_addr); uint32_t shift = offset << 3; uint64_t keepMask = (offset == 0) ? 0ull : (0xFFFFFFFFFFFFFFFFull << ((8u - offset) << 3)); (a1 = (uint64_t)((a1 & keepMask) | (mem >> shift))); }
    { uint32_t addr = ADD32((uint32_t)a3, 31); uint32_t aligned_addr = addr & ~7u; uint32_t offset = addr & 7u; uint32_t shift = (7u - offset) << 3; uint64_t mask = 0xFFFFFFFFFFFFFFFFull >> shift; uint64_t old_data = A1_R64(aligned_addr); uint64_t val = a0; uint64_t new_data = (old_data & ~mask) | ((val >> shift) & mask); A1_W64(aligned_addr, new_data); }
    { uint32_t addr = ADD32((uint32_t)a3, 24); uint32_t aligned_addr = addr & ~7u; uint32_t offset = addr & 7u; uint32_t shift = offset << 3; uint64_t mask = 0xFFFFFFFFFFFFFFFFull << shift; uint64_t old_data = A1_R64(aligned_addr); uint64_t val = a0; uint64_t new_data = (old_data & ~mask) | ((val << shift) & mask); A1_W64(aligned_addr, new_data); }
    { uint32_t addr = ADD32((uint32_t)a3, 39); uint32_t aligned_addr = addr & ~7u; uint32_t offset = addr & 7u; uint32_t shift = (7u - offset) << 3; uint64_t mask = 0xFFFFFFFFFFFFFFFFull >> shift; uint64_t old_data = A1_R64(aligned_addr); uint64_t val = a1; uint64_t new_data = (old_data & ~mask) | ((val >> shift) & mask); A1_W64(aligned_addr, new_data); }
    { uint32_t addr = ADD32((uint32_t)a3, 32); uint32_t aligned_addr = addr & ~7u; uint32_t offset = addr & 7u; uint32_t shift = offset << 3; uint64_t mask = 0xFFFFFFFFFFFFFFFFull << shift; uint64_t old_data = A1_R64(aligned_addr); uint64_t val = a1; uint64_t new_data = (old_data & ~mask) | ((val << shift) & mask); A1_W64(aligned_addr, new_data); }
    { if (xo) { xo[0] = a1; xo[1] = a2; xo[2] = a3; xo[3] = t0; } return v0; }
    __builtin_unreachable();
}

// glBlendFunc (0x108d150): typed by tools/typed_lift2.py (round A3; leaf, extra outputs xo[0]=a0 xo[1]=a1 xo[2]=a2 xo[3]=a3 xo[4]=t0)
inline uint64_t glBlendFunc(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint64_t a0, uint64_t a1, uint64_t *xo = nullptr)
{
    uint64_t v0 = 0;
    uint64_t v1 = 0;
    uint64_t a2 = 0;
    uint64_t a3 = 0;
    uint64_t t0 = 0;
    bool a3xw0 = false;
    bool a3xw1 = false;
    bool a3xw2 = false;
    bool a3xw3 = false;
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 5))));
    (t0 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL));
    { const bool c = (int64_t)a0 != (int64_t)v0; (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 1)))); if (c) goto L_108d190; }
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 6))));
    { const bool c = (int64_t)a1 != (int64_t)v0; (a3 = (uint64_t)((uint64_t)RC_U64X(t0) + (uint64_t)0ULL)); a3xw3 = true; if (c) goto L_108d178; }
    (a1 = (uint64_t)((uint64_t)RC_U64X(v1) + (uint64_t)0ULL)); a3xw1 = true;
    goto L_108d1f0;
L_108d178:;
    { const bool c = (int64_t)a1 != (int64_t)v1; (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 6)))); if (c) goto L_108d194; }
    (a3 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL)); a3xw3 = true;
    (a1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 2)))); a3xw1 = true;
    goto L_108d1f0;
L_108d190:;
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 6))));
L_108d194:;
    { const bool c = (int64_t)a0 != (int64_t)v0; (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 7)))); if (c) goto L_108d1b8; }
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 1))));
    { const bool c = (int64_t)a1 != (int64_t)v0; (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 7)))); if (c) goto L_108d1b8; }
    (a3 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 2)))); a3xw3 = true;
    (a1 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL)); a3xw1 = true;
    (t0 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL));
    goto L_108d1f0;
L_108d1b8:;
    { const bool c = (int64_t)a0 != (int64_t)v0; (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 8)))); if (c) goto L_108d1d8; }
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 1))));
    { const bool c = (int64_t)a1 != (int64_t)v0; (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 8)))); if (c) goto L_108d1d8; }
    (a3 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL)); a3xw3 = true;
    (a1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 2)))); a3xw1 = true;
    goto L_108d1ec;
L_108d1d8:;
    { const bool c = (int64_t)a0 != (int64_t)v0; (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 1)))); if (c) goto L_108d284; }
    { const bool c = (int64_t)a1 != (int64_t)v0; (a3 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL)); a3xw3 = true; if (c) goto L_108d284; }
    (a1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 1)))); a3xw1 = true;
L_108d1ec:;
    (t0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 1))));
L_108d1f0:;
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)280 << 16))));
    (a2 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v0, 25488)))); a3xw2 = true;
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)a2, 48))));
    { const bool c = (int64_t)v1 == 0; (v0 = (uint64_t)(RC_U64X(a3) << (32 + 0))); if (c) goto L_108d238; }
    (v1 = (uint64_t)(RC_U64X(a1) << 2));
    (a0 = (uint64_t)(t0 << 4)); a3xw0 = true;
    (v0 = (uint64_t)(v0 >> (32 + 0)));
    (a1 = (uint64_t)(0ULL | (uint64_t)(uint16_t)32768)); a3xw1 = true;
    (a1 = (uint64_t)(RC_U64X(a1) << 24)); a3xw1 = true;
    (a0 = (uint64_t)(a0 | v1)); a3xw0 = true;
    (a2 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)a2, 4)))); a3xw2 = true;
    (v0 = (uint64_t)(v0 | a1));
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)a2, 0))));
    (v0 = (uint64_t)(v0 | a0));
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 64)))); a3xw0 = true;
    (a1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 67)))); a3xw1 = true;
    goto L_108d268;
L_108d238:;
    (v1 = (uint64_t)(RC_U64X(a1) << 2));
    (a0 = (uint64_t)(t0 << 4)); a3xw0 = true;
    (v0 = (uint64_t)(v0 >> (32 + 0)));
    (a1 = (uint64_t)(0ULL | (uint64_t)(uint16_t)32768)); a3xw1 = true;
    (a1 = (uint64_t)(RC_U64X(a1) << 24)); a3xw1 = true;
    (a0 = (uint64_t)(a0 | v1)); a3xw0 = true;
    (a2 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)a2, 4)))); a3xw2 = true;
    (v0 = (uint64_t)(v0 | a1));
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)a2, 0))));
    (v0 = (uint64_t)(v0 | a0));
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 64)))); a3xw0 = true;
    (a1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 66)))); a3xw1 = true;
L_108d268:;
    (v0 = (uint64_t)(v0 | RC_U64X(a0)));
    A1_W64B((uint32_t)v1, 0, v0);
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v1, 8))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v1, 8))));
    A1_W32B((uint32_t)a2, 0, (uint32_t)v1);
    A1_W64B((uint32_t)v1, 0, RC_U64X(a1));
    A1_W32B((uint32_t)a2, 0, (uint32_t)v0);
L_108d284:;
    { if (xo) { if (a3xw0) xo[0] = a0; if (a3xw1) xo[1] = a1; if (a3xw2) xo[2] = a2; if (a3xw3) xo[3] = a3; xo[4] = t0; } return v0; }
    __builtin_unreachable();
}

// glBlendFuncParticle (0x108d290): typed by tools/typed_lift2.py (round A3; leaf, extra outputs xo[0]=a0 xo[1]=a1 xo[2]=a2 xo[3]=a3)
inline uint64_t glBlendFuncParticle(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint64_t a0, uint64_t *xo = nullptr)
{
    uint64_t v0 = 0;
    uint64_t v1 = 0;
    uint64_t a1 = 0;
    uint64_t a2 = 0;
    uint64_t a3 = 0;
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 1))));
    (a3 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL));
    { const bool c = (int64_t)a0 == (int64_t)v0; (a2 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 1)))); if (c) goto L_108d2d0; }
    (v0 = (uint64_t)(((int64_t)(int64_t)a0 < (int64_t)(int32_t)2) ? 1 : 0));
    { const bool c = (int64_t)v0 == 0; (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 2)))); if (c) goto L_108d2c0; }
    { const bool c = (int64_t)a0 == 0; (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)280 << 16)))); if (c) goto L_108d2e0; }
    (a1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v0, 25488))));
    goto L_108d2e8;
L_108d2c0:;
    { const bool c = (int64_t)a0 == (int64_t)v0; (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)280 << 16)))); if (c) goto L_108d2d8; }
    (a1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v0, 25488))));
    goto L_108d2e8;
L_108d2d0:;
    (a2 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 2))));
    goto L_108d2e0;
L_108d2d8:;
    (a3 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 2))));
    (a2 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL));
L_108d2e0:;
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)280 << 16))));
    (a1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v0, 25488))));
L_108d2e8:;
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)a1, 48))));
    { const bool c = (int64_t)v1 == 0; (v0 = (uint64_t)(a3 << (32 + 0))); if (c) goto L_108d310; }
    (a0 = (uint64_t)(0ULL | (uint64_t)(uint16_t)32768));
    (a0 = (uint64_t)(RC_U64X(a0) << 24));
    (v0 = (uint64_t)(v0 >> (32 + 0)));
    (a1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)a1, 4))));
    (v1 = (uint64_t)(a2 << 2));
    (a2 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 67))));
    goto L_108d328;
L_108d310:;
    (a0 = (uint64_t)(0ULL | (uint64_t)(uint16_t)32768));
    (a0 = (uint64_t)(RC_U64X(a0) << 24));
    (v0 = (uint64_t)(v0 >> (32 + 0)));
    (a1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)a1, 4))));
    (v1 = (uint64_t)(a2 << 2));
    (a2 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 66))));
L_108d328:;
    (v0 = (uint64_t)(v0 | a0));
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)a1, 0))));
    (v0 = (uint64_t)(v0 | v1));
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 64))));
    (v0 = (uint64_t)(v0 | RC_U64X(v1)));
    A1_W64B((uint32_t)a0, 0, v0);
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)a0, 8))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)a0, 8))));
    A1_W32B((uint32_t)a1, 0, (uint32_t)a0);
    A1_W64B((uint32_t)a0, 0, RC_U64X(a2));
    { A1_W32B((uint32_t)a1, 0, (uint32_t)v0); if (xo) { xo[0] = a0; xo[1] = a1; xo[2] = a2; xo[3] = a3; } return v0; }
    __builtin_unreachable();
}

// glColor4f (0x108e8b8): typed by tools/typed_lift2.py (round A3; loop leaf, extra outputs xo[0]=f12 xo[1]=f13 xo[2]=f14 xo[3]=f15 xo[4]=a0)
inline a1::RetVF glColor4f(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, float f12, float f13, float f14, float f15, uint64_t *xo = nullptr)
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
    f0 = a1_flt(at);
    v0 = 0x1180000u;
    at = 0x43000000u;
    f1 = a1_flt(at);
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
    { if (xo) { xo[0] = a1_bits(f12); xo[1] = a1_bits(f13); xo[2] = a1_bits(f14); xo[3] = a1_bits(f15); xo[4] = a1_sx64(a0); } return a1::RetVF{(uint64_t)a1_sx64(v0), f0}; }
    __builtin_unreachable();
}

// glColor4fv (0x108e938): typed by tools/typed_lift2.py (round A3; loop leaf, extra outputs xo[0]=f2 xo[1]=f3 xo[2]=f4 xo[3]=f5 xo[4]=a1)
inline a1::RetVF glColor4fv(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t a0, uint64_t *xo = nullptr)
{
    float f0 = 0.0f;
    float f1 = 0.0f;
    float f2 = 0.0f;
    float f3 = 0.0f;
    float f4 = 0.0f;
    float f5 = 0.0f;
    uint32_t at = 0;
    uint32_t v0 = 0;
    uint32_t v1 = 0;
    uint32_t a1 = 0;
    uint32_t fcr31 = 0;
    f1 = a1_flt(A1_R32(a0));
    v0 = 0x1180000u;
    at = 0x437F0000u;
    f2 = a1_flt(at);
    v0 = v0 + 0x6390u;
    at = 0x43000000u;
    f3 = a1_flt(at);
    v1 = v0 + 0x90u;
    f1 = a1f::mul(f1, f2);
    f4 = ((float)(f2));
    f5 = a1_flt(0u);
    a1 = 3u;
    A1_W32B(v0, 144, a1_bits(f1));
    f0 = a1_flt(A1_R32B(a0, 4));
    f0 = a1f::mul(f0, f2);
    A1_W32B(v0, 148, a1_bits(f0));
    f1 = a1_flt(A1_R32B(a0, 8));
    f1 = a1f::mul(f1, f2);
    A1_W32B(v0, 152, a1_bits(f1));
    f0 = a1_flt(A1_R32B(a0, 12));
    f0 = a1f::mul(f0, f3);
    A1_W32B(v0, 156, a1_bits(f0));
L_108e990:;
    f0 = a1_flt(A1_R32(v1));
    fcr31 = (a1f::lt(f4, f0)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
    if ((fcr31 & 0x800000u) == 0u) goto L_108e9a8;
    A1_W32(v1, a1_bits(f4));
    f0 = ((float)(f4));
L_108e9a8:;
    fcr31 = (a1f::lt(f0, f5)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
    if ((fcr31 & 0x800000u) != 0u) { A1_W32(v1, 0u); goto L_108e9b4; }
L_108e9b4:;
    a1 = a1 - 1u;
    { const bool c = (int32_t)a1 >= 0; v1 = v1 + 4u; if (c) { A3_BACK(); goto L_108e990; } }
    { if (xo) { xo[0] = a1_bits(f2); xo[1] = a1_bits(f3); xo[2] = a1_bits(f4); xo[3] = a1_bits(f5); xo[4] = a1_sx64(a1); } return a1::RetVF{(uint64_t)a1_sx64(v0), f0}; }
    __builtin_unreachable();
}

// modelGetDlLayer (0x1096a80): typed by tools/typed_lift2.py (round A3; leaf, extra outputs xo[0]=a1)
inline uint64_t modelGetDlLayer(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint64_t a0, uint64_t a1, uint64_t a2, uint64_t *xo = nullptr)
{
    uint64_t v0 = 0;
    uint64_t v1 = 0;
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)a0, 8))));
    { const bool c = (int64_t)v0 == 0; (a0 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL)); if (c) goto L_1096a90; }
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)v0, 56))));
L_1096a90:;
    (a1 = (uint64_t)(A1_R64B((uint32_t)a1, 24)));
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)258 << 16))));
    (v1 = (uint64_t)(a1 & RC_U64X(v1)));
    { const bool c = (int64_t)v1 != 0; (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 8)))); if (c) goto L_1096b7c; }
    (v1 = (uint64_t)(a1 << 9));
    (v1 = (uint64_t)((int64_t)v1 >> (32 + 0)));
    (v1 = (uint64_t)(v1 & (uint64_t)(uint16_t)1));
    { const bool c = (int64_t)v1 != 0; (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 10)))); if (c) goto L_1096b7c; }
    (v1 = (uint64_t)(a1 << 7));
    (v1 = (uint64_t)((int64_t)v1 >> (32 + 0)));
    (v1 = (uint64_t)(v1 & (uint64_t)(uint16_t)1));
    { const bool c = (int64_t)v1 != 0; (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 16)))); if (c) goto L_1096b7c; }
    (v1 = (uint64_t)(a1 << 23));
    (v1 = (uint64_t)((int64_t)v1 >> (32 + 0)));
    (v1 = (uint64_t)(v1 & (uint64_t)(uint16_t)1));
    { const bool c = (int64_t)v1 != 0; (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 17)))); if (c) goto L_1096b7c; }
    (v0 = (uint64_t)(a1 << 22));
    (v0 = (uint64_t)((int64_t)v0 >> (32 + 0)));
    (v0 = (uint64_t)(v0 & (uint64_t)(uint16_t)1));
    if ((int64_t)v0 == 0) goto L_1096b18;
    (v0 = (uint64_t)(a1 << 24));
    (v0 = (uint64_t)((int64_t)v0 >> (32 + 0)));
    (v0 = (uint64_t)(v0 & (uint64_t)(uint16_t)1));
    if ((int64_t)v0 != 0) goto L_1096b10;
    { const bool c = (int64_t)a2 == 0; (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 12)))); if (c) goto L_1096b7c; }
L_1096b10:;
    { (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 13)))); if (xo) { xo[0] = a1; } return v0; }
L_1096b18:;
    (v1 = (uint64_t)(a1 << 18));
    (v1 = (uint64_t)((int64_t)v1 >> (32 + 0)));
    (v1 = (uint64_t)(v1 & (uint64_t)(uint16_t)1));
    { const bool c = (int64_t)v1 != 0; (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 1)))); if (c) goto L_1096b7c; }
    (v1 = (uint64_t)(a1 << 17));
    (v1 = (uint64_t)((int64_t)v1 >> (32 + 0)));
    (v1 = (uint64_t)(v1 & (uint64_t)(uint16_t)1));
    { const bool c = (int64_t)v1 != 0; (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 18)))); if (c) goto L_1096b7c; }
    (v0 = (uint64_t)(a1 << 24));
    (v0 = (uint64_t)((int64_t)v0 >> (32 + 0)));
    (v0 = (uint64_t)(v0 & (uint64_t)(uint16_t)1));
    { const bool c = (int64_t)v0 != 0; (a0 = (uint64_t)(a0 ^ (uint64_t)(uint16_t)1)); if (c) goto L_1096b5c; }
    if ((int64_t)a2 == 0) goto L_1096b70;
L_1096b5c:;
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 3))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 6))));
    { if (a0 != 0) v0 = (uint64_t)(v1);; if (xo) { xo[0] = a1; } return v0; }
L_1096b70:;
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 2))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 5))));
    if (a0 != 0) v0 = (uint64_t)(v1);;
L_1096b7c:;
    { if (xo) { xo[0] = a1; } return v0; }
    __builtin_unreachable();
}

// modelResetSurface (0x10993c8): typed by tools/typed_lift2.py (round A3; leaf, extra outputs xo[0]=a0)
inline uint32_t modelResetSurface(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t a0, uint64_t *xo = nullptr)
{
    uint32_t v0 = 0;
    uint32_t v1 = 0;
    v0 = 0x1190000u;
    a0 = a0 << 2;
    v1 = A1_R32(v0 - 0x4038u);
    a0 = a0 + v1;
    { A1_W32(a0, 0u); if (xo) { xo[0] = a1_sx64(a0); } return v0; }
    __builtin_unreachable();
}

// modelSetMatrixTransTag (0x1099438): typed by tools/typed_lift2.py (round A3; loop leaf, extra outputs xo[0]=t1 xo[1]=t2 xo[2]=t3)
inline uint64_t modelSetMatrixTransTag(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint64_t a1, uint64_t a2, uint64_t a3, uint64_t *xo = nullptr)
{
    uint64_t v0 = 0;
    uint64_t v1 = 0;
    uint64_t a0 = 0;
    uint64_t t0 = 0;
    uint64_t t1 = 0;
    uint64_t t2 = 0;
    uint64_t t3 = 0;
    uint64_t t4 = 0;
    uint64_t t5 = 0;
    (t1 = (uint64_t)((uint64_t)a2 + (uint64_t)0ULL));
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 63))));
    (v1 = (uint64_t)(((int64_t)(int64_t)t1 < (int64_t)(int32_t)64) ? 1 : 0));
    (t4 = (uint64_t)((uint64_t)a1 + (uint64_t)0ULL));
    if (RC_U64X(v1) == 0) t1 = (uint64_t)(a0);;
    (v0 = (uint64_t)((int64_t)(int32_t)(SRA32((int32_t)a2, 6))));
    (t2 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v0, 1))));
    (t0 = (uint64_t)((uint64_t)t4 + (uint64_t)0ULL));
    { const bool c = (int64_t)a3 == 0; (t3 = (uint64_t)((int64_t)(int32_t)((int32_t)SUB32((uint32_t)a2, (uint32_t)t1)))); if (c) goto L_1099480; }
    (v0 = (uint64_t)((uint64_t)a2 + (uint64_t)0ULL));
    (a2 = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)24576 << 16))));
    (v0 = (uint64_t)(v0 << 2));
    (a1 = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)256 << 16))));
    (v0 = (uint64_t)((uint64_t)RC_U64X(t2) + (uint64_t)v0));
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)SLL32((uint32_t)t1, 18))));
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)27648 << 16))));
    goto L_10994a0;
L_1099480:;
    (v0 = (uint64_t)((uint64_t)a2 + (uint64_t)0ULL));
    (a2 = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)24576 << 16))));
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)27648 << 16))));
    (v0 = (uint64_t)(v0 << 2));
    (v0 = (uint64_t)((uint64_t)RC_U64X(t2) + (uint64_t)v0));
    (a1 = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)256 << 16))));
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)SLL32((uint32_t)t1, 18))));
    (a0 = (uint64_t)(RC_U64X(a0) | (uint64_t)(uint16_t)32768));
L_10994a0:;
    (v0 = (uint64_t)(v0 | RC_U64X(a2)));
    (v1 = (uint64_t)(RC_U64X(v1) | RC_U64X(a0)));
    (a1 = (uint64_t)(RC_U64X(a1) | (uint64_t)(uint16_t)257));
    A1_W32B((uint32_t)t0, 12, (uint32_t)v1);
    A1_W64B((uint32_t)t0, 0, v0);
    A1_W32B((uint32_t)t0, 8, (uint32_t)a1);
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)SLL32((uint32_t)t1, 6))));
    (a1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 1))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)t4, (uint32_t)v0))));
    (v1 = (uint64_t)(((int64_t)(int64_t)RC_U64X(a1) < (int64_t)(int64_t)RC_U64X(t2)) ? 1 : 0));
    { const bool c = (int64_t)v1 == 0; (t0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v0, 16)))); if (c) goto L_1099544; }
    (t4 = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)27648 << 16))));
    (a1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)t2, 4294967295))));
    (t5 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 63))));
    (t4 = (uint64_t)(RC_U64X(t4) | (uint64_t)(uint16_t)32768));
    (t2 = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)27648 << 16))));
    (a2 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 252))));
L_10994e8:;
    (t1 = (uint64_t)((uint64_t)t3 + (uint64_t)0ULL));
    (v0 = (uint64_t)(((int64_t)(int64_t)t1 < (int64_t)(int32_t)64) ? 1 : 0));
    if (RC_U64X(v0) == 0) t1 = (uint64_t)(t5);;
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)SLL32((uint32_t)t1, 6))));
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)SLL32((uint32_t)t1, 18))));
    (v1 = (uint64_t)(a2 | RC_U64X(v1)));
    (a2 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)a2, 252))));
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)t0, (uint32_t)v0))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)SLL32((uint32_t)t1, 18))));
    (v1 = (uint64_t)(v1 | t2));
    (t3 = (uint64_t)((int64_t)(int32_t)((int32_t)SUB32((uint32_t)t3, (uint32_t)t1))));
    { const bool c = (int64_t)a3 == 0; (v0 = (uint64_t)(RC_U64X(v0) | t4)); if (c) goto L_1099528; }
    A1_W32B((uint32_t)t0, 12, (uint32_t)v1);
    goto L_109952c;
L_1099528:;
    A1_W32B((uint32_t)t0, 12, (uint32_t)v0);
L_109952c:;
    A1_W32B((uint32_t)t0, 8, 0u);
    A1_W32B((uint32_t)t0, 4, 0u);
    A1_W32B((uint32_t)t0, 0, 0u);
    (a1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)a1, 4294967295))));
    { const bool c = (int64_t)a1 != 0; (t0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)a0, 16)))); if (c) { A3_BACK(); goto L_10994e8; } }
L_1099544:;
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)5120 << 16))));
    A1_W32B((uint32_t)t0, 12, 0u);
    (v0 = (uint64_t)(RC_U64X(v0) | (uint64_t)(uint16_t)6));
    A1_W32B((uint32_t)t0, 0, 0u);
    A1_W32B((uint32_t)t0, 4, (uint32_t)v0);
    { A1_W32B((uint32_t)t0, 8, 0u); if (xo) { xo[0] = t1; xo[1] = t2; xo[2] = t3; } return v0; }
    __builtin_unreachable();
}

// ptclmanBuildMatPacket (0x1099e00): typed by tools/typed_lift2.py (round A3; leaf, extra outputs xo[0]=a0 xo[1]=a1 xo[2]=a2 xo[3]=a3 xo[4]=t0 xo[5]=t1 xo[6]=t2 xo[7]=t3)
inline a1::RetVF ptclmanBuildMatPacket(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint64_t a0, float f12, float f13, uint64_t *xo = nullptr)
{
    float f0 = 0.0f;
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
    uint32_t fcr31 = 0;
    (t4 = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)289 << 16))));
    (at = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)16256 << 16))));
    { uint32_t bits = (uint32_t)at; std::memcpy(&f0, &bits, sizeof(bits)); }
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)t4, 23528))));
    (a3 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 71))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)v1, 4))));
    fcr31 = (a1f::le(f12, f0)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
    (a1 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)v1, 0))));
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 8))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)SLL32((uint32_t)v0, 4))));
    (a2 = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)5 << 16))));
    (a2 = (uint64_t)(RC_U64X(a2) | (uint64_t)(uint16_t)4113));
    (t3 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)a1, (uint32_t)v0))));
    (a1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 1))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)t3, 32))));
    (t0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)t3, 48))));
    A1_W64B((uint32_t)v0, 0, RC_U64X(a2));
    A1_W64B((uint32_t)v0, 8, RC_U64X(a3));
    { const bool c = (fcr31 & 0x800000u) != 0u; A1_W64B((uint32_t)t0, 8, RC_U64X(v1)); if (c) goto L_1099e54; }
    (a1 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL));
L_1099e54:;
    fcr31 = (a1f::le(f13, f0)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
    if ((fcr31 & 0x800000u) == 0u) { (v1 = (uint64_t)((uint64_t)RC_U64X(a1) + (uint64_t)0ULL)); goto L_1099e68; }
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 4))));
    (v1 = (uint64_t)(RC_U64X(a1) | RC_U64X(v0)));
L_1099e68:;
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 1))));
    A1_W64B((uint32_t)t0, 0, RC_U64X(v1));
    (a1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 1))));
    { const bool c = (int64_t)a0 == (int64_t)v0; (v1 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL)); if (c) goto L_1099ea8; }
    (v0 = (uint64_t)(((int64_t)(int64_t)a0 < (int64_t)(int32_t)2) ? 1 : 0));
    { const bool c = (int64_t)v0 == 0; (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 2)))); if (c) goto L_1099e98; }
    { const bool c = (int64_t)a0 == 0; (v0 = (uint64_t)(RC_U64X(v1) << (32 + 0))); if (c) goto L_1099eb8; }
    goto L_1099eb8;
L_1099e98:;
    if ((int64_t)a0 == (int64_t)v0) { (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 2)))); goto L_1099eb0; }
    (v0 = (uint64_t)(RC_U64X(v1) << (32 + 0)));
    goto L_1099eb8;
L_1099ea8:;
    (a1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 2))));
    goto L_1099eb4;
L_1099eb0:;
    (a1 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL));
L_1099eb4:;
    (v0 = (uint64_t)(RC_U64X(v1) << (32 + 0)));
L_1099eb8:;
    (v1 = (uint64_t)(0ULL | (uint64_t)(uint16_t)32768));
    (v1 = (uint64_t)(RC_U64X(v1) << 24));
    (v0 = (uint64_t)(v0 >> (32 + 0)));
    (a0 = (uint64_t)(RC_U64X(a1) << 2));
    (v0 = (uint64_t)(v0 | v1));
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)4096 << 16))));
    (a1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 64))));
    (v0 = (uint64_t)(v0 | a0));
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)20480 << 16))));
    (v0 = (uint64_t)(v0 | RC_U64X(a1)));
    (a2 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)t3, 64))));
    (t0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 66))));
    (v1 = (uint64_t)(RC_U64X(v1) | (uint64_t)(uint16_t)4));
    (t1 = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)4864 << 16))));
    (a0 = (uint64_t)(RC_U64X(a0) | (uint64_t)(uint16_t)4));
    (t2 = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)4096 << 16))));
    (t2 = (uint64_t)(RC_U64X(t2) << (32 + 0)));
    (t2 = (uint64_t)(t2 | (uint64_t)(uint16_t)32771));
    (a1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 14))));
    A1_W64B((uint32_t)a2, 0, v0);
    A1_W64B((uint32_t)a2, 8, RC_U64X(t0));
    (a3 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)t4, 23528))));
    A1_W64B((uint32_t)t3, 24, RC_U64X(a1));
    A1_W32B((uint32_t)t3, 0, (uint32_t)v1);
    A1_W32B((uint32_t)t3, 8, (uint32_t)t1);
    A1_W32B((uint32_t)t3, 12, (uint32_t)a0);
    A1_W64B((uint32_t)t3, 16, t2);
    A1_W32B((uint32_t)t3, 4, 0u);
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)a3, 4))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v0, 5))));
    { A1_W32B((uint32_t)a3, 4, (uint32_t)v0); if (xo) { xo[0] = a0; xo[1] = a1; xo[2] = a2; xo[3] = a3; xo[4] = t0; xo[5] = t1; xo[6] = t2; xo[7] = t3; } return a1::RetVF{(uint64_t)v0, f0}; }
    __builtin_unreachable();
}

// GetMeshClass (0x109aa48): typed by tools/typed_lift2.py (round A3; leaf, extra outputs xo[0]=a0)
inline uint64_t GetMeshClass(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint64_t a0, uint64_t *xo = nullptr)
{
    uint64_t v0 = 0;
    uint64_t v1 = 0;
    (v0 = (uint64_t)((uint64_t)a0 + (uint64_t)0ULL));
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 35))));
    (v1 = (uint64_t)(uint32_t)((uint8_t)A1_R8B((uint32_t)v0, 8)));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v0, 32))));
    (v1 = (uint64_t)(RC_U64X(v1) ^ RC_U64X(a0)));
    { if (RC_U64X(v1) != 0) v0 = (uint64_t)0;; if (xo) { xo[0] = a0; } return v0; }
    __builtin_unreachable();
}

// meshFlip (0x109abc8): typed by tools/typed_lift2.py (round A3; leaf, extra outputs xo[0]=a0)
inline uint32_t meshFlip(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t a0, uint64_t *xo = nullptr)
{
    uint32_t v0 = 0;
    uint32_t v1 = 0;
    v1 = A1_R8B(a0, 8);
    a0 = a0 + 0x20u;
    v1 = v1 ^ 0x23u;
    if (a1_sx64(v1) != 0) a0 = (uint32_t)((uint64_t)0);;
    v0 = A1_R32B(a0, 16);
    v0 = v0 < 1u ? 1u : 0u;
    { A1_W32B(a0, 16, v0); if (xo) { xo[0] = a1_sx64(a0); } return v0; }
    __builtin_unreachable();
}

// spriteSetTextureCoordinateST (0x10a52e0): typed by tools/typed_lift2.py (round A3; leaf, extra outputs xo[0]=f14 xo[1]=f15)
inline uint64_t spriteSetTextureCoordinateST(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint64_t a0, float f12, float f13, float f14, float f15, uint64_t *xo = nullptr)
{
    uint64_t v0 = 0;
    uint64_t v1 = 0;
    f14 = a1f::add(f12, f14);
    (v0 = (uint64_t)(A1_R64B((uint32_t)a0, 72)));
    f15 = a1f::add(f13, f15);
    (v1 = (uint64_t)(0ULL | (uint64_t)(uint16_t)32768));
    (v1 = (uint64_t)(RC_U64X(v1) << (32 + 1)));
    (v0 = (uint64_t)(v0 | v1));
    { float f = f12; uint32_t bits; std::memcpy(&bits, &f, sizeof(bits)); A1_W32B((uint32_t)a0, 32, bits); }
    { float f = f13; uint32_t bits; std::memcpy(&bits, &f, sizeof(bits)); A1_W32B((uint32_t)a0, 36, bits); }
    A1_W64B((uint32_t)a0, 72, v0);
    { float f = f15; uint32_t bits; std::memcpy(&bits, &f, sizeof(bits)); A1_W32B((uint32_t)a0, 44, bits); }
    { { float f = f14; uint32_t bits; std::memcpy(&bits, &f, sizeof(bits)); A1_W32B((uint32_t)a0, 40, bits); } if (xo) { xo[0] = a1_bits(f14); xo[1] = a1_bits(f15); } return v0; }
    __builtin_unreachable();
}

// spriteSetBlendMode (0x10a5440): typed by tools/typed_lift2.py (round A3; leaf, extra outputs xo[0]=a2 xo[1]=a3 xo[2]=t0)
inline uint64_t spriteSetBlendMode(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint64_t a0, uint64_t a1, uint64_t a2, uint64_t a3, uint64_t *xo = nullptr)
{
    uint64_t v0 = 0;
    uint64_t v1 = 0;
    uint64_t t0 = 0;
    (v0 = (uint64_t)(A1_R64B((uint32_t)a0, 72)));
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 4294963455))));
    (v1 = (uint64_t)(RC_U64X(v1) << 16));
    (v1 = (uint64_t)(v1 | (uint64_t)(uint16_t)65535));
    (v1 = (uint64_t)(v1 << 16));
    (v1 = (uint64_t)(v1 | (uint64_t)(uint16_t)65535));
    (a1 = (uint64_t)(a1 & (uint64_t)(uint16_t)15));
    (a2 = (uint64_t)(a2 << (32 + 0)));
    (v0 = (uint64_t)(v0 & v1));
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 4294955007))));
    (v1 = (uint64_t)(RC_U64X(v1) << 16));
    (v1 = (uint64_t)(v1 | (uint64_t)(uint16_t)65535));
    (v1 = (uint64_t)(v1 << 16));
    (v1 = (uint64_t)(v1 | (uint64_t)(uint16_t)65535));
    (t0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 3))));
    (a1 = (uint64_t)(RC_U64X(a1) << (32 + 8)));
    (a2 = (uint64_t)(a2 >> (32 + 0)));
    (v0 = (uint64_t)(v0 | a1));
    (a2 = (uint64_t)(a2 & RC_U64X(t0)));
    (a3 = (uint64_t)(a3 << (32 + 0)));
    (v0 = (uint64_t)(v0 & v1));
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)65535 << 16))));
    (v1 = (uint64_t)(RC_U64X(v1) | (uint64_t)(uint16_t)16383));
    (v1 = (uint64_t)(RC_U64X(v1) << 16));
    (v1 = (uint64_t)(v1 | (uint64_t)(uint16_t)65535));
    (v1 = (uint64_t)(v1 << 16));
    (v1 = (uint64_t)(v1 | (uint64_t)(uint16_t)65535));
    (a2 = (uint64_t)(a2 << (32 + 12)));
    (a3 = (uint64_t)(a3 >> (32 + 0)));
    (a3 = (uint64_t)(a3 & RC_U64X(t0)));
    (v0 = (uint64_t)(v0 | a2));
    (a3 = (uint64_t)(a3 << (32 + 14)));
    (v0 = (uint64_t)(v0 & v1));
    (v0 = (uint64_t)(v0 | a3));
    { A1_W64B((uint32_t)a0, 72, v0); if (xo) { xo[0] = a2; xo[1] = a3; xo[2] = t0; } return v0; }
    __builtin_unreachable();
}

// texGetTexTBPSize (0x10aacb8): typed by tools/typed_lift2.py (round A3; leaf, extra outputs xo[0]=a3)
inline uint64_t texGetTexTBPSize(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t a0, uint64_t a1, uint32_t a2, uint64_t *xo = nullptr)
{
    uint64_t v0 = 0;
    uint32_t v1 = 0;
    uint32_t a3 = 0;
    uint64_t hi = 0;
    uint64_t lo = 0;
    a3 = a2 << 1;
    { int64_t result = (int64_t)(int32_t)a0 * (int64_t)(int32_t)a1; lo = (uint64_t)(int64_t)(int32_t)result; hi = (uint64_t)(int64_t)(int32_t)(result >> 32); (a0 = (uint32_t)((int32_t)((int32_t)result))); }
    a3 = a3 + a2;
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 1))));
    a3 = a3 << 2;
    v1 = 0x1190000u;
    v1 = v1 + a3;
    v1 = A1_R32(v1 - 0x3E70u);
    v1 = v1 + 6u;
    a0 = (uint32_t)((int32_t)a0 >> (v1 & 31));
    (a1 = (uint64_t)(((int64_t)(int64_t)a1_sx64(a0) < (int64_t)(int32_t)1) ? 1 : 0));
    { if (RC_U64X(a1) == 0) v0 = (uint64_t)(a1_sx64(a0));; if (xo) { xo[0] = a1_sx64(a3); } return v0; }
    __builtin_unreachable();
}

// manager_sub_010AE658 (0x10ae658): typed by tools/typed_lift2.py (round A3; loop leaf, extra outputs xo[0]=f0 xo[1]=t1 xo[2]=t2)
inline uint64_t manager_sub_010AE658(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t a0, uint64_t a1, uint32_t a2, float f12, float f13, uint64_t *xo = nullptr)
{
    float f0 = 0.0f;
    float f1 = 0.0f;
    uint64_t v0 = 0;
    uint64_t v1 = 0;
    uint64_t a3 = 0;
    uint32_t t0 = 0;
    uint32_t t1 = 0;
    uint32_t t2 = 0;
    uint32_t fcr31 = 0;
    bool a3xw0 = false;
    bool a3xw1 = false;
    a2 = A1_R32(a2);
    { const bool c = a2 == 0; (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 4294967295)))); if (c) goto L_10aeaf0; }
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 1))));
    { const bool c = (int64_t)a1 == (int64_t)v0; (v0 = (uint64_t)(((int64_t)(int64_t)a1 < (int64_t)(int32_t)2) ? 1 : 0)); if (c) goto L_10ae7b8; }
    { const bool c = (int64_t)v0 == 0; (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 2)))); if (c) goto L_10ae688; }
    { const bool c = (int64_t)a1 == 0; (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 4294967295)))); if (c) goto L_10ae698; }
    goto L_10aeaf0;
L_10ae688:;
    { const bool c = (int64_t)a1 == (int64_t)v0; (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 4294967295)))); if (c) goto L_10ae8d8; }
    goto L_10aeaf0;
L_10ae698:;
    A1_W8B(a0, 2, (uint8_t)0u);
    A1_W8B(a0, 1, (uint8_t)0u);
    A1_W8(a0, (uint8_t)0u);
    (v0 = (uint64_t)(uint32_t)((uint8_t)A1_R8B(a2, 1)));
    { const bool c = (int64_t)v0 == 0; (a3 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 4294967295)))); if (c) goto L_10ae8d0; }
    (a1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 1))));
    (v0 = (uint64_t)(uint32_t)((uint8_t)A1_R8B(a2, 0)));
L_10ae6b8:;
    (v1 = (uint64_t)(uint32_t)((uint8_t)A1_R8B(a2, 1)));
    { uint32_t bits = (uint32_t)v0; std::memcpy(&f0, &bits, sizeof(bits)); } a3xw0 = true;
    { int32_t tmp; std::memcpy(&tmp, &f0, sizeof(tmp)); f0 = ((float)(int32_t)(tmp)); } a3xw0 = true;
    fcr31 = (a1f::lt(f0, f12)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
    { const bool c = (fcr31 & 0x800000u) == 0u; (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v0, (uint32_t)v1)))); if (c) goto L_10ae6dc; }
    fcr31 = (a1f::le(f13, f0)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
    if ((fcr31 & 0x800000u) != 0u) { A1_W8B(a0, 0, (uint8_t)(uint32_t)a1); goto L_10ae6dc; }
L_10ae6dc:;
    { uint32_t bits = (uint32_t)v0; std::memcpy(&f1, &bits, sizeof(bits)); }
    { int32_t tmp; std::memcpy(&tmp, &f1, sizeof(tmp)); f1 = ((float)(int32_t)(tmp)); }
    fcr31 = (a1f::lt(f1, f12)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
    if ((fcr31 & 0x800000u) == 0u) { (v1 = (uint64_t)(uint32_t)((uint8_t)A1_R8B(a2, 0))); goto L_10ae700; }
    fcr31 = (a1f::le(f13, f1)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
    if ((fcr31 & 0x800000u) != 0u) { A1_W8B(a0, 2, (uint8_t)(uint32_t)a1); goto L_10ae6fc; }
L_10ae6fc:;
    (v1 = (uint64_t)(uint32_t)((uint8_t)A1_R8B(a2, 0)));
L_10ae700:;
    { uint32_t bits = (uint32_t)v1; std::memcpy(&f0, &bits, sizeof(bits)); } a3xw0 = true;
    { int32_t tmp; std::memcpy(&tmp, &f0, sizeof(tmp)); f0 = ((float)(int32_t)(tmp)); } a3xw0 = true;
    fcr31 = (a1f::le(f13, f0)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
    if ((fcr31 & 0x800000u) == 0u) goto L_10ae734;
    fcr31 = (a1f::le(f1, f12)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
    if ((fcr31 & 0x800000u) == 0u) goto L_10ae734;
    A1_W8B(a0, 1, (uint8_t)(uint32_t)a1);
    (a3 = (uint64_t)((uint64_t)RC_U64X(v0) + (uint64_t)0ULL));
    (v1 = (uint64_t)(uint32_t)((uint8_t)A1_R8B(a2, 0)));
    { uint32_t bits = (uint32_t)v1; std::memcpy(&f0, &bits, sizeof(bits)); } a3xw0 = true;
    { int32_t tmp; std::memcpy(&tmp, &f0, sizeof(tmp)); f0 = ((float)(int32_t)(tmp)); } a3xw0 = true;
L_10ae734:;
    fcr31 = (a1f::le(f0, f13)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
    if ((fcr31 & 0x800000u) == 0u) goto L_10ae75c;
    fcr31 = (a1f::le(f13, f1)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
    if ((fcr31 & 0x800000u) == 0u) goto L_10ae75c;
    A1_W8B(a0, 1, (uint8_t)(uint32_t)a1);
    { int32_t tmp = a1f::cvtw(f13); std::memcpy(&f0, &tmp, sizeof(tmp)); } a3xw0 = true;
    { uint32_t bits; std::memcpy(&bits, &f0, sizeof(bits)); (a3 = (uint64_t)((int64_t)(int32_t)(bits))); }
    (v1 = (uint64_t)(uint32_t)((uint8_t)A1_R8B(a2, 0)));
L_10ae75c:;
    { uint32_t bits = (uint32_t)v1; std::memcpy(&f0, &bits, sizeof(bits)); } a3xw0 = true;
    { int32_t tmp; std::memcpy(&tmp, &f0, sizeof(tmp)); f0 = ((float)(int32_t)(tmp)); } a3xw0 = true;
    fcr31 = (a1f::le(f0, f12)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
    if ((fcr31 & 0x800000u) == 0u) goto L_10ae794;
    fcr31 = (a1f::le(f12, f1)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
    if ((fcr31 & 0x800000u) == 0u) goto L_10ae794;
    A1_W8B(a0, 1, (uint8_t)(uint32_t)a1);
    { int32_t tmp = a1f::cvtw(f12); std::memcpy(&f0, &tmp, sizeof(tmp)); } a3xw0 = true;
    { uint32_t bits; std::memcpy(&bits, &f0, sizeof(bits)); (a3 = (uint64_t)((int64_t)(int32_t)(bits))); }
    (v1 = (uint64_t)(uint32_t)((uint8_t)A1_R8B(a2, 0)));
    { uint32_t bits = (uint32_t)v1; std::memcpy(&f0, &bits, sizeof(bits)); } a3xw0 = true;
    { int32_t tmp; std::memcpy(&tmp, &f0, sizeof(tmp)); f0 = ((float)(int32_t)(tmp)); } a3xw0 = true;
L_10ae794:;
    fcr31 = (a1f::lt(f12, f0)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
    if ((fcr31 & 0x800000u) != 0u) goto L_10ae8d0;
    a2 = a2 + 2u;
    (v0 = (uint64_t)(uint32_t)((uint8_t)A1_R8B(a2, 1)));
    if ((int64_t)v0 != 0) { (v0 = (uint64_t)(uint32_t)((uint8_t)A1_R8B(a2, 0))); { A3_BACK(); goto L_10ae6b8; } }
    { (v0 = (uint64_t)((uint64_t)a3 + (uint64_t)0ULL)); if (xo) { xo[0] = a1_bits(f0); } return v0; }
L_10ae7b8:;
    A1_W8B(a0, 2, (uint8_t)0u);
    A1_W8B(a0, 1, (uint8_t)0u);
    A1_W8(a0, (uint8_t)0u);
    (v0 = (uint64_t)(uint32_t)((uint16_t)A1_R16B(a2, 2)));
    { const bool c = (int64_t)v0 == 0; (a3 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 4294967295)))); if (c) goto L_10ae8d0; }
    (a1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 1))));
    (v0 = (uint64_t)(uint32_t)((uint16_t)A1_R16B(a2, 0)));
L_10ae7d8:;
    (v1 = (uint64_t)(uint32_t)((uint16_t)A1_R16B(a2, 2)));
    { uint32_t bits = (uint32_t)v0; std::memcpy(&f0, &bits, sizeof(bits)); } a3xw0 = true;
    { int32_t tmp; std::memcpy(&tmp, &f0, sizeof(tmp)); f0 = ((float)(int32_t)(tmp)); } a3xw0 = true;
    fcr31 = (a1f::lt(f0, f12)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
    { const bool c = (fcr31 & 0x800000u) == 0u; (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v0, (uint32_t)v1)))); if (c) goto L_10ae7fc; }
    fcr31 = (a1f::le(f13, f0)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
    if ((fcr31 & 0x800000u) != 0u) { A1_W8B(a0, 0, (uint8_t)(uint32_t)a1); goto L_10ae7fc; }
L_10ae7fc:;
    { uint32_t bits = (uint32_t)v0; std::memcpy(&f1, &bits, sizeof(bits)); }
    { int32_t tmp; std::memcpy(&tmp, &f1, sizeof(tmp)); f1 = ((float)(int32_t)(tmp)); }
    fcr31 = (a1f::lt(f1, f12)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
    if ((fcr31 & 0x800000u) == 0u) { (v1 = (uint64_t)(uint32_t)((uint16_t)A1_R16B(a2, 0))); goto L_10ae820; }
    fcr31 = (a1f::le(f13, f1)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
    if ((fcr31 & 0x800000u) != 0u) { A1_W8B(a0, 2, (uint8_t)(uint32_t)a1); goto L_10ae81c; }
L_10ae81c:;
    (v1 = (uint64_t)(uint32_t)((uint16_t)A1_R16B(a2, 0)));
L_10ae820:;
    { uint32_t bits = (uint32_t)v1; std::memcpy(&f0, &bits, sizeof(bits)); } a3xw0 = true;
    { int32_t tmp; std::memcpy(&tmp, &f0, sizeof(tmp)); f0 = ((float)(int32_t)(tmp)); } a3xw0 = true;
    fcr31 = (a1f::le(f13, f0)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
    if ((fcr31 & 0x800000u) == 0u) goto L_10ae854;
    fcr31 = (a1f::le(f1, f12)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
    if ((fcr31 & 0x800000u) == 0u) goto L_10ae854;
    A1_W8B(a0, 1, (uint8_t)(uint32_t)a1);
    (a3 = (uint64_t)((uint64_t)RC_U64X(v0) + (uint64_t)0ULL));
    (v1 = (uint64_t)(uint32_t)((uint16_t)A1_R16B(a2, 0)));
    { uint32_t bits = (uint32_t)v1; std::memcpy(&f0, &bits, sizeof(bits)); } a3xw0 = true;
    { int32_t tmp; std::memcpy(&tmp, &f0, sizeof(tmp)); f0 = ((float)(int32_t)(tmp)); } a3xw0 = true;
L_10ae854:;
    fcr31 = (a1f::le(f0, f13)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
    if ((fcr31 & 0x800000u) == 0u) goto L_10ae87c;
    fcr31 = (a1f::le(f13, f1)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
    if ((fcr31 & 0x800000u) == 0u) goto L_10ae87c;
    A1_W8B(a0, 1, (uint8_t)(uint32_t)a1);
    { int32_t tmp = a1f::cvtw(f13); std::memcpy(&f0, &tmp, sizeof(tmp)); } a3xw0 = true;
    { uint32_t bits; std::memcpy(&bits, &f0, sizeof(bits)); (a3 = (uint64_t)((int64_t)(int32_t)(bits))); }
    (v1 = (uint64_t)(uint32_t)((uint16_t)A1_R16B(a2, 0)));
L_10ae87c:;
    { uint32_t bits = (uint32_t)v1; std::memcpy(&f0, &bits, sizeof(bits)); } a3xw0 = true;
    { int32_t tmp; std::memcpy(&tmp, &f0, sizeof(tmp)); f0 = ((float)(int32_t)(tmp)); } a3xw0 = true;
    fcr31 = (a1f::le(f0, f12)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
    if ((fcr31 & 0x800000u) == 0u) goto L_10ae8b4;
    fcr31 = (a1f::le(f12, f1)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
    if ((fcr31 & 0x800000u) == 0u) goto L_10ae8b4;
    A1_W8B(a0, 1, (uint8_t)(uint32_t)a1);
    { int32_t tmp = a1f::cvtw(f12); std::memcpy(&f0, &tmp, sizeof(tmp)); } a3xw0 = true;
    { uint32_t bits; std::memcpy(&bits, &f0, sizeof(bits)); (a3 = (uint64_t)((int64_t)(int32_t)(bits))); }
    (v1 = (uint64_t)(uint32_t)((uint16_t)A1_R16B(a2, 0)));
    { uint32_t bits = (uint32_t)v1; std::memcpy(&f0, &bits, sizeof(bits)); } a3xw0 = true;
    { int32_t tmp; std::memcpy(&tmp, &f0, sizeof(tmp)); f0 = ((float)(int32_t)(tmp)); } a3xw0 = true;
L_10ae8b4:;
    fcr31 = (a1f::lt(f12, f0)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
    if ((fcr31 & 0x800000u) != 0u) goto L_10ae8d0;
    a2 = a2 + 4u;
    (v0 = (uint64_t)(uint32_t)((uint16_t)A1_R16B(a2, 2)));
    if ((int64_t)v0 != 0) { (v0 = (uint64_t)(uint32_t)((uint16_t)A1_R16B(a2, 0))); { A3_BACK(); goto L_10ae7d8; } }
L_10ae8d0:;
    { (v0 = (uint64_t)((uint64_t)a3 + (uint64_t)0ULL)); if (xo) { if (a3xw0) xo[0] = a1_bits(f0); } return v0; }
L_10ae8d8:;
    A1_W8B(a0, 2, (uint8_t)0u);
    A1_W8B(a0, 1, (uint8_t)0u);
    A1_W8(a0, (uint8_t)0u);
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B(a2, 4))));
    { const bool c = (int64_t)v1 == 0; t2 = 0xFFFFFFFFu; if (c) goto L_10aeae8; }
    t1 = 1u; a3xw1 = true;
L_10ae8f8:;
    (a1 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B(a2, 0))));
    (t0 = (uint32_t)((int32_t)((int32_t)ADD32((uint32_t)a1, (uint32_t)v1))));
    { const bool c = (int32_t)a1 < 0; (a3 = (uint64_t)((uint64_t)RC_U64X(a1) + (uint64_t)0ULL)); if (c) goto L_10ae918; }
    { uint32_t bits = (uint32_t)a1; std::memcpy(&f0, &bits, sizeof(bits)); } a3xw0 = true;
    { int32_t tmp; std::memcpy(&tmp, &f0, sizeof(tmp)); f0 = ((float)(int32_t)(tmp)); } a3xw0 = true;
    goto L_10ae930;
L_10ae918:;
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)SRL32((uint32_t)a1, 1))));
    (v0 = (uint64_t)(RC_U64X(a1) & (uint64_t)(uint16_t)1));
    (v0 = (uint64_t)(RC_U64X(v0) | RC_U64X(v1)));
    { uint32_t bits = (uint32_t)v0; std::memcpy(&f0, &bits, sizeof(bits)); } a3xw0 = true;
    { int32_t tmp; std::memcpy(&tmp, &f0, sizeof(tmp)); f0 = ((float)(int32_t)(tmp)); } a3xw0 = true;
    f0 = a1f::add(f0, f0); a3xw0 = true;
L_10ae930:;
    fcr31 = (a1f::lt(f0, f12)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
    if ((fcr31 & 0x800000u) == 0u) goto L_10ae978;
    { const bool c = (int32_t)a3 < 0; (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)SRL32((uint32_t)a3, 1)))); if (c) goto L_10ae958; }
    { uint32_t bits = (uint32_t)a3; std::memcpy(&f0, &bits, sizeof(bits)); } a3xw0 = true;
    { int32_t tmp; std::memcpy(&tmp, &f0, sizeof(tmp)); f0 = ((float)(int32_t)(tmp)); } a3xw0 = true;
    goto L_10ae96c;
L_10ae958:;
    (v0 = (uint64_t)(a3 & (uint64_t)(uint16_t)1));
    (v0 = (uint64_t)(RC_U64X(v0) | v1));
    { uint32_t bits = (uint32_t)v0; std::memcpy(&f0, &bits, sizeof(bits)); } a3xw0 = true;
    { int32_t tmp; std::memcpy(&tmp, &f0, sizeof(tmp)); f0 = ((float)(int32_t)(tmp)); } a3xw0 = true;
    f0 = a1f::add(f0, f0); a3xw0 = true;
L_10ae96c:;
    fcr31 = (a1f::le(f13, f0)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
    if ((fcr31 & 0x800000u) != 0u) { A1_W8(a0, (uint8_t)t1); goto L_10ae978; }
L_10ae978:;
    f1 = a1_flt(t0);
    { int32_t tmp; std::memcpy(&tmp, &f1, sizeof(tmp)); f1 = ((float)(int32_t)(tmp)); }
    fcr31 = (a1f::lt(f1, f12)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
    if ((fcr31 & 0x800000u) == 0u) { (a1 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B(a2, 0)))); goto L_10ae99c; }
    fcr31 = (a1f::le(f13, f1)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
    if ((fcr31 & 0x800000u) != 0u) { A1_W8B(a0, 2, (uint8_t)t1); goto L_10ae998; }
L_10ae998:;
    (a1 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B(a2, 0))));
L_10ae99c:;
    { const bool c = (int32_t)a1 < 0; (v0 = (uint64_t)(RC_U64X(a1) & (uint64_t)(uint16_t)1)); if (c) goto L_10ae9b8; }
    { uint32_t bits = (uint32_t)a1; std::memcpy(&f0, &bits, sizeof(bits)); } a3xw0 = true;
    { int32_t tmp; std::memcpy(&tmp, &f0, sizeof(tmp)); f0 = ((float)(int32_t)(tmp)); } a3xw0 = true;
    goto L_10ae9cc;
L_10ae9b8:;
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)SRL32((uint32_t)a1, 1))));
    (v0 = (uint64_t)(v0 | RC_U64X(v1)));
    { uint32_t bits = (uint32_t)v0; std::memcpy(&f0, &bits, sizeof(bits)); } a3xw0 = true;
    { int32_t tmp; std::memcpy(&tmp, &f0, sizeof(tmp)); f0 = ((float)(int32_t)(tmp)); } a3xw0 = true;
    f0 = a1f::add(f0, f0); a3xw0 = true;
L_10ae9cc:;
    fcr31 = (a1f::le(f13, f0)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
    if ((fcr31 & 0x800000u) == 0u) goto L_10ae9f0;
    fcr31 = (a1f::le(f1, f12)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
    if ((fcr31 & 0x800000u) == 0u) goto L_10ae9f0;
    A1_W8B(a0, 1, (uint8_t)t1);
    t2 = t0;
    (a1 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B(a2, 0))));
L_10ae9f0:;
    { const bool c = (int32_t)a1 < 0; (v0 = (uint64_t)(a1 & (uint64_t)(uint16_t)1)); if (c) goto L_10aea08; }
    { uint32_t bits = (uint32_t)a1; std::memcpy(&f0, &bits, sizeof(bits)); } a3xw0 = true;
    { int32_t tmp; std::memcpy(&tmp, &f0, sizeof(tmp)); f0 = ((float)(int32_t)(tmp)); } a3xw0 = true;
    goto L_10aea1c;
L_10aea08:;
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)SRL32((uint32_t)a1, 1))));
    (v0 = (uint64_t)(RC_U64X(v0) | RC_U64X(v1)));
    { uint32_t bits = (uint32_t)v0; std::memcpy(&f0, &bits, sizeof(bits)); } a3xw0 = true;
    { int32_t tmp; std::memcpy(&tmp, &f0, sizeof(tmp)); f0 = ((float)(int32_t)(tmp)); } a3xw0 = true;
    f0 = a1f::add(f0, f0); a3xw0 = true;
L_10aea1c:;
    fcr31 = (a1f::le(f0, f13)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
    if ((fcr31 & 0x800000u) == 0u) goto L_10aea44;
    fcr31 = (a1f::le(f13, f1)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
    if ((fcr31 & 0x800000u) == 0u) goto L_10aea44;
    A1_W8B(a0, 1, (uint8_t)t1);
    { int32_t tmp = a1f::cvtw(f13); std::memcpy(&f0, &tmp, sizeof(tmp)); } a3xw0 = true;
    t2 = a1_bits(f0);
    (a1 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B(a2, 0))));
L_10aea44:;
    { const bool c = (int32_t)a1 < 0; (v0 = (uint64_t)(a1 & (uint64_t)(uint16_t)1)); if (c) goto L_10aea60; }
    { uint32_t bits = (uint32_t)a1; std::memcpy(&f0, &bits, sizeof(bits)); } a3xw0 = true;
    { int32_t tmp; std::memcpy(&tmp, &f0, sizeof(tmp)); f0 = ((float)(int32_t)(tmp)); } a3xw0 = true;
    goto L_10aea74;
L_10aea60:;
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)SRL32((uint32_t)a1, 1))));
    (v0 = (uint64_t)(v0 | RC_U64X(v1)));
    { uint32_t bits = (uint32_t)v0; std::memcpy(&f0, &bits, sizeof(bits)); } a3xw0 = true;
    { int32_t tmp; std::memcpy(&tmp, &f0, sizeof(tmp)); f0 = ((float)(int32_t)(tmp)); } a3xw0 = true;
    f0 = a1f::add(f0, f0); a3xw0 = true;
L_10aea74:;
    fcr31 = (a1f::le(f0, f12)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
    if ((fcr31 & 0x800000u) == 0u) goto L_10aea9c;
    fcr31 = (a1f::le(f12, f1)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
    if ((fcr31 & 0x800000u) == 0u) goto L_10aea9c;
    A1_W8B(a0, 1, (uint8_t)t1);
    { int32_t tmp = a1f::cvtw(f12); std::memcpy(&f0, &tmp, sizeof(tmp)); } a3xw0 = true;
    t2 = a1_bits(f0);
    (a1 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B(a2, 0))));
L_10aea9c:;
    { const bool c = (int32_t)a1 < 0; (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)SRL32((uint32_t)a1, 1)))); if (c) goto L_10aeab8; }
    { uint32_t bits = (uint32_t)a1; std::memcpy(&f0, &bits, sizeof(bits)); } a3xw0 = true;
    { int32_t tmp; std::memcpy(&tmp, &f0, sizeof(tmp)); f0 = ((float)(int32_t)(tmp)); } a3xw0 = true;
    goto L_10aeacc;
L_10aeab8:;
    (v0 = (uint64_t)(a1 & (uint64_t)(uint16_t)1));
    (v0 = (uint64_t)(RC_U64X(v0) | v1));
    { uint32_t bits = (uint32_t)v0; std::memcpy(&f0, &bits, sizeof(bits)); } a3xw0 = true;
    { int32_t tmp; std::memcpy(&tmp, &f0, sizeof(tmp)); f0 = ((float)(int32_t)(tmp)); } a3xw0 = true;
    f0 = a1f::add(f0, f0); a3xw0 = true;
L_10aeacc:;
    fcr31 = (a1f::lt(f12, f0)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
    if ((fcr31 & 0x800000u) != 0u) goto L_10aeae8;
    a2 = a2 + 8u;
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B(a2, 4))));
    { const bool c = (int64_t)v0 != 0; (v1 = (uint64_t)((uint64_t)RC_U64X(v0) + (uint64_t)0ULL)); if (c) { A3_BACK(); goto L_10ae8f8; } }
L_10aeae8:;
    { (v0 = (uint64_t)((uint64_t)a1_sx64(t2) + (uint64_t)0ULL)); if (xo) { if (a3xw0) xo[0] = a1_bits(f0); if (a3xw1) xo[1] = a1_sx64(t1); xo[2] = a1_sx64(t2); } return v0; }
L_10aeaf0:;
    { return v0; }
    __builtin_unreachable();
}

// getBlendBaseMotion (0x10b2790): typed by tools/typed_lift2.py (round A3; loop leaf, extra outputs xo[0]=a2 xo[1]=a3 xo[2]=t0 xo[3]=t1)
inline uint64_t getBlendBaseMotion(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint64_t a0, uint64_t *xo = nullptr)
{
    uint64_t v0 = 0;
    uint32_t v1 = 0;
    uint32_t a1 = 0;
    uint64_t a2 = 0;
    uint64_t a3 = 0;
    uint32_t t0 = 0;
    uint32_t t1 = 0;
    uint64_t hi = 0;
    uint64_t lo = 0;
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)3 << 16))));
    (a2 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)a0, 0))));
    (v0 = (uint64_t)(RC_U64X(v0) | (uint64_t)(uint16_t)65535));
    (v0 = (uint64_t)(RC_U64X(a2) & RC_U64X(v0)));
    { const bool c = (int64_t)v0 == 0; (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)SUB32((uint32_t)a0, (uint32_t)v0)))); if (c) goto L_10b27b0; }
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)v0, 0))));
    goto L_10b27b4;
L_10b27b0:;
    (a0 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL));
L_10b27b4:;
    { const bool c = (int64_t)a0 == 0; { const uint32_t _dynInsn = A1_R32(0x10B27B8u); const int32_t _dynSimm = (int16_t)(_dynInsn & 0xFFFFu); const uint32_t _dynImm = _dynInsn & 0xFFFFu; (void)_dynSimm; (void)_dynImm; (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)_dynImm << 16)))); } if (c) goto L_10b27e8; }
    { const uint32_t _dynInsn = A1_R32(0x10B27BCu); const int32_t _dynSimm = (int16_t)(_dynInsn & 0xFFFFu); const uint32_t _dynImm = _dynInsn & 0xFFFFu; (void)_dynSimm; (void)_dynImm; (v1 = (uint32_t)((int32_t)((int32_t)A1_R32(ADD32((uint32_t)v0, _dynSimm))))); }
    { const bool c = (int64_t)a0 == (int64_t)(int32_t)v1; (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)SRL32((uint32_t)a2, 18)))); if (c) goto L_10b27e8; }
    (a1 = (uint32_t)((int32_t)((int32_t)A1_R32B((uint32_t)a0, 8))));
    (v0 = (uint64_t)(RC_U64X(v0) & (uint64_t)(uint16_t)4095));
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)a0, 16))));
    v1 = A1_R32B(a1, 12);
    { int64_t result = (int64_t)(int32_t)v0 * (int64_t)(int32_t)v1; lo = (uint64_t)(int64_t)(int32_t)result; hi = (uint64_t)(int64_t)(int32_t)(result >> 32); (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)result))); }
    (a3 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)a0, (uint32_t)v0))));
    goto L_10b27ec;
L_10b27e8:;
    (a3 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL));
L_10b27ec:;
    t0 = 0x30000u;
    { const uint32_t _dynInsn = A1_R32(0x10B27F0u); const int32_t _dynSimm = (int16_t)(_dynInsn & 0xFFFFu); const uint32_t _dynImm = _dynInsn & 0xFFFFu; (void)_dynSimm; (void)_dynImm; (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)_dynImm << 16)))); }
    { const uint32_t _dynInsn = A1_R32(0x10B27F4u); const int32_t _dynSimm = (int16_t)(_dynInsn & 0xFFFFu); const uint32_t _dynImm = _dynInsn & 0xFFFFu; (void)_dynSimm; (void)_dynImm; (t1 = (uint32_t)((int32_t)((int32_t)ADD32((uint32_t)v0, _dynSimm)))); }
    (a2 = (uint64_t)((uint64_t)RC_U64X(a3) + (uint64_t)0ULL));
    t0 = t0 | 0xFFFFu;
    goto L_10b2818;
L_10b2808:;
    v1 = v1 & 1u;
    { const bool c = v1 != 0; (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v0, 80)))); if (c) goto L_10b28d4; }
    (a2 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)a2, 12))));
L_10b2818:;
    (v1 = (uint32_t)((int32_t)((int32_t)A1_R32B((uint32_t)a2, 0))));
    (v0 = (uint64_t)(a1_sx64(v1) & a1_sx64(t0)));
    { const bool c = (int64_t)v0 == 0; (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)SUB32((uint32_t)a2, (uint32_t)v0)))); if (c) goto L_10b2830; }
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)a0, 0))));
    goto L_10b2834;
L_10b2830:;
    (a0 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL));
L_10b2834:;
    { const bool c = (int64_t)a0 == 0; (v0 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL)); if (c) goto L_10b286c; }
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B(t1, 0))));
    { const bool c = (int64_t)a0 == (int64_t)v0; v1 = v1 >> 18; if (c) goto L_10b2868; }
    (a1 = (uint32_t)((int32_t)((int32_t)A1_R32B((uint32_t)a0, 8))));
    v1 = v1 & 0xFFFu;
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)a0, 16))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B(a1, 12))));
    { int64_t result = (int64_t)(int32_t)v1 * (int64_t)(int32_t)v0; lo = (uint64_t)(int64_t)(int32_t)result; hi = (uint64_t)(int64_t)(int32_t)(result >> 32); (v1 = (uint32_t)((int32_t)((int32_t)result))); }
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)a0, v1))));
    goto L_10b286c;
L_10b2868:;
    (v0 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL));
L_10b286c:;
    if ((int64_t)v0 != 0) { (v1 = (uint32_t)((int32_t)((int32_t)A1_R32B((uint32_t)a2, 8)))); { A3_BACK(); goto L_10b2808; } }
    (a2 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)a3, 0))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)3 << 16))));
    (v0 = (uint64_t)(RC_U64X(v0) | (uint64_t)(uint16_t)65535));
    (v0 = (uint64_t)(RC_U64X(a2) & RC_U64X(v0)));
    { const bool c = (int64_t)v0 == 0; (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)SUB32((uint32_t)a3, (uint32_t)v0)))); if (c) goto L_10b2898; }
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)v0, 0))));
    goto L_10b289c;
L_10b2898:;
    (a0 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL));
L_10b289c:;
    { const bool c = (int64_t)a0 == 0; (v0 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL)); if (c) goto L_10b28d0; }
    { const uint32_t _dynInsn = A1_R32(0x10B28A4u); const int32_t _dynSimm = (int16_t)(_dynInsn & 0xFFFFu); const uint32_t _dynImm = _dynInsn & 0xFFFFu; (void)_dynSimm; (void)_dynImm; (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)_dynImm << 16)))); }
    { const uint32_t _dynInsn = A1_R32(0x10B28A8u); const int32_t _dynSimm = (int16_t)(_dynInsn & 0xFFFFu); const uint32_t _dynImm = _dynInsn & 0xFFFFu; (void)_dynSimm; (void)_dynImm; (v1 = (uint32_t)((int32_t)((int32_t)A1_R32(ADD32((uint32_t)v0, _dynSimm))))); }
    if ((int64_t)a0 == (int64_t)(int32_t)v1) { (v0 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL)); goto L_10b28d0; }
    (a1 = (uint32_t)((int32_t)((int32_t)A1_R32B((uint32_t)a0, 8))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)SRL32((uint32_t)a2, 18))));
    (v0 = (uint64_t)(RC_U64X(v0) & (uint64_t)(uint16_t)4095));
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)a0, 16))));
    v1 = A1_R32B(a1, 12);
    { int64_t result = (int64_t)(int32_t)v0 * (int64_t)(int32_t)v1; lo = (uint64_t)(int64_t)(int32_t)result; hi = (uint64_t)(int64_t)(int32_t)(result >> 32); (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)result))); }
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)a0, (uint32_t)v0))));
L_10b28d0:;
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v0, 80))));
L_10b28d4:;
    { if (xo) { xo[0] = a2; xo[1] = a3; xo[2] = a1_sx64(t0); xo[3] = a1_sx64(t1); } return v0; }
    __builtin_unreachable();
}

// GetMotionFrameDirectResource (0x10b62f0): typed by tools/typed_lift2.py (round A3; loop leaf)
inline a1::RetVF GetMotionFrameDirectResource(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t a0, uint32_t a1, uint64_t a2, float f12, float f13)
{
    float f0 = 0.0f;
    float f1 = 0.0f;
    float f2 = 0.0f;
    uint64_t v0 = 0;
    uint32_t v1 = 0;
    uint32_t fcr31 = 0;
    f12 = a1f::mul(f13, f12);
    f1 = a1_flt(A1_R32(a0));
    (v0 = (uint64_t)((int64_t)(int32_t)((int16_t)A1_R16B((uint32_t)a2, 4))));
    (a2 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v0, 4294967295))));
    f1 = a1f::add(f1, f12);
    { uint32_t bits = (uint32_t)v0; std::memcpy(&f2, &bits, sizeof(bits)); }
    { int32_t tmp; std::memcpy(&tmp, &f2, sizeof(tmp)); f2 = ((float)(int32_t)(tmp)); }
    A1_W32(a0, a1_bits(f1));
    f0 = a1_flt(A1_R32(a1));
    f0 = a1f::add(f0, f12);
    A1_W32(a1, a1_bits(f0));
    f0 = a1_flt(A1_R32(a0));
    fcr31 = (a1f::le(f2, f0)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
    { const bool c = (fcr31 & 0x800000u) != 0u; v1 = 1u; if (c) goto L_10b6334; }
    v1 = 0u;
L_10b6334:;
    if (v1 == 0) goto L_10b638c;
    if ((int64_t)v0 == 0) { A1_W32(a0, 0u); goto L_10b6388; }
    if ((int32_t)v0 >= 0) { f1 = ((float)(f0)); goto L_10b6358; }
    A1_W32(a0, 0u);
    goto L_10b6388;
L_10b6358:;
    fcr31 = (a1f::lt(f2, f1)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
    { const bool c = (fcr31 & 0x800000u) == 0u; (a2 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 1)))); if (c) goto L_10b638c; }
L_10b6368:;
    f0 = a1f::sub(f1, f2);
    fcr31 = (a1f::lt(f2, f0)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
    { const bool c = (fcr31 & 0x800000u) != 0u; f1 = ((float)(f0)); if (c) { A3_BACK(); goto L_10b6368; } }
    A1_W32(a0, a1_bits(f0));
L_10b6388:;
    (a2 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 1))));
L_10b638c:;
    { (v0 = (uint64_t)((uint64_t)a2 + (uint64_t)0ULL)); return a1::RetVF{(uint64_t)v0, f0}; }
    __builtin_unreachable();
}

// GetMotionOptionNodeParentId (0x10b63c0): typed by tools/typed_lift2.py (round A3; leaf)
inline uint32_t GetMotionOptionNodeParentId(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t a0, uint32_t a1)
{
    uint32_t v0 = 0;
    uint32_t v1 = 0;
    v1 = A1_R32B(a0, 36);
    a1 = a1 << 6;
    v1 = v1 + a1;
    { v0 = A1_R32B(v1, 48); return v0; }
    __builtin_unreachable();
}

// InitMotionBoolInfo (0x10b7618): typed by tools/typed_lift2.py (round A3; leaf, extra outputs xo[0]=a3)
inline uint32_t InitMotionBoolInfo(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t a0, uint64_t *xo = nullptr)
{
    uint32_t v0 = 0;
    uint32_t v1 = 0;
    uint32_t a1 = 0;
    uint32_t a2 = 0;
    uint32_t a3 = 0;
    v0 = 0x1190000u;
    a3 = v0 - 0x3B30u;
    v1 = (uint32_t)(int8_t)A1_R8(a3);
    a1 = (uint32_t)(int8_t)A1_R8B(a3, 1);
    a2 = (uint32_t)(int8_t)A1_R8B(a3, 2);
    A1_W8(a0, (uint8_t)v1);
    A1_W8B(a0, 1, (uint8_t)a1);
    A1_W8B(a0, 2, (uint8_t)a2);
    { if (xo) { xo[0] = a1_sx64(a3); } return v0; }
    __builtin_unreachable();
}

// ResetCharacterClassIKInfo (0x10c1158): typed by tools/typed_lift2.py (round A3; loop leaf, extra outputs xo[0]=f0)
inline uint32_t ResetCharacterClassIKInfo(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t a0, uint64_t *xo = nullptr)
{
    float f0 = 0.0f;
    float f1 = 0.0f;
    uint32_t at = 0;
    uint32_t v0 = 0;
    uint32_t v1 = 0;
    bool a3xw0 = false;
    v0 = A1_R32B(a0, 856);
    if (v0 == 0) goto L_10c11b0;
    v1 = A1_R32B(v0, 32);
    v0 = 0xFFFFFFFFu;
    v1 = v1 - 1u;
    { const bool c = v1 == v0; a0 = A1_R32B(a0, 796); if (c) goto L_10c11b0; }
    at = 0x3F800000u;
    f1 = a1_flt(at);
    at = 0x42C80000u;
    f0 = a1_flt(at); a3xw0 = true;
L_10c1188:;
    v1 = v1 - 1u;
    A1_W8B(a0, 196, (uint8_t)0u);
    A1_W32B(a0, 176, 0u);
    A1_W32B(a0, 184, a1_bits(f1));
    A1_W32B(a0, 180, 0u);
    A1_W8B(a0, 197, (uint8_t)0u);
    A1_W32B(a0, 188, a1_bits(f0));
    A1_W32B(a0, 192, 0u);
    { const bool c = v1 != v0; a0 = a0 + 0xD0u; if (c) { A3_BACK(); goto L_10c1188; } }
L_10c11b0:;
    { if (xo) { if (a3xw0) xo[0] = a1_bits(f0); } return v0; }
    __builtin_unreachable();
}

// GetCharacterClassCurAnimationCurFrame (0x10c1a80): typed by tools/typed_lift2.py (round A3; leaf, extra outputs xo[0]=a0)
inline float GetCharacterClassCurAnimationCurFrame(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t a0, uint64_t *xo = nullptr)
{
    float f0 = 0.0f;
    float f1 = 0.0f;
    a0 = a0 + 0x1C0u;
    f1 = a1_flt(A1_R32B(a0, 44));
    f0 = a1_flt(A1_R32B(a0, 8));
    { f0 = a1f::mul(f0, f1); if (xo) { xo[0] = a1_sx64(a0); } return f0; }
    __builtin_unreachable();
}

// SetCharacterObjControlBypassFunc (0x10c1d38): typed by tools/typed_lift2.py (round A3; leaf, extra outputs xo[0]=a2)
inline uint32_t SetCharacterObjControlBypassFunc(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint64_t a0, uint32_t a1, uint64_t *xo = nullptr)
{
    uint32_t v0 = 0;
    uint32_t v1 = 0;
    uint32_t a2 = 0;
    (v1 = (uint32_t)((uint8_t)A1_R8B((uint32_t)a0, 8)));
    (a2 = (uint32_t)((int32_t)((int32_t)ADD32((uint32_t)a0, 144))));
    v0 = 9u;
    { const bool c = v1 == v0; (a0 = (uint64_t)((uint64_t)a1_sx64(a2) + (uint64_t)0ULL)); if (c) goto L_10c1d5c; }
    v0 = 0x16u;
    (a0 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL));
    v0 = v1 ^ v0;
    if (a1_sx64(v0) == 0) a0 = (uint64_t)(a1_sx64(a2));;
L_10c1d5c:;
    { A1_W32B((uint32_t)a0, 2508, a1); if (xo) { xo[0] = a1_sx64(a2); } return v0; }
    __builtin_unreachable();
}

// SetCharacterObjParentPlaneDiscontinuousJudgeFunc (0x10c2910): typed by tools/typed_lift2.py (round A3; leaf, extra outputs xo[0]=a2)
inline uint32_t SetCharacterObjParentPlaneDiscontinuousJudgeFunc(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint64_t a0, uint32_t a1, uint64_t *xo = nullptr)
{
    uint32_t v0 = 0;
    uint32_t v1 = 0;
    uint32_t a2 = 0;
    (v1 = (uint32_t)((uint8_t)A1_R8B((uint32_t)a0, 8)));
    (a2 = (uint32_t)((int32_t)((int32_t)ADD32((uint32_t)a0, 144))));
    v0 = 9u;
    { const bool c = v1 == v0; (a0 = (uint64_t)((uint64_t)a1_sx64(a2) + (uint64_t)0ULL)); if (c) goto L_10c2934; }
    v0 = 0x16u;
    (a0 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL));
    v0 = v1 ^ v0;
    if (a1_sx64(v0) == 0) a0 = (uint64_t)(a1_sx64(a2));;
L_10c2934:;
    { A1_W32B((uint32_t)a0, 3120, a1); if (xo) { xo[0] = a1_sx64(a2); } return v0; }
    __builtin_unreachable();
}

// SetCharacterObjParentPointXZFitGpChangeJudgeFunc (0x10c2940): typed by tools/typed_lift2.py (round A3; leaf, extra outputs xo[0]=a3)
inline uint32_t SetCharacterObjParentPointXZFitGpChangeJudgeFunc(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint64_t a0, uint32_t a1, uint32_t a2, uint64_t *xo = nullptr)
{
    uint32_t v0 = 0;
    uint32_t v1 = 0;
    uint32_t a3 = 0;
    (v1 = (uint32_t)((uint8_t)A1_R8B((uint32_t)a0, 8)));
    (a3 = (uint32_t)((int32_t)((int32_t)ADD32((uint32_t)a0, 144))));
    v0 = 9u;
    { const bool c = v1 == v0; (a0 = (uint64_t)((uint64_t)a1_sx64(a3) + (uint64_t)0ULL)); if (c) goto L_10c2964; }
    v0 = 0x16u;
    (a0 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL));
    v0 = v1 ^ v0;
    if (a1_sx64(v0) == 0) a0 = (uint64_t)(a1_sx64(a3));;
L_10c2964:;
    A1_W8B((uint32_t)a0, 2704, (uint8_t)a2);
    { A1_W32B((uint32_t)a0, 2700, a1); if (xo) { xo[0] = a1_sx64(a3); } return v0; }
    __builtin_unreachable();
}

// manager_sub_010C9888 (0x10c9888): typed by tools/typed_lift2.py (round A3; leaf, extra outputs xo[0]=f0)
inline uint64_t manager_sub_010C9888(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t a0, uint32_t a1, uint64_t *xo = nullptr)
{
    float f0 = 0.0f;
    float f1 = 0.0f;
    float f2 = 0.0f;
    uint32_t at = 0;
    uint64_t v0 = 0;
    uint32_t v1 = 0;
    uint32_t a2 = 0;
    uint64_t a3 = 0;
    uint64_t t0 = 0;
    uint64_t t1 = 0;
    uint32_t fcr31 = 0;
    bool a3xw0 = false;
    v1 = A1_R32(a1);
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 4))));
    (t1 = (uint64_t)(A1_R64B(a1, 24)));
    (a3 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL));
    (t0 = (uint64_t)(A1_R64B(a1, 32)));
    a2 = a0 + 0x10u;
    { uint32_t bits = A1_R32B(a1, 12); float f; std::memcpy(&f, &bits, sizeof(f)); f2 = f; }
    f1 = a1_flt(A1_R32B(a1, 4));
    { const bool c = (int64_t)(int32_t)v1 == (int64_t)v0; a1 = A1_R32B(a1, 40); if (c) goto L_10c98e0; }
    (v0 = (uint64_t)(((int64_t)(int64_t)a1_sx64(v1) < (int64_t)(int32_t)5) ? 1 : 0));
    { const bool c = (int64_t)v0 == 0; (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 81)))); if (c) goto L_10c98d0; }
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 2))));
    { const bool c = (int64_t)(int32_t)v1 == (int64_t)v0; (v0 = (uint64_t)(((uint64_t)RC_U64X(a3) < (uint64_t)(int64_t)(int32_t)1) ? 1 : 0)); if (c) goto L_10c9918; }
    goto L_10c99f0;
L_10c98d0:;
    { const bool c = (int64_t)(int32_t)v1 == (int64_t)v0; (v0 = (uint64_t)(((uint64_t)RC_U64X(a3) < (uint64_t)(int64_t)(int32_t)1) ? 1 : 0)); if (c) goto L_10c9988; }
    goto L_10c99f0;
L_10c98e0:;
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B(a0, 0))));
    if ((int64_t)v0 == 0) { (v0 = (uint64_t)(((uint64_t)RC_U64X(a3) < (uint64_t)(int64_t)(int32_t)1) ? 1 : 0)); goto L_10c99f0; }
    (v0 = (uint64_t)(A1_R64B(a0, 8)));
    { const bool c = (int32_t)t0 >= 0; (v0 = (uint64_t)(v0 & t1)); if (c) goto L_10c9908; }
    if ((int64_t)v0 != 0) { (a3 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 1)))); goto L_10c99ec; }
    (v0 = (uint64_t)(((uint64_t)RC_U64X(a3) < (uint64_t)(int64_t)(int32_t)1) ? 1 : 0));
    goto L_10c99f0;
L_10c9908:;
    if ((int64_t)v0 == (int64_t)t0) { (a3 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 1)))); goto L_10c99ec; }
    (v0 = (uint64_t)(((uint64_t)RC_U64X(a3) < (uint64_t)(int64_t)(int32_t)1) ? 1 : 0));
    goto L_10c99f0;
L_10c9918:;
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B(a0, 0))));
    if ((int64_t)v0 == 0) { (v0 = (uint64_t)(((uint64_t)RC_U64X(a3) < (uint64_t)(int64_t)(int32_t)1) ? 1 : 0)); goto L_10c99f0; }
    at = 0xC3340000u;
    f0 = a1_flt(at); a3xw0 = true;
    fcr31 = (a1f::lt(f1, f0)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
    { const bool c = (fcr31 & 0x800000u) != 0u; v1 = 1u; if (c) goto L_10c9950; }
    f0 = a1_flt(A1_R32B(a0, 4)); a3xw0 = true;
    fcr31 = (a1f::le(f1, f0)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
    if ((fcr31 & 0x800000u) == 0u) { v1 = 0u; goto L_10c9948; }
L_10c9948:;
    at = 0xC3340000u;
    f0 = a1_flt(at); a3xw0 = true;
L_10c9950:;
    fcr31 = (a1f::lt(f2, f0)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
    { const bool c = (fcr31 & 0x800000u) != 0u; (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 1)))); if (c) goto L_10c996c; }
    f0 = a1_flt(A1_R32B(a0, 4)); a3xw0 = true;
    fcr31 = (a1f::le(f0, f2)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
    if ((fcr31 & 0x800000u) == 0u) { (v0 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL)); goto L_10c996c; }
L_10c996c:;
    { const bool c = v1 == 0; (a3 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL)); if (c) goto L_10c99ec; }
    if ((int64_t)v0 != 0) { (a3 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 1)))); goto L_10c99ec; }
    (v0 = (uint64_t)(((uint64_t)RC_U64X(a3) < (uint64_t)(int64_t)(int32_t)1) ? 1 : 0));
    goto L_10c99f0;
L_10c9988:;
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B(a2, 0))));
    if ((int64_t)v0 == 0) { (v0 = (uint64_t)(((uint64_t)a3 < (uint64_t)(int64_t)(int32_t)1) ? 1 : 0)); goto L_10c99f0; }
    at = 0xC61C0000u;
    (at = (uint32_t)(a1_sx64(at) | (uint64_t)(uint16_t)16384));
    f0 = a1_flt(at); a3xw0 = true;
    fcr31 = (a1f::lt(f1, f0)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
    { const bool c = (fcr31 & 0x800000u) != 0u; v1 = 1u; if (c) goto L_10c99c8; }
    f0 = a1_flt(A1_R32B(a2, 4)); a3xw0 = true;
    fcr31 = (a1f::le(f1, f0)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
    if ((fcr31 & 0x800000u) == 0u) { v1 = 0u; goto L_10c99bc; }
L_10c99bc:;
    at = 0xC61C0000u;
    (at = (uint32_t)(a1_sx64(at) | (uint64_t)(uint16_t)16384));
    f0 = a1_flt(at); a3xw0 = true;
L_10c99c8:;
    fcr31 = (a1f::lt(f2, f0)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
    { const bool c = (fcr31 & 0x800000u) != 0u; (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 1)))); if (c) goto L_10c99e4; }
    f0 = a1_flt(A1_R32B(a2, 4)); a3xw0 = true;
    fcr31 = (a1f::le(f0, f2)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
    if ((fcr31 & 0x800000u) == 0u) { (v0 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL)); goto L_10c99e4; }
L_10c99e4:;
    (a3 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL));
    if (a1_sx64(v1) != 0) a3 = (uint64_t)(v0);;
L_10c99ec:;
    (v0 = (uint64_t)(((uint64_t)a3 < (uint64_t)(int64_t)(int32_t)1) ? 1 : 0));
L_10c99f0:;
    { if (a1_sx64(a1) != 0) v0 = (uint64_t)(a3);; if (xo) { if (a3xw0) xo[0] = a1_bits(f0); } return v0; }
    __builtin_unreachable();
}

// InitScriptControlButtonCallBack (0x10cc4f0): typed by tools/typed_lift2.py (round A3; leaf, extra outputs xo[0]=a1 xo[1]=a2 xo[2]=a3)
inline uint64_t InitScriptControlButtonCallBack(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint64_t a0, uint64_t *xo = nullptr)
{
    uint64_t v0 = 0;
    uint64_t v1 = 0;
    uint64_t a1 = 0;
    uint64_t a2 = 0;
    uint64_t a3 = 0;
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)281 << 16))));
    (a3 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v0, 4294953416))));
    { uint32_t addr = ADD32((uint32_t)a3, 7); uint32_t aligned_addr = addr & ~7u; uint32_t offset = addr & 7u; uint64_t mem = A1_R64(aligned_addr); uint32_t shift = (7u - offset) << 3; uint64_t keepMask = (shift == 0) ? 0ull : ((1ull << shift) - 1ull); (v1 = (uint64_t)((v1 & keepMask) | (mem << shift))); }
    { uint32_t addr = ADD32((uint32_t)a3, 0); uint32_t aligned_addr = addr & ~7u; uint32_t offset = addr & 7u; uint64_t mem = A1_R64(aligned_addr); uint32_t shift = offset << 3; uint64_t keepMask = (offset == 0) ? 0ull : (0xFFFFFFFFFFFFFFFFull << ((8u - offset) << 3)); (v1 = (uint64_t)((v1 & keepMask) | (mem >> shift))); }
    { uint32_t addr = ADD32((uint32_t)a3, 15); uint32_t aligned_addr = addr & ~7u; uint32_t offset = addr & 7u; uint64_t mem = A1_R64(aligned_addr); uint32_t shift = (7u - offset) << 3; uint64_t keepMask = (shift == 0) ? 0ull : ((1ull << shift) - 1ull); (a1 = (uint64_t)((a1 & keepMask) | (mem << shift))); }
    { uint32_t addr = ADD32((uint32_t)a3, 8); uint32_t aligned_addr = addr & ~7u; uint32_t offset = addr & 7u; uint64_t mem = A1_R64(aligned_addr); uint32_t shift = offset << 3; uint64_t keepMask = (offset == 0) ? 0ull : (0xFFFFFFFFFFFFFFFFull << ((8u - offset) << 3)); (a1 = (uint64_t)((a1 & keepMask) | (mem >> shift))); }
    { uint32_t addr = ADD32((uint32_t)a3, 23); uint32_t aligned_addr = addr & ~7u; uint32_t offset = addr & 7u; uint64_t mem = A1_R64(aligned_addr); uint32_t shift = (7u - offset) << 3; uint64_t keepMask = (shift == 0) ? 0ull : ((1ull << shift) - 1ull); (a2 = (uint64_t)((a2 & keepMask) | (mem << shift))); }
    { uint32_t addr = ADD32((uint32_t)a3, 16); uint32_t aligned_addr = addr & ~7u; uint32_t offset = addr & 7u; uint64_t mem = A1_R64(aligned_addr); uint32_t shift = offset << 3; uint64_t keepMask = (offset == 0) ? 0ull : (0xFFFFFFFFFFFFFFFFull << ((8u - offset) << 3)); (a2 = (uint64_t)((a2 & keepMask) | (mem >> shift))); }
    { uint32_t addr = ADD32((uint32_t)a0, 7); uint32_t aligned_addr = addr & ~7u; uint32_t offset = addr & 7u; uint32_t shift = (7u - offset) << 3; uint64_t mask = 0xFFFFFFFFFFFFFFFFull >> shift; uint64_t old_data = A1_R64(aligned_addr); uint64_t val = v1; uint64_t new_data = (old_data & ~mask) | ((val >> shift) & mask); A1_W64(aligned_addr, new_data); }
    { uint32_t addr = ADD32((uint32_t)a0, 0); uint32_t aligned_addr = addr & ~7u; uint32_t offset = addr & 7u; uint32_t shift = offset << 3; uint64_t mask = 0xFFFFFFFFFFFFFFFFull << shift; uint64_t old_data = A1_R64(aligned_addr); uint64_t val = v1; uint64_t new_data = (old_data & ~mask) | ((val << shift) & mask); A1_W64(aligned_addr, new_data); }
    { uint32_t addr = ADD32((uint32_t)a0, 15); uint32_t aligned_addr = addr & ~7u; uint32_t offset = addr & 7u; uint32_t shift = (7u - offset) << 3; uint64_t mask = 0xFFFFFFFFFFFFFFFFull >> shift; uint64_t old_data = A1_R64(aligned_addr); uint64_t val = a1; uint64_t new_data = (old_data & ~mask) | ((val >> shift) & mask); A1_W64(aligned_addr, new_data); }
    { uint32_t addr = ADD32((uint32_t)a0, 8); uint32_t aligned_addr = addr & ~7u; uint32_t offset = addr & 7u; uint32_t shift = offset << 3; uint64_t mask = 0xFFFFFFFFFFFFFFFFull << shift; uint64_t old_data = A1_R64(aligned_addr); uint64_t val = a1; uint64_t new_data = (old_data & ~mask) | ((val << shift) & mask); A1_W64(aligned_addr, new_data); }
    { uint32_t addr = ADD32((uint32_t)a0, 23); uint32_t aligned_addr = addr & ~7u; uint32_t offset = addr & 7u; uint32_t shift = (7u - offset) << 3; uint64_t mask = 0xFFFFFFFFFFFFFFFFull >> shift; uint64_t old_data = A1_R64(aligned_addr); uint64_t val = a2; uint64_t new_data = (old_data & ~mask) | ((val >> shift) & mask); A1_W64(aligned_addr, new_data); }
    { uint32_t addr = ADD32((uint32_t)a0, 16); uint32_t aligned_addr = addr & ~7u; uint32_t offset = addr & 7u; uint32_t shift = offset << 3; uint64_t mask = 0xFFFFFFFFFFFFFFFFull << shift; uint64_t old_data = A1_R64(aligned_addr); uint64_t val = a2; uint64_t new_data = (old_data & ~mask) | ((val << shift) & mask); A1_W64(aligned_addr, new_data); }
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)a3, 24))));
    A1_W32B((uint32_t)a0, 24, (uint32_t)v1);
    { if (xo) { xo[0] = a1; xo[1] = a2; xo[2] = a3; } return v0; }
    __builtin_unreachable();
}

// manager_sub_010CD400 (0x10cd400): typed by tools/typed_lift2.py (round A3; leaf, extra outputs xo[0]=f0 xo[1]=v0 xo[2]=a1)
inline void manager_sub_010CD400(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t a0, float f12, uint64_t *xo = nullptr)
{
    float f0 = 0.0f;
    float f1 = 0.0f;
    float f2 = 0.0f;
    float f3 = 0.0f;
    float f4 = 0.0f;
    float f5 = 0.0f;
    uint32_t v0 = 0;
    uint32_t v1 = 0;
    uint32_t a1 = 0;
    uint32_t fcr31 = 0;
    bool a3xw0 = false;
    bool a3xw1 = false;
    bool a3xw2 = false;
    v1 = A1_R32B(a0, 3216);
    if (v1 == 0) goto L_10cd510;
    v0 = (uint32_t)(int8_t)A1_R8B(a0, 1506); a3xw1 = true;
    { const bool c = v0 == 0; v0 = 0xFFFFFFFFu; a3xw1 = true; if (c) goto L_10cd50c; }
    a1 = a0 + 0x1C0u; a3xw2 = true;
    f4 = a1_flt(A1_R32B(a0, 1480));
    { const bool c = v1 == v0; f5 = a1_flt(A1_R32B(a0, 1476)); if (c) goto L_10cd490; }
    { const bool c = (int32_t)v1 >= 0; v0 = 1u; a3xw1 = true; if (c) goto L_10cd448; }
    v0 = 0xFFFFFFFEu; a3xw1 = true;
    if (v1 == v0) { f0 = a1f::sub(f5, f4); a3xw0 = true; goto L_10cd4a0; }
    f0 = a1_flt(A1_R32B(a0, 1496)); a3xw0 = true;
    goto L_10cd4d8;
L_10cd448:;
    if (v1 != v0) { f0 = a1_flt(A1_R32B(a0, 1496)); a3xw0 = true; goto L_10cd4d8; }
    f0 = a1f::sub(f5, f4); a3xw0 = true;
    f2 = a1_flt(A1_R32B(a0, 1484));
    f3 = a1_flt(A1_R32B(a0, 1508));
    f1 = a1_flt(A1_R32B(a1, 28));
    f0 = a1f::mul(f0, f12); a3xw0 = true;
    f0 = a1f::div(fcr31, f0, f2); a3xw0 = true;
    f0 = a1f::mul(f0, f3); a3xw0 = true;
    f1 = a1f::add(f1, f0);
    fcr31 = (a1f::lt(f5, f1)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
    { const bool c = (fcr31 & 0x800000u) == 0u; A1_W32B(a1, 28, a1_bits(f1)); if (c) goto L_10cd4d4; }
    A1_W32B(a1, 28, a1_bits(f5));
    goto L_10cd4d4;
L_10cd490:;
    f0 = a1f::sub(f5, f4); a3xw0 = true;
    f2 = a1_flt(A1_R32B(a0, 1488));
    goto L_10cd4a4;
L_10cd4a0:;
    f2 = a1_flt(A1_R32B(a0, 1492));
L_10cd4a4:;
    f3 = a1_flt(A1_R32B(a0, 1508));
    f1 = a1_flt(A1_R32B(a1, 28));
    f0 = a1f::mul(f0, f12); a3xw0 = true;
    f0 = a1f::div(fcr31, f0, f2); a3xw0 = true;
    f0 = a1f::mul(f0, f3); a3xw0 = true;
    f1 = a1f::sub(f1, f0);
    fcr31 = (a1f::lt(f1, f4)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
    { const bool c = (fcr31 & 0x800000u) == 0u; A1_W32B(a1, 28, a1_bits(f1)); if (c) goto L_10cd4d4; }
    A1_W32B(a1, 28, a1_bits(f4));
L_10cd4d4:;
    f0 = a1_flt(A1_R32B(a0, 1496)); a3xw0 = true;
L_10cd4d8:;
    f1 = a1_flt(A1_R32B(a1, 28));
    fcr31 = (a1f::le(f1, f0)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
    if ((fcr31 & 0x800000u) == 0u) { f0 = a1_flt(A1_R32B(a0, 1500)); a3xw0 = true; goto L_10cd4f8; }
    v0 = 1u; a3xw1 = true;
    A1_W8B(a0, 1504, (uint8_t)v0);
    f1 = a1_flt(A1_R32B(a1, 28));
    f0 = a1_flt(A1_R32B(a0, 1500)); a3xw0 = true;
L_10cd4f8:;
    fcr31 = (a1f::le(f0, f1)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
    if ((fcr31 & 0x800000u) == 0u) { A1_W32B(a0, 3216, 0u); goto L_10cd510; }
    v0 = 1u; a3xw1 = true;
    A1_W8B(a0, 1505, (uint8_t)v0);
L_10cd50c:;
    A1_W32B(a0, 3216, 0u);
L_10cd510:;
    { if (xo) { if (a3xw0) xo[0] = a1_bits(f0); if (a3xw1) xo[1] = a1_sx64(v0); if (a3xw2) xo[2] = a1_sx64(a1); } return; }
    __builtin_unreachable();
}

// RequestCharacterClassMotionChangeWithParam (0x10d2b18): typed by tools/typed_lift2.py (round A3; leaf, extra outputs xo[0]=a1)
inline uint32_t RequestCharacterClassMotionChangeWithParam(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t a0, uint32_t a1, uint32_t a2, float f12, uint64_t *xo = nullptr)
{
    uint32_t v0 = 0;
    uint32_t v1 = 0;
    v1 = a0 + 0xC40u;
    A1_W32B(a0, 3140, a1);
    A1_W32B(a0, 3144, a2);
    v0 = (uint32_t)(int8_t)A1_R8(v1);
    { const bool c = (int32_t)v0 > 0; a1 = A1_R8(v1); if (c) goto L_10d2b38; }
    v0 = a1 + 1u;
    A1_W8(v1, (uint8_t)v0);
L_10d2b38:;
    v0 = 1u;
    A1_W32B(a0, 3152, a1_bits(f12));
    { A1_W8B(a0, 3148, (uint8_t)v0); if (xo) { xo[0] = a1_sx64(a1); } return v0; }
    __builtin_unreachable();
}

// charrootdyn_getLimitMode (0x10de398): typed by tools/typed_lift2.py (round A3; leaf, extra outputs xo[0]=f0)
inline uint32_t charrootdyn_getLimitMode(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t a0, uint32_t a1, uint64_t *xo = nullptr)
{
    float f0 = 0.0f;
    uint32_t v0 = 0;
    v0 = (uint32_t)(int8_t)A1_R8B(a1, 1325);
    { const bool c = v0 == 0; v0 = 1u; if (c) goto L_10de3c0; }
    A1_W32(a0, v0);
    f0 = a1_flt(A1_R32B(a1, 1024));
    A1_W32B(a0, 4, a1_bits(f0));
    v0 = A1_R32B(a1, 3120);
    { A1_W32B(a0, 8, v0); if (xo) { xo[0] = a1_bits(f0); } return v0; }
L_10de3c0:;
    v0 = 3u;
    A1_W32B(a0, 8, 0u);
    A1_W32B(a0, 4, 0u);
    { A1_W32(a0, v0); return v0; }
    __builtin_unreachable();
}

// resetExpressionLocalHeightDiff (0x10e2170): typed by tools/typed_lift2.py (round A3; loop leaf)
inline uint32_t resetExpressionLocalHeightDiff(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t a0)
{
    uint32_t v0 = 0;
    uint32_t v1 = 0;
    uint64_t a1 = 0;
    v0 = A1_R32B(a0, 856);
    (a1 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL));
    { const bool c = v0 == 0; v1 = A1_R32B(a0, 828); if (c) goto L_10e2184; }
    (a1 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B(v0, 32))));
L_10e2184:;
    (a1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)a1, 4294967295))));
    v0 = 0xFFFFFFFFu;
    if ((int64_t)a1 == (int64_t)(int32_t)v0) goto L_10e21b4;
L_10e2198:;
    (a1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)a1, 4294967295))));
    A1_W32(v1, 0u);
    { const bool c = (int64_t)a1 != (int64_t)(int32_t)v0; v1 = v1 + 4u; if (c) { A3_BACK(); goto L_10e2198; } }
L_10e21b4:;
    { return v0; }
    __builtin_unreachable();
}

// GetDemoOrientNext (0x10ebb90): typed by tools/typed_lift2.py (round A3; leaf, extra outputs xo[0]=a1 xo[1]=a2 xo[2]=a3 xo[3]=t0 xo[4]=t1)
inline uint32_t GetDemoOrientNext(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint64_t a0, uint64_t *xo = nullptr)
{
    uint32_t v0 = 0;
    uint32_t v1 = 0;
    uint32_t a1 = 0;
    uint32_t a2 = 0;
    uint64_t a3 = 0;
    uint64_t t0 = 0;
    uint32_t t1 = 0;
    uint64_t hi = 0;
    uint64_t lo = 0;
    (a3 = (uint64_t)((uint64_t)a0 + (uint64_t)0ULL));
    v1 = 0x30000u;
    (a1 = (uint32_t)((int32_t)((int32_t)A1_R32B((uint32_t)a3, 16))));
    v1 = v1 | 0xFFFFu;
    v0 = A1_R32(a1);
    v0 = v0 & v1;
    { const bool c = v0 == 0; (t1 = (uint32_t)((int32_t)((int32_t)A1_R32B((uint32_t)a3, 20)))); if (c) goto L_10ebbc0; }
    v0 = a1 - v0;
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B(v0, 0))));
    goto L_10ebbc4;
L_10ebbc0:;
    (a0 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL));
L_10ebbc4:;
    { const bool c = (int64_t)a0 == 0; { const uint32_t _dynInsn = A1_R32(0x10EBBC8u); const int32_t _dynSimm = (int16_t)(_dynInsn & 0xFFFFu); const uint32_t _dynImm = _dynInsn & 0xFFFFu; (void)_dynSimm; (void)_dynImm; (v0 = (uint32_t)((int32_t)((int32_t)((uint32_t)_dynImm << 16)))); } if (c) goto L_10ebbe0; }
    { const uint32_t _dynInsn = A1_R32(0x10EBBCCu); const int32_t _dynSimm = (int16_t)(_dynInsn & 0xFFFFu); const uint32_t _dynImm = _dynInsn & 0xFFFFu; (void)_dynSimm; (void)_dynImm; (v1 = (uint32_t)((int32_t)((int32_t)A1_R32(ADD32(v0, _dynSimm))))); }
    if ((int64_t)a0 == (int64_t)(int32_t)v1) { (t0 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL)); goto L_10ebbe4; }
    (t0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)a0, 12))));
    goto L_10ebbe4;
L_10ebbe0:;
    (t0 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL));
L_10ebbe4:;
    a2 = A1_R32(a1);
    v0 = 0x30000u;
    v0 = v0 | 0xFFFFu;
    v0 = a2 & v0;
    { const bool c = v0 == 0; v0 = a1 - v0; if (c) goto L_10ebc08; }
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B(v0, 0))));
    goto L_10ebc0c;
L_10ebc08:;
    (a0 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL));
L_10ebc0c:;
    { const bool c = (int64_t)a0 == 0; { const uint32_t _dynInsn = A1_R32(0x10EBC10u); const int32_t _dynSimm = (int16_t)(_dynInsn & 0xFFFFu); const uint32_t _dynImm = _dynInsn & 0xFFFFu; (void)_dynSimm; (void)_dynImm; (v0 = (uint32_t)((int32_t)((int32_t)((uint32_t)_dynImm << 16)))); } if (c) goto L_10ebc40; }
    { const uint32_t _dynInsn = A1_R32(0x10EBC14u); const int32_t _dynSimm = (int16_t)(_dynInsn & 0xFFFFu); const uint32_t _dynImm = _dynInsn & 0xFFFFu; (void)_dynSimm; (void)_dynImm; (v1 = (uint32_t)((int32_t)((int32_t)A1_R32(ADD32(v0, _dynSimm))))); }
    { const bool c = (int64_t)a0 == (int64_t)(int32_t)v1; v0 = a2 >> 18; if (c) goto L_10ebc40; }
    (a1 = (uint32_t)((int32_t)((int32_t)A1_R32B((uint32_t)a0, 8))));
    v0 = v0 & 0xFFFu;
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)a0, 16))));
    v1 = A1_R32B(a1, 12);
    { int64_t result = (int64_t)(int32_t)v0 * (int64_t)(int32_t)v1; lo = (uint64_t)(int64_t)(int32_t)result; hi = (uint64_t)(int64_t)(int32_t)(result >> 32); (v0 = (uint32_t)((int32_t)((int32_t)result))); }
    (v0 = (uint32_t)((int32_t)((int32_t)ADD32((uint32_t)a0, v0))));
    goto L_10ebc44;
L_10ebc40:;
    v0 = 0u;
L_10ebc44:;
    v0 = t1 - v0;
    v1 = 0xAAAA0000u;
    v0 = (uint32_t)((int32_t)v0 >> 2);
    v1 = v1 | 0xAAABu;
    { int64_t result = (int64_t)(int32_t)v0 * (int64_t)(int32_t)v1; lo = (uint64_t)(int64_t)(int32_t)result; hi = (uint64_t)(int64_t)(int32_t)(result >> 32); (v0 = (uint32_t)((int32_t)((int32_t)result))); }
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)t0, 4294967295))));
    (v0 = (uint32_t)(((int64_t)(int64_t)a1_sx64(v0) < (int64_t)(int64_t)RC_U64X(a0)) ? 1 : 0));
    if (v0 == 0) goto L_10ebc78;
    v0 = t1 + 0xCu;
    { A1_W32B((uint32_t)a3, 20, v0); if (xo) { xo[0] = a1_sx64(a1); xo[1] = a1_sx64(a2); xo[2] = a3; xo[3] = t0; xo[4] = a1_sx64(t1); } return v0; }
L_10ebc78:;
    { v0 = 0u; if (xo) { xo[0] = a1_sx64(a1); xo[1] = a1_sx64(a2); xo[2] = a3; xo[3] = t0; xo[4] = a1_sx64(t1); } return v0; }
    __builtin_unreachable();
}

// UpdateFogCol (0x10ed440): typed by tools/typed_lift2.py (round A3; leaf, extra outputs xo[0]=v0)
inline float UpdateFogCol(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t a0, uint32_t a1, uint64_t *xo = nullptr)
{
    float f0 = 0.0f;
    float f1 = 0.0f;
    float f2 = 0.0f;
    uint32_t at = 0;
    uint32_t v0 = 0;
    uint32_t v1 = 0;
    uint32_t fcr31 = 0;
    bool a3xw0 = false;
    f0 = a1_flt(A1_R32(a1));
    at = 0x4F000000u;
    f1 = a1_flt(at);
    { int32_t tmp = a1f::cvtw(f0); std::memcpy(&f2, &tmp, sizeof(tmp)); }
    v1 = a1_bits(f2);
    fcr31 = (a1f::le(f1, f0)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
    if ((fcr31 & 0x800000u) == 0u) { A1_W8B(a0, 78, (uint8_t)v1); goto L_10ed480; }
    f0 = a1f::sub(f0, f1);
    v0 = 0x80000000u; a3xw0 = true;
    { int32_t tmp = a1f::cvtw(f0); std::memcpy(&f1, &tmp, sizeof(tmp)); }
    v1 = a1_bits(f1);
    v1 = v1 | v0;
    A1_W8B(a0, 78, (uint8_t)v1);
    at = 0x4F000000u;
    f1 = a1_flt(at);
L_10ed480:;
    f0 = a1_flt(A1_R32B(a1, 4));
    fcr31 = (a1f::le(f1, f0)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
    { int32_t tmp = a1f::cvtw(f0); std::memcpy(&f2, &tmp, sizeof(tmp)); }
    v1 = a1_bits(f2);
    if ((fcr31 & 0x800000u) == 0u) { A1_W8B(a0, 79, (uint8_t)v1); goto L_10ed4b0; }
    f0 = a1f::sub(f0, f1);
    v0 = 0x80000000u; a3xw0 = true;
    { int32_t tmp = a1f::cvtw(f0); std::memcpy(&f1, &tmp, sizeof(tmp)); }
    v1 = a1_bits(f1);
    v1 = v1 | v0;
    A1_W8B(a0, 79, (uint8_t)v1);
L_10ed4b0:;
    at = 0x4F000000u;
    f1 = a1_flt(at);
    f0 = a1_flt(A1_R32B(a1, 8));
    fcr31 = (a1f::le(f1, f0)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
    { int32_t tmp = a1f::cvtw(f0); std::memcpy(&f2, &tmp, sizeof(tmp)); }
    v1 = a1_bits(f2);
    if ((fcr31 & 0x800000u) == 0u) { A1_W8B(a0, 80, (uint8_t)v1); goto L_10ed4e8; }
    f0 = a1f::sub(f0, f1);
    v0 = 0x80000000u; a3xw0 = true;
    { int32_t tmp = a1f::cvtw(f0); std::memcpy(&f1, &tmp, sizeof(tmp)); }
    v1 = a1_bits(f1);
    v1 = v1 | v0;
    A1_W8B(a0, 80, (uint8_t)v1);
L_10ed4e8:;
    f0 = a1_flt(A1_R32B(a1, 16));
    A1_W32B(a0, 88, a1_bits(f0));
    f1 = a1_flt(A1_R32B(a1, 20));
    A1_W32B(a0, 92, a1_bits(f1));
    f0 = a1_flt(A1_R32B(a1, 24));
    A1_W32B(a0, 96, a1_bits(f0));
    f1 = a1_flt(A1_R32B(a1, 32));
    A1_W32B(a0, 100, a1_bits(f1));
    f0 = a1_flt(A1_R32B(a1, 36));
    A1_W32B(a0, 104, a1_bits(f0));
    f1 = a1_flt(A1_R32B(a1, 40));
    { A1_W32B(a0, 108, a1_bits(f1)); if (xo) { if (a3xw0) xo[0] = a1_sx64(v0); } return f0; }
    __builtin_unreachable();
}

// IsEnvelopeDistantLightType (0x10f8f90): typed by tools/typed_lift2.py (round A3; loop leaf)
inline uint32_t IsEnvelopeDistantLightType(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint64_t a0, uint32_t a1)
{
    uint32_t v0 = 0;
    uint32_t v1 = 0;
    uint64_t a2 = 0;
    uint32_t a3 = 0;
    uint64_t t0 = 0;
    a3 = A1_R32B(a1, 36);
    (t0 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL));
    (v0 = (uint32_t)((int32_t)((int8_t)A1_R8B((uint32_t)a0, 48))));
    { const bool c = v0 != a3; (a2 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL)); if (c) goto L_10f8fb0; }
    (t0 = (uint64_t)((uint64_t)a0 + (uint64_t)0ULL));
    goto L_10f8fdc;
L_10f8fb0:;
    (a2 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)a2, 1))));
L_10f8fb4:;
    (a1 = (uint32_t)((int32_t)((int32_t)SLL32((uint32_t)a2, 6))));
    (v0 = (uint32_t)(((int64_t)(int64_t)a2 < (int64_t)(int32_t)3) ? 1 : 0));
    { const bool c = v0 == 0; (v1 = (uint32_t)((int32_t)((int32_t)ADD32(a1, (uint32_t)a0)))); if (c) goto L_10f8fdc; }
    v0 = (uint32_t)(int8_t)A1_R8B(v1, 48);
    if (v0 != a3) { (a2 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)a2, 1)))); { A3_BACK(); goto L_10f8fb4; } }
    (t0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)a0, a1))));
L_10f8fdc:;
    (v0 = (uint32_t)((int32_t)((int32_t)A1_R32B((uint32_t)t0, 44))));
    v1 = 0x20000u;
    v0 = v0 & v1;
    { v0 = 0u < v0 ? 1u : 0u; return v0; }
    __builtin_unreachable();
}

// globalLightCalcLightNum (0x10f8ff0): typed by tools/typed_lift2.py (round A3; leaf, extra outputs xo[0]=a2)
inline uint32_t globalLightCalcLightNum(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t a0, uint64_t a1, uint64_t *xo = nullptr)
{
    uint32_t v0 = 0;
    uint32_t v1 = 0;
    uint32_t a2 = 0;
    v0 = 0x19u;
    { const bool c = (int64_t)a1 == (int64_t)(int32_t)v0; a2 = 4u; if (c) goto L_10f9010; }
    v0 = 0x1Au;
    { const bool c = (int64_t)a1 == (int64_t)(int32_t)v0; v0 = a2; if (c) goto L_10f9030; }
    goto L_10f90c0;
L_10f9010:;
    v1 = A1_R32B(a0, 272);
    a2 = 3u;
    v0 = A1_R32B(a0, 288);
    v1 = v1 | 4u;
    v0 = v0 + 1u;
    A1_W32B(a0, 272, v1);
    goto L_10f90b8;
L_10f9030:;
    v1 = A1_R32B(a0, 288);
    v0 = 1u;
    if (v1 == v0) { v0 = A1_R32B(a0, 272); goto L_10f9080; }
    v0 = (int32_t)v1 < 2 ? 1u : 0u;
    if (v0 == 0) { v0 = 2u; goto L_10f9060; }
    if (v1 == 0) { v0 = A1_R32B(a0, 272); goto L_10f9070; }
    v0 = 4u;
    goto L_10f90a4;
L_10f9060:;
    { const bool c = v1 == v0; v0 = 4u; if (c) goto L_10f9090; }
    goto L_10f90a4;
L_10f9070:;
    a2 = 0u;
    v0 = v0 | 8u;
    goto L_10f909c;
L_10f9080:;
    a2 = 1u;
    v0 = v0 | 0x10u;
    goto L_10f909c;
L_10f9090:;
    v0 = A1_R32B(a0, 272);
    a2 = 2u;
    v0 = v0 | 0x20u;
L_10f909c:;
    A1_W32B(a0, 272, v0);
    v0 = 4u;
L_10f90a4:;
    { const bool c = a2 == v0; v1 = v1 + 1u; if (c) goto L_10f90bc; }
    v0 = A1_R32B(a0, 292);
    A1_W32B(a0, 288, v1);
    v0 = v0 + 1u;
L_10f90b8:;
    A1_W32B(a0, 292, v0);
L_10f90bc:;
    v0 = a2;
L_10f90c0:;
    { if (xo) { xo[0] = a1_sx64(a2); } return v0; }
    __builtin_unreachable();
}

// GetImpactWorstWorkLightClass (0x10f92b0): typed by tools/typed_lift2.py (round A3; loop leaf)
inline uint32_t GetImpactWorstWorkLightClass(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t a0)
{
    float f0 = 0.0f;
    float f1 = 0.0f;
    uint32_t v0 = 0;
    uint32_t v1 = 0;
    uint32_t a1 = 0;
    uint32_t fcr31 = 0;
    v1 = A1_R32B(a0, 1328);
    v0 = 1u;
    v0 = (int32_t)v0 < (int32_t)v1 ? 1u : 0u;
    { const bool c = v0 == 0; a1 = A1_R32B(a0, 1332); if (c) goto L_10f92f4; }
    a0 = a0 + 0x538u;
    v0 = v1 - 1u;
    v1 = A1_R32(a0);
L_10f92d0:;
    f1 = a1_flt(A1_R32B(a1, 56));
    f0 = a1_flt(A1_R32B(v1, 56));
    fcr31 = (a1f::lt(f0, f1)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
    { const bool c = (fcr31 & 0x800000u) == 0u; a0 = a0 + 4u; if (c) goto L_10f92e8; }
    a1 = v1;
L_10f92e8:;
    v0 = v0 - 1u;
    if (v0 != 0) { v1 = A1_R32(a0); { A3_BACK(); goto L_10f92d0; } }
L_10f92f4:;
    { v0 = a1; return v0; }
    __builtin_unreachable();
}

// GetLwsOrientNodeNum (0x10fc1f0): typed by tools/typed_lift2.py (round A3; loop leaf, extra outputs xo[0]=a3 xo[1]=t0)
inline uint32_t GetLwsOrientNodeNum(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t a0, uint64_t *xo = nullptr)
{
    uint32_t v0 = 0;
    uint32_t v1 = 0;
    uint64_t a1 = 0;
    uint32_t a2 = 0;
    uint32_t a3 = 0;
    uint64_t t0 = 0;
    a3 = A1_R32B(a0, 20);
    (a1 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL));
    { const bool c = (int32_t)a3 <= 0; (t0 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL)); if (c) goto L_10fc244; }
    a2 = A1_R32B(a0, 16);
    v1 = 0xEu;
    v0 = A1_R32(a2);
    if (v0 == v1) { (t0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 1)))); goto L_10fc244; }
    (a1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)a1, 1))));
L_10fc218:;
    (v0 = (uint32_t)((int32_t)((int32_t)SLL32((uint32_t)a1, 1))));
    (v1 = (uint32_t)(((int64_t)(int64_t)a1 < (int64_t)(int64_t)a1_sx64(a3)) ? 1 : 0));
    { const bool c = v1 == 0; (v0 = (uint32_t)((int32_t)((int32_t)ADD32(v0, (uint32_t)a1)))); if (c) goto L_10fc244; }
    v0 = v0 << 2;
    a0 = 0xEu;
    v0 = a2 + v0;
    v1 = A1_R32(v0);
    if (v1 != a0) { (a1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)a1, 1)))); { A3_BACK(); goto L_10fc218; } }
    (t0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 1))));
L_10fc244:;
    { (v0 = (uint32_t)((int32_t)((int32_t)SUB32(a3, (uint32_t)t0)))); if (xo) { xo[0] = a1_sx64(a3); xo[1] = t0; } return v0; }
    __builtin_unreachable();
}

// manager_sub_010FC618 (0x10fc618): typed by tools/typed_lift2.py (round A3; loop leaf, extra outputs xo[0]=a3 xo[1]=t0 xo[2]=t1 xo[3]=t2 xo[4]=t3)
inline uint64_t manager_sub_010FC618(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint64_t a0, uint64_t *xo = nullptr)
{
    uint64_t v0 = 0;
    uint32_t v1 = 0;
    uint64_t a1 = 0;
    uint64_t a2 = 0;
    uint64_t a3 = 0;
    uint32_t t0 = 0;
    uint32_t t1 = 0;
    uint32_t t2 = 0;
    uint32_t t3 = 0;
    uint64_t hi = 0;
    uint64_t lo = 0;
    bool a3xw1 = false;
    bool a3xw2 = false;
    bool a3xw3 = false;
    bool a3xw4 = false;
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)3 << 16))));
    (a2 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)a0, 0))));
    (v0 = (uint64_t)(RC_U64X(v0) | (uint64_t)(uint16_t)65535));
    (v0 = (uint64_t)(RC_U64X(a2) & RC_U64X(v0)));
    { const bool c = (int64_t)v0 == 0; (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)SUB32((uint32_t)a0, (uint32_t)v0)))); if (c) goto L_10fc638; }
    (a1 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)v0, 0))));
    goto L_10fc63c;
L_10fc638:;
    (a1 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL));
L_10fc63c:;
    { const bool c = (int64_t)a1 == 0; { const uint32_t _dynInsn = A1_R32(0x10FC640u); const int32_t _dynSimm = (int16_t)(_dynInsn & 0xFFFFu); const uint32_t _dynImm = _dynInsn & 0xFFFFu; (void)_dynSimm; (void)_dynImm; (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)_dynImm << 16)))); } if (c) goto L_10fc658; }
    { const uint32_t _dynInsn = A1_R32(0x10FC644u); const int32_t _dynSimm = (int16_t)(_dynInsn & 0xFFFFu); const uint32_t _dynImm = _dynInsn & 0xFFFFu; (void)_dynSimm; (void)_dynImm; (v1 = (uint32_t)((int32_t)((int32_t)A1_R32(ADD32((uint32_t)v0, _dynSimm))))); }
    if ((int64_t)a1 == (int64_t)(int32_t)v1) { (a3 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL)); goto L_10fc65c; }
    (a3 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)a1, 12))));
    goto L_10fc65c;
L_10fc658:;
    (a3 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL));
L_10fc65c:;
    { const bool c = (int64_t)a3 == 0; (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)3 << 16)))); if (c) goto L_10fc7b8; }
    (v0 = (uint64_t)(RC_U64X(v0) | (uint64_t)(uint16_t)65535));
    (v0 = (uint64_t)(RC_U64X(a2) & RC_U64X(v0)));
    { const bool c = (int64_t)v0 == 0; (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)SUB32((uint32_t)a0, (uint32_t)v0)))); if (c) goto L_10fc680; }
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)v0, 0))));
    goto L_10fc684;
L_10fc680:;
    (a0 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL));
L_10fc684:;
    { const bool c = (int64_t)a0 == 0; { const uint32_t _dynInsn = A1_R32(0x10FC688u); const int32_t _dynSimm = (int16_t)(_dynInsn & 0xFFFFu); const uint32_t _dynImm = _dynInsn & 0xFFFFu; (void)_dynSimm; (void)_dynImm; (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)_dynImm << 16)))); } if (c) goto L_10fc6b8; }
    { const uint32_t _dynInsn = A1_R32(0x10FC68Cu); const int32_t _dynSimm = (int16_t)(_dynInsn & 0xFFFFu); const uint32_t _dynImm = _dynInsn & 0xFFFFu; (void)_dynSimm; (void)_dynImm; (v1 = (uint32_t)((int32_t)((int32_t)A1_R32(ADD32((uint32_t)v0, _dynSimm))))); }
    { const bool c = (int64_t)a0 == (int64_t)(int32_t)v1; (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)SRL32((uint32_t)a2, 18)))); if (c) goto L_10fc6b8; }
    (a1 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)a0, 8))));
    (v0 = (uint64_t)(RC_U64X(v0) & (uint64_t)(uint16_t)4095));
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)a0, 16))));
    (v1 = (uint32_t)((int32_t)((int32_t)A1_R32B((uint32_t)a1, 12))));
    { int64_t result = (int64_t)(int32_t)v0 * (int64_t)(int32_t)v1; lo = (uint64_t)(int64_t)(int32_t)result; hi = (uint64_t)(int64_t)(int32_t)(result >> 32); (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)result))); }
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)a0, (uint32_t)v0))));
    goto L_10fc6bc;
L_10fc6b8:;
    (v0 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL));
L_10fc6bc:;
    (a2 = (uint64_t)((uint64_t)RC_U64X(v0) + (uint64_t)0ULL));
    { const bool c = (int64_t)a3 == 0; (t0 = (uint32_t)((int32_t)((int32_t)ADD32((uint32_t)a3, 4294967295)))); a3xw1 = true; if (c) goto L_10fc7b8; }
    (a3 = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)3 << 16))));
    { const uint32_t _dynInsn = A1_R32(0x10FC6CCu); const int32_t _dynSimm = (int16_t)(_dynInsn & 0xFFFFu); const uint32_t _dynImm = _dynInsn & 0xFFFFu; (void)_dynSimm; (void)_dynImm; (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)_dynImm << 16)))); }
    { const uint32_t _dynInsn = A1_R32(0x10FC6D0u); const int32_t _dynSimm = (int16_t)(_dynInsn & 0xFFFFu); const uint32_t _dynImm = _dynInsn & 0xFFFFu; (void)_dynSimm; (void)_dynImm; (t1 = (uint32_t)((int32_t)((int32_t)ADD32((uint32_t)v0, _dynSimm)))); } a3xw2 = true;
    (a3 = (uint64_t)(RC_U64X(a3) | (uint64_t)(uint16_t)65535));
    t3 = 0xFFFFFFFFu; a3xw4 = true;
    t2 = 0xFFFFFFFFu; a3xw3 = true;
L_10fc6e0:;
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)a2, 4))));
    (v1 = (uint32_t)((int32_t)((int32_t)A1_R32B((uint32_t)a0, 0))));
    (v0 = (uint64_t)(a1_sx64(v1) & a3));
    { const bool c = (int64_t)v0 == 0; (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)SUB32((uint32_t)a0, (uint32_t)v0)))); if (c) goto L_10fc700; }
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)v0, 0))));
    goto L_10fc704;
L_10fc700:;
    (a0 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL));
L_10fc704:;
    { const bool c = (int64_t)a0 == 0; (v0 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL)); if (c) goto L_10fc73c; }
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B(t1, 0))));
    { const bool c = (int64_t)a0 == (int64_t)v0; v1 = v1 >> 18; if (c) goto L_10fc738; }
    (a1 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)a0, 8))));
    v1 = v1 & 0xFFFu;
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)a0, 16))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)a1, 12))));
    { int64_t result = (int64_t)(int32_t)v1 * (int64_t)(int32_t)v0; lo = (uint64_t)(int64_t)(int32_t)result; hi = (uint64_t)(int64_t)(int32_t)(result >> 32); (v1 = (uint32_t)((int32_t)((int32_t)result))); }
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)a0, v1))));
    goto L_10fc73c;
L_10fc738:;
    (v0 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL));
L_10fc73c:;
    (v0 = (uint64_t)(uint32_t)((uint16_t)A1_R16B((uint32_t)v0, 8)));
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)a2, 28))));
    A1_W16B((uint32_t)a2, 8, (uint16_t)(uint32_t)v0);
    (v1 = (uint32_t)((int32_t)((int32_t)A1_R32B((uint32_t)a0, 0))));
    (v0 = (uint64_t)(a1_sx64(v1) & a3));
    { const bool c = (int64_t)v0 == 0; (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)SUB32((uint32_t)a0, (uint32_t)v0)))); if (c) goto L_10fc760; }
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)v0, 0))));
    goto L_10fc764;
L_10fc760:;
    (a0 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL));
L_10fc764:;
    { const bool c = (int64_t)a0 == 0; (v0 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL)); if (c) goto L_10fc79c; }
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B(t1, 0))));
    { const bool c = (int64_t)a0 == (int64_t)v0; v1 = v1 >> 18; if (c) goto L_10fc798; }
    (a1 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)a0, 8))));
    v1 = v1 & 0xFFFu;
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)a0, 16))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)a1, 12))));
    { int64_t result = (int64_t)(int32_t)v1 * (int64_t)(int32_t)v0; lo = (uint64_t)(int64_t)(int32_t)result; hi = (uint64_t)(int64_t)(int32_t)(result >> 32); (v1 = (uint32_t)((int32_t)((int32_t)result))); }
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)a0, v1))));
    goto L_10fc79c;
L_10fc798:;
    (v0 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL));
L_10fc79c:;
    if ((int64_t)v0 == 0) { A1_W16B((uint32_t)a2, 32, (uint16_t)t3); goto L_10fc7ac; }
    (v0 = (uint64_t)(uint32_t)((uint16_t)A1_R16B((uint32_t)v0, 8)));
    A1_W16B((uint32_t)a2, 32, (uint16_t)(uint32_t)v0);
L_10fc7ac:;
    t0 = t0 - 1u; a3xw1 = true;
    { const bool c = t0 != t2; (a2 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)a2, 52)))); if (c) { A3_BACK(); goto L_10fc6e0; } }
L_10fc7b8:;
    { if (xo) { xo[0] = a3; if (a3xw1) xo[1] = a1_sx64(t0); if (a3xw2) xo[2] = a1_sx64(t1); if (a3xw3) xo[3] = a1_sx64(t2); if (a3xw4) xo[4] = a1_sx64(t3); } return v0; }
    __builtin_unreachable();
}

// initAnimationObjResource (0x1100af8): typed by tools/typed_lift2.py (round A3; leaf, extra outputs xo[0]=a2)
inline uint32_t initAnimationObjResource(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t a0, uint64_t *xo = nullptr)
{
    uint32_t v0 = 0;
    uint32_t v1 = 0;
    uint32_t a1 = 0;
    uint32_t a2 = 0;
    uint64_t hi = 0;
    uint64_t lo = 0;
    a2 = A1_R32(a0);
    v0 = 0x30000u;
    v0 = v0 | 0xFFFFu;
    v0 = a2 & v0;
    { const bool c = v0 == 0; a0 = a0 - v0; if (c) goto L_1100b18; }
    a0 = A1_R32(a0);
    goto L_1100b1c;
L_1100b18:;
    a0 = 0u;
L_1100b1c:;
    { const bool c = a0 == 0; { const uint32_t _dynInsn = A1_R32(0x1100B20u); const int32_t _dynSimm = (int16_t)(_dynInsn & 0xFFFFu); const uint32_t _dynImm = _dynInsn & 0xFFFFu; (void)_dynSimm; (void)_dynImm; (v0 = (uint32_t)((int32_t)((int32_t)((uint32_t)_dynImm << 16)))); } if (c) goto L_1100b50; }
    { const uint32_t _dynInsn = A1_R32(0x1100B24u); const int32_t _dynSimm = (int16_t)(_dynInsn & 0xFFFFu); const uint32_t _dynImm = _dynInsn & 0xFFFFu; (void)_dynSimm; (void)_dynImm; (v1 = (uint32_t)((int32_t)((int32_t)A1_R32(ADD32(v0, _dynSimm))))); }
    { const bool c = a0 == v1; v0 = a2 >> 18; if (c) goto L_1100b50; }
    a1 = A1_R32B(a0, 8);
    v0 = v0 & 0xFFFu;
    a0 = A1_R32B(a0, 16);
    v1 = A1_R32B(a1, 12);
    { int64_t result = (int64_t)(int32_t)v0 * (int64_t)(int32_t)v1; lo = (uint64_t)(int64_t)(int32_t)result; hi = (uint64_t)(int64_t)(int32_t)(result >> 32); (v0 = (uint32_t)((int32_t)((int32_t)result))); }
    a0 = a0 + v0;
    goto L_1100b54;
L_1100b50:;
    a0 = 0u;
L_1100b54:;
    { const bool c = a0 == 0; v0 = 0u; if (c) goto L_1100b60; }
    v0 = A1_R32B(a0, 8);
L_1100b60:;
    { if (xo) { xo[0] = a1_sx64(a2); } return v0; }
    __builtin_unreachable();
}

// InitBinarySize (0x1101650): typed by tools/typed_lift2.py (round A3; loop leaf)
inline uint32_t InitBinarySize(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt)
{
    uint32_t v0 = 0;
    uint32_t v1 = 0;
    v0 = 0x1210000u;
    v1 = 0xBu;
    v0 = v0 + 0x6D00u;
    v0 = v0 + 0x2Cu;
L_1101660:;
    v1 = v1 - 1u;
    A1_W32(v0, 0u);
    { const bool c = (int32_t)v1 >= 0; v0 = v0 - 4u; if (c) { A3_BACK(); goto L_1101660; } }
    { return v0; }
    __builtin_unreachable();
}

// SetScriptAnimationGlobalBaseInfo (0x110a670): typed by tools/typed_lift2.py (round A3; leaf, extra outputs xo[0]=a1 xo[1]=a2 xo[2]=a3)
inline uint64_t SetScriptAnimationGlobalBaseInfo(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint64_t a0, uint64_t *xo = nullptr)
{
    uint64_t v0 = 0;
    uint64_t v1 = 0;
    uint64_t a1 = 0;
    uint64_t a2 = 0;
    uint64_t a3 = 0;
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)281 << 16))));
    (a3 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v0, 4294954176))));
    { uint32_t addr = ADD32((uint32_t)a0, 7); uint32_t aligned_addr = addr & ~7u; uint32_t offset = addr & 7u; uint64_t mem = A1_R64(aligned_addr); uint32_t shift = (7u - offset) << 3; uint64_t keepMask = (shift == 0) ? 0ull : ((1ull << shift) - 1ull); (v1 = (uint64_t)((v1 & keepMask) | (mem << shift))); }
    { uint32_t addr = ADD32((uint32_t)a0, 0); uint32_t aligned_addr = addr & ~7u; uint32_t offset = addr & 7u; uint64_t mem = A1_R64(aligned_addr); uint32_t shift = offset << 3; uint64_t keepMask = (offset == 0) ? 0ull : (0xFFFFFFFFFFFFFFFFull << ((8u - offset) << 3)); (v1 = (uint64_t)((v1 & keepMask) | (mem >> shift))); }
    { uint32_t addr = ADD32((uint32_t)a0, 15); uint32_t aligned_addr = addr & ~7u; uint32_t offset = addr & 7u; uint64_t mem = A1_R64(aligned_addr); uint32_t shift = (7u - offset) << 3; uint64_t keepMask = (shift == 0) ? 0ull : ((1ull << shift) - 1ull); (a1 = (uint64_t)((a1 & keepMask) | (mem << shift))); }
    { uint32_t addr = ADD32((uint32_t)a0, 8); uint32_t aligned_addr = addr & ~7u; uint32_t offset = addr & 7u; uint64_t mem = A1_R64(aligned_addr); uint32_t shift = offset << 3; uint64_t keepMask = (offset == 0) ? 0ull : (0xFFFFFFFFFFFFFFFFull << ((8u - offset) << 3)); (a1 = (uint64_t)((a1 & keepMask) | (mem >> shift))); }
    { uint32_t addr = ADD32((uint32_t)a0, 23); uint32_t aligned_addr = addr & ~7u; uint32_t offset = addr & 7u; uint64_t mem = A1_R64(aligned_addr); uint32_t shift = (7u - offset) << 3; uint64_t keepMask = (shift == 0) ? 0ull : ((1ull << shift) - 1ull); (a2 = (uint64_t)((a2 & keepMask) | (mem << shift))); }
    { uint32_t addr = ADD32((uint32_t)a0, 16); uint32_t aligned_addr = addr & ~7u; uint32_t offset = addr & 7u; uint64_t mem = A1_R64(aligned_addr); uint32_t shift = offset << 3; uint64_t keepMask = (offset == 0) ? 0ull : (0xFFFFFFFFFFFFFFFFull << ((8u - offset) << 3)); (a2 = (uint64_t)((a2 & keepMask) | (mem >> shift))); }
    { uint32_t addr = ADD32((uint32_t)a3, 7); uint32_t aligned_addr = addr & ~7u; uint32_t offset = addr & 7u; uint32_t shift = (7u - offset) << 3; uint64_t mask = 0xFFFFFFFFFFFFFFFFull >> shift; uint64_t old_data = A1_R64(aligned_addr); uint64_t val = v1; uint64_t new_data = (old_data & ~mask) | ((val >> shift) & mask); A1_W64(aligned_addr, new_data); }
    { uint32_t addr = ADD32((uint32_t)a3, 0); uint32_t aligned_addr = addr & ~7u; uint32_t offset = addr & 7u; uint32_t shift = offset << 3; uint64_t mask = 0xFFFFFFFFFFFFFFFFull << shift; uint64_t old_data = A1_R64(aligned_addr); uint64_t val = v1; uint64_t new_data = (old_data & ~mask) | ((val << shift) & mask); A1_W64(aligned_addr, new_data); }
    { uint32_t addr = ADD32((uint32_t)a3, 15); uint32_t aligned_addr = addr & ~7u; uint32_t offset = addr & 7u; uint32_t shift = (7u - offset) << 3; uint64_t mask = 0xFFFFFFFFFFFFFFFFull >> shift; uint64_t old_data = A1_R64(aligned_addr); uint64_t val = a1; uint64_t new_data = (old_data & ~mask) | ((val >> shift) & mask); A1_W64(aligned_addr, new_data); }
    { uint32_t addr = ADD32((uint32_t)a3, 8); uint32_t aligned_addr = addr & ~7u; uint32_t offset = addr & 7u; uint32_t shift = offset << 3; uint64_t mask = 0xFFFFFFFFFFFFFFFFull << shift; uint64_t old_data = A1_R64(aligned_addr); uint64_t val = a1; uint64_t new_data = (old_data & ~mask) | ((val << shift) & mask); A1_W64(aligned_addr, new_data); }
    { uint32_t addr = ADD32((uint32_t)a3, 23); uint32_t aligned_addr = addr & ~7u; uint32_t offset = addr & 7u; uint32_t shift = (7u - offset) << 3; uint64_t mask = 0xFFFFFFFFFFFFFFFFull >> shift; uint64_t old_data = A1_R64(aligned_addr); uint64_t val = a2; uint64_t new_data = (old_data & ~mask) | ((val >> shift) & mask); A1_W64(aligned_addr, new_data); }
    { uint32_t addr = ADD32((uint32_t)a3, 16); uint32_t aligned_addr = addr & ~7u; uint32_t offset = addr & 7u; uint32_t shift = offset << 3; uint64_t mask = 0xFFFFFFFFFFFFFFFFull << shift; uint64_t old_data = A1_R64(aligned_addr); uint64_t val = a2; uint64_t new_data = (old_data & ~mask) | ((val << shift) & mask); A1_W64(aligned_addr, new_data); }
    { uint32_t addr = ADD32((uint32_t)a0, 31); uint32_t aligned_addr = addr & ~7u; uint32_t offset = addr & 7u; uint64_t mem = A1_R64(aligned_addr); uint32_t shift = (7u - offset) << 3; uint64_t keepMask = (shift == 0) ? 0ull : ((1ull << shift) - 1ull); (v1 = (uint64_t)((v1 & keepMask) | (mem << shift))); }
    { uint32_t addr = ADD32((uint32_t)a0, 24); uint32_t aligned_addr = addr & ~7u; uint32_t offset = addr & 7u; uint64_t mem = A1_R64(aligned_addr); uint32_t shift = offset << 3; uint64_t keepMask = (offset == 0) ? 0ull : (0xFFFFFFFFFFFFFFFFull << ((8u - offset) << 3)); (v1 = (uint64_t)((v1 & keepMask) | (mem >> shift))); }
    { uint32_t addr = ADD32((uint32_t)a0, 39); uint32_t aligned_addr = addr & ~7u; uint32_t offset = addr & 7u; uint64_t mem = A1_R64(aligned_addr); uint32_t shift = (7u - offset) << 3; uint64_t keepMask = (shift == 0) ? 0ull : ((1ull << shift) - 1ull); (a1 = (uint64_t)((a1 & keepMask) | (mem << shift))); }
    { uint32_t addr = ADD32((uint32_t)a0, 32); uint32_t aligned_addr = addr & ~7u; uint32_t offset = addr & 7u; uint64_t mem = A1_R64(aligned_addr); uint32_t shift = offset << 3; uint64_t keepMask = (offset == 0) ? 0ull : (0xFFFFFFFFFFFFFFFFull << ((8u - offset) << 3)); (a1 = (uint64_t)((a1 & keepMask) | (mem >> shift))); }
    (a2 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)a0, 40))));
    { uint32_t addr = ADD32((uint32_t)a3, 31); uint32_t aligned_addr = addr & ~7u; uint32_t offset = addr & 7u; uint32_t shift = (7u - offset) << 3; uint64_t mask = 0xFFFFFFFFFFFFFFFFull >> shift; uint64_t old_data = A1_R64(aligned_addr); uint64_t val = v1; uint64_t new_data = (old_data & ~mask) | ((val >> shift) & mask); A1_W64(aligned_addr, new_data); }
    { uint32_t addr = ADD32((uint32_t)a3, 24); uint32_t aligned_addr = addr & ~7u; uint32_t offset = addr & 7u; uint32_t shift = offset << 3; uint64_t mask = 0xFFFFFFFFFFFFFFFFull << shift; uint64_t old_data = A1_R64(aligned_addr); uint64_t val = v1; uint64_t new_data = (old_data & ~mask) | ((val << shift) & mask); A1_W64(aligned_addr, new_data); }
    { uint32_t addr = ADD32((uint32_t)a3, 39); uint32_t aligned_addr = addr & ~7u; uint32_t offset = addr & 7u; uint32_t shift = (7u - offset) << 3; uint64_t mask = 0xFFFFFFFFFFFFFFFFull >> shift; uint64_t old_data = A1_R64(aligned_addr); uint64_t val = a1; uint64_t new_data = (old_data & ~mask) | ((val >> shift) & mask); A1_W64(aligned_addr, new_data); }
    { uint32_t addr = ADD32((uint32_t)a3, 32); uint32_t aligned_addr = addr & ~7u; uint32_t offset = addr & 7u; uint32_t shift = offset << 3; uint64_t mask = 0xFFFFFFFFFFFFFFFFull << shift; uint64_t old_data = A1_R64(aligned_addr); uint64_t val = a1; uint64_t new_data = (old_data & ~mask) | ((val << shift) & mask); A1_W64(aligned_addr, new_data); }
    A1_W32B((uint32_t)a3, 40, (uint32_t)a2);
    { if (xo) { xo[0] = a1; xo[1] = a2; xo[2] = a3; } return v0; }
    __builtin_unreachable();
}

// CopyScriptAnimationWindGeometryPointer (0x110a7d8): typed by tools/typed_lift2.py (round A3; loop leaf, extra outputs xo[0]=t0)
inline uint64_t CopyScriptAnimationWindGeometryPointer(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t a0, uint32_t a1, uint64_t *xo = nullptr)
{
    uint64_t v0 = 0;
    uint64_t v1 = 0;
    uint64_t a2 = 0;
    uint64_t a3 = 0;
    uint64_t t0 = 0;
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(a1, 64))));
L_110a7dc:;
    (v1 = (uint64_t)(A1_R64B(a1, 0)));
    (a2 = (uint64_t)(A1_R64B(a1, 8)));
    (a3 = (uint64_t)(A1_R64B(a1, 16)));
    (t0 = (uint64_t)(A1_R64B(a1, 24)));
    A1_W64B(a0, 0, v1);
    A1_W64B(a0, 8, a2);
    A1_W64B(a0, 16, a3);
    A1_W64B(a0, 24, t0);
    a1 = a1 + 0x20u;
    { const bool c = (int64_t)(int32_t)a1 != (int64_t)v0; a0 = a0 + 0x20u; if (c) { A3_BACK(); goto L_110a7dc; } }
    (v0 = (uint64_t)(A1_R64B(a1, 0)));
    (v1 = (uint64_t)(A1_R64B(a1, 8)));
    A1_W64B(a0, 0, v0);
    { A1_W64B(a0, 8, v1); if (xo) { xo[0] = t0; } return v0; }
    __builtin_unreachable();
}

// GetSpriteMenuClass (0x1111998): typed by tools/typed_lift2.py (round A3; leaf, extra outputs xo[0]=a0)
inline uint64_t GetSpriteMenuClass(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint64_t a0, uint64_t *xo = nullptr)
{
    uint64_t v0 = 0;
    uint64_t v1 = 0;
    (v0 = (uint64_t)((uint64_t)a0 + (uint64_t)0ULL));
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 33))));
    (v1 = (uint64_t)(uint32_t)((uint8_t)A1_R8B((uint32_t)v0, 8)));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v0, 144))));
    (v1 = (uint64_t)(RC_U64X(v1) ^ RC_U64X(a0)));
    { if (RC_U64X(v1) != 0) v0 = (uint64_t)0;; if (xo) { xo[0] = a0; } return v0; }
    __builtin_unreachable();
}

// SpriteMenuSetPadEventCallback (0x11119d8): typed by tools/typed_lift2.py (round A3; leaf, extra outputs xo[0]=a3)
inline uint32_t SpriteMenuSetPadEventCallback(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t a0, uint32_t a1, uint32_t a2, uint64_t *xo = nullptr)
{
    uint32_t v0 = 0;
    uint32_t v1 = 0;
    uint32_t a3 = 0;
    v1 = A1_R8B(a0, 8);
    a0 = a0 + 0x90u;
    a3 = a1 << 4;
    v1 = v1 ^ 0x21u;
    a3 = a3 + a1;
    if (a1_sx64(v1) != 0) a0 = (uint32_t)((uint64_t)0);;
    a3 = a3 << 2;
    v0 = A1_R32B(a0, 4);
    v0 = v0 + a3;
    { A1_W32B(v0, 60, a2); if (xo) { xo[0] = a1_sx64(a3); } return v0; }
    __builtin_unreachable();
}

// SpriteSubMenuSetPadEventCallback (0x1111a08): typed by tools/typed_lift2.py (round A3; leaf, extra outputs xo[0]=a3)
inline uint32_t SpriteSubMenuSetPadEventCallback(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t a0, uint32_t a1, uint32_t a2, uint64_t *xo = nullptr)
{
    uint32_t v0 = 0;
    uint32_t v1 = 0;
    uint32_t a3 = 0;
    v1 = A1_R8B(a0, 8);
    a0 = a0 + 0x90u;
    a3 = a1 << 4;
    v1 = v1 ^ 0x21u;
    a3 = a3 + a1;
    if (a1_sx64(v1) != 0) a0 = (uint32_t)((uint64_t)0);;
    a3 = a3 << 2;
    v0 = A1_R32B(a0, 4);
    v0 = v0 + a3;
    { A1_W32B(v0, 64, a2); if (xo) { xo[0] = a1_sx64(a3); } return v0; }
    __builtin_unreachable();
}

// SpriteMenuSetDispFlag (0x1111a38): typed by tools/typed_lift2.py (round A3; leaf, extra outputs xo[0]=a0 xo[1]=a1 xo[2]=a3)
inline uint32_t SpriteMenuSetDispFlag(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t a0, uint32_t a1, uint64_t a2, uint64_t *xo = nullptr)
{
    uint32_t v0 = 0;
    uint32_t v1 = 0;
    uint32_t a3 = 0;
    v1 = A1_R8B(a0, 8);
    a0 = a0 + 0x90u;
    a3 = a1 << 4;
    v1 = v1 ^ 0x21u;
    a3 = a3 + a1;
    if (a1_sx64(v1) != 0) a0 = (uint32_t)((uint64_t)0);;
    a3 = a3 << 2;
    v0 = A1_R32B(a0, 4);
    { const bool c = (int64_t)a2 == 0; a0 = v0 + a3; if (c) goto L_1111a98; }
    a1 = A1_R32B(a0, 8);
    v1 = a1 & 0xFu;
    if (v1 == 0) { v0 = 0xFFFFFFF0u; goto L_1111a88; }
    { const bool c = (int32_t)v1 < 0; v0 = (int32_t)v1 < 6 ? 1u : 0u; if (c) goto L_1111ac0; }
    { const bool c = v0 == 0; v0 = (int32_t)v1 < 4 ? 1u : 0u; if (c) goto L_1111ac0; }
    { const bool c = v0 != 0; v0 = 0xFFFFFFF0u; if (c) goto L_1111ac0; }
L_1111a88:;
    v0 = a1 & v0;
    v0 = v0 | 1u;
    { A1_W32B(a0, 8, v0); if (xo) { xo[0] = a1_sx64(a0); xo[1] = a1_sx64(a1); xo[2] = a1_sx64(a3); } return v0; }
L_1111a98:;
    a1 = A1_R32B(a0, 8);
    v1 = a1 & 0xFu;
    v0 = (int32_t)v1 < 4 ? 1u : 0u;
    if (v0 == 0) goto L_1111ac0;
    { const bool c = v1 == 0; v0 = 0xFFFFFFF0u; if (c) goto L_1111ac0; }
    v0 = a1 & v0;
    v0 = v0 | 4u;
    A1_W32B(a0, 8, v0);
L_1111ac0:;
    { if (xo) { xo[0] = a1_sx64(a0); xo[1] = a1_sx64(a1); xo[2] = a1_sx64(a3); } return v0; }
    __builtin_unreachable();
}

// SpriteMenuSetDispFlagByHash (0x1111b78): typed by tools/typed_lift2.py (round A3; loop leaf, extra outputs xo[0]=a1 xo[1]=a3 xo[2]=t0 xo[3]=t1 xo[4]=t2 xo[5]=t3)
inline uint64_t SpriteMenuSetDispFlagByHash(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint64_t a0, uint64_t a1, uint64_t a2, uint64_t *xo = nullptr)
{
    uint64_t v0 = 0;
    uint64_t v1 = 0;
    uint64_t a3 = 0;
    uint64_t t0 = 0;
    uint32_t t1 = 0;
    uint64_t t2 = 0;
    uint32_t t3 = 0;
    bool a3xw2 = false;
    (t3 = (uint32_t)((uint8_t)A1_R8B((uint32_t)a0, 8)));
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)a0, 144))));
    (t2 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 4294967295))));
    (v0 = (uint64_t)(a1_sx64(t3) ^ (uint64_t)(uint16_t)33));
    if (RC_U64X(v0) != 0) v1 = (uint64_t)0;;
    (t1 = (uint32_t)((int32_t)((int32_t)A1_R32B((uint32_t)v1, 8))));
    { const bool c = (int32_t)t1 <= 0; (a3 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL)); if (c) goto L_1111be4; }
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)v1, 4))));
    (t0 = (uint64_t)((uint64_t)RC_U64X(v0) + (uint64_t)0ULL)); a3xw2 = true;
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)t0, 0))));
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)v0, 4))));
    if ((int64_t)v1 != (int64_t)a1) { (a3 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)a3, 1)))); goto L_1111bb8; }
    (t2 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL));
    goto L_1111be4;
L_1111bb8:;
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)SLL32((uint32_t)a3, 4))));
    (v1 = (uint64_t)(((int64_t)(int64_t)a3 < (int64_t)(int64_t)a1_sx64(t1)) ? 1 : 0));
    { const bool c = (int64_t)v1 == 0; (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v0, (uint32_t)a3)))); if (c) goto L_1111be4; }
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)SLL32((uint32_t)v0, 2))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)t0, (uint32_t)v0))));
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)v0, 0))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)v1, 4))));
    if ((int64_t)v0 != (int64_t)a1) { (a3 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)a3, 1)))); { A3_BACK(); goto L_1111bb8; } }
    (t2 = (uint64_t)((uint64_t)a3 + (uint64_t)0ULL));
L_1111be4:;
    (v1 = (uint64_t)(a1_sx64(t3) ^ (uint64_t)(uint16_t)33));
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)a0, 144))));
    if (v1 != 0) a0 = (uint64_t)0;;
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)SLL32((uint32_t)t2, 4))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v0, (uint32_t)t2))));
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)a0, 4))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)SLL32((uint32_t)v0, 2))));
    { const bool c = (int64_t)a2 == 0; (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v1, (uint32_t)v0)))); if (c) goto L_1111c40; }
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)v1, 8))));
    (a1 = (uint64_t)(RC_U64X(a0) & (uint64_t)(uint16_t)15));
    if ((int64_t)a1 == 0) { (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 4294967280)))); goto L_1111c30; }
    { const bool c = (int32_t)a1 < 0; (v0 = (uint64_t)(((int64_t)(int64_t)RC_U64X(a1) < (int64_t)(int32_t)6) ? 1 : 0)); if (c) goto L_1111c68; }
    { const bool c = (int64_t)v0 == 0; (v0 = (uint64_t)(((int64_t)(int64_t)RC_U64X(a1) < (int64_t)(int32_t)4) ? 1 : 0)); if (c) goto L_1111c68; }
    { const bool c = (int64_t)v0 != 0; (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 4294967280)))); if (c) goto L_1111c68; }
L_1111c30:;
    (v0 = (uint64_t)(RC_U64X(a0) & RC_U64X(v0)));
    (v0 = (uint64_t)(RC_U64X(v0) | (uint64_t)(uint16_t)1));
    { A1_W32B((uint32_t)v1, 8, (uint32_t)v0); if (xo) { xo[0] = a1; xo[1] = a3; if (a3xw2) xo[2] = t0; xo[3] = a1_sx64(t1); xo[4] = t2; xo[5] = a1_sx64(t3); } return v0; }
L_1111c40:;
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)v1, 8))));
    (a1 = (uint64_t)(RC_U64X(a0) & (uint64_t)(uint16_t)15));
    (v0 = (uint64_t)(((int64_t)(int64_t)RC_U64X(a1) < (int64_t)(int32_t)4) ? 1 : 0));
    if ((int64_t)v0 == 0) goto L_1111c68;
    { const bool c = (int64_t)a1 == 0; (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 4294967280)))); if (c) goto L_1111c68; }
    (v0 = (uint64_t)(RC_U64X(a0) & RC_U64X(v0)));
    (v0 = (uint64_t)(RC_U64X(v0) | (uint64_t)(uint16_t)4));
    A1_W32B((uint32_t)v1, 8, (uint32_t)v0);
L_1111c68:;
    { if (xo) { xo[0] = a1; xo[1] = a3; if (a3xw2) xo[2] = t0; xo[3] = a1_sx64(t1); xo[4] = t2; xo[5] = a1_sx64(t3); } return v0; }
    __builtin_unreachable();
}

// SpriteMenuSetLayer (0x1111e20): typed by tools/typed_lift2.py (round A3; leaf, extra outputs xo[0]=a0 xo[1]=a2 xo[2]=a3 xo[3]=t0)
inline uint32_t SpriteMenuSetLayer(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t a0, uint64_t a1, uint64_t *xo = nullptr)
{
    uint32_t v0 = 0;
    uint32_t v1 = 0;
    uint32_t a2 = 0;
    uint64_t a3 = 0;
    uint32_t t0 = 0;
    v1 = A1_R8B(a0, 8);
    a0 = a0 + 0x90u;
    (a3 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL));
    (v0 = (uint32_t)((int32_t)((int32_t)SLL32((uint32_t)a1, 4))));
    v1 = v1 ^ 0x21u;
    (v0 = (uint32_t)((int32_t)((int32_t)ADD32(v0, (uint32_t)a1))));
    if (a1_sx64(v1) == 0) a3 = (uint64_t)(a1_sx64(a0));;
    t0 = v0 << 2;
    (a0 = (uint32_t)((int32_t)((int32_t)A1_R32B((uint32_t)a3, 12))));
    (v1 = (uint32_t)((int32_t)((int32_t)A1_R32B((uint32_t)a3, 4))));
    v0 = a0 << 4;
    v0 = v0 + a0;
    v0 = v0 << 2;
    { const bool c = (int64_t)(int32_t)a0 == (int64_t)a1; a2 = v1 + v0; if (c) goto L_1111e8c; }
    v1 = A1_R32B(a2, 8);
    v0 = 0xFFEF0000u;
    v0 = v0 | 0xFFFFu;
    a0 = 0x100000u;
    v1 = v1 & v0;
    A1_W32B(a2, 8, v1);
    A1_W32B((uint32_t)a3, 12, (uint32_t)a1);
    (v1 = (uint32_t)((int32_t)((int32_t)A1_R32B((uint32_t)a3, 4))));
    a2 = v1 + t0;
    v0 = A1_R32B(a2, 8);
    v0 = v0 | a0;
    A1_W32B(a2, 8, v0);
L_1111e8c:;
    { if (xo) { xo[0] = a1_sx64(a0); xo[1] = a1_sx64(a2); xo[2] = a3; xo[3] = a1_sx64(t0); } return v0; }
    __builtin_unreachable();
}

// SpriteMenuGetLayerStatus (0x1111f60): typed by tools/typed_lift2.py (round A3; leaf, extra outputs xo[0]=a2)
inline uint32_t SpriteMenuGetLayerStatus(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t a0, uint32_t a1, uint64_t *xo = nullptr)
{
    uint32_t v0 = 0;
    uint32_t v1 = 0;
    uint32_t a2 = 0;
    v0 = A1_R8B(a0, 8);
    a0 = a0 + 0x90u;
    a2 = a1 << 4;
    v0 = v0 ^ 0x21u;
    a2 = a2 + a1;
    if (a1_sx64(v0) != 0) a0 = (uint32_t)((uint64_t)0);;
    a2 = a2 << 2;
    v1 = A1_R32B(a0, 4);
    v1 = v1 + a2;
    v0 = A1_R32B(v1, 8);
    { v0 = v0 & 0xFu; if (xo) { xo[0] = a1_sx64(a2); } return v0; }
    __builtin_unreachable();
}

// SpriteMenuGetLayerByHash (0x1111f90): typed by tools/typed_lift2.py (round A3; loop leaf, extra outputs xo[0]=a2 xo[1]=a3 xo[2]=t0 xo[3]=t1)
inline uint64_t SpriteMenuGetLayerByHash(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint64_t a0, uint64_t a1, uint64_t *xo = nullptr)
{
    uint64_t v0 = 0;
    uint32_t v1 = 0;
    uint64_t a2 = 0;
    uint64_t a3 = 0;
    uint64_t t0 = 0;
    uint64_t t1 = 0;
    bool a3xw0 = false;
    bool a3xw3 = false;
    (v0 = (uint64_t)(uint32_t)((uint8_t)A1_R8B((uint32_t)a0, 8)));
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)a0, 144))));
    (t0 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL));
    (v0 = (uint64_t)(RC_U64X(v0) ^ (uint64_t)(uint16_t)33));
    if (RC_U64X(v0) != 0) a0 = (uint64_t)0;;
    (v0 = (uint64_t)((uint64_t)a0 + (uint64_t)0ULL));
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)v0, 8))));
    { const bool c = (int32_t)a0 <= 0; (a3 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL)); if (c) goto L_1111ffc; }
    (a3 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)v0, 4))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)a3, 0))));
    (v1 = (uint32_t)((int32_t)((int32_t)A1_R32B((uint32_t)v0, 4))));
    if ((int64_t)(int32_t)v1 == (int64_t)a1) goto L_1111ffc;
    (t1 = (uint64_t)((uint64_t)RC_U64X(a0) + (uint64_t)0ULL)); a3xw3 = true;
    (a0 = (uint64_t)((uint64_t)RC_U64X(a3) + (uint64_t)0ULL));
    (a2 = (uint64_t)((uint64_t)RC_U64X(a3) + (uint64_t)0ULL)); a3xw0 = true;
L_1111fd8:;
    (t0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)t0, 1))));
    (a2 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)a2, 68)))); a3xw0 = true;
    (v0 = (uint64_t)(((int64_t)(int64_t)RC_U64X(t0) < (int64_t)(int64_t)t1) ? 1 : 0));
    { const bool c = (int64_t)v0 == 0; (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)a0, 68)))); if (c) goto L_1111ffc; }
    (v1 = (uint32_t)((int32_t)((int32_t)A1_R32B((uint32_t)a2, 0))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B(v1, 4))));
    { const bool c = (int64_t)v0 != (int64_t)a1; (a3 = (uint64_t)((uint64_t)RC_U64X(a0) + (uint64_t)0ULL)); if (c) { A3_BACK(); goto L_1111fd8; } }
L_1111ffc:;
    { (v0 = (uint64_t)((uint64_t)a3 + (uint64_t)0ULL)); if (xo) { if (a3xw0) xo[0] = a2; xo[1] = a3; xo[2] = t0; if (a3xw3) xo[3] = t1; } return v0; }
    __builtin_unreachable();
}

// SpriteMenuGetLayerNoByHash (0x1112008): typed by tools/typed_lift2.py (round A3; loop leaf, extra outputs xo[0]=a0 xo[1]=a2 xo[2]=a3 xo[3]=t0)
inline uint64_t SpriteMenuGetLayerNoByHash(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint64_t a0, uint64_t a1, uint64_t *xo = nullptr)
{
    uint64_t v0 = 0;
    uint64_t v1 = 0;
    uint64_t a2 = 0;
    uint32_t a3 = 0;
    uint64_t t0 = 0;
    (v0 = (uint64_t)(uint32_t)((uint8_t)A1_R8B((uint32_t)a0, 8)));
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)a0, 144))));
    (v1 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL));
    (t0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 4294967295))));
    (v0 = (uint64_t)(RC_U64X(v0) ^ (uint64_t)(uint16_t)33));
    if (RC_U64X(v0) == 0) v1 = (uint64_t)(a0);;
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
    { (v0 = (uint64_t)((uint64_t)t0 + (uint64_t)0ULL)); if (xo) { xo[0] = a0; xo[1] = a2; xo[2] = a1_sx64(a3); xo[3] = t0; } return v0; }
    __builtin_unreachable();
}

// SpriteMenuGetMenuIdByHash (0x1112c88): typed by tools/typed_lift2.py (round A3; loop leaf, extra outputs xo[0]=a1 xo[1]=a3 xo[2]=t0 xo[3]=t1 xo[4]=t2 xo[5]=t3)
inline uint64_t SpriteMenuGetMenuIdByHash(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint64_t a0, uint64_t a1, uint64_t a2, uint64_t *xo = nullptr)
{
    uint64_t v0 = 0;
    uint32_t v1 = 0;
    uint64_t a3 = 0;
    uint64_t t0 = 0;
    uint64_t t1 = 0;
    uint64_t t2 = 0;
    uint32_t t3 = 0;
    bool a3xw1 = false;
    bool a3xw4 = false;
    (v0 = (uint64_t)(uint32_t)((uint8_t)A1_R8B((uint32_t)a0, 8)));
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)a0, 144))));
    t3 = 0xFFFFFFFFu;
    (t1 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL));
    (v0 = (uint64_t)(RC_U64X(v0) ^ (uint64_t)(uint16_t)33));
    if (RC_U64X(v0) != 0) a0 = (uint64_t)0;;
    (v0 = (uint64_t)((uint64_t)a0 + (uint64_t)0ULL));
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)v0, 8))));
    { const bool c = (int32_t)a0 <= 0; (t0 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL)); if (c) goto L_1112cf4; }
    (t1 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)v0, 4))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)t1, 0))));
    (v1 = (uint32_t)((int32_t)((int32_t)A1_R32B((uint32_t)v0, 4))));
    if ((int64_t)(int32_t)v1 == (int64_t)a1) { (a1 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)t1, 8)))); goto L_1112cf8; }
    (t2 = (uint64_t)((uint64_t)RC_U64X(a0) + (uint64_t)0ULL)); a3xw4 = true;
    (a0 = (uint64_t)((uint64_t)RC_U64X(t1) + (uint64_t)0ULL));
    (a3 = (uint64_t)((uint64_t)RC_U64X(t1) + (uint64_t)0ULL)); a3xw1 = true;
L_1112cd0:;
    (t0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)t0, 1))));
    (a3 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)a3, 68)))); a3xw1 = true;
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
    { (v0 = (uint64_t)((uint64_t)a1_sx64(t3) + (uint64_t)0ULL)); if (xo) { xo[0] = a1; if (a3xw1) xo[1] = a3; xo[2] = t0; xo[3] = t1; if (a3xw4) xo[4] = t2; xo[5] = a1_sx64(t3); } return v0; }
    __builtin_unreachable();
}

// SpriteMenuGetSelectMenuIdBySpriteMenuClass (0x1112e68): typed by tools/typed_lift2.py (round A3; leaf, extra outputs xo[0]=a1)
inline uint32_t SpriteMenuGetSelectMenuIdBySpriteMenuClass(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t a0, uint64_t *xo = nullptr)
{
    uint32_t v0 = 0;
    uint32_t v1 = 0;
    uint32_t a1 = 0;
    v1 = A1_R32B(a0, 12);
    v0 = 0xFFFFFFFFu;
    { const bool c = v1 == v0; a1 = v1 << 4; if (c) goto L_1112e98; }
    v0 = a1 + v1;
    v1 = A1_R32B(a0, 4);
    v0 = v0 << 2;
    v1 = v1 + v0;
    v0 = A1_R32B(v1, 56);
    v0 = v0 << 11;
    { v0 = (uint32_t)((int32_t)v0 >> 22); if (xo) { xo[0] = a1_sx64(a1); } return v0; }
L_1112e98:;
    { if (xo) { xo[0] = a1_sx64(a1); } return v0; }
    __builtin_unreachable();
}

// SpriteMenuSetSelectMenuId (0x1112ef0): typed by tools/typed_lift2.py (round A3; leaf, extra outputs xo[0]=a1 xo[1]=a2)
inline uint32_t SpriteMenuSetSelectMenuId(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t a0, uint32_t a1, uint64_t *xo = nullptr)
{
    uint32_t v0 = 0;
    uint32_t v1 = 0;
    uint64_t a2 = 0;
    v0 = A1_R8B(a0, 8);
    a0 = a0 + 0x90u;
    (a2 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL));
    a1 = a1 & 0x3FFu;
    v0 = v0 ^ 0x21u;
    v1 = 0xFFFFFFFFu;
    if (a1_sx64(v0) == 0) a2 = (uint64_t)(a1_sx64(a0));;
    a1 = a1 << 11;
    (v0 = (uint32_t)((int32_t)((int32_t)A1_R32B((uint32_t)a2, 12))));
    { const bool c = v0 == v1; a0 = v0 << 4; if (c) goto L_1112f44; }
    v0 = a0 + v0;
    (a0 = (uint32_t)((int32_t)((int32_t)A1_R32B((uint32_t)a2, 4))));
    v0 = v0 << 2;
    v1 = 0xFFE00000u;
    a0 = a0 + v0;
    v1 = v1 | 0x7FFu;
    v0 = A1_R32B(a0, 56);
    v0 = v0 & v1;
    v0 = v0 | a1;
    A1_W32B(a0, 56, v0);
L_1112f44:;
    { if (xo) { xo[0] = a1_sx64(a1); xo[1] = a2; } return v0; }
    __builtin_unreachable();
}

// SpriteMenuGetSelectMenuMode (0x1113088): typed by tools/typed_lift2.py (round A3; leaf, extra outputs xo[0]=a1)
inline uint32_t SpriteMenuGetSelectMenuMode(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t a0, uint64_t *xo = nullptr)
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
    { const bool c = v0 == v1; a0 = v0 << 4; if (c) goto L_11130d0; }
    v0 = a0 + v0;
    (v1 = (uint32_t)((int32_t)((int32_t)A1_R32B((uint32_t)a1, 4))));
    v0 = v0 << 2;
    v1 = v1 + v0;
    v0 = A1_R32B(v1, 56);
    v0 = v0 << 21;
    { v0 = (uint32_t)((int32_t)v0 >> 30); if (xo) { xo[0] = a1; } return v0; }
L_11130d0:;
    { v0 = 0xFFFFFFFFu; if (xo) { xo[0] = a1; } return v0; }
    __builtin_unreachable();
}

// SpriteMenuSetDefaultSelectMenu (0x1113130): typed by tools/typed_lift2.py (round A3; leaf, extra outputs xo[0]=a0 xo[1]=a1 xo[2]=a2 xo[3]=a3)
inline uint32_t SpriteMenuSetDefaultSelectMenu(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t a0, uint32_t a1, uint32_t a2, uint64_t *xo = nullptr)
{
    uint32_t v0 = 0;
    uint32_t v1 = 0;
    uint32_t a3 = 0;
    v0 = A1_R8B(a0, 8);
    a0 = a0 + 0x90u;
    a3 = a1 << 4;
    a2 = a2 & 0x3FFu;
    v0 = v0 ^ 0x21u;
    a3 = a3 + a1;
    if (a1_sx64(v0) != 0) a0 = (uint32_t)((uint64_t)0);;
    a3 = a3 << 2;
    v1 = A1_R32B(a0, 4);
    a1 = 0xFFE00000u;
    a1 = a1 | 0x7FFu;
    a2 = a2 << 11;
    v1 = v1 + a3;
    v0 = A1_R32B(v1, 56);
    v0 = v0 & a1;
    v0 = v0 | a2;
    { A1_W32B(v1, 56, v0); if (xo) { xo[0] = a1_sx64(a0); xo[1] = a1_sx64(a1); xo[2] = a1_sx64(a2); xo[3] = a1_sx64(a3); } return v0; }
    __builtin_unreachable();
}

// SpriteMenuSetNumPartsFormatByPartsParam (0x11137b8): typed by tools/typed_lift2.py (round A3; leaf, extra outputs xo[0]=a2)
inline uint32_t SpriteMenuSetNumPartsFormatByPartsParam(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t a1, uint32_t a2, uint64_t *xo = nullptr)
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

// SpriteMenuSetAnimParam (0x11137e0): typed by tools/typed_lift2.py (round A3; leaf, extra outputs xo[0]=v0)
inline void SpriteMenuSetAnimParam(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint64_t a0, uint64_t a1, uint32_t a2, uint64_t a3, float f12, uint64_t *xo = nullptr)
{
    uint64_t v0 = 0;
    uint32_t v1 = 0;
    uint64_t t0 = 0;
    bool a3xw0 = false;
    (t0 = (uint64_t)((uint64_t)a0 + (uint64_t)0ULL));
    if ((int64_t)t0 == 0) goto L_1113840;
    (v1 = (uint32_t)((int32_t)((int32_t)A1_R32B((uint32_t)t0, 8))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)SRL32(v1, 27)))); a3xw0 = true;
    (v0 = (uint64_t)(RC_U64X(v0) & (uint64_t)(uint16_t)15)); a3xw0 = true;
    if ((int64_t)v0 == 0) goto L_1113808;
    if ((int64_t)a3 == 0) goto L_1113840;
L_1113808:;
    if ((int64_t)v0 != (int64_t)a1) { { float f = f12; uint32_t bits; std::memcpy(&bits, &f, sizeof(bits)); A1_W32B((uint32_t)t0, 12, bits); } goto L_1113810; }
L_1113810:;
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)34815 << 16)))); a3xw0 = true;
    (a0 = (uint64_t)(a1 & (uint64_t)(uint16_t)15));
    (v0 = (uint64_t)(RC_U64X(v0) | (uint64_t)(uint16_t)65535)); a3xw0 = true;
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)SLL32((uint32_t)a0, 27))));
    (v0 = (uint64_t)(a1_sx64(v1) & RC_U64X(v0))); a3xw0 = true;
    v1 = 0x7FFF0000u;
    (v0 = (uint64_t)(RC_U64X(v0) | RC_U64X(a0))); a3xw0 = true;
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)SLL32(a2, 31))));
    v1 = v1 | 0xFFFFu;
    (v0 = (uint64_t)(RC_U64X(v0) & a1_sx64(v1))); a3xw0 = true;
    (v0 = (uint64_t)(RC_U64X(v0) | RC_U64X(a0))); a3xw0 = true;
    A1_W32B((uint32_t)t0, 8, (uint32_t)v0);
L_1113840:;
    { if (xo) { if (a3xw0) xo[0] = v0; } return; }
    __builtin_unreachable();
}

// GetSpriteClass (0x1114170): typed by tools/typed_lift2.py (round A3; leaf, extra outputs xo[0]=a0)
inline uint64_t GetSpriteClass(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint64_t a0, uint64_t *xo = nullptr)
{
    uint64_t v0 = 0;
    uint64_t v1 = 0;
    (v0 = (uint64_t)((uint64_t)a0 + (uint64_t)0ULL));
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 14))));
    (v1 = (uint64_t)(uint32_t)((uint8_t)A1_R8B((uint32_t)v0, 8)));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v0, 144))));
    (v1 = (uint64_t)(RC_U64X(v1) ^ RC_U64X(a0)));
    { if (RC_U64X(v1) != 0) v0 = (uint64_t)0;; if (xo) { xo[0] = a0; } return v0; }
    __builtin_unreachable();
}

// ChangeCollisionResourceAttribute (0x1128778): typed by tools/typed_lift2.py (round A3; loop leaf)
inline uint64_t ChangeCollisionResourceAttribute(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint64_t a0, uint64_t a1, uint64_t a2, uint64_t a3, uint64_t t0, uint64_t t1, uint64_t t2)
{
    uint64_t v0 = 0;
    uint64_t v1 = 0;
    uint32_t t3 = 0;
    uint32_t t4 = 0;
    uint32_t t5 = 0;
    (t4 = (uint32_t)((int32_t)((int32_t)A1_R32B((uint32_t)a0, 48))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 4294967295))));
    t4 = t4 - 1u;
    { const bool c = (int64_t)(int32_t)t4 == (int64_t)v0; (t3 = (uint32_t)((int32_t)((int32_t)A1_R32B((uint32_t)a0, 52)))); if (c) goto L_11287c8; }
    (t1 = (uint64_t)(~(0ULL | t1)));
    t5 = 0xFFFFFFFFu;
L_1128798:;
    (v1 = (uint64_t)(A1_R64B(t3, 0)));
    (v0 = (uint64_t)(v1 & a1));
    { const bool c = (int64_t)v0 != (int64_t)a2; (a0 = (uint64_t)(v1 & a3)); if (c) goto L_11287bc; }
    if ((int64_t)a0 != (int64_t)t0) { t4 = t4 - 1u; goto L_11287c0; }
    (v0 = (uint64_t)(v1 & t1));
    (v0 = (uint64_t)(v0 | t2));
    A1_W64B(t3, 0, v0);
L_11287bc:;
    t4 = t4 - 1u;
L_11287c0:;
    { const bool c = t4 != t5; t3 = t3 + 0x10u; if (c) { A3_BACK(); goto L_1128798; } }
L_11287c8:;
    { return v0; }
    __builtin_unreachable();
}

// ConstructorCollisionObjectJudgeCallbackData (0x1128898): typed by tools/typed_lift2.py (round A3; loop leaf)
inline uint32_t ConstructorCollisionObjectJudgeCallbackData(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t a0)
{
    uint32_t v0 = 0;
    A1_W32(a0, 0u);
    a0 = a0 + 0x14u;
    v0 = 4u;
L_11288a8:;
    v0 = v0 - 1u;
    A1_W32(a0, 0u);
    { const bool c = (int32_t)v0 >= 0; a0 = a0 - 4u; if (c) { A3_BACK(); goto L_11288a8; } }
    { return v0; }
    __builtin_unreachable();
}

// CopyCollisionFinder (0x112a350): typed by tools/typed_lift2.py (round A3; leaf)
inline uint64_t CopyCollisionFinder(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint64_t a0, uint64_t a1)
{
    uint64_t v0 = 0;
    uint64_t v1 = 0;
    uint64_t a2 = 0;
    { uint32_t addr = ADD32((uint32_t)a1, 7); uint32_t aligned_addr = addr & ~7u; uint32_t offset = addr & 7u; uint64_t mem = A1_R64(aligned_addr); uint32_t shift = (7u - offset) << 3; uint64_t keepMask = (shift == 0) ? 0ull : ((1ull << shift) - 1ull); (v0 = (uint64_t)((v0 & keepMask) | (mem << shift))); }
    { uint32_t addr = ADD32((uint32_t)a1, 0); uint32_t aligned_addr = addr & ~7u; uint32_t offset = addr & 7u; uint64_t mem = A1_R64(aligned_addr); uint32_t shift = offset << 3; uint64_t keepMask = (offset == 0) ? 0ull : (0xFFFFFFFFFFFFFFFFull << ((8u - offset) << 3)); (v0 = (uint64_t)((v0 & keepMask) | (mem >> shift))); }
    { uint32_t addr = ADD32((uint32_t)a1, 15); uint32_t aligned_addr = addr & ~7u; uint32_t offset = addr & 7u; uint64_t mem = A1_R64(aligned_addr); uint32_t shift = (7u - offset) << 3; uint64_t keepMask = (shift == 0) ? 0ull : ((1ull << shift) - 1ull); (v1 = (uint64_t)((v1 & keepMask) | (mem << shift))); }
    { uint32_t addr = ADD32((uint32_t)a1, 8); uint32_t aligned_addr = addr & ~7u; uint32_t offset = addr & 7u; uint64_t mem = A1_R64(aligned_addr); uint32_t shift = offset << 3; uint64_t keepMask = (offset == 0) ? 0ull : (0xFFFFFFFFFFFFFFFFull << ((8u - offset) << 3)); (v1 = (uint64_t)((v1 & keepMask) | (mem >> shift))); }
    (a2 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)a1, 16))));
    { uint32_t addr = ADD32((uint32_t)a0, 7); uint32_t aligned_addr = addr & ~7u; uint32_t offset = addr & 7u; uint32_t shift = (7u - offset) << 3; uint64_t mask = 0xFFFFFFFFFFFFFFFFull >> shift; uint64_t old_data = A1_R64(aligned_addr); uint64_t val = v0; uint64_t new_data = (old_data & ~mask) | ((val >> shift) & mask); A1_W64(aligned_addr, new_data); }
    { uint32_t addr = ADD32((uint32_t)a0, 0); uint32_t aligned_addr = addr & ~7u; uint32_t offset = addr & 7u; uint32_t shift = offset << 3; uint64_t mask = 0xFFFFFFFFFFFFFFFFull << shift; uint64_t old_data = A1_R64(aligned_addr); uint64_t val = v0; uint64_t new_data = (old_data & ~mask) | ((val << shift) & mask); A1_W64(aligned_addr, new_data); }
    { uint32_t addr = ADD32((uint32_t)a0, 15); uint32_t aligned_addr = addr & ~7u; uint32_t offset = addr & 7u; uint32_t shift = (7u - offset) << 3; uint64_t mask = 0xFFFFFFFFFFFFFFFFull >> shift; uint64_t old_data = A1_R64(aligned_addr); uint64_t val = v1; uint64_t new_data = (old_data & ~mask) | ((val >> shift) & mask); A1_W64(aligned_addr, new_data); }
    { uint32_t addr = ADD32((uint32_t)a0, 8); uint32_t aligned_addr = addr & ~7u; uint32_t offset = addr & 7u; uint32_t shift = offset << 3; uint64_t mask = 0xFFFFFFFFFFFFFFFFull << shift; uint64_t old_data = A1_R64(aligned_addr); uint64_t val = v1; uint64_t new_data = (old_data & ~mask) | ((val << shift) & mask); A1_W64(aligned_addr, new_data); }
    { A1_W32B((uint32_t)a0, 16, (uint32_t)a2); return v0; }
    __builtin_unreachable();
}

// GetGeometryPointerObjectParentNodeId (0x1132a00): typed by tools/typed_lift2.py (round A3; leaf, extra outputs xo[0]=a1)
inline uint32_t GetGeometryPointerObjectParentNodeId(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint64_t a0, uint64_t *xo = nullptr)
{
    uint32_t v0 = 0;
    uint32_t v1 = 0;
    uint64_t a1 = 0;
    (a1 = (uint64_t)((uint64_t)a0 + (uint64_t)0ULL));
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 1))));
    (v1 = (uint32_t)((int32_t)((int32_t)A1_R32B((uint32_t)a1, 48))));
    { const bool c = (int64_t)(int32_t)v1 != (int64_t)a0; v0 = 0xFFFFFFFFu; if (c) goto L_1132a18; }
    (v0 = (uint32_t)((int32_t)((int32_t)A1_R32B((uint32_t)a1, 4))));
L_1132a18:;
    { if (xo) { xo[0] = a1; } return v0; }
    __builtin_unreachable();
}

// GetClothWorkSize (0x1139128): typed by tools/typed_lift2.py (round A3; loop leaf)
inline uint32_t GetClothWorkSize(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint64_t a0)
{
    uint32_t v0 = 0;
    uint64_t v1 = 0;
    uint64_t a1 = 0;
    uint64_t a2 = 0;
    uint32_t a3 = 0;
    uint64_t hi = 0;
    uint64_t lo = 0;
    (a1 = (uint64_t)((uint64_t)a0 + (uint64_t)0ULL));
    (a0 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL));
    (v0 = (uint32_t)((int32_t)((int16_t)A1_R16B((uint32_t)a1, 0))));
    { const bool c = (int32_t)v0 < 0; (v1 = (uint64_t)((uint64_t)a1 + (uint64_t)0ULL)); if (c) goto L_113915c; }
L_1139140:;
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v1, 52))));
    (v0 = (uint32_t)((int32_t)((int16_t)A1_R16B((uint32_t)v1, 0))));
    { const bool c = (int32_t)v0 >= 0; (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)a0, 1)))); if (c) { A3_BACK(); goto L_1139140; } }
L_113915c:;
    (v0 = (uint32_t)((int32_t)((int32_t)SLL32((uint32_t)a0, 1))));
    (v0 = (uint32_t)((int32_t)((int32_t)ADD32(v0, (uint32_t)a0))));
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)SLL32(v0, 6))));
    (v0 = (uint32_t)((int32_t)((int32_t)ADD32(v0, (uint32_t)v1))));
    { const bool c = (int32_t)a0 <= 0; a3 = v0 + 0x4Bu; if (c) goto L_11391ac; }
    (a2 = (uint64_t)((uint64_t)a0 + (uint64_t)0ULL));
L_1139178:;
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)a1, 4))));
    (a2 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)a2, 4294967295))));
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)a1, 8))));
    (a1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)a1, 52))));
    (v0 = (uint32_t)((int32_t)((int32_t)SLL32((uint32_t)a0, 1))));
    { int64_t result = (int64_t)(int32_t)a0 * (int64_t)(int32_t)v1; lo = (uint64_t)(int64_t)(int32_t)result; hi = (uint64_t)(int64_t)(int32_t)(result >> 32); (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)result))); }
    (v0 = (uint32_t)((int32_t)((int32_t)ADD32(v0, (uint32_t)a0))));
    v0 = v0 << 3;
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)SLL32((uint32_t)v1, 6))));
    (v0 = (uint32_t)((int32_t)((int32_t)ADD32(v0, (uint32_t)v1))));
    v0 = a3 + v0;
    { const bool c = (int64_t)a2 != 0; a3 = v0 + 0x33u; if (c) { A3_BACK(); goto L_1139178; } }
L_11391ac:;
    { v0 = a3; return v0; }
    __builtin_unreachable();
}

// manager_sub_01140630 (0x1140630): typed by tools/typed_lift2.py (round A3; loop leaf, extra outputs xo[0]=a1 xo[1]=a3 xo[2]=t0)
inline uint32_t manager_sub_01140630(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint64_t *xo = nullptr)
{
    uint32_t v0 = 0;
    uint32_t v1 = 0;
    uint32_t a0 = 0;
    uint32_t a1 = 0;
    uint64_t a2 = 0;
    uint32_t a3 = 0;
    uint32_t t0 = 0;
    v0 = 0x1190000u;
    (a2 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL));
    a3 = v0 - 0x2DD8u;
    t0 = a3 + 4u;
    (v0 = (uint32_t)((int32_t)((int32_t)SLL32((uint32_t)a2, 4))));
L_1140648:;
    (a2 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)a2, 1))));
    v1 = v0 + t0;
    v0 = v0 + a3;
    a0 = A1_R32(v1);
    { const bool c = a0 == 0; (a1 = (uint32_t)(((int64_t)(int64_t)RC_U64X(a2) < (int64_t)(int32_t)50) ? 1 : 0)); if (c) goto L_1140670; }
    if (a1 != 0) { (v0 = (uint32_t)((int32_t)((int32_t)SLL32((uint32_t)a2, 4)))); { A3_BACK(); goto L_1140648; } }
    v0 = 0u;
L_1140670:;
    { if (xo) { xo[0] = a1_sx64(a1); xo[1] = a1_sx64(a3); xo[2] = a1_sx64(t0); } return v0; }
    __builtin_unreachable();
}

// manager_sub_011408E0 (0x11408e0): typed by tools/typed_lift2.py (round A3; leaf, extra outputs xo[0]=a2 xo[1]=a3 xo[2]=t0 xo[3]=t1)
inline uint64_t manager_sub_011408E0(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint64_t a0, uint32_t a1, uint64_t *xo = nullptr)
{
    uint64_t v0 = 0;
    uint32_t v1 = 0;
    uint32_t a2 = 0;
    uint32_t a3 = 0;
    uint64_t t0 = 0;
    uint64_t t1 = 0;
    bool a3xw1 = false;
    (t1 = (uint64_t)((uint64_t)a0 + (uint64_t)0ULL));
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)281 << 16))));
    (v1 = (uint32_t)((int32_t)((int32_t)A1_R32B((uint32_t)t1, 280))));
    (t0 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL));
    a2 = A1_R32B(a1, 212);
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)SLL32(v1, 3))));
    (a1 = (uint32_t)((int32_t)((int32_t)A1_R32B((uint32_t)a0, -12304))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)SUB32((uint32_t)v0, v1))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)SLL32((uint32_t)v0, 4))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)SUB32((uint32_t)v0, v1))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)SLL32((uint32_t)v0, 2))));
    { const bool c = a1 == 0; (a2 = (uint32_t)((int32_t)((int32_t)ADD32(a2, (uint32_t)v0)))); if (c) goto L_11409a0; }
    v1 = A1_R32B(a2, 12);
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 2))));
    { const bool c = (int64_t)(int32_t)v1 == (int64_t)v0; a3 = 1u; a3xw1 = true; if (c) goto L_114095c; }
    (v0 = (uint64_t)(((uint64_t)a1_sx64(v1) < (uint64_t)(int64_t)(int32_t)3) ? 1 : 0));
    { const bool c = (int64_t)v0 == 0; (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 3)))); if (c) goto L_1140940; }
    { const bool c = v1 == a3; (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 1)))); if (c) goto L_1140950; }
    goto L_1140964;
L_1140940:;
    { const bool c = (int64_t)(int32_t)v1 == (int64_t)v0; (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 1)))); if (c) goto L_114095c; }
    goto L_1140964;
L_1140950:;
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B(a2, 396))));
    { const bool c = (int64_t)v0 != 0; (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 1)))); if (c) goto L_1140964; }
L_114095c:;
    a3 = 0u; a3xw1 = true;
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 1))));
L_1140964:;
    if ((int64_t)(int32_t)a3 != (int64_t)v0) { (t0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 1)))); goto L_11409a0; }
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)281 << 16))));
    (a2 = (uint32_t)((int32_t)((int32_t)A1_R32B((uint32_t)t1, 292))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v0, 4294954996))));
    v1 = 0x1190000u;
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)v0, 0))));
    a1 = A1_R32(v1 - 0x3008u);
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)a0, a2))));
    { const bool c = a1 != a3; A1_W32B((uint32_t)v0, 0, (uint32_t)a0); if (c) goto L_114099c; }
    (v0 = (uint64_t)(((int64_t)(int64_t)RC_U64X(a0) < (int64_t)(int32_t)128) ? 1 : 0));
    if ((int64_t)v0 == 0) goto L_11409a0;
L_114099c:;
    (t0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 1))));
L_11409a0:;
    { (v0 = (uint64_t)((uint64_t)RC_U64X(t0) + (uint64_t)0ULL)); if (xo) { xo[0] = a1_sx64(a2); if (a3xw1) xo[1] = a1_sx64(a3); xo[2] = t0; xo[3] = t1; } return v0; }
    __builtin_unreachable();
}

// manager_sub_01140E60 (0x1140e60): typed by tools/typed_lift2.py (round A3; leaf)
inline uint32_t manager_sub_01140E60(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t a0, uint32_t a1, uint32_t a2)
{
    float f0 = 0.0f;
    float f1 = 0.0f;
    uint32_t v0 = 0;
    uint32_t v1 = 0;
    uint32_t fcr31 = 0;
    v0 = A1_R32B(a0, 272);
    if (v0 != 0) goto L_1140ea0;
    v0 = A1_R32B(a1, 100);
    if (v0 != 0) goto L_1140e98;
    v1 = A1_R32B(a1, 176);
    v0 = 1u;
    if (v1 == v0) goto L_1140e98;
    v1 = A1_R32B(a2, 356);
    v0 = 2u;
    if (v1 != v0) goto L_1140ea0;
L_1140e98:;
    { v0 = 1u; return v0; }
L_1140ea0:;
    { v0 = 0u; return v0; }
    __builtin_unreachable();
}

// manager_sub_01140FD8 (0x1140fd8): typed by tools/typed_lift2.py (round A3; leaf)
inline a1::RetVF manager_sub_01140FD8(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t a0, uint32_t a1)
{
    float f0 = 0.0f;
    float f1 = 0.0f;
    uint32_t at = 0;
    uint32_t v0 = 0;
    uint32_t v1 = 0;
    v1 = A1_R32B(a1, 348);
    v0 = 1u;
    if (v1 == v0) { f0 = a1_flt(A1_R32B(a1, 180)); goto L_1141020; }
    { const bool c = v1 == 0; v0 = 2u; if (c) goto L_1141000; }
    { const bool c = v1 == v0; f0 = a1_flt(A1_R32B(a1, 180)); if (c) goto L_1141018; }
    f1 = a1_flt(A1_R32B(a0, 332));
    goto L_114101c;
L_1141000:;
    at = 0x4B180000u;
    at = at | 0x9680u;
    f0 = a1_flt(at);
    A1_W32B(a0, 336, a1_bits(f0));
    goto L_1141024;
L_1141018:;
    f1 = a1_flt(A1_R32B(a0, 20));
L_114101c:;
    f0 = a1f::add(f0, f1);
L_1141020:;
    A1_W32B(a0, 336, a1_bits(f0));
L_1141024:;
    v1 = A1_R32B(a1, 352);
    v0 = 1u;
    if (v1 == v0) { f0 = a1_flt(A1_R32B(a1, 320)); goto L_1141070; }
    if (v1 == 0) goto L_1141050;
    v0 = 2u;
    { const bool c = v1 == v0; f0 = a1_flt(A1_R32B(a1, 320)); if (c) goto L_1141068; }
    f1 = a1_flt(A1_R32B(a0, 332));
    goto L_114106c;
L_1141050:;
    at = 0xCB180000u;
    at = at | 0x9680u;
    f0 = a1_flt(at);
    A1_W32B(a0, 340, a1_bits(f0));
    goto L_1141074;
L_1141068:;
    f1 = a1_flt(A1_R32B(a0, 20));
L_114106c:;
    f0 = a1f::add(f0, f1);
L_1141070:;
    A1_W32B(a0, 340, a1_bits(f0));
L_1141074:;
    { v0 = 1u; return a1::RetVF{(uint64_t)a1_sx64(v0), f0}; }
    __builtin_unreachable();
}

// manager_sub_011418D8 (0x11418d8): typed by tools/typed_lift2.py (round A3; leaf, extra outputs xo[0]=f0)
inline uint32_t manager_sub_011418D8(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t a0, uint32_t a1, uint32_t a2, uint64_t *xo = nullptr)
{
    float f0 = 0.0f;
    uint32_t v0 = 0;
    uint32_t v1 = 0;
    v1 = A1_R32B(a2, 200);
    v0 = 1u;
    if (v1 == v0) { f0 = a1_flt(A1_R32B(a1, 332)); goto L_1141900; }
    { const bool c = v1 == 0; v0 = 2u; if (c) goto L_11418f8; }
    if (v1 == v0) { v0 = 0x1210000u; goto L_1141908; }
L_11418f8:;
    { return v0; }
L_1141900:;
    { A1_W32B(a0, 4, a1_bits(f0)); if (xo) { xo[0] = a1_bits(f0); } return v0; }
L_1141908:;
    f0 = a1_flt(A1_R32B(v0, 28356));
    A1_W32B(a0, 4, a1_bits(f0));
    { if (xo) { xo[0] = a1_bits(f0); } return v0; }
    __builtin_unreachable();
}

// manager_sub_01141E80 (0x1141e80): typed by tools/typed_lift2.py (round A3; leaf)
inline uint32_t manager_sub_01141E80(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t a0)
{
    uint32_t v0 = 0;
    uint32_t v1 = 0;
    v0 = A1_R32B(a0, 52);
    v1 = 1u;
    { const bool c = v0 != v1; v0 = 2u; if (c) goto L_1141ea0; }
    v1 = A1_R32B(a0, 220);
    { const bool c = v1 == 0; v0 = 1u; if (c) goto L_1141ea0; }
    v0 = 2u;
L_1141ea0:;
    { return v0; }
    __builtin_unreachable();
}

// get_particle_parameter_blend_disp (0x1145ba8): typed by tools/typed_lift2.py (round A3; leaf, extra outputs xo[0]=t0 xo[1]=t1)
inline a1::RetVF get_particle_parameter_blend_disp(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t a0, uint32_t a1, uint64_t a2, uint64_t a3, float f12, uint64_t *xo = nullptr)
{
    float f0 = 0.0f;
    float f1 = 0.0f;
    float f2 = 0.0f;
    float f3 = 0.0f;
    uint32_t at = 0;
    uint32_t v0 = 0;
    uint32_t v1 = 0;
    uint32_t t0 = 0;
    uint32_t t1 = 0;
    uint32_t fcr31 = 0;
    (v0 = (uint32_t)((int32_t)((int32_t)SLL32((uint32_t)a3, 3))));
    v1 = A1_R32B(a1, 212);
    (v0 = (uint32_t)((int32_t)((int32_t)SUB32(v0, (uint32_t)a3))));
    v0 = v0 << 4;
    (v0 = (uint32_t)((int32_t)((int32_t)SUB32(v0, (uint32_t)a3))));
    t0 = v0 << 2;
    { const bool c = (int64_t)a2 == 0; t1 = v1 + t0; if (c) goto L_1145c60; }
    (a1 = (uint32_t)((int32_t)((int32_t)A1_R32B((uint32_t)a2, 208))));
    v0 = a1 - 1u;
    (v0 = (uint32_t)(((int64_t)(int64_t)a1_sx64(v0) < (int64_t)(int64_t)a3) ? 1 : 0));
    if (v0 == 0) { (v0 = (uint32_t)((int32_t)((int32_t)A1_R32B((uint32_t)a2, 212)))); goto L_1145c00; }
    v0 = a1 << 3;
    (v1 = (uint32_t)((int32_t)((int32_t)A1_R32B((uint32_t)a2, 212))));
    v0 = v0 - a1;
    v0 = v0 << 4;
    v0 = v0 - a1;
    v0 = v0 << 2;
    v1 = v1 + v0;
    v0 = v1 - 0x1BCu;
    goto L_1145c04;
L_1145c00:;
    v0 = v0 + t0;
L_1145c04:;
    f1 = a1_flt(0u);
    fcr31 = (a1f::lt(f12, f1)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
    { const bool c = (fcr31 & 0x800000u) == 0u; f3 = ((float)(f1)); if (c) goto L_1145c28; }
    (at = (uint32_t)((int32_t)((int32_t)((uint32_t)16256 << 16))));
    f0 = a1_flt(at);
    f2 = a1f::add(f12, f0);
    goto L_1145c44;
L_1145c28:;
    (at = (uint32_t)((int32_t)((int32_t)((uint32_t)16256 << 16))));
    f0 = a1_flt(at);
    fcr31 = (a1f::lt(f12, f0)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
    if ((fcr31 & 0x800000u) == 0u) { f2 = ((float)(f1)); goto L_1145c40; }
    f2 = a1f::sub(f0, f12);
L_1145c40:;
    f3 = ((float)(f12));
L_1145c44:;
    f1 = a1_flt(A1_R32B(t1, 384));
    f0 = a1_flt(A1_R32B(v0, 384));
    f1 = a1f::mul(f2, f1);
    f0 = a1f::mul(f3, f0);
    f1 = a1f::addFinite(f1, f0);
    { A1_W32(a0, a1_bits(f1)); if (xo) { xo[0] = a1_sx64(t0); xo[1] = a1_sx64(t1); } return a1::RetVF{(uint64_t)a1_sx64(v0), f0}; }
L_1145c60:;
    f0 = a1_flt(A1_R32B(t1, 384));
    { A1_W32(a0, a1_bits(f0)); if (xo) { xo[0] = a1_sx64(t0); xo[1] = a1_sx64(t1); } return a1::RetVF{(uint64_t)a1_sx64(v0), f0}; }
    __builtin_unreachable();
}

// InitTwoWayLink (0x1147a78): typed by tools/typed_lift2.py (round A3; leaf)
inline void InitTwoWayLink(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t a0, uint32_t a1)
{
    A1_W32B(a0, 8, a1);
    A1_W32B(a0, 4, 0u);
    { A1_W32(a0, 0u); return; }
    __builtin_unreachable();
}

// AddTwoWayLinkNext (0x1147a88): typed by tools/typed_lift2.py (round A3; leaf)
inline uint32_t AddTwoWayLinkNext(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t a0, uint32_t a1)
{
    uint32_t v0 = 0;
    v0 = A1_R32B(a0, 4);
    A1_W32(a1, a0);
    A1_W32B(a1, 4, v0);
    v0 = A1_R32B(a0, 4);
    if (v0 != 0) { A1_W32(v0, a1); goto L_1147aa0; }
L_1147aa0:;
    A1_W32B(a0, 4, a1);
    { v0 = 1u; return v0; }
    __builtin_unreachable();
}

// DeleteTwoWayLink (0x1147ab0): typed by tools/typed_lift2.py (round A3; leaf)
inline uint32_t DeleteTwoWayLink(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t a0)
{
    uint32_t v0 = 0;
    uint32_t v1 = 0;
    v1 = A1_R32B(a0, 4);
    if (v1 == 0) { v1 = A1_R32(a0); goto L_1147ac8; }
    v0 = A1_R32(a0);
    A1_W32(v1, v0);
    v1 = A1_R32(a0);
L_1147ac8:;
    if (v1 == 0) { A1_W32B(a0, 4, 0u); goto L_1147adc; }
    v0 = A1_R32B(a0, 4);
    A1_W32B(v1, 4, v0);
    A1_W32B(a0, 4, 0u);
L_1147adc:;
    v0 = 1u;
    { A1_W32(a0, 0u); return v0; }
    __builtin_unreachable();
}

// adpcmSetPan (0x1157a98): typed by tools/typed_lift2.py (round A3; leaf, extra outputs xo[0]=a0)
inline uint32_t adpcmSetPan(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t a0, float f12, uint64_t *xo = nullptr)
{
    uint32_t at = 0;
    uint32_t v0 = 0;
    uint64_t hi = 0;
    uint64_t lo = 0;
    v0 = 0x560u;
    { int64_t result = (int64_t)(int32_t)a0 * (int64_t)(int32_t)v0; lo = (uint64_t)(int64_t)(int32_t)result; hi = (uint64_t)(int64_t)(int32_t)(result >> 32); (a0 = (uint32_t)((int32_t)((int32_t)result))); }
    at = 0x1220000u;
    at = at + a0;
    A1_W32(at - 0x42C0u, a1_bits(f12));
    { if (xo) { xo[0] = a1_sx64(a0); } return v0; }
    __builtin_unreachable();
}

// dsysFileInit (0x115a060): typed by tools/typed_lift2.py (round A3; loop leaf, extra outputs xo[0]=t0 xo[1]=t1)
inline uint32_t dsysFileInit(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint64_t *xo = nullptr)
{
    uint32_t v0 = 0;
    uint32_t v1 = 0;
    uint32_t a0 = 0;
    uint64_t a1 = 0;
    uint64_t a2 = 0;
    uint32_t a3 = 0;
    uint32_t t0 = 0;
    uint64_t t1 = 0;
    v0 = 0x1220000u;
    a0 = 0x1220000u;
    v0 = v0 - 0x28D4u;
    v1 = 0x1190000u;
    A1_W32(a0 - 0x28D0u, 0u);
    a3 = v1 - 0x280u;
    A1_W32(v0, 0u);
    (t1 = (uint64_t)((uint64_t)a1_sx64(v0) + (uint64_t)0ULL));
    (a1 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL));
    t0 = a3 + 0xCu;
    (a2 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL));
L_115a090:;
    (v0 = (uint32_t)((int32_t)((int32_t)SLL32((uint32_t)a1, 4))));
    (a1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)a1, 1))));
    v1 = v0 + t0;
    (a0 = (uint32_t)(((int64_t)(int64_t)RC_U64X(a1) < (int64_t)(int32_t)8) ? 1 : 0));
    A1_W32B(v1, 0, (uint32_t)a2);
    { const bool c = a0 != 0; (a2 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(v0, a3)))); if (c) { A3_BACK(); goto L_115a090; } }
    { A1_W32B((uint32_t)t1, 0, (uint32_t)a2); if (xo) { xo[0] = a1_sx64(t0); xo[1] = t1; } return v0; }
    __builtin_unreachable();
}

// sheetFileGetDataAdrs (0x115cf90): typed by tools/typed_lift2.py (round A3; leaf, extra outputs xo[0]=a1)
inline uint32_t sheetFileGetDataAdrs(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t a0, uint64_t *xo = nullptr)
{
    uint32_t v0 = 0;
    uint32_t v1 = 0;
    uint32_t a1 = 0;
    uint64_t hi = 0;
    uint64_t lo = 0;
    v0 = A1_R32B(a0, 4);
    a1 = A1_R32B(v0, 16);
    if (a1 == 0) goto L_115cfb4;
    v0 = A1_R32B(v0, 8);
    a0 = A1_R32B(a0, 8);
    v1 = A1_R32B(v0, 12);
    { int64_t result = (int64_t)(int32_t)v1 * (int64_t)(int32_t)a0; lo = (uint64_t)(int64_t)(int32_t)result; hi = (uint64_t)(int64_t)(int32_t)(result >> 32); (v0 = (uint32_t)((int32_t)((int32_t)result))); }
    a1 = v0 + a1;
L_115cfb4:;
    { v0 = a1; if (xo) { xo[0] = a1_sx64(a1); } return v0; }
    __builtin_unreachable();
}

// sheetMgrSheetHeaderSearchSheetDataObj (0x115ec60): typed by tools/typed_lift2.py (round A3; loop leaf)
inline uint32_t sheetMgrSheetHeaderSearchSheetDataObj(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t a0, uint64_t a1)
{
    uint32_t v0 = 0;
    uint32_t v1 = 0;
    a0 = A1_R32B(a0, 64);
L_115ec68:;
    if (a0 == 0) goto L_115ec80;
    v0 = A1_R32B(a0, 56);
    v1 = A1_R32B(v0, 4);
    if ((int64_t)a1 != (int64_t)(int32_t)v1) { a0 = A1_R32B(a0, 84); { A3_BACK(); goto L_115ec68; } }
L_115ec80:;
    { v0 = a0; return v0; }
    __builtin_unreachable();
}

// manager_sub_011629E8 (0x11629e8): typed by tools/typed_lift2.py (round A3; loop leaf, extra outputs xo[0]=a2)
inline uint32_t manager_sub_011629E8(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint64_t a0, uint64_t *xo = nullptr)
{
    uint32_t v0 = 0;
    uint32_t v1 = 0;
    uint32_t a1 = 0;
    uint64_t a2 = 0;
    v0 = 0x1230000u;
    (a2 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL));
    a1 = v0 - 0x4FF0u;
L_11629f8:;
    v0 = A1_R32(a1);
    if (v0 == 0) { (a2 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)a2, 1)))); goto L_1162a1c; }
    v1 = A1_R32B(v0, 16);
    v1 = v1 >> 3;
    v1 = v1 & 7u;
    { const bool c = (int64_t)(int32_t)v1 == (int64_t)a0; v0 = a1; if (c) goto L_1162a2c; }
    (a2 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)a2, 1))));
L_1162a1c:;
    (v0 = (uint32_t)(((int64_t)(int64_t)RC_U64X(a2) < (int64_t)(int32_t)10) ? 1 : 0));
    { const bool c = v0 != 0; a1 = a1 + 0xC0u; if (c) { A3_BACK(); goto L_11629f8; } }
    v0 = 0u;
L_1162a2c:;
    { if (xo) { xo[0] = a2; } return v0; }
    __builtin_unreachable();
}

// get_hit_region_ofst_camera_region (0x1163848): typed by tools/typed_lift2.py (round A3; leaf, extra outputs xo[0]=v0)
inline float get_hit_region_ofst_camera_region(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint64_t a0, uint64_t *xo = nullptr)
{
    float f0 = 0.0f;
    uint32_t v0 = 0;
    uint64_t v1 = 0;
    bool a3xw0 = false;
    { const bool c = (int64_t)a0 == 0; (v1 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL)); if (c) goto L_1163860; }
    (v0 = (uint32_t)((int32_t)((int32_t)A1_R32B((uint32_t)a0, 20)))); a3xw0 = true;
    { const bool c = v0 != 0; (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 1)))); if (c) goto L_1163860; }
    (v1 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL));
L_1163860:;
    if ((int64_t)v1 == 0) goto L_1163878;
    (v0 = (uint32_t)((int32_t)((int32_t)A1_R32B((uint32_t)a0, 20)))); a3xw0 = true;
    { f0 = a1_flt(A1_R32B(v0, 20)); if (xo) { xo[0] = a1_sx64(v0); } return f0; }
L_1163878:;
    { uint32_t bits = 0u; std::memcpy(&f0, &bits, sizeof(bits)); }
    { if (xo) { if (a3xw0) xo[0] = a1_sx64(v0); } return f0; }
    __builtin_unreachable();
}

// set_scissor_screen_size (0x11644d0): typed by tools/typed_lift2.py (round A3; leaf, extra outputs xo[0]=a1)
inline uint32_t set_scissor_screen_size(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint64_t a0, uint32_t a1, uint32_t a2, uint32_t a3, uint64_t *xo = nullptr)
{
    uint32_t v0 = 0;
    a1 = a1 << 2;
    a1 = a1 + 0x1D0u;
    (v0 = (uint32_t)((int32_t)((int32_t)ADD32((uint32_t)a0, a1))));
    (a0 = (uint64_t)((uint64_t)a1_sx64(v0) + (uint64_t)0ULL));
    A1_W32B((uint32_t)a0, 0, a2);
    { A1_W32B(v0, 8, a3); if (xo) { xo[0] = a1_sx64(a1); } return v0; }
    __builtin_unreachable();
}

// DecreaseDamageLockTime (0x1166e50): typed by tools/typed_lift2.py (round A3; leaf, extra outputs xo[0]=f0)
inline uint32_t DecreaseDamageLockTime(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t a0, float f12, uint64_t *xo = nullptr)
{
    float f0 = 0.0f;
    float f1 = 0.0f;
    uint32_t v0 = 0;
    uint32_t fcr31 = 0;
    bool a3xw0 = false;
    v0 = A1_R32B(a0, 84);
    A1_W32B(a0, 100, 0u);
    { const bool c = (int32_t)v0 < 0; A1_W32(a0, 0u); if (c) goto L_1166e90; }
    f0 = a1_flt(A1_R32B(a0, 88)); a3xw0 = true;
    f1 = a1_flt(A1_R32B(a0, 100));
    f0 = a1f::sub(f0, f12); a3xw0 = true;
    fcr31 = (a1f::lt(f0, f1)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
    { const bool c = (fcr31 & 0x800000u) == 0u; A1_W32B(a0, 88, a1_bits(f0)); if (c) goto L_1166e90; }
    v0 = 0xFFFFFFFFu;
    A1_W32B(a0, 104, a1_bits(f1));
    A1_W32B(a0, 88, a1_bits(f1));
    A1_W32B(a0, 92, a1_bits(f1));
    A1_W32B(a0, 96, v0);
    A1_W32B(a0, 84, v0);
L_1166e90:;
    { if (xo) { if (a3xw0) xo[0] = a1_bits(f0); } return v0; }
    __builtin_unreachable();
}

// GetDamagerClassDamageSheetType (0x1166e98): typed by tools/typed_lift2.py (round A3; leaf, extra outputs xo[0]=a1)
inline uint32_t GetDamagerClassDamageSheetType(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t a0, uint64_t *xo = nullptr)
{
    uint32_t v0 = 0;
    uint32_t v1 = 0;
    uint32_t a1 = 0;
    v0 = 0x1190000u;
    a1 = A1_R32B(a0, 80);
    v1 = A1_R32(v0 - 0x188u);
    v1 = v1 << 3;
    v1 = v1 + a1;
    { v0 = A1_R32B(v1, 16); if (xo) { xo[0] = a1_sx64(a1); } return v0; }
    __builtin_unreachable();
}

// ResetAnimeFrameTableInfo (0x11692a8): typed by tools/typed_lift2.py (round A3; leaf, extra outputs xo[0]=a2 xo[1]=a3 xo[2]=t0)
inline uint64_t ResetAnimeFrameTableInfo(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint64_t a0, uint64_t a1, uint64_t a2, uint64_t *xo = nullptr)
{
    uint64_t v0 = 0;
    uint32_t v1 = 0;
    uint32_t a3 = 0;
    uint64_t t0 = 0;
    uint64_t hi = 0;
    uint64_t lo = 0;
    bool a3xw0 = false;
    bool a3xw1 = false;
    (t0 = (uint64_t)((uint64_t)a0 + (uint64_t)0ULL));
    { const bool c = (int64_t)a1 == 0; A1_W32B((uint32_t)t0, 0, (uint32_t)a1); if (c) goto L_1169310; }
    (a3 = (uint32_t)((int32_t)((int32_t)A1_R32B((uint32_t)a1, 0)))); a3xw1 = true;
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)3 << 16))));
    (v0 = (uint64_t)(RC_U64X(v0) | (uint64_t)(uint16_t)65535));
    (v0 = (uint64_t)(a1_sx64(a3) & RC_U64X(v0)));
    { const bool c = (int64_t)v0 == 0; (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)SUB32((uint32_t)a1, (uint32_t)v0)))); if (c) goto L_11692d8; }
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)v0, 0))));
    goto L_11692dc;
L_11692d8:;
    (a0 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL));
L_11692dc:;
    { const bool c = (int64_t)a0 == 0; { const uint32_t _dynInsn = A1_R32(0x11692E0u); const int32_t _dynSimm = (int16_t)(_dynInsn & 0xFFFFu); const uint32_t _dynImm = _dynInsn & 0xFFFFu; (void)_dynSimm; (void)_dynImm; (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)_dynImm << 16)))); } if (c) goto L_1169310; }
    { const uint32_t _dynInsn = A1_R32(0x11692E4u); const int32_t _dynSimm = (int16_t)(_dynInsn & 0xFFFFu); const uint32_t _dynImm = _dynInsn & 0xFFFFu; (void)_dynSimm; (void)_dynImm; (v1 = (uint32_t)((int32_t)((int32_t)A1_R32(ADD32((uint32_t)v0, _dynSimm))))); }
    { const bool c = (int64_t)a0 == (int64_t)(int32_t)v1; (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)SRL32(a3, 18)))); if (c) goto L_1169310; }
    (a1 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)a0, 8))));
    (v0 = (uint64_t)(RC_U64X(v0) & (uint64_t)(uint16_t)4095));
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)a0, 16))));
    (v1 = (uint32_t)((int32_t)((int32_t)A1_R32B((uint32_t)a1, 12))));
    { int64_t result = (int64_t)(int32_t)v0 * (int64_t)(int32_t)v1; lo = (uint64_t)(int64_t)(int32_t)result; hi = (uint64_t)(int64_t)(int32_t)(result >> 32); (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)result))); }
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)a0, (uint32_t)v0))));
    goto L_1169314;
L_1169310:;
    (v0 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL));
L_1169314:;
    A1_W32B((uint32_t)t0, 4, (uint32_t)v0);
    { const bool c = (int64_t)a2 == 0; A1_W32B((uint32_t)t0, 8, (uint32_t)a2); if (c) goto L_1169378; }
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)a2, 0))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)3 << 16))));
    (v0 = (uint64_t)(RC_U64X(v0) | (uint64_t)(uint16_t)65535));
    (v0 = (uint64_t)(RC_U64X(a0) & RC_U64X(v0)));
    { const bool c = (int64_t)v0 == 0; (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)SUB32((uint32_t)a2, (uint32_t)v0)))); if (c) goto L_1169340; }
    (a2 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)v0, 0)))); a3xw0 = true;
    goto L_1169344;
L_1169340:;
    (a2 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL)); a3xw0 = true;
L_1169344:;
    { const bool c = (int64_t)a2 == 0; { const uint32_t _dynInsn = A1_R32(0x1169348u); const int32_t _dynSimm = (int16_t)(_dynInsn & 0xFFFFu); const uint32_t _dynImm = _dynInsn & 0xFFFFu; (void)_dynSimm; (void)_dynImm; (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)_dynImm << 16)))); } if (c) goto L_1169378; }
    { const uint32_t _dynInsn = A1_R32(0x116934Cu); const int32_t _dynSimm = (int16_t)(_dynInsn & 0xFFFFu); const uint32_t _dynImm = _dynInsn & 0xFFFFu; (void)_dynSimm; (void)_dynImm; (v1 = (uint32_t)((int32_t)((int32_t)A1_R32(ADD32((uint32_t)v0, _dynSimm))))); }
    { const bool c = (int64_t)a2 == (int64_t)(int32_t)v1; (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)SRL32((uint32_t)a0, 18)))); if (c) goto L_1169378; }
    (a1 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)a2, 8))));
    (v0 = (uint64_t)(RC_U64X(v0) & (uint64_t)(uint16_t)4095));
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)a2, 16))));
    (v1 = (uint32_t)((int32_t)((int32_t)A1_R32B((uint32_t)a1, 12))));
    { int64_t result = (int64_t)(int32_t)v0 * (int64_t)(int32_t)v1; lo = (uint64_t)(int64_t)(int32_t)result; hi = (uint64_t)(int64_t)(int32_t)(result >> 32); (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)result))); }
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)a0, (uint32_t)v0))));
    goto L_116937c;
L_1169378:;
    (v0 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL));
L_116937c:;
    { A1_W32B((uint32_t)t0, 12, (uint32_t)v0); if (xo) { if (a3xw0) xo[0] = a2; if (a3xw1) xo[1] = a1_sx64(a3); xo[2] = t0; } return v0; }
    __builtin_unreachable();
}

// rintf (0x116ad80): typed by tools/typed_lift2.py (round A3; frame leaf, extra outputs xo[0]=f12 xo[1]=a0 xo[2]=a1 xo[3]=a2 xo[4]=a3 xo[5]=t0 xo[6]=t1, frame 16 bytes, 4 slot accesses)
inline a1::RetVF rintf(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, float f12, uint64_t *xo = nullptr)
{
    float f0 = 0.0f;
    float f1 = 0.0f;
    uint32_t at = 0;
    uint64_t v0 = 0;
    uint64_t v1 = 0;
    uint64_t a0 = 0;
    uint32_t a1 = 0;
    uint64_t a2 = 0;
    uint64_t a3 = 0;
    uint32_t t0 = 0;
    uint32_t t1 = 0;
    uint8_t a2fr[16];
    bool a3xw0 = false;
    { uint32_t bits; std::memcpy(&bits, &f12, sizeof(bits)); (v0 = (uint64_t)((int64_t)(int32_t)(bits))); }
    (a2 = (uint64_t)((uint64_t)RC_U64X(v0) + (uint64_t)0ULL));
    (t0 = (uint32_t)((int32_t)((int32_t)SRL32((uint32_t)a2, 31))));
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)127 << 16))));
    t1 = 0x7FFF0000u;
    (a0 = (uint64_t)(RC_U64X(a0) | (uint64_t)(uint16_t)65535));
    t1 = t1 | 0xFFFFu;
    (v0 = (uint64_t)(RC_U64X(a2) & a1_sx64(t1)));
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)SRL32((uint32_t)v0, 23))));
    (a3 = (uint64_t)(((uint64_t)RC_U64X(a0) < (uint64_t)RC_U64X(v0)) ? 1 : 0));
    (a1 = (uint32_t)((int32_t)((int32_t)ADD32((uint32_t)v1, 4294967169))));
    (v0 = (uint64_t)(((int64_t)(int64_t)a1_sx64(a1) < (int64_t)(int32_t)23) ? 1 : 0));
    { const bool c = (int64_t)v0 == 0; f0 = ((float)(f12)); if (c) goto L_116ae78; }
    if ((int64_t)a3 == 0) goto L_116ae78;
    { const bool c = (int32_t)a1 >= 0; (v1 = (uint64_t)((int64_t)(int32_t)(SRA32((int32_t)a0, a1 & 0x1F)))); if (c) goto L_116ae30; }
    (v0 = (uint64_t)(RC_U64X(a2) & RC_U64X(a0)));
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)64 << 16))));
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)SUB32(0u, (uint32_t)v0))));
    a1 = 0xFFF00000u;
    (v0 = (uint64_t)(RC_U64X(v0) | RC_U64X(v1)));
    (a2 = (uint64_t)(RC_U64X(a2) & a1_sx64(a1)));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)SRL32((uint32_t)v0, 9))));
    (v0 = (uint64_t)(RC_U64X(v0) & RC_U64X(a0)));
    (a2 = (uint64_t)(RC_U64X(a2) | RC_U64X(v0)));
    { uint32_t bits = (uint32_t)a2; std::memcpy(&f12, &bits, sizeof(bits)); } a3xw0 = true;
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)SLL32(t0, 2))));
    at = 0x1190000u;
    (at = (uint32_t)((int32_t)((int32_t)ADD32(at, (uint32_t)v1))));
    f1 = a1_flt(A1_R32B(at, 27488));
    f0 = a1f::add(f1, f12);
    a2::frSt<uint32_t>(a2fr, 0, a1_bits(f0));
    f0 = a1f::sub(f0, f1);
    a2::frSt<uint32_t>(a2fr, 4, a1_bits(f0));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)a2::frLd<uint32_t>(a2fr, 4))));
    (v1 = (uint64_t)(RC_U64X(v0) & a1_sx64(t1)));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)SLL32(t0, 31))));
    (v1 = (uint64_t)(RC_U64X(v1) | RC_U64X(v0)));
    { uint32_t bits = (uint32_t)v1; std::memcpy(&f0, &bits, sizeof(bits)); }
    goto L_116ae78;
L_116ae30:;
    (v0 = (uint64_t)(RC_U64X(a2) & RC_U64X(v1)));
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)SRL32((uint32_t)v1, 1))));
    { const bool c = (int64_t)v0 == 0; (a0 = (uint64_t)(RC_U64X(a2) & RC_U64X(v1))); if (c) goto L_116ae78; }
    { const bool c = (int64_t)a0 == 0; (v1 = (uint64_t)(~(0ULL | RC_U64X(v1)))); if (c) goto L_116ae58; }
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)16 << 16))));
    (v1 = (uint64_t)(RC_U64X(a2) & v1));
    (v0 = (uint64_t)((int64_t)(int32_t)(SRA32((int32_t)v0, a1 & 0x1F))));
    (a2 = (uint64_t)(v1 | RC_U64X(v0)));
L_116ae58:;
    { uint32_t bits = (uint32_t)a2; std::memcpy(&f12, &bits, sizeof(bits)); } a3xw0 = true;
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)SLL32(t0, 2))));
    at = 0x1190000u;
    (at = (uint32_t)((int32_t)((int32_t)ADD32(at, (uint32_t)v0))));
    f1 = a1_flt(A1_R32B(at, 27488));
    f0 = a1f::add(f1, f12);
    a2::frSt<uint32_t>(a2fr, 0, a1_bits(f0));
    f0 = a1f::sub(f0, f1);
L_116ae78:;
    { if (xo) { if (a3xw0) xo[0] = a1_bits(f12); xo[1] = a0; xo[2] = a1_sx64(a1); xo[3] = a2; xo[4] = a3; xo[5] = a1_sx64(t0); xo[6] = a1_sx64(t1); } return a1::RetVF{(uint64_t)v0, f0}; }
    __builtin_unreachable();
}

// GAMEUTIL_set_parameter_look_at_vs_offset (0x1343eb8): typed by tools/typed_lift2.py (round A3; leaf, extra outputs xo[0]=f12 xo[1]=f13 xo[2]=f14)
inline float GAMEUTIL_set_parameter_look_at_vs_offset(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t a0, float f12, float f13, float f14, float f15, float f16, uint64_t *xo = nullptr)
{
    float f0 = 0.0f;
    uint32_t at = 0;
    at = 0x3C8E0000u;
    at = at | 0xFA35u;
    f0 = a1_flt(at);
    A1_W32B(a0, 76, a1_bits(f16));
    f14 = a1f::mul(f14, f0);
    A1_W32B(a0, 72, a1_bits(f15));
    f12 = a1f::mul(f12, f0);
    f13 = a1f::mul(f13, f0);
    A1_W32B(a0, 68, a1_bits(f14));
    A1_W32B(a0, 60, a1_bits(f12));
    { A1_W32B(a0, 64, a1_bits(f13)); if (xo) { xo[0] = a1_bits(f12); xo[1] = a1_bits(f13); xo[2] = a1_bits(f14); } return f0; }
    __builtin_unreachable();
}

// GAMEUTIL_doit_status_manager (0x13445d8): typed by tools/typed_lift2.py (round A3; leaf, extra outputs xo[0]=f0)
inline uint32_t GAMEUTIL_doit_status_manager(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t a0, uint64_t a1, uint64_t a2, float f12, uint64_t *xo = nullptr)
{
    float f0 = 0.0f;
    uint32_t v0 = 0;
    bool a3xw0 = false;
    v0 = 1u;
    if ((int64_t)a2 != (int64_t)(int32_t)v0) { v0 = A1_R32(a0); goto L_13445f8; }
    A1_W32B(a0, 0, (uint32_t)a1);
    A1_W32B(a0, 12, 0u);
    A1_W32B(a0, 8, (uint32_t)a1);
    { A1_W32B(a0, 4, (uint32_t)a1); return v0; }
L_13445f8:;
    if ((int64_t)(int32_t)v0 != (int64_t)a1) { A1_W32B(a0, 12, 0u); goto L_1344610; }
    f0 = a1_flt(A1_R32B(a0, 12)); a3xw0 = true;
    f0 = a1f::add(f0, f12); a3xw0 = true;
    A1_W32B(a0, 12, a1_bits(f0));
    goto L_1344614;
L_1344610:;
    A1_W32B(a0, 8, v0);
L_1344614:;
    A1_W32B(a0, 0, (uint32_t)a1);
    { A1_W32B(a0, 4, v0); if (xo) { if (a3xw0) xo[0] = a1_bits(f0); } return v0; }
    __builtin_unreachable();
}

// ReleaseGame (0x1345ce0): typed by tools/typed_lift2.py (round A3; leaf, extra outputs xo[0]=a0)
inline uint32_t ReleaseGame(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint64_t a0, uint64_t *xo = nullptr)
{
    uint32_t v0 = 0;
    uint64_t v1 = 0;
    v0 = 0x1460000u;
    (a0 = (uint64_t)(~(0ULL | a0)));
    v0 = v0 + 0x5650u;
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B(v0, 428))));
    (v1 = (uint64_t)(RC_U64X(v1) & a0));
    { A1_W32B(v0, 428, (uint32_t)v1); if (xo) { xo[0] = a0; } return v0; }
    __builtin_unreachable();
}

// getDemoBarFade (0x1346300): typed by tools/typed_lift2.py (round A3; leaf, extra outputs xo[0]=f1)
inline a1::RetVF getDemoBarFade(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint64_t *xo = nullptr)
{
    float f0 = 0.0f;
    float f1 = 0.0f;
    uint32_t at = 0;
    uint32_t v0 = 0;
    v0 = 0x1460000u;
    at = 0x3F800000u;
    f1 = a1_flt(at);
    f0 = a1_flt(A1_R32B(v0, 23144));
    { f0 = a1f::sub(f1, f0); if (xo) { xo[0] = a1_bits(f1); } return a1::RetVF{(uint64_t)a1_sx64(v0), f0}; }
    __builtin_unreachable();
}

// set_base_pitch_parameter_nico_yaw_pitch_cntrl (0x1347028): typed by tools/typed_lift2.py (round A3; leaf, extra outputs xo[0]=f14 xo[1]=f15 xo[2]=f16)
inline float set_base_pitch_parameter_nico_yaw_pitch_cntrl(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t a0, uint32_t a1, float f12, float f13, float f14, float f15, float f16, float f17, uint64_t *xo = nullptr)
{
    float f0 = 0.0f;
    uint32_t at = 0;
    at = 0x3C8E0000u;
    at = at | 0xFA35u;
    f0 = a1_flt(at);
    A1_W32B(a0, 256, a1_bits(f17));
    f16 = a1f::mul(f16, f0);
    A1_W32B(a0, 232, a1);
    f12 = a1f::mul(f12, f0);
    f13 = a1f::mul(f13, f0);
    f14 = a1f::mul(f14, f0);
    f15 = a1f::mul(f15, f0);
    A1_W32B(a0, 252, a1_bits(f16));
    A1_W32B(a0, 236, a1_bits(f12));
    A1_W32B(a0, 240, a1_bits(f13));
    A1_W32B(a0, 244, a1_bits(f14));
    { A1_W32B(a0, 248, a1_bits(f15)); if (xo) { xo[0] = a1_bits(f14); xo[1] = a1_bits(f15); xo[2] = a1_bits(f16); } return f0; }
    __builtin_unreachable();
}

// gamecore_sub_01347B28 (0x1347b28): typed by tools/typed_lift2.py (round A3; loop leaf, extra outputs xo[0]=f0)
inline uint64_t gamecore_sub_01347B28(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t a0, float f12, float f13, uint64_t *xo = nullptr)
{
    float f0 = 0.0f;
    float f1 = 0.0f;
    uint64_t v0 = 0;
    uint64_t v1 = 0;
    uint32_t a1 = 0;
    bool a3xw0 = false;
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B(a0, 0))));
    (v0 = (uint64_t)(((int64_t)(int64_t)RC_U64X(v1) < (int64_t)(int32_t)5) ? 1 : 0));
    { const bool c = (int64_t)v0 == 0; (a1 = (uint32_t)((int32_t)((int32_t)ADD32((uint32_t)v1, 4294967295)))); if (c) goto L_1347b48; }
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v1, 1))));
    A1_W32B(a0, 0, (uint32_t)v0);
    (v1 = (uint64_t)((uint64_t)RC_U64X(v0) + (uint64_t)0ULL));
    (a1 = (uint32_t)((int32_t)((int32_t)ADD32((uint32_t)v1, 4294967295))));
L_1347b48:;
    if ((int32_t)a1 <= 0) { A1_W32B(a0, 8, a1_bits(f13)); goto L_1347b80; }
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(a0, 4))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)SLL32(a1, 3))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v0, (uint32_t)v1))));
L_1347b60:;
    { uint32_t bits = A1_R32B((uint32_t)v0, -8); float f; std::memcpy(&f, &bits, sizeof(f)); f1 = f; }
    a1 = a1 - 1u;
    { uint32_t bits = A1_R32B((uint32_t)v0, -4); float f; std::memcpy(&f, &bits, sizeof(f)); f0 = f; } a3xw0 = true;
    { float f = f1; uint32_t bits; std::memcpy(&bits, &f, sizeof(bits)); A1_W32B((uint32_t)v0, 0, bits); }
    { float f = f0; uint32_t bits; std::memcpy(&bits, &f, sizeof(bits)); A1_W32B((uint32_t)v0, 4, bits); }
    { const bool c = (int32_t)a1 > 0; (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v0, 4294967288)))); if (c) { A3_BACK(); goto L_1347b60; } }
    A1_W32B(a0, 8, a1_bits(f13));
L_1347b80:;
    { A1_W32B(a0, 4, a1_bits(f12)); if (xo) { if (a3xw0) xo[0] = a1_bits(f0); } return v0; }
    __builtin_unreachable();
}

// gamecore_sub_01347B88 (0x1347b88): typed by tools/typed_lift2.py (round A3; loop leaf, extra outputs xo[0]=v0 xo[1]=a3)
inline float gamecore_sub_01347B88(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t a0, uint32_t a1, uint32_t a2, uint64_t *xo = nullptr)
{
    float f0 = 0.0f;
    float f1 = 0.0f;
    float f2 = 0.0f;
    float f3 = 0.0f;
    uint32_t v0 = 0;
    uint64_t v1 = 0;
    uint32_t a3 = 0;
    uint32_t fcr31 = 0;
    bool a3xw0 = false;
    { uint32_t bits = 0u; std::memcpy(&f3, &bits, sizeof(bits)); }
    a3 = A1_R32(a2);
    { const bool c = (int32_t)a3 <= 0; f2 = ((float)(f3)); if (c) goto L_1347bbc; }
    v0 = a2 + 4u; a3xw0 = true;
    (v1 = (uint64_t)((uint64_t)a1_sx64(a3) + (uint64_t)0ULL));
L_1347ba0:;
    f1 = a1_flt(A1_R32(v0));
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v1, 4294967295))));
    f0 = a1_flt(A1_R32B(v0, 4));
    v0 = v0 + 8u; a3xw0 = true;
    f2 = a1f::add(f2, f1);
    { const bool c = (int64_t)v1 != 0; f3 = a1f::add(f3, f0); if (c) { A3_BACK(); goto L_1347ba0; } }
L_1347bbc:;
    f0 = a1_flt(a3);
    { int32_t tmp; std::memcpy(&tmp, &f0, sizeof(tmp)); f0 = ((float)(int32_t)(tmp)); }
    f0 = a1f::div(fcr31, f2, f0);
    A1_W32(a0, a1_bits(f0));
    f1 = a1_flt(A1_R32(a2));
    { int32_t tmp; std::memcpy(&tmp, &f1, sizeof(tmp)); f1 = ((float)(int32_t)(tmp)); }
    f1 = a1f::div(fcr31, f3, f1);
    { A1_W32(a1, a1_bits(f1)); if (xo) { if (a3xw0) xo[0] = a1_sx64(v0); xo[1] = a1_sx64(a3); } return f0; }
    __builtin_unreachable();
}

// backStageCheckPointSave (0x1349318): typed by tools/typed_lift2.py (round A3; loop leaf, extra outputs xo[0]=a0 xo[1]=a1 xo[2]=a2 xo[3]=a3 xo[4]=t0)
inline uint64_t backStageCheckPointSave(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint64_t a0, uint64_t *xo = nullptr)
{
    uint64_t v0 = 0;
    uint64_t v1 = 0;
    uint64_t a1 = 0;
    uint64_t a2 = 0;
    uint64_t a3 = 0;
    uint64_t t0 = 0;
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)286 << 16))));
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v1, 4294964280))));
    (v0 = (uint64_t)(a0 | RC_U64X(v1)));
    (v0 = (uint64_t)(v0 & (uint64_t)(uint16_t)7));
    { const bool c = (int64_t)v0 == 0; (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v1, 576)))); if (c) goto L_1349390; }
L_1349330:;
    { uint32_t addr = ADD32((uint32_t)v1, 7); uint32_t aligned_addr = addr & ~7u; uint32_t offset = addr & 7u; uint64_t mem = A1_R64(aligned_addr); uint32_t shift = (7u - offset) << 3; uint64_t keepMask = (shift == 0) ? 0ull : ((1ull << shift) - 1ull); (a1 = (uint64_t)((a1 & keepMask) | (mem << shift))); }
    { uint32_t addr = ADD32((uint32_t)v1, 0); uint32_t aligned_addr = addr & ~7u; uint32_t offset = addr & 7u; uint64_t mem = A1_R64(aligned_addr); uint32_t shift = offset << 3; uint64_t keepMask = (offset == 0) ? 0ull : (0xFFFFFFFFFFFFFFFFull << ((8u - offset) << 3)); (a1 = (uint64_t)((a1 & keepMask) | (mem >> shift))); }
    { uint32_t addr = ADD32((uint32_t)v1, 15); uint32_t aligned_addr = addr & ~7u; uint32_t offset = addr & 7u; uint64_t mem = A1_R64(aligned_addr); uint32_t shift = (7u - offset) << 3; uint64_t keepMask = (shift == 0) ? 0ull : ((1ull << shift) - 1ull); (a2 = (uint64_t)((a2 & keepMask) | (mem << shift))); }
    { uint32_t addr = ADD32((uint32_t)v1, 8); uint32_t aligned_addr = addr & ~7u; uint32_t offset = addr & 7u; uint64_t mem = A1_R64(aligned_addr); uint32_t shift = offset << 3; uint64_t keepMask = (offset == 0) ? 0ull : (0xFFFFFFFFFFFFFFFFull << ((8u - offset) << 3)); (a2 = (uint64_t)((a2 & keepMask) | (mem >> shift))); }
    { uint32_t addr = ADD32((uint32_t)v1, 23); uint32_t aligned_addr = addr & ~7u; uint32_t offset = addr & 7u; uint64_t mem = A1_R64(aligned_addr); uint32_t shift = (7u - offset) << 3; uint64_t keepMask = (shift == 0) ? 0ull : ((1ull << shift) - 1ull); (a3 = (uint64_t)((a3 & keepMask) | (mem << shift))); }
    { uint32_t addr = ADD32((uint32_t)v1, 16); uint32_t aligned_addr = addr & ~7u; uint32_t offset = addr & 7u; uint64_t mem = A1_R64(aligned_addr); uint32_t shift = offset << 3; uint64_t keepMask = (offset == 0) ? 0ull : (0xFFFFFFFFFFFFFFFFull << ((8u - offset) << 3)); (a3 = (uint64_t)((a3 & keepMask) | (mem >> shift))); }
    { uint32_t addr = ADD32((uint32_t)v1, 31); uint32_t aligned_addr = addr & ~7u; uint32_t offset = addr & 7u; uint64_t mem = A1_R64(aligned_addr); uint32_t shift = (7u - offset) << 3; uint64_t keepMask = (shift == 0) ? 0ull : ((1ull << shift) - 1ull); (t0 = (uint64_t)((t0 & keepMask) | (mem << shift))); }
    { uint32_t addr = ADD32((uint32_t)v1, 24); uint32_t aligned_addr = addr & ~7u; uint32_t offset = addr & 7u; uint64_t mem = A1_R64(aligned_addr); uint32_t shift = offset << 3; uint64_t keepMask = (offset == 0) ? 0ull : (0xFFFFFFFFFFFFFFFFull << ((8u - offset) << 3)); (t0 = (uint64_t)((t0 & keepMask) | (mem >> shift))); }
    { uint32_t addr = ADD32((uint32_t)a0, 7); uint32_t aligned_addr = addr & ~7u; uint32_t offset = addr & 7u; uint32_t shift = (7u - offset) << 3; uint64_t mask = 0xFFFFFFFFFFFFFFFFull >> shift; uint64_t old_data = A1_R64(aligned_addr); uint64_t val = a1; uint64_t new_data = (old_data & ~mask) | ((val >> shift) & mask); A1_W64(aligned_addr, new_data); }
    { uint32_t addr = ADD32((uint32_t)a0, 0); uint32_t aligned_addr = addr & ~7u; uint32_t offset = addr & 7u; uint32_t shift = offset << 3; uint64_t mask = 0xFFFFFFFFFFFFFFFFull << shift; uint64_t old_data = A1_R64(aligned_addr); uint64_t val = a1; uint64_t new_data = (old_data & ~mask) | ((val << shift) & mask); A1_W64(aligned_addr, new_data); }
    { uint32_t addr = ADD32((uint32_t)a0, 15); uint32_t aligned_addr = addr & ~7u; uint32_t offset = addr & 7u; uint32_t shift = (7u - offset) << 3; uint64_t mask = 0xFFFFFFFFFFFFFFFFull >> shift; uint64_t old_data = A1_R64(aligned_addr); uint64_t val = a2; uint64_t new_data = (old_data & ~mask) | ((val >> shift) & mask); A1_W64(aligned_addr, new_data); }
    { uint32_t addr = ADD32((uint32_t)a0, 8); uint32_t aligned_addr = addr & ~7u; uint32_t offset = addr & 7u; uint32_t shift = offset << 3; uint64_t mask = 0xFFFFFFFFFFFFFFFFull << shift; uint64_t old_data = A1_R64(aligned_addr); uint64_t val = a2; uint64_t new_data = (old_data & ~mask) | ((val << shift) & mask); A1_W64(aligned_addr, new_data); }
    { uint32_t addr = ADD32((uint32_t)a0, 23); uint32_t aligned_addr = addr & ~7u; uint32_t offset = addr & 7u; uint32_t shift = (7u - offset) << 3; uint64_t mask = 0xFFFFFFFFFFFFFFFFull >> shift; uint64_t old_data = A1_R64(aligned_addr); uint64_t val = a3; uint64_t new_data = (old_data & ~mask) | ((val >> shift) & mask); A1_W64(aligned_addr, new_data); }
    { uint32_t addr = ADD32((uint32_t)a0, 16); uint32_t aligned_addr = addr & ~7u; uint32_t offset = addr & 7u; uint32_t shift = offset << 3; uint64_t mask = 0xFFFFFFFFFFFFFFFFull << shift; uint64_t old_data = A1_R64(aligned_addr); uint64_t val = a3; uint64_t new_data = (old_data & ~mask) | ((val << shift) & mask); A1_W64(aligned_addr, new_data); }
    { uint32_t addr = ADD32((uint32_t)a0, 31); uint32_t aligned_addr = addr & ~7u; uint32_t offset = addr & 7u; uint32_t shift = (7u - offset) << 3; uint64_t mask = 0xFFFFFFFFFFFFFFFFull >> shift; uint64_t old_data = A1_R64(aligned_addr); uint64_t val = t0; uint64_t new_data = (old_data & ~mask) | ((val >> shift) & mask); A1_W64(aligned_addr, new_data); }
    { uint32_t addr = ADD32((uint32_t)a0, 24); uint32_t aligned_addr = addr & ~7u; uint32_t offset = addr & 7u; uint32_t shift = offset << 3; uint64_t mask = 0xFFFFFFFFFFFFFFFFull << shift; uint64_t old_data = A1_R64(aligned_addr); uint64_t val = t0; uint64_t new_data = (old_data & ~mask) | ((val << shift) & mask); A1_W64(aligned_addr, new_data); }
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v1, 32))));
    { const bool c = (int64_t)v1 != (int64_t)v0; (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)a0, 32)))); if (c) { A3_BACK(); goto L_1349330; } }
    goto L_13493c4;
L_1349390:;
    (a1 = (uint64_t)(A1_R64B((uint32_t)v1, 0)));
    (a2 = (uint64_t)(A1_R64B((uint32_t)v1, 8)));
    (a3 = (uint64_t)(A1_R64B((uint32_t)v1, 16)));
    (t0 = (uint64_t)(A1_R64B((uint32_t)v1, 24)));
    A1_W64B((uint32_t)a0, 0, a1);
    A1_W64B((uint32_t)a0, 8, a2);
    A1_W64B((uint32_t)a0, 16, a3);
    A1_W64B((uint32_t)a0, 24, t0);
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v1, 32))));
    { const bool c = (int64_t)v1 != (int64_t)v0; (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)a0, 32)))); if (c) { A3_BACK(); goto L_1349390; } }
L_13493c4:;
    { uint32_t addr = ADD32((uint32_t)v1, 7); uint32_t aligned_addr = addr & ~7u; uint32_t offset = addr & 7u; uint64_t mem = A1_R64(aligned_addr); uint32_t shift = (7u - offset) << 3; uint64_t keepMask = (shift == 0) ? 0ull : ((1ull << shift) - 1ull); (v0 = (uint64_t)((v0 & keepMask) | (mem << shift))); }
    { uint32_t addr = ADD32((uint32_t)v1, 0); uint32_t aligned_addr = addr & ~7u; uint32_t offset = addr & 7u; uint64_t mem = A1_R64(aligned_addr); uint32_t shift = offset << 3; uint64_t keepMask = (offset == 0) ? 0ull : (0xFFFFFFFFFFFFFFFFull << ((8u - offset) << 3)); (v0 = (uint64_t)((v0 & keepMask) | (mem >> shift))); }
    (a1 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)v1, 8))));
    { uint32_t addr = ADD32((uint32_t)a0, 7); uint32_t aligned_addr = addr & ~7u; uint32_t offset = addr & 7u; uint32_t shift = (7u - offset) << 3; uint64_t mask = 0xFFFFFFFFFFFFFFFFull >> shift; uint64_t old_data = A1_R64(aligned_addr); uint64_t val = v0; uint64_t new_data = (old_data & ~mask) | ((val >> shift) & mask); A1_W64(aligned_addr, new_data); }
    { uint32_t addr = ADD32((uint32_t)a0, 0); uint32_t aligned_addr = addr & ~7u; uint32_t offset = addr & 7u; uint32_t shift = offset << 3; uint64_t mask = 0xFFFFFFFFFFFFFFFFull << shift; uint64_t old_data = A1_R64(aligned_addr); uint64_t val = v0; uint64_t new_data = (old_data & ~mask) | ((val << shift) & mask); A1_W64(aligned_addr, new_data); }
    { A1_W32B((uint32_t)a0, 8, (uint32_t)a1); if (xo) { xo[0] = a0; xo[1] = a1; xo[2] = a2; xo[3] = a3; xo[4] = t0; } return v0; }
    __builtin_unreachable();
}

// backStageCheckPointLoad (0x13493e0): typed by tools/typed_lift2.py (round A3; loop leaf, extra outputs xo[0]=a0 xo[1]=a1 xo[2]=a2 xo[3]=a3 xo[4]=t0)
inline uint64_t backStageCheckPointLoad(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint64_t a0, uint64_t *xo = nullptr)
{
    uint64_t v0 = 0;
    uint64_t v1 = 0;
    uint64_t a1 = 0;
    uint64_t a2 = 0;
    uint64_t a3 = 0;
    uint64_t t0 = 0;
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)286 << 16))));
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v1, 4294964280))));
    (v0 = (uint64_t)(a0 | RC_U64X(v1)));
    (v0 = (uint64_t)(v0 & (uint64_t)(uint16_t)7));
    { const bool c = (int64_t)v0 == 0; (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)a0, 576)))); if (c) goto L_1349458; }
L_13493f8:;
    { uint32_t addr = ADD32((uint32_t)a0, 7); uint32_t aligned_addr = addr & ~7u; uint32_t offset = addr & 7u; uint64_t mem = A1_R64(aligned_addr); uint32_t shift = (7u - offset) << 3; uint64_t keepMask = (shift == 0) ? 0ull : ((1ull << shift) - 1ull); (a1 = (uint64_t)((a1 & keepMask) | (mem << shift))); }
    { uint32_t addr = ADD32((uint32_t)a0, 0); uint32_t aligned_addr = addr & ~7u; uint32_t offset = addr & 7u; uint64_t mem = A1_R64(aligned_addr); uint32_t shift = offset << 3; uint64_t keepMask = (offset == 0) ? 0ull : (0xFFFFFFFFFFFFFFFFull << ((8u - offset) << 3)); (a1 = (uint64_t)((a1 & keepMask) | (mem >> shift))); }
    { uint32_t addr = ADD32((uint32_t)a0, 15); uint32_t aligned_addr = addr & ~7u; uint32_t offset = addr & 7u; uint64_t mem = A1_R64(aligned_addr); uint32_t shift = (7u - offset) << 3; uint64_t keepMask = (shift == 0) ? 0ull : ((1ull << shift) - 1ull); (a2 = (uint64_t)((a2 & keepMask) | (mem << shift))); }
    { uint32_t addr = ADD32((uint32_t)a0, 8); uint32_t aligned_addr = addr & ~7u; uint32_t offset = addr & 7u; uint64_t mem = A1_R64(aligned_addr); uint32_t shift = offset << 3; uint64_t keepMask = (offset == 0) ? 0ull : (0xFFFFFFFFFFFFFFFFull << ((8u - offset) << 3)); (a2 = (uint64_t)((a2 & keepMask) | (mem >> shift))); }
    { uint32_t addr = ADD32((uint32_t)a0, 23); uint32_t aligned_addr = addr & ~7u; uint32_t offset = addr & 7u; uint64_t mem = A1_R64(aligned_addr); uint32_t shift = (7u - offset) << 3; uint64_t keepMask = (shift == 0) ? 0ull : ((1ull << shift) - 1ull); (a3 = (uint64_t)((a3 & keepMask) | (mem << shift))); }
    { uint32_t addr = ADD32((uint32_t)a0, 16); uint32_t aligned_addr = addr & ~7u; uint32_t offset = addr & 7u; uint64_t mem = A1_R64(aligned_addr); uint32_t shift = offset << 3; uint64_t keepMask = (offset == 0) ? 0ull : (0xFFFFFFFFFFFFFFFFull << ((8u - offset) << 3)); (a3 = (uint64_t)((a3 & keepMask) | (mem >> shift))); }
    { uint32_t addr = ADD32((uint32_t)a0, 31); uint32_t aligned_addr = addr & ~7u; uint32_t offset = addr & 7u; uint64_t mem = A1_R64(aligned_addr); uint32_t shift = (7u - offset) << 3; uint64_t keepMask = (shift == 0) ? 0ull : ((1ull << shift) - 1ull); (t0 = (uint64_t)((t0 & keepMask) | (mem << shift))); }
    { uint32_t addr = ADD32((uint32_t)a0, 24); uint32_t aligned_addr = addr & ~7u; uint32_t offset = addr & 7u; uint64_t mem = A1_R64(aligned_addr); uint32_t shift = offset << 3; uint64_t keepMask = (offset == 0) ? 0ull : (0xFFFFFFFFFFFFFFFFull << ((8u - offset) << 3)); (t0 = (uint64_t)((t0 & keepMask) | (mem >> shift))); }
    { uint32_t addr = ADD32((uint32_t)v1, 7); uint32_t aligned_addr = addr & ~7u; uint32_t offset = addr & 7u; uint32_t shift = (7u - offset) << 3; uint64_t mask = 0xFFFFFFFFFFFFFFFFull >> shift; uint64_t old_data = A1_R64(aligned_addr); uint64_t val = a1; uint64_t new_data = (old_data & ~mask) | ((val >> shift) & mask); A1_W64(aligned_addr, new_data); }
    { uint32_t addr = ADD32((uint32_t)v1, 0); uint32_t aligned_addr = addr & ~7u; uint32_t offset = addr & 7u; uint32_t shift = offset << 3; uint64_t mask = 0xFFFFFFFFFFFFFFFFull << shift; uint64_t old_data = A1_R64(aligned_addr); uint64_t val = a1; uint64_t new_data = (old_data & ~mask) | ((val << shift) & mask); A1_W64(aligned_addr, new_data); }
    { uint32_t addr = ADD32((uint32_t)v1, 15); uint32_t aligned_addr = addr & ~7u; uint32_t offset = addr & 7u; uint32_t shift = (7u - offset) << 3; uint64_t mask = 0xFFFFFFFFFFFFFFFFull >> shift; uint64_t old_data = A1_R64(aligned_addr); uint64_t val = a2; uint64_t new_data = (old_data & ~mask) | ((val >> shift) & mask); A1_W64(aligned_addr, new_data); }
    { uint32_t addr = ADD32((uint32_t)v1, 8); uint32_t aligned_addr = addr & ~7u; uint32_t offset = addr & 7u; uint32_t shift = offset << 3; uint64_t mask = 0xFFFFFFFFFFFFFFFFull << shift; uint64_t old_data = A1_R64(aligned_addr); uint64_t val = a2; uint64_t new_data = (old_data & ~mask) | ((val << shift) & mask); A1_W64(aligned_addr, new_data); }
    { uint32_t addr = ADD32((uint32_t)v1, 23); uint32_t aligned_addr = addr & ~7u; uint32_t offset = addr & 7u; uint32_t shift = (7u - offset) << 3; uint64_t mask = 0xFFFFFFFFFFFFFFFFull >> shift; uint64_t old_data = A1_R64(aligned_addr); uint64_t val = a3; uint64_t new_data = (old_data & ~mask) | ((val >> shift) & mask); A1_W64(aligned_addr, new_data); }
    { uint32_t addr = ADD32((uint32_t)v1, 16); uint32_t aligned_addr = addr & ~7u; uint32_t offset = addr & 7u; uint32_t shift = offset << 3; uint64_t mask = 0xFFFFFFFFFFFFFFFFull << shift; uint64_t old_data = A1_R64(aligned_addr); uint64_t val = a3; uint64_t new_data = (old_data & ~mask) | ((val << shift) & mask); A1_W64(aligned_addr, new_data); }
    { uint32_t addr = ADD32((uint32_t)v1, 31); uint32_t aligned_addr = addr & ~7u; uint32_t offset = addr & 7u; uint32_t shift = (7u - offset) << 3; uint64_t mask = 0xFFFFFFFFFFFFFFFFull >> shift; uint64_t old_data = A1_R64(aligned_addr); uint64_t val = t0; uint64_t new_data = (old_data & ~mask) | ((val >> shift) & mask); A1_W64(aligned_addr, new_data); }
    { uint32_t addr = ADD32((uint32_t)v1, 24); uint32_t aligned_addr = addr & ~7u; uint32_t offset = addr & 7u; uint32_t shift = offset << 3; uint64_t mask = 0xFFFFFFFFFFFFFFFFull << shift; uint64_t old_data = A1_R64(aligned_addr); uint64_t val = t0; uint64_t new_data = (old_data & ~mask) | ((val << shift) & mask); A1_W64(aligned_addr, new_data); }
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)a0, 32))));
    { const bool c = (int64_t)a0 != (int64_t)v0; (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v1, 32)))); if (c) { A3_BACK(); goto L_13493f8; } }
    goto L_134948c;
L_1349458:;
    (a1 = (uint64_t)(A1_R64B((uint32_t)a0, 0)));
    (a2 = (uint64_t)(A1_R64B((uint32_t)a0, 8)));
    (a3 = (uint64_t)(A1_R64B((uint32_t)a0, 16)));
    (t0 = (uint64_t)(A1_R64B((uint32_t)a0, 24)));
    A1_W64B((uint32_t)v1, 0, a1);
    A1_W64B((uint32_t)v1, 8, a2);
    A1_W64B((uint32_t)v1, 16, a3);
    A1_W64B((uint32_t)v1, 24, t0);
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)a0, 32))));
    { const bool c = (int64_t)a0 != (int64_t)v0; (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v1, 32)))); if (c) { A3_BACK(); goto L_1349458; } }
L_134948c:;
    { uint32_t addr = ADD32((uint32_t)a0, 7); uint32_t aligned_addr = addr & ~7u; uint32_t offset = addr & 7u; uint64_t mem = A1_R64(aligned_addr); uint32_t shift = (7u - offset) << 3; uint64_t keepMask = (shift == 0) ? 0ull : ((1ull << shift) - 1ull); (v0 = (uint64_t)((v0 & keepMask) | (mem << shift))); }
    { uint32_t addr = ADD32((uint32_t)a0, 0); uint32_t aligned_addr = addr & ~7u; uint32_t offset = addr & 7u; uint64_t mem = A1_R64(aligned_addr); uint32_t shift = offset << 3; uint64_t keepMask = (offset == 0) ? 0ull : (0xFFFFFFFFFFFFFFFFull << ((8u - offset) << 3)); (v0 = (uint64_t)((v0 & keepMask) | (mem >> shift))); }
    (a1 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)a0, 8))));
    { uint32_t addr = ADD32((uint32_t)v1, 7); uint32_t aligned_addr = addr & ~7u; uint32_t offset = addr & 7u; uint32_t shift = (7u - offset) << 3; uint64_t mask = 0xFFFFFFFFFFFFFFFFull >> shift; uint64_t old_data = A1_R64(aligned_addr); uint64_t val = v0; uint64_t new_data = (old_data & ~mask) | ((val >> shift) & mask); A1_W64(aligned_addr, new_data); }
    { uint32_t addr = ADD32((uint32_t)v1, 0); uint32_t aligned_addr = addr & ~7u; uint32_t offset = addr & 7u; uint32_t shift = offset << 3; uint64_t mask = 0xFFFFFFFFFFFFFFFFull << shift; uint64_t old_data = A1_R64(aligned_addr); uint64_t val = v0; uint64_t new_data = (old_data & ~mask) | ((val << shift) & mask); A1_W64(aligned_addr, new_data); }
    { A1_W32B((uint32_t)v1, 8, (uint32_t)a1); if (xo) { xo[0] = a0; xo[1] = a1; xo[2] = a2; xo[3] = a3; xo[4] = t0; } return v0; }
    __builtin_unreachable();
}

// _optionWorkSet (0x134ef50): typed by tools/typed_lift2.py (round A3; leaf, extra outputs xo[0]=a0)
inline void _optionWorkSet(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t a0, uint32_t a1, uint64_t *xo = nullptr)
{
    uint32_t at = 0;
    a0 = a0 << 2;
    at = 0x11E0000u;
    at = at + a0;
    A1_W32(at - 0x8B8u, a1);
    { if (xo) { xo[0] = a1_sx64(a0); } return; }
    __builtin_unreachable();
}

// _gameFlagSet (0x134efb0): typed by tools/typed_lift2.py (round A3; leaf, extra outputs xo[0]=at xo[1]=a0 xo[2]=a1 xo[3]=a2)
inline uint64_t _gameFlagSet(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint64_t a0, uint64_t *xo = nullptr)
{
    uint32_t at = 0;
    uint64_t v0 = 0;
    uint64_t v1 = 0;
    uint32_t a1 = 0;
    uint32_t a2 = 0;
    (v0 = (uint64_t)(((int64_t)(int64_t)a0 < (int64_t)(int32_t)0) ? 1 : 0));
    (a2 = (uint32_t)((int32_t)((int32_t)ADD32((uint32_t)a0, 7))));
    (v1 = (uint64_t)((uint64_t)a0 + (uint64_t)0ULL));
    (a0 = (uint64_t)(a0 & (uint64_t)(uint16_t)7));
    if (RC_U64X(v0) != 0) v1 = (uint64_t)(a1_sx64(a2));;
    a1 = 1u;
    (v1 = (uint64_t)((int64_t)(int32_t)(SRA32((int32_t)v1, 3))));
    (a1 = (uint32_t)((int32_t)((int32_t)SLL32(a1, (uint32_t)a0 & 0x1F))));
    at = 0x11E0000u;
    at = at - 0x830u;
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(at, (uint32_t)v1))));
    (v0 = (uint64_t)(uint32_t)((uint8_t)A1_R8B((uint32_t)v1, 0)));
    (v0 = (uint64_t)(RC_U64X(v0) | a1_sx64(a1)));
    { A1_W8B((uint32_t)v1, 0, (uint8_t)(uint32_t)v0); if (xo) { xo[0] = a1_sx64(at); xo[1] = a0; xo[2] = a1_sx64(a1); xo[3] = a1_sx64(a2); } return v0; }
    __builtin_unreachable();
}

// _gameFlagCls (0x134eff0): typed by tools/typed_lift2.py (round A3; leaf, extra outputs xo[0]=a0 xo[1]=a1 xo[2]=a2)
inline uint64_t _gameFlagCls(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint64_t a0, uint64_t *xo = nullptr)
{
    uint32_t at = 0;
    uint64_t v0 = 0;
    uint64_t v1 = 0;
    uint32_t a1 = 0;
    uint32_t a2 = 0;
    (v0 = (uint64_t)(((int64_t)(int64_t)a0 < (int64_t)(int32_t)0) ? 1 : 0));
    (a2 = (uint32_t)((int32_t)((int32_t)ADD32((uint32_t)a0, 7))));
    (v1 = (uint64_t)((uint64_t)a0 + (uint64_t)0ULL));
    (a0 = (uint64_t)(a0 & (uint64_t)(uint16_t)7));
    if (RC_U64X(v0) != 0) v1 = (uint64_t)(a1_sx64(a2));;
    a1 = 1u;
    (v1 = (uint64_t)((int64_t)(int32_t)(SRA32((int32_t)v1, 3))));
    (a1 = (uint32_t)((int32_t)((int32_t)SLL32(a1, (uint32_t)a0 & 0x1F))));
    at = 0x11E0000u;
    at = at - 0x830u;
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(at, (uint32_t)v1))));
    a1 = ~(0u | a1);
    (v0 = (uint64_t)(uint32_t)((uint8_t)A1_R8B((uint32_t)v1, 0)));
    (v0 = (uint64_t)(RC_U64X(v0) & a1_sx64(a1)));
    { A1_W8B((uint32_t)v1, 0, (uint8_t)(uint32_t)v0); if (xo) { xo[0] = a0; xo[1] = a1_sx64(a1); xo[2] = a1_sx64(a2); } return v0; }
    __builtin_unreachable();
}

// _gameFlagChk (0x134f030): typed by tools/typed_lift2.py (round A3; leaf, extra outputs xo[0]=a0 xo[1]=a1)
inline uint64_t _gameFlagChk(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint64_t a0, uint64_t *xo = nullptr)
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
    (v0 = (uint64_t)(uint32_t)((uint8_t)A1_R8B((uint32_t)v0, -2096)));
    (v0 = (uint64_t)((int64_t)(int32_t)(SRA32((int32_t)v0, (uint32_t)a0 & 0x1F))));
    { (v0 = (uint64_t)(RC_U64X(v0) & (uint64_t)(uint16_t)1)); if (xo) { xo[0] = a0; xo[1] = a1_sx64(a1); } return v0; }
    __builtin_unreachable();
}

// _gameWorkSet (0x134f0a8): typed by tools/typed_lift2.py (round A3; leaf, extra outputs xo[0]=at)
inline void _gameWorkSet(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t a0, uint32_t a1, uint64_t *xo = nullptr)
{
    uint32_t at = 0;
    a0 = a0 << 2;
    at = 0x11E0000u;
    at = at + a0;
    A1_W32(at - 0x7E0u, a1);
    { if (xo) { xo[0] = a1_sx64(at); } return; }
    __builtin_unreachable();
}

// _gameFWorkSet (0x134f0c0): typed by tools/typed_lift2.py (round A3; leaf, extra outputs xo[0]=a0)
inline uint32_t _gameFWorkSet(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t a0, float f12, uint64_t *xo = nullptr)
{
    uint32_t v0 = 0;
    uint32_t v1 = 0;
    v0 = 0x1460000u;
    a0 = a0 << 2;
    v1 = A1_R32B(v0, 23408);
    a0 = a0 + v1;
    { A1_W32(a0, a1_bits(f12)); if (xo) { xo[0] = a1_sx64(a0); } return v0; }
    __builtin_unreachable();
}

// _gameFWorkChk (0x134f120): typed by tools/typed_lift2.py (round A3; leaf, extra outputs xo[0]=a0)
inline a1::RetVF _gameFWorkChk(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t a0, uint64_t *xo = nullptr)
{
    float f0 = 0.0f;
    uint32_t v0 = 0;
    uint32_t v1 = 0;
    v0 = 0x1460000u;
    a0 = a0 << 2;
    v1 = A1_R32B(v0, 23408);
    a0 = a0 + v1;
    { f0 = a1_flt(A1_R32(a0)); if (xo) { xo[0] = a1_sx64(a0); } return a1::RetVF{(uint64_t)a1_sx64(v0), f0}; }
    __builtin_unreachable();
}

// fruitsFlagChk (0x134f1b0): typed by tools/typed_lift2.py (round A3; leaf, extra outputs xo[0]=a2 xo[1]=a3 xo[2]=t0)
inline uint32_t fruitsFlagChk(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint64_t a0, uint64_t a1, uint32_t a3, uint64_t *xo = nullptr)
{
    uint32_t v0 = 0;
    uint64_t v1 = 0;
    uint64_t a2 = 0;
    uint32_t t0 = 0;
    bool a3xw1 = false;
    v0 = 0x11E0000u;
    (v1 = (uint64_t)(((uint64_t)a0 < (uint64_t)(int64_t)(int32_t)100) ? 1 : 0));
    (a2 = (uint64_t)(((int64_t)(int64_t)a1 < (int64_t)(int32_t)32) ? 1 : 0));
    { const bool c = (int64_t)v1 == 0; t0 = v0 - 0x778u; if (c) goto L_134f1d4; }
    { const bool c = (int64_t)a2 == 0; v0 = 0u; if (c) goto L_134f1f0; }
    if ((int32_t)a1 >= 0) { (v0 = (uint32_t)((int32_t)((int32_t)SLL32((uint32_t)a0, 5)))); goto L_134f1e0; }
L_134f1d4:;
    v0 = 0u;
    goto L_134f1f0;
L_134f1e0:;
    (v0 = (uint32_t)((int32_t)((int32_t)ADD32(v0, (uint32_t)a1))));
    (v1 = (uint64_t)((int64_t)(int32_t)(SRA32((int32_t)v0, 3))));
    a3 = v0 & 7u; a3xw1 = true;
    (v0 = (uint32_t)((int32_t)((int32_t)ADD32((uint32_t)v1, t0))));
L_134f1f0:;
    if (v0 == 0) { v0 = 0u; goto L_134f210; }
    (v1 = (uint64_t)(uint32_t)((uint8_t)A1_R8B(v0, 0)));
    (v1 = (uint64_t)((int64_t)(int32_t)(SRA32((int32_t)v1, a3 & 0x1F))));
    (v1 = (uint64_t)(RC_U64X(v1) & (uint64_t)(uint16_t)1));
    { const bool c = (int64_t)v1 != 0; v0 = 1u; if (c) goto L_134f210; }
    v0 = 0u;
L_134f210:;
    { if (xo) { xo[0] = a2; if (a3xw1) xo[1] = a1_sx64(a3); xo[2] = a1_sx64(t0); } return v0; }
    __builtin_unreachable();
}

// gameFlagCheckPointLoad (0x134f350): typed by tools/typed_lift2.py (round A3; loop leaf, extra outputs xo[0]=a1 xo[1]=a2 xo[2]=a3 xo[3]=t0 xo[4]=t1 xo[5]=t2 xo[6]=t3)
inline uint64_t gameFlagCheckPointLoad(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint64_t a0, uint64_t a1, uint64_t a2, uint64_t a3, uint64_t t0, uint64_t t1, uint64_t *xo = nullptr)
{
    uint64_t v0 = 0;
    uint64_t v1 = 0;
    uint64_t t2 = 0;
    uint64_t t3 = 0;
    uint64_t t4 = 0;
    uint64_t t5 = 0;
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)286 << 16))));
    (t2 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v0, 4294965200))));
    (v0 = (uint64_t)(a0 | RC_U64X(t2)));
    (v0 = (uint64_t)(v0 & (uint64_t)(uint16_t)7));
    { const bool c = (int64_t)v0 == 0; (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)a0, 64)))); if (c) goto L_134f3c8; }
L_134f368:;
    { uint32_t addr = ADD32((uint32_t)a0, 7); uint32_t aligned_addr = addr & ~7u; uint32_t offset = addr & 7u; uint64_t mem = A1_R64(aligned_addr); uint32_t shift = (7u - offset) << 3; uint64_t keepMask = (shift == 0) ? 0ull : ((1ull << shift) - 1ull); (v1 = (uint64_t)((v1 & keepMask) | (mem << shift))); }
    { uint32_t addr = ADD32((uint32_t)a0, 0); uint32_t aligned_addr = addr & ~7u; uint32_t offset = addr & 7u; uint64_t mem = A1_R64(aligned_addr); uint32_t shift = offset << 3; uint64_t keepMask = (offset == 0) ? 0ull : (0xFFFFFFFFFFFFFFFFull << ((8u - offset) << 3)); (v1 = (uint64_t)((v1 & keepMask) | (mem >> shift))); }
    { uint32_t addr = ADD32((uint32_t)a0, 15); uint32_t aligned_addr = addr & ~7u; uint32_t offset = addr & 7u; uint64_t mem = A1_R64(aligned_addr); uint32_t shift = (7u - offset) << 3; uint64_t keepMask = (shift == 0) ? 0ull : ((1ull << shift) - 1ull); (t3 = (uint64_t)((t3 & keepMask) | (mem << shift))); }
    { uint32_t addr = ADD32((uint32_t)a0, 8); uint32_t aligned_addr = addr & ~7u; uint32_t offset = addr & 7u; uint64_t mem = A1_R64(aligned_addr); uint32_t shift = offset << 3; uint64_t keepMask = (offset == 0) ? 0ull : (0xFFFFFFFFFFFFFFFFull << ((8u - offset) << 3)); (t3 = (uint64_t)((t3 & keepMask) | (mem >> shift))); }
    { uint32_t addr = ADD32((uint32_t)a0, 23); uint32_t aligned_addr = addr & ~7u; uint32_t offset = addr & 7u; uint64_t mem = A1_R64(aligned_addr); uint32_t shift = (7u - offset) << 3; uint64_t keepMask = (shift == 0) ? 0ull : ((1ull << shift) - 1ull); (t4 = (uint64_t)((t4 & keepMask) | (mem << shift))); }
    { uint32_t addr = ADD32((uint32_t)a0, 16); uint32_t aligned_addr = addr & ~7u; uint32_t offset = addr & 7u; uint64_t mem = A1_R64(aligned_addr); uint32_t shift = offset << 3; uint64_t keepMask = (offset == 0) ? 0ull : (0xFFFFFFFFFFFFFFFFull << ((8u - offset) << 3)); (t4 = (uint64_t)((t4 & keepMask) | (mem >> shift))); }
    { uint32_t addr = ADD32((uint32_t)a0, 31); uint32_t aligned_addr = addr & ~7u; uint32_t offset = addr & 7u; uint64_t mem = A1_R64(aligned_addr); uint32_t shift = (7u - offset) << 3; uint64_t keepMask = (shift == 0) ? 0ull : ((1ull << shift) - 1ull); (t5 = (uint64_t)((t5 & keepMask) | (mem << shift))); }
    { uint32_t addr = ADD32((uint32_t)a0, 24); uint32_t aligned_addr = addr & ~7u; uint32_t offset = addr & 7u; uint64_t mem = A1_R64(aligned_addr); uint32_t shift = offset << 3; uint64_t keepMask = (offset == 0) ? 0ull : (0xFFFFFFFFFFFFFFFFull << ((8u - offset) << 3)); (t5 = (uint64_t)((t5 & keepMask) | (mem >> shift))); }
    { uint32_t addr = ADD32((uint32_t)t2, 7); uint32_t aligned_addr = addr & ~7u; uint32_t offset = addr & 7u; uint32_t shift = (7u - offset) << 3; uint64_t mask = 0xFFFFFFFFFFFFFFFFull >> shift; uint64_t old_data = A1_R64(aligned_addr); uint64_t val = v1; uint64_t new_data = (old_data & ~mask) | ((val >> shift) & mask); A1_W64(aligned_addr, new_data); }
    { uint32_t addr = ADD32((uint32_t)t2, 0); uint32_t aligned_addr = addr & ~7u; uint32_t offset = addr & 7u; uint32_t shift = offset << 3; uint64_t mask = 0xFFFFFFFFFFFFFFFFull << shift; uint64_t old_data = A1_R64(aligned_addr); uint64_t val = v1; uint64_t new_data = (old_data & ~mask) | ((val << shift) & mask); A1_W64(aligned_addr, new_data); }
    { uint32_t addr = ADD32((uint32_t)t2, 15); uint32_t aligned_addr = addr & ~7u; uint32_t offset = addr & 7u; uint32_t shift = (7u - offset) << 3; uint64_t mask = 0xFFFFFFFFFFFFFFFFull >> shift; uint64_t old_data = A1_R64(aligned_addr); uint64_t val = t3; uint64_t new_data = (old_data & ~mask) | ((val >> shift) & mask); A1_W64(aligned_addr, new_data); }
    { uint32_t addr = ADD32((uint32_t)t2, 8); uint32_t aligned_addr = addr & ~7u; uint32_t offset = addr & 7u; uint32_t shift = offset << 3; uint64_t mask = 0xFFFFFFFFFFFFFFFFull << shift; uint64_t old_data = A1_R64(aligned_addr); uint64_t val = t3; uint64_t new_data = (old_data & ~mask) | ((val << shift) & mask); A1_W64(aligned_addr, new_data); }
    { uint32_t addr = ADD32((uint32_t)t2, 23); uint32_t aligned_addr = addr & ~7u; uint32_t offset = addr & 7u; uint32_t shift = (7u - offset) << 3; uint64_t mask = 0xFFFFFFFFFFFFFFFFull >> shift; uint64_t old_data = A1_R64(aligned_addr); uint64_t val = t4; uint64_t new_data = (old_data & ~mask) | ((val >> shift) & mask); A1_W64(aligned_addr, new_data); }
    { uint32_t addr = ADD32((uint32_t)t2, 16); uint32_t aligned_addr = addr & ~7u; uint32_t offset = addr & 7u; uint32_t shift = offset << 3; uint64_t mask = 0xFFFFFFFFFFFFFFFFull << shift; uint64_t old_data = A1_R64(aligned_addr); uint64_t val = t4; uint64_t new_data = (old_data & ~mask) | ((val << shift) & mask); A1_W64(aligned_addr, new_data); }
    { uint32_t addr = ADD32((uint32_t)t2, 31); uint32_t aligned_addr = addr & ~7u; uint32_t offset = addr & 7u; uint32_t shift = (7u - offset) << 3; uint64_t mask = 0xFFFFFFFFFFFFFFFFull >> shift; uint64_t old_data = A1_R64(aligned_addr); uint64_t val = t5; uint64_t new_data = (old_data & ~mask) | ((val >> shift) & mask); A1_W64(aligned_addr, new_data); }
    { uint32_t addr = ADD32((uint32_t)t2, 24); uint32_t aligned_addr = addr & ~7u; uint32_t offset = addr & 7u; uint32_t shift = offset << 3; uint64_t mask = 0xFFFFFFFFFFFFFFFFull << shift; uint64_t old_data = A1_R64(aligned_addr); uint64_t val = t5; uint64_t new_data = (old_data & ~mask) | ((val << shift) & mask); A1_W64(aligned_addr, new_data); }
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)a0, 32))));
    { const bool c = (int64_t)a0 != (int64_t)v0; (t2 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)t2, 32)))); if (c) { A3_BACK(); goto L_134f368; } }
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)286 << 16))));
    goto L_134f400;
L_134f3c8:;
    (v1 = (uint64_t)(A1_R64B((uint32_t)a0, 0)));
    (t3 = (uint64_t)(A1_R64B((uint32_t)a0, 8)));
    (t4 = (uint64_t)(A1_R64B((uint32_t)a0, 16)));
    (t5 = (uint64_t)(A1_R64B((uint32_t)a0, 24)));
    A1_W64B((uint32_t)t2, 0, v1);
    A1_W64B((uint32_t)t2, 8, t3);
    A1_W64B((uint32_t)t2, 16, t4);
    A1_W64B((uint32_t)t2, 24, t5);
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)a0, 32))));
    { const bool c = (int64_t)a0 != (int64_t)v0; (t2 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)t2, 32)))); if (c) { A3_BACK(); goto L_134f3c8; } }
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)286 << 16))));
L_134f400:;
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v0, 4294965280))));
    { uint32_t addr = ADD32((uint32_t)a0, 7); uint32_t aligned_addr = addr & ~7u; uint32_t offset = addr & 7u; uint64_t mem = A1_R64(aligned_addr); uint32_t shift = (7u - offset) << 3; uint64_t keepMask = (shift == 0) ? 0ull : ((1ull << shift) - 1ull); (v0 = (uint64_t)((v0 & keepMask) | (mem << shift))); }
    { uint32_t addr = ADD32((uint32_t)a0, 0); uint32_t aligned_addr = addr & ~7u; uint32_t offset = addr & 7u; uint64_t mem = A1_R64(aligned_addr); uint32_t shift = offset << 3; uint64_t keepMask = (offset == 0) ? 0ull : (0xFFFFFFFFFFFFFFFFull << ((8u - offset) << 3)); (v0 = (uint64_t)((v0 & keepMask) | (mem >> shift))); }
    { uint32_t addr = ADD32((uint32_t)a0, 15); uint32_t aligned_addr = addr & ~7u; uint32_t offset = addr & 7u; uint64_t mem = A1_R64(aligned_addr); uint32_t shift = (7u - offset) << 3; uint64_t keepMask = (shift == 0) ? 0ull : ((1ull << shift) - 1ull); (t3 = (uint64_t)((t3 & keepMask) | (mem << shift))); }
    { uint32_t addr = ADD32((uint32_t)a0, 8); uint32_t aligned_addr = addr & ~7u; uint32_t offset = addr & 7u; uint64_t mem = A1_R64(aligned_addr); uint32_t shift = offset << 3; uint64_t keepMask = (offset == 0) ? 0ull : (0xFFFFFFFFFFFFFFFFull << ((8u - offset) << 3)); (t3 = (uint64_t)((t3 & keepMask) | (mem >> shift))); }
    { uint32_t addr = ADD32((uint32_t)t2, 7); uint32_t aligned_addr = addr & ~7u; uint32_t offset = addr & 7u; uint32_t shift = (7u - offset) << 3; uint64_t mask = 0xFFFFFFFFFFFFFFFFull >> shift; uint64_t old_data = A1_R64(aligned_addr); uint64_t val = v0; uint64_t new_data = (old_data & ~mask) | ((val >> shift) & mask); A1_W64(aligned_addr, new_data); }
    { uint32_t addr = ADD32((uint32_t)t2, 0); uint32_t aligned_addr = addr & ~7u; uint32_t offset = addr & 7u; uint32_t shift = offset << 3; uint64_t mask = 0xFFFFFFFFFFFFFFFFull << shift; uint64_t old_data = A1_R64(aligned_addr); uint64_t val = v0; uint64_t new_data = (old_data & ~mask) | ((val << shift) & mask); A1_W64(aligned_addr, new_data); }
    { uint32_t addr = ADD32((uint32_t)t2, 15); uint32_t aligned_addr = addr & ~7u; uint32_t offset = addr & 7u; uint32_t shift = (7u - offset) << 3; uint64_t mask = 0xFFFFFFFFFFFFFFFFull >> shift; uint64_t old_data = A1_R64(aligned_addr); uint64_t val = t3; uint64_t new_data = (old_data & ~mask) | ((val >> shift) & mask); A1_W64(aligned_addr, new_data); }
    { uint32_t addr = ADD32((uint32_t)t2, 8); uint32_t aligned_addr = addr & ~7u; uint32_t offset = addr & 7u; uint32_t shift = offset << 3; uint64_t mask = 0xFFFFFFFFFFFFFFFFull << shift; uint64_t old_data = A1_R64(aligned_addr); uint64_t val = t3; uint64_t new_data = (old_data & ~mask) | ((val << shift) & mask); A1_W64(aligned_addr, new_data); }
    (v0 = (uint64_t)(a1 | RC_U64X(v1)));
    (v0 = (uint64_t)(v0 & (uint64_t)(uint16_t)7));
    { const bool c = (int64_t)v0 == 0; (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)a1, 96)))); if (c) goto L_134f490; }
L_134f434:;
    { uint32_t addr = ADD32((uint32_t)a1, 7); uint32_t aligned_addr = addr & ~7u; uint32_t offset = addr & 7u; uint64_t mem = A1_R64(aligned_addr); uint32_t shift = (7u - offset) << 3; uint64_t keepMask = (shift == 0) ? 0ull : ((1ull << shift) - 1ull); (a0 = (uint64_t)((a0 & keepMask) | (mem << shift))); }
    { uint32_t addr = ADD32((uint32_t)a1, 0); uint32_t aligned_addr = addr & ~7u; uint32_t offset = addr & 7u; uint64_t mem = A1_R64(aligned_addr); uint32_t shift = offset << 3; uint64_t keepMask = (offset == 0) ? 0ull : (0xFFFFFFFFFFFFFFFFull << ((8u - offset) << 3)); (a0 = (uint64_t)((a0 & keepMask) | (mem >> shift))); }
    { uint32_t addr = ADD32((uint32_t)a1, 15); uint32_t aligned_addr = addr & ~7u; uint32_t offset = addr & 7u; uint64_t mem = A1_R64(aligned_addr); uint32_t shift = (7u - offset) << 3; uint64_t keepMask = (shift == 0) ? 0ull : ((1ull << shift) - 1ull); (t2 = (uint64_t)((t2 & keepMask) | (mem << shift))); }
    { uint32_t addr = ADD32((uint32_t)a1, 8); uint32_t aligned_addr = addr & ~7u; uint32_t offset = addr & 7u; uint64_t mem = A1_R64(aligned_addr); uint32_t shift = offset << 3; uint64_t keepMask = (offset == 0) ? 0ull : (0xFFFFFFFFFFFFFFFFull << ((8u - offset) << 3)); (t2 = (uint64_t)((t2 & keepMask) | (mem >> shift))); }
    { uint32_t addr = ADD32((uint32_t)a1, 23); uint32_t aligned_addr = addr & ~7u; uint32_t offset = addr & 7u; uint64_t mem = A1_R64(aligned_addr); uint32_t shift = (7u - offset) << 3; uint64_t keepMask = (shift == 0) ? 0ull : ((1ull << shift) - 1ull); (t3 = (uint64_t)((t3 & keepMask) | (mem << shift))); }
    { uint32_t addr = ADD32((uint32_t)a1, 16); uint32_t aligned_addr = addr & ~7u; uint32_t offset = addr & 7u; uint64_t mem = A1_R64(aligned_addr); uint32_t shift = offset << 3; uint64_t keepMask = (offset == 0) ? 0ull : (0xFFFFFFFFFFFFFFFFull << ((8u - offset) << 3)); (t3 = (uint64_t)((t3 & keepMask) | (mem >> shift))); }
    { uint32_t addr = ADD32((uint32_t)a1, 31); uint32_t aligned_addr = addr & ~7u; uint32_t offset = addr & 7u; uint64_t mem = A1_R64(aligned_addr); uint32_t shift = (7u - offset) << 3; uint64_t keepMask = (shift == 0) ? 0ull : ((1ull << shift) - 1ull); (t4 = (uint64_t)((t4 & keepMask) | (mem << shift))); }
    { uint32_t addr = ADD32((uint32_t)a1, 24); uint32_t aligned_addr = addr & ~7u; uint32_t offset = addr & 7u; uint64_t mem = A1_R64(aligned_addr); uint32_t shift = offset << 3; uint64_t keepMask = (offset == 0) ? 0ull : (0xFFFFFFFFFFFFFFFFull << ((8u - offset) << 3)); (t4 = (uint64_t)((t4 & keepMask) | (mem >> shift))); }
    { uint32_t addr = ADD32((uint32_t)v1, 7); uint32_t aligned_addr = addr & ~7u; uint32_t offset = addr & 7u; uint32_t shift = (7u - offset) << 3; uint64_t mask = 0xFFFFFFFFFFFFFFFFull >> shift; uint64_t old_data = A1_R64(aligned_addr); uint64_t val = a0; uint64_t new_data = (old_data & ~mask) | ((val >> shift) & mask); A1_W64(aligned_addr, new_data); }
    { uint32_t addr = ADD32((uint32_t)v1, 0); uint32_t aligned_addr = addr & ~7u; uint32_t offset = addr & 7u; uint32_t shift = offset << 3; uint64_t mask = 0xFFFFFFFFFFFFFFFFull << shift; uint64_t old_data = A1_R64(aligned_addr); uint64_t val = a0; uint64_t new_data = (old_data & ~mask) | ((val << shift) & mask); A1_W64(aligned_addr, new_data); }
    { uint32_t addr = ADD32((uint32_t)v1, 15); uint32_t aligned_addr = addr & ~7u; uint32_t offset = addr & 7u; uint32_t shift = (7u - offset) << 3; uint64_t mask = 0xFFFFFFFFFFFFFFFFull >> shift; uint64_t old_data = A1_R64(aligned_addr); uint64_t val = t2; uint64_t new_data = (old_data & ~mask) | ((val >> shift) & mask); A1_W64(aligned_addr, new_data); }
    { uint32_t addr = ADD32((uint32_t)v1, 8); uint32_t aligned_addr = addr & ~7u; uint32_t offset = addr & 7u; uint32_t shift = offset << 3; uint64_t mask = 0xFFFFFFFFFFFFFFFFull << shift; uint64_t old_data = A1_R64(aligned_addr); uint64_t val = t2; uint64_t new_data = (old_data & ~mask) | ((val << shift) & mask); A1_W64(aligned_addr, new_data); }
    { uint32_t addr = ADD32((uint32_t)v1, 23); uint32_t aligned_addr = addr & ~7u; uint32_t offset = addr & 7u; uint32_t shift = (7u - offset) << 3; uint64_t mask = 0xFFFFFFFFFFFFFFFFull >> shift; uint64_t old_data = A1_R64(aligned_addr); uint64_t val = t3; uint64_t new_data = (old_data & ~mask) | ((val >> shift) & mask); A1_W64(aligned_addr, new_data); }
    { uint32_t addr = ADD32((uint32_t)v1, 16); uint32_t aligned_addr = addr & ~7u; uint32_t offset = addr & 7u; uint32_t shift = offset << 3; uint64_t mask = 0xFFFFFFFFFFFFFFFFull << shift; uint64_t old_data = A1_R64(aligned_addr); uint64_t val = t3; uint64_t new_data = (old_data & ~mask) | ((val << shift) & mask); A1_W64(aligned_addr, new_data); }
    { uint32_t addr = ADD32((uint32_t)v1, 31); uint32_t aligned_addr = addr & ~7u; uint32_t offset = addr & 7u; uint32_t shift = (7u - offset) << 3; uint64_t mask = 0xFFFFFFFFFFFFFFFFull >> shift; uint64_t old_data = A1_R64(aligned_addr); uint64_t val = t4; uint64_t new_data = (old_data & ~mask) | ((val >> shift) & mask); A1_W64(aligned_addr, new_data); }
    { uint32_t addr = ADD32((uint32_t)v1, 24); uint32_t aligned_addr = addr & ~7u; uint32_t offset = addr & 7u; uint32_t shift = offset << 3; uint64_t mask = 0xFFFFFFFFFFFFFFFFull << shift; uint64_t old_data = A1_R64(aligned_addr); uint64_t val = t4; uint64_t new_data = (old_data & ~mask) | ((val << shift) & mask); A1_W64(aligned_addr, new_data); }
    (a1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)a1, 32))));
    { const bool c = (int64_t)a1 != (int64_t)v0; (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v1, 32)))); if (c) { A3_BACK(); goto L_134f434; } }
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)286 << 16))));
    goto L_134f4c8;
L_134f490:;
    (t5 = (uint64_t)(A1_R64B((uint32_t)a1, 0)));
    (a0 = (uint64_t)(A1_R64B((uint32_t)a1, 8)));
    (t2 = (uint64_t)(A1_R64B((uint32_t)a1, 16)));
    (t3 = (uint64_t)(A1_R64B((uint32_t)a1, 24)));
    A1_W64B((uint32_t)v1, 0, t5);
    A1_W64B((uint32_t)v1, 8, a0);
    A1_W64B((uint32_t)v1, 16, t2);
    A1_W64B((uint32_t)v1, 24, t3);
    (a1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)a1, 32))));
    { const bool c = (int64_t)a1 != (int64_t)v0; (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v1, 32)))); if (c) { A3_BACK(); goto L_134f490; } }
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)286 << 16))));
L_134f4c8:;
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v0, 4294965384))));
    { uint32_t addr = ADD32((uint32_t)a1, 7); uint32_t aligned_addr = addr & ~7u; uint32_t offset = addr & 7u; uint64_t mem = A1_R64(aligned_addr); uint32_t shift = (7u - offset) << 3; uint64_t keepMask = (shift == 0) ? 0ull : ((1ull << shift) - 1ull); (t4 = (uint64_t)((t4 & keepMask) | (mem << shift))); }
    { uint32_t addr = ADD32((uint32_t)a1, 0); uint32_t aligned_addr = addr & ~7u; uint32_t offset = addr & 7u; uint64_t mem = A1_R64(aligned_addr); uint32_t shift = offset << 3; uint64_t keepMask = (offset == 0) ? 0ull : (0xFFFFFFFFFFFFFFFFull << ((8u - offset) << 3)); (t4 = (uint64_t)((t4 & keepMask) | (mem >> shift))); }
    { uint32_t addr = ADD32((uint32_t)v1, 7); uint32_t aligned_addr = addr & ~7u; uint32_t offset = addr & 7u; uint32_t shift = (7u - offset) << 3; uint64_t mask = 0xFFFFFFFFFFFFFFFFull >> shift; uint64_t old_data = A1_R64(aligned_addr); uint64_t val = t4; uint64_t new_data = (old_data & ~mask) | ((val >> shift) & mask); A1_W64(aligned_addr, new_data); }
    { uint32_t addr = ADD32((uint32_t)v1, 0); uint32_t aligned_addr = addr & ~7u; uint32_t offset = addr & 7u; uint32_t shift = offset << 3; uint64_t mask = 0xFFFFFFFFFFFFFFFFull << shift; uint64_t old_data = A1_R64(aligned_addr); uint64_t val = t4; uint64_t new_data = (old_data & ~mask) | ((val << shift) & mask); A1_W64(aligned_addr, new_data); }
    (v0 = (uint64_t)(a2 | RC_U64X(a0)));
    (v0 = (uint64_t)(v0 & (uint64_t)(uint16_t)7));
    { const bool c = (int64_t)v0 == 0; (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)a2, 384)))); if (c) goto L_134f548; }
L_134f4ec:;
    { uint32_t addr = ADD32((uint32_t)a2, 7); uint32_t aligned_addr = addr & ~7u; uint32_t offset = addr & 7u; uint64_t mem = A1_R64(aligned_addr); uint32_t shift = (7u - offset) << 3; uint64_t keepMask = (shift == 0) ? 0ull : ((1ull << shift) - 1ull); (t3 = (uint64_t)((t3 & keepMask) | (mem << shift))); }
    { uint32_t addr = ADD32((uint32_t)a2, 0); uint32_t aligned_addr = addr & ~7u; uint32_t offset = addr & 7u; uint64_t mem = A1_R64(aligned_addr); uint32_t shift = offset << 3; uint64_t keepMask = (offset == 0) ? 0ull : (0xFFFFFFFFFFFFFFFFull << ((8u - offset) << 3)); (t3 = (uint64_t)((t3 & keepMask) | (mem >> shift))); }
    { uint32_t addr = ADD32((uint32_t)a2, 15); uint32_t aligned_addr = addr & ~7u; uint32_t offset = addr & 7u; uint64_t mem = A1_R64(aligned_addr); uint32_t shift = (7u - offset) << 3; uint64_t keepMask = (shift == 0) ? 0ull : ((1ull << shift) - 1ull); (t4 = (uint64_t)((t4 & keepMask) | (mem << shift))); }
    { uint32_t addr = ADD32((uint32_t)a2, 8); uint32_t aligned_addr = addr & ~7u; uint32_t offset = addr & 7u; uint64_t mem = A1_R64(aligned_addr); uint32_t shift = offset << 3; uint64_t keepMask = (offset == 0) ? 0ull : (0xFFFFFFFFFFFFFFFFull << ((8u - offset) << 3)); (t4 = (uint64_t)((t4 & keepMask) | (mem >> shift))); }
    { uint32_t addr = ADD32((uint32_t)a2, 23); uint32_t aligned_addr = addr & ~7u; uint32_t offset = addr & 7u; uint64_t mem = A1_R64(aligned_addr); uint32_t shift = (7u - offset) << 3; uint64_t keepMask = (shift == 0) ? 0ull : ((1ull << shift) - 1ull); (t5 = (uint64_t)((t5 & keepMask) | (mem << shift))); }
    { uint32_t addr = ADD32((uint32_t)a2, 16); uint32_t aligned_addr = addr & ~7u; uint32_t offset = addr & 7u; uint64_t mem = A1_R64(aligned_addr); uint32_t shift = offset << 3; uint64_t keepMask = (offset == 0) ? 0ull : (0xFFFFFFFFFFFFFFFFull << ((8u - offset) << 3)); (t5 = (uint64_t)((t5 & keepMask) | (mem >> shift))); }
    { uint32_t addr = ADD32((uint32_t)a2, 31); uint32_t aligned_addr = addr & ~7u; uint32_t offset = addr & 7u; uint64_t mem = A1_R64(aligned_addr); uint32_t shift = (7u - offset) << 3; uint64_t keepMask = (shift == 0) ? 0ull : ((1ull << shift) - 1ull); (v1 = (uint64_t)((v1 & keepMask) | (mem << shift))); }
    { uint32_t addr = ADD32((uint32_t)a2, 24); uint32_t aligned_addr = addr & ~7u; uint32_t offset = addr & 7u; uint64_t mem = A1_R64(aligned_addr); uint32_t shift = offset << 3; uint64_t keepMask = (offset == 0) ? 0ull : (0xFFFFFFFFFFFFFFFFull << ((8u - offset) << 3)); (v1 = (uint64_t)((v1 & keepMask) | (mem >> shift))); }
    { uint32_t addr = ADD32((uint32_t)a0, 7); uint32_t aligned_addr = addr & ~7u; uint32_t offset = addr & 7u; uint32_t shift = (7u - offset) << 3; uint64_t mask = 0xFFFFFFFFFFFFFFFFull >> shift; uint64_t old_data = A1_R64(aligned_addr); uint64_t val = t3; uint64_t new_data = (old_data & ~mask) | ((val >> shift) & mask); A1_W64(aligned_addr, new_data); }
    { uint32_t addr = ADD32((uint32_t)a0, 0); uint32_t aligned_addr = addr & ~7u; uint32_t offset = addr & 7u; uint32_t shift = offset << 3; uint64_t mask = 0xFFFFFFFFFFFFFFFFull << shift; uint64_t old_data = A1_R64(aligned_addr); uint64_t val = t3; uint64_t new_data = (old_data & ~mask) | ((val << shift) & mask); A1_W64(aligned_addr, new_data); }
    { uint32_t addr = ADD32((uint32_t)a0, 15); uint32_t aligned_addr = addr & ~7u; uint32_t offset = addr & 7u; uint32_t shift = (7u - offset) << 3; uint64_t mask = 0xFFFFFFFFFFFFFFFFull >> shift; uint64_t old_data = A1_R64(aligned_addr); uint64_t val = t4; uint64_t new_data = (old_data & ~mask) | ((val >> shift) & mask); A1_W64(aligned_addr, new_data); }
    { uint32_t addr = ADD32((uint32_t)a0, 8); uint32_t aligned_addr = addr & ~7u; uint32_t offset = addr & 7u; uint32_t shift = offset << 3; uint64_t mask = 0xFFFFFFFFFFFFFFFFull << shift; uint64_t old_data = A1_R64(aligned_addr); uint64_t val = t4; uint64_t new_data = (old_data & ~mask) | ((val << shift) & mask); A1_W64(aligned_addr, new_data); }
    { uint32_t addr = ADD32((uint32_t)a0, 23); uint32_t aligned_addr = addr & ~7u; uint32_t offset = addr & 7u; uint32_t shift = (7u - offset) << 3; uint64_t mask = 0xFFFFFFFFFFFFFFFFull >> shift; uint64_t old_data = A1_R64(aligned_addr); uint64_t val = t5; uint64_t new_data = (old_data & ~mask) | ((val >> shift) & mask); A1_W64(aligned_addr, new_data); }
    { uint32_t addr = ADD32((uint32_t)a0, 16); uint32_t aligned_addr = addr & ~7u; uint32_t offset = addr & 7u; uint32_t shift = offset << 3; uint64_t mask = 0xFFFFFFFFFFFFFFFFull << shift; uint64_t old_data = A1_R64(aligned_addr); uint64_t val = t5; uint64_t new_data = (old_data & ~mask) | ((val << shift) & mask); A1_W64(aligned_addr, new_data); }
    { uint32_t addr = ADD32((uint32_t)a0, 31); uint32_t aligned_addr = addr & ~7u; uint32_t offset = addr & 7u; uint32_t shift = (7u - offset) << 3; uint64_t mask = 0xFFFFFFFFFFFFFFFFull >> shift; uint64_t old_data = A1_R64(aligned_addr); uint64_t val = v1; uint64_t new_data = (old_data & ~mask) | ((val >> shift) & mask); A1_W64(aligned_addr, new_data); }
    { uint32_t addr = ADD32((uint32_t)a0, 24); uint32_t aligned_addr = addr & ~7u; uint32_t offset = addr & 7u; uint32_t shift = offset << 3; uint64_t mask = 0xFFFFFFFFFFFFFFFFull << shift; uint64_t old_data = A1_R64(aligned_addr); uint64_t val = v1; uint64_t new_data = (old_data & ~mask) | ((val << shift) & mask); A1_W64(aligned_addr, new_data); }
    (a2 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)a2, 32))));
    { const bool c = (int64_t)a2 != (int64_t)v0; (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)a0, 32)))); if (c) { A3_BACK(); goto L_134f4ec; } }
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)286 << 16))));
    goto L_134f580;
L_134f548:;
    (a1 = (uint64_t)(A1_R64B((uint32_t)a2, 0)));
    (t2 = (uint64_t)(A1_R64B((uint32_t)a2, 8)));
    (t3 = (uint64_t)(A1_R64B((uint32_t)a2, 16)));
    (t4 = (uint64_t)(A1_R64B((uint32_t)a2, 24)));
    A1_W64B((uint32_t)a0, 0, a1);
    A1_W64B((uint32_t)a0, 8, t2);
    A1_W64B((uint32_t)a0, 16, t3);
    A1_W64B((uint32_t)a0, 24, t4);
    (a2 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)a2, 32))));
    { const bool c = (int64_t)a2 != (int64_t)v0; (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)a0, 32)))); if (c) { A3_BACK(); goto L_134f548; } }
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)286 << 16))));
L_134f580:;
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v0, 4294965784))));
    { uint32_t addr = ADD32((uint32_t)a2, 7); uint32_t aligned_addr = addr & ~7u; uint32_t offset = addr & 7u; uint64_t mem = A1_R64(aligned_addr); uint32_t shift = (7u - offset) << 3; uint64_t keepMask = (shift == 0) ? 0ull : ((1ull << shift) - 1ull); (t5 = (uint64_t)((t5 & keepMask) | (mem << shift))); }
    { uint32_t addr = ADD32((uint32_t)a2, 0); uint32_t aligned_addr = addr & ~7u; uint32_t offset = addr & 7u; uint64_t mem = A1_R64(aligned_addr); uint32_t shift = offset << 3; uint64_t keepMask = (offset == 0) ? 0ull : (0xFFFFFFFFFFFFFFFFull << ((8u - offset) << 3)); (t5 = (uint64_t)((t5 & keepMask) | (mem >> shift))); }
    { uint32_t addr = ADD32((uint32_t)a2, 15); uint32_t aligned_addr = addr & ~7u; uint32_t offset = addr & 7u; uint64_t mem = A1_R64(aligned_addr); uint32_t shift = (7u - offset) << 3; uint64_t keepMask = (shift == 0) ? 0ull : ((1ull << shift) - 1ull); (v0 = (uint64_t)((v0 & keepMask) | (mem << shift))); }
    { uint32_t addr = ADD32((uint32_t)a2, 8); uint32_t aligned_addr = addr & ~7u; uint32_t offset = addr & 7u; uint64_t mem = A1_R64(aligned_addr); uint32_t shift = offset << 3; uint64_t keepMask = (offset == 0) ? 0ull : (0xFFFFFFFFFFFFFFFFull << ((8u - offset) << 3)); (v0 = (uint64_t)((v0 & keepMask) | (mem >> shift))); }
    { uint32_t addr = ADD32((uint32_t)a0, 7); uint32_t aligned_addr = addr & ~7u; uint32_t offset = addr & 7u; uint32_t shift = (7u - offset) << 3; uint64_t mask = 0xFFFFFFFFFFFFFFFFull >> shift; uint64_t old_data = A1_R64(aligned_addr); uint64_t val = t5; uint64_t new_data = (old_data & ~mask) | ((val >> shift) & mask); A1_W64(aligned_addr, new_data); }
    { uint32_t addr = ADD32((uint32_t)a0, 0); uint32_t aligned_addr = addr & ~7u; uint32_t offset = addr & 7u; uint32_t shift = offset << 3; uint64_t mask = 0xFFFFFFFFFFFFFFFFull << shift; uint64_t old_data = A1_R64(aligned_addr); uint64_t val = t5; uint64_t new_data = (old_data & ~mask) | ((val << shift) & mask); A1_W64(aligned_addr, new_data); }
    { uint32_t addr = ADD32((uint32_t)a0, 15); uint32_t aligned_addr = addr & ~7u; uint32_t offset = addr & 7u; uint32_t shift = (7u - offset) << 3; uint64_t mask = 0xFFFFFFFFFFFFFFFFull >> shift; uint64_t old_data = A1_R64(aligned_addr); uint64_t val = v0; uint64_t new_data = (old_data & ~mask) | ((val >> shift) & mask); A1_W64(aligned_addr, new_data); }
    { uint32_t addr = ADD32((uint32_t)a0, 8); uint32_t aligned_addr = addr & ~7u; uint32_t offset = addr & 7u; uint32_t shift = offset << 3; uint64_t mask = 0xFFFFFFFFFFFFFFFFull << shift; uint64_t old_data = A1_R64(aligned_addr); uint64_t val = v0; uint64_t new_data = (old_data & ~mask) | ((val << shift) & mask); A1_W64(aligned_addr, new_data); }
    (v0 = (uint64_t)(a3 | RC_U64X(v1)));
    (v0 = (uint64_t)(v0 & (uint64_t)(uint16_t)7));
    { const bool c = (int64_t)v0 == 0; (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)a3, 800)))); if (c) goto L_134f610; }
L_134f5b4:;
    { uint32_t addr = ADD32((uint32_t)a3, 7); uint32_t aligned_addr = addr & ~7u; uint32_t offset = addr & 7u; uint64_t mem = A1_R64(aligned_addr); uint32_t shift = (7u - offset) << 3; uint64_t keepMask = (shift == 0) ? 0ull : ((1ull << shift) - 1ull); (t3 = (uint64_t)((t3 & keepMask) | (mem << shift))); }
    { uint32_t addr = ADD32((uint32_t)a3, 0); uint32_t aligned_addr = addr & ~7u; uint32_t offset = addr & 7u; uint64_t mem = A1_R64(aligned_addr); uint32_t shift = offset << 3; uint64_t keepMask = (offset == 0) ? 0ull : (0xFFFFFFFFFFFFFFFFull << ((8u - offset) << 3)); (t3 = (uint64_t)((t3 & keepMask) | (mem >> shift))); }
    { uint32_t addr = ADD32((uint32_t)a3, 15); uint32_t aligned_addr = addr & ~7u; uint32_t offset = addr & 7u; uint64_t mem = A1_R64(aligned_addr); uint32_t shift = (7u - offset) << 3; uint64_t keepMask = (shift == 0) ? 0ull : ((1ull << shift) - 1ull); (t4 = (uint64_t)((t4 & keepMask) | (mem << shift))); }
    { uint32_t addr = ADD32((uint32_t)a3, 8); uint32_t aligned_addr = addr & ~7u; uint32_t offset = addr & 7u; uint64_t mem = A1_R64(aligned_addr); uint32_t shift = offset << 3; uint64_t keepMask = (offset == 0) ? 0ull : (0xFFFFFFFFFFFFFFFFull << ((8u - offset) << 3)); (t4 = (uint64_t)((t4 & keepMask) | (mem >> shift))); }
    { uint32_t addr = ADD32((uint32_t)a3, 23); uint32_t aligned_addr = addr & ~7u; uint32_t offset = addr & 7u; uint64_t mem = A1_R64(aligned_addr); uint32_t shift = (7u - offset) << 3; uint64_t keepMask = (shift == 0) ? 0ull : ((1ull << shift) - 1ull); (t5 = (uint64_t)((t5 & keepMask) | (mem << shift))); }
    { uint32_t addr = ADD32((uint32_t)a3, 16); uint32_t aligned_addr = addr & ~7u; uint32_t offset = addr & 7u; uint64_t mem = A1_R64(aligned_addr); uint32_t shift = offset << 3; uint64_t keepMask = (offset == 0) ? 0ull : (0xFFFFFFFFFFFFFFFFull << ((8u - offset) << 3)); (t5 = (uint64_t)((t5 & keepMask) | (mem >> shift))); }
    { uint32_t addr = ADD32((uint32_t)a3, 31); uint32_t aligned_addr = addr & ~7u; uint32_t offset = addr & 7u; uint64_t mem = A1_R64(aligned_addr); uint32_t shift = (7u - offset) << 3; uint64_t keepMask = (shift == 0) ? 0ull : ((1ull << shift) - 1ull); (a0 = (uint64_t)((a0 & keepMask) | (mem << shift))); }
    { uint32_t addr = ADD32((uint32_t)a3, 24); uint32_t aligned_addr = addr & ~7u; uint32_t offset = addr & 7u; uint64_t mem = A1_R64(aligned_addr); uint32_t shift = offset << 3; uint64_t keepMask = (offset == 0) ? 0ull : (0xFFFFFFFFFFFFFFFFull << ((8u - offset) << 3)); (a0 = (uint64_t)((a0 & keepMask) | (mem >> shift))); }
    { uint32_t addr = ADD32((uint32_t)v1, 7); uint32_t aligned_addr = addr & ~7u; uint32_t offset = addr & 7u; uint32_t shift = (7u - offset) << 3; uint64_t mask = 0xFFFFFFFFFFFFFFFFull >> shift; uint64_t old_data = A1_R64(aligned_addr); uint64_t val = t3; uint64_t new_data = (old_data & ~mask) | ((val >> shift) & mask); A1_W64(aligned_addr, new_data); }
    { uint32_t addr = ADD32((uint32_t)v1, 0); uint32_t aligned_addr = addr & ~7u; uint32_t offset = addr & 7u; uint32_t shift = offset << 3; uint64_t mask = 0xFFFFFFFFFFFFFFFFull << shift; uint64_t old_data = A1_R64(aligned_addr); uint64_t val = t3; uint64_t new_data = (old_data & ~mask) | ((val << shift) & mask); A1_W64(aligned_addr, new_data); }
    { uint32_t addr = ADD32((uint32_t)v1, 15); uint32_t aligned_addr = addr & ~7u; uint32_t offset = addr & 7u; uint32_t shift = (7u - offset) << 3; uint64_t mask = 0xFFFFFFFFFFFFFFFFull >> shift; uint64_t old_data = A1_R64(aligned_addr); uint64_t val = t4; uint64_t new_data = (old_data & ~mask) | ((val >> shift) & mask); A1_W64(aligned_addr, new_data); }
    { uint32_t addr = ADD32((uint32_t)v1, 8); uint32_t aligned_addr = addr & ~7u; uint32_t offset = addr & 7u; uint32_t shift = offset << 3; uint64_t mask = 0xFFFFFFFFFFFFFFFFull << shift; uint64_t old_data = A1_R64(aligned_addr); uint64_t val = t4; uint64_t new_data = (old_data & ~mask) | ((val << shift) & mask); A1_W64(aligned_addr, new_data); }
    { uint32_t addr = ADD32((uint32_t)v1, 23); uint32_t aligned_addr = addr & ~7u; uint32_t offset = addr & 7u; uint32_t shift = (7u - offset) << 3; uint64_t mask = 0xFFFFFFFFFFFFFFFFull >> shift; uint64_t old_data = A1_R64(aligned_addr); uint64_t val = t5; uint64_t new_data = (old_data & ~mask) | ((val >> shift) & mask); A1_W64(aligned_addr, new_data); }
    { uint32_t addr = ADD32((uint32_t)v1, 16); uint32_t aligned_addr = addr & ~7u; uint32_t offset = addr & 7u; uint32_t shift = offset << 3; uint64_t mask = 0xFFFFFFFFFFFFFFFFull << shift; uint64_t old_data = A1_R64(aligned_addr); uint64_t val = t5; uint64_t new_data = (old_data & ~mask) | ((val << shift) & mask); A1_W64(aligned_addr, new_data); }
    { uint32_t addr = ADD32((uint32_t)v1, 31); uint32_t aligned_addr = addr & ~7u; uint32_t offset = addr & 7u; uint32_t shift = (7u - offset) << 3; uint64_t mask = 0xFFFFFFFFFFFFFFFFull >> shift; uint64_t old_data = A1_R64(aligned_addr); uint64_t val = a0; uint64_t new_data = (old_data & ~mask) | ((val >> shift) & mask); A1_W64(aligned_addr, new_data); }
    { uint32_t addr = ADD32((uint32_t)v1, 24); uint32_t aligned_addr = addr & ~7u; uint32_t offset = addr & 7u; uint32_t shift = offset << 3; uint64_t mask = 0xFFFFFFFFFFFFFFFFull << shift; uint64_t old_data = A1_R64(aligned_addr); uint64_t val = a0; uint64_t new_data = (old_data & ~mask) | ((val << shift) & mask); A1_W64(aligned_addr, new_data); }
    (a3 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)a3, 32))));
    { const bool c = (int64_t)a3 != (int64_t)v0; (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v1, 32)))); if (c) { A3_BACK(); goto L_134f5b4; } }
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)286 << 16))));
    goto L_134f648;
L_134f610:;
    (a1 = (uint64_t)(A1_R64B((uint32_t)a3, 0)));
    (a2 = (uint64_t)(A1_R64B((uint32_t)a3, 8)));
    (t2 = (uint64_t)(A1_R64B((uint32_t)a3, 16)));
    (t3 = (uint64_t)(A1_R64B((uint32_t)a3, 24)));
    A1_W64B((uint32_t)v1, 0, a1);
    A1_W64B((uint32_t)v1, 8, a2);
    A1_W64B((uint32_t)v1, 16, t2);
    A1_W64B((uint32_t)v1, 24, t3);
    (a3 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)a3, 32))));
    { const bool c = (int64_t)a3 != (int64_t)v0; (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v1, 32)))); if (c) { A3_BACK(); goto L_134f610; } }
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)286 << 16))));
L_134f648:;
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v1, 4294966584))));
    (v0 = (uint64_t)(t0 | RC_U64X(v1)));
    (v0 = (uint64_t)(v0 & (uint64_t)(uint16_t)7));
    { const bool c = (int64_t)v0 == 0; (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)t0, 800)))); if (c) goto L_134f6b8; }
L_134f65c:;
    { uint32_t addr = ADD32((uint32_t)t0, 7); uint32_t aligned_addr = addr & ~7u; uint32_t offset = addr & 7u; uint64_t mem = A1_R64(aligned_addr); uint32_t shift = (7u - offset) << 3; uint64_t keepMask = (shift == 0) ? 0ull : ((1ull << shift) - 1ull); (t4 = (uint64_t)((t4 & keepMask) | (mem << shift))); }
    { uint32_t addr = ADD32((uint32_t)t0, 0); uint32_t aligned_addr = addr & ~7u; uint32_t offset = addr & 7u; uint64_t mem = A1_R64(aligned_addr); uint32_t shift = offset << 3; uint64_t keepMask = (offset == 0) ? 0ull : (0xFFFFFFFFFFFFFFFFull << ((8u - offset) << 3)); (t4 = (uint64_t)((t4 & keepMask) | (mem >> shift))); }
    { uint32_t addr = ADD32((uint32_t)t0, 15); uint32_t aligned_addr = addr & ~7u; uint32_t offset = addr & 7u; uint64_t mem = A1_R64(aligned_addr); uint32_t shift = (7u - offset) << 3; uint64_t keepMask = (shift == 0) ? 0ull : ((1ull << shift) - 1ull); (t5 = (uint64_t)((t5 & keepMask) | (mem << shift))); }
    { uint32_t addr = ADD32((uint32_t)t0, 8); uint32_t aligned_addr = addr & ~7u; uint32_t offset = addr & 7u; uint64_t mem = A1_R64(aligned_addr); uint32_t shift = offset << 3; uint64_t keepMask = (offset == 0) ? 0ull : (0xFFFFFFFFFFFFFFFFull << ((8u - offset) << 3)); (t5 = (uint64_t)((t5 & keepMask) | (mem >> shift))); }
    { uint32_t addr = ADD32((uint32_t)t0, 23); uint32_t aligned_addr = addr & ~7u; uint32_t offset = addr & 7u; uint64_t mem = A1_R64(aligned_addr); uint32_t shift = (7u - offset) << 3; uint64_t keepMask = (shift == 0) ? 0ull : ((1ull << shift) - 1ull); (a0 = (uint64_t)((a0 & keepMask) | (mem << shift))); }
    { uint32_t addr = ADD32((uint32_t)t0, 16); uint32_t aligned_addr = addr & ~7u; uint32_t offset = addr & 7u; uint64_t mem = A1_R64(aligned_addr); uint32_t shift = offset << 3; uint64_t keepMask = (offset == 0) ? 0ull : (0xFFFFFFFFFFFFFFFFull << ((8u - offset) << 3)); (a0 = (uint64_t)((a0 & keepMask) | (mem >> shift))); }
    { uint32_t addr = ADD32((uint32_t)t0, 31); uint32_t aligned_addr = addr & ~7u; uint32_t offset = addr & 7u; uint64_t mem = A1_R64(aligned_addr); uint32_t shift = (7u - offset) << 3; uint64_t keepMask = (shift == 0) ? 0ull : ((1ull << shift) - 1ull); (a1 = (uint64_t)((a1 & keepMask) | (mem << shift))); }
    { uint32_t addr = ADD32((uint32_t)t0, 24); uint32_t aligned_addr = addr & ~7u; uint32_t offset = addr & 7u; uint64_t mem = A1_R64(aligned_addr); uint32_t shift = offset << 3; uint64_t keepMask = (offset == 0) ? 0ull : (0xFFFFFFFFFFFFFFFFull << ((8u - offset) << 3)); (a1 = (uint64_t)((a1 & keepMask) | (mem >> shift))); }
    { uint32_t addr = ADD32((uint32_t)v1, 7); uint32_t aligned_addr = addr & ~7u; uint32_t offset = addr & 7u; uint32_t shift = (7u - offset) << 3; uint64_t mask = 0xFFFFFFFFFFFFFFFFull >> shift; uint64_t old_data = A1_R64(aligned_addr); uint64_t val = t4; uint64_t new_data = (old_data & ~mask) | ((val >> shift) & mask); A1_W64(aligned_addr, new_data); }
    { uint32_t addr = ADD32((uint32_t)v1, 0); uint32_t aligned_addr = addr & ~7u; uint32_t offset = addr & 7u; uint32_t shift = offset << 3; uint64_t mask = 0xFFFFFFFFFFFFFFFFull << shift; uint64_t old_data = A1_R64(aligned_addr); uint64_t val = t4; uint64_t new_data = (old_data & ~mask) | ((val << shift) & mask); A1_W64(aligned_addr, new_data); }
    { uint32_t addr = ADD32((uint32_t)v1, 15); uint32_t aligned_addr = addr & ~7u; uint32_t offset = addr & 7u; uint32_t shift = (7u - offset) << 3; uint64_t mask = 0xFFFFFFFFFFFFFFFFull >> shift; uint64_t old_data = A1_R64(aligned_addr); uint64_t val = t5; uint64_t new_data = (old_data & ~mask) | ((val >> shift) & mask); A1_W64(aligned_addr, new_data); }
    { uint32_t addr = ADD32((uint32_t)v1, 8); uint32_t aligned_addr = addr & ~7u; uint32_t offset = addr & 7u; uint32_t shift = offset << 3; uint64_t mask = 0xFFFFFFFFFFFFFFFFull << shift; uint64_t old_data = A1_R64(aligned_addr); uint64_t val = t5; uint64_t new_data = (old_data & ~mask) | ((val << shift) & mask); A1_W64(aligned_addr, new_data); }
    { uint32_t addr = ADD32((uint32_t)v1, 23); uint32_t aligned_addr = addr & ~7u; uint32_t offset = addr & 7u; uint32_t shift = (7u - offset) << 3; uint64_t mask = 0xFFFFFFFFFFFFFFFFull >> shift; uint64_t old_data = A1_R64(aligned_addr); uint64_t val = a0; uint64_t new_data = (old_data & ~mask) | ((val >> shift) & mask); A1_W64(aligned_addr, new_data); }
    { uint32_t addr = ADD32((uint32_t)v1, 16); uint32_t aligned_addr = addr & ~7u; uint32_t offset = addr & 7u; uint32_t shift = offset << 3; uint64_t mask = 0xFFFFFFFFFFFFFFFFull << shift; uint64_t old_data = A1_R64(aligned_addr); uint64_t val = a0; uint64_t new_data = (old_data & ~mask) | ((val << shift) & mask); A1_W64(aligned_addr, new_data); }
    { uint32_t addr = ADD32((uint32_t)v1, 31); uint32_t aligned_addr = addr & ~7u; uint32_t offset = addr & 7u; uint32_t shift = (7u - offset) << 3; uint64_t mask = 0xFFFFFFFFFFFFFFFFull >> shift; uint64_t old_data = A1_R64(aligned_addr); uint64_t val = a1; uint64_t new_data = (old_data & ~mask) | ((val >> shift) & mask); A1_W64(aligned_addr, new_data); }
    { uint32_t addr = ADD32((uint32_t)v1, 24); uint32_t aligned_addr = addr & ~7u; uint32_t offset = addr & 7u; uint32_t shift = offset << 3; uint64_t mask = 0xFFFFFFFFFFFFFFFFull << shift; uint64_t old_data = A1_R64(aligned_addr); uint64_t val = a1; uint64_t new_data = (old_data & ~mask) | ((val << shift) & mask); A1_W64(aligned_addr, new_data); }
    (t0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)t0, 32))));
    { const bool c = (int64_t)t0 != (int64_t)v0; (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v1, 32)))); if (c) { A3_BACK(); goto L_134f65c; } }
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)286 << 16))));
    goto L_134f6f0;
L_134f6b8:;
    (a2 = (uint64_t)(A1_R64B((uint32_t)t0, 0)));
    (a3 = (uint64_t)(A1_R64B((uint32_t)t0, 8)));
    (t2 = (uint64_t)(A1_R64B((uint32_t)t0, 16)));
    (t3 = (uint64_t)(A1_R64B((uint32_t)t0, 24)));
    A1_W64B((uint32_t)v1, 0, a2);
    A1_W64B((uint32_t)v1, 8, a3);
    A1_W64B((uint32_t)v1, 16, t2);
    A1_W64B((uint32_t)v1, 24, t3);
    (t0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)t0, 32))));
    { const bool c = (int64_t)t0 != (int64_t)v0; (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v1, 32)))); if (c) { A3_BACK(); goto L_134f6b8; } }
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)286 << 16))));
L_134f6f0:;
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)t1, 96))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v0, 88))));
L_134f6f8:;
    (t4 = (uint64_t)(A1_R64B((uint32_t)t1, 0)));
    (t5 = (uint64_t)(A1_R64B((uint32_t)t1, 8)));
    (a0 = (uint64_t)(A1_R64B((uint32_t)t1, 16)));
    (a1 = (uint64_t)(A1_R64B((uint32_t)t1, 24)));
    A1_W64B((uint32_t)v0, 0, t4);
    A1_W64B((uint32_t)v0, 8, t5);
    A1_W64B((uint32_t)v0, 16, a0);
    A1_W64B((uint32_t)v0, 24, a1);
    (t1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)t1, 32))));
    { const bool c = (int64_t)t1 != (int64_t)v1; (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v0, 32)))); if (c) { A3_BACK(); goto L_134f6f8; } }
    (a2 = (uint64_t)(A1_R64B((uint32_t)t1, 0)));
    (a3 = (uint64_t)(A1_R64B((uint32_t)t1, 8)));
    A1_W64B((uint32_t)v0, 0, a2);
    { A1_W64B((uint32_t)v0, 8, a3); if (xo) { xo[0] = a1; xo[1] = a2; xo[2] = a3; xo[3] = t0; xo[4] = t1; xo[5] = t2; xo[6] = t3; } return v0; }
    __builtin_unreachable();
}

// tailsFlagChk (0x134f820): typed by tools/typed_lift2.py (round A3; leaf, extra outputs xo[0]=t0)
inline uint32_t tailsFlagChk(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint64_t a0, uint64_t a1, uint32_t a3, uint64_t *xo = nullptr)
{
    uint32_t v0 = 0;
    uint64_t v1 = 0;
    uint64_t a2 = 0;
    uint32_t t0 = 0;
    v0 = 0x11E0000u;
    (v1 = (uint64_t)(((int64_t)(int64_t)a0 < (int64_t)(int32_t)100) ? 1 : 0));
    (a2 = (uint64_t)(((int64_t)(int64_t)a1 < (int64_t)(int32_t)64) ? 1 : 0));
    { const bool c = (int64_t)v1 == 0; t0 = v0 - 0x5E8u; if (c) goto L_134f83c; }
    if ((int64_t)a2 != 0) { (v0 = (uint32_t)((int32_t)((int32_t)SLL32((uint32_t)a0, 6)))); goto L_134f848; }
L_134f83c:;
    v0 = 0u;
    goto L_134f858;
L_134f848:;
    (v0 = (uint32_t)((int32_t)((int32_t)ADD32(v0, (uint32_t)a1))));
    (v1 = (uint64_t)((int64_t)(int32_t)(SRA32((int32_t)v0, 3))));
    a3 = v0 & 7u;
    (v0 = (uint32_t)((int32_t)((int32_t)ADD32((uint32_t)v1, t0))));
L_134f858:;
    if (v0 == 0) { v0 = 0u; goto L_134f878; }
    (v1 = (uint64_t)(uint32_t)((uint8_t)A1_R8B(v0, 0)));
    (v1 = (uint64_t)((int64_t)(int32_t)(SRA32((int32_t)v1, a3 & 0x1F))));
    (v1 = (uint64_t)(RC_U64X(v1) & (uint64_t)(uint16_t)1));
    { const bool c = (int64_t)v1 != 0; v0 = 1u; if (c) goto L_134f878; }
    v0 = 0u;
L_134f878:;
    { if (xo) { xo[0] = a1_sx64(t0); } return v0; }
    __builtin_unreachable();
}

// gexsFlagChk (0x134f938): typed by tools/typed_lift2.py (round A3; leaf, extra outputs xo[0]=a2 xo[1]=a3 xo[2]=t0)
inline uint32_t gexsFlagChk(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint64_t a0, uint64_t a1, uint32_t a3, uint64_t *xo = nullptr)
{
    uint32_t v0 = 0;
    uint64_t v1 = 0;
    uint64_t a2 = 0;
    uint32_t t0 = 0;
    bool a3xw1 = false;
    v0 = 0x11E0000u;
    (v1 = (uint64_t)(((int64_t)(int64_t)a0 < (int64_t)(int32_t)100) ? 1 : 0));
    (a2 = (uint64_t)(((int64_t)(int64_t)a1 < (int64_t)(int32_t)64) ? 1 : 0));
    { const bool c = (int64_t)v1 == 0; t0 = v0 - 0x2C8u; if (c) goto L_134f954; }
    if ((int64_t)a2 != 0) { (v0 = (uint32_t)((int32_t)((int32_t)SLL32((uint32_t)a0, 6)))); goto L_134f960; }
L_134f954:;
    v0 = 0u;
    goto L_134f970;
L_134f960:;
    (v0 = (uint32_t)((int32_t)((int32_t)ADD32(v0, (uint32_t)a1))));
    (v1 = (uint64_t)((int64_t)(int32_t)(SRA32((int32_t)v0, 3))));
    a3 = v0 & 7u; a3xw1 = true;
    (v0 = (uint32_t)((int32_t)((int32_t)ADD32((uint32_t)v1, t0))));
L_134f970:;
    if (v0 == 0) { v0 = 0u; goto L_134f990; }
    (v1 = (uint64_t)(uint32_t)((uint8_t)A1_R8B(v0, 0)));
    (v1 = (uint64_t)((int64_t)(int32_t)(SRA32((int32_t)v1, a3 & 0x1F))));
    (v1 = (uint64_t)(RC_U64X(v1) & (uint64_t)(uint16_t)1));
    { const bool c = (int64_t)v1 != 0; v0 = 1u; if (c) goto L_134f990; }
    v0 = 0u;
L_134f990:;
    { if (xo) { xo[0] = a2; if (a3xw1) xo[1] = a1_sx64(a3); xo[2] = a1_sx64(t0); } return v0; }
    __builtin_unreachable();
}

// _omakeGetFlagChk (0x134fa18): typed by tools/typed_lift2.py (round A3; leaf, extra outputs xo[0]=a1)
inline uint64_t _omakeGetFlagChk(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint64_t a0, uint64_t *xo = nullptr)
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
    (v0 = (uint64_t)(uint32_t)((uint8_t)A1_R8B((uint32_t)v0, 90)));
    (v0 = (uint64_t)((int64_t)(int32_t)(SRA32((int32_t)v0, (uint32_t)a0 & 0x1F))));
    { (v0 = (uint64_t)(RC_U64X(v0) & (uint64_t)(uint16_t)1)); if (xo) { xo[0] = a1_sx64(a1); } return v0; }
    __builtin_unreachable();
}

// _omakeSelectFlagChk (0x134fb28): typed by tools/typed_lift2.py (round A3; leaf, extra outputs xo[0]=a0 xo[1]=a1)
inline uint64_t _omakeSelectFlagChk(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint64_t a0, uint64_t *xo = nullptr)
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
    (v0 = (uint64_t)(uint32_t)((uint8_t)A1_R8B((uint32_t)v0, 98)));
    (v0 = (uint64_t)((int64_t)(int32_t)(SRA32((int32_t)v0, (uint32_t)a0 & 0x1F))));
    { (v0 = (uint64_t)(RC_U64X(v0) & (uint64_t)(uint16_t)1)); if (xo) { xo[0] = a0; xo[1] = a1_sx64(a1); } return v0; }
    __builtin_unreachable();
}

// gameflowBossEyeCameraDisable (0x1352030): typed by tools/typed_lift2.py (round A3; leaf, extra outputs xo[0]=a1 xo[1]=a2)
inline uint32_t gameflowBossEyeCameraDisable(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint64_t a0, uint64_t *xo = nullptr)
{
    uint32_t v0 = 0;
    uint32_t v1 = 0;
    uint64_t a1 = 0;
    uint32_t a2 = 0;
    v0 = 0x1460000u;
    v1 = 1u;
    a2 = v0 + 0x5BB4u;
    { const bool c = (int64_t)a0 != (int64_t)(int32_t)v1; (a1 = (uint64_t)((uint64_t)a1_sx64(a2) + (uint64_t)0ULL)); if (c) goto L_1352050; }
    (v0 = (uint32_t)((int32_t)((int32_t)A1_R32B((uint32_t)a1, 0))));
    v0 = v0 + 1u;
    goto L_1352058;
L_1352050:;
    (v0 = (uint32_t)((int32_t)((int32_t)A1_R32B((uint32_t)a1, 0))));
    v0 = v0 - 1u;
L_1352058:;
    A1_W32B((uint32_t)a1, 0, v0);
    v0 = A1_R32(a2);
    if ((int32_t)v0 < 0) { A1_W32(a2, 0u); goto L_1352068; }
L_1352068:;
    { if (xo) { xo[0] = a1; xo[1] = a1_sx64(a2); } return v0; }
    __builtin_unreachable();
}

// gameModeSet (0x1352b30): typed by tools/typed_lift2.py (round A3; loop leaf, extra outputs xo[0]=a0 xo[1]=a1 xo[2]=a2)
inline uint64_t gameModeSet(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint64_t a0, uint64_t *xo = nullptr)
{
    uint64_t v0 = 0;
    uint32_t v1 = 0;
    uint64_t a1 = 0;
    uint32_t a2 = 0;
    bool a3xw0 = false;
    bool a3xw2 = false;
    v1 = 0x1460000u;
    (a1 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL));
    v1 = v1 + 0x5D30u;
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B(v1, 0))));
    (v0 = (uint64_t)(RC_U64X(v0) | a0));
    { const bool c = (int64_t)a0 == 0; A1_W32B(v1, 0, (uint32_t)v0); if (c) goto L_1352b7c; }
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)286 << 16))));
    (a2 = (uint32_t)((int32_t)((int32_t)ADD32((uint32_t)v0, 368)))); a3xw2 = true;
L_1352b58:;
    (v0 = (uint64_t)(a0 & (uint64_t)(uint16_t)1));
    { const bool c = (int64_t)v0 == 0; (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)SRL32((uint32_t)a0, 1)))); a3xw0 = true; if (c) goto L_1352b74; }
    (v1 = (uint32_t)((int32_t)((int32_t)ADD32((uint32_t)a1, a2))));
    (v0 = (uint64_t)(uint32_t)((uint8_t)A1_R8B(v1, 0)));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v0, 1))));
    A1_W8B(v1, 0, (uint8_t)(uint32_t)v0);
L_1352b74:;
    { const bool c = (int64_t)a0 != 0; (a1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)a1, 1)))); if (c) { A3_BACK(); goto L_1352b58; } }
L_1352b7c:;
    { if (xo) { if (a3xw0) xo[0] = a0; xo[1] = a1; if (a3xw2) xo[2] = a1_sx64(a2); } return v0; }
    __builtin_unreachable();
}

// gameModeCls (0x1352b88): typed by tools/typed_lift2.py (round A3; loop leaf, extra outputs xo[0]=v0 xo[1]=a0 xo[2]=a1 xo[3]=a2 xo[4]=a3 xo[5]=t0 xo[6]=t1)
inline void gameModeCls(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint64_t a0, uint64_t *xo = nullptr)
{
    uint64_t v0 = 0;
    uint64_t v1 = 0;
    uint32_t a1 = 0;
    uint64_t a2 = 0;
    uint32_t a3 = 0;
    uint32_t t0 = 0;
    uint32_t t1 = 0;
    bool a3xw0 = false;
    bool a3xw1 = false;
    bool a3xw2 = false;
    bool a3xw4 = false;
    bool a3xw5 = false;
    bool a3xw6 = false;
    { const bool c = (int64_t)a0 == 0; (a2 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL)); if (c) goto L_1352bf8; }
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)286 << 16)))); a3xw0 = true;
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)326 << 16))));
    (t1 = (uint32_t)((int32_t)((int32_t)ADD32((uint32_t)v0, 368)))); a3xw6 = true;
    (a3 = (uint32_t)((int32_t)((int32_t)ADD32((uint32_t)v1, 23856)))); a3xw4 = true;
    t0 = 1u; a3xw5 = true;
L_1352ba8:;
    (v0 = (uint64_t)(a0 & (uint64_t)(uint16_t)1)); a3xw0 = true;
    { const bool c = (int64_t)v0 == 0; (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)SRL32((uint32_t)a0, 1)))); a3xw1 = true; if (c) goto L_1352bf0; }
    (a1 = (uint32_t)((int32_t)((int32_t)ADD32((uint32_t)a2, t1)))); a3xw2 = true;
    (v0 = (uint64_t)(uint32_t)((uint8_t)A1_R8B(a1, 0))); a3xw0 = true;
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v0, 4294967295)))); a3xw0 = true;
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)SLL32((uint32_t)v0, 24))));
    (v1 = (uint64_t)((int64_t)(int32_t)(SRA32((int32_t)v1, 24))));
    { const bool c = (int64_t)v1 != 0; A1_W8B(a1, 0, (uint8_t)(uint32_t)v0); if (c) goto L_1352be8; }
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B(a3, 0))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)SLL32(t0, (uint32_t)a2 & 0x1F)))); a3xw0 = true;
    (v0 = (uint64_t)(~(0ULL | RC_U64X(v0)))); a3xw0 = true;
    (v1 = (uint64_t)(RC_U64X(v1) & v0));
    A1_W32B(a3, 0, (uint32_t)v1);
    goto L_1352bf0;
L_1352be8:;
    if ((int32_t)v1 < 0) { A1_W8(a1, (uint8_t)0u); goto L_1352bf0; }
L_1352bf0:;
    { const bool c = (int64_t)a0 != 0; (a2 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)a2, 1)))); if (c) { A3_BACK(); goto L_1352ba8; } }
L_1352bf8:;
    { if (xo) { if (a3xw0) xo[0] = v0; if (a3xw1) xo[1] = a0; if (a3xw2) xo[2] = a1_sx64(a1); xo[3] = a2; if (a3xw4) xo[4] = a1_sx64(a3); if (a3xw5) xo[5] = a1_sx64(t0); if (a3xw6) xo[6] = a1_sx64(t1); } return; }
    __builtin_unreachable();
}

// gStatPlaceIdToSavePoint (0x1353440): typed by tools/typed_lift2.py (round A3; loop leaf, extra outputs xo[0]=a1 xo[1]=a2 xo[2]=a3)
inline uint32_t gStatPlaceIdToSavePoint(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint64_t a0, uint64_t *xo = nullptr)
{
    uint32_t v0 = 0;
    uint32_t v1 = 0;
    uint32_t a1 = 0;
    uint32_t a2 = 0;
    uint64_t a3 = 0;
    { const uint32_t _dynInsn = A1_R32(0x1353440u); const int32_t _dynSimm = (int16_t)(_dynInsn & 0xFFFFu); const uint32_t _dynImm = _dynInsn & 0xFFFFu; (void)_dynSimm; (void)_dynImm; (v0 = (uint32_t)((int32_t)((int32_t)((uint32_t)_dynImm << 16)))); }
    (a3 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL));
    { const uint32_t _dynInsn = A1_R32(0x1353448u); const int32_t _dynSimm = (int16_t)(_dynInsn & 0xFFFFu); const uint32_t _dynImm = _dynInsn & 0xFFFFu; (void)_dynSimm; (void)_dynImm; (v0 = (uint32_t)((int32_t)((int32_t)ADD32(v0, _dynSimm)))); }
    a2 = A1_R32B(v0, 12);
    { const bool c = (int32_t)a2 <= 0; a1 = A1_R32B(v0, 16); if (c) goto L_1353484; }
    v1 = (uint32_t)(int8_t)A1_R8B(a1, 1);
L_1353460:;
    v0 = a1;
    { const bool c = (int64_t)(int32_t)v1 == (int64_t)a0; a1 = a1 + 0x44u; if (c) goto L_1353488; }
    (a3 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)a3, 1))));
    (v0 = (uint32_t)(((int64_t)(int64_t)RC_U64X(a3) < (int64_t)(int64_t)a1_sx64(a2)) ? 1 : 0));
    if (v0 != 0) { v1 = (uint32_t)(int8_t)A1_R8B(a1, 1); { A3_BACK(); goto L_1353460; } }
L_1353484:;
    v0 = 0u;
L_1353488:;
    { if (xo) { xo[0] = a1_sx64(a1); xo[1] = a1_sx64(a2); xo[2] = a3; } return v0; }
    __builtin_unreachable();
}

// gstatBootMcSetLastSaveOmakeStat (0x1353e18): typed by tools/typed_lift2.py (round A3; loop leaf, extra outputs xo[0]=a2 xo[1]=t0)
inline uint32_t gstatBootMcSetLastSaveOmakeStat(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t a0, uint64_t *xo = nullptr)
{
    uint32_t v0 = 0;
    uint64_t v1 = 0;
    uint64_t a1 = 0;
    uint64_t a2 = 0;
    uint64_t a3 = 0;
    uint64_t t0 = 0;
    v0 = 0x11E0000u;
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(a0, 96))));
    (v0 = (uint32_t)((int32_t)((int32_t)ADD32(v0, 15920))));
L_1353e24:;
    (a1 = (uint64_t)(A1_R64B(a0, 0)));
    (a2 = (uint64_t)(A1_R64B(a0, 8)));
    (a3 = (uint64_t)(A1_R64B(a0, 16)));
    (t0 = (uint64_t)(A1_R64B(a0, 24)));
    A1_W64B(v0, 0, a1);
    A1_W64B(v0, 8, a2);
    A1_W64B(v0, 16, a3);
    A1_W64B(v0, 24, t0);
    a0 = a0 + 0x20u;
    { const bool c = (int64_t)(int32_t)a0 != (int64_t)v1; v0 = v0 + 0x20u; if (c) { A3_BACK(); goto L_1353e24; } }
    (v1 = (uint64_t)(A1_R64B(a0, 0)));
    (a1 = (uint64_t)(A1_R64B(a0, 8)));
    A1_W64B(v0, 0, v1);
    { A1_W64B(v0, 8, a1); if (xo) { xo[0] = a2; xo[1] = t0; } return v0; }
    __builtin_unreachable();
}

// opflowSetDemoFrame (0x13576d8): typed by tools/typed_lift2.py (round A3; leaf, extra outputs xo[0]=f12)
inline a1::RetVF opflowSetDemoFrame(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, float f12, uint64_t *xo = nullptr)
{
    float f0 = 0.0f;
    uint32_t at = 0;
    uint32_t v0 = 0;
    uint32_t fcr31 = 0;
    at = 0x41F00000u;
    f0 = a1_flt(at);
    v0 = 0x11E0000u;
    f12 = a1f::div(fcr31, f12, f0);
    { A1_W32B(v0, 16088, a1_bits(f12)); if (xo) { xo[0] = a1_bits(f12); } return a1::RetVF{(uint64_t)a1_sx64(v0), f0}; }
    __builtin_unreachable();
}

// getStageChipID (0x1358888): typed by tools/typed_lift2.py (round A3; leaf, extra outputs xo[0]=a3)
inline a1::RetVF getStageChipID(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t a0, uint32_t a1, uint32_t a2, uint64_t *xo = nullptr)
{
    float f0 = 0.0f;
    float f1 = 0.0f;
    float f2 = 0.0f;
    float f3 = 0.0f;
    uint32_t at = 0;
    uint32_t v0 = 0;
    uint64_t v1 = 0;
    uint32_t a3 = 0;
    uint32_t fcr31 = 0;
    f0 = a1_flt(A1_R32(a0));
    v0 = 0x1460000u;
    a3 = v0 + 0x5E80u;
    at = 0x3F000000u;
    { uint32_t bits = at; std::memcpy(&f3, &bits, sizeof(bits)); }
    f0 = (-(float)(f0));
    v0 = a3;
    f1 = a1_flt(A1_R32B(v0, 80));
    f2 = a1_flt(A1_R32B(v0, 64));
    f0 = a1f::sub(f0, f1);
    f1 = a1f::div(fcr31, f0, f2);
    { int32_t tmp = a1f::cvtw(f1); std::memcpy(&f0, &tmp, sizeof(tmp)); }
    v0 = a1_bits(f0);
    f0 = a1_flt(v0);
    { int32_t tmp; std::memcpy(&tmp, &f0, sizeof(tmp)); f0 = ((float)(int32_t)(tmp)); }
    f0 = a1f::sub(f1, f0);
    fcr31 = (a1f::lt(f0, f3)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
    { const bool c = (fcr31 & 0x800000u) != 0u; (v1 = (uint64_t)((uint64_t)a1_sx64(v0) + (uint64_t)0ULL)); if (c) goto L_13588f0; }
    f0 = a1_flt(0u);
    fcr31 = (a1f::lt(f1, f0)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
    { const bool c = (fcr31 & 0x800000u) != 0u; (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(v0, 4294967295)))); if (c) goto L_13588f0; }
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(v0, 1))));
L_13588f0:;
    A1_W32B(a1, 0, (uint32_t)v1);
    at = 0x3F000000u;
    { uint32_t bits = at; std::memcpy(&f3, &bits, sizeof(bits)); }
    f0 = a1_flt(A1_R32B(a0, 8));
    f2 = a1_flt(A1_R32B(a3, 84));
    f0 = (-(float)(f0));
    f1 = a1_flt(A1_R32B(a3, 68));
    f0 = a1f::sub(f0, f2);
    f1 = a1f::div(fcr31, f0, f1);
    { int32_t tmp = a1f::cvtw(f1); std::memcpy(&f0, &tmp, sizeof(tmp)); }
    v0 = a1_bits(f0);
    f0 = a1_flt(v0);
    { int32_t tmp; std::memcpy(&tmp, &f0, sizeof(tmp)); f0 = ((float)(int32_t)(tmp)); }
    f0 = a1f::sub(f1, f0);
    fcr31 = (a1f::lt(f0, f3)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
    { const bool c = (fcr31 & 0x800000u) != 0u; (v1 = (uint64_t)((uint64_t)a1_sx64(v0) + (uint64_t)0ULL)); if (c) goto L_1358950; }
    f0 = a1_flt(0u);
    fcr31 = (a1f::lt(f1, f0)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
    { const bool c = (fcr31 & 0x800000u) != 0u; (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(v0, 4294967295)))); if (c) goto L_1358950; }
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(v0, 1))));
L_1358950:;
    { A1_W32B(a2, 0, (uint32_t)v1); if (xo) { xo[0] = a1_sx64(a3); } return a1::RetVF{(uint64_t)a1_sx64(v0), f0}; }
    __builtin_unreachable();
}

// gamecore_sub_01358A68 (0x1358a68): typed by tools/typed_lift2.py (round A3; leaf, extra outputs xo[0]=a2 xo[1]=a3 xo[2]=t0)
inline uint32_t gamecore_sub_01358A68(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint64_t a0, uint64_t *xo = nullptr)
{
    uint32_t v0 = 0;
    uint32_t v1 = 0;
    uint32_t a1 = 0;
    uint32_t a2 = 0;
    uint64_t a3 = 0;
    uint32_t t0 = 0;
    uint64_t hi = 0;
    uint64_t lo = 0;
    (a3 = (uint64_t)((uint64_t)a0 + (uint64_t)0ULL));
    (v1 = (uint32_t)((int32_t)((int32_t)A1_R32B((uint32_t)a3, 0))));
    { const bool c = (int32_t)v1 < 0; (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)a3, 4)))); if (c) goto L_1358aa8; }
    v0 = 0x1460000u;
    a2 = v0 + 0x5E80u;
    a1 = A1_R32B(a2, 72);
    v0 = (int32_t)v1 < (int32_t)a1 ? 1u : 0u;
    if (v0 == 0) { a2 = 0xFFFFFFFFu; goto L_1358aac; }
    { const bool c = (int32_t)a0 < 0; { int64_t result = (int64_t)(int32_t)a0 * (int64_t)(int32_t)a1; lo = (uint64_t)(int64_t)(int32_t)result; hi = (uint64_t)(int64_t)(int32_t)(result >> 32); (t0 = (uint32_t)((int32_t)((int32_t)result))); } if (c) goto L_1358aa8; }
    v0 = A1_R32B(a2, 76);
    (v0 = (uint32_t)(((int64_t)(int64_t)RC_U64X(a0) < (int64_t)(int64_t)a1_sx64(v0)) ? 1 : 0));
    { const bool c = v0 != 0; a2 = t0 + v1; if (c) goto L_1358aac; }
L_1358aa8:;
    a2 = 0xFFFFFFFFu;
L_1358aac:;
    v0 = 0xFFFFFFFFu;
    { const bool c = a2 == v0; (t0 = (uint32_t)((int32_t)((int32_t)A1_R32B((uint32_t)a3, 8)))); if (c) goto L_1358ae8; }
    v1 = 0x1460000u;
    v0 = a2 << 1;
    a1 = A1_R32B(v1, 24200);
    v0 = v0 + a2;
    v0 = v0 << 3;
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)SLL32(t0, 2))));
    v0 = v0 + a1;
    (v0 = (uint32_t)((int32_t)((int32_t)ADD32(v0, (uint32_t)a0))));
    v1 = A1_R32B(v0, 8);
    v1 = v1 & 0xFu;
    goto L_1358aec;
L_1358ae8:;
    v1 = 7u;
L_1358aec:;
    v0 = 3u;
    { const bool c = v1 == v0; v0 = 0xFFFFFFFFu; if (c) goto L_1358b38; }
    { const bool c = a2 == v0; v1 = 0x1460000u; if (c) goto L_1358b28; }
    v0 = a2 << 1;
    a1 = A1_R32B(v1, 24200);
    v0 = v0 + a2;
    v0 = v0 << 3;
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)SLL32(t0, 2))));
    v0 = v0 + a1;
    (v0 = (uint32_t)((int32_t)((int32_t)ADD32(v0, (uint32_t)a0))));
    v1 = A1_R32B(v0, 8);
    v1 = v1 & 0xFu;
    goto L_1358b2c;
L_1358b28:;
    v1 = 7u;
L_1358b2c:;
    v0 = 6u;
    if (v1 != v0) goto L_1358b80;
L_1358b38:;
    v0 = 0x1460000u;
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)a3, 20))));
    a1 = v0 + 0x5F14u;
    v0 = A1_R32(a1);
    { const bool c = (int64_t)a3 != (int64_t)(int32_t)v0; (v1 = (uint32_t)((int32_t)((int32_t)A1_R32B((uint32_t)a3, 24)))); if (c) goto L_1358b54; }
    A1_W32(a1, v1);
L_1358b54:;
    if ((int64_t)a0 != 0) { A1_W32B((uint32_t)a0, 24, v1); goto L_1358b5c; }
L_1358b5c:;
    if (v1 != 0) { A1_W32B(v1, 20, (uint32_t)a0); goto L_1358b64; }
L_1358b64:;
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)326 << 16))));
    v0 = 1u;
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)a0, 24336))));
    (v1 = (uint32_t)((int32_t)((int32_t)A1_R32B((uint32_t)a0, 0))));
    v1 = v1 - 1u;
    { A1_W32B((uint32_t)a0, 0, v1); if (xo) { xo[0] = a1_sx64(a2); xo[1] = a3; xo[2] = a1_sx64(t0); } return v0; }
L_1358b80:;
    { v0 = 0u; if (xo) { xo[0] = a1_sx64(a2); xo[1] = a3; xo[2] = a1_sx64(t0); } return v0; }
    __builtin_unreachable();
}

// stagemanager_checkstageAlreadyLoadRequestedByStrptr (0x1365ab0): typed by tools/typed_lift2.py (round A3; leaf, extra outputs xo[0]=a1 xo[1]=a2 xo[2]=a3)
inline uint32_t stagemanager_checkstageAlreadyLoadRequestedByStrptr(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint64_t a0, uint64_t *xo = nullptr)
{
    uint32_t v0 = 0;
    uint32_t v1 = 0;
    uint32_t a1 = 0;
    uint32_t a2 = 0;
    uint32_t a3 = 0;
    uint64_t hi = 0;
    uint64_t lo = 0;
    bool a3xw0 = false;
    v0 = 0x30000u;
    (v1 = (uint32_t)((int32_t)((int32_t)A1_R32B((uint32_t)a0, 0))));
    v0 = v0 | 0xFFFFu;
    v0 = v1 & v0;
    { const bool c = v0 == 0; (v0 = (uint32_t)((int32_t)((int32_t)SUB32((uint32_t)a0, v0)))); if (c) goto L_1365ad0; }
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B(v0, 0))));
    goto L_1365ad4;
L_1365ad0:;
    (a0 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL));
L_1365ad4:;
    { const bool c = (int64_t)a0 == 0; { const uint32_t _dynInsn = A1_R32(0x1365AD8u); const int32_t _dynSimm = (int16_t)(_dynInsn & 0xFFFFu); const uint32_t _dynImm = _dynInsn & 0xFFFFu; (void)_dynSimm; (void)_dynImm; (a3 = (uint32_t)((int32_t)((int32_t)((uint32_t)_dynImm << 16)))); } if (c) goto L_1365b08; }
    { const uint32_t _dynInsn = A1_R32(0x1365ADCu); const int32_t _dynSimm = (int16_t)(_dynInsn & 0xFFFFu); const uint32_t _dynImm = _dynInsn & 0xFFFFu; (void)_dynSimm; (void)_dynImm; (v0 = (uint32_t)((int32_t)((int32_t)A1_R32(ADD32(a3, _dynSimm))))); }
    { const bool c = (int64_t)a0 == (int64_t)(int32_t)v0; v0 = v1 >> 18; if (c) goto L_1365b08; }
    (a1 = (uint32_t)((int32_t)((int32_t)A1_R32B((uint32_t)a0, 8)))); a3xw0 = true;
    v0 = v0 & 0xFFFu;
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)a0, 16))));
    v1 = A1_R32B(a1, 12);
    { int64_t result = (int64_t)(int32_t)v0 * (int64_t)(int32_t)v1; lo = (uint64_t)(int64_t)(int32_t)result; hi = (uint64_t)(int64_t)(int32_t)(result >> 32); (v0 = (uint32_t)((int32_t)((int32_t)result))); }
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)a0, v0))));
    goto L_1365b0c;
L_1365b08:;
    (a0 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL));
L_1365b0c:;
    { const bool c = (int64_t)a0 == 0; { const uint32_t _dynInsn = A1_R32(0x1365B10u); const int32_t _dynSimm = (int16_t)(_dynInsn & 0xFFFFu); const uint32_t _dynImm = _dynInsn & 0xFFFFu; (void)_dynSimm; (void)_dynImm; (v0 = (uint32_t)((int32_t)((int32_t)A1_R32(ADD32(a3, _dynSimm))))); } if (c) goto L_1365b50; }
    { const uint32_t _dynInsn = A1_R32(0x1365B14u); const int32_t _dynSimm = (int16_t)(_dynInsn & 0xFFFFu); const uint32_t _dynImm = _dynInsn & 0xFFFFu; (void)_dynSimm; (void)_dynImm; (a2 = (uint32_t)((int32_t)((int32_t)((uint32_t)_dynImm << 16)))); }
    { const uint32_t _dynInsn = A1_R32(0x1365B18u); const int32_t _dynSimm = (int16_t)(_dynInsn & 0xFFFFu); const uint32_t _dynImm = _dynInsn & 0xFFFFu; (void)_dynSimm; (void)_dynImm; (v1 = (uint32_t)((int32_t)((int32_t)ADD32(a2, _dynSimm)))); }
    v0 = v0 ^ v1;
    if (a1_sx64(v0) == 0) v1 = (uint32_t)((uint64_t)0);;
    v0 = v1;
    { const bool c = v0 == 0; a1 = 0u; a3xw0 = true; if (c) goto L_1365b34; }
    a1 = A1_R32B(v0, 16); a3xw0 = true;
L_1365b34:;
    (v1 = (uint32_t)((int32_t)((int32_t)SUB32((uint32_t)a0, a1))));
    v0 = 0xCCCC0000u;
    v1 = (uint32_t)((int32_t)v1 >> 2);
    v0 = v0 | 0xCCCDu;
    { int64_t result = (int64_t)(int32_t)v1 * (int64_t)(int32_t)v0; lo = (uint64_t)(int64_t)(int32_t)result; hi = (uint64_t)(int64_t)(int32_t)(result >> 32); (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)result))); }
    goto L_1365b58;
L_1365b50:;
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 4294967295))));
    { const uint32_t _dynInsn = A1_R32(0x1365B54u); const int32_t _dynSimm = (int16_t)(_dynInsn & 0xFFFFu); const uint32_t _dynImm = _dynInsn & 0xFFFFu; (void)_dynSimm; (void)_dynImm; (a2 = (uint32_t)((int32_t)((int32_t)((uint32_t)_dynImm << 16)))); }
L_1365b58:;
    { const uint32_t _dynInsn = A1_R32(0x1365B58u); const int32_t _dynSimm = (int16_t)(_dynInsn & 0xFFFFu); const uint32_t _dynImm = _dynInsn & 0xFFFFu; (void)_dynSimm; (void)_dynImm; (v0 = (uint32_t)((int32_t)((int32_t)A1_R32(ADD32(a3, _dynSimm))))); }
    { const uint32_t _dynInsn = A1_R32(0x1365B5Cu); const int32_t _dynSimm = (int16_t)(_dynInsn & 0xFFFFu); const uint32_t _dynImm = _dynInsn & 0xFFFFu; (void)_dynSimm; (void)_dynImm; (v1 = (uint32_t)((int32_t)((int32_t)ADD32(a2, _dynSimm)))); }
    v0 = v0 ^ v1;
    if (a1_sx64(v0) == 0) v1 = (uint32_t)((uint64_t)0);;
    if (v1 == 0) { v0 = 0u; goto L_1365b84; }
    (v0 = (uint32_t)((int32_t)((int32_t)SLL32((uint32_t)a0, 2))));
    v1 = A1_R32B(v1, 16);
    (v0 = (uint32_t)((int32_t)((int32_t)ADD32(v0, (uint32_t)a0))));
    v0 = v0 << 2;
    v0 = v1 + v0;
L_1365b84:;
    { v0 = A1_R16B(v0, 16); if (xo) { if (a3xw0) xo[0] = a1_sx64(a1); xo[1] = a1_sx64(a2); xo[2] = a1_sx64(a3); } return v0; }
    __builtin_unreachable();
}

// gcGetStageData (0x1365d18): typed by tools/typed_lift2.py (round A3; leaf, extra outputs xo[0]=a1)
inline uint32_t gcGetStageData(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t a0, uint64_t *xo = nullptr)
{
    uint32_t v0 = 0;
    uint32_t v1 = 0;
    uint32_t a1 = 0;
    { const uint32_t _dynInsn = A1_R32(0x1365D18u); const int32_t _dynSimm = (int16_t)(_dynInsn & 0xFFFFu); const uint32_t _dynImm = _dynInsn & 0xFFFFu; (void)_dynSimm; (void)_dynImm; (v0 = (uint32_t)((int32_t)((int32_t)((uint32_t)_dynImm << 16)))); }
    { const uint32_t _dynInsn = A1_R32(0x1365D1Cu); const int32_t _dynSimm = (int16_t)(_dynInsn & 0xFFFFu); const uint32_t _dynImm = _dynInsn & 0xFFFFu; (void)_dynSimm; (void)_dynImm; (v1 = (uint32_t)((int32_t)((int32_t)((uint32_t)_dynImm << 16)))); }
    { const uint32_t _dynInsn = A1_R32(0x1365D20u); const int32_t _dynSimm = (int16_t)(_dynInsn & 0xFFFFu); const uint32_t _dynImm = _dynInsn & 0xFFFFu; (void)_dynSimm; (void)_dynImm; (a1 = (uint32_t)((int32_t)((int32_t)A1_R32(ADD32(v0, _dynSimm))))); }
    v0 = a0 << 2;
    { const uint32_t _dynInsn = A1_R32(0x1365D28u); const int32_t _dynSimm = (int16_t)(_dynInsn & 0xFFFFu); const uint32_t _dynImm = _dynInsn & 0xFFFFu; (void)_dynSimm; (void)_dynImm; (v1 = (uint32_t)((int32_t)((int32_t)ADD32(v1, _dynSimm)))); }
    v0 = v0 + a0;
    a1 = a1 ^ v1;
    if (a1_sx64(a1) == 0) v1 = (uint32_t)((uint64_t)0);;
    { const bool c = v1 == 0; a0 = v0 << 2; if (c) goto L_1365d50; }
    v0 = A1_R32B(v1, 16);
    { v0 = v0 + a0; if (xo) { xo[0] = a1_sx64(a1); } return v0; }
L_1365d50:;
    { v0 = 0u; if (xo) { xo[0] = a1_sx64(a1); } return v0; }
    __builtin_unreachable();
}

// stagemanager_getStageNumberWithStageDataStrptr (0x13663d0): typed by tools/typed_lift2.py (round A3; leaf, extra outputs xo[0]=a0 xo[1]=a1 xo[2]=a2)
inline uint32_t stagemanager_getStageNumberWithStageDataStrptr(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint64_t a0, uint64_t *xo = nullptr)
{
    uint32_t v0 = 0;
    uint64_t v1 = 0;
    uint64_t a1 = 0;
    uint32_t a2 = 0;
    uint64_t hi = 0;
    uint64_t lo = 0;
    v0 = 0x30000u;
    (a2 = (uint32_t)((int32_t)((int32_t)A1_R32B((uint32_t)a0, 0))));
    v0 = v0 | 0xFFFFu;
    v0 = a2 & v0;
    { const bool c = v0 == 0; (v0 = (uint32_t)((int32_t)((int32_t)SUB32((uint32_t)a0, v0)))); if (c) goto L_13663f0; }
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B(v0, 0))));
    goto L_13663f4;
L_13663f0:;
    (a0 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL));
L_13663f4:;
    { const bool c = (int64_t)a0 == 0; { const uint32_t _dynInsn = A1_R32(0x13663F8u); const int32_t _dynSimm = (int16_t)(_dynInsn & 0xFFFFu); const uint32_t _dynImm = _dynInsn & 0xFFFFu; (void)_dynSimm; (void)_dynImm; (v0 = (uint32_t)((int32_t)((int32_t)((uint32_t)_dynImm << 16)))); } if (c) goto L_1366428; }
    { const uint32_t _dynInsn = A1_R32(0x13663FCu); const int32_t _dynSimm = (int16_t)(_dynInsn & 0xFFFFu); const uint32_t _dynImm = _dynInsn & 0xFFFFu; (void)_dynSimm; (void)_dynImm; (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32(ADD32(v0, _dynSimm))))); }
    { const bool c = (int64_t)a0 == (int64_t)v1; v0 = a2 >> 18; if (c) goto L_1366428; }
    (a1 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)a0, 8))));
    v0 = v0 & 0xFFFu;
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)a0, 16))));
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)a1, 12))));
    { int64_t result = (int64_t)(int32_t)v0 * (int64_t)(int32_t)v1; lo = (uint64_t)(int64_t)(int32_t)result; hi = (uint64_t)(int64_t)(int32_t)(result >> 32); (v0 = (uint32_t)((int32_t)((int32_t)result))); }
    (a1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)a0, v0))));
    goto L_136642c;
L_1366428:;
    (a1 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL));
L_136642c:;
    { const bool c = (int64_t)a1 == 0; { const uint32_t _dynInsn = A1_R32(0x1366430u); const int32_t _dynSimm = (int16_t)(_dynInsn & 0xFFFFu); const uint32_t _dynImm = _dynInsn & 0xFFFFu; (void)_dynSimm; (void)_dynImm; (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)_dynImm << 16)))); } if (c) goto L_1366470; }
    { const uint32_t _dynInsn = A1_R32(0x1366434u); const int32_t _dynSimm = (int16_t)(_dynInsn & 0xFFFFu); const uint32_t _dynImm = _dynInsn & 0xFFFFu; (void)_dynSimm; (void)_dynImm; (v0 = (uint32_t)((int32_t)((int32_t)((uint32_t)_dynImm << 16)))); }
    { const uint32_t _dynInsn = A1_R32(0x1366438u); const int32_t _dynSimm = (int16_t)(_dynInsn & 0xFFFFu); const uint32_t _dynImm = _dynInsn & 0xFFFFu; (void)_dynSimm; (void)_dynImm; (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32(ADD32(v0, _dynSimm))))); }
    { const uint32_t _dynInsn = A1_R32(0x136643Cu); const int32_t _dynSimm = (int16_t)(_dynInsn & 0xFFFFu); const uint32_t _dynImm = _dynInsn & 0xFFFFu; (void)_dynSimm; (void)_dynImm; (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v1, _dynSimm)))); }
    (a0 = (uint64_t)(RC_U64X(a0) ^ RC_U64X(v1)));
    if (RC_U64X(a0) == 0) v1 = (uint64_t)0;;
    { const bool c = (int64_t)v1 == 0; v0 = 0u; if (c) goto L_1366454; }
    (v0 = (uint32_t)((int32_t)((int32_t)A1_R32B((uint32_t)v1, 16))));
L_1366454:;
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)SUB32((uint32_t)a1, v0))));
    v0 = 0xCCCC0000u;
    (v1 = (uint64_t)((int64_t)(int32_t)(SRA32((int32_t)v1, 2))));
    v0 = v0 | 0xCCCDu;
    { { int64_t result = (int64_t)(int32_t)v1 * (int64_t)(int32_t)v0; lo = (uint64_t)(int64_t)(int32_t)result; hi = (uint64_t)(int64_t)(int32_t)(result >> 32); (v0 = (uint32_t)((int32_t)((int32_t)result))); } if (xo) { xo[0] = a0; xo[1] = a1; xo[2] = a1_sx64(a2); } return v0; }
L_1366470:;
    { v0 = 0xFFFFFFFFu; if (xo) { xo[0] = a0; xo[1] = a1; xo[2] = a1_sx64(a2); } return v0; }
    __builtin_unreachable();
}

// PauseStageBoundaryManager (0x13678e8): typed by tools/typed_lift2.py (round A3; leaf, extra outputs xo[0]=a0 xo[1]=a1 xo[2]=a2 xo[3]=a3 xo[4]=t0 xo[5]=t1 xo[6]=t2 xo[7]=t3)
inline uint32_t PauseStageBoundaryManager(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint64_t a0, uint64_t *xo = nullptr)
{
    uint32_t v0 = 0;
    uint32_t v1 = 0;
    uint64_t a1 = 0;
    uint64_t a2 = 0;
    uint64_t a3 = 0;
    uint64_t t0 = 0;
    uint64_t t1 = 0;
    uint64_t t2 = 0;
    uint64_t t3 = 0;
    uint64_t t4 = 0;
    bool a3xw0 = false;
    bool a3xw1 = false;
    bool a3xw2 = false;
    bool a3xw3 = false;
    bool a3xw4 = false;
    bool a3xw5 = false;
    bool a3xw6 = false;
    bool a3xw7 = false;
    v0 = 0x1460000u;
    { const bool c = (int64_t)a0 != 0; A1_W32B(v0, 25552, (uint32_t)a0); if (c) goto L_136796c; }
    v0 = 0x1470000u;
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)326 << 16)))); a3xw0 = true;
    v0 = v0 - 0x7DA0u;
    v1 = 0x1460000u;
    (a1 = (uint64_t)(A1_R64B(v0, 56))); a3xw1 = true;
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)a0, 25568)))); a3xw0 = true;
    (t4 = (uint64_t)(A1_R64B(v0, 0)));
    (v1 = (uint32_t)((int32_t)((int32_t)ADD32(v1, 25632))));
    (a2 = (uint64_t)(A1_R64B(v0, 8))); a3xw2 = true;
    (a3 = (uint64_t)(A1_R64B(v0, 16))); a3xw3 = true;
    (t0 = (uint64_t)(A1_R64B(v0, 24))); a3xw4 = true;
    (t1 = (uint64_t)(A1_R64B(v0, 32))); a3xw5 = true;
    (t2 = (uint64_t)(A1_R64B(v0, 40))); a3xw6 = true;
    (t3 = (uint64_t)(A1_R64B(v0, 48))); a3xw7 = true;
    A1_W64B(v1, 56, a1);
    A1_W64B((uint32_t)a0, 56, a1);
    A1_W64B(v1, 0, t4);
    A1_W64B(v1, 8, a2);
    A1_W64B(v1, 16, a3);
    A1_W64B(v1, 24, t0);
    A1_W64B(v1, 32, t1);
    A1_W64B(v1, 40, t2);
    A1_W64B(v1, 48, t3);
    A1_W64B((uint32_t)a0, 0, t4);
    A1_W64B((uint32_t)a0, 8, a2);
    A1_W64B((uint32_t)a0, 16, a3);
    A1_W64B((uint32_t)a0, 24, t0);
    A1_W64B((uint32_t)a0, 32, t1);
    A1_W64B((uint32_t)a0, 40, t2);
    A1_W64B((uint32_t)a0, 48, t3);
L_136796c:;
    { if (xo) { if (a3xw0) xo[0] = a0; if (a3xw1) xo[1] = a1; if (a3xw2) xo[2] = a2; if (a3xw3) xo[3] = a3; if (a3xw4) xo[4] = t0; if (a3xw5) xo[5] = t1; if (a3xw6) xo[6] = t2; if (a3xw7) xo[7] = t3; } return v0; }
    __builtin_unreachable();
}

// initBgmScript (0x136a2e0): typed by tools/typed_lift2.py (round A3; loop leaf, extra outputs xo[0]=a0 xo[1]=a1 xo[2]=a2)
inline uint64_t initBgmScript(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint64_t *xo = nullptr)
{
    uint64_t v0 = 0;
    uint64_t v1 = 0;
    uint64_t a0 = 0;
    uint64_t a1 = 0;
    uint64_t a2 = 0;
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)286 << 16))));
    (a0 = (uint64_t)(0ULL | (uint64_t)(uint16_t)49152));
    (a0 = (uint64_t)(RC_U64X(a0) << (32 + 13)));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v0, 16552))));
    (a1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 1))));
    A1_W32B((uint32_t)v0, 0, 0u);
    (a2 = (uint64_t)((uint64_t)RC_U64X(v0) + (uint64_t)0ULL));
    (v1 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL));
L_136a300:;
    (a0 = (uint64_t)((int64_t)a0 >> 1));
    (v0 = (uint64_t)(a0 & a1));
    { const bool c = (int64_t)v0 == 0; (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v1, 1)))); if (c) { A3_BACK(); goto L_136a300; } }
    { A1_W32B((uint32_t)a2, 0, (uint32_t)v1); if (xo) { xo[0] = a0; xo[1] = a1; xo[2] = a2; } return v0; }
    __builtin_unreachable();
}

// gamecore_sub_0136A328 (0x136a328): typed by tools/typed_lift2.py (round A3; loop leaf)
inline uint32_t gamecore_sub_0136A328(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint64_t a0)
{
    uint32_t v0 = 0;
    uint32_t v1 = 0;
    uint32_t a1 = 0;
    v0 = 0x11E0000u;
    a1 = 1u;
    v1 = v0 + 0x41B0u;
L_136a338:;
    v0 = A1_R32(v1);
    if ((int64_t)(int32_t)v0 == (int64_t)a0) { A1_W32(v1, 0u); goto L_136a344; }
L_136a344:;
    a1 = a1 - 1u;
    { const bool c = (int32_t)a1 >= 0; v1 = v1 + 4u; if (c) { A3_BACK(); goto L_136a338; } }
    v0 = 0x1460000u;
    v0 = v0 + 0x6474u;
    v1 = A1_R32(v0);
    if ((int64_t)(int32_t)v1 == (int64_t)a0) { A1_W32(v0, 0u); goto L_136a370; }
L_136a370:;
    { return v0; }
    __builtin_unreachable();
}

// GetBossPartsDamagePerParts (0x1379270): typed by tools/typed_lift2.py (round A3; leaf, extra outputs xo[0]=a2 xo[1]=a3)
inline a1::RetVF GetBossPartsDamagePerParts(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t a0, uint32_t a1, uint64_t *xo = nullptr)
{
    float f0 = 0.0f;
    float f1 = 0.0f;
    uint32_t v0 = 0;
    uint32_t v1 = 0;
    uint32_t a2 = 0;
    uint32_t a3 = 0;
    uint32_t fcr31 = 0;
    v0 = a1 << 1;
    a2 = A1_R32B(a0, 232);
    v0 = v0 + a1;
    a3 = A1_R32B(a0, 16);
    v0 = v0 << 2;
    v1 = a1 << 3;
    v0 = v0 - a1;
    v1 = v1 + a2;
    v0 = v0 << 3;
    f0 = a1_flt(A1_R32B(v1, 36));
    v0 = v0 + a3;
    f1 = a1_flt(A1_R32B(v0, 28));
    f0 = a1f::sub(f1, f0);
    f0 = a1f::div(fcr31, f0, f1);
    { if (xo) { xo[0] = a1_sx64(a2); xo[1] = a1_sx64(a3); } return a1::RetVF{(uint64_t)a1_sx64(v0), f0}; }
    __builtin_unreachable();
}

// getexpressiondatapoint (0x137def8): typed by tools/typed_lift2.py (round A3; leaf, extra outputs xo[0]=a1)
inline float getexpressiondatapoint(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t a0, uint32_t a1, uint64_t *xo = nullptr)
{
    float f0 = 0.0f;
    a1 = a1 << 4;
    a1 = a1 + a0;
    { f0 = a1_flt(A1_R32B(a1, 4)); if (xo) { xo[0] = a1_sx64(a1); } return f0; }
    __builtin_unreachable();
}

// controllerAlertDisable (0x137e828): typed by tools/typed_lift2.py (round A3; leaf, extra outputs xo[0]=a1 xo[1]=a2)
inline uint32_t controllerAlertDisable(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint64_t a0, uint64_t *xo = nullptr)
{
    uint32_t v0 = 0;
    uint32_t v1 = 0;
    uint64_t a1 = 0;
    uint32_t a2 = 0;
    v0 = 0x1460000u;
    v1 = 1u;
    a2 = v0 + 0x64E8u;
    { const bool c = (int64_t)a0 != (int64_t)(int32_t)v1; (a1 = (uint64_t)((uint64_t)a1_sx64(a2) + (uint64_t)0ULL)); if (c) goto L_137e848; }
    (v0 = (uint32_t)((int32_t)((int32_t)A1_R32B((uint32_t)a1, 0))));
    v0 = v0 + 1u;
    goto L_137e850;
L_137e848:;
    (v0 = (uint32_t)((int32_t)((int32_t)A1_R32B((uint32_t)a1, 0))));
    v0 = v0 - 1u;
L_137e850:;
    A1_W32B((uint32_t)a1, 0, v0);
    v0 = A1_R32(a2);
    if ((int32_t)v0 < 0) { A1_W32(a2, 0u); goto L_137e860; }
L_137e860:;
    { if (xo) { xo[0] = a1; xo[1] = a1_sx64(a2); } return v0; }
    __builtin_unreachable();
}

// gamecore_sub_01386B98 (0x1386b98): typed by tools/typed_lift2.py (round A3; loop leaf, extra outputs xo[0]=f0)
inline uint32_t gamecore_sub_01386B98(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint64_t *xo = nullptr)
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
    { if (xo) { if (a3xw0) xo[0] = a1_bits(f0); } return v0; }
    __builtin_unreachable();
}

// fadeout_weakpoint_effect (0x13883b0): typed by tools/typed_lift2.py (round A3; loop leaf)
inline uint32_t fadeout_weakpoint_effect(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt)
{
    uint32_t v0 = 0;
    uint32_t v1 = 0;
    uint32_t a0 = 0;
    v0 = 0x1460000u;
    v1 = A1_R32B(v0, 26352);
    if (v1 == 0) goto L_13883ec;
    v0 = 0x11E0000u;
    a0 = 2u;
    v0 = v0 + 0x6290u;
    v1 = 7u;
L_13883d0:;
    v1 = v1 - 1u;
    A1_W32B(v0, 16, 0u);
    A1_W32B(v0, 4, a0);
    { const bool c = (int32_t)v1 >= 0; v0 = v0 + 0x78u; if (c) { A3_BACK(); goto L_13883d0; } }
L_13883ec:;
    { return v0; }
    __builtin_unreachable();
}

// cancel_fadeout_weakpoint_effect (0x13883f8): typed by tools/typed_lift2.py (round A3; loop leaf)
inline uint32_t cancel_fadeout_weakpoint_effect(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt)
{
    uint32_t v0 = 0;
    uint32_t v1 = 0;
    uint32_t a0 = 0;
    v0 = 0x1460000u;
    v1 = A1_R32B(v0, 26352);
    { const bool c = v1 == 0; v0 = 0x11E0000u; if (c) goto L_1388434; }
    a0 = 7u;
    v1 = v0 + 0x6290u;
L_1388410:;
    v0 = A1_R32B(v1, 4);
    v0 = v0 - 2u;
    v0 = v0 < 2u ? 1u : 0u;
    if (v0 != 0) { A1_W32B(v1, 4, 0u); goto L_1388424; }
L_1388424:;
    a0 = a0 - 1u;
    { const bool c = (int32_t)a0 >= 0; v1 = v1 + 0x78u; if (c) { A3_BACK(); goto L_1388410; } }
L_1388434:;
    { return v0; }
    __builtin_unreachable();
}

// set_node_data_expression (0x138be30): typed by tools/typed_lift2.py (round A3; loop leaf, extra outputs xo[0]=v0 xo[1]=a0 xo[2]=a3)
inline void set_node_data_expression(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t a0, uint64_t a1, uint32_t a2, float f12, float f13, float f14, uint64_t *xo = nullptr)
{
    uint32_t v0 = 0;
    uint32_t v1 = 0;
    uint64_t a3 = 0;
    bool a3xw0 = false;
    bool a3xw1 = false;
    bool a3xw2 = false;
    if ((int64_t)a1 == 0) goto L_138be74;
    v1 = A1_R32B(a0, 156);
    { const bool c = (int32_t)v1 <= 0; (a3 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL)); a3xw2 = true; if (c) goto L_138be74; }
    a0 = a0 + 0xA0u; a3xw1 = true;
L_138be48:;
    v0 = A1_R32(a0); a3xw0 = true;
    if ((int64_t)(int32_t)v0 != (int64_t)a1) { (a3 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)a3, 1)))); a3xw2 = true; goto L_138be68; }
    A1_W32B(a0, 52, a2);
    A1_W32B(a0, 4, a1_bits(f12));
    A1_W32B(a0, 8, a1_bits(f13));
    { A1_W32B(a0, 12, a1_bits(f14)); if (xo) { xo[0] = a1_sx64(v0); xo[1] = a1_sx64(a0); xo[2] = a3; } return; }
L_138be68:;
    (v0 = (uint32_t)(((int64_t)(int64_t)a3 < (int64_t)(int64_t)a1_sx64(v1)) ? 1 : 0)); a3xw0 = true;
    { const bool c = v0 != 0; a0 = a0 + 0x38u; a3xw1 = true; if (c) { A3_BACK(); goto L_138be48; } }
L_138be74:;
    { if (xo) { if (a3xw0) xo[0] = a1_sx64(v0); if (a3xw1) xo[1] = a1_sx64(a0); if (a3xw2) xo[2] = a3; } return; }
    __builtin_unreachable();
}

// constructor_expression_change (0x138bec8): typed by tools/typed_lift2.py (round A3; loop leaf)
inline uint32_t constructor_expression_change(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t a0)
{
    uint32_t v0 = 0;
    A1_W32(a0, 0u);
    a0 = a0 + 4u;
    v0 = 4u;
L_138bed8:;
    v0 = v0 - 1u;
    A1_W32(a0, 0u);
    A1_W32B(a0, 4, 0u);
    A1_W32B(a0, 16, 0u);
    A1_W32B(a0, 8, 0u);
    A1_W32B(a0, 20, 0u);
    A1_W32B(a0, 12, 0u);
    { const bool c = (int32_t)v0 >= 0; a0 = a0 + 0x18u; if (c) { A3_BACK(); goto L_138bed8; } }
    { return v0; }
    __builtin_unreachable();
}

// EnableArrowWater (0x138eab0): typed by tools/typed_lift2.py (round A3; loop leaf, extra outputs xo[0]=a1 xo[1]=a2)
inline uint32_t EnableArrowWater(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t a0, uint64_t *xo = nullptr)
{
    uint32_t v0 = 0;
    uint32_t v1 = 0;
    uint64_t a1 = 0;
    uint32_t a2 = 0;
    v0 = 0x11E0000u;
    (a1 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL));
    v1 = v0 + 0x6650u;
    a2 = 1u;
L_138eac0:;
    v0 = A1_R32(v1);
    if (v0 != 0) { (a1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)a1, 1)))); goto L_138eae0; }
    A1_W32B(v1, 16, a2);
    A1_W32B(v1, 8, a0);
    A1_W32(v1, a2);
    { A1_W32B(v1, 12, 0u); if (xo) { xo[0] = a1; xo[1] = a1_sx64(a2); } return v0; }
L_138eae0:;
    (v0 = (uint32_t)(((int64_t)(int64_t)a1 < (int64_t)(int32_t)25) ? 1 : 0));
    { const bool c = v0 != 0; v1 = v1 + 0x14u; if (c) { A3_BACK(); goto L_138eac0; } }
    { if (xo) { xo[0] = a1; xo[1] = a1_sx64(a2); } return v0; }
    __builtin_unreachable();
}

// EndArrowWater (0x138eaf8): typed by tools/typed_lift2.py (round A3; loop leaf, extra outputs xo[0]=a1 xo[1]=a2)
inline uint32_t EndArrowWater(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint64_t a0, uint64_t *xo = nullptr)
{
    uint32_t v0 = 0;
    uint32_t v1 = 0;
    uint64_t a1 = 0;
    uint32_t a2 = 0;
    v0 = 0x11E0000u;
    (a1 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL));
    v0 = v0 + 0x6650u;
    a2 = 1u;
    v1 = v0 + 8u;
L_138eb10:;
    v0 = A1_R32(v1);
    if ((int64_t)(int32_t)v0 != (int64_t)a0) { (a1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)a1, 1)))); goto L_138eb30; }
    A1_W32B(v1, 8, a2);
    A1_W32(v1 - 8u, 0u);
    A1_W32(v1, 0u);
    { A1_W32B(v1, 4, 0u); if (xo) { xo[0] = a1; xo[1] = a1_sx64(a2); } return v0; }
L_138eb30:;
    (v0 = (uint32_t)(((int64_t)(int64_t)a1 < (int64_t)(int32_t)25) ? 1 : 0));
    { const bool c = v0 != 0; v1 = v1 + 0x14u; if (c) { A3_BACK(); goto L_138eb10; } }
    { if (xo) { xo[0] = a1; xo[1] = a1_sx64(a2); } return v0; }
    __builtin_unreachable();
}

// gamecore_sub_01393E58 (0x1393e58): typed by tools/typed_lift2.py (round A3; leaf, extra outputs xo[0]=a1 xo[1]=a2 xo[2]=a3 xo[3]=t0 xo[4]=t1 xo[5]=t2)
inline a1::RetVF gamecore_sub_01393E58(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint64_t a0, uint64_t a1, uint64_t a2, uint64_t a3, float f12, uint64_t *xo = nullptr)
{
    float f0 = 0.0f;
    uint64_t v0 = 0;
    uint64_t v1 = 0;
    uint64_t t0 = 0;
    uint64_t t1 = 0;
    uint64_t t2 = 0;
    uint64_t hi = 0;
    uint64_t lo = 0;
    uint32_t fcr31 = 0;
    bool a3xw0 = false;
    bool a3xw1 = false;
    bool a3xw2 = false;
    bool a3xw5 = false;
    { uint32_t bits = A1_R32B((uint32_t)a0, 28); float f; std::memcpy(&f, &bits, sizeof(f)); f0 = f; }
    (t0 = (uint64_t)((uint64_t)a1 + (uint64_t)0ULL));
    (t1 = (uint64_t)((uint64_t)a3 + (uint64_t)0ULL));
    fcr31 = (a1f::lt(f12, f0)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
    { const bool c = (fcr31 & 0x800000u) == 0u; (v0 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL)); if (c) goto L_1394144; }
    { const bool c = (int64_t)a2 == 0; (a2 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)a2, 4)))); a3xw1 = true; if (c) goto L_1394120; }
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)3 << 16))));
    (a3 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)a2, 0)))); a3xw2 = true;
    (v0 = (uint64_t)(RC_U64X(v0) | (uint64_t)(uint16_t)65535));
    (v0 = (uint64_t)(RC_U64X(a3) & RC_U64X(v0)));
    { const bool c = (int64_t)v0 == 0; (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)SUB32((uint32_t)a2, (uint32_t)v0)))); if (c) goto L_1393e98; }
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)v0, 0))));
    goto L_1393e9c;
L_1393e98:;
    (a0 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL));
L_1393e9c:;
    { const bool c = (int64_t)a0 == 0; { const uint32_t _dynInsn = A1_R32(0x1393EA0u); const int32_t _dynSimm = (int16_t)(_dynInsn & 0xFFFFu); const uint32_t _dynImm = _dynInsn & 0xFFFFu; (void)_dynSimm; (void)_dynImm; (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)_dynImm << 16)))); } if (c) goto L_1393ed0; }
    { const uint32_t _dynInsn = A1_R32(0x1393EA4u); const int32_t _dynSimm = (int16_t)(_dynInsn & 0xFFFFu); const uint32_t _dynImm = _dynInsn & 0xFFFFu; (void)_dynSimm; (void)_dynImm; (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32(ADD32((uint32_t)v0, _dynSimm))))); }
    { const bool c = (int64_t)a0 == (int64_t)v1; (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)SRL32((uint32_t)a3, 18)))); if (c) goto L_1393ed0; }
    (a1 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)a0, 8)))); a3xw0 = true;
    (v0 = (uint64_t)(RC_U64X(v0) & (uint64_t)(uint16_t)4095));
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)a0, 16))));
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)a1, 12))));
    { int64_t result = (int64_t)(int32_t)v0 * (int64_t)(int32_t)v1; lo = (uint64_t)(int64_t)(int32_t)result; hi = (uint64_t)(int64_t)(int32_t)(result >> 32); (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)result))); }
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)a0, (uint32_t)v0))));
    goto L_1393ed4;
L_1393ed0:;
    (a0 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL));
L_1393ed4:;
    { const uint32_t _dynInsn = A1_R32(0x1393ED4u); const int32_t _dynSimm = (int16_t)(_dynInsn & 0xFFFFu); const uint32_t _dynImm = _dynInsn & 0xFFFFu; (void)_dynSimm; (void)_dynImm; (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)_dynImm << 16)))); }
    { const uint32_t _dynInsn = A1_R32(0x1393ED8u); const int32_t _dynSimm = (int16_t)(_dynInsn & 0xFFFFu); const uint32_t _dynImm = _dynInsn & 0xFFFFu; (void)_dynSimm; (void)_dynImm; (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32(ADD32((uint32_t)v0, _dynSimm))))); }
    { const bool c = (int64_t)a0 == (int64_t)v1; { const uint32_t _dynInsn = A1_R32(0x1393EE0u); const int32_t _dynSimm = (int16_t)(_dynInsn & 0xFFFFu); const uint32_t _dynImm = _dynInsn & 0xFFFFu; (void)_dynSimm; (void)_dynImm; (t2 = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)_dynImm << 16)))); } a3xw5 = true; if (c) goto L_13940b0; }
    (a3 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)a2, 0)))); a3xw2 = true;
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)3 << 16))));
    (v0 = (uint64_t)(RC_U64X(v0) | (uint64_t)(uint16_t)65535));
    (v0 = (uint64_t)(RC_U64X(a3) & RC_U64X(v0)));
    { const bool c = (int64_t)v0 == 0; (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)SUB32((uint32_t)a2, (uint32_t)v0)))); if (c) goto L_1393f08; }
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)v0, 0))));
    goto L_1393f0c;
L_1393f08:;
    (a0 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL));
L_1393f0c:;
    { const bool c = (int64_t)a0 == 0; { const uint32_t _dynInsn = A1_R32(0x1393F10u); const int32_t _dynSimm = (int16_t)(_dynInsn & 0xFFFFu); const uint32_t _dynImm = _dynInsn & 0xFFFFu; (void)_dynSimm; (void)_dynImm; (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)_dynImm << 16)))); } if (c) goto L_1393f40; }
    { const uint32_t _dynInsn = A1_R32(0x1393F14u); const int32_t _dynSimm = (int16_t)(_dynInsn & 0xFFFFu); const uint32_t _dynImm = _dynInsn & 0xFFFFu; (void)_dynSimm; (void)_dynImm; (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32(ADD32((uint32_t)v0, _dynSimm))))); }
    { const bool c = (int64_t)a0 == (int64_t)v1; (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)SRL32((uint32_t)a3, 18)))); if (c) goto L_1393f40; }
    (a1 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)a0, 8)))); a3xw0 = true;
    (v0 = (uint64_t)(RC_U64X(v0) & (uint64_t)(uint16_t)4095));
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)a0, 16))));
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)a1, 12))));
    { int64_t result = (int64_t)(int32_t)v0 * (int64_t)(int32_t)v1; lo = (uint64_t)(int64_t)(int32_t)result; hi = (uint64_t)(int64_t)(int32_t)(result >> 32); (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)result))); }
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)a0, (uint32_t)v0))));
    goto L_1393f44;
L_1393f40:;
    (a0 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL));
L_1393f44:;
    { const uint32_t _dynInsn = A1_R32(0x1393F44u); const int32_t _dynSimm = (int16_t)(_dynInsn & 0xFFFFu); const uint32_t _dynImm = _dynInsn & 0xFFFFu; (void)_dynSimm; (void)_dynImm; (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)_dynImm << 16)))); }
    { const uint32_t _dynInsn = A1_R32(0x1393F48u); const int32_t _dynSimm = (int16_t)(_dynInsn & 0xFFFFu); const uint32_t _dynImm = _dynInsn & 0xFFFFu; (void)_dynSimm; (void)_dynImm; (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32(ADD32((uint32_t)v0, _dynSimm))))); }
    { const bool c = (int64_t)a0 == (int64_t)v1; { const uint32_t _dynInsn = A1_R32(0x1393F50u); const int32_t _dynSimm = (int16_t)(_dynInsn & 0xFFFFu); const uint32_t _dynImm = _dynInsn & 0xFFFFu; (void)_dynSimm; (void)_dynImm; (t2 = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)_dynImm << 16)))); } a3xw5 = true; if (c) goto L_13940b0; }
    (a3 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)a2, 0)))); a3xw2 = true;
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)3 << 16))));
    (v0 = (uint64_t)(RC_U64X(v0) | (uint64_t)(uint16_t)65535));
    (v0 = (uint64_t)(RC_U64X(a3) & RC_U64X(v0)));
    { const bool c = (int64_t)v0 == 0; (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)SUB32((uint32_t)a2, (uint32_t)v0)))); if (c) goto L_1393f78; }
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)v0, 0))));
    goto L_1393f7c;
L_1393f78:;
    (a0 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL));
L_1393f7c:;
    { const bool c = (int64_t)a0 == 0; { const uint32_t _dynInsn = A1_R32(0x1393F80u); const int32_t _dynSimm = (int16_t)(_dynInsn & 0xFFFFu); const uint32_t _dynImm = _dynInsn & 0xFFFFu; (void)_dynSimm; (void)_dynImm; (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)_dynImm << 16)))); } if (c) goto L_1393fb0; }
    { const uint32_t _dynInsn = A1_R32(0x1393F84u); const int32_t _dynSimm = (int16_t)(_dynInsn & 0xFFFFu); const uint32_t _dynImm = _dynInsn & 0xFFFFu; (void)_dynSimm; (void)_dynImm; (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32(ADD32((uint32_t)v0, _dynSimm))))); }
    { const bool c = (int64_t)a0 == (int64_t)v1; (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)SRL32((uint32_t)a3, 18)))); if (c) goto L_1393fb0; }
    (a1 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)a0, 8)))); a3xw0 = true;
    (v0 = (uint64_t)(RC_U64X(v0) & (uint64_t)(uint16_t)4095));
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)a0, 16))));
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)a1, 12))));
    { int64_t result = (int64_t)(int32_t)v0 * (int64_t)(int32_t)v1; lo = (uint64_t)(int64_t)(int32_t)result; hi = (uint64_t)(int64_t)(int32_t)(result >> 32); (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)result))); }
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)a0, (uint32_t)v0))));
    goto L_1393fb4;
L_1393fb0:;
    (a0 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL));
L_1393fb4:;
    { const uint32_t _dynInsn = A1_R32(0x1393FB4u); const int32_t _dynSimm = (int16_t)(_dynInsn & 0xFFFFu); const uint32_t _dynImm = _dynInsn & 0xFFFFu; (void)_dynSimm; (void)_dynImm; (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)_dynImm << 16)))); }
    { const uint32_t _dynInsn = A1_R32(0x1393FB8u); const int32_t _dynSimm = (int16_t)(_dynInsn & 0xFFFFu); const uint32_t _dynImm = _dynInsn & 0xFFFFu; (void)_dynSimm; (void)_dynImm; (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32(ADD32((uint32_t)v0, _dynSimm))))); }
    { const bool c = (int64_t)a0 == (int64_t)v1; { const uint32_t _dynInsn = A1_R32(0x1393FC0u); const int32_t _dynSimm = (int16_t)(_dynInsn & 0xFFFFu); const uint32_t _dynImm = _dynInsn & 0xFFFFu; (void)_dynSimm; (void)_dynImm; (t2 = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)_dynImm << 16)))); } a3xw5 = true; if (c) goto L_13940b0; }
    (a3 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)a2, 0)))); a3xw2 = true;
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)3 << 16))));
    (v0 = (uint64_t)(RC_U64X(v0) | (uint64_t)(uint16_t)65535));
    (v0 = (uint64_t)(RC_U64X(a3) & RC_U64X(v0)));
    { const bool c = (int64_t)v0 == 0; (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)SUB32((uint32_t)a2, (uint32_t)v0)))); if (c) goto L_1393fe8; }
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)v0, 0))));
    goto L_1393fec;
L_1393fe8:;
    (a0 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL));
L_1393fec:;
    { const bool c = (int64_t)a0 == 0; { const uint32_t _dynInsn = A1_R32(0x1393FF0u); const int32_t _dynSimm = (int16_t)(_dynInsn & 0xFFFFu); const uint32_t _dynImm = _dynInsn & 0xFFFFu; (void)_dynSimm; (void)_dynImm; (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)_dynImm << 16)))); } if (c) goto L_1394020; }
    { const uint32_t _dynInsn = A1_R32(0x1393FF4u); const int32_t _dynSimm = (int16_t)(_dynInsn & 0xFFFFu); const uint32_t _dynImm = _dynInsn & 0xFFFFu; (void)_dynSimm; (void)_dynImm; (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32(ADD32((uint32_t)v0, _dynSimm))))); }
    { const bool c = (int64_t)a0 == (int64_t)v1; (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)SRL32((uint32_t)a3, 18)))); if (c) goto L_1394020; }
    (a1 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)a0, 8)))); a3xw0 = true;
    (v0 = (uint64_t)(RC_U64X(v0) & (uint64_t)(uint16_t)4095));
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)a0, 16))));
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)a1, 12))));
    { int64_t result = (int64_t)(int32_t)v0 * (int64_t)(int32_t)v1; lo = (uint64_t)(int64_t)(int32_t)result; hi = (uint64_t)(int64_t)(int32_t)(result >> 32); (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)result))); }
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)a0, (uint32_t)v0))));
    goto L_1394024;
L_1394020:;
    (a0 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL));
L_1394024:;
    { const uint32_t _dynInsn = A1_R32(0x1394024u); const int32_t _dynSimm = (int16_t)(_dynInsn & 0xFFFFu); const uint32_t _dynImm = _dynInsn & 0xFFFFu; (void)_dynSimm; (void)_dynImm; (t2 = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)_dynImm << 16)))); } a3xw5 = true;
    { const uint32_t _dynInsn = A1_R32(0x1394028u); const int32_t _dynSimm = (int16_t)(_dynInsn & 0xFFFFu); const uint32_t _dynImm = _dynInsn & 0xFFFFu; (void)_dynSimm; (void)_dynImm; (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32(ADD32((uint32_t)t2, _dynSimm))))); }
    if ((int64_t)a0 == (int64_t)v0) { A1_W32B((uint32_t)t0, 4, 0u); goto L_13940b4; }
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)a2, 0))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)3 << 16))));
    (v0 = (uint64_t)(RC_U64X(v0) | (uint64_t)(uint16_t)65535));
    (v0 = (uint64_t)(RC_U64X(a0) & RC_U64X(v0)));
    { const bool c = (int64_t)v0 == 0; (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)SUB32((uint32_t)a2, (uint32_t)v0)))); if (c) goto L_1394058; }
    (a2 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)v0, 0)))); a3xw1 = true;
    goto L_139405c;
L_1394058:;
    (a2 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL)); a3xw1 = true;
L_139405c:;
    { const bool c = (int64_t)a2 == 0; { const uint32_t _dynInsn = A1_R32(0x1394060u); const int32_t _dynSimm = (int16_t)(_dynInsn & 0xFFFFu); const uint32_t _dynImm = _dynInsn & 0xFFFFu; (void)_dynSimm; (void)_dynImm; (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)_dynImm << 16)))); } if (c) goto L_1394090; }
    { const uint32_t _dynInsn = A1_R32(0x1394064u); const int32_t _dynSimm = (int16_t)(_dynInsn & 0xFFFFu); const uint32_t _dynImm = _dynInsn & 0xFFFFu; (void)_dynSimm; (void)_dynImm; (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32(ADD32((uint32_t)v0, _dynSimm))))); }
    { const bool c = (int64_t)a2 == (int64_t)v1; (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)SRL32((uint32_t)a0, 18)))); if (c) goto L_1394090; }
    (a1 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)a2, 8)))); a3xw0 = true;
    (v0 = (uint64_t)(RC_U64X(v0) & (uint64_t)(uint16_t)4095));
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)a2, 16))));
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)a1, 12))));
    { int64_t result = (int64_t)(int32_t)v0 * (int64_t)(int32_t)v1; lo = (uint64_t)(int64_t)(int32_t)result; hi = (uint64_t)(int64_t)(int32_t)(result >> 32); (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)result))); }
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)a0, (uint32_t)v0))));
    goto L_1394094;
L_1394090:;
    (a0 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL));
L_1394094:;
    { const uint32_t _dynInsn = A1_R32(0x1394094u); const int32_t _dynSimm = (int16_t)(_dynInsn & 0xFFFFu); const uint32_t _dynImm = _dynInsn & 0xFFFFu; (void)_dynSimm; (void)_dynImm; (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)_dynImm << 16)))); }
    { const uint32_t _dynInsn = A1_R32(0x1394098u); const int32_t _dynSimm = (int16_t)(_dynInsn & 0xFFFFu); const uint32_t _dynImm = _dynInsn & 0xFFFFu; (void)_dynSimm; (void)_dynImm; (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32(ADD32((uint32_t)v0, _dynSimm))))); }
    if ((int64_t)a0 != (int64_t)v1) { { const uint32_t _dynInsn = A1_R32(0x13940A0u); const int32_t _dynSimm = (int16_t)(_dynInsn & 0xFFFFu); const uint32_t _dynImm = _dynInsn & 0xFFFFu; (void)_dynSimm; (void)_dynImm; (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)_dynImm << 16)))); } goto L_1394124; }
    A1_W32B((uint32_t)t0, 4, 0u);
    goto L_13940b4;
L_13940b0:;
    A1_W32B((uint32_t)t0, 4, 0u);
L_13940b4:;
    (a1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 1)))); a3xw0 = true;
    A1_W32B((uint32_t)t0, 52, 0u);
    { const uint32_t _dynInsn = A1_R32(0x13940BCu); const int32_t _dynSimm = (int16_t)(_dynInsn & 0xFFFFu); const uint32_t _dynImm = _dynInsn & 0xFFFFu; (void)_dynSimm; (void)_dynImm; (a2 = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)_dynImm << 16)))); } a3xw1 = true;
    A1_W32B((uint32_t)t0, 56, 0u);
    { const uint32_t _dynInsn = A1_R32(0x13940C4u); const int32_t _dynSimm = (int16_t)(_dynInsn & 0xFFFFu); const uint32_t _dynImm = _dynInsn & 0xFFFFu; (void)_dynSimm; (void)_dynImm; (a3 = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)_dynImm << 16)))); } a3xw2 = true;
    A1_W32B((uint32_t)t0, 60, 0u);
    A1_W32B((uint32_t)t0, 64, 0u);
    A1_W32B((uint32_t)t0, 68, 0u);
    { const uint32_t _dynInsn = A1_R32(0x13940D4u); const int32_t _dynSimm = (int16_t)(_dynInsn & 0xFFFFu); const uint32_t _dynImm = _dynInsn & 0xFFFFu; (void)_dynSimm; (void)_dynImm; (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32(ADD32((uint32_t)t2, _dynSimm))))); }
    (a0 = (uint64_t)(A1_R64B((uint32_t)t1, 448)));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)v1, 4))));
    (v0 = (uint64_t)(RC_U64X(a1) << ((uint32_t)v0 & 0x3F)));
    (v0 = (uint64_t)(~(0ULL | v0)));
    (a0 = (uint64_t)(a0 & v0));
    A1_W64B((uint32_t)t1, 448, a0);
    { const uint32_t _dynInsn = A1_R32(0x13940F0u); const int32_t _dynSimm = (int16_t)(_dynInsn & 0xFFFFu); const uint32_t _dynImm = _dynInsn & 0xFFFFu; (void)_dynSimm; (void)_dynImm; (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32(ADD32((uint32_t)a2, _dynSimm))))); }
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)v1, 4))));
    (v0 = (uint64_t)(RC_U64X(a1) << ((uint32_t)v0 & 0x3F)));
    (v0 = (uint64_t)(~(0ULL | v0)));
    (a0 = (uint64_t)(a0 & v0));
    A1_W64B((uint32_t)t1, 448, a0);
    { const uint32_t _dynInsn = A1_R32(0x1394108u); const int32_t _dynSimm = (int16_t)(_dynInsn & 0xFFFFu); const uint32_t _dynImm = _dynInsn & 0xFFFFu; (void)_dynSimm; (void)_dynImm; (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32(ADD32((uint32_t)a3, _dynSimm))))); }
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)v0, 4))));
    (a1 = (uint64_t)(RC_U64X(a1) << ((uint32_t)v1 & 0x3F))); a3xw0 = true;
    (a1 = (uint64_t)(~(0ULL | a1))); a3xw0 = true;
    (a0 = (uint64_t)(a0 & a1));
    A1_W64B((uint32_t)t1, 448, a0);
L_1394120:;
    { const uint32_t _dynInsn = A1_R32(0x1394120u); const int32_t _dynSimm = (int16_t)(_dynInsn & 0xFFFFu); const uint32_t _dynImm = _dynInsn & 0xFFFFu; (void)_dynSimm; (void)_dynImm; (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)_dynImm << 16)))); }
L_1394124:;
    (a1 = (uint64_t)(A1_R64B((uint32_t)t1, 448))); a3xw0 = true;
    { const uint32_t _dynInsn = A1_R32(0x1394128u); const int32_t _dynSimm = (int16_t)(_dynInsn & 0xFFFFu); const uint32_t _dynImm = _dynInsn & 0xFFFFu; (void)_dynSimm; (void)_dynImm; (a2 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32(ADD32((uint32_t)v0, _dynSimm))))); } a3xw1 = true;
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 1))));
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 1))));
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)a2, 4))));
    (v1 = (uint64_t)(RC_U64X(v1) << ((uint32_t)a0 & 0x3F)));
    (a1 = (uint64_t)(a1 | v1)); a3xw0 = true;
    A1_W64B((uint32_t)t1, 448, a1);
L_1394144:;
    { if (xo) { if (a3xw0) xo[0] = a1; if (a3xw1) xo[1] = a2; if (a3xw2) xo[2] = a3; xo[3] = t0; xo[4] = t1; if (a3xw5) xo[5] = t2; } return a1::RetVF{(uint64_t)v0, f0}; }
    __builtin_unreachable();
}

// gamecore_sub_0139F3B0 (0x139f3b0): typed by tools/typed_lift2.py (round A3; loop leaf)
inline uint64_t gamecore_sub_0139F3B0(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint64_t a0)
{
    uint64_t v0 = 0;
    uint32_t v1 = 0;
    uint64_t a1 = 0;
    uint64_t a2 = 0;
    uint64_t a3 = 0;
    (a2 = (uint64_t)((uint64_t)a0 + (uint64_t)0ULL));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 4294967295))));
    (v1 = (uint32_t)((int32_t)((int32_t)A1_R32B((uint32_t)a2, 36))));
    { const bool c = (int64_t)(int32_t)v1 == (int64_t)v0; (a3 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL)); if (c) goto L_139f460; }
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)SLL32(v1, 2))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)a2, (uint32_t)v0))));
    (v1 = (uint32_t)((int32_t)((int32_t)A1_R32B((uint32_t)v0, 8))));
    { const bool c = v1 != 0; (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 1)))); if (c) goto L_139f438; }
    (v1 = (uint32_t)((int32_t)((int32_t)A1_R32B((uint32_t)a2, 284))));
    { const bool c = (int64_t)(int32_t)v1 == (int64_t)v0; (a1 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL)); if (c) goto L_139f418; }
    (a1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)a1, 1))));
L_139f3e8:;
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)SLL32((uint32_t)a1, 2))));
    (v1 = (uint32_t)(((int64_t)(int64_t)a1 < (int64_t)(int32_t)10) ? 1 : 0));
    { const bool c = v1 == 0; (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v0, (uint32_t)a1)))); if (c) goto L_139f41c; }
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)SLL32((uint32_t)v0, 2))));
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 1))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)SUB32((uint32_t)v0, (uint32_t)a1))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)SLL32((uint32_t)v0, 3))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v0, (uint32_t)a2))));
    (v1 = (uint32_t)((int32_t)((int32_t)A1_R32B((uint32_t)v0, 284))));
    if ((int64_t)(int32_t)v1 != (int64_t)a0) { (a1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)a1, 1)))); { A3_BACK(); goto L_139f3e8; } }
L_139f418:;
    (a3 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 1))));
L_139f41c:;
    { const bool c = (int64_t)a3 == 0; (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 4294967295)))); if (c) goto L_139f464; }
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)a2, 40))));
    if ((int32_t)a0 < 0) { (a0 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL)); goto L_139f464; }
    { (v0 = (uint64_t)((uint64_t)RC_U64X(a0) + (uint64_t)0ULL)); return v0; }
L_139f438:;
    { const bool c = (int64_t)(int32_t)v1 == (int64_t)v0; (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 4294967293)))); if (c) goto L_139f464; }
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 3))));
    { const bool c = (int64_t)(int32_t)v1 == (int64_t)v0; (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 4294967294)))); if (c) goto L_139f464; }
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 5))));
    { const bool c = (int64_t)(int32_t)v1 != (int64_t)v0; (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 4294967292)))); if (c) goto L_139f464; }
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 4294967294))));
    goto L_139f464;
L_139f460:;
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 4294967293))));
L_139f464:;
    (v0 = (uint64_t)((uint64_t)a0 + (uint64_t)0ULL));
    { return v0; }
    __builtin_unreachable();
}

// AI_GetPosition3 (0x13d46d8): typed by tools/typed_lift2.py (round A3; leaf, extra outputs xo[0]=f13 xo[1]=f14 xo[2]=f16 xo[3]=f17)
inline float AI_GetPosition3(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, float f12, float f13, float f14, float f15, float f16, float f17, uint64_t *xo = nullptr)
{
    float f0 = 0.0f;
    float f1 = 0.0f;
    float f2 = 0.0f;
    float f3 = 0.0f;
    float f4 = 0.0f;
    uint32_t at = 0;
    uint32_t fcr31 = 0;
    f2 = a1f::mul(f17, f17);
    at = 0x41400000u;
    f4 = a1_flt(at);
    f16 = a1f::mul(f16, f17);
    at = 0x40C00000u;
    f3 = a1_flt(at);
    at = 0x3F000000u;
    f0 = a1_flt(at);
    f1 = a1f::mul(f2, f17);
    f14 = a1f::mul(f14, f0);
    f17 = a1f::mul(f1, f17);
    f13 = a1f::mul(f13, f1);
    f14 = ((float)(f14) * (float)(f2));
    f12 = a1f::mul(f12, f17);
    f13 = a1f::div(fcr31, f13, f3);
    f12 = a1f::div(fcr31, f12, f4);
    f0 = a1f::add(f15, f12);
    f0 = a1f::addFinite(f0, f13);
    f0 = a1f::addFinite(f0, f14);
    { f0 = a1f::add(f0, f16); if (xo) { xo[0] = a1_bits(f13); xo[1] = a1_bits(f14); xo[2] = a1_bits(f16); xo[3] = a1_bits(f17); } return f0; }
    __builtin_unreachable();
}

// AI_GetInterRatio (0x13d4a28): typed by tools/typed_lift2.py (round A3; leaf, extra outputs xo[0]=f14)
inline float AI_GetInterRatio(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, float f12, float f13, float f14, float f15, float f16, uint64_t *xo = nullptr)
{
    float f0 = 0.0f;
    float f1 = 0.0f;
    uint32_t fcr31 = 0;
    f14 = a1f::sub(f14, f13);
    f1 = a1_flt(0u);
    fcr31 = (a1f::eq(f14, f1)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
    { const bool c = (fcr31 & 0x800000u) != 0u; f0 = ((float)(f15)); if (c) goto L_13d4a58; }
    f0 = a1f::sub(f12, f13);
    f1 = a1f::sub(f16, f15);
    f0 = ((float)(f0) * (float)(f1));
    f0 = a1f::div(fcr31, f0, f14);
    f0 = a1f::add(f0, f15);
L_13d4a58:;
    { if (xo) { xo[0] = a1_bits(f14); } return f0; }
    __builtin_unreachable();
}

// AI_GetInterRatioOutCut (0x13d4a60): typed by tools/typed_lift2.py (round A3; leaf, extra outputs xo[0]=f14)
inline float AI_GetInterRatioOutCut(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, float f12, float f13, float f14, float f15, float f16, uint64_t *xo = nullptr)
{
    float f0 = 0.0f;
    float f1 = 0.0f;
    uint32_t fcr31 = 0;
    bool a3xw0 = false;
    fcr31 = (a1f::le(f12, f13)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
    { const bool c = (fcr31 & 0x800000u) != 0u; f0 = ((float)(f15)); if (c) goto L_13d4ab0; }
    fcr31 = (a1f::le(f14, f12)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
    { const bool c = (fcr31 & 0x800000u) != 0u; f0 = ((float)(f16)); if (c) goto L_13d4ab0; }
    f14 = a1f::sub(f14, f13); a3xw0 = true;
    f0 = a1_flt(0u);
    fcr31 = (a1f::eq(f14, f0)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
    if ((fcr31 & 0x800000u) != 0u) { f0 = ((float)(f15)); goto L_13d4ab0; }
    f0 = a1f::sub(f12, f13);
    f1 = a1f::sub(f16, f15);
    f0 = ((float)(f0) * (float)(f1));
    f0 = a1f::div(fcr31, f0, f14);
    { f0 = a1f::add(f0, f15); if (xo) { xo[0] = a1_bits(f14); } return f0; }
L_13d4ab0:;
    { if (xo) { if (a3xw0) xo[0] = a1_bits(f14); } return f0; }
    __builtin_unreachable();
}

// Kite_CmpPathPoint (0x13e79c8): typed by tools/typed_lift2.py (round A3; leaf, extra outputs xo[0]=a0)
inline uint32_t Kite_CmpPathPoint(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t a0, uint32_t a1, uint64_t *xo = nullptr)
{
    uint32_t v0 = 0;
    uint32_t v1 = 0;
    bool a3xw0 = false;
    v1 = A1_R32B(a0, 24);
    v0 = A1_R32B(a1, 24);
    { const bool c = v1 != v0; v0 = 0u; if (c) goto L_13e79ec; }
    a0 = A1_R32B(a0, 16); a3xw0 = true;
    v1 = A1_R32B(a1, 16);
    { const bool c = a0 == v1; v0 = 1u; if (c) goto L_13e79ec; }
    v0 = 0u;
L_13e79ec:;
    { if (xo) { if (a3xw0) xo[0] = a1_sx64(a0); } return v0; }
    __builtin_unreachable();
}

// gamecore_sub_013EF210 (0x13ef210): typed by tools/typed_lift2.py (round A3; loop leaf)
inline a1::RetVF gamecore_sub_013EF210(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint64_t a0, uint64_t a1, uint32_t a2)
{
    float f0 = 0.0f;
    uint32_t v0 = 0;
    uint32_t v1 = 0;
    uint32_t a3 = 0;
    uint64_t t0 = 0;
    uint32_t t1 = 0;
    uint64_t t2 = 0;
    uint32_t t3 = 0;
    uint32_t t4 = 0;
    uint64_t t5 = 0;
    uint32_t t6 = 0;
    uint32_t t7 = 0;
    uint32_t t8 = 0;
    uint32_t t9 = 0;
    (t2 = (uint64_t)((uint64_t)a0 + (uint64_t)0ULL));
    v1 = 0x1470000u;
    (t1 = (uint32_t)((int32_t)((int32_t)ADD32((uint32_t)t2, 580))));
    (v0 = (uint32_t)((int32_t)((int32_t)ADD32((uint32_t)t2, 568))));
    A1_W32(t1, 0u);
    (t5 = (uint64_t)((uint64_t)a1 + (uint64_t)0ULL));
    A1_W32B(t1, 4, 0u);
    (t9 = (uint32_t)((int32_t)((int32_t)ADD32((uint32_t)t2, 4))));
    A1_W32B(t1, 8, 0u);
    t4 = 0xFFFFFFFFu;
    A1_W32B(v0, 8, 0u);
    (t8 = (uint32_t)((int32_t)((int32_t)ADD32((uint32_t)t2, 8))));
    A1_W32(v0, 0u);
    (t7 = (uint32_t)((int32_t)((int32_t)ADD32((uint32_t)t2, 12))));
    A1_W32B(v0, 4, 0u);
    (t6 = (uint32_t)((int32_t)((int32_t)ADD32((uint32_t)t2, 16))));
    A1_W32B((uint32_t)t2, 0, 0u);
    (a3 = (uint32_t)((int32_t)((int32_t)ADD32((uint32_t)t2, 20))));
    f0 = a1_flt(A1_R32(v1 - 0x4B40u));
    (t0 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL));
    t3 = 9u;
L_13ef268:;
    (v0 = (uint32_t)((int32_t)((int32_t)ADD32(t9, (uint32_t)t0))));
    (v1 = (uint32_t)((int32_t)((int32_t)ADD32(t8, (uint32_t)t0))));
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(t7, (uint32_t)t0))));
    (a1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(t6, (uint32_t)t0))));
    t3 = t3 - 1u;
    A1_W32(v0, t4);
    (t0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)t0, 28))));
    A1_W32(v1, t4);
    { float f = f0; uint32_t bits; std::memcpy(&bits, &f, sizeof(bits)); A1_W32B((uint32_t)a0, 0, bits); }
    A1_W32B((uint32_t)a1, 0, 0u);
    A1_W32(a3, 0u);
    A1_W32B(a3, 4, 0u);
    A1_W32B(a3, 8, 0u);
    { const bool c = (int32_t)t3 >= 0; a3 = a3 + 0x1Cu; if (c) { A3_BACK(); goto L_13ef268; } }
    A1_W32B((uint32_t)t2, 2516, a2);
    { const bool c = (int32_t)t5 <= 0; A1_W32B((uint32_t)t2, 2512, (uint32_t)t5); if (c) goto L_13ef318; }
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 4294967295))));
    (v1 = (uint32_t)((int32_t)((int32_t)ADD32((uint32_t)t2, 592))));
    (a1 = (uint64_t)((uint64_t)t5 + (uint64_t)0ULL));
L_13ef2c0:;
    if (v1 == 0) { (a1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)a1, 4294967295)))); goto L_13ef2f0; }
    v0 = A1_R32B(t1, 8);
    if (v0 == 0) { A1_W32B(t1, 4, v1); goto L_13ef2d8; }
    A1_W32B(v0, 88, v1);
L_13ef2d8:;
    A1_W32B(v1, 88, 0u);
    A1_W32B(t1, 8, v1);
    v0 = A1_R32(t1);
    v0 = v0 + 1u;
    A1_W32(t1, v0);
    (a1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)a1, 4294967295))));
L_13ef2f0:;
    A1_W32(v1, 0u);
    A1_W32B(v1, 4, 0u);
    A1_W32B(v1, 8, 0u);
    A1_W32B(v1, 12, (uint32_t)a0);
    A1_W32B(v1, 16, 0u);
    A1_W32B(v1, 84, 0u);
    A1_W16B(v1, 80, (uint16_t)(uint32_t)a0);
    A1_W16B(v1, 82, (uint16_t)(uint32_t)a0);
    { const bool c = (int64_t)a1 != 0; v1 = v1 + 0x60u; if (c) { A3_BACK(); goto L_13ef2c0; } }
L_13ef318:;
    (a1 = (uint64_t)((uint64_t)t5 + (uint64_t)0ULL));
    (v0 = (uint32_t)(((int64_t)(int64_t)a1 < (int64_t)(int32_t)20) ? 1 : 0));
    { const bool c = v0 == 0; (v0 = (uint32_t)((int32_t)((int32_t)SLL32((uint32_t)a1, 1)))); if (c) goto L_13ef370; }
    v1 = 0xFFFFFFFFu;
    (v0 = (uint32_t)((int32_t)((int32_t)ADD32(v0, (uint32_t)a1))));
    v0 = v0 << 5;
    (v0 = (uint32_t)((int32_t)((int32_t)ADD32(v0, (uint32_t)t2))));
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(v0, 592))));
L_13ef340:;
    (a1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)a1, 1))));
    A1_W32B((uint32_t)a0, 0, 0u);
    (v0 = (uint32_t)(((int64_t)(int64_t)RC_U64X(a1) < (int64_t)(int32_t)20) ? 1 : 0));
    A1_W32B((uint32_t)a0, 4, 0u);
    A1_W32B((uint32_t)a0, 8, 0u);
    A1_W32B((uint32_t)a0, 12, v1);
    A1_W32B((uint32_t)a0, 16, 0u);
    A1_W32B((uint32_t)a0, 84, 0u);
    A1_W16B((uint32_t)a0, 80, (uint16_t)v1);
    A1_W16B((uint32_t)a0, 82, (uint16_t)v1);
    { const bool c = v0 != 0; (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)a0, 96)))); if (c) { A3_BACK(); goto L_13ef340; } }
L_13ef370:;
    { return a1::RetVF{(uint64_t)a1_sx64(v0), f0}; }
    __builtin_unreachable();
}

// getSObjNode (0x13f0d60): typed by tools/typed_lift2.py (round A3; loop leaf, extra outputs xo[0]=a1 xo[1]=a2 xo[2]=a3 xo[3]=t0 xo[4]=t1 xo[5]=t2 xo[6]=t3)
inline uint32_t getSObjNode(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint64_t a0, uint64_t *xo = nullptr)
{
    uint32_t v0 = 0;
    uint64_t v1 = 0;
    uint32_t a1 = 0;
    uint32_t a2 = 0;
    uint32_t a3 = 0;
    uint32_t t0 = 0;
    uint64_t t1 = 0;
    uint32_t t2 = 0;
    uint32_t t3 = 0;
    bool a3xw0 = false;
    bool a3xw1 = false;
    bool a3xw3 = false;
    (t1 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL));
    t3 = 0x11F0000u;
    t2 = 0x11F0000u;
    v0 = t2 + 0x7B30u;
L_13f0d70:;
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(t3, 29008))));
    if (t1 != 0) v1 = (uint64_t)(a1_sx64(v0));;
    (v0 = (uint32_t)((int32_t)((int32_t)A1_R32B((uint32_t)v1, 2512))));
    { const bool c = (int32_t)v0 <= 0; a3 = 0u; if (c) goto L_13f0dbc; }
    (a2 = (uint32_t)((int32_t)((int32_t)ADD32((uint32_t)v1, 592)))); a3xw1 = true;
    t0 = v0; a3xw3 = true;
    a1 = a2; a3xw0 = true;
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B(a2, 0))));
L_13f0d98:;
    a2 = a2 + 0x60u; a3xw1 = true;
    v0 = a1;
    { const bool c = (int64_t)a0 == (int64_t)v1; a1 = a1 + 0x60u; a3xw0 = true; if (c) goto L_13f0dd0; }
    a3 = a3 + 1u;
    v0 = (int32_t)a3 < (int32_t)t0 ? 1u : 0u;
    if (v0 != 0) { (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B(a2, 0)))); { A3_BACK(); goto L_13f0d98; } }
L_13f0dbc:;
    (t1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)t1, 1))));
    (v0 = (uint32_t)(((int64_t)(int64_t)RC_U64X(t1) < (int64_t)(int32_t)2) ? 1 : 0));
    if (v0 != 0) { v0 = t2 + 0x7B30u; { A3_BACK(); goto L_13f0d70; } }
    v0 = 0u;
L_13f0dd0:;
    { if (xo) { if (a3xw0) xo[0] = a1_sx64(a1); if (a3xw1) xo[1] = a1_sx64(a2); xo[2] = a1_sx64(a3); if (a3xw3) xo[3] = a1_sx64(t0); xo[4] = t1; xo[5] = a1_sx64(t2); xo[6] = a1_sx64(t3); } return v0; }
    __builtin_unreachable();
}

// gamecore_sub_013F1290 (0x13f1290): typed by tools/typed_lift2.py (round A3; leaf, extra outputs xo[0]=a1)
inline uint32_t gamecore_sub_013F1290(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t a0, uint64_t *xo = nullptr)
{
    uint32_t v0 = 0;
    uint32_t v1 = 0;
    uint32_t a1 = 0;
    v1 = A1_R32B(a0, 16);
    { const uint32_t _dynInsn = A1_R32(0x13F1294u); const int32_t _dynSimm = (int16_t)(_dynInsn & 0xFFFFu); const uint32_t _dynImm = _dynInsn & 0xFFFFu; (void)_dynSimm; (void)_dynImm; (v0 = (uint32_t)((int32_t)((int32_t)((uint32_t)_dynImm << 16)))); }
    { const bool c = v1 == 0; { const uint32_t _dynInsn = A1_R32(0x13F129Cu); const int32_t _dynSimm = (int16_t)(_dynInsn & 0xFFFFu); const uint32_t _dynImm = _dynInsn & 0xFFFFu; (void)_dynSimm; (void)_dynImm; (a1 = (uint32_t)((int32_t)((int32_t)A1_R32(ADD32(v0, _dynSimm))))); } if (c) goto L_13f12c8; }
    v1 = (uint32_t)(int16_t)A1_R16B(a0, 52);
    v0 = v1 << 1;
    v0 = v0 + v1;
    v0 = v0 << 2;
    v0 = v0 + v1;
    v0 = v0 << 2;
    v0 = a1 + v0;
    { v0 = v0 + 8u; if (xo) { xo[0] = a1_sx64(a1); } return v0; }
L_13f12c8:;
    v1 = (uint32_t)(int16_t)A1_R16B(a0, 52);
    v0 = v1 << 1;
    v0 = v0 + v1;
    v0 = v0 << 2;
    v0 = v0 + v1;
    v0 = v0 << 2;
    v0 = a1 + v0;
    { v0 = v0 + 4u; if (xo) { xo[0] = a1_sx64(a1); } return v0; }
    __builtin_unreachable();
}

// gamecore_sub_013F12F0 (0x13f12f0): typed by tools/typed_lift2.py (round A3; loop leaf)
inline uint64_t gamecore_sub_013F12F0(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t a0, uint64_t a1)
{
    uint64_t v0 = 0;
    uint32_t v1 = 0;
    a0 = A1_R32B(a0, 20);
    { const bool c = a0 == 0; (v0 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL)); if (c) goto L_13f1328; }
    v1 = A1_R32B(a0, 12);
L_13f1300:;
    { const bool c = (int64_t)a1 == (int64_t)(int32_t)v1; (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 1)))); if (c) goto L_13f1328; }
    a0 = A1_R32B(a0, 88);
    if (a0 != 0) { v1 = A1_R32B(a0, 12); { A3_BACK(); goto L_13f1300; } }
    (v0 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL));
L_13f1328:;
    { return v0; }
    __builtin_unreachable();
}

// gamecore_sub_013F1330 (0x13f1330): typed by tools/typed_lift2.py (round A3; loop leaf, extra outputs xo[0]=a1)
inline uint64_t gamecore_sub_013F1330(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint64_t a0, uint64_t *xo = nullptr)
{
    uint64_t v0 = 0;
    uint32_t v1 = 0;
    uint32_t a1 = 0;
    uint64_t a2 = 0;
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)a0, 16))));
    (a2 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL));
    { const bool c = (int32_t)v0 <= 0; (a1 = (uint32_t)((int32_t)((int32_t)A1_R32B((uint32_t)a0, 20)))); if (c) goto L_13f1364; }
    (a0 = (uint64_t)((uint64_t)RC_U64X(v0) + (uint64_t)0ULL));
L_13f1348:;
    v1 = A1_R32B(a1, 84);
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)a2, 1))));
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)a0, 4294967295))));
    a1 = A1_R32B(a1, 88);
    { const bool c = (int64_t)a0 != 0; if (a1_sx64(v1) != 0) a2 = (uint64_t)(v0);; if (c) { A3_BACK(); goto L_13f1348; } }
L_13f1364:;
    { (v0 = (uint64_t)((uint64_t)a2 + (uint64_t)0ULL)); if (xo) { xo[0] = a1_sx64(a1); } return v0; }
    __builtin_unreachable();
}

// gamecore_sub_013F1370 (0x13f1370): typed by tools/typed_lift2.py (round A3; loop leaf, extra outputs xo[0]=a1 xo[1]=t0 xo[2]=t1 xo[3]=t2 xo[4]=t3)
inline uint64_t gamecore_sub_013F1370(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t a0, uint64_t *xo = nullptr)
{
    uint64_t v0 = 0;
    uint32_t v1 = 0;
    uint32_t a1 = 0;
    uint32_t a2 = 0;
    uint64_t a3 = 0;
    uint32_t t0 = 0;
    uint64_t t1 = 0;
    uint32_t t2 = 0;
    uint32_t t3 = 0;
    bool a3xw0 = false;
    bool a3xw1 = false;
    bool a3xw3 = false;
    bool a3xw4 = false;
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B(a0, 0))));
    { const bool c = (int32_t)v0 <= 0; (t1 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL)); if (c) goto L_13f13d4; }
    t0 = a0 + 0x14u; a3xw1 = true;
    (a3 = (uint64_t)((uint64_t)RC_U64X(v0) + (uint64_t)0ULL));
    t3 = a0 + 8u; a3xw4 = true;
    t2 = a0 + 4u; a3xw3 = true;
    a2 = 0x10u;
L_13f1390:;
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(t2, a2))));
    v1 = t3 + a2;
    (a0 = (uint32_t)((int32_t)((int32_t)A1_R32B((uint32_t)v0, 0))));
    { const bool c = (int32_t)a0 <= 0; a1 = A1_R32(v1); a3xw0 = true; if (c) goto L_13f13c4; }
    a0 = A1_R32(t0);
L_13f13a8:;
    v1 = A1_R32B(a1, 84);
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)t1, 1))));
    a0 = a0 - 1u;
    a1 = A1_R32B(a1, 88); a3xw0 = true;
    { const bool c = a0 != 0; if (a1_sx64(v1) != 0) t1 = (uint64_t)(v0);; if (c) { A3_BACK(); goto L_13f13a8; } }
L_13f13c4:;
    (a3 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)a3, 4294967295))));
    t0 = t0 + 0x1Cu; a3xw1 = true;
    { const bool c = (int64_t)a3 != 0; a2 = a2 + 0x1Cu; if (c) { A3_BACK(); goto L_13f1390; } }
L_13f13d4:;
    { (v0 = (uint64_t)((uint64_t)t1 + (uint64_t)0ULL)); if (xo) { if (a3xw0) xo[0] = a1_sx64(a1); if (a3xw1) xo[1] = a1_sx64(t0); xo[2] = t1; if (a3xw3) xo[3] = a1_sx64(t2); if (a3xw4) xo[4] = a1_sx64(t3); } return v0; }
    __builtin_unreachable();
}

// gamecore_sub_013F13E0 (0x13f13e0): typed by tools/typed_lift2.py (round A3; loop leaf)
inline uint32_t gamecore_sub_013F13E0(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t a0, uint64_t a1)
{
    uint32_t v0 = 0;
    uint32_t v1 = 0;
    v0 = 0xFFFFFFFFu;
    { const bool c = (int64_t)a1 == (int64_t)(int32_t)v0; v0 = 0u; if (c) goto L_13f1428; }
    a0 = A1_R32B(a0, 572);
    if (a0 == 0) goto L_13f1428;
    v1 = A1_R32B(a0, 12);
L_13f1400:;
    { const bool c = (int64_t)a1 == (int64_t)(int32_t)v1; v0 = a0; if (c) goto L_13f1428; }
    a0 = A1_R32B(a0, 88);
    if (a0 != 0) { v1 = A1_R32B(a0, 12); { A3_BACK(); goto L_13f1400; } }
    v0 = 0u;
L_13f1428:;
    { return v0; }
    __builtin_unreachable();
}

// gamecore_sub_013F1430 (0x13f1430): typed by tools/typed_lift2.py (round A3; loop leaf, extra outputs xo[0]=a3)
inline uint64_t gamecore_sub_013F1430(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint64_t a0, uint64_t a1, uint64_t a2, uint64_t *xo = nullptr)
{
    uint64_t v0 = 0;
    uint32_t v1 = 0;
    uint64_t a3 = 0;
    bool a3xw0 = false;
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 4294967295))));
    if ((int64_t)a1 == (int64_t)v0) { (v0 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL)); goto L_13f14d0; }
    { const bool c = (int64_t)a2 == (int64_t)v0; (v0 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL)); if (c) goto L_13f14d0; }
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)a0, 572))));
    { const bool c = (int64_t)a0 == 0; (a3 = (uint64_t)((uint64_t)RC_U64X(a0) + (uint64_t)0ULL)); a3xw0 = true; if (c) goto L_13f148c; }
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)a0, 0))));
L_13f1458:;
    if ((int64_t)v0 == 0) { (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)a0, 88)))); goto L_13f147c; }
    (v0 = (uint64_t)((int64_t)(int32_t)((int16_t)A1_R16B((uint32_t)a0, 82))));
    if ((int64_t)a1 != (int64_t)v0) { (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)a0, 88)))); goto L_13f147c; }
    (v1 = (uint32_t)((int32_t)((int16_t)A1_R16B((uint32_t)a0, 80))));
    { const bool c = (int64_t)a2 == (int64_t)(int32_t)v1; (v0 = (uint64_t)((uint64_t)a0 + (uint64_t)0ULL)); if (c) goto L_13f14d0; }
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)a0, 88))));
L_13f147c:;
    if ((int64_t)a0 != 0) { (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)a0, 0)))); { A3_BACK(); goto L_13f1458; } }
L_13f148c:;
    (a0 = (uint64_t)((uint64_t)a3 + (uint64_t)0ULL));
    { const bool c = (int64_t)a0 == 0; (v0 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL)); if (c) goto L_13f14d0; }
    (v0 = (uint64_t)((int64_t)(int32_t)((int16_t)A1_R16B((uint32_t)a0, 82))));
L_13f14a0:;
    if ((int64_t)a1 != (int64_t)v0) { (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)a0, 88)))); goto L_13f14b8; }
    (v1 = (uint32_t)((int32_t)((int16_t)A1_R16B((uint32_t)a0, 80))));
    { const bool c = (int64_t)a2 == (int64_t)(int32_t)v1; (v0 = (uint64_t)((uint64_t)a0 + (uint64_t)0ULL)); if (c) goto L_13f14d0; }
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)a0, 88))));
L_13f14b8:;
    if ((int64_t)a0 != 0) { (v0 = (uint64_t)((int64_t)(int32_t)((int16_t)A1_R16B((uint32_t)a0, 82)))); { A3_BACK(); goto L_13f14a0; } }
    (v0 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL));
L_13f14d0:;
    { if (xo) { if (a3xw0) xo[0] = a3; } return v0; }
    __builtin_unreachable();
}

// gamecore_sub_013F2558 (0x13f2558): typed by tools/typed_lift2.py (round A3; leaf, extra outputs xo[0]=f0)
inline uint32_t gamecore_sub_013F2558(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t a0, uint64_t *xo = nullptr)
{
    float f0 = 0.0f;
    float f1 = 0.0f;
    float f2 = 0.0f;
    float f3 = 0.0f;
    uint32_t v0 = 0;
    uint32_t v1 = 0;
    uint32_t fcr31 = 0;
    bool a3xw0 = false;
    v1 = A1_R32B(a0, 344);
    v0 = 2u;
    { const bool c = v1 == v0; v0 = 6u; if (c) goto L_13f2598; }
    { const bool c = v1 == v0; v0 = 8u; if (c) goto L_13f2598; }
    { const bool c = v1 == v0; v0 = 9u; if (c) goto L_13f2598; }
    { const bool c = v1 == v0; v0 = 0xAu; if (c) goto L_13f2598; }
    { const bool c = v1 == v0; v0 = 0xBu; if (c) goto L_13f2598; }
    { const bool c = v1 == v0; v0 = 0xDu; if (c) goto L_13f2598; }
    if (v1 != v0) goto L_13f2674;
L_13f2598:;
    v1 = A1_R32B(a0, 380);
    v0 = v1 - 0x1Eu;
    v0 = v0 < 2u ? 1u : 0u;
    { const bool c = v0 == 0; v0 = v1 - 0x1Cu; if (c) goto L_13f25c8; }
    f0 = a1_flt(A1_R32B(a0, 528)); a3xw0 = true;
    f1 = a1_flt(A1_R32B(a0, 536));
    f0 = (-(float)(f0)); a3xw0 = true;
    f1 = (-(float)(f1));
    A1_W32B(a0, 528, a1_bits(f0));
    A1_W32B(a0, 536, a1_bits(f1));
    goto L_13f260c;
L_13f25c8:;
    v0 = v0 < 2u ? 1u : 0u;
    if (v0 == 0) { A1_W32B(a0, 532, 0u); goto L_13f2610; }
    f1 = a1_flt(A1_R32B(a0, 536));
    f0 = a1_flt(0u); a3xw0 = true;
    fcr31 = (a1f::lt(f1, f0)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
    { const bool c = (fcr31 & 0x800000u) == 0u; f2 = a1_flt(A1_R32B(a0, 528)); if (c) goto L_13f25ec; }
    f1 = (-(float)(f1));
L_13f25ec:;
    v0 = 0x1Cu;
    { const bool c = v1 != v0; A1_W32B(a0, 528, a1_bits(f1)); if (c) goto L_13f2608; }
    f0 = (-(float)(f2)); a3xw0 = true;
    A1_W32B(a0, 536, a1_bits(f0));
    goto L_13f260c;
L_13f2608:;
    A1_W32B(a0, 536, a1_bits(f2));
L_13f260c:;
    A1_W32B(a0, 532, 0u);
L_13f2610:;
    f1 = a1_flt(A1_R32B(a0, 528));
    f0 = a1_flt(A1_R32B(a0, 532)); a3xw0 = true;
    v0 = A1_R32B(a0, 100);
    fcr31 = (a1f::lt(f1, f0)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
    f2 = a1_flt(A1_R32B(v0, 20));
    { const bool c = (fcr31 & 0x800000u) == 0u; f3 = a1_flt(A1_R32B(v0, 48)); if (c) goto L_13f2648; }
    f0 = (-(float)(f1)); a3xw0 = true;
    f2 = a1f::div(fcr31, f0, f2);
    goto L_13f2654;
L_13f2648:;
    f2 = a1f::div(fcr31, f1, f2);
L_13f2654:;
    f0 = a1_flt(0u); a3xw0 = true;
    fcr31 = (a1f::lt(f1, f0)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
    if ((fcr31 & 0x800000u) == 0u) { f0 = a1f::mul(f1, f3); a3xw0 = true; goto L_13f266c; }
    f0 = (-(float)(f1)); a3xw0 = true;
    f0 = a1f::mul(f0, f3); a3xw0 = true;
L_13f266c:;
    f0 = a1f::addFinite(f2, f0); a3xw0 = true;
    A1_W32B(a0, 612, a1_bits(f0));
L_13f2674:;
    { if (xo) { if (a3xw0) xo[0] = a1_bits(f0); } return v0; }
    __builtin_unreachable();
}

// initWeaponSpriteStatus (0x13f95a0): typed by tools/typed_lift2.py (round A3; loop leaf)
inline a1::RetVF initWeaponSpriteStatus(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt)
{
    float f0 = 0.0f;
    float f1 = 0.0f;
    float f2 = 0.0f;
    uint32_t at = 0;
    uint32_t v0 = 0;
    uint32_t v1 = 0;
    uint32_t a0 = 0;
    v0 = 0x1200000u;
    f0 = a1_flt(0u);
    at = 0x3F000000u;
    f2 = a1_flt(at);
    v0 = v0 - 0x79D8u;
    at = 0x42800000u;
    f1 = a1_flt(at);
    a0 = 1u;
    v1 = 8u;
L_13f95c8:;
    v1 = v1 - 1u;
    A1_W32(v0, 0u);
    A1_W32B(v0, 8, a1_bits(f0));
    A1_W32B(v0, 4, a1_bits(f0));
    A1_W32B(v0, 12, a1_bits(f2));
    A1_W32B(v0, 16, a1_bits(f1));
    A1_W32B(v0, 20, a1_bits(f0));
    A1_W32B(v0, 24, a0);
    A1_W32B(v0, 28, a1_bits(f0));
    { const bool c = (int32_t)v1 >= 0; v0 = v0 + 0x20u; if (c) { A3_BACK(); goto L_13f95c8; } }
    v0 = 0xFFFFFFFFu;
    v1 = 0x1460000u;
    a0 = 0x1460000u;
    A1_W32B(v1, 27524, v0);
    { A1_W32B(a0, 27528, v0); return a1::RetVF{(uint64_t)a1_sx64(v0), f0}; }
    __builtin_unreachable();
}

// get_camera_collision_data_base_filter_radius (0x140f418): typed by tools/typed_lift2.py (round A3; leaf, extra outputs xo[0]=f13)
inline float get_camera_collision_data_base_filter_radius(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, float f12, float f13, float f14, uint64_t *xo = nullptr)
{
    float f0 = 0.0f;
    uint32_t at = 0;
    uint32_t fcr31 = 0;
    fcr31 = (a1f::lt(f13, f14)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
    if ((fcr31 & 0x800000u) != 0u) { f13 = ((float)(f14)); goto L_140f424; }
L_140f424:;
    at = 0x3DCC0000u;
    at = at | 0xCCCCu;
    f0 = a1_flt(at);
    f13 = a1f::add(f13, f0);
    fcr31 = (a1f::lt(f12, f13)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
    if ((fcr31 & 0x800000u) != 0u) { f12 = ((float)(f13)); goto L_140f440; }
L_140f440:;
    { f0 = ((float)(f12)); if (xo) { xo[0] = a1_bits(f13); } return f0; }
    __builtin_unreachable();
}

// is_cliff_rise_ok_camera_place_status (0x1413538): typed by tools/typed_lift2.py (round A3; leaf, extra outputs xo[0]=a0 xo[1]=a1)
inline uint64_t is_cliff_rise_ok_camera_place_status(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint64_t a0, uint64_t a1, uint64_t *xo = nullptr)
{
    uint64_t v0 = 0;
    uint64_t v1 = 0;
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)a1, 0))));
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 1))));
    (a1 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)v0, 252))));
    if ((int64_t)a1 != (int64_t)v1) goto L_14135b0;
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)a0, 208))));
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)v0, 8))));
    { const bool c = (int64_t)v1 != (int64_t)a1; (a0 = (uint64_t)(A1_R64B((uint32_t)v0, 16))); if (c) goto L_1413570; }
    (v0 = (uint64_t)(0ULL | (uint64_t)(uint16_t)61440));
    (v0 = (uint64_t)(RC_U64X(v0) << 30));
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 1))));
    (a0 = (uint64_t)(a0 & v0));
    goto L_1413574;
L_1413570:;
    (v1 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL));
L_1413574:;
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 1))));
    if ((int64_t)v1 != (int64_t)v0) goto L_14135a8;
    (v0 = (uint64_t)(0ULL | (uint64_t)(uint16_t)32768));
    (v0 = (uint64_t)(RC_U64X(v0) << 29));
    if ((int64_t)a0 == (int64_t)v0) goto L_14135a0;
    (v0 = (uint64_t)(0ULL | (uint64_t)(uint16_t)49152));
    (v0 = (uint64_t)(RC_U64X(v0) << 29));
    if ((int64_t)a0 != (int64_t)v0) goto L_14135a8;
L_14135a0:;
    { (v0 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL)); if (xo) { xo[0] = a0; xo[1] = a1; } return v0; }
L_14135a8:;
    { (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 1)))); if (xo) { xo[0] = a0; xo[1] = a1; } return v0; }
L_14135b0:;
    { (v0 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL)); if (xo) { xo[1] = a1; } return v0; }
    __builtin_unreachable();
}

// act_local_vs_ofst_camera_place_status (0x1413608): typed by tools/typed_lift2.py (round A3; leaf, extra outputs xo[0]=a1 xo[1]=a2)
inline uint64_t act_local_vs_ofst_camera_place_status(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint64_t a0, uint64_t a1, uint64_t *xo = nullptr)
{
    uint64_t v0 = 0;
    uint64_t v1 = 0;
    uint64_t a2 = 0;
    (a2 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)a1, 0))));
    (a1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 1))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 1))));
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)a2, 48))));
    { const bool c = (int64_t)v1 == (int64_t)a1; (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)a0, 208)))); if (c) goto L_1413660; }
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)a0, 8))));
    { const bool c = (int64_t)v0 != (int64_t)a1; (a0 = (uint64_t)(A1_R64B((uint32_t)a0, 16))); if (c) goto L_1413640; }
    (v0 = (uint64_t)(0ULL | (uint64_t)(uint16_t)61440));
    (v0 = (uint64_t)(RC_U64X(v0) << 30));
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 1))));
    (a0 = (uint64_t)(a0 & v0));
    goto L_1413644;
L_1413640:;
    (v1 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL));
L_1413644:;
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 1))));
    if ((int64_t)v1 != (int64_t)v0) { (v0 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL)); goto L_1413660; }
    (v1 = (uint64_t)(0ULL | (uint64_t)(uint16_t)40960));
    (v1 = (uint64_t)(RC_U64X(v1) << 29));
    if ((int64_t)a0 != (int64_t)v1) { (v0 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL)); goto L_1413660; }
L_1413660:;
    { if (xo) { xo[0] = a1; xo[1] = a2; } return v0; }
    __builtin_unreachable();
}

// gamecore_sub_014144F0 (0x14144f0): typed by tools/typed_lift2.py (round A3; leaf)
inline float gamecore_sub_014144F0(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, float f12, float f13, float f14)
{
    float f0 = 0.0f;
    float f1 = 0.0f;
    float f2 = 0.0f;
    uint32_t at = 0;
    uint32_t v0 = 0;
    uint32_t fcr31 = 0;
    f0 = a1_flt(0u);
    fcr31 = (a1f::eq(f14, f0)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
    { const bool c = (fcr31 & 0x800000u) != 0u; f0 = ((float)(f13)); if (c) goto L_1414574; }
    f0 = a1f::div(fcr31, f12, f14);
    at = 0x3F000000u;
    f1 = a1_flt(at);
    { int32_t tmp = a1f::cvtw(f0); std::memcpy(&f2, &tmp, sizeof(tmp)); }
    v0 = a1_bits(f2);
    f2 = a1_flt(v0);
    { int32_t tmp; std::memcpy(&tmp, &f2, sizeof(tmp)); f2 = ((float)(int32_t)(tmp)); }
    f0 = a1f::sub(f0, f2);
    fcr31 = (a1f::le(f0, f1)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
    { const bool c = (fcr31 & 0x800000u) == 0u; v0 = v0 + 1u; if (c) goto L_1414540; }
    f2 = a1f::mul(f14, f2);
    goto L_141454c;
L_1414540:;
    f0 = a1_flt(v0);
    { int32_t tmp; std::memcpy(&tmp, &f0, sizeof(tmp)); f0 = ((float)(int32_t)(tmp)); }
    f2 = a1f::mul(f14, f0);
L_141454c:;
    f0 = a1f::sub(f13, f2);
    f0 = a1f::div(fcr31, f0, f14);
    { int32_t tmp = a1f::cvtw(f0); std::memcpy(&f1, &tmp, sizeof(tmp)); }
    v0 = a1_bits(f1);
    f1 = a1_flt(v0);
    { int32_t tmp; std::memcpy(&tmp, &f1, sizeof(tmp)); f1 = ((float)(int32_t)(tmp)); }
    f1 = a1f::mul(f1, f14);
    f0 = a1f::add(f2, f1);
L_1414574:;
    { return f0; }
    __builtin_unreachable();
}

// gamecore_sub_01414E78 (0x1414e78): typed by tools/typed_lift2.py (round A3; leaf, extra outputs xo[0]=f0)
inline uint32_t gamecore_sub_01414E78(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t a0, uint64_t *xo = nullptr)
{
    float f0 = 0.0f;
    float f1 = 0.0f;
    uint32_t at = 0;
    uint32_t v0 = 0;
    uint32_t v1 = 0;
    uint32_t fcr31 = 0;
    bool a3xw0 = false;
    v0 = A1_R32(a0);
    v1 = 2u;
    { const bool c = v0 != v1; v0 = 0u; if (c) goto L_1414ea8; }
    f1 = a1_flt(A1_R32B(a0, 36));
    at = 0xBF860000u;
    at = at | 0xA91u;
    f0 = a1_flt(at); a3xw0 = true;
    fcr31 = (a1f::le(f1, f0)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
    { const bool c = (fcr31 & 0x800000u) != 0u; v0 = 1u; if (c) goto L_1414ea8; }
    v0 = 0u;
L_1414ea8:;
    { if (xo) { if (a3xw0) xo[0] = a1_bits(f0); } return v0; }
    __builtin_unreachable();
}

// gamecore_sub_0141B6C0 (0x141b6c0): typed by tools/typed_lift2.py (round A3; loop leaf)
inline uint64_t gamecore_sub_0141B6C0(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint64_t a0, uint64_t a3)
{
    uint64_t v0 = 0;
    uint64_t v1 = 0;
    uint64_t a1 = 0;
    uint64_t a2 = 0;
    uint64_t t0 = 0;
    (v0 = (uint64_t)(a3 | a0));
    (v0 = (uint64_t)(v0 & (uint64_t)(uint16_t)7));
    { const bool c = (int64_t)v0 == 0; (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)a3, 192)))); if (c) goto L_141b730; }
L_141b6d0:;
    { uint32_t addr = ADD32((uint32_t)a3, 7); uint32_t aligned_addr = addr & ~7u; uint32_t offset = addr & 7u; uint64_t mem = A1_R64(aligned_addr); uint32_t shift = (7u - offset) << 3; uint64_t keepMask = (shift == 0) ? 0ull : ((1ull << shift) - 1ull); (v1 = (uint64_t)((v1 & keepMask) | (mem << shift))); }
    { uint32_t addr = ADD32((uint32_t)a3, 0); uint32_t aligned_addr = addr & ~7u; uint32_t offset = addr & 7u; uint64_t mem = A1_R64(aligned_addr); uint32_t shift = offset << 3; uint64_t keepMask = (offset == 0) ? 0ull : (0xFFFFFFFFFFFFFFFFull << ((8u - offset) << 3)); (v1 = (uint64_t)((v1 & keepMask) | (mem >> shift))); }
    { uint32_t addr = ADD32((uint32_t)a3, 15); uint32_t aligned_addr = addr & ~7u; uint32_t offset = addr & 7u; uint64_t mem = A1_R64(aligned_addr); uint32_t shift = (7u - offset) << 3; uint64_t keepMask = (shift == 0) ? 0ull : ((1ull << shift) - 1ull); (a1 = (uint64_t)((a1 & keepMask) | (mem << shift))); }
    { uint32_t addr = ADD32((uint32_t)a3, 8); uint32_t aligned_addr = addr & ~7u; uint32_t offset = addr & 7u; uint64_t mem = A1_R64(aligned_addr); uint32_t shift = offset << 3; uint64_t keepMask = (offset == 0) ? 0ull : (0xFFFFFFFFFFFFFFFFull << ((8u - offset) << 3)); (a1 = (uint64_t)((a1 & keepMask) | (mem >> shift))); }
    { uint32_t addr = ADD32((uint32_t)a3, 23); uint32_t aligned_addr = addr & ~7u; uint32_t offset = addr & 7u; uint64_t mem = A1_R64(aligned_addr); uint32_t shift = (7u - offset) << 3; uint64_t keepMask = (shift == 0) ? 0ull : ((1ull << shift) - 1ull); (a2 = (uint64_t)((a2 & keepMask) | (mem << shift))); }
    { uint32_t addr = ADD32((uint32_t)a3, 16); uint32_t aligned_addr = addr & ~7u; uint32_t offset = addr & 7u; uint64_t mem = A1_R64(aligned_addr); uint32_t shift = offset << 3; uint64_t keepMask = (offset == 0) ? 0ull : (0xFFFFFFFFFFFFFFFFull << ((8u - offset) << 3)); (a2 = (uint64_t)((a2 & keepMask) | (mem >> shift))); }
    { uint32_t addr = ADD32((uint32_t)a3, 31); uint32_t aligned_addr = addr & ~7u; uint32_t offset = addr & 7u; uint64_t mem = A1_R64(aligned_addr); uint32_t shift = (7u - offset) << 3; uint64_t keepMask = (shift == 0) ? 0ull : ((1ull << shift) - 1ull); (t0 = (uint64_t)((t0 & keepMask) | (mem << shift))); }
    { uint32_t addr = ADD32((uint32_t)a3, 24); uint32_t aligned_addr = addr & ~7u; uint32_t offset = addr & 7u; uint64_t mem = A1_R64(aligned_addr); uint32_t shift = offset << 3; uint64_t keepMask = (offset == 0) ? 0ull : (0xFFFFFFFFFFFFFFFFull << ((8u - offset) << 3)); (t0 = (uint64_t)((t0 & keepMask) | (mem >> shift))); }
    { uint32_t addr = ADD32((uint32_t)a0, 7); uint32_t aligned_addr = addr & ~7u; uint32_t offset = addr & 7u; uint32_t shift = (7u - offset) << 3; uint64_t mask = 0xFFFFFFFFFFFFFFFFull >> shift; uint64_t old_data = A1_R64(aligned_addr); uint64_t val = v1; uint64_t new_data = (old_data & ~mask) | ((val >> shift) & mask); A1_W64(aligned_addr, new_data); }
    { uint32_t addr = ADD32((uint32_t)a0, 0); uint32_t aligned_addr = addr & ~7u; uint32_t offset = addr & 7u; uint32_t shift = offset << 3; uint64_t mask = 0xFFFFFFFFFFFFFFFFull << shift; uint64_t old_data = A1_R64(aligned_addr); uint64_t val = v1; uint64_t new_data = (old_data & ~mask) | ((val << shift) & mask); A1_W64(aligned_addr, new_data); }
    { uint32_t addr = ADD32((uint32_t)a0, 15); uint32_t aligned_addr = addr & ~7u; uint32_t offset = addr & 7u; uint32_t shift = (7u - offset) << 3; uint64_t mask = 0xFFFFFFFFFFFFFFFFull >> shift; uint64_t old_data = A1_R64(aligned_addr); uint64_t val = a1; uint64_t new_data = (old_data & ~mask) | ((val >> shift) & mask); A1_W64(aligned_addr, new_data); }
    { uint32_t addr = ADD32((uint32_t)a0, 8); uint32_t aligned_addr = addr & ~7u; uint32_t offset = addr & 7u; uint32_t shift = offset << 3; uint64_t mask = 0xFFFFFFFFFFFFFFFFull << shift; uint64_t old_data = A1_R64(aligned_addr); uint64_t val = a1; uint64_t new_data = (old_data & ~mask) | ((val << shift) & mask); A1_W64(aligned_addr, new_data); }
    { uint32_t addr = ADD32((uint32_t)a0, 23); uint32_t aligned_addr = addr & ~7u; uint32_t offset = addr & 7u; uint32_t shift = (7u - offset) << 3; uint64_t mask = 0xFFFFFFFFFFFFFFFFull >> shift; uint64_t old_data = A1_R64(aligned_addr); uint64_t val = a2; uint64_t new_data = (old_data & ~mask) | ((val >> shift) & mask); A1_W64(aligned_addr, new_data); }
    { uint32_t addr = ADD32((uint32_t)a0, 16); uint32_t aligned_addr = addr & ~7u; uint32_t offset = addr & 7u; uint32_t shift = offset << 3; uint64_t mask = 0xFFFFFFFFFFFFFFFFull << shift; uint64_t old_data = A1_R64(aligned_addr); uint64_t val = a2; uint64_t new_data = (old_data & ~mask) | ((val << shift) & mask); A1_W64(aligned_addr, new_data); }
    { uint32_t addr = ADD32((uint32_t)a0, 31); uint32_t aligned_addr = addr & ~7u; uint32_t offset = addr & 7u; uint32_t shift = (7u - offset) << 3; uint64_t mask = 0xFFFFFFFFFFFFFFFFull >> shift; uint64_t old_data = A1_R64(aligned_addr); uint64_t val = t0; uint64_t new_data = (old_data & ~mask) | ((val >> shift) & mask); A1_W64(aligned_addr, new_data); }
    { uint32_t addr = ADD32((uint32_t)a0, 24); uint32_t aligned_addr = addr & ~7u; uint32_t offset = addr & 7u; uint32_t shift = offset << 3; uint64_t mask = 0xFFFFFFFFFFFFFFFFull << shift; uint64_t old_data = A1_R64(aligned_addr); uint64_t val = t0; uint64_t new_data = (old_data & ~mask) | ((val << shift) & mask); A1_W64(aligned_addr, new_data); }
    (a3 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)a3, 32))));
    { const bool c = (int64_t)a3 != (int64_t)v0; (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)a0, 32)))); if (c) { A3_BACK(); goto L_141b6d0; } }
    goto L_141b764;
L_141b730:;
    (v1 = (uint64_t)(A1_R64B((uint32_t)a3, 0)));
    (a1 = (uint64_t)(A1_R64B((uint32_t)a3, 8)));
    (a2 = (uint64_t)(A1_R64B((uint32_t)a3, 16)));
    (t0 = (uint64_t)(A1_R64B((uint32_t)a3, 24)));
    A1_W64B((uint32_t)a0, 0, v1);
    A1_W64B((uint32_t)a0, 8, a1);
    A1_W64B((uint32_t)a0, 16, a2);
    A1_W64B((uint32_t)a0, 24, t0);
    (a3 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)a3, 32))));
    { const bool c = (int64_t)a3 != (int64_t)v0; (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)a0, 32)))); if (c) { A3_BACK(); goto L_141b730; } }
L_141b764:;
    { uint32_t addr = ADD32((uint32_t)a3, 7); uint32_t aligned_addr = addr & ~7u; uint32_t offset = addr & 7u; uint64_t mem = A1_R64(aligned_addr); uint32_t shift = (7u - offset) << 3; uint64_t keepMask = (shift == 0) ? 0ull : ((1ull << shift) - 1ull); (v0 = (uint64_t)((v0 & keepMask) | (mem << shift))); }
    { uint32_t addr = ADD32((uint32_t)a3, 0); uint32_t aligned_addr = addr & ~7u; uint32_t offset = addr & 7u; uint64_t mem = A1_R64(aligned_addr); uint32_t shift = offset << 3; uint64_t keepMask = (offset == 0) ? 0ull : (0xFFFFFFFFFFFFFFFFull << ((8u - offset) << 3)); (v0 = (uint64_t)((v0 & keepMask) | (mem >> shift))); }
    { uint32_t addr = ADD32((uint32_t)a3, 15); uint32_t aligned_addr = addr & ~7u; uint32_t offset = addr & 7u; uint64_t mem = A1_R64(aligned_addr); uint32_t shift = (7u - offset) << 3; uint64_t keepMask = (shift == 0) ? 0ull : ((1ull << shift) - 1ull); (v1 = (uint64_t)((v1 & keepMask) | (mem << shift))); }
    { uint32_t addr = ADD32((uint32_t)a3, 8); uint32_t aligned_addr = addr & ~7u; uint32_t offset = addr & 7u; uint64_t mem = A1_R64(aligned_addr); uint32_t shift = offset << 3; uint64_t keepMask = (offset == 0) ? 0ull : (0xFFFFFFFFFFFFFFFFull << ((8u - offset) << 3)); (v1 = (uint64_t)((v1 & keepMask) | (mem >> shift))); }
    { uint32_t addr = ADD32((uint32_t)a3, 23); uint32_t aligned_addr = addr & ~7u; uint32_t offset = addr & 7u; uint64_t mem = A1_R64(aligned_addr); uint32_t shift = (7u - offset) << 3; uint64_t keepMask = (shift == 0) ? 0ull : ((1ull << shift) - 1ull); (a1 = (uint64_t)((a1 & keepMask) | (mem << shift))); }
    { uint32_t addr = ADD32((uint32_t)a3, 16); uint32_t aligned_addr = addr & ~7u; uint32_t offset = addr & 7u; uint64_t mem = A1_R64(aligned_addr); uint32_t shift = offset << 3; uint64_t keepMask = (offset == 0) ? 0ull : (0xFFFFFFFFFFFFFFFFull << ((8u - offset) << 3)); (a1 = (uint64_t)((a1 & keepMask) | (mem >> shift))); }
    (a2 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)a3, 24))));
    { uint32_t addr = ADD32((uint32_t)a0, 7); uint32_t aligned_addr = addr & ~7u; uint32_t offset = addr & 7u; uint32_t shift = (7u - offset) << 3; uint64_t mask = 0xFFFFFFFFFFFFFFFFull >> shift; uint64_t old_data = A1_R64(aligned_addr); uint64_t val = v0; uint64_t new_data = (old_data & ~mask) | ((val >> shift) & mask); A1_W64(aligned_addr, new_data); }
    { uint32_t addr = ADD32((uint32_t)a0, 0); uint32_t aligned_addr = addr & ~7u; uint32_t offset = addr & 7u; uint32_t shift = offset << 3; uint64_t mask = 0xFFFFFFFFFFFFFFFFull << shift; uint64_t old_data = A1_R64(aligned_addr); uint64_t val = v0; uint64_t new_data = (old_data & ~mask) | ((val << shift) & mask); A1_W64(aligned_addr, new_data); }
    { uint32_t addr = ADD32((uint32_t)a0, 15); uint32_t aligned_addr = addr & ~7u; uint32_t offset = addr & 7u; uint32_t shift = (7u - offset) << 3; uint64_t mask = 0xFFFFFFFFFFFFFFFFull >> shift; uint64_t old_data = A1_R64(aligned_addr); uint64_t val = v1; uint64_t new_data = (old_data & ~mask) | ((val >> shift) & mask); A1_W64(aligned_addr, new_data); }
    { uint32_t addr = ADD32((uint32_t)a0, 8); uint32_t aligned_addr = addr & ~7u; uint32_t offset = addr & 7u; uint32_t shift = offset << 3; uint64_t mask = 0xFFFFFFFFFFFFFFFFull << shift; uint64_t old_data = A1_R64(aligned_addr); uint64_t val = v1; uint64_t new_data = (old_data & ~mask) | ((val << shift) & mask); A1_W64(aligned_addr, new_data); }
    { uint32_t addr = ADD32((uint32_t)a0, 23); uint32_t aligned_addr = addr & ~7u; uint32_t offset = addr & 7u; uint32_t shift = (7u - offset) << 3; uint64_t mask = 0xFFFFFFFFFFFFFFFFull >> shift; uint64_t old_data = A1_R64(aligned_addr); uint64_t val = a1; uint64_t new_data = (old_data & ~mask) | ((val >> shift) & mask); A1_W64(aligned_addr, new_data); }
    { uint32_t addr = ADD32((uint32_t)a0, 16); uint32_t aligned_addr = addr & ~7u; uint32_t offset = addr & 7u; uint32_t shift = offset << 3; uint64_t mask = 0xFFFFFFFFFFFFFFFFull << shift; uint64_t old_data = A1_R64(aligned_addr); uint64_t val = a1; uint64_t new_data = (old_data & ~mask) | ((val << shift) & mask); A1_W64(aligned_addr, new_data); }
    { A1_W32B((uint32_t)a0, 24, (uint32_t)a2); return v0; }
    __builtin_unreachable();
}

// gamecore_sub_0141B900 (0x141b900): typed by tools/typed_lift2.py (round A3; leaf, extra outputs xo[0]=f0)
inline uint32_t gamecore_sub_0141B900(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t a0, uint32_t a1, uint64_t *xo = nullptr)
{
    float f0 = 0.0f;
    float f1 = 0.0f;
    uint32_t v0 = 0;
    uint32_t v1 = 0;
    uint32_t fcr31 = 0;
    bool a3xw0 = false;
    v0 = A1_R32(a1);
    a1 = 2u;
    v1 = A1_R32B(v0, 4);
    if (v1 == a1) { v0 = 0u; goto L_141b970; }
    f0 = a1_flt(A1_R32B(a0, 8)); a3xw0 = true;
    f1 = a1_flt(0u);
    fcr31 = (a1f::lt(f0, f1)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
    if ((fcr31 & 0x800000u) != 0u) goto L_141b92c;
    fcr31 = (a1f::lt(f1, f0)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
L_141b92c:;
    if ((fcr31 & 0x800000u) != 0u) goto L_141b964;
    f0 = a1_flt(A1_R32B(a0, 16)); a3xw0 = true;
    f1 = a1_flt(0u);
    fcr31 = (a1f::lt(f0, f1)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
    if ((fcr31 & 0x800000u) == 0u) goto L_141b958;
    if ((fcr31 & 0x800000u) != 0u) goto L_141b964;
    { v0 = 0u; if (xo) { xo[0] = a1_bits(f0); } return v0; }
L_141b958:;
    fcr31 = (a1f::lt(f1, f0)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
    if ((fcr31 & 0x800000u) == 0u) { v0 = 0u; goto L_141b970; }
L_141b964:;
    { v0 = 1u; if (xo) { xo[0] = a1_bits(f0); } return v0; }
L_141b970:;
    { if (xo) { if (a3xw0) xo[0] = a1_bits(f0); } return v0; }
    __builtin_unreachable();
}

// gamecore_sub_0141BA40 (0x141ba40): typed by tools/typed_lift2.py (round A3; leaf, extra outputs xo[0]=f0)
inline uint32_t gamecore_sub_0141BA40(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t a0, float f12, uint64_t *xo = nullptr)
{
    float f0 = 0.0f;
    uint32_t v0 = 0;
    uint32_t fcr31 = 0;
    bool a3xw0 = false;
    v0 = A1_R32B(a0, 1380);
    { const bool c = v0 != 0; v0 = 0u; if (c) goto L_141ba60; }
    f0 = a1_flt(A1_R32B(a0, 216)); a3xw0 = true;
    fcr31 = (a1f::le(f12, f0)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
    { const bool c = (fcr31 & 0x800000u) != 0u; v0 = 1u; if (c) goto L_141ba60; }
    v0 = 0u;
L_141ba60:;
    { if (xo) { if (a3xw0) xo[0] = a1_bits(f0); } return v0; }
    __builtin_unreachable();
}

// gamecore_sub_0141BC78 (0x141bc78): typed by tools/typed_lift2.py (round A3; leaf, extra outputs xo[0]=f0)
inline uint32_t gamecore_sub_0141BC78(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t a0, uint32_t a1, float f12, float f13, uint64_t *xo = nullptr)
{
    float f0 = 0.0f;
    float f1 = 0.0f;
    uint32_t v0 = 0;
    uint32_t v1 = 0;
    uint64_t a2 = 0;
    uint32_t fcr31 = 0;
    bool a3xw0 = false;
    v0 = A1_R32B(a0, 28);
    (a2 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL));
    { const bool c = v0 == 0; A1_W32B(a0, 36, 0u); if (c) goto L_141bcc8; }
    f0 = a1_flt(0u); a3xw0 = true;
    fcr31 = (a1f::lt(f0, f12)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
    if ((fcr31 & 0x800000u) == 0u) goto L_141bcc8;
    f0 = a1_flt(A1_R32B(a1, 280)); a3xw0 = true;
    fcr31 = (a1f::le(f12, f0)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
    { const bool c = (fcr31 & 0x800000u) == 0u; v0 = 1u; if (c) goto L_141bcc8; }
    A1_W32B(a0, 36, v0);
    v1 = A1_R32B(a1, 6196);
    if (v1 != 0) goto L_141bcc8;
    f0 = a1_flt(A1_R32B(a0, 32)); a3xw0 = true;
    (a2 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 1))));
    f0 = a1f::add(f0, f13); a3xw0 = true;
    A1_W32B(a0, 32, a1_bits(f0));
L_141bcc8:;
    if ((int64_t)a2 != 0) goto L_141bcec;
    f0 = a1_flt(A1_R32B(a0, 32)); a3xw0 = true;
    f1 = a1_flt(0u);
    f0 = a1f::sub(f0, f13); a3xw0 = true;
    fcr31 = (a1f::lt(f0, f1)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
    { const bool c = (fcr31 & 0x800000u) == 0u; A1_W32B(a0, 32, a1_bits(f0)); if (c) goto L_141bcec; }
    A1_W32B(a0, 32, a1_bits(f1));
L_141bcec:;
    { if (xo) { if (a3xw0) xo[0] = a1_bits(f0); } return v0; }
    __builtin_unreachable();
}

// add_camera_mode_common_param_segment (0x14255a0): typed by tools/typed_lift2.py (round A3; loop leaf)
inline void add_camera_mode_common_param_segment(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint64_t a0)
{
    uint32_t v0 = 0;
    uint32_t v1 = 0;
    uint64_t a1 = 0;
    uint64_t a2 = 0;
    if ((int64_t)a0 == 0) goto L_14255e4;
    (v1 = (uint32_t)((int32_t)((int32_t)A1_R32B((uint32_t)a0, 12))));
    { const bool c = (int32_t)v1 <= 0; (v0 = (uint32_t)((int32_t)((int32_t)A1_R32B((uint32_t)a0, 16)))); if (c) goto L_14255e4; }
    (a1 = (uint64_t)((uint64_t)a1_sx64(v0) + (uint64_t)0ULL));
    (a2 = (uint64_t)((uint64_t)a1_sx64(v1) + (uint64_t)0ULL));
L_14255c0:;
    (v0 = (uint32_t)((int32_t)((int32_t)ADD32((uint32_t)a1, 16))));
    (v1 = (uint32_t)((int32_t)((int32_t)ADD32((uint32_t)a1, 148))));
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)a1, 160))));
    (a2 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)a2, 4294967295))));
    A1_W32B((uint32_t)a1, 20, v0);
    A1_W32B((uint32_t)a1, 152, v1);
    A1_W32B((uint32_t)a1, 164, (uint32_t)a0);
    { const bool c = (int64_t)a2 != 0; (a1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)a1, 404)))); if (c) { A3_BACK(); goto L_14255c0; } }
L_14255e4:;
    { return; }
    __builtin_unreachable();
}

// add_camera_mode_manager_segment (0x14255f0): typed by tools/typed_lift2.py (round A3; loop leaf)
inline void add_camera_mode_manager_segment(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint64_t a0)
{
    uint32_t v0 = 0;
    uint32_t v1 = 0;
    uint32_t a1 = 0;
    uint32_t a2 = 0;
    uint32_t a3 = 0;
    uint32_t t0 = 0;
    uint64_t t1 = 0;
    uint64_t t2 = 0;
    if ((int64_t)a0 == 0) goto L_1425654;
    (v1 = (uint32_t)((int32_t)((int32_t)A1_R32B((uint32_t)a0, 12))));
    { const bool c = (int32_t)v1 <= 0; (v0 = (uint32_t)((int32_t)((int32_t)A1_R32B((uint32_t)a0, 16)))); if (c) goto L_1425654; }
    (t1 = (uint64_t)((uint64_t)a1_sx64(v0) + (uint64_t)0ULL));
    (t2 = (uint64_t)((uint64_t)a1_sx64(v1) + (uint64_t)0ULL));
L_1425610:;
    (v0 = (uint32_t)((int32_t)((int32_t)ADD32((uint32_t)t1, 16))));
    (v1 = (uint32_t)((int32_t)((int32_t)ADD32((uint32_t)t1, 24))));
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)t1, 32))));
    (a1 = (uint32_t)((int32_t)((int32_t)ADD32((uint32_t)t1, 40))));
    (a2 = (uint32_t)((int32_t)((int32_t)ADD32((uint32_t)t1, 48))));
    (a3 = (uint32_t)((int32_t)((int32_t)ADD32((uint32_t)t1, 56))));
    (t0 = (uint32_t)((int32_t)((int32_t)ADD32((uint32_t)t1, 64))));
    (t2 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)t2, 4294967295))));
    A1_W32B((uint32_t)t1, 20, v0);
    A1_W32B((uint32_t)t1, 28, v1);
    A1_W32B((uint32_t)t1, 36, (uint32_t)a0);
    A1_W32B((uint32_t)t1, 44, a1);
    A1_W32B((uint32_t)t1, 52, a2);
    A1_W32B((uint32_t)t1, 60, a3);
    A1_W32B((uint32_t)t1, 68, t0);
    { const bool c = (int64_t)t2 != 0; (t1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)t1, 72)))); if (c) { A3_BACK(); goto L_1425610; } }
L_1425654:;
    { return; }
    __builtin_unreachable();
}

// is_base_default_dir_attr_change_game_camera_param_manager (0x142c1a0): typed by tools/typed_lift2.py (round A3; leaf, extra outputs xo[0]=a2)
inline uint32_t is_base_default_dir_attr_change_game_camera_param_manager(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t a0, uint64_t *xo = nullptr)
{
    uint32_t v0 = 0;
    uint64_t v1 = 0;
    uint32_t a1 = 0;
    uint32_t a2 = 0;
    bool a3xw0 = false;
    v0 = A1_R32(a0);
    { const bool c = v0 == 0; v0 = 0u; if (c) goto L_142c240; }
    v0 = a0 + 0x10u;
    { const bool c = v0 == 0; a2 = a0 + 0x5Cu; a3xw0 = true; if (c) goto L_142c230; }
    a0 = a0 + 0x6B0u;
    a1 = A1_R32B(a0, 140);
    v0 = A1_R32B(a2, 140);
    { const bool c = a1 != v0; (v1 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL)); if (c) goto L_142c234; }
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B(a0, 168))));
    v0 = A1_R32B(a2, 168);
    { const bool c = (int64_t)v1 != (int64_t)(int32_t)v0; (v1 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL)); if (c) goto L_142c234; }
    v0 = 1u;
    { const bool c = a1 == v0; (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 1)))); if (c) goto L_142c234; }
    a0 = A1_R32B(a0, 144);
    v0 = A1_R32B(a2, 144);
    if (a0 != v0) { (v1 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL)); goto L_142c234; }
    v0 = a0 - 0x64u;
    v0 = v0 < 3u ? 1u : 0u;
    if (v0 == 0) { (v1 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL)); goto L_142c234; }
    { const bool c = a1 == 0; v0 = 4u; if (c) goto L_142c234; }
    { const bool c = a1 == v0; v0 = 5u; if (c) goto L_142c234; }
    { const bool c = a1 == v0; v0 = 6u; if (c) goto L_142c234; }
    if (a1 != v0) { (v1 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL)); goto L_142c234; }
    goto L_142c234;
L_142c230:;
    (v1 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL));
L_142c234:;
    { const bool c = (int64_t)v1 == 0; v0 = 1u; if (c) goto L_142c240; }
    v0 = 0u;
L_142c240:;
    { if (xo) { if (a3xw0) xo[0] = a1_sx64(a2); } return v0; }
    __builtin_unreachable();
}

// gamecore_sub_0142C7A0 (0x142c7a0): typed by tools/typed_lift2.py (round A3; loop leaf, extra outputs xo[0]=f0)
inline uint32_t gamecore_sub_0142C7A0(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t a0, uint32_t a1, uint32_t a2, uint64_t *xo = nullptr)
{
    float f0 = 0.0f;
    float f1 = 0.0f;
    uint32_t at = 0;
    uint32_t v0 = 0;
    uint64_t v1 = 0;
    uint64_t a3 = 0;
    uint32_t fcr31 = 0;
    bool a3xw0 = false;
    v0 = A1_R32(a2);
    (a3 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL));
    at = 0xBF800000u;
    f1 = a1_flt(at);
    { const bool c = (int32_t)v0 <= 0; (v1 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL)); if (c) goto L_142c7e4; }
    a2 = A1_R32B(a2, 4);
L_142c7c0:;
    f0 = a1_flt(A1_R32(a2)); a3xw0 = true;
    fcr31 = (a1f::lt(f1, f0)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
    { const bool c = (fcr31 & 0x800000u) == 0u; v0 = v0 - 1u; if (c) goto L_142c7dc; }
    (a3 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B(a2, 4))));
    f1 = ((float)(f0));
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B(a2, 8))));
L_142c7dc:;
    { const bool c = v0 != 0; a2 = a2 + 0x65Cu; if (c) { A3_BACK(); goto L_142c7c0; } }
L_142c7e4:;
    A1_W32B(a0, 0, (uint32_t)a3);
    { A1_W32B(a1, 0, (uint32_t)v1); if (xo) { if (a3xw0) xo[0] = a1_bits(f0); } return v0; }
    __builtin_unreachable();
}

// gamecore_sub_01430248 (0x1430248): typed by tools/typed_lift2.py (round A3; loop leaf, extra outputs xo[0]=f0 xo[1]=f12 xo[2]=a3 xo[3]=t2 xo[4]=t3)
inline uint32_t gamecore_sub_01430248(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t a0, uint32_t a1, uint32_t a2, uint64_t a3, uint64_t t0, uint32_t t1, float f12, uint64_t *xo = nullptr)
{
    float f0 = 0.0f;
    uint32_t v0 = 0;
    uint32_t v1 = 0;
    uint32_t t2 = 0;
    uint64_t t3 = 0;
    uint64_t t4 = 0;
    uint64_t t5 = 0;
    uint32_t t6 = 0;
    uint32_t fcr31 = 0;
    bool a3xw0 = false;
    bool a3xw1 = false;
    bool a3xw2 = false;
    (v1 = (uint32_t)((int32_t)((int32_t)A1_R32B(a0, 15440))));
    v0 = 0x1200000u;
    t6 = v0 - 0x4338u;
    (t3 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL));
    v0 = v1 << 2;
    a0 = t6 + 0x5400u;
    v0 = v0 + v1;
    v0 = v0 << 2;
    v0 = v0 - v1;
    v0 = v0 << 3;
    { const bool c = (int32_t)a3 < 0; v1 = v0 + a0; if (c) goto L_14302d0; }
    { const bool c = (int32_t)t0 < 0; t2 = 0xFFFFFFFFu; if (c) goto L_14302d4; }
    a0 = A1_R32B(v1, 144);
    (v0 = (uint32_t)(((int64_t)(int64_t)RC_U64X(t3) < (int64_t)(int64_t)a1_sx64(a0)) ? 1 : 0));
    { const bool c = v0 == 0; (t4 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL)); if (c) goto L_14302d4; }
    (t5 = (uint64_t)((uint64_t)a1_sx64(a0) + (uint64_t)0ULL));
    a0 = v1;
    v1 = A1_R32(a0);
L_14302a0:;
    v0 = A1_R32B(v1, 28);
    { const bool c = (int64_t)t0 == (int64_t)(int32_t)v0; (t2 = (uint32_t)((int32_t)((int32_t)ADD32((uint32_t)t3, (uint32_t)a3)))); if (c) goto L_14302d4; }
    v0 = A1_R32B(a0, 4);
    { const bool c = v0 == 0; a0 = a0 + 0x24u; if (c) goto L_14302c0; }
    v0 = A1_R32(v0);
    (t3 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)t3, v0))));
L_14302c0:;
    (t4 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)t4, 1))));
    (v0 = (uint32_t)(((int64_t)(int64_t)RC_U64X(t4) < (int64_t)(int64_t)t5) ? 1 : 0));
    if (v0 != 0) { v1 = A1_R32(a0); { A3_BACK(); goto L_14302a0; } }
L_14302d0:;
    t2 = 0xFFFFFFFFu;
L_14302d4:;
    a0 = 0xFFFFFFFFu;
    { const bool c = t2 == a0; v0 = t2 << 1; if (c) goto L_14303a0; }
    v0 = v0 + t2;
    v0 = v0 << 3;
    (a3 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(v0, t6)))); a3xw2 = true;
    (v1 = (uint32_t)((int32_t)((int16_t)A1_R16B((uint32_t)a3, 10))));
    if (v1 == 0) goto L_14303a0;
    (v0 = (uint32_t)((int32_t)((int16_t)A1_R16B((uint32_t)a3, 6))));
    { const bool c = v0 != a0; v0 = a2 << 1; if (c) goto L_1430368; }
    A1_W16B((uint32_t)a3, 6, (uint16_t)a2);
    v0 = v0 + a2;
    v0 = v0 << 3;
    v0 = v0 + t6;
    f0 = a1_flt(A1_R32B(v0, 12)); a3xw0 = true;
    A1_W16B((uint32_t)a3, 8, (uint16_t)t1);
    f0 = a1f::add(f0, f12); a3xw0 = true;
    { float f = f0; uint32_t bits; std::memcpy(&bits, &f, sizeof(bits)); A1_W32B((uint32_t)a3, 12, bits); }
    v1 = A1_R32(a1);
    if (v1 == 0) { A1_W32B(a1, 0, (uint32_t)a3); goto L_1430348; }
    A1_W32B(v1, 16, (uint32_t)a3);
    A1_W32B(a1, 0, (uint32_t)a3);
    A1_W32B((uint32_t)a3, 16, 0u);
    v0 = A1_R32(a1);
    A1_W32B(v0, 20, v1);
    goto L_1430354;
L_1430348:;
    A1_W32B((uint32_t)a3, 16, 0u);
    v0 = A1_R32(a1);
    A1_W32B(v0, 20, 0u);
L_1430354:;
    v0 = A1_R32B(a1, 4);
    v0 = v0 + 1u;
    { A1_W32B(a1, 4, v0); if (xo) { xo[0] = a1_bits(f0); xo[2] = a3; xo[3] = a1_sx64(t2); xo[4] = t3; } return v0; }
L_1430368:;
    v0 = v0 + a2;
    v0 = v0 << 3;
    v0 = v0 + t6;
    f0 = a1_flt(A1_R32B(v0, 12)); a3xw0 = true;
    v1 = (uint32_t)(int16_t)A1_R16B(v0, 6);
    { const bool c = v1 == t2; f12 = a1f::add(f0, f12); a3xw1 = true; if (c) goto L_14303a0; }
    { uint32_t bits = A1_R32B((uint32_t)a3, 12); float f; std::memcpy(&f, &bits, sizeof(f)); f0 = f; } a3xw0 = true;
    fcr31 = (a1f::lt(f12, f0)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
    if ((fcr31 & 0x800000u) == 0u) goto L_14303a0;
    A1_W16B((uint32_t)a3, 8, (uint16_t)t1);
    A1_W16B((uint32_t)a3, 6, (uint16_t)a2);
    { float f = f12; uint32_t bits; std::memcpy(&bits, &f, sizeof(bits)); A1_W32B((uint32_t)a3, 12, bits); }
L_14303a0:;
    { if (xo) { if (a3xw0) xo[0] = a1_bits(f0); if (a3xw1) xo[1] = a1_bits(f12); if (a3xw2) xo[2] = a3; xo[3] = a1_sx64(t2); xo[4] = t3; } return v0; }
    __builtin_unreachable();
}

// getPathDataAddr (0x1433340): typed by tools/typed_lift2.py (round A3; loop leaf, extra outputs xo[0]=a2 xo[1]=a3 xo[2]=t0)
inline uint64_t getPathDataAddr(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t a0, uint64_t a1, uint64_t *xo = nullptr)
{
    uint32_t at = 0;
    uint64_t v0 = 0;
    uint32_t v1 = 0;
    uint64_t a2 = 0;
    uint64_t a3 = 0;
    uint64_t t0 = 0;
    bool a3xw0 = false;
    bool a3xw2 = false;
    (v1 = (uint32_t)((int32_t)((int32_t)A1_R32B(a0, 15440))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)SLL32(v1, 2))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v0, v1))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)SLL32((uint32_t)v0, 2))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)SUB32((uint32_t)v0, v1))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)SLL32((uint32_t)v0, 3))));
    at = 0x1200000u;
    (at = (uint32_t)((int32_t)((int32_t)ADD32(at, 4296))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(at, (uint32_t)v0))));
    (v1 = (uint32_t)((int32_t)((int32_t)A1_R32B((uint32_t)v0, 144))));
    { const bool c = (int32_t)v1 <= 0; (a3 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL)); if (c) goto L_14333a4; }
    (t0 = (uint64_t)((uint64_t)a1_sx64(v1) + (uint64_t)0ULL)); a3xw2 = true;
    (a2 = (uint64_t)((uint64_t)RC_U64X(v0) + (uint64_t)0ULL)); a3xw0 = true;
    (a0 = (uint32_t)((int32_t)((int32_t)A1_R32B((uint32_t)a2, 0))));
L_1433380:;
    (v0 = (uint64_t)((uint64_t)a2 + (uint64_t)0ULL));
    v1 = A1_R32B(a0, 28);
    { const bool c = (int64_t)a1 == (int64_t)(int32_t)v1; (a2 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)a2, 36)))); a3xw0 = true; if (c) goto L_14333a8; }
    (a3 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)a3, 1))));
    (v0 = (uint64_t)(((int64_t)(int64_t)RC_U64X(a3) < (int64_t)(int64_t)t0) ? 1 : 0));
    if ((int64_t)v0 != 0) { (a0 = (uint32_t)((int32_t)((int32_t)A1_R32B((uint32_t)a2, 0)))); { A3_BACK(); goto L_1433380; } }
L_14333a4:;
    (v0 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL));
L_14333a8:;
    { if (xo) { if (a3xw0) xo[0] = a2; xo[1] = a3; if (a3xw2) xo[2] = t0; } return v0; }
    __builtin_unreachable();
}

// gamecore_sub_01434610 (0x1434610): typed by tools/typed_lift2.py (round A3; loop leaf, extra outputs xo[0]=a3 xo[1]=t0 xo[2]=t1 xo[3]=t2)
inline uint32_t gamecore_sub_01434610(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t a0, uint64_t a1, uint64_t a2, uint64_t *xo = nullptr)
{
    uint32_t at = 0;
    uint32_t v0 = 0;
    uint32_t v1 = 0;
    uint64_t a3 = 0;
    uint64_t t0 = 0;
    uint64_t t1 = 0;
    uint64_t t2 = 0;
    bool a3xw0 = false;
    bool a3xw1 = false;
    bool a3xw3 = false;
    (v1 = (uint32_t)((int32_t)((int32_t)A1_R32B(a0, 15440))));
    v0 = v1 << 2;
    v0 = v0 + v1;
    v0 = v0 << 2;
    v0 = v0 - v1;
    v0 = v0 << 3;
    at = 0x1200000u;
    at = at + 0x10C8u;
    v0 = at + v0;
    { const bool c = (int32_t)a1 < 0; (t1 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL)); if (c) goto L_1434690; }
    if ((int32_t)a2 < 0) { v0 = 0xFFFFFFFFu; goto L_1434694; }
    v1 = A1_R32B(v0, 144);
    { const bool c = (int32_t)v1 <= 0; (t0 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL)); a3xw1 = true; if (c) goto L_1434690; }
    (t2 = (uint64_t)((uint64_t)a1_sx64(v1) + (uint64_t)0ULL)); a3xw3 = true;
    (a3 = (uint64_t)((uint64_t)a1_sx64(v0) + (uint64_t)0ULL)); a3xw0 = true;
    (a0 = (uint32_t)((int32_t)((int32_t)A1_R32B((uint32_t)a3, 0))));
L_1434660:;
    v1 = A1_R32B(a0, 28);
    { const bool c = (int64_t)a2 == (int64_t)(int32_t)v1; (v0 = (uint32_t)((int32_t)((int32_t)ADD32((uint32_t)t1, (uint32_t)a1)))); if (c) goto L_1434694; }
    (v0 = (uint32_t)((int32_t)((int32_t)A1_R32B((uint32_t)a3, 4))));
    { const bool c = v0 == 0; (a3 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)a3, 36)))); a3xw0 = true; if (c) goto L_1434680; }
    v0 = A1_R32(v0);
    (t1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)t1, v0))));
L_1434680:;
    (t0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)t0, 1)))); a3xw1 = true;
    (v0 = (uint32_t)(((int64_t)(int64_t)RC_U64X(t0) < (int64_t)(int64_t)t2) ? 1 : 0));
    if (v0 != 0) { (a0 = (uint32_t)((int32_t)((int32_t)A1_R32B((uint32_t)a3, 0)))); { A3_BACK(); goto L_1434660; } }
L_1434690:;
    v0 = 0xFFFFFFFFu;
L_1434694:;
    { if (xo) { if (a3xw0) xo[0] = a3; if (a3xw1) xo[1] = t0; xo[2] = t1; if (a3xw3) xo[3] = t2; } return v0; }
    __builtin_unreachable();
}

// calcDownSpeedRatio (0x1437f28): typed by tools/typed_lift2.py (round A3; leaf, extra outputs xo[0]=f12 xo[1]=f13)
inline float calcDownSpeedRatio(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, float f12, float f13, float f14, uint64_t *xo = nullptr)
{
    float f0 = 0.0f;
    float f1 = 0.0f;
    float f2 = 0.0f;
    uint32_t at = 0;
    uint32_t fcr31 = 0;
    at = 0x3BA30000u;
    at = at | 0xD70Au;
    f1 = a1_flt(at);
    f2 = ((float)(f13));
    f0 = a1_flt(0u);
    f12 = a1f::sub(f12, f1);
    fcr31 = (a1f::lt(f0, f12)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
    { const bool c = (fcr31 & 0x800000u) == 0u; f13 = ((float)(f0)); if (c) goto L_1437f84; }
    f0 = (-(float)(f2));
    at = 0x3F800000u;
    f1 = a1_flt(at);
    f0 = a1f::mul(f0, f14);
    f13 = a1f::div(fcr31, f12, f0);
    fcr31 = (a1f::le(f1, f13)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
    if ((fcr31 & 0x800000u) != 0u) { f13 = ((float)(f1)); goto L_1437f84; }
    at = 0x3F660000u;
    at = at | 0x6666u;
    f0 = a1_flt(at);
    f13 = a1f::mul(f13, f0);
L_1437f84:;
    { f0 = ((float)(f13)); if (xo) { xo[0] = a1_bits(f12); xo[1] = a1_bits(f13); } return f0; }
    __builtin_unreachable();
}

// gamecore_sub_0143D6F8 (0x143d6f8): typed by tools/typed_lift2.py (round A3; leaf, extra outputs xo[0]=a1 xo[1]=a2)
inline uint32_t gamecore_sub_0143D6F8(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t a0, uint64_t *xo = nullptr)
{
    uint32_t v0 = 0;
    uint32_t v1 = 0;
    uint32_t a1 = 0;
    uint32_t a2 = 0;
    a1 = A1_R32B(a0, 28);
    a2 = A1_R32B(a0, 204);
    v0 = A1_R32(a1);
    v1 = A1_R32B(a2, 188);
    v0 = v0 ^ v1;
    { v0 = v0 < 1u ? 1u : 0u; if (xo) { xo[0] = a1_sx64(a1); xo[1] = a1_sx64(a2); } return v0; }
    __builtin_unreachable();
}

// setRunStopTurnAngleFastLegCtrlWork (0x1440f48): typed by tools/typed_lift2.py (round A3; leaf, extra outputs xo[0]=f12)
inline float setRunStopTurnAngleFastLegCtrlWork(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t a0, float f12, uint64_t *xo = nullptr)
{
    float f0 = 0.0f;
    uint32_t at = 0;
    at = 0x3C8E0000u;
    at = at | 0xFA35u;
    f0 = a1_flt(at);
    f12 = a1f::mul(f12, f0);
    { A1_W32B(a0, 76, a1_bits(f12)); if (xo) { xo[0] = a1_bits(f12); } return f0; }
    __builtin_unreachable();
}

// gamecore_sub_01441020 (0x1441020): typed by tools/typed_lift2.py (round A3; leaf, extra outputs xo[0]=a1)
inline uint32_t gamecore_sub_01441020(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t a0, uint64_t *xo = nullptr)
{
    uint32_t v0 = 0;
    uint32_t v1 = 0;
    uint32_t a1 = 0;
    v1 = A1_R32B(a0, 28);
    a0 = A1_R32B(a0, 204);
    a1 = A1_R32(v1);
    v0 = A1_R32B(a0, 160);
    { const bool c = v0 == a1; v0 = 1u; if (c) goto L_1441078; }
    v0 = A1_R32B(a0, 164);
    { const bool c = v0 == a1; v0 = 1u; if (c) goto L_1441078; }
    v0 = A1_R32B(a0, 172);
    { const bool c = v0 == a1; v0 = 1u; if (c) goto L_1441078; }
    v0 = A1_R32B(a0, 176);
    { const bool c = v0 == a1; v0 = 1u; if (c) goto L_1441078; }
    v0 = A1_R32B(a0, 180);
    { const bool c = v0 == a1; v0 = 1u; if (c) goto L_1441078; }
    v1 = A1_R32B(a0, 184);
    { const bool c = v1 != a1; v0 = 0u; if (c) goto L_1441078; }
    v0 = 1u;
L_1441078:;
    { if (xo) { xo[0] = a1_sx64(a1); } return v0; }
    __builtin_unreachable();
}

// gamecore_sub_01441210 (0x1441210): typed by tools/typed_lift2.py (round A3; leaf, extra outputs xo[0]=a2)
inline uint32_t gamecore_sub_01441210(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t a0, uint64_t a1, uint64_t *xo = nullptr)
{
    float f0 = 0.0f;
    float f1 = 0.0f;
    float f2 = 0.0f;
    float f3 = 0.0f;
    uint32_t at = 0;
    uint32_t v0 = 0;
    uint32_t v1 = 0;
    uint32_t a2 = 0;
    uint32_t fcr31 = 0;
    a2 = A1_R32B(a0, 204);
    v0 = A1_R32B(a2, 52);
    { const bool c = v0 == 0; v0 = 0u; if (c) goto L_14412f4; }
    v0 = A1_R32B(a0, 196);
    if (v0 == 0) { v0 = A1_R32B(a0, 200); goto L_1441254; }
    f0 = a1_flt(A1_R32B(a2, 120));
    at = 0x3F000000u;
    f2 = a1_flt(at);
    f1 = a1_flt(A1_R32B(a0, 304));
    f0 = a1f::mul(f0, f2);
    f1 = std::fabs((float)(f1));
    fcr31 = (a1f::lt(f0, f1)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
    { const bool c = (fcr31 & 0x800000u) != 0u; v0 = 0u; if (c) goto L_14412f4; }
    v0 = A1_R32B(a0, 200);
L_1441254:;
    if (v0 == 0) goto L_14412a8;
    f0 = a1_flt(A1_R32B(a0, 316));
    f1 = a1_flt(A1_R32B(a2, 196));
    at = 0x3F800000u;
    f2 = a1_flt(at);
    f0 = a1f::div(fcr31, f0, f1);
    fcr31 = (a1f::lt(f0, f2)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
    if ((fcr31 & 0x800000u) == 0u) goto L_14412a8;
    f0 = a1_flt(A1_R32B(a2, 120));
    at = 0x3F000000u;
    f2 = a1_flt(at);
    f1 = a1_flt(A1_R32B(a0, 308));
    f0 = a1f::mul(f0, f2);
    f1 = std::fabs((float)(f1));
    fcr31 = (a1f::lt(f0, f1)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
    { const bool c = (fcr31 & 0x800000u) != 0u; v0 = 0u; if (c) goto L_14412f4; }
L_14412a8:;
    { const bool c = (int64_t)a1 == 0; v0 = 1u; if (c) goto L_14412f4; }
    v1 = A1_R32B(a2, 48);
    if (v1 == 0) goto L_14412f4;
    f2 = a1_flt(A1_R32B(a0, 304));
    f3 = a1_flt(A1_R32B(a2, 120));
    f2 = std::fabs((float)(f2));
    f0 = a1_flt(A1_R32B(a0, 316));
    f1 = a1_flt(A1_R32B(a2, 196));
    f0 = a1f::div(fcr31, f0, f1);
    f2 = a1f::div(fcr31, f2, f3);
    fcr31 = (a1f::lt(f2, f0)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
    if ((fcr31 & 0x800000u) == 0u) { v0 = 0u; goto L_14412f4; }
L_14412f4:;
    { if (xo) { xo[0] = a1_sx64(a2); } return v0; }
    __builtin_unreachable();
}

// gamecore_sub_01444370 (0x1444370): typed by tools/typed_lift2.py (round A3; leaf, extra outputs xo[0]=f0 xo[1]=a2)
inline uint32_t gamecore_sub_01444370(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t a0, uint64_t a1, uint64_t *xo = nullptr)
{
    float f0 = 0.0f;
    float f1 = 0.0f;
    float f2 = 0.0f;
    uint32_t at = 0;
    uint32_t v0 = 0;
    uint32_t v1 = 0;
    uint32_t a2 = 0;
    uint32_t fcr31 = 0;
    bool a3xw0 = false;
    a2 = A1_R32B(a0, 204);
    v0 = A1_R32B(a2, 52);
    { const bool c = v0 == 0; v0 = 0u; if (c) goto L_14444bc; }
    v0 = A1_R32B(a0, 196);
    if (v0 == 0) { v0 = A1_R32B(a0, 200); goto L_14443b4; }
    f0 = a1_flt(A1_R32B(a2, 100)); a3xw0 = true;
    at = 0x3F000000u;
    f2 = a1_flt(at);
    f1 = a1_flt(A1_R32B(a0, 304));
    f0 = a1f::mul(f0, f2); a3xw0 = true;
    f1 = std::fabs((float)(f1));
    fcr31 = (a1f::lt(f0, f1)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
    { const bool c = (fcr31 & 0x800000u) != 0u; v0 = 0u; if (c) goto L_14444bc; }
    v0 = A1_R32B(a0, 200);
L_14443b4:;
    if (v0 == 0) goto L_1444408;
    f0 = a1_flt(A1_R32B(a0, 316)); a3xw0 = true;
    f1 = a1_flt(A1_R32B(a2, 136));
    at = 0x3F800000u;
    f2 = a1_flt(at);
    f0 = a1f::div(fcr31, f0, f1); a3xw0 = true;
    fcr31 = (a1f::lt(f0, f2)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
    if ((fcr31 & 0x800000u) == 0u) goto L_1444408;
    f0 = a1_flt(A1_R32B(a2, 100)); a3xw0 = true;
    at = 0x3F000000u;
    f2 = a1_flt(at);
    f1 = a1_flt(A1_R32B(a0, 308));
    f0 = a1f::mul(f0, f2); a3xw0 = true;
    f1 = std::fabs((float)(f1));
    fcr31 = (a1f::lt(f0, f1)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
    { const bool c = (fcr31 & 0x800000u) != 0u; v0 = 0u; if (c) goto L_14444bc; }
L_1444408:;
    if ((int64_t)a1 != 0) { v1 = A1_R32B(a2, 48); goto L_1444468; }
    f0 = a1_flt(A1_R32B(a0, 316)); a3xw0 = true;
    f1 = a1_flt(A1_R32B(a2, 136));
    at = 0x3F800000u;
    f2 = a1_flt(at);
    f0 = a1f::div(fcr31, f0, f1); a3xw0 = true;
    fcr31 = (a1f::lt(f0, f2)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
    if ((fcr31 & 0x800000u) == 0u) goto L_144445c;
    f0 = a1_flt(A1_R32B(a2, 100)); a3xw0 = true;
    f1 = a1_flt(A1_R32B(a0, 308));
    at = 0x3F000000u;
    f2 = a1_flt(at);
    f1 = std::fabs((float)(f1));
    f0 = a1f::mul(f0, f2); a3xw0 = true;
    fcr31 = (a1f::lt(f0, f1)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
    { const bool c = (fcr31 & 0x800000u) != 0u; v0 = 0u; if (c) goto L_14444bc; }
L_144445c:;
    { v0 = 1u; if (xo) { xo[0] = a1_bits(f0); xo[1] = a1_sx64(a2); } return v0; }
L_1444468:;
    { const bool c = v1 == 0; v0 = 1u; if (c) goto L_14444bc; }
    f0 = a1_flt(A1_R32B(a0, 316)); a3xw0 = true;
    at = 0x41100000u;
    f1 = a1_flt(at);
    f2 = a1_flt(A1_R32B(a2, 136));
    fcr31 = (a1f::lt(f1, f0)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
    f2 = a1f::div(fcr31, f0, f2);
    if ((fcr31 & 0x800000u) == 0u) { v0 = 0u; goto L_14444bc; }
    f0 = a1_flt(A1_R32B(a0, 304)); a3xw0 = true;
    f1 = a1_flt(A1_R32B(a2, 100));
    f0 = std::fabs((float)(f0)); a3xw0 = true;
    f0 = a1f::div(fcr31, f0, f1); a3xw0 = true;
    fcr31 = (a1f::lt(f0, f2)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
    if ((fcr31 & 0x800000u) == 0u) { v0 = 0u; goto L_14444bc; }
L_14444bc:;
    { if (xo) { if (a3xw0) xo[0] = a1_bits(f0); xo[1] = a1_sx64(a2); } return v0; }
    __builtin_unreachable();
}

// adjustAnglePI (0x1446f80): typed by tools/typed_lift2.py (round A3; leaf, extra outputs xo[0]=f12)
inline float adjustAnglePI(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, float f12, uint64_t *xo = nullptr)
{
    float f0 = 0.0f;
    float f1 = 0.0f;
    uint32_t at = 0;
    uint32_t fcr31 = 0;
    at = 0x40C90000u;
    at = at | 0xFDAu;
    f1 = a1_flt(at);
    fcr31 = (a1f::lt(f1, f12)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
    if ((fcr31 & 0x800000u) == 0u) goto L_1446fa0;
    f12 = a1f::sub(f12, f1);
    goto L_1446fb8;
L_1446fa0:;
    at = 0xC0C90000u;
    at = at | 0xFDAu;
    f0 = a1_flt(at);
    fcr31 = (a1f::lt(f12, f0)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
    if ((fcr31 & 0x800000u) != 0u) { f12 = a1f::add(f12, f1); goto L_1446fb8; }
L_1446fb8:;
    at = 0x40490000u;
    at = at | 0xFDAu;
    f0 = a1_flt(at);
    fcr31 = (a1f::lt(f0, f12)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
    if ((fcr31 & 0x800000u) == 0u) goto L_1446fe8;
    at = 0xC0C90000u;
    at = at | 0xFDAu;
    f0 = a1_flt(at);
    f12 = a1f::add(f12, f0);
    goto L_1447010;
L_1446fe8:;
    at = 0xC0490000u;
    at = at | 0xFDAu;
    f0 = a1_flt(at);
    fcr31 = (a1f::lt(f12, f0)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
    if ((fcr31 & 0x800000u) == 0u) goto L_1447010;
    at = 0x40C90000u;
    at = at | 0xFDAu;
    f0 = a1_flt(at);
    f12 = a1f::add(f12, f0);
L_1447010:;
    { f0 = ((float)(f12)); if (xo) { xo[0] = a1_bits(f12); } return f0; }
    __builtin_unreachable();
}

// initBeardIKWork (0x145f0e8): typed by tools/typed_lift2.py (round A3; loop leaf)
inline uint32_t initBeardIKWork(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t a2)
{
    uint32_t v0 = 0;
    a2 = a2 + 0x3B0u;
    v0 = 9u;
L_145f0f0:;
    v0 = v0 - 1u;
    A1_W32(a2, 0u);
    { const bool c = (int32_t)v0 >= 0; a2 = a2 - 0x60u; if (c) { A3_BACK(); goto L_145f0f0; } }
    { return v0; }
    __builtin_unreachable();
}

// setRatioGazeIKWork (0x1461758): typed by tools/typed_lift2.py (round A3; leaf, extra outputs xo[0]=f0)
inline uint32_t setRatioGazeIKWork(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t a0, float f12, float f13, float f14, uint64_t *xo = nullptr)
{
    float f0 = 0.0f;
    uint32_t at = 0;
    uint32_t v0 = 0;
    uint32_t fcr31 = 0;
    bool a3xw0 = false;
    v0 = A1_R32B(a0, 4364);
    if (v0 == 0) goto L_14617ac;
    f0 = a1_flt(A1_R32B(v0, 52)); a3xw0 = true;
    fcr31 = (a1f::eq(f0, f12)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
    if ((fcr31 & 0x800000u) == 0u) goto L_1461794;
    f0 = a1_flt(A1_R32B(v0, 56)); a3xw0 = true;
    fcr31 = (a1f::eq(f0, f13)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
    if ((fcr31 & 0x800000u) == 0u) goto L_1461794;
    f0 = a1_flt(A1_R32B(v0, 60)); a3xw0 = true;
    fcr31 = (a1f::eq(f0, f14)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
    if ((fcr31 & 0x800000u) != 0u) { A1_W32B(v0, 60, a1_bits(f14)); goto L_14617a4; }
L_1461794:;
    at = 0x3F800000u;
    f0 = a1_flt(at); a3xw0 = true;
    A1_W32B(v0, 112, a1_bits(f0));
    A1_W32B(v0, 60, a1_bits(f14));
L_14617a4:;
    A1_W32B(v0, 52, a1_bits(f12));
    A1_W32B(v0, 56, a1_bits(f13));
L_14617ac:;
    { if (xo) { if (a3xw0) xo[0] = a1_bits(f0); } return v0; }
    __builtin_unreachable();
}

// setWormIKNodeLimit (0x14647f0): typed by tools/typed_lift2.py (round A3; loop leaf)
inline uint32_t setWormIKNodeLimit(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t a0, float f12)
{
    uint32_t v0 = 0;
    a0 = a0 + 0x9F0u;
    v0 = 0x1Fu;
L_14647f8:;
    v0 = v0 - 1u;
    A1_W32(a0, a1_bits(f12));
    { const bool c = (int32_t)v0 >= 0; a0 = a0 - 0x50u; if (c) { A3_BACK(); goto L_14647f8; } }
    { return v0; }
    __builtin_unreachable();
}

// kernel_sub_001AD958 (0x1ad958): typed by tools/typed_lift2.py (round A3; loop frame calls, frame 112 bytes, 24 slot accesses)
inline uint64_t kernel_sub_001AD958(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint64_t a0, uint64_t a1, uint32_t a2)
{
    uint64_t v0 = 0;
    uint32_t v1 = 0;
    uint32_t s0 = 0;
    uint32_t s1 = 0;
    uint32_t s2 = 0;
    uint32_t s3 = 0;
    uint64_t s4 = 0;
    uint64_t s5 = 0;
    uint64_t s6 = 0;
    uint32_t s7 = 0;
    uint32_t fp = 0;
    uint8_t a2fr[112];
    (s5 = (uint64_t)((int64_t)(int32_t)((int32_t)SRL32(a2, 4))));
    (v0 = (uint64_t)(((uint64_t)RC_U64X(s5) < (uint64_t)(int64_t)(int32_t)1025) ? 1 : 0));
    (s6 = (uint64_t)((uint64_t)a0 + (uint64_t)0ULL));
    (s4 = (uint64_t)((uint64_t)a1 + (uint64_t)0ULL));
    if ((int64_t)v0 != 0) goto L_1adad4;
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)4096 << 16))));
    (v1 = (uint32_t)((int32_t)((int32_t)((uint32_t)4096 << 16))));
    (s7 = (uint32_t)((int32_t)((int32_t)((uint32_t)4096 << 16))));
    (v0 = (uint64_t)(RC_U64X(v0) | (uint64_t)(uint16_t)54288));
    v1 = v1 | 0xD420u;
    a2::frSt<uint32_t>(a2fr, 0, s7);
    a2::frSt<uint32_t>(a2fr, 4, s7);
    (s0 = (uint32_t)((int32_t)((int32_t)((uint32_t)4096 << 16))));
    a2::frSt<uint32_t>(a2fr, 0, (uint32_t)v0);
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)4096 << 16))));
    a2::frSt<uint32_t>(a2fr, 4, v1);
    (v1 = (uint32_t)((int32_t)((int32_t)((uint32_t)4096 << 16))));
    (v0 = (uint64_t)(RC_U64X(v0) | (uint64_t)(uint16_t)54400));
    v1 = v1 | 0xD400u;
    a2::frSt<uint32_t>(a2fr, 8, s7);
    s0 = s0 | 0xE020u;
    a2::frSt<uint32_t>(a2fr, 12, s7);
    fp = 0xFFFFFC00u;
    a2::frSt<uint32_t>(a2fr, 8, (uint32_t)v0);
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)4096 << 16))));
    a2::frSt<uint32_t>(a2fr, 12, v1);
    (v1 = (uint32_t)((int32_t)((int32_t)((uint32_t)4096 << 16))));
    (v0 = (uint64_t)(RC_U64X(v0) | (uint64_t)(uint16_t)53264));
    v1 = v1 | 0xD020u;
    a2::frSt<uint32_t>(a2fr, 16, s7);
    s3 = 0x400u;
    a2::frSt<uint32_t>(a2fr, 20, s7);
    s2 = 0x70000000u;
    a2::frSt<uint32_t>(a2fr, 16, (uint32_t)v0);
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)4096 << 16))));
    a2::frSt<uint32_t>(a2fr, 20, v1);
    (v1 = (uint32_t)((int32_t)((int32_t)((uint32_t)4096 << 16))));
    (v0 = (uint64_t)(RC_U64X(v0) | (uint64_t)(uint16_t)53376));
    v1 = v1 | 0xD000u;
    a2::frSt<uint32_t>(a2fr, 24, s7);
    s1 = 0x100u;
    a2::frSt<uint32_t>(a2fr, 28, s7);
    s7 = s7 | 0xE010u;
    a2::frSt<uint32_t>(a2fr, 24, (uint32_t)v0);
    a2::frSt<uint32_t>(a2fr, 28, v1);
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 512))));
L_1ada40:;
    A1_W32(s0, fp);
    A1_W32B(s7, 0, (uint32_t)v0);
    (s5 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)s5, 4294966272))));
    v1 = a2::frLd<uint32_t>(a2fr, 0);
    A1_W32B(v1, 0, (uint32_t)s4);
    (s4 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)s4, 16384))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)a2::frLd<uint32_t>(a2fr, 4))));
    A1_W32B((uint32_t)v0, 0, s3);
    v1 = a2::frLd<uint32_t>(a2fr, 8);
    A1_W32(v1, s2);
    v1 = 0x101u;
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B(s0, 0))));
    (v0 = (uint64_t)(RC_U64X(v0) | (uint64_t)(uint16_t)512));
    A1_W32B(s0, 0, (uint32_t)v0);
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)a2::frLd<uint32_t>(a2fr, 12))));
    A1_W32B((uint32_t)v0, 0, v1);
    { A3_CHARGE(); kernel_sub_001B06C8(ram, rt); }
    A1_W32(s0, fp);
    A1_W32(s7, s1);
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)a2::frLd<uint32_t>(a2fr, 16))));
    A1_W32B((uint32_t)v0, 0, (uint32_t)s6);
    (s6 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)s6, 16384))));
    v1 = a2::frLd<uint32_t>(a2fr, 20);
    A1_W32(v1, s3);
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)a2::frLd<uint32_t>(a2fr, 24))));
    A1_W32B((uint32_t)v0, 0, s2);
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B(s0, 0))));
    (v0 = (uint64_t)(RC_U64X(v0) | (uint64_t)(uint16_t)256));
    A1_W32B(s0, 0, (uint32_t)v0);
    v1 = a2::frLd<uint32_t>(a2fr, 28);
    A1_W32(v1, s1);
    { A3_CHARGE(); kernel_sub_001B06C8(ram, rt); }
    (v0 = (uint64_t)(((uint64_t)s5 < (uint64_t)(int64_t)(int32_t)1025) ? 1 : 0));
    { const bool c = (int64_t)v0 == 0; (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 512)))); if (c) { A3_BACK(); goto L_1ada40; } }
L_1adad4:;
    (s0 = (uint32_t)((int32_t)((int32_t)((uint32_t)4096 << 16))));
    (s1 = (uint32_t)((int32_t)((int32_t)((uint32_t)4096 << 16))));
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)4096 << 16))));
    (a1 = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)4096 << 16))));
    (a2 = (uint32_t)((int32_t)((int32_t)((uint32_t)4096 << 16))));
    s0 = s0 | 0xE020u;
    s2 = 0xFFFFFC00u;
    s1 = s1 | 0xE010u;
    (a1 = (uint64_t)(RC_U64X(a1) | (uint64_t)(uint16_t)54304));
    a2 = a2 | 0xD480u;
    s3 = 0x70000000u;
    (a0 = (uint64_t)(RC_U64X(a0) | (uint64_t)(uint16_t)54288));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 512))));
    A1_W32(s0, s2);
    A1_W32B(s1, 0, (uint32_t)v0);
    (v1 = (uint32_t)((int32_t)((int32_t)((uint32_t)4096 << 16))));
    A1_W32B((uint32_t)a0, 0, (uint32_t)s4);
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 257))));
    A1_W32B((uint32_t)a1, 0, (uint32_t)s5);
    v1 = v1 | 0xD400u;
    A1_W32(a2, s3);
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B(s0, 0))));
    (v0 = (uint64_t)(RC_U64X(v0) | (uint64_t)(uint16_t)512));
    A1_W32B(s0, 0, (uint32_t)v0);
    A1_W32B(v1, 0, (uint32_t)a0);
    { A3_CHARGE(); kernel_sub_001B06C8(ram, rt); }
    A1_W32(s0, s2);
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)4096 << 16))));
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)4096 << 16))));
    (a1 = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)4096 << 16))));
    a2 = 0x100u;
    (a0 = (uint64_t)(RC_U64X(a0) | (uint64_t)(uint16_t)53280));
    (a1 = (uint64_t)(RC_U64X(a1) | (uint64_t)(uint16_t)53376));
    (v0 = (uint64_t)(RC_U64X(v0) | (uint64_t)(uint16_t)53264));
    A1_W32(s1, a2);
    A1_W32B((uint32_t)v0, 0, (uint32_t)s6);
    (v1 = (uint32_t)((int32_t)((int32_t)((uint32_t)4096 << 16))));
    A1_W32B((uint32_t)a0, 0, (uint32_t)s5);
    v1 = v1 | 0xD000u;
    A1_W32B((uint32_t)a1, 0, s3);
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B(s0, 0))));
    (v0 = (uint64_t)(RC_U64X(v0) | (uint64_t)(uint16_t)256));
    A1_W32B(s0, 0, (uint32_t)v0);
    A1_W32(v1, a2);
    { A3_CHARGE(); kernel_sub_001B06C8(ram, rt); }
    { return v0; }
    __builtin_unreachable();
}

// Sg2CtrlInit (0x1cc278): typed by tools/typed_lift2.py (round A3; loop frame calls, frame 16 bytes, 0 slot accesses)
inline uint64_t Sg2CtrlInit(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt)
{
    uint64_t v0 = 0;
    uint32_t v1 = 0;
    uint32_t a0 = 0;
    v1 = 0x270000u;
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)29 << 16))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v0, 4294950952))));
    v1 = v1 + 0x3D40u;
    a0 = 0xFu;
L_1cc298:;
    a0 = a0 - 1u;
    A1_W32B(v1, 0, (uint32_t)v0);
    A1_W32B(v1, 4, 0u);
    { const bool c = (int32_t)a0 >= 0; v1 = v1 + 8u; if (c) { A3_BACK(); goto L_1cc298; } }
    { a0 = 0u; A3_CHARGE(); v0 = a1_sx64(Sg2CtrlSetTickTime(ram, rt, a0)); }
    (v0 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL));
    { return v0; }
    __builtin_unreachable();
}

// GAMEUTIL_reset_interp_value_cntrl (0x1084428): typed by tools/typed_lift2.py (round A3; frame calls, extra outputs xo[0]=v0, frame 16 bytes, 0 slot accesses)
inline float GAMEUTIL_reset_interp_value_cntrl(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint64_t a0, float f12, uint64_t *xo = nullptr)
{
    uint64_t v0 = 0;
    (v0 = (uint64_t)((uint64_t)a0 + (uint64_t)0ULL));
    { float f = f12; uint32_t bits; std::memcpy(&bits, &f, sizeof(bits)); A1_W32B((uint32_t)v0, 0, bits); }
    { float f = f12; uint32_t bits; std::memcpy(&bits, &f, sizeof(bits)); A1_W32B((uint32_t)v0, 8, bits); }
    { float f = f12; uint32_t bits; std::memcpy(&bits, &f, sizeof(bits)); A1_W32B((uint32_t)v0, 4, bits); }
    { if (xo) { xo[0] = v0; } return GAMEUTIL_get_interp_value_interp_value_cntrl(ram, rt, (uint32_t)a0); }
    __builtin_unreachable();
}

// constructor_double_spring_interp_cntrl (0x1084528): typed by tools/typed_lift2.py (round A3; frame calls, extra outputs xo[0]=f13 xo[1]=f14 xo[2]=f15 xo[3]=f20, frame 32 bytes, 2 slot accesses)
inline void constructor_double_spring_interp_cntrl(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint64_t a0, float f12, float f20, uint64_t *xo = nullptr)
{
    float f13 = 0.0f;
    float f14 = 0.0f;
    float f15 = 0.0f;
    uint32_t at = 0;
    uint64_t s0 = 0;
    uint8_t a2fr[32];
    at = 0xBF800000u;
    { uint32_t bits = at; std::memcpy(&f13, &bits, sizeof(bits)); }
    (s0 = (uint64_t)((uint64_t)a0 + (uint64_t)0ULL));
    f15 = ((float)(f13));
    a2::frSt<uint32_t>(a2fr, 16, a1_bits(f20));
    f20 = ((float)(f12));
    A1_W32B((uint32_t)s0, 20, 0u);
    at = 0x40000000u;
    { uint32_t bits = at; std::memcpy(&f12, &bits, sizeof(bits)); }
    A1_W32B((uint32_t)s0, 28, 0u);
    at = 0x3F800000u;
    { uint32_t bits = at; std::memcpy(&f14, &bits, sizeof(bits)); }
    { A1_W32B((uint32_t)s0, 24, 0u); A3_CHARGE(); set_param_double_spring_interp_common(ram, rt, (uint32_t)a0, f12, f13, f14, f15); }
    { float f = f20; uint32_t bits; std::memcpy(&bits, &f, sizeof(bits)); A1_W32B((uint32_t)s0, 16, bits); }
    f20 = a1_flt(a2::frLd<uint32_t>(a2fr, 16));
    { if (xo) { xo[0] = a1_bits(f13); xo[1] = a1_bits(f14); xo[2] = a1_bits(f15); xo[3] = a1_bits(f20); } return; }
    __builtin_unreachable();
}

// constructor_double_spring_interp_cntrl3D (0x10845f8): typed by tools/typed_lift2.py (round A3; loop frame calls, extra outputs xo[0]=f14 xo[1]=f15 xo[2]=f20 xo[3]=f21 xo[4]=a2 xo[5]=a3 xo[6]=t0 xo[7]=t1, frame 32 bytes, 4 slot accesses)
inline uint32_t constructor_double_spring_interp_cntrl3D(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint64_t a0, float f12, float f13, float f20, float f21, uint64_t *xo = nullptr)
{
    float f14 = 0.0f;
    float f15 = 0.0f;
    uint32_t at = 0;
    uint32_t v0 = 0;
    uint32_t v1 = 0;
    uint32_t a1 = 0;
    uint64_t a2 = 0;
    uint32_t a3 = 0;
    uint32_t t0 = 0;
    uint32_t t1 = 0;
    uint64_t s0 = 0;
    uint8_t a2fr[32];
    (a2 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL));
    (s0 = (uint64_t)((uint64_t)a0 + (uint64_t)0ULL));
    (t1 = (uint32_t)((int32_t)((int32_t)ADD32((uint32_t)s0, 20))));
    a2::frSt<uint32_t>(a2fr, 24, a1_bits(f21));
    f21 = ((float)(f12));
    a2::frSt<uint32_t>(a2fr, 16, a1_bits(f20));
    f20 = ((float)(f13));
    (t0 = (uint32_t)((int32_t)((int32_t)ADD32((uint32_t)s0, 32))));
    (a3 = (uint32_t)((int32_t)((int32_t)ADD32((uint32_t)s0, 48))));
L_1084628:;
    (v0 = (uint32_t)((int32_t)((int32_t)SLL32((uint32_t)a2, 2))));
    (a2 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)a2, 1))));
    a1 = v0 + a3;
    v1 = v0 + t1;
    (a0 = (uint64_t)(((int64_t)(int64_t)RC_U64X(a2) < (int64_t)(int32_t)3) ? 1 : 0));
    v0 = v0 + t0;
    A1_W32(v1, 0u);
    A1_W32(a1, 0u);
    { const bool c = (int64_t)a0 != 0; A1_W32(v0, 0u); if (c) { A3_BACK(); goto L_1084628; } }
    at = 0xBF800000u;
    { uint32_t bits = at; std::memcpy(&f13, &bits, sizeof(bits)); }
    (a0 = (uint64_t)((uint64_t)s0 + (uint64_t)0ULL));
    at = 0x40000000u;
    { uint32_t bits = at; std::memcpy(&f12, &bits, sizeof(bits)); }
    at = 0x3F800000u;
    { uint32_t bits = at; std::memcpy(&f14, &bits, sizeof(bits)); }
    { f15 = ((float)(f13)); A3_CHARGE(); set_param_double_spring_interp_common(ram, rt, (uint32_t)a0, f12, f13, f14, f15); }
    { float f = f21; uint32_t bits; std::memcpy(&bits, &f, sizeof(bits)); A1_W32B((uint32_t)s0, 16, bits); }
    { float f = f20; uint32_t bits; std::memcpy(&bits, &f, sizeof(bits)); A1_W32B((uint32_t)s0, 44, bits); }
    { float f = f20; uint32_t bits; std::memcpy(&bits, &f, sizeof(bits)); A1_W32B((uint32_t)s0, 60, bits); }
    f21 = a1_flt(a2::frLd<uint32_t>(a2fr, 24));
    f20 = a1_flt(a2::frLd<uint32_t>(a2fr, 16));
    { if (xo) { xo[0] = a1_bits(f14); xo[1] = a1_bits(f15); xo[2] = a1_bits(f20); xo[3] = a1_bits(f21); xo[4] = a2; xo[5] = a1_sx64(a3); xo[6] = a1_sx64(t0); xo[7] = a1_sx64(t1); } return v0; }
    __builtin_unreachable();
}

// manager_sub_010DF5D0 (0x10df5d0): typed by tools/typed_lift2.py (round A3; loop frame calls, frame 32 bytes, 0 slot accesses)
inline uint64_t manager_sub_010DF5D0(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint64_t a0, uint64_t a1)
{
    uint64_t v0 = 0;
    uint64_t v1 = 0;
    uint64_t a2 = 0;
    uint64_t s0 = 0;
    uint64_t s1 = 0;
    (s0 = (uint64_t)((uint64_t)a1 + (uint64_t)0ULL));
    (s1 = (uint64_t)((uint64_t)a0 + (uint64_t)0ULL));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)s0, 64))));
    if ((int64_t)s1 != (int64_t)v0) goto L_10df600;
L_10df5f4:;
    (v0 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL));
    goto L_10df6b8;
L_10df600:;
    { A3_CHARGE(); v0 = GetScrLwsOriClass(ram, rt, a0); }
    (v1 = (uint64_t)((uint64_t)v0 + (uint64_t)0ULL));
    { const bool c = (int64_t)v1 == 0; (a2 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL)); if (c) goto L_10df660; }
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)v1, 4600))));
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)v0, 48))));
    { const bool c = (int32_t)a0 <= 0; (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)v1, 4608)))); if (c) goto L_10df6b4; }
    (a1 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)s0, 64))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)v1, 0))));
L_10df630:;
    { const bool c = (int64_t)v0 == (int64_t)a1; (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v1, 80)))); if (c) { A3_BACK(); goto L_10df5f4; } }
    (a2 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)a2, 1))));
    (v0 = (uint64_t)(((int64_t)(int64_t)RC_U64X(a2) < (int64_t)(int64_t)a0) ? 1 : 0));
    if ((int64_t)v0 != 0) { (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)v1, 0)))); { A3_BACK(); goto L_10df630; } }
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 1))));
    goto L_10df6b8;
L_10df660:;
    { (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)s0, 64)))); A3_CHARGE(); v0 = GetScrLwsOriClass(ram, rt, a0); }
    (v1 = (uint64_t)((uint64_t)v0 + (uint64_t)0ULL));
    { const bool c = (int64_t)v1 == 0; (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 1)))); if (c) goto L_10df6b8; }
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)v1, 4600))));
    (a1 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL));
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)v0, 48))));
    { const bool c = (int32_t)a0 <= 0; (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)v1, 4608)))); if (c) goto L_10df6b4; }
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)v1, 0))));
L_10df690:;
    { const bool c = (int64_t)v0 == (int64_t)s1; (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v1, 80)))); if (c) { A3_BACK(); goto L_10df5f4; } }
    (a1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)a1, 1))));
    (v0 = (uint64_t)(((int64_t)(int64_t)RC_U64X(a1) < (int64_t)(int64_t)a0) ? 1 : 0));
    if ((int64_t)v0 != 0) { (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)v1, 0)))); { A3_BACK(); goto L_10df690; } }
L_10df6b4:;
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 1))));
L_10df6b8:;
    { return v0; }
    __builtin_unreachable();
}

// SetScriptLayoutObjLetterProcMode (0x110d0a0): typed by tools/typed_lift2.py (round A3; frame calls, extra outputs xo[0]=a2, frame 16 bytes, 0 slot accesses)
inline uint64_t SetScriptLayoutObjLetterProcMode(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint64_t a0, uint64_t a1, uint64_t *xo = nullptr)
{
    uint64_t v0 = 0;
    uint32_t v1 = 0;
    uint32_t a2 = 0;
    uint64_t s0 = 0;
    (v0 = (uint64_t)((uint64_t)a0 + (uint64_t)0ULL));
    (s0 = (uint64_t)((uint64_t)a1 + (uint64_t)0ULL));
    v1 = 0xBu;
    (a2 = (uint32_t)((uint8_t)A1_R8B((uint32_t)v0, 8)));
    { const bool c = a2 == v1; (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v0, 32)))); if (c) goto L_110d0e0; }
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 13))));
    if ((int64_t)(int32_t)a2 != (int64_t)v0) { (v0 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL)); goto L_110d0e0; }
    { A3_CHARGE(); v0 = GetScriptAnimationClass(ram, rt, a0); }
    A1_W32B((uint32_t)v0, 720, (uint32_t)s0);
    goto L_110d0e4;
L_110d0e0:;
    A1_W32B((uint32_t)v0, 720, (uint32_t)s0);
L_110d0e4:;
    { if (xo) { xo[0] = a1_sx64(a2); } return v0; }
    __builtin_unreachable();
}

// gStatGetHorseDemoStageIndex (0x13538e8): typed by tools/typed_lift2.py (round A3; loop frame calls, frame 48 bytes, 0 slot accesses)
inline uint64_t gStatGetHorseDemoStageIndex(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t a0)
{
    uint64_t v0 = 0;
    uint64_t v1 = 0;
    uint64_t s0 = 0;
    uint64_t s1 = 0;
    uint64_t s2 = 0;
    uint32_t s3 = 0;
    { const uint32_t _dynInsn = A1_R32(0x13538ECu); const int32_t _dynSimm = (int16_t)(_dynInsn & 0xFFFFu); const uint32_t _dynImm = _dynInsn & 0xFFFFu; (void)_dynSimm; (void)_dynImm; (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)_dynImm << 16)))); }
    { const uint32_t _dynInsn = A1_R32(0x13538F4u); const int32_t _dynSimm = (int16_t)(_dynInsn & 0xFFFFu); const uint32_t _dynImm = _dynInsn & 0xFFFFu; (void)_dynSimm; (void)_dynImm; (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v0, _dynSimm)))); }
    (s2 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL));
    (s1 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)v0, 12))));
    (s3 = (uint32_t)((int32_t)((int32_t)A1_R32B((uint32_t)v0, 16))));
    (s1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)s1, 4294967295))));
    { const bool c = (int32_t)s1 <= 0; (s0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(s3, 20)))); if (c) goto L_1353940; }
L_1353920:;
    { (a0 = (uint32_t)((int32_t)((int32_t)A1_R32B((uint32_t)s0, 4)))); A3_CHARGE(); v0 = _omakeSelectFlagChk(ram, rt, a1_sx64(a0)); }
    { const bool c = (int64_t)v0 != 0; (v1 = (uint64_t)((uint64_t)s0 + (uint64_t)0ULL)); if (c) goto L_1353944; }
    (s2 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)s2, 1))));
    (v0 = (uint64_t)(((int64_t)(int64_t)RC_U64X(s2) < (int64_t)(int64_t)s1) ? 1 : 0));
    { const bool c = (int64_t)v0 != 0; (s0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)s0, 20)))); if (c) { A3_BACK(); goto L_1353920; } }
L_1353940:;
    (v1 = (uint64_t)((uint64_t)a1_sx64(s3) + (uint64_t)0ULL));
L_1353944:;
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)v1, 8))));
    { return v0; }
    __builtin_unreachable();
}

// getPlayerMaxGripRatio (0x13586d0): typed by tools/typed_lift2.py (round A3; frame calls, frame 16 bytes, 0 slot accesses)
inline a1::RetVF getPlayerMaxGripRatio(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t a0, float f0)
{
    float f1 = 0.0f;
    uint32_t at = 0;
    uint32_t v0 = 0;
    uint32_t fcr31 = 0;
    { a0 = 0xCu; A3_CHARGE(); { const a1::RetVF a2_r = _gameFWorkChk(ram, rt, a0); v0 = (uint32_t)a2_r.v0; f0 = a2_r.f0; } }
    at = 0x42C80000u;
    f1 = a1_flt(at);
    f0 = a1f::div(fcr31, f0, f1);
    { return a1::RetVF{(uint64_t)a1_sx64(v0), f0}; }
    __builtin_unreachable();
}

// initGameObject (0x1366d20): typed by tools/typed_lift2.py (round A3; loop frame calls, frame 48 bytes, 0 slot accesses)
inline uint32_t initGameObject(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t a0, uint32_t a1)
{
    uint32_t v0 = 0;
    uint32_t v1 = 0;
    uint32_t a2 = 0;
    uint32_t s0 = 0;
    uint64_t s1 = 0;
    uint32_t s2 = 0;
    uint32_t s3 = 0;
    uint64_t hi = 0;
    uint64_t lo = 0;
    v0 = A1_R32B(a0, 12);
    { const bool c = (int32_t)v0 <= 0; s0 = A1_R32B(a0, 16); if (c) goto L_1366dc0; }
    s2 = 0x30000u;
    (s1 = (uint64_t)((uint64_t)a1_sx64(v0) + (uint64_t)0ULL));
    s2 = s2 | 0xFFFFu;
    { const uint32_t _dynInsn = A1_R32(0x1366D50u); const int32_t _dynSimm = (int16_t)(_dynInsn & 0xFFFFu); const uint32_t _dynImm = _dynInsn & 0xFFFFu; (void)_dynSimm; (void)_dynImm; (s3 = (uint32_t)((int32_t)((int32_t)((uint32_t)_dynImm << 16)))); }
L_1366d58:;
    v1 = s0 + 0x10u;
    a0 = 0u;
    a2 = A1_R32(v1);
    v0 = a2 & s2;
    { const bool c = v0 == 0; v1 = v1 - v0; if (c) goto L_1366d74; }
    a0 = A1_R32(v1);
L_1366d74:;
    { const bool c = a0 == 0; { const uint32_t _dynInsn = A1_R32(0x1366D78u); const int32_t _dynSimm = (int16_t)(_dynInsn & 0xFFFFu); const uint32_t _dynImm = _dynInsn & 0xFFFFu; (void)_dynSimm; (void)_dynImm; (v0 = (uint32_t)((int32_t)((int32_t)A1_R32(ADD32(s3, _dynSimm))))); } if (c) goto L_1366da0; }
    { const bool c = a0 == v0; v1 = a2 >> 18; if (c) goto L_1366da0; }
    a1 = A1_R32B(a0, 8);
    v1 = v1 & 0xFFFu;
    a0 = A1_R32B(a0, 16);
    v0 = A1_R32B(a1, 12);
    { int64_t result = (int64_t)(int32_t)v1 * (int64_t)(int32_t)v0; lo = (uint64_t)(int64_t)(int32_t)result; hi = (uint64_t)(int64_t)(int32_t)(result >> 32); (v1 = (uint32_t)((int32_t)((int32_t)result))); }
    a0 = a0 + v1;
    goto L_1366da4;
L_1366da0:;
    a0 = 0u;
L_1366da4:;
    { const bool c = a0 == 0; A1_W32B(s0, 8, 0u); if (c) goto L_1366db4; }
    { a1 = s0; A3_CHARGE(); gStatSetGObjAdrs(ram, rt, a0, a1); }
L_1366db4:;
    (s1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)s1, 4294967295))));
    { const bool c = (int64_t)s1 != 0; s0 = s0 + 0x14u; if (c) { A3_BACK(); goto L_1366d58; } }
L_1366dc0:;
    { return v0; }
    __builtin_unreachable();
}

// gamecore_sub_013AE748 (0x13ae748): typed by tools/typed_lift2.py (round A3; frame calls, frame 16 bytes, 0 slot accesses)
inline void gamecore_sub_013AE748(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint64_t a0)
{
    uint64_t a1 = 0;
    (a1 = (uint64_t)((uint64_t)a0 + (uint64_t)0ULL));
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 17))));
    { (void)_optionWorkSet(ram, rt, (uint32_t)a0, (uint32_t)a1); return; }
    __builtin_unreachable();
}

// gamecore_sub_013F3378 (0x13f3378): typed by tools/typed_lift2.py (round A3; frame calls, extra outputs xo[0]=f12 xo[1]=f13 xo[2]=f20, frame 48 bytes, 2 slot accesses)
inline a1::RetVF gamecore_sub_013F3378(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint64_t a0, uint64_t a1, float f12, float f13, float f20, uint64_t *xo = nullptr)
{
    float f0 = 0.0f;
    float f1 = 0.0f;
    float f2 = 0.0f;
    float f3 = 0.0f;
    uint32_t v0 = 0;
    uint32_t v1 = 0;
    uint64_t s0 = 0;
    uint64_t s1 = 0;
    uint64_t s2 = 0;
    uint64_t s3 = 0;
    uint32_t fcr31 = 0;
    uint8_t a2fr[48];
    { uint32_t bits = 0u; std::memcpy(&f2, &bits, sizeof(bits)); }
    (s2 = (uint64_t)((uint64_t)a0 + (uint64_t)0ULL));
    (s3 = (uint64_t)((uint64_t)a1 + (uint64_t)0ULL));
    a2::frSt<uint32_t>(a2fr, 40, a1_bits(f20));
    { uint32_t bits = A1_R32B((uint32_t)s2, 560); float f; std::memcpy(&f, &bits, sizeof(f)); f1 = f; }
    { uint32_t bits = A1_R32B((uint32_t)s3, 0); float f; std::memcpy(&f, &bits, sizeof(f)); f0 = f; }
    f1 = a1f::sub(f1, f0);
    fcr31 = (a1f::lt(f2, f1)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
    { const bool c = (fcr31 & 0x800000u) == 0u; f3 = ((float)(f12)); if (c) goto L_13f3428; }
    v0 = 0x26u;
    (v1 = (uint32_t)((int32_t)((int32_t)A1_R32B((uint32_t)s2, 100))));
    A1_W32B((uint32_t)s2, 616, v0);
    fcr31 = (a1f::lt(f13, f2)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
    v0 = v1;
    f0 = a1_flt(A1_R32B(v0, 56));
    f1 = a1_flt(A1_R32B(v0, 36));
    f2 = a1_flt(A1_R32B(v0, 44));
    f0 = a1f::mul(f0, f1);
    { const bool c = (fcr31 & 0x800000u) == 0u; f12 = a1f::add(f3, f0); if (c) goto L_13f33f0; }
    f0 = (-(float)(f13));
    f0 = a1f::mul(f0, f2);
    goto L_13f33f4;
L_13f33f0:;
    f0 = a1f::mul(f13, f2);
L_13f33f4:;
    f3 = a1f::add(f12, f0);
    f0 = a1_flt(A1_R32B(v0, 48));
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)s2, 612))));
    { uint32_t bits = A1_R32B((uint32_t)s3, 0); float f; std::memcpy(&f, &bits, sizeof(f)); f12 = f; }
    { uint32_t bits = A1_R32B((uint32_t)s2, 764); float f; std::memcpy(&f, &bits, sizeof(f)); f2 = f; }
    f12 = a1f::mul(f12, f0);
    { uint32_t bits = A1_R32B((uint32_t)s2, 768); float f; std::memcpy(&f, &bits, sizeof(f)); f0 = f; }
    f1 = a1_flt(A1_R32B(v0, 56));
    f13 = a1_flt(A1_R32B(v0, 32));
    f20 = a1f::mul(f1, f0);
    f12 = a1f::add(f12, f2);
    f12 = a1f::add(f3, f12);
    goto L_13f3538;
L_13f3428:;
    fcr31 = (a1f::lt(f1, f2)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
    if ((fcr31 & 0x800000u) == 0u) { { uint32_t bits = A1_R32B((uint32_t)s2, 572); float f; std::memcpy(&f, &bits, sizeof(f)); f0 = f; } goto L_13f34a0; }
    v0 = 0x27u;
    (v1 = (uint32_t)((int32_t)((int32_t)A1_R32B((uint32_t)s2, 100))));
    A1_W32B((uint32_t)s2, 616, v0);
    fcr31 = (a1f::lt(f13, f2)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
    v0 = v1;
    f0 = a1_flt(A1_R32B(v0, 56));
    f1 = a1_flt(A1_R32B(v0, 40));
    f2 = a1_flt(A1_R32B(v0, 44));
    f0 = a1f::mul(f0, f1);
    { const bool c = (fcr31 & 0x800000u) == 0u; f12 = a1f::sub(f3, f0); if (c) goto L_13f3470; }
    f0 = (-(float)(f13));
    f0 = a1f::mul(f0, f2);
    goto L_13f3474;
L_13f3470:;
    f0 = a1f::mul(f13, f2);
L_13f3474:;
    f3 = a1f::add(f12, f0);
    f0 = a1_flt(A1_R32B(v0, 48));
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)s2, 612))));
    { uint32_t bits = A1_R32B((uint32_t)s3, 0); float f; std::memcpy(&f, &bits, sizeof(f)); f12 = f; }
    { uint32_t bits = A1_R32B((uint32_t)s2, 764); float f; std::memcpy(&f, &bits, sizeof(f)); f1 = f; }
    f12 = a1f::mul(f12, f0);
    f13 = a1_flt(A1_R32B(v0, 32));
    f20 = a1_flt(A1_R32B(v0, 56));
    f12 = a1f::add(f12, f1);
    f12 = a1f::add(f3, f12);
    goto L_13f3538;
L_13f34a0:;
    fcr31 = (a1f::eq(f0, f2)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
    { const bool c = (fcr31 & 0x800000u) != 0u; v0 = 0x28u; if (c) goto L_13f3508; }
    fcr31 = (a1f::lt(f13, f2)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
    (v1 = (uint32_t)((int32_t)((int32_t)A1_R32B((uint32_t)s2, 100))));
    A1_W32B((uint32_t)s2, 616, v0);
    v0 = v1;
    { const bool c = (fcr31 & 0x800000u) == 0u; f1 = a1_flt(A1_R32B(v0, 44)); if (c) goto L_13f34d0; }
    f0 = (-(float)(f13));
    f0 = a1f::mul(f0, f1);
    goto L_13f34d4;
L_13f34d0:;
    f0 = a1f::mul(f13, f1);
L_13f34d4:;
    f3 = a1f::add(f3, f0);
    f0 = a1_flt(A1_R32B(v0, 48));
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)s2, 612))));
    { uint32_t bits = A1_R32B((uint32_t)s3, 0); float f; std::memcpy(&f, &bits, sizeof(f)); f12 = f; }
    { uint32_t bits = A1_R32B((uint32_t)s2, 764); float f; std::memcpy(&f, &bits, sizeof(f)); f1 = f; }
    f12 = a1f::mul(f12, f0);
    A1_W32B((uint32_t)s2, 564, 0u);
    f13 = a1_flt(A1_R32B(v0, 32));
    { uint32_t bits = A1_R32B((uint32_t)s2, 564); float f; std::memcpy(&f, &bits, sizeof(f)); f20 = f; }
    f12 = a1f::add(f12, f1);
    f12 = a1f::add(f3, f12);
    goto L_13f3538;
L_13f3508:;
    (v1 = (uint32_t)((int32_t)((int32_t)A1_R32B((uint32_t)s2, 100))));
    A1_W32B((uint32_t)s2, 616, v0);
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)s2, 612))));
    { uint32_t bits = A1_R32B((uint32_t)s2, 764); float f; std::memcpy(&f, &bits, sizeof(f)); f1 = f; }
    f20 = ((float)(f2));
    f0 = a1_flt(A1_R32B(v1, 48));
    { uint32_t bits = A1_R32B((uint32_t)s3, 0); float f; std::memcpy(&f, &bits, sizeof(f)); f12 = f; }
    { float f = f2; uint32_t bits; std::memcpy(&bits, &f, sizeof(bits)); A1_W32B((uint32_t)s2, 564, bits); }
    f12 = a1f::mul(f12, f0);
    f13 = a1_flt(A1_R32B(v1, 32));
    f12 = a1f::add(f3, f12);
    f12 = a1f::add(f12, f1);
L_13f3538:;
    { (s0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)s2, 564)))); A3_CHARGE(); { const a1::RetVF a2_r = AI_UpdateVelocity(ram, rt, (uint32_t)a0, f12, f13); v0 = (uint32_t)a2_r.v0; f0 = a2_r.f0; } }
    { uint32_t bits = A1_R32B((uint32_t)s2, 608); float f; std::memcpy(&f, &bits, sizeof(f)); f1 = f; }
    (v1 = (uint32_t)((int32_t)((int32_t)A1_R32B((uint32_t)s2, 100))));
    f0 = a1_flt(A1_R32B(v1, 20));
    f0 = a1f::mul(f0, f1);
    { float f = f0; uint32_t bits; std::memcpy(&bits, &f, sizeof(bits)); A1_W32B((uint32_t)s2, 560, bits); }
    { uint32_t bits = A1_R32B((uint32_t)s2, 760); float f; std::memcpy(&f, &bits, sizeof(f)); f1 = f; }
    { uint32_t bits = A1_R32B((uint32_t)s2, 560); float f; std::memcpy(&f, &bits, sizeof(f)); f0 = f; }
    f12 = ((float)(f20));
    (a0 = (uint64_t)((uint64_t)s0 + (uint64_t)0ULL));
    (s1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)s2, 568))));
    f0 = a1f::mul(f0, f1);
    { float f = f0; uint32_t bits; std::memcpy(&bits, &f, sizeof(bits)); A1_W32B((uint32_t)s2, 560, bits); }
    { f13 = a1_flt(A1_R32B(v1, 52)); A3_CHARGE(); { const a1::RetVF a2_r = AI_UpdateVelocity(ram, rt, (uint32_t)a0, f12, f13); v0 = (uint32_t)a2_r.v0; f0 = a2_r.f0; } }
    (a0 = (uint64_t)((uint64_t)s3 + (uint64_t)0ULL));
    { uint32_t bits = A1_R32B((uint32_t)s2, 560); float f; std::memcpy(&f, &bits, sizeof(f)); f12 = f; }
    { { uint32_t bits = A1_R32B((uint32_t)s0, 0); float f; std::memcpy(&f, &bits, sizeof(f)); f13 = f; } A3_CHARGE(); { const a1::RetVF a2_r = AI_UpdateVelocity(ram, rt, (uint32_t)a0, f12, f13); v0 = (uint32_t)a2_r.v0; f0 = a2_r.f0; } }
    (v1 = (uint32_t)((int32_t)((int32_t)A1_R32B((uint32_t)s2, 100))));
    (a0 = (uint64_t)((uint64_t)s1 + (uint64_t)0ULL));
    f13 = a1_flt(A1_R32B(v1, 60));
    { f12 = a1_flt(A1_R32B(v1, 64)); A3_CHARGE(); { const a1::RetVF a2_r = AI_UpdateVelocity(ram, rt, (uint32_t)a0, f12, f13); v0 = (uint32_t)a2_r.v0; f0 = a2_r.f0; } }
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)s3, 8))));
    { uint32_t bits = A1_R32B((uint32_t)s1, 0); float f; std::memcpy(&f, &bits, sizeof(f)); f13 = f; }
    { uint32_t bits = 0u; std::memcpy(&f12, &bits, sizeof(bits)); }
    f20 = a1_flt(a2::frLd<uint32_t>(a2fr, 40));
    { if (xo) { xo[0] = a1_bits(f12); xo[1] = a1_bits(f13); xo[2] = a1_bits(f20); } return AI_UpdateVelocity(ram, rt, (uint32_t)a0, f12, f13); }
    __builtin_unreachable();
}

// gamecore_sub_013F3778 (0x13f3778): typed by tools/typed_lift2.py (round A3; frame calls, extra outputs xo[0]=f12 xo[1]=f13, frame 32 bytes, 2 slot accesses)
inline a1::RetVF gamecore_sub_013F3778(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint64_t a0, float f20, uint64_t *xo = nullptr)
{
    float f0 = 0.0f;
    float f1 = 0.0f;
    float f2 = 0.0f;
    float f3 = 0.0f;
    float f4 = 0.0f;
    float f12 = 0.0f;
    float f13 = 0.0f;
    uint32_t at = 0;
    uint32_t v0 = 0;
    uint32_t v1 = 0;
    uint64_t s0 = 0;
    uint64_t s1 = 0;
    uint32_t fcr31 = 0;
    uint8_t a2fr[32];
    (s0 = (uint64_t)((uint64_t)a0 + (uint64_t)0ULL));
    (s1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)s0, 568))));
    a2::frSt<uint32_t>(a2fr, 24, a1_bits(f20));
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)s0, 612))));
    at = 0x3F800000u;
    { uint32_t bits = at; std::memcpy(&f20, &bits, sizeof(bits)); }
    A1_W32B((uint32_t)s0, 388, 0u);
    { float f = f20; uint32_t bits; std::memcpy(&bits, &f, sizeof(bits)); A1_W32B((uint32_t)s0, 608, bits); }
    f12 = ((float)(f20));
    (v0 = (uint32_t)((int32_t)((int32_t)A1_R32B((uint32_t)s0, 100))));
    { f13 = a1_flt(A1_R32B(v0, 32)); A3_CHARGE(); { const a1::RetVF a2_r = AI_UpdateVelocity(ram, rt, (uint32_t)a0, f12, f13); v0 = (uint32_t)a2_r.v0; f0 = a2_r.f0; } }
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)s0, 528))));
    A1_W32B((uint32_t)s0, 560, 0u);
    (v0 = (uint32_t)((int32_t)((int32_t)A1_R32B((uint32_t)s0, 100))));
    { uint32_t bits = A1_R32B((uint32_t)s0, 396); float f; std::memcpy(&f, &bits, sizeof(f)); f4 = f; }
    f0 = a1_flt(A1_R32B(v0, 164));
    fcr31 = (a1f::le(f4, f0)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
    { const bool c = (fcr31 & 0x800000u) == 0u; f13 = ((float)(f20)); if (c) goto L_13f3810; }
    { uint32_t bits = A1_R32B((uint32_t)s0, 744); float f; std::memcpy(&f, &bits, sizeof(f)); f0 = f; }
    at = 0x3F000000u;
    { uint32_t bits = at; std::memcpy(&f1, &bits, sizeof(bits)); }
    { uint32_t bits = A1_R32B((uint32_t)s0, 752); float f; std::memcpy(&f, &bits, sizeof(f)); f2 = f; }
    f0 = a1f::mul(f0, f1);
    { uint32_t bits = A1_R32B((uint32_t)s0, 748); float f; std::memcpy(&f, &bits, sizeof(f)); f3 = f; }
    f3 = a1f::mul(f3, f4);
    f0 = a1f::mul(f0, f4);
    f0 = a1f::mul(f0, f4);
    f2 = a1f::add(f2, f0);
    f2 = a1f::addFinite(f2, f3);
    { float f = f2; uint32_t bits; std::memcpy(&bits, &f, sizeof(bits)); A1_W32B((uint32_t)s0, 528, bits); }
    goto L_13f381c;
L_13f3810:;
    { { uint32_t bits = A1_R32B((uint32_t)s0, 560); float f; std::memcpy(&f, &bits, sizeof(f)); f12 = f; } A3_CHARGE(); { const a1::RetVF a2_r = AI_UpdateVelocity(ram, rt, (uint32_t)a0, f12, f13); v0 = (uint32_t)a2_r.v0; f0 = a2_r.f0; } }
    (v0 = (uint32_t)((int32_t)((int32_t)A1_R32B((uint32_t)s0, 100))));
L_13f381c:;
    f13 = a1_flt(A1_R32B(v0, 60));
    (a0 = (uint64_t)((uint64_t)s1 + (uint64_t)0ULL));
    { uint32_t bits = 0u; std::memcpy(&f20, &bits, sizeof(bits)); }
    { f12 = a1_flt(A1_R32B(v0, 64)); A3_CHARGE(); { const a1::RetVF a2_r = AI_UpdateVelocity(ram, rt, (uint32_t)a0, f12, f13); v0 = (uint32_t)a2_r.v0; f0 = a2_r.f0; } }
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)s0, 536))));
    { uint32_t bits = A1_R32B((uint32_t)s1, 0); float f; std::memcpy(&f, &bits, sizeof(f)); f13 = f; }
    { f12 = ((float)(f20)); A3_CHARGE(); { const a1::RetVF a2_r = AI_UpdateVelocity(ram, rt, (uint32_t)a0, f12, f13); v0 = (uint32_t)a2_r.v0; f0 = a2_r.f0; } }
    (v1 = (uint32_t)((int32_t)((int32_t)A1_R32B((uint32_t)s0, 100))));
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)s0, 572))));
    f12 = ((float)(f20));
    f20 = a1_flt(a2::frLd<uint32_t>(a2fr, 24));
    f13 = a1_flt(A1_R32B(v1, 68));
    { if (xo) { xo[0] = a1_bits(f12); xo[1] = a1_bits(f13); } return AI_UpdateVelocity(ram, rt, (uint32_t)a0, f12, f13); }
    __builtin_unreachable();
}

// gamecore_sub_013F3868 (0x13f3868): typed by tools/typed_lift2.py (round A3; frame calls, extra outputs xo[0]=f12 xo[1]=f13, frame 48 bytes, 2 slot accesses)
inline a1::RetVF gamecore_sub_013F3868(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint64_t a0, float f20, uint64_t *xo = nullptr)
{
    float f0 = 0.0f;
    float f1 = 0.0f;
    float f12 = 0.0f;
    float f13 = 0.0f;
    uint32_t v0 = 0;
    uint32_t v1 = 0;
    uint64_t s0 = 0;
    uint64_t s1 = 0;
    uint64_t s2 = 0;
    uint8_t a2fr[48];
    (s0 = (uint64_t)((uint64_t)a0 + (uint64_t)0ULL));
    (s1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)s0, 564))));
    (s2 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)s0, 568))));
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)s0, 572))));
    a2::frSt<uint32_t>(a2fr, 32, a1_bits(f20));
    { uint32_t bits = 0u; std::memcpy(&f20, &bits, sizeof(bits)); }
    (v0 = (uint32_t)((int32_t)((int32_t)A1_R32B((uint32_t)s0, 100))));
    f12 = ((float)(f20));
    { f13 = a1_flt(A1_R32B(v0, 68)); A3_CHARGE(); { const a1::RetVF a2_r = AI_UpdateVelocity(ram, rt, (uint32_t)a0, f12, f13); v0 = (uint32_t)a2_r.v0; f0 = a2_r.f0; } }
    (v1 = (uint32_t)((int32_t)((int32_t)A1_R32B((uint32_t)s0, 100))));
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)s0, 612))));
    f12 = a1_flt(A1_R32B(v1, 160));
    { float f = f12; uint32_t bits; std::memcpy(&bits, &f, sizeof(bits)); A1_W32B((uint32_t)s0, 608, bits); }
    { f13 = a1_flt(A1_R32B(v1, 156)); A3_CHARGE(); { const a1::RetVF a2_r = AI_UpdateVelocity(ram, rt, (uint32_t)a0, f12, f13); v0 = (uint32_t)a2_r.v0; f0 = a2_r.f0; } }
    { uint32_t bits = A1_R32B((uint32_t)s0, 608); float f; std::memcpy(&f, &bits, sizeof(f)); f1 = f; }
    (v0 = (uint32_t)((int32_t)((int32_t)A1_R32B((uint32_t)s0, 100))));
    (a0 = (uint64_t)((uint64_t)s1 + (uint64_t)0ULL));
    f0 = a1_flt(A1_R32B(v0, 20));
    f0 = a1f::mul(f0, f1);
    { float f = f0; uint32_t bits; std::memcpy(&bits, &f, sizeof(bits)); A1_W32B((uint32_t)s0, 560, bits); }
    f13 = a1_flt(A1_R32B(v0, 152));
    { f12 = a1_flt(A1_R32B(v0, 148)); A3_CHARGE(); { const a1::RetVF a2_r = AI_UpdateVelocity(ram, rt, (uint32_t)a0, f12, f13); v0 = (uint32_t)a2_r.v0; f0 = a2_r.f0; } }
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)s0, 528))));
    { uint32_t bits = A1_R32B((uint32_t)s1, 0); float f; std::memcpy(&f, &bits, sizeof(f)); f13 = f; }
    { { uint32_t bits = A1_R32B((uint32_t)s0, 560); float f; std::memcpy(&f, &bits, sizeof(f)); f12 = f; } A3_CHARGE(); { const a1::RetVF a2_r = AI_UpdateVelocity(ram, rt, (uint32_t)a0, f12, f13); v0 = (uint32_t)a2_r.v0; f0 = a2_r.f0; } }
    (v1 = (uint32_t)((int32_t)((int32_t)A1_R32B((uint32_t)s0, 100))));
    (a0 = (uint64_t)((uint64_t)s2 + (uint64_t)0ULL));
    f13 = a1_flt(A1_R32B(v1, 60));
    { f12 = a1_flt(A1_R32B(v1, 64)); A3_CHARGE(); { const a1::RetVF a2_r = AI_UpdateVelocity(ram, rt, (uint32_t)a0, f12, f13); v0 = (uint32_t)a2_r.v0; f0 = a2_r.f0; } }
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)s0, 536))));
    { uint32_t bits = A1_R32B((uint32_t)s2, 0); float f; std::memcpy(&f, &bits, sizeof(f)); f13 = f; }
    f12 = ((float)(f20));
    f20 = a1_flt(a2::frLd<uint32_t>(a2fr, 32));
    { if (xo) { xo[0] = a1_bits(f12); xo[1] = a1_bits(f13); } return AI_UpdateVelocity(ram, rt, (uint32_t)a0, f12, f13); }
    __builtin_unreachable();
}

// checkChangeWeaponTypeSwordToOther (0x13ff4a8): typed by tools/typed_lift2.py (round A3; frame calls, frame 16 bytes, 0 slot accesses)
inline uint64_t checkChangeWeaponTypeSwordToOther(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint64_t a0, uint64_t a1)
{
    uint64_t v0 = 0;
    (v0 = (uint64_t)(((int64_t)(int64_t)a0 < (int64_t)(int32_t)4) ? 1 : 0));
    if ((int64_t)a1 == (int64_t)a0) goto L_13ff4f8;
    { const bool c = (int64_t)v0 == 0; (v0 = (uint64_t)(((int64_t)(int64_t)a1 < (int64_t)(int32_t)4) ? 1 : 0)); if (c) goto L_13ff4e0; }
    if ((int32_t)a0 <= 0) goto L_13ff4e0;
    if ((int64_t)v0 == 0) goto L_13ff4d8;
    if ((int32_t)a1 > 0) goto L_13ff4fc;
L_13ff4d8:;
    { return a1_sx64(fadeout_weakpoint_effect(ram, rt)); }
L_13ff4e0:;
    if ((int64_t)v0 == 0) goto L_13ff4fc;
    if ((int32_t)a1 <= 0) goto L_13ff4fc;
    { A3_CHARGE(); v0 = a1_sx64(cancel_fadeout_weakpoint_effect(ram, rt)); }
L_13ff4f8:;
L_13ff4fc:;
    { return v0; }
    __builtin_unreachable();
}

// initWeapon (0x14072f8): typed by tools/typed_lift2.py (round A3; frame calls, frame 16 bytes, 0 slot accesses)
inline a1::RetVF initWeapon(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt)
{
    { return initWeaponSpriteStatus(ram, rt); }
    __builtin_unreachable();
}

// gamecore_sub_014140B8 (0x14140b8): typed by tools/typed_lift2.py (round A3; frame calls, extra outputs xo[0]=f20, frame 32 bytes, 2 slot accesses)
inline float gamecore_sub_014140B8(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint64_t a0, uint32_t a1, float f0, float f20, uint64_t *xo = nullptr)
{
    float f1 = 0.0f;
    float f12 = 0.0f;
    float f13 = 0.0f;
    float f14 = 0.0f;
    uint32_t at = 0;
    uint64_t s0 = 0;
    uint32_t s1 = 0;
    uint32_t fcr31 = 0;
    uint8_t a2fr[32];
    (s0 = (uint64_t)((uint64_t)a0 + (uint64_t)0ULL));
    a2::frSt<uint32_t>(a2fr, 24, a1_bits(f20));
    s1 = A1_R32(a1);
    { uint32_t bits = A1_R32B((uint32_t)s0, 44); float f; std::memcpy(&f, &bits, sizeof(f)); f12 = f; }
    { uint32_t bits = A1_R32B((uint32_t)s0, 488); float f; std::memcpy(&f, &bits, sizeof(f)); f13 = f; }
    { uint32_t bits = 0u; std::memcpy(&f20, &bits, sizeof(bits)); }
    { { uint32_t bits = A1_R32B((uint32_t)s0, 492); float f; std::memcpy(&f, &bits, sizeof(f)); f14 = f; } A3_CHARGE(); f0 = gamecore_sub_014144D0(ram, rt, f12, f13, f14); }
    { uint32_t bits = A1_R32B((uint32_t)s0, 48); float f; std::memcpy(&f, &bits, sizeof(f)); f12 = f; }
    { uint32_t bits = A1_R32B((uint32_t)s0, 496); float f; std::memcpy(&f, &bits, sizeof(f)); f14 = f; }
    { f13 = ((float)(f0)); A3_CHARGE(); f0 = gamecore_sub_014144F0(ram, rt, f12, f13, f14); }
    { uint32_t bits = A1_R32B((uint32_t)s0, 40); float f; std::memcpy(&f, &bits, sizeof(f)); f1 = f; }
    fcr31 = (a1f::le(f20, f1)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
    { const bool c = (fcr31 & 0x800000u) == 0u; { float f = f0; uint32_t bits; std::memcpy(&bits, &f, sizeof(bits)); A1_W32B((uint32_t)s0, 48, bits); } if (c) goto L_1414110; }
    f14 = a1_flt(A1_R32B(s1, 240));
    goto L_1414114;
L_1414110:;
    f14 = a1_flt(A1_R32B(s1, 244));
L_1414114:;
    at = 0x3C8E0000u;
    at = at | 0xFA35u;
    { uint32_t bits = at; std::memcpy(&f20, &bits, sizeof(bits)); }
    f13 = a1_flt(A1_R32B(s1, 236));
    { uint32_t bits = A1_R32B((uint32_t)s0, 32); float f; std::memcpy(&f, &bits, sizeof(f)); f12 = f; }
    { f13 = a1f::mul(f13, f20); A3_CHARGE(); f0 = gamecore_sub_014144D0(ram, rt, f12, f13, f14); }
    { uint32_t bits = A1_R32B((uint32_t)s0, 40); float f; std::memcpy(&f, &bits, sizeof(f)); f13 = f; }
    f14 = a1_flt(A1_R32B(s1, 248));
    f13 = a1f::mul(f13, f0);
    { uint32_t bits = A1_R32B((uint32_t)s0, 36); float f; std::memcpy(&f, &bits, sizeof(f)); f12 = f; }
    { f14 = a1f::mul(f14, f20); A3_CHARGE(); f0 = gamecore_sub_014144F0(ram, rt, f12, f13, f14); }
    { uint32_t bits = A1_R32B((uint32_t)s0, 48); float f; std::memcpy(&f, &bits, sizeof(f)); f1 = f; }
    { float f = f0; uint32_t bits; std::memcpy(&bits, &f, sizeof(bits)); A1_W32B((uint32_t)s0, 36, bits); }
    f1 = a1f::add(f1, f0);
    { float f = f1; uint32_t bits; std::memcpy(&bits, &f, sizeof(bits)); A1_W32B((uint32_t)s0, 56, bits); }
    f0 = a1_flt(A1_R32B(s1, 224));
    f0 = a1f::mul(f0, f20);
    fcr31 = (a1f::lt(f0, f1)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
    if ((fcr31 & 0x800000u) != 0u) { { float f = f0; uint32_t bits; std::memcpy(&bits, &f, sizeof(bits)); A1_W32B((uint32_t)s0, 56, bits); } goto L_141416c; }
L_141416c:;
    f0 = a1_flt(A1_R32B(s1, 228));
    { uint32_t bits = A1_R32B((uint32_t)s0, 56); float f; std::memcpy(&f, &bits, sizeof(f)); f1 = f; }
    f0 = a1f::mul(f0, f20);
    fcr31 = (a1f::lt(f1, f0)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
    if ((fcr31 & 0x800000u) != 0u) { { float f = f0; uint32_t bits; std::memcpy(&bits, &f, sizeof(bits)); A1_W32B((uint32_t)s0, 56, bits); } goto L_1414184; }
L_1414184:;
    f20 = a1_flt(a2::frLd<uint32_t>(a2fr, 24));
    { if (xo) { xo[0] = a1_bits(f20); } return f0; }
    __builtin_unreachable();
}

// is_view_angle_zoom_game_camera (0x1425518): typed by tools/typed_lift2.py (round A3; frame calls, frame 16 bytes, 0 slot accesses)
inline uint64_t is_view_angle_zoom_game_camera(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint64_t a0)
{
    uint64_t v0 = 0;
    uint32_t a1 = 0;
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)326 << 16))));
    (a1 = (uint32_t)((int32_t)((int32_t)A1_R32B((uint32_t)v0, 29304))));
    { (a0 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL)); A3_CHARGE(); v0 = a1_sx64(GAMEUTIL_is_push_or_down_1p_button(ram, rt, a0, a1)); }
    (v0 = (uint64_t)(v0 ^ (uint64_t)(uint16_t)1));
    (v0 = (uint64_t)(((uint64_t)v0 < (uint64_t)(int64_t)(int32_t)1) ? 1 : 0));
    { return v0; }
    __builtin_unreachable();
}

// SCRConditionInvisibilityCloak (0x1459a20): typed by tools/typed_lift2.py (round A3; frame calls, frame 16 bytes, 0 slot accesses)
inline uint64_t SCRConditionInvisibilityCloak(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt)
{
    uint32_t a0 = 0;
    a0 = 7u;
    { return _omakeSelectFlagChk(ram, rt, a1_sx64(a0)); }
    __builtin_unreachable();
}

// SCRConditionGetOmakeFlag (0x1459a38): typed by tools/typed_lift2.py (round A3; frame calls, frame 16 bytes, 0 slot accesses)
inline uint64_t SCRConditionGetOmakeFlag(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint64_t a1)
{
    uint64_t a0 = 0;
    (a0 = (uint64_t)((uint64_t)a1 + (uint64_t)0ULL));
    { return _omakeGetFlagChk(ram, rt, a0); }
    __builtin_unreachable();
}

// eventBgmAllClose (0x145aeb0): typed by tools/typed_lift2.py (round A3; loop frame calls, extra outputs xo[0]=f12 xo[1]=a1, frame 32 bytes, 0 slot accesses)
inline uint32_t eventBgmAllClose(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint64_t a1, uint64_t *xo = nullptr)
{
    float f12 = 0.0f;
    uint32_t at = 0;
    uint32_t v0 = 0;
    uint32_t a0 = 0;
    uint32_t s0 = 0;
    uint32_t s1 = 0;
    uint32_t s2 = 0;
    bool a3xw0 = false;
    bool a3xw1 = false;
    v0 = 0x1200000u;
    s0 = v0 + 0x1358u;
    s1 = 1u;
    s2 = 1u;
L_145aed8:;
    v0 = A1_R32(s0);
    if (v0 == 0) { s1 = s1 - 1u; goto L_145af0c; }
    v0 = A1_R32B(s0, 28);
    if (v0 != 0) { s1 = s1 - 1u; goto L_145af0c; }
    a0 = A1_R32B(s0, 24);
    at = 0x40A00000u;
    f12 = a1_flt(at); a3xw0 = true;
    { (a1 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL)); a3xw1 = true; A3_CHARGE(); v0 = bgmStop(ram, rt, a0, a1, f12); }
    A1_W32B(s0, 28, s2);
    s1 = s1 - 1u;
L_145af0c:;
    { const bool c = (int32_t)s1 >= 0; s0 = s0 + 0x58u; if (c) { A3_BACK(); goto L_145aed8; } }
    { if (xo) { if (a3xw0) xo[0] = a1_bits(f12); if (a3xw1) xo[1] = a1; } return v0; }
    __builtin_unreachable();
}
