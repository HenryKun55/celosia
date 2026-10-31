// The script system's SCR data interpreter (round 48, SOTC_SCRIPTVM=0 off;
// each table entry alone with SOTC_DECOMP_SKIP=<name>).
//
// The data-driven AI. Inside the script bodies (decomp_scriptvm.cpp's
// sub-threads) the AI's behaviour is data: lists of 0x40-byte records, each
// naming a C function (an SCR* "function" or "condition") and its arguments.
// execSCRBaseFunc walks one list and calls each record's function; the
// opcode functions decode their arguments (resource handles: a word whose
// low 18 bits are the distance back to the handle's owner pointer, bits
// 18..29 an element index) and do one step of game logic.
//
// - execSCRBaseFunc (0x1455fa8): a0 the object, a1 a handle to the record
//   list. The handle's owner (*(a1 - (h & 0x3ffff))) unless 0 or the null
//   owner (*0x1e0ed4, a relocated lui / lw pair): the list is the owner's
//   +0x10 array + index x the element size (owner +8 -> +0xc; the mult sets
//   HI / LO). Then for each record until +0 is 0: jalr +0 with a0 = the
//   object, a1 / a2 = +4 / +8, f12-f15 = +0xc..+0x18, a3 / t0 = the 64-bit
//   +0x20 / +0x28, t1 / t2 / t3 = record + 0x30 / 0x34 / 0x38, and the frame's
//   word sp + 0 = record + 0x3c (the callees read it at their entry sp + 0).
//   A coroutine: the record calls go through callMaySleep (none of the
//   SCR* functions can sleep statically, but the call is indirect), resumed
//   at 0x1456078 (s0 = the next record, s1 = the object, the frame).
// - SCRFuncSubFunc (0x1450bb8): execSCRBaseFunc(a0, t3) as a tail jump.
// - SCRFuncConditionExecFunc (0x1450b60): execCondition(a0, t2) (the
//   condition walker, decomp_hot44b), when true execSCRBaseFunc(a0, t3) as a
//   tail jump; else v0 0. Both are coroutines through their tail jump.
// - getSCRDataCtrlWork (0x1455f80): GetSCRCharaAIWorkPtr(a0) rounded up to
//   16 bytes (+0x13 & ~0xf). GetSCRCharaAIWorkPtr (0x13cfae8): the script
//   character class's +0x1200 (GetScriptCharacterClass, a relocated jal).
//   CheckSCRCharaAIWorkPtr (0x13cfb08): the same, 0 for a0 0.
// - SCRGetFirstObj (0x137c348): GAMEUTIL_get_first_obj as a tail jump.
// - SCRConditionPlayerLastMotionShiftCheck (0x1457648): the player's
//   (work +0x4078) character class: its current motion (+0x1c0 +4) is the
//   handle (the caller's sp + 0 word) element and its +0x360 byte set.
// - SCRConditionPlayerControlModeCheck (0x1457708): the player's control
//   mode (class +0x750) is the handle's element.
// - SCRConditionPlayerParentColAttr (0x1456a38) / SCRSoundConditionPlayer-
//   ParentColAttr (0x1464ed0): t3 a handle to a parent object (owner +0x10),
//   t0 / t1 an attribute: with a parent (and its +8 set for the first), the
//   player's collision controller (getPlayerColCont2) must stand on it
//   (ColCont2_IsParentObj); without one, the handle must be empty; then
//   ColCont2_IsColStatus / ColCont2_IsEventColStatus(t0, t1). The work is
//   getSCRDataCtrlWork's (+0x4078 the player) / getSScriptWork's (+0).
// - SCRConditionRootAnglePlayerWorldXZ (0x1457cf8): the player's position
//   (work +0x407c -> +0x134) in the object's frame (its character class's
//   +0x470 quaternion-made matrix and the layout's +0x134 position, VU0),
//   normalised; atan2(x, z) in [f12, f13] degrees (x pi/180).
// - SCRFuncStartEventDemoGameModeCheck (0x1451468): first play only (no game
//   clear, not hard mode, *0x1465d38 0, flag a1 unset): execCondition(a0, t3)
//   and then createScrEventDemoManager with the decoded handles t1 / t2.
// - SCRFuncVolumeCheckByGameObjForSelfPos (0x14582d8): the object's position
//   (layout +0x134, kept at sp + 0x60) inside the judge volume (t3's owner
//   +0x10, its +8) at the handle (caller's sp + 0) element:
//   CheckPositionInsideOfJudgeVolumeObject.
// - SCRCharaTurnDirectInput (0x137c798): character class +0x7e8 = f12.
// - SCRGetGetGeometryPointerLegalParentObj (0x137d640): the geometry pointer
//   object, or for a type-10 one with a layout +0x124 the geometry of its
//   +0xc0 when that one is of type 13 or 22.
//
// Exact: every statement of the generated code in its order on the
// context's registers (scratch ones included: the record calls can sleep,
// and the sleep saves every register), the frames' saves and stores, HI / LO
// at each mult, the FPU / VU0 state as the code leaves it, v0 as each path
// leaves it (SCRFuncStartEventDemoGameModeCheck's 0x30000 included). Guest
// calls with the original's ra and frame sp; relocated jal / lui / lw sites
// read from the live code words as the generated code does.
// Not as the original: the record loop's EE checkpoint (as every native loop).
#include "sotc_decomp.h"

#include "ps2_runtime_macros.h"
#include "runtime/ps2_memory.h"

#include <cstring>

namespace decomp_scriptvm2
{
    using sotc_decomp::callGuest;
    using sotc_decomp::codeWord;
    using sotc_decomp::jalTarget;

    enum : int
    {
        rAT = 1, rV0 = 2, rV1 = 3, rA0 = 4, rA1 = 5, rA2 = 6, rA3 = 7,
        rT0 = 8, rT1 = 9, rT2 = 10, rT3 = 11,
        rS0 = 16, rS1 = 17, rS2 = 18, rS3 = 19, rS4 = 20, rS5 = 21, rS6 = 22,
        rSP = 29, rRA = 31
    };

    constexpr uint32_t kExecBase = 0x01455FA8u, kExecResume = 0x01456078u;
    constexpr uint32_t kSubFunc = 0x01450BB8u, kCondExecFunc = 0x01450B60u;
    constexpr uint32_t kCtrlWork = 0x01455F80u, kAiWork = 0x013CFAE8u, kCheckAiWork = 0x013CFB08u, kFirstObj = 0x0137C348u;
    constexpr uint32_t kLastMotion = 0x01457648u, kControlMode = 0x01457708u, kParentCol = 0x01456A38u, kSoundParentCol = 0x01464ED0u;
    constexpr uint32_t kRootAngle = 0x01457CF8u, kEventDemo = 0x01451468u, kVolumeCheck = 0x014582D8u;
    constexpr uint32_t kTurnDirect = 0x0137C798u, kLegalParent = 0x0137D640u;

    // Callees (constant jal targets of the generated code).
    constexpr uint32_t kExecCondition = 0x01455AA0u;    // decomp_hot44b
    constexpr uint32_t kGetFirstObj = 0x01342D80u;      // GAMEUTIL_get_first_obj
    constexpr uint32_t kPlayerColCont2 = 0x013B5390u;   // getPlayerColCont2
    constexpr uint32_t kIsParentObj = 0x0139AAA0u;      // ColCont2_IsParentObj
    constexpr uint32_t kIsColStatus = 0x0139AA00u;      // ColCont2_IsColStatus
    constexpr uint32_t kIsEventColStatus = 0x0139AA50u; // ColCont2_IsEventColStatus
    constexpr uint32_t kSScriptWork = 0x01465308u;      // getSScriptWork
    constexpr uint32_t kGameClearCnt = 0x0134F278u, kGameHardMode = 0x0134F2A8u, kGameFlagChk = 0x0134F030u;
    constexpr uint32_t kCreateEventDemo = 0x0145AF30u;  // createScrEventDemoManager

    // ---- the generated code's statement forms on the context ----------------
#define V2_U(r) GPR_U32(ctx, r)
#define V2_Q(r) GPR_U64(ctx, r)
#define V2_LW(rt, addr) SET_GPR_S32(ctx, rt, (int32_t)READ32(addr))
#define V2_MOVE(rd, rs) SET_GPR_U64(ctx, rd, (uint64_t)GPR_U64(ctx, rs) + (uint64_t)GPR_U64(ctx, 0))
#define V2_ZERO(rd) SET_GPR_U64(ctx, rd, 0u)
#define V2_LI(rt, v) SET_GPR_S32(ctx, rt, (int32_t)(v))
#define V2_SD(addr, r) WRITE64(addr, GPR_U64(ctx, r))
#define V2_LD(r, addr) SET_GPR_U64(ctx, r, READ64(addr))
#define V2_ADDIU(rt, rs, imm) SET_GPR_S32(ctx, rt, (int32_t)ADD32(GPR_U32(ctx, rs), (uint32_t)(int32_t)(imm)))
#define V2_SUBU(rd, rs, rt) SET_GPR_S32(ctx, rd, (int32_t)SUB32(GPR_U32(ctx, rs), GPR_U32(ctx, rt)))
#define V2_ADDU(rd, rs, rt) SET_GPR_S32(ctx, rd, (int32_t)ADD32(GPR_U32(ctx, rs), GPR_U32(ctx, rt)))
#define V2_AND(rd, rs, rt) SET_GPR_U64(ctx, rd, GPR_U64(ctx, rs) & GPR_U64(ctx, rt))
#define V2_ORI(rt, rs, imm) SET_GPR_U64(ctx, rt, GPR_U64(ctx, rs) | (uint64_t)(uint16_t)(imm))
#define V2_ANDI(rt, rs, imm) SET_GPR_U64(ctx, rt, GPR_U64(ctx, rs) & (uint64_t)(uint16_t)(imm))
#define V2_SRL(rd, rt, sa) SET_GPR_S32(ctx, rd, (int32_t)SRL32(GPR_U32(ctx, rt), sa))
// lui at a relocated site (the immediate from the live code word).
#define V2_LUI_AT(rt, site) SET_GPR_S32(ctx, rt, (int32_t)((uint32_t)(READ32(site) & 0xFFFFu) << 16))
// lw rt, simm(base) with the immediate from the live code word at site.
#define V2_LW_AT(rt, base, site) SET_GPR_S32(ctx, rt, (int32_t)READ32(ADD32(GPR_U32(ctx, base), (uint32_t)(int32_t)(int16_t)(READ32(site) & 0xFFFFu))))
#define V2_SWC1(addr, fr) do { float f_ = ctx->f[fr]; uint32_t b_; std::memcpy(&b_, &f_, 4); WRITE32(addr, b_); } while (0)
#define V2_LWC1(fr, addr) do { uint32_t b_ = READ32(addr); float f_; std::memcpy(&f_, &b_, 4); ctx->f[fr] = f_; } while (0)

    // mult rd, rs, rt (the EE's three-operand form): HI / LO and rd.
    inline void mult(R5900Context *ctx, int rd, int rs, int rt)
    {
        int64_t result = (int64_t)GPR_S32(ctx, rs) * (int64_t)GPR_S32(ctx, rt);
        ctx->lo = (uint64_t)(int64_t)(int32_t)result;
        ctx->hi = (uint64_t)(int64_t)(int32_t)(result >> 32);
        SET_GPR_S32(ctx, rd, (int32_t)result);
    }

    // The recompiled function from its entry.
    inline void fallback(uint32_t self, uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
    {
        ctx->pc = self;
        sotc_decomp::recompiled(self)(rdram, ctx, runtime);
    }
    // A coroutine's: from where pc is (its entry or a label).
    inline void coFallback(uint32_t self, uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
    {
        sotc_decomp::recompiled(self)(rdram, ctx, runtime);
    }

    // jr $ra.
    inline void ret(R5900Context *ctx) { ctx->pc = GPR_U32(ctx, 31); }

    // ---- execSCRBaseFunc: the record walk ------------------------------------
    void execSCRBaseFunc(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
    {
        if (!sotc_decomp::scriptVmOn())
            return coFallback(kExecBase, rdram, ctx, runtime);
        uint32_t sp = 0;
        if (ctx->pc == kExecResume)
        {
            sp = V2_U(rSP);
            goto resume;
        }
        if (ctx->pc != kExecBase)
            return coFallback(kExecBase, rdram, ctx, runtime);

        // The frame (0x30: sp + 0 the record's sp word, s0 0x10, s1 0x18, ra 0x20).
        V2_ADDIU(rSP, rSP, -0x30);
        sp = V2_U(rSP);
        V2_LI(rV0, 0x30000);
        V2_SD(sp + 24u, rS1);
        V2_SD(sp + 16u, rS0);
        V2_ORI(rV0, rV0, 0xFFFF);
        V2_SD(sp + 32u, rRA);
        // The list handle: its owner.
        V2_LW(rA2, V2_U(rA1));
        V2_AND(rV0, rA2, rV0);
        if (V2_Q(rV0) == 0u)
        {
            V2_MOVE(rS1, rA0); // the object
            V2_ZERO(rA0);
        }
        else
        {
            V2_MOVE(rS1, rA0);
            V2_SUBU(rV0, rA1, rV0);
            V2_LW(rA0, V2_U(rV0));
        }
        // 0x1455fe4: an owner, not the null one: its element.
        V2_LUI_AT(rV0, 0x01455FE8u);
        if (V2_Q(rA0) == 0u)
            goto none;
        V2_LW_AT(rV1, rV0, 0x01455FECu);
        V2_SRL(rV0, rA2, 18);
        if (V2_Q(rA0) == V2_Q(rV1))
            goto none;
        V2_LW(rA1, V2_U(rA0) + 8u);
        V2_ANDI(rV0, rV0, 0xFFF);
        V2_LW(rA0, V2_U(rA0) + 0x10u);
        V2_LW(rV1, V2_U(rA1) + 0xCu);
        mult(ctx, rV0, rV0, rV1);
        V2_ADDU(rV0, rA0, rV0);
        goto have;
    none:
        V2_ZERO(rV0);
    have:
        V2_MOVE(rS0, rV0);
        if (V2_Q(rS0) == 0u)
        {
            V2_LD(rS0, sp + 16u);
            goto leave;
        }
        V2_LW(rV0, V2_U(rS0));
        if (V2_Q(rV0) == 0u)
        {
            V2_LD(rS0, sp + 16u);
            goto leave;
        }
        V2_MOVE(rV1, rV0);
        for (;;)
        {
            // 0x1456038: the record's arguments, s0 to the next record.
            V2_ADDIU(rV0, rS0, 0x3C);
            V2_LW(rA1, V2_U(rS0) + 4u);
            V2_LW(rA2, V2_U(rS0) + 8u);
            V2_ADDIU(rT1, rS0, 0x30);
            V2_LWC1(12, V2_U(rS0) + 12u);
            V2_ADDIU(rT2, rS0, 0x34);
            V2_LWC1(13, V2_U(rS0) + 16u);
            V2_ADDIU(rT3, rS0, 0x38);
            V2_LWC1(14, V2_U(rS0) + 20u);
            V2_MOVE(rA0, rS1);
            V2_LWC1(15, V2_U(rS0) + 24u);
            V2_LD(rA3, V2_U(rS0) + 32u);
            V2_LD(rT0, V2_U(rS0) + 40u);
            V2_ADDIU(rS0, rS0, 0x40);
            WRITE32(sp, V2_U(rV0)); // the jalr's slot: sw $v0, 0($sp)
            if (!sotc_decomp::callMaySleep(rdram, ctx, runtime, V2_U(rV1), kExecResume, sp))
                return;
        resume:
            if (!sotc_decomp::scriptVmOn())
                return coFallback(kExecBase, rdram, ctx, runtime);
            V2_LW(rV0, V2_U(rS0));
            V2_MOVE(rV1, rV0);
            if (V2_Q(rV0) == 0u)
                break;
        }
        V2_LD(rS0, sp + 16u);
    leave:
        V2_LD(rS1, sp + 24u);
        V2_LD(rRA, sp + 32u);
        V2_ADDIU(rSP, rSP, 0x30);
        ret(ctx);
    }

    // A tail jump into execSCRBaseFunc (ra and sp as the jump leaves them):
    // this native directly while the table holds it.
    inline void tailExec(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
    {
        ctx->pc = kExecBase;
        const PS2Runtime::RecompiledFunction fn = sotc_decomp::guestFunction(runtime, kExecBase);
        if (fn == execSCRBaseFunc)
            return execSCRBaseFunc(rdram, ctx, runtime);
        if (fn)
            fn(rdram, ctx, runtime); // else the dispatcher takes pc
    }

    // ---- SCRFuncSubFunc: execSCRBaseFunc(a0, t3) --------------------------------
    void scrFuncSubFunc(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
    {
        if (!sotc_decomp::scriptVmOn() || ctx->pc != kSubFunc)
            return coFallback(kSubFunc, rdram, ctx, runtime);
        V2_ADDIU(rSP, rSP, -0x10);
        const uint32_t sp = V2_U(rSP);
        V2_MOVE(rA1, rT3);
        V2_SD(sp, rRA);
        V2_LD(rRA, sp);
        V2_ADDIU(rSP, rSP, 0x10);
        tailExec(rdram, ctx, runtime);
    }

    // ---- SCRFuncConditionExecFunc: execCondition(a0, t2) ? execSCRBaseFunc(a0, t3) : 0
    void scrFuncConditionExecFunc(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
    {
        if (!sotc_decomp::scriptVmOn() || ctx->pc != kCondExecFunc)
            return coFallback(kCondExecFunc, rdram, ctx, runtime);
        V2_ADDIU(rSP, rSP, -0x20);
        const uint32_t sp = V2_U(rSP);
        V2_MOVE(rA1, rT2);
        V2_SD(sp, rS0);
        V2_MOVE(rS0, rA0);
        V2_SD(sp + 8u, rS1);
        V2_SD(sp + 16u, rRA);
        V2_MOVE(rS1, rT3);
        callGuest(rdram, ctx, runtime, kExecCondition, 0x01450B80u, sp); // cannot sleep
        V2_MOVE(rA0, rS0);
        V2_MOVE(rA1, rS1);
        const bool run = V2_Q(rV0) != 0u;
        V2_LD(rS0, sp);
        V2_LD(rS1, sp + 8u);
        V2_LD(rRA, sp + 16u);
        V2_ADDIU(rSP, rSP, 0x20);
        if (run)
            return tailExec(rdram, ctx, runtime);
        ret(ctx);
    }

    // ---- GetSCRCharaAIWorkPtr: GetScriptCharacterClass(a0) +0x1200 ---------------
    void getSCRCharaAIWorkPtr(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
    {
        if (!sotc_decomp::scriptVmOn())
            return fallback(kAiWork, rdram, ctx, runtime);
        V2_ADDIU(rSP, rSP, -0x10);
        const uint32_t sp = V2_U(rSP);
        V2_SD(sp, rRA);
        callGuest(rdram, ctx, runtime, jalTarget(rdram, 0x013CFAF0u), 0x013CFAF8u, sp);
        V2_LW(rV0, V2_U(rV0) + 0x1200u);
        V2_LD(rRA, sp);
        V2_ADDIU(rSP, rSP, 0x10);
        ret(ctx);
    }

    // ---- CheckSCRCharaAIWorkPtr: the same, 0 for no object -------------------------
    void checkSCRCharaAIWorkPtr(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
    {
        if (!sotc_decomp::scriptVmOn())
            return fallback(kCheckAiWork, rdram, ctx, runtime);
        V2_ADDIU(rSP, rSP, -0x10);
        const uint32_t sp = V2_U(rSP);
        V2_ZERO(rV0);
        V2_SD(sp, rRA);
        if (V2_Q(rA0) != 0u)
        {
            callGuest(rdram, ctx, runtime, jalTarget(rdram, 0x013CFB18u), 0x013CFB20u, sp);
            V2_LW(rV0, V2_U(rV0) + 0x1200u);
        }
        V2_LD(rRA, sp);
        V2_ADDIU(rSP, rSP, 0x10);
        ret(ctx);
    }

    // ---- getSCRDataCtrlWork: the AI work rounded up to 16 bytes ------------------
    void getSCRDataCtrlWork(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
    {
        if (!sotc_decomp::scriptVmOn())
            return fallback(kCtrlWork, rdram, ctx, runtime);
        V2_ADDIU(rSP, rSP, -0x10);
        const uint32_t sp = V2_U(rSP);
        V2_SD(sp, rRA);
        sotc_decomp::callNative(rdram, ctx, runtime, kAiWork, getSCRCharaAIWorkPtr, 0x01455F90u, sp);
        V2_ADDIU(rV0, rV0, 0x13);
        V2_LD(rRA, sp);
        V2_LI(rV1, -0x10);
        V2_AND(rV0, rV0, rV1);
        V2_ADDIU(rSP, rSP, 0x10);
        ret(ctx);
    }

    // A call of getSCRDataCtrlWork from this file.
    inline void ctrlWork(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t ra, uint32_t sp)
    {
        sotc_decomp::callNative(rdram, ctx, runtime, kCtrlWork, getSCRDataCtrlWork, ra, sp);
    }

    // ---- SCRGetFirstObj: GAMEUTIL_get_first_obj as a tail jump -------------------
    void scrGetFirstObj(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
    {
        if (!sotc_decomp::scriptVmOn())
            return fallback(kFirstObj, rdram, ctx, runtime);
        V2_ADDIU(rSP, rSP, -0x10);
        const uint32_t sp = V2_U(rSP);
        V2_SD(sp, rRA);
        V2_LD(rRA, sp);
        V2_ADDIU(rSP, rSP, 0x10);
        callGuest(rdram, ctx, runtime, kGetFirstObj, V2_U(rRA), V2_U(rSP));
    }

    // ---- SCRConditionPlayerLastMotionShiftCheck ------------------------------------
    // The handle at the caller's sp + 0 word; v0 1 when the player's motion
    // (class +0x1c0 +4) is its element and the class's +0x360 byte is set.
    void scrConditionPlayerLastMotionShiftCheck(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
    {
        if (!sotc_decomp::scriptVmOn())
            return fallback(kLastMotion, rdram, ctx, runtime);
        V2_ADDIU(rSP, rSP, -0x20);
        const uint32_t sp = V2_U(rSP);
        V2_SD(sp, rS0);
        V2_SD(sp + 8u, rS1);
        V2_LW(rS0, sp + 0x20u);
        V2_SD(sp + 0x10u, rRA);
        ctrlWork(rdram, ctx, runtime, 0x01457664u, sp);
        V2_MOVE(rS1, rV0);
        V2_LW(rA0, V2_U(rS1) + 0x4078u);
        callGuest(rdram, ctx, runtime, jalTarget(rdram, 0x01457668u), 0x01457670u, sp); // GetCharacterClass
        V2_ZERO(rA1);
        V2_LW(rA2, V2_U(rS0));
        V2_LI(rV1, 0x30000);
        V2_ORI(rV1, rV1, 0xFFFF);
        V2_ADDIU(rA3, rV0, 0x1C0);
        V2_AND(rV1, rA2, rV1);
        V2_ZERO(rT0);
        {
            const bool empty = V2_Q(rV1) == 0u;
            V2_SUBU(rS0, rS0, rV1);
            if (empty)
                V2_ZERO(rS0);
            else
                V2_LW(rS0, V2_U(rS0));
        }
        V2_LUI_AT(rV0, 0x014576A8u);
        if (V2_Q(rS0) != 0u)
        {
            V2_LW_AT(rV1, rV0, 0x014576ACu);
            V2_SRL(rV0, rA2, 18);
            if (V2_Q(rS0) != V2_Q(rV1))
            {
                V2_LW(rA0, V2_U(rS0) + 8u);
                V2_ANDI(rV0, rV0, 0xFFF);
                V2_LW(rA1, V2_U(rS0) + 0x10u);
                V2_LW(rV1, V2_U(rA0) + 0xCu);
                mult(ctx, rV0, rV0, rV1);
                V2_ADDU(rA1, rA1, rV0);
            }
        }
        // 0x14576d0: the class's motion against the element.
        V2_LW(rV0, V2_U(rA3) + 4u);
        {
            const bool differ = V2_Q(rA1) != V2_Q(rV0);
            V2_LD(rS0, sp);
            if (!differ)
            {
                V2_LW(rA0, V2_U(rS1) + 0x4078u);
                callGuest(rdram, ctx, runtime, jalTarget(rdram, 0x014576DCu), 0x014576E4u, sp); // GetCharacterClass
                SET_GPR_S32(ctx, rV1, (int8_t)READ8(ADD32(V2_U(rV0), 0x360u)));
                SET_GPR_U64(ctx, rT0, ((uint64_t)GPR_U64(ctx, 0) < (uint64_t)V2_Q(rV1)) ? 1 : 0);
                V2_LD(rS0, sp);
            }
        }
        V2_MOVE(rV0, rT0);
        V2_LD(rS1, sp + 8u);
        V2_LD(rRA, sp + 0x10u);
        V2_ADDIU(rSP, rSP, 0x20);
        ret(ctx);
    }

    // ---- SCRConditionPlayerControlModeCheck ----------------------------------------
    // v0 1 when the player's control mode (class +0x750) is the handle's
    // (caller's sp + 0 word) element.
    void scrConditionPlayerControlModeCheck(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
    {
        if (!sotc_decomp::scriptVmOn())
            return fallback(kControlMode, rdram, ctx, runtime);
        V2_ADDIU(rSP, rSP, -0x10);
        const uint32_t sp = V2_U(rSP);
        V2_SD(sp, rS0);
        V2_LW(rS0, sp + 0x10u);
        V2_SD(sp + 8u, rRA);
        ctrlWork(rdram, ctx, runtime, 0x01457720u, sp);
        V2_MOVE(rA3, rV0);
        V2_LI(rV0, 0x30000);
        V2_LW(rA2, V2_U(rS0));
        V2_ORI(rV0, rV0, 0xFFFF);
        V2_ZERO(rA0);
        V2_AND(rV0, rA2, rV0);
        {
            const bool empty = V2_Q(rV0) == 0u;
            V2_SUBU(rS0, rS0, rV0);
            if (!empty)
                V2_LW(rA0, V2_U(rS0));
        }
        V2_LUI_AT(rV0, 0x01457748u);
        if (V2_Q(rA0) == 0u)
            goto none;
        V2_LW_AT(rV1, rV0, 0x0145774Cu);
        V2_SRL(rV0, rA2, 18);
        if (V2_Q(rA0) == V2_Q(rV1))
            goto none;
        V2_LW(rA1, V2_U(rA0) + 8u);
        V2_ANDI(rV0, rV0, 0xFFF);
        V2_LW(rA0, V2_U(rA0) + 0x10u);
        V2_LW(rV1, V2_U(rA1) + 0xCu);
        mult(ctx, rV0, rV0, rV1);
        V2_ADDU(rS0, rA0, rV0);
        goto have;
    none:
        V2_ZERO(rS0);
    have:
        V2_LW(rA0, V2_U(rA3) + 0x4078u);
        callGuest(rdram, ctx, runtime, jalTarget(rdram, 0x0145777Cu), 0x01457784u, sp); // GetCharacterClass
        V2_LW(rV0, V2_U(rV0) + 0x750u);
        V2_LD(rRA, sp + 8u);
        SET_GPR_U64(ctx, rV0, V2_Q(rS0) ^ V2_Q(rV0));
        V2_LD(rS0, sp);
        SET_GPR_U64(ctx, rV0, ((uint64_t)V2_Q(rV0) < (uint64_t)(int64_t)(int32_t)1) ? 1 : 0);
        V2_ADDIU(rSP, rSP, 0x10);
        ret(ctx);
    }

    // ---- SCRConditionPlayerParentColAttr / SCRSoundConditionPlayerParentColAttr --
    // t3 the parent handle, t0 / t1 the attribute.
    void scrConditionPlayerParentColAttr(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
    {
        if (!sotc_decomp::scriptVmOn())
            return fallback(kParentCol, rdram, ctx, runtime);
        V2_ADDIU(rSP, rSP, -0x30);
        const uint32_t sp = V2_U(rSP);
        V2_SD(sp + 8u, rS1);
        V2_MOVE(rS1, rT3);
        V2_SD(sp + 0x10u, rS2);
        V2_SD(sp + 0x18u, rS3);
        V2_MOVE(rS3, rT0);
        V2_SD(sp + 0x20u, rS4);
        V2_SD(sp, rS0);
        V2_SD(sp + 0x28u, rRA);
        V2_MOVE(rS4, rT1);
        ctrlWork(rdram, ctx, runtime, 0x01456A64u, sp);
        V2_MOVE(rS2, rV0);
        V2_LI(rV0, 0x30000);
        V2_LW(rA0, V2_U(rS1));
        V2_ORI(rV0, rV0, 0xFFFF);
        V2_AND(rV0, rA0, rV0);
        {
            const bool empty = V2_Q(rV0) == 0u;
            V2_SUBU(rV0, rS1, rV0);
            if (empty)
                V2_ZERO(rA1);
            else
                V2_LW(rA1, V2_U(rV0));
        }
        // 0x1456a8c: the parent object (owner +0x10).
        V2_LUI_AT(rV0, 0x01456A90u);
        if (V2_Q(rA1) == 0u)
            V2_ZERO(rS0);
        else
        {
            V2_LW_AT(rV1, rV0, 0x01456A94u);
            if (V2_Q(rA1) == V2_Q(rV1))
                V2_ZERO(rS0);
            else
                V2_LW(rS0, V2_U(rA1) + 0x10u);
        }
        // 0x1456aac
        {
            const bool noParent = V2_Q(rS0) == 0u;
            V2_LI(rV0, 0x30000);
            if (noParent)
                goto noParentObj;
        }
        V2_LW(rV0, V2_U(rS0) + 8u);
        if (V2_Q(rV0) == 0u)
        {
            V2_LI(rV0, 0x30000);
            goto noParentObj;
        }
        V2_LW(rA0, V2_U(rS2) + 0x4078u);
        V2_ZERO(rS1);
        callGuest(rdram, ctx, runtime, kPlayerColCont2, 0x01456ACCu, sp);
        V2_MOVE(rA1, rS0);
        V2_MOVE(rA0, rV0);
        callGuest(rdram, ctx, runtime, kIsParentObj, 0x01456AD8u, sp);
        if (V2_Q(rV0) != 0u)
        {
            V2_LI(rS1, 1);
            goto zeroV0;
        }
        V2_ZERO(rV0);
        goto test;
    noParentObj: // 0x1456ae8: true when the handle is empty
        V2_ORI(rV0, rV0, 0xFFFF);
        V2_AND(rV0, rA0, rV0);
        {
            const bool empty = V2_Q(rV0) == 0u;
            V2_SUBU(rV0, rS1, rV0);
            if (empty)
                V2_ZERO(rV0);
            else
                V2_LW(rV0, V2_U(rV0));
        }
        SET_GPR_U64(ctx, rS1, ((uint64_t)V2_Q(rV0) < (uint64_t)(int64_t)(int32_t)1) ? 1 : 0);
    zeroV0:
        V2_ZERO(rV0);
    test:
        {
            const bool no = V2_Q(rS1) == 0u;
            V2_LD(rS0, sp);
            if (!no)
            {
                V2_LW(rA0, V2_U(rS2) + 0x4078u);
                callGuest(rdram, ctx, runtime, kPlayerColCont2, 0x01456B1Cu, sp);
                V2_MOVE(rA1, rS3);
                V2_MOVE(rA0, rV0);
                V2_MOVE(rA2, rS4);
                callGuest(rdram, ctx, runtime, kIsColStatus, 0x01456B2Cu, sp);
                SET_GPR_U64(ctx, rV0, ((uint64_t)GPR_U64(ctx, 0) < (uint64_t)V2_Q(rV0)) ? 1 : 0);
                V2_LD(rS0, sp);
            }
        }
        V2_LD(rS1, sp + 8u);
        V2_LD(rS2, sp + 0x10u);
        V2_LD(rS3, sp + 0x18u);
        V2_LD(rS4, sp + 0x20u);
        V2_LD(rRA, sp + 0x28u);
        V2_ADDIU(rSP, rSP, 0x30);
        ret(ctx);
    }

    void scrSoundConditionPlayerParentColAttr(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
    {
        if (!sotc_decomp::scriptVmOn())
            return fallback(kSoundParentCol, rdram, ctx, runtime);
        V2_ADDIU(rSP, rSP, -0x30);
        const uint32_t sp = V2_U(rSP);
        V2_SD(sp + 8u, rS1);
        V2_MOVE(rS1, rT3);
        V2_SD(sp + 0x10u, rS2);
        V2_SD(sp + 0x18u, rS3);
        V2_MOVE(rS3, rT0);
        V2_SD(sp + 0x20u, rS4);
        V2_SD(sp, rS0);
        V2_SD(sp + 0x28u, rRA);
        V2_MOVE(rS4, rT1);
        callGuest(rdram, ctx, runtime, kSScriptWork, 0x01464EFCu, sp);
        V2_MOVE(rS2, rV0);
        V2_LI(rV0, 0x30000);
        V2_LW(rA2, V2_U(rS1));
        V2_ORI(rV0, rV0, 0xFFFF);
        V2_AND(rV0, rA2, rV0);
        {
            const bool empty = V2_Q(rV0) == 0u;
            V2_SUBU(rV0, rS1, rV0);
            if (empty)
                V2_ZERO(rA1);
            else
                V2_LW(rA1, V2_U(rV0));
        }
        V2_LUI_AT(rV0, 0x01464F28u);
        if (V2_Q(rA1) == 0u)
            V2_ZERO(rS0);
        else
        {
            V2_LW_AT(rV1, rV0, 0x01464F2Cu);
            if (V2_Q(rA1) == V2_Q(rV1))
                V2_ZERO(rS0);
            else
                V2_LW(rS0, V2_U(rA1) + 0x10u);
        }
        // 0x1464f44: no player: v0 0.
        V2_LW(rA0, V2_U(rS2));
        {
            const bool noPlayer = V2_Q(rA0) == 0u;
            V2_ZERO(rV0);
            if (noPlayer)
            {
                V2_LD(rS0, sp);
                goto leave;
            }
        }
        if (V2_Q(rS0) == 0u)
        {
            V2_LI(rV0, 0x30000);
            goto noParentObj;
        }
        V2_ZERO(rS1);
        callGuest(rdram, ctx, runtime, kPlayerColCont2, 0x01464F60u, sp);
        V2_MOVE(rA1, rS0);
        V2_MOVE(rA0, rV0);
        callGuest(rdram, ctx, runtime, kIsParentObj, 0x01464F6Cu, sp);
        if (V2_Q(rV0) != 0u)
        {
            V2_LI(rS1, 1);
            goto zeroV0;
        }
        V2_ZERO(rV0);
        goto test;
    noParentObj: // 0x1464f80
        V2_ORI(rV0, rV0, 0xFFFF);
        V2_AND(rV0, rA2, rV0);
        {
            const bool empty = V2_Q(rV0) == 0u;
            V2_SUBU(rV0, rS1, rV0);
            if (empty)
                V2_ZERO(rV0);
            else
                V2_LW(rV0, V2_U(rV0));
        }
        SET_GPR_U64(ctx, rS1, ((uint64_t)V2_Q(rV0) < (uint64_t)(int64_t)(int32_t)1) ? 1 : 0);
    zeroV0:
        V2_ZERO(rV0);
    test:
        {
            const bool no = V2_Q(rS1) == 0u;
            V2_LD(rS0, sp);
            if (no)
                goto leave;
        }
        V2_LW(rA0, V2_U(rS2));
        callGuest(rdram, ctx, runtime, kPlayerColCont2, 0x01464FB4u, sp);
        V2_MOVE(rA1, rS3);
        V2_MOVE(rA0, rV0);
        V2_MOVE(rA2, rS4);
        callGuest(rdram, ctx, runtime, kIsEventColStatus, 0x01464FC4u, sp);
        SET_GPR_U64(ctx, rV0, ((uint64_t)GPR_U64(ctx, 0) < (uint64_t)V2_Q(rV0)) ? 1 : 0);
        V2_LD(rS0, sp);
    leave:
        V2_LD(rS1, sp + 8u);
        V2_LD(rS2, sp + 0x10u);
        V2_LD(rS3, sp + 0x18u);
        V2_LD(rS4, sp + 0x20u);
        V2_LD(rRA, sp + 0x28u);
        V2_ADDIU(rSP, rSP, 0x30);
        ret(ctx);
    }

    // ---- SCRConditionRootAnglePlayerWorldXZ ----------------------------------------
    // a0 the object, f12 / f13 the angle range in degrees.
    void scrConditionRootAnglePlayerWorldXZ(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
    {
        if (!sotc_decomp::scriptVmOn())
            return fallback(kRootAngle, rdram, ctx, runtime);
        V2_ADDIU(rSP, rSP, -0x80);
        const uint32_t sp = V2_U(rSP);
        V2_SD(sp + 0x58u, rS1);
        V2_MOVE(rS1, rA0);
        V2_SD(sp + 0x50u, rS0);
        V2_SD(sp + 0x60u, rS2);
        V2_SWC1(sp + 0x78u, 21);
        ctx->f[21] = FPU_MOV_S(ctx->f[13]);
        V2_SWC1(sp + 0x70u, 20);
        V2_SD(sp + 0x68u, rRA);
        ctx->f[20] = FPU_MOV_S(ctx->f[12]);
        ctrlWork(rdram, ctx, runtime, 0x01457D24u, sp);
        V2_MOVE(rA0, rS1);
        V2_MOVE(rS2, rV0);
        callGuest(rdram, ctx, runtime, jalTarget(rdram, 0x01457D28u), 0x01457D30u, sp); // GetLayoutClass
        V2_MOVE(rA0, rS1);
        V2_MOVE(rS0, rV0);
        callGuest(rdram, ctx, runtime, jalTarget(rdram, 0x01457D34u), 0x01457D3Cu, sp); // GetCharacterClass
        // 0x1457d3c..0x1457e8c: the generated code's statements as they are
        // (the object's matrix from the class's +0x470 quaternion and the
        // layout's position, its inverse, the player's position through it,
        // normalised).
        // 0x1457d3c: 0x3a0182d  daddu       $v1, $sp, $zero
        SET_GPR_U64(ctx, 3, (uint64_t)GPR_U64(ctx, 29) + (uint64_t)GPR_U64(ctx, 0));
        // 0x1457d40: 0x24420470  addiu       $v0, $v0, 0x470
        SET_GPR_S32(ctx, 2, (int32_t)ADD32(GPR_U32(ctx, 2), 1136));
        // 0x1457d44: 0x8e040134  lw          $a0, 0x134($s0)
        SET_GPR_S32(ctx, 4, (int32_t)READ32(ADD32(GPR_U32(ctx, 16), 308)));
        // 0x1457d48: 0x4bc00303  vaddw.xyz   $vf12, $vf0, $vf0w
        { __m128 res = PS2_VADD(ctx->vu0_vf[0], _mm_shuffle_ps(ctx->vu0_vf[0], ctx->vu0_vf[0], _MM_SHUFFLE(3,3,3,3))); __m128i mask = _mm_set_epi32(0, -1, -1, -1); ctx->vu0_vf[12] = _mm_blendv_ps(ctx->vu0_vf[12], res, _mm_castsi128_ps(mask)); }
        // 0x1457d4c: 0xd84b0000  lqc2        $vf11, 0x0($v0)
        ctx->vu0_vf[11] = _mm_castsi128_ps(READ128(ADD32(GPR_U32(ctx, 2), 0)));
        // 0x1457d50: 0x4a2e033d  vmr32.w     $vf14, $vf0
        { __m128 res = _mm_shuffle_ps(ctx->vu0_vf[0], ctx->vu0_vf[0], _MM_SHUFFLE(0,3,2,1)); __m128i mask = _mm_set_epi32(-1, 0, 0, 0); ctx->vu0_vf[14] = _mm_blendv_ps(ctx->vu0_vf[14], res, _mm_castsi128_ps(mask)); }
        // 0x1457d54: 0x4a2f033d  vmr32.w     $vf15, $vf0
        { __m128 res = _mm_shuffle_ps(ctx->vu0_vf[0], ctx->vu0_vf[0], _MM_SHUFFLE(0,3,2,1)); __m128i mask = _mm_set_epi32(-1, 0, 0, 0); ctx->vu0_vf[15] = _mm_blendv_ps(ctx->vu0_vf[15], res, _mm_castsi128_ps(mask)); }
        // 0x1457d58: 0x4a30033d  vmr32.w     $vf16, $vf0
        { __m128 res = _mm_shuffle_ps(ctx->vu0_vf[0], ctx->vu0_vf[0], _MM_SHUFFLE(0,3,2,1)); __m128i mask = _mm_set_epi32(-1, 0, 0, 0); ctx->vu0_vf[16] = _mm_blendv_ps(ctx->vu0_vf[16], res, _mm_castsi128_ps(mask)); }
        // 0x1457d5c: 0x4bcc5afe  vopmula.xyz $ACC, $vf11, $vf12
        { __m128 fs_yzx = _mm_shuffle_ps(ctx->vu0_vf[11], ctx->vu0_vf[11], _MM_SHUFFLE(3,0,2,1)); __m128 ft_zxy = _mm_shuffle_ps(ctx->vu0_vf[12], ctx->vu0_vf[12], _MM_SHUFFLE(3,1,0,2)); __m128 res = PS2_VMUL(fs_yzx, ft_zxy); ctx->vu0_acc = _mm_blendv_ps(ctx->vu0_acc, res, _mm_castsi128_ps(_mm_set_epi32(0, -1, -1, -1))); }
        // 0x1457d60: 0x4bc002e9  vmadd.xyz   $vf11, $vf0, $vf0
        { __m128 mul_res = PS2_VMUL(ctx->vu0_vf[0], ctx->vu0_vf[0]); __m128 res = PS2_VADD(ctx->vu0_acc, mul_res); __m128i mask = _mm_set_epi32(0, -1, -1, -1); ctx->vu0_vf[11] = _mm_blendv_ps(ctx->vu0_vf[11], res, _mm_castsi128_ps(mask)); }
        // 0x1457d64: 0x4bcc5aa9  vmadd.xyz   $vf10, $vf11, $vf12
        { __m128 mul_res = PS2_VMUL(ctx->vu0_vf[11], ctx->vu0_vf[12]); __m128 res = PS2_VADD(ctx->vu0_acc, mul_res); __m128i mask = _mm_set_epi32(0, -1, -1, -1); ctx->vu0_vf[10] = _mm_blendv_ps(ctx->vu0_vf[10], res, _mm_castsi128_ps(mask)); }
        // 0x1457d68: 0x4bca5b6a  vmul.xyz    $vf13, $vf11, $vf10
        { __m128 res = PS2_VMUL(ctx->vu0_vf[11], ctx->vu0_vf[10]); __m128i mask = _mm_set_epi32(0, -1, -1, -1); ctx->vu0_vf[13] = PS2_VBLEND(ctx->vu0_vf[13], res, _mm_castsi128_ps(mask)); }
        // 0x1457d6c: 0x4bca5afe  vopmula.xyz $ACC, $vf11, $vf10
        { __m128 fs_yzx = _mm_shuffle_ps(ctx->vu0_vf[11], ctx->vu0_vf[11], _MM_SHUFFLE(3,0,2,1)); __m128 ft_zxy = _mm_shuffle_ps(ctx->vu0_vf[10], ctx->vu0_vf[10], _MM_SHUFFLE(3,1,0,2)); __m128 res = PS2_VMUL(fs_yzx, ft_zxy); ctx->vu0_acc = _mm_blendv_ps(ctx->vu0_acc, res, _mm_castsi128_ps(_mm_set_epi32(0, -1, -1, -1))); }
        // 0x1457d70: 0x4bcb53cb  vmaddw.xyz  $vf15, $vf10, $vf11w
        { __m128 mul_res = PS2_VMUL(ctx->vu0_vf[10], _mm_shuffle_ps(ctx->vu0_vf[11], ctx->vu0_vf[11], _MM_SHUFFLE(3,3,3,3))); __m128 res = PS2_VADD(ctx->vu0_acc, mul_res); __m128i mask = _mm_set_epi32(0, -1, -1, -1); ctx->vu0_vf[15] = _mm_blendv_ps(ctx->vu0_vf[15], res, _mm_castsi128_ps(mask)); }
        // 0x1457d74: 0x4bcb540f  vmsubw.xyz  $vf16, $vf10, $vf11w
        { __m128 mul_res = PS2_VMUL(ctx->vu0_vf[10], _mm_shuffle_ps(ctx->vu0_vf[11], ctx->vu0_vf[11], _MM_SHUFFLE(3,3,3,3))); __m128 res = PS2_VSUB(ctx->vu0_acc, mul_res); __m128i mask = _mm_set_epi32(0, -1, -1, -1); ctx->vu0_vf[16] = _mm_blendv_ps(ctx->vu0_vf[16], res, _mm_castsi128_ps(mask)); }
        // 0x1457d78: 0x4bcc6afe  vopmula.xyz $ACC, $vf13, $vf12
        { __m128 fs_yzx = _mm_shuffle_ps(ctx->vu0_vf[13], ctx->vu0_vf[13], _MM_SHUFFLE(3,0,2,1)); __m128 ft_zxy = _mm_shuffle_ps(ctx->vu0_vf[12], ctx->vu0_vf[12], _MM_SHUFFLE(3,1,0,2)); __m128 res = PS2_VMUL(fs_yzx, ft_zxy); ctx->vu0_acc = _mm_blendv_ps(ctx->vu0_acc, res, _mm_castsi128_ps(_mm_set_epi32(0, -1, -1, -1))); }
        // 0x1457d7c: 0x4bcc6c69  vmadd.xyz   $vf17, $vf13, $vf12
        { __m128 mul_res = PS2_VMUL(ctx->vu0_vf[13], ctx->vu0_vf[12]); __m128 res = PS2_VADD(ctx->vu0_acc, mul_res); __m128i mask = _mm_set_epi32(0, -1, -1, -1); ctx->vu0_vf[17] = _mm_blendv_ps(ctx->vu0_vf[17], res, _mm_castsi128_ps(mask)); }
        // 0x1457d80: 0x4bcc7afe  vopmula.xyz $ACC, $vf15, $vf12
        { __m128 fs_yzx = _mm_shuffle_ps(ctx->vu0_vf[15], ctx->vu0_vf[15], _MM_SHUFFLE(3,0,2,1)); __m128 ft_zxy = _mm_shuffle_ps(ctx->vu0_vf[12], ctx->vu0_vf[12], _MM_SHUFFLE(3,1,0,2)); __m128 res = PS2_VMUL(fs_yzx, ft_zxy); ctx->vu0_acc = _mm_blendv_ps(ctx->vu0_acc, res, _mm_castsi128_ps(_mm_set_epi32(0, -1, -1, -1))); }
        // 0x1457d84: 0x4bc003e9  vmadd.xyz   $vf15, $vf0, $vf0
        { __m128 mul_res = PS2_VMUL(ctx->vu0_vf[0], ctx->vu0_vf[0]); __m128 res = PS2_VADD(ctx->vu0_acc, mul_res); __m128i mask = _mm_set_epi32(0, -1, -1, -1); ctx->vu0_vf[15] = _mm_blendv_ps(ctx->vu0_vf[15], res, _mm_castsi128_ps(mask)); }
        // 0x1457d88: 0x4bd163ac  vsub.xyz    $vf14, $vf12, $vf17
        { __m128 res = PS2_VSUB(ctx->vu0_vf[12], ctx->vu0_vf[17]); __m128i mask = _mm_set_epi32(0, -1, -1, -1); ctx->vu0_vf[14] = PS2_VBLEND(ctx->vu0_vf[14], res, _mm_castsi128_ps(mask)); }
        // 0x1457d8c: 0x4ad1733c  vmove.yz    $vf17, $vf14
        { __m128i mask = _mm_set_epi32(0, -1, -1, 0); ctx->vu0_vf[17] = _mm_blendv_ps(ctx->vu0_vf[17], ctx->vu0_vf[14], _mm_castsi128_ps(mask)); }
        // 0x1457d90: 0x4a8e833c  vmove.y     $vf14, $vf16
        { __m128i mask = _mm_set_epi32(0, 0, -1, 0); ctx->vu0_vf[14] = _mm_blendv_ps(ctx->vu0_vf[14], ctx->vu0_vf[16], _mm_castsi128_ps(mask)); }
        // 0x1457d94: 0x4a907b3c  vmove.y     $vf16, $vf15
        { __m128i mask = _mm_set_epi32(0, 0, -1, 0); ctx->vu0_vf[16] = _mm_blendv_ps(ctx->vu0_vf[16], ctx->vu0_vf[15], _mm_castsi128_ps(mask)); }
        // 0x1457d98: 0x4a8f8b3c  vmove.y     $vf15, $vf17
        { __m128i mask = _mm_set_epi32(0, 0, -1, 0); ctx->vu0_vf[15] = _mm_blendv_ps(ctx->vu0_vf[15], ctx->vu0_vf[17], _mm_castsi128_ps(mask)); }
        // 0x1457d9c: 0x4a4e7b3c  vmove.z     $vf14, $vf15
        { __m128i mask = _mm_set_epi32(0, -1, 0, 0); ctx->vu0_vf[14] = _mm_blendv_ps(ctx->vu0_vf[14], ctx->vu0_vf[15], _mm_castsi128_ps(mask)); }
        // 0x1457da0: 0x4a4f833c  vmove.z     $vf15, $vf16
        { __m128i mask = _mm_set_epi32(0, -1, 0, 0); ctx->vu0_vf[15] = _mm_blendv_ps(ctx->vu0_vf[15], ctx->vu0_vf[16], _mm_castsi128_ps(mask)); }
        // 0x1457da4: 0x4a508b3c  vmove.z     $vf16, $vf17
        { __m128i mask = _mm_set_epi32(0, -1, 0, 0); ctx->vu0_vf[16] = _mm_blendv_ps(ctx->vu0_vf[16], ctx->vu0_vf[17], _mm_castsi128_ps(mask)); }
        // 0x1457da8: 0xf86e0000  sqc2        $vf14, 0x0($v1)
        WRITE128(ADD32(GPR_U32(ctx, 3), 0), _mm_castps_si128(ctx->vu0_vf[14]));
        // 0x1457dac: 0xf86f0010  sqc2        $vf15, 0x10($v1)
        WRITE128(ADD32(GPR_U32(ctx, 3), 16), _mm_castps_si128(ctx->vu0_vf[15]));
        // 0x1457db0: 0xf8700020  sqc2        $vf16, 0x20($v1)
        WRITE128(ADD32(GPR_U32(ctx, 3), 32), _mm_castps_si128(ctx->vu0_vf[16]));
        // 0x1457db4: 0xd8840000  lqc2        $vf4, 0x0($a0)
        ctx->vu0_vf[4] = _mm_castsi128_ps(READ128(ADD32(GPR_U32(ctx, 4), 0)));
        // 0x1457db8: 0x4a24033c  vmove.w     $vf4, $vf0
        { __m128i mask = _mm_set_epi32(-1, 0, 0, 0); ctx->vu0_vf[4] = _mm_blendv_ps(ctx->vu0_vf[4], ctx->vu0_vf[0], _mm_castsi128_ps(mask)); }
        // 0x1457dbc: 0xf8640030  sqc2        $vf4, 0x30($v1)
        WRITE128(ADD32(GPR_U32(ctx, 3), 48), _mm_castps_si128(ctx->vu0_vf[4]));
        // 0x1457dc0: 0xdbb80030  lqc2        $vf24, 0x30($sp)
        ctx->vu0_vf[24] = _mm_castsi128_ps(READ128(ADD32(GPR_U32(ctx, 29), 48)));
        // 0x1457dc4: 0x4b000647  vsubw.x     $vf25, $vf0, $vf0w
        { __m128 res = PS2_VSUB(ctx->vu0_vf[0], _mm_shuffle_ps(ctx->vu0_vf[0], ctx->vu0_vf[0], _MM_SHUFFLE(3,3,3,3))); __m128i mask = _mm_set_epi32(0, 0, 0, -1); ctx->vu0_vf[25] = _mm_blendv_ps(ctx->vu0_vf[25], res, _mm_castsi128_ps(mask)); }
        // 0x1457dc8: 0x4bd9c718  vmulx.xyz   $vf28, $vf24, $vf25x
        { __m128 res = PS2_VMUL(ctx->vu0_vf[24], _mm_shuffle_ps(ctx->vu0_vf[25], ctx->vu0_vf[25], _MM_SHUFFLE(0,0,0,0))); __m128i mask = _mm_set_epi32(0, -1, -1, -1); ctx->vu0_vf[28] = _mm_blendv_ps(ctx->vu0_vf[28], res, _mm_castsi128_ps(mask)); }
        // 0x1457dcc: 0x4a3ce72c  vsub.w      $vf28, $vf28, $vf28
        { __m128 res = PS2_VSUB(ctx->vu0_vf[28], ctx->vu0_vf[28]); __m128i mask = _mm_set_epi32(-1, 0, 0, 0); ctx->vu0_vf[28] = PS2_VBLEND(ctx->vu0_vf[28], res, _mm_castsi128_ps(mask)); }
        // 0x1457dd0: 0x7ba80000  lq          $t0, 0x0($sp)
        SET_GPR_VEC(ctx, 8, READ128(ADD32(GPR_U32(ctx, 29), 0)));
        // 0x1457dd4: 0x7ba90010  lq          $t1, 0x10($sp)
        SET_GPR_VEC(ctx, 9, READ128(ADD32(GPR_U32(ctx, 29), 16)));
        // 0x1457dd8: 0x7baa0020  lq          $t2, 0x20($sp)
        SET_GPR_VEC(ctx, 10, READ128(ADD32(GPR_U32(ctx, 29), 32)));
        // 0x1457ddc: 0x482b0000  qmfc2.ni    $t3, $vf0
        SET_GPR_VEC(ctx, 11, _mm_castps_si128(ctx->vu0_vf[0]));
        // 0x1457de0: 0x71286488  pextlw      $t4, $t1, $t0
        SET_GPR_VEC(ctx, 12, PS2_PEXTLW(GPR_VEC(ctx, 9), GPR_VEC(ctx, 8)));
        // 0x1457de4: 0x71286ca8  pextuw      $t5, $t1, $t0
        SET_GPR_VEC(ctx, 13, PS2_PEXTUW(GPR_VEC(ctx, 9), GPR_VEC(ctx, 8)));
        // 0x1457de8: 0x716a7488  pextlw      $t6, $t3, $t2
        SET_GPR_VEC(ctx, 14, PS2_PEXTLW(GPR_VEC(ctx, 11), GPR_VEC(ctx, 10)));
        // 0x1457dec: 0x716a7ca8  pextuw      $t7, $t3, $t2
        SET_GPR_VEC(ctx, 15, PS2_PEXTUW(GPR_VEC(ctx, 11), GPR_VEC(ctx, 10)));
        // 0x1457df0: 0x71cc4389  pcpyld      $t0, $t6, $t4
        SET_GPR_VEC(ctx, 8, PS2_PCPYLD(GPR_VEC(ctx, 14), GPR_VEC(ctx, 12)));
        // 0x1457df4: 0x718e4ba9  pcpyud      $t1, $t4, $t6
        SET_GPR_VEC(ctx, 9, _mm_unpackhi_epi64(GPR_VEC(ctx, 12), GPR_VEC(ctx, 14)));
        // 0x1457df8: 0x71ed5389  pcpyld      $t2, $t7, $t5
        SET_GPR_VEC(ctx, 10, PS2_PCPYLD(GPR_VEC(ctx, 15), GPR_VEC(ctx, 13)));
        // 0x1457dfc: 0x71af5ba9  pcpyud      $t3, $t5, $t7
        SET_GPR_VEC(ctx, 11, _mm_unpackhi_epi64(GPR_VEC(ctx, 13), GPR_VEC(ctx, 15)));
        // 0x1457e00: 0x48a8c000  qmtc2.ni    $t0, $vf24
        ctx->vu0_vf[24] = _mm_castsi128_ps(GPR_VEC(ctx, 8));
        // 0x1457e04: 0x48a9c800  qmtc2.ni    $t1, $vf25
        ctx->vu0_vf[25] = _mm_castsi128_ps(GPR_VEC(ctx, 9));
        // 0x1457e08: 0x48aad000  qmtc2.ni    $t2, $vf26
        ctx->vu0_vf[26] = _mm_castsi128_ps(GPR_VEC(ctx, 10));
        // 0x1457e0c: 0x4bfcc1bc  vmulax.xyzw $ACC, $vf24, $vf28x
        { __m128 res = PS2_VMUL(ctx->vu0_vf[24], _mm_shuffle_ps(ctx->vu0_vf[28], ctx->vu0_vf[28], _MM_SHUFFLE(0,0,0,0))); ctx->vu0_acc = _mm_blendv_ps(ctx->vu0_acc, res, _mm_castsi128_ps(_mm_set_epi32(-1, -1, -1, -1))); }
        // 0x1457e10: 0x4bfcc8bd  vmadday.xyzw $ACC, $vf25, $vf28y
        { __m128 mul_res = PS2_VMUL(ctx->vu0_vf[25], _mm_shuffle_ps(ctx->vu0_vf[28], ctx->vu0_vf[28], _MM_SHUFFLE(1,1,1,1))); __m128 res = PS2_VADD(ctx->vu0_acc, mul_res); ctx->vu0_acc = _mm_blendv_ps(ctx->vu0_acc, res, _mm_castsi128_ps(_mm_set_epi32(-1, -1, -1, -1))); }
        // 0x1457e14: 0x4bfcd0be  vmaddaz.xyzw $ACC, $vf26, $vf28z
        { __m128 mul_res = PS2_VMUL(ctx->vu0_vf[26], _mm_shuffle_ps(ctx->vu0_vf[28], ctx->vu0_vf[28], _MM_SHUFFLE(2,2,2,2))); __m128 res = PS2_VADD(ctx->vu0_acc, mul_res); ctx->vu0_acc = _mm_blendv_ps(ctx->vu0_acc, res, _mm_castsi128_ps(_mm_set_epi32(-1, -1, -1, -1))); }
        // 0x1457e18: 0x4bfc074b  vmaddw.xyzw $vf29, $vf0, $vf28w
        { __m128 mul_res = PS2_VMUL(ctx->vu0_vf[0], _mm_shuffle_ps(ctx->vu0_vf[28], ctx->vu0_vf[28], _MM_SHUFFLE(3,3,3,3))); __m128 res = PS2_VADD(ctx->vu0_acc, mul_res); __m128i mask = _mm_set_epi32(-1, -1, -1, -1); ctx->vu0_vf[29] = _mm_blendv_ps(ctx->vu0_vf[29], res, _mm_castsi128_ps(mask)); }
        // 0x1457e1c: 0x4a3def6c  vsub.w      $vf29, $vf29, $vf29
        { __m128 res = PS2_VSUB(ctx->vu0_vf[29], ctx->vu0_vf[29]); __m128i mask = _mm_set_epi32(-1, 0, 0, 0); ctx->vu0_vf[29] = PS2_VBLEND(ctx->vu0_vf[29], res, _mm_castsi128_ps(mask)); }
        // 0x1457e20: 0x4a20ef68  vadd.w      $vf29, $vf29, $vf0
        { __m128 res = PS2_VADD(ctx->vu0_vf[29], ctx->vu0_vf[0]); __m128i mask = _mm_set_epi32(-1, 0, 0, 0); ctx->vu0_vf[29] = PS2_VBLEND(ctx->vu0_vf[29], res, _mm_castsi128_ps(mask)); }
        // 0x1457e24: 0xfbb80000  sqc2        $vf24, 0x0($sp)
        WRITE128(ADD32(GPR_U32(ctx, 29), 0), _mm_castps_si128(ctx->vu0_vf[24]));
        // 0x1457e28: 0xfbb90010  sqc2        $vf25, 0x10($sp)
        WRITE128(ADD32(GPR_U32(ctx, 29), 16), _mm_castps_si128(ctx->vu0_vf[25]));
        // 0x1457e2c: 0xfbba0020  sqc2        $vf26, 0x20($sp)
        WRITE128(ADD32(GPR_U32(ctx, 29), 32), _mm_castps_si128(ctx->vu0_vf[26]));
        // 0x1457e30: 0xfbbd0030  sqc2        $vf29, 0x30($sp)
        WRITE128(ADD32(GPR_U32(ctx, 29), 48), _mm_castps_si128(ctx->vu0_vf[29]));
        // 0x1457e34: 0x8e44407c  lw          $a0, 0x407C($s2)
        SET_GPR_S32(ctx, 4, (int32_t)READ32(ADD32(GPR_U32(ctx, 18), 16508)));
        // 0x1457e38: 0x27a20040  addiu       $v0, $sp, 0x40
        SET_GPR_S32(ctx, 2, (int32_t)ADD32(GPR_U32(ctx, 29), 64));
        // 0x1457e3c: 0x8c830134  lw          $v1, 0x134($a0)
        SET_GPR_S32(ctx, 3, (int32_t)READ32(ADD32(GPR_U32(ctx, 4), 308)));
        // 0x1457e40: 0xd8680000  lqc2        $vf8, 0x0($v1)
        ctx->vu0_vf[8] = _mm_castsi128_ps(READ128(ADD32(GPR_U32(ctx, 3), 0)));
        // 0x1457e44: 0xdbae0000  lqc2        $vf14, 0x0($sp)
        ctx->vu0_vf[14] = _mm_castsi128_ps(READ128(ADD32(GPR_U32(ctx, 29), 0)));
        // 0x1457e48: 0xdbaf0010  lqc2        $vf15, 0x10($sp)
        ctx->vu0_vf[15] = _mm_castsi128_ps(READ128(ADD32(GPR_U32(ctx, 29), 16)));
        // 0x1457e4c: 0xdbb00020  lqc2        $vf16, 0x20($sp)
        ctx->vu0_vf[16] = _mm_castsi128_ps(READ128(ADD32(GPR_U32(ctx, 29), 32)));
        // 0x1457e50: 0xdbb10030  lqc2        $vf17, 0x30($sp)
        ctx->vu0_vf[17] = _mm_castsi128_ps(READ128(ADD32(GPR_U32(ctx, 29), 48)));
        // 0x1457e54: 0x4be871bc  vmulax.xyzw $ACC, $vf14, $vf8x
        { __m128 res = PS2_VMUL(ctx->vu0_vf[14], _mm_shuffle_ps(ctx->vu0_vf[8], ctx->vu0_vf[8], _MM_SHUFFLE(0,0,0,0))); ctx->vu0_acc = _mm_blendv_ps(ctx->vu0_acc, res, _mm_castsi128_ps(_mm_set_epi32(-1, -1, -1, -1))); }
        // 0x1457e58: 0x4be878bd  vmadday.xyzw $ACC, $vf15, $vf8y
        { __m128 mul_res = PS2_VMUL(ctx->vu0_vf[15], _mm_shuffle_ps(ctx->vu0_vf[8], ctx->vu0_vf[8], _MM_SHUFFLE(1,1,1,1))); __m128 res = PS2_VADD(ctx->vu0_acc, mul_res); ctx->vu0_acc = _mm_blendv_ps(ctx->vu0_acc, res, _mm_castsi128_ps(_mm_set_epi32(-1, -1, -1, -1))); }
        // 0x1457e5c: 0x4be880be  vmaddaz.xyzw $ACC, $vf16, $vf8z
        { __m128 mul_res = PS2_VMUL(ctx->vu0_vf[16], _mm_shuffle_ps(ctx->vu0_vf[8], ctx->vu0_vf[8], _MM_SHUFFLE(2,2,2,2))); __m128 res = PS2_VADD(ctx->vu0_acc, mul_res); ctx->vu0_acc = _mm_blendv_ps(ctx->vu0_acc, res, _mm_castsi128_ps(_mm_set_epi32(-1, -1, -1, -1))); }
        // 0x1457e60: 0x4be88a8b  vmaddw.xyzw $vf10, $vf17, $vf8w
        { __m128 mul_res = PS2_VMUL(ctx->vu0_vf[17], _mm_shuffle_ps(ctx->vu0_vf[8], ctx->vu0_vf[8], _MM_SHUFFLE(3,3,3,3))); __m128 res = PS2_VADD(ctx->vu0_acc, mul_res); __m128i mask = _mm_set_epi32(-1, -1, -1, -1); ctx->vu0_vf[10] = _mm_blendv_ps(ctx->vu0_vf[10], res, _mm_castsi128_ps(mask)); }
        // 0x1457e64: 0xf84a0000  sqc2        $vf10, 0x0($v0)
        WRITE128(ADD32(GPR_U32(ctx, 2), 0), _mm_castps_si128(ctx->vu0_vf[10]));
        // 0x1457e68: 0xafa00044  sw          $zero, 0x44($sp)
        WRITE32(ADD32(GPR_U32(ctx, 29), 68), GPR_U32(ctx, 0));
        // 0x1457e6c: 0xd8450000  lqc2        $vf5, 0x0($v0)
        ctx->vu0_vf[5] = _mm_castsi128_ps(READ128(ADD32(GPR_U32(ctx, 2), 0)));
        // 0x1457e70: 0x4bc529ea  vmul.xyz    $vf7, $vf5, $vf5
        { __m128 res = PS2_VMUL(ctx->vu0_vf[5], ctx->vu0_vf[5]); __m128i mask = _mm_set_epi32(0, -1, -1, -1); ctx->vu0_vf[7] = PS2_VBLEND(ctx->vu0_vf[7], res, _mm_castsi128_ps(mask)); }
        // 0x1457e74: 0x4a2701bc  vmulax.w    $ACC, $vf0, $vf7x
        { __m128 res = PS2_VMUL(ctx->vu0_vf[0], _mm_shuffle_ps(ctx->vu0_vf[7], ctx->vu0_vf[7], _MM_SHUFFLE(0,0,0,0))); ctx->vu0_acc = _mm_blendv_ps(ctx->vu0_acc, res, _mm_castsi128_ps(_mm_set_epi32(-1, 0, 0, 0))); }
        // 0x1457e78: 0x4a2700bd  vmadday.w   $ACC, $vf0, $vf7y
        { __m128 mul_res = PS2_VMUL(ctx->vu0_vf[0], _mm_shuffle_ps(ctx->vu0_vf[7], ctx->vu0_vf[7], _MM_SHUFFLE(1,1,1,1))); __m128 res = PS2_VADD(ctx->vu0_acc, mul_res); ctx->vu0_acc = _mm_blendv_ps(ctx->vu0_acc, res, _mm_castsi128_ps(_mm_set_epi32(-1, 0, 0, 0))); }
        // 0x1457e7c: 0x4a2701ca  vmaddz.w    $vf7, $vf0, $vf7z
        { __m128 mul_res = PS2_VMUL(ctx->vu0_vf[0], _mm_shuffle_ps(ctx->vu0_vf[7], ctx->vu0_vf[7], _MM_SHUFFLE(2,2,2,2))); __m128 res = PS2_VADD(ctx->vu0_acc, mul_res); __m128i mask = _mm_set_epi32(-1, 0, 0, 0); ctx->vu0_vf[7] = _mm_blendv_ps(ctx->vu0_vf[7], res, _mm_castsi128_ps(mask)); }
        // 0x1457e80: 0x4be703be  vrsqrt      $Q, $vf0w, $vf7w
        { float fs = _mm_cvtss_f32(_mm_shuffle_ps(ctx->vu0_vf[0], ctx->vu0_vf[0], _MM_SHUFFLE(0,0,0,3))); float ft = _mm_cvtss_f32(_mm_shuffle_ps(ctx->vu0_vf[7], ctx->vu0_vf[7], _MM_SHUFFLE(0,0,0,3))); ctx->vu0_q = ps2VuRsqrt(ctx->vu0_status, fs, ft); }
        // 0x1457e84: 0x4a0003bf  vwaitq
        // VWAITQ (Q already resolved in this runtime)
        // 0x1457e88: 0x4bc0295c  vmulq.xyz   $vf5, $vf5, $Q
        { __m128 res = PS2_VMUL(ctx->vu0_vf[5], _mm_set1_ps(ctx->vu0_q)); __m128i mask = _mm_set_epi32(0, -1, -1, -1); ctx->vu0_vf[5] = _mm_blendv_ps(ctx->vu0_vf[5], res, _mm_castsi128_ps(mask)); }
        // 0x1457e8c: 0xf8450000  sqc2        $vf5, 0x0($v0)
        WRITE128(ADD32(GPR_U32(ctx, 2), 0), _mm_castps_si128(ctx->vu0_vf[5]));
        V2_LWC1(12, sp + 0x40u);
        V2_LWC1(13, sp + 0x48u);
        callGuest(rdram, ctx, runtime, jalTarget(rdram, 0x01457E94u), 0x01457E9Cu, sp); // iosGetATan2fExact
        ctx->f[1] = FPU_MOV_S(ctx->f[0]);
        SET_GPR_S32(ctx, rAT, (int32_t)((uint32_t)15502 << 16));
        SET_GPR_U64(ctx, rAT, GPR_U64(ctx, rAT) | (uint64_t)(uint16_t)64053);
        {
            uint32_t bits = GPR_U32(ctx, rAT);
            std::memcpy(&ctx->f[0], &bits, sizeof(bits)); // pi / 180
        }
        ctx->f[20] = FPU_MUL_S(ctx->f[20], ctx->f[0]);
        ctx->fcr31 = (FPU_C_OLE_S(ctx->f[20], ctx->f[1])) ? (ctx->fcr31 | 0x800000) : (ctx->fcr31 & ~0x800000);
        {
            const bool below = !(ctx->fcr31 & 0x800000);
            V2_ZERO(rV0);
            if (!below)
            {
                ctx->f[0] = FPU_MUL_S(ctx->f[21], ctx->f[0]);
                ctx->fcr31 = (FPU_C_OLE_S(ctx->f[1], ctx->f[0])) ? (ctx->fcr31 | 0x800000) : (ctx->fcr31 & ~0x800000);
                if (ctx->fcr31 & 0x800000)
                    V2_LI(rV0, 1);
            }
        }
        V2_LD(rS0, sp + 0x50u);
        V2_LD(rS1, sp + 0x58u);
        V2_LD(rS2, sp + 0x60u);
        V2_LD(rRA, sp + 0x68u);
        V2_LWC1(21, sp + 0x78u);
        V2_LWC1(20, sp + 0x70u);
        V2_ADDIU(rSP, rSP, 0x80);
        ret(ctx);
    }

    // ---- SCRFuncStartEventDemoGameModeCheck ----------------------------------------
    // a0 the object, a1 a game flag, a2 / a3 / f12-f15 the demo's arguments,
    // t1 / t2 handles, t3 a condition list.
    void scrFuncStartEventDemoGameModeCheck(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
    {
        if (!sotc_decomp::scriptVmOn())
            return fallback(kEventDemo, rdram, ctx, runtime);
        V2_ADDIU(rSP, rSP, -0x80);
        const uint32_t sp = V2_U(rSP);
        V2_SD(sp + 0x20u, rS0);
        V2_MOVE(rS0, rT1);
        V2_SD(sp + 0x28u, rS1);
        V2_MOVE(rS1, rA1);
        V2_SD(sp + 0x30u, rS2);
        V2_MOVE(rS2, rT2);
        V2_SD(sp + 0x38u, rS3);
        V2_MOVE(rS3, rT3);
        V2_SD(sp + 0x40u, rS4);
        V2_MOVE(rS4, rA0);
        V2_SD(sp + 0x48u, rS5);
        V2_MOVE(rS5, rA3);
        V2_SD(sp + 0x50u, rS6);
        V2_MOVE(rS6, rA2);
        V2_SWC1(sp + 0x78u, 23);
        ctx->f[23] = FPU_MOV_S(ctx->f[12]);
        V2_SWC1(sp + 0x70u, 22);
        ctx->f[22] = FPU_MOV_S(ctx->f[15]);
        V2_SWC1(sp + 0x68u, 21);
        ctx->f[21] = FPU_MOV_S(ctx->f[14]);
        V2_SWC1(sp + 0x60u, 20);
        V2_SD(sp + 0x58u, rRA);
        ctx->f[20] = FPU_MOV_S(ctx->f[13]);
        callGuest(rdram, ctx, runtime, kGameClearCnt, 0x014514CCu, sp);
        if (GPR_S32(ctx, rV0) > 0)
            goto skip;
        callGuest(rdram, ctx, runtime, kGameHardMode, 0x014514DCu, sp);
        if (V2_Q(rV0) != 0u)
            goto skip;
        V2_LI(rV0, 0x01460000);
        V2_LW(rV1, V2_U(rV0) + 0x5D38u);
        if (V2_Q(rV1) != 0u)
            goto skip;
        if (V2_Q(rS1) != 0u)
        {
            V2_MOVE(rA0, rS1);
            callGuest(rdram, ctx, runtime, kGameFlagChk, 0x01451504u, sp);
            if (V2_Q(rV0) != 0u)
                goto skip;
        }
        V2_MOVE(rA0, rS4);
        V2_MOVE(rA1, rS3);
        callGuest(rdram, ctx, runtime, kExecCondition, 0x01451518u, sp);
        {
            const bool fail = V2_Q(rV0) == 0u;
            V2_LI(rV0, 0x30000);
            if (fail)
                goto done;
        }
        // t1's handle: its owner's +0x10 (a2).
        V2_LW(rV1, V2_U(rS0));
        V2_ORI(rV0, rV0, 0xFFFF);
        V2_AND(rV0, rV1, rV0);
        {
            const bool empty = V2_Q(rV0) == 0u;
            V2_SUBU(rV0, rS0, rV0);
            if (empty)
                V2_ZERO(rA1);
            else
                V2_LW(rA1, V2_U(rV0));
        }
        V2_LUI_AT(rV0, 0x01451548u);
        if (V2_Q(rA1) == 0u)
            V2_ZERO(rA2);
        else
        {
            V2_LW_AT(rV1, rV0, 0x0145154Cu);
            if (V2_Q(rA1) == V2_Q(rV1))
                V2_ZERO(rA2);
            else
                V2_LW(rA2, V2_U(rA1) + 0x10u);
        }
        // t2's handle: its element (a3).
        V2_LW(rA0, V2_U(rS2));
        V2_LI(rV0, 0x30000);
        V2_ORI(rV0, rV0, 0xFFFF);
        V2_AND(rV0, rA0, rV0);
        {
            const bool empty = V2_Q(rV0) == 0u;
            V2_SUBU(rV0, rS2, rV0);
            if (empty)
                V2_ZERO(rT2);
            else
                V2_LW(rT2, V2_U(rV0));
        }
        V2_LUI_AT(rV0, 0x01451590u);
        if (V2_Q(rT2) == 0u)
            goto noElement;
        V2_LW_AT(rV1, rV0, 0x01451594u);
        V2_SRL(rV0, rA0, 18);
        if (V2_Q(rT2) == V2_Q(rV1))
            goto noElement;
        V2_LW(rA1, V2_U(rT2) + 8u);
        V2_ANDI(rV0, rV0, 0xFFF);
        V2_LW(rA0, V2_U(rT2) + 0x10u);
        V2_LW(rV1, V2_U(rA1) + 0xCu);
        mult(ctx, rV0, rV0, rV1);
        V2_ADDU(rA3, rA0, rV0);
        goto create;
    noElement:
        V2_ZERO(rA3);
    create: // 0x14515c4: createScrEventDemoManager's arguments
        {
            int32_t tmp = FPU_CVT_W_S(ctx->f[20]);
            std::memcpy(&ctx->f[0], &tmp, sizeof(tmp));
        }
        {
            uint32_t bits;
            std::memcpy(&bits, &ctx->f[0], sizeof(bits));
            SET_GPR_U32(ctx, rT1, bits);
        }
        SET_GPR_U64(ctx, rV0, GPR_U64(ctx, rS5) << 32);
        SET_GPR_S64(ctx, rV0, GPR_S64(ctx, rV0) >> 32);
        {
            int32_t tmp = FPU_CVT_W_S(ctx->f[21]);
            std::memcpy(&ctx->f[0], &tmp, sizeof(tmp));
        }
        {
            uint32_t bits;
            std::memcpy(&bits, &ctx->f[0], sizeof(bits));
            SET_GPR_U32(ctx, rT3, bits);
        }
        V2_MOVE(rA1, rA2);
        V2_MOVE(rA0, rS4);
        V2_MOVE(rT0, rS1);
        V2_MOVE(rA2, rS6);
        ctx->f[12] = FPU_MOV_S(ctx->f[23]);
        V2_MOVE(rT2, rS3);
        {
            int32_t tmp = FPU_CVT_W_S(ctx->f[22]);
            std::memcpy(&ctx->f[0], &tmp, sizeof(tmp));
        }
        V2_SWC1(sp + 8u, 0);
        WRITE32(sp, V2_U(rV0));
        callGuest(rdram, ctx, runtime, kCreateEventDemo, 0x01451604u, sp);
    done:
        V2_LD(rS0, sp + 0x20u);
        goto leave;
    skip: // the branch-likely's slot: ld $s0
        V2_LD(rS0, sp + 0x20u);
    leave:
        V2_LD(rS1, sp + 0x28u);
        V2_LD(rS2, sp + 0x30u);
        V2_LD(rS3, sp + 0x38u);
        V2_LD(rS4, sp + 0x40u);
        V2_LD(rS5, sp + 0x48u);
        V2_LD(rS6, sp + 0x50u);
        V2_LD(rRA, sp + 0x58u);
        V2_LWC1(23, sp + 0x78u);
        V2_LWC1(22, sp + 0x70u);
        V2_LWC1(21, sp + 0x68u);
        V2_LWC1(20, sp + 0x60u);
        V2_ADDIU(rSP, rSP, 0x80);
        ret(ctx);
    }

    // ---- SCRFuncVolumeCheckByGameObjForSelfPos -------------------------------------
    // a0 the object, t3 the volume's handle, the caller's sp + 0 word its
    // element handle.
    void scrFuncVolumeCheckByGameObjForSelfPos(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
    {
        if (!sotc_decomp::scriptVmOn())
            return fallback(kVolumeCheck, rdram, ctx, runtime);
        V2_ADDIU(rSP, rSP, -0x90);
        const uint32_t sp = V2_U(rSP);
        V2_LI(rV0, 0x30000);
        V2_SD(sp + 0x78u, rS1);
        V2_ORI(rV0, rV0, 0xFFFF);
        V2_SD(sp + 0x70u, rS0);
        V2_SD(sp + 0x80u, rRA);
        V2_LW(rV1, V2_U(rT3));
        V2_AND(rV0, rV1, rV0);
        {
            const bool empty = V2_Q(rV0) == 0u;
            V2_LW(rS1, sp + 0x90u);
            if (empty)
                V2_ZERO(rT3);
            else
            {
                V2_SUBU(rV0, rT3, rV0);
                V2_LW(rT3, V2_U(rV0));
            }
        }
        V2_LUI_AT(rV0, 0x01458318u);
        if (V2_Q(rT3) == 0u)
            V2_ZERO(rS0);
        else
        {
            V2_LW_AT(rV1, rV0, 0x0145831Cu);
            if (V2_Q(rT3) == V2_Q(rV1))
                V2_ZERO(rS0);
            else
                V2_LW(rS0, V2_U(rT3) + 0x10u);
        }
        callGuest(rdram, ctx, runtime, jalTarget(rdram, 0x01458334u), 0x0145833Cu, sp); // GetLayoutClass(a0)
        V2_LW(rV1, V2_U(rV0) + 0x134u);
        SET_GPR_VEC(ctx, rV0, READ128(ADD32(V2_U(rV1), 0)));
        {
            const bool noVolume = V2_Q(rS0) == 0u;
            WRITE128(ADD32(sp, 96), GPR_VEC(ctx, rV0)); // the position at sp + 0x60
            if (noVolume)
                goto no;
        }
        V2_LW(rT3, V2_U(rS0) + 8u);
        {
            const bool noVolume = V2_Q(rT3) == 0u;
            V2_LI(rV0, 0x30000);
            if (noVolume)
                goto no;
        }
        V2_LW(rA2, V2_U(rS1));
        V2_ORI(rV0, rV0, 0xFFFF);
        V2_AND(rV0, rA2, rV0);
        {
            const bool empty = V2_Q(rV0) == 0u;
            V2_SUBU(rV0, rS1, rV0);
            if (empty)
                V2_ZERO(rA0);
            else
                V2_LW(rA0, V2_U(rV0));
        }
        V2_LUI_AT(rV0, 0x01458380u);
        if (V2_Q(rA0) == 0u)
            goto noElement;
        V2_LW_AT(rV1, rV0, 0x01458384u);
        V2_SRL(rV0, rA2, 18);
        if (V2_Q(rA0) == V2_Q(rV1))
            goto noElement;
        V2_LW(rA1, V2_U(rA0) + 8u);
        V2_ANDI(rV0, rV0, 0xFFF);
        V2_LW(rA0, V2_U(rA0) + 0x10u);
        V2_LW(rV1, V2_U(rA1) + 0xCu);
        mult(ctx, rV0, rV0, rV1);
        V2_ADDU(rV0, rA0, rV0);
        goto have;
    noElement:
        V2_ZERO(rV0);
    have:
        {
            const bool none = V2_Q(rV0) == 0u;
            V2_MOVE(rA1, rT3);
            if (none)
                goto no;
        }
        V2_MOVE(rA2, rS1);
        V2_MOVE(rA0, rSP);
        V2_ADDIU(rA3, rSP, 0x60);
        callGuest(rdram, ctx, runtime, jalTarget(rdram, 0x014583C4u), 0x014583CCu, sp); // CheckPositionInsideOfJudgeVolumeObject
        {
            const bool inside = V2_Q(rV0) != 0u;
            V2_LI(rV0, 1);
            if (inside)
                goto leave;
        }
    no:
        V2_ZERO(rV0);
    leave:
        V2_LD(rS0, sp + 0x70u);
        V2_LD(rS1, sp + 0x78u);
        V2_LD(rRA, sp + 0x80u);
        V2_ADDIU(rSP, rSP, 0x90);
        ret(ctx);
    }

    // ---- SCRCharaTurnDirectInput: character class +0x7e8 = f12 -----------------------
    void scrCharaTurnDirectInput(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
    {
        if (!sotc_decomp::scriptVmOn())
            return fallback(kTurnDirect, rdram, ctx, runtime);
        V2_ADDIU(rSP, rSP, -0x10);
        const uint32_t sp = V2_U(rSP);
        V2_SD(sp, rRA);
        V2_SWC1(sp + 8u, 20);
        ctx->f[20] = FPU_MOV_S(ctx->f[12]);
        callGuest(rdram, ctx, runtime, jalTarget(rdram, 0x0137C7A4u), 0x0137C7ACu, sp); // GetCharacterClass
        V2_SWC1(V2_U(rV0) + 0x7E8u, 20);
        V2_LD(rRA, sp);
        V2_LWC1(20, sp + 8u);
        V2_ADDIU(rSP, rSP, 0x10);
        ret(ctx);
    }

    // ---- SCRGetGetGeometryPointerLegalParentObj ----------------------------------
    void scrGetGetGeometryPointerLegalParentObj(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
    {
        if (!sotc_decomp::scriptVmOn())
            return fallback(kLegalParent, rdram, ctx, runtime);
        V2_ADDIU(rSP, rSP, -0x10);
        const uint32_t sp = V2_U(rSP);
        V2_SD(sp, rS0);
        V2_SD(sp + 8u, rRA);
        callGuest(rdram, ctx, runtime, jalTarget(rdram, 0x0137D64Cu), 0x0137D654u, sp); // GetGeometryPointerObject
        V2_MOVE(rS0, rV0);
        V2_ADDIU(rV1, 0, 0xA);
        if (V2_Q(rS0) == 0u)
            goto result;
        SET_GPR_ZE32(ctx, rV0, (uint8_t)READ8(ADD32(V2_U(rS0), 8)));
        if (V2_Q(rV0) != V2_Q(rV1))
        {
            V2_MOVE(rV0, rS0);
            goto leave;
        }
        V2_MOVE(rA0, rS0);
        callGuest(rdram, ctx, runtime, jalTarget(rdram, 0x0137D66Cu), 0x0137D674u, sp); // GetLayoutClass
        V2_LW(rV1, V2_U(rV0) + 0x124u);
        {
            const bool none = V2_Q(rV1) == 0u;
            V2_ADDIU(rA0, rV0, 0xC0);
            if (none)
                goto result;
        }
        callGuest(rdram, ctx, runtime, jalTarget(rdram, 0x0137D680u), 0x0137D688u, sp); // GetGeometryPointerObject
        V2_MOVE(rA0, rV0);
        V2_ADDIU(rV0, 0, 0xD);
        SET_GPR_ZE32(ctx, rV1, (uint8_t)READ8(ADD32(V2_U(rA0), 8)));
        {
            const bool is13 = V2_Q(rV1) == V2_Q(rV0);
            V2_ADDIU(rA1, 0, 0x16);
            if (!is13)
            {
                const bool not22 = V2_Q(rV1) != V2_Q(rA1);
                V2_MOVE(rV0, rS0);
                if (not22)
                    goto leave;
            }
        }
        V2_MOVE(rS0, rA0);
    result:
        V2_MOVE(rV0, rS0);
    leave:
        V2_LD(rS0, sp);
        V2_LD(rRA, sp + 8u);
        V2_ADDIU(rSP, rSP, 0x10);
        ret(ctx);
    }

#undef V2_U
#undef V2_Q
#undef V2_LW
#undef V2_MOVE
#undef V2_ZERO
#undef V2_LI
#undef V2_SD
#undef V2_LD
#undef V2_ADDIU
#undef V2_SUBU
#undef V2_ADDU
#undef V2_AND
#undef V2_ORI
#undef V2_ANDI
#undef V2_SRL
#undef V2_LUI_AT
#undef V2_LW_AT
#undef V2_SWC1
#undef V2_LWC1
}

SOTC_DECOMP_COROUTINE(0x01455FA8u, execSCRBaseFunc, decomp_scriptvm2::execSCRBaseFunc);
SOTC_DECOMP_RESUME(0x01456078u, execSCRBaseFunc, r078, decomp_scriptvm2::execSCRBaseFunc);
SOTC_DECOMP_COROUTINE(0x01450BB8u, SCRFuncSubFunc, decomp_scriptvm2::scrFuncSubFunc);
SOTC_DECOMP_COROUTINE(0x01450B60u, SCRFuncConditionExecFunc, decomp_scriptvm2::scrFuncConditionExecFunc);
SOTC_DECOMP_FUNCTION(0x01455F80u, getSCRDataCtrlWork, decomp_scriptvm2::getSCRDataCtrlWork);
SOTC_DECOMP_FUNCTION(0x013CFAE8u, GetSCRCharaAIWorkPtr, decomp_scriptvm2::getSCRCharaAIWorkPtr);
SOTC_DECOMP_FUNCTION(0x013CFB08u, CheckSCRCharaAIWorkPtr, decomp_scriptvm2::checkSCRCharaAIWorkPtr);
SOTC_DECOMP_FUNCTION(0x0137C348u, SCRGetFirstObj, decomp_scriptvm2::scrGetFirstObj);
SOTC_DECOMP_FUNCTION(0x01457648u, SCRConditionPlayerLastMotionShiftCheck, decomp_scriptvm2::scrConditionPlayerLastMotionShiftCheck);
SOTC_DECOMP_FUNCTION(0x01457708u, SCRConditionPlayerControlModeCheck, decomp_scriptvm2::scrConditionPlayerControlModeCheck);
SOTC_DECOMP_FUNCTION(0x01456A38u, SCRConditionPlayerParentColAttr, decomp_scriptvm2::scrConditionPlayerParentColAttr);
SOTC_DECOMP_FUNCTION(0x01464ED0u, SCRSoundConditionPlayerParentColAttr, decomp_scriptvm2::scrSoundConditionPlayerParentColAttr);
SOTC_DECOMP_FUNCTION(0x01457CF8u, SCRConditionRootAnglePlayerWorldXZ, decomp_scriptvm2::scrConditionRootAnglePlayerWorldXZ);
SOTC_DECOMP_FUNCTION(0x01451468u, SCRFuncStartEventDemoGameModeCheck, decomp_scriptvm2::scrFuncStartEventDemoGameModeCheck);
SOTC_DECOMP_FUNCTION(0x014582D8u, SCRFuncVolumeCheckByGameObjForSelfPos, decomp_scriptvm2::scrFuncVolumeCheckByGameObjForSelfPos);
SOTC_DECOMP_FUNCTION(0x0137C798u, SCRCharaTurnDirectInput, decomp_scriptvm2::scrCharaTurnDirectInput);
SOTC_DECOMP_FUNCTION(0x0137D640u, SCRGetGetGeometryPointerLegalParentObj, decomp_scriptvm2::scrGetGetGeometryPointerLegalParentObj);
