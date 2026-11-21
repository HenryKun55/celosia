// typed_a1.inc: round A1's typed functions (tools/typed_lift.py). Generated: do not edit. Included inside a namespace (typed_a1.h: namespace typed).
// SetHeapStartPoint (0x101a30): typed by tools/typed_lift.py (round A1)
inline uint32_t SetHeapStartPoint(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t a0, uint32_t gp)
{
    uint32_t v0 = 0;
    a0 = a0 + 0xFu;
    v0 = 0xFFFFFFF0u;
    a0 = a0 & v0;
    {
        A1_W32(gp - 0x7F6Cu, a0);
        return v0;
    }
}

// GetHeapCurrentPoint (0x101a48): typed by tools/typed_lift.py (round A1)
inline uint32_t GetHeapCurrentPoint(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t gp)
{
    uint32_t v0 = 0;
    {
        v0 = A1_R32(gp - 0x7F6Cu);
        return v0;
    }
}

// LoaderSysCheckCDBootMode (0x102fe0): typed by tools/typed_lift.py (round A1)
inline uint32_t LoaderSysCheckCDBootMode(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt)
{
    uint32_t v0 = 0;
    {
        v0 = 2u;
        return v0;
    }
}

// LoaderSysSendAbort (0x1032f0): typed by tools/typed_lift.py (round A1)
inline void LoaderSysSendAbort(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt)
{
    {
        
        return;
    }
}

// LoaderSysFlushPrint (0x1032f8): typed by tools/typed_lift.py (round A1)
inline void LoaderSysFlushPrint(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt)
{
    {
        
        return;
    }
}

// LoaderSysGetBootArg (0x103ca8): typed by tools/typed_lift.py (round A1)
inline uint32_t LoaderSysGetBootArg(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt)
{
    uint32_t v0 = 0;
    v0 = 0x140000u;
    {
        v0 = v0 - 0x3070u;
        return v0;
    }
}

// LoaderSysHookPoint (0x103e90): typed by tools/typed_lift.py (round A1)
inline void LoaderSysHookPoint(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt)
{
    {
        
        return;
    }
}

// initmemprintf (0x104088): typed by tools/typed_lift.py (round A1)
inline void initmemprintf(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt)
{
    {
        
        return;
    }
}

// padSysGet (0x104e78): typed by tools/typed_lift.py (round A1)
inline uint32_t padSysGet(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t a0)
{
    uint32_t at = 0;
    uint32_t v0 = 0;
    v0 = a0 << 2;
    v0 = v0 + a0;
    v0 = v0 << 6;
    at = 0x140000u;
    at = at - 0x27C0u;
    v0 = at + v0;
    {
        return v0;
    }
}

// LoaderSysSetPowerOffCallBackFunc (0x104f58): typed by tools/typed_lift.py (round A1)
inline void LoaderSysSetPowerOffCallBackFunc(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t a0, uint32_t gp)
{
    {
        A1_W32(gp - 0x7C54u, a0);
        
        return;
    }
}

// SetLocate (0x106380): typed by tools/typed_lift.py (round A1)
inline void SetLocate(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t a0, uint32_t a1, uint32_t gp)
{
    A1_W32(gp - 0x7C04u, a0);
    {
        A1_W32(gp - 0x7C00u, a1);
        
        return;
    }
}

// PutChar (0x106b58): typed by tools/typed_lift.py (round A1)
inline uint32_t PutChar(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t a0, uint32_t a1, uint32_t gp)
{
    uint32_t at = 0;
    uint32_t v0 = 0;
    uint32_t v1 = 0;
    uint32_t a2 = 0;
    v0 = A1_R32(gp - 0x7C00u);
    a2 = A1_R32(gp - 0x7C04u);
    v1 = v0 << 4;
    v1 = v1 - v0;
    v1 = v1 << 2;
    v1 = v1 + a2;
    v1 = v1 << 3;
    at = 0x140000u;
    at = at - 0x1270u;
    v1 = at + v1;
    A1_W8B(v1, 4, (uint8_t)a1);
    {
        A1_W32(v1, a0);
        return v0;
    }
}

// sub_00107D90 (0x107d90): typed by tools/typed_lift.py (round A1)
inline uint64_t sub_00107D90(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint64_t a0)
{
    uint64_t v0 = 0;
    uint64_t v1 = 0;
    uint64_t a1 = 0;
    uint64_t a2 = 0;
    (a1 = (uint64_t)((uint64_t)a0 + (uint64_t)0ULL));
    (v0 = (uint64_t)(a1 << 1));
    (a2 = (uint64_t)(v0 >> (32 + 21)));
    (a2 = (uint64_t)((int64_t)(int64_t)a2 + (int64_t)(int32_t)4294966221));
    (v0 = (uint64_t)(((int64_t)(int64_t)a2 < (int64_t)(int32_t)4294967243) ? 1 : 0));
    {
        const bool c = (int64_t)v0 == 0;
        (v0 = (uint64_t)(((int64_t)(int64_t)a2 < (int64_t)(int32_t)13) ? 1 : 0));
        if (!c) {
            {
                (v0 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL));
                return v0;
            }
        }
    }
    {
        const bool c = (int64_t)v0 != 0;
        (v0 = (uint64_t)(a1 << 12));
        if (!c) {
            {
                (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 9999))));
                return v0;
            }
        }
    }
    (v1 = (uint64_t)(0ULL | (uint64_t)(uint16_t)32768));
    (v1 = (uint64_t)(RC_U64X(v1) << (32 + 5)));
    (a1 = (uint64_t)(v0 >> 12));
    {
        const bool c = (int32_t)a2 >= 0;
        (a1 = (uint64_t)(a1 | v1));
        if (!c) {
            (a2 = (uint64_t)(0ULL - a2));
            (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 3))));
            (v1 = (uint64_t)((int64_t)(int64_t)a2 + (int64_t)(int32_t)4294967294));
            (v1 = (uint64_t)(v1 << (32 + 0)));
            (v1 = (uint64_t)((int64_t)v1 >> (32 + 0)));
            (a1 = (uint64_t)(a1 >> ((uint32_t)v1 & 0x3F)));
            (a0 = (uint64_t)(a1 & (uint64_t)(uint16_t)3));
            if ((int64_t)a0 != (int64_t)v0) {
                (a1 = (uint64_t)(a1 >> 2));
                goto L_107e14;
            }
            (v0 = (uint64_t)(a1 >> 2));
            (a1 = (uint64_t)((int64_t)(int64_t)v0 + (int64_t)(int32_t)1));
            goto L_107e14;
        }
    }
    (v0 = (uint64_t)(a2 << (32 + 0)));
    (v0 = (uint64_t)((int64_t)v0 >> (32 + 0)));
    (a1 = (uint64_t)(a1 << ((uint32_t)v0 & 0x3F)));
L_107e14:
    (v0 = (uint64_t)(a1 << (32 + 0)));
    {
        (v0 = (uint64_t)((int64_t)v0 >> (32 + 0)));
        return v0;
    }
}

// sub_00109268 (0x109268): typed by tools/typed_lift.py (round A1)
inline uint32_t sub_00109268(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t a0)
{
    uint32_t v0 = 0;
    uint32_t v1 = 0;
    v1 = A1_R32B(a0, 16);
    v0 = 0xFFFF0000u;
    v0 = v0 | 0xFFFEu;
    A1_W32B(a0, 24, 0u);
    v1 = v1 & v0;
    {
        A1_W32B(a0, 16, v1);
        return v0;
    }
}

// ps2___sceKernlGetEhSemaId (0x10fbe8): typed by tools/typed_lift.py (round A1)
inline uint32_t ps2___sceKernlGetEhSemaId(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt)
{
    uint32_t v0 = 0;
    uint32_t v1 = 0;
    v1 = 0x130000u;
    {
        v0 = A1_R32B(v1, 10660);
        return v0;
    }
}

// GetTimerPreScaleFactor (0x110640): typed by tools/typed_lift.py (round A1)
inline uint32_t GetTimerPreScaleFactor(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt)
{
    uint32_t v0 = 0;
    uint32_t v1 = 0;
    v0 = 0x130000u;
    v1 = A1_R32B(v0, 14624);
    {
        const bool c = (int32_t)v1 >= 0;
        v1 = 0x10000000u;
        if (!c) {
            v0 = 0x80000000u;
            {
                v0 = v0 | 0x8001u;
                return v0;
            }
        }
    }
    v1 = v1 | 0x1010u;
    v0 = A1_R32(v1);
    {
        v0 = v0 & 3u;
        return v0;
    }
}

// sub_00110998 (0x110998): typed by tools/typed_lift.py (round A1)
inline uint32_t sub_00110998(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t a0)
{
    uint32_t v0 = 0;
    uint32_t v1 = 0;
    v0 = A1_R32B(a0, 4);
    {
        const bool c = v0 == 0;
        v1 = A1_R32(a0);
        if (!c) {
            A1_W32(v0, v1);
            goto L_1109b4;
        }
    }
    v0 = 0x130000u;
    A1_W32B(v0, 14640, v1);
L_1109b4:
    if (v1 == 0) {
        A1_W32B(a0, 4, 0u);
    } else {
        v0 = A1_R32B(a0, 4);
        A1_W32B(v1, 4, v0);
        A1_W32B(a0, 4, 0u);
    }
    {
        v0 = v1;
        return v0;
    }
}

// iGetTimerUsedUnusedCounters (0x110ef0): typed by tools/typed_lift.py (round A1)
inline uint32_t iGetTimerUsedUnusedCounters(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint64_t a0, uint64_t a1)
{
    uint32_t v0 = 0;
    uint32_t v1 = 0;
    v0 = 0x130000u;
    v1 = v0 + 0x3918u;
    v0 = A1_R32B(v1, 8);
    if ((int32_t)v0 < 0) {
        v0 = 0x80000000u;
        {
            v0 = v0 | 0x8001u;
            return v0;
        }
    }
    if ((int64_t)a0 != 0) {
        v0 = A1_R32B(v1, 16);
        A1_W32B((uint32_t)a0, 0, v0);
    }
    if ((int64_t)a1 != 0) {
        v1 = A1_R32B(v1, 16);
        v0 = 0x80u;
        v0 = v0 - v1;
        A1_W32B((uint32_t)a1, 0, v0);
    }
    {
        v0 = 0u;
        return v0;
    }
}

// sceSifRemoveRebootNotifyHandler (0x111c50): typed by tools/typed_lift.py (round A1)
inline uint32_t sceSifRemoveRebootNotifyHandler(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint64_t a0)
{
    uint32_t v0 = 0;
    uint32_t v1 = 0;
    uint32_t a1 = 0;
    if ((int32_t)a0 >= 0) {
        v0 = 0x130000u;
    } else {
        v0 = 0x7FFF0000u;
        v1 = 0x150000u;
        v0 = v0 | 0xFFFFu;
        a1 = v1 - 0x5378u;
        (a0 = (uint64_t)(a0 & a1_sx64(v0)));
        v0 = 0x20u;
        goto L_111c80;
    }
    v1 = 0x130000u;
    a1 = A1_R32B(v0, 14648);
    v0 = A1_R32B(v1, 14652);
L_111c80:
    (v0 = (uint32_t)(((uint64_t)a0 < (uint64_t)a1_sx64(v0)) ? 1 : 0));
    {
        const bool c = v0 != 0;
        (v1 = (uint32_t)((int32_t)((int32_t)SLL32((uint32_t)a0, 4))));
        if (!c) {
            {
                v0 = 0xFFFFFF97u;
                return v0;
            }
        }
    }
    v0 = 0u;
    v1 = v1 + a1;
    {
        A1_W32(v1, 0u);
        return v0;
    }
}

// ioctl (0x111fc0): typed by tools/typed_lift.py (round A1)
inline uint32_t ioctl(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt)
{
    uint32_t v0 = 0;
    {
        v0 = 0xFFFFFFFFu;
        return v0;
    }
}

// isatty (0x112080): typed by tools/typed_lift.py (round A1)
inline uint32_t isatty(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt)
{
    uint32_t v0 = 0;
    {
        v0 = 1u;
        return v0;
    }
}

// sub_00112358 (0x112358): typed by tools/typed_lift.py (round A1)
inline uint32_t sub_00112358(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t a0)
{
    uint32_t v0 = 0;
    uint32_t v1 = 0;
    v0 = 0x150000u;
    v1 = v0 - 0x5140u;
    A1_W32(v0 - 0x5140u, a0);
    a0 = v1 + 0x10u;
    v0 = v1;
    A1_W32B(v1, 8, a0);
    A1_W32B(v1, 4, 0u);
    {
        A1_W32B(v1, 12, a0);
        return v0;
    }
}

// sub_00112380 (0x112380): typed by tools/typed_lift.py (round A1)
inline uint32_t sub_00112380(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint64_t a0)
{
    uint32_t v0 = 0;
    uint32_t v1 = 0;
    uint64_t a1 = 0;
    (a1 = (uint64_t)((uint64_t)a0 + (uint64_t)0ULL));
    (v0 = (uint32_t)((int32_t)((int32_t)A1_R32B((uint32_t)a1, 4))));
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)a1, 12))));
    (v1 = (uint32_t)((int32_t)((int32_t)A1_R32B((uint32_t)a1, 0))));
    v0 = v0 + 1u;
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)a0, 1))));
    A1_W32B((uint32_t)a1, 4, v0);
    v1 = v1 + 0x10u;
    (v1 = (uint32_t)((int32_t)((int32_t)ADD32((uint32_t)a1, v1))));
    {
        const bool c = (int64_t)a0 != (int64_t)(int32_t)v1;
        A1_W32B((uint32_t)a1, 12, (uint32_t)a0);
        if (!c) {
            (v0 = (uint32_t)((int32_t)((int32_t)ADD32((uint32_t)a1, 16))));
            A1_W32B((uint32_t)a1, 12, v0);
        }
    }
    {
        return v0;
    }
}

// sub_001123C0 (0x1123c0): typed by tools/typed_lift.py (round A1)
inline uint32_t sub_001123C0(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint64_t a0)
{
    uint32_t v0 = 0;
    uint32_t v1 = 0;
    uint64_t a1 = 0;
    (a1 = (uint64_t)((uint64_t)a0 + (uint64_t)0ULL));
    (v0 = (uint32_t)((int32_t)((int32_t)A1_R32B((uint32_t)a1, 4))));
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)a1, 8))));
    (v1 = (uint32_t)((int32_t)((int32_t)A1_R32B((uint32_t)a1, 0))));
    v0 = v0 - 1u;
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)a0, 1))));
    A1_W32B((uint32_t)a1, 4, v0);
    v1 = v1 + 0x10u;
    (v1 = (uint32_t)((int32_t)((int32_t)ADD32((uint32_t)a1, v1))));
    {
        const bool c = (int64_t)a0 != (int64_t)(int32_t)v1;
        A1_W32B((uint32_t)a1, 8, (uint32_t)a0);
        if (!c) {
            (v0 = (uint32_t)((int32_t)((int32_t)ADD32((uint32_t)a1, 16))));
            A1_W32B((uint32_t)a1, 8, v0);
        }
    }
    {
        return v0;
    }
}

// sub_001128A0 (0x1128a0): typed by tools/typed_lift.py (round A1)
inline uint32_t sub_001128A0(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t a0, uint32_t a1)
{
    uint32_t v0 = 0;
    uint32_t v1 = 0;
    uint32_t a2 = 0;
    v0 = A1_R32B(a0, 16);
    a2 = A1_R32B(a1, 28);
    v1 = A1_R32B(a0, 20);
    v0 = v0 << 2;
    v0 = v0 + a2;
    {
        A1_W32(v0, v1);
        return v0;
    }
}

// sub_001128C0 (0x1128c0): typed by tools/typed_lift.py (round A1)
inline uint32_t sub_001128C0(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t a0, uint32_t a1)
{
    uint32_t v0 = 0;
    v0 = A1_R32B(a0, 16);
    {
        A1_W32B(a1, 8, v0);
        return v0;
    }
}

// sub_001148B8 (0x1148b8): typed by tools/typed_lift.py (round A1)
inline uint64_t sub_001148B8(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint64_t a0)
{
    uint32_t v1 = 0;
    uint64_t v0 = 0;
    v1 = 7u;
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)SRL32((uint32_t)a0, 28))));
    {
        const bool c = (int64_t)v0 != (int64_t)(int32_t)v1;
        (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)4095 << 16))));
        if (!c) {
            v1 = 0x80000000u;
            (v0 = (uint64_t)(RC_U64X(v0) | (uint64_t)(uint16_t)65535));
            (a0 = (uint64_t)(a0 & RC_U64X(v0)));
            (a0 = (uint64_t)(a0 | a1_sx64(v1)));
        }
    }
    {
        (v0 = (uint64_t)((uint64_t)a0 + (uint64_t)0ULL));
        return v0;
    }
}

// _sceCdSC (0x117a00): typed by tools/typed_lift.py (round A1)
inline uint64_t _sceCdSC(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t a1, uint64_t a0)
{
    uint64_t v0 = 0;
    (v0 = (uint64_t)(0ULL | (uint64_t)(uint16_t)32769));
    {
        const bool c = (int64_t)a0 == (int64_t)v0;
        (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)19 << 16))));
        if (!c) {
            (v0 = (uint64_t)(0ULL | (uint64_t)(uint16_t)32771));
            {
                const bool c = (int64_t)a0 == (int64_t)v0;
                (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)19 << 16))));
                if (!c) {
                    (v0 = (uint64_t)(0ULL | (uint64_t)(uint16_t)32770));
                    {
                        const bool c = (int64_t)a0 == (int64_t)v0;
                        (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)19 << 16))));
                        if (!c) {
                            (v0 = (uint64_t)(0ULL | (uint64_t)(uint16_t)32772));
                            {
                                const bool c = (int64_t)a0 == (int64_t)v0;
                                (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)19 << 16))));
                                if (!c) {
                                    (v0 = (uint64_t)(0ULL | (uint64_t)(uint16_t)32785));
                                    {
                                        const bool c = (int64_t)a0 == (int64_t)v0;
                                        (v0 = (uint64_t)(0ULL | (uint64_t)(uint16_t)32787));
                                        if (!c) {
                                            {
                                                const bool c = (int64_t)a0 == (int64_t)v0;
                                                (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)19 << 16))));
                                                if (!c) {
                                                    (v0 = (uint64_t)(0ULL | (uint64_t)(uint16_t)32786));
                                                    {
                                                        const bool c = (int64_t)a0 == (int64_t)v0;
                                                        (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)19 << 16))));
                                                        if (!c) {
                                                            (v0 = (uint64_t)(0ULL | (uint64_t)(uint16_t)32788));
                                                            if ((int64_t)a0 != (int64_t)v0) {
                                                                (v0 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL));
                                                                goto L_117aa4;
                                                            }
                                                            (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)19 << 16))));
                                                            (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v0, 21952))));
                                                            goto L_117aa4;
                                                        }
                                                    }
                                                    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v0, 20608))));
                                                    goto L_117aa4;
                                                }
                                            }
                                            (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v0, 19520))));
                                            goto L_117aa4;
                                        }
                                    }
                                    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)19 << 16))));
                                    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)v0, 14956))));
                                    A1_W32B(a1, 0, (uint32_t)v0);
                                    goto L_117aa8;
                                }
                            }
                            (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v0, 19472))));
                            goto L_117aa4;
                        }
                    }
                    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v0, 15168))));
                    goto L_117aa4;
                }
            }
            (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v0, 15040))));
            goto L_117aa4;
        }
    }
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)v0, 14952))));
L_117aa4:
    A1_W32B(a1, 0, (uint32_t)v0);
L_117aa8:
    {
        (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 1))));
        return v0;
    }
}

// sub_00119AA0 (0x119aa0): typed by tools/typed_lift.py (round A1)
inline uint32_t sub_00119AA0(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t a0)
{
    uint32_t v0 = 0;
    uint32_t v1 = 0;
    v1 = A1_R32B(a0, 16);
    v0 = 0xFFFF0000u;
    v0 = v0 | 0xFFFEu;
    A1_W32B(a0, 24, 0u);
    v1 = v1 & v0;
    {
        A1_W32B(a0, 16, v1);
        return v0;
    }
}

// ps2__SceMcComGetSysInfo (0x120f18): typed by tools/typed_lift.py (round A1)
inline uint32_t ps2__SceMcComGetSysInfo(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t a0)
{
    uint32_t v0 = 0;
    uint32_t v1 = 0;
    uint64_t hi = 0;
    uint64_t lo = 0;
    v1 = 0x184u;
    v0 = 0x130000u;
    { int64_t result = (int64_t)(int32_t)a0 * (int64_t)(int32_t)v1; lo = (uint64_t)(int64_t)(int32_t)result; hi = (uint64_t)(int64_t)(int32_t)(result >> 32); (a0 = (uint32_t)((int32_t)((int32_t)result))); }
    v0 = v0 + 0x5638u;
    {
        v0 = a0 + v0;
        return v0;
    }
}

// sceNetcnfifSetFNoDecode (0x122448): typed by tools/typed_lift.py (round A1)
inline uint64_t sceNetcnfifSetFNoDecode(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t a0)
{
    uint32_t v1 = 0;
    uint64_t v0 = 0;
    v1 = 0x130000u;
    (v0 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL));
    {
        A1_W32B(v1, 22872, a0);
        return v0;
    }
}

// ps2___unpack_f (0x127048): typed by tools/typed_lift.py (round A1)
inline uint32_t ps2___unpack_f(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t a0, uint64_t a1)
{
    uint32_t v0 = 0;
    uint32_t v1 = 0;
    uint64_t a2 = 0;
    v1 = A1_R32(a0);
    a0 = 0x7F0000u;
    a0 = a0 | 0xFFFFu;
    (a2 = (uint64_t)((uint64_t)a1 + (uint64_t)0ULL));
    v0 = v1 >> 23;
    (a1 = (uint64_t)((int64_t)(int32_t)((int32_t)SRL32(v1, 31))));
    v0 = v0 & 0xFFu;
    v1 = v1 & a0;
    A1_W32B((uint32_t)a2, 4, (uint32_t)a1);
    (a1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(v0, 4294967169))));
    {
        const bool c = v0 != 0;
        a0 = v1 << 7;
        if (!c) {
            v0 = 2u;
            {
                A1_W32B((uint32_t)a2, 0, v0);
                return v0;
            }
        }
    }
    v0 = 0x40000000u;
    v1 = 3u;
    v0 = a0 | v0;
    A1_W32B((uint32_t)a2, 8, (uint32_t)a1);
    A1_W32B((uint32_t)a2, 0, v1);
    {
        A1_W32B((uint32_t)a2, 12, v0);
        return v0;
    }
}

// ps2___pack_d (0x1270d8): typed by tools/typed_lift.py (round A1)
inline uint64_t ps2___pack_d(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint64_t a0, uint64_t a2)
{
    uint64_t v0 = 0;
    uint64_t v1 = 0;
    uint64_t a1 = 0;
    uint64_t a3 = 0;
    uint64_t t0 = 0;
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)a0, 0))));
    (a3 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL));
    (a1 = (uint64_t)(A1_R64B((uint32_t)a0, 16)));
    (v0 = (uint64_t)(((uint64_t)RC_U64X(v1) < (uint64_t)(int64_t)(int32_t)2) ? 1 : 0));
    {
        const bool c = (int64_t)v0 == 0;
        (t0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)a0, 4))));
        if (!c) {
            (v0 = (uint64_t)(0ULL | (uint64_t)(uint16_t)32768));
            (v0 = (uint64_t)(RC_U64X(v0) << (32 + 4)));
            (a3 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 2047))));
            (a1 = (uint64_t)(a1 | v0));
            goto L_127154;
        }
    }
    (v0 = (uint64_t)(v1 ^ (uint64_t)(uint16_t)4));
    if ((int64_t)v0 == 0) {
        (a3 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 2047))));
    } else {
        (v0 = (uint64_t)(v1 ^ (uint64_t)(uint16_t)2));
        if ((int64_t)v0 == 0) {
            (a1 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL));
            goto L_127154;
        }
        if ((int64_t)a1 == 0) {
            goto L_127154;
        }
        (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)a0, 8))));
        (v0 = (uint64_t)(((int64_t)(int64_t)RC_U64X(a0) < (int64_t)(int32_t)4294966274) ? 1 : 0));
        if ((int64_t)v0 != 0) {
            (a1 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL));
            goto L_127154;
        }
        (v0 = (uint64_t)(((int64_t)(int64_t)RC_U64X(a0) < (int64_t)(int32_t)1024) ? 1 : 0));
        if ((int64_t)v0 != 0) {
            (a3 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)a0, 1023))));
            goto L_127150;
        }
        (a3 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 2047))));
    }
    (a1 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL));
    goto L_127154;
L_127150:
    (a1 = (uint64_t)(a1 >> 8));
L_127154:
    (v1 = (uint64_t)(0ULL | (uint64_t)(uint16_t)65520));
    (v1 = (uint64_t)(RC_U64X(v1) << (32 + 16)));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 4294967295))));
    (v0 = (uint64_t)(RC_U64X(v0) >> 12));
    (v0 = (uint64_t)(a1 & v0));
    (a2 = (uint64_t)(a2 & v1));
    (a2 = (uint64_t)(a2 | v0));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)32783 << 16))));
    (v0 = (uint64_t)(RC_U64X(v0) | (uint64_t)(uint16_t)65535));
    (v0 = (uint64_t)(RC_U64X(v0) << 16));
    (v0 = (uint64_t)(v0 | (uint64_t)(uint16_t)65535));
    (v0 = (uint64_t)(v0 << 16));
    (v0 = (uint64_t)(v0 | (uint64_t)(uint16_t)65535));
    (v1 = (uint64_t)(a3 & (uint64_t)(uint16_t)2047));
    (a2 = (uint64_t)(a2 & v0));
    (v1 = (uint64_t)(RC_U64X(v1) << (32 + 20)));
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 4294967295))));
    (a0 = (uint64_t)(RC_U64X(a0) >> 1));
    (a2 = (uint64_t)(a2 | v1));
    (v0 = (uint64_t)(t0 << (32 + 31)));
    (a2 = (uint64_t)(a2 & a0));
    {
        (v0 = (uint64_t)(a2 | v0));
        return v0;
    }
}

// ps2___pack_f (0x127390): typed by tools/typed_lift.py (round A1)
inline a1::RetVF ps2___pack_f(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint64_t a0, uint64_t a2)
{
    uint32_t v0 = 0;
    uint32_t v1 = 0;
    uint32_t a1 = 0;
    uint32_t t0 = 0;
    uint64_t a3 = 0;
    float f0 = 0.0f;
    (v0 = (uint32_t)((int32_t)((int32_t)A1_R32B((uint32_t)a0, 0))));
    (a3 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL));
    (a1 = (uint32_t)((int32_t)((int32_t)A1_R32B((uint32_t)a0, 12))));
    v0 = v0 ^ 2u;
    {
        const bool c = v0 == 0;
        (t0 = (uint32_t)((int32_t)((int32_t)A1_R32B((uint32_t)a0, 4))));
        if (!c) {
            {
                const bool c = a1 == 0;
                v0 = 0x7F0000u;
                if (c) {
                    goto L_1273f8;
                }
            }
            (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)a0, 8))));
            (v0 = (uint32_t)(((int64_t)(int64_t)RC_U64X(a0) < (int64_t)(int32_t)4294967170) ? 1 : 0));
            if (v0 != 0) {
                a1 = 0u;
                goto L_1273f4;
            }
            (v0 = (uint32_t)(((int64_t)(int64_t)RC_U64X(a0) < (int64_t)(int32_t)129) ? 1 : 0));
            if (v0 == 0) {
                (a3 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 255))));
            } else {
                v0 = 0x80u;
                {
                    const bool c = (int64_t)a0 != (int64_t)(int32_t)v0;
                    (a3 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)a0, 127))));
                    if (c) {
                        goto L_1273f0;
                    }
                }
                if ((int32_t)a1 >= 0) {
                    a1 = a1 >> 7;
                    goto L_1273f4;
                }
                (a3 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 255))));
            }
        }
    }
    a1 = 0u;
    goto L_1273f4;
L_1273f0:
    a1 = a1 >> 7;
L_1273f4:
    v0 = 0x7F0000u;
L_1273f8:
    v1 = 0xFF800000u;
    v0 = v0 | 0xFFFFu;
    (a2 = (uint64_t)(a2 & a1_sx64(v1)));
    v0 = a1 & v0;
    v1 = 0x807F0000u;
    (a2 = (uint64_t)(a2 | a1_sx64(v0)));
    v0 = 0x7FFF0000u;
    v1 = v1 | 0xFFFFu;
    (a0 = (uint64_t)(a3 & (uint64_t)(uint16_t)255));
    (a2 = (uint64_t)(a2 & a1_sx64(v1)));
    v1 = t0 << 31;
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)SLL32((uint32_t)a0, 23))));
    v0 = v0 | 0xFFFFu;
    (a2 = (uint64_t)(a2 | RC_U64X(a0)));
    (a2 = (uint64_t)(a2 & a1_sx64(v0)));
    (a2 = (uint64_t)(a2 | a1_sx64(v1)));
    { uint32_t bits = (uint32_t)a2; std::memcpy(&f0, &bits, sizeof(bits)); }
    {
        return a1::RetVF{(uint64_t)a1_sx64(v0), f0};
    }
}

// ps2___errno (0x127528): typed by tools/typed_lift.py (round A1)
inline uint32_t ps2___errno(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt)
{
    uint32_t v0 = 0;
    uint32_t v1 = 0;
    v1 = 0x130000u;
    {
        v0 = A1_R32B(v1, 23648);
        return v0;
    }
}

// sub_00128B00 (0x128b00): typed by tools/typed_lift.py (round A1)
inline uint64_t sub_00128B00(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt)
{
    uint64_t v0 = 0;
    {
        (v0 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL));
        return v0;
    }
}

// sub_0012D7F0 (0x12d7f0): typed by tools/typed_lift.py (round A1)
inline uint32_t sub_0012D7F0(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t a0, uint32_t a1, uint32_t a2, uint32_t a3)
{
    uint32_t v0 = 0;
    uint32_t v1 = 0;
    uint32_t t0 = 0;
    uint32_t t1 = 0;
    v0 = 0x130000u;
    v1 = 0x130000u;
    t0 = 0x130000u;
    t1 = 0x130000u;
    v0 = v0 - 0x5C0u;
    v1 = v1 - 0x558u;
    t0 = t0 - 0x4D8u;
    t1 = t1 - 0x470u;
    A1_W32B(a0, 84, a3);
    A1_W16B(a0, 12, (uint16_t)a1);
    A1_W16B(a0, 14, (uint16_t)a2);
    A1_W32B(a0, 32, v0);
    A1_W32B(a0, 36, v1);
    A1_W32B(a0, 40, t0);
    A1_W32B(a0, 44, t1);
    A1_W32(a0, 0u);
    A1_W32B(a0, 4, 0u);
    A1_W32B(a0, 8, 0u);
    A1_W32B(a0, 16, 0u);
    A1_W32B(a0, 20, 0u);
    A1_W32B(a0, 24, 0u);
    {
        A1_W32B(a0, 28, a0);
        return v0;
    }
}

// _localeconv_r (0x12e520): typed by tools/typed_lift.py (round A1)
inline uint32_t _localeconv_r(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt)
{
    uint32_t v0 = 0;
    v0 = 0x140000u;
    {
        v0 = v0 - 0x64F8u;
        return v0;
    }
}

// _hi0bits (0x12eaf8): typed by tools/typed_lift.py (round A1)
inline uint64_t _hi0bits(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint64_t a0)
{
    uint64_t v0 = 0;
    uint64_t v1 = 0;
    uint64_t a1 = 0;
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)65535 << 16))));
    (v0 = (uint64_t)(a0 & RC_U64X(v0)));
    {
        const bool c = (int64_t)v0 != 0;
        (a1 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL));
        if (!c) {
            (a1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 16))));
            (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)SLL32((uint32_t)a0, 16))));
        }
    }
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)65280 << 16))));
    (v0 = (uint64_t)(a0 & RC_U64X(v0)));
    {
        const bool c = (int64_t)v0 != 0;
        (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)61440 << 16))));
        if (!c) {
            (a1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)a1, 8))));
            (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)SLL32((uint32_t)a0, 8))));
        }
    }
    (v0 = (uint64_t)(a0 & RC_U64X(v0)));
    {
        const bool c = (int64_t)v0 != 0;
        (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)49152 << 16))));
        if (!c) {
            (a1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)a1, 4))));
            (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)SLL32((uint32_t)a0, 4))));
        }
    }
    (v0 = (uint64_t)(a0 & RC_U64X(v0)));
    if ((int64_t)v0 == 0) {
        (a1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)a1, 2))));
        (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)SLL32((uint32_t)a0, 2))));
    }
    {
        const bool c = (int32_t)a0 < 0;
        (v0 = (uint64_t)((uint64_t)RC_U64X(a1) + (uint64_t)0ULL));
        if (!c) {
            (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)16384 << 16))));
            (a1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)a1, 1))));
            (v1 = (uint64_t)(a0 & RC_U64X(v1)));
            {
                const bool c = (int64_t)v1 == 0;
                (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 32))));
                if (!c) {
                    (v0 = (uint64_t)((uint64_t)RC_U64X(a1) + (uint64_t)0ULL));
                }
            }
        }
    }
    {
        return v0;
    }
}

// _lo0bits (0x12eb78): typed by tools/typed_lift.py (round A1)
inline uint32_t _lo0bits(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t a0)
{
    uint32_t v0 = 0;
    uint32_t v1 = 0;
    uint32_t a1 = 0;
    a1 = A1_R32(a0);
    v0 = a1 & 7u;
    {
        const bool c = v0 == 0;
        v1 = a1 & 1u;
        if (!c) {
            {
                const bool c = v1 != 0;
                v0 = 0u;
                if (c) {
                    goto L_12ec2c;
                }
            }
            v0 = a1 & 2u;
            {
                const bool c = v0 == 0;
                v1 = a1 >> 1;
                if (!c) {
                    v0 = 1u;
                    {
                        A1_W32(a0, v1);
                        return v0;
                    }
                }
            }
            v1 = a1 >> 2;
            v0 = 2u;
            {
                A1_W32(a0, v1);
                return v0;
            }
        }
    }
    v0 = a1 & 0xFFFFu;
    {
        const bool c = v0 != 0;
        v1 = 0u;
        if (!c) {
            v1 = 0x10u;
            a1 = a1 >> 16;
        }
    }
    v0 = a1 & 0xFFu;
    {
        const bool c = v0 != 0;
        v0 = a1 & 0xFu;
        if (!c) {
            v1 = v1 + 8u;
            a1 = a1 >> 8;
            v0 = a1 & 0xFu;
        }
    }
    {
        const bool c = v0 != 0;
        v0 = a1 & 3u;
        if (!c) {
            v1 = v1 + 4u;
            a1 = a1 >> 4;
            v0 = a1 & 3u;
        }
    }
    {
        const bool c = v0 != 0;
        v0 = a1 & 1u;
        if (!c) {
            v1 = v1 + 2u;
            a1 = a1 >> 2;
            v0 = a1 & 1u;
        }
    }
    if (v0 != 0) {
        A1_W32(a0, a1);
    } else {
        a1 = a1 >> 1;
        v1 = v1 + 1u;
        {
            const bool c = a1 == 0;
            v0 = 0x20u;
            if (c) {
                goto L_12ec2c;
            }
        }
        A1_W32(a0, a1);
    }
    v0 = v1;
L_12ec2c:
    {
        return v0;
    }
}

// isnan (0x12fa00): typed by tools/typed_lift.py (round A1)
inline uint64_t isnan(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint64_t a0)
{
    uint64_t v0 = 0;
    uint64_t v1 = 0;
    uint64_t a1 = 0;
    uint64_t a2 = 0;
    (v0 = (uint64_t)((uint64_t)a0 + (uint64_t)0ULL));
    (a0 = (uint64_t)(v0 << (32 + 0)));
    (a0 = (uint64_t)((int64_t)a0 >> (32 + 0)));
    (v0 = (uint64_t)((int64_t)v0 >> (32 + 0)));
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)32767 << 16))));
    (a2 = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)32752 << 16))));
    (a1 = (uint64_t)((int64_t)(int32_t)((int32_t)SUB32(0u, (uint32_t)a0))));
    (v1 = (uint64_t)(RC_U64X(v1) | (uint64_t)(uint16_t)65535));
    (a0 = (uint64_t)(a0 | RC_U64X(a1)));
    (v0 = (uint64_t)(v0 & RC_U64X(v1)));
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)SRL32((uint32_t)a0, 31))));
    (v0 = (uint64_t)(v0 | RC_U64X(a0)));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)SUB32((uint32_t)a2, (uint32_t)v0))));
    {
        (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)SRL32((uint32_t)v0, 31))));
        return v0;
    }
}

// iosCdStStop (0x1ace28): typed by tools/typed_lift.py (round A1)
inline void iosCdStStop(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt)
{
    {
        
        return;
    }
}

// iosCdStSkip (0x1acf40): typed by tools/typed_lift.py (round A1)
inline uint64_t iosCdStSkip(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint64_t a0)
{
    uint64_t v0 = 0;
    uint64_t v1 = 0;
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)32 << 16))));
    (a0 = (uint64_t)(a0 << (32 + 0)));
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v1, 4294953040))));
    (a0 = (uint64_t)(a0 >> (32 + 0)));
    (v0 = (uint64_t)(A1_R64B((uint32_t)v1, 0)));
    (v0 = (uint64_t)((uint64_t)v0 + (uint64_t)a0));
    {
        A1_W64B((uint32_t)v1, 0, v0);
        return v0;
    }
}

// iosCdSetErrorCallback (0x1acfb8): typed by tools/typed_lift.py (round A1)
inline uint32_t iosCdSetErrorCallback(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t a0, uint32_t a1)
{
    uint32_t v0 = 0;
    uint32_t v1 = 0;
    v0 = 0x1E0000u;
    v1 = 0x1E0000u;
    A1_W32B(v0, 2880, a0);
    {
        A1_W32B(v1, 2884, a1);
        return v0;
    }
}

// iosCdSetMode (0x1acfd0): typed by tools/typed_lift.py (round A1)
inline uint32_t iosCdSetMode(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint64_t a0)
{
    uint32_t v0 = 0;
    uint32_t v1 = 0;
    uint32_t a1 = 0;
    v0 = 0x1F0000u;
    v1 = 1u;
    {
        const bool c = (int64_t)a0 != (int64_t)(int32_t)v1;
        a1 = v0 + 0x4810u;
        if (!c) {
            v0 = 0x1F0000u;
            v0 = v0 + 0x4800u;
            goto L_1acff8;
        }
    }
    v0 = 0x1F0000u;
    v0 = v0 + 0x4808u;
L_1acff8:
    {
        A1_W32(a1, v0);
        return v0;
    }
}

// iosInitDebPc (0x1ad140): typed by tools/typed_lift.py (round A1)
inline void iosInitDebPc(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt)
{
    {
        
        return;
    }
}

// iosStopPc (0x1ad148): typed by tools/typed_lift.py (round A1)
inline void iosStopPc(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt)
{
    {
        
        return;
    }
}

// iosDebPcVsyncCounterCheck (0x1ad150): typed by tools/typed_lift.py (round A1)
inline void iosDebPcVsyncCounterCheck(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt)
{
    {
        
        return;
    }
}

// iosGetCnt0Pc (0x1ad158): typed by tools/typed_lift.py (round A1)
inline uint64_t iosGetCnt0Pc(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt)
{
    uint64_t v0 = 0;
    {
        (v0 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL));
        return v0;
    }
}

// iosGetCnt1Pc (0x1ad160): typed by tools/typed_lift.py (round A1)
inline uint64_t iosGetCnt1Pc(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt)
{
    uint64_t v0 = 0;
    {
        (v0 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL));
        return v0;
    }
}

// InitDebPc (0x1ad168): typed by tools/typed_lift.py (round A1)
inline void InitDebPc(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt)
{
    {
        
        return;
    }
}

// iosGetClock (0x1ad1f0): typed by tools/typed_lift.py (round A1)
inline uint32_t iosGetClock(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t a0)
{
    uint32_t v0 = 0;
    uint64_t v1 = 0;
    v0 = 0x200000u;
    (v1 = (uint64_t)(A1_R64B(v0, -10088)));
    {
        A1_W64B(a0, 0, v1);
        return v0;
    }
}

// iosEE2IopEndChk (0x1ad948): typed by tools/typed_lift.py (round A1)
inline uint32_t iosEE2IopEndChk(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt)
{
    uint32_t v0 = 0;
    {
        v0 = 1u;
        return v0;
    }
}

// iosCreateIopMgr (0x1ad950): typed by tools/typed_lift.py (round A1)
inline void iosCreateIopMgr(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt)
{
    {
        
        return;
    }
}

// iosGetMallocPtn (0x1b0530): typed by tools/typed_lift.py (round A1)
inline uint32_t iosGetMallocPtn(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t a0)
{
    uint32_t v0 = 0;
    uint32_t v1 = 0;
    uint32_t a1 = 0;
    v1 = A1_R32(a0 - 4u);
    v0 = A1_R8B(v1, 11);
    a0 = A1_R8B(v1, 7);
    v0 = v0 << 16;
    a1 = A1_R8B(v1, 3);
    a0 = a0 << 8;
    v0 = v0 | a0;
    v0 = v0 | a1;
    {
        v0 = v0 << 2;
        return v0;
    }
}

// iosGetMallocRootPtnRemainSize (0x1b0558): typed by tools/typed_lift.py (round A1)
inline uint32_t iosGetMallocRootPtnRemainSize(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt)
{
    uint32_t v0 = 0;
    uint32_t v1 = 0;
    v1 = 0x1E0000u;
    v0 = A1_R32B(v1, 3056);
    {
        v0 = v0 << 2;
        return v0;
    }
}

// addMemoryDebugCallback (0x1b06a8): typed by tools/typed_lift.py (round A1)
inline uint32_t addMemoryDebugCallback(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t a0)
{
    uint32_t v0 = 0;
    v0 = 0x1E0000u;
    {
        A1_W32B(v0, 3064, a0);
        return v0;
    }
}

// getMemorySafetyLockFlag (0x1b06b8): typed by tools/typed_lift.py (round A1)
inline uint32_t getMemorySafetyLockFlag(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt)
{
    uint32_t v0 = 0;
    v0 = 0x1E0000u;
    {
        v0 = v0 + 0xBFCu;
        return v0;
    }
}

// kernel_sub_001B06C8 (0x1b06c8): typed by tools/typed_lift.py (round A1)
inline void kernel_sub_001B06C8(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt)
{
    // SYNC instruction - memory barrier // In recompiled code, we don't need explicit memory barriers
    {
        
        return;
    }
}

// iosInitMessageSystem (0x1b12b0): typed by tools/typed_lift.py (round A1)
inline void iosInitMessageSystem(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt)
{
    {
        
        return;
    }
}

// iosPadGetXZInputLwithThresholdIndepAxis (0x1b2438): typed by tools/typed_lift.py (round A1)
inline a1::RetVF iosPadGetXZInputLwithThresholdIndepAxis(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t a0, uint32_t a1, float f12)
{
    uint32_t at = 0;
    uint32_t v0 = 0;
    float f0 = 0.0f;
    float f1 = 0.0f;
    float f2 = 0.0f;
    float f3 = 0.0f;
    float f4 = 0.0f;
    uint32_t fcr31 = 0;
    v0 = A1_R8B(a1, 84);
    at = 0x3C000000u;
    at = at | 0x8080u;
    f3 = a1_flt(at);
    f0 = a1_flt(v0);
    { int32_t tmp; std::memcpy(&tmp, &f0, sizeof(tmp)); f0 = ((float)(int32_t)(tmp)); }
    at = 0x3F800000u;
    f2 = a1_flt(at);
    A1_W32B(a0, 4, 0u);
    f1 = a1_flt(A1_R32B(a0, 4));
    f0 = a1f::mul(f0, f3);
    f4 = a1f::subFinite(f0, f2);
    A1_W32(a0, a1_bits(f4));
    fcr31 = (a1f::lt(f4, f1)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
    v0 = A1_R8B(a1, 85);
    A1_W32B(a0, 12, a1_bits(f1));
    f0 = a1_flt(v0);
    { int32_t tmp; std::memcpy(&tmp, &f0, sizeof(tmp)); f0 = ((float)(int32_t)(tmp)); }
    f0 = a1f::mul(f0, f3);
    f0 = a1f::subFinite(f0, f2);
    {
        const bool c = (fcr31 & 0x800000u) == 0u;
        A1_W32B(a0, 8, a1_bits(f0));
        if (!c) {
            f0 = (-(float)(f4));
            fcr31 = (a1f::lt(f0, f12)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
            goto L_1b24a4;
        }
    }
    fcr31 = (a1f::lt(f4, f12)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
L_1b24a4:
    if ((fcr31 & 0x800000u) == 0u) {
        at = 0x3F800000u;
        f3 = a1_flt(at);
        f2 = a1_flt(A1_R32(a0));
        f0 = a1_flt(0u);
        f1 = a1f::sub(f3, f12);
        at = 0xBF800000u;
        f4 = a1_flt(at);
        fcr31 = (a1f::lt(f2, f0)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
        f1 = a1f::div(fcr31, f3, f1);
        if ((fcr31 & 0x800000u) == 0u) {
            f4 = ((float)(f3));
        } else {
            f0 = a1_flt(0u);
            fcr31 = (a1f::lt(f2, f0)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
        }
        if ((fcr31 & 0x800000u) != 0u) {
            f0 = (-(float)(f2));
        } else {
            f0 = a1f::sub(f2, f12);
            goto L_1b24fc;
        }
        f0 = a1f::sub(f0, f12);
L_1b24fc:
        f0 = a1f::mul(f0, f1);
        f1 = a1f::mul(f4, f0);
        goto L_1b250c;
    }
    f1 = a1_flt(0u);
L_1b250c:
    f2 = a1_flt(A1_R32B(a0, 8));
    f0 = a1_flt(0u);
    fcr31 = (a1f::lt(f2, f0)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
    {
        const bool c = (fcr31 & 0x800000u) == 0u;
        A1_W32(a0, a1_bits(f1));
        if (!c) {
            f0 = (-(float)(f2));
            fcr31 = (a1f::lt(f0, f12)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
            goto L_1b2534;
        }
    }
    fcr31 = (a1f::lt(f2, f12)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
L_1b2534:
    if ((fcr31 & 0x800000u) == 0u) {
        at = 0x3F800000u;
        f3 = a1_flt(at);
        f0 = a1_flt(0u);
        f1 = a1f::sub(f3, f12);
        at = 0xBF800000u;
        f4 = a1_flt(at);
        fcr31 = (a1f::lt(f2, f0)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
        f1 = a1f::div(fcr31, f3, f1);
        if ((fcr31 & 0x800000u) == 0u) {
            f4 = ((float)(f3));
        } else {
            f0 = a1_flt(0u);
            fcr31 = (a1f::lt(f2, f0)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
        }
        if ((fcr31 & 0x800000u) != 0u) {
            f0 = (-(float)(f2));
        } else {
            f0 = a1f::sub(f2, f12);
            goto L_1b258c;
        }
        f0 = a1f::sub(f0, f12);
L_1b258c:
        f0 = a1f::mul(f0, f1);
        f0 = a1f::mul(f4, f0);
        goto L_1b259c;
    }
    f0 = a1_flt(0u);
L_1b259c:
    {
        A1_W32B(a0, 8, a1_bits(f0));
        return a1::RetVF{(uint64_t)a1_sx64(v0), f0};
    }
}

// iosPadEntryTickFunc (0x1b27a0): typed by tools/typed_lift.py (round A1)
inline uint32_t iosPadEntryTickFunc(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t a0)
{
    uint32_t v0 = 0;
    v0 = 0x1E0000u;
    {
        A1_W32B(v0, 3496, a0);
        return v0;
    }
}

// iosCreatePrintMgr (0x1b29e8): typed by tools/typed_lift.py (round A1)
inline void iosCreatePrintMgr(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt)
{
    {
        
        return;
    }
}

// iosGetPrintfMode (0x1b29f0): typed by tools/typed_lift.py (round A1)
inline uint32_t iosGetPrintfMode(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt)
{
    uint32_t v0 = 0;
    v0 = 0x1E0000u;
    {
        v0 = v0 + 0xEA0u;
        return v0;
    }
}

// endSCEDemo (0x1b34f0): typed by tools/typed_lift.py (round A1)
inline void endSCEDemo(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt)
{
    {
        
        return;
    }
}

// Init_ShockVoiceSet (0x1b3c18): typed by tools/typed_lift.py (round A1)
inline uint32_t Init_ShockVoiceSet(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t a0, uint32_t a1)
{
    uint32_t v0 = 0;
    uint32_t v1 = 0;
    A1_W32(a0, a1);
    v0 = A1_R16B(a1, 10);
    v0 = v0 << 2;
    v0 = a1 + v0;
    A1_W32B(a0, 12, v0);
    v1 = A1_R16B(a1, 2);
    v1 = v1 << 2;
    v1 = a1 + v1;
    A1_W32B(a0, 4, v1);
    v0 = A1_R16B(a1, 6);
    v0 = v0 << 2;
    a1 = a1 + v0;
    {
        A1_W32B(a0, 8, a1);
        return v0;
    }
}

// Shock_SetShockVoiceSet (0x1b3d10): typed by tools/typed_lift.py (round A1)
inline uint64_t Shock_SetShockVoiceSet(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t a1, uint64_t a0)
{
    uint32_t v1 = 0;
    uint32_t a2 = 0;
    uint64_t v0 = 0;
    uint64_t a3 = 0;
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)30 << 16))));
    (a2 = (uint32_t)((int32_t)((int32_t)SLL32((uint32_t)a0, 2))));
    (v1 = (uint32_t)((int32_t)((int32_t)A1_R32B((uint32_t)v0, 3752))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B(v1, 0))));
    (v0 = (uint64_t)(((uint64_t)a0 < (uint64_t)RC_U64X(v0)) ? 1 : 0));
    {
        const bool c = (int64_t)v0 == 0;
        (a3 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 4294967295))));
        if (!c) {
            (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B(v1, 4))));
            (a3 = (uint64_t)((uint64_t)a0 + (uint64_t)0ULL));
            (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(a2, (uint32_t)v0))));
            A1_W32B((uint32_t)v0, 0, a1);
        }
    }
    {
        (v0 = (uint64_t)((uint64_t)a3 + (uint64_t)0ULL));
        return v0;
    }
}

// Init_Player (0x1b3d48): typed by tools/typed_lift.py (round A1)
inline uint32_t Init_Player(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t a0)
{
    uint32_t v0 = 0;
    uint32_t v1 = 0;
    uint32_t a1 = 0;
    uint32_t a2 = 0;
    v0 = 0x1B0000u;
    v1 = 0x1B0000u;
    a1 = v0 + 0x40D0u;
    v0 = 0x200000u;
    v1 = v1 + 0x4118u;
    a2 = v0 + 0x21E8u;
    {
        const bool c = a1 == 0;
        A1_W32(a0, 0u);
        if (!c) {
            A1_W32B(a0, 4, a1);
            goto L_1b3d7c;
        }
    }
    v0 = 0x1B0000u;
    v0 = v0 + 0x40C8u;
    A1_W32B(a0, 4, v0);
L_1b3d7c:
    A1_W32B(a0, 12, a2);
    {
        A1_W32B(a0, 8, v1);
        return v0;
    }
}

// Init_Controler (0x1b3d88): typed by tools/typed_lift.py (round A1)
inline void Init_Controler(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t a0)
{
    A1_W16(a0, (uint16_t)0u);
    {
        A1_W16B(a0, 2, (uint16_t)0u);
        
        return;
    }
}

// kernel_sub_001B40C8 (0x1b40c8): typed by tools/typed_lift.py (round A1)
inline uint64_t kernel_sub_001B40C8(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt)
{
    uint64_t v0 = 0;
    {
        (v0 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL));
        return v0;
    }
}

// kernel_sub_001B4118 (0x1b4118): typed by tools/typed_lift.py (round A1)
inline void kernel_sub_001B4118(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t a0)
{
    {
        A1_W8(a0, (uint8_t)0u);
        
        return;
    }
}

// IosGetMergeDataFileName (0x1b5f18): typed by tools/typed_lift.py (round A1)
inline uint32_t IosGetMergeDataFileName(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt)
{
    uint32_t v0 = 0;
    v0 = 0x1E0000u;
    {
        v0 = v0 + 0x2780u;
        return v0;
    }
}

// GetSymbolByIndex (0x1b6798): typed by tools/typed_lift.py (round A1)
inline uint32_t GetSymbolByIndex(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t a0, uint32_t a1)
{
    uint32_t v0 = 0;
    uint32_t v1 = 0;
    uint32_t a2 = 0;
    v0 = A1_R32B(a0, 28);
    v1 = A1_R32B(v0, 64);
    a0 = A1_R32B(v0, 40);
    a1 = a1 + v1;
    a2 = A1_R32B(v0, 44);
    a1 = a1 << 4;
    a1 = a1 + a0;
    v0 = A1_R32(a1);
    {
        v0 = a2 + v0;
        return v0;
    }
}

// isysGetXffWorkSize (0x1b6828): typed by tools/typed_lift.py (round A1)
inline uint32_t isysGetXffWorkSize(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t a0)
{
    uint32_t v0 = 0;
    uint32_t v1 = 0;
    uint32_t a1 = 0;
    uint32_t a2 = 0;
    v0 = A1_R8B(a0, 12);
    {
        const bool c = v0 != 0;
        a1 = v0 + 1u;
        if (!c) {
            a1 = 2u;
        }
    }
    v1 = A1_R32B(a0, 28);
    v0 = A1_R32B(a0, 36);
    a2 = v1 << 4;
    v0 = v0 - v1;
    v0 = v0 - a1;
    {
        const bool c = v0 == 0;
        a0 = v0 << 4;
        if (!c) {
            v0 = a0 + 0x33u;
            goto L_1b6864;
        }
    }
    v0 = 0u;
L_1b6864:
    if (v1 != 0) {
        v0 = v0 + a2;
        v0 = v0 + 0x33u;
    }
    {
        return v0;
    }
}

// isysGetModuleObjMallocSize (0x1b6880): typed by tools/typed_lift.py (round A1)
inline uint32_t isysGetModuleObjMallocSize(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t a0)
{
    uint32_t v0 = 0;
    uint32_t v1 = 0;
    v0 = A1_R32B(a0, 44);
    v1 = 0x40000000u;
    v0 = v0 & v1;
    {
        const bool c = v0 == 0;
        v0 = A1_R32B(a0, 28);
        if (!c) {
            v1 = A1_R32B(v0, 4);
            v0 = v1 - 4u;
            goto L_1b68a4;
        }
    }
    v0 = v0 - 4u;
L_1b68a4:
    a0 = A1_R32(v0);
    v1 = 0xFF0000u;
    v1 = v1 | 0xFFFFu;
    v0 = A1_R32(a0);
    v0 = v0 & v1;
    {
        v0 = v0 << 2;
        return v0;
    }
}

// GetExecModuleSize (0x1b69b8): typed by tools/typed_lift.py (round A1)
inline uint32_t GetExecModuleSize(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt)
{
    uint32_t v0 = 0;
    uint32_t v1 = 0;
    v1 = 0x1E0000u;
    {
        v0 = A1_R32B(v1, 3760);
        return v0;
    }
}

// getModuleSize (0x1b69c8): typed by tools/typed_lift.py (round A1)
inline uint32_t getModuleSize(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t a0)
{
    uint32_t v0 = 0;
    uint32_t v1 = 0;
    v1 = A1_R32B(a0, 28);
    {
        v0 = A1_R32B(v1, 20);
        return v0;
    }
}

// isysCreateExceptionThread (0x1b7b58): typed by tools/typed_lift.py (round A1)
inline void isysCreateExceptionThread(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt)
{
    {
        
        return;
    }
}

// iosCubicBezier (0x1b7b78): typed by tools/typed_lift.py (round A1)
inline float iosCubicBezier(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t a0, float f12)
{
    uint32_t at = 0;
    float f0 = 0.0f;
    float f1 = 0.0f;
    float f2 = 0.0f;
    float f3 = 0.0f;
    float f4 = 0.0f;
    float f5 = 0.0f;
    at = 0x3F800000u;
    f0 = a1_flt(at);
    f2 = a1f::mul(f12, f12);
    f4 = a1_flt(A1_R32(a0));
    f0 = a1f::sub(f0, f12);
    f5 = a1_flt(A1_R32B(a0, 4));
    f3 = a1_flt(A1_R32B(a0, 8));
    f2 = a1f::mul(f2, f3);
    f1 = a1f::addFinite(f0, f0);
    f0 = ((float)(f0) * (float)(f0));
    f1 = a1f::mul(f1, f12);
    f0 = a1f::mul(f0, f4);
    f1 = a1f::mul(f1, f5);
    f0 = a1f::addFinite(f0, f1);
    {
        f0 = a1f::add(f0, f2);
        return f0;
    }
}

// iosFourthBezier (0x1b7c20): typed by tools/typed_lift.py (round A1)
inline float iosFourthBezier(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t a0, float f12)
{
    uint32_t at = 0;
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
    at = 0x3F800000u;
    f4 = a1_flt(at);
    f6 = a1f::mul(f12, f12);
    at = 0x40400000u;
    f1 = a1_flt(at);
    f4 = a1f::sub(f4, f12);
    f8 = a1_flt(A1_R32(a0));
    f9 = a1_flt(A1_R32B(a0, 4));
    f7 = a1_flt(A1_R32B(a0, 8));
    f5 = a1f::mul(f6, f12);
    f3 = a1_flt(A1_R32B(a0, 12));
    f0 = ((float)(f4) * (float)(f4));
    f2 = ((float)(f4) * (float)(f1));
    f5 = a1f::mul(f5, f3);
    f1 = ((float)(f0) * (float)(f1));
    f0 = ((float)(f0) * (float)(f4));
    f2 = ((float)(f2) * (float)(f6));
    f1 = a1f::mul(f1, f12);
    f0 = a1f::mul(f0, f8);
    f2 = a1f::mul(f2, f7);
    f1 = a1f::mul(f1, f9);
    f0 = a1f::addFinite(f0, f1);
    f0 = a1f::addFinite(f0, f2);
    {
        f0 = a1f::add(f0, f5);
        return f0;
    }
}

// iosFloatModulo (0x1b9308): typed by tools/typed_lift.py (round A1)
inline float iosFloatModulo(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, float f12, float f13)
{
    uint32_t v0 = 0;
    float f0 = 0.0f;
    float f1 = 0.0f;
    uint32_t fcr31 = 0;
    bool df_v0 = false;
    f0 = a1_flt(0u);
    fcr31 = (a1f::eq(f13, f0)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
    if ((fcr31 & 0x800000u) == 0u) {
        f0 = a1f::div(fcr31, f12, f13);
        { int32_t tmp = a1f::cvtw(f0); std::memcpy(&f1, &tmp, sizeof(tmp)); }
        v0 = a1_bits(f1); df_v0 = true;
        f0 = a1_flt(v0);
        { int32_t tmp; std::memcpy(&tmp, &f0, sizeof(tmp)); f0 = ((float)(int32_t)(tmp)); }
        f0 = a1f::mul(f13, f0);
        f0 = a1f::sub(f12, f0);
    }
    {
        return f0;
    }
}

// iosIColToMvecSl (0x1ba428): typed by tools/typed_lift.py (round A1)
inline uint32_t iosIColToMvecSl(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t a0, uint32_t a1)
{
    uint32_t v0 = 0;
    uint32_t v1 = 0;
    uint32_t a2 = 0;
    v0 = a1 >> 8;
    v1 = a1 >> 16;
    a2 = a1 >> 24;
    a1 = a1 & 0xFFu;
    v0 = v0 & 0xFFu;
    v1 = v1 & 0xFFu;
    A1_W32B(a0, 12, a2);
    A1_W32(a0, a1);
    A1_W32B(a0, 4, v0);
    {
        A1_W32B(a0, 8, v1);
        return v0;
    }
}

// GetCurrentQuaternion (0x1bb868): typed by tools/typed_lift.py (round A1)
inline uint32_t GetCurrentQuaternion(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt)
{
    uint32_t at = 0;
    uint32_t v0 = 0;
    uint32_t v1 = 0;
    v1 = 0x1E0000u;
    v0 = A1_R32B(v1, 3936);
    v0 = v0 << 4;
    at = 0x200000u;
    at = at + 0x2AC0u;
    v0 = at + v0;
    {
        return v0;
    }
}

// GetLastQuaternion (0x1bb888): typed by tools/typed_lift.py (round A1)
inline uint32_t GetLastQuaternion(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt)
{
    uint32_t v0 = 0;
    uint32_t v1 = 0;
    uint32_t a0 = 0;
    v1 = 0x1E0000u;
    v0 = 0x200000u;
    a0 = A1_R32B(v1, 3936);
    v0 = v0 + 0x2AC0u;
    v0 = v0 - 0x10u;
    a0 = a0 << 4;
    {
        v0 = a0 + v0;
        return v0;
    }
}

// iosGetACos (0x1be2e8): typed by tools/typed_lift.py (round A1)
inline float iosGetACos(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t a0)
{
    uint32_t at = 0;
    float f0 = 0.0f;
    float f1 = 0.0f;
    float f2 = 0.0f;
    float f3 = 0.0f;
    float f4 = 0.0f;
    float f5 = 0.0f;
    float f6 = 0.0f;
    float f7 = 0.0f;
    f3 = a1_flt(a0);
    { int32_t tmp; std::memcpy(&tmp, &f3, sizeof(tmp)); f3 = ((float)(int32_t)(tmp)); }
    at = 0x2FFF0000u;
    at = at | 0xFFF5u;
    f0 = a1_flt(at);
    at = 0x3E2A0000u;
    at = at | 0xAAAAu;
    f2 = a1_flt(at);
    at = 0x3FC90000u;
    at = at | 0xFDAu;
    f4 = a1_flt(at);
    at = 0x3D990000u;
    at = at | 0x9999u;
    f5 = a1_flt(at);
    f3 = a1f::mul(f3, f0);
    at = 0x3D360000u;
    at = at | 0xDB6Du;
    f6 = a1_flt(at);
    at = 0x3CF80000u;
    at = at | 0xE38Du;
    f7 = a1_flt(at);
    f0 = ((float)(f3) * (float)(f3));
    f4 = a1f::sub(f4, f3);
    f0 = ((float)(f0) * (float)(f3));
    f1 = ((float)(f0) * (float)(f3));
    f0 = a1f::mul(f0, f2);
    f1 = ((float)(f1) * (float)(f3));
    f4 = a1f::subFinite(f4, f0);
    f2 = ((float)(f1) * (float)(f3));
    f1 = a1f::mul(f1, f5);
    f2 = ((float)(f2) * (float)(f3));
    f4 = a1f::subFinite(f4, f1);
    f0 = ((float)(f2) * (float)(f3));
    f2 = a1f::mul(f2, f6);
    f0 = ((float)(f0) * (float)(f3));
    f4 = a1f::subFinite(f4, f2);
    f0 = a1f::mul(f0, f7);
    {
        f0 = a1f::sub(f4, f0);
        return f0;
    }
}

// iosGetASin (0x1be4d8): typed by tools/typed_lift.py (round A1)
inline float iosGetASin(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt)
{
    float f0 = 0.0f;
    f0 = a1_flt(0u);
    {
        return f0;
    }
}

// iosGetATan2 (0x1be4e8): typed by tools/typed_lift.py (round A1)
inline float iosGetATan2(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt)
{
    float f0 = 0.0f;
    f0 = a1_flt(0u);
    {
        return f0;
    }
}

// resetArcFuncCall (0x1be4f8): typed by tools/typed_lift.py (round A1)
inline uint32_t resetArcFuncCall(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt)
{
    uint32_t v0 = 0;
    uint32_t v1 = 0;
    uint32_t a0 = 0;
    uint32_t a1 = 0;
    a1 = 0x1E0000u;
    a0 = 0x1E0000u;
    v1 = 0x1E0000u;
    v0 = 0x1E0000u;
    A1_W32B(v0, 3968, 0u);
    A1_W32B(v1, 3964, 0u);
    A1_W32B(a0, 3960, 0u);
    {
        A1_W32B(a1, 3956, 0u);
        return v0;
    }
}

// iosGetTableSin (0x1be520): typed by tools/typed_lift.py (round A1)
inline a1::RetVF iosGetTableSin(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t a0)
{
    uint32_t at = 0;
    uint32_t v0 = 0;
    uint64_t v1 = 0;
    float f0 = 0.0f;
    v0 = 0xC0000000u;
    a0 = a0 << 16;
    a0 = a0 + v0;
    a0 = (uint32_t)((int32_t)a0 >> 16);
    if ((int32_t)a0 < 0) {
        a0 = 0u - a0;
    } else {
    }
    v0 = (int32_t)a0 < 16384 ? 1u : 0u;
    {
        const bool c = v0 != 0;
        (v1 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL));
        if (!c) {
            v0 = 0x8000u;
            (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 1))));
            a0 = v0 - a0;
        }
    }
    v0 = a0 << 2;
    at = 0x200000u;
    at = at + v0;
    f0 = a1_flt(A1_R32B(at, 11968));
    if ((int64_t)v1 != 0) {
        f0 = (-(float)(f0));
    } else {
    }
    {
        return a1::RetVF{(uint64_t)a1_sx64(v0), f0};
    }
}

// iosDumpFVector (0x1be8e0): typed by tools/typed_lift.py (round A1)
inline void iosDumpFVector(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt)
{
    {
        
        return;
    }
}

// iosDumpIVector (0x1be8e8): typed by tools/typed_lift.py (round A1)
inline void iosDumpIVector(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt)
{
    {
        
        return;
    }
}

// iosGetScaleInverseVector (0x1be980): typed by tools/typed_lift.py (round A1)
inline float iosGetScaleInverseVector(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t a0, uint32_t a1)
{
    uint32_t at = 0;
    float f0 = 0.0f;
    float f1 = 0.0f;
    float f2 = 0.0f;
    uint32_t fcr31 = 0;
    f1 = a1_flt(A1_R32(a1));
    at = 0x3F800000u;
    f2 = a1_flt(at);
    f1 = a1f::div(fcr31, f2, f1);
    A1_W32(a0, a1_bits(f1));
    f0 = a1_flt(A1_R32B(a1, 4));
    f0 = a1f::div(fcr31, f2, f0);
    A1_W32B(a0, 4, a1_bits(f0));
    f1 = a1_flt(A1_R32B(a1, 8));
    f1 = a1f::div(fcr31, f2, f1);
    A1_W32B(a0, 8, a1_bits(f1));
    f0 = a1_flt(A1_R32B(a1, 12));
    f2 = a1f::div(fcr31, f2, f0);
    {
        A1_W32B(a0, 12, a1_bits(f2));
        return f0;
    }
}

// iosSetDCViewPort (0x1bf0b0): typed by tools/typed_lift.py (round A1)
inline uint64_t iosSetDCViewPort(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint64_t a0, uint64_t a1, uint64_t a2, uint64_t a3, uint64_t t0)
{
    uint64_t v0 = 0;
    uint64_t v1 = 0;
    uint64_t t1 = 0;
    uint64_t t2 = 0;
    uint64_t t3 = 0;
    uint64_t t4 = 0;
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)SRL32((uint32_t)t0, 31))));
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)SRL32((uint32_t)a3, 31))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)t0, (uint32_t)v0))));
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)a3, (uint32_t)v1))));
    (v0 = (uint64_t)((int64_t)(int32_t)(SRA32((int32_t)v0, 1))));
    (t1 = (uint64_t)((int64_t)(int32_t)((int32_t)SUB32(0u, (uint32_t)a2))));
    (t1 = (uint64_t)((int64_t)(int32_t)((int32_t)SUB32((uint32_t)t1, (uint32_t)v0))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)SUB32(0u, (uint32_t)a1))));
    (v1 = (uint64_t)((int64_t)(int32_t)(SRA32((int32_t)v1, 1))));
    (t3 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)a1, (uint32_t)a3))));
    (t2 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)a2, (uint32_t)t0))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)SUB32((uint32_t)v0, (uint32_t)v1))));
    (t1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)t1, 2048))));
    (t2 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)t2, 4294967295))));
    (t3 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)t3, 4294967295))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v0, 2048))));
    (t3 = (uint64_t)(RC_U64X(t3) << 16));
    (t1 = (uint64_t)((int64_t)(int32_t)((int32_t)SLL32((uint32_t)t1, 4))));
    (t2 = (uint64_t)(RC_U64X(t2) << (32 + 16)));
    (v1 = (uint64_t)(a2 << (32 + 0)));
    (v1 = (uint64_t)(v1 | t3));
    (t3 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 64))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)SLL32((uint32_t)v0, 4))));
    (t1 = (uint64_t)(RC_U64X(t1) << (32 + 0)));
    (t2 = (uint64_t)(a1 | t2));
    (t1 = (uint64_t)(RC_U64X(v0) | t1));
    (t2 = (uint64_t)(t2 | v1));
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 25))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 24))));
    (t4 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 65))));
    A1_W64B((uint32_t)a0, 128, t2);
    A1_W64B((uint32_t)a0, 104, RC_U64X(v0));
    A1_W64B((uint32_t)a0, 120, RC_U64X(v1));
    A1_W64B((uint32_t)a0, 96, t1);
    A1_W64B((uint32_t)a0, 136, RC_U64X(t3));
    A1_W64B((uint32_t)a0, 152, RC_U64X(t4));
    A1_W32B((uint32_t)a0, 372, (uint32_t)a1);
    A1_W32B((uint32_t)a0, 376, (uint32_t)a2);
    A1_W32B((uint32_t)a0, 380, (uint32_t)a3);
    A1_W32B((uint32_t)a0, 384, (uint32_t)t0);
    A1_W64B((uint32_t)a0, 112, t1);
    {
        A1_W64B((uint32_t)a0, 144, t2);
        return v0;
    }
}

// iosGetRootDC (0x1bf2d8): typed by tools/typed_lift.py (round A1)
inline uint32_t iosGetRootDC(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt)
{
    uint32_t v0 = 0;
    uint32_t v1 = 0;
    v1 = 0x1E0000u;
    {
        v0 = A1_R32B(v1, 4244);
        return v0;
    }
}

// iosGetCurrentDC (0x1bf308): typed by tools/typed_lift.py (round A1)
inline uint32_t iosGetCurrentDC(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt)
{
    uint32_t v0 = 0;
    uint32_t v1 = 0;
    v1 = 0x1E0000u;
    {
        v0 = A1_R32B(v1, 4240);
        return v0;
    }
}

// iosGetDCGroup (0x1bf420): typed by tools/typed_lift.py (round A1)
inline uint32_t iosGetDCGroup(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt)
{
    uint32_t v0 = 0;
    v0 = 0x210000u;
    {
        v0 = v0 + 0x32C8u;
        return v0;
    }
}

// kernel_sub_001BF430 (0x1bf430): typed by tools/typed_lift.py (round A1)
inline void kernel_sub_001BF430(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt)
{
    {
        
        return;
    }
}

// kernel_sub_001BF438 (0x1bf438): typed by tools/typed_lift.py (round A1)
inline void kernel_sub_001BF438(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt)
{
    {
        
        return;
    }
}

// iosDlGetTransAdrs (0x1bfb80): typed by tools/typed_lift.py (round A1)
inline uint32_t iosDlGetTransAdrs(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt)
{
    uint32_t v0 = 0;
    uint32_t v1 = 0;
    v0 = 0x210000u;
    v0 = v0 + 0x32D8u;
    v1 = A1_R32B(v0, 24);
    v0 = v0 + 8u;
    v1 = v1 << 2;
    v1 = v1 + v0;
    v0 = A1_R32(v1);
    {
        return v0;
    }
}

// iosGetPacketPtrPath1 (0x1bfba8): typed by tools/typed_lift.py (round A1)
inline uint32_t iosGetPacketPtrPath1(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt)
{
    uint32_t v0 = 0;
    uint32_t v1 = 0;
    uint32_t a0 = 0;
    uint32_t a1 = 0;
    bool df_v1 = false;
    a1 = 0x210000u;
    a0 = a1 + 0x32D8u;
    v0 = A1_R32B(a0, 16);
    v0 = v0 & 0xFu;
    if (v0 != 0) {
        v1 = A1_R32B(a0, 16); df_v1 = true;
        v0 = A1_R32B(a0, 16);
        v1 = v1 & 0xFu; df_v1 = true;
        v0 = v0 - v1;
        v0 = v0 + 0x10u;
        A1_W32B(a0, 16, v0);
    }
    v0 = A1_R32B(a0, 16);
    // PREF instruction (ignored)
    v1 = a1 + 0x32D8u; df_v1 = true;
    v0 = A1_R32B(v1, 16);
    {
        return v0;
    }
}

// iosSetPacketPtrPath1 (0x1bfbf0): typed by tools/typed_lift.py (round A1)
inline uint32_t iosSetPacketPtrPath1(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t a0)
{
    uint32_t v0 = 0;
    v0 = 0x210000u;
    v0 = v0 + 0x32D8u;
    A1_W32B(v0, 16, a0);
    {
        return v0;
    }
}

// iosDlChainHead (0x1bfc30): typed by tools/typed_lift.py (round A1)
inline uint64_t iosDlChainHead(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint64_t a0, uint64_t a1)
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
    (t1 = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)20480 << 16))));
    (a1 = (uint64_t)(a1 | RC_U64X(t1)));
    A1_W32B((uint32_t)a0, 28, 0u);
    A1_W64B((uint32_t)a0, 0, a1);
    (a1 = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)32768 << 16))));
    (a1 = (uint64_t)(RC_U64X(a1) << 16));
    (a1 = (uint64_t)(a1 | (uint64_t)(uint16_t)65535));
    (a1 = (uint64_t)(a1 << 16));
    (a1 = (uint64_t)(a1 | (uint64_t)(uint16_t)65535));
    A1_W32B((uint32_t)a0, 24, 0u);
    (v0 = (uint64_t)(RC_U64X(a0) << (32 + 0)));
    A1_W32B((uint32_t)a0, 12, 0u);
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 4294967295))));
    (v1 = (uint64_t)(RC_U64X(v1) << (32 + 1)));
    (v1 = (uint64_t)(v1 >> 1));
    A1_W32B((uint32_t)a0, 8, 0u);
    (v1 = (uint64_t)(v1 & v0));
    (t0 = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)8192 << 16))));
    (a2 = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)36863 << 16))));
    (a2 = (uint64_t)(RC_U64X(a2) | (uint64_t)(uint16_t)65535));
    (v0 = (uint64_t)(uint32_t)(A1_R32B((uint32_t)a3, 32)));
    (v0 = (uint64_t)(v0 << (32 + 0)));
    (v0 = (uint64_t)(v0 | RC_U64X(t0)));
    A1_W64B((uint32_t)a0, 16, v0);
    A1_W32B((uint32_t)a3, 32, (uint32_t)a0);
    (v0 = (uint64_t)(A1_R64B((uint32_t)a3, 0)));
    (v0 = (uint64_t)(v0 & a1));
    (v0 = (uint64_t)(v0 | v1));
    (v0 = (uint64_t)(v0 & RC_U64X(a2)));
    (v0 = (uint64_t)(v0 | RC_U64X(t1)));
    {
        A1_W64B((uint32_t)a3, 0, v0);
        return v0;
    }
}

// iosDlChainHeadCurrent (0x1bfdc0): typed by tools/typed_lift.py (round A1)
inline uint64_t iosDlChainHeadCurrent(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint64_t a0)
{
    uint64_t v0 = 0;
    uint64_t v1 = 0;
    uint64_t a1 = 0;
    uint64_t a2 = 0;
    uint64_t a3 = 0;
    uint64_t t0 = 0;
    uint64_t t1 = 0;
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)33 << 16))));
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v1, 13016))));
    (a2 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)v1, 28))));
    (a3 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v1, 8))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)v1, 24))));
    (a1 = (uint64_t)((int64_t)(int32_t)((int32_t)SLL32((uint32_t)a2, 1))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)SLL32((uint32_t)v0, 2))));
    (a1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)a1, (uint32_t)a2))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v0, (uint32_t)a3))));
    (a1 = (uint64_t)((int64_t)(int32_t)((int32_t)SLL32((uint32_t)a1, 4))));
    (a3 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)v0, 0))));
    (a2 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)v1, 16))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)v1, 16))));
    (a3 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)a3, (uint32_t)a1))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v0, 32))));
    A1_W32B((uint32_t)v1, 16, (uint32_t)v0);
    // PREF instruction (ignored)
    (a0 = (uint64_t)(a0 << (32 + 0)));
    (t1 = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)20480 << 16))));
    (a0 = (uint64_t)(a0 | RC_U64X(t1)));
    A1_W32B((uint32_t)a2, 28, 0u);
    A1_W64B((uint32_t)a2, 0, a0);
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)32768 << 16))));
    (a0 = (uint64_t)(RC_U64X(a0) << 16));
    (a0 = (uint64_t)(a0 | (uint64_t)(uint16_t)65535));
    (a0 = (uint64_t)(a0 << 16));
    (a0 = (uint64_t)(a0 | (uint64_t)(uint16_t)65535));
    A1_W32B((uint32_t)a2, 24, 0u);
    (v0 = (uint64_t)(RC_U64X(a2) << (32 + 0)));
    A1_W32B((uint32_t)a2, 12, 0u);
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 4294967295))));
    (v1 = (uint64_t)(RC_U64X(v1) << (32 + 1)));
    (v1 = (uint64_t)(v1 >> 1));
    A1_W32B((uint32_t)a2, 8, 0u);
    (v1 = (uint64_t)(v1 & v0));
    (t0 = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)8192 << 16))));
    (a1 = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)36863 << 16))));
    (a1 = (uint64_t)(RC_U64X(a1) | (uint64_t)(uint16_t)65535));
    (v0 = (uint64_t)(uint32_t)(A1_R32B((uint32_t)a3, 32)));
    (v0 = (uint64_t)(v0 << (32 + 0)));
    (v0 = (uint64_t)(v0 | RC_U64X(t0)));
    A1_W64B((uint32_t)a2, 16, v0);
    A1_W32B((uint32_t)a3, 32, (uint32_t)a2);
    (v0 = (uint64_t)(A1_R64B((uint32_t)a3, 0)));
    (v0 = (uint64_t)(v0 & a0));
    (v0 = (uint64_t)(v0 | v1));
    (v0 = (uint64_t)(v0 & RC_U64X(a1)));
    (v0 = (uint64_t)(v0 | RC_U64X(t1)));
    {
        A1_W64B((uint32_t)a3, 0, v0);
        return v0;
    }
}

// iosDlGetWorkBufId (0x1c0138): typed by tools/typed_lift.py (round A1)
inline uint32_t iosDlGetWorkBufId(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt)
{
    uint32_t v0 = 0;
    uint32_t v1 = 0;
    v1 = 0x210000u;
    v1 = v1 + 0x32D8u;
    v0 = A1_R32B(v1, 24);
    {
        return v0;
    }
}

// iosDlGetTransBufId (0x1c0150): typed by tools/typed_lift.py (round A1)
inline uint32_t iosDlGetTransBufId(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt)
{
    uint32_t v0 = 0;
    uint32_t v1 = 0;
    v1 = 0x210000u;
    v1 = v1 + 0x32D8u;
    v0 = A1_R32B(v1, 24);
    {
        v0 = v0 < 1u ? 1u : 0u;
        return v0;
    }
}

// iosDlGetEndAdrs (0x1c0168): typed by tools/typed_lift.py (round A1)
inline uint32_t iosDlGetEndAdrs(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt)
{
    uint32_t v0 = 0;
    uint32_t v1 = 0;
    uint32_t a0 = 0;
    uint32_t a1 = 0;
    v0 = 0x210000u;
    a1 = 0x1E0000u;
    v0 = v0 + 0x32D8u;
    a0 = A1_R32B(a1, 4268);
    v1 = A1_R32B(v0, 24);
    a0 = a0 << 4;
    v1 = v1 << 2;
    v1 = v1 + v0;
    v0 = A1_R32(v1);
    v0 = v0 + a0;
    {
        v0 = v0 - 0x1000u;
        return v0;
    }
}

// iosDlGetLayerNum (0x1c02c8): typed by tools/typed_lift.py (round A1)
inline uint32_t iosDlGetLayerNum(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt)
{
    uint32_t v0 = 0;
    uint32_t v1 = 0;
    v1 = 0x210000u;
    v1 = v1 + 0x32D8u;
    v0 = A1_R32B(v1, 20);
    {
        return v0;
    }
}

// iosDlCheckAdrs (0x1c03b8): typed by tools/typed_lift.py (round A1)
inline uint32_t iosDlCheckAdrs(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint64_t a0)
{
    uint32_t v0 = 0;
    uint32_t v1 = 0;
    uint32_t a1 = 0;
    uint32_t a2 = 0;
    v0 = 0x210000u;
    a2 = 0x1E0000u;
    v0 = v0 + 0x32D8u;
    a1 = A1_R32B(a2, 4268);
    v1 = A1_R32B(v0, 24);
    a1 = a1 << 4;
    v1 = v1 << 2;
    v1 = v1 + v0;
    v0 = A1_R32(v1);
    v0 = v0 + a1;
    v0 = v0 - 0x1000u;
    {
        (v0 = (uint32_t)(((uint64_t)a0 < (uint64_t)a1_sx64(v0)) ? 1 : 0));
        return v0;
    }
}

// iosDlCheckSize (0x1c03f0): typed by tools/typed_lift.py (round A1)
inline uint32_t iosDlCheckSize(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt)
{
    uint32_t v0 = 0;
    uint32_t v1 = 0;
    uint32_t a0 = 0;
    uint32_t a1 = 0;
    uint32_t a2 = 0;
    uint32_t a3 = 0;
    a3 = 0x210000u;
    v1 = 0x1E0000u;
    a2 = a3 + 0x32D8u;
    a1 = A1_R32B(v1, 4268);
    v0 = A1_R32B(a2, 24);
    a1 = a1 << 4;
    v0 = v0 << 2;
    v0 = v0 + a2;
    v1 = A1_R32(v0);
    a0 = A1_R32B(a2, 16);
    v1 = v1 + a1;
    a0 = a0 & 0xFu;
    {
        const bool c = a0 == 0;
        a1 = v1 - 0x1000u;
        if (!c) {
            v1 = A1_R32B(a2, 16);
            v0 = A1_R32B(a2, 16);
            v1 = v1 & 0xFu;
            v0 = v0 - v1;
            v0 = v0 + 0x10u;
            A1_W32B(a2, 16, v0);
        }
    }
    v0 = A1_R32B(a2, 16);
    // PREF instruction (ignored)
    v1 = a3 + 0x32D8u;
    v0 = A1_R32B(v1, 16);
    {
        v0 = a1 - v0;
        return v0;
    }
}

// iosAllocPacketPtrPath1UnCachedA (0x1c0480): typed by tools/typed_lift.py (round A1)
inline uint32_t iosAllocPacketPtrPath1UnCachedA(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t a0)
{
    uint32_t v0 = 0;
    uint32_t v1 = 0;
    uint32_t a1 = 0;
    uint32_t a2 = 0;
    a1 = 0x210000u;
    a2 = 0x30000000u;
    a1 = a1 + 0x32D8u;
    v0 = A1_R32B(a1, 16);
    v1 = A1_R32B(a1, 16);
    v0 = v0 | a2;
    v1 = v1 + a0;
    A1_W32B(a1, 16, v1);
    {
        return v0;
    }
}

// getDispScreenWidth (0x1c2018): typed by tools/typed_lift.py (round A1)
inline uint32_t getDispScreenWidth(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt)
{
    uint32_t v0 = 0;
    uint32_t v1 = 0;
    v1 = 0x210000u;
    {
        v0 = A1_R32B(v1, 14352);
        return v0;
    }
}

// getDispScreenHeight (0x1c2028): typed by tools/typed_lift.py (round A1)
inline uint32_t getDispScreenHeight(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt)
{
    uint32_t v0 = 0;
    uint32_t v1 = 0;
    v1 = 0x210000u;
    {
        v0 = A1_R32B(v1, 14356);
        return v0;
    }
}

// getDrawScreenWidth (0x1c2038): typed by tools/typed_lift.py (round A1)
inline uint32_t getDrawScreenWidth(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt)
{
    uint32_t v0 = 0;
    uint32_t v1 = 0;
    v1 = 0x210000u;
    {
        v0 = A1_R32B(v1, 14360);
        return v0;
    }
}

// getDrawScreenHeight (0x1c2048): typed by tools/typed_lift.py (round A1)
inline uint32_t getDrawScreenHeight(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt)
{
    uint32_t v0 = 0;
    uint32_t v1 = 0;
    v1 = 0x210000u;
    {
        v0 = A1_R32B(v1, 14364);
        return v0;
    }
}

// getDispBufferAddressTBP (0x1c20e8): typed by tools/typed_lift.py (round A1)
inline uint64_t getDispBufferAddressTBP(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt)
{
    uint64_t v0 = 0;
    {
        (v0 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL));
        return v0;
    }
}

// getDispBufferAddressFBP (0x1c20f0): typed by tools/typed_lift.py (round A1)
inline uint64_t getDispBufferAddressFBP(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt)
{
    uint64_t v0 = 0;
    {
        (v0 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL));
        return v0;
    }
}

// getZBufferAddressTBP (0x1c2158): typed by tools/typed_lift.py (round A1)
inline uint32_t getZBufferAddressTBP(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt)
{
    uint32_t v0 = 0;
    uint32_t v1 = 0;
    uint32_t a0 = 0;
    uint64_t hi = 0;
    uint64_t lo = 0;
    v1 = 0x210000u;
    v1 = v1 + 0x3810u;
    a0 = A1_R32B(v1, 4);
    v0 = A1_R32(v1);
    { int64_t result = (int64_t)(int32_t)v0 * (int64_t)(int32_t)a0; lo = (uint64_t)(int64_t)(int32_t)result; hi = (uint64_t)(int64_t)(int32_t)(result >> 32); (v0 = (uint32_t)((int32_t)((int32_t)result))); }
    {
        v0 = (uint32_t)((int32_t)v0 >> 6);
        return v0;
    }
}

// getZBufferAddressFBP (0x1c2178): typed by tools/typed_lift.py (round A1)
inline uint32_t getZBufferAddressFBP(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt)
{
    uint32_t v0 = 0;
    uint32_t v1 = 0;
    uint32_t a0 = 0;
    uint64_t hi = 0;
    uint64_t lo = 0;
    v1 = 0x210000u;
    v1 = v1 + 0x3810u;
    a0 = A1_R32B(v1, 4);
    v0 = A1_R32(v1);
    { int64_t result = (int64_t)(int32_t)v0 * (int64_t)(int32_t)a0; lo = (uint64_t)(int64_t)(int32_t)result; hi = (uint64_t)(int64_t)(int32_t)(result >> 32); (v0 = (uint32_t)((int32_t)((int32_t)result))); }
    {
        v0 = (uint32_t)((int32_t)v0 >> 11);
        return v0;
    }
}

// getScreenOffsetX (0x1c2208): typed by tools/typed_lift.py (round A1)
inline uint32_t getScreenOffsetX(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt)
{
    uint32_t v0 = 0;
    v0 = 0x210000u;
    {
        v0 = v0 + 0x38BCu;
        return v0;
    }
}

// getScreenOffsetY (0x1c2218): typed by tools/typed_lift.py (round A1)
inline uint32_t getScreenOffsetY(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt)
{
    uint32_t v0 = 0;
    v0 = 0x210000u;
    {
        v0 = v0 + 0x38C0u;
        return v0;
    }
}

// getAntiAlias (0x1c2228): typed by tools/typed_lift.py (round A1)
inline uint32_t getAntiAlias(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt)
{
    uint32_t v0 = 0;
    v0 = 0x210000u;
    {
        v0 = v0 + 0x38B0u;
        return v0;
    }
}

// getProgressiveMode (0x1c2238): typed by tools/typed_lift.py (round A1)
inline uint32_t getProgressiveMode(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt)
{
    uint32_t v0 = 0;
    v0 = 0x210000u;
    {
        v0 = v0 + 0x38B4u;
        return v0;
    }
}

// iosGetField (0x1c28b0): typed by tools/typed_lift.py (round A1)
inline uint64_t iosGetField(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt)
{
    uint64_t v0 = 0;
    uint64_t v1 = 0;
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)4608 << 16))));
    (v1 = (uint64_t)(RC_U64X(v1) | (uint64_t)(uint16_t)4096));
    (v0 = (uint64_t)(A1_R64B((uint32_t)v1, 0)));
    (v0 = (uint64_t)(v0 >> 13));
    (v0 = (uint64_t)(v0 & (uint64_t)(uint16_t)1));
    (v0 = (uint64_t)(RC_U64X(v0) << (32 + 0)));
    (v0 = (uint64_t)((int64_t)v0 >> (32 + 0)));
    {
        return v0;
    }
}

// iosSetSyncNoReturnFunc (0x1c2a68): typed by tools/typed_lift.py (round A1)
inline uint32_t iosSetSyncNoReturnFunc(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t a0)
{
    uint32_t v0 = 0;
    uint32_t v1 = 0;
    v1 = 0x1E0000u;
    v0 = 1u;
    {
        A1_W32B(v1, 4384, a0);
        return v0;
    }
}

// iosWaitDMA (0x1c2b98): typed by tools/typed_lift.py (round A1)
inline uint64_t iosWaitDMA(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt)
{
    uint64_t v0 = 0;
    {
        (v0 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL));
        return v0;
    }
}

// getCRTCoffsetU (0x1c2be8): typed by tools/typed_lift.py (round A1)
inline uint32_t getCRTCoffsetU(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt)
{
    uint32_t v0 = 0;
    v0 = 0x1E0000u;
    {
        v0 = v0 + 0x1148u;
        return v0;
    }
}

// getCRTCoffsetV (0x1c2bf8): typed by tools/typed_lift.py (round A1)
inline uint32_t getCRTCoffsetV(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt)
{
    uint32_t v0 = 0;
    v0 = 0x1E0000u;
    {
        v0 = v0 + 0x114Cu;
        return v0;
    }
}

// iosSetIStartFunc (0x1c3720): typed by tools/typed_lift.py (round A1)
inline uint32_t iosSetIStartFunc(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t a0)
{
    uint32_t v0 = 0;
    uint32_t v1 = 0;
    v1 = 0x1E0000u;
    v0 = 1u;
    {
        A1_W32B(v1, 4452, a0);
        return v0;
    }
}

// iosSetGpFinishFunc (0x1c3730): typed by tools/typed_lift.py (round A1)
inline uint32_t iosSetGpFinishFunc(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t a0)
{
    uint32_t v0 = 0;
    uint32_t v1 = 0;
    v1 = 0x1E0000u;
    v0 = 1u;
    {
        A1_W32B(v1, 4460, a0);
        return v0;
    }
}

// iosSetIFinishFunc (0x1c3740): typed by tools/typed_lift.py (round A1)
inline uint32_t iosSetIFinishFunc(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t a0)
{
    uint32_t v0 = 0;
    uint32_t v1 = 0;
    v1 = 0x1E0000u;
    v0 = 1u;
    {
        A1_W32B(v1, 4456, a0);
        return v0;
    }
}

// getIntrGpCount (0x1c3750): typed by tools/typed_lift.py (round A1)
inline uint64_t getIntrGpCount(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt)
{
    uint32_t v1 = 0;
    uint64_t v0 = 0;
    v1 = 0x1E0000u;
    (v1 = (uint32_t)((int32_t)((int32_t)ADD32(v1, 4440))));
    (v0 = (uint64_t)(A1_R64B(v1, 0)));
    {
        return v0;
    }
}

// iosSetDlTransFunc (0x1c3768): typed by tools/typed_lift.py (round A1)
inline uint32_t iosSetDlTransFunc(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t a0)
{
    uint32_t v0 = 0;
    uint32_t v1 = 0;
    v1 = 0x1E0000u;
    v0 = 1u;
    {
        A1_W32B(v1, 4464, a0);
        return v0;
    }
}

// iosSetVSchedulerFunc (0x1c3778): typed by tools/typed_lift.py (round A1)
inline uint32_t iosSetVSchedulerFunc(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t a0)
{
    uint32_t v0 = 0;
    uint32_t v1 = 0;
    v1 = 0x1E0000u;
    v0 = 1u;
    {
        A1_W32B(v1, 4468, a0);
        return v0;
    }
}

// getSpriteDrawFirst (0x1c5b48): typed by tools/typed_lift.py (round A1)
inline uint32_t getSpriteDrawFirst(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt)
{
    uint32_t v0 = 0;
    v0 = 0x1E0000u;
    {
        v0 = v0 + 0x11A8u;
        return v0;
    }
}

// gsReglist (0x1c5b58): typed by tools/typed_lift.py (round A1)
inline void gsReglist(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt)
{
    {
        
        return;
    }
}

// isysSetObjImmediateDeleteFunc (0x1c7540): typed by tools/typed_lift.py (round A1)
inline void isysSetObjImmediateDeleteFunc(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t a0, uint32_t a1)
{
    {
        A1_W32B(a0, 24, a1);
        
        return;
    }
}

// isysInitGroup (0x1c7658): typed by tools/typed_lift.py (round A1)
inline void isysInitGroup(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t a0)
{
    A1_W16(a0, (uint16_t)0u);
    {
        A1_W16B(a0, 2, (uint16_t)0u);
        
        return;
    }
}

// isysObjDeleteLock (0x1c7680): typed by tools/typed_lift.py (round A1)
inline uint32_t isysObjDeleteLock(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t a0)
{
    uint32_t v0 = 0;
    uint32_t v1 = 0;
    uint32_t a1 = 0;
    a1 = A1_R32B(a0, 8);
    v1 = 0x10000u;
    v1 = v1 | 0xFFFFu;
    v0 = a1 >> 17;
    a1 = a1 & v1;
    v0 = v0 + 1u;
    v0 = v0 << 17;
    a1 = a1 | v0;
    {
        A1_W32B(a0, 8, a1);
        return v0;
    }
}

// isysGetLinkMax (0x1c79a8): typed by tools/typed_lift.py (round A1)
inline uint32_t isysGetLinkMax(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt)
{
    uint32_t v0 = 0;
    {
        v0 = 0x36B0u;
        return v0;
    }
}

// isysGetObjIdentifierByObj (0x1c79b0): typed by tools/typed_lift.py (round A1)
inline uint32_t isysGetObjIdentifierByObj(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t a0)
{
    uint32_t v0 = 0;
    uint32_t v1 = 0;
    v0 = A1_R8B(a0, 8);
    v1 = v0 << 2;
    v0 = (int32_t)v0 < 37 ? 1u : 0u;
    if (v0 == 0) {
        v0 = 0x1E0000u;
    } else {
        v0 = 0x1E0000u;
        v0 = v0 + v1;
        v0 = A1_R32B(v0, 4576);
        {
            return v0;
        }
    }
    {
        v0 = A1_R32B(v0, 4576);
        return v0;
    }
}

// iosGetISchedulerExecCnt (0x1c7fa0): typed by tools/typed_lift.py (round A1)
inline uint32_t iosGetISchedulerExecCnt(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt)
{
    uint32_t v0 = 0;
    uint32_t v1 = 0;
    v1 = 0x1E0000u;
    {
        v0 = A1_R32B(v1, 4904);
        return v0;
    }
}

// ischedRestart (0x1c7ff0): typed by tools/typed_lift.py (round A1)
inline uint32_t ischedRestart(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt)
{
    uint32_t v0 = 0;
    uint32_t v1 = 0;
    uint32_t a0 = 0;
    v0 = 0x270000u;
    v0 = v0 - 0x2660u;
    v1 = A1_R32(v0);
    v1 = v1 - 1u;
    A1_W32(v0, v1);
    a0 = A1_R32(v0);
    {
        const bool c = a0 != 0;
        v1 = 0x10000000u;
        if (!c) {
            v1 = v1 | 0x10u;
            v0 = A1_R32(v1);
            v0 = v0 | 0x80u;
            A1_W32(v1, v0);
        }
    }
    {
        return v0;
    }
}

// iosSetISchedulerSubthreadTask (0x1c8028): typed by tools/typed_lift.py (round A1)
inline uint32_t iosSetISchedulerSubthreadTask(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t a0)
{
    uint32_t v0 = 0;
    v0 = 0x1E0000u;
    {
        A1_W32B(v0, 4916, a0);
        return v0;
    }
}

// iosSetIVSyncMode (0x1c8038): typed by tools/typed_lift.py (round A1)
inline uint32_t iosSetIVSyncMode(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t a0)
{
    uint32_t v0 = 0;
    v0 = 0x1E0000u;
    {
        A1_W32B(v0, 4896, a0);
        return v0;
    }
}

// iosGetIVSyncMode (0x1c8048): typed by tools/typed_lift.py (round A1)
inline uint32_t iosGetIVSyncMode(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt)
{
    uint32_t v0 = 0;
    uint32_t v1 = 0;
    v1 = 0x1E0000u;
    {
        v0 = A1_R32B(v1, 4896);
        return v0;
    }
}

// vschedSetNextFrameForceGarbage (0x1c8058): typed by tools/typed_lift.py (round A1)
inline uint32_t vschedSetNextFrameForceGarbage(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt)
{
    uint32_t v0 = 0;
    uint32_t v1 = 0;
    v1 = 0x1E0000u;
    v0 = 1u;
    {
        A1_W32B(v1, 4892, v0);
        return v0;
    }
}

// GetGarbageTime (0x1c8068): typed by tools/typed_lift.py (round A1)
inline uint64_t GetGarbageTime(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt)
{
    uint32_t v1 = 0;
    uint64_t v0 = 0;
    v1 = 0x270000u;
    {
        (v0 = (uint64_t)(A1_R64B(v1, -9832)));
        return v0;
    }
}

// init_letter (0x1c84c0): typed by tools/typed_lift.py (round A1)
inline uint32_t init_letter(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t a0, uint32_t a1, uint32_t a2)
{
    uint32_t v0 = 0;
    v0 = 1u;
    A1_W32(a1, 0u);
    A1_W32(a2, 0u);
    {
        A1_W32B(a0, 32, v0);
        return v0;
    }
}

// is_hope_letter_type (0x1c84d8): typed by tools/typed_lift.py (round A1)
inline uint64_t is_hope_letter_type(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t a0, uint64_t a1)
{
    uint64_t v0 = 0;
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B(a0, 12))));
    (v0 = (uint64_t)(RC_U64X(v0) ^ a1));
    {
        (v0 = (uint64_t)(((uint64_t)v0 < (uint64_t)(int64_t)(int32_t)1) ? 1 : 0));
        return v0;
    }
}

// get_letter_contents_data (0x1c84e8): typed by tools/typed_lift.py (round A1)
inline uint32_t get_letter_contents_data(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t a0)
{
    uint32_t v0 = 0;
    {
        v0 = A1_R32B(a0, 28);
        return v0;
    }
}

// increment_letter_refcount (0x1c8500): typed by tools/typed_lift.py (round A1)
inline uint32_t increment_letter_refcount(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t a0)
{
    uint32_t v0 = 0;
    v0 = A1_R32(a0);
    v0 = v0 + 1u;
    {
        A1_W32(a0, v0);
        return v0;
    }
}

// is_letter_reach_time (0x1c8570): typed by tools/typed_lift.py (round A1)
inline a1::RetVF is_letter_reach_time(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t a0, float f12)
{
    uint32_t v0 = 0;
    float f0 = 0.0f;
    float f1 = 0.0f;
    uint32_t fcr31 = 0;
    f1 = a1_flt(A1_R32B(a0, 8));
    f0 = a1_flt(A1_R32B(a0, 4));
    fcr31 = (a1f::le(f0, f1)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
    {
        const bool c = (fcr31 & 0x800000u) != 0u;
        v0 = 1u;
        if (!c) {
            f0 = a1f::add(f1, f12);
            v0 = 0u;
            A1_W32B(a0, 8, a1_bits(f0));
        }
    }
    {
        return a1::RetVF{(uint64_t)a1_sx64(v0), f0};
    }
}

// is_init_letter_called (0x1c8598): typed by tools/typed_lift.py (round A1)
inline uint32_t is_init_letter_called(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t a0)
{
    uint32_t v0 = 0;
    {
        v0 = A1_R32B(a0, 32);
        return v0;
    }
}

// get_reference_letter_type (0x1c85a0): typed by tools/typed_lift.py (round A1)
inline uint32_t get_reference_letter_type(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t a0)
{
    uint32_t v0 = 0;
    {
        v0 = A1_R32B(a0, 16);
        return v0;
    }
}

// get_letter_receiver_obj_group (0x1c85a8): typed by tools/typed_lift.py (round A1)
inline uint32_t get_letter_receiver_obj_group(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t a0)
{
    uint32_t v0 = 0;
    {
        v0 = A1_R32B(a0, 20);
        return v0;
    }
}

// get_letter_receiver_data (0x1c85b0): typed by tools/typed_lift.py (round A1)
inline uint32_t get_letter_receiver_data(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t a0)
{
    uint32_t v0 = 0;
    {
        v0 = A1_R32B(a0, 24);
        return v0;
    }
}

// get_total_letter_in_post (0x1c88f8): typed by tools/typed_lift.py (round A1)
inline uint32_t get_total_letter_in_post(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t a0)
{
    uint32_t v0 = 0;
    {
        v0 = A1_R32(a0);
        return v0;
    }
}

// kernel_sub_001C8948 (0x1c8948): typed by tools/typed_lift.py (round A1)
inline void kernel_sub_001C8948(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t a0)
{
    A1_W32B(a0, 16, 0u);
    A1_W32(a0, 0u);
    A1_W32B(a0, 4, 0u);
    A1_W32B(a0, 8, 0u);
    {
        A1_W32B(a0, 12, 0u);
        
        return;
    }
}

// constructor_letter_type (0x1c8ad0): typed by tools/typed_lift.py (round A1)
inline uint32_t constructor_letter_type(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t a0)
{
    uint32_t v0 = 0;
    v0 = 2u;
    A1_W32B(a0, 20, 0u);
    A1_W32(a0, v0);
    A1_W32B(a0, 4, 0u);
    A1_W32B(a0, 8, 0u);
    A1_W32B(a0, 12, 0u);
    {
        A1_W32B(a0, 16, 0u);
        return v0;
    }
}

// get_letter_type (0x1c8b68): typed by tools/typed_lift.py (round A1)
inline uint32_t get_letter_type(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint64_t a0)
{
    uint32_t at = 0;
    uint32_t v0 = 0;
    (v0 = (uint32_t)((int32_t)((int32_t)SLL32((uint32_t)a0, 2))));
    (a0 = (uint64_t)(((uint64_t)a0 < (uint64_t)(int64_t)(int32_t)20) ? 1 : 0));
    if ((int64_t)a0 != 0) {
        at = 0x270000u;
        at = at + v0;
        v0 = A1_R32B(at, 2584);
        {
            return v0;
        }
    }
    {
        v0 = 0u;
        return v0;
    }
}

// iosGetDataSizeOfFont (0x1c90d0): typed by tools/typed_lift.py (round A1)
inline uint32_t iosGetDataSizeOfFont(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t a0)
{
    uint32_t v0 = 0;
    a0 = a0 << 2;
    v0 = 0x1E0000u;
    v0 = v0 + a0;
    v0 = A1_R32B(v0, 4936);
    {
        return v0;
    }
}

// iosGetFontDataWithJIS (0x1c9160): typed by tools/typed_lift.py (round A1)
inline uint64_t iosGetFontDataWithJIS(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint64_t a0, uint64_t a1, uint64_t a2)
{
    uint64_t v0 = 0;
    uint64_t v1 = 0;
    uint64_t a3 = 0;
    uint64_t t0 = 0;
    uint64_t hi = 0;
    uint64_t lo = 0;
    (v1 = (uint64_t)((int64_t)(int32_t)(SRA32((int32_t)a1, 8))));
    (a1 = (uint64_t)(a1 & (uint64_t)(uint16_t)255));
    (a2 = (uint64_t)((int64_t)(int32_t)((int32_t)SLL32((uint32_t)a2, 5))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)SLL32((uint32_t)v1, 1))));
    (t0 = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)39 << 16))));
    (t0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)t0, 2680))));
    (t0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)t0, (uint32_t)a2))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v0, (uint32_t)v1))));
    (a2 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)t0, 16))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)SLL32((uint32_t)v0, 4))));
    (a3 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)t0, 20))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)SUB32((uint32_t)v0, (uint32_t)v1))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)SLL32((uint32_t)v0, 1))));
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)t0, 8))));
    { int64_t result = (int64_t)(int32_t)a2 * (int64_t)(int32_t)a3; lo = (uint64_t)(int64_t)(int32_t)result; hi = (uint64_t)(int64_t)(int32_t)(result >> 32); (a2 = (uint64_t)((int64_t)(int32_t)((int32_t)result))); }
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v0, (uint32_t)a1))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v0, 4294964161))));
    { int64_t result = (int64_t)(int32_t)v0 * (int64_t)(int32_t)a2; lo = (uint64_t)(int64_t)(int32_t)result; hi = (uint64_t)(int64_t)(int32_t)(result >> 32); (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)result))); }
    {
        const bool c = (int64_t)a0 == 0;
        (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v1, (uint32_t)v0))));
        if (!c) {
            { uint32_t addr = ADD32((uint32_t)t0, 7); uint32_t aligned_addr = addr & ~7u; uint32_t offset = addr & 7u; uint64_t mem = A1_R64(aligned_addr); uint32_t shift = (7u - offset) << 3; uint64_t keepMask = (shift == 0) ? 0ull : ((1ull << shift) - 1ull); (v1 = (uint64_t)((RC_U64X(v1) & keepMask) | (mem << shift))); }
            { uint32_t addr = ADD32((uint32_t)t0, 0); uint32_t aligned_addr = addr & ~7u; uint32_t offset = addr & 7u; uint64_t mem = A1_R64(aligned_addr); uint32_t shift = offset << 3; uint64_t keepMask = (offset == 0) ? 0ull : (0xFFFFFFFFFFFFFFFFull << ((8u - offset) << 3)); (v1 = (uint64_t)((v1 & keepMask) | (mem >> shift))); }
            { uint32_t addr = ADD32((uint32_t)t0, 15); uint32_t aligned_addr = addr & ~7u; uint32_t offset = addr & 7u; uint64_t mem = A1_R64(aligned_addr); uint32_t shift = (7u - offset) << 3; uint64_t keepMask = (shift == 0) ? 0ull : ((1ull << shift) - 1ull); (a1 = (uint64_t)((RC_U64X(a1) & keepMask) | (mem << shift))); }
            { uint32_t addr = ADD32((uint32_t)t0, 8); uint32_t aligned_addr = addr & ~7u; uint32_t offset = addr & 7u; uint64_t mem = A1_R64(aligned_addr); uint32_t shift = offset << 3; uint64_t keepMask = (offset == 0) ? 0ull : (0xFFFFFFFFFFFFFFFFull << ((8u - offset) << 3)); (a1 = (uint64_t)((a1 & keepMask) | (mem >> shift))); }
            { uint32_t addr = ADD32((uint32_t)t0, 23); uint32_t aligned_addr = addr & ~7u; uint32_t offset = addr & 7u; uint64_t mem = A1_R64(aligned_addr); uint32_t shift = (7u - offset) << 3; uint64_t keepMask = (shift == 0) ? 0ull : ((1ull << shift) - 1ull); (a2 = (uint64_t)((RC_U64X(a2) & keepMask) | (mem << shift))); }
            { uint32_t addr = ADD32((uint32_t)t0, 16); uint32_t aligned_addr = addr & ~7u; uint32_t offset = addr & 7u; uint64_t mem = A1_R64(aligned_addr); uint32_t shift = offset << 3; uint64_t keepMask = (offset == 0) ? 0ull : (0xFFFFFFFFFFFFFFFFull << ((8u - offset) << 3)); (a2 = (uint64_t)((a2 & keepMask) | (mem >> shift))); }
            { uint32_t addr = ADD32((uint32_t)t0, 31); uint32_t aligned_addr = addr & ~7u; uint32_t offset = addr & 7u; uint64_t mem = A1_R64(aligned_addr); uint32_t shift = (7u - offset) << 3; uint64_t keepMask = (shift == 0) ? 0ull : ((1ull << shift) - 1ull); (a3 = (uint64_t)((RC_U64X(a3) & keepMask) | (mem << shift))); }
            { uint32_t addr = ADD32((uint32_t)t0, 24); uint32_t aligned_addr = addr & ~7u; uint32_t offset = addr & 7u; uint64_t mem = A1_R64(aligned_addr); uint32_t shift = offset << 3; uint64_t keepMask = (offset == 0) ? 0ull : (0xFFFFFFFFFFFFFFFFull << ((8u - offset) << 3)); (a3 = (uint64_t)((a3 & keepMask) | (mem >> shift))); }
            { uint32_t addr = ADD32((uint32_t)a0, 7); uint32_t aligned_addr = addr & ~7u; uint32_t offset = addr & 7u; uint32_t shift = (7u - offset) << 3; uint64_t mask = 0xFFFFFFFFFFFFFFFFull >> shift; uint64_t old_data = A1_R64(aligned_addr); uint64_t val = v1; uint64_t new_data = (old_data & ~mask) | ((val >> shift) & mask); A1_W64(aligned_addr, new_data); }
            { uint32_t addr = ADD32((uint32_t)a0, 0); uint32_t aligned_addr = addr & ~7u; uint32_t offset = addr & 7u; uint32_t shift = offset << 3; uint64_t mask = 0xFFFFFFFFFFFFFFFFull << shift; uint64_t old_data = A1_R64(aligned_addr); uint64_t val = v1; uint64_t new_data = (old_data & ~mask) | ((val << shift) & mask); A1_W64(aligned_addr, new_data); }
            { uint32_t addr = ADD32((uint32_t)a0, 15); uint32_t aligned_addr = addr & ~7u; uint32_t offset = addr & 7u; uint32_t shift = (7u - offset) << 3; uint64_t mask = 0xFFFFFFFFFFFFFFFFull >> shift; uint64_t old_data = A1_R64(aligned_addr); uint64_t val = a1; uint64_t new_data = (old_data & ~mask) | ((val >> shift) & mask); A1_W64(aligned_addr, new_data); }
            { uint32_t addr = ADD32((uint32_t)a0, 8); uint32_t aligned_addr = addr & ~7u; uint32_t offset = addr & 7u; uint32_t shift = offset << 3; uint64_t mask = 0xFFFFFFFFFFFFFFFFull << shift; uint64_t old_data = A1_R64(aligned_addr); uint64_t val = a1; uint64_t new_data = (old_data & ~mask) | ((val << shift) & mask); A1_W64(aligned_addr, new_data); }
            { uint32_t addr = ADD32((uint32_t)a0, 23); uint32_t aligned_addr = addr & ~7u; uint32_t offset = addr & 7u; uint32_t shift = (7u - offset) << 3; uint64_t mask = 0xFFFFFFFFFFFFFFFFull >> shift; uint64_t old_data = A1_R64(aligned_addr); uint64_t val = a2; uint64_t new_data = (old_data & ~mask) | ((val >> shift) & mask); A1_W64(aligned_addr, new_data); }
            { uint32_t addr = ADD32((uint32_t)a0, 16); uint32_t aligned_addr = addr & ~7u; uint32_t offset = addr & 7u; uint32_t shift = offset << 3; uint64_t mask = 0xFFFFFFFFFFFFFFFFull << shift; uint64_t old_data = A1_R64(aligned_addr); uint64_t val = a2; uint64_t new_data = (old_data & ~mask) | ((val << shift) & mask); A1_W64(aligned_addr, new_data); }
            { uint32_t addr = ADD32((uint32_t)a0, 31); uint32_t aligned_addr = addr & ~7u; uint32_t offset = addr & 7u; uint32_t shift = (7u - offset) << 3; uint64_t mask = 0xFFFFFFFFFFFFFFFFull >> shift; uint64_t old_data = A1_R64(aligned_addr); uint64_t val = a3; uint64_t new_data = (old_data & ~mask) | ((val >> shift) & mask); A1_W64(aligned_addr, new_data); }
            { uint32_t addr = ADD32((uint32_t)a0, 24); uint32_t aligned_addr = addr & ~7u; uint32_t offset = addr & 7u; uint32_t shift = offset << 3; uint64_t mask = 0xFFFFFFFFFFFFFFFFull << shift; uint64_t old_data = A1_R64(aligned_addr); uint64_t val = a3; uint64_t new_data = (old_data & ~mask) | ((val << shift) & mask); A1_W64(aligned_addr, new_data); }
        }
    }
    {
        return v0;
    }
}

// iosGetFontIDWithJIS (0x1c9470): typed by tools/typed_lift.py (round A1)
inline uint32_t iosGetFontIDWithJIS(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t a0)
{
    uint32_t v0 = 0;
    uint32_t v1 = 0;
    v1 = (uint32_t)((int32_t)a0 >> 8);
    a0 = a0 & 0xFFu;
    v0 = v1 << 1;
    v0 = v0 + v1;
    v0 = v0 << 4;
    v0 = v0 - v1;
    v0 = v0 << 1;
    v0 = v0 + a0;
    {
        v0 = v0 - 0xC3Fu;
        return v0;
    }
}

// iosEntryLocaleFontNoLoad (0x1c9620): typed by tools/typed_lift.py (round A1)
inline uint64_t iosEntryLocaleFontNoLoad(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint64_t a0, uint64_t a1)
{
    uint64_t v0 = 0;
    uint64_t v1 = 0;
    uint64_t a2 = 0;
    uint64_t a3 = 0;
    uint64_t t0 = 0;
    uint64_t t1 = 0;
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)39 << 16))));
    (v0 = (uint64_t)((uint64_t)a0 + (uint64_t)0ULL));
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v1, 2680))));
    { uint32_t addr = ADD32((uint32_t)a1, 7); uint32_t aligned_addr = addr & ~7u; uint32_t offset = addr & 7u; uint64_t mem = A1_R64(aligned_addr); uint32_t shift = (7u - offset) << 3; uint64_t keepMask = (shift == 0) ? 0ull : ((1ull << shift) - 1ull); (a2 = (uint64_t)((a2 & keepMask) | (mem << shift))); }
    { uint32_t addr = ADD32((uint32_t)a1, 0); uint32_t aligned_addr = addr & ~7u; uint32_t offset = addr & 7u; uint64_t mem = A1_R64(aligned_addr); uint32_t shift = offset << 3; uint64_t keepMask = (offset == 0) ? 0ull : (0xFFFFFFFFFFFFFFFFull << ((8u - offset) << 3)); (a2 = (uint64_t)((a2 & keepMask) | (mem >> shift))); }
    { uint32_t addr = ADD32((uint32_t)a1, 15); uint32_t aligned_addr = addr & ~7u; uint32_t offset = addr & 7u; uint64_t mem = A1_R64(aligned_addr); uint32_t shift = (7u - offset) << 3; uint64_t keepMask = (shift == 0) ? 0ull : ((1ull << shift) - 1ull); (a3 = (uint64_t)((a3 & keepMask) | (mem << shift))); }
    { uint32_t addr = ADD32((uint32_t)a1, 8); uint32_t aligned_addr = addr & ~7u; uint32_t offset = addr & 7u; uint64_t mem = A1_R64(aligned_addr); uint32_t shift = offset << 3; uint64_t keepMask = (offset == 0) ? 0ull : (0xFFFFFFFFFFFFFFFFull << ((8u - offset) << 3)); (a3 = (uint64_t)((a3 & keepMask) | (mem >> shift))); }
    { uint32_t addr = ADD32((uint32_t)a1, 23); uint32_t aligned_addr = addr & ~7u; uint32_t offset = addr & 7u; uint64_t mem = A1_R64(aligned_addr); uint32_t shift = (7u - offset) << 3; uint64_t keepMask = (shift == 0) ? 0ull : ((1ull << shift) - 1ull); (t0 = (uint64_t)((t0 & keepMask) | (mem << shift))); }
    { uint32_t addr = ADD32((uint32_t)a1, 16); uint32_t aligned_addr = addr & ~7u; uint32_t offset = addr & 7u; uint64_t mem = A1_R64(aligned_addr); uint32_t shift = offset << 3; uint64_t keepMask = (offset == 0) ? 0ull : (0xFFFFFFFFFFFFFFFFull << ((8u - offset) << 3)); (t0 = (uint64_t)((t0 & keepMask) | (mem >> shift))); }
    { uint32_t addr = ADD32((uint32_t)a1, 31); uint32_t aligned_addr = addr & ~7u; uint32_t offset = addr & 7u; uint64_t mem = A1_R64(aligned_addr); uint32_t shift = (7u - offset) << 3; uint64_t keepMask = (shift == 0) ? 0ull : ((1ull << shift) - 1ull); (t1 = (uint64_t)((t1 & keepMask) | (mem << shift))); }
    { uint32_t addr = ADD32((uint32_t)a1, 24); uint32_t aligned_addr = addr & ~7u; uint32_t offset = addr & 7u; uint64_t mem = A1_R64(aligned_addr); uint32_t shift = offset << 3; uint64_t keepMask = (offset == 0) ? 0ull : (0xFFFFFFFFFFFFFFFFull << ((8u - offset) << 3)); (t1 = (uint64_t)((t1 & keepMask) | (mem >> shift))); }
    { uint32_t addr = ADD32((uint32_t)v1, 7); uint32_t aligned_addr = addr & ~7u; uint32_t offset = addr & 7u; uint32_t shift = (7u - offset) << 3; uint64_t mask = 0xFFFFFFFFFFFFFFFFull >> shift; uint64_t old_data = A1_R64(aligned_addr); uint64_t val = a2; uint64_t new_data = (old_data & ~mask) | ((val >> shift) & mask); A1_W64(aligned_addr, new_data); }
    { uint32_t addr = ADD32((uint32_t)v1, 0); uint32_t aligned_addr = addr & ~7u; uint32_t offset = addr & 7u; uint32_t shift = offset << 3; uint64_t mask = 0xFFFFFFFFFFFFFFFFull << shift; uint64_t old_data = A1_R64(aligned_addr); uint64_t val = a2; uint64_t new_data = (old_data & ~mask) | ((val << shift) & mask); A1_W64(aligned_addr, new_data); }
    { uint32_t addr = ADD32((uint32_t)v1, 15); uint32_t aligned_addr = addr & ~7u; uint32_t offset = addr & 7u; uint32_t shift = (7u - offset) << 3; uint64_t mask = 0xFFFFFFFFFFFFFFFFull >> shift; uint64_t old_data = A1_R64(aligned_addr); uint64_t val = a3; uint64_t new_data = (old_data & ~mask) | ((val >> shift) & mask); A1_W64(aligned_addr, new_data); }
    { uint32_t addr = ADD32((uint32_t)v1, 8); uint32_t aligned_addr = addr & ~7u; uint32_t offset = addr & 7u; uint32_t shift = offset << 3; uint64_t mask = 0xFFFFFFFFFFFFFFFFull << shift; uint64_t old_data = A1_R64(aligned_addr); uint64_t val = a3; uint64_t new_data = (old_data & ~mask) | ((val << shift) & mask); A1_W64(aligned_addr, new_data); }
    { uint32_t addr = ADD32((uint32_t)v1, 23); uint32_t aligned_addr = addr & ~7u; uint32_t offset = addr & 7u; uint32_t shift = (7u - offset) << 3; uint64_t mask = 0xFFFFFFFFFFFFFFFFull >> shift; uint64_t old_data = A1_R64(aligned_addr); uint64_t val = t0; uint64_t new_data = (old_data & ~mask) | ((val >> shift) & mask); A1_W64(aligned_addr, new_data); }
    { uint32_t addr = ADD32((uint32_t)v1, 16); uint32_t aligned_addr = addr & ~7u; uint32_t offset = addr & 7u; uint32_t shift = offset << 3; uint64_t mask = 0xFFFFFFFFFFFFFFFFull << shift; uint64_t old_data = A1_R64(aligned_addr); uint64_t val = t0; uint64_t new_data = (old_data & ~mask) | ((val << shift) & mask); A1_W64(aligned_addr, new_data); }
    { uint32_t addr = ADD32((uint32_t)v1, 31); uint32_t aligned_addr = addr & ~7u; uint32_t offset = addr & 7u; uint32_t shift = (7u - offset) << 3; uint64_t mask = 0xFFFFFFFFFFFFFFFFull >> shift; uint64_t old_data = A1_R64(aligned_addr); uint64_t val = t1; uint64_t new_data = (old_data & ~mask) | ((val >> shift) & mask); A1_W64(aligned_addr, new_data); }
    { uint32_t addr = ADD32((uint32_t)v1, 24); uint32_t aligned_addr = addr & ~7u; uint32_t offset = addr & 7u; uint32_t shift = offset << 3; uint64_t mask = 0xFFFFFFFFFFFFFFFFull << shift; uint64_t old_data = A1_R64(aligned_addr); uint64_t val = t1; uint64_t new_data = (old_data & ~mask) | ((val << shift) & mask); A1_W64(aligned_addr, new_data); }
    {
        A1_W32B((uint32_t)v1, 8, (uint32_t)a0);
        return v0;
    }
}

// iosGet2ByteCharCode (0x1c9708): typed by tools/typed_lift.py (round A1)
inline uint32_t iosGet2ByteCharCode(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t a0)
{
    uint32_t v0 = 0;
    uint32_t v1 = 0;
    v0 = A1_R8B(a0, 1);
    v1 = A1_R8(a0);
    v0 = v0 << 8;
    {
        v0 = v0 | v1;
        return v0;
    }
}

// _isysCreateSubMsgQueue (0x1c9720): typed by tools/typed_lift.py (round A1)
inline void _isysCreateSubMsgQueue(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t a0, uint32_t a1, uint32_t a2, uint32_t a3)
{
    A1_W32B(a0, 4, a1);
    A1_W32(a0, a2);
    A1_W32B(a0, 8, a3);
    A1_W32B(a0, 12, 0u);
    {
        A1_W32B(a0, 16, 0u);
        
        return;
    }
}

// isysGetSubThreadWorkSize (0x1ca4a0): typed by tools/typed_lift.py (round A1)
inline uint32_t isysGetSubThreadWorkSize(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t a0)
{
    uint32_t v0 = 0;
    {
        v0 = a0 + 0x33u;
        return v0;
    }
}

// isysDumpSubThreadPC (0x1ca5a0): typed by tools/typed_lift.py (round A1)
inline void isysDumpSubThreadPC(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt)
{
    {
        
        return;
    }
}

// isysDebugSetSubThreadSleepProtection (0x1ca5a8): typed by tools/typed_lift.py (round A1)
inline uint32_t isysDebugSetSubThreadSleepProtection(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t a0)
{
    uint32_t v0 = 0;
    v0 = 0x1E0000u;
    {
        A1_W32B(v0, 5460, a0);
        return v0;
    }
}

// sndSepSetDefaultParam (0x1cad30): typed by tools/typed_lift.py (round A1)
inline a1::RetVF sndSepSetDefaultParam(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t a0)
{
    uint32_t at = 0;
    uint32_t v0 = 0;
    uint32_t v1 = 0;
    uint32_t a1 = 0;
    uint32_t a2 = 0;
    float f0 = 0.0f;
    float f1 = 0.0f;
    float f2 = 0.0f;
    float f3 = 0.0f;
    at = 0x42FE0000u;
    f1 = a1_flt(at);
    v1 = 0xFFFFFFFFu;
    f0 = a1_flt(A1_R32B(a0, 28));
    v0 = 0xCu;
    A1_W32B(a0, 8, v0);
    a2 = 1u;
    f0 = a1f::mul(f0, f1);
    A1_W16B(a0, 42, (uint16_t)v1);
    at = 0xBF800000u;
    f3 = a1_flt(at);
    a1 = 0x1000u;
    at = 0x42800000u;
    f2 = a1_flt(at);
    A1_W32B(a0, 32, a1_bits(f3));
    { int32_t tmp = a1f::cvtw(f0); std::memcpy(&f1, &tmp, sizeof(tmp)); }
    v0 = a1_bits(f1);
    A1_W32B(a0, 36, a1_bits(f2));
    A1_W16B(a0, 46, (uint16_t)a2);
    A1_W16B(a0, 44, (uint16_t)a1);
    A1_W16B(a0, 48, (uint16_t)a2);
    {
        A1_W16B(a0, 40, (uint16_t)v0);
        return a1::RetVF{(uint64_t)a1_sx64(v0), f0};
    }
}

// sndSepPauseAll (0x1cad90): typed by tools/typed_lift.py (round A1)
inline uint32_t sndSepPauseAll(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint64_t a0)
{
    uint32_t v0 = 0;
    uint32_t v1 = 0;
    {
        const bool c = (int64_t)a0 == 0;
        v1 = 0x1E0000u;
        if (!c) {
            v0 = 1u;
            {
                A1_W32B(v1, 5468, v0);
                return v0;
            }
        }
    }
    v0 = 0x1E0000u;
    {
        A1_W32B(v0, 5468, 0u);
        return v0;
    }
}

// sndSepPause (0x1cadb8): typed by tools/typed_lift.py (round A1)
inline uint64_t sndSepPause(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint64_t a0)
{
    uint64_t v0 = 0;
    uint64_t v1 = 0;
    (v0 = (uint64_t)(A1_R64B((uint32_t)a0, 8)));
    (v1 = (uint64_t)(0ULL | (uint64_t)(uint16_t)32768));
    (v1 = (uint64_t)(RC_U64X(v1) << 18));
    (v0 = (uint64_t)(v0 | v1));
    {
        A1_W64B((uint32_t)a0, 8, v0);
        return v0;
    }
}

// sndSepCont (0x1cadd0): typed by tools/typed_lift.py (round A1)
inline uint64_t sndSepCont(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint64_t a0)
{
    uint64_t v0 = 0;
    uint64_t v1 = 0;
    (v0 = (uint64_t)(A1_R64B((uint32_t)a0, 8)));
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 4294967293))));
    (v1 = (uint64_t)(RC_U64X(v1) << 16));
    (v1 = (uint64_t)(v1 | (uint64_t)(uint16_t)65535));
    (v1 = (uint64_t)(v1 << 16));
    (v1 = (uint64_t)(v1 | (uint64_t)(uint16_t)65535));
    (v0 = (uint64_t)(v0 & v1));
    {
        A1_W64B((uint32_t)a0, 8, v0);
        return v0;
    }
}

// sndVabDelete (0x1caf88): typed by tools/typed_lift.py (round A1)
inline uint32_t sndVabDelete(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t a0)
{
    uint32_t v0 = 0;
    uint32_t v1 = 0;
    v0 = A1_R32(a0);
    v1 = 0xFFFFFFFEu;
    v0 = v0 & v1;
    {
        A1_W32(a0, v0);
        return v0;
    }
}

// sndBDClose (0x1cb168): typed by tools/typed_lift.py (round A1)
inline void sndBDClose(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt)
{
    {
        
        return;
    }
}

// addAdpcmTickProcFunc (0x1cb3a0): typed by tools/typed_lift.py (round A1)
inline uint32_t addAdpcmTickProcFunc(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t a0)
{
    uint32_t v0 = 0;
    v0 = 0x270000u;
    {
        A1_W32B(v0, 7176, a0);
        return v0;
    }
}

// setPcmTickProcFunc (0x1cb3b0): typed by tools/typed_lift.py (round A1)
inline uint32_t setPcmTickProcFunc(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t a0)
{
    uint32_t v0 = 0;
    v0 = 0x270000u;
    {
        A1_W32B(v0, 7180, a0);
        return v0;
    }
}

// soundGetTickProcCnt (0x1cb3c0): typed by tools/typed_lift.py (round A1)
inline uint32_t soundGetTickProcCnt(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt)
{
    uint32_t v0 = 0;
    uint32_t v1 = 0;
    v1 = 0x1E0000u;
    {
        v0 = A1_R32B(v1, 5476);
        return v0;
    }
}

// soundSetRevMode (0x1cb3d0): typed by tools/typed_lift.py (round A1)
inline uint64_t soundSetRevMode(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint64_t a0, uint64_t a1)
{
    uint64_t v0 = 0;
    uint64_t v1 = 0;
    {
        const bool c = (int64_t)a0 != 0;
        (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)30 << 16))));
        if (!c) {
            (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)30 << 16))));
            (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v1, 5860))));
            (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)v1, 0))));
            (v0 = (uint64_t)(RC_U64X(v0) | a1));
            {
                A1_W32B((uint32_t)v1, 0, (uint32_t)v0);
                return v0;
            }
        }
    }
    (a0 = (uint64_t)(~(0ULL | a1)));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v0, 5860))));
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)v0, 0))));
    (v1 = (uint64_t)(RC_U64X(v1) & a0));
    {
        A1_W32B((uint32_t)v0, 0, (uint32_t)v1);
        return v0;
    }
}

// soundGetRevMode (0x1cb408): typed by tools/typed_lift.py (round A1)
inline uint32_t soundGetRevMode(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt)
{
    uint32_t v0 = 0;
    uint32_t v1 = 0;
    v1 = 0x1E0000u;
    {
        v0 = A1_R32B(v1, 5860);
        return v0;
    }
}

// getSoundMode (0x1cb460): typed by tools/typed_lift.py (round A1)
inline uint32_t getSoundMode(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt)
{
    uint32_t v0 = 0;
    uint32_t v1 = 0;
    v1 = 0x1E0000u;
    v0 = A1_R32B(v1, 5472);
    v0 = v0 ^ 1u;
    {
        v0 = v0 < 1u ? 1u : 0u;
        return v0;
    }
}

// iosKeybdSetCallBackFunc (0x1cb920): typed by tools/typed_lift.py (round A1)
inline uint32_t iosKeybdSetCallBackFunc(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t a0)
{
    uint32_t v0 = 0;
    uint32_t v1 = 0;
    v0 = 0x1E0000u;
    v1 = A1_R32B(v0, 5484);
    {
        const bool c = v1 == 0;
        v0 = 0u;
        if (!c) {
            v1 = 0x1E0000u;
            v0 = 1u;
            A1_W32B(v1, 5488, a0);
        }
    }
    {
        return v0;
    }
}

// iosMouseSetCallBackFunc (0x1cbc70): typed by tools/typed_lift.py (round A1)
inline uint32_t iosMouseSetCallBackFunc(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t a0)
{
    uint32_t v0 = 0;
    uint32_t v1 = 0;
    v0 = 0x1E0000u;
    v1 = A1_R32B(v0, 5492);
    {
        const bool c = v1 == 0;
        v0 = 0u;
        if (!c) {
            v1 = 0x1E0000u;
            v0 = 1u;
            A1_W32B(v1, 5496, a0);
        }
    }
    {
        return v0;
    }
}

// GetStrHashWorkBuffer (0x1cbcb8): typed by tools/typed_lift.py (round A1)
inline uint32_t GetStrHashWorkBuffer(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t a0, uint32_t a1)
{
    uint32_t v0 = 0;
    v0 = 4u;
    a1 = a1 << 4;
    v0 = v0 << (a0 & 31);
    v0 = v0 + a1;
    {
        v0 = v0 + 0x66u;
        return v0;
    }
}

// kernel_sub_001CC028 (0x1cc028): typed by tools/typed_lift.py (round A1)
inline void kernel_sub_001CC028(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt)
{
    {
        
        return;
    }
}

// Sg2CtrlSetTickMode (0x1cc228): typed by tools/typed_lift.py (round A1)
inline uint32_t Sg2CtrlSetTickMode(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t a0)
{
    uint32_t v0 = 0;
    uint32_t v1 = 0;
    v1 = 0x270000u;
    v1 = v1 + 0x3DC4u;
    v0 = A1_R32(v1);
    {
        A1_W32(v1, a0);
        return v0;
    }
}

// Sg2CtrlGetTickMode (0x1cc240): typed by tools/typed_lift.py (round A1)
inline uint32_t Sg2CtrlGetTickMode(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt)
{
    uint32_t v0 = 0;
    uint32_t v1 = 0;
    v1 = 0x270000u;
    {
        v0 = A1_R32B(v1, 15812);
        return v0;
    }
}

// Sg2CtrlSetTickTime (0x1cc250): typed by tools/typed_lift.py (round A1)
inline uint32_t Sg2CtrlSetTickTime(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t a0)
{
    uint32_t v0 = 0;
    uint32_t v1 = 0;
    v1 = 0x270000u;
    v1 = v1 + 0x3DC0u;
    v0 = A1_R32(v1);
    {
        A1_W32(v1, a0);
        return v0;
    }
}

// Sg2CtrlGetTickTime (0x1cc268): typed by tools/typed_lift.py (round A1)
inline uint32_t Sg2CtrlGetTickTime(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt)
{
    uint32_t v0 = 0;
    uint32_t v1 = 0;
    v1 = 0x270000u;
    {
        v0 = A1_R32B(v1, 15808);
        return v0;
    }
}

// Sg2DmaInit (0x1cc318): typed by tools/typed_lift.py (round A1)
inline uint64_t Sg2DmaInit(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt)
{
    uint64_t v0 = 0;
    {
        (v0 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL));
        return v0;
    }
}

// Sg2DspGetMasterVolume (0x1cc7d8): typed by tools/typed_lift.py (round A1)
inline uint32_t Sg2DspGetMasterVolume(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t a0)
{
    uint32_t v0 = 0;
    uint32_t v1 = 0;
    v1 = a0 << 1;
    v1 = v1 + a0;
    v1 = v1 << 3;
    v0 = 0x270000u;
    v0 = v0 + v1;
    v0 = A1_R32B(v0, 15816);
    {
        return v0;
    }
}

// Sg2ExStAdpcmStopMode (0x1cc898): typed by tools/typed_lift.py (round A1)
inline uint32_t Sg2ExStAdpcmStopMode(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t a0)
{
    uint32_t v0 = 0;
    v0 = 0x270000u;
    {
        A1_W32B(v0, 15868, a0);
        return v0;
    }
}

// Sg2ExStAdpcmSetFd (0x1cced0): typed by tools/typed_lift.py (round A1)
inline uint64_t Sg2ExStAdpcmSetFd(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t a0, uint32_t a1)
{
    uint64_t v0 = 0;
    A1_W32B(a0, 728, a1);
    {
        (v0 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL));
        return v0;
    }
}

// Sg2ExStAdpcmGetFd (0x1ccee0): typed by tools/typed_lift.py (round A1)
inline uint32_t Sg2ExStAdpcmGetFd(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t a0)
{
    uint32_t v0 = 0;
    {
        v0 = A1_R32B(a0, 728);
        return v0;
    }
}

// Sg2ExStAdpcmSetLsn (0x1ccee8): typed by tools/typed_lift.py (round A1)
inline uint64_t Sg2ExStAdpcmSetLsn(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t a0, uint32_t a1)
{
    uint64_t v0 = 0;
    A1_W32B(a0, 728, a1);
    {
        (v0 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL));
        return v0;
    }
}

// Sg2ExStAdpcmGetLsn (0x1ccef8): typed by tools/typed_lift.py (round A1)
inline uint32_t Sg2ExStAdpcmGetLsn(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t a0)
{
    uint32_t v0 = 0;
    {
        v0 = A1_R32B(a0, 728);
        return v0;
    }
}

// Sg2ExStAdpcmGetTargetLsn (0x1ccf00): typed by tools/typed_lift.py (round A1)
inline uint32_t Sg2ExStAdpcmGetTargetLsn(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t a0)
{
    uint32_t v0 = 0;
    uint32_t v1 = 0;
    v0 = A1_R32B(a0, 584);
    v1 = A1_R32B(a0, 728);
    v0 = v0 >> 11;
    {
        v0 = v1 + v0;
        return v0;
    }
}

// Sg2ExStAdpcmSetUserParam (0x1ccf18): typed by tools/typed_lift.py (round A1)
inline uint64_t Sg2ExStAdpcmSetUserParam(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t a0, uint32_t a1)
{
    uint64_t v0 = 0;
    A1_W32B(a0, 732, a1);
    {
        (v0 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL));
        return v0;
    }
}

// Sg2ExStAdpcmGetUserParam (0x1ccf28): typed by tools/typed_lift.py (round A1)
inline uint32_t Sg2ExStAdpcmGetUserParam(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t a0)
{
    uint32_t v0 = 0;
    {
        v0 = A1_R32B(a0, 732);
        return v0;
    }
}

// Sg2ExStAdpcmLoopSecNum (0x1cd6f8): typed by tools/typed_lift.py (round A1)
inline uint64_t Sg2ExStAdpcmLoopSecNum(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t a0, uint32_t a1, uint32_t a2)
{
    uint32_t v1 = 0;
    uint64_t v0 = 0;
    v1 = A1_R32B(a0, 56);
    (v0 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL));
    A1_W32(a1, v1);
    a1 = A1_R32B(a0, 60);
    {
        A1_W32(a2, a1);
        return v0;
    }
}

// Sg2ExStAdpcmGetBuffer (0x1cd710): typed by tools/typed_lift.py (round A1)
inline uint64_t Sg2ExStAdpcmGetBuffer(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t a0, uint32_t a1, uint32_t a2, uint32_t a3, uint32_t t0)
{
    uint32_t v1 = 0;
    uint64_t v0 = 0;
    v1 = A1_R32B(a0, 560);
    (v0 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL));
    A1_W32(a1, v1);
    a1 = A1_R32B(a0, 564);
    A1_W32(a2, a1);
    v1 = A1_R32B(a0, 568);
    A1_W32(a3, v1);
    a1 = A1_R32B(a0, 572);
    {
        A1_W32(t0, a1);
        return v0;
    }
}

// Sg2ExStAdpcmSetSignal (0x1cd738): typed by tools/typed_lift.py (round A1)
inline uint32_t Sg2ExStAdpcmSetSignal(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t a0)
{
    uint32_t v0 = 0;
    v0 = 0x270000u;
    {
        A1_W32B(v0, 16064, a0);
        return v0;
    }
}

// Sg2ModCheckSndOn (0x1cec38): typed by tools/typed_lift.py (round A1)
inline uint32_t Sg2ModCheckSndOn(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint64_t a0)
{
    uint32_t v0 = 0;
    uint32_t v1 = 0;
    {
        const bool c = (int32_t)a0 < 0;
        (v0 = (uint32_t)((int32_t)((int32_t)SLL32((uint32_t)a0, 2))));
        if (!c) {
            (v0 = (uint32_t)((int32_t)((int32_t)ADD32(v0, (uint32_t)a0))));
            v1 = v0 << 5;
            v1 = v1 - v0;
            v1 = v1 << 4;
            v0 = 0x270000u;
            v0 = v0 + v1;
            v0 = A1_R32B(v0, 16088);
            v0 = ~(0u | v0);
            {
                v0 = 0u < v0 ? 1u : 0u;
                return v0;
            }
        }
    }
    {
        v0 = 0u;
        return v0;
    }
}

// Sg2ModClose (0x1cec70): typed by tools/typed_lift.py (round A1)
inline uint32_t Sg2ModClose(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint64_t a0)
{
    uint32_t at = 0;
    uint32_t v0 = 0;
    uint32_t v1 = 0;
    bool df_at = false, df_v1 = false;
    {
        const bool c = (int32_t)a0 < 0;
        (v0 = (uint32_t)((int32_t)((int32_t)SLL32((uint32_t)a0, 2))));
        if (!c) {
            (v0 = (uint32_t)((int32_t)((int32_t)ADD32(v0, (uint32_t)a0))));
            (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 1))));
            v1 = v0 << 5; df_v1 = true;
            v1 = v1 - v0; df_v1 = true;
            v1 = v1 << 4; df_v1 = true;
            at = 0x270000u; df_at = true;
            at = at + v1; df_at = true;
            A1_W32B(at, 16104, (uint32_t)a0);
        }
    }
    {
        v0 = 0u;
        return v0;
    }
}

// Sg2ModGetParam (0x1cf6e8): typed by tools/typed_lift.py (round A1)
inline uint32_t Sg2ModGetParam(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t a2, uint32_t a3, uint64_t a0, uint64_t a1)
{
    uint32_t v0 = 0;
    uint32_t v1 = 0;
    uint64_t t0 = 0;
    uint64_t t1 = 0;
    (t0 = (uint64_t)((uint64_t)a0 + (uint64_t)0ULL));
    v1 = 4u;
    (v0 = (uint32_t)((int32_t)((int32_t)SLL32((uint32_t)t0, 2))));
    (t1 = (uint64_t)(((int64_t)(int64_t)a1 < (int64_t)(int32_t)5) ? 1 : 0));
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(v0, (uint32_t)t0))));
    {
        const bool c = (int64_t)a1 == (int64_t)(int32_t)v1;
        (v0 = (uint32_t)((int32_t)((int32_t)SLL32((uint32_t)a0, 5))));
        if (!c) {
            {
                const bool c = (int64_t)t1 == 0;
                v0 = 8u;
                if (!c) {
                    v0 = 1u;
                    {
                        const bool c = (int64_t)a1 == (int64_t)(int32_t)v0;
                        (v1 = (uint32_t)((int32_t)((int32_t)SLL32((uint32_t)t0, 2))));
                        if (c) {
                            goto L_1cf730;
                        }
                    }
                    {
                        v0 = 0u;
                        return v0;
                    }
                }
            }
            if ((int64_t)a1 == (int64_t)(int32_t)v0) {
                (v0 = (uint32_t)((int32_t)((int32_t)SLL32((uint32_t)t0, 2))));
                goto L_1cf788;
            }
            {
                v0 = 0u;
                return v0;
            }
L_1cf730:
            (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)39 << 16))));
            (v1 = (uint32_t)((int32_t)((int32_t)ADD32(v1, (uint32_t)t0))));
            (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)a0, 16088))));
            v0 = v1 << 5;
            v0 = v0 - v1;
            v0 = v0 << 4;
            v0 = v0 + 0x30u;
            (a1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)a0, v0))));
            (v1 = (uint32_t)((int32_t)((int32_t)A1_R32B((uint32_t)a1, 4))));
            (a0 = (uint64_t)((uint64_t)RC_U64X(a1) + (uint64_t)0ULL));
            A1_W32(a2, v1);
            (v0 = (uint32_t)((int32_t)((int32_t)A1_R32B((uint32_t)a0, 8))));
            A1_W32(a3, v0);
            goto L_1cf7a8;
        }
    }
    (v0 = (uint32_t)((int32_t)((int32_t)SUB32(v0, (uint32_t)a0))));
    v0 = v0 << 4;
    v1 = 0x270000u;
    v1 = v1 + v0;
    v1 = A1_R32B(v1, 16148);
    A1_W32(a2, v1);
    goto L_1cf7a8;
L_1cf788:
    (v0 = (uint32_t)((int32_t)((int32_t)ADD32(v0, (uint32_t)t0))));
    v1 = v0 << 5;
    v1 = v1 - v0;
    v1 = v1 << 4;
    v0 = 0x270000u;
    v0 = v0 + v1;
    v0 = A1_R32B(v0, 16152);
    A1_W32(a2, v0);
L_1cf7a8:
    {
        v0 = 0u;
        return v0;
    }
}

// kernel_sub_001D0D78 (0x1d0d78): typed by tools/typed_lift.py (round A1)
inline uint32_t kernel_sub_001D0D78(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt)
{
    uint32_t v0 = 0;
    uint32_t v1 = 0;
    uint32_t a0 = 0;
    uint32_t a1 = 0;
    uint32_t a2 = 0;
    v0 = 0x270000u;
    a0 = 0x270000u;
    a1 = A1_R32B(v0, 16076);
    a2 = A1_R32B(a0, 16080);
    v1 = A1_R8(a1);
    a0 = A1_R8B(a1, 2);
    v1 = v1 & 0xFu;
    v0 = v1 << 3;
    v0 = v0 - v1;
    v0 = v0 << 4;
    v0 = v0 + a2;
    {
        A1_W32B(v0, 692, a0);
        return v0;
    }
}

// kernel_sub_001D12A8 (0x1d12a8): typed by tools/typed_lift.py (round A1)
inline uint32_t kernel_sub_001D12A8(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt)
{
    uint32_t v0 = 0;
    uint32_t v1 = 0;
    uint32_t a0 = 0;
    uint32_t a1 = 0;
    uint32_t a2 = 0;
    v0 = 0x270000u;
    a0 = 0x270000u;
    a1 = A1_R32B(v0, 16076);
    a2 = A1_R32B(a0, 16080);
    v1 = A1_R8(a1);
    a0 = A1_R8B(a1, 2);
    v1 = v1 & 0xFu;
    v0 = v1 << 3;
    v0 = v0 - v1;
    v0 = v0 << 4;
    v0 = v0 + a2;
    {
        A1_W32B(v0, 708, a0);
        return v0;
    }
}

// kernel_sub_001D12E0 (0x1d12e0): typed by tools/typed_lift.py (round A1)
inline uint32_t kernel_sub_001D12E0(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt)
{
    uint32_t v0 = 0;
    uint32_t v1 = 0;
    uint32_t a0 = 0;
    uint32_t a1 = 0;
    uint32_t a2 = 0;
    v0 = 0x270000u;
    a0 = 0x270000u;
    a1 = A1_R32B(v0, 16076);
    a2 = A1_R32B(a0, 16080);
    v1 = A1_R8(a1);
    a0 = A1_R8B(a1, 2);
    v1 = v1 & 0xFu;
    v0 = v1 << 3;
    v0 = v0 - v1;
    v0 = v0 << 4;
    v0 = v0 + a2;
    {
        A1_W32B(v0, 700, a0);
        return v0;
    }
}

// kernel_sub_001D1318 (0x1d1318): typed by tools/typed_lift.py (round A1)
inline uint32_t kernel_sub_001D1318(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt)
{
    uint32_t v0 = 0;
    uint32_t v1 = 0;
    uint32_t a0 = 0;
    uint32_t a1 = 0;
    uint32_t a2 = 0;
    v0 = 0x270000u;
    a0 = 0x270000u;
    a1 = A1_R32B(v0, 16076);
    a2 = A1_R32B(a0, 16080);
    v1 = A1_R8(a1);
    a0 = A1_R8B(a1, 2);
    v1 = v1 & 0xFu;
    v0 = v1 << 3;
    v0 = v0 - v1;
    v0 = v0 << 4;
    v0 = v0 + a2;
    {
        A1_W32B(v0, 720, a0);
        return v0;
    }
}

// kernel_sub_001D1350 (0x1d1350): typed by tools/typed_lift.py (round A1)
inline void kernel_sub_001D1350(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt)
{
    {
        
        return;
    }
}

// kernel_sub_001D1358 (0x1d1358): typed by tools/typed_lift.py (round A1)
inline void kernel_sub_001D1358(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt)
{
    {
        
        return;
    }
}

// kernel_sub_001D13E8 (0x1d13e8): typed by tools/typed_lift.py (round A1)
inline uint32_t kernel_sub_001D13E8(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt)
{
    uint32_t v0 = 0;
    uint32_t v1 = 0;
    uint32_t a0 = 0;
    uint32_t a1 = 0;
    uint32_t a2 = 0;
    v0 = 0x270000u;
    a0 = 0x270000u;
    a1 = A1_R32B(v0, 16076);
    a2 = A1_R32B(a0, 16080);
    v1 = A1_R8(a1);
    a0 = A1_R8B(a1, 2);
    v1 = v1 & 0xFu;
    v0 = v1 << 3;
    v0 = v0 - v1;
    v0 = v0 << 4;
    v0 = v0 + a2;
    {
        A1_W32B(v0, 744, a0);
        return v0;
    }
}

// kernel_sub_001D1470 (0x1d1470): typed by tools/typed_lift.py (round A1)
inline uint32_t kernel_sub_001D1470(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt)
{
    uint32_t v0 = 0;
    uint32_t v1 = 0;
    uint32_t a0 = 0;
    uint32_t a1 = 0;
    uint32_t a2 = 0;
    v0 = 0x270000u;
    a1 = v0 + 0x3ECCu;
    a0 = A1_R32(a1);
    v0 = A1_R8B(a0, 2);
    v1 = A1_R8(a0);
    v0 = v0 < 0x10u ? 1u : 0u;
    {
        const bool c = v0 != 0;
        a2 = v1 & 0xFu;
        if (!c) {
            v0 = 0xFu;
            A1_W8B(a0, 2, (uint8_t)v0);
            a0 = A1_R32(a1);
        }
    }
    v1 = A1_R8B(a0, 2);
    a1 = 0x270000u;
    v0 = a2 << 3;
    a0 = A1_R32B(a1, 16080);
    v0 = v0 - a2;
    v1 = v1 << 1;
    v0 = v0 << 4;
    v1 = v1 & 0xF0u;
    v0 = v0 + a0;
    {
        A1_W16B(v0, 782, (uint16_t)v1);
        return v0;
    }
}

// kernel_sub_001D14C8 (0x1d14c8): typed by tools/typed_lift.py (round A1)
inline uint64_t kernel_sub_001D14C8(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt)
{
    uint64_t v0 = 0;
    uint64_t v1 = 0;
    uint64_t a0 = 0;
    uint64_t a1 = 0;
    uint64_t a2 = 0;
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)39 << 16))));
    (a1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v0, 16076))));
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)a1, 0))));
    (v0 = (uint64_t)(uint32_t)((uint8_t)A1_R8B((uint32_t)a0, 2)));
    (v1 = (uint64_t)(uint32_t)((uint8_t)A1_R8B((uint32_t)a0, 0)));
    (v0 = (uint64_t)(((uint64_t)RC_U64X(v0) < (uint64_t)(int64_t)(int32_t)32) ? 1 : 0));
    {
        const bool c = (int64_t)v0 != 0;
        (a2 = (uint64_t)(RC_U64X(v1) & (uint64_t)(uint16_t)15));
        if (!c) {
            (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 31))));
            A1_W8B((uint32_t)a0, 2, (uint8_t)(uint32_t)v0);
            (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)a1, 0))));
        }
    }
    (v1 = (uint64_t)(uint32_t)((uint8_t)A1_R8B((uint32_t)a0, 2)));
    (a1 = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)39 << 16))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)SLL32((uint32_t)a2, 3))));
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)a1, 16080))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)SUB32((uint32_t)v0, (uint32_t)a2))));
    (v1 = (uint64_t)(RC_U64X(v1) >> 2));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)SLL32((uint32_t)v0, 4))));
    (v1 = (uint64_t)(v1 & (uint64_t)(uint16_t)31));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v0, (uint32_t)a0))));
    {
        A1_W16B((uint32_t)v0, 786, (uint16_t)(uint32_t)v1);
        return v0;
    }
}

// kernel_sub_001D1520 (0x1d1520): typed by tools/typed_lift.py (round A1)
inline uint32_t kernel_sub_001D1520(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt)
{
    uint32_t v0 = 0;
    uint32_t v1 = 0;
    uint32_t a0 = 0;
    uint32_t a1 = 0;
    uint32_t a2 = 0;
    uint32_t a3 = 0;
    v0 = 0x270000u;
    a1 = v0 + 0x3ECCu;
    a0 = A1_R32(a1);
    v0 = A1_R8(a0);
    v1 = (uint32_t)(int8_t)A1_R8B(a0, 2);
    a2 = v0 & 0xFu;
    {
        const bool c = (int32_t)v1 >= 0;
        a3 = a2 << 3;
        if (!c) {
            v0 = 0x7Fu;
            A1_W8B(a0, 2, (uint8_t)v0);
            a0 = A1_R32(a1);
        }
    }
    a0 = A1_R8B(a0, 2);
    a1 = 0x270000u;
    v1 = A1_R32B(a1, 16080);
    v0 = a3 - a2;
    v0 = v0 << 4;
    a0 = a0 << 8;
    a0 = a0 & 0x7F00u;
    v0 = v0 + v1;
    {
        A1_W16B(v0, 780, (uint16_t)a0);
        return v0;
    }
}

// kernel_sub_001D1578 (0x1d1578): typed by tools/typed_lift.py (round A1)
inline void kernel_sub_001D1578(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt)
{
    {
        
        return;
    }
}

// kernel_sub_001D1580 (0x1d1580): typed by tools/typed_lift.py (round A1)
inline uint32_t kernel_sub_001D1580(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt)
{
    uint32_t v0 = 0;
    uint32_t v1 = 0;
    uint32_t a0 = 0;
    uint32_t a1 = 0;
    uint32_t a2 = 0;
    v0 = 0x270000u;
    a0 = 0x270000u;
    a1 = A1_R32B(v0, 16076);
    a2 = A1_R32B(a0, 16080);
    v1 = A1_R8(a1);
    a0 = A1_R8B(a1, 2);
    v1 = v1 & 0xFu;
    v0 = v1 << 3;
    v0 = v0 - v1;
    v0 = v0 << 4;
    v0 = v0 + a2;
    {
        A1_W32B(v0, 724, a0);
        return v0;
    }
}

// kernel_sub_001D1658 (0x1d1658): typed by tools/typed_lift.py (round A1)
inline void kernel_sub_001D1658(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt)
{
    {
        
        return;
    }
}

// kernel_sub_001D1660 (0x1d1660): typed by tools/typed_lift.py (round A1)
inline uint32_t kernel_sub_001D1660(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt)
{
    uint32_t v0 = 0;
    uint32_t v1 = 0;
    uint32_t a0 = 0;
    uint32_t a1 = 0;
    uint32_t a2 = 0;
    v0 = 0x270000u;
    a0 = 0x270000u;
    a1 = A1_R32B(v0, 16076);
    a2 = A1_R32B(a0, 16080);
    v1 = A1_R8(a1);
    a0 = A1_R8B(a1, 2);
    v1 = v1 & 0xFu;
    v0 = v1 << 3;
    v0 = v0 - v1;
    v0 = v0 << 4;
    v0 = v0 + a2;
    {
        A1_W8B(v0, 789, (uint8_t)a0);
        return v0;
    }
}

// kernel_sub_001D1698 (0x1d1698): typed by tools/typed_lift.py (round A1)
inline uint32_t kernel_sub_001D1698(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt)
{
    uint32_t v0 = 0;
    uint32_t v1 = 0;
    uint32_t a0 = 0;
    uint32_t a1 = 0;
    uint32_t a2 = 0;
    v0 = 0x270000u;
    a0 = 0x270000u;
    a1 = A1_R32B(v0, 16076);
    a2 = A1_R32B(a0, 16080);
    v1 = A1_R8(a1);
    a0 = A1_R8B(a1, 2);
    v1 = v1 & 0xFu;
    v0 = v1 << 3;
    v0 = v0 - v1;
    v0 = v0 << 4;
    v0 = v0 + a2;
    {
        A1_W8B(v0, 788, (uint8_t)a0);
        return v0;
    }
}

// kernel_sub_001D1810 (0x1d1810): typed by tools/typed_lift.py (round A1)
inline void kernel_sub_001D1810(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt)
{
    {
        
        return;
    }
}

// kernel_sub_001D18D8 (0x1d18d8): typed by tools/typed_lift.py (round A1)
inline void kernel_sub_001D18D8(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt)
{
    {
        
        return;
    }
}

// kernel_sub_001D1F08 (0x1d1f08): typed by tools/typed_lift.py (round A1)
inline void kernel_sub_001D1F08(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt)
{
    {
        
        return;
    }
}

// kernel_sub_001D1F10 (0x1d1f10): typed by tools/typed_lift.py (round A1)
inline void kernel_sub_001D1F10(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt)
{
    {
        
        return;
    }
}

// kernel_sub_001D1F18 (0x1d1f18): typed by tools/typed_lift.py (round A1)
inline void kernel_sub_001D1F18(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt)
{
    {
        
        return;
    }
}

// kernel_sub_001D1F20 (0x1d1f20): typed by tools/typed_lift.py (round A1)
inline void kernel_sub_001D1F20(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt)
{
    {
        
        return;
    }
}

// kernel_sub_001D2008 (0x1d2008): typed by tools/typed_lift.py (round A1)
inline void kernel_sub_001D2008(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt)
{
    {
        
        return;
    }
}

// Sg2RemoteDmaTransStatus (0x1d2150): typed by tools/typed_lift.py (round A1)
inline uint32_t Sg2RemoteDmaTransStatus(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t a0)
{
    uint32_t v0 = 0;
    uint32_t v1 = 0;
    v1 = a0 << 2;
    v1 = v1 + a0;
    v1 = v1 << 2;
    v0 = 0x290000u;
    v0 = v0 + v1;
    v0 = A1_R32(v0 - 0x48B0u);
    {
        return v0;
    }
}

// Sg2RemoteGetAdpStmAdr (0x1d2170): typed by tools/typed_lift.py (round A1)
inline uint32_t Sg2RemoteGetAdpStmAdr(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t a0)
{
    uint32_t v0 = 0;
    a0 = a0 << 2;
    v0 = 0x290000u;
    v0 = v0 + a0;
    v0 = A1_R32(v0 - 0x4A00u);
    {
        return v0;
    }
}

// Sg2RemoteGetPcmStmTransOffset (0x1d2188): typed by tools/typed_lift.py (round A1)
inline uint32_t Sg2RemoteGetPcmStmTransOffset(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt)
{
    uint32_t v0 = 0;
    uint32_t v1 = 0;
    v1 = 0x290000u;
    {
        v0 = A1_R32(v1 - 0x4940u);
        return v0;
    }
}

// Sg2RemoteGetSlotReleaseLv (0x1d2198): typed by tools/typed_lift.py (round A1)
inline uint32_t Sg2RemoteGetSlotReleaseLv(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t a0)
{
    uint32_t v0 = 0;
    a0 = a0 << 2;
    v0 = 0x290000u;
    v0 = v0 + a0;
    v0 = A1_R32(v0 - 0x4AC0u);
    {
        return v0;
    }
}

// Sg2RemoteGetDspChangeStatus (0x1d21b0): typed by tools/typed_lift.py (round A1)
inline uint32_t Sg2RemoteGetDspChangeStatus(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t a0)
{
    uint32_t v0 = 0;
    a0 = a0 << 2;
    v0 = 0x290000u;
    v0 = v0 + a0;
    v0 = A1_R32(v0 - 0x4938u);
    {
        return v0;
    }
}

// Sg2SepStatus (0x1d2b98): typed by tools/typed_lift.py (round A1)
inline uint32_t Sg2SepStatus(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint64_t a0)
{
    uint32_t v0 = 0;
    uint64_t v1 = 0;
    (v0 = (uint32_t)((int32_t)((int32_t)SLL32((uint32_t)a0, 2))));
    (v1 = (uint64_t)(((uint64_t)a0 < (uint64_t)(int64_t)(int32_t)32) ? 1 : 0));
    {
        const bool c = (int64_t)v1 == 0;
        (v0 = (uint32_t)((int32_t)((int32_t)ADD32(v0, (uint32_t)a0))));
        if (!c) {
            (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)SLL32(v0, 4))));
            v0 = 0x290000u;
            (v0 = (uint32_t)((int32_t)((int32_t)ADD32(v0, (uint32_t)v1))));
            v0 = A1_R32(v0 - 0x4640u);
            {
                return v0;
            }
        }
    }
    {
        v0 = 0u;
        return v0;
    }
}

// kernel_sub_001D3428 (0x1d3428): typed by tools/typed_lift.py (round A1)
inline void kernel_sub_001D3428(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt)
{
    {
        
        return;
    }
}

// kernel_sub_001D3430 (0x1d3430): typed by tools/typed_lift.py (round A1)
inline uint32_t kernel_sub_001D3430(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t a0)
{
    uint32_t at = 0;
    uint32_t v0 = 0;
    uint32_t v1 = 0;
    uint32_t a1 = 0;
    uint32_t a2 = 0;
    uint32_t a3 = 0;
    uint32_t t0 = 0;
    uint32_t t1 = 0;
    uint32_t t2 = 0;
    uint32_t t3 = 0;
    bool df_at = false;
    a1 = a0 + 0x40u;
    v1 = 0x290000u;
    v0 = A1_R32(a1);
    t0 = v1 - 0x3B2Cu;
    a3 = A1_R8B(a0, 72);
    t3 = A1_R8(v0);
    v0 = v0 + 1u;
    A1_W32(a1, v0);
    v1 = v0 + 1u;
    a0 = v0 + 2u;
    a2 = A1_R32(t0);
    t2 = A1_R8(v0);
    A1_W32(a1, v1);
    t1 = A1_R8(v1);
    {
        const bool c = a2 == a3;
        A1_W32(a1, a0);
        if (!c) {
            A1_W32(t0, a3);
            a2 = 0x290000u;
            v1 = a2 - 0x3B30u;
            a0 = 0x290000u;
            v0 = A1_R32(v1);
            at = 0x290000u; df_at = true;
            at = at + v0; df_at = true;
            A1_W8(at - 0x3C30u, (uint8_t)a3);
            v0 = v0 + 1u;
            A1_W32(v1, v0);
            goto L_1d34a8;
        }
    }
    a0 = 0x290000u;
    a2 = 0x290000u;
L_1d34a8:
    a2 = a2 - 0x3B30u;
    v1 = a0 - 0x3C30u;
    v0 = A1_R32(a2);
    a1 = v0 + v1;
    v0 = v0 + 1u;
    a3 = v0 + v1;
    a0 = v0 + 2u;
    v0 = a3;
    A1_W8(a1, (uint8_t)t3);
    A1_W32(a2, a0);
    A1_W8(v0, (uint8_t)t2);
    {
        A1_W8B(a3, 1, (uint8_t)t1);
        return v0;
    }
}

// kernel_sub_001D3A48 (0x1d3a48): typed by tools/typed_lift.py (round A1)
inline uint32_t kernel_sub_001D3A48(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint64_t a0)
{
    uint32_t at = 0;
    uint32_t v0 = 0;
    uint32_t v1 = 0;
    uint32_t a1 = 0;
    uint32_t a2 = 0;
    uint32_t t0 = 0;
    uint32_t t1 = 0;
    uint64_t a3 = 0;
    bool df_at = false, df_v1 = false, df_t0 = false, df_t1 = false;
    (a3 = (uint64_t)((uint64_t)a0 + (uint64_t)0ULL));
    v0 = 0xFFu;
    (a2 = (uint32_t)((int32_t)((int32_t)A1_R32B((uint32_t)a3, 72))));
    {
        const bool c = a2 != v0;
        (a1 = (uint32_t)((int32_t)((int32_t)ADD32((uint32_t)a3, 64))));
        if (!c) {
            v0 = A1_R32(a1);
            (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 47))));
            t0 = A1_R8(v0); df_t0 = true;
            v0 = v0 + 1u;
            A1_W32(a1, v0);
            v1 = v0 + 1u; df_v1 = true;
            t1 = A1_R8(v0); df_t1 = true;
            {
                const bool c = (int64_t)(int32_t)t0 != (int64_t)a0;
                A1_W32(a1, v1);
                if (!c) {
                    v0 = 0x290000u;
                    A1_W32(a1, 0u);
                    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(v0, 4294952148))));
                    v1 = 2u; df_v1 = true;
                    (v0 = (uint32_t)((int32_t)((int32_t)A1_R32B((uint32_t)a0, 0))));
                    t0 = 0x3Fu; df_t0 = true;
                    {
                        const bool c = v0 == a2;
                        A1_W32B((uint32_t)a3, 0, v1);
                        if (!c) {
                            A1_W32B((uint32_t)a0, 0, a2);
                            (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)41 << 16))));
                            a1 = 0x290000u;
                            v1 = a1 - 0x3B30u; df_v1 = true;
                            v0 = A1_R32(v1);
                            at = 0x290000u; df_at = true;
                            at = at + v0; df_at = true;
                            A1_W8(at - 0x3C30u, (uint8_t)a2);
                            v0 = v0 + 1u;
                            A1_W32(v1, v0);
                            goto L_1d3ad8;
                        }
                    }
                    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)41 << 16))));
                    a1 = 0x290000u;
L_1d3ad8:
                    a1 = a1 - 0x3B30u;
                    (v1 = (uint32_t)((int32_t)((int32_t)ADD32((uint32_t)a0, 4294951888)))); df_v1 = true;
                    v0 = A1_R32(a1);
                    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(v0, v1))));
                    v0 = v0 + 1u;
                    v1 = v0 + v1; df_v1 = true;
                    v0 = v0 + 1u;
                    A1_W8B((uint32_t)a0, 0, (uint8_t)t0);
                    A1_W32(a1, v0);
                    A1_W8(v1, (uint8_t)t1);
                }
            }
        }
    }
    {
        return v0;
    }
}

// Sg2SlotFree (0x1d5d28): typed by tools/typed_lift.py (round A1)
inline uint32_t Sg2SlotFree(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t a0)
{
    uint32_t v0 = 0;
    uint32_t v1 = 0;
    v1 = a0 << 3;
    v0 = 0x290000u;
    v1 = v1 - a0;
    v0 = v0 - 0x39D0u;
    v1 = v1 << 5;
    v0 = v0 + 4u;
    v1 = v1 + v0;
    a0 = A1_R32(v1);
    if ((int32_t)a0 > 0) {
        A1_W32(v1, 0u);
    } else {
    }
    {
        return v0;
    }
}

// Sg2SlotSetGroup (0x1d6250): typed by tools/typed_lift.py (round A1)
inline uint32_t Sg2SlotSetGroup(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t a0, uint32_t a1)
{
    uint32_t at = 0;
    uint32_t v0 = 0;
    v0 = a0 << 3;
    v0 = v0 - a0;
    v0 = v0 << 5;
    at = 0x290000u;
    at = at + v0;
    A1_W32(at - 0x39B4u, a1);
    {
        return v0;
    }
}

// Sg2SlotSetMute (0x1d6270): typed by tools/typed_lift.py (round A1)
inline uint64_t Sg2SlotSetMute(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint64_t a0, uint64_t a1)
{
    uint32_t at = 0;
    uint32_t v1 = 0;
    uint32_t a3 = 0;
    uint64_t v0 = 0;
    uint64_t a2 = 0;
    {
        const bool c = (int64_t)a1 == 0;
        (a2 = (uint64_t)((uint64_t)a0 + (uint64_t)0ULL));
        if (!c) {
            (a1 = (uint64_t)((int64_t)(int32_t)((int32_t)SLL32((uint32_t)a2, 3))));
            a3 = 0x290000u;
            (v1 = (uint32_t)((int32_t)((int32_t)SUB32((uint32_t)a1, (uint32_t)a2))));
            v1 = v1 << 5;
            at = 0x290000u;
            at = at - 0x39D0u;
            v1 = at + v1;
            (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B(v1, 0))));
            (v0 = (uint64_t)(RC_U64X(v0) | (uint64_t)(uint16_t)32768));
            goto L_1d62cc;
        }
    }
    (a1 = (uint64_t)((int64_t)(int32_t)((int32_t)SLL32((uint32_t)a2, 3))));
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)65535 << 16))));
    (v1 = (uint32_t)((int32_t)((int32_t)SUB32((uint32_t)a1, (uint32_t)a2))));
    (a0 = (uint64_t)(RC_U64X(a0) | (uint64_t)(uint16_t)32767));
    v1 = v1 << 5;
    a3 = 0x290000u;
    at = 0x290000u;
    at = at - 0x39D0u;
    v1 = at + v1;
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B(v1, 0))));
    (v0 = (uint64_t)(RC_U64X(v0) & RC_U64X(a0)));
L_1d62cc:
    A1_W32B(v1, 0, (uint32_t)v0);
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)SUB32((uint32_t)a1, (uint32_t)a2))));
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(a3, 4294952496))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)SLL32((uint32_t)v0, 5))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v0, (uint32_t)a0))));
    (v1 = (uint32_t)((int32_t)((int32_t)A1_R32B((uint32_t)v0, 0))));
    (v1 = (uint32_t)(a1_sx64(v1) | (uint64_t)(uint16_t)4096));
    {
        A1_W32B((uint32_t)v0, 0, v1);
        return v0;
    }
}

// Sg2SlotSetBendRange (0x1d6528): typed by tools/typed_lift.py (round A1)
inline uint32_t Sg2SlotSetBendRange(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t a0, uint32_t a1)
{
    uint32_t at = 0;
    uint32_t v0 = 0;
    v0 = a0 << 3;
    v0 = v0 - a0;
    v0 = v0 << 5;
    at = 0x290000u;
    at = at + v0;
    A1_W32(at - 0x3978u, a1);
    {
        return v0;
    }
}

// Sg2SlotSetBendValue (0x1d6548): typed by tools/typed_lift.py (round A1)
inline uint32_t Sg2SlotSetBendValue(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t a0, uint32_t a1)
{
    uint32_t v0 = 0;
    uint32_t v1 = 0;
    v1 = a0 << 3;
    v0 = 0x290000u;
    v1 = v1 - a0;
    v0 = v0 - 0x39D0u;
    v1 = v1 << 5;
    a0 = v1 + v0;
    A1_W32B(a0, 92, a1);
    v1 = a0;
    v0 = A1_R32(v1);
    v0 = v0 | 0x80u;
    {
        A1_W32(v1, v0);
        return v0;
    }
}

// Sg2SlotSetPause (0x1d65f0): typed by tools/typed_lift.py (round A1)
inline uint64_t Sg2SlotSetPause(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint64_t a0, uint64_t a1)
{
    uint32_t at = 0;
    uint32_t v1 = 0;
    uint32_t a3 = 0;
    uint64_t v0 = 0;
    uint64_t a2 = 0;
    {
        const bool c = (int64_t)a1 == 0;
        (a2 = (uint64_t)((uint64_t)a0 + (uint64_t)0ULL));
        if (!c) {
            (a1 = (uint64_t)((int64_t)(int32_t)((int32_t)SLL32((uint32_t)a2, 3))));
            a3 = 0x290000u;
            (v1 = (uint32_t)((int32_t)((int32_t)SUB32((uint32_t)a1, (uint32_t)a2))));
            v1 = v1 << 5;
            at = 0x290000u;
            at = at - 0x39D0u;
            v1 = at + v1;
            (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B(v1, 0))));
            (v0 = (uint64_t)(RC_U64X(v0) | (uint64_t)(uint16_t)16384));
            goto L_1d6648;
        }
    }
    (a1 = (uint64_t)((int64_t)(int32_t)((int32_t)SLL32((uint32_t)a2, 3))));
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 4294950911))));
    (v1 = (uint32_t)((int32_t)((int32_t)SUB32((uint32_t)a1, (uint32_t)a2))));
    a3 = 0x290000u;
    v1 = v1 << 5;
    at = 0x290000u;
    at = at - 0x39D0u;
    v1 = at + v1;
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B(v1, 0))));
    (v0 = (uint64_t)(RC_U64X(v0) & RC_U64X(a0)));
L_1d6648:
    A1_W32B(v1, 0, (uint32_t)v0);
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)SUB32((uint32_t)a1, (uint32_t)a2))));
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(a3, 4294952496))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)SLL32((uint32_t)v0, 5))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v0, (uint32_t)a0))));
    (v1 = (uint32_t)((int32_t)((int32_t)A1_R32B((uint32_t)v0, 0))));
    v1 = v1 | 0x80u;
    {
        A1_W32B((uint32_t)v0, 0, v1);
        return v0;
    }
}

// Sg2SlotSetDirectPitch (0x1d6670): typed by tools/typed_lift.py (round A1)
inline uint32_t Sg2SlotSetDirectPitch(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t a0, uint32_t a1)
{
    uint32_t v0 = 0;
    uint32_t v1 = 0;
    v1 = a0 << 3;
    v0 = 0x290000u;
    v1 = v1 - a0;
    v0 = v0 - 0x39D0u;
    v1 = v1 << 5;
    a0 = v1 + v0;
    A1_W32B(a0, 112, a1);
    v1 = a0;
    v0 = A1_R32(v1);
    v0 = v0 | 0x80u;
    {
        A1_W32(v1, v0);
        return v0;
    }
}

// Sg2SlotSetLoop (0x1d6758): typed by tools/typed_lift.py (round A1)
inline uint32_t Sg2SlotSetLoop(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t a0)
{
    uint32_t at = 0;
    uint32_t v0 = 0;
    uint32_t v1 = 0;
    v0 = a0 << 3;
    v0 = v0 - a0;
    v0 = v0 << 5;
    at = 0x290000u;
    at = at - 0x39D0u;
    v0 = at + v0;
    v1 = A1_R32(v0);
    v1 = v1 | 4u;
    {
        A1_W32(v0, v1);
        return v0;
    }
}

// Sg2SlotSetDumperOn (0x1d6780): typed by tools/typed_lift.py (round A1)
inline uint32_t Sg2SlotSetDumperOn(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t a0)
{
    uint32_t at = 0;
    uint32_t v0 = 0;
    uint32_t v1 = 0;
    v0 = a0 << 3;
    v0 = v0 - a0;
    v0 = v0 << 5;
    at = 0x290000u;
    at = at - 0x39D0u;
    v0 = at + v0;
    v1 = A1_R32(v0);
    v1 = v1 | 8u;
    {
        A1_W32(v0, v1);
        return v0;
    }
}

// Sg2SlotSetLfoVolTbl (0x1d6950): typed by tools/typed_lift.py (round A1)
inline uint32_t Sg2SlotSetLfoVolTbl(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t a0, uint64_t a1)
{
    uint32_t v0 = 0;
    uint32_t v1 = 0;
    uint32_t a3 = 0;
    uint64_t a2 = 0;
    bool df_v1 = false, df_a0 = false;
    (a2 = (uint64_t)((uint64_t)a1 + (uint64_t)0ULL));
    v0 = a0 << 3;
    (a3 = (uint32_t)((int32_t)((int32_t)ADD32((uint32_t)a2, 60))));
    {
        const bool c = (int64_t)a2 == 0;
        v0 = v0 - a0;
        if (!c) {
            a0 = v0 << 5; df_a0 = true;
            v0 = 0x290000u;
            v0 = v0 - 0x39D0u;
            v1 = a0 + 0xA0u; df_v1 = true;
            (a1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(v0, v1))));
            v1 = v1 + v0; df_v1 = true;
            a0 = a0 + v0; df_a0 = true;
            A1_W32B(a0, 156, (uint32_t)a2);
            A1_W32B(v1, 0, (uint32_t)a2);
            A1_W32B((uint32_t)a1, 4, a3);
        }
    }
    {
        return v0;
    }
}

// Sg2SlotSetTimeVolParam (0x1d6c30): typed by tools/typed_lift.py (round A1)
inline uint32_t Sg2SlotSetTimeVolParam(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t a0, float f12, float f13)
{
    uint32_t v0 = 0;
    uint32_t v1 = 0;
    uint32_t a1 = 0;
    uint32_t a2 = 0;
    v0 = a0 << 3;
    v1 = 0x290000u;
    v0 = v0 - a0;
    v1 = v1 - 0x39D0u;
    v0 = v0 << 5;
    a1 = v0 + v1;
    v0 = v0 + 0xA0u;
    a0 = A1_R32(a1);
    a2 = v1 + v0;
    v1 = a2;
    a0 = a0 | 0x20u;
    A1_W32B(v1, 12, a1_bits(f12));
    A1_W32(a1, a0);
    {
        A1_W32B(a2, 8, a1_bits(f13));
        return v0;
    }
}

// Sg2SlotSetTimePitchParam (0x1d6c70): typed by tools/typed_lift.py (round A1)
inline uint32_t Sg2SlotSetTimePitchParam(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t a0, float f12, float f13)
{
    uint32_t v0 = 0;
    uint32_t v1 = 0;
    uint32_t a1 = 0;
    uint32_t a2 = 0;
    v0 = a0 << 3;
    v1 = 0x290000u;
    v0 = v0 - a0;
    v1 = v1 - 0x39D0u;
    v0 = v0 << 5;
    a1 = v0 + v1;
    v0 = v0 + 0xC0u;
    a0 = A1_R32(a1);
    a2 = v0 + v1;
    v1 = v1 + v0;
    a0 = a0 | 0x10u;
    A1_W32B(v1, 4, a1_bits(f12));
    A1_W32(a1, a0);
    {
        A1_W32(a2, a1_bits(f13));
        return v0;
    }
}

// Sg2SlotSetTimePanParam (0x1d6cb0): typed by tools/typed_lift.py (round A1)
inline uint32_t Sg2SlotSetTimePanParam(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t a0, float f12, float f13)
{
    uint32_t v0 = 0;
    uint32_t v1 = 0;
    uint32_t a1 = 0;
    uint32_t a2 = 0;
    v0 = a0 << 3;
    v1 = 0x290000u;
    v0 = v0 - a0;
    v1 = v1 - 0x39D0u;
    v0 = v0 << 5;
    a1 = v0 + v1;
    v0 = v0 + 0xB0u;
    a0 = A1_R32(a1);
    a2 = v1 + v0;
    v1 = a2;
    a0 = a0 | 0x40u;
    A1_W32B(v1, 8, a1_bits(f12));
    A1_W32(a1, a0);
    {
        A1_W32B(a2, 4, a1_bits(f13));
        return v0;
    }
}

// Sg2SlotGetVol (0x1d6d50): typed by tools/typed_lift.py (round A1)
inline uint32_t Sg2SlotGetVol(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t a0)
{
    uint32_t v0 = 0;
    uint32_t v1 = 0;
    v1 = a0 << 3;
    v1 = v1 - a0;
    v1 = v1 << 5;
    v0 = 0x290000u;
    v0 = v0 + v1;
    v0 = A1_R32(v0 - 0x3988u);
    {
        return v0;
    }
}

// Sg2SlotGetPan (0x1d6d70): typed by tools/typed_lift.py (round A1)
inline uint32_t Sg2SlotGetPan(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t a0)
{
    uint32_t v0 = 0;
    uint32_t v1 = 0;
    v1 = a0 << 3;
    v1 = v1 - a0;
    v1 = v1 << 5;
    v0 = 0x290000u;
    v0 = v0 + v1;
    v0 = A1_R32(v0 - 0x397Cu);
    {
        return v0;
    }
}

// Sg2SlotSetOutputMode (0x1d7458): typed by tools/typed_lift.py (round A1)
inline uint32_t Sg2SlotSetOutputMode(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t a0)
{
    uint32_t v0 = 0;
    uint32_t v1 = 0;
    v1 = 0x290000u;
    v1 = v1 - 0x39D8u;
    v0 = A1_R32(v1);
    {
        A1_W32(v1, a0);
        return v0;
    }
}

// Sg2SlotSetVolShiftDownParam (0x1d7470): typed by tools/typed_lift.py (round A1)
inline uint32_t Sg2SlotSetVolShiftDownParam(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t a0)
{
    uint32_t v0 = 0;
    uint32_t v1 = 0;
    v1 = 0x290000u;
    v1 = v1 - 0x39D4u;
    v0 = A1_R32(v1);
    {
        A1_W32(v1, a0);
        return v0;
    }
}

// Sg2SlotGetVolShiftDownParam (0x1d7488): typed by tools/typed_lift.py (round A1)
inline uint32_t Sg2SlotGetVolShiftDownParam(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt)
{
    uint32_t v0 = 0;
    uint32_t v1 = 0;
    v1 = 0x290000u;
    {
        v0 = A1_R32(v1 - 0x39D4u);
        return v0;
    }
}

// kernel_sub_001D8250 (0x1d8250): typed by tools/typed_lift.py (round A1)
inline uint64_t kernel_sub_001D8250(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t a0)
{
    uint32_t v1 = 0;
    uint64_t v0 = 0;
    v1 = A1_R32B(a0, 96);
    (v0 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL));
    v1 = v1 + 8u;
    {
        A1_W32B(a0, 96, v1);
        return v0;
    }
}

// Sg2SmfpStatus (0x1d88f8): typed by tools/typed_lift.py (round A1)
inline uint32_t Sg2SmfpStatus(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint64_t a0)
{
    uint32_t at = 0;
    uint32_t v0 = 0;
    {
        const bool c = (int32_t)a0 < 0;
        (v0 = (uint32_t)((int32_t)((int32_t)SLL32((uint32_t)a0, 7))));
        if (!c) {
            at = 0x290000u;
            at = at + v0;
            v0 = A1_R32(at - 0xF80u);
            {
                return v0;
            }
        }
    }
    {
        v0 = 1u;
        return v0;
    }
}

// Sg2SmfpSetSignal (0x1d8e78): typed by tools/typed_lift.py (round A1)
inline uint32_t Sg2SmfpSetSignal(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t a0)
{
    uint32_t v0 = 0;
    v0 = 0x290000u;
    {
        A1_W32(v0 - 0xB78u, a0);
        return v0;
    }
}

// kernel_sub_001D95C8 (0x1d95c8): typed by tools/typed_lift.py (round A1)
inline void kernel_sub_001D95C8(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt)
{
    {
        
        return;
    }
}

// kernel_sub_001D95D0 (0x1d95d0): typed by tools/typed_lift.py (round A1)
inline uint32_t kernel_sub_001D95D0(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t a0)
{
    uint32_t at = 0;
    uint32_t v0 = 0;
    uint32_t v1 = 0;
    uint32_t a1 = 0;
    uint32_t a2 = 0;
    uint32_t a3 = 0;
    uint32_t t0 = 0;
    uint32_t t1 = 0;
    bool df_at = false;
    a1 = a0 + 0x60u;
    v1 = 0x290000u;
    v0 = A1_R32(a1);
    a3 = v1 - 0x36Cu;
    a2 = A1_R32(a3);
    t1 = A1_R8(v0);
    v0 = v0 + 1u;
    A1_W32(a1, v0);
    v1 = v0 + 1u;
    t0 = A1_R8(v0);
    A1_W32(a1, v1);
    a0 = A1_R8B(a0, 104);
    {
        const bool c = a2 == a0;
        a2 = 0x290000u;
        if (!c) {
            A1_W32(a3, a0);
            a1 = 0x290000u;
            v1 = a1 - 0x370u;
            v0 = A1_R32(v1);
            at = 0x290000u; df_at = true;
            at = at + v0; df_at = true;
            A1_W8(at - 0xB70u, (uint8_t)a0);
            v0 = v0 + 1u;
            A1_W32(v1, v0);
            goto L_1d9634;
        }
    }
    a1 = 0x290000u;
L_1d9634:
    a1 = a1 - 0x370u;
    v1 = a2 - 0xB70u;
    v0 = A1_R32(a1);
    a0 = v0 + v1;
    v0 = v0 + 1u;
    v1 = v0 + v1;
    v0 = v0 + 1u;
    A1_W8(a0, (uint8_t)t1);
    A1_W32(a1, v0);
    {
        A1_W8(v1, (uint8_t)t0);
        return v0;
    }
}

// kernel_sub_001D97F0 (0x1d97f0): typed by tools/typed_lift.py (round A1)
inline uint32_t kernel_sub_001D97F0(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t a0)
{
    uint32_t at = 0;
    uint32_t v0 = 0;
    uint32_t v1 = 0;
    uint32_t a1 = 0;
    uint32_t a2 = 0;
    uint32_t a3 = 0;
    bool df_at = false;
    a1 = a0 + 0x60u;
    v1 = 0x290000u;
    v0 = A1_R32(a1);
    a2 = v1 - 0x36Cu;
    v1 = A1_R32(a2);
    a3 = A1_R8(v0);
    v0 = v0 + 1u;
    A1_W32(a1, v0);
    a1 = A1_R8B(a0, 104);
    if (v1 == a1) {
        a2 = 0x290000u;
    } else {
        A1_W32(a2, a1);
        a2 = 0x290000u;
        a0 = 0x290000u;
        v1 = a0 - 0x370u;
        v0 = A1_R32(v1);
        at = 0x290000u; df_at = true;
        at = at + v0; df_at = true;
        A1_W8(at - 0xB70u, (uint8_t)a1);
        v0 = v0 + 1u;
        A1_W32(v1, v0);
        goto L_1d984c;
    }
    a0 = 0x290000u;
L_1d984c:
    a0 = a0 - 0x370u;
    v1 = a2 - 0xB70u;
    v0 = A1_R32(a0);
    v1 = v0 + v1;
    v0 = v0 + 1u;
    A1_W8(v1, (uint8_t)a3);
    {
        A1_W32(a0, v0);
        return v0;
    }
}

// vab_check_alive (0x1da0a0): typed by tools/typed_lift.py (round A1)
inline uint64_t vab_check_alive(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint64_t a0)
{
    uint64_t v0 = 0;
    uint64_t v1 = 0;
    (v0 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL));
    {
        const bool c = (int32_t)a0 < 0;
        (v1 = (uint64_t)(((int64_t)(int64_t)a0 < (int64_t)(int32_t)8) ? 1 : 0));
        if (!c) {
            {
                const bool c = (int64_t)v1 == 0;
                (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)SLL32((uint32_t)a0, 1))));
                if (!c) {
                    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v1, (uint32_t)a0))));
                    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)SLL32((uint32_t)v1, 2))));
                    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)SUB32((uint32_t)v1, (uint32_t)a0))));
                    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)SLL32((uint32_t)v1, 2))));
                    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)41 << 16))));
                    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v0, (uint32_t)v1))));
                    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)v0, 1216))));
                    (v0 = (uint64_t)(((uint64_t)0ULL < (uint64_t)RC_U64X(v0)) ? 1 : 0));
                }
            }
        }
    }
    {
        return v0;
    }
}

// vab_get_info (0x1da0e0): typed by tools/typed_lift.py (round A1)
inline uint64_t vab_get_info(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t a0, uint32_t a1, uint32_t a2)
{
    uint32_t v1 = 0;
    uint32_t a3 = 0;
    uint32_t t0 = 0;
    uint64_t v0 = 0;
    v1 = a0 << 1;
    a3 = 0x290000u;
    v1 = v1 + a0;
    a3 = a3 + 0x4C0u;
    v1 = v1 << 2;
    (v0 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL));
    v1 = v1 - a0;
    v1 = v1 << 2;
    t0 = a3 + v1;
    v1 = v1 + a3;
    a0 = A1_R32B(t0, 12);
    A1_W32(a1, a0);
    a1 = A1_R32(v1);
    {
        A1_W32(a2, a1);
        return v0;
    }
}

// vab_get_veltbl (0x1da348): typed by tools/typed_lift.py (round A1)
inline uint32_t vab_get_veltbl(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t a0, uint32_t a1)
{
    uint32_t v0 = 0;
    uint32_t v1 = 0;
    uint32_t a2 = 0;
    v0 = a0 << 1;
    v1 = 0x290000u;
    v0 = v0 + a0;
    v1 = v1 + 0x4C0u;
    v0 = v0 << 2;
    v0 = v0 - a0;
    v0 = v0 << 2;
    v0 = v0 + 0x10u;
    a0 = v1 + v0;
    a2 = A1_R32B(a0, 8);
    v1 = a0;
    v0 = A1_R32B(v1, 12);
    {
        A1_W32(a1, a2);
        return v0;
    }
}

// vab_get_lfotbl_param (0x1da380): typed by tools/typed_lift.py (round A1)
inline uint32_t vab_get_lfotbl_param(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t a0, uint32_t a1, uint32_t a2)
{
    uint32_t v0 = 0;
    uint32_t v1 = 0;
    uint32_t a3 = 0;
    uint32_t t0 = 0;
    v0 = a0 << 1;
    v1 = 0x290000u;
    v0 = v0 + a0;
    v1 = v1 + 0x4C0u;
    v0 = v0 << 2;
    v0 = v0 - a0;
    v0 = v0 << 2;
    v0 = v0 + 0x20u;
    a0 = v1 + v0;
    a3 = v0 + v1;
    t0 = A1_R32B(a0, 4);
    v1 = a0;
    a0 = A1_R32(a3);
    A1_W32(a2, t0);
    A1_W32(a1, a0);
    {
        v0 = A1_R32B(v1, 8);
        return v0;
    }
}

// vab_set_semastervol (0x1da3c8): typed by tools/typed_lift.py (round A1)
inline uint32_t vab_set_semastervol(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t a0, uint32_t a1)
{
    uint32_t at = 0;
    uint32_t v0 = 0;
    uint32_t v1 = 0;
    v1 = a0 << 1;
    v1 = v1 + a0;
    v1 = v1 << 2;
    v1 = v1 - a0;
    v1 = v1 << 2;
    at = 0x290000u;
    at = at + 0x4D0u;
    v1 = at + v1;
    v0 = A1_R32(v1);
    {
        A1_W32(v1, a1);
        return v0;
    }
}

// vab_get_semastervol (0x1da3f8): typed by tools/typed_lift.py (round A1)
inline uint32_t vab_get_semastervol(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t a0)
{
    uint32_t v0 = 0;
    uint32_t v1 = 0;
    v1 = a0 << 1;
    v1 = v1 + a0;
    v1 = v1 << 2;
    v1 = v1 - a0;
    v1 = v1 << 2;
    v0 = 0x290000u;
    v0 = v0 + v1;
    v0 = A1_R32B(v0, 1232);
    {
        return v0;
    }
}

// kernel_sub_001DB3C8 (0x1db3c8): typed by tools/typed_lift.py (round A1)
inline uint32_t kernel_sub_001DB3C8(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t a0)
{
    uint32_t v0 = 0;
    uint32_t v1 = 0;
    v0 = A1_R8(a0);
    v1 = 0x45u;
    if (v0 == v1) {
        v1 = A1_R8B(a0, 1);
        v0 = 0x51u;
        if (v1 == v0) {
            v1 = A1_R8B(a0, 2);
            v0 = 0x41u;
            if (v1 == v0) {
                v1 = A1_R8B(a0, 3);
                v0 = 0x54u;
                if (v1 == v0) {
                    goto L_1db410;
                }
            }
        }
    }
    {
        v0 = 0xFFFFFFFFu;
        return v0;
    }
L_1db410:
    {
        v0 = A1_R32B(a0, 4);
        return v0;
    }
}

// Sg2ExPcmInputGetLipFrame (0x1db9e8): typed by tools/typed_lift.py (round A1)
inline uint32_t Sg2ExPcmInputGetLipFrame(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint64_t a0)
{
    uint32_t v0 = 0;
    uint64_t v1 = 0;
    uint64_t hi = 0;
    uint64_t lo = 0;
    (v1 = (uint64_t)(((uint64_t)a0 < (uint64_t)(int64_t)(int32_t)4) ? 1 : 0));
    {
        const bool c = (int64_t)v1 == 0;
        v0 = 0xFFFFFFFFu;
        if (!c) {
            (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 468))));
            { int64_t result = (int64_t)(int32_t)a0 * (int64_t)(int32_t)v1; lo = (uint64_t)(int64_t)(int32_t)result; hi = (uint64_t)(int64_t)(int32_t)(result >> 32); (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)result))); }
            v0 = 0x290000u;
            (v0 = (uint32_t)((int32_t)((int32_t)ADD32(v0, (uint32_t)v1))));
            v0 = A1_R32B(v0, 2028);
        }
    }
    {
        return v0;
    }
}

// Sg2ExPcmInputSetOutputMode (0x1dbba0): typed by tools/typed_lift.py (round A1)
inline uint64_t Sg2ExPcmInputSetOutputMode(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t a1, uint64_t a0)
{
    uint32_t at = 0;
    uint64_t v0 = 0;
    uint64_t hi = 0;
    uint64_t lo = 0;
    bool df_at = false;
    (v0 = (uint64_t)(((uint64_t)a0 < (uint64_t)(int64_t)(int32_t)4) ? 1 : 0));
    {
        const bool c = (int64_t)v0 == 0;
        (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 468))));
        if (!c) {
            { int64_t result = (int64_t)(int32_t)a0 * (int64_t)(int32_t)v0; lo = (uint64_t)(int64_t)(int32_t)result; hi = (uint64_t)(int64_t)(int32_t)(result >> 32); (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)result))); }
            at = 0x290000u; df_at = true;
            (at = (uint32_t)((int32_t)((int32_t)ADD32(at, (uint32_t)v0)))); df_at = true;
            A1_W32B(at, 2432, a1);
        }
    }
    {
        return v0;
    }
}

// Sg2ExPcmInputAutoFilterCtrl (0x1dbd90): typed by tools/typed_lift.py (round A1)
inline uint32_t Sg2ExPcmInputAutoFilterCtrl(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t a1, uint32_t a2, uint64_t a0)
{
    uint32_t at = 0;
    uint32_t v0 = 0;
    uint64_t v1 = 0;
    uint64_t hi = 0;
    uint64_t lo = 0;
    bool df_at = false;
    v0 = a1 << 1;
    (v1 = (uint64_t)(((uint64_t)a0 < (uint64_t)(int64_t)(int32_t)4) ? 1 : 0));
    v0 = v0 + a1;
    {
        const bool c = (int64_t)v1 == 0;
        v0 = v0 << 4;
        if (!c) {
            (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 468))));
            v0 = v0 + a1;
            { int64_t result = (int64_t)(int32_t)a0 * (int64_t)(int32_t)v1; lo = (uint64_t)(int64_t)(int32_t)result; hi = (uint64_t)(int64_t)(int32_t)(result >> 32); (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)result))); }
            v0 = v0 << 2;
            (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v1, v0))));
            at = 0x290000u; df_at = true;
            (at = (uint32_t)((int32_t)((int32_t)ADD32(at, (uint32_t)v1)))); df_at = true;
            A1_W32B(at, 2032, a2);
        }
    }
    {
        return v0;
    }
}

// Sg2ExPcmOutputSetOutputMode (0x1dbfb8): typed by tools/typed_lift.py (round A1)
inline uint32_t Sg2ExPcmOutputSetOutputMode(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t a0)
{
    uint32_t v0 = 0;
    v0 = 0x290000u;
    {
        A1_W32B(v0, 1568, a0);
        return v0;
    }
}

// Sg2ExPcmOutputGetLipFrame (0x1dc068): typed by tools/typed_lift.py (round A1)
inline uint32_t Sg2ExPcmOutputGetLipFrame(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt)
{
    uint32_t v0 = 0;
    uint32_t v1 = 0;
    v1 = 0x290000u;
    {
        v0 = A1_R32B(v1, 1592);
        return v0;
    }
}

// Sg2ExPcmOutputAutoFilterCtrl (0x1dc1d0): typed by tools/typed_lift.py (round A1)
inline uint32_t Sg2ExPcmOutputAutoFilterCtrl(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t a0, uint32_t a1)
{
    uint32_t at = 0;
    uint32_t v0 = 0;
    v0 = a0 << 1;
    v0 = v0 + a0;
    v0 = v0 << 4;
    v0 = v0 + a0;
    v0 = v0 << 2;
    at = 0x290000u;
    at = at + v0;
    A1_W32B(at, 1596, a1);
    {
        return v0;
    }
}

// Sg2ExPcmSetSignal (0x1dc2d0): typed by tools/typed_lift.py (round A1)
inline uint32_t Sg2ExPcmSetSignal(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t a0)
{
    uint32_t v0 = 0;
    v0 = 0x290000u;
    {
        A1_W32B(v0, 3880, a0);
        return v0;
    }
}

// IirInit (0x1dca28): typed by tools/typed_lift.py (round A1)
inline void IirInit(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t a0)
{
    A1_W32B(a0, 44, 0u);
    A1_W32B(a0, 28, 0u);
    A1_W32B(a0, 32, 0u);
    A1_W32B(a0, 36, 0u);
    A1_W32B(a0, 40, 0u);
    {
        A1_W32B(a0, 48, 0u);
        
        return;
    }
}

// GAMEUTIL_is_inside_of_region_pitch (0x107d5c8): typed by tools/typed_lift.py (round A1)
inline a1::RetVF GAMEUTIL_is_inside_of_region_pitch(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint64_t a0, uint64_t a1, float f12, float f13, float f14)
{
    uint32_t at = 0;
    uint64_t v0 = 0;
    float f0 = 0.0f;
    float f1 = 0.0f;
    float f2 = 0.0f;
    float f3 = 0.0f;
    float f4 = 0.0f;
    float f5 = 0.0f;
    uint32_t fcr31 = 0;
    at = 0x3F000000u;
    { uint32_t bits = at; std::memcpy(&f0, &bits, sizeof(bits)); }
    { uint32_t bits = 0u; std::memcpy(&f1, &bits, sizeof(bits)); }
    f14 = a1f::mul(f14, f0);
    at = 0x38D10000u;
    at = at | 0xB717u;
    { uint32_t bits = at; std::memcpy(&f0, &bits, sizeof(bits)); }
    at = 0x3FBD0000u;
    at = at | 0xE44Eu;
    { uint32_t bits = at; std::memcpy(&f4, &bits, sizeof(bits)); }
    at = 0xBFBD0000u;
    at = at | 0xE44Eu;
    { uint32_t bits = at; std::memcpy(&f5, &bits, sizeof(bits)); }
    fcr31 = (a1f::lt(f14, f1)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
    {
        const bool c = (fcr31 & 0x800000u) == 0u;
        f3 = ((float)(f13));
        if (!c) {
            f2 = (-(float)(f14));
            fcr31 = (a1f::le(f2, f0)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
            if ((fcr31 & 0x800000u) != 0u) {
                (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 1))));
                goto L_107d63c;
            }
            (v0 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL));
            goto L_107d63c;
        }
    }
    fcr31 = (a1f::le(f14, f0)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
    {
        const bool c = (fcr31 & 0x800000u) == 0u;
        f2 = (-(float)(f14));
        if (!c) {
            (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 1))));
            goto L_107d63c;
        }
    }
    (v0 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL));
L_107d63c:
    if ((int64_t)v0 == 0) {
        at = 0x3FBD0000u;
        at = at | 0xE44Eu;
        { uint32_t bits = at; std::memcpy(&f0, &bits, sizeof(bits)); }
        fcr31 = (a1f::lt(f0, f4)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
        if ((fcr31 & 0x800000u) != 0u) {
            f4 = ((float)(f0));
            goto L_107d680;
        }
        at = 0xBFBD0000u;
        at = at | 0xE44Eu;
        { uint32_t bits = at; std::memcpy(&f0, &bits, sizeof(bits)); }
        fcr31 = (a1f::lt(f5, f0)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
        if ((fcr31 & 0x800000u) != 0u) {
            f5 = ((float)(f0));
        } else {
        }
L_107d680:
        fcr31 = (a1f::lt(f4, f13)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
        if ((fcr31 & 0x800000u) != 0u) {
            { uint32_t bits = 0u; std::memcpy(&f0, &bits, sizeof(bits)); }
            fcr31 = (a1f::lt(f14, f0)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
            goto L_107d6b4;
        }
        fcr31 = (a1f::lt(f13, f5)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
        if ((fcr31 & 0x800000u) == 0u) {
            f3 = a1f::add(f13, f14);
            goto L_107d6c8;
        }
        { uint32_t bits = 0u; std::memcpy(&f0, &bits, sizeof(bits)); }
        fcr31 = (a1f::lt(f0, f14)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
L_107d6b4:
        if ((fcr31 & 0x800000u) != 0u) {
            f3 = a1f::add(f13, f14);
            goto L_107d6ec;
        }
        goto L_107d6ec;
L_107d6c8:
        fcr31 = (a1f::lt(f4, f3)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
        if ((fcr31 & 0x800000u) != 0u) {
            f3 = ((float)(f4));
            goto L_107d6ec;
        }
        fcr31 = (a1f::lt(f3, f5)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
        if ((fcr31 & 0x800000u) != 0u) {
            f3 = ((float)(f5));
        } else {
        }
    }
L_107d6ec:
    { uint32_t bits = 0u; std::memcpy(&f0, &bits, sizeof(bits)); }
    at = 0x3FBD0000u;
    at = at | 0xE44Eu;
    { uint32_t bits = at; std::memcpy(&f4, &bits, sizeof(bits)); }
    fcr31 = (a1f::lt(f2, f0)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
    at = 0x38D10000u;
    at = at | 0xB717u;
    { uint32_t bits = at; std::memcpy(&f0, &bits, sizeof(bits)); }
    at = 0xBFBD0000u;
    at = at | 0xE44Eu;
    { uint32_t bits = at; std::memcpy(&f5, &bits, sizeof(bits)); }
    {
        const bool c = (fcr31 & 0x800000u) == 0u;
        f1 = ((float)(f13));
        if (!c) {
            fcr31 = (a1f::le(f14, f0)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
            if ((fcr31 & 0x800000u) != 0u) {
                (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 1))));
                goto L_107d748;
            }
            (v0 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL));
            goto L_107d748;
        }
    }
    fcr31 = (a1f::le(f2, f0)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
    if ((fcr31 & 0x800000u) == 0u) {
        (v0 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL));
    } else {
        (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 1))));
    }
L_107d748:
    if ((int64_t)v0 == 0) {
        at = 0x3FBD0000u;
        at = at | 0xE44Eu;
        { uint32_t bits = at; std::memcpy(&f0, &bits, sizeof(bits)); }
        fcr31 = (a1f::lt(f0, f4)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
        if ((fcr31 & 0x800000u) != 0u) {
            f4 = ((float)(f0));
            goto L_107d788;
        }
        at = 0xBFBD0000u;
        at = at | 0xE44Eu;
        { uint32_t bits = at; std::memcpy(&f0, &bits, sizeof(bits)); }
        fcr31 = (a1f::lt(f5, f0)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
        if ((fcr31 & 0x800000u) != 0u) {
            f5 = ((float)(f0));
        } else {
        }
L_107d788:
        fcr31 = (a1f::lt(f4, f13)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
        if ((fcr31 & 0x800000u) != 0u) {
            { uint32_t bits = 0u; std::memcpy(&f0, &bits, sizeof(bits)); }
            fcr31 = (a1f::lt(f2, f0)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
            goto L_107d7bc;
        }
        fcr31 = (a1f::lt(f13, f5)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
        if ((fcr31 & 0x800000u) == 0u) {
            f1 = a1f::add(f13, f2);
            goto L_107d7d0;
        }
        { uint32_t bits = 0u; std::memcpy(&f0, &bits, sizeof(bits)); }
        fcr31 = (a1f::lt(f0, f2)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
L_107d7bc:
        if ((fcr31 & 0x800000u) != 0u) {
            f1 = a1f::add(f13, f2);
            goto L_107d7f4;
        }
        goto L_107d7f4;
L_107d7d0:
        fcr31 = (a1f::lt(f4, f1)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
        if ((fcr31 & 0x800000u) != 0u) {
            f1 = ((float)(f4));
            goto L_107d7f4;
        }
        fcr31 = (a1f::lt(f1, f5)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
        if ((fcr31 & 0x800000u) != 0u) {
            f1 = ((float)(f5));
        } else {
        }
    }
L_107d7f4:
    if ((int64_t)a0 != 0) {
        { float f = f3; uint32_t bits; std::memcpy(&bits, &f, sizeof(bits)); A1_W32B((uint32_t)a0, 0, bits); }
    } else {
    }
    if ((int64_t)a1 != 0) {
        { float f = f1; uint32_t bits; std::memcpy(&bits, &f, sizeof(bits)); A1_W32B((uint32_t)a1, 0, bits); }
    } else {
    }
    fcr31 = (a1f::le(f3, f12)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
    if ((fcr31 & 0x800000u) != 0u) {
        fcr31 = (a1f::le(f12, f1)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
        if ((fcr31 & 0x800000u) != 0u) {
            goto L_107d834;
        }
    }
    fcr31 = (a1f::le(f1, f12)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
    if ((fcr31 & 0x800000u) == 0u) {
        goto L_107d840;
    }
    fcr31 = (a1f::le(f12, f3)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
    if ((fcr31 & 0x800000u) == 0u) {
        goto L_107d840;
    }
L_107d834:
    {
        (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 1))));
        return a1::RetVF{(uint64_t)v0, f0};
    }
L_107d840:
    {
        (v0 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL));
        return a1::RetVF{(uint64_t)v0, f0};
    }
}

// GAMEUTIL_is_nearly_equal_zero (0x107ec28): typed by tools/typed_lift.py (round A1)
inline a1::RetVF GAMEUTIL_is_nearly_equal_zero(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, float f12, float f13)
{
    uint64_t v0 = 0;
    float f0 = 0.0f;
    uint32_t fcr31 = 0;
    f0 = a1_flt(0u);
    fcr31 = (a1f::lt(f12, f0)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
    if ((fcr31 & 0x800000u) != 0u) {
        f0 = (-(float)(f12));
        fcr31 = (a1f::le(f0, f13)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
        if ((fcr31 & 0x800000u) != 0u) {
            goto L_107ec5c;
        }
        {
            (v0 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL));
            return a1::RetVF{(uint64_t)v0, f0};
        }
    }
    fcr31 = (a1f::le(f12, f13)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
    if ((fcr31 & 0x800000u) == 0u) {
        (v0 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL));
        goto L_107ec68;
    }
L_107ec5c:
    {
        (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 1))));
        return a1::RetVF{(uint64_t)v0, f0};
    }
L_107ec68:
    {
        return a1::RetVF{(uint64_t)v0, f0};
    }
}

// GAMEUTIL_is_nearly_equal_zero3D (0x107ec70): typed by tools/typed_lift.py (round A1)
inline a1::RetVF GAMEUTIL_is_nearly_equal_zero3D(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t a0, float f12)
{
    uint64_t v0 = 0;
    float f0 = 0.0f;
    float f1 = 0.0f;
    uint32_t fcr31 = 0;
    f0 = a1_flt(0u);
    f1 = a1_flt(A1_R32(a0));
    fcr31 = (a1f::lt(f1, f0)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
    if ((fcr31 & 0x800000u) != 0u) {
        f0 = (-(float)(f1));
        fcr31 = (a1f::le(f0, f12)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
        if ((fcr31 & 0x800000u) != 0u) {
            f1 = a1_flt(A1_R32B(a0, 4));
            goto L_107ecb0;
        }
        {
            (v0 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL));
            return a1::RetVF{(uint64_t)v0, f0};
        }
    }
    fcr31 = (a1f::le(f1, f12)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
    {
        const bool c = (fcr31 & 0x800000u) == 0u;
        (v0 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL));
        if (c) {
            goto L_107ed28;
        }
    }
    f1 = a1_flt(A1_R32B(a0, 4));
L_107ecb0:
    f0 = a1_flt(0u);
    fcr31 = (a1f::lt(f1, f0)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
    if ((fcr31 & 0x800000u) != 0u) {
        f0 = (-(float)(f1));
        fcr31 = (a1f::le(f0, f12)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
        if ((fcr31 & 0x800000u) != 0u) {
            f1 = a1_flt(A1_R32B(a0, 8));
            goto L_107ece8;
        }
        {
            (v0 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL));
            return a1::RetVF{(uint64_t)v0, f0};
        }
    }
    fcr31 = (a1f::le(f1, f12)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
    {
        const bool c = (fcr31 & 0x800000u) == 0u;
        (v0 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL));
        if (c) {
            goto L_107ed28;
        }
    }
    f1 = a1_flt(A1_R32B(a0, 8));
L_107ece8:
    f0 = a1_flt(0u);
    fcr31 = (a1f::lt(f1, f0)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
    if ((fcr31 & 0x800000u) != 0u) {
        f0 = (-(float)(f1));
        fcr31 = (a1f::le(f0, f12)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
        if ((fcr31 & 0x800000u) != 0u) {
            goto L_107ed1c;
        }
        {
            (v0 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL));
            return a1::RetVF{(uint64_t)v0, f0};
        }
    }
    fcr31 = (a1f::le(f1, f12)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
    if ((fcr31 & 0x800000u) == 0u) {
        (v0 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL));
        goto L_107ed28;
    }
L_107ed1c:
    {
        (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 1))));
        return a1::RetVF{(uint64_t)v0, f0};
    }
L_107ed28:
    {
        return a1::RetVF{(uint64_t)v0, f0};
    }
}

// GAMEUTIL_get_answer_of_2jihouteishiki (0x107ee48): typed by tools/typed_lift.py (round A1)
inline a1::RetVF GAMEUTIL_get_answer_of_2jihouteishiki(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t a0, uint32_t a1, float f12, float f13, float f14)
{
    uint32_t at = 0;
    uint32_t v0 = 0;
    float f0 = 0.0f;
    float f1 = 0.0f;
    float f2 = 0.0f;
    float f3 = 0.0f;
    uint32_t fcr31 = 0;
    at = 0x40800000u;
    f0 = a1_flt(at);
    f1 = a1f::mul(f13, f13);
    f2 = a1_flt(0u);
    f0 = a1f::mul(f12, f0);
    f0 = a1f::mul(f0, f14);
    f3 = a1f::subFinite(f1, f0);
    fcr31 = (a1f::lt(f3, f2)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
    {
        const bool c = (fcr31 & 0x800000u) == 0u;
        v0 = 1u;
        if (!c) {
            v0 = 0u;
        }
    }
    f3 = a1f::sqrt(fcr31, f3);
    f0 = (-(float)(f13));
    f2 = a1f::add(f12, f12);
    f1 = a1f::sub(f3, f13);
    f0 = a1f::sub(f0, f3);
    f1 = a1f::div(fcr31, f1, f2);
    f0 = a1f::div(fcr31, f0, f2);
    A1_W32(a0, a1_bits(f1));
    A1_W32(a1, a1_bits(f0));
    f1 = a1_flt(A1_R32(a0));
    fcr31 = (a1f::lt(f1, f0)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
    if ((fcr31 & 0x800000u) != 0u) {
        A1_W32(a0, a1_bits(f0));
        A1_W32(a1, a1_bits(f1));
    }
    {
        return a1::RetVF{(uint64_t)a1_sx64(v0), f0};
    }
}

// GAMEUTIL_get_new_velocity_by_brake_abs (0x107fb30): typed by tools/typed_lift.py (round A1)
inline float GAMEUTIL_get_new_velocity_by_brake_abs(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, float f12, float f13, float f14, float f15)
{
    float f0 = 0.0f;
    uint32_t fcr31 = 0;
    f0 = a1_flt(0u);
    fcr31 = (a1f::le(f0, f13)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
    {
        const bool c = (fcr31 & 0x800000u) == 0u;
        f0 = ((float)(f14));
        if (!c) {
            f0 = a1f::mul(f13, f15);
            f0 = a1f::sub(f12, f0);
            fcr31 = (a1f::lt(f0, f14)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
            if ((fcr31 & 0x800000u) != 0u) {
                f0 = ((float)(f14));
            } else {
            }
        }
    }
    {
        return f0;
    }
}

// GAMEUTIL_get_velocity_by_diff_move (0x107fb60): typed by tools/typed_lift.py (round A1)
inline float GAMEUTIL_get_velocity_by_diff_move(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint64_t a0, float f12, float f13, float f14, float f15)
{
    float f0 = 0.0f;
    float f1 = 0.0f;
    uint32_t fcr31 = 0;
    { uint32_t bits = 0u; std::memcpy(&f1, &bits, sizeof(bits)); }
    fcr31 = (a1f::eq(f15, f1)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
    {
        const bool c = (fcr31 & 0x800000u) != 0u;
        f0 = ((float)(f12));
        if (!c) {
            f0 = a1f::sub(f14, f13);
            f0 = a1f::div(fcr31, f0, f15);
            if ((int64_t)a0 != 0) {
                fcr31 = (a1f::lt(f0, f1)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
                if ((fcr31 & 0x800000u) != 0u) {
                    f0 = (-(float)(f0));
                } else {
                }
            }
        }
    }
    {
        return f0;
    }
}

// GAMEUTIL_mul_ratio (0x107fed0): typed by tools/typed_lift.py (round A1)
inline float GAMEUTIL_mul_ratio(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint64_t a0, float f12, float f13)
{
    uint32_t at = 0;
    float f0 = 0.0f;
    float f1 = 0.0f;
    uint32_t fcr31 = 0;
    bool df_at = false;
    if ((int64_t)a0 == 0) {
        f0 = a1f::mul(f13, f12);
    } else {
        { uint32_t bits = 0u; std::memcpy(&f0, &bits, sizeof(bits)); }
        fcr31 = (a1f::le(f13, f0)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
        if ((fcr31 & 0x800000u) == 0u) {
            at = 0x3F800000u; df_at = true;
            { uint32_t bits = at; std::memcpy(&f1, &bits, sizeof(bits)); }
            fcr31 = (a1f::le(f1, f13)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
            {
                const bool c = (fcr31 & 0x800000u) != 0u;
                f0 = ((float)(f12));
                if (!c) {
                    f0 = a1f::mul(f13, f12);
                }
            }
        }
    }
    {
        return f0;
    }
}

// GAMEUTIL_convert_from_lw_to_game_coord (0x10802f0): typed by tools/typed_lift.py (round A1)
inline float GAMEUTIL_convert_from_lw_to_game_coord(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t a0)
{
    float f0 = 0.0f;
    float f1 = 0.0f;
    f0 = a1_flt(A1_R32(a0));
    f1 = a1_flt(A1_R32B(a0, 8));
    f0 = (-(float)(f0));
    f1 = (-(float)(f1));
    A1_W32(a0, a1_bits(f0));
    {
        A1_W32B(a0, 8, a1_bits(f1));
        return f0;
    }
}

// GAMEUTIL_get_difference_pitch_and_pitch (0x1080b08): typed by tools/typed_lift.py (round A1)
inline float GAMEUTIL_get_difference_pitch_and_pitch(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, float f12, float f13)
{
    float f0 = 0.0f;
    uint32_t fcr31 = 0;
    f13 = a1f::sub(f13, f12);
    f0 = a1_flt(0u);
    fcr31 = (a1f::lt(f13, f0)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
    if ((fcr31 & 0x800000u) != 0u) {
        f13 = (-(float)(f13));
    } else {
    }
    {
        f0 = ((float)(f13));
        return f0;
    }
}

// GAMEUTIL_get_move_radi_from_yaw1_to_yaw2 (0x1080b28): typed by tools/typed_lift.py (round A1)
inline a1::RetVF GAMEUTIL_get_move_radi_from_yaw1_to_yaw2(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, float f12, float f13)
{
    uint32_t at = 0;
    uint32_t v0 = 0;
    float f0 = 0.0f;
    float f1 = 0.0f;
    uint32_t fcr31 = 0;
    f1 = a1f::sub(f13, f12);
    f0 = a1_flt(0u);
    fcr31 = (a1f::lt(f1, f0)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
    if ((fcr31 & 0x800000u) != 0u) {
        f1 = (-(float)(f1));
    } else {
    }
    fcr31 = (a1f::lt(f13, f12)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
    {
        const bool c = (fcr31 & 0x800000u) == 0u;
        v0 = 1u;
        if (!c) {
            v0 = 0u;
        }
    }
    at = 0x40490000u;
    at = at | 0xFDAu;
    f0 = a1_flt(at);
    fcr31 = (a1f::lt(f0, f1)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
    if ((fcr31 & 0x800000u) != 0u) {
        at = 0x40C90000u;
        at = at | 0xFDAu;
        f0 = a1_flt(at);
        v0 = v0 < 1u ? 1u : 0u;
        f1 = a1f::sub(f0, f1);
    }
    {
        const bool c = v0 != 0;
        f0 = ((float)(f1));
        if (!c) {
            f0 = (-(float)(f1));
        }
    }
    {
        return a1::RetVF{(uint64_t)a1_sx64(v0), f0};
    }
}

// GAMEUTIL_get_move_radi_from_pitch1_to_pitch2 (0x1080b90): typed by tools/typed_lift.py (round A1)
inline float GAMEUTIL_get_move_radi_from_pitch1_to_pitch2(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, float f12, float f13)
{
    float f0 = 0.0f;
    {
        f0 = a1f::sub(f13, f12);
        return f0;
    }
}

// GAMEUTIL_is_between_yaw_and_yaw (0x1080b98): typed by tools/typed_lift.py (round A1)
inline a1::RetVF GAMEUTIL_is_between_yaw_and_yaw(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, float f12, float f13, float f14)
{
    uint32_t at = 0;
    uint64_t v0 = 0;
    float f0 = 0.0f;
    float f1 = 0.0f;
    uint32_t fcr31 = 0;
    f1 = a1f::sub(f13, f14);
    f0 = a1_flt(0u);
    fcr31 = (a1f::lt(f1, f0)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
    {
        const bool c = (fcr31 & 0x800000u) == 0u;
        (v0 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL));
        if (!c) {
            at = 0xC0490000u;
            at = at | 0xFDAu;
            f0 = a1_flt(at);
            fcr31 = (a1f::lt(f1, f0)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
            if ((fcr31 & 0x800000u) != 0u) {
                (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 1))));
                goto L_1080be8;
            }
            goto L_1080be8;
        }
    }
    at = 0x40490000u;
    at = at | 0xFDAu;
    f0 = a1_flt(at);
    fcr31 = (a1f::lt(f0, f1)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
    if ((fcr31 & 0x800000u) != 0u) {
        (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 1))));
    } else {
    }
L_1080be8:
    fcr31 = (a1f::le(f13, f12)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
    if ((fcr31 & 0x800000u) != 0u) {
        fcr31 = (a1f::le(f12, f14)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
        if ((fcr31 & 0x800000u) != 0u) {
            goto L_1080c18;
        }
    }
    fcr31 = (a1f::le(f14, f12)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
    if ((fcr31 & 0x800000u) == 0u) {
        goto L_1080c20;
    }
    fcr31 = (a1f::le(f12, f13)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
    if ((fcr31 & 0x800000u) == 0u) {
        goto L_1080c20;
    }
L_1080c18:
    {
        (v0 = (uint64_t)(((uint64_t)v0 < (uint64_t)(int64_t)(int32_t)1) ? 1 : 0));
        return a1::RetVF{(uint64_t)v0, f0};
    }
L_1080c20:
    {
        (v0 = (uint64_t)(((uint64_t)0ULL < (uint64_t)v0) ? 1 : 0));
        return a1::RetVF{(uint64_t)v0, f0};
    }
}

// GAMEUTIL_is_between_pitch_and_pitch (0x1080d58): typed by tools/typed_lift.py (round A1)
inline uint64_t GAMEUTIL_is_between_pitch_and_pitch(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, float f12, float f13, float f14)
{
    uint64_t v0 = 0;
    uint32_t fcr31 = 0;
    fcr31 = (a1f::le(f13, f12)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
    if ((fcr31 & 0x800000u) != 0u) {
        fcr31 = (a1f::le(f12, f14)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
        if ((fcr31 & 0x800000u) != 0u) {
            goto L_1080d88;
        }
    }
    fcr31 = (a1f::le(f14, f12)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
    if ((fcr31 & 0x800000u) == 0u) {
        goto L_1080d90;
    }
    fcr31 = (a1f::le(f12, f13)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
    if ((fcr31 & 0x800000u) == 0u) {
        goto L_1080d90;
    }
L_1080d88:
    {
        (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 1))));
        return v0;
    }
L_1080d90:
    {
        (v0 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL));
        return v0;
    }
}

// GAMEUTIL_is_reach_aim_yaw_angle (0x1080f50): typed by tools/typed_lift.py (round A1)
inline a1::RetVF GAMEUTIL_is_reach_aim_yaw_angle(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint64_t a0, float f12, float f13, float f14)
{
    uint32_t at = 0;
    uint64_t v0 = 0;
    uint64_t v1 = 0;
    float f0 = 0.0f;
    float f1 = 0.0f;
    float f2 = 0.0f;
    uint32_t fcr31 = 0;
    f1 = a1f::sub(f12, f13);
    { uint32_t bits = 0u; std::memcpy(&f0, &bits, sizeof(bits)); }
    fcr31 = (a1f::lt(f1, f0)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
    {
        const bool c = (fcr31 & 0x800000u) == 0u;
        (v0 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL));
        if (!c) {
            at = 0xC0490000u;
            at = at | 0xFDAu;
            { uint32_t bits = at; std::memcpy(&f0, &bits, sizeof(bits)); }
            fcr31 = (a1f::lt(f1, f0)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
            if ((fcr31 & 0x800000u) != 0u) {
                (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 1))));
                goto L_1080fa0;
            }
            goto L_1080fa0;
        }
    }
    at = 0x40490000u;
    at = at | 0xFDAu;
    { uint32_t bits = at; std::memcpy(&f0, &bits, sizeof(bits)); }
    fcr31 = (a1f::lt(f0, f1)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
    if ((fcr31 & 0x800000u) != 0u) {
        (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 1))));
    } else {
    }
L_1080fa0:
    fcr31 = (a1f::le(f12, f14)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
    if ((fcr31 & 0x800000u) != 0u) {
        fcr31 = (a1f::le(f14, f13)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
        {
            const bool c = (fcr31 & 0x800000u) != 0u;
            (v1 = (uint64_t)(((uint64_t)v0 < (uint64_t)(int64_t)(int32_t)1) ? 1 : 0));
            if (c) {
                goto L_1080fd4;
            }
        }
    }
    fcr31 = (a1f::le(f13, f14)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
    {
        const bool c = (fcr31 & 0x800000u) == 0u;
        (v1 = (uint64_t)(((uint64_t)0ULL < (uint64_t)v0) ? 1 : 0));
        if (!c) {
            fcr31 = (a1f::le(f14, f12)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
            if ((fcr31 & 0x800000u) == 0u) {
                (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 1))));
                goto L_1080fd8;
            }
            (v1 = (uint64_t)(((uint64_t)v0 < (uint64_t)(int64_t)(int32_t)1) ? 1 : 0));
        }
    }
L_1080fd4:
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 1))));
L_1080fd8:
    if ((int64_t)v1 != (int64_t)v0) {
        f1 = a1f::sub(f14, f13);
        { uint32_t bits = 0u; std::memcpy(&f2, &bits, sizeof(bits)); }
        fcr31 = (a1f::lt(f1, f2)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
        if ((fcr31 & 0x800000u) != 0u) {
            f1 = (-(float)(f1));
        } else {
        }
        at = 0x40490000u;
        at = at | 0xFDAu;
        { uint32_t bits = at; std::memcpy(&f0, &bits, sizeof(bits)); }
        fcr31 = (a1f::lt(f0, f1)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
        if ((fcr31 & 0x800000u) != 0u) {
            at = 0x40C90000u;
            at = at | 0xFDAu;
            { uint32_t bits = at; std::memcpy(&f0, &bits, sizeof(bits)); }
            f1 = a1f::sub(f0, f1);
        }
        fcr31 = (a1f::lt(f1, f2)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
        at = 0x38D10000u;
        at = at | 0xB717u;
        { uint32_t bits = at; std::memcpy(&f2, &bits, sizeof(bits)); }
        if ((fcr31 & 0x800000u) != 0u) {
            f0 = (-(float)(f1));
            fcr31 = (a1f::le(f0, f2)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
            if ((fcr31 & 0x800000u) != 0u) {
                (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 1))));
                goto L_1081060;
            }
            (v1 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL));
            goto L_1081060;
        }
        fcr31 = (a1f::le(f1, f2)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
        if ((fcr31 & 0x800000u) == 0u) {
            (v1 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL));
        } else {
            (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 1))));
        }
L_1081060:
        (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 1))));
        if ((int64_t)v1 != (int64_t)v0) {
            goto L_1081080;
        }
    }
    if ((int64_t)a0 != 0) {
        { float f = f14; uint32_t bits; std::memcpy(&bits, &f, sizeof(bits)); A1_W32B((uint32_t)a0, 0, bits); }
    } else {
    }
    {
        (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 1))));
        return a1::RetVF{(uint64_t)v0, f0};
    }
L_1081080:
    if ((int64_t)a0 != 0) {
        { float f = f13; uint32_t bits; std::memcpy(&bits, &f, sizeof(bits)); A1_W32B((uint32_t)a0, 0, bits); }
    } else {
    }
    {
        (v0 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL));
        return a1::RetVF{(uint64_t)v0, f0};
    }
}

// GAMEUTIL_is_nearly_equal_yaw (0x10829f8): typed by tools/typed_lift.py (round A1)
inline a1::RetVF GAMEUTIL_is_nearly_equal_yaw(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, float f12, float f13, float f14)
{
    uint32_t at = 0;
    uint32_t v0 = 0;
    float f0 = 0.0f;
    uint32_t fcr31 = 0;
    f13 = a1f::sub(f13, f12);
    f0 = a1_flt(0u);
    fcr31 = (a1f::lt(f13, f0)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
    if ((fcr31 & 0x800000u) != 0u) {
        f13 = (-(float)(f13));
    } else {
    }
    at = 0x40490000u;
    at = at | 0xFDAu;
    f0 = a1_flt(at);
    fcr31 = (a1f::lt(f0, f13)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
    if ((fcr31 & 0x800000u) != 0u) {
        at = 0x40C90000u;
        at = at | 0xFDAu;
        f0 = a1_flt(at);
        f13 = a1f::sub(f0, f13);
    }
    fcr31 = (a1f::le(f13, f14)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
    {
        const bool c = (fcr31 & 0x800000u) != 0u;
        v0 = 1u;
        if (!c) {
            v0 = 0u;
        }
    }
    {
        return a1::RetVF{(uint64_t)a1_sx64(v0), f0};
    }
}

// GAMEUTIL_is_nearly_equal_pitch (0x1082a50): typed by tools/typed_lift.py (round A1)
inline a1::RetVF GAMEUTIL_is_nearly_equal_pitch(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, float f12, float f13, float f14)
{
    uint32_t v0 = 0;
    float f0 = 0.0f;
    uint32_t fcr31 = 0;
    f13 = a1f::sub(f13, f12);
    f0 = a1_flt(0u);
    fcr31 = (a1f::lt(f13, f0)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
    if ((fcr31 & 0x800000u) != 0u) {
        f13 = (-(float)(f13));
    } else {
    }
    fcr31 = (a1f::le(f13, f14)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
    {
        const bool c = (fcr31 & 0x800000u) != 0u;
        v0 = 1u;
        if (!c) {
            v0 = 0u;
        }
    }
    {
        return a1::RetVF{(uint64_t)a1_sx64(v0), f0};
    }
}

// manager_sub_01082C78 (0x1082c78): typed by tools/typed_lift.py (round A1)
inline a1::RetVF manager_sub_01082C78(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, float f12)
{
    uint32_t at = 0;
    uint64_t v0 = 0;
    float f0 = 0.0f;
    float f1 = 0.0f;
    uint32_t fcr31 = 0;
    f0 = a1_flt(0u);
    at = 0x38D10000u;
    at = at | 0xB717u;
    f1 = a1_flt(at);
    fcr31 = (a1f::lt(f12, f0)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
    if ((fcr31 & 0x800000u) != 0u) {
        f0 = (-(float)(f12));
        fcr31 = (a1f::le(f0, f1)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
        if ((fcr31 & 0x800000u) != 0u) {
            (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 1))));
            goto L_1082cc0;
        }
        (v0 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL));
        goto L_1082cc0;
    }
    fcr31 = (a1f::le(f12, f1)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
    if ((fcr31 & 0x800000u) == 0u) {
        (v0 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL));
    } else {
        (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 1))));
    }
L_1082cc0:
    at = 0x3F800000u;
    f0 = a1_flt(at);
    if ((int64_t)v0 == 0) {
        f0 = ((float)(f12));
    } else {
    }
    {
        return a1::RetVF{(uint64_t)v0, f0};
    }
}

// manager_sub_01082CD8 (0x1082cd8): typed by tools/typed_lift.py (round A1)
inline a1::RetVF manager_sub_01082CD8(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, float f12, float f13, float f14)
{
    uint32_t at = 0;
    uint64_t v0 = 0;
    float f0 = 0.0f;
    float f1 = 0.0f;
    float f2 = 0.0f;
    uint32_t fcr31 = 0;
    f1 = a1_flt(0u);
    at = 0x38D10000u;
    at = at | 0xB717u;
    { uint32_t bits = at; std::memcpy(&f2, &bits, sizeof(bits)); }
    fcr31 = (a1f::lt(f14, f1)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
    {
        const bool c = (fcr31 & 0x800000u) == 0u;
        f0 = ((float)(f13));
        if (!c) {
            f1 = (-(float)(f14));
            fcr31 = (a1f::le(f1, f2)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
            if ((fcr31 & 0x800000u) != 0u) {
                (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 1))));
                goto L_1082d20;
            }
            (v0 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL));
            goto L_1082d20;
        }
    }
    fcr31 = (a1f::le(f14, f2)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
    if ((fcr31 & 0x800000u) == 0u) {
        (v0 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL));
    } else {
        (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 1))));
    }
L_1082d20:
    if ((int64_t)v0 == 0) {
        f0 = ((float)(f12));
    } else {
    }
    {
        return a1::RetVF{(uint64_t)v0, f0};
    }
}

// get_fast_get_malloc_partition (0x1082da0): typed by tools/typed_lift.py (round A1)
inline uint32_t get_fast_get_malloc_partition(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt)
{
    uint32_t v0 = 0;
    uint32_t v1 = 0;
    v1 = 0x1180000u;
    {
        v0 = A1_R32B(v1, 25168);
        return v0;
    }
}

// GAMEUTIL_doit_interp_ratio_cntrl (0x10832a8): typed by tools/typed_lift.py (round A1)
inline a1::RetVF GAMEUTIL_doit_interp_ratio_cntrl(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t a0, float f12)
{
    uint32_t at = 0;
    uint32_t v0 = 0;
    uint32_t v1 = 0;
    float f0 = 0.0f;
    float f1 = 0.0f;
    float f2 = 0.0f;
    float f3 = 0.0f;
    uint32_t fcr31 = 0;
    bool df_at = false;
    v0 = A1_R32(a0);
    v1 = 1u;
    if (v0 != v1) {
        A1_W32B(a0, 8, v1);
    } else {
        f3 = a1_flt(A1_R32B(a0, 12));
        f0 = a1_flt(0u);
        fcr31 = (a1f::lt(f0, f3)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
        if ((fcr31 & 0x800000u) != 0u) {
            A1_W32B(a0, 8, 0u);
        } else {
        }
        fcr31 = (a1f::le(f0, f12)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
        if ((fcr31 & 0x800000u) == 0u) {
            f0 = a1_flt(A1_R32B(a0, 28));
        } else {
            f0 = a1f::add(f3, f12);
            goto L_10832e8;
        }
        f0 = a1f::mul(f12, f0);
        f0 = a1f::add(f3, f0);
L_10832e8:
        A1_W32B(a0, 12, a1_bits(f0));
        v1 = A1_R32B(a0, 4);
        v0 = 1u;
        {
            const bool c = v1 != v0;
            f3 = a1_flt(A1_R32B(a0, 12));
            if (!c) {
                f0 = a1_flt(0u);
                fcr31 = (a1f::lt(f3, f0)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
                if ((fcr31 & 0x800000u) == 0u) {
                    f0 = a1_flt(A1_R32B(a0, 20));
                } else {
                    A1_W32B(a0, 12, a1_bits(f0));
                    f3 = ((float)(f0));
                    f0 = a1_flt(A1_R32B(a0, 20));
                }
                f1 = a1_flt(A1_R32B(a0, 24));
                f2 = a1f::add(f0, f1);
                fcr31 = (a1f::lt(f2, f3)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
                if ((fcr31 & 0x800000u) == 0u) {
                    goto L_1083340;
                }
                A1_W32B(a0, 12, a1_bits(f2));
                f3 = ((float)(f2));
                goto L_1083340;
            }
        }
        f0 = a1_flt(A1_R32B(a0, 20));
        f1 = a1_flt(A1_R32B(a0, 24));
L_1083340:
        fcr31 = (a1f::le(f3, f0)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
        f2 = a1f::add(f0, f1);
        f1 = a1_flt(0u);
        if ((fcr31 & 0x800000u) != 0u) {
            A1_W32B(a0, 16, a1_bits(f1));
        } else {
            fcr31 = (a1f::le(f2, f3)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
            at = 0x3F800000u; df_at = true;
            f1 = a1_flt(at);
            if ((fcr31 & 0x800000u) != 0u) {
                A1_W32B(a0, 16, a1_bits(f1));
            } else {
                f1 = a1f::sub(f2, f0);
                f0 = a1f::sub(f3, f0);
                f1 = a1f::div(fcr31, f0, f1);
                A1_W32B(a0, 16, a1_bits(f1));
            }
        }
        {
            f0 = ((float)(f1));
            return a1::RetVF{(uint64_t)a1_sx64(v0), f0};
        }
    }
    at = 0x3F800000u; df_at = true;
    f0 = a1_flt(at);
    A1_W32B(a0, 16, a1_bits(f0));
    {
        A1_W32B(a0, 12, 0u);
        return a1::RetVF{(uint64_t)a1_sx64(v0), f0};
    }
}

// GAMEUTIL_get_dist_ratio (0x10836c0): typed by tools/typed_lift.py (round A1)
inline float GAMEUTIL_get_dist_ratio(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, float f12, float f13, float f14)
{
    uint32_t at = 0;
    float f0 = 0.0f;
    float f1 = 0.0f;
    uint32_t fcr31 = 0;
    bool df_at = false;
    fcr31 = (a1f::le(f12, f13)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
    f0 = a1_flt(0u);
    if ((fcr31 & 0x800000u) == 0u) {
        fcr31 = (a1f::le(f14, f12)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
        at = 0x3F800000u; df_at = true;
        f0 = a1_flt(at);
        if ((fcr31 & 0x800000u) == 0u) {
            f1 = a1f::sub(f14, f13);
            f0 = a1f::sub(f12, f13);
            f0 = a1f::div(fcr31, f0, f1);
        }
    }
    {
        return f0;
    }
}

// GAMEUTIL_get_interp_value (0x1083828): typed by tools/typed_lift.py (round A1)
inline float GAMEUTIL_get_interp_value(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, float f12, float f13, float f14)
{
    uint32_t at = 0;
    float f0 = 0.0f;
    float f1 = 0.0f;
    uint32_t fcr31 = 0;
    bool df_at = false;
    f0 = a1_flt(0u);
    fcr31 = (a1f::le(f14, f0)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
    {
        const bool c = (fcr31 & 0x800000u) != 0u;
        f0 = ((float)(f13));
        if (!c) {
            at = 0x3F800000u; df_at = true;
            f1 = a1_flt(at);
            fcr31 = (a1f::le(f1, f14)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
            {
                const bool c = (fcr31 & 0x800000u) != 0u;
                f0 = ((float)(f12));
                if (!c) {
                    fcr31 = (a1f::eq(f12, f13)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
                    if ((fcr31 & 0x800000u) == 0u) {
                        f0 = a1f::sub(f1, f14);
                        f1 = a1f::mul(f14, f12);
                        f0 = a1f::mul(f0, f13);
                        f0 = a1f::addFinite(f1, f0);
                    }
                }
            }
        }
    }
    {
        return f0;
    }
}

// GAMEUTIL_forward_timer_still_one_ratio_time_interp_ratio_cntrl (0x10842d0): typed by tools/typed_lift.py (round A1)
inline float GAMEUTIL_forward_timer_still_one_ratio_time_interp_ratio_cntrl(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t a0)
{
    float f0 = 0.0f;
    float f1 = 0.0f;
    f0 = a1_flt(A1_R32B(a0, 20));
    f1 = a1_flt(A1_R32B(a0, 24));
    f0 = a1f::add(f0, f1);
    {
        A1_W32B(a0, 12, a1_bits(f0));
        return f0;
    }
}

// GAMEUTIL_get_interp_ratio_interp_ratio_cntrl (0x10842e8): typed by tools/typed_lift.py (round A1)
inline float GAMEUTIL_get_interp_ratio_interp_ratio_cntrl(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t a0)
{
    float f0 = 0.0f;
    {
        f0 = a1_flt(A1_R32B(a0, 16));
        return f0;
    }
}

// GAMEUTIL_get_interp_timer_interp_ratio_cntrl (0x10842f0): typed by tools/typed_lift.py (round A1)
inline float GAMEUTIL_get_interp_timer_interp_ratio_cntrl(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t a0)
{
    float f0 = 0.0f;
    {
        f0 = a1_flt(A1_R32B(a0, 12));
        return f0;
    }
}

// GAMEUTIL_set_act_interp_flag_interp_ratio_cntrl (0x10842f8): typed by tools/typed_lift.py (round A1)
inline void GAMEUTIL_set_act_interp_flag_interp_ratio_cntrl(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t a0, uint32_t a1)
{
    {
        A1_W32(a0, a1);
        
        return;
    }
}

// GAMEUTIL_set_act_max_zero_clip_flag_interp_ratio_cntrl (0x1084300): typed by tools/typed_lift.py (round A1)
inline void GAMEUTIL_set_act_max_zero_clip_flag_interp_ratio_cntrl(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t a0, uint32_t a1)
{
    {
        A1_W32B(a0, 4, a1);
        
        return;
    }
}

// GAMEUTIL_get_interp_value_interp_value_cntrl (0x1084500): typed by tools/typed_lift.py (round A1)
inline float GAMEUTIL_get_interp_value_interp_value_cntrl(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t a0)
{
    float f0 = 0.0f;
    {
        f0 = a1_flt(A1_R32B(a0, 4));
        return f0;
    }
}

// GAMEUTIL_get_hope_value_interp_value_cntrl (0x1084508): typed by tools/typed_lift.py (round A1)
inline float GAMEUTIL_get_hope_value_interp_value_cntrl(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t a0)
{
    float f0 = 0.0f;
    {
        f0 = a1_flt(A1_R32B(a0, 8));
        return f0;
    }
}

// set_param_double_spring_interp_common (0x1084510): typed by tools/typed_lift.py (round A1)
inline void set_param_double_spring_interp_common(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t a0, float f12, float f13, float f14, float f15)
{
    A1_W32B(a0, 12, a1_bits(f15));
    A1_W32(a0, a1_bits(f12));
    A1_W32B(a0, 4, a1_bits(f13));
    {
        A1_W32B(a0, 8, a1_bits(f14));
        
        return;
    }
}

// set_rot_radi_value_flag_double_spring_interp_cntrl (0x1084588): typed by tools/typed_lift.py (round A1)
inline void set_rot_radi_value_flag_double_spring_interp_cntrl(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t a0, uint32_t a1)
{
    {
        A1_W32B(a0, 20, a1);
        
        return;
    }
}

// set_rot_radi_value_flag_double_spring_interp_cntrl3D (0x1084698): typed by tools/typed_lift.py (round A1)
inline void set_rot_radi_value_flag_double_spring_interp_cntrl3D(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t a0, uint32_t a1, uint32_t a2, uint32_t a3)
{
    A1_W32B(a0, 28, a3);
    A1_W32B(a0, 20, a1);
    {
        A1_W32B(a0, 24, a2);
        
        return;
    }
}

// constructor_aim_angle_interp_cntrl (0x1084880): typed by tools/typed_lift.py (round A1)
inline float constructor_aim_angle_interp_cntrl(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t a0, uint32_t a1)
{
    uint32_t at = 0;
    float f0 = 0.0f;
    float f1 = 0.0f;
    at = 0x40800000u;
    f0 = a1_flt(at);
    at = 0x3FC90000u;
    at = at | 0xFDAu;
    f1 = a1_flt(at);
    A1_W32(a0, a1);
    A1_W32B(a0, 4, a1_bits(f0));
    A1_W32B(a0, 8, a1_bits(f1));
    {
        A1_W32B(a0, 12, 0u);
        return f0;
    }
}

// reset_aim_angle_interp_cntrl (0x10848a8): typed by tools/typed_lift.py (round A1)
inline float reset_aim_angle_interp_cntrl(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t a0, float f12)
{
    float f0 = 0.0f;
    f0 = ((float)(f12));
    {
        A1_W32B(a0, 12, a1_bits(f0));
        return f0;
    }
}

// set_interp_param_aim_angle_interp_cntrl (0x1084968): typed by tools/typed_lift.py (round A1)
inline void set_interp_param_aim_angle_interp_cntrl(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t a0, float f12, float f13)
{
    A1_W32B(a0, 8, a1_bits(f13));
    {
        A1_W32B(a0, 4, a1_bits(f12));
        
        return;
    }
}

// manager_sub_01084978 (0x1084978): typed by tools/typed_lift.py (round A1)
inline a1::RetVF manager_sub_01084978(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t a0, float f12, float f13)
{
    uint32_t at = 0;
    uint32_t a1 = 0;
    uint64_t v0 = 0;
    float f0 = 0.0f;
    float f1 = 0.0f;
    float f2 = 0.0f;
    float f3 = 0.0f;
    float f4 = 0.0f;
    float f5 = 0.0f;
    uint32_t fcr31 = 0;
    f1 = a1_flt(0u);
    f0 = a1_flt(A1_R32B(a0, 12));
    fcr31 = (a1f::lt(f1, f0)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
    if ((fcr31 & 0x800000u) != 0u) {
        f0 = a1_flt(A1_R32B(a0, 20));
        fcr31 = (a1f::eq(f0, f12)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
        if ((fcr31 & 0x800000u) != 0u) {
            f0 = a1_flt(A1_R32B(a0, 24));
            fcr31 = (a1f::eq(f0, f13)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
            if ((fcr31 & 0x800000u) != 0u) {
                goto L_10849b8;
            }
        }
        {
            (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 1))));
            return a1::RetVF{(uint64_t)v0, f0};
        }
    }
L_10849b8:
    {
        (v0 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL));
        return a1::RetVF{(uint64_t)v0, f0};
    }
}

// getNowBossEffect (0x1087108): typed by tools/typed_lift.py (round A1)
inline uint32_t getNowBossEffect(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt)
{
    uint32_t v0 = 0;
    uint32_t v1 = 0;
    v1 = 0x1180000u;
    {
        v0 = A1_R32B(v1, 25232);
        return v0;
    }
}

// setBossDeathEffect (0x1087118): typed by tools/typed_lift.py (round A1)
inline a1::RetVF setBossDeathEffect(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint64_t a0, uint64_t a1, float f12)
{
    uint32_t at = 0;
    uint32_t v0 = 0;
    uint32_t a2 = 0;
    uint32_t a3 = 0;
    uint64_t v1 = 0;
    uint64_t t0 = 0;
    uint64_t t1 = 0;
    float f0 = 0.0f;
    float f1 = 0.0f;
    (t0 = (uint64_t)((uint64_t)a0 + (uint64_t)0ULL));
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)289 << 16))));
    a2 = 0x1180000u;
    (v0 = (uint32_t)((int32_t)((int32_t)SRL32((uint32_t)t0, 1))));
    a2 = a2 + 0x6290u;
    (t1 = (uint64_t)(t0 & (uint64_t)(uint16_t)1));
    (t1 = (uint64_t)(RC_U64X(t1) | a1_sx64(v0)));
    v0 = A1_R32(a2);
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)280 << 16))));
    a3 = 0x1180000u;
    v0 = v0 | 1u;
    { float f = f12; uint32_t bits; std::memcpy(&bits, &f, sizeof(bits)); A1_W32B((uint32_t)v1, 25244, bits); }
    A1_W32(a2, v0);
    (v0 = (uint32_t)((int32_t)((int32_t)SRL32((uint32_t)a1, 1))));
    (v1 = (uint64_t)(a1 & (uint64_t)(uint16_t)1));
    A1_W32B(a3, 25240, 0u);
    (v1 = (uint64_t)(RC_U64X(v1) | a1_sx64(v0)));
    (v0 = (uint32_t)((int32_t)((int32_t)ADD32((uint32_t)a0, 18240))));
    {
        const bool c = (int32_t)t0 < 0;
        (a0 = (uint64_t)((uint64_t)a1_sx64(v0) + (uint64_t)0ULL));
        if (!c) {
            { uint32_t bits = (uint32_t)t0; std::memcpy(&f1, &bits, sizeof(bits)); }
            { int32_t tmp; std::memcpy(&tmp, &f1, sizeof(tmp)); f1 = ((float)(int32_t)(tmp)); }
            goto L_1087184;
        }
    }
    { uint32_t bits = (uint32_t)t1; std::memcpy(&f1, &bits, sizeof(bits)); }
    { int32_t tmp; std::memcpy(&tmp, &f1, sizeof(tmp)); f1 = ((float)(int32_t)(tmp)); }
    f1 = a1f::add(f1, f1);
L_1087184:
    at = 0x39800000u;
    { uint32_t bits = at; std::memcpy(&f0, &bits, sizeof(bits)); }
    f0 = a1f::mul(f1, f0);
    {
        const bool c = (int32_t)a1 < 0;
        { float f = f0; uint32_t bits; std::memcpy(&bits, &f, sizeof(bits)); A1_W32B((uint32_t)a0, 0, bits); }
        if (!c) {
            { uint32_t bits = (uint32_t)a1; std::memcpy(&f1, &bits, sizeof(bits)); }
            { int32_t tmp; std::memcpy(&tmp, &f1, sizeof(tmp)); f1 = ((float)(int32_t)(tmp)); }
            goto L_10871b4;
        }
    }
    { uint32_t bits = (uint32_t)v1; std::memcpy(&f1, &bits, sizeof(bits)); }
    { int32_t tmp; std::memcpy(&tmp, &f1, sizeof(tmp)); f1 = ((float)(int32_t)(tmp)); }
    f1 = a1f::add(f1, f1);
L_10871b4:
    at = 0x39800000u;
    { uint32_t bits = at; std::memcpy(&f0, &bits, sizeof(bits)); }
    f0 = a1f::mul(f1, f0);
    {
        A1_W32B(v0, 4, a1_bits(f0));
        return a1::RetVF{(uint64_t)a1_sx64(v0), f0};
    }
}

// getBossEffectBlurLevel (0x1087200): typed by tools/typed_lift.py (round A1)
inline uint32_t getBossEffectBlurLevel(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt)
{
    uint32_t v0 = 0;
    v0 = 0x1180000u;
    {
        v0 = v0 + 0x62A0u;
        return v0;
    }
}

// getBossEffectColorR (0x1087210): typed by tools/typed_lift.py (round A1)
inline uint32_t getBossEffectColorR(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt)
{
    uint32_t v0 = 0;
    v0 = 0x1180000u;
    {
        v0 = v0 + 0x62B0u;
        return v0;
    }
}

// getBossEffectColorG (0x1087220): typed by tools/typed_lift.py (round A1)
inline uint32_t getBossEffectColorG(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt)
{
    uint32_t v0 = 0;
    v0 = 0x1180000u;
    {
        v0 = v0 + 0x62B4u;
        return v0;
    }
}

// getBossEffectColorB (0x1087230): typed by tools/typed_lift.py (round A1)
inline uint32_t getBossEffectColorB(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt)
{
    uint32_t v0 = 0;
    v0 = 0x1180000u;
    {
        v0 = v0 + 0x62B8u;
        return v0;
    }
}

// getBossEffectBlendRatio (0x1087240): typed by tools/typed_lift.py (round A1)
inline uint32_t getBossEffectBlendRatio(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt)
{
    uint32_t v0 = 0;
    v0 = 0x1180000u;
    {
        v0 = v0 + 0x62C0u;
        return v0;
    }
}

// getBossEffectOutLine (0x1087250): typed by tools/typed_lift.py (round A1)
inline uint32_t getBossEffectOutLine(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt)
{
    uint32_t v0 = 0;
    v0 = 0x1180000u;
    {
        v0 = v0 + 0x62C4u;
        return v0;
    }
}

// getDepthOfFieldSw (0x1088190): typed by tools/typed_lift.py (round A1)
inline uint32_t getDepthOfFieldSw(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt)
{
    uint32_t v0 = 0;
    v0 = 0x1180000u;
    {
        v0 = v0 + 0x62D0u;
        return v0;
    }
}

// getDepthOfFieldLevel (0x10881a0): typed by tools/typed_lift.py (round A1)
inline uint32_t getDepthOfFieldLevel(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt)
{
    uint32_t v0 = 0;
    v0 = 0x1180000u;
    {
        v0 = v0 + 0x62D4u;
        return v0;
    }
}

// getDepthOfFieldPlanes (0x10881b0): typed by tools/typed_lift.py (round A1)
inline uint32_t getDepthOfFieldPlanes(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt)
{
    uint32_t v0 = 0;
    v0 = 0x1180000u;
    {
        v0 = v0 + 0x62D8u;
        return v0;
    }
}

// getDepthOfFieldNearZ (0x10881c0): typed by tools/typed_lift.py (round A1)
inline uint32_t getDepthOfFieldNearZ(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt)
{
    uint32_t v0 = 0;
    v0 = 0x1180000u;
    {
        v0 = v0 + 0x62DCu;
        return v0;
    }
}

// fsEffectSetParamSheetSeg (0x1088a78): typed by tools/typed_lift.py (round A1)
inline uint32_t fsEffectSetParamSheetSeg(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint64_t a0)
{
    uint32_t v0 = 0;
    uint32_t v1 = 0;
    uint32_t a1 = 0;
    uint32_t a2 = 0;
    uint64_t hi = 0;
    uint64_t lo = 0;
    bool df_v1 = false, df_a1 = false;
    v0 = 0x30000u;
    (a2 = (uint32_t)((int32_t)((int32_t)A1_R32B((uint32_t)a0, 0))));
    v0 = v0 | 0xFFFFu;
    v0 = a2 & v0;
    {
        const bool c = v0 == 0;
        (v0 = (uint32_t)((int32_t)((int32_t)SUB32((uint32_t)a0, v0))));
        if (!c) {
            (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B(v0, 0))));
            goto L_1088a9c;
        }
    }
    (a0 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL));
L_1088a9c:
    {
        const bool c = (int64_t)a0 == 0;
        { const uint32_t _dynInsn = A1_R32(0x1088AA0u); const int32_t _dynSimm = (int16_t)(_dynInsn & 0xFFFFu); const uint32_t _dynImm = _dynInsn & 0xFFFFu; (void)_dynSimm; (void)_dynImm; (v0 = (uint32_t)((int32_t)((int32_t)((uint32_t)_dynImm << 16)))); }
        if (!c) {
            { const uint32_t _dynInsn = A1_R32(0x1088AA4u); const int32_t _dynSimm = (int16_t)(_dynInsn & 0xFFFFu); const uint32_t _dynImm = _dynInsn & 0xFFFFu; (void)_dynSimm; (void)_dynImm; (v1 = (uint32_t)((int32_t)((int32_t)A1_R32(ADD32(v0, _dynSimm))))); } df_v1 = true;
            {
                const bool c = (int64_t)a0 == (int64_t)(int32_t)v1;
                v0 = a2 >> 18;
                if (!c) {
                    (a1 = (uint32_t)((int32_t)((int32_t)A1_R32B((uint32_t)a0, 8)))); df_a1 = true;
                    v0 = v0 & 0xFFFu;
                    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)a0, 16))));
                    v1 = A1_R32B(a1, 12); df_v1 = true;
                    { int64_t result = (int64_t)(int32_t)v0 * (int64_t)(int32_t)v1; lo = (uint64_t)(int64_t)(int32_t)result; hi = (uint64_t)(int64_t)(int32_t)(result >> 32); (v0 = (uint32_t)((int32_t)((int32_t)result))); }
                    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)a0, v0))));
                    goto L_1088ad4;
                }
            }
        }
    }
    (a0 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL));
L_1088ad4:
    {
        const bool c = (int64_t)a0 == 0;
        v0 = 0x1180000u;
        if (!c) {
            a1 = v0 + 0x62F0u; df_a1 = true;
            v0 = A1_R32(a1);
            if (v0 == 0) {
                A1_W32B(a1, 0, (uint32_t)a0);
            } else {
                v1 = A1_R32B(v0, 4); df_v1 = true;
                (v0 = (uint32_t)((int32_t)((int32_t)A1_R32B((uint32_t)a0, 4))));
                v1 = v1 << 4; df_v1 = true;
                v0 = v0 << 4;
                v0 = (int32_t)v0 < (int32_t)v1 ? 1u : 0u;
                if (v0 == 0) {
                    A1_W32B(a1, 0, (uint32_t)a0);
                } else {
                }
            }
        }
    }
    {
        return v0;
    }
}

// getFogMode (0x10898f8): typed by tools/typed_lift.py (round A1)
inline uint32_t getFogMode(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt)
{
    uint32_t v0 = 0;
    v0 = 0x1210000u;
    {
        v0 = v0 + 0x47B0u;
        return v0;
    }
}

// getFogNearDist (0x1089908): typed by tools/typed_lift.py (round A1)
inline uint32_t getFogNearDist(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt)
{
    uint32_t v0 = 0;
    v0 = 0x1210000u;
    {
        v0 = v0 + 0x47B4u;
        return v0;
    }
}

// getFogFarDist (0x1089918): typed by tools/typed_lift.py (round A1)
inline uint32_t getFogFarDist(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt)
{
    uint32_t v0 = 0;
    v0 = 0x1210000u;
    {
        v0 = v0 + 0x47B8u;
        return v0;
    }
}

// getFogNearRate (0x1089928): typed by tools/typed_lift.py (round A1)
inline uint32_t getFogNearRate(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt)
{
    uint32_t v0 = 0;
    v0 = 0x1210000u;
    {
        v0 = v0 + 0x47BCu;
        return v0;
    }
}

// getFogFarRate (0x1089938): typed by tools/typed_lift.py (round A1)
inline uint32_t getFogFarRate(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt)
{
    uint32_t v0 = 0;
    v0 = 0x1210000u;
    {
        v0 = v0 + 0x47C0u;
        return v0;
    }
}

// getFogOffset (0x1089958): typed by tools/typed_lift.py (round A1)
inline uint32_t getFogOffset(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt)
{
    uint32_t v0 = 0;
    v0 = 0x1210000u;
    {
        v0 = v0 + 0x47C8u;
        return v0;
    }
}

// getFogUnderWater (0x1089968): typed by tools/typed_lift.py (round A1)
inline uint32_t getFogUnderWater(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt)
{
    uint32_t v0 = 0;
    v0 = 0x1180000u;
    {
        v0 = v0 + 0x6328u;
        return v0;
    }
}

// getFogColorR (0x1089978): typed by tools/typed_lift.py (round A1)
inline uint32_t getFogColorR(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt)
{
    uint32_t v0 = 0;
    v0 = 0x1210000u;
    {
        v0 = v0 + 0x47CCu;
        return v0;
    }
}

// getFogColorG (0x1089988): typed by tools/typed_lift.py (round A1)
inline uint32_t getFogColorG(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt)
{
    uint32_t v0 = 0;
    v0 = 0x1210000u;
    {
        v0 = v0 + 0x47D0u;
        return v0;
    }
}

// getFogColorB (0x1089998): typed by tools/typed_lift.py (round A1)
inline uint32_t getFogColorB(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt)
{
    uint32_t v0 = 0;
    v0 = 0x1210000u;
    {
        v0 = v0 + 0x47D4u;
        return v0;
    }
}

// SeemlessInitFogTargetParamWorldLayout (0x1089a38): typed by tools/typed_lift.py (round A1)
inline a1::RetVF SeemlessInitFogTargetParamWorldLayout(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t a0)
{
    uint32_t at = 0;
    uint32_t v0 = 0;
    uint32_t v1 = 0;
    uint32_t a1 = 0;
    uint32_t a2 = 0;
    uint32_t a3 = 0;
    float f0 = 0.0f;
    float f1 = 0.0f;
    float f2 = 0.0f;
    f0 = a1_flt(A1_R32B(a0, 32));
    a1 = 0x1210000u;
    a1 = a1 + 0x47B0u;
    at = 0x3F800000u;
    f2 = a1_flt(at);
    A1_W32B(a1, 4, a1_bits(f0));
    a3 = 0x1180000u;
    a2 = 0x78u;
    f0 = a1_flt(A1_R32B(a0, 36));
    A1_W32B(a1, 8, a1_bits(f0));
    f1 = a1_flt(A1_R32B(a0, 40));
    A1_W32B(a1, 12, a1_bits(f1));
    f0 = a1_flt(A1_R32B(a0, 44));
    A1_W32B(a1, 20, a1_bits(f2));
    A1_W32B(a1, 16, a1_bits(f0));
    v0 = A1_R8B(a0, 48);
    A1_W32B(a1, 24, v0);
    v1 = A1_R8B(a0, 49);
    A1_W32B(a1, 28, v1);
    v0 = A1_R8B(a0, 50);
    A1_W32B(a1, 32, v0);
    v1 = A1_R8B(a0, 51);
    A1_W32B(a3, 25376, a2);
    {
        A1_W32B(a1, 36, v1);
        return a1::RetVF{(uint64_t)a1_sx64(v0), f0};
    }
}

// fogSetFogColorMulRatio (0x1089b60): typed by tools/typed_lift.py (round A1)
inline uint32_t fogSetFogColorMulRatio(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, float f12)
{
    uint32_t v0 = 0;
    v0 = 0x1180000u;
    {
        A1_W32B(v0, 25380, a1_bits(f12));
        return v0;
    }
}

// getFogFactor1 (0x1089bb0): typed by tools/typed_lift.py (round A1)
inline a1::RetVF getFogFactor1(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt)
{
    uint32_t v0 = 0;
    float f0 = 0.0f;
    float f1 = 0.0f;
    float f2 = 0.0f;
    float f3 = 0.0f;
    float f4 = 0.0f;
    float f5 = 0.0f;
    uint32_t fcr31 = 0;
    v0 = 0x1210000u;
    v0 = v0 + 0x47D8u;
    f2 = a1_flt(A1_R32B(v0, 12));
    f3 = a1_flt(A1_R32B(v0, 16));
    f1 = a1_flt(A1_R32B(v0, 8));
    f4 = a1_flt(A1_R32B(v0, 4));
    f0 = a1f::sub(f3, f2);
    f3 = a1f::add(f3, f2);
    f5 = a1_flt(A1_R32B(v0, 20));
    f2 = a1f::add(f1, f4);
    f1 = a1f::sub(f1, f4);
    f0 = ((float)(f0) * (float)(f2));
    f0 = a1f::div(fcr31, f0, f1);
    f0 = a1f::addFinite(f0, f3);
    {
        f0 = a1f::mul(f0, f5);
        return a1::RetVF{(uint64_t)a1_sx64(v0), f0};
    }
}

// setFadeColor (0x108b970): typed by tools/typed_lift.py (round A1)
inline uint32_t setFadeColor(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint64_t a0)
{
    uint32_t v0 = 0;
    uint32_t v1 = 0;
    {
        const bool c = (int64_t)a0 == 0;
        v0 = 1u;
        if (!c) {
            {
                const bool c = (int64_t)a0 != (int64_t)(int32_t)v0;
                v0 = 0x1180000u;
                if (c) {
                    goto L_108b9ac;
                }
            }
            v1 = 0xC8u;
            v0 = v0 + 0x6340u;
            A1_W32B(v0, 8, v1);
            A1_W32(v0, v1);
            {
                A1_W32B(v0, 4, v1);
                return v0;
            }
        }
    }
    v0 = 0x1180000u;
    v0 = v0 + 0x6340u;
    A1_W32B(v0, 8, 0u);
    A1_W32(v0, 0u);
    A1_W32B(v0, 4, 0u);
L_108b9ac:
    {
        return v0;
    }
}

// setFadeLayer (0x108b9b8): typed by tools/typed_lift.py (round A1)
inline uint32_t setFadeLayer(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t a0)
{
    uint32_t v0 = 0;
    v0 = 0x1180000u;
    {
        A1_W32B(v0, 25424, a0);
        return v0;
    }
}

// getCameraBlurSw (0x108ba28): typed by tools/typed_lift.py (round A1)
inline uint32_t getCameraBlurSw(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt)
{
    uint32_t v0 = 0;
    v0 = 0x1180000u;
    {
        v0 = v0 + 0x6354u;
        return v0;
    }
}

// getBlurAlsoChara (0x108ba38): typed by tools/typed_lift.py (round A1)
inline uint32_t getBlurAlsoChara(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt)
{
    uint32_t v0 = 0;
    v0 = 0x1180000u;
    {
        v0 = v0 + 0x6358u;
        return v0;
    }
}

// getBlurWorkSize (0x108ba48): typed by tools/typed_lift.py (round A1)
inline uint32_t getBlurWorkSize(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt)
{
    uint32_t v0 = 0;
    v0 = 0x1180000u;
    {
        v0 = v0 + 0x635Cu;
        return v0;
    }
}

// getNBlurPlanes (0x108ba58): typed by tools/typed_lift.py (round A1)
inline uint32_t getNBlurPlanes(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt)
{
    uint32_t v0 = 0;
    v0 = 0x1180000u;
    {
        v0 = v0 + 0x6360u;
        return v0;
    }
}

// getBlurIntensity (0x108ba68): typed by tools/typed_lift.py (round A1)
inline uint32_t getBlurIntensity(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt)
{
    uint32_t v0 = 0;
    v0 = 0x1180000u;
    {
        v0 = v0 + 0x6364u;
        return v0;
    }
}

// getBlurZBase (0x108ba78): typed by tools/typed_lift.py (round A1)
inline uint32_t getBlurZBase(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt)
{
    uint32_t v0 = 0;
    v0 = 0x1180000u;
    {
        v0 = v0 + 0x636Cu;
        return v0;
    }
}

// getBlurMinThreshold (0x108ba88): typed by tools/typed_lift.py (round A1)
inline uint32_t getBlurMinThreshold(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt)
{
    uint32_t v0 = 0;
    v0 = 0x1180000u;
    {
        v0 = v0 + 0x6370u;
        return v0;
    }
}

// getBlurMaxThreshold (0x108ba98): typed by tools/typed_lift.py (round A1)
inline uint32_t getBlurMaxThreshold(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt)
{
    uint32_t v0 = 0;
    v0 = 0x1180000u;
    {
        v0 = v0 + 0x6374u;
        return v0;
    }
}

// getBlurShutterSpeed (0x108baa8): typed by tools/typed_lift.py (round A1)
inline uint32_t getBlurShutterSpeed(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt)
{
    uint32_t v0 = 0;
    v0 = 0x1180000u;
    {
        v0 = v0 + 0x6378u;
        return v0;
    }
}

// getBlurXYZRatio (0x108bab8): typed by tools/typed_lift.py (round A1)
inline uint32_t getBlurXYZRatio(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt)
{
    uint32_t v0 = 0;
    v0 = 0x1180000u;
    {
        v0 = v0 + 0x637Cu;
        return v0;
    }
}

// getBlurCamMoveThreshold (0x108bac8): typed by tools/typed_lift.py (round A1)
inline uint32_t getBlurCamMoveThreshold(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt)
{
    uint32_t v0 = 0;
    v0 = 0x1180000u;
    {
        v0 = v0 + 0x6380u;
        return v0;
    }
}

// glTexCoord2f (0x108e9c8): typed by tools/typed_lift.py (round A1)
inline a1::RetVF glTexCoord2f(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, float f12, float f13)
{
    uint32_t at = 0;
    uint32_t v0 = 0;
    float f0 = 0.0f;
    v0 = 0x1180000u;
    at = 0x3F800000u;
    f0 = a1_flt(at);
    v0 = v0 + 0x6390u;
    A1_W32B(v0, 160, a1_bits(f12));
    A1_W32B(v0, 164, a1_bits(f13));
    A1_W32B(v0, 168, a1_bits(f0));
    {
        A1_W32B(v0, 172, 0u);
        return a1::RetVF{(uint64_t)a1_sx64(v0), f0};
    }
}

// glTexCoord2fv (0x108e9f0): typed by tools/typed_lift.py (round A1)
inline a1::RetVF glTexCoord2fv(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t a0)
{
    uint32_t at = 0;
    uint32_t v0 = 0;
    float f0 = 0.0f;
    float f1 = 0.0f;
    float f2 = 0.0f;
    f1 = a1_flt(A1_R32(a0));
    v0 = 0x1180000u;
    v0 = v0 + 0x6390u;
    at = 0x3F800000u;
    f2 = a1_flt(at);
    A1_W32B(v0, 160, a1_bits(f1));
    f0 = a1_flt(A1_R32B(a0, 4));
    A1_W32B(v0, 168, a1_bits(f2));
    A1_W32B(v0, 164, a1_bits(f0));
    {
        A1_W32B(v0, 172, 0u);
        return a1::RetVF{(uint64_t)a1_sx64(v0), f0};
    }
}

// glZOffset (0x108f3b0): typed by tools/typed_lift.py (round A1)
inline uint32_t glZOffset(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, float f12)
{
    uint32_t v0 = 0;
    v0 = 0x1190000u;
    {
        A1_W32(v0 - 0x5A7Cu, a1_bits(f12));
        return v0;
    }
}

// glSetFogFactorDirect (0x108f608): typed by tools/typed_lift.py (round A1)
inline uint32_t glSetFogFactorDirect(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t a0)
{
    uint32_t v0 = 0;
    v0 = 0x1180000u;
    {
        A1_W32B(v0, 25584, a0);
        return v0;
    }
}

// gluGetBrightness (0x108f7e0): typed by tools/typed_lift.py (round A1)
inline float gluGetBrightness(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t a0)
{
    uint32_t at = 0;
    float f0 = 0.0f;
    float f1 = 0.0f;
    float f2 = 0.0f;
    float f3 = 0.0f;
    float f4 = 0.0f;
    at = 0x3E990000u;
    at = at | 0x1687u;
    f0 = a1_flt(at);
    f2 = a1_flt(A1_R32(a0));
    f3 = a1_flt(A1_R32B(a0, 4));
    at = 0x3F160000u;
    at = at | 0x45A1u;
    f4 = a1_flt(at);
    f2 = a1f::mul(f2, f0);
    at = 0x3DE90000u;
    at = at | 0x78D4u;
    f1 = a1_flt(at);
    f3 = a1f::mul(f3, f4);
    f0 = a1_flt(A1_R32B(a0, 8));
    f0 = a1f::mul(f0, f1);
    f2 = a1f::addFinite(f2, f3);
    {
        f0 = a1f::add(f2, f0);
        return f0;
    }
}

// getMBlur (0x108fba8): typed by tools/typed_lift.py (round A1)
inline uint32_t getMBlur(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt)
{
    uint32_t v0 = 0;
    v0 = 0x1190000u;
    {
        v0 = v0 - 0x48C0u;
        return v0;
    }
}

// getBrightness (0x108fbc8): typed by tools/typed_lift.py (round A1)
inline uint32_t getBrightness(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt)
{
    uint32_t v0 = 0;
    v0 = 0x1190000u;
    {
        v0 = v0 - 0x48BCu;
        return v0;
    }
}

// kSetTex (0x10916f8): typed by tools/typed_lift.py (round A1)
inline uint32_t kSetTex(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t a0)
{
    uint32_t v0 = 0;
    v0 = 0x1190000u;
    {
        A1_W32(v0 - 0x45B0u, a0);
        return v0;
    }
}

// getSpecularity (0x1092470): typed by tools/typed_lift.py (round A1)
inline uint32_t getSpecularity(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt)
{
    uint32_t v0 = 0;
    v0 = 0x1190000u;
    {
        v0 = v0 - 0x45A0u;
        return v0;
    }
}

// modelDeleteDispModelClass (0x1099590): typed by tools/typed_lift.py (round A1)
inline uint64_t modelDeleteDispModelClass(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint64_t a0)
{
    uint64_t v0 = 0;
    uint64_t v1 = 0;
    uint64_t a1 = 0;
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)a0, 0))));
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)65280 << 16))));
    (a0 = (uint64_t)(RC_U64X(a0) | (uint64_t)(uint16_t)511));
    (a1 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)v0, 32))));
    (v1 = (uint64_t)(A1_R64B((uint32_t)a1, 24)));
    (v0 = (uint64_t)(v1 << 23));
    (v0 = (uint64_t)((int64_t)v0 >> (32 + 0)));
    (v1 = (uint64_t)(v1 & RC_U64X(a0)));
    (v0 = (uint64_t)(v0 & (uint64_t)(uint16_t)32767));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v0, 4294967295))));
    (v0 = (uint64_t)(RC_U64X(v0) & (uint64_t)(uint16_t)32767));
    (v0 = (uint64_t)(RC_U64X(v0) << 9));
    (v1 = (uint64_t)(v1 | v0));
    {
        A1_W64B((uint32_t)a1, 24, v1);
        return v0;
    }
}

// modelGetWorkSize (0x1099658): typed by tools/typed_lift.py (round A1)
inline uint32_t modelGetWorkSize(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t a0)
{
    uint32_t v0 = 0;
    uint32_t v1 = 0;
    uint32_t a1 = 0;
    a1 = A1_R32B(a0, 100);
    v1 = A1_R32B(a0, 84);
    v0 = a1 << 1;
    v0 = v0 + a1;
    v1 = v1 << 6;
    v0 = v0 << 4;
    v0 = v0 + v1;
    {
        v0 = v0 + 0xA1u;
        return v0;
    }
}

// SetDisplayModelNearClippingSwitch (0x1099758): typed by tools/typed_lift.py (round A1)
inline uint32_t SetDisplayModelNearClippingSwitch(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t a0)
{
    uint32_t v0 = 0;
    v0 = 0x1190000u;
    {
        A1_W32(v0 - 0x402Cu, a0);
        return v0;
    }
}

// ptclmanGet (0x109a3d8): typed by tools/typed_lift.py (round A1)
inline uint32_t ptclmanGet(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt)
{
    uint32_t v0 = 0;
    uint32_t v1 = 0;
    uint32_t a0 = 0;
    v1 = 0x1210000u;
    v1 = v1 + 0x5BE8u;
    v0 = A1_R32B(v1, 4);
    a0 = A1_R32(v1);
    v0 = v0 << 4;
    {
        v0 = a0 + v0;
        return v0;
    }
}

// ptclmanPut (0x109a3f8): typed by tools/typed_lift.py (round A1)
inline uint32_t ptclmanPut(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t a0)
{
    uint32_t v0 = 0;
    uint32_t v1 = 0;
    v1 = 0x1210000u;
    v1 = v1 + 0x5BE8u;
    v0 = A1_R32B(v1, 4);
    v0 = v0 + a0;
    {
        A1_W32B(v1, 4, v0);
        return v0;
    }
}

// ptclCounterGet (0x109a4d0): typed by tools/typed_lift.py (round A1)
inline uint64_t ptclCounterGet(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt)
{
    uint64_t v0 = 0;
    {
        (v0 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL));
        return v0;
    }
}

// getRefBlurLevel (0x109be88): typed by tools/typed_lift.py (round A1)
inline uint32_t getRefBlurLevel(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt)
{
    uint32_t v0 = 0;
    v0 = 0x1190000u;
    {
        v0 = v0 - 0x4000u;
        return v0;
    }
}

// getIsInWater (0x109d0d8): typed by tools/typed_lift.py (round A1)
inline uint32_t getIsInWater(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt)
{
    uint32_t v0 = 0;
    v0 = 0x1190000u;
    {
        v0 = v0 - 0x3FCCu;
        return v0;
    }
}

// getRefractBlurSpeed (0x109d0e8): typed by tools/typed_lift.py (round A1)
inline uint32_t getRefractBlurSpeed(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt)
{
    uint32_t v0 = 0;
    v0 = 0x1190000u;
    {
        v0 = v0 - 0x3FC8u;
        return v0;
    }
}

// getRefractBlurSpeedRand (0x109d0f8): typed by tools/typed_lift.py (round A1)
inline uint32_t getRefractBlurSpeedRand(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt)
{
    uint32_t v0 = 0;
    v0 = 0x1190000u;
    {
        v0 = v0 - 0x3FC4u;
        return v0;
    }
}

// getRefractBlurLevel (0x109d108): typed by tools/typed_lift.py (round A1)
inline uint32_t getRefractBlurLevel(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt)
{
    uint32_t v0 = 0;
    v0 = 0x1190000u;
    {
        v0 = v0 - 0x3FC0u;
        return v0;
    }
}

// getRefractBlurLevelRand (0x109d118): typed by tools/typed_lift.py (round A1)
inline uint32_t getRefractBlurLevelRand(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt)
{
    uint32_t v0 = 0;
    v0 = 0x1190000u;
    {
        v0 = v0 - 0x3FBCu;
        return v0;
    }
}

// getRefractLengthRatio (0x109d128): typed by tools/typed_lift.py (round A1)
inline uint32_t getRefractLengthRatio(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt)
{
    uint32_t v0 = 0;
    v0 = 0x1190000u;
    {
        v0 = v0 - 0x3FB8u;
        return v0;
    }
}

// getRefractBlendRatio (0x109d138): typed by tools/typed_lift.py (round A1)
inline uint32_t getRefractBlendRatio(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt)
{
    uint32_t v0 = 0;
    v0 = 0x1190000u;
    {
        v0 = v0 - 0x3FB4u;
        return v0;
    }
}

// getRefractBrightness (0x109d148): typed by tools/typed_lift.py (round A1)
inline uint32_t getRefractBrightness(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt)
{
    uint32_t v0 = 0;
    v0 = 0x1190000u;
    {
        v0 = v0 - 0x3FB0u;
        return v0;
    }
}

// getShadowIntensity (0x109de78): typed by tools/typed_lift.py (round A1)
inline uint32_t getShadowIntensity(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt)
{
    uint32_t v0 = 0;
    v0 = 0x1190000u;
    {
        v0 = v0 - 0x3F7Cu;
        return v0;
    }
}

// GetShadowLength (0x109dea8): typed by tools/typed_lift.py (round A1)
inline float GetShadowLength(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t a0)
{
    float f0 = 0.0f;
    {
        f0 = a1_flt(A1_R32B(a0, 24));
        return f0;
    }
}

// shadowModelReInit (0x109df30): typed by tools/typed_lift.py (round A1)
inline uint64_t shadowModelReInit(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint64_t a0)
{
    uint64_t v0 = 0;
    uint64_t v1 = 0;
    uint64_t a1 = 0;
    uint64_t a2 = 0;
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)a0, 20))));
    (a2 = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)12288 << 16))));
    (v1 = (uint64_t)(uint32_t)(A1_R32B((uint32_t)a0, 16)));
    (v0 = (uint64_t)((int64_t)(int32_t)(SRA32((int32_t)v0, 4))));
    (a1 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)a0, 28))));
    (v0 = (uint64_t)(RC_U64X(v0) | RC_U64X(a2)));
    (v1 = (uint64_t)(v1 << (32 + 0)));
    (v0 = (uint64_t)(RC_U64X(v0) | v1));
    {
        A1_W64B((uint32_t)a1, 0, v0);
        return v0;
    }
}

// getSolarFlareSw (0x10a1728): typed by tools/typed_lift.py (round A1)
inline uint32_t getSolarFlareSw(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt)
{
    uint32_t v0 = 0;
    v0 = 0x1190000u;
    {
        v0 = v0 - 0x3F70u;
        return v0;
    }
}

// getSolarFlareBlur (0x10a1738): typed by tools/typed_lift.py (round A1)
inline uint32_t getSolarFlareBlur(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt)
{
    uint32_t v0 = 0;
    v0 = 0x1190000u;
    {
        v0 = v0 - 0x3F64u;
        return v0;
    }
}

// getSolarFlareColorR (0x10a1748): typed by tools/typed_lift.py (round A1)
inline uint32_t getSolarFlareColorR(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt)
{
    uint32_t v0 = 0;
    v0 = 0x1190000u;
    {
        v0 = v0 - 0x3F58u;
        return v0;
    }
}

// getSolarFlareColorG (0x10a1758): typed by tools/typed_lift.py (round A1)
inline uint32_t getSolarFlareColorG(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt)
{
    uint32_t v0 = 0;
    v0 = 0x1190000u;
    {
        v0 = v0 - 0x3F54u;
        return v0;
    }
}

// getSolarFlareColorB (0x10a1768): typed by tools/typed_lift.py (round A1)
inline uint32_t getSolarFlareColorB(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt)
{
    uint32_t v0 = 0;
    v0 = 0x1190000u;
    {
        v0 = v0 - 0x3F50u;
        return v0;
    }
}

// getSolarFlareSolarLevel (0x10a1778): typed by tools/typed_lift.py (round A1)
inline uint32_t getSolarFlareSolarLevel(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt)
{
    uint32_t v0 = 0;
    v0 = 0x1190000u;
    {
        v0 = v0 - 0x3F60u;
        return v0;
    }
}

// getSolarGlowLevel (0x10a1788): typed by tools/typed_lift.py (round A1)
inline uint32_t getSolarGlowLevel(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt)
{
    uint32_t v0 = 0;
    v0 = 0x1190000u;
    {
        v0 = v0 - 0x3F5Cu;
        return v0;
    }
}

// getSolarFlareSamplePix (0x10a1798): typed by tools/typed_lift.py (round A1)
inline uint32_t getSolarFlareSamplePix(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt)
{
    uint32_t v0 = 0;
    v0 = 0x1190000u;
    {
        v0 = v0 - 0x3F68u;
        return v0;
    }
}

// manager_sub_010A2040 (0x10a2040): typed by tools/typed_lift.py (round A1)
inline a1::RetVF manager_sub_010A2040(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t a0)
{
    uint32_t at = 0;
    uint32_t v0 = 0;
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
    uint32_t fcr31 = 0;
    bool df_at = false;
    f0 = a1_flt(0u);
    f13 = a1_flt(A1_R32(a0));
    fcr31 = (a1f::lt(f13, f0)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
    {
        const bool c = (fcr31 & 0x800000u) != 0u;
        v0 = 1u;
        if (!c) {
            f12 = a1_flt(A1_R32B(a0, 4));
            fcr31 = (a1f::lt(f12, f0)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
            if ((fcr31 & 0x800000u) == 0u) {
                f11 = a1_flt(A1_R32B(a0, 8));
                fcr31 = (a1f::lt(f11, f0)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
                if ((fcr31 & 0x800000u) == 0u) {
                    f3 = a1_flt(A1_R32B(a0, 12));
                    fcr31 = (a1f::lt(f3, f0)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
                    if ((fcr31 & 0x800000u) == 0u) {
                        if ((fcr31 & 0x800000u) == 0u) {
                            f10 = a1_flt(A1_R32B(a0, 16));
                            fcr31 = (a1f::lt(f10, f0)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
                            if ((fcr31 & 0x800000u) == 0u) {
                                f9 = a1_flt(A1_R32B(a0, 20));
                                fcr31 = (a1f::lt(f9, f0)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
                                if ((fcr31 & 0x800000u) == 0u) {
                                    f2 = a1_flt(A1_R32B(a0, 24));
                                    fcr31 = (a1f::lt(f2, f0)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
                                    if ((fcr31 & 0x800000u) == 0u) {
                                        if ((fcr31 & 0x800000u) == 0u) {
                                            f8 = a1_flt(A1_R32B(a0, 28));
                                            fcr31 = (a1f::lt(f8, f0)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
                                            if ((fcr31 & 0x800000u) == 0u) {
                                                f7 = a1_flt(A1_R32B(a0, 32));
                                                fcr31 = (a1f::lt(f7, f0)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
                                                if ((fcr31 & 0x800000u) == 0u) {
                                                    f1 = a1_flt(A1_R32B(a0, 36));
                                                    fcr31 = (a1f::lt(f1, f0)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
                                                    if ((fcr31 & 0x800000u) == 0u) {
                                                        if ((fcr31 & 0x800000u) == 0u) {
                                                            f6 = a1_flt(A1_R32B(a0, 40));
                                                            fcr31 = (a1f::lt(f6, f0)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
                                                            if ((fcr31 & 0x800000u) == 0u) {
                                                                f5 = a1_flt(A1_R32B(a0, 44));
                                                                fcr31 = (a1f::lt(f5, f0)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
                                                                if ((fcr31 & 0x800000u) == 0u) {
                                                                    f4 = a1_flt(A1_R32B(a0, 48));
                                                                    fcr31 = (a1f::lt(f4, f0)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
                                                                    if ((fcr31 & 0x800000u) == 0u) {
                                                                        at = 0x45800000u; df_at = true;
                                                                        f0 = a1_flt(at);
                                                                        fcr31 = (a1f::le(f0, f13)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
                                                                        if ((fcr31 & 0x800000u) == 0u) {
                                                                            fcr31 = (a1f::le(f0, f12)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
                                                                            if ((fcr31 & 0x800000u) == 0u) {
                                                                                fcr31 = (a1f::le(f0, f11)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
                                                                                if ((fcr31 & 0x800000u) == 0u) {
                                                                                    fcr31 = (a1f::le(f0, f3)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
                                                                                    if ((fcr31 & 0x800000u) == 0u) {
                                                                                        if ((fcr31 & 0x800000u) == 0u) {
                                                                                            fcr31 = (a1f::le(f0, f10)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
                                                                                            if ((fcr31 & 0x800000u) == 0u) {
                                                                                                fcr31 = (a1f::le(f0, f9)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
                                                                                                if ((fcr31 & 0x800000u) == 0u) {
                                                                                                    fcr31 = (a1f::le(f0, f2)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
                                                                                                    if ((fcr31 & 0x800000u) == 0u) {
                                                                                                        if ((fcr31 & 0x800000u) == 0u) {
                                                                                                            fcr31 = (a1f::le(f0, f8)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
                                                                                                            if ((fcr31 & 0x800000u) == 0u) {
                                                                                                                fcr31 = (a1f::le(f0, f7)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
                                                                                                                if ((fcr31 & 0x800000u) == 0u) {
                                                                                                                    fcr31 = (a1f::le(f0, f1)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
                                                                                                                    if ((fcr31 & 0x800000u) == 0u) {
                                                                                                                        if ((fcr31 & 0x800000u) == 0u) {
                                                                                                                            fcr31 = (a1f::le(f0, f6)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
                                                                                                                            if ((fcr31 & 0x800000u) == 0u) {
                                                                                                                                fcr31 = (a1f::le(f0, f5)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
                                                                                                                                if ((fcr31 & 0x800000u) == 0u) {
                                                                                                                                    fcr31 = (a1f::le(f0, f4)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
                                                                                                                                    {
                                                                                                                                        const bool c = (fcr31 & 0x800000u) == 0u;
                                                                                                                                        v0 = 0u;
                                                                                                                                        if (!c) {
                                                                                                                                            v0 = 1u;
                                                                                                                                        }
                                                                                                                                    }
                                                                                                                                }
                                                                                                                            }
                                                                                                                        }
                                                                                                                    }
                                                                                                                }
                                                                                                            }
                                                                                                        }
                                                                                                    }
                                                                                                }
                                                                                            }
                                                                                        }
                                                                                    }
                                                                                }
                                                                            }
                                                                        }
                                                                    }
                                                                }
                                                            }
                                                        }
                                                    }
                                                }
                                            }
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }
    {
        return a1::RetVF{(uint64_t)a1_sx64(v0), f0};
    }
}

// spriteSetPosition (0x10a52d0): typed by tools/typed_lift.py (round A1)
inline void spriteSetPosition(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t a0, float f12, float f13)
{
    A1_W32B(a0, 52, a1_bits(f13));
    {
        A1_W32B(a0, 48, a1_bits(f12));
        
        return;
    }
}

// spriteSetTextureCoordinateSTFromUV (0x10a5310): typed by tools/typed_lift.py (round A1)
inline a1::RetVF spriteSetTextureCoordinateSTFromUV(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint64_t a0, float f12, float f13, float f14, float f15)
{
    uint64_t at = 0;
    uint64_t v0 = 0;
    uint64_t v1 = 0;
    uint64_t a1 = 0;
    uint64_t a2 = 0;
    uint64_t a3 = 0;
    float f0 = 0.0f;
    float f1 = 0.0f;
    float f2 = 0.0f;
    float f3 = 0.0f;
    uint32_t fcr31 = 0;
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)a0, 0))));
    (a2 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 1))));
    (at = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)16256 << 16))));
    { uint32_t bits = (uint32_t)at; std::memcpy(&f1, &bits, sizeof(bits)); }
    (a3 = (uint64_t)(0ULL | (uint64_t)(uint16_t)32768));
    (a3 = (uint64_t)(RC_U64X(a3) << (32 + 1)));
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)v0, 32))));
    (at = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)16128 << 16))));
    { uint32_t bits = (uint32_t)at; std::memcpy(&f3, &bits, sizeof(bits)); }
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)v1, 28))));
    f13 = a1f::add(f13, f3);
    (a1 = (uint64_t)(A1_R64B((uint32_t)a0, 72)));
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)SRL32((uint32_t)v0, 20))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)SRL32((uint32_t)v0, 16))));
    (v0 = (uint64_t)(RC_U64X(v0) & (uint64_t)(uint16_t)15));
    (v1 = (uint64_t)(RC_U64X(v1) & (uint64_t)(uint16_t)15));
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)SLL32((uint32_t)a2, (uint32_t)v1 & 0x1F))));
    (a2 = (uint64_t)((int64_t)(int32_t)((int32_t)SLL32((uint32_t)a2, (uint32_t)v0 & 0x1F))));
    { uint32_t bits = (uint32_t)a2; std::memcpy(&f0, &bits, sizeof(bits)); }
    { int32_t tmp; std::memcpy(&tmp, &f0, sizeof(tmp)); f0 = ((float)(int32_t)(tmp)); }
    (a1 = (uint64_t)(a1 | a3));
    { uint32_t bits = (uint32_t)v1; std::memcpy(&f2, &bits, sizeof(bits)); }
    { int32_t tmp; std::memcpy(&tmp, &f2, sizeof(tmp)); f2 = ((float)(int32_t)(tmp)); }
    A1_W64B((uint32_t)a0, 72, a1);
    f12 = a1f::add(f12, f3);
    f15 = a1f::add(f13, f15);
    f0 = a1f::div(fcr31, f1, f0);
    f14 = a1f::add(f12, f14);
    f1 = a1f::div(fcr31, f1, f2);
    f14 = ((float)(f14) * (float)(f0));
    f12 = ((float)(f12) * (float)(f0));
    { float f = f14; uint32_t bits; std::memcpy(&bits, &f, sizeof(bits)); A1_W32B((uint32_t)a0, 40, bits); }
    { float f = f12; uint32_t bits; std::memcpy(&bits, &f, sizeof(bits)); A1_W32B((uint32_t)a0, 32, bits); }
    f15 = ((float)(f15) * (float)(f1));
    f13 = ((float)(f13) * (float)(f1));
    { float f = f15; uint32_t bits; std::memcpy(&bits, &f, sizeof(bits)); A1_W32B((uint32_t)a0, 44, bits); }
    {
        { float f = f13; uint32_t bits; std::memcpy(&bits, &f, sizeof(bits)); A1_W32B((uint32_t)a0, 36, bits); }
        return a1::RetVF{(uint64_t)v0, f0};
    }
}

// spriteSetTextureCoordinateUV (0x10a53b8): typed by tools/typed_lift.py (round A1)
inline a1::RetVF spriteSetTextureCoordinateUV(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint64_t a0, float f12, float f13, float f14, float f15)
{
    uint64_t v0 = 0;
    uint64_t v1 = 0;
    float f0 = 0.0f;
    float f1 = 0.0f;
    float f2 = 0.0f;
    f0 = a1f::add(f12, f14);
    (v0 = (uint64_t)(A1_R64B((uint32_t)a0, 72)));
    f1 = a1f::add(f13, f15);
    { float f = f15; uint32_t bits; std::memcpy(&bits, &f, sizeof(bits)); A1_W32B((uint32_t)a0, 60, bits); }
    { int32_t tmp = a1f::cvtw(f12); std::memcpy(&f2, &tmp, sizeof(tmp)); }
    { float f = f2; uint32_t bits; std::memcpy(&bits, &f, sizeof(bits)); A1_W32B((uint32_t)a0, 16, bits); }
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)65534 << 16))));
    (v1 = (uint64_t)(RC_U64X(v1) | (uint64_t)(uint16_t)65535));
    (v1 = (uint64_t)(RC_U64X(v1) << 16));
    (v1 = (uint64_t)(v1 | (uint64_t)(uint16_t)65535));
    (v1 = (uint64_t)(v1 << 16));
    (v1 = (uint64_t)(v1 | (uint64_t)(uint16_t)65535));
    (v0 = (uint64_t)(v0 & v1));
    { int32_t tmp = a1f::cvtw(f13); std::memcpy(&f2, &tmp, sizeof(tmp)); }
    { float f = f2; uint32_t bits; std::memcpy(&bits, &f, sizeof(bits)); A1_W32B((uint32_t)a0, 20, bits); }
    { int32_t tmp = a1f::cvtw(f0); std::memcpy(&f2, &tmp, sizeof(tmp)); }
    { float f = f2; uint32_t bits; std::memcpy(&bits, &f, sizeof(bits)); A1_W32B((uint32_t)a0, 24, bits); }
    A1_W64B((uint32_t)a0, 72, v0);
    { int32_t tmp = a1f::cvtw(f1); std::memcpy(&f0, &tmp, sizeof(tmp)); }
    { float f = f0; uint32_t bits; std::memcpy(&bits, &f, sizeof(bits)); A1_W32B((uint32_t)a0, 28, bits); }
    {
        { float f = f14; uint32_t bits; std::memcpy(&bits, &f, sizeof(bits)); A1_W32B((uint32_t)a0, 56, bits); }
        return a1::RetVF{(uint64_t)v0, f0};
    }
}

// spriteSetColor (0x10a5410): typed by tools/typed_lift.py (round A1)
inline void spriteSetColor(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t a0, uint32_t a1, uint32_t a2, uint32_t a3, uint32_t t0)
{
    A1_W8B(a0, 7, (uint8_t)t0);
    A1_W8B(a0, 4, (uint8_t)a1);
    A1_W8B(a0, 5, (uint8_t)a2);
    {
        A1_W8B(a0, 6, (uint8_t)a3);
        
        return;
    }
}

// spriteSetScale (0x10a5428): typed by tools/typed_lift.py (round A1)
inline void spriteSetScale(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t a0, float f12, float f13)
{
    A1_W32B(a0, 68, a1_bits(f13));
    {
        A1_W32B(a0, 64, a1_bits(f12));
        
        return;
    }
}

// spriteSetRotation (0x10a5438): typed by tools/typed_lift.py (round A1)
inline void spriteSetRotation(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t a0, float f12)
{
    {
        A1_W32B(a0, 72, a1_bits(f12));
        
        return;
    }
}

// spriteSetFade (0x10a54d0): typed by tools/typed_lift.py (round A1)
inline void spriteSetFade(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t a0, float f12)
{
    {
        A1_W32B(a0, 8, a1_bits(f12));
        
        return;
    }
}

// spriteSetPriority (0x10a54d8): typed by tools/typed_lift.py (round A1)
inline void spriteSetPriority(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t a0, uint32_t a1)
{
    {
        A1_W8B(a0, 76, (uint8_t)a1);
        
        return;
    }
}

// manager_sub_010ABC48 (0x10abc48): typed by tools/typed_lift.py (round A1)
inline uint64_t manager_sub_010ABC48(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint64_t a0)
{
    uint32_t v1 = 0;
    uint32_t a1 = 0;
    uint32_t a2 = 0;
    uint32_t a3 = 0;
    uint32_t t0 = 0;
    uint32_t t1 = 0;
    uint32_t t2 = 0;
    uint32_t t4 = 0;
    uint64_t v0 = 0;
    uint64_t t3 = 0;
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)281 << 16))));
    (t3 = (uint64_t)((uint64_t)a0 + (uint64_t)0ULL));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v0, 4294952024))));
    (v1 = (uint32_t)((int32_t)((int32_t)A1_R32B((uint32_t)t3, 68))));
    (t0 = (uint32_t)((int32_t)((int32_t)A1_R32B((uint32_t)v0, 12))));
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v0, 12))));
    v1 = v1 & 0x7FFFu;
    (a1 = (uint32_t)((int32_t)((int32_t)A1_R32B((uint32_t)t3, 72))));
    (v0 = (uint64_t)((uint64_t)a1_sx64(t0) + (uint64_t)0ULL));
    t1 = t0 + 8u;
    t4 = v1 << 16;
    {
        const bool c = (int64_t)v0 != (int64_t)(int32_t)t0;
        (t2 = (uint32_t)((int32_t)((int32_t)ADD32((uint32_t)t3, 84))));
        if (!c) {
            (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B(t0, 4))));
            A1_W32B((uint32_t)a0, 0, (uint32_t)v0);
        }
    }
    v1 = A1_R32(t0);
    if (v1 == 0) {
        v1 = A1_R32B(t0, 4);
    } else {
        (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B(t0, 4))));
        A1_W32B(v1, 4, (uint32_t)v0);
        v1 = A1_R32B(t0, 4);
    }
    if (v1 == 0) {
        (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)a0, 4))));
    } else {
        (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B(t0, 0))));
        A1_W32B(v1, 0, (uint32_t)v0);
        (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)a0, 4))));
    }
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v0, 4294967295))));
    A1_W32B((uint32_t)a0, 4, (uint32_t)v0);
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B(t2, 0))));
    if ((int64_t)v0 != 0) {
        A1_W32B((uint32_t)v0, 0, t0);
    } else {
    }
    A1_W32(t0, 0u);
    a3 = 0xFFFFFFFEu;
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)32768 << 16))));
    a2 = 0xFFFF0000u;
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B(t2, 0))));
    (a0 = (uint64_t)(RC_U64X(a0) | (uint64_t)(uint16_t)65535));
    a1 = a1 & 0x7FFFu;
    a2 = a2 | 1u;
    A1_W32B(t0, 4, (uint32_t)v0);
    a1 = a1 << 1;
    A1_W32(t2, t0);
    v1 = 1u;
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B(t2, 4))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v0, 1))));
    A1_W32B(t2, 4, (uint32_t)v0);
    A1_W32(t1, v1);
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B(t1, 4))));
    A1_W32B(t1, 20, 0u);
    (v0 = (uint64_t)(RC_U64X(v0) & a1_sx64(a3)));
    A1_W32B(t1, 12, t0);
    (v0 = (uint64_t)(RC_U64X(v0) & RC_U64X(a0)));
    A1_W32B(t1, 8, (uint32_t)t3);
    (v0 = (uint64_t)(RC_U64X(v0) | a1_sx64(t4)));
    A1_W32B(t1, 16, 0u);
    (v0 = (uint64_t)(RC_U64X(v0) & a1_sx64(a2)));
    (v0 = (uint64_t)(RC_U64X(v0) | a1_sx64(a1)));
    A1_W32B(t1, 4, (uint32_t)v0);
    {
        A1_W32B((uint32_t)t3, 80, t0);
        return v0;
    }
}

// getZFogDivide (0x10ac400): typed by tools/typed_lift.py (round A1)
inline uint32_t getZFogDivide(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt)
{
    uint32_t v0 = 0;
    v0 = 0x1190000u;
    {
        v0 = v0 - 0x3B94u;
        return v0;
    }
}

// getZFogScissorMargin (0x10ac440): typed by tools/typed_lift.py (round A1)
inline uint32_t getZFogScissorMargin(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt)
{
    uint32_t v0 = 0;
    v0 = 0x1190000u;
    {
        v0 = v0 - 0x3B88u;
        return v0;
    }
}

// setZFogFar (0x10ac460): typed by tools/typed_lift.py (round A1)
inline uint32_t setZFogFar(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, float f12)
{
    uint32_t v0 = 0;
    v0 = 0x1190000u;
    {
        A1_W32(v0 - 0x3B80u, a1_bits(f12));
        return v0;
    }
}

// getZFogNear (0x10ac478): typed by tools/typed_lift.py (round A1)
inline a1::RetVF getZFogNear(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt)
{
    uint32_t v0 = 0;
    uint32_t v1 = 0;
    uint32_t a0 = 0;
    float f0 = 0.0f;
    float f1 = 0.0f;
    float f2 = 0.0f;
    v0 = 0x1190000u;
    a0 = 0x1190000u;
    f0 = a1_flt(A1_R32(v0 - 0x3B80u));
    v1 = 0x1190000u;
    f1 = a1_flt(A1_R32(a0 - 0x3B8Cu));
    f2 = a1_flt(A1_R32(v1 - 0x3B90u));
    f0 = a1f::sub(f0, f1);
    {
        f0 = a1f::sub(f0, f2);
        return a1::RetVF{(uint64_t)a1_sx64(v0), f0};
    }
}

// GetObjectAnimationParentNodeId (0x10b6788): typed by tools/typed_lift.py (round A1)
inline uint32_t GetObjectAnimationParentNodeId(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t a0, uint32_t a1)
{
    uint32_t v0 = 0;
    uint32_t v1 = 0;
    v1 = A1_R32B(a0, 8);
    a1 = a1 << 2;
    v0 = 0xFFFFFFFFu;
    {
        const bool c = v1 == 0;
        a1 = a1 + v1;
        if (!c) {
            v0 = A1_R32(a1);
        }
    }
    {
        return v0;
    }
}

// IsFullFixFormat (0x10b74c0): typed by tools/typed_lift.py (round A1)
inline uint64_t IsFullFixFormat(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint64_t a0)
{
    uint32_t v1 = 0;
    uint64_t v0 = 0;
    v1 = 0xFF0000u;
    v1 = v1 | 0xFFFFu;
    (v0 = (uint64_t)(a0 & a1_sx64(v1)));
    {
        (v0 = (uint64_t)(((uint64_t)v0 < (uint64_t)(int64_t)(int32_t)1) ? 1 : 0));
        return v0;
    }
}

// GetMotionResourceRotAngle (0x10b7650): typed by tools/typed_lift.py (round A1)
inline uint64_t GetMotionResourceRotAngle(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint64_t a0)
{
    uint64_t v0 = 0;
    uint64_t v1 = 0;
    uint64_t a1 = 0;
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 1))));
    (v1 = (uint64_t)(((int64_t)(int64_t)a0 < (int64_t)(int32_t)2) ? 1 : 0));
    {
        const bool c = (int64_t)a0 == (int64_t)v0;
        (a1 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL));
        if (!c) {
            if ((int64_t)v1 != 0) {
                goto L_10b7684;
            }
            (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 2))));
            if ((int64_t)a0 == (int64_t)v0) {
                (a1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 4294950912))));
                goto L_10b7684;
            }
            (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 3))));
            if ((int64_t)a0 != (int64_t)v0) {
                goto L_10b7684;
            }
        }
    }
    (a1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 16384))));
L_10b7684:
    {
        (v0 = (uint64_t)((uint64_t)RC_U64X(a1) + (uint64_t)0ULL));
        return v0;
    }
}

// constructor_motion_ik_base (0x10b92a8): typed by tools/typed_lift.py (round A1)
inline void constructor_motion_ik_base(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t a0)
{
    {
        A1_W32(a0, 0u);
        
        return;
    }
}

// init_motion_ik_base (0x10b92b0): typed by tools/typed_lift.py (round A1)
inline void init_motion_ik_base(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t a0)
{
    {
        A1_W32(a0, 0u);
        
        return;
    }
}

// doit_motion_ik_base (0x10b92b8): typed by tools/typed_lift.py (round A1)
inline void doit_motion_ik_base(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t a0, uint32_t a1)
{
    {
        A1_W32(a0, a1);
        
        return;
    }
}

// manager_sub_010BA140 (0x10ba140): typed by tools/typed_lift.py (round A1)
inline void manager_sub_010BA140(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t a0)
{
    A1_W32B(a0, 128, 0u);
    A1_W32B(a0, 136, 0u);
    {
        A1_W32B(a0, 132, 0u);
        
        return;
    }
}

// UpdateAnimationClassPastSimpleInfo (0x10ba9d8): typed by tools/typed_lift.py (round A1)
inline float UpdateAnimationClassPastSimpleInfo(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t a0)
{
    float f0 = 0.0f;
    f0 = a1_flt(A1_R32B(a0, 8));
    {
        A1_W32B(a0, 12, a1_bits(f0));
        return f0;
    }
}

// GetCharacterClassInitParamNbOptionNodes (0x10c0c78): typed by tools/typed_lift.py (round A1)
inline uint32_t GetCharacterClassInitParamNbOptionNodes(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t a0)
{
    uint32_t v0 = 0;
    uint32_t v1 = 0;
    v1 = A1_R32B(a0, 132);
    {
        const bool c = v1 == 0;
        v0 = 0xFFFFFFFFu;
        if (!c) {
            v0 = A1_R32B(v1, 32);
            v0 = A1_R32B(v0, 32);
        }
    }
    {
        return v0;
    }
}

// ResetCharacterClassWaterStatus (0x10c11b8): typed by tools/typed_lift.py (round A1)
inline void ResetCharacterClassWaterStatus(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t a0)
{
    A1_W32B(a0, 4300, 0u);
    A1_W8B(a0, 865, (uint8_t)0u);
    A1_W8B(a0, 866, (uint8_t)0u);
    A1_W32B(a0, 4296, 0u);
    {
        A1_W32B(a0, 4292, 0u);
        
        return;
    }
}

// ResetCharacterClassMotInterpInfo (0x10c17b0): typed by tools/typed_lift.py (round A1)
inline a1::RetVF ResetCharacterClassMotInterpInfo(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t a0)
{
    uint32_t at = 0;
    uint32_t v0 = 0;
    float f0 = 0.0f;
    float f1 = 0.0f;
    at = 0x3F800000u;
    f1 = a1_flt(at);
    A1_W32B(a0, 1824, 0u);
    A1_W32B(a0, 1828, a1_bits(f1));
    A1_W32B(a0, 1832, 0u);
    A1_W32B(a0, 1836, 0u);
    A1_W32B(a0, 1852, a1_bits(f1));
    v0 = A1_R32B(a0, 4416);
    f0 = a1_flt(A1_R32B(v0, 312));
    A1_W32B(a0, 1860, a1_bits(f1));
    {
        A1_W32B(a0, 1856, a1_bits(f0));
        return a1::RetVF{(uint64_t)a1_sx64(v0), f0};
    }
}

// GetCharacterClassCurAnimationFrameRate (0x10c1a08): typed by tools/typed_lift.py (round A1)
inline uint64_t GetCharacterClassCurAnimationFrameRate(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t a0)
{
    uint32_t v1 = 0;
    uint64_t v0 = 0;
    v1 = A1_R32B(a0, 448);
    {
        const bool c = v1 == 0;
        (v0 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL));
        if (!c) {
            (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B(v1, 92))));
        }
    }
    {
        return v0;
    }
}

// SetCharacterClassGazeAdditionChannelInfo (0x10c66f0): typed by tools/typed_lift.py (round A1)
inline uint32_t SetCharacterClassGazeAdditionChannelInfo(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t a0, uint32_t a1, float f12, float f13)
{
    uint32_t v0 = 0;
    uint32_t v1 = 0;
    v0 = A1_R32(a1);
    v1 = A1_R32B(a0, 820);
    v0 = v0 >> 14;
    v0 = v0 & 0xFFF0u;
    v0 = v0 + v1;
    A1_W32(v0, a1_bits(f12));
    {
        A1_W32B(v0, 4, a1_bits(f13));
        return v0;
    }
}

// ForceAcceptMotionControlChange (0x10cc6c8): typed by tools/typed_lift.py (round A1)
inline uint32_t ForceAcceptMotionControlChange(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t a0, uint64_t a1)
{
    uint32_t v0 = 0;
    v0 = A1_R32B(a0, 1872);
    if ((int64_t)(int32_t)v0 != (int64_t)a1) {
        A1_W32B(a0, 1872, (uint32_t)a1);
        A1_W32B(a0, 1876, v0);
    }
    {
        return v0;
    }
}

// YattsukeSetPlayerR1ON (0x10cc778): typed by tools/typed_lift.py (round A1)
inline uint32_t YattsukeSetPlayerR1ON(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t a0)
{
    uint32_t v0 = 0;
    v0 = 0x1190000u;
    {
        A1_W32(v0 - 0x3610u, a0);
        return v0;
    }
}

// RequestCharacterClassSlip (0x10d2a88): typed by tools/typed_lift.py (round A1)
inline uint32_t RequestCharacterClassSlip(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t a0)
{
    uint32_t v0 = 0;
    v0 = (uint32_t)(int8_t)A1_R8B(a0, 3222);
    {
        const bool c = v0 != 0;
        v0 = 1u;
        if (!c) {
            A1_W8B(a0, 3222, (uint8_t)v0);
        }
    }
    {
        return v0;
    }
}

// RequestCharacterClassNoClimbableSlip (0x10d2aa0): typed by tools/typed_lift.py (round A1)
inline uint32_t RequestCharacterClassNoClimbableSlip(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t a0)
{
    uint32_t v0 = 0;
    v0 = 2u;
    {
        A1_W8B(a0, 3222, (uint8_t)v0);
        return v0;
    }
}

// RequestCharacterClassAdditionMotionChange (0x10d2ae0): typed by tools/typed_lift.py (round A1)
inline uint32_t RequestCharacterClassAdditionMotionChange(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t a0, uint32_t a1)
{
    uint32_t v0 = 0;
    uint32_t v1 = 0;
    uint32_t a2 = 0;
    v0 = (uint32_t)(int8_t)A1_R8B(a0, 3220);
    v1 = A1_R32B(a0, 1020);
    a2 = v0 << 2;
    v0 = (int32_t)v0 < (int32_t)v1 ? 1u : 0u;
    if (v0 != 0) {
        v0 = A1_R32B(a0, 776);
        v0 = a2 + v0;
        A1_W32(v0, a1);
        v1 = A1_R8B(a0, 3220);
        v1 = v1 + 1u;
        A1_W8B(a0, 3220, (uint8_t)v1);
    }
    {
        return v0;
    }
}

// RequestCharacterClassMotionDirectionByGPTop (0x10d2b60): typed by tools/typed_lift.py (round A1)
inline uint32_t RequestCharacterClassMotionDirectionByGPTop(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t a0)
{
    uint32_t v0 = 0;
    v0 = 3u;
    {
        A1_W32B(a0, 3156, v0);
        return v0;
    }
}

// RequestCharacterClassMotionDirectionByGPTail (0x10d2b70): typed by tools/typed_lift.py (round A1)
inline uint32_t RequestCharacterClassMotionDirectionByGPTail(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t a0)
{
    uint32_t v0 = 0;
    v0 = 4u;
    {
        A1_W32B(a0, 3156, v0);
        return v0;
    }
}

// RequestCharacterClassMotionTurnLevel (0x10d2b90): typed by tools/typed_lift.py (round A1)
inline uint32_t RequestCharacterClassMotionTurnLevel(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t a0, float f12)
{
    uint32_t v0 = 0;
    v0 = 2u;
    A1_W32B(a0, 3168, a1_bits(f12));
    {
        A1_W32B(a0, 3156, v0);
        return v0;
    }
}

// RequestCharacterClassMotionPlaySpeedUp (0x10d2bb0): typed by tools/typed_lift.py (round A1)
inline uint32_t RequestCharacterClassMotionPlaySpeedUp(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t a0)
{
    uint32_t v0 = 0;
    v0 = 1u;
    {
        A1_W32B(a0, 3216, v0);
        return v0;
    }
}

// RequestCharacterClassMotionPlaySpeedDown (0x10d2bc0): typed by tools/typed_lift.py (round A1)
inline uint32_t RequestCharacterClassMotionPlaySpeedDown(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t a0)
{
    uint32_t v0 = 0;
    v0 = 0xFFFFFFFFu;
    {
        A1_W32B(a0, 3216, v0);
        return v0;
    }
}

// RequestCharacterClassMotionPlaySpeedDown2 (0x10d2bd0): typed by tools/typed_lift.py (round A1)
inline uint32_t RequestCharacterClassMotionPlaySpeedDown2(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t a0)
{
    uint32_t v0 = 0;
    v0 = 0xFFFFFFFEu;
    {
        A1_W32B(a0, 3216, v0);
        return v0;
    }
}

// RequestCharacterClassWindForceMove (0x10d2be0): typed by tools/typed_lift.py (round A1)
inline uint32_t RequestCharacterClassWindForceMove(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t a0)
{
    uint32_t v0 = 0;
    v0 = 1u;
    {
        A1_W8B(a0, 3221, (uint8_t)v0);
        return v0;
    }
}

// getTurnLimitInterp (0x10d2ce8): typed by tools/typed_lift.py (round A1)
inline float getTurnLimitInterp(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, float f12, float f13, float f14, float f15)
{
    uint32_t at = 0;
    float f0 = 0.0f;
    float f1 = 0.0f;
    uint32_t fcr31 = 0;
    at = 0x3A830000u;
    at = at | 0x126Eu;
    f0 = a1_flt(at);
    fcr31 = (a1f::le(f14, f0)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
    {
        const bool c = (fcr31 & 0x800000u) != 0u;
        f0 = ((float)(f13));
        if (!c) {
            f1 = a1f::div(fcr31, f15, f14);
            at = 0x3F800000u;
            f0 = a1_flt(at);
            f14 = ((float)(f1));
            fcr31 = (a1f::le(f14, f0)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
            if ((fcr31 & 0x800000u) == 0u) {
                f14 = ((float)(f0));
            } else {
                at = 0x3F800000u;
                f0 = a1_flt(at);
            }
            f1 = ((float)(f14));
            f0 = a1f::subFinite(f0, f1);
            f1 = a1f::mul(f13, f1);
            f0 = a1f::mul(f12, f0);
            f0 = a1f::addFinite(f1, f0);
        }
    }
    {
        return f0;
    }
}

// getRatioFromInterp (0x10d2d48): typed by tools/typed_lift.py (round A1)
inline float getRatioFromInterp(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, float f12, float f13)
{
    uint32_t at = 0;
    float f0 = 0.0f;
    float f1 = 0.0f;
    uint32_t fcr31 = 0;
    at = 0x3A830000u;
    at = at | 0x126Eu;
    f1 = a1_flt(at);
    at = 0x3F800000u;
    f0 = a1_flt(at);
    fcr31 = (a1f::le(f12, f1)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
    if ((fcr31 & 0x800000u) == 0u) {
        f0 = a1f::div(fcr31, f13, f12);
        at = 0x3F800000u;
        f1 = a1_flt(at);
        fcr31 = (a1f::le(f0, f1)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
        if ((fcr31 & 0x800000u) == 0u) {
            f0 = ((float)(f1));
        } else {
        }
    }
    {
        return f0;
    }
}

// SetCharacterObjMotionJumpSpeedLimit (0x10d37a8): typed by tools/typed_lift.py (round A1)
inline uint32_t SetCharacterObjMotionJumpSpeedLimit(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t a1)
{
    uint32_t v0 = 0;
    uint32_t v1 = 0;
    uint32_t a0 = 0;
    v0 = 0x10D0000u;
    a0 = 0x1190000u;
    v0 = v0 + 0x8A0u;
    v1 = 0x1190000u;
    A1_W32(a0 - 0x3600u, v0);
    {
        A1_W32(v1 - 0x35FCu, a1);
        return v0;
    }
}

// charrootdyn_getUpperEdgeEffectiveHeight (0x10dc3f0): typed by tools/typed_lift.py (round A1)
inline void charrootdyn_getUpperEdgeEffectiveHeight(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t a0)
{
    {
        A1_W32B(a0, 152, 0u);
        
        return;
    }
}

// charrootdyn_getNoLimitMode (0x10de368): typed by tools/typed_lift.py (round A1)
inline uint32_t charrootdyn_getNoLimitMode(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t a0, uint32_t a1)
{
    uint32_t v0 = 0;
    A1_W32(a0, 0u);
    A1_W32B(a0, 4, 0u);
    v0 = A1_R32B(a1, 3120);
    {
        A1_W32B(a0, 8, v0);
        return v0;
    }
}

// charrootdyn_getForceNoLimitMode (0x10de380): typed by tools/typed_lift.py (round A1)
inline uint32_t charrootdyn_getForceNoLimitMode(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t a0)
{
    uint32_t v0 = 0;
    v0 = 3u;
    A1_W32B(a0, 8, 0u);
    A1_W32(a0, v0);
    {
        A1_W32B(a0, 4, 0u);
        return v0;
    }
}

// charrootdyn_isParentToEdge (0x10df130): typed by tools/typed_lift.py (round A1)
inline uint64_t charrootdyn_isParentToEdge(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t a0)
{
    uint64_t v0 = 0;
    uint64_t v1 = 0;
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B(a0, 292))));
    {
        const bool c = (int64_t)v0 == 0;
        (v1 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL));
        if (!c) {
            (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B(a0, 240))));
            (v0 = (uint64_t)(RC_U64X(v0) ^ (uint64_t)(uint16_t)4));
            (v1 = (uint64_t)(((uint64_t)RC_U64X(v0) < (uint64_t)(int64_t)(int32_t)1) ? 1 : 0));
        }
    }
    {
        (v0 = (uint64_t)((uint64_t)RC_U64X(v1) + (uint64_t)0ULL));
        return v0;
    }
}

// manager_sub_010DFD18 (0x10dfd18): typed by tools/typed_lift.py (round A1)
inline uint32_t manager_sub_010DFD18(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t a2, uint64_t a1)
{
    uint32_t v0 = 0;
    v0 = (uint32_t)(int8_t)A1_R8B(a2, 4);
    {
        const bool c = v0 == 0;
        v0 = 1u;
        if (!c) {
            {
                A1_W8B(a2, 3, (uint8_t)v0);
                return v0;
            }
        }
    }
    v0 = 1u;
    if ((int64_t)a1 == (int64_t)(int32_t)v0) {
        A1_W8B(a2, 2, (uint8_t)(uint32_t)a1);
    } else {
    }
    {
        return v0;
    }
}

// SetInitFogColClassFromStageData (0x10ed358): typed by tools/typed_lift.py (round A1)
inline a1::RetVF SetInitFogColClassFromStageData(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t a0)
{
    uint32_t v0 = 0;
    uint32_t v1 = 0;
    uint32_t a1 = 0;
    float f0 = 0.0f;
    float f1 = 0.0f;
    f1 = a1_flt(A1_R32B(a0, 4));
    a1 = 0x1210000u;
    a1 = a1 + 0x5E80u;
    A1_W32B(a1, 88, a1_bits(f1));
    f0 = a1_flt(A1_R32B(a0, 8));
    A1_W32B(a1, 92, a1_bits(f0));
    f1 = a1_flt(A1_R32B(a0, 12));
    A1_W32B(a1, 100, a1_bits(f1));
    f0 = a1_flt(A1_R32B(a0, 16));
    A1_W32B(a1, 104, a1_bits(f0));
    f1 = a1_flt(A1_R32B(a0, 24));
    { int32_t tmp; std::memcpy(&tmp, &f1, sizeof(tmp)); f1 = ((float)(int32_t)(tmp)); }
    A1_W32B(a1, 96, a1_bits(f1));
    v0 = A1_R8B(a0, 28);
    A1_W8B(a1, 78, (uint8_t)v0);
    v1 = A1_R8B(a0, 32);
    A1_W8B(a1, 79, (uint8_t)v1);
    v0 = A1_R8B(a0, 36);
    {
        A1_W8B(a1, 80, (uint8_t)v0);
        return a1::RetVF{(uint64_t)a1_sx64(v0), f0};
    }
}

// SetFogColAvailableFlag (0x10ed520): typed by tools/typed_lift.py (round A1)
inline void SetFogColAvailableFlag(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t a0, uint32_t a1)
{
    {
        A1_W32B(a0, 128, a1);
        
        return;
    }
}

// IsEnvelopeFogType (0x10ed528): typed by tools/typed_lift.py (round A1)
inline uint32_t IsEnvelopeFogType(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t a0)
{
    uint32_t v0 = 0;
    v0 = A1_R32B(a0, 64);
    v0 = v0 & 0x800u;
    {
        v0 = 0u < v0 ? 1u : 0u;
        return v0;
    }
}

// GetJudgeVolumeClassWorkSize (0x10ee768): typed by tools/typed_lift.py (round A1)
inline uint32_t GetJudgeVolumeClassWorkSize(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint64_t a0)
{
    uint32_t v0 = 0;
    uint32_t v1 = 0;
    v0 = 0x30000u;
    (v1 = (uint32_t)((int32_t)((int32_t)A1_R32B((uint32_t)a0, 0))));
    v0 = v0 | 0xFFFFu;
    v0 = v1 & v0;
    {
        const bool c = v0 == 0;
        (v0 = (uint32_t)((int32_t)((int32_t)SUB32((uint32_t)a0, v0))));
        if (!c) {
            (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B(v0, 0))));
            goto L_10ee78c;
        }
    }
    (a0 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL));
L_10ee78c:
    {
        const bool c = (int64_t)a0 == 0;
        { const uint32_t _dynInsn = A1_R32(0x10EE790u); const int32_t _dynSimm = (int16_t)(_dynInsn & 0xFFFFu); const uint32_t _dynImm = _dynInsn & 0xFFFFu; (void)_dynSimm; (void)_dynImm; (v0 = (uint32_t)((int32_t)((int32_t)((uint32_t)_dynImm << 16)))); }
        if (!c) {
            { const uint32_t _dynInsn = A1_R32(0x10EE794u); const int32_t _dynSimm = (int16_t)(_dynInsn & 0xFFFFu); const uint32_t _dynImm = _dynInsn & 0xFFFFu; (void)_dynSimm; (void)_dynImm; (v1 = (uint32_t)((int32_t)((int32_t)A1_R32(ADD32(v0, _dynSimm))))); }
            if ((int64_t)a0 == (int64_t)(int32_t)v1) {
                v0 = 0u;
                goto L_10ee7ac;
            }
            (v0 = (uint32_t)((int32_t)((int32_t)A1_R32B((uint32_t)a0, 12))));
            goto L_10ee7ac;
        }
    }
    v0 = 0u;
L_10ee7ac:
    if (v0 != 0) {
        v0 = v0 << 2;
        {
            v0 = v0 + 0x33u;
            return v0;
        }
    }
    {
        v0 = 0u;
        return v0;
    }
}

// GetJudgeVolumeClassObjectGroup (0x10ee808): typed by tools/typed_lift.py (round A1)
inline uint32_t GetJudgeVolumeClassObjectGroup(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t a0, uint32_t a1)
{
    uint32_t v0 = 0;
    uint32_t v1 = 0;
    v0 = A1_R32(a1);
    v1 = A1_R32B(a0, 4);
    v0 = v0 >> 18;
    {
        const bool c = v1 == 0;
        v0 = v0 & 0xFFFu;
        if (!c) {
            v0 = v0 << 2;
            {
                v0 = v1 + v0;
                return v0;
            }
        }
    }
    {
        v0 = 0u;
        return v0;
    }
}

// GetJudgeVolumeClassObjectGroupById (0x10ee830): typed by tools/typed_lift.py (round A1)
inline uint64_t GetJudgeVolumeClassObjectGroupById(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t a0, uint64_t a1)
{
    uint32_t v1 = 0;
    uint64_t v0 = 0;
    v1 = A1_R32B(a0, 4);
    (v0 = (uint64_t)((uint64_t)a1 + (uint64_t)0ULL));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)SLL32((uint32_t)v0, 2))));
    {
        const bool c = v1 != 0;
        (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(v1, (uint32_t)v0))));
        if (!c) {
            (v0 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL));
        }
    }
    {
        return v0;
    }
}

// SetLayoutClassCollisionWorkAdr (0x10f31b8): typed by tools/typed_lift.py (round A1)
inline void SetLayoutClassCollisionWorkAdr(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t a0, uint32_t a1)
{
    {
        A1_W32B(a0, 372, a1);
        
        return;
    }
}

// manager_sub_010F3470 (0x10f3470): typed by tools/typed_lift.py (round A1)
inline uint32_t manager_sub_010F3470(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t a0)
{
    uint32_t v0 = 0;
    uint32_t v1 = 0;
    bool df_v1 = false;
    v0 = A1_R32B(a0, 64);
    {
        const bool c = v0 != 0;
        v0 = 1u;
        if (!c) {
            v0 = A1_R32B(a0, 68);
            {
                const bool c = v0 != 0;
                v0 = 1u;
                if (!c) {
                    v1 = A1_R32B(a0, 72); df_v1 = true;
                    {
                        const bool c = v1 == 0;
                        v0 = 0u;
                        if (!c) {
                            v0 = 1u;
                        }
                    }
                }
            }
        }
    }
    {
        return v0;
    }
}

// manager_sub_010F3668 (0x10f3668): typed by tools/typed_lift.py (round A1)
inline void manager_sub_010F3668(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t a0)
{
    {
        A1_W32(a0, 0u);
        
        return;
    }
}

// SetCameraResetSw (0x10f5810): typed by tools/typed_lift.py (round A1)
inline uint32_t SetCameraResetSw(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t a0)
{
    uint32_t v0 = 0;
    v0 = 0x1190000u;
    {
        A1_W32(v0 - 0x3508u, a0);
        return v0;
    }
}

// GetResetCameraFlagForDisplay (0x10f5af8): typed by tools/typed_lift.py (round A1)
inline uint32_t GetResetCameraFlagForDisplay(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt)
{
    uint32_t v0 = 0;
    uint32_t v1 = 0;
    v1 = 0x1190000u;
    {
        v0 = A1_R32(v1 - 0x3508u);
        return v0;
    }
}

// GetLightColMan (0x10f6358): typed by tools/typed_lift.py (round A1)
inline uint32_t GetLightColMan(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt)
{
    uint32_t v0 = 0;
    uint32_t v1 = 0;
    v1 = 0x1190000u;
    {
        v0 = A1_R32(v1 - 0x34F0u);
        return v0;
    }
}

// TaskExecLightColMan (0x10f6408): typed by tools/typed_lift.py (round A1)
inline uint32_t TaskExecLightColMan(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt)
{
    uint32_t v0 = 0;
    uint32_t v1 = 0;
    v0 = 0x1210000u;
    v1 = 0x1190000u;
    v0 = v0 + 0x6130u;
    {
        A1_W32(v1 - 0x34F0u, v0);
        return v0;
    }
}

// SetGameLightAvailable (0x10f7508): typed by tools/typed_lift.py (round A1)
inline void SetGameLightAvailable(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t a0, uint32_t a1)
{
    {
        A1_W8B(a0, 57, (uint8_t)a1);
        
        return;
    }
}

// ForceGameLightFlagEnvelope (0x10f7658): typed by tools/typed_lift.py (round A1)
inline uint32_t ForceGameLightFlagEnvelope(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t a0)
{
    uint32_t v0 = 0;
    uint32_t v1 = 0;
    uint32_t a1 = 0;
    v1 = A1_R32B(a0, 44);
    a1 = 0x20000u;
    v0 = 0xFFFE0000u;
    v0 = v0 | 0xFFFFu;
    v1 = v1 | a1;
    v1 = v1 & v0;
    {
        A1_W32B(a0, 44, v1);
        return v0;
    }
}

// IsEnvelopeGameLightType (0x10f7678): typed by tools/typed_lift.py (round A1)
inline uint32_t IsEnvelopeGameLightType(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t a0)
{
    uint32_t v0 = 0;
    uint32_t v1 = 0;
    v0 = A1_R32B(a0, 44);
    v1 = 0x20000u;
    v0 = v0 & v1;
    {
        v0 = 0u < v0 ? 1u : 0u;
        return v0;
    }
}

// SetGameLightUserParam (0x10f78a8): typed by tools/typed_lift.py (round A1)
inline void SetGameLightUserParam(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t a0, uint32_t a1)
{
    {
        A1_W32B(a0, 80, a1);
        
        return;
    }
}

// SetGameLightUserFunc (0x10f78b0): typed by tools/typed_lift.py (round A1)
inline void SetGameLightUserFunc(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t a0, uint32_t a1)
{
    {
        A1_W32B(a0, 84, a1);
        
        return;
    }
}

// SetGlobalLightAvailable (0x10f8b40): typed by tools/typed_lift.py (round A1)
inline void SetGlobalLightAvailable(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t a1, uint64_t a0)
{
    if ((int64_t)a0 != 0) {
        A1_W32B((uint32_t)a0, 324, a1);
    } else {
    }
    {
        
        return;
    }
}

// ForceGlobalLightFlagEnvelope (0x10f8c90): typed by tools/typed_lift.py (round A1)
inline uint32_t ForceGlobalLightFlagEnvelope(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t a0)
{
    uint32_t v0 = 0;
    uint32_t v1 = 0;
    uint32_t a1 = 0;
    v1 = A1_R32B(a0, 272);
    a1 = 0x20000u;
    v0 = 0xFFFE0000u;
    v0 = v0 | 0xFFFFu;
    v1 = v1 | a1;
    v1 = v1 & v0;
    {
        A1_W32B(a0, 272, v1);
        return v0;
    }
}

// IsEnvelopeGlobalLightType (0x10f8cb0): typed by tools/typed_lift.py (round A1)
inline uint32_t IsEnvelopeGlobalLightType(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t a0)
{
    uint32_t v0 = 0;
    uint32_t v1 = 0;
    v0 = A1_R32B(a0, 272);
    v1 = 0x20000u;
    v0 = v0 & v1;
    {
        v0 = 0u < v0 ? 1u : 0u;
        return v0;
    }
}

// SetLwoDemoOrientManagerObjCutFinishFlag (0x10f9b58): typed by tools/typed_lift.py (round A1)
inline void SetLwoDemoOrientManagerObjCutFinishFlag(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t a0, uint32_t a1)
{
    {
        A1_W32B(a0, 176, a1);
        
        return;
    }
}

// GetLayoutObjGroup (0x11008a0): typed by tools/typed_lift.py (round A1)
inline uint32_t GetLayoutObjGroup(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt)
{
    uint32_t v0 = 0;
    uint32_t v1 = 0;
    v1 = 0x1190000u;
    v0 = A1_R32(v1 - 0x34E0u);
    {
        v0 = v0 + 0xCu;
        return v0;
    }
}

// GetLayoutDispObjGroup (0x11008b0): typed by tools/typed_lift.py (round A1)
inline uint32_t GetLayoutDispObjGroup(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt)
{
    uint32_t v0 = 0;
    uint32_t v1 = 0;
    v1 = 0x1190000u;
    v0 = A1_R32(v1 - 0x34E0u);
    {
        v0 = v0 + 0x10u;
        return v0;
    }
}

// GetSkeletonFocusNodeDirect (0x1100c48): typed by tools/typed_lift.py (round A1)
inline uint32_t GetSkeletonFocusNodeDirect(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint64_t a0)
{
    uint32_t v0 = 0;
    {
        const bool c = (int64_t)a0 == 0;
        v0 = 0xFFFFFFFFu;
        if (!c) {
            (v0 = (uint32_t)((int32_t)((int32_t)A1_R32B((uint32_t)a0, 8))));
        }
    }
    {
        return v0;
    }
}

// AddObjectManagerSheetWorkSize (0x11016c0): typed by tools/typed_lift.py (round A1)
inline uint32_t AddObjectManagerSheetWorkSize(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t a0)
{
    uint32_t v0 = 0;
    uint32_t v1 = 0;
    v1 = 0x1210000u;
    v1 = v1 + 0x6D00u;
    v0 = A1_R32B(v1, 36);
    v0 = v0 + a0;
    {
        A1_W32B(v1, 36, v0);
        return v0;
    }
}

// GetAmbientLightObjGroup (0x1101718): typed by tools/typed_lift.py (round A1)
inline uint32_t GetAmbientLightObjGroup(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt)
{
    uint32_t v0 = 0;
    uint32_t v1 = 0;
    v1 = 0x1190000u;
    v0 = A1_R32(v1 - 0x34E0u);
    {
        v0 = v0 + 0x14u;
        return v0;
    }
}

// GetScriptPriorityGroup (0x1102b00): typed by tools/typed_lift.py (round A1)
inline uint32_t GetScriptPriorityGroup(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t a0)
{
    uint32_t at = 0;
    uint32_t v0 = 0;
    v0 = a0 << 2;
    at = 0x1190000u;
    at = at - 0x3390u;
    v0 = at + v0;
    {
        return v0;
    }
}

// manager_sub_01103080 (0x1103080): typed by tools/typed_lift.py (round A1)
inline uint64_t manager_sub_01103080(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint64_t a0)
{
    uint32_t v1 = 0;
    uint64_t v0 = 0;
    bool df_v1 = false;
    {
        const bool c = (int64_t)a0 == 0;
        (v0 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL));
        if (!c) {
            v1 = 0xA5A50000u; df_v1 = true;
            (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)a0, 0))));
            v1 = v1 | 0xA5A5u; df_v1 = true;
            {
                const bool c = (int64_t)a0 == (int64_t)(int32_t)v1;
                (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 1))));
                if (!c) {
                    (v0 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL));
                }
            }
        }
    }
    {
        return v0;
    }
}

// GetScriptAnimationGlobalBaseInfo (0x110a6d8): typed by tools/typed_lift.py (round A1)
inline uint32_t GetScriptAnimationGlobalBaseInfo(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt)
{
    uint32_t v0 = 0;
    v0 = 0x1190000u;
    {
        v0 = v0 - 0x3340u;
        return v0;
    }
}

// CopyScriptAnimationWindParam (0x110a790): typed by tools/typed_lift.py (round A1)
inline uint64_t CopyScriptAnimationWindParam(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t a0, uint32_t a1)
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
    {
        A1_W64B(a0, 56, v1);
        return v0;
    }
}

// IsNonISysObjTypeRes (0x110b818): typed by tools/typed_lift.py (round A1)
inline uint64_t IsNonISysObjTypeRes(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint64_t a0)
{
    uint64_t v0 = 0;
    uint64_t v1 = 0;
    (v0 = (uint64_t)(((int64_t)(int64_t)a0 < (int64_t)(int32_t)15) ? 1 : 0));
    {
        const bool c = (int64_t)v0 != 0;
        (v1 = (uint64_t)(((int64_t)(int64_t)a0 < (int64_t)(int32_t)29) ? 1 : 0));
        if (!c) {
            if ((int64_t)v1 == 0) {
                (v0 = (uint64_t)(((int64_t)(int64_t)a0 < (int64_t)(int32_t)36) ? 1 : 0));
                if ((int64_t)v0 == 0) {
                    goto L_110b850;
                }
                (v0 = (uint64_t)(((int64_t)(int64_t)a0 < (int64_t)(int32_t)32) ? 1 : 0));
                if ((int64_t)v0 != 0) {
                    goto L_110b850;
                }
            }
            {
                (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 1))));
                return v0;
            }
        }
    }
L_110b850:
    {
        (v0 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL));
        return v0;
    }
}

// SetScriptCharDefaultScriptFunc (0x110cc68): typed by tools/typed_lift.py (round A1)
inline uint32_t SetScriptCharDefaultScriptFunc(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t a0)
{
    uint32_t v0 = 0;
    v0 = 0x1190000u;
    {
        A1_W32(v0 - 0x3280u, a0);
        return v0;
    }
}

// InitSortObj (0x110de28): typed by tools/typed_lift.py (round A1)
inline void InitSortObj(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t a0, uint32_t a1)
{
    A1_W32(a0, a1);
    A1_W32B(a0, 8, 0u);
    {
        A1_W32B(a0, 12, 0u);
        
        return;
    }
}

// setDefaultMenuControlButton (0x1113ae8): typed by tools/typed_lift.py (round A1)
inline uint32_t setDefaultMenuControlButton(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt)
{
    uint32_t v0 = 0;
    uint32_t v1 = 0;
    uint32_t a0 = 0;
    { const uint32_t _dynInsn = A1_R32(0x1113AE8u); const int32_t _dynSimm = (int16_t)(_dynInsn & 0xFFFFu); const uint32_t _dynImm = _dynInsn & 0xFFFFu; (void)_dynSimm; (void)_dynImm; (v0 = (uint32_t)((int32_t)((int32_t)((uint32_t)_dynImm << 16)))); }
    a0 = 0x1190000u;
    { const uint32_t _dynInsn = A1_R32(0x1113AF0u); const int32_t _dynSimm = (int16_t)(_dynInsn & 0xFFFFu); const uint32_t _dynImm = _dynInsn & 0xFFFFu; (void)_dynSimm; (void)_dynImm; (v1 = (uint32_t)((int32_t)((int32_t)A1_R32(ADD32(v0, _dynSimm))))); }
    {
        A1_W32(a0 - 0x3278u, v1);
        return v0;
    }
}

// InitViewIntervalClip (0x1114af0): typed by tools/typed_lift.py (round A1)
inline float InitViewIntervalClip(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t a0)
{
    uint32_t at = 0;
    float f0 = 0.0f;
    at = 0xBD080000u;
    at = at | 0x8888u;
    f0 = a1_flt(at);
    A1_W32B(a0, 4460, 0u);
    A1_W32B(a0, 4456, a1_bits(f0));
    {
        A1_W32B(a0, 4452, 0u);
        return f0;
    }
}

// CopyCollisionEdge (0x11280e0): typed by tools/typed_lift.py (round A1)
inline uint32_t CopyCollisionEdge(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t a0, uint32_t a1)
{
    uint32_t v0 = 0;
    uint32_t v1 = 0;
    v0 = A1_R32(a1);
    A1_W32(a0, v0);
    v1 = A1_R32B(a1, 4);
    A1_W32B(a0, 4, v1);
    v0 = A1_R32B(a1, 8);
    A1_W32B(a0, 8, v0);
    v1 = A1_R32B(a1, 12);
    {
        A1_W32B(a0, 12, v1);
        return v0;
    }
}

// MakeCollisionEdgePointer (0x1128270): typed by tools/typed_lift.py (round A1)
inline uint32_t MakeCollisionEdgePointer(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t a0, uint32_t a1, uint32_t a2)
{
    uint32_t v0 = 0;
    uint32_t v1 = 0;
    v0 = A1_R32(a1);
    A1_W32(a0, v0);
    v1 = A1_R32B(a1, 4);
    A1_W32B(a0, 8, a2);
    {
        A1_W32B(a0, 4, v1);
        return v0;
    }
}

// IsClusterCollision (0x11282f0): typed by tools/typed_lift.py (round A1)
inline uint32_t IsClusterCollision(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t a0)
{
    uint32_t v0 = 0;
    v0 = A1_R32B(a0, 4);
    v0 = (int32_t)v0 < 2 ? 1u : 0u;
    {
        v0 = v0 ^ 1u;
        return v0;
    }
}

// MakeGeometryPointerCollisionEdgeInfo (0x11283d0): typed by tools/typed_lift.py (round A1)
inline uint32_t MakeGeometryPointerCollisionEdgeInfo(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t a0, uint32_t a1, uint32_t a2, float f12)
{
    uint32_t v0 = 0;
    uint32_t v1 = 0;
    uint32_t a3 = 0;
    v1 = A1_R32(a1);
    a3 = 4u;
    A1_W32(a0, v1);
    v0 = A1_R32B(a1, 4);
    A1_W32B(a0, 4, v0);
    v1 = A1_R32B(a1, 8);
    A1_W32B(a0, 48, a3);
    A1_W32B(a0, 8, v1);
    A1_W32B(a0, 12, a1_bits(f12));
    {
        A1_W32B(a0, 16, a2);
        return v0;
    }
}

// GetCollisionResource (0x11287d0): typed by tools/typed_lift.py (round A1)
inline uint32_t GetCollisionResource(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t a0)
{
    uint32_t v0 = 0;
    uint32_t v1 = 0;
    v1 = A1_R32B(a0, 8);
    {
        v0 = A1_R32B(v1, 32);
        return v0;
    }
}

// CollisionObjectJudgeCallbackExceptOneObj (0x1128818): typed by tools/typed_lift.py (round A1)
inline uint64_t CollisionObjectJudgeCallbackExceptOneObj(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint64_t a0, uint64_t a1)
{
    uint64_t v0 = 0;
    (v0 = (uint64_t)(a0 ^ a1));
    {
        (v0 = (uint64_t)(((uint64_t)0ULL < (uint64_t)v0) ? 1 : 0));
        return v0;
    }
}

// manager_sub_011293E0 (0x11293e0): typed by tools/typed_lift.py (round A1)
inline a1::RetVF manager_sub_011293E0(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t a3, float f12)
{
    uint32_t v0 = 0;
    float f0 = 0.0f;
    uint32_t fcr31 = 0;
    f0 = a1_flt(A1_R32(a3));
    fcr31 = (a1f::le(f0, f12)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
    {
        const bool c = (fcr31 & 0x800000u) != 0u;
        v0 = 1u;
        if (!c) {
            v0 = 0u;
        }
    }
    {
        return a1::RetVF{(uint64_t)a1_sx64(v0), f0};
    }
}

// manager_sub_01129AE8 (0x1129ae8): typed by tools/typed_lift.py (round A1)
inline void manager_sub_01129AE8(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t a0)
{
    {
        A1_W32(a0, 0u);
        
        return;
    }
}

// FlushCollisionEdgeLinkDBVWallFinder (0x1129b00): typed by tools/typed_lift.py (round A1)
inline void FlushCollisionEdgeLinkDBVWallFinder(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t a0)
{
    {
        A1_W32B(a0, 32, 0u);
        
        return;
    }
}

// GetCollisionEdgeLinkDBWorkSize (0x1129b08): typed by tools/typed_lift.py (round A1)
inline uint32_t GetCollisionEdgeLinkDBWorkSize(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint64_t a0)
{
    uint32_t v0 = 0;
    {
        const bool c = (int64_t)a0 == 0;
        (v0 = (uint32_t)((int32_t)((int32_t)SLL32((uint32_t)a0, 2))));
        if (!c) {
            (v0 = (uint32_t)((int32_t)((int32_t)ADD32(v0, (uint32_t)a0))));
            v0 = v0 << 2;
            {
                v0 = v0 + 0x96u;
                return v0;
            }
        }
    }
    {
        v0 = 0u;
        return v0;
    }
}

// GetCollisionFinderWorkSize (0x112a330): typed by tools/typed_lift.py (round A1)
inline uint32_t GetCollisionFinderWorkSize(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t a0)
{
    uint32_t v0 = 0;
    uint32_t v1 = 0;
    v1 = a0 << 1;
    v0 = a0 << 3;
    v1 = v1 + a0;
    v1 = v1 << 2;
    v0 = v0 + v1;
    {
        v0 = v0 + 0x66u;
        return v0;
    }
}

// InitGeometryPointer (0x11328a8): typed by tools/typed_lift.py (round A1)
inline uint64_t InitGeometryPointer(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t a0)
{
    uint32_t a1 = 0;
    uint64_t v0 = 0;
    uint64_t v1 = 0;
    a1 = 0x1190000u;
    a1 = a1 - 0x3070u;
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
    {
        A1_W64B(a0, 56, v1);
        return v0;
    }
}

// SetGeometryPointerChainPos (0x1132cb0): typed by tools/typed_lift.py (round A1)
inline uint32_t SetGeometryPointerChainPos(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t a0, uint32_t a1, uint32_t a2, float f12)
{
    uint32_t v0 = 0;
    v0 = 9u;
    A1_W32B(a0, 8, a1_bits(f12));
    A1_W32B(a0, 48, v0);
    A1_W32(a0, a1);
    {
        A1_W32B(a0, 4, a2);
        return v0;
    }
}

// CheckLegalCollisionGP (0x1132d58): typed by tools/typed_lift.py (round A1)
inline uint32_t CheckLegalCollisionGP(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t a0)
{
    uint32_t v0 = 0;
    uint64_t v1 = 0;
    v0 = A1_R32B(a0, 48);
    (v1 = (uint64_t)(((int64_t)(int64_t)a1_sx64(v0) < (int64_t)(int32_t)2) ? 1 : 0));
    v0 = (int32_t)v0 < 6 ? 1u : 0u;
    {
        const bool c = v0 == 0;
        v0 = 0u;
        if (!c) {
            {
                const bool c = (int64_t)v1 == 0;
                v0 = 1u;
                if (!c) {
                    v0 = 0u;
                }
            }
        }
    }
    {
        return v0;
    }
}

// GetYHeightOfPlaneXZProjection (0x1134308): typed by tools/typed_lift.py (round A1)
inline float GetYHeightOfPlaneXZProjection(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t a0, uint32_t a1)
{
    float f0 = 0.0f;
    float f1 = 0.0f;
    float f2 = 0.0f;
    float f3 = 0.0f;
    float f4 = 0.0f;
    uint32_t fcr31 = 0;
    f2 = a1_flt(A1_R32(a1));
    f4 = a1_flt(A1_R32B(a1, 8));
    f0 = a1_flt(A1_R32(a0));
    f1 = a1_flt(A1_R32B(a0, 8));
    f0 = a1f::mul(f0, f2);
    f2 = a1_flt(A1_R32B(a0, 4));
    f1 = a1f::mul(f1, f4);
    f3 = a1_flt(A1_R32B(a0, 12));
    f0 = a1f::addFinite(f0, f1);
    f0 = a1f::add(f0, f3);
    f0 = (-(float)(f0));
    f0 = a1f::div(fcr31, f0, f2);
    {
        return f0;
    }
}

// CheckClothChainExist (0x11398f0): typed by tools/typed_lift.py (round A1)
inline uint32_t CheckClothChainExist(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t a0)
{
    uint32_t v0 = 0;
    {
        v0 = (uint32_t)(int8_t)A1_R8B(a0, 6);
        return v0;
    }
}

// SetClothSingleFixedChainLeafFixNodeOffset (0x11399d8): typed by tools/typed_lift.py (round A1)
inline void SetClothSingleFixedChainLeafFixNodeOffset(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t a0, uint32_t a1)
{
    {
        A1_W8B(a0, 113, (uint8_t)a1);
        
        return;
    }
}

// GetTwoWayLinkNext (0x1147ba8): typed by tools/typed_lift.py (round A1)
inline uint32_t GetTwoWayLinkNext(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t a0)
{
    uint32_t v0 = 0;
    {
        v0 = A1_R32B(a0, 4);
        return v0;
    }
}

// RelocateTwoWayLinkHeader (0x1147bb0): typed by tools/typed_lift.py (round A1)
inline uint32_t RelocateTwoWayLinkHeader(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t a0)
{
    uint32_t v0 = 0;
    v0 = A1_R32B(a0, 4);
    if (v0 != 0) {
        A1_W32(v0, a0);
    } else {
    }
    {
        return v0;
    }
}

// GetTwoWayLinkData (0x1147bc8): typed by tools/typed_lift.py (round A1)
inline uint32_t GetTwoWayLinkData(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t a0)
{
    uint32_t v0 = 0;
    {
        v0 = A1_R32B(a0, 8);
        return v0;
    }
}

// mcReqSetWaitFunc (0x1149460): typed by tools/typed_lift.py (round A1)
inline uint32_t mcReqSetWaitFunc(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t a0)
{
    uint32_t v0 = 0;
    v0 = 0x1220000u;
    {
        A1_W32(v0 - 0x500Cu, a0);
        return v0;
    }
}

// mcGetProcessPer (0x1149cf8): typed by tools/typed_lift.py (round A1)
inline a1::RetVF mcGetProcessPer(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt)
{
    uint32_t v0 = 0;
    float f0 = 0.0f;
    v0 = 0x1190000u;
    {
        f0 = a1_flt(A1_R32(v0 - 0x29E8u));
        return a1::RetVF{(uint64_t)a1_sx64(v0), f0};
    }
}

// mcGetGameClearFlag (0x114a300): typed by tools/typed_lift.py (round A1)
inline uint32_t mcGetGameClearFlag(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt)
{
    uint32_t v0 = 0;
    uint32_t v1 = 0;
    v1 = 0x1190000u;
    {
        v0 = A1_R32(v1 - 0x29E0u);
        return v0;
    }
}

// mcGetCompleteTimeAttack (0x114a320): typed by tools/typed_lift.py (round A1)
inline uint32_t mcGetCompleteTimeAttack(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t a0, uint32_t a1)
{
    uint32_t v0 = 0;
    uint32_t v1 = 0;
    uint32_t a2 = 0;
    v1 = 0x1190000u;
    a2 = 0x1190000u;
    v0 = A1_R32(v1 - 0x29DCu);
    A1_W32(a0, v0);
    v1 = A1_R32(a2 - 0x29D8u);
    {
        A1_W32(a1, v1);
        return v0;
    }
}

// mouseSetCursorPosition (0x114ad38): typed by tools/typed_lift.py (round A1)
inline uint32_t mouseSetCursorPosition(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, float f12, float f13)
{
    uint32_t v0 = 0;
    v0 = 0x1220000u;
    v0 = v0 - 0x5008u;
    A1_W32B(v0, 4, a1_bits(f13));
    {
        A1_W32(v0, a1_bits(f12));
        return v0;
    }
}

// mouseMoveCursorPosition (0x114ae78): typed by tools/typed_lift.py (round A1)
inline a1::RetVF mouseMoveCursorPosition(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, float f12, float f13)
{
    uint32_t v0 = 0;
    float f0 = 0.0f;
    float f1 = 0.0f;
    v0 = 0x1220000u;
    v0 = v0 - 0x5008u;
    f0 = a1_flt(A1_R32(v0));
    f1 = a1_flt(A1_R32B(v0, 4));
    f0 = a1f::add(f0, f12);
    f1 = a1f::add(f1, f13);
    A1_W32(v0, a1_bits(f0));
    {
        A1_W32B(v0, 4, a1_bits(f1));
        return a1::RetVF{(uint64_t)a1_sx64(v0), f0};
    }
}

// setVibrationEnableWorkAdrs (0x114b580): typed by tools/typed_lift.py (round A1)
inline uint32_t setVibrationEnableWorkAdrs(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t a0)
{
    uint32_t v0 = 0;
    v0 = 0x1190000u;
    {
        A1_W32(v0 - 0xA50u, a0);
        return v0;
    }
}

// usbdevMouseGetStatus (0x114b6e0): typed by tools/typed_lift.py (round A1)
inline uint32_t usbdevMouseGetStatus(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt)
{
    uint32_t v0 = 0;
    v0 = 0x1220000u;
    {
        v0 = v0 - 0x4FACu;
        return v0;
    }
}

// windowSemiTransRate (0x1155b50): typed by tools/typed_lift.py (round A1)
inline uint32_t windowSemiTransRate(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt)
{
    uint32_t v0 = 0;
    v0 = 0x1190000u;
    {
        v0 = v0 - 0x530u;
        return v0;
    }
}

// windowGetCurrentWindowHandle (0x11560f8): typed by tools/typed_lift.py (round A1)
inline uint32_t windowGetCurrentWindowHandle(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt)
{
    uint32_t v0 = 0;
    uint32_t v1 = 0;
    v1 = 0x1190000u;
    {
        v0 = A1_R32(v1 - 0x548u);
        return v0;
    }
}

// windowSetTex (0x1156338): typed by tools/typed_lift.py (round A1)
inline uint32_t windowSetTex(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t a0)
{
    uint32_t v0 = 0;
    v0 = 0x1190000u;
    {
        A1_W32(v0 - 0x570u, a0);
        return v0;
    }
}

// adpcmOpenChkComplete (0x1157870): typed by tools/typed_lift.py (round A1)
inline uint64_t adpcmOpenChkComplete(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint64_t a0)
{
    uint64_t v0 = 0;
    uint64_t hi = 0;
    uint64_t lo = 0;
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 1376))));
    { int64_t result = (int64_t)(int32_t)a0 * (int64_t)(int32_t)v0; lo = (uint64_t)(int64_t)(int32_t)result; hi = (uint64_t)(int64_t)(int32_t)(result >> 32); (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)result))); }
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)290 << 16))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v0, (uint32_t)a0))));
    (v0 = (uint64_t)(A1_R64B((uint32_t)v0, -17136)));
    (v0 = (uint64_t)(v0 >> (32 + 4)));
    {
        (v0 = (uint64_t)(v0 & (uint64_t)(uint16_t)1));
        return v0;
    }
}

// adpcmCloseReq (0x1157940): typed by tools/typed_lift.py (round A1)
inline uint64_t adpcmCloseReq(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint64_t a0, uint64_t a1)
{
    uint64_t at = 0;
    uint64_t v0 = 0;
    uint64_t v1 = 0;
    uint64_t hi = 0;
    uint64_t lo = 0;
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 1376))));
    { int64_t result = (int64_t)(int32_t)a0 * (int64_t)(int32_t)v0; lo = (uint64_t)(int64_t)(int32_t)result; hi = (uint64_t)(int64_t)(int32_t)(result >> 32); (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)result))); }
    (at = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)290 << 16))));
    (at = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)at, 4294950104))));
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)at, (uint32_t)a0))));
    {
        const bool c = (int64_t)a1 == 0;
        A1_W32B((uint32_t)a0, 68, (uint32_t)a1);
        if (!c) {
            A1_W32B((uint32_t)a1, 0, 0u);
        }
    }
    (v0 = (uint64_t)(A1_R64B((uint32_t)a0, 56)));
    (v1 = (uint64_t)(0ULL | (uint64_t)(uint16_t)32768));
    (v1 = (uint64_t)(RC_U64X(v1) << 22));
    (v0 = (uint64_t)(v0 | v1));
    {
        A1_W64B((uint32_t)a0, 56, v0);
        return v0;
    }
}

// adpcmCloseFadeReq (0x1157978): typed by tools/typed_lift.py (round A1)
inline uint64_t adpcmCloseFadeReq(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint64_t a0, uint64_t a1, float f12)
{
    uint64_t at = 0;
    uint64_t v0 = 0;
    uint64_t v1 = 0;
    uint64_t hi = 0;
    uint64_t lo = 0;
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 1376))));
    (v1 = (uint64_t)(0ULL | (uint64_t)(uint16_t)32768));
    (v1 = (uint64_t)(RC_U64X(v1) << 24));
    { int64_t result = (int64_t)(int32_t)a0 * (int64_t)(int32_t)v0; lo = (uint64_t)(int64_t)(int32_t)result; hi = (uint64_t)(int64_t)(int32_t)(result >> 32); (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)result))); }
    (at = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)290 << 16))));
    (at = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)at, 4294950104))));
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)at, (uint32_t)a0))));
    (v0 = (uint64_t)(A1_R64B((uint32_t)a0, 56)));
    { float f = f12; uint32_t bits; std::memcpy(&bits, &f, sizeof(bits)); A1_W32B((uint32_t)a0, 80, bits); }
    (v0 = (uint64_t)(v0 | v1));
    A1_W32B((uint32_t)a0, 88, 0u);
    A1_W64B((uint32_t)a0, 56, v0);
    {
        const bool c = (int64_t)a1 == 0;
        A1_W32B((uint32_t)a0, 68, (uint32_t)a1);
        if (!c) {
            A1_W32B((uint32_t)a1, 0, 0u);
        }
    }
    (v0 = (uint64_t)(A1_R64B((uint32_t)a0, 56)));
    (v1 = (uint64_t)(0ULL | (uint64_t)(uint16_t)32768));
    (v1 = (uint64_t)(RC_U64X(v1) << 22));
    (v0 = (uint64_t)(v0 | v1));
    {
        A1_W64B((uint32_t)a0, 56, v0);
        return v0;
    }
}

// adpcmCloseFadeSetVol (0x11579d0): typed by tools/typed_lift.py (round A1)
inline a1::RetVF adpcmCloseFadeSetVol(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t a0, float f12)
{
    uint32_t at = 0;
    uint32_t v0 = 0;
    float f0 = 0.0f;
    float f1 = 0.0f;
    uint32_t fcr31 = 0;
    uint64_t hi = 0;
    uint64_t lo = 0;
    v0 = 0x560u;
    f1 = a1_flt(0u);
    { int64_t result = (int64_t)(int32_t)a0 * (int64_t)(int32_t)v0; lo = (uint64_t)(int64_t)(int32_t)result; hi = (uint64_t)(int64_t)(int32_t)(result >> 32); (a0 = (uint32_t)((int32_t)((int32_t)result))); }
    at = 0x1220000u;
    at = at - 0x4328u;
    a0 = at + a0;
    f0 = a1_flt(A1_R32B(a0, 76));
    fcr31 = (a1f::eq(f0, f1)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
    if ((fcr31 & 0x800000u) == 0u) {
        A1_W32B(a0, 80, a1_bits(f12));
    } else {
    }
    {
        return a1::RetVF{(uint64_t)a1_sx64(v0), f0};
    }
}

// adpcmCloseChkComplete (0x1157a00): typed by tools/typed_lift.py (round A1)
inline uint64_t adpcmCloseChkComplete(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint64_t a0)
{
    uint64_t v0 = 0;
    (v0 = (uint64_t)(a0 ^ (uint64_t)(uint16_t)1));
    {
        (v0 = (uint64_t)(((uint64_t)v0 < (uint64_t)(int64_t)(int32_t)1) ? 1 : 0));
        return v0;
    }
}

// adpcmDeleteAdpcmSeq (0x1157a10): typed by tools/typed_lift.py (round A1)
inline uint64_t adpcmDeleteAdpcmSeq(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint64_t a0)
{
    uint64_t at = 0;
    uint64_t v0 = 0;
    uint64_t v1 = 0;
    uint64_t hi = 0;
    uint64_t lo = 0;
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 1376))));
    (v1 = (uint64_t)(0ULL | (uint64_t)(uint16_t)32768));
    (v1 = (uint64_t)(RC_U64X(v1) << 23));
    { int64_t result = (int64_t)(int32_t)a0 * (int64_t)(int32_t)v0; lo = (uint64_t)(int64_t)(int32_t)result; hi = (uint64_t)(int64_t)(int32_t)(result >> 32); (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)result))); }
    (at = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)290 << 16))));
    (at = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)at, 4294950104))));
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)at, (uint32_t)a0))));
    (v0 = (uint64_t)(A1_R64B((uint32_t)a0, 56)));
    (v0 = (uint64_t)(v0 | v1));
    {
        A1_W64B((uint32_t)a0, 56, v0);
        return v0;
    }
}

// adpcmSetVol (0x1157a40): typed by tools/typed_lift.py (round A1)
inline uint64_t adpcmSetVol(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint64_t a0, float f12, float f13)
{
    uint64_t at = 0;
    uint64_t v0 = 0;
    uint64_t hi = 0;
    uint64_t lo = 0;
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 1376))));
    { int64_t result = (int64_t)(int32_t)a0 * (int64_t)(int32_t)v0; lo = (uint64_t)(int64_t)(int32_t)result; hi = (uint64_t)(int64_t)(int32_t)(result >> 32); (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)result))); }
    (at = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)290 << 16))));
    (at = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)at, 4294950104))));
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)at, (uint32_t)a0))));
    (v0 = (uint64_t)(A1_R64B((uint32_t)a0, 56)));
    (v0 = (uint64_t)(v0 >> (32 + 7)));
    (v0 = (uint64_t)(v0 & (uint64_t)(uint16_t)1));
    if ((int64_t)v0 == 0) {
        { float f = f12; uint32_t bits; std::memcpy(&bits, &f, sizeof(bits)); A1_W32B((uint32_t)a0, 88, bits); }
        { float f = f13; uint32_t bits; std::memcpy(&bits, &f, sizeof(bits)); A1_W32B((uint32_t)a0, 80, bits); }
    }
    {
        return v0;
    }
}

// adpcmSetPitch (0x1157a78): typed by tools/typed_lift.py (round A1)
inline uint32_t adpcmSetPitch(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t a0, float f12, float f13)
{
    uint32_t at = 0;
    uint32_t v0 = 0;
    uint64_t hi = 0;
    uint64_t lo = 0;
    v0 = 0x560u;
    { int64_t result = (int64_t)(int32_t)a0 * (int64_t)(int32_t)v0; lo = (uint64_t)(int64_t)(int32_t)result; hi = (uint64_t)(int64_t)(int32_t)(result >> 32); (a0 = (uint32_t)((int32_t)((int32_t)result))); }
    at = 0x1220000u;
    at = at - 0x4328u;
    a0 = at + a0;
    A1_W32B(a0, 100, a1_bits(f13));
    {
        A1_W32B(a0, 96, a1_bits(f12));
        return v0;
    }
}

// adpcmGamePause (0x1157b78): typed by tools/typed_lift.py (round A1)
inline uint32_t adpcmGamePause(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t a0)
{
    uint32_t v0 = 0;
    v0 = 0x1190000u;
    {
        A1_W32(v0 - 0x508u, a0);
        return v0;
    }
}

// seSetVol (0x11587a8): typed by tools/typed_lift.py (round A1)
inline void seSetVol(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t a0, float f12)
{
    {
        A1_W32B(a0, 24, a1_bits(f12));
        
        return;
    }
}

// manager_sub_01159008 (0x1159008): typed by tools/typed_lift.py (round A1)
inline uint64_t manager_sub_01159008(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t a0)
{
    uint32_t a1 = 0;
    uint32_t a2 = 0;
    uint32_t a3 = 0;
    uint64_t v0 = 0;
    uint64_t v1 = 0;
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)290 << 16))));
    a1 = A1_R32B(a0, 72);
    (a0 = (uint32_t)((int32_t)((int32_t)A1_R32B((uint32_t)v0, -10464))));
    a2 = 0x1220000u;
    a3 = 0x1220000u;
    (v0 = (uint64_t)(A1_R64B(a0, 0)));
    A1_W64B(a1, 0, v0);
    (v1 = (uint64_t)(A1_R64B(a0, 8)));
    A1_W64B(a1, 8, v1);
    (v0 = (uint64_t)(A1_R64B(a0, 16)));
    A1_W64B(a1, 16, v0);
    (v1 = (uint64_t)(A1_R64B(a0, 24)));
    A1_W64B(a1, 24, v1);
    (v0 = (uint64_t)(A1_R64B(a0, 32)));
    A1_W64B(a1, 32, v0);
    (v1 = (uint64_t)(A1_R64B(a0, 40)));
    A1_W64B(a1, 40, v1);
    (v0 = (uint64_t)(A1_R64B(a0, 48)));
    A1_W64B(a1, 48, v0);
    (v1 = (uint64_t)(A1_R64B(a0, 56)));
    A1_W64B(a1, 56, v1);
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B(a2, -10456))));
    A1_W32B(a1, 64, (uint32_t)v0);
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B(a3, -10460))));
    {
        A1_W32B(a1, 68, (uint32_t)v1);
        return v0;
    }
}

// seDelayPlay3DTest (0x11590b0): typed by tools/typed_lift.py (round A1)
inline uint64_t seDelayPlay3DTest(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt)
{
    uint64_t v0 = 0;
    {
        (v0 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL));
        return v0;
    }
}

// sheetFsAddStageTexSegIndex (0x115bab8): typed by tools/typed_lift.py (round A1)
inline uint32_t sheetFsAddStageTexSegIndex(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t a0)
{
    uint32_t v0 = 0;
    uint32_t v1 = 0;
    v1 = 0x1220000u;
    a0 = a0 << 3;
    v0 = A1_R32(v1 - 0x1408u);
    a0 = a0 + v0;
    v0 = A1_R32(a0);
    v1 = v0 + 1u;
    {
        A1_W32(a0, v1);
        return v0;
    }
}

// sheetFsDecStageTexSegIndex (0x115bad8): typed by tools/typed_lift.py (round A1)
inline uint32_t sheetFsDecStageTexSegIndex(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t a0)
{
    uint32_t v0 = 0;
    uint32_t v1 = 0;
    v1 = 0x1220000u;
    a0 = a0 << 3;
    v0 = A1_R32(v1 - 0x1408u);
    a0 = a0 + v0;
    v0 = A1_R32(a0);
    v0 = v0 - 1u;
    {
        A1_W32(a0, v0);
        return v0;
    }
}

// sheetFsSetStageTexSegTexSegDef (0x115bb98): typed by tools/typed_lift.py (round A1)
inline uint32_t sheetFsSetStageTexSegTexSegDef(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t a0, uint32_t a1)
{
    uint32_t v0 = 0;
    uint32_t v1 = 0;
    v0 = 0x1220000u;
    a0 = a0 << 3;
    v1 = A1_R32(v0 - 0x1408u);
    a0 = a0 + v1;
    {
        A1_W32B(a0, 4, a1);
        return v0;
    }
}

// sheetFsGetStageTexSegTexSegDef (0x115bbb0): typed by tools/typed_lift.py (round A1)
inline uint32_t sheetFsGetStageTexSegTexSegDef(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t a0)
{
    uint32_t v0 = 0;
    uint32_t v1 = 0;
    v0 = 0x1220000u;
    a0 = a0 << 3;
    v1 = A1_R32(v0 - 0x1408u);
    a0 = a0 + v1;
    {
        v0 = A1_R32B(a0, 4);
        return v0;
    }
}

// sheetFsGetDirectFileSize (0x115c1e0): typed by tools/typed_lift.py (round A1)
inline uint32_t sheetFsGetDirectFileSize(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t a0)
{
    uint32_t v0 = 0;
    uint32_t v1 = 0;
    v0 = 0x1220000u;
    a0 = a0 << 3;
    v1 = A1_R32(v0 - 0x13C4u);
    v1 = v1 + a0;
    {
        v0 = A1_R32B(v1, 4);
        return v0;
    }
}

// sheetFileGetSize (0x115cfc0): typed by tools/typed_lift.py (round A1)
inline uint32_t sheetFileGetSize(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t a0)
{
    uint32_t v0 = 0;
    {
        v0 = A1_R32B(a0, 16);
        return v0;
    }
}

// sheetHeaderObjSearchSheetId (0x115ec48): typed by tools/typed_lift.py (round A1)
inline uint32_t sheetHeaderObjSearchSheetId(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t a0)
{
    uint32_t v0 = 0;
    uint32_t v1 = 0;
    v0 = 0x1190000u;
    a0 = a0 << 2;
    v1 = A1_R32(v0 - 0x1E0u);
    a0 = a0 + v1;
    {
        v0 = A1_R32(a0);
        return v0;
    }
}

// sheetMgrAddSegLoadList (0x115ed10): typed by tools/typed_lift.py (round A1)
inline uint32_t sheetMgrAddSegLoadList(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t a0)
{
    uint32_t v0 = 0;
    uint32_t v1 = 0;
    v0 = 0x1220000u;
    v0 = v0 - 0x1378u;
    v1 = A1_R32(v0);
    A1_W32B(a0, 76, v1);
    {
        A1_W32(v0, a0);
        return v0;
    }
}

// sheetMgrGetSegLoadHeader (0x115ed28): typed by tools/typed_lift.py (round A1)
inline uint32_t sheetMgrGetSegLoadHeader(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt)
{
    uint32_t v0 = 0;
    uint32_t v1 = 0;
    v1 = 0x1220000u;
    {
        v0 = A1_R32(v1 - 0x1378u);
        return v0;
    }
}

// sheetMgrAddSegInitList (0x115ed38): typed by tools/typed_lift.py (round A1)
inline uint32_t sheetMgrAddSegInitList(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t a0)
{
    uint32_t v0 = 0;
    uint32_t v1 = 0;
    v1 = 0x1220000u;
    A1_W32B(a0, 80, 0u);
    v1 = v1 - 0x1370u;
    v0 = A1_R32(v1);
    if (v0 == 0) {
        A1_W32(v1, a0);
    } else {
        v1 = 0x1220000u;
        v0 = A1_R32(v1 - 0x136Cu);
        A1_W32B(v0, 80, a0);
        goto L_115ed64;
    }
    v1 = 0x1220000u;
L_115ed64:
    {
        A1_W32(v1 - 0x136Cu, a0);
        return v0;
    }
}

// sheetMgrGetSegInitHeader (0x115edb0): typed by tools/typed_lift.py (round A1)
inline uint32_t sheetMgrGetSegInitHeader(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt)
{
    uint32_t v0 = 0;
    uint32_t v1 = 0;
    v1 = 0x1220000u;
    {
        v0 = A1_R32(v1 - 0x1370u);
        return v0;
    }
}

// sheetMgrSetNoWaitMode (0x115fa20): typed by tools/typed_lift.py (round A1)
inline uint32_t sheetMgrSetNoWaitMode(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t a0)
{
    uint32_t v0 = 0;
    v0 = 0x1190000u;
    {
        A1_W32(v0 - 0x1D0u, a0);
        return v0;
    }
}

// get_camera_lws_animation_param (0x1161c80): typed by tools/typed_lift.py (round A1)
inline uint32_t get_camera_lws_animation_param(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t a0)
{
    uint32_t v0 = 0;
    {
        v0 = A1_R32(a0);
        return v0;
    }
}

// get_boot_obj_camera_lws_animation (0x1161f70): typed by tools/typed_lift.py (round A1)
inline uint32_t get_boot_obj_camera_lws_animation(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t a0)
{
    uint32_t v0 = 0;
    {
        v0 = A1_R32B(a0, 172);
        return v0;
    }
}

// is_loop_camera_lws_animation (0x1161f78): typed by tools/typed_lift.py (round A1)
inline uint32_t is_loop_camera_lws_animation(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t a0)
{
    uint32_t v0 = 0;
    v0 = A1_R32B(a0, 16);
    v0 = v0 & 7u;
    {
        v0 = 0u < v0 ? 1u : 0u;
        return v0;
    }
}

// is_sub_type_camera_lws_animation (0x1161fa0): typed by tools/typed_lift.py (round A1)
inline uint32_t is_sub_type_camera_lws_animation(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t a0)
{
    uint32_t v0 = 0;
    v0 = A1_R32B(a0, 16);
    v0 = v0 >> 3;
    v0 = v0 & 7u;
    v0 = v0 ^ 1u;
    {
        v0 = v0 < 1u ? 1u : 0u;
        return v0;
    }
}

// reset_main_type_camera_lws_animation_update_flag (0x1162018): typed by tools/typed_lift.py (round A1)
inline uint32_t reset_main_type_camera_lws_animation_update_flag(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt)
{
    uint32_t v0 = 0;
    v0 = 0x1190000u;
    {
        A1_W32(v0 - 0x1B0u, 0u);
        return v0;
    }
}

// is_same_camera_lws_animation (0x1162150): typed by tools/typed_lift.py (round A1)
inline uint64_t is_same_camera_lws_animation(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint64_t a0, uint64_t a1)
{
    uint32_t v1 = 0;
    uint64_t v0 = 0;
    bool df_v1 = false;
    (v0 = (uint64_t)(((uint64_t)a0 < (uint64_t)(int64_t)(int32_t)10) ? 1 : 0));
    if ((int64_t)v0 == 0) {
        (v0 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL));
    } else {
        {
            const bool c = (int64_t)a1 == 0;
            (v1 = (uint32_t)((int32_t)((int32_t)SLL32((uint32_t)a0, 1)))); df_v1 = true;
            if (!c) {
                (v1 = (uint32_t)((int32_t)((int32_t)ADD32(v1, (uint32_t)a0)))); df_v1 = true;
                v1 = v1 << 6; df_v1 = true;
                (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)291 << 16))));
                (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)a0, v1))));
                (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)a0, -20464))));
                {
                    const bool c = (int64_t)a0 == (int64_t)a1;
                    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 1))));
                    if (c) {
                        goto L_1162184;
                    }
                }
            }
        }
        (v0 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL));
    }
L_1162184:
    {
        return v0;
    }
}

// stop_camera_lws_animation_geometry_pointer_reference (0x1162208): typed by tools/typed_lift.py (round A1)
inline uint32_t stop_camera_lws_animation_geometry_pointer_reference(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint64_t a0)
{
    uint32_t at = 0;
    uint32_t v0 = 0;
    uint64_t v1 = 0;
    (v0 = (uint32_t)((int32_t)((int32_t)SLL32((uint32_t)a0, 1))));
    (v1 = (uint64_t)(((uint64_t)a0 < (uint64_t)(int64_t)(int32_t)10) ? 1 : 0));
    {
        const bool c = (int64_t)v1 == 0;
        (v0 = (uint32_t)((int32_t)((int32_t)ADD32(v0, (uint32_t)a0))));
        if (!c) {
            (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)SLL32(v0, 6))));
            at = 0x1230000u;
            (at = (uint32_t)((int32_t)((int32_t)ADD32(at, (uint32_t)v1))));
            A1_W32(at - 0x4FA8u, 0u);
            {
                v0 = 1u;
                return v0;
            }
        }
    }
    {
        v0 = 0u;
        return v0;
    }
}

// stop_camera_lws_animation_callback (0x1162238): typed by tools/typed_lift.py (round A1)
inline uint32_t stop_camera_lws_animation_callback(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint64_t a0)
{
    uint32_t at = 0;
    uint32_t v0 = 0;
    uint64_t v1 = 0;
    (v0 = (uint32_t)((int32_t)((int32_t)SLL32((uint32_t)a0, 1))));
    (v1 = (uint64_t)(((uint64_t)a0 < (uint64_t)(int64_t)(int32_t)10) ? 1 : 0));
    {
        const bool c = (int64_t)v1 == 0;
        (v0 = (uint32_t)((int32_t)((int32_t)ADD32(v0, (uint32_t)a0))));
        if (!c) {
            (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)SLL32(v0, 6))));
            at = 0x1230000u;
            (at = (uint32_t)((int32_t)((int32_t)ADD32(at, (uint32_t)v1))));
            A1_W32(at - 0x4F50u, 0u);
            {
                v0 = 1u;
                return v0;
            }
        }
    }
    {
        v0 = 0u;
        return v0;
    }
}

// manager_sub_011629B8 (0x11629b8): typed by tools/typed_lift.py (round A1)
inline uint32_t manager_sub_011629B8(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t a0)
{
    uint32_t v0 = 0;
    float f0 = 0.0f;
    float f1 = 0.0f;
    uint32_t fcr31 = 0;
    v0 = A1_R32B(a0, 100);
    {
        const bool c = v0 == 0;
        v0 = 0u;
        if (!c) {
            f1 = a1_flt(A1_R32B(a0, 104));
            f0 = a1_flt(0u);
            fcr31 = (a1f::le(f0, f1)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
            {
                const bool c = (fcr31 & 0x800000u) != 0u;
                v0 = 1u;
                if (!c) {
                    v0 = 0u;
                }
            }
        }
    }
    {
        return v0;
    }
}

// get_clip_screen (0x1162f10): typed by tools/typed_lift.py (round A1)
inline uint32_t get_clip_screen(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t a0)
{
    uint32_t v0 = 0;
    {
        v0 = A1_R32B(a0, 592);
        return v0;
    }
}

// get_transform_matrix_set (0x1162f18): typed by tools/typed_lift.py (round A1)
inline uint32_t get_transform_matrix_set(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t a0)
{
    uint32_t v0 = 0;
    {
        v0 = A1_R32B(a0, 596);
        return v0;
    }
}

// get_change_grace_time_camera_region (0x1163808): typed by tools/typed_lift.py (round A1)
inline float get_change_grace_time_camera_region(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint64_t a0)
{
    uint32_t v0 = 0;
    uint64_t v1 = 0;
    float f0 = 0.0f;
    bool df_v0 = false;
    {
        const bool c = (int64_t)a0 == 0;
        (v1 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL));
        if (!c) {
            (v0 = (uint32_t)((int32_t)((int32_t)A1_R32B((uint32_t)a0, 20)))); df_v0 = true;
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
        (v0 = (uint32_t)((int32_t)((int32_t)A1_R32B((uint32_t)a0, 20)))); df_v0 = true;
        {
            f0 = a1_flt(A1_R32B(v0, 24));
            return f0;
        }
    }
    { uint32_t bits = 0u; std::memcpy(&f0, &bits, sizeof(bits)); }
    {
        return f0;
    }
}

// manager_sub_01164260 (0x1164260): typed by tools/typed_lift.py (round A1)
inline uint64_t manager_sub_01164260(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint64_t a0)
{
    uint32_t v1 = 0;
    uint64_t v0 = 0;
    bool df_v1 = false;
    {
        const bool c = (int64_t)a0 == 0;
        (v0 = (uint64_t)((uint64_t)a0 + (uint64_t)0ULL));
        if (!c) {
            (v1 = (uint32_t)((int32_t)((int32_t)A1_R32B((uint32_t)a0, 20)))); df_v1 = true;
            if (v1 != 0) {
                goto L_1164278;
            }
        }
    }
    (v0 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL));
L_1164278:
    {
        return v0;
    }
}

// set_screen_wide_and_progressive (0x1164468): typed by tools/typed_lift.py (round A1)
inline a1::RetVF set_screen_wide_and_progressive(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint64_t a0, uint64_t a1)
{
    uint32_t at = 0;
    uint32_t v0 = 0;
    float f0 = 0.0f;
    if ((int64_t)a0 != 0) {
        if ((int64_t)a1 == 0) {
            at = 0x3FAA0000u;
            at = at | 0xAAAAu;
            { uint32_t bits = at; std::memcpy(&f0, &bits, sizeof(bits)); }
            v0 = 0x1190000u;
            goto L_11644c0;
        }
        at = 0x3F800000u;
        { uint32_t bits = at; std::memcpy(&f0, &bits, sizeof(bits)); }
        v0 = 0x1190000u;
        goto L_11644c0;
    }
    {
        const bool c = (int64_t)a1 != 0;
        v0 = 0x1190000u;
        if (!c) {
            at = 0x3F800000u;
            { uint32_t bits = at; std::memcpy(&f0, &bits, sizeof(bits)); }
            goto L_11644c0;
        }
    }
    at = 0x3F400000u;
    { uint32_t bits = at; std::memcpy(&f0, &bits, sizeof(bits)); }
L_11644c0:
    {
        A1_W32(v0 - 0x19Cu, a1_bits(f0));
        return a1::RetVF{(uint64_t)a1_sx64(v0), f0};
    }
}

// set_view_angle (0x11644c8): typed by tools/typed_lift.py (round A1)
inline void set_view_angle(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t a0, float f12)
{
    {
        A1_W32B(a0, 12, a1_bits(f12));
        
        return;
    }
}

// get_screen_width (0x11644f0): typed by tools/typed_lift.py (round A1)
inline uint32_t get_screen_width(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t a0)
{
    uint32_t v0 = 0;
    {
        v0 = A1_R32(a0);
        return v0;
    }
}

// get_screen_height (0x11644f8): typed by tools/typed_lift.py (round A1)
inline uint32_t get_screen_height(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t a0)
{
    uint32_t v0 = 0;
    {
        v0 = A1_R32B(a0, 4);
        return v0;
    }
}

// get_aspect_ratio (0x1164500): typed by tools/typed_lift.py (round A1)
inline a1::RetVF get_aspect_ratio(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t a0)
{
    uint32_t v0 = 0;
    uint32_t v1 = 0;
    float f0 = 0.0f;
    float f1 = 0.0f;
    float f2 = 0.0f;
    uint32_t fcr31 = 0;
    v1 = 0x1190000u;
    f0 = a1_flt(A1_R32B(a0, 8));
    f2 = a1_flt(A1_R32(v1 - 0x19Cu));
    v0 = 0x1190000u;
    f1 = a1_flt(A1_R32(v0 - 0x198u));
    f0 = a1f::mul(f0, f2);
    f0 = a1f::div(fcr31, f0, f1);
    {
        return a1::RetVF{(uint64_t)a1_sx64(v0), f0};
    }
}

// get_view_angle (0x1164530): typed by tools/typed_lift.py (round A1)
inline float get_view_angle(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t a0)
{
    float f0 = 0.0f;
    {
        f0 = a1_flt(A1_R32B(a0, 12));
        return f0;
    }
}

// get_near_clip_z_clip_screen (0x1164558): typed by tools/typed_lift.py (round A1)
inline float get_near_clip_z_clip_screen(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt)
{
    uint32_t at = 0;
    float f0 = 0.0f;
    at = 0x3D4C0000u;
    at = at | 0xCCCCu;
    f0 = a1_flt(at);
    {
        return f0;
    }
}

// get_far_clip_z_clip_screen (0x1164570): typed by tools/typed_lift.py (round A1)
inline float get_far_clip_z_clip_screen(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt)
{
    uint32_t at = 0;
    float f0 = 0.0f;
    at = 0x477F0000u;
    at = at | 0xFF00u;
    f0 = a1_flt(at);
    {
        return f0;
    }
}

// get_z_buf_min_clip_screen (0x1164588): typed by tools/typed_lift.py (round A1)
inline float get_z_buf_min_clip_screen(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt)
{
    uint32_t at = 0;
    float f0 = 0.0f;
    at = 0x437F0000u;
    f0 = a1_flt(at);
    {
        return f0;
    }
}

// get_z_buf_max_clip_screen (0x1164598): typed by tools/typed_lift.py (round A1)
inline float get_z_buf_max_clip_screen(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt)
{
    uint32_t at = 0;
    float f0 = 0.0f;
    at = 0x4B7E0000u;
    at = at | 0xD260u;
    f0 = a1_flt(at);
    {
        return f0;
    }
}

// get_scissor_clip_matrix (0x1164ca8): typed by tools/typed_lift.py (round A1)
inline uint32_t get_scissor_clip_matrix(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t a0, uint32_t a1)
{
    uint32_t v0 = 0;
    a1 = a1 << 6;
    v0 = a0 + a1;
    {
        v0 = v0 + 0x1E0u;
        return v0;
    }
}

// get_WV_matrix (0x1164cb8): typed by tools/typed_lift.py (round A1)
inline uint64_t get_WV_matrix(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint64_t a0)
{
    uint64_t v0 = 0;
    {
        (v0 = (uint64_t)((uint64_t)a0 + (uint64_t)0ULL));
        return v0;
    }
}

// ResetDamageClass (0x1166de8): typed by tools/typed_lift.py (round A1)
inline uint32_t ResetDamageClass(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t a0)
{
    uint32_t v0 = 0;
    v0 = 0xFFFFFFFFu;
    A1_W32B(a0, 104, 0u);
    A1_W32B(a0, 96, v0);
    A1_W32B(a0, 84, v0);
    A1_W32B(a0, 88, 0u);
    A1_W32(a0, 0u);
    A1_W32B(a0, 92, 0u);
    {
        A1_W32B(a0, 100, 0u);
        return v0;
    }
}

// SetDamageClassDamageTableType (0x1166eb8): typed by tools/typed_lift.py (round A1)
inline uint64_t SetDamageClassDamageTableType(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint64_t a0)
{
    uint32_t v1 = 0;
    uint64_t v0 = 0;
    (v0 = (uint64_t)((uint64_t)a0 + (uint64_t)0ULL));
    v1 = 0x1190000u;
    {
        A1_W32B(v1, -392, (uint32_t)v0);
        return v0;
    }
}

// getEditWorkInitData (0x1169c30): typed by tools/typed_lift.py (round A1)
inline uint64_t getEditWorkInitData(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint64_t a0)
{
    uint64_t v0 = 0;
    if ((int64_t)a0 != 0) {
        (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)a0, 8))));
        {
            (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)a0, (uint32_t)v0))));
            return v0;
        }
    }
    {
        (v0 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL));
        return v0;
    }
}

// getPointData (0x1169d48): typed by tools/typed_lift.py (round A1)
inline uint64_t getPointData(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint64_t a0, uint64_t a1)
{
    uint32_t v1 = 0;
    uint64_t v0 = 0;
    bool df_v1 = false;
    {
        const bool c = (int64_t)a0 == 0;
        (v0 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL));
        if (!c) {
            if ((int32_t)a1 >= 0) {
                (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)SLL32((uint32_t)a1, 1))));
                (v1 = (uint32_t)((int32_t)((int32_t)A1_R32B((uint32_t)a0, 0)))); df_v1 = true;
                (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v0, (uint32_t)a1))));
                (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)SLL32((uint32_t)v0, 4))));
                (v1 = (uint32_t)(((uint64_t)a1 < (uint64_t)a1_sx64(v1)) ? 1 : 0)); df_v1 = true;
                (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)a0, (uint32_t)v0))));
                {
                    const bool c = v1 != 0;
                    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v0, 16))));
                    if (!c) {
                        (v0 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL));
                    }
                }
            }
        }
    }
    {
        return v0;
    }
}

// getLineData (0x1169de0): typed by tools/typed_lift.py (round A1)
inline uint64_t getLineData(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint64_t a0, uint64_t a1)
{
    uint32_t v1 = 0;
    uint64_t v0 = 0;
    bool df_v1 = false;
    {
        const bool c = (int64_t)a0 == 0;
        (v0 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL));
        if (!c) {
            if ((int32_t)a1 >= 0) {
                (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)SLL32((uint32_t)a1, 1))));
                (v1 = (uint32_t)((int32_t)((int32_t)A1_R32B((uint32_t)a0, 0)))); df_v1 = true;
                (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v0, (uint32_t)a1))));
                (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)SLL32((uint32_t)v0, 4))));
                (v1 = (uint32_t)(((uint64_t)a1 < (uint64_t)a1_sx64(v1)) ? 1 : 0)); df_v1 = true;
                (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)a0, (uint32_t)v0))));
                {
                    const bool c = v1 != 0;
                    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v0, 16))));
                    if (!c) {
                        (v0 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL));
                    }
                }
            }
        }
    }
    {
        return v0;
    }
}

// getRootPointData (0x1169e68): typed by tools/typed_lift.py (round A1)
inline uint64_t getRootPointData(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint64_t a0, uint64_t a1)
{
    uint32_t v1 = 0;
    uint64_t v0 = 0;
    bool df_v1 = false;
    {
        const bool c = (int64_t)a0 == 0;
        (v0 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL));
        if (!c) {
            if ((int32_t)a1 >= 0) {
                (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)SLL32((uint32_t)a1, 1))));
                (v1 = (uint32_t)((int32_t)((int32_t)A1_R32B((uint32_t)a0, 0)))); df_v1 = true;
                (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v0, (uint32_t)a1))));
                (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)SLL32((uint32_t)v0, 4))));
                (v1 = (uint32_t)(((uint64_t)a1 < (uint64_t)a1_sx64(v1)) ? 1 : 0)); df_v1 = true;
                (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)a0, (uint32_t)v0))));
                {
                    const bool c = v1 != 0;
                    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v0, 16))));
                    if (!c) {
                        (v0 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL));
                    }
                }
            }
        }
    }
    {
        return v0;
    }
}

// getGoalPointData (0x1169ea8): typed by tools/typed_lift.py (round A1)
inline uint64_t getGoalPointData(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint64_t a0, uint64_t a1)
{
    uint32_t v1 = 0;
    uint64_t v0 = 0;
    bool df_v1 = false;
    {
        const bool c = (int64_t)a0 == 0;
        (v0 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL));
        if (!c) {
            if ((int32_t)a1 >= 0) {
                (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)SLL32((uint32_t)a1, 1))));
                (v1 = (uint32_t)((int32_t)((int32_t)A1_R32B((uint32_t)a0, 0)))); df_v1 = true;
                (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v0, (uint32_t)a1))));
                (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)SLL32((uint32_t)v0, 4))));
                (v1 = (uint32_t)(((uint64_t)a1 < (uint64_t)a1_sx64(v1)) ? 1 : 0)); df_v1 = true;
                (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)a0, (uint32_t)v0))));
                {
                    const bool c = v1 != 0;
                    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v0, 16))));
                    if (!c) {
                        (v0 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL));
                    }
                }
            }
        }
    }
    {
        return v0;
    }
}

// getSubPointData (0x1169ee8): typed by tools/typed_lift.py (round A1)
inline uint64_t getSubPointData(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint64_t a0, uint64_t a1)
{
    uint32_t v1 = 0;
    uint64_t v0 = 0;
    bool df_v1 = false;
    {
        const bool c = (int64_t)a0 == 0;
        (v0 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL));
        if (!c) {
            if ((int32_t)a1 >= 0) {
                (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)SLL32((uint32_t)a1, 1))));
                (v1 = (uint32_t)((int32_t)((int32_t)A1_R32B((uint32_t)a0, 0)))); df_v1 = true;
                (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v0, (uint32_t)a1))));
                (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)SLL32((uint32_t)v0, 4))));
                (v1 = (uint32_t)(((uint64_t)a1 < (uint64_t)a1_sx64(v1)) ? 1 : 0)); df_v1 = true;
                (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)a0, (uint32_t)v0))));
                {
                    const bool c = v1 != 0;
                    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v0, 16))));
                    if (!c) {
                        (v0 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL));
                    }
                }
            }
        }
    }
    {
        return v0;
    }
}

// getInterestPointData (0x1169ff0): typed by tools/typed_lift.py (round A1)
inline uint64_t getInterestPointData(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint64_t a0, uint64_t a1)
{
    uint32_t v1 = 0;
    uint64_t v0 = 0;
    bool df_v1 = false;
    {
        const bool c = (int64_t)a0 == 0;
        (v0 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL));
        if (!c) {
            if ((int32_t)a1 >= 0) {
                (v1 = (uint32_t)((int32_t)((int32_t)A1_R32B((uint32_t)a0, 0)))); df_v1 = true;
                (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)SLL32((uint32_t)a1, 5))));
                (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)a0, (uint32_t)v0))));
                (v1 = (uint32_t)(((uint64_t)a1 < (uint64_t)a1_sx64(v1)) ? 1 : 0)); df_v1 = true;
                {
                    const bool c = v1 != 0;
                    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v0, 16))));
                    if (!c) {
                        (v0 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL));
                    }
                }
            }
        }
    }
    {
        return v0;
    }
}

// getDeadPointData (0x116a040): typed by tools/typed_lift.py (round A1)
inline uint64_t getDeadPointData(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint64_t a0, uint64_t a1)
{
    uint32_t v1 = 0;
    uint64_t v0 = 0;
    bool df_v1 = false;
    {
        const bool c = (int64_t)a0 == 0;
        (v0 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL));
        if (!c) {
            if ((int32_t)a1 >= 0) {
                (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)SLL32((uint32_t)a1, 1))));
                (v1 = (uint32_t)((int32_t)((int32_t)A1_R32B((uint32_t)a0, 0)))); df_v1 = true;
                (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v0, (uint32_t)a1))));
                (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)SLL32((uint32_t)v0, 4))));
                (v1 = (uint32_t)(((uint64_t)a1 < (uint64_t)a1_sx64(v1)) ? 1 : 0)); df_v1 = true;
                (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)a0, (uint32_t)v0))));
                {
                    const bool c = v1 != 0;
                    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v0, 16))));
                    if (!c) {
                        (v0 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL));
                    }
                }
            }
        }
    }
    {
        return v0;
    }
}

// getLayoutPointData (0x116a098): typed by tools/typed_lift.py (round A1)
inline uint64_t getLayoutPointData(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint64_t a0, uint64_t a1)
{
    uint32_t v1 = 0;
    uint64_t v0 = 0;
    bool df_v1 = false;
    {
        const bool c = (int64_t)a0 == 0;
        (v0 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL));
        if (!c) {
            if ((int32_t)a1 >= 0) {
                (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)SLL32((uint32_t)a1, 1))));
                (v1 = (uint32_t)((int32_t)((int32_t)A1_R32B((uint32_t)a0, 0)))); df_v1 = true;
                (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v0, (uint32_t)a1))));
                (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)SLL32((uint32_t)v0, 4))));
                (v1 = (uint32_t)(((uint64_t)a1 < (uint64_t)a1_sx64(v1)) ? 1 : 0)); df_v1 = true;
                (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)a0, (uint32_t)v0))));
                {
                    const bool c = v1 != 0;
                    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v0, 16))));
                    if (!c) {
                        (v0 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL));
                    }
                }
            }
        }
    }
    {
        return v0;
    }
}

// getSnapShotFormat (0x1341920): typed by tools/typed_lift.py (round A1)
inline uint32_t getSnapShotFormat(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt)
{
    uint32_t v0 = 0;
    v0 = 0x1460000u;
    {
        v0 = v0 + 0x544Cu;
        return v0;
    }
}

// GAMEUTIL_get_sight_on_reverse_cntrlLR_bool (0x1342b40): typed by tools/typed_lift.py (round A1)
inline uint32_t GAMEUTIL_get_sight_on_reverse_cntrlLR_bool(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt)
{
    uint32_t v0 = 0;
    uint32_t v1 = 0;
    v1 = 0x1460000u;
    v0 = A1_R32B(v1, 22032);
    {
        v0 = 0u < v0 ? 1u : 0u;
        return v0;
    }
}

// GAMEUTIL_get_sight_on_reverse_cntrlUD_bool (0x1342b70): typed by tools/typed_lift.py (round A1)
inline uint32_t GAMEUTIL_get_sight_on_reverse_cntrlUD_bool(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt)
{
    uint32_t v0 = 0;
    uint32_t v1 = 0;
    v1 = 0x1460000u;
    v0 = A1_R32B(v1, 22036);
    {
        v0 = 0u < v0 ? 1u : 0u;
        return v0;
    }
}

// GAMEUTIL_get_game_camera_free_mode_reverse_cntrlLR_bool (0x1342c70): typed by tools/typed_lift.py (round A1)
inline uint32_t GAMEUTIL_get_game_camera_free_mode_reverse_cntrlLR_bool(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt)
{
    uint32_t v0 = 0;
    uint32_t v1 = 0;
    v1 = 0x1460000u;
    v0 = A1_R32B(v1, 22044);
    {
        v0 = 0u < v0 ? 1u : 0u;
        return v0;
    }
}

// GAMEUTIL_get_game_camera_free_mode_reverse_cntrlUD_bool (0x1342ca0): typed by tools/typed_lift.py (round A1)
inline uint32_t GAMEUTIL_get_game_camera_free_mode_reverse_cntrlUD_bool(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt)
{
    uint32_t v0 = 0;
    uint32_t v1 = 0;
    v1 = 0x1460000u;
    v0 = A1_R32B(v1, 22048);
    {
        v0 = 0u < v0 ? 1u : 0u;
        return v0;
    }
}

// GAMEUTIL_set_auto_get_flag_boss_interest_position (0x1343b28): typed by tools/typed_lift.py (round A1)
inline uint32_t GAMEUTIL_set_auto_get_flag_boss_interest_position(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t a0)
{
    uint32_t v0 = 0;
    v0 = 0x1460000u;
    {
        A1_W32B(v0, 22056, a0);
        return v0;
    }
}

// GAMEUTIL_reset_move_angle (0x1343cd0): typed by tools/typed_lift.py (round A1)
inline void GAMEUTIL_reset_move_angle(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t a0)
{
    {
        A1_W32(a0, 0u);
        
        return;
    }
}

// GAMEUTIL_set_parameter_move_angle (0x1343cd8): typed by tools/typed_lift.py (round A1)
inline float GAMEUTIL_set_parameter_move_angle(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t a0, float f12, float f13, float f14, float f15, float f16, float f17)
{
    uint32_t at = 0;
    float f0 = 0.0f;
    at = 0x3C8E0000u;
    at = at | 0xFA35u;
    f0 = a1_flt(at);
    A1_W32B(a0, 16, a1_bits(f15));
    f17 = a1f::mul(f17, f0);
    f12 = a1f::mul(f12, f0);
    f13 = a1f::mul(f13, f0);
    f14 = a1f::mul(f14, f0);
    f16 = a1f::mul(f16, f0);
    A1_W32B(a0, 24, a1_bits(f17));
    A1_W32B(a0, 4, a1_bits(f12));
    A1_W32B(a0, 8, a1_bits(f13));
    A1_W32B(a0, 12, a1_bits(f14));
    {
        A1_W32B(a0, 20, a1_bits(f16));
        return f0;
    }
}

// GAMEUTIL_begin_move_angle (0x1343d18): typed by tools/typed_lift.py (round A1)
inline void GAMEUTIL_begin_move_angle(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t a0)
{
    {
        A1_W32(a0, 0u);
        
        return;
    }
}

// GAMEUTIL_get_cur_velocity_move_angle (0x1343e10): typed by tools/typed_lift.py (round A1)
inline float GAMEUTIL_get_cur_velocity_move_angle(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t a0)
{
    float f0 = 0.0f;
    {
        f0 = a1_flt(A1_R32(a0));
        return f0;
    }
}

// GAMEUTIL_set_need_dir_ratio_flag_look_at_vs_offset (0x1343ee8): typed by tools/typed_lift.py (round A1)
inline void GAMEUTIL_set_need_dir_ratio_flag_look_at_vs_offset(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t a0, uint32_t a1)
{
    {
        A1_W32B(a0, 24, a1);
        
        return;
    }
}

// GAMEUTIL_set_enable_side_change_angle_look_at_vs_offset (0x1343ef0): typed by tools/typed_lift.py (round A1)
inline void GAMEUTIL_set_enable_side_change_angle_look_at_vs_offset(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t a0, float f12)
{
    {
        A1_W32B(a0, 80, a1_bits(f12));
        
        return;
    }
}

// GAMEUTIL_set_parameter_col_plane_info_filter (0x13440a8): typed by tools/typed_lift.py (round A1)
inline void GAMEUTIL_set_parameter_col_plane_info_filter(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t a0, float f12, float f13)
{
    A1_W32B(a0, 116, a1_bits(f13));
    {
        A1_W32B(a0, 112, a1_bits(f12));
        
        return;
    }
}

// GAMEUTIL_set_parameter_position_filter (0x1344208): typed by tools/typed_lift.py (round A1)
inline void GAMEUTIL_set_parameter_position_filter(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t a0, float f12)
{
    {
        A1_W32B(a0, 36, a1_bits(f12));
        
        return;
    }
}

// GAMEUTIL_reset_status_manager (0x13445c0): typed by tools/typed_lift.py (round A1)
inline void GAMEUTIL_reset_status_manager(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t a0, uint32_t a1)
{
    A1_W32(a0, a1);
    A1_W32B(a0, 12, 0u);
    A1_W32B(a0, 8, a1);
    {
        A1_W32B(a0, 4, a1);
        
        return;
    }
}

// GAMEUTIL_is_status_change_status_manager (0x1344628): typed by tools/typed_lift.py (round A1)
inline a1::RetVF GAMEUTIL_is_status_change_status_manager(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t a0)
{
    uint32_t v0 = 0;
    float f0 = 0.0f;
    float f1 = 0.0f;
    uint32_t fcr31 = 0;
    f1 = a1_flt(A1_R32B(a0, 12));
    f0 = a1_flt(0u);
    fcr31 = (a1f::le(f1, f0)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
    {
        const bool c = (fcr31 & 0x800000u) != 0u;
        v0 = 1u;
        if (!c) {
            v0 = 0u;
        }
    }
    {
        return a1::RetVF{(uint64_t)a1_sx64(v0), f0};
    }
}

// GAMEUTIL_get_status_continue_time_status_manager (0x1344648): typed by tools/typed_lift.py (round A1)
inline float GAMEUTIL_get_status_continue_time_status_manager(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t a0)
{
    float f0 = 0.0f;
    {
        f0 = a1_flt(A1_R32B(a0, 12));
        return f0;
    }
}

// GAMEUTIL_get_cur_status_status_manager (0x1344650): typed by tools/typed_lift.py (round A1)
inline uint32_t GAMEUTIL_get_cur_status_status_manager(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t a0)
{
    uint32_t v0 = 0;
    {
        v0 = A1_R32(a0);
        return v0;
    }
}

// GAMEUTIL_get_pre_status_status_manager (0x1344658): typed by tools/typed_lift.py (round A1)
inline uint32_t GAMEUTIL_get_pre_status_status_manager(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t a0)
{
    uint32_t v0 = 0;
    {
        v0 = A1_R32B(a0, 4);
        return v0;
    }
}

// GAMEUTIL_get_pre_changed_status_status_manager (0x1344660): typed by tools/typed_lift.py (round A1)
inline uint32_t GAMEUTIL_get_pre_changed_status_status_manager(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t a0)
{
    uint32_t v0 = 0;
    {
        v0 = A1_R32B(a0, 8);
        return v0;
    }
}

// getSightOnRevCntrlLR (0x1344668): typed by tools/typed_lift.py (round A1)
inline uint32_t getSightOnRevCntrlLR(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt)
{
    uint32_t v0 = 0;
    v0 = 0x1460000u;
    {
        v0 = v0 + 0x5610u;
        return v0;
    }
}

// getSightOnRevCntrlUD (0x1344678): typed by tools/typed_lift.py (round A1)
inline uint32_t getSightOnRevCntrlUD(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt)
{
    uint32_t v0 = 0;
    v0 = 0x1460000u;
    {
        v0 = v0 + 0x5614u;
        return v0;
    }
}

// getFreeModeRevControlLR (0x1344698): typed by tools/typed_lift.py (round A1)
inline uint32_t getFreeModeRevControlLR(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt)
{
    uint32_t v0 = 0;
    v0 = 0x1460000u;
    {
        v0 = v0 + 0x561Cu;
        return v0;
    }
}

// getFreeModeRevControlUD (0x13446a8): typed by tools/typed_lift.py (round A1)
inline uint32_t getFreeModeRevControlUD(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt)
{
    uint32_t v0 = 0;
    v0 = 0x1460000u;
    {
        v0 = v0 + 0x5620u;
        return v0;
    }
}

// getFreeModeRevAngleVelRatio (0x13446b8): typed by tools/typed_lift.py (round A1)
inline uint32_t getFreeModeRevAngleVelRatio(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt)
{
    uint32_t v0 = 0;
    v0 = 0x1460000u;
    {
        v0 = v0 + 0x5624u;
        return v0;
    }
}

// GetGameTimeScaleRatio (0x1345d40): typed by tools/typed_lift.py (round A1)
inline a1::RetVF GetGameTimeScaleRatio(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt)
{
    uint32_t v0 = 0;
    float f0 = 0.0f;
    v0 = 0x1460000u;
    {
        f0 = a1_flt(A1_R32B(v0, 22508));
        return a1::RetVF{(uint64_t)a1_sx64(v0), f0};
    }
}

// GetCurrentGameAnimeTimeScaleRatio (0x1345e38): typed by tools/typed_lift.py (round A1)
inline a1::RetVF GetCurrentGameAnimeTimeScaleRatio(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt)
{
    uint32_t v0 = 0;
    float f0 = 0.0f;
    v0 = 0x1460000u;
    {
        f0 = a1_flt(A1_R32B(v0, 22512));
        return a1::RetVF{(uint64_t)a1_sx64(v0), f0};
    }
}

// GetGameAnimeTimeScaleRatio (0x1345e48): typed by tools/typed_lift.py (round A1)
inline a1::RetVF GetGameAnimeTimeScaleRatio(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt)
{
    uint32_t v0 = 0;
    float f0 = 0.0f;
    v0 = 0x1460000u;
    {
        f0 = a1_flt(A1_R32B(v0, 23012));
        return a1::RetVF{(uint64_t)a1_sx64(v0), f0};
    }
}

// GetPlayerControlButtonContext (0x1345e58): typed by tools/typed_lift.py (round A1)
inline uint32_t GetPlayerControlButtonContext(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt)
{
    uint32_t v0 = 0;
    v0 = 0x1460000u;
    {
        v0 = v0 + 0x5964u;
        return v0;
    }
}

// RestoreDefaultGameAnimeTimeScaleRatio (0x1345e78): typed by tools/typed_lift.py (round A1)
inline a1::RetVF RestoreDefaultGameAnimeTimeScaleRatio(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt)
{
    uint32_t at = 0;
    uint32_t v0 = 0;
    float f0 = 0.0f;
    at = 0x3F660000u;
    at = at | 0x6666u;
    f0 = a1_flt(at);
    v0 = 0x1460000u;
    {
        A1_W32B(v0, 23012, a1_bits(f0));
        return a1::RetVF{(uint64_t)a1_sx64(v0), f0};
    }
}

// SetDefaultGameAnimeTimeScaleRatio (0x1345e90): typed by tools/typed_lift.py (round A1)
inline uint32_t SetDefaultGameAnimeTimeScaleRatio(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, float f12)
{
    uint32_t v0 = 0;
    v0 = 0x1460000u;
    {
        A1_W32B(v0, 23012, a1_bits(f12));
        return v0;
    }
}

// getFrameRateMode (0x1345ed0): typed by tools/typed_lift.py (round A1)
inline uint32_t getFrameRateMode(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt)
{
    uint32_t v0 = 0;
    v0 = 0x1460000u;
    {
        v0 = v0 + 0x59E8u;
        return v0;
    }
}

// setDefaultPlayerControlButton (0x1346028): typed by tools/typed_lift.py (round A1)
inline uint64_t setDefaultPlayerControlButton(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint64_t a0)
{
    uint64_t v0 = 0;
    uint64_t v1 = 0;
    uint64_t a1 = 0;
    uint64_t a2 = 0;
    uint64_t a3 = 0;
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)a0, 16))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)326 << 16))));
    (a3 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v0, 22884))));
    { uint32_t addr = ADD32((uint32_t)v1, 7); uint32_t aligned_addr = addr & ~7u; uint32_t offset = addr & 7u; uint64_t mem = A1_R64(aligned_addr); uint32_t shift = (7u - offset) << 3; uint64_t keepMask = (shift == 0) ? 0ull : ((1ull << shift) - 1ull); (a0 = (uint64_t)((a0 & keepMask) | (mem << shift))); }
    { uint32_t addr = ADD32((uint32_t)v1, 0); uint32_t aligned_addr = addr & ~7u; uint32_t offset = addr & 7u; uint64_t mem = A1_R64(aligned_addr); uint32_t shift = offset << 3; uint64_t keepMask = (offset == 0) ? 0ull : (0xFFFFFFFFFFFFFFFFull << ((8u - offset) << 3)); (a0 = (uint64_t)((a0 & keepMask) | (mem >> shift))); }
    { uint32_t addr = ADD32((uint32_t)v1, 15); uint32_t aligned_addr = addr & ~7u; uint32_t offset = addr & 7u; uint64_t mem = A1_R64(aligned_addr); uint32_t shift = (7u - offset) << 3; uint64_t keepMask = (shift == 0) ? 0ull : ((1ull << shift) - 1ull); (a1 = (uint64_t)((a1 & keepMask) | (mem << shift))); }
    { uint32_t addr = ADD32((uint32_t)v1, 8); uint32_t aligned_addr = addr & ~7u; uint32_t offset = addr & 7u; uint64_t mem = A1_R64(aligned_addr); uint32_t shift = offset << 3; uint64_t keepMask = (offset == 0) ? 0ull : (0xFFFFFFFFFFFFFFFFull << ((8u - offset) << 3)); (a1 = (uint64_t)((a1 & keepMask) | (mem >> shift))); }
    { uint32_t addr = ADD32((uint32_t)v1, 23); uint32_t aligned_addr = addr & ~7u; uint32_t offset = addr & 7u; uint64_t mem = A1_R64(aligned_addr); uint32_t shift = (7u - offset) << 3; uint64_t keepMask = (shift == 0) ? 0ull : ((1ull << shift) - 1ull); (a2 = (uint64_t)((a2 & keepMask) | (mem << shift))); }
    { uint32_t addr = ADD32((uint32_t)v1, 16); uint32_t aligned_addr = addr & ~7u; uint32_t offset = addr & 7u; uint64_t mem = A1_R64(aligned_addr); uint32_t shift = offset << 3; uint64_t keepMask = (offset == 0) ? 0ull : (0xFFFFFFFFFFFFFFFFull << ((8u - offset) << 3)); (a2 = (uint64_t)((a2 & keepMask) | (mem >> shift))); }
    { uint32_t addr = ADD32((uint32_t)a3, 7); uint32_t aligned_addr = addr & ~7u; uint32_t offset = addr & 7u; uint32_t shift = (7u - offset) << 3; uint64_t mask = 0xFFFFFFFFFFFFFFFFull >> shift; uint64_t old_data = A1_R64(aligned_addr); uint64_t val = a0; uint64_t new_data = (old_data & ~mask) | ((val >> shift) & mask); A1_W64(aligned_addr, new_data); }
    { uint32_t addr = ADD32((uint32_t)a3, 0); uint32_t aligned_addr = addr & ~7u; uint32_t offset = addr & 7u; uint32_t shift = offset << 3; uint64_t mask = 0xFFFFFFFFFFFFFFFFull << shift; uint64_t old_data = A1_R64(aligned_addr); uint64_t val = a0; uint64_t new_data = (old_data & ~mask) | ((val << shift) & mask); A1_W64(aligned_addr, new_data); }
    { uint32_t addr = ADD32((uint32_t)a3, 15); uint32_t aligned_addr = addr & ~7u; uint32_t offset = addr & 7u; uint32_t shift = (7u - offset) << 3; uint64_t mask = 0xFFFFFFFFFFFFFFFFull >> shift; uint64_t old_data = A1_R64(aligned_addr); uint64_t val = a1; uint64_t new_data = (old_data & ~mask) | ((val >> shift) & mask); A1_W64(aligned_addr, new_data); }
    { uint32_t addr = ADD32((uint32_t)a3, 8); uint32_t aligned_addr = addr & ~7u; uint32_t offset = addr & 7u; uint32_t shift = offset << 3; uint64_t mask = 0xFFFFFFFFFFFFFFFFull << shift; uint64_t old_data = A1_R64(aligned_addr); uint64_t val = a1; uint64_t new_data = (old_data & ~mask) | ((val << shift) & mask); A1_W64(aligned_addr, new_data); }
    { uint32_t addr = ADD32((uint32_t)a3, 23); uint32_t aligned_addr = addr & ~7u; uint32_t offset = addr & 7u; uint32_t shift = (7u - offset) << 3; uint64_t mask = 0xFFFFFFFFFFFFFFFFull >> shift; uint64_t old_data = A1_R64(aligned_addr); uint64_t val = a2; uint64_t new_data = (old_data & ~mask) | ((val >> shift) & mask); A1_W64(aligned_addr, new_data); }
    { uint32_t addr = ADD32((uint32_t)a3, 16); uint32_t aligned_addr = addr & ~7u; uint32_t offset = addr & 7u; uint32_t shift = offset << 3; uint64_t mask = 0xFFFFFFFFFFFFFFFFull << shift; uint64_t old_data = A1_R64(aligned_addr); uint64_t val = a2; uint64_t new_data = (old_data & ~mask) | ((val << shift) & mask); A1_W64(aligned_addr, new_data); }
    { uint32_t addr = ADD32((uint32_t)v1, 31); uint32_t aligned_addr = addr & ~7u; uint32_t offset = addr & 7u; uint64_t mem = A1_R64(aligned_addr); uint32_t shift = (7u - offset) << 3; uint64_t keepMask = (shift == 0) ? 0ull : ((1ull << shift) - 1ull); (a0 = (uint64_t)((a0 & keepMask) | (mem << shift))); }
    { uint32_t addr = ADD32((uint32_t)v1, 24); uint32_t aligned_addr = addr & ~7u; uint32_t offset = addr & 7u; uint64_t mem = A1_R64(aligned_addr); uint32_t shift = offset << 3; uint64_t keepMask = (offset == 0) ? 0ull : (0xFFFFFFFFFFFFFFFFull << ((8u - offset) << 3)); (a0 = (uint64_t)((a0 & keepMask) | (mem >> shift))); }
    { uint32_t addr = ADD32((uint32_t)v1, 39); uint32_t aligned_addr = addr & ~7u; uint32_t offset = addr & 7u; uint64_t mem = A1_R64(aligned_addr); uint32_t shift = (7u - offset) << 3; uint64_t keepMask = (shift == 0) ? 0ull : ((1ull << shift) - 1ull); (a1 = (uint64_t)((a1 & keepMask) | (mem << shift))); }
    { uint32_t addr = ADD32((uint32_t)v1, 32); uint32_t aligned_addr = addr & ~7u; uint32_t offset = addr & 7u; uint64_t mem = A1_R64(aligned_addr); uint32_t shift = offset << 3; uint64_t keepMask = (offset == 0) ? 0ull : (0xFFFFFFFFFFFFFFFFull << ((8u - offset) << 3)); (a1 = (uint64_t)((a1 & keepMask) | (mem >> shift))); }
    { uint32_t addr = ADD32((uint32_t)a3, 31); uint32_t aligned_addr = addr & ~7u; uint32_t offset = addr & 7u; uint32_t shift = (7u - offset) << 3; uint64_t mask = 0xFFFFFFFFFFFFFFFFull >> shift; uint64_t old_data = A1_R64(aligned_addr); uint64_t val = a0; uint64_t new_data = (old_data & ~mask) | ((val >> shift) & mask); A1_W64(aligned_addr, new_data); }
    { uint32_t addr = ADD32((uint32_t)a3, 24); uint32_t aligned_addr = addr & ~7u; uint32_t offset = addr & 7u; uint32_t shift = offset << 3; uint64_t mask = 0xFFFFFFFFFFFFFFFFull << shift; uint64_t old_data = A1_R64(aligned_addr); uint64_t val = a0; uint64_t new_data = (old_data & ~mask) | ((val << shift) & mask); A1_W64(aligned_addr, new_data); }
    { uint32_t addr = ADD32((uint32_t)a3, 39); uint32_t aligned_addr = addr & ~7u; uint32_t offset = addr & 7u; uint32_t shift = (7u - offset) << 3; uint64_t mask = 0xFFFFFFFFFFFFFFFFull >> shift; uint64_t old_data = A1_R64(aligned_addr); uint64_t val = a1; uint64_t new_data = (old_data & ~mask) | ((val >> shift) & mask); A1_W64(aligned_addr, new_data); }
    { uint32_t addr = ADD32((uint32_t)a3, 32); uint32_t aligned_addr = addr & ~7u; uint32_t offset = addr & 7u; uint32_t shift = offset << 3; uint64_t mask = 0xFFFFFFFFFFFFFFFFull << shift; uint64_t old_data = A1_R64(aligned_addr); uint64_t val = a1; uint64_t new_data = (old_data & ~mask) | ((val << shift) & mask); A1_W64(aligned_addr, new_data); }
    {
        return v0;
    }
}

// getFadeValue (0x1346210): typed by tools/typed_lift.py (round A1)
inline a1::RetVF getFadeValue(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt)
{
    uint32_t v0 = 0;
    float f0 = 0.0f;
    v0 = 0x1460000u;
    {
        f0 = a1_flt(A1_R32B(v0, 23120));
        return a1::RetVF{(uint64_t)a1_sx64(v0), f0};
    }
}

// set_yaw_pitch_input_border_parameter_nico_yaw_pitch_cntrl (0x1346df0): typed by tools/typed_lift.py (round A1)
inline void set_yaw_pitch_input_border_parameter_nico_yaw_pitch_cntrl(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t a0, float f12, float f13, float f14, float f15)
{
    A1_W32B(a0, 120, a1_bits(f15));
    A1_W32B(a0, 108, a1_bits(f12));
    A1_W32B(a0, 112, a1_bits(f13));
    {
        A1_W32B(a0, 116, a1_bits(f14));
        
        return;
    }
}

// set_base_yaw_parameter_nico_yaw_pitch_cntrl (0x1346fe8): typed by tools/typed_lift.py (round A1)
inline float set_base_yaw_parameter_nico_yaw_pitch_cntrl(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t a0, uint32_t a1, float f12, float f13, float f14, float f15, float f16, float f17)
{
    uint32_t at = 0;
    float f0 = 0.0f;
    at = 0x3C8E0000u;
    at = at | 0xFA35u;
    f0 = a1_flt(at);
    A1_W32B(a0, 228, a1_bits(f17));
    f16 = a1f::mul(f16, f0);
    A1_W32B(a0, 204, a1);
    f12 = a1f::mul(f12, f0);
    f13 = a1f::mul(f13, f0);
    f14 = a1f::mul(f14, f0);
    f15 = a1f::mul(f15, f0);
    A1_W32B(a0, 224, a1_bits(f16));
    A1_W32B(a0, 208, a1_bits(f12));
    A1_W32B(a0, 212, a1_bits(f13));
    A1_W32B(a0, 216, a1_bits(f14));
    {
        A1_W32B(a0, 220, a1_bits(f15));
        return f0;
    }
}

// set_yaw_parameter_effective_ratio_nico_yaw_pitch_cntrl (0x1347358): typed by tools/typed_lift.py (round A1)
inline void set_yaw_parameter_effective_ratio_nico_yaw_pitch_cntrl(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t a0, float f12)
{
    {
        A1_W32B(a0, 312, a1_bits(f12));
        
        return;
    }
}

// set_pitch_parameter_effective_ratio_nico_yaw_pitch_cntrl (0x1347360): typed by tools/typed_lift.py (round A1)
inline void set_pitch_parameter_effective_ratio_nico_yaw_pitch_cntrl(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t a0, float f12)
{
    {
        A1_W32B(a0, 316, a1_bits(f12));
        
        return;
    }
}

// set_temp_stop_base_rot_flag_nico_yaw_pitch_cntrl (0x1347598): typed by tools/typed_lift.py (round A1)
inline void set_temp_stop_base_rot_flag_nico_yaw_pitch_cntrl(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t a0, uint32_t a1)
{
    {
        A1_W32B(a0, 48, a1);
        
        return;
    }
}

// is_enable_base_yaw_rot_nico_yaw_pitch_cntrl (0x1347838): typed by tools/typed_lift.py (round A1)
inline uint32_t is_enable_base_yaw_rot_nico_yaw_pitch_cntrl(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t a0)
{
    uint32_t v0 = 0;
    {
        v0 = A1_R32B(a0, 204);
        return v0;
    }
}

// is_enable_base_pitch_rot_nico_yaw_pitch_cntrl (0x1347840): typed by tools/typed_lift.py (round A1)
inline uint32_t is_enable_base_pitch_rot_nico_yaw_pitch_cntrl(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t a0)
{
    uint32_t v0 = 0;
    {
        v0 = A1_R32B(a0, 232);
        return v0;
    }
}

// get_base_yaw_move_nico_yaw_pitch_cntrl (0x1347848): typed by tools/typed_lift.py (round A1)
inline float get_base_yaw_move_nico_yaw_pitch_cntrl(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t a0)
{
    float f0 = 0.0f;
    {
        f0 = a1_flt(A1_R32B(a0, 88));
        return f0;
    }
}

// set_base_yaw_nico_yaw_pitch_cntrl (0x1347858): typed by tools/typed_lift.py (round A1)
inline void set_base_yaw_nico_yaw_pitch_cntrl(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t a0, float f12)
{
    {
        A1_W32B(a0, 92, a1_bits(f12));
        
        return;
    }
}

// set_base_pitch_nico_yaw_pitch_cntrl (0x1347860): typed by tools/typed_lift.py (round A1)
inline void set_base_pitch_nico_yaw_pitch_cntrl(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t a0, float f12)
{
    {
        A1_W32B(a0, 104, a1_bits(f12));
        
        return;
    }
}

// get_base_yaw_nico_yaw_pitch_cntrl (0x1347868): typed by tools/typed_lift.py (round A1)
inline float get_base_yaw_nico_yaw_pitch_cntrl(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t a0)
{
    float f0 = 0.0f;
    {
        f0 = a1_flt(A1_R32B(a0, 92));
        return f0;
    }
}

// get_base_pitch_nico_yaw_pitch_cntrl (0x1347870): typed by tools/typed_lift.py (round A1)
inline float get_base_pitch_nico_yaw_pitch_cntrl(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t a0)
{
    float f0 = 0.0f;
    {
        f0 = a1_flt(A1_R32B(a0, 104));
        return f0;
    }
}

// backStageGetBossSaveArea (0x13494a8): typed by tools/typed_lift.py (round A1)
inline uint32_t backStageGetBossSaveArea(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt)
{
    uint32_t v0 = 0;
    v0 = 0x11E0000u;
    {
        v0 = v0 - 0xB80u;
        return v0;
    }
}

// backStageInit (0x13494b8): typed by tools/typed_lift.py (round A1)
inline uint32_t backStageInit(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt)
{
    uint32_t v0 = 0;
    uint32_t v1 = 0;
    v0 = 0x11E0000u;
    v1 = 0xFFFFFFFFu;
    v0 = v0 - 0xBC8u;
    A1_W32B(v0, 72, v1);
    A1_W32(v0, v1);
    {
        A1_W32B(v0, 36, v1);
        return v0;
    }
}

// bossStatGetDeadBoss (0x134a358): typed by tools/typed_lift.py (round A1)
inline uint32_t bossStatGetDeadBoss(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt)
{
    uint32_t v0 = 0;
    uint32_t v1 = 0;
    v1 = 0x1460000u;
    {
        v0 = A1_R32B(v1, 23256);
        return v0;
    }
}

// CheckCamFinalizerReady (0x134a778): typed by tools/typed_lift.py (round A1)
inline uint32_t CheckCamFinalizerReady(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt)
{
    uint32_t v0 = 0;
    uint32_t v1 = 0;
    v1 = 0x1460000u;
    {
        v0 = A1_R32B(v1, 23264);
        return v0;
    }
}

// SetCamFinalizerReady (0x134a788): typed by tools/typed_lift.py (round A1)
inline uint32_t SetCamFinalizerReady(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t a0)
{
    uint32_t v0 = 0;
    v0 = 0x1460000u;
    {
        A1_W32B(v0, 23264, a0);
        return v0;
    }
}

// PauseCamFinalizer (0x134a7c0): typed by tools/typed_lift.py (round A1)
inline uint32_t PauseCamFinalizer(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t a0)
{
    uint32_t v0 = 0;
    v0 = 0x1460000u;
    {
        A1_W32B(v0, 23328, a0);
        return v0;
    }
}

// CheckDemoCameraOrientFinished (0x134b260): typed by tools/typed_lift.py (round A1)
inline uint32_t CheckDemoCameraOrientFinished(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t a0)
{
    uint32_t v0 = 0;
    {
        v0 = A1_R32B(a0, 56);
        return v0;
    }
}

// getDemoEnableFlag (0x134b268): typed by tools/typed_lift.py (round A1)
inline uint32_t getDemoEnableFlag(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt)
{
    uint32_t v0 = 0;
    v0 = 0x1460000u;
    {
        v0 = v0 + 0x5B24u;
        return v0;
    }
}

// demoscrCheckDemoFinish (0x134c838): typed by tools/typed_lift.py (round A1)
inline uint32_t demoscrCheckDemoFinish(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt)
{
    uint32_t v0 = 0;
    uint32_t v1 = 0;
    { const uint32_t _dynInsn = A1_R32(0x134C838u); const int32_t _dynSimm = (int16_t)(_dynInsn & 0xFFFFu); const uint32_t _dynImm = _dynInsn & 0xFFFFu; (void)_dynSimm; (void)_dynImm; (v1 = (uint32_t)((int32_t)((int32_t)((uint32_t)_dynImm << 16)))); }
    { const uint32_t _dynInsn = A1_R32(0x134C83Cu); const int32_t _dynSimm = (int16_t)(_dynInsn & 0xFFFFu); const uint32_t _dynImm = _dynInsn & 0xFFFFu; (void)_dynSimm; (void)_dynImm; (v0 = (uint32_t)((int32_t)((int32_t)A1_R32(ADD32(v1, _dynSimm))))); }
    v0 = v0 & 0x800u;
    {
        v0 = 0u < v0 ? 1u : 0u;
        return v0;
    }
}

// demoscrCheckDemoCutSkip (0x134c850): typed by tools/typed_lift.py (round A1)
inline uint64_t demoscrCheckDemoCutSkip(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt)
{
    uint64_t v0 = 0;
    {
        (v0 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL));
        return v0;
    }
}

// endingflowTickAdpcmSeq (0x134d728): typed by tools/typed_lift.py (round A1)
inline uint32_t endingflowTickAdpcmSeq(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t a0)
{
    uint32_t v0 = 0;
    v0 = 0x11E0000u;
    {
        A1_W32(v0 - 0x8BCu, a0);
        return v0;
    }
}

// gamecore_sub_0134D920 (0x134d920): typed by tools/typed_lift.py (round A1)
inline uint32_t gamecore_sub_0134D920(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t t0)
{
    uint32_t v0 = 0;
    uint32_t v1 = 0;
    v0 = 0x10000u;
    v1 = A1_R32B(t0, 20);
    v0 = v0 | 0x869Eu;
    v0 = (int32_t)v0 < (int32_t)v1 ? 1u : 0u;
    if (v0 == 0) {
        v0 = A1_R32B(t0, 4);
        v0 = v0 + 1u;
        A1_W32B(t0, 4, v0);
    }
    {
        return v0;
    }
}

// initFadeParam (0x134db40): typed by tools/typed_lift.py (round A1)
inline a1::RetVF initFadeParam(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t a0, float f12)
{
    uint32_t v0 = 0;
    float f0 = 0.0f;
    float f1 = 0.0f;
    v0 = 0x1460000u;
    v0 = v0 + 0x5650u;
    f0 = a1_flt(A1_R32B(v0, 360));
    A1_W32(a0, a1_bits(f0));
    f1 = a1_flt(A1_R32B(v0, 360));
    A1_W32B(a0, 12, a1_bits(f12));
    A1_W32B(a0, 4, a1_bits(f1));
    A1_W32B(a0, 8, a1_bits(f12));
    {
        A1_W32B(a0, 16, a1_bits(f12));
        return a1::RetVF{(uint64_t)a1_sx64(v0), f0};
    }
}

// setFadeParam (0x134db68): typed by tools/typed_lift.py (round A1)
inline a1::RetVF setFadeParam(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t a0, uint32_t a1, float f12, float f13)
{
    uint32_t v0 = 0;
    float f0 = 0.0f;
    float f1 = 0.0f;
    float f2 = 0.0f;
    v0 = 0x1460000u;
    f2 = a1_flt(A1_R32B(a0, 16));
    v0 = v0 + 0x5650u;
    f0 = a1_flt(A1_R32B(v0, 360));
    A1_W32(a0, a1_bits(f0));
    f1 = a1_flt(A1_R32B(v0, 360));
    A1_W32B(a0, 20, a1);
    f1 = a1f::add(f1, f13);
    A1_W32B(a0, 8, a1_bits(f2));
    A1_W32B(a0, 12, a1_bits(f12));
    {
        A1_W32B(a0, 4, a1_bits(f1));
        return a1::RetVF{(uint64_t)a1_sx64(v0), f0};
    }
}

// calcFadeParam (0x134db98): typed by tools/typed_lift.py (round A1)
inline a1::RetVF calcFadeParam(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t a0)
{
    uint32_t v0 = 0;
    float f0 = 0.0f;
    float f1 = 0.0f;
    float f2 = 0.0f;
    float f3 = 0.0f;
    float f4 = 0.0f;
    uint32_t fcr31 = 0;
    v0 = 0x1460000u;
    f4 = a1_flt(A1_R32B(a0, 4));
    f3 = a1_flt(A1_R32B(v0, 22456));
    fcr31 = (a1f::le(f3, f4)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
    {
        const bool c = (fcr31 & 0x800000u) == 0u;
        f0 = a1_flt(A1_R32B(a0, 12));
        if (!c) {
            f1 = a1_flt(A1_R32(a0));
            f2 = a1_flt(A1_R32B(a0, 8));
            f3 = a1f::sub(f3, f1);
            f1 = a1f::sub(f4, f1);
            f0 = a1f::sub(f0, f2);
            f3 = a1f::div(fcr31, f3, f1);
            f0 = ((float)(f0) * (float)(f3));
            f2 = a1f::add(f2, f0);
            {
                A1_W32B(a0, 16, a1_bits(f2));
                return a1::RetVF{(uint64_t)a1_sx64(v0), f0};
            }
        }
    }
    {
        A1_W32B(a0, 16, a1_bits(f0));
        return a1::RetVF{(uint64_t)a1_sx64(v0), f0};
    }
}

// checkFadeParam (0x134dbe8): typed by tools/typed_lift.py (round A1)
inline a1::RetVF checkFadeParam(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t a0)
{
    uint32_t v0 = 0;
    float f0 = 0.0f;
    float f1 = 0.0f;
    uint32_t fcr31 = 0;
    v0 = 0x1460000u;
    f1 = a1_flt(A1_R32B(a0, 4));
    f0 = a1_flt(A1_R32B(v0, 22456));
    fcr31 = (a1f::le(f0, f1)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
    {
        const bool c = (fcr31 & 0x800000u) != 0u;
        v0 = 1u;
        if (!c) {
            v0 = 0u;
        }
    }
    {
        return a1::RetVF{(uint64_t)a1_sx64(v0), f0};
    }
}

// fadeChkStat (0x134dc20): typed by tools/typed_lift.py (round A1)
inline uint64_t fadeChkStat(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint64_t a0)
{
    uint32_t v1 = 0;
    uint64_t v0 = 0;
    v1 = 0x1460000u;
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B(v1, 23396))));
    (v0 = (uint64_t)(RC_U64X(v0) & a0));
    {
        (v0 = (uint64_t)(((uint64_t)0ULL < (uint64_t)v0) ? 1 : 0));
        return v0;
    }
}

// setTreeId (0x134e390): typed by tools/typed_lift.py (round A1)
inline uint32_t setTreeId(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t a0)
{
    uint32_t v0 = 0;
    uint32_t v1 = 0;
    v0 = 0x1460000u;
    v1 = 0x1460000u;
    A1_W32B(v0, 23400, a0);
    {
        A1_W32B(v1, 23404, 0u);
        return v0;
    }
}

// _optionWorkChk (0x134ef80): typed by tools/typed_lift.py (round A1)
inline uint32_t _optionWorkChk(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t a0)
{
    uint32_t v0 = 0;
    a0 = a0 << 2;
    v0 = 0x11E0000u;
    v0 = v0 + a0;
    v0 = A1_R32(v0 - 0x8B8u);
    {
        return v0;
    }
}

// _optionWorkPtr (0x134ef98): typed by tools/typed_lift.py (round A1)
inline uint32_t _optionWorkPtr(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t a0)
{
    uint32_t at = 0;
    uint32_t v0 = 0;
    v0 = a0 << 2;
    at = 0x11E0000u;
    at = at - 0x8B8u;
    v0 = at + v0;
    {
        return v0;
    }
}

// _gameIWorkSet (0x134f0d8): typed by tools/typed_lift.py (round A1)
inline uint32_t _gameIWorkSet(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t a0, uint32_t a1)
{
    uint32_t v0 = 0;
    uint32_t v1 = 0;
    v0 = 0x1460000u;
    a0 = a0 << 2;
    v1 = A1_R32B(v0, 23412);
    a0 = a0 + v1;
    {
        A1_W32(a0, a1);
        return v0;
    }
}

// _gameWorkChk (0x134f108): typed by tools/typed_lift.py (round A1)
inline uint32_t _gameWorkChk(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t a0)
{
    uint32_t v0 = 0;
    a0 = a0 << 2;
    v0 = 0x11E0000u;
    v0 = v0 + a0;
    v0 = A1_R32(v0 - 0x7E0u);
    {
        return v0;
    }
}

// _gameIWorkChk (0x134f138): typed by tools/typed_lift.py (round A1)
inline uint32_t _gameIWorkChk(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t a0)
{
    uint32_t v0 = 0;
    uint32_t v1 = 0;
    v0 = 0x1460000u;
    a0 = a0 << 2;
    v1 = A1_R32B(v0, 23412);
    a0 = a0 + v1;
    {
        v0 = A1_R32(a0);
        return v0;
    }
}

// setBossClearCnt (0x134f238): typed by tools/typed_lift.py (round A1)
inline uint32_t setBossClearCnt(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t a0)
{
    uint32_t v0 = 0;
    v0 = 0x11E0000u;
    {
        A1_W32B(v0, 112, a0);
        return v0;
    }
}

// getGameClearCnt (0x134f278): typed by tools/typed_lift.py (round A1)
inline uint32_t getGameClearCnt(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt)
{
    uint32_t v0 = 0;
    uint32_t v1 = 0;
    v1 = 0x11E0000u;
    {
        v0 = A1_R32B(v1, 116);
        return v0;
    }
}

// getGameHardMode (0x134f2a8): typed by tools/typed_lift.py (round A1)
inline uint32_t getGameHardMode(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt)
{
    uint32_t v0 = 0;
    uint32_t v1 = 0;
    v1 = 0x11E0000u;
    v0 = A1_R32B(v1, 124);
    {
        v0 = 0u < v0 ? 1u : 0u;
        return v0;
    }
}

// setSavedClock (0x134f320): typed by tools/typed_lift.py (round A1)
inline uint32_t setSavedClock(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t a0)
{
    uint32_t v0 = 0;
    uint64_t v1 = 0;
    (v1 = (uint64_t)(A1_R64B(a0, 0)));
    v0 = 0x11E0000u;
    {
        A1_W64B(v0, 128, v1);
        return v0;
    }
}

// getSavedPlaceId (0x134f330): typed by tools/typed_lift.py (round A1)
inline uint32_t getSavedPlaceId(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt)
{
    uint32_t v0 = 0;
    uint32_t v1 = 0;
    v1 = 0x11E0000u;
    {
        v0 = (uint32_t)(int8_t)A1_R8B(v1, 88);
        return v0;
    }
}

// getOptionDataAdrsAndSize (0x134fe00): typed by tools/typed_lift.py (round A1)
inline uint32_t getOptionDataAdrsAndSize(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t a0)
{
    uint32_t v0 = 0;
    uint32_t v1 = 0;
    v1 = 0x88u;
    v0 = 0x11E0000u;
    A1_W32(a0, v1);
    {
        v0 = v0 - 0x8B8u;
        return v0;
    }
}

// gameflowSelectGameoverMenu (0x13521e0): typed by tools/typed_lift.py (round A1)
inline uint32_t gameflowSelectGameoverMenu(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t a0)
{
    uint32_t v0 = 0;
    v0 = 0x11E0000u;
    {
        A1_W32B(v0, 340, a0);
        return v0;
    }
}

// gameflowSetPauseEnable (0x1352418): typed by tools/typed_lift.py (round A1)
inline uint32_t gameflowSetPauseEnable(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint64_t a0)
{
    uint32_t v0 = 0;
    uint32_t v1 = 0;
    v0 = 0x1460000u;
    {
        const bool c = (int64_t)a0 != 0;
        v1 = v0 + 0x5BC4u;
        if (!c) {
            v0 = A1_R32(v1);
            v0 = v0 + 1u;
            goto L_1352438;
        }
    }
    v0 = A1_R32(v1);
    v0 = v0 - 1u;
L_1352438:
    {
        A1_W32(v1, v0);
        return v0;
    }
}

// gameflowGameOver (0x13524a8): typed by tools/typed_lift.py (round A1)
inline uint32_t gameflowGameOver(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt)
{
    uint32_t v0 = 0;
    uint32_t v1 = 0;
    v1 = 0x11E0000u;
    v0 = 1u;
    {
        A1_W32B(v1, 336, v0);
        return v0;
    }
}

// gameflowGetJumpSekiban (0x13524b8): typed by tools/typed_lift.py (round A1)
inline uint32_t gameflowGetJumpSekiban(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt)
{
    uint32_t v0 = 0;
    uint32_t v1 = 0;
    v1 = 0x1460000u;
    {
        v0 = A1_R32B(v1, 23488);
        return v0;
    }
}

// gameflowSetJumpSekiban (0x13524c8): typed by tools/typed_lift.py (round A1)
inline uint32_t gameflowSetJumpSekiban(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t a0)
{
    uint32_t v0 = 0;
    v0 = 0x1460000u;
    {
        A1_W32B(v0, 23488, a0);
        return v0;
    }
}

// gamecore_sub_01352770 (0x1352770): typed by tools/typed_lift.py (round A1)
inline uint32_t gamecore_sub_01352770(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt)
{
    uint32_t v0 = 0;
    uint32_t v1 = 0;
    v1 = 0x11E0000u;
    v0 = 5u;
    {
        A1_W32B(v1, 336, v0);
        return v0;
    }
}

// gStatGetOmakeStatAdrs (0x1353570): typed by tools/typed_lift.py (round A1)
inline uint32_t gStatGetOmakeStatAdrs(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt)
{
    uint32_t v0 = 0;
    v0 = 0x11E0000u;
    {
        v0 = v0 + 0x3DB0u;
        return v0;
    }
}

// gStatGObjDispChk (0x13535d8): typed by tools/typed_lift.py (round A1)
inline uint32_t gStatGObjDispChk(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t a0)
{
    uint32_t v0 = 0;
    v0 = A1_R32B(a0, 16);
    {
        v0 = v0 & 1u;
        return v0;
    }
}

// gStatSetGObjAdrs (0x13536d0): typed by tools/typed_lift.py (round A1)
inline void gStatSetGObjAdrs(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t a0, uint32_t a1)
{
    {
        A1_W32B(a0, 12, a1);
        
        return;
    }
}

// gStatGetPlayerAccessoriesDamgeRaitoToPlayer (0x1353730): typed by tools/typed_lift.py (round A1)
inline a1::RetVF gStatGetPlayerAccessoriesDamgeRaitoToPlayer(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt)
{
    uint32_t at = 0;
    uint32_t v0 = 0;
    float f0 = 0.0f;
    float f1 = 0.0f;
    v0 = 0x1460000u;
    at = 0x3F800000u;
    f0 = a1_flt(at);
    v0 = A1_R32B(v0, 24012);
    if (v0 != 0) {
        f0 = a1_flt(A1_R32B(v0, 8));
    } else {
    }
    v0 = 0x1460000u;
    v0 = A1_R32B(v0, 24016);
    {
        const bool c = v0 == 0;
        f1 = ((float)(f0));
        if (!c) {
            f0 = a1_flt(A1_R32B(v0, 8));
            f1 = a1f::mul(f1, f0);
        }
    }
    {
        f0 = ((float)(f1));
        return a1::RetVF{(uint64_t)a1_sx64(v0), f0};
    }
}

// gStatGetPlayerAccessoriesDamgeRaitoToBoss (0x1353768): typed by tools/typed_lift.py (round A1)
inline a1::RetVF gStatGetPlayerAccessoriesDamgeRaitoToBoss(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt)
{
    uint32_t at = 0;
    uint32_t v0 = 0;
    float f0 = 0.0f;
    float f1 = 0.0f;
    v0 = 0x1460000u;
    at = 0x3F800000u;
    f0 = a1_flt(at);
    v0 = A1_R32B(v0, 24012);
    if (v0 != 0) {
        f0 = a1_flt(A1_R32B(v0, 12));
    } else {
    }
    v0 = 0x1460000u;
    v0 = A1_R32B(v0, 24016);
    {
        const bool c = v0 == 0;
        f1 = ((float)(f0));
        if (!c) {
            f0 = a1_flt(A1_R32B(v0, 12));
            f1 = a1f::mul(f1, f0);
        }
    }
    {
        f0 = ((float)(f1));
        return a1::RetVF{(uint64_t)a1_sx64(v0), f0};
    }
}

// gstatBootMcSetExistNicoData (0x1353e00): typed by tools/typed_lift.py (round A1)
inline uint32_t gstatBootMcSetExistNicoData(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t a0, uint32_t a1)
{
    uint32_t v0 = 0;
    uint32_t v1 = 0;
    v0 = 0x1460000u;
    v1 = 0x1460000u;
    A1_W32B(v0, 24024, a0);
    {
        A1_W32B(v1, 24028, a1);
        return v0;
    }
}

// gstatBootMcSetExistIcoData (0x1353ef8): typed by tools/typed_lift.py (round A1)
inline uint32_t gstatBootMcSetExistIcoData(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t a0)
{
    uint32_t v0 = 0;
    v0 = 0x1460000u;
    {
        A1_W32B(v0, 24032, a0);
        return v0;
    }
}

// gstatBootMcChkExistIcoData (0x1353f08): typed by tools/typed_lift.py (round A1)
inline uint32_t gstatBootMcChkExistIcoData(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt)
{
    uint32_t v0 = 0;
    uint32_t v1 = 0;
    v1 = 0x1460000u;
    {
        v0 = A1_R32B(v1, 24032);
        return v0;
    }
}

// jumphokoraflowDemoStart (0x13549a0): typed by tools/typed_lift.py (round A1)
inline uint32_t jumphokoraflowDemoStart(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt)
{
    uint32_t v0 = 0;
    uint32_t v1 = 0;
    v1 = 0x1460000u;
    v0 = A1_R32B(v1, 24048);
    {
        v0 = v0 & 1u;
        return v0;
    }
}

// jumphokoraflowDemoEnd (0x13549b0): typed by tools/typed_lift.py (round A1)
inline uint32_t jumphokoraflowDemoEnd(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t a0)
{
    uint32_t v0 = 0;
    uint32_t v1 = 0;
    v1 = 0x1460000u;
    v1 = v1 + 0x5DF0u;
    v0 = A1_R32(v1);
    A1_W32B(v1, 4, a0);
    v0 = v0 | 2u;
    {
        A1_W32(v1, v0);
        return v0;
    }
}

// jumphokoraGetDemoInfo (0x13549d0): typed by tools/typed_lift.py (round A1)
inline uint32_t jumphokoraGetDemoInfo(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t a0, uint32_t a1)
{
    uint32_t v0 = 0;
    uint32_t v1 = 0;
    uint32_t a2 = 0;
    v1 = 0x11E0000u;
    a2 = 0x11E0000u;
    v0 = A1_R32B(v1, 16032);
    A1_W32(a0, v0);
    v1 = A1_R32B(a2, 16036);
    {
        A1_W32(a1, v1);
        return v0;
    }
}

// gcCheckStageLoadFinish (0x1356570): typed by tools/typed_lift.py (round A1)
inline uint32_t gcCheckStageLoadFinish(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt)
{
    uint32_t v0 = 0;
    uint32_t v1 = 0;
    v1 = 0x1460000u;
    v0 = A1_R32B(v1, 24068);
    v0 = ~(0u | v0);
    {
        v0 = v0 < 1u ? 1u : 0u;
        return v0;
    }
}

// opflowOp1End (0x1357648): typed by tools/typed_lift.py (round A1)
inline uint32_t opflowOp1End(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt)
{
    uint32_t v0 = 0;
    uint32_t v1 = 0;
    v1 = 0x1460000u;
    v1 = v1 + 0x5E40u;
    v0 = A1_R32(v1);
    v0 = v0 | 0x11u;
    {
        A1_W32(v1, v0);
        return v0;
    }
}

// opflowOp2Start (0x1357660): typed by tools/typed_lift.py (round A1)
inline uint32_t opflowOp2Start(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt)
{
    uint32_t v0 = 0;
    uint32_t v1 = 0;
    v1 = 0x1460000u;
    v1 = v1 + 0x5E40u;
    v0 = A1_R32(v1);
    v0 = v0 | 2u;
    {
        A1_W32(v1, v0);
        return v0;
    }
}

// opflowOp2End (0x1357678): typed by tools/typed_lift.py (round A1)
inline uint32_t opflowOp2End(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t a0)
{
    uint32_t v0 = 0;
    uint32_t v1 = 0;
    uint32_t a1 = 0;
    v1 = 0x1460000u;
    a0 = a0 & 1u;
    v1 = v1 + 0x5E40u;
    a1 = 0xFFFFFFF7u;
    v0 = A1_R32(v1);
    a0 = a0 << 3;
    v0 = v0 | 4u;
    v0 = v0 & a1;
    v0 = v0 | a0;
    {
        A1_W32(v1, v0);
        return v0;
    }
}

// opflowChkTitleDemoStart (0x13576a8): typed by tools/typed_lift.py (round A1)
inline uint32_t opflowChkTitleDemoStart(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt)
{
    uint32_t v0 = 0;
    uint32_t v1 = 0;
    v1 = 0x1460000u;
    v0 = A1_R32B(v1, 24128);
    v0 = v0 >> 4;
    {
        v0 = v0 & 1u;
        return v0;
    }
}

// opflowTitleDmoeEnd (0x13576c0): typed by tools/typed_lift.py (round A1)
inline uint32_t opflowTitleDmoeEnd(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt)
{
    uint32_t v0 = 0;
    uint32_t v1 = 0;
    v1 = 0x1460000u;
    v1 = v1 + 0x5E40u;
    v0 = A1_R32(v1);
    v0 = v0 | 0x20u;
    {
        A1_W32(v1, v0);
        return v0;
    }
}

// opflowChkAbort (0x13576f8): typed by tools/typed_lift.py (round A1)
inline uint32_t opflowChkAbort(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt)
{
    uint32_t v0 = 0;
    uint32_t v1 = 0;
    v1 = 0x11E0000u;
    v0 = A1_R32B(v1, 16080);
    {
        v0 = 0u < v0 ? 1u : 0u;
        return v0;
    }
}

// opflowChkLoadStart (0x1357708): typed by tools/typed_lift.py (round A1)
inline uint32_t opflowChkLoadStart(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt)
{
    uint32_t v0 = 0;
    uint32_t v1 = 0;
    v1 = 0x11E0000u;
    {
        v0 = A1_R32B(v1, 16080);
        return v0;
    }
}

// getPlayerHp (0x1358248): typed by tools/typed_lift.py (round A1)
inline a1::RetVF getPlayerHp(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt)
{
    uint32_t v0 = 0;
    float f0 = 0.0f;
    v0 = 0x11E0000u;
    {
        f0 = a1_flt(A1_R32B(v0, 16100));
        return a1::RetVF{(uint64_t)a1_sx64(v0), f0};
    }
}

// getPlayerGrip (0x13584e8): typed by tools/typed_lift.py (round A1)
inline a1::RetVF getPlayerGrip(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt)
{
    uint32_t v0 = 0;
    float f0 = 0.0f;
    v0 = 0x11E0000u;
    {
        f0 = a1_flt(A1_R32B(v0, 16096));
        return a1::RetVF{(uint64_t)a1_sx64(v0), f0};
    }
}

// initPowerWork (0x1358778): typed by tools/typed_lift.py (round A1)
inline uint32_t initPowerWork(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt)
{
    uint32_t v0 = 0;
    v0 = 0x11E0000u;
    {
        A1_W32B(v0, 16104, 0u);
        return v0;
    }
}

// resetAttackPower (0x1358788): typed by tools/typed_lift.py (round A1)
inline uint32_t resetAttackPower(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt)
{
    uint32_t v0 = 0;
    v0 = 0x11E0000u;
    {
        A1_W32B(v0, 16104, 0u);
        return v0;
    }
}

// getAttackPowerRatio (0x1358798): typed by tools/typed_lift.py (round A1)
inline a1::RetVF getAttackPowerRatio(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt)
{
    uint32_t at = 0;
    uint32_t v0 = 0;
    float f0 = 0.0f;
    float f1 = 0.0f;
    v0 = 0x11E0000u;
    at = 0x3C230000u;
    at = at | 0xD70Au;
    f1 = a1_flt(at);
    f0 = a1_flt(A1_R32B(v0, 16104));
    {
        f0 = a1f::mul(f0, f1);
        return a1::RetVF{(uint64_t)a1_sx64(v0), f0};
    }
}

// setFullPower (0x1358840): typed by tools/typed_lift.py (round A1)
inline a1::RetVF setFullPower(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt)
{
    uint32_t at = 0;
    uint32_t v0 = 0;
    float f0 = 0.0f;
    at = 0x42C80000u;
    f0 = a1_flt(at);
    v0 = 0x11E0000u;
    {
        A1_W32B(v0, 16104, a1_bits(f0));
        return a1::RetVF{(uint64_t)a1_sx64(v0), f0};
    }
}

// gamecore_sub_01358878 (0x1358878): typed by tools/typed_lift.py (round A1)
inline uint32_t gamecore_sub_01358878(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt)
{
    uint32_t v0 = 0;
    v0 = 0x11E0000u;
    {
        A1_W32B(v0, 16104, 0u);
        return v0;
    }
}

// getAreaSize (0x135d740): typed by tools/typed_lift.py (round A1)
inline a1::RetVF getAreaSize(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt)
{
    uint32_t v0 = 0;
    float f0 = 0.0f;
    float f1 = 0.0f;
    v0 = 0x1460000u;
    v0 = v0 + 0x5E80u;
    f1 = a1_flt(A1_R32B(v0, 56));
    { int32_t tmp; std::memcpy(&tmp, &f1, sizeof(tmp)); f1 = ((float)(int32_t)(tmp)); }
    f0 = a1_flt(A1_R32B(v0, 64));
    {
        f0 = a1f::mul(f0, f1);
        return a1::RetVF{(uint64_t)a1_sx64(v0), f0};
    }
}

// getPosOnMap (0x135d9c0): typed by tools/typed_lift.py (round A1)
inline a1::RetVF getPosOnMap(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t a0, uint32_t a1, uint32_t a2)
{
    uint32_t v0 = 0;
    float f0 = 0.0f;
    float f1 = 0.0f;
    float f2 = 0.0f;
    uint32_t fcr31 = 0;
    f1 = a1_flt(A1_R32(a0));
    v0 = 0x1460000u;
    v0 = v0 + 0x5E80u;
    f0 = a1_flt(A1_R32B(v0, 40));
    { int32_t tmp; std::memcpy(&tmp, &f0, sizeof(tmp)); f0 = ((float)(int32_t)(tmp)); }
    f2 = a1_flt(A1_R32B(v0, 80));
    f1 = (-(float)(f1));
    f1 = a1f::mul(f1, f0);
    f1 = a1f::div(fcr31, f1, f2);
    A1_W32(a1, a1_bits(f1));
    f0 = a1_flt(A1_R32B(a0, 8));
    f1 = a1_flt(A1_R32B(v0, 44));
    { int32_t tmp; std::memcpy(&tmp, &f1, sizeof(tmp)); f1 = ((float)(int32_t)(tmp)); }
    f2 = a1_flt(A1_R32B(v0, 84));
    f0 = (-(float)(f0));
    f0 = a1f::mul(f0, f1);
    f0 = a1f::div(fcr31, f0, f2);
    {
        A1_W32(a2, a1_bits(f0));
        return a1::RetVF{(uint64_t)a1_sx64(v0), f0};
    }
}

// getSeamlessStageHighModelDispDistDefault (0x135dae8): typed by tools/typed_lift.py (round A1)
inline uint32_t getSeamlessStageHighModelDispDistDefault(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt)
{
    uint32_t v0 = 0;
    v0 = 0x1460000u;
    {
        v0 = v0 + 0x5E64u;
        return v0;
    }
}

// getSeamlessStageMiddleModelDispDistDefault (0x135daf8): typed by tools/typed_lift.py (round A1)
inline uint32_t getSeamlessStageMiddleModelDispDistDefault(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt)
{
    uint32_t v0 = 0;
    v0 = 0x1460000u;
    {
        v0 = v0 + 0x5E68u;
        return v0;
    }
}

// getSeamlessStageEnv (0x135dd58): typed by tools/typed_lift.py (round A1)
inline uint32_t getSeamlessStageEnv(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt)
{
    uint32_t v0 = 0;
    v0 = 0x1460000u;
    {
        v0 = v0 + 0x5E80u;
        return v0;
    }
}

// forceLoadSeamlessStage (0x135df88): typed by tools/typed_lift.py (round A1)
inline uint32_t forceLoadSeamlessStage(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt)
{
    uint32_t v0 = 0;
    uint32_t v1 = 0;
    v1 = 0x1460000u;
    v0 = 1u;
    {
        A1_W32B(v1, 24172, v0);
        return v0;
    }
}

// SetSeamlessStageControlObject (0x135df98): typed by tools/typed_lift.py (round A1)
inline uint32_t SetSeamlessStageControlObject(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t a0)
{
    uint32_t v0 = 0;
    v0 = 0x1460000u;
    {
        A1_W32B(v0, 24228, a0);
        return v0;
    }
}

// PauseSeamlessStageManager (0x135e138): typed by tools/typed_lift.py (round A1)
inline uint32_t PauseSeamlessStageManager(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint64_t a0)
{
    uint32_t v0 = 0;
    uint32_t v1 = 0;
    bool df_v1 = false;
    v0 = 0x1460000u;
    {
        const bool c = (int64_t)a0 != 0;
        A1_W32B(v0, 24196, (uint32_t)a0);
        if (!c) {
            v1 = 0x1460000u; df_v1 = true;
            v0 = 1u;
            A1_W32B(v1, 24172, v0);
        }
    }
    {
        return v0;
    }
}

// getStageIDWithOffset (0x135e280): typed by tools/typed_lift.py (round A1)
inline a1::RetVF getStageIDWithOffset(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t a0, uint32_t a1, uint32_t a2, uint32_t a3, uint32_t t0)
{
    uint32_t v0 = 0;
    float f0 = 0.0f;
    float f1 = 0.0f;
    float f2 = 0.0f;
    float f3 = 0.0f;
    float f4 = 0.0f;
    float f5 = 0.0f;
    float f6 = 0.0f;
    float f7 = 0.0f;
    uint32_t fcr31 = 0;
    f1 = a1_flt(A1_R32(a0));
    v0 = 0x1460000u;
    v0 = v0 + 0x5E80u;
    f2 = a1_flt(A1_R32B(a0, 8));
    f7 = a1_flt(A1_R32B(v0, 56));
    { int32_t tmp; std::memcpy(&tmp, &f7, sizeof(tmp)); f7 = ((float)(int32_t)(tmp)); }
    f3 = a1_flt(A1_R32B(v0, 68));
    f6 = a1_flt(A1_R32B(v0, 60));
    { int32_t tmp; std::memcpy(&tmp, &f6, sizeof(tmp)); f6 = ((float)(int32_t)(tmp)); }
    f5 = a1_flt(A1_R32B(v0, 80));
    f1 = (-(float)(f1));
    f0 = a1_flt(A1_R32B(v0, 64));
    f4 = a1_flt(A1_R32B(v0, 84));
    f0 = a1f::mul(f0, f7);
    f2 = a1f::sub(f2, f4);
    f1 = a1f::sub(f1, f5);
    f3 = a1f::mul(f3, f6);
    f1 = a1f::div(fcr31, f1, f0);
    f2 = a1f::div(fcr31, f2, f3);
    { int32_t tmp = a1f::cvtw(f1); std::memcpy(&f0, &tmp, sizeof(tmp)); }
    A1_W32(a1, a1_bits(f0));
    { int32_t tmp = a1f::cvtw(f2); std::memcpy(&f0, &tmp, sizeof(tmp)); }
    A1_W32(a2, a1_bits(f0));
    f0 = a1_flt(A1_R32(a1));
    { int32_t tmp; std::memcpy(&tmp, &f0, sizeof(tmp)); f0 = ((float)(int32_t)(tmp)); }
    f1 = a1f::sub(f1, f0);
    A1_W32(a3, a1_bits(f1));
    f0 = a1_flt(A1_R32(a2));
    { int32_t tmp; std::memcpy(&tmp, &f0, sizeof(tmp)); f0 = ((float)(int32_t)(tmp)); }
    f2 = a1f::sub(f2, f0);
    {
        A1_W32(t0, a1_bits(f2));
        return a1::RetVF{(uint64_t)a1_sx64(v0), f0};
    }
}

// isSeamlessStage (0x135e608): typed by tools/typed_lift.py (round A1)
inline uint32_t isSeamlessStage(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt)
{
    uint32_t v0 = 0;
    uint32_t v1 = 0;
    v1 = 0x1460000u;
    {
        v0 = A1_R32B(v1, 24192);
        return v0;
    }
}

// getSpecialEffectObjWork (0x135efb0): typed by tools/typed_lift.py (round A1)
inline uint32_t getSpecialEffectObjWork(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t a0)
{
    uint32_t v0 = 0;
    {
        v0 = A1_R32B(a0, 48);
        return v0;
    }
}

// GetWorldLayoutSheetSegHeader (0x1365d08): typed by tools/typed_lift.py (round A1)
inline uint32_t GetWorldLayoutSheetSegHeader(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt)
{
    uint32_t v0 = 0;
    uint32_t v1 = 0;
    v1 = 0x1460000u;
    {
        v0 = A1_R32B(v1, 25540);
        return v0;
    }
}

// stagemanager_getSegmentVariation (0x1366b60): typed by tools/typed_lift.py (round A1)
inline uint32_t stagemanager_getSegmentVariation(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt)
{
    uint32_t v0 = 0;
    uint32_t v1 = 0;
    v1 = 0x1460000u;
    {
        v0 = A1_R32B(v1, 25532);
        return v0;
    }
}

// initWorldLayout (0x1366d00): typed by tools/typed_lift.py (round A1)
inline uint32_t initWorldLayout(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t a0)
{
    uint32_t v0 = 0;
    v0 = 0x1460000u;
    {
        A1_W32B(v0, 25540, a0);
        return v0;
    }
}

// bgmEnvTickProckMask (0x136a2d0): typed by tools/typed_lift.py (round A1)
inline uint32_t bgmEnvTickProckMask(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t a0)
{
    uint32_t v0 = 0;
    v0 = 0x1460000u;
    {
        A1_W32B(v0, 25780, a0);
        return v0;
    }
}

// bossNormalHook (0x136a378): typed by tools/typed_lift.py (round A1)
inline uint32_t bossNormalHook(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t t0)
{
    uint32_t v0 = 0;
    v0 = A1_R32B(t0, 20);
    v0 = v0 + 1u;
    {
        A1_W32B(t0, 20, v0);
        return v0;
    }
}

// gamecore_sub_0136A388 (0x136a388): typed by tools/typed_lift.py (round A1)
inline uint32_t gamecore_sub_0136A388(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint64_t a0)
{
    uint32_t v0 = 0;
    uint32_t v1 = 0;
    v0 = 0x1460000u;
    v0 = v0 + 0x64B0u;
    v1 = A1_R32(v0);
    if ((int64_t)a0 == (int64_t)(int32_t)v1) {
        A1_W32(v0, 0u);
    } else {
    }
    {
        return v0;
    }
}

// bgmPlay (0x136aa38): typed by tools/typed_lift.py (round A1)
inline uint32_t bgmPlay(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t a0, float f12)
{
    uint32_t v0 = 0;
    v0 = A1_R32B(a0, 36);
    A1_W32B(a0, 12, a1_bits(f12));
    v0 = v0 | 1u;
    {
        A1_W32B(a0, 36, v0);
        return v0;
    }
}

// bgmStop (0x136aa50): typed by tools/typed_lift.py (round A1)
inline uint32_t bgmStop(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t a0, uint64_t a1, float f12)
{
    uint32_t v0 = 0;
    {
        const bool c = (int64_t)a1 == 0;
        A1_W32B(a0, 24, a1_bits(f12));
        if (!c) {
            A1_W32B((uint32_t)a1, 0, 0u);
        }
    }
    v0 = A1_R32B(a0, 36);
    A1_W32B(a0, 32, (uint32_t)a1);
    v0 = v0 | 4u;
    {
        A1_W32B(a0, 36, v0);
        return v0;
    }
}

// bgmStopChk (0x136aa70): typed by tools/typed_lift.py (round A1)
inline uint64_t bgmStopChk(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint64_t a0)
{
    uint64_t v0 = 0;
    {
        (v0 = (uint64_t)((uint64_t)a0 + (uint64_t)0ULL));
        return v0;
    }
}

// bgmChangeVol (0x136aad0): typed by tools/typed_lift.py (round A1)
inline uint32_t bgmChangeVol(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t a0, float f12, float f13)
{
    uint32_t v0 = 0;
    v0 = A1_R32B(a0, 36);
    A1_W32B(a0, 20, a1_bits(f13));
    v0 = v0 | 2u;
    A1_W32B(a0, 16, a1_bits(f12));
    {
        A1_W32B(a0, 36, v0);
        return v0;
    }
}

// gamecore_sub_0136E4F8 (0x136e4f8): typed by tools/typed_lift.py (round A1)
inline uint32_t gamecore_sub_0136E4F8(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t a0)
{
    uint32_t v0 = 0;
    v0 = 1u;
    {
        A1_W16B(a0, 170, (uint16_t)v0);
        return v0;
    }
}

// gamecore_sub_0136E7A0 (0x136e7a0): typed by tools/typed_lift.py (round A1)
inline void gamecore_sub_0136E7A0(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t a1)
{
    A1_W32B(a1, 156, 0u);
    {
        A1_W32B(a1, 144, 0u);
        
        return;
    }
}

// nico_IK_idDetect (0x13757f8): typed by tools/typed_lift.py (round A1)
inline uint32_t nico_IK_idDetect(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t a0)
{
    uint32_t v0 = 0;
    uint32_t v1 = 0;
    v0 = A1_R32B(a0, 20);
    a0 = 0x11E0000u;
    v1 = 0xFFFE0000u;
    v1 = v1 | 0xFFFFu;
    v0 = v0 & v1;
    {
        A1_W32B(a0, 24128, v0);
        return v0;
    }
}

// gamecore_sub_0137B738 (0x137b738): typed by tools/typed_lift.py (round A1)
inline uint64_t gamecore_sub_0137B738(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint64_t a0, uint64_t a1)
{
    uint64_t v0 = 0;
    (v0 = (uint64_t)(a1 ^ a0));
    {
        (v0 = (uint64_t)(((uint64_t)0ULL < (uint64_t)v0) ? 1 : 0));
        return v0;
    }
}

// menuSelected (0x137b800): typed by tools/typed_lift.py (round A1)
inline uint32_t menuSelected(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt)
{
    uint32_t v0 = 0;
    uint32_t v1 = 0;
    v1 = 0x1460000u;
    {
        v0 = A1_R32B(v1, 25820);
        return v0;
    }
}

// GetDebugDispType (0x137c310): typed by tools/typed_lift.py (round A1)
inline uint64_t GetDebugDispType(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt)
{
    uint64_t v0 = 0;
    {
        (v0 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL));
        return v0;
    }
}

// is_LwsOrientObj (0x137e9f0): typed by tools/typed_lift.py (round A1)
inline uint32_t is_LwsOrientObj(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t a0)
{
    uint32_t v0 = 0;
    v0 = A1_R8B(a0, 8);
    v0 = v0 ^ 0x16u;
    {
        v0 = v0 < 1u ? 1u : 0u;
        return v0;
    }
}

// CheckDeathTentacle (0x1384de0): typed by tools/typed_lift.py (round A1)
inline uint32_t CheckDeathTentacle(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt)
{
    uint32_t v0 = 0;
    uint32_t v1 = 0;
    v0 = 0x1460000u;
    v0 = v0 + 0x64F0u;
    v1 = A1_R32(v0);
    if (v1 != 0) {
        A1_W32(v0, 0u);
        {
            v0 = 0u;
            return v0;
        }
    }
    {
        v0 = 1u;
        return v0;
    }
}

// getEffectBloodWorkSize (0x1388048): typed by tools/typed_lift.py (round A1)
inline uint32_t getEffectBloodWorkSize(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t a0)
{
    uint32_t v0 = 0;
    v0 = a0 << 1;
    v0 = v0 + a0;
    v0 = v0 << 4;
    {
        v0 = v0 + 0x33u;
        return v0;
    }
}

// getWeakPointEffectIntensity (0x1388060): typed by tools/typed_lift.py (round A1)
inline a1::RetVF getWeakPointEffectIntensity(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t a0)
{
    uint32_t at = 0;
    uint32_t v0 = 0;
    float f0 = 0.0f;
    v0 = a0 << 4;
    v0 = v0 - a0;
    v0 = v0 << 3;
    at = 0x11E0000u;
    at = at + v0;
    f0 = a1_flt(A1_R32B(at, 25252));
    {
        return a1::RetVF{(uint64_t)a1_sx64(v0), f0};
    }
}

// getTriggerPointEffectIntensity (0x1388080): typed by tools/typed_lift.py (round A1)
inline a1::RetVF getTriggerPointEffectIntensity(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t a0)
{
    uint32_t at = 0;
    uint32_t v0 = 0;
    float f0 = 0.0f;
    a0 = a0 + 3u;
    v0 = a0 << 4;
    v0 = v0 - a0;
    v0 = v0 << 3;
    at = 0x11E0000u;
    at = at + v0;
    f0 = a1_flt(A1_R32B(at, 25252));
    {
        return a1::RetVF{(uint64_t)a1_sx64(v0), f0};
    }
}

// effectClearDamageWork (0x13885f0): typed by tools/typed_lift.py (round A1)
inline uint32_t effectClearDamageWork(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt)
{
    uint32_t v0 = 0;
    v0 = 0x11E0000u;
    {
        A1_W32B(v0, 25160, 0u);
        return v0;
    }
}

// effectGetDamageWork (0x1388600): typed by tools/typed_lift.py (round A1)
inline uint32_t effectGetDamageWork(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t a0)
{
    uint32_t v0 = 0;
    uint32_t v1 = 0;
    v1 = 0x11E0000u;
    v1 = v1 + 0x5FA0u;
    A1_W32(a0, v1);
    {
        v0 = A1_R32B(v1, 680);
        return v0;
    }
}

// gamecore_sub_013886B8 (0x13886b8): typed by tools/typed_lift.py (round A1)
inline a1::RetVF gamecore_sub_013886B8(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t a0)
{
    uint32_t v0 = 0;
    float f0 = 0.0f;
    float f1 = 0.0f;
    float f2 = 0.0f;
    v0 = A1_R32B(a0, 80);
    f0 = a1_flt(A1_R32B(a0, 40));
    f2 = a1_flt(A1_R32B(v0, 112));
    f1 = a1_flt(A1_R32B(a0, 36));
    f0 = a1f::mul(f0, f2);
    A1_W32B(a0, 40, a1_bits(f0));
    f2 = a1_flt(A1_R32B(v0, 20));
    f0 = a1_flt(A1_R32B(v0, 116));
    f0 = a1f::mul(f0, f2);
    f1 = a1f::mul(f1, f0);
    {
        A1_W32B(a0, 36, a1_bits(f1));
        return a1::RetVF{(uint64_t)a1_sx64(v0), f0};
    }
}

// getFallenLeafSize (0x138ea10): typed by tools/typed_lift.py (round A1)
inline uint32_t getFallenLeafSize(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint64_t a0)
{
    uint32_t v0 = 0;
    uint32_t v1 = 0;
    uint32_t a1 = 0;
    uint32_t a3 = 0;
    uint64_t a2 = 0;
    uint64_t hi = 0;
    uint64_t lo = 0;
    bool df_a1 = false;
    (a2 = (uint64_t)((uint64_t)a0 + (uint64_t)0ULL));
    v0 = 0x30000u;
    (v1 = (uint32_t)((int32_t)((int32_t)ADD32((uint32_t)a2, 28))));
    v0 = v0 | 0xFFFFu;
    a3 = A1_R32(v1);
    v0 = a3 & v0;
    {
        const bool c = v0 == 0;
        v0 = v1 - v0;
        if (!c) {
            (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B(v0, 0))));
            goto L_138ea3c;
        }
    }
    (a0 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL));
L_138ea3c:
    {
        const bool c = (int64_t)a0 == 0;
        { const uint32_t _dynInsn = A1_R32(0x138EA40u); const int32_t _dynSimm = (int16_t)(_dynInsn & 0xFFFFu); const uint32_t _dynImm = _dynInsn & 0xFFFFu; (void)_dynSimm; (void)_dynImm; (v0 = (uint32_t)((int32_t)((int32_t)((uint32_t)_dynImm << 16)))); }
        if (!c) {
            { const uint32_t _dynInsn = A1_R32(0x138EA44u); const int32_t _dynSimm = (int16_t)(_dynInsn & 0xFFFFu); const uint32_t _dynImm = _dynInsn & 0xFFFFu; (void)_dynSimm; (void)_dynImm; (v1 = (uint32_t)((int32_t)((int32_t)A1_R32(ADD32(v0, _dynSimm))))); }
            {
                const bool c = (int64_t)a0 == (int64_t)(int32_t)v1;
                v0 = a3 >> 18;
                if (!c) {
                    (a1 = (uint32_t)((int32_t)((int32_t)A1_R32B((uint32_t)a0, 8)))); df_a1 = true;
                    v0 = v0 & 0xFFFu;
                    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)a0, 16))));
                    v1 = A1_R32B(a1, 12);
                    { int64_t result = (int64_t)(int32_t)v0 * (int64_t)(int32_t)v1; lo = (uint64_t)(int64_t)(int32_t)result; hi = (uint64_t)(int64_t)(int32_t)(result >> 32); (v0 = (uint32_t)((int32_t)((int32_t)result))); }
                    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)a0, v0))));
                    goto L_138ea74;
                }
            }
        }
    }
    (a0 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL));
L_138ea74:
    (v0 = (uint32_t)((int32_t)((int32_t)A1_R32B((uint32_t)a2, 48))));
    {
        const bool c = v0 == 0;
        a1 = 0x170u; df_a1 = true;
        if (!c) {
            (v0 = (uint32_t)((int32_t)((int32_t)A1_R32B((uint32_t)a2, 60))));
            goto L_138ea94;
        }
    }
    (v1 = (uint32_t)((int32_t)((int32_t)A1_R32B((uint32_t)a2, 44))));
    (v0 = (uint32_t)((int32_t)((int32_t)A1_R32B((uint32_t)a2, 40))));
    { int64_t result = (int64_t)(int32_t)v0 * (int64_t)(int32_t)v1; lo = (uint64_t)(int64_t)(int32_t)result; hi = (uint64_t)(int64_t)(int32_t)(result >> 32); (v0 = (uint32_t)((int32_t)((int32_t)result))); }
L_138ea94:
    v0 = v0 << 7;
    a1 = v0 + a1; df_a1 = true;
    (v0 = (uint32_t)((int32_t)((int32_t)A1_R32B((uint32_t)a0, 8))));
    v0 = v0 << 6;
    {
        v0 = a1 + v0;
        return v0;
    }
}

// getArrowWaterSize (0x138eb48): typed by tools/typed_lift.py (round A1)
inline uint32_t getArrowWaterSize(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t a0)
{
    uint32_t v0 = 0;
    uint32_t v1 = 0;
    uint32_t a1 = 0;
    uint32_t a2 = 0;
    uint64_t hi = 0;
    uint64_t lo = 0;
    bool df_v1 = false, df_a1 = false;
    a0 = a0 + 0x1Cu;
    v0 = 0x30000u;
    a2 = A1_R32(a0);
    v0 = v0 | 0xFFFFu;
    v0 = a2 & v0;
    {
        const bool c = v0 == 0;
        a0 = a0 - v0;
        if (!c) {
            a0 = A1_R32(a0);
            goto L_138eb74;
        }
    }
    a0 = 0u;
L_138eb74:
    {
        const bool c = a0 == 0;
        v0 = 0u;
        if (!c) {
            { const uint32_t _dynInsn = A1_R32(0x138EB7Cu); const int32_t _dynSimm = (int16_t)(_dynInsn & 0xFFFFu); const uint32_t _dynImm = _dynInsn & 0xFFFFu; (void)_dynSimm; (void)_dynImm; (v0 = (uint32_t)((int32_t)((int32_t)((uint32_t)_dynImm << 16)))); }
            { const uint32_t _dynInsn = A1_R32(0x138EB80u); const int32_t _dynSimm = (int16_t)(_dynInsn & 0xFFFFu); const uint32_t _dynImm = _dynInsn & 0xFFFFu; (void)_dynSimm; (void)_dynImm; (v1 = (uint32_t)((int32_t)((int32_t)A1_R32(ADD32(v0, _dynSimm))))); } df_v1 = true;
            if (a0 == v1) {
                v0 = 0u;
            } else {
                a1 = A1_R32B(a0, 8); df_a1 = true;
                v0 = a2 >> 18;
                v0 = v0 & 0xFFFu;
                a0 = A1_R32B(a0, 16);
                v1 = A1_R32B(a1, 12); df_v1 = true;
                { int64_t result = (int64_t)(int32_t)v0 * (int64_t)(int32_t)v1; lo = (uint64_t)(int64_t)(int32_t)result; hi = (uint64_t)(int64_t)(int32_t)(result >> 32); (v0 = (uint32_t)((int32_t)((int32_t)result))); }
                v0 = a0 + v0;
            }
        }
    }
    v0 = A1_R32B(v0, 8);
    v0 = v0 << 6;
    {
        v0 = v0 + 0xDF0u;
        return v0;
    }
}

// gamecore_sub_0138F138 (0x138f138): typed by tools/typed_lift.py (round A1)
inline void gamecore_sub_0138F138(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t a2, uint64_t a3)
{
    uint32_t v0 = 0;
    uint32_t v1 = 0;
    bool df_v0 = false, df_v1 = false;
    a2 = a2 & 1u;
    if (a2 != 0) {
        {
            const bool c = (int64_t)a3 == 0;
            v0 = 1u; df_v0 = true;
            if (!c) {
                {
                    const bool c = (int64_t)a3 == (int64_t)(int32_t)v0;
                    v1 = 0x1460000u; df_v1 = true;
                    if (c) {
                        goto L_138f170;
                    }
                }
                {
                    return;
                }
            }
        }
        v1 = 0x1460000u; df_v1 = true;
        {
            A1_W32B(v1, 26360, v0);
            return;
        }
L_138f170:
        v0 = 2u; df_v0 = true;
        A1_W32B(v1, 26360, v0);
    }
    {
        return;
    }
}

// setHorseAIStopFlag (0x1395e00): typed by tools/typed_lift.py (round A1)
inline uint32_t setHorseAIStopFlag(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t a0)
{
    uint32_t v0 = 0;
    v0 = 0x1460000u;
    {
        A1_W32B(v0, 26372, a0);
        return v0;
    }
}

// getHorseStandByMode (0x1395e10): typed by tools/typed_lift.py (round A1)
inline uint32_t getHorseStandByMode(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt)
{
    uint32_t v0 = 0;
    uint32_t v1 = 0;
    v1 = 0x1460000u;
    {
        v0 = A1_R32B(v1, 26376);
        return v0;
    }
}

// SCRFuncHorseStandByFlag (0x1396328): typed by tools/typed_lift.py (round A1)
inline uint32_t SCRFuncHorseStandByFlag(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t a1)
{
    uint32_t v0 = 0;
    v0 = 0x1460000u;
    {
        A1_W32B(v0, 26376, a1);
        return v0;
    }
}

// ColCont_Reset (0x139a878): typed by tools/typed_lift.py (round A1)
inline void ColCont_Reset(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t a0)
{
    A1_W64(a0, 0ULL);
    {
        A1_W32B(a0, 8, 0u);
        
        return;
    }
}

// ColCont2_IsColStatus (0x139aa00): typed by tools/typed_lift.py (round A1)
inline uint64_t ColCont2_IsColStatus(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t a0, uint64_t a1, uint64_t a2)
{
    uint64_t v0 = 0;
    uint64_t v1 = 0;
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 4294967295))));
    if ((int64_t)a2 != (int64_t)v0) {
        (v0 = (uint64_t)(A1_R64B(a0, 0)));
    } else {
        if ((int64_t)a1 != (int64_t)a2) {
            (v1 = (uint64_t)(A1_R64B(a0, 0)));
            goto L_139aa38;
        }
        {
            (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 1))));
            return v0;
        }
    }
    (v0 = (uint64_t)(v0 & a1));
    if ((int64_t)v0 != (int64_t)a2) {
        (v0 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL));
        goto L_139aa48;
    }
    {
        (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 1))));
        return v0;
    }
L_139aa38:
    (v1 = (uint64_t)(v1 & a1));
    {
        const bool c = (int64_t)v1 != 0;
        (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 1))));
        if (!c) {
            (v0 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL));
        }
    }
L_139aa48:
    {
        return v0;
    }
}

// ColCont2_IsEventColStatus (0x139aa50): typed by tools/typed_lift.py (round A1)
inline uint64_t ColCont2_IsEventColStatus(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t a0, uint64_t a1, uint64_t a2)
{
    uint64_t v0 = 0;
    uint64_t v1 = 0;
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 4294967295))));
    if ((int64_t)a2 != (int64_t)v0) {
        (v0 = (uint64_t)(A1_R64B(a0, 8)));
    } else {
        if ((int64_t)a1 != (int64_t)a2) {
            (v1 = (uint64_t)(A1_R64B(a0, 8)));
            goto L_139aa88;
        }
        {
            (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 1))));
            return v0;
        }
    }
    (v0 = (uint64_t)(v0 & a1));
    if ((int64_t)v0 != (int64_t)a2) {
        (v0 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL));
        goto L_139aa98;
    }
    {
        (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 1))));
        return v0;
    }
L_139aa88:
    (v1 = (uint64_t)(v1 & a1));
    {
        const bool c = (int64_t)v1 != 0;
        (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 1))));
        if (!c) {
            (v0 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL));
        }
    }
L_139aa98:
    {
        return v0;
    }
}

// MotSpdCtrl_Reset (0x139ac08): typed by tools/typed_lift.py (round A1)
inline void MotSpdCtrl_Reset(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t a0)
{
    A1_W8B(a0, 17, (uint8_t)0u);
    A1_W8B(a0, 16, (uint8_t)0u);
    {
        A1_W32(a0, 0u);
        
        return;
    }
}

// MotSpdCtrl_SetParam (0x139ac60): typed by tools/typed_lift.py (round A1)
inline void MotSpdCtrl_SetParam(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t a0, uint32_t a1)
{
    {
        A1_W32B(a0, 20, a1);
        
        return;
    }
}

// inInNowLoadingEffect (0x139e988): typed by tools/typed_lift.py (round A1)
inline uint32_t inInNowLoadingEffect(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt)
{
    uint32_t v0 = 0;
    uint32_t v1 = 0;
    v1 = 0x1460000u;
    {
        v0 = A1_R32B(v1, 26384);
        return v0;
    }
}

// DirectTurn_Reset (0x13a75f8): typed by tools/typed_lift.py (round A1)
inline void DirectTurn_Reset(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t a0)
{
    A1_W32B(a0, 16, 0u);
    {
        A1_W8B(a0, 20, (uint8_t)0u);
        
        return;
    }
}

// is_doing_nico_sight_on (0x13aa380): typed by tools/typed_lift.py (round A1)
inline uint32_t is_doing_nico_sight_on(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t a0)
{
    uint32_t v0 = 0;
    v0 = A1_R32(a0);
    {
        v0 = 0u < v0 ? 1u : 0u;
        return v0;
    }
}

// getPlayerLastAttrStatus (0x13b53b0): typed by tools/typed_lift.py (round A1)
inline uint64_t getPlayerLastAttrStatus(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt)
{
    uint32_t v1 = 0;
    uint64_t v0 = 0;
    v1 = 0x1460000u;
    {
        (v0 = (uint64_t)(A1_R64B(v1, 27072)));
        return v0;
    }
}

// getPlayerLastParentObj (0x13b53c0): typed by tools/typed_lift.py (round A1)
inline uint32_t getPlayerLastParentObj(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt)
{
    uint32_t v0 = 0;
    uint32_t v1 = 0;
    v1 = 0x1460000u;
    {
        v0 = A1_R32B(v1, 27080);
        return v0;
    }
}

// nico_setPlayerDemoAdditionClothCollisionFloorTable1 (0x13ba418): typed by tools/typed_lift.py (round A1)
inline uint32_t nico_setPlayerDemoAdditionClothCollisionFloorTable1(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt)
{
    uint32_t v0 = 0;
    uint32_t v1 = 0;
    uint32_t a0 = 0;
    uint32_t a1 = 0;
    uint32_t a2 = 0;
    uint32_t a3 = 0;
    uint32_t t0 = 0;
    v0 = 0x13C0000u;
    t0 = 0x1460000u;
    v0 = v0 - 0x56D8u;
    a1 = 0x1460000u;
    a2 = 0x1460000u;
    a3 = 0x1460000u;
    v1 = 0x1460000u;
    a0 = 0x1460000u;
    A1_W32B(v1, 27136, v0);
    A1_W32B(t0, 27104, v0);
    A1_W32B(a1, 27108, 0u);
    A1_W32B(a2, 27112, 0u);
    A1_W32B(a3, 27116, 0u);
    {
        A1_W32B(a0, 27140, 0u);
        return v0;
    }
}

// nico_setPlayerDemoAdditionClothCollisionRightArm (0x13ba458): typed by tools/typed_lift.py (round A1)
inline a1::RetVF nico_setPlayerDemoAdditionClothCollisionRightArm(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t a0)
{
    uint32_t at = 0;
    uint32_t v0 = 0;
    uint32_t v1 = 0;
    uint32_t a1 = 0;
    uint32_t a2 = 0;
    uint32_t a3 = 0;
    uint32_t t0 = 0;
    uint32_t t1 = 0;
    uint32_t t2 = 0;
    float f0 = 0.0f;
    v1 = 0x11E0000u;
    at = 0x3DCC0000u;
    at = at | 0xCCCCu;
    f0 = a1_flt(at);
    v0 = 0x13C0000u;
    v1 = v1 + 0x7020u;
    v0 = v0 - 0x5538u;
    a1 = 0x1460000u;
    a2 = 0x1460000u;
    a3 = 0x1460000u;
    t0 = 0x1460000u;
    t1 = 0x1460000u;
    t2 = 0x1460000u;
    A1_W32B(a1, 27104, v0);
    A1_W32B(a2, 27108, v1);
    A1_W32(v1, a0);
    A1_W32B(v1, 4, a1_bits(f0));
    A1_W32B(a3, 27112, 0u);
    A1_W32B(t0, 27116, 0u);
    A1_W32B(t1, 27136, 0u);
    {
        A1_W32B(t2, 27140, 0u);
        return a1::RetVF{(uint64_t)a1_sx64(v0), f0};
    }
}

// nico_setPlayerDemoAdditionClothCollisionRightHandAndSwordDeep (0x13ba4b0): typed by tools/typed_lift.py (round A1)
inline a1::RetVF nico_setPlayerDemoAdditionClothCollisionRightHandAndSwordDeep(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t a0)
{
    uint32_t at = 0;
    uint32_t v0 = 0;
    uint32_t v1 = 0;
    uint32_t a1 = 0;
    uint32_t a2 = 0;
    uint32_t a3 = 0;
    uint32_t t0 = 0;
    uint32_t t1 = 0;
    uint32_t t2 = 0;
    float f0 = 0.0f;
    v1 = 0x11E0000u;
    at = 0x3D8F0000u;
    at = at | 0x5C28u;
    f0 = a1_flt(at);
    v0 = 0x13C0000u;
    v1 = v1 + 0x7020u;
    v0 = v0 - 0x5368u;
    a1 = 0x1460000u;
    a2 = 0x1460000u;
    a3 = 0x1460000u;
    t0 = 0x1460000u;
    t1 = 0x1460000u;
    t2 = 0x1460000u;
    A1_W32B(a1, 27104, v0);
    A1_W32B(a2, 27108, v1);
    A1_W32(v1, a0);
    A1_W32B(v1, 4, a1_bits(f0));
    A1_W32B(a3, 27112, 0u);
    A1_W32B(t0, 27116, 0u);
    A1_W32B(t1, 27136, 0u);
    {
        A1_W32B(t2, 27140, 0u);
        return a1::RetVF{(uint64_t)a1_sx64(v0), f0};
    }
}

// nico_setPlayerDemoAdditionClothCollisionRightHandAndSword (0x13ba508): typed by tools/typed_lift.py (round A1)
inline a1::RetVF nico_setPlayerDemoAdditionClothCollisionRightHandAndSword(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t a0)
{
    uint32_t at = 0;
    uint32_t v0 = 0;
    uint32_t v1 = 0;
    uint32_t a1 = 0;
    uint32_t a2 = 0;
    uint32_t a3 = 0;
    uint32_t t0 = 0;
    uint32_t t1 = 0;
    uint32_t t2 = 0;
    float f0 = 0.0f;
    v1 = 0x11E0000u;
    at = 0x3CF50000u;
    at = at | 0xC28Fu;
    f0 = a1_flt(at);
    v0 = 0x13C0000u;
    v1 = v1 + 0x7020u;
    v0 = v0 - 0x5368u;
    a1 = 0x1460000u;
    a2 = 0x1460000u;
    a3 = 0x1460000u;
    t0 = 0x1460000u;
    t1 = 0x1460000u;
    t2 = 0x1460000u;
    A1_W32B(a1, 27104, v0);
    A1_W32B(a2, 27108, v1);
    A1_W32(v1, a0);
    A1_W32B(v1, 4, a1_bits(f0));
    A1_W32B(a3, 27112, 0u);
    A1_W32B(t0, 27116, 0u);
    A1_W32B(t1, 27136, 0u);
    {
        A1_W32B(t2, 27140, 0u);
        return a1::RetVF{(uint64_t)a1_sx64(v0), f0};
    }
}

// nico_resetPlayerDemoAdditionClothCollision (0x13ba768): typed by tools/typed_lift.py (round A1)
inline uint32_t nico_resetPlayerDemoAdditionClothCollision(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt)
{
    uint32_t v0 = 0;
    uint32_t v1 = 0;
    uint32_t a0 = 0;
    uint32_t a1 = 0;
    a1 = 0x1460000u;
    v0 = 0x1460000u;
    v1 = 0x1460000u;
    a0 = 0x1460000u;
    A1_W32B(v0, 27112, 0u);
    A1_W32B(a1, 27104, 0u);
    A1_W32B(v1, 27136, 0u);
    {
        A1_W32B(a0, 27140, 0u);
        return v0;
    }
}

// player_initSlipEffect (0x13cd5f8): typed by tools/typed_lift.py (round A1)
inline uint32_t player_initSlipEffect(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t a1)
{
    uint32_t v0 = 0;
    uint32_t v1 = 0;
    uint32_t a0 = 0;
    v0 = 0x11E0000u;
    v1 = 1u;
    v0 = v0 + 0x70E8u;
    a0 = 2u;
    A1_W64B(a1, 8, 0ULL);
    A1_W32(a1, 0u);
    A1_W32B(v0, 28, v1);
    A1_W32B(v0, 52, a0);
    A1_W32B(v0, 56, 0u);
    A1_W32(v0, 0u);
    A1_W32B(v0, 4, 0u);
    A1_W32B(v0, 8, 0u);
    A1_W32B(v0, 12, 0u);
    A1_W32B(v0, 32, 0u);
    A1_W32B(v0, 16, 0u);
    A1_W32B(v0, 20, 0u);
    A1_W32B(v0, 24, 0u);
    A1_W32B(v0, 36, 0u);
    A1_W32B(v0, 40, 0u);
    A1_W32B(v0, 44, 0u);
    {
        A1_W32B(v0, 48, 0u);
        return v0;
    }
}

// bossCloudStart (0x13d0ab8): typed by tools/typed_lift.py (round A1)
inline uint32_t bossCloudStart(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt)
{
    uint32_t v0 = 0;
    uint32_t v1 = 0;
    v1 = 0x1460000u;
    v0 = 1u;
    {
        A1_W32B(v1, 27316, v0);
        return v0;
    }
}

// bossDemoPlayerBreakDownAccept (0x13d0bc0): typed by tools/typed_lift.py (round A1)
inline uint32_t bossDemoPlayerBreakDownAccept(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt)
{
    uint32_t v0 = 0;
    uint32_t v1 = 0;
    v1 = 0x1460000u;
    v0 = 1u;
    {
        A1_W32B(v1, 27324, v0);
        return v0;
    }
}

// bossDemoPlayerBreakDownEnd (0x13d0bd0): typed by tools/typed_lift.py (round A1)
inline uint32_t bossDemoPlayerBreakDownEnd(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt)
{
    uint32_t v0 = 0;
    uint32_t v1 = 0;
    v1 = 0x1460000u;
    v0 = 2u;
    {
        A1_W32B(v1, 27324, v0);
        return v0;
    }
}

// AI_InitAnimalOptimize (0x13d38a0): typed by tools/typed_lift.py (round A1)
inline a1::RetVF AI_InitAnimalOptimize(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t a0)
{
    uint32_t at = 0;
    uint32_t v0 = 0;
    float f0 = 0.0f;
    at = 0x41F00000u;
    f0 = a1_flt(at);
    v0 = 1u;
    A1_W32B(a0, 8, 0u);
    A1_W32(a0, v0);
    {
        A1_W32B(a0, 4, a1_bits(f0));
        return a1::RetVF{(uint64_t)a1_sx64(v0), f0};
    }
}

// AI_InitAnimalWork (0x13d3a48): typed by tools/typed_lift.py (round A1)
inline uint32_t AI_InitAnimalWork(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t a0, uint32_t a1)
{
    uint32_t v0 = 0;
    v0 = 0xFFFFFFFFu;
    A1_W32(a0, a1);
    A1_W32B(a0, 8, v0);
    A1_W32B(a0, 16, 0u);
    A1_W32B(a0, 4, v0);
    {
        A1_W32B(a0, 12, 0u);
        return v0;
    }
}

// AI_SetAnimalWorkParam (0x13d3a68): typed by tools/typed_lift.py (round A1)
inline void AI_SetAnimalWorkParam(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t a0, uint32_t a1)
{
    {
        A1_W32B(a0, 16, a1);
        
        return;
    }
}

// AI_InitGexCommonWork (0x13d3a70): typed by tools/typed_lift.py (round A1)
inline void AI_InitGexCommonWork(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t a0)
{
    A1_W32B(a0, 12, 0u);
    A1_W32B(a0, 8, 0u);
    A1_W32(a0, 0u);
    {
        A1_W32B(a0, 4, 0u);
        
        return;
    }
}

// AI_InitSurpriseArrow (0x13d3a88): typed by tools/typed_lift.py (round A1)
inline void AI_InitSurpriseArrow(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t a0)
{
    A1_W32B(a0, 4, 0u);
    {
        A1_W32(a0, 0u);
        
        return;
    }
}

// AI_SetSurprisedArea (0x13d3a98): typed by tools/typed_lift.py (round A1)
inline void AI_SetSurprisedArea(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t a0, float f12)
{
    {
        A1_W32B(a0, 4, a1_bits(f12));
        
        return;
    }
}

// AI_UpdateVelocity (0x13d3da0): typed by tools/typed_lift.py (round A1)
inline a1::RetVF AI_UpdateVelocity(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t a0, float f12, float f13)
{
    uint32_t v0 = 0;
    uint32_t v1 = 0;
    float f0 = 0.0f;
    float f1 = 0.0f;
    uint32_t fcr31 = 0;
    f1 = a1_flt(A1_R32(a0));
    fcr31 = (a1f::lt(f1, f12)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
    {
        const bool c = (fcr31 & 0x800000u) == 0u;
        v1 = 1u;
        if (!c) {
            v0 = 0x1460000u;
            f0 = a1_flt(A1_R32B(v0, 22496));
            f0 = a1f::mul(f13, f0);
            f1 = a1f::add(f1, f0);
            fcr31 = (a1f::lt(f12, f1)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
            goto L_13d3de4;
        }
    }
    v0 = 0x1460000u;
    f0 = a1_flt(A1_R32B(v0, 22496));
    f0 = a1f::mul(f13, f0);
    f1 = a1f::sub(f1, f0);
    fcr31 = (a1f::lt(f1, f12)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
L_13d3de4:
    if ((fcr31 & 0x800000u) == 0u) {
        A1_W32(a0, a1_bits(f1));
    } else {
        f1 = ((float)(f12));
        v1 = 0u;
        A1_W32(a0, a1_bits(f1));
    }
    {
        v0 = v1;
        return a1::RetVF{(uint64_t)a1_sx64(v0), f0};
    }
}

// AI_GetAccelB (0x13d4008): typed by tools/typed_lift.py (round A1)
inline float AI_GetAccelB(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, float f12, float f13, float f14, float f15)
{
    uint32_t at = 0;
    float f0 = 0.0f;
    float f1 = 0.0f;
    float f2 = 0.0f;
    float f3 = 0.0f;
    uint32_t fcr31 = 0;
    at = 0x3D080000u;
    at = at | 0xAB85u;
    f0 = a1_flt(at);
    f2 = ((float)(f0));
    fcr31 = (a1f::lt(f15, f2)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
    {
        const bool c = (fcr31 & 0x800000u) != 0u;
        f3 = a1f::sub(f12, f13);
        if (!c) {
            f2 = ((float)(f15));
        }
    }
    f0 = a1f::div(fcr31, f14, f2);
    at = 0x40400000u;
    f1 = a1_flt(at);
    f0 = a1f::sub(f12, f0);
    f0 = ((float)(f0) * (float)(f1));
    f0 = a1f::sub(f3, f0);
    f0 = a1f::addFinite(f0, f0);
    f0 = a1f::div(fcr31, f0, f2);
    {
        return f0;
    }
}

// AI_GetAccelA (0x13d4060): typed by tools/typed_lift.py (round A1)
inline float AI_GetAccelA(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, float f12, float f13, float f14, float f15)
{
    uint32_t at = 0;
    float f0 = 0.0f;
    float f1 = 0.0f;
    uint32_t fcr31 = 0;
    at = 0x3D080000u;
    at = at | 0xAB85u;
    f0 = a1_flt(at);
    f1 = ((float)(f0));
    fcr31 = (a1f::lt(f14, f1)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
    {
        const bool c = (fcr31 & 0x800000u) != 0u;
        f13 = a1f::sub(f13, f12);
        if (!c) {
            f1 = ((float)(f14));
        }
    }
    f0 = a1f::mul(f15, f1);
    f1 = a1f::mul(f1, f1);
    f0 = a1f::sub(f13, f0);
    f0 = a1f::addFinite(f0, f0);
    f0 = a1f::div(fcr31, f0, f1);
    {
        return f0;
    }
}

// AI_GetAccelA_at0 (0x13d40a8): typed by tools/typed_lift.py (round A1)
inline float AI_GetAccelA_at0(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, float f12, float f13, float f14)
{
    uint32_t at = 0;
    float f0 = 0.0f;
    float f1 = 0.0f;
    uint32_t fcr31 = 0;
    at = 0x3D080000u;
    at = at | 0xAB85u;
    f0 = a1_flt(at);
    f1 = ((float)(f0));
    fcr31 = (a1f::lt(f14, f1)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
    {
        const bool c = (fcr31 & 0x800000u) != 0u;
        f13 = a1f::sub(f13, f12);
        if (!c) {
            f1 = ((float)(f14));
        }
    }
    at = 0xC0000000u;
    f0 = a1_flt(at);
    f1 = a1f::mul(f1, f1);
    f0 = a1f::mul(f13, f0);
    f0 = a1f::div(fcr31, f0, f1);
    {
        return f0;
    }
}

// AI_GetAccelB_at0 (0x13d40f0): typed by tools/typed_lift.py (round A1)
inline float AI_GetAccelB_at0(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, float f12, float f13, float f14)
{
    uint32_t at = 0;
    float f0 = 0.0f;
    uint32_t fcr31 = 0;
    at = 0x3D080000u;
    at = at | 0xAB85u;
    f0 = a1_flt(at);
    f13 = a1f::sub(f13, f12);
    fcr31 = (a1f::lt(f14, f0)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
    {
        const bool c = (fcr31 & 0x800000u) != 0u;
        f13 = a1f::add(f13, f13);
        if (!c) {
            f0 = ((float)(f14));
        }
    }
    f0 = a1f::div(fcr31, f13, f0);
    {
        return f0;
    }
}

// AI_GetAccelA3 (0x13d4128): typed by tools/typed_lift.py (round A1)
inline float AI_GetAccelA3(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, float f12, float f13, float f14, float f15, float f16, float f17)
{
    uint32_t at = 0;
    float f0 = 0.0f;
    float f1 = 0.0f;
    float f2 = 0.0f;
    float f3 = 0.0f;
    float f4 = 0.0f;
    uint32_t fcr31 = 0;
    at = 0x3D080000u;
    at = at | 0xAB85u;
    f0 = a1_flt(at);
    f4 = a1f::sub(f14, f13);
    fcr31 = (a1f::lt(f16, f0)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
    {
        const bool c = (fcr31 & 0x800000u) == 0u;
        f14 = a1f::add(f17, f17);
        if (!c) {
            f16 = ((float)(f0));
        }
    }
    f3 = a1f::mul(f16, f16);
    at = 0x40C00000u;
    f0 = a1_flt(at);
    at = 0xC0000000u;
    f2 = a1_flt(at);
    f1 = a1f::div(fcr31, f0, f16);
    f0 = a1f::div(fcr31, f0, f3);
    f1 = a1f::mul(f1, f12);
    f2 = a1f::div(fcr31, f2, f3);
    f0 = ((float)(f0) * (float)(f4));
    f0 = a1f::sub(f0, f14);
    f0 = a1f::subFinite(f0, f1);
    f0 = a1f::sub(f0, f15);
    {
        f0 = a1f::mul(f2, f0);
        return f0;
    }
}

// AI_GetAccelB3 (0x13d4298): typed by tools/typed_lift.py (round A1)
inline float AI_GetAccelB3(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, float f12, float f13, float f14, float f15, float f16, float f17)
{
    uint32_t at = 0;
    float f0 = 0.0f;
    float f1 = 0.0f;
    float f2 = 0.0f;
    float f3 = 0.0f;
    float f4 = 0.0f;
    uint32_t fcr31 = 0;
    at = 0x3D080000u;
    at = at | 0xAB85u;
    f0 = a1_flt(at);
    fcr31 = (a1f::lt(f16, f0)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
    {
        const bool c = (fcr31 & 0x800000u) == 0u;
        f15 = a1f::sub(f15, f14);
        if (!c) {
            f16 = ((float)(f0));
        }
    }
    at = 0x40800000u;
    f0 = a1_flt(at);
    f3 = a1f::mul(f16, f16);
    at = 0x40400000u;
    f2 = a1_flt(at);
    f4 = a1f::mul(f17, f16);
    f0 = a1f::div(fcr31, f0, f16);
    at = 0x40C00000u;
    f1 = a1_flt(at);
    f2 = a1f::mul(f12, f2);
    f1 = a1f::div(fcr31, f1, f3);
    f0 = a1f::mul(f0, f15);
    f0 = a1f::sub(f0, f13);
    f0 = a1f::subFinite(f0, f2);
    f0 = a1f::subFinite(f0, f4);
    {
        f0 = a1f::mul(f1, f0);
        return f0;
    }
}

// AI_GetAccelC3 (0x13d43f0): typed by tools/typed_lift.py (round A1)
inline float AI_GetAccelC3(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, float f12, float f13, float f14, float f15, float f16, float f17)
{
    uint32_t at = 0;
    float f0 = 0.0f;
    float f1 = 0.0f;
    float f2 = 0.0f;
    uint32_t fcr31 = 0;
    at = 0x3D080000u;
    at = at | 0xAB85u;
    f0 = a1_flt(at);
    f13 = a1f::add(f13, f12);
    fcr31 = (a1f::lt(f17, f0)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
    {
        const bool c = (fcr31 & 0x800000u) == 0u;
        f15 = a1f::sub(f15, f14);
        if (!c) {
            f17 = ((float)(f0));
        }
    }
    at = 0x41400000u;
    f0 = a1_flt(at);
    f2 = a1f::mul(f16, f17);
    at = 0x40C00000u;
    f1 = a1_flt(at);
    f0 = a1f::div(fcr31, f0, f17);
    f1 = ((float)(f13) * (float)(f1));
    f0 = a1f::mul(f0, f15);
    f0 = a1f::subFinite(f0, f1);
    f0 = a1f::addFinite(f0, f2);
    f0 = a1f::div(fcr31, f0, f17);
    {
        return f0;
    }
}

// AI_GetTimeFromAccelC3 (0x13d4540): typed by tools/typed_lift.py (round A1)
inline float AI_GetTimeFromAccelC3(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, float f12, float f13, float f14, float f15)
{
    uint32_t at = 0;
    float f0 = 0.0f;
    float f1 = 0.0f;
    float f2 = 0.0f;
    uint32_t fcr31 = 0;
    at = 0x3DCC0000u;
    at = at | 0xCCCCu;
    f0 = a1_flt(at);
    fcr31 = (a1f::lt(f15, f0)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
    {
        const bool c = (fcr31 & 0x800000u) == 0u;
        f13 = a1f::add(f13, f12);
        if (!c) {
            f15 = ((float)(f0));
        }
    }
    f1 = a1_flt(0u);
    fcr31 = (a1f::lt(f14, f1)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
    {
        const bool c = (fcr31 & 0x800000u) == 0u;
        f0 = ((float)(f1));
        if (!c) {
            f14 = (-(float)(f14));
        }
    }
    at = 0x41100000u;
    f2 = a1_flt(at);
    at = 0x41400000u;
    f1 = a1_flt(at);
    f2 = a1f::mul(f13, f2);
    f1 = a1f::mul(f15, f1);
    f2 = a1f::mul(f2, f13);
    f1 = a1f::mul(f1, f14);
    f2 = a1f::addFinite(f2, f1);
    fcr31 = (a1f::lt(f2, f0)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
    if ((fcr31 & 0x800000u) == 0u) {
        f1 = a1f::sqrt(fcr31, f2);
        at = 0xC0400000u;
        f0 = a1_flt(at);
        f0 = a1f::mul(f13, f0);
        f0 = a1f::addFinite(f0, f1);
        f0 = a1f::div(fcr31, f0, f15);
    }
    {
        return f0;
    }
}

// AI_GetVelocity3 (0x13d45f0): typed by tools/typed_lift.py (round A1)
inline float AI_GetVelocity3(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, float f12, float f13, float f14, float f15, float f16)
{
    uint32_t at = 0;
    float f0 = 0.0f;
    float f1 = 0.0f;
    uint32_t fcr31 = 0;
    at = 0x40400000u;
    f1 = a1_flt(at);
    f14 = a1f::mul(f14, f16);
    at = 0x3F000000u;
    f0 = a1_flt(at);
    f12 = a1f::div(fcr31, f12, f1);
    f13 = a1f::mul(f13, f0);
    f13 = a1f::mul(f13, f16);
    f12 = a1f::mul(f12, f16);
    f13 = a1f::mul(f13, f16);
    f12 = a1f::mul(f12, f16);
    f12 = a1f::mul(f12, f16);
    f0 = a1f::add(f15, f12);
    f0 = a1f::addFinite(f0, f13);
    {
        f0 = a1f::add(f0, f14);
        return f0;
    }
}

// AI_GetAreaDistScale (0x13d4ce0): typed by tools/typed_lift.py (round A1)
inline float AI_GetAreaDistScale(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, float f12, float f13)
{
    float f0 = 0.0f;
    uint32_t fcr31 = 0;
    fcr31 = (a1f::lt(f12, f13)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
    {
        const bool c = (fcr31 & 0x800000u) != 0u;
        f0 = ((float)(f12));
        if (!c) {
            f0 = ((float)(f13));
        }
    }
    f0 = a1f::div(fcr31, f0, f12);
    {
        f0 = a1f::mul(f0, f0);
        return f0;
    }
}

// AI_GetAreaDistScale2 (0x13d4d08): typed by tools/typed_lift.py (round A1)
inline float AI_GetAreaDistScale2(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, float f12, float f13)
{
    float f0 = 0.0f;
    uint32_t fcr31 = 0;
    fcr31 = (a1f::lt(f12, f13)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
    {
        const bool c = (fcr31 & 0x800000u) != 0u;
        f0 = ((float)(f12));
        if (!c) {
            f0 = ((float)(f13));
        }
    }
    f0 = a1f::div(fcr31, f0, f12);
    {
        return f0;
    }
}

// AI_GetNegaVector (0x13d5300): typed by tools/typed_lift.py (round A1)
inline float AI_GetNegaVector(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t a0, uint32_t a1)
{
    float f0 = 0.0f;
    float f1 = 0.0f;
    f0 = a1_flt(A1_R32(a1));
    f0 = (-(float)(f0));
    A1_W32(a0, a1_bits(f0));
    f1 = a1_flt(A1_R32B(a1, 4));
    f1 = (-(float)(f1));
    A1_W32B(a0, 4, a1_bits(f1));
    f0 = a1_flt(A1_R32B(a1, 8));
    f0 = (-(float)(f0));
    A1_W32B(a0, 8, a1_bits(f0));
    f1 = a1_flt(A1_R32B(a1, 12));
    {
        A1_W32B(a0, 12, a1_bits(f1));
        return f0;
    }
}

// Kite_SetAnimalID_KitePathWork (0x13e7c00): typed by tools/typed_lift.py (round A1)
inline void Kite_SetAnimalID_KitePathWork(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t a0, uint32_t a1)
{
    {
        A1_W32(a0, a1);
        
        return;
    }
}

// Kite_SetEventIDList (0x13e7c08): typed by tools/typed_lift.py (round A1)
inline uint32_t Kite_SetEventIDList(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t a0, uint32_t a1, uint64_t a2)
{
    uint32_t v0 = 0;
    (v0 = (uint32_t)((int32_t)((int32_t)SLL32((uint32_t)a2, 2))));
    (a2 = (uint64_t)(((uint64_t)a2 < (uint64_t)(int64_t)(int32_t)10) ? 1 : 0));
    {
        const bool c = (int64_t)a2 == 0;
        v0 = v0 + a0;
        if (!c) {
            A1_W32B(v0, 444, a1);
        }
    }
    {
        return v0;
    }
}

// Kite_InitEventCheck (0x13e7c28): typed by tools/typed_lift.py (round A1)
inline uint32_t Kite_InitEventCheck(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t a0)
{
    uint32_t v0 = 0;
    v0 = 0xFFFFFFFFu;
    A1_W32B(a0, 12, 0u);
    A1_W32(a0, v0);
    A1_W32B(a0, 4, 0u);
    {
        A1_W32B(a0, 8, 0u);
        return v0;
    }
}

// Kite_ResetEventFrameCheck (0x13e7c40): typed by tools/typed_lift.py (round A1)
inline void Kite_ResetEventFrameCheck(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t a0)
{
    A1_W32B(a0, 12, 0u);
    {
        A1_W32B(a0, 8, 0u);
        
        return;
    }
}

// Kite_StartEventCheck (0x13e7c50): typed by tools/typed_lift.py (round A1)
inline uint32_t Kite_StartEventCheck(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t a0, uint32_t a1)
{
    uint32_t v0 = 0;
    v0 = 1u;
    A1_W32(a0, a1);
    A1_W32B(a0, 8, v0);
    {
        A1_W32B(a0, 4, 0u);
        return v0;
    }
}

// Kite_EndEventCheck (0x13e7c68): typed by tools/typed_lift.py (round A1)
inline uint32_t Kite_EndEventCheck(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t a0)
{
    uint32_t v0 = 0;
    v0 = 1u;
    A1_W32B(a0, 12, v0);
    {
        A1_W32B(a0, 4, v0);
        return v0;
    }
}

// Kite_SucceedKitePathWork (0x13e7cf8): typed by tools/typed_lift.py (round A1)
inline uint64_t Kite_SucceedKitePathWork(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t a0, uint32_t a1)
{
    uint64_t v0 = 0;
    uint64_t v1 = 0;
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
    A1_W64B(a0, 56, v1);
    (v0 = (uint64_t)(A1_R64B(a1, 64)));
    A1_W64B(a0, 64, v0);
    (v1 = (uint64_t)(A1_R64B(a1, 72)));
    A1_W64B(a0, 72, v1);
    (v0 = (uint64_t)(A1_R64B(a1, 80)));
    A1_W64B(a0, 80, v0);
    (v1 = (uint64_t)(A1_R64B(a1, 88)));
    A1_W64B(a0, 88, v1);
    (v0 = (uint64_t)(A1_R64B(a1, 96)));
    A1_W64B(a0, 96, v0);
    (v1 = (uint64_t)(A1_R64B(a1, 104)));
    A1_W64B(a0, 104, v1);
    (v0 = (uint64_t)(A1_R64B(a1, 112)));
    A1_W64B(a0, 112, v0);
    (v1 = (uint64_t)(A1_R64B(a1, 120)));
    A1_W64B(a0, 120, v1);
    (v0 = (uint64_t)(A1_R64B(a1, 128)));
    A1_W64B(a0, 128, v0);
    (v1 = (uint64_t)(A1_R64B(a1, 136)));
    A1_W64B(a0, 136, v1);
    (v0 = (uint64_t)(A1_R64B(a1, 144)));
    A1_W64B(a0, 144, v0);
    (v1 = (uint64_t)(A1_R64B(a1, 152)));
    A1_W64B(a0, 152, v1);
    (v0 = (uint64_t)(A1_R64B(a1, 160)));
    A1_W64B(a0, 160, v0);
    (v1 = (uint64_t)(A1_R64B(a1, 168)));
    A1_W64B(a0, 168, v1);
    (v0 = (uint64_t)(A1_R64B(a1, 176)));
    A1_W64B(a0, 176, v0);
    (v1 = (uint64_t)(A1_R64B(a1, 184)));
    A1_W64B(a0, 184, v1);
    (v0 = (uint64_t)(A1_R64B(a1, 192)));
    A1_W64B(a0, 192, v0);
    (v1 = (uint64_t)(A1_R64B(a1, 200)));
    {
        A1_W64B(a0, 200, v1);
        return v0;
    }
}

// setSmallManagerSystemLock (0x13f0f90): typed by tools/typed_lift.py (round A1)
inline uint32_t setSmallManagerSystemLock(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t a0)
{
    uint32_t v0 = 0;
    v0 = 0x1200000u;
    {
        A1_W32(v0 - 0x7AE0u, a0);
        return v0;
    }
}

// gamecore_sub_013F1190 (0x13f1190): typed by tools/typed_lift.py (round A1)
inline uint32_t gamecore_sub_013F1190(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t a0)
{
    uint32_t v0 = 0;
    v0 = 0xFFFFFFFFu;
    A1_W32(a0, 0u);
    A1_W16B(a0, 82, (uint16_t)v0);
    A1_W32B(a0, 4, 0u);
    A1_W32B(a0, 8, 0u);
    A1_W32B(a0, 12, v0);
    A1_W32B(a0, 16, 0u);
    A1_W32B(a0, 84, 0u);
    {
        A1_W16B(a0, 80, (uint16_t)v0);
        return v0;
    }
}

// gamecore_sub_013F1938 (0x13f1938): typed by tools/typed_lift.py (round A1)
inline a1::RetVF gamecore_sub_013F1938(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t a0, uint32_t a1)
{
    uint32_t at = 0;
    uint32_t v1 = 0;
    uint64_t v0 = 0;
    float f0 = 0.0f;
    float f1 = 0.0f;
    float f2 = 0.0f;
    uint32_t fcr31 = 0;
    bool df_at = false;
    v1 = A1_R32B(a0, 580);
    if (v1 == 0) {
        f0 = a1_flt(A1_R32B(a1, 72));
    } else {
        f1 = a1_flt(A1_R32B(a0, 500));
        f0 = a1_flt(0u);
        fcr31 = (a1f::lt(f1, f0)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
        if ((fcr31 & 0x800000u) != 0u) {
            at = 0xBF350000u; df_at = true;
            at = at | 0x4F3u; df_at = true;
            f0 = a1_flt(at);
            fcr31 = (a1f::le(f1, f0)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
            if ((fcr31 & 0x800000u) != 0u) {
                (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 1))));
                goto L_13f1994;
            }
            (v0 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL));
            goto L_13f1994;
        }
        at = 0x3F350000u; df_at = true;
        at = at | 0x4F3u; df_at = true;
        f0 = a1_flt(at);
        fcr31 = (a1f::le(f0, f1)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
        if ((fcr31 & 0x800000u) == 0u) {
            (v0 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL));
        } else {
            (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 1))));
        }
L_13f1994:
        {
            const bool c = (int64_t)v0 == 0;
            (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 1))));
            if (c) {
                goto L_13f19cc;
            }
        }
        f1 = a1_flt(A1_R32B(a0, 452));
        f0 = a1_flt(0u);
        fcr31 = (a1f::lt(f1, f0)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
        goto L_13f19c4;
    }
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 1))));
    f1 = a1_flt(A1_R32B(a0, 52));
    { uint32_t bits = 0u; std::memcpy(&f2, &bits, sizeof(bits)); }
    f0 = a1f::add(f0, f1);
    fcr31 = (a1f::le(f0, f2)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
L_13f19c4:
    if ((fcr31 & 0x800000u) != 0u) {
        goto L_13f19d0;
    }
L_13f19cc:
    (v0 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL));
L_13f19d0:
    if ((int64_t)v0 != 0) {
        (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 1))));
    } else {
        if (v1 == 0) {
            f0 = a1_flt(A1_R32B(a1, 72));
        } else {
            f1 = a1_flt(A1_R32B(a0, 500));
            f0 = a1_flt(0u);
            fcr31 = (a1f::lt(f1, f0)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
            if ((fcr31 & 0x800000u) != 0u) {
                at = 0xBF350000u; df_at = true;
                at = at | 0x4F3u; df_at = true;
                f0 = a1_flt(at);
                fcr31 = (a1f::le(f1, f0)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
                if ((fcr31 & 0x800000u) != 0u) {
                    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 1))));
                    goto L_13f1a34;
                }
                (v0 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL));
                goto L_13f1a34;
            }
            at = 0x3F350000u; df_at = true;
            at = at | 0x4F3u; df_at = true;
            f0 = a1_flt(at);
            fcr31 = (a1f::le(f0, f1)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
            if ((fcr31 & 0x800000u) == 0u) {
                (v0 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL));
            } else {
                (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 1))));
            }
L_13f1a34:
            {
                const bool c = (int64_t)v0 == 0;
                v1 = 1u;
                if (c) {
                    goto L_13f1a6c;
                }
            }
            f1 = a1_flt(A1_R32B(a0, 452));
            f0 = a1_flt(0u);
            fcr31 = (a1f::lt(f0, f1)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
            goto L_13f1a64;
        }
        v1 = 1u;
        f1 = a1_flt(A1_R32B(a0, 52));
        { uint32_t bits = 0u; std::memcpy(&f2, &bits, sizeof(bits)); }
        f0 = a1f::sub(f0, f1);
        fcr31 = (a1f::le(f2, f0)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
L_13f1a64:
        if ((fcr31 & 0x800000u) != 0u) {
            goto L_13f1a70;
        }
L_13f1a6c:
        v1 = 0u;
L_13f1a70:
        {
            const bool c = v1 == 0;
            (v0 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL));
            if (!c) {
                (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 1))));
            }
        }
    }
    {
        return a1::RetVF{(uint64_t)v0, f0};
    }
}

// getNowExecEventScriptObj (0x13f62c8): typed by tools/typed_lift.py (round A1)
inline uint32_t getNowExecEventScriptObj(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt)
{
    uint32_t v0 = 0;
    uint32_t v1 = 0;
    v1 = 0x1460000u;
    {
        v0 = A1_R32B(v1, 27404);
        return v0;
    }
}

// setEventScriptEnable (0x13f62f0): typed by tools/typed_lift.py (round A1)
inline uint32_t setEventScriptEnable(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t a0)
{
    uint32_t v0 = 0;
    v0 = 0x1460000u;
    {
        A1_W32B(v0, 27416, a0);
        return v0;
    }
}

// setPlayerStatusSwitch (0x13fa530): typed by tools/typed_lift.py (round A1)
inline uint32_t setPlayerStatusSwitch(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t a0)
{
    uint32_t v0 = 0;
    v0 = 0x1460000u;
    {
        A1_W32B(v0, 27432, a0);
        return v0;
    }
}

// startAttackEffect (0x13fa628): typed by tools/typed_lift.py (round A1)
inline a1::RetVF startAttackEffect(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, float f12)
{
    uint32_t at = 0;
    uint32_t v0 = 0;
    uint32_t v1 = 0;
    float f0 = 0.0f;
    float f1 = 0.0f;
    v1 = 0x1460000u;
    at = 0x3F000000u;
    f0 = a1_flt(at);
    f1 = a1_flt(A1_R32B(v1, 22472));
    v1 = 1u;
    v0 = 0x1200000u;
    v0 = v0 - 0x79F8u;
    A1_W32B(v0, 12, a1_bits(f12));
    A1_W32(v0, v1);
    A1_W32B(v0, 8, a1_bits(f0));
    A1_W32B(v0, 16, a1_bits(f1));
    {
        A1_W32B(v0, 4, a1_bits(f0));
        return a1::RetVF{(uint64_t)a1_sx64(v0), f0};
    }
}

// bosslifebar_display (0x13fa930): typed by tools/typed_lift.py (round A1)
inline a1::RetVF bosslifebar_display(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt)
{
    uint64_t v0 = 0;
    uint64_t v1 = 0;
    float f0 = 0.0f;
    float f1 = 0.0f;
    uint32_t fcr31 = 0;
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)326 << 16))));
    f1 = a1_flt(0u);
    { uint32_t bits = A1_R32B((uint32_t)v0, 27560); float f; std::memcpy(&f, &bits, sizeof(f)); f0 = f; }
    fcr31 = (a1f::lt(f1, f0)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
    {
        const bool c = (fcr31 & 0x800000u) != 0u;
        (v1 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL));
        if (!c) {
            (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)326 << 16))));
            { uint32_t bits = A1_R32B((uint32_t)v0, 27544); float f; std::memcpy(&f, &bits, sizeof(f)); f0 = f; }
            fcr31 = (a1f::lt(f1, f0)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
            if ((fcr31 & 0x800000u) == 0u) {
                goto L_13fa960;
            }
        }
    }
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 1))));
L_13fa960:
    {
        (v0 = (uint64_t)((uint64_t)RC_U64X(v1) + (uint64_t)0ULL));
        return a1::RetVF{(uint64_t)v0, f0};
    }
}

// bosslifebar_resethp (0x13fa968): typed by tools/typed_lift.py (round A1)
inline a1::RetVF bosslifebar_resethp(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, float f12, float f13)
{
    uint32_t at = 0;
    uint32_t v0 = 0;
    uint32_t v1 = 0;
    float f0 = 0.0f;
    uint32_t fcr31 = 0;
    f13 = a1f::div(fcr31, f13, f12);
    at = 0x3C230000u;
    at = at | 0xD70Au;
    f0 = a1_flt(at);
    v0 = 0x1460000u;
    v1 = 0x1460000u;
    f12 = a1f::mul(f12, f0);
    v0 = v0 + 0x6B90u;
    A1_W32B(v0, 12, 0u);
    A1_W32B(v1, 27556, a1_bits(f12));
    A1_W32(v0, a1_bits(f13));
    A1_W32B(v0, 8, a1_bits(f13));
    {
        A1_W32B(v0, 4, a1_bits(f13));
        return a1::RetVF{(uint64_t)a1_sx64(v0), f0};
    }
}

// PlayerSprStatusEnd (0x13fab00): typed by tools/typed_lift.py (round A1)
inline uint32_t PlayerSprStatusEnd(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt)
{
    uint32_t v0 = 0;
    v0 = 0x1460000u;
    {
        A1_W32B(v0, 27564, 0u);
        return v0;
    }
}

// BossSprStatusEnd (0x13fab68): typed by tools/typed_lift.py (round A1)
inline uint32_t BossSprStatusEnd(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt)
{
    uint32_t v0 = 0;
    v0 = 0x1460000u;
    {
        A1_W32B(v0, 27568, 0u);
        return v0;
    }
}

// GetSwordFlareHitPlaneAttribute (0x13ff148): typed by tools/typed_lift.py (round A1)
inline uint64_t GetSwordFlareHitPlaneAttribute(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt)
{
    uint32_t v1 = 0;
    uint64_t v0 = 0;
    v1 = 0x1460000u;
    {
        (v0 = (uint64_t)(A1_R64B(v1, 27768)));
        return v0;
    }
}

// getSelectWeapon (0x1407310): typed by tools/typed_lift.py (round A1)
inline uint32_t getSelectWeapon(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt)
{
    uint32_t v0 = 0;
    uint32_t v1 = 0;
    v1 = 0x1460000u;
    {
        v0 = A1_R32B(v1, 28272);
        return v0;
    }
}

// getSprLogWorkAddress (0x140ae40): typed by tools/typed_lift.py (round A1)
inline uint32_t getSprLogWorkAddress(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt)
{
    uint32_t v0 = 0;
    v0 = 0x1200000u;
    {
        v0 = v0 - 0x77D0u;
        return v0;
    }
}

// getSprLogStatusAddress (0x140ae58): typed by tools/typed_lift.py (round A1)
inline uint32_t getSprLogStatusAddress(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt)
{
    uint32_t v0 = 0;
    v0 = 0x1460000u;
    {
        v0 = v0 + 0x6EB0u;
        return v0;
    }
}

// initPauseMenuMap (0x140af68): typed by tools/typed_lift.py (round A1)
inline uint32_t initPauseMenuMap(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt)
{
    uint32_t v0 = 0;
    v0 = 0x1460000u;
    v0 = v0 + 0x6EB0u;
    A1_W32B(v0, 8, 0u);
    A1_W32(v0, 0u);
    {
        A1_W32B(v0, 4, 0u);
        return v0;
    }
}

// setLoadMapOffset (0x140afb0): typed by tools/typed_lift.py (round A1)
inline uint32_t setLoadMapOffset(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t a0, uint32_t a1, float f12, float f13)
{
    uint32_t v0 = 0;
    uint32_t v1 = 0;
    uint32_t a2 = 0;
    uint32_t a3 = 0;
    v0 = 0x1460000u;
    v1 = 0x1460000u;
    a2 = 0x1460000u;
    a3 = 0x1460000u;
    A1_W32B(v0, 28376, a1_bits(f12));
    A1_W32B(v1, 28380, a1_bits(f13));
    A1_W32B(a2, 28384, a0);
    {
        A1_W32B(a3, 28388, a1);
        return v0;
    }
}

// isInLoadWait (0x140b0c0): typed by tools/typed_lift.py (round A1)
inline uint32_t isInLoadWait(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt)
{
    uint32_t v0 = 0;
    uint32_t v1 = 0;
    v1 = 0x1460000u;
    v0 = A1_R32B(v1, 28472);
    {
        v0 = v0 < 1u ? 1u : 0u;
        return v0;
    }
}

// LoadWaitCancel (0x140b0d0): typed by tools/typed_lift.py (round A1)
inline uint32_t LoadWaitCancel(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t a0)
{
    uint32_t v0 = 0;
    uint32_t v1 = 0;
    v0 = 0x1460000u;
    v1 = 0x1460000u;
    A1_W32B(v0, 28476, a0);
    {
        A1_W32B(v1, 28468, 0u);
        return v0;
    }
}

// acted_camera_warp (0x140ee50): typed by tools/typed_lift.py (round A1)
inline uint64_t acted_camera_warp(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint64_t a0)
{
    uint64_t v0 = 0;
    (v0 = (uint64_t)(a0 ^ (uint64_t)(uint16_t)1));
    {
        (v0 = (uint64_t)(((uint64_t)v0 < (uint64_t)(int64_t)(int32_t)1) ? 1 : 0));
        return v0;
    }
}

// gamecore_sub_0140FE00 (0x140fe00): typed by tools/typed_lift.py (round A1)
inline float gamecore_sub_0140FE00(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t a0, uint32_t a1, uint32_t a2)
{
    float f0 = 0.0f;
    float f1 = 0.0f;
    a2 = a2 << 2;
    a1 = a2 + a1;
    a2 = a2 + a0;
    f0 = a1_flt(A1_R32(a2));
    f1 = a1_flt(A1_R32(a1));
    f0 = a1f::sub(f0, f1);
    {
        f0 = a1f::mul(f0, f0);
        return f0;
    }
}

// gamecore_sub_0140FE20 (0x140fe20): typed by tools/typed_lift.py (round A1)
inline float gamecore_sub_0140FE20(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t a0, uint32_t a1, uint32_t a2, uint32_t a3)
{
    float f0 = 0.0f;
    float f1 = 0.0f;
    float f2 = 0.0f;
    a3 = a3 << 2;
    a0 = a3 + a0;
    a1 = a3 + a1;
    f1 = a1_flt(A1_R32(a1));
    a3 = a3 + a2;
    f0 = a1_flt(A1_R32(a0));
    f2 = a1_flt(A1_R32(a3));
    f0 = a1f::sub(f0, f1);
    f1 = a1f::sub(f1, f2);
    f0 = a1f::addFinite(f0, f0);
    {
        f0 = a1f::mul(f0, f1);
        return f0;
    }
}

// gamecore_sub_0140FE50 (0x140fe50): typed by tools/typed_lift.py (round A1)
inline float gamecore_sub_0140FE50(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t a0, uint32_t a1, uint32_t a2)
{
    float f0 = 0.0f;
    float f1 = 0.0f;
    a2 = a2 << 2;
    a1 = a2 + a1;
    a2 = a2 + a0;
    f0 = a1_flt(A1_R32(a2));
    f1 = a1_flt(A1_R32(a1));
    f0 = a1f::sub(f0, f1);
    {
        f0 = a1f::mul(f0, f0);
        return f0;
    }
}

// gamecore_sub_0140FE70 (0x140fe70): typed by tools/typed_lift.py (round A1)
inline a1::RetVF gamecore_sub_0140FE70(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, float f12)
{
    uint32_t at = 0;
    uint64_t v0 = 0;
    float f0 = 0.0f;
    uint32_t fcr31 = 0;
    bool df_at = false;
    f0 = a1_flt(0u);
    fcr31 = (a1f::le(f0, f12)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
    {
        const bool c = (fcr31 & 0x800000u) == 0u;
        (v0 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL));
        if (!c) {
            at = 0x3F800000u; df_at = true;
            f0 = a1_flt(at);
            fcr31 = (a1f::lt(f12, f0)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
            {
                const bool c = (fcr31 & 0x800000u) != 0u;
                (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 1))));
                if (!c) {
                    (v0 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL));
                }
            }
        }
    }
    {
        return a1::RetVF{(uint64_t)v0, f0};
    }
}

// reset_time_interp_param_camera_dist_to_look_at (0x1411448): typed by tools/typed_lift.py (round A1)
inline float reset_time_interp_param_camera_dist_to_look_at(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t a0)
{
    uint32_t at = 0;
    float f0 = 0.0f;
    at = 0x3F800000u;
    f0 = a1_flt(at);
    A1_W32B(a0, 112, 0u);
    A1_W32B(a0, 120, a1_bits(f0));
    {
        A1_W32B(a0, 116, 0u);
        return f0;
    }
}

// is_acting_rewind_camera_dist_to_look_at (0x1411480): typed by tools/typed_lift.py (round A1)
inline uint32_t is_acting_rewind_camera_dist_to_look_at(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t a0)
{
    uint32_t v0 = 0;
    v0 = A1_R32B(a0, 48);
    v0 = v0 ^ 2u;
    {
        v0 = v0 < 1u ? 1u : 0u;
        return v0;
    }
}

// get_rewind_begin_dist_to_target_camera_dist_to_look_at (0x1411490): typed by tools/typed_lift.py (round A1)
inline a1::RetVF get_rewind_begin_dist_to_target_camera_dist_to_look_at(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t a0, float f12)
{
    uint32_t v0 = 0;
    float f0 = 0.0f;
    v0 = A1_R32B(a0, 48);
    v0 = v0 ^ 2u;
    {
        const bool c = v0 != 0;
        f0 = ((float)(f12));
        if (!c) {
            f0 = a1_flt(A1_R32B(a0, 56));
        }
    }
    {
        return a1::RetVF{(uint64_t)a1_sx64(v0), f0};
    }
}

// end_rewind_camera_dist_to_look_at (0x14114b0): typed by tools/typed_lift.py (round A1)
inline void end_rewind_camera_dist_to_look_at(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t a0)
{
    {
        A1_W32B(a0, 48, 0u);
        
        return;
    }
}

// is_leave_camera_dist_to_look_at (0x14114b8): typed by tools/typed_lift.py (round A1)
inline uint32_t is_leave_camera_dist_to_look_at(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t a0)
{
    uint32_t v0 = 0;
    uint32_t v1 = 0;
    v1 = A1_R32(a0);
    v0 = A1_R32B(v1, 320);
    v0 = v0 ^ 2u;
    {
        v0 = v0 < 1u ? 1u : 0u;
        return v0;
    }
}

// request_end_camera_pause_rise (0x1412a60): typed by tools/typed_lift.py (round A1)
inline uint32_t request_end_camera_pause_rise(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t a0)
{
    uint32_t v0 = 0;
    v0 = 2u;
    A1_W32B(a0, 4, 0u);
    {
        A1_W32(a0, v0);
        return v0;
    }
}

// is_end_camera_pause_rise (0x1412a70): typed by tools/typed_lift.py (round A1)
inline uint32_t is_end_camera_pause_rise(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t a0)
{
    uint32_t v0 = 0;
    v0 = A1_R32(a0);
    {
        v0 = v0 < 1u ? 1u : 0u;
        return v0;
    }
}

// get_begin_interp_finish_time_camera_pause_rise (0x1412a80): typed by tools/typed_lift.py (round A1)
inline a1::RetVF get_begin_interp_finish_time_camera_pause_rise(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt)
{
    uint32_t v0 = 0;
    float f0 = 0.0f;
    v0 = 0x1470000u;
    {
        f0 = a1_flt(A1_R32(v0 - 0x3C50u));
        return a1::RetVF{(uint64_t)a1_sx64(v0), f0};
    }
}

// get_place_status_camera_place_status (0x1413340): typed by tools/typed_lift.py (round A1)
inline uint32_t get_place_status_camera_place_status(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t a0)
{
    uint32_t v0 = 0;
    {
        v0 = A1_R32(a0);
        return v0;
    }
}

// get_manage_obj_camera_place_status (0x1413348): typed by tools/typed_lift.py (round A1)
inline uint32_t get_manage_obj_camera_place_status(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t a0)
{
    uint32_t v0 = 0;
    {
        v0 = A1_R32B(a0, 4);
        return v0;
    }
}

// get_hit_camera_region_camera_place_status (0x1413408): typed by tools/typed_lift.py (round A1)
inline uint32_t get_hit_camera_region_camera_place_status(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t a0)
{
    uint32_t v0 = 0;
    {
        v0 = A1_R32B(a0, 12);
        return v0;
    }
}

// get_on_boss_camera_col_attr_camera_place_status (0x14134b8): typed by tools/typed_lift.py (round A1)
inline uint64_t get_on_boss_camera_col_attr_camera_place_status(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t a0, uint32_t a1)
{
    uint64_t v0 = 0;
    a1 = a1 + 0x150u;
    (v0 = (uint64_t)(A1_R64B(a1, 16)));
    A1_W64B(a0, 0, v0);
    {
        (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B(a1, 8))));
        return v0;
    }
}

// is_approach_rise_ok_camera_place_status (0x14134d0): typed by tools/typed_lift.py (round A1)
inline uint64_t is_approach_rise_ok_camera_place_status(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint64_t a0)
{
    uint64_t v0 = 0;
    uint64_t v1 = 0;
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)a0, 208))));
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 1))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)a0, 8))));
    {
        const bool c = (int64_t)v0 != (int64_t)v1;
        (a0 = (uint64_t)(A1_R64B((uint32_t)a0, 16)));
        if (!c) {
            (v0 = (uint64_t)(0ULL | (uint64_t)(uint16_t)61440));
            (v0 = (uint64_t)(RC_U64X(v0) << 30));
            (a0 = (uint64_t)(a0 & v0));
            goto L_14134fc;
        }
    }
    (v1 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL));
L_14134fc:
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 1))));
    if ((int64_t)v1 == (int64_t)v0) {
        (v0 = (uint64_t)(0ULL | (uint64_t)(uint16_t)49152));
        (v0 = (uint64_t)(RC_U64X(v0) << 28));
        if ((int64_t)a0 != (int64_t)v0) {
            (v0 = (uint64_t)(0ULL | (uint64_t)(uint16_t)49152));
            (v0 = (uint64_t)(RC_U64X(v0) << 29));
            if ((int64_t)a0 != (int64_t)v0) {
                goto L_1413530;
            }
        }
        {
            (v0 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL));
            return v0;
        }
    }
L_1413530:
    {
        (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 1))));
        return v0;
    }
}

// is_cliff_rise_delay_camera_place_status (0x14135b8): typed by tools/typed_lift.py (round A1)
inline uint64_t is_cliff_rise_delay_camera_place_status(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint64_t a0)
{
    uint64_t v0 = 0;
    uint64_t v1 = 0;
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)a0, 208))));
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 1))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)a0, 8))));
    {
        const bool c = (int64_t)v0 != (int64_t)v1;
        (a0 = (uint64_t)(A1_R64B((uint32_t)a0, 16)));
        if (!c) {
            (v0 = (uint64_t)(0ULL | (uint64_t)(uint16_t)61440));
            (v0 = (uint64_t)(RC_U64X(v0) << 30));
            (a0 = (uint64_t)(a0 & v0));
            goto L_14135e4;
        }
    }
    (v1 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL));
L_14135e4:
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 1))));
    if ((int64_t)v1 != (int64_t)v0) {
        (v0 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL));
    } else {
        (v1 = (uint64_t)(0ULL | (uint64_t)(uint16_t)40960));
        (v1 = (uint64_t)(RC_U64X(v1) << 30));
        if ((int64_t)a0 != (int64_t)v1) {
            (v0 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL));
        } else {
        }
    }
    {
        return v0;
    }
}

// act_horse_look_at_ofst_decrease_camera_place_status (0x1413668): typed by tools/typed_lift.py (round A1)
inline uint64_t act_horse_look_at_ofst_decrease_camera_place_status(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint64_t a0)
{
    uint64_t v0 = 0;
    uint64_t v1 = 0;
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)a0, 208))));
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 1))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)a0, 8))));
    {
        const bool c = (int64_t)v0 != (int64_t)v1;
        (a0 = (uint64_t)(A1_R64B((uint32_t)a0, 16)));
        if (!c) {
            (v0 = (uint64_t)(0ULL | (uint64_t)(uint16_t)61440));
            (v0 = (uint64_t)(RC_U64X(v0) << 30));
            (a0 = (uint64_t)(a0 & v0));
            goto L_1413694;
        }
    }
    (v1 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL));
L_1413694:
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 1))));
    if ((int64_t)v1 != (int64_t)v0) {
        (v0 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL));
    } else {
        (v1 = (uint64_t)(0ULL | (uint64_t)(uint16_t)36864));
        (v1 = (uint64_t)(RC_U64X(v1) << 30));
        if ((int64_t)a0 != (int64_t)v1) {
            (v0 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL));
        } else {
        }
    }
    {
        return v0;
    }
}

// act_look_at_ofst_cut_camera_place_status (0x14136b8): typed by tools/typed_lift.py (round A1)
inline uint64_t act_look_at_ofst_cut_camera_place_status(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint64_t a0)
{
    uint64_t v0 = 0;
    uint64_t v1 = 0;
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)a0, 208))));
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 1))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)a0, 8))));
    {
        const bool c = (int64_t)v0 != (int64_t)v1;
        (a0 = (uint64_t)(A1_R64B((uint32_t)a0, 16)));
        if (!c) {
            (v0 = (uint64_t)(0ULL | (uint64_t)(uint16_t)61440));
            (v0 = (uint64_t)(RC_U64X(v0) << 30));
            (a0 = (uint64_t)(a0 & v0));
            goto L_14136e4;
        }
    }
    (v1 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL));
L_14136e4:
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 1))));
    if ((int64_t)v1 != (int64_t)v0) {
        (v0 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL));
    } else {
        (v1 = (uint64_t)(0ULL | (uint64_t)(uint16_t)45056));
        (v1 = (uint64_t)(RC_U64X(v1) << 30));
        if ((int64_t)a0 != (int64_t)v1) {
            (v0 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL));
        } else {
        }
    }
    {
        return v0;
    }
}

// gamecore_sub_01413708 (0x1413708): typed by tools/typed_lift.py (round A1)
inline uint32_t gamecore_sub_01413708(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t a0)
{
    uint32_t v0 = 0;
    v0 = 0xFFFFFFFFu;
    A1_W32B(a0, 24, 0u);
    A1_W64B(a0, 192, a1_sx64(v0));
    {
        A1_W32B(a0, 28, 0u);
        return v0;
    }
}

// CAM_get_interp_slope_pitch_ofst (0x1414200): typed by tools/typed_lift.py (round A1)
inline float CAM_get_interp_slope_pitch_ofst(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t a0)
{
    float f0 = 0.0f;
    f0 = a1_flt(A1_R32B(a0, 28));
    {
        f0 = (-(float)(f0));
        return f0;
    }
}

// gamecore_sub_014144D0 (0x14144d0): typed by tools/typed_lift.py (round A1)
inline float gamecore_sub_014144D0(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, float f12, float f13, float f14)
{
    float f0 = 0.0f;
    float f1 = 0.0f;
    uint32_t fcr31 = 0;
    f12 = a1f::sub(f12, f13);
    f1 = a1_flt(0u);
    fcr31 = (a1f::lt(f12, f1)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
    if ((fcr31 & 0x800000u) != 0u) {
        f12 = ((float)(f1));
    } else {
    }
    {
        f0 = a1f::mul(f14, f12);
        return f0;
    }
}

// get_attr_camera_target_lost_angle (0x1414e60): typed by tools/typed_lift.py (round A1)
inline uint32_t get_attr_camera_target_lost_angle(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t a0)
{
    uint32_t v0 = 0;
    {
        v0 = A1_R32(a0);
        return v0;
    }
}

// set_parameter_camera_target_visible_angle_manager (0x1415900): typed by tools/typed_lift.py (round A1)
inline a1::RetVF set_parameter_camera_target_visible_angle_manager(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t a0, uint32_t a1, float f12, float f13)
{
    uint32_t at = 0;
    uint32_t v0 = 0;
    uint32_t v1 = 0;
    uint32_t a2 = 0;
    float f0 = 0.0f;
    float f1 = 0.0f;
    uint32_t fcr31 = 0;
    v0 = a1 >> 31;
    v1 = a1 - 1u;
    v0 = a1 + v0;
    a2 = a1 + 1u;
    v0 = (uint32_t)((int32_t)v0 >> 1);
    v0 = v0 << 1;
    {
        const bool c = v0 != v1;
        A1_W32B(a0, 988, a1);
        if (!c) {
            A1_W32B(a0, 988, a2);
        }
    }
    v1 = A1_R32B(a0, 988);
    v0 = (int32_t)v1 < 21 ? 1u : 0u;
    {
        const bool c = v0 != 0;
        v0 = 0x14u;
        if (!c) {
            v1 = 0x14u;
            A1_W32B(a0, 988, v0);
        }
    }
    at = 0x3C8E0000u;
    at = at | 0xFA35u;
    f0 = a1_flt(at);
    f1 = a1_flt(v1);
    { int32_t tmp; std::memcpy(&tmp, &f1, sizeof(tmp)); f1 = ((float)(int32_t)(tmp)); }
    A1_W32B(a0, 996, a1_bits(f13));
    f0 = a1f::mul(f12, f0);
    f1 = a1f::div(fcr31, f0, f1);
    A1_W32B(a0, 992, a1_bits(f0));
    {
        A1_W32B(a0, 1000, a1_bits(f1));
        return a1::RetVF{(uint64_t)a1_sx64(v0), f0};
    }
}

// end_get_yaw_pitch_camera_target_visible_angle_manager (0x1415a40): typed by tools/typed_lift.py (round A1)
inline void end_get_yaw_pitch_camera_target_visible_angle_manager(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t a0)
{
    {
        A1_W32B(a0, 976, 0u);
        
        return;
    }
}

// gamecore_sub_01415CA0 (0x1415ca0): typed by tools/typed_lift.py (round A1)
inline a1::RetVF gamecore_sub_01415CA0(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t a0, uint32_t a1)
{
    uint32_t v0 = 0;
    uint32_t v1 = 0;
    uint32_t a2 = 0;
    float f0 = 0.0f;
    float f1 = 0.0f;
    uint32_t fcr31 = 0;
    a2 = A1_R32(a0);
    v1 = A1_R32(a1);
    f1 = a1_flt(A1_R32B(a2, 4));
    f0 = a1_flt(A1_R32B(v1, 4));
    fcr31 = (a1f::lt(f0, f1)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
    {
        const bool c = (fcr31 & 0x800000u) != 0u;
        v0 = 1u;
        if (!c) {
            v0 = 0xFFFFFFFFu;
        }
    }
    {
        return a1::RetVF{(uint64_t)a1_sx64(v0), f0};
    }
}

// CAM_copy_camera_base_data (0x141a108): typed by tools/typed_lift.py (round A1)
inline uint64_t CAM_copy_camera_base_data(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t a0, uint32_t a1)
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
    {
        A1_W64B(a0, 40, v1);
        return v0;
    }
}

// CAM_interp_reset_aim_rot_common (0x141a160): typed by tools/typed_lift.py (round A1)
inline uint32_t CAM_interp_reset_aim_rot_common(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t a0)
{
    uint32_t v0 = 0;
    v0 = 1u;
    {
        A1_W32B(a0, 32, v0);
        return v0;
    }
}

// CAM_motion_kind_is_look_around_motion_target (0x141a658): typed by tools/typed_lift.py (round A1)
inline uint32_t CAM_motion_kind_is_look_around_motion_target(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t a0)
{
    uint32_t v0 = 0;
    v0 = A1_R32B(a0, 372);
    v0 = v0 >> 28;
    {
        v0 = v0 & 1u;
        return v0;
    }
}

// CAM_motion_kind_is_accept_auto_rot_motion_target (0x141a678): typed by tools/typed_lift.py (round A1)
inline uint32_t CAM_motion_kind_is_accept_auto_rot_motion_target(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t a0)
{
    uint32_t v0 = 0;
    v0 = A1_R32B(a0, 372);
    v0 = v0 >> 30;
    {
        v0 = v0 & 1u;
        return v0;
    }
}

// CAM_get_target_obj (0x141a7b8): typed by tools/typed_lift.py (round A1)
inline uint32_t CAM_get_target_obj(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t a0)
{
    uint32_t v0 = 0;
    {
        v0 = A1_R32(a0);
        return v0;
    }
}

// CAM_get_target_riding_obj (0x141a7c0): typed by tools/typed_lift.py (round A1)
inline uint32_t CAM_get_target_riding_obj(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t a0)
{
    uint32_t v0 = 0;
    {
        v0 = A1_R32B(a0, 712);
        return v0;
    }
}

// CAM_get_ground_contact_obj (0x141a7c8): typed by tools/typed_lift.py (round A1)
inline uint64_t CAM_get_ground_contact_obj(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t a0)
{
    uint32_t v1 = 0;
    uint64_t v0 = 0;
    v1 = A1_R32B(a0, 712);
    {
        const bool c = v1 != 0;
        (v0 = (uint64_t)((uint64_t)a1_sx64(v1) + (uint64_t)0ULL));
        if (!c) {
            (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B(a0, 0))));
        }
    }
    {
        return v0;
    }
}

// CAM_is_visible_now_target_visibility (0x141aa80): typed by tools/typed_lift.py (round A1)
inline uint32_t CAM_is_visible_now_target_visibility(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t a0)
{
    uint32_t v0 = 0;
    v0 = A1_R32B(a0, 68);
    {
        v0 = v0 < 1u ? 1u : 0u;
        return v0;
    }
}

// CAM_is_return_cam_posi_target_visibility (0x141aa90): typed by tools/typed_lift.py (round A1)
inline uint32_t CAM_is_return_cam_posi_target_visibility(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t a0)
{
    uint32_t v0 = 0;
    {
        v0 = A1_R32B(a0, 304);
        return v0;
    }
}

// CAM_reset_camera_warp_cntrl (0x141aad0): typed by tools/typed_lift.py (round A1)
inline a1::RetVF CAM_reset_camera_warp_cntrl(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t a0)
{
    uint32_t at = 0;
    uint32_t v0 = 0;
    float f0 = 0.0f;
    float f1 = 0.0f;
    v0 = 0x1470000u;
    at = 0x3F800000u;
    f0 = a1_flt(at);
    f1 = a1_flt(A1_R32(v0 - 0x3B60u));
    A1_W32B(a0, 60, a1_bits(f0));
    A1_W32B(a0, 48, a1_bits(f1));
    A1_W32B(a0, 4, 0u);
    A1_W32B(a0, 8, 0u);
    A1_W32B(a0, 16, 0u);
    A1_W32B(a0, 32, 0u);
    A1_W32B(a0, 36, 0u);
    A1_W32B(a0, 40, 0u);
    A1_W32B(a0, 44, 0u);
    A1_W32B(a0, 56, a1_bits(f1));
    {
        A1_W32B(a0, 52, a1_bits(f1));
        return a1::RetVF{(uint64_t)a1_sx64(v0), f0};
    }
}

// CAM_set_invisible_warp_apply_range_camera_warp_cntrl (0x141aca8): typed by tools/typed_lift.py (round A1)
inline void CAM_set_invisible_warp_apply_range_camera_warp_cntrl(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t a0, float f12, float f13)
{
    A1_W32B(a0, 24, a1_bits(f13));
    {
        A1_W32B(a0, 20, a1_bits(f12));
        
        return;
    }
}

// CAM_act_warp_order_camera_warp_cntrl (0x141acb8): typed by tools/typed_lift.py (round A1)
inline uint32_t CAM_act_warp_order_camera_warp_cntrl(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t a0)
{
    uint32_t v0 = 0;
    v0 = 1u;
    {
        A1_W32B(a0, 40, v0);
        return v0;
    }
}

// CAM_is_acting_view_cut_avoid_cntrl (0x141b010): typed by tools/typed_lift.py (round A1)
inline uint32_t CAM_is_acting_view_cut_avoid_cntrl(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t a0)
{
    uint32_t v0 = 0;
    v0 = A1_R32(a0);
    {
        v0 = 0u < v0 ? 1u : 0u;
        return v0;
    }
}

// CAM_set_param_view_cut_avoid_cntrl (0x141b020): typed by tools/typed_lift.py (round A1)
inline void CAM_set_param_view_cut_avoid_cntrl(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t a0, float f12, float f13)
{
    A1_W32B(a0, 152, a1_bits(f13));
    {
        A1_W32B(a0, 148, a1_bits(f12));
        
        return;
    }
}

// CAM_reset_obstacle_avoid_cntrl (0x141b030): typed by tools/typed_lift.py (round A1)
inline void CAM_reset_obstacle_avoid_cntrl(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t a0)
{
    A1_W32B(a0, 16, 0u);
    A1_W32(a0, 0u);
    A1_W32B(a0, 4, 0u);
    A1_W32B(a0, 8, 0u);
    {
        A1_W32B(a0, 12, 0u);
        
        return;
    }
}

// gamecore_sub_0141B428 (0x141b428): typed by tools/typed_lift.py (round A1)
inline float gamecore_sub_0141B428(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, float f12, float f13)
{
    uint32_t at = 0;
    float f0 = 0.0f;
    uint32_t fcr31 = 0;
    fcr31 = (a1f::le(f13, f12)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
    at = 0x3F800000u;
    f0 = a1_flt(at);
    if ((fcr31 & 0x800000u) == 0u) {
        f0 = a1f::div(fcr31, f12, f13);
    }
    {
        return f0;
    }
}

// gamecore_sub_0141B480 (0x141b480): typed by tools/typed_lift.py (round A1)
inline void gamecore_sub_0141B480(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t a0)
{
    {
        A1_W32B(a0, 4, 0u);
        
        return;
    }
}

// gamecore_sub_0141B978 (0x141b978): typed by tools/typed_lift.py (round A1)
inline uint32_t gamecore_sub_0141B978(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t a0)
{
    uint32_t v0 = 0;
    v0 = a0 - 3u;
    {
        v0 = v0 < 2u ? 1u : 0u;
        return v0;
    }
}

// gamecore_sub_0141B988 (0x141b988): typed by tools/typed_lift.py (round A1)
inline uint32_t gamecore_sub_0141B988(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t a0)
{
    uint32_t v0 = 0;
    v0 = a0 - 1u;
    {
        v0 = v0 < 2u ? 1u : 0u;
        return v0;
    }
}

// gamecore_sub_0141BB20 (0x141bb20): typed by tools/typed_lift.py (round A1)
inline a1::RetVF gamecore_sub_0141BB20(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t a0, uint32_t a1, float f12)
{
    uint32_t at = 0;
    uint32_t v0 = 0;
    uint32_t v1 = 0;
    float f0 = 0.0f;
    float f1 = 0.0f;
    float f2 = 0.0f;
    uint32_t fcr31 = 0;
    bool df_v1 = false;
    v0 = A1_R32(a0);
    {
        const bool c = v0 == 0;
        v0 = 1u;
        if (!c) {
            v1 = A1_R32B(a1, 6196); df_v1 = true;
            if (v1 != v0) {
                f0 = a1_flt(A1_R32B(a0, 8));
                goto L_141bb68;
            }
        }
    }
    at = 0x3F000000u;
    f0 = a1_flt(at);
    f1 = a1_flt(A1_R32B(a0, 8));
    f0 = a1f::mul(f12, f0);
    f2 = a1_flt(0u);
    f1 = a1f::sub(f1, f0);
    fcr31 = (a1f::lt(f1, f2)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
    {
        const bool c = (fcr31 & 0x800000u) == 0u;
        A1_W32B(a0, 8, a1_bits(f1));
        if (c) {
            goto L_141bb7c;
        }
    }
    {
        A1_W32B(a0, 8, a1_bits(f2));
        return a1::RetVF{(uint64_t)a1_sx64(v0), f0};
    }
L_141bb68:
    at = 0x3F800000u;
    f1 = a1_flt(at);
    f0 = a1f::add(f0, f12);
    A1_W32B(a0, 4, a1_bits(f1));
    A1_W32B(a0, 8, a1_bits(f0));
L_141bb7c:
    {
        return a1::RetVF{(uint64_t)a1_sx64(v0), f0};
    }
}

// gamecore_sub_0141BCF8 (0x141bcf8): typed by tools/typed_lift.py (round A1)
inline uint32_t gamecore_sub_0141BCF8(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t a1)
{
    uint32_t v0 = 0;
    v0 = A1_R32B(a1, 1380);
    {
        v0 = v0 < 1u ? 1u : 0u;
        return v0;
    }
}

// gamecore_sub_0141C0A8 (0x141c0a8): typed by tools/typed_lift.py (round A1)
inline float gamecore_sub_0141C0A8(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, float f12, float f13, float f14)
{
    float f0 = 0.0f;
    uint32_t fcr31 = 0;
    f0 = a1_flt(0u);
    fcr31 = (a1f::lt(f13, f0)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
    if ((fcr31 & 0x800000u) != 0u) {
        f0 = (-(float)(f13));
        f0 = a1f::div(fcr31, f0, f12);
        goto L_141c0dc;
    }
    f0 = a1f::div(fcr31, f13, f12);
L_141c0dc:
    fcr31 = (a1f::lt(f0, f14)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
    if ((fcr31 & 0x800000u) != 0u) {
        f0 = ((float)(f14));
    } else {
    }
    {
        return f0;
    }
}

// get_view_angle_camera_view_angle_cntrl (0x141c9b0): typed by tools/typed_lift.py (round A1)
inline float get_view_angle_camera_view_angle_cntrl(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t a0)
{
    float f0 = 0.0f;
    {
        f0 = a1_flt(A1_R32B(a0, 28));
        return f0;
    }
}

// get_camera_obj (0x141cd68): typed by tools/typed_lift.py (round A1)
inline uint32_t get_camera_obj(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t a0)
{
    uint32_t v0 = 0;
    {
        v0 = A1_R32(a0);
        return v0;
    }
}

// get_player_collision_group_cameraman (0x141ce48): typed by tools/typed_lift.py (round A1)
inline uint32_t get_player_collision_group_cameraman(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t a0)
{
    uint32_t v0 = 0;
    {
        v0 = A1_R32B(a0, 20);
        return v0;
    }
}

// is_camera_target_change (0x141ce80): typed by tools/typed_lift.py (round A1)
inline uint32_t is_camera_target_change(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t a0)
{
    uint32_t v0 = 0;
    uint32_t v1 = 0;
    v1 = A1_R32B(a0, 24);
    v0 = A1_R32B(a0, 28);
    v0 = v0 ^ v1;
    {
        v0 = 0u < v0 ? 1u : 0u;
        return v0;
    }
}

// is_camera_target_debug_move (0x141ce98): typed by tools/typed_lift.py (round A1)
inline uint64_t is_camera_target_debug_move(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt)
{
    uint64_t v0 = 0;
    {
        (v0 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL));
        return v0;
    }
}

// get_main_cameraman (0x141d370): typed by tools/typed_lift.py (round A1)
inline uint32_t get_main_cameraman(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt)
{
    uint32_t v0 = 0;
    uint32_t v1 = 0;
    v1 = 0x1200000u;
    v1 = v1 - 0x4A00u;
    v0 = A1_R32(v1);
    v0 = v0 << 2;
    v1 = v1 + v0;
    {
        v0 = A1_R32B(v1, 8);
        return v0;
    }
}

// reset_game_camera (0x1424e18): typed by tools/typed_lift.py (round A1)
inline uint32_t reset_game_camera(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t a0)
{
    uint32_t v0 = 0;
    v0 = 5u;
    {
        A1_W32B(a0, 11328, v0);
        return v0;
    }
}

// get_game_camera_cur_mode (0x1424e28): typed by tools/typed_lift.py (round A1)
inline uint32_t get_game_camera_cur_mode(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t a0)
{
    uint32_t v0 = 0;
    {
        v0 = A1_R32B(a0, 4960);
        return v0;
    }
}

// is_reset_or_lws_movie_game_camera (0x1424e30): typed by tools/typed_lift.py (round A1)
inline uint64_t is_reset_or_lws_movie_game_camera(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t a0)
{
    uint32_t v1 = 0;
    uint64_t v0 = 0;
    (a0 = (uint32_t)((int32_t)((int32_t)A1_R32B(a0, 4960))));
    {
        const bool c = a0 == 0;
        v1 = 8u;
        if (!c) {
            {
                const bool c = a0 != v1;
                (v0 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL));
                if (c) {
                    goto L_1424e48;
                }
            }
        }
    }
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 1))));
L_1424e48:
    {
        return v0;
    }
}

// reset_game_camera_standby_flag (0x1425028): typed by tools/typed_lift.py (round A1)
inline void reset_game_camera_standby_flag(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t a0)
{
    {
        A1_W32B(a0, 4, 0u);
        
        return;
    }
}

// set_object_interest_mode_target_game_camera (0x1425030): typed by tools/typed_lift.py (round A1)
inline void set_object_interest_mode_target_game_camera(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t a0, uint32_t a1)
{
    {
        A1_W32B(a0, 12560, a1);
        
        return;
    }
}

// get_object_interest_mode_target_game_camera (0x1425038): typed by tools/typed_lift.py (round A1)
inline uint32_t get_object_interest_mode_target_game_camera(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t a0)
{
    uint32_t v0 = 0;
    {
        v0 = A1_R32B(a0, 12560);
        return v0;
    }
}

// begin_object_interest_game_camera (0x1425040): typed by tools/typed_lift.py (round A1)
inline uint32_t begin_object_interest_game_camera(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t a0)
{
    uint32_t v0 = 0;
    v0 = 1u;
    {
        A1_W32B(a0, 12564, v0);
        return v0;
    }
}

// begin_stream_lws_movie_playing_game_camera (0x14252d8): typed by tools/typed_lift.py (round A1)
inline uint32_t begin_stream_lws_movie_playing_game_camera(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t a0)
{
    uint32_t v0 = 0;
    v0 = 1u;
    {
        A1_W32B(a0, 13684, v0);
        return v0;
    }
}

// end_stream_lws_movie_playing_game_camera (0x14252e8): typed by tools/typed_lift.py (round A1)
inline void end_stream_lws_movie_playing_game_camera(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t a0)
{
    {
        A1_W32B(a0, 13684, 0u);
        
        return;
    }
}

// is_view_angle_exist_lws_anime_mode_base (0x1425548): typed by tools/typed_lift.py (round A1)
inline a1::RetVF is_view_angle_exist_lws_anime_mode_base(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t a0)
{
    uint32_t v0 = 0;
    float f0 = 0.0f;
    float f1 = 0.0f;
    uint32_t fcr31 = 0;
    f1 = a1_flt(A1_R32B(a0, 548));
    f0 = a1_flt(0u);
    fcr31 = (a1f::lt(f0, f1)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
    {
        const bool c = (fcr31 & 0x800000u) != 0u;
        v0 = 1u;
        if (!c) {
            v0 = 0u;
        }
    }
    {
        return a1::RetVF{(uint64_t)a1_sx64(v0), f0};
    }
}

// get_view_angle_lws_anime_mode_base (0x1425568): typed by tools/typed_lift.py (round A1)
inline a1::RetVF get_view_angle_lws_anime_mode_base(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t a0)
{
    uint32_t at = 0;
    uint32_t v0 = 0;
    uint32_t v1 = 0;
    float f0 = 0.0f;
    float f1 = 0.0f;
    uint32_t fcr31 = 0;
    f1 = a1_flt(0u);
    f0 = a1_flt(A1_R32B(a0, 548));
    fcr31 = (a1f::lt(f1, f0)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
    {
        const bool c = (fcr31 & 0x800000u) != 0u;
        v1 = 1u;
        if (!c) {
            v1 = 0u;
        }
    }
    v0 = 1u;
    at = 0xBF800000u;
    f0 = a1_flt(at);
    if (v1 == v0) {
        f0 = a1_flt(A1_R32B(a0, 596));
    } else {
    }
    {
        return a1::RetVF{(uint64_t)a1_sx64(v0), f0};
    }
}

// getBossInterestKey (0x14256a0): typed by tools/typed_lift.py (round A1)
inline uint32_t getBossInterestKey(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt)
{
    uint32_t v0 = 0;
    uint32_t v1 = 0;
    v1 = 0x1460000u;
    {
        v0 = A1_R32B(v1, 29292);
        return v0;
    }
}

// getInterestKey (0x14256b0): typed by tools/typed_lift.py (round A1)
inline uint32_t getInterestKey(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt)
{
    uint32_t v0 = 0;
    uint32_t v1 = 0;
    v1 = 0x1460000u;
    {
        v0 = A1_R32B(v1, 29300);
        return v0;
    }
}

// getZoomKey (0x14256c0): typed by tools/typed_lift.py (round A1)
inline uint32_t getZoomKey(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt)
{
    uint32_t v0 = 0;
    uint32_t v1 = 0;
    v1 = 0x1460000u;
    {
        v0 = A1_R32B(v1, 29304);
        return v0;
    }
}

// setBossInterestKey (0x14256d0): typed by tools/typed_lift.py (round A1)
inline uint32_t setBossInterestKey(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t a0)
{
    uint32_t v0 = 0;
    v0 = 0x1460000u;
    {
        A1_W32B(v0, 29292, a0);
        return v0;
    }
}

// setInterestKey (0x14256e0): typed by tools/typed_lift.py (round A1)
inline uint32_t setInterestKey(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t a0)
{
    uint32_t v0 = 0;
    v0 = 0x1460000u;
    {
        A1_W32B(v0, 29300, a0);
        return v0;
    }
}

// setZoomKey (0x14256f0): typed by tools/typed_lift.py (round A1)
inline uint32_t setZoomKey(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t a0)
{
    uint32_t v0 = 0;
    v0 = 0x1460000u;
    {
        A1_W32B(v0, 29304, a0);
        return v0;
    }
}

// gamecore_sub_01425700 (0x1425700): typed by tools/typed_lift.py (round A1)
inline uint64_t gamecore_sub_01425700(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t a0, uint64_t a1)
{
    uint32_t v1 = 0;
    uint64_t v0 = 0;
    bool df_v1 = false, df_a0 = false;
    {
        const bool c = (int64_t)a1 != 0;
        A1_W32B(a0, 144, (uint32_t)a1);
        if (!c) {
            a0 = A1_R32B(a0, 140); df_a0 = true;
            v1 = 4u; df_v1 = true;
            {
                const bool c = a0 == v1;
                (v0 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL));
                if (c) {
                    goto L_142571c;
                }
            }
        }
    }
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 1))));
L_142571c:
    {
        return v0;
    }
}

// gamecore_sub_01425788 (0x1425788): typed by tools/typed_lift.py (round A1)
inline a1::RetVF gamecore_sub_01425788(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t a0)
{
    uint32_t v0 = 0;
    float f0 = 0.0f;
    float f1 = 0.0f;
    uint32_t fcr31 = 0;
    f1 = a1_flt(A1_R32B(a0, 6184));
    f0 = a1_flt(0u);
    fcr31 = (a1f::lt(f0, f1)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
    {
        const bool c = (fcr31 & 0x800000u) != 0u;
        v0 = 1u;
        if (!c) {
            v0 = 0u;
        }
    }
    {
        return a1::RetVF{(uint64_t)a1_sx64(v0), f0};
    }
}

// gamecore_sub_014257A8 (0x14257a8): typed by tools/typed_lift.py (round A1)
inline void gamecore_sub_014257A8(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t a0)
{
    {
        A1_W32(a0, 0u);
        
        return;
    }
}

// gamecore_sub_01425AC0 (0x1425ac0): typed by tools/typed_lift.py (round A1)
inline uint32_t gamecore_sub_01425AC0(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t a0, uint32_t a1)
{
    uint32_t v0 = 0;
    uint32_t v1 = 0;
    uint32_t a2 = 0;
    uint32_t a3 = 0;
    uint32_t t0 = 0;
    uint32_t t1 = 0;
    v0 = a1 + 0x5C8u;
    v1 = a1 + 0x4Cu;
    a2 = a1 + 0x1E0u;
    a3 = a1 + 0x2ACu;
    t0 = a1 + 0x330u;
    t1 = a1 + 0x400u;
    a1 = a1 + 0x4E4u;
    A1_W32B(a0, 12576, v0);
    A1_W32B(a0, 4976, v1);
    A1_W32B(a0, 11332, a2);
    A1_W32B(a0, 11552, a3);
    A1_W32B(a0, 11744, t0);
    A1_W32B(a0, 11984, t1);
    {
        A1_W32B(a0, 12272, a1);
        return v0;
    }
}

// gamecore_sub_014262A0 (0x14262a0): typed by tools/typed_lift.py (round A1)
inline void gamecore_sub_014262A0(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t a1)
{
    A1_W32B(a1, 152, 0u);
    {
        A1_W32B(a1, 148, 0u);
        
        return;
    }
}

// gamecore_sub_01426B18 (0x1426b18): typed by tools/typed_lift.py (round A1)
inline float gamecore_sub_01426B18(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t a0)
{
    float f0 = 0.0f;
    {
        f0 = a1_flt(A1_R32B(a0, 592));
        return f0;
    }
}

// gamecore_sub_01426B20 (0x1426b20): typed by tools/typed_lift.py (round A1)
inline float gamecore_sub_01426B20(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t a0)
{
    float f0 = 0.0f;
    {
        f0 = a1_flt(A1_R32B(a0, 600));
        return f0;
    }
}

// gamecore_sub_01427230 (0x1427230): typed by tools/typed_lift.py (round A1)
inline uint64_t gamecore_sub_01427230(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint64_t a0)
{
    uint64_t v0 = 0;
    uint64_t v1 = 0;
    (v0 = (uint64_t)(((int64_t)(int64_t)a0 < (int64_t)(int32_t)2) ? 1 : 0));
    {
        const bool c = (int64_t)v0 != 0;
        (v1 = (uint64_t)(((int64_t)(int64_t)a0 < (int64_t)(int32_t)4) ? 1 : 0));
        if (!c) {
            if ((int64_t)v1 == 0) {
                (v0 = (uint64_t)(((int64_t)(int64_t)a0 < (int64_t)(int32_t)9) ? 1 : 0));
                if ((int64_t)v0 == 0) {
                    goto L_1427268;
                }
                (v0 = (uint64_t)(((int64_t)(int64_t)a0 < (int64_t)(int32_t)7) ? 1 : 0));
                if ((int64_t)v0 != 0) {
                    goto L_1427268;
                }
            }
            {
                (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 1))));
                return v0;
            }
        }
    }
L_1427268:
    {
        (v0 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL));
        return v0;
    }
}

// gamecore_sub_01428CD0 (0x1428cd0): typed by tools/typed_lift.py (round A1)
inline a1::RetVF gamecore_sub_01428CD0(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t a0, uint32_t a1, uint32_t a2, float f12, float f13)
{
    uint32_t v0 = 0;
    uint32_t v1 = 0;
    float f0 = 0.0f;
    float f1 = 0.0f;
    float f2 = 0.0f;
    float f3 = 0.0f;
    uint32_t fcr31 = 0;
    f2 = a1_flt(0u);
    f0 = a1_flt(A1_R32B(a1, 28));
    fcr31 = (a1f::lt(f2, f0)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
    if ((fcr31 & 0x800000u) == 0u) {
        f0 = a1_flt(A1_R32B(a1, 32));
    } else {
        f1 = a1_flt(A1_R32B(a2, 28));
        fcr31 = (a1f::lt(f2, f1)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
        if ((fcr31 & 0x800000u) == 0u) {
            f0 = a1_flt(A1_R32B(a1, 32));
        } else {
            f0 = a1f::mul(f12, f0);
            f1 = a1f::mul(f13, f1);
            f0 = a1f::addFinite(f0, f1);
            A1_W32B(a0, 28, a1_bits(f0));
            f0 = a1_flt(A1_R32B(a1, 32));
            f2 = a1_flt(0u);
        }
    }
    fcr31 = (a1f::lt(f2, f0)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
    if ((fcr31 & 0x800000u) == 0u) {
        f0 = a1_flt(A1_R32B(a1, 36));
    } else {
        f1 = a1_flt(A1_R32B(a2, 32));
        fcr31 = (a1f::lt(f2, f1)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
        if ((fcr31 & 0x800000u) == 0u) {
            f0 = a1_flt(A1_R32B(a1, 36));
        } else {
            f0 = a1f::mul(f12, f0);
            f1 = a1f::mul(f13, f1);
            f0 = a1f::addFinite(f0, f1);
            A1_W32B(a0, 32, a1_bits(f0));
            f0 = a1_flt(A1_R32B(a1, 36));
        }
    }
    f2 = a1_flt(0u);
    fcr31 = (a1f::lt(f2, f0)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
    if ((fcr31 & 0x800000u) == 0u) {
        f0 = a1_flt(A1_R32B(a1, 40));
    } else {
        f1 = a1_flt(A1_R32B(a2, 36));
        fcr31 = (a1f::lt(f2, f1)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
        if ((fcr31 & 0x800000u) == 0u) {
            f0 = a1_flt(A1_R32B(a1, 40));
        } else {
            f0 = a1f::mul(f12, f0);
            f1 = a1f::mul(f13, f1);
            f0 = a1f::addFinite(f0, f1);
            A1_W32B(a0, 36, a1_bits(f0));
            f0 = a1_flt(A1_R32B(a1, 40));
        }
    }
    f1 = a1_flt(A1_R32B(a2, 40));
    f0 = a1f::mul(f12, f0);
    f1 = a1f::mul(f13, f1);
    f3 = a1_flt(0u);
    f0 = a1f::addFinite(f0, f1);
    A1_W32B(a0, 40, a1_bits(f0));
    f1 = a1_flt(A1_R32B(a1, 52));
    f0 = a1_flt(A1_R32B(a2, 52));
    f1 = a1f::mul(f12, f1);
    f0 = a1f::mul(f13, f0);
    f1 = a1f::addFinite(f1, f0);
    A1_W32B(a0, 52, a1_bits(f1));
    f2 = a1_flt(A1_R32B(a1, 56));
    f0 = a1_flt(A1_R32B(a2, 56));
    f2 = a1f::mul(f12, f2);
    f0 = a1f::mul(f13, f0);
    f2 = a1f::addFinite(f2, f0);
    A1_W32B(a0, 56, a1_bits(f2));
    f1 = a1_flt(A1_R32B(a1, 60));
    f0 = a1_flt(A1_R32B(a2, 60));
    f1 = a1f::mul(f12, f1);
    f0 = a1f::mul(f13, f0);
    f1 = a1f::addFinite(f1, f0);
    A1_W32B(a0, 60, a1_bits(f1));
    f2 = a1_flt(A1_R32B(a1, 64));
    f0 = a1_flt(A1_R32B(a2, 64));
    f2 = a1f::mul(f12, f2);
    f0 = a1f::mul(f13, f0);
    f2 = a1f::addFinite(f2, f0);
    A1_W32B(a0, 64, a1_bits(f2));
    f0 = a1_flt(A1_R32B(a1, 68));
    fcr31 = (a1f::lt(f3, f0)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
    if ((fcr31 & 0x800000u) == 0u) {
        f0 = a1_flt(A1_R32B(a1, 72));
    } else {
        f1 = a1_flt(A1_R32B(a2, 68));
        fcr31 = (a1f::lt(f3, f1)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
        if ((fcr31 & 0x800000u) == 0u) {
            f0 = a1_flt(A1_R32B(a1, 72));
        } else {
            f0 = a1f::mul(f12, f0);
            f1 = a1f::mul(f13, f1);
            f0 = a1f::addFinite(f0, f1);
            A1_W32B(a0, 68, a1_bits(f0));
            f0 = a1_flt(A1_R32B(a1, 72));
        }
    }
    f2 = a1_flt(0u);
    fcr31 = (a1f::lt(f2, f0)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
    if ((fcr31 & 0x800000u) == 0u) {
        f0 = a1_flt(A1_R32B(a1, 76));
    } else {
        f1 = a1_flt(A1_R32B(a2, 72));
        fcr31 = (a1f::lt(f2, f1)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
        if ((fcr31 & 0x800000u) == 0u) {
            f0 = a1_flt(A1_R32B(a1, 76));
        } else {
            f0 = a1f::mul(f12, f0);
            f1 = a1f::mul(f13, f1);
            f0 = a1f::addFinite(f0, f1);
            A1_W32B(a0, 72, a1_bits(f0));
            f0 = a1_flt(A1_R32B(a1, 76));
        }
    }
    f1 = a1_flt(A1_R32B(a2, 76));
    f0 = a1f::mul(f12, f0);
    f1 = a1f::mul(f13, f1);
    f3 = a1_flt(0u);
    f0 = a1f::addFinite(f0, f1);
    A1_W32B(a0, 76, a1_bits(f0));
    f1 = a1_flt(A1_R32B(a1, 80));
    f0 = a1_flt(A1_R32B(a2, 80));
    f1 = a1f::mul(f12, f1);
    f0 = a1f::mul(f13, f0);
    f1 = a1f::addFinite(f1, f0);
    A1_W32B(a0, 80, a1_bits(f1));
    f2 = a1_flt(A1_R32B(a1, 84));
    f0 = a1_flt(A1_R32B(a2, 84));
    f2 = a1f::mul(f12, f2);
    f0 = a1f::mul(f13, f0);
    f2 = a1f::addFinite(f2, f0);
    A1_W32B(a0, 84, a1_bits(f2));
    f1 = a1_flt(A1_R32B(a1, 88));
    f0 = a1_flt(A1_R32B(a2, 88));
    f1 = a1f::mul(f12, f1);
    f0 = a1f::mul(f13, f0);
    f1 = a1f::addFinite(f1, f0);
    A1_W32B(a0, 88, a1_bits(f1));
    f2 = a1_flt(A1_R32B(a1, 92));
    f0 = a1_flt(A1_R32B(a2, 92));
    f2 = a1f::mul(f12, f2);
    f0 = a1f::mul(f13, f0);
    f2 = a1f::addFinite(f2, f0);
    A1_W32B(a0, 92, a1_bits(f2));
    f1 = a1_flt(A1_R32B(a1, 96));
    f0 = a1_flt(A1_R32B(a2, 96));
    f1 = a1f::mul(f12, f1);
    f0 = a1f::mul(f13, f0);
    f1 = a1f::addFinite(f1, f0);
    A1_W32B(a0, 96, a1_bits(f1));
    f2 = a1_flt(A1_R32B(a1, 100));
    f0 = a1_flt(A1_R32B(a2, 100));
    f2 = a1f::mul(f12, f2);
    f0 = a1f::mul(f13, f0);
    f2 = a1f::addFinite(f2, f0);
    A1_W32B(a0, 100, a1_bits(f2));
    f1 = a1_flt(A1_R32B(a1, 104));
    f0 = a1_flt(A1_R32B(a2, 104));
    f1 = a1f::mul(f12, f1);
    f0 = a1f::mul(f13, f0);
    f1 = a1f::addFinite(f1, f0);
    A1_W32B(a0, 104, a1_bits(f1));
    f2 = a1_flt(A1_R32B(a1, 108));
    f0 = a1_flt(A1_R32B(a2, 108));
    f2 = a1f::mul(f12, f2);
    f0 = a1f::mul(f13, f0);
    f2 = a1f::addFinite(f2, f0);
    A1_W32B(a0, 108, a1_bits(f2));
    f0 = a1_flt(A1_R32B(a1, 112));
    fcr31 = (a1f::lt(f3, f0)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
    if ((fcr31 & 0x800000u) == 0u) {
        f0 = a1_flt(A1_R32B(a1, 116));
    } else {
        f1 = a1_flt(A1_R32B(a2, 112));
        fcr31 = (a1f::lt(f3, f1)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
        if ((fcr31 & 0x800000u) == 0u) {
            f0 = a1_flt(A1_R32B(a1, 116));
        } else {
            f0 = a1f::mul(f12, f0);
            f1 = a1f::mul(f13, f1);
            f0 = a1f::addFinite(f0, f1);
            A1_W32B(a0, 112, a1_bits(f0));
            f0 = a1_flt(A1_R32B(a1, 116));
        }
    }
    f2 = a1_flt(0u);
    fcr31 = (a1f::lt(f2, f0)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
    if ((fcr31 & 0x800000u) == 0u) {
        f0 = a1_flt(A1_R32B(a1, 120));
    } else {
        f1 = a1_flt(A1_R32B(a2, 116));
        fcr31 = (a1f::lt(f2, f1)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
        if ((fcr31 & 0x800000u) == 0u) {
            f0 = a1_flt(A1_R32B(a1, 120));
        } else {
            f0 = a1f::mul(f12, f0);
            f1 = a1f::mul(f13, f1);
            f0 = a1f::addFinite(f0, f1);
            A1_W32B(a0, 116, a1_bits(f0));
            f0 = a1_flt(A1_R32B(a1, 120));
        }
    }
    f1 = a1_flt(A1_R32B(a2, 120));
    f0 = a1f::mul(f12, f0);
    f1 = a1f::mul(f13, f1);
    f2 = a1_flt(0u);
    f0 = a1f::addFinite(f0, f1);
    A1_W32B(a0, 120, a1_bits(f0));
    f0 = a1_flt(A1_R32B(a1, 124));
    fcr31 = (a1f::lt(f2, f0)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
    if ((fcr31 & 0x800000u) == 0u) {
        f0 = a1_flt(A1_R32B(a1, 128));
    } else {
        f1 = a1_flt(A1_R32B(a2, 124));
        fcr31 = (a1f::lt(f2, f1)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
        if ((fcr31 & 0x800000u) == 0u) {
            f0 = a1_flt(A1_R32B(a1, 128));
        } else {
            f0 = a1f::mul(f12, f0);
            f1 = a1f::mul(f13, f1);
            f0 = a1f::addFinite(f0, f1);
            A1_W32B(a0, 124, a1_bits(f0));
            f0 = a1_flt(A1_R32B(a1, 128));
        }
    }
    f1 = a1_flt(A1_R32B(a2, 128));
    f0 = a1f::mul(f12, f0);
    f1 = a1f::mul(f13, f1);
    f3 = a1_flt(0u);
    f0 = a1f::addFinite(f0, f1);
    A1_W32B(a0, 128, a1_bits(f0));
    f1 = a1_flt(A1_R32B(a1, 132));
    f0 = a1_flt(A1_R32B(a2, 132));
    f1 = a1f::mul(f12, f1);
    f0 = a1f::mul(f13, f0);
    f1 = a1f::addFinite(f1, f0);
    A1_W32B(a0, 132, a1_bits(f1));
    f2 = a1_flt(A1_R32B(a1, 136));
    f0 = a1_flt(A1_R32B(a2, 136));
    f2 = a1f::mul(f12, f2);
    f0 = a1f::mul(f13, f0);
    f2 = a1f::addFinite(f2, f0);
    A1_W32B(a0, 136, a1_bits(f2));
    f1 = a1_flt(A1_R32B(a1, 172));
    f0 = a1_flt(A1_R32B(a2, 172));
    f1 = a1f::mul(f12, f1);
    f0 = a1f::mul(f13, f0);
    f1 = a1f::addFinite(f1, f0);
    A1_W32B(a0, 172, a1_bits(f1));
    f2 = a1_flt(A1_R32B(a1, 176));
    f0 = a1_flt(A1_R32B(a2, 176));
    f2 = a1f::mul(f12, f2);
    f0 = a1f::mul(f13, f0);
    f2 = a1f::addFinite(f2, f0);
    A1_W32B(a0, 176, a1_bits(f2));
    f0 = a1_flt(A1_R32B(a1, 184));
    fcr31 = (a1f::lt(f3, f0)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
    if ((fcr31 & 0x800000u) == 0u) {
        f0 = a1_flt(A1_R32B(a1, 188));
    } else {
        f1 = a1_flt(A1_R32B(a2, 184));
        fcr31 = (a1f::lt(f3, f1)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
        if ((fcr31 & 0x800000u) == 0u) {
            f0 = a1_flt(A1_R32B(a1, 188));
        } else {
            f0 = a1f::mul(f12, f0);
            f1 = a1f::mul(f13, f1);
            f0 = a1f::addFinite(f0, f1);
            A1_W32B(a0, 184, a1_bits(f0));
            f0 = a1_flt(A1_R32B(a1, 188));
        }
    }
    f2 = a1_flt(0u);
    fcr31 = (a1f::lt(f2, f0)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
    if ((fcr31 & 0x800000u) == 0u) {
        f0 = a1_flt(A1_R32B(a1, 192));
    } else {
        f1 = a1_flt(A1_R32B(a2, 188));
        fcr31 = (a1f::lt(f2, f1)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
        if ((fcr31 & 0x800000u) == 0u) {
            f0 = a1_flt(A1_R32B(a1, 192));
        } else {
            f0 = a1f::mul(f12, f0);
            f1 = a1f::mul(f13, f1);
            f0 = a1f::addFinite(f0, f1);
            A1_W32B(a0, 188, a1_bits(f0));
            f0 = a1_flt(A1_R32B(a1, 192));
            f2 = a1_flt(0u);
        }
    }
    fcr31 = (a1f::lt(f2, f0)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
    if ((fcr31 & 0x800000u) == 0u) {
        f0 = a1_flt(A1_R32B(a1, 196));
    } else {
        f1 = a1_flt(A1_R32B(a2, 192));
        fcr31 = (a1f::lt(f2, f1)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
        if ((fcr31 & 0x800000u) == 0u) {
            f0 = a1_flt(A1_R32B(a1, 196));
        } else {
            f0 = a1f::mul(f12, f0);
            f1 = a1f::mul(f13, f1);
            f0 = a1f::addFinite(f0, f1);
            A1_W32B(a0, 192, a1_bits(f0));
            f0 = a1_flt(A1_R32B(a1, 196));
        }
    }
    f2 = a1_flt(0u);
    fcr31 = (a1f::lt(f2, f0)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
    if ((fcr31 & 0x800000u) == 0u) {
        f0 = a1_flt(A1_R32B(a1, 212));
    } else {
        f1 = a1_flt(A1_R32B(a2, 196));
        fcr31 = (a1f::lt(f2, f1)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
        if ((fcr31 & 0x800000u) == 0u) {
            f0 = a1_flt(A1_R32B(a1, 212));
        } else {
            f0 = a1f::mul(f12, f0);
            f1 = a1f::mul(f13, f1);
            f0 = a1f::addFinite(f0, f1);
            A1_W32B(a0, 196, a1_bits(f0));
            f0 = a1_flt(A1_R32B(a1, 212));
            f2 = a1_flt(0u);
        }
    }
    fcr31 = (a1f::lt(f2, f0)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
    if ((fcr31 & 0x800000u) == 0u) {
        f0 = a1_flt(A1_R32B(a1, 216));
    } else {
        f1 = a1_flt(A1_R32B(a2, 212));
        fcr31 = (a1f::lt(f2, f1)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
        if ((fcr31 & 0x800000u) == 0u) {
            f0 = a1_flt(A1_R32B(a1, 216));
        } else {
            f0 = a1f::mul(f12, f0);
            f1 = a1f::mul(f13, f1);
            f0 = a1f::addFinite(f0, f1);
            A1_W32B(a0, 212, a1_bits(f0));
            f0 = a1_flt(A1_R32B(a1, 216));
        }
    }
    f2 = a1_flt(0u);
    fcr31 = (a1f::lt(f2, f0)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
    if ((fcr31 & 0x800000u) == 0u) {
        f0 = a1_flt(A1_R32B(a1, 220));
    } else {
        f1 = a1_flt(A1_R32B(a2, 216));
        fcr31 = (a1f::lt(f2, f1)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
        if ((fcr31 & 0x800000u) == 0u) {
            f0 = a1_flt(A1_R32B(a1, 220));
        } else {
            f0 = a1f::mul(f12, f0);
            f1 = a1f::mul(f13, f1);
            f0 = a1f::addFinite(f0, f1);
            A1_W32B(a0, 216, a1_bits(f0));
            f0 = a1_flt(A1_R32B(a1, 220));
            f2 = a1_flt(0u);
        }
    }
    fcr31 = (a1f::lt(f2, f0)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
    if ((fcr31 & 0x800000u) == 0u) {
        f0 = a1_flt(A1_R32B(a1, 224));
    } else {
        f1 = a1_flt(A1_R32B(a2, 220));
        fcr31 = (a1f::lt(f2, f1)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
        if ((fcr31 & 0x800000u) == 0u) {
            f0 = a1_flt(A1_R32B(a1, 224));
        } else {
            f0 = a1f::mul(f12, f0);
            f1 = a1f::mul(f13, f1);
            f0 = a1f::addFinite(f0, f1);
            A1_W32B(a0, 220, a1_bits(f0));
            f0 = a1_flt(A1_R32B(a1, 224));
        }
    }
    f1 = a1_flt(A1_R32B(a2, 224));
    f0 = a1f::mul(f12, f0);
    f1 = a1f::mul(f13, f1);
    f3 = a1_flt(0u);
    f0 = a1f::addFinite(f0, f1);
    A1_W32B(a0, 224, a1_bits(f0));
    f1 = a1_flt(A1_R32B(a1, 228));
    f0 = a1_flt(A1_R32B(a2, 228));
    f1 = a1f::mul(f12, f1);
    f0 = a1f::mul(f13, f0);
    f1 = a1f::addFinite(f1, f0);
    A1_W32B(a0, 228, a1_bits(f1));
    f2 = a1_flt(A1_R32B(a1, 236));
    f0 = a1_flt(A1_R32B(a2, 236));
    f2 = a1f::mul(f12, f2);
    f0 = a1f::mul(f13, f0);
    f2 = a1f::addFinite(f2, f0);
    A1_W32B(a0, 236, a1_bits(f2));
    f1 = a1_flt(A1_R32B(a1, 240));
    f0 = a1_flt(A1_R32B(a2, 240));
    f1 = a1f::mul(f12, f1);
    f0 = a1f::mul(f13, f0);
    f1 = a1f::addFinite(f1, f0);
    A1_W32B(a0, 240, a1_bits(f1));
    f2 = a1_flt(A1_R32B(a1, 244));
    f0 = a1_flt(A1_R32B(a2, 244));
    f2 = a1f::mul(f12, f2);
    f0 = a1f::mul(f13, f0);
    f2 = a1f::addFinite(f2, f0);
    A1_W32B(a0, 244, a1_bits(f2));
    f1 = a1_flt(A1_R32B(a1, 248));
    f0 = a1_flt(A1_R32B(a2, 248));
    f1 = a1f::mul(f12, f1);
    f0 = a1f::mul(f13, f0);
    f1 = a1f::addFinite(f1, f0);
    A1_W32B(a0, 248, a1_bits(f1));
    f2 = a1_flt(A1_R32B(a1, 256));
    f0 = a1_flt(A1_R32B(a2, 256));
    f2 = a1f::mul(f12, f2);
    f0 = a1f::mul(f13, f0);
    f2 = a1f::addFinite(f2, f0);
    A1_W32B(a0, 256, a1_bits(f2));
    f1 = a1_flt(A1_R32B(a1, 260));
    f0 = a1_flt(A1_R32B(a2, 260));
    f1 = a1f::mul(f12, f1);
    f0 = a1f::mul(f13, f0);
    f1 = a1f::addFinite(f1, f0);
    A1_W32B(a0, 260, a1_bits(f1));
    f2 = a1_flt(A1_R32B(a1, 264));
    f0 = a1_flt(A1_R32B(a2, 264));
    f2 = a1f::mul(f12, f2);
    f0 = a1f::mul(f13, f0);
    f2 = a1f::addFinite(f2, f0);
    A1_W32B(a0, 264, a1_bits(f2));
    f1 = a1_flt(A1_R32B(a1, 268));
    f0 = a1_flt(A1_R32B(a2, 268));
    f1 = a1f::mul(f12, f1);
    f0 = a1f::mul(f13, f0);
    f1 = a1f::addFinite(f1, f0);
    A1_W32B(a0, 268, a1_bits(f1));
    f2 = a1_flt(A1_R32B(a1, 272));
    f0 = a1_flt(A1_R32B(a2, 272));
    f2 = a1f::mul(f12, f2);
    f0 = a1f::mul(f13, f0);
    f2 = a1f::addFinite(f2, f0);
    A1_W32B(a0, 272, a1_bits(f2));
    f0 = a1_flt(A1_R32B(a1, 284));
    fcr31 = (a1f::lt(f3, f0)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
    if ((fcr31 & 0x800000u) == 0u) {
        f0 = a1_flt(A1_R32B(a1, 288));
    } else {
        f1 = a1_flt(A1_R32B(a2, 284));
        fcr31 = (a1f::lt(f3, f1)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
        if ((fcr31 & 0x800000u) == 0u) {
            f0 = a1_flt(A1_R32B(a1, 288));
        } else {
            f0 = a1f::mul(f12, f0);
            f1 = a1f::mul(f13, f1);
            f0 = a1f::addFinite(f0, f1);
            A1_W32B(a0, 284, a1_bits(f0));
            f0 = a1_flt(A1_R32B(a1, 288));
        }
    }
    f2 = a1_flt(0u);
    fcr31 = (a1f::lt(f2, f0)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
    if ((fcr31 & 0x800000u) == 0u) {
        f0 = a1_flt(A1_R32B(a1, 292));
    } else {
        f1 = a1_flt(A1_R32B(a2, 288));
        fcr31 = (a1f::lt(f2, f1)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
        if ((fcr31 & 0x800000u) == 0u) {
            f0 = a1_flt(A1_R32B(a1, 292));
        } else {
            f0 = a1f::mul(f12, f0);
            f1 = a1f::mul(f13, f1);
            f0 = a1f::addFinite(f0, f1);
            A1_W32B(a0, 288, a1_bits(f0));
            f0 = a1_flt(A1_R32B(a1, 292));
            f2 = a1_flt(0u);
        }
    }
    fcr31 = (a1f::lt(f2, f0)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
    if ((fcr31 & 0x800000u) == 0u) {
        f0 = a1_flt(A1_R32B(a1, 296));
    } else {
        f1 = a1_flt(A1_R32B(a2, 292));
        fcr31 = (a1f::lt(f2, f1)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
        if ((fcr31 & 0x800000u) == 0u) {
            f0 = a1_flt(A1_R32B(a1, 296));
        } else {
            f0 = a1f::mul(f12, f0);
            f1 = a1f::mul(f13, f1);
            f0 = a1f::addFinite(f0, f1);
            A1_W32B(a0, 292, a1_bits(f0));
            f0 = a1_flt(A1_R32B(a1, 296));
        }
    }
    f1 = a1_flt(A1_R32B(a2, 296));
    f0 = a1f::mul(f12, f0);
    f1 = a1f::mul(f13, f1);
    f3 = a1_flt(0u);
    f0 = a1f::addFinite(f0, f1);
    A1_W32B(a0, 296, a1_bits(f0));
    f1 = a1_flt(A1_R32B(a1, 300));
    f0 = a1_flt(A1_R32B(a2, 300));
    f1 = a1f::mul(f12, f1);
    f0 = a1f::mul(f13, f0);
    f1 = a1f::addFinite(f1, f0);
    A1_W32B(a0, 300, a1_bits(f1));
    f2 = a1_flt(A1_R32B(a1, 304));
    f0 = a1_flt(A1_R32B(a2, 304));
    f2 = a1f::mul(f12, f2);
    f0 = a1f::mul(f13, f0);
    f2 = a1f::addFinite(f2, f0);
    A1_W32B(a0, 304, a1_bits(f2));
    f1 = a1_flt(A1_R32B(a1, 308));
    f0 = a1_flt(A1_R32B(a2, 308));
    f1 = a1f::mul(f12, f1);
    f0 = a1f::mul(f13, f0);
    f1 = a1f::addFinite(f1, f0);
    A1_W32B(a0, 308, a1_bits(f1));
    f2 = a1_flt(A1_R32B(a1, 312));
    f0 = a1_flt(A1_R32B(a2, 312));
    f2 = a1f::mul(f12, f2);
    f0 = a1f::mul(f13, f0);
    f2 = a1f::addFinite(f2, f0);
    A1_W32B(a0, 312, a1_bits(f2));
    f1 = a1_flt(A1_R32B(a1, 316));
    f0 = a1_flt(A1_R32B(a2, 316));
    f1 = a1f::mul(f12, f1);
    f0 = a1f::mul(f13, f0);
    f1 = a1f::addFinite(f1, f0);
    A1_W32B(a0, 316, a1_bits(f1));
    f0 = a1_flt(A1_R32B(a1, 324));
    fcr31 = (a1f::lt(f3, f0)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
    if ((fcr31 & 0x800000u) == 0u) {
        f0 = a1_flt(A1_R32B(a1, 328));
    } else {
        f1 = a1_flt(A1_R32B(a2, 324));
        fcr31 = (a1f::lt(f3, f1)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
        if ((fcr31 & 0x800000u) == 0u) {
            f0 = a1_flt(A1_R32B(a1, 328));
        } else {
            f0 = a1f::mul(f12, f0);
            f1 = a1f::mul(f13, f1);
            f0 = a1f::addFinite(f0, f1);
            A1_W32B(a0, 324, a1_bits(f0));
            f0 = a1_flt(A1_R32B(a1, 328));
        }
    }
    f2 = a1_flt(0u);
    fcr31 = (a1f::lt(f2, f0)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
    if ((fcr31 & 0x800000u) == 0u) {
        f0 = a1_flt(A1_R32B(a1, 332));
    } else {
        f1 = a1_flt(A1_R32B(a2, 328));
        fcr31 = (a1f::lt(f2, f1)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
        if ((fcr31 & 0x800000u) == 0u) {
            f0 = a1_flt(A1_R32B(a1, 332));
        } else {
            f0 = a1f::mul(f12, f0);
            f1 = a1f::mul(f13, f1);
            f0 = a1f::addFinite(f0, f1);
            A1_W32B(a0, 328, a1_bits(f0));
            f0 = a1_flt(A1_R32B(a1, 332));
            f2 = a1_flt(0u);
        }
    }
    fcr31 = (a1f::lt(f2, f0)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
    if ((fcr31 & 0x800000u) == 0u) {
        v1 = A1_R32B(a2, 320);
    } else {
        f1 = a1_flt(A1_R32B(a2, 332));
        fcr31 = (a1f::lt(f2, f1)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
        if ((fcr31 & 0x800000u) == 0u) {
            v1 = A1_R32B(a2, 320);
        } else {
            f0 = a1f::mul(f12, f0);
            f1 = a1f::mul(f13, f1);
            f0 = a1f::addFinite(f0, f1);
            A1_W32B(a0, 332, a1_bits(f0));
            v1 = A1_R32B(a2, 320);
        }
    }
    v0 = 1u;
    if (v1 != v0) {
        f1 = a1_flt(A1_R32B(a1, 336));
    } else {
        f0 = a1_flt(A1_R32B(a1, 336));
        A1_W32B(a0, 336, a1_bits(f0));
        goto L_14294bc;
    }
    f0 = a1_flt(A1_R32B(a2, 336));
    f1 = a1f::mul(f12, f1);
    f0 = a1f::mul(f13, f0);
    f1 = a1f::addFinite(f1, f0);
    A1_W32B(a0, 336, a1_bits(f1));
L_14294bc:
    f0 = a1_flt(A1_R32B(a1, 340));
    f2 = a1_flt(0u);
    fcr31 = (a1f::lt(f2, f0)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
    if ((fcr31 & 0x800000u) == 0u) {
        f0 = a1_flt(A1_R32B(a1, 344));
    } else {
        f1 = a1_flt(A1_R32B(a2, 340));
        fcr31 = (a1f::lt(f2, f1)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
        if ((fcr31 & 0x800000u) == 0u) {
            f0 = a1_flt(A1_R32B(a1, 344));
        } else {
            f0 = a1f::mul(f12, f0);
            f1 = a1f::mul(f13, f1);
            f0 = a1f::addFinite(f0, f1);
            A1_W32B(a0, 340, a1_bits(f0));
            f0 = a1_flt(A1_R32B(a1, 344));
            f2 = a1_flt(0u);
        }
    }
    fcr31 = (a1f::lt(f2, f0)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
    if ((fcr31 & 0x800000u) == 0u) {
        f0 = a1_flt(A1_R32B(a1, 348));
    } else {
        f1 = a1_flt(A1_R32B(a2, 344));
        fcr31 = (a1f::lt(f2, f1)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
        if ((fcr31 & 0x800000u) == 0u) {
            f0 = a1_flt(A1_R32B(a1, 348));
        } else {
            f0 = a1f::mul(f12, f0);
            f1 = a1f::mul(f13, f1);
            f0 = a1f::addFinite(f0, f1);
            A1_W32B(a0, 344, a1_bits(f0));
            f0 = a1_flt(A1_R32B(a1, 348));
        }
    }
    f2 = a1_flt(0u);
    fcr31 = (a1f::lt(f2, f0)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
    if ((fcr31 & 0x800000u) == 0u) {
        f0 = a1_flt(A1_R32B(a1, 352));
    } else {
        f1 = a1_flt(A1_R32B(a2, 348));
        fcr31 = (a1f::lt(f2, f1)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
        if ((fcr31 & 0x800000u) == 0u) {
            f0 = a1_flt(A1_R32B(a1, 352));
        } else {
            f0 = a1f::mul(f12, f0);
            f1 = a1f::mul(f13, f1);
            f0 = a1f::addFinite(f0, f1);
            A1_W32B(a0, 348, a1_bits(f0));
            f0 = a1_flt(A1_R32B(a1, 352));
        }
    }
    f1 = a1_flt(A1_R32B(a2, 352));
    f0 = a1f::mul(f12, f0);
    f1 = a1f::mul(f13, f1);
    f3 = a1_flt(0u);
    f0 = a1f::addFinite(f0, f1);
    A1_W32B(a0, 352, a1_bits(f0));
    f1 = a1_flt(A1_R32B(a1, 356));
    f0 = a1_flt(A1_R32B(a2, 356));
    f1 = a1f::mul(f12, f1);
    f0 = a1f::mul(f13, f0);
    f1 = a1f::addFinite(f1, f0);
    A1_W32B(a0, 356, a1_bits(f1));
    f2 = a1_flt(A1_R32B(a1, 368));
    f0 = a1_flt(A1_R32B(a2, 368));
    f2 = a1f::mul(f12, f2);
    f0 = a1f::mul(f13, f0);
    f2 = a1f::addFinite(f2, f0);
    A1_W32B(a0, 368, a1_bits(f2));
    f0 = a1_flt(A1_R32B(a1, 372));
    fcr31 = (a1f::lt(f3, f0)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
    if ((fcr31 & 0x800000u) == 0u) {
        f0 = a1_flt(A1_R32B(a1, 376));
    } else {
        f1 = a1_flt(A1_R32B(a2, 372));
        fcr31 = (a1f::lt(f3, f1)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
        if ((fcr31 & 0x800000u) == 0u) {
            f0 = a1_flt(A1_R32B(a1, 376));
        } else {
            f0 = a1f::mul(f12, f0);
            f1 = a1f::mul(f13, f1);
            f0 = a1f::addFinite(f0, f1);
            A1_W32B(a0, 372, a1_bits(f0));
            f0 = a1_flt(A1_R32B(a1, 376));
        }
    }
    f2 = a1_flt(0u);
    fcr31 = (a1f::lt(f2, f0)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
    if ((fcr31 & 0x800000u) == 0u) {
        f0 = a1_flt(A1_R32B(a1, 380));
    } else {
        f1 = a1_flt(A1_R32B(a2, 376));
        fcr31 = (a1f::lt(f2, f1)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
        if ((fcr31 & 0x800000u) == 0u) {
            f0 = a1_flt(A1_R32B(a1, 380));
        } else {
            f0 = a1f::mul(f12, f0);
            f1 = a1f::mul(f13, f1);
            f0 = a1f::addFinite(f0, f1);
            A1_W32B(a0, 376, a1_bits(f0));
            f0 = a1_flt(A1_R32B(a1, 380));
            f2 = a1_flt(0u);
        }
    }
    fcr31 = (a1f::lt(f2, f0)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
    if ((fcr31 & 0x800000u) == 0u) {
        f0 = a1_flt(A1_R32B(a1, 384));
    } else {
        f1 = a1_flt(A1_R32B(a2, 380));
        fcr31 = (a1f::lt(f2, f1)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
        if ((fcr31 & 0x800000u) == 0u) {
            f0 = a1_flt(A1_R32B(a1, 384));
        } else {
            f0 = a1f::mul(f12, f0);
            f1 = a1f::mul(f13, f1);
            f0 = a1f::addFinite(f0, f1);
            A1_W32B(a0, 380, a1_bits(f0));
            f0 = a1_flt(A1_R32B(a1, 384));
        }
    }
    f2 = a1_flt(0u);
    fcr31 = (a1f::lt(f2, f0)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
    if ((fcr31 & 0x800000u) == 0u) {
        f0 = a1_flt(A1_R32B(a1, 388));
    } else {
        f1 = a1_flt(A1_R32B(a2, 384));
        fcr31 = (a1f::lt(f2, f1)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
        if ((fcr31 & 0x800000u) == 0u) {
            f0 = a1_flt(A1_R32B(a1, 388));
        } else {
            f0 = a1f::mul(f12, f0);
            f1 = a1f::mul(f13, f1);
            f0 = a1f::addFinite(f0, f1);
            A1_W32B(a0, 384, a1_bits(f0));
            f0 = a1_flt(A1_R32B(a1, 388));
            f2 = a1_flt(0u);
        }
    }
    fcr31 = (a1f::lt(f2, f0)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
    if ((fcr31 & 0x800000u) == 0u) {
        f0 = a1_flt(A1_R32B(a1, 392));
    } else {
        f1 = a1_flt(A1_R32B(a2, 388));
        fcr31 = (a1f::lt(f2, f1)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
        if ((fcr31 & 0x800000u) == 0u) {
            f0 = a1_flt(A1_R32B(a1, 392));
        } else {
            f0 = a1f::mul(f12, f0);
            f1 = a1f::mul(f13, f1);
            f0 = a1f::addFinite(f0, f1);
            A1_W32B(a0, 388, a1_bits(f0));
            f0 = a1_flt(A1_R32B(a1, 392));
        }
    }
    f2 = a1_flt(0u);
    fcr31 = (a1f::lt(f2, f0)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
    if ((fcr31 & 0x800000u) == 0u) {
        f1 = a1_flt(A1_R32B(a1, 396));
    } else {
        f1 = a1_flt(A1_R32B(a2, 392));
        fcr31 = (a1f::lt(f2, f1)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
        if ((fcr31 & 0x800000u) == 0u) {
            f1 = a1_flt(A1_R32B(a1, 396));
        } else {
            f0 = a1f::mul(f12, f0);
            f1 = a1f::mul(f13, f1);
            f0 = a1f::addFinite(f0, f1);
            A1_W32B(a0, 392, a1_bits(f0));
            f1 = a1_flt(A1_R32B(a1, 396));
        }
    }
    f0 = a1_flt(A1_R32B(a2, 396));
    f1 = a1f::mul(f12, f1);
    f0 = a1f::mul(f13, f0);
    f1 = a1f::addFinite(f1, f0);
    {
        A1_W32B(a0, 396, a1_bits(f1));
        return a1::RetVF{(uint64_t)a1_sx64(v0), f0};
    }
}

// gamecore_sub_014296F8 (0x14296f8): typed by tools/typed_lift.py (round A1)
inline a1::RetVF gamecore_sub_014296F8(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t a0, uint32_t a1, uint32_t a2, float f12, float f13)
{
    uint32_t at = 0;
    uint32_t v0 = 0;
    uint32_t v1 = 0;
    float f0 = 0.0f;
    float f1 = 0.0f;
    float f2 = 0.0f;
    float f3 = 0.0f;
    uint32_t fcr31 = 0;
    bool df_at = false;
    f0 = a1_flt(A1_R32B(a1, 12));
    f1 = a1_flt(A1_R32B(a2, 12));
    f0 = a1f::mul(f12, f0);
    f1 = a1f::mul(f13, f1);
    f3 = a1_flt(0u);
    f0 = a1f::addFinite(f0, f1);
    A1_W32B(a0, 12, a1_bits(f0));
    f2 = a1_flt(A1_R32B(a1, 20));
    fcr31 = (a1f::lt(f3, f2)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
    if ((fcr31 & 0x800000u) == 0u) {
        f2 = a1_flt(A1_R32B(a1, 24));
    } else {
        f1 = a1_flt(A1_R32B(a2, 20));
        fcr31 = (a1f::lt(f3, f1)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
        if ((fcr31 & 0x800000u) == 0u) {
            f2 = a1_flt(A1_R32B(a1, 24));
        } else {
            at = 0x3A830000u; df_at = true;
            at = at | 0x126Eu; df_at = true;
            f0 = a1_flt(at);
            fcr31 = (a1f::eq(f1, f0)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
            if ((fcr31 & 0x800000u) != 0u) {
                f2 = a1_flt(A1_R32B(a1, 24));
            } else {
                f0 = a1f::mul(f12, f2);
                f1 = a1f::mul(f13, f1);
                f0 = a1f::addFinite(f0, f1);
                A1_W32B(a0, 20, a1_bits(f0));
                f2 = a1_flt(A1_R32B(a1, 24));
            }
        }
    }
    f0 = a1_flt(0u);
    fcr31 = (a1f::lt(f0, f2)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
    if ((fcr31 & 0x800000u) == 0u) {
        f0 = a1_flt(A1_R32B(a1, 28));
    } else {
        f1 = a1_flt(A1_R32B(a2, 24));
        fcr31 = (a1f::lt(f0, f1)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
        if ((fcr31 & 0x800000u) == 0u) {
            f0 = a1_flt(A1_R32B(a1, 28));
        } else {
            at = 0x3A830000u; df_at = true;
            at = at | 0x126Eu; df_at = true;
            f0 = a1_flt(at);
            fcr31 = (a1f::eq(f1, f0)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
            if ((fcr31 & 0x800000u) != 0u) {
                f0 = a1_flt(A1_R32B(a1, 28));
            } else {
                f0 = a1f::mul(f12, f2);
                f1 = a1f::mul(f13, f1);
                f0 = a1f::addFinite(f0, f1);
                A1_W32B(a0, 24, a1_bits(f0));
                f0 = a1_flt(A1_R32B(a1, 28));
            }
        }
    }
    f2 = a1_flt(0u);
    fcr31 = (a1f::lt(f2, f0)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
    if ((fcr31 & 0x800000u) == 0u) {
        f0 = a1_flt(A1_R32B(a1, 40));
    } else {
        f1 = a1_flt(A1_R32B(a2, 28));
        fcr31 = (a1f::lt(f2, f1)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
        if ((fcr31 & 0x800000u) == 0u) {
            f0 = a1_flt(A1_R32B(a1, 40));
        } else {
            f0 = a1f::mul(f12, f0);
            f1 = a1f::mul(f13, f1);
            f0 = a1f::addFinite(f0, f1);
            A1_W32B(a0, 28, a1_bits(f0));
            f0 = a1_flt(A1_R32B(a1, 40));
            f2 = a1_flt(0u);
        }
    }
    fcr31 = (a1f::lt(f2, f0)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
    if ((fcr31 & 0x800000u) == 0u) {
        f0 = a1_flt(A1_R32B(a1, 44));
    } else {
        f1 = a1_flt(A1_R32B(a2, 40));
        fcr31 = (a1f::lt(f2, f1)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
        if ((fcr31 & 0x800000u) == 0u) {
            f0 = a1_flt(A1_R32B(a1, 44));
        } else {
            f0 = a1f::mul(f12, f0);
            f1 = a1f::mul(f13, f1);
            f0 = a1f::addFinite(f0, f1);
            A1_W32B(a0, 40, a1_bits(f0));
            f0 = a1_flt(A1_R32B(a1, 44));
        }
    }
    f1 = a1_flt(A1_R32B(a2, 44));
    f0 = a1f::mul(f12, f0);
    f1 = a1f::mul(f13, f1);
    f3 = a1_flt(0u);
    f0 = a1f::addFinite(f0, f1);
    A1_W32B(a0, 44, a1_bits(f0));
    f1 = a1_flt(A1_R32B(a1, 48));
    f0 = a1_flt(A1_R32B(a2, 48));
    f1 = a1f::mul(f12, f1);
    f0 = a1f::mul(f13, f0);
    f1 = a1f::addFinite(f1, f0);
    A1_W32B(a0, 48, a1_bits(f1));
    f2 = a1_flt(A1_R32B(a1, 52));
    f0 = a1_flt(A1_R32B(a2, 52));
    f2 = a1f::mul(f12, f2);
    f0 = a1f::mul(f13, f0);
    f2 = a1f::addFinite(f2, f0);
    A1_W32B(a0, 52, a1_bits(f2));
    f1 = a1_flt(A1_R32B(a1, 56));
    f0 = a1_flt(A1_R32B(a2, 56));
    f1 = a1f::mul(f12, f1);
    f0 = a1f::mul(f13, f0);
    f1 = a1f::addFinite(f1, f0);
    A1_W32B(a0, 56, a1_bits(f1));
    f2 = a1_flt(A1_R32B(a1, 60));
    f0 = a1_flt(A1_R32B(a2, 60));
    f2 = a1f::mul(f12, f2);
    f0 = a1f::mul(f13, f0);
    f2 = a1f::addFinite(f2, f0);
    A1_W32B(a0, 60, a1_bits(f2));
    f1 = a1_flt(A1_R32B(a1, 64));
    f0 = a1_flt(A1_R32B(a2, 64));
    f1 = a1f::mul(f12, f1);
    f0 = a1f::mul(f13, f0);
    f1 = a1f::addFinite(f1, f0);
    A1_W32B(a0, 64, a1_bits(f1));
    f2 = a1_flt(A1_R32B(a1, 68));
    f0 = a1_flt(A1_R32B(a2, 68));
    f2 = a1f::mul(f12, f2);
    f0 = a1f::mul(f13, f0);
    f2 = a1f::addFinite(f2, f0);
    A1_W32B(a0, 68, a1_bits(f2));
    f0 = a1_flt(A1_R32B(a1, 76));
    fcr31 = (a1f::lt(f3, f0)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
    if ((fcr31 & 0x800000u) == 0u) {
        f2 = a1_flt(A1_R32B(a1, 80));
    } else {
        f1 = a1_flt(A1_R32B(a2, 76));
        fcr31 = (a1f::lt(f3, f1)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
        if ((fcr31 & 0x800000u) == 0u) {
            f2 = a1_flt(A1_R32B(a1, 80));
        } else {
            f0 = a1f::mul(f12, f0);
            f1 = a1f::mul(f13, f1);
            f0 = a1f::addFinite(f0, f1);
            A1_W32B(a0, 76, a1_bits(f0));
            f2 = a1_flt(A1_R32B(a1, 80));
        }
    }
    v1 = 1u;
    f0 = a1_flt(A1_R32B(a2, 80));
    f2 = a1f::mul(f12, f2);
    f0 = a1f::mul(f13, f0);
    f2 = a1f::addFinite(f2, f0);
    A1_W32B(a0, 80, a1_bits(f2));
    f1 = a1_flt(A1_R32B(a1, 84));
    f0 = a1_flt(A1_R32B(a2, 84));
    f1 = a1f::mul(f12, f1);
    f0 = a1f::mul(f13, f0);
    f1 = a1f::addFinite(f1, f0);
    A1_W32B(a0, 84, a1_bits(f1));
    f2 = a1_flt(A1_R32B(a1, 88));
    f0 = a1_flt(A1_R32B(a2, 88));
    f2 = a1f::mul(f12, f2);
    f0 = a1f::mul(f13, f0);
    f2 = a1f::addFinite(f2, f0);
    A1_W32B(a0, 88, a1_bits(f2));
    f1 = a1_flt(A1_R32B(a1, 92));
    f0 = a1_flt(A1_R32B(a2, 92));
    f1 = a1f::mul(f12, f1);
    f0 = a1f::mul(f13, f0);
    f1 = a1f::addFinite(f1, f0);
    A1_W32B(a0, 92, a1_bits(f1));
    f2 = a1_flt(A1_R32B(a1, 96));
    f0 = a1_flt(A1_R32B(a2, 96));
    f2 = a1f::mul(f12, f2);
    f0 = a1f::mul(f13, f0);
    f2 = a1f::addFinite(f2, f0);
    A1_W32B(a0, 96, a1_bits(f2));
    f1 = a1_flt(A1_R32B(a1, 100));
    f0 = a1_flt(A1_R32B(a2, 100));
    f1 = a1f::mul(f12, f1);
    f0 = a1f::mul(f13, f0);
    f1 = a1f::addFinite(f1, f0);
    A1_W32B(a0, 100, a1_bits(f1));
    f2 = a1_flt(A1_R32B(a1, 104));
    f0 = a1_flt(A1_R32B(a2, 104));
    f2 = a1f::mul(f12, f2);
    f0 = a1f::mul(f13, f0);
    f2 = a1f::addFinite(f2, f0);
    A1_W32B(a0, 104, a1_bits(f2));
    f1 = a1_flt(A1_R32B(a1, 112));
    f0 = a1_flt(A1_R32B(a2, 112));
    f1 = a1f::mul(f12, f1);
    f0 = a1f::mul(f13, f0);
    f1 = a1f::addFinite(f1, f0);
    A1_W32B(a0, 112, a1_bits(f1));
    f2 = a1_flt(A1_R32B(a1, 116));
    f0 = a1_flt(A1_R32B(a2, 116));
    f2 = a1f::mul(f12, f2);
    f0 = a1f::mul(f13, f0);
    f2 = a1f::addFinite(f2, f0);
    A1_W32B(a0, 116, a1_bits(f2));
    f1 = a1_flt(A1_R32B(a1, 120));
    f0 = a1_flt(A1_R32B(a2, 120));
    f1 = a1f::mul(f12, f1);
    f0 = a1f::mul(f13, f0);
    f1 = a1f::addFinite(f1, f0);
    A1_W32B(a0, 120, a1_bits(f1));
    f2 = a1_flt(A1_R32B(a1, 124));
    f0 = a1_flt(A1_R32B(a2, 124));
    f2 = a1f::mul(f12, f2);
    f0 = a1f::mul(f13, f0);
    f2 = a1f::addFinite(f2, f0);
    A1_W32B(a0, 124, a1_bits(f2));
    f1 = a1_flt(A1_R32B(a1, 128));
    f0 = a1_flt(A1_R32B(a2, 128));
    f1 = a1f::mul(f12, f1);
    f0 = a1f::mul(f13, f0);
    f1 = a1f::addFinite(f1, f0);
    A1_W32B(a0, 128, a1_bits(f1));
    f2 = a1_flt(A1_R32B(a1, 132));
    f0 = a1_flt(A1_R32B(a2, 132));
    f2 = a1f::mul(f12, f2);
    f0 = a1f::mul(f13, f0);
    f2 = a1f::addFinite(f2, f0);
    A1_W32B(a0, 132, a1_bits(f2));
    f1 = a1_flt(A1_R32B(a1, 136));
    f0 = a1_flt(A1_R32B(a2, 136));
    f1 = a1f::mul(f12, f1);
    f0 = a1f::mul(f13, f0);
    f1 = a1f::addFinite(f1, f0);
    A1_W32B(a0, 136, a1_bits(f1));
    f2 = a1_flt(A1_R32B(a1, 140));
    f0 = a1_flt(A1_R32B(a2, 140));
    f2 = a1f::mul(f12, f2);
    f0 = a1f::mul(f13, f0);
    f2 = a1f::addFinite(f2, f0);
    A1_W32B(a0, 140, a1_bits(f2));
    f1 = a1_flt(A1_R32B(a1, 144));
    f0 = a1_flt(A1_R32B(a2, 144));
    f1 = a1f::mul(f12, f1);
    f0 = a1f::mul(f13, f0);
    f1 = a1f::addFinite(f1, f0);
    A1_W32B(a0, 144, a1_bits(f1));
    f2 = a1_flt(A1_R32B(a1, 148));
    f0 = a1_flt(A1_R32B(a2, 148));
    f2 = a1f::mul(f12, f2);
    f0 = a1f::mul(f13, f0);
    f2 = a1f::addFinite(f2, f0);
    A1_W32B(a0, 148, a1_bits(f2));
    f1 = a1_flt(A1_R32B(a1, 152));
    f0 = a1_flt(A1_R32B(a2, 152));
    f1 = a1f::mul(f12, f1);
    f0 = a1f::mul(f13, f0);
    f1 = a1f::addFinite(f1, f0);
    A1_W32B(a0, 152, a1_bits(f1));
    f2 = a1_flt(A1_R32B(a1, 156));
    f0 = a1_flt(A1_R32B(a2, 156));
    f2 = a1f::mul(f12, f2);
    f0 = a1f::mul(f13, f0);
    f2 = a1f::addFinite(f2, f0);
    A1_W32B(a0, 156, a1_bits(f2));
    f1 = a1_flt(A1_R32B(a1, 160));
    f0 = a1_flt(A1_R32B(a2, 160));
    f1 = a1f::mul(f12, f1);
    f0 = a1f::mul(f13, f0);
    f1 = a1f::addFinite(f1, f0);
    A1_W32B(a0, 160, a1_bits(f1));
    f2 = a1_flt(A1_R32B(a1, 164));
    f0 = a1_flt(A1_R32B(a2, 164));
    f2 = a1f::mul(f12, f2);
    f0 = a1f::mul(f13, f0);
    f2 = a1f::addFinite(f2, f0);
    A1_W32B(a0, 164, a1_bits(f2));
    v0 = A1_R32B(a1, 168);
    if (v0 == v1) {
        f1 = a1_flt(A1_R32B(a1, 172));
        f0 = a1_flt(A1_R32B(a2, 172));
        f1 = a1f::mul(f12, f1);
        f0 = a1f::mul(f13, f0);
        f1 = a1f::addFinite(f1, f0);
        A1_W32B(a0, 172, a1_bits(f1));
        f2 = a1_flt(A1_R32B(a1, 176));
        f0 = a1_flt(A1_R32B(a2, 176));
        f2 = a1f::mul(f12, f2);
        f0 = a1f::mul(f13, f0);
        f2 = a1f::addFinite(f2, f0);
        A1_W32B(a0, 176, a1_bits(f2));
        f1 = a1_flt(A1_R32B(a1, 180));
        f0 = a1_flt(A1_R32B(a2, 180));
        f1 = a1f::mul(f12, f1);
        f0 = a1f::mul(f13, f0);
        f1 = a1f::addFinite(f1, f0);
        A1_W32B(a0, 180, a1_bits(f1));
        f2 = a1_flt(A1_R32B(a1, 184));
        f0 = a1_flt(A1_R32B(a2, 184));
        f2 = a1f::mul(f12, f2);
        f0 = a1f::mul(f13, f0);
        f2 = a1f::addFinite(f2, f0);
        A1_W32B(a0, 184, a1_bits(f2));
        f1 = a1_flt(A1_R32B(a1, 188));
        f0 = a1_flt(A1_R32B(a2, 188));
        f1 = a1f::mul(f12, f1);
        f0 = a1f::mul(f13, f0);
        f1 = a1f::addFinite(f1, f0);
        A1_W32B(a0, 188, a1_bits(f1));
        f2 = a1_flt(A1_R32B(a1, 192));
        f0 = a1_flt(A1_R32B(a2, 192));
        f2 = a1f::mul(f12, f2);
        f0 = a1f::mul(f13, f0);
        f2 = a1f::addFinite(f2, f0);
        A1_W32B(a0, 192, a1_bits(f2));
        f1 = a1_flt(A1_R32B(a1, 196));
        f0 = a1_flt(A1_R32B(a2, 196));
        f1 = a1f::mul(f12, f1);
        f0 = a1f::mul(f13, f0);
        f1 = a1f::addFinite(f1, f0);
        A1_W32B(a0, 196, a1_bits(f1));
        f2 = a1_flt(A1_R32B(a1, 200));
        f0 = a1_flt(A1_R32B(a2, 200));
        f2 = a1f::mul(f12, f2);
        f0 = a1f::mul(f13, f0);
        f2 = a1f::addFinite(f2, f0);
        A1_W32B(a0, 200, a1_bits(f2));
    }
    {
        return a1::RetVF{(uint64_t)a1_sx64(v0), f0};
    }
}

// gamecore_sub_01429BC0 (0x1429bc0): typed by tools/typed_lift.py (round A1)
inline float gamecore_sub_01429BC0(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t a0, uint32_t a1, uint32_t a2, float f12, float f13)
{
    uint32_t at = 0;
    float f0 = 0.0f;
    float f1 = 0.0f;
    float f2 = 0.0f;
    float f3 = 0.0f;
    float f4 = 0.0f;
    float f5 = 0.0f;
    float f6 = 0.0f;
    uint32_t fcr31 = 0;
    f0 = a1_flt(A1_R32B(a1, 4));
    f1 = a1_flt(A1_R32B(a2, 4));
    f0 = a1f::mul(f12, f0);
    f1 = a1f::mul(f13, f1);
    f3 = a1_flt(0u);
    f0 = a1f::addFinite(f0, f1);
    A1_W32B(a0, 4, a1_bits(f0));
    f1 = a1_flt(A1_R32B(a1, 8));
    f0 = a1_flt(A1_R32B(a2, 8));
    f1 = a1f::mul(f12, f1);
    f0 = a1f::mul(f13, f0);
    f1 = a1f::addFinite(f1, f0);
    A1_W32B(a0, 8, a1_bits(f1));
    f2 = a1_flt(A1_R32B(a1, 12));
    f0 = a1_flt(A1_R32B(a2, 12));
    f2 = a1f::mul(f12, f2);
    f0 = a1f::mul(f13, f0);
    f2 = a1f::addFinite(f2, f0);
    A1_W32B(a0, 12, a1_bits(f2));
    f1 = a1_flt(A1_R32B(a1, 16));
    f0 = a1_flt(A1_R32B(a2, 16));
    f1 = a1f::mul(f12, f1);
    f0 = a1f::mul(f13, f0);
    f1 = a1f::addFinite(f1, f0);
    A1_W32B(a0, 16, a1_bits(f1));
    f2 = a1_flt(A1_R32B(a1, 20));
    f0 = a1_flt(A1_R32B(a2, 20));
    f2 = a1f::mul(f12, f2);
    f0 = a1f::mul(f13, f0);
    f2 = a1f::addFinite(f2, f0);
    A1_W32B(a0, 20, a1_bits(f2));
    f1 = a1_flt(A1_R32B(a1, 24));
    f0 = a1_flt(A1_R32B(a2, 24));
    f1 = a1f::mul(f12, f1);
    f0 = a1f::mul(f13, f0);
    f1 = a1f::addFinite(f1, f0);
    A1_W32B(a0, 24, a1_bits(f1));
    f2 = a1_flt(A1_R32B(a1, 28));
    f0 = a1_flt(A1_R32B(a2, 28));
    f2 = a1f::mul(f12, f2);
    f0 = a1f::mul(f13, f0);
    f2 = a1f::addFinite(f2, f0);
    A1_W32B(a0, 28, a1_bits(f2));
    f1 = a1_flt(A1_R32B(a1, 32));
    f0 = a1_flt(A1_R32B(a2, 32));
    f1 = a1f::mul(f12, f1);
    f0 = a1f::mul(f13, f0);
    f1 = a1f::addFinite(f1, f0);
    A1_W32B(a0, 32, a1_bits(f1));
    f2 = a1_flt(A1_R32B(a1, 36));
    f0 = a1_flt(A1_R32B(a2, 36));
    f2 = a1f::mul(f12, f2);
    f0 = a1f::mul(f13, f0);
    f2 = a1f::addFinite(f2, f0);
    A1_W32B(a0, 36, a1_bits(f2));
    f1 = a1_flt(A1_R32B(a1, 40));
    f0 = a1_flt(A1_R32B(a2, 40));
    f1 = a1f::mul(f12, f1);
    f0 = a1f::mul(f13, f0);
    f1 = a1f::addFinite(f1, f0);
    A1_W32B(a0, 40, a1_bits(f1));
    f2 = a1_flt(A1_R32B(a1, 44));
    f0 = a1_flt(A1_R32B(a2, 44));
    f2 = a1f::mul(f12, f2);
    f0 = a1f::mul(f13, f0);
    f2 = a1f::addFinite(f2, f0);
    A1_W32B(a0, 44, a1_bits(f2));
    f1 = a1_flt(A1_R32B(a1, 48));
    f0 = a1_flt(A1_R32B(a2, 48));
    f1 = a1f::mul(f12, f1);
    f0 = a1f::mul(f13, f0);
    f1 = a1f::addFinite(f1, f0);
    A1_W32B(a0, 48, a1_bits(f1));
    f2 = a1_flt(A1_R32B(a1, 52));
    f0 = a1_flt(A1_R32B(a2, 52));
    f2 = a1f::mul(f12, f2);
    f0 = a1f::mul(f13, f0);
    f2 = a1f::addFinite(f2, f0);
    A1_W32B(a0, 52, a1_bits(f2));
    f1 = a1_flt(A1_R32B(a1, 56));
    f0 = a1_flt(A1_R32B(a2, 56));
    f1 = a1f::mul(f12, f1);
    f0 = a1f::mul(f13, f0);
    f1 = a1f::addFinite(f1, f0);
    A1_W32B(a0, 56, a1_bits(f1));
    f2 = a1_flt(A1_R32B(a1, 60));
    f0 = a1_flt(A1_R32B(a2, 60));
    f2 = a1f::mul(f12, f2);
    f0 = a1f::mul(f13, f0);
    f2 = a1f::addFinite(f2, f0);
    A1_W32B(a0, 60, a1_bits(f2));
    f5 = a1_flt(A1_R32B(a2, 64));
    f4 = a1_flt(A1_R32B(a1, 64));
    f1 = ((float)(f5));
    f6 = ((float)(f4));
    f2 = a1f::sub(f1, f6);
    fcr31 = (a1f::lt(f2, f3)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
    if ((fcr31 & 0x800000u) != 0u) {
        at = 0xC3340000u;
        f0 = a1_flt(at);
        fcr31 = (a1f::le(f0, f2)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
        if ((fcr31 & 0x800000u) != 0u) {
            f1 = a1f::mul(f12, f4);
            goto L_1429d80;
        }
        goto L_1429d90;
    }
    at = 0x43340000u;
    f0 = a1_flt(at);
    fcr31 = (a1f::le(f2, f0)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
    if ((fcr31 & 0x800000u) == 0u) {
        goto L_1429d90;
    }
    f1 = a1f::mul(f12, f4);
L_1429d80:
    f0 = a1f::mul(f13, f5);
    f1 = a1f::add(f1, f0);
    goto L_1429df4;
L_1429d90:
    f0 = a1_flt(0u);
    fcr31 = (a1f::lt(f4, f0)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
    if ((fcr31 & 0x800000u) != 0u) {
        at = 0x43B40000u;
        f0 = a1_flt(at);
        f6 = a1f::add(f4, f0);
        goto L_1429dc8;
    }
    fcr31 = (a1f::lt(f5, f0)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
    if ((fcr31 & 0x800000u) == 0u) {
        f0 = a1f::mul(f12, f6);
        goto L_1429dcc;
    }
    at = 0x43B40000u;
    f0 = a1_flt(at);
    f1 = a1f::add(f5, f0);
L_1429dc8:
    f0 = a1f::mul(f12, f6);
L_1429dcc:
    at = 0x43340000u;
    f2 = a1_flt(at);
    f1 = a1f::mul(f13, f1);
    f1 = a1f::add(f0, f1);
    fcr31 = (a1f::lt(f2, f1)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
    if ((fcr31 & 0x800000u) == 0u) {
        A1_W32B(a0, 64, a1_bits(f1));
        goto L_1429df8;
    }
    at = 0x43B40000u;
    f0 = a1_flt(at);
    f1 = a1f::subFinite(f1, f0);
L_1429df4:
    A1_W32B(a0, 64, a1_bits(f1));
L_1429df8:
    f3 = a1_flt(0u);
    f1 = a1_flt(A1_R32B(a1, 68));
    f0 = a1_flt(A1_R32B(a2, 68));
    f1 = a1f::mul(f12, f1);
    f0 = a1f::mul(f13, f0);
    f1 = a1f::addFinite(f1, f0);
    A1_W32B(a0, 68, a1_bits(f1));
    f2 = a1_flt(A1_R32B(a1, 72));
    f0 = a1_flt(A1_R32B(a2, 72));
    f2 = a1f::mul(f12, f2);
    f0 = a1f::mul(f13, f0);
    f2 = a1f::addFinite(f2, f0);
    A1_W32B(a0, 72, a1_bits(f2));
    f1 = a1_flt(A1_R32B(a1, 76));
    f0 = a1_flt(A1_R32B(a2, 76));
    f1 = a1f::mul(f12, f1);
    f0 = a1f::mul(f13, f0);
    f1 = a1f::addFinite(f1, f0);
    A1_W32B(a0, 76, a1_bits(f1));
    f2 = a1_flt(A1_R32B(a1, 80));
    f0 = a1_flt(A1_R32B(a2, 80));
    f2 = a1f::mul(f12, f2);
    f0 = a1f::mul(f13, f0);
    f2 = a1f::addFinite(f2, f0);
    A1_W32B(a0, 80, a1_bits(f2));
    f1 = a1_flt(A1_R32B(a1, 84));
    f0 = a1_flt(A1_R32B(a2, 84));
    f1 = a1f::mul(f12, f1);
    f0 = a1f::mul(f13, f0);
    f1 = a1f::addFinite(f1, f0);
    A1_W32B(a0, 84, a1_bits(f1));
    f2 = a1_flt(A1_R32B(a1, 88));
    f0 = a1_flt(A1_R32B(a2, 88));
    f2 = a1f::mul(f12, f2);
    f0 = a1f::mul(f13, f0);
    f2 = a1f::addFinite(f2, f0);
    A1_W32B(a0, 88, a1_bits(f2));
    f1 = a1_flt(A1_R32B(a1, 92));
    f0 = a1_flt(A1_R32B(a2, 92));
    f1 = a1f::mul(f12, f1);
    f0 = a1f::mul(f13, f0);
    f1 = a1f::addFinite(f1, f0);
    A1_W32B(a0, 92, a1_bits(f1));
    f2 = a1_flt(A1_R32B(a1, 96));
    f0 = a1_flt(A1_R32B(a2, 96));
    f2 = a1f::mul(f12, f2);
    f0 = a1f::mul(f13, f0);
    f2 = a1f::addFinite(f2, f0);
    A1_W32B(a0, 96, a1_bits(f2));
    f1 = a1_flt(A1_R32B(a1, 100));
    f0 = a1_flt(A1_R32B(a2, 100));
    f1 = a1f::mul(f12, f1);
    f0 = a1f::mul(f13, f0);
    f1 = a1f::addFinite(f1, f0);
    A1_W32B(a0, 100, a1_bits(f1));
    f2 = a1_flt(A1_R32B(a1, 104));
    f0 = a1_flt(A1_R32B(a2, 104));
    f2 = a1f::mul(f12, f2);
    f0 = a1f::mul(f13, f0);
    f2 = a1f::addFinite(f2, f0);
    A1_W32B(a0, 104, a1_bits(f2));
    f1 = a1_flt(A1_R32B(a1, 108));
    f0 = a1_flt(A1_R32B(a2, 108));
    f1 = a1f::mul(f12, f1);
    f0 = a1f::mul(f13, f0);
    f1 = a1f::addFinite(f1, f0);
    A1_W32B(a0, 108, a1_bits(f1));
    f2 = a1_flt(A1_R32B(a1, 112));
    f0 = a1_flt(A1_R32B(a2, 112));
    f2 = a1f::mul(f12, f2);
    f0 = a1f::mul(f13, f0);
    f2 = a1f::addFinite(f2, f0);
    A1_W32B(a0, 112, a1_bits(f2));
    f1 = a1_flt(A1_R32B(a1, 116));
    f0 = a1_flt(A1_R32B(a2, 116));
    f1 = a1f::mul(f12, f1);
    f0 = a1f::mul(f13, f0);
    f1 = a1f::addFinite(f1, f0);
    A1_W32B(a0, 116, a1_bits(f1));
    f2 = a1_flt(A1_R32B(a1, 120));
    f0 = a1_flt(A1_R32B(a2, 120));
    f2 = a1f::mul(f12, f2);
    f0 = a1f::mul(f13, f0);
    f2 = a1f::addFinite(f2, f0);
    A1_W32B(a0, 120, a1_bits(f2));
    f2 = a1_flt(A1_R32B(a1, 124));
    fcr31 = (a1f::lt(f3, f2)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
    if ((fcr31 & 0x800000u) == 0u) {
        f2 = a1_flt(A1_R32B(a1, 128));
    } else {
        f1 = a1_flt(A1_R32B(a2, 124));
        fcr31 = (a1f::lt(f3, f1)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
        if ((fcr31 & 0x800000u) == 0u) {
            f2 = a1_flt(A1_R32B(a1, 128));
        } else {
            at = 0x3A830000u;
            at = at | 0x126Eu;
            f0 = a1_flt(at);
            fcr31 = (a1f::eq(f1, f0)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
            if ((fcr31 & 0x800000u) != 0u) {
                f2 = a1_flt(A1_R32B(a1, 128));
            } else {
                f0 = a1f::mul(f12, f2);
                f1 = a1f::mul(f13, f1);
                f0 = a1f::addFinite(f0, f1);
                A1_W32B(a0, 124, a1_bits(f0));
                f2 = a1_flt(A1_R32B(a1, 128));
            }
        }
    }
    f0 = a1_flt(0u);
    fcr31 = (a1f::lt(f0, f2)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
    if ((fcr31 & 0x800000u) != 0u) {
        f1 = a1_flt(A1_R32B(a2, 128));
        fcr31 = (a1f::lt(f0, f1)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
        if ((fcr31 & 0x800000u) != 0u) {
            at = 0x3A830000u;
            at = at | 0x126Eu;
            f0 = a1_flt(at);
            fcr31 = (a1f::eq(f1, f0)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
            if ((fcr31 & 0x800000u) == 0u) {
                f0 = a1f::mul(f12, f2);
                f1 = a1f::mul(f13, f1);
                f0 = a1f::addFinite(f0, f1);
                A1_W32B(a0, 128, a1_bits(f0));
            }
        }
    }
    {
        return f0;
    }
}

// gamecore_sub_0142B968 (0x142b968): typed by tools/typed_lift.py (round A1)
inline float gamecore_sub_0142B968(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t a0, uint32_t a1, uint32_t a2, float f12, float f13)
{
    uint32_t at = 0;
    float f0 = 0.0f;
    float f1 = 0.0f;
    float f2 = 0.0f;
    float f3 = 0.0f;
    uint32_t fcr31 = 0;
    bool df_at = false;
    f0 = a1_flt(A1_R32B(a1, 4));
    f1 = a1_flt(A1_R32B(a2, 4));
    f0 = a1f::mul(f12, f0);
    f1 = a1f::mul(f13, f1);
    f3 = a1_flt(0u);
    f0 = a1f::addFinite(f0, f1);
    A1_W32B(a0, 4, a1_bits(f0));
    f1 = a1_flt(A1_R32B(a1, 8));
    f0 = a1_flt(A1_R32B(a2, 8));
    f1 = a1f::mul(f12, f1);
    f0 = a1f::mul(f13, f0);
    f1 = a1f::addFinite(f1, f0);
    A1_W32B(a0, 8, a1_bits(f1));
    f2 = a1_flt(A1_R32B(a1, 12));
    f0 = a1_flt(A1_R32B(a2, 12));
    f2 = a1f::mul(f12, f2);
    f0 = a1f::mul(f13, f0);
    f2 = a1f::addFinite(f2, f0);
    A1_W32B(a0, 12, a1_bits(f2));
    f1 = a1_flt(A1_R32B(a1, 16));
    f0 = a1_flt(A1_R32B(a2, 16));
    f1 = a1f::mul(f12, f1);
    f0 = a1f::mul(f13, f0);
    f1 = a1f::addFinite(f1, f0);
    A1_W32B(a0, 16, a1_bits(f1));
    f2 = a1_flt(A1_R32B(a1, 20));
    f0 = a1_flt(A1_R32B(a2, 20));
    f2 = a1f::mul(f12, f2);
    f0 = a1f::mul(f13, f0);
    f2 = a1f::addFinite(f2, f0);
    A1_W32B(a0, 20, a1_bits(f2));
    f1 = a1_flt(A1_R32B(a1, 24));
    f0 = a1_flt(A1_R32B(a2, 24));
    f1 = a1f::mul(f12, f1);
    f0 = a1f::mul(f13, f0);
    f1 = a1f::addFinite(f1, f0);
    A1_W32B(a0, 24, a1_bits(f1));
    f2 = a1_flt(A1_R32B(a1, 28));
    f0 = a1_flt(A1_R32B(a2, 28));
    f2 = a1f::mul(f12, f2);
    f0 = a1f::mul(f13, f0);
    f2 = a1f::addFinite(f2, f0);
    A1_W32B(a0, 28, a1_bits(f2));
    f1 = a1_flt(A1_R32B(a1, 32));
    f0 = a1_flt(A1_R32B(a2, 32));
    f1 = a1f::mul(f12, f1);
    f0 = a1f::mul(f13, f0);
    f1 = a1f::addFinite(f1, f0);
    A1_W32B(a0, 32, a1_bits(f1));
    f2 = a1_flt(A1_R32B(a1, 36));
    f0 = a1_flt(A1_R32B(a2, 36));
    f2 = a1f::mul(f12, f2);
    f0 = a1f::mul(f13, f0);
    f2 = a1f::addFinite(f2, f0);
    A1_W32B(a0, 36, a1_bits(f2));
    f1 = a1_flt(A1_R32B(a1, 40));
    f0 = a1_flt(A1_R32B(a2, 40));
    f1 = a1f::mul(f12, f1);
    f0 = a1f::mul(f13, f0);
    f1 = a1f::addFinite(f1, f0);
    A1_W32B(a0, 40, a1_bits(f1));
    f2 = a1_flt(A1_R32B(a1, 44));
    f0 = a1_flt(A1_R32B(a2, 44));
    f2 = a1f::mul(f12, f2);
    f0 = a1f::mul(f13, f0);
    f2 = a1f::addFinite(f2, f0);
    A1_W32B(a0, 44, a1_bits(f2));
    f1 = a1_flt(A1_R32B(a1, 48));
    f0 = a1_flt(A1_R32B(a2, 48));
    f1 = a1f::mul(f12, f1);
    f0 = a1f::mul(f13, f0);
    f1 = a1f::addFinite(f1, f0);
    A1_W32B(a0, 48, a1_bits(f1));
    f2 = a1_flt(A1_R32B(a1, 52));
    f0 = a1_flt(A1_R32B(a2, 52));
    f2 = a1f::mul(f12, f2);
    f0 = a1f::mul(f13, f0);
    f2 = a1f::addFinite(f2, f0);
    A1_W32B(a0, 52, a1_bits(f2));
    f1 = a1_flt(A1_R32B(a1, 56));
    f0 = a1_flt(A1_R32B(a2, 56));
    f1 = a1f::mul(f12, f1);
    f0 = a1f::mul(f13, f0);
    f1 = a1f::addFinite(f1, f0);
    A1_W32B(a0, 56, a1_bits(f1));
    f2 = a1_flt(A1_R32B(a1, 60));
    f0 = a1_flt(A1_R32B(a2, 60));
    f2 = a1f::mul(f12, f2);
    f0 = a1f::mul(f13, f0);
    f2 = a1f::addFinite(f2, f0);
    A1_W32B(a0, 60, a1_bits(f2));
    f1 = a1_flt(A1_R32B(a1, 64));
    f0 = a1_flt(A1_R32B(a2, 64));
    f1 = a1f::mul(f12, f1);
    f0 = a1f::mul(f13, f0);
    f1 = a1f::addFinite(f1, f0);
    A1_W32B(a0, 64, a1_bits(f1));
    f2 = a1_flt(A1_R32B(a1, 68));
    f0 = a1_flt(A1_R32B(a2, 68));
    f2 = a1f::mul(f12, f2);
    f0 = a1f::mul(f13, f0);
    f2 = a1f::addFinite(f2, f0);
    A1_W32B(a0, 68, a1_bits(f2));
    f1 = a1_flt(A1_R32B(a1, 72));
    f0 = a1_flt(A1_R32B(a2, 72));
    f1 = a1f::mul(f12, f1);
    f0 = a1f::mul(f13, f0);
    f1 = a1f::addFinite(f1, f0);
    A1_W32B(a0, 72, a1_bits(f1));
    f2 = a1_flt(A1_R32B(a1, 76));
    f0 = a1_flt(A1_R32B(a2, 76));
    f2 = a1f::mul(f12, f2);
    f0 = a1f::mul(f13, f0);
    f2 = a1f::addFinite(f2, f0);
    A1_W32B(a0, 76, a1_bits(f2));
    f1 = a1_flt(A1_R32B(a1, 80));
    f0 = a1_flt(A1_R32B(a2, 80));
    f1 = a1f::mul(f12, f1);
    f0 = a1f::mul(f13, f0);
    f1 = a1f::addFinite(f1, f0);
    A1_W32B(a0, 80, a1_bits(f1));
    f2 = a1_flt(A1_R32B(a1, 84));
    f0 = a1_flt(A1_R32B(a2, 84));
    f2 = a1f::mul(f12, f2);
    f0 = a1f::mul(f13, f0);
    f2 = a1f::addFinite(f2, f0);
    A1_W32B(a0, 84, a1_bits(f2));
    f1 = a1_flt(A1_R32B(a1, 88));
    f0 = a1_flt(A1_R32B(a2, 88));
    f1 = a1f::mul(f12, f1);
    f0 = a1f::mul(f13, f0);
    f1 = a1f::addFinite(f1, f0);
    A1_W32B(a0, 88, a1_bits(f1));
    f2 = a1_flt(A1_R32B(a1, 92));
    f0 = a1_flt(A1_R32B(a2, 92));
    f2 = a1f::mul(f12, f2);
    f0 = a1f::mul(f13, f0);
    f2 = a1f::addFinite(f2, f0);
    A1_W32B(a0, 92, a1_bits(f2));
    f1 = a1_flt(A1_R32B(a1, 96));
    f0 = a1_flt(A1_R32B(a2, 96));
    f1 = a1f::mul(f12, f1);
    f0 = a1f::mul(f13, f0);
    f1 = a1f::addFinite(f1, f0);
    A1_W32B(a0, 96, a1_bits(f1));
    f2 = a1_flt(A1_R32B(a1, 100));
    f0 = a1_flt(A1_R32B(a2, 100));
    f2 = a1f::mul(f12, f2);
    f0 = a1f::mul(f13, f0);
    f2 = a1f::addFinite(f2, f0);
    A1_W32B(a0, 100, a1_bits(f2));
    f1 = a1_flt(A1_R32B(a1, 104));
    f0 = a1_flt(A1_R32B(a2, 104));
    f1 = a1f::mul(f12, f1);
    f0 = a1f::mul(f13, f0);
    f1 = a1f::addFinite(f1, f0);
    A1_W32B(a0, 104, a1_bits(f1));
    f2 = a1_flt(A1_R32B(a1, 108));
    f0 = a1_flt(A1_R32B(a2, 108));
    f2 = a1f::mul(f12, f2);
    f0 = a1f::mul(f13, f0);
    f2 = a1f::addFinite(f2, f0);
    A1_W32B(a0, 108, a1_bits(f2));
    f1 = a1_flt(A1_R32B(a1, 112));
    f0 = a1_flt(A1_R32B(a2, 112));
    f1 = a1f::mul(f12, f1);
    f0 = a1f::mul(f13, f0);
    f1 = a1f::addFinite(f1, f0);
    A1_W32B(a0, 112, a1_bits(f1));
    f2 = a1_flt(A1_R32B(a1, 116));
    f0 = a1_flt(A1_R32B(a2, 116));
    f2 = a1f::mul(f12, f2);
    f0 = a1f::mul(f13, f0);
    f2 = a1f::addFinite(f2, f0);
    A1_W32B(a0, 116, a1_bits(f2));
    f1 = a1_flt(A1_R32B(a1, 120));
    f0 = a1_flt(A1_R32B(a2, 120));
    f1 = a1f::mul(f12, f1);
    f0 = a1f::mul(f13, f0);
    f1 = a1f::addFinite(f1, f0);
    A1_W32B(a0, 120, a1_bits(f1));
    f2 = a1_flt(A1_R32B(a1, 124));
    f0 = a1_flt(A1_R32B(a2, 124));
    f2 = a1f::mul(f12, f2);
    f0 = a1f::mul(f13, f0);
    f2 = a1f::addFinite(f2, f0);
    A1_W32B(a0, 124, a1_bits(f2));
    f1 = a1_flt(A1_R32B(a1, 128));
    f0 = a1_flt(A1_R32B(a2, 128));
    f1 = a1f::mul(f12, f1);
    f0 = a1f::mul(f13, f0);
    f1 = a1f::addFinite(f1, f0);
    A1_W32B(a0, 128, a1_bits(f1));
    f2 = a1_flt(A1_R32B(a1, 132));
    f0 = a1_flt(A1_R32B(a2, 132));
    f2 = a1f::mul(f12, f2);
    f0 = a1f::mul(f13, f0);
    f2 = a1f::addFinite(f2, f0);
    A1_W32B(a0, 132, a1_bits(f2));
    f2 = a1_flt(A1_R32B(a1, 136));
    fcr31 = (a1f::lt(f3, f2)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
    if ((fcr31 & 0x800000u) != 0u) {
        f1 = a1_flt(A1_R32B(a2, 136));
        fcr31 = (a1f::lt(f3, f1)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
        if ((fcr31 & 0x800000u) != 0u) {
            at = 0x3A830000u; df_at = true;
            at = at | 0x126Eu; df_at = true;
            f0 = a1_flt(at);
            fcr31 = (a1f::eq(f1, f0)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
            if ((fcr31 & 0x800000u) == 0u) {
                f0 = a1f::mul(f12, f2);
                f1 = a1f::mul(f13, f1);
                f0 = a1f::addFinite(f0, f1);
                A1_W32B(a0, 136, a1_bits(f0));
            }
        }
    }
    {
        return f0;
    }
}

// is_base_mode_manager_update_game_camera_param_manager (0x142c118): typed by tools/typed_lift.py (round A1)
inline uint32_t is_base_mode_manager_update_game_camera_param_manager(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t a0)
{
    uint32_t v0 = 0;
    {
        v0 = A1_R32(a0);
        return v0;
    }
}

// get_interp_time_game_camera_param_manager (0x142c128): typed by tools/typed_lift.py (round A1)
inline float get_interp_time_game_camera_param_manager(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t a0)
{
    float f0 = 0.0f;
    float f1 = 0.0f;
    uint32_t fcr31 = 0;
    f0 = a1_flt(A1_R32B(a0, 32));
    f1 = a1_flt(0u);
    fcr31 = (a1f::lt(f0, f1)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
    if ((fcr31 & 0x800000u) != 0u) {
        f0 = a1_flt(A1_R32B(a0, 1648));
    } else {
    }
    {
        return f0;
    }
}

// act_camera_cutout_game_camera_param_manager (0x142c248): typed by tools/typed_lift.py (round A1)
inline uint32_t act_camera_cutout_game_camera_param_manager(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t a0, uint32_t a1)
{
    uint32_t v0 = 0;
    uint32_t v1 = 0;
    bool df_v1 = false;
    a0 = A1_R32B(a0, 8);
    v0 = 1u;
    if (a0 != v0) {
        v0 = 0u;
    } else {
        v1 = A1_R32B(a1, 4960); df_v1 = true;
        if (v1 != a0) {
            v0 = 0u;
        } else {
        }
    }
    {
        return v0;
    }
}

// drop_camera_cutout_flag_game_camera_param_manager (0x142c270): typed by tools/typed_lift.py (round A1)
inline void drop_camera_cutout_flag_game_camera_param_manager(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t a0)
{
    {
        A1_W32B(a0, 8, 0u);
        
        return;
    }
}

// constructor_game_camera_param_update_data_manager (0x142c278): typed by tools/typed_lift.py (round A1)
inline void constructor_game_camera_param_update_data_manager(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t a0)
{
    A1_W32B(a0, 4, 0u);
    {
        A1_W32(a0, 0u);
        
        return;
    }
}

// is_already_set_game_camera_param_update_data_manager (0x142c398): typed by tools/typed_lift.py (round A1)
inline uint32_t is_already_set_game_camera_param_update_data_manager(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t a0, uint32_t a1)
{
    uint32_t v0 = 0;
    uint32_t v1 = 0;
    uint64_t hi = 0;
    uint64_t lo = 0;
    v0 = 0x65Cu;
    v1 = A1_R32B(a0, 4);
    { int64_t result = (int64_t)(int32_t)a1 * (int64_t)(int32_t)v0; lo = (uint64_t)(int64_t)(int32_t)result; hi = (uint64_t)(int64_t)(int32_t)(result >> 32); (a1 = (uint32_t)((int32_t)((int32_t)result))); }
    a1 = a1 + v1;
    v0 = A1_R32B(a1, 4);
    {
        v0 = 0u < v0 ? 1u : 0u;
        return v0;
    }
}

// constructor_game_camera_mode_local_parameter (0x142c3b8): typed by tools/typed_lift.py (round A1)
inline float constructor_game_camera_mode_local_parameter(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t a0)
{
    uint32_t at = 0;
    float f0 = 0.0f;
    float f1 = 0.0f;
    float f2 = 0.0f;
    float f3 = 0.0f;
    float f4 = 0.0f;
    at = 0x40000000u;
    f2 = a1_flt(at);
    at = 0x3F000000u;
    f3 = a1_flt(at);
    at = 0x40900000u;
    f1 = a1_flt(at);
    at = 0x42340000u;
    f4 = a1_flt(at);
    at = 0x3F800000u;
    f0 = a1_flt(at);
    A1_W32B(a0, 28, a1_bits(f1));
    A1_W32B(a0, 60, a1_bits(f0));
    A1_W32B(a0, 40, a1_bits(f2));
    A1_W32B(a0, 52, a1_bits(f3));
    A1_W32B(a0, 56, a1_bits(f4));
    A1_W32(a0, 0u);
    A1_W32B(a0, 4, 0u);
    A1_W32B(a0, 8, 0u);
    A1_W32B(a0, 12, 0u);
    A1_W32B(a0, 16, 0u);
    A1_W32B(a0, 20, 0u);
    A1_W32B(a0, 24, 0u);
    A1_W32B(a0, 32, a1_bits(f2));
    A1_W32B(a0, 36, a1_bits(f2));
    A1_W32B(a0, 44, a1_bits(f3));
    {
        A1_W32B(a0, 48, a1_bits(f3));
        return f0;
    }
}

// gamecore_sub_0142C440 (0x142c440): typed by tools/typed_lift.py (round A1)
inline float gamecore_sub_0142C440(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t a0, uint32_t a1, uint32_t a2, float f12, float f13)
{
    uint32_t at = 0;
    float f0 = 0.0f;
    float f1 = 0.0f;
    float f2 = 0.0f;
    float f3 = 0.0f;
    float f4 = 0.0f;
    float f5 = 0.0f;
    uint32_t fcr31 = 0;
    f5 = a1_flt(A1_R32B(a2, 200));
    f3 = a1_flt(A1_R32B(a1, 200));
    f1 = ((float)(f5));
    f0 = a1_flt(0u);
    f4 = ((float)(f3));
    f2 = a1f::sub(f1, f4);
    fcr31 = (a1f::lt(f2, f0)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
    if ((fcr31 & 0x800000u) != 0u) {
        at = 0xC3340000u;
        f0 = a1_flt(at);
        fcr31 = (a1f::le(f0, f2)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
        if ((fcr31 & 0x800000u) != 0u) {
            f1 = a1f::mul(f12, f3);
            goto L_142c498;
        }
        goto L_142c4a8;
    }
    at = 0x43340000u;
    f0 = a1_flt(at);
    fcr31 = (a1f::le(f2, f0)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
    if ((fcr31 & 0x800000u) == 0u) {
        goto L_142c4a8;
    }
    f1 = a1f::mul(f12, f3);
L_142c498:
    f0 = a1f::mul(f13, f5);
    f1 = a1f::add(f1, f0);
    goto L_142c50c;
L_142c4a8:
    f0 = a1_flt(0u);
    fcr31 = (a1f::lt(f3, f0)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
    if ((fcr31 & 0x800000u) != 0u) {
        at = 0x43B40000u;
        f0 = a1_flt(at);
        f4 = a1f::add(f3, f0);
        goto L_142c4e0;
    }
    fcr31 = (a1f::lt(f5, f0)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
    if ((fcr31 & 0x800000u) == 0u) {
        f0 = a1f::mul(f12, f4);
        goto L_142c4e4;
    }
    at = 0x43B40000u;
    f0 = a1_flt(at);
    f1 = a1f::add(f5, f0);
L_142c4e0:
    f0 = a1f::mul(f12, f4);
L_142c4e4:
    at = 0x43340000u;
    f2 = a1_flt(at);
    f1 = a1f::mul(f13, f1);
    f1 = a1f::add(f0, f1);
    fcr31 = (a1f::lt(f2, f1)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
    if ((fcr31 & 0x800000u) == 0u) {
        A1_W32B(a0, 200, a1_bits(f1));
        goto L_142c510;
    }
    at = 0x43B40000u;
    f0 = a1_flt(at);
    f1 = a1f::subFinite(f1, f0);
L_142c50c:
    A1_W32B(a0, 200, a1_bits(f1));
L_142c510:
    f1 = a1_flt(A1_R32B(a1, 204));
    f0 = a1_flt(A1_R32B(a2, 204));
    f1 = a1f::mul(f12, f1);
    f0 = a1f::mul(f13, f0);
    f1 = a1f::addFinite(f1, f0);
    {
        A1_W32B(a0, 204, a1_bits(f1));
        return f0;
    }
}

// gamecore_sub_0142C530 (0x142c530): typed by tools/typed_lift.py (round A1)
inline uint64_t gamecore_sub_0142C530(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t a0, uint64_t a1)
{
    uint32_t v1 = 0;
    uint32_t a2 = 0;
    uint64_t v0 = 0;
    bool df_v1 = false, df_a2 = false;
    {
        const bool c = (int64_t)a1 == 0;
        a0 = a0 + 0x4Cu;
        if (!c) {
            (a1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)a1, 76))));
            a2 = A1_R32B(a0, 140); df_a2 = true;
            (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)a1, 140))));
            {
                const bool c = (int64_t)(int32_t)a2 != (int64_t)v0;
                (v0 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL));
                if (c) {
                    goto L_142c5c4;
                }
            }
            v1 = A1_R32B(a0, 168); df_v1 = true;
            (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)a1, 168))));
            {
                const bool c = (int64_t)(int32_t)v1 != (int64_t)v0;
                (v0 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL));
                if (c) {
                    goto L_142c5c4;
                }
            }
            v1 = 1u; df_v1 = true;
            {
                const bool c = a2 == v1;
                (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 1))));
                if (c) {
                    goto L_142c5c4;
                }
            }
            a0 = A1_R32B(a0, 144);
            (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)a1, 144))));
            {
                const bool c = (int64_t)(int32_t)a0 != (int64_t)v0;
                (v0 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL));
                if (c) {
                    goto L_142c5c4;
                }
            }
            (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(a0, 4294967196))));
            (v0 = (uint64_t)(((uint64_t)RC_U64X(v0) < (uint64_t)(int64_t)(int32_t)3) ? 1 : 0));
            {
                const bool c = (int64_t)v0 == 0;
                (v0 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL));
                if (c) {
                    goto L_142c5c4;
                }
            }
            if (a2 != 0) {
                (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 4))));
                if ((int64_t)(int32_t)a2 != (int64_t)v0) {
                    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 5))));
                    if ((int64_t)(int32_t)a2 != (int64_t)v0) {
                        (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 6))));
                        if ((int64_t)(int32_t)a2 != (int64_t)v0) {
                            (v0 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL));
                            goto L_142c5c4;
                        }
                    }
                }
            }
            {
                (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 1))));
                return v0;
            }
        }
    }
    (v0 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL));
L_142c5c4:
    {
        return v0;
    }
}

// setBossPathColiGroup (0x1431c38): typed by tools/typed_lift.py (round A1)
inline uint32_t setBossPathColiGroup(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t a0)
{
    uint32_t v0 = 0;
    v0 = 0x1200000u;
    {
        A1_W32(v0 - 0x4340u, a0);
        return v0;
    }
}

// setHorsePathColiGroup (0x1431c48): typed by tools/typed_lift.py (round A1)
inline uint32_t setHorsePathColiGroup(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t a0)
{
    uint32_t v0 = 0;
    v0 = 0x1200000u;
    {
        A1_W32(v0 - 0x433Cu, a0);
        return v0;
    }
}

// getPathColiGroupByType (0x1431c70): typed by tools/typed_lift.py (round A1)
inline uint32_t getPathColiGroupByType(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint64_t a0)
{
    uint32_t v0 = 0;
    uint32_t v1 = 0;
    if ((int64_t)a0 != 0) {
        v0 = 1u;
    } else {
        v1 = 0x1200000u;
        {
            v0 = A1_R32(v1 - 0x4340u);
            return v0;
        }
    }
    {
        const bool c = (int64_t)a0 != (int64_t)(int32_t)v0;
        v1 = 0x1200000u;
        if (!c) {
            {
                v0 = A1_R32(v1 - 0x433Cu);
                return v0;
            }
        }
    }
    {
        v0 = 0u;
        return v0;
    }
}

// getPassageWork (0x1431ce0): typed by tools/typed_lift.py (round A1)
inline uint64_t getPassageWork(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t a0, uint64_t a1)
{
    uint64_t v0 = 0;
    (v0 = (uint64_t)(((int64_t)(int64_t)a1 < (int64_t)(int32_t)20) ? 1 : 0));
    if ((int64_t)v0 != 0) {
        (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B(a0, 896))));
        (v0 = (uint64_t)(((uint64_t)a1 < (uint64_t)RC_U64X(v0)) ? 1 : 0));
        if ((int64_t)v0 != 0) {
            (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)SLL32((uint32_t)a1, 5))));
            (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(a0, (uint32_t)v0))));
            {
                (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v0, 256))));
                return v0;
            }
        }
    }
    {
        (v0 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL));
        return v0;
    }
}

// getPassageNum (0x1431d18): typed by tools/typed_lift.py (round A1)
inline uint32_t getPassageNum(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t a0)
{
    uint32_t v0 = 0;
    {
        v0 = A1_R32B(a0, 896);
        return v0;
    }
}

// setRealColJudgeFunc (0x1431ed0): typed by tools/typed_lift.py (round A1)
inline void setRealColJudgeFunc(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t a0, uint32_t a1)
{
    {
        A1_W32B(a0, 15444, a1);
        
        return;
    }
}

// getRealColJudgeFunc (0x1431ed8): typed by tools/typed_lift.py (round A1)
inline uint32_t getRealColJudgeFunc(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t a0)
{
    uint32_t v0 = 0;
    {
        v0 = A1_R32B(a0, 15444);
        return v0;
    }
}

// setSelfColAndNearMinDist (0x1431ee0): typed by tools/typed_lift.py (round A1)
inline void setSelfColAndNearMinDist(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t a0, uint32_t a1, float f12)
{
    A1_W32B(a0, 964, a1);
    {
        A1_W32B(a0, 960, a1_bits(f12));
        
        return;
    }
}

// setStabilizeYPathWork (0x1431f38): typed by tools/typed_lift.py (round A1)
inline void setStabilizeYPathWork(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t a0, uint32_t a1)
{
    {
        A1_W32B(a0, 232, a1);
        
        return;
    }
}

// setAngleStabilizeYPathWork (0x1431f40): typed by tools/typed_lift.py (round A1)
inline void setAngleStabilizeYPathWork(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t a0, uint32_t a1)
{
    {
        A1_W32B(a0, 236, a1);
        
        return;
    }
}

// setRayByRealColPathWork (0x1431f48): typed by tools/typed_lift.py (round A1)
inline void setRayByRealColPathWork(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t a0, uint32_t a1)
{
    {
        A1_W32B(a0, 240, a1);
        
        return;
    }
}

// setPassageWideRayPathWork (0x1431f50): typed by tools/typed_lift.py (round A1)
inline void setPassageWideRayPathWork(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t a0, uint32_t a1)
{
    {
        A1_W32B(a0, 244, a1);
        
        return;
    }
}

// setExecRayChkPathWork (0x1431f58): typed by tools/typed_lift.py (round A1)
inline void setExecRayChkPathWork(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t a0, uint32_t a1)
{
    {
        A1_W32B(a0, 900, a1);
        
        return;
    }
}

// setForceRouteAttrPathWork (0x1431fa8): typed by tools/typed_lift.py (round A1)
inline void setForceRouteAttrPathWork(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t a0, uint32_t a1)
{
    {
        A1_W32B(a0, 908, a1);
        
        return;
    }
}

// setRouteColCheckPathWork (0x1431fb0): typed by tools/typed_lift.py (round A1)
inline void setRouteColCheckPathWork(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t a0, uint32_t a1)
{
    {
        A1_W32B(a0, 912, a1);
        
        return;
    }
}

// setRouteLockPathWork (0x1431fb8): typed by tools/typed_lift.py (round A1)
inline void setRouteLockPathWork(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t a0, uint32_t a1)
{
    {
        A1_W32B(a0, 916, a1);
        
        return;
    }
}

// setPlayerWayColCheckPathWork (0x1431fc0): typed by tools/typed_lift.py (round A1)
inline void setPlayerWayColCheckPathWork(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t a0, uint32_t a1)
{
    {
        A1_W32B(a0, 920, a1);
        
        return;
    }
}

// setFollowModePathWork (0x1431fc8): typed by tools/typed_lift.py (round A1)
inline void setFollowModePathWork(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t a0, uint32_t a1)
{
    {
        A1_W32B(a0, 924, a1);
        
        return;
    }
}

// setUserPathMode (0x1432018): typed by tools/typed_lift.py (round A1)
inline void setUserPathMode(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t a0, uint32_t a1)
{
    {
        A1_W32B(a0, 928, a1);
        
        return;
    }
}

// setPassageRangeData (0x1432020): typed by tools/typed_lift.py (round A1)
inline void setPassageRangeData(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t a0, float f12, float f13)
{
    A1_W32B(a0, 976, a1_bits(f13));
    {
        A1_W32B(a0, 972, a1_bits(f12));
        
        return;
    }
}

// setAttainRangeStruct (0x1432050): typed by tools/typed_lift.py (round A1)
inline uint64_t setAttainRangeStruct(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t a0, uint32_t a1)
{
    uint64_t v0 = 0;
    uint64_t v1 = 0;
    (v0 = (uint64_t)(A1_R64B(a1, 0)));
    A1_W64B(a0, 992, v0);
    (v1 = (uint64_t)(A1_R64B(a1, 8)));
    A1_W64B(a0, 1000, v1);
    (v0 = (uint64_t)(A1_R64B(a1, 16)));
    A1_W64B(a0, 1008, v0);
    (v1 = (uint64_t)(A1_R64B(a1, 24)));
    {
        A1_W64B(a0, 1016, v1);
        return v0;
    }
}

// setCreateTempAttain (0x14320b0): typed by tools/typed_lift.py (round A1)
inline void setCreateTempAttain(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t a0, uint32_t a1)
{
    {
        A1_W32B(a0, 1044, a1);
        
        return;
    }
}

// setAttainModePathWork (0x1432700): typed by tools/typed_lift.py (round A1)
inline void setAttainModePathWork(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t a0, uint32_t a1)
{
    {
        A1_W32B(a0, 224, a1);
        
        return;
    }
}

// setAttainRadPathWork (0x1432708): typed by tools/typed_lift.py (round A1)
inline void setAttainRadPathWork(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t a0, float f12)
{
    {
        A1_W32B(a0, 228, a1_bits(f12));
        
        return;
    }
}

// getPathInfo (0x1432710): typed by tools/typed_lift.py (round A1)
inline uint32_t getPathInfo(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t a0)
{
    uint32_t at = 0;
    uint32_t v0 = 0;
    uint32_t v1 = 0;
    v1 = A1_R32B(a0, 15440);
    v0 = v1 << 2;
    v0 = v0 + v1;
    v0 = v0 << 2;
    v0 = v0 - v1;
    v0 = v0 << 3;
    at = 0x1200000u;
    at = at + 0x10C8u;
    v0 = at + v0;
    {
        return v0;
    }
}

// getWayFinish (0x1432740): typed by tools/typed_lift.py (round A1)
inline uint64_t getWayFinish(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t a0)
{
    uint64_t v0 = 0;
    uint64_t v1 = 0;
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B(a0, 1032))));
    {
        const bool c = (int64_t)v0 == 0;
        (v1 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL));
        if (!c) {
            (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B(a0, 896))));
            (v1 = (uint64_t)(((uint64_t)RC_U64X(v0) < (uint64_t)(int64_t)(int32_t)1) ? 1 : 0));
        }
    }
    {
        (v0 = (uint64_t)((uint64_t)RC_U64X(v1) + (uint64_t)0ULL));
        return v0;
    }
}

// getPathStatus (0x1432760): typed by tools/typed_lift.py (round A1)
inline uint32_t getPathStatus(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t a0)
{
    uint32_t v0 = 0;
    uint32_t v1 = 0;
    uint32_t a1 = 0;
    bool df_v1 = false;
    a1 = A1_R32B(a0, 928);
    if (a1 == 0) {
        v0 = A1_R32B(a0, 896);
    } else {
        {
            const bool c = (int32_t)a1 > 0;
            v0 = 1u;
            if (!c) {
                v1 = 0xFFFFFFFFu; df_v1 = true;
                if (a1 == v1) {
                    goto L_14327c8;
                }
                {
                    v0 = 0u;
                    return v0;
                }
            }
        }
        {
            const bool c = a1 == v0;
            v0 = 0u;
            if (c) {
                goto L_14327b8;
            }
        }
        goto L_14327c8;
    }
    if (v0 == 0) {
        v0 = A1_R32B(a0, 1032);
        if (v0 == 0) {
            v0 = 0u;
            goto L_14327c8;
        }
    }
    {
        v0 = 1u;
        return v0;
    }
L_14327b8:
    v1 = A1_R32B(a0, 896); df_v1 = true;
    {
        const bool c = v1 != 0;
        v0 = 1u;
        if (!c) {
            v0 = 0u;
        }
    }
L_14327c8:
    {
        return v0;
    }
}

// getLastPassagePointIdx (0x1432808): typed by tools/typed_lift.py (round A1)
inline uint32_t getLastPassagePointIdx(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t a0)
{
    uint32_t v0 = 0;
    {
        v0 = A1_R32B(a0, 1060);
        return v0;
    }
}

// getLastPassageAreaIdx (0x1432810): typed by tools/typed_lift.py (round A1)
inline uint32_t getLastPassageAreaIdx(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t a0)
{
    uint32_t v0 = 0;
    {
        v0 = A1_R32B(a0, 1064);
        return v0;
    }
}

// getNowPassagePointIdx (0x1432818): typed by tools/typed_lift.py (round A1)
inline uint32_t getNowPassagePointIdx(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t a0)
{
    uint32_t v0 = 0;
    {
        v0 = A1_R32B(a0, 1068);
        return v0;
    }
}

// getNowPassageAreaIdx (0x1432820): typed by tools/typed_lift.py (round A1)
inline uint32_t getNowPassageAreaIdx(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t a0)
{
    uint32_t v0 = 0;
    {
        v0 = A1_R32B(a0, 1072);
        return v0;
    }
}

// setSearchRadius (0x14328a8): typed by tools/typed_lift.py (round A1)
inline a1::RetVF setSearchRadius(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t a0, float f12)
{
    uint32_t v0 = 0;
    float f0 = 0.0f;
    uint32_t fcr31 = 0;
    f0 = a1_flt(0u);
    fcr31 = (a1f::lt(f12, f0)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
    {
        const bool c = (fcr31 & 0x800000u) == 0u;
        v0 = 0x1470000u;
        if (!c) {
            f0 = a1_flt(A1_R32(v0 - 0x38F4u));
            f0 = a1f::sqrt(fcr31, f0);
            {
                A1_W32B(a0, 952, a1_bits(f0));
                return a1::RetVF{(uint64_t)a1_sx64(v0), f0};
            }
        }
    }
    {
        A1_W32B(a0, 952, a1_bits(f12));
        return a1::RetVF{(uint64_t)a1_sx64(v0), f0};
    }
}

// setSearchPointMax (0x14328d0): typed by tools/typed_lift.py (round A1)
inline uint32_t setSearchPointMax(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t a0, uint64_t a1)
{
    uint32_t v0 = 0;
    {
        const bool c = (int32_t)a1 > 0;
        v0 = 0x380u;
        if (!c) {
            {
                A1_W32B(a0, 956, v0);
                return v0;
            }
        }
    }
    {
        A1_W32B(a0, 956, (uint32_t)a1);
        return v0;
    }
}

// setPathRayWideParam (0x14328e8): typed by tools/typed_lift.py (round A1)
inline void setPathRayWideParam(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t a0, uint32_t a1, float f12)
{
    A1_W32B(a0, 15424, a1);
    {
        A1_W32B(a0, 15420, a1_bits(f12));
        
        return;
    }
}

// resetPassageWork (0x1433120): typed by tools/typed_lift.py (round A1)
inline void resetPassageWork(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t a0)
{
    {
        A1_W32B(a0, 896, 0u);
        
        return;
    }
}

// getColiGroupByUserPathWork (0x1433f60): typed by tools/typed_lift.py (round A1)
inline uint32_t getColiGroupByUserPathWork(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t a0)
{
    uint32_t v0 = 0;
    uint32_t v1 = 0;
    v1 = A1_R32B(a0, 15440);
    if (v1 != 0) {
        v0 = 1u;
    } else {
        v0 = 0x1460000u;
        {
            v0 = v0 + 0x5888u;
            return v0;
        }
    }
    if (v1 == v0) {
        v0 = 0x1460000u;
        {
            v0 = v0 + 0x5884u;
            return v0;
        }
    }
    {
        v0 = 0u;
        return v0;
    }
}

// gamecore_sub_01434CF8 (0x1434cf8): typed by tools/typed_lift.py (round A1)
inline uint64_t gamecore_sub_01434CF8(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint64_t a0)
{
    uint32_t v1 = 0;
    uint64_t v0 = 0;
    uint64_t a1 = 0;
    (a1 = (uint64_t)((uint64_t)a0 + (uint64_t)0ULL));
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 1))));
    (v1 = (uint32_t)((int32_t)((int32_t)A1_R32B((uint32_t)a1, 15440))));
    {
        const bool c = (int64_t)(int32_t)v1 != (int64_t)a0;
        (v0 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL));
        if (!c) {
            (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)a1, 920))));
            {
                const bool c = (int64_t)v0 == 0;
                (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)323 << 16))));
                if (!c) {
                    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)288 << 16))));
                    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)v0, 4908))));
                    if ((int64_t)v0 == 0) {
                        (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)323 << 16))));
                    } else {
                        (v1 = (uint32_t)((int32_t)((int32_t)A1_R32B((uint32_t)v0, 1456))));
                        {
                            const bool c = v1 != 0;
                            (v0 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL));
                            if (c) {
                                goto L_1434d3c;
                            }
                        }
                        (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)323 << 16))));
                    }
                }
            }
            (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v0, 20416))));
        }
    }
L_1434d3c:
    {
        return v0;
    }
}

// gamecore_sub_01434F90 (0x1434f90): typed by tools/typed_lift.py (round A1)
inline a1::RetVF gamecore_sub_01434F90(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t a0)
{
    uint32_t v1 = 0;
    uint64_t v0 = 0;
    float f0 = 0.0f;
    float f1 = 0.0f;
    uint32_t fcr31 = 0;
    bool df_v1 = false;
    f1 = a1_flt(0u);
    f0 = a1_flt(A1_R32B(a0, 15420));
    fcr31 = (a1f::lt(f1, f0)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
    {
        const bool c = (fcr31 & 0x800000u) == 0u;
        (v0 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL));
        if (!c) {
            v1 = A1_R32B(a0, 15424); df_v1 = true;
            {
                const bool c = (int32_t)v1 > 0;
                (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 1))));
                if (!c) {
                    (v0 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL));
                }
            }
        }
    }
    {
        return a1::RetVF{(uint64_t)v0, f0};
    }
}

// setUserPathExMode (0x14359c8): typed by tools/typed_lift.py (round A1)
inline void setUserPathExMode(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t a0, uint32_t a1)
{
    {
        A1_W32B(a0, 15456, a1);
        
        return;
    }
}

// setUserPathGroupID (0x14359d0): typed by tools/typed_lift.py (round A1)
inline void setUserPathGroupID(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t a0, uint32_t a1, uint32_t a2)
{
    A1_W32B(a0, 15468, a2);
    {
        A1_W32B(a0, 15464, a1);
        
        return;
    }
}

// setGPSelectMode (0x1435a50): typed by tools/typed_lift.py (round A1)
inline void setGPSelectMode(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t a0, uint32_t a1)
{
    {
        A1_W32B(a0, 15480, a1);
        
        return;
    }
}

// setForceInitPathEx (0x1435a58): typed by tools/typed_lift.py (round A1)
inline void setForceInitPathEx(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t a0, uint32_t a1)
{
    {
        A1_W32B(a0, 15484, a1);
        
        return;
    }
}

// setGpReferencePathEx (0x1435a60): typed by tools/typed_lift.py (round A1)
inline void setGpReferencePathEx(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t a0, uint32_t a1)
{
    {
        A1_W32B(a0, 15488, a1);
        
        return;
    }
}

// gamecore_sub_01435EF0 (0x1435ef0): typed by tools/typed_lift.py (round A1)
inline uint32_t gamecore_sub_01435EF0(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t a0)
{
    uint32_t v0 = 0;
    v0 = 0xFFFFFFFFu;
    A1_W32B(a0, 15516, v0);
    {
        A1_W32B(a0, 15512, v0);
        return v0;
    }
}

// _setExecBaseMoveWork (0x1437a60): typed by tools/typed_lift.py (round A1)
inline void _setExecBaseMoveWork(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t a0, uint32_t a1)
{
    {
        A1_W32B(a0, 4, a1);
        
        return;
    }
}

// _setStopCtrlModeBaseMoveWork (0x1437a68): typed by tools/typed_lift.py (round A1)
inline void _setStopCtrlModeBaseMoveWork(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t a0, uint32_t a1)
{
    {
        A1_W32B(a0, 32, a1);
        
        return;
    }
}

// _getStopCtrlModeBaseMoveWork (0x1437a70): typed by tools/typed_lift.py (round A1)
inline uint32_t _getStopCtrlModeBaseMoveWork(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t a0)
{
    uint32_t v0 = 0;
    {
        v0 = A1_R32B(a0, 32);
        return v0;
    }
}

// _setBackVolumeBaseMoveWork (0x1437a78): typed by tools/typed_lift.py (round A1)
inline void _setBackVolumeBaseMoveWork(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t a0, uint32_t a1)
{
    {
        A1_W32B(a0, 36, a1);
        
        return;
    }
}

// _setSlideVolumeBaseMoveWork (0x1437a80): typed by tools/typed_lift.py (round A1)
inline void _setSlideVolumeBaseMoveWork(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t a0, uint32_t a1)
{
    {
        A1_W32B(a0, 40, a1);
        
        return;
    }
}

// _setAttainFlagBaseMoveWork (0x1437b88): typed by tools/typed_lift.py (round A1)
inline void _setAttainFlagBaseMoveWork(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t a0, uint32_t a1)
{
    {
        A1_W32(a0, a1);
        
        return;
    }
}

// gamecore_sub_01438448 (0x1438448): typed by tools/typed_lift.py (round A1)
inline a1::RetVF gamecore_sub_01438448(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t a0)
{
    uint32_t at = 0;
    uint32_t v0 = 0;
    float f0 = 0.0f;
    float f1 = 0.0f;
    v0 = 0x1470000u;
    at = 0x3F800000u;
    f1 = a1_flt(at);
    f0 = a1_flt(A1_R32(v0 - 0x37F0u));
    A1_W32B(a0, 28, a1_bits(f1));
    A1_W32B(a0, 24, a1_bits(f0));
    A1_W32B(a0, 16, a1_bits(f0));
    {
        A1_W32B(a0, 20, a1_bits(f0));
        return a1::RetVF{(uint64_t)a1_sx64(v0), f0};
    }
}

// resetRouteAttackBirdCtrlWork (0x143d5c8): typed by tools/typed_lift.py (round A1)
inline void resetRouteAttackBirdCtrlWork(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t a0)
{
    A1_W32B(a0, 340, 0u);
    {
        A1_W32B(a0, 48, 0u);
        
        return;
    }
}

// setFlapWingBirdCtrlWork (0x143d5d8): typed by tools/typed_lift.py (round A1)
inline void setFlapWingBirdCtrlWork(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t a0, uint32_t a1)
{
    {
        A1_W32B(a0, 52, a1);
        
        return;
    }
}

// setHoveringBirdCtrlWork (0x143d5e0): typed by tools/typed_lift.py (round A1)
inline void setHoveringBirdCtrlWork(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t a0, uint32_t a1)
{
    {
        A1_W32B(a0, 56, a1);
        
        return;
    }
}

// setHoverAttackBirdCtrlWork (0x143d5e8): typed by tools/typed_lift.py (round A1)
inline void setHoverAttackBirdCtrlWork(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t a0, uint32_t a1)
{
    {
        A1_W32B(a0, 60, a1);
        
        return;
    }
}

// gamecore_sub_0143D660 (0x143d660): typed by tools/typed_lift.py (round A1)
inline a1::RetVF gamecore_sub_0143D660(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t a0)
{
    uint32_t at = 0;
    uint32_t v0 = 0;
    float f0 = 0.0f;
    float f1 = 0.0f;
    v0 = 0x1470000u;
    at = 0x3F800000u;
    f1 = a1_flt(at);
    f0 = a1_flt(A1_R32(v0 - 0x379Cu));
    A1_W32B(a0, 92, a1_bits(f1));
    A1_W32B(a0, 88, a1_bits(f0));
    A1_W32B(a0, 64, 0u);
    A1_W32B(a0, 96, 0u);
    A1_W32B(a0, 68, 0u);
    A1_W32B(a0, 80, a1_bits(f0));
    {
        A1_W32B(a0, 84, a1_bits(f0));
        return a1::RetVF{(uint64_t)a1_sx64(v0), f0};
    }
}

// setDashFastLegCtrlWork (0x1440ef0): typed by tools/typed_lift.py (round A1)
inline void setDashFastLegCtrlWork(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t a0, uint32_t a1)
{
    {
        A1_W32B(a0, 52, a1);
        
        return;
    }
}

// setFinalAttainWalkFastLegCtrlWork (0x1440ef8): typed by tools/typed_lift.py (round A1)
inline void setFinalAttainWalkFastLegCtrlWork(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t a0, uint32_t a1)
{
    {
        A1_W32B(a0, 48, a1);
        
        return;
    }
}

// setAllowMaxSpeedFastLegCtrlWork (0x1440f00): typed by tools/typed_lift.py (round A1)
inline float setAllowMaxSpeedFastLegCtrlWork(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t a0, float f12)
{
    float f0 = 0.0f;
    uint32_t fcr31 = 0;
    f0 = a1_flt(A1_R32B(a0, 124));
    fcr31 = (a1f::lt(f0, f12)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
    if ((fcr31 & 0x800000u) != 0u) {
        A1_W32B(a0, 80, a1_bits(f12));
    } else {
    }
    {
        return f0;
    }
}

// setBackFastLegCtrlWork (0x1440f18): typed by tools/typed_lift.py (round A1)
inline void setBackFastLegCtrlWork(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t a0, uint32_t a1)
{
    {
        A1_W32B(a0, 56, a1);
        
        return;
    }
}

// setNotFastTurnFastLegCtrlWork (0x1440f20): typed by tools/typed_lift.py (round A1)
inline void setNotFastTurnFastLegCtrlWork(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t a0, uint32_t a1)
{
    {
        A1_W32B(a0, 60, a1);
        
        return;
    }
}

// setRunStopTurnFastLegCtrlWork (0x1440f28): typed by tools/typed_lift.py (round A1)
inline void setRunStopTurnFastLegCtrlWork(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t a0, uint32_t a1)
{
    {
        A1_W32B(a0, 64, a1);
        
        return;
    }
}

// setWalkStopTurnFastLegCtrlWork (0x1440f30): typed by tools/typed_lift.py (round A1)
inline void setWalkStopTurnFastLegCtrlWork(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t a0, uint32_t a1)
{
    {
        A1_W32B(a0, 68, a1);
        
        return;
    }
}

// setCurveCollisionFastLegCtrlWork (0x1440f38): typed by tools/typed_lift.py (round A1)
inline void setCurveCollisionFastLegCtrlWork(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t a0, uint32_t a1)
{
    {
        A1_W32B(a0, 72, a1);
        
        return;
    }
}

// setExecGazeCtrlWork (0x14418d0): typed by tools/typed_lift.py (round A1)
inline void setExecGazeCtrlWork(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t a0, uint32_t a1)
{
    {
        A1_W32(a0, a1);
        
        return;
    }
}

// setViewJudgeGazeCtrlWork (0x14418d8): typed by tools/typed_lift.py (round A1)
inline void setViewJudgeGazeCtrlWork(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t a0, uint32_t a1)
{
    {
        A1_W32B(a0, 4, a1);
        
        return;
    }
}

// setNextDistGazeCtrlWork (0x14418e0): typed by tools/typed_lift.py (round A1)
inline void setNextDistGazeCtrlWork(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t a0, float f12)
{
    {
        A1_W32B(a0, 8, a1_bits(f12));
        
        return;
    }
}

// setValidPosGazeCtrlWork (0x14418e8): typed by tools/typed_lift.py (round A1)
inline void setValidPosGazeCtrlWork(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t a0, uint32_t a1)
{
    {
        A1_W32B(a0, 20, a1);
        
        return;
    }
}

// setBirdModeGazeCtrlWork (0x1441968): typed by tools/typed_lift.py (round A1)
inline void setBirdModeGazeCtrlWork(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t a0, uint32_t a1)
{
    {
        A1_W32B(a0, 12, a1);
        
        return;
    }
}

// setGazeCoffGazeCtrlWork (0x1441970): typed by tools/typed_lift.py (round A1)
inline void setGazeCoffGazeCtrlWork(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t a0, float f12)
{
    {
        A1_W32B(a0, 16, a1_bits(f12));
        
        return;
    }
}

// setDashHorseCtrlWork (0x14444d0): typed by tools/typed_lift.py (round A1)
inline void setDashHorseCtrlWork(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t a0, uint32_t a1)
{
    {
        A1_W32B(a0, 52, a1);
        
        return;
    }
}

// setWallStopHorseCtrlWork (0x14444d8): typed by tools/typed_lift.py (round A1)
inline void setWallStopHorseCtrlWork(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t a0, uint32_t a1)
{
    {
        A1_W32B(a0, 56, a1);
        
        return;
    }
}

// setLimitModeHorseCtrlWork (0x14444e0): typed by tools/typed_lift.py (round A1)
inline void setLimitModeHorseCtrlWork(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t a0, uint32_t a1)
{
    {
        A1_W32B(a0, 68, a1);
        
        return;
    }
}

// setFinalAttainWalkHorseCtrlWork (0x14444e8): typed by tools/typed_lift.py (round A1)
inline void setFinalAttainWalkHorseCtrlWork(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t a0, uint32_t a1)
{
    {
        A1_W32B(a0, 48, a1);
        
        return;
    }
}

// setAllowMaxSpeedHorseCtrlWork (0x14444f0): typed by tools/typed_lift.py (round A1)
inline a1::RetVF setAllowMaxSpeedHorseCtrlWork(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t a0, float f12)
{
    uint32_t v0 = 0;
    float f0 = 0.0f;
    uint32_t fcr31 = 0;
    v0 = A1_R32B(a0, 120);
    f0 = a1_flt(A1_R32(v0));
    fcr31 = (a1f::lt(f0, f12)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
    if ((fcr31 & 0x800000u) != 0u) {
        A1_W32B(a0, 64, a1_bits(f12));
    } else {
    }
    {
        return a1::RetVF{(uint64_t)a1_sx64(v0), f0};
    }
}

// setFreezeHorseCtrlWork (0x1444510): typed by tools/typed_lift.py (round A1)
inline void setFreezeHorseCtrlWork(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t a0, uint32_t a1)
{
    {
        A1_W32B(a0, 60, a1);
        
        return;
    }
}

// chkMoveAngle (0x14460a8): typed by tools/typed_lift.py (round A1)
inline a1::RetVF chkMoveAngle(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, float f12, float f13)
{
    uint32_t at = 0;
    uint32_t v0 = 0;
    float f0 = 0.0f;
    float f1 = 0.0f;
    uint32_t fcr31 = 0;
    f0 = a1f::add(f12, f13);
    at = 0x3C8E0000u;
    at = at | 0xFA35u;
    f1 = a1_flt(at);
    f0 = std::fabs((float)(f0));
    fcr31 = (a1f::le(f0, f1)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
    {
        const bool c = (fcr31 & 0x800000u) != 0u;
        v0 = 1u;
        if (!c) {
            f0 = a1_flt(0u);
            fcr31 = (a1f::le(f0, f13)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
            if ((fcr31 & 0x800000u) != 0u) {
                fcr31 = (a1f::le(f0, f12)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
                if ((fcr31 & 0x800000u) != 0u) {
                    f1 = std::fabs((float)(f12));
                    goto L_1446100;
                }
            }
            fcr31 = (a1f::le(f13, f0)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
            {
                const bool c = (fcr31 & 0x800000u) == 0u;
                v0 = 0u;
                if (c) {
                    goto L_1446114;
                }
            }
            fcr31 = (a1f::le(f12, f0)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
            if ((fcr31 & 0x800000u) == 0u) {
                goto L_1446114;
            }
            f1 = std::fabs((float)(f12));
L_1446100:
            f0 = std::fabs((float)(f13));
            fcr31 = (a1f::le(f0, f1)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
            {
                const bool c = (fcr31 & 0x800000u) != 0u;
                v0 = 1u;
                if (!c) {
                    v0 = 0u;
                }
            }
        }
    }
L_1446114:
    {
        return a1::RetVF{(uint64_t)a1_sx64(v0), f0};
    }
}

// adjustAngleForLimit (0x1446868): typed by tools/typed_lift.py (round A1)
inline float adjustAngleForLimit(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, float f12, float f13)
{
    float f0 = 0.0f;
    uint32_t fcr31 = 0;
    fcr31 = (a1f::lt(f13, f12)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
    {
        const bool c = (fcr31 & 0x800000u) != 0u;
        f0 = ((float)(f13));
        if (!c) {
            f0 = (-(float)(f13));
            fcr31 = (a1f::lt(f12, f0)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
            if ((fcr31 & 0x800000u) == 0u) {
                f0 = ((float)(f12));
            } else {
            }
        }
    }
    {
        return f0;
    }
}

// setDashSlowLegCtrlWork (0x1449810): typed by tools/typed_lift.py (round A1)
inline void setDashSlowLegCtrlWork(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t a0, uint32_t a1)
{
    {
        A1_W32B(a0, 48, a1);
        
        return;
    }
}

// setStabilizeYSlowLegCtrlWork (0x1449818): typed by tools/typed_lift.py (round A1)
inline void setStabilizeYSlowLegCtrlWork(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t a0, uint32_t a1)
{
    {
        A1_W32B(a0, 60, a1);
        
        return;
    }
}

// setMoveModeSlowLegCtrlWork (0x1449820): typed by tools/typed_lift.py (round A1)
inline void setMoveModeSlowLegCtrlWork(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t a0, uint32_t a1)
{
    {
        A1_W32B(a0, 52, a1);
        
        return;
    }
}

// setMotScaleMaxSlowLegCtrlWork (0x1449828): typed by tools/typed_lift.py (round A1)
inline void setMotScaleMaxSlowLegCtrlWork(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t a0, float f12)
{
    {
        A1_W32B(a0, 56, a1_bits(f12));
        
        return;
    }
}

// setGuardModeSlowLegCtrlWork (0x1449838): typed by tools/typed_lift.py (round A1)
inline void setGuardModeSlowLegCtrlWork(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t a0, uint32_t a1)
{
    {
        A1_W32B(a0, 156, a1);
        
        return;
    }
}

// gamecore_sub_014498B8 (0x14498b8): typed by tools/typed_lift.py (round A1)
inline void gamecore_sub_014498B8(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t a0)
{
    {
        A1_W32B(a0, 148, 0u);
        
        return;
    }
}

// setAttainSpeedWormCtrlWork (0x144cc60): typed by tools/typed_lift.py (round A1)
inline a1::RetVF setAttainSpeedWormCtrlWork(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t a0, float f12)
{
    uint64_t v0 = 0;
    float f0 = 0.0f;
    uint32_t fcr31 = 0;
    f0 = a1_flt(0u);
    fcr31 = (a1f::le(f0, f12)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
    if ((fcr31 & 0x800000u) != 0u) {
        f0 = a1_flt(A1_R32B(a0, 88));
        fcr31 = (a1f::le(f12, f0)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
        if ((fcr31 & 0x800000u) != 0u) {
            A1_W32B(a0, 176, a1_bits(f12));
            {
                (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 1))));
                return a1::RetVF{(uint64_t)v0, f0};
            }
        }
    }
    {
        (v0 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL));
        return a1::RetVF{(uint64_t)v0, f0};
    }
}

// setAttackParamWormCtrlWork (0x144cc98): typed by tools/typed_lift.py (round A1)
inline void setAttackParamWormCtrlWork(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t a0, float f12)
{
    {
        A1_W32B(a0, 184, a1_bits(f12));
        
        return;
    }
}

// setRotParamWormCtrlWork (0x144cd18): typed by tools/typed_lift.py (round A1)
inline float setRotParamWormCtrlWork(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t a0, float f12, float f13)
{
    uint32_t at = 0;
    float f0 = 0.0f;
    at = 0x3C8E0000u;
    at = at | 0xFA35u;
    f0 = a1_flt(at);
    A1_W32B(a0, 108, a1_bits(f12));
    f13 = a1f::mul(f13, f0);
    {
        A1_W32B(a0, 112, a1_bits(f13));
        return f0;
    }
}

// setChkKeepHitWormCtrlWork (0x144cd38): typed by tools/typed_lift.py (round A1)
inline void setChkKeepHitWormCtrlWork(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t a0, uint32_t a1)
{
    {
        A1_W32B(a0, 208, a1);
        
        return;
    }
}

// setRotColCheckWormCtrlWork (0x144cd40): typed by tools/typed_lift.py (round A1)
inline void setRotColCheckWormCtrlWork(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t a0, uint32_t a1)
{
    {
        A1_W32B(a0, 216, a1);
        
        return;
    }
}

// setObstacleWormCtrlWork (0x144cd50): typed by tools/typed_lift.py (round A1)
inline void setObstacleWormCtrlWork(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t a0, uint32_t a1)
{
    {
        A1_W32B(a0, 224, a1);
        
        return;
    }
}

// setSelfEvasionUpWormCtrlWork (0x144cd58): typed by tools/typed_lift.py (round A1)
inline void setSelfEvasionUpWormCtrlWork(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t a0, uint32_t a1)
{
    {
        A1_W32B(a0, 228, a1);
        
        return;
    }
}

// setTypeWormCtrlWork (0x144cd60): typed by tools/typed_lift.py (round A1)
inline void setTypeWormCtrlWork(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t a0, uint32_t a1)
{
    {
        A1_W32B(a0, 236, a1);
        
        return;
    }
}

// setFallGravCoffWormCtrlWork (0x144cd68): typed by tools/typed_lift.py (round A1)
inline void setFallGravCoffWormCtrlWork(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t a0, float f12)
{
    {
        A1_W32B(a0, 136, a1_bits(f12));
        
        return;
    }
}

// setMaxFallSpeedWormCtrlWork (0x144cd70): typed by tools/typed_lift.py (round A1)
inline void setMaxFallSpeedWormCtrlWork(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t a0, float f12)
{
    {
        A1_W32B(a0, 140, a1_bits(f12));
        
        return;
    }
}

// setGroundFrictionCoffWormCtrlWork (0x144cd78): typed by tools/typed_lift.py (round A1)
inline void setGroundFrictionCoffWormCtrlWork(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t a0, float f12)
{
    {
        A1_W32B(a0, 144, a1_bits(f12));
        
        return;
    }
}

// setUnderFrictionCoffWormCtrlWork (0x144cd80): typed by tools/typed_lift.py (round A1)
inline void setUnderFrictionCoffWormCtrlWork(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t a0, float f12)
{
    {
        A1_W32B(a0, 148, a1_bits(f12));
        
        return;
    }
}

// setWorldPitchLimitWormCtrlWork (0x144cd88): typed by tools/typed_lift.py (round A1)
inline float setWorldPitchLimitWormCtrlWork(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t a0, float f12, float f13)
{
    uint32_t at = 0;
    float f0 = 0.0f;
    at = 0x3C8E0000u;
    at = at | 0xFA35u;
    f0 = a1_flt(at);
    f12 = a1f::mul(f12, f0);
    f13 = a1f::mul(f13, f0);
    f12 = std::fabs((float)(f12));
    f13 = std::fabs((float)(f13));
    f12 = (-(float)(f12));
    A1_W32B(a0, 120, a1_bits(f13));
    {
        A1_W32B(a0, 116, a1_bits(f12));
        return f0;
    }
}

// setColOffsetJudgeFunc (0x144cde8): typed by tools/typed_lift.py (round A1)
inline void setColOffsetJudgeFunc(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t a0, uint32_t a1)
{
    {
        A1_W32B(a0, 240, a1);
        
        return;
    }
}

// gamecore_sub_0144CEA8 (0x144cea8): typed by tools/typed_lift.py (round A1)
inline a1::RetVF gamecore_sub_0144CEA8(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t a0)
{
    uint32_t at = 0;
    uint32_t v0 = 0;
    float f0 = 0.0f;
    float f1 = 0.0f;
    v0 = 0x1470000u;
    at = 0x3F800000u;
    f1 = a1_flt(at);
    f0 = a1_flt(A1_R32(v0 - 0x35A8u));
    A1_W32B(a0, 76, a1_bits(f1));
    A1_W32B(a0, 72, a1_bits(f0));
    A1_W32B(a0, 48, 0u);
    A1_W32B(a0, 80, 0u);
    A1_W32B(a0, 52, 0u);
    A1_W32B(a0, 64, a1_bits(f0));
    {
        A1_W32B(a0, 68, a1_bits(f0));
        return a1::RetVF{(uint64_t)a1_sx64(v0), f0};
    }
}

// createArrowThrustWork (0x144d3a0): typed by tools/typed_lift.py (round A1)
inline void createArrowThrustWork(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t a1, uint32_t a2)
{
    A1_W32(a1, a2);
    {
        A1_W32B(a1, 4, 0u);
        
        return;
    }
}

// SCRConditionChkDisplayOmakeCompleteAlert (0x1459a50): typed by tools/typed_lift.py (round A1)
inline uint32_t SCRConditionChkDisplayOmakeCompleteAlert(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint64_t a1)
{
    uint32_t v0 = 0;
    if ((int64_t)a1 == 0) {
        v0 = 0x1460000u;
    } else {
        v0 = 0x1460000u;
        {
            v0 = A1_R32B(v0, 26756);
            return v0;
        }
    }
    {
        v0 = A1_R32B(v0, 26752);
        return v0;
    }
}

// setScrDemoMangerFinishCallback (0x145ae68): typed by tools/typed_lift.py (round A1)
inline uint32_t setScrDemoMangerFinishCallback(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t a0, uint32_t a1)
{
    uint32_t v0 = 0;
    uint32_t v1 = 0;
    v0 = 0x1460000u;
    v1 = 0x1460000u;
    A1_W32B(v0, 29680, a0);
    {
        A1_W32B(v1, 29684, a1);
        return v0;
    }
}

// initInterestWork (0x145c100): typed by tools/typed_lift.py (round A1)
inline uint32_t initInterestWork(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t a1)
{
    uint32_t v0 = 0;
    v0 = 0xFFFFFFFFu;
    A1_W32B(a1, 12, 0u);
    A1_W32B(a1, 8, v0);
    A1_W32(a1, 0u);
    {
        A1_W32B(a1, 4, 0u);
        return v0;
    }
}

// getInterestExec (0x145c118): typed by tools/typed_lift.py (round A1)
inline uint32_t getInterestExec(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t a0)
{
    uint32_t v0 = 0;
    v0 = A1_R32(a0);
    {
        v0 = 0u < v0 ? 1u : 0u;
        return v0;
    }
}

// setPerceptionParam (0x145c990): typed by tools/typed_lift.py (round A1)
inline void setPerceptionParam(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t a0, uint32_t a1)
{
    {
        A1_W32B(a0, 64, a1);
        
        return;
    }
}

// setGoingAngleGazeIKWork (0x14617b8): typed by tools/typed_lift.py (round A1)
inline uint32_t setGoingAngleGazeIKWork(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t a0, float f12)
{
    uint32_t v0 = 0;
    v0 = A1_R32B(a0, 4364);
    if (v0 != 0) {
        A1_W32B(v0, 64, a1_bits(f12));
    } else {
    }
    {
        return v0;
    }
}

// setSubParamGazeIKWork (0x14617f0): typed by tools/typed_lift.py (round A1)
inline uint32_t setSubParamGazeIKWork(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t a0, uint32_t a1)
{
    uint32_t v0 = 0;
    v0 = A1_R32B(a0, 4364);
    if (v0 != 0) {
        A1_W32B(v0, 188, a1);
    } else {
    }
    {
        return v0;
    }
}

// setGazeModeGazeIKWork (0x1461808): typed by tools/typed_lift.py (round A1)
inline uint32_t setGazeModeGazeIKWork(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t a0, uint32_t a1)
{
    uint32_t v0 = 0;
    v0 = A1_R32B(a0, 4364);
    if (v0 != 0) {
        A1_W32B(v0, 96, a1);
    } else {
    }
    {
        return v0;
    }
}

// setExecRootQuatGazeIKWork (0x1461820): typed by tools/typed_lift.py (round A1)
inline uint32_t setExecRootQuatGazeIKWork(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t a0, uint32_t a1)
{
    uint32_t v0 = 0;
    v0 = A1_R32B(a0, 4364);
    if (v0 != 0) {
        A1_W32B(v0, 176, a1);
    } else {
    }
    {
        return v0;
    }
}

// setExecGazeIKWork (0x1461838): typed by tools/typed_lift.py (round A1)
inline uint32_t setExecGazeIKWork(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t a0, uint32_t a1)
{
    uint32_t v0 = 0;
    v0 = A1_R32B(a0, 4364);
    if (v0 != 0) {
        A1_W32B(v0, 48, a1);
    } else {
    }
    {
        return v0;
    }
}

// initTailIKWork (0x14621b8): typed by tools/typed_lift.py (round A1)
inline void initTailIKWork(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t a2)
{
    A1_W32B(a2, 964, 0u);
    A1_W32B(a2, 960, 0u);
    {
        A1_W32B(a2, 968, 0u);
        
        return;
    }
}

// setWormIKBoundDynamics (0x14647e0): typed by tools/typed_lift.py (round A1)
inline void setWormIKBoundDynamics(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t a0, float f12, float f13, float f14)
{
    A1_W32B(a0, 4744, a1_bits(f14));
    A1_W32B(a0, 4736, a1_bits(f12));
    {
        A1_W32B(a0, 4740, a1_bits(f13));
        
        return;
    }
}

// setWormIKBankInterpSec (0x1464820): typed by tools/typed_lift.py (round A1)
inline void setWormIKBankInterpSec(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t a0, float f12)
{
    {
        A1_W32B(a0, 4748, a1_bits(f12));
        
        return;
    }
}

// getSScriptWork (0x1465308): typed by tools/typed_lift.py (round A1)
inline uint32_t getSScriptWork(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt)
{
    uint32_t v0 = 0;
    v0 = 0x1200000u;
    {
        v0 = v0 + 0x1440u;
        return v0;
    }
}
