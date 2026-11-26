// Round X1's check reference for xz (included by x1_charageo.cpp; generated from the recompiler's xz_0x10e7610.cpp by
// the round's mkxzref.py, docs/decomp_x1.md): the recompiled code's statements unchanged, its guest memory through the
// check's replay (reads see its own writes, then what X1's run recorded; writes never reach RAM) and each guest call a
// boundary compared with X1's recorded call (the context then set to what that call left). The two runtime dispatch
// lines of its return / fall-through are left out (the replay returns with pc = ra as the default build does).
namespace x1geo
{
    namespace xzref
    {
        inline Check *g_k = nullptr;
        template <class T>
        inline T rd(uint32_t a)
        {
            T v;
            g_k->replay.read(g_k->rdram, a, &v, sizeof(T));
            return v;
        }
        template <class T>
        inline void wr(uint32_t a, T v) { g_k->replay.write(a, &v, sizeof(T)); }
    }
#pragma push_macro("READ8")
#undef READ8
#pragma push_macro("READ16")
#undef READ16
#pragma push_macro("READ32")
#undef READ32
#pragma push_macro("READ64")
#undef READ64
#pragma push_macro("READ128")
#undef READ128
#pragma push_macro("WRITE8")
#undef WRITE8
#pragma push_macro("WRITE16")
#undef WRITE16
#pragma push_macro("WRITE32")
#undef WRITE32
#pragma push_macro("WRITE64")
#undef WRITE64
#pragma push_macro("WRITE128")
#undef WRITE128
#pragma push_macro("PS2_CALL_GUEST_KNOWN")
#undef PS2_CALL_GUEST_KNOWN
#pragma push_macro("PS2_CALL_GUEST_NOCHK")
#undef PS2_CALL_GUEST_NOCHK
#define READ8(addr) xzref::rd<uint8_t>((uint32_t)(addr))
#define READ16(addr) xzref::rd<uint16_t>((uint32_t)(addr))
#define READ32(addr) xzref::rd<uint32_t>((uint32_t)(addr))
#define READ64(addr) xzref::rd<uint64_t>((uint32_t)(addr))
#define READ128(addr) xzref::rd<__m128i>((uint32_t)(addr) & ~0xFu)
#define WRITE8(addr, val) xzref::wr<uint8_t>((uint32_t)(addr), (uint8_t)(val))
#define WRITE16(addr, val) xzref::wr<uint16_t>((uint32_t)(addr), (uint16_t)(val))
#define WRITE32(addr, val) xzref::wr<uint32_t>((uint32_t)(addr), (uint32_t)(val))
#define WRITE64(addr, val) xzref::wr<uint64_t>((uint32_t)(addr), (uint64_t)(val))
#define WRITE128(addr, val) xzref::wr<__m128i>((uint32_t)(addr) & ~0xFu, (val))
#define PS2_CALL_GUEST_KNOWN(target, function, returnPc, sourcePc, kind, name) (xzref::g_k->refBoundary((sourcePc), (target)), true)
#define PS2_CALL_GUEST_NOCHK(target, function, returnPc, sourcePc, kind, name) (xzref::g_k->refBoundary((sourcePc), (target)), true)
#if defined(__clang__)
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wunused-label"
#pragma clang diagnostic ignored "-Wunused-variable"
#elif defined(__GNUC__)
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-label"
#pragma GCC diagnostic ignored "-Wunused-variable"
#pragma GCC diagnostic ignored "-Wunused-but-set-variable"
#endif
    void xzRef(Check &k)
    {
        xzref::g_k = &k;
        uint8_t *const rdram = k.rdram;
        R5900Context *const ctx = k.rc;
        (void)rdram;

#ifdef PS2_FUNCTION_LOG_TRACKER
    PS_LOG_ENTRY("xz_0x10e7610");
#endif
    uint64_t rc_1;
    uint64_t rc_2;
    uint64_t rc_3;
    uint64_t rc_4;
    uint64_t rc_5;
    uint64_t rc_6;
    uint64_t rc_7;
    uint64_t rc_8;
    uint64_t rc_9;
    uint64_t rc_10;
    uint64_t rc_11;
    uint64_t rc_12;
    uint64_t rc_13;
    uint64_t rc_14;
    uint64_t rc_15;
    uint64_t rc_16;
    uint64_t rc_17;
    uint64_t rc_18;
    uint64_t rc_19;
    uint64_t rc_20;
    uint64_t rc_21;
    uint64_t rc_22;
    uint64_t rc_23;
    uint64_t rc_29;
    uint64_t rc_30;
    uint64_t rc_31;
    float rf_0;
    float rf_1;
    float rf_2;
    float rf_3;
    float rf_12;
    float rf_13;
    float rf_14;
    float rf_15;
    float rf_20;
    float rf_21;
    float rf_22;
    float rf_23;
    float rf_24;
    float rf_25;
    float rf_26;
    float rf_27;
    if (__builtin_expect(ctx->pc != 0x10e7610u, 0)) {
        switch (ctx->pc) {
            case 0x10e7680u: goto label_10e7680;
            case 0x10e7738u: goto label_10e7738;
            case 0x10e7760u: goto label_10e7760;
            case 0x10e7774u: goto label_10e7774;
            case 0x10e777cu: goto label_10e777c;
            case 0x10e7788u: goto label_10e7788;
            case 0x10e779cu: goto label_10e779c;
            case 0x10e785cu: goto label_10e785c;
            case 0x10e786cu: goto label_10e786c;
            case 0x10e78c4u: goto label_10e78c4;
            case 0x10e7960u: goto label_10e7960;
            case 0x10e798cu: goto label_10e798c;
            case 0x10e79a0u: goto label_10e79a0;
            case 0x10e7a00u: goto label_10e7a00;
            case 0x10e7a10u: goto label_10e7a10;
            case 0x10e7a44u: goto label_10e7a44;
            case 0x10e7a7cu: goto label_10e7a7c;
            case 0x10e7ab0u: goto label_10e7ab0;
            case 0x10e7b04u: goto label_10e7b04;
            case 0x10e7b38u: goto label_10e7b38;
            case 0x10e7b58u: goto label_10e7b58;
            case 0x10e7b8cu: goto label_10e7b8c;
            case 0x10e7ba0u: goto label_10e7ba0;
            case 0x10e7c34u: goto label_10e7c34;
            case 0x10e7c58u: goto label_10e7c58;
            case 0x10e7cb8u: goto label_10e7cb8;
            case 0x10e7cecu: goto label_10e7cec;
            case 0x10e7d30u: goto label_10e7d30;
            case 0x10e7d54u: goto label_10e7d54;
            case 0x10e7d60u: goto label_10e7d60;
            case 0x10e7d98u: goto label_10e7d98;
            case 0x10e7dbcu: goto label_10e7dbc;
            case 0x10e7dccu: goto label_10e7dcc;
            case 0x10e7dd8u: goto label_10e7dd8;
            case 0x10e7e18u: goto label_10e7e18;
            case 0x10e7e2cu: goto label_10e7e2c;
            case 0x10e7e60u: goto label_10e7e60;
            case 0x10e7f84u: goto label_10e7f84;
            case 0x10e7f94u: goto label_10e7f94;
            case 0x10e7fa4u: goto label_10e7fa4;
            case 0x10e7fc0u: goto label_10e7fc0;
            case 0x10e7fc8u: goto label_10e7fc8;
            case 0x10e8008u: goto label_10e8008;
            case 0x10e8028u: goto label_10e8028;
            case 0x10e8048u: goto label_10e8048;
            case 0x10e8064u: goto label_10e8064;
            case 0x10e80a4u: goto label_10e80a4;
            case 0x10e80b8u: goto label_10e80b8;
            default: ctx->pc = 0x10e7610u; break;
        }
    }
    // 0x10e7610: 0x27bdfda0  addiu       $sp, $sp, -0x260
    rc_29 = Ps2GprU64(ctx, 29); 
    RC_SET_S32(rc_29, (int32_t)ADD32(RC_U32(rc_29), 4294966688));
    Ps2SetGpr64(ctx, 29, RC_U64X(rc_29)); 
    // 0x10e7614: 0xffb101d8  sd          $s1, 0x1D8($sp)
    rc_17 = Ps2GprU64(ctx, 17); 
    WRITE64(ADD32(RC_U32(rc_29), 472), RC_U64(rc_17));
    // 0x10e7618: 0xffb201e0  sd          $s2, 0x1E0($sp)
    rc_18 = Ps2GprU64(ctx, 18); 
    WRITE64(ADD32(RC_U32(rc_29), 480), RC_U64(rc_18));
    // 0x10e761c: 0xffb301e8  sd          $s3, 0x1E8($sp)
    rc_19 = Ps2GprU64(ctx, 19); 
    WRITE64(ADD32(RC_U32(rc_29), 488), RC_U64(rc_19));
    // 0x10e7620: 0xa0982d  daddu       $s3, $a1, $zero
    rc_5 = Ps2GprU64(ctx, 5); 
    RC_SET_U64(rc_19, (uint64_t)RC_U64(rc_5) + (uint64_t)RC_U64(0));
    Ps2SetGpr64(ctx, 19, rc_19); 
    // 0x10e7624: 0xffb401f0  sd          $s4, 0x1F0($sp)
    rc_20 = Ps2GprU64(ctx, 20); 
    WRITE64(ADD32(RC_U32(rc_29), 496), RC_U64(rc_20));
    // 0x10e7628: 0x100a02d  daddu       $s4, $t0, $zero
    rc_8 = Ps2GprU64(ctx, 8); 
    RC_SET_U64(rc_20, (uint64_t)RC_U64(rc_8) + (uint64_t)RC_U64(0));
    Ps2SetGpr64(ctx, 20, rc_20); 
    // 0x10e762c: 0xffb501f8  sd          $s5, 0x1F8($sp)
    rc_21 = Ps2GprU64(ctx, 21); 
    WRITE64(ADD32(RC_U32(rc_29), 504), RC_U64(rc_21));
    // 0x10e7630: 0xffb60200  sd          $s6, 0x200($sp)
    rc_22 = Ps2GprU64(ctx, 22); 
    WRITE64(ADD32(RC_U32(rc_29), 512), RC_U64(rc_22));
    // 0x10e7634: 0xffb70208  sd          $s7, 0x208($sp)
    rc_23 = Ps2GprU64(ctx, 23); 
    WRITE64(ADD32(RC_U32(rc_29), 520), RC_U64(rc_23));
    // 0x10e7638: 0xffbe0210  sd          $fp, 0x210($sp)
    rc_30 = Ps2GprU64(ctx, 30); 
    WRITE64(ADD32(RC_U32(rc_29), 528), RC_U64(rc_30));
    // 0x10e763c: 0xe7bb0258  swc1        $f27, 0x258($sp)
    rf_27 = ctx->f[27]; 
    { float f = rf_27; uint32_t bits; std::memcpy(&bits, &f, sizeof(bits)); WRITE32(ADD32(RC_U32(rc_29), 600), bits); }
    // 0x10e7640: 0x460076c6  mov.s       $f27, $f14
    rf_14 = ctx->f[14]; 
    rf_27 = FPU_MOV_S(rf_14);
    ctx->f[27] = rf_27; 
    // 0x10e7644: 0xe7ba0250  swc1        $f26, 0x250($sp)
    rf_26 = ctx->f[26]; 
    { float f = rf_26; uint32_t bits; std::memcpy(&bits, &f, sizeof(bits)); WRITE32(ADD32(RC_U32(rc_29), 592), bits); }
    // 0x10e7648: 0xe7b70238  swc1        $f23, 0x238($sp)
    rf_23 = ctx->f[23]; 
    { float f = rf_23; uint32_t bits; std::memcpy(&bits, &f, sizeof(bits)); WRITE32(ADD32(RC_U32(rc_29), 568), bits); }
    // 0x10e764c: 0x46006dc6  mov.s       $f23, $f13
    rf_13 = ctx->f[13]; 
    rf_23 = FPU_MOV_S(rf_13);
    ctx->f[23] = rf_23; 
    // 0x10e7650: 0xe7b40220  swc1        $f20, 0x220($sp)
    rf_20 = ctx->f[20]; 
    { float f = rf_20; uint32_t bits; std::memcpy(&bits, &f, sizeof(bits)); WRITE32(ADD32(RC_U32(rc_29), 544), bits); }
    // 0x10e7654: 0x46006506  mov.s       $f20, $f12
    rf_12 = ctx->f[12]; 
    rf_20 = FPU_MOV_S(rf_12);
    ctx->f[20] = rf_20; 
    // 0x10e7658: 0xafa601a4  sw          $a2, 0x1A4($sp)
    rc_6 = Ps2GprU64(ctx, 6); 
    WRITE32(ADD32(RC_U32(rc_29), 420), RC_U32(rc_6));
    // 0x10e765c: 0xafa401a0  sw          $a0, 0x1A0($sp)
    rc_4 = Ps2GprU64(ctx, 4); 
    WRITE32(ADD32(RC_U32(rc_29), 416), RC_U32(rc_4));
    // 0x10e7660: 0xffb001d0  sd          $s0, 0x1D0($sp)
    rc_16 = Ps2GprU64(ctx, 16); 
    WRITE64(ADD32(RC_U32(rc_29), 464), RC_U64(rc_16));
    // 0x10e7664: 0xffbf0218  sd          $ra, 0x218($sp)
    rc_31 = Ps2GprU64(ctx, 31); 
    WRITE64(ADD32(RC_U32(rc_29), 536), RC_U64(rc_31));
    // 0x10e7668: 0xe7b90248  swc1        $f25, 0x248($sp)
    rf_25 = ctx->f[25]; 
    { float f = rf_25; uint32_t bits; std::memcpy(&bits, &f, sizeof(bits)); WRITE32(ADD32(RC_U32(rc_29), 584), bits); }
    // 0x10e766c: 0xe7b80240  swc1        $f24, 0x240($sp)
    rf_24 = ctx->f[24]; 
    { float f = rf_24; uint32_t bits; std::memcpy(&bits, &f, sizeof(bits)); WRITE32(ADD32(RC_U32(rc_29), 576), bits); }
    // 0x10e7670: 0xe7b60230  swc1        $f22, 0x230($sp)
    rf_22 = ctx->f[22]; 
    { float f = rf_22; uint32_t bits; std::memcpy(&bits, &f, sizeof(bits)); WRITE32(ADD32(RC_U32(rc_29), 560), bits); }
    // 0x10e7674: 0xe7b50228  swc1        $f21, 0x228($sp)
    rf_21 = ctx->f[21]; 
    { float f = rf_21; uint32_t bits; std::memcpy(&bits, &f, sizeof(bits)); WRITE32(ADD32(RC_U32(rc_29), 552), bits); }
    // 0x10e7678: 0xc43029c  jal         func_10C0A70
    RC_SET_U32(rc_31, 0x10E7680u);
    Ps2SetGpr64(ctx, 31, RC_U64X(rc_31)); 
    // 0x10e767c: 0xafa701a8  sw          $a3, 0x1A8($sp) (Delay Slot)
    rc_7 = Ps2GprU64(ctx, 7); 
    WRITE32(ADD32(RC_U32(rc_29), 424), RC_U32(rc_7));
    ctx->pc = 0x10C0A70u;
    if (!PS2_CALL_GUEST_KNOWN(0x10C0A70u, GetCharacterClass_0x10c0a70, 0x10E7680u, 0x10E7678u, DirectCall, "JAL")) {
        return;
    }
label_10e7680:
    // 0x10e7680: 0x40882d  daddu       $s1, $v0, $zero
    rc_2 = Ps2GprU64(ctx, 2); 
    RC_SET_U64(rc_17, (uint64_t)RC_U64(rc_2) + (uint64_t)RC_U64(0));
    Ps2SetGpr64(ctx, 17, rc_17); 
    // 0x10e7684: 0x3c020003  lui         $v0, 0x3
    RC_SET_S32(rc_2, (int32_t)((uint32_t)3 << 16));
    // 0x10e7688: 0x8e2301c0  lw          $v1, 0x1C0($s1)
    RC_SET_S32(rc_3, (int32_t)READ32(ADD32(RC_U32(rc_17), 448)));
    // 0x10e768c: 0x262400c0  addiu       $a0, $s1, 0xC0
    RC_SET_S32(rc_4, (int32_t)ADD32(RC_U32(rc_17), 192));
    // 0x10e7690: 0x3442ffff  ori         $v0, $v0, 0xFFFF
    RC_SET_U64(rc_2, RC_U64X(rc_2) | (uint64_t)(uint16_t)65535);
    // 0x10e7694: 0x263706d0  addiu       $s7, $s1, 0x6D0
    RC_SET_S32(rc_23, (int32_t)ADD32(RC_U32(rc_17), 1744));
    Ps2SetGpr64(ctx, 23, RC_U64X(rc_23)); 
    // 0x10e7698: 0xafa301b4  sw          $v1, 0x1B4($sp)
    rc_29 = Ps2GprU64(ctx, 29); 
    WRITE32(ADD32(RC_U32(rc_29), 436), RC_U32(rc_3));
    // 0x10e769c: 0x2463000c  addiu       $v1, $v1, 0xC
    RC_SET_S32(rc_3, (int32_t)ADD32(RC_U32(rc_3), 12));
    Ps2SetGpr64(ctx, 3, RC_U64X(rc_3)); 
    // 0x10e76a0: 0x263e0e20  addiu       $fp, $s1, 0xE20
    RC_SET_S32(rc_30, (int32_t)ADD32(RC_U32(rc_17), 3616));
    Ps2SetGpr64(ctx, 30, RC_U64X(rc_30)); 
    // 0x10e76a4: 0x263504c0  addiu       $s5, $s1, 0x4C0
    RC_SET_S32(rc_21, (int32_t)ADD32(RC_U32(rc_17), 1216));
    Ps2SetGpr64(ctx, 21, RC_U64X(rc_21)); 
    // 0x10e76a8: 0x8c660000  lw          $a2, 0x0($v1)
    RC_SET_S32(rc_6, (int32_t)READ32(ADD32(RC_U32(rc_3), 0)));
    Ps2SetGpr64(ctx, 6, RC_U64X(rc_6)); 
    // 0x10e76ac: 0x8e320134  lw          $s2, 0x134($s1)
    RC_SET_S32(rc_18, (int32_t)READ32(ADD32(RC_U32(rc_17), 308)));
    Ps2SetGpr64(ctx, 18, RC_U64X(rc_18)); 
    // 0x10e76b0: 0x8e36013c  lw          $s6, 0x13C($s1)
    RC_SET_S32(rc_22, (int32_t)READ32(ADD32(RC_U32(rc_17), 316)));
    Ps2SetGpr64(ctx, 22, RC_U64X(rc_22)); 
    // 0x10e76b4: 0xc21024  and         $v0, $a2, $v0
    RC_SET_U64(rc_2, RC_U64X(rc_6) & RC_U64X(rc_2));
    Ps2SetGpr64(ctx, 2, RC_U64X(rc_2)); 
    // 0x10e76b8: 0xafa401ac  sw          $a0, 0x1AC($sp)
    WRITE32(ADD32(RC_U32(rc_29), 428), RC_U32(rc_4));
    // 0x10e76bc: 0x8e240138  lw          $a0, 0x138($s1)
    RC_SET_S32(rc_4, (int32_t)READ32(ADD32(RC_U32(rc_17), 312)));
    // 0x10e76c0: 0xafa401b0  sw          $a0, 0x1B0($sp)
    WRITE32(ADD32(RC_U32(rc_29), 432), RC_U32(rc_4));
    // 0x10e76c4: 0xc63a0e34  lwc1        $f26, 0xE34($s1)
    { uint32_t bits = READ32(ADD32(RC_U32(rc_17), 3636)); float f; std::memcpy(&f, &bits, sizeof(f)); rf_26 = f; }
    ctx->f[26] = rf_26; 
    // 0x10e76c8: 0xafa001bc  sw          $zero, 0x1BC($sp)
    WRITE32(ADD32(RC_U32(rc_29), 444), RC_U32(0));
    // 0x10e76cc: 0x8e240e40  lw          $a0, 0xE40($s1)
    RC_SET_S32(rc_4, (int32_t)READ32(ADD32(RC_U32(rc_17), 3648)));
    Ps2SetGpr64(ctx, 4, RC_U64X(rc_4)); 
    // 0x10e76d0: 0xafa001c4  sw          $zero, 0x1C4($sp)
    WRITE32(ADD32(RC_U32(rc_29), 452), RC_U32(0));
    // 0x10e76d4: 0x10400004  beqz        $v0, . + 4 + (0x4 << 2)
    {
        const bool branch_taken_0x10e76d4 = (RC_U64X(rc_2) == RC_U64(0));
        // 0x10e76d8: 0xafa401b8  sw          $a0, 0x1B8($sp) (Delay Slot)
        WRITE32(ADD32(RC_U32(rc_29), 440), RC_U32(rc_4));
        if (branch_taken_0x10e76d4) {
            goto label_10e76e8;
        }
    }
    // 0x10e76dc: 0x621023  subu        $v0, $v1, $v0
    RC_SET_S32(rc_2, (int32_t)SUB32(RC_U32(rc_3), RC_U32(rc_2)));
    Ps2SetGpr64(ctx, 2, RC_U64X(rc_2)); 
    // 0x10e76e0: 0x10000002  b           . + 4 + (0x2 << 2)
    {
        const bool branch_taken_0x10e76e0 = (RC_U64(0) == RC_U64(0));
        // 0x10e76e4: 0x8c440000  lw          $a0, 0x0($v0) (Delay Slot)
        RC_SET_S32(rc_4, (int32_t)READ32(ADD32(RC_U32(rc_2), 0)));
        Ps2SetGpr64(ctx, 4, RC_U64X(rc_4)); 
        if (branch_taken_0x10e76e0) {
            goto label_10e76ec;
        }
    }
label_10e76e8:
    // 0x10e76e8: 0x202d  daddu       $a0, $zero, $zero
    RC_SET_U64(rc_4, (uint64_t)RC_U64(0) + (uint64_t)RC_U64(0));
    Ps2SetGpr64(ctx, 4, RC_U64X(rc_4)); 
label_10e76ec:
    // 0x10e76ec: 0x1080000c  beqz        $a0, . + 4 + (0xC << 2)
    {
        const bool branch_taken_0x10e76ec = (RC_U64X(rc_4) == RC_U64(0));
        // 0x10e76f0: 0x3c02001e  lui         $v0, 0x1E (Delay Slot)
        { const uint32_t _dynInsn = READ32(0x10E76F0u); const int32_t _dynSimm = (int16_t)(_dynInsn & 0xFFFFu); const uint32_t _dynImm = _dynInsn & 0xFFFFu; (void)_dynSimm; (void)_dynImm; RC_SET_S32(rc_2, (int32_t)((uint32_t)_dynImm << 16)); }
        Ps2SetGpr64(ctx, 2, RC_U64X(rc_2)); 
        if (branch_taken_0x10e76ec) {
            goto label_10e7720;
        }
    }
    // 0x10e76f4: 0x8c430ed4  lw          $v1, 0xED4($v0)
    { const uint32_t _dynInsn = READ32(0x10E76F4u); const int32_t _dynSimm = (int16_t)(_dynInsn & 0xFFFFu); const uint32_t _dynImm = _dynInsn & 0xFFFFu; (void)_dynSimm; (void)_dynImm; RC_SET_S32(rc_3, (int32_t)READ32(ADD32(RC_U32(rc_2), _dynSimm))); }
    Ps2SetGpr64(ctx, 3, RC_U64X(rc_3)); 
    // 0x10e76f8: 0x10830009  beq         $a0, $v1, . + 4 + (0x9 << 2)
    {
        const bool branch_taken_0x10e76f8 = (RC_U64X(rc_4) == RC_U64X(rc_3));
        // 0x10e76fc: 0x61482  srl         $v0, $a2, 18 (Delay Slot)
        RC_SET_S32(rc_2, (int32_t)SRL32(RC_U32(rc_6), 18));
        Ps2SetGpr64(ctx, 2, RC_U64X(rc_2)); 
        if (branch_taken_0x10e76f8) {
            goto label_10e7720;
        }
    }
    // 0x10e7700: 0x8c850008  lw          $a1, 0x8($a0)
    RC_SET_S32(rc_5, (int32_t)READ32(ADD32(RC_U32(rc_4), 8)));
    Ps2SetGpr64(ctx, 5, RC_U64X(rc_5)); 
    // 0x10e7704: 0x30420fff  andi        $v0, $v0, 0xFFF
    RC_SET_U64(rc_2, RC_U64X(rc_2) & (uint64_t)(uint16_t)4095);
    // 0x10e7708: 0x8c840010  lw          $a0, 0x10($a0)
    RC_SET_S32(rc_4, (int32_t)READ32(ADD32(RC_U32(rc_4), 16)));
    // 0x10e770c: 0x8ca3000c  lw          $v1, 0xC($a1)
    RC_SET_S32(rc_3, (int32_t)READ32(ADD32(RC_U32(rc_5), 12)));
    Ps2SetGpr64(ctx, 3, RC_U64X(rc_3)); 
    // 0x10e7710: 0x431018  mult        $v0, $v0, $v1
    { int64_t result = (int64_t)RC_S32(rc_2) * (int64_t)RC_S32(rc_3); ctx->lo = (uint64_t)(int64_t)(int32_t)result; ctx->hi = (uint64_t)(int64_t)(int32_t)(result >> 32); RC_SET_S32(rc_2, (int32_t)result); }
    Ps2SetGpr64(ctx, 2, RC_U64X(rc_2)); 
    // 0x10e7714: 0x822021  addu        $a0, $a0, $v0
    RC_SET_S32(rc_4, (int32_t)ADD32(RC_U32(rc_4), RC_U32(rc_2)));
    Ps2SetGpr64(ctx, 4, RC_U64X(rc_4)); 
    // 0x10e7718: 0x10000002  b           . + 4 + (0x2 << 2)
    {
        const bool branch_taken_0x10e7718 = (RC_U64(0) == RC_U64(0));
        // 0x10e771c: 0xafa401c8  sw          $a0, 0x1C8($sp) (Delay Slot)
        WRITE32(ADD32(RC_U32(rc_29), 456), RC_U32(rc_4));
        if (branch_taken_0x10e7718) {
            goto label_10e7724;
        }
    }
label_10e7720:
    // 0x10e7720: 0xafa001c8  sw          $zero, 0x1C8($sp)
    WRITE32(ADD32(RC_U32(rc_29), 456), RC_U32(0));
label_10e7724:
    // 0x10e7724: 0x27b00010  addiu       $s0, $sp, 0x10
    RC_SET_S32(rc_16, (int32_t)ADD32(RC_U32(rc_29), 16));
    Ps2SetGpr64(ctx, 16, RC_U64X(rc_16)); 
    // 0x10e7728: 0x282d  daddu       $a1, $zero, $zero
    RC_SET_U64(rc_5, (uint64_t)RC_U64(0) + (uint64_t)RC_U64(0));
    Ps2SetGpr64(ctx, 5, RC_U64X(rc_5)); 
    // 0x10e772c: 0x200202d  daddu       $a0, $s0, $zero
    RC_SET_U64(rc_4, (uint64_t)RC_U64X(rc_16) + (uint64_t)RC_U64(0));
    Ps2SetGpr64(ctx, 4, RC_U64X(rc_4)); 
    // 0x10e7730: 0xc04a0f4  jal         func_1283D0
    {
        const uint32_t jumpTarget = 0x0u | ((READ32(0x10E7730u) & 0x3FFFFFFu) << 2);
        RC_SET_U32(rc_31, 0x10E7738u);
        Ps2SetGpr64(ctx, 31, RC_U64X(rc_31)); 
        // 0x10e7734: 0x24060010  addiu       $a2, $zero, 0x10 (Delay Slot)
        RC_SET_S32(rc_6, (int32_t)ADD32(RC_U32(0), 16));
        Ps2SetGpr64(ctx, 6, RC_U64X(rc_6)); 
        ctx->pc = jumpTarget;
        if (!PS2_CALL_GUEST_KNOWN(jumpTarget, memset_0x1283d0, 0x10E7738u, 0x10E7730u, IndirectCall, "JALR")) {
            return;
        }
    }
label_10e7738:
    // 0x10e7738: 0x3c013f80  lui         $at, 0x3F80
    RC_SET_S32(rc_1, (int32_t)((uint32_t)16256 << 16));
    Ps2SetGpr64(ctx, 1, RC_U64X(rc_1)); 
    // 0x10e773c: 0x44810000  mtc1        $at, $f0
    { uint32_t bits = RC_U32(rc_1); std::memcpy(&rf_0, &bits, sizeof(bits)); }
    ctx->f[0] = rf_0; 
    // 0x10e7740: 0xc6610004  lwc1        $f1, 0x4($s3)
    rc_19 = Ps2GprU64(ctx, 19); 
    { uint32_t bits = READ32(ADD32(RC_U32(rc_19), 4)); float f; std::memcpy(&f, &bits, sizeof(f)); rf_1 = f; }
    ctx->f[1] = rf_1; 
    // 0x10e7744: 0x4600a306  mov.s       $f12, $f20
    rf_20 = ctx->f[20]; 
    rf_12 = FPU_MOV_S(rf_20);
    ctx->f[12] = rf_12; 
    // 0x10e7748: 0x200282d  daddu       $a1, $s0, $zero
    rc_16 = Ps2GprU64(ctx, 16); 
    RC_SET_U64(rc_5, (uint64_t)RC_U64(rc_16) + (uint64_t)RC_U64(0));
    Ps2SetGpr64(ctx, 5, rc_5); 
    // 0x10e774c: 0x2e0202d  daddu       $a0, $s7, $zero
    rc_23 = Ps2GprU64(ctx, 23); 
    RC_SET_U64(rc_4, (uint64_t)RC_U64(rc_23) + (uint64_t)RC_U64(0));
    Ps2SetGpr64(ctx, 4, rc_4); 
    // 0x10e7750: 0x2e0302d  daddu       $a2, $s7, $zero
    RC_SET_U64(rc_6, (uint64_t)RC_U64(rc_23) + (uint64_t)RC_U64(0));
    Ps2SetGpr64(ctx, 6, rc_6); 
    // 0x10e7754: 0xe7a0001c  swc1        $f0, 0x1C($sp)
    rc_29 = Ps2GprU64(ctx, 29); 
    { float f = rf_0; uint32_t bits; std::memcpy(&bits, &f, sizeof(bits)); WRITE32(ADD32(RC_U32(rc_29), 28), bits); }
    // 0x10e7758: 0xc437076  jal         func_10DC1D8
    RC_SET_U32(rc_31, 0x10E7760u);
    Ps2SetGpr64(ctx, 31, RC_U64X(rc_31)); 
    // 0x10e775c: 0xe7a10014  swc1        $f1, 0x14($sp) (Delay Slot)
    { float f = rf_1; uint32_t bits; std::memcpy(&bits, &f, sizeof(bits)); WRITE32(ADD32(RC_U32(rc_29), 20), bits); }
    ctx->pc = 0x10DC1D8u;
    if (!PS2_CALL_GUEST_NOCHK(0x10DC1D8u, charrootdyn_getInterpVector_0x10dc1d8, 0x10E7760u, 0x10E7758u, DirectCall, "JAL")) {
        return;
    }
label_10e7760:
    // 0x10e7760: 0x3c05001e  lui         $a1, 0x1E
    { const uint32_t _dynInsn = READ32(0x10E7760u); const int32_t _dynSimm = (int16_t)(_dynInsn & 0xFFFFu); const uint32_t _dynImm = _dynInsn & 0xFFFFu; (void)_dynSimm; (void)_dynImm; RC_SET_S32(rc_5, (int32_t)((uint32_t)_dynImm << 16)); }
    // 0x10e7764: 0x2e0302d  daddu       $a2, $s7, $zero
    rc_23 = Ps2GprU64(ctx, 23); 
    RC_SET_U64(rc_6, (uint64_t)RC_U64(rc_23) + (uint64_t)RC_U64(0));
    Ps2SetGpr64(ctx, 6, rc_6); 
    // 0x10e7768: 0x24a50fd0  addiu       $a1, $a1, 0xFD0
    { const uint32_t _dynInsn = READ32(0x10E7768u); const int32_t _dynSimm = (int16_t)(_dynInsn & 0xFFFFu); const uint32_t _dynImm = _dynInsn & 0xFFFFu; (void)_dynSimm; (void)_dynImm; RC_SET_S32(rc_5, (int32_t)ADD32(RC_U32(rc_5), _dynSimm)); }
    Ps2SetGpr64(ctx, 5, RC_U64X(rc_5)); 
    // 0x10e776c: 0xc437c86  jal         func_10DF218
    RC_SET_U32(rc_31, 0x10E7774u);
    Ps2SetGpr64(ctx, 31, RC_U64X(rc_31)); 
    // 0x10e7770: 0x220202d  daddu       $a0, $s1, $zero (Delay Slot)
    rc_17 = Ps2GprU64(ctx, 17); 
    RC_SET_U64(rc_4, (uint64_t)RC_U64(rc_17) + (uint64_t)RC_U64(0));
    Ps2SetGpr64(ctx, 4, rc_4); 
    ctx->pc = 0x10DF218u;
    if (!PS2_CALL_GUEST_KNOWN(0x10DF218u, charrootdyn_updateCharacterClassParentGeometry_0x10df218, 0x10E7774u, 0x10E776Cu, DirectCall, "JAL")) {
        return;
    }
label_10e7774:
    // 0x10e7774: 0xc06f290  jal         func_1BCA40
    {
        const uint32_t jumpTarget = 0x0u | ((READ32(0x10E7774u) & 0x3FFFFFFu) << 2);
        RC_SET_U32(rc_31, 0x10E777Cu);
        Ps2SetGpr64(ctx, 31, RC_U64X(rc_31)); 
        // 0x10e7778: 0x2c0202d  daddu       $a0, $s6, $zero (Delay Slot)
        rc_22 = Ps2GprU64(ctx, 22); 
        RC_SET_U64(rc_4, (uint64_t)RC_U64(rc_22) + (uint64_t)RC_U64(0));
        Ps2SetGpr64(ctx, 4, rc_4); 
        ctx->pc = jumpTarget;
        if (!PS2_CALL_GUEST_KNOWN(jumpTarget, StabilizeToYRotQuaternion_0x1bca40, 0x10E777Cu, 0x10E7774u, IndirectCall, "JALR")) {
            return;
        }
    }
label_10e777c:
    // 0x10e777c: 0x220202d  daddu       $a0, $s1, $zero
    rc_17 = Ps2GprU64(ctx, 17); 
    RC_SET_U64(rc_4, (uint64_t)RC_U64(rc_17) + (uint64_t)RC_U64(0));
    Ps2SetGpr64(ctx, 4, rc_4); 
    // 0x10e7780: 0xc43c896  jal         func_10F2258
    RC_SET_U32(rc_31, 0x10E7788u);
    Ps2SetGpr64(ctx, 31, RC_U64X(rc_31)); 
    // 0x10e7784: 0x2c0282d  daddu       $a1, $s6, $zero (Delay Slot)
    rc_22 = Ps2GprU64(ctx, 22); 
    RC_SET_U64(rc_5, (uint64_t)RC_U64(rc_22) + (uint64_t)RC_U64(0));
    Ps2SetGpr64(ctx, 5, rc_5); 
    ctx->pc = 0x10F2258u;
    if (!PS2_CALL_GUEST_KNOWN(0x10F2258u, SetLayoutClassRootQuaternion_0x10f2258, 0x10E7788u, 0x10E7780u, DirectCall, "JAL")) {
        return;
    }
label_10e7788:
    // 0x10e7788: 0x12800005  beqz        $s4, . + 4 + (0x5 << 2)
    {
        rc_20 = Ps2GprU64(ctx, 20); 
        const bool branch_taken_0x10e7788 = (RC_U64(rc_20) == RC_U64(0));
        // 0x10e778c: 0x220202d  daddu       $a0, $s1, $zero (Delay Slot)
        rc_17 = Ps2GprU64(ctx, 17); 
        RC_SET_U64(rc_4, (uint64_t)RC_U64(rc_17) + (uint64_t)RC_U64(0));
        Ps2SetGpr64(ctx, 4, rc_4); 
        if (branch_taken_0x10e7788) {
            goto label_10e77a0;
        }
    }
    // 0x10e7790: 0xc62c03ec  lwc1        $f12, 0x3EC($s1)
    { uint32_t bits = READ32(ADD32(RC_U32(rc_17), 1004)); float f; std::memcpy(&f, &bits, sizeof(f)); rf_12 = f; }
    ctx->f[12] = rf_12; 
    // 0x10e7794: 0xc43737e  jal         func_10DCDF8
    RC_SET_U32(rc_31, 0x10E779Cu);
    Ps2SetGpr64(ctx, 31, RC_U64X(rc_31)); 
    // 0x10e7798: 0x4600bb46  mov.s       $f13, $f23 (Delay Slot)
    rf_23 = ctx->f[23]; 
    rf_13 = FPU_MOV_S(rf_23);
    ctx->f[13] = rf_13; 
    ctx->pc = 0x10DCDF8u;
    if (!PS2_CALL_GUEST_KNOWN(0x10DCDF8u, charrootdyn_addWindDynamicsToRoot_0x10dcdf8, 0x10E779Cu, 0x10E7794u, DirectCall, "JAL")) {
        return;
    }
label_10e779c:
    // 0x10e779c: 0xafa201c4  sw          $v0, 0x1C4($sp)
    rc_2 = Ps2GprU64(ctx, 2); rc_29 = Ps2GprU64(ctx, 29); 
    WRITE32(ADD32(RC_U32(rc_29), 452), RC_U32(rc_2));
label_10e77a0:
    // 0x10e77a0: 0x82230c96  lb          $v1, 0xC96($s1)
    rc_17 = Ps2GprU64(ctx, 17); 
    RC_SET_S32(rc_3, (int8_t)READ8(ADD32(RC_U32(rc_17), 3222)));
    Ps2SetGpr64(ctx, 3, RC_U64X(rc_3)); 
    // 0x10e77a4: 0x24020002  addiu       $v0, $zero, 0x2
    RC_SET_S32(rc_2, (int32_t)ADD32(RC_U32(0), 2));
    Ps2SetGpr64(ctx, 2, RC_U64X(rc_2)); 
    // 0x10e77a8: 0x10620039  beq         $v1, $v0, . + 4 + (0x39 << 2)
    {
        const bool branch_taken_0x10e77a8 = (RC_U64X(rc_3) == RC_U64X(rc_2));
        // 0x10e77ac: 0x8fa201a4  lw          $v0, 0x1A4($sp) (Delay Slot)
        rc_29 = Ps2GprU64(ctx, 29); 
        RC_SET_S32(rc_2, (int32_t)READ32(ADD32(RC_U32(rc_29), 420)));
        Ps2SetGpr64(ctx, 2, RC_U64X(rc_2)); 
        if (branch_taken_0x10e77a8) {
            goto label_10e7890;
        }
    }
    // 0x10e77b0: 0x27b00020  addiu       $s0, $sp, 0x20
    RC_SET_S32(rc_16, (int32_t)ADD32(RC_U32(rc_29), 32));
    Ps2SetGpr64(ctx, 16, RC_U64X(rc_16)); 
    // 0x10e77b4: 0x78430000  lq          $v1, 0x0($v0)
    RC_SET_VEC(ctx, 3, rc_3, READ128(ADD32(RC_U32(rc_2), 0)));
    // 0x10e77b8: 0x262206f0  addiu       $v0, $s1, 0x6F0
    RC_SET_S32(rc_2, (int32_t)ADD32(RC_U32(rc_17), 1776));
    // 0x10e77bc: 0x7e2306f0  sq          $v1, 0x6F0($s1)
    WRITE128(ADD32(RC_U32(rc_17), 1776), GPR_VEC(ctx, 3));
    // 0x10e77c0: 0xd8480000  lqc2        $vf8, 0x0($v0)
    ctx->vu0_vf[8] = _mm_castsi128_ps(READ128(ADD32(RC_U32(rc_2), 0)));
    // 0x10e77c4: 0x4bc00303  vaddw.xyz   $vf12, $vf0, $vf0w
    { __m128 res = PS2_VADD_K(1, 1, ctx->vu0_vf[0], _mm_shuffle_ps(ctx->vu0_vf[0], ctx->vu0_vf[0], _MM_SHUFFLE(3,3,3,3))); __m128i mask = _mm_set_epi32(0, -1, -1, -1); ctx->vu0_vf[12] = _mm_blendv_ps(ctx->vu0_vf[12], res, _mm_castsi128_ps(mask)); }
    // 0x10e77c8: 0xdacb0000  lqc2        $vf11, 0x0($s6)
    rc_22 = Ps2GprU64(ctx, 22); 
    ctx->vu0_vf[11] = _mm_castsi128_ps(READ128(ADD32(RC_U32(rc_22), 0)));
    // 0x10e77cc: 0x4a2e033d  vmr32.w     $vf14, $vf0
    { __m128 res = _mm_shuffle_ps(ctx->vu0_vf[0], ctx->vu0_vf[0], _MM_SHUFFLE(0,3,2,1)); __m128i mask = _mm_set_epi32(-1, 0, 0, 0); ctx->vu0_vf[14] = _mm_blendv_ps(ctx->vu0_vf[14], res, _mm_castsi128_ps(mask)); }
    // 0x10e77d0: 0x4a2f033d  vmr32.w     $vf15, $vf0
    { __m128 res = _mm_shuffle_ps(ctx->vu0_vf[0], ctx->vu0_vf[0], _MM_SHUFFLE(0,3,2,1)); __m128i mask = _mm_set_epi32(-1, 0, 0, 0); ctx->vu0_vf[15] = _mm_blendv_ps(ctx->vu0_vf[15], res, _mm_castsi128_ps(mask)); }
    // 0x10e77d4: 0x4a30033d  vmr32.w     $vf16, $vf0
    { __m128 res = _mm_shuffle_ps(ctx->vu0_vf[0], ctx->vu0_vf[0], _MM_SHUFFLE(0,3,2,1)); __m128i mask = _mm_set_epi32(-1, 0, 0, 0); ctx->vu0_vf[16] = _mm_blendv_ps(ctx->vu0_vf[16], res, _mm_castsi128_ps(mask)); }
    // 0x10e77d8: 0x4bcc5afe  vopmula.xyz $ACC, $vf11, $vf12
    { __m128 fs_yzx = _mm_shuffle_ps(ctx->vu0_vf[11], ctx->vu0_vf[11], _MM_SHUFFLE(3,0,2,1)); __m128 ft_zxy = _mm_shuffle_ps(ctx->vu0_vf[12], ctx->vu0_vf[12], _MM_SHUFFLE(3,1,0,2)); __m128 res = PS2_VMUL_K(0, 1, fs_yzx, ft_zxy); ctx->vu0_acc = _mm_blendv_ps(ctx->vu0_acc, res, _mm_castsi128_ps(_mm_set_epi32(0, -1, -1, -1))); }
    // 0x10e77dc: 0x4bc002e9  vmadd.xyz   $vf11, $vf0, $vf0
    { __m128 mul_res = PS2_VMUL_K(1, 1, ctx->vu0_vf[0], ctx->vu0_vf[0]); __m128 res = PS2_VADD_K(1, 1, ctx->vu0_acc, mul_res); __m128i mask = _mm_set_epi32(0, -1, -1, -1); ctx->vu0_vf[11] = _mm_blendv_ps(ctx->vu0_vf[11], res, _mm_castsi128_ps(mask)); }
    // 0x10e77e0: 0x4bcc5aa9  vmadd.xyz   $vf10, $vf11, $vf12
    { __m128 mul_res = PS2_VMUL_K(1, 1, ctx->vu0_vf[11], ctx->vu0_vf[12]); __m128 res = PS2_VADD_K(1, 1, ctx->vu0_acc, mul_res); __m128i mask = _mm_set_epi32(0, -1, -1, -1); ctx->vu0_vf[10] = _mm_blendv_ps(ctx->vu0_vf[10], res, _mm_castsi128_ps(mask)); }
    // 0x10e77e4: 0x4bca5b6a  vmul.xyz    $vf13, $vf11, $vf10
    { __m128 res = PS2_VMUL_K(1, 1, ctx->vu0_vf[11], ctx->vu0_vf[10]); __m128i mask = _mm_set_epi32(0, -1, -1, -1); ctx->vu0_vf[13] = PS2_VBLEND(ctx->vu0_vf[13], res, _mm_castsi128_ps(mask)); }
    // 0x10e77e8: 0x4bca5afe  vopmula.xyz $ACC, $vf11, $vf10
    { __m128 fs_yzx = _mm_shuffle_ps(ctx->vu0_vf[11], ctx->vu0_vf[11], _MM_SHUFFLE(3,0,2,1)); __m128 ft_zxy = _mm_shuffle_ps(ctx->vu0_vf[10], ctx->vu0_vf[10], _MM_SHUFFLE(3,1,0,2)); __m128 res = PS2_VMUL_K(1, 1, fs_yzx, ft_zxy); ctx->vu0_acc = _mm_blendv_ps(ctx->vu0_acc, res, _mm_castsi128_ps(_mm_set_epi32(0, -1, -1, -1))); }
    // 0x10e77ec: 0x4bcb53cb  vmaddw.xyz  $vf15, $vf10, $vf11w
    { __m128 mul_res = PS2_VMUL_K(1, 0, ctx->vu0_vf[10], _mm_shuffle_ps(ctx->vu0_vf[11], ctx->vu0_vf[11], _MM_SHUFFLE(3,3,3,3))); __m128 res = PS2_VADD_K(1, 1, ctx->vu0_acc, mul_res); __m128i mask = _mm_set_epi32(0, -1, -1, -1); ctx->vu0_vf[15] = _mm_blendv_ps(ctx->vu0_vf[15], res, _mm_castsi128_ps(mask)); }
    // 0x10e77f0: 0x4bcb540f  vmsubw.xyz  $vf16, $vf10, $vf11w
    { __m128 mul_res = PS2_VMUL_K(1, 0, ctx->vu0_vf[10], _mm_shuffle_ps(ctx->vu0_vf[11], ctx->vu0_vf[11], _MM_SHUFFLE(3,3,3,3))); __m128 res = PS2_VSUB_K(1, 1, ctx->vu0_acc, mul_res); __m128i mask = _mm_set_epi32(0, -1, -1, -1); ctx->vu0_vf[16] = _mm_blendv_ps(ctx->vu0_vf[16], res, _mm_castsi128_ps(mask)); }
    // 0x10e77f4: 0x4bcc6afe  vopmula.xyz $ACC, $vf13, $vf12
    { __m128 fs_yzx = _mm_shuffle_ps(ctx->vu0_vf[13], ctx->vu0_vf[13], _MM_SHUFFLE(3,0,2,1)); __m128 ft_zxy = _mm_shuffle_ps(ctx->vu0_vf[12], ctx->vu0_vf[12], _MM_SHUFFLE(3,1,0,2)); __m128 res = PS2_VMUL_K(1, 1, fs_yzx, ft_zxy); ctx->vu0_acc = _mm_blendv_ps(ctx->vu0_acc, res, _mm_castsi128_ps(_mm_set_epi32(0, -1, -1, -1))); }
    // 0x10e77f8: 0x4bcc6c69  vmadd.xyz   $vf17, $vf13, $vf12
    { __m128 mul_res = PS2_VMUL_K(1, 1, ctx->vu0_vf[13], ctx->vu0_vf[12]); __m128 res = PS2_VADD_K(1, 1, ctx->vu0_acc, mul_res); __m128i mask = _mm_set_epi32(0, -1, -1, -1); ctx->vu0_vf[17] = _mm_blendv_ps(ctx->vu0_vf[17], res, _mm_castsi128_ps(mask)); }
    // 0x10e77fc: 0x4bcc7afe  vopmula.xyz $ACC, $vf15, $vf12
    { __m128 fs_yzx = _mm_shuffle_ps(ctx->vu0_vf[15], ctx->vu0_vf[15], _MM_SHUFFLE(3,0,2,1)); __m128 ft_zxy = _mm_shuffle_ps(ctx->vu0_vf[12], ctx->vu0_vf[12], _MM_SHUFFLE(3,1,0,2)); __m128 res = PS2_VMUL_K(1, 1, fs_yzx, ft_zxy); ctx->vu0_acc = _mm_blendv_ps(ctx->vu0_acc, res, _mm_castsi128_ps(_mm_set_epi32(0, -1, -1, -1))); }
    // 0x10e7800: 0x4bc003e9  vmadd.xyz   $vf15, $vf0, $vf0
    { __m128 mul_res = PS2_VMUL_K(1, 1, ctx->vu0_vf[0], ctx->vu0_vf[0]); __m128 res = PS2_VADD_K(1, 1, ctx->vu0_acc, mul_res); __m128i mask = _mm_set_epi32(0, -1, -1, -1); ctx->vu0_vf[15] = _mm_blendv_ps(ctx->vu0_vf[15], res, _mm_castsi128_ps(mask)); }
    // 0x10e7804: 0x4bd163ac  vsub.xyz    $vf14, $vf12, $vf17
    { __m128 res = PS2_VSUB_K(1, 1, ctx->vu0_vf[12], ctx->vu0_vf[17]); __m128i mask = _mm_set_epi32(0, -1, -1, -1); ctx->vu0_vf[14] = PS2_VBLEND(ctx->vu0_vf[14], res, _mm_castsi128_ps(mask)); }
    // 0x10e7808: 0x4ad1733c  vmove.yz    $vf17, $vf14
    { __m128i mask = _mm_set_epi32(0, -1, -1, 0); ctx->vu0_vf[17] = _mm_blendv_ps(ctx->vu0_vf[17], ctx->vu0_vf[14], _mm_castsi128_ps(mask)); }
    // 0x10e780c: 0x4a8e833c  vmove.y     $vf14, $vf16
    { __m128i mask = _mm_set_epi32(0, 0, -1, 0); ctx->vu0_vf[14] = _mm_blendv_ps(ctx->vu0_vf[14], ctx->vu0_vf[16], _mm_castsi128_ps(mask)); }
    // 0x10e7810: 0x4a907b3c  vmove.y     $vf16, $vf15
    { __m128i mask = _mm_set_epi32(0, 0, -1, 0); ctx->vu0_vf[16] = _mm_blendv_ps(ctx->vu0_vf[16], ctx->vu0_vf[15], _mm_castsi128_ps(mask)); }
    // 0x10e7814: 0x4a8f8b3c  vmove.y     $vf15, $vf17
    { __m128i mask = _mm_set_epi32(0, 0, -1, 0); ctx->vu0_vf[15] = _mm_blendv_ps(ctx->vu0_vf[15], ctx->vu0_vf[17], _mm_castsi128_ps(mask)); }
    // 0x10e7818: 0x4a4e7b3c  vmove.z     $vf14, $vf15
    { __m128i mask = _mm_set_epi32(0, -1, 0, 0); ctx->vu0_vf[14] = _mm_blendv_ps(ctx->vu0_vf[14], ctx->vu0_vf[15], _mm_castsi128_ps(mask)); }
    // 0x10e781c: 0x4a4f833c  vmove.z     $vf15, $vf16
    { __m128i mask = _mm_set_epi32(0, -1, 0, 0); ctx->vu0_vf[15] = _mm_blendv_ps(ctx->vu0_vf[15], ctx->vu0_vf[16], _mm_castsi128_ps(mask)); }
    // 0x10e7820: 0x4a508b3c  vmove.z     $vf16, $vf17
    { __m128i mask = _mm_set_epi32(0, -1, 0, 0); ctx->vu0_vf[16] = _mm_blendv_ps(ctx->vu0_vf[16], ctx->vu0_vf[17], _mm_castsi128_ps(mask)); }
    // 0x10e7824: 0x4be871bc  vmulax.xyzw $ACC, $vf14, $vf8x
    { __m128 res = PS2_VMUL_K(1, 0, ctx->vu0_vf[14], _mm_shuffle_ps(ctx->vu0_vf[8], ctx->vu0_vf[8], _MM_SHUFFLE(0,0,0,0))); ctx->vu0_acc = _mm_blendv_ps(ctx->vu0_acc, res, _mm_castsi128_ps(_mm_set_epi32(-1, -1, -1, -1))); }
    // 0x10e7828: 0x4be878bd  vmadday.xyzw $ACC, $vf15, $vf8y
    { __m128 mul_res = PS2_VMUL_K(1, 0, ctx->vu0_vf[15], _mm_shuffle_ps(ctx->vu0_vf[8], ctx->vu0_vf[8], _MM_SHUFFLE(1,1,1,1))); __m128 res = PS2_VADD_K(1, 1, ctx->vu0_acc, mul_res); ctx->vu0_acc = _mm_blendv_ps(ctx->vu0_acc, res, _mm_castsi128_ps(_mm_set_epi32(-1, -1, -1, -1))); }
    // 0x10e782c: 0x4be880be  vmaddaz.xyzw $ACC, $vf16, $vf8z
    { __m128 mul_res = PS2_VMUL_K(1, 0, ctx->vu0_vf[16], _mm_shuffle_ps(ctx->vu0_vf[8], ctx->vu0_vf[8], _MM_SHUFFLE(2,2,2,2))); __m128 res = PS2_VADD_K(1, 1, ctx->vu0_acc, mul_res); ctx->vu0_acc = _mm_blendv_ps(ctx->vu0_acc, res, _mm_castsi128_ps(_mm_set_epi32(-1, -1, -1, -1))); }
    // 0x10e7830: 0x4be8028b  vmaddw.xyzw $vf10, $vf0, $vf8w
    { __m128 mul_res = PS2_VMUL_K(1, 0, ctx->vu0_vf[0], _mm_shuffle_ps(ctx->vu0_vf[8], ctx->vu0_vf[8], _MM_SHUFFLE(3,3,3,3))); __m128 res = PS2_VADD_K(1, 1, ctx->vu0_acc, mul_res); __m128i mask = _mm_set_epi32(-1, -1, -1, -1); ctx->vu0_vf[10] = _mm_blendv_ps(ctx->vu0_vf[10], res, _mm_castsi128_ps(mask)); }
    // 0x10e7834: 0xfa0a0000  sqc2        $vf10, 0x0($s0)
    WRITE128(ADD32(RC_U32(rc_16), 0), _mm_castps_si128(ctx->vu0_vf[10]));
    // 0x10e7838: 0x8e220124  lw          $v0, 0x124($s1)
    RC_SET_S32(rc_2, (int32_t)READ32(ADD32(RC_U32(rc_17), 292)));
    Ps2SetGpr64(ctx, 2, RC_U64X(rc_2)); 
    // 0x10e783c: 0x1040000b  beqz        $v0, . + 4 + (0xB << 2)
    {
        const bool branch_taken_0x10e783c = (RC_U64X(rc_2) == RC_U64(0));
        // 0x10e7840: 0x8fa301b4  lw          $v1, 0x1B4($sp) (Delay Slot)
        RC_SET_S32(rc_3, (int32_t)READ32(ADD32(RC_U32(rc_29), 436)));
        Ps2SetGpr64(ctx, 3, RC_U64X(rc_3)); 
        if (branch_taken_0x10e783c) {
            goto label_10e786c;
        }
    }
    // 0x10e7844: 0x8c620018  lw          $v0, 0x18($v1)
    RC_SET_S32(rc_2, (int32_t)READ32(ADD32(RC_U32(rc_3), 24)));
    // 0x10e7848: 0x30420001  andi        $v0, $v0, 0x1
    RC_SET_U64(rc_2, RC_U64X(rc_2) & (uint64_t)(uint16_t)1);
    Ps2SetGpr64(ctx, 2, RC_U64X(rc_2)); 
    // 0x10e784c: 0x10400007  beqz        $v0, . + 4 + (0x7 << 2)
    {
        const bool branch_taken_0x10e784c = (RC_U64X(rc_2) == RC_U64(0));
        // 0x10e7850: 0x8fa401ac  lw          $a0, 0x1AC($sp) (Delay Slot)
        RC_SET_S32(rc_4, (int32_t)READ32(ADD32(RC_U32(rc_29), 428)));
        Ps2SetGpr64(ctx, 4, RC_U64X(rc_4)); 
        if (branch_taken_0x10e784c) {
            goto label_10e786c;
        }
    }
    // 0x10e7854: 0xc44ca14  jal         func_1132850
    RC_SET_U32(rc_31, 0x10E785Cu);
    Ps2SetGpr64(ctx, 31, RC_U64X(rc_31)); 
    // 0x10e7858: 0xafa00024  sw          $zero, 0x24($sp) (Delay Slot)
    WRITE32(ADD32(RC_U32(rc_29), 36), RC_U32(0));
    ctx->pc = 0x1132850u;
    if (!PS2_CALL_GUEST_KNOWN(0x1132850u, GetGeometryPointerSurfacePlane_0x1132850, 0x10E785Cu, 0x10E7854u, DirectCall, "JAL")) {
        return;
    }
label_10e785c:
    // 0x10e785c: 0x200202d  daddu       $a0, $s0, $zero
    rc_16 = Ps2GprU64(ctx, 16); 
    RC_SET_U64(rc_4, (uint64_t)RC_U64(rc_16) + (uint64_t)RC_U64(0));
    Ps2SetGpr64(ctx, 4, rc_4); 
    // 0x10e7860: 0x40282d  daddu       $a1, $v0, $zero
    rc_2 = Ps2GprU64(ctx, 2); 
    RC_SET_U64(rc_5, (uint64_t)RC_U64(rc_2) + (uint64_t)RC_U64(0));
    Ps2SetGpr64(ctx, 5, rc_5); 
    // 0x10e7864: 0xc4373c4  jal         func_10DCF10
    RC_SET_U32(rc_31, 0x10E786Cu);
    Ps2SetGpr64(ctx, 31, RC_U64X(rc_31)); 
    // 0x10e7868: 0x200302d  daddu       $a2, $s0, $zero (Delay Slot)
    RC_SET_U64(rc_6, (uint64_t)RC_U64(rc_16) + (uint64_t)RC_U64(0));
    Ps2SetGpr64(ctx, 6, rc_6); 
    ctx->pc = 0x10DCF10u;
    if (!PS2_CALL_GUEST_KNOWN(0x10DCF10u, charrootdyn_getPlaneYFittingVector_0x10dcf10, 0x10E786Cu, 0x10E7864u, DirectCall, "JAL")) {
        return;
    }
label_10e786c:
    // 0x10e786c: 0x4404b800  mfc1        $a0, $f23
    rf_23 = ctx->f[23]; 
    { uint32_t bits; std::memcpy(&bits, &rf_23, sizeof(bits)); RC_SET_U32(rc_4, bits); }
    Ps2SetGpr64(ctx, 4, RC_U64X(rc_4)); 
    // 0x10e7870: 0xda050000  lqc2        $vf5, 0x0($s0)
    rc_16 = Ps2GprU64(ctx, 16); 
    ctx->vu0_vf[5] = _mm_castsi128_ps(READ128(ADD32(RC_U32(rc_16), 0)));
    // 0x10e7874: 0x48a43000  qmtc2.ni    $a0, $vf6
    ctx->vu0_vf[6] = _mm_castsi128_ps(GPR_VEC(ctx, 4));
    // 0x10e7878: 0x4be62958  vmulx.xyzw  $vf5, $vf5, $vf6x
    { __m128 res = PS2_VMUL(ctx->vu0_vf[5], _mm_shuffle_ps(ctx->vu0_vf[6], ctx->vu0_vf[6], _MM_SHUFFLE(0,0,0,0))); __m128i mask = _mm_set_epi32(-1, -1, -1, -1); ctx->vu0_vf[5] = _mm_blendv_ps(ctx->vu0_vf[5], res, _mm_castsi128_ps(mask)); }
    // 0x10e787c: 0xfa050000  sqc2        $vf5, 0x0($s0)
    WRITE128(ADD32(RC_U32(rc_16), 0), _mm_castps_si128(ctx->vu0_vf[5]));
    // 0x10e7880: 0xda450000  lqc2        $vf5, 0x0($s2)
    rc_18 = Ps2GprU64(ctx, 18); 
    ctx->vu0_vf[5] = _mm_castsi128_ps(READ128(ADD32(RC_U32(rc_18), 0)));
    // 0x10e7884: 0xda060000  lqc2        $vf6, 0x0($s0)
    ctx->vu0_vf[6] = _mm_castsi128_ps(READ128(ADD32(RC_U32(rc_16), 0)));
    // 0x10e7888: 0x4bc62968  vadd.xyz    $vf5, $vf5, $vf6
    { __m128 res = PS2_VADD(ctx->vu0_vf[5], ctx->vu0_vf[6]); __m128i mask = _mm_set_epi32(0, -1, -1, -1); ctx->vu0_vf[5] = PS2_VBLEND(ctx->vu0_vf[5], res, _mm_castsi128_ps(mask)); }
    // 0x10e788c: 0xfa450000  sqc2        $vf5, 0x0($s2)
    WRITE128(ADD32(RC_U32(rc_18), 0), _mm_castps_si128(ctx->vu0_vf[5]));
label_10e7890:
    // 0x10e7890: 0x8fa201b4  lw          $v0, 0x1B4($sp)
    rc_29 = Ps2GprU64(ctx, 29); 
    RC_SET_S32(rc_2, (int32_t)READ32(ADD32(RC_U32(rc_29), 436)));
    Ps2SetGpr64(ctx, 2, RC_U64X(rc_2)); 
    // 0x10e7894: 0x26240ab0  addiu       $a0, $s1, 0xAB0
    rc_17 = Ps2GprU64(ctx, 17); 
    RC_SET_S32(rc_4, (int32_t)ADD32(RC_U32(rc_17), 2736));
    Ps2SetGpr64(ctx, 4, RC_U64X(rc_4)); 
    // 0x10e7898: 0x3c013f33  lui         $at, 0x3F33
    RC_SET_S32(rc_1, (int32_t)((uint32_t)16179 << 16));
    // 0x10e789c: 0x34213333  ori         $at, $at, 0x3333
    RC_SET_U64(rc_1, RC_U64X(rc_1) | (uint64_t)(uint16_t)13107);
    Ps2SetGpr64(ctx, 1, RC_U64X(rc_1)); 
    // 0x10e78a0: 0x44817000  mtc1        $at, $f14
    { uint32_t bits = RC_U32(rc_1); std::memcpy(&rf_14, &bits, sizeof(bits)); }
    ctx->f[14] = rf_14; 
    // 0x10e78a4: 0x240282d  daddu       $a1, $s2, $zero
    rc_18 = Ps2GprU64(ctx, 18); 
    RC_SET_U64(rc_5, (uint64_t)RC_U64(rc_18) + (uint64_t)RC_U64(0));
    Ps2SetGpr64(ctx, 5, rc_5); 
    // 0x10e78a8: 0xc44c0084  lwc1        $f12, 0x84($v0)
    { uint32_t bits = READ32(ADD32(RC_U32(rc_2), 132)); float f; std::memcpy(&f, &bits, sizeof(f)); rf_12 = f; }
    ctx->f[12] = rf_12; 
    // 0x10e78ac: 0x80302d  daddu       $a2, $a0, $zero
    RC_SET_U64(rc_6, (uint64_t)RC_U64X(rc_4) + (uint64_t)RC_U64(0));
    Ps2SetGpr64(ctx, 6, RC_U64X(rc_6)); 
    // 0x10e78b0: 0xc44d0088  lwc1        $f13, 0x88($v0)
    { uint32_t bits = READ32(ADD32(RC_U32(rc_2), 136)); float f; std::memcpy(&f, &bits, sizeof(f)); rf_13 = f; }
    ctx->f[13] = rf_13; 
    // 0x10e78b4: 0x240382d  daddu       $a3, $s2, $zero
    RC_SET_U64(rc_7, (uint64_t)RC_U64(rc_18) + (uint64_t)RC_U64(0));
    Ps2SetGpr64(ctx, 7, rc_7); 
    // 0x10e78b8: 0x220402d  daddu       $t0, $s1, $zero
    RC_SET_U64(rc_8, (uint64_t)RC_U64(rc_17) + (uint64_t)RC_U64(0));
    Ps2SetGpr64(ctx, 8, rc_8); 
    // 0x10e78bc: 0xc4372c4  jal         func_10DCB10
    RC_SET_U32(rc_31, 0x10E78C4u);
    Ps2SetGpr64(ctx, 31, RC_U64X(rc_31)); 
    // 0x10e78c0: 0x4600bbc6  mov.s       $f15, $f23 (Delay Slot)
    rf_23 = ctx->f[23]; 
    rf_15 = FPU_MOV_S(rf_23);
    ctx->f[15] = rf_15; 
    ctx->pc = 0x10DCB10u;
    if (!PS2_CALL_GUEST_KNOWN(0x10DCB10u, charrootdyn_addSlideElement_0x10dcb10, 0x10E78C4u, 0x10E78BCu, DirectCall, "JAL")) {
        return;
    }
label_10e78c4:
    // 0x10e78c4: 0xafa201c0  sw          $v0, 0x1C0($sp)
    rc_2 = Ps2GprU64(ctx, 2); rc_29 = Ps2GprU64(ctx, 29); 
    WRITE32(ADD32(RC_U32(rc_29), 448), RC_U32(rc_2));
    // 0x10e78c8: 0x26220e00  addiu       $v0, $s1, 0xE00
    rc_17 = Ps2GprU64(ctx, 17); 
    RC_SET_S32(rc_2, (int32_t)ADD32(RC_U32(rc_17), 3584));
    Ps2SetGpr64(ctx, 2, RC_U64X(rc_2)); 
    // 0x10e78cc: 0x3a0182d  daddu       $v1, $sp, $zero
    RC_SET_U64(rc_3, (uint64_t)RC_U64(rc_29) + (uint64_t)RC_U64(0));
    Ps2SetGpr64(ctx, 3, rc_3); 
    // 0x10e78d0: 0xd8480000  lqc2        $vf8, 0x0($v0)
    ctx->vu0_vf[8] = _mm_castsi128_ps(READ128(ADD32(RC_U32(rc_2), 0)));
    // 0x10e78d4: 0x4bc00303  vaddw.xyz   $vf12, $vf0, $vf0w
    { __m128 res = PS2_VADD_K(1, 1, ctx->vu0_vf[0], _mm_shuffle_ps(ctx->vu0_vf[0], ctx->vu0_vf[0], _MM_SHUFFLE(3,3,3,3))); __m128i mask = _mm_set_epi32(0, -1, -1, -1); ctx->vu0_vf[12] = _mm_blendv_ps(ctx->vu0_vf[12], res, _mm_castsi128_ps(mask)); }
    // 0x10e78d8: 0xdacb0000  lqc2        $vf11, 0x0($s6)
    rc_22 = Ps2GprU64(ctx, 22); 
    ctx->vu0_vf[11] = _mm_castsi128_ps(READ128(ADD32(RC_U32(rc_22), 0)));
    // 0x10e78dc: 0x4a2e033d  vmr32.w     $vf14, $vf0
    { __m128 res = _mm_shuffle_ps(ctx->vu0_vf[0], ctx->vu0_vf[0], _MM_SHUFFLE(0,3,2,1)); __m128i mask = _mm_set_epi32(-1, 0, 0, 0); ctx->vu0_vf[14] = _mm_blendv_ps(ctx->vu0_vf[14], res, _mm_castsi128_ps(mask)); }
    // 0x10e78e0: 0x4a2f033d  vmr32.w     $vf15, $vf0
    { __m128 res = _mm_shuffle_ps(ctx->vu0_vf[0], ctx->vu0_vf[0], _MM_SHUFFLE(0,3,2,1)); __m128i mask = _mm_set_epi32(-1, 0, 0, 0); ctx->vu0_vf[15] = _mm_blendv_ps(ctx->vu0_vf[15], res, _mm_castsi128_ps(mask)); }
    // 0x10e78e4: 0x4a30033d  vmr32.w     $vf16, $vf0
    { __m128 res = _mm_shuffle_ps(ctx->vu0_vf[0], ctx->vu0_vf[0], _MM_SHUFFLE(0,3,2,1)); __m128i mask = _mm_set_epi32(-1, 0, 0, 0); ctx->vu0_vf[16] = _mm_blendv_ps(ctx->vu0_vf[16], res, _mm_castsi128_ps(mask)); }
    // 0x10e78e8: 0x4bcc5afe  vopmula.xyz $ACC, $vf11, $vf12
    { __m128 fs_yzx = _mm_shuffle_ps(ctx->vu0_vf[11], ctx->vu0_vf[11], _MM_SHUFFLE(3,0,2,1)); __m128 ft_zxy = _mm_shuffle_ps(ctx->vu0_vf[12], ctx->vu0_vf[12], _MM_SHUFFLE(3,1,0,2)); __m128 res = PS2_VMUL_K(0, 1, fs_yzx, ft_zxy); ctx->vu0_acc = _mm_blendv_ps(ctx->vu0_acc, res, _mm_castsi128_ps(_mm_set_epi32(0, -1, -1, -1))); }
    // 0x10e78ec: 0x4bc002e9  vmadd.xyz   $vf11, $vf0, $vf0
    { __m128 mul_res = PS2_VMUL_K(1, 1, ctx->vu0_vf[0], ctx->vu0_vf[0]); __m128 res = PS2_VADD_K(1, 1, ctx->vu0_acc, mul_res); __m128i mask = _mm_set_epi32(0, -1, -1, -1); ctx->vu0_vf[11] = _mm_blendv_ps(ctx->vu0_vf[11], res, _mm_castsi128_ps(mask)); }
    // 0x10e78f0: 0x4bcc5aa9  vmadd.xyz   $vf10, $vf11, $vf12
    { __m128 mul_res = PS2_VMUL_K(1, 1, ctx->vu0_vf[11], ctx->vu0_vf[12]); __m128 res = PS2_VADD_K(1, 1, ctx->vu0_acc, mul_res); __m128i mask = _mm_set_epi32(0, -1, -1, -1); ctx->vu0_vf[10] = _mm_blendv_ps(ctx->vu0_vf[10], res, _mm_castsi128_ps(mask)); }
    // 0x10e78f4: 0x4bca5b6a  vmul.xyz    $vf13, $vf11, $vf10
    { __m128 res = PS2_VMUL_K(1, 1, ctx->vu0_vf[11], ctx->vu0_vf[10]); __m128i mask = _mm_set_epi32(0, -1, -1, -1); ctx->vu0_vf[13] = PS2_VBLEND(ctx->vu0_vf[13], res, _mm_castsi128_ps(mask)); }
    // 0x10e78f8: 0x4bca5afe  vopmula.xyz $ACC, $vf11, $vf10
    { __m128 fs_yzx = _mm_shuffle_ps(ctx->vu0_vf[11], ctx->vu0_vf[11], _MM_SHUFFLE(3,0,2,1)); __m128 ft_zxy = _mm_shuffle_ps(ctx->vu0_vf[10], ctx->vu0_vf[10], _MM_SHUFFLE(3,1,0,2)); __m128 res = PS2_VMUL_K(1, 1, fs_yzx, ft_zxy); ctx->vu0_acc = _mm_blendv_ps(ctx->vu0_acc, res, _mm_castsi128_ps(_mm_set_epi32(0, -1, -1, -1))); }
    // 0x10e78fc: 0x4bcb53cb  vmaddw.xyz  $vf15, $vf10, $vf11w
    { __m128 mul_res = PS2_VMUL_K(1, 0, ctx->vu0_vf[10], _mm_shuffle_ps(ctx->vu0_vf[11], ctx->vu0_vf[11], _MM_SHUFFLE(3,3,3,3))); __m128 res = PS2_VADD_K(1, 1, ctx->vu0_acc, mul_res); __m128i mask = _mm_set_epi32(0, -1, -1, -1); ctx->vu0_vf[15] = _mm_blendv_ps(ctx->vu0_vf[15], res, _mm_castsi128_ps(mask)); }
    // 0x10e7900: 0x4bcb540f  vmsubw.xyz  $vf16, $vf10, $vf11w
    { __m128 mul_res = PS2_VMUL_K(1, 0, ctx->vu0_vf[10], _mm_shuffle_ps(ctx->vu0_vf[11], ctx->vu0_vf[11], _MM_SHUFFLE(3,3,3,3))); __m128 res = PS2_VSUB_K(1, 1, ctx->vu0_acc, mul_res); __m128i mask = _mm_set_epi32(0, -1, -1, -1); ctx->vu0_vf[16] = _mm_blendv_ps(ctx->vu0_vf[16], res, _mm_castsi128_ps(mask)); }
    // 0x10e7904: 0x4bcc6afe  vopmula.xyz $ACC, $vf13, $vf12
    { __m128 fs_yzx = _mm_shuffle_ps(ctx->vu0_vf[13], ctx->vu0_vf[13], _MM_SHUFFLE(3,0,2,1)); __m128 ft_zxy = _mm_shuffle_ps(ctx->vu0_vf[12], ctx->vu0_vf[12], _MM_SHUFFLE(3,1,0,2)); __m128 res = PS2_VMUL_K(1, 1, fs_yzx, ft_zxy); ctx->vu0_acc = _mm_blendv_ps(ctx->vu0_acc, res, _mm_castsi128_ps(_mm_set_epi32(0, -1, -1, -1))); }
    // 0x10e7908: 0x4bcc6c69  vmadd.xyz   $vf17, $vf13, $vf12
    { __m128 mul_res = PS2_VMUL_K(1, 1, ctx->vu0_vf[13], ctx->vu0_vf[12]); __m128 res = PS2_VADD_K(1, 1, ctx->vu0_acc, mul_res); __m128i mask = _mm_set_epi32(0, -1, -1, -1); ctx->vu0_vf[17] = _mm_blendv_ps(ctx->vu0_vf[17], res, _mm_castsi128_ps(mask)); }
    // 0x10e790c: 0x4bcc7afe  vopmula.xyz $ACC, $vf15, $vf12
    { __m128 fs_yzx = _mm_shuffle_ps(ctx->vu0_vf[15], ctx->vu0_vf[15], _MM_SHUFFLE(3,0,2,1)); __m128 ft_zxy = _mm_shuffle_ps(ctx->vu0_vf[12], ctx->vu0_vf[12], _MM_SHUFFLE(3,1,0,2)); __m128 res = PS2_VMUL_K(1, 1, fs_yzx, ft_zxy); ctx->vu0_acc = _mm_blendv_ps(ctx->vu0_acc, res, _mm_castsi128_ps(_mm_set_epi32(0, -1, -1, -1))); }
    // 0x10e7910: 0x4bc003e9  vmadd.xyz   $vf15, $vf0, $vf0
    { __m128 mul_res = PS2_VMUL_K(1, 1, ctx->vu0_vf[0], ctx->vu0_vf[0]); __m128 res = PS2_VADD_K(1, 1, ctx->vu0_acc, mul_res); __m128i mask = _mm_set_epi32(0, -1, -1, -1); ctx->vu0_vf[15] = _mm_blendv_ps(ctx->vu0_vf[15], res, _mm_castsi128_ps(mask)); }
    // 0x10e7914: 0x4bd163ac  vsub.xyz    $vf14, $vf12, $vf17
    { __m128 res = PS2_VSUB_K(1, 1, ctx->vu0_vf[12], ctx->vu0_vf[17]); __m128i mask = _mm_set_epi32(0, -1, -1, -1); ctx->vu0_vf[14] = PS2_VBLEND(ctx->vu0_vf[14], res, _mm_castsi128_ps(mask)); }
    // 0x10e7918: 0x4ad1733c  vmove.yz    $vf17, $vf14
    { __m128i mask = _mm_set_epi32(0, -1, -1, 0); ctx->vu0_vf[17] = _mm_blendv_ps(ctx->vu0_vf[17], ctx->vu0_vf[14], _mm_castsi128_ps(mask)); }
    // 0x10e791c: 0x4a8e833c  vmove.y     $vf14, $vf16
    { __m128i mask = _mm_set_epi32(0, 0, -1, 0); ctx->vu0_vf[14] = _mm_blendv_ps(ctx->vu0_vf[14], ctx->vu0_vf[16], _mm_castsi128_ps(mask)); }
    // 0x10e7920: 0x4a907b3c  vmove.y     $vf16, $vf15
    { __m128i mask = _mm_set_epi32(0, 0, -1, 0); ctx->vu0_vf[16] = _mm_blendv_ps(ctx->vu0_vf[16], ctx->vu0_vf[15], _mm_castsi128_ps(mask)); }
    // 0x10e7924: 0x4a8f8b3c  vmove.y     $vf15, $vf17
    { __m128i mask = _mm_set_epi32(0, 0, -1, 0); ctx->vu0_vf[15] = _mm_blendv_ps(ctx->vu0_vf[15], ctx->vu0_vf[17], _mm_castsi128_ps(mask)); }
    // 0x10e7928: 0x4a4e7b3c  vmove.z     $vf14, $vf15
    { __m128i mask = _mm_set_epi32(0, -1, 0, 0); ctx->vu0_vf[14] = _mm_blendv_ps(ctx->vu0_vf[14], ctx->vu0_vf[15], _mm_castsi128_ps(mask)); }
    // 0x10e792c: 0x4a4f833c  vmove.z     $vf15, $vf16
    { __m128i mask = _mm_set_epi32(0, -1, 0, 0); ctx->vu0_vf[15] = _mm_blendv_ps(ctx->vu0_vf[15], ctx->vu0_vf[16], _mm_castsi128_ps(mask)); }
    // 0x10e7930: 0x4a508b3c  vmove.z     $vf16, $vf17
    { __m128i mask = _mm_set_epi32(0, -1, 0, 0); ctx->vu0_vf[16] = _mm_blendv_ps(ctx->vu0_vf[16], ctx->vu0_vf[17], _mm_castsi128_ps(mask)); }
    // 0x10e7934: 0x4be871bc  vmulax.xyzw $ACC, $vf14, $vf8x
    { __m128 res = PS2_VMUL_K(1, 0, ctx->vu0_vf[14], _mm_shuffle_ps(ctx->vu0_vf[8], ctx->vu0_vf[8], _MM_SHUFFLE(0,0,0,0))); ctx->vu0_acc = _mm_blendv_ps(ctx->vu0_acc, res, _mm_castsi128_ps(_mm_set_epi32(-1, -1, -1, -1))); }
    // 0x10e7938: 0x4be878bd  vmadday.xyzw $ACC, $vf15, $vf8y
    { __m128 mul_res = PS2_VMUL_K(1, 0, ctx->vu0_vf[15], _mm_shuffle_ps(ctx->vu0_vf[8], ctx->vu0_vf[8], _MM_SHUFFLE(1,1,1,1))); __m128 res = PS2_VADD_K(1, 1, ctx->vu0_acc, mul_res); ctx->vu0_acc = _mm_blendv_ps(ctx->vu0_acc, res, _mm_castsi128_ps(_mm_set_epi32(-1, -1, -1, -1))); }
    // 0x10e793c: 0x4be880be  vmaddaz.xyzw $ACC, $vf16, $vf8z
    { __m128 mul_res = PS2_VMUL_K(1, 0, ctx->vu0_vf[16], _mm_shuffle_ps(ctx->vu0_vf[8], ctx->vu0_vf[8], _MM_SHUFFLE(2,2,2,2))); __m128 res = PS2_VADD_K(1, 1, ctx->vu0_acc, mul_res); ctx->vu0_acc = _mm_blendv_ps(ctx->vu0_acc, res, _mm_castsi128_ps(_mm_set_epi32(-1, -1, -1, -1))); }
    // 0x10e7940: 0x4be8028b  vmaddw.xyzw $vf10, $vf0, $vf8w
    { __m128 mul_res = PS2_VMUL_K(1, 0, ctx->vu0_vf[0], _mm_shuffle_ps(ctx->vu0_vf[8], ctx->vu0_vf[8], _MM_SHUFFLE(3,3,3,3))); __m128 res = PS2_VADD_K(1, 1, ctx->vu0_acc, mul_res); __m128i mask = _mm_set_epi32(-1, -1, -1, -1); ctx->vu0_vf[10] = _mm_blendv_ps(ctx->vu0_vf[10], res, _mm_castsi128_ps(mask)); }
    // 0x10e7944: 0xf86a0000  sqc2        $vf10, 0x0($v1)
    WRITE128(ADD32(RC_U32(rc_3), 0), _mm_castps_si128(ctx->vu0_vf[10]));
    // 0x10e7948: 0x2a0202d  daddu       $a0, $s5, $zero
    rc_21 = Ps2GprU64(ctx, 21); 
    RC_SET_U64(rc_4, (uint64_t)RC_U64(rc_21) + (uint64_t)RC_U64(0));
    Ps2SetGpr64(ctx, 4, rc_4); 
    // 0x10e794c: 0x220282d  daddu       $a1, $s1, $zero
    RC_SET_U64(rc_5, (uint64_t)RC_U64(rc_17) + (uint64_t)RC_U64(0));
    Ps2SetGpr64(ctx, 5, rc_5); 
    // 0x10e7950: 0x2c0302d  daddu       $a2, $s6, $zero
    RC_SET_U64(rc_6, (uint64_t)RC_U64(rc_22) + (uint64_t)RC_U64(0));
    Ps2SetGpr64(ctx, 6, rc_6); 
    // 0x10e7954: 0x240382d  daddu       $a3, $s2, $zero
    rc_18 = Ps2GprU64(ctx, 18); 
    RC_SET_U64(rc_7, (uint64_t)RC_U64(rc_18) + (uint64_t)RC_U64(0));
    Ps2SetGpr64(ctx, 7, rc_7); 
    // 0x10e7958: 0xc437868  jal         func_10DE1A0
    RC_SET_U32(rc_31, 0x10E7960u);
    Ps2SetGpr64(ctx, 31, RC_U64X(rc_31)); 
    // 0x10e795c: 0xc6390e0c  lwc1        $f25, 0xE0C($s1) (Delay Slot)
    { uint32_t bits = READ32(ADD32(RC_U32(rc_17), 3596)); float f; std::memcpy(&f, &bits, sizeof(f)); rf_25 = f; }
    ctx->f[25] = rf_25; 
    ctx->pc = 0x10DE1A0u;
    if (!PS2_CALL_GUEST_NOCHK(0x10DE1A0u, charrootdyn_getRegularColFinderPosition_0x10de1a0, 0x10E7960u, 0x10E7958u, DirectCall, "JAL")) {
        return;
    }
label_10e7960:
    // 0x10e7960: 0x86250370  lh          $a1, 0x370($s1)
    rc_17 = Ps2GprU64(ctx, 17); 
    RC_SET_S32(rc_5, (int16_t)READ16(ADD32(RC_U32(rc_17), 880)));
    // 0x10e7964: 0x2a0382d  daddu       $a3, $s5, $zero
    rc_21 = Ps2GprU64(ctx, 21); 
    RC_SET_U64(rc_7, (uint64_t)RC_U64(rc_21) + (uint64_t)RC_U64(0));
    Ps2SetGpr64(ctx, 7, rc_7); 
    // 0x10e7968: 0x8e220e38  lw          $v0, 0xE38($s1)
    RC_SET_S32(rc_2, (int32_t)READ32(ADD32(RC_U32(rc_17), 3640)));
    Ps2SetGpr64(ctx, 2, RC_U64X(rc_2)); 
    // 0x10e796c: 0x3c0202d  daddu       $a0, $fp, $zero
    rc_30 = Ps2GprU64(ctx, 30); 
    RC_SET_U64(rc_4, (uint64_t)RC_U64(rc_30) + (uint64_t)RC_U64(0));
    Ps2SetGpr64(ctx, 4, rc_4); 
    // 0x10e7970: 0x8fa601a0  lw          $a2, 0x1A0($sp)
    rc_29 = Ps2GprU64(ctx, 29); 
    RC_SET_S32(rc_6, (int32_t)READ32(ADD32(RC_U32(rc_29), 416)));
    Ps2SetGpr64(ctx, 6, RC_U64X(rc_6)); 
    // 0x10e7974: 0x52880  sll         $a1, $a1, 2
    RC_SET_S32(rc_5, (int32_t)SLL32(RC_U32(rc_5), 2));
    // 0x10e7978: 0x8fa901b4  lw          $t1, 0x1B4($sp)
    RC_SET_S32(rc_9, (int32_t)READ32(ADD32(RC_U32(rc_29), 436)));
    Ps2SetGpr64(ctx, 9, RC_U64X(rc_9)); 
    // 0x10e797c: 0x452821  addu        $a1, $v0, $a1
    RC_SET_S32(rc_5, (int32_t)ADD32(RC_U32(rc_2), RC_U32(rc_5)));
    Ps2SetGpr64(ctx, 5, RC_U64X(rc_5)); 
    // 0x10e7980: 0x4600d306  mov.s       $f12, $f26
    rf_26 = ctx->f[26]; 
    rf_12 = FPU_MOV_S(rf_26);
    ctx->f[12] = rf_12; 
    // 0x10e7984: 0xc4350fa  jal         func_10D43E8
    RC_SET_U32(rc_31, 0x10E798Cu);
    Ps2SetGpr64(ctx, 31, RC_U64X(rc_31)); 
    // 0x10e7988: 0x262804d0  addiu       $t0, $s1, 0x4D0 (Delay Slot)
    RC_SET_S32(rc_8, (int32_t)ADD32(RC_U32(rc_17), 1232));
    Ps2SetGpr64(ctx, 8, RC_U64X(rc_8)); 
    ctx->pc = 0x10D43E8u;
    if (!PS2_CALL_GUEST_KNOWN(0x10D43E8u, charrootdyn_getCollisionDB_0x10d43e8, 0x10E798Cu, 0x10E7984u, DirectCall, "JAL")) {
        return;
    }
label_10e798c:
    // 0x10e798c: 0x220282d  daddu       $a1, $s1, $zero
    rc_17 = Ps2GprU64(ctx, 17); 
    RC_SET_U64(rc_5, (uint64_t)RC_U64(rc_17) + (uint64_t)RC_U64(0));
    Ps2SetGpr64(ctx, 5, rc_5); 
    // 0x10e7990: 0x27b40030  addiu       $s4, $sp, 0x30
    rc_29 = Ps2GprU64(ctx, 29); 
    RC_SET_S32(rc_20, (int32_t)ADD32(RC_U32(rc_29), 48));
    Ps2SetGpr64(ctx, 20, RC_U64X(rc_20)); 
    // 0x10e7994: 0x4600bb06  mov.s       $f12, $f23
    rf_23 = ctx->f[23]; 
    rf_12 = FPU_MOV_S(rf_23);
    ctx->f[12] = rf_12; 
    // 0x10e7998: 0xc434f48  jal         func_10D3D20
    RC_SET_U32(rc_31, 0x10E79A0u);
    Ps2SetGpr64(ctx, 31, RC_U64X(rc_31)); 
    // 0x10e799c: 0x280202d  daddu       $a0, $s4, $zero (Delay Slot)
    RC_SET_U64(rc_4, (uint64_t)RC_U64X(rc_20) + (uint64_t)RC_U64(0));
    Ps2SetGpr64(ctx, 4, RC_U64X(rc_4)); 
    ctx->pc = 0x10D3D20u;
    if (!PS2_CALL_GUEST_KNOWN(0x10D3D20u, charrootdyn_acceptMotionRootOffset_0x10d3d20, 0x10E79A0u, 0x10E7998u, DirectCall, "JAL")) {
        return;
    }
label_10e79a0:
    // 0x10e79a0: 0x8fa301a8  lw          $v1, 0x1A8($sp)
    rc_29 = Ps2GprU64(ctx, 29); 
    RC_SET_S32(rc_3, (int32_t)READ32(ADD32(RC_U32(rc_29), 424)));
    Ps2SetGpr64(ctx, 3, RC_U64X(rc_3)); 
    // 0x10e79a4: 0x10600042  beqz        $v1, . + 4 + (0x42 << 2)
    {
        const bool branch_taken_0x10e79a4 = (RC_U64X(rc_3) == RC_U64(0));
        // 0x10e79a8: 0x27a40040  addiu       $a0, $sp, 0x40 (Delay Slot)
        RC_SET_S32(rc_4, (int32_t)ADD32(RC_U32(rc_29), 64));
        Ps2SetGpr64(ctx, 4, RC_U64X(rc_4)); 
        if (branch_taken_0x10e79a4) {
            goto label_10e7ab0;
        }
    }
    // 0x10e79ac: 0xc6340e0c  lwc1        $f20, 0xE0C($s1)
    rc_17 = Ps2GprU64(ctx, 17); 
    { uint32_t bits = READ32(ADD32(RC_U32(rc_17), 3596)); float f; std::memcpy(&f, &bits, sizeof(f)); rf_20 = f; }
    ctx->f[20] = rf_20; 
    // 0x10e79b0: 0x80982d  daddu       $s3, $a0, $zero
    RC_SET_U64(rc_19, (uint64_t)RC_U64X(rc_4) + (uint64_t)RC_U64(0));
    Ps2SetGpr64(ctx, 19, RC_U64X(rc_19)); 
    // 0x10e79b4: 0x8fa201b0  lw          $v0, 0x1B0($sp)
    RC_SET_S32(rc_2, (int32_t)READ32(ADD32(RC_U32(rc_29), 432)));
    Ps2SetGpr64(ctx, 2, RC_U64X(rc_2)); 
    // 0x10e79b8: 0xd8450000  lqc2        $vf5, 0x0($v0)
    ctx->vu0_vf[5] = _mm_castsi128_ps(READ128(ADD32(RC_U32(rc_2), 0)));
    // 0x10e79bc: 0xdba60000  lqc2        $vf6, 0x0($sp)
    ctx->vu0_vf[6] = _mm_castsi128_ps(READ128(ADD32(RC_U32(rc_29), 0)));
    // 0x10e79c0: 0x4bc62968  vadd.xyz    $vf5, $vf5, $vf6
    { __m128 res = PS2_VADD(ctx->vu0_vf[5], ctx->vu0_vf[6]); __m128i mask = _mm_set_epi32(0, -1, -1, -1); ctx->vu0_vf[5] = PS2_VBLEND(ctx->vu0_vf[5], res, _mm_castsi128_ps(mask)); }
    // 0x10e79c4: 0xfa650000  sqc2        $vf5, 0x0($s3)
    WRITE128(ADD32(RC_U32(rc_19), 0), _mm_castps_si128(ctx->vu0_vf[5]));
    // 0x10e79c8: 0x27b00050  addiu       $s0, $sp, 0x50
    RC_SET_S32(rc_16, (int32_t)ADD32(RC_U32(rc_29), 80));
    Ps2SetGpr64(ctx, 16, RC_U64X(rc_16)); 
    // 0x10e79cc: 0xda450000  lqc2        $vf5, 0x0($s2)
    rc_18 = Ps2GprU64(ctx, 18); 
    ctx->vu0_vf[5] = _mm_castsi128_ps(READ128(ADD32(RC_U32(rc_18), 0)));
    // 0x10e79d0: 0xdba60000  lqc2        $vf6, 0x0($sp)
    ctx->vu0_vf[6] = _mm_castsi128_ps(READ128(ADD32(RC_U32(rc_29), 0)));
    // 0x10e79d4: 0x4bc62968  vadd.xyz    $vf5, $vf5, $vf6
    { __m128 res = PS2_VADD(ctx->vu0_vf[5], ctx->vu0_vf[6]); __m128i mask = _mm_set_epi32(0, -1, -1, -1); ctx->vu0_vf[5] = PS2_VBLEND(ctx->vu0_vf[5], res, _mm_castsi128_ps(mask)); }
    // 0x10e79d8: 0xfa050000  sqc2        $vf5, 0x0($s0)
    WRITE128(ADD32(RC_U32(rc_16), 0), _mm_castps_si128(ctx->vu0_vf[5]));
    // 0x10e79dc: 0x8fa301b8  lw          $v1, 0x1B8($sp)
    RC_SET_S32(rc_3, (int32_t)READ32(ADD32(RC_U32(rc_29), 440)));
    Ps2SetGpr64(ctx, 3, RC_U64X(rc_3)); 
    // 0x10e79e0: 0x5060000c  beql        $v1, $zero, . + 4 + (0xC << 2)
    {
        const bool branch_taken_0x10e79e0 = (RC_U64X(rc_3) == RC_U64(0));
        rc_21 = Ps2GprU64(ctx, 21); 
        if (branch_taken_0x10e79e0) {
            // 0x10e79e4: 0x27b50060  addiu       $s5, $sp, 0x60 (Delay Slot)
            RC_SET_S32(rc_21, (int32_t)ADD32(RC_U32(rc_29), 96));
            Ps2SetGpr64(ctx, 21, RC_U64X(rc_21)); 
            goto label_10e7a14;
        }
    }
    // 0x10e79e8: 0x82220aa0  lb          $v0, 0xAA0($s1)
    RC_SET_S32(rc_2, (int8_t)READ8(ADD32(RC_U32(rc_17), 2720)));
    Ps2SetGpr64(ctx, 2, RC_U64X(rc_2)); 
    // 0x10e79ec: 0x10400006  beqz        $v0, . + 4 + (0x6 << 2)
    {
        const bool branch_taken_0x10e79ec = (RC_U64X(rc_2) == RC_U64(0));
        if (branch_taken_0x10e79ec) {
            goto label_10e7a08;
        }
    }
    // 0x10e79f4: 0x60202d  daddu       $a0, $v1, $zero
    RC_SET_U64(rc_4, (uint64_t)RC_U64X(rc_3) + (uint64_t)RC_U64(0));
    Ps2SetGpr64(ctx, 4, RC_U64X(rc_4)); 
    // 0x10e79f8: 0xc44a704  jal         func_1129C10
    RC_SET_U32(rc_31, 0x10E7A00u);
    Ps2SetGpr64(ctx, 31, RC_U64X(rc_31)); 
    // 0x10e79fc: 0x3c0282d  daddu       $a1, $fp, $zero (Delay Slot)
    rc_30 = Ps2GprU64(ctx, 30); 
    RC_SET_U64(rc_5, (uint64_t)RC_U64(rc_30) + (uint64_t)RC_U64(0));
    Ps2SetGpr64(ctx, 5, rc_5); 
    ctx->pc = 0x1129C10u;
    if (!PS2_CALL_GUEST_KNOWN(0x1129C10u, GetCollisionEdgeLinkDBVWallFinder_0x1129c10, 0x10E7A00u, 0x10E79F8u, DirectCall, "JAL")) {
        return;
    }
label_10e7a00:
    // 0x10e7a00: 0x10000004  b           . + 4 + (0x4 << 2)
    {
        const bool branch_taken_0x10e7a00 = (RC_U64(0) == RC_U64(0));
        // 0x10e7a04: 0x27b50060  addiu       $s5, $sp, 0x60 (Delay Slot)
        rc_29 = Ps2GprU64(ctx, 29); 
        RC_SET_S32(rc_21, (int32_t)ADD32(RC_U32(rc_29), 96));
        Ps2SetGpr64(ctx, 21, RC_U64X(rc_21)); 
        if (branch_taken_0x10e7a00) {
            goto label_10e7a14;
        }
    }
label_10e7a08:
    // 0x10e7a08: 0xc44a6c0  jal         func_1129B00
    RC_SET_U32(rc_31, 0x10E7A10u);
    Ps2SetGpr64(ctx, 31, RC_U64X(rc_31)); 
    // 0x10e7a0c: 0x8fa401b8  lw          $a0, 0x1B8($sp) (Delay Slot)
    rc_29 = Ps2GprU64(ctx, 29); 
    RC_SET_S32(rc_4, (int32_t)READ32(ADD32(RC_U32(rc_29), 440)));
    Ps2SetGpr64(ctx, 4, RC_U64X(rc_4)); 
    ctx->pc = 0x1129B00u;
    if (!PS2_CALL_GUEST_NOCHK(0x1129B00u, FlushCollisionEdgeLinkDBVWallFinder_0x1129b00, 0x10E7A10u, 0x10E7A08u, DirectCall, "JAL")) {
        return;
    }
label_10e7a10:
    // 0x10e7a10: 0x27b50060  addiu       $s5, $sp, 0x60
    rc_29 = Ps2GprU64(ctx, 29); 
    RC_SET_S32(rc_21, (int32_t)ADD32(RC_U32(rc_29), 96));
    Ps2SetGpr64(ctx, 21, RC_U64X(rc_21)); 
label_10e7a14:
    // 0x10e7a14: 0x3c013c23  lui         $at, 0x3C23
    RC_SET_S32(rc_1, (int32_t)((uint32_t)15395 << 16));
    // 0x10e7a18: 0x3421d70a  ori         $at, $at, 0xD70A
    RC_SET_U64(rc_1, RC_U64X(rc_1) | (uint64_t)(uint16_t)55050);
    Ps2SetGpr64(ctx, 1, RC_U64X(rc_1)); 
    // 0x10e7a1c: 0x44816800  mtc1        $at, $f13
    { uint32_t bits = RC_U32(rc_1); std::memcpy(&rf_13, &bits, sizeof(bits)); }
    ctx->f[13] = rf_13; 
    // 0x10e7a20: 0x8fa901b8  lw          $t1, 0x1B8($sp)
    rc_29 = Ps2GprU64(ctx, 29); 
    RC_SET_S32(rc_9, (int32_t)READ32(ADD32(RC_U32(rc_29), 440)));
    Ps2SetGpr64(ctx, 9, RC_U64X(rc_9)); 
    // 0x10e7a24: 0x260282d  daddu       $a1, $s3, $zero
    rc_19 = Ps2GprU64(ctx, 19); 
    RC_SET_U64(rc_5, (uint64_t)RC_U64(rc_19) + (uint64_t)RC_U64(0));
    Ps2SetGpr64(ctx, 5, rc_5); 
    // 0x10e7a28: 0x8faa01b4  lw          $t2, 0x1B4($sp)
    RC_SET_S32(rc_10, (int32_t)READ32(ADD32(RC_U32(rc_29), 436)));
    Ps2SetGpr64(ctx, 10, RC_U64X(rc_10)); 
    // 0x10e7a2c: 0x200302d  daddu       $a2, $s0, $zero
    rc_16 = Ps2GprU64(ctx, 16); 
    RC_SET_U64(rc_6, (uint64_t)RC_U64(rc_16) + (uint64_t)RC_U64(0));
    Ps2SetGpr64(ctx, 6, rc_6); 
    // 0x10e7a30: 0x280382d  daddu       $a3, $s4, $zero
    rc_20 = Ps2GprU64(ctx, 20); 
    RC_SET_U64(rc_7, (uint64_t)RC_U64(rc_20) + (uint64_t)RC_U64(0));
    Ps2SetGpr64(ctx, 7, rc_7); 
    // 0x10e7a34: 0x2a0202d  daddu       $a0, $s5, $zero
    rc_21 = Ps2GprU64(ctx, 21); 
    RC_SET_U64(rc_4, (uint64_t)RC_U64(rc_21) + (uint64_t)RC_U64(0));
    Ps2SetGpr64(ctx, 4, rc_4); 
    // 0x10e7a38: 0x4600a306  mov.s       $f12, $f20
    rf_20 = ctx->f[20]; 
    rf_12 = FPU_MOV_S(rf_20);
    ctx->f[12] = rf_12; 
    // 0x10e7a3c: 0xc437008  jal         func_10DC020
    RC_SET_U32(rc_31, 0x10E7A44u);
    Ps2SetGpr64(ctx, 31, RC_U64X(rc_31)); 
    // 0x10e7a40: 0x3c0402d  daddu       $t0, $fp, $zero (Delay Slot)
    rc_30 = Ps2GprU64(ctx, 30); 
    RC_SET_U64(rc_8, (uint64_t)RC_U64(rc_30) + (uint64_t)RC_U64(0));
    Ps2SetGpr64(ctx, 8, rc_8); 
    ctx->pc = 0x10DC020u;
    if (!PS2_CALL_GUEST_KNOWN(0x10DC020u, charrootdyn_slidecollision_0x10dc020, 0x10E7A44u, 0x10E7A3Cu, DirectCall, "JAL")) {
        return;
    }
label_10e7a44:
    // 0x10e7a44: 0x1040000d  beqz        $v0, . + 4 + (0xD << 2)
    {
        rc_2 = Ps2GprU64(ctx, 2); 
        const bool branch_taken_0x10e7a44 = (RC_U64(rc_2) == RC_U64(0));
        // 0x10e7a48: 0xafa201bc  sw          $v0, 0x1BC($sp) (Delay Slot)
        rc_29 = Ps2GprU64(ctx, 29); 
        WRITE32(ADD32(RC_U32(rc_29), 444), RC_U32(rc_2));
        if (branch_taken_0x10e7a44) {
            goto label_10e7a7c;
        }
    }
    // 0x10e7a4c: 0x8fa401bc  lw          $a0, 0x1BC($sp)
    RC_SET_S32(rc_4, (int32_t)READ32(ADD32(RC_U32(rc_29), 444)));
    Ps2SetGpr64(ctx, 4, RC_U64X(rc_4)); 
    // 0x10e7a50: 0x24020002  addiu       $v0, $zero, 0x2
    RC_SET_S32(rc_2, (int32_t)ADD32(RC_U32(0), 2));
    Ps2SetGpr64(ctx, 2, RC_U64X(rc_2)); 
    // 0x10e7a54: 0x14820003  bne         $a0, $v0, . + 4 + (0x3 << 2)
    {
        const bool branch_taken_0x10e7a54 = (RC_U64X(rc_4) != RC_U64X(rc_2));
        // 0x10e7a58: 0x4600a306  mov.s       $f12, $f20 (Delay Slot)
        rf_20 = ctx->f[20]; 
        rf_12 = FPU_MOV_S(rf_20);
        ctx->f[12] = rf_12; 
        if (branch_taken_0x10e7a54) {
            goto label_10e7a64;
        }
    }
    // 0x10e7a5c: 0x24020001  addiu       $v0, $zero, 0x1
    RC_SET_S32(rc_2, (int32_t)ADD32(RC_U32(0), 1));
    Ps2SetGpr64(ctx, 2, RC_U64X(rc_2)); 
    // 0x10e7a60: 0xa2220530  sb          $v0, 0x530($s1)
    rc_17 = Ps2GprU64(ctx, 17); 
    WRITE8(ADD32(RC_U32(rc_17), 1328), (uint8_t)RC_U32(rc_2));
label_10e7a64:
    // 0x10e7a64: 0x220202d  daddu       $a0, $s1, $zero
    rc_17 = Ps2GprU64(ctx, 17); 
    RC_SET_U64(rc_4, (uint64_t)RC_U64(rc_17) + (uint64_t)RC_U64(0));
    Ps2SetGpr64(ctx, 4, rc_4); 
    // 0x10e7a68: 0x27a50080  addiu       $a1, $sp, 0x80
    RC_SET_S32(rc_5, (int32_t)ADD32(RC_U32(rc_29), 128));
    Ps2SetGpr64(ctx, 5, RC_U64X(rc_5)); 
    // 0x10e7a6c: 0x2a0302d  daddu       $a2, $s5, $zero
    rc_21 = Ps2GprU64(ctx, 21); 
    RC_SET_U64(rc_6, (uint64_t)RC_U64(rc_21) + (uint64_t)RC_U64(0));
    Ps2SetGpr64(ctx, 6, rc_6); 
    // 0x10e7a70: 0x27a70090  addiu       $a3, $sp, 0x90
    RC_SET_S32(rc_7, (int32_t)ADD32(RC_U32(rc_29), 144));
    Ps2SetGpr64(ctx, 7, RC_U64X(rc_7)); 
    // 0x10e7a74: 0xc435af6  jal         func_10D6BD8
    RC_SET_U32(rc_31, 0x10E7A7Cu);
    Ps2SetGpr64(ctx, 31, RC_U64X(rc_31)); 
    // 0x10e7a78: 0x27a800a0  addiu       $t0, $sp, 0xA0 (Delay Slot)
    RC_SET_S32(rc_8, (int32_t)ADD32(RC_U32(rc_29), 160));
    Ps2SetGpr64(ctx, 8, RC_U64X(rc_8)); 
    ctx->pc = 0x10D6BD8u;
    if (!PS2_CALL_GUEST_KNOWN(0x10D6BD8u, charrootdyn_setkeepcolhitstatus_0x10d6bd8, 0x10E7A7Cu, 0x10E7A74u, DirectCall, "JAL")) {
        return;
    }
label_10e7a7c:
    // 0x10e7a7c: 0x27a20070  addiu       $v0, $sp, 0x70
    rc_29 = Ps2GprU64(ctx, 29); 
    RC_SET_S32(rc_2, (int32_t)ADD32(RC_U32(rc_29), 112));
    Ps2SetGpr64(ctx, 2, RC_U64X(rc_2)); 
    // 0x10e7a80: 0x8fa301b0  lw          $v1, 0x1B0($sp)
    RC_SET_S32(rc_3, (int32_t)READ32(ADD32(RC_U32(rc_29), 432)));
    Ps2SetGpr64(ctx, 3, RC_U64X(rc_3)); 
    // 0x10e7a84: 0xd8450000  lqc2        $vf5, 0x0($v0)
    ctx->vu0_vf[5] = _mm_castsi128_ps(READ128(ADD32(RC_U32(rc_2), 0)));
    // 0x10e7a88: 0xdba60000  lqc2        $vf6, 0x0($sp)
    ctx->vu0_vf[6] = _mm_castsi128_ps(READ128(ADD32(RC_U32(rc_29), 0)));
    // 0x10e7a8c: 0x4bc6296c  vsub.xyz    $vf5, $vf5, $vf6
    { __m128 res = PS2_VSUB(ctx->vu0_vf[5], ctx->vu0_vf[6]); __m128i mask = _mm_set_epi32(0, -1, -1, -1); ctx->vu0_vf[5] = PS2_VBLEND(ctx->vu0_vf[5], res, _mm_castsi128_ps(mask)); }
    // 0x10e7a90: 0xf8650000  sqc2        $vf5, 0x0($v1)
    WRITE128(ADD32(RC_U32(rc_3), 0), _mm_castps_si128(ctx->vu0_vf[5]));
    // 0x10e7a94: 0xdaa50000  lqc2        $vf5, 0x0($s5)
    rc_21 = Ps2GprU64(ctx, 21); 
    ctx->vu0_vf[5] = _mm_castsi128_ps(READ128(ADD32(RC_U32(rc_21), 0)));
    // 0x10e7a98: 0xdba60000  lqc2        $vf6, 0x0($sp)
    ctx->vu0_vf[6] = _mm_castsi128_ps(READ128(ADD32(RC_U32(rc_29), 0)));
    // 0x10e7a9c: 0x4bc6296c  vsub.xyz    $vf5, $vf5, $vf6
    { __m128 res = PS2_VSUB(ctx->vu0_vf[5], ctx->vu0_vf[6]); __m128i mask = _mm_set_epi32(0, -1, -1, -1); ctx->vu0_vf[5] = PS2_VBLEND(ctx->vu0_vf[5], res, _mm_castsi128_ps(mask)); }
    // 0x10e7aa0: 0xfa450000  sqc2        $vf5, 0x0($s2)
    rc_18 = Ps2GprU64(ctx, 18); 
    WRITE128(ADD32(RC_U32(rc_18), 0), _mm_castps_si128(ctx->vu0_vf[5]));
    // 0x10e7aa4: 0x10000007  b           . + 4 + (0x7 << 2)
    {
        const bool branch_taken_0x10e7aa4 = (RC_U64(0) == RC_U64(0));
        // 0x10e7aa8: 0x8e220124  lw          $v0, 0x124($s1) (Delay Slot)
        rc_17 = Ps2GprU64(ctx, 17); 
        RC_SET_S32(rc_2, (int32_t)READ32(ADD32(RC_U32(rc_17), 292)));
        Ps2SetGpr64(ctx, 2, RC_U64X(rc_2)); 
        if (branch_taken_0x10e7aa4) {
            goto label_10e7ac4;
        }
    }
    // 0x10e7aac: 0x0  nop
    // NOP
label_10e7ab0:
    // 0x10e7ab0: 0xda450000  lqc2        $vf5, 0x0($s2)
    rc_18 = Ps2GprU64(ctx, 18); 
    ctx->vu0_vf[5] = _mm_castsi128_ps(READ128(ADD32(RC_U32(rc_18), 0)));
    // 0x10e7ab4: 0xda860000  lqc2        $vf6, 0x0($s4)
    rc_20 = Ps2GprU64(ctx, 20); 
    ctx->vu0_vf[6] = _mm_castsi128_ps(READ128(ADD32(RC_U32(rc_20), 0)));
    // 0x10e7ab8: 0x4bc62968  vadd.xyz    $vf5, $vf5, $vf6
    { __m128 res = PS2_VADD(ctx->vu0_vf[5], ctx->vu0_vf[6]); __m128i mask = _mm_set_epi32(0, -1, -1, -1); ctx->vu0_vf[5] = PS2_VBLEND(ctx->vu0_vf[5], res, _mm_castsi128_ps(mask)); }
    // 0x10e7abc: 0xfa450000  sqc2        $vf5, 0x0($s2)
    WRITE128(ADD32(RC_U32(rc_18), 0), _mm_castps_si128(ctx->vu0_vf[5]));
    // 0x10e7ac0: 0x8e220124  lw          $v0, 0x124($s1)
    rc_17 = Ps2GprU64(ctx, 17); 
    RC_SET_S32(rc_2, (int32_t)READ32(ADD32(RC_U32(rc_17), 292)));
    Ps2SetGpr64(ctx, 2, RC_U64X(rc_2)); 
label_10e7ac4:
    // 0x10e7ac4: 0x26280700  addiu       $t0, $s1, 0x700
    rc_17 = Ps2GprU64(ctx, 17); 
    RC_SET_S32(rc_8, (int32_t)ADD32(RC_U32(rc_17), 1792));
    Ps2SetGpr64(ctx, 8, RC_U64X(rc_8)); 
    // 0x10e7ac8: 0x10400009  beqz        $v0, . + 4 + (0x9 << 2)
    {
        rc_2 = Ps2GprU64(ctx, 2); 
        const bool branch_taken_0x10e7ac8 = (RC_U64(rc_2) == RC_U64(0));
        // 0x10e7acc: 0x482d  daddu       $t1, $zero, $zero (Delay Slot)
        RC_SET_U64(rc_9, (uint64_t)RC_U64(0) + (uint64_t)RC_U64(0));
        Ps2SetGpr64(ctx, 9, RC_U64X(rc_9)); 
        if (branch_taken_0x10e7ac8) {
            goto label_10e7af0;
        }
    }
    // 0x10e7ad0: 0x8fa401bc  lw          $a0, 0x1BC($sp)
    rc_29 = Ps2GprU64(ctx, 29); 
    RC_SET_S32(rc_4, (int32_t)READ32(ADD32(RC_U32(rc_29), 444)));
    Ps2SetGpr64(ctx, 4, RC_U64X(rc_4)); 
    // 0x10e7ad4: 0x14800007  bnez        $a0, . + 4 + (0x7 << 2)
    {
        const bool branch_taken_0x10e7ad4 = (RC_U64X(rc_4) != RC_U64(0));
        // 0x10e7ad8: 0x8fa501a0  lw          $a1, 0x1A0($sp) (Delay Slot)
        RC_SET_S32(rc_5, (int32_t)READ32(ADD32(RC_U32(rc_29), 416)));
        Ps2SetGpr64(ctx, 5, RC_U64X(rc_5)); 
        if (branch_taken_0x10e7ad4) {
            goto label_10e7af4;
        }
    }
    // 0x10e7adc: 0x8fa301c4  lw          $v1, 0x1C4($sp)
    RC_SET_S32(rc_3, (int32_t)READ32(ADD32(RC_U32(rc_29), 452)));
    Ps2SetGpr64(ctx, 3, RC_U64X(rc_3)); 
    // 0x10e7ae0: 0x80482d  daddu       $t1, $a0, $zero
    RC_SET_U64(rc_9, (uint64_t)RC_U64X(rc_4) + (uint64_t)RC_U64(0));
    Ps2SetGpr64(ctx, 9, RC_U64X(rc_9)); 
    // 0x10e7ae4: 0x8fa401c0  lw          $a0, 0x1C0($sp)
    RC_SET_S32(rc_4, (int32_t)READ32(ADD32(RC_U32(rc_29), 448)));
    Ps2SetGpr64(ctx, 4, RC_U64X(rc_4)); 
    // 0x10e7ae8: 0x2c620001  sltiu       $v0, $v1, 0x1
    RC_SET_U64(rc_2, ((uint64_t)RC_U64X(rc_3) < (uint64_t)(int64_t)(int32_t)1) ? 1 : 0);
    Ps2SetGpr64(ctx, 2, RC_U64X(rc_2)); 
    // 0x10e7aec: 0x44480a  movz        $t1, $v0, $a0
    if (RC_U64X(rc_4) == 0) RC_SET_VEC(ctx, 9, rc_9, GPR_VEC(ctx, 2));
    Ps2SetGpr64(ctx, 9, rc_9); 
label_10e7af0:
    // 0x10e7af0: 0x8fa501a0  lw          $a1, 0x1A0($sp)
    rc_29 = Ps2GprU64(ctx, 29); 
    RC_SET_S32(rc_5, (int32_t)READ32(ADD32(RC_U32(rc_29), 416)));
    Ps2SetGpr64(ctx, 5, RC_U64X(rc_5)); 
label_10e7af4:
    // 0x10e7af4: 0x2c0302d  daddu       $a2, $s6, $zero
    rc_22 = Ps2GprU64(ctx, 22); 
    RC_SET_U64(rc_6, (uint64_t)RC_U64(rc_22) + (uint64_t)RC_U64(0));
    Ps2SetGpr64(ctx, 6, rc_6); 
    // 0x10e7af8: 0x240202d  daddu       $a0, $s2, $zero
    rc_18 = Ps2GprU64(ctx, 18); 
    RC_SET_U64(rc_4, (uint64_t)RC_U64(rc_18) + (uint64_t)RC_U64(0));
    Ps2SetGpr64(ctx, 4, rc_4); 
    // 0x10e7afc: 0xc4377ec  jal         func_10DDFB0
    RC_SET_U32(rc_31, 0x10E7B04u);
    Ps2SetGpr64(ctx, 31, RC_U64X(rc_31)); 
    // 0x10e7b00: 0x240382d  daddu       $a3, $s2, $zero (Delay Slot)
    RC_SET_U64(rc_7, (uint64_t)RC_U64(rc_18) + (uint64_t)RC_U64(0));
    Ps2SetGpr64(ctx, 7, rc_7); 
    ctx->pc = 0x10DDFB0u;
    if (!PS2_CALL_GUEST_KNOWN(0x10DDFB0u, charrootdyn_restoreposition_0x10ddfb0, 0x10E7B04u, 0x10E7AFCu, DirectCall, "JAL")) {
        return;
    }
label_10e7b04:
    // 0x10e7b04: 0xc6e10004  lwc1        $f1, 0x4($s7)
    rc_23 = Ps2GprU64(ctx, 23); 
    { uint32_t bits = READ32(ADD32(RC_U32(rc_23), 4)); float f; std::memcpy(&f, &bits, sizeof(f)); rf_1 = f; }
    ctx->f[1] = rf_1; 
    // 0x10e7b08: 0xc7a00004  lwc1        $f0, 0x4($sp)
    rc_29 = Ps2GprU64(ctx, 29); 
    { uint32_t bits = READ32(ADD32(RC_U32(rc_29), 4)); float f; std::memcpy(&f, &bits, sizeof(f)); rf_0 = f; }
    ctx->f[0] = rf_0; 
    // 0x10e7b0c: 0x46000d80  add.s       $f22, $f1, $f0
    rf_22 = FPU_ADD_S(rf_1, rf_0);
    ctx->f[22] = rf_22; 
    // 0x10e7b10: 0x4619b034  c.lt.s      $f22, $f25
    rf_25 = ctx->f[25]; 
    ctx->fcr31 = (FPU_C_OLT_S(rf_22, rf_25)) ? (ctx->fcr31 | 0x800000) : (ctx->fcr31 & ~0x800000);
    // 0x10e7b14: 0x45010002  bc1t        . + 4 + (0x2 << 2)
    {
        const bool branch_taken_0x10e7b14 = ((ctx->fcr31 & 0x800000));
        // 0x10e7b18: 0x4600cb06  mov.s       $f12, $f25 (Delay Slot)
        rf_12 = FPU_MOV_S(rf_25);
        ctx->f[12] = rf_12; 
        if (branch_taken_0x10e7b14) {
            goto label_10e7b20;
        }
    }
    // 0x10e7b1c: 0x4600b306  mov.s       $f12, $f22
    rf_12 = FPU_MOV_S(rf_22);
    ctx->f[12] = rf_12; 
label_10e7b20:
    // 0x10e7b20: 0x8fa201c8  lw          $v0, 0x1C8($sp)
    RC_SET_S32(rc_2, (int32_t)READ32(ADD32(RC_U32(rc_29), 456)));
    Ps2SetGpr64(ctx, 2, RC_U64X(rc_2)); 
    // 0x10e7b24: 0x14400004  bnez        $v0, . + 4 + (0x4 << 2)
    {
        const bool branch_taken_0x10e7b24 = (RC_U64X(rc_2) != RC_U64(0));
        // 0x10e7b28: 0x46006607  neg.s       $f24, $f12 (Delay Slot)
        rf_24 = FPU_NEG_S(rf_12);
        ctx->f[24] = rf_24; 
        if (branch_taken_0x10e7b24) {
            goto label_10e7b38;
        }
    }
    // 0x10e7b2c: 0x10000003  b           . + 4 + (0x3 << 2)
    {
        const bool branch_taken_0x10e7b2c = (RC_U64(0) == RC_U64(0));
        // 0x10e7b30: 0xc62003d8  lwc1        $f0, 0x3D8($s1) (Delay Slot)
        rc_17 = Ps2GprU64(ctx, 17); 
        { uint32_t bits = READ32(ADD32(RC_U32(rc_17), 984)); float f; std::memcpy(&f, &bits, sizeof(f)); rf_0 = f; }
        ctx->f[0] = rf_0; 
        if (branch_taken_0x10e7b2c) {
            goto label_10e7b3c;
        }
    }
    // 0x10e7b34: 0x0  nop
    // NOP
label_10e7b38:
    // 0x10e7b38: 0x44800000  mtc1        $zero, $f0
    { uint32_t bits = RC_U32(0); std::memcpy(&rf_0, &bits, sizeof(bits)); }
    ctx->f[0] = rf_0; 
label_10e7b3c:
    // 0x10e7b3c: 0x4600c301  sub.s       $f12, $f24, $f0
    rf_24 = ctx->f[24]; 
    rf_12 = FPU_SUB_S(rf_24, rf_0);
    ctx->f[12] = rf_12; 
    // 0x10e7b40: 0x27a30040  addiu       $v1, $sp, 0x40
    rc_29 = Ps2GprU64(ctx, 29); 
    RC_SET_S32(rc_3, (int32_t)ADD32(RC_U32(rc_29), 64));
    Ps2SetGpr64(ctx, 3, RC_U64X(rc_3)); 
    // 0x10e7b44: 0x3c0282d  daddu       $a1, $fp, $zero
    rc_30 = Ps2GprU64(ctx, 30); 
    RC_SET_U64(rc_5, (uint64_t)RC_U64(rc_30) + (uint64_t)RC_U64(0));
    Ps2SetGpr64(ctx, 5, rc_5); 
    // 0x10e7b48: 0x60202d  daddu       $a0, $v1, $zero
    RC_SET_U64(rc_4, (uint64_t)RC_U64X(rc_3) + (uint64_t)RC_U64(0));
    Ps2SetGpr64(ctx, 4, RC_U64X(rc_4)); 
    // 0x10e7b4c: 0x240302d  daddu       $a2, $s2, $zero
    rc_18 = Ps2GprU64(ctx, 18); 
    RC_SET_U64(rc_6, (uint64_t)RC_U64(rc_18) + (uint64_t)RC_U64(0));
    Ps2SetGpr64(ctx, 6, rc_6); 
    // 0x10e7b50: 0xc4371f0  jal         func_10DC7C0
    RC_SET_U32(rc_31, 0x10E7B58u);
    Ps2SetGpr64(ctx, 31, RC_U64X(rc_31)); 
    // 0x10e7b54: 0x240382d  daddu       $a3, $s2, $zero (Delay Slot)
    RC_SET_U64(rc_7, (uint64_t)RC_U64(rc_18) + (uint64_t)RC_U64(0));
    Ps2SetGpr64(ctx, 7, rc_7); 
    ctx->pc = 0x10DC7C0u;
    if (!PS2_CALL_GUEST_KNOWN(0x10DC7C0u, charrootdyn_getLandingSurface_0x10dc7c0, 0x10E7B58u, 0x10E7B50u, DirectCall, "JAL")) {
        return;
    }
label_10e7b58:
    // 0x10e7b58: 0x40982d  daddu       $s3, $v0, $zero
    rc_2 = Ps2GprU64(ctx, 2); 
    RC_SET_U64(rc_19, (uint64_t)RC_U64(rc_2) + (uint64_t)RC_U64(0));
    Ps2SetGpr64(ctx, 19, rc_19); 
    // 0x10e7b5c: 0x12600118  beqz        $s3, . + 4 + (0x118 << 2)
    {
        const bool branch_taken_0x10e7b5c = (RC_U64(rc_19) == RC_U64(0));
        if (branch_taken_0x10e7b5c) {
            goto label_10e7fc0;
        }
    }
    // 0x10e7b64: 0x8e231140  lw          $v1, 0x1140($s1)
    rc_17 = Ps2GprU64(ctx, 17); 
    RC_SET_S32(rc_3, (int32_t)READ32(ADD32(RC_U32(rc_17), 4416)));
    Ps2SetGpr64(ctx, 3, RC_U64X(rc_3)); 
    // 0x10e7b68: 0x8c620038  lw          $v0, 0x38($v1)
    RC_SET_S32(rc_2, (int32_t)READ32(ADD32(RC_U32(rc_3), 56)));
    // 0x10e7b6c: 0x30420001  andi        $v0, $v0, 0x1
    RC_SET_U64(rc_2, RC_U64X(rc_2) & (uint64_t)(uint16_t)1);
    Ps2SetGpr64(ctx, 2, RC_U64X(rc_2)); 
    // 0x10e7b70: 0x1040002a  beqz        $v0, . + 4 + (0x2A << 2)
    {
        const bool branch_taken_0x10e7b70 = (RC_U64X(rc_2) == RC_U64(0));
        // 0x10e7b74: 0x27a20040  addiu       $v0, $sp, 0x40 (Delay Slot)
        rc_29 = Ps2GprU64(ctx, 29); 
        RC_SET_S32(rc_2, (int32_t)ADD32(RC_U32(rc_29), 64));
        Ps2SetGpr64(ctx, 2, RC_U64X(rc_2)); 
        if (branch_taken_0x10e7b70) {
            goto label_10e7c1c;
        }
    }
    // 0x10e7b78: 0x0  nop
    // NOP
    // 0x10e7b7c: 0x0  nop
    // NOP
    // 0x10e7b80: 0x4616cd43  div.s       $f21, $f25, $f22
    rf_22 = ctx->f[22]; rf_25 = ctx->f[25]; 
    rf_21 = ps2FpuDivide(ctx->fcr31, rf_25, rf_22);
    ctx->f[21] = rf_21; 
    // 0x10e7b84: 0xc44ca14  jal         func_1132850
    RC_SET_U32(rc_31, 0x10E7B8Cu);
    Ps2SetGpr64(ctx, 31, RC_U64X(rc_31)); 
    // 0x10e7b88: 0x40202d  daddu       $a0, $v0, $zero (Delay Slot)
    RC_SET_U64(rc_4, (uint64_t)RC_U64X(rc_2) + (uint64_t)RC_U64(0));
    Ps2SetGpr64(ctx, 4, RC_U64X(rc_4)); 
    ctx->pc = 0x1132850u;
    if (!PS2_CALL_GUEST_KNOWN(0x1132850u, GetGeometryPointerSurfacePlane_0x1132850, 0x10E7B8Cu, 0x10E7B84u, DirectCall, "JAL")) {
        return;
    }
label_10e7b8c:
    // 0x10e7b8c: 0x8fa501ac  lw          $a1, 0x1AC($sp)
    rc_29 = Ps2GprU64(ctx, 29); 
    RC_SET_S32(rc_5, (int32_t)READ32(ADD32(RC_U32(rc_29), 428)));
    Ps2SetGpr64(ctx, 5, RC_U64X(rc_5)); 
    // 0x10e7b90: 0x40802d  daddu       $s0, $v0, $zero
    rc_2 = Ps2GprU64(ctx, 2); 
    RC_SET_U64(rc_16, (uint64_t)RC_U64(rc_2) + (uint64_t)RC_U64(0));
    Ps2SetGpr64(ctx, 16, rc_16); 
    // 0x10e7b94: 0x27a400a0  addiu       $a0, $sp, 0xA0
    RC_SET_S32(rc_4, (int32_t)ADD32(RC_U32(rc_29), 160));
    Ps2SetGpr64(ctx, 4, RC_U64X(rc_4)); 
    // 0x10e7b98: 0xc44c760  jal         func_1131D80
    RC_SET_U32(rc_31, 0x10E7BA0u);
    Ps2SetGpr64(ctx, 31, RC_U64X(rc_31)); 
    // 0x10e7b9c: 0xc6140004  lwc1        $f20, 0x4($s0) (Delay Slot)
    { uint32_t bits = READ32(ADD32(RC_U32(rc_16), 4)); float f; std::memcpy(&f, &bits, sizeof(f)); rf_20 = f; }
    ctx->f[20] = rf_20; 
    ctx->pc = 0x1131D80u;
    if (!PS2_CALL_GUEST_KNOWN(0x1131D80u, GetGeometryPointerPosition_0x1131d80, 0x10E7BA0u, 0x10E7B98u, DirectCall, "JAL")) {
        return;
    }
label_10e7ba0:
    // 0x10e7ba0: 0x8fa301bc  lw          $v1, 0x1BC($sp)
    rc_29 = Ps2GprU64(ctx, 29); 
    RC_SET_S32(rc_3, (int32_t)READ32(ADD32(RC_U32(rc_29), 444)));
    Ps2SetGpr64(ctx, 3, RC_U64X(rc_3)); 
    // 0x10e7ba4: 0x1060001d  beqz        $v1, . + 4 + (0x1D << 2)
    {
        const bool branch_taken_0x10e7ba4 = (RC_U64X(rc_3) == RC_U64(0));
        if (branch_taken_0x10e7ba4) {
            goto label_10e7c1c;
        }
    }
    // 0x10e7bac: 0x4615a882  mul.s       $f2, $f21, $f21
    rf_21 = ctx->f[21]; 
    rf_2 = FPU_MUL_S(rf_21, rf_21);
    ctx->f[2] = rf_2; 
    // 0x10e7bb0: 0xda070000  lqc2        $vf7, 0x0($s0)
    rc_16 = Ps2GprU64(ctx, 16); 
    ctx->vu0_vf[7] = _mm_castsi128_ps(READ128(ADD32(RC_U32(rc_16), 0)));
    // 0x10e7bb4: 0x4b4739ea  vmul.xz     $vf7, $vf7, $vf7
    { __m128 res = PS2_VMUL(ctx->vu0_vf[7], ctx->vu0_vf[7]); __m128i mask = _mm_set_epi32(0, -1, 0, -1); ctx->vu0_vf[7] = PS2_VBLEND(ctx->vu0_vf[7], res, _mm_castsi128_ps(mask)); }
    // 0x10e7bb8: 0x4b0739c2  vaddz.x     $vf7, $vf7, $vf7z
    { __m128 res = PS2_VADD_K(1, 1, ctx->vu0_vf[7], _mm_shuffle_ps(ctx->vu0_vf[7], ctx->vu0_vf[7], _MM_SHUFFLE(2,2,2,2))); __m128i mask = _mm_set_epi32(0, 0, 0, -1); ctx->vu0_vf[7] = _mm_blendv_ps(ctx->vu0_vf[7], res, _mm_castsi128_ps(mask)); }
    // 0x10e7bbc: 0x48223800  qmfc2.ni    $v0, $vf7
    RC_SET_VEC(ctx, 2, rc_2, _mm_castps_si128(ctx->vu0_vf[7]));
    // 0x10e7bc0: 0x44820800  mtc1        $v0, $f1
    { uint32_t bits = RC_U32(rc_2); std::memcpy(&rf_1, &bits, sizeof(bits)); }
    ctx->f[1] = rf_1; 
    // 0x10e7bc4: 0x4614a002  mul.s       $f0, $f20, $f20
    rf_20 = ctx->f[20]; 
    rf_0 = FPU_MUL_S(rf_20, rf_20);
    // 0x10e7bc8: 0x0  nop
    // NOP
    // 0x10e7bcc: 0x0  nop
    // NOP
    // 0x10e7bd0: 0x46010003  div.s       $f0, $f0, $f1
    rf_0 = ps2FpuDivide(ctx->fcr31, rf_0, rf_1);
    ctx->f[0] = rf_0; 
    // 0x10e7bd4: 0x46020034  c.lt.s      $f0, $f2
    ctx->fcr31 = (FPU_C_OLT_S(rf_0, rf_2)) ? (ctx->fcr31 | 0x800000) : (ctx->fcr31 & ~0x800000);
    // 0x10e7bd8: 0x45000010  bc1f        . + 4 + (0x10 << 2)
    {
        const bool branch_taken_0x10e7bd8 = (!(ctx->fcr31 & 0x800000));
        // 0x10e7bdc: 0xc7a200a4  lwc1        $f2, 0xA4($sp) (Delay Slot)
        { uint32_t bits = READ32(ADD32(RC_U32(rc_29), 164)); float f; std::memcpy(&f, &bits, sizeof(f)); rf_2 = f; }
        ctx->f[2] = rf_2; 
        if (branch_taken_0x10e7bd8) {
            goto label_10e7c1c;
        }
    }
    // 0x10e7be0: 0x3c01c0eb  lui         $at, 0xC0EB
    RC_SET_S32(rc_1, (int32_t)((uint32_t)49387 << 16));
    // 0x10e7be4: 0x34213332  ori         $at, $at, 0x3332
    RC_SET_U64(rc_1, RC_U64X(rc_1) | (uint64_t)(uint16_t)13106);
    Ps2SetGpr64(ctx, 1, RC_U64X(rc_1)); 
    // 0x10e7be8: 0x44810000  mtc1        $at, $f0
    { uint32_t bits = RC_U32(rc_1); std::memcpy(&rf_0, &bits, sizeof(bits)); }
    // 0x10e7bec: 0xc6210b54  lwc1        $f1, 0xB54($s1)
    rc_17 = Ps2GprU64(ctx, 17); 
    { uint32_t bits = READ32(ADD32(RC_U32(rc_17), 2900)); float f; std::memcpy(&f, &bits, sizeof(f)); rf_1 = f; }
    // 0x10e7bf0: 0x4600b802  mul.s       $f0, $f23, $f0
    rf_23 = ctx->f[23]; 
    rf_0 = FPU_MUL_S(rf_23, rf_0);
    // 0x10e7bf4: 0x46170842  mul.s       $f1, $f1, $f23
    rf_1 = FPU_MUL_S(rf_1, rf_23);
    // 0x10e7bf8: 0xc7a30084  lwc1        $f3, 0x84($sp)
    { uint32_t bits = READ32(ADD32(RC_U32(rc_29), 132)); float f; std::memcpy(&f, &bits, sizeof(f)); rf_3 = f; }
    ctx->f[3] = rf_3; 
    // 0x10e7bfc: 0x46170002  mul.s       $f0, $f0, $f23
    rf_0 = FPU_MUL_S(rf_0, rf_23);
    ctx->f[0] = rf_0; 
    // 0x10e7c00: 0x46000840  add.s       $f1, $f1, $f0
    rf_1 = FPU_ADD_SF(rf_1, rf_0);
    ctx->f[1] = rf_1; 
    // 0x10e7c04: 0x46011080  add.s       $f2, $f2, $f1
    rf_2 = FPU_ADD_S(rf_2, rf_1);
    ctx->f[2] = rf_2; 
    // 0x10e7c08: 0x46021834  c.lt.s      $f3, $f2
    ctx->fcr31 = (FPU_C_OLT_S(rf_3, rf_2)) ? (ctx->fcr31 | 0x800000) : (ctx->fcr31 & ~0x800000);
    // 0x10e7c0c: 0x45000003  bc1f        . + 4 + (0x3 << 2)
    {
        const bool branch_taken_0x10e7c0c = (!(ctx->fcr31 & 0x800000));
        // 0x10e7c10: 0x24020001  addiu       $v0, $zero, 0x1 (Delay Slot)
        RC_SET_S32(rc_2, (int32_t)ADD32(RC_U32(0), 1));
        Ps2SetGpr64(ctx, 2, RC_U64X(rc_2)); 
        if (branch_taken_0x10e7c0c) {
            goto label_10e7c1c;
        }
    }
    // 0x10e7c14: 0x982d  daddu       $s3, $zero, $zero
    RC_SET_U64(rc_19, (uint64_t)RC_U64(0) + (uint64_t)RC_U64(0));
    Ps2SetGpr64(ctx, 19, RC_U64X(rc_19)); 
    // 0x10e7c18: 0xa22210a6  sb          $v0, 0x10A6($s1)
    WRITE8(ADD32(RC_U32(rc_17), 4262), (uint8_t)RC_U32(rc_2));
label_10e7c1c:
    // 0x10e7c1c: 0x126000e8  beqz        $s3, . + 4 + (0xE8 << 2)
    {
        rc_19 = Ps2GprU64(ctx, 19); 
        const bool branch_taken_0x10e7c1c = (RC_U64(rc_19) == RC_U64(0));
        // 0x10e7c20: 0x27b300b0  addiu       $s3, $sp, 0xB0 (Delay Slot)
        rc_29 = Ps2GprU64(ctx, 29); 
        RC_SET_S32(rc_19, (int32_t)ADD32(RC_U32(rc_29), 176));
        Ps2SetGpr64(ctx, 19, RC_U64X(rc_19)); 
        if (branch_taken_0x10e7c1c) {
            goto label_10e7fc0;
        }
    }
    // 0x10e7c24: 0x27a40040  addiu       $a0, $sp, 0x40
    RC_SET_S32(rc_4, (int32_t)ADD32(RC_U32(rc_29), 64));
    // 0x10e7c28: 0x80282d  daddu       $a1, $a0, $zero
    RC_SET_U64(rc_5, (uint64_t)RC_U64X(rc_4) + (uint64_t)RC_U64(0));
    Ps2SetGpr64(ctx, 5, RC_U64X(rc_5)); 
    // 0x10e7c2c: 0xc44c760  jal         func_1131D80
    RC_SET_U32(rc_31, 0x10E7C34u);
    Ps2SetGpr64(ctx, 31, RC_U64X(rc_31)); 
    // 0x10e7c30: 0x260202d  daddu       $a0, $s3, $zero (Delay Slot)
    RC_SET_U64(rc_4, (uint64_t)RC_U64X(rc_19) + (uint64_t)RC_U64(0));
    Ps2SetGpr64(ctx, 4, RC_U64X(rc_4)); 
    ctx->pc = 0x1131D80u;
    if (!PS2_CALL_GUEST_KNOWN(0x1131D80u, GetGeometryPointerPosition_0x1131d80, 0x10E7C34u, 0x10E7C2Cu, DirectCall, "JAL")) {
        return;
    }
label_10e7c34:
    // 0x10e7c34: 0xda650000  lqc2        $vf5, 0x0($s3)
    rc_19 = Ps2GprU64(ctx, 19); 
    ctx->vu0_vf[5] = _mm_castsi128_ps(READ128(ADD32(RC_U32(rc_19), 0)));
    // 0x10e7c38: 0xdae60000  lqc2        $vf6, 0x0($s7)
    rc_23 = Ps2GprU64(ctx, 23); 
    ctx->vu0_vf[6] = _mm_castsi128_ps(READ128(ADD32(RC_U32(rc_23), 0)));
    // 0x10e7c3c: 0x4bc62968  vadd.xyz    $vf5, $vf5, $vf6
    { __m128 res = PS2_VADD(ctx->vu0_vf[5], ctx->vu0_vf[6]); __m128i mask = _mm_set_epi32(0, -1, -1, -1); ctx->vu0_vf[5] = PS2_VBLEND(ctx->vu0_vf[5], res, _mm_castsi128_ps(mask)); }
    // 0x10e7c40: 0xfa650000  sqc2        $vf5, 0x0($s3)
    WRITE128(ADD32(RC_U32(rc_19), 0), _mm_castps_si128(ctx->vu0_vf[5]));
    // 0x10e7c44: 0x8e350124  lw          $s5, 0x124($s1)
    rc_17 = Ps2GprU64(ctx, 17); 
    RC_SET_S32(rc_21, (int32_t)READ32(ADD32(RC_U32(rc_17), 292)));
    Ps2SetGpr64(ctx, 21, RC_U64X(rc_21)); 
    // 0x10e7c48: 0x12a00003  beqz        $s5, . + 4 + (0x3 << 2)
    {
        const bool branch_taken_0x10e7c48 = (RC_U64X(rc_21) == RC_U64(0));
        // 0x10e7c4c: 0x8fa501ac  lw          $a1, 0x1AC($sp) (Delay Slot)
        rc_29 = Ps2GprU64(ctx, 29); 
        RC_SET_S32(rc_5, (int32_t)READ32(ADD32(RC_U32(rc_29), 428)));
        Ps2SetGpr64(ctx, 5, RC_U64X(rc_5)); 
        if (branch_taken_0x10e7c48) {
            goto label_10e7c58;
        }
    }
    // 0x10e7c50: 0xc44c760  jal         func_1131D80
    RC_SET_U32(rc_31, 0x10E7C58u);
    Ps2SetGpr64(ctx, 31, RC_U64X(rc_31)); 
    // 0x10e7c54: 0x27a400a0  addiu       $a0, $sp, 0xA0 (Delay Slot)
    RC_SET_S32(rc_4, (int32_t)ADD32(RC_U32(rc_29), 160));
    Ps2SetGpr64(ctx, 4, RC_U64X(rc_4)); 
    ctx->pc = 0x1131D80u;
    if (!PS2_CALL_GUEST_KNOWN(0x1131D80u, GetGeometryPointerPosition_0x1131d80, 0x10E7C58u, 0x10E7C50u, DirectCall, "JAL")) {
        return;
    }
label_10e7c58:
    // 0x10e7c58: 0x8fa201a8  lw          $v0, 0x1A8($sp)
    rc_29 = Ps2GprU64(ctx, 29); 
    RC_SET_S32(rc_2, (int32_t)READ32(ADD32(RC_U32(rc_29), 424)));
    Ps2SetGpr64(ctx, 2, RC_U64X(rc_2)); 
    // 0x10e7c5c: 0x10400040  beqz        $v0, . + 4 + (0x40 << 2)
    {
        const bool branch_taken_0x10e7c5c = (RC_U64X(rc_2) == RC_U64(0));
        // 0x10e7c60: 0xa02d  daddu       $s4, $zero, $zero (Delay Slot)
        RC_SET_U64(rc_20, (uint64_t)RC_U64(0) + (uint64_t)RC_U64(0));
        Ps2SetGpr64(ctx, 20, RC_U64X(rc_20)); 
        if (branch_taken_0x10e7c5c) {
            goto label_10e7d60;
        }
    }
    // 0x10e7c64: 0x4600b306  mov.s       $f12, $f22
    rf_22 = ctx->f[22]; 
    rf_12 = FPU_MOV_S(rf_22);
    ctx->f[12] = rf_12; 
    // 0x10e7c68: 0x46196034  c.lt.s      $f12, $f25
    rf_25 = ctx->f[25]; 
    ctx->fcr31 = (FPU_C_OLT_S(rf_12, rf_25)) ? (ctx->fcr31 | 0x800000) : (ctx->fcr31 & ~0x800000);
    // 0x10e7c6c: 0x45020001  bc1fl       . + 4 + (0x1 << 2)
    {
        const bool branch_taken_0x10e7c6c = (!(ctx->fcr31 & 0x800000));
        if (branch_taken_0x10e7c6c) {
            // 0x10e7c70: 0x4600cb06  mov.s       $f12, $f25 (Delay Slot)
            rf_12 = FPU_MOV_S(rf_25);
            ctx->f[12] = rf_12; 
            goto label_10e7c74;
        }
    }
label_10e7c74:
    // 0x10e7c74: 0x7a420000  lq          $v0, 0x0($s2)
    rc_18 = Ps2GprU64(ctx, 18); 
    RC_SET_VEC(ctx, 2, rc_2, READ128(ADD32(RC_U32(rc_18), 0)));
    // 0x10e7c78: 0x27b000c0  addiu       $s0, $sp, 0xC0
    RC_SET_S32(rc_16, (int32_t)ADD32(RC_U32(rc_29), 192));
    Ps2SetGpr64(ctx, 16, RC_U64X(rc_16)); 
    // 0x10e7c7c: 0x7ba300b0  lq          $v1, 0xB0($sp)
    RC_SET_VEC(ctx, 3, rc_3, READ128(ADD32(RC_U32(rc_29), 176)));
    // 0x10e7c80: 0x200202d  daddu       $a0, $s0, $zero
    RC_SET_U64(rc_4, (uint64_t)RC_U64X(rc_16) + (uint64_t)RC_U64(0));
    Ps2SetGpr64(ctx, 4, RC_U64X(rc_4)); 
    // 0x10e7c84: 0x7fa200f0  sq          $v0, 0xF0($sp)
    WRITE128(ADD32(RC_U32(rc_29), 240), GPR_VEC(ctx, 2));
    // 0x10e7c88: 0x27a500f0  addiu       $a1, $sp, 0xF0
    RC_SET_S32(rc_5, (int32_t)ADD32(RC_U32(rc_29), 240));
    Ps2SetGpr64(ctx, 5, RC_U64X(rc_5)); 
    // 0x10e7c8c: 0x7fa30100  sq          $v1, 0x100($sp)
    WRITE128(ADD32(RC_U32(rc_29), 256), GPR_VEC(ctx, 3));
    // 0x10e7c90: 0x27a60100  addiu       $a2, $sp, 0x100
    RC_SET_S32(rc_6, (int32_t)ADD32(RC_U32(rc_29), 256));
    Ps2SetGpr64(ctx, 6, RC_U64X(rc_6)); 
    // 0x10e7c94: 0xc7a20004  lwc1        $f2, 0x4($sp)
    { uint32_t bits = READ32(ADD32(RC_U32(rc_29), 4)); float f; std::memcpy(&f, &bits, sizeof(f)); rf_2 = f; }
    ctx->f[2] = rf_2; 
    // 0x10e7c98: 0x3c0382d  daddu       $a3, $fp, $zero
    rc_30 = Ps2GprU64(ctx, 30); 
    RC_SET_U64(rc_7, (uint64_t)RC_U64(rc_30) + (uint64_t)RC_U64(0));
    Ps2SetGpr64(ctx, 7, rc_7); 
    // 0x10e7c9c: 0xc7a100f4  lwc1        $f1, 0xF4($sp)
    { uint32_t bits = READ32(ADD32(RC_U32(rc_29), 244)); float f; std::memcpy(&f, &bits, sizeof(f)); rf_1 = f; }
    // 0x10e7ca0: 0xc7a00104  lwc1        $f0, 0x104($sp)
    { uint32_t bits = READ32(ADD32(RC_U32(rc_29), 260)); float f; std::memcpy(&f, &bits, sizeof(f)); rf_0 = f; }
    // 0x10e7ca4: 0x46020840  add.s       $f1, $f1, $f2
    rf_1 = FPU_ADD_S(rf_1, rf_2);
    ctx->f[1] = rf_1; 
    // 0x10e7ca8: 0x46020000  add.s       $f0, $f0, $f2
    rf_0 = FPU_ADD_S(rf_0, rf_2);
    ctx->f[0] = rf_0; 
    // 0x10e7cac: 0xe7a100f4  swc1        $f1, 0xF4($sp)
    { float f = rf_1; uint32_t bits; std::memcpy(&bits, &f, sizeof(bits)); WRITE32(ADD32(RC_U32(rc_29), 244), bits); }
    // 0x10e7cb0: 0xc4497e4  jal         func_1125F90
    RC_SET_U32(rc_31, 0x10E7CB8u);
    Ps2SetGpr64(ctx, 31, RC_U64X(rc_31)); 
    // 0x10e7cb4: 0xe7a00104  swc1        $f0, 0x104($sp) (Delay Slot)
    { float f = rf_0; uint32_t bits; std::memcpy(&bits, &f, sizeof(bits)); WRITE32(ADD32(RC_U32(rc_29), 260), bits); }
    ctx->pc = 0x1125F90u;
    if (!PS2_CALL_GUEST_KNOWN(0x1125F90u, ClipCollisionFinderWithKeep_0x1125f90, 0x10E7CB8u, 0x10E7CB0u, DirectCall, "JAL")) {
        return;
    }
label_10e7cb8:
    // 0x10e7cb8: 0x44820800  mtc1        $v0, $f1
    rc_2 = Ps2GprU64(ctx, 2); 
    { uint32_t bits = RC_U32(rc_2); std::memcpy(&rf_1, &bits, sizeof(bits)); }
    // 0x10e7cbc: 0x46800860  cvt.s.w     $f1, $f1
    { int32_t tmp; std::memcpy(&tmp, &rf_1, sizeof(tmp)); rf_1 = FPU_CVT_S_W(tmp); }
    ctx->f[1] = rf_1; 
    // 0x10e7cc0: 0x44800000  mtc1        $zero, $f0
    { uint32_t bits = RC_U32(0); std::memcpy(&rf_0, &bits, sizeof(bits)); }
    ctx->f[0] = rf_0; 
    // 0x10e7cc4: 0x46000832  c.eq.s      $f1, $f0
    ctx->fcr31 = (FPU_C_EQ_S(rf_1, rf_0)) ? (ctx->fcr31 | 0x800000) : (ctx->fcr31 & ~0x800000);
    // 0x10e7cc8: 0x45010025  bc1t        . + 4 + (0x25 << 2)
    {
        const bool branch_taken_0x10e7cc8 = ((ctx->fcr31 & 0x800000));
        // 0x10e7ccc: 0x200302d  daddu       $a2, $s0, $zero (Delay Slot)
        rc_16 = Ps2GprU64(ctx, 16); 
        RC_SET_U64(rc_6, (uint64_t)RC_U64(rc_16) + (uint64_t)RC_U64(0));
        Ps2SetGpr64(ctx, 6, rc_6); 
        if (branch_taken_0x10e7cc8) {
            goto label_10e7d60;
        }
    }
    // 0x10e7cd0: 0xc62c03d8  lwc1        $f12, 0x3D8($s1)
    rc_17 = Ps2GprU64(ctx, 17); 
    { uint32_t bits = READ32(ADD32(RC_U32(rc_17), 984)); float f; std::memcpy(&f, &bits, sizeof(f)); rf_12 = f; }
    // 0x10e7cd4: 0x27b00110  addiu       $s0, $sp, 0x110
    rc_29 = Ps2GprU64(ctx, 29); 
    RC_SET_S32(rc_16, (int32_t)ADD32(RC_U32(rc_29), 272));
    Ps2SetGpr64(ctx, 16, RC_U64X(rc_16)); 
    // 0x10e7cd8: 0x3c0282d  daddu       $a1, $fp, $zero
    rc_30 = Ps2GprU64(ctx, 30); 
    RC_SET_U64(rc_5, (uint64_t)RC_U64(rc_30) + (uint64_t)RC_U64(0));
    Ps2SetGpr64(ctx, 5, rc_5); 
    // 0x10e7cdc: 0x460cc301  sub.s       $f12, $f24, $f12
    rf_24 = ctx->f[24]; 
    rf_12 = FPU_SUB_S(rf_24, rf_12);
    ctx->f[12] = rf_12; 
    // 0x10e7ce0: 0x200202d  daddu       $a0, $s0, $zero
    RC_SET_U64(rc_4, (uint64_t)RC_U64X(rc_16) + (uint64_t)RC_U64(0));
    Ps2SetGpr64(ctx, 4, RC_U64X(rc_4)); 
    // 0x10e7ce4: 0xc4371f0  jal         func_10DC7C0
    RC_SET_U32(rc_31, 0x10E7CECu);
    Ps2SetGpr64(ctx, 31, RC_U64X(rc_31)); 
    // 0x10e7ce8: 0xc0382d  daddu       $a3, $a2, $zero (Delay Slot)
    RC_SET_U64(rc_7, (uint64_t)RC_U64(rc_6) + (uint64_t)RC_U64(0));
    Ps2SetGpr64(ctx, 7, rc_7); 
    ctx->pc = 0x10DC7C0u;
    if (!PS2_CALL_GUEST_KNOWN(0x10DC7C0u, charrootdyn_getLandingSurface_0x10dc7c0, 0x10E7CECu, 0x10E7CE4u, DirectCall, "JAL")) {
        return;
    }
label_10e7cec:
    // 0x10e7cec: 0x1040001c  beqz        $v0, . + 4 + (0x1C << 2)
    {
        rc_2 = Ps2GprU64(ctx, 2); 
        const bool branch_taken_0x10e7cec = (RC_U64(rc_2) == RC_U64(0));
        // 0x10e7cf0: 0x27a60160  addiu       $a2, $sp, 0x160 (Delay Slot)
        rc_29 = Ps2GprU64(ctx, 29); 
        RC_SET_S32(rc_6, (int32_t)ADD32(RC_U32(rc_29), 352));
        Ps2SetGpr64(ctx, 6, RC_U64X(rc_6)); 
        if (branch_taken_0x10e7cec) {
            goto label_10e7d60;
        }
    }
    // 0x10e7cf4: 0x27a20150  addiu       $v0, $sp, 0x150
    RC_SET_S32(rc_2, (int32_t)ADD32(RC_U32(rc_29), 336));
    Ps2SetGpr64(ctx, 2, RC_U64X(rc_2)); 
    // 0x10e7cf8: 0xd8450000  lqc2        $vf5, 0x0($v0)
    ctx->vu0_vf[5] = _mm_castsi128_ps(READ128(ADD32(RC_U32(rc_2), 0)));
    // 0x10e7cfc: 0xdae60000  lqc2        $vf6, 0x0($s7)
    rc_23 = Ps2GprU64(ctx, 23); 
    ctx->vu0_vf[6] = _mm_castsi128_ps(READ128(ADD32(RC_U32(rc_23), 0)));
    // 0x10e7d00: 0x4bc62968  vadd.xyz    $vf5, $vf5, $vf6
    { __m128 res = PS2_VADD(ctx->vu0_vf[5], ctx->vu0_vf[6]); __m128i mask = _mm_set_epi32(0, -1, -1, -1); ctx->vu0_vf[5] = PS2_VBLEND(ctx->vu0_vf[5], res, _mm_castsi128_ps(mask)); }
    // 0x10e7d04: 0xf8c50000  sqc2        $vf5, 0x0($a2)
    WRITE128(ADD32(RC_U32(rc_6), 0), _mm_castps_si128(ctx->vu0_vf[5]));
    // 0x10e7d08: 0xc6e00004  lwc1        $f0, 0x4($s7)
    { uint32_t bits = READ32(ADD32(RC_U32(rc_23), 4)); float f; std::memcpy(&f, &bits, sizeof(f)); rf_0 = f; }
    ctx->f[0] = rf_0; 
    // 0x10e7d0c: 0xc6210e0c  lwc1        $f1, 0xE0C($s1)
    rc_17 = Ps2GprU64(ctx, 17); 
    { uint32_t bits = READ32(ADD32(RC_U32(rc_17), 3596)); float f; std::memcpy(&f, &bits, sizeof(f)); rf_1 = f; }
    ctx->f[1] = rf_1; 
    // 0x10e7d10: 0x46010036  c.le.s      $f0, $f1
    ctx->fcr31 = (FPU_C_OLE_S(rf_0, rf_1)) ? (ctx->fcr31 | 0x800000) : (ctx->fcr31 & ~0x800000);
    // 0x10e7d14: 0x45020001  bc1fl       . + 4 + (0x1 << 2)
    {
        const bool branch_taken_0x10e7d14 = (!(ctx->fcr31 & 0x800000));
        if (branch_taken_0x10e7d14) {
            // 0x10e7d18: 0x46000806  mov.s       $f0, $f1 (Delay Slot)
            rf_0 = FPU_MOV_S(rf_1);
            ctx->f[0] = rf_0; 
            goto label_10e7d1c;
        }
    }
label_10e7d1c:
    // 0x10e7d1c: 0x27a40170  addiu       $a0, $sp, 0x170
    RC_SET_S32(rc_4, (int32_t)ADD32(RC_U32(rc_29), 368));
    Ps2SetGpr64(ctx, 4, RC_U64X(rc_4)); 
    // 0x10e7d20: 0x46000306  mov.s       $f12, $f0
    rf_12 = FPU_MOV_S(rf_0);
    ctx->f[12] = rf_12; 
    // 0x10e7d24: 0x240282d  daddu       $a1, $s2, $zero
    rc_18 = Ps2GprU64(ctx, 18); 
    RC_SET_U64(rc_5, (uint64_t)RC_U64(rc_18) + (uint64_t)RC_U64(0));
    Ps2SetGpr64(ctx, 5, rc_5); 
    // 0x10e7d28: 0xc4497e4  jal         func_1125F90
    RC_SET_U32(rc_31, 0x10E7D30u);
    Ps2SetGpr64(ctx, 31, RC_U64X(rc_31)); 
    // 0x10e7d2c: 0x3c0382d  daddu       $a3, $fp, $zero (Delay Slot)
    rc_30 = Ps2GprU64(ctx, 30); 
    RC_SET_U64(rc_7, (uint64_t)RC_U64(rc_30) + (uint64_t)RC_U64(0));
    Ps2SetGpr64(ctx, 7, rc_7); 
    ctx->pc = 0x1125F90u;
    if (!PS2_CALL_GUEST_KNOWN(0x1125F90u, ClipCollisionFinderWithKeep_0x1125f90, 0x10E7D30u, 0x10E7D28u, DirectCall, "JAL")) {
        return;
    }
label_10e7d30:
    // 0x10e7d30: 0x7ba30160  lq          $v1, 0x160($sp)
    rc_29 = Ps2GprU64(ctx, 29); 
    RC_SET_VEC(ctx, 3, rc_3, READ128(ADD32(RC_U32(rc_29), 352)));
    // 0x10e7d34: 0x10400003  beqz        $v0, . + 4 + (0x3 << 2)
    {
        rc_2 = Ps2GprU64(ctx, 2); 
        const bool branch_taken_0x10e7d34 = (RC_U64(rc_2) == RC_U64(0));
        // 0x10e7d38: 0x7e430000  sq          $v1, 0x0($s2) (Delay Slot)
        rc_18 = Ps2GprU64(ctx, 18); 
        WRITE128(ADD32(RC_U32(rc_18), 0), GPR_VEC(ctx, 3));
        if (branch_taken_0x10e7d34) {
            goto label_10e7d44;
        }
    }
    // 0x10e7d3c: 0xc7a00174  lwc1        $f0, 0x174($sp)
    { uint32_t bits = READ32(ADD32(RC_U32(rc_29), 372)); float f; std::memcpy(&f, &bits, sizeof(f)); rf_0 = f; }
    ctx->f[0] = rf_0; 
    // 0x10e7d40: 0xe6400004  swc1        $f0, 0x4($s2)
    { float f = rf_0; uint32_t bits; std::memcpy(&bits, &f, sizeof(bits)); WRITE32(ADD32(RC_U32(rc_18), 4), bits); }
label_10e7d44:
    // 0x10e7d44: 0x8fa401a0  lw          $a0, 0x1A0($sp)
    RC_SET_S32(rc_4, (int32_t)READ32(ADD32(RC_U32(rc_29), 416)));
    Ps2SetGpr64(ctx, 4, RC_U64X(rc_4)); 
    // 0x10e7d48: 0x200282d  daddu       $a1, $s0, $zero
    rc_16 = Ps2GprU64(ctx, 16); 
    RC_SET_U64(rc_5, (uint64_t)RC_U64(rc_16) + (uint64_t)RC_U64(0));
    Ps2SetGpr64(ctx, 5, rc_5); 
    // 0x10e7d4c: 0xc43c7b8  jal         func_10F1EE0
    RC_SET_U32(rc_31, 0x10E7D54u);
    Ps2SetGpr64(ctx, 31, RC_U64X(rc_31)); 
    // 0x10e7d50: 0x24140001  addiu       $s4, $zero, 0x1 (Delay Slot)
    RC_SET_S32(rc_20, (int32_t)ADD32(RC_U32(0), 1));
    Ps2SetGpr64(ctx, 20, RC_U64X(rc_20)); 
    ctx->pc = 0x10F1EE0u;
    if (!PS2_CALL_GUEST_KNOWN(0x10F1EE0u, ParentLayoutObj_0x10f1ee0, 0x10E7D54u, 0x10E7D4Cu, DirectCall, "JAL")) {
        return;
    }
label_10e7d54:
    // 0x10e7d54: 0x220202d  daddu       $a0, $s1, $zero
    rc_17 = Ps2GprU64(ctx, 17); 
    RC_SET_U64(rc_4, (uint64_t)RC_U64(rc_17) + (uint64_t)RC_U64(0));
    Ps2SetGpr64(ctx, 4, rc_4); 
    // 0x10e7d58: 0xc43709c  jal         func_10DC270
    RC_SET_U32(rc_31, 0x10E7D60u);
    Ps2SetGpr64(ctx, 31, RC_U64X(rc_31)); 
    // 0x10e7d5c: 0x200282d  daddu       $a1, $s0, $zero (Delay Slot)
    rc_16 = Ps2GprU64(ctx, 16); 
    RC_SET_U64(rc_5, (uint64_t)RC_U64(rc_16) + (uint64_t)RC_U64(0));
    Ps2SetGpr64(ctx, 5, rc_5); 
    ctx->pc = 0x10DC270u;
    if (!PS2_CALL_GUEST_KNOWN(0x10DC270u, charrootdyn_getGPPlaneInfo_0x10dc270, 0x10E7D60u, 0x10E7D58u, DirectCall, "JAL")) {
        return;
    }
label_10e7d60:
    // 0x10e7d60: 0x56800033  bnel        $s4, $zero, . + 4 + (0x33 << 2)
    {
        rc_20 = Ps2GprU64(ctx, 20); 
        const bool branch_taken_0x10e7d60 = (RC_U64(rc_20) != RC_U64(0));
        rc_17 = Ps2GprU64(ctx, 17); rf_0 = ctx->f[0]; 
        if (branch_taken_0x10e7d60) {
            // 0x10e7d64: 0xc62006b4  lwc1        $f0, 0x6B4($s1) (Delay Slot)
            { uint32_t bits = READ32(ADD32(RC_U32(rc_17), 1716)); float f; std::memcpy(&f, &bits, sizeof(f)); rf_0 = f; }
            ctx->f[0] = rf_0; 
            goto label_10e7e30;
        }
    }
    // 0x10e7d68: 0xc6e00004  lwc1        $f0, 0x4($s7)
    rc_23 = Ps2GprU64(ctx, 23); 
    { uint32_t bits = READ32(ADD32(RC_U32(rc_23), 4)); float f; std::memcpy(&f, &bits, sizeof(f)); rf_0 = f; }
    ctx->f[0] = rf_0; 
    // 0x10e7d6c: 0xc6210e0c  lwc1        $f1, 0xE0C($s1)
    { uint32_t bits = READ32(ADD32(RC_U32(rc_17), 3596)); float f; std::memcpy(&f, &bits, sizeof(f)); rf_1 = f; }
    ctx->f[1] = rf_1; 
    // 0x10e7d70: 0x46010036  c.le.s      $f0, $f1
    ctx->fcr31 = (FPU_C_OLE_S(rf_0, rf_1)) ? (ctx->fcr31 | 0x800000) : (ctx->fcr31 & ~0x800000);
    // 0x10e7d74: 0x45010002  bc1t        . + 4 + (0x2 << 2)
    {
        const bool branch_taken_0x10e7d74 = ((ctx->fcr31 & 0x800000));
        // 0x10e7d78: 0x27b000c0  addiu       $s0, $sp, 0xC0 (Delay Slot)
        rc_29 = Ps2GprU64(ctx, 29); 
        RC_SET_S32(rc_16, (int32_t)ADD32(RC_U32(rc_29), 192));
        Ps2SetGpr64(ctx, 16, RC_U64X(rc_16)); 
        if (branch_taken_0x10e7d74) {
            goto label_10e7d80;
        }
    }
    // 0x10e7d7c: 0x46000806  mov.s       $f0, $f1
    rf_0 = FPU_MOV_S(rf_1);
    ctx->f[0] = rf_0; 
label_10e7d80:
    // 0x10e7d80: 0x260302d  daddu       $a2, $s3, $zero
    rc_19 = Ps2GprU64(ctx, 19); 
    RC_SET_U64(rc_6, (uint64_t)RC_U64(rc_19) + (uint64_t)RC_U64(0));
    Ps2SetGpr64(ctx, 6, rc_6); 
    // 0x10e7d84: 0x46000306  mov.s       $f12, $f0
    rf_12 = FPU_MOV_S(rf_0);
    ctx->f[12] = rf_12; 
    // 0x10e7d88: 0x200202d  daddu       $a0, $s0, $zero
    RC_SET_U64(rc_4, (uint64_t)RC_U64X(rc_16) + (uint64_t)RC_U64(0));
    Ps2SetGpr64(ctx, 4, RC_U64X(rc_4)); 
    // 0x10e7d8c: 0x240282d  daddu       $a1, $s2, $zero
    rc_18 = Ps2GprU64(ctx, 18); 
    RC_SET_U64(rc_5, (uint64_t)RC_U64(rc_18) + (uint64_t)RC_U64(0));
    Ps2SetGpr64(ctx, 5, rc_5); 
    // 0x10e7d90: 0xc4497e4  jal         func_1125F90
    RC_SET_U32(rc_31, 0x10E7D98u);
    Ps2SetGpr64(ctx, 31, RC_U64X(rc_31)); 
    // 0x10e7d94: 0x3c0382d  daddu       $a3, $fp, $zero (Delay Slot)
    rc_30 = Ps2GprU64(ctx, 30); 
    RC_SET_U64(rc_7, (uint64_t)RC_U64(rc_30) + (uint64_t)RC_U64(0));
    Ps2SetGpr64(ctx, 7, rc_7); 
    ctx->pc = 0x1125F90u;
    if (!PS2_CALL_GUEST_KNOWN(0x1125F90u, ClipCollisionFinderWithKeep_0x1125f90, 0x10E7D98u, 0x10E7D90u, DirectCall, "JAL")) {
        return;
    }
label_10e7d98:
    // 0x10e7d98: 0x7ba300b0  lq          $v1, 0xB0($sp)
    rc_29 = Ps2GprU64(ctx, 29); 
    RC_SET_VEC(ctx, 3, rc_3, READ128(ADD32(RC_U32(rc_29), 176)));
    // 0x10e7d9c: 0x10400003  beqz        $v0, . + 4 + (0x3 << 2)
    {
        rc_2 = Ps2GprU64(ctx, 2); 
        const bool branch_taken_0x10e7d9c = (RC_U64(rc_2) == RC_U64(0));
        // 0x10e7da0: 0x7e430000  sq          $v1, 0x0($s2) (Delay Slot)
        rc_18 = Ps2GprU64(ctx, 18); 
        WRITE128(ADD32(RC_U32(rc_18), 0), GPR_VEC(ctx, 3));
        if (branch_taken_0x10e7d9c) {
            goto label_10e7dac;
        }
    }
    // 0x10e7da4: 0xc7a000c4  lwc1        $f0, 0xC4($sp)
    { uint32_t bits = READ32(ADD32(RC_U32(rc_29), 196)); float f; std::memcpy(&f, &bits, sizeof(f)); rf_0 = f; }
    ctx->f[0] = rf_0; 
    // 0x10e7da8: 0xe6400004  swc1        $f0, 0x4($s2)
    { float f = rf_0; uint32_t bits; std::memcpy(&bits, &f, sizeof(bits)); WRITE32(ADD32(RC_U32(rc_18), 4), bits); }
label_10e7dac:
    // 0x10e7dac: 0x27a30040  addiu       $v1, $sp, 0x40
    RC_SET_S32(rc_3, (int32_t)ADD32(RC_U32(rc_29), 64));
    Ps2SetGpr64(ctx, 3, RC_U64X(rc_3)); 
    // 0x10e7db0: 0x8fa401a0  lw          $a0, 0x1A0($sp)
    RC_SET_S32(rc_4, (int32_t)READ32(ADD32(RC_U32(rc_29), 416)));
    Ps2SetGpr64(ctx, 4, RC_U64X(rc_4)); 
    // 0x10e7db4: 0xc43c7b8  jal         func_10F1EE0
    RC_SET_U32(rc_31, 0x10E7DBCu);
    Ps2SetGpr64(ctx, 31, RC_U64X(rc_31)); 
    // 0x10e7db8: 0x60282d  daddu       $a1, $v1, $zero (Delay Slot)
    RC_SET_U64(rc_5, (uint64_t)RC_U64X(rc_3) + (uint64_t)RC_U64(0));
    Ps2SetGpr64(ctx, 5, RC_U64X(rc_5)); 
    ctx->pc = 0x10F1EE0u;
    if (!PS2_CALL_GUEST_KNOWN(0x10F1EE0u, ParentLayoutObj_0x10f1ee0, 0x10E7DBCu, 0x10E7DB4u, DirectCall, "JAL")) {
        return;
    }
label_10e7dbc:
    // 0x10e7dbc: 0x220202d  daddu       $a0, $s1, $zero
    rc_17 = Ps2GprU64(ctx, 17); 
    RC_SET_U64(rc_4, (uint64_t)RC_U64(rc_17) + (uint64_t)RC_U64(0));
    Ps2SetGpr64(ctx, 4, rc_4); 
    // 0x10e7dc0: 0x27a20040  addiu       $v0, $sp, 0x40
    rc_29 = Ps2GprU64(ctx, 29); 
    RC_SET_S32(rc_2, (int32_t)ADD32(RC_U32(rc_29), 64));
    Ps2SetGpr64(ctx, 2, RC_U64X(rc_2)); 
    // 0x10e7dc4: 0xc43709c  jal         func_10DC270
    RC_SET_U32(rc_31, 0x10E7DCCu);
    Ps2SetGpr64(ctx, 31, RC_U64X(rc_31)); 
    // 0x10e7dc8: 0x40282d  daddu       $a1, $v0, $zero (Delay Slot)
    RC_SET_U64(rc_5, (uint64_t)RC_U64X(rc_2) + (uint64_t)RC_U64(0));
    Ps2SetGpr64(ctx, 5, RC_U64X(rc_5)); 
    ctx->pc = 0x10DC270u;
    if (!PS2_CALL_GUEST_KNOWN(0x10DC270u, charrootdyn_getGPPlaneInfo_0x10dc270, 0x10E7DCCu, 0x10E7DC4u, DirectCall, "JAL")) {
        return;
    }
label_10e7dcc:
    // 0x10e7dcc: 0x27a30040  addiu       $v1, $sp, 0x40
    rc_29 = Ps2GprU64(ctx, 29); 
    RC_SET_S32(rc_3, (int32_t)ADD32(RC_U32(rc_29), 64));
    Ps2SetGpr64(ctx, 3, RC_U64X(rc_3)); 
    // 0x10e7dd0: 0xc44ca14  jal         func_1132850
    RC_SET_U32(rc_31, 0x10E7DD8u);
    Ps2SetGpr64(ctx, 31, RC_U64X(rc_31)); 
    // 0x10e7dd4: 0x60202d  daddu       $a0, $v1, $zero (Delay Slot)
    RC_SET_U64(rc_4, (uint64_t)RC_U64X(rc_3) + (uint64_t)RC_U64(0));
    Ps2SetGpr64(ctx, 4, RC_U64X(rc_4)); 
    ctx->pc = 0x1132850u;
    if (!PS2_CALL_GUEST_KNOWN(0x1132850u, GetGeometryPointerSurfacePlane_0x1132850, 0x10E7DD8u, 0x10E7DD0u, DirectCall, "JAL")) {
        return;
    }
label_10e7dd8:
    // 0x10e7dd8: 0x4600d007  neg.s       $f0, $f26
    rf_26 = ctx->f[26]; 
    rf_0 = FPU_NEG_S(rf_26);
    ctx->f[0] = rf_0; 
    // 0x10e7ddc: 0x44040000  mfc1        $a0, $f0
    { uint32_t bits; std::memcpy(&bits, &rf_0, sizeof(bits)); RC_SET_U32(rc_4, bits); }
    // 0x10e7de0: 0xd8450000  lqc2        $vf5, 0x0($v0)
    rc_2 = Ps2GprU64(ctx, 2); 
    ctx->vu0_vf[5] = _mm_castsi128_ps(READ128(ADD32(RC_U32(rc_2), 0)));
    // 0x10e7de4: 0x48a43000  qmtc2.ni    $a0, $vf6
    ctx->vu0_vf[6] = _mm_castsi128_ps(RC_VECX(ctx, 4, rc_4));
    // 0x10e7de8: 0x4bc62958  vmulx.xyz   $vf5, $vf5, $vf6x
    { __m128 res = PS2_VMUL(ctx->vu0_vf[5], _mm_shuffle_ps(ctx->vu0_vf[6], ctx->vu0_vf[6], _MM_SHUFFLE(0,0,0,0))); __m128i mask = _mm_set_epi32(0, -1, -1, -1); ctx->vu0_vf[5] = _mm_blendv_ps(ctx->vu0_vf[5], res, _mm_castsi128_ps(mask)); }
    // 0x10e7dec: 0xfa050000  sqc2        $vf5, 0x0($s0)
    rc_16 = Ps2GprU64(ctx, 16); 
    WRITE128(ADD32(RC_U32(rc_16), 0), _mm_castps_si128(ctx->vu0_vf[5]));
    // 0x10e7df0: 0x27a600d0  addiu       $a2, $sp, 0xD0
    rc_29 = Ps2GprU64(ctx, 29); 
    RC_SET_S32(rc_6, (int32_t)ADD32(RC_U32(rc_29), 208));
    Ps2SetGpr64(ctx, 6, RC_U64X(rc_6)); 
    // 0x10e7df4: 0xda450000  lqc2        $vf5, 0x0($s2)
    rc_18 = Ps2GprU64(ctx, 18); 
    ctx->vu0_vf[5] = _mm_castsi128_ps(READ128(ADD32(RC_U32(rc_18), 0)));
    // 0x10e7df8: 0xda060000  lqc2        $vf6, 0x0($s0)
    ctx->vu0_vf[6] = _mm_castsi128_ps(READ128(ADD32(RC_U32(rc_16), 0)));
    // 0x10e7dfc: 0x4bc62968  vadd.xyz    $vf5, $vf5, $vf6
    { __m128 res = PS2_VADD(ctx->vu0_vf[5], ctx->vu0_vf[6]); __m128i mask = _mm_set_epi32(0, -1, -1, -1); ctx->vu0_vf[5] = PS2_VBLEND(ctx->vu0_vf[5], res, _mm_castsi128_ps(mask)); }
    // 0x10e7e00: 0xf8c50000  sqc2        $vf5, 0x0($a2)
    WRITE128(ADD32(RC_U32(rc_6), 0), _mm_castps_si128(ctx->vu0_vf[5]));
    // 0x10e7e04: 0x27b000e0  addiu       $s0, $sp, 0xE0
    RC_SET_S32(rc_16, (int32_t)ADD32(RC_U32(rc_29), 224));
    Ps2SetGpr64(ctx, 16, RC_U64X(rc_16)); 
    // 0x10e7e08: 0x240282d  daddu       $a1, $s2, $zero
    RC_SET_U64(rc_5, (uint64_t)RC_U64(rc_18) + (uint64_t)RC_U64(0));
    Ps2SetGpr64(ctx, 5, rc_5); 
    // 0x10e7e0c: 0x200202d  daddu       $a0, $s0, $zero
    RC_SET_U64(rc_4, (uint64_t)RC_U64X(rc_16) + (uint64_t)RC_U64(0));
    Ps2SetGpr64(ctx, 4, RC_U64X(rc_4)); 
    // 0x10e7e10: 0xc449772  jal         func_1125DC8
    RC_SET_U32(rc_31, 0x10E7E18u);
    Ps2SetGpr64(ctx, 31, RC_U64X(rc_31)); 
    // 0x10e7e14: 0x3c0382d  daddu       $a3, $fp, $zero (Delay Slot)
    rc_30 = Ps2GprU64(ctx, 30); 
    RC_SET_U64(rc_7, (uint64_t)RC_U64(rc_30) + (uint64_t)RC_U64(0));
    Ps2SetGpr64(ctx, 7, rc_7); 
    ctx->pc = 0x1125DC8u;
    if (!PS2_CALL_GUEST_KNOWN(0x1125DC8u, ClipCollisionFinder_0x1125dc8, 0x10E7E18u, 0x10E7E10u, DirectCall, "JAL")) {
        return;
    }
label_10e7e18:
    // 0x10e7e18: 0x10400004  beqz        $v0, . + 4 + (0x4 << 2)
    {
        rc_2 = Ps2GprU64(ctx, 2); 
        const bool branch_taken_0x10e7e18 = (RC_U64(rc_2) == RC_U64(0));
        // 0x10e7e1c: 0x200282d  daddu       $a1, $s0, $zero (Delay Slot)
        rc_16 = Ps2GprU64(ctx, 16); 
        RC_SET_U64(rc_5, (uint64_t)RC_U64(rc_16) + (uint64_t)RC_U64(0));
        Ps2SetGpr64(ctx, 5, rc_5); 
        if (branch_taken_0x10e7e18) {
            goto label_10e7e2c;
        }
    }
    // 0x10e7e20: 0x44806000  mtc1        $zero, $f12
    { uint32_t bits = RC_U32(0); std::memcpy(&rf_12, &bits, sizeof(bits)); }
    ctx->f[12] = rf_12; 
    // 0x10e7e24: 0xc4371d8  jal         func_10DC760
    RC_SET_U32(rc_31, 0x10E7E2Cu);
    Ps2SetGpr64(ctx, 31, RC_U64X(rc_31)); 
    // 0x10e7e28: 0x220202d  daddu       $a0, $s1, $zero (Delay Slot)
    rc_17 = Ps2GprU64(ctx, 17); 
    RC_SET_U64(rc_4, (uint64_t)RC_U64(rc_17) + (uint64_t)RC_U64(0));
    Ps2SetGpr64(ctx, 4, rc_4); 
    ctx->pc = 0x10DC760u;
    if (!PS2_CALL_GUEST_KNOWN(0x10DC760u, charrootdyn_setHColFindStatus_0x10dc760, 0x10E7E2Cu, 0x10E7E24u, DirectCall, "JAL")) {
        return;
    }
label_10e7e2c:
    // 0x10e7e2c: 0xc62006b4  lwc1        $f0, 0x6B4($s1)
    rc_17 = Ps2GprU64(ctx, 17); 
    { uint32_t bits = READ32(ADD32(RC_U32(rc_17), 1716)); float f; std::memcpy(&f, &bits, sizeof(f)); rf_0 = f; }
    ctx->f[0] = rf_0; 
label_10e7e30:
    // 0x10e7e30: 0xc62106bc  lwc1        $f1, 0x6BC($s1)
    rc_17 = Ps2GprU64(ctx, 17); 
    { uint32_t bits = READ32(ADD32(RC_U32(rc_17), 1724)); float f; std::memcpy(&f, &bits, sizeof(f)); rf_1 = f; }
    ctx->f[1] = rf_1; 
    // 0x10e7e34: 0xe62006b8  swc1        $f0, 0x6B8($s1)
    rf_0 = ctx->f[0]; 
    { float f = rf_0; uint32_t bits; std::memcpy(&bits, &f, sizeof(bits)); WRITE32(ADD32(RC_U32(rc_17), 1720), bits); }
    // 0x10e7e38: 0xe62106c0  swc1        $f1, 0x6C0($s1)
    { float f = rf_1; uint32_t bits; std::memcpy(&bits, &f, sizeof(bits)); WRITE32(ADD32(RC_U32(rc_17), 1728), bits); }
    // 0x10e7e3c: 0x8fa301b4  lw          $v1, 0x1B4($sp)
    rc_29 = Ps2GprU64(ctx, 29); 
    RC_SET_S32(rc_3, (int32_t)READ32(ADD32(RC_U32(rc_29), 436)));
    Ps2SetGpr64(ctx, 3, RC_U64X(rc_3)); 
    // 0x10e7e40: 0x8c620068  lw          $v0, 0x68($v1)
    RC_SET_S32(rc_2, (int32_t)READ32(ADD32(RC_U32(rc_3), 104)));
    // 0x10e7e44: 0x215c2  srl         $v0, $v0, 23
    RC_SET_S32(rc_2, (int32_t)SRL32(RC_U32(rc_2), 23));
    // 0x10e7e48: 0x30420001  andi        $v0, $v0, 0x1
    RC_SET_U64(rc_2, RC_U64X(rc_2) & (uint64_t)(uint16_t)1);
    Ps2SetGpr64(ctx, 2, RC_U64X(rc_2)); 
    // 0x10e7e4c: 0x1040004e  beqz        $v0, . + 4 + (0x4E << 2)
    {
        const bool branch_taken_0x10e7e4c = (RC_U64X(rc_2) == RC_U64(0));
        // 0x10e7e50: 0x8fa601ac  lw          $a2, 0x1AC($sp) (Delay Slot)
        RC_SET_S32(rc_6, (int32_t)READ32(ADD32(RC_U32(rc_29), 428)));
        Ps2SetGpr64(ctx, 6, RC_U64X(rc_6)); 
        if (branch_taken_0x10e7e4c) {
            goto label_10e7f88;
        }
    }
    // 0x10e7e54: 0x8fa501ac  lw          $a1, 0x1AC($sp)
    RC_SET_S32(rc_5, (int32_t)READ32(ADD32(RC_U32(rc_29), 428)));
    Ps2SetGpr64(ctx, 5, RC_U64X(rc_5)); 
    // 0x10e7e58: 0xc44c760  jal         func_1131D80
    RC_SET_U32(rc_31, 0x10E7E60u);
    Ps2SetGpr64(ctx, 31, RC_U64X(rc_31)); 
    // 0x10e7e5c: 0x27a400c0  addiu       $a0, $sp, 0xC0 (Delay Slot)
    RC_SET_S32(rc_4, (int32_t)ADD32(RC_U32(rc_29), 192));
    Ps2SetGpr64(ctx, 4, RC_U64X(rc_4)); 
    ctx->pc = 0x1131D80u;
    if (!PS2_CALL_GUEST_KNOWN(0x1131D80u, GetGeometryPointerPosition_0x1131d80, 0x10E7E60u, 0x10E7E58u, DirectCall, "JAL")) {
        return;
    }
label_10e7e60:
    // 0x10e7e60: 0x8e23013c  lw          $v1, 0x13C($s1)
    rc_17 = Ps2GprU64(ctx, 17); 
    RC_SET_S32(rc_3, (int32_t)READ32(ADD32(RC_U32(rc_17), 316)));
    // 0x10e7e64: 0x27a20110  addiu       $v0, $sp, 0x110
    rc_29 = Ps2GprU64(ctx, 29); 
    RC_SET_S32(rc_2, (int32_t)ADD32(RC_U32(rc_29), 272));
    // 0x10e7e68: 0x8e240134  lw          $a0, 0x134($s1)
    RC_SET_S32(rc_4, (int32_t)READ32(ADD32(RC_U32(rc_17), 308)));
    Ps2SetGpr64(ctx, 4, RC_U64X(rc_4)); 
    // 0x10e7e6c: 0x4bc00303  vaddw.xyz   $vf12, $vf0, $vf0w
    { __m128 res = PS2_VADD_K(1, 1, ctx->vu0_vf[0], _mm_shuffle_ps(ctx->vu0_vf[0], ctx->vu0_vf[0], _MM_SHUFFLE(3,3,3,3))); __m128i mask = _mm_set_epi32(0, -1, -1, -1); ctx->vu0_vf[12] = _mm_blendv_ps(ctx->vu0_vf[12], res, _mm_castsi128_ps(mask)); }
    // 0x10e7e70: 0xd86b0000  lqc2        $vf11, 0x0($v1)
    ctx->vu0_vf[11] = _mm_castsi128_ps(READ128(ADD32(RC_U32(rc_3), 0)));
    // 0x10e7e74: 0x4a2e033d  vmr32.w     $vf14, $vf0
    { __m128 res = _mm_shuffle_ps(ctx->vu0_vf[0], ctx->vu0_vf[0], _MM_SHUFFLE(0,3,2,1)); __m128i mask = _mm_set_epi32(-1, 0, 0, 0); ctx->vu0_vf[14] = _mm_blendv_ps(ctx->vu0_vf[14], res, _mm_castsi128_ps(mask)); }
    // 0x10e7e78: 0x4a2f033d  vmr32.w     $vf15, $vf0
    { __m128 res = _mm_shuffle_ps(ctx->vu0_vf[0], ctx->vu0_vf[0], _MM_SHUFFLE(0,3,2,1)); __m128i mask = _mm_set_epi32(-1, 0, 0, 0); ctx->vu0_vf[15] = _mm_blendv_ps(ctx->vu0_vf[15], res, _mm_castsi128_ps(mask)); }
    // 0x10e7e7c: 0x4a30033d  vmr32.w     $vf16, $vf0
    { __m128 res = _mm_shuffle_ps(ctx->vu0_vf[0], ctx->vu0_vf[0], _MM_SHUFFLE(0,3,2,1)); __m128i mask = _mm_set_epi32(-1, 0, 0, 0); ctx->vu0_vf[16] = _mm_blendv_ps(ctx->vu0_vf[16], res, _mm_castsi128_ps(mask)); }
    // 0x10e7e80: 0x4bcc5afe  vopmula.xyz $ACC, $vf11, $vf12
    { __m128 fs_yzx = _mm_shuffle_ps(ctx->vu0_vf[11], ctx->vu0_vf[11], _MM_SHUFFLE(3,0,2,1)); __m128 ft_zxy = _mm_shuffle_ps(ctx->vu0_vf[12], ctx->vu0_vf[12], _MM_SHUFFLE(3,1,0,2)); __m128 res = PS2_VMUL_K(0, 1, fs_yzx, ft_zxy); ctx->vu0_acc = _mm_blendv_ps(ctx->vu0_acc, res, _mm_castsi128_ps(_mm_set_epi32(0, -1, -1, -1))); }
    // 0x10e7e84: 0x4bc002e9  vmadd.xyz   $vf11, $vf0, $vf0
    { __m128 mul_res = PS2_VMUL_K(1, 1, ctx->vu0_vf[0], ctx->vu0_vf[0]); __m128 res = PS2_VADD_K(1, 1, ctx->vu0_acc, mul_res); __m128i mask = _mm_set_epi32(0, -1, -1, -1); ctx->vu0_vf[11] = _mm_blendv_ps(ctx->vu0_vf[11], res, _mm_castsi128_ps(mask)); }
    // 0x10e7e88: 0x4bcc5aa9  vmadd.xyz   $vf10, $vf11, $vf12
    { __m128 mul_res = PS2_VMUL_K(1, 1, ctx->vu0_vf[11], ctx->vu0_vf[12]); __m128 res = PS2_VADD_K(1, 1, ctx->vu0_acc, mul_res); __m128i mask = _mm_set_epi32(0, -1, -1, -1); ctx->vu0_vf[10] = _mm_blendv_ps(ctx->vu0_vf[10], res, _mm_castsi128_ps(mask)); }
    // 0x10e7e8c: 0x4bca5b6a  vmul.xyz    $vf13, $vf11, $vf10
    { __m128 res = PS2_VMUL_K(1, 1, ctx->vu0_vf[11], ctx->vu0_vf[10]); __m128i mask = _mm_set_epi32(0, -1, -1, -1); ctx->vu0_vf[13] = PS2_VBLEND(ctx->vu0_vf[13], res, _mm_castsi128_ps(mask)); }
    // 0x10e7e90: 0x4bca5afe  vopmula.xyz $ACC, $vf11, $vf10
    { __m128 fs_yzx = _mm_shuffle_ps(ctx->vu0_vf[11], ctx->vu0_vf[11], _MM_SHUFFLE(3,0,2,1)); __m128 ft_zxy = _mm_shuffle_ps(ctx->vu0_vf[10], ctx->vu0_vf[10], _MM_SHUFFLE(3,1,0,2)); __m128 res = PS2_VMUL_K(1, 1, fs_yzx, ft_zxy); ctx->vu0_acc = _mm_blendv_ps(ctx->vu0_acc, res, _mm_castsi128_ps(_mm_set_epi32(0, -1, -1, -1))); }
    // 0x10e7e94: 0x4bcb53cb  vmaddw.xyz  $vf15, $vf10, $vf11w
    { __m128 mul_res = PS2_VMUL_K(1, 0, ctx->vu0_vf[10], _mm_shuffle_ps(ctx->vu0_vf[11], ctx->vu0_vf[11], _MM_SHUFFLE(3,3,3,3))); __m128 res = PS2_VADD_K(1, 1, ctx->vu0_acc, mul_res); __m128i mask = _mm_set_epi32(0, -1, -1, -1); ctx->vu0_vf[15] = _mm_blendv_ps(ctx->vu0_vf[15], res, _mm_castsi128_ps(mask)); }
    // 0x10e7e98: 0x4bcb540f  vmsubw.xyz  $vf16, $vf10, $vf11w
    { __m128 mul_res = PS2_VMUL_K(1, 0, ctx->vu0_vf[10], _mm_shuffle_ps(ctx->vu0_vf[11], ctx->vu0_vf[11], _MM_SHUFFLE(3,3,3,3))); __m128 res = PS2_VSUB_K(1, 1, ctx->vu0_acc, mul_res); __m128i mask = _mm_set_epi32(0, -1, -1, -1); ctx->vu0_vf[16] = _mm_blendv_ps(ctx->vu0_vf[16], res, _mm_castsi128_ps(mask)); }
    // 0x10e7e9c: 0x4bcc6afe  vopmula.xyz $ACC, $vf13, $vf12
    { __m128 fs_yzx = _mm_shuffle_ps(ctx->vu0_vf[13], ctx->vu0_vf[13], _MM_SHUFFLE(3,0,2,1)); __m128 ft_zxy = _mm_shuffle_ps(ctx->vu0_vf[12], ctx->vu0_vf[12], _MM_SHUFFLE(3,1,0,2)); __m128 res = PS2_VMUL_K(1, 1, fs_yzx, ft_zxy); ctx->vu0_acc = _mm_blendv_ps(ctx->vu0_acc, res, _mm_castsi128_ps(_mm_set_epi32(0, -1, -1, -1))); }
    // 0x10e7ea0: 0x4bcc6c69  vmadd.xyz   $vf17, $vf13, $vf12
    { __m128 mul_res = PS2_VMUL_K(1, 1, ctx->vu0_vf[13], ctx->vu0_vf[12]); __m128 res = PS2_VADD_K(1, 1, ctx->vu0_acc, mul_res); __m128i mask = _mm_set_epi32(0, -1, -1, -1); ctx->vu0_vf[17] = _mm_blendv_ps(ctx->vu0_vf[17], res, _mm_castsi128_ps(mask)); }
    // 0x10e7ea4: 0x4bcc7afe  vopmula.xyz $ACC, $vf15, $vf12
    { __m128 fs_yzx = _mm_shuffle_ps(ctx->vu0_vf[15], ctx->vu0_vf[15], _MM_SHUFFLE(3,0,2,1)); __m128 ft_zxy = _mm_shuffle_ps(ctx->vu0_vf[12], ctx->vu0_vf[12], _MM_SHUFFLE(3,1,0,2)); __m128 res = PS2_VMUL_K(1, 1, fs_yzx, ft_zxy); ctx->vu0_acc = _mm_blendv_ps(ctx->vu0_acc, res, _mm_castsi128_ps(_mm_set_epi32(0, -1, -1, -1))); }
    // 0x10e7ea8: 0x4bc003e9  vmadd.xyz   $vf15, $vf0, $vf0
    { __m128 mul_res = PS2_VMUL_K(1, 1, ctx->vu0_vf[0], ctx->vu0_vf[0]); __m128 res = PS2_VADD_K(1, 1, ctx->vu0_acc, mul_res); __m128i mask = _mm_set_epi32(0, -1, -1, -1); ctx->vu0_vf[15] = _mm_blendv_ps(ctx->vu0_vf[15], res, _mm_castsi128_ps(mask)); }
    // 0x10e7eac: 0x4bd163ac  vsub.xyz    $vf14, $vf12, $vf17
    { __m128 res = PS2_VSUB_K(1, 1, ctx->vu0_vf[12], ctx->vu0_vf[17]); __m128i mask = _mm_set_epi32(0, -1, -1, -1); ctx->vu0_vf[14] = PS2_VBLEND(ctx->vu0_vf[14], res, _mm_castsi128_ps(mask)); }
    // 0x10e7eb0: 0x4ad1733c  vmove.yz    $vf17, $vf14
    { __m128i mask = _mm_set_epi32(0, -1, -1, 0); ctx->vu0_vf[17] = _mm_blendv_ps(ctx->vu0_vf[17], ctx->vu0_vf[14], _mm_castsi128_ps(mask)); }
    // 0x10e7eb4: 0x4a8e833c  vmove.y     $vf14, $vf16
    { __m128i mask = _mm_set_epi32(0, 0, -1, 0); ctx->vu0_vf[14] = _mm_blendv_ps(ctx->vu0_vf[14], ctx->vu0_vf[16], _mm_castsi128_ps(mask)); }
    // 0x10e7eb8: 0x4a907b3c  vmove.y     $vf16, $vf15
    { __m128i mask = _mm_set_epi32(0, 0, -1, 0); ctx->vu0_vf[16] = _mm_blendv_ps(ctx->vu0_vf[16], ctx->vu0_vf[15], _mm_castsi128_ps(mask)); }
    // 0x10e7ebc: 0x4a8f8b3c  vmove.y     $vf15, $vf17
    { __m128i mask = _mm_set_epi32(0, 0, -1, 0); ctx->vu0_vf[15] = _mm_blendv_ps(ctx->vu0_vf[15], ctx->vu0_vf[17], _mm_castsi128_ps(mask)); }
    // 0x10e7ec0: 0x4a4e7b3c  vmove.z     $vf14, $vf15
    { __m128i mask = _mm_set_epi32(0, -1, 0, 0); ctx->vu0_vf[14] = _mm_blendv_ps(ctx->vu0_vf[14], ctx->vu0_vf[15], _mm_castsi128_ps(mask)); }
    // 0x10e7ec4: 0x4a4f833c  vmove.z     $vf15, $vf16
    { __m128i mask = _mm_set_epi32(0, -1, 0, 0); ctx->vu0_vf[15] = _mm_blendv_ps(ctx->vu0_vf[15], ctx->vu0_vf[16], _mm_castsi128_ps(mask)); }
    // 0x10e7ec8: 0x4a508b3c  vmove.z     $vf16, $vf17
    { __m128i mask = _mm_set_epi32(0, -1, 0, 0); ctx->vu0_vf[16] = _mm_blendv_ps(ctx->vu0_vf[16], ctx->vu0_vf[17], _mm_castsi128_ps(mask)); }
    // 0x10e7ecc: 0xf84e0000  sqc2        $vf14, 0x0($v0)
    WRITE128(ADD32(RC_U32(rc_2), 0), _mm_castps_si128(ctx->vu0_vf[14]));
    // 0x10e7ed0: 0xf84f0010  sqc2        $vf15, 0x10($v0)
    WRITE128(ADD32(RC_U32(rc_2), 16), _mm_castps_si128(ctx->vu0_vf[15]));
    // 0x10e7ed4: 0xf8500020  sqc2        $vf16, 0x20($v0)
    WRITE128(ADD32(RC_U32(rc_2), 32), _mm_castps_si128(ctx->vu0_vf[16]));
    // 0x10e7ed8: 0xd8840000  lqc2        $vf4, 0x0($a0)
    ctx->vu0_vf[4] = _mm_castsi128_ps(READ128(ADD32(RC_U32(rc_4), 0)));
    // 0x10e7edc: 0x4a24033c  vmove.w     $vf4, $vf0
    { __m128i mask = _mm_set_epi32(-1, 0, 0, 0); ctx->vu0_vf[4] = _mm_blendv_ps(ctx->vu0_vf[4], ctx->vu0_vf[0], _mm_castsi128_ps(mask)); }
    // 0x10e7ee0: 0xf8440030  sqc2        $vf4, 0x30($v0)
    WRITE128(ADD32(RC_U32(rc_2), 48), _mm_castps_si128(ctx->vu0_vf[4]));
    // 0x10e7ee4: 0x27a300d0  addiu       $v1, $sp, 0xD0
    RC_SET_S32(rc_3, (int32_t)ADD32(RC_U32(rc_29), 208));
    Ps2SetGpr64(ctx, 3, RC_U64X(rc_3)); 
    // 0x10e7ee8: 0x27a20110  addiu       $v0, $sp, 0x110
    RC_SET_S32(rc_2, (int32_t)ADD32(RC_U32(rc_29), 272));
    Ps2SetGpr64(ctx, 2, RC_U64X(rc_2)); 
    // 0x10e7eec: 0xd8580030  lqc2        $vf24, 0x30($v0)
    ctx->vu0_vf[24] = _mm_castsi128_ps(READ128(ADD32(RC_U32(rc_2), 48)));
    // 0x10e7ef0: 0x4b000647  vsubw.x     $vf25, $vf0, $vf0w
    { __m128 res = PS2_VSUB_K(1, 1, ctx->vu0_vf[0], _mm_shuffle_ps(ctx->vu0_vf[0], ctx->vu0_vf[0], _MM_SHUFFLE(3,3,3,3))); __m128i mask = _mm_set_epi32(0, 0, 0, -1); ctx->vu0_vf[25] = _mm_blendv_ps(ctx->vu0_vf[25], res, _mm_castsi128_ps(mask)); }
    // 0x10e7ef4: 0x4bd9c718  vmulx.xyz   $vf28, $vf24, $vf25x
    { __m128 res = PS2_VMUL_K(0, 1, ctx->vu0_vf[24], _mm_shuffle_ps(ctx->vu0_vf[25], ctx->vu0_vf[25], _MM_SHUFFLE(0,0,0,0))); __m128i mask = _mm_set_epi32(0, -1, -1, -1); ctx->vu0_vf[28] = _mm_blendv_ps(ctx->vu0_vf[28], res, _mm_castsi128_ps(mask)); }
    // 0x10e7ef8: 0x4a3ce72c  vsub.w      $vf28, $vf28, $vf28
    { __m128 res = PS2_VSUB(ctx->vu0_vf[28], ctx->vu0_vf[28]); __m128i mask = _mm_set_epi32(-1, 0, 0, 0); ctx->vu0_vf[28] = PS2_VBLEND(ctx->vu0_vf[28], res, _mm_castsi128_ps(mask)); }
    // 0x10e7efc: 0x78480000  lq          $t0, 0x0($v0)
    RC_SET_VEC(ctx, 8, rc_8, READ128(ADD32(RC_U32(rc_2), 0)));
    // 0x10e7f00: 0x78490010  lq          $t1, 0x10($v0)
    RC_SET_VEC(ctx, 9, rc_9, READ128(ADD32(RC_U32(rc_2), 16)));
    // 0x10e7f04: 0x784a0020  lq          $t2, 0x20($v0)
    RC_SET_VEC(ctx, 10, rc_10, READ128(ADD32(RC_U32(rc_2), 32)));
    // 0x10e7f08: 0x482b0000  qmfc2.ni    $t3, $vf0
    RC_SET_VEC(ctx, 11, rc_11, _mm_castps_si128(ctx->vu0_vf[0]));
    // 0x10e7f0c: 0x71286488  pextlw      $t4, $t1, $t0
    RC_SET_VEC(ctx, 12, rc_12, PS2_PEXTLW(GPR_VEC(ctx, 9), GPR_VEC(ctx, 8)));
    // 0x10e7f10: 0x71286ca8  pextuw      $t5, $t1, $t0
    RC_SET_VEC(ctx, 13, rc_13, PS2_PEXTUW(GPR_VEC(ctx, 9), GPR_VEC(ctx, 8)));
    // 0x10e7f14: 0x716a7488  pextlw      $t6, $t3, $t2
    RC_SET_VEC(ctx, 14, rc_14, PS2_PEXTLW(GPR_VEC(ctx, 11), GPR_VEC(ctx, 10)));
    // 0x10e7f18: 0x716a7ca8  pextuw      $t7, $t3, $t2
    RC_SET_VEC(ctx, 15, rc_15, PS2_PEXTUW(GPR_VEC(ctx, 11), GPR_VEC(ctx, 10)));
    // 0x10e7f1c: 0x71cc4389  pcpyld      $t0, $t6, $t4
    RC_SET_VEC(ctx, 8, rc_8, PS2_PCPYLD(GPR_VEC(ctx, 14), GPR_VEC(ctx, 12)));
    // 0x10e7f20: 0x718e4ba9  pcpyud      $t1, $t4, $t6
    RC_SET_VEC(ctx, 9, rc_9, _mm_unpackhi_epi64(GPR_VEC(ctx, 12), GPR_VEC(ctx, 14)));
    // 0x10e7f24: 0x71ed5389  pcpyld      $t2, $t7, $t5
    RC_SET_VEC(ctx, 10, rc_10, PS2_PCPYLD(GPR_VEC(ctx, 15), GPR_VEC(ctx, 13)));
    // 0x10e7f28: 0x71af5ba9  pcpyud      $t3, $t5, $t7
    RC_SET_VEC(ctx, 11, rc_11, _mm_unpackhi_epi64(GPR_VEC(ctx, 13), GPR_VEC(ctx, 15)));
    // 0x10e7f2c: 0x48a8c000  qmtc2.ni    $t0, $vf24
    ctx->vu0_vf[24] = _mm_castsi128_ps(GPR_VEC(ctx, 8));
    // 0x10e7f30: 0x48a9c800  qmtc2.ni    $t1, $vf25
    ctx->vu0_vf[25] = _mm_castsi128_ps(GPR_VEC(ctx, 9));
    // 0x10e7f34: 0x48aad000  qmtc2.ni    $t2, $vf26
    ctx->vu0_vf[26] = _mm_castsi128_ps(GPR_VEC(ctx, 10));
    // 0x10e7f38: 0x4bfcc1bc  vmulax.xyzw $ACC, $vf24, $vf28x
    { __m128 res = PS2_VMUL_K(0, 1, ctx->vu0_vf[24], _mm_shuffle_ps(ctx->vu0_vf[28], ctx->vu0_vf[28], _MM_SHUFFLE(0,0,0,0))); ctx->vu0_acc = _mm_blendv_ps(ctx->vu0_acc, res, _mm_castsi128_ps(_mm_set_epi32(-1, -1, -1, -1))); }
    // 0x10e7f3c: 0x4bfcc8bd  vmadday.xyzw $ACC, $vf25, $vf28y
    { __m128 mul_res = PS2_VMUL_K(0, 1, ctx->vu0_vf[25], _mm_shuffle_ps(ctx->vu0_vf[28], ctx->vu0_vf[28], _MM_SHUFFLE(1,1,1,1))); __m128 res = PS2_VADD_K(1, 1, ctx->vu0_acc, mul_res); ctx->vu0_acc = _mm_blendv_ps(ctx->vu0_acc, res, _mm_castsi128_ps(_mm_set_epi32(-1, -1, -1, -1))); }
    // 0x10e7f40: 0x4bfcd0be  vmaddaz.xyzw $ACC, $vf26, $vf28z
    { __m128 mul_res = PS2_VMUL_K(0, 1, ctx->vu0_vf[26], _mm_shuffle_ps(ctx->vu0_vf[28], ctx->vu0_vf[28], _MM_SHUFFLE(2,2,2,2))); __m128 res = PS2_VADD_K(1, 1, ctx->vu0_acc, mul_res); ctx->vu0_acc = _mm_blendv_ps(ctx->vu0_acc, res, _mm_castsi128_ps(_mm_set_epi32(-1, -1, -1, -1))); }
    // 0x10e7f44: 0x4bfc074b  vmaddw.xyzw $vf29, $vf0, $vf28w
    { __m128 mul_res = PS2_VMUL_K(1, 1, ctx->vu0_vf[0], _mm_shuffle_ps(ctx->vu0_vf[28], ctx->vu0_vf[28], _MM_SHUFFLE(3,3,3,3))); __m128 res = PS2_VADD_K(1, 1, ctx->vu0_acc, mul_res); __m128i mask = _mm_set_epi32(-1, -1, -1, -1); ctx->vu0_vf[29] = _mm_blendv_ps(ctx->vu0_vf[29], res, _mm_castsi128_ps(mask)); }
    // 0x10e7f48: 0x4a3def6c  vsub.w      $vf29, $vf29, $vf29
    { __m128 res = PS2_VSUB_K(1, 1, ctx->vu0_vf[29], ctx->vu0_vf[29]); __m128i mask = _mm_set_epi32(-1, 0, 0, 0); ctx->vu0_vf[29] = PS2_VBLEND(ctx->vu0_vf[29], res, _mm_castsi128_ps(mask)); }
    // 0x10e7f4c: 0x4a20ef68  vadd.w      $vf29, $vf29, $vf0
    { __m128 res = PS2_VADD_K(1, 1, ctx->vu0_vf[29], ctx->vu0_vf[0]); __m128i mask = _mm_set_epi32(-1, 0, 0, 0); ctx->vu0_vf[29] = PS2_VBLEND(ctx->vu0_vf[29], res, _mm_castsi128_ps(mask)); }
    // 0x10e7f50: 0xf8780000  sqc2        $vf24, 0x0($v1)
    WRITE128(ADD32(RC_U32(rc_3), 0), _mm_castps_si128(ctx->vu0_vf[24]));
    // 0x10e7f54: 0xf8790010  sqc2        $vf25, 0x10($v1)
    WRITE128(ADD32(RC_U32(rc_3), 16), _mm_castps_si128(ctx->vu0_vf[25]));
    // 0x10e7f58: 0xf87a0020  sqc2        $vf26, 0x20($v1)
    WRITE128(ADD32(RC_U32(rc_3), 32), _mm_castps_si128(ctx->vu0_vf[26]));
    // 0x10e7f5c: 0xf87d0030  sqc2        $vf29, 0x30($v1)
    WRITE128(ADD32(RC_U32(rc_3), 48), _mm_castps_si128(ctx->vu0_vf[29]));
    // 0x10e7f60: 0x3c0142c8  lui         $at, 0x42C8
    RC_SET_S32(rc_1, (int32_t)((uint32_t)17096 << 16));
    Ps2SetGpr64(ctx, 1, RC_U64X(rc_1)); 
    // 0x10e7f64: 0x44816000  mtc1        $at, $f12
    { uint32_t bits = RC_U32(rc_1); std::memcpy(&rf_12, &bits, sizeof(bits)); }
    ctx->f[12] = rf_12; 
    // 0x10e7f68: 0x8fa501ac  lw          $a1, 0x1AC($sp)
    RC_SET_S32(rc_5, (int32_t)READ32(ADD32(RC_U32(rc_29), 428)));
    Ps2SetGpr64(ctx, 5, RC_U64X(rc_5)); 
    // 0x10e7f6c: 0x60482d  daddu       $t1, $v1, $zero
    RC_SET_U64(rc_9, (uint64_t)RC_U64X(rc_3) + (uint64_t)RC_U64(0));
    Ps2SetGpr64(ctx, 9, RC_U64X(rc_9)); 
    // 0x10e7f70: 0x262405f0  addiu       $a0, $s1, 0x5F0
    RC_SET_S32(rc_4, (int32_t)ADD32(RC_U32(rc_17), 1520));
    Ps2SetGpr64(ctx, 4, RC_U64X(rc_4)); 
    // 0x10e7f74: 0x27a600c0  addiu       $a2, $sp, 0xC0
    RC_SET_S32(rc_6, (int32_t)ADD32(RC_U32(rc_29), 192));
    Ps2SetGpr64(ctx, 6, RC_U64X(rc_6)); 
    // 0x10e7f78: 0x3c0382d  daddu       $a3, $fp, $zero
    rc_30 = Ps2GprU64(ctx, 30); 
    RC_SET_U64(rc_7, (uint64_t)RC_U64(rc_30) + (uint64_t)RC_U64(0));
    Ps2SetGpr64(ctx, 7, rc_7); 
    // 0x10e7f7c: 0xc437bf4  jal         func_10DEFD0
    RC_SET_U32(rc_31, 0x10E7F84u);
    Ps2SetGpr64(ctx, 31, RC_U64X(rc_31)); 
    // 0x10e7f80: 0x402d  daddu       $t0, $zero, $zero (Delay Slot)
    RC_SET_U64(rc_8, (uint64_t)RC_U64(0) + (uint64_t)RC_U64(0));
    Ps2SetGpr64(ctx, 8, RC_U64X(rc_8)); 
    ctx->pc = 0x10DEFD0u;
    if (!PS2_CALL_GUEST_KNOWN(0x10DEFD0u, charrootdyn_detectCharEdge_0x10defd0, 0x10E7F84u, 0x10E7F7Cu, DirectCall, "JAL")) {
        return;
    }
label_10e7f84:
    // 0x10e7f84: 0x8fa601ac  lw          $a2, 0x1AC($sp)
    rc_29 = Ps2GprU64(ctx, 29); 
    RC_SET_S32(rc_6, (int32_t)READ32(ADD32(RC_U32(rc_29), 428)));
    Ps2SetGpr64(ctx, 6, RC_U64X(rc_6)); 
label_10e7f88:
    // 0x10e7f88: 0x26240fd0  addiu       $a0, $s1, 0xFD0
    rc_17 = Ps2GprU64(ctx, 17); 
    RC_SET_S32(rc_4, (int32_t)ADD32(RC_U32(rc_17), 4048));
    Ps2SetGpr64(ctx, 4, RC_U64X(rc_4)); 
    // 0x10e7f8c: 0xc4356fc  jal         func_10D5BF0
    RC_SET_U32(rc_31, 0x10E7F94u);
    Ps2SetGpr64(ctx, 31, RC_U64X(rc_31)); 
    // 0x10e7f90: 0x220282d  daddu       $a1, $s1, $zero (Delay Slot)
    RC_SET_U64(rc_5, (uint64_t)RC_U64(rc_17) + (uint64_t)RC_U64(0));
    Ps2SetGpr64(ctx, 5, rc_5); 
    ctx->pc = 0x10D5BF0u;
    if (!PS2_CALL_GUEST_KNOWN(0x10D5BF0u, charrootdyn_getNipPlaneStatus_0x10d5bf0, 0x10E7F94u, 0x10E7F8Cu, DirectCall, "JAL")) {
        return;
    }
label_10e7f94:
    // 0x10e7f94: 0x12a0000e  beqz        $s5, . + 4 + (0xE << 2)
    {
        rc_21 = Ps2GprU64(ctx, 21); 
        const bool branch_taken_0x10e7f94 = (RC_U64(rc_21) == RC_U64(0));
        // 0x10e7f98: 0x8fa501ac  lw          $a1, 0x1AC($sp) (Delay Slot)
        rc_29 = Ps2GprU64(ctx, 29); 
        RC_SET_S32(rc_5, (int32_t)READ32(ADD32(RC_U32(rc_29), 428)));
        Ps2SetGpr64(ctx, 5, RC_U64X(rc_5)); 
        if (branch_taken_0x10e7f94) {
            goto label_10e7fd0;
        }
    }
    // 0x10e7f9c: 0xc44c760  jal         func_1131D80
    RC_SET_U32(rc_31, 0x10E7FA4u);
    Ps2SetGpr64(ctx, 31, RC_U64X(rc_31)); 
    // 0x10e7fa0: 0x27a400d0  addiu       $a0, $sp, 0xD0 (Delay Slot)
    RC_SET_S32(rc_4, (int32_t)ADD32(RC_U32(rc_29), 208));
    Ps2SetGpr64(ctx, 4, RC_U64X(rc_4)); 
    ctx->pc = 0x1131D80u;
    if (!PS2_CALL_GUEST_KNOWN(0x1131D80u, GetGeometryPointerPosition_0x1131d80, 0x10E7FA4u, 0x10E7F9Cu, DirectCall, "JAL")) {
        return;
    }
label_10e7fa4:
    // 0x10e7fa4: 0xc7a000d4  lwc1        $f0, 0xD4($sp)
    rc_29 = Ps2GprU64(ctx, 29); 
    { uint32_t bits = READ32(ADD32(RC_U32(rc_29), 212)); float f; std::memcpy(&f, &bits, sizeof(f)); rf_0 = f; }
    // 0x10e7fa8: 0xc7a100a4  lwc1        $f1, 0xA4($sp)
    { uint32_t bits = READ32(ADD32(RC_U32(rc_29), 164)); float f; std::memcpy(&f, &bits, sizeof(f)); rf_1 = f; }
    ctx->f[1] = rf_1; 
    // 0x10e7fac: 0x46010001  sub.s       $f0, $f0, $f1
    rf_0 = FPU_SUB_S(rf_0, rf_1);
    // 0x10e7fb0: 0x461b0002  mul.s       $f0, $f0, $f27
    rf_27 = ctx->f[27]; 
    rf_0 = FPU_MUL_S(rf_0, rf_27);
    ctx->f[0] = rf_0; 
    // 0x10e7fb4: 0x10000007  b           . + 4 + (0x7 << 2)
    {
        const bool branch_taken_0x10e7fb4 = (RC_U64(0) == RC_U64(0));
        // 0x10e7fb8: 0xe6200b54  swc1        $f0, 0xB54($s1) (Delay Slot)
        rc_17 = Ps2GprU64(ctx, 17); 
        { float f = rf_0; uint32_t bits; std::memcpy(&bits, &f, sizeof(bits)); WRITE32(ADD32(RC_U32(rc_17), 2900), bits); }
        if (branch_taken_0x10e7fb4) {
            goto label_10e7fd4;
        }
    }
    // 0x10e7fbc: 0x0  nop
    // NOP
label_10e7fc0:
    // 0x10e7fc0: 0xc43c7f0  jal         func_10F1FC0
    RC_SET_U32(rc_31, 0x10E7FC8u);
    Ps2SetGpr64(ctx, 31, RC_U64X(rc_31)); 
    // 0x10e7fc4: 0x8fa401a0  lw          $a0, 0x1A0($sp) (Delay Slot)
    rc_29 = Ps2GprU64(ctx, 29); 
    RC_SET_S32(rc_4, (int32_t)READ32(ADD32(RC_U32(rc_29), 416)));
    Ps2SetGpr64(ctx, 4, RC_U64X(rc_4)); 
    ctx->pc = 0x10F1FC0u;
    if (!PS2_CALL_GUEST_KNOWN(0x10F1FC0u, ForceUnparentLayoutObj_0x10f1fc0, 0x10E7FC8u, 0x10E7FC0u, DirectCall, "JAL")) {
        return;
    }
label_10e7fc8:
    // 0x10e7fc8: 0x24020001  addiu       $v0, $zero, 0x1
    RC_SET_S32(rc_2, (int32_t)ADD32(RC_U32(0), 1));
    Ps2SetGpr64(ctx, 2, RC_U64X(rc_2)); 
    // 0x10e7fcc: 0xa222035c  sb          $v0, 0x35C($s1)
    rc_17 = Ps2GprU64(ctx, 17); 
    WRITE8(ADD32(RC_U32(rc_17), 860), (uint8_t)RC_U32(rc_2));
label_10e7fd0:
    // 0x10e7fd0: 0xae200b54  sw          $zero, 0xB54($s1)
    rc_17 = Ps2GprU64(ctx, 17); 
    WRITE32(ADD32(RC_U32(rc_17), 2900), RC_U32(0));
label_10e7fd4:
    // 0x10e7fd4: 0x8fa401a8  lw          $a0, 0x1A8($sp)
    rc_29 = Ps2GprU64(ctx, 29); 
    RC_SET_S32(rc_4, (int32_t)READ32(ADD32(RC_U32(rc_29), 424)));
    Ps2SetGpr64(ctx, 4, RC_U64X(rc_4)); 
    // 0x10e7fd8: 0x10800014  beqz        $a0, . + 4 + (0x14 << 2)
    {
        const bool branch_taken_0x10e7fd8 = (RC_U64X(rc_4) == RC_U64(0));
        // 0x10e7fdc: 0x8fa401b4  lw          $a0, 0x1B4($sp) (Delay Slot)
        RC_SET_S32(rc_4, (int32_t)READ32(ADD32(RC_U32(rc_29), 436)));
        Ps2SetGpr64(ctx, 4, RC_U64X(rc_4)); 
        if (branch_taken_0x10e7fd8) {
            goto label_10e802c;
        }
    }
    // 0x10e7fe0: 0xc62c03e0  lwc1        $f12, 0x3E0($s1)
    rc_17 = Ps2GprU64(ctx, 17); 
    { uint32_t bits = READ32(ADD32(RC_U32(rc_17), 992)); float f; std::memcpy(&f, &bits, sizeof(f)); rf_12 = f; }
    ctx->f[12] = rf_12; 
    // 0x10e7fe4: 0x4480a000  mtc1        $zero, $f20
    { uint32_t bits = RC_U32(0); std::memcpy(&rf_20, &bits, sizeof(bits)); }
    ctx->f[20] = rf_20; 
    // 0x10e7fe8: 0x46146032  c.eq.s      $f12, $f20
    ctx->fcr31 = (FPU_C_EQ_S(rf_12, rf_20)) ? (ctx->fcr31 | 0x800000) : (ctx->fcr31 & ~0x800000);
    // 0x10e7fec: 0x4501000f  bc1t        . + 4 + (0xF << 2)
    {
        const bool branch_taken_0x10e7fec = ((ctx->fcr31 & 0x800000));
        // 0x10e7ff0: 0x27a20040  addiu       $v0, $sp, 0x40 (Delay Slot)
        RC_SET_S32(rc_2, (int32_t)ADD32(RC_U32(rc_29), 64));
        Ps2SetGpr64(ctx, 2, RC_U64X(rc_2)); 
        if (branch_taken_0x10e7fec) {
            goto label_10e802c;
        }
    }
    // 0x10e7ff4: 0x8fa601b0  lw          $a2, 0x1B0($sp)
    RC_SET_S32(rc_6, (int32_t)READ32(ADD32(RC_U32(rc_29), 432)));
    Ps2SetGpr64(ctx, 6, RC_U64X(rc_6)); 
    // 0x10e7ff8: 0x40202d  daddu       $a0, $v0, $zero
    RC_SET_U64(rc_4, (uint64_t)RC_U64X(rc_2) + (uint64_t)RC_U64(0));
    Ps2SetGpr64(ctx, 4, RC_U64X(rc_4)); 
    // 0x10e7ffc: 0x3c0282d  daddu       $a1, $fp, $zero
    rc_30 = Ps2GprU64(ctx, 30); 
    RC_SET_U64(rc_5, (uint64_t)RC_U64(rc_30) + (uint64_t)RC_U64(0));
    Ps2SetGpr64(ctx, 5, rc_5); 
    // 0x10e8000: 0xc4371f0  jal         func_10DC7C0
    RC_SET_U32(rc_31, 0x10E8008u);
    Ps2SetGpr64(ctx, 31, RC_U64X(rc_31)); 
    // 0x10e8004: 0x240382d  daddu       $a3, $s2, $zero (Delay Slot)
    rc_18 = Ps2GprU64(ctx, 18); 
    RC_SET_U64(rc_7, (uint64_t)RC_U64(rc_18) + (uint64_t)RC_U64(0));
    Ps2SetGpr64(ctx, 7, rc_7); 
    ctx->pc = 0x10DC7C0u;
    if (!PS2_CALL_GUEST_KNOWN(0x10DC7C0u, charrootdyn_getLandingSurface_0x10dc7c0, 0x10E8008u, 0x10E8000u, DirectCall, "JAL")) {
        return;
    }
label_10e8008:
    // 0x10e8008: 0x10400007  beqz        $v0, . + 4 + (0x7 << 2)
    {
        rc_2 = Ps2GprU64(ctx, 2); 
        const bool branch_taken_0x10e8008 = (RC_U64(rc_2) == RC_U64(0));
        // 0x10e800c: 0x24020001  addiu       $v0, $zero, 0x1 (Delay Slot)
        RC_SET_S32(rc_2, (int32_t)ADD32(RC_U32(0), 1));
        Ps2SetGpr64(ctx, 2, RC_U64X(rc_2)); 
        if (branch_taken_0x10e8008) {
            goto label_10e8028;
        }
    }
    // 0x10e8010: 0x27a30040  addiu       $v1, $sp, 0x40
    rc_29 = Ps2GprU64(ctx, 29); 
    RC_SET_S32(rc_3, (int32_t)ADD32(RC_U32(rc_29), 64));
    Ps2SetGpr64(ctx, 3, RC_U64X(rc_3)); 
    // 0x10e8014: 0xa222035e  sb          $v0, 0x35E($s1)
    rc_17 = Ps2GprU64(ctx, 17); 
    WRITE8(ADD32(RC_U32(rc_17), 862), (uint8_t)RC_U32(rc_2));
    // 0x10e8018: 0x4600a306  mov.s       $f12, $f20
    rf_20 = ctx->f[20]; 
    rf_12 = FPU_MOV_S(rf_20);
    ctx->f[12] = rf_12; 
    // 0x10e801c: 0x220202d  daddu       $a0, $s1, $zero
    RC_SET_U64(rc_4, (uint64_t)RC_U64(rc_17) + (uint64_t)RC_U64(0));
    Ps2SetGpr64(ctx, 4, rc_4); 
    // 0x10e8020: 0xc4371d8  jal         func_10DC760
    RC_SET_U32(rc_31, 0x10E8028u);
    Ps2SetGpr64(ctx, 31, RC_U64X(rc_31)); 
    // 0x10e8024: 0x60282d  daddu       $a1, $v1, $zero (Delay Slot)
    RC_SET_U64(rc_5, (uint64_t)RC_U64X(rc_3) + (uint64_t)RC_U64(0));
    Ps2SetGpr64(ctx, 5, RC_U64X(rc_5)); 
    ctx->pc = 0x10DC760u;
    if (!PS2_CALL_GUEST_KNOWN(0x10DC760u, charrootdyn_setHColFindStatus_0x10dc760, 0x10E8028u, 0x10E8020u, DirectCall, "JAL")) {
        return;
    }
label_10e8028:
    // 0x10e8028: 0x8fa401b4  lw          $a0, 0x1B4($sp)
    rc_29 = Ps2GprU64(ctx, 29); 
    RC_SET_S32(rc_4, (int32_t)READ32(ADD32(RC_U32(rc_29), 436)));
    Ps2SetGpr64(ctx, 4, RC_U64X(rc_4)); 
label_10e802c:
    // 0x10e802c: 0x8c820068  lw          $v0, 0x68($a0)
    rc_4 = Ps2GprU64(ctx, 4); 
    RC_SET_S32(rc_2, (int32_t)READ32(ADD32(RC_U32(rc_4), 104)));
    // 0x10e8030: 0x21582  srl         $v0, $v0, 22
    RC_SET_S32(rc_2, (int32_t)SRL32(RC_U32(rc_2), 22));
    // 0x10e8034: 0x30420001  andi        $v0, $v0, 0x1
    RC_SET_U64(rc_2, RC_U64X(rc_2) & (uint64_t)(uint16_t)1);
    Ps2SetGpr64(ctx, 2, RC_U64X(rc_2)); 
    // 0x10e8038: 0x10400008  beqz        $v0, . + 4 + (0x8 << 2)
    {
        const bool branch_taken_0x10e8038 = (RC_U64X(rc_2) == RC_U64(0));
        if (branch_taken_0x10e8038) {
            goto label_10e805c;
        }
    }
    // 0x10e8040: 0xc43575c  jal         func_10D5D70
    RC_SET_U32(rc_31, 0x10E8048u);
    Ps2SetGpr64(ctx, 31, RC_U64X(rc_31)); 
    // 0x10e8044: 0x220202d  daddu       $a0, $s1, $zero (Delay Slot)
    rc_17 = Ps2GprU64(ctx, 17); 
    RC_SET_U64(rc_4, (uint64_t)RC_U64(rc_17) + (uint64_t)RC_U64(0));
    Ps2SetGpr64(ctx, 4, rc_4); 
    ctx->pc = 0x10D5D70u;
    if (!PS2_CALL_GUEST_KNOWN(0x10D5D70u, GetCharaVCollisionStatus_0x10d5d70, 0x10E8048u, 0x10E8040u, DirectCall, "JAL")) {
        return;
    }
label_10e8048:
    // 0x10e8048: 0x10400004  beqz        $v0, . + 4 + (0x4 << 2)
    {
        rc_2 = Ps2GprU64(ctx, 2); 
        const bool branch_taken_0x10e8048 = (RC_U64(rc_2) == RC_U64(0));
        if (branch_taken_0x10e8048) {
            goto label_10e805c;
        }
    }
    // 0x10e8050: 0x8e220e70  lw          $v0, 0xE70($s1)
    rc_17 = Ps2GprU64(ctx, 17); 
    RC_SET_S32(rc_2, (int32_t)READ32(ADD32(RC_U32(rc_17), 3696)));
    Ps2SetGpr64(ctx, 2, RC_U64X(rc_2)); 
    // 0x10e8054: 0x54400001  bnel        $v0, $zero, . + 4 + (0x1 << 2)
    {
        const bool branch_taken_0x10e8054 = (RC_U64X(rc_2) != RC_U64(0));
        if (branch_taken_0x10e8054) {
            // 0x10e8058: 0xae200610  sw          $zero, 0x610($s1) (Delay Slot)
            WRITE32(ADD32(RC_U32(rc_17), 1552), RC_U32(0));
            goto label_10e805c;
        }
    }
label_10e805c:
    // 0x10e805c: 0xc435690  jal         func_10D5A40
    RC_SET_U32(rc_31, 0x10E8064u);
    Ps2SetGpr64(ctx, 31, RC_U64X(rc_31)); 
    // 0x10e8060: 0x8fa401a0  lw          $a0, 0x1A0($sp) (Delay Slot)
    rc_29 = Ps2GprU64(ctx, 29); 
    RC_SET_S32(rc_4, (int32_t)READ32(ADD32(RC_U32(rc_29), 416)));
    Ps2SetGpr64(ctx, 4, RC_U64X(rc_4)); 
    ctx->pc = 0x10D5A40u;
    if (!PS2_CALL_GUEST_KNOWN(0x10D5A40u, charrootdyn_getChainStatus_0x10d5a40, 0x10E8064u, 0x10E805Cu, DirectCall, "JAL")) {
        return;
    }
label_10e8064:
    // 0x10e8064: 0x8fa301b4  lw          $v1, 0x1B4($sp)
    rc_29 = Ps2GprU64(ctx, 29); 
    RC_SET_S32(rc_3, (int32_t)READ32(ADD32(RC_U32(rc_29), 436)));
    Ps2SetGpr64(ctx, 3, RC_U64X(rc_3)); 
    // 0x10e8068: 0x9062006b  lbu         $v0, 0x6B($v1)
    RC_SET_ZE32(rc_2, (uint8_t)READ8(ADD32(RC_U32(rc_3), 107)));
    // 0x10e806c: 0x30420001  andi        $v0, $v0, 0x1
    RC_SET_U64(rc_2, RC_U64X(rc_2) & (uint64_t)(uint16_t)1);
    Ps2SetGpr64(ctx, 2, RC_U64X(rc_2)); 
    // 0x10e8070: 0x1040000c  beqz        $v0, . + 4 + (0xC << 2)
    {
        const bool branch_taken_0x10e8070 = (RC_U64X(rc_2) == RC_U64(0));
        // 0x10e8074: 0x27a40040  addiu       $a0, $sp, 0x40 (Delay Slot)
        RC_SET_S32(rc_4, (int32_t)ADD32(RC_U32(rc_29), 64));
        Ps2SetGpr64(ctx, 4, RC_U64X(rc_4)); 
        if (branch_taken_0x10e8070) {
            goto label_10e80a4;
        }
    }
    // 0x10e8078: 0xc62c03dc  lwc1        $f12, 0x3DC($s1)
    rc_17 = Ps2GprU64(ctx, 17); 
    { uint32_t bits = READ32(ADD32(RC_U32(rc_17), 988)); float f; std::memcpy(&f, &bits, sizeof(f)); rf_12 = f; }
    // 0x10e807c: 0x86260370  lh          $a2, 0x370($s1)
    RC_SET_S32(rc_6, (int16_t)READ16(ADD32(RC_U32(rc_17), 880)));
    // 0x10e8080: 0x80282d  daddu       $a1, $a0, $zero
    RC_SET_U64(rc_5, (uint64_t)RC_U64X(rc_4) + (uint64_t)RC_U64(0));
    Ps2SetGpr64(ctx, 5, RC_U64X(rc_5)); 
    // 0x10e8084: 0x46006307  neg.s       $f12, $f12
    rf_12 = FPU_NEG_S(rf_12);
    ctx->f[12] = rf_12; 
    // 0x10e8088: 0x8e220e38  lw          $v0, 0xE38($s1)
    RC_SET_S32(rc_2, (int32_t)READ32(ADD32(RC_U32(rc_17), 3640)));
    Ps2SetGpr64(ctx, 2, RC_U64X(rc_2)); 
    // 0x10e808c: 0x63080  sll         $a2, $a2, 2
    RC_SET_S32(rc_6, (int32_t)SLL32(RC_U32(rc_6), 2));
    // 0x10e8090: 0x3c0382d  daddu       $a3, $fp, $zero
    rc_30 = Ps2GprU64(ctx, 30); 
    RC_SET_U64(rc_7, (uint64_t)RC_U64(rc_30) + (uint64_t)RC_U64(0));
    Ps2SetGpr64(ctx, 7, rc_7); 
    // 0x10e8094: 0x220202d  daddu       $a0, $s1, $zero
    RC_SET_U64(rc_4, (uint64_t)RC_U64(rc_17) + (uint64_t)RC_U64(0));
    Ps2SetGpr64(ctx, 4, rc_4); 
    // 0x10e8098: 0x463021  addu        $a2, $v0, $a2
    RC_SET_S32(rc_6, (int32_t)ADD32(RC_U32(rc_2), RC_U32(rc_6)));
    Ps2SetGpr64(ctx, 6, RC_U64X(rc_6)); 
    // 0x10e809c: 0xc437264  jal         func_10DC990
    RC_SET_U32(rc_31, 0x10E80A4u);
    Ps2SetGpr64(ctx, 31, RC_U64X(rc_31)); 
    // 0x10e80a0: 0x240402d  daddu       $t0, $s2, $zero (Delay Slot)
    rc_18 = Ps2GprU64(ctx, 18); 
    RC_SET_U64(rc_8, (uint64_t)RC_U64(rc_18) + (uint64_t)RC_U64(0));
    Ps2SetGpr64(ctx, 8, rc_8); 
    ctx->pc = 0x10DC990u;
    if (!PS2_CALL_GUEST_KNOWN(0x10DC990u, charrootdyn_getWaterStatus_0x10dc990, 0x10E80A4u, 0x10E809Cu, DirectCall, "JAL")) {
        return;
    }
label_10e80a4:
    // 0x10e80a4: 0x8fa601b0  lw          $a2, 0x1B0($sp)
    rc_29 = Ps2GprU64(ctx, 29); 
    RC_SET_S32(rc_6, (int32_t)READ32(ADD32(RC_U32(rc_29), 432)));
    Ps2SetGpr64(ctx, 6, RC_U64X(rc_6)); 
    // 0x10e80a8: 0x26240030  addiu       $a0, $s1, 0x30
    rc_17 = Ps2GprU64(ctx, 17); 
    RC_SET_S32(rc_4, (int32_t)ADD32(RC_U32(rc_17), 48));
    Ps2SetGpr64(ctx, 4, RC_U64X(rc_4)); 
    // 0x10e80ac: 0x240282d  daddu       $a1, $s2, $zero
    rc_18 = Ps2GprU64(ctx, 18); 
    RC_SET_U64(rc_5, (uint64_t)RC_U64(rc_18) + (uint64_t)RC_U64(0));
    Ps2SetGpr64(ctx, 5, rc_5); 
    // 0x10e80b0: 0xc437370  jal         func_10DCDC0
    RC_SET_U32(rc_31, 0x10E80B8u);
    Ps2SetGpr64(ctx, 31, RC_U64X(rc_31)); 
    // 0x10e80b4: 0x4600db06  mov.s       $f12, $f27 (Delay Slot)
    rf_27 = ctx->f[27]; 
    rf_12 = FPU_MOV_S(rf_27);
    ctx->f[12] = rf_12; 
    ctx->pc = 0x10DCDC0u;
    if (!PS2_CALL_GUEST_NOCHK(0x10DCDC0u, charrootdyn_getvelocity_0x10dcdc0, 0x10E80B8u, 0x10E80B0u, DirectCall, "JAL")) {
        return;
    }
label_10e80b8:
    // 0x10e80b8: 0xdfb001d0  ld          $s0, 0x1D0($sp)
    rc_29 = Ps2GprU64(ctx, 29); 
    RC_SET_U64(rc_16, READ64(ADD32(RC_U32(rc_29), 464)));
    Ps2SetGpr64(ctx, 16, rc_16); 
    // 0x10e80bc: 0xdfb101d8  ld          $s1, 0x1D8($sp)
    RC_SET_U64(rc_17, READ64(ADD32(RC_U32(rc_29), 472)));
    Ps2SetGpr64(ctx, 17, rc_17); 
    // 0x10e80c0: 0xdfb201e0  ld          $s2, 0x1E0($sp)
    RC_SET_U64(rc_18, READ64(ADD32(RC_U32(rc_29), 480)));
    Ps2SetGpr64(ctx, 18, rc_18); 
    // 0x10e80c4: 0xdfb301e8  ld          $s3, 0x1E8($sp)
    RC_SET_U64(rc_19, READ64(ADD32(RC_U32(rc_29), 488)));
    Ps2SetGpr64(ctx, 19, rc_19); 
    // 0x10e80c8: 0xdfb401f0  ld          $s4, 0x1F0($sp)
    RC_SET_U64(rc_20, READ64(ADD32(RC_U32(rc_29), 496)));
    Ps2SetGpr64(ctx, 20, rc_20); 
    // 0x10e80cc: 0xdfb501f8  ld          $s5, 0x1F8($sp)
    RC_SET_U64(rc_21, READ64(ADD32(RC_U32(rc_29), 504)));
    Ps2SetGpr64(ctx, 21, rc_21); 
    // 0x10e80d0: 0xdfb60200  ld          $s6, 0x200($sp)
    RC_SET_U64(rc_22, READ64(ADD32(RC_U32(rc_29), 512)));
    Ps2SetGpr64(ctx, 22, rc_22); 
    // 0x10e80d4: 0xdfb70208  ld          $s7, 0x208($sp)
    RC_SET_U64(rc_23, READ64(ADD32(RC_U32(rc_29), 520)));
    Ps2SetGpr64(ctx, 23, rc_23); 
    // 0x10e80d8: 0xdfbe0210  ld          $fp, 0x210($sp)
    RC_SET_U64(rc_30, READ64(ADD32(RC_U32(rc_29), 528)));
    Ps2SetGpr64(ctx, 30, rc_30); 
    // 0x10e80dc: 0xdfbf0218  ld          $ra, 0x218($sp)
    RC_SET_U64(rc_31, READ64(ADD32(RC_U32(rc_29), 536)));
    Ps2SetGpr64(ctx, 31, rc_31); 
    // 0x10e80e0: 0xc7bb0258  lwc1        $f27, 0x258($sp)
    { uint32_t bits = READ32(ADD32(RC_U32(rc_29), 600)); float f; std::memcpy(&f, &bits, sizeof(f)); rf_27 = f; }
    ctx->f[27] = rf_27; 
    // 0x10e80e4: 0xc7ba0250  lwc1        $f26, 0x250($sp)
    { uint32_t bits = READ32(ADD32(RC_U32(rc_29), 592)); float f; std::memcpy(&f, &bits, sizeof(f)); rf_26 = f; }
    ctx->f[26] = rf_26; 
    // 0x10e80e8: 0xc7b90248  lwc1        $f25, 0x248($sp)
    { uint32_t bits = READ32(ADD32(RC_U32(rc_29), 584)); float f; std::memcpy(&f, &bits, sizeof(f)); rf_25 = f; }
    ctx->f[25] = rf_25; 
    // 0x10e80ec: 0xc7b80240  lwc1        $f24, 0x240($sp)
    { uint32_t bits = READ32(ADD32(RC_U32(rc_29), 576)); float f; std::memcpy(&f, &bits, sizeof(f)); rf_24 = f; }
    ctx->f[24] = rf_24; 
    // 0x10e80f0: 0xc7b70238  lwc1        $f23, 0x238($sp)
    { uint32_t bits = READ32(ADD32(RC_U32(rc_29), 568)); float f; std::memcpy(&f, &bits, sizeof(f)); rf_23 = f; }
    ctx->f[23] = rf_23; 
    // 0x10e80f4: 0xc7b60230  lwc1        $f22, 0x230($sp)
    { uint32_t bits = READ32(ADD32(RC_U32(rc_29), 560)); float f; std::memcpy(&f, &bits, sizeof(f)); rf_22 = f; }
    ctx->f[22] = rf_22; 
    // 0x10e80f8: 0xc7b50228  lwc1        $f21, 0x228($sp)
    { uint32_t bits = READ32(ADD32(RC_U32(rc_29), 552)); float f; std::memcpy(&f, &bits, sizeof(f)); rf_21 = f; }
    ctx->f[21] = rf_21; 
    // 0x10e80fc: 0xc7b40220  lwc1        $f20, 0x220($sp)
    { uint32_t bits = READ32(ADD32(RC_U32(rc_29), 544)); float f; std::memcpy(&f, &bits, sizeof(f)); rf_20 = f; }
    ctx->f[20] = rf_20; 
    // 0x10e8100: 0x3e00008  jr          $ra
    {
        const uint32_t jumpTarget = RC_U32(rc_31);
        // 0x10e8104: 0x27bd0260  addiu       $sp, $sp, 0x260 (Delay Slot)
        RC_SET_S32(rc_29, (int32_t)ADD32(RC_U32(rc_29), 608));
        Ps2SetGpr64(ctx, 29, RC_U64X(rc_29)); 
        ctx->pc = jumpTarget;
        #if defined(PS2X_STRICT_RETURN_DIAGNOSTICS) && PS2X_STRICT_RETURN_DIAGNOSTICS
        // (the strict-return dispatch: not in the replay)
        return;
        #else
        ctx->pc = jumpTarget;
        return;
        #endif
    }
    ctx->pc = 0x10E8108u;
    // (the fall-through jump: not reached)

    }
#if defined(__clang__)
#pragma clang diagnostic pop
#elif defined(__GNUC__)
#pragma GCC diagnostic pop
#endif
#pragma pop_macro("READ8")
#pragma pop_macro("READ16")
#pragma pop_macro("READ32")
#pragma pop_macro("READ64")
#pragma pop_macro("READ128")
#pragma pop_macro("WRITE8")
#pragma pop_macro("WRITE16")
#pragma pop_macro("WRITE32")
#pragma pop_macro("WRITE64")
#pragma pop_macro("WRITE128")
#pragma pop_macro("PS2_CALL_GUEST_KNOWN")
#pragma pop_macro("PS2_CALL_GUEST_NOCHK")
} // namespace x1geo
