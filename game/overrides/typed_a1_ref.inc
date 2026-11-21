// typed_a1_ref.inc: round A1's references for the check (tools/typed_lift.py --check-tu). Generated.
// SetHeapStartPoint (0x101a30): lift_d1's text (the reference)
void SetHeapStartPoint(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t v0 = Ps2GprU32(ctx, 2), a0 = Ps2GprU32(ctx, 4), gp = Ps2GprU32(ctx, 28), ra = Ps2GprU32(ctx, 31);
    a0 = a0 + 0xFu;
    v0 = 0xFFFFFFF0u;
    a0 = a0 & v0;
    {
        const uint32_t target = ra;
        WRITE32(gp - 0x7F6Cu, a0);
        Ps2SetGpr64(ctx, 2, sx64(v0)); Ps2SetGpr64(ctx, 4, sx64(a0));
        ctx->pc = target;
        return;
    }
}

// GetHeapCurrentPoint (0x101a48): lift_d1's text (the reference)
void GetHeapCurrentPoint(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t v0 = Ps2GprU32(ctx, 2), gp = Ps2GprU32(ctx, 28), ra = Ps2GprU32(ctx, 31);
    {
        const uint32_t target = ra;
        v0 = READ32(gp - 0x7F6Cu);
        Ps2SetGpr64(ctx, 2, sx64(v0));
        ctx->pc = target;
        return;
    }
}

// LoaderSysCheckCDBootMode (0x102fe0): lift_d1's text (the reference)
void LoaderSysCheckCDBootMode(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t v0 = Ps2GprU32(ctx, 2), ra = Ps2GprU32(ctx, 31);
    {
        const uint32_t target = ra;
        v0 = 2u;
        Ps2SetGpr64(ctx, 2, sx64(v0));
        ctx->pc = target;
        return;
    }
}

// LoaderSysSendAbort (0x1032f0): lift_d1's text (the reference)
void LoaderSysSendAbort(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t ra = Ps2GprU32(ctx, 31);
    {
        const uint32_t target = ra;
        
        ctx->pc = target;
        return;
    }
}

// LoaderSysFlushPrint (0x1032f8): lift_d1's text (the reference)
void LoaderSysFlushPrint(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t ra = Ps2GprU32(ctx, 31);
    {
        const uint32_t target = ra;
        
        ctx->pc = target;
        return;
    }
}

// LoaderSysGetBootArg (0x103ca8): lift_d1's text (the reference)
void LoaderSysGetBootArg(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t v0 = Ps2GprU32(ctx, 2), ra = Ps2GprU32(ctx, 31);
    v0 = 0x140000u;
    {
        const uint32_t target = ra;
        v0 = v0 - 0x3070u;
        Ps2SetGpr64(ctx, 2, sx64(v0));
        ctx->pc = target;
        return;
    }
}

// LoaderSysHookPoint (0x103e90): lift_d1's text (the reference)
void LoaderSysHookPoint(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t ra = Ps2GprU32(ctx, 31);
    {
        const uint32_t target = ra;
        
        ctx->pc = target;
        return;
    }
}

// initmemprintf (0x104088): lift_d1's text (the reference)
void initmemprintf(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t ra = Ps2GprU32(ctx, 31);
    {
        const uint32_t target = ra;
        
        ctx->pc = target;
        return;
    }
}

// padSysGet (0x104e78): lift_d1's text (the reference)
void padSysGet(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t at = Ps2GprU32(ctx, 1), v0 = Ps2GprU32(ctx, 2), a0 = Ps2GprU32(ctx, 4), ra = Ps2GprU32(ctx, 31);
    v0 = a0 << 2;
    v0 = v0 + a0;
    v0 = v0 << 6;
    at = 0x140000u;
    at = at - 0x27C0u;
    v0 = at + v0;
    {
        const uint32_t target = ra;
        Ps2SetGpr64(ctx, 1, sx64(at)); Ps2SetGpr64(ctx, 2, sx64(v0));
        ctx->pc = target;
        return;
    }
}

// LoaderSysSetPowerOffCallBackFunc (0x104f58): lift_d1's text (the reference)
void LoaderSysSetPowerOffCallBackFunc(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t a0 = Ps2GprU32(ctx, 4), gp = Ps2GprU32(ctx, 28), ra = Ps2GprU32(ctx, 31);
    {
        const uint32_t target = ra;
        WRITE32(gp - 0x7C54u, a0);
        
        ctx->pc = target;
        return;
    }
}

// SetLocate (0x106380): lift_d1's text (the reference)
void SetLocate(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t a0 = Ps2GprU32(ctx, 4), a1 = Ps2GprU32(ctx, 5), gp = Ps2GprU32(ctx, 28), ra = Ps2GprU32(ctx, 31);
    WRITE32(gp - 0x7C04u, a0);
    {
        const uint32_t target = ra;
        WRITE32(gp - 0x7C00u, a1);
        
        ctx->pc = target;
        return;
    }
}

// PutChar (0x106b58): lift_d1's text (the reference)
void PutChar(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t at = Ps2GprU32(ctx, 1), v0 = Ps2GprU32(ctx, 2), v1 = Ps2GprU32(ctx, 3), a0 = Ps2GprU32(ctx, 4), a1 = Ps2GprU32(ctx, 5), a2 = Ps2GprU32(ctx, 6), gp = Ps2GprU32(ctx, 28), ra = Ps2GprU32(ctx, 31);
    v0 = READ32(gp - 0x7C00u);
    a2 = READ32(gp - 0x7C04u);
    v1 = v0 << 4;
    v1 = v1 - v0;
    v1 = v1 << 2;
    v1 = v1 + a2;
    v1 = v1 << 3;
    at = 0x140000u;
    at = at - 0x1270u;
    v1 = at + v1;
    WRITE8_B(v1, 4, (uint8_t)a1);
    {
        const uint32_t target = ra;
        WRITE32(v1, a0);
        Ps2SetGpr64(ctx, 1, sx64(at)); Ps2SetGpr64(ctx, 2, sx64(v0)); Ps2SetGpr64(ctx, 3, sx64(v1)); Ps2SetGpr64(ctx, 6, sx64(a2));
        ctx->pc = target;
        return;
    }
}

// sub_00107D90 (0x107d90): lift_d1's text (the reference)
void sub_00107D90(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint64_t v0 = Ps2GprU64(ctx, 2), v1 = Ps2GprU64(ctx, 3), a0 = Ps2GprU64(ctx, 4), a1 = Ps2GprU64(ctx, 5), a2 = Ps2GprU64(ctx, 6), ra = Ps2GprU64(ctx, 31);
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
                const uint32_t target = (uint32_t)ra;
                (v0 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL));
                Ps2SetGpr64(ctx, 2, v0); Ps2SetGpr64(ctx, 5, a1); Ps2SetGpr64(ctx, 6, a2);
                ctx->pc = target;
                return;
            }
        }
    }
    {
        const bool c = (int64_t)v0 != 0;
        (v0 = (uint64_t)(a1 << 12));
        if (!c) {
            {
                const uint32_t target = (uint32_t)ra;
                (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 9999))));
                Ps2SetGpr64(ctx, 2, v0); Ps2SetGpr64(ctx, 5, a1); Ps2SetGpr64(ctx, 6, a2);
                ctx->pc = target;
                return;
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
        const uint32_t target = (uint32_t)ra;
        (v0 = (uint64_t)((int64_t)v0 >> (32 + 0)));
        Ps2SetGpr64(ctx, 2, v0); Ps2SetGpr64(ctx, 3, v1); Ps2SetGpr64(ctx, 4, a0); Ps2SetGpr64(ctx, 5, a1); Ps2SetGpr64(ctx, 6, a2);
        ctx->pc = target;
        return;
    }
}

// sub_00109268 (0x109268): lift_d1's text (the reference)
void sub_00109268(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t v0 = Ps2GprU32(ctx, 2), v1 = Ps2GprU32(ctx, 3), a0 = Ps2GprU32(ctx, 4), ra = Ps2GprU32(ctx, 31);
    v1 = READ32_B(a0, 16);
    v0 = 0xFFFF0000u;
    v0 = v0 | 0xFFFEu;
    WRITE32_B(a0, 24, 0u);
    v1 = v1 & v0;
    {
        const uint32_t target = ra;
        WRITE32_B(a0, 16, v1);
        Ps2SetGpr64(ctx, 2, sx64(v0)); Ps2SetGpr64(ctx, 3, sx64(v1));
        ctx->pc = target;
        return;
    }
}

// ps2___sceKernlGetEhSemaId (0x10fbe8): lift_d1's text (the reference)
void ps2___sceKernlGetEhSemaId(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t v0 = Ps2GprU32(ctx, 2), v1 = Ps2GprU32(ctx, 3), ra = Ps2GprU32(ctx, 31);
    v1 = 0x130000u;
    {
        const uint32_t target = ra;
        v0 = READ32_B(v1, 10660);
        Ps2SetGpr64(ctx, 2, sx64(v0)); Ps2SetGpr64(ctx, 3, sx64(v1));
        ctx->pc = target;
        return;
    }
}

// GetTimerPreScaleFactor (0x110640): lift_d1's text (the reference)
void GetTimerPreScaleFactor(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t v0 = Ps2GprU32(ctx, 2), v1 = Ps2GprU32(ctx, 3), ra = Ps2GprU32(ctx, 31);
    v0 = 0x130000u;
    v1 = READ32_B(v0, 14624);
    {
        const bool c = (int32_t)v1 >= 0;
        v1 = 0x10000000u;
        if (!c) {
            v0 = 0x80000000u;
            {
                const uint32_t target = ra;
                v0 = v0 | 0x8001u;
                Ps2SetGpr64(ctx, 2, sx64(v0)); Ps2SetGpr64(ctx, 3, sx64(v1));
                ctx->pc = target;
                return;
            }
        }
    }
    v1 = v1 | 0x1010u;
    v0 = READ32(v1);
    {
        const uint32_t target = ra;
        v0 = v0 & 3u;
        Ps2SetGpr64(ctx, 2, sx64(v0)); Ps2SetGpr64(ctx, 3, sx64(v1));
        ctx->pc = target;
        return;
    }
}

// sub_00110998 (0x110998): lift_d1's text (the reference)
void sub_00110998(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t v0 = Ps2GprU32(ctx, 2), v1 = Ps2GprU32(ctx, 3), a0 = Ps2GprU32(ctx, 4), ra = Ps2GprU32(ctx, 31);
    v0 = READ32_B(a0, 4);
    {
        const bool c = v0 == 0;
        v1 = READ32(a0);
        if (!c) {
            WRITE32(v0, v1);
            goto L_1109b4;
        }
    }
    v0 = 0x130000u;
    WRITE32_B(v0, 14640, v1);
L_1109b4:
    if (v1 == 0) {
        WRITE32_B(a0, 4, 0u);
    } else {
        v0 = READ32_B(a0, 4);
        WRITE32_B(v1, 4, v0);
        WRITE32_B(a0, 4, 0u);
    }
    {
        const uint32_t target = ra;
        v0 = v1;
        Ps2SetGpr64(ctx, 2, sx64(v0)); Ps2SetGpr64(ctx, 3, sx64(v1));
        ctx->pc = target;
        return;
    }
}

// iGetTimerUsedUnusedCounters (0x110ef0): lift_d1's text (the reference)
void iGetTimerUsedUnusedCounters(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t v0 = Ps2GprU32(ctx, 2), v1 = Ps2GprU32(ctx, 3), ra = Ps2GprU32(ctx, 31);
    uint64_t a0 = Ps2GprU64(ctx, 4), a1 = Ps2GprU64(ctx, 5);
    v0 = 0x130000u;
    v1 = v0 + 0x3918u;
    v0 = READ32_B(v1, 8);
    if ((int32_t)v0 < 0) {
        v0 = 0x80000000u;
        {
            const uint32_t target = ra;
            v0 = v0 | 0x8001u;
            Ps2SetGpr64(ctx, 2, sx64(v0)); Ps2SetGpr64(ctx, 3, sx64(v1));
            ctx->pc = target;
            return;
        }
    }
    if ((int64_t)a0 != 0) {
        v0 = READ32_B(v1, 16);
        WRITE32_B((uint32_t)a0, 0, v0);
    }
    if ((int64_t)a1 != 0) {
        v1 = READ32_B(v1, 16);
        v0 = 0x80u;
        v0 = v0 - v1;
        WRITE32_B((uint32_t)a1, 0, v0);
    }
    {
        const uint32_t target = ra;
        v0 = 0u;
        Ps2SetGpr64(ctx, 2, sx64(v0)); Ps2SetGpr64(ctx, 3, sx64(v1));
        ctx->pc = target;
        return;
    }
}

// sceSifRemoveRebootNotifyHandler (0x111c50): lift_d1's text (the reference)
void sceSifRemoveRebootNotifyHandler(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t v0 = Ps2GprU32(ctx, 2), v1 = Ps2GprU32(ctx, 3), a1 = Ps2GprU32(ctx, 5), ra = Ps2GprU32(ctx, 31);
    uint64_t a0 = Ps2GprU64(ctx, 4);
    if ((int32_t)a0 >= 0) {
        v0 = 0x130000u;
    } else {
        v0 = 0x7FFF0000u;
        v1 = 0x150000u;
        v0 = v0 | 0xFFFFu;
        a1 = v1 - 0x5378u;
        (a0 = (uint64_t)(a0 & sx64(v0)));
        v0 = 0x20u;
        goto L_111c80;
    }
    v1 = 0x130000u;
    a1 = READ32_B(v0, 14648);
    v0 = READ32_B(v1, 14652);
L_111c80:
    (v0 = (uint32_t)(((uint64_t)a0 < (uint64_t)sx64(v0)) ? 1 : 0));
    {
        const bool c = v0 != 0;
        (v1 = (uint32_t)((int32_t)((int32_t)SLL32((uint32_t)a0, 4))));
        if (!c) {
            {
                const uint32_t target = ra;
                v0 = 0xFFFFFF97u;
                Ps2SetGpr64(ctx, 2, sx64(v0)); Ps2SetGpr64(ctx, 3, sx64(v1)); Ps2SetGpr64(ctx, 4, a0); Ps2SetGpr64(ctx, 5, sx64(a1));
                ctx->pc = target;
                return;
            }
        }
    }
    v0 = 0u;
    v1 = v1 + a1;
    {
        const uint32_t target = ra;
        WRITE32(v1, 0u);
        Ps2SetGpr64(ctx, 2, sx64(v0)); Ps2SetGpr64(ctx, 3, sx64(v1)); Ps2SetGpr64(ctx, 4, a0); Ps2SetGpr64(ctx, 5, sx64(a1));
        ctx->pc = target;
        return;
    }
}

// ioctl (0x111fc0): lift_d1's text (the reference)
void ioctl(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t v0 = Ps2GprU32(ctx, 2), ra = Ps2GprU32(ctx, 31);
    {
        const uint32_t target = ra;
        v0 = 0xFFFFFFFFu;
        Ps2SetGpr64(ctx, 2, sx64(v0));
        ctx->pc = target;
        return;
    }
}

// isatty (0x112080): lift_d1's text (the reference)
void isatty(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t v0 = Ps2GprU32(ctx, 2), ra = Ps2GprU32(ctx, 31);
    {
        const uint32_t target = ra;
        v0 = 1u;
        Ps2SetGpr64(ctx, 2, sx64(v0));
        ctx->pc = target;
        return;
    }
}

// sub_00112358 (0x112358): lift_d1's text (the reference)
void sub_00112358(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t v0 = Ps2GprU32(ctx, 2), v1 = Ps2GprU32(ctx, 3), a0 = Ps2GprU32(ctx, 4), ra = Ps2GprU32(ctx, 31);
    v0 = 0x150000u;
    v1 = v0 - 0x5140u;
    WRITE32(v0 - 0x5140u, a0);
    a0 = v1 + 0x10u;
    v0 = v1;
    WRITE32_B(v1, 8, a0);
    WRITE32_B(v1, 4, 0u);
    {
        const uint32_t target = ra;
        WRITE32_B(v1, 12, a0);
        Ps2SetGpr64(ctx, 2, sx64(v0)); Ps2SetGpr64(ctx, 3, sx64(v1)); Ps2SetGpr64(ctx, 4, sx64(a0));
        ctx->pc = target;
        return;
    }
}

// sub_00112380 (0x112380): lift_d1's text (the reference)
void sub_00112380(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t v0 = Ps2GprU32(ctx, 2), v1 = Ps2GprU32(ctx, 3), ra = Ps2GprU32(ctx, 31);
    uint64_t a0 = Ps2GprU64(ctx, 4), a1 = Ps2GprU64(ctx, 5);
    (a1 = (uint64_t)((uint64_t)a0 + (uint64_t)0ULL));
    (v0 = (uint32_t)((int32_t)((int32_t)READ32_B((uint32_t)a1, 4))));
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)READ32_B((uint32_t)a1, 12))));
    (v1 = (uint32_t)((int32_t)((int32_t)READ32_B((uint32_t)a1, 0))));
    v0 = v0 + 1u;
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)a0, 1))));
    WRITE32_B((uint32_t)a1, 4, v0);
    v1 = v1 + 0x10u;
    (v1 = (uint32_t)((int32_t)((int32_t)ADD32((uint32_t)a1, v1))));
    {
        const bool c = (int64_t)a0 != (int64_t)(int32_t)v1;
        WRITE32_B((uint32_t)a1, 12, (uint32_t)a0);
        if (!c) {
            (v0 = (uint32_t)((int32_t)((int32_t)ADD32((uint32_t)a1, 16))));
            WRITE32_B((uint32_t)a1, 12, v0);
        }
    }
    {
        const uint32_t target = ra;
        Ps2SetGpr64(ctx, 2, sx64(v0)); Ps2SetGpr64(ctx, 3, sx64(v1)); Ps2SetGpr64(ctx, 4, a0); Ps2SetGpr64(ctx, 5, a1);
        ctx->pc = target;
        return;
    }
}

// sub_001123C0 (0x1123c0): lift_d1's text (the reference)
void sub_001123C0(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t v0 = Ps2GprU32(ctx, 2), v1 = Ps2GprU32(ctx, 3), ra = Ps2GprU32(ctx, 31);
    uint64_t a0 = Ps2GprU64(ctx, 4), a1 = Ps2GprU64(ctx, 5);
    (a1 = (uint64_t)((uint64_t)a0 + (uint64_t)0ULL));
    (v0 = (uint32_t)((int32_t)((int32_t)READ32_B((uint32_t)a1, 4))));
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)READ32_B((uint32_t)a1, 8))));
    (v1 = (uint32_t)((int32_t)((int32_t)READ32_B((uint32_t)a1, 0))));
    v0 = v0 - 1u;
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)a0, 1))));
    WRITE32_B((uint32_t)a1, 4, v0);
    v1 = v1 + 0x10u;
    (v1 = (uint32_t)((int32_t)((int32_t)ADD32((uint32_t)a1, v1))));
    {
        const bool c = (int64_t)a0 != (int64_t)(int32_t)v1;
        WRITE32_B((uint32_t)a1, 8, (uint32_t)a0);
        if (!c) {
            (v0 = (uint32_t)((int32_t)((int32_t)ADD32((uint32_t)a1, 16))));
            WRITE32_B((uint32_t)a1, 8, v0);
        }
    }
    {
        const uint32_t target = ra;
        Ps2SetGpr64(ctx, 2, sx64(v0)); Ps2SetGpr64(ctx, 3, sx64(v1)); Ps2SetGpr64(ctx, 4, a0); Ps2SetGpr64(ctx, 5, a1);
        ctx->pc = target;
        return;
    }
}

// sub_001128A0 (0x1128a0): lift_d1's text (the reference)
void sub_001128A0(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t v0 = Ps2GprU32(ctx, 2), v1 = Ps2GprU32(ctx, 3), a0 = Ps2GprU32(ctx, 4), a1 = Ps2GprU32(ctx, 5), a2 = Ps2GprU32(ctx, 6), ra = Ps2GprU32(ctx, 31);
    v0 = READ32_B(a0, 16);
    a2 = READ32_B(a1, 28);
    v1 = READ32_B(a0, 20);
    v0 = v0 << 2;
    v0 = v0 + a2;
    {
        const uint32_t target = ra;
        WRITE32(v0, v1);
        Ps2SetGpr64(ctx, 2, sx64(v0)); Ps2SetGpr64(ctx, 3, sx64(v1)); Ps2SetGpr64(ctx, 6, sx64(a2));
        ctx->pc = target;
        return;
    }
}

// sub_001128C0 (0x1128c0): lift_d1's text (the reference)
void sub_001128C0(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t v0 = Ps2GprU32(ctx, 2), a0 = Ps2GprU32(ctx, 4), a1 = Ps2GprU32(ctx, 5), ra = Ps2GprU32(ctx, 31);
    v0 = READ32_B(a0, 16);
    {
        const uint32_t target = ra;
        WRITE32_B(a1, 8, v0);
        Ps2SetGpr64(ctx, 2, sx64(v0));
        ctx->pc = target;
        return;
    }
}

// sub_001148B8 (0x1148b8): lift_d1's text (the reference)
void sub_001148B8(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t v1 = Ps2GprU32(ctx, 3), ra = Ps2GprU32(ctx, 31);
    uint64_t v0 = Ps2GprU64(ctx, 2), a0 = Ps2GprU64(ctx, 4);
    v1 = 7u;
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)SRL32((uint32_t)a0, 28))));
    {
        const bool c = (int64_t)v0 != (int64_t)(int32_t)v1;
        (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)4095 << 16))));
        if (!c) {
            v1 = 0x80000000u;
            (v0 = (uint64_t)(RC_U64X(v0) | (uint64_t)(uint16_t)65535));
            (a0 = (uint64_t)(a0 & RC_U64X(v0)));
            (a0 = (uint64_t)(a0 | sx64(v1)));
        }
    }
    {
        const uint32_t target = ra;
        (v0 = (uint64_t)((uint64_t)a0 + (uint64_t)0ULL));
        Ps2SetGpr64(ctx, 2, v0); Ps2SetGpr64(ctx, 3, sx64(v1)); Ps2SetGpr64(ctx, 4, a0);
        ctx->pc = target;
        return;
    }
}

// _sceCdSC (0x117a00): lift_d1's text (the reference)
void _sceCdSC(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t a1 = Ps2GprU32(ctx, 5), ra = Ps2GprU32(ctx, 31);
    uint64_t v0 = Ps2GprU64(ctx, 2), a0 = Ps2GprU64(ctx, 4);
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
                                    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)READ32_B((uint32_t)v0, 14956))));
                                    WRITE32_B(a1, 0, (uint32_t)v0);
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
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)READ32_B((uint32_t)v0, 14952))));
L_117aa4:
    WRITE32_B(a1, 0, (uint32_t)v0);
L_117aa8:
    {
        const uint32_t target = ra;
        (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 1))));
        Ps2SetGpr64(ctx, 2, v0);
        ctx->pc = target;
        return;
    }
}

// sub_00119AA0 (0x119aa0): lift_d1's text (the reference)
void sub_00119AA0(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t v0 = Ps2GprU32(ctx, 2), v1 = Ps2GprU32(ctx, 3), a0 = Ps2GprU32(ctx, 4), ra = Ps2GprU32(ctx, 31);
    v1 = READ32_B(a0, 16);
    v0 = 0xFFFF0000u;
    v0 = v0 | 0xFFFEu;
    WRITE32_B(a0, 24, 0u);
    v1 = v1 & v0;
    {
        const uint32_t target = ra;
        WRITE32_B(a0, 16, v1);
        Ps2SetGpr64(ctx, 2, sx64(v0)); Ps2SetGpr64(ctx, 3, sx64(v1));
        ctx->pc = target;
        return;
    }
}

// ps2__SceMcComGetSysInfo (0x120f18): lift_d1's text (the reference)
void ps2__SceMcComGetSysInfo(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t v0 = Ps2GprU32(ctx, 2), v1 = Ps2GprU32(ctx, 3), a0 = Ps2GprU32(ctx, 4), ra = Ps2GprU32(ctx, 31);
    uint64_t hi = ctx->hi;
    uint64_t lo = ctx->lo;
    v1 = 0x184u;
    v0 = 0x130000u;
    { int64_t result = (int64_t)(int32_t)a0 * (int64_t)(int32_t)v1; lo = (uint64_t)(int64_t)(int32_t)result; hi = (uint64_t)(int64_t)(int32_t)(result >> 32); (a0 = (uint32_t)((int32_t)((int32_t)result))); }
    v0 = v0 + 0x5638u;
    {
        const uint32_t target = ra;
        v0 = a0 + v0;
        Ps2SetGpr64(ctx, 2, sx64(v0)); Ps2SetGpr64(ctx, 3, sx64(v1)); Ps2SetGpr64(ctx, 4, sx64(a0)); ctx->hi = hi; ctx->lo = lo;
        ctx->pc = target;
        return;
    }
}

// sceNetcnfifSetFNoDecode (0x122448): lift_d1's text (the reference)
void sceNetcnfifSetFNoDecode(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t v1 = Ps2GprU32(ctx, 3), a0 = Ps2GprU32(ctx, 4), ra = Ps2GprU32(ctx, 31);
    uint64_t v0 = Ps2GprU64(ctx, 2);
    v1 = 0x130000u;
    (v0 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL));
    {
        const uint32_t target = ra;
        WRITE32_B(v1, 22872, a0);
        Ps2SetGpr64(ctx, 2, v0); Ps2SetGpr64(ctx, 3, sx64(v1));
        ctx->pc = target;
        return;
    }
}

// ps2___unpack_f (0x127048): lift_d1's text (the reference)
void ps2___unpack_f(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t v0 = Ps2GprU32(ctx, 2), v1 = Ps2GprU32(ctx, 3), a0 = Ps2GprU32(ctx, 4), ra = Ps2GprU32(ctx, 31);
    uint64_t a1 = Ps2GprU64(ctx, 5), a2 = Ps2GprU64(ctx, 6);
    v1 = READ32(a0);
    a0 = 0x7F0000u;
    a0 = a0 | 0xFFFFu;
    (a2 = (uint64_t)((uint64_t)a1 + (uint64_t)0ULL));
    v0 = v1 >> 23;
    (a1 = (uint64_t)((int64_t)(int32_t)((int32_t)SRL32(v1, 31))));
    v0 = v0 & 0xFFu;
    v1 = v1 & a0;
    WRITE32_B((uint32_t)a2, 4, (uint32_t)a1);
    (a1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(v0, 4294967169))));
    {
        const bool c = v0 != 0;
        a0 = v1 << 7;
        if (!c) {
            v0 = 2u;
            {
                const uint32_t target = ra;
                WRITE32_B((uint32_t)a2, 0, v0);
                Ps2SetGpr64(ctx, 2, sx64(v0)); Ps2SetGpr64(ctx, 3, sx64(v1)); Ps2SetGpr64(ctx, 4, sx64(a0)); Ps2SetGpr64(ctx, 5, a1); Ps2SetGpr64(ctx, 6, a2);
                ctx->pc = target;
                return;
            }
        }
    }
    v0 = 0x40000000u;
    v1 = 3u;
    v0 = a0 | v0;
    WRITE32_B((uint32_t)a2, 8, (uint32_t)a1);
    WRITE32_B((uint32_t)a2, 0, v1);
    {
        const uint32_t target = ra;
        WRITE32_B((uint32_t)a2, 12, v0);
        Ps2SetGpr64(ctx, 2, sx64(v0)); Ps2SetGpr64(ctx, 3, sx64(v1)); Ps2SetGpr64(ctx, 4, sx64(a0)); Ps2SetGpr64(ctx, 5, a1); Ps2SetGpr64(ctx, 6, a2);
        ctx->pc = target;
        return;
    }
}

// ps2___pack_d (0x1270d8): lift_d1's text (the reference)
void ps2___pack_d(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint64_t v0 = Ps2GprU64(ctx, 2), v1 = Ps2GprU64(ctx, 3), a0 = Ps2GprU64(ctx, 4), a1 = Ps2GprU64(ctx, 5), a2 = Ps2GprU64(ctx, 6), a3 = Ps2GprU64(ctx, 7), t0 = Ps2GprU64(ctx, 8), ra = Ps2GprU64(ctx, 31);
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)READ32_B((uint32_t)a0, 0))));
    (a3 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL));
    (a1 = (uint64_t)(READ64_B((uint32_t)a0, 16)));
    (v0 = (uint64_t)(((uint64_t)RC_U64X(v1) < (uint64_t)(int64_t)(int32_t)2) ? 1 : 0));
    {
        const bool c = (int64_t)v0 == 0;
        (t0 = (uint64_t)((int64_t)(int32_t)((int32_t)READ32_B((uint32_t)a0, 4))));
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
        (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)READ32_B((uint32_t)a0, 8))));
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
        const uint32_t target = (uint32_t)ra;
        (v0 = (uint64_t)(a2 | v0));
        Ps2SetGpr64(ctx, 2, v0); Ps2SetGpr64(ctx, 3, v1); Ps2SetGpr64(ctx, 4, a0); Ps2SetGpr64(ctx, 5, a1); Ps2SetGpr64(ctx, 6, a2); Ps2SetGpr64(ctx, 7, a3); Ps2SetGpr64(ctx, 8, t0);
        ctx->pc = target;
        return;
    }
}

// ps2___pack_f (0x127390): lift_d1's text (the reference)
void ps2___pack_f(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t v0 = Ps2GprU32(ctx, 2), v1 = Ps2GprU32(ctx, 3), a1 = Ps2GprU32(ctx, 5), t0 = Ps2GprU32(ctx, 8), ra = Ps2GprU32(ctx, 31);
    uint64_t a0 = Ps2GprU64(ctx, 4), a2 = Ps2GprU64(ctx, 6), a3 = Ps2GprU64(ctx, 7);
    float f0 = ctx->f[0];
    (v0 = (uint32_t)((int32_t)((int32_t)READ32_B((uint32_t)a0, 0))));
    (a3 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL));
    (a1 = (uint32_t)((int32_t)((int32_t)READ32_B((uint32_t)a0, 12))));
    v0 = v0 ^ 2u;
    {
        const bool c = v0 == 0;
        (t0 = (uint32_t)((int32_t)((int32_t)READ32_B((uint32_t)a0, 4))));
        if (!c) {
            {
                const bool c = a1 == 0;
                v0 = 0x7F0000u;
                if (c) {
                    goto L_1273f8;
                }
            }
            (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)READ32_B((uint32_t)a0, 8))));
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
    (a2 = (uint64_t)(a2 & sx64(v1)));
    v0 = a1 & v0;
    v1 = 0x807F0000u;
    (a2 = (uint64_t)(a2 | sx64(v0)));
    v0 = 0x7FFF0000u;
    v1 = v1 | 0xFFFFu;
    (a0 = (uint64_t)(a3 & (uint64_t)(uint16_t)255));
    (a2 = (uint64_t)(a2 & sx64(v1)));
    v1 = t0 << 31;
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)SLL32((uint32_t)a0, 23))));
    v0 = v0 | 0xFFFFu;
    (a2 = (uint64_t)(a2 | RC_U64X(a0)));
    (a2 = (uint64_t)(a2 & sx64(v0)));
    (a2 = (uint64_t)(a2 | sx64(v1)));
    { uint32_t bits = (uint32_t)a2; std::memcpy(&f0, &bits, sizeof(bits)); }
    {
        const uint32_t target = ra;
        ctx->f[0] = f0; Ps2SetGpr64(ctx, 2, sx64(v0)); Ps2SetGpr64(ctx, 3, sx64(v1)); Ps2SetGpr64(ctx, 4, a0); Ps2SetGpr64(ctx, 5, sx64(a1)); Ps2SetGpr64(ctx, 6, a2); Ps2SetGpr64(ctx, 7, a3); Ps2SetGpr64(ctx, 8, sx64(t0));
        ctx->pc = target;
        return;
    }
}

// ps2___errno (0x127528): lift_d1's text (the reference)
void ps2___errno(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t v0 = Ps2GprU32(ctx, 2), v1 = Ps2GprU32(ctx, 3), ra = Ps2GprU32(ctx, 31);
    v1 = 0x130000u;
    {
        const uint32_t target = ra;
        v0 = READ32_B(v1, 23648);
        Ps2SetGpr64(ctx, 2, sx64(v0)); Ps2SetGpr64(ctx, 3, sx64(v1));
        ctx->pc = target;
        return;
    }
}

// sub_00128B00 (0x128b00): lift_d1's text (the reference)
void sub_00128B00(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t ra = Ps2GprU32(ctx, 31);
    uint64_t v0 = Ps2GprU64(ctx, 2);
    {
        const uint32_t target = ra;
        (v0 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL));
        Ps2SetGpr64(ctx, 2, v0);
        ctx->pc = target;
        return;
    }
}

// sub_0012D7F0 (0x12d7f0): lift_d1's text (the reference)
void sub_0012D7F0(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t v0 = Ps2GprU32(ctx, 2), v1 = Ps2GprU32(ctx, 3), a0 = Ps2GprU32(ctx, 4), a1 = Ps2GprU32(ctx, 5), a2 = Ps2GprU32(ctx, 6), a3 = Ps2GprU32(ctx, 7), t0 = Ps2GprU32(ctx, 8), t1 = Ps2GprU32(ctx, 9), ra = Ps2GprU32(ctx, 31);
    v0 = 0x130000u;
    v1 = 0x130000u;
    t0 = 0x130000u;
    t1 = 0x130000u;
    v0 = v0 - 0x5C0u;
    v1 = v1 - 0x558u;
    t0 = t0 - 0x4D8u;
    t1 = t1 - 0x470u;
    WRITE32_B(a0, 84, a3);
    WRITE16_B(a0, 12, (uint16_t)a1);
    WRITE16_B(a0, 14, (uint16_t)a2);
    WRITE32_B(a0, 32, v0);
    WRITE32_B(a0, 36, v1);
    WRITE32_B(a0, 40, t0);
    WRITE32_B(a0, 44, t1);
    WRITE32(a0, 0u);
    WRITE32_B(a0, 4, 0u);
    WRITE32_B(a0, 8, 0u);
    WRITE32_B(a0, 16, 0u);
    WRITE32_B(a0, 20, 0u);
    WRITE32_B(a0, 24, 0u);
    {
        const uint32_t target = ra;
        WRITE32_B(a0, 28, a0);
        Ps2SetGpr64(ctx, 2, sx64(v0)); Ps2SetGpr64(ctx, 3, sx64(v1)); Ps2SetGpr64(ctx, 8, sx64(t0)); Ps2SetGpr64(ctx, 9, sx64(t1));
        ctx->pc = target;
        return;
    }
}

// _localeconv_r (0x12e520): lift_d1's text (the reference)
void _localeconv_r(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t v0 = Ps2GprU32(ctx, 2), ra = Ps2GprU32(ctx, 31);
    v0 = 0x140000u;
    {
        const uint32_t target = ra;
        v0 = v0 - 0x64F8u;
        Ps2SetGpr64(ctx, 2, sx64(v0));
        ctx->pc = target;
        return;
    }
}

// _hi0bits (0x12eaf8): lift_d1's text (the reference)
void _hi0bits(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t ra = Ps2GprU32(ctx, 31);
    uint64_t v0 = Ps2GprU64(ctx, 2), v1 = Ps2GprU64(ctx, 3), a0 = Ps2GprU64(ctx, 4), a1 = Ps2GprU64(ctx, 5);
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
        const uint32_t target = ra;
        Ps2SetGpr64(ctx, 2, v0); Ps2SetGpr64(ctx, 3, v1); Ps2SetGpr64(ctx, 4, a0); Ps2SetGpr64(ctx, 5, a1);
        ctx->pc = target;
        return;
    }
}

// _lo0bits (0x12eb78): lift_d1's text (the reference)
void _lo0bits(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t v0 = Ps2GprU32(ctx, 2), v1 = Ps2GprU32(ctx, 3), a0 = Ps2GprU32(ctx, 4), a1 = Ps2GprU32(ctx, 5), ra = Ps2GprU32(ctx, 31);
    a1 = READ32(a0);
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
                        const uint32_t target = ra;
                        WRITE32(a0, v1);
                        Ps2SetGpr64(ctx, 2, sx64(v0)); Ps2SetGpr64(ctx, 3, sx64(v1)); Ps2SetGpr64(ctx, 5, sx64(a1));
                        ctx->pc = target;
                        return;
                    }
                }
            }
            v1 = a1 >> 2;
            v0 = 2u;
            {
                const uint32_t target = ra;
                WRITE32(a0, v1);
                Ps2SetGpr64(ctx, 2, sx64(v0)); Ps2SetGpr64(ctx, 3, sx64(v1)); Ps2SetGpr64(ctx, 5, sx64(a1));
                ctx->pc = target;
                return;
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
        WRITE32(a0, a1);
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
        WRITE32(a0, a1);
    }
    v0 = v1;
L_12ec2c:
    {
        const uint32_t target = ra;
        Ps2SetGpr64(ctx, 2, sx64(v0)); Ps2SetGpr64(ctx, 3, sx64(v1)); Ps2SetGpr64(ctx, 5, sx64(a1));
        ctx->pc = target;
        return;
    }
}

// isnan (0x12fa00): lift_d1's text (the reference)
void isnan(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint64_t v0 = Ps2GprU64(ctx, 2), v1 = Ps2GprU64(ctx, 3), a0 = Ps2GprU64(ctx, 4), a1 = Ps2GprU64(ctx, 5), a2 = Ps2GprU64(ctx, 6), ra = Ps2GprU64(ctx, 31);
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
        const uint32_t target = (uint32_t)ra;
        (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)SRL32((uint32_t)v0, 31))));
        Ps2SetGpr64(ctx, 2, v0); Ps2SetGpr64(ctx, 3, v1); Ps2SetGpr64(ctx, 4, a0); Ps2SetGpr64(ctx, 5, a1); Ps2SetGpr64(ctx, 6, a2);
        ctx->pc = target;
        return;
    }
}

// iosCdStStop (0x1ace28): lift_d1's text (the reference)
void iosCdStStop(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t ra = Ps2GprU32(ctx, 31);
    {
        const uint32_t target = ra;
        
        ctx->pc = target;
        return;
    }
}

// iosCdStSkip (0x1acf40): lift_d1's text (the reference)
void iosCdStSkip(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint64_t v0 = Ps2GprU64(ctx, 2), v1 = Ps2GprU64(ctx, 3), a0 = Ps2GprU64(ctx, 4), ra = Ps2GprU64(ctx, 31);
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)32 << 16))));
    (a0 = (uint64_t)(a0 << (32 + 0)));
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v1, 4294953040))));
    (a0 = (uint64_t)(a0 >> (32 + 0)));
    (v0 = (uint64_t)(READ64_B((uint32_t)v1, 0)));
    (v0 = (uint64_t)((uint64_t)v0 + (uint64_t)a0));
    {
        const uint32_t target = (uint32_t)ra;
        WRITE64_B((uint32_t)v1, 0, v0);
        Ps2SetGpr64(ctx, 2, v0); Ps2SetGpr64(ctx, 3, v1); Ps2SetGpr64(ctx, 4, a0);
        ctx->pc = target;
        return;
    }
}

// iosCdSetErrorCallback (0x1acfb8): lift_d1's text (the reference)
void iosCdSetErrorCallback(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t v0 = Ps2GprU32(ctx, 2), v1 = Ps2GprU32(ctx, 3), a0 = Ps2GprU32(ctx, 4), a1 = Ps2GprU32(ctx, 5), ra = Ps2GprU32(ctx, 31);
    v0 = 0x1E0000u;
    v1 = 0x1E0000u;
    WRITE32_B(v0, 2880, a0);
    {
        const uint32_t target = ra;
        WRITE32_B(v1, 2884, a1);
        Ps2SetGpr64(ctx, 2, sx64(v0)); Ps2SetGpr64(ctx, 3, sx64(v1));
        ctx->pc = target;
        return;
    }
}

// iosCdSetMode (0x1acfd0): lift_d1's text (the reference)
void iosCdSetMode(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t v0 = Ps2GprU32(ctx, 2), v1 = Ps2GprU32(ctx, 3), a1 = Ps2GprU32(ctx, 5), ra = Ps2GprU32(ctx, 31);
    uint64_t a0 = Ps2GprU64(ctx, 4);
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
        const uint32_t target = ra;
        WRITE32(a1, v0);
        Ps2SetGpr64(ctx, 2, sx64(v0)); Ps2SetGpr64(ctx, 3, sx64(v1)); Ps2SetGpr64(ctx, 5, sx64(a1));
        ctx->pc = target;
        return;
    }
}

// iosInitDebPc (0x1ad140): lift_d1's text (the reference)
void iosInitDebPc(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t ra = Ps2GprU32(ctx, 31);
    {
        const uint32_t target = ra;
        
        ctx->pc = target;
        return;
    }
}

// iosStopPc (0x1ad148): lift_d1's text (the reference)
void iosStopPc(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t ra = Ps2GprU32(ctx, 31);
    {
        const uint32_t target = ra;
        
        ctx->pc = target;
        return;
    }
}

// iosDebPcVsyncCounterCheck (0x1ad150): lift_d1's text (the reference)
void iosDebPcVsyncCounterCheck(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t ra = Ps2GprU32(ctx, 31);
    {
        const uint32_t target = ra;
        
        ctx->pc = target;
        return;
    }
}

// iosGetCnt0Pc (0x1ad158): lift_d1's text (the reference)
void iosGetCnt0Pc(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t ra = Ps2GprU32(ctx, 31);
    uint64_t v0 = Ps2GprU64(ctx, 2);
    {
        const uint32_t target = ra;
        (v0 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL));
        Ps2SetGpr64(ctx, 2, v0);
        ctx->pc = target;
        return;
    }
}

// iosGetCnt1Pc (0x1ad160): lift_d1's text (the reference)
void iosGetCnt1Pc(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t ra = Ps2GprU32(ctx, 31);
    uint64_t v0 = Ps2GprU64(ctx, 2);
    {
        const uint32_t target = ra;
        (v0 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL));
        Ps2SetGpr64(ctx, 2, v0);
        ctx->pc = target;
        return;
    }
}

// InitDebPc (0x1ad168): lift_d1's text (the reference)
void InitDebPc(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t ra = Ps2GprU32(ctx, 31);
    {
        const uint32_t target = ra;
        
        ctx->pc = target;
        return;
    }
}

// iosGetClock (0x1ad1f0): lift_d1's text (the reference)
void iosGetClock(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t v0 = Ps2GprU32(ctx, 2), a0 = Ps2GprU32(ctx, 4), ra = Ps2GprU32(ctx, 31);
    uint64_t v1 = Ps2GprU64(ctx, 3);
    v0 = 0x200000u;
    (v1 = (uint64_t)(READ64_B(v0, -10088)));
    {
        const uint32_t target = ra;
        WRITE64_B(a0, 0, v1);
        Ps2SetGpr64(ctx, 2, sx64(v0)); Ps2SetGpr64(ctx, 3, v1);
        ctx->pc = target;
        return;
    }
}

// iosEE2IopEndChk (0x1ad948): lift_d1's text (the reference)
void iosEE2IopEndChk(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t v0 = Ps2GprU32(ctx, 2), ra = Ps2GprU32(ctx, 31);
    {
        const uint32_t target = ra;
        v0 = 1u;
        Ps2SetGpr64(ctx, 2, sx64(v0));
        ctx->pc = target;
        return;
    }
}

// iosCreateIopMgr (0x1ad950): lift_d1's text (the reference)
void iosCreateIopMgr(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t ra = Ps2GprU32(ctx, 31);
    {
        const uint32_t target = ra;
        
        ctx->pc = target;
        return;
    }
}

// iosGetMallocPtn (0x1b0530): lift_d1's text (the reference)
void iosGetMallocPtn(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t v0 = Ps2GprU32(ctx, 2), v1 = Ps2GprU32(ctx, 3), a0 = Ps2GprU32(ctx, 4), a1 = Ps2GprU32(ctx, 5), ra = Ps2GprU32(ctx, 31);
    v1 = READ32(a0 - 4u);
    v0 = READ8_B(v1, 11);
    a0 = READ8_B(v1, 7);
    v0 = v0 << 16;
    a1 = READ8_B(v1, 3);
    a0 = a0 << 8;
    v0 = v0 | a0;
    v0 = v0 | a1;
    {
        const uint32_t target = ra;
        v0 = v0 << 2;
        Ps2SetGpr64(ctx, 2, sx64(v0)); Ps2SetGpr64(ctx, 3, sx64(v1)); Ps2SetGpr64(ctx, 4, sx64(a0)); Ps2SetGpr64(ctx, 5, sx64(a1));
        ctx->pc = target;
        return;
    }
}

// iosGetMallocRootPtnRemainSize (0x1b0558): lift_d1's text (the reference)
void iosGetMallocRootPtnRemainSize(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t v0 = Ps2GprU32(ctx, 2), v1 = Ps2GprU32(ctx, 3), ra = Ps2GprU32(ctx, 31);
    v1 = 0x1E0000u;
    v0 = READ32_B(v1, 3056);
    {
        const uint32_t target = ra;
        v0 = v0 << 2;
        Ps2SetGpr64(ctx, 2, sx64(v0)); Ps2SetGpr64(ctx, 3, sx64(v1));
        ctx->pc = target;
        return;
    }
}

// addMemoryDebugCallback (0x1b06a8): lift_d1's text (the reference)
void addMemoryDebugCallback(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t v0 = Ps2GprU32(ctx, 2), a0 = Ps2GprU32(ctx, 4), ra = Ps2GprU32(ctx, 31);
    v0 = 0x1E0000u;
    {
        const uint32_t target = ra;
        WRITE32_B(v0, 3064, a0);
        Ps2SetGpr64(ctx, 2, sx64(v0));
        ctx->pc = target;
        return;
    }
}

// getMemorySafetyLockFlag (0x1b06b8): lift_d1's text (the reference)
void getMemorySafetyLockFlag(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t v0 = Ps2GprU32(ctx, 2), ra = Ps2GprU32(ctx, 31);
    v0 = 0x1E0000u;
    {
        const uint32_t target = ra;
        v0 = v0 + 0xBFCu;
        Ps2SetGpr64(ctx, 2, sx64(v0));
        ctx->pc = target;
        return;
    }
}

// kernel_sub_001B06C8 (0x1b06c8): lift_d1's text (the reference)
void kernel_sub_001B06C8(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t ra = Ps2GprU32(ctx, 31);
    // SYNC instruction - memory barrier // In recompiled code, we don't need explicit memory barriers
    {
        const uint32_t target = ra;
        
        ctx->pc = target;
        return;
    }
}

// iosInitMessageSystem (0x1b12b0): lift_d1's text (the reference)
void iosInitMessageSystem(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t ra = Ps2GprU32(ctx, 31);
    {
        const uint32_t target = ra;
        
        ctx->pc = target;
        return;
    }
}

// iosPadGetXZInputLwithThresholdIndepAxis (0x1b2438): lift_d1's text (the reference)
void iosPadGetXZInputLwithThresholdIndepAxis(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t at = Ps2GprU32(ctx, 1), v0 = Ps2GprU32(ctx, 2), a0 = Ps2GprU32(ctx, 4), a1 = Ps2GprU32(ctx, 5), ra = Ps2GprU32(ctx, 31);
    float f0 = ctx->f[0], f1 = ctx->f[1], f2 = ctx->f[2], f3 = ctx->f[3], f4 = ctx->f[4], f12 = ctx->f[12];
    uint32_t fcr31 = ctx->fcr31;
    v0 = READ8_B(a1, 84);
    at = 0x3C000000u;
    at = at | 0x8080u;
    f3 = lift_d1::flt(at);
    f0 = lift_d1::flt(v0);
    { int32_t tmp; std::memcpy(&tmp, &f0, sizeof(tmp)); f0 = FPU_CVT_S_W(tmp); }
    at = 0x3F800000u;
    f2 = lift_d1::flt(at);
    WRITE32_B(a0, 4, 0u);
    f1 = lift_d1::flt(READ32_B(a0, 4));
    f0 = FPU_MUL_S(f0, f3);
    f4 = FPU_SUB_SF(f0, f2);
    WRITE32(a0, lift_d1::bits(f4));
    fcr31 = (FPU_C_OLT_S(f4, f1)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
    v0 = READ8_B(a1, 85);
    WRITE32_B(a0, 12, lift_d1::bits(f1));
    f0 = lift_d1::flt(v0);
    { int32_t tmp; std::memcpy(&tmp, &f0, sizeof(tmp)); f0 = FPU_CVT_S_W(tmp); }
    f0 = FPU_MUL_S(f0, f3);
    f0 = FPU_SUB_SF(f0, f2);
    {
        const bool c = (fcr31 & 0x800000u) == 0u;
        WRITE32_B(a0, 8, lift_d1::bits(f0));
        if (!c) {
            f0 = FPU_NEG_S(f4);
            fcr31 = (FPU_C_OLT_S(f0, f12)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
            goto L_1b24a4;
        }
    }
    fcr31 = (FPU_C_OLT_S(f4, f12)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
L_1b24a4:
    if ((fcr31 & 0x800000u) == 0u) {
        at = 0x3F800000u;
        f3 = lift_d1::flt(at);
        f2 = lift_d1::flt(READ32(a0));
        f0 = lift_d1::flt(0u);
        f1 = FPU_SUB_S(f3, f12);
        at = 0xBF800000u;
        f4 = lift_d1::flt(at);
        fcr31 = (FPU_C_OLT_S(f2, f0)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
        f1 = ps2FpuDivide(fcr31, f3, f1);
        if ((fcr31 & 0x800000u) == 0u) {
            f4 = FPU_MOV_S(f3);
        } else {
            f0 = lift_d1::flt(0u);
            fcr31 = (FPU_C_OLT_S(f2, f0)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
        }
        if ((fcr31 & 0x800000u) != 0u) {
            f0 = FPU_NEG_S(f2);
        } else {
            f0 = FPU_SUB_S(f2, f12);
            goto L_1b24fc;
        }
        f0 = FPU_SUB_S(f0, f12);
L_1b24fc:
        f0 = FPU_MUL_S(f0, f1);
        f1 = FPU_MUL_S(f4, f0);
        goto L_1b250c;
    }
    f1 = lift_d1::flt(0u);
L_1b250c:
    f2 = lift_d1::flt(READ32_B(a0, 8));
    f0 = lift_d1::flt(0u);
    fcr31 = (FPU_C_OLT_S(f2, f0)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
    {
        const bool c = (fcr31 & 0x800000u) == 0u;
        WRITE32(a0, lift_d1::bits(f1));
        if (!c) {
            f0 = FPU_NEG_S(f2);
            fcr31 = (FPU_C_OLT_S(f0, f12)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
            goto L_1b2534;
        }
    }
    fcr31 = (FPU_C_OLT_S(f2, f12)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
L_1b2534:
    if ((fcr31 & 0x800000u) == 0u) {
        at = 0x3F800000u;
        f3 = lift_d1::flt(at);
        f0 = lift_d1::flt(0u);
        f1 = FPU_SUB_S(f3, f12);
        at = 0xBF800000u;
        f4 = lift_d1::flt(at);
        fcr31 = (FPU_C_OLT_S(f2, f0)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
        f1 = ps2FpuDivide(fcr31, f3, f1);
        if ((fcr31 & 0x800000u) == 0u) {
            f4 = FPU_MOV_S(f3);
        } else {
            f0 = lift_d1::flt(0u);
            fcr31 = (FPU_C_OLT_S(f2, f0)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
        }
        if ((fcr31 & 0x800000u) != 0u) {
            f0 = FPU_NEG_S(f2);
        } else {
            f0 = FPU_SUB_S(f2, f12);
            goto L_1b258c;
        }
        f0 = FPU_SUB_S(f0, f12);
L_1b258c:
        f0 = FPU_MUL_S(f0, f1);
        f0 = FPU_MUL_S(f4, f0);
        goto L_1b259c;
    }
    f0 = lift_d1::flt(0u);
L_1b259c:
    {
        const uint32_t target = ra;
        WRITE32_B(a0, 8, lift_d1::bits(f0));
        ctx->f[0] = f0; ctx->f[1] = f1; ctx->f[2] = f2; ctx->f[3] = f3; ctx->f[4] = f4; Ps2SetGpr64(ctx, 1, sx64(at)); Ps2SetGpr64(ctx, 2, sx64(v0)); ctx->fcr31 = fcr31;
        ctx->pc = target;
        return;
    }
}

// iosPadEntryTickFunc (0x1b27a0): lift_d1's text (the reference)
void iosPadEntryTickFunc(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t v0 = Ps2GprU32(ctx, 2), a0 = Ps2GprU32(ctx, 4), ra = Ps2GprU32(ctx, 31);
    v0 = 0x1E0000u;
    {
        const uint32_t target = ra;
        WRITE32_B(v0, 3496, a0);
        Ps2SetGpr64(ctx, 2, sx64(v0));
        ctx->pc = target;
        return;
    }
}

// iosCreatePrintMgr (0x1b29e8): lift_d1's text (the reference)
void iosCreatePrintMgr(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t ra = Ps2GprU32(ctx, 31);
    {
        const uint32_t target = ra;
        
        ctx->pc = target;
        return;
    }
}

// iosGetPrintfMode (0x1b29f0): lift_d1's text (the reference)
void iosGetPrintfMode(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t v0 = Ps2GprU32(ctx, 2), ra = Ps2GprU32(ctx, 31);
    v0 = 0x1E0000u;
    {
        const uint32_t target = ra;
        v0 = v0 + 0xEA0u;
        Ps2SetGpr64(ctx, 2, sx64(v0));
        ctx->pc = target;
        return;
    }
}

// endSCEDemo (0x1b34f0): lift_d1's text (the reference)
void endSCEDemo(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t ra = Ps2GprU32(ctx, 31);
    {
        const uint32_t target = ra;
        
        ctx->pc = target;
        return;
    }
}

// Init_ShockVoiceSet (0x1b3c18): lift_d1's text (the reference)
void Init_ShockVoiceSet(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t v0 = Ps2GprU32(ctx, 2), v1 = Ps2GprU32(ctx, 3), a0 = Ps2GprU32(ctx, 4), a1 = Ps2GprU32(ctx, 5), ra = Ps2GprU32(ctx, 31);
    WRITE32(a0, a1);
    v0 = READ16_B(a1, 10);
    v0 = v0 << 2;
    v0 = a1 + v0;
    WRITE32_B(a0, 12, v0);
    v1 = READ16_B(a1, 2);
    v1 = v1 << 2;
    v1 = a1 + v1;
    WRITE32_B(a0, 4, v1);
    v0 = READ16_B(a1, 6);
    v0 = v0 << 2;
    a1 = a1 + v0;
    {
        const uint32_t target = ra;
        WRITE32_B(a0, 8, a1);
        Ps2SetGpr64(ctx, 2, sx64(v0)); Ps2SetGpr64(ctx, 3, sx64(v1)); Ps2SetGpr64(ctx, 5, sx64(a1));
        ctx->pc = target;
        return;
    }
}

// Shock_SetShockVoiceSet (0x1b3d10): lift_d1's text (the reference)
void Shock_SetShockVoiceSet(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t v1 = Ps2GprU32(ctx, 3), a1 = Ps2GprU32(ctx, 5), a2 = Ps2GprU32(ctx, 6), ra = Ps2GprU32(ctx, 31);
    uint64_t v0 = Ps2GprU64(ctx, 2), a0 = Ps2GprU64(ctx, 4), a3 = Ps2GprU64(ctx, 7);
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)30 << 16))));
    (a2 = (uint32_t)((int32_t)((int32_t)SLL32((uint32_t)a0, 2))));
    (v1 = (uint32_t)((int32_t)((int32_t)READ32_B((uint32_t)v0, 3752))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)READ32_B(v1, 0))));
    (v0 = (uint64_t)(((uint64_t)a0 < (uint64_t)RC_U64X(v0)) ? 1 : 0));
    {
        const bool c = (int64_t)v0 == 0;
        (a3 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 4294967295))));
        if (!c) {
            (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)READ32_B(v1, 4))));
            (a3 = (uint64_t)((uint64_t)a0 + (uint64_t)0ULL));
            (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(a2, (uint32_t)v0))));
            WRITE32_B((uint32_t)v0, 0, a1);
        }
    }
    {
        const uint32_t target = ra;
        (v0 = (uint64_t)((uint64_t)a3 + (uint64_t)0ULL));
        Ps2SetGpr64(ctx, 2, v0); Ps2SetGpr64(ctx, 3, sx64(v1)); Ps2SetGpr64(ctx, 6, sx64(a2)); Ps2SetGpr64(ctx, 7, a3);
        ctx->pc = target;
        return;
    }
}

// Init_Player (0x1b3d48): lift_d1's text (the reference)
void Init_Player(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t v0 = Ps2GprU32(ctx, 2), v1 = Ps2GprU32(ctx, 3), a0 = Ps2GprU32(ctx, 4), a1 = Ps2GprU32(ctx, 5), a2 = Ps2GprU32(ctx, 6), ra = Ps2GprU32(ctx, 31);
    v0 = 0x1B0000u;
    v1 = 0x1B0000u;
    a1 = v0 + 0x40D0u;
    v0 = 0x200000u;
    v1 = v1 + 0x4118u;
    a2 = v0 + 0x21E8u;
    {
        const bool c = a1 == 0;
        WRITE32(a0, 0u);
        if (!c) {
            WRITE32_B(a0, 4, a1);
            goto L_1b3d7c;
        }
    }
    v0 = 0x1B0000u;
    v0 = v0 + 0x40C8u;
    WRITE32_B(a0, 4, v0);
L_1b3d7c:
    WRITE32_B(a0, 12, a2);
    {
        const uint32_t target = ra;
        WRITE32_B(a0, 8, v1);
        Ps2SetGpr64(ctx, 2, sx64(v0)); Ps2SetGpr64(ctx, 3, sx64(v1)); Ps2SetGpr64(ctx, 5, sx64(a1)); Ps2SetGpr64(ctx, 6, sx64(a2));
        ctx->pc = target;
        return;
    }
}

// Init_Controler (0x1b3d88): lift_d1's text (the reference)
void Init_Controler(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t a0 = Ps2GprU32(ctx, 4), ra = Ps2GprU32(ctx, 31);
    WRITE16(a0, (uint16_t)0u);
    {
        const uint32_t target = ra;
        WRITE16_B(a0, 2, (uint16_t)0u);
        
        ctx->pc = target;
        return;
    }
}

// kernel_sub_001B40C8 (0x1b40c8): lift_d1's text (the reference)
void kernel_sub_001B40C8(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t ra = Ps2GprU32(ctx, 31);
    uint64_t v0 = Ps2GprU64(ctx, 2);
    {
        const uint32_t target = ra;
        (v0 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL));
        Ps2SetGpr64(ctx, 2, v0);
        ctx->pc = target;
        return;
    }
}

// kernel_sub_001B4118 (0x1b4118): lift_d1's text (the reference)
void kernel_sub_001B4118(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t a0 = Ps2GprU32(ctx, 4), ra = Ps2GprU32(ctx, 31);
    {
        const uint32_t target = ra;
        WRITE8(a0, (uint8_t)0u);
        
        ctx->pc = target;
        return;
    }
}

// IosGetMergeDataFileName (0x1b5f18): lift_d1's text (the reference)
void IosGetMergeDataFileName(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t v0 = Ps2GprU32(ctx, 2), ra = Ps2GprU32(ctx, 31);
    v0 = 0x1E0000u;
    {
        const uint32_t target = ra;
        v0 = v0 + 0x2780u;
        Ps2SetGpr64(ctx, 2, sx64(v0));
        ctx->pc = target;
        return;
    }
}

// GetSymbolByIndex (0x1b6798): lift_d1's text (the reference)
void GetSymbolByIndex(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t v0 = Ps2GprU32(ctx, 2), v1 = Ps2GprU32(ctx, 3), a0 = Ps2GprU32(ctx, 4), a1 = Ps2GprU32(ctx, 5), a2 = Ps2GprU32(ctx, 6), ra = Ps2GprU32(ctx, 31);
    v0 = READ32_B(a0, 28);
    v1 = READ32_B(v0, 64);
    a0 = READ32_B(v0, 40);
    a1 = a1 + v1;
    a2 = READ32_B(v0, 44);
    a1 = a1 << 4;
    a1 = a1 + a0;
    v0 = READ32(a1);
    {
        const uint32_t target = ra;
        v0 = a2 + v0;
        Ps2SetGpr64(ctx, 2, sx64(v0)); Ps2SetGpr64(ctx, 3, sx64(v1)); Ps2SetGpr64(ctx, 4, sx64(a0)); Ps2SetGpr64(ctx, 5, sx64(a1)); Ps2SetGpr64(ctx, 6, sx64(a2));
        ctx->pc = target;
        return;
    }
}

// isysGetXffWorkSize (0x1b6828): lift_d1's text (the reference)
void isysGetXffWorkSize(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t v0 = Ps2GprU32(ctx, 2), v1 = Ps2GprU32(ctx, 3), a0 = Ps2GprU32(ctx, 4), a1 = Ps2GprU32(ctx, 5), a2 = Ps2GprU32(ctx, 6), ra = Ps2GprU32(ctx, 31);
    v0 = READ8_B(a0, 12);
    {
        const bool c = v0 != 0;
        a1 = v0 + 1u;
        if (!c) {
            a1 = 2u;
        }
    }
    v1 = READ32_B(a0, 28);
    v0 = READ32_B(a0, 36);
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
        const uint32_t target = ra;
        Ps2SetGpr64(ctx, 2, sx64(v0)); Ps2SetGpr64(ctx, 3, sx64(v1)); Ps2SetGpr64(ctx, 4, sx64(a0)); Ps2SetGpr64(ctx, 5, sx64(a1)); Ps2SetGpr64(ctx, 6, sx64(a2));
        ctx->pc = target;
        return;
    }
}

// isysGetModuleObjMallocSize (0x1b6880): lift_d1's text (the reference)
void isysGetModuleObjMallocSize(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t v0 = Ps2GprU32(ctx, 2), v1 = Ps2GprU32(ctx, 3), a0 = Ps2GprU32(ctx, 4), ra = Ps2GprU32(ctx, 31);
    v0 = READ32_B(a0, 44);
    v1 = 0x40000000u;
    v0 = v0 & v1;
    {
        const bool c = v0 == 0;
        v0 = READ32_B(a0, 28);
        if (!c) {
            v1 = READ32_B(v0, 4);
            v0 = v1 - 4u;
            goto L_1b68a4;
        }
    }
    v0 = v0 - 4u;
L_1b68a4:
    a0 = READ32(v0);
    v1 = 0xFF0000u;
    v1 = v1 | 0xFFFFu;
    v0 = READ32(a0);
    v0 = v0 & v1;
    {
        const uint32_t target = ra;
        v0 = v0 << 2;
        Ps2SetGpr64(ctx, 2, sx64(v0)); Ps2SetGpr64(ctx, 3, sx64(v1)); Ps2SetGpr64(ctx, 4, sx64(a0));
        ctx->pc = target;
        return;
    }
}

// GetExecModuleSize (0x1b69b8): lift_d1's text (the reference)
void GetExecModuleSize(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t v0 = Ps2GprU32(ctx, 2), v1 = Ps2GprU32(ctx, 3), ra = Ps2GprU32(ctx, 31);
    v1 = 0x1E0000u;
    {
        const uint32_t target = ra;
        v0 = READ32_B(v1, 3760);
        Ps2SetGpr64(ctx, 2, sx64(v0)); Ps2SetGpr64(ctx, 3, sx64(v1));
        ctx->pc = target;
        return;
    }
}

// getModuleSize (0x1b69c8): lift_d1's text (the reference)
void getModuleSize(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t v0 = Ps2GprU32(ctx, 2), v1 = Ps2GprU32(ctx, 3), a0 = Ps2GprU32(ctx, 4), ra = Ps2GprU32(ctx, 31);
    v1 = READ32_B(a0, 28);
    {
        const uint32_t target = ra;
        v0 = READ32_B(v1, 20);
        Ps2SetGpr64(ctx, 2, sx64(v0)); Ps2SetGpr64(ctx, 3, sx64(v1));
        ctx->pc = target;
        return;
    }
}

// isysCreateExceptionThread (0x1b7b58): lift_d1's text (the reference)
void isysCreateExceptionThread(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t ra = Ps2GprU32(ctx, 31);
    {
        const uint32_t target = ra;
        
        ctx->pc = target;
        return;
    }
}

// iosCubicBezier (0x1b7b78): lift_d1's text (the reference)
void iosCubicBezier(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t at = Ps2GprU32(ctx, 1), a0 = Ps2GprU32(ctx, 4), ra = Ps2GprU32(ctx, 31);
    float f0 = ctx->f[0], f1 = ctx->f[1], f2 = ctx->f[2], f3 = ctx->f[3], f4 = ctx->f[4], f5 = ctx->f[5], f12 = ctx->f[12];
    at = 0x3F800000u;
    f0 = lift_d1::flt(at);
    f2 = FPU_MUL_S(f12, f12);
    f4 = lift_d1::flt(READ32(a0));
    f0 = FPU_SUB_S(f0, f12);
    f5 = lift_d1::flt(READ32_B(a0, 4));
    f3 = lift_d1::flt(READ32_B(a0, 8));
    f2 = FPU_MUL_S(f2, f3);
    f1 = FPU_ADD_SF(f0, f0);
    f0 = FPU_MUL_SF(f0, f0);
    f1 = FPU_MUL_S(f1, f12);
    f0 = FPU_MUL_S(f0, f4);
    f1 = FPU_MUL_S(f1, f5);
    f0 = FPU_ADD_SF(f0, f1);
    {
        const uint32_t target = ra;
        f0 = FPU_ADD_S(f0, f2);
        ctx->f[0] = f0; ctx->f[1] = f1; ctx->f[2] = f2; ctx->f[3] = f3; ctx->f[4] = f4; ctx->f[5] = f5; Ps2SetGpr64(ctx, 1, sx64(at));
        ctx->pc = target;
        return;
    }
}

// iosFourthBezier (0x1b7c20): lift_d1's text (the reference)
void iosFourthBezier(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t at = Ps2GprU32(ctx, 1), a0 = Ps2GprU32(ctx, 4), ra = Ps2GprU32(ctx, 31);
    float f0 = ctx->f[0], f1 = ctx->f[1], f2 = ctx->f[2], f3 = ctx->f[3], f4 = ctx->f[4], f5 = ctx->f[5], f6 = ctx->f[6], f7 = ctx->f[7], f8 = ctx->f[8], f9 = ctx->f[9], f12 = ctx->f[12];
    at = 0x3F800000u;
    f4 = lift_d1::flt(at);
    f6 = FPU_MUL_S(f12, f12);
    at = 0x40400000u;
    f1 = lift_d1::flt(at);
    f4 = FPU_SUB_S(f4, f12);
    f8 = lift_d1::flt(READ32(a0));
    f9 = lift_d1::flt(READ32_B(a0, 4));
    f7 = lift_d1::flt(READ32_B(a0, 8));
    f5 = FPU_MUL_S(f6, f12);
    f3 = lift_d1::flt(READ32_B(a0, 12));
    f0 = FPU_MUL_SF(f4, f4);
    f2 = FPU_MUL_SF(f4, f1);
    f5 = FPU_MUL_S(f5, f3);
    f1 = FPU_MUL_SF(f0, f1);
    f0 = FPU_MUL_SF(f0, f4);
    f2 = FPU_MUL_SF(f2, f6);
    f1 = FPU_MUL_S(f1, f12);
    f0 = FPU_MUL_S(f0, f8);
    f2 = FPU_MUL_S(f2, f7);
    f1 = FPU_MUL_S(f1, f9);
    f0 = FPU_ADD_SF(f0, f1);
    f0 = FPU_ADD_SF(f0, f2);
    {
        const uint32_t target = ra;
        f0 = FPU_ADD_S(f0, f5);
        ctx->f[0] = f0; ctx->f[1] = f1; ctx->f[2] = f2; ctx->f[3] = f3; ctx->f[4] = f4; ctx->f[5] = f5; ctx->f[6] = f6; ctx->f[7] = f7; ctx->f[8] = f8; ctx->f[9] = f9; Ps2SetGpr64(ctx, 1, sx64(at));
        ctx->pc = target;
        return;
    }
}

// iosFloatModulo (0x1b9308): lift_d1's text (the reference)
void iosFloatModulo(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t v0 = Ps2GprU32(ctx, 2), ra = Ps2GprU32(ctx, 31);
    float f0 = ctx->f[0], f1 = ctx->f[1], f12 = ctx->f[12], f13 = ctx->f[13];
    uint32_t fcr31 = ctx->fcr31;
    bool df_v0 = false;
    f0 = lift_d1::flt(0u);
    fcr31 = (FPU_C_EQ_S(f13, f0)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
    if ((fcr31 & 0x800000u) == 0u) {
        f0 = ps2FpuDivide(fcr31, f12, f13);
        { int32_t tmp = FPU_CVT_W_S(f0); std::memcpy(&f1, &tmp, sizeof(tmp)); }
        v0 = lift_d1::bits(f1); df_v0 = true;
        f0 = lift_d1::flt(v0);
        { int32_t tmp; std::memcpy(&tmp, &f0, sizeof(tmp)); f0 = FPU_CVT_S_W(tmp); }
        f0 = FPU_MUL_S(f13, f0);
        f0 = FPU_SUB_S(f12, f0);
    }
    {
        const uint32_t target = ra;
        ctx->f[0] = f0; ctx->f[1] = f1; if (df_v0) Ps2SetGpr64(ctx, 2, sx64(v0)); ctx->fcr31 = fcr31;
        ctx->pc = target;
        return;
    }
}

// iosIColToMvecSl (0x1ba428): lift_d1's text (the reference)
void iosIColToMvecSl(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t v0 = Ps2GprU32(ctx, 2), v1 = Ps2GprU32(ctx, 3), a0 = Ps2GprU32(ctx, 4), a1 = Ps2GprU32(ctx, 5), a2 = Ps2GprU32(ctx, 6), ra = Ps2GprU32(ctx, 31);
    v0 = a1 >> 8;
    v1 = a1 >> 16;
    a2 = a1 >> 24;
    a1 = a1 & 0xFFu;
    v0 = v0 & 0xFFu;
    v1 = v1 & 0xFFu;
    WRITE32_B(a0, 12, a2);
    WRITE32(a0, a1);
    WRITE32_B(a0, 4, v0);
    {
        const uint32_t target = ra;
        WRITE32_B(a0, 8, v1);
        Ps2SetGpr64(ctx, 2, sx64(v0)); Ps2SetGpr64(ctx, 3, sx64(v1)); Ps2SetGpr64(ctx, 5, sx64(a1)); Ps2SetGpr64(ctx, 6, sx64(a2));
        ctx->pc = target;
        return;
    }
}

// GetCurrentQuaternion (0x1bb868): lift_d1's text (the reference)
void GetCurrentQuaternion(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t at = Ps2GprU32(ctx, 1), v0 = Ps2GprU32(ctx, 2), v1 = Ps2GprU32(ctx, 3), ra = Ps2GprU32(ctx, 31);
    v1 = 0x1E0000u;
    v0 = READ32_B(v1, 3936);
    v0 = v0 << 4;
    at = 0x200000u;
    at = at + 0x2AC0u;
    v0 = at + v0;
    {
        const uint32_t target = ra;
        Ps2SetGpr64(ctx, 1, sx64(at)); Ps2SetGpr64(ctx, 2, sx64(v0)); Ps2SetGpr64(ctx, 3, sx64(v1));
        ctx->pc = target;
        return;
    }
}

// GetLastQuaternion (0x1bb888): lift_d1's text (the reference)
void GetLastQuaternion(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t v0 = Ps2GprU32(ctx, 2), v1 = Ps2GprU32(ctx, 3), a0 = Ps2GprU32(ctx, 4), ra = Ps2GprU32(ctx, 31);
    v1 = 0x1E0000u;
    v0 = 0x200000u;
    a0 = READ32_B(v1, 3936);
    v0 = v0 + 0x2AC0u;
    v0 = v0 - 0x10u;
    a0 = a0 << 4;
    {
        const uint32_t target = ra;
        v0 = a0 + v0;
        Ps2SetGpr64(ctx, 2, sx64(v0)); Ps2SetGpr64(ctx, 3, sx64(v1)); Ps2SetGpr64(ctx, 4, sx64(a0));
        ctx->pc = target;
        return;
    }
}

// iosGetACos (0x1be2e8): lift_d1's text (the reference)
void iosGetACos(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t at = Ps2GprU32(ctx, 1), a0 = Ps2GprU32(ctx, 4), ra = Ps2GprU32(ctx, 31);
    float f0 = ctx->f[0], f1 = ctx->f[1], f2 = ctx->f[2], f3 = ctx->f[3], f4 = ctx->f[4], f5 = ctx->f[5], f6 = ctx->f[6], f7 = ctx->f[7];
    f3 = lift_d1::flt(a0);
    { int32_t tmp; std::memcpy(&tmp, &f3, sizeof(tmp)); f3 = FPU_CVT_S_W(tmp); }
    at = 0x2FFF0000u;
    at = at | 0xFFF5u;
    f0 = lift_d1::flt(at);
    at = 0x3E2A0000u;
    at = at | 0xAAAAu;
    f2 = lift_d1::flt(at);
    at = 0x3FC90000u;
    at = at | 0xFDAu;
    f4 = lift_d1::flt(at);
    at = 0x3D990000u;
    at = at | 0x9999u;
    f5 = lift_d1::flt(at);
    f3 = FPU_MUL_S(f3, f0);
    at = 0x3D360000u;
    at = at | 0xDB6Du;
    f6 = lift_d1::flt(at);
    at = 0x3CF80000u;
    at = at | 0xE38Du;
    f7 = lift_d1::flt(at);
    f0 = FPU_MUL_SF(f3, f3);
    f4 = FPU_SUB_S(f4, f3);
    f0 = FPU_MUL_SF(f0, f3);
    f1 = FPU_MUL_SF(f0, f3);
    f0 = FPU_MUL_S(f0, f2);
    f1 = FPU_MUL_SF(f1, f3);
    f4 = FPU_SUB_SF(f4, f0);
    f2 = FPU_MUL_SF(f1, f3);
    f1 = FPU_MUL_S(f1, f5);
    f2 = FPU_MUL_SF(f2, f3);
    f4 = FPU_SUB_SF(f4, f1);
    f0 = FPU_MUL_SF(f2, f3);
    f2 = FPU_MUL_S(f2, f6);
    f0 = FPU_MUL_SF(f0, f3);
    f4 = FPU_SUB_SF(f4, f2);
    f0 = FPU_MUL_S(f0, f7);
    {
        const uint32_t target = ra;
        f0 = FPU_SUB_S(f4, f0);
        ctx->f[0] = f0; ctx->f[1] = f1; ctx->f[2] = f2; ctx->f[3] = f3; ctx->f[4] = f4; ctx->f[5] = f5; ctx->f[6] = f6; ctx->f[7] = f7; Ps2SetGpr64(ctx, 1, sx64(at));
        ctx->pc = target;
        return;
    }
}

// iosGetASin (0x1be4d8): lift_d1's text (the reference)
void iosGetASin(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t ra = Ps2GprU32(ctx, 31);
    float f0 = ctx->f[0];
    f0 = lift_d1::flt(0u);
    {
        const uint32_t target = ra;
        ctx->f[0] = f0;
        ctx->pc = target;
        return;
    }
}

// iosGetATan2 (0x1be4e8): lift_d1's text (the reference)
void iosGetATan2(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t ra = Ps2GprU32(ctx, 31);
    float f0 = ctx->f[0];
    f0 = lift_d1::flt(0u);
    {
        const uint32_t target = ra;
        ctx->f[0] = f0;
        ctx->pc = target;
        return;
    }
}

// resetArcFuncCall (0x1be4f8): lift_d1's text (the reference)
void resetArcFuncCall(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t v0 = Ps2GprU32(ctx, 2), v1 = Ps2GprU32(ctx, 3), a0 = Ps2GprU32(ctx, 4), a1 = Ps2GprU32(ctx, 5), ra = Ps2GprU32(ctx, 31);
    a1 = 0x1E0000u;
    a0 = 0x1E0000u;
    v1 = 0x1E0000u;
    v0 = 0x1E0000u;
    WRITE32_B(v0, 3968, 0u);
    WRITE32_B(v1, 3964, 0u);
    WRITE32_B(a0, 3960, 0u);
    {
        const uint32_t target = ra;
        WRITE32_B(a1, 3956, 0u);
        Ps2SetGpr64(ctx, 2, sx64(v0)); Ps2SetGpr64(ctx, 3, sx64(v1)); Ps2SetGpr64(ctx, 4, sx64(a0)); Ps2SetGpr64(ctx, 5, sx64(a1));
        ctx->pc = target;
        return;
    }
}

// iosGetTableSin (0x1be520): lift_d1's text (the reference)
void iosGetTableSin(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t at = Ps2GprU32(ctx, 1), v0 = Ps2GprU32(ctx, 2), a0 = Ps2GprU32(ctx, 4), ra = Ps2GprU32(ctx, 31);
    uint64_t v1 = Ps2GprU64(ctx, 3);
    float f0 = ctx->f[0];
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
    f0 = lift_d1::flt(READ32_B(at, 11968));
    if ((int64_t)v1 != 0) {
        f0 = FPU_NEG_S(f0);
    } else {
    }
    {
        const uint32_t target = ra;
        ctx->f[0] = f0; Ps2SetGpr64(ctx, 1, sx64(at)); Ps2SetGpr64(ctx, 2, sx64(v0)); Ps2SetGpr64(ctx, 3, v1); Ps2SetGpr64(ctx, 4, sx64(a0));
        ctx->pc = target;
        return;
    }
}

// iosDumpFVector (0x1be8e0): lift_d1's text (the reference)
void iosDumpFVector(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t ra = Ps2GprU32(ctx, 31);
    {
        const uint32_t target = ra;
        
        ctx->pc = target;
        return;
    }
}

// iosDumpIVector (0x1be8e8): lift_d1's text (the reference)
void iosDumpIVector(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t ra = Ps2GprU32(ctx, 31);
    {
        const uint32_t target = ra;
        
        ctx->pc = target;
        return;
    }
}

// iosGetScaleInverseVector (0x1be980): lift_d1's text (the reference)
void iosGetScaleInverseVector(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t at = Ps2GprU32(ctx, 1), a0 = Ps2GprU32(ctx, 4), a1 = Ps2GprU32(ctx, 5), ra = Ps2GprU32(ctx, 31);
    float f0 = ctx->f[0], f1 = ctx->f[1], f2 = ctx->f[2];
    uint32_t fcr31 = ctx->fcr31;
    f1 = lift_d1::flt(READ32(a1));
    at = 0x3F800000u;
    f2 = lift_d1::flt(at);
    f1 = ps2FpuDivide(fcr31, f2, f1);
    WRITE32(a0, lift_d1::bits(f1));
    f0 = lift_d1::flt(READ32_B(a1, 4));
    f0 = ps2FpuDivide(fcr31, f2, f0);
    WRITE32_B(a0, 4, lift_d1::bits(f0));
    f1 = lift_d1::flt(READ32_B(a1, 8));
    f1 = ps2FpuDivide(fcr31, f2, f1);
    WRITE32_B(a0, 8, lift_d1::bits(f1));
    f0 = lift_d1::flt(READ32_B(a1, 12));
    f2 = ps2FpuDivide(fcr31, f2, f0);
    {
        const uint32_t target = ra;
        WRITE32_B(a0, 12, lift_d1::bits(f2));
        ctx->f[0] = f0; ctx->f[1] = f1; ctx->f[2] = f2; Ps2SetGpr64(ctx, 1, sx64(at)); ctx->fcr31 = fcr31;
        ctx->pc = target;
        return;
    }
}

// iosSetDCViewPort (0x1bf0b0): lift_d1's text (the reference)
void iosSetDCViewPort(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint64_t v0 = Ps2GprU64(ctx, 2), v1 = Ps2GprU64(ctx, 3), a0 = Ps2GprU64(ctx, 4), a1 = Ps2GprU64(ctx, 5), a2 = Ps2GprU64(ctx, 6), a3 = Ps2GprU64(ctx, 7), t0 = Ps2GprU64(ctx, 8), t1 = Ps2GprU64(ctx, 9), t2 = Ps2GprU64(ctx, 10), t3 = Ps2GprU64(ctx, 11), t4 = Ps2GprU64(ctx, 12), ra = Ps2GprU64(ctx, 31);
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
    WRITE64_B((uint32_t)a0, 128, t2);
    WRITE64_B((uint32_t)a0, 104, RC_U64X(v0));
    WRITE64_B((uint32_t)a0, 120, RC_U64X(v1));
    WRITE64_B((uint32_t)a0, 96, t1);
    WRITE64_B((uint32_t)a0, 136, RC_U64X(t3));
    WRITE64_B((uint32_t)a0, 152, RC_U64X(t4));
    WRITE32_B((uint32_t)a0, 372, (uint32_t)a1);
    WRITE32_B((uint32_t)a0, 376, (uint32_t)a2);
    WRITE32_B((uint32_t)a0, 380, (uint32_t)a3);
    WRITE32_B((uint32_t)a0, 384, (uint32_t)t0);
    WRITE64_B((uint32_t)a0, 112, t1);
    {
        const uint32_t target = (uint32_t)ra;
        WRITE64_B((uint32_t)a0, 144, t2);
        Ps2SetGpr64(ctx, 10, t2); Ps2SetGpr64(ctx, 11, t3); Ps2SetGpr64(ctx, 12, t4); Ps2SetGpr64(ctx, 2, v0); Ps2SetGpr64(ctx, 3, v1); Ps2SetGpr64(ctx, 9, t1);
        ctx->pc = target;
        return;
    }
}

// iosGetRootDC (0x1bf2d8): lift_d1's text (the reference)
void iosGetRootDC(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t v0 = Ps2GprU32(ctx, 2), v1 = Ps2GprU32(ctx, 3), ra = Ps2GprU32(ctx, 31);
    v1 = 0x1E0000u;
    {
        const uint32_t target = ra;
        v0 = READ32_B(v1, 4244);
        Ps2SetGpr64(ctx, 2, sx64(v0)); Ps2SetGpr64(ctx, 3, sx64(v1));
        ctx->pc = target;
        return;
    }
}

// iosGetCurrentDC (0x1bf308): lift_d1's text (the reference)
void iosGetCurrentDC(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t v0 = Ps2GprU32(ctx, 2), v1 = Ps2GprU32(ctx, 3), ra = Ps2GprU32(ctx, 31);
    v1 = 0x1E0000u;
    {
        const uint32_t target = ra;
        v0 = READ32_B(v1, 4240);
        Ps2SetGpr64(ctx, 2, sx64(v0)); Ps2SetGpr64(ctx, 3, sx64(v1));
        ctx->pc = target;
        return;
    }
}

// iosGetDCGroup (0x1bf420): lift_d1's text (the reference)
void iosGetDCGroup(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t v0 = Ps2GprU32(ctx, 2), ra = Ps2GprU32(ctx, 31);
    v0 = 0x210000u;
    {
        const uint32_t target = ra;
        v0 = v0 + 0x32C8u;
        Ps2SetGpr64(ctx, 2, sx64(v0));
        ctx->pc = target;
        return;
    }
}

// kernel_sub_001BF430 (0x1bf430): lift_d1's text (the reference)
void kernel_sub_001BF430(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t ra = Ps2GprU32(ctx, 31);
    {
        const uint32_t target = ra;
        
        ctx->pc = target;
        return;
    }
}

// kernel_sub_001BF438 (0x1bf438): lift_d1's text (the reference)
void kernel_sub_001BF438(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t ra = Ps2GprU32(ctx, 31);
    {
        const uint32_t target = ra;
        
        ctx->pc = target;
        return;
    }
}

// iosDlGetTransAdrs (0x1bfb80): lift_d1's text (the reference)
void iosDlGetTransAdrs(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t v0 = Ps2GprU32(ctx, 2), v1 = Ps2GprU32(ctx, 3), ra = Ps2GprU32(ctx, 31);
    v0 = 0x210000u;
    v0 = v0 + 0x32D8u;
    v1 = READ32_B(v0, 24);
    v0 = v0 + 8u;
    v1 = v1 << 2;
    v1 = v1 + v0;
    v0 = READ32(v1);
    {
        const uint32_t target = ra;
        Ps2SetGpr64(ctx, 2, sx64(v0)); Ps2SetGpr64(ctx, 3, sx64(v1));
        ctx->pc = target;
        return;
    }
}

// iosGetPacketPtrPath1 (0x1bfba8): lift_d1's text (the reference)
void iosGetPacketPtrPath1(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t v0 = Ps2GprU32(ctx, 2), v1 = Ps2GprU32(ctx, 3), a0 = Ps2GprU32(ctx, 4), a1 = Ps2GprU32(ctx, 5), ra = Ps2GprU32(ctx, 31);
    bool df_v1 = false;
    a1 = 0x210000u;
    a0 = a1 + 0x32D8u;
    v0 = READ32_B(a0, 16);
    v0 = v0 & 0xFu;
    if (v0 != 0) {
        v1 = READ32_B(a0, 16); df_v1 = true;
        v0 = READ32_B(a0, 16);
        v1 = v1 & 0xFu; df_v1 = true;
        v0 = v0 - v1;
        v0 = v0 + 0x10u;
        WRITE32_B(a0, 16, v0);
    }
    v0 = READ32_B(a0, 16);
    // PREF instruction (ignored)
    v1 = a1 + 0x32D8u; df_v1 = true;
    v0 = READ32_B(v1, 16);
    {
        const uint32_t target = ra;
        Ps2SetGpr64(ctx, 2, sx64(v0)); if (df_v1) Ps2SetGpr64(ctx, 3, sx64(v1)); Ps2SetGpr64(ctx, 4, sx64(a0)); Ps2SetGpr64(ctx, 5, sx64(a1));
        ctx->pc = target;
        return;
    }
}

// iosSetPacketPtrPath1 (0x1bfbf0): lift_d1's text (the reference)
void iosSetPacketPtrPath1(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t v0 = Ps2GprU32(ctx, 2), a0 = Ps2GprU32(ctx, 4), ra = Ps2GprU32(ctx, 31);
    v0 = 0x210000u;
    v0 = v0 + 0x32D8u;
    WRITE32_B(v0, 16, a0);
    {
        const uint32_t target = ra;
        Ps2SetGpr64(ctx, 2, sx64(v0));
        ctx->pc = target;
        return;
    }
}

// iosDlChainHead (0x1bfc30): lift_d1's text (the reference)
void iosDlChainHead(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint64_t v0 = Ps2GprU64(ctx, 2), v1 = Ps2GprU64(ctx, 3), a0 = Ps2GprU64(ctx, 4), a1 = Ps2GprU64(ctx, 5), a2 = Ps2GprU64(ctx, 6), a3 = Ps2GprU64(ctx, 7), t0 = Ps2GprU64(ctx, 8), t1 = Ps2GprU64(ctx, 9), ra = Ps2GprU64(ctx, 31);
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)33 << 16))));
    (a2 = (uint64_t)((int64_t)(int32_t)((int32_t)SLL32((uint32_t)a0, 1))));
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v1, 13016))));
    (a2 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)a2, (uint32_t)a0))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)READ32_B((uint32_t)v1, 24))));
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v1, 8))));
    (a2 = (uint64_t)((int64_t)(int32_t)((int32_t)SLL32((uint32_t)a2, 4))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)SLL32((uint32_t)v0, 2))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v0, (uint32_t)a0))));
    (a3 = (uint64_t)((int64_t)(int32_t)((int32_t)READ32_B((uint32_t)v0, 0))));
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)READ32_B((uint32_t)v1, 16))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)READ32_B((uint32_t)v1, 16))));
    (a3 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)a3, (uint32_t)a2))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v0, 32))));
    WRITE32_B((uint32_t)v1, 16, (uint32_t)v0);
    // PREF instruction (ignored)
    (a1 = (uint64_t)(a1 << (32 + 0)));
    (t1 = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)20480 << 16))));
    (a1 = (uint64_t)(a1 | RC_U64X(t1)));
    WRITE32_B((uint32_t)a0, 28, 0u);
    WRITE64_B((uint32_t)a0, 0, a1);
    (a1 = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)32768 << 16))));
    (a1 = (uint64_t)(RC_U64X(a1) << 16));
    (a1 = (uint64_t)(a1 | (uint64_t)(uint16_t)65535));
    (a1 = (uint64_t)(a1 << 16));
    (a1 = (uint64_t)(a1 | (uint64_t)(uint16_t)65535));
    WRITE32_B((uint32_t)a0, 24, 0u);
    (v0 = (uint64_t)(RC_U64X(a0) << (32 + 0)));
    WRITE32_B((uint32_t)a0, 12, 0u);
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 4294967295))));
    (v1 = (uint64_t)(RC_U64X(v1) << (32 + 1)));
    (v1 = (uint64_t)(v1 >> 1));
    WRITE32_B((uint32_t)a0, 8, 0u);
    (v1 = (uint64_t)(v1 & v0));
    (t0 = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)8192 << 16))));
    (a2 = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)36863 << 16))));
    (a2 = (uint64_t)(RC_U64X(a2) | (uint64_t)(uint16_t)65535));
    (v0 = (uint64_t)(uint32_t)(READ32_B((uint32_t)a3, 32)));
    (v0 = (uint64_t)(v0 << (32 + 0)));
    (v0 = (uint64_t)(v0 | RC_U64X(t0)));
    WRITE64_B((uint32_t)a0, 16, v0);
    WRITE32_B((uint32_t)a3, 32, (uint32_t)a0);
    (v0 = (uint64_t)(READ64_B((uint32_t)a3, 0)));
    (v0 = (uint64_t)(v0 & a1));
    (v0 = (uint64_t)(v0 | v1));
    (v0 = (uint64_t)(v0 & RC_U64X(a2)));
    (v0 = (uint64_t)(v0 | RC_U64X(t1)));
    {
        const uint32_t target = (uint32_t)ra;
        WRITE64_B((uint32_t)a3, 0, v0);
        Ps2SetGpr64(ctx, 2, v0); Ps2SetGpr64(ctx, 3, v1); Ps2SetGpr64(ctx, 4, a0); Ps2SetGpr64(ctx, 5, a1); Ps2SetGpr64(ctx, 6, a2); Ps2SetGpr64(ctx, 7, a3); Ps2SetGpr64(ctx, 8, t0); Ps2SetGpr64(ctx, 9, t1);
        ctx->pc = target;
        return;
    }
}

// iosDlChainHeadCurrent (0x1bfdc0): lift_d1's text (the reference)
void iosDlChainHeadCurrent(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint64_t v0 = Ps2GprU64(ctx, 2), v1 = Ps2GprU64(ctx, 3), a0 = Ps2GprU64(ctx, 4), a1 = Ps2GprU64(ctx, 5), a2 = Ps2GprU64(ctx, 6), a3 = Ps2GprU64(ctx, 7), t0 = Ps2GprU64(ctx, 8), t1 = Ps2GprU64(ctx, 9), ra = Ps2GprU64(ctx, 31);
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)33 << 16))));
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v1, 13016))));
    (a2 = (uint64_t)((int64_t)(int32_t)((int32_t)READ32_B((uint32_t)v1, 28))));
    (a3 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v1, 8))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)READ32_B((uint32_t)v1, 24))));
    (a1 = (uint64_t)((int64_t)(int32_t)((int32_t)SLL32((uint32_t)a2, 1))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)SLL32((uint32_t)v0, 2))));
    (a1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)a1, (uint32_t)a2))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v0, (uint32_t)a3))));
    (a1 = (uint64_t)((int64_t)(int32_t)((int32_t)SLL32((uint32_t)a1, 4))));
    (a3 = (uint64_t)((int64_t)(int32_t)((int32_t)READ32_B((uint32_t)v0, 0))));
    (a2 = (uint64_t)((int64_t)(int32_t)((int32_t)READ32_B((uint32_t)v1, 16))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)READ32_B((uint32_t)v1, 16))));
    (a3 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)a3, (uint32_t)a1))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v0, 32))));
    WRITE32_B((uint32_t)v1, 16, (uint32_t)v0);
    // PREF instruction (ignored)
    (a0 = (uint64_t)(a0 << (32 + 0)));
    (t1 = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)20480 << 16))));
    (a0 = (uint64_t)(a0 | RC_U64X(t1)));
    WRITE32_B((uint32_t)a2, 28, 0u);
    WRITE64_B((uint32_t)a2, 0, a0);
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)32768 << 16))));
    (a0 = (uint64_t)(RC_U64X(a0) << 16));
    (a0 = (uint64_t)(a0 | (uint64_t)(uint16_t)65535));
    (a0 = (uint64_t)(a0 << 16));
    (a0 = (uint64_t)(a0 | (uint64_t)(uint16_t)65535));
    WRITE32_B((uint32_t)a2, 24, 0u);
    (v0 = (uint64_t)(RC_U64X(a2) << (32 + 0)));
    WRITE32_B((uint32_t)a2, 12, 0u);
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 4294967295))));
    (v1 = (uint64_t)(RC_U64X(v1) << (32 + 1)));
    (v1 = (uint64_t)(v1 >> 1));
    WRITE32_B((uint32_t)a2, 8, 0u);
    (v1 = (uint64_t)(v1 & v0));
    (t0 = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)8192 << 16))));
    (a1 = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)36863 << 16))));
    (a1 = (uint64_t)(RC_U64X(a1) | (uint64_t)(uint16_t)65535));
    (v0 = (uint64_t)(uint32_t)(READ32_B((uint32_t)a3, 32)));
    (v0 = (uint64_t)(v0 << (32 + 0)));
    (v0 = (uint64_t)(v0 | RC_U64X(t0)));
    WRITE64_B((uint32_t)a2, 16, v0);
    WRITE32_B((uint32_t)a3, 32, (uint32_t)a2);
    (v0 = (uint64_t)(READ64_B((uint32_t)a3, 0)));
    (v0 = (uint64_t)(v0 & a0));
    (v0 = (uint64_t)(v0 | v1));
    (v0 = (uint64_t)(v0 & RC_U64X(a1)));
    (v0 = (uint64_t)(v0 | RC_U64X(t1)));
    {
        const uint32_t target = (uint32_t)ra;
        WRITE64_B((uint32_t)a3, 0, v0);
        Ps2SetGpr64(ctx, 2, v0); Ps2SetGpr64(ctx, 3, v1); Ps2SetGpr64(ctx, 4, a0); Ps2SetGpr64(ctx, 5, a1); Ps2SetGpr64(ctx, 6, a2); Ps2SetGpr64(ctx, 7, a3); Ps2SetGpr64(ctx, 8, t0); Ps2SetGpr64(ctx, 9, t1);
        ctx->pc = target;
        return;
    }
}

// iosDlGetWorkBufId (0x1c0138): lift_d1's text (the reference)
void iosDlGetWorkBufId(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t v0 = Ps2GprU32(ctx, 2), v1 = Ps2GprU32(ctx, 3), ra = Ps2GprU32(ctx, 31);
    v1 = 0x210000u;
    v1 = v1 + 0x32D8u;
    v0 = READ32_B(v1, 24);
    {
        const uint32_t target = ra;
        Ps2SetGpr64(ctx, 2, sx64(v0)); Ps2SetGpr64(ctx, 3, sx64(v1));
        ctx->pc = target;
        return;
    }
}

// iosDlGetTransBufId (0x1c0150): lift_d1's text (the reference)
void iosDlGetTransBufId(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t v0 = Ps2GprU32(ctx, 2), v1 = Ps2GprU32(ctx, 3), ra = Ps2GprU32(ctx, 31);
    v1 = 0x210000u;
    v1 = v1 + 0x32D8u;
    v0 = READ32_B(v1, 24);
    {
        const uint32_t target = ra;
        v0 = v0 < 1u ? 1u : 0u;
        Ps2SetGpr64(ctx, 2, sx64(v0)); Ps2SetGpr64(ctx, 3, sx64(v1));
        ctx->pc = target;
        return;
    }
}

// iosDlGetEndAdrs (0x1c0168): lift_d1's text (the reference)
void iosDlGetEndAdrs(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t v0 = Ps2GprU32(ctx, 2), v1 = Ps2GprU32(ctx, 3), a0 = Ps2GprU32(ctx, 4), a1 = Ps2GprU32(ctx, 5), ra = Ps2GprU32(ctx, 31);
    v0 = 0x210000u;
    a1 = 0x1E0000u;
    v0 = v0 + 0x32D8u;
    a0 = READ32_B(a1, 4268);
    v1 = READ32_B(v0, 24);
    a0 = a0 << 4;
    v1 = v1 << 2;
    v1 = v1 + v0;
    v0 = READ32(v1);
    v0 = v0 + a0;
    {
        const uint32_t target = ra;
        v0 = v0 - 0x1000u;
        Ps2SetGpr64(ctx, 2, sx64(v0)); Ps2SetGpr64(ctx, 3, sx64(v1)); Ps2SetGpr64(ctx, 4, sx64(a0)); Ps2SetGpr64(ctx, 5, sx64(a1));
        ctx->pc = target;
        return;
    }
}

// iosDlGetLayerNum (0x1c02c8): lift_d1's text (the reference)
void iosDlGetLayerNum(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t v0 = Ps2GprU32(ctx, 2), v1 = Ps2GprU32(ctx, 3), ra = Ps2GprU32(ctx, 31);
    v1 = 0x210000u;
    v1 = v1 + 0x32D8u;
    v0 = READ32_B(v1, 20);
    {
        const uint32_t target = ra;
        Ps2SetGpr64(ctx, 2, sx64(v0)); Ps2SetGpr64(ctx, 3, sx64(v1));
        ctx->pc = target;
        return;
    }
}

// iosDlCheckAdrs (0x1c03b8): lift_d1's text (the reference)
void iosDlCheckAdrs(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t v0 = Ps2GprU32(ctx, 2), v1 = Ps2GprU32(ctx, 3), a1 = Ps2GprU32(ctx, 5), a2 = Ps2GprU32(ctx, 6), ra = Ps2GprU32(ctx, 31);
    uint64_t a0 = Ps2GprU64(ctx, 4);
    v0 = 0x210000u;
    a2 = 0x1E0000u;
    v0 = v0 + 0x32D8u;
    a1 = READ32_B(a2, 4268);
    v1 = READ32_B(v0, 24);
    a1 = a1 << 4;
    v1 = v1 << 2;
    v1 = v1 + v0;
    v0 = READ32(v1);
    v0 = v0 + a1;
    v0 = v0 - 0x1000u;
    {
        const uint32_t target = ra;
        (v0 = (uint32_t)(((uint64_t)a0 < (uint64_t)sx64(v0)) ? 1 : 0));
        Ps2SetGpr64(ctx, 2, sx64(v0)); Ps2SetGpr64(ctx, 3, sx64(v1)); Ps2SetGpr64(ctx, 5, sx64(a1)); Ps2SetGpr64(ctx, 6, sx64(a2));
        ctx->pc = target;
        return;
    }
}

// iosDlCheckSize (0x1c03f0): lift_d1's text (the reference)
void iosDlCheckSize(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t v0 = Ps2GprU32(ctx, 2), v1 = Ps2GprU32(ctx, 3), a0 = Ps2GprU32(ctx, 4), a1 = Ps2GprU32(ctx, 5), a2 = Ps2GprU32(ctx, 6), a3 = Ps2GprU32(ctx, 7), ra = Ps2GprU32(ctx, 31);
    a3 = 0x210000u;
    v1 = 0x1E0000u;
    a2 = a3 + 0x32D8u;
    a1 = READ32_B(v1, 4268);
    v0 = READ32_B(a2, 24);
    a1 = a1 << 4;
    v0 = v0 << 2;
    v0 = v0 + a2;
    v1 = READ32(v0);
    a0 = READ32_B(a2, 16);
    v1 = v1 + a1;
    a0 = a0 & 0xFu;
    {
        const bool c = a0 == 0;
        a1 = v1 - 0x1000u;
        if (!c) {
            v1 = READ32_B(a2, 16);
            v0 = READ32_B(a2, 16);
            v1 = v1 & 0xFu;
            v0 = v0 - v1;
            v0 = v0 + 0x10u;
            WRITE32_B(a2, 16, v0);
        }
    }
    v0 = READ32_B(a2, 16);
    // PREF instruction (ignored)
    v1 = a3 + 0x32D8u;
    v0 = READ32_B(v1, 16);
    {
        const uint32_t target = ra;
        v0 = a1 - v0;
        Ps2SetGpr64(ctx, 2, sx64(v0)); Ps2SetGpr64(ctx, 3, sx64(v1)); Ps2SetGpr64(ctx, 4, sx64(a0)); Ps2SetGpr64(ctx, 5, sx64(a1)); Ps2SetGpr64(ctx, 6, sx64(a2)); Ps2SetGpr64(ctx, 7, sx64(a3));
        ctx->pc = target;
        return;
    }
}

// iosAllocPacketPtrPath1UnCachedA (0x1c0480): lift_d1's text (the reference)
void iosAllocPacketPtrPath1UnCachedA(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t v0 = Ps2GprU32(ctx, 2), v1 = Ps2GprU32(ctx, 3), a0 = Ps2GprU32(ctx, 4), a1 = Ps2GprU32(ctx, 5), a2 = Ps2GprU32(ctx, 6), ra = Ps2GprU32(ctx, 31);
    a1 = 0x210000u;
    a2 = 0x30000000u;
    a1 = a1 + 0x32D8u;
    v0 = READ32_B(a1, 16);
    v1 = READ32_B(a1, 16);
    v0 = v0 | a2;
    v1 = v1 + a0;
    WRITE32_B(a1, 16, v1);
    {
        const uint32_t target = ra;
        Ps2SetGpr64(ctx, 2, sx64(v0)); Ps2SetGpr64(ctx, 3, sx64(v1)); Ps2SetGpr64(ctx, 5, sx64(a1)); Ps2SetGpr64(ctx, 6, sx64(a2));
        ctx->pc = target;
        return;
    }
}

// getDispScreenWidth (0x1c2018): lift_d1's text (the reference)
void getDispScreenWidth(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t v0 = Ps2GprU32(ctx, 2), v1 = Ps2GprU32(ctx, 3), ra = Ps2GprU32(ctx, 31);
    v1 = 0x210000u;
    {
        const uint32_t target = ra;
        v0 = READ32_B(v1, 14352);
        Ps2SetGpr64(ctx, 2, sx64(v0)); Ps2SetGpr64(ctx, 3, sx64(v1));
        ctx->pc = target;
        return;
    }
}

// getDispScreenHeight (0x1c2028): lift_d1's text (the reference)
void getDispScreenHeight(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t v0 = Ps2GprU32(ctx, 2), v1 = Ps2GprU32(ctx, 3), ra = Ps2GprU32(ctx, 31);
    v1 = 0x210000u;
    {
        const uint32_t target = ra;
        v0 = READ32_B(v1, 14356);
        Ps2SetGpr64(ctx, 2, sx64(v0)); Ps2SetGpr64(ctx, 3, sx64(v1));
        ctx->pc = target;
        return;
    }
}

// getDrawScreenWidth (0x1c2038): lift_d1's text (the reference)
void getDrawScreenWidth(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t v0 = Ps2GprU32(ctx, 2), v1 = Ps2GprU32(ctx, 3), ra = Ps2GprU32(ctx, 31);
    v1 = 0x210000u;
    {
        const uint32_t target = ra;
        v0 = READ32_B(v1, 14360);
        Ps2SetGpr64(ctx, 2, sx64(v0)); Ps2SetGpr64(ctx, 3, sx64(v1));
        ctx->pc = target;
        return;
    }
}

// getDrawScreenHeight (0x1c2048): lift_d1's text (the reference)
void getDrawScreenHeight(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t v0 = Ps2GprU32(ctx, 2), v1 = Ps2GprU32(ctx, 3), ra = Ps2GprU32(ctx, 31);
    v1 = 0x210000u;
    {
        const uint32_t target = ra;
        v0 = READ32_B(v1, 14364);
        Ps2SetGpr64(ctx, 2, sx64(v0)); Ps2SetGpr64(ctx, 3, sx64(v1));
        ctx->pc = target;
        return;
    }
}

// getDispBufferAddressTBP (0x1c20e8): lift_d1's text (the reference)
void getDispBufferAddressTBP(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t ra = Ps2GprU32(ctx, 31);
    uint64_t v0 = Ps2GprU64(ctx, 2);
    {
        const uint32_t target = ra;
        (v0 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL));
        Ps2SetGpr64(ctx, 2, v0);
        ctx->pc = target;
        return;
    }
}

// getDispBufferAddressFBP (0x1c20f0): lift_d1's text (the reference)
void getDispBufferAddressFBP(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t ra = Ps2GprU32(ctx, 31);
    uint64_t v0 = Ps2GprU64(ctx, 2);
    {
        const uint32_t target = ra;
        (v0 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL));
        Ps2SetGpr64(ctx, 2, v0);
        ctx->pc = target;
        return;
    }
}

// getZBufferAddressTBP (0x1c2158): lift_d1's text (the reference)
void getZBufferAddressTBP(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t v0 = Ps2GprU32(ctx, 2), v1 = Ps2GprU32(ctx, 3), a0 = Ps2GprU32(ctx, 4), ra = Ps2GprU32(ctx, 31);
    uint64_t hi = ctx->hi;
    uint64_t lo = ctx->lo;
    v1 = 0x210000u;
    v1 = v1 + 0x3810u;
    a0 = READ32_B(v1, 4);
    v0 = READ32(v1);
    { int64_t result = (int64_t)(int32_t)v0 * (int64_t)(int32_t)a0; lo = (uint64_t)(int64_t)(int32_t)result; hi = (uint64_t)(int64_t)(int32_t)(result >> 32); (v0 = (uint32_t)((int32_t)((int32_t)result))); }
    {
        const uint32_t target = ra;
        v0 = (uint32_t)((int32_t)v0 >> 6);
        Ps2SetGpr64(ctx, 2, sx64(v0)); Ps2SetGpr64(ctx, 3, sx64(v1)); Ps2SetGpr64(ctx, 4, sx64(a0)); ctx->hi = hi; ctx->lo = lo;
        ctx->pc = target;
        return;
    }
}

// getZBufferAddressFBP (0x1c2178): lift_d1's text (the reference)
void getZBufferAddressFBP(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t v0 = Ps2GprU32(ctx, 2), v1 = Ps2GprU32(ctx, 3), a0 = Ps2GprU32(ctx, 4), ra = Ps2GprU32(ctx, 31);
    uint64_t hi = ctx->hi;
    uint64_t lo = ctx->lo;
    v1 = 0x210000u;
    v1 = v1 + 0x3810u;
    a0 = READ32_B(v1, 4);
    v0 = READ32(v1);
    { int64_t result = (int64_t)(int32_t)v0 * (int64_t)(int32_t)a0; lo = (uint64_t)(int64_t)(int32_t)result; hi = (uint64_t)(int64_t)(int32_t)(result >> 32); (v0 = (uint32_t)((int32_t)((int32_t)result))); }
    {
        const uint32_t target = ra;
        v0 = (uint32_t)((int32_t)v0 >> 11);
        Ps2SetGpr64(ctx, 2, sx64(v0)); Ps2SetGpr64(ctx, 3, sx64(v1)); Ps2SetGpr64(ctx, 4, sx64(a0)); ctx->hi = hi; ctx->lo = lo;
        ctx->pc = target;
        return;
    }
}

// getScreenOffsetX (0x1c2208): lift_d1's text (the reference)
void getScreenOffsetX(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t v0 = Ps2GprU32(ctx, 2), ra = Ps2GprU32(ctx, 31);
    v0 = 0x210000u;
    {
        const uint32_t target = ra;
        v0 = v0 + 0x38BCu;
        Ps2SetGpr64(ctx, 2, sx64(v0));
        ctx->pc = target;
        return;
    }
}

// getScreenOffsetY (0x1c2218): lift_d1's text (the reference)
void getScreenOffsetY(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t v0 = Ps2GprU32(ctx, 2), ra = Ps2GprU32(ctx, 31);
    v0 = 0x210000u;
    {
        const uint32_t target = ra;
        v0 = v0 + 0x38C0u;
        Ps2SetGpr64(ctx, 2, sx64(v0));
        ctx->pc = target;
        return;
    }
}

// getAntiAlias (0x1c2228): lift_d1's text (the reference)
void getAntiAlias(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t v0 = Ps2GprU32(ctx, 2), ra = Ps2GprU32(ctx, 31);
    v0 = 0x210000u;
    {
        const uint32_t target = ra;
        v0 = v0 + 0x38B0u;
        Ps2SetGpr64(ctx, 2, sx64(v0));
        ctx->pc = target;
        return;
    }
}

// getProgressiveMode (0x1c2238): lift_d1's text (the reference)
void getProgressiveMode(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t v0 = Ps2GprU32(ctx, 2), ra = Ps2GprU32(ctx, 31);
    v0 = 0x210000u;
    {
        const uint32_t target = ra;
        v0 = v0 + 0x38B4u;
        Ps2SetGpr64(ctx, 2, sx64(v0));
        ctx->pc = target;
        return;
    }
}

// iosGetField (0x1c28b0): lift_d1's text (the reference)
void iosGetField(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint64_t v0 = Ps2GprU64(ctx, 2), v1 = Ps2GprU64(ctx, 3), ra = Ps2GprU64(ctx, 31);
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)4608 << 16))));
    (v1 = (uint64_t)(RC_U64X(v1) | (uint64_t)(uint16_t)4096));
    (v0 = (uint64_t)(READ64_B((uint32_t)v1, 0)));
    (v0 = (uint64_t)(v0 >> 13));
    (v0 = (uint64_t)(v0 & (uint64_t)(uint16_t)1));
    (v0 = (uint64_t)(RC_U64X(v0) << (32 + 0)));
    (v0 = (uint64_t)((int64_t)v0 >> (32 + 0)));
    {
        const uint32_t target = (uint32_t)ra;
        Ps2SetGpr64(ctx, 2, v0); Ps2SetGpr64(ctx, 3, v1);
        ctx->pc = target;
        return;
    }
}

// iosSetSyncNoReturnFunc (0x1c2a68): lift_d1's text (the reference)
void iosSetSyncNoReturnFunc(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t v0 = Ps2GprU32(ctx, 2), v1 = Ps2GprU32(ctx, 3), a0 = Ps2GprU32(ctx, 4), ra = Ps2GprU32(ctx, 31);
    v1 = 0x1E0000u;
    v0 = 1u;
    {
        const uint32_t target = ra;
        WRITE32_B(v1, 4384, a0);
        Ps2SetGpr64(ctx, 2, sx64(v0)); Ps2SetGpr64(ctx, 3, sx64(v1));
        ctx->pc = target;
        return;
    }
}

// iosWaitDMA (0x1c2b98): lift_d1's text (the reference)
void iosWaitDMA(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t ra = Ps2GprU32(ctx, 31);
    uint64_t v0 = Ps2GprU64(ctx, 2);
    {
        const uint32_t target = ra;
        (v0 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL));
        Ps2SetGpr64(ctx, 2, v0);
        ctx->pc = target;
        return;
    }
}

// getCRTCoffsetU (0x1c2be8): lift_d1's text (the reference)
void getCRTCoffsetU(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t v0 = Ps2GprU32(ctx, 2), ra = Ps2GprU32(ctx, 31);
    v0 = 0x1E0000u;
    {
        const uint32_t target = ra;
        v0 = v0 + 0x1148u;
        Ps2SetGpr64(ctx, 2, sx64(v0));
        ctx->pc = target;
        return;
    }
}

// getCRTCoffsetV (0x1c2bf8): lift_d1's text (the reference)
void getCRTCoffsetV(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t v0 = Ps2GprU32(ctx, 2), ra = Ps2GprU32(ctx, 31);
    v0 = 0x1E0000u;
    {
        const uint32_t target = ra;
        v0 = v0 + 0x114Cu;
        Ps2SetGpr64(ctx, 2, sx64(v0));
        ctx->pc = target;
        return;
    }
}

// iosSetIStartFunc (0x1c3720): lift_d1's text (the reference)
void iosSetIStartFunc(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t v0 = Ps2GprU32(ctx, 2), v1 = Ps2GprU32(ctx, 3), a0 = Ps2GprU32(ctx, 4), ra = Ps2GprU32(ctx, 31);
    v1 = 0x1E0000u;
    v0 = 1u;
    {
        const uint32_t target = ra;
        WRITE32_B(v1, 4452, a0);
        Ps2SetGpr64(ctx, 2, sx64(v0)); Ps2SetGpr64(ctx, 3, sx64(v1));
        ctx->pc = target;
        return;
    }
}

// iosSetGpFinishFunc (0x1c3730): lift_d1's text (the reference)
void iosSetGpFinishFunc(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t v0 = Ps2GprU32(ctx, 2), v1 = Ps2GprU32(ctx, 3), a0 = Ps2GprU32(ctx, 4), ra = Ps2GprU32(ctx, 31);
    v1 = 0x1E0000u;
    v0 = 1u;
    {
        const uint32_t target = ra;
        WRITE32_B(v1, 4460, a0);
        Ps2SetGpr64(ctx, 2, sx64(v0)); Ps2SetGpr64(ctx, 3, sx64(v1));
        ctx->pc = target;
        return;
    }
}

// iosSetIFinishFunc (0x1c3740): lift_d1's text (the reference)
void iosSetIFinishFunc(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t v0 = Ps2GprU32(ctx, 2), v1 = Ps2GprU32(ctx, 3), a0 = Ps2GprU32(ctx, 4), ra = Ps2GprU32(ctx, 31);
    v1 = 0x1E0000u;
    v0 = 1u;
    {
        const uint32_t target = ra;
        WRITE32_B(v1, 4456, a0);
        Ps2SetGpr64(ctx, 2, sx64(v0)); Ps2SetGpr64(ctx, 3, sx64(v1));
        ctx->pc = target;
        return;
    }
}

// getIntrGpCount (0x1c3750): lift_d1's text (the reference)
void getIntrGpCount(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t v1 = Ps2GprU32(ctx, 3), ra = Ps2GprU32(ctx, 31);
    uint64_t v0 = Ps2GprU64(ctx, 2);
    v1 = 0x1E0000u;
    (v1 = (uint32_t)((int32_t)((int32_t)ADD32(v1, 4440))));
    (v0 = (uint64_t)(READ64_B(v1, 0)));
    {
        const uint32_t target = ra;
        Ps2SetGpr64(ctx, 2, v0); Ps2SetGpr64(ctx, 3, sx64(v1));
        ctx->pc = target;
        return;
    }
}

// iosSetDlTransFunc (0x1c3768): lift_d1's text (the reference)
void iosSetDlTransFunc(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t v0 = Ps2GprU32(ctx, 2), v1 = Ps2GprU32(ctx, 3), a0 = Ps2GprU32(ctx, 4), ra = Ps2GprU32(ctx, 31);
    v1 = 0x1E0000u;
    v0 = 1u;
    {
        const uint32_t target = ra;
        WRITE32_B(v1, 4464, a0);
        Ps2SetGpr64(ctx, 2, sx64(v0)); Ps2SetGpr64(ctx, 3, sx64(v1));
        ctx->pc = target;
        return;
    }
}

// iosSetVSchedulerFunc (0x1c3778): lift_d1's text (the reference)
void iosSetVSchedulerFunc(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t v0 = Ps2GprU32(ctx, 2), v1 = Ps2GprU32(ctx, 3), a0 = Ps2GprU32(ctx, 4), ra = Ps2GprU32(ctx, 31);
    v1 = 0x1E0000u;
    v0 = 1u;
    {
        const uint32_t target = ra;
        WRITE32_B(v1, 4468, a0);
        Ps2SetGpr64(ctx, 2, sx64(v0)); Ps2SetGpr64(ctx, 3, sx64(v1));
        ctx->pc = target;
        return;
    }
}

// getSpriteDrawFirst (0x1c5b48): lift_d1's text (the reference)
void getSpriteDrawFirst(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t v0 = Ps2GprU32(ctx, 2), ra = Ps2GprU32(ctx, 31);
    v0 = 0x1E0000u;
    {
        const uint32_t target = ra;
        v0 = v0 + 0x11A8u;
        Ps2SetGpr64(ctx, 2, sx64(v0));
        ctx->pc = target;
        return;
    }
}

// gsReglist (0x1c5b58): lift_d1's text (the reference)
void gsReglist(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t ra = Ps2GprU32(ctx, 31);
    {
        const uint32_t target = ra;
        
        ctx->pc = target;
        return;
    }
}

// isysSetObjImmediateDeleteFunc (0x1c7540): lift_d1's text (the reference)
void isysSetObjImmediateDeleteFunc(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t a0 = Ps2GprU32(ctx, 4), a1 = Ps2GprU32(ctx, 5), ra = Ps2GprU32(ctx, 31);
    {
        const uint32_t target = ra;
        WRITE32_B(a0, 24, a1);
        
        ctx->pc = target;
        return;
    }
}

// isysInitGroup (0x1c7658): lift_d1's text (the reference)
void isysInitGroup(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t a0 = Ps2GprU32(ctx, 4), ra = Ps2GprU32(ctx, 31);
    WRITE16(a0, (uint16_t)0u);
    {
        const uint32_t target = ra;
        WRITE16_B(a0, 2, (uint16_t)0u);
        
        ctx->pc = target;
        return;
    }
}

// isysObjDeleteLock (0x1c7680): lift_d1's text (the reference)
void isysObjDeleteLock(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t v0 = Ps2GprU32(ctx, 2), v1 = Ps2GprU32(ctx, 3), a0 = Ps2GprU32(ctx, 4), a1 = Ps2GprU32(ctx, 5), ra = Ps2GprU32(ctx, 31);
    a1 = READ32_B(a0, 8);
    v1 = 0x10000u;
    v1 = v1 | 0xFFFFu;
    v0 = a1 >> 17;
    a1 = a1 & v1;
    v0 = v0 + 1u;
    v0 = v0 << 17;
    a1 = a1 | v0;
    {
        const uint32_t target = ra;
        WRITE32_B(a0, 8, a1);
        Ps2SetGpr64(ctx, 2, sx64(v0)); Ps2SetGpr64(ctx, 3, sx64(v1)); Ps2SetGpr64(ctx, 5, sx64(a1));
        ctx->pc = target;
        return;
    }
}

// isysGetLinkMax (0x1c79a8): lift_d1's text (the reference)
void isysGetLinkMax(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t v0 = Ps2GprU32(ctx, 2), ra = Ps2GprU32(ctx, 31);
    {
        const uint32_t target = ra;
        v0 = 0x36B0u;
        Ps2SetGpr64(ctx, 2, sx64(v0));
        ctx->pc = target;
        return;
    }
}

// isysGetObjIdentifierByObj (0x1c79b0): lift_d1's text (the reference)
void isysGetObjIdentifierByObj(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t v0 = Ps2GprU32(ctx, 2), v1 = Ps2GprU32(ctx, 3), a0 = Ps2GprU32(ctx, 4), ra = Ps2GprU32(ctx, 31);
    v0 = READ8_B(a0, 8);
    v1 = v0 << 2;
    v0 = (int32_t)v0 < 37 ? 1u : 0u;
    if (v0 == 0) {
        v0 = 0x1E0000u;
    } else {
        v0 = 0x1E0000u;
        v0 = v0 + v1;
        v0 = READ32_B(v0, 4576);
        {
            const uint32_t target = ra;
            Ps2SetGpr64(ctx, 2, sx64(v0)); Ps2SetGpr64(ctx, 3, sx64(v1));
            ctx->pc = target;
            return;
        }
    }
    {
        const uint32_t target = ra;
        v0 = READ32_B(v0, 4576);
        Ps2SetGpr64(ctx, 2, sx64(v0)); Ps2SetGpr64(ctx, 3, sx64(v1));
        ctx->pc = target;
        return;
    }
}

// iosGetISchedulerExecCnt (0x1c7fa0): lift_d1's text (the reference)
void iosGetISchedulerExecCnt(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t v0 = Ps2GprU32(ctx, 2), v1 = Ps2GprU32(ctx, 3), ra = Ps2GprU32(ctx, 31);
    v1 = 0x1E0000u;
    {
        const uint32_t target = ra;
        v0 = READ32_B(v1, 4904);
        Ps2SetGpr64(ctx, 2, sx64(v0)); Ps2SetGpr64(ctx, 3, sx64(v1));
        ctx->pc = target;
        return;
    }
}

// ischedRestart (0x1c7ff0): lift_d1's text (the reference)
void ischedRestart(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t v0 = Ps2GprU32(ctx, 2), v1 = Ps2GprU32(ctx, 3), a0 = Ps2GprU32(ctx, 4), ra = Ps2GprU32(ctx, 31);
    v0 = 0x270000u;
    v0 = v0 - 0x2660u;
    v1 = READ32(v0);
    v1 = v1 - 1u;
    WRITE32(v0, v1);
    a0 = READ32(v0);
    {
        const bool c = a0 != 0;
        v1 = 0x10000000u;
        if (!c) {
            v1 = v1 | 0x10u;
            v0 = READ32(v1);
            v0 = v0 | 0x80u;
            WRITE32(v1, v0);
        }
    }
    {
        const uint32_t target = ra;
        Ps2SetGpr64(ctx, 2, sx64(v0)); Ps2SetGpr64(ctx, 3, sx64(v1)); Ps2SetGpr64(ctx, 4, sx64(a0));
        ctx->pc = target;
        return;
    }
}

// iosSetISchedulerSubthreadTask (0x1c8028): lift_d1's text (the reference)
void iosSetISchedulerSubthreadTask(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t v0 = Ps2GprU32(ctx, 2), a0 = Ps2GprU32(ctx, 4), ra = Ps2GprU32(ctx, 31);
    v0 = 0x1E0000u;
    {
        const uint32_t target = ra;
        WRITE32_B(v0, 4916, a0);
        Ps2SetGpr64(ctx, 2, sx64(v0));
        ctx->pc = target;
        return;
    }
}

// iosSetIVSyncMode (0x1c8038): lift_d1's text (the reference)
void iosSetIVSyncMode(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t v0 = Ps2GprU32(ctx, 2), a0 = Ps2GprU32(ctx, 4), ra = Ps2GprU32(ctx, 31);
    v0 = 0x1E0000u;
    {
        const uint32_t target = ra;
        WRITE32_B(v0, 4896, a0);
        Ps2SetGpr64(ctx, 2, sx64(v0));
        ctx->pc = target;
        return;
    }
}

// iosGetIVSyncMode (0x1c8048): lift_d1's text (the reference)
void iosGetIVSyncMode(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t v0 = Ps2GprU32(ctx, 2), v1 = Ps2GprU32(ctx, 3), ra = Ps2GprU32(ctx, 31);
    v1 = 0x1E0000u;
    {
        const uint32_t target = ra;
        v0 = READ32_B(v1, 4896);
        Ps2SetGpr64(ctx, 2, sx64(v0)); Ps2SetGpr64(ctx, 3, sx64(v1));
        ctx->pc = target;
        return;
    }
}

// vschedSetNextFrameForceGarbage (0x1c8058): lift_d1's text (the reference)
void vschedSetNextFrameForceGarbage(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t v0 = Ps2GprU32(ctx, 2), v1 = Ps2GprU32(ctx, 3), ra = Ps2GprU32(ctx, 31);
    v1 = 0x1E0000u;
    v0 = 1u;
    {
        const uint32_t target = ra;
        WRITE32_B(v1, 4892, v0);
        Ps2SetGpr64(ctx, 2, sx64(v0)); Ps2SetGpr64(ctx, 3, sx64(v1));
        ctx->pc = target;
        return;
    }
}

// GetGarbageTime (0x1c8068): lift_d1's text (the reference)
void GetGarbageTime(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t v1 = Ps2GprU32(ctx, 3), ra = Ps2GprU32(ctx, 31);
    uint64_t v0 = Ps2GprU64(ctx, 2);
    v1 = 0x270000u;
    {
        const uint32_t target = ra;
        (v0 = (uint64_t)(READ64_B(v1, -9832)));
        Ps2SetGpr64(ctx, 2, v0); Ps2SetGpr64(ctx, 3, sx64(v1));
        ctx->pc = target;
        return;
    }
}

// init_letter (0x1c84c0): lift_d1's text (the reference)
void init_letter(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t v0 = Ps2GprU32(ctx, 2), a0 = Ps2GprU32(ctx, 4), a1 = Ps2GprU32(ctx, 5), a2 = Ps2GprU32(ctx, 6), ra = Ps2GprU32(ctx, 31);
    v0 = 1u;
    WRITE32(a1, 0u);
    WRITE32(a2, 0u);
    {
        const uint32_t target = ra;
        WRITE32_B(a0, 32, v0);
        Ps2SetGpr64(ctx, 2, sx64(v0));
        ctx->pc = target;
        return;
    }
}

// is_hope_letter_type (0x1c84d8): lift_d1's text (the reference)
void is_hope_letter_type(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t a0 = Ps2GprU32(ctx, 4), ra = Ps2GprU32(ctx, 31);
    uint64_t v0 = Ps2GprU64(ctx, 2), a1 = Ps2GprU64(ctx, 5);
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)READ32_B(a0, 12))));
    (v0 = (uint64_t)(RC_U64X(v0) ^ a1));
    {
        const uint32_t target = ra;
        (v0 = (uint64_t)(((uint64_t)v0 < (uint64_t)(int64_t)(int32_t)1) ? 1 : 0));
        Ps2SetGpr64(ctx, 2, v0);
        ctx->pc = target;
        return;
    }
}

// get_letter_contents_data (0x1c84e8): lift_d1's text (the reference)
void get_letter_contents_data(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t v0 = Ps2GprU32(ctx, 2), a0 = Ps2GprU32(ctx, 4), ra = Ps2GprU32(ctx, 31);
    {
        const uint32_t target = ra;
        v0 = READ32_B(a0, 28);
        Ps2SetGpr64(ctx, 2, sx64(v0));
        ctx->pc = target;
        return;
    }
}

// increment_letter_refcount (0x1c8500): lift_d1's text (the reference)
void increment_letter_refcount(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t v0 = Ps2GprU32(ctx, 2), a0 = Ps2GprU32(ctx, 4), ra = Ps2GprU32(ctx, 31);
    v0 = READ32(a0);
    v0 = v0 + 1u;
    {
        const uint32_t target = ra;
        WRITE32(a0, v0);
        Ps2SetGpr64(ctx, 2, sx64(v0));
        ctx->pc = target;
        return;
    }
}

// is_letter_reach_time (0x1c8570): lift_d1's text (the reference)
void is_letter_reach_time(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t v0 = Ps2GprU32(ctx, 2), a0 = Ps2GprU32(ctx, 4), ra = Ps2GprU32(ctx, 31);
    float f0 = ctx->f[0], f1 = ctx->f[1], f12 = ctx->f[12];
    uint32_t fcr31 = ctx->fcr31;
    f1 = lift_d1::flt(READ32_B(a0, 8));
    f0 = lift_d1::flt(READ32_B(a0, 4));
    fcr31 = (FPU_C_OLE_S(f0, f1)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
    {
        const bool c = (fcr31 & 0x800000u) != 0u;
        v0 = 1u;
        if (!c) {
            f0 = FPU_ADD_S(f1, f12);
            v0 = 0u;
            WRITE32_B(a0, 8, lift_d1::bits(f0));
        }
    }
    {
        const uint32_t target = ra;
        ctx->f[0] = f0; ctx->f[1] = f1; Ps2SetGpr64(ctx, 2, sx64(v0)); ctx->fcr31 = fcr31;
        ctx->pc = target;
        return;
    }
}

// is_init_letter_called (0x1c8598): lift_d1's text (the reference)
void is_init_letter_called(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t v0 = Ps2GprU32(ctx, 2), a0 = Ps2GprU32(ctx, 4), ra = Ps2GprU32(ctx, 31);
    {
        const uint32_t target = ra;
        v0 = READ32_B(a0, 32);
        Ps2SetGpr64(ctx, 2, sx64(v0));
        ctx->pc = target;
        return;
    }
}

// get_reference_letter_type (0x1c85a0): lift_d1's text (the reference)
void get_reference_letter_type(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t v0 = Ps2GprU32(ctx, 2), a0 = Ps2GprU32(ctx, 4), ra = Ps2GprU32(ctx, 31);
    {
        const uint32_t target = ra;
        v0 = READ32_B(a0, 16);
        Ps2SetGpr64(ctx, 2, sx64(v0));
        ctx->pc = target;
        return;
    }
}

// get_letter_receiver_obj_group (0x1c85a8): lift_d1's text (the reference)
void get_letter_receiver_obj_group(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t v0 = Ps2GprU32(ctx, 2), a0 = Ps2GprU32(ctx, 4), ra = Ps2GprU32(ctx, 31);
    {
        const uint32_t target = ra;
        v0 = READ32_B(a0, 20);
        Ps2SetGpr64(ctx, 2, sx64(v0));
        ctx->pc = target;
        return;
    }
}

// get_letter_receiver_data (0x1c85b0): lift_d1's text (the reference)
void get_letter_receiver_data(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t v0 = Ps2GprU32(ctx, 2), a0 = Ps2GprU32(ctx, 4), ra = Ps2GprU32(ctx, 31);
    {
        const uint32_t target = ra;
        v0 = READ32_B(a0, 24);
        Ps2SetGpr64(ctx, 2, sx64(v0));
        ctx->pc = target;
        return;
    }
}

// get_total_letter_in_post (0x1c88f8): lift_d1's text (the reference)
void get_total_letter_in_post(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t v0 = Ps2GprU32(ctx, 2), a0 = Ps2GprU32(ctx, 4), ra = Ps2GprU32(ctx, 31);
    {
        const uint32_t target = ra;
        v0 = READ32(a0);
        Ps2SetGpr64(ctx, 2, sx64(v0));
        ctx->pc = target;
        return;
    }
}

// kernel_sub_001C8948 (0x1c8948): lift_d1's text (the reference)
void kernel_sub_001C8948(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t a0 = Ps2GprU32(ctx, 4), ra = Ps2GprU32(ctx, 31);
    WRITE32_B(a0, 16, 0u);
    WRITE32(a0, 0u);
    WRITE32_B(a0, 4, 0u);
    WRITE32_B(a0, 8, 0u);
    {
        const uint32_t target = ra;
        WRITE32_B(a0, 12, 0u);
        
        ctx->pc = target;
        return;
    }
}

// constructor_letter_type (0x1c8ad0): lift_d1's text (the reference)
void constructor_letter_type(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t v0 = Ps2GprU32(ctx, 2), a0 = Ps2GprU32(ctx, 4), ra = Ps2GprU32(ctx, 31);
    v0 = 2u;
    WRITE32_B(a0, 20, 0u);
    WRITE32(a0, v0);
    WRITE32_B(a0, 4, 0u);
    WRITE32_B(a0, 8, 0u);
    WRITE32_B(a0, 12, 0u);
    {
        const uint32_t target = ra;
        WRITE32_B(a0, 16, 0u);
        Ps2SetGpr64(ctx, 2, sx64(v0));
        ctx->pc = target;
        return;
    }
}

// get_letter_type (0x1c8b68): lift_d1's text (the reference)
void get_letter_type(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t at = Ps2GprU32(ctx, 1), v0 = Ps2GprU32(ctx, 2), ra = Ps2GprU32(ctx, 31);
    uint64_t a0 = Ps2GprU64(ctx, 4);
    (v0 = (uint32_t)((int32_t)((int32_t)SLL32((uint32_t)a0, 2))));
    (a0 = (uint64_t)(((uint64_t)a0 < (uint64_t)(int64_t)(int32_t)20) ? 1 : 0));
    if ((int64_t)a0 != 0) {
        at = 0x270000u;
        at = at + v0;
        v0 = READ32_B(at, 2584);
        {
            const uint32_t target = ra;
            Ps2SetGpr64(ctx, 1, sx64(at)); Ps2SetGpr64(ctx, 2, sx64(v0)); Ps2SetGpr64(ctx, 4, a0);
            ctx->pc = target;
            return;
        }
    }
    {
        const uint32_t target = ra;
        v0 = 0u;
        Ps2SetGpr64(ctx, 2, sx64(v0)); Ps2SetGpr64(ctx, 4, a0);
        ctx->pc = target;
        return;
    }
}

// iosGetDataSizeOfFont (0x1c90d0): lift_d1's text (the reference)
void iosGetDataSizeOfFont(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t v0 = Ps2GprU32(ctx, 2), a0 = Ps2GprU32(ctx, 4), ra = Ps2GprU32(ctx, 31);
    a0 = a0 << 2;
    v0 = 0x1E0000u;
    v0 = v0 + a0;
    v0 = READ32_B(v0, 4936);
    {
        const uint32_t target = ra;
        Ps2SetGpr64(ctx, 2, sx64(v0)); Ps2SetGpr64(ctx, 4, sx64(a0));
        ctx->pc = target;
        return;
    }
}

// iosGetFontDataWithJIS (0x1c9160): lift_d1's text (the reference)
void iosGetFontDataWithJIS(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint64_t v0 = Ps2GprU64(ctx, 2), v1 = Ps2GprU64(ctx, 3), a0 = Ps2GprU64(ctx, 4), a1 = Ps2GprU64(ctx, 5), a2 = Ps2GprU64(ctx, 6), a3 = Ps2GprU64(ctx, 7), t0 = Ps2GprU64(ctx, 8), ra = Ps2GprU64(ctx, 31);
    uint64_t hi = ctx->hi;
    uint64_t lo = ctx->lo;
    (v1 = (uint64_t)((int64_t)(int32_t)(SRA32((int32_t)a1, 8))));
    (a1 = (uint64_t)(a1 & (uint64_t)(uint16_t)255));
    (a2 = (uint64_t)((int64_t)(int32_t)((int32_t)SLL32((uint32_t)a2, 5))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)SLL32((uint32_t)v1, 1))));
    (t0 = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)39 << 16))));
    (t0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)t0, 2680))));
    (t0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)t0, (uint32_t)a2))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v0, (uint32_t)v1))));
    (a2 = (uint64_t)((int64_t)(int32_t)((int32_t)READ32_B((uint32_t)t0, 16))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)SLL32((uint32_t)v0, 4))));
    (a3 = (uint64_t)((int64_t)(int32_t)((int32_t)READ32_B((uint32_t)t0, 20))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)SUB32((uint32_t)v0, (uint32_t)v1))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)SLL32((uint32_t)v0, 1))));
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)READ32_B((uint32_t)t0, 8))));
    { int64_t result = (int64_t)(int32_t)a2 * (int64_t)(int32_t)a3; lo = (uint64_t)(int64_t)(int32_t)result; hi = (uint64_t)(int64_t)(int32_t)(result >> 32); (a2 = (uint64_t)((int64_t)(int32_t)((int32_t)result))); }
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v0, (uint32_t)a1))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v0, 4294964161))));
    { int64_t result = (int64_t)(int32_t)v0 * (int64_t)(int32_t)a2; lo = (uint64_t)(int64_t)(int32_t)result; hi = (uint64_t)(int64_t)(int32_t)(result >> 32); (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)result))); }
    {
        const bool c = (int64_t)a0 == 0;
        (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v1, (uint32_t)v0))));
        if (!c) {
            { uint32_t addr = ADD32((uint32_t)t0, 7); uint32_t aligned_addr = addr & ~7u; uint32_t offset = addr & 7u; uint64_t mem = READ64(aligned_addr); uint32_t shift = (7u - offset) << 3; uint64_t keepMask = (shift == 0) ? 0ull : ((1ull << shift) - 1ull); (v1 = (uint64_t)((RC_U64X(v1) & keepMask) | (mem << shift))); }
            { uint32_t addr = ADD32((uint32_t)t0, 0); uint32_t aligned_addr = addr & ~7u; uint32_t offset = addr & 7u; uint64_t mem = READ64(aligned_addr); uint32_t shift = offset << 3; uint64_t keepMask = (offset == 0) ? 0ull : (0xFFFFFFFFFFFFFFFFull << ((8u - offset) << 3)); (v1 = (uint64_t)((v1 & keepMask) | (mem >> shift))); }
            { uint32_t addr = ADD32((uint32_t)t0, 15); uint32_t aligned_addr = addr & ~7u; uint32_t offset = addr & 7u; uint64_t mem = READ64(aligned_addr); uint32_t shift = (7u - offset) << 3; uint64_t keepMask = (shift == 0) ? 0ull : ((1ull << shift) - 1ull); (a1 = (uint64_t)((RC_U64X(a1) & keepMask) | (mem << shift))); }
            { uint32_t addr = ADD32((uint32_t)t0, 8); uint32_t aligned_addr = addr & ~7u; uint32_t offset = addr & 7u; uint64_t mem = READ64(aligned_addr); uint32_t shift = offset << 3; uint64_t keepMask = (offset == 0) ? 0ull : (0xFFFFFFFFFFFFFFFFull << ((8u - offset) << 3)); (a1 = (uint64_t)((a1 & keepMask) | (mem >> shift))); }
            { uint32_t addr = ADD32((uint32_t)t0, 23); uint32_t aligned_addr = addr & ~7u; uint32_t offset = addr & 7u; uint64_t mem = READ64(aligned_addr); uint32_t shift = (7u - offset) << 3; uint64_t keepMask = (shift == 0) ? 0ull : ((1ull << shift) - 1ull); (a2 = (uint64_t)((RC_U64X(a2) & keepMask) | (mem << shift))); }
            { uint32_t addr = ADD32((uint32_t)t0, 16); uint32_t aligned_addr = addr & ~7u; uint32_t offset = addr & 7u; uint64_t mem = READ64(aligned_addr); uint32_t shift = offset << 3; uint64_t keepMask = (offset == 0) ? 0ull : (0xFFFFFFFFFFFFFFFFull << ((8u - offset) << 3)); (a2 = (uint64_t)((a2 & keepMask) | (mem >> shift))); }
            { uint32_t addr = ADD32((uint32_t)t0, 31); uint32_t aligned_addr = addr & ~7u; uint32_t offset = addr & 7u; uint64_t mem = READ64(aligned_addr); uint32_t shift = (7u - offset) << 3; uint64_t keepMask = (shift == 0) ? 0ull : ((1ull << shift) - 1ull); (a3 = (uint64_t)((RC_U64X(a3) & keepMask) | (mem << shift))); }
            { uint32_t addr = ADD32((uint32_t)t0, 24); uint32_t aligned_addr = addr & ~7u; uint32_t offset = addr & 7u; uint64_t mem = READ64(aligned_addr); uint32_t shift = offset << 3; uint64_t keepMask = (offset == 0) ? 0ull : (0xFFFFFFFFFFFFFFFFull << ((8u - offset) << 3)); (a3 = (uint64_t)((a3 & keepMask) | (mem >> shift))); }
            { uint32_t addr = ADD32((uint32_t)a0, 7); uint32_t aligned_addr = addr & ~7u; uint32_t offset = addr & 7u; uint32_t shift = (7u - offset) << 3; uint64_t mask = 0xFFFFFFFFFFFFFFFFull >> shift; uint64_t old_data = READ64(aligned_addr); uint64_t val = v1; uint64_t new_data = (old_data & ~mask) | ((val >> shift) & mask); WRITE64(aligned_addr, new_data); }
            { uint32_t addr = ADD32((uint32_t)a0, 0); uint32_t aligned_addr = addr & ~7u; uint32_t offset = addr & 7u; uint32_t shift = offset << 3; uint64_t mask = 0xFFFFFFFFFFFFFFFFull << shift; uint64_t old_data = READ64(aligned_addr); uint64_t val = v1; uint64_t new_data = (old_data & ~mask) | ((val << shift) & mask); WRITE64(aligned_addr, new_data); }
            { uint32_t addr = ADD32((uint32_t)a0, 15); uint32_t aligned_addr = addr & ~7u; uint32_t offset = addr & 7u; uint32_t shift = (7u - offset) << 3; uint64_t mask = 0xFFFFFFFFFFFFFFFFull >> shift; uint64_t old_data = READ64(aligned_addr); uint64_t val = a1; uint64_t new_data = (old_data & ~mask) | ((val >> shift) & mask); WRITE64(aligned_addr, new_data); }
            { uint32_t addr = ADD32((uint32_t)a0, 8); uint32_t aligned_addr = addr & ~7u; uint32_t offset = addr & 7u; uint32_t shift = offset << 3; uint64_t mask = 0xFFFFFFFFFFFFFFFFull << shift; uint64_t old_data = READ64(aligned_addr); uint64_t val = a1; uint64_t new_data = (old_data & ~mask) | ((val << shift) & mask); WRITE64(aligned_addr, new_data); }
            { uint32_t addr = ADD32((uint32_t)a0, 23); uint32_t aligned_addr = addr & ~7u; uint32_t offset = addr & 7u; uint32_t shift = (7u - offset) << 3; uint64_t mask = 0xFFFFFFFFFFFFFFFFull >> shift; uint64_t old_data = READ64(aligned_addr); uint64_t val = a2; uint64_t new_data = (old_data & ~mask) | ((val >> shift) & mask); WRITE64(aligned_addr, new_data); }
            { uint32_t addr = ADD32((uint32_t)a0, 16); uint32_t aligned_addr = addr & ~7u; uint32_t offset = addr & 7u; uint32_t shift = offset << 3; uint64_t mask = 0xFFFFFFFFFFFFFFFFull << shift; uint64_t old_data = READ64(aligned_addr); uint64_t val = a2; uint64_t new_data = (old_data & ~mask) | ((val << shift) & mask); WRITE64(aligned_addr, new_data); }
            { uint32_t addr = ADD32((uint32_t)a0, 31); uint32_t aligned_addr = addr & ~7u; uint32_t offset = addr & 7u; uint32_t shift = (7u - offset) << 3; uint64_t mask = 0xFFFFFFFFFFFFFFFFull >> shift; uint64_t old_data = READ64(aligned_addr); uint64_t val = a3; uint64_t new_data = (old_data & ~mask) | ((val >> shift) & mask); WRITE64(aligned_addr, new_data); }
            { uint32_t addr = ADD32((uint32_t)a0, 24); uint32_t aligned_addr = addr & ~7u; uint32_t offset = addr & 7u; uint32_t shift = offset << 3; uint64_t mask = 0xFFFFFFFFFFFFFFFFull << shift; uint64_t old_data = READ64(aligned_addr); uint64_t val = a3; uint64_t new_data = (old_data & ~mask) | ((val << shift) & mask); WRITE64(aligned_addr, new_data); }
        }
    }
    {
        const uint32_t target = (uint32_t)ra;
        Ps2SetGpr64(ctx, 2, v0); Ps2SetGpr64(ctx, 3, v1); Ps2SetGpr64(ctx, 5, a1); Ps2SetGpr64(ctx, 6, a2); Ps2SetGpr64(ctx, 7, a3); Ps2SetGpr64(ctx, 8, t0); ctx->hi = hi; ctx->lo = lo;
        ctx->pc = target;
        return;
    }
}

// iosGetFontIDWithJIS (0x1c9470): lift_d1's text (the reference)
void iosGetFontIDWithJIS(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t v0 = Ps2GprU32(ctx, 2), v1 = Ps2GprU32(ctx, 3), a0 = Ps2GprU32(ctx, 4), ra = Ps2GprU32(ctx, 31);
    v1 = (uint32_t)((int32_t)a0 >> 8);
    a0 = a0 & 0xFFu;
    v0 = v1 << 1;
    v0 = v0 + v1;
    v0 = v0 << 4;
    v0 = v0 - v1;
    v0 = v0 << 1;
    v0 = v0 + a0;
    {
        const uint32_t target = ra;
        v0 = v0 - 0xC3Fu;
        Ps2SetGpr64(ctx, 2, sx64(v0)); Ps2SetGpr64(ctx, 3, sx64(v1)); Ps2SetGpr64(ctx, 4, sx64(a0));
        ctx->pc = target;
        return;
    }
}

// iosEntryLocaleFontNoLoad (0x1c9620): lift_d1's text (the reference)
void iosEntryLocaleFontNoLoad(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint64_t v0 = Ps2GprU64(ctx, 2), v1 = Ps2GprU64(ctx, 3), a0 = Ps2GprU64(ctx, 4), a1 = Ps2GprU64(ctx, 5), a2 = Ps2GprU64(ctx, 6), a3 = Ps2GprU64(ctx, 7), t0 = Ps2GprU64(ctx, 8), t1 = Ps2GprU64(ctx, 9), ra = Ps2GprU64(ctx, 31);
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)39 << 16))));
    (v0 = (uint64_t)((uint64_t)a0 + (uint64_t)0ULL));
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v1, 2680))));
    { uint32_t addr = ADD32((uint32_t)a1, 7); uint32_t aligned_addr = addr & ~7u; uint32_t offset = addr & 7u; uint64_t mem = READ64(aligned_addr); uint32_t shift = (7u - offset) << 3; uint64_t keepMask = (shift == 0) ? 0ull : ((1ull << shift) - 1ull); (a2 = (uint64_t)((a2 & keepMask) | (mem << shift))); }
    { uint32_t addr = ADD32((uint32_t)a1, 0); uint32_t aligned_addr = addr & ~7u; uint32_t offset = addr & 7u; uint64_t mem = READ64(aligned_addr); uint32_t shift = offset << 3; uint64_t keepMask = (offset == 0) ? 0ull : (0xFFFFFFFFFFFFFFFFull << ((8u - offset) << 3)); (a2 = (uint64_t)((a2 & keepMask) | (mem >> shift))); }
    { uint32_t addr = ADD32((uint32_t)a1, 15); uint32_t aligned_addr = addr & ~7u; uint32_t offset = addr & 7u; uint64_t mem = READ64(aligned_addr); uint32_t shift = (7u - offset) << 3; uint64_t keepMask = (shift == 0) ? 0ull : ((1ull << shift) - 1ull); (a3 = (uint64_t)((a3 & keepMask) | (mem << shift))); }
    { uint32_t addr = ADD32((uint32_t)a1, 8); uint32_t aligned_addr = addr & ~7u; uint32_t offset = addr & 7u; uint64_t mem = READ64(aligned_addr); uint32_t shift = offset << 3; uint64_t keepMask = (offset == 0) ? 0ull : (0xFFFFFFFFFFFFFFFFull << ((8u - offset) << 3)); (a3 = (uint64_t)((a3 & keepMask) | (mem >> shift))); }
    { uint32_t addr = ADD32((uint32_t)a1, 23); uint32_t aligned_addr = addr & ~7u; uint32_t offset = addr & 7u; uint64_t mem = READ64(aligned_addr); uint32_t shift = (7u - offset) << 3; uint64_t keepMask = (shift == 0) ? 0ull : ((1ull << shift) - 1ull); (t0 = (uint64_t)((t0 & keepMask) | (mem << shift))); }
    { uint32_t addr = ADD32((uint32_t)a1, 16); uint32_t aligned_addr = addr & ~7u; uint32_t offset = addr & 7u; uint64_t mem = READ64(aligned_addr); uint32_t shift = offset << 3; uint64_t keepMask = (offset == 0) ? 0ull : (0xFFFFFFFFFFFFFFFFull << ((8u - offset) << 3)); (t0 = (uint64_t)((t0 & keepMask) | (mem >> shift))); }
    { uint32_t addr = ADD32((uint32_t)a1, 31); uint32_t aligned_addr = addr & ~7u; uint32_t offset = addr & 7u; uint64_t mem = READ64(aligned_addr); uint32_t shift = (7u - offset) << 3; uint64_t keepMask = (shift == 0) ? 0ull : ((1ull << shift) - 1ull); (t1 = (uint64_t)((t1 & keepMask) | (mem << shift))); }
    { uint32_t addr = ADD32((uint32_t)a1, 24); uint32_t aligned_addr = addr & ~7u; uint32_t offset = addr & 7u; uint64_t mem = READ64(aligned_addr); uint32_t shift = offset << 3; uint64_t keepMask = (offset == 0) ? 0ull : (0xFFFFFFFFFFFFFFFFull << ((8u - offset) << 3)); (t1 = (uint64_t)((t1 & keepMask) | (mem >> shift))); }
    { uint32_t addr = ADD32((uint32_t)v1, 7); uint32_t aligned_addr = addr & ~7u; uint32_t offset = addr & 7u; uint32_t shift = (7u - offset) << 3; uint64_t mask = 0xFFFFFFFFFFFFFFFFull >> shift; uint64_t old_data = READ64(aligned_addr); uint64_t val = a2; uint64_t new_data = (old_data & ~mask) | ((val >> shift) & mask); WRITE64(aligned_addr, new_data); }
    { uint32_t addr = ADD32((uint32_t)v1, 0); uint32_t aligned_addr = addr & ~7u; uint32_t offset = addr & 7u; uint32_t shift = offset << 3; uint64_t mask = 0xFFFFFFFFFFFFFFFFull << shift; uint64_t old_data = READ64(aligned_addr); uint64_t val = a2; uint64_t new_data = (old_data & ~mask) | ((val << shift) & mask); WRITE64(aligned_addr, new_data); }
    { uint32_t addr = ADD32((uint32_t)v1, 15); uint32_t aligned_addr = addr & ~7u; uint32_t offset = addr & 7u; uint32_t shift = (7u - offset) << 3; uint64_t mask = 0xFFFFFFFFFFFFFFFFull >> shift; uint64_t old_data = READ64(aligned_addr); uint64_t val = a3; uint64_t new_data = (old_data & ~mask) | ((val >> shift) & mask); WRITE64(aligned_addr, new_data); }
    { uint32_t addr = ADD32((uint32_t)v1, 8); uint32_t aligned_addr = addr & ~7u; uint32_t offset = addr & 7u; uint32_t shift = offset << 3; uint64_t mask = 0xFFFFFFFFFFFFFFFFull << shift; uint64_t old_data = READ64(aligned_addr); uint64_t val = a3; uint64_t new_data = (old_data & ~mask) | ((val << shift) & mask); WRITE64(aligned_addr, new_data); }
    { uint32_t addr = ADD32((uint32_t)v1, 23); uint32_t aligned_addr = addr & ~7u; uint32_t offset = addr & 7u; uint32_t shift = (7u - offset) << 3; uint64_t mask = 0xFFFFFFFFFFFFFFFFull >> shift; uint64_t old_data = READ64(aligned_addr); uint64_t val = t0; uint64_t new_data = (old_data & ~mask) | ((val >> shift) & mask); WRITE64(aligned_addr, new_data); }
    { uint32_t addr = ADD32((uint32_t)v1, 16); uint32_t aligned_addr = addr & ~7u; uint32_t offset = addr & 7u; uint32_t shift = offset << 3; uint64_t mask = 0xFFFFFFFFFFFFFFFFull << shift; uint64_t old_data = READ64(aligned_addr); uint64_t val = t0; uint64_t new_data = (old_data & ~mask) | ((val << shift) & mask); WRITE64(aligned_addr, new_data); }
    { uint32_t addr = ADD32((uint32_t)v1, 31); uint32_t aligned_addr = addr & ~7u; uint32_t offset = addr & 7u; uint32_t shift = (7u - offset) << 3; uint64_t mask = 0xFFFFFFFFFFFFFFFFull >> shift; uint64_t old_data = READ64(aligned_addr); uint64_t val = t1; uint64_t new_data = (old_data & ~mask) | ((val >> shift) & mask); WRITE64(aligned_addr, new_data); }
    { uint32_t addr = ADD32((uint32_t)v1, 24); uint32_t aligned_addr = addr & ~7u; uint32_t offset = addr & 7u; uint32_t shift = offset << 3; uint64_t mask = 0xFFFFFFFFFFFFFFFFull << shift; uint64_t old_data = READ64(aligned_addr); uint64_t val = t1; uint64_t new_data = (old_data & ~mask) | ((val << shift) & mask); WRITE64(aligned_addr, new_data); }
    {
        const uint32_t target = (uint32_t)ra;
        WRITE32_B((uint32_t)v1, 8, (uint32_t)a0);
        Ps2SetGpr64(ctx, 2, v0); Ps2SetGpr64(ctx, 3, v1); Ps2SetGpr64(ctx, 6, a2); Ps2SetGpr64(ctx, 7, a3); Ps2SetGpr64(ctx, 8, t0); Ps2SetGpr64(ctx, 9, t1);
        ctx->pc = target;
        return;
    }
}

// iosGet2ByteCharCode (0x1c9708): lift_d1's text (the reference)
void iosGet2ByteCharCode(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t v0 = Ps2GprU32(ctx, 2), v1 = Ps2GprU32(ctx, 3), a0 = Ps2GprU32(ctx, 4), ra = Ps2GprU32(ctx, 31);
    v0 = READ8_B(a0, 1);
    v1 = READ8(a0);
    v0 = v0 << 8;
    {
        const uint32_t target = ra;
        v0 = v0 | v1;
        Ps2SetGpr64(ctx, 2, sx64(v0)); Ps2SetGpr64(ctx, 3, sx64(v1));
        ctx->pc = target;
        return;
    }
}

// _isysCreateSubMsgQueue (0x1c9720): lift_d1's text (the reference)
void _isysCreateSubMsgQueue(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t a0 = Ps2GprU32(ctx, 4), a1 = Ps2GprU32(ctx, 5), a2 = Ps2GprU32(ctx, 6), a3 = Ps2GprU32(ctx, 7), ra = Ps2GprU32(ctx, 31);
    WRITE32_B(a0, 4, a1);
    WRITE32(a0, a2);
    WRITE32_B(a0, 8, a3);
    WRITE32_B(a0, 12, 0u);
    {
        const uint32_t target = ra;
        WRITE32_B(a0, 16, 0u);
        
        ctx->pc = target;
        return;
    }
}

// isysGetSubThreadWorkSize (0x1ca4a0): lift_d1's text (the reference)
void isysGetSubThreadWorkSize(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t v0 = Ps2GprU32(ctx, 2), a0 = Ps2GprU32(ctx, 4), ra = Ps2GprU32(ctx, 31);
    {
        const uint32_t target = ra;
        v0 = a0 + 0x33u;
        Ps2SetGpr64(ctx, 2, sx64(v0));
        ctx->pc = target;
        return;
    }
}

// isysDumpSubThreadPC (0x1ca5a0): lift_d1's text (the reference)
void isysDumpSubThreadPC(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t ra = Ps2GprU32(ctx, 31);
    {
        const uint32_t target = ra;
        
        ctx->pc = target;
        return;
    }
}

// isysDebugSetSubThreadSleepProtection (0x1ca5a8): lift_d1's text (the reference)
void isysDebugSetSubThreadSleepProtection(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t v0 = Ps2GprU32(ctx, 2), a0 = Ps2GprU32(ctx, 4), ra = Ps2GprU32(ctx, 31);
    v0 = 0x1E0000u;
    {
        const uint32_t target = ra;
        WRITE32_B(v0, 5460, a0);
        Ps2SetGpr64(ctx, 2, sx64(v0));
        ctx->pc = target;
        return;
    }
}

// sndSepSetDefaultParam (0x1cad30): lift_d1's text (the reference)
void sndSepSetDefaultParam(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t at = Ps2GprU32(ctx, 1), v0 = Ps2GprU32(ctx, 2), v1 = Ps2GprU32(ctx, 3), a0 = Ps2GprU32(ctx, 4), a1 = Ps2GprU32(ctx, 5), a2 = Ps2GprU32(ctx, 6), ra = Ps2GprU32(ctx, 31);
    float f0 = ctx->f[0], f1 = ctx->f[1], f2 = ctx->f[2], f3 = ctx->f[3];
    at = 0x42FE0000u;
    f1 = lift_d1::flt(at);
    v1 = 0xFFFFFFFFu;
    f0 = lift_d1::flt(READ32_B(a0, 28));
    v0 = 0xCu;
    WRITE32_B(a0, 8, v0);
    a2 = 1u;
    f0 = FPU_MUL_S(f0, f1);
    WRITE16_B(a0, 42, (uint16_t)v1);
    at = 0xBF800000u;
    f3 = lift_d1::flt(at);
    a1 = 0x1000u;
    at = 0x42800000u;
    f2 = lift_d1::flt(at);
    WRITE32_B(a0, 32, lift_d1::bits(f3));
    { int32_t tmp = FPU_CVT_W_S(f0); std::memcpy(&f1, &tmp, sizeof(tmp)); }
    v0 = lift_d1::bits(f1);
    WRITE32_B(a0, 36, lift_d1::bits(f2));
    WRITE16_B(a0, 46, (uint16_t)a2);
    WRITE16_B(a0, 44, (uint16_t)a1);
    WRITE16_B(a0, 48, (uint16_t)a2);
    {
        const uint32_t target = ra;
        WRITE16_B(a0, 40, (uint16_t)v0);
        ctx->f[0] = f0; ctx->f[1] = f1; ctx->f[2] = f2; ctx->f[3] = f3; Ps2SetGpr64(ctx, 1, sx64(at)); Ps2SetGpr64(ctx, 2, sx64(v0)); Ps2SetGpr64(ctx, 3, sx64(v1)); Ps2SetGpr64(ctx, 5, sx64(a1)); Ps2SetGpr64(ctx, 6, sx64(a2));
        ctx->pc = target;
        return;
    }
}

// sndSepPauseAll (0x1cad90): lift_d1's text (the reference)
void sndSepPauseAll(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t v0 = Ps2GprU32(ctx, 2), v1 = Ps2GprU32(ctx, 3), ra = Ps2GprU32(ctx, 31);
    uint64_t a0 = Ps2GprU64(ctx, 4);
    {
        const bool c = (int64_t)a0 == 0;
        v1 = 0x1E0000u;
        if (!c) {
            v0 = 1u;
            {
                const uint32_t target = ra;
                WRITE32_B(v1, 5468, v0);
                Ps2SetGpr64(ctx, 2, sx64(v0)); Ps2SetGpr64(ctx, 3, sx64(v1));
                ctx->pc = target;
                return;
            }
        }
    }
    v0 = 0x1E0000u;
    {
        const uint32_t target = ra;
        WRITE32_B(v0, 5468, 0u);
        Ps2SetGpr64(ctx, 2, sx64(v0)); Ps2SetGpr64(ctx, 3, sx64(v1));
        ctx->pc = target;
        return;
    }
}

// sndSepPause (0x1cadb8): lift_d1's text (the reference)
void sndSepPause(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint64_t v0 = Ps2GprU64(ctx, 2), v1 = Ps2GprU64(ctx, 3), a0 = Ps2GprU64(ctx, 4), ra = Ps2GprU64(ctx, 31);
    (v0 = (uint64_t)(READ64_B((uint32_t)a0, 8)));
    (v1 = (uint64_t)(0ULL | (uint64_t)(uint16_t)32768));
    (v1 = (uint64_t)(RC_U64X(v1) << 18));
    (v0 = (uint64_t)(v0 | v1));
    {
        const uint32_t target = (uint32_t)ra;
        WRITE64_B((uint32_t)a0, 8, v0);
        Ps2SetGpr64(ctx, 2, v0); Ps2SetGpr64(ctx, 3, v1);
        ctx->pc = target;
        return;
    }
}

// sndSepCont (0x1cadd0): lift_d1's text (the reference)
void sndSepCont(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint64_t v0 = Ps2GprU64(ctx, 2), v1 = Ps2GprU64(ctx, 3), a0 = Ps2GprU64(ctx, 4), ra = Ps2GprU64(ctx, 31);
    (v0 = (uint64_t)(READ64_B((uint32_t)a0, 8)));
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 4294967293))));
    (v1 = (uint64_t)(RC_U64X(v1) << 16));
    (v1 = (uint64_t)(v1 | (uint64_t)(uint16_t)65535));
    (v1 = (uint64_t)(v1 << 16));
    (v1 = (uint64_t)(v1 | (uint64_t)(uint16_t)65535));
    (v0 = (uint64_t)(v0 & v1));
    {
        const uint32_t target = (uint32_t)ra;
        WRITE64_B((uint32_t)a0, 8, v0);
        Ps2SetGpr64(ctx, 2, v0); Ps2SetGpr64(ctx, 3, v1);
        ctx->pc = target;
        return;
    }
}

// sndVabDelete (0x1caf88): lift_d1's text (the reference)
void sndVabDelete(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t v0 = Ps2GprU32(ctx, 2), v1 = Ps2GprU32(ctx, 3), a0 = Ps2GprU32(ctx, 4), ra = Ps2GprU32(ctx, 31);
    v0 = READ32(a0);
    v1 = 0xFFFFFFFEu;
    v0 = v0 & v1;
    {
        const uint32_t target = ra;
        WRITE32(a0, v0);
        Ps2SetGpr64(ctx, 2, sx64(v0)); Ps2SetGpr64(ctx, 3, sx64(v1));
        ctx->pc = target;
        return;
    }
}

// sndBDClose (0x1cb168): lift_d1's text (the reference)
void sndBDClose(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t ra = Ps2GprU32(ctx, 31);
    {
        const uint32_t target = ra;
        
        ctx->pc = target;
        return;
    }
}

// addAdpcmTickProcFunc (0x1cb3a0): lift_d1's text (the reference)
void addAdpcmTickProcFunc(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t v0 = Ps2GprU32(ctx, 2), a0 = Ps2GprU32(ctx, 4), ra = Ps2GprU32(ctx, 31);
    v0 = 0x270000u;
    {
        const uint32_t target = ra;
        WRITE32_B(v0, 7176, a0);
        Ps2SetGpr64(ctx, 2, sx64(v0));
        ctx->pc = target;
        return;
    }
}

// setPcmTickProcFunc (0x1cb3b0): lift_d1's text (the reference)
void setPcmTickProcFunc(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t v0 = Ps2GprU32(ctx, 2), a0 = Ps2GprU32(ctx, 4), ra = Ps2GprU32(ctx, 31);
    v0 = 0x270000u;
    {
        const uint32_t target = ra;
        WRITE32_B(v0, 7180, a0);
        Ps2SetGpr64(ctx, 2, sx64(v0));
        ctx->pc = target;
        return;
    }
}

// soundGetTickProcCnt (0x1cb3c0): lift_d1's text (the reference)
void soundGetTickProcCnt(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t v0 = Ps2GprU32(ctx, 2), v1 = Ps2GprU32(ctx, 3), ra = Ps2GprU32(ctx, 31);
    v1 = 0x1E0000u;
    {
        const uint32_t target = ra;
        v0 = READ32_B(v1, 5476);
        Ps2SetGpr64(ctx, 2, sx64(v0)); Ps2SetGpr64(ctx, 3, sx64(v1));
        ctx->pc = target;
        return;
    }
}

// soundSetRevMode (0x1cb3d0): lift_d1's text (the reference)
void soundSetRevMode(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t ra = Ps2GprU32(ctx, 31);
    uint64_t v0 = Ps2GprU64(ctx, 2), v1 = Ps2GprU64(ctx, 3), a0 = Ps2GprU64(ctx, 4), a1 = Ps2GprU64(ctx, 5);
    {
        const bool c = (int64_t)a0 != 0;
        (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)30 << 16))));
        if (!c) {
            (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)30 << 16))));
            (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v1, 5860))));
            (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)READ32_B((uint32_t)v1, 0))));
            (v0 = (uint64_t)(RC_U64X(v0) | a1));
            {
                const uint32_t target = ra;
                WRITE32_B((uint32_t)v1, 0, (uint32_t)v0);
                Ps2SetGpr64(ctx, 2, v0); Ps2SetGpr64(ctx, 3, v1);
                ctx->pc = target;
                return;
            }
        }
    }
    (a0 = (uint64_t)(~(0ULL | a1)));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v0, 5860))));
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)READ32_B((uint32_t)v0, 0))));
    (v1 = (uint64_t)(RC_U64X(v1) & a0));
    {
        const uint32_t target = ra;
        WRITE32_B((uint32_t)v0, 0, (uint32_t)v1);
        Ps2SetGpr64(ctx, 2, v0); Ps2SetGpr64(ctx, 3, v1); Ps2SetGpr64(ctx, 4, a0);
        ctx->pc = target;
        return;
    }
}

// soundGetRevMode (0x1cb408): lift_d1's text (the reference)
void soundGetRevMode(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t v0 = Ps2GprU32(ctx, 2), v1 = Ps2GprU32(ctx, 3), ra = Ps2GprU32(ctx, 31);
    v1 = 0x1E0000u;
    {
        const uint32_t target = ra;
        v0 = READ32_B(v1, 5860);
        Ps2SetGpr64(ctx, 2, sx64(v0)); Ps2SetGpr64(ctx, 3, sx64(v1));
        ctx->pc = target;
        return;
    }
}

// getSoundMode (0x1cb460): lift_d1's text (the reference)
void getSoundMode(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t v0 = Ps2GprU32(ctx, 2), v1 = Ps2GprU32(ctx, 3), ra = Ps2GprU32(ctx, 31);
    v1 = 0x1E0000u;
    v0 = READ32_B(v1, 5472);
    v0 = v0 ^ 1u;
    {
        const uint32_t target = ra;
        v0 = v0 < 1u ? 1u : 0u;
        Ps2SetGpr64(ctx, 2, sx64(v0)); Ps2SetGpr64(ctx, 3, sx64(v1));
        ctx->pc = target;
        return;
    }
}

// iosKeybdSetCallBackFunc (0x1cb920): lift_d1's text (the reference)
void iosKeybdSetCallBackFunc(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t v0 = Ps2GprU32(ctx, 2), v1 = Ps2GprU32(ctx, 3), a0 = Ps2GprU32(ctx, 4), ra = Ps2GprU32(ctx, 31);
    v0 = 0x1E0000u;
    v1 = READ32_B(v0, 5484);
    {
        const bool c = v1 == 0;
        v0 = 0u;
        if (!c) {
            v1 = 0x1E0000u;
            v0 = 1u;
            WRITE32_B(v1, 5488, a0);
        }
    }
    {
        const uint32_t target = ra;
        Ps2SetGpr64(ctx, 2, sx64(v0)); Ps2SetGpr64(ctx, 3, sx64(v1));
        ctx->pc = target;
        return;
    }
}

// iosMouseSetCallBackFunc (0x1cbc70): lift_d1's text (the reference)
void iosMouseSetCallBackFunc(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t v0 = Ps2GprU32(ctx, 2), v1 = Ps2GprU32(ctx, 3), a0 = Ps2GprU32(ctx, 4), ra = Ps2GprU32(ctx, 31);
    v0 = 0x1E0000u;
    v1 = READ32_B(v0, 5492);
    {
        const bool c = v1 == 0;
        v0 = 0u;
        if (!c) {
            v1 = 0x1E0000u;
            v0 = 1u;
            WRITE32_B(v1, 5496, a0);
        }
    }
    {
        const uint32_t target = ra;
        Ps2SetGpr64(ctx, 2, sx64(v0)); Ps2SetGpr64(ctx, 3, sx64(v1));
        ctx->pc = target;
        return;
    }
}

// GetStrHashWorkBuffer (0x1cbcb8): lift_d1's text (the reference)
void GetStrHashWorkBuffer(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t v0 = Ps2GprU32(ctx, 2), a0 = Ps2GprU32(ctx, 4), a1 = Ps2GprU32(ctx, 5), ra = Ps2GprU32(ctx, 31);
    v0 = 4u;
    a1 = a1 << 4;
    v0 = v0 << (a0 & 31);
    v0 = v0 + a1;
    {
        const uint32_t target = ra;
        v0 = v0 + 0x66u;
        Ps2SetGpr64(ctx, 2, sx64(v0)); Ps2SetGpr64(ctx, 5, sx64(a1));
        ctx->pc = target;
        return;
    }
}

// kernel_sub_001CC028 (0x1cc028): lift_d1's text (the reference)
void kernel_sub_001CC028(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t ra = Ps2GprU32(ctx, 31);
    {
        const uint32_t target = ra;
        
        ctx->pc = target;
        return;
    }
}

// Sg2CtrlSetTickMode (0x1cc228): lift_d1's text (the reference)
void Sg2CtrlSetTickMode(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t v0 = Ps2GprU32(ctx, 2), v1 = Ps2GprU32(ctx, 3), a0 = Ps2GprU32(ctx, 4), ra = Ps2GprU32(ctx, 31);
    v1 = 0x270000u;
    v1 = v1 + 0x3DC4u;
    v0 = READ32(v1);
    {
        const uint32_t target = ra;
        WRITE32(v1, a0);
        Ps2SetGpr64(ctx, 2, sx64(v0)); Ps2SetGpr64(ctx, 3, sx64(v1));
        ctx->pc = target;
        return;
    }
}

// Sg2CtrlGetTickMode (0x1cc240): lift_d1's text (the reference)
void Sg2CtrlGetTickMode(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t v0 = Ps2GprU32(ctx, 2), v1 = Ps2GprU32(ctx, 3), ra = Ps2GprU32(ctx, 31);
    v1 = 0x270000u;
    {
        const uint32_t target = ra;
        v0 = READ32_B(v1, 15812);
        Ps2SetGpr64(ctx, 2, sx64(v0)); Ps2SetGpr64(ctx, 3, sx64(v1));
        ctx->pc = target;
        return;
    }
}

// Sg2CtrlSetTickTime (0x1cc250): lift_d1's text (the reference)
void Sg2CtrlSetTickTime(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t v0 = Ps2GprU32(ctx, 2), v1 = Ps2GprU32(ctx, 3), a0 = Ps2GprU32(ctx, 4), ra = Ps2GprU32(ctx, 31);
    v1 = 0x270000u;
    v1 = v1 + 0x3DC0u;
    v0 = READ32(v1);
    {
        const uint32_t target = ra;
        WRITE32(v1, a0);
        Ps2SetGpr64(ctx, 2, sx64(v0)); Ps2SetGpr64(ctx, 3, sx64(v1));
        ctx->pc = target;
        return;
    }
}

// Sg2CtrlGetTickTime (0x1cc268): lift_d1's text (the reference)
void Sg2CtrlGetTickTime(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t v0 = Ps2GprU32(ctx, 2), v1 = Ps2GprU32(ctx, 3), ra = Ps2GprU32(ctx, 31);
    v1 = 0x270000u;
    {
        const uint32_t target = ra;
        v0 = READ32_B(v1, 15808);
        Ps2SetGpr64(ctx, 2, sx64(v0)); Ps2SetGpr64(ctx, 3, sx64(v1));
        ctx->pc = target;
        return;
    }
}

// Sg2DmaInit (0x1cc318): lift_d1's text (the reference)
void Sg2DmaInit(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t ra = Ps2GprU32(ctx, 31);
    uint64_t v0 = Ps2GprU64(ctx, 2);
    {
        const uint32_t target = ra;
        (v0 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL));
        Ps2SetGpr64(ctx, 2, v0);
        ctx->pc = target;
        return;
    }
}

// Sg2DspGetMasterVolume (0x1cc7d8): lift_d1's text (the reference)
void Sg2DspGetMasterVolume(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t v0 = Ps2GprU32(ctx, 2), v1 = Ps2GprU32(ctx, 3), a0 = Ps2GprU32(ctx, 4), ra = Ps2GprU32(ctx, 31);
    v1 = a0 << 1;
    v1 = v1 + a0;
    v1 = v1 << 3;
    v0 = 0x270000u;
    v0 = v0 + v1;
    v0 = READ32_B(v0, 15816);
    {
        const uint32_t target = ra;
        Ps2SetGpr64(ctx, 2, sx64(v0)); Ps2SetGpr64(ctx, 3, sx64(v1));
        ctx->pc = target;
        return;
    }
}

// Sg2ExStAdpcmStopMode (0x1cc898): lift_d1's text (the reference)
void Sg2ExStAdpcmStopMode(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t v0 = Ps2GprU32(ctx, 2), a0 = Ps2GprU32(ctx, 4), ra = Ps2GprU32(ctx, 31);
    v0 = 0x270000u;
    {
        const uint32_t target = ra;
        WRITE32_B(v0, 15868, a0);
        Ps2SetGpr64(ctx, 2, sx64(v0));
        ctx->pc = target;
        return;
    }
}

// Sg2ExStAdpcmSetFd (0x1cced0): lift_d1's text (the reference)
void Sg2ExStAdpcmSetFd(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t a0 = Ps2GprU32(ctx, 4), a1 = Ps2GprU32(ctx, 5), ra = Ps2GprU32(ctx, 31);
    uint64_t v0 = Ps2GprU64(ctx, 2);
    WRITE32_B(a0, 728, a1);
    {
        const uint32_t target = ra;
        (v0 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL));
        Ps2SetGpr64(ctx, 2, v0);
        ctx->pc = target;
        return;
    }
}

// Sg2ExStAdpcmGetFd (0x1ccee0): lift_d1's text (the reference)
void Sg2ExStAdpcmGetFd(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t v0 = Ps2GprU32(ctx, 2), a0 = Ps2GprU32(ctx, 4), ra = Ps2GprU32(ctx, 31);
    {
        const uint32_t target = ra;
        v0 = READ32_B(a0, 728);
        Ps2SetGpr64(ctx, 2, sx64(v0));
        ctx->pc = target;
        return;
    }
}

// Sg2ExStAdpcmSetLsn (0x1ccee8): lift_d1's text (the reference)
void Sg2ExStAdpcmSetLsn(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t a0 = Ps2GprU32(ctx, 4), a1 = Ps2GprU32(ctx, 5), ra = Ps2GprU32(ctx, 31);
    uint64_t v0 = Ps2GprU64(ctx, 2);
    WRITE32_B(a0, 728, a1);
    {
        const uint32_t target = ra;
        (v0 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL));
        Ps2SetGpr64(ctx, 2, v0);
        ctx->pc = target;
        return;
    }
}

// Sg2ExStAdpcmGetLsn (0x1ccef8): lift_d1's text (the reference)
void Sg2ExStAdpcmGetLsn(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t v0 = Ps2GprU32(ctx, 2), a0 = Ps2GprU32(ctx, 4), ra = Ps2GprU32(ctx, 31);
    {
        const uint32_t target = ra;
        v0 = READ32_B(a0, 728);
        Ps2SetGpr64(ctx, 2, sx64(v0));
        ctx->pc = target;
        return;
    }
}

// Sg2ExStAdpcmGetTargetLsn (0x1ccf00): lift_d1's text (the reference)
void Sg2ExStAdpcmGetTargetLsn(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t v0 = Ps2GprU32(ctx, 2), v1 = Ps2GprU32(ctx, 3), a0 = Ps2GprU32(ctx, 4), ra = Ps2GprU32(ctx, 31);
    v0 = READ32_B(a0, 584);
    v1 = READ32_B(a0, 728);
    v0 = v0 >> 11;
    {
        const uint32_t target = ra;
        v0 = v1 + v0;
        Ps2SetGpr64(ctx, 2, sx64(v0)); Ps2SetGpr64(ctx, 3, sx64(v1));
        ctx->pc = target;
        return;
    }
}

// Sg2ExStAdpcmSetUserParam (0x1ccf18): lift_d1's text (the reference)
void Sg2ExStAdpcmSetUserParam(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t a0 = Ps2GprU32(ctx, 4), a1 = Ps2GprU32(ctx, 5), ra = Ps2GprU32(ctx, 31);
    uint64_t v0 = Ps2GprU64(ctx, 2);
    WRITE32_B(a0, 732, a1);
    {
        const uint32_t target = ra;
        (v0 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL));
        Ps2SetGpr64(ctx, 2, v0);
        ctx->pc = target;
        return;
    }
}

// Sg2ExStAdpcmGetUserParam (0x1ccf28): lift_d1's text (the reference)
void Sg2ExStAdpcmGetUserParam(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t v0 = Ps2GprU32(ctx, 2), a0 = Ps2GprU32(ctx, 4), ra = Ps2GprU32(ctx, 31);
    {
        const uint32_t target = ra;
        v0 = READ32_B(a0, 732);
        Ps2SetGpr64(ctx, 2, sx64(v0));
        ctx->pc = target;
        return;
    }
}

// Sg2ExStAdpcmLoopSecNum (0x1cd6f8): lift_d1's text (the reference)
void Sg2ExStAdpcmLoopSecNum(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t v1 = Ps2GprU32(ctx, 3), a0 = Ps2GprU32(ctx, 4), a1 = Ps2GprU32(ctx, 5), a2 = Ps2GprU32(ctx, 6), ra = Ps2GprU32(ctx, 31);
    uint64_t v0 = Ps2GprU64(ctx, 2);
    v1 = READ32_B(a0, 56);
    (v0 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL));
    WRITE32(a1, v1);
    a1 = READ32_B(a0, 60);
    {
        const uint32_t target = ra;
        WRITE32(a2, a1);
        Ps2SetGpr64(ctx, 2, v0); Ps2SetGpr64(ctx, 3, sx64(v1)); Ps2SetGpr64(ctx, 5, sx64(a1));
        ctx->pc = target;
        return;
    }
}

// Sg2ExStAdpcmGetBuffer (0x1cd710): lift_d1's text (the reference)
void Sg2ExStAdpcmGetBuffer(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t v1 = Ps2GprU32(ctx, 3), a0 = Ps2GprU32(ctx, 4), a1 = Ps2GprU32(ctx, 5), a2 = Ps2GprU32(ctx, 6), a3 = Ps2GprU32(ctx, 7), t0 = Ps2GprU32(ctx, 8), ra = Ps2GprU32(ctx, 31);
    uint64_t v0 = Ps2GprU64(ctx, 2);
    v1 = READ32_B(a0, 560);
    (v0 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL));
    WRITE32(a1, v1);
    a1 = READ32_B(a0, 564);
    WRITE32(a2, a1);
    v1 = READ32_B(a0, 568);
    WRITE32(a3, v1);
    a1 = READ32_B(a0, 572);
    {
        const uint32_t target = ra;
        WRITE32(t0, a1);
        Ps2SetGpr64(ctx, 2, v0); Ps2SetGpr64(ctx, 3, sx64(v1)); Ps2SetGpr64(ctx, 5, sx64(a1));
        ctx->pc = target;
        return;
    }
}

// Sg2ExStAdpcmSetSignal (0x1cd738): lift_d1's text (the reference)
void Sg2ExStAdpcmSetSignal(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t v0 = Ps2GprU32(ctx, 2), a0 = Ps2GprU32(ctx, 4), ra = Ps2GprU32(ctx, 31);
    v0 = 0x270000u;
    {
        const uint32_t target = ra;
        WRITE32_B(v0, 16064, a0);
        Ps2SetGpr64(ctx, 2, sx64(v0));
        ctx->pc = target;
        return;
    }
}

// Sg2ModCheckSndOn (0x1cec38): lift_d1's text (the reference)
void Sg2ModCheckSndOn(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t v0 = Ps2GprU32(ctx, 2), v1 = Ps2GprU32(ctx, 3), ra = Ps2GprU32(ctx, 31);
    uint64_t a0 = Ps2GprU64(ctx, 4);
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
            v0 = READ32_B(v0, 16088);
            v0 = ~(0u | v0);
            {
                const uint32_t target = ra;
                v0 = 0u < v0 ? 1u : 0u;
                Ps2SetGpr64(ctx, 2, sx64(v0)); Ps2SetGpr64(ctx, 3, sx64(v1));
                ctx->pc = target;
                return;
            }
        }
    }
    {
        const uint32_t target = ra;
        v0 = 0u;
        Ps2SetGpr64(ctx, 2, sx64(v0));
        ctx->pc = target;
        return;
    }
}

// Sg2ModClose (0x1cec70): lift_d1's text (the reference)
void Sg2ModClose(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t at = Ps2GprU32(ctx, 1), v0 = Ps2GprU32(ctx, 2), v1 = Ps2GprU32(ctx, 3), ra = Ps2GprU32(ctx, 31);
    uint64_t a0 = Ps2GprU64(ctx, 4);
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
            WRITE32_B(at, 16104, (uint32_t)a0);
        }
    }
    {
        const uint32_t target = ra;
        v0 = 0u;
        if (df_at) Ps2SetGpr64(ctx, 1, sx64(at)); Ps2SetGpr64(ctx, 2, sx64(v0)); if (df_v1) Ps2SetGpr64(ctx, 3, sx64(v1)); Ps2SetGpr64(ctx, 4, a0);
        ctx->pc = target;
        return;
    }
}

// Sg2ModGetParam (0x1cf6e8): lift_d1's text (the reference)
void Sg2ModGetParam(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t v0 = Ps2GprU32(ctx, 2), v1 = Ps2GprU32(ctx, 3), a2 = Ps2GprU32(ctx, 6), a3 = Ps2GprU32(ctx, 7), ra = Ps2GprU32(ctx, 31);
    uint64_t a0 = Ps2GprU64(ctx, 4), a1 = Ps2GprU64(ctx, 5), t0 = Ps2GprU64(ctx, 8), t1 = Ps2GprU64(ctx, 9);
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
                        const uint32_t target = ra;
                        v0 = 0u;
                        Ps2SetGpr64(ctx, 2, sx64(v0)); Ps2SetGpr64(ctx, 3, sx64(v1)); Ps2SetGpr64(ctx, 4, a0); Ps2SetGpr64(ctx, 8, t0); Ps2SetGpr64(ctx, 9, t1);
                        ctx->pc = target;
                        return;
                    }
                }
            }
            if ((int64_t)a1 == (int64_t)(int32_t)v0) {
                (v0 = (uint32_t)((int32_t)((int32_t)SLL32((uint32_t)t0, 2))));
                goto L_1cf788;
            }
            {
                const uint32_t target = ra;
                v0 = 0u;
                Ps2SetGpr64(ctx, 2, sx64(v0)); Ps2SetGpr64(ctx, 3, sx64(v1)); Ps2SetGpr64(ctx, 4, a0); Ps2SetGpr64(ctx, 8, t0); Ps2SetGpr64(ctx, 9, t1);
                ctx->pc = target;
                return;
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
            (v1 = (uint32_t)((int32_t)((int32_t)READ32_B((uint32_t)a1, 4))));
            (a0 = (uint64_t)((uint64_t)RC_U64X(a1) + (uint64_t)0ULL));
            WRITE32(a2, v1);
            (v0 = (uint32_t)((int32_t)((int32_t)READ32_B((uint32_t)a0, 8))));
            WRITE32(a3, v0);
            goto L_1cf7a8;
        }
    }
    (v0 = (uint32_t)((int32_t)((int32_t)SUB32(v0, (uint32_t)a0))));
    v0 = v0 << 4;
    v1 = 0x270000u;
    v1 = v1 + v0;
    v1 = READ32_B(v1, 16148);
    WRITE32(a2, v1);
    goto L_1cf7a8;
L_1cf788:
    (v0 = (uint32_t)((int32_t)((int32_t)ADD32(v0, (uint32_t)t0))));
    v1 = v0 << 5;
    v1 = v1 - v0;
    v1 = v1 << 4;
    v0 = 0x270000u;
    v0 = v0 + v1;
    v0 = READ32_B(v0, 16152);
    WRITE32(a2, v0);
L_1cf7a8:
    {
        const uint32_t target = ra;
        v0 = 0u;
        Ps2SetGpr64(ctx, 2, sx64(v0)); Ps2SetGpr64(ctx, 3, sx64(v1)); Ps2SetGpr64(ctx, 4, a0); Ps2SetGpr64(ctx, 5, a1); Ps2SetGpr64(ctx, 8, t0); Ps2SetGpr64(ctx, 9, t1);
        ctx->pc = target;
        return;
    }
}

// kernel_sub_001D0D78 (0x1d0d78): lift_d1's text (the reference)
void kernel_sub_001D0D78(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t v0 = Ps2GprU32(ctx, 2), v1 = Ps2GprU32(ctx, 3), a0 = Ps2GprU32(ctx, 4), a1 = Ps2GprU32(ctx, 5), a2 = Ps2GprU32(ctx, 6), ra = Ps2GprU32(ctx, 31);
    v0 = 0x270000u;
    a0 = 0x270000u;
    a1 = READ32_B(v0, 16076);
    a2 = READ32_B(a0, 16080);
    v1 = READ8(a1);
    a0 = READ8_B(a1, 2);
    v1 = v1 & 0xFu;
    v0 = v1 << 3;
    v0 = v0 - v1;
    v0 = v0 << 4;
    v0 = v0 + a2;
    {
        const uint32_t target = ra;
        WRITE32_B(v0, 692, a0);
        Ps2SetGpr64(ctx, 2, sx64(v0)); Ps2SetGpr64(ctx, 3, sx64(v1)); Ps2SetGpr64(ctx, 4, sx64(a0)); Ps2SetGpr64(ctx, 5, sx64(a1)); Ps2SetGpr64(ctx, 6, sx64(a2));
        ctx->pc = target;
        return;
    }
}

// kernel_sub_001D12A8 (0x1d12a8): lift_d1's text (the reference)
void kernel_sub_001D12A8(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t v0 = Ps2GprU32(ctx, 2), v1 = Ps2GprU32(ctx, 3), a0 = Ps2GprU32(ctx, 4), a1 = Ps2GprU32(ctx, 5), a2 = Ps2GprU32(ctx, 6), ra = Ps2GprU32(ctx, 31);
    v0 = 0x270000u;
    a0 = 0x270000u;
    a1 = READ32_B(v0, 16076);
    a2 = READ32_B(a0, 16080);
    v1 = READ8(a1);
    a0 = READ8_B(a1, 2);
    v1 = v1 & 0xFu;
    v0 = v1 << 3;
    v0 = v0 - v1;
    v0 = v0 << 4;
    v0 = v0 + a2;
    {
        const uint32_t target = ra;
        WRITE32_B(v0, 708, a0);
        Ps2SetGpr64(ctx, 2, sx64(v0)); Ps2SetGpr64(ctx, 3, sx64(v1)); Ps2SetGpr64(ctx, 4, sx64(a0)); Ps2SetGpr64(ctx, 5, sx64(a1)); Ps2SetGpr64(ctx, 6, sx64(a2));
        ctx->pc = target;
        return;
    }
}

// kernel_sub_001D12E0 (0x1d12e0): lift_d1's text (the reference)
void kernel_sub_001D12E0(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t v0 = Ps2GprU32(ctx, 2), v1 = Ps2GprU32(ctx, 3), a0 = Ps2GprU32(ctx, 4), a1 = Ps2GprU32(ctx, 5), a2 = Ps2GprU32(ctx, 6), ra = Ps2GprU32(ctx, 31);
    v0 = 0x270000u;
    a0 = 0x270000u;
    a1 = READ32_B(v0, 16076);
    a2 = READ32_B(a0, 16080);
    v1 = READ8(a1);
    a0 = READ8_B(a1, 2);
    v1 = v1 & 0xFu;
    v0 = v1 << 3;
    v0 = v0 - v1;
    v0 = v0 << 4;
    v0 = v0 + a2;
    {
        const uint32_t target = ra;
        WRITE32_B(v0, 700, a0);
        Ps2SetGpr64(ctx, 2, sx64(v0)); Ps2SetGpr64(ctx, 3, sx64(v1)); Ps2SetGpr64(ctx, 4, sx64(a0)); Ps2SetGpr64(ctx, 5, sx64(a1)); Ps2SetGpr64(ctx, 6, sx64(a2));
        ctx->pc = target;
        return;
    }
}

// kernel_sub_001D1318 (0x1d1318): lift_d1's text (the reference)
void kernel_sub_001D1318(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t v0 = Ps2GprU32(ctx, 2), v1 = Ps2GprU32(ctx, 3), a0 = Ps2GprU32(ctx, 4), a1 = Ps2GprU32(ctx, 5), a2 = Ps2GprU32(ctx, 6), ra = Ps2GprU32(ctx, 31);
    v0 = 0x270000u;
    a0 = 0x270000u;
    a1 = READ32_B(v0, 16076);
    a2 = READ32_B(a0, 16080);
    v1 = READ8(a1);
    a0 = READ8_B(a1, 2);
    v1 = v1 & 0xFu;
    v0 = v1 << 3;
    v0 = v0 - v1;
    v0 = v0 << 4;
    v0 = v0 + a2;
    {
        const uint32_t target = ra;
        WRITE32_B(v0, 720, a0);
        Ps2SetGpr64(ctx, 2, sx64(v0)); Ps2SetGpr64(ctx, 3, sx64(v1)); Ps2SetGpr64(ctx, 4, sx64(a0)); Ps2SetGpr64(ctx, 5, sx64(a1)); Ps2SetGpr64(ctx, 6, sx64(a2));
        ctx->pc = target;
        return;
    }
}

// kernel_sub_001D1350 (0x1d1350): lift_d1's text (the reference)
void kernel_sub_001D1350(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t ra = Ps2GprU32(ctx, 31);
    {
        const uint32_t target = ra;
        
        ctx->pc = target;
        return;
    }
}

// kernel_sub_001D1358 (0x1d1358): lift_d1's text (the reference)
void kernel_sub_001D1358(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t ra = Ps2GprU32(ctx, 31);
    {
        const uint32_t target = ra;
        
        ctx->pc = target;
        return;
    }
}

// kernel_sub_001D13E8 (0x1d13e8): lift_d1's text (the reference)
void kernel_sub_001D13E8(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t v0 = Ps2GprU32(ctx, 2), v1 = Ps2GprU32(ctx, 3), a0 = Ps2GprU32(ctx, 4), a1 = Ps2GprU32(ctx, 5), a2 = Ps2GprU32(ctx, 6), ra = Ps2GprU32(ctx, 31);
    v0 = 0x270000u;
    a0 = 0x270000u;
    a1 = READ32_B(v0, 16076);
    a2 = READ32_B(a0, 16080);
    v1 = READ8(a1);
    a0 = READ8_B(a1, 2);
    v1 = v1 & 0xFu;
    v0 = v1 << 3;
    v0 = v0 - v1;
    v0 = v0 << 4;
    v0 = v0 + a2;
    {
        const uint32_t target = ra;
        WRITE32_B(v0, 744, a0);
        Ps2SetGpr64(ctx, 2, sx64(v0)); Ps2SetGpr64(ctx, 3, sx64(v1)); Ps2SetGpr64(ctx, 4, sx64(a0)); Ps2SetGpr64(ctx, 5, sx64(a1)); Ps2SetGpr64(ctx, 6, sx64(a2));
        ctx->pc = target;
        return;
    }
}

// kernel_sub_001D1470 (0x1d1470): lift_d1's text (the reference)
void kernel_sub_001D1470(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t v0 = Ps2GprU32(ctx, 2), v1 = Ps2GprU32(ctx, 3), a0 = Ps2GprU32(ctx, 4), a1 = Ps2GprU32(ctx, 5), a2 = Ps2GprU32(ctx, 6), ra = Ps2GprU32(ctx, 31);
    v0 = 0x270000u;
    a1 = v0 + 0x3ECCu;
    a0 = READ32(a1);
    v0 = READ8_B(a0, 2);
    v1 = READ8(a0);
    v0 = v0 < 0x10u ? 1u : 0u;
    {
        const bool c = v0 != 0;
        a2 = v1 & 0xFu;
        if (!c) {
            v0 = 0xFu;
            WRITE8_B(a0, 2, (uint8_t)v0);
            a0 = READ32(a1);
        }
    }
    v1 = READ8_B(a0, 2);
    a1 = 0x270000u;
    v0 = a2 << 3;
    a0 = READ32_B(a1, 16080);
    v0 = v0 - a2;
    v1 = v1 << 1;
    v0 = v0 << 4;
    v1 = v1 & 0xF0u;
    v0 = v0 + a0;
    {
        const uint32_t target = ra;
        WRITE16_B(v0, 782, (uint16_t)v1);
        Ps2SetGpr64(ctx, 2, sx64(v0)); Ps2SetGpr64(ctx, 3, sx64(v1)); Ps2SetGpr64(ctx, 4, sx64(a0)); Ps2SetGpr64(ctx, 5, sx64(a1)); Ps2SetGpr64(ctx, 6, sx64(a2));
        ctx->pc = target;
        return;
    }
}

// kernel_sub_001D14C8 (0x1d14c8): lift_d1's text (the reference)
void kernel_sub_001D14C8(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint64_t v0 = Ps2GprU64(ctx, 2), v1 = Ps2GprU64(ctx, 3), a0 = Ps2GprU64(ctx, 4), a1 = Ps2GprU64(ctx, 5), a2 = Ps2GprU64(ctx, 6), ra = Ps2GprU64(ctx, 31);
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)39 << 16))));
    (a1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v0, 16076))));
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)READ32_B((uint32_t)a1, 0))));
    (v0 = (uint64_t)(uint32_t)((uint8_t)READ8_B((uint32_t)a0, 2)));
    (v1 = (uint64_t)(uint32_t)((uint8_t)READ8_B((uint32_t)a0, 0)));
    (v0 = (uint64_t)(((uint64_t)RC_U64X(v0) < (uint64_t)(int64_t)(int32_t)32) ? 1 : 0));
    {
        const bool c = (int64_t)v0 != 0;
        (a2 = (uint64_t)(RC_U64X(v1) & (uint64_t)(uint16_t)15));
        if (!c) {
            (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 31))));
            WRITE8_B((uint32_t)a0, 2, (uint8_t)(uint32_t)v0);
            (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)READ32_B((uint32_t)a1, 0))));
        }
    }
    (v1 = (uint64_t)(uint32_t)((uint8_t)READ8_B((uint32_t)a0, 2)));
    (a1 = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)39 << 16))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)SLL32((uint32_t)a2, 3))));
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)READ32_B((uint32_t)a1, 16080))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)SUB32((uint32_t)v0, (uint32_t)a2))));
    (v1 = (uint64_t)(RC_U64X(v1) >> 2));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)SLL32((uint32_t)v0, 4))));
    (v1 = (uint64_t)(v1 & (uint64_t)(uint16_t)31));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v0, (uint32_t)a0))));
    {
        const uint32_t target = (uint32_t)ra;
        WRITE16_B((uint32_t)v0, 786, (uint16_t)(uint32_t)v1);
        Ps2SetGpr64(ctx, 2, v0); Ps2SetGpr64(ctx, 3, v1); Ps2SetGpr64(ctx, 4, a0); Ps2SetGpr64(ctx, 5, a1); Ps2SetGpr64(ctx, 6, a2);
        ctx->pc = target;
        return;
    }
}

// kernel_sub_001D1520 (0x1d1520): lift_d1's text (the reference)
void kernel_sub_001D1520(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t v0 = Ps2GprU32(ctx, 2), v1 = Ps2GprU32(ctx, 3), a0 = Ps2GprU32(ctx, 4), a1 = Ps2GprU32(ctx, 5), a2 = Ps2GprU32(ctx, 6), a3 = Ps2GprU32(ctx, 7), ra = Ps2GprU32(ctx, 31);
    v0 = 0x270000u;
    a1 = v0 + 0x3ECCu;
    a0 = READ32(a1);
    v0 = READ8(a0);
    v1 = (uint32_t)(int8_t)READ8_B(a0, 2);
    a2 = v0 & 0xFu;
    {
        const bool c = (int32_t)v1 >= 0;
        a3 = a2 << 3;
        if (!c) {
            v0 = 0x7Fu;
            WRITE8_B(a0, 2, (uint8_t)v0);
            a0 = READ32(a1);
        }
    }
    a0 = READ8_B(a0, 2);
    a1 = 0x270000u;
    v1 = READ32_B(a1, 16080);
    v0 = a3 - a2;
    v0 = v0 << 4;
    a0 = a0 << 8;
    a0 = a0 & 0x7F00u;
    v0 = v0 + v1;
    {
        const uint32_t target = ra;
        WRITE16_B(v0, 780, (uint16_t)a0);
        Ps2SetGpr64(ctx, 2, sx64(v0)); Ps2SetGpr64(ctx, 3, sx64(v1)); Ps2SetGpr64(ctx, 4, sx64(a0)); Ps2SetGpr64(ctx, 5, sx64(a1)); Ps2SetGpr64(ctx, 6, sx64(a2)); Ps2SetGpr64(ctx, 7, sx64(a3));
        ctx->pc = target;
        return;
    }
}

// kernel_sub_001D1578 (0x1d1578): lift_d1's text (the reference)
void kernel_sub_001D1578(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t ra = Ps2GprU32(ctx, 31);
    {
        const uint32_t target = ra;
        
        ctx->pc = target;
        return;
    }
}

// kernel_sub_001D1580 (0x1d1580): lift_d1's text (the reference)
void kernel_sub_001D1580(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t v0 = Ps2GprU32(ctx, 2), v1 = Ps2GprU32(ctx, 3), a0 = Ps2GprU32(ctx, 4), a1 = Ps2GprU32(ctx, 5), a2 = Ps2GprU32(ctx, 6), ra = Ps2GprU32(ctx, 31);
    v0 = 0x270000u;
    a0 = 0x270000u;
    a1 = READ32_B(v0, 16076);
    a2 = READ32_B(a0, 16080);
    v1 = READ8(a1);
    a0 = READ8_B(a1, 2);
    v1 = v1 & 0xFu;
    v0 = v1 << 3;
    v0 = v0 - v1;
    v0 = v0 << 4;
    v0 = v0 + a2;
    {
        const uint32_t target = ra;
        WRITE32_B(v0, 724, a0);
        Ps2SetGpr64(ctx, 2, sx64(v0)); Ps2SetGpr64(ctx, 3, sx64(v1)); Ps2SetGpr64(ctx, 4, sx64(a0)); Ps2SetGpr64(ctx, 5, sx64(a1)); Ps2SetGpr64(ctx, 6, sx64(a2));
        ctx->pc = target;
        return;
    }
}

// kernel_sub_001D1658 (0x1d1658): lift_d1's text (the reference)
void kernel_sub_001D1658(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t ra = Ps2GprU32(ctx, 31);
    {
        const uint32_t target = ra;
        
        ctx->pc = target;
        return;
    }
}

// kernel_sub_001D1660 (0x1d1660): lift_d1's text (the reference)
void kernel_sub_001D1660(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t v0 = Ps2GprU32(ctx, 2), v1 = Ps2GprU32(ctx, 3), a0 = Ps2GprU32(ctx, 4), a1 = Ps2GprU32(ctx, 5), a2 = Ps2GprU32(ctx, 6), ra = Ps2GprU32(ctx, 31);
    v0 = 0x270000u;
    a0 = 0x270000u;
    a1 = READ32_B(v0, 16076);
    a2 = READ32_B(a0, 16080);
    v1 = READ8(a1);
    a0 = READ8_B(a1, 2);
    v1 = v1 & 0xFu;
    v0 = v1 << 3;
    v0 = v0 - v1;
    v0 = v0 << 4;
    v0 = v0 + a2;
    {
        const uint32_t target = ra;
        WRITE8_B(v0, 789, (uint8_t)a0);
        Ps2SetGpr64(ctx, 2, sx64(v0)); Ps2SetGpr64(ctx, 3, sx64(v1)); Ps2SetGpr64(ctx, 4, sx64(a0)); Ps2SetGpr64(ctx, 5, sx64(a1)); Ps2SetGpr64(ctx, 6, sx64(a2));
        ctx->pc = target;
        return;
    }
}

// kernel_sub_001D1698 (0x1d1698): lift_d1's text (the reference)
void kernel_sub_001D1698(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t v0 = Ps2GprU32(ctx, 2), v1 = Ps2GprU32(ctx, 3), a0 = Ps2GprU32(ctx, 4), a1 = Ps2GprU32(ctx, 5), a2 = Ps2GprU32(ctx, 6), ra = Ps2GprU32(ctx, 31);
    v0 = 0x270000u;
    a0 = 0x270000u;
    a1 = READ32_B(v0, 16076);
    a2 = READ32_B(a0, 16080);
    v1 = READ8(a1);
    a0 = READ8_B(a1, 2);
    v1 = v1 & 0xFu;
    v0 = v1 << 3;
    v0 = v0 - v1;
    v0 = v0 << 4;
    v0 = v0 + a2;
    {
        const uint32_t target = ra;
        WRITE8_B(v0, 788, (uint8_t)a0);
        Ps2SetGpr64(ctx, 2, sx64(v0)); Ps2SetGpr64(ctx, 3, sx64(v1)); Ps2SetGpr64(ctx, 4, sx64(a0)); Ps2SetGpr64(ctx, 5, sx64(a1)); Ps2SetGpr64(ctx, 6, sx64(a2));
        ctx->pc = target;
        return;
    }
}

// kernel_sub_001D1810 (0x1d1810): lift_d1's text (the reference)
void kernel_sub_001D1810(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t ra = Ps2GprU32(ctx, 31);
    {
        const uint32_t target = ra;
        
        ctx->pc = target;
        return;
    }
}

// kernel_sub_001D18D8 (0x1d18d8): lift_d1's text (the reference)
void kernel_sub_001D18D8(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t ra = Ps2GprU32(ctx, 31);
    {
        const uint32_t target = ra;
        
        ctx->pc = target;
        return;
    }
}

// kernel_sub_001D1F08 (0x1d1f08): lift_d1's text (the reference)
void kernel_sub_001D1F08(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t ra = Ps2GprU32(ctx, 31);
    {
        const uint32_t target = ra;
        
        ctx->pc = target;
        return;
    }
}

// kernel_sub_001D1F10 (0x1d1f10): lift_d1's text (the reference)
void kernel_sub_001D1F10(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t ra = Ps2GprU32(ctx, 31);
    {
        const uint32_t target = ra;
        
        ctx->pc = target;
        return;
    }
}

// kernel_sub_001D1F18 (0x1d1f18): lift_d1's text (the reference)
void kernel_sub_001D1F18(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t ra = Ps2GprU32(ctx, 31);
    {
        const uint32_t target = ra;
        
        ctx->pc = target;
        return;
    }
}

// kernel_sub_001D1F20 (0x1d1f20): lift_d1's text (the reference)
void kernel_sub_001D1F20(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t ra = Ps2GprU32(ctx, 31);
    {
        const uint32_t target = ra;
        
        ctx->pc = target;
        return;
    }
}

// kernel_sub_001D2008 (0x1d2008): lift_d1's text (the reference)
void kernel_sub_001D2008(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t ra = Ps2GprU32(ctx, 31);
    {
        const uint32_t target = ra;
        
        ctx->pc = target;
        return;
    }
}

// Sg2RemoteDmaTransStatus (0x1d2150): lift_d1's text (the reference)
void Sg2RemoteDmaTransStatus(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t v0 = Ps2GprU32(ctx, 2), v1 = Ps2GprU32(ctx, 3), a0 = Ps2GprU32(ctx, 4), ra = Ps2GprU32(ctx, 31);
    v1 = a0 << 2;
    v1 = v1 + a0;
    v1 = v1 << 2;
    v0 = 0x290000u;
    v0 = v0 + v1;
    v0 = READ32(v0 - 0x48B0u);
    {
        const uint32_t target = ra;
        Ps2SetGpr64(ctx, 2, sx64(v0)); Ps2SetGpr64(ctx, 3, sx64(v1));
        ctx->pc = target;
        return;
    }
}

// Sg2RemoteGetAdpStmAdr (0x1d2170): lift_d1's text (the reference)
void Sg2RemoteGetAdpStmAdr(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t v0 = Ps2GprU32(ctx, 2), a0 = Ps2GprU32(ctx, 4), ra = Ps2GprU32(ctx, 31);
    a0 = a0 << 2;
    v0 = 0x290000u;
    v0 = v0 + a0;
    v0 = READ32(v0 - 0x4A00u);
    {
        const uint32_t target = ra;
        Ps2SetGpr64(ctx, 2, sx64(v0)); Ps2SetGpr64(ctx, 4, sx64(a0));
        ctx->pc = target;
        return;
    }
}

// Sg2RemoteGetPcmStmTransOffset (0x1d2188): lift_d1's text (the reference)
void Sg2RemoteGetPcmStmTransOffset(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t v0 = Ps2GprU32(ctx, 2), v1 = Ps2GprU32(ctx, 3), ra = Ps2GprU32(ctx, 31);
    v1 = 0x290000u;
    {
        const uint32_t target = ra;
        v0 = READ32(v1 - 0x4940u);
        Ps2SetGpr64(ctx, 2, sx64(v0)); Ps2SetGpr64(ctx, 3, sx64(v1));
        ctx->pc = target;
        return;
    }
}

// Sg2RemoteGetSlotReleaseLv (0x1d2198): lift_d1's text (the reference)
void Sg2RemoteGetSlotReleaseLv(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t v0 = Ps2GprU32(ctx, 2), a0 = Ps2GprU32(ctx, 4), ra = Ps2GprU32(ctx, 31);
    a0 = a0 << 2;
    v0 = 0x290000u;
    v0 = v0 + a0;
    v0 = READ32(v0 - 0x4AC0u);
    {
        const uint32_t target = ra;
        Ps2SetGpr64(ctx, 2, sx64(v0)); Ps2SetGpr64(ctx, 4, sx64(a0));
        ctx->pc = target;
        return;
    }
}

// Sg2RemoteGetDspChangeStatus (0x1d21b0): lift_d1's text (the reference)
void Sg2RemoteGetDspChangeStatus(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t v0 = Ps2GprU32(ctx, 2), a0 = Ps2GprU32(ctx, 4), ra = Ps2GprU32(ctx, 31);
    a0 = a0 << 2;
    v0 = 0x290000u;
    v0 = v0 + a0;
    v0 = READ32(v0 - 0x4938u);
    {
        const uint32_t target = ra;
        Ps2SetGpr64(ctx, 2, sx64(v0)); Ps2SetGpr64(ctx, 4, sx64(a0));
        ctx->pc = target;
        return;
    }
}

// Sg2SepStatus (0x1d2b98): lift_d1's text (the reference)
void Sg2SepStatus(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t v0 = Ps2GprU32(ctx, 2), ra = Ps2GprU32(ctx, 31);
    uint64_t v1 = Ps2GprU64(ctx, 3), a0 = Ps2GprU64(ctx, 4);
    (v0 = (uint32_t)((int32_t)((int32_t)SLL32((uint32_t)a0, 2))));
    (v1 = (uint64_t)(((uint64_t)a0 < (uint64_t)(int64_t)(int32_t)32) ? 1 : 0));
    {
        const bool c = (int64_t)v1 == 0;
        (v0 = (uint32_t)((int32_t)((int32_t)ADD32(v0, (uint32_t)a0))));
        if (!c) {
            (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)SLL32(v0, 4))));
            v0 = 0x290000u;
            (v0 = (uint32_t)((int32_t)((int32_t)ADD32(v0, (uint32_t)v1))));
            v0 = READ32(v0 - 0x4640u);
            {
                const uint32_t target = ra;
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

// kernel_sub_001D3428 (0x1d3428): lift_d1's text (the reference)
void kernel_sub_001D3428(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t ra = Ps2GprU32(ctx, 31);
    {
        const uint32_t target = ra;
        
        ctx->pc = target;
        return;
    }
}

// kernel_sub_001D3430 (0x1d3430): lift_d1's text (the reference)
void kernel_sub_001D3430(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t at = Ps2GprU32(ctx, 1), v0 = Ps2GprU32(ctx, 2), v1 = Ps2GprU32(ctx, 3), a0 = Ps2GprU32(ctx, 4), a1 = Ps2GprU32(ctx, 5), a2 = Ps2GprU32(ctx, 6), a3 = Ps2GprU32(ctx, 7), t0 = Ps2GprU32(ctx, 8), t1 = Ps2GprU32(ctx, 9), t2 = Ps2GprU32(ctx, 10), t3 = Ps2GprU32(ctx, 11), ra = Ps2GprU32(ctx, 31);
    bool df_at = false;
    a1 = a0 + 0x40u;
    v1 = 0x290000u;
    v0 = READ32(a1);
    t0 = v1 - 0x3B2Cu;
    a3 = READ8_B(a0, 72);
    t3 = READ8(v0);
    v0 = v0 + 1u;
    WRITE32(a1, v0);
    v1 = v0 + 1u;
    a0 = v0 + 2u;
    a2 = READ32(t0);
    t2 = READ8(v0);
    WRITE32(a1, v1);
    t1 = READ8(v1);
    {
        const bool c = a2 == a3;
        WRITE32(a1, a0);
        if (!c) {
            WRITE32(t0, a3);
            a2 = 0x290000u;
            v1 = a2 - 0x3B30u;
            a0 = 0x290000u;
            v0 = READ32(v1);
            at = 0x290000u; df_at = true;
            at = at + v0; df_at = true;
            WRITE8(at - 0x3C30u, (uint8_t)a3);
            v0 = v0 + 1u;
            WRITE32(v1, v0);
            goto L_1d34a8;
        }
    }
    a0 = 0x290000u;
    a2 = 0x290000u;
L_1d34a8:
    a2 = a2 - 0x3B30u;
    v1 = a0 - 0x3C30u;
    v0 = READ32(a2);
    a1 = v0 + v1;
    v0 = v0 + 1u;
    a3 = v0 + v1;
    a0 = v0 + 2u;
    v0 = a3;
    WRITE8(a1, (uint8_t)t3);
    WRITE32(a2, a0);
    WRITE8(v0, (uint8_t)t2);
    {
        const uint32_t target = ra;
        WRITE8_B(a3, 1, (uint8_t)t1);
        if (df_at) Ps2SetGpr64(ctx, 1, sx64(at)); Ps2SetGpr64(ctx, 10, sx64(t2)); Ps2SetGpr64(ctx, 11, sx64(t3)); Ps2SetGpr64(ctx, 2, sx64(v0)); Ps2SetGpr64(ctx, 3, sx64(v1)); Ps2SetGpr64(ctx, 4, sx64(a0)); Ps2SetGpr64(ctx, 5, sx64(a1)); Ps2SetGpr64(ctx, 6, sx64(a2)); Ps2SetGpr64(ctx, 7, sx64(a3)); Ps2SetGpr64(ctx, 8, sx64(t0)); Ps2SetGpr64(ctx, 9, sx64(t1));
        ctx->pc = target;
        return;
    }
}

// kernel_sub_001D3A48 (0x1d3a48): lift_d1's text (the reference)
void kernel_sub_001D3A48(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t at = Ps2GprU32(ctx, 1), v0 = Ps2GprU32(ctx, 2), v1 = Ps2GprU32(ctx, 3), a1 = Ps2GprU32(ctx, 5), a2 = Ps2GprU32(ctx, 6), t0 = Ps2GprU32(ctx, 8), t1 = Ps2GprU32(ctx, 9), ra = Ps2GprU32(ctx, 31);
    uint64_t a0 = Ps2GprU64(ctx, 4), a3 = Ps2GprU64(ctx, 7);
    bool df_at = false, df_v1 = false, df_t0 = false, df_t1 = false;
    (a3 = (uint64_t)((uint64_t)a0 + (uint64_t)0ULL));
    v0 = 0xFFu;
    (a2 = (uint32_t)((int32_t)((int32_t)READ32_B((uint32_t)a3, 72))));
    {
        const bool c = a2 != v0;
        (a1 = (uint32_t)((int32_t)((int32_t)ADD32((uint32_t)a3, 64))));
        if (!c) {
            v0 = READ32(a1);
            (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 47))));
            t0 = READ8(v0); df_t0 = true;
            v0 = v0 + 1u;
            WRITE32(a1, v0);
            v1 = v0 + 1u; df_v1 = true;
            t1 = READ8(v0); df_t1 = true;
            {
                const bool c = (int64_t)(int32_t)t0 != (int64_t)a0;
                WRITE32(a1, v1);
                if (!c) {
                    v0 = 0x290000u;
                    WRITE32(a1, 0u);
                    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(v0, 4294952148))));
                    v1 = 2u; df_v1 = true;
                    (v0 = (uint32_t)((int32_t)((int32_t)READ32_B((uint32_t)a0, 0))));
                    t0 = 0x3Fu; df_t0 = true;
                    {
                        const bool c = v0 == a2;
                        WRITE32_B((uint32_t)a3, 0, v1);
                        if (!c) {
                            WRITE32_B((uint32_t)a0, 0, a2);
                            (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)41 << 16))));
                            a1 = 0x290000u;
                            v1 = a1 - 0x3B30u; df_v1 = true;
                            v0 = READ32(v1);
                            at = 0x290000u; df_at = true;
                            at = at + v0; df_at = true;
                            WRITE8(at - 0x3C30u, (uint8_t)a2);
                            v0 = v0 + 1u;
                            WRITE32(v1, v0);
                            goto L_1d3ad8;
                        }
                    }
                    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)41 << 16))));
                    a1 = 0x290000u;
L_1d3ad8:
                    a1 = a1 - 0x3B30u;
                    (v1 = (uint32_t)((int32_t)((int32_t)ADD32((uint32_t)a0, 4294951888)))); df_v1 = true;
                    v0 = READ32(a1);
                    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(v0, v1))));
                    v0 = v0 + 1u;
                    v1 = v0 + v1; df_v1 = true;
                    v0 = v0 + 1u;
                    WRITE8_B((uint32_t)a0, 0, (uint8_t)t0);
                    WRITE32(a1, v0);
                    WRITE8(v1, (uint8_t)t1);
                }
            }
        }
    }
    {
        const uint32_t target = ra;
        if (df_at) Ps2SetGpr64(ctx, 1, sx64(at)); Ps2SetGpr64(ctx, 2, sx64(v0)); if (df_v1) Ps2SetGpr64(ctx, 3, sx64(v1)); Ps2SetGpr64(ctx, 4, a0); Ps2SetGpr64(ctx, 5, sx64(a1)); Ps2SetGpr64(ctx, 6, sx64(a2)); Ps2SetGpr64(ctx, 7, a3); if (df_t0) Ps2SetGpr64(ctx, 8, sx64(t0)); if (df_t1) Ps2SetGpr64(ctx, 9, sx64(t1));
        ctx->pc = target;
        return;
    }
}

// Sg2SlotFree (0x1d5d28): lift_d1's text (the reference)
void Sg2SlotFree(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t v0 = Ps2GprU32(ctx, 2), v1 = Ps2GprU32(ctx, 3), a0 = Ps2GprU32(ctx, 4), ra = Ps2GprU32(ctx, 31);
    v1 = a0 << 3;
    v0 = 0x290000u;
    v1 = v1 - a0;
    v0 = v0 - 0x39D0u;
    v1 = v1 << 5;
    v0 = v0 + 4u;
    v1 = v1 + v0;
    a0 = READ32(v1);
    if ((int32_t)a0 > 0) {
        WRITE32(v1, 0u);
    } else {
    }
    {
        const uint32_t target = ra;
        Ps2SetGpr64(ctx, 2, sx64(v0)); Ps2SetGpr64(ctx, 3, sx64(v1)); Ps2SetGpr64(ctx, 4, sx64(a0));
        ctx->pc = target;
        return;
    }
}

// Sg2SlotSetGroup (0x1d6250): lift_d1's text (the reference)
void Sg2SlotSetGroup(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t at = Ps2GprU32(ctx, 1), v0 = Ps2GprU32(ctx, 2), a0 = Ps2GprU32(ctx, 4), a1 = Ps2GprU32(ctx, 5), ra = Ps2GprU32(ctx, 31);
    v0 = a0 << 3;
    v0 = v0 - a0;
    v0 = v0 << 5;
    at = 0x290000u;
    at = at + v0;
    WRITE32(at - 0x39B4u, a1);
    {
        const uint32_t target = ra;
        Ps2SetGpr64(ctx, 1, sx64(at)); Ps2SetGpr64(ctx, 2, sx64(v0));
        ctx->pc = target;
        return;
    }
}

// Sg2SlotSetMute (0x1d6270): lift_d1's text (the reference)
void Sg2SlotSetMute(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t at = Ps2GprU32(ctx, 1), v1 = Ps2GprU32(ctx, 3), a3 = Ps2GprU32(ctx, 7), ra = Ps2GprU32(ctx, 31);
    uint64_t v0 = Ps2GprU64(ctx, 2), a0 = Ps2GprU64(ctx, 4), a1 = Ps2GprU64(ctx, 5), a2 = Ps2GprU64(ctx, 6);
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
            (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)READ32_B(v1, 0))));
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
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)READ32_B(v1, 0))));
    (v0 = (uint64_t)(RC_U64X(v0) & RC_U64X(a0)));
L_1d62cc:
    WRITE32_B(v1, 0, (uint32_t)v0);
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)SUB32((uint32_t)a1, (uint32_t)a2))));
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(a3, 4294952496))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)SLL32((uint32_t)v0, 5))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v0, (uint32_t)a0))));
    (v1 = (uint32_t)((int32_t)((int32_t)READ32_B((uint32_t)v0, 0))));
    (v1 = (uint32_t)(sx64(v1) | (uint64_t)(uint16_t)4096));
    {
        const uint32_t target = ra;
        WRITE32_B((uint32_t)v0, 0, v1);
        Ps2SetGpr64(ctx, 1, sx64(at)); Ps2SetGpr64(ctx, 2, v0); Ps2SetGpr64(ctx, 3, sx64(v1)); Ps2SetGpr64(ctx, 4, a0); Ps2SetGpr64(ctx, 5, a1); Ps2SetGpr64(ctx, 6, a2); Ps2SetGpr64(ctx, 7, sx64(a3));
        ctx->pc = target;
        return;
    }
}

// Sg2SlotSetBendRange (0x1d6528): lift_d1's text (the reference)
void Sg2SlotSetBendRange(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t at = Ps2GprU32(ctx, 1), v0 = Ps2GprU32(ctx, 2), a0 = Ps2GprU32(ctx, 4), a1 = Ps2GprU32(ctx, 5), ra = Ps2GprU32(ctx, 31);
    v0 = a0 << 3;
    v0 = v0 - a0;
    v0 = v0 << 5;
    at = 0x290000u;
    at = at + v0;
    WRITE32(at - 0x3978u, a1);
    {
        const uint32_t target = ra;
        Ps2SetGpr64(ctx, 1, sx64(at)); Ps2SetGpr64(ctx, 2, sx64(v0));
        ctx->pc = target;
        return;
    }
}

// Sg2SlotSetBendValue (0x1d6548): lift_d1's text (the reference)
void Sg2SlotSetBendValue(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t v0 = Ps2GprU32(ctx, 2), v1 = Ps2GprU32(ctx, 3), a0 = Ps2GprU32(ctx, 4), a1 = Ps2GprU32(ctx, 5), ra = Ps2GprU32(ctx, 31);
    v1 = a0 << 3;
    v0 = 0x290000u;
    v1 = v1 - a0;
    v0 = v0 - 0x39D0u;
    v1 = v1 << 5;
    a0 = v1 + v0;
    WRITE32_B(a0, 92, a1);
    v1 = a0;
    v0 = READ32(v1);
    v0 = v0 | 0x80u;
    {
        const uint32_t target = ra;
        WRITE32(v1, v0);
        Ps2SetGpr64(ctx, 2, sx64(v0)); Ps2SetGpr64(ctx, 3, sx64(v1)); Ps2SetGpr64(ctx, 4, sx64(a0));
        ctx->pc = target;
        return;
    }
}

// Sg2SlotSetPause (0x1d65f0): lift_d1's text (the reference)
void Sg2SlotSetPause(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t at = Ps2GprU32(ctx, 1), v1 = Ps2GprU32(ctx, 3), a3 = Ps2GprU32(ctx, 7), ra = Ps2GprU32(ctx, 31);
    uint64_t v0 = Ps2GprU64(ctx, 2), a0 = Ps2GprU64(ctx, 4), a1 = Ps2GprU64(ctx, 5), a2 = Ps2GprU64(ctx, 6);
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
            (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)READ32_B(v1, 0))));
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
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)READ32_B(v1, 0))));
    (v0 = (uint64_t)(RC_U64X(v0) & RC_U64X(a0)));
L_1d6648:
    WRITE32_B(v1, 0, (uint32_t)v0);
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)SUB32((uint32_t)a1, (uint32_t)a2))));
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(a3, 4294952496))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)SLL32((uint32_t)v0, 5))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v0, (uint32_t)a0))));
    (v1 = (uint32_t)((int32_t)((int32_t)READ32_B((uint32_t)v0, 0))));
    v1 = v1 | 0x80u;
    {
        const uint32_t target = ra;
        WRITE32_B((uint32_t)v0, 0, v1);
        Ps2SetGpr64(ctx, 1, sx64(at)); Ps2SetGpr64(ctx, 2, v0); Ps2SetGpr64(ctx, 3, sx64(v1)); Ps2SetGpr64(ctx, 4, a0); Ps2SetGpr64(ctx, 5, a1); Ps2SetGpr64(ctx, 6, a2); Ps2SetGpr64(ctx, 7, sx64(a3));
        ctx->pc = target;
        return;
    }
}

// Sg2SlotSetDirectPitch (0x1d6670): lift_d1's text (the reference)
void Sg2SlotSetDirectPitch(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t v0 = Ps2GprU32(ctx, 2), v1 = Ps2GprU32(ctx, 3), a0 = Ps2GprU32(ctx, 4), a1 = Ps2GprU32(ctx, 5), ra = Ps2GprU32(ctx, 31);
    v1 = a0 << 3;
    v0 = 0x290000u;
    v1 = v1 - a0;
    v0 = v0 - 0x39D0u;
    v1 = v1 << 5;
    a0 = v1 + v0;
    WRITE32_B(a0, 112, a1);
    v1 = a0;
    v0 = READ32(v1);
    v0 = v0 | 0x80u;
    {
        const uint32_t target = ra;
        WRITE32(v1, v0);
        Ps2SetGpr64(ctx, 2, sx64(v0)); Ps2SetGpr64(ctx, 3, sx64(v1)); Ps2SetGpr64(ctx, 4, sx64(a0));
        ctx->pc = target;
        return;
    }
}

// Sg2SlotSetLoop (0x1d6758): lift_d1's text (the reference)
void Sg2SlotSetLoop(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t at = Ps2GprU32(ctx, 1), v0 = Ps2GprU32(ctx, 2), v1 = Ps2GprU32(ctx, 3), a0 = Ps2GprU32(ctx, 4), ra = Ps2GprU32(ctx, 31);
    v0 = a0 << 3;
    v0 = v0 - a0;
    v0 = v0 << 5;
    at = 0x290000u;
    at = at - 0x39D0u;
    v0 = at + v0;
    v1 = READ32(v0);
    v1 = v1 | 4u;
    {
        const uint32_t target = ra;
        WRITE32(v0, v1);
        Ps2SetGpr64(ctx, 1, sx64(at)); Ps2SetGpr64(ctx, 2, sx64(v0)); Ps2SetGpr64(ctx, 3, sx64(v1));
        ctx->pc = target;
        return;
    }
}

// Sg2SlotSetDumperOn (0x1d6780): lift_d1's text (the reference)
void Sg2SlotSetDumperOn(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t at = Ps2GprU32(ctx, 1), v0 = Ps2GprU32(ctx, 2), v1 = Ps2GprU32(ctx, 3), a0 = Ps2GprU32(ctx, 4), ra = Ps2GprU32(ctx, 31);
    v0 = a0 << 3;
    v0 = v0 - a0;
    v0 = v0 << 5;
    at = 0x290000u;
    at = at - 0x39D0u;
    v0 = at + v0;
    v1 = READ32(v0);
    v1 = v1 | 8u;
    {
        const uint32_t target = ra;
        WRITE32(v0, v1);
        Ps2SetGpr64(ctx, 1, sx64(at)); Ps2SetGpr64(ctx, 2, sx64(v0)); Ps2SetGpr64(ctx, 3, sx64(v1));
        ctx->pc = target;
        return;
    }
}

// Sg2SlotSetLfoVolTbl (0x1d6950): lift_d1's text (the reference)
void Sg2SlotSetLfoVolTbl(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t v0 = Ps2GprU32(ctx, 2), v1 = Ps2GprU32(ctx, 3), a0 = Ps2GprU32(ctx, 4), a3 = Ps2GprU32(ctx, 7), ra = Ps2GprU32(ctx, 31);
    uint64_t a1 = Ps2GprU64(ctx, 5), a2 = Ps2GprU64(ctx, 6);
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
            WRITE32_B(a0, 156, (uint32_t)a2);
            WRITE32_B(v1, 0, (uint32_t)a2);
            WRITE32_B((uint32_t)a1, 4, a3);
        }
    }
    {
        const uint32_t target = ra;
        Ps2SetGpr64(ctx, 2, sx64(v0)); if (df_v1) Ps2SetGpr64(ctx, 3, sx64(v1)); if (df_a0) Ps2SetGpr64(ctx, 4, sx64(a0)); Ps2SetGpr64(ctx, 5, a1); Ps2SetGpr64(ctx, 6, a2); Ps2SetGpr64(ctx, 7, sx64(a3));
        ctx->pc = target;
        return;
    }
}

// Sg2SlotSetTimeVolParam (0x1d6c30): lift_d1's text (the reference)
void Sg2SlotSetTimeVolParam(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t v0 = Ps2GprU32(ctx, 2), v1 = Ps2GprU32(ctx, 3), a0 = Ps2GprU32(ctx, 4), a1 = Ps2GprU32(ctx, 5), a2 = Ps2GprU32(ctx, 6), ra = Ps2GprU32(ctx, 31);
    float f12 = ctx->f[12], f13 = ctx->f[13];
    v0 = a0 << 3;
    v1 = 0x290000u;
    v0 = v0 - a0;
    v1 = v1 - 0x39D0u;
    v0 = v0 << 5;
    a1 = v0 + v1;
    v0 = v0 + 0xA0u;
    a0 = READ32(a1);
    a2 = v1 + v0;
    v1 = a2;
    a0 = a0 | 0x20u;
    WRITE32_B(v1, 12, lift_d1::bits(f12));
    WRITE32(a1, a0);
    {
        const uint32_t target = ra;
        WRITE32_B(a2, 8, lift_d1::bits(f13));
        Ps2SetGpr64(ctx, 2, sx64(v0)); Ps2SetGpr64(ctx, 3, sx64(v1)); Ps2SetGpr64(ctx, 4, sx64(a0)); Ps2SetGpr64(ctx, 5, sx64(a1)); Ps2SetGpr64(ctx, 6, sx64(a2));
        ctx->pc = target;
        return;
    }
}

// Sg2SlotSetTimePitchParam (0x1d6c70): lift_d1's text (the reference)
void Sg2SlotSetTimePitchParam(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t v0 = Ps2GprU32(ctx, 2), v1 = Ps2GprU32(ctx, 3), a0 = Ps2GprU32(ctx, 4), a1 = Ps2GprU32(ctx, 5), a2 = Ps2GprU32(ctx, 6), ra = Ps2GprU32(ctx, 31);
    float f12 = ctx->f[12], f13 = ctx->f[13];
    v0 = a0 << 3;
    v1 = 0x290000u;
    v0 = v0 - a0;
    v1 = v1 - 0x39D0u;
    v0 = v0 << 5;
    a1 = v0 + v1;
    v0 = v0 + 0xC0u;
    a0 = READ32(a1);
    a2 = v0 + v1;
    v1 = v1 + v0;
    a0 = a0 | 0x10u;
    WRITE32_B(v1, 4, lift_d1::bits(f12));
    WRITE32(a1, a0);
    {
        const uint32_t target = ra;
        WRITE32(a2, lift_d1::bits(f13));
        Ps2SetGpr64(ctx, 2, sx64(v0)); Ps2SetGpr64(ctx, 3, sx64(v1)); Ps2SetGpr64(ctx, 4, sx64(a0)); Ps2SetGpr64(ctx, 5, sx64(a1)); Ps2SetGpr64(ctx, 6, sx64(a2));
        ctx->pc = target;
        return;
    }
}

// Sg2SlotSetTimePanParam (0x1d6cb0): lift_d1's text (the reference)
void Sg2SlotSetTimePanParam(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t v0 = Ps2GprU32(ctx, 2), v1 = Ps2GprU32(ctx, 3), a0 = Ps2GprU32(ctx, 4), a1 = Ps2GprU32(ctx, 5), a2 = Ps2GprU32(ctx, 6), ra = Ps2GprU32(ctx, 31);
    float f12 = ctx->f[12], f13 = ctx->f[13];
    v0 = a0 << 3;
    v1 = 0x290000u;
    v0 = v0 - a0;
    v1 = v1 - 0x39D0u;
    v0 = v0 << 5;
    a1 = v0 + v1;
    v0 = v0 + 0xB0u;
    a0 = READ32(a1);
    a2 = v1 + v0;
    v1 = a2;
    a0 = a0 | 0x40u;
    WRITE32_B(v1, 8, lift_d1::bits(f12));
    WRITE32(a1, a0);
    {
        const uint32_t target = ra;
        WRITE32_B(a2, 4, lift_d1::bits(f13));
        Ps2SetGpr64(ctx, 2, sx64(v0)); Ps2SetGpr64(ctx, 3, sx64(v1)); Ps2SetGpr64(ctx, 4, sx64(a0)); Ps2SetGpr64(ctx, 5, sx64(a1)); Ps2SetGpr64(ctx, 6, sx64(a2));
        ctx->pc = target;
        return;
    }
}

// Sg2SlotGetVol (0x1d6d50): lift_d1's text (the reference)
void Sg2SlotGetVol(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t v0 = Ps2GprU32(ctx, 2), v1 = Ps2GprU32(ctx, 3), a0 = Ps2GprU32(ctx, 4), ra = Ps2GprU32(ctx, 31);
    v1 = a0 << 3;
    v1 = v1 - a0;
    v1 = v1 << 5;
    v0 = 0x290000u;
    v0 = v0 + v1;
    v0 = READ32(v0 - 0x3988u);
    {
        const uint32_t target = ra;
        Ps2SetGpr64(ctx, 2, sx64(v0)); Ps2SetGpr64(ctx, 3, sx64(v1));
        ctx->pc = target;
        return;
    }
}

// Sg2SlotGetPan (0x1d6d70): lift_d1's text (the reference)
void Sg2SlotGetPan(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t v0 = Ps2GprU32(ctx, 2), v1 = Ps2GprU32(ctx, 3), a0 = Ps2GprU32(ctx, 4), ra = Ps2GprU32(ctx, 31);
    v1 = a0 << 3;
    v1 = v1 - a0;
    v1 = v1 << 5;
    v0 = 0x290000u;
    v0 = v0 + v1;
    v0 = READ32(v0 - 0x397Cu);
    {
        const uint32_t target = ra;
        Ps2SetGpr64(ctx, 2, sx64(v0)); Ps2SetGpr64(ctx, 3, sx64(v1));
        ctx->pc = target;
        return;
    }
}

// Sg2SlotSetOutputMode (0x1d7458): lift_d1's text (the reference)
void Sg2SlotSetOutputMode(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t v0 = Ps2GprU32(ctx, 2), v1 = Ps2GprU32(ctx, 3), a0 = Ps2GprU32(ctx, 4), ra = Ps2GprU32(ctx, 31);
    v1 = 0x290000u;
    v1 = v1 - 0x39D8u;
    v0 = READ32(v1);
    {
        const uint32_t target = ra;
        WRITE32(v1, a0);
        Ps2SetGpr64(ctx, 2, sx64(v0)); Ps2SetGpr64(ctx, 3, sx64(v1));
        ctx->pc = target;
        return;
    }
}

// Sg2SlotSetVolShiftDownParam (0x1d7470): lift_d1's text (the reference)
void Sg2SlotSetVolShiftDownParam(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t v0 = Ps2GprU32(ctx, 2), v1 = Ps2GprU32(ctx, 3), a0 = Ps2GprU32(ctx, 4), ra = Ps2GprU32(ctx, 31);
    v1 = 0x290000u;
    v1 = v1 - 0x39D4u;
    v0 = READ32(v1);
    {
        const uint32_t target = ra;
        WRITE32(v1, a0);
        Ps2SetGpr64(ctx, 2, sx64(v0)); Ps2SetGpr64(ctx, 3, sx64(v1));
        ctx->pc = target;
        return;
    }
}

// Sg2SlotGetVolShiftDownParam (0x1d7488): lift_d1's text (the reference)
void Sg2SlotGetVolShiftDownParam(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t v0 = Ps2GprU32(ctx, 2), v1 = Ps2GprU32(ctx, 3), ra = Ps2GprU32(ctx, 31);
    v1 = 0x290000u;
    {
        const uint32_t target = ra;
        v0 = READ32(v1 - 0x39D4u);
        Ps2SetGpr64(ctx, 2, sx64(v0)); Ps2SetGpr64(ctx, 3, sx64(v1));
        ctx->pc = target;
        return;
    }
}

// kernel_sub_001D8250 (0x1d8250): lift_d1's text (the reference)
void kernel_sub_001D8250(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t v1 = Ps2GprU32(ctx, 3), a0 = Ps2GprU32(ctx, 4), ra = Ps2GprU32(ctx, 31);
    uint64_t v0 = Ps2GprU64(ctx, 2);
    v1 = READ32_B(a0, 96);
    (v0 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL));
    v1 = v1 + 8u;
    {
        const uint32_t target = ra;
        WRITE32_B(a0, 96, v1);
        Ps2SetGpr64(ctx, 2, v0); Ps2SetGpr64(ctx, 3, sx64(v1));
        ctx->pc = target;
        return;
    }
}

// Sg2SmfpStatus (0x1d88f8): lift_d1's text (the reference)
void Sg2SmfpStatus(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t at = Ps2GprU32(ctx, 1), v0 = Ps2GprU32(ctx, 2), ra = Ps2GprU32(ctx, 31);
    uint64_t a0 = Ps2GprU64(ctx, 4);
    {
        const bool c = (int32_t)a0 < 0;
        (v0 = (uint32_t)((int32_t)((int32_t)SLL32((uint32_t)a0, 7))));
        if (!c) {
            at = 0x290000u;
            at = at + v0;
            v0 = READ32(at - 0xF80u);
            {
                const uint32_t target = ra;
                Ps2SetGpr64(ctx, 1, sx64(at)); Ps2SetGpr64(ctx, 2, sx64(v0));
                ctx->pc = target;
                return;
            }
        }
    }
    {
        const uint32_t target = ra;
        v0 = 1u;
        Ps2SetGpr64(ctx, 2, sx64(v0));
        ctx->pc = target;
        return;
    }
}

// Sg2SmfpSetSignal (0x1d8e78): lift_d1's text (the reference)
void Sg2SmfpSetSignal(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t v0 = Ps2GprU32(ctx, 2), a0 = Ps2GprU32(ctx, 4), ra = Ps2GprU32(ctx, 31);
    v0 = 0x290000u;
    {
        const uint32_t target = ra;
        WRITE32(v0 - 0xB78u, a0);
        Ps2SetGpr64(ctx, 2, sx64(v0));
        ctx->pc = target;
        return;
    }
}

// kernel_sub_001D95C8 (0x1d95c8): lift_d1's text (the reference)
void kernel_sub_001D95C8(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t ra = Ps2GprU32(ctx, 31);
    {
        const uint32_t target = ra;
        
        ctx->pc = target;
        return;
    }
}

// kernel_sub_001D95D0 (0x1d95d0): lift_d1's text (the reference)
void kernel_sub_001D95D0(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t at = Ps2GprU32(ctx, 1), v0 = Ps2GprU32(ctx, 2), v1 = Ps2GprU32(ctx, 3), a0 = Ps2GprU32(ctx, 4), a1 = Ps2GprU32(ctx, 5), a2 = Ps2GprU32(ctx, 6), a3 = Ps2GprU32(ctx, 7), t0 = Ps2GprU32(ctx, 8), t1 = Ps2GprU32(ctx, 9), ra = Ps2GprU32(ctx, 31);
    bool df_at = false;
    a1 = a0 + 0x60u;
    v1 = 0x290000u;
    v0 = READ32(a1);
    a3 = v1 - 0x36Cu;
    a2 = READ32(a3);
    t1 = READ8(v0);
    v0 = v0 + 1u;
    WRITE32(a1, v0);
    v1 = v0 + 1u;
    t0 = READ8(v0);
    WRITE32(a1, v1);
    a0 = READ8_B(a0, 104);
    {
        const bool c = a2 == a0;
        a2 = 0x290000u;
        if (!c) {
            WRITE32(a3, a0);
            a1 = 0x290000u;
            v1 = a1 - 0x370u;
            v0 = READ32(v1);
            at = 0x290000u; df_at = true;
            at = at + v0; df_at = true;
            WRITE8(at - 0xB70u, (uint8_t)a0);
            v0 = v0 + 1u;
            WRITE32(v1, v0);
            goto L_1d9634;
        }
    }
    a1 = 0x290000u;
L_1d9634:
    a1 = a1 - 0x370u;
    v1 = a2 - 0xB70u;
    v0 = READ32(a1);
    a0 = v0 + v1;
    v0 = v0 + 1u;
    v1 = v0 + v1;
    v0 = v0 + 1u;
    WRITE8(a0, (uint8_t)t1);
    WRITE32(a1, v0);
    {
        const uint32_t target = ra;
        WRITE8(v1, (uint8_t)t0);
        if (df_at) Ps2SetGpr64(ctx, 1, sx64(at)); Ps2SetGpr64(ctx, 2, sx64(v0)); Ps2SetGpr64(ctx, 3, sx64(v1)); Ps2SetGpr64(ctx, 4, sx64(a0)); Ps2SetGpr64(ctx, 5, sx64(a1)); Ps2SetGpr64(ctx, 6, sx64(a2)); Ps2SetGpr64(ctx, 7, sx64(a3)); Ps2SetGpr64(ctx, 8, sx64(t0)); Ps2SetGpr64(ctx, 9, sx64(t1));
        ctx->pc = target;
        return;
    }
}

// kernel_sub_001D97F0 (0x1d97f0): lift_d1's text (the reference)
void kernel_sub_001D97F0(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t at = Ps2GprU32(ctx, 1), v0 = Ps2GprU32(ctx, 2), v1 = Ps2GprU32(ctx, 3), a0 = Ps2GprU32(ctx, 4), a1 = Ps2GprU32(ctx, 5), a2 = Ps2GprU32(ctx, 6), a3 = Ps2GprU32(ctx, 7), ra = Ps2GprU32(ctx, 31);
    bool df_at = false;
    a1 = a0 + 0x60u;
    v1 = 0x290000u;
    v0 = READ32(a1);
    a2 = v1 - 0x36Cu;
    v1 = READ32(a2);
    a3 = READ8(v0);
    v0 = v0 + 1u;
    WRITE32(a1, v0);
    a1 = READ8_B(a0, 104);
    if (v1 == a1) {
        a2 = 0x290000u;
    } else {
        WRITE32(a2, a1);
        a2 = 0x290000u;
        a0 = 0x290000u;
        v1 = a0 - 0x370u;
        v0 = READ32(v1);
        at = 0x290000u; df_at = true;
        at = at + v0; df_at = true;
        WRITE8(at - 0xB70u, (uint8_t)a1);
        v0 = v0 + 1u;
        WRITE32(v1, v0);
        goto L_1d984c;
    }
    a0 = 0x290000u;
L_1d984c:
    a0 = a0 - 0x370u;
    v1 = a2 - 0xB70u;
    v0 = READ32(a0);
    v1 = v0 + v1;
    v0 = v0 + 1u;
    WRITE8(v1, (uint8_t)a3);
    {
        const uint32_t target = ra;
        WRITE32(a0, v0);
        if (df_at) Ps2SetGpr64(ctx, 1, sx64(at)); Ps2SetGpr64(ctx, 2, sx64(v0)); Ps2SetGpr64(ctx, 3, sx64(v1)); Ps2SetGpr64(ctx, 4, sx64(a0)); Ps2SetGpr64(ctx, 5, sx64(a1)); Ps2SetGpr64(ctx, 6, sx64(a2)); Ps2SetGpr64(ctx, 7, sx64(a3));
        ctx->pc = target;
        return;
    }
}

// vab_check_alive (0x1da0a0): lift_d1's text (the reference)
void vab_check_alive(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t ra = Ps2GprU32(ctx, 31);
    uint64_t v0 = Ps2GprU64(ctx, 2), v1 = Ps2GprU64(ctx, 3), a0 = Ps2GprU64(ctx, 4);
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
                    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)READ32_B((uint32_t)v0, 1216))));
                    (v0 = (uint64_t)(((uint64_t)0ULL < (uint64_t)RC_U64X(v0)) ? 1 : 0));
                }
            }
        }
    }
    {
        const uint32_t target = ra;
        Ps2SetGpr64(ctx, 2, v0); Ps2SetGpr64(ctx, 3, v1);
        ctx->pc = target;
        return;
    }
}

// vab_get_info (0x1da0e0): lift_d1's text (the reference)
void vab_get_info(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t v1 = Ps2GprU32(ctx, 3), a0 = Ps2GprU32(ctx, 4), a1 = Ps2GprU32(ctx, 5), a2 = Ps2GprU32(ctx, 6), a3 = Ps2GprU32(ctx, 7), t0 = Ps2GprU32(ctx, 8), ra = Ps2GprU32(ctx, 31);
    uint64_t v0 = Ps2GprU64(ctx, 2);
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
    a0 = READ32_B(t0, 12);
    WRITE32(a1, a0);
    a1 = READ32(v1);
    {
        const uint32_t target = ra;
        WRITE32(a2, a1);
        Ps2SetGpr64(ctx, 2, v0); Ps2SetGpr64(ctx, 3, sx64(v1)); Ps2SetGpr64(ctx, 4, sx64(a0)); Ps2SetGpr64(ctx, 5, sx64(a1)); Ps2SetGpr64(ctx, 7, sx64(a3)); Ps2SetGpr64(ctx, 8, sx64(t0));
        ctx->pc = target;
        return;
    }
}

// vab_get_veltbl (0x1da348): lift_d1's text (the reference)
void vab_get_veltbl(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t v0 = Ps2GprU32(ctx, 2), v1 = Ps2GprU32(ctx, 3), a0 = Ps2GprU32(ctx, 4), a1 = Ps2GprU32(ctx, 5), a2 = Ps2GprU32(ctx, 6), ra = Ps2GprU32(ctx, 31);
    v0 = a0 << 1;
    v1 = 0x290000u;
    v0 = v0 + a0;
    v1 = v1 + 0x4C0u;
    v0 = v0 << 2;
    v0 = v0 - a0;
    v0 = v0 << 2;
    v0 = v0 + 0x10u;
    a0 = v1 + v0;
    a2 = READ32_B(a0, 8);
    v1 = a0;
    v0 = READ32_B(v1, 12);
    {
        const uint32_t target = ra;
        WRITE32(a1, a2);
        Ps2SetGpr64(ctx, 2, sx64(v0)); Ps2SetGpr64(ctx, 3, sx64(v1)); Ps2SetGpr64(ctx, 4, sx64(a0)); Ps2SetGpr64(ctx, 6, sx64(a2));
        ctx->pc = target;
        return;
    }
}

// vab_get_lfotbl_param (0x1da380): lift_d1's text (the reference)
void vab_get_lfotbl_param(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t v0 = Ps2GprU32(ctx, 2), v1 = Ps2GprU32(ctx, 3), a0 = Ps2GprU32(ctx, 4), a1 = Ps2GprU32(ctx, 5), a2 = Ps2GprU32(ctx, 6), a3 = Ps2GprU32(ctx, 7), t0 = Ps2GprU32(ctx, 8), ra = Ps2GprU32(ctx, 31);
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
    t0 = READ32_B(a0, 4);
    v1 = a0;
    a0 = READ32(a3);
    WRITE32(a2, t0);
    WRITE32(a1, a0);
    {
        const uint32_t target = ra;
        v0 = READ32_B(v1, 8);
        Ps2SetGpr64(ctx, 2, sx64(v0)); Ps2SetGpr64(ctx, 3, sx64(v1)); Ps2SetGpr64(ctx, 4, sx64(a0)); Ps2SetGpr64(ctx, 7, sx64(a3)); Ps2SetGpr64(ctx, 8, sx64(t0));
        ctx->pc = target;
        return;
    }
}

// vab_set_semastervol (0x1da3c8): lift_d1's text (the reference)
void vab_set_semastervol(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t at = Ps2GprU32(ctx, 1), v0 = Ps2GprU32(ctx, 2), v1 = Ps2GprU32(ctx, 3), a0 = Ps2GprU32(ctx, 4), a1 = Ps2GprU32(ctx, 5), ra = Ps2GprU32(ctx, 31);
    v1 = a0 << 1;
    v1 = v1 + a0;
    v1 = v1 << 2;
    v1 = v1 - a0;
    v1 = v1 << 2;
    at = 0x290000u;
    at = at + 0x4D0u;
    v1 = at + v1;
    v0 = READ32(v1);
    {
        const uint32_t target = ra;
        WRITE32(v1, a1);
        Ps2SetGpr64(ctx, 1, sx64(at)); Ps2SetGpr64(ctx, 2, sx64(v0)); Ps2SetGpr64(ctx, 3, sx64(v1));
        ctx->pc = target;
        return;
    }
}

// vab_get_semastervol (0x1da3f8): lift_d1's text (the reference)
void vab_get_semastervol(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t v0 = Ps2GprU32(ctx, 2), v1 = Ps2GprU32(ctx, 3), a0 = Ps2GprU32(ctx, 4), ra = Ps2GprU32(ctx, 31);
    v1 = a0 << 1;
    v1 = v1 + a0;
    v1 = v1 << 2;
    v1 = v1 - a0;
    v1 = v1 << 2;
    v0 = 0x290000u;
    v0 = v0 + v1;
    v0 = READ32_B(v0, 1232);
    {
        const uint32_t target = ra;
        Ps2SetGpr64(ctx, 2, sx64(v0)); Ps2SetGpr64(ctx, 3, sx64(v1));
        ctx->pc = target;
        return;
    }
}

// kernel_sub_001DB3C8 (0x1db3c8): lift_d1's text (the reference)
void kernel_sub_001DB3C8(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t v0 = Ps2GprU32(ctx, 2), v1 = Ps2GprU32(ctx, 3), a0 = Ps2GprU32(ctx, 4), ra = Ps2GprU32(ctx, 31);
    v0 = READ8(a0);
    v1 = 0x45u;
    if (v0 == v1) {
        v1 = READ8_B(a0, 1);
        v0 = 0x51u;
        if (v1 == v0) {
            v1 = READ8_B(a0, 2);
            v0 = 0x41u;
            if (v1 == v0) {
                v1 = READ8_B(a0, 3);
                v0 = 0x54u;
                if (v1 == v0) {
                    goto L_1db410;
                }
            }
        }
    }
    {
        const uint32_t target = ra;
        v0 = 0xFFFFFFFFu;
        Ps2SetGpr64(ctx, 2, sx64(v0)); Ps2SetGpr64(ctx, 3, sx64(v1));
        ctx->pc = target;
        return;
    }
L_1db410:
    {
        const uint32_t target = ra;
        v0 = READ32_B(a0, 4);
        Ps2SetGpr64(ctx, 2, sx64(v0)); Ps2SetGpr64(ctx, 3, sx64(v1));
        ctx->pc = target;
        return;
    }
}

// Sg2ExPcmInputGetLipFrame (0x1db9e8): lift_d1's text (the reference)
void Sg2ExPcmInputGetLipFrame(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t v0 = Ps2GprU32(ctx, 2), ra = Ps2GprU32(ctx, 31);
    uint64_t v1 = Ps2GprU64(ctx, 3), a0 = Ps2GprU64(ctx, 4);
    uint64_t hi = ctx->hi;
    uint64_t lo = ctx->lo;
    (v1 = (uint64_t)(((uint64_t)a0 < (uint64_t)(int64_t)(int32_t)4) ? 1 : 0));
    {
        const bool c = (int64_t)v1 == 0;
        v0 = 0xFFFFFFFFu;
        if (!c) {
            (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 468))));
            { int64_t result = (int64_t)(int32_t)a0 * (int64_t)(int32_t)v1; lo = (uint64_t)(int64_t)(int32_t)result; hi = (uint64_t)(int64_t)(int32_t)(result >> 32); (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)result))); }
            v0 = 0x290000u;
            (v0 = (uint32_t)((int32_t)((int32_t)ADD32(v0, (uint32_t)v1))));
            v0 = READ32_B(v0, 2028);
        }
    }
    {
        const uint32_t target = ra;
        Ps2SetGpr64(ctx, 2, sx64(v0)); Ps2SetGpr64(ctx, 3, v1); ctx->hi = hi; ctx->lo = lo;
        ctx->pc = target;
        return;
    }
}

// Sg2ExPcmInputSetOutputMode (0x1dbba0): lift_d1's text (the reference)
void Sg2ExPcmInputSetOutputMode(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t at = Ps2GprU32(ctx, 1), a1 = Ps2GprU32(ctx, 5), ra = Ps2GprU32(ctx, 31);
    uint64_t v0 = Ps2GprU64(ctx, 2), a0 = Ps2GprU64(ctx, 4);
    uint64_t hi = ctx->hi;
    uint64_t lo = ctx->lo;
    bool df_at = false;
    (v0 = (uint64_t)(((uint64_t)a0 < (uint64_t)(int64_t)(int32_t)4) ? 1 : 0));
    {
        const bool c = (int64_t)v0 == 0;
        (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 468))));
        if (!c) {
            { int64_t result = (int64_t)(int32_t)a0 * (int64_t)(int32_t)v0; lo = (uint64_t)(int64_t)(int32_t)result; hi = (uint64_t)(int64_t)(int32_t)(result >> 32); (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)result))); }
            at = 0x290000u; df_at = true;
            (at = (uint32_t)((int32_t)((int32_t)ADD32(at, (uint32_t)v0)))); df_at = true;
            WRITE32_B(at, 2432, a1);
        }
    }
    {
        const uint32_t target = ra;
        if (df_at) Ps2SetGpr64(ctx, 1, sx64(at)); Ps2SetGpr64(ctx, 2, v0); ctx->hi = hi; ctx->lo = lo;
        ctx->pc = target;
        return;
    }
}

// Sg2ExPcmInputAutoFilterCtrl (0x1dbd90): lift_d1's text (the reference)
void Sg2ExPcmInputAutoFilterCtrl(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t at = Ps2GprU32(ctx, 1), v0 = Ps2GprU32(ctx, 2), a1 = Ps2GprU32(ctx, 5), a2 = Ps2GprU32(ctx, 6), ra = Ps2GprU32(ctx, 31);
    uint64_t v1 = Ps2GprU64(ctx, 3), a0 = Ps2GprU64(ctx, 4);
    uint64_t hi = ctx->hi;
    uint64_t lo = ctx->lo;
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
            WRITE32_B(at, 2032, a2);
        }
    }
    {
        const uint32_t target = ra;
        if (df_at) Ps2SetGpr64(ctx, 1, sx64(at)); Ps2SetGpr64(ctx, 2, sx64(v0)); Ps2SetGpr64(ctx, 3, v1); ctx->hi = hi; ctx->lo = lo;
        ctx->pc = target;
        return;
    }
}

// Sg2ExPcmOutputSetOutputMode (0x1dbfb8): lift_d1's text (the reference)
void Sg2ExPcmOutputSetOutputMode(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t v0 = Ps2GprU32(ctx, 2), a0 = Ps2GprU32(ctx, 4), ra = Ps2GprU32(ctx, 31);
    v0 = 0x290000u;
    {
        const uint32_t target = ra;
        WRITE32_B(v0, 1568, a0);
        Ps2SetGpr64(ctx, 2, sx64(v0));
        ctx->pc = target;
        return;
    }
}

// Sg2ExPcmOutputGetLipFrame (0x1dc068): lift_d1's text (the reference)
void Sg2ExPcmOutputGetLipFrame(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t v0 = Ps2GprU32(ctx, 2), v1 = Ps2GprU32(ctx, 3), ra = Ps2GprU32(ctx, 31);
    v1 = 0x290000u;
    {
        const uint32_t target = ra;
        v0 = READ32_B(v1, 1592);
        Ps2SetGpr64(ctx, 2, sx64(v0)); Ps2SetGpr64(ctx, 3, sx64(v1));
        ctx->pc = target;
        return;
    }
}

// Sg2ExPcmOutputAutoFilterCtrl (0x1dc1d0): lift_d1's text (the reference)
void Sg2ExPcmOutputAutoFilterCtrl(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t at = Ps2GprU32(ctx, 1), v0 = Ps2GprU32(ctx, 2), a0 = Ps2GprU32(ctx, 4), a1 = Ps2GprU32(ctx, 5), ra = Ps2GprU32(ctx, 31);
    v0 = a0 << 1;
    v0 = v0 + a0;
    v0 = v0 << 4;
    v0 = v0 + a0;
    v0 = v0 << 2;
    at = 0x290000u;
    at = at + v0;
    WRITE32_B(at, 1596, a1);
    {
        const uint32_t target = ra;
        Ps2SetGpr64(ctx, 1, sx64(at)); Ps2SetGpr64(ctx, 2, sx64(v0));
        ctx->pc = target;
        return;
    }
}

// Sg2ExPcmSetSignal (0x1dc2d0): lift_d1's text (the reference)
void Sg2ExPcmSetSignal(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t v0 = Ps2GprU32(ctx, 2), a0 = Ps2GprU32(ctx, 4), ra = Ps2GprU32(ctx, 31);
    v0 = 0x290000u;
    {
        const uint32_t target = ra;
        WRITE32_B(v0, 3880, a0);
        Ps2SetGpr64(ctx, 2, sx64(v0));
        ctx->pc = target;
        return;
    }
}

// IirInit (0x1dca28): lift_d1's text (the reference)
void IirInit(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t a0 = Ps2GprU32(ctx, 4), ra = Ps2GprU32(ctx, 31);
    WRITE32_B(a0, 44, 0u);
    WRITE32_B(a0, 28, 0u);
    WRITE32_B(a0, 32, 0u);
    WRITE32_B(a0, 36, 0u);
    WRITE32_B(a0, 40, 0u);
    {
        const uint32_t target = ra;
        WRITE32_B(a0, 48, 0u);
        
        ctx->pc = target;
        return;
    }
}

// GAMEUTIL_is_inside_of_region_pitch (0x107d5c8): lift_d1's text (the reference)
void GAMEUTIL_is_inside_of_region_pitch(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t at = Ps2GprU32(ctx, 1), ra = Ps2GprU32(ctx, 31);
    uint64_t v0 = Ps2GprU64(ctx, 2), a0 = Ps2GprU64(ctx, 4), a1 = Ps2GprU64(ctx, 5);
    float f0 = ctx->f[0], f1 = ctx->f[1], f2 = ctx->f[2], f3 = ctx->f[3], f4 = ctx->f[4], f5 = ctx->f[5], f12 = ctx->f[12], f13 = ctx->f[13], f14 = ctx->f[14];
    uint32_t fcr31 = ctx->fcr31;
    at = 0x3F000000u;
    { uint32_t bits = at; std::memcpy(&f0, &bits, sizeof(bits)); }
    { uint32_t bits = 0u; std::memcpy(&f1, &bits, sizeof(bits)); }
    f14 = FPU_MUL_S(f14, f0);
    at = 0x38D10000u;
    at = at | 0xB717u;
    { uint32_t bits = at; std::memcpy(&f0, &bits, sizeof(bits)); }
    at = 0x3FBD0000u;
    at = at | 0xE44Eu;
    { uint32_t bits = at; std::memcpy(&f4, &bits, sizeof(bits)); }
    at = 0xBFBD0000u;
    at = at | 0xE44Eu;
    { uint32_t bits = at; std::memcpy(&f5, &bits, sizeof(bits)); }
    fcr31 = (FPU_C_OLT_S(f14, f1)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
    {
        const bool c = (fcr31 & 0x800000u) == 0u;
        f3 = FPU_MOV_S(f13);
        if (!c) {
            f2 = FPU_NEG_S(f14);
            fcr31 = (FPU_C_OLE_S(f2, f0)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
            if ((fcr31 & 0x800000u) != 0u) {
                (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 1))));
                goto L_107d63c;
            }
            (v0 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL));
            goto L_107d63c;
        }
    }
    fcr31 = (FPU_C_OLE_S(f14, f0)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
    {
        const bool c = (fcr31 & 0x800000u) == 0u;
        f2 = FPU_NEG_S(f14);
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
        fcr31 = (FPU_C_OLT_S(f0, f4)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
        if ((fcr31 & 0x800000u) != 0u) {
            f4 = FPU_MOV_S(f0);
            goto L_107d680;
        }
        at = 0xBFBD0000u;
        at = at | 0xE44Eu;
        { uint32_t bits = at; std::memcpy(&f0, &bits, sizeof(bits)); }
        fcr31 = (FPU_C_OLT_S(f5, f0)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
        if ((fcr31 & 0x800000u) != 0u) {
            f5 = FPU_MOV_S(f0);
        } else {
        }
L_107d680:
        fcr31 = (FPU_C_OLT_S(f4, f13)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
        if ((fcr31 & 0x800000u) != 0u) {
            { uint32_t bits = 0u; std::memcpy(&f0, &bits, sizeof(bits)); }
            fcr31 = (FPU_C_OLT_S(f14, f0)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
            goto L_107d6b4;
        }
        fcr31 = (FPU_C_OLT_S(f13, f5)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
        if ((fcr31 & 0x800000u) == 0u) {
            f3 = FPU_ADD_S(f13, f14);
            goto L_107d6c8;
        }
        { uint32_t bits = 0u; std::memcpy(&f0, &bits, sizeof(bits)); }
        fcr31 = (FPU_C_OLT_S(f0, f14)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
L_107d6b4:
        if ((fcr31 & 0x800000u) != 0u) {
            f3 = FPU_ADD_S(f13, f14);
            goto L_107d6ec;
        }
        goto L_107d6ec;
L_107d6c8:
        fcr31 = (FPU_C_OLT_S(f4, f3)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
        if ((fcr31 & 0x800000u) != 0u) {
            f3 = FPU_MOV_S(f4);
            goto L_107d6ec;
        }
        fcr31 = (FPU_C_OLT_S(f3, f5)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
        if ((fcr31 & 0x800000u) != 0u) {
            f3 = FPU_MOV_S(f5);
        } else {
        }
    }
L_107d6ec:
    { uint32_t bits = 0u; std::memcpy(&f0, &bits, sizeof(bits)); }
    at = 0x3FBD0000u;
    at = at | 0xE44Eu;
    { uint32_t bits = at; std::memcpy(&f4, &bits, sizeof(bits)); }
    fcr31 = (FPU_C_OLT_S(f2, f0)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
    at = 0x38D10000u;
    at = at | 0xB717u;
    { uint32_t bits = at; std::memcpy(&f0, &bits, sizeof(bits)); }
    at = 0xBFBD0000u;
    at = at | 0xE44Eu;
    { uint32_t bits = at; std::memcpy(&f5, &bits, sizeof(bits)); }
    {
        const bool c = (fcr31 & 0x800000u) == 0u;
        f1 = FPU_MOV_S(f13);
        if (!c) {
            fcr31 = (FPU_C_OLE_S(f14, f0)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
            if ((fcr31 & 0x800000u) != 0u) {
                (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 1))));
                goto L_107d748;
            }
            (v0 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL));
            goto L_107d748;
        }
    }
    fcr31 = (FPU_C_OLE_S(f2, f0)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
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
        fcr31 = (FPU_C_OLT_S(f0, f4)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
        if ((fcr31 & 0x800000u) != 0u) {
            f4 = FPU_MOV_S(f0);
            goto L_107d788;
        }
        at = 0xBFBD0000u;
        at = at | 0xE44Eu;
        { uint32_t bits = at; std::memcpy(&f0, &bits, sizeof(bits)); }
        fcr31 = (FPU_C_OLT_S(f5, f0)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
        if ((fcr31 & 0x800000u) != 0u) {
            f5 = FPU_MOV_S(f0);
        } else {
        }
L_107d788:
        fcr31 = (FPU_C_OLT_S(f4, f13)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
        if ((fcr31 & 0x800000u) != 0u) {
            { uint32_t bits = 0u; std::memcpy(&f0, &bits, sizeof(bits)); }
            fcr31 = (FPU_C_OLT_S(f2, f0)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
            goto L_107d7bc;
        }
        fcr31 = (FPU_C_OLT_S(f13, f5)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
        if ((fcr31 & 0x800000u) == 0u) {
            f1 = FPU_ADD_S(f13, f2);
            goto L_107d7d0;
        }
        { uint32_t bits = 0u; std::memcpy(&f0, &bits, sizeof(bits)); }
        fcr31 = (FPU_C_OLT_S(f0, f2)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
L_107d7bc:
        if ((fcr31 & 0x800000u) != 0u) {
            f1 = FPU_ADD_S(f13, f2);
            goto L_107d7f4;
        }
        goto L_107d7f4;
L_107d7d0:
        fcr31 = (FPU_C_OLT_S(f4, f1)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
        if ((fcr31 & 0x800000u) != 0u) {
            f1 = FPU_MOV_S(f4);
            goto L_107d7f4;
        }
        fcr31 = (FPU_C_OLT_S(f1, f5)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
        if ((fcr31 & 0x800000u) != 0u) {
            f1 = FPU_MOV_S(f5);
        } else {
        }
    }
L_107d7f4:
    if ((int64_t)a0 != 0) {
        { float f = f3; uint32_t bits; std::memcpy(&bits, &f, sizeof(bits)); WRITE32_B((uint32_t)a0, 0, bits); }
    } else {
    }
    if ((int64_t)a1 != 0) {
        { float f = f1; uint32_t bits; std::memcpy(&bits, &f, sizeof(bits)); WRITE32_B((uint32_t)a1, 0, bits); }
    } else {
    }
    fcr31 = (FPU_C_OLE_S(f3, f12)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
    if ((fcr31 & 0x800000u) != 0u) {
        fcr31 = (FPU_C_OLE_S(f12, f1)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
        if ((fcr31 & 0x800000u) != 0u) {
            goto L_107d834;
        }
    }
    fcr31 = (FPU_C_OLE_S(f1, f12)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
    if ((fcr31 & 0x800000u) == 0u) {
        goto L_107d840;
    }
    fcr31 = (FPU_C_OLE_S(f12, f3)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
    if ((fcr31 & 0x800000u) == 0u) {
        goto L_107d840;
    }
L_107d834:
    {
        const uint32_t target = ra;
        (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 1))));
        ctx->f[0] = f0; ctx->f[1] = f1; ctx->f[14] = f14; ctx->f[2] = f2; ctx->f[3] = f3; ctx->f[4] = f4; ctx->f[5] = f5; Ps2SetGpr64(ctx, 1, sx64(at)); Ps2SetGpr64(ctx, 2, v0); ctx->fcr31 = fcr31;
        ctx->pc = target;
        return;
    }
L_107d840:
    {
        const uint32_t target = ra;
        (v0 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL));
        ctx->f[0] = f0; ctx->f[1] = f1; ctx->f[14] = f14; ctx->f[2] = f2; ctx->f[3] = f3; ctx->f[4] = f4; ctx->f[5] = f5; Ps2SetGpr64(ctx, 1, sx64(at)); Ps2SetGpr64(ctx, 2, v0); ctx->fcr31 = fcr31;
        ctx->pc = target;
        return;
    }
}

// GAMEUTIL_is_nearly_equal_zero (0x107ec28): lift_d1's text (the reference)
void GAMEUTIL_is_nearly_equal_zero(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t ra = Ps2GprU32(ctx, 31);
    uint64_t v0 = Ps2GprU64(ctx, 2);
    float f0 = ctx->f[0], f12 = ctx->f[12], f13 = ctx->f[13];
    uint32_t fcr31 = ctx->fcr31;
    f0 = lift_d1::flt(0u);
    fcr31 = (FPU_C_OLT_S(f12, f0)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
    if ((fcr31 & 0x800000u) != 0u) {
        f0 = FPU_NEG_S(f12);
        fcr31 = (FPU_C_OLE_S(f0, f13)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
        if ((fcr31 & 0x800000u) != 0u) {
            goto L_107ec5c;
        }
        {
            const uint32_t target = ra;
            (v0 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL));
            ctx->f[0] = f0; Ps2SetGpr64(ctx, 2, v0); ctx->fcr31 = fcr31;
            ctx->pc = target;
            return;
        }
    }
    fcr31 = (FPU_C_OLE_S(f12, f13)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
    if ((fcr31 & 0x800000u) == 0u) {
        (v0 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL));
        goto L_107ec68;
    }
L_107ec5c:
    {
        const uint32_t target = ra;
        (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 1))));
        ctx->f[0] = f0; Ps2SetGpr64(ctx, 2, v0); ctx->fcr31 = fcr31;
        ctx->pc = target;
        return;
    }
L_107ec68:
    {
        const uint32_t target = ra;
        ctx->f[0] = f0; Ps2SetGpr64(ctx, 2, v0); ctx->fcr31 = fcr31;
        ctx->pc = target;
        return;
    }
}

// GAMEUTIL_is_nearly_equal_zero3D (0x107ec70): lift_d1's text (the reference)
void GAMEUTIL_is_nearly_equal_zero3D(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t a0 = Ps2GprU32(ctx, 4), ra = Ps2GprU32(ctx, 31);
    uint64_t v0 = Ps2GprU64(ctx, 2);
    float f0 = ctx->f[0], f1 = ctx->f[1], f12 = ctx->f[12];
    uint32_t fcr31 = ctx->fcr31;
    f0 = lift_d1::flt(0u);
    f1 = lift_d1::flt(READ32(a0));
    fcr31 = (FPU_C_OLT_S(f1, f0)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
    if ((fcr31 & 0x800000u) != 0u) {
        f0 = FPU_NEG_S(f1);
        fcr31 = (FPU_C_OLE_S(f0, f12)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
        if ((fcr31 & 0x800000u) != 0u) {
            f1 = lift_d1::flt(READ32_B(a0, 4));
            goto L_107ecb0;
        }
        {
            const uint32_t target = ra;
            (v0 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL));
            ctx->f[0] = f0; ctx->f[1] = f1; Ps2SetGpr64(ctx, 2, v0); ctx->fcr31 = fcr31;
            ctx->pc = target;
            return;
        }
    }
    fcr31 = (FPU_C_OLE_S(f1, f12)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
    {
        const bool c = (fcr31 & 0x800000u) == 0u;
        (v0 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL));
        if (c) {
            goto L_107ed28;
        }
    }
    f1 = lift_d1::flt(READ32_B(a0, 4));
L_107ecb0:
    f0 = lift_d1::flt(0u);
    fcr31 = (FPU_C_OLT_S(f1, f0)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
    if ((fcr31 & 0x800000u) != 0u) {
        f0 = FPU_NEG_S(f1);
        fcr31 = (FPU_C_OLE_S(f0, f12)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
        if ((fcr31 & 0x800000u) != 0u) {
            f1 = lift_d1::flt(READ32_B(a0, 8));
            goto L_107ece8;
        }
        {
            const uint32_t target = ra;
            (v0 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL));
            ctx->f[0] = f0; ctx->f[1] = f1; Ps2SetGpr64(ctx, 2, v0); ctx->fcr31 = fcr31;
            ctx->pc = target;
            return;
        }
    }
    fcr31 = (FPU_C_OLE_S(f1, f12)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
    {
        const bool c = (fcr31 & 0x800000u) == 0u;
        (v0 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL));
        if (c) {
            goto L_107ed28;
        }
    }
    f1 = lift_d1::flt(READ32_B(a0, 8));
L_107ece8:
    f0 = lift_d1::flt(0u);
    fcr31 = (FPU_C_OLT_S(f1, f0)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
    if ((fcr31 & 0x800000u) != 0u) {
        f0 = FPU_NEG_S(f1);
        fcr31 = (FPU_C_OLE_S(f0, f12)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
        if ((fcr31 & 0x800000u) != 0u) {
            goto L_107ed1c;
        }
        {
            const uint32_t target = ra;
            (v0 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL));
            ctx->f[0] = f0; ctx->f[1] = f1; Ps2SetGpr64(ctx, 2, v0); ctx->fcr31 = fcr31;
            ctx->pc = target;
            return;
        }
    }
    fcr31 = (FPU_C_OLE_S(f1, f12)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
    if ((fcr31 & 0x800000u) == 0u) {
        (v0 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL));
        goto L_107ed28;
    }
L_107ed1c:
    {
        const uint32_t target = ra;
        (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 1))));
        ctx->f[0] = f0; ctx->f[1] = f1; Ps2SetGpr64(ctx, 2, v0); ctx->fcr31 = fcr31;
        ctx->pc = target;
        return;
    }
L_107ed28:
    {
        const uint32_t target = ra;
        ctx->f[0] = f0; ctx->f[1] = f1; Ps2SetGpr64(ctx, 2, v0); ctx->fcr31 = fcr31;
        ctx->pc = target;
        return;
    }
}

// GAMEUTIL_get_answer_of_2jihouteishiki (0x107ee48): lift_d1's text (the reference)
void GAMEUTIL_get_answer_of_2jihouteishiki(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t at = Ps2GprU32(ctx, 1), v0 = Ps2GprU32(ctx, 2), a0 = Ps2GprU32(ctx, 4), a1 = Ps2GprU32(ctx, 5), ra = Ps2GprU32(ctx, 31);
    float f0 = ctx->f[0], f1 = ctx->f[1], f2 = ctx->f[2], f3 = ctx->f[3], f12 = ctx->f[12], f13 = ctx->f[13], f14 = ctx->f[14];
    uint32_t fcr31 = ctx->fcr31;
    at = 0x40800000u;
    f0 = lift_d1::flt(at);
    f1 = FPU_MUL_S(f13, f13);
    f2 = lift_d1::flt(0u);
    f0 = FPU_MUL_S(f12, f0);
    f0 = FPU_MUL_S(f0, f14);
    f3 = FPU_SUB_SF(f1, f0);
    fcr31 = (FPU_C_OLT_S(f3, f2)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
    {
        const bool c = (fcr31 & 0x800000u) == 0u;
        v0 = 1u;
        if (!c) {
            v0 = 0u;
        }
    }
    f3 = ps2FpuSqrt(fcr31, f3);
    f0 = FPU_NEG_S(f13);
    f2 = FPU_ADD_S(f12, f12);
    f1 = FPU_SUB_S(f3, f13);
    f0 = FPU_SUB_S(f0, f3);
    f1 = ps2FpuDivide(fcr31, f1, f2);
    f0 = ps2FpuDivide(fcr31, f0, f2);
    WRITE32(a0, lift_d1::bits(f1));
    WRITE32(a1, lift_d1::bits(f0));
    f1 = lift_d1::flt(READ32(a0));
    fcr31 = (FPU_C_OLT_S(f1, f0)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
    if ((fcr31 & 0x800000u) != 0u) {
        WRITE32(a0, lift_d1::bits(f0));
        WRITE32(a1, lift_d1::bits(f1));
    }
    {
        const uint32_t target = ra;
        ctx->f[0] = f0; ctx->f[1] = f1; ctx->f[2] = f2; ctx->f[3] = f3; Ps2SetGpr64(ctx, 1, sx64(at)); Ps2SetGpr64(ctx, 2, sx64(v0)); ctx->fcr31 = fcr31;
        ctx->pc = target;
        return;
    }
}

// GAMEUTIL_get_new_velocity_by_brake_abs (0x107fb30): lift_d1's text (the reference)
void GAMEUTIL_get_new_velocity_by_brake_abs(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t ra = Ps2GprU32(ctx, 31);
    float f0 = ctx->f[0], f12 = ctx->f[12], f13 = ctx->f[13], f14 = ctx->f[14], f15 = ctx->f[15];
    uint32_t fcr31 = ctx->fcr31;
    f0 = lift_d1::flt(0u);
    fcr31 = (FPU_C_OLE_S(f0, f13)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
    {
        const bool c = (fcr31 & 0x800000u) == 0u;
        f0 = FPU_MOV_S(f14);
        if (!c) {
            f0 = FPU_MUL_S(f13, f15);
            f0 = FPU_SUB_S(f12, f0);
            fcr31 = (FPU_C_OLT_S(f0, f14)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
            if ((fcr31 & 0x800000u) != 0u) {
                f0 = FPU_MOV_S(f14);
            } else {
            }
        }
    }
    {
        const uint32_t target = ra;
        ctx->f[0] = f0; ctx->fcr31 = fcr31;
        ctx->pc = target;
        return;
    }
}

// GAMEUTIL_get_velocity_by_diff_move (0x107fb60): lift_d1's text (the reference)
void GAMEUTIL_get_velocity_by_diff_move(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t ra = Ps2GprU32(ctx, 31);
    uint64_t a0 = Ps2GprU64(ctx, 4);
    float f0 = ctx->f[0], f1 = ctx->f[1], f12 = ctx->f[12], f13 = ctx->f[13], f14 = ctx->f[14], f15 = ctx->f[15];
    uint32_t fcr31 = ctx->fcr31;
    { uint32_t bits = 0u; std::memcpy(&f1, &bits, sizeof(bits)); }
    fcr31 = (FPU_C_EQ_S(f15, f1)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
    {
        const bool c = (fcr31 & 0x800000u) != 0u;
        f0 = FPU_MOV_S(f12);
        if (!c) {
            f0 = FPU_SUB_S(f14, f13);
            f0 = ps2FpuDivide(fcr31, f0, f15);
            if ((int64_t)a0 != 0) {
                fcr31 = (FPU_C_OLT_S(f0, f1)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
                if ((fcr31 & 0x800000u) != 0u) {
                    f0 = FPU_NEG_S(f0);
                } else {
                }
            }
        }
    }
    {
        const uint32_t target = ra;
        ctx->f[0] = f0; ctx->f[1] = f1; ctx->fcr31 = fcr31;
        ctx->pc = target;
        return;
    }
}

// GAMEUTIL_mul_ratio (0x107fed0): lift_d1's text (the reference)
void GAMEUTIL_mul_ratio(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t at = Ps2GprU32(ctx, 1), ra = Ps2GprU32(ctx, 31);
    uint64_t a0 = Ps2GprU64(ctx, 4);
    float f0 = ctx->f[0], f1 = ctx->f[1], f12 = ctx->f[12], f13 = ctx->f[13];
    uint32_t fcr31 = ctx->fcr31;
    bool df_at = false;
    if ((int64_t)a0 == 0) {
        f0 = FPU_MUL_S(f13, f12);
    } else {
        { uint32_t bits = 0u; std::memcpy(&f0, &bits, sizeof(bits)); }
        fcr31 = (FPU_C_OLE_S(f13, f0)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
        if ((fcr31 & 0x800000u) == 0u) {
            at = 0x3F800000u; df_at = true;
            { uint32_t bits = at; std::memcpy(&f1, &bits, sizeof(bits)); }
            fcr31 = (FPU_C_OLE_S(f1, f13)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
            {
                const bool c = (fcr31 & 0x800000u) != 0u;
                f0 = FPU_MOV_S(f12);
                if (!c) {
                    f0 = FPU_MUL_S(f13, f12);
                }
            }
        }
    }
    {
        const uint32_t target = ra;
        ctx->f[0] = f0; ctx->f[1] = f1; if (df_at) Ps2SetGpr64(ctx, 1, sx64(at)); ctx->fcr31 = fcr31;
        ctx->pc = target;
        return;
    }
}

// GAMEUTIL_convert_from_lw_to_game_coord (0x10802f0): lift_d1's text (the reference)
void GAMEUTIL_convert_from_lw_to_game_coord(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t a0 = Ps2GprU32(ctx, 4), ra = Ps2GprU32(ctx, 31);
    float f0 = ctx->f[0], f1 = ctx->f[1];
    f0 = lift_d1::flt(READ32(a0));
    f1 = lift_d1::flt(READ32_B(a0, 8));
    f0 = FPU_NEG_S(f0);
    f1 = FPU_NEG_S(f1);
    WRITE32(a0, lift_d1::bits(f0));
    {
        const uint32_t target = ra;
        WRITE32_B(a0, 8, lift_d1::bits(f1));
        ctx->f[0] = f0; ctx->f[1] = f1;
        ctx->pc = target;
        return;
    }
}

// GAMEUTIL_get_difference_pitch_and_pitch (0x1080b08): lift_d1's text (the reference)
void GAMEUTIL_get_difference_pitch_and_pitch(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t ra = Ps2GprU32(ctx, 31);
    float f0 = ctx->f[0], f12 = ctx->f[12], f13 = ctx->f[13];
    uint32_t fcr31 = ctx->fcr31;
    f13 = FPU_SUB_S(f13, f12);
    f0 = lift_d1::flt(0u);
    fcr31 = (FPU_C_OLT_S(f13, f0)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
    if ((fcr31 & 0x800000u) != 0u) {
        f13 = FPU_NEG_S(f13);
    } else {
    }
    {
        const uint32_t target = ra;
        f0 = FPU_MOV_S(f13);
        ctx->f[0] = f0; ctx->f[13] = f13; ctx->fcr31 = fcr31;
        ctx->pc = target;
        return;
    }
}

// GAMEUTIL_get_move_radi_from_yaw1_to_yaw2 (0x1080b28): lift_d1's text (the reference)
void GAMEUTIL_get_move_radi_from_yaw1_to_yaw2(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t at = Ps2GprU32(ctx, 1), v0 = Ps2GprU32(ctx, 2), ra = Ps2GprU32(ctx, 31);
    float f0 = ctx->f[0], f1 = ctx->f[1], f12 = ctx->f[12], f13 = ctx->f[13];
    uint32_t fcr31 = ctx->fcr31;
    f1 = FPU_SUB_S(f13, f12);
    f0 = lift_d1::flt(0u);
    fcr31 = (FPU_C_OLT_S(f1, f0)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
    if ((fcr31 & 0x800000u) != 0u) {
        f1 = FPU_NEG_S(f1);
    } else {
    }
    fcr31 = (FPU_C_OLT_S(f13, f12)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
    {
        const bool c = (fcr31 & 0x800000u) == 0u;
        v0 = 1u;
        if (!c) {
            v0 = 0u;
        }
    }
    at = 0x40490000u;
    at = at | 0xFDAu;
    f0 = lift_d1::flt(at);
    fcr31 = (FPU_C_OLT_S(f0, f1)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
    if ((fcr31 & 0x800000u) != 0u) {
        at = 0x40C90000u;
        at = at | 0xFDAu;
        f0 = lift_d1::flt(at);
        v0 = v0 < 1u ? 1u : 0u;
        f1 = FPU_SUB_S(f0, f1);
    }
    {
        const bool c = v0 != 0;
        f0 = FPU_MOV_S(f1);
        if (!c) {
            f0 = FPU_NEG_S(f1);
        }
    }
    {
        const uint32_t target = ra;
        ctx->f[0] = f0; ctx->f[1] = f1; Ps2SetGpr64(ctx, 1, sx64(at)); Ps2SetGpr64(ctx, 2, sx64(v0)); ctx->fcr31 = fcr31;
        ctx->pc = target;
        return;
    }
}

// GAMEUTIL_get_move_radi_from_pitch1_to_pitch2 (0x1080b90): lift_d1's text (the reference)
void GAMEUTIL_get_move_radi_from_pitch1_to_pitch2(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t ra = Ps2GprU32(ctx, 31);
    float f0 = ctx->f[0], f12 = ctx->f[12], f13 = ctx->f[13];
    {
        const uint32_t target = ra;
        f0 = FPU_SUB_S(f13, f12);
        ctx->f[0] = f0;
        ctx->pc = target;
        return;
    }
}

// GAMEUTIL_is_between_yaw_and_yaw (0x1080b98): lift_d1's text (the reference)
void GAMEUTIL_is_between_yaw_and_yaw(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t at = Ps2GprU32(ctx, 1), ra = Ps2GprU32(ctx, 31);
    uint64_t v0 = Ps2GprU64(ctx, 2);
    float f0 = ctx->f[0], f1 = ctx->f[1], f12 = ctx->f[12], f13 = ctx->f[13], f14 = ctx->f[14];
    uint32_t fcr31 = ctx->fcr31;
    f1 = FPU_SUB_S(f13, f14);
    f0 = lift_d1::flt(0u);
    fcr31 = (FPU_C_OLT_S(f1, f0)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
    {
        const bool c = (fcr31 & 0x800000u) == 0u;
        (v0 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL));
        if (!c) {
            at = 0xC0490000u;
            at = at | 0xFDAu;
            f0 = lift_d1::flt(at);
            fcr31 = (FPU_C_OLT_S(f1, f0)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
            if ((fcr31 & 0x800000u) != 0u) {
                (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 1))));
                goto L_1080be8;
            }
            goto L_1080be8;
        }
    }
    at = 0x40490000u;
    at = at | 0xFDAu;
    f0 = lift_d1::flt(at);
    fcr31 = (FPU_C_OLT_S(f0, f1)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
    if ((fcr31 & 0x800000u) != 0u) {
        (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 1))));
    } else {
    }
L_1080be8:
    fcr31 = (FPU_C_OLE_S(f13, f12)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
    if ((fcr31 & 0x800000u) != 0u) {
        fcr31 = (FPU_C_OLE_S(f12, f14)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
        if ((fcr31 & 0x800000u) != 0u) {
            goto L_1080c18;
        }
    }
    fcr31 = (FPU_C_OLE_S(f14, f12)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
    if ((fcr31 & 0x800000u) == 0u) {
        goto L_1080c20;
    }
    fcr31 = (FPU_C_OLE_S(f12, f13)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
    if ((fcr31 & 0x800000u) == 0u) {
        goto L_1080c20;
    }
L_1080c18:
    {
        const uint32_t target = ra;
        (v0 = (uint64_t)(((uint64_t)v0 < (uint64_t)(int64_t)(int32_t)1) ? 1 : 0));
        ctx->f[0] = f0; ctx->f[1] = f1; Ps2SetGpr64(ctx, 1, sx64(at)); Ps2SetGpr64(ctx, 2, v0); ctx->fcr31 = fcr31;
        ctx->pc = target;
        return;
    }
L_1080c20:
    {
        const uint32_t target = ra;
        (v0 = (uint64_t)(((uint64_t)0ULL < (uint64_t)v0) ? 1 : 0));
        ctx->f[0] = f0; ctx->f[1] = f1; Ps2SetGpr64(ctx, 1, sx64(at)); Ps2SetGpr64(ctx, 2, v0); ctx->fcr31 = fcr31;
        ctx->pc = target;
        return;
    }
}

// GAMEUTIL_is_between_pitch_and_pitch (0x1080d58): lift_d1's text (the reference)
void GAMEUTIL_is_between_pitch_and_pitch(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t ra = Ps2GprU32(ctx, 31);
    uint64_t v0 = Ps2GprU64(ctx, 2);
    float f12 = ctx->f[12], f13 = ctx->f[13], f14 = ctx->f[14];
    uint32_t fcr31 = ctx->fcr31;
    fcr31 = (FPU_C_OLE_S(f13, f12)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
    if ((fcr31 & 0x800000u) != 0u) {
        fcr31 = (FPU_C_OLE_S(f12, f14)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
        if ((fcr31 & 0x800000u) != 0u) {
            goto L_1080d88;
        }
    }
    fcr31 = (FPU_C_OLE_S(f14, f12)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
    if ((fcr31 & 0x800000u) == 0u) {
        goto L_1080d90;
    }
    fcr31 = (FPU_C_OLE_S(f12, f13)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
    if ((fcr31 & 0x800000u) == 0u) {
        goto L_1080d90;
    }
L_1080d88:
    {
        const uint32_t target = ra;
        (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 1))));
        Ps2SetGpr64(ctx, 2, v0); ctx->fcr31 = fcr31;
        ctx->pc = target;
        return;
    }
L_1080d90:
    {
        const uint32_t target = ra;
        (v0 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL));
        Ps2SetGpr64(ctx, 2, v0); ctx->fcr31 = fcr31;
        ctx->pc = target;
        return;
    }
}

// GAMEUTIL_is_reach_aim_yaw_angle (0x1080f50): lift_d1's text (the reference)
void GAMEUTIL_is_reach_aim_yaw_angle(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t at = Ps2GprU32(ctx, 1), ra = Ps2GprU32(ctx, 31);
    uint64_t v0 = Ps2GprU64(ctx, 2), v1 = Ps2GprU64(ctx, 3), a0 = Ps2GprU64(ctx, 4);
    float f0 = ctx->f[0], f1 = ctx->f[1], f2 = ctx->f[2], f12 = ctx->f[12], f13 = ctx->f[13], f14 = ctx->f[14];
    uint32_t fcr31 = ctx->fcr31;
    f1 = FPU_SUB_S(f12, f13);
    { uint32_t bits = 0u; std::memcpy(&f0, &bits, sizeof(bits)); }
    fcr31 = (FPU_C_OLT_S(f1, f0)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
    {
        const bool c = (fcr31 & 0x800000u) == 0u;
        (v0 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL));
        if (!c) {
            at = 0xC0490000u;
            at = at | 0xFDAu;
            { uint32_t bits = at; std::memcpy(&f0, &bits, sizeof(bits)); }
            fcr31 = (FPU_C_OLT_S(f1, f0)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
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
    fcr31 = (FPU_C_OLT_S(f0, f1)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
    if ((fcr31 & 0x800000u) != 0u) {
        (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 1))));
    } else {
    }
L_1080fa0:
    fcr31 = (FPU_C_OLE_S(f12, f14)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
    if ((fcr31 & 0x800000u) != 0u) {
        fcr31 = (FPU_C_OLE_S(f14, f13)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
        {
            const bool c = (fcr31 & 0x800000u) != 0u;
            (v1 = (uint64_t)(((uint64_t)v0 < (uint64_t)(int64_t)(int32_t)1) ? 1 : 0));
            if (c) {
                goto L_1080fd4;
            }
        }
    }
    fcr31 = (FPU_C_OLE_S(f13, f14)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
    {
        const bool c = (fcr31 & 0x800000u) == 0u;
        (v1 = (uint64_t)(((uint64_t)0ULL < (uint64_t)v0) ? 1 : 0));
        if (!c) {
            fcr31 = (FPU_C_OLE_S(f14, f12)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
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
        f1 = FPU_SUB_S(f14, f13);
        { uint32_t bits = 0u; std::memcpy(&f2, &bits, sizeof(bits)); }
        fcr31 = (FPU_C_OLT_S(f1, f2)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
        if ((fcr31 & 0x800000u) != 0u) {
            f1 = FPU_NEG_S(f1);
        } else {
        }
        at = 0x40490000u;
        at = at | 0xFDAu;
        { uint32_t bits = at; std::memcpy(&f0, &bits, sizeof(bits)); }
        fcr31 = (FPU_C_OLT_S(f0, f1)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
        if ((fcr31 & 0x800000u) != 0u) {
            at = 0x40C90000u;
            at = at | 0xFDAu;
            { uint32_t bits = at; std::memcpy(&f0, &bits, sizeof(bits)); }
            f1 = FPU_SUB_S(f0, f1);
        }
        fcr31 = (FPU_C_OLT_S(f1, f2)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
        at = 0x38D10000u;
        at = at | 0xB717u;
        { uint32_t bits = at; std::memcpy(&f2, &bits, sizeof(bits)); }
        if ((fcr31 & 0x800000u) != 0u) {
            f0 = FPU_NEG_S(f1);
            fcr31 = (FPU_C_OLE_S(f0, f2)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
            if ((fcr31 & 0x800000u) != 0u) {
                (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 1))));
                goto L_1081060;
            }
            (v1 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL));
            goto L_1081060;
        }
        fcr31 = (FPU_C_OLE_S(f1, f2)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
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
        { float f = f14; uint32_t bits; std::memcpy(&bits, &f, sizeof(bits)); WRITE32_B((uint32_t)a0, 0, bits); }
    } else {
    }
    {
        const uint32_t target = ra;
        (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 1))));
        ctx->f[0] = f0; ctx->f[1] = f1; ctx->f[2] = f2; Ps2SetGpr64(ctx, 1, sx64(at)); Ps2SetGpr64(ctx, 2, v0); Ps2SetGpr64(ctx, 3, v1); ctx->fcr31 = fcr31;
        ctx->pc = target;
        return;
    }
L_1081080:
    if ((int64_t)a0 != 0) {
        { float f = f13; uint32_t bits; std::memcpy(&bits, &f, sizeof(bits)); WRITE32_B((uint32_t)a0, 0, bits); }
    } else {
    }
    {
        const uint32_t target = ra;
        (v0 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL));
        ctx->f[0] = f0; ctx->f[1] = f1; ctx->f[2] = f2; Ps2SetGpr64(ctx, 1, sx64(at)); Ps2SetGpr64(ctx, 2, v0); Ps2SetGpr64(ctx, 3, v1); ctx->fcr31 = fcr31;
        ctx->pc = target;
        return;
    }
}

// GAMEUTIL_is_nearly_equal_yaw (0x10829f8): lift_d1's text (the reference)
void GAMEUTIL_is_nearly_equal_yaw(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t at = Ps2GprU32(ctx, 1), v0 = Ps2GprU32(ctx, 2), ra = Ps2GprU32(ctx, 31);
    float f0 = ctx->f[0], f12 = ctx->f[12], f13 = ctx->f[13], f14 = ctx->f[14];
    uint32_t fcr31 = ctx->fcr31;
    f13 = FPU_SUB_S(f13, f12);
    f0 = lift_d1::flt(0u);
    fcr31 = (FPU_C_OLT_S(f13, f0)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
    if ((fcr31 & 0x800000u) != 0u) {
        f13 = FPU_NEG_S(f13);
    } else {
    }
    at = 0x40490000u;
    at = at | 0xFDAu;
    f0 = lift_d1::flt(at);
    fcr31 = (FPU_C_OLT_S(f0, f13)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
    if ((fcr31 & 0x800000u) != 0u) {
        at = 0x40C90000u;
        at = at | 0xFDAu;
        f0 = lift_d1::flt(at);
        f13 = FPU_SUB_S(f0, f13);
    }
    fcr31 = (FPU_C_OLE_S(f13, f14)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
    {
        const bool c = (fcr31 & 0x800000u) != 0u;
        v0 = 1u;
        if (!c) {
            v0 = 0u;
        }
    }
    {
        const uint32_t target = ra;
        ctx->f[0] = f0; ctx->f[13] = f13; Ps2SetGpr64(ctx, 1, sx64(at)); Ps2SetGpr64(ctx, 2, sx64(v0)); ctx->fcr31 = fcr31;
        ctx->pc = target;
        return;
    }
}

// GAMEUTIL_is_nearly_equal_pitch (0x1082a50): lift_d1's text (the reference)
void GAMEUTIL_is_nearly_equal_pitch(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t v0 = Ps2GprU32(ctx, 2), ra = Ps2GprU32(ctx, 31);
    float f0 = ctx->f[0], f12 = ctx->f[12], f13 = ctx->f[13], f14 = ctx->f[14];
    uint32_t fcr31 = ctx->fcr31;
    f13 = FPU_SUB_S(f13, f12);
    f0 = lift_d1::flt(0u);
    fcr31 = (FPU_C_OLT_S(f13, f0)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
    if ((fcr31 & 0x800000u) != 0u) {
        f13 = FPU_NEG_S(f13);
    } else {
    }
    fcr31 = (FPU_C_OLE_S(f13, f14)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
    {
        const bool c = (fcr31 & 0x800000u) != 0u;
        v0 = 1u;
        if (!c) {
            v0 = 0u;
        }
    }
    {
        const uint32_t target = ra;
        ctx->f[0] = f0; ctx->f[13] = f13; Ps2SetGpr64(ctx, 2, sx64(v0)); ctx->fcr31 = fcr31;
        ctx->pc = target;
        return;
    }
}

// manager_sub_01082C78 (0x1082c78): lift_d1's text (the reference)
void manager_sub_01082C78(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t at = Ps2GprU32(ctx, 1), ra = Ps2GprU32(ctx, 31);
    uint64_t v0 = Ps2GprU64(ctx, 2);
    float f0 = ctx->f[0], f1 = ctx->f[1], f12 = ctx->f[12];
    uint32_t fcr31 = ctx->fcr31;
    f0 = lift_d1::flt(0u);
    at = 0x38D10000u;
    at = at | 0xB717u;
    f1 = lift_d1::flt(at);
    fcr31 = (FPU_C_OLT_S(f12, f0)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
    if ((fcr31 & 0x800000u) != 0u) {
        f0 = FPU_NEG_S(f12);
        fcr31 = (FPU_C_OLE_S(f0, f1)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
        if ((fcr31 & 0x800000u) != 0u) {
            (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 1))));
            goto L_1082cc0;
        }
        (v0 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL));
        goto L_1082cc0;
    }
    fcr31 = (FPU_C_OLE_S(f12, f1)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
    if ((fcr31 & 0x800000u) == 0u) {
        (v0 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL));
    } else {
        (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 1))));
    }
L_1082cc0:
    at = 0x3F800000u;
    f0 = lift_d1::flt(at);
    if ((int64_t)v0 == 0) {
        f0 = FPU_MOV_S(f12);
    } else {
    }
    {
        const uint32_t target = ra;
        ctx->f[0] = f0; ctx->f[1] = f1; Ps2SetGpr64(ctx, 1, sx64(at)); Ps2SetGpr64(ctx, 2, v0); ctx->fcr31 = fcr31;
        ctx->pc = target;
        return;
    }
}

// manager_sub_01082CD8 (0x1082cd8): lift_d1's text (the reference)
void manager_sub_01082CD8(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t at = Ps2GprU32(ctx, 1), ra = Ps2GprU32(ctx, 31);
    uint64_t v0 = Ps2GprU64(ctx, 2);
    float f0 = ctx->f[0], f1 = ctx->f[1], f2 = ctx->f[2], f12 = ctx->f[12], f13 = ctx->f[13], f14 = ctx->f[14];
    uint32_t fcr31 = ctx->fcr31;
    f1 = lift_d1::flt(0u);
    at = 0x38D10000u;
    at = at | 0xB717u;
    { uint32_t bits = at; std::memcpy(&f2, &bits, sizeof(bits)); }
    fcr31 = (FPU_C_OLT_S(f14, f1)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
    {
        const bool c = (fcr31 & 0x800000u) == 0u;
        f0 = FPU_MOV_S(f13);
        if (!c) {
            f1 = FPU_NEG_S(f14);
            fcr31 = (FPU_C_OLE_S(f1, f2)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
            if ((fcr31 & 0x800000u) != 0u) {
                (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 1))));
                goto L_1082d20;
            }
            (v0 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL));
            goto L_1082d20;
        }
    }
    fcr31 = (FPU_C_OLE_S(f14, f2)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
    if ((fcr31 & 0x800000u) == 0u) {
        (v0 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL));
    } else {
        (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 1))));
    }
L_1082d20:
    if ((int64_t)v0 == 0) {
        f0 = FPU_MOV_S(f12);
    } else {
    }
    {
        const uint32_t target = ra;
        ctx->f[0] = f0; ctx->f[1] = f1; ctx->f[2] = f2; Ps2SetGpr64(ctx, 1, sx64(at)); Ps2SetGpr64(ctx, 2, v0); ctx->fcr31 = fcr31;
        ctx->pc = target;
        return;
    }
}

// get_fast_get_malloc_partition (0x1082da0): lift_d1's text (the reference)
void get_fast_get_malloc_partition(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t v0 = Ps2GprU32(ctx, 2), v1 = Ps2GprU32(ctx, 3), ra = Ps2GprU32(ctx, 31);
    v1 = 0x1180000u;
    {
        const uint32_t target = ra;
        v0 = READ32_B(v1, 25168);
        Ps2SetGpr64(ctx, 2, sx64(v0)); Ps2SetGpr64(ctx, 3, sx64(v1));
        ctx->pc = target;
        return;
    }
}

// GAMEUTIL_doit_interp_ratio_cntrl (0x10832a8): lift_d1's text (the reference)
void GAMEUTIL_doit_interp_ratio_cntrl(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t at = Ps2GprU32(ctx, 1), v0 = Ps2GprU32(ctx, 2), v1 = Ps2GprU32(ctx, 3), a0 = Ps2GprU32(ctx, 4), ra = Ps2GprU32(ctx, 31);
    float f0 = ctx->f[0], f1 = ctx->f[1], f2 = ctx->f[2], f3 = ctx->f[3], f12 = ctx->f[12];
    uint32_t fcr31 = ctx->fcr31;
    bool df_at = false;
    v0 = READ32(a0);
    v1 = 1u;
    if (v0 != v1) {
        WRITE32_B(a0, 8, v1);
    } else {
        f3 = lift_d1::flt(READ32_B(a0, 12));
        f0 = lift_d1::flt(0u);
        fcr31 = (FPU_C_OLT_S(f0, f3)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
        if ((fcr31 & 0x800000u) != 0u) {
            WRITE32_B(a0, 8, 0u);
        } else {
        }
        fcr31 = (FPU_C_OLE_S(f0, f12)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
        if ((fcr31 & 0x800000u) == 0u) {
            f0 = lift_d1::flt(READ32_B(a0, 28));
        } else {
            f0 = FPU_ADD_S(f3, f12);
            goto L_10832e8;
        }
        f0 = FPU_MUL_S(f12, f0);
        f0 = FPU_ADD_S(f3, f0);
L_10832e8:
        WRITE32_B(a0, 12, lift_d1::bits(f0));
        v1 = READ32_B(a0, 4);
        v0 = 1u;
        {
            const bool c = v1 != v0;
            f3 = lift_d1::flt(READ32_B(a0, 12));
            if (!c) {
                f0 = lift_d1::flt(0u);
                fcr31 = (FPU_C_OLT_S(f3, f0)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
                if ((fcr31 & 0x800000u) == 0u) {
                    f0 = lift_d1::flt(READ32_B(a0, 20));
                } else {
                    WRITE32_B(a0, 12, lift_d1::bits(f0));
                    f3 = FPU_MOV_S(f0);
                    f0 = lift_d1::flt(READ32_B(a0, 20));
                }
                f1 = lift_d1::flt(READ32_B(a0, 24));
                f2 = FPU_ADD_S(f0, f1);
                fcr31 = (FPU_C_OLT_S(f2, f3)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
                if ((fcr31 & 0x800000u) == 0u) {
                    goto L_1083340;
                }
                WRITE32_B(a0, 12, lift_d1::bits(f2));
                f3 = FPU_MOV_S(f2);
                goto L_1083340;
            }
        }
        f0 = lift_d1::flt(READ32_B(a0, 20));
        f1 = lift_d1::flt(READ32_B(a0, 24));
L_1083340:
        fcr31 = (FPU_C_OLE_S(f3, f0)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
        f2 = FPU_ADD_S(f0, f1);
        f1 = lift_d1::flt(0u);
        if ((fcr31 & 0x800000u) != 0u) {
            WRITE32_B(a0, 16, lift_d1::bits(f1));
        } else {
            fcr31 = (FPU_C_OLE_S(f2, f3)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
            at = 0x3F800000u; df_at = true;
            f1 = lift_d1::flt(at);
            if ((fcr31 & 0x800000u) != 0u) {
                WRITE32_B(a0, 16, lift_d1::bits(f1));
            } else {
                f1 = FPU_SUB_S(f2, f0);
                f0 = FPU_SUB_S(f3, f0);
                f1 = ps2FpuDivide(fcr31, f0, f1);
                WRITE32_B(a0, 16, lift_d1::bits(f1));
            }
        }
        {
            const uint32_t target = ra;
            f0 = FPU_MOV_S(f1);
            ctx->f[0] = f0; ctx->f[1] = f1; ctx->f[2] = f2; ctx->f[3] = f3; if (df_at) Ps2SetGpr64(ctx, 1, sx64(at)); Ps2SetGpr64(ctx, 2, sx64(v0)); Ps2SetGpr64(ctx, 3, sx64(v1)); ctx->fcr31 = fcr31;
            ctx->pc = target;
            return;
        }
    }
    at = 0x3F800000u; df_at = true;
    f0 = lift_d1::flt(at);
    WRITE32_B(a0, 16, lift_d1::bits(f0));
    {
        const uint32_t target = ra;
        WRITE32_B(a0, 12, 0u);
        ctx->f[0] = f0; if (df_at) Ps2SetGpr64(ctx, 1, sx64(at)); Ps2SetGpr64(ctx, 2, sx64(v0)); Ps2SetGpr64(ctx, 3, sx64(v1));
        ctx->pc = target;
        return;
    }
}

// GAMEUTIL_get_dist_ratio (0x10836c0): lift_d1's text (the reference)
void GAMEUTIL_get_dist_ratio(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t at = Ps2GprU32(ctx, 1), ra = Ps2GprU32(ctx, 31);
    float f0 = ctx->f[0], f1 = ctx->f[1], f12 = ctx->f[12], f13 = ctx->f[13], f14 = ctx->f[14];
    uint32_t fcr31 = ctx->fcr31;
    bool df_at = false;
    fcr31 = (FPU_C_OLE_S(f12, f13)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
    f0 = lift_d1::flt(0u);
    if ((fcr31 & 0x800000u) == 0u) {
        fcr31 = (FPU_C_OLE_S(f14, f12)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
        at = 0x3F800000u; df_at = true;
        f0 = lift_d1::flt(at);
        if ((fcr31 & 0x800000u) == 0u) {
            f1 = FPU_SUB_S(f14, f13);
            f0 = FPU_SUB_S(f12, f13);
            f0 = ps2FpuDivide(fcr31, f0, f1);
        }
    }
    {
        const uint32_t target = ra;
        ctx->f[0] = f0; ctx->f[1] = f1; if (df_at) Ps2SetGpr64(ctx, 1, sx64(at)); ctx->fcr31 = fcr31;
        ctx->pc = target;
        return;
    }
}

// GAMEUTIL_get_interp_value (0x1083828): lift_d1's text (the reference)
void GAMEUTIL_get_interp_value(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t at = Ps2GprU32(ctx, 1), ra = Ps2GprU32(ctx, 31);
    float f0 = ctx->f[0], f1 = ctx->f[1], f12 = ctx->f[12], f13 = ctx->f[13], f14 = ctx->f[14];
    uint32_t fcr31 = ctx->fcr31;
    bool df_at = false;
    f0 = lift_d1::flt(0u);
    fcr31 = (FPU_C_OLE_S(f14, f0)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
    {
        const bool c = (fcr31 & 0x800000u) != 0u;
        f0 = FPU_MOV_S(f13);
        if (!c) {
            at = 0x3F800000u; df_at = true;
            f1 = lift_d1::flt(at);
            fcr31 = (FPU_C_OLE_S(f1, f14)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
            {
                const bool c = (fcr31 & 0x800000u) != 0u;
                f0 = FPU_MOV_S(f12);
                if (!c) {
                    fcr31 = (FPU_C_EQ_S(f12, f13)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
                    if ((fcr31 & 0x800000u) == 0u) {
                        f0 = FPU_SUB_S(f1, f14);
                        f1 = FPU_MUL_S(f14, f12);
                        f0 = FPU_MUL_S(f0, f13);
                        f0 = FPU_ADD_SF(f1, f0);
                    }
                }
            }
        }
    }
    {
        const uint32_t target = ra;
        ctx->f[0] = f0; ctx->f[1] = f1; if (df_at) Ps2SetGpr64(ctx, 1, sx64(at)); ctx->fcr31 = fcr31;
        ctx->pc = target;
        return;
    }
}

// GAMEUTIL_forward_timer_still_one_ratio_time_interp_ratio_cntrl (0x10842d0): lift_d1's text (the reference)
void GAMEUTIL_forward_timer_still_one_ratio_time_interp_ratio_cntrl(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t a0 = Ps2GprU32(ctx, 4), ra = Ps2GprU32(ctx, 31);
    float f0 = ctx->f[0], f1 = ctx->f[1];
    f0 = lift_d1::flt(READ32_B(a0, 20));
    f1 = lift_d1::flt(READ32_B(a0, 24));
    f0 = FPU_ADD_S(f0, f1);
    {
        const uint32_t target = ra;
        WRITE32_B(a0, 12, lift_d1::bits(f0));
        ctx->f[0] = f0; ctx->f[1] = f1;
        ctx->pc = target;
        return;
    }
}

// GAMEUTIL_get_interp_ratio_interp_ratio_cntrl (0x10842e8): lift_d1's text (the reference)
void GAMEUTIL_get_interp_ratio_interp_ratio_cntrl(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t a0 = Ps2GprU32(ctx, 4), ra = Ps2GprU32(ctx, 31);
    float f0 = ctx->f[0];
    {
        const uint32_t target = ra;
        f0 = lift_d1::flt(READ32_B(a0, 16));
        ctx->f[0] = f0;
        ctx->pc = target;
        return;
    }
}

// GAMEUTIL_get_interp_timer_interp_ratio_cntrl (0x10842f0): lift_d1's text (the reference)
void GAMEUTIL_get_interp_timer_interp_ratio_cntrl(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t a0 = Ps2GprU32(ctx, 4), ra = Ps2GprU32(ctx, 31);
    float f0 = ctx->f[0];
    {
        const uint32_t target = ra;
        f0 = lift_d1::flt(READ32_B(a0, 12));
        ctx->f[0] = f0;
        ctx->pc = target;
        return;
    }
}

// GAMEUTIL_set_act_interp_flag_interp_ratio_cntrl (0x10842f8): lift_d1's text (the reference)
void GAMEUTIL_set_act_interp_flag_interp_ratio_cntrl(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t a0 = Ps2GprU32(ctx, 4), a1 = Ps2GprU32(ctx, 5), ra = Ps2GprU32(ctx, 31);
    {
        const uint32_t target = ra;
        WRITE32(a0, a1);
        
        ctx->pc = target;
        return;
    }
}

// GAMEUTIL_set_act_max_zero_clip_flag_interp_ratio_cntrl (0x1084300): lift_d1's text (the reference)
void GAMEUTIL_set_act_max_zero_clip_flag_interp_ratio_cntrl(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t a0 = Ps2GprU32(ctx, 4), a1 = Ps2GprU32(ctx, 5), ra = Ps2GprU32(ctx, 31);
    {
        const uint32_t target = ra;
        WRITE32_B(a0, 4, a1);
        
        ctx->pc = target;
        return;
    }
}

// GAMEUTIL_get_interp_value_interp_value_cntrl (0x1084500): lift_d1's text (the reference)
void GAMEUTIL_get_interp_value_interp_value_cntrl(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t a0 = Ps2GprU32(ctx, 4), ra = Ps2GprU32(ctx, 31);
    float f0 = ctx->f[0];
    {
        const uint32_t target = ra;
        f0 = lift_d1::flt(READ32_B(a0, 4));
        ctx->f[0] = f0;
        ctx->pc = target;
        return;
    }
}

// GAMEUTIL_get_hope_value_interp_value_cntrl (0x1084508): lift_d1's text (the reference)
void GAMEUTIL_get_hope_value_interp_value_cntrl(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t a0 = Ps2GprU32(ctx, 4), ra = Ps2GprU32(ctx, 31);
    float f0 = ctx->f[0];
    {
        const uint32_t target = ra;
        f0 = lift_d1::flt(READ32_B(a0, 8));
        ctx->f[0] = f0;
        ctx->pc = target;
        return;
    }
}

// set_param_double_spring_interp_common (0x1084510): lift_d1's text (the reference)
void set_param_double_spring_interp_common(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t a0 = Ps2GprU32(ctx, 4), ra = Ps2GprU32(ctx, 31);
    float f12 = ctx->f[12], f13 = ctx->f[13], f14 = ctx->f[14], f15 = ctx->f[15];
    WRITE32_B(a0, 12, lift_d1::bits(f15));
    WRITE32(a0, lift_d1::bits(f12));
    WRITE32_B(a0, 4, lift_d1::bits(f13));
    {
        const uint32_t target = ra;
        WRITE32_B(a0, 8, lift_d1::bits(f14));
        
        ctx->pc = target;
        return;
    }
}

// set_rot_radi_value_flag_double_spring_interp_cntrl (0x1084588): lift_d1's text (the reference)
void set_rot_radi_value_flag_double_spring_interp_cntrl(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t a0 = Ps2GprU32(ctx, 4), a1 = Ps2GprU32(ctx, 5), ra = Ps2GprU32(ctx, 31);
    {
        const uint32_t target = ra;
        WRITE32_B(a0, 20, a1);
        
        ctx->pc = target;
        return;
    }
}

// set_rot_radi_value_flag_double_spring_interp_cntrl3D (0x1084698): lift_d1's text (the reference)
void set_rot_radi_value_flag_double_spring_interp_cntrl3D(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t a0 = Ps2GprU32(ctx, 4), a1 = Ps2GprU32(ctx, 5), a2 = Ps2GprU32(ctx, 6), a3 = Ps2GprU32(ctx, 7), ra = Ps2GprU32(ctx, 31);
    WRITE32_B(a0, 28, a3);
    WRITE32_B(a0, 20, a1);
    {
        const uint32_t target = ra;
        WRITE32_B(a0, 24, a2);
        
        ctx->pc = target;
        return;
    }
}

// constructor_aim_angle_interp_cntrl (0x1084880): lift_d1's text (the reference)
void constructor_aim_angle_interp_cntrl(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t at = Ps2GprU32(ctx, 1), a0 = Ps2GprU32(ctx, 4), a1 = Ps2GprU32(ctx, 5), ra = Ps2GprU32(ctx, 31);
    float f0 = ctx->f[0], f1 = ctx->f[1];
    at = 0x40800000u;
    f0 = lift_d1::flt(at);
    at = 0x3FC90000u;
    at = at | 0xFDAu;
    f1 = lift_d1::flt(at);
    WRITE32(a0, a1);
    WRITE32_B(a0, 4, lift_d1::bits(f0));
    WRITE32_B(a0, 8, lift_d1::bits(f1));
    {
        const uint32_t target = ra;
        WRITE32_B(a0, 12, 0u);
        ctx->f[0] = f0; ctx->f[1] = f1; Ps2SetGpr64(ctx, 1, sx64(at));
        ctx->pc = target;
        return;
    }
}

// reset_aim_angle_interp_cntrl (0x10848a8): lift_d1's text (the reference)
void reset_aim_angle_interp_cntrl(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t a0 = Ps2GprU32(ctx, 4), ra = Ps2GprU32(ctx, 31);
    float f0 = ctx->f[0], f12 = ctx->f[12];
    f0 = FPU_MOV_S(f12);
    {
        const uint32_t target = ra;
        WRITE32_B(a0, 12, lift_d1::bits(f0));
        ctx->f[0] = f0;
        ctx->pc = target;
        return;
    }
}

// set_interp_param_aim_angle_interp_cntrl (0x1084968): lift_d1's text (the reference)
void set_interp_param_aim_angle_interp_cntrl(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t a0 = Ps2GprU32(ctx, 4), ra = Ps2GprU32(ctx, 31);
    float f12 = ctx->f[12], f13 = ctx->f[13];
    WRITE32_B(a0, 8, lift_d1::bits(f13));
    {
        const uint32_t target = ra;
        WRITE32_B(a0, 4, lift_d1::bits(f12));
        
        ctx->pc = target;
        return;
    }
}

// manager_sub_01084978 (0x1084978): lift_d1's text (the reference)
void manager_sub_01084978(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t at = Ps2GprU32(ctx, 1), a0 = Ps2GprU32(ctx, 4), a1 = Ps2GprU32(ctx, 5), ra = Ps2GprU32(ctx, 31);
    uint64_t v0 = Ps2GprU64(ctx, 2);
    float f0 = ctx->f[0], f1 = ctx->f[1], f2 = ctx->f[2], f3 = ctx->f[3], f4 = ctx->f[4], f5 = ctx->f[5], f12 = ctx->f[12], f13 = ctx->f[13];
    uint32_t fcr31 = ctx->fcr31;
    f1 = lift_d1::flt(0u);
    f0 = lift_d1::flt(READ32_B(a0, 12));
    fcr31 = (FPU_C_OLT_S(f1, f0)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
    if ((fcr31 & 0x800000u) != 0u) {
        f0 = lift_d1::flt(READ32_B(a0, 20));
        fcr31 = (FPU_C_EQ_S(f0, f12)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
        if ((fcr31 & 0x800000u) != 0u) {
            f0 = lift_d1::flt(READ32_B(a0, 24));
            fcr31 = (FPU_C_EQ_S(f0, f13)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
            if ((fcr31 & 0x800000u) != 0u) {
                goto L_10849b8;
            }
        }
        {
            const uint32_t target = ra;
            (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 1))));
            ctx->f[0] = f0; ctx->f[1] = f1; Ps2SetGpr64(ctx, 2, v0); ctx->fcr31 = fcr31;
            ctx->pc = target;
            return;
        }
    }
L_10849b8:
    {
        const uint32_t target = ra;
        (v0 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL));
        ctx->f[0] = f0; ctx->f[1] = f1; Ps2SetGpr64(ctx, 2, v0); ctx->fcr31 = fcr31;
        ctx->pc = target;
        return;
    }
}

// getNowBossEffect (0x1087108): lift_d1's text (the reference)
void getNowBossEffect(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t v0 = Ps2GprU32(ctx, 2), v1 = Ps2GprU32(ctx, 3), ra = Ps2GprU32(ctx, 31);
    v1 = 0x1180000u;
    {
        const uint32_t target = ra;
        v0 = READ32_B(v1, 25232);
        Ps2SetGpr64(ctx, 2, sx64(v0)); Ps2SetGpr64(ctx, 3, sx64(v1));
        ctx->pc = target;
        return;
    }
}

// setBossDeathEffect (0x1087118): lift_d1's text (the reference)
void setBossDeathEffect(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t at = Ps2GprU32(ctx, 1), v0 = Ps2GprU32(ctx, 2), a2 = Ps2GprU32(ctx, 6), a3 = Ps2GprU32(ctx, 7), ra = Ps2GprU32(ctx, 31);
    uint64_t v1 = Ps2GprU64(ctx, 3), a0 = Ps2GprU64(ctx, 4), a1 = Ps2GprU64(ctx, 5), t0 = Ps2GprU64(ctx, 8), t1 = Ps2GprU64(ctx, 9);
    float f0 = ctx->f[0], f1 = ctx->f[1], f12 = ctx->f[12];
    (t0 = (uint64_t)((uint64_t)a0 + (uint64_t)0ULL));
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)289 << 16))));
    a2 = 0x1180000u;
    (v0 = (uint32_t)((int32_t)((int32_t)SRL32((uint32_t)t0, 1))));
    a2 = a2 + 0x6290u;
    (t1 = (uint64_t)(t0 & (uint64_t)(uint16_t)1));
    (t1 = (uint64_t)(RC_U64X(t1) | sx64(v0)));
    v0 = READ32(a2);
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)280 << 16))));
    a3 = 0x1180000u;
    v0 = v0 | 1u;
    { float f = f12; uint32_t bits; std::memcpy(&bits, &f, sizeof(bits)); WRITE32_B((uint32_t)v1, 25244, bits); }
    WRITE32(a2, v0);
    (v0 = (uint32_t)((int32_t)((int32_t)SRL32((uint32_t)a1, 1))));
    (v1 = (uint64_t)(a1 & (uint64_t)(uint16_t)1));
    WRITE32_B(a3, 25240, 0u);
    (v1 = (uint64_t)(RC_U64X(v1) | sx64(v0)));
    (v0 = (uint32_t)((int32_t)((int32_t)ADD32((uint32_t)a0, 18240))));
    {
        const bool c = (int32_t)t0 < 0;
        (a0 = (uint64_t)((uint64_t)sx64(v0) + (uint64_t)0ULL));
        if (!c) {
            { uint32_t bits = (uint32_t)t0; std::memcpy(&f1, &bits, sizeof(bits)); }
            { int32_t tmp; std::memcpy(&tmp, &f1, sizeof(tmp)); f1 = FPU_CVT_S_W(tmp); }
            goto L_1087184;
        }
    }
    { uint32_t bits = (uint32_t)t1; std::memcpy(&f1, &bits, sizeof(bits)); }
    { int32_t tmp; std::memcpy(&tmp, &f1, sizeof(tmp)); f1 = FPU_CVT_S_W(tmp); }
    f1 = FPU_ADD_S(f1, f1);
L_1087184:
    at = 0x39800000u;
    { uint32_t bits = at; std::memcpy(&f0, &bits, sizeof(bits)); }
    f0 = FPU_MUL_S(f1, f0);
    {
        const bool c = (int32_t)a1 < 0;
        { float f = f0; uint32_t bits; std::memcpy(&bits, &f, sizeof(bits)); WRITE32_B((uint32_t)a0, 0, bits); }
        if (!c) {
            { uint32_t bits = (uint32_t)a1; std::memcpy(&f1, &bits, sizeof(bits)); }
            { int32_t tmp; std::memcpy(&tmp, &f1, sizeof(tmp)); f1 = FPU_CVT_S_W(tmp); }
            goto L_10871b4;
        }
    }
    { uint32_t bits = (uint32_t)v1; std::memcpy(&f1, &bits, sizeof(bits)); }
    { int32_t tmp; std::memcpy(&tmp, &f1, sizeof(tmp)); f1 = FPU_CVT_S_W(tmp); }
    f1 = FPU_ADD_S(f1, f1);
L_10871b4:
    at = 0x39800000u;
    { uint32_t bits = at; std::memcpy(&f0, &bits, sizeof(bits)); }
    f0 = FPU_MUL_S(f1, f0);
    {
        const uint32_t target = ra;
        WRITE32_B(v0, 4, lift_d1::bits(f0));
        ctx->f[0] = f0; ctx->f[1] = f1; Ps2SetGpr64(ctx, 1, sx64(at)); Ps2SetGpr64(ctx, 2, sx64(v0)); Ps2SetGpr64(ctx, 3, v1); Ps2SetGpr64(ctx, 4, a0); Ps2SetGpr64(ctx, 6, sx64(a2)); Ps2SetGpr64(ctx, 7, sx64(a3)); Ps2SetGpr64(ctx, 8, t0); Ps2SetGpr64(ctx, 9, t1);
        ctx->pc = target;
        return;
    }
}

// getBossEffectBlurLevel (0x1087200): lift_d1's text (the reference)
void getBossEffectBlurLevel(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t v0 = Ps2GprU32(ctx, 2), ra = Ps2GprU32(ctx, 31);
    v0 = 0x1180000u;
    {
        const uint32_t target = ra;
        v0 = v0 + 0x62A0u;
        Ps2SetGpr64(ctx, 2, sx64(v0));
        ctx->pc = target;
        return;
    }
}

// getBossEffectColorR (0x1087210): lift_d1's text (the reference)
void getBossEffectColorR(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t v0 = Ps2GprU32(ctx, 2), ra = Ps2GprU32(ctx, 31);
    v0 = 0x1180000u;
    {
        const uint32_t target = ra;
        v0 = v0 + 0x62B0u;
        Ps2SetGpr64(ctx, 2, sx64(v0));
        ctx->pc = target;
        return;
    }
}

// getBossEffectColorG (0x1087220): lift_d1's text (the reference)
void getBossEffectColorG(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t v0 = Ps2GprU32(ctx, 2), ra = Ps2GprU32(ctx, 31);
    v0 = 0x1180000u;
    {
        const uint32_t target = ra;
        v0 = v0 + 0x62B4u;
        Ps2SetGpr64(ctx, 2, sx64(v0));
        ctx->pc = target;
        return;
    }
}

// getBossEffectColorB (0x1087230): lift_d1's text (the reference)
void getBossEffectColorB(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t v0 = Ps2GprU32(ctx, 2), ra = Ps2GprU32(ctx, 31);
    v0 = 0x1180000u;
    {
        const uint32_t target = ra;
        v0 = v0 + 0x62B8u;
        Ps2SetGpr64(ctx, 2, sx64(v0));
        ctx->pc = target;
        return;
    }
}

// getBossEffectBlendRatio (0x1087240): lift_d1's text (the reference)
void getBossEffectBlendRatio(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t v0 = Ps2GprU32(ctx, 2), ra = Ps2GprU32(ctx, 31);
    v0 = 0x1180000u;
    {
        const uint32_t target = ra;
        v0 = v0 + 0x62C0u;
        Ps2SetGpr64(ctx, 2, sx64(v0));
        ctx->pc = target;
        return;
    }
}

// getBossEffectOutLine (0x1087250): lift_d1's text (the reference)
void getBossEffectOutLine(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t v0 = Ps2GprU32(ctx, 2), ra = Ps2GprU32(ctx, 31);
    v0 = 0x1180000u;
    {
        const uint32_t target = ra;
        v0 = v0 + 0x62C4u;
        Ps2SetGpr64(ctx, 2, sx64(v0));
        ctx->pc = target;
        return;
    }
}

// getDepthOfFieldSw (0x1088190): lift_d1's text (the reference)
void getDepthOfFieldSw(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t v0 = Ps2GprU32(ctx, 2), ra = Ps2GprU32(ctx, 31);
    v0 = 0x1180000u;
    {
        const uint32_t target = ra;
        v0 = v0 + 0x62D0u;
        Ps2SetGpr64(ctx, 2, sx64(v0));
        ctx->pc = target;
        return;
    }
}

// getDepthOfFieldLevel (0x10881a0): lift_d1's text (the reference)
void getDepthOfFieldLevel(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t v0 = Ps2GprU32(ctx, 2), ra = Ps2GprU32(ctx, 31);
    v0 = 0x1180000u;
    {
        const uint32_t target = ra;
        v0 = v0 + 0x62D4u;
        Ps2SetGpr64(ctx, 2, sx64(v0));
        ctx->pc = target;
        return;
    }
}

// getDepthOfFieldPlanes (0x10881b0): lift_d1's text (the reference)
void getDepthOfFieldPlanes(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t v0 = Ps2GprU32(ctx, 2), ra = Ps2GprU32(ctx, 31);
    v0 = 0x1180000u;
    {
        const uint32_t target = ra;
        v0 = v0 + 0x62D8u;
        Ps2SetGpr64(ctx, 2, sx64(v0));
        ctx->pc = target;
        return;
    }
}

// getDepthOfFieldNearZ (0x10881c0): lift_d1's text (the reference)
void getDepthOfFieldNearZ(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t v0 = Ps2GprU32(ctx, 2), ra = Ps2GprU32(ctx, 31);
    v0 = 0x1180000u;
    {
        const uint32_t target = ra;
        v0 = v0 + 0x62DCu;
        Ps2SetGpr64(ctx, 2, sx64(v0));
        ctx->pc = target;
        return;
    }
}

// fsEffectSetParamSheetSeg (0x1088a78): lift_d1's text (the reference)
void fsEffectSetParamSheetSeg(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t v0 = Ps2GprU32(ctx, 2), v1 = Ps2GprU32(ctx, 3), a1 = Ps2GprU32(ctx, 5), a2 = Ps2GprU32(ctx, 6), ra = Ps2GprU32(ctx, 31);
    uint64_t a0 = Ps2GprU64(ctx, 4);
    uint64_t hi = ctx->hi;
    uint64_t lo = ctx->lo;
    bool df_v1 = false, df_a1 = false;
    v0 = 0x30000u;
    (a2 = (uint32_t)((int32_t)((int32_t)READ32_B((uint32_t)a0, 0))));
    v0 = v0 | 0xFFFFu;
    v0 = a2 & v0;
    {
        const bool c = v0 == 0;
        (v0 = (uint32_t)((int32_t)((int32_t)SUB32((uint32_t)a0, v0))));
        if (!c) {
            (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)READ32_B(v0, 0))));
            goto L_1088a9c;
        }
    }
    (a0 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL));
L_1088a9c:
    {
        const bool c = (int64_t)a0 == 0;
        { const uint32_t _dynInsn = READ32(0x1088AA0u); const int32_t _dynSimm = (int16_t)(_dynInsn & 0xFFFFu); const uint32_t _dynImm = _dynInsn & 0xFFFFu; (void)_dynSimm; (void)_dynImm; (v0 = (uint32_t)((int32_t)((int32_t)((uint32_t)_dynImm << 16)))); }
        if (!c) {
            { const uint32_t _dynInsn = READ32(0x1088AA4u); const int32_t _dynSimm = (int16_t)(_dynInsn & 0xFFFFu); const uint32_t _dynImm = _dynInsn & 0xFFFFu; (void)_dynSimm; (void)_dynImm; (v1 = (uint32_t)((int32_t)((int32_t)READ32(ADD32(v0, _dynSimm))))); } df_v1 = true;
            {
                const bool c = (int64_t)a0 == (int64_t)(int32_t)v1;
                v0 = a2 >> 18;
                if (!c) {
                    (a1 = (uint32_t)((int32_t)((int32_t)READ32_B((uint32_t)a0, 8)))); df_a1 = true;
                    v0 = v0 & 0xFFFu;
                    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)READ32_B((uint32_t)a0, 16))));
                    v1 = READ32_B(a1, 12); df_v1 = true;
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
            v0 = READ32(a1);
            if (v0 == 0) {
                WRITE32_B(a1, 0, (uint32_t)a0);
            } else {
                v1 = READ32_B(v0, 4); df_v1 = true;
                (v0 = (uint32_t)((int32_t)((int32_t)READ32_B((uint32_t)a0, 4))));
                v1 = v1 << 4; df_v1 = true;
                v0 = v0 << 4;
                v0 = (int32_t)v0 < (int32_t)v1 ? 1u : 0u;
                if (v0 == 0) {
                    WRITE32_B(a1, 0, (uint32_t)a0);
                } else {
                }
            }
        }
    }
    {
        const uint32_t target = ra;
        Ps2SetGpr64(ctx, 2, sx64(v0)); if (df_v1) Ps2SetGpr64(ctx, 3, sx64(v1)); Ps2SetGpr64(ctx, 4, a0); if (df_a1) Ps2SetGpr64(ctx, 5, sx64(a1)); Ps2SetGpr64(ctx, 6, sx64(a2)); ctx->hi = hi; ctx->lo = lo;
        ctx->pc = target;
        return;
    }
}

// getFogMode (0x10898f8): lift_d1's text (the reference)
void getFogMode(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t v0 = Ps2GprU32(ctx, 2), ra = Ps2GprU32(ctx, 31);
    v0 = 0x1210000u;
    {
        const uint32_t target = ra;
        v0 = v0 + 0x47B0u;
        Ps2SetGpr64(ctx, 2, sx64(v0));
        ctx->pc = target;
        return;
    }
}

// getFogNearDist (0x1089908): lift_d1's text (the reference)
void getFogNearDist(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t v0 = Ps2GprU32(ctx, 2), ra = Ps2GprU32(ctx, 31);
    v0 = 0x1210000u;
    {
        const uint32_t target = ra;
        v0 = v0 + 0x47B4u;
        Ps2SetGpr64(ctx, 2, sx64(v0));
        ctx->pc = target;
        return;
    }
}

// getFogFarDist (0x1089918): lift_d1's text (the reference)
void getFogFarDist(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t v0 = Ps2GprU32(ctx, 2), ra = Ps2GprU32(ctx, 31);
    v0 = 0x1210000u;
    {
        const uint32_t target = ra;
        v0 = v0 + 0x47B8u;
        Ps2SetGpr64(ctx, 2, sx64(v0));
        ctx->pc = target;
        return;
    }
}

// getFogNearRate (0x1089928): lift_d1's text (the reference)
void getFogNearRate(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t v0 = Ps2GprU32(ctx, 2), ra = Ps2GprU32(ctx, 31);
    v0 = 0x1210000u;
    {
        const uint32_t target = ra;
        v0 = v0 + 0x47BCu;
        Ps2SetGpr64(ctx, 2, sx64(v0));
        ctx->pc = target;
        return;
    }
}

// getFogFarRate (0x1089938): lift_d1's text (the reference)
void getFogFarRate(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t v0 = Ps2GprU32(ctx, 2), ra = Ps2GprU32(ctx, 31);
    v0 = 0x1210000u;
    {
        const uint32_t target = ra;
        v0 = v0 + 0x47C0u;
        Ps2SetGpr64(ctx, 2, sx64(v0));
        ctx->pc = target;
        return;
    }
}

// getFogOffset (0x1089958): lift_d1's text (the reference)
void getFogOffset(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t v0 = Ps2GprU32(ctx, 2), ra = Ps2GprU32(ctx, 31);
    v0 = 0x1210000u;
    {
        const uint32_t target = ra;
        v0 = v0 + 0x47C8u;
        Ps2SetGpr64(ctx, 2, sx64(v0));
        ctx->pc = target;
        return;
    }
}

// getFogUnderWater (0x1089968): lift_d1's text (the reference)
void getFogUnderWater(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t v0 = Ps2GprU32(ctx, 2), ra = Ps2GprU32(ctx, 31);
    v0 = 0x1180000u;
    {
        const uint32_t target = ra;
        v0 = v0 + 0x6328u;
        Ps2SetGpr64(ctx, 2, sx64(v0));
        ctx->pc = target;
        return;
    }
}

// getFogColorR (0x1089978): lift_d1's text (the reference)
void getFogColorR(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t v0 = Ps2GprU32(ctx, 2), ra = Ps2GprU32(ctx, 31);
    v0 = 0x1210000u;
    {
        const uint32_t target = ra;
        v0 = v0 + 0x47CCu;
        Ps2SetGpr64(ctx, 2, sx64(v0));
        ctx->pc = target;
        return;
    }
}

// getFogColorG (0x1089988): lift_d1's text (the reference)
void getFogColorG(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t v0 = Ps2GprU32(ctx, 2), ra = Ps2GprU32(ctx, 31);
    v0 = 0x1210000u;
    {
        const uint32_t target = ra;
        v0 = v0 + 0x47D0u;
        Ps2SetGpr64(ctx, 2, sx64(v0));
        ctx->pc = target;
        return;
    }
}

// getFogColorB (0x1089998): lift_d1's text (the reference)
void getFogColorB(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t v0 = Ps2GprU32(ctx, 2), ra = Ps2GprU32(ctx, 31);
    v0 = 0x1210000u;
    {
        const uint32_t target = ra;
        v0 = v0 + 0x47D4u;
        Ps2SetGpr64(ctx, 2, sx64(v0));
        ctx->pc = target;
        return;
    }
}

// SeemlessInitFogTargetParamWorldLayout (0x1089a38): lift_d1's text (the reference)
void SeemlessInitFogTargetParamWorldLayout(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t at = Ps2GprU32(ctx, 1), v0 = Ps2GprU32(ctx, 2), v1 = Ps2GprU32(ctx, 3), a0 = Ps2GprU32(ctx, 4), a1 = Ps2GprU32(ctx, 5), a2 = Ps2GprU32(ctx, 6), a3 = Ps2GprU32(ctx, 7), ra = Ps2GprU32(ctx, 31);
    float f0 = ctx->f[0], f1 = ctx->f[1], f2 = ctx->f[2];
    f0 = lift_d1::flt(READ32_B(a0, 32));
    a1 = 0x1210000u;
    a1 = a1 + 0x47B0u;
    at = 0x3F800000u;
    f2 = lift_d1::flt(at);
    WRITE32_B(a1, 4, lift_d1::bits(f0));
    a3 = 0x1180000u;
    a2 = 0x78u;
    f0 = lift_d1::flt(READ32_B(a0, 36));
    WRITE32_B(a1, 8, lift_d1::bits(f0));
    f1 = lift_d1::flt(READ32_B(a0, 40));
    WRITE32_B(a1, 12, lift_d1::bits(f1));
    f0 = lift_d1::flt(READ32_B(a0, 44));
    WRITE32_B(a1, 20, lift_d1::bits(f2));
    WRITE32_B(a1, 16, lift_d1::bits(f0));
    v0 = READ8_B(a0, 48);
    WRITE32_B(a1, 24, v0);
    v1 = READ8_B(a0, 49);
    WRITE32_B(a1, 28, v1);
    v0 = READ8_B(a0, 50);
    WRITE32_B(a1, 32, v0);
    v1 = READ8_B(a0, 51);
    WRITE32_B(a3, 25376, a2);
    {
        const uint32_t target = ra;
        WRITE32_B(a1, 36, v1);
        ctx->f[0] = f0; ctx->f[1] = f1; ctx->f[2] = f2; Ps2SetGpr64(ctx, 1, sx64(at)); Ps2SetGpr64(ctx, 2, sx64(v0)); Ps2SetGpr64(ctx, 3, sx64(v1)); Ps2SetGpr64(ctx, 5, sx64(a1)); Ps2SetGpr64(ctx, 6, sx64(a2)); Ps2SetGpr64(ctx, 7, sx64(a3));
        ctx->pc = target;
        return;
    }
}

// fogSetFogColorMulRatio (0x1089b60): lift_d1's text (the reference)
void fogSetFogColorMulRatio(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t v0 = Ps2GprU32(ctx, 2), ra = Ps2GprU32(ctx, 31);
    float f12 = ctx->f[12];
    v0 = 0x1180000u;
    {
        const uint32_t target = ra;
        WRITE32_B(v0, 25380, lift_d1::bits(f12));
        Ps2SetGpr64(ctx, 2, sx64(v0));
        ctx->pc = target;
        return;
    }
}

// getFogFactor1 (0x1089bb0): lift_d1's text (the reference)
void getFogFactor1(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t v0 = Ps2GprU32(ctx, 2), ra = Ps2GprU32(ctx, 31);
    float f0 = ctx->f[0], f1 = ctx->f[1], f2 = ctx->f[2], f3 = ctx->f[3], f4 = ctx->f[4], f5 = ctx->f[5];
    uint32_t fcr31 = ctx->fcr31;
    v0 = 0x1210000u;
    v0 = v0 + 0x47D8u;
    f2 = lift_d1::flt(READ32_B(v0, 12));
    f3 = lift_d1::flt(READ32_B(v0, 16));
    f1 = lift_d1::flt(READ32_B(v0, 8));
    f4 = lift_d1::flt(READ32_B(v0, 4));
    f0 = FPU_SUB_S(f3, f2);
    f3 = FPU_ADD_S(f3, f2);
    f5 = lift_d1::flt(READ32_B(v0, 20));
    f2 = FPU_ADD_S(f1, f4);
    f1 = FPU_SUB_S(f1, f4);
    f0 = FPU_MUL_SF(f0, f2);
    f0 = ps2FpuDivide(fcr31, f0, f1);
    f0 = FPU_ADD_SF(f0, f3);
    {
        const uint32_t target = ra;
        f0 = FPU_MUL_S(f0, f5);
        ctx->f[0] = f0; ctx->f[1] = f1; ctx->f[2] = f2; ctx->f[3] = f3; ctx->f[4] = f4; ctx->f[5] = f5; Ps2SetGpr64(ctx, 2, sx64(v0)); ctx->fcr31 = fcr31;
        ctx->pc = target;
        return;
    }
}

// setFadeColor (0x108b970): lift_d1's text (the reference)
void setFadeColor(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t v0 = Ps2GprU32(ctx, 2), v1 = Ps2GprU32(ctx, 3), ra = Ps2GprU32(ctx, 31);
    uint64_t a0 = Ps2GprU64(ctx, 4);
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
            WRITE32_B(v0, 8, v1);
            WRITE32(v0, v1);
            {
                const uint32_t target = ra;
                WRITE32_B(v0, 4, v1);
                Ps2SetGpr64(ctx, 2, sx64(v0)); Ps2SetGpr64(ctx, 3, sx64(v1));
                ctx->pc = target;
                return;
            }
        }
    }
    v0 = 0x1180000u;
    v0 = v0 + 0x6340u;
    WRITE32_B(v0, 8, 0u);
    WRITE32(v0, 0u);
    WRITE32_B(v0, 4, 0u);
L_108b9ac:
    {
        const uint32_t target = ra;
        Ps2SetGpr64(ctx, 2, sx64(v0));
        ctx->pc = target;
        return;
    }
}

// setFadeLayer (0x108b9b8): lift_d1's text (the reference)
void setFadeLayer(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t v0 = Ps2GprU32(ctx, 2), a0 = Ps2GprU32(ctx, 4), ra = Ps2GprU32(ctx, 31);
    v0 = 0x1180000u;
    {
        const uint32_t target = ra;
        WRITE32_B(v0, 25424, a0);
        Ps2SetGpr64(ctx, 2, sx64(v0));
        ctx->pc = target;
        return;
    }
}

// getCameraBlurSw (0x108ba28): lift_d1's text (the reference)
void getCameraBlurSw(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t v0 = Ps2GprU32(ctx, 2), ra = Ps2GprU32(ctx, 31);
    v0 = 0x1180000u;
    {
        const uint32_t target = ra;
        v0 = v0 + 0x6354u;
        Ps2SetGpr64(ctx, 2, sx64(v0));
        ctx->pc = target;
        return;
    }
}

// getBlurAlsoChara (0x108ba38): lift_d1's text (the reference)
void getBlurAlsoChara(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t v0 = Ps2GprU32(ctx, 2), ra = Ps2GprU32(ctx, 31);
    v0 = 0x1180000u;
    {
        const uint32_t target = ra;
        v0 = v0 + 0x6358u;
        Ps2SetGpr64(ctx, 2, sx64(v0));
        ctx->pc = target;
        return;
    }
}

// getBlurWorkSize (0x108ba48): lift_d1's text (the reference)
void getBlurWorkSize(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t v0 = Ps2GprU32(ctx, 2), ra = Ps2GprU32(ctx, 31);
    v0 = 0x1180000u;
    {
        const uint32_t target = ra;
        v0 = v0 + 0x635Cu;
        Ps2SetGpr64(ctx, 2, sx64(v0));
        ctx->pc = target;
        return;
    }
}

// getNBlurPlanes (0x108ba58): lift_d1's text (the reference)
void getNBlurPlanes(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t v0 = Ps2GprU32(ctx, 2), ra = Ps2GprU32(ctx, 31);
    v0 = 0x1180000u;
    {
        const uint32_t target = ra;
        v0 = v0 + 0x6360u;
        Ps2SetGpr64(ctx, 2, sx64(v0));
        ctx->pc = target;
        return;
    }
}

// getBlurIntensity (0x108ba68): lift_d1's text (the reference)
void getBlurIntensity(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t v0 = Ps2GprU32(ctx, 2), ra = Ps2GprU32(ctx, 31);
    v0 = 0x1180000u;
    {
        const uint32_t target = ra;
        v0 = v0 + 0x6364u;
        Ps2SetGpr64(ctx, 2, sx64(v0));
        ctx->pc = target;
        return;
    }
}

// getBlurZBase (0x108ba78): lift_d1's text (the reference)
void getBlurZBase(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t v0 = Ps2GprU32(ctx, 2), ra = Ps2GprU32(ctx, 31);
    v0 = 0x1180000u;
    {
        const uint32_t target = ra;
        v0 = v0 + 0x636Cu;
        Ps2SetGpr64(ctx, 2, sx64(v0));
        ctx->pc = target;
        return;
    }
}

// getBlurMinThreshold (0x108ba88): lift_d1's text (the reference)
void getBlurMinThreshold(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t v0 = Ps2GprU32(ctx, 2), ra = Ps2GprU32(ctx, 31);
    v0 = 0x1180000u;
    {
        const uint32_t target = ra;
        v0 = v0 + 0x6370u;
        Ps2SetGpr64(ctx, 2, sx64(v0));
        ctx->pc = target;
        return;
    }
}

// getBlurMaxThreshold (0x108ba98): lift_d1's text (the reference)
void getBlurMaxThreshold(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t v0 = Ps2GprU32(ctx, 2), ra = Ps2GprU32(ctx, 31);
    v0 = 0x1180000u;
    {
        const uint32_t target = ra;
        v0 = v0 + 0x6374u;
        Ps2SetGpr64(ctx, 2, sx64(v0));
        ctx->pc = target;
        return;
    }
}

// getBlurShutterSpeed (0x108baa8): lift_d1's text (the reference)
void getBlurShutterSpeed(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t v0 = Ps2GprU32(ctx, 2), ra = Ps2GprU32(ctx, 31);
    v0 = 0x1180000u;
    {
        const uint32_t target = ra;
        v0 = v0 + 0x6378u;
        Ps2SetGpr64(ctx, 2, sx64(v0));
        ctx->pc = target;
        return;
    }
}

// getBlurXYZRatio (0x108bab8): lift_d1's text (the reference)
void getBlurXYZRatio(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t v0 = Ps2GprU32(ctx, 2), ra = Ps2GprU32(ctx, 31);
    v0 = 0x1180000u;
    {
        const uint32_t target = ra;
        v0 = v0 + 0x637Cu;
        Ps2SetGpr64(ctx, 2, sx64(v0));
        ctx->pc = target;
        return;
    }
}

// getBlurCamMoveThreshold (0x108bac8): lift_d1's text (the reference)
void getBlurCamMoveThreshold(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t v0 = Ps2GprU32(ctx, 2), ra = Ps2GprU32(ctx, 31);
    v0 = 0x1180000u;
    {
        const uint32_t target = ra;
        v0 = v0 + 0x6380u;
        Ps2SetGpr64(ctx, 2, sx64(v0));
        ctx->pc = target;
        return;
    }
}

// glTexCoord2f (0x108e9c8): lift_d1's text (the reference)
void glTexCoord2f(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t at = Ps2GprU32(ctx, 1), v0 = Ps2GprU32(ctx, 2), ra = Ps2GprU32(ctx, 31);
    float f0 = ctx->f[0], f12 = ctx->f[12], f13 = ctx->f[13];
    v0 = 0x1180000u;
    at = 0x3F800000u;
    f0 = lift_d1::flt(at);
    v0 = v0 + 0x6390u;
    WRITE32_B(v0, 160, lift_d1::bits(f12));
    WRITE32_B(v0, 164, lift_d1::bits(f13));
    WRITE32_B(v0, 168, lift_d1::bits(f0));
    {
        const uint32_t target = ra;
        WRITE32_B(v0, 172, 0u);
        ctx->f[0] = f0; Ps2SetGpr64(ctx, 1, sx64(at)); Ps2SetGpr64(ctx, 2, sx64(v0));
        ctx->pc = target;
        return;
    }
}

// glTexCoord2fv (0x108e9f0): lift_d1's text (the reference)
void glTexCoord2fv(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t at = Ps2GprU32(ctx, 1), v0 = Ps2GprU32(ctx, 2), a0 = Ps2GprU32(ctx, 4), ra = Ps2GprU32(ctx, 31);
    float f0 = ctx->f[0], f1 = ctx->f[1], f2 = ctx->f[2];
    f1 = lift_d1::flt(READ32(a0));
    v0 = 0x1180000u;
    v0 = v0 + 0x6390u;
    at = 0x3F800000u;
    f2 = lift_d1::flt(at);
    WRITE32_B(v0, 160, lift_d1::bits(f1));
    f0 = lift_d1::flt(READ32_B(a0, 4));
    WRITE32_B(v0, 168, lift_d1::bits(f2));
    WRITE32_B(v0, 164, lift_d1::bits(f0));
    {
        const uint32_t target = ra;
        WRITE32_B(v0, 172, 0u);
        ctx->f[0] = f0; ctx->f[1] = f1; ctx->f[2] = f2; Ps2SetGpr64(ctx, 1, sx64(at)); Ps2SetGpr64(ctx, 2, sx64(v0));
        ctx->pc = target;
        return;
    }
}

// glZOffset (0x108f3b0): lift_d1's text (the reference)
void glZOffset(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t v0 = Ps2GprU32(ctx, 2), ra = Ps2GprU32(ctx, 31);
    float f12 = ctx->f[12];
    v0 = 0x1190000u;
    {
        const uint32_t target = ra;
        WRITE32(v0 - 0x5A7Cu, lift_d1::bits(f12));
        Ps2SetGpr64(ctx, 2, sx64(v0));
        ctx->pc = target;
        return;
    }
}

// glSetFogFactorDirect (0x108f608): lift_d1's text (the reference)
void glSetFogFactorDirect(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t v0 = Ps2GprU32(ctx, 2), a0 = Ps2GprU32(ctx, 4), ra = Ps2GprU32(ctx, 31);
    v0 = 0x1180000u;
    {
        const uint32_t target = ra;
        WRITE32_B(v0, 25584, a0);
        Ps2SetGpr64(ctx, 2, sx64(v0));
        ctx->pc = target;
        return;
    }
}

// gluGetBrightness (0x108f7e0): lift_d1's text (the reference)
void gluGetBrightness(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t at = Ps2GprU32(ctx, 1), a0 = Ps2GprU32(ctx, 4), ra = Ps2GprU32(ctx, 31);
    float f0 = ctx->f[0], f1 = ctx->f[1], f2 = ctx->f[2], f3 = ctx->f[3], f4 = ctx->f[4];
    at = 0x3E990000u;
    at = at | 0x1687u;
    f0 = lift_d1::flt(at);
    f2 = lift_d1::flt(READ32(a0));
    f3 = lift_d1::flt(READ32_B(a0, 4));
    at = 0x3F160000u;
    at = at | 0x45A1u;
    f4 = lift_d1::flt(at);
    f2 = FPU_MUL_S(f2, f0);
    at = 0x3DE90000u;
    at = at | 0x78D4u;
    f1 = lift_d1::flt(at);
    f3 = FPU_MUL_S(f3, f4);
    f0 = lift_d1::flt(READ32_B(a0, 8));
    f0 = FPU_MUL_S(f0, f1);
    f2 = FPU_ADD_SF(f2, f3);
    {
        const uint32_t target = ra;
        f0 = FPU_ADD_S(f2, f0);
        ctx->f[0] = f0; ctx->f[1] = f1; ctx->f[2] = f2; ctx->f[3] = f3; ctx->f[4] = f4; Ps2SetGpr64(ctx, 1, sx64(at));
        ctx->pc = target;
        return;
    }
}

// getMBlur (0x108fba8): lift_d1's text (the reference)
void getMBlur(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t v0 = Ps2GprU32(ctx, 2), ra = Ps2GprU32(ctx, 31);
    v0 = 0x1190000u;
    {
        const uint32_t target = ra;
        v0 = v0 - 0x48C0u;
        Ps2SetGpr64(ctx, 2, sx64(v0));
        ctx->pc = target;
        return;
    }
}

// getBrightness (0x108fbc8): lift_d1's text (the reference)
void getBrightness(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t v0 = Ps2GprU32(ctx, 2), ra = Ps2GprU32(ctx, 31);
    v0 = 0x1190000u;
    {
        const uint32_t target = ra;
        v0 = v0 - 0x48BCu;
        Ps2SetGpr64(ctx, 2, sx64(v0));
        ctx->pc = target;
        return;
    }
}

// kSetTex (0x10916f8): lift_d1's text (the reference)
void kSetTex(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t v0 = Ps2GprU32(ctx, 2), a0 = Ps2GprU32(ctx, 4), ra = Ps2GprU32(ctx, 31);
    v0 = 0x1190000u;
    {
        const uint32_t target = ra;
        WRITE32(v0 - 0x45B0u, a0);
        Ps2SetGpr64(ctx, 2, sx64(v0));
        ctx->pc = target;
        return;
    }
}

// getSpecularity (0x1092470): lift_d1's text (the reference)
void getSpecularity(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t v0 = Ps2GprU32(ctx, 2), ra = Ps2GprU32(ctx, 31);
    v0 = 0x1190000u;
    {
        const uint32_t target = ra;
        v0 = v0 - 0x45A0u;
        Ps2SetGpr64(ctx, 2, sx64(v0));
        ctx->pc = target;
        return;
    }
}

// modelDeleteDispModelClass (0x1099590): lift_d1's text (the reference)
void modelDeleteDispModelClass(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint64_t v0 = Ps2GprU64(ctx, 2), v1 = Ps2GprU64(ctx, 3), a0 = Ps2GprU64(ctx, 4), a1 = Ps2GprU64(ctx, 5), ra = Ps2GprU64(ctx, 31);
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)READ32_B((uint32_t)a0, 0))));
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)65280 << 16))));
    (a0 = (uint64_t)(RC_U64X(a0) | (uint64_t)(uint16_t)511));
    (a1 = (uint64_t)((int64_t)(int32_t)((int32_t)READ32_B((uint32_t)v0, 32))));
    (v1 = (uint64_t)(READ64_B((uint32_t)a1, 24)));
    (v0 = (uint64_t)(v1 << 23));
    (v0 = (uint64_t)((int64_t)v0 >> (32 + 0)));
    (v1 = (uint64_t)(v1 & RC_U64X(a0)));
    (v0 = (uint64_t)(v0 & (uint64_t)(uint16_t)32767));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v0, 4294967295))));
    (v0 = (uint64_t)(RC_U64X(v0) & (uint64_t)(uint16_t)32767));
    (v0 = (uint64_t)(RC_U64X(v0) << 9));
    (v1 = (uint64_t)(v1 | v0));
    {
        const uint32_t target = (uint32_t)ra;
        WRITE64_B((uint32_t)a1, 24, v1);
        Ps2SetGpr64(ctx, 2, v0); Ps2SetGpr64(ctx, 3, v1); Ps2SetGpr64(ctx, 4, a0); Ps2SetGpr64(ctx, 5, a1);
        ctx->pc = target;
        return;
    }
}

// modelGetWorkSize (0x1099658): lift_d1's text (the reference)
void modelGetWorkSize(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t v0 = Ps2GprU32(ctx, 2), v1 = Ps2GprU32(ctx, 3), a0 = Ps2GprU32(ctx, 4), a1 = Ps2GprU32(ctx, 5), ra = Ps2GprU32(ctx, 31);
    a1 = READ32_B(a0, 100);
    v1 = READ32_B(a0, 84);
    v0 = a1 << 1;
    v0 = v0 + a1;
    v1 = v1 << 6;
    v0 = v0 << 4;
    v0 = v0 + v1;
    {
        const uint32_t target = ra;
        v0 = v0 + 0xA1u;
        Ps2SetGpr64(ctx, 2, sx64(v0)); Ps2SetGpr64(ctx, 3, sx64(v1)); Ps2SetGpr64(ctx, 5, sx64(a1));
        ctx->pc = target;
        return;
    }
}

// SetDisplayModelNearClippingSwitch (0x1099758): lift_d1's text (the reference)
void SetDisplayModelNearClippingSwitch(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t v0 = Ps2GprU32(ctx, 2), a0 = Ps2GprU32(ctx, 4), ra = Ps2GprU32(ctx, 31);
    v0 = 0x1190000u;
    {
        const uint32_t target = ra;
        WRITE32(v0 - 0x402Cu, a0);
        Ps2SetGpr64(ctx, 2, sx64(v0));
        ctx->pc = target;
        return;
    }
}

// ptclmanGet (0x109a3d8): lift_d1's text (the reference)
void ptclmanGet(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t v0 = Ps2GprU32(ctx, 2), v1 = Ps2GprU32(ctx, 3), a0 = Ps2GprU32(ctx, 4), ra = Ps2GprU32(ctx, 31);
    v1 = 0x1210000u;
    v1 = v1 + 0x5BE8u;
    v0 = READ32_B(v1, 4);
    a0 = READ32(v1);
    v0 = v0 << 4;
    {
        const uint32_t target = ra;
        v0 = a0 + v0;
        Ps2SetGpr64(ctx, 2, sx64(v0)); Ps2SetGpr64(ctx, 3, sx64(v1)); Ps2SetGpr64(ctx, 4, sx64(a0));
        ctx->pc = target;
        return;
    }
}

// ptclmanPut (0x109a3f8): lift_d1's text (the reference)
void ptclmanPut(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t v0 = Ps2GprU32(ctx, 2), v1 = Ps2GprU32(ctx, 3), a0 = Ps2GprU32(ctx, 4), ra = Ps2GprU32(ctx, 31);
    v1 = 0x1210000u;
    v1 = v1 + 0x5BE8u;
    v0 = READ32_B(v1, 4);
    v0 = v0 + a0;
    {
        const uint32_t target = ra;
        WRITE32_B(v1, 4, v0);
        Ps2SetGpr64(ctx, 2, sx64(v0)); Ps2SetGpr64(ctx, 3, sx64(v1));
        ctx->pc = target;
        return;
    }
}

// ptclCounterGet (0x109a4d0): lift_d1's text (the reference)
void ptclCounterGet(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t ra = Ps2GprU32(ctx, 31);
    uint64_t v0 = Ps2GprU64(ctx, 2);
    {
        const uint32_t target = ra;
        (v0 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL));
        Ps2SetGpr64(ctx, 2, v0);
        ctx->pc = target;
        return;
    }
}

// getRefBlurLevel (0x109be88): lift_d1's text (the reference)
void getRefBlurLevel(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t v0 = Ps2GprU32(ctx, 2), ra = Ps2GprU32(ctx, 31);
    v0 = 0x1190000u;
    {
        const uint32_t target = ra;
        v0 = v0 - 0x4000u;
        Ps2SetGpr64(ctx, 2, sx64(v0));
        ctx->pc = target;
        return;
    }
}

// getIsInWater (0x109d0d8): lift_d1's text (the reference)
void getIsInWater(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t v0 = Ps2GprU32(ctx, 2), ra = Ps2GprU32(ctx, 31);
    v0 = 0x1190000u;
    {
        const uint32_t target = ra;
        v0 = v0 - 0x3FCCu;
        Ps2SetGpr64(ctx, 2, sx64(v0));
        ctx->pc = target;
        return;
    }
}

// getRefractBlurSpeed (0x109d0e8): lift_d1's text (the reference)
void getRefractBlurSpeed(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t v0 = Ps2GprU32(ctx, 2), ra = Ps2GprU32(ctx, 31);
    v0 = 0x1190000u;
    {
        const uint32_t target = ra;
        v0 = v0 - 0x3FC8u;
        Ps2SetGpr64(ctx, 2, sx64(v0));
        ctx->pc = target;
        return;
    }
}

// getRefractBlurSpeedRand (0x109d0f8): lift_d1's text (the reference)
void getRefractBlurSpeedRand(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t v0 = Ps2GprU32(ctx, 2), ra = Ps2GprU32(ctx, 31);
    v0 = 0x1190000u;
    {
        const uint32_t target = ra;
        v0 = v0 - 0x3FC4u;
        Ps2SetGpr64(ctx, 2, sx64(v0));
        ctx->pc = target;
        return;
    }
}

// getRefractBlurLevel (0x109d108): lift_d1's text (the reference)
void getRefractBlurLevel(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t v0 = Ps2GprU32(ctx, 2), ra = Ps2GprU32(ctx, 31);
    v0 = 0x1190000u;
    {
        const uint32_t target = ra;
        v0 = v0 - 0x3FC0u;
        Ps2SetGpr64(ctx, 2, sx64(v0));
        ctx->pc = target;
        return;
    }
}

// getRefractBlurLevelRand (0x109d118): lift_d1's text (the reference)
void getRefractBlurLevelRand(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t v0 = Ps2GprU32(ctx, 2), ra = Ps2GprU32(ctx, 31);
    v0 = 0x1190000u;
    {
        const uint32_t target = ra;
        v0 = v0 - 0x3FBCu;
        Ps2SetGpr64(ctx, 2, sx64(v0));
        ctx->pc = target;
        return;
    }
}

// getRefractLengthRatio (0x109d128): lift_d1's text (the reference)
void getRefractLengthRatio(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t v0 = Ps2GprU32(ctx, 2), ra = Ps2GprU32(ctx, 31);
    v0 = 0x1190000u;
    {
        const uint32_t target = ra;
        v0 = v0 - 0x3FB8u;
        Ps2SetGpr64(ctx, 2, sx64(v0));
        ctx->pc = target;
        return;
    }
}

// getRefractBlendRatio (0x109d138): lift_d1's text (the reference)
void getRefractBlendRatio(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t v0 = Ps2GprU32(ctx, 2), ra = Ps2GprU32(ctx, 31);
    v0 = 0x1190000u;
    {
        const uint32_t target = ra;
        v0 = v0 - 0x3FB4u;
        Ps2SetGpr64(ctx, 2, sx64(v0));
        ctx->pc = target;
        return;
    }
}

// getRefractBrightness (0x109d148): lift_d1's text (the reference)
void getRefractBrightness(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t v0 = Ps2GprU32(ctx, 2), ra = Ps2GprU32(ctx, 31);
    v0 = 0x1190000u;
    {
        const uint32_t target = ra;
        v0 = v0 - 0x3FB0u;
        Ps2SetGpr64(ctx, 2, sx64(v0));
        ctx->pc = target;
        return;
    }
}

// getShadowIntensity (0x109de78): lift_d1's text (the reference)
void getShadowIntensity(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t v0 = Ps2GprU32(ctx, 2), ra = Ps2GprU32(ctx, 31);
    v0 = 0x1190000u;
    {
        const uint32_t target = ra;
        v0 = v0 - 0x3F7Cu;
        Ps2SetGpr64(ctx, 2, sx64(v0));
        ctx->pc = target;
        return;
    }
}

// GetShadowLength (0x109dea8): lift_d1's text (the reference)
void GetShadowLength(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t a0 = Ps2GprU32(ctx, 4), ra = Ps2GprU32(ctx, 31);
    float f0 = ctx->f[0];
    {
        const uint32_t target = ra;
        f0 = lift_d1::flt(READ32_B(a0, 24));
        ctx->f[0] = f0;
        ctx->pc = target;
        return;
    }
}

// shadowModelReInit (0x109df30): lift_d1's text (the reference)
void shadowModelReInit(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint64_t v0 = Ps2GprU64(ctx, 2), v1 = Ps2GprU64(ctx, 3), a0 = Ps2GprU64(ctx, 4), a1 = Ps2GprU64(ctx, 5), a2 = Ps2GprU64(ctx, 6), ra = Ps2GprU64(ctx, 31);
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)READ32_B((uint32_t)a0, 20))));
    (a2 = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)12288 << 16))));
    (v1 = (uint64_t)(uint32_t)(READ32_B((uint32_t)a0, 16)));
    (v0 = (uint64_t)((int64_t)(int32_t)(SRA32((int32_t)v0, 4))));
    (a1 = (uint64_t)((int64_t)(int32_t)((int32_t)READ32_B((uint32_t)a0, 28))));
    (v0 = (uint64_t)(RC_U64X(v0) | RC_U64X(a2)));
    (v1 = (uint64_t)(v1 << (32 + 0)));
    (v0 = (uint64_t)(RC_U64X(v0) | v1));
    {
        const uint32_t target = (uint32_t)ra;
        WRITE64_B((uint32_t)a1, 0, v0);
        Ps2SetGpr64(ctx, 2, v0); Ps2SetGpr64(ctx, 3, v1); Ps2SetGpr64(ctx, 5, a1); Ps2SetGpr64(ctx, 6, a2);
        ctx->pc = target;
        return;
    }
}

// getSolarFlareSw (0x10a1728): lift_d1's text (the reference)
void getSolarFlareSw(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t v0 = Ps2GprU32(ctx, 2), ra = Ps2GprU32(ctx, 31);
    v0 = 0x1190000u;
    {
        const uint32_t target = ra;
        v0 = v0 - 0x3F70u;
        Ps2SetGpr64(ctx, 2, sx64(v0));
        ctx->pc = target;
        return;
    }
}

// getSolarFlareBlur (0x10a1738): lift_d1's text (the reference)
void getSolarFlareBlur(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t v0 = Ps2GprU32(ctx, 2), ra = Ps2GprU32(ctx, 31);
    v0 = 0x1190000u;
    {
        const uint32_t target = ra;
        v0 = v0 - 0x3F64u;
        Ps2SetGpr64(ctx, 2, sx64(v0));
        ctx->pc = target;
        return;
    }
}

// getSolarFlareColorR (0x10a1748): lift_d1's text (the reference)
void getSolarFlareColorR(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t v0 = Ps2GprU32(ctx, 2), ra = Ps2GprU32(ctx, 31);
    v0 = 0x1190000u;
    {
        const uint32_t target = ra;
        v0 = v0 - 0x3F58u;
        Ps2SetGpr64(ctx, 2, sx64(v0));
        ctx->pc = target;
        return;
    }
}

// getSolarFlareColorG (0x10a1758): lift_d1's text (the reference)
void getSolarFlareColorG(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t v0 = Ps2GprU32(ctx, 2), ra = Ps2GprU32(ctx, 31);
    v0 = 0x1190000u;
    {
        const uint32_t target = ra;
        v0 = v0 - 0x3F54u;
        Ps2SetGpr64(ctx, 2, sx64(v0));
        ctx->pc = target;
        return;
    }
}

// getSolarFlareColorB (0x10a1768): lift_d1's text (the reference)
void getSolarFlareColorB(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t v0 = Ps2GprU32(ctx, 2), ra = Ps2GprU32(ctx, 31);
    v0 = 0x1190000u;
    {
        const uint32_t target = ra;
        v0 = v0 - 0x3F50u;
        Ps2SetGpr64(ctx, 2, sx64(v0));
        ctx->pc = target;
        return;
    }
}

// getSolarFlareSolarLevel (0x10a1778): lift_d1's text (the reference)
void getSolarFlareSolarLevel(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t v0 = Ps2GprU32(ctx, 2), ra = Ps2GprU32(ctx, 31);
    v0 = 0x1190000u;
    {
        const uint32_t target = ra;
        v0 = v0 - 0x3F60u;
        Ps2SetGpr64(ctx, 2, sx64(v0));
        ctx->pc = target;
        return;
    }
}

// getSolarGlowLevel (0x10a1788): lift_d1's text (the reference)
void getSolarGlowLevel(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t v0 = Ps2GprU32(ctx, 2), ra = Ps2GprU32(ctx, 31);
    v0 = 0x1190000u;
    {
        const uint32_t target = ra;
        v0 = v0 - 0x3F5Cu;
        Ps2SetGpr64(ctx, 2, sx64(v0));
        ctx->pc = target;
        return;
    }
}

// getSolarFlareSamplePix (0x10a1798): lift_d1's text (the reference)
void getSolarFlareSamplePix(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t v0 = Ps2GprU32(ctx, 2), ra = Ps2GprU32(ctx, 31);
    v0 = 0x1190000u;
    {
        const uint32_t target = ra;
        v0 = v0 - 0x3F68u;
        Ps2SetGpr64(ctx, 2, sx64(v0));
        ctx->pc = target;
        return;
    }
}

// manager_sub_010A2040 (0x10a2040): lift_d1's text (the reference)
void manager_sub_010A2040(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t at = Ps2GprU32(ctx, 1), v0 = Ps2GprU32(ctx, 2), a0 = Ps2GprU32(ctx, 4), ra = Ps2GprU32(ctx, 31);
    float f0 = ctx->f[0], f1 = ctx->f[1], f2 = ctx->f[2], f3 = ctx->f[3], f4 = ctx->f[4], f5 = ctx->f[5], f6 = ctx->f[6], f7 = ctx->f[7], f8 = ctx->f[8], f9 = ctx->f[9], f10 = ctx->f[10], f11 = ctx->f[11], f12 = ctx->f[12], f13 = ctx->f[13];
    uint32_t fcr31 = ctx->fcr31;
    bool df_at = false;
    f0 = lift_d1::flt(0u);
    f13 = lift_d1::flt(READ32(a0));
    fcr31 = (FPU_C_OLT_S(f13, f0)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
    {
        const bool c = (fcr31 & 0x800000u) != 0u;
        v0 = 1u;
        if (!c) {
            f12 = lift_d1::flt(READ32_B(a0, 4));
            fcr31 = (FPU_C_OLT_S(f12, f0)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
            if ((fcr31 & 0x800000u) == 0u) {
                f11 = lift_d1::flt(READ32_B(a0, 8));
                fcr31 = (FPU_C_OLT_S(f11, f0)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
                if ((fcr31 & 0x800000u) == 0u) {
                    f3 = lift_d1::flt(READ32_B(a0, 12));
                    fcr31 = (FPU_C_OLT_S(f3, f0)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
                    if ((fcr31 & 0x800000u) == 0u) {
                        if ((fcr31 & 0x800000u) == 0u) {
                            f10 = lift_d1::flt(READ32_B(a0, 16));
                            fcr31 = (FPU_C_OLT_S(f10, f0)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
                            if ((fcr31 & 0x800000u) == 0u) {
                                f9 = lift_d1::flt(READ32_B(a0, 20));
                                fcr31 = (FPU_C_OLT_S(f9, f0)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
                                if ((fcr31 & 0x800000u) == 0u) {
                                    f2 = lift_d1::flt(READ32_B(a0, 24));
                                    fcr31 = (FPU_C_OLT_S(f2, f0)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
                                    if ((fcr31 & 0x800000u) == 0u) {
                                        if ((fcr31 & 0x800000u) == 0u) {
                                            f8 = lift_d1::flt(READ32_B(a0, 28));
                                            fcr31 = (FPU_C_OLT_S(f8, f0)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
                                            if ((fcr31 & 0x800000u) == 0u) {
                                                f7 = lift_d1::flt(READ32_B(a0, 32));
                                                fcr31 = (FPU_C_OLT_S(f7, f0)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
                                                if ((fcr31 & 0x800000u) == 0u) {
                                                    f1 = lift_d1::flt(READ32_B(a0, 36));
                                                    fcr31 = (FPU_C_OLT_S(f1, f0)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
                                                    if ((fcr31 & 0x800000u) == 0u) {
                                                        if ((fcr31 & 0x800000u) == 0u) {
                                                            f6 = lift_d1::flt(READ32_B(a0, 40));
                                                            fcr31 = (FPU_C_OLT_S(f6, f0)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
                                                            if ((fcr31 & 0x800000u) == 0u) {
                                                                f5 = lift_d1::flt(READ32_B(a0, 44));
                                                                fcr31 = (FPU_C_OLT_S(f5, f0)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
                                                                if ((fcr31 & 0x800000u) == 0u) {
                                                                    f4 = lift_d1::flt(READ32_B(a0, 48));
                                                                    fcr31 = (FPU_C_OLT_S(f4, f0)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
                                                                    if ((fcr31 & 0x800000u) == 0u) {
                                                                        at = 0x45800000u; df_at = true;
                                                                        f0 = lift_d1::flt(at);
                                                                        fcr31 = (FPU_C_OLE_S(f0, f13)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
                                                                        if ((fcr31 & 0x800000u) == 0u) {
                                                                            fcr31 = (FPU_C_OLE_S(f0, f12)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
                                                                            if ((fcr31 & 0x800000u) == 0u) {
                                                                                fcr31 = (FPU_C_OLE_S(f0, f11)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
                                                                                if ((fcr31 & 0x800000u) == 0u) {
                                                                                    fcr31 = (FPU_C_OLE_S(f0, f3)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
                                                                                    if ((fcr31 & 0x800000u) == 0u) {
                                                                                        if ((fcr31 & 0x800000u) == 0u) {
                                                                                            fcr31 = (FPU_C_OLE_S(f0, f10)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
                                                                                            if ((fcr31 & 0x800000u) == 0u) {
                                                                                                fcr31 = (FPU_C_OLE_S(f0, f9)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
                                                                                                if ((fcr31 & 0x800000u) == 0u) {
                                                                                                    fcr31 = (FPU_C_OLE_S(f0, f2)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
                                                                                                    if ((fcr31 & 0x800000u) == 0u) {
                                                                                                        if ((fcr31 & 0x800000u) == 0u) {
                                                                                                            fcr31 = (FPU_C_OLE_S(f0, f8)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
                                                                                                            if ((fcr31 & 0x800000u) == 0u) {
                                                                                                                fcr31 = (FPU_C_OLE_S(f0, f7)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
                                                                                                                if ((fcr31 & 0x800000u) == 0u) {
                                                                                                                    fcr31 = (FPU_C_OLE_S(f0, f1)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
                                                                                                                    if ((fcr31 & 0x800000u) == 0u) {
                                                                                                                        if ((fcr31 & 0x800000u) == 0u) {
                                                                                                                            fcr31 = (FPU_C_OLE_S(f0, f6)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
                                                                                                                            if ((fcr31 & 0x800000u) == 0u) {
                                                                                                                                fcr31 = (FPU_C_OLE_S(f0, f5)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
                                                                                                                                if ((fcr31 & 0x800000u) == 0u) {
                                                                                                                                    fcr31 = (FPU_C_OLE_S(f0, f4)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
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
        const uint32_t target = ra;
        ctx->f[0] = f0; ctx->f[1] = f1; ctx->f[10] = f10; ctx->f[11] = f11; ctx->f[12] = f12; ctx->f[13] = f13; ctx->f[2] = f2; ctx->f[3] = f3; ctx->f[4] = f4; ctx->f[5] = f5; ctx->f[6] = f6; ctx->f[7] = f7; ctx->f[8] = f8; ctx->f[9] = f9; if (df_at) Ps2SetGpr64(ctx, 1, sx64(at)); Ps2SetGpr64(ctx, 2, sx64(v0)); ctx->fcr31 = fcr31;
        ctx->pc = target;
        return;
    }
}

// spriteSetPosition (0x10a52d0): lift_d1's text (the reference)
void spriteSetPosition(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t a0 = Ps2GprU32(ctx, 4), ra = Ps2GprU32(ctx, 31);
    float f12 = ctx->f[12], f13 = ctx->f[13];
    WRITE32_B(a0, 52, lift_d1::bits(f13));
    {
        const uint32_t target = ra;
        WRITE32_B(a0, 48, lift_d1::bits(f12));
        
        ctx->pc = target;
        return;
    }
}

// spriteSetTextureCoordinateSTFromUV (0x10a5310): lift_d1's text (the reference)
void spriteSetTextureCoordinateSTFromUV(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint64_t at = Ps2GprU64(ctx, 1), v0 = Ps2GprU64(ctx, 2), v1 = Ps2GprU64(ctx, 3), a0 = Ps2GprU64(ctx, 4), a1 = Ps2GprU64(ctx, 5), a2 = Ps2GprU64(ctx, 6), a3 = Ps2GprU64(ctx, 7), ra = Ps2GprU64(ctx, 31);
    float f0 = ctx->f[0], f1 = ctx->f[1], f2 = ctx->f[2], f3 = ctx->f[3], f12 = ctx->f[12], f13 = ctx->f[13], f14 = ctx->f[14], f15 = ctx->f[15];
    uint32_t fcr31 = ctx->fcr31;
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)READ32_B((uint32_t)a0, 0))));
    (a2 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 1))));
    (at = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)16256 << 16))));
    { uint32_t bits = (uint32_t)at; std::memcpy(&f1, &bits, sizeof(bits)); }
    (a3 = (uint64_t)(0ULL | (uint64_t)(uint16_t)32768));
    (a3 = (uint64_t)(RC_U64X(a3) << (32 + 1)));
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)READ32_B((uint32_t)v0, 32))));
    (at = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)16128 << 16))));
    { uint32_t bits = (uint32_t)at; std::memcpy(&f3, &bits, sizeof(bits)); }
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)READ32_B((uint32_t)v1, 28))));
    f13 = FPU_ADD_S(f13, f3);
    (a1 = (uint64_t)(READ64_B((uint32_t)a0, 72)));
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)SRL32((uint32_t)v0, 20))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)SRL32((uint32_t)v0, 16))));
    (v0 = (uint64_t)(RC_U64X(v0) & (uint64_t)(uint16_t)15));
    (v1 = (uint64_t)(RC_U64X(v1) & (uint64_t)(uint16_t)15));
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)SLL32((uint32_t)a2, (uint32_t)v1 & 0x1F))));
    (a2 = (uint64_t)((int64_t)(int32_t)((int32_t)SLL32((uint32_t)a2, (uint32_t)v0 & 0x1F))));
    { uint32_t bits = (uint32_t)a2; std::memcpy(&f0, &bits, sizeof(bits)); }
    { int32_t tmp; std::memcpy(&tmp, &f0, sizeof(tmp)); f0 = FPU_CVT_S_W(tmp); }
    (a1 = (uint64_t)(a1 | a3));
    { uint32_t bits = (uint32_t)v1; std::memcpy(&f2, &bits, sizeof(bits)); }
    { int32_t tmp; std::memcpy(&tmp, &f2, sizeof(tmp)); f2 = FPU_CVT_S_W(tmp); }
    WRITE64_B((uint32_t)a0, 72, a1);
    f12 = FPU_ADD_S(f12, f3);
    f15 = FPU_ADD_S(f13, f15);
    f0 = ps2FpuDivide(fcr31, f1, f0);
    f14 = FPU_ADD_S(f12, f14);
    f1 = ps2FpuDivide(fcr31, f1, f2);
    f14 = FPU_MUL_SF(f14, f0);
    f12 = FPU_MUL_SF(f12, f0);
    { float f = f14; uint32_t bits; std::memcpy(&bits, &f, sizeof(bits)); WRITE32_B((uint32_t)a0, 40, bits); }
    { float f = f12; uint32_t bits; std::memcpy(&bits, &f, sizeof(bits)); WRITE32_B((uint32_t)a0, 32, bits); }
    f15 = FPU_MUL_SF(f15, f1);
    f13 = FPU_MUL_SF(f13, f1);
    { float f = f15; uint32_t bits; std::memcpy(&bits, &f, sizeof(bits)); WRITE32_B((uint32_t)a0, 44, bits); }
    {
        const uint32_t target = (uint32_t)ra;
        { float f = f13; uint32_t bits; std::memcpy(&bits, &f, sizeof(bits)); WRITE32_B((uint32_t)a0, 36, bits); }
        ctx->f[0] = f0; ctx->f[1] = f1; ctx->f[12] = f12; ctx->f[13] = f13; ctx->f[14] = f14; ctx->f[15] = f15; ctx->f[2] = f2; ctx->f[3] = f3; Ps2SetGpr64(ctx, 1, at); Ps2SetGpr64(ctx, 2, v0); Ps2SetGpr64(ctx, 3, v1); Ps2SetGpr64(ctx, 5, a1); Ps2SetGpr64(ctx, 6, a2); Ps2SetGpr64(ctx, 7, a3); ctx->fcr31 = fcr31;
        ctx->pc = target;
        return;
    }
}

// spriteSetTextureCoordinateUV (0x10a53b8): lift_d1's text (the reference)
void spriteSetTextureCoordinateUV(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint64_t v0 = Ps2GprU64(ctx, 2), v1 = Ps2GprU64(ctx, 3), a0 = Ps2GprU64(ctx, 4), ra = Ps2GprU64(ctx, 31);
    float f0 = ctx->f[0], f1 = ctx->f[1], f2 = ctx->f[2], f12 = ctx->f[12], f13 = ctx->f[13], f14 = ctx->f[14], f15 = ctx->f[15];
    f0 = FPU_ADD_S(f12, f14);
    (v0 = (uint64_t)(READ64_B((uint32_t)a0, 72)));
    f1 = FPU_ADD_S(f13, f15);
    { float f = f15; uint32_t bits; std::memcpy(&bits, &f, sizeof(bits)); WRITE32_B((uint32_t)a0, 60, bits); }
    { int32_t tmp = FPU_CVT_W_S(f12); std::memcpy(&f2, &tmp, sizeof(tmp)); }
    { float f = f2; uint32_t bits; std::memcpy(&bits, &f, sizeof(bits)); WRITE32_B((uint32_t)a0, 16, bits); }
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)65534 << 16))));
    (v1 = (uint64_t)(RC_U64X(v1) | (uint64_t)(uint16_t)65535));
    (v1 = (uint64_t)(RC_U64X(v1) << 16));
    (v1 = (uint64_t)(v1 | (uint64_t)(uint16_t)65535));
    (v1 = (uint64_t)(v1 << 16));
    (v1 = (uint64_t)(v1 | (uint64_t)(uint16_t)65535));
    (v0 = (uint64_t)(v0 & v1));
    { int32_t tmp = FPU_CVT_W_S(f13); std::memcpy(&f2, &tmp, sizeof(tmp)); }
    { float f = f2; uint32_t bits; std::memcpy(&bits, &f, sizeof(bits)); WRITE32_B((uint32_t)a0, 20, bits); }
    { int32_t tmp = FPU_CVT_W_S(f0); std::memcpy(&f2, &tmp, sizeof(tmp)); }
    { float f = f2; uint32_t bits; std::memcpy(&bits, &f, sizeof(bits)); WRITE32_B((uint32_t)a0, 24, bits); }
    WRITE64_B((uint32_t)a0, 72, v0);
    { int32_t tmp = FPU_CVT_W_S(f1); std::memcpy(&f0, &tmp, sizeof(tmp)); }
    { float f = f0; uint32_t bits; std::memcpy(&bits, &f, sizeof(bits)); WRITE32_B((uint32_t)a0, 28, bits); }
    {
        const uint32_t target = (uint32_t)ra;
        { float f = f14; uint32_t bits; std::memcpy(&bits, &f, sizeof(bits)); WRITE32_B((uint32_t)a0, 56, bits); }
        ctx->f[0] = f0; ctx->f[1] = f1; ctx->f[2] = f2; Ps2SetGpr64(ctx, 2, v0); Ps2SetGpr64(ctx, 3, v1);
        ctx->pc = target;
        return;
    }
}

// spriteSetColor (0x10a5410): lift_d1's text (the reference)
void spriteSetColor(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t a0 = Ps2GprU32(ctx, 4), a1 = Ps2GprU32(ctx, 5), a2 = Ps2GprU32(ctx, 6), a3 = Ps2GprU32(ctx, 7), t0 = Ps2GprU32(ctx, 8), ra = Ps2GprU32(ctx, 31);
    WRITE8_B(a0, 7, (uint8_t)t0);
    WRITE8_B(a0, 4, (uint8_t)a1);
    WRITE8_B(a0, 5, (uint8_t)a2);
    {
        const uint32_t target = ra;
        WRITE8_B(a0, 6, (uint8_t)a3);
        
        ctx->pc = target;
        return;
    }
}

// spriteSetScale (0x10a5428): lift_d1's text (the reference)
void spriteSetScale(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t a0 = Ps2GprU32(ctx, 4), ra = Ps2GprU32(ctx, 31);
    float f12 = ctx->f[12], f13 = ctx->f[13];
    WRITE32_B(a0, 68, lift_d1::bits(f13));
    {
        const uint32_t target = ra;
        WRITE32_B(a0, 64, lift_d1::bits(f12));
        
        ctx->pc = target;
        return;
    }
}

// spriteSetRotation (0x10a5438): lift_d1's text (the reference)
void spriteSetRotation(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t a0 = Ps2GprU32(ctx, 4), ra = Ps2GprU32(ctx, 31);
    float f12 = ctx->f[12];
    {
        const uint32_t target = ra;
        WRITE32_B(a0, 72, lift_d1::bits(f12));
        
        ctx->pc = target;
        return;
    }
}

// spriteSetFade (0x10a54d0): lift_d1's text (the reference)
void spriteSetFade(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t a0 = Ps2GprU32(ctx, 4), ra = Ps2GprU32(ctx, 31);
    float f12 = ctx->f[12];
    {
        const uint32_t target = ra;
        WRITE32_B(a0, 8, lift_d1::bits(f12));
        
        ctx->pc = target;
        return;
    }
}

// spriteSetPriority (0x10a54d8): lift_d1's text (the reference)
void spriteSetPriority(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t a0 = Ps2GprU32(ctx, 4), a1 = Ps2GprU32(ctx, 5), ra = Ps2GprU32(ctx, 31);
    {
        const uint32_t target = ra;
        WRITE8_B(a0, 76, (uint8_t)a1);
        
        ctx->pc = target;
        return;
    }
}

// manager_sub_010ABC48 (0x10abc48): lift_d1's text (the reference)
void manager_sub_010ABC48(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t v1 = Ps2GprU32(ctx, 3), a1 = Ps2GprU32(ctx, 5), a2 = Ps2GprU32(ctx, 6), a3 = Ps2GprU32(ctx, 7), t0 = Ps2GprU32(ctx, 8), t1 = Ps2GprU32(ctx, 9), t2 = Ps2GprU32(ctx, 10), t4 = Ps2GprU32(ctx, 12), ra = Ps2GprU32(ctx, 31);
    uint64_t v0 = Ps2GprU64(ctx, 2), a0 = Ps2GprU64(ctx, 4), t3 = Ps2GprU64(ctx, 11);
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)281 << 16))));
    (t3 = (uint64_t)((uint64_t)a0 + (uint64_t)0ULL));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v0, 4294952024))));
    (v1 = (uint32_t)((int32_t)((int32_t)READ32_B((uint32_t)t3, 68))));
    (t0 = (uint32_t)((int32_t)((int32_t)READ32_B((uint32_t)v0, 12))));
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v0, 12))));
    v1 = v1 & 0x7FFFu;
    (a1 = (uint32_t)((int32_t)((int32_t)READ32_B((uint32_t)t3, 72))));
    (v0 = (uint64_t)((uint64_t)sx64(t0) + (uint64_t)0ULL));
    t1 = t0 + 8u;
    t4 = v1 << 16;
    {
        const bool c = (int64_t)v0 != (int64_t)(int32_t)t0;
        (t2 = (uint32_t)((int32_t)((int32_t)ADD32((uint32_t)t3, 84))));
        if (!c) {
            (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)READ32_B(t0, 4))));
            WRITE32_B((uint32_t)a0, 0, (uint32_t)v0);
        }
    }
    v1 = READ32(t0);
    if (v1 == 0) {
        v1 = READ32_B(t0, 4);
    } else {
        (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)READ32_B(t0, 4))));
        WRITE32_B(v1, 4, (uint32_t)v0);
        v1 = READ32_B(t0, 4);
    }
    if (v1 == 0) {
        (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)READ32_B((uint32_t)a0, 4))));
    } else {
        (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)READ32_B(t0, 0))));
        WRITE32_B(v1, 0, (uint32_t)v0);
        (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)READ32_B((uint32_t)a0, 4))));
    }
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v0, 4294967295))));
    WRITE32_B((uint32_t)a0, 4, (uint32_t)v0);
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)READ32_B(t2, 0))));
    if ((int64_t)v0 != 0) {
        WRITE32_B((uint32_t)v0, 0, t0);
    } else {
    }
    WRITE32(t0, 0u);
    a3 = 0xFFFFFFFEu;
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)32768 << 16))));
    a2 = 0xFFFF0000u;
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)READ32_B(t2, 0))));
    (a0 = (uint64_t)(RC_U64X(a0) | (uint64_t)(uint16_t)65535));
    a1 = a1 & 0x7FFFu;
    a2 = a2 | 1u;
    WRITE32_B(t0, 4, (uint32_t)v0);
    a1 = a1 << 1;
    WRITE32(t2, t0);
    v1 = 1u;
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)READ32_B(t2, 4))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v0, 1))));
    WRITE32_B(t2, 4, (uint32_t)v0);
    WRITE32(t1, v1);
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)READ32_B(t1, 4))));
    WRITE32_B(t1, 20, 0u);
    (v0 = (uint64_t)(RC_U64X(v0) & sx64(a3)));
    WRITE32_B(t1, 12, t0);
    (v0 = (uint64_t)(RC_U64X(v0) & RC_U64X(a0)));
    WRITE32_B(t1, 8, (uint32_t)t3);
    (v0 = (uint64_t)(RC_U64X(v0) | sx64(t4)));
    WRITE32_B(t1, 16, 0u);
    (v0 = (uint64_t)(RC_U64X(v0) & sx64(a2)));
    (v0 = (uint64_t)(RC_U64X(v0) | sx64(a1)));
    WRITE32_B(t1, 4, (uint32_t)v0);
    {
        const uint32_t target = ra;
        WRITE32_B((uint32_t)t3, 80, t0);
        Ps2SetGpr64(ctx, 10, sx64(t2)); Ps2SetGpr64(ctx, 11, t3); Ps2SetGpr64(ctx, 12, sx64(t4)); Ps2SetGpr64(ctx, 2, v0); Ps2SetGpr64(ctx, 3, sx64(v1)); Ps2SetGpr64(ctx, 4, a0); Ps2SetGpr64(ctx, 5, sx64(a1)); Ps2SetGpr64(ctx, 6, sx64(a2)); Ps2SetGpr64(ctx, 7, sx64(a3)); Ps2SetGpr64(ctx, 8, sx64(t0)); Ps2SetGpr64(ctx, 9, sx64(t1));
        ctx->pc = target;
        return;
    }
}

// getZFogDivide (0x10ac400): lift_d1's text (the reference)
void getZFogDivide(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t v0 = Ps2GprU32(ctx, 2), ra = Ps2GprU32(ctx, 31);
    v0 = 0x1190000u;
    {
        const uint32_t target = ra;
        v0 = v0 - 0x3B94u;
        Ps2SetGpr64(ctx, 2, sx64(v0));
        ctx->pc = target;
        return;
    }
}

// getZFogScissorMargin (0x10ac440): lift_d1's text (the reference)
void getZFogScissorMargin(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t v0 = Ps2GprU32(ctx, 2), ra = Ps2GprU32(ctx, 31);
    v0 = 0x1190000u;
    {
        const uint32_t target = ra;
        v0 = v0 - 0x3B88u;
        Ps2SetGpr64(ctx, 2, sx64(v0));
        ctx->pc = target;
        return;
    }
}

// setZFogFar (0x10ac460): lift_d1's text (the reference)
void setZFogFar(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t v0 = Ps2GprU32(ctx, 2), ra = Ps2GprU32(ctx, 31);
    float f12 = ctx->f[12];
    v0 = 0x1190000u;
    {
        const uint32_t target = ra;
        WRITE32(v0 - 0x3B80u, lift_d1::bits(f12));
        Ps2SetGpr64(ctx, 2, sx64(v0));
        ctx->pc = target;
        return;
    }
}

// getZFogNear (0x10ac478): lift_d1's text (the reference)
void getZFogNear(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t v0 = Ps2GprU32(ctx, 2), v1 = Ps2GprU32(ctx, 3), a0 = Ps2GprU32(ctx, 4), ra = Ps2GprU32(ctx, 31);
    float f0 = ctx->f[0], f1 = ctx->f[1], f2 = ctx->f[2];
    v0 = 0x1190000u;
    a0 = 0x1190000u;
    f0 = lift_d1::flt(READ32(v0 - 0x3B80u));
    v1 = 0x1190000u;
    f1 = lift_d1::flt(READ32(a0 - 0x3B8Cu));
    f2 = lift_d1::flt(READ32(v1 - 0x3B90u));
    f0 = FPU_SUB_S(f0, f1);
    {
        const uint32_t target = ra;
        f0 = FPU_SUB_S(f0, f2);
        ctx->f[0] = f0; ctx->f[1] = f1; ctx->f[2] = f2; Ps2SetGpr64(ctx, 2, sx64(v0)); Ps2SetGpr64(ctx, 3, sx64(v1)); Ps2SetGpr64(ctx, 4, sx64(a0));
        ctx->pc = target;
        return;
    }
}

// GetObjectAnimationParentNodeId (0x10b6788): lift_d1's text (the reference)
void GetObjectAnimationParentNodeId(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t v0 = Ps2GprU32(ctx, 2), v1 = Ps2GprU32(ctx, 3), a0 = Ps2GprU32(ctx, 4), a1 = Ps2GprU32(ctx, 5), ra = Ps2GprU32(ctx, 31);
    v1 = READ32_B(a0, 8);
    a1 = a1 << 2;
    v0 = 0xFFFFFFFFu;
    {
        const bool c = v1 == 0;
        a1 = a1 + v1;
        if (!c) {
            v0 = READ32(a1);
        }
    }
    {
        const uint32_t target = ra;
        Ps2SetGpr64(ctx, 2, sx64(v0)); Ps2SetGpr64(ctx, 3, sx64(v1)); Ps2SetGpr64(ctx, 5, sx64(a1));
        ctx->pc = target;
        return;
    }
}

// IsFullFixFormat (0x10b74c0): lift_d1's text (the reference)
void IsFullFixFormat(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t v1 = Ps2GprU32(ctx, 3), ra = Ps2GprU32(ctx, 31);
    uint64_t v0 = Ps2GprU64(ctx, 2), a0 = Ps2GprU64(ctx, 4);
    v1 = 0xFF0000u;
    v1 = v1 | 0xFFFFu;
    (v0 = (uint64_t)(a0 & sx64(v1)));
    {
        const uint32_t target = ra;
        (v0 = (uint64_t)(((uint64_t)v0 < (uint64_t)(int64_t)(int32_t)1) ? 1 : 0));
        Ps2SetGpr64(ctx, 2, v0); Ps2SetGpr64(ctx, 3, sx64(v1));
        ctx->pc = target;
        return;
    }
}

// GetMotionResourceRotAngle (0x10b7650): lift_d1's text (the reference)
void GetMotionResourceRotAngle(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t ra = Ps2GprU32(ctx, 31);
    uint64_t v0 = Ps2GprU64(ctx, 2), v1 = Ps2GprU64(ctx, 3), a0 = Ps2GprU64(ctx, 4), a1 = Ps2GprU64(ctx, 5);
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
        const uint32_t target = ra;
        (v0 = (uint64_t)((uint64_t)RC_U64X(a1) + (uint64_t)0ULL));
        Ps2SetGpr64(ctx, 2, v0); Ps2SetGpr64(ctx, 3, v1); Ps2SetGpr64(ctx, 5, a1);
        ctx->pc = target;
        return;
    }
}

// constructor_motion_ik_base (0x10b92a8): lift_d1's text (the reference)
void constructor_motion_ik_base(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t a0 = Ps2GprU32(ctx, 4), ra = Ps2GprU32(ctx, 31);
    {
        const uint32_t target = ra;
        WRITE32(a0, 0u);
        
        ctx->pc = target;
        return;
    }
}

// init_motion_ik_base (0x10b92b0): lift_d1's text (the reference)
void init_motion_ik_base(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t a0 = Ps2GprU32(ctx, 4), ra = Ps2GprU32(ctx, 31);
    {
        const uint32_t target = ra;
        WRITE32(a0, 0u);
        
        ctx->pc = target;
        return;
    }
}

// doit_motion_ik_base (0x10b92b8): lift_d1's text (the reference)
void doit_motion_ik_base(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t a0 = Ps2GprU32(ctx, 4), a1 = Ps2GprU32(ctx, 5), ra = Ps2GprU32(ctx, 31);
    {
        const uint32_t target = ra;
        WRITE32(a0, a1);
        
        ctx->pc = target;
        return;
    }
}

// manager_sub_010BA140 (0x10ba140): lift_d1's text (the reference)
void manager_sub_010BA140(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t a0 = Ps2GprU32(ctx, 4), ra = Ps2GprU32(ctx, 31);
    WRITE32_B(a0, 128, 0u);
    WRITE32_B(a0, 136, 0u);
    {
        const uint32_t target = ra;
        WRITE32_B(a0, 132, 0u);
        
        ctx->pc = target;
        return;
    }
}

// UpdateAnimationClassPastSimpleInfo (0x10ba9d8): lift_d1's text (the reference)
void UpdateAnimationClassPastSimpleInfo(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t a0 = Ps2GprU32(ctx, 4), ra = Ps2GprU32(ctx, 31);
    float f0 = ctx->f[0];
    f0 = lift_d1::flt(READ32_B(a0, 8));
    {
        const uint32_t target = ra;
        WRITE32_B(a0, 12, lift_d1::bits(f0));
        ctx->f[0] = f0;
        ctx->pc = target;
        return;
    }
}

// GetCharacterClassInitParamNbOptionNodes (0x10c0c78): lift_d1's text (the reference)
void GetCharacterClassInitParamNbOptionNodes(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t v0 = Ps2GprU32(ctx, 2), v1 = Ps2GprU32(ctx, 3), a0 = Ps2GprU32(ctx, 4), ra = Ps2GprU32(ctx, 31);
    v1 = READ32_B(a0, 132);
    {
        const bool c = v1 == 0;
        v0 = 0xFFFFFFFFu;
        if (!c) {
            v0 = READ32_B(v1, 32);
            v0 = READ32_B(v0, 32);
        }
    }
    {
        const uint32_t target = ra;
        Ps2SetGpr64(ctx, 2, sx64(v0)); Ps2SetGpr64(ctx, 3, sx64(v1));
        ctx->pc = target;
        return;
    }
}

// ResetCharacterClassWaterStatus (0x10c11b8): lift_d1's text (the reference)
void ResetCharacterClassWaterStatus(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t a0 = Ps2GprU32(ctx, 4), ra = Ps2GprU32(ctx, 31);
    WRITE32_B(a0, 4300, 0u);
    WRITE8_B(a0, 865, (uint8_t)0u);
    WRITE8_B(a0, 866, (uint8_t)0u);
    WRITE32_B(a0, 4296, 0u);
    {
        const uint32_t target = ra;
        WRITE32_B(a0, 4292, 0u);
        
        ctx->pc = target;
        return;
    }
}

// ResetCharacterClassMotInterpInfo (0x10c17b0): lift_d1's text (the reference)
void ResetCharacterClassMotInterpInfo(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t at = Ps2GprU32(ctx, 1), v0 = Ps2GprU32(ctx, 2), a0 = Ps2GprU32(ctx, 4), ra = Ps2GprU32(ctx, 31);
    float f0 = ctx->f[0], f1 = ctx->f[1];
    at = 0x3F800000u;
    f1 = lift_d1::flt(at);
    WRITE32_B(a0, 1824, 0u);
    WRITE32_B(a0, 1828, lift_d1::bits(f1));
    WRITE32_B(a0, 1832, 0u);
    WRITE32_B(a0, 1836, 0u);
    WRITE32_B(a0, 1852, lift_d1::bits(f1));
    v0 = READ32_B(a0, 4416);
    f0 = lift_d1::flt(READ32_B(v0, 312));
    WRITE32_B(a0, 1860, lift_d1::bits(f1));
    {
        const uint32_t target = ra;
        WRITE32_B(a0, 1856, lift_d1::bits(f0));
        ctx->f[0] = f0; ctx->f[1] = f1; Ps2SetGpr64(ctx, 1, sx64(at)); Ps2SetGpr64(ctx, 2, sx64(v0));
        ctx->pc = target;
        return;
    }
}

// GetCharacterClassCurAnimationFrameRate (0x10c1a08): lift_d1's text (the reference)
void GetCharacterClassCurAnimationFrameRate(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t v1 = Ps2GprU32(ctx, 3), a0 = Ps2GprU32(ctx, 4), ra = Ps2GprU32(ctx, 31);
    uint64_t v0 = Ps2GprU64(ctx, 2);
    v1 = READ32_B(a0, 448);
    {
        const bool c = v1 == 0;
        (v0 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL));
        if (!c) {
            (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)READ32_B(v1, 92))));
        }
    }
    {
        const uint32_t target = ra;
        Ps2SetGpr64(ctx, 2, v0); Ps2SetGpr64(ctx, 3, sx64(v1));
        ctx->pc = target;
        return;
    }
}

// SetCharacterClassGazeAdditionChannelInfo (0x10c66f0): lift_d1's text (the reference)
void SetCharacterClassGazeAdditionChannelInfo(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t v0 = Ps2GprU32(ctx, 2), v1 = Ps2GprU32(ctx, 3), a0 = Ps2GprU32(ctx, 4), a1 = Ps2GprU32(ctx, 5), ra = Ps2GprU32(ctx, 31);
    float f12 = ctx->f[12], f13 = ctx->f[13];
    v0 = READ32(a1);
    v1 = READ32_B(a0, 820);
    v0 = v0 >> 14;
    v0 = v0 & 0xFFF0u;
    v0 = v0 + v1;
    WRITE32(v0, lift_d1::bits(f12));
    {
        const uint32_t target = ra;
        WRITE32_B(v0, 4, lift_d1::bits(f13));
        Ps2SetGpr64(ctx, 2, sx64(v0)); Ps2SetGpr64(ctx, 3, sx64(v1));
        ctx->pc = target;
        return;
    }
}

// ForceAcceptMotionControlChange (0x10cc6c8): lift_d1's text (the reference)
void ForceAcceptMotionControlChange(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t v0 = Ps2GprU32(ctx, 2), a0 = Ps2GprU32(ctx, 4), ra = Ps2GprU32(ctx, 31);
    uint64_t a1 = Ps2GprU64(ctx, 5);
    v0 = READ32_B(a0, 1872);
    if ((int64_t)(int32_t)v0 != (int64_t)a1) {
        WRITE32_B(a0, 1872, (uint32_t)a1);
        WRITE32_B(a0, 1876, v0);
    }
    {
        const uint32_t target = ra;
        Ps2SetGpr64(ctx, 2, sx64(v0));
        ctx->pc = target;
        return;
    }
}

// YattsukeSetPlayerR1ON (0x10cc778): lift_d1's text (the reference)
void YattsukeSetPlayerR1ON(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t v0 = Ps2GprU32(ctx, 2), a0 = Ps2GprU32(ctx, 4), ra = Ps2GprU32(ctx, 31);
    v0 = 0x1190000u;
    {
        const uint32_t target = ra;
        WRITE32(v0 - 0x3610u, a0);
        Ps2SetGpr64(ctx, 2, sx64(v0));
        ctx->pc = target;
        return;
    }
}

// RequestCharacterClassSlip (0x10d2a88): lift_d1's text (the reference)
void RequestCharacterClassSlip(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t v0 = Ps2GprU32(ctx, 2), a0 = Ps2GprU32(ctx, 4), ra = Ps2GprU32(ctx, 31);
    v0 = (uint32_t)(int8_t)READ8_B(a0, 3222);
    {
        const bool c = v0 != 0;
        v0 = 1u;
        if (!c) {
            WRITE8_B(a0, 3222, (uint8_t)v0);
        }
    }
    {
        const uint32_t target = ra;
        Ps2SetGpr64(ctx, 2, sx64(v0));
        ctx->pc = target;
        return;
    }
}

// RequestCharacterClassNoClimbableSlip (0x10d2aa0): lift_d1's text (the reference)
void RequestCharacterClassNoClimbableSlip(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t v0 = Ps2GprU32(ctx, 2), a0 = Ps2GprU32(ctx, 4), ra = Ps2GprU32(ctx, 31);
    v0 = 2u;
    {
        const uint32_t target = ra;
        WRITE8_B(a0, 3222, (uint8_t)v0);
        Ps2SetGpr64(ctx, 2, sx64(v0));
        ctx->pc = target;
        return;
    }
}

// RequestCharacterClassAdditionMotionChange (0x10d2ae0): lift_d1's text (the reference)
void RequestCharacterClassAdditionMotionChange(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t v0 = Ps2GprU32(ctx, 2), v1 = Ps2GprU32(ctx, 3), a0 = Ps2GprU32(ctx, 4), a1 = Ps2GprU32(ctx, 5), a2 = Ps2GprU32(ctx, 6), ra = Ps2GprU32(ctx, 31);
    v0 = (uint32_t)(int8_t)READ8_B(a0, 3220);
    v1 = READ32_B(a0, 1020);
    a2 = v0 << 2;
    v0 = (int32_t)v0 < (int32_t)v1 ? 1u : 0u;
    if (v0 != 0) {
        v0 = READ32_B(a0, 776);
        v0 = a2 + v0;
        WRITE32(v0, a1);
        v1 = READ8_B(a0, 3220);
        v1 = v1 + 1u;
        WRITE8_B(a0, 3220, (uint8_t)v1);
    }
    {
        const uint32_t target = ra;
        Ps2SetGpr64(ctx, 2, sx64(v0)); Ps2SetGpr64(ctx, 3, sx64(v1)); Ps2SetGpr64(ctx, 6, sx64(a2));
        ctx->pc = target;
        return;
    }
}

// RequestCharacterClassMotionDirectionByGPTop (0x10d2b60): lift_d1's text (the reference)
void RequestCharacterClassMotionDirectionByGPTop(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t v0 = Ps2GprU32(ctx, 2), a0 = Ps2GprU32(ctx, 4), ra = Ps2GprU32(ctx, 31);
    v0 = 3u;
    {
        const uint32_t target = ra;
        WRITE32_B(a0, 3156, v0);
        Ps2SetGpr64(ctx, 2, sx64(v0));
        ctx->pc = target;
        return;
    }
}

// RequestCharacterClassMotionDirectionByGPTail (0x10d2b70): lift_d1's text (the reference)
void RequestCharacterClassMotionDirectionByGPTail(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t v0 = Ps2GprU32(ctx, 2), a0 = Ps2GprU32(ctx, 4), ra = Ps2GprU32(ctx, 31);
    v0 = 4u;
    {
        const uint32_t target = ra;
        WRITE32_B(a0, 3156, v0);
        Ps2SetGpr64(ctx, 2, sx64(v0));
        ctx->pc = target;
        return;
    }
}

// RequestCharacterClassMotionTurnLevel (0x10d2b90): lift_d1's text (the reference)
void RequestCharacterClassMotionTurnLevel(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t v0 = Ps2GprU32(ctx, 2), a0 = Ps2GprU32(ctx, 4), ra = Ps2GprU32(ctx, 31);
    float f12 = ctx->f[12];
    v0 = 2u;
    WRITE32_B(a0, 3168, lift_d1::bits(f12));
    {
        const uint32_t target = ra;
        WRITE32_B(a0, 3156, v0);
        Ps2SetGpr64(ctx, 2, sx64(v0));
        ctx->pc = target;
        return;
    }
}

// RequestCharacterClassMotionPlaySpeedUp (0x10d2bb0): lift_d1's text (the reference)
void RequestCharacterClassMotionPlaySpeedUp(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t v0 = Ps2GprU32(ctx, 2), a0 = Ps2GprU32(ctx, 4), ra = Ps2GprU32(ctx, 31);
    v0 = 1u;
    {
        const uint32_t target = ra;
        WRITE32_B(a0, 3216, v0);
        Ps2SetGpr64(ctx, 2, sx64(v0));
        ctx->pc = target;
        return;
    }
}

// RequestCharacterClassMotionPlaySpeedDown (0x10d2bc0): lift_d1's text (the reference)
void RequestCharacterClassMotionPlaySpeedDown(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t v0 = Ps2GprU32(ctx, 2), a0 = Ps2GprU32(ctx, 4), ra = Ps2GprU32(ctx, 31);
    v0 = 0xFFFFFFFFu;
    {
        const uint32_t target = ra;
        WRITE32_B(a0, 3216, v0);
        Ps2SetGpr64(ctx, 2, sx64(v0));
        ctx->pc = target;
        return;
    }
}

// RequestCharacterClassMotionPlaySpeedDown2 (0x10d2bd0): lift_d1's text (the reference)
void RequestCharacterClassMotionPlaySpeedDown2(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t v0 = Ps2GprU32(ctx, 2), a0 = Ps2GprU32(ctx, 4), ra = Ps2GprU32(ctx, 31);
    v0 = 0xFFFFFFFEu;
    {
        const uint32_t target = ra;
        WRITE32_B(a0, 3216, v0);
        Ps2SetGpr64(ctx, 2, sx64(v0));
        ctx->pc = target;
        return;
    }
}

// RequestCharacterClassWindForceMove (0x10d2be0): lift_d1's text (the reference)
void RequestCharacterClassWindForceMove(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t v0 = Ps2GprU32(ctx, 2), a0 = Ps2GprU32(ctx, 4), ra = Ps2GprU32(ctx, 31);
    v0 = 1u;
    {
        const uint32_t target = ra;
        WRITE8_B(a0, 3221, (uint8_t)v0);
        Ps2SetGpr64(ctx, 2, sx64(v0));
        ctx->pc = target;
        return;
    }
}

// getTurnLimitInterp (0x10d2ce8): lift_d1's text (the reference)
void getTurnLimitInterp(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t at = Ps2GprU32(ctx, 1), ra = Ps2GprU32(ctx, 31);
    float f0 = ctx->f[0], f1 = ctx->f[1], f12 = ctx->f[12], f13 = ctx->f[13], f14 = ctx->f[14], f15 = ctx->f[15];
    uint32_t fcr31 = ctx->fcr31;
    at = 0x3A830000u;
    at = at | 0x126Eu;
    f0 = lift_d1::flt(at);
    fcr31 = (FPU_C_OLE_S(f14, f0)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
    {
        const bool c = (fcr31 & 0x800000u) != 0u;
        f0 = FPU_MOV_S(f13);
        if (!c) {
            f1 = ps2FpuDivide(fcr31, f15, f14);
            at = 0x3F800000u;
            f0 = lift_d1::flt(at);
            f14 = FPU_MOV_S(f1);
            fcr31 = (FPU_C_OLE_S(f14, f0)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
            if ((fcr31 & 0x800000u) == 0u) {
                f14 = FPU_MOV_S(f0);
            } else {
                at = 0x3F800000u;
                f0 = lift_d1::flt(at);
            }
            f1 = FPU_MOV_S(f14);
            f0 = FPU_SUB_SF(f0, f1);
            f1 = FPU_MUL_S(f13, f1);
            f0 = FPU_MUL_S(f12, f0);
            f0 = FPU_ADD_SF(f1, f0);
        }
    }
    {
        const uint32_t target = ra;
        ctx->f[0] = f0; ctx->f[1] = f1; ctx->f[14] = f14; Ps2SetGpr64(ctx, 1, sx64(at)); ctx->fcr31 = fcr31;
        ctx->pc = target;
        return;
    }
}

// getRatioFromInterp (0x10d2d48): lift_d1's text (the reference)
void getRatioFromInterp(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t at = Ps2GprU32(ctx, 1), ra = Ps2GprU32(ctx, 31);
    float f0 = ctx->f[0], f1 = ctx->f[1], f12 = ctx->f[12], f13 = ctx->f[13];
    uint32_t fcr31 = ctx->fcr31;
    at = 0x3A830000u;
    at = at | 0x126Eu;
    f1 = lift_d1::flt(at);
    at = 0x3F800000u;
    f0 = lift_d1::flt(at);
    fcr31 = (FPU_C_OLE_S(f12, f1)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
    if ((fcr31 & 0x800000u) == 0u) {
        f0 = ps2FpuDivide(fcr31, f13, f12);
        at = 0x3F800000u;
        f1 = lift_d1::flt(at);
        fcr31 = (FPU_C_OLE_S(f0, f1)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
        if ((fcr31 & 0x800000u) == 0u) {
            f0 = FPU_MOV_S(f1);
        } else {
        }
    }
    {
        const uint32_t target = ra;
        ctx->f[0] = f0; ctx->f[1] = f1; Ps2SetGpr64(ctx, 1, sx64(at)); ctx->fcr31 = fcr31;
        ctx->pc = target;
        return;
    }
}

// SetCharacterObjMotionJumpSpeedLimit (0x10d37a8): lift_d1's text (the reference)
void SetCharacterObjMotionJumpSpeedLimit(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t v0 = Ps2GprU32(ctx, 2), v1 = Ps2GprU32(ctx, 3), a0 = Ps2GprU32(ctx, 4), a1 = Ps2GprU32(ctx, 5), ra = Ps2GprU32(ctx, 31);
    v0 = 0x10D0000u;
    a0 = 0x1190000u;
    v0 = v0 + 0x8A0u;
    v1 = 0x1190000u;
    WRITE32(a0 - 0x3600u, v0);
    {
        const uint32_t target = ra;
        WRITE32(v1 - 0x35FCu, a1);
        Ps2SetGpr64(ctx, 2, sx64(v0)); Ps2SetGpr64(ctx, 3, sx64(v1)); Ps2SetGpr64(ctx, 4, sx64(a0));
        ctx->pc = target;
        return;
    }
}

// charrootdyn_getUpperEdgeEffectiveHeight (0x10dc3f0): lift_d1's text (the reference)
void charrootdyn_getUpperEdgeEffectiveHeight(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t a0 = Ps2GprU32(ctx, 4), ra = Ps2GprU32(ctx, 31);
    {
        const uint32_t target = ra;
        WRITE32_B(a0, 152, 0u);
        
        ctx->pc = target;
        return;
    }
}

// charrootdyn_getNoLimitMode (0x10de368): lift_d1's text (the reference)
void charrootdyn_getNoLimitMode(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t v0 = Ps2GprU32(ctx, 2), a0 = Ps2GprU32(ctx, 4), a1 = Ps2GprU32(ctx, 5), ra = Ps2GprU32(ctx, 31);
    WRITE32(a0, 0u);
    WRITE32_B(a0, 4, 0u);
    v0 = READ32_B(a1, 3120);
    {
        const uint32_t target = ra;
        WRITE32_B(a0, 8, v0);
        Ps2SetGpr64(ctx, 2, sx64(v0));
        ctx->pc = target;
        return;
    }
}

// charrootdyn_getForceNoLimitMode (0x10de380): lift_d1's text (the reference)
void charrootdyn_getForceNoLimitMode(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t v0 = Ps2GprU32(ctx, 2), a0 = Ps2GprU32(ctx, 4), ra = Ps2GprU32(ctx, 31);
    v0 = 3u;
    WRITE32_B(a0, 8, 0u);
    WRITE32(a0, v0);
    {
        const uint32_t target = ra;
        WRITE32_B(a0, 4, 0u);
        Ps2SetGpr64(ctx, 2, sx64(v0));
        ctx->pc = target;
        return;
    }
}

// charrootdyn_isParentToEdge (0x10df130): lift_d1's text (the reference)
void charrootdyn_isParentToEdge(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t a0 = Ps2GprU32(ctx, 4), ra = Ps2GprU32(ctx, 31);
    uint64_t v0 = Ps2GprU64(ctx, 2), v1 = Ps2GprU64(ctx, 3);
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)READ32_B(a0, 292))));
    {
        const bool c = (int64_t)v0 == 0;
        (v1 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL));
        if (!c) {
            (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)READ32_B(a0, 240))));
            (v0 = (uint64_t)(RC_U64X(v0) ^ (uint64_t)(uint16_t)4));
            (v1 = (uint64_t)(((uint64_t)RC_U64X(v0) < (uint64_t)(int64_t)(int32_t)1) ? 1 : 0));
        }
    }
    {
        const uint32_t target = ra;
        (v0 = (uint64_t)((uint64_t)RC_U64X(v1) + (uint64_t)0ULL));
        Ps2SetGpr64(ctx, 2, v0); Ps2SetGpr64(ctx, 3, v1);
        ctx->pc = target;
        return;
    }
}

// manager_sub_010DFD18 (0x10dfd18): lift_d1's text (the reference)
void manager_sub_010DFD18(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t v0 = Ps2GprU32(ctx, 2), a2 = Ps2GprU32(ctx, 6), ra = Ps2GprU32(ctx, 31);
    uint64_t a1 = Ps2GprU64(ctx, 5);
    v0 = (uint32_t)(int8_t)READ8_B(a2, 4);
    {
        const bool c = v0 == 0;
        v0 = 1u;
        if (!c) {
            {
                const uint32_t target = ra;
                WRITE8_B(a2, 3, (uint8_t)v0);
                Ps2SetGpr64(ctx, 2, sx64(v0));
                ctx->pc = target;
                return;
            }
        }
    }
    v0 = 1u;
    if ((int64_t)a1 == (int64_t)(int32_t)v0) {
        WRITE8_B(a2, 2, (uint8_t)(uint32_t)a1);
    } else {
    }
    {
        const uint32_t target = ra;
        Ps2SetGpr64(ctx, 2, sx64(v0));
        ctx->pc = target;
        return;
    }
}

// SetInitFogColClassFromStageData (0x10ed358): lift_d1's text (the reference)
void SetInitFogColClassFromStageData(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t v0 = Ps2GprU32(ctx, 2), v1 = Ps2GprU32(ctx, 3), a0 = Ps2GprU32(ctx, 4), a1 = Ps2GprU32(ctx, 5), ra = Ps2GprU32(ctx, 31);
    float f0 = ctx->f[0], f1 = ctx->f[1];
    f1 = lift_d1::flt(READ32_B(a0, 4));
    a1 = 0x1210000u;
    a1 = a1 + 0x5E80u;
    WRITE32_B(a1, 88, lift_d1::bits(f1));
    f0 = lift_d1::flt(READ32_B(a0, 8));
    WRITE32_B(a1, 92, lift_d1::bits(f0));
    f1 = lift_d1::flt(READ32_B(a0, 12));
    WRITE32_B(a1, 100, lift_d1::bits(f1));
    f0 = lift_d1::flt(READ32_B(a0, 16));
    WRITE32_B(a1, 104, lift_d1::bits(f0));
    f1 = lift_d1::flt(READ32_B(a0, 24));
    { int32_t tmp; std::memcpy(&tmp, &f1, sizeof(tmp)); f1 = FPU_CVT_S_W(tmp); }
    WRITE32_B(a1, 96, lift_d1::bits(f1));
    v0 = READ8_B(a0, 28);
    WRITE8_B(a1, 78, (uint8_t)v0);
    v1 = READ8_B(a0, 32);
    WRITE8_B(a1, 79, (uint8_t)v1);
    v0 = READ8_B(a0, 36);
    {
        const uint32_t target = ra;
        WRITE8_B(a1, 80, (uint8_t)v0);
        ctx->f[0] = f0; ctx->f[1] = f1; Ps2SetGpr64(ctx, 2, sx64(v0)); Ps2SetGpr64(ctx, 3, sx64(v1)); Ps2SetGpr64(ctx, 5, sx64(a1));
        ctx->pc = target;
        return;
    }
}

// SetFogColAvailableFlag (0x10ed520): lift_d1's text (the reference)
void SetFogColAvailableFlag(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t a0 = Ps2GprU32(ctx, 4), a1 = Ps2GprU32(ctx, 5), ra = Ps2GprU32(ctx, 31);
    {
        const uint32_t target = ra;
        WRITE32_B(a0, 128, a1);
        
        ctx->pc = target;
        return;
    }
}

// IsEnvelopeFogType (0x10ed528): lift_d1's text (the reference)
void IsEnvelopeFogType(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t v0 = Ps2GprU32(ctx, 2), a0 = Ps2GprU32(ctx, 4), ra = Ps2GprU32(ctx, 31);
    v0 = READ32_B(a0, 64);
    v0 = v0 & 0x800u;
    {
        const uint32_t target = ra;
        v0 = 0u < v0 ? 1u : 0u;
        Ps2SetGpr64(ctx, 2, sx64(v0));
        ctx->pc = target;
        return;
    }
}

// GetJudgeVolumeClassWorkSize (0x10ee768): lift_d1's text (the reference)
void GetJudgeVolumeClassWorkSize(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t v0 = Ps2GprU32(ctx, 2), v1 = Ps2GprU32(ctx, 3), ra = Ps2GprU32(ctx, 31);
    uint64_t a0 = Ps2GprU64(ctx, 4);
    v0 = 0x30000u;
    (v1 = (uint32_t)((int32_t)((int32_t)READ32_B((uint32_t)a0, 0))));
    v0 = v0 | 0xFFFFu;
    v0 = v1 & v0;
    {
        const bool c = v0 == 0;
        (v0 = (uint32_t)((int32_t)((int32_t)SUB32((uint32_t)a0, v0))));
        if (!c) {
            (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)READ32_B(v0, 0))));
            goto L_10ee78c;
        }
    }
    (a0 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL));
L_10ee78c:
    {
        const bool c = (int64_t)a0 == 0;
        { const uint32_t _dynInsn = READ32(0x10EE790u); const int32_t _dynSimm = (int16_t)(_dynInsn & 0xFFFFu); const uint32_t _dynImm = _dynInsn & 0xFFFFu; (void)_dynSimm; (void)_dynImm; (v0 = (uint32_t)((int32_t)((int32_t)((uint32_t)_dynImm << 16)))); }
        if (!c) {
            { const uint32_t _dynInsn = READ32(0x10EE794u); const int32_t _dynSimm = (int16_t)(_dynInsn & 0xFFFFu); const uint32_t _dynImm = _dynInsn & 0xFFFFu; (void)_dynSimm; (void)_dynImm; (v1 = (uint32_t)((int32_t)((int32_t)READ32(ADD32(v0, _dynSimm))))); }
            if ((int64_t)a0 == (int64_t)(int32_t)v1) {
                v0 = 0u;
                goto L_10ee7ac;
            }
            (v0 = (uint32_t)((int32_t)((int32_t)READ32_B((uint32_t)a0, 12))));
            goto L_10ee7ac;
        }
    }
    v0 = 0u;
L_10ee7ac:
    if (v0 != 0) {
        v0 = v0 << 2;
        {
            const uint32_t target = ra;
            v0 = v0 + 0x33u;
            Ps2SetGpr64(ctx, 2, sx64(v0)); Ps2SetGpr64(ctx, 3, sx64(v1)); Ps2SetGpr64(ctx, 4, a0);
            ctx->pc = target;
            return;
        }
    }
    {
        const uint32_t target = ra;
        v0 = 0u;
        Ps2SetGpr64(ctx, 2, sx64(v0)); Ps2SetGpr64(ctx, 3, sx64(v1)); Ps2SetGpr64(ctx, 4, a0);
        ctx->pc = target;
        return;
    }
}

// GetJudgeVolumeClassObjectGroup (0x10ee808): lift_d1's text (the reference)
void GetJudgeVolumeClassObjectGroup(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t v0 = Ps2GprU32(ctx, 2), v1 = Ps2GprU32(ctx, 3), a0 = Ps2GprU32(ctx, 4), a1 = Ps2GprU32(ctx, 5), ra = Ps2GprU32(ctx, 31);
    v0 = READ32(a1);
    v1 = READ32_B(a0, 4);
    v0 = v0 >> 18;
    {
        const bool c = v1 == 0;
        v0 = v0 & 0xFFFu;
        if (!c) {
            v0 = v0 << 2;
            {
                const uint32_t target = ra;
                v0 = v1 + v0;
                Ps2SetGpr64(ctx, 2, sx64(v0)); Ps2SetGpr64(ctx, 3, sx64(v1));
                ctx->pc = target;
                return;
            }
        }
    }
    {
        const uint32_t target = ra;
        v0 = 0u;
        Ps2SetGpr64(ctx, 2, sx64(v0)); Ps2SetGpr64(ctx, 3, sx64(v1));
        ctx->pc = target;
        return;
    }
}

// GetJudgeVolumeClassObjectGroupById (0x10ee830): lift_d1's text (the reference)
void GetJudgeVolumeClassObjectGroupById(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t v1 = Ps2GprU32(ctx, 3), a0 = Ps2GprU32(ctx, 4), ra = Ps2GprU32(ctx, 31);
    uint64_t v0 = Ps2GprU64(ctx, 2), a1 = Ps2GprU64(ctx, 5);
    v1 = READ32_B(a0, 4);
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
        const uint32_t target = ra;
        Ps2SetGpr64(ctx, 2, v0); Ps2SetGpr64(ctx, 3, sx64(v1));
        ctx->pc = target;
        return;
    }
}

// SetLayoutClassCollisionWorkAdr (0x10f31b8): lift_d1's text (the reference)
void SetLayoutClassCollisionWorkAdr(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t a0 = Ps2GprU32(ctx, 4), a1 = Ps2GprU32(ctx, 5), ra = Ps2GprU32(ctx, 31);
    {
        const uint32_t target = ra;
        WRITE32_B(a0, 372, a1);
        
        ctx->pc = target;
        return;
    }
}

// manager_sub_010F3470 (0x10f3470): lift_d1's text (the reference)
void manager_sub_010F3470(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t v0 = Ps2GprU32(ctx, 2), v1 = Ps2GprU32(ctx, 3), a0 = Ps2GprU32(ctx, 4), ra = Ps2GprU32(ctx, 31);
    bool df_v1 = false;
    v0 = READ32_B(a0, 64);
    {
        const bool c = v0 != 0;
        v0 = 1u;
        if (!c) {
            v0 = READ32_B(a0, 68);
            {
                const bool c = v0 != 0;
                v0 = 1u;
                if (!c) {
                    v1 = READ32_B(a0, 72); df_v1 = true;
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
        const uint32_t target = ra;
        Ps2SetGpr64(ctx, 2, sx64(v0)); if (df_v1) Ps2SetGpr64(ctx, 3, sx64(v1));
        ctx->pc = target;
        return;
    }
}

// manager_sub_010F3668 (0x10f3668): lift_d1's text (the reference)
void manager_sub_010F3668(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t a0 = Ps2GprU32(ctx, 4), ra = Ps2GprU32(ctx, 31);
    {
        const uint32_t target = ra;
        WRITE32(a0, 0u);
        
        ctx->pc = target;
        return;
    }
}

// SetCameraResetSw (0x10f5810): lift_d1's text (the reference)
void SetCameraResetSw(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t v0 = Ps2GprU32(ctx, 2), a0 = Ps2GprU32(ctx, 4), ra = Ps2GprU32(ctx, 31);
    v0 = 0x1190000u;
    {
        const uint32_t target = ra;
        WRITE32(v0 - 0x3508u, a0);
        Ps2SetGpr64(ctx, 2, sx64(v0));
        ctx->pc = target;
        return;
    }
}

// GetResetCameraFlagForDisplay (0x10f5af8): lift_d1's text (the reference)
void GetResetCameraFlagForDisplay(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t v0 = Ps2GprU32(ctx, 2), v1 = Ps2GprU32(ctx, 3), ra = Ps2GprU32(ctx, 31);
    v1 = 0x1190000u;
    {
        const uint32_t target = ra;
        v0 = READ32(v1 - 0x3508u);
        Ps2SetGpr64(ctx, 2, sx64(v0)); Ps2SetGpr64(ctx, 3, sx64(v1));
        ctx->pc = target;
        return;
    }
}

// GetLightColMan (0x10f6358): lift_d1's text (the reference)
void GetLightColMan(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t v0 = Ps2GprU32(ctx, 2), v1 = Ps2GprU32(ctx, 3), ra = Ps2GprU32(ctx, 31);
    v1 = 0x1190000u;
    {
        const uint32_t target = ra;
        v0 = READ32(v1 - 0x34F0u);
        Ps2SetGpr64(ctx, 2, sx64(v0)); Ps2SetGpr64(ctx, 3, sx64(v1));
        ctx->pc = target;
        return;
    }
}

// TaskExecLightColMan (0x10f6408): lift_d1's text (the reference)
void TaskExecLightColMan(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t v0 = Ps2GprU32(ctx, 2), v1 = Ps2GprU32(ctx, 3), ra = Ps2GprU32(ctx, 31);
    v0 = 0x1210000u;
    v1 = 0x1190000u;
    v0 = v0 + 0x6130u;
    {
        const uint32_t target = ra;
        WRITE32(v1 - 0x34F0u, v0);
        Ps2SetGpr64(ctx, 2, sx64(v0)); Ps2SetGpr64(ctx, 3, sx64(v1));
        ctx->pc = target;
        return;
    }
}

// SetGameLightAvailable (0x10f7508): lift_d1's text (the reference)
void SetGameLightAvailable(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t a0 = Ps2GprU32(ctx, 4), a1 = Ps2GprU32(ctx, 5), ra = Ps2GprU32(ctx, 31);
    {
        const uint32_t target = ra;
        WRITE8_B(a0, 57, (uint8_t)a1);
        
        ctx->pc = target;
        return;
    }
}

// ForceGameLightFlagEnvelope (0x10f7658): lift_d1's text (the reference)
void ForceGameLightFlagEnvelope(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t v0 = Ps2GprU32(ctx, 2), v1 = Ps2GprU32(ctx, 3), a0 = Ps2GprU32(ctx, 4), a1 = Ps2GprU32(ctx, 5), ra = Ps2GprU32(ctx, 31);
    v1 = READ32_B(a0, 44);
    a1 = 0x20000u;
    v0 = 0xFFFE0000u;
    v0 = v0 | 0xFFFFu;
    v1 = v1 | a1;
    v1 = v1 & v0;
    {
        const uint32_t target = ra;
        WRITE32_B(a0, 44, v1);
        Ps2SetGpr64(ctx, 2, sx64(v0)); Ps2SetGpr64(ctx, 3, sx64(v1)); Ps2SetGpr64(ctx, 5, sx64(a1));
        ctx->pc = target;
        return;
    }
}

// IsEnvelopeGameLightType (0x10f7678): lift_d1's text (the reference)
void IsEnvelopeGameLightType(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t v0 = Ps2GprU32(ctx, 2), v1 = Ps2GprU32(ctx, 3), a0 = Ps2GprU32(ctx, 4), ra = Ps2GprU32(ctx, 31);
    v0 = READ32_B(a0, 44);
    v1 = 0x20000u;
    v0 = v0 & v1;
    {
        const uint32_t target = ra;
        v0 = 0u < v0 ? 1u : 0u;
        Ps2SetGpr64(ctx, 2, sx64(v0)); Ps2SetGpr64(ctx, 3, sx64(v1));
        ctx->pc = target;
        return;
    }
}

// SetGameLightUserParam (0x10f78a8): lift_d1's text (the reference)
void SetGameLightUserParam(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t a0 = Ps2GprU32(ctx, 4), a1 = Ps2GprU32(ctx, 5), ra = Ps2GprU32(ctx, 31);
    {
        const uint32_t target = ra;
        WRITE32_B(a0, 80, a1);
        
        ctx->pc = target;
        return;
    }
}

// SetGameLightUserFunc (0x10f78b0): lift_d1's text (the reference)
void SetGameLightUserFunc(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t a0 = Ps2GprU32(ctx, 4), a1 = Ps2GprU32(ctx, 5), ra = Ps2GprU32(ctx, 31);
    {
        const uint32_t target = ra;
        WRITE32_B(a0, 84, a1);
        
        ctx->pc = target;
        return;
    }
}

// SetGlobalLightAvailable (0x10f8b40): lift_d1's text (the reference)
void SetGlobalLightAvailable(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t a1 = Ps2GprU32(ctx, 5), ra = Ps2GprU32(ctx, 31);
    uint64_t a0 = Ps2GprU64(ctx, 4);
    if ((int64_t)a0 != 0) {
        WRITE32_B((uint32_t)a0, 324, a1);
    } else {
    }
    {
        const uint32_t target = ra;
        
        ctx->pc = target;
        return;
    }
}

// ForceGlobalLightFlagEnvelope (0x10f8c90): lift_d1's text (the reference)
void ForceGlobalLightFlagEnvelope(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t v0 = Ps2GprU32(ctx, 2), v1 = Ps2GprU32(ctx, 3), a0 = Ps2GprU32(ctx, 4), a1 = Ps2GprU32(ctx, 5), ra = Ps2GprU32(ctx, 31);
    v1 = READ32_B(a0, 272);
    a1 = 0x20000u;
    v0 = 0xFFFE0000u;
    v0 = v0 | 0xFFFFu;
    v1 = v1 | a1;
    v1 = v1 & v0;
    {
        const uint32_t target = ra;
        WRITE32_B(a0, 272, v1);
        Ps2SetGpr64(ctx, 2, sx64(v0)); Ps2SetGpr64(ctx, 3, sx64(v1)); Ps2SetGpr64(ctx, 5, sx64(a1));
        ctx->pc = target;
        return;
    }
}

// IsEnvelopeGlobalLightType (0x10f8cb0): lift_d1's text (the reference)
void IsEnvelopeGlobalLightType(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t v0 = Ps2GprU32(ctx, 2), v1 = Ps2GprU32(ctx, 3), a0 = Ps2GprU32(ctx, 4), ra = Ps2GprU32(ctx, 31);
    v0 = READ32_B(a0, 272);
    v1 = 0x20000u;
    v0 = v0 & v1;
    {
        const uint32_t target = ra;
        v0 = 0u < v0 ? 1u : 0u;
        Ps2SetGpr64(ctx, 2, sx64(v0)); Ps2SetGpr64(ctx, 3, sx64(v1));
        ctx->pc = target;
        return;
    }
}

// SetLwoDemoOrientManagerObjCutFinishFlag (0x10f9b58): lift_d1's text (the reference)
void SetLwoDemoOrientManagerObjCutFinishFlag(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t a0 = Ps2GprU32(ctx, 4), a1 = Ps2GprU32(ctx, 5), ra = Ps2GprU32(ctx, 31);
    {
        const uint32_t target = ra;
        WRITE32_B(a0, 176, a1);
        
        ctx->pc = target;
        return;
    }
}

// GetLayoutObjGroup (0x11008a0): lift_d1's text (the reference)
void GetLayoutObjGroup(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t v0 = Ps2GprU32(ctx, 2), v1 = Ps2GprU32(ctx, 3), ra = Ps2GprU32(ctx, 31);
    v1 = 0x1190000u;
    v0 = READ32(v1 - 0x34E0u);
    {
        const uint32_t target = ra;
        v0 = v0 + 0xCu;
        Ps2SetGpr64(ctx, 2, sx64(v0)); Ps2SetGpr64(ctx, 3, sx64(v1));
        ctx->pc = target;
        return;
    }
}

// GetLayoutDispObjGroup (0x11008b0): lift_d1's text (the reference)
void GetLayoutDispObjGroup(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t v0 = Ps2GprU32(ctx, 2), v1 = Ps2GprU32(ctx, 3), ra = Ps2GprU32(ctx, 31);
    v1 = 0x1190000u;
    v0 = READ32(v1 - 0x34E0u);
    {
        const uint32_t target = ra;
        v0 = v0 + 0x10u;
        Ps2SetGpr64(ctx, 2, sx64(v0)); Ps2SetGpr64(ctx, 3, sx64(v1));
        ctx->pc = target;
        return;
    }
}

// GetSkeletonFocusNodeDirect (0x1100c48): lift_d1's text (the reference)
void GetSkeletonFocusNodeDirect(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t v0 = Ps2GprU32(ctx, 2), ra = Ps2GprU32(ctx, 31);
    uint64_t a0 = Ps2GprU64(ctx, 4);
    {
        const bool c = (int64_t)a0 == 0;
        v0 = 0xFFFFFFFFu;
        if (!c) {
            (v0 = (uint32_t)((int32_t)((int32_t)READ32_B((uint32_t)a0, 8))));
        }
    }
    {
        const uint32_t target = ra;
        Ps2SetGpr64(ctx, 2, sx64(v0));
        ctx->pc = target;
        return;
    }
}

// AddObjectManagerSheetWorkSize (0x11016c0): lift_d1's text (the reference)
void AddObjectManagerSheetWorkSize(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t v0 = Ps2GprU32(ctx, 2), v1 = Ps2GprU32(ctx, 3), a0 = Ps2GprU32(ctx, 4), ra = Ps2GprU32(ctx, 31);
    v1 = 0x1210000u;
    v1 = v1 + 0x6D00u;
    v0 = READ32_B(v1, 36);
    v0 = v0 + a0;
    {
        const uint32_t target = ra;
        WRITE32_B(v1, 36, v0);
        Ps2SetGpr64(ctx, 2, sx64(v0)); Ps2SetGpr64(ctx, 3, sx64(v1));
        ctx->pc = target;
        return;
    }
}

// GetAmbientLightObjGroup (0x1101718): lift_d1's text (the reference)
void GetAmbientLightObjGroup(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t v0 = Ps2GprU32(ctx, 2), v1 = Ps2GprU32(ctx, 3), ra = Ps2GprU32(ctx, 31);
    v1 = 0x1190000u;
    v0 = READ32(v1 - 0x34E0u);
    {
        const uint32_t target = ra;
        v0 = v0 + 0x14u;
        Ps2SetGpr64(ctx, 2, sx64(v0)); Ps2SetGpr64(ctx, 3, sx64(v1));
        ctx->pc = target;
        return;
    }
}

// GetScriptPriorityGroup (0x1102b00): lift_d1's text (the reference)
void GetScriptPriorityGroup(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t at = Ps2GprU32(ctx, 1), v0 = Ps2GprU32(ctx, 2), a0 = Ps2GprU32(ctx, 4), ra = Ps2GprU32(ctx, 31);
    v0 = a0 << 2;
    at = 0x1190000u;
    at = at - 0x3390u;
    v0 = at + v0;
    {
        const uint32_t target = ra;
        Ps2SetGpr64(ctx, 1, sx64(at)); Ps2SetGpr64(ctx, 2, sx64(v0));
        ctx->pc = target;
        return;
    }
}

// manager_sub_01103080 (0x1103080): lift_d1's text (the reference)
void manager_sub_01103080(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t v1 = Ps2GprU32(ctx, 3), ra = Ps2GprU32(ctx, 31);
    uint64_t v0 = Ps2GprU64(ctx, 2), a0 = Ps2GprU64(ctx, 4);
    bool df_v1 = false;
    {
        const bool c = (int64_t)a0 == 0;
        (v0 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL));
        if (!c) {
            v1 = 0xA5A50000u; df_v1 = true;
            (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)READ32_B((uint32_t)a0, 0))));
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
        const uint32_t target = ra;
        Ps2SetGpr64(ctx, 2, v0); if (df_v1) Ps2SetGpr64(ctx, 3, sx64(v1)); Ps2SetGpr64(ctx, 4, a0);
        ctx->pc = target;
        return;
    }
}

// GetScriptAnimationGlobalBaseInfo (0x110a6d8): lift_d1's text (the reference)
void GetScriptAnimationGlobalBaseInfo(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t v0 = Ps2GprU32(ctx, 2), ra = Ps2GprU32(ctx, 31);
    v0 = 0x1190000u;
    {
        const uint32_t target = ra;
        v0 = v0 - 0x3340u;
        Ps2SetGpr64(ctx, 2, sx64(v0));
        ctx->pc = target;
        return;
    }
}

// CopyScriptAnimationWindParam (0x110a790): lift_d1's text (the reference)
void CopyScriptAnimationWindParam(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t a0 = Ps2GprU32(ctx, 4), a1 = Ps2GprU32(ctx, 5), ra = Ps2GprU32(ctx, 31);
    uint64_t v0 = Ps2GprU64(ctx, 2), v1 = Ps2GprU64(ctx, 3);
    (v0 = (uint64_t)(READ64_B(a1, 0)));
    WRITE64_B(a0, 0, v0);
    (v1 = (uint64_t)(READ64_B(a1, 8)));
    WRITE64_B(a0, 8, v1);
    (v0 = (uint64_t)(READ64_B(a1, 16)));
    WRITE64_B(a0, 16, v0);
    (v1 = (uint64_t)(READ64_B(a1, 24)));
    WRITE64_B(a0, 24, v1);
    (v0 = (uint64_t)(READ64_B(a1, 32)));
    WRITE64_B(a0, 32, v0);
    (v1 = (uint64_t)(READ64_B(a1, 40)));
    WRITE64_B(a0, 40, v1);
    (v0 = (uint64_t)(READ64_B(a1, 48)));
    WRITE64_B(a0, 48, v0);
    (v1 = (uint64_t)(READ64_B(a1, 56)));
    {
        const uint32_t target = ra;
        WRITE64_B(a0, 56, v1);
        Ps2SetGpr64(ctx, 2, v0); Ps2SetGpr64(ctx, 3, v1);
        ctx->pc = target;
        return;
    }
}

// IsNonISysObjTypeRes (0x110b818): lift_d1's text (the reference)
void IsNonISysObjTypeRes(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t ra = Ps2GprU32(ctx, 31);
    uint64_t v0 = Ps2GprU64(ctx, 2), v1 = Ps2GprU64(ctx, 3), a0 = Ps2GprU64(ctx, 4);
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
                const uint32_t target = ra;
                (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 1))));
                Ps2SetGpr64(ctx, 2, v0); Ps2SetGpr64(ctx, 3, v1);
                ctx->pc = target;
                return;
            }
        }
    }
L_110b850:
    {
        const uint32_t target = ra;
        (v0 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL));
        Ps2SetGpr64(ctx, 2, v0); Ps2SetGpr64(ctx, 3, v1);
        ctx->pc = target;
        return;
    }
}

// SetScriptCharDefaultScriptFunc (0x110cc68): lift_d1's text (the reference)
void SetScriptCharDefaultScriptFunc(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t v0 = Ps2GprU32(ctx, 2), a0 = Ps2GprU32(ctx, 4), ra = Ps2GprU32(ctx, 31);
    v0 = 0x1190000u;
    {
        const uint32_t target = ra;
        WRITE32(v0 - 0x3280u, a0);
        Ps2SetGpr64(ctx, 2, sx64(v0));
        ctx->pc = target;
        return;
    }
}

// InitSortObj (0x110de28): lift_d1's text (the reference)
void InitSortObj(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t a0 = Ps2GprU32(ctx, 4), a1 = Ps2GprU32(ctx, 5), ra = Ps2GprU32(ctx, 31);
    WRITE32(a0, a1);
    WRITE32_B(a0, 8, 0u);
    {
        const uint32_t target = ra;
        WRITE32_B(a0, 12, 0u);
        
        ctx->pc = target;
        return;
    }
}

// setDefaultMenuControlButton (0x1113ae8): lift_d1's text (the reference)
void setDefaultMenuControlButton(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t v0 = Ps2GprU32(ctx, 2), v1 = Ps2GprU32(ctx, 3), a0 = Ps2GprU32(ctx, 4), ra = Ps2GprU32(ctx, 31);
    { const uint32_t _dynInsn = READ32(0x1113AE8u); const int32_t _dynSimm = (int16_t)(_dynInsn & 0xFFFFu); const uint32_t _dynImm = _dynInsn & 0xFFFFu; (void)_dynSimm; (void)_dynImm; (v0 = (uint32_t)((int32_t)((int32_t)((uint32_t)_dynImm << 16)))); }
    a0 = 0x1190000u;
    { const uint32_t _dynInsn = READ32(0x1113AF0u); const int32_t _dynSimm = (int16_t)(_dynInsn & 0xFFFFu); const uint32_t _dynImm = _dynInsn & 0xFFFFu; (void)_dynSimm; (void)_dynImm; (v1 = (uint32_t)((int32_t)((int32_t)READ32(ADD32(v0, _dynSimm))))); }
    {
        const uint32_t target = ra;
        WRITE32(a0 - 0x3278u, v1);
        Ps2SetGpr64(ctx, 2, sx64(v0)); Ps2SetGpr64(ctx, 3, sx64(v1)); Ps2SetGpr64(ctx, 4, sx64(a0));
        ctx->pc = target;
        return;
    }
}

// InitViewIntervalClip (0x1114af0): lift_d1's text (the reference)
void InitViewIntervalClip(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t at = Ps2GprU32(ctx, 1), a0 = Ps2GprU32(ctx, 4), ra = Ps2GprU32(ctx, 31);
    float f0 = ctx->f[0];
    at = 0xBD080000u;
    at = at | 0x8888u;
    f0 = lift_d1::flt(at);
    WRITE32_B(a0, 4460, 0u);
    WRITE32_B(a0, 4456, lift_d1::bits(f0));
    {
        const uint32_t target = ra;
        WRITE32_B(a0, 4452, 0u);
        ctx->f[0] = f0; Ps2SetGpr64(ctx, 1, sx64(at));
        ctx->pc = target;
        return;
    }
}

// CopyCollisionEdge (0x11280e0): lift_d1's text (the reference)
void CopyCollisionEdge(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t v0 = Ps2GprU32(ctx, 2), v1 = Ps2GprU32(ctx, 3), a0 = Ps2GprU32(ctx, 4), a1 = Ps2GprU32(ctx, 5), ra = Ps2GprU32(ctx, 31);
    v0 = READ32(a1);
    WRITE32(a0, v0);
    v1 = READ32_B(a1, 4);
    WRITE32_B(a0, 4, v1);
    v0 = READ32_B(a1, 8);
    WRITE32_B(a0, 8, v0);
    v1 = READ32_B(a1, 12);
    {
        const uint32_t target = ra;
        WRITE32_B(a0, 12, v1);
        Ps2SetGpr64(ctx, 2, sx64(v0)); Ps2SetGpr64(ctx, 3, sx64(v1));
        ctx->pc = target;
        return;
    }
}

// MakeCollisionEdgePointer (0x1128270): lift_d1's text (the reference)
void MakeCollisionEdgePointer(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t v0 = Ps2GprU32(ctx, 2), v1 = Ps2GprU32(ctx, 3), a0 = Ps2GprU32(ctx, 4), a1 = Ps2GprU32(ctx, 5), a2 = Ps2GprU32(ctx, 6), ra = Ps2GprU32(ctx, 31);
    v0 = READ32(a1);
    WRITE32(a0, v0);
    v1 = READ32_B(a1, 4);
    WRITE32_B(a0, 8, a2);
    {
        const uint32_t target = ra;
        WRITE32_B(a0, 4, v1);
        Ps2SetGpr64(ctx, 2, sx64(v0)); Ps2SetGpr64(ctx, 3, sx64(v1));
        ctx->pc = target;
        return;
    }
}

// IsClusterCollision (0x11282f0): lift_d1's text (the reference)
void IsClusterCollision(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t v0 = Ps2GprU32(ctx, 2), a0 = Ps2GprU32(ctx, 4), ra = Ps2GprU32(ctx, 31);
    v0 = READ32_B(a0, 4);
    v0 = (int32_t)v0 < 2 ? 1u : 0u;
    {
        const uint32_t target = ra;
        v0 = v0 ^ 1u;
        Ps2SetGpr64(ctx, 2, sx64(v0));
        ctx->pc = target;
        return;
    }
}

// MakeGeometryPointerCollisionEdgeInfo (0x11283d0): lift_d1's text (the reference)
void MakeGeometryPointerCollisionEdgeInfo(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t v0 = Ps2GprU32(ctx, 2), v1 = Ps2GprU32(ctx, 3), a0 = Ps2GprU32(ctx, 4), a1 = Ps2GprU32(ctx, 5), a2 = Ps2GprU32(ctx, 6), a3 = Ps2GprU32(ctx, 7), ra = Ps2GprU32(ctx, 31);
    float f12 = ctx->f[12];
    v1 = READ32(a1);
    a3 = 4u;
    WRITE32(a0, v1);
    v0 = READ32_B(a1, 4);
    WRITE32_B(a0, 4, v0);
    v1 = READ32_B(a1, 8);
    WRITE32_B(a0, 48, a3);
    WRITE32_B(a0, 8, v1);
    WRITE32_B(a0, 12, lift_d1::bits(f12));
    {
        const uint32_t target = ra;
        WRITE32_B(a0, 16, a2);
        Ps2SetGpr64(ctx, 2, sx64(v0)); Ps2SetGpr64(ctx, 3, sx64(v1)); Ps2SetGpr64(ctx, 7, sx64(a3));
        ctx->pc = target;
        return;
    }
}

// GetCollisionResource (0x11287d0): lift_d1's text (the reference)
void GetCollisionResource(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t v0 = Ps2GprU32(ctx, 2), v1 = Ps2GprU32(ctx, 3), a0 = Ps2GprU32(ctx, 4), ra = Ps2GprU32(ctx, 31);
    v1 = READ32_B(a0, 8);
    {
        const uint32_t target = ra;
        v0 = READ32_B(v1, 32);
        Ps2SetGpr64(ctx, 2, sx64(v0)); Ps2SetGpr64(ctx, 3, sx64(v1));
        ctx->pc = target;
        return;
    }
}

// CollisionObjectJudgeCallbackExceptOneObj (0x1128818): lift_d1's text (the reference)
void CollisionObjectJudgeCallbackExceptOneObj(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t ra = Ps2GprU32(ctx, 31);
    uint64_t v0 = Ps2GprU64(ctx, 2), a0 = Ps2GprU64(ctx, 4), a1 = Ps2GprU64(ctx, 5);
    (v0 = (uint64_t)(a0 ^ a1));
    {
        const uint32_t target = ra;
        (v0 = (uint64_t)(((uint64_t)0ULL < (uint64_t)v0) ? 1 : 0));
        Ps2SetGpr64(ctx, 2, v0);
        ctx->pc = target;
        return;
    }
}

// manager_sub_011293E0 (0x11293e0): lift_d1's text (the reference)
void manager_sub_011293E0(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t v0 = Ps2GprU32(ctx, 2), a3 = Ps2GprU32(ctx, 7), ra = Ps2GprU32(ctx, 31);
    float f0 = ctx->f[0], f12 = ctx->f[12];
    uint32_t fcr31 = ctx->fcr31;
    f0 = lift_d1::flt(READ32(a3));
    fcr31 = (FPU_C_OLE_S(f0, f12)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
    {
        const bool c = (fcr31 & 0x800000u) != 0u;
        v0 = 1u;
        if (!c) {
            v0 = 0u;
        }
    }
    {
        const uint32_t target = ra;
        ctx->f[0] = f0; Ps2SetGpr64(ctx, 2, sx64(v0)); ctx->fcr31 = fcr31;
        ctx->pc = target;
        return;
    }
}

// manager_sub_01129AE8 (0x1129ae8): lift_d1's text (the reference)
void manager_sub_01129AE8(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t a0 = Ps2GprU32(ctx, 4), ra = Ps2GprU32(ctx, 31);
    {
        const uint32_t target = ra;
        WRITE32(a0, 0u);
        
        ctx->pc = target;
        return;
    }
}

// FlushCollisionEdgeLinkDBVWallFinder (0x1129b00): lift_d1's text (the reference)
void FlushCollisionEdgeLinkDBVWallFinder(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t a0 = Ps2GprU32(ctx, 4), ra = Ps2GprU32(ctx, 31);
    {
        const uint32_t target = ra;
        WRITE32_B(a0, 32, 0u);
        
        ctx->pc = target;
        return;
    }
}

// GetCollisionEdgeLinkDBWorkSize (0x1129b08): lift_d1's text (the reference)
void GetCollisionEdgeLinkDBWorkSize(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t v0 = Ps2GprU32(ctx, 2), ra = Ps2GprU32(ctx, 31);
    uint64_t a0 = Ps2GprU64(ctx, 4);
    {
        const bool c = (int64_t)a0 == 0;
        (v0 = (uint32_t)((int32_t)((int32_t)SLL32((uint32_t)a0, 2))));
        if (!c) {
            (v0 = (uint32_t)((int32_t)((int32_t)ADD32(v0, (uint32_t)a0))));
            v0 = v0 << 2;
            {
                const uint32_t target = ra;
                v0 = v0 + 0x96u;
                Ps2SetGpr64(ctx, 2, sx64(v0));
                ctx->pc = target;
                return;
            }
        }
    }
    {
        const uint32_t target = ra;
        v0 = 0u;
        Ps2SetGpr64(ctx, 2, sx64(v0));
        ctx->pc = target;
        return;
    }
}

// GetCollisionFinderWorkSize (0x112a330): lift_d1's text (the reference)
void GetCollisionFinderWorkSize(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t v0 = Ps2GprU32(ctx, 2), v1 = Ps2GprU32(ctx, 3), a0 = Ps2GprU32(ctx, 4), ra = Ps2GprU32(ctx, 31);
    v1 = a0 << 1;
    v0 = a0 << 3;
    v1 = v1 + a0;
    v1 = v1 << 2;
    v0 = v0 + v1;
    {
        const uint32_t target = ra;
        v0 = v0 + 0x66u;
        Ps2SetGpr64(ctx, 2, sx64(v0)); Ps2SetGpr64(ctx, 3, sx64(v1));
        ctx->pc = target;
        return;
    }
}

// InitGeometryPointer (0x11328a8): lift_d1's text (the reference)
void InitGeometryPointer(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t a0 = Ps2GprU32(ctx, 4), a1 = Ps2GprU32(ctx, 5), ra = Ps2GprU32(ctx, 31);
    uint64_t v0 = Ps2GprU64(ctx, 2), v1 = Ps2GprU64(ctx, 3);
    a1 = 0x1190000u;
    a1 = a1 - 0x3070u;
    (v0 = (uint64_t)(READ64_B(a1, 0)));
    WRITE64_B(a0, 0, v0);
    (v1 = (uint64_t)(READ64_B(a1, 8)));
    WRITE64_B(a0, 8, v1);
    (v0 = (uint64_t)(READ64_B(a1, 16)));
    WRITE64_B(a0, 16, v0);
    (v1 = (uint64_t)(READ64_B(a1, 24)));
    WRITE64_B(a0, 24, v1);
    (v0 = (uint64_t)(READ64_B(a1, 32)));
    WRITE64_B(a0, 32, v0);
    (v1 = (uint64_t)(READ64_B(a1, 40)));
    WRITE64_B(a0, 40, v1);
    (v0 = (uint64_t)(READ64_B(a1, 48)));
    WRITE64_B(a0, 48, v0);
    (v1 = (uint64_t)(READ64_B(a1, 56)));
    {
        const uint32_t target = ra;
        WRITE64_B(a0, 56, v1);
        Ps2SetGpr64(ctx, 2, v0); Ps2SetGpr64(ctx, 3, v1); Ps2SetGpr64(ctx, 5, sx64(a1));
        ctx->pc = target;
        return;
    }
}

// SetGeometryPointerChainPos (0x1132cb0): lift_d1's text (the reference)
void SetGeometryPointerChainPos(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t v0 = Ps2GprU32(ctx, 2), a0 = Ps2GprU32(ctx, 4), a1 = Ps2GprU32(ctx, 5), a2 = Ps2GprU32(ctx, 6), ra = Ps2GprU32(ctx, 31);
    float f12 = ctx->f[12];
    v0 = 9u;
    WRITE32_B(a0, 8, lift_d1::bits(f12));
    WRITE32_B(a0, 48, v0);
    WRITE32(a0, a1);
    {
        const uint32_t target = ra;
        WRITE32_B(a0, 4, a2);
        Ps2SetGpr64(ctx, 2, sx64(v0));
        ctx->pc = target;
        return;
    }
}

// CheckLegalCollisionGP (0x1132d58): lift_d1's text (the reference)
void CheckLegalCollisionGP(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t v0 = Ps2GprU32(ctx, 2), a0 = Ps2GprU32(ctx, 4), ra = Ps2GprU32(ctx, 31);
    uint64_t v1 = Ps2GprU64(ctx, 3);
    v0 = READ32_B(a0, 48);
    (v1 = (uint64_t)(((int64_t)(int64_t)sx64(v0) < (int64_t)(int32_t)2) ? 1 : 0));
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
        const uint32_t target = ra;
        Ps2SetGpr64(ctx, 2, sx64(v0)); Ps2SetGpr64(ctx, 3, v1);
        ctx->pc = target;
        return;
    }
}

// GetYHeightOfPlaneXZProjection (0x1134308): lift_d1's text (the reference)
void GetYHeightOfPlaneXZProjection(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t a0 = Ps2GprU32(ctx, 4), a1 = Ps2GprU32(ctx, 5), ra = Ps2GprU32(ctx, 31);
    float f0 = ctx->f[0], f1 = ctx->f[1], f2 = ctx->f[2], f3 = ctx->f[3], f4 = ctx->f[4];
    uint32_t fcr31 = ctx->fcr31;
    f2 = lift_d1::flt(READ32(a1));
    f4 = lift_d1::flt(READ32_B(a1, 8));
    f0 = lift_d1::flt(READ32(a0));
    f1 = lift_d1::flt(READ32_B(a0, 8));
    f0 = FPU_MUL_S(f0, f2);
    f2 = lift_d1::flt(READ32_B(a0, 4));
    f1 = FPU_MUL_S(f1, f4);
    f3 = lift_d1::flt(READ32_B(a0, 12));
    f0 = FPU_ADD_SF(f0, f1);
    f0 = FPU_ADD_S(f0, f3);
    f0 = FPU_NEG_S(f0);
    f0 = ps2FpuDivide(fcr31, f0, f2);
    {
        const uint32_t target = ra;
        ctx->f[0] = f0; ctx->f[1] = f1; ctx->f[2] = f2; ctx->f[3] = f3; ctx->f[4] = f4; ctx->fcr31 = fcr31;
        ctx->pc = target;
        return;
    }
}

// CheckClothChainExist (0x11398f0): lift_d1's text (the reference)
void CheckClothChainExist(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t v0 = Ps2GprU32(ctx, 2), a0 = Ps2GprU32(ctx, 4), ra = Ps2GprU32(ctx, 31);
    {
        const uint32_t target = ra;
        v0 = (uint32_t)(int8_t)READ8_B(a0, 6);
        Ps2SetGpr64(ctx, 2, sx64(v0));
        ctx->pc = target;
        return;
    }
}

// SetClothSingleFixedChainLeafFixNodeOffset (0x11399d8): lift_d1's text (the reference)
void SetClothSingleFixedChainLeafFixNodeOffset(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t a0 = Ps2GprU32(ctx, 4), a1 = Ps2GprU32(ctx, 5), ra = Ps2GprU32(ctx, 31);
    {
        const uint32_t target = ra;
        WRITE8_B(a0, 113, (uint8_t)a1);
        
        ctx->pc = target;
        return;
    }
}

// GetTwoWayLinkNext (0x1147ba8): lift_d1's text (the reference)
void GetTwoWayLinkNext(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t v0 = Ps2GprU32(ctx, 2), a0 = Ps2GprU32(ctx, 4), ra = Ps2GprU32(ctx, 31);
    {
        const uint32_t target = ra;
        v0 = READ32_B(a0, 4);
        Ps2SetGpr64(ctx, 2, sx64(v0));
        ctx->pc = target;
        return;
    }
}

// RelocateTwoWayLinkHeader (0x1147bb0): lift_d1's text (the reference)
void RelocateTwoWayLinkHeader(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t v0 = Ps2GprU32(ctx, 2), a0 = Ps2GprU32(ctx, 4), ra = Ps2GprU32(ctx, 31);
    v0 = READ32_B(a0, 4);
    if (v0 != 0) {
        WRITE32(v0, a0);
    } else {
    }
    {
        const uint32_t target = ra;
        Ps2SetGpr64(ctx, 2, sx64(v0));
        ctx->pc = target;
        return;
    }
}

// GetTwoWayLinkData (0x1147bc8): lift_d1's text (the reference)
void GetTwoWayLinkData(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t v0 = Ps2GprU32(ctx, 2), a0 = Ps2GprU32(ctx, 4), ra = Ps2GprU32(ctx, 31);
    {
        const uint32_t target = ra;
        v0 = READ32_B(a0, 8);
        Ps2SetGpr64(ctx, 2, sx64(v0));
        ctx->pc = target;
        return;
    }
}

// mcReqSetWaitFunc (0x1149460): lift_d1's text (the reference)
void mcReqSetWaitFunc(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t v0 = Ps2GprU32(ctx, 2), a0 = Ps2GprU32(ctx, 4), ra = Ps2GprU32(ctx, 31);
    v0 = 0x1220000u;
    {
        const uint32_t target = ra;
        WRITE32(v0 - 0x500Cu, a0);
        Ps2SetGpr64(ctx, 2, sx64(v0));
        ctx->pc = target;
        return;
    }
}

// mcGetProcessPer (0x1149cf8): lift_d1's text (the reference)
void mcGetProcessPer(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t v0 = Ps2GprU32(ctx, 2), ra = Ps2GprU32(ctx, 31);
    float f0 = ctx->f[0];
    v0 = 0x1190000u;
    {
        const uint32_t target = ra;
        f0 = lift_d1::flt(READ32(v0 - 0x29E8u));
        ctx->f[0] = f0; Ps2SetGpr64(ctx, 2, sx64(v0));
        ctx->pc = target;
        return;
    }
}

// mcGetGameClearFlag (0x114a300): lift_d1's text (the reference)
void mcGetGameClearFlag(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t v0 = Ps2GprU32(ctx, 2), v1 = Ps2GprU32(ctx, 3), ra = Ps2GprU32(ctx, 31);
    v1 = 0x1190000u;
    {
        const uint32_t target = ra;
        v0 = READ32(v1 - 0x29E0u);
        Ps2SetGpr64(ctx, 2, sx64(v0)); Ps2SetGpr64(ctx, 3, sx64(v1));
        ctx->pc = target;
        return;
    }
}

// mcGetCompleteTimeAttack (0x114a320): lift_d1's text (the reference)
void mcGetCompleteTimeAttack(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t v0 = Ps2GprU32(ctx, 2), v1 = Ps2GprU32(ctx, 3), a0 = Ps2GprU32(ctx, 4), a1 = Ps2GprU32(ctx, 5), a2 = Ps2GprU32(ctx, 6), ra = Ps2GprU32(ctx, 31);
    v1 = 0x1190000u;
    a2 = 0x1190000u;
    v0 = READ32(v1 - 0x29DCu);
    WRITE32(a0, v0);
    v1 = READ32(a2 - 0x29D8u);
    {
        const uint32_t target = ra;
        WRITE32(a1, v1);
        Ps2SetGpr64(ctx, 2, sx64(v0)); Ps2SetGpr64(ctx, 3, sx64(v1)); Ps2SetGpr64(ctx, 6, sx64(a2));
        ctx->pc = target;
        return;
    }
}

// mouseSetCursorPosition (0x114ad38): lift_d1's text (the reference)
void mouseSetCursorPosition(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t v0 = Ps2GprU32(ctx, 2), ra = Ps2GprU32(ctx, 31);
    float f12 = ctx->f[12], f13 = ctx->f[13];
    v0 = 0x1220000u;
    v0 = v0 - 0x5008u;
    WRITE32_B(v0, 4, lift_d1::bits(f13));
    {
        const uint32_t target = ra;
        WRITE32(v0, lift_d1::bits(f12));
        Ps2SetGpr64(ctx, 2, sx64(v0));
        ctx->pc = target;
        return;
    }
}

// mouseMoveCursorPosition (0x114ae78): lift_d1's text (the reference)
void mouseMoveCursorPosition(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t v0 = Ps2GprU32(ctx, 2), ra = Ps2GprU32(ctx, 31);
    float f0 = ctx->f[0], f1 = ctx->f[1], f12 = ctx->f[12], f13 = ctx->f[13];
    v0 = 0x1220000u;
    v0 = v0 - 0x5008u;
    f0 = lift_d1::flt(READ32(v0));
    f1 = lift_d1::flt(READ32_B(v0, 4));
    f0 = FPU_ADD_S(f0, f12);
    f1 = FPU_ADD_S(f1, f13);
    WRITE32(v0, lift_d1::bits(f0));
    {
        const uint32_t target = ra;
        WRITE32_B(v0, 4, lift_d1::bits(f1));
        ctx->f[0] = f0; ctx->f[1] = f1; Ps2SetGpr64(ctx, 2, sx64(v0));
        ctx->pc = target;
        return;
    }
}

// setVibrationEnableWorkAdrs (0x114b580): lift_d1's text (the reference)
void setVibrationEnableWorkAdrs(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t v0 = Ps2GprU32(ctx, 2), a0 = Ps2GprU32(ctx, 4), ra = Ps2GprU32(ctx, 31);
    v0 = 0x1190000u;
    {
        const uint32_t target = ra;
        WRITE32(v0 - 0xA50u, a0);
        Ps2SetGpr64(ctx, 2, sx64(v0));
        ctx->pc = target;
        return;
    }
}

// usbdevMouseGetStatus (0x114b6e0): lift_d1's text (the reference)
void usbdevMouseGetStatus(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t v0 = Ps2GprU32(ctx, 2), ra = Ps2GprU32(ctx, 31);
    v0 = 0x1220000u;
    {
        const uint32_t target = ra;
        v0 = v0 - 0x4FACu;
        Ps2SetGpr64(ctx, 2, sx64(v0));
        ctx->pc = target;
        return;
    }
}

// windowSemiTransRate (0x1155b50): lift_d1's text (the reference)
void windowSemiTransRate(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t v0 = Ps2GprU32(ctx, 2), ra = Ps2GprU32(ctx, 31);
    v0 = 0x1190000u;
    {
        const uint32_t target = ra;
        v0 = v0 - 0x530u;
        Ps2SetGpr64(ctx, 2, sx64(v0));
        ctx->pc = target;
        return;
    }
}

// windowGetCurrentWindowHandle (0x11560f8): lift_d1's text (the reference)
void windowGetCurrentWindowHandle(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t v0 = Ps2GprU32(ctx, 2), v1 = Ps2GprU32(ctx, 3), ra = Ps2GprU32(ctx, 31);
    v1 = 0x1190000u;
    {
        const uint32_t target = ra;
        v0 = READ32(v1 - 0x548u);
        Ps2SetGpr64(ctx, 2, sx64(v0)); Ps2SetGpr64(ctx, 3, sx64(v1));
        ctx->pc = target;
        return;
    }
}

// windowSetTex (0x1156338): lift_d1's text (the reference)
void windowSetTex(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t v0 = Ps2GprU32(ctx, 2), a0 = Ps2GprU32(ctx, 4), ra = Ps2GprU32(ctx, 31);
    v0 = 0x1190000u;
    {
        const uint32_t target = ra;
        WRITE32(v0 - 0x570u, a0);
        Ps2SetGpr64(ctx, 2, sx64(v0));
        ctx->pc = target;
        return;
    }
}

// adpcmOpenChkComplete (0x1157870): lift_d1's text (the reference)
void adpcmOpenChkComplete(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint64_t v0 = Ps2GprU64(ctx, 2), a0 = Ps2GprU64(ctx, 4), ra = Ps2GprU64(ctx, 31);
    uint64_t hi = ctx->hi;
    uint64_t lo = ctx->lo;
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 1376))));
    { int64_t result = (int64_t)(int32_t)a0 * (int64_t)(int32_t)v0; lo = (uint64_t)(int64_t)(int32_t)result; hi = (uint64_t)(int64_t)(int32_t)(result >> 32); (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)result))); }
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)290 << 16))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v0, (uint32_t)a0))));
    (v0 = (uint64_t)(READ64_B((uint32_t)v0, -17136)));
    (v0 = (uint64_t)(v0 >> (32 + 4)));
    {
        const uint32_t target = (uint32_t)ra;
        (v0 = (uint64_t)(v0 & (uint64_t)(uint16_t)1));
        Ps2SetGpr64(ctx, 2, v0); Ps2SetGpr64(ctx, 4, a0); ctx->hi = hi; ctx->lo = lo;
        ctx->pc = target;
        return;
    }
}

// adpcmCloseReq (0x1157940): lift_d1's text (the reference)
void adpcmCloseReq(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint64_t at = Ps2GprU64(ctx, 1), v0 = Ps2GprU64(ctx, 2), v1 = Ps2GprU64(ctx, 3), a0 = Ps2GprU64(ctx, 4), a1 = Ps2GprU64(ctx, 5), ra = Ps2GprU64(ctx, 31);
    uint64_t hi = ctx->hi;
    uint64_t lo = ctx->lo;
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 1376))));
    { int64_t result = (int64_t)(int32_t)a0 * (int64_t)(int32_t)v0; lo = (uint64_t)(int64_t)(int32_t)result; hi = (uint64_t)(int64_t)(int32_t)(result >> 32); (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)result))); }
    (at = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)290 << 16))));
    (at = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)at, 4294950104))));
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)at, (uint32_t)a0))));
    {
        const bool c = (int64_t)a1 == 0;
        WRITE32_B((uint32_t)a0, 68, (uint32_t)a1);
        if (!c) {
            WRITE32_B((uint32_t)a1, 0, 0u);
        }
    }
    (v0 = (uint64_t)(READ64_B((uint32_t)a0, 56)));
    (v1 = (uint64_t)(0ULL | (uint64_t)(uint16_t)32768));
    (v1 = (uint64_t)(RC_U64X(v1) << 22));
    (v0 = (uint64_t)(v0 | v1));
    {
        const uint32_t target = (uint32_t)ra;
        WRITE64_B((uint32_t)a0, 56, v0);
        Ps2SetGpr64(ctx, 1, at); Ps2SetGpr64(ctx, 2, v0); Ps2SetGpr64(ctx, 3, v1); Ps2SetGpr64(ctx, 4, a0); ctx->hi = hi; ctx->lo = lo;
        ctx->pc = target;
        return;
    }
}

// adpcmCloseFadeReq (0x1157978): lift_d1's text (the reference)
void adpcmCloseFadeReq(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint64_t at = Ps2GprU64(ctx, 1), v0 = Ps2GprU64(ctx, 2), v1 = Ps2GprU64(ctx, 3), a0 = Ps2GprU64(ctx, 4), a1 = Ps2GprU64(ctx, 5), ra = Ps2GprU64(ctx, 31);
    float f12 = ctx->f[12];
    uint64_t hi = ctx->hi;
    uint64_t lo = ctx->lo;
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 1376))));
    (v1 = (uint64_t)(0ULL | (uint64_t)(uint16_t)32768));
    (v1 = (uint64_t)(RC_U64X(v1) << 24));
    { int64_t result = (int64_t)(int32_t)a0 * (int64_t)(int32_t)v0; lo = (uint64_t)(int64_t)(int32_t)result; hi = (uint64_t)(int64_t)(int32_t)(result >> 32); (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)result))); }
    (at = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)290 << 16))));
    (at = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)at, 4294950104))));
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)at, (uint32_t)a0))));
    (v0 = (uint64_t)(READ64_B((uint32_t)a0, 56)));
    { float f = f12; uint32_t bits; std::memcpy(&bits, &f, sizeof(bits)); WRITE32_B((uint32_t)a0, 80, bits); }
    (v0 = (uint64_t)(v0 | v1));
    WRITE32_B((uint32_t)a0, 88, 0u);
    WRITE64_B((uint32_t)a0, 56, v0);
    {
        const bool c = (int64_t)a1 == 0;
        WRITE32_B((uint32_t)a0, 68, (uint32_t)a1);
        if (!c) {
            WRITE32_B((uint32_t)a1, 0, 0u);
        }
    }
    (v0 = (uint64_t)(READ64_B((uint32_t)a0, 56)));
    (v1 = (uint64_t)(0ULL | (uint64_t)(uint16_t)32768));
    (v1 = (uint64_t)(RC_U64X(v1) << 22));
    (v0 = (uint64_t)(v0 | v1));
    {
        const uint32_t target = (uint32_t)ra;
        WRITE64_B((uint32_t)a0, 56, v0);
        Ps2SetGpr64(ctx, 1, at); Ps2SetGpr64(ctx, 2, v0); Ps2SetGpr64(ctx, 3, v1); Ps2SetGpr64(ctx, 4, a0); ctx->hi = hi; ctx->lo = lo;
        ctx->pc = target;
        return;
    }
}

// adpcmCloseFadeSetVol (0x11579d0): lift_d1's text (the reference)
void adpcmCloseFadeSetVol(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t at = Ps2GprU32(ctx, 1), v0 = Ps2GprU32(ctx, 2), a0 = Ps2GprU32(ctx, 4), ra = Ps2GprU32(ctx, 31);
    float f0 = ctx->f[0], f1 = ctx->f[1], f12 = ctx->f[12];
    uint32_t fcr31 = ctx->fcr31;
    uint64_t hi = ctx->hi;
    uint64_t lo = ctx->lo;
    v0 = 0x560u;
    f1 = lift_d1::flt(0u);
    { int64_t result = (int64_t)(int32_t)a0 * (int64_t)(int32_t)v0; lo = (uint64_t)(int64_t)(int32_t)result; hi = (uint64_t)(int64_t)(int32_t)(result >> 32); (a0 = (uint32_t)((int32_t)((int32_t)result))); }
    at = 0x1220000u;
    at = at - 0x4328u;
    a0 = at + a0;
    f0 = lift_d1::flt(READ32_B(a0, 76));
    fcr31 = (FPU_C_EQ_S(f0, f1)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
    if ((fcr31 & 0x800000u) == 0u) {
        WRITE32_B(a0, 80, lift_d1::bits(f12));
    } else {
    }
    {
        const uint32_t target = ra;
        ctx->f[0] = f0; ctx->f[1] = f1; Ps2SetGpr64(ctx, 1, sx64(at)); Ps2SetGpr64(ctx, 2, sx64(v0)); Ps2SetGpr64(ctx, 4, sx64(a0)); ctx->fcr31 = fcr31; ctx->hi = hi; ctx->lo = lo;
        ctx->pc = target;
        return;
    }
}

// adpcmCloseChkComplete (0x1157a00): lift_d1's text (the reference)
void adpcmCloseChkComplete(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t ra = Ps2GprU32(ctx, 31);
    uint64_t v0 = Ps2GprU64(ctx, 2), a0 = Ps2GprU64(ctx, 4);
    (v0 = (uint64_t)(a0 ^ (uint64_t)(uint16_t)1));
    {
        const uint32_t target = ra;
        (v0 = (uint64_t)(((uint64_t)v0 < (uint64_t)(int64_t)(int32_t)1) ? 1 : 0));
        Ps2SetGpr64(ctx, 2, v0);
        ctx->pc = target;
        return;
    }
}

// adpcmDeleteAdpcmSeq (0x1157a10): lift_d1's text (the reference)
void adpcmDeleteAdpcmSeq(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint64_t at = Ps2GprU64(ctx, 1), v0 = Ps2GprU64(ctx, 2), v1 = Ps2GprU64(ctx, 3), a0 = Ps2GprU64(ctx, 4), ra = Ps2GprU64(ctx, 31);
    uint64_t hi = ctx->hi;
    uint64_t lo = ctx->lo;
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 1376))));
    (v1 = (uint64_t)(0ULL | (uint64_t)(uint16_t)32768));
    (v1 = (uint64_t)(RC_U64X(v1) << 23));
    { int64_t result = (int64_t)(int32_t)a0 * (int64_t)(int32_t)v0; lo = (uint64_t)(int64_t)(int32_t)result; hi = (uint64_t)(int64_t)(int32_t)(result >> 32); (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)result))); }
    (at = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)290 << 16))));
    (at = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)at, 4294950104))));
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)at, (uint32_t)a0))));
    (v0 = (uint64_t)(READ64_B((uint32_t)a0, 56)));
    (v0 = (uint64_t)(v0 | v1));
    {
        const uint32_t target = (uint32_t)ra;
        WRITE64_B((uint32_t)a0, 56, v0);
        Ps2SetGpr64(ctx, 1, at); Ps2SetGpr64(ctx, 2, v0); Ps2SetGpr64(ctx, 3, v1); Ps2SetGpr64(ctx, 4, a0); ctx->hi = hi; ctx->lo = lo;
        ctx->pc = target;
        return;
    }
}

// adpcmSetVol (0x1157a40): lift_d1's text (the reference)
void adpcmSetVol(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint64_t at = Ps2GprU64(ctx, 1), v0 = Ps2GprU64(ctx, 2), a0 = Ps2GprU64(ctx, 4), ra = Ps2GprU64(ctx, 31);
    float f12 = ctx->f[12], f13 = ctx->f[13];
    uint64_t hi = ctx->hi;
    uint64_t lo = ctx->lo;
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 1376))));
    { int64_t result = (int64_t)(int32_t)a0 * (int64_t)(int32_t)v0; lo = (uint64_t)(int64_t)(int32_t)result; hi = (uint64_t)(int64_t)(int32_t)(result >> 32); (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)result))); }
    (at = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)290 << 16))));
    (at = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)at, 4294950104))));
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)at, (uint32_t)a0))));
    (v0 = (uint64_t)(READ64_B((uint32_t)a0, 56)));
    (v0 = (uint64_t)(v0 >> (32 + 7)));
    (v0 = (uint64_t)(v0 & (uint64_t)(uint16_t)1));
    if ((int64_t)v0 == 0) {
        { float f = f12; uint32_t bits; std::memcpy(&bits, &f, sizeof(bits)); WRITE32_B((uint32_t)a0, 88, bits); }
        { float f = f13; uint32_t bits; std::memcpy(&bits, &f, sizeof(bits)); WRITE32_B((uint32_t)a0, 80, bits); }
    }
    {
        const uint32_t target = (uint32_t)ra;
        Ps2SetGpr64(ctx, 1, at); Ps2SetGpr64(ctx, 2, v0); Ps2SetGpr64(ctx, 4, a0); ctx->hi = hi; ctx->lo = lo;
        ctx->pc = target;
        return;
    }
}

// adpcmSetPitch (0x1157a78): lift_d1's text (the reference)
void adpcmSetPitch(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t at = Ps2GprU32(ctx, 1), v0 = Ps2GprU32(ctx, 2), a0 = Ps2GprU32(ctx, 4), ra = Ps2GprU32(ctx, 31);
    float f12 = ctx->f[12], f13 = ctx->f[13];
    uint64_t hi = ctx->hi;
    uint64_t lo = ctx->lo;
    v0 = 0x560u;
    { int64_t result = (int64_t)(int32_t)a0 * (int64_t)(int32_t)v0; lo = (uint64_t)(int64_t)(int32_t)result; hi = (uint64_t)(int64_t)(int32_t)(result >> 32); (a0 = (uint32_t)((int32_t)((int32_t)result))); }
    at = 0x1220000u;
    at = at - 0x4328u;
    a0 = at + a0;
    WRITE32_B(a0, 100, lift_d1::bits(f13));
    {
        const uint32_t target = ra;
        WRITE32_B(a0, 96, lift_d1::bits(f12));
        Ps2SetGpr64(ctx, 1, sx64(at)); Ps2SetGpr64(ctx, 2, sx64(v0)); Ps2SetGpr64(ctx, 4, sx64(a0)); ctx->hi = hi; ctx->lo = lo;
        ctx->pc = target;
        return;
    }
}

// adpcmGamePause (0x1157b78): lift_d1's text (the reference)
void adpcmGamePause(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t v0 = Ps2GprU32(ctx, 2), a0 = Ps2GprU32(ctx, 4), ra = Ps2GprU32(ctx, 31);
    v0 = 0x1190000u;
    {
        const uint32_t target = ra;
        WRITE32(v0 - 0x508u, a0);
        Ps2SetGpr64(ctx, 2, sx64(v0));
        ctx->pc = target;
        return;
    }
}

// seSetVol (0x11587a8): lift_d1's text (the reference)
void seSetVol(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t a0 = Ps2GprU32(ctx, 4), ra = Ps2GprU32(ctx, 31);
    float f12 = ctx->f[12];
    {
        const uint32_t target = ra;
        WRITE32_B(a0, 24, lift_d1::bits(f12));
        
        ctx->pc = target;
        return;
    }
}

// manager_sub_01159008 (0x1159008): lift_d1's text (the reference)
void manager_sub_01159008(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t a0 = Ps2GprU32(ctx, 4), a1 = Ps2GprU32(ctx, 5), a2 = Ps2GprU32(ctx, 6), a3 = Ps2GprU32(ctx, 7), ra = Ps2GprU32(ctx, 31);
    uint64_t v0 = Ps2GprU64(ctx, 2), v1 = Ps2GprU64(ctx, 3);
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)290 << 16))));
    a1 = READ32_B(a0, 72);
    (a0 = (uint32_t)((int32_t)((int32_t)READ32_B((uint32_t)v0, -10464))));
    a2 = 0x1220000u;
    a3 = 0x1220000u;
    (v0 = (uint64_t)(READ64_B(a0, 0)));
    WRITE64_B(a1, 0, v0);
    (v1 = (uint64_t)(READ64_B(a0, 8)));
    WRITE64_B(a1, 8, v1);
    (v0 = (uint64_t)(READ64_B(a0, 16)));
    WRITE64_B(a1, 16, v0);
    (v1 = (uint64_t)(READ64_B(a0, 24)));
    WRITE64_B(a1, 24, v1);
    (v0 = (uint64_t)(READ64_B(a0, 32)));
    WRITE64_B(a1, 32, v0);
    (v1 = (uint64_t)(READ64_B(a0, 40)));
    WRITE64_B(a1, 40, v1);
    (v0 = (uint64_t)(READ64_B(a0, 48)));
    WRITE64_B(a1, 48, v0);
    (v1 = (uint64_t)(READ64_B(a0, 56)));
    WRITE64_B(a1, 56, v1);
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)READ32_B(a2, -10456))));
    WRITE32_B(a1, 64, (uint32_t)v0);
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)READ32_B(a3, -10460))));
    {
        const uint32_t target = ra;
        WRITE32_B(a1, 68, (uint32_t)v1);
        Ps2SetGpr64(ctx, 2, v0); Ps2SetGpr64(ctx, 3, v1); Ps2SetGpr64(ctx, 4, sx64(a0)); Ps2SetGpr64(ctx, 5, sx64(a1)); Ps2SetGpr64(ctx, 6, sx64(a2)); Ps2SetGpr64(ctx, 7, sx64(a3));
        ctx->pc = target;
        return;
    }
}

// seDelayPlay3DTest (0x11590b0): lift_d1's text (the reference)
void seDelayPlay3DTest(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t ra = Ps2GprU32(ctx, 31);
    uint64_t v0 = Ps2GprU64(ctx, 2);
    {
        const uint32_t target = ra;
        (v0 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL));
        Ps2SetGpr64(ctx, 2, v0);
        ctx->pc = target;
        return;
    }
}

// sheetFsAddStageTexSegIndex (0x115bab8): lift_d1's text (the reference)
void sheetFsAddStageTexSegIndex(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t v0 = Ps2GprU32(ctx, 2), v1 = Ps2GprU32(ctx, 3), a0 = Ps2GprU32(ctx, 4), ra = Ps2GprU32(ctx, 31);
    v1 = 0x1220000u;
    a0 = a0 << 3;
    v0 = READ32(v1 - 0x1408u);
    a0 = a0 + v0;
    v0 = READ32(a0);
    v1 = v0 + 1u;
    {
        const uint32_t target = ra;
        WRITE32(a0, v1);
        Ps2SetGpr64(ctx, 2, sx64(v0)); Ps2SetGpr64(ctx, 3, sx64(v1)); Ps2SetGpr64(ctx, 4, sx64(a0));
        ctx->pc = target;
        return;
    }
}

// sheetFsDecStageTexSegIndex (0x115bad8): lift_d1's text (the reference)
void sheetFsDecStageTexSegIndex(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t v0 = Ps2GprU32(ctx, 2), v1 = Ps2GprU32(ctx, 3), a0 = Ps2GprU32(ctx, 4), ra = Ps2GprU32(ctx, 31);
    v1 = 0x1220000u;
    a0 = a0 << 3;
    v0 = READ32(v1 - 0x1408u);
    a0 = a0 + v0;
    v0 = READ32(a0);
    v0 = v0 - 1u;
    {
        const uint32_t target = ra;
        WRITE32(a0, v0);
        Ps2SetGpr64(ctx, 2, sx64(v0)); Ps2SetGpr64(ctx, 3, sx64(v1)); Ps2SetGpr64(ctx, 4, sx64(a0));
        ctx->pc = target;
        return;
    }
}

// sheetFsSetStageTexSegTexSegDef (0x115bb98): lift_d1's text (the reference)
void sheetFsSetStageTexSegTexSegDef(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t v0 = Ps2GprU32(ctx, 2), v1 = Ps2GprU32(ctx, 3), a0 = Ps2GprU32(ctx, 4), a1 = Ps2GprU32(ctx, 5), ra = Ps2GprU32(ctx, 31);
    v0 = 0x1220000u;
    a0 = a0 << 3;
    v1 = READ32(v0 - 0x1408u);
    a0 = a0 + v1;
    {
        const uint32_t target = ra;
        WRITE32_B(a0, 4, a1);
        Ps2SetGpr64(ctx, 2, sx64(v0)); Ps2SetGpr64(ctx, 3, sx64(v1)); Ps2SetGpr64(ctx, 4, sx64(a0));
        ctx->pc = target;
        return;
    }
}

// sheetFsGetStageTexSegTexSegDef (0x115bbb0): lift_d1's text (the reference)
void sheetFsGetStageTexSegTexSegDef(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t v0 = Ps2GprU32(ctx, 2), v1 = Ps2GprU32(ctx, 3), a0 = Ps2GprU32(ctx, 4), ra = Ps2GprU32(ctx, 31);
    v0 = 0x1220000u;
    a0 = a0 << 3;
    v1 = READ32(v0 - 0x1408u);
    a0 = a0 + v1;
    {
        const uint32_t target = ra;
        v0 = READ32_B(a0, 4);
        Ps2SetGpr64(ctx, 2, sx64(v0)); Ps2SetGpr64(ctx, 3, sx64(v1)); Ps2SetGpr64(ctx, 4, sx64(a0));
        ctx->pc = target;
        return;
    }
}

// sheetFsGetDirectFileSize (0x115c1e0): lift_d1's text (the reference)
void sheetFsGetDirectFileSize(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t v0 = Ps2GprU32(ctx, 2), v1 = Ps2GprU32(ctx, 3), a0 = Ps2GprU32(ctx, 4), ra = Ps2GprU32(ctx, 31);
    v0 = 0x1220000u;
    a0 = a0 << 3;
    v1 = READ32(v0 - 0x13C4u);
    v1 = v1 + a0;
    {
        const uint32_t target = ra;
        v0 = READ32_B(v1, 4);
        Ps2SetGpr64(ctx, 2, sx64(v0)); Ps2SetGpr64(ctx, 3, sx64(v1)); Ps2SetGpr64(ctx, 4, sx64(a0));
        ctx->pc = target;
        return;
    }
}

// sheetFileGetSize (0x115cfc0): lift_d1's text (the reference)
void sheetFileGetSize(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t v0 = Ps2GprU32(ctx, 2), a0 = Ps2GprU32(ctx, 4), ra = Ps2GprU32(ctx, 31);
    {
        const uint32_t target = ra;
        v0 = READ32_B(a0, 16);
        Ps2SetGpr64(ctx, 2, sx64(v0));
        ctx->pc = target;
        return;
    }
}

// sheetHeaderObjSearchSheetId (0x115ec48): lift_d1's text (the reference)
void sheetHeaderObjSearchSheetId(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t v0 = Ps2GprU32(ctx, 2), v1 = Ps2GprU32(ctx, 3), a0 = Ps2GprU32(ctx, 4), ra = Ps2GprU32(ctx, 31);
    v0 = 0x1190000u;
    a0 = a0 << 2;
    v1 = READ32(v0 - 0x1E0u);
    a0 = a0 + v1;
    {
        const uint32_t target = ra;
        v0 = READ32(a0);
        Ps2SetGpr64(ctx, 2, sx64(v0)); Ps2SetGpr64(ctx, 3, sx64(v1)); Ps2SetGpr64(ctx, 4, sx64(a0));
        ctx->pc = target;
        return;
    }
}

// sheetMgrAddSegLoadList (0x115ed10): lift_d1's text (the reference)
void sheetMgrAddSegLoadList(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t v0 = Ps2GprU32(ctx, 2), v1 = Ps2GprU32(ctx, 3), a0 = Ps2GprU32(ctx, 4), ra = Ps2GprU32(ctx, 31);
    v0 = 0x1220000u;
    v0 = v0 - 0x1378u;
    v1 = READ32(v0);
    WRITE32_B(a0, 76, v1);
    {
        const uint32_t target = ra;
        WRITE32(v0, a0);
        Ps2SetGpr64(ctx, 2, sx64(v0)); Ps2SetGpr64(ctx, 3, sx64(v1));
        ctx->pc = target;
        return;
    }
}

// sheetMgrGetSegLoadHeader (0x115ed28): lift_d1's text (the reference)
void sheetMgrGetSegLoadHeader(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t v0 = Ps2GprU32(ctx, 2), v1 = Ps2GprU32(ctx, 3), ra = Ps2GprU32(ctx, 31);
    v1 = 0x1220000u;
    {
        const uint32_t target = ra;
        v0 = READ32(v1 - 0x1378u);
        Ps2SetGpr64(ctx, 2, sx64(v0)); Ps2SetGpr64(ctx, 3, sx64(v1));
        ctx->pc = target;
        return;
    }
}

// sheetMgrAddSegInitList (0x115ed38): lift_d1's text (the reference)
void sheetMgrAddSegInitList(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t v0 = Ps2GprU32(ctx, 2), v1 = Ps2GprU32(ctx, 3), a0 = Ps2GprU32(ctx, 4), ra = Ps2GprU32(ctx, 31);
    v1 = 0x1220000u;
    WRITE32_B(a0, 80, 0u);
    v1 = v1 - 0x1370u;
    v0 = READ32(v1);
    if (v0 == 0) {
        WRITE32(v1, a0);
    } else {
        v1 = 0x1220000u;
        v0 = READ32(v1 - 0x136Cu);
        WRITE32_B(v0, 80, a0);
        goto L_115ed64;
    }
    v1 = 0x1220000u;
L_115ed64:
    {
        const uint32_t target = ra;
        WRITE32(v1 - 0x136Cu, a0);
        Ps2SetGpr64(ctx, 2, sx64(v0)); Ps2SetGpr64(ctx, 3, sx64(v1));
        ctx->pc = target;
        return;
    }
}

// sheetMgrGetSegInitHeader (0x115edb0): lift_d1's text (the reference)
void sheetMgrGetSegInitHeader(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t v0 = Ps2GprU32(ctx, 2), v1 = Ps2GprU32(ctx, 3), ra = Ps2GprU32(ctx, 31);
    v1 = 0x1220000u;
    {
        const uint32_t target = ra;
        v0 = READ32(v1 - 0x1370u);
        Ps2SetGpr64(ctx, 2, sx64(v0)); Ps2SetGpr64(ctx, 3, sx64(v1));
        ctx->pc = target;
        return;
    }
}

// sheetMgrSetNoWaitMode (0x115fa20): lift_d1's text (the reference)
void sheetMgrSetNoWaitMode(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t v0 = Ps2GprU32(ctx, 2), a0 = Ps2GprU32(ctx, 4), ra = Ps2GprU32(ctx, 31);
    v0 = 0x1190000u;
    {
        const uint32_t target = ra;
        WRITE32(v0 - 0x1D0u, a0);
        Ps2SetGpr64(ctx, 2, sx64(v0));
        ctx->pc = target;
        return;
    }
}

// get_camera_lws_animation_param (0x1161c80): lift_d1's text (the reference)
void get_camera_lws_animation_param(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t v0 = Ps2GprU32(ctx, 2), a0 = Ps2GprU32(ctx, 4), ra = Ps2GprU32(ctx, 31);
    {
        const uint32_t target = ra;
        v0 = READ32(a0);
        Ps2SetGpr64(ctx, 2, sx64(v0));
        ctx->pc = target;
        return;
    }
}

// get_boot_obj_camera_lws_animation (0x1161f70): lift_d1's text (the reference)
void get_boot_obj_camera_lws_animation(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t v0 = Ps2GprU32(ctx, 2), a0 = Ps2GprU32(ctx, 4), ra = Ps2GprU32(ctx, 31);
    {
        const uint32_t target = ra;
        v0 = READ32_B(a0, 172);
        Ps2SetGpr64(ctx, 2, sx64(v0));
        ctx->pc = target;
        return;
    }
}

// is_loop_camera_lws_animation (0x1161f78): lift_d1's text (the reference)
void is_loop_camera_lws_animation(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t v0 = Ps2GprU32(ctx, 2), a0 = Ps2GprU32(ctx, 4), ra = Ps2GprU32(ctx, 31);
    v0 = READ32_B(a0, 16);
    v0 = v0 & 7u;
    {
        const uint32_t target = ra;
        v0 = 0u < v0 ? 1u : 0u;
        Ps2SetGpr64(ctx, 2, sx64(v0));
        ctx->pc = target;
        return;
    }
}

// is_sub_type_camera_lws_animation (0x1161fa0): lift_d1's text (the reference)
void is_sub_type_camera_lws_animation(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t v0 = Ps2GprU32(ctx, 2), a0 = Ps2GprU32(ctx, 4), ra = Ps2GprU32(ctx, 31);
    v0 = READ32_B(a0, 16);
    v0 = v0 >> 3;
    v0 = v0 & 7u;
    v0 = v0 ^ 1u;
    {
        const uint32_t target = ra;
        v0 = v0 < 1u ? 1u : 0u;
        Ps2SetGpr64(ctx, 2, sx64(v0));
        ctx->pc = target;
        return;
    }
}

// reset_main_type_camera_lws_animation_update_flag (0x1162018): lift_d1's text (the reference)
void reset_main_type_camera_lws_animation_update_flag(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t v0 = Ps2GprU32(ctx, 2), ra = Ps2GprU32(ctx, 31);
    v0 = 0x1190000u;
    {
        const uint32_t target = ra;
        WRITE32(v0 - 0x1B0u, 0u);
        Ps2SetGpr64(ctx, 2, sx64(v0));
        ctx->pc = target;
        return;
    }
}

// is_same_camera_lws_animation (0x1162150): lift_d1's text (the reference)
void is_same_camera_lws_animation(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t v1 = Ps2GprU32(ctx, 3), ra = Ps2GprU32(ctx, 31);
    uint64_t v0 = Ps2GprU64(ctx, 2), a0 = Ps2GprU64(ctx, 4), a1 = Ps2GprU64(ctx, 5);
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
                (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)READ32_B((uint32_t)a0, -20464))));
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
        const uint32_t target = ra;
        Ps2SetGpr64(ctx, 2, v0); if (df_v1) Ps2SetGpr64(ctx, 3, sx64(v1)); Ps2SetGpr64(ctx, 4, a0);
        ctx->pc = target;
        return;
    }
}

// stop_camera_lws_animation_geometry_pointer_reference (0x1162208): lift_d1's text (the reference)
void stop_camera_lws_animation_geometry_pointer_reference(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t at = Ps2GprU32(ctx, 1), v0 = Ps2GprU32(ctx, 2), ra = Ps2GprU32(ctx, 31);
    uint64_t v1 = Ps2GprU64(ctx, 3), a0 = Ps2GprU64(ctx, 4);
    (v0 = (uint32_t)((int32_t)((int32_t)SLL32((uint32_t)a0, 1))));
    (v1 = (uint64_t)(((uint64_t)a0 < (uint64_t)(int64_t)(int32_t)10) ? 1 : 0));
    {
        const bool c = (int64_t)v1 == 0;
        (v0 = (uint32_t)((int32_t)((int32_t)ADD32(v0, (uint32_t)a0))));
        if (!c) {
            (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)SLL32(v0, 6))));
            at = 0x1230000u;
            (at = (uint32_t)((int32_t)((int32_t)ADD32(at, (uint32_t)v1))));
            WRITE32(at - 0x4FA8u, 0u);
            {
                const uint32_t target = ra;
                v0 = 1u;
                Ps2SetGpr64(ctx, 1, sx64(at)); Ps2SetGpr64(ctx, 2, sx64(v0)); Ps2SetGpr64(ctx, 3, v1);
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

// stop_camera_lws_animation_callback (0x1162238): lift_d1's text (the reference)
void stop_camera_lws_animation_callback(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t at = Ps2GprU32(ctx, 1), v0 = Ps2GprU32(ctx, 2), ra = Ps2GprU32(ctx, 31);
    uint64_t v1 = Ps2GprU64(ctx, 3), a0 = Ps2GprU64(ctx, 4);
    (v0 = (uint32_t)((int32_t)((int32_t)SLL32((uint32_t)a0, 1))));
    (v1 = (uint64_t)(((uint64_t)a0 < (uint64_t)(int64_t)(int32_t)10) ? 1 : 0));
    {
        const bool c = (int64_t)v1 == 0;
        (v0 = (uint32_t)((int32_t)((int32_t)ADD32(v0, (uint32_t)a0))));
        if (!c) {
            (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)SLL32(v0, 6))));
            at = 0x1230000u;
            (at = (uint32_t)((int32_t)((int32_t)ADD32(at, (uint32_t)v1))));
            WRITE32(at - 0x4F50u, 0u);
            {
                const uint32_t target = ra;
                v0 = 1u;
                Ps2SetGpr64(ctx, 1, sx64(at)); Ps2SetGpr64(ctx, 2, sx64(v0)); Ps2SetGpr64(ctx, 3, v1);
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

// manager_sub_011629B8 (0x11629b8): lift_d1's text (the reference)
void manager_sub_011629B8(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t v0 = Ps2GprU32(ctx, 2), a0 = Ps2GprU32(ctx, 4), ra = Ps2GprU32(ctx, 31);
    float f0 = ctx->f[0], f1 = ctx->f[1];
    uint32_t fcr31 = ctx->fcr31;
    v0 = READ32_B(a0, 100);
    {
        const bool c = v0 == 0;
        v0 = 0u;
        if (!c) {
            f1 = lift_d1::flt(READ32_B(a0, 104));
            f0 = lift_d1::flt(0u);
            fcr31 = (FPU_C_OLE_S(f0, f1)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
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
        const uint32_t target = ra;
        ctx->f[0] = f0; ctx->f[1] = f1; Ps2SetGpr64(ctx, 2, sx64(v0)); ctx->fcr31 = fcr31;
        ctx->pc = target;
        return;
    }
}

// get_clip_screen (0x1162f10): lift_d1's text (the reference)
void get_clip_screen(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t v0 = Ps2GprU32(ctx, 2), a0 = Ps2GprU32(ctx, 4), ra = Ps2GprU32(ctx, 31);
    {
        const uint32_t target = ra;
        v0 = READ32_B(a0, 592);
        Ps2SetGpr64(ctx, 2, sx64(v0));
        ctx->pc = target;
        return;
    }
}

// get_transform_matrix_set (0x1162f18): lift_d1's text (the reference)
void get_transform_matrix_set(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t v0 = Ps2GprU32(ctx, 2), a0 = Ps2GprU32(ctx, 4), ra = Ps2GprU32(ctx, 31);
    {
        const uint32_t target = ra;
        v0 = READ32_B(a0, 596);
        Ps2SetGpr64(ctx, 2, sx64(v0));
        ctx->pc = target;
        return;
    }
}

// get_change_grace_time_camera_region (0x1163808): lift_d1's text (the reference)
void get_change_grace_time_camera_region(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t v0 = Ps2GprU32(ctx, 2), ra = Ps2GprU32(ctx, 31);
    uint64_t v1 = Ps2GprU64(ctx, 3), a0 = Ps2GprU64(ctx, 4);
    float f0 = ctx->f[0];
    bool df_v0 = false;
    {
        const bool c = (int64_t)a0 == 0;
        (v1 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL));
        if (!c) {
            (v0 = (uint32_t)((int32_t)((int32_t)READ32_B((uint32_t)a0, 20)))); df_v0 = true;
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
        (v0 = (uint32_t)((int32_t)((int32_t)READ32_B((uint32_t)a0, 20)))); df_v0 = true;
        {
            const uint32_t target = ra;
            f0 = lift_d1::flt(READ32_B(v0, 24));
            ctx->f[0] = f0; if (df_v0) Ps2SetGpr64(ctx, 2, sx64(v0)); Ps2SetGpr64(ctx, 3, v1);
            ctx->pc = target;
            return;
        }
    }
    { uint32_t bits = 0u; std::memcpy(&f0, &bits, sizeof(bits)); }
    {
        const uint32_t target = ra;
        ctx->f[0] = f0; if (df_v0) Ps2SetGpr64(ctx, 2, sx64(v0)); Ps2SetGpr64(ctx, 3, v1);
        ctx->pc = target;
        return;
    }
}

// manager_sub_01164260 (0x1164260): lift_d1's text (the reference)
void manager_sub_01164260(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t v1 = Ps2GprU32(ctx, 3), ra = Ps2GprU32(ctx, 31);
    uint64_t v0 = Ps2GprU64(ctx, 2), a0 = Ps2GprU64(ctx, 4);
    bool df_v1 = false;
    {
        const bool c = (int64_t)a0 == 0;
        (v0 = (uint64_t)((uint64_t)a0 + (uint64_t)0ULL));
        if (!c) {
            (v1 = (uint32_t)((int32_t)((int32_t)READ32_B((uint32_t)a0, 20)))); df_v1 = true;
            if (v1 != 0) {
                goto L_1164278;
            }
        }
    }
    (v0 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL));
L_1164278:
    {
        const uint32_t target = ra;
        Ps2SetGpr64(ctx, 2, v0); if (df_v1) Ps2SetGpr64(ctx, 3, sx64(v1));
        ctx->pc = target;
        return;
    }
}

// set_screen_wide_and_progressive (0x1164468): lift_d1's text (the reference)
void set_screen_wide_and_progressive(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t at = Ps2GprU32(ctx, 1), v0 = Ps2GprU32(ctx, 2), ra = Ps2GprU32(ctx, 31);
    uint64_t a0 = Ps2GprU64(ctx, 4), a1 = Ps2GprU64(ctx, 5);
    float f0 = ctx->f[0];
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
        const uint32_t target = ra;
        WRITE32(v0 - 0x19Cu, lift_d1::bits(f0));
        ctx->f[0] = f0; Ps2SetGpr64(ctx, 1, sx64(at)); Ps2SetGpr64(ctx, 2, sx64(v0));
        ctx->pc = target;
        return;
    }
}

// set_view_angle (0x11644c8): lift_d1's text (the reference)
void set_view_angle(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t a0 = Ps2GprU32(ctx, 4), ra = Ps2GprU32(ctx, 31);
    float f12 = ctx->f[12];
    {
        const uint32_t target = ra;
        WRITE32_B(a0, 12, lift_d1::bits(f12));
        
        ctx->pc = target;
        return;
    }
}

// get_screen_width (0x11644f0): lift_d1's text (the reference)
void get_screen_width(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t v0 = Ps2GprU32(ctx, 2), a0 = Ps2GprU32(ctx, 4), ra = Ps2GprU32(ctx, 31);
    {
        const uint32_t target = ra;
        v0 = READ32(a0);
        Ps2SetGpr64(ctx, 2, sx64(v0));
        ctx->pc = target;
        return;
    }
}

// get_screen_height (0x11644f8): lift_d1's text (the reference)
void get_screen_height(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t v0 = Ps2GprU32(ctx, 2), a0 = Ps2GprU32(ctx, 4), ra = Ps2GprU32(ctx, 31);
    {
        const uint32_t target = ra;
        v0 = READ32_B(a0, 4);
        Ps2SetGpr64(ctx, 2, sx64(v0));
        ctx->pc = target;
        return;
    }
}

// get_aspect_ratio (0x1164500): lift_d1's text (the reference)
void get_aspect_ratio(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t v0 = Ps2GprU32(ctx, 2), v1 = Ps2GprU32(ctx, 3), a0 = Ps2GprU32(ctx, 4), ra = Ps2GprU32(ctx, 31);
    float f0 = ctx->f[0], f1 = ctx->f[1], f2 = ctx->f[2];
    uint32_t fcr31 = ctx->fcr31;
    v1 = 0x1190000u;
    f0 = lift_d1::flt(READ32_B(a0, 8));
    f2 = lift_d1::flt(READ32(v1 - 0x19Cu));
    v0 = 0x1190000u;
    f1 = lift_d1::flt(READ32(v0 - 0x198u));
    f0 = FPU_MUL_S(f0, f2);
    f0 = ps2FpuDivide(fcr31, f0, f1);
    {
        const uint32_t target = ra;
        ctx->f[0] = f0; ctx->f[1] = f1; ctx->f[2] = f2; Ps2SetGpr64(ctx, 2, sx64(v0)); Ps2SetGpr64(ctx, 3, sx64(v1)); ctx->fcr31 = fcr31;
        ctx->pc = target;
        return;
    }
}

// get_view_angle (0x1164530): lift_d1's text (the reference)
void get_view_angle(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t a0 = Ps2GprU32(ctx, 4), ra = Ps2GprU32(ctx, 31);
    float f0 = ctx->f[0];
    {
        const uint32_t target = ra;
        f0 = lift_d1::flt(READ32_B(a0, 12));
        ctx->f[0] = f0;
        ctx->pc = target;
        return;
    }
}

// get_near_clip_z_clip_screen (0x1164558): lift_d1's text (the reference)
void get_near_clip_z_clip_screen(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t at = Ps2GprU32(ctx, 1), ra = Ps2GprU32(ctx, 31);
    float f0 = ctx->f[0];
    at = 0x3D4C0000u;
    at = at | 0xCCCCu;
    f0 = lift_d1::flt(at);
    {
        const uint32_t target = ra;
        ctx->f[0] = f0; Ps2SetGpr64(ctx, 1, sx64(at));
        ctx->pc = target;
        return;
    }
}

// get_far_clip_z_clip_screen (0x1164570): lift_d1's text (the reference)
void get_far_clip_z_clip_screen(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t at = Ps2GprU32(ctx, 1), ra = Ps2GprU32(ctx, 31);
    float f0 = ctx->f[0];
    at = 0x477F0000u;
    at = at | 0xFF00u;
    f0 = lift_d1::flt(at);
    {
        const uint32_t target = ra;
        ctx->f[0] = f0; Ps2SetGpr64(ctx, 1, sx64(at));
        ctx->pc = target;
        return;
    }
}

// get_z_buf_min_clip_screen (0x1164588): lift_d1's text (the reference)
void get_z_buf_min_clip_screen(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t at = Ps2GprU32(ctx, 1), ra = Ps2GprU32(ctx, 31);
    float f0 = ctx->f[0];
    at = 0x437F0000u;
    f0 = lift_d1::flt(at);
    {
        const uint32_t target = ra;
        ctx->f[0] = f0; Ps2SetGpr64(ctx, 1, sx64(at));
        ctx->pc = target;
        return;
    }
}

// get_z_buf_max_clip_screen (0x1164598): lift_d1's text (the reference)
void get_z_buf_max_clip_screen(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t at = Ps2GprU32(ctx, 1), ra = Ps2GprU32(ctx, 31);
    float f0 = ctx->f[0];
    at = 0x4B7E0000u;
    at = at | 0xD260u;
    f0 = lift_d1::flt(at);
    {
        const uint32_t target = ra;
        ctx->f[0] = f0; Ps2SetGpr64(ctx, 1, sx64(at));
        ctx->pc = target;
        return;
    }
}

// get_scissor_clip_matrix (0x1164ca8): lift_d1's text (the reference)
void get_scissor_clip_matrix(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t v0 = Ps2GprU32(ctx, 2), a0 = Ps2GprU32(ctx, 4), a1 = Ps2GprU32(ctx, 5), ra = Ps2GprU32(ctx, 31);
    a1 = a1 << 6;
    v0 = a0 + a1;
    {
        const uint32_t target = ra;
        v0 = v0 + 0x1E0u;
        Ps2SetGpr64(ctx, 2, sx64(v0)); Ps2SetGpr64(ctx, 5, sx64(a1));
        ctx->pc = target;
        return;
    }
}

// get_WV_matrix (0x1164cb8): lift_d1's text (the reference)
void get_WV_matrix(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t ra = Ps2GprU32(ctx, 31);
    uint64_t v0 = Ps2GprU64(ctx, 2), a0 = Ps2GprU64(ctx, 4);
    {
        const uint32_t target = ra;
        (v0 = (uint64_t)((uint64_t)a0 + (uint64_t)0ULL));
        Ps2SetGpr64(ctx, 2, v0);
        ctx->pc = target;
        return;
    }
}

// ResetDamageClass (0x1166de8): lift_d1's text (the reference)
void ResetDamageClass(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t v0 = Ps2GprU32(ctx, 2), a0 = Ps2GprU32(ctx, 4), ra = Ps2GprU32(ctx, 31);
    v0 = 0xFFFFFFFFu;
    WRITE32_B(a0, 104, 0u);
    WRITE32_B(a0, 96, v0);
    WRITE32_B(a0, 84, v0);
    WRITE32_B(a0, 88, 0u);
    WRITE32(a0, 0u);
    WRITE32_B(a0, 92, 0u);
    {
        const uint32_t target = ra;
        WRITE32_B(a0, 100, 0u);
        Ps2SetGpr64(ctx, 2, sx64(v0));
        ctx->pc = target;
        return;
    }
}

// SetDamageClassDamageTableType (0x1166eb8): lift_d1's text (the reference)
void SetDamageClassDamageTableType(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t v1 = Ps2GprU32(ctx, 3), ra = Ps2GprU32(ctx, 31);
    uint64_t v0 = Ps2GprU64(ctx, 2), a0 = Ps2GprU64(ctx, 4);
    (v0 = (uint64_t)((uint64_t)a0 + (uint64_t)0ULL));
    v1 = 0x1190000u;
    {
        const uint32_t target = ra;
        WRITE32_B(v1, -392, (uint32_t)v0);
        Ps2SetGpr64(ctx, 2, v0); Ps2SetGpr64(ctx, 3, sx64(v1));
        ctx->pc = target;
        return;
    }
}

// getEditWorkInitData (0x1169c30): lift_d1's text (the reference)
void getEditWorkInitData(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t ra = Ps2GprU32(ctx, 31);
    uint64_t v0 = Ps2GprU64(ctx, 2), a0 = Ps2GprU64(ctx, 4);
    if ((int64_t)a0 != 0) {
        (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)READ32_B((uint32_t)a0, 8))));
        {
            const uint32_t target = ra;
            (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)a0, (uint32_t)v0))));
            Ps2SetGpr64(ctx, 2, v0);
            ctx->pc = target;
            return;
        }
    }
    {
        const uint32_t target = ra;
        (v0 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL));
        Ps2SetGpr64(ctx, 2, v0);
        ctx->pc = target;
        return;
    }
}

// getPointData (0x1169d48): lift_d1's text (the reference)
void getPointData(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t v1 = Ps2GprU32(ctx, 3), ra = Ps2GprU32(ctx, 31);
    uint64_t v0 = Ps2GprU64(ctx, 2), a0 = Ps2GprU64(ctx, 4), a1 = Ps2GprU64(ctx, 5);
    bool df_v1 = false;
    {
        const bool c = (int64_t)a0 == 0;
        (v0 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL));
        if (!c) {
            if ((int32_t)a1 >= 0) {
                (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)SLL32((uint32_t)a1, 1))));
                (v1 = (uint32_t)((int32_t)((int32_t)READ32_B((uint32_t)a0, 0)))); df_v1 = true;
                (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v0, (uint32_t)a1))));
                (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)SLL32((uint32_t)v0, 4))));
                (v1 = (uint32_t)(((uint64_t)a1 < (uint64_t)sx64(v1)) ? 1 : 0)); df_v1 = true;
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
        const uint32_t target = ra;
        Ps2SetGpr64(ctx, 2, v0); if (df_v1) Ps2SetGpr64(ctx, 3, sx64(v1));
        ctx->pc = target;
        return;
    }
}

// getLineData (0x1169de0): lift_d1's text (the reference)
void getLineData(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t v1 = Ps2GprU32(ctx, 3), ra = Ps2GprU32(ctx, 31);
    uint64_t v0 = Ps2GprU64(ctx, 2), a0 = Ps2GprU64(ctx, 4), a1 = Ps2GprU64(ctx, 5);
    bool df_v1 = false;
    {
        const bool c = (int64_t)a0 == 0;
        (v0 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL));
        if (!c) {
            if ((int32_t)a1 >= 0) {
                (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)SLL32((uint32_t)a1, 1))));
                (v1 = (uint32_t)((int32_t)((int32_t)READ32_B((uint32_t)a0, 0)))); df_v1 = true;
                (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v0, (uint32_t)a1))));
                (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)SLL32((uint32_t)v0, 4))));
                (v1 = (uint32_t)(((uint64_t)a1 < (uint64_t)sx64(v1)) ? 1 : 0)); df_v1 = true;
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
        const uint32_t target = ra;
        Ps2SetGpr64(ctx, 2, v0); if (df_v1) Ps2SetGpr64(ctx, 3, sx64(v1));
        ctx->pc = target;
        return;
    }
}

// getRootPointData (0x1169e68): lift_d1's text (the reference)
void getRootPointData(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t v1 = Ps2GprU32(ctx, 3), ra = Ps2GprU32(ctx, 31);
    uint64_t v0 = Ps2GprU64(ctx, 2), a0 = Ps2GprU64(ctx, 4), a1 = Ps2GprU64(ctx, 5);
    bool df_v1 = false;
    {
        const bool c = (int64_t)a0 == 0;
        (v0 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL));
        if (!c) {
            if ((int32_t)a1 >= 0) {
                (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)SLL32((uint32_t)a1, 1))));
                (v1 = (uint32_t)((int32_t)((int32_t)READ32_B((uint32_t)a0, 0)))); df_v1 = true;
                (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v0, (uint32_t)a1))));
                (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)SLL32((uint32_t)v0, 4))));
                (v1 = (uint32_t)(((uint64_t)a1 < (uint64_t)sx64(v1)) ? 1 : 0)); df_v1 = true;
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
        const uint32_t target = ra;
        Ps2SetGpr64(ctx, 2, v0); if (df_v1) Ps2SetGpr64(ctx, 3, sx64(v1));
        ctx->pc = target;
        return;
    }
}

// getGoalPointData (0x1169ea8): lift_d1's text (the reference)
void getGoalPointData(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t v1 = Ps2GprU32(ctx, 3), ra = Ps2GprU32(ctx, 31);
    uint64_t v0 = Ps2GprU64(ctx, 2), a0 = Ps2GprU64(ctx, 4), a1 = Ps2GprU64(ctx, 5);
    bool df_v1 = false;
    {
        const bool c = (int64_t)a0 == 0;
        (v0 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL));
        if (!c) {
            if ((int32_t)a1 >= 0) {
                (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)SLL32((uint32_t)a1, 1))));
                (v1 = (uint32_t)((int32_t)((int32_t)READ32_B((uint32_t)a0, 0)))); df_v1 = true;
                (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v0, (uint32_t)a1))));
                (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)SLL32((uint32_t)v0, 4))));
                (v1 = (uint32_t)(((uint64_t)a1 < (uint64_t)sx64(v1)) ? 1 : 0)); df_v1 = true;
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
        const uint32_t target = ra;
        Ps2SetGpr64(ctx, 2, v0); if (df_v1) Ps2SetGpr64(ctx, 3, sx64(v1));
        ctx->pc = target;
        return;
    }
}

// getSubPointData (0x1169ee8): lift_d1's text (the reference)
void getSubPointData(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t v1 = Ps2GprU32(ctx, 3), ra = Ps2GprU32(ctx, 31);
    uint64_t v0 = Ps2GprU64(ctx, 2), a0 = Ps2GprU64(ctx, 4), a1 = Ps2GprU64(ctx, 5);
    bool df_v1 = false;
    {
        const bool c = (int64_t)a0 == 0;
        (v0 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL));
        if (!c) {
            if ((int32_t)a1 >= 0) {
                (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)SLL32((uint32_t)a1, 1))));
                (v1 = (uint32_t)((int32_t)((int32_t)READ32_B((uint32_t)a0, 0)))); df_v1 = true;
                (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v0, (uint32_t)a1))));
                (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)SLL32((uint32_t)v0, 4))));
                (v1 = (uint32_t)(((uint64_t)a1 < (uint64_t)sx64(v1)) ? 1 : 0)); df_v1 = true;
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
        const uint32_t target = ra;
        Ps2SetGpr64(ctx, 2, v0); if (df_v1) Ps2SetGpr64(ctx, 3, sx64(v1));
        ctx->pc = target;
        return;
    }
}

// getInterestPointData (0x1169ff0): lift_d1's text (the reference)
void getInterestPointData(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t v1 = Ps2GprU32(ctx, 3), ra = Ps2GprU32(ctx, 31);
    uint64_t v0 = Ps2GprU64(ctx, 2), a0 = Ps2GprU64(ctx, 4), a1 = Ps2GprU64(ctx, 5);
    bool df_v1 = false;
    {
        const bool c = (int64_t)a0 == 0;
        (v0 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL));
        if (!c) {
            if ((int32_t)a1 >= 0) {
                (v1 = (uint32_t)((int32_t)((int32_t)READ32_B((uint32_t)a0, 0)))); df_v1 = true;
                (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)SLL32((uint32_t)a1, 5))));
                (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)a0, (uint32_t)v0))));
                (v1 = (uint32_t)(((uint64_t)a1 < (uint64_t)sx64(v1)) ? 1 : 0)); df_v1 = true;
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
        const uint32_t target = ra;
        Ps2SetGpr64(ctx, 2, v0); if (df_v1) Ps2SetGpr64(ctx, 3, sx64(v1));
        ctx->pc = target;
        return;
    }
}

// getDeadPointData (0x116a040): lift_d1's text (the reference)
void getDeadPointData(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t v1 = Ps2GprU32(ctx, 3), ra = Ps2GprU32(ctx, 31);
    uint64_t v0 = Ps2GprU64(ctx, 2), a0 = Ps2GprU64(ctx, 4), a1 = Ps2GprU64(ctx, 5);
    bool df_v1 = false;
    {
        const bool c = (int64_t)a0 == 0;
        (v0 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL));
        if (!c) {
            if ((int32_t)a1 >= 0) {
                (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)SLL32((uint32_t)a1, 1))));
                (v1 = (uint32_t)((int32_t)((int32_t)READ32_B((uint32_t)a0, 0)))); df_v1 = true;
                (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v0, (uint32_t)a1))));
                (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)SLL32((uint32_t)v0, 4))));
                (v1 = (uint32_t)(((uint64_t)a1 < (uint64_t)sx64(v1)) ? 1 : 0)); df_v1 = true;
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
        const uint32_t target = ra;
        Ps2SetGpr64(ctx, 2, v0); if (df_v1) Ps2SetGpr64(ctx, 3, sx64(v1));
        ctx->pc = target;
        return;
    }
}

// getLayoutPointData (0x116a098): lift_d1's text (the reference)
void getLayoutPointData(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t v1 = Ps2GprU32(ctx, 3), ra = Ps2GprU32(ctx, 31);
    uint64_t v0 = Ps2GprU64(ctx, 2), a0 = Ps2GprU64(ctx, 4), a1 = Ps2GprU64(ctx, 5);
    bool df_v1 = false;
    {
        const bool c = (int64_t)a0 == 0;
        (v0 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL));
        if (!c) {
            if ((int32_t)a1 >= 0) {
                (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)SLL32((uint32_t)a1, 1))));
                (v1 = (uint32_t)((int32_t)((int32_t)READ32_B((uint32_t)a0, 0)))); df_v1 = true;
                (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v0, (uint32_t)a1))));
                (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)SLL32((uint32_t)v0, 4))));
                (v1 = (uint32_t)(((uint64_t)a1 < (uint64_t)sx64(v1)) ? 1 : 0)); df_v1 = true;
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
        const uint32_t target = ra;
        Ps2SetGpr64(ctx, 2, v0); if (df_v1) Ps2SetGpr64(ctx, 3, sx64(v1));
        ctx->pc = target;
        return;
    }
}

// getSnapShotFormat (0x1341920): lift_d1's text (the reference)
void getSnapShotFormat(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t v0 = Ps2GprU32(ctx, 2), ra = Ps2GprU32(ctx, 31);
    v0 = 0x1460000u;
    {
        const uint32_t target = ra;
        v0 = v0 + 0x544Cu;
        Ps2SetGpr64(ctx, 2, sx64(v0));
        ctx->pc = target;
        return;
    }
}

// GAMEUTIL_get_sight_on_reverse_cntrlLR_bool (0x1342b40): lift_d1's text (the reference)
void GAMEUTIL_get_sight_on_reverse_cntrlLR_bool(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t v0 = Ps2GprU32(ctx, 2), v1 = Ps2GprU32(ctx, 3), ra = Ps2GprU32(ctx, 31);
    v1 = 0x1460000u;
    v0 = READ32_B(v1, 22032);
    {
        const uint32_t target = ra;
        v0 = 0u < v0 ? 1u : 0u;
        Ps2SetGpr64(ctx, 2, sx64(v0)); Ps2SetGpr64(ctx, 3, sx64(v1));
        ctx->pc = target;
        return;
    }
}

// GAMEUTIL_get_sight_on_reverse_cntrlUD_bool (0x1342b70): lift_d1's text (the reference)
void GAMEUTIL_get_sight_on_reverse_cntrlUD_bool(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t v0 = Ps2GprU32(ctx, 2), v1 = Ps2GprU32(ctx, 3), ra = Ps2GprU32(ctx, 31);
    v1 = 0x1460000u;
    v0 = READ32_B(v1, 22036);
    {
        const uint32_t target = ra;
        v0 = 0u < v0 ? 1u : 0u;
        Ps2SetGpr64(ctx, 2, sx64(v0)); Ps2SetGpr64(ctx, 3, sx64(v1));
        ctx->pc = target;
        return;
    }
}

// GAMEUTIL_get_game_camera_free_mode_reverse_cntrlLR_bool (0x1342c70): lift_d1's text (the reference)
void GAMEUTIL_get_game_camera_free_mode_reverse_cntrlLR_bool(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t v0 = Ps2GprU32(ctx, 2), v1 = Ps2GprU32(ctx, 3), ra = Ps2GprU32(ctx, 31);
    v1 = 0x1460000u;
    v0 = READ32_B(v1, 22044);
    {
        const uint32_t target = ra;
        v0 = 0u < v0 ? 1u : 0u;
        Ps2SetGpr64(ctx, 2, sx64(v0)); Ps2SetGpr64(ctx, 3, sx64(v1));
        ctx->pc = target;
        return;
    }
}

// GAMEUTIL_get_game_camera_free_mode_reverse_cntrlUD_bool (0x1342ca0): lift_d1's text (the reference)
void GAMEUTIL_get_game_camera_free_mode_reverse_cntrlUD_bool(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t v0 = Ps2GprU32(ctx, 2), v1 = Ps2GprU32(ctx, 3), ra = Ps2GprU32(ctx, 31);
    v1 = 0x1460000u;
    v0 = READ32_B(v1, 22048);
    {
        const uint32_t target = ra;
        v0 = 0u < v0 ? 1u : 0u;
        Ps2SetGpr64(ctx, 2, sx64(v0)); Ps2SetGpr64(ctx, 3, sx64(v1));
        ctx->pc = target;
        return;
    }
}

// GAMEUTIL_set_auto_get_flag_boss_interest_position (0x1343b28): lift_d1's text (the reference)
void GAMEUTIL_set_auto_get_flag_boss_interest_position(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t v0 = Ps2GprU32(ctx, 2), a0 = Ps2GprU32(ctx, 4), ra = Ps2GprU32(ctx, 31);
    v0 = 0x1460000u;
    {
        const uint32_t target = ra;
        WRITE32_B(v0, 22056, a0);
        Ps2SetGpr64(ctx, 2, sx64(v0));
        ctx->pc = target;
        return;
    }
}

// GAMEUTIL_reset_move_angle (0x1343cd0): lift_d1's text (the reference)
void GAMEUTIL_reset_move_angle(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t a0 = Ps2GprU32(ctx, 4), ra = Ps2GprU32(ctx, 31);
    {
        const uint32_t target = ra;
        WRITE32(a0, 0u);
        
        ctx->pc = target;
        return;
    }
}

// GAMEUTIL_set_parameter_move_angle (0x1343cd8): lift_d1's text (the reference)
void GAMEUTIL_set_parameter_move_angle(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t at = Ps2GprU32(ctx, 1), a0 = Ps2GprU32(ctx, 4), ra = Ps2GprU32(ctx, 31);
    float f0 = ctx->f[0], f12 = ctx->f[12], f13 = ctx->f[13], f14 = ctx->f[14], f15 = ctx->f[15], f16 = ctx->f[16], f17 = ctx->f[17];
    at = 0x3C8E0000u;
    at = at | 0xFA35u;
    f0 = lift_d1::flt(at);
    WRITE32_B(a0, 16, lift_d1::bits(f15));
    f17 = FPU_MUL_S(f17, f0);
    f12 = FPU_MUL_S(f12, f0);
    f13 = FPU_MUL_S(f13, f0);
    f14 = FPU_MUL_S(f14, f0);
    f16 = FPU_MUL_S(f16, f0);
    WRITE32_B(a0, 24, lift_d1::bits(f17));
    WRITE32_B(a0, 4, lift_d1::bits(f12));
    WRITE32_B(a0, 8, lift_d1::bits(f13));
    WRITE32_B(a0, 12, lift_d1::bits(f14));
    {
        const uint32_t target = ra;
        WRITE32_B(a0, 20, lift_d1::bits(f16));
        ctx->f[0] = f0; ctx->f[12] = f12; ctx->f[13] = f13; ctx->f[14] = f14; ctx->f[16] = f16; ctx->f[17] = f17; Ps2SetGpr64(ctx, 1, sx64(at));
        ctx->pc = target;
        return;
    }
}

// GAMEUTIL_begin_move_angle (0x1343d18): lift_d1's text (the reference)
void GAMEUTIL_begin_move_angle(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t a0 = Ps2GprU32(ctx, 4), ra = Ps2GprU32(ctx, 31);
    {
        const uint32_t target = ra;
        WRITE32(a0, 0u);
        
        ctx->pc = target;
        return;
    }
}

// GAMEUTIL_get_cur_velocity_move_angle (0x1343e10): lift_d1's text (the reference)
void GAMEUTIL_get_cur_velocity_move_angle(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t a0 = Ps2GprU32(ctx, 4), ra = Ps2GprU32(ctx, 31);
    float f0 = ctx->f[0];
    {
        const uint32_t target = ra;
        f0 = lift_d1::flt(READ32(a0));
        ctx->f[0] = f0;
        ctx->pc = target;
        return;
    }
}

// GAMEUTIL_set_need_dir_ratio_flag_look_at_vs_offset (0x1343ee8): lift_d1's text (the reference)
void GAMEUTIL_set_need_dir_ratio_flag_look_at_vs_offset(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t a0 = Ps2GprU32(ctx, 4), a1 = Ps2GprU32(ctx, 5), ra = Ps2GprU32(ctx, 31);
    {
        const uint32_t target = ra;
        WRITE32_B(a0, 24, a1);
        
        ctx->pc = target;
        return;
    }
}

// GAMEUTIL_set_enable_side_change_angle_look_at_vs_offset (0x1343ef0): lift_d1's text (the reference)
void GAMEUTIL_set_enable_side_change_angle_look_at_vs_offset(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t a0 = Ps2GprU32(ctx, 4), ra = Ps2GprU32(ctx, 31);
    float f12 = ctx->f[12];
    {
        const uint32_t target = ra;
        WRITE32_B(a0, 80, lift_d1::bits(f12));
        
        ctx->pc = target;
        return;
    }
}

// GAMEUTIL_set_parameter_col_plane_info_filter (0x13440a8): lift_d1's text (the reference)
void GAMEUTIL_set_parameter_col_plane_info_filter(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t a0 = Ps2GprU32(ctx, 4), ra = Ps2GprU32(ctx, 31);
    float f12 = ctx->f[12], f13 = ctx->f[13];
    WRITE32_B(a0, 116, lift_d1::bits(f13));
    {
        const uint32_t target = ra;
        WRITE32_B(a0, 112, lift_d1::bits(f12));
        
        ctx->pc = target;
        return;
    }
}

// GAMEUTIL_set_parameter_position_filter (0x1344208): lift_d1's text (the reference)
void GAMEUTIL_set_parameter_position_filter(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t a0 = Ps2GprU32(ctx, 4), ra = Ps2GprU32(ctx, 31);
    float f12 = ctx->f[12];
    {
        const uint32_t target = ra;
        WRITE32_B(a0, 36, lift_d1::bits(f12));
        
        ctx->pc = target;
        return;
    }
}

// GAMEUTIL_reset_status_manager (0x13445c0): lift_d1's text (the reference)
void GAMEUTIL_reset_status_manager(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t a0 = Ps2GprU32(ctx, 4), a1 = Ps2GprU32(ctx, 5), ra = Ps2GprU32(ctx, 31);
    WRITE32(a0, a1);
    WRITE32_B(a0, 12, 0u);
    WRITE32_B(a0, 8, a1);
    {
        const uint32_t target = ra;
        WRITE32_B(a0, 4, a1);
        
        ctx->pc = target;
        return;
    }
}

// GAMEUTIL_is_status_change_status_manager (0x1344628): lift_d1's text (the reference)
void GAMEUTIL_is_status_change_status_manager(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t v0 = Ps2GprU32(ctx, 2), a0 = Ps2GprU32(ctx, 4), ra = Ps2GprU32(ctx, 31);
    float f0 = ctx->f[0], f1 = ctx->f[1];
    uint32_t fcr31 = ctx->fcr31;
    f1 = lift_d1::flt(READ32_B(a0, 12));
    f0 = lift_d1::flt(0u);
    fcr31 = (FPU_C_OLE_S(f1, f0)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
    {
        const bool c = (fcr31 & 0x800000u) != 0u;
        v0 = 1u;
        if (!c) {
            v0 = 0u;
        }
    }
    {
        const uint32_t target = ra;
        ctx->f[0] = f0; ctx->f[1] = f1; Ps2SetGpr64(ctx, 2, sx64(v0)); ctx->fcr31 = fcr31;
        ctx->pc = target;
        return;
    }
}

// GAMEUTIL_get_status_continue_time_status_manager (0x1344648): lift_d1's text (the reference)
void GAMEUTIL_get_status_continue_time_status_manager(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t a0 = Ps2GprU32(ctx, 4), ra = Ps2GprU32(ctx, 31);
    float f0 = ctx->f[0];
    {
        const uint32_t target = ra;
        f0 = lift_d1::flt(READ32_B(a0, 12));
        ctx->f[0] = f0;
        ctx->pc = target;
        return;
    }
}

// GAMEUTIL_get_cur_status_status_manager (0x1344650): lift_d1's text (the reference)
void GAMEUTIL_get_cur_status_status_manager(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t v0 = Ps2GprU32(ctx, 2), a0 = Ps2GprU32(ctx, 4), ra = Ps2GprU32(ctx, 31);
    {
        const uint32_t target = ra;
        v0 = READ32(a0);
        Ps2SetGpr64(ctx, 2, sx64(v0));
        ctx->pc = target;
        return;
    }
}

// GAMEUTIL_get_pre_status_status_manager (0x1344658): lift_d1's text (the reference)
void GAMEUTIL_get_pre_status_status_manager(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t v0 = Ps2GprU32(ctx, 2), a0 = Ps2GprU32(ctx, 4), ra = Ps2GprU32(ctx, 31);
    {
        const uint32_t target = ra;
        v0 = READ32_B(a0, 4);
        Ps2SetGpr64(ctx, 2, sx64(v0));
        ctx->pc = target;
        return;
    }
}

// GAMEUTIL_get_pre_changed_status_status_manager (0x1344660): lift_d1's text (the reference)
void GAMEUTIL_get_pre_changed_status_status_manager(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t v0 = Ps2GprU32(ctx, 2), a0 = Ps2GprU32(ctx, 4), ra = Ps2GprU32(ctx, 31);
    {
        const uint32_t target = ra;
        v0 = READ32_B(a0, 8);
        Ps2SetGpr64(ctx, 2, sx64(v0));
        ctx->pc = target;
        return;
    }
}

// getSightOnRevCntrlLR (0x1344668): lift_d1's text (the reference)
void getSightOnRevCntrlLR(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t v0 = Ps2GprU32(ctx, 2), ra = Ps2GprU32(ctx, 31);
    v0 = 0x1460000u;
    {
        const uint32_t target = ra;
        v0 = v0 + 0x5610u;
        Ps2SetGpr64(ctx, 2, sx64(v0));
        ctx->pc = target;
        return;
    }
}

// getSightOnRevCntrlUD (0x1344678): lift_d1's text (the reference)
void getSightOnRevCntrlUD(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t v0 = Ps2GprU32(ctx, 2), ra = Ps2GprU32(ctx, 31);
    v0 = 0x1460000u;
    {
        const uint32_t target = ra;
        v0 = v0 + 0x5614u;
        Ps2SetGpr64(ctx, 2, sx64(v0));
        ctx->pc = target;
        return;
    }
}

// getFreeModeRevControlLR (0x1344698): lift_d1's text (the reference)
void getFreeModeRevControlLR(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t v0 = Ps2GprU32(ctx, 2), ra = Ps2GprU32(ctx, 31);
    v0 = 0x1460000u;
    {
        const uint32_t target = ra;
        v0 = v0 + 0x561Cu;
        Ps2SetGpr64(ctx, 2, sx64(v0));
        ctx->pc = target;
        return;
    }
}

// getFreeModeRevControlUD (0x13446a8): lift_d1's text (the reference)
void getFreeModeRevControlUD(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t v0 = Ps2GprU32(ctx, 2), ra = Ps2GprU32(ctx, 31);
    v0 = 0x1460000u;
    {
        const uint32_t target = ra;
        v0 = v0 + 0x5620u;
        Ps2SetGpr64(ctx, 2, sx64(v0));
        ctx->pc = target;
        return;
    }
}

// getFreeModeRevAngleVelRatio (0x13446b8): lift_d1's text (the reference)
void getFreeModeRevAngleVelRatio(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t v0 = Ps2GprU32(ctx, 2), ra = Ps2GprU32(ctx, 31);
    v0 = 0x1460000u;
    {
        const uint32_t target = ra;
        v0 = v0 + 0x5624u;
        Ps2SetGpr64(ctx, 2, sx64(v0));
        ctx->pc = target;
        return;
    }
}

// GetGameTimeScaleRatio (0x1345d40): lift_d1's text (the reference)
void GetGameTimeScaleRatio(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t v0 = Ps2GprU32(ctx, 2), ra = Ps2GprU32(ctx, 31);
    float f0 = ctx->f[0];
    v0 = 0x1460000u;
    {
        const uint32_t target = ra;
        f0 = lift_d1::flt(READ32_B(v0, 22508));
        ctx->f[0] = f0; Ps2SetGpr64(ctx, 2, sx64(v0));
        ctx->pc = target;
        return;
    }
}

// GetCurrentGameAnimeTimeScaleRatio (0x1345e38): lift_d1's text (the reference)
void GetCurrentGameAnimeTimeScaleRatio(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t v0 = Ps2GprU32(ctx, 2), ra = Ps2GprU32(ctx, 31);
    float f0 = ctx->f[0];
    v0 = 0x1460000u;
    {
        const uint32_t target = ra;
        f0 = lift_d1::flt(READ32_B(v0, 22512));
        ctx->f[0] = f0; Ps2SetGpr64(ctx, 2, sx64(v0));
        ctx->pc = target;
        return;
    }
}

// GetGameAnimeTimeScaleRatio (0x1345e48): lift_d1's text (the reference)
void GetGameAnimeTimeScaleRatio(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t v0 = Ps2GprU32(ctx, 2), ra = Ps2GprU32(ctx, 31);
    float f0 = ctx->f[0];
    v0 = 0x1460000u;
    {
        const uint32_t target = ra;
        f0 = lift_d1::flt(READ32_B(v0, 23012));
        ctx->f[0] = f0; Ps2SetGpr64(ctx, 2, sx64(v0));
        ctx->pc = target;
        return;
    }
}

// GetPlayerControlButtonContext (0x1345e58): lift_d1's text (the reference)
void GetPlayerControlButtonContext(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t v0 = Ps2GprU32(ctx, 2), ra = Ps2GprU32(ctx, 31);
    v0 = 0x1460000u;
    {
        const uint32_t target = ra;
        v0 = v0 + 0x5964u;
        Ps2SetGpr64(ctx, 2, sx64(v0));
        ctx->pc = target;
        return;
    }
}

// RestoreDefaultGameAnimeTimeScaleRatio (0x1345e78): lift_d1's text (the reference)
void RestoreDefaultGameAnimeTimeScaleRatio(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t at = Ps2GprU32(ctx, 1), v0 = Ps2GprU32(ctx, 2), ra = Ps2GprU32(ctx, 31);
    float f0 = ctx->f[0];
    at = 0x3F660000u;
    at = at | 0x6666u;
    f0 = lift_d1::flt(at);
    v0 = 0x1460000u;
    {
        const uint32_t target = ra;
        WRITE32_B(v0, 23012, lift_d1::bits(f0));
        ctx->f[0] = f0; Ps2SetGpr64(ctx, 1, sx64(at)); Ps2SetGpr64(ctx, 2, sx64(v0));
        ctx->pc = target;
        return;
    }
}

// SetDefaultGameAnimeTimeScaleRatio (0x1345e90): lift_d1's text (the reference)
void SetDefaultGameAnimeTimeScaleRatio(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t v0 = Ps2GprU32(ctx, 2), ra = Ps2GprU32(ctx, 31);
    float f12 = ctx->f[12];
    v0 = 0x1460000u;
    {
        const uint32_t target = ra;
        WRITE32_B(v0, 23012, lift_d1::bits(f12));
        Ps2SetGpr64(ctx, 2, sx64(v0));
        ctx->pc = target;
        return;
    }
}

// getFrameRateMode (0x1345ed0): lift_d1's text (the reference)
void getFrameRateMode(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t v0 = Ps2GprU32(ctx, 2), ra = Ps2GprU32(ctx, 31);
    v0 = 0x1460000u;
    {
        const uint32_t target = ra;
        v0 = v0 + 0x59E8u;
        Ps2SetGpr64(ctx, 2, sx64(v0));
        ctx->pc = target;
        return;
    }
}

// setDefaultPlayerControlButton (0x1346028): lift_d1's text (the reference)
void setDefaultPlayerControlButton(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint64_t v0 = Ps2GprU64(ctx, 2), v1 = Ps2GprU64(ctx, 3), a0 = Ps2GprU64(ctx, 4), a1 = Ps2GprU64(ctx, 5), a2 = Ps2GprU64(ctx, 6), a3 = Ps2GprU64(ctx, 7), ra = Ps2GprU64(ctx, 31);
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)READ32_B((uint32_t)a0, 16))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)326 << 16))));
    (a3 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v0, 22884))));
    { uint32_t addr = ADD32((uint32_t)v1, 7); uint32_t aligned_addr = addr & ~7u; uint32_t offset = addr & 7u; uint64_t mem = READ64(aligned_addr); uint32_t shift = (7u - offset) << 3; uint64_t keepMask = (shift == 0) ? 0ull : ((1ull << shift) - 1ull); (a0 = (uint64_t)((a0 & keepMask) | (mem << shift))); }
    { uint32_t addr = ADD32((uint32_t)v1, 0); uint32_t aligned_addr = addr & ~7u; uint32_t offset = addr & 7u; uint64_t mem = READ64(aligned_addr); uint32_t shift = offset << 3; uint64_t keepMask = (offset == 0) ? 0ull : (0xFFFFFFFFFFFFFFFFull << ((8u - offset) << 3)); (a0 = (uint64_t)((a0 & keepMask) | (mem >> shift))); }
    { uint32_t addr = ADD32((uint32_t)v1, 15); uint32_t aligned_addr = addr & ~7u; uint32_t offset = addr & 7u; uint64_t mem = READ64(aligned_addr); uint32_t shift = (7u - offset) << 3; uint64_t keepMask = (shift == 0) ? 0ull : ((1ull << shift) - 1ull); (a1 = (uint64_t)((a1 & keepMask) | (mem << shift))); }
    { uint32_t addr = ADD32((uint32_t)v1, 8); uint32_t aligned_addr = addr & ~7u; uint32_t offset = addr & 7u; uint64_t mem = READ64(aligned_addr); uint32_t shift = offset << 3; uint64_t keepMask = (offset == 0) ? 0ull : (0xFFFFFFFFFFFFFFFFull << ((8u - offset) << 3)); (a1 = (uint64_t)((a1 & keepMask) | (mem >> shift))); }
    { uint32_t addr = ADD32((uint32_t)v1, 23); uint32_t aligned_addr = addr & ~7u; uint32_t offset = addr & 7u; uint64_t mem = READ64(aligned_addr); uint32_t shift = (7u - offset) << 3; uint64_t keepMask = (shift == 0) ? 0ull : ((1ull << shift) - 1ull); (a2 = (uint64_t)((a2 & keepMask) | (mem << shift))); }
    { uint32_t addr = ADD32((uint32_t)v1, 16); uint32_t aligned_addr = addr & ~7u; uint32_t offset = addr & 7u; uint64_t mem = READ64(aligned_addr); uint32_t shift = offset << 3; uint64_t keepMask = (offset == 0) ? 0ull : (0xFFFFFFFFFFFFFFFFull << ((8u - offset) << 3)); (a2 = (uint64_t)((a2 & keepMask) | (mem >> shift))); }
    { uint32_t addr = ADD32((uint32_t)a3, 7); uint32_t aligned_addr = addr & ~7u; uint32_t offset = addr & 7u; uint32_t shift = (7u - offset) << 3; uint64_t mask = 0xFFFFFFFFFFFFFFFFull >> shift; uint64_t old_data = READ64(aligned_addr); uint64_t val = a0; uint64_t new_data = (old_data & ~mask) | ((val >> shift) & mask); WRITE64(aligned_addr, new_data); }
    { uint32_t addr = ADD32((uint32_t)a3, 0); uint32_t aligned_addr = addr & ~7u; uint32_t offset = addr & 7u; uint32_t shift = offset << 3; uint64_t mask = 0xFFFFFFFFFFFFFFFFull << shift; uint64_t old_data = READ64(aligned_addr); uint64_t val = a0; uint64_t new_data = (old_data & ~mask) | ((val << shift) & mask); WRITE64(aligned_addr, new_data); }
    { uint32_t addr = ADD32((uint32_t)a3, 15); uint32_t aligned_addr = addr & ~7u; uint32_t offset = addr & 7u; uint32_t shift = (7u - offset) << 3; uint64_t mask = 0xFFFFFFFFFFFFFFFFull >> shift; uint64_t old_data = READ64(aligned_addr); uint64_t val = a1; uint64_t new_data = (old_data & ~mask) | ((val >> shift) & mask); WRITE64(aligned_addr, new_data); }
    { uint32_t addr = ADD32((uint32_t)a3, 8); uint32_t aligned_addr = addr & ~7u; uint32_t offset = addr & 7u; uint32_t shift = offset << 3; uint64_t mask = 0xFFFFFFFFFFFFFFFFull << shift; uint64_t old_data = READ64(aligned_addr); uint64_t val = a1; uint64_t new_data = (old_data & ~mask) | ((val << shift) & mask); WRITE64(aligned_addr, new_data); }
    { uint32_t addr = ADD32((uint32_t)a3, 23); uint32_t aligned_addr = addr & ~7u; uint32_t offset = addr & 7u; uint32_t shift = (7u - offset) << 3; uint64_t mask = 0xFFFFFFFFFFFFFFFFull >> shift; uint64_t old_data = READ64(aligned_addr); uint64_t val = a2; uint64_t new_data = (old_data & ~mask) | ((val >> shift) & mask); WRITE64(aligned_addr, new_data); }
    { uint32_t addr = ADD32((uint32_t)a3, 16); uint32_t aligned_addr = addr & ~7u; uint32_t offset = addr & 7u; uint32_t shift = offset << 3; uint64_t mask = 0xFFFFFFFFFFFFFFFFull << shift; uint64_t old_data = READ64(aligned_addr); uint64_t val = a2; uint64_t new_data = (old_data & ~mask) | ((val << shift) & mask); WRITE64(aligned_addr, new_data); }
    { uint32_t addr = ADD32((uint32_t)v1, 31); uint32_t aligned_addr = addr & ~7u; uint32_t offset = addr & 7u; uint64_t mem = READ64(aligned_addr); uint32_t shift = (7u - offset) << 3; uint64_t keepMask = (shift == 0) ? 0ull : ((1ull << shift) - 1ull); (a0 = (uint64_t)((a0 & keepMask) | (mem << shift))); }
    { uint32_t addr = ADD32((uint32_t)v1, 24); uint32_t aligned_addr = addr & ~7u; uint32_t offset = addr & 7u; uint64_t mem = READ64(aligned_addr); uint32_t shift = offset << 3; uint64_t keepMask = (offset == 0) ? 0ull : (0xFFFFFFFFFFFFFFFFull << ((8u - offset) << 3)); (a0 = (uint64_t)((a0 & keepMask) | (mem >> shift))); }
    { uint32_t addr = ADD32((uint32_t)v1, 39); uint32_t aligned_addr = addr & ~7u; uint32_t offset = addr & 7u; uint64_t mem = READ64(aligned_addr); uint32_t shift = (7u - offset) << 3; uint64_t keepMask = (shift == 0) ? 0ull : ((1ull << shift) - 1ull); (a1 = (uint64_t)((a1 & keepMask) | (mem << shift))); }
    { uint32_t addr = ADD32((uint32_t)v1, 32); uint32_t aligned_addr = addr & ~7u; uint32_t offset = addr & 7u; uint64_t mem = READ64(aligned_addr); uint32_t shift = offset << 3; uint64_t keepMask = (offset == 0) ? 0ull : (0xFFFFFFFFFFFFFFFFull << ((8u - offset) << 3)); (a1 = (uint64_t)((a1 & keepMask) | (mem >> shift))); }
    { uint32_t addr = ADD32((uint32_t)a3, 31); uint32_t aligned_addr = addr & ~7u; uint32_t offset = addr & 7u; uint32_t shift = (7u - offset) << 3; uint64_t mask = 0xFFFFFFFFFFFFFFFFull >> shift; uint64_t old_data = READ64(aligned_addr); uint64_t val = a0; uint64_t new_data = (old_data & ~mask) | ((val >> shift) & mask); WRITE64(aligned_addr, new_data); }
    { uint32_t addr = ADD32((uint32_t)a3, 24); uint32_t aligned_addr = addr & ~7u; uint32_t offset = addr & 7u; uint32_t shift = offset << 3; uint64_t mask = 0xFFFFFFFFFFFFFFFFull << shift; uint64_t old_data = READ64(aligned_addr); uint64_t val = a0; uint64_t new_data = (old_data & ~mask) | ((val << shift) & mask); WRITE64(aligned_addr, new_data); }
    { uint32_t addr = ADD32((uint32_t)a3, 39); uint32_t aligned_addr = addr & ~7u; uint32_t offset = addr & 7u; uint32_t shift = (7u - offset) << 3; uint64_t mask = 0xFFFFFFFFFFFFFFFFull >> shift; uint64_t old_data = READ64(aligned_addr); uint64_t val = a1; uint64_t new_data = (old_data & ~mask) | ((val >> shift) & mask); WRITE64(aligned_addr, new_data); }
    { uint32_t addr = ADD32((uint32_t)a3, 32); uint32_t aligned_addr = addr & ~7u; uint32_t offset = addr & 7u; uint32_t shift = offset << 3; uint64_t mask = 0xFFFFFFFFFFFFFFFFull << shift; uint64_t old_data = READ64(aligned_addr); uint64_t val = a1; uint64_t new_data = (old_data & ~mask) | ((val << shift) & mask); WRITE64(aligned_addr, new_data); }
    {
        const uint32_t target = (uint32_t)ra;
        Ps2SetGpr64(ctx, 2, v0); Ps2SetGpr64(ctx, 3, v1); Ps2SetGpr64(ctx, 4, a0); Ps2SetGpr64(ctx, 5, a1); Ps2SetGpr64(ctx, 6, a2); Ps2SetGpr64(ctx, 7, a3);
        ctx->pc = target;
        return;
    }
}

// getFadeValue (0x1346210): lift_d1's text (the reference)
void getFadeValue(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t v0 = Ps2GprU32(ctx, 2), ra = Ps2GprU32(ctx, 31);
    float f0 = ctx->f[0];
    v0 = 0x1460000u;
    {
        const uint32_t target = ra;
        f0 = lift_d1::flt(READ32_B(v0, 23120));
        ctx->f[0] = f0; Ps2SetGpr64(ctx, 2, sx64(v0));
        ctx->pc = target;
        return;
    }
}

// set_yaw_pitch_input_border_parameter_nico_yaw_pitch_cntrl (0x1346df0): lift_d1's text (the reference)
void set_yaw_pitch_input_border_parameter_nico_yaw_pitch_cntrl(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t a0 = Ps2GprU32(ctx, 4), ra = Ps2GprU32(ctx, 31);
    float f12 = ctx->f[12], f13 = ctx->f[13], f14 = ctx->f[14], f15 = ctx->f[15];
    WRITE32_B(a0, 120, lift_d1::bits(f15));
    WRITE32_B(a0, 108, lift_d1::bits(f12));
    WRITE32_B(a0, 112, lift_d1::bits(f13));
    {
        const uint32_t target = ra;
        WRITE32_B(a0, 116, lift_d1::bits(f14));
        
        ctx->pc = target;
        return;
    }
}

// set_base_yaw_parameter_nico_yaw_pitch_cntrl (0x1346fe8): lift_d1's text (the reference)
void set_base_yaw_parameter_nico_yaw_pitch_cntrl(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t at = Ps2GprU32(ctx, 1), a0 = Ps2GprU32(ctx, 4), a1 = Ps2GprU32(ctx, 5), ra = Ps2GprU32(ctx, 31);
    float f0 = ctx->f[0], f12 = ctx->f[12], f13 = ctx->f[13], f14 = ctx->f[14], f15 = ctx->f[15], f16 = ctx->f[16], f17 = ctx->f[17];
    at = 0x3C8E0000u;
    at = at | 0xFA35u;
    f0 = lift_d1::flt(at);
    WRITE32_B(a0, 228, lift_d1::bits(f17));
    f16 = FPU_MUL_S(f16, f0);
    WRITE32_B(a0, 204, a1);
    f12 = FPU_MUL_S(f12, f0);
    f13 = FPU_MUL_S(f13, f0);
    f14 = FPU_MUL_S(f14, f0);
    f15 = FPU_MUL_S(f15, f0);
    WRITE32_B(a0, 224, lift_d1::bits(f16));
    WRITE32_B(a0, 208, lift_d1::bits(f12));
    WRITE32_B(a0, 212, lift_d1::bits(f13));
    WRITE32_B(a0, 216, lift_d1::bits(f14));
    {
        const uint32_t target = ra;
        WRITE32_B(a0, 220, lift_d1::bits(f15));
        ctx->f[0] = f0; ctx->f[12] = f12; ctx->f[13] = f13; ctx->f[14] = f14; ctx->f[15] = f15; ctx->f[16] = f16; Ps2SetGpr64(ctx, 1, sx64(at));
        ctx->pc = target;
        return;
    }
}

// set_yaw_parameter_effective_ratio_nico_yaw_pitch_cntrl (0x1347358): lift_d1's text (the reference)
void set_yaw_parameter_effective_ratio_nico_yaw_pitch_cntrl(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t a0 = Ps2GprU32(ctx, 4), ra = Ps2GprU32(ctx, 31);
    float f12 = ctx->f[12];
    {
        const uint32_t target = ra;
        WRITE32_B(a0, 312, lift_d1::bits(f12));
        
        ctx->pc = target;
        return;
    }
}

// set_pitch_parameter_effective_ratio_nico_yaw_pitch_cntrl (0x1347360): lift_d1's text (the reference)
void set_pitch_parameter_effective_ratio_nico_yaw_pitch_cntrl(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t a0 = Ps2GprU32(ctx, 4), ra = Ps2GprU32(ctx, 31);
    float f12 = ctx->f[12];
    {
        const uint32_t target = ra;
        WRITE32_B(a0, 316, lift_d1::bits(f12));
        
        ctx->pc = target;
        return;
    }
}

// set_temp_stop_base_rot_flag_nico_yaw_pitch_cntrl (0x1347598): lift_d1's text (the reference)
void set_temp_stop_base_rot_flag_nico_yaw_pitch_cntrl(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t a0 = Ps2GprU32(ctx, 4), a1 = Ps2GprU32(ctx, 5), ra = Ps2GprU32(ctx, 31);
    {
        const uint32_t target = ra;
        WRITE32_B(a0, 48, a1);
        
        ctx->pc = target;
        return;
    }
}

// is_enable_base_yaw_rot_nico_yaw_pitch_cntrl (0x1347838): lift_d1's text (the reference)
void is_enable_base_yaw_rot_nico_yaw_pitch_cntrl(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t v0 = Ps2GprU32(ctx, 2), a0 = Ps2GprU32(ctx, 4), ra = Ps2GprU32(ctx, 31);
    {
        const uint32_t target = ra;
        v0 = READ32_B(a0, 204);
        Ps2SetGpr64(ctx, 2, sx64(v0));
        ctx->pc = target;
        return;
    }
}

// is_enable_base_pitch_rot_nico_yaw_pitch_cntrl (0x1347840): lift_d1's text (the reference)
void is_enable_base_pitch_rot_nico_yaw_pitch_cntrl(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t v0 = Ps2GprU32(ctx, 2), a0 = Ps2GprU32(ctx, 4), ra = Ps2GprU32(ctx, 31);
    {
        const uint32_t target = ra;
        v0 = READ32_B(a0, 232);
        Ps2SetGpr64(ctx, 2, sx64(v0));
        ctx->pc = target;
        return;
    }
}

// get_base_yaw_move_nico_yaw_pitch_cntrl (0x1347848): lift_d1's text (the reference)
void get_base_yaw_move_nico_yaw_pitch_cntrl(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t a0 = Ps2GprU32(ctx, 4), ra = Ps2GprU32(ctx, 31);
    float f0 = ctx->f[0];
    {
        const uint32_t target = ra;
        f0 = lift_d1::flt(READ32_B(a0, 88));
        ctx->f[0] = f0;
        ctx->pc = target;
        return;
    }
}

// set_base_yaw_nico_yaw_pitch_cntrl (0x1347858): lift_d1's text (the reference)
void set_base_yaw_nico_yaw_pitch_cntrl(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t a0 = Ps2GprU32(ctx, 4), ra = Ps2GprU32(ctx, 31);
    float f12 = ctx->f[12];
    {
        const uint32_t target = ra;
        WRITE32_B(a0, 92, lift_d1::bits(f12));
        
        ctx->pc = target;
        return;
    }
}

// set_base_pitch_nico_yaw_pitch_cntrl (0x1347860): lift_d1's text (the reference)
void set_base_pitch_nico_yaw_pitch_cntrl(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t a0 = Ps2GprU32(ctx, 4), ra = Ps2GprU32(ctx, 31);
    float f12 = ctx->f[12];
    {
        const uint32_t target = ra;
        WRITE32_B(a0, 104, lift_d1::bits(f12));
        
        ctx->pc = target;
        return;
    }
}

// get_base_yaw_nico_yaw_pitch_cntrl (0x1347868): lift_d1's text (the reference)
void get_base_yaw_nico_yaw_pitch_cntrl(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t a0 = Ps2GprU32(ctx, 4), ra = Ps2GprU32(ctx, 31);
    float f0 = ctx->f[0];
    {
        const uint32_t target = ra;
        f0 = lift_d1::flt(READ32_B(a0, 92));
        ctx->f[0] = f0;
        ctx->pc = target;
        return;
    }
}

// get_base_pitch_nico_yaw_pitch_cntrl (0x1347870): lift_d1's text (the reference)
void get_base_pitch_nico_yaw_pitch_cntrl(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t a0 = Ps2GprU32(ctx, 4), ra = Ps2GprU32(ctx, 31);
    float f0 = ctx->f[0];
    {
        const uint32_t target = ra;
        f0 = lift_d1::flt(READ32_B(a0, 104));
        ctx->f[0] = f0;
        ctx->pc = target;
        return;
    }
}

// backStageGetBossSaveArea (0x13494a8): lift_d1's text (the reference)
void backStageGetBossSaveArea(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t v0 = Ps2GprU32(ctx, 2), ra = Ps2GprU32(ctx, 31);
    v0 = 0x11E0000u;
    {
        const uint32_t target = ra;
        v0 = v0 - 0xB80u;
        Ps2SetGpr64(ctx, 2, sx64(v0));
        ctx->pc = target;
        return;
    }
}

// backStageInit (0x13494b8): lift_d1's text (the reference)
void backStageInit(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t v0 = Ps2GprU32(ctx, 2), v1 = Ps2GprU32(ctx, 3), ra = Ps2GprU32(ctx, 31);
    v0 = 0x11E0000u;
    v1 = 0xFFFFFFFFu;
    v0 = v0 - 0xBC8u;
    WRITE32_B(v0, 72, v1);
    WRITE32(v0, v1);
    {
        const uint32_t target = ra;
        WRITE32_B(v0, 36, v1);
        Ps2SetGpr64(ctx, 2, sx64(v0)); Ps2SetGpr64(ctx, 3, sx64(v1));
        ctx->pc = target;
        return;
    }
}

// bossStatGetDeadBoss (0x134a358): lift_d1's text (the reference)
void bossStatGetDeadBoss(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t v0 = Ps2GprU32(ctx, 2), v1 = Ps2GprU32(ctx, 3), ra = Ps2GprU32(ctx, 31);
    v1 = 0x1460000u;
    {
        const uint32_t target = ra;
        v0 = READ32_B(v1, 23256);
        Ps2SetGpr64(ctx, 2, sx64(v0)); Ps2SetGpr64(ctx, 3, sx64(v1));
        ctx->pc = target;
        return;
    }
}

// CheckCamFinalizerReady (0x134a778): lift_d1's text (the reference)
void CheckCamFinalizerReady(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t v0 = Ps2GprU32(ctx, 2), v1 = Ps2GprU32(ctx, 3), ra = Ps2GprU32(ctx, 31);
    v1 = 0x1460000u;
    {
        const uint32_t target = ra;
        v0 = READ32_B(v1, 23264);
        Ps2SetGpr64(ctx, 2, sx64(v0)); Ps2SetGpr64(ctx, 3, sx64(v1));
        ctx->pc = target;
        return;
    }
}

// SetCamFinalizerReady (0x134a788): lift_d1's text (the reference)
void SetCamFinalizerReady(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t v0 = Ps2GprU32(ctx, 2), a0 = Ps2GprU32(ctx, 4), ra = Ps2GprU32(ctx, 31);
    v0 = 0x1460000u;
    {
        const uint32_t target = ra;
        WRITE32_B(v0, 23264, a0);
        Ps2SetGpr64(ctx, 2, sx64(v0));
        ctx->pc = target;
        return;
    }
}

// PauseCamFinalizer (0x134a7c0): lift_d1's text (the reference)
void PauseCamFinalizer(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t v0 = Ps2GprU32(ctx, 2), a0 = Ps2GprU32(ctx, 4), ra = Ps2GprU32(ctx, 31);
    v0 = 0x1460000u;
    {
        const uint32_t target = ra;
        WRITE32_B(v0, 23328, a0);
        Ps2SetGpr64(ctx, 2, sx64(v0));
        ctx->pc = target;
        return;
    }
}

// CheckDemoCameraOrientFinished (0x134b260): lift_d1's text (the reference)
void CheckDemoCameraOrientFinished(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t v0 = Ps2GprU32(ctx, 2), a0 = Ps2GprU32(ctx, 4), ra = Ps2GprU32(ctx, 31);
    {
        const uint32_t target = ra;
        v0 = READ32_B(a0, 56);
        Ps2SetGpr64(ctx, 2, sx64(v0));
        ctx->pc = target;
        return;
    }
}

// getDemoEnableFlag (0x134b268): lift_d1's text (the reference)
void getDemoEnableFlag(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t v0 = Ps2GprU32(ctx, 2), ra = Ps2GprU32(ctx, 31);
    v0 = 0x1460000u;
    {
        const uint32_t target = ra;
        v0 = v0 + 0x5B24u;
        Ps2SetGpr64(ctx, 2, sx64(v0));
        ctx->pc = target;
        return;
    }
}

// demoscrCheckDemoFinish (0x134c838): lift_d1's text (the reference)
void demoscrCheckDemoFinish(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t v0 = Ps2GprU32(ctx, 2), v1 = Ps2GprU32(ctx, 3), ra = Ps2GprU32(ctx, 31);
    { const uint32_t _dynInsn = READ32(0x134C838u); const int32_t _dynSimm = (int16_t)(_dynInsn & 0xFFFFu); const uint32_t _dynImm = _dynInsn & 0xFFFFu; (void)_dynSimm; (void)_dynImm; (v1 = (uint32_t)((int32_t)((int32_t)((uint32_t)_dynImm << 16)))); }
    { const uint32_t _dynInsn = READ32(0x134C83Cu); const int32_t _dynSimm = (int16_t)(_dynInsn & 0xFFFFu); const uint32_t _dynImm = _dynInsn & 0xFFFFu; (void)_dynSimm; (void)_dynImm; (v0 = (uint32_t)((int32_t)((int32_t)READ32(ADD32(v1, _dynSimm))))); }
    v0 = v0 & 0x800u;
    {
        const uint32_t target = ra;
        v0 = 0u < v0 ? 1u : 0u;
        Ps2SetGpr64(ctx, 2, sx64(v0)); Ps2SetGpr64(ctx, 3, sx64(v1));
        ctx->pc = target;
        return;
    }
}

// demoscrCheckDemoCutSkip (0x134c850): lift_d1's text (the reference)
void demoscrCheckDemoCutSkip(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t ra = Ps2GprU32(ctx, 31);
    uint64_t v0 = Ps2GprU64(ctx, 2);
    {
        const uint32_t target = ra;
        (v0 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL));
        Ps2SetGpr64(ctx, 2, v0);
        ctx->pc = target;
        return;
    }
}

// endingflowTickAdpcmSeq (0x134d728): lift_d1's text (the reference)
void endingflowTickAdpcmSeq(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t v0 = Ps2GprU32(ctx, 2), a0 = Ps2GprU32(ctx, 4), ra = Ps2GprU32(ctx, 31);
    v0 = 0x11E0000u;
    {
        const uint32_t target = ra;
        WRITE32(v0 - 0x8BCu, a0);
        Ps2SetGpr64(ctx, 2, sx64(v0));
        ctx->pc = target;
        return;
    }
}

// gamecore_sub_0134D920 (0x134d920): lift_d1's text (the reference)
void gamecore_sub_0134D920(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t v0 = Ps2GprU32(ctx, 2), v1 = Ps2GprU32(ctx, 3), t0 = Ps2GprU32(ctx, 8), ra = Ps2GprU32(ctx, 31);
    v0 = 0x10000u;
    v1 = READ32_B(t0, 20);
    v0 = v0 | 0x869Eu;
    v0 = (int32_t)v0 < (int32_t)v1 ? 1u : 0u;
    if (v0 == 0) {
        v0 = READ32_B(t0, 4);
        v0 = v0 + 1u;
        WRITE32_B(t0, 4, v0);
    }
    {
        const uint32_t target = ra;
        Ps2SetGpr64(ctx, 2, sx64(v0)); Ps2SetGpr64(ctx, 3, sx64(v1));
        ctx->pc = target;
        return;
    }
}

// initFadeParam (0x134db40): lift_d1's text (the reference)
void initFadeParam(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t v0 = Ps2GprU32(ctx, 2), a0 = Ps2GprU32(ctx, 4), ra = Ps2GprU32(ctx, 31);
    float f0 = ctx->f[0], f1 = ctx->f[1], f12 = ctx->f[12];
    v0 = 0x1460000u;
    v0 = v0 + 0x5650u;
    f0 = lift_d1::flt(READ32_B(v0, 360));
    WRITE32(a0, lift_d1::bits(f0));
    f1 = lift_d1::flt(READ32_B(v0, 360));
    WRITE32_B(a0, 12, lift_d1::bits(f12));
    WRITE32_B(a0, 4, lift_d1::bits(f1));
    WRITE32_B(a0, 8, lift_d1::bits(f12));
    {
        const uint32_t target = ra;
        WRITE32_B(a0, 16, lift_d1::bits(f12));
        ctx->f[0] = f0; ctx->f[1] = f1; Ps2SetGpr64(ctx, 2, sx64(v0));
        ctx->pc = target;
        return;
    }
}

// setFadeParam (0x134db68): lift_d1's text (the reference)
void setFadeParam(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t v0 = Ps2GprU32(ctx, 2), a0 = Ps2GprU32(ctx, 4), a1 = Ps2GprU32(ctx, 5), ra = Ps2GprU32(ctx, 31);
    float f0 = ctx->f[0], f1 = ctx->f[1], f2 = ctx->f[2], f12 = ctx->f[12], f13 = ctx->f[13];
    v0 = 0x1460000u;
    f2 = lift_d1::flt(READ32_B(a0, 16));
    v0 = v0 + 0x5650u;
    f0 = lift_d1::flt(READ32_B(v0, 360));
    WRITE32(a0, lift_d1::bits(f0));
    f1 = lift_d1::flt(READ32_B(v0, 360));
    WRITE32_B(a0, 20, a1);
    f1 = FPU_ADD_S(f1, f13);
    WRITE32_B(a0, 8, lift_d1::bits(f2));
    WRITE32_B(a0, 12, lift_d1::bits(f12));
    {
        const uint32_t target = ra;
        WRITE32_B(a0, 4, lift_d1::bits(f1));
        ctx->f[0] = f0; ctx->f[1] = f1; ctx->f[2] = f2; Ps2SetGpr64(ctx, 2, sx64(v0));
        ctx->pc = target;
        return;
    }
}

// calcFadeParam (0x134db98): lift_d1's text (the reference)
void calcFadeParam(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t v0 = Ps2GprU32(ctx, 2), a0 = Ps2GprU32(ctx, 4), ra = Ps2GprU32(ctx, 31);
    float f0 = ctx->f[0], f1 = ctx->f[1], f2 = ctx->f[2], f3 = ctx->f[3], f4 = ctx->f[4];
    uint32_t fcr31 = ctx->fcr31;
    v0 = 0x1460000u;
    f4 = lift_d1::flt(READ32_B(a0, 4));
    f3 = lift_d1::flt(READ32_B(v0, 22456));
    fcr31 = (FPU_C_OLE_S(f3, f4)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
    {
        const bool c = (fcr31 & 0x800000u) == 0u;
        f0 = lift_d1::flt(READ32_B(a0, 12));
        if (!c) {
            f1 = lift_d1::flt(READ32(a0));
            f2 = lift_d1::flt(READ32_B(a0, 8));
            f3 = FPU_SUB_S(f3, f1);
            f1 = FPU_SUB_S(f4, f1);
            f0 = FPU_SUB_S(f0, f2);
            f3 = ps2FpuDivide(fcr31, f3, f1);
            f0 = FPU_MUL_SF(f0, f3);
            f2 = FPU_ADD_S(f2, f0);
            {
                const uint32_t target = ra;
                WRITE32_B(a0, 16, lift_d1::bits(f2));
                ctx->f[0] = f0; ctx->f[1] = f1; ctx->f[2] = f2; ctx->f[3] = f3; ctx->f[4] = f4; Ps2SetGpr64(ctx, 2, sx64(v0)); ctx->fcr31 = fcr31;
                ctx->pc = target;
                return;
            }
        }
    }
    {
        const uint32_t target = ra;
        WRITE32_B(a0, 16, lift_d1::bits(f0));
        ctx->f[0] = f0; ctx->f[3] = f3; ctx->f[4] = f4; Ps2SetGpr64(ctx, 2, sx64(v0)); ctx->fcr31 = fcr31;
        ctx->pc = target;
        return;
    }
}

// checkFadeParam (0x134dbe8): lift_d1's text (the reference)
void checkFadeParam(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t v0 = Ps2GprU32(ctx, 2), a0 = Ps2GprU32(ctx, 4), ra = Ps2GprU32(ctx, 31);
    float f0 = ctx->f[0], f1 = ctx->f[1];
    uint32_t fcr31 = ctx->fcr31;
    v0 = 0x1460000u;
    f1 = lift_d1::flt(READ32_B(a0, 4));
    f0 = lift_d1::flt(READ32_B(v0, 22456));
    fcr31 = (FPU_C_OLE_S(f0, f1)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
    {
        const bool c = (fcr31 & 0x800000u) != 0u;
        v0 = 1u;
        if (!c) {
            v0 = 0u;
        }
    }
    {
        const uint32_t target = ra;
        ctx->f[0] = f0; ctx->f[1] = f1; Ps2SetGpr64(ctx, 2, sx64(v0)); ctx->fcr31 = fcr31;
        ctx->pc = target;
        return;
    }
}

// fadeChkStat (0x134dc20): lift_d1's text (the reference)
void fadeChkStat(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t v1 = Ps2GprU32(ctx, 3), ra = Ps2GprU32(ctx, 31);
    uint64_t v0 = Ps2GprU64(ctx, 2), a0 = Ps2GprU64(ctx, 4);
    v1 = 0x1460000u;
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)READ32_B(v1, 23396))));
    (v0 = (uint64_t)(RC_U64X(v0) & a0));
    {
        const uint32_t target = ra;
        (v0 = (uint64_t)(((uint64_t)0ULL < (uint64_t)v0) ? 1 : 0));
        Ps2SetGpr64(ctx, 2, v0); Ps2SetGpr64(ctx, 3, sx64(v1));
        ctx->pc = target;
        return;
    }
}

// setTreeId (0x134e390): lift_d1's text (the reference)
void setTreeId(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t v0 = Ps2GprU32(ctx, 2), v1 = Ps2GprU32(ctx, 3), a0 = Ps2GprU32(ctx, 4), ra = Ps2GprU32(ctx, 31);
    v0 = 0x1460000u;
    v1 = 0x1460000u;
    WRITE32_B(v0, 23400, a0);
    {
        const uint32_t target = ra;
        WRITE32_B(v1, 23404, 0u);
        Ps2SetGpr64(ctx, 2, sx64(v0)); Ps2SetGpr64(ctx, 3, sx64(v1));
        ctx->pc = target;
        return;
    }
}

// _optionWorkChk (0x134ef80): lift_d1's text (the reference)
void _optionWorkChk(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t v0 = Ps2GprU32(ctx, 2), a0 = Ps2GprU32(ctx, 4), ra = Ps2GprU32(ctx, 31);
    a0 = a0 << 2;
    v0 = 0x11E0000u;
    v0 = v0 + a0;
    v0 = READ32(v0 - 0x8B8u);
    {
        const uint32_t target = ra;
        Ps2SetGpr64(ctx, 2, sx64(v0)); Ps2SetGpr64(ctx, 4, sx64(a0));
        ctx->pc = target;
        return;
    }
}

// _optionWorkPtr (0x134ef98): lift_d1's text (the reference)
void _optionWorkPtr(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t at = Ps2GprU32(ctx, 1), v0 = Ps2GprU32(ctx, 2), a0 = Ps2GprU32(ctx, 4), ra = Ps2GprU32(ctx, 31);
    v0 = a0 << 2;
    at = 0x11E0000u;
    at = at - 0x8B8u;
    v0 = at + v0;
    {
        const uint32_t target = ra;
        Ps2SetGpr64(ctx, 1, sx64(at)); Ps2SetGpr64(ctx, 2, sx64(v0));
        ctx->pc = target;
        return;
    }
}

// _gameIWorkSet (0x134f0d8): lift_d1's text (the reference)
void _gameIWorkSet(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t v0 = Ps2GprU32(ctx, 2), v1 = Ps2GprU32(ctx, 3), a0 = Ps2GprU32(ctx, 4), a1 = Ps2GprU32(ctx, 5), ra = Ps2GprU32(ctx, 31);
    v0 = 0x1460000u;
    a0 = a0 << 2;
    v1 = READ32_B(v0, 23412);
    a0 = a0 + v1;
    {
        const uint32_t target = ra;
        WRITE32(a0, a1);
        Ps2SetGpr64(ctx, 2, sx64(v0)); Ps2SetGpr64(ctx, 3, sx64(v1)); Ps2SetGpr64(ctx, 4, sx64(a0));
        ctx->pc = target;
        return;
    }
}

// _gameWorkChk (0x134f108): lift_d1's text (the reference)
void _gameWorkChk(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t v0 = Ps2GprU32(ctx, 2), a0 = Ps2GprU32(ctx, 4), ra = Ps2GprU32(ctx, 31);
    a0 = a0 << 2;
    v0 = 0x11E0000u;
    v0 = v0 + a0;
    v0 = READ32(v0 - 0x7E0u);
    {
        const uint32_t target = ra;
        Ps2SetGpr64(ctx, 2, sx64(v0)); Ps2SetGpr64(ctx, 4, sx64(a0));
        ctx->pc = target;
        return;
    }
}

// _gameIWorkChk (0x134f138): lift_d1's text (the reference)
void _gameIWorkChk(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t v0 = Ps2GprU32(ctx, 2), v1 = Ps2GprU32(ctx, 3), a0 = Ps2GprU32(ctx, 4), ra = Ps2GprU32(ctx, 31);
    v0 = 0x1460000u;
    a0 = a0 << 2;
    v1 = READ32_B(v0, 23412);
    a0 = a0 + v1;
    {
        const uint32_t target = ra;
        v0 = READ32(a0);
        Ps2SetGpr64(ctx, 2, sx64(v0)); Ps2SetGpr64(ctx, 3, sx64(v1)); Ps2SetGpr64(ctx, 4, sx64(a0));
        ctx->pc = target;
        return;
    }
}

// setBossClearCnt (0x134f238): lift_d1's text (the reference)
void setBossClearCnt(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t v0 = Ps2GprU32(ctx, 2), a0 = Ps2GprU32(ctx, 4), ra = Ps2GprU32(ctx, 31);
    v0 = 0x11E0000u;
    {
        const uint32_t target = ra;
        WRITE32_B(v0, 112, a0);
        Ps2SetGpr64(ctx, 2, sx64(v0));
        ctx->pc = target;
        return;
    }
}

// getGameClearCnt (0x134f278): lift_d1's text (the reference)
void getGameClearCnt(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t v0 = Ps2GprU32(ctx, 2), v1 = Ps2GprU32(ctx, 3), ra = Ps2GprU32(ctx, 31);
    v1 = 0x11E0000u;
    {
        const uint32_t target = ra;
        v0 = READ32_B(v1, 116);
        Ps2SetGpr64(ctx, 2, sx64(v0)); Ps2SetGpr64(ctx, 3, sx64(v1));
        ctx->pc = target;
        return;
    }
}

// getGameHardMode (0x134f2a8): lift_d1's text (the reference)
void getGameHardMode(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t v0 = Ps2GprU32(ctx, 2), v1 = Ps2GprU32(ctx, 3), ra = Ps2GprU32(ctx, 31);
    v1 = 0x11E0000u;
    v0 = READ32_B(v1, 124);
    {
        const uint32_t target = ra;
        v0 = 0u < v0 ? 1u : 0u;
        Ps2SetGpr64(ctx, 2, sx64(v0)); Ps2SetGpr64(ctx, 3, sx64(v1));
        ctx->pc = target;
        return;
    }
}

// setSavedClock (0x134f320): lift_d1's text (the reference)
void setSavedClock(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t v0 = Ps2GprU32(ctx, 2), a0 = Ps2GprU32(ctx, 4), ra = Ps2GprU32(ctx, 31);
    uint64_t v1 = Ps2GprU64(ctx, 3);
    (v1 = (uint64_t)(READ64_B(a0, 0)));
    v0 = 0x11E0000u;
    {
        const uint32_t target = ra;
        WRITE64_B(v0, 128, v1);
        Ps2SetGpr64(ctx, 2, sx64(v0)); Ps2SetGpr64(ctx, 3, v1);
        ctx->pc = target;
        return;
    }
}

// getSavedPlaceId (0x134f330): lift_d1's text (the reference)
void getSavedPlaceId(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t v0 = Ps2GprU32(ctx, 2), v1 = Ps2GprU32(ctx, 3), ra = Ps2GprU32(ctx, 31);
    v1 = 0x11E0000u;
    {
        const uint32_t target = ra;
        v0 = (uint32_t)(int8_t)READ8_B(v1, 88);
        Ps2SetGpr64(ctx, 2, sx64(v0)); Ps2SetGpr64(ctx, 3, sx64(v1));
        ctx->pc = target;
        return;
    }
}

// getOptionDataAdrsAndSize (0x134fe00): lift_d1's text (the reference)
void getOptionDataAdrsAndSize(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t v0 = Ps2GprU32(ctx, 2), v1 = Ps2GprU32(ctx, 3), a0 = Ps2GprU32(ctx, 4), ra = Ps2GprU32(ctx, 31);
    v1 = 0x88u;
    v0 = 0x11E0000u;
    WRITE32(a0, v1);
    {
        const uint32_t target = ra;
        v0 = v0 - 0x8B8u;
        Ps2SetGpr64(ctx, 2, sx64(v0)); Ps2SetGpr64(ctx, 3, sx64(v1));
        ctx->pc = target;
        return;
    }
}

// gameflowSelectGameoverMenu (0x13521e0): lift_d1's text (the reference)
void gameflowSelectGameoverMenu(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t v0 = Ps2GprU32(ctx, 2), a0 = Ps2GprU32(ctx, 4), ra = Ps2GprU32(ctx, 31);
    v0 = 0x11E0000u;
    {
        const uint32_t target = ra;
        WRITE32_B(v0, 340, a0);
        Ps2SetGpr64(ctx, 2, sx64(v0));
        ctx->pc = target;
        return;
    }
}

// gameflowSetPauseEnable (0x1352418): lift_d1's text (the reference)
void gameflowSetPauseEnable(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t v0 = Ps2GprU32(ctx, 2), v1 = Ps2GprU32(ctx, 3), ra = Ps2GprU32(ctx, 31);
    uint64_t a0 = Ps2GprU64(ctx, 4);
    v0 = 0x1460000u;
    {
        const bool c = (int64_t)a0 != 0;
        v1 = v0 + 0x5BC4u;
        if (!c) {
            v0 = READ32(v1);
            v0 = v0 + 1u;
            goto L_1352438;
        }
    }
    v0 = READ32(v1);
    v0 = v0 - 1u;
L_1352438:
    {
        const uint32_t target = ra;
        WRITE32(v1, v0);
        Ps2SetGpr64(ctx, 2, sx64(v0)); Ps2SetGpr64(ctx, 3, sx64(v1));
        ctx->pc = target;
        return;
    }
}

// gameflowGameOver (0x13524a8): lift_d1's text (the reference)
void gameflowGameOver(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t v0 = Ps2GprU32(ctx, 2), v1 = Ps2GprU32(ctx, 3), ra = Ps2GprU32(ctx, 31);
    v1 = 0x11E0000u;
    v0 = 1u;
    {
        const uint32_t target = ra;
        WRITE32_B(v1, 336, v0);
        Ps2SetGpr64(ctx, 2, sx64(v0)); Ps2SetGpr64(ctx, 3, sx64(v1));
        ctx->pc = target;
        return;
    }
}

// gameflowGetJumpSekiban (0x13524b8): lift_d1's text (the reference)
void gameflowGetJumpSekiban(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t v0 = Ps2GprU32(ctx, 2), v1 = Ps2GprU32(ctx, 3), ra = Ps2GprU32(ctx, 31);
    v1 = 0x1460000u;
    {
        const uint32_t target = ra;
        v0 = READ32_B(v1, 23488);
        Ps2SetGpr64(ctx, 2, sx64(v0)); Ps2SetGpr64(ctx, 3, sx64(v1));
        ctx->pc = target;
        return;
    }
}

// gameflowSetJumpSekiban (0x13524c8): lift_d1's text (the reference)
void gameflowSetJumpSekiban(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t v0 = Ps2GprU32(ctx, 2), a0 = Ps2GprU32(ctx, 4), ra = Ps2GprU32(ctx, 31);
    v0 = 0x1460000u;
    {
        const uint32_t target = ra;
        WRITE32_B(v0, 23488, a0);
        Ps2SetGpr64(ctx, 2, sx64(v0));
        ctx->pc = target;
        return;
    }
}

// gamecore_sub_01352770 (0x1352770): lift_d1's text (the reference)
void gamecore_sub_01352770(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t v0 = Ps2GprU32(ctx, 2), v1 = Ps2GprU32(ctx, 3), ra = Ps2GprU32(ctx, 31);
    v1 = 0x11E0000u;
    v0 = 5u;
    {
        const uint32_t target = ra;
        WRITE32_B(v1, 336, v0);
        Ps2SetGpr64(ctx, 2, sx64(v0)); Ps2SetGpr64(ctx, 3, sx64(v1));
        ctx->pc = target;
        return;
    }
}

// gStatGetOmakeStatAdrs (0x1353570): lift_d1's text (the reference)
void gStatGetOmakeStatAdrs(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t v0 = Ps2GprU32(ctx, 2), ra = Ps2GprU32(ctx, 31);
    v0 = 0x11E0000u;
    {
        const uint32_t target = ra;
        v0 = v0 + 0x3DB0u;
        Ps2SetGpr64(ctx, 2, sx64(v0));
        ctx->pc = target;
        return;
    }
}

// gStatGObjDispChk (0x13535d8): lift_d1's text (the reference)
void gStatGObjDispChk(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t v0 = Ps2GprU32(ctx, 2), a0 = Ps2GprU32(ctx, 4), ra = Ps2GprU32(ctx, 31);
    v0 = READ32_B(a0, 16);
    {
        const uint32_t target = ra;
        v0 = v0 & 1u;
        Ps2SetGpr64(ctx, 2, sx64(v0));
        ctx->pc = target;
        return;
    }
}

// gStatSetGObjAdrs (0x13536d0): lift_d1's text (the reference)
void gStatSetGObjAdrs(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t a0 = Ps2GprU32(ctx, 4), a1 = Ps2GprU32(ctx, 5), ra = Ps2GprU32(ctx, 31);
    {
        const uint32_t target = ra;
        WRITE32_B(a0, 12, a1);
        
        ctx->pc = target;
        return;
    }
}

// gStatGetPlayerAccessoriesDamgeRaitoToPlayer (0x1353730): lift_d1's text (the reference)
void gStatGetPlayerAccessoriesDamgeRaitoToPlayer(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t at = Ps2GprU32(ctx, 1), v0 = Ps2GprU32(ctx, 2), ra = Ps2GprU32(ctx, 31);
    float f0 = ctx->f[0], f1 = ctx->f[1];
    v0 = 0x1460000u;
    at = 0x3F800000u;
    f0 = lift_d1::flt(at);
    v0 = READ32_B(v0, 24012);
    if (v0 != 0) {
        f0 = lift_d1::flt(READ32_B(v0, 8));
    } else {
    }
    v0 = 0x1460000u;
    v0 = READ32_B(v0, 24016);
    {
        const bool c = v0 == 0;
        f1 = FPU_MOV_S(f0);
        if (!c) {
            f0 = lift_d1::flt(READ32_B(v0, 8));
            f1 = FPU_MUL_S(f1, f0);
        }
    }
    {
        const uint32_t target = ra;
        f0 = FPU_MOV_S(f1);
        ctx->f[0] = f0; ctx->f[1] = f1; Ps2SetGpr64(ctx, 1, sx64(at)); Ps2SetGpr64(ctx, 2, sx64(v0));
        ctx->pc = target;
        return;
    }
}

// gStatGetPlayerAccessoriesDamgeRaitoToBoss (0x1353768): lift_d1's text (the reference)
void gStatGetPlayerAccessoriesDamgeRaitoToBoss(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t at = Ps2GprU32(ctx, 1), v0 = Ps2GprU32(ctx, 2), ra = Ps2GprU32(ctx, 31);
    float f0 = ctx->f[0], f1 = ctx->f[1];
    v0 = 0x1460000u;
    at = 0x3F800000u;
    f0 = lift_d1::flt(at);
    v0 = READ32_B(v0, 24012);
    if (v0 != 0) {
        f0 = lift_d1::flt(READ32_B(v0, 12));
    } else {
    }
    v0 = 0x1460000u;
    v0 = READ32_B(v0, 24016);
    {
        const bool c = v0 == 0;
        f1 = FPU_MOV_S(f0);
        if (!c) {
            f0 = lift_d1::flt(READ32_B(v0, 12));
            f1 = FPU_MUL_S(f1, f0);
        }
    }
    {
        const uint32_t target = ra;
        f0 = FPU_MOV_S(f1);
        ctx->f[0] = f0; ctx->f[1] = f1; Ps2SetGpr64(ctx, 1, sx64(at)); Ps2SetGpr64(ctx, 2, sx64(v0));
        ctx->pc = target;
        return;
    }
}

// gstatBootMcSetExistNicoData (0x1353e00): lift_d1's text (the reference)
void gstatBootMcSetExistNicoData(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t v0 = Ps2GprU32(ctx, 2), v1 = Ps2GprU32(ctx, 3), a0 = Ps2GprU32(ctx, 4), a1 = Ps2GprU32(ctx, 5), ra = Ps2GprU32(ctx, 31);
    v0 = 0x1460000u;
    v1 = 0x1460000u;
    WRITE32_B(v0, 24024, a0);
    {
        const uint32_t target = ra;
        WRITE32_B(v1, 24028, a1);
        Ps2SetGpr64(ctx, 2, sx64(v0)); Ps2SetGpr64(ctx, 3, sx64(v1));
        ctx->pc = target;
        return;
    }
}

// gstatBootMcSetExistIcoData (0x1353ef8): lift_d1's text (the reference)
void gstatBootMcSetExistIcoData(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t v0 = Ps2GprU32(ctx, 2), a0 = Ps2GprU32(ctx, 4), ra = Ps2GprU32(ctx, 31);
    v0 = 0x1460000u;
    {
        const uint32_t target = ra;
        WRITE32_B(v0, 24032, a0);
        Ps2SetGpr64(ctx, 2, sx64(v0));
        ctx->pc = target;
        return;
    }
}

// gstatBootMcChkExistIcoData (0x1353f08): lift_d1's text (the reference)
void gstatBootMcChkExistIcoData(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t v0 = Ps2GprU32(ctx, 2), v1 = Ps2GprU32(ctx, 3), ra = Ps2GprU32(ctx, 31);
    v1 = 0x1460000u;
    {
        const uint32_t target = ra;
        v0 = READ32_B(v1, 24032);
        Ps2SetGpr64(ctx, 2, sx64(v0)); Ps2SetGpr64(ctx, 3, sx64(v1));
        ctx->pc = target;
        return;
    }
}

// jumphokoraflowDemoStart (0x13549a0): lift_d1's text (the reference)
void jumphokoraflowDemoStart(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t v0 = Ps2GprU32(ctx, 2), v1 = Ps2GprU32(ctx, 3), ra = Ps2GprU32(ctx, 31);
    v1 = 0x1460000u;
    v0 = READ32_B(v1, 24048);
    {
        const uint32_t target = ra;
        v0 = v0 & 1u;
        Ps2SetGpr64(ctx, 2, sx64(v0)); Ps2SetGpr64(ctx, 3, sx64(v1));
        ctx->pc = target;
        return;
    }
}

// jumphokoraflowDemoEnd (0x13549b0): lift_d1's text (the reference)
void jumphokoraflowDemoEnd(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t v0 = Ps2GprU32(ctx, 2), v1 = Ps2GprU32(ctx, 3), a0 = Ps2GprU32(ctx, 4), ra = Ps2GprU32(ctx, 31);
    v1 = 0x1460000u;
    v1 = v1 + 0x5DF0u;
    v0 = READ32(v1);
    WRITE32_B(v1, 4, a0);
    v0 = v0 | 2u;
    {
        const uint32_t target = ra;
        WRITE32(v1, v0);
        Ps2SetGpr64(ctx, 2, sx64(v0)); Ps2SetGpr64(ctx, 3, sx64(v1));
        ctx->pc = target;
        return;
    }
}

// jumphokoraGetDemoInfo (0x13549d0): lift_d1's text (the reference)
void jumphokoraGetDemoInfo(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t v0 = Ps2GprU32(ctx, 2), v1 = Ps2GprU32(ctx, 3), a0 = Ps2GprU32(ctx, 4), a1 = Ps2GprU32(ctx, 5), a2 = Ps2GprU32(ctx, 6), ra = Ps2GprU32(ctx, 31);
    v1 = 0x11E0000u;
    a2 = 0x11E0000u;
    v0 = READ32_B(v1, 16032);
    WRITE32(a0, v0);
    v1 = READ32_B(a2, 16036);
    {
        const uint32_t target = ra;
        WRITE32(a1, v1);
        Ps2SetGpr64(ctx, 2, sx64(v0)); Ps2SetGpr64(ctx, 3, sx64(v1)); Ps2SetGpr64(ctx, 6, sx64(a2));
        ctx->pc = target;
        return;
    }
}

// gcCheckStageLoadFinish (0x1356570): lift_d1's text (the reference)
void gcCheckStageLoadFinish(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t v0 = Ps2GprU32(ctx, 2), v1 = Ps2GprU32(ctx, 3), ra = Ps2GprU32(ctx, 31);
    v1 = 0x1460000u;
    v0 = READ32_B(v1, 24068);
    v0 = ~(0u | v0);
    {
        const uint32_t target = ra;
        v0 = v0 < 1u ? 1u : 0u;
        Ps2SetGpr64(ctx, 2, sx64(v0)); Ps2SetGpr64(ctx, 3, sx64(v1));
        ctx->pc = target;
        return;
    }
}

// opflowOp1End (0x1357648): lift_d1's text (the reference)
void opflowOp1End(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t v0 = Ps2GprU32(ctx, 2), v1 = Ps2GprU32(ctx, 3), ra = Ps2GprU32(ctx, 31);
    v1 = 0x1460000u;
    v1 = v1 + 0x5E40u;
    v0 = READ32(v1);
    v0 = v0 | 0x11u;
    {
        const uint32_t target = ra;
        WRITE32(v1, v0);
        Ps2SetGpr64(ctx, 2, sx64(v0)); Ps2SetGpr64(ctx, 3, sx64(v1));
        ctx->pc = target;
        return;
    }
}

// opflowOp2Start (0x1357660): lift_d1's text (the reference)
void opflowOp2Start(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t v0 = Ps2GprU32(ctx, 2), v1 = Ps2GprU32(ctx, 3), ra = Ps2GprU32(ctx, 31);
    v1 = 0x1460000u;
    v1 = v1 + 0x5E40u;
    v0 = READ32(v1);
    v0 = v0 | 2u;
    {
        const uint32_t target = ra;
        WRITE32(v1, v0);
        Ps2SetGpr64(ctx, 2, sx64(v0)); Ps2SetGpr64(ctx, 3, sx64(v1));
        ctx->pc = target;
        return;
    }
}

// opflowOp2End (0x1357678): lift_d1's text (the reference)
void opflowOp2End(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t v0 = Ps2GprU32(ctx, 2), v1 = Ps2GprU32(ctx, 3), a0 = Ps2GprU32(ctx, 4), a1 = Ps2GprU32(ctx, 5), ra = Ps2GprU32(ctx, 31);
    v1 = 0x1460000u;
    a0 = a0 & 1u;
    v1 = v1 + 0x5E40u;
    a1 = 0xFFFFFFF7u;
    v0 = READ32(v1);
    a0 = a0 << 3;
    v0 = v0 | 4u;
    v0 = v0 & a1;
    v0 = v0 | a0;
    {
        const uint32_t target = ra;
        WRITE32(v1, v0);
        Ps2SetGpr64(ctx, 2, sx64(v0)); Ps2SetGpr64(ctx, 3, sx64(v1)); Ps2SetGpr64(ctx, 4, sx64(a0)); Ps2SetGpr64(ctx, 5, sx64(a1));
        ctx->pc = target;
        return;
    }
}

// opflowChkTitleDemoStart (0x13576a8): lift_d1's text (the reference)
void opflowChkTitleDemoStart(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t v0 = Ps2GprU32(ctx, 2), v1 = Ps2GprU32(ctx, 3), ra = Ps2GprU32(ctx, 31);
    v1 = 0x1460000u;
    v0 = READ32_B(v1, 24128);
    v0 = v0 >> 4;
    {
        const uint32_t target = ra;
        v0 = v0 & 1u;
        Ps2SetGpr64(ctx, 2, sx64(v0)); Ps2SetGpr64(ctx, 3, sx64(v1));
        ctx->pc = target;
        return;
    }
}

// opflowTitleDmoeEnd (0x13576c0): lift_d1's text (the reference)
void opflowTitleDmoeEnd(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t v0 = Ps2GprU32(ctx, 2), v1 = Ps2GprU32(ctx, 3), ra = Ps2GprU32(ctx, 31);
    v1 = 0x1460000u;
    v1 = v1 + 0x5E40u;
    v0 = READ32(v1);
    v0 = v0 | 0x20u;
    {
        const uint32_t target = ra;
        WRITE32(v1, v0);
        Ps2SetGpr64(ctx, 2, sx64(v0)); Ps2SetGpr64(ctx, 3, sx64(v1));
        ctx->pc = target;
        return;
    }
}

// opflowChkAbort (0x13576f8): lift_d1's text (the reference)
void opflowChkAbort(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t v0 = Ps2GprU32(ctx, 2), v1 = Ps2GprU32(ctx, 3), ra = Ps2GprU32(ctx, 31);
    v1 = 0x11E0000u;
    v0 = READ32_B(v1, 16080);
    {
        const uint32_t target = ra;
        v0 = 0u < v0 ? 1u : 0u;
        Ps2SetGpr64(ctx, 2, sx64(v0)); Ps2SetGpr64(ctx, 3, sx64(v1));
        ctx->pc = target;
        return;
    }
}

// opflowChkLoadStart (0x1357708): lift_d1's text (the reference)
void opflowChkLoadStart(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t v0 = Ps2GprU32(ctx, 2), v1 = Ps2GprU32(ctx, 3), ra = Ps2GprU32(ctx, 31);
    v1 = 0x11E0000u;
    {
        const uint32_t target = ra;
        v0 = READ32_B(v1, 16080);
        Ps2SetGpr64(ctx, 2, sx64(v0)); Ps2SetGpr64(ctx, 3, sx64(v1));
        ctx->pc = target;
        return;
    }
}

// getPlayerHp (0x1358248): lift_d1's text (the reference)
void getPlayerHp(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t v0 = Ps2GprU32(ctx, 2), ra = Ps2GprU32(ctx, 31);
    float f0 = ctx->f[0];
    v0 = 0x11E0000u;
    {
        const uint32_t target = ra;
        f0 = lift_d1::flt(READ32_B(v0, 16100));
        ctx->f[0] = f0; Ps2SetGpr64(ctx, 2, sx64(v0));
        ctx->pc = target;
        return;
    }
}

// getPlayerGrip (0x13584e8): lift_d1's text (the reference)
void getPlayerGrip(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t v0 = Ps2GprU32(ctx, 2), ra = Ps2GprU32(ctx, 31);
    float f0 = ctx->f[0];
    v0 = 0x11E0000u;
    {
        const uint32_t target = ra;
        f0 = lift_d1::flt(READ32_B(v0, 16096));
        ctx->f[0] = f0; Ps2SetGpr64(ctx, 2, sx64(v0));
        ctx->pc = target;
        return;
    }
}

// initPowerWork (0x1358778): lift_d1's text (the reference)
void initPowerWork(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t v0 = Ps2GprU32(ctx, 2), ra = Ps2GprU32(ctx, 31);
    v0 = 0x11E0000u;
    {
        const uint32_t target = ra;
        WRITE32_B(v0, 16104, 0u);
        Ps2SetGpr64(ctx, 2, sx64(v0));
        ctx->pc = target;
        return;
    }
}

// resetAttackPower (0x1358788): lift_d1's text (the reference)
void resetAttackPower(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t v0 = Ps2GprU32(ctx, 2), ra = Ps2GprU32(ctx, 31);
    v0 = 0x11E0000u;
    {
        const uint32_t target = ra;
        WRITE32_B(v0, 16104, 0u);
        Ps2SetGpr64(ctx, 2, sx64(v0));
        ctx->pc = target;
        return;
    }
}

// getAttackPowerRatio (0x1358798): lift_d1's text (the reference)
void getAttackPowerRatio(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t at = Ps2GprU32(ctx, 1), v0 = Ps2GprU32(ctx, 2), ra = Ps2GprU32(ctx, 31);
    float f0 = ctx->f[0], f1 = ctx->f[1];
    v0 = 0x11E0000u;
    at = 0x3C230000u;
    at = at | 0xD70Au;
    f1 = lift_d1::flt(at);
    f0 = lift_d1::flt(READ32_B(v0, 16104));
    {
        const uint32_t target = ra;
        f0 = FPU_MUL_S(f0, f1);
        ctx->f[0] = f0; ctx->f[1] = f1; Ps2SetGpr64(ctx, 1, sx64(at)); Ps2SetGpr64(ctx, 2, sx64(v0));
        ctx->pc = target;
        return;
    }
}

// setFullPower (0x1358840): lift_d1's text (the reference)
void setFullPower(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t at = Ps2GprU32(ctx, 1), v0 = Ps2GprU32(ctx, 2), ra = Ps2GprU32(ctx, 31);
    float f0 = ctx->f[0];
    at = 0x42C80000u;
    f0 = lift_d1::flt(at);
    v0 = 0x11E0000u;
    {
        const uint32_t target = ra;
        WRITE32_B(v0, 16104, lift_d1::bits(f0));
        ctx->f[0] = f0; Ps2SetGpr64(ctx, 1, sx64(at)); Ps2SetGpr64(ctx, 2, sx64(v0));
        ctx->pc = target;
        return;
    }
}

// gamecore_sub_01358878 (0x1358878): lift_d1's text (the reference)
void gamecore_sub_01358878(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t v0 = Ps2GprU32(ctx, 2), ra = Ps2GprU32(ctx, 31);
    v0 = 0x11E0000u;
    {
        const uint32_t target = ra;
        WRITE32_B(v0, 16104, 0u);
        Ps2SetGpr64(ctx, 2, sx64(v0));
        ctx->pc = target;
        return;
    }
}

// getAreaSize (0x135d740): lift_d1's text (the reference)
void getAreaSize(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t v0 = Ps2GprU32(ctx, 2), ra = Ps2GprU32(ctx, 31);
    float f0 = ctx->f[0], f1 = ctx->f[1];
    v0 = 0x1460000u;
    v0 = v0 + 0x5E80u;
    f1 = lift_d1::flt(READ32_B(v0, 56));
    { int32_t tmp; std::memcpy(&tmp, &f1, sizeof(tmp)); f1 = FPU_CVT_S_W(tmp); }
    f0 = lift_d1::flt(READ32_B(v0, 64));
    {
        const uint32_t target = ra;
        f0 = FPU_MUL_S(f0, f1);
        ctx->f[0] = f0; ctx->f[1] = f1; Ps2SetGpr64(ctx, 2, sx64(v0));
        ctx->pc = target;
        return;
    }
}

// getPosOnMap (0x135d9c0): lift_d1's text (the reference)
void getPosOnMap(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t v0 = Ps2GprU32(ctx, 2), a0 = Ps2GprU32(ctx, 4), a1 = Ps2GprU32(ctx, 5), a2 = Ps2GprU32(ctx, 6), ra = Ps2GprU32(ctx, 31);
    float f0 = ctx->f[0], f1 = ctx->f[1], f2 = ctx->f[2];
    uint32_t fcr31 = ctx->fcr31;
    f1 = lift_d1::flt(READ32(a0));
    v0 = 0x1460000u;
    v0 = v0 + 0x5E80u;
    f0 = lift_d1::flt(READ32_B(v0, 40));
    { int32_t tmp; std::memcpy(&tmp, &f0, sizeof(tmp)); f0 = FPU_CVT_S_W(tmp); }
    f2 = lift_d1::flt(READ32_B(v0, 80));
    f1 = FPU_NEG_S(f1);
    f1 = FPU_MUL_S(f1, f0);
    f1 = ps2FpuDivide(fcr31, f1, f2);
    WRITE32(a1, lift_d1::bits(f1));
    f0 = lift_d1::flt(READ32_B(a0, 8));
    f1 = lift_d1::flt(READ32_B(v0, 44));
    { int32_t tmp; std::memcpy(&tmp, &f1, sizeof(tmp)); f1 = FPU_CVT_S_W(tmp); }
    f2 = lift_d1::flt(READ32_B(v0, 84));
    f0 = FPU_NEG_S(f0);
    f0 = FPU_MUL_S(f0, f1);
    f0 = ps2FpuDivide(fcr31, f0, f2);
    {
        const uint32_t target = ra;
        WRITE32(a2, lift_d1::bits(f0));
        ctx->f[0] = f0; ctx->f[1] = f1; ctx->f[2] = f2; Ps2SetGpr64(ctx, 2, sx64(v0)); ctx->fcr31 = fcr31;
        ctx->pc = target;
        return;
    }
}

// getSeamlessStageHighModelDispDistDefault (0x135dae8): lift_d1's text (the reference)
void getSeamlessStageHighModelDispDistDefault(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t v0 = Ps2GprU32(ctx, 2), ra = Ps2GprU32(ctx, 31);
    v0 = 0x1460000u;
    {
        const uint32_t target = ra;
        v0 = v0 + 0x5E64u;
        Ps2SetGpr64(ctx, 2, sx64(v0));
        ctx->pc = target;
        return;
    }
}

// getSeamlessStageMiddleModelDispDistDefault (0x135daf8): lift_d1's text (the reference)
void getSeamlessStageMiddleModelDispDistDefault(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t v0 = Ps2GprU32(ctx, 2), ra = Ps2GprU32(ctx, 31);
    v0 = 0x1460000u;
    {
        const uint32_t target = ra;
        v0 = v0 + 0x5E68u;
        Ps2SetGpr64(ctx, 2, sx64(v0));
        ctx->pc = target;
        return;
    }
}

// getSeamlessStageEnv (0x135dd58): lift_d1's text (the reference)
void getSeamlessStageEnv(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t v0 = Ps2GprU32(ctx, 2), ra = Ps2GprU32(ctx, 31);
    v0 = 0x1460000u;
    {
        const uint32_t target = ra;
        v0 = v0 + 0x5E80u;
        Ps2SetGpr64(ctx, 2, sx64(v0));
        ctx->pc = target;
        return;
    }
}

// forceLoadSeamlessStage (0x135df88): lift_d1's text (the reference)
void forceLoadSeamlessStage(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t v0 = Ps2GprU32(ctx, 2), v1 = Ps2GprU32(ctx, 3), ra = Ps2GprU32(ctx, 31);
    v1 = 0x1460000u;
    v0 = 1u;
    {
        const uint32_t target = ra;
        WRITE32_B(v1, 24172, v0);
        Ps2SetGpr64(ctx, 2, sx64(v0)); Ps2SetGpr64(ctx, 3, sx64(v1));
        ctx->pc = target;
        return;
    }
}

// SetSeamlessStageControlObject (0x135df98): lift_d1's text (the reference)
void SetSeamlessStageControlObject(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t v0 = Ps2GprU32(ctx, 2), a0 = Ps2GprU32(ctx, 4), ra = Ps2GprU32(ctx, 31);
    v0 = 0x1460000u;
    {
        const uint32_t target = ra;
        WRITE32_B(v0, 24228, a0);
        Ps2SetGpr64(ctx, 2, sx64(v0));
        ctx->pc = target;
        return;
    }
}

// PauseSeamlessStageManager (0x135e138): lift_d1's text (the reference)
void PauseSeamlessStageManager(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t v0 = Ps2GprU32(ctx, 2), v1 = Ps2GprU32(ctx, 3), ra = Ps2GprU32(ctx, 31);
    uint64_t a0 = Ps2GprU64(ctx, 4);
    bool df_v1 = false;
    v0 = 0x1460000u;
    {
        const bool c = (int64_t)a0 != 0;
        WRITE32_B(v0, 24196, (uint32_t)a0);
        if (!c) {
            v1 = 0x1460000u; df_v1 = true;
            v0 = 1u;
            WRITE32_B(v1, 24172, v0);
        }
    }
    {
        const uint32_t target = ra;
        Ps2SetGpr64(ctx, 2, sx64(v0)); if (df_v1) Ps2SetGpr64(ctx, 3, sx64(v1));
        ctx->pc = target;
        return;
    }
}

// getStageIDWithOffset (0x135e280): lift_d1's text (the reference)
void getStageIDWithOffset(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t v0 = Ps2GprU32(ctx, 2), a0 = Ps2GprU32(ctx, 4), a1 = Ps2GprU32(ctx, 5), a2 = Ps2GprU32(ctx, 6), a3 = Ps2GprU32(ctx, 7), t0 = Ps2GprU32(ctx, 8), ra = Ps2GprU32(ctx, 31);
    float f0 = ctx->f[0], f1 = ctx->f[1], f2 = ctx->f[2], f3 = ctx->f[3], f4 = ctx->f[4], f5 = ctx->f[5], f6 = ctx->f[6], f7 = ctx->f[7];
    uint32_t fcr31 = ctx->fcr31;
    f1 = lift_d1::flt(READ32(a0));
    v0 = 0x1460000u;
    v0 = v0 + 0x5E80u;
    f2 = lift_d1::flt(READ32_B(a0, 8));
    f7 = lift_d1::flt(READ32_B(v0, 56));
    { int32_t tmp; std::memcpy(&tmp, &f7, sizeof(tmp)); f7 = FPU_CVT_S_W(tmp); }
    f3 = lift_d1::flt(READ32_B(v0, 68));
    f6 = lift_d1::flt(READ32_B(v0, 60));
    { int32_t tmp; std::memcpy(&tmp, &f6, sizeof(tmp)); f6 = FPU_CVT_S_W(tmp); }
    f5 = lift_d1::flt(READ32_B(v0, 80));
    f1 = FPU_NEG_S(f1);
    f0 = lift_d1::flt(READ32_B(v0, 64));
    f4 = lift_d1::flt(READ32_B(v0, 84));
    f0 = FPU_MUL_S(f0, f7);
    f2 = FPU_SUB_S(f2, f4);
    f1 = FPU_SUB_S(f1, f5);
    f3 = FPU_MUL_S(f3, f6);
    f1 = ps2FpuDivide(fcr31, f1, f0);
    f2 = ps2FpuDivide(fcr31, f2, f3);
    { int32_t tmp = FPU_CVT_W_S(f1); std::memcpy(&f0, &tmp, sizeof(tmp)); }
    WRITE32(a1, lift_d1::bits(f0));
    { int32_t tmp = FPU_CVT_W_S(f2); std::memcpy(&f0, &tmp, sizeof(tmp)); }
    WRITE32(a2, lift_d1::bits(f0));
    f0 = lift_d1::flt(READ32(a1));
    { int32_t tmp; std::memcpy(&tmp, &f0, sizeof(tmp)); f0 = FPU_CVT_S_W(tmp); }
    f1 = FPU_SUB_S(f1, f0);
    WRITE32(a3, lift_d1::bits(f1));
    f0 = lift_d1::flt(READ32(a2));
    { int32_t tmp; std::memcpy(&tmp, &f0, sizeof(tmp)); f0 = FPU_CVT_S_W(tmp); }
    f2 = FPU_SUB_S(f2, f0);
    {
        const uint32_t target = ra;
        WRITE32(t0, lift_d1::bits(f2));
        ctx->f[0] = f0; ctx->f[1] = f1; ctx->f[2] = f2; ctx->f[3] = f3; ctx->f[4] = f4; ctx->f[5] = f5; ctx->f[6] = f6; ctx->f[7] = f7; Ps2SetGpr64(ctx, 2, sx64(v0)); ctx->fcr31 = fcr31;
        ctx->pc = target;
        return;
    }
}

// isSeamlessStage (0x135e608): lift_d1's text (the reference)
void isSeamlessStage(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t v0 = Ps2GprU32(ctx, 2), v1 = Ps2GprU32(ctx, 3), ra = Ps2GprU32(ctx, 31);
    v1 = 0x1460000u;
    {
        const uint32_t target = ra;
        v0 = READ32_B(v1, 24192);
        Ps2SetGpr64(ctx, 2, sx64(v0)); Ps2SetGpr64(ctx, 3, sx64(v1));
        ctx->pc = target;
        return;
    }
}

// getSpecialEffectObjWork (0x135efb0): lift_d1's text (the reference)
void getSpecialEffectObjWork(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t v0 = Ps2GprU32(ctx, 2), a0 = Ps2GprU32(ctx, 4), ra = Ps2GprU32(ctx, 31);
    {
        const uint32_t target = ra;
        v0 = READ32_B(a0, 48);
        Ps2SetGpr64(ctx, 2, sx64(v0));
        ctx->pc = target;
        return;
    }
}

// GetWorldLayoutSheetSegHeader (0x1365d08): lift_d1's text (the reference)
void GetWorldLayoutSheetSegHeader(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t v0 = Ps2GprU32(ctx, 2), v1 = Ps2GprU32(ctx, 3), ra = Ps2GprU32(ctx, 31);
    v1 = 0x1460000u;
    {
        const uint32_t target = ra;
        v0 = READ32_B(v1, 25540);
        Ps2SetGpr64(ctx, 2, sx64(v0)); Ps2SetGpr64(ctx, 3, sx64(v1));
        ctx->pc = target;
        return;
    }
}

// stagemanager_getSegmentVariation (0x1366b60): lift_d1's text (the reference)
void stagemanager_getSegmentVariation(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t v0 = Ps2GprU32(ctx, 2), v1 = Ps2GprU32(ctx, 3), ra = Ps2GprU32(ctx, 31);
    v1 = 0x1460000u;
    {
        const uint32_t target = ra;
        v0 = READ32_B(v1, 25532);
        Ps2SetGpr64(ctx, 2, sx64(v0)); Ps2SetGpr64(ctx, 3, sx64(v1));
        ctx->pc = target;
        return;
    }
}

// initWorldLayout (0x1366d00): lift_d1's text (the reference)
void initWorldLayout(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t v0 = Ps2GprU32(ctx, 2), a0 = Ps2GprU32(ctx, 4), ra = Ps2GprU32(ctx, 31);
    v0 = 0x1460000u;
    {
        const uint32_t target = ra;
        WRITE32_B(v0, 25540, a0);
        Ps2SetGpr64(ctx, 2, sx64(v0));
        ctx->pc = target;
        return;
    }
}

// bgmEnvTickProckMask (0x136a2d0): lift_d1's text (the reference)
void bgmEnvTickProckMask(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t v0 = Ps2GprU32(ctx, 2), a0 = Ps2GprU32(ctx, 4), ra = Ps2GprU32(ctx, 31);
    v0 = 0x1460000u;
    {
        const uint32_t target = ra;
        WRITE32_B(v0, 25780, a0);
        Ps2SetGpr64(ctx, 2, sx64(v0));
        ctx->pc = target;
        return;
    }
}

// bossNormalHook (0x136a378): lift_d1's text (the reference)
void bossNormalHook(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t v0 = Ps2GprU32(ctx, 2), t0 = Ps2GprU32(ctx, 8), ra = Ps2GprU32(ctx, 31);
    v0 = READ32_B(t0, 20);
    v0 = v0 + 1u;
    {
        const uint32_t target = ra;
        WRITE32_B(t0, 20, v0);
        Ps2SetGpr64(ctx, 2, sx64(v0));
        ctx->pc = target;
        return;
    }
}

// gamecore_sub_0136A388 (0x136a388): lift_d1's text (the reference)
void gamecore_sub_0136A388(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t v0 = Ps2GprU32(ctx, 2), v1 = Ps2GprU32(ctx, 3), ra = Ps2GprU32(ctx, 31);
    uint64_t a0 = Ps2GprU64(ctx, 4);
    v0 = 0x1460000u;
    v0 = v0 + 0x64B0u;
    v1 = READ32(v0);
    if ((int64_t)a0 == (int64_t)(int32_t)v1) {
        WRITE32(v0, 0u);
    } else {
    }
    {
        const uint32_t target = ra;
        Ps2SetGpr64(ctx, 2, sx64(v0)); Ps2SetGpr64(ctx, 3, sx64(v1));
        ctx->pc = target;
        return;
    }
}

// bgmPlay (0x136aa38): lift_d1's text (the reference)
void bgmPlay(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t v0 = Ps2GprU32(ctx, 2), a0 = Ps2GprU32(ctx, 4), ra = Ps2GprU32(ctx, 31);
    float f12 = ctx->f[12];
    v0 = READ32_B(a0, 36);
    WRITE32_B(a0, 12, lift_d1::bits(f12));
    v0 = v0 | 1u;
    {
        const uint32_t target = ra;
        WRITE32_B(a0, 36, v0);
        Ps2SetGpr64(ctx, 2, sx64(v0));
        ctx->pc = target;
        return;
    }
}

// bgmStop (0x136aa50): lift_d1's text (the reference)
void bgmStop(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t v0 = Ps2GprU32(ctx, 2), a0 = Ps2GprU32(ctx, 4), ra = Ps2GprU32(ctx, 31);
    uint64_t a1 = Ps2GprU64(ctx, 5);
    float f12 = ctx->f[12];
    {
        const bool c = (int64_t)a1 == 0;
        WRITE32_B(a0, 24, lift_d1::bits(f12));
        if (!c) {
            WRITE32_B((uint32_t)a1, 0, 0u);
        }
    }
    v0 = READ32_B(a0, 36);
    WRITE32_B(a0, 32, (uint32_t)a1);
    v0 = v0 | 4u;
    {
        const uint32_t target = ra;
        WRITE32_B(a0, 36, v0);
        Ps2SetGpr64(ctx, 2, sx64(v0));
        ctx->pc = target;
        return;
    }
}

// bgmStopChk (0x136aa70): lift_d1's text (the reference)
void bgmStopChk(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t ra = Ps2GprU32(ctx, 31);
    uint64_t v0 = Ps2GprU64(ctx, 2), a0 = Ps2GprU64(ctx, 4);
    {
        const uint32_t target = ra;
        (v0 = (uint64_t)((uint64_t)a0 + (uint64_t)0ULL));
        Ps2SetGpr64(ctx, 2, v0);
        ctx->pc = target;
        return;
    }
}

// bgmChangeVol (0x136aad0): lift_d1's text (the reference)
void bgmChangeVol(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t v0 = Ps2GprU32(ctx, 2), a0 = Ps2GprU32(ctx, 4), ra = Ps2GprU32(ctx, 31);
    float f12 = ctx->f[12], f13 = ctx->f[13];
    v0 = READ32_B(a0, 36);
    WRITE32_B(a0, 20, lift_d1::bits(f13));
    v0 = v0 | 2u;
    WRITE32_B(a0, 16, lift_d1::bits(f12));
    {
        const uint32_t target = ra;
        WRITE32_B(a0, 36, v0);
        Ps2SetGpr64(ctx, 2, sx64(v0));
        ctx->pc = target;
        return;
    }
}

// gamecore_sub_0136E4F8 (0x136e4f8): lift_d1's text (the reference)
void gamecore_sub_0136E4F8(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t v0 = Ps2GprU32(ctx, 2), a0 = Ps2GprU32(ctx, 4), ra = Ps2GprU32(ctx, 31);
    v0 = 1u;
    {
        const uint32_t target = ra;
        WRITE16_B(a0, 170, (uint16_t)v0);
        Ps2SetGpr64(ctx, 2, sx64(v0));
        ctx->pc = target;
        return;
    }
}

// gamecore_sub_0136E7A0 (0x136e7a0): lift_d1's text (the reference)
void gamecore_sub_0136E7A0(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t a1 = Ps2GprU32(ctx, 5), ra = Ps2GprU32(ctx, 31);
    WRITE32_B(a1, 156, 0u);
    {
        const uint32_t target = ra;
        WRITE32_B(a1, 144, 0u);
        
        ctx->pc = target;
        return;
    }
}

// nico_IK_idDetect (0x13757f8): lift_d1's text (the reference)
void nico_IK_idDetect(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t v0 = Ps2GprU32(ctx, 2), v1 = Ps2GprU32(ctx, 3), a0 = Ps2GprU32(ctx, 4), ra = Ps2GprU32(ctx, 31);
    v0 = READ32_B(a0, 20);
    a0 = 0x11E0000u;
    v1 = 0xFFFE0000u;
    v1 = v1 | 0xFFFFu;
    v0 = v0 & v1;
    {
        const uint32_t target = ra;
        WRITE32_B(a0, 24128, v0);
        Ps2SetGpr64(ctx, 2, sx64(v0)); Ps2SetGpr64(ctx, 3, sx64(v1)); Ps2SetGpr64(ctx, 4, sx64(a0));
        ctx->pc = target;
        return;
    }
}

// gamecore_sub_0137B738 (0x137b738): lift_d1's text (the reference)
void gamecore_sub_0137B738(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t ra = Ps2GprU32(ctx, 31);
    uint64_t v0 = Ps2GprU64(ctx, 2), a0 = Ps2GprU64(ctx, 4), a1 = Ps2GprU64(ctx, 5);
    (v0 = (uint64_t)(a1 ^ a0));
    {
        const uint32_t target = ra;
        (v0 = (uint64_t)(((uint64_t)0ULL < (uint64_t)v0) ? 1 : 0));
        Ps2SetGpr64(ctx, 2, v0);
        ctx->pc = target;
        return;
    }
}

// menuSelected (0x137b800): lift_d1's text (the reference)
void menuSelected(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t v0 = Ps2GprU32(ctx, 2), v1 = Ps2GprU32(ctx, 3), ra = Ps2GprU32(ctx, 31);
    v1 = 0x1460000u;
    {
        const uint32_t target = ra;
        v0 = READ32_B(v1, 25820);
        Ps2SetGpr64(ctx, 2, sx64(v0)); Ps2SetGpr64(ctx, 3, sx64(v1));
        ctx->pc = target;
        return;
    }
}

// GetDebugDispType (0x137c310): lift_d1's text (the reference)
void GetDebugDispType(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t ra = Ps2GprU32(ctx, 31);
    uint64_t v0 = Ps2GprU64(ctx, 2);
    {
        const uint32_t target = ra;
        (v0 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL));
        Ps2SetGpr64(ctx, 2, v0);
        ctx->pc = target;
        return;
    }
}

// is_LwsOrientObj (0x137e9f0): lift_d1's text (the reference)
void is_LwsOrientObj(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t v0 = Ps2GprU32(ctx, 2), a0 = Ps2GprU32(ctx, 4), ra = Ps2GprU32(ctx, 31);
    v0 = READ8_B(a0, 8);
    v0 = v0 ^ 0x16u;
    {
        const uint32_t target = ra;
        v0 = v0 < 1u ? 1u : 0u;
        Ps2SetGpr64(ctx, 2, sx64(v0));
        ctx->pc = target;
        return;
    }
}

// CheckDeathTentacle (0x1384de0): lift_d1's text (the reference)
void CheckDeathTentacle(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t v0 = Ps2GprU32(ctx, 2), v1 = Ps2GprU32(ctx, 3), ra = Ps2GprU32(ctx, 31);
    v0 = 0x1460000u;
    v0 = v0 + 0x64F0u;
    v1 = READ32(v0);
    if (v1 != 0) {
        WRITE32(v0, 0u);
        {
            const uint32_t target = ra;
            v0 = 0u;
            Ps2SetGpr64(ctx, 2, sx64(v0)); Ps2SetGpr64(ctx, 3, sx64(v1));
            ctx->pc = target;
            return;
        }
    }
    {
        const uint32_t target = ra;
        v0 = 1u;
        Ps2SetGpr64(ctx, 2, sx64(v0)); Ps2SetGpr64(ctx, 3, sx64(v1));
        ctx->pc = target;
        return;
    }
}

// getEffectBloodWorkSize (0x1388048): lift_d1's text (the reference)
void getEffectBloodWorkSize(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t v0 = Ps2GprU32(ctx, 2), a0 = Ps2GprU32(ctx, 4), ra = Ps2GprU32(ctx, 31);
    v0 = a0 << 1;
    v0 = v0 + a0;
    v0 = v0 << 4;
    {
        const uint32_t target = ra;
        v0 = v0 + 0x33u;
        Ps2SetGpr64(ctx, 2, sx64(v0));
        ctx->pc = target;
        return;
    }
}

// getWeakPointEffectIntensity (0x1388060): lift_d1's text (the reference)
void getWeakPointEffectIntensity(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t at = Ps2GprU32(ctx, 1), v0 = Ps2GprU32(ctx, 2), a0 = Ps2GprU32(ctx, 4), ra = Ps2GprU32(ctx, 31);
    float f0 = ctx->f[0];
    v0 = a0 << 4;
    v0 = v0 - a0;
    v0 = v0 << 3;
    at = 0x11E0000u;
    at = at + v0;
    f0 = lift_d1::flt(READ32_B(at, 25252));
    {
        const uint32_t target = ra;
        ctx->f[0] = f0; Ps2SetGpr64(ctx, 1, sx64(at)); Ps2SetGpr64(ctx, 2, sx64(v0));
        ctx->pc = target;
        return;
    }
}

// getTriggerPointEffectIntensity (0x1388080): lift_d1's text (the reference)
void getTriggerPointEffectIntensity(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t at = Ps2GprU32(ctx, 1), v0 = Ps2GprU32(ctx, 2), a0 = Ps2GprU32(ctx, 4), ra = Ps2GprU32(ctx, 31);
    float f0 = ctx->f[0];
    a0 = a0 + 3u;
    v0 = a0 << 4;
    v0 = v0 - a0;
    v0 = v0 << 3;
    at = 0x11E0000u;
    at = at + v0;
    f0 = lift_d1::flt(READ32_B(at, 25252));
    {
        const uint32_t target = ra;
        ctx->f[0] = f0; Ps2SetGpr64(ctx, 1, sx64(at)); Ps2SetGpr64(ctx, 2, sx64(v0)); Ps2SetGpr64(ctx, 4, sx64(a0));
        ctx->pc = target;
        return;
    }
}

// effectClearDamageWork (0x13885f0): lift_d1's text (the reference)
void effectClearDamageWork(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t v0 = Ps2GprU32(ctx, 2), ra = Ps2GprU32(ctx, 31);
    v0 = 0x11E0000u;
    {
        const uint32_t target = ra;
        WRITE32_B(v0, 25160, 0u);
        Ps2SetGpr64(ctx, 2, sx64(v0));
        ctx->pc = target;
        return;
    }
}

// effectGetDamageWork (0x1388600): lift_d1's text (the reference)
void effectGetDamageWork(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t v0 = Ps2GprU32(ctx, 2), v1 = Ps2GprU32(ctx, 3), a0 = Ps2GprU32(ctx, 4), ra = Ps2GprU32(ctx, 31);
    v1 = 0x11E0000u;
    v1 = v1 + 0x5FA0u;
    WRITE32(a0, v1);
    {
        const uint32_t target = ra;
        v0 = READ32_B(v1, 680);
        Ps2SetGpr64(ctx, 2, sx64(v0)); Ps2SetGpr64(ctx, 3, sx64(v1));
        ctx->pc = target;
        return;
    }
}

// gamecore_sub_013886B8 (0x13886b8): lift_d1's text (the reference)
void gamecore_sub_013886B8(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t v0 = Ps2GprU32(ctx, 2), a0 = Ps2GprU32(ctx, 4), ra = Ps2GprU32(ctx, 31);
    float f0 = ctx->f[0], f1 = ctx->f[1], f2 = ctx->f[2];
    v0 = READ32_B(a0, 80);
    f0 = lift_d1::flt(READ32_B(a0, 40));
    f2 = lift_d1::flt(READ32_B(v0, 112));
    f1 = lift_d1::flt(READ32_B(a0, 36));
    f0 = FPU_MUL_S(f0, f2);
    WRITE32_B(a0, 40, lift_d1::bits(f0));
    f2 = lift_d1::flt(READ32_B(v0, 20));
    f0 = lift_d1::flt(READ32_B(v0, 116));
    f0 = FPU_MUL_S(f0, f2);
    f1 = FPU_MUL_S(f1, f0);
    {
        const uint32_t target = ra;
        WRITE32_B(a0, 36, lift_d1::bits(f1));
        ctx->f[0] = f0; ctx->f[1] = f1; ctx->f[2] = f2; Ps2SetGpr64(ctx, 2, sx64(v0));
        ctx->pc = target;
        return;
    }
}

// getFallenLeafSize (0x138ea10): lift_d1's text (the reference)
void getFallenLeafSize(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t v0 = Ps2GprU32(ctx, 2), v1 = Ps2GprU32(ctx, 3), a1 = Ps2GprU32(ctx, 5), a3 = Ps2GprU32(ctx, 7), ra = Ps2GprU32(ctx, 31);
    uint64_t a0 = Ps2GprU64(ctx, 4), a2 = Ps2GprU64(ctx, 6);
    uint64_t hi = ctx->hi;
    uint64_t lo = ctx->lo;
    bool df_a1 = false;
    (a2 = (uint64_t)((uint64_t)a0 + (uint64_t)0ULL));
    v0 = 0x30000u;
    (v1 = (uint32_t)((int32_t)((int32_t)ADD32((uint32_t)a2, 28))));
    v0 = v0 | 0xFFFFu;
    a3 = READ32(v1);
    v0 = a3 & v0;
    {
        const bool c = v0 == 0;
        v0 = v1 - v0;
        if (!c) {
            (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)READ32_B(v0, 0))));
            goto L_138ea3c;
        }
    }
    (a0 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL));
L_138ea3c:
    {
        const bool c = (int64_t)a0 == 0;
        { const uint32_t _dynInsn = READ32(0x138EA40u); const int32_t _dynSimm = (int16_t)(_dynInsn & 0xFFFFu); const uint32_t _dynImm = _dynInsn & 0xFFFFu; (void)_dynSimm; (void)_dynImm; (v0 = (uint32_t)((int32_t)((int32_t)((uint32_t)_dynImm << 16)))); }
        if (!c) {
            { const uint32_t _dynInsn = READ32(0x138EA44u); const int32_t _dynSimm = (int16_t)(_dynInsn & 0xFFFFu); const uint32_t _dynImm = _dynInsn & 0xFFFFu; (void)_dynSimm; (void)_dynImm; (v1 = (uint32_t)((int32_t)((int32_t)READ32(ADD32(v0, _dynSimm))))); }
            {
                const bool c = (int64_t)a0 == (int64_t)(int32_t)v1;
                v0 = a3 >> 18;
                if (!c) {
                    (a1 = (uint32_t)((int32_t)((int32_t)READ32_B((uint32_t)a0, 8)))); df_a1 = true;
                    v0 = v0 & 0xFFFu;
                    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)READ32_B((uint32_t)a0, 16))));
                    v1 = READ32_B(a1, 12);
                    { int64_t result = (int64_t)(int32_t)v0 * (int64_t)(int32_t)v1; lo = (uint64_t)(int64_t)(int32_t)result; hi = (uint64_t)(int64_t)(int32_t)(result >> 32); (v0 = (uint32_t)((int32_t)((int32_t)result))); }
                    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)a0, v0))));
                    goto L_138ea74;
                }
            }
        }
    }
    (a0 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL));
L_138ea74:
    (v0 = (uint32_t)((int32_t)((int32_t)READ32_B((uint32_t)a2, 48))));
    {
        const bool c = v0 == 0;
        a1 = 0x170u; df_a1 = true;
        if (!c) {
            (v0 = (uint32_t)((int32_t)((int32_t)READ32_B((uint32_t)a2, 60))));
            goto L_138ea94;
        }
    }
    (v1 = (uint32_t)((int32_t)((int32_t)READ32_B((uint32_t)a2, 44))));
    (v0 = (uint32_t)((int32_t)((int32_t)READ32_B((uint32_t)a2, 40))));
    { int64_t result = (int64_t)(int32_t)v0 * (int64_t)(int32_t)v1; lo = (uint64_t)(int64_t)(int32_t)result; hi = (uint64_t)(int64_t)(int32_t)(result >> 32); (v0 = (uint32_t)((int32_t)((int32_t)result))); }
L_138ea94:
    v0 = v0 << 7;
    a1 = v0 + a1; df_a1 = true;
    (v0 = (uint32_t)((int32_t)((int32_t)READ32_B((uint32_t)a0, 8))));
    v0 = v0 << 6;
    {
        const uint32_t target = ra;
        v0 = a1 + v0;
        Ps2SetGpr64(ctx, 2, sx64(v0)); Ps2SetGpr64(ctx, 3, sx64(v1)); Ps2SetGpr64(ctx, 4, a0); if (df_a1) Ps2SetGpr64(ctx, 5, sx64(a1)); Ps2SetGpr64(ctx, 6, a2); Ps2SetGpr64(ctx, 7, sx64(a3)); ctx->hi = hi; ctx->lo = lo;
        ctx->pc = target;
        return;
    }
}

// getArrowWaterSize (0x138eb48): lift_d1's text (the reference)
void getArrowWaterSize(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t v0 = Ps2GprU32(ctx, 2), v1 = Ps2GprU32(ctx, 3), a0 = Ps2GprU32(ctx, 4), a1 = Ps2GprU32(ctx, 5), a2 = Ps2GprU32(ctx, 6), ra = Ps2GprU32(ctx, 31);
    uint64_t hi = ctx->hi;
    uint64_t lo = ctx->lo;
    bool df_v1 = false, df_a1 = false;
    a0 = a0 + 0x1Cu;
    v0 = 0x30000u;
    a2 = READ32(a0);
    v0 = v0 | 0xFFFFu;
    v0 = a2 & v0;
    {
        const bool c = v0 == 0;
        a0 = a0 - v0;
        if (!c) {
            a0 = READ32(a0);
            goto L_138eb74;
        }
    }
    a0 = 0u;
L_138eb74:
    {
        const bool c = a0 == 0;
        v0 = 0u;
        if (!c) {
            { const uint32_t _dynInsn = READ32(0x138EB7Cu); const int32_t _dynSimm = (int16_t)(_dynInsn & 0xFFFFu); const uint32_t _dynImm = _dynInsn & 0xFFFFu; (void)_dynSimm; (void)_dynImm; (v0 = (uint32_t)((int32_t)((int32_t)((uint32_t)_dynImm << 16)))); }
            { const uint32_t _dynInsn = READ32(0x138EB80u); const int32_t _dynSimm = (int16_t)(_dynInsn & 0xFFFFu); const uint32_t _dynImm = _dynInsn & 0xFFFFu; (void)_dynSimm; (void)_dynImm; (v1 = (uint32_t)((int32_t)((int32_t)READ32(ADD32(v0, _dynSimm))))); } df_v1 = true;
            if (a0 == v1) {
                v0 = 0u;
            } else {
                a1 = READ32_B(a0, 8); df_a1 = true;
                v0 = a2 >> 18;
                v0 = v0 & 0xFFFu;
                a0 = READ32_B(a0, 16);
                v1 = READ32_B(a1, 12); df_v1 = true;
                { int64_t result = (int64_t)(int32_t)v0 * (int64_t)(int32_t)v1; lo = (uint64_t)(int64_t)(int32_t)result; hi = (uint64_t)(int64_t)(int32_t)(result >> 32); (v0 = (uint32_t)((int32_t)((int32_t)result))); }
                v0 = a0 + v0;
            }
        }
    }
    v0 = READ32_B(v0, 8);
    v0 = v0 << 6;
    {
        const uint32_t target = ra;
        v0 = v0 + 0xDF0u;
        Ps2SetGpr64(ctx, 2, sx64(v0)); if (df_v1) Ps2SetGpr64(ctx, 3, sx64(v1)); Ps2SetGpr64(ctx, 4, sx64(a0)); if (df_a1) Ps2SetGpr64(ctx, 5, sx64(a1)); Ps2SetGpr64(ctx, 6, sx64(a2)); ctx->hi = hi; ctx->lo = lo;
        ctx->pc = target;
        return;
    }
}

// gamecore_sub_0138F138 (0x138f138): lift_d1's text (the reference)
void gamecore_sub_0138F138(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t v0 = Ps2GprU32(ctx, 2), v1 = Ps2GprU32(ctx, 3), a2 = Ps2GprU32(ctx, 6), ra = Ps2GprU32(ctx, 31);
    uint64_t a3 = Ps2GprU64(ctx, 7);
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
                    const uint32_t target = ra;
                    if (df_v0) Ps2SetGpr64(ctx, 2, sx64(v0)); if (df_v1) Ps2SetGpr64(ctx, 3, sx64(v1)); Ps2SetGpr64(ctx, 6, sx64(a2));
                    ctx->pc = target;
                    return;
                }
            }
        }
        v1 = 0x1460000u; df_v1 = true;
        {
            const uint32_t target = ra;
            WRITE32_B(v1, 26360, v0);
            if (df_v0) Ps2SetGpr64(ctx, 2, sx64(v0)); if (df_v1) Ps2SetGpr64(ctx, 3, sx64(v1)); Ps2SetGpr64(ctx, 6, sx64(a2));
            ctx->pc = target;
            return;
        }
L_138f170:
        v0 = 2u; df_v0 = true;
        WRITE32_B(v1, 26360, v0);
    }
    {
        const uint32_t target = ra;
        if (df_v0) Ps2SetGpr64(ctx, 2, sx64(v0)); if (df_v1) Ps2SetGpr64(ctx, 3, sx64(v1)); Ps2SetGpr64(ctx, 6, sx64(a2));
        ctx->pc = target;
        return;
    }
}

// setHorseAIStopFlag (0x1395e00): lift_d1's text (the reference)
void setHorseAIStopFlag(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t v0 = Ps2GprU32(ctx, 2), a0 = Ps2GprU32(ctx, 4), ra = Ps2GprU32(ctx, 31);
    v0 = 0x1460000u;
    {
        const uint32_t target = ra;
        WRITE32_B(v0, 26372, a0);
        Ps2SetGpr64(ctx, 2, sx64(v0));
        ctx->pc = target;
        return;
    }
}

// getHorseStandByMode (0x1395e10): lift_d1's text (the reference)
void getHorseStandByMode(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t v0 = Ps2GprU32(ctx, 2), v1 = Ps2GprU32(ctx, 3), ra = Ps2GprU32(ctx, 31);
    v1 = 0x1460000u;
    {
        const uint32_t target = ra;
        v0 = READ32_B(v1, 26376);
        Ps2SetGpr64(ctx, 2, sx64(v0)); Ps2SetGpr64(ctx, 3, sx64(v1));
        ctx->pc = target;
        return;
    }
}

// SCRFuncHorseStandByFlag (0x1396328): lift_d1's text (the reference)
void SCRFuncHorseStandByFlag(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t v0 = Ps2GprU32(ctx, 2), a1 = Ps2GprU32(ctx, 5), ra = Ps2GprU32(ctx, 31);
    v0 = 0x1460000u;
    {
        const uint32_t target = ra;
        WRITE32_B(v0, 26376, a1);
        Ps2SetGpr64(ctx, 2, sx64(v0));
        ctx->pc = target;
        return;
    }
}

// ColCont_Reset (0x139a878): lift_d1's text (the reference)
void ColCont_Reset(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t a0 = Ps2GprU32(ctx, 4), ra = Ps2GprU32(ctx, 31);
    WRITE64(a0, 0ULL);
    {
        const uint32_t target = ra;
        WRITE32_B(a0, 8, 0u);
        
        ctx->pc = target;
        return;
    }
}

// ColCont2_IsColStatus (0x139aa00): lift_d1's text (the reference)
void ColCont2_IsColStatus(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t a0 = Ps2GprU32(ctx, 4), ra = Ps2GprU32(ctx, 31);
    uint64_t v0 = Ps2GprU64(ctx, 2), v1 = Ps2GprU64(ctx, 3), a1 = Ps2GprU64(ctx, 5), a2 = Ps2GprU64(ctx, 6);
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 4294967295))));
    if ((int64_t)a2 != (int64_t)v0) {
        (v0 = (uint64_t)(READ64_B(a0, 0)));
    } else {
        if ((int64_t)a1 != (int64_t)a2) {
            (v1 = (uint64_t)(READ64_B(a0, 0)));
            goto L_139aa38;
        }
        {
            const uint32_t target = ra;
            (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 1))));
            Ps2SetGpr64(ctx, 2, v0);
            ctx->pc = target;
            return;
        }
    }
    (v0 = (uint64_t)(v0 & a1));
    if ((int64_t)v0 != (int64_t)a2) {
        (v0 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL));
        goto L_139aa48;
    }
    {
        const uint32_t target = ra;
        (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 1))));
        Ps2SetGpr64(ctx, 2, v0);
        ctx->pc = target;
        return;
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
        const uint32_t target = ra;
        Ps2SetGpr64(ctx, 2, v0); Ps2SetGpr64(ctx, 3, v1);
        ctx->pc = target;
        return;
    }
}

// ColCont2_IsEventColStatus (0x139aa50): lift_d1's text (the reference)
void ColCont2_IsEventColStatus(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t a0 = Ps2GprU32(ctx, 4), ra = Ps2GprU32(ctx, 31);
    uint64_t v0 = Ps2GprU64(ctx, 2), v1 = Ps2GprU64(ctx, 3), a1 = Ps2GprU64(ctx, 5), a2 = Ps2GprU64(ctx, 6);
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 4294967295))));
    if ((int64_t)a2 != (int64_t)v0) {
        (v0 = (uint64_t)(READ64_B(a0, 8)));
    } else {
        if ((int64_t)a1 != (int64_t)a2) {
            (v1 = (uint64_t)(READ64_B(a0, 8)));
            goto L_139aa88;
        }
        {
            const uint32_t target = ra;
            (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 1))));
            Ps2SetGpr64(ctx, 2, v0);
            ctx->pc = target;
            return;
        }
    }
    (v0 = (uint64_t)(v0 & a1));
    if ((int64_t)v0 != (int64_t)a2) {
        (v0 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL));
        goto L_139aa98;
    }
    {
        const uint32_t target = ra;
        (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 1))));
        Ps2SetGpr64(ctx, 2, v0);
        ctx->pc = target;
        return;
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
        const uint32_t target = ra;
        Ps2SetGpr64(ctx, 2, v0); Ps2SetGpr64(ctx, 3, v1);
        ctx->pc = target;
        return;
    }
}

// MotSpdCtrl_Reset (0x139ac08): lift_d1's text (the reference)
void MotSpdCtrl_Reset(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t a0 = Ps2GprU32(ctx, 4), ra = Ps2GprU32(ctx, 31);
    WRITE8_B(a0, 17, (uint8_t)0u);
    WRITE8_B(a0, 16, (uint8_t)0u);
    {
        const uint32_t target = ra;
        WRITE32(a0, 0u);
        
        ctx->pc = target;
        return;
    }
}

// MotSpdCtrl_SetParam (0x139ac60): lift_d1's text (the reference)
void MotSpdCtrl_SetParam(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t a0 = Ps2GprU32(ctx, 4), a1 = Ps2GprU32(ctx, 5), ra = Ps2GprU32(ctx, 31);
    {
        const uint32_t target = ra;
        WRITE32_B(a0, 20, a1);
        
        ctx->pc = target;
        return;
    }
}

// inInNowLoadingEffect (0x139e988): lift_d1's text (the reference)
void inInNowLoadingEffect(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t v0 = Ps2GprU32(ctx, 2), v1 = Ps2GprU32(ctx, 3), ra = Ps2GprU32(ctx, 31);
    v1 = 0x1460000u;
    {
        const uint32_t target = ra;
        v0 = READ32_B(v1, 26384);
        Ps2SetGpr64(ctx, 2, sx64(v0)); Ps2SetGpr64(ctx, 3, sx64(v1));
        ctx->pc = target;
        return;
    }
}

// DirectTurn_Reset (0x13a75f8): lift_d1's text (the reference)
void DirectTurn_Reset(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t a0 = Ps2GprU32(ctx, 4), ra = Ps2GprU32(ctx, 31);
    WRITE32_B(a0, 16, 0u);
    {
        const uint32_t target = ra;
        WRITE8_B(a0, 20, (uint8_t)0u);
        
        ctx->pc = target;
        return;
    }
}

// is_doing_nico_sight_on (0x13aa380): lift_d1's text (the reference)
void is_doing_nico_sight_on(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t v0 = Ps2GprU32(ctx, 2), a0 = Ps2GprU32(ctx, 4), ra = Ps2GprU32(ctx, 31);
    v0 = READ32(a0);
    {
        const uint32_t target = ra;
        v0 = 0u < v0 ? 1u : 0u;
        Ps2SetGpr64(ctx, 2, sx64(v0));
        ctx->pc = target;
        return;
    }
}

// getPlayerLastAttrStatus (0x13b53b0): lift_d1's text (the reference)
void getPlayerLastAttrStatus(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t v1 = Ps2GprU32(ctx, 3), ra = Ps2GprU32(ctx, 31);
    uint64_t v0 = Ps2GprU64(ctx, 2);
    v1 = 0x1460000u;
    {
        const uint32_t target = ra;
        (v0 = (uint64_t)(READ64_B(v1, 27072)));
        Ps2SetGpr64(ctx, 2, v0); Ps2SetGpr64(ctx, 3, sx64(v1));
        ctx->pc = target;
        return;
    }
}

// getPlayerLastParentObj (0x13b53c0): lift_d1's text (the reference)
void getPlayerLastParentObj(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t v0 = Ps2GprU32(ctx, 2), v1 = Ps2GprU32(ctx, 3), ra = Ps2GprU32(ctx, 31);
    v1 = 0x1460000u;
    {
        const uint32_t target = ra;
        v0 = READ32_B(v1, 27080);
        Ps2SetGpr64(ctx, 2, sx64(v0)); Ps2SetGpr64(ctx, 3, sx64(v1));
        ctx->pc = target;
        return;
    }
}

// nico_setPlayerDemoAdditionClothCollisionFloorTable1 (0x13ba418): lift_d1's text (the reference)
void nico_setPlayerDemoAdditionClothCollisionFloorTable1(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t v0 = Ps2GprU32(ctx, 2), v1 = Ps2GprU32(ctx, 3), a0 = Ps2GprU32(ctx, 4), a1 = Ps2GprU32(ctx, 5), a2 = Ps2GprU32(ctx, 6), a3 = Ps2GprU32(ctx, 7), t0 = Ps2GprU32(ctx, 8), ra = Ps2GprU32(ctx, 31);
    v0 = 0x13C0000u;
    t0 = 0x1460000u;
    v0 = v0 - 0x56D8u;
    a1 = 0x1460000u;
    a2 = 0x1460000u;
    a3 = 0x1460000u;
    v1 = 0x1460000u;
    a0 = 0x1460000u;
    WRITE32_B(v1, 27136, v0);
    WRITE32_B(t0, 27104, v0);
    WRITE32_B(a1, 27108, 0u);
    WRITE32_B(a2, 27112, 0u);
    WRITE32_B(a3, 27116, 0u);
    {
        const uint32_t target = ra;
        WRITE32_B(a0, 27140, 0u);
        Ps2SetGpr64(ctx, 2, sx64(v0)); Ps2SetGpr64(ctx, 3, sx64(v1)); Ps2SetGpr64(ctx, 4, sx64(a0)); Ps2SetGpr64(ctx, 5, sx64(a1)); Ps2SetGpr64(ctx, 6, sx64(a2)); Ps2SetGpr64(ctx, 7, sx64(a3)); Ps2SetGpr64(ctx, 8, sx64(t0));
        ctx->pc = target;
        return;
    }
}

// nico_setPlayerDemoAdditionClothCollisionRightArm (0x13ba458): lift_d1's text (the reference)
void nico_setPlayerDemoAdditionClothCollisionRightArm(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t at = Ps2GprU32(ctx, 1), v0 = Ps2GprU32(ctx, 2), v1 = Ps2GprU32(ctx, 3), a0 = Ps2GprU32(ctx, 4), a1 = Ps2GprU32(ctx, 5), a2 = Ps2GprU32(ctx, 6), a3 = Ps2GprU32(ctx, 7), t0 = Ps2GprU32(ctx, 8), t1 = Ps2GprU32(ctx, 9), t2 = Ps2GprU32(ctx, 10), ra = Ps2GprU32(ctx, 31);
    float f0 = ctx->f[0];
    v1 = 0x11E0000u;
    at = 0x3DCC0000u;
    at = at | 0xCCCCu;
    f0 = lift_d1::flt(at);
    v0 = 0x13C0000u;
    v1 = v1 + 0x7020u;
    v0 = v0 - 0x5538u;
    a1 = 0x1460000u;
    a2 = 0x1460000u;
    a3 = 0x1460000u;
    t0 = 0x1460000u;
    t1 = 0x1460000u;
    t2 = 0x1460000u;
    WRITE32_B(a1, 27104, v0);
    WRITE32_B(a2, 27108, v1);
    WRITE32(v1, a0);
    WRITE32_B(v1, 4, lift_d1::bits(f0));
    WRITE32_B(a3, 27112, 0u);
    WRITE32_B(t0, 27116, 0u);
    WRITE32_B(t1, 27136, 0u);
    {
        const uint32_t target = ra;
        WRITE32_B(t2, 27140, 0u);
        ctx->f[0] = f0; Ps2SetGpr64(ctx, 1, sx64(at)); Ps2SetGpr64(ctx, 10, sx64(t2)); Ps2SetGpr64(ctx, 2, sx64(v0)); Ps2SetGpr64(ctx, 3, sx64(v1)); Ps2SetGpr64(ctx, 5, sx64(a1)); Ps2SetGpr64(ctx, 6, sx64(a2)); Ps2SetGpr64(ctx, 7, sx64(a3)); Ps2SetGpr64(ctx, 8, sx64(t0)); Ps2SetGpr64(ctx, 9, sx64(t1));
        ctx->pc = target;
        return;
    }
}

// nico_setPlayerDemoAdditionClothCollisionRightHandAndSwordDeep (0x13ba4b0): lift_d1's text (the reference)
void nico_setPlayerDemoAdditionClothCollisionRightHandAndSwordDeep(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t at = Ps2GprU32(ctx, 1), v0 = Ps2GprU32(ctx, 2), v1 = Ps2GprU32(ctx, 3), a0 = Ps2GprU32(ctx, 4), a1 = Ps2GprU32(ctx, 5), a2 = Ps2GprU32(ctx, 6), a3 = Ps2GprU32(ctx, 7), t0 = Ps2GprU32(ctx, 8), t1 = Ps2GprU32(ctx, 9), t2 = Ps2GprU32(ctx, 10), ra = Ps2GprU32(ctx, 31);
    float f0 = ctx->f[0];
    v1 = 0x11E0000u;
    at = 0x3D8F0000u;
    at = at | 0x5C28u;
    f0 = lift_d1::flt(at);
    v0 = 0x13C0000u;
    v1 = v1 + 0x7020u;
    v0 = v0 - 0x5368u;
    a1 = 0x1460000u;
    a2 = 0x1460000u;
    a3 = 0x1460000u;
    t0 = 0x1460000u;
    t1 = 0x1460000u;
    t2 = 0x1460000u;
    WRITE32_B(a1, 27104, v0);
    WRITE32_B(a2, 27108, v1);
    WRITE32(v1, a0);
    WRITE32_B(v1, 4, lift_d1::bits(f0));
    WRITE32_B(a3, 27112, 0u);
    WRITE32_B(t0, 27116, 0u);
    WRITE32_B(t1, 27136, 0u);
    {
        const uint32_t target = ra;
        WRITE32_B(t2, 27140, 0u);
        ctx->f[0] = f0; Ps2SetGpr64(ctx, 1, sx64(at)); Ps2SetGpr64(ctx, 10, sx64(t2)); Ps2SetGpr64(ctx, 2, sx64(v0)); Ps2SetGpr64(ctx, 3, sx64(v1)); Ps2SetGpr64(ctx, 5, sx64(a1)); Ps2SetGpr64(ctx, 6, sx64(a2)); Ps2SetGpr64(ctx, 7, sx64(a3)); Ps2SetGpr64(ctx, 8, sx64(t0)); Ps2SetGpr64(ctx, 9, sx64(t1));
        ctx->pc = target;
        return;
    }
}

// nico_setPlayerDemoAdditionClothCollisionRightHandAndSword (0x13ba508): lift_d1's text (the reference)
void nico_setPlayerDemoAdditionClothCollisionRightHandAndSword(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t at = Ps2GprU32(ctx, 1), v0 = Ps2GprU32(ctx, 2), v1 = Ps2GprU32(ctx, 3), a0 = Ps2GprU32(ctx, 4), a1 = Ps2GprU32(ctx, 5), a2 = Ps2GprU32(ctx, 6), a3 = Ps2GprU32(ctx, 7), t0 = Ps2GprU32(ctx, 8), t1 = Ps2GprU32(ctx, 9), t2 = Ps2GprU32(ctx, 10), ra = Ps2GprU32(ctx, 31);
    float f0 = ctx->f[0];
    v1 = 0x11E0000u;
    at = 0x3CF50000u;
    at = at | 0xC28Fu;
    f0 = lift_d1::flt(at);
    v0 = 0x13C0000u;
    v1 = v1 + 0x7020u;
    v0 = v0 - 0x5368u;
    a1 = 0x1460000u;
    a2 = 0x1460000u;
    a3 = 0x1460000u;
    t0 = 0x1460000u;
    t1 = 0x1460000u;
    t2 = 0x1460000u;
    WRITE32_B(a1, 27104, v0);
    WRITE32_B(a2, 27108, v1);
    WRITE32(v1, a0);
    WRITE32_B(v1, 4, lift_d1::bits(f0));
    WRITE32_B(a3, 27112, 0u);
    WRITE32_B(t0, 27116, 0u);
    WRITE32_B(t1, 27136, 0u);
    {
        const uint32_t target = ra;
        WRITE32_B(t2, 27140, 0u);
        ctx->f[0] = f0; Ps2SetGpr64(ctx, 1, sx64(at)); Ps2SetGpr64(ctx, 10, sx64(t2)); Ps2SetGpr64(ctx, 2, sx64(v0)); Ps2SetGpr64(ctx, 3, sx64(v1)); Ps2SetGpr64(ctx, 5, sx64(a1)); Ps2SetGpr64(ctx, 6, sx64(a2)); Ps2SetGpr64(ctx, 7, sx64(a3)); Ps2SetGpr64(ctx, 8, sx64(t0)); Ps2SetGpr64(ctx, 9, sx64(t1));
        ctx->pc = target;
        return;
    }
}

// nico_resetPlayerDemoAdditionClothCollision (0x13ba768): lift_d1's text (the reference)
void nico_resetPlayerDemoAdditionClothCollision(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t v0 = Ps2GprU32(ctx, 2), v1 = Ps2GprU32(ctx, 3), a0 = Ps2GprU32(ctx, 4), a1 = Ps2GprU32(ctx, 5), ra = Ps2GprU32(ctx, 31);
    a1 = 0x1460000u;
    v0 = 0x1460000u;
    v1 = 0x1460000u;
    a0 = 0x1460000u;
    WRITE32_B(v0, 27112, 0u);
    WRITE32_B(a1, 27104, 0u);
    WRITE32_B(v1, 27136, 0u);
    {
        const uint32_t target = ra;
        WRITE32_B(a0, 27140, 0u);
        Ps2SetGpr64(ctx, 2, sx64(v0)); Ps2SetGpr64(ctx, 3, sx64(v1)); Ps2SetGpr64(ctx, 4, sx64(a0)); Ps2SetGpr64(ctx, 5, sx64(a1));
        ctx->pc = target;
        return;
    }
}

// player_initSlipEffect (0x13cd5f8): lift_d1's text (the reference)
void player_initSlipEffect(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t v0 = Ps2GprU32(ctx, 2), v1 = Ps2GprU32(ctx, 3), a0 = Ps2GprU32(ctx, 4), a1 = Ps2GprU32(ctx, 5), ra = Ps2GprU32(ctx, 31);
    v0 = 0x11E0000u;
    v1 = 1u;
    v0 = v0 + 0x70E8u;
    a0 = 2u;
    WRITE64_B(a1, 8, 0ULL);
    WRITE32(a1, 0u);
    WRITE32_B(v0, 28, v1);
    WRITE32_B(v0, 52, a0);
    WRITE32_B(v0, 56, 0u);
    WRITE32(v0, 0u);
    WRITE32_B(v0, 4, 0u);
    WRITE32_B(v0, 8, 0u);
    WRITE32_B(v0, 12, 0u);
    WRITE32_B(v0, 32, 0u);
    WRITE32_B(v0, 16, 0u);
    WRITE32_B(v0, 20, 0u);
    WRITE32_B(v0, 24, 0u);
    WRITE32_B(v0, 36, 0u);
    WRITE32_B(v0, 40, 0u);
    WRITE32_B(v0, 44, 0u);
    {
        const uint32_t target = ra;
        WRITE32_B(v0, 48, 0u);
        Ps2SetGpr64(ctx, 2, sx64(v0)); Ps2SetGpr64(ctx, 3, sx64(v1)); Ps2SetGpr64(ctx, 4, sx64(a0));
        ctx->pc = target;
        return;
    }
}

// bossCloudStart (0x13d0ab8): lift_d1's text (the reference)
void bossCloudStart(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t v0 = Ps2GprU32(ctx, 2), v1 = Ps2GprU32(ctx, 3), ra = Ps2GprU32(ctx, 31);
    v1 = 0x1460000u;
    v0 = 1u;
    {
        const uint32_t target = ra;
        WRITE32_B(v1, 27316, v0);
        Ps2SetGpr64(ctx, 2, sx64(v0)); Ps2SetGpr64(ctx, 3, sx64(v1));
        ctx->pc = target;
        return;
    }
}

// bossDemoPlayerBreakDownAccept (0x13d0bc0): lift_d1's text (the reference)
void bossDemoPlayerBreakDownAccept(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t v0 = Ps2GprU32(ctx, 2), v1 = Ps2GprU32(ctx, 3), ra = Ps2GprU32(ctx, 31);
    v1 = 0x1460000u;
    v0 = 1u;
    {
        const uint32_t target = ra;
        WRITE32_B(v1, 27324, v0);
        Ps2SetGpr64(ctx, 2, sx64(v0)); Ps2SetGpr64(ctx, 3, sx64(v1));
        ctx->pc = target;
        return;
    }
}

// bossDemoPlayerBreakDownEnd (0x13d0bd0): lift_d1's text (the reference)
void bossDemoPlayerBreakDownEnd(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t v0 = Ps2GprU32(ctx, 2), v1 = Ps2GprU32(ctx, 3), ra = Ps2GprU32(ctx, 31);
    v1 = 0x1460000u;
    v0 = 2u;
    {
        const uint32_t target = ra;
        WRITE32_B(v1, 27324, v0);
        Ps2SetGpr64(ctx, 2, sx64(v0)); Ps2SetGpr64(ctx, 3, sx64(v1));
        ctx->pc = target;
        return;
    }
}

// AI_InitAnimalOptimize (0x13d38a0): lift_d1's text (the reference)
void AI_InitAnimalOptimize(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t at = Ps2GprU32(ctx, 1), v0 = Ps2GprU32(ctx, 2), a0 = Ps2GprU32(ctx, 4), ra = Ps2GprU32(ctx, 31);
    float f0 = ctx->f[0];
    at = 0x41F00000u;
    f0 = lift_d1::flt(at);
    v0 = 1u;
    WRITE32_B(a0, 8, 0u);
    WRITE32(a0, v0);
    {
        const uint32_t target = ra;
        WRITE32_B(a0, 4, lift_d1::bits(f0));
        ctx->f[0] = f0; Ps2SetGpr64(ctx, 1, sx64(at)); Ps2SetGpr64(ctx, 2, sx64(v0));
        ctx->pc = target;
        return;
    }
}

// AI_InitAnimalWork (0x13d3a48): lift_d1's text (the reference)
void AI_InitAnimalWork(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t v0 = Ps2GprU32(ctx, 2), a0 = Ps2GprU32(ctx, 4), a1 = Ps2GprU32(ctx, 5), ra = Ps2GprU32(ctx, 31);
    v0 = 0xFFFFFFFFu;
    WRITE32(a0, a1);
    WRITE32_B(a0, 8, v0);
    WRITE32_B(a0, 16, 0u);
    WRITE32_B(a0, 4, v0);
    {
        const uint32_t target = ra;
        WRITE32_B(a0, 12, 0u);
        Ps2SetGpr64(ctx, 2, sx64(v0));
        ctx->pc = target;
        return;
    }
}

// AI_SetAnimalWorkParam (0x13d3a68): lift_d1's text (the reference)
void AI_SetAnimalWorkParam(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t a0 = Ps2GprU32(ctx, 4), a1 = Ps2GprU32(ctx, 5), ra = Ps2GprU32(ctx, 31);
    {
        const uint32_t target = ra;
        WRITE32_B(a0, 16, a1);
        
        ctx->pc = target;
        return;
    }
}

// AI_InitGexCommonWork (0x13d3a70): lift_d1's text (the reference)
void AI_InitGexCommonWork(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t a0 = Ps2GprU32(ctx, 4), ra = Ps2GprU32(ctx, 31);
    WRITE32_B(a0, 12, 0u);
    WRITE32_B(a0, 8, 0u);
    WRITE32(a0, 0u);
    {
        const uint32_t target = ra;
        WRITE32_B(a0, 4, 0u);
        
        ctx->pc = target;
        return;
    }
}

// AI_InitSurpriseArrow (0x13d3a88): lift_d1's text (the reference)
void AI_InitSurpriseArrow(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t a0 = Ps2GprU32(ctx, 4), ra = Ps2GprU32(ctx, 31);
    WRITE32_B(a0, 4, 0u);
    {
        const uint32_t target = ra;
        WRITE32(a0, 0u);
        
        ctx->pc = target;
        return;
    }
}

// AI_SetSurprisedArea (0x13d3a98): lift_d1's text (the reference)
void AI_SetSurprisedArea(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t a0 = Ps2GprU32(ctx, 4), ra = Ps2GprU32(ctx, 31);
    float f12 = ctx->f[12];
    {
        const uint32_t target = ra;
        WRITE32_B(a0, 4, lift_d1::bits(f12));
        
        ctx->pc = target;
        return;
    }
}

// AI_UpdateVelocity (0x13d3da0): lift_d1's text (the reference)
void AI_UpdateVelocity(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t v0 = Ps2GprU32(ctx, 2), v1 = Ps2GprU32(ctx, 3), a0 = Ps2GprU32(ctx, 4), ra = Ps2GprU32(ctx, 31);
    float f0 = ctx->f[0], f1 = ctx->f[1], f12 = ctx->f[12], f13 = ctx->f[13];
    uint32_t fcr31 = ctx->fcr31;
    f1 = lift_d1::flt(READ32(a0));
    fcr31 = (FPU_C_OLT_S(f1, f12)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
    {
        const bool c = (fcr31 & 0x800000u) == 0u;
        v1 = 1u;
        if (!c) {
            v0 = 0x1460000u;
            f0 = lift_d1::flt(READ32_B(v0, 22496));
            f0 = FPU_MUL_S(f13, f0);
            f1 = FPU_ADD_S(f1, f0);
            fcr31 = (FPU_C_OLT_S(f12, f1)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
            goto L_13d3de4;
        }
    }
    v0 = 0x1460000u;
    f0 = lift_d1::flt(READ32_B(v0, 22496));
    f0 = FPU_MUL_S(f13, f0);
    f1 = FPU_SUB_S(f1, f0);
    fcr31 = (FPU_C_OLT_S(f1, f12)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
L_13d3de4:
    if ((fcr31 & 0x800000u) == 0u) {
        WRITE32(a0, lift_d1::bits(f1));
    } else {
        f1 = FPU_MOV_S(f12);
        v1 = 0u;
        WRITE32(a0, lift_d1::bits(f1));
    }
    {
        const uint32_t target = ra;
        v0 = v1;
        ctx->f[0] = f0; ctx->f[1] = f1; Ps2SetGpr64(ctx, 2, sx64(v0)); Ps2SetGpr64(ctx, 3, sx64(v1)); ctx->fcr31 = fcr31;
        ctx->pc = target;
        return;
    }
}

// AI_GetAccelB (0x13d4008): lift_d1's text (the reference)
void AI_GetAccelB(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t at = Ps2GprU32(ctx, 1), ra = Ps2GprU32(ctx, 31);
    float f0 = ctx->f[0], f1 = ctx->f[1], f2 = ctx->f[2], f3 = ctx->f[3], f12 = ctx->f[12], f13 = ctx->f[13], f14 = ctx->f[14], f15 = ctx->f[15];
    uint32_t fcr31 = ctx->fcr31;
    at = 0x3D080000u;
    at = at | 0xAB85u;
    f0 = lift_d1::flt(at);
    f2 = FPU_MOV_S(f0);
    fcr31 = (FPU_C_OLT_S(f15, f2)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
    {
        const bool c = (fcr31 & 0x800000u) != 0u;
        f3 = FPU_SUB_S(f12, f13);
        if (!c) {
            f2 = FPU_MOV_S(f15);
        }
    }
    f0 = ps2FpuDivide(fcr31, f14, f2);
    at = 0x40400000u;
    f1 = lift_d1::flt(at);
    f0 = FPU_SUB_S(f12, f0);
    f0 = FPU_MUL_SF(f0, f1);
    f0 = FPU_SUB_S(f3, f0);
    f0 = FPU_ADD_SF(f0, f0);
    f0 = ps2FpuDivide(fcr31, f0, f2);
    {
        const uint32_t target = ra;
        ctx->f[0] = f0; ctx->f[1] = f1; ctx->f[2] = f2; ctx->f[3] = f3; Ps2SetGpr64(ctx, 1, sx64(at)); ctx->fcr31 = fcr31;
        ctx->pc = target;
        return;
    }
}

// AI_GetAccelA (0x13d4060): lift_d1's text (the reference)
void AI_GetAccelA(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t at = Ps2GprU32(ctx, 1), ra = Ps2GprU32(ctx, 31);
    float f0 = ctx->f[0], f1 = ctx->f[1], f12 = ctx->f[12], f13 = ctx->f[13], f14 = ctx->f[14], f15 = ctx->f[15];
    uint32_t fcr31 = ctx->fcr31;
    at = 0x3D080000u;
    at = at | 0xAB85u;
    f0 = lift_d1::flt(at);
    f1 = FPU_MOV_S(f0);
    fcr31 = (FPU_C_OLT_S(f14, f1)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
    {
        const bool c = (fcr31 & 0x800000u) != 0u;
        f13 = FPU_SUB_S(f13, f12);
        if (!c) {
            f1 = FPU_MOV_S(f14);
        }
    }
    f0 = FPU_MUL_S(f15, f1);
    f1 = FPU_MUL_S(f1, f1);
    f0 = FPU_SUB_S(f13, f0);
    f0 = FPU_ADD_SF(f0, f0);
    f0 = ps2FpuDivide(fcr31, f0, f1);
    {
        const uint32_t target = ra;
        ctx->f[0] = f0; ctx->f[1] = f1; ctx->f[13] = f13; Ps2SetGpr64(ctx, 1, sx64(at)); ctx->fcr31 = fcr31;
        ctx->pc = target;
        return;
    }
}

// AI_GetAccelA_at0 (0x13d40a8): lift_d1's text (the reference)
void AI_GetAccelA_at0(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t at = Ps2GprU32(ctx, 1), ra = Ps2GprU32(ctx, 31);
    float f0 = ctx->f[0], f1 = ctx->f[1], f12 = ctx->f[12], f13 = ctx->f[13], f14 = ctx->f[14];
    uint32_t fcr31 = ctx->fcr31;
    at = 0x3D080000u;
    at = at | 0xAB85u;
    f0 = lift_d1::flt(at);
    f1 = FPU_MOV_S(f0);
    fcr31 = (FPU_C_OLT_S(f14, f1)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
    {
        const bool c = (fcr31 & 0x800000u) != 0u;
        f13 = FPU_SUB_S(f13, f12);
        if (!c) {
            f1 = FPU_MOV_S(f14);
        }
    }
    at = 0xC0000000u;
    f0 = lift_d1::flt(at);
    f1 = FPU_MUL_S(f1, f1);
    f0 = FPU_MUL_S(f13, f0);
    f0 = ps2FpuDivide(fcr31, f0, f1);
    {
        const uint32_t target = ra;
        ctx->f[0] = f0; ctx->f[1] = f1; ctx->f[13] = f13; Ps2SetGpr64(ctx, 1, sx64(at)); ctx->fcr31 = fcr31;
        ctx->pc = target;
        return;
    }
}

// AI_GetAccelB_at0 (0x13d40f0): lift_d1's text (the reference)
void AI_GetAccelB_at0(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t at = Ps2GprU32(ctx, 1), ra = Ps2GprU32(ctx, 31);
    float f0 = ctx->f[0], f12 = ctx->f[12], f13 = ctx->f[13], f14 = ctx->f[14];
    uint32_t fcr31 = ctx->fcr31;
    at = 0x3D080000u;
    at = at | 0xAB85u;
    f0 = lift_d1::flt(at);
    f13 = FPU_SUB_S(f13, f12);
    fcr31 = (FPU_C_OLT_S(f14, f0)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
    {
        const bool c = (fcr31 & 0x800000u) != 0u;
        f13 = FPU_ADD_S(f13, f13);
        if (!c) {
            f0 = FPU_MOV_S(f14);
        }
    }
    f0 = ps2FpuDivide(fcr31, f13, f0);
    {
        const uint32_t target = ra;
        ctx->f[0] = f0; ctx->f[13] = f13; Ps2SetGpr64(ctx, 1, sx64(at)); ctx->fcr31 = fcr31;
        ctx->pc = target;
        return;
    }
}

// AI_GetAccelA3 (0x13d4128): lift_d1's text (the reference)
void AI_GetAccelA3(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t at = Ps2GprU32(ctx, 1), ra = Ps2GprU32(ctx, 31);
    float f0 = ctx->f[0], f1 = ctx->f[1], f2 = ctx->f[2], f3 = ctx->f[3], f4 = ctx->f[4], f12 = ctx->f[12], f13 = ctx->f[13], f14 = ctx->f[14], f15 = ctx->f[15], f16 = ctx->f[16], f17 = ctx->f[17];
    uint32_t fcr31 = ctx->fcr31;
    at = 0x3D080000u;
    at = at | 0xAB85u;
    f0 = lift_d1::flt(at);
    f4 = FPU_SUB_S(f14, f13);
    fcr31 = (FPU_C_OLT_S(f16, f0)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
    {
        const bool c = (fcr31 & 0x800000u) == 0u;
        f14 = FPU_ADD_S(f17, f17);
        if (!c) {
            f16 = FPU_MOV_S(f0);
        }
    }
    f3 = FPU_MUL_S(f16, f16);
    at = 0x40C00000u;
    f0 = lift_d1::flt(at);
    at = 0xC0000000u;
    f2 = lift_d1::flt(at);
    f1 = ps2FpuDivide(fcr31, f0, f16);
    f0 = ps2FpuDivide(fcr31, f0, f3);
    f1 = FPU_MUL_S(f1, f12);
    f2 = ps2FpuDivide(fcr31, f2, f3);
    f0 = FPU_MUL_SF(f0, f4);
    f0 = FPU_SUB_S(f0, f14);
    f0 = FPU_SUB_SF(f0, f1);
    f0 = FPU_SUB_S(f0, f15);
    {
        const uint32_t target = ra;
        f0 = FPU_MUL_S(f2, f0);
        ctx->f[0] = f0; ctx->f[1] = f1; ctx->f[14] = f14; ctx->f[16] = f16; ctx->f[2] = f2; ctx->f[3] = f3; ctx->f[4] = f4; Ps2SetGpr64(ctx, 1, sx64(at)); ctx->fcr31 = fcr31;
        ctx->pc = target;
        return;
    }
}

// AI_GetAccelB3 (0x13d4298): lift_d1's text (the reference)
void AI_GetAccelB3(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t at = Ps2GprU32(ctx, 1), ra = Ps2GprU32(ctx, 31);
    float f0 = ctx->f[0], f1 = ctx->f[1], f2 = ctx->f[2], f3 = ctx->f[3], f4 = ctx->f[4], f12 = ctx->f[12], f13 = ctx->f[13], f14 = ctx->f[14], f15 = ctx->f[15], f16 = ctx->f[16], f17 = ctx->f[17];
    uint32_t fcr31 = ctx->fcr31;
    at = 0x3D080000u;
    at = at | 0xAB85u;
    f0 = lift_d1::flt(at);
    fcr31 = (FPU_C_OLT_S(f16, f0)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
    {
        const bool c = (fcr31 & 0x800000u) == 0u;
        f15 = FPU_SUB_S(f15, f14);
        if (!c) {
            f16 = FPU_MOV_S(f0);
        }
    }
    at = 0x40800000u;
    f0 = lift_d1::flt(at);
    f3 = FPU_MUL_S(f16, f16);
    at = 0x40400000u;
    f2 = lift_d1::flt(at);
    f4 = FPU_MUL_S(f17, f16);
    f0 = ps2FpuDivide(fcr31, f0, f16);
    at = 0x40C00000u;
    f1 = lift_d1::flt(at);
    f2 = FPU_MUL_S(f12, f2);
    f1 = ps2FpuDivide(fcr31, f1, f3);
    f0 = FPU_MUL_S(f0, f15);
    f0 = FPU_SUB_S(f0, f13);
    f0 = FPU_SUB_SF(f0, f2);
    f0 = FPU_SUB_SF(f0, f4);
    {
        const uint32_t target = ra;
        f0 = FPU_MUL_S(f1, f0);
        ctx->f[0] = f0; ctx->f[1] = f1; ctx->f[15] = f15; ctx->f[16] = f16; ctx->f[2] = f2; ctx->f[3] = f3; ctx->f[4] = f4; Ps2SetGpr64(ctx, 1, sx64(at)); ctx->fcr31 = fcr31;
        ctx->pc = target;
        return;
    }
}

// AI_GetAccelC3 (0x13d43f0): lift_d1's text (the reference)
void AI_GetAccelC3(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t at = Ps2GprU32(ctx, 1), ra = Ps2GprU32(ctx, 31);
    float f0 = ctx->f[0], f1 = ctx->f[1], f2 = ctx->f[2], f12 = ctx->f[12], f13 = ctx->f[13], f14 = ctx->f[14], f15 = ctx->f[15], f16 = ctx->f[16], f17 = ctx->f[17];
    uint32_t fcr31 = ctx->fcr31;
    at = 0x3D080000u;
    at = at | 0xAB85u;
    f0 = lift_d1::flt(at);
    f13 = FPU_ADD_S(f13, f12);
    fcr31 = (FPU_C_OLT_S(f17, f0)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
    {
        const bool c = (fcr31 & 0x800000u) == 0u;
        f15 = FPU_SUB_S(f15, f14);
        if (!c) {
            f17 = FPU_MOV_S(f0);
        }
    }
    at = 0x41400000u;
    f0 = lift_d1::flt(at);
    f2 = FPU_MUL_S(f16, f17);
    at = 0x40C00000u;
    f1 = lift_d1::flt(at);
    f0 = ps2FpuDivide(fcr31, f0, f17);
    f1 = FPU_MUL_SF(f13, f1);
    f0 = FPU_MUL_S(f0, f15);
    f0 = FPU_SUB_SF(f0, f1);
    f0 = FPU_ADD_SF(f0, f2);
    f0 = ps2FpuDivide(fcr31, f0, f17);
    {
        const uint32_t target = ra;
        ctx->f[0] = f0; ctx->f[1] = f1; ctx->f[13] = f13; ctx->f[15] = f15; ctx->f[17] = f17; ctx->f[2] = f2; Ps2SetGpr64(ctx, 1, sx64(at)); ctx->fcr31 = fcr31;
        ctx->pc = target;
        return;
    }
}

// AI_GetTimeFromAccelC3 (0x13d4540): lift_d1's text (the reference)
void AI_GetTimeFromAccelC3(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t at = Ps2GprU32(ctx, 1), ra = Ps2GprU32(ctx, 31);
    float f0 = ctx->f[0], f1 = ctx->f[1], f2 = ctx->f[2], f12 = ctx->f[12], f13 = ctx->f[13], f14 = ctx->f[14], f15 = ctx->f[15];
    uint32_t fcr31 = ctx->fcr31;
    at = 0x3DCC0000u;
    at = at | 0xCCCCu;
    f0 = lift_d1::flt(at);
    fcr31 = (FPU_C_OLT_S(f15, f0)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
    {
        const bool c = (fcr31 & 0x800000u) == 0u;
        f13 = FPU_ADD_S(f13, f12);
        if (!c) {
            f15 = FPU_MOV_S(f0);
        }
    }
    f1 = lift_d1::flt(0u);
    fcr31 = (FPU_C_OLT_S(f14, f1)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
    {
        const bool c = (fcr31 & 0x800000u) == 0u;
        f0 = FPU_MOV_S(f1);
        if (!c) {
            f14 = FPU_NEG_S(f14);
        }
    }
    at = 0x41100000u;
    f2 = lift_d1::flt(at);
    at = 0x41400000u;
    f1 = lift_d1::flt(at);
    f2 = FPU_MUL_S(f13, f2);
    f1 = FPU_MUL_S(f15, f1);
    f2 = FPU_MUL_S(f2, f13);
    f1 = FPU_MUL_S(f1, f14);
    f2 = FPU_ADD_SF(f2, f1);
    fcr31 = (FPU_C_OLT_S(f2, f0)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
    if ((fcr31 & 0x800000u) == 0u) {
        f1 = ps2FpuSqrt(fcr31, f2);
        at = 0xC0400000u;
        f0 = lift_d1::flt(at);
        f0 = FPU_MUL_S(f13, f0);
        f0 = FPU_ADD_SF(f0, f1);
        f0 = ps2FpuDivide(fcr31, f0, f15);
    }
    {
        const uint32_t target = ra;
        ctx->f[0] = f0; ctx->f[1] = f1; ctx->f[13] = f13; ctx->f[14] = f14; ctx->f[15] = f15; ctx->f[2] = f2; Ps2SetGpr64(ctx, 1, sx64(at)); ctx->fcr31 = fcr31;
        ctx->pc = target;
        return;
    }
}

// AI_GetVelocity3 (0x13d45f0): lift_d1's text (the reference)
void AI_GetVelocity3(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t at = Ps2GprU32(ctx, 1), ra = Ps2GprU32(ctx, 31);
    float f0 = ctx->f[0], f1 = ctx->f[1], f12 = ctx->f[12], f13 = ctx->f[13], f14 = ctx->f[14], f15 = ctx->f[15], f16 = ctx->f[16];
    uint32_t fcr31 = ctx->fcr31;
    at = 0x40400000u;
    f1 = lift_d1::flt(at);
    f14 = FPU_MUL_S(f14, f16);
    at = 0x3F000000u;
    f0 = lift_d1::flt(at);
    f12 = ps2FpuDivide(fcr31, f12, f1);
    f13 = FPU_MUL_S(f13, f0);
    f13 = FPU_MUL_S(f13, f16);
    f12 = FPU_MUL_S(f12, f16);
    f13 = FPU_MUL_S(f13, f16);
    f12 = FPU_MUL_S(f12, f16);
    f12 = FPU_MUL_S(f12, f16);
    f0 = FPU_ADD_S(f15, f12);
    f0 = FPU_ADD_SF(f0, f13);
    {
        const uint32_t target = ra;
        f0 = FPU_ADD_S(f0, f14);
        ctx->f[0] = f0; ctx->f[1] = f1; ctx->f[12] = f12; ctx->f[13] = f13; ctx->f[14] = f14; Ps2SetGpr64(ctx, 1, sx64(at)); ctx->fcr31 = fcr31;
        ctx->pc = target;
        return;
    }
}

// AI_GetAreaDistScale (0x13d4ce0): lift_d1's text (the reference)
void AI_GetAreaDistScale(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t ra = Ps2GprU32(ctx, 31);
    float f0 = ctx->f[0], f12 = ctx->f[12], f13 = ctx->f[13];
    uint32_t fcr31 = ctx->fcr31;
    fcr31 = (FPU_C_OLT_S(f12, f13)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
    {
        const bool c = (fcr31 & 0x800000u) != 0u;
        f0 = FPU_MOV_S(f12);
        if (!c) {
            f0 = FPU_MOV_S(f13);
        }
    }
    f0 = ps2FpuDivide(fcr31, f0, f12);
    {
        const uint32_t target = ra;
        f0 = FPU_MUL_S(f0, f0);
        ctx->f[0] = f0; ctx->fcr31 = fcr31;
        ctx->pc = target;
        return;
    }
}

// AI_GetAreaDistScale2 (0x13d4d08): lift_d1's text (the reference)
void AI_GetAreaDistScale2(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t ra = Ps2GprU32(ctx, 31);
    float f0 = ctx->f[0], f12 = ctx->f[12], f13 = ctx->f[13];
    uint32_t fcr31 = ctx->fcr31;
    fcr31 = (FPU_C_OLT_S(f12, f13)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
    {
        const bool c = (fcr31 & 0x800000u) != 0u;
        f0 = FPU_MOV_S(f12);
        if (!c) {
            f0 = FPU_MOV_S(f13);
        }
    }
    f0 = ps2FpuDivide(fcr31, f0, f12);
    {
        const uint32_t target = ra;
        ctx->f[0] = f0; ctx->fcr31 = fcr31;
        ctx->pc = target;
        return;
    }
}

// AI_GetNegaVector (0x13d5300): lift_d1's text (the reference)
void AI_GetNegaVector(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t a0 = Ps2GprU32(ctx, 4), a1 = Ps2GprU32(ctx, 5), ra = Ps2GprU32(ctx, 31);
    float f0 = ctx->f[0], f1 = ctx->f[1];
    f0 = lift_d1::flt(READ32(a1));
    f0 = FPU_NEG_S(f0);
    WRITE32(a0, lift_d1::bits(f0));
    f1 = lift_d1::flt(READ32_B(a1, 4));
    f1 = FPU_NEG_S(f1);
    WRITE32_B(a0, 4, lift_d1::bits(f1));
    f0 = lift_d1::flt(READ32_B(a1, 8));
    f0 = FPU_NEG_S(f0);
    WRITE32_B(a0, 8, lift_d1::bits(f0));
    f1 = lift_d1::flt(READ32_B(a1, 12));
    {
        const uint32_t target = ra;
        WRITE32_B(a0, 12, lift_d1::bits(f1));
        ctx->f[0] = f0; ctx->f[1] = f1;
        ctx->pc = target;
        return;
    }
}

// Kite_SetAnimalID_KitePathWork (0x13e7c00): lift_d1's text (the reference)
void Kite_SetAnimalID_KitePathWork(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t a0 = Ps2GprU32(ctx, 4), a1 = Ps2GprU32(ctx, 5), ra = Ps2GprU32(ctx, 31);
    {
        const uint32_t target = ra;
        WRITE32(a0, a1);
        
        ctx->pc = target;
        return;
    }
}

// Kite_SetEventIDList (0x13e7c08): lift_d1's text (the reference)
void Kite_SetEventIDList(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t v0 = Ps2GprU32(ctx, 2), a0 = Ps2GprU32(ctx, 4), a1 = Ps2GprU32(ctx, 5), ra = Ps2GprU32(ctx, 31);
    uint64_t a2 = Ps2GprU64(ctx, 6);
    (v0 = (uint32_t)((int32_t)((int32_t)SLL32((uint32_t)a2, 2))));
    (a2 = (uint64_t)(((uint64_t)a2 < (uint64_t)(int64_t)(int32_t)10) ? 1 : 0));
    {
        const bool c = (int64_t)a2 == 0;
        v0 = v0 + a0;
        if (!c) {
            WRITE32_B(v0, 444, a1);
        }
    }
    {
        const uint32_t target = ra;
        Ps2SetGpr64(ctx, 2, sx64(v0)); Ps2SetGpr64(ctx, 6, a2);
        ctx->pc = target;
        return;
    }
}

// Kite_InitEventCheck (0x13e7c28): lift_d1's text (the reference)
void Kite_InitEventCheck(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t v0 = Ps2GprU32(ctx, 2), a0 = Ps2GprU32(ctx, 4), ra = Ps2GprU32(ctx, 31);
    v0 = 0xFFFFFFFFu;
    WRITE32_B(a0, 12, 0u);
    WRITE32(a0, v0);
    WRITE32_B(a0, 4, 0u);
    {
        const uint32_t target = ra;
        WRITE32_B(a0, 8, 0u);
        Ps2SetGpr64(ctx, 2, sx64(v0));
        ctx->pc = target;
        return;
    }
}

// Kite_ResetEventFrameCheck (0x13e7c40): lift_d1's text (the reference)
void Kite_ResetEventFrameCheck(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t a0 = Ps2GprU32(ctx, 4), ra = Ps2GprU32(ctx, 31);
    WRITE32_B(a0, 12, 0u);
    {
        const uint32_t target = ra;
        WRITE32_B(a0, 8, 0u);
        
        ctx->pc = target;
        return;
    }
}

// Kite_StartEventCheck (0x13e7c50): lift_d1's text (the reference)
void Kite_StartEventCheck(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t v0 = Ps2GprU32(ctx, 2), a0 = Ps2GprU32(ctx, 4), a1 = Ps2GprU32(ctx, 5), ra = Ps2GprU32(ctx, 31);
    v0 = 1u;
    WRITE32(a0, a1);
    WRITE32_B(a0, 8, v0);
    {
        const uint32_t target = ra;
        WRITE32_B(a0, 4, 0u);
        Ps2SetGpr64(ctx, 2, sx64(v0));
        ctx->pc = target;
        return;
    }
}

// Kite_EndEventCheck (0x13e7c68): lift_d1's text (the reference)
void Kite_EndEventCheck(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t v0 = Ps2GprU32(ctx, 2), a0 = Ps2GprU32(ctx, 4), ra = Ps2GprU32(ctx, 31);
    v0 = 1u;
    WRITE32_B(a0, 12, v0);
    {
        const uint32_t target = ra;
        WRITE32_B(a0, 4, v0);
        Ps2SetGpr64(ctx, 2, sx64(v0));
        ctx->pc = target;
        return;
    }
}

// Kite_SucceedKitePathWork (0x13e7cf8): lift_d1's text (the reference)
void Kite_SucceedKitePathWork(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t a0 = Ps2GprU32(ctx, 4), a1 = Ps2GprU32(ctx, 5), ra = Ps2GprU32(ctx, 31);
    uint64_t v0 = Ps2GprU64(ctx, 2), v1 = Ps2GprU64(ctx, 3);
    (v0 = (uint64_t)(READ64_B(a1, 16)));
    WRITE64_B(a0, 16, v0);
    (v1 = (uint64_t)(READ64_B(a1, 24)));
    WRITE64_B(a0, 24, v1);
    (v0 = (uint64_t)(READ64_B(a1, 32)));
    WRITE64_B(a0, 32, v0);
    (v1 = (uint64_t)(READ64_B(a1, 40)));
    WRITE64_B(a0, 40, v1);
    (v0 = (uint64_t)(READ64_B(a1, 48)));
    WRITE64_B(a0, 48, v0);
    (v1 = (uint64_t)(READ64_B(a1, 56)));
    WRITE64_B(a0, 56, v1);
    (v0 = (uint64_t)(READ64_B(a1, 64)));
    WRITE64_B(a0, 64, v0);
    (v1 = (uint64_t)(READ64_B(a1, 72)));
    WRITE64_B(a0, 72, v1);
    (v0 = (uint64_t)(READ64_B(a1, 80)));
    WRITE64_B(a0, 80, v0);
    (v1 = (uint64_t)(READ64_B(a1, 88)));
    WRITE64_B(a0, 88, v1);
    (v0 = (uint64_t)(READ64_B(a1, 96)));
    WRITE64_B(a0, 96, v0);
    (v1 = (uint64_t)(READ64_B(a1, 104)));
    WRITE64_B(a0, 104, v1);
    (v0 = (uint64_t)(READ64_B(a1, 112)));
    WRITE64_B(a0, 112, v0);
    (v1 = (uint64_t)(READ64_B(a1, 120)));
    WRITE64_B(a0, 120, v1);
    (v0 = (uint64_t)(READ64_B(a1, 128)));
    WRITE64_B(a0, 128, v0);
    (v1 = (uint64_t)(READ64_B(a1, 136)));
    WRITE64_B(a0, 136, v1);
    (v0 = (uint64_t)(READ64_B(a1, 144)));
    WRITE64_B(a0, 144, v0);
    (v1 = (uint64_t)(READ64_B(a1, 152)));
    WRITE64_B(a0, 152, v1);
    (v0 = (uint64_t)(READ64_B(a1, 160)));
    WRITE64_B(a0, 160, v0);
    (v1 = (uint64_t)(READ64_B(a1, 168)));
    WRITE64_B(a0, 168, v1);
    (v0 = (uint64_t)(READ64_B(a1, 176)));
    WRITE64_B(a0, 176, v0);
    (v1 = (uint64_t)(READ64_B(a1, 184)));
    WRITE64_B(a0, 184, v1);
    (v0 = (uint64_t)(READ64_B(a1, 192)));
    WRITE64_B(a0, 192, v0);
    (v1 = (uint64_t)(READ64_B(a1, 200)));
    {
        const uint32_t target = ra;
        WRITE64_B(a0, 200, v1);
        Ps2SetGpr64(ctx, 2, v0); Ps2SetGpr64(ctx, 3, v1);
        ctx->pc = target;
        return;
    }
}

// setSmallManagerSystemLock (0x13f0f90): lift_d1's text (the reference)
void setSmallManagerSystemLock(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t v0 = Ps2GprU32(ctx, 2), a0 = Ps2GprU32(ctx, 4), ra = Ps2GprU32(ctx, 31);
    v0 = 0x1200000u;
    {
        const uint32_t target = ra;
        WRITE32(v0 - 0x7AE0u, a0);
        Ps2SetGpr64(ctx, 2, sx64(v0));
        ctx->pc = target;
        return;
    }
}

// gamecore_sub_013F1190 (0x13f1190): lift_d1's text (the reference)
void gamecore_sub_013F1190(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t v0 = Ps2GprU32(ctx, 2), a0 = Ps2GprU32(ctx, 4), ra = Ps2GprU32(ctx, 31);
    v0 = 0xFFFFFFFFu;
    WRITE32(a0, 0u);
    WRITE16_B(a0, 82, (uint16_t)v0);
    WRITE32_B(a0, 4, 0u);
    WRITE32_B(a0, 8, 0u);
    WRITE32_B(a0, 12, v0);
    WRITE32_B(a0, 16, 0u);
    WRITE32_B(a0, 84, 0u);
    {
        const uint32_t target = ra;
        WRITE16_B(a0, 80, (uint16_t)v0);
        Ps2SetGpr64(ctx, 2, sx64(v0));
        ctx->pc = target;
        return;
    }
}

// gamecore_sub_013F1938 (0x13f1938): lift_d1's text (the reference)
void gamecore_sub_013F1938(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t at = Ps2GprU32(ctx, 1), v1 = Ps2GprU32(ctx, 3), a0 = Ps2GprU32(ctx, 4), a1 = Ps2GprU32(ctx, 5), ra = Ps2GprU32(ctx, 31);
    uint64_t v0 = Ps2GprU64(ctx, 2);
    float f0 = ctx->f[0], f1 = ctx->f[1], f2 = ctx->f[2];
    uint32_t fcr31 = ctx->fcr31;
    bool df_at = false;
    v1 = READ32_B(a0, 580);
    if (v1 == 0) {
        f0 = lift_d1::flt(READ32_B(a1, 72));
    } else {
        f1 = lift_d1::flt(READ32_B(a0, 500));
        f0 = lift_d1::flt(0u);
        fcr31 = (FPU_C_OLT_S(f1, f0)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
        if ((fcr31 & 0x800000u) != 0u) {
            at = 0xBF350000u; df_at = true;
            at = at | 0x4F3u; df_at = true;
            f0 = lift_d1::flt(at);
            fcr31 = (FPU_C_OLE_S(f1, f0)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
            if ((fcr31 & 0x800000u) != 0u) {
                (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 1))));
                goto L_13f1994;
            }
            (v0 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL));
            goto L_13f1994;
        }
        at = 0x3F350000u; df_at = true;
        at = at | 0x4F3u; df_at = true;
        f0 = lift_d1::flt(at);
        fcr31 = (FPU_C_OLE_S(f0, f1)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
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
        f1 = lift_d1::flt(READ32_B(a0, 452));
        f0 = lift_d1::flt(0u);
        fcr31 = (FPU_C_OLT_S(f1, f0)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
        goto L_13f19c4;
    }
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 1))));
    f1 = lift_d1::flt(READ32_B(a0, 52));
    { uint32_t bits = 0u; std::memcpy(&f2, &bits, sizeof(bits)); }
    f0 = FPU_ADD_S(f0, f1);
    fcr31 = (FPU_C_OLE_S(f0, f2)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
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
            f0 = lift_d1::flt(READ32_B(a1, 72));
        } else {
            f1 = lift_d1::flt(READ32_B(a0, 500));
            f0 = lift_d1::flt(0u);
            fcr31 = (FPU_C_OLT_S(f1, f0)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
            if ((fcr31 & 0x800000u) != 0u) {
                at = 0xBF350000u; df_at = true;
                at = at | 0x4F3u; df_at = true;
                f0 = lift_d1::flt(at);
                fcr31 = (FPU_C_OLE_S(f1, f0)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
                if ((fcr31 & 0x800000u) != 0u) {
                    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 1))));
                    goto L_13f1a34;
                }
                (v0 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL));
                goto L_13f1a34;
            }
            at = 0x3F350000u; df_at = true;
            at = at | 0x4F3u; df_at = true;
            f0 = lift_d1::flt(at);
            fcr31 = (FPU_C_OLE_S(f0, f1)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
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
            f1 = lift_d1::flt(READ32_B(a0, 452));
            f0 = lift_d1::flt(0u);
            fcr31 = (FPU_C_OLT_S(f0, f1)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
            goto L_13f1a64;
        }
        v1 = 1u;
        f1 = lift_d1::flt(READ32_B(a0, 52));
        { uint32_t bits = 0u; std::memcpy(&f2, &bits, sizeof(bits)); }
        f0 = FPU_SUB_S(f0, f1);
        fcr31 = (FPU_C_OLE_S(f2, f0)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
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
        const uint32_t target = ra;
        ctx->f[0] = f0; ctx->f[1] = f1; ctx->f[2] = f2; if (df_at) Ps2SetGpr64(ctx, 1, sx64(at)); Ps2SetGpr64(ctx, 2, v0); Ps2SetGpr64(ctx, 3, sx64(v1)); ctx->fcr31 = fcr31;
        ctx->pc = target;
        return;
    }
}

// getNowExecEventScriptObj (0x13f62c8): lift_d1's text (the reference)
void getNowExecEventScriptObj(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t v0 = Ps2GprU32(ctx, 2), v1 = Ps2GprU32(ctx, 3), ra = Ps2GprU32(ctx, 31);
    v1 = 0x1460000u;
    {
        const uint32_t target = ra;
        v0 = READ32_B(v1, 27404);
        Ps2SetGpr64(ctx, 2, sx64(v0)); Ps2SetGpr64(ctx, 3, sx64(v1));
        ctx->pc = target;
        return;
    }
}

// setEventScriptEnable (0x13f62f0): lift_d1's text (the reference)
void setEventScriptEnable(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t v0 = Ps2GprU32(ctx, 2), a0 = Ps2GprU32(ctx, 4), ra = Ps2GprU32(ctx, 31);
    v0 = 0x1460000u;
    {
        const uint32_t target = ra;
        WRITE32_B(v0, 27416, a0);
        Ps2SetGpr64(ctx, 2, sx64(v0));
        ctx->pc = target;
        return;
    }
}

// setPlayerStatusSwitch (0x13fa530): lift_d1's text (the reference)
void setPlayerStatusSwitch(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t v0 = Ps2GprU32(ctx, 2), a0 = Ps2GprU32(ctx, 4), ra = Ps2GprU32(ctx, 31);
    v0 = 0x1460000u;
    {
        const uint32_t target = ra;
        WRITE32_B(v0, 27432, a0);
        Ps2SetGpr64(ctx, 2, sx64(v0));
        ctx->pc = target;
        return;
    }
}

// startAttackEffect (0x13fa628): lift_d1's text (the reference)
void startAttackEffect(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t at = Ps2GprU32(ctx, 1), v0 = Ps2GprU32(ctx, 2), v1 = Ps2GprU32(ctx, 3), ra = Ps2GprU32(ctx, 31);
    float f0 = ctx->f[0], f1 = ctx->f[1], f12 = ctx->f[12];
    v1 = 0x1460000u;
    at = 0x3F000000u;
    f0 = lift_d1::flt(at);
    f1 = lift_d1::flt(READ32_B(v1, 22472));
    v1 = 1u;
    v0 = 0x1200000u;
    v0 = v0 - 0x79F8u;
    WRITE32_B(v0, 12, lift_d1::bits(f12));
    WRITE32(v0, v1);
    WRITE32_B(v0, 8, lift_d1::bits(f0));
    WRITE32_B(v0, 16, lift_d1::bits(f1));
    {
        const uint32_t target = ra;
        WRITE32_B(v0, 4, lift_d1::bits(f0));
        ctx->f[0] = f0; ctx->f[1] = f1; Ps2SetGpr64(ctx, 1, sx64(at)); Ps2SetGpr64(ctx, 2, sx64(v0)); Ps2SetGpr64(ctx, 3, sx64(v1));
        ctx->pc = target;
        return;
    }
}

// bosslifebar_display (0x13fa930): lift_d1's text (the reference)
void bosslifebar_display(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t ra = Ps2GprU32(ctx, 31);
    uint64_t v0 = Ps2GprU64(ctx, 2), v1 = Ps2GprU64(ctx, 3);
    float f0 = ctx->f[0], f1 = ctx->f[1];
    uint32_t fcr31 = ctx->fcr31;
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)326 << 16))));
    f1 = lift_d1::flt(0u);
    { uint32_t bits = READ32_B((uint32_t)v0, 27560); float f; std::memcpy(&f, &bits, sizeof(f)); f0 = f; }
    fcr31 = (FPU_C_OLT_S(f1, f0)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
    {
        const bool c = (fcr31 & 0x800000u) != 0u;
        (v1 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL));
        if (!c) {
            (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)326 << 16))));
            { uint32_t bits = READ32_B((uint32_t)v0, 27544); float f; std::memcpy(&f, &bits, sizeof(f)); f0 = f; }
            fcr31 = (FPU_C_OLT_S(f1, f0)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
            if ((fcr31 & 0x800000u) == 0u) {
                goto L_13fa960;
            }
        }
    }
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 1))));
L_13fa960:
    {
        const uint32_t target = ra;
        (v0 = (uint64_t)((uint64_t)RC_U64X(v1) + (uint64_t)0ULL));
        ctx->f[0] = f0; ctx->f[1] = f1; Ps2SetGpr64(ctx, 2, v0); Ps2SetGpr64(ctx, 3, v1); ctx->fcr31 = fcr31;
        ctx->pc = target;
        return;
    }
}

// bosslifebar_resethp (0x13fa968): lift_d1's text (the reference)
void bosslifebar_resethp(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t at = Ps2GprU32(ctx, 1), v0 = Ps2GprU32(ctx, 2), v1 = Ps2GprU32(ctx, 3), ra = Ps2GprU32(ctx, 31);
    float f0 = ctx->f[0], f12 = ctx->f[12], f13 = ctx->f[13];
    uint32_t fcr31 = ctx->fcr31;
    f13 = ps2FpuDivide(fcr31, f13, f12);
    at = 0x3C230000u;
    at = at | 0xD70Au;
    f0 = lift_d1::flt(at);
    v0 = 0x1460000u;
    v1 = 0x1460000u;
    f12 = FPU_MUL_S(f12, f0);
    v0 = v0 + 0x6B90u;
    WRITE32_B(v0, 12, 0u);
    WRITE32_B(v1, 27556, lift_d1::bits(f12));
    WRITE32(v0, lift_d1::bits(f13));
    WRITE32_B(v0, 8, lift_d1::bits(f13));
    {
        const uint32_t target = ra;
        WRITE32_B(v0, 4, lift_d1::bits(f13));
        ctx->f[0] = f0; ctx->f[12] = f12; ctx->f[13] = f13; Ps2SetGpr64(ctx, 1, sx64(at)); Ps2SetGpr64(ctx, 2, sx64(v0)); Ps2SetGpr64(ctx, 3, sx64(v1)); ctx->fcr31 = fcr31;
        ctx->pc = target;
        return;
    }
}

// PlayerSprStatusEnd (0x13fab00): lift_d1's text (the reference)
void PlayerSprStatusEnd(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t v0 = Ps2GprU32(ctx, 2), ra = Ps2GprU32(ctx, 31);
    v0 = 0x1460000u;
    {
        const uint32_t target = ra;
        WRITE32_B(v0, 27564, 0u);
        Ps2SetGpr64(ctx, 2, sx64(v0));
        ctx->pc = target;
        return;
    }
}

// BossSprStatusEnd (0x13fab68): lift_d1's text (the reference)
void BossSprStatusEnd(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t v0 = Ps2GprU32(ctx, 2), ra = Ps2GprU32(ctx, 31);
    v0 = 0x1460000u;
    {
        const uint32_t target = ra;
        WRITE32_B(v0, 27568, 0u);
        Ps2SetGpr64(ctx, 2, sx64(v0));
        ctx->pc = target;
        return;
    }
}

// GetSwordFlareHitPlaneAttribute (0x13ff148): lift_d1's text (the reference)
void GetSwordFlareHitPlaneAttribute(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t v1 = Ps2GprU32(ctx, 3), ra = Ps2GprU32(ctx, 31);
    uint64_t v0 = Ps2GprU64(ctx, 2);
    v1 = 0x1460000u;
    {
        const uint32_t target = ra;
        (v0 = (uint64_t)(READ64_B(v1, 27768)));
        Ps2SetGpr64(ctx, 2, v0); Ps2SetGpr64(ctx, 3, sx64(v1));
        ctx->pc = target;
        return;
    }
}

// getSelectWeapon (0x1407310): lift_d1's text (the reference)
void getSelectWeapon(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t v0 = Ps2GprU32(ctx, 2), v1 = Ps2GprU32(ctx, 3), ra = Ps2GprU32(ctx, 31);
    v1 = 0x1460000u;
    {
        const uint32_t target = ra;
        v0 = READ32_B(v1, 28272);
        Ps2SetGpr64(ctx, 2, sx64(v0)); Ps2SetGpr64(ctx, 3, sx64(v1));
        ctx->pc = target;
        return;
    }
}

// getSprLogWorkAddress (0x140ae40): lift_d1's text (the reference)
void getSprLogWorkAddress(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t v0 = Ps2GprU32(ctx, 2), ra = Ps2GprU32(ctx, 31);
    v0 = 0x1200000u;
    {
        const uint32_t target = ra;
        v0 = v0 - 0x77D0u;
        Ps2SetGpr64(ctx, 2, sx64(v0));
        ctx->pc = target;
        return;
    }
}

// getSprLogStatusAddress (0x140ae58): lift_d1's text (the reference)
void getSprLogStatusAddress(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t v0 = Ps2GprU32(ctx, 2), ra = Ps2GprU32(ctx, 31);
    v0 = 0x1460000u;
    {
        const uint32_t target = ra;
        v0 = v0 + 0x6EB0u;
        Ps2SetGpr64(ctx, 2, sx64(v0));
        ctx->pc = target;
        return;
    }
}

// initPauseMenuMap (0x140af68): lift_d1's text (the reference)
void initPauseMenuMap(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t v0 = Ps2GprU32(ctx, 2), ra = Ps2GprU32(ctx, 31);
    v0 = 0x1460000u;
    v0 = v0 + 0x6EB0u;
    WRITE32_B(v0, 8, 0u);
    WRITE32(v0, 0u);
    {
        const uint32_t target = ra;
        WRITE32_B(v0, 4, 0u);
        Ps2SetGpr64(ctx, 2, sx64(v0));
        ctx->pc = target;
        return;
    }
}

// setLoadMapOffset (0x140afb0): lift_d1's text (the reference)
void setLoadMapOffset(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t v0 = Ps2GprU32(ctx, 2), v1 = Ps2GprU32(ctx, 3), a0 = Ps2GprU32(ctx, 4), a1 = Ps2GprU32(ctx, 5), a2 = Ps2GprU32(ctx, 6), a3 = Ps2GprU32(ctx, 7), ra = Ps2GprU32(ctx, 31);
    float f12 = ctx->f[12], f13 = ctx->f[13];
    v0 = 0x1460000u;
    v1 = 0x1460000u;
    a2 = 0x1460000u;
    a3 = 0x1460000u;
    WRITE32_B(v0, 28376, lift_d1::bits(f12));
    WRITE32_B(v1, 28380, lift_d1::bits(f13));
    WRITE32_B(a2, 28384, a0);
    {
        const uint32_t target = ra;
        WRITE32_B(a3, 28388, a1);
        Ps2SetGpr64(ctx, 2, sx64(v0)); Ps2SetGpr64(ctx, 3, sx64(v1)); Ps2SetGpr64(ctx, 6, sx64(a2)); Ps2SetGpr64(ctx, 7, sx64(a3));
        ctx->pc = target;
        return;
    }
}

// isInLoadWait (0x140b0c0): lift_d1's text (the reference)
void isInLoadWait(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t v0 = Ps2GprU32(ctx, 2), v1 = Ps2GprU32(ctx, 3), ra = Ps2GprU32(ctx, 31);
    v1 = 0x1460000u;
    v0 = READ32_B(v1, 28472);
    {
        const uint32_t target = ra;
        v0 = v0 < 1u ? 1u : 0u;
        Ps2SetGpr64(ctx, 2, sx64(v0)); Ps2SetGpr64(ctx, 3, sx64(v1));
        ctx->pc = target;
        return;
    }
}

// LoadWaitCancel (0x140b0d0): lift_d1's text (the reference)
void LoadWaitCancel(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t v0 = Ps2GprU32(ctx, 2), v1 = Ps2GprU32(ctx, 3), a0 = Ps2GprU32(ctx, 4), ra = Ps2GprU32(ctx, 31);
    v0 = 0x1460000u;
    v1 = 0x1460000u;
    WRITE32_B(v0, 28476, a0);
    {
        const uint32_t target = ra;
        WRITE32_B(v1, 28468, 0u);
        Ps2SetGpr64(ctx, 2, sx64(v0)); Ps2SetGpr64(ctx, 3, sx64(v1));
        ctx->pc = target;
        return;
    }
}

// acted_camera_warp (0x140ee50): lift_d1's text (the reference)
void acted_camera_warp(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t ra = Ps2GprU32(ctx, 31);
    uint64_t v0 = Ps2GprU64(ctx, 2), a0 = Ps2GprU64(ctx, 4);
    (v0 = (uint64_t)(a0 ^ (uint64_t)(uint16_t)1));
    {
        const uint32_t target = ra;
        (v0 = (uint64_t)(((uint64_t)v0 < (uint64_t)(int64_t)(int32_t)1) ? 1 : 0));
        Ps2SetGpr64(ctx, 2, v0);
        ctx->pc = target;
        return;
    }
}

// gamecore_sub_0140FE00 (0x140fe00): lift_d1's text (the reference)
void gamecore_sub_0140FE00(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t a0 = Ps2GprU32(ctx, 4), a1 = Ps2GprU32(ctx, 5), a2 = Ps2GprU32(ctx, 6), ra = Ps2GprU32(ctx, 31);
    float f0 = ctx->f[0], f1 = ctx->f[1];
    a2 = a2 << 2;
    a1 = a2 + a1;
    a2 = a2 + a0;
    f0 = lift_d1::flt(READ32(a2));
    f1 = lift_d1::flt(READ32(a1));
    f0 = FPU_SUB_S(f0, f1);
    {
        const uint32_t target = ra;
        f0 = FPU_MUL_S(f0, f0);
        ctx->f[0] = f0; ctx->f[1] = f1; Ps2SetGpr64(ctx, 5, sx64(a1)); Ps2SetGpr64(ctx, 6, sx64(a2));
        ctx->pc = target;
        return;
    }
}

// gamecore_sub_0140FE20 (0x140fe20): lift_d1's text (the reference)
void gamecore_sub_0140FE20(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t a0 = Ps2GprU32(ctx, 4), a1 = Ps2GprU32(ctx, 5), a2 = Ps2GprU32(ctx, 6), a3 = Ps2GprU32(ctx, 7), ra = Ps2GprU32(ctx, 31);
    float f0 = ctx->f[0], f1 = ctx->f[1], f2 = ctx->f[2];
    a3 = a3 << 2;
    a0 = a3 + a0;
    a1 = a3 + a1;
    f1 = lift_d1::flt(READ32(a1));
    a3 = a3 + a2;
    f0 = lift_d1::flt(READ32(a0));
    f2 = lift_d1::flt(READ32(a3));
    f0 = FPU_SUB_S(f0, f1);
    f1 = FPU_SUB_S(f1, f2);
    f0 = FPU_ADD_SF(f0, f0);
    {
        const uint32_t target = ra;
        f0 = FPU_MUL_S(f0, f1);
        ctx->f[0] = f0; ctx->f[1] = f1; ctx->f[2] = f2; Ps2SetGpr64(ctx, 4, sx64(a0)); Ps2SetGpr64(ctx, 5, sx64(a1)); Ps2SetGpr64(ctx, 7, sx64(a3));
        ctx->pc = target;
        return;
    }
}

// gamecore_sub_0140FE50 (0x140fe50): lift_d1's text (the reference)
void gamecore_sub_0140FE50(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t a0 = Ps2GprU32(ctx, 4), a1 = Ps2GprU32(ctx, 5), a2 = Ps2GprU32(ctx, 6), ra = Ps2GprU32(ctx, 31);
    float f0 = ctx->f[0], f1 = ctx->f[1];
    a2 = a2 << 2;
    a1 = a2 + a1;
    a2 = a2 + a0;
    f0 = lift_d1::flt(READ32(a2));
    f1 = lift_d1::flt(READ32(a1));
    f0 = FPU_SUB_S(f0, f1);
    {
        const uint32_t target = ra;
        f0 = FPU_MUL_S(f0, f0);
        ctx->f[0] = f0; ctx->f[1] = f1; Ps2SetGpr64(ctx, 5, sx64(a1)); Ps2SetGpr64(ctx, 6, sx64(a2));
        ctx->pc = target;
        return;
    }
}

// gamecore_sub_0140FE70 (0x140fe70): lift_d1's text (the reference)
void gamecore_sub_0140FE70(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t at = Ps2GprU32(ctx, 1), ra = Ps2GprU32(ctx, 31);
    uint64_t v0 = Ps2GprU64(ctx, 2);
    float f0 = ctx->f[0], f12 = ctx->f[12];
    uint32_t fcr31 = ctx->fcr31;
    bool df_at = false;
    f0 = lift_d1::flt(0u);
    fcr31 = (FPU_C_OLE_S(f0, f12)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
    {
        const bool c = (fcr31 & 0x800000u) == 0u;
        (v0 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL));
        if (!c) {
            at = 0x3F800000u; df_at = true;
            f0 = lift_d1::flt(at);
            fcr31 = (FPU_C_OLT_S(f12, f0)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
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
        const uint32_t target = ra;
        ctx->f[0] = f0; if (df_at) Ps2SetGpr64(ctx, 1, sx64(at)); Ps2SetGpr64(ctx, 2, v0); ctx->fcr31 = fcr31;
        ctx->pc = target;
        return;
    }
}

// reset_time_interp_param_camera_dist_to_look_at (0x1411448): lift_d1's text (the reference)
void reset_time_interp_param_camera_dist_to_look_at(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t at = Ps2GprU32(ctx, 1), a0 = Ps2GprU32(ctx, 4), ra = Ps2GprU32(ctx, 31);
    float f0 = ctx->f[0];
    at = 0x3F800000u;
    f0 = lift_d1::flt(at);
    WRITE32_B(a0, 112, 0u);
    WRITE32_B(a0, 120, lift_d1::bits(f0));
    {
        const uint32_t target = ra;
        WRITE32_B(a0, 116, 0u);
        ctx->f[0] = f0; Ps2SetGpr64(ctx, 1, sx64(at));
        ctx->pc = target;
        return;
    }
}

// is_acting_rewind_camera_dist_to_look_at (0x1411480): lift_d1's text (the reference)
void is_acting_rewind_camera_dist_to_look_at(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t v0 = Ps2GprU32(ctx, 2), a0 = Ps2GprU32(ctx, 4), ra = Ps2GprU32(ctx, 31);
    v0 = READ32_B(a0, 48);
    v0 = v0 ^ 2u;
    {
        const uint32_t target = ra;
        v0 = v0 < 1u ? 1u : 0u;
        Ps2SetGpr64(ctx, 2, sx64(v0));
        ctx->pc = target;
        return;
    }
}

// get_rewind_begin_dist_to_target_camera_dist_to_look_at (0x1411490): lift_d1's text (the reference)
void get_rewind_begin_dist_to_target_camera_dist_to_look_at(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t v0 = Ps2GprU32(ctx, 2), a0 = Ps2GprU32(ctx, 4), ra = Ps2GprU32(ctx, 31);
    float f0 = ctx->f[0], f12 = ctx->f[12];
    v0 = READ32_B(a0, 48);
    v0 = v0 ^ 2u;
    {
        const bool c = v0 != 0;
        f0 = FPU_MOV_S(f12);
        if (!c) {
            f0 = lift_d1::flt(READ32_B(a0, 56));
        }
    }
    {
        const uint32_t target = ra;
        ctx->f[0] = f0; Ps2SetGpr64(ctx, 2, sx64(v0));
        ctx->pc = target;
        return;
    }
}

// end_rewind_camera_dist_to_look_at (0x14114b0): lift_d1's text (the reference)
void end_rewind_camera_dist_to_look_at(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t a0 = Ps2GprU32(ctx, 4), ra = Ps2GprU32(ctx, 31);
    {
        const uint32_t target = ra;
        WRITE32_B(a0, 48, 0u);
        
        ctx->pc = target;
        return;
    }
}

// is_leave_camera_dist_to_look_at (0x14114b8): lift_d1's text (the reference)
void is_leave_camera_dist_to_look_at(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t v0 = Ps2GprU32(ctx, 2), v1 = Ps2GprU32(ctx, 3), a0 = Ps2GprU32(ctx, 4), ra = Ps2GprU32(ctx, 31);
    v1 = READ32(a0);
    v0 = READ32_B(v1, 320);
    v0 = v0 ^ 2u;
    {
        const uint32_t target = ra;
        v0 = v0 < 1u ? 1u : 0u;
        Ps2SetGpr64(ctx, 2, sx64(v0)); Ps2SetGpr64(ctx, 3, sx64(v1));
        ctx->pc = target;
        return;
    }
}

// request_end_camera_pause_rise (0x1412a60): lift_d1's text (the reference)
void request_end_camera_pause_rise(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t v0 = Ps2GprU32(ctx, 2), a0 = Ps2GprU32(ctx, 4), ra = Ps2GprU32(ctx, 31);
    v0 = 2u;
    WRITE32_B(a0, 4, 0u);
    {
        const uint32_t target = ra;
        WRITE32(a0, v0);
        Ps2SetGpr64(ctx, 2, sx64(v0));
        ctx->pc = target;
        return;
    }
}

// is_end_camera_pause_rise (0x1412a70): lift_d1's text (the reference)
void is_end_camera_pause_rise(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t v0 = Ps2GprU32(ctx, 2), a0 = Ps2GprU32(ctx, 4), ra = Ps2GprU32(ctx, 31);
    v0 = READ32(a0);
    {
        const uint32_t target = ra;
        v0 = v0 < 1u ? 1u : 0u;
        Ps2SetGpr64(ctx, 2, sx64(v0));
        ctx->pc = target;
        return;
    }
}

// get_begin_interp_finish_time_camera_pause_rise (0x1412a80): lift_d1's text (the reference)
void get_begin_interp_finish_time_camera_pause_rise(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t v0 = Ps2GprU32(ctx, 2), ra = Ps2GprU32(ctx, 31);
    float f0 = ctx->f[0];
    v0 = 0x1470000u;
    {
        const uint32_t target = ra;
        f0 = lift_d1::flt(READ32(v0 - 0x3C50u));
        ctx->f[0] = f0; Ps2SetGpr64(ctx, 2, sx64(v0));
        ctx->pc = target;
        return;
    }
}

// get_place_status_camera_place_status (0x1413340): lift_d1's text (the reference)
void get_place_status_camera_place_status(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t v0 = Ps2GprU32(ctx, 2), a0 = Ps2GprU32(ctx, 4), ra = Ps2GprU32(ctx, 31);
    {
        const uint32_t target = ra;
        v0 = READ32(a0);
        Ps2SetGpr64(ctx, 2, sx64(v0));
        ctx->pc = target;
        return;
    }
}

// get_manage_obj_camera_place_status (0x1413348): lift_d1's text (the reference)
void get_manage_obj_camera_place_status(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t v0 = Ps2GprU32(ctx, 2), a0 = Ps2GprU32(ctx, 4), ra = Ps2GprU32(ctx, 31);
    {
        const uint32_t target = ra;
        v0 = READ32_B(a0, 4);
        Ps2SetGpr64(ctx, 2, sx64(v0));
        ctx->pc = target;
        return;
    }
}

// get_hit_camera_region_camera_place_status (0x1413408): lift_d1's text (the reference)
void get_hit_camera_region_camera_place_status(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t v0 = Ps2GprU32(ctx, 2), a0 = Ps2GprU32(ctx, 4), ra = Ps2GprU32(ctx, 31);
    {
        const uint32_t target = ra;
        v0 = READ32_B(a0, 12);
        Ps2SetGpr64(ctx, 2, sx64(v0));
        ctx->pc = target;
        return;
    }
}

// get_on_boss_camera_col_attr_camera_place_status (0x14134b8): lift_d1's text (the reference)
void get_on_boss_camera_col_attr_camera_place_status(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t a0 = Ps2GprU32(ctx, 4), a1 = Ps2GprU32(ctx, 5), ra = Ps2GprU32(ctx, 31);
    uint64_t v0 = Ps2GprU64(ctx, 2);
    a1 = a1 + 0x150u;
    (v0 = (uint64_t)(READ64_B(a1, 16)));
    WRITE64_B(a0, 0, v0);
    {
        const uint32_t target = ra;
        (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)READ32_B(a1, 8))));
        Ps2SetGpr64(ctx, 2, v0); Ps2SetGpr64(ctx, 5, sx64(a1));
        ctx->pc = target;
        return;
    }
}

// is_approach_rise_ok_camera_place_status (0x14134d0): lift_d1's text (the reference)
void is_approach_rise_ok_camera_place_status(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint64_t v0 = Ps2GprU64(ctx, 2), v1 = Ps2GprU64(ctx, 3), a0 = Ps2GprU64(ctx, 4), ra = Ps2GprU64(ctx, 31);
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)a0, 208))));
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 1))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)READ32_B((uint32_t)a0, 8))));
    {
        const bool c = (int64_t)v0 != (int64_t)v1;
        (a0 = (uint64_t)(READ64_B((uint32_t)a0, 16)));
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
            const uint32_t target = (uint32_t)ra;
            (v0 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL));
            Ps2SetGpr64(ctx, 2, v0); Ps2SetGpr64(ctx, 3, v1); Ps2SetGpr64(ctx, 4, a0);
            ctx->pc = target;
            return;
        }
    }
L_1413530:
    {
        const uint32_t target = (uint32_t)ra;
        (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 1))));
        Ps2SetGpr64(ctx, 2, v0); Ps2SetGpr64(ctx, 3, v1); Ps2SetGpr64(ctx, 4, a0);
        ctx->pc = target;
        return;
    }
}

// is_cliff_rise_delay_camera_place_status (0x14135b8): lift_d1's text (the reference)
void is_cliff_rise_delay_camera_place_status(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint64_t v0 = Ps2GprU64(ctx, 2), v1 = Ps2GprU64(ctx, 3), a0 = Ps2GprU64(ctx, 4), ra = Ps2GprU64(ctx, 31);
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)a0, 208))));
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 1))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)READ32_B((uint32_t)a0, 8))));
    {
        const bool c = (int64_t)v0 != (int64_t)v1;
        (a0 = (uint64_t)(READ64_B((uint32_t)a0, 16)));
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
        const uint32_t target = (uint32_t)ra;
        Ps2SetGpr64(ctx, 2, v0); Ps2SetGpr64(ctx, 3, v1); Ps2SetGpr64(ctx, 4, a0);
        ctx->pc = target;
        return;
    }
}

// act_horse_look_at_ofst_decrease_camera_place_status (0x1413668): lift_d1's text (the reference)
void act_horse_look_at_ofst_decrease_camera_place_status(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint64_t v0 = Ps2GprU64(ctx, 2), v1 = Ps2GprU64(ctx, 3), a0 = Ps2GprU64(ctx, 4), ra = Ps2GprU64(ctx, 31);
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)a0, 208))));
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 1))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)READ32_B((uint32_t)a0, 8))));
    {
        const bool c = (int64_t)v0 != (int64_t)v1;
        (a0 = (uint64_t)(READ64_B((uint32_t)a0, 16)));
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
        const uint32_t target = (uint32_t)ra;
        Ps2SetGpr64(ctx, 2, v0); Ps2SetGpr64(ctx, 3, v1); Ps2SetGpr64(ctx, 4, a0);
        ctx->pc = target;
        return;
    }
}

// act_look_at_ofst_cut_camera_place_status (0x14136b8): lift_d1's text (the reference)
void act_look_at_ofst_cut_camera_place_status(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint64_t v0 = Ps2GprU64(ctx, 2), v1 = Ps2GprU64(ctx, 3), a0 = Ps2GprU64(ctx, 4), ra = Ps2GprU64(ctx, 31);
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)a0, 208))));
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 1))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)READ32_B((uint32_t)a0, 8))));
    {
        const bool c = (int64_t)v0 != (int64_t)v1;
        (a0 = (uint64_t)(READ64_B((uint32_t)a0, 16)));
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
        const uint32_t target = (uint32_t)ra;
        Ps2SetGpr64(ctx, 2, v0); Ps2SetGpr64(ctx, 3, v1); Ps2SetGpr64(ctx, 4, a0);
        ctx->pc = target;
        return;
    }
}

// gamecore_sub_01413708 (0x1413708): lift_d1's text (the reference)
void gamecore_sub_01413708(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t v0 = Ps2GprU32(ctx, 2), a0 = Ps2GprU32(ctx, 4), ra = Ps2GprU32(ctx, 31);
    v0 = 0xFFFFFFFFu;
    WRITE32_B(a0, 24, 0u);
    WRITE64_B(a0, 192, sx64(v0));
    {
        const uint32_t target = ra;
        WRITE32_B(a0, 28, 0u);
        Ps2SetGpr64(ctx, 2, sx64(v0));
        ctx->pc = target;
        return;
    }
}

// CAM_get_interp_slope_pitch_ofst (0x1414200): lift_d1's text (the reference)
void CAM_get_interp_slope_pitch_ofst(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t a0 = Ps2GprU32(ctx, 4), ra = Ps2GprU32(ctx, 31);
    float f0 = ctx->f[0];
    f0 = lift_d1::flt(READ32_B(a0, 28));
    {
        const uint32_t target = ra;
        f0 = FPU_NEG_S(f0);
        ctx->f[0] = f0;
        ctx->pc = target;
        return;
    }
}

// gamecore_sub_014144D0 (0x14144d0): lift_d1's text (the reference)
void gamecore_sub_014144D0(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t ra = Ps2GprU32(ctx, 31);
    float f0 = ctx->f[0], f1 = ctx->f[1], f12 = ctx->f[12], f13 = ctx->f[13], f14 = ctx->f[14];
    uint32_t fcr31 = ctx->fcr31;
    f12 = FPU_SUB_S(f12, f13);
    f1 = lift_d1::flt(0u);
    fcr31 = (FPU_C_OLT_S(f12, f1)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
    if ((fcr31 & 0x800000u) != 0u) {
        f12 = FPU_MOV_S(f1);
    } else {
    }
    {
        const uint32_t target = ra;
        f0 = FPU_MUL_S(f14, f12);
        ctx->f[0] = f0; ctx->f[1] = f1; ctx->f[12] = f12; ctx->fcr31 = fcr31;
        ctx->pc = target;
        return;
    }
}

// get_attr_camera_target_lost_angle (0x1414e60): lift_d1's text (the reference)
void get_attr_camera_target_lost_angle(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t v0 = Ps2GprU32(ctx, 2), a0 = Ps2GprU32(ctx, 4), ra = Ps2GprU32(ctx, 31);
    {
        const uint32_t target = ra;
        v0 = READ32(a0);
        Ps2SetGpr64(ctx, 2, sx64(v0));
        ctx->pc = target;
        return;
    }
}

// set_parameter_camera_target_visible_angle_manager (0x1415900): lift_d1's text (the reference)
void set_parameter_camera_target_visible_angle_manager(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t at = Ps2GprU32(ctx, 1), v0 = Ps2GprU32(ctx, 2), v1 = Ps2GprU32(ctx, 3), a0 = Ps2GprU32(ctx, 4), a1 = Ps2GprU32(ctx, 5), a2 = Ps2GprU32(ctx, 6), ra = Ps2GprU32(ctx, 31);
    float f0 = ctx->f[0], f1 = ctx->f[1], f12 = ctx->f[12], f13 = ctx->f[13];
    uint32_t fcr31 = ctx->fcr31;
    v0 = a1 >> 31;
    v1 = a1 - 1u;
    v0 = a1 + v0;
    a2 = a1 + 1u;
    v0 = (uint32_t)((int32_t)v0 >> 1);
    v0 = v0 << 1;
    {
        const bool c = v0 != v1;
        WRITE32_B(a0, 988, a1);
        if (!c) {
            WRITE32_B(a0, 988, a2);
        }
    }
    v1 = READ32_B(a0, 988);
    v0 = (int32_t)v1 < 21 ? 1u : 0u;
    {
        const bool c = v0 != 0;
        v0 = 0x14u;
        if (!c) {
            v1 = 0x14u;
            WRITE32_B(a0, 988, v0);
        }
    }
    at = 0x3C8E0000u;
    at = at | 0xFA35u;
    f0 = lift_d1::flt(at);
    f1 = lift_d1::flt(v1);
    { int32_t tmp; std::memcpy(&tmp, &f1, sizeof(tmp)); f1 = FPU_CVT_S_W(tmp); }
    WRITE32_B(a0, 996, lift_d1::bits(f13));
    f0 = FPU_MUL_S(f12, f0);
    f1 = ps2FpuDivide(fcr31, f0, f1);
    WRITE32_B(a0, 992, lift_d1::bits(f0));
    {
        const uint32_t target = ra;
        WRITE32_B(a0, 1000, lift_d1::bits(f1));
        ctx->f[0] = f0; ctx->f[1] = f1; Ps2SetGpr64(ctx, 1, sx64(at)); Ps2SetGpr64(ctx, 2, sx64(v0)); Ps2SetGpr64(ctx, 3, sx64(v1)); Ps2SetGpr64(ctx, 6, sx64(a2)); ctx->fcr31 = fcr31;
        ctx->pc = target;
        return;
    }
}

// end_get_yaw_pitch_camera_target_visible_angle_manager (0x1415a40): lift_d1's text (the reference)
void end_get_yaw_pitch_camera_target_visible_angle_manager(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t a0 = Ps2GprU32(ctx, 4), ra = Ps2GprU32(ctx, 31);
    {
        const uint32_t target = ra;
        WRITE32_B(a0, 976, 0u);
        
        ctx->pc = target;
        return;
    }
}

// gamecore_sub_01415CA0 (0x1415ca0): lift_d1's text (the reference)
void gamecore_sub_01415CA0(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t v0 = Ps2GprU32(ctx, 2), v1 = Ps2GprU32(ctx, 3), a0 = Ps2GprU32(ctx, 4), a1 = Ps2GprU32(ctx, 5), a2 = Ps2GprU32(ctx, 6), ra = Ps2GprU32(ctx, 31);
    float f0 = ctx->f[0], f1 = ctx->f[1];
    uint32_t fcr31 = ctx->fcr31;
    a2 = READ32(a0);
    v1 = READ32(a1);
    f1 = lift_d1::flt(READ32_B(a2, 4));
    f0 = lift_d1::flt(READ32_B(v1, 4));
    fcr31 = (FPU_C_OLT_S(f0, f1)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
    {
        const bool c = (fcr31 & 0x800000u) != 0u;
        v0 = 1u;
        if (!c) {
            v0 = 0xFFFFFFFFu;
        }
    }
    {
        const uint32_t target = ra;
        ctx->f[0] = f0; ctx->f[1] = f1; Ps2SetGpr64(ctx, 2, sx64(v0)); Ps2SetGpr64(ctx, 3, sx64(v1)); Ps2SetGpr64(ctx, 6, sx64(a2)); ctx->fcr31 = fcr31;
        ctx->pc = target;
        return;
    }
}

// CAM_copy_camera_base_data (0x141a108): lift_d1's text (the reference)
void CAM_copy_camera_base_data(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t a0 = Ps2GprU32(ctx, 4), a1 = Ps2GprU32(ctx, 5), ra = Ps2GprU32(ctx, 31);
    uint64_t v0 = Ps2GprU64(ctx, 2), v1 = Ps2GprU64(ctx, 3);
    (v0 = (uint64_t)(READ64_B(a1, 0)));
    WRITE64_B(a0, 0, v0);
    (v1 = (uint64_t)(READ64_B(a1, 8)));
    WRITE64_B(a0, 8, v1);
    (v0 = (uint64_t)(READ64_B(a1, 16)));
    WRITE64_B(a0, 16, v0);
    (v1 = (uint64_t)(READ64_B(a1, 24)));
    WRITE64_B(a0, 24, v1);
    (v0 = (uint64_t)(READ64_B(a1, 32)));
    WRITE64_B(a0, 32, v0);
    (v1 = (uint64_t)(READ64_B(a1, 40)));
    {
        const uint32_t target = ra;
        WRITE64_B(a0, 40, v1);
        Ps2SetGpr64(ctx, 2, v0); Ps2SetGpr64(ctx, 3, v1);
        ctx->pc = target;
        return;
    }
}

// CAM_interp_reset_aim_rot_common (0x141a160): lift_d1's text (the reference)
void CAM_interp_reset_aim_rot_common(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t v0 = Ps2GprU32(ctx, 2), a0 = Ps2GprU32(ctx, 4), ra = Ps2GprU32(ctx, 31);
    v0 = 1u;
    {
        const uint32_t target = ra;
        WRITE32_B(a0, 32, v0);
        Ps2SetGpr64(ctx, 2, sx64(v0));
        ctx->pc = target;
        return;
    }
}

// CAM_motion_kind_is_look_around_motion_target (0x141a658): lift_d1's text (the reference)
void CAM_motion_kind_is_look_around_motion_target(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t v0 = Ps2GprU32(ctx, 2), a0 = Ps2GprU32(ctx, 4), ra = Ps2GprU32(ctx, 31);
    v0 = READ32_B(a0, 372);
    v0 = v0 >> 28;
    {
        const uint32_t target = ra;
        v0 = v0 & 1u;
        Ps2SetGpr64(ctx, 2, sx64(v0));
        ctx->pc = target;
        return;
    }
}

// CAM_motion_kind_is_accept_auto_rot_motion_target (0x141a678): lift_d1's text (the reference)
void CAM_motion_kind_is_accept_auto_rot_motion_target(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t v0 = Ps2GprU32(ctx, 2), a0 = Ps2GprU32(ctx, 4), ra = Ps2GprU32(ctx, 31);
    v0 = READ32_B(a0, 372);
    v0 = v0 >> 30;
    {
        const uint32_t target = ra;
        v0 = v0 & 1u;
        Ps2SetGpr64(ctx, 2, sx64(v0));
        ctx->pc = target;
        return;
    }
}

// CAM_get_target_obj (0x141a7b8): lift_d1's text (the reference)
void CAM_get_target_obj(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t v0 = Ps2GprU32(ctx, 2), a0 = Ps2GprU32(ctx, 4), ra = Ps2GprU32(ctx, 31);
    {
        const uint32_t target = ra;
        v0 = READ32(a0);
        Ps2SetGpr64(ctx, 2, sx64(v0));
        ctx->pc = target;
        return;
    }
}

// CAM_get_target_riding_obj (0x141a7c0): lift_d1's text (the reference)
void CAM_get_target_riding_obj(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t v0 = Ps2GprU32(ctx, 2), a0 = Ps2GprU32(ctx, 4), ra = Ps2GprU32(ctx, 31);
    {
        const uint32_t target = ra;
        v0 = READ32_B(a0, 712);
        Ps2SetGpr64(ctx, 2, sx64(v0));
        ctx->pc = target;
        return;
    }
}

// CAM_get_ground_contact_obj (0x141a7c8): lift_d1's text (the reference)
void CAM_get_ground_contact_obj(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t v1 = Ps2GprU32(ctx, 3), a0 = Ps2GprU32(ctx, 4), ra = Ps2GprU32(ctx, 31);
    uint64_t v0 = Ps2GprU64(ctx, 2);
    v1 = READ32_B(a0, 712);
    {
        const bool c = v1 != 0;
        (v0 = (uint64_t)((uint64_t)sx64(v1) + (uint64_t)0ULL));
        if (!c) {
            (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)READ32_B(a0, 0))));
        }
    }
    {
        const uint32_t target = ra;
        Ps2SetGpr64(ctx, 2, v0); Ps2SetGpr64(ctx, 3, sx64(v1));
        ctx->pc = target;
        return;
    }
}

// CAM_is_visible_now_target_visibility (0x141aa80): lift_d1's text (the reference)
void CAM_is_visible_now_target_visibility(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t v0 = Ps2GprU32(ctx, 2), a0 = Ps2GprU32(ctx, 4), ra = Ps2GprU32(ctx, 31);
    v0 = READ32_B(a0, 68);
    {
        const uint32_t target = ra;
        v0 = v0 < 1u ? 1u : 0u;
        Ps2SetGpr64(ctx, 2, sx64(v0));
        ctx->pc = target;
        return;
    }
}

// CAM_is_return_cam_posi_target_visibility (0x141aa90): lift_d1's text (the reference)
void CAM_is_return_cam_posi_target_visibility(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t v0 = Ps2GprU32(ctx, 2), a0 = Ps2GprU32(ctx, 4), ra = Ps2GprU32(ctx, 31);
    {
        const uint32_t target = ra;
        v0 = READ32_B(a0, 304);
        Ps2SetGpr64(ctx, 2, sx64(v0));
        ctx->pc = target;
        return;
    }
}

// CAM_reset_camera_warp_cntrl (0x141aad0): lift_d1's text (the reference)
void CAM_reset_camera_warp_cntrl(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t at = Ps2GprU32(ctx, 1), v0 = Ps2GprU32(ctx, 2), a0 = Ps2GprU32(ctx, 4), ra = Ps2GprU32(ctx, 31);
    float f0 = ctx->f[0], f1 = ctx->f[1];
    v0 = 0x1470000u;
    at = 0x3F800000u;
    f0 = lift_d1::flt(at);
    f1 = lift_d1::flt(READ32(v0 - 0x3B60u));
    WRITE32_B(a0, 60, lift_d1::bits(f0));
    WRITE32_B(a0, 48, lift_d1::bits(f1));
    WRITE32_B(a0, 4, 0u);
    WRITE32_B(a0, 8, 0u);
    WRITE32_B(a0, 16, 0u);
    WRITE32_B(a0, 32, 0u);
    WRITE32_B(a0, 36, 0u);
    WRITE32_B(a0, 40, 0u);
    WRITE32_B(a0, 44, 0u);
    WRITE32_B(a0, 56, lift_d1::bits(f1));
    {
        const uint32_t target = ra;
        WRITE32_B(a0, 52, lift_d1::bits(f1));
        ctx->f[0] = f0; ctx->f[1] = f1; Ps2SetGpr64(ctx, 1, sx64(at)); Ps2SetGpr64(ctx, 2, sx64(v0));
        ctx->pc = target;
        return;
    }
}

// CAM_set_invisible_warp_apply_range_camera_warp_cntrl (0x141aca8): lift_d1's text (the reference)
void CAM_set_invisible_warp_apply_range_camera_warp_cntrl(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t a0 = Ps2GprU32(ctx, 4), ra = Ps2GprU32(ctx, 31);
    float f12 = ctx->f[12], f13 = ctx->f[13];
    WRITE32_B(a0, 24, lift_d1::bits(f13));
    {
        const uint32_t target = ra;
        WRITE32_B(a0, 20, lift_d1::bits(f12));
        
        ctx->pc = target;
        return;
    }
}

// CAM_act_warp_order_camera_warp_cntrl (0x141acb8): lift_d1's text (the reference)
void CAM_act_warp_order_camera_warp_cntrl(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t v0 = Ps2GprU32(ctx, 2), a0 = Ps2GprU32(ctx, 4), ra = Ps2GprU32(ctx, 31);
    v0 = 1u;
    {
        const uint32_t target = ra;
        WRITE32_B(a0, 40, v0);
        Ps2SetGpr64(ctx, 2, sx64(v0));
        ctx->pc = target;
        return;
    }
}

// CAM_is_acting_view_cut_avoid_cntrl (0x141b010): lift_d1's text (the reference)
void CAM_is_acting_view_cut_avoid_cntrl(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t v0 = Ps2GprU32(ctx, 2), a0 = Ps2GprU32(ctx, 4), ra = Ps2GprU32(ctx, 31);
    v0 = READ32(a0);
    {
        const uint32_t target = ra;
        v0 = 0u < v0 ? 1u : 0u;
        Ps2SetGpr64(ctx, 2, sx64(v0));
        ctx->pc = target;
        return;
    }
}

// CAM_set_param_view_cut_avoid_cntrl (0x141b020): lift_d1's text (the reference)
void CAM_set_param_view_cut_avoid_cntrl(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t a0 = Ps2GprU32(ctx, 4), ra = Ps2GprU32(ctx, 31);
    float f12 = ctx->f[12], f13 = ctx->f[13];
    WRITE32_B(a0, 152, lift_d1::bits(f13));
    {
        const uint32_t target = ra;
        WRITE32_B(a0, 148, lift_d1::bits(f12));
        
        ctx->pc = target;
        return;
    }
}

// CAM_reset_obstacle_avoid_cntrl (0x141b030): lift_d1's text (the reference)
void CAM_reset_obstacle_avoid_cntrl(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t a0 = Ps2GprU32(ctx, 4), ra = Ps2GprU32(ctx, 31);
    WRITE32_B(a0, 16, 0u);
    WRITE32(a0, 0u);
    WRITE32_B(a0, 4, 0u);
    WRITE32_B(a0, 8, 0u);
    {
        const uint32_t target = ra;
        WRITE32_B(a0, 12, 0u);
        
        ctx->pc = target;
        return;
    }
}

// gamecore_sub_0141B428 (0x141b428): lift_d1's text (the reference)
void gamecore_sub_0141B428(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t at = Ps2GprU32(ctx, 1), ra = Ps2GprU32(ctx, 31);
    float f0 = ctx->f[0], f12 = ctx->f[12], f13 = ctx->f[13];
    uint32_t fcr31 = ctx->fcr31;
    fcr31 = (FPU_C_OLE_S(f13, f12)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
    at = 0x3F800000u;
    f0 = lift_d1::flt(at);
    if ((fcr31 & 0x800000u) == 0u) {
        f0 = ps2FpuDivide(fcr31, f12, f13);
    }
    {
        const uint32_t target = ra;
        ctx->f[0] = f0; Ps2SetGpr64(ctx, 1, sx64(at)); ctx->fcr31 = fcr31;
        ctx->pc = target;
        return;
    }
}

// gamecore_sub_0141B480 (0x141b480): lift_d1's text (the reference)
void gamecore_sub_0141B480(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t a0 = Ps2GprU32(ctx, 4), ra = Ps2GprU32(ctx, 31);
    {
        const uint32_t target = ra;
        WRITE32_B(a0, 4, 0u);
        
        ctx->pc = target;
        return;
    }
}

// gamecore_sub_0141B978 (0x141b978): lift_d1's text (the reference)
void gamecore_sub_0141B978(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t v0 = Ps2GprU32(ctx, 2), a0 = Ps2GprU32(ctx, 4), ra = Ps2GprU32(ctx, 31);
    v0 = a0 - 3u;
    {
        const uint32_t target = ra;
        v0 = v0 < 2u ? 1u : 0u;
        Ps2SetGpr64(ctx, 2, sx64(v0));
        ctx->pc = target;
        return;
    }
}

// gamecore_sub_0141B988 (0x141b988): lift_d1's text (the reference)
void gamecore_sub_0141B988(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t v0 = Ps2GprU32(ctx, 2), a0 = Ps2GprU32(ctx, 4), ra = Ps2GprU32(ctx, 31);
    v0 = a0 - 1u;
    {
        const uint32_t target = ra;
        v0 = v0 < 2u ? 1u : 0u;
        Ps2SetGpr64(ctx, 2, sx64(v0));
        ctx->pc = target;
        return;
    }
}

// gamecore_sub_0141BB20 (0x141bb20): lift_d1's text (the reference)
void gamecore_sub_0141BB20(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t at = Ps2GprU32(ctx, 1), v0 = Ps2GprU32(ctx, 2), v1 = Ps2GprU32(ctx, 3), a0 = Ps2GprU32(ctx, 4), a1 = Ps2GprU32(ctx, 5), ra = Ps2GprU32(ctx, 31);
    float f0 = ctx->f[0], f1 = ctx->f[1], f2 = ctx->f[2], f12 = ctx->f[12];
    uint32_t fcr31 = ctx->fcr31;
    bool df_v1 = false;
    v0 = READ32(a0);
    {
        const bool c = v0 == 0;
        v0 = 1u;
        if (!c) {
            v1 = READ32_B(a1, 6196); df_v1 = true;
            if (v1 != v0) {
                f0 = lift_d1::flt(READ32_B(a0, 8));
                goto L_141bb68;
            }
        }
    }
    at = 0x3F000000u;
    f0 = lift_d1::flt(at);
    f1 = lift_d1::flt(READ32_B(a0, 8));
    f0 = FPU_MUL_S(f12, f0);
    f2 = lift_d1::flt(0u);
    f1 = FPU_SUB_S(f1, f0);
    fcr31 = (FPU_C_OLT_S(f1, f2)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
    {
        const bool c = (fcr31 & 0x800000u) == 0u;
        WRITE32_B(a0, 8, lift_d1::bits(f1));
        if (c) {
            goto L_141bb7c;
        }
    }
    {
        const uint32_t target = ra;
        WRITE32_B(a0, 8, lift_d1::bits(f2));
        ctx->f[0] = f0; ctx->f[1] = f1; ctx->f[2] = f2; Ps2SetGpr64(ctx, 1, sx64(at)); Ps2SetGpr64(ctx, 2, sx64(v0)); if (df_v1) Ps2SetGpr64(ctx, 3, sx64(v1)); ctx->fcr31 = fcr31;
        ctx->pc = target;
        return;
    }
L_141bb68:
    at = 0x3F800000u;
    f1 = lift_d1::flt(at);
    f0 = FPU_ADD_S(f0, f12);
    WRITE32_B(a0, 4, lift_d1::bits(f1));
    WRITE32_B(a0, 8, lift_d1::bits(f0));
L_141bb7c:
    {
        const uint32_t target = ra;
        ctx->f[0] = f0; ctx->f[1] = f1; ctx->f[2] = f2; Ps2SetGpr64(ctx, 1, sx64(at)); Ps2SetGpr64(ctx, 2, sx64(v0)); if (df_v1) Ps2SetGpr64(ctx, 3, sx64(v1)); ctx->fcr31 = fcr31;
        ctx->pc = target;
        return;
    }
}

// gamecore_sub_0141BCF8 (0x141bcf8): lift_d1's text (the reference)
void gamecore_sub_0141BCF8(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t v0 = Ps2GprU32(ctx, 2), a1 = Ps2GprU32(ctx, 5), ra = Ps2GprU32(ctx, 31);
    v0 = READ32_B(a1, 1380);
    {
        const uint32_t target = ra;
        v0 = v0 < 1u ? 1u : 0u;
        Ps2SetGpr64(ctx, 2, sx64(v0));
        ctx->pc = target;
        return;
    }
}

// gamecore_sub_0141C0A8 (0x141c0a8): lift_d1's text (the reference)
void gamecore_sub_0141C0A8(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t ra = Ps2GprU32(ctx, 31);
    float f0 = ctx->f[0], f12 = ctx->f[12], f13 = ctx->f[13], f14 = ctx->f[14];
    uint32_t fcr31 = ctx->fcr31;
    f0 = lift_d1::flt(0u);
    fcr31 = (FPU_C_OLT_S(f13, f0)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
    if ((fcr31 & 0x800000u) != 0u) {
        f0 = FPU_NEG_S(f13);
        f0 = ps2FpuDivide(fcr31, f0, f12);
        goto L_141c0dc;
    }
    f0 = ps2FpuDivide(fcr31, f13, f12);
L_141c0dc:
    fcr31 = (FPU_C_OLT_S(f0, f14)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
    if ((fcr31 & 0x800000u) != 0u) {
        f0 = FPU_MOV_S(f14);
    } else {
    }
    {
        const uint32_t target = ra;
        ctx->f[0] = f0; ctx->fcr31 = fcr31;
        ctx->pc = target;
        return;
    }
}

// get_view_angle_camera_view_angle_cntrl (0x141c9b0): lift_d1's text (the reference)
void get_view_angle_camera_view_angle_cntrl(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t a0 = Ps2GprU32(ctx, 4), ra = Ps2GprU32(ctx, 31);
    float f0 = ctx->f[0];
    {
        const uint32_t target = ra;
        f0 = lift_d1::flt(READ32_B(a0, 28));
        ctx->f[0] = f0;
        ctx->pc = target;
        return;
    }
}

// get_camera_obj (0x141cd68): lift_d1's text (the reference)
void get_camera_obj(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t v0 = Ps2GprU32(ctx, 2), a0 = Ps2GprU32(ctx, 4), ra = Ps2GprU32(ctx, 31);
    {
        const uint32_t target = ra;
        v0 = READ32(a0);
        Ps2SetGpr64(ctx, 2, sx64(v0));
        ctx->pc = target;
        return;
    }
}

// get_player_collision_group_cameraman (0x141ce48): lift_d1's text (the reference)
void get_player_collision_group_cameraman(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t v0 = Ps2GprU32(ctx, 2), a0 = Ps2GprU32(ctx, 4), ra = Ps2GprU32(ctx, 31);
    {
        const uint32_t target = ra;
        v0 = READ32_B(a0, 20);
        Ps2SetGpr64(ctx, 2, sx64(v0));
        ctx->pc = target;
        return;
    }
}

// is_camera_target_change (0x141ce80): lift_d1's text (the reference)
void is_camera_target_change(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t v0 = Ps2GprU32(ctx, 2), v1 = Ps2GprU32(ctx, 3), a0 = Ps2GprU32(ctx, 4), ra = Ps2GprU32(ctx, 31);
    v1 = READ32_B(a0, 24);
    v0 = READ32_B(a0, 28);
    v0 = v0 ^ v1;
    {
        const uint32_t target = ra;
        v0 = 0u < v0 ? 1u : 0u;
        Ps2SetGpr64(ctx, 2, sx64(v0)); Ps2SetGpr64(ctx, 3, sx64(v1));
        ctx->pc = target;
        return;
    }
}

// is_camera_target_debug_move (0x141ce98): lift_d1's text (the reference)
void is_camera_target_debug_move(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t ra = Ps2GprU32(ctx, 31);
    uint64_t v0 = Ps2GprU64(ctx, 2);
    {
        const uint32_t target = ra;
        (v0 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL));
        Ps2SetGpr64(ctx, 2, v0);
        ctx->pc = target;
        return;
    }
}

// get_main_cameraman (0x141d370): lift_d1's text (the reference)
void get_main_cameraman(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t v0 = Ps2GprU32(ctx, 2), v1 = Ps2GprU32(ctx, 3), ra = Ps2GprU32(ctx, 31);
    v1 = 0x1200000u;
    v1 = v1 - 0x4A00u;
    v0 = READ32(v1);
    v0 = v0 << 2;
    v1 = v1 + v0;
    {
        const uint32_t target = ra;
        v0 = READ32_B(v1, 8);
        Ps2SetGpr64(ctx, 2, sx64(v0)); Ps2SetGpr64(ctx, 3, sx64(v1));
        ctx->pc = target;
        return;
    }
}

// reset_game_camera (0x1424e18): lift_d1's text (the reference)
void reset_game_camera(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t v0 = Ps2GprU32(ctx, 2), a0 = Ps2GprU32(ctx, 4), ra = Ps2GprU32(ctx, 31);
    v0 = 5u;
    {
        const uint32_t target = ra;
        WRITE32_B(a0, 11328, v0);
        Ps2SetGpr64(ctx, 2, sx64(v0));
        ctx->pc = target;
        return;
    }
}

// get_game_camera_cur_mode (0x1424e28): lift_d1's text (the reference)
void get_game_camera_cur_mode(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t v0 = Ps2GprU32(ctx, 2), a0 = Ps2GprU32(ctx, 4), ra = Ps2GprU32(ctx, 31);
    {
        const uint32_t target = ra;
        v0 = READ32_B(a0, 4960);
        Ps2SetGpr64(ctx, 2, sx64(v0));
        ctx->pc = target;
        return;
    }
}

// is_reset_or_lws_movie_game_camera (0x1424e30): lift_d1's text (the reference)
void is_reset_or_lws_movie_game_camera(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t v1 = Ps2GprU32(ctx, 3), a0 = Ps2GprU32(ctx, 4), ra = Ps2GprU32(ctx, 31);
    uint64_t v0 = Ps2GprU64(ctx, 2);
    (a0 = (uint32_t)((int32_t)((int32_t)READ32_B(a0, 4960))));
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
        const uint32_t target = ra;
        Ps2SetGpr64(ctx, 2, v0); Ps2SetGpr64(ctx, 3, sx64(v1)); Ps2SetGpr64(ctx, 4, sx64(a0));
        ctx->pc = target;
        return;
    }
}

// reset_game_camera_standby_flag (0x1425028): lift_d1's text (the reference)
void reset_game_camera_standby_flag(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t a0 = Ps2GprU32(ctx, 4), ra = Ps2GprU32(ctx, 31);
    {
        const uint32_t target = ra;
        WRITE32_B(a0, 4, 0u);
        
        ctx->pc = target;
        return;
    }
}

// set_object_interest_mode_target_game_camera (0x1425030): lift_d1's text (the reference)
void set_object_interest_mode_target_game_camera(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t a0 = Ps2GprU32(ctx, 4), a1 = Ps2GprU32(ctx, 5), ra = Ps2GprU32(ctx, 31);
    {
        const uint32_t target = ra;
        WRITE32_B(a0, 12560, a1);
        
        ctx->pc = target;
        return;
    }
}

// get_object_interest_mode_target_game_camera (0x1425038): lift_d1's text (the reference)
void get_object_interest_mode_target_game_camera(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t v0 = Ps2GprU32(ctx, 2), a0 = Ps2GprU32(ctx, 4), ra = Ps2GprU32(ctx, 31);
    {
        const uint32_t target = ra;
        v0 = READ32_B(a0, 12560);
        Ps2SetGpr64(ctx, 2, sx64(v0));
        ctx->pc = target;
        return;
    }
}

// begin_object_interest_game_camera (0x1425040): lift_d1's text (the reference)
void begin_object_interest_game_camera(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t v0 = Ps2GprU32(ctx, 2), a0 = Ps2GprU32(ctx, 4), ra = Ps2GprU32(ctx, 31);
    v0 = 1u;
    {
        const uint32_t target = ra;
        WRITE32_B(a0, 12564, v0);
        Ps2SetGpr64(ctx, 2, sx64(v0));
        ctx->pc = target;
        return;
    }
}

// begin_stream_lws_movie_playing_game_camera (0x14252d8): lift_d1's text (the reference)
void begin_stream_lws_movie_playing_game_camera(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t v0 = Ps2GprU32(ctx, 2), a0 = Ps2GprU32(ctx, 4), ra = Ps2GprU32(ctx, 31);
    v0 = 1u;
    {
        const uint32_t target = ra;
        WRITE32_B(a0, 13684, v0);
        Ps2SetGpr64(ctx, 2, sx64(v0));
        ctx->pc = target;
        return;
    }
}

// end_stream_lws_movie_playing_game_camera (0x14252e8): lift_d1's text (the reference)
void end_stream_lws_movie_playing_game_camera(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t a0 = Ps2GprU32(ctx, 4), ra = Ps2GprU32(ctx, 31);
    {
        const uint32_t target = ra;
        WRITE32_B(a0, 13684, 0u);
        
        ctx->pc = target;
        return;
    }
}

// is_view_angle_exist_lws_anime_mode_base (0x1425548): lift_d1's text (the reference)
void is_view_angle_exist_lws_anime_mode_base(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t v0 = Ps2GprU32(ctx, 2), a0 = Ps2GprU32(ctx, 4), ra = Ps2GprU32(ctx, 31);
    float f0 = ctx->f[0], f1 = ctx->f[1];
    uint32_t fcr31 = ctx->fcr31;
    f1 = lift_d1::flt(READ32_B(a0, 548));
    f0 = lift_d1::flt(0u);
    fcr31 = (FPU_C_OLT_S(f0, f1)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
    {
        const bool c = (fcr31 & 0x800000u) != 0u;
        v0 = 1u;
        if (!c) {
            v0 = 0u;
        }
    }
    {
        const uint32_t target = ra;
        ctx->f[0] = f0; ctx->f[1] = f1; Ps2SetGpr64(ctx, 2, sx64(v0)); ctx->fcr31 = fcr31;
        ctx->pc = target;
        return;
    }
}

// get_view_angle_lws_anime_mode_base (0x1425568): lift_d1's text (the reference)
void get_view_angle_lws_anime_mode_base(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t at = Ps2GprU32(ctx, 1), v0 = Ps2GprU32(ctx, 2), v1 = Ps2GprU32(ctx, 3), a0 = Ps2GprU32(ctx, 4), ra = Ps2GprU32(ctx, 31);
    float f0 = ctx->f[0], f1 = ctx->f[1];
    uint32_t fcr31 = ctx->fcr31;
    f1 = lift_d1::flt(0u);
    f0 = lift_d1::flt(READ32_B(a0, 548));
    fcr31 = (FPU_C_OLT_S(f1, f0)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
    {
        const bool c = (fcr31 & 0x800000u) != 0u;
        v1 = 1u;
        if (!c) {
            v1 = 0u;
        }
    }
    v0 = 1u;
    at = 0xBF800000u;
    f0 = lift_d1::flt(at);
    if (v1 == v0) {
        f0 = lift_d1::flt(READ32_B(a0, 596));
    } else {
    }
    {
        const uint32_t target = ra;
        ctx->f[0] = f0; ctx->f[1] = f1; Ps2SetGpr64(ctx, 1, sx64(at)); Ps2SetGpr64(ctx, 2, sx64(v0)); Ps2SetGpr64(ctx, 3, sx64(v1)); ctx->fcr31 = fcr31;
        ctx->pc = target;
        return;
    }
}

// getBossInterestKey (0x14256a0): lift_d1's text (the reference)
void getBossInterestKey(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t v0 = Ps2GprU32(ctx, 2), v1 = Ps2GprU32(ctx, 3), ra = Ps2GprU32(ctx, 31);
    v1 = 0x1460000u;
    {
        const uint32_t target = ra;
        v0 = READ32_B(v1, 29292);
        Ps2SetGpr64(ctx, 2, sx64(v0)); Ps2SetGpr64(ctx, 3, sx64(v1));
        ctx->pc = target;
        return;
    }
}

// getInterestKey (0x14256b0): lift_d1's text (the reference)
void getInterestKey(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t v0 = Ps2GprU32(ctx, 2), v1 = Ps2GprU32(ctx, 3), ra = Ps2GprU32(ctx, 31);
    v1 = 0x1460000u;
    {
        const uint32_t target = ra;
        v0 = READ32_B(v1, 29300);
        Ps2SetGpr64(ctx, 2, sx64(v0)); Ps2SetGpr64(ctx, 3, sx64(v1));
        ctx->pc = target;
        return;
    }
}

// getZoomKey (0x14256c0): lift_d1's text (the reference)
void getZoomKey(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t v0 = Ps2GprU32(ctx, 2), v1 = Ps2GprU32(ctx, 3), ra = Ps2GprU32(ctx, 31);
    v1 = 0x1460000u;
    {
        const uint32_t target = ra;
        v0 = READ32_B(v1, 29304);
        Ps2SetGpr64(ctx, 2, sx64(v0)); Ps2SetGpr64(ctx, 3, sx64(v1));
        ctx->pc = target;
        return;
    }
}

// setBossInterestKey (0x14256d0): lift_d1's text (the reference)
void setBossInterestKey(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t v0 = Ps2GprU32(ctx, 2), a0 = Ps2GprU32(ctx, 4), ra = Ps2GprU32(ctx, 31);
    v0 = 0x1460000u;
    {
        const uint32_t target = ra;
        WRITE32_B(v0, 29292, a0);
        Ps2SetGpr64(ctx, 2, sx64(v0));
        ctx->pc = target;
        return;
    }
}

// setInterestKey (0x14256e0): lift_d1's text (the reference)
void setInterestKey(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t v0 = Ps2GprU32(ctx, 2), a0 = Ps2GprU32(ctx, 4), ra = Ps2GprU32(ctx, 31);
    v0 = 0x1460000u;
    {
        const uint32_t target = ra;
        WRITE32_B(v0, 29300, a0);
        Ps2SetGpr64(ctx, 2, sx64(v0));
        ctx->pc = target;
        return;
    }
}

// setZoomKey (0x14256f0): lift_d1's text (the reference)
void setZoomKey(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t v0 = Ps2GprU32(ctx, 2), a0 = Ps2GprU32(ctx, 4), ra = Ps2GprU32(ctx, 31);
    v0 = 0x1460000u;
    {
        const uint32_t target = ra;
        WRITE32_B(v0, 29304, a0);
        Ps2SetGpr64(ctx, 2, sx64(v0));
        ctx->pc = target;
        return;
    }
}

// gamecore_sub_01425700 (0x1425700): lift_d1's text (the reference)
void gamecore_sub_01425700(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t v1 = Ps2GprU32(ctx, 3), a0 = Ps2GprU32(ctx, 4), ra = Ps2GprU32(ctx, 31);
    uint64_t v0 = Ps2GprU64(ctx, 2), a1 = Ps2GprU64(ctx, 5);
    bool df_v1 = false, df_a0 = false;
    {
        const bool c = (int64_t)a1 != 0;
        WRITE32_B(a0, 144, (uint32_t)a1);
        if (!c) {
            a0 = READ32_B(a0, 140); df_a0 = true;
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
        const uint32_t target = ra;
        Ps2SetGpr64(ctx, 2, v0); if (df_v1) Ps2SetGpr64(ctx, 3, sx64(v1)); if (df_a0) Ps2SetGpr64(ctx, 4, sx64(a0));
        ctx->pc = target;
        return;
    }
}

// gamecore_sub_01425788 (0x1425788): lift_d1's text (the reference)
void gamecore_sub_01425788(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t v0 = Ps2GprU32(ctx, 2), a0 = Ps2GprU32(ctx, 4), ra = Ps2GprU32(ctx, 31);
    float f0 = ctx->f[0], f1 = ctx->f[1];
    uint32_t fcr31 = ctx->fcr31;
    f1 = lift_d1::flt(READ32_B(a0, 6184));
    f0 = lift_d1::flt(0u);
    fcr31 = (FPU_C_OLT_S(f0, f1)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
    {
        const bool c = (fcr31 & 0x800000u) != 0u;
        v0 = 1u;
        if (!c) {
            v0 = 0u;
        }
    }
    {
        const uint32_t target = ra;
        ctx->f[0] = f0; ctx->f[1] = f1; Ps2SetGpr64(ctx, 2, sx64(v0)); ctx->fcr31 = fcr31;
        ctx->pc = target;
        return;
    }
}

// gamecore_sub_014257A8 (0x14257a8): lift_d1's text (the reference)
void gamecore_sub_014257A8(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t a0 = Ps2GprU32(ctx, 4), ra = Ps2GprU32(ctx, 31);
    {
        const uint32_t target = ra;
        WRITE32(a0, 0u);
        
        ctx->pc = target;
        return;
    }
}

// gamecore_sub_01425AC0 (0x1425ac0): lift_d1's text (the reference)
void gamecore_sub_01425AC0(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t v0 = Ps2GprU32(ctx, 2), v1 = Ps2GprU32(ctx, 3), a0 = Ps2GprU32(ctx, 4), a1 = Ps2GprU32(ctx, 5), a2 = Ps2GprU32(ctx, 6), a3 = Ps2GprU32(ctx, 7), t0 = Ps2GprU32(ctx, 8), t1 = Ps2GprU32(ctx, 9), ra = Ps2GprU32(ctx, 31);
    v0 = a1 + 0x5C8u;
    v1 = a1 + 0x4Cu;
    a2 = a1 + 0x1E0u;
    a3 = a1 + 0x2ACu;
    t0 = a1 + 0x330u;
    t1 = a1 + 0x400u;
    a1 = a1 + 0x4E4u;
    WRITE32_B(a0, 12576, v0);
    WRITE32_B(a0, 4976, v1);
    WRITE32_B(a0, 11332, a2);
    WRITE32_B(a0, 11552, a3);
    WRITE32_B(a0, 11744, t0);
    WRITE32_B(a0, 11984, t1);
    {
        const uint32_t target = ra;
        WRITE32_B(a0, 12272, a1);
        Ps2SetGpr64(ctx, 2, sx64(v0)); Ps2SetGpr64(ctx, 3, sx64(v1)); Ps2SetGpr64(ctx, 5, sx64(a1)); Ps2SetGpr64(ctx, 6, sx64(a2)); Ps2SetGpr64(ctx, 7, sx64(a3)); Ps2SetGpr64(ctx, 8, sx64(t0)); Ps2SetGpr64(ctx, 9, sx64(t1));
        ctx->pc = target;
        return;
    }
}

// gamecore_sub_014262A0 (0x14262a0): lift_d1's text (the reference)
void gamecore_sub_014262A0(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t a1 = Ps2GprU32(ctx, 5), ra = Ps2GprU32(ctx, 31);
    WRITE32_B(a1, 152, 0u);
    {
        const uint32_t target = ra;
        WRITE32_B(a1, 148, 0u);
        
        ctx->pc = target;
        return;
    }
}

// gamecore_sub_01426B18 (0x1426b18): lift_d1's text (the reference)
void gamecore_sub_01426B18(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t a0 = Ps2GprU32(ctx, 4), ra = Ps2GprU32(ctx, 31);
    float f0 = ctx->f[0];
    {
        const uint32_t target = ra;
        f0 = lift_d1::flt(READ32_B(a0, 592));
        ctx->f[0] = f0;
        ctx->pc = target;
        return;
    }
}

// gamecore_sub_01426B20 (0x1426b20): lift_d1's text (the reference)
void gamecore_sub_01426B20(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t a0 = Ps2GprU32(ctx, 4), ra = Ps2GprU32(ctx, 31);
    float f0 = ctx->f[0];
    {
        const uint32_t target = ra;
        f0 = lift_d1::flt(READ32_B(a0, 600));
        ctx->f[0] = f0;
        ctx->pc = target;
        return;
    }
}

// gamecore_sub_01427230 (0x1427230): lift_d1's text (the reference)
void gamecore_sub_01427230(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t ra = Ps2GprU32(ctx, 31);
    uint64_t v0 = Ps2GprU64(ctx, 2), v1 = Ps2GprU64(ctx, 3), a0 = Ps2GprU64(ctx, 4);
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
                const uint32_t target = ra;
                (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 1))));
                Ps2SetGpr64(ctx, 2, v0); Ps2SetGpr64(ctx, 3, v1);
                ctx->pc = target;
                return;
            }
        }
    }
L_1427268:
    {
        const uint32_t target = ra;
        (v0 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL));
        Ps2SetGpr64(ctx, 2, v0); Ps2SetGpr64(ctx, 3, v1);
        ctx->pc = target;
        return;
    }
}

// gamecore_sub_01428CD0 (0x1428cd0): lift_d1's text (the reference)
void gamecore_sub_01428CD0(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t v0 = Ps2GprU32(ctx, 2), v1 = Ps2GprU32(ctx, 3), a0 = Ps2GprU32(ctx, 4), a1 = Ps2GprU32(ctx, 5), a2 = Ps2GprU32(ctx, 6), ra = Ps2GprU32(ctx, 31);
    float f0 = ctx->f[0], f1 = ctx->f[1], f2 = ctx->f[2], f3 = ctx->f[3], f12 = ctx->f[12], f13 = ctx->f[13];
    uint32_t fcr31 = ctx->fcr31;
    f2 = lift_d1::flt(0u);
    f0 = lift_d1::flt(READ32_B(a1, 28));
    fcr31 = (FPU_C_OLT_S(f2, f0)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
    if ((fcr31 & 0x800000u) == 0u) {
        f0 = lift_d1::flt(READ32_B(a1, 32));
    } else {
        f1 = lift_d1::flt(READ32_B(a2, 28));
        fcr31 = (FPU_C_OLT_S(f2, f1)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
        if ((fcr31 & 0x800000u) == 0u) {
            f0 = lift_d1::flt(READ32_B(a1, 32));
        } else {
            f0 = FPU_MUL_S(f12, f0);
            f1 = FPU_MUL_S(f13, f1);
            f0 = FPU_ADD_SF(f0, f1);
            WRITE32_B(a0, 28, lift_d1::bits(f0));
            f0 = lift_d1::flt(READ32_B(a1, 32));
            f2 = lift_d1::flt(0u);
        }
    }
    fcr31 = (FPU_C_OLT_S(f2, f0)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
    if ((fcr31 & 0x800000u) == 0u) {
        f0 = lift_d1::flt(READ32_B(a1, 36));
    } else {
        f1 = lift_d1::flt(READ32_B(a2, 32));
        fcr31 = (FPU_C_OLT_S(f2, f1)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
        if ((fcr31 & 0x800000u) == 0u) {
            f0 = lift_d1::flt(READ32_B(a1, 36));
        } else {
            f0 = FPU_MUL_S(f12, f0);
            f1 = FPU_MUL_S(f13, f1);
            f0 = FPU_ADD_SF(f0, f1);
            WRITE32_B(a0, 32, lift_d1::bits(f0));
            f0 = lift_d1::flt(READ32_B(a1, 36));
        }
    }
    f2 = lift_d1::flt(0u);
    fcr31 = (FPU_C_OLT_S(f2, f0)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
    if ((fcr31 & 0x800000u) == 0u) {
        f0 = lift_d1::flt(READ32_B(a1, 40));
    } else {
        f1 = lift_d1::flt(READ32_B(a2, 36));
        fcr31 = (FPU_C_OLT_S(f2, f1)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
        if ((fcr31 & 0x800000u) == 0u) {
            f0 = lift_d1::flt(READ32_B(a1, 40));
        } else {
            f0 = FPU_MUL_S(f12, f0);
            f1 = FPU_MUL_S(f13, f1);
            f0 = FPU_ADD_SF(f0, f1);
            WRITE32_B(a0, 36, lift_d1::bits(f0));
            f0 = lift_d1::flt(READ32_B(a1, 40));
        }
    }
    f1 = lift_d1::flt(READ32_B(a2, 40));
    f0 = FPU_MUL_S(f12, f0);
    f1 = FPU_MUL_S(f13, f1);
    f3 = lift_d1::flt(0u);
    f0 = FPU_ADD_SF(f0, f1);
    WRITE32_B(a0, 40, lift_d1::bits(f0));
    f1 = lift_d1::flt(READ32_B(a1, 52));
    f0 = lift_d1::flt(READ32_B(a2, 52));
    f1 = FPU_MUL_S(f12, f1);
    f0 = FPU_MUL_S(f13, f0);
    f1 = FPU_ADD_SF(f1, f0);
    WRITE32_B(a0, 52, lift_d1::bits(f1));
    f2 = lift_d1::flt(READ32_B(a1, 56));
    f0 = lift_d1::flt(READ32_B(a2, 56));
    f2 = FPU_MUL_S(f12, f2);
    f0 = FPU_MUL_S(f13, f0);
    f2 = FPU_ADD_SF(f2, f0);
    WRITE32_B(a0, 56, lift_d1::bits(f2));
    f1 = lift_d1::flt(READ32_B(a1, 60));
    f0 = lift_d1::flt(READ32_B(a2, 60));
    f1 = FPU_MUL_S(f12, f1);
    f0 = FPU_MUL_S(f13, f0);
    f1 = FPU_ADD_SF(f1, f0);
    WRITE32_B(a0, 60, lift_d1::bits(f1));
    f2 = lift_d1::flt(READ32_B(a1, 64));
    f0 = lift_d1::flt(READ32_B(a2, 64));
    f2 = FPU_MUL_S(f12, f2);
    f0 = FPU_MUL_S(f13, f0);
    f2 = FPU_ADD_SF(f2, f0);
    WRITE32_B(a0, 64, lift_d1::bits(f2));
    f0 = lift_d1::flt(READ32_B(a1, 68));
    fcr31 = (FPU_C_OLT_S(f3, f0)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
    if ((fcr31 & 0x800000u) == 0u) {
        f0 = lift_d1::flt(READ32_B(a1, 72));
    } else {
        f1 = lift_d1::flt(READ32_B(a2, 68));
        fcr31 = (FPU_C_OLT_S(f3, f1)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
        if ((fcr31 & 0x800000u) == 0u) {
            f0 = lift_d1::flt(READ32_B(a1, 72));
        } else {
            f0 = FPU_MUL_S(f12, f0);
            f1 = FPU_MUL_S(f13, f1);
            f0 = FPU_ADD_SF(f0, f1);
            WRITE32_B(a0, 68, lift_d1::bits(f0));
            f0 = lift_d1::flt(READ32_B(a1, 72));
        }
    }
    f2 = lift_d1::flt(0u);
    fcr31 = (FPU_C_OLT_S(f2, f0)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
    if ((fcr31 & 0x800000u) == 0u) {
        f0 = lift_d1::flt(READ32_B(a1, 76));
    } else {
        f1 = lift_d1::flt(READ32_B(a2, 72));
        fcr31 = (FPU_C_OLT_S(f2, f1)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
        if ((fcr31 & 0x800000u) == 0u) {
            f0 = lift_d1::flt(READ32_B(a1, 76));
        } else {
            f0 = FPU_MUL_S(f12, f0);
            f1 = FPU_MUL_S(f13, f1);
            f0 = FPU_ADD_SF(f0, f1);
            WRITE32_B(a0, 72, lift_d1::bits(f0));
            f0 = lift_d1::flt(READ32_B(a1, 76));
        }
    }
    f1 = lift_d1::flt(READ32_B(a2, 76));
    f0 = FPU_MUL_S(f12, f0);
    f1 = FPU_MUL_S(f13, f1);
    f3 = lift_d1::flt(0u);
    f0 = FPU_ADD_SF(f0, f1);
    WRITE32_B(a0, 76, lift_d1::bits(f0));
    f1 = lift_d1::flt(READ32_B(a1, 80));
    f0 = lift_d1::flt(READ32_B(a2, 80));
    f1 = FPU_MUL_S(f12, f1);
    f0 = FPU_MUL_S(f13, f0);
    f1 = FPU_ADD_SF(f1, f0);
    WRITE32_B(a0, 80, lift_d1::bits(f1));
    f2 = lift_d1::flt(READ32_B(a1, 84));
    f0 = lift_d1::flt(READ32_B(a2, 84));
    f2 = FPU_MUL_S(f12, f2);
    f0 = FPU_MUL_S(f13, f0);
    f2 = FPU_ADD_SF(f2, f0);
    WRITE32_B(a0, 84, lift_d1::bits(f2));
    f1 = lift_d1::flt(READ32_B(a1, 88));
    f0 = lift_d1::flt(READ32_B(a2, 88));
    f1 = FPU_MUL_S(f12, f1);
    f0 = FPU_MUL_S(f13, f0);
    f1 = FPU_ADD_SF(f1, f0);
    WRITE32_B(a0, 88, lift_d1::bits(f1));
    f2 = lift_d1::flt(READ32_B(a1, 92));
    f0 = lift_d1::flt(READ32_B(a2, 92));
    f2 = FPU_MUL_S(f12, f2);
    f0 = FPU_MUL_S(f13, f0);
    f2 = FPU_ADD_SF(f2, f0);
    WRITE32_B(a0, 92, lift_d1::bits(f2));
    f1 = lift_d1::flt(READ32_B(a1, 96));
    f0 = lift_d1::flt(READ32_B(a2, 96));
    f1 = FPU_MUL_S(f12, f1);
    f0 = FPU_MUL_S(f13, f0);
    f1 = FPU_ADD_SF(f1, f0);
    WRITE32_B(a0, 96, lift_d1::bits(f1));
    f2 = lift_d1::flt(READ32_B(a1, 100));
    f0 = lift_d1::flt(READ32_B(a2, 100));
    f2 = FPU_MUL_S(f12, f2);
    f0 = FPU_MUL_S(f13, f0);
    f2 = FPU_ADD_SF(f2, f0);
    WRITE32_B(a0, 100, lift_d1::bits(f2));
    f1 = lift_d1::flt(READ32_B(a1, 104));
    f0 = lift_d1::flt(READ32_B(a2, 104));
    f1 = FPU_MUL_S(f12, f1);
    f0 = FPU_MUL_S(f13, f0);
    f1 = FPU_ADD_SF(f1, f0);
    WRITE32_B(a0, 104, lift_d1::bits(f1));
    f2 = lift_d1::flt(READ32_B(a1, 108));
    f0 = lift_d1::flt(READ32_B(a2, 108));
    f2 = FPU_MUL_S(f12, f2);
    f0 = FPU_MUL_S(f13, f0);
    f2 = FPU_ADD_SF(f2, f0);
    WRITE32_B(a0, 108, lift_d1::bits(f2));
    f0 = lift_d1::flt(READ32_B(a1, 112));
    fcr31 = (FPU_C_OLT_S(f3, f0)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
    if ((fcr31 & 0x800000u) == 0u) {
        f0 = lift_d1::flt(READ32_B(a1, 116));
    } else {
        f1 = lift_d1::flt(READ32_B(a2, 112));
        fcr31 = (FPU_C_OLT_S(f3, f1)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
        if ((fcr31 & 0x800000u) == 0u) {
            f0 = lift_d1::flt(READ32_B(a1, 116));
        } else {
            f0 = FPU_MUL_S(f12, f0);
            f1 = FPU_MUL_S(f13, f1);
            f0 = FPU_ADD_SF(f0, f1);
            WRITE32_B(a0, 112, lift_d1::bits(f0));
            f0 = lift_d1::flt(READ32_B(a1, 116));
        }
    }
    f2 = lift_d1::flt(0u);
    fcr31 = (FPU_C_OLT_S(f2, f0)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
    if ((fcr31 & 0x800000u) == 0u) {
        f0 = lift_d1::flt(READ32_B(a1, 120));
    } else {
        f1 = lift_d1::flt(READ32_B(a2, 116));
        fcr31 = (FPU_C_OLT_S(f2, f1)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
        if ((fcr31 & 0x800000u) == 0u) {
            f0 = lift_d1::flt(READ32_B(a1, 120));
        } else {
            f0 = FPU_MUL_S(f12, f0);
            f1 = FPU_MUL_S(f13, f1);
            f0 = FPU_ADD_SF(f0, f1);
            WRITE32_B(a0, 116, lift_d1::bits(f0));
            f0 = lift_d1::flt(READ32_B(a1, 120));
        }
    }
    f1 = lift_d1::flt(READ32_B(a2, 120));
    f0 = FPU_MUL_S(f12, f0);
    f1 = FPU_MUL_S(f13, f1);
    f2 = lift_d1::flt(0u);
    f0 = FPU_ADD_SF(f0, f1);
    WRITE32_B(a0, 120, lift_d1::bits(f0));
    f0 = lift_d1::flt(READ32_B(a1, 124));
    fcr31 = (FPU_C_OLT_S(f2, f0)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
    if ((fcr31 & 0x800000u) == 0u) {
        f0 = lift_d1::flt(READ32_B(a1, 128));
    } else {
        f1 = lift_d1::flt(READ32_B(a2, 124));
        fcr31 = (FPU_C_OLT_S(f2, f1)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
        if ((fcr31 & 0x800000u) == 0u) {
            f0 = lift_d1::flt(READ32_B(a1, 128));
        } else {
            f0 = FPU_MUL_S(f12, f0);
            f1 = FPU_MUL_S(f13, f1);
            f0 = FPU_ADD_SF(f0, f1);
            WRITE32_B(a0, 124, lift_d1::bits(f0));
            f0 = lift_d1::flt(READ32_B(a1, 128));
        }
    }
    f1 = lift_d1::flt(READ32_B(a2, 128));
    f0 = FPU_MUL_S(f12, f0);
    f1 = FPU_MUL_S(f13, f1);
    f3 = lift_d1::flt(0u);
    f0 = FPU_ADD_SF(f0, f1);
    WRITE32_B(a0, 128, lift_d1::bits(f0));
    f1 = lift_d1::flt(READ32_B(a1, 132));
    f0 = lift_d1::flt(READ32_B(a2, 132));
    f1 = FPU_MUL_S(f12, f1);
    f0 = FPU_MUL_S(f13, f0);
    f1 = FPU_ADD_SF(f1, f0);
    WRITE32_B(a0, 132, lift_d1::bits(f1));
    f2 = lift_d1::flt(READ32_B(a1, 136));
    f0 = lift_d1::flt(READ32_B(a2, 136));
    f2 = FPU_MUL_S(f12, f2);
    f0 = FPU_MUL_S(f13, f0);
    f2 = FPU_ADD_SF(f2, f0);
    WRITE32_B(a0, 136, lift_d1::bits(f2));
    f1 = lift_d1::flt(READ32_B(a1, 172));
    f0 = lift_d1::flt(READ32_B(a2, 172));
    f1 = FPU_MUL_S(f12, f1);
    f0 = FPU_MUL_S(f13, f0);
    f1 = FPU_ADD_SF(f1, f0);
    WRITE32_B(a0, 172, lift_d1::bits(f1));
    f2 = lift_d1::flt(READ32_B(a1, 176));
    f0 = lift_d1::flt(READ32_B(a2, 176));
    f2 = FPU_MUL_S(f12, f2);
    f0 = FPU_MUL_S(f13, f0);
    f2 = FPU_ADD_SF(f2, f0);
    WRITE32_B(a0, 176, lift_d1::bits(f2));
    f0 = lift_d1::flt(READ32_B(a1, 184));
    fcr31 = (FPU_C_OLT_S(f3, f0)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
    if ((fcr31 & 0x800000u) == 0u) {
        f0 = lift_d1::flt(READ32_B(a1, 188));
    } else {
        f1 = lift_d1::flt(READ32_B(a2, 184));
        fcr31 = (FPU_C_OLT_S(f3, f1)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
        if ((fcr31 & 0x800000u) == 0u) {
            f0 = lift_d1::flt(READ32_B(a1, 188));
        } else {
            f0 = FPU_MUL_S(f12, f0);
            f1 = FPU_MUL_S(f13, f1);
            f0 = FPU_ADD_SF(f0, f1);
            WRITE32_B(a0, 184, lift_d1::bits(f0));
            f0 = lift_d1::flt(READ32_B(a1, 188));
        }
    }
    f2 = lift_d1::flt(0u);
    fcr31 = (FPU_C_OLT_S(f2, f0)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
    if ((fcr31 & 0x800000u) == 0u) {
        f0 = lift_d1::flt(READ32_B(a1, 192));
    } else {
        f1 = lift_d1::flt(READ32_B(a2, 188));
        fcr31 = (FPU_C_OLT_S(f2, f1)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
        if ((fcr31 & 0x800000u) == 0u) {
            f0 = lift_d1::flt(READ32_B(a1, 192));
        } else {
            f0 = FPU_MUL_S(f12, f0);
            f1 = FPU_MUL_S(f13, f1);
            f0 = FPU_ADD_SF(f0, f1);
            WRITE32_B(a0, 188, lift_d1::bits(f0));
            f0 = lift_d1::flt(READ32_B(a1, 192));
            f2 = lift_d1::flt(0u);
        }
    }
    fcr31 = (FPU_C_OLT_S(f2, f0)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
    if ((fcr31 & 0x800000u) == 0u) {
        f0 = lift_d1::flt(READ32_B(a1, 196));
    } else {
        f1 = lift_d1::flt(READ32_B(a2, 192));
        fcr31 = (FPU_C_OLT_S(f2, f1)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
        if ((fcr31 & 0x800000u) == 0u) {
            f0 = lift_d1::flt(READ32_B(a1, 196));
        } else {
            f0 = FPU_MUL_S(f12, f0);
            f1 = FPU_MUL_S(f13, f1);
            f0 = FPU_ADD_SF(f0, f1);
            WRITE32_B(a0, 192, lift_d1::bits(f0));
            f0 = lift_d1::flt(READ32_B(a1, 196));
        }
    }
    f2 = lift_d1::flt(0u);
    fcr31 = (FPU_C_OLT_S(f2, f0)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
    if ((fcr31 & 0x800000u) == 0u) {
        f0 = lift_d1::flt(READ32_B(a1, 212));
    } else {
        f1 = lift_d1::flt(READ32_B(a2, 196));
        fcr31 = (FPU_C_OLT_S(f2, f1)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
        if ((fcr31 & 0x800000u) == 0u) {
            f0 = lift_d1::flt(READ32_B(a1, 212));
        } else {
            f0 = FPU_MUL_S(f12, f0);
            f1 = FPU_MUL_S(f13, f1);
            f0 = FPU_ADD_SF(f0, f1);
            WRITE32_B(a0, 196, lift_d1::bits(f0));
            f0 = lift_d1::flt(READ32_B(a1, 212));
            f2 = lift_d1::flt(0u);
        }
    }
    fcr31 = (FPU_C_OLT_S(f2, f0)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
    if ((fcr31 & 0x800000u) == 0u) {
        f0 = lift_d1::flt(READ32_B(a1, 216));
    } else {
        f1 = lift_d1::flt(READ32_B(a2, 212));
        fcr31 = (FPU_C_OLT_S(f2, f1)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
        if ((fcr31 & 0x800000u) == 0u) {
            f0 = lift_d1::flt(READ32_B(a1, 216));
        } else {
            f0 = FPU_MUL_S(f12, f0);
            f1 = FPU_MUL_S(f13, f1);
            f0 = FPU_ADD_SF(f0, f1);
            WRITE32_B(a0, 212, lift_d1::bits(f0));
            f0 = lift_d1::flt(READ32_B(a1, 216));
        }
    }
    f2 = lift_d1::flt(0u);
    fcr31 = (FPU_C_OLT_S(f2, f0)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
    if ((fcr31 & 0x800000u) == 0u) {
        f0 = lift_d1::flt(READ32_B(a1, 220));
    } else {
        f1 = lift_d1::flt(READ32_B(a2, 216));
        fcr31 = (FPU_C_OLT_S(f2, f1)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
        if ((fcr31 & 0x800000u) == 0u) {
            f0 = lift_d1::flt(READ32_B(a1, 220));
        } else {
            f0 = FPU_MUL_S(f12, f0);
            f1 = FPU_MUL_S(f13, f1);
            f0 = FPU_ADD_SF(f0, f1);
            WRITE32_B(a0, 216, lift_d1::bits(f0));
            f0 = lift_d1::flt(READ32_B(a1, 220));
            f2 = lift_d1::flt(0u);
        }
    }
    fcr31 = (FPU_C_OLT_S(f2, f0)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
    if ((fcr31 & 0x800000u) == 0u) {
        f0 = lift_d1::flt(READ32_B(a1, 224));
    } else {
        f1 = lift_d1::flt(READ32_B(a2, 220));
        fcr31 = (FPU_C_OLT_S(f2, f1)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
        if ((fcr31 & 0x800000u) == 0u) {
            f0 = lift_d1::flt(READ32_B(a1, 224));
        } else {
            f0 = FPU_MUL_S(f12, f0);
            f1 = FPU_MUL_S(f13, f1);
            f0 = FPU_ADD_SF(f0, f1);
            WRITE32_B(a0, 220, lift_d1::bits(f0));
            f0 = lift_d1::flt(READ32_B(a1, 224));
        }
    }
    f1 = lift_d1::flt(READ32_B(a2, 224));
    f0 = FPU_MUL_S(f12, f0);
    f1 = FPU_MUL_S(f13, f1);
    f3 = lift_d1::flt(0u);
    f0 = FPU_ADD_SF(f0, f1);
    WRITE32_B(a0, 224, lift_d1::bits(f0));
    f1 = lift_d1::flt(READ32_B(a1, 228));
    f0 = lift_d1::flt(READ32_B(a2, 228));
    f1 = FPU_MUL_S(f12, f1);
    f0 = FPU_MUL_S(f13, f0);
    f1 = FPU_ADD_SF(f1, f0);
    WRITE32_B(a0, 228, lift_d1::bits(f1));
    f2 = lift_d1::flt(READ32_B(a1, 236));
    f0 = lift_d1::flt(READ32_B(a2, 236));
    f2 = FPU_MUL_S(f12, f2);
    f0 = FPU_MUL_S(f13, f0);
    f2 = FPU_ADD_SF(f2, f0);
    WRITE32_B(a0, 236, lift_d1::bits(f2));
    f1 = lift_d1::flt(READ32_B(a1, 240));
    f0 = lift_d1::flt(READ32_B(a2, 240));
    f1 = FPU_MUL_S(f12, f1);
    f0 = FPU_MUL_S(f13, f0);
    f1 = FPU_ADD_SF(f1, f0);
    WRITE32_B(a0, 240, lift_d1::bits(f1));
    f2 = lift_d1::flt(READ32_B(a1, 244));
    f0 = lift_d1::flt(READ32_B(a2, 244));
    f2 = FPU_MUL_S(f12, f2);
    f0 = FPU_MUL_S(f13, f0);
    f2 = FPU_ADD_SF(f2, f0);
    WRITE32_B(a0, 244, lift_d1::bits(f2));
    f1 = lift_d1::flt(READ32_B(a1, 248));
    f0 = lift_d1::flt(READ32_B(a2, 248));
    f1 = FPU_MUL_S(f12, f1);
    f0 = FPU_MUL_S(f13, f0);
    f1 = FPU_ADD_SF(f1, f0);
    WRITE32_B(a0, 248, lift_d1::bits(f1));
    f2 = lift_d1::flt(READ32_B(a1, 256));
    f0 = lift_d1::flt(READ32_B(a2, 256));
    f2 = FPU_MUL_S(f12, f2);
    f0 = FPU_MUL_S(f13, f0);
    f2 = FPU_ADD_SF(f2, f0);
    WRITE32_B(a0, 256, lift_d1::bits(f2));
    f1 = lift_d1::flt(READ32_B(a1, 260));
    f0 = lift_d1::flt(READ32_B(a2, 260));
    f1 = FPU_MUL_S(f12, f1);
    f0 = FPU_MUL_S(f13, f0);
    f1 = FPU_ADD_SF(f1, f0);
    WRITE32_B(a0, 260, lift_d1::bits(f1));
    f2 = lift_d1::flt(READ32_B(a1, 264));
    f0 = lift_d1::flt(READ32_B(a2, 264));
    f2 = FPU_MUL_S(f12, f2);
    f0 = FPU_MUL_S(f13, f0);
    f2 = FPU_ADD_SF(f2, f0);
    WRITE32_B(a0, 264, lift_d1::bits(f2));
    f1 = lift_d1::flt(READ32_B(a1, 268));
    f0 = lift_d1::flt(READ32_B(a2, 268));
    f1 = FPU_MUL_S(f12, f1);
    f0 = FPU_MUL_S(f13, f0);
    f1 = FPU_ADD_SF(f1, f0);
    WRITE32_B(a0, 268, lift_d1::bits(f1));
    f2 = lift_d1::flt(READ32_B(a1, 272));
    f0 = lift_d1::flt(READ32_B(a2, 272));
    f2 = FPU_MUL_S(f12, f2);
    f0 = FPU_MUL_S(f13, f0);
    f2 = FPU_ADD_SF(f2, f0);
    WRITE32_B(a0, 272, lift_d1::bits(f2));
    f0 = lift_d1::flt(READ32_B(a1, 284));
    fcr31 = (FPU_C_OLT_S(f3, f0)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
    if ((fcr31 & 0x800000u) == 0u) {
        f0 = lift_d1::flt(READ32_B(a1, 288));
    } else {
        f1 = lift_d1::flt(READ32_B(a2, 284));
        fcr31 = (FPU_C_OLT_S(f3, f1)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
        if ((fcr31 & 0x800000u) == 0u) {
            f0 = lift_d1::flt(READ32_B(a1, 288));
        } else {
            f0 = FPU_MUL_S(f12, f0);
            f1 = FPU_MUL_S(f13, f1);
            f0 = FPU_ADD_SF(f0, f1);
            WRITE32_B(a0, 284, lift_d1::bits(f0));
            f0 = lift_d1::flt(READ32_B(a1, 288));
        }
    }
    f2 = lift_d1::flt(0u);
    fcr31 = (FPU_C_OLT_S(f2, f0)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
    if ((fcr31 & 0x800000u) == 0u) {
        f0 = lift_d1::flt(READ32_B(a1, 292));
    } else {
        f1 = lift_d1::flt(READ32_B(a2, 288));
        fcr31 = (FPU_C_OLT_S(f2, f1)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
        if ((fcr31 & 0x800000u) == 0u) {
            f0 = lift_d1::flt(READ32_B(a1, 292));
        } else {
            f0 = FPU_MUL_S(f12, f0);
            f1 = FPU_MUL_S(f13, f1);
            f0 = FPU_ADD_SF(f0, f1);
            WRITE32_B(a0, 288, lift_d1::bits(f0));
            f0 = lift_d1::flt(READ32_B(a1, 292));
            f2 = lift_d1::flt(0u);
        }
    }
    fcr31 = (FPU_C_OLT_S(f2, f0)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
    if ((fcr31 & 0x800000u) == 0u) {
        f0 = lift_d1::flt(READ32_B(a1, 296));
    } else {
        f1 = lift_d1::flt(READ32_B(a2, 292));
        fcr31 = (FPU_C_OLT_S(f2, f1)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
        if ((fcr31 & 0x800000u) == 0u) {
            f0 = lift_d1::flt(READ32_B(a1, 296));
        } else {
            f0 = FPU_MUL_S(f12, f0);
            f1 = FPU_MUL_S(f13, f1);
            f0 = FPU_ADD_SF(f0, f1);
            WRITE32_B(a0, 292, lift_d1::bits(f0));
            f0 = lift_d1::flt(READ32_B(a1, 296));
        }
    }
    f1 = lift_d1::flt(READ32_B(a2, 296));
    f0 = FPU_MUL_S(f12, f0);
    f1 = FPU_MUL_S(f13, f1);
    f3 = lift_d1::flt(0u);
    f0 = FPU_ADD_SF(f0, f1);
    WRITE32_B(a0, 296, lift_d1::bits(f0));
    f1 = lift_d1::flt(READ32_B(a1, 300));
    f0 = lift_d1::flt(READ32_B(a2, 300));
    f1 = FPU_MUL_S(f12, f1);
    f0 = FPU_MUL_S(f13, f0);
    f1 = FPU_ADD_SF(f1, f0);
    WRITE32_B(a0, 300, lift_d1::bits(f1));
    f2 = lift_d1::flt(READ32_B(a1, 304));
    f0 = lift_d1::flt(READ32_B(a2, 304));
    f2 = FPU_MUL_S(f12, f2);
    f0 = FPU_MUL_S(f13, f0);
    f2 = FPU_ADD_SF(f2, f0);
    WRITE32_B(a0, 304, lift_d1::bits(f2));
    f1 = lift_d1::flt(READ32_B(a1, 308));
    f0 = lift_d1::flt(READ32_B(a2, 308));
    f1 = FPU_MUL_S(f12, f1);
    f0 = FPU_MUL_S(f13, f0);
    f1 = FPU_ADD_SF(f1, f0);
    WRITE32_B(a0, 308, lift_d1::bits(f1));
    f2 = lift_d1::flt(READ32_B(a1, 312));
    f0 = lift_d1::flt(READ32_B(a2, 312));
    f2 = FPU_MUL_S(f12, f2);
    f0 = FPU_MUL_S(f13, f0);
    f2 = FPU_ADD_SF(f2, f0);
    WRITE32_B(a0, 312, lift_d1::bits(f2));
    f1 = lift_d1::flt(READ32_B(a1, 316));
    f0 = lift_d1::flt(READ32_B(a2, 316));
    f1 = FPU_MUL_S(f12, f1);
    f0 = FPU_MUL_S(f13, f0);
    f1 = FPU_ADD_SF(f1, f0);
    WRITE32_B(a0, 316, lift_d1::bits(f1));
    f0 = lift_d1::flt(READ32_B(a1, 324));
    fcr31 = (FPU_C_OLT_S(f3, f0)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
    if ((fcr31 & 0x800000u) == 0u) {
        f0 = lift_d1::flt(READ32_B(a1, 328));
    } else {
        f1 = lift_d1::flt(READ32_B(a2, 324));
        fcr31 = (FPU_C_OLT_S(f3, f1)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
        if ((fcr31 & 0x800000u) == 0u) {
            f0 = lift_d1::flt(READ32_B(a1, 328));
        } else {
            f0 = FPU_MUL_S(f12, f0);
            f1 = FPU_MUL_S(f13, f1);
            f0 = FPU_ADD_SF(f0, f1);
            WRITE32_B(a0, 324, lift_d1::bits(f0));
            f0 = lift_d1::flt(READ32_B(a1, 328));
        }
    }
    f2 = lift_d1::flt(0u);
    fcr31 = (FPU_C_OLT_S(f2, f0)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
    if ((fcr31 & 0x800000u) == 0u) {
        f0 = lift_d1::flt(READ32_B(a1, 332));
    } else {
        f1 = lift_d1::flt(READ32_B(a2, 328));
        fcr31 = (FPU_C_OLT_S(f2, f1)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
        if ((fcr31 & 0x800000u) == 0u) {
            f0 = lift_d1::flt(READ32_B(a1, 332));
        } else {
            f0 = FPU_MUL_S(f12, f0);
            f1 = FPU_MUL_S(f13, f1);
            f0 = FPU_ADD_SF(f0, f1);
            WRITE32_B(a0, 328, lift_d1::bits(f0));
            f0 = lift_d1::flt(READ32_B(a1, 332));
            f2 = lift_d1::flt(0u);
        }
    }
    fcr31 = (FPU_C_OLT_S(f2, f0)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
    if ((fcr31 & 0x800000u) == 0u) {
        v1 = READ32_B(a2, 320);
    } else {
        f1 = lift_d1::flt(READ32_B(a2, 332));
        fcr31 = (FPU_C_OLT_S(f2, f1)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
        if ((fcr31 & 0x800000u) == 0u) {
            v1 = READ32_B(a2, 320);
        } else {
            f0 = FPU_MUL_S(f12, f0);
            f1 = FPU_MUL_S(f13, f1);
            f0 = FPU_ADD_SF(f0, f1);
            WRITE32_B(a0, 332, lift_d1::bits(f0));
            v1 = READ32_B(a2, 320);
        }
    }
    v0 = 1u;
    if (v1 != v0) {
        f1 = lift_d1::flt(READ32_B(a1, 336));
    } else {
        f0 = lift_d1::flt(READ32_B(a1, 336));
        WRITE32_B(a0, 336, lift_d1::bits(f0));
        goto L_14294bc;
    }
    f0 = lift_d1::flt(READ32_B(a2, 336));
    f1 = FPU_MUL_S(f12, f1);
    f0 = FPU_MUL_S(f13, f0);
    f1 = FPU_ADD_SF(f1, f0);
    WRITE32_B(a0, 336, lift_d1::bits(f1));
L_14294bc:
    f0 = lift_d1::flt(READ32_B(a1, 340));
    f2 = lift_d1::flt(0u);
    fcr31 = (FPU_C_OLT_S(f2, f0)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
    if ((fcr31 & 0x800000u) == 0u) {
        f0 = lift_d1::flt(READ32_B(a1, 344));
    } else {
        f1 = lift_d1::flt(READ32_B(a2, 340));
        fcr31 = (FPU_C_OLT_S(f2, f1)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
        if ((fcr31 & 0x800000u) == 0u) {
            f0 = lift_d1::flt(READ32_B(a1, 344));
        } else {
            f0 = FPU_MUL_S(f12, f0);
            f1 = FPU_MUL_S(f13, f1);
            f0 = FPU_ADD_SF(f0, f1);
            WRITE32_B(a0, 340, lift_d1::bits(f0));
            f0 = lift_d1::flt(READ32_B(a1, 344));
            f2 = lift_d1::flt(0u);
        }
    }
    fcr31 = (FPU_C_OLT_S(f2, f0)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
    if ((fcr31 & 0x800000u) == 0u) {
        f0 = lift_d1::flt(READ32_B(a1, 348));
    } else {
        f1 = lift_d1::flt(READ32_B(a2, 344));
        fcr31 = (FPU_C_OLT_S(f2, f1)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
        if ((fcr31 & 0x800000u) == 0u) {
            f0 = lift_d1::flt(READ32_B(a1, 348));
        } else {
            f0 = FPU_MUL_S(f12, f0);
            f1 = FPU_MUL_S(f13, f1);
            f0 = FPU_ADD_SF(f0, f1);
            WRITE32_B(a0, 344, lift_d1::bits(f0));
            f0 = lift_d1::flt(READ32_B(a1, 348));
        }
    }
    f2 = lift_d1::flt(0u);
    fcr31 = (FPU_C_OLT_S(f2, f0)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
    if ((fcr31 & 0x800000u) == 0u) {
        f0 = lift_d1::flt(READ32_B(a1, 352));
    } else {
        f1 = lift_d1::flt(READ32_B(a2, 348));
        fcr31 = (FPU_C_OLT_S(f2, f1)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
        if ((fcr31 & 0x800000u) == 0u) {
            f0 = lift_d1::flt(READ32_B(a1, 352));
        } else {
            f0 = FPU_MUL_S(f12, f0);
            f1 = FPU_MUL_S(f13, f1);
            f0 = FPU_ADD_SF(f0, f1);
            WRITE32_B(a0, 348, lift_d1::bits(f0));
            f0 = lift_d1::flt(READ32_B(a1, 352));
        }
    }
    f1 = lift_d1::flt(READ32_B(a2, 352));
    f0 = FPU_MUL_S(f12, f0);
    f1 = FPU_MUL_S(f13, f1);
    f3 = lift_d1::flt(0u);
    f0 = FPU_ADD_SF(f0, f1);
    WRITE32_B(a0, 352, lift_d1::bits(f0));
    f1 = lift_d1::flt(READ32_B(a1, 356));
    f0 = lift_d1::flt(READ32_B(a2, 356));
    f1 = FPU_MUL_S(f12, f1);
    f0 = FPU_MUL_S(f13, f0);
    f1 = FPU_ADD_SF(f1, f0);
    WRITE32_B(a0, 356, lift_d1::bits(f1));
    f2 = lift_d1::flt(READ32_B(a1, 368));
    f0 = lift_d1::flt(READ32_B(a2, 368));
    f2 = FPU_MUL_S(f12, f2);
    f0 = FPU_MUL_S(f13, f0);
    f2 = FPU_ADD_SF(f2, f0);
    WRITE32_B(a0, 368, lift_d1::bits(f2));
    f0 = lift_d1::flt(READ32_B(a1, 372));
    fcr31 = (FPU_C_OLT_S(f3, f0)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
    if ((fcr31 & 0x800000u) == 0u) {
        f0 = lift_d1::flt(READ32_B(a1, 376));
    } else {
        f1 = lift_d1::flt(READ32_B(a2, 372));
        fcr31 = (FPU_C_OLT_S(f3, f1)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
        if ((fcr31 & 0x800000u) == 0u) {
            f0 = lift_d1::flt(READ32_B(a1, 376));
        } else {
            f0 = FPU_MUL_S(f12, f0);
            f1 = FPU_MUL_S(f13, f1);
            f0 = FPU_ADD_SF(f0, f1);
            WRITE32_B(a0, 372, lift_d1::bits(f0));
            f0 = lift_d1::flt(READ32_B(a1, 376));
        }
    }
    f2 = lift_d1::flt(0u);
    fcr31 = (FPU_C_OLT_S(f2, f0)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
    if ((fcr31 & 0x800000u) == 0u) {
        f0 = lift_d1::flt(READ32_B(a1, 380));
    } else {
        f1 = lift_d1::flt(READ32_B(a2, 376));
        fcr31 = (FPU_C_OLT_S(f2, f1)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
        if ((fcr31 & 0x800000u) == 0u) {
            f0 = lift_d1::flt(READ32_B(a1, 380));
        } else {
            f0 = FPU_MUL_S(f12, f0);
            f1 = FPU_MUL_S(f13, f1);
            f0 = FPU_ADD_SF(f0, f1);
            WRITE32_B(a0, 376, lift_d1::bits(f0));
            f0 = lift_d1::flt(READ32_B(a1, 380));
            f2 = lift_d1::flt(0u);
        }
    }
    fcr31 = (FPU_C_OLT_S(f2, f0)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
    if ((fcr31 & 0x800000u) == 0u) {
        f0 = lift_d1::flt(READ32_B(a1, 384));
    } else {
        f1 = lift_d1::flt(READ32_B(a2, 380));
        fcr31 = (FPU_C_OLT_S(f2, f1)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
        if ((fcr31 & 0x800000u) == 0u) {
            f0 = lift_d1::flt(READ32_B(a1, 384));
        } else {
            f0 = FPU_MUL_S(f12, f0);
            f1 = FPU_MUL_S(f13, f1);
            f0 = FPU_ADD_SF(f0, f1);
            WRITE32_B(a0, 380, lift_d1::bits(f0));
            f0 = lift_d1::flt(READ32_B(a1, 384));
        }
    }
    f2 = lift_d1::flt(0u);
    fcr31 = (FPU_C_OLT_S(f2, f0)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
    if ((fcr31 & 0x800000u) == 0u) {
        f0 = lift_d1::flt(READ32_B(a1, 388));
    } else {
        f1 = lift_d1::flt(READ32_B(a2, 384));
        fcr31 = (FPU_C_OLT_S(f2, f1)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
        if ((fcr31 & 0x800000u) == 0u) {
            f0 = lift_d1::flt(READ32_B(a1, 388));
        } else {
            f0 = FPU_MUL_S(f12, f0);
            f1 = FPU_MUL_S(f13, f1);
            f0 = FPU_ADD_SF(f0, f1);
            WRITE32_B(a0, 384, lift_d1::bits(f0));
            f0 = lift_d1::flt(READ32_B(a1, 388));
            f2 = lift_d1::flt(0u);
        }
    }
    fcr31 = (FPU_C_OLT_S(f2, f0)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
    if ((fcr31 & 0x800000u) == 0u) {
        f0 = lift_d1::flt(READ32_B(a1, 392));
    } else {
        f1 = lift_d1::flt(READ32_B(a2, 388));
        fcr31 = (FPU_C_OLT_S(f2, f1)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
        if ((fcr31 & 0x800000u) == 0u) {
            f0 = lift_d1::flt(READ32_B(a1, 392));
        } else {
            f0 = FPU_MUL_S(f12, f0);
            f1 = FPU_MUL_S(f13, f1);
            f0 = FPU_ADD_SF(f0, f1);
            WRITE32_B(a0, 388, lift_d1::bits(f0));
            f0 = lift_d1::flt(READ32_B(a1, 392));
        }
    }
    f2 = lift_d1::flt(0u);
    fcr31 = (FPU_C_OLT_S(f2, f0)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
    if ((fcr31 & 0x800000u) == 0u) {
        f1 = lift_d1::flt(READ32_B(a1, 396));
    } else {
        f1 = lift_d1::flt(READ32_B(a2, 392));
        fcr31 = (FPU_C_OLT_S(f2, f1)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
        if ((fcr31 & 0x800000u) == 0u) {
            f1 = lift_d1::flt(READ32_B(a1, 396));
        } else {
            f0 = FPU_MUL_S(f12, f0);
            f1 = FPU_MUL_S(f13, f1);
            f0 = FPU_ADD_SF(f0, f1);
            WRITE32_B(a0, 392, lift_d1::bits(f0));
            f1 = lift_d1::flt(READ32_B(a1, 396));
        }
    }
    f0 = lift_d1::flt(READ32_B(a2, 396));
    f1 = FPU_MUL_S(f12, f1);
    f0 = FPU_MUL_S(f13, f0);
    f1 = FPU_ADD_SF(f1, f0);
    {
        const uint32_t target = ra;
        WRITE32_B(a0, 396, lift_d1::bits(f1));
        ctx->f[0] = f0; ctx->f[1] = f1; ctx->f[2] = f2; ctx->f[3] = f3; Ps2SetGpr64(ctx, 2, sx64(v0)); Ps2SetGpr64(ctx, 3, sx64(v1)); ctx->fcr31 = fcr31;
        ctx->pc = target;
        return;
    }
}

// gamecore_sub_014296F8 (0x14296f8): lift_d1's text (the reference)
void gamecore_sub_014296F8(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t at = Ps2GprU32(ctx, 1), v0 = Ps2GprU32(ctx, 2), v1 = Ps2GprU32(ctx, 3), a0 = Ps2GprU32(ctx, 4), a1 = Ps2GprU32(ctx, 5), a2 = Ps2GprU32(ctx, 6), ra = Ps2GprU32(ctx, 31);
    float f0 = ctx->f[0], f1 = ctx->f[1], f2 = ctx->f[2], f3 = ctx->f[3], f12 = ctx->f[12], f13 = ctx->f[13];
    uint32_t fcr31 = ctx->fcr31;
    bool df_at = false;
    f0 = lift_d1::flt(READ32_B(a1, 12));
    f1 = lift_d1::flt(READ32_B(a2, 12));
    f0 = FPU_MUL_S(f12, f0);
    f1 = FPU_MUL_S(f13, f1);
    f3 = lift_d1::flt(0u);
    f0 = FPU_ADD_SF(f0, f1);
    WRITE32_B(a0, 12, lift_d1::bits(f0));
    f2 = lift_d1::flt(READ32_B(a1, 20));
    fcr31 = (FPU_C_OLT_S(f3, f2)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
    if ((fcr31 & 0x800000u) == 0u) {
        f2 = lift_d1::flt(READ32_B(a1, 24));
    } else {
        f1 = lift_d1::flt(READ32_B(a2, 20));
        fcr31 = (FPU_C_OLT_S(f3, f1)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
        if ((fcr31 & 0x800000u) == 0u) {
            f2 = lift_d1::flt(READ32_B(a1, 24));
        } else {
            at = 0x3A830000u; df_at = true;
            at = at | 0x126Eu; df_at = true;
            f0 = lift_d1::flt(at);
            fcr31 = (FPU_C_EQ_S(f1, f0)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
            if ((fcr31 & 0x800000u) != 0u) {
                f2 = lift_d1::flt(READ32_B(a1, 24));
            } else {
                f0 = FPU_MUL_S(f12, f2);
                f1 = FPU_MUL_S(f13, f1);
                f0 = FPU_ADD_SF(f0, f1);
                WRITE32_B(a0, 20, lift_d1::bits(f0));
                f2 = lift_d1::flt(READ32_B(a1, 24));
            }
        }
    }
    f0 = lift_d1::flt(0u);
    fcr31 = (FPU_C_OLT_S(f0, f2)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
    if ((fcr31 & 0x800000u) == 0u) {
        f0 = lift_d1::flt(READ32_B(a1, 28));
    } else {
        f1 = lift_d1::flt(READ32_B(a2, 24));
        fcr31 = (FPU_C_OLT_S(f0, f1)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
        if ((fcr31 & 0x800000u) == 0u) {
            f0 = lift_d1::flt(READ32_B(a1, 28));
        } else {
            at = 0x3A830000u; df_at = true;
            at = at | 0x126Eu; df_at = true;
            f0 = lift_d1::flt(at);
            fcr31 = (FPU_C_EQ_S(f1, f0)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
            if ((fcr31 & 0x800000u) != 0u) {
                f0 = lift_d1::flt(READ32_B(a1, 28));
            } else {
                f0 = FPU_MUL_S(f12, f2);
                f1 = FPU_MUL_S(f13, f1);
                f0 = FPU_ADD_SF(f0, f1);
                WRITE32_B(a0, 24, lift_d1::bits(f0));
                f0 = lift_d1::flt(READ32_B(a1, 28));
            }
        }
    }
    f2 = lift_d1::flt(0u);
    fcr31 = (FPU_C_OLT_S(f2, f0)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
    if ((fcr31 & 0x800000u) == 0u) {
        f0 = lift_d1::flt(READ32_B(a1, 40));
    } else {
        f1 = lift_d1::flt(READ32_B(a2, 28));
        fcr31 = (FPU_C_OLT_S(f2, f1)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
        if ((fcr31 & 0x800000u) == 0u) {
            f0 = lift_d1::flt(READ32_B(a1, 40));
        } else {
            f0 = FPU_MUL_S(f12, f0);
            f1 = FPU_MUL_S(f13, f1);
            f0 = FPU_ADD_SF(f0, f1);
            WRITE32_B(a0, 28, lift_d1::bits(f0));
            f0 = lift_d1::flt(READ32_B(a1, 40));
            f2 = lift_d1::flt(0u);
        }
    }
    fcr31 = (FPU_C_OLT_S(f2, f0)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
    if ((fcr31 & 0x800000u) == 0u) {
        f0 = lift_d1::flt(READ32_B(a1, 44));
    } else {
        f1 = lift_d1::flt(READ32_B(a2, 40));
        fcr31 = (FPU_C_OLT_S(f2, f1)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
        if ((fcr31 & 0x800000u) == 0u) {
            f0 = lift_d1::flt(READ32_B(a1, 44));
        } else {
            f0 = FPU_MUL_S(f12, f0);
            f1 = FPU_MUL_S(f13, f1);
            f0 = FPU_ADD_SF(f0, f1);
            WRITE32_B(a0, 40, lift_d1::bits(f0));
            f0 = lift_d1::flt(READ32_B(a1, 44));
        }
    }
    f1 = lift_d1::flt(READ32_B(a2, 44));
    f0 = FPU_MUL_S(f12, f0);
    f1 = FPU_MUL_S(f13, f1);
    f3 = lift_d1::flt(0u);
    f0 = FPU_ADD_SF(f0, f1);
    WRITE32_B(a0, 44, lift_d1::bits(f0));
    f1 = lift_d1::flt(READ32_B(a1, 48));
    f0 = lift_d1::flt(READ32_B(a2, 48));
    f1 = FPU_MUL_S(f12, f1);
    f0 = FPU_MUL_S(f13, f0);
    f1 = FPU_ADD_SF(f1, f0);
    WRITE32_B(a0, 48, lift_d1::bits(f1));
    f2 = lift_d1::flt(READ32_B(a1, 52));
    f0 = lift_d1::flt(READ32_B(a2, 52));
    f2 = FPU_MUL_S(f12, f2);
    f0 = FPU_MUL_S(f13, f0);
    f2 = FPU_ADD_SF(f2, f0);
    WRITE32_B(a0, 52, lift_d1::bits(f2));
    f1 = lift_d1::flt(READ32_B(a1, 56));
    f0 = lift_d1::flt(READ32_B(a2, 56));
    f1 = FPU_MUL_S(f12, f1);
    f0 = FPU_MUL_S(f13, f0);
    f1 = FPU_ADD_SF(f1, f0);
    WRITE32_B(a0, 56, lift_d1::bits(f1));
    f2 = lift_d1::flt(READ32_B(a1, 60));
    f0 = lift_d1::flt(READ32_B(a2, 60));
    f2 = FPU_MUL_S(f12, f2);
    f0 = FPU_MUL_S(f13, f0);
    f2 = FPU_ADD_SF(f2, f0);
    WRITE32_B(a0, 60, lift_d1::bits(f2));
    f1 = lift_d1::flt(READ32_B(a1, 64));
    f0 = lift_d1::flt(READ32_B(a2, 64));
    f1 = FPU_MUL_S(f12, f1);
    f0 = FPU_MUL_S(f13, f0);
    f1 = FPU_ADD_SF(f1, f0);
    WRITE32_B(a0, 64, lift_d1::bits(f1));
    f2 = lift_d1::flt(READ32_B(a1, 68));
    f0 = lift_d1::flt(READ32_B(a2, 68));
    f2 = FPU_MUL_S(f12, f2);
    f0 = FPU_MUL_S(f13, f0);
    f2 = FPU_ADD_SF(f2, f0);
    WRITE32_B(a0, 68, lift_d1::bits(f2));
    f0 = lift_d1::flt(READ32_B(a1, 76));
    fcr31 = (FPU_C_OLT_S(f3, f0)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
    if ((fcr31 & 0x800000u) == 0u) {
        f2 = lift_d1::flt(READ32_B(a1, 80));
    } else {
        f1 = lift_d1::flt(READ32_B(a2, 76));
        fcr31 = (FPU_C_OLT_S(f3, f1)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
        if ((fcr31 & 0x800000u) == 0u) {
            f2 = lift_d1::flt(READ32_B(a1, 80));
        } else {
            f0 = FPU_MUL_S(f12, f0);
            f1 = FPU_MUL_S(f13, f1);
            f0 = FPU_ADD_SF(f0, f1);
            WRITE32_B(a0, 76, lift_d1::bits(f0));
            f2 = lift_d1::flt(READ32_B(a1, 80));
        }
    }
    v1 = 1u;
    f0 = lift_d1::flt(READ32_B(a2, 80));
    f2 = FPU_MUL_S(f12, f2);
    f0 = FPU_MUL_S(f13, f0);
    f2 = FPU_ADD_SF(f2, f0);
    WRITE32_B(a0, 80, lift_d1::bits(f2));
    f1 = lift_d1::flt(READ32_B(a1, 84));
    f0 = lift_d1::flt(READ32_B(a2, 84));
    f1 = FPU_MUL_S(f12, f1);
    f0 = FPU_MUL_S(f13, f0);
    f1 = FPU_ADD_SF(f1, f0);
    WRITE32_B(a0, 84, lift_d1::bits(f1));
    f2 = lift_d1::flt(READ32_B(a1, 88));
    f0 = lift_d1::flt(READ32_B(a2, 88));
    f2 = FPU_MUL_S(f12, f2);
    f0 = FPU_MUL_S(f13, f0);
    f2 = FPU_ADD_SF(f2, f0);
    WRITE32_B(a0, 88, lift_d1::bits(f2));
    f1 = lift_d1::flt(READ32_B(a1, 92));
    f0 = lift_d1::flt(READ32_B(a2, 92));
    f1 = FPU_MUL_S(f12, f1);
    f0 = FPU_MUL_S(f13, f0);
    f1 = FPU_ADD_SF(f1, f0);
    WRITE32_B(a0, 92, lift_d1::bits(f1));
    f2 = lift_d1::flt(READ32_B(a1, 96));
    f0 = lift_d1::flt(READ32_B(a2, 96));
    f2 = FPU_MUL_S(f12, f2);
    f0 = FPU_MUL_S(f13, f0);
    f2 = FPU_ADD_SF(f2, f0);
    WRITE32_B(a0, 96, lift_d1::bits(f2));
    f1 = lift_d1::flt(READ32_B(a1, 100));
    f0 = lift_d1::flt(READ32_B(a2, 100));
    f1 = FPU_MUL_S(f12, f1);
    f0 = FPU_MUL_S(f13, f0);
    f1 = FPU_ADD_SF(f1, f0);
    WRITE32_B(a0, 100, lift_d1::bits(f1));
    f2 = lift_d1::flt(READ32_B(a1, 104));
    f0 = lift_d1::flt(READ32_B(a2, 104));
    f2 = FPU_MUL_S(f12, f2);
    f0 = FPU_MUL_S(f13, f0);
    f2 = FPU_ADD_SF(f2, f0);
    WRITE32_B(a0, 104, lift_d1::bits(f2));
    f1 = lift_d1::flt(READ32_B(a1, 112));
    f0 = lift_d1::flt(READ32_B(a2, 112));
    f1 = FPU_MUL_S(f12, f1);
    f0 = FPU_MUL_S(f13, f0);
    f1 = FPU_ADD_SF(f1, f0);
    WRITE32_B(a0, 112, lift_d1::bits(f1));
    f2 = lift_d1::flt(READ32_B(a1, 116));
    f0 = lift_d1::flt(READ32_B(a2, 116));
    f2 = FPU_MUL_S(f12, f2);
    f0 = FPU_MUL_S(f13, f0);
    f2 = FPU_ADD_SF(f2, f0);
    WRITE32_B(a0, 116, lift_d1::bits(f2));
    f1 = lift_d1::flt(READ32_B(a1, 120));
    f0 = lift_d1::flt(READ32_B(a2, 120));
    f1 = FPU_MUL_S(f12, f1);
    f0 = FPU_MUL_S(f13, f0);
    f1 = FPU_ADD_SF(f1, f0);
    WRITE32_B(a0, 120, lift_d1::bits(f1));
    f2 = lift_d1::flt(READ32_B(a1, 124));
    f0 = lift_d1::flt(READ32_B(a2, 124));
    f2 = FPU_MUL_S(f12, f2);
    f0 = FPU_MUL_S(f13, f0);
    f2 = FPU_ADD_SF(f2, f0);
    WRITE32_B(a0, 124, lift_d1::bits(f2));
    f1 = lift_d1::flt(READ32_B(a1, 128));
    f0 = lift_d1::flt(READ32_B(a2, 128));
    f1 = FPU_MUL_S(f12, f1);
    f0 = FPU_MUL_S(f13, f0);
    f1 = FPU_ADD_SF(f1, f0);
    WRITE32_B(a0, 128, lift_d1::bits(f1));
    f2 = lift_d1::flt(READ32_B(a1, 132));
    f0 = lift_d1::flt(READ32_B(a2, 132));
    f2 = FPU_MUL_S(f12, f2);
    f0 = FPU_MUL_S(f13, f0);
    f2 = FPU_ADD_SF(f2, f0);
    WRITE32_B(a0, 132, lift_d1::bits(f2));
    f1 = lift_d1::flt(READ32_B(a1, 136));
    f0 = lift_d1::flt(READ32_B(a2, 136));
    f1 = FPU_MUL_S(f12, f1);
    f0 = FPU_MUL_S(f13, f0);
    f1 = FPU_ADD_SF(f1, f0);
    WRITE32_B(a0, 136, lift_d1::bits(f1));
    f2 = lift_d1::flt(READ32_B(a1, 140));
    f0 = lift_d1::flt(READ32_B(a2, 140));
    f2 = FPU_MUL_S(f12, f2);
    f0 = FPU_MUL_S(f13, f0);
    f2 = FPU_ADD_SF(f2, f0);
    WRITE32_B(a0, 140, lift_d1::bits(f2));
    f1 = lift_d1::flt(READ32_B(a1, 144));
    f0 = lift_d1::flt(READ32_B(a2, 144));
    f1 = FPU_MUL_S(f12, f1);
    f0 = FPU_MUL_S(f13, f0);
    f1 = FPU_ADD_SF(f1, f0);
    WRITE32_B(a0, 144, lift_d1::bits(f1));
    f2 = lift_d1::flt(READ32_B(a1, 148));
    f0 = lift_d1::flt(READ32_B(a2, 148));
    f2 = FPU_MUL_S(f12, f2);
    f0 = FPU_MUL_S(f13, f0);
    f2 = FPU_ADD_SF(f2, f0);
    WRITE32_B(a0, 148, lift_d1::bits(f2));
    f1 = lift_d1::flt(READ32_B(a1, 152));
    f0 = lift_d1::flt(READ32_B(a2, 152));
    f1 = FPU_MUL_S(f12, f1);
    f0 = FPU_MUL_S(f13, f0);
    f1 = FPU_ADD_SF(f1, f0);
    WRITE32_B(a0, 152, lift_d1::bits(f1));
    f2 = lift_d1::flt(READ32_B(a1, 156));
    f0 = lift_d1::flt(READ32_B(a2, 156));
    f2 = FPU_MUL_S(f12, f2);
    f0 = FPU_MUL_S(f13, f0);
    f2 = FPU_ADD_SF(f2, f0);
    WRITE32_B(a0, 156, lift_d1::bits(f2));
    f1 = lift_d1::flt(READ32_B(a1, 160));
    f0 = lift_d1::flt(READ32_B(a2, 160));
    f1 = FPU_MUL_S(f12, f1);
    f0 = FPU_MUL_S(f13, f0);
    f1 = FPU_ADD_SF(f1, f0);
    WRITE32_B(a0, 160, lift_d1::bits(f1));
    f2 = lift_d1::flt(READ32_B(a1, 164));
    f0 = lift_d1::flt(READ32_B(a2, 164));
    f2 = FPU_MUL_S(f12, f2);
    f0 = FPU_MUL_S(f13, f0);
    f2 = FPU_ADD_SF(f2, f0);
    WRITE32_B(a0, 164, lift_d1::bits(f2));
    v0 = READ32_B(a1, 168);
    if (v0 == v1) {
        f1 = lift_d1::flt(READ32_B(a1, 172));
        f0 = lift_d1::flt(READ32_B(a2, 172));
        f1 = FPU_MUL_S(f12, f1);
        f0 = FPU_MUL_S(f13, f0);
        f1 = FPU_ADD_SF(f1, f0);
        WRITE32_B(a0, 172, lift_d1::bits(f1));
        f2 = lift_d1::flt(READ32_B(a1, 176));
        f0 = lift_d1::flt(READ32_B(a2, 176));
        f2 = FPU_MUL_S(f12, f2);
        f0 = FPU_MUL_S(f13, f0);
        f2 = FPU_ADD_SF(f2, f0);
        WRITE32_B(a0, 176, lift_d1::bits(f2));
        f1 = lift_d1::flt(READ32_B(a1, 180));
        f0 = lift_d1::flt(READ32_B(a2, 180));
        f1 = FPU_MUL_S(f12, f1);
        f0 = FPU_MUL_S(f13, f0);
        f1 = FPU_ADD_SF(f1, f0);
        WRITE32_B(a0, 180, lift_d1::bits(f1));
        f2 = lift_d1::flt(READ32_B(a1, 184));
        f0 = lift_d1::flt(READ32_B(a2, 184));
        f2 = FPU_MUL_S(f12, f2);
        f0 = FPU_MUL_S(f13, f0);
        f2 = FPU_ADD_SF(f2, f0);
        WRITE32_B(a0, 184, lift_d1::bits(f2));
        f1 = lift_d1::flt(READ32_B(a1, 188));
        f0 = lift_d1::flt(READ32_B(a2, 188));
        f1 = FPU_MUL_S(f12, f1);
        f0 = FPU_MUL_S(f13, f0);
        f1 = FPU_ADD_SF(f1, f0);
        WRITE32_B(a0, 188, lift_d1::bits(f1));
        f2 = lift_d1::flt(READ32_B(a1, 192));
        f0 = lift_d1::flt(READ32_B(a2, 192));
        f2 = FPU_MUL_S(f12, f2);
        f0 = FPU_MUL_S(f13, f0);
        f2 = FPU_ADD_SF(f2, f0);
        WRITE32_B(a0, 192, lift_d1::bits(f2));
        f1 = lift_d1::flt(READ32_B(a1, 196));
        f0 = lift_d1::flt(READ32_B(a2, 196));
        f1 = FPU_MUL_S(f12, f1);
        f0 = FPU_MUL_S(f13, f0);
        f1 = FPU_ADD_SF(f1, f0);
        WRITE32_B(a0, 196, lift_d1::bits(f1));
        f2 = lift_d1::flt(READ32_B(a1, 200));
        f0 = lift_d1::flt(READ32_B(a2, 200));
        f2 = FPU_MUL_S(f12, f2);
        f0 = FPU_MUL_S(f13, f0);
        f2 = FPU_ADD_SF(f2, f0);
        WRITE32_B(a0, 200, lift_d1::bits(f2));
    }
    {
        const uint32_t target = ra;
        ctx->f[0] = f0; ctx->f[1] = f1; ctx->f[2] = f2; ctx->f[3] = f3; if (df_at) Ps2SetGpr64(ctx, 1, sx64(at)); Ps2SetGpr64(ctx, 2, sx64(v0)); Ps2SetGpr64(ctx, 3, sx64(v1)); ctx->fcr31 = fcr31;
        ctx->pc = target;
        return;
    }
}

// gamecore_sub_01429BC0 (0x1429bc0): lift_d1's text (the reference)
void gamecore_sub_01429BC0(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t at = Ps2GprU32(ctx, 1), a0 = Ps2GprU32(ctx, 4), a1 = Ps2GprU32(ctx, 5), a2 = Ps2GprU32(ctx, 6), ra = Ps2GprU32(ctx, 31);
    float f0 = ctx->f[0], f1 = ctx->f[1], f2 = ctx->f[2], f3 = ctx->f[3], f4 = ctx->f[4], f5 = ctx->f[5], f6 = ctx->f[6], f12 = ctx->f[12], f13 = ctx->f[13];
    uint32_t fcr31 = ctx->fcr31;
    f0 = lift_d1::flt(READ32_B(a1, 4));
    f1 = lift_d1::flt(READ32_B(a2, 4));
    f0 = FPU_MUL_S(f12, f0);
    f1 = FPU_MUL_S(f13, f1);
    f3 = lift_d1::flt(0u);
    f0 = FPU_ADD_SF(f0, f1);
    WRITE32_B(a0, 4, lift_d1::bits(f0));
    f1 = lift_d1::flt(READ32_B(a1, 8));
    f0 = lift_d1::flt(READ32_B(a2, 8));
    f1 = FPU_MUL_S(f12, f1);
    f0 = FPU_MUL_S(f13, f0);
    f1 = FPU_ADD_SF(f1, f0);
    WRITE32_B(a0, 8, lift_d1::bits(f1));
    f2 = lift_d1::flt(READ32_B(a1, 12));
    f0 = lift_d1::flt(READ32_B(a2, 12));
    f2 = FPU_MUL_S(f12, f2);
    f0 = FPU_MUL_S(f13, f0);
    f2 = FPU_ADD_SF(f2, f0);
    WRITE32_B(a0, 12, lift_d1::bits(f2));
    f1 = lift_d1::flt(READ32_B(a1, 16));
    f0 = lift_d1::flt(READ32_B(a2, 16));
    f1 = FPU_MUL_S(f12, f1);
    f0 = FPU_MUL_S(f13, f0);
    f1 = FPU_ADD_SF(f1, f0);
    WRITE32_B(a0, 16, lift_d1::bits(f1));
    f2 = lift_d1::flt(READ32_B(a1, 20));
    f0 = lift_d1::flt(READ32_B(a2, 20));
    f2 = FPU_MUL_S(f12, f2);
    f0 = FPU_MUL_S(f13, f0);
    f2 = FPU_ADD_SF(f2, f0);
    WRITE32_B(a0, 20, lift_d1::bits(f2));
    f1 = lift_d1::flt(READ32_B(a1, 24));
    f0 = lift_d1::flt(READ32_B(a2, 24));
    f1 = FPU_MUL_S(f12, f1);
    f0 = FPU_MUL_S(f13, f0);
    f1 = FPU_ADD_SF(f1, f0);
    WRITE32_B(a0, 24, lift_d1::bits(f1));
    f2 = lift_d1::flt(READ32_B(a1, 28));
    f0 = lift_d1::flt(READ32_B(a2, 28));
    f2 = FPU_MUL_S(f12, f2);
    f0 = FPU_MUL_S(f13, f0);
    f2 = FPU_ADD_SF(f2, f0);
    WRITE32_B(a0, 28, lift_d1::bits(f2));
    f1 = lift_d1::flt(READ32_B(a1, 32));
    f0 = lift_d1::flt(READ32_B(a2, 32));
    f1 = FPU_MUL_S(f12, f1);
    f0 = FPU_MUL_S(f13, f0);
    f1 = FPU_ADD_SF(f1, f0);
    WRITE32_B(a0, 32, lift_d1::bits(f1));
    f2 = lift_d1::flt(READ32_B(a1, 36));
    f0 = lift_d1::flt(READ32_B(a2, 36));
    f2 = FPU_MUL_S(f12, f2);
    f0 = FPU_MUL_S(f13, f0);
    f2 = FPU_ADD_SF(f2, f0);
    WRITE32_B(a0, 36, lift_d1::bits(f2));
    f1 = lift_d1::flt(READ32_B(a1, 40));
    f0 = lift_d1::flt(READ32_B(a2, 40));
    f1 = FPU_MUL_S(f12, f1);
    f0 = FPU_MUL_S(f13, f0);
    f1 = FPU_ADD_SF(f1, f0);
    WRITE32_B(a0, 40, lift_d1::bits(f1));
    f2 = lift_d1::flt(READ32_B(a1, 44));
    f0 = lift_d1::flt(READ32_B(a2, 44));
    f2 = FPU_MUL_S(f12, f2);
    f0 = FPU_MUL_S(f13, f0);
    f2 = FPU_ADD_SF(f2, f0);
    WRITE32_B(a0, 44, lift_d1::bits(f2));
    f1 = lift_d1::flt(READ32_B(a1, 48));
    f0 = lift_d1::flt(READ32_B(a2, 48));
    f1 = FPU_MUL_S(f12, f1);
    f0 = FPU_MUL_S(f13, f0);
    f1 = FPU_ADD_SF(f1, f0);
    WRITE32_B(a0, 48, lift_d1::bits(f1));
    f2 = lift_d1::flt(READ32_B(a1, 52));
    f0 = lift_d1::flt(READ32_B(a2, 52));
    f2 = FPU_MUL_S(f12, f2);
    f0 = FPU_MUL_S(f13, f0);
    f2 = FPU_ADD_SF(f2, f0);
    WRITE32_B(a0, 52, lift_d1::bits(f2));
    f1 = lift_d1::flt(READ32_B(a1, 56));
    f0 = lift_d1::flt(READ32_B(a2, 56));
    f1 = FPU_MUL_S(f12, f1);
    f0 = FPU_MUL_S(f13, f0);
    f1 = FPU_ADD_SF(f1, f0);
    WRITE32_B(a0, 56, lift_d1::bits(f1));
    f2 = lift_d1::flt(READ32_B(a1, 60));
    f0 = lift_d1::flt(READ32_B(a2, 60));
    f2 = FPU_MUL_S(f12, f2);
    f0 = FPU_MUL_S(f13, f0);
    f2 = FPU_ADD_SF(f2, f0);
    WRITE32_B(a0, 60, lift_d1::bits(f2));
    f5 = lift_d1::flt(READ32_B(a2, 64));
    f4 = lift_d1::flt(READ32_B(a1, 64));
    f1 = FPU_MOV_S(f5);
    f6 = FPU_MOV_S(f4);
    f2 = FPU_SUB_S(f1, f6);
    fcr31 = (FPU_C_OLT_S(f2, f3)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
    if ((fcr31 & 0x800000u) != 0u) {
        at = 0xC3340000u;
        f0 = lift_d1::flt(at);
        fcr31 = (FPU_C_OLE_S(f0, f2)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
        if ((fcr31 & 0x800000u) != 0u) {
            f1 = FPU_MUL_S(f12, f4);
            goto L_1429d80;
        }
        goto L_1429d90;
    }
    at = 0x43340000u;
    f0 = lift_d1::flt(at);
    fcr31 = (FPU_C_OLE_S(f2, f0)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
    if ((fcr31 & 0x800000u) == 0u) {
        goto L_1429d90;
    }
    f1 = FPU_MUL_S(f12, f4);
L_1429d80:
    f0 = FPU_MUL_S(f13, f5);
    f1 = FPU_ADD_S(f1, f0);
    goto L_1429df4;
L_1429d90:
    f0 = lift_d1::flt(0u);
    fcr31 = (FPU_C_OLT_S(f4, f0)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
    if ((fcr31 & 0x800000u) != 0u) {
        at = 0x43B40000u;
        f0 = lift_d1::flt(at);
        f6 = FPU_ADD_S(f4, f0);
        goto L_1429dc8;
    }
    fcr31 = (FPU_C_OLT_S(f5, f0)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
    if ((fcr31 & 0x800000u) == 0u) {
        f0 = FPU_MUL_S(f12, f6);
        goto L_1429dcc;
    }
    at = 0x43B40000u;
    f0 = lift_d1::flt(at);
    f1 = FPU_ADD_S(f5, f0);
L_1429dc8:
    f0 = FPU_MUL_S(f12, f6);
L_1429dcc:
    at = 0x43340000u;
    f2 = lift_d1::flt(at);
    f1 = FPU_MUL_S(f13, f1);
    f1 = FPU_ADD_S(f0, f1);
    fcr31 = (FPU_C_OLT_S(f2, f1)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
    if ((fcr31 & 0x800000u) == 0u) {
        WRITE32_B(a0, 64, lift_d1::bits(f1));
        goto L_1429df8;
    }
    at = 0x43B40000u;
    f0 = lift_d1::flt(at);
    f1 = FPU_SUB_SF(f1, f0);
L_1429df4:
    WRITE32_B(a0, 64, lift_d1::bits(f1));
L_1429df8:
    f3 = lift_d1::flt(0u);
    f1 = lift_d1::flt(READ32_B(a1, 68));
    f0 = lift_d1::flt(READ32_B(a2, 68));
    f1 = FPU_MUL_S(f12, f1);
    f0 = FPU_MUL_S(f13, f0);
    f1 = FPU_ADD_SF(f1, f0);
    WRITE32_B(a0, 68, lift_d1::bits(f1));
    f2 = lift_d1::flt(READ32_B(a1, 72));
    f0 = lift_d1::flt(READ32_B(a2, 72));
    f2 = FPU_MUL_S(f12, f2);
    f0 = FPU_MUL_S(f13, f0);
    f2 = FPU_ADD_SF(f2, f0);
    WRITE32_B(a0, 72, lift_d1::bits(f2));
    f1 = lift_d1::flt(READ32_B(a1, 76));
    f0 = lift_d1::flt(READ32_B(a2, 76));
    f1 = FPU_MUL_S(f12, f1);
    f0 = FPU_MUL_S(f13, f0);
    f1 = FPU_ADD_SF(f1, f0);
    WRITE32_B(a0, 76, lift_d1::bits(f1));
    f2 = lift_d1::flt(READ32_B(a1, 80));
    f0 = lift_d1::flt(READ32_B(a2, 80));
    f2 = FPU_MUL_S(f12, f2);
    f0 = FPU_MUL_S(f13, f0);
    f2 = FPU_ADD_SF(f2, f0);
    WRITE32_B(a0, 80, lift_d1::bits(f2));
    f1 = lift_d1::flt(READ32_B(a1, 84));
    f0 = lift_d1::flt(READ32_B(a2, 84));
    f1 = FPU_MUL_S(f12, f1);
    f0 = FPU_MUL_S(f13, f0);
    f1 = FPU_ADD_SF(f1, f0);
    WRITE32_B(a0, 84, lift_d1::bits(f1));
    f2 = lift_d1::flt(READ32_B(a1, 88));
    f0 = lift_d1::flt(READ32_B(a2, 88));
    f2 = FPU_MUL_S(f12, f2);
    f0 = FPU_MUL_S(f13, f0);
    f2 = FPU_ADD_SF(f2, f0);
    WRITE32_B(a0, 88, lift_d1::bits(f2));
    f1 = lift_d1::flt(READ32_B(a1, 92));
    f0 = lift_d1::flt(READ32_B(a2, 92));
    f1 = FPU_MUL_S(f12, f1);
    f0 = FPU_MUL_S(f13, f0);
    f1 = FPU_ADD_SF(f1, f0);
    WRITE32_B(a0, 92, lift_d1::bits(f1));
    f2 = lift_d1::flt(READ32_B(a1, 96));
    f0 = lift_d1::flt(READ32_B(a2, 96));
    f2 = FPU_MUL_S(f12, f2);
    f0 = FPU_MUL_S(f13, f0);
    f2 = FPU_ADD_SF(f2, f0);
    WRITE32_B(a0, 96, lift_d1::bits(f2));
    f1 = lift_d1::flt(READ32_B(a1, 100));
    f0 = lift_d1::flt(READ32_B(a2, 100));
    f1 = FPU_MUL_S(f12, f1);
    f0 = FPU_MUL_S(f13, f0);
    f1 = FPU_ADD_SF(f1, f0);
    WRITE32_B(a0, 100, lift_d1::bits(f1));
    f2 = lift_d1::flt(READ32_B(a1, 104));
    f0 = lift_d1::flt(READ32_B(a2, 104));
    f2 = FPU_MUL_S(f12, f2);
    f0 = FPU_MUL_S(f13, f0);
    f2 = FPU_ADD_SF(f2, f0);
    WRITE32_B(a0, 104, lift_d1::bits(f2));
    f1 = lift_d1::flt(READ32_B(a1, 108));
    f0 = lift_d1::flt(READ32_B(a2, 108));
    f1 = FPU_MUL_S(f12, f1);
    f0 = FPU_MUL_S(f13, f0);
    f1 = FPU_ADD_SF(f1, f0);
    WRITE32_B(a0, 108, lift_d1::bits(f1));
    f2 = lift_d1::flt(READ32_B(a1, 112));
    f0 = lift_d1::flt(READ32_B(a2, 112));
    f2 = FPU_MUL_S(f12, f2);
    f0 = FPU_MUL_S(f13, f0);
    f2 = FPU_ADD_SF(f2, f0);
    WRITE32_B(a0, 112, lift_d1::bits(f2));
    f1 = lift_d1::flt(READ32_B(a1, 116));
    f0 = lift_d1::flt(READ32_B(a2, 116));
    f1 = FPU_MUL_S(f12, f1);
    f0 = FPU_MUL_S(f13, f0);
    f1 = FPU_ADD_SF(f1, f0);
    WRITE32_B(a0, 116, lift_d1::bits(f1));
    f2 = lift_d1::flt(READ32_B(a1, 120));
    f0 = lift_d1::flt(READ32_B(a2, 120));
    f2 = FPU_MUL_S(f12, f2);
    f0 = FPU_MUL_S(f13, f0);
    f2 = FPU_ADD_SF(f2, f0);
    WRITE32_B(a0, 120, lift_d1::bits(f2));
    f2 = lift_d1::flt(READ32_B(a1, 124));
    fcr31 = (FPU_C_OLT_S(f3, f2)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
    if ((fcr31 & 0x800000u) == 0u) {
        f2 = lift_d1::flt(READ32_B(a1, 128));
    } else {
        f1 = lift_d1::flt(READ32_B(a2, 124));
        fcr31 = (FPU_C_OLT_S(f3, f1)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
        if ((fcr31 & 0x800000u) == 0u) {
            f2 = lift_d1::flt(READ32_B(a1, 128));
        } else {
            at = 0x3A830000u;
            at = at | 0x126Eu;
            f0 = lift_d1::flt(at);
            fcr31 = (FPU_C_EQ_S(f1, f0)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
            if ((fcr31 & 0x800000u) != 0u) {
                f2 = lift_d1::flt(READ32_B(a1, 128));
            } else {
                f0 = FPU_MUL_S(f12, f2);
                f1 = FPU_MUL_S(f13, f1);
                f0 = FPU_ADD_SF(f0, f1);
                WRITE32_B(a0, 124, lift_d1::bits(f0));
                f2 = lift_d1::flt(READ32_B(a1, 128));
            }
        }
    }
    f0 = lift_d1::flt(0u);
    fcr31 = (FPU_C_OLT_S(f0, f2)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
    if ((fcr31 & 0x800000u) != 0u) {
        f1 = lift_d1::flt(READ32_B(a2, 128));
        fcr31 = (FPU_C_OLT_S(f0, f1)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
        if ((fcr31 & 0x800000u) != 0u) {
            at = 0x3A830000u;
            at = at | 0x126Eu;
            f0 = lift_d1::flt(at);
            fcr31 = (FPU_C_EQ_S(f1, f0)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
            if ((fcr31 & 0x800000u) == 0u) {
                f0 = FPU_MUL_S(f12, f2);
                f1 = FPU_MUL_S(f13, f1);
                f0 = FPU_ADD_SF(f0, f1);
                WRITE32_B(a0, 128, lift_d1::bits(f0));
            }
        }
    }
    {
        const uint32_t target = ra;
        ctx->f[0] = f0; ctx->f[1] = f1; ctx->f[2] = f2; ctx->f[3] = f3; ctx->f[4] = f4; ctx->f[5] = f5; ctx->f[6] = f6; Ps2SetGpr64(ctx, 1, sx64(at)); ctx->fcr31 = fcr31;
        ctx->pc = target;
        return;
    }
}

// gamecore_sub_0142B968 (0x142b968): lift_d1's text (the reference)
void gamecore_sub_0142B968(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t at = Ps2GprU32(ctx, 1), a0 = Ps2GprU32(ctx, 4), a1 = Ps2GprU32(ctx, 5), a2 = Ps2GprU32(ctx, 6), ra = Ps2GprU32(ctx, 31);
    float f0 = ctx->f[0], f1 = ctx->f[1], f2 = ctx->f[2], f3 = ctx->f[3], f12 = ctx->f[12], f13 = ctx->f[13];
    uint32_t fcr31 = ctx->fcr31;
    bool df_at = false;
    f0 = lift_d1::flt(READ32_B(a1, 4));
    f1 = lift_d1::flt(READ32_B(a2, 4));
    f0 = FPU_MUL_S(f12, f0);
    f1 = FPU_MUL_S(f13, f1);
    f3 = lift_d1::flt(0u);
    f0 = FPU_ADD_SF(f0, f1);
    WRITE32_B(a0, 4, lift_d1::bits(f0));
    f1 = lift_d1::flt(READ32_B(a1, 8));
    f0 = lift_d1::flt(READ32_B(a2, 8));
    f1 = FPU_MUL_S(f12, f1);
    f0 = FPU_MUL_S(f13, f0);
    f1 = FPU_ADD_SF(f1, f0);
    WRITE32_B(a0, 8, lift_d1::bits(f1));
    f2 = lift_d1::flt(READ32_B(a1, 12));
    f0 = lift_d1::flt(READ32_B(a2, 12));
    f2 = FPU_MUL_S(f12, f2);
    f0 = FPU_MUL_S(f13, f0);
    f2 = FPU_ADD_SF(f2, f0);
    WRITE32_B(a0, 12, lift_d1::bits(f2));
    f1 = lift_d1::flt(READ32_B(a1, 16));
    f0 = lift_d1::flt(READ32_B(a2, 16));
    f1 = FPU_MUL_S(f12, f1);
    f0 = FPU_MUL_S(f13, f0);
    f1 = FPU_ADD_SF(f1, f0);
    WRITE32_B(a0, 16, lift_d1::bits(f1));
    f2 = lift_d1::flt(READ32_B(a1, 20));
    f0 = lift_d1::flt(READ32_B(a2, 20));
    f2 = FPU_MUL_S(f12, f2);
    f0 = FPU_MUL_S(f13, f0);
    f2 = FPU_ADD_SF(f2, f0);
    WRITE32_B(a0, 20, lift_d1::bits(f2));
    f1 = lift_d1::flt(READ32_B(a1, 24));
    f0 = lift_d1::flt(READ32_B(a2, 24));
    f1 = FPU_MUL_S(f12, f1);
    f0 = FPU_MUL_S(f13, f0);
    f1 = FPU_ADD_SF(f1, f0);
    WRITE32_B(a0, 24, lift_d1::bits(f1));
    f2 = lift_d1::flt(READ32_B(a1, 28));
    f0 = lift_d1::flt(READ32_B(a2, 28));
    f2 = FPU_MUL_S(f12, f2);
    f0 = FPU_MUL_S(f13, f0);
    f2 = FPU_ADD_SF(f2, f0);
    WRITE32_B(a0, 28, lift_d1::bits(f2));
    f1 = lift_d1::flt(READ32_B(a1, 32));
    f0 = lift_d1::flt(READ32_B(a2, 32));
    f1 = FPU_MUL_S(f12, f1);
    f0 = FPU_MUL_S(f13, f0);
    f1 = FPU_ADD_SF(f1, f0);
    WRITE32_B(a0, 32, lift_d1::bits(f1));
    f2 = lift_d1::flt(READ32_B(a1, 36));
    f0 = lift_d1::flt(READ32_B(a2, 36));
    f2 = FPU_MUL_S(f12, f2);
    f0 = FPU_MUL_S(f13, f0);
    f2 = FPU_ADD_SF(f2, f0);
    WRITE32_B(a0, 36, lift_d1::bits(f2));
    f1 = lift_d1::flt(READ32_B(a1, 40));
    f0 = lift_d1::flt(READ32_B(a2, 40));
    f1 = FPU_MUL_S(f12, f1);
    f0 = FPU_MUL_S(f13, f0);
    f1 = FPU_ADD_SF(f1, f0);
    WRITE32_B(a0, 40, lift_d1::bits(f1));
    f2 = lift_d1::flt(READ32_B(a1, 44));
    f0 = lift_d1::flt(READ32_B(a2, 44));
    f2 = FPU_MUL_S(f12, f2);
    f0 = FPU_MUL_S(f13, f0);
    f2 = FPU_ADD_SF(f2, f0);
    WRITE32_B(a0, 44, lift_d1::bits(f2));
    f1 = lift_d1::flt(READ32_B(a1, 48));
    f0 = lift_d1::flt(READ32_B(a2, 48));
    f1 = FPU_MUL_S(f12, f1);
    f0 = FPU_MUL_S(f13, f0);
    f1 = FPU_ADD_SF(f1, f0);
    WRITE32_B(a0, 48, lift_d1::bits(f1));
    f2 = lift_d1::flt(READ32_B(a1, 52));
    f0 = lift_d1::flt(READ32_B(a2, 52));
    f2 = FPU_MUL_S(f12, f2);
    f0 = FPU_MUL_S(f13, f0);
    f2 = FPU_ADD_SF(f2, f0);
    WRITE32_B(a0, 52, lift_d1::bits(f2));
    f1 = lift_d1::flt(READ32_B(a1, 56));
    f0 = lift_d1::flt(READ32_B(a2, 56));
    f1 = FPU_MUL_S(f12, f1);
    f0 = FPU_MUL_S(f13, f0);
    f1 = FPU_ADD_SF(f1, f0);
    WRITE32_B(a0, 56, lift_d1::bits(f1));
    f2 = lift_d1::flt(READ32_B(a1, 60));
    f0 = lift_d1::flt(READ32_B(a2, 60));
    f2 = FPU_MUL_S(f12, f2);
    f0 = FPU_MUL_S(f13, f0);
    f2 = FPU_ADD_SF(f2, f0);
    WRITE32_B(a0, 60, lift_d1::bits(f2));
    f1 = lift_d1::flt(READ32_B(a1, 64));
    f0 = lift_d1::flt(READ32_B(a2, 64));
    f1 = FPU_MUL_S(f12, f1);
    f0 = FPU_MUL_S(f13, f0);
    f1 = FPU_ADD_SF(f1, f0);
    WRITE32_B(a0, 64, lift_d1::bits(f1));
    f2 = lift_d1::flt(READ32_B(a1, 68));
    f0 = lift_d1::flt(READ32_B(a2, 68));
    f2 = FPU_MUL_S(f12, f2);
    f0 = FPU_MUL_S(f13, f0);
    f2 = FPU_ADD_SF(f2, f0);
    WRITE32_B(a0, 68, lift_d1::bits(f2));
    f1 = lift_d1::flt(READ32_B(a1, 72));
    f0 = lift_d1::flt(READ32_B(a2, 72));
    f1 = FPU_MUL_S(f12, f1);
    f0 = FPU_MUL_S(f13, f0);
    f1 = FPU_ADD_SF(f1, f0);
    WRITE32_B(a0, 72, lift_d1::bits(f1));
    f2 = lift_d1::flt(READ32_B(a1, 76));
    f0 = lift_d1::flt(READ32_B(a2, 76));
    f2 = FPU_MUL_S(f12, f2);
    f0 = FPU_MUL_S(f13, f0);
    f2 = FPU_ADD_SF(f2, f0);
    WRITE32_B(a0, 76, lift_d1::bits(f2));
    f1 = lift_d1::flt(READ32_B(a1, 80));
    f0 = lift_d1::flt(READ32_B(a2, 80));
    f1 = FPU_MUL_S(f12, f1);
    f0 = FPU_MUL_S(f13, f0);
    f1 = FPU_ADD_SF(f1, f0);
    WRITE32_B(a0, 80, lift_d1::bits(f1));
    f2 = lift_d1::flt(READ32_B(a1, 84));
    f0 = lift_d1::flt(READ32_B(a2, 84));
    f2 = FPU_MUL_S(f12, f2);
    f0 = FPU_MUL_S(f13, f0);
    f2 = FPU_ADD_SF(f2, f0);
    WRITE32_B(a0, 84, lift_d1::bits(f2));
    f1 = lift_d1::flt(READ32_B(a1, 88));
    f0 = lift_d1::flt(READ32_B(a2, 88));
    f1 = FPU_MUL_S(f12, f1);
    f0 = FPU_MUL_S(f13, f0);
    f1 = FPU_ADD_SF(f1, f0);
    WRITE32_B(a0, 88, lift_d1::bits(f1));
    f2 = lift_d1::flt(READ32_B(a1, 92));
    f0 = lift_d1::flt(READ32_B(a2, 92));
    f2 = FPU_MUL_S(f12, f2);
    f0 = FPU_MUL_S(f13, f0);
    f2 = FPU_ADD_SF(f2, f0);
    WRITE32_B(a0, 92, lift_d1::bits(f2));
    f1 = lift_d1::flt(READ32_B(a1, 96));
    f0 = lift_d1::flt(READ32_B(a2, 96));
    f1 = FPU_MUL_S(f12, f1);
    f0 = FPU_MUL_S(f13, f0);
    f1 = FPU_ADD_SF(f1, f0);
    WRITE32_B(a0, 96, lift_d1::bits(f1));
    f2 = lift_d1::flt(READ32_B(a1, 100));
    f0 = lift_d1::flt(READ32_B(a2, 100));
    f2 = FPU_MUL_S(f12, f2);
    f0 = FPU_MUL_S(f13, f0);
    f2 = FPU_ADD_SF(f2, f0);
    WRITE32_B(a0, 100, lift_d1::bits(f2));
    f1 = lift_d1::flt(READ32_B(a1, 104));
    f0 = lift_d1::flt(READ32_B(a2, 104));
    f1 = FPU_MUL_S(f12, f1);
    f0 = FPU_MUL_S(f13, f0);
    f1 = FPU_ADD_SF(f1, f0);
    WRITE32_B(a0, 104, lift_d1::bits(f1));
    f2 = lift_d1::flt(READ32_B(a1, 108));
    f0 = lift_d1::flt(READ32_B(a2, 108));
    f2 = FPU_MUL_S(f12, f2);
    f0 = FPU_MUL_S(f13, f0);
    f2 = FPU_ADD_SF(f2, f0);
    WRITE32_B(a0, 108, lift_d1::bits(f2));
    f1 = lift_d1::flt(READ32_B(a1, 112));
    f0 = lift_d1::flt(READ32_B(a2, 112));
    f1 = FPU_MUL_S(f12, f1);
    f0 = FPU_MUL_S(f13, f0);
    f1 = FPU_ADD_SF(f1, f0);
    WRITE32_B(a0, 112, lift_d1::bits(f1));
    f2 = lift_d1::flt(READ32_B(a1, 116));
    f0 = lift_d1::flt(READ32_B(a2, 116));
    f2 = FPU_MUL_S(f12, f2);
    f0 = FPU_MUL_S(f13, f0);
    f2 = FPU_ADD_SF(f2, f0);
    WRITE32_B(a0, 116, lift_d1::bits(f2));
    f1 = lift_d1::flt(READ32_B(a1, 120));
    f0 = lift_d1::flt(READ32_B(a2, 120));
    f1 = FPU_MUL_S(f12, f1);
    f0 = FPU_MUL_S(f13, f0);
    f1 = FPU_ADD_SF(f1, f0);
    WRITE32_B(a0, 120, lift_d1::bits(f1));
    f2 = lift_d1::flt(READ32_B(a1, 124));
    f0 = lift_d1::flt(READ32_B(a2, 124));
    f2 = FPU_MUL_S(f12, f2);
    f0 = FPU_MUL_S(f13, f0);
    f2 = FPU_ADD_SF(f2, f0);
    WRITE32_B(a0, 124, lift_d1::bits(f2));
    f1 = lift_d1::flt(READ32_B(a1, 128));
    f0 = lift_d1::flt(READ32_B(a2, 128));
    f1 = FPU_MUL_S(f12, f1);
    f0 = FPU_MUL_S(f13, f0);
    f1 = FPU_ADD_SF(f1, f0);
    WRITE32_B(a0, 128, lift_d1::bits(f1));
    f2 = lift_d1::flt(READ32_B(a1, 132));
    f0 = lift_d1::flt(READ32_B(a2, 132));
    f2 = FPU_MUL_S(f12, f2);
    f0 = FPU_MUL_S(f13, f0);
    f2 = FPU_ADD_SF(f2, f0);
    WRITE32_B(a0, 132, lift_d1::bits(f2));
    f2 = lift_d1::flt(READ32_B(a1, 136));
    fcr31 = (FPU_C_OLT_S(f3, f2)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
    if ((fcr31 & 0x800000u) != 0u) {
        f1 = lift_d1::flt(READ32_B(a2, 136));
        fcr31 = (FPU_C_OLT_S(f3, f1)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
        if ((fcr31 & 0x800000u) != 0u) {
            at = 0x3A830000u; df_at = true;
            at = at | 0x126Eu; df_at = true;
            f0 = lift_d1::flt(at);
            fcr31 = (FPU_C_EQ_S(f1, f0)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
            if ((fcr31 & 0x800000u) == 0u) {
                f0 = FPU_MUL_S(f12, f2);
                f1 = FPU_MUL_S(f13, f1);
                f0 = FPU_ADD_SF(f0, f1);
                WRITE32_B(a0, 136, lift_d1::bits(f0));
            }
        }
    }
    {
        const uint32_t target = ra;
        ctx->f[0] = f0; ctx->f[1] = f1; ctx->f[2] = f2; ctx->f[3] = f3; if (df_at) Ps2SetGpr64(ctx, 1, sx64(at)); ctx->fcr31 = fcr31;
        ctx->pc = target;
        return;
    }
}

// is_base_mode_manager_update_game_camera_param_manager (0x142c118): lift_d1's text (the reference)
void is_base_mode_manager_update_game_camera_param_manager(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t v0 = Ps2GprU32(ctx, 2), a0 = Ps2GprU32(ctx, 4), ra = Ps2GprU32(ctx, 31);
    {
        const uint32_t target = ra;
        v0 = READ32(a0);
        Ps2SetGpr64(ctx, 2, sx64(v0));
        ctx->pc = target;
        return;
    }
}

// get_interp_time_game_camera_param_manager (0x142c128): lift_d1's text (the reference)
void get_interp_time_game_camera_param_manager(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t a0 = Ps2GprU32(ctx, 4), ra = Ps2GprU32(ctx, 31);
    float f0 = ctx->f[0], f1 = ctx->f[1];
    uint32_t fcr31 = ctx->fcr31;
    f0 = lift_d1::flt(READ32_B(a0, 32));
    f1 = lift_d1::flt(0u);
    fcr31 = (FPU_C_OLT_S(f0, f1)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
    if ((fcr31 & 0x800000u) != 0u) {
        f0 = lift_d1::flt(READ32_B(a0, 1648));
    } else {
    }
    {
        const uint32_t target = ra;
        ctx->f[0] = f0; ctx->f[1] = f1; ctx->fcr31 = fcr31;
        ctx->pc = target;
        return;
    }
}

// act_camera_cutout_game_camera_param_manager (0x142c248): lift_d1's text (the reference)
void act_camera_cutout_game_camera_param_manager(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t v0 = Ps2GprU32(ctx, 2), v1 = Ps2GprU32(ctx, 3), a0 = Ps2GprU32(ctx, 4), a1 = Ps2GprU32(ctx, 5), ra = Ps2GprU32(ctx, 31);
    bool df_v1 = false;
    a0 = READ32_B(a0, 8);
    v0 = 1u;
    if (a0 != v0) {
        v0 = 0u;
    } else {
        v1 = READ32_B(a1, 4960); df_v1 = true;
        if (v1 != a0) {
            v0 = 0u;
        } else {
        }
    }
    {
        const uint32_t target = ra;
        Ps2SetGpr64(ctx, 2, sx64(v0)); if (df_v1) Ps2SetGpr64(ctx, 3, sx64(v1)); Ps2SetGpr64(ctx, 4, sx64(a0));
        ctx->pc = target;
        return;
    }
}

// drop_camera_cutout_flag_game_camera_param_manager (0x142c270): lift_d1's text (the reference)
void drop_camera_cutout_flag_game_camera_param_manager(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t a0 = Ps2GprU32(ctx, 4), ra = Ps2GprU32(ctx, 31);
    {
        const uint32_t target = ra;
        WRITE32_B(a0, 8, 0u);
        
        ctx->pc = target;
        return;
    }
}

// constructor_game_camera_param_update_data_manager (0x142c278): lift_d1's text (the reference)
void constructor_game_camera_param_update_data_manager(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t a0 = Ps2GprU32(ctx, 4), ra = Ps2GprU32(ctx, 31);
    WRITE32_B(a0, 4, 0u);
    {
        const uint32_t target = ra;
        WRITE32(a0, 0u);
        
        ctx->pc = target;
        return;
    }
}

// is_already_set_game_camera_param_update_data_manager (0x142c398): lift_d1's text (the reference)
void is_already_set_game_camera_param_update_data_manager(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t v0 = Ps2GprU32(ctx, 2), v1 = Ps2GprU32(ctx, 3), a0 = Ps2GprU32(ctx, 4), a1 = Ps2GprU32(ctx, 5), ra = Ps2GprU32(ctx, 31);
    uint64_t hi = ctx->hi;
    uint64_t lo = ctx->lo;
    v0 = 0x65Cu;
    v1 = READ32_B(a0, 4);
    { int64_t result = (int64_t)(int32_t)a1 * (int64_t)(int32_t)v0; lo = (uint64_t)(int64_t)(int32_t)result; hi = (uint64_t)(int64_t)(int32_t)(result >> 32); (a1 = (uint32_t)((int32_t)((int32_t)result))); }
    a1 = a1 + v1;
    v0 = READ32_B(a1, 4);
    {
        const uint32_t target = ra;
        v0 = 0u < v0 ? 1u : 0u;
        Ps2SetGpr64(ctx, 2, sx64(v0)); Ps2SetGpr64(ctx, 3, sx64(v1)); Ps2SetGpr64(ctx, 5, sx64(a1)); ctx->hi = hi; ctx->lo = lo;
        ctx->pc = target;
        return;
    }
}

// constructor_game_camera_mode_local_parameter (0x142c3b8): lift_d1's text (the reference)
void constructor_game_camera_mode_local_parameter(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t at = Ps2GprU32(ctx, 1), a0 = Ps2GprU32(ctx, 4), ra = Ps2GprU32(ctx, 31);
    float f0 = ctx->f[0], f1 = ctx->f[1], f2 = ctx->f[2], f3 = ctx->f[3], f4 = ctx->f[4];
    at = 0x40000000u;
    f2 = lift_d1::flt(at);
    at = 0x3F000000u;
    f3 = lift_d1::flt(at);
    at = 0x40900000u;
    f1 = lift_d1::flt(at);
    at = 0x42340000u;
    f4 = lift_d1::flt(at);
    at = 0x3F800000u;
    f0 = lift_d1::flt(at);
    WRITE32_B(a0, 28, lift_d1::bits(f1));
    WRITE32_B(a0, 60, lift_d1::bits(f0));
    WRITE32_B(a0, 40, lift_d1::bits(f2));
    WRITE32_B(a0, 52, lift_d1::bits(f3));
    WRITE32_B(a0, 56, lift_d1::bits(f4));
    WRITE32(a0, 0u);
    WRITE32_B(a0, 4, 0u);
    WRITE32_B(a0, 8, 0u);
    WRITE32_B(a0, 12, 0u);
    WRITE32_B(a0, 16, 0u);
    WRITE32_B(a0, 20, 0u);
    WRITE32_B(a0, 24, 0u);
    WRITE32_B(a0, 32, lift_d1::bits(f2));
    WRITE32_B(a0, 36, lift_d1::bits(f2));
    WRITE32_B(a0, 44, lift_d1::bits(f3));
    {
        const uint32_t target = ra;
        WRITE32_B(a0, 48, lift_d1::bits(f3));
        ctx->f[0] = f0; ctx->f[1] = f1; ctx->f[2] = f2; ctx->f[3] = f3; ctx->f[4] = f4; Ps2SetGpr64(ctx, 1, sx64(at));
        ctx->pc = target;
        return;
    }
}

// gamecore_sub_0142C440 (0x142c440): lift_d1's text (the reference)
void gamecore_sub_0142C440(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t at = Ps2GprU32(ctx, 1), a0 = Ps2GprU32(ctx, 4), a1 = Ps2GprU32(ctx, 5), a2 = Ps2GprU32(ctx, 6), ra = Ps2GprU32(ctx, 31);
    float f0 = ctx->f[0], f1 = ctx->f[1], f2 = ctx->f[2], f3 = ctx->f[3], f4 = ctx->f[4], f5 = ctx->f[5], f12 = ctx->f[12], f13 = ctx->f[13];
    uint32_t fcr31 = ctx->fcr31;
    f5 = lift_d1::flt(READ32_B(a2, 200));
    f3 = lift_d1::flt(READ32_B(a1, 200));
    f1 = FPU_MOV_S(f5);
    f0 = lift_d1::flt(0u);
    f4 = FPU_MOV_S(f3);
    f2 = FPU_SUB_S(f1, f4);
    fcr31 = (FPU_C_OLT_S(f2, f0)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
    if ((fcr31 & 0x800000u) != 0u) {
        at = 0xC3340000u;
        f0 = lift_d1::flt(at);
        fcr31 = (FPU_C_OLE_S(f0, f2)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
        if ((fcr31 & 0x800000u) != 0u) {
            f1 = FPU_MUL_S(f12, f3);
            goto L_142c498;
        }
        goto L_142c4a8;
    }
    at = 0x43340000u;
    f0 = lift_d1::flt(at);
    fcr31 = (FPU_C_OLE_S(f2, f0)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
    if ((fcr31 & 0x800000u) == 0u) {
        goto L_142c4a8;
    }
    f1 = FPU_MUL_S(f12, f3);
L_142c498:
    f0 = FPU_MUL_S(f13, f5);
    f1 = FPU_ADD_S(f1, f0);
    goto L_142c50c;
L_142c4a8:
    f0 = lift_d1::flt(0u);
    fcr31 = (FPU_C_OLT_S(f3, f0)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
    if ((fcr31 & 0x800000u) != 0u) {
        at = 0x43B40000u;
        f0 = lift_d1::flt(at);
        f4 = FPU_ADD_S(f3, f0);
        goto L_142c4e0;
    }
    fcr31 = (FPU_C_OLT_S(f5, f0)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
    if ((fcr31 & 0x800000u) == 0u) {
        f0 = FPU_MUL_S(f12, f4);
        goto L_142c4e4;
    }
    at = 0x43B40000u;
    f0 = lift_d1::flt(at);
    f1 = FPU_ADD_S(f5, f0);
L_142c4e0:
    f0 = FPU_MUL_S(f12, f4);
L_142c4e4:
    at = 0x43340000u;
    f2 = lift_d1::flt(at);
    f1 = FPU_MUL_S(f13, f1);
    f1 = FPU_ADD_S(f0, f1);
    fcr31 = (FPU_C_OLT_S(f2, f1)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
    if ((fcr31 & 0x800000u) == 0u) {
        WRITE32_B(a0, 200, lift_d1::bits(f1));
        goto L_142c510;
    }
    at = 0x43B40000u;
    f0 = lift_d1::flt(at);
    f1 = FPU_SUB_SF(f1, f0);
L_142c50c:
    WRITE32_B(a0, 200, lift_d1::bits(f1));
L_142c510:
    f1 = lift_d1::flt(READ32_B(a1, 204));
    f0 = lift_d1::flt(READ32_B(a2, 204));
    f1 = FPU_MUL_S(f12, f1);
    f0 = FPU_MUL_S(f13, f0);
    f1 = FPU_ADD_SF(f1, f0);
    {
        const uint32_t target = ra;
        WRITE32_B(a0, 204, lift_d1::bits(f1));
        ctx->f[0] = f0; ctx->f[1] = f1; ctx->f[2] = f2; ctx->f[3] = f3; ctx->f[4] = f4; ctx->f[5] = f5; Ps2SetGpr64(ctx, 1, sx64(at)); ctx->fcr31 = fcr31;
        ctx->pc = target;
        return;
    }
}

// gamecore_sub_0142C530 (0x142c530): lift_d1's text (the reference)
void gamecore_sub_0142C530(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t v1 = Ps2GprU32(ctx, 3), a0 = Ps2GprU32(ctx, 4), a2 = Ps2GprU32(ctx, 6), ra = Ps2GprU32(ctx, 31);
    uint64_t v0 = Ps2GprU64(ctx, 2), a1 = Ps2GprU64(ctx, 5);
    bool df_v1 = false, df_a2 = false;
    {
        const bool c = (int64_t)a1 == 0;
        a0 = a0 + 0x4Cu;
        if (!c) {
            (a1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)a1, 76))));
            a2 = READ32_B(a0, 140); df_a2 = true;
            (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)READ32_B((uint32_t)a1, 140))));
            {
                const bool c = (int64_t)(int32_t)a2 != (int64_t)v0;
                (v0 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL));
                if (c) {
                    goto L_142c5c4;
                }
            }
            v1 = READ32_B(a0, 168); df_v1 = true;
            (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)READ32_B((uint32_t)a1, 168))));
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
            a0 = READ32_B(a0, 144);
            (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)READ32_B((uint32_t)a1, 144))));
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
                const uint32_t target = ra;
                (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 1))));
                Ps2SetGpr64(ctx, 2, v0); if (df_v1) Ps2SetGpr64(ctx, 3, sx64(v1)); Ps2SetGpr64(ctx, 4, sx64(a0)); Ps2SetGpr64(ctx, 5, a1); if (df_a2) Ps2SetGpr64(ctx, 6, sx64(a2));
                ctx->pc = target;
                return;
            }
        }
    }
    (v0 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL));
L_142c5c4:
    {
        const uint32_t target = ra;
        Ps2SetGpr64(ctx, 2, v0); if (df_v1) Ps2SetGpr64(ctx, 3, sx64(v1)); Ps2SetGpr64(ctx, 4, sx64(a0)); Ps2SetGpr64(ctx, 5, a1); if (df_a2) Ps2SetGpr64(ctx, 6, sx64(a2));
        ctx->pc = target;
        return;
    }
}

// setBossPathColiGroup (0x1431c38): lift_d1's text (the reference)
void setBossPathColiGroup(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t v0 = Ps2GprU32(ctx, 2), a0 = Ps2GprU32(ctx, 4), ra = Ps2GprU32(ctx, 31);
    v0 = 0x1200000u;
    {
        const uint32_t target = ra;
        WRITE32(v0 - 0x4340u, a0);
        Ps2SetGpr64(ctx, 2, sx64(v0));
        ctx->pc = target;
        return;
    }
}

// setHorsePathColiGroup (0x1431c48): lift_d1's text (the reference)
void setHorsePathColiGroup(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t v0 = Ps2GprU32(ctx, 2), a0 = Ps2GprU32(ctx, 4), ra = Ps2GprU32(ctx, 31);
    v0 = 0x1200000u;
    {
        const uint32_t target = ra;
        WRITE32(v0 - 0x433Cu, a0);
        Ps2SetGpr64(ctx, 2, sx64(v0));
        ctx->pc = target;
        return;
    }
}

// getPathColiGroupByType (0x1431c70): lift_d1's text (the reference)
void getPathColiGroupByType(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t v0 = Ps2GprU32(ctx, 2), v1 = Ps2GprU32(ctx, 3), ra = Ps2GprU32(ctx, 31);
    uint64_t a0 = Ps2GprU64(ctx, 4);
    if ((int64_t)a0 != 0) {
        v0 = 1u;
    } else {
        v1 = 0x1200000u;
        {
            const uint32_t target = ra;
            v0 = READ32(v1 - 0x4340u);
            Ps2SetGpr64(ctx, 2, sx64(v0)); Ps2SetGpr64(ctx, 3, sx64(v1));
            ctx->pc = target;
            return;
        }
    }
    {
        const bool c = (int64_t)a0 != (int64_t)(int32_t)v0;
        v1 = 0x1200000u;
        if (!c) {
            {
                const uint32_t target = ra;
                v0 = READ32(v1 - 0x433Cu);
                Ps2SetGpr64(ctx, 2, sx64(v0)); Ps2SetGpr64(ctx, 3, sx64(v1));
                ctx->pc = target;
                return;
            }
        }
    }
    {
        const uint32_t target = ra;
        v0 = 0u;
        Ps2SetGpr64(ctx, 2, sx64(v0)); Ps2SetGpr64(ctx, 3, sx64(v1));
        ctx->pc = target;
        return;
    }
}

// getPassageWork (0x1431ce0): lift_d1's text (the reference)
void getPassageWork(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t a0 = Ps2GprU32(ctx, 4), ra = Ps2GprU32(ctx, 31);
    uint64_t v0 = Ps2GprU64(ctx, 2), a1 = Ps2GprU64(ctx, 5);
    (v0 = (uint64_t)(((int64_t)(int64_t)a1 < (int64_t)(int32_t)20) ? 1 : 0));
    if ((int64_t)v0 != 0) {
        (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)READ32_B(a0, 896))));
        (v0 = (uint64_t)(((uint64_t)a1 < (uint64_t)RC_U64X(v0)) ? 1 : 0));
        if ((int64_t)v0 != 0) {
            (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)SLL32((uint32_t)a1, 5))));
            (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(a0, (uint32_t)v0))));
            {
                const uint32_t target = ra;
                (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v0, 256))));
                Ps2SetGpr64(ctx, 2, v0);
                ctx->pc = target;
                return;
            }
        }
    }
    {
        const uint32_t target = ra;
        (v0 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL));
        Ps2SetGpr64(ctx, 2, v0);
        ctx->pc = target;
        return;
    }
}

// getPassageNum (0x1431d18): lift_d1's text (the reference)
void getPassageNum(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t v0 = Ps2GprU32(ctx, 2), a0 = Ps2GprU32(ctx, 4), ra = Ps2GprU32(ctx, 31);
    {
        const uint32_t target = ra;
        v0 = READ32_B(a0, 896);
        Ps2SetGpr64(ctx, 2, sx64(v0));
        ctx->pc = target;
        return;
    }
}

// setRealColJudgeFunc (0x1431ed0): lift_d1's text (the reference)
void setRealColJudgeFunc(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t a0 = Ps2GprU32(ctx, 4), a1 = Ps2GprU32(ctx, 5), ra = Ps2GprU32(ctx, 31);
    {
        const uint32_t target = ra;
        WRITE32_B(a0, 15444, a1);
        
        ctx->pc = target;
        return;
    }
}

// getRealColJudgeFunc (0x1431ed8): lift_d1's text (the reference)
void getRealColJudgeFunc(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t v0 = Ps2GprU32(ctx, 2), a0 = Ps2GprU32(ctx, 4), ra = Ps2GprU32(ctx, 31);
    {
        const uint32_t target = ra;
        v0 = READ32_B(a0, 15444);
        Ps2SetGpr64(ctx, 2, sx64(v0));
        ctx->pc = target;
        return;
    }
}

// setSelfColAndNearMinDist (0x1431ee0): lift_d1's text (the reference)
void setSelfColAndNearMinDist(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t a0 = Ps2GprU32(ctx, 4), a1 = Ps2GprU32(ctx, 5), ra = Ps2GprU32(ctx, 31);
    float f12 = ctx->f[12];
    WRITE32_B(a0, 964, a1);
    {
        const uint32_t target = ra;
        WRITE32_B(a0, 960, lift_d1::bits(f12));
        
        ctx->pc = target;
        return;
    }
}

// setStabilizeYPathWork (0x1431f38): lift_d1's text (the reference)
void setStabilizeYPathWork(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t a0 = Ps2GprU32(ctx, 4), a1 = Ps2GprU32(ctx, 5), ra = Ps2GprU32(ctx, 31);
    {
        const uint32_t target = ra;
        WRITE32_B(a0, 232, a1);
        
        ctx->pc = target;
        return;
    }
}

// setAngleStabilizeYPathWork (0x1431f40): lift_d1's text (the reference)
void setAngleStabilizeYPathWork(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t a0 = Ps2GprU32(ctx, 4), a1 = Ps2GprU32(ctx, 5), ra = Ps2GprU32(ctx, 31);
    {
        const uint32_t target = ra;
        WRITE32_B(a0, 236, a1);
        
        ctx->pc = target;
        return;
    }
}

// setRayByRealColPathWork (0x1431f48): lift_d1's text (the reference)
void setRayByRealColPathWork(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t a0 = Ps2GprU32(ctx, 4), a1 = Ps2GprU32(ctx, 5), ra = Ps2GprU32(ctx, 31);
    {
        const uint32_t target = ra;
        WRITE32_B(a0, 240, a1);
        
        ctx->pc = target;
        return;
    }
}

// setPassageWideRayPathWork (0x1431f50): lift_d1's text (the reference)
void setPassageWideRayPathWork(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t a0 = Ps2GprU32(ctx, 4), a1 = Ps2GprU32(ctx, 5), ra = Ps2GprU32(ctx, 31);
    {
        const uint32_t target = ra;
        WRITE32_B(a0, 244, a1);
        
        ctx->pc = target;
        return;
    }
}

// setExecRayChkPathWork (0x1431f58): lift_d1's text (the reference)
void setExecRayChkPathWork(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t a0 = Ps2GprU32(ctx, 4), a1 = Ps2GprU32(ctx, 5), ra = Ps2GprU32(ctx, 31);
    {
        const uint32_t target = ra;
        WRITE32_B(a0, 900, a1);
        
        ctx->pc = target;
        return;
    }
}

// setForceRouteAttrPathWork (0x1431fa8): lift_d1's text (the reference)
void setForceRouteAttrPathWork(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t a0 = Ps2GprU32(ctx, 4), a1 = Ps2GprU32(ctx, 5), ra = Ps2GprU32(ctx, 31);
    {
        const uint32_t target = ra;
        WRITE32_B(a0, 908, a1);
        
        ctx->pc = target;
        return;
    }
}

// setRouteColCheckPathWork (0x1431fb0): lift_d1's text (the reference)
void setRouteColCheckPathWork(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t a0 = Ps2GprU32(ctx, 4), a1 = Ps2GprU32(ctx, 5), ra = Ps2GprU32(ctx, 31);
    {
        const uint32_t target = ra;
        WRITE32_B(a0, 912, a1);
        
        ctx->pc = target;
        return;
    }
}

// setRouteLockPathWork (0x1431fb8): lift_d1's text (the reference)
void setRouteLockPathWork(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t a0 = Ps2GprU32(ctx, 4), a1 = Ps2GprU32(ctx, 5), ra = Ps2GprU32(ctx, 31);
    {
        const uint32_t target = ra;
        WRITE32_B(a0, 916, a1);
        
        ctx->pc = target;
        return;
    }
}

// setPlayerWayColCheckPathWork (0x1431fc0): lift_d1's text (the reference)
void setPlayerWayColCheckPathWork(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t a0 = Ps2GprU32(ctx, 4), a1 = Ps2GprU32(ctx, 5), ra = Ps2GprU32(ctx, 31);
    {
        const uint32_t target = ra;
        WRITE32_B(a0, 920, a1);
        
        ctx->pc = target;
        return;
    }
}

// setFollowModePathWork (0x1431fc8): lift_d1's text (the reference)
void setFollowModePathWork(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t a0 = Ps2GprU32(ctx, 4), a1 = Ps2GprU32(ctx, 5), ra = Ps2GprU32(ctx, 31);
    {
        const uint32_t target = ra;
        WRITE32_B(a0, 924, a1);
        
        ctx->pc = target;
        return;
    }
}

// setUserPathMode (0x1432018): lift_d1's text (the reference)
void setUserPathMode(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t a0 = Ps2GprU32(ctx, 4), a1 = Ps2GprU32(ctx, 5), ra = Ps2GprU32(ctx, 31);
    {
        const uint32_t target = ra;
        WRITE32_B(a0, 928, a1);
        
        ctx->pc = target;
        return;
    }
}

// setPassageRangeData (0x1432020): lift_d1's text (the reference)
void setPassageRangeData(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t a0 = Ps2GprU32(ctx, 4), ra = Ps2GprU32(ctx, 31);
    float f12 = ctx->f[12], f13 = ctx->f[13];
    WRITE32_B(a0, 976, lift_d1::bits(f13));
    {
        const uint32_t target = ra;
        WRITE32_B(a0, 972, lift_d1::bits(f12));
        
        ctx->pc = target;
        return;
    }
}

// setAttainRangeStruct (0x1432050): lift_d1's text (the reference)
void setAttainRangeStruct(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t a0 = Ps2GprU32(ctx, 4), a1 = Ps2GprU32(ctx, 5), ra = Ps2GprU32(ctx, 31);
    uint64_t v0 = Ps2GprU64(ctx, 2), v1 = Ps2GprU64(ctx, 3);
    (v0 = (uint64_t)(READ64_B(a1, 0)));
    WRITE64_B(a0, 992, v0);
    (v1 = (uint64_t)(READ64_B(a1, 8)));
    WRITE64_B(a0, 1000, v1);
    (v0 = (uint64_t)(READ64_B(a1, 16)));
    WRITE64_B(a0, 1008, v0);
    (v1 = (uint64_t)(READ64_B(a1, 24)));
    {
        const uint32_t target = ra;
        WRITE64_B(a0, 1016, v1);
        Ps2SetGpr64(ctx, 2, v0); Ps2SetGpr64(ctx, 3, v1);
        ctx->pc = target;
        return;
    }
}

// setCreateTempAttain (0x14320b0): lift_d1's text (the reference)
void setCreateTempAttain(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t a0 = Ps2GprU32(ctx, 4), a1 = Ps2GprU32(ctx, 5), ra = Ps2GprU32(ctx, 31);
    {
        const uint32_t target = ra;
        WRITE32_B(a0, 1044, a1);
        
        ctx->pc = target;
        return;
    }
}

// setAttainModePathWork (0x1432700): lift_d1's text (the reference)
void setAttainModePathWork(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t a0 = Ps2GprU32(ctx, 4), a1 = Ps2GprU32(ctx, 5), ra = Ps2GprU32(ctx, 31);
    {
        const uint32_t target = ra;
        WRITE32_B(a0, 224, a1);
        
        ctx->pc = target;
        return;
    }
}

// setAttainRadPathWork (0x1432708): lift_d1's text (the reference)
void setAttainRadPathWork(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t a0 = Ps2GprU32(ctx, 4), ra = Ps2GprU32(ctx, 31);
    float f12 = ctx->f[12];
    {
        const uint32_t target = ra;
        WRITE32_B(a0, 228, lift_d1::bits(f12));
        
        ctx->pc = target;
        return;
    }
}

// getPathInfo (0x1432710): lift_d1's text (the reference)
void getPathInfo(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t at = Ps2GprU32(ctx, 1), v0 = Ps2GprU32(ctx, 2), v1 = Ps2GprU32(ctx, 3), a0 = Ps2GprU32(ctx, 4), ra = Ps2GprU32(ctx, 31);
    v1 = READ32_B(a0, 15440);
    v0 = v1 << 2;
    v0 = v0 + v1;
    v0 = v0 << 2;
    v0 = v0 - v1;
    v0 = v0 << 3;
    at = 0x1200000u;
    at = at + 0x10C8u;
    v0 = at + v0;
    {
        const uint32_t target = ra;
        Ps2SetGpr64(ctx, 1, sx64(at)); Ps2SetGpr64(ctx, 2, sx64(v0)); Ps2SetGpr64(ctx, 3, sx64(v1));
        ctx->pc = target;
        return;
    }
}

// getWayFinish (0x1432740): lift_d1's text (the reference)
void getWayFinish(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t a0 = Ps2GprU32(ctx, 4), ra = Ps2GprU32(ctx, 31);
    uint64_t v0 = Ps2GprU64(ctx, 2), v1 = Ps2GprU64(ctx, 3);
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)READ32_B(a0, 1032))));
    {
        const bool c = (int64_t)v0 == 0;
        (v1 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL));
        if (!c) {
            (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)READ32_B(a0, 896))));
            (v1 = (uint64_t)(((uint64_t)RC_U64X(v0) < (uint64_t)(int64_t)(int32_t)1) ? 1 : 0));
        }
    }
    {
        const uint32_t target = ra;
        (v0 = (uint64_t)((uint64_t)RC_U64X(v1) + (uint64_t)0ULL));
        Ps2SetGpr64(ctx, 2, v0); Ps2SetGpr64(ctx, 3, v1);
        ctx->pc = target;
        return;
    }
}

// getPathStatus (0x1432760): lift_d1's text (the reference)
void getPathStatus(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t v0 = Ps2GprU32(ctx, 2), v1 = Ps2GprU32(ctx, 3), a0 = Ps2GprU32(ctx, 4), a1 = Ps2GprU32(ctx, 5), ra = Ps2GprU32(ctx, 31);
    bool df_v1 = false;
    a1 = READ32_B(a0, 928);
    if (a1 == 0) {
        v0 = READ32_B(a0, 896);
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
                    const uint32_t target = ra;
                    v0 = 0u;
                    Ps2SetGpr64(ctx, 2, sx64(v0)); if (df_v1) Ps2SetGpr64(ctx, 3, sx64(v1)); Ps2SetGpr64(ctx, 5, sx64(a1));
                    ctx->pc = target;
                    return;
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
        v0 = READ32_B(a0, 1032);
        if (v0 == 0) {
            v0 = 0u;
            goto L_14327c8;
        }
    }
    {
        const uint32_t target = ra;
        v0 = 1u;
        Ps2SetGpr64(ctx, 2, sx64(v0)); Ps2SetGpr64(ctx, 5, sx64(a1));
        ctx->pc = target;
        return;
    }
L_14327b8:
    v1 = READ32_B(a0, 896); df_v1 = true;
    {
        const bool c = v1 != 0;
        v0 = 1u;
        if (!c) {
            v0 = 0u;
        }
    }
L_14327c8:
    {
        const uint32_t target = ra;
        Ps2SetGpr64(ctx, 2, sx64(v0)); if (df_v1) Ps2SetGpr64(ctx, 3, sx64(v1)); Ps2SetGpr64(ctx, 5, sx64(a1));
        ctx->pc = target;
        return;
    }
}

// getLastPassagePointIdx (0x1432808): lift_d1's text (the reference)
void getLastPassagePointIdx(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t v0 = Ps2GprU32(ctx, 2), a0 = Ps2GprU32(ctx, 4), ra = Ps2GprU32(ctx, 31);
    {
        const uint32_t target = ra;
        v0 = READ32_B(a0, 1060);
        Ps2SetGpr64(ctx, 2, sx64(v0));
        ctx->pc = target;
        return;
    }
}

// getLastPassageAreaIdx (0x1432810): lift_d1's text (the reference)
void getLastPassageAreaIdx(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t v0 = Ps2GprU32(ctx, 2), a0 = Ps2GprU32(ctx, 4), ra = Ps2GprU32(ctx, 31);
    {
        const uint32_t target = ra;
        v0 = READ32_B(a0, 1064);
        Ps2SetGpr64(ctx, 2, sx64(v0));
        ctx->pc = target;
        return;
    }
}

// getNowPassagePointIdx (0x1432818): lift_d1's text (the reference)
void getNowPassagePointIdx(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t v0 = Ps2GprU32(ctx, 2), a0 = Ps2GprU32(ctx, 4), ra = Ps2GprU32(ctx, 31);
    {
        const uint32_t target = ra;
        v0 = READ32_B(a0, 1068);
        Ps2SetGpr64(ctx, 2, sx64(v0));
        ctx->pc = target;
        return;
    }
}

// getNowPassageAreaIdx (0x1432820): lift_d1's text (the reference)
void getNowPassageAreaIdx(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t v0 = Ps2GprU32(ctx, 2), a0 = Ps2GprU32(ctx, 4), ra = Ps2GprU32(ctx, 31);
    {
        const uint32_t target = ra;
        v0 = READ32_B(a0, 1072);
        Ps2SetGpr64(ctx, 2, sx64(v0));
        ctx->pc = target;
        return;
    }
}

// setSearchRadius (0x14328a8): lift_d1's text (the reference)
void setSearchRadius(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t v0 = Ps2GprU32(ctx, 2), a0 = Ps2GprU32(ctx, 4), ra = Ps2GprU32(ctx, 31);
    float f0 = ctx->f[0], f12 = ctx->f[12];
    uint32_t fcr31 = ctx->fcr31;
    f0 = lift_d1::flt(0u);
    fcr31 = (FPU_C_OLT_S(f12, f0)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
    {
        const bool c = (fcr31 & 0x800000u) == 0u;
        v0 = 0x1470000u;
        if (!c) {
            f0 = lift_d1::flt(READ32(v0 - 0x38F4u));
            f0 = ps2FpuSqrt(fcr31, f0);
            {
                const uint32_t target = ra;
                WRITE32_B(a0, 952, lift_d1::bits(f0));
                ctx->f[0] = f0; Ps2SetGpr64(ctx, 2, sx64(v0)); ctx->fcr31 = fcr31;
                ctx->pc = target;
                return;
            }
        }
    }
    {
        const uint32_t target = ra;
        WRITE32_B(a0, 952, lift_d1::bits(f12));
        ctx->f[0] = f0; Ps2SetGpr64(ctx, 2, sx64(v0)); ctx->fcr31 = fcr31;
        ctx->pc = target;
        return;
    }
}

// setSearchPointMax (0x14328d0): lift_d1's text (the reference)
void setSearchPointMax(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t v0 = Ps2GprU32(ctx, 2), a0 = Ps2GprU32(ctx, 4), ra = Ps2GprU32(ctx, 31);
    uint64_t a1 = Ps2GprU64(ctx, 5);
    {
        const bool c = (int32_t)a1 > 0;
        v0 = 0x380u;
        if (!c) {
            {
                const uint32_t target = ra;
                WRITE32_B(a0, 956, v0);
                Ps2SetGpr64(ctx, 2, sx64(v0));
                ctx->pc = target;
                return;
            }
        }
    }
    {
        const uint32_t target = ra;
        WRITE32_B(a0, 956, (uint32_t)a1);
        Ps2SetGpr64(ctx, 2, sx64(v0));
        ctx->pc = target;
        return;
    }
}

// setPathRayWideParam (0x14328e8): lift_d1's text (the reference)
void setPathRayWideParam(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t a0 = Ps2GprU32(ctx, 4), a1 = Ps2GprU32(ctx, 5), ra = Ps2GprU32(ctx, 31);
    float f12 = ctx->f[12];
    WRITE32_B(a0, 15424, a1);
    {
        const uint32_t target = ra;
        WRITE32_B(a0, 15420, lift_d1::bits(f12));
        
        ctx->pc = target;
        return;
    }
}

// resetPassageWork (0x1433120): lift_d1's text (the reference)
void resetPassageWork(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t a0 = Ps2GprU32(ctx, 4), ra = Ps2GprU32(ctx, 31);
    {
        const uint32_t target = ra;
        WRITE32_B(a0, 896, 0u);
        
        ctx->pc = target;
        return;
    }
}

// getColiGroupByUserPathWork (0x1433f60): lift_d1's text (the reference)
void getColiGroupByUserPathWork(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t v0 = Ps2GprU32(ctx, 2), v1 = Ps2GprU32(ctx, 3), a0 = Ps2GprU32(ctx, 4), ra = Ps2GprU32(ctx, 31);
    v1 = READ32_B(a0, 15440);
    if (v1 != 0) {
        v0 = 1u;
    } else {
        v0 = 0x1460000u;
        {
            const uint32_t target = ra;
            v0 = v0 + 0x5888u;
            Ps2SetGpr64(ctx, 2, sx64(v0)); Ps2SetGpr64(ctx, 3, sx64(v1));
            ctx->pc = target;
            return;
        }
    }
    if (v1 == v0) {
        v0 = 0x1460000u;
        {
            const uint32_t target = ra;
            v0 = v0 + 0x5884u;
            Ps2SetGpr64(ctx, 2, sx64(v0)); Ps2SetGpr64(ctx, 3, sx64(v1));
            ctx->pc = target;
            return;
        }
    }
    {
        const uint32_t target = ra;
        v0 = 0u;
        Ps2SetGpr64(ctx, 2, sx64(v0)); Ps2SetGpr64(ctx, 3, sx64(v1));
        ctx->pc = target;
        return;
    }
}

// gamecore_sub_01434CF8 (0x1434cf8): lift_d1's text (the reference)
void gamecore_sub_01434CF8(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t v1 = Ps2GprU32(ctx, 3), ra = Ps2GprU32(ctx, 31);
    uint64_t v0 = Ps2GprU64(ctx, 2), a0 = Ps2GprU64(ctx, 4), a1 = Ps2GprU64(ctx, 5);
    (a1 = (uint64_t)((uint64_t)a0 + (uint64_t)0ULL));
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 1))));
    (v1 = (uint32_t)((int32_t)((int32_t)READ32_B((uint32_t)a1, 15440))));
    {
        const bool c = (int64_t)(int32_t)v1 != (int64_t)a0;
        (v0 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL));
        if (!c) {
            (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)READ32_B((uint32_t)a1, 920))));
            {
                const bool c = (int64_t)v0 == 0;
                (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)323 << 16))));
                if (!c) {
                    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)288 << 16))));
                    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)READ32_B((uint32_t)v0, 4908))));
                    if ((int64_t)v0 == 0) {
                        (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)323 << 16))));
                    } else {
                        (v1 = (uint32_t)((int32_t)((int32_t)READ32_B((uint32_t)v0, 1456))));
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
        const uint32_t target = ra;
        Ps2SetGpr64(ctx, 2, v0); Ps2SetGpr64(ctx, 3, sx64(v1)); Ps2SetGpr64(ctx, 4, a0); Ps2SetGpr64(ctx, 5, a1);
        ctx->pc = target;
        return;
    }
}

// gamecore_sub_01434F90 (0x1434f90): lift_d1's text (the reference)
void gamecore_sub_01434F90(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t v1 = Ps2GprU32(ctx, 3), a0 = Ps2GprU32(ctx, 4), ra = Ps2GprU32(ctx, 31);
    uint64_t v0 = Ps2GprU64(ctx, 2);
    float f0 = ctx->f[0], f1 = ctx->f[1];
    uint32_t fcr31 = ctx->fcr31;
    bool df_v1 = false;
    f1 = lift_d1::flt(0u);
    f0 = lift_d1::flt(READ32_B(a0, 15420));
    fcr31 = (FPU_C_OLT_S(f1, f0)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
    {
        const bool c = (fcr31 & 0x800000u) == 0u;
        (v0 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL));
        if (!c) {
            v1 = READ32_B(a0, 15424); df_v1 = true;
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
        const uint32_t target = ra;
        ctx->f[0] = f0; ctx->f[1] = f1; Ps2SetGpr64(ctx, 2, v0); if (df_v1) Ps2SetGpr64(ctx, 3, sx64(v1)); ctx->fcr31 = fcr31;
        ctx->pc = target;
        return;
    }
}

// setUserPathExMode (0x14359c8): lift_d1's text (the reference)
void setUserPathExMode(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t a0 = Ps2GprU32(ctx, 4), a1 = Ps2GprU32(ctx, 5), ra = Ps2GprU32(ctx, 31);
    {
        const uint32_t target = ra;
        WRITE32_B(a0, 15456, a1);
        
        ctx->pc = target;
        return;
    }
}

// setUserPathGroupID (0x14359d0): lift_d1's text (the reference)
void setUserPathGroupID(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t a0 = Ps2GprU32(ctx, 4), a1 = Ps2GprU32(ctx, 5), a2 = Ps2GprU32(ctx, 6), ra = Ps2GprU32(ctx, 31);
    WRITE32_B(a0, 15468, a2);
    {
        const uint32_t target = ra;
        WRITE32_B(a0, 15464, a1);
        
        ctx->pc = target;
        return;
    }
}

// setGPSelectMode (0x1435a50): lift_d1's text (the reference)
void setGPSelectMode(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t a0 = Ps2GprU32(ctx, 4), a1 = Ps2GprU32(ctx, 5), ra = Ps2GprU32(ctx, 31);
    {
        const uint32_t target = ra;
        WRITE32_B(a0, 15480, a1);
        
        ctx->pc = target;
        return;
    }
}

// setForceInitPathEx (0x1435a58): lift_d1's text (the reference)
void setForceInitPathEx(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t a0 = Ps2GprU32(ctx, 4), a1 = Ps2GprU32(ctx, 5), ra = Ps2GprU32(ctx, 31);
    {
        const uint32_t target = ra;
        WRITE32_B(a0, 15484, a1);
        
        ctx->pc = target;
        return;
    }
}

// setGpReferencePathEx (0x1435a60): lift_d1's text (the reference)
void setGpReferencePathEx(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t a0 = Ps2GprU32(ctx, 4), a1 = Ps2GprU32(ctx, 5), ra = Ps2GprU32(ctx, 31);
    {
        const uint32_t target = ra;
        WRITE32_B(a0, 15488, a1);
        
        ctx->pc = target;
        return;
    }
}

// gamecore_sub_01435EF0 (0x1435ef0): lift_d1's text (the reference)
void gamecore_sub_01435EF0(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t v0 = Ps2GprU32(ctx, 2), a0 = Ps2GprU32(ctx, 4), ra = Ps2GprU32(ctx, 31);
    v0 = 0xFFFFFFFFu;
    WRITE32_B(a0, 15516, v0);
    {
        const uint32_t target = ra;
        WRITE32_B(a0, 15512, v0);
        Ps2SetGpr64(ctx, 2, sx64(v0));
        ctx->pc = target;
        return;
    }
}

// _setExecBaseMoveWork (0x1437a60): lift_d1's text (the reference)
void _setExecBaseMoveWork(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t a0 = Ps2GprU32(ctx, 4), a1 = Ps2GprU32(ctx, 5), ra = Ps2GprU32(ctx, 31);
    {
        const uint32_t target = ra;
        WRITE32_B(a0, 4, a1);
        
        ctx->pc = target;
        return;
    }
}

// _setStopCtrlModeBaseMoveWork (0x1437a68): lift_d1's text (the reference)
void _setStopCtrlModeBaseMoveWork(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t a0 = Ps2GprU32(ctx, 4), a1 = Ps2GprU32(ctx, 5), ra = Ps2GprU32(ctx, 31);
    {
        const uint32_t target = ra;
        WRITE32_B(a0, 32, a1);
        
        ctx->pc = target;
        return;
    }
}

// _getStopCtrlModeBaseMoveWork (0x1437a70): lift_d1's text (the reference)
void _getStopCtrlModeBaseMoveWork(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t v0 = Ps2GprU32(ctx, 2), a0 = Ps2GprU32(ctx, 4), ra = Ps2GprU32(ctx, 31);
    {
        const uint32_t target = ra;
        v0 = READ32_B(a0, 32);
        Ps2SetGpr64(ctx, 2, sx64(v0));
        ctx->pc = target;
        return;
    }
}

// _setBackVolumeBaseMoveWork (0x1437a78): lift_d1's text (the reference)
void _setBackVolumeBaseMoveWork(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t a0 = Ps2GprU32(ctx, 4), a1 = Ps2GprU32(ctx, 5), ra = Ps2GprU32(ctx, 31);
    {
        const uint32_t target = ra;
        WRITE32_B(a0, 36, a1);
        
        ctx->pc = target;
        return;
    }
}

// _setSlideVolumeBaseMoveWork (0x1437a80): lift_d1's text (the reference)
void _setSlideVolumeBaseMoveWork(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t a0 = Ps2GprU32(ctx, 4), a1 = Ps2GprU32(ctx, 5), ra = Ps2GprU32(ctx, 31);
    {
        const uint32_t target = ra;
        WRITE32_B(a0, 40, a1);
        
        ctx->pc = target;
        return;
    }
}

// _setAttainFlagBaseMoveWork (0x1437b88): lift_d1's text (the reference)
void _setAttainFlagBaseMoveWork(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t a0 = Ps2GprU32(ctx, 4), a1 = Ps2GprU32(ctx, 5), ra = Ps2GprU32(ctx, 31);
    {
        const uint32_t target = ra;
        WRITE32(a0, a1);
        
        ctx->pc = target;
        return;
    }
}

// gamecore_sub_01438448 (0x1438448): lift_d1's text (the reference)
void gamecore_sub_01438448(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t at = Ps2GprU32(ctx, 1), v0 = Ps2GprU32(ctx, 2), a0 = Ps2GprU32(ctx, 4), ra = Ps2GprU32(ctx, 31);
    float f0 = ctx->f[0], f1 = ctx->f[1];
    v0 = 0x1470000u;
    at = 0x3F800000u;
    f1 = lift_d1::flt(at);
    f0 = lift_d1::flt(READ32(v0 - 0x37F0u));
    WRITE32_B(a0, 28, lift_d1::bits(f1));
    WRITE32_B(a0, 24, lift_d1::bits(f0));
    WRITE32_B(a0, 16, lift_d1::bits(f0));
    {
        const uint32_t target = ra;
        WRITE32_B(a0, 20, lift_d1::bits(f0));
        ctx->f[0] = f0; ctx->f[1] = f1; Ps2SetGpr64(ctx, 1, sx64(at)); Ps2SetGpr64(ctx, 2, sx64(v0));
        ctx->pc = target;
        return;
    }
}

// resetRouteAttackBirdCtrlWork (0x143d5c8): lift_d1's text (the reference)
void resetRouteAttackBirdCtrlWork(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t a0 = Ps2GprU32(ctx, 4), ra = Ps2GprU32(ctx, 31);
    WRITE32_B(a0, 340, 0u);
    {
        const uint32_t target = ra;
        WRITE32_B(a0, 48, 0u);
        
        ctx->pc = target;
        return;
    }
}

// setFlapWingBirdCtrlWork (0x143d5d8): lift_d1's text (the reference)
void setFlapWingBirdCtrlWork(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t a0 = Ps2GprU32(ctx, 4), a1 = Ps2GprU32(ctx, 5), ra = Ps2GprU32(ctx, 31);
    {
        const uint32_t target = ra;
        WRITE32_B(a0, 52, a1);
        
        ctx->pc = target;
        return;
    }
}

// setHoveringBirdCtrlWork (0x143d5e0): lift_d1's text (the reference)
void setHoveringBirdCtrlWork(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t a0 = Ps2GprU32(ctx, 4), a1 = Ps2GprU32(ctx, 5), ra = Ps2GprU32(ctx, 31);
    {
        const uint32_t target = ra;
        WRITE32_B(a0, 56, a1);
        
        ctx->pc = target;
        return;
    }
}

// setHoverAttackBirdCtrlWork (0x143d5e8): lift_d1's text (the reference)
void setHoverAttackBirdCtrlWork(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t a0 = Ps2GprU32(ctx, 4), a1 = Ps2GprU32(ctx, 5), ra = Ps2GprU32(ctx, 31);
    {
        const uint32_t target = ra;
        WRITE32_B(a0, 60, a1);
        
        ctx->pc = target;
        return;
    }
}

// gamecore_sub_0143D660 (0x143d660): lift_d1's text (the reference)
void gamecore_sub_0143D660(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t at = Ps2GprU32(ctx, 1), v0 = Ps2GprU32(ctx, 2), a0 = Ps2GprU32(ctx, 4), ra = Ps2GprU32(ctx, 31);
    float f0 = ctx->f[0], f1 = ctx->f[1];
    v0 = 0x1470000u;
    at = 0x3F800000u;
    f1 = lift_d1::flt(at);
    f0 = lift_d1::flt(READ32(v0 - 0x379Cu));
    WRITE32_B(a0, 92, lift_d1::bits(f1));
    WRITE32_B(a0, 88, lift_d1::bits(f0));
    WRITE32_B(a0, 64, 0u);
    WRITE32_B(a0, 96, 0u);
    WRITE32_B(a0, 68, 0u);
    WRITE32_B(a0, 80, lift_d1::bits(f0));
    {
        const uint32_t target = ra;
        WRITE32_B(a0, 84, lift_d1::bits(f0));
        ctx->f[0] = f0; ctx->f[1] = f1; Ps2SetGpr64(ctx, 1, sx64(at)); Ps2SetGpr64(ctx, 2, sx64(v0));
        ctx->pc = target;
        return;
    }
}

// setDashFastLegCtrlWork (0x1440ef0): lift_d1's text (the reference)
void setDashFastLegCtrlWork(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t a0 = Ps2GprU32(ctx, 4), a1 = Ps2GprU32(ctx, 5), ra = Ps2GprU32(ctx, 31);
    {
        const uint32_t target = ra;
        WRITE32_B(a0, 52, a1);
        
        ctx->pc = target;
        return;
    }
}

// setFinalAttainWalkFastLegCtrlWork (0x1440ef8): lift_d1's text (the reference)
void setFinalAttainWalkFastLegCtrlWork(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t a0 = Ps2GprU32(ctx, 4), a1 = Ps2GprU32(ctx, 5), ra = Ps2GprU32(ctx, 31);
    {
        const uint32_t target = ra;
        WRITE32_B(a0, 48, a1);
        
        ctx->pc = target;
        return;
    }
}

// setAllowMaxSpeedFastLegCtrlWork (0x1440f00): lift_d1's text (the reference)
void setAllowMaxSpeedFastLegCtrlWork(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t a0 = Ps2GprU32(ctx, 4), ra = Ps2GprU32(ctx, 31);
    float f0 = ctx->f[0], f12 = ctx->f[12];
    uint32_t fcr31 = ctx->fcr31;
    f0 = lift_d1::flt(READ32_B(a0, 124));
    fcr31 = (FPU_C_OLT_S(f0, f12)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
    if ((fcr31 & 0x800000u) != 0u) {
        WRITE32_B(a0, 80, lift_d1::bits(f12));
    } else {
    }
    {
        const uint32_t target = ra;
        ctx->f[0] = f0; ctx->fcr31 = fcr31;
        ctx->pc = target;
        return;
    }
}

// setBackFastLegCtrlWork (0x1440f18): lift_d1's text (the reference)
void setBackFastLegCtrlWork(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t a0 = Ps2GprU32(ctx, 4), a1 = Ps2GprU32(ctx, 5), ra = Ps2GprU32(ctx, 31);
    {
        const uint32_t target = ra;
        WRITE32_B(a0, 56, a1);
        
        ctx->pc = target;
        return;
    }
}

// setNotFastTurnFastLegCtrlWork (0x1440f20): lift_d1's text (the reference)
void setNotFastTurnFastLegCtrlWork(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t a0 = Ps2GprU32(ctx, 4), a1 = Ps2GprU32(ctx, 5), ra = Ps2GprU32(ctx, 31);
    {
        const uint32_t target = ra;
        WRITE32_B(a0, 60, a1);
        
        ctx->pc = target;
        return;
    }
}

// setRunStopTurnFastLegCtrlWork (0x1440f28): lift_d1's text (the reference)
void setRunStopTurnFastLegCtrlWork(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t a0 = Ps2GprU32(ctx, 4), a1 = Ps2GprU32(ctx, 5), ra = Ps2GprU32(ctx, 31);
    {
        const uint32_t target = ra;
        WRITE32_B(a0, 64, a1);
        
        ctx->pc = target;
        return;
    }
}

// setWalkStopTurnFastLegCtrlWork (0x1440f30): lift_d1's text (the reference)
void setWalkStopTurnFastLegCtrlWork(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t a0 = Ps2GprU32(ctx, 4), a1 = Ps2GprU32(ctx, 5), ra = Ps2GprU32(ctx, 31);
    {
        const uint32_t target = ra;
        WRITE32_B(a0, 68, a1);
        
        ctx->pc = target;
        return;
    }
}

// setCurveCollisionFastLegCtrlWork (0x1440f38): lift_d1's text (the reference)
void setCurveCollisionFastLegCtrlWork(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t a0 = Ps2GprU32(ctx, 4), a1 = Ps2GprU32(ctx, 5), ra = Ps2GprU32(ctx, 31);
    {
        const uint32_t target = ra;
        WRITE32_B(a0, 72, a1);
        
        ctx->pc = target;
        return;
    }
}

// setExecGazeCtrlWork (0x14418d0): lift_d1's text (the reference)
void setExecGazeCtrlWork(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t a0 = Ps2GprU32(ctx, 4), a1 = Ps2GprU32(ctx, 5), ra = Ps2GprU32(ctx, 31);
    {
        const uint32_t target = ra;
        WRITE32(a0, a1);
        
        ctx->pc = target;
        return;
    }
}

// setViewJudgeGazeCtrlWork (0x14418d8): lift_d1's text (the reference)
void setViewJudgeGazeCtrlWork(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t a0 = Ps2GprU32(ctx, 4), a1 = Ps2GprU32(ctx, 5), ra = Ps2GprU32(ctx, 31);
    {
        const uint32_t target = ra;
        WRITE32_B(a0, 4, a1);
        
        ctx->pc = target;
        return;
    }
}

// setNextDistGazeCtrlWork (0x14418e0): lift_d1's text (the reference)
void setNextDistGazeCtrlWork(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t a0 = Ps2GprU32(ctx, 4), ra = Ps2GprU32(ctx, 31);
    float f12 = ctx->f[12];
    {
        const uint32_t target = ra;
        WRITE32_B(a0, 8, lift_d1::bits(f12));
        
        ctx->pc = target;
        return;
    }
}

// setValidPosGazeCtrlWork (0x14418e8): lift_d1's text (the reference)
void setValidPosGazeCtrlWork(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t a0 = Ps2GprU32(ctx, 4), a1 = Ps2GprU32(ctx, 5), ra = Ps2GprU32(ctx, 31);
    {
        const uint32_t target = ra;
        WRITE32_B(a0, 20, a1);
        
        ctx->pc = target;
        return;
    }
}

// setBirdModeGazeCtrlWork (0x1441968): lift_d1's text (the reference)
void setBirdModeGazeCtrlWork(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t a0 = Ps2GprU32(ctx, 4), a1 = Ps2GprU32(ctx, 5), ra = Ps2GprU32(ctx, 31);
    {
        const uint32_t target = ra;
        WRITE32_B(a0, 12, a1);
        
        ctx->pc = target;
        return;
    }
}

// setGazeCoffGazeCtrlWork (0x1441970): lift_d1's text (the reference)
void setGazeCoffGazeCtrlWork(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t a0 = Ps2GprU32(ctx, 4), ra = Ps2GprU32(ctx, 31);
    float f12 = ctx->f[12];
    {
        const uint32_t target = ra;
        WRITE32_B(a0, 16, lift_d1::bits(f12));
        
        ctx->pc = target;
        return;
    }
}

// setDashHorseCtrlWork (0x14444d0): lift_d1's text (the reference)
void setDashHorseCtrlWork(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t a0 = Ps2GprU32(ctx, 4), a1 = Ps2GprU32(ctx, 5), ra = Ps2GprU32(ctx, 31);
    {
        const uint32_t target = ra;
        WRITE32_B(a0, 52, a1);
        
        ctx->pc = target;
        return;
    }
}

// setWallStopHorseCtrlWork (0x14444d8): lift_d1's text (the reference)
void setWallStopHorseCtrlWork(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t a0 = Ps2GprU32(ctx, 4), a1 = Ps2GprU32(ctx, 5), ra = Ps2GprU32(ctx, 31);
    {
        const uint32_t target = ra;
        WRITE32_B(a0, 56, a1);
        
        ctx->pc = target;
        return;
    }
}

// setLimitModeHorseCtrlWork (0x14444e0): lift_d1's text (the reference)
void setLimitModeHorseCtrlWork(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t a0 = Ps2GprU32(ctx, 4), a1 = Ps2GprU32(ctx, 5), ra = Ps2GprU32(ctx, 31);
    {
        const uint32_t target = ra;
        WRITE32_B(a0, 68, a1);
        
        ctx->pc = target;
        return;
    }
}

// setFinalAttainWalkHorseCtrlWork (0x14444e8): lift_d1's text (the reference)
void setFinalAttainWalkHorseCtrlWork(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t a0 = Ps2GprU32(ctx, 4), a1 = Ps2GprU32(ctx, 5), ra = Ps2GprU32(ctx, 31);
    {
        const uint32_t target = ra;
        WRITE32_B(a0, 48, a1);
        
        ctx->pc = target;
        return;
    }
}

// setAllowMaxSpeedHorseCtrlWork (0x14444f0): lift_d1's text (the reference)
void setAllowMaxSpeedHorseCtrlWork(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t v0 = Ps2GprU32(ctx, 2), a0 = Ps2GprU32(ctx, 4), ra = Ps2GprU32(ctx, 31);
    float f0 = ctx->f[0], f12 = ctx->f[12];
    uint32_t fcr31 = ctx->fcr31;
    v0 = READ32_B(a0, 120);
    f0 = lift_d1::flt(READ32(v0));
    fcr31 = (FPU_C_OLT_S(f0, f12)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
    if ((fcr31 & 0x800000u) != 0u) {
        WRITE32_B(a0, 64, lift_d1::bits(f12));
    } else {
    }
    {
        const uint32_t target = ra;
        ctx->f[0] = f0; Ps2SetGpr64(ctx, 2, sx64(v0)); ctx->fcr31 = fcr31;
        ctx->pc = target;
        return;
    }
}

// setFreezeHorseCtrlWork (0x1444510): lift_d1's text (the reference)
void setFreezeHorseCtrlWork(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t a0 = Ps2GprU32(ctx, 4), a1 = Ps2GprU32(ctx, 5), ra = Ps2GprU32(ctx, 31);
    {
        const uint32_t target = ra;
        WRITE32_B(a0, 60, a1);
        
        ctx->pc = target;
        return;
    }
}

// chkMoveAngle (0x14460a8): lift_d1's text (the reference)
void chkMoveAngle(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t at = Ps2GprU32(ctx, 1), v0 = Ps2GprU32(ctx, 2), ra = Ps2GprU32(ctx, 31);
    float f0 = ctx->f[0], f1 = ctx->f[1], f12 = ctx->f[12], f13 = ctx->f[13];
    uint32_t fcr31 = ctx->fcr31;
    f0 = FPU_ADD_S(f12, f13);
    at = 0x3C8E0000u;
    at = at | 0xFA35u;
    f1 = lift_d1::flt(at);
    f0 = FPU_ABS_S(f0);
    fcr31 = (FPU_C_OLE_S(f0, f1)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
    {
        const bool c = (fcr31 & 0x800000u) != 0u;
        v0 = 1u;
        if (!c) {
            f0 = lift_d1::flt(0u);
            fcr31 = (FPU_C_OLE_S(f0, f13)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
            if ((fcr31 & 0x800000u) != 0u) {
                fcr31 = (FPU_C_OLE_S(f0, f12)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
                if ((fcr31 & 0x800000u) != 0u) {
                    f1 = FPU_ABS_S(f12);
                    goto L_1446100;
                }
            }
            fcr31 = (FPU_C_OLE_S(f13, f0)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
            {
                const bool c = (fcr31 & 0x800000u) == 0u;
                v0 = 0u;
                if (c) {
                    goto L_1446114;
                }
            }
            fcr31 = (FPU_C_OLE_S(f12, f0)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
            if ((fcr31 & 0x800000u) == 0u) {
                goto L_1446114;
            }
            f1 = FPU_ABS_S(f12);
L_1446100:
            f0 = FPU_ABS_S(f13);
            fcr31 = (FPU_C_OLE_S(f0, f1)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
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
        const uint32_t target = ra;
        ctx->f[0] = f0; ctx->f[1] = f1; Ps2SetGpr64(ctx, 1, sx64(at)); Ps2SetGpr64(ctx, 2, sx64(v0)); ctx->fcr31 = fcr31;
        ctx->pc = target;
        return;
    }
}

// adjustAngleForLimit (0x1446868): lift_d1's text (the reference)
void adjustAngleForLimit(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t ra = Ps2GprU32(ctx, 31);
    float f0 = ctx->f[0], f12 = ctx->f[12], f13 = ctx->f[13];
    uint32_t fcr31 = ctx->fcr31;
    fcr31 = (FPU_C_OLT_S(f13, f12)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
    {
        const bool c = (fcr31 & 0x800000u) != 0u;
        f0 = FPU_MOV_S(f13);
        if (!c) {
            f0 = FPU_NEG_S(f13);
            fcr31 = (FPU_C_OLT_S(f12, f0)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
            if ((fcr31 & 0x800000u) == 0u) {
                f0 = FPU_MOV_S(f12);
            } else {
            }
        }
    }
    {
        const uint32_t target = ra;
        ctx->f[0] = f0; ctx->fcr31 = fcr31;
        ctx->pc = target;
        return;
    }
}

// setDashSlowLegCtrlWork (0x1449810): lift_d1's text (the reference)
void setDashSlowLegCtrlWork(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t a0 = Ps2GprU32(ctx, 4), a1 = Ps2GprU32(ctx, 5), ra = Ps2GprU32(ctx, 31);
    {
        const uint32_t target = ra;
        WRITE32_B(a0, 48, a1);
        
        ctx->pc = target;
        return;
    }
}

// setStabilizeYSlowLegCtrlWork (0x1449818): lift_d1's text (the reference)
void setStabilizeYSlowLegCtrlWork(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t a0 = Ps2GprU32(ctx, 4), a1 = Ps2GprU32(ctx, 5), ra = Ps2GprU32(ctx, 31);
    {
        const uint32_t target = ra;
        WRITE32_B(a0, 60, a1);
        
        ctx->pc = target;
        return;
    }
}

// setMoveModeSlowLegCtrlWork (0x1449820): lift_d1's text (the reference)
void setMoveModeSlowLegCtrlWork(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t a0 = Ps2GprU32(ctx, 4), a1 = Ps2GprU32(ctx, 5), ra = Ps2GprU32(ctx, 31);
    {
        const uint32_t target = ra;
        WRITE32_B(a0, 52, a1);
        
        ctx->pc = target;
        return;
    }
}

// setMotScaleMaxSlowLegCtrlWork (0x1449828): lift_d1's text (the reference)
void setMotScaleMaxSlowLegCtrlWork(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t a0 = Ps2GprU32(ctx, 4), ra = Ps2GprU32(ctx, 31);
    float f12 = ctx->f[12];
    {
        const uint32_t target = ra;
        WRITE32_B(a0, 56, lift_d1::bits(f12));
        
        ctx->pc = target;
        return;
    }
}

// setGuardModeSlowLegCtrlWork (0x1449838): lift_d1's text (the reference)
void setGuardModeSlowLegCtrlWork(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t a0 = Ps2GprU32(ctx, 4), a1 = Ps2GprU32(ctx, 5), ra = Ps2GprU32(ctx, 31);
    {
        const uint32_t target = ra;
        WRITE32_B(a0, 156, a1);
        
        ctx->pc = target;
        return;
    }
}

// gamecore_sub_014498B8 (0x14498b8): lift_d1's text (the reference)
void gamecore_sub_014498B8(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t a0 = Ps2GprU32(ctx, 4), ra = Ps2GprU32(ctx, 31);
    {
        const uint32_t target = ra;
        WRITE32_B(a0, 148, 0u);
        
        ctx->pc = target;
        return;
    }
}

// setAttainSpeedWormCtrlWork (0x144cc60): lift_d1's text (the reference)
void setAttainSpeedWormCtrlWork(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t a0 = Ps2GprU32(ctx, 4), ra = Ps2GprU32(ctx, 31);
    uint64_t v0 = Ps2GprU64(ctx, 2);
    float f0 = ctx->f[0], f12 = ctx->f[12];
    uint32_t fcr31 = ctx->fcr31;
    f0 = lift_d1::flt(0u);
    fcr31 = (FPU_C_OLE_S(f0, f12)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
    if ((fcr31 & 0x800000u) != 0u) {
        f0 = lift_d1::flt(READ32_B(a0, 88));
        fcr31 = (FPU_C_OLE_S(f12, f0)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
        if ((fcr31 & 0x800000u) != 0u) {
            WRITE32_B(a0, 176, lift_d1::bits(f12));
            {
                const uint32_t target = ra;
                (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 1))));
                ctx->f[0] = f0; Ps2SetGpr64(ctx, 2, v0); ctx->fcr31 = fcr31;
                ctx->pc = target;
                return;
            }
        }
    }
    {
        const uint32_t target = ra;
        (v0 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL));
        ctx->f[0] = f0; Ps2SetGpr64(ctx, 2, v0); ctx->fcr31 = fcr31;
        ctx->pc = target;
        return;
    }
}

// setAttackParamWormCtrlWork (0x144cc98): lift_d1's text (the reference)
void setAttackParamWormCtrlWork(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t a0 = Ps2GprU32(ctx, 4), ra = Ps2GprU32(ctx, 31);
    float f12 = ctx->f[12];
    {
        const uint32_t target = ra;
        WRITE32_B(a0, 184, lift_d1::bits(f12));
        
        ctx->pc = target;
        return;
    }
}

// setRotParamWormCtrlWork (0x144cd18): lift_d1's text (the reference)
void setRotParamWormCtrlWork(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t at = Ps2GprU32(ctx, 1), a0 = Ps2GprU32(ctx, 4), ra = Ps2GprU32(ctx, 31);
    float f0 = ctx->f[0], f12 = ctx->f[12], f13 = ctx->f[13];
    at = 0x3C8E0000u;
    at = at | 0xFA35u;
    f0 = lift_d1::flt(at);
    WRITE32_B(a0, 108, lift_d1::bits(f12));
    f13 = FPU_MUL_S(f13, f0);
    {
        const uint32_t target = ra;
        WRITE32_B(a0, 112, lift_d1::bits(f13));
        ctx->f[0] = f0; ctx->f[13] = f13; Ps2SetGpr64(ctx, 1, sx64(at));
        ctx->pc = target;
        return;
    }
}

// setChkKeepHitWormCtrlWork (0x144cd38): lift_d1's text (the reference)
void setChkKeepHitWormCtrlWork(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t a0 = Ps2GprU32(ctx, 4), a1 = Ps2GprU32(ctx, 5), ra = Ps2GprU32(ctx, 31);
    {
        const uint32_t target = ra;
        WRITE32_B(a0, 208, a1);
        
        ctx->pc = target;
        return;
    }
}

// setRotColCheckWormCtrlWork (0x144cd40): lift_d1's text (the reference)
void setRotColCheckWormCtrlWork(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t a0 = Ps2GprU32(ctx, 4), a1 = Ps2GprU32(ctx, 5), ra = Ps2GprU32(ctx, 31);
    {
        const uint32_t target = ra;
        WRITE32_B(a0, 216, a1);
        
        ctx->pc = target;
        return;
    }
}

// setObstacleWormCtrlWork (0x144cd50): lift_d1's text (the reference)
void setObstacleWormCtrlWork(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t a0 = Ps2GprU32(ctx, 4), a1 = Ps2GprU32(ctx, 5), ra = Ps2GprU32(ctx, 31);
    {
        const uint32_t target = ra;
        WRITE32_B(a0, 224, a1);
        
        ctx->pc = target;
        return;
    }
}

// setSelfEvasionUpWormCtrlWork (0x144cd58): lift_d1's text (the reference)
void setSelfEvasionUpWormCtrlWork(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t a0 = Ps2GprU32(ctx, 4), a1 = Ps2GprU32(ctx, 5), ra = Ps2GprU32(ctx, 31);
    {
        const uint32_t target = ra;
        WRITE32_B(a0, 228, a1);
        
        ctx->pc = target;
        return;
    }
}

// setTypeWormCtrlWork (0x144cd60): lift_d1's text (the reference)
void setTypeWormCtrlWork(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t a0 = Ps2GprU32(ctx, 4), a1 = Ps2GprU32(ctx, 5), ra = Ps2GprU32(ctx, 31);
    {
        const uint32_t target = ra;
        WRITE32_B(a0, 236, a1);
        
        ctx->pc = target;
        return;
    }
}

// setFallGravCoffWormCtrlWork (0x144cd68): lift_d1's text (the reference)
void setFallGravCoffWormCtrlWork(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t a0 = Ps2GprU32(ctx, 4), ra = Ps2GprU32(ctx, 31);
    float f12 = ctx->f[12];
    {
        const uint32_t target = ra;
        WRITE32_B(a0, 136, lift_d1::bits(f12));
        
        ctx->pc = target;
        return;
    }
}

// setMaxFallSpeedWormCtrlWork (0x144cd70): lift_d1's text (the reference)
void setMaxFallSpeedWormCtrlWork(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t a0 = Ps2GprU32(ctx, 4), ra = Ps2GprU32(ctx, 31);
    float f12 = ctx->f[12];
    {
        const uint32_t target = ra;
        WRITE32_B(a0, 140, lift_d1::bits(f12));
        
        ctx->pc = target;
        return;
    }
}

// setGroundFrictionCoffWormCtrlWork (0x144cd78): lift_d1's text (the reference)
void setGroundFrictionCoffWormCtrlWork(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t a0 = Ps2GprU32(ctx, 4), ra = Ps2GprU32(ctx, 31);
    float f12 = ctx->f[12];
    {
        const uint32_t target = ra;
        WRITE32_B(a0, 144, lift_d1::bits(f12));
        
        ctx->pc = target;
        return;
    }
}

// setUnderFrictionCoffWormCtrlWork (0x144cd80): lift_d1's text (the reference)
void setUnderFrictionCoffWormCtrlWork(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t a0 = Ps2GprU32(ctx, 4), ra = Ps2GprU32(ctx, 31);
    float f12 = ctx->f[12];
    {
        const uint32_t target = ra;
        WRITE32_B(a0, 148, lift_d1::bits(f12));
        
        ctx->pc = target;
        return;
    }
}

// setWorldPitchLimitWormCtrlWork (0x144cd88): lift_d1's text (the reference)
void setWorldPitchLimitWormCtrlWork(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t at = Ps2GprU32(ctx, 1), a0 = Ps2GprU32(ctx, 4), ra = Ps2GprU32(ctx, 31);
    float f0 = ctx->f[0], f12 = ctx->f[12], f13 = ctx->f[13];
    at = 0x3C8E0000u;
    at = at | 0xFA35u;
    f0 = lift_d1::flt(at);
    f12 = FPU_MUL_S(f12, f0);
    f13 = FPU_MUL_S(f13, f0);
    f12 = FPU_ABS_S(f12);
    f13 = FPU_ABS_S(f13);
    f12 = FPU_NEG_S(f12);
    WRITE32_B(a0, 120, lift_d1::bits(f13));
    {
        const uint32_t target = ra;
        WRITE32_B(a0, 116, lift_d1::bits(f12));
        ctx->f[0] = f0; ctx->f[12] = f12; ctx->f[13] = f13; Ps2SetGpr64(ctx, 1, sx64(at));
        ctx->pc = target;
        return;
    }
}

// setColOffsetJudgeFunc (0x144cde8): lift_d1's text (the reference)
void setColOffsetJudgeFunc(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t a0 = Ps2GprU32(ctx, 4), a1 = Ps2GprU32(ctx, 5), ra = Ps2GprU32(ctx, 31);
    {
        const uint32_t target = ra;
        WRITE32_B(a0, 240, a1);
        
        ctx->pc = target;
        return;
    }
}

// gamecore_sub_0144CEA8 (0x144cea8): lift_d1's text (the reference)
void gamecore_sub_0144CEA8(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t at = Ps2GprU32(ctx, 1), v0 = Ps2GprU32(ctx, 2), a0 = Ps2GprU32(ctx, 4), ra = Ps2GprU32(ctx, 31);
    float f0 = ctx->f[0], f1 = ctx->f[1];
    v0 = 0x1470000u;
    at = 0x3F800000u;
    f1 = lift_d1::flt(at);
    f0 = lift_d1::flt(READ32(v0 - 0x35A8u));
    WRITE32_B(a0, 76, lift_d1::bits(f1));
    WRITE32_B(a0, 72, lift_d1::bits(f0));
    WRITE32_B(a0, 48, 0u);
    WRITE32_B(a0, 80, 0u);
    WRITE32_B(a0, 52, 0u);
    WRITE32_B(a0, 64, lift_d1::bits(f0));
    {
        const uint32_t target = ra;
        WRITE32_B(a0, 68, lift_d1::bits(f0));
        ctx->f[0] = f0; ctx->f[1] = f1; Ps2SetGpr64(ctx, 1, sx64(at)); Ps2SetGpr64(ctx, 2, sx64(v0));
        ctx->pc = target;
        return;
    }
}

// createArrowThrustWork (0x144d3a0): lift_d1's text (the reference)
void createArrowThrustWork(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t a1 = Ps2GprU32(ctx, 5), a2 = Ps2GprU32(ctx, 6), ra = Ps2GprU32(ctx, 31);
    WRITE32(a1, a2);
    {
        const uint32_t target = ra;
        WRITE32_B(a1, 4, 0u);
        
        ctx->pc = target;
        return;
    }
}

// SCRConditionChkDisplayOmakeCompleteAlert (0x1459a50): lift_d1's text (the reference)
void SCRConditionChkDisplayOmakeCompleteAlert(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t v0 = Ps2GprU32(ctx, 2), ra = Ps2GprU32(ctx, 31);
    uint64_t a1 = Ps2GprU64(ctx, 5);
    if ((int64_t)a1 == 0) {
        v0 = 0x1460000u;
    } else {
        v0 = 0x1460000u;
        {
            const uint32_t target = ra;
            v0 = READ32_B(v0, 26756);
            Ps2SetGpr64(ctx, 2, sx64(v0));
            ctx->pc = target;
            return;
        }
    }
    {
        const uint32_t target = ra;
        v0 = READ32_B(v0, 26752);
        Ps2SetGpr64(ctx, 2, sx64(v0));
        ctx->pc = target;
        return;
    }
}

// setScrDemoMangerFinishCallback (0x145ae68): lift_d1's text (the reference)
void setScrDemoMangerFinishCallback(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t v0 = Ps2GprU32(ctx, 2), v1 = Ps2GprU32(ctx, 3), a0 = Ps2GprU32(ctx, 4), a1 = Ps2GprU32(ctx, 5), ra = Ps2GprU32(ctx, 31);
    v0 = 0x1460000u;
    v1 = 0x1460000u;
    WRITE32_B(v0, 29680, a0);
    {
        const uint32_t target = ra;
        WRITE32_B(v1, 29684, a1);
        Ps2SetGpr64(ctx, 2, sx64(v0)); Ps2SetGpr64(ctx, 3, sx64(v1));
        ctx->pc = target;
        return;
    }
}

// initInterestWork (0x145c100): lift_d1's text (the reference)
void initInterestWork(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t v0 = Ps2GprU32(ctx, 2), a1 = Ps2GprU32(ctx, 5), ra = Ps2GprU32(ctx, 31);
    v0 = 0xFFFFFFFFu;
    WRITE32_B(a1, 12, 0u);
    WRITE32_B(a1, 8, v0);
    WRITE32(a1, 0u);
    {
        const uint32_t target = ra;
        WRITE32_B(a1, 4, 0u);
        Ps2SetGpr64(ctx, 2, sx64(v0));
        ctx->pc = target;
        return;
    }
}

// getInterestExec (0x145c118): lift_d1's text (the reference)
void getInterestExec(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t v0 = Ps2GprU32(ctx, 2), a0 = Ps2GprU32(ctx, 4), ra = Ps2GprU32(ctx, 31);
    v0 = READ32(a0);
    {
        const uint32_t target = ra;
        v0 = 0u < v0 ? 1u : 0u;
        Ps2SetGpr64(ctx, 2, sx64(v0));
        ctx->pc = target;
        return;
    }
}

// setPerceptionParam (0x145c990): lift_d1's text (the reference)
void setPerceptionParam(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t a0 = Ps2GprU32(ctx, 4), a1 = Ps2GprU32(ctx, 5), ra = Ps2GprU32(ctx, 31);
    {
        const uint32_t target = ra;
        WRITE32_B(a0, 64, a1);
        
        ctx->pc = target;
        return;
    }
}

// setGoingAngleGazeIKWork (0x14617b8): lift_d1's text (the reference)
void setGoingAngleGazeIKWork(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t v0 = Ps2GprU32(ctx, 2), a0 = Ps2GprU32(ctx, 4), ra = Ps2GprU32(ctx, 31);
    float f12 = ctx->f[12];
    v0 = READ32_B(a0, 4364);
    if (v0 != 0) {
        WRITE32_B(v0, 64, lift_d1::bits(f12));
    } else {
    }
    {
        const uint32_t target = ra;
        Ps2SetGpr64(ctx, 2, sx64(v0));
        ctx->pc = target;
        return;
    }
}

// setSubParamGazeIKWork (0x14617f0): lift_d1's text (the reference)
void setSubParamGazeIKWork(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t v0 = Ps2GprU32(ctx, 2), a0 = Ps2GprU32(ctx, 4), a1 = Ps2GprU32(ctx, 5), ra = Ps2GprU32(ctx, 31);
    v0 = READ32_B(a0, 4364);
    if (v0 != 0) {
        WRITE32_B(v0, 188, a1);
    } else {
    }
    {
        const uint32_t target = ra;
        Ps2SetGpr64(ctx, 2, sx64(v0));
        ctx->pc = target;
        return;
    }
}

// setGazeModeGazeIKWork (0x1461808): lift_d1's text (the reference)
void setGazeModeGazeIKWork(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t v0 = Ps2GprU32(ctx, 2), a0 = Ps2GprU32(ctx, 4), a1 = Ps2GprU32(ctx, 5), ra = Ps2GprU32(ctx, 31);
    v0 = READ32_B(a0, 4364);
    if (v0 != 0) {
        WRITE32_B(v0, 96, a1);
    } else {
    }
    {
        const uint32_t target = ra;
        Ps2SetGpr64(ctx, 2, sx64(v0));
        ctx->pc = target;
        return;
    }
}

// setExecRootQuatGazeIKWork (0x1461820): lift_d1's text (the reference)
void setExecRootQuatGazeIKWork(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t v0 = Ps2GprU32(ctx, 2), a0 = Ps2GprU32(ctx, 4), a1 = Ps2GprU32(ctx, 5), ra = Ps2GprU32(ctx, 31);
    v0 = READ32_B(a0, 4364);
    if (v0 != 0) {
        WRITE32_B(v0, 176, a1);
    } else {
    }
    {
        const uint32_t target = ra;
        Ps2SetGpr64(ctx, 2, sx64(v0));
        ctx->pc = target;
        return;
    }
}

// setExecGazeIKWork (0x1461838): lift_d1's text (the reference)
void setExecGazeIKWork(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t v0 = Ps2GprU32(ctx, 2), a0 = Ps2GprU32(ctx, 4), a1 = Ps2GprU32(ctx, 5), ra = Ps2GprU32(ctx, 31);
    v0 = READ32_B(a0, 4364);
    if (v0 != 0) {
        WRITE32_B(v0, 48, a1);
    } else {
    }
    {
        const uint32_t target = ra;
        Ps2SetGpr64(ctx, 2, sx64(v0));
        ctx->pc = target;
        return;
    }
}

// initTailIKWork (0x14621b8): lift_d1's text (the reference)
void initTailIKWork(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t a2 = Ps2GprU32(ctx, 6), ra = Ps2GprU32(ctx, 31);
    WRITE32_B(a2, 964, 0u);
    WRITE32_B(a2, 960, 0u);
    {
        const uint32_t target = ra;
        WRITE32_B(a2, 968, 0u);
        
        ctx->pc = target;
        return;
    }
}

// setWormIKBoundDynamics (0x14647e0): lift_d1's text (the reference)
void setWormIKBoundDynamics(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t a0 = Ps2GprU32(ctx, 4), ra = Ps2GprU32(ctx, 31);
    float f12 = ctx->f[12], f13 = ctx->f[13], f14 = ctx->f[14];
    WRITE32_B(a0, 4744, lift_d1::bits(f14));
    WRITE32_B(a0, 4736, lift_d1::bits(f12));
    {
        const uint32_t target = ra;
        WRITE32_B(a0, 4740, lift_d1::bits(f13));
        
        ctx->pc = target;
        return;
    }
}

// setWormIKBankInterpSec (0x1464820): lift_d1's text (the reference)
void setWormIKBankInterpSec(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t a0 = Ps2GprU32(ctx, 4), ra = Ps2GprU32(ctx, 31);
    float f12 = ctx->f[12];
    {
        const uint32_t target = ra;
        WRITE32_B(a0, 4748, lift_d1::bits(f12));
        
        ctx->pc = target;
        return;
    }
}

// getSScriptWork (0x1465308): lift_d1's text (the reference)
void getSScriptWork(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t v0 = Ps2GprU32(ctx, 2), ra = Ps2GprU32(ctx, 31);
    v0 = 0x1200000u;
    {
        const uint32_t target = ra;
        v0 = v0 + 0x1440u;
        Ps2SetGpr64(ctx, 2, sx64(v0));
        ctx->pc = target;
        return;
    }
}
