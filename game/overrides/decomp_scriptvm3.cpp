// The script objects' per-frame leaves (round 48, SOTC_SCRIPTVM=0 puts every
// one back to the recompiled code; each table entry alone with
// SOTC_DECOMP_SKIP=<name>), transcribed from the generator's code
// (build/recomp/game/output, the same statements in order). None of them can
// reach the sub-thread sleep (sleepers.py: False; CheckLetter's callback is
// CheckDamageLetter at every static call site), so every call is a plain
// callGuest / callNative.
//
// The frame end's per-type helpers (gamecore_sub_013CF258, decomp_scriptvm.cpp,
// calls them on the sub-thread's object; a0 is ignored: each asks
// isysGetThisSubThread for the running sub-thread's object). Each runs its
// callees on the script system's work stack: the word at 0x11f7140 gets the
// helper's frame sp, sp becomes 0x11f7140 (the stack grows down from it, so
// the sub-thread's small stack is not used by the heavy callees), and at the
// end sp is read back from that word and the frame's saves are reloaded.
// - gamecore_sub_013CFCA0 (0x13cfca0): an LWS orient object. The view
//   interval clip (ViewIntervalClip(obj, the main camera, globals +0x188))
//   gives a rate f20; ExecScriptLwsOrientClass(obj, the camera, globals
//   +0x370) with f12 = f20, f13 = 1 / f20. Frame 0x30: s0, s1, s2, ra, f20.
// - gamecore_sub_013CFE40 (0x13cfe40): an LWS demo orient manager:
//   ExecLwsDemoOrientManagerObj(obj, f12 = globals +0x178, f13 = +0x180).
//   Frame 0x10: s0, ra.
// - gamecore_sub_013CFBE0 (0x13cfbe0): a character's geometry (its +0xcec
//   byte set): ExecCharaGeometry(obj, 0x14659c0), then the character class's
//   +0x539 byte = StageManagerCheckAreaExist(the layout's +0x134 area).
//   Frame 0x20: s0, s1, s2, ra.
// - gamecore_sub_013CFD80 (0x13cfd80): an object animation:
//   ExecObjectAnimation(obj, the camera, f12 = globals +0x190, f13 = +0x194);
//   its result goes to 0x11e7130 and, when non-zero, the object is deleted
//   (isysRequestDeleteObj). Frame 0x20: s0, s1, ra.
// - gamecore_sub_013CF4E0 (0x13cf4e0): the character type's tail (the frame
//   end's tail jump). The view interval clip gives f20; when the class's
//   +0x534 byte is set and the character is not in a skip case, its
//   geometry pre-process: ExecCharaGeometryPreprocess(obj, f12 = globals
//   +0x178, f13 = f20, f14 = 1 / f20). Run when the class's +0x1170 word is
//   0, or its area exists, or +0x370 is 4, or the layout's geometry parent
//   (SCRGetGetGeometryPointerLegalParentObj) is of type 9 / 13 / 22, or
//   +0xd90 is 1, or +0x1070 is 1, or +0x370 is 11. Frame 0x30: s0-s3, ra, f20.
//
// The script class accessors:
// - GetScriptClass (0x1102438): an object's script class by its type (+8):
//   types 6..33 through the jump table at 0x1194cc0 (tail jumps to the
//   type's accessor, or a0 + 0x20 / a0 + 0x1c, or 0); other types 0.
// - GetScriptCharacterClass (0x110cb28): a0 + 0x20 when the type is 9, else 0.
// - GetScriptLayoutClass (0x110d058): a0 + 0x20 for type 11, a tail jump to
//   GetScriptAnimationClass for type 13, else 0.
// - ExecScriptLayoutObj (0x110d0f8): a layout object's frame:
//   ResetDamageClass(GetDamageClass(obj)), then a tail jump to
//   CheckLetter(obj, CheckDamageLetter).
// - CheckLetter (0x1166ba0): the object's letters (a0 +0x1c the post):
//   open_letter_post, each letter's contents to the callback a1(obj,
//   contents), then a tail jump to the post's close (0x1c8938).
//
// The script control status (a character's +0x920 64-bit mask; a0 the
// object: its character class, or for a layout object whose geometry
// pointer object is a character (type 22) that one's class):
// - CharaCtrlResetScriptCtrlStatus (0x10cc210): clears bit a1.
// - CharaCtrlSetScriptCtrlStatus (0x10cc178): sets bit a1.
// - CharaCtrlResetAllScriptCtrlStatus (0x10cc100): clears the mask.
// - checkScriptControlDef (0x10cc730): a control definition's script
//   condition (a1: +4 the bit, +8 the mode: 1 "set", 2 "clear"; a0 the mask,
//   a2 the default): v0 = (bit set in a0) ? 1 : a2 for mode 1 / other modes,
//   (bit set) ? 0 : a2 for mode 2; a1 0: a2.
// - isysGetThisSubThread (0x1ca1b0): the running sub-thread's object
//   (*(0x1e1550)).
//
// Exact: every frame save and store as the code makes them, the s-registers
// set as the code sets them before each call (round 47's rule) and reloaded
// from the frame as the epilogues do, v0 / v1 / a0 / a1 / at and the FPU
// registers as the code leaves them, the jal sites of the boot ELF and the
// other modules read from the live words (jalTarget). Not as the original:
// CheckLetter's loop checkpoint is left out; GetScriptClass takes the
// recompiled code (before any store) for a jump table entry it does not know.
#include "sotc_decomp.h"

#include "ps2_runtime_macros.h"
#include "runtime/ps2_memory.h"

#include <cstring>

namespace decomp_scriptvm3
{
    using sotc_decomp::callGuest;
    using sotc_decomp::callNative;
    using sotc_decomp::jalTarget;

    constexpr uint32_t kWorkStack = 0x011F7140u;  // the work stack's top; its word holds the helper's frame sp
    constexpr uint32_t kWorkStack1E = 0x011E7140u; // v1's value in the switch (lui 0x11e + 0x7140)
    constexpr uint32_t kGlobals = 0x01465650u;    // the script system's globals (lui 0x146 + 0x5650)
    constexpr uint32_t kGetCamera = 0x0141D390u;  // get_main_cameraman_camera_obj
    constexpr uint32_t kAreaExist = 0x01365450u;  // StageManagerCheckAreaExist
    constexpr uint32_t kGetCharacterClass = 0x010C0A70u, kGetLayoutClass = 0x010F1A20u, kGetGeometryPointerObject = 0x011329C0u;
    constexpr uint32_t kGetThisSubThread = 0x001CA1B0u;

    inline void fallback(uint32_t self, uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
    {
        ctx->pc = self;
        sotc_decomp::recompiled(self)(rdram, ctx, runtime);
    }

    inline void loadF(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, int reg, uint32_t addr)
    {
        uint32_t bits = READ32(addr);
        float f;
        std::memcpy(&f, &bits, sizeof(f));
        ctx->f[reg] = f;
    }
    inline void storeF(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, int reg, uint32_t addr)
    {
        float f = ctx->f[reg];
        uint32_t bits;
        std::memcpy(&bits, &f, sizeof(bits));
        WRITE32(addr, bits);
    }
    inline void move(R5900Context *ctx, int rd, int rs) { SET_GPR_U64(ctx, rd, (uint64_t)GPR_U64(ctx, rs) + (uint64_t)GPR_U64(ctx, 0)); } // daddu rd, rs, $zero
    inline void li(R5900Context *ctx, int rd, uint32_t v) { SET_GPR_S32(ctx, rd, (int32_t)v); }

    void getThisSubThread(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);

    // ---- the work stack (the frame end's helpers) -------------------------
    // a0 = the word's old value, the word = the frame sp, v1 = 0x11e7140,
    // v0 = sp = 0x11f7140 (e.g. 0x13cfcc0-0x13cfcf0).
    inline void enterWork(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
    {
        SET_GPR_S32(ctx, 4, (int32_t)READ32(kWorkStack));
        li(ctx, 2, kWorkStack);
        WRITE32(kWorkStack, GPR_U32(ctx, 29));
        li(ctx, 3, kWorkStack1E);
        li(ctx, 2, kWorkStack);
        move(ctx, 29, 2);
    }
    // v1 = the word, v0 = 0x11f7140, sp = the word (the frame sp; e.g.
    // 0x13cfd48-0x13cfd5c). Returns the frame sp.
    inline uint32_t leaveWork(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
    {
        SET_GPR_S32(ctx, 3, (int32_t)READ32(kWorkStack));
        li(ctx, 2, kWorkStack);
        SET_GPR_S32(ctx, 29, (int32_t)READ32(kWorkStack));
        return GPR_U32(ctx, 29);
    }
    // jal isysGetThisSubThread (a relocated site) on the work stack.
    inline void thisSubThread(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t site, uint32_t ra)
    {
        callNative(rdram, ctx, runtime, jalTarget(rdram, site), &getThisSubThread, ra, kWorkStack);
    }
    inline void leave(R5900Context *ctx, uint32_t frameSp, uint32_t size)
    {
        SET_GPR_S32(ctx, 29, (int32_t)(frameSp + size));
        ctx->pc = GPR_U32(ctx, 31);
    }

    // ---- gamecore_sub_013CFCA0: an LWS orient object ------------------------
    void lwsOrient(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
    {
        if (!sotc_decomp::scriptVmOn())
            return fallback(0x013CFCA0u, rdram, ctx, runtime);
        const uint32_t sp = GPR_U32(ctx, 29) - 0x30u;
        SET_GPR_S32(ctx, 29, (int32_t)sp);
        WRITE64(sp + 8u, GPR_U64(ctx, 17));
        WRITE64(sp + 0u, GPR_U64(ctx, 16));
        li(ctx, 17, kWorkStack); // s1
        WRITE64(sp + 0x10u, GPR_U64(ctx, 18));
        WRITE64(sp + 0x18u, GPR_U64(ctx, 31));
        storeF(rdram, ctx, runtime, 20, sp + 0x20u);
        enterWork(rdram, ctx, runtime);
        li(ctx, 16, 0x01460000u); // s0 (lui, the delay slot)
        thisSubThread(rdram, ctx, runtime, 0x013CFCF4u, 0x013CFCFCu);
        li(ctx, 16, kGlobals);
        move(ctx, 18, 2); // s2 = the object
        callGuest(rdram, ctx, runtime, kGetCamera, 0x013CFD08u, kWorkStack);
        move(ctx, 4, 18);
        loadF(rdram, ctx, runtime, 12, kGlobals + 0x188u); // the clip interval
        move(ctx, 5, 2);
        callGuest(rdram, ctx, runtime, jalTarget(rdram, 0x013CFD10u), 0x013CFD18u, kWorkStack); // ViewIntervalClip
        ctx->f[20] = FPU_MOV_S(ctx->f[0]);
        callGuest(rdram, ctx, runtime, kGetCamera, 0x013CFD20u, kWorkStack);
        li(ctx, 1, 0x3F800000u);
        { uint32_t bits = GPR_U32(ctx, 1); std::memcpy(&ctx->f[13], &bits, sizeof(bits)); }
        ctx->f[12] = FPU_MOV_S(ctx->f[20]);
        move(ctx, 4, 18);
        move(ctx, 5, 2);
        ctx->f[13] = ps2FpuDivide(ctx->fcr31, ctx->f[13], ctx->f[12]);
        li(ctx, 6, kGlobals + 0x370u);
        callGuest(rdram, ctx, runtime, jalTarget(rdram, 0x013CFD40u), 0x013CFD48u, kWorkStack); // ExecScriptLwsOrientClass
        const uint32_t f = leaveWork(rdram, ctx, runtime);
        SET_GPR_U64(ctx, 16, READ64(f + 0u));
        SET_GPR_U64(ctx, 17, READ64(f + 8u));
        SET_GPR_U64(ctx, 18, READ64(f + 0x10u));
        SET_GPR_U64(ctx, 31, READ64(f + 0x18u));
        loadF(rdram, ctx, runtime, 20, f + 0x20u);
        leave(ctx, f, 0x30u);
    }

    // ---- gamecore_sub_013CFE40: an LWS demo orient manager ------------------
    void lwsDemo(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
    {
        if (!sotc_decomp::scriptVmOn())
            return fallback(0x013CFE40u, rdram, ctx, runtime);
        const uint32_t sp = GPR_U32(ctx, 29) - 0x10u;
        SET_GPR_S32(ctx, 29, (int32_t)sp);
        WRITE64(sp + 0u, GPR_U64(ctx, 16));
        li(ctx, 16, kWorkStack); // s0
        WRITE64(sp + 8u, GPR_U64(ctx, 31));
        enterWork(rdram, ctx, runtime);
        thisSubThread(rdram, ctx, runtime, 0x013CFE88u, 0x013CFE90u);
        li(ctx, 3, kGlobals);
        move(ctx, 4, 2);
        loadF(rdram, ctx, runtime, 13, kGlobals + 0x180u);
        loadF(rdram, ctx, runtime, 12, kGlobals + 0x178u);
        callGuest(rdram, ctx, runtime, jalTarget(rdram, 0x013CFEA0u), 0x013CFEA8u, kWorkStack); // ExecLwsDemoOrientManagerObj
        const uint32_t f = leaveWork(rdram, ctx, runtime);
        SET_GPR_U64(ctx, 16, READ64(f + 0u));
        SET_GPR_U64(ctx, 31, READ64(f + 8u));
        leave(ctx, f, 0x10u);
    }

    // ---- gamecore_sub_013CFBE0: a character's geometry ----------------------
    void charaGeometry(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
    {
        if (!sotc_decomp::scriptVmOn())
            return fallback(0x013CFBE0u, rdram, ctx, runtime);
        const uint32_t sp = GPR_U32(ctx, 29) - 0x20u;
        SET_GPR_S32(ctx, 29, (int32_t)sp);
        WRITE64(sp + 8u, GPR_U64(ctx, 17));
        WRITE64(sp + 0u, GPR_U64(ctx, 16));
        li(ctx, 17, kWorkStack); // s1
        WRITE64(sp + 0x10u, GPR_U64(ctx, 18));
        WRITE64(sp + 0x18u, GPR_U64(ctx, 31));
        enterWork(rdram, ctx, runtime);
        thisSubThread(rdram, ctx, runtime, 0x013CFC30u, 0x013CFC38u);
        move(ctx, 16, 2); // s0 = the object
        move(ctx, 4, 16);
        callGuest(rdram, ctx, runtime, jalTarget(rdram, 0x013CFC3Cu), 0x013CFC44u, kWorkStack); // GetCharacterClass
        li(ctx, 5, 0x014659C0u);
        move(ctx, 4, 16);
        move(ctx, 18, 2); // s2 = the character class
        callGuest(rdram, ctx, runtime, jalTarget(rdram, 0x013CFC50u), 0x013CFC58u, kWorkStack); // ExecCharaGeometry
        move(ctx, 4, 16);
        callGuest(rdram, ctx, runtime, jalTarget(rdram, 0x013CFC58u), 0x013CFC60u, kWorkStack); // GetLayoutClass
        SET_GPR_S32(ctx, 4, (int32_t)READ32(GPR_U32(ctx, 2) + 0x134u)); // the layout's area
        callGuest(rdram, ctx, runtime, kAreaExist, 0x013CFC68u, kWorkStack);
        WRITE8(GPR_U32(ctx, 18) + 0x539u, (uint8_t)GPR_U32(ctx, 2));
        const uint32_t f = leaveWork(rdram, ctx, runtime);
        SET_GPR_U64(ctx, 16, READ64(f + 0u));
        SET_GPR_U64(ctx, 17, READ64(f + 8u));
        SET_GPR_U64(ctx, 18, READ64(f + 0x10u));
        SET_GPR_U64(ctx, 31, READ64(f + 0x18u));
        leave(ctx, f, 0x20u);
    }

    // ---- gamecore_sub_013CFD80: an object animation -------------------------
    void objectAnimation(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
    {
        if (!sotc_decomp::scriptVmOn())
            return fallback(0x013CFD80u, rdram, ctx, runtime);
        const uint32_t sp = GPR_U32(ctx, 29) - 0x20u;
        SET_GPR_S32(ctx, 29, (int32_t)sp);
        li(ctx, 2, 0x011F0000u);
        WRITE64(sp + 8u, GPR_U64(ctx, 17));
        li(ctx, 17, kWorkStack); // s1
        WRITE64(sp + 0u, GPR_U64(ctx, 16));
        WRITE64(sp + 0x10u, GPR_U64(ctx, 31));
        enterWork(rdram, ctx, runtime);
        thisSubThread(rdram, ctx, runtime, 0x013CFDCCu, 0x013CFDD4u);
        move(ctx, 16, 2); // s0 = the object (the delay slot)
        callGuest(rdram, ctx, runtime, kGetCamera, 0x013CFDDCu, kWorkStack);
        li(ctx, 3, 0x01460000u);
        move(ctx, 5, 2);
        li(ctx, 3, kGlobals);
        move(ctx, 4, 16);
        loadF(rdram, ctx, runtime, 13, kGlobals + 0x194u);
        loadF(rdram, ctx, runtime, 12, kGlobals + 0x190u);
        callGuest(rdram, ctx, runtime, jalTarget(rdram, 0x013CFDF0u), 0x013CFDF8u, kWorkStack); // ExecObjectAnimation
        move(ctx, 4, 16);
        li(ctx, 3, 0x011E0000u);
        const bool done = GPR_U64(ctx, 2) == 0u;
        WRITE32(0x011E7130u, GPR_U32(ctx, 2));
        if (!done)
            callGuest(rdram, ctx, runtime, jalTarget(rdram, 0x013CFE08u), 0x013CFE10u, kWorkStack); // isysRequestDeleteObj
        const uint32_t f = leaveWork(rdram, ctx, runtime);
        SET_GPR_U64(ctx, 16, READ64(f + 0u));
        SET_GPR_U64(ctx, 17, READ64(f + 8u));
        SET_GPR_U64(ctx, 31, READ64(f + 0x10u));
        leave(ctx, f, 0x20u);
    }

    // ---- gamecore_sub_013CF4E0: the character type's tail -------------------
    void charaTail(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
    {
        if (!sotc_decomp::scriptVmOn())
            return fallback(0x013CF4E0u, rdram, ctx, runtime);
        const uint32_t sp = GPR_U32(ctx, 29) - 0x30u;
        SET_GPR_S32(ctx, 29, (int32_t)sp);
        WRITE64(sp + 0x18u, GPR_U64(ctx, 19));
        li(ctx, 19, 0x011F0000u); // s3
        WRITE64(sp + 0u, GPR_U64(ctx, 16));
        li(ctx, 3, kWorkStack);
        WRITE64(sp + 8u, GPR_U64(ctx, 17));
        WRITE64(sp + 0x10u, GPR_U64(ctx, 18));
        WRITE64(sp + 0x20u, GPR_U64(ctx, 31));
        storeF(rdram, ctx, runtime, 20, sp + 0x28u);
        enterWork(rdram, ctx, runtime);
        li(ctx, 18, 0x01460000u); // s2 (the delay slot)
        thisSubThread(rdram, ctx, runtime, 0x013CF538u, 0x013CF540u);
        move(ctx, 17, 2); // s1 = the object
        move(ctx, 4, 17);
        callGuest(rdram, ctx, runtime, jalTarget(rdram, 0x013CF544u), 0x013CF54Cu, kWorkStack); // GetCharacterClass
        move(ctx, 16, 2); // s0 = the character class
        callGuest(rdram, ctx, runtime, kGetCamera, 0x013CF554u, kWorkStack);
        li(ctx, 3, kGlobals);
        move(ctx, 5, 2);
        loadF(rdram, ctx, runtime, 12, kGlobals + 0x188u);
        move(ctx, 4, 17);
        callGuest(rdram, ctx, runtime, jalTarget(rdram, 0x013CF560u), 0x013CF568u, kWorkStack); // ViewIntervalClip
        SET_GPR_S32(ctx, 2, (int8_t)READ8(GPR_U32(ctx, 16) + 0x534u));
        ctx->f[20] = FPU_MOV_S(ctx->f[0]);
        if (GPR_U64(ctx, 2) == 0u)
            goto l670;
        move(ctx, 4, 17);
        callGuest(rdram, ctx, runtime, jalTarget(rdram, 0x013CF574u), 0x013CF57Cu, kWorkStack); // GetCharacterClass
        SET_GPR_S32(ctx, 3, (int32_t)READ32(GPR_U32(ctx, 2) + 0x1170u));
        if (GPR_U64(ctx, 3) == 0u)
            goto l648;
        move(ctx, 4, 17);
        callGuest(rdram, ctx, runtime, jalTarget(rdram, 0x013CF588u), 0x013CF590u, kWorkStack); // GetLayoutClass
        SET_GPR_S32(ctx, 4, (int32_t)READ32(GPR_U32(ctx, 2) + 0x134u));
        callGuest(rdram, ctx, runtime, kAreaExist, 0x013CF598u, kWorkStack);
        {
            const bool exists = GPR_U64(ctx, 2) != 0u;
            li(ctx, 2, 4u);
            if (exists)
                goto l648;
        }
        SET_GPR_S32(ctx, 3, (int16_t)READ16(GPR_U32(ctx, 16) + 0x370u));
        if (GPR_U64(ctx, 3) == GPR_U64(ctx, 2))
            goto l648;
        move(ctx, 4, 17);
        callGuest(rdram, ctx, runtime, jalTarget(rdram, 0x013CF5ACu), 0x013CF5B4u, kWorkStack); // GetLayoutClass
        // The layout's geometry parent's type: 13 / 9 / 22 -> 1, else 0.
        move(ctx, 4, 2);
        SET_GPR_U64(ctx, 2, 0u);
        if (GPR_U64(ctx, 4) == 0u)
            goto l61c;
        SET_GPR_S32(ctx, 2, (int32_t)READ32(GPR_U32(ctx, 4) + 0x124u));
        if (GPR_U64(ctx, 2) == 0u)
        {
            SET_GPR_U64(ctx, 2, 0u);
            goto l61c;
        }
        SET_GPR_S32(ctx, 4, (int32_t)(GPR_U32(ctx, 4) + 0xC0u));
        callGuest(rdram, ctx, runtime, 0x0137D640u, 0x013CF5D4u, kWorkStack); // SCRGetGetGeometryPointerLegalParentObj
        if (GPR_U64(ctx, 2) == 0u)
        {
            SET_GPR_U64(ctx, 2, 0u);
            goto l61c;
        }
        SET_GPR_ZE32(ctx, 3, (uint8_t)READ8(GPR_U32(ctx, 2) + 8u));
        li(ctx, 2, 13u);
        {
            const bool is13 = GPR_U64(ctx, 3) == GPR_U64(ctx, 2);
            li(ctx, 2, 1u);
            if (is13)
                goto l61c;
        }
        SET_GPR_U64(ctx, 2, ((int64_t)GPR_S64(ctx, 3) < (int64_t)(int32_t)14) ? 1 : 0);
        {
            const bool ge14 = GPR_U64(ctx, 2) == 0u;
            li(ctx, 2, 22u);
            if (ge14)
                goto l610;
        }
        li(ctx, 2, 9u);
        if (GPR_U64(ctx, 3) == GPR_U64(ctx, 2))
        {
            li(ctx, 2, 1u);
            goto l61c;
        }
        SET_GPR_U64(ctx, 2, 0u);
        goto l61c;
    l610:
        if (GPR_U64(ctx, 3) != GPR_U64(ctx, 2))
        {
            SET_GPR_U64(ctx, 2, 0u);
            goto l61c;
        }
        li(ctx, 2, 1u);
    l61c:
        {
            const bool parent = GPR_U64(ctx, 2) != 0u;
            li(ctx, 3, 1u);
            if (parent)
                goto l648;
        }
        SET_GPR_ZE32(ctx, 2, (uint8_t)READ8(GPR_U32(ctx, 16) + 0xD90u));
        if (GPR_U64(ctx, 2) == GPR_U64(ctx, 3))
            goto l648;
        SET_GPR_S32(ctx, 2, (int32_t)READ32(GPR_U32(ctx, 16) + 0x1070u));
        {
            const bool one = GPR_U64(ctx, 2) == GPR_U64(ctx, 3);
            li(ctx, 2, 11u);
            if (one)
                goto l648;
        }
        SET_GPR_S32(ctx, 3, (int16_t)READ16(GPR_U32(ctx, 16) + 0x370u));
        {
            const bool skip = GPR_U64(ctx, 3) != GPR_U64(ctx, 2);
            SET_GPR_S32(ctx, 4, (int32_t)(GPR_U32(ctx, 19) + 0x7140u));
            if (skip)
                goto l674;
        }
    l648: // the geometry pre-process
        li(ctx, 1, 0x3F800000u);
        { uint32_t bits = GPR_U32(ctx, 1); std::memcpy(&ctx->f[14], &bits, sizeof(bits)); }
        ctx->f[13] = FPU_MOV_S(ctx->f[20]);
        li(ctx, 2, kGlobals);
        move(ctx, 4, 17);
        ctx->f[14] = ps2FpuDivide(ctx->fcr31, ctx->f[14], ctx->f[13]);
        loadF(rdram, ctx, runtime, 12, kGlobals + 0x178u);
        callGuest(rdram, ctx, runtime, jalTarget(rdram, 0x013CF668u), 0x013CF670u, kWorkStack); // ExecCharaGeometryPreprocess
    l670:
        SET_GPR_S32(ctx, 4, (int32_t)(GPR_U32(ctx, 19) + 0x7140u));
    l674:
        const uint32_t f = leaveWork(rdram, ctx, runtime);
        SET_GPR_U64(ctx, 16, READ64(f + 0u));
        SET_GPR_U64(ctx, 17, READ64(f + 8u));
        SET_GPR_U64(ctx, 18, READ64(f + 0x10u));
        SET_GPR_U64(ctx, 19, READ64(f + 0x18u));
        SET_GPR_U64(ctx, 31, READ64(f + 0x20u));
        loadF(rdram, ctx, runtime, 20, f + 0x28u);
        leave(ctx, f, 0x30u);
    }

    // ---- the script class accessors -----------------------------------------
    void getScriptCharacterClass(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
    {
        if (!sotc_decomp::scriptVmOn())
            return fallback(0x0110CB28u, rdram, ctx, runtime);
        move(ctx, 2, 4);
        li(ctx, 4, 9u);
        SET_GPR_ZE32(ctx, 3, (uint8_t)READ8(GPR_U32(ctx, 2) + 8u));
        SET_GPR_S32(ctx, 2, (int32_t)(GPR_U32(ctx, 2) + 0x20u));
        SET_GPR_U64(ctx, 3, GPR_U64(ctx, 3) ^ GPR_U64(ctx, 4));
        if (GPR_U64(ctx, 3) != 0)
            SET_GPR_VEC(ctx, 2, GPR_VEC(ctx, 0));
        ctx->pc = GPR_U32(ctx, 31);
    }

    void getScriptLayoutClass(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
    {
        if (!sotc_decomp::scriptVmOn())
            return fallback(0x0110D058u, rdram, ctx, runtime);
        const uint32_t entrySp = GPR_U32(ctx, 29);
        const uint32_t sp = entrySp - 0x10u;
        SET_GPR_S32(ctx, 29, (int32_t)sp);
        move(ctx, 2, 4);
        WRITE64(sp, GPR_U64(ctx, 31));
        li(ctx, 3, 11u);
        SET_GPR_ZE32(ctx, 5, (uint8_t)READ8(GPR_U32(ctx, 2) + 8u));
        {
            const bool layout = GPR_U64(ctx, 5) == GPR_U64(ctx, 3);
            SET_GPR_S32(ctx, 2, (int32_t)(GPR_U32(ctx, 2) + 0x20u));
            if (!layout)
            {
                li(ctx, 2, 13u);
                if (GPR_U64(ctx, 5) != GPR_U64(ctx, 2))
                    SET_GPR_U64(ctx, 2, 0u);
                else
                {
                    // An animation object: j GetScriptAnimationClass.
                    SET_GPR_U64(ctx, 31, READ64(sp));
                    SET_GPR_S32(ctx, 29, (int32_t)(sp + 0x10u));
                    callGuest(rdram, ctx, runtime, 0x0110A838u, GPR_U32(ctx, 31), entrySp);
                    return;
                }
            }
        }
        SET_GPR_U64(ctx, 31, READ64(sp));
        SET_GPR_S32(ctx, 29, (int32_t)(sp + 0x10u));
        ctx->pc = GPR_U32(ctx, 31);
    }

    void getScriptClass(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
    {
        if (!sotc_decomp::scriptVmOn())
            return fallback(0x01102438u, rdram, ctx, runtime);
        // The type's jump table entry, read before any store (an entry not
        // known here: the recompiled code).
        const uint32_t idx = (uint32_t)READ8(GPR_U32(ctx, 4) + 8u) - 6u;
        const bool inTable = (uint64_t)(int64_t)(int32_t)idx < 28u;
        uint32_t target = 0u;
        if (inTable)
        {
            target = READ32(0x01190000u + (idx << 2) + 0x4CC0u);
            switch (target)
            {
            case 0x01102470u: case 0x01102480u: case 0x01102490u: case 0x011024A0u: case 0x011024B0u:
            case 0x011024C0u: case 0x011024D0u: case 0x011024D8u: case 0x011024E0u:
                break;
            default:
                return fallback(0x01102438u, rdram, ctx, runtime);
            }
        }
        const uint32_t entrySp = GPR_U32(ctx, 29);
        const uint32_t sp = entrySp - 0x10u;
        SET_GPR_S32(ctx, 29, (int32_t)sp);
        WRITE64(sp, GPR_U64(ctx, 31));
        SET_GPR_S32(ctx, 2, (int32_t)idx);
        SET_GPR_U64(ctx, 3, inTable ? 1 : 0);
        if (!inTable)
            SET_GPR_U64(ctx, 2, 0u);
        else
        {
            SET_GPR_S32(ctx, 2, (int32_t)(idx << 2));
            SET_GPR_S32(ctx, 3, (int32_t)(0x01190000u + (idx << 2)));
            SET_GPR_S32(ctx, 3, (int32_t)target);
            ctx->pc = target;
            uint32_t tail = 0u;
            PS2Runtime::RecompiledFunction known = nullptr;
            switch (target)
            {
            case 0x01102470u: tail = 0x0110D058u; known = &getScriptLayoutClass; break;
            case 0x01102480u: tail = 0x0110CB28u; known = &getScriptCharacterClass; break;
            case 0x01102490u: tail = 0x01156628u; break; // GetWindowClass
            case 0x011024A0u: tail = 0x0110A838u; break; // GetScriptAnimationClass
            case 0x011024B0u: tail = 0x01114238u; break; // GetScriptSpriteClass
            case 0x011024C0u: tail = 0x011119B8u; break; // GetScriptSpriteMenuClass
            case 0x011024D0u: SET_GPR_S32(ctx, 2, (int32_t)(GPR_U32(ctx, 4) + 0x20u)); break;
            case 0x011024D8u: SET_GPR_S32(ctx, 2, (int32_t)(GPR_U32(ctx, 4) + 0x1Cu)); break;
            default: SET_GPR_U64(ctx, 2, 0u); break; // 0x11024e0
            }
            if (tail)
            {
                SET_GPR_U64(ctx, 31, READ64(sp));
                SET_GPR_S32(ctx, 29, (int32_t)(sp + 0x10u));
                if (known)
                    callNative(rdram, ctx, runtime, tail, known, GPR_U32(ctx, 31), entrySp);
                else
                    callGuest(rdram, ctx, runtime, tail, GPR_U32(ctx, 31), entrySp);
                return;
            }
        }
        SET_GPR_U64(ctx, 31, READ64(sp));
        SET_GPR_S32(ctx, 29, (int32_t)(sp + 0x10u));
        ctx->pc = GPR_U32(ctx, 31);
    }

    // ---- the letters ----------------------------------------------------------
    void checkLetter(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
    {
        if (!sotc_decomp::scriptVmOn())
            return fallback(0x01166BA0u, rdram, ctx, runtime);
        const uint32_t entrySp = GPR_U32(ctx, 29);
        const uint32_t sp = entrySp - 0x20u;
        SET_GPR_S32(ctx, 29, (int32_t)sp);
        WRITE64(sp + 8u, GPR_U64(ctx, 17));
        move(ctx, 17, 4); // s1 = the object
        WRITE64(sp + 0x10u, GPR_U64(ctx, 18));
        move(ctx, 18, 5); // s2 = the callback
        WRITE64(sp + 0u, GPR_U64(ctx, 16));
        WRITE64(sp + 0x18u, GPR_U64(ctx, 31));
        SET_GPR_S32(ctx, 16, (int32_t)READ32(GPR_U32(ctx, 17) + 0x1Cu)); // s0 = the post
        move(ctx, 4, 16);
        callGuest(rdram, ctx, runtime, jalTarget(rdram, 0x01166BC0u), 0x01166BC8u, sp); // open_letter_post
        for (;;)
        {
            move(ctx, 4, 16);
            callGuest(rdram, ctx, runtime, jalTarget(rdram, 0x01166BE4u), 0x01166BECu, sp); // get_next_letter_in_post
            move(ctx, 4, 2);
            if (GPR_U64(ctx, 2) == 0u)
                break;
            callGuest(rdram, ctx, runtime, jalTarget(rdram, 0x01166BD0u), 0x01166BD8u, sp); // get_letter_contents_data
            move(ctx, 4, 17);
            move(ctx, 5, 2);
            callGuest(rdram, ctx, runtime, GPR_U32(ctx, 18), 0x01166BE4u, sp); // jalr $s2 (obj, contents)
        }
        move(ctx, 4, 16);
        SET_GPR_U64(ctx, 16, READ64(sp + 0u));
        SET_GPR_U64(ctx, 17, READ64(sp + 8u));
        SET_GPR_U64(ctx, 18, READ64(sp + 0x10u));
        SET_GPR_U64(ctx, 31, READ64(sp + 0x18u));
        SET_GPR_S32(ctx, 29, (int32_t)(sp + 0x20u));
        callGuest(rdram, ctx, runtime, jalTarget(rdram, 0x01166C08u), GPR_U32(ctx, 31), entrySp); // j: the post's close
    }

    void execScriptLayoutObj(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
    {
        if (!sotc_decomp::scriptVmOn())
            return fallback(0x0110D0F8u, rdram, ctx, runtime);
        const uint32_t entrySp = GPR_U32(ctx, 29);
        const uint32_t sp = entrySp - 0x10u;
        SET_GPR_S32(ctx, 29, (int32_t)sp);
        WRITE64(sp + 0u, GPR_U64(ctx, 16));
        WRITE64(sp + 8u, GPR_U64(ctx, 31));
        move(ctx, 16, 4); // s0 = the object
        callGuest(rdram, ctx, runtime, 0x01166C10u, 0x0110D10Cu, sp); // GetDamageClass
        move(ctx, 4, 2);
        callGuest(rdram, ctx, runtime, 0x01166DE8u, 0x0110D114u, sp); // ResetDamageClass
        li(ctx, 5, 0x01160000u);
        move(ctx, 4, 16);
        SET_GPR_U64(ctx, 16, READ64(sp + 0u));
        SET_GPR_U64(ctx, 31, READ64(sp + 8u));
        li(ctx, 5, 0x01166C98u); // CheckDamageLetter
        SET_GPR_S32(ctx, 29, (int32_t)(sp + 0x10u));
        callNative(rdram, ctx, runtime, 0x01166BA0u, &checkLetter, GPR_U32(ctx, 31), entrySp); // j CheckLetter
    }

    // ---- the script control status ------------------------------------------
    // The control target's character class (v0, 0: none) as 010CC100 /
    // 010CC178 / 010CC210 find it (0x10cc10c-0x10cc154; s0 = a0 before).
    // Returns false when the first GetCharacterClass found it (the code's
    // branch straight to the store: s0 stays a0 there).
    inline bool controlTarget(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t sp, uint32_t ra0, uint32_t ra1, uint32_t ra2,
                              uint32_t ra3)
    {
        callGuest(rdram, ctx, runtime, kGetCharacterClass, ra0, sp);
        move(ctx, 4, 16);
        if (GPR_U64(ctx, 2) != 0u)
            return false;
        SET_GPR_U64(ctx, 16, 0u);
        callGuest(rdram, ctx, runtime, kGetLayoutClass, ra1, sp);
        SET_GPR_S32(ctx, 3, (int32_t)READ32(GPR_U32(ctx, 2) + 0x124u));
        SET_GPR_S32(ctx, 4, (int32_t)(GPR_U32(ctx, 2) + 0xC0u));
        if (GPR_U64(ctx, 3) != 0u)
        {
            callGuest(rdram, ctx, runtime, kGetGeometryPointerObject, ra2, sp);
            li(ctx, 5, 22u);
            SET_GPR_ZE32(ctx, 3, (uint8_t)READ8(GPR_U32(ctx, 2) + 8u));
            const bool chara = GPR_U64(ctx, 3) == GPR_U64(ctx, 5);
            move(ctx, 4, 2);
            if (chara)
            {
                callGuest(rdram, ctx, runtime, kGetCharacterClass, ra3, sp);
                move(ctx, 16, 2);
            }
        }
        move(ctx, 2, 16);
        return true;
    }

    void resetAllScriptCtrlStatus(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
    {
        if (!sotc_decomp::scriptVmOn())
            return fallback(0x010CC100u, rdram, ctx, runtime);
        const uint32_t sp = GPR_U32(ctx, 29) - 0x10u;
        SET_GPR_S32(ctx, 29, (int32_t)sp);
        WRITE64(sp + 0u, GPR_U64(ctx, 16));
        WRITE64(sp + 8u, GPR_U64(ctx, 31));
        move(ctx, 16, 4);
        const bool searched = controlTarget(rdram, ctx, runtime, sp, 0x010CC114u, 0x010CC124u, 0x010CC138u, 0x010CC150u);
        if (searched)
            SET_GPR_U64(ctx, 16, READ64(sp + 0u)); // the delay slot
        if (!searched || GPR_U64(ctx, 2) != 0u)
        {
            WRITE64(GPR_U32(ctx, 2) + 0x920u, 0u);
            SET_GPR_U64(ctx, 16, READ64(sp + 0u));
        }
        SET_GPR_U64(ctx, 31, READ64(sp + 8u));
        leave(ctx, sp, 0x10u);
    }

    // Set (or clear) bit a1 of the control target's +0x920 mask.
    inline void scriptCtrlStatus(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, bool set, uint32_t base)
    {
        const uint32_t sp = GPR_U32(ctx, 29) - 0x20u;
        SET_GPR_S32(ctx, 29, (int32_t)sp);
        WRITE64(sp + 0u, GPR_U64(ctx, 16));
        move(ctx, 16, 4);
        WRITE64(sp + 8u, GPR_U64(ctx, 17));
        WRITE64(sp + 0x10u, GPR_U64(ctx, 31));
        move(ctx, 17, 5); // s1 = the bit
        const bool searched = controlTarget(rdram, ctx, runtime, sp, base + 0x1Cu, base + 0x2Cu, base + 0x40u, base + 0x58u);
        if (searched)
            SET_GPR_U64(ctx, 16, READ64(sp + 0u)); // the delay slot
        if (!searched || GPR_U64(ctx, 2) != 0u)
        {
            li(ctx, 4, GPR_U32(ctx, 2) + 0x920u);
            li(ctx, 3, 1u);
            SET_GPR_U64(ctx, 2, READ64(GPR_U32(ctx, 4)));
            SET_GPR_U64(ctx, 3, GPR_U64(ctx, 3) << (GPR_U32(ctx, 17) & 0x3F));
            if (set)
                SET_GPR_U64(ctx, 2, GPR_U64(ctx, 2) | GPR_U64(ctx, 3));
            else
            {
                SET_GPR_U64(ctx, 3, ~(GPR_U64(ctx, 0) | GPR_U64(ctx, 3)));
                SET_GPR_U64(ctx, 2, GPR_U64(ctx, 2) & GPR_U64(ctx, 3));
            }
            WRITE64(GPR_U32(ctx, 4), GPR_U64(ctx, 2));
            SET_GPR_U64(ctx, 16, READ64(sp + 0u));
        }
        SET_GPR_U64(ctx, 17, READ64(sp + 8u));
        SET_GPR_U64(ctx, 31, READ64(sp + 0x10u));
        leave(ctx, sp, 0x20u);
    }

    void setScriptCtrlStatus(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
    {
        if (!sotc_decomp::scriptVmOn())
            return fallback(0x010CC178u, rdram, ctx, runtime);
        scriptCtrlStatus(rdram, ctx, runtime, true, 0x010CC178u);
    }

    void resetScriptCtrlStatus(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
    {
        if (!sotc_decomp::scriptVmOn())
            return fallback(0x010CC210u, rdram, ctx, runtime);
        scriptCtrlStatus(rdram, ctx, runtime, false, 0x010CC210u);
    }

    void checkScriptControlDef(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
    {
        if (!sotc_decomp::scriptVmOn())
            return fallback(0x010CC730u, rdram, ctx, runtime);
        const bool none = GPR_U64(ctx, 5) == 0u;
        move(ctx, 2, 6);
        if (!none)
        {
            SET_GPR_S32(ctx, 3, (int32_t)READ32(GPR_U32(ctx, 5) + 4u)); // the bit
            li(ctx, 2, 1u);
            SET_GPR_S32(ctx, 5, (int32_t)READ32(GPR_U32(ctx, 5) + 8u)); // the mode
            li(ctx, 7, 1u);
            SET_GPR_U64(ctx, 2, GPR_U64(ctx, 2) << (GPR_U32(ctx, 3) & 0x3F));
            const bool mode1 = GPR_U64(ctx, 5) == GPR_U64(ctx, 7);
            SET_GPR_U64(ctx, 4, GPR_U64(ctx, 4) & GPR_U64(ctx, 2));
            bool done = false;
            if (!mode1)
            {
                SET_GPR_U64(ctx, 2, 0u);
                li(ctx, 3, 2u);
                const bool mode2 = GPR_U64(ctx, 5) == GPR_U64(ctx, 3);
                if (GPR_U64(ctx, 4) == 0)
                    SET_GPR_VEC(ctx, 2, GPR_VEC(ctx, 6));
                done = mode2;
            }
            if (!done)
            {
                move(ctx, 2, 7);
                if (GPR_U64(ctx, 4) == 0)
                    SET_GPR_VEC(ctx, 2, GPR_VEC(ctx, 6));
            }
        }
        ctx->pc = GPR_U32(ctx, 31);
    }

    // ---- isysGetThisSubThread -------------------------------------------------
    void getThisSubThread(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
    {
        if (!sotc_decomp::scriptVmOn())
            return fallback(kGetThisSubThread, rdram, ctx, runtime);
        li(ctx, 3, 0x001E0000u);
        SET_GPR_S32(ctx, 2, (int32_t)READ32(0x001E1550u));
        ctx->pc = GPR_U32(ctx, 31);
    }
}

SOTC_DECOMP_FUNCTION(0x013CFCA0u, gamecore_sub_013CFCA0, decomp_scriptvm3::lwsOrient);
SOTC_DECOMP_FUNCTION(0x013CFE40u, gamecore_sub_013CFE40, decomp_scriptvm3::lwsDemo);
SOTC_DECOMP_FUNCTION(0x013CFBE0u, gamecore_sub_013CFBE0, decomp_scriptvm3::charaGeometry);
SOTC_DECOMP_FUNCTION(0x013CFD80u, gamecore_sub_013CFD80, decomp_scriptvm3::objectAnimation);
SOTC_DECOMP_FUNCTION(0x013CF4E0u, gamecore_sub_013CF4E0, decomp_scriptvm3::charaTail);
SOTC_DECOMP_FUNCTION(0x01102438u, GetScriptClass, decomp_scriptvm3::getScriptClass);
SOTC_DECOMP_FUNCTION(0x0110CB28u, GetScriptCharacterClass, decomp_scriptvm3::getScriptCharacterClass);
SOTC_DECOMP_FUNCTION(0x0110D058u, GetScriptLayoutClass, decomp_scriptvm3::getScriptLayoutClass);
SOTC_DECOMP_FUNCTION(0x0110D0F8u, ExecScriptLayoutObj, decomp_scriptvm3::execScriptLayoutObj);
SOTC_DECOMP_FUNCTION(0x01166BA0u, CheckLetter, decomp_scriptvm3::checkLetter);
SOTC_DECOMP_FUNCTION(0x010CC210u, CharaCtrlResetScriptCtrlStatus, decomp_scriptvm3::resetScriptCtrlStatus);
SOTC_DECOMP_FUNCTION(0x010CC178u, CharaCtrlSetScriptCtrlStatus, decomp_scriptvm3::setScriptCtrlStatus);
SOTC_DECOMP_FUNCTION(0x010CC100u, CharaCtrlResetAllScriptCtrlStatus, decomp_scriptvm3::resetAllScriptCtrlStatus);
SOTC_DECOMP_FUNCTION(0x010CC730u, checkScriptControlDef, decomp_scriptvm3::checkScriptControlDef);
SOTC_DECOMP_FUNCTION(0x001CA1B0u, isysGetThisSubThread, decomp_scriptvm3::getThisSubThread);
