// Round I2: the lifted D files' call protocol out of line (PS2X_VITA_LIFTED_OUTLINE=ON; off by default).
//
// CMake force-includes this header (-include) into game/overrides/decomp_liftd*.cpp only, when the option is on. Every
// guest call / tail jump / jump the lifter writes (PS2_CALL_GUEST*, PS2_TAIL_GUEST_*, PS2_JUMP_GUEST: ~21 000 sites)
// then compiles to a store of ctx->pc, four argument registers and one bl into a shared helper below, instead of the
// ~25 inline instructions of PS2Runtime::callGuestKnown & co. (table slot, stack floor, EE checkpoint, the callee, the
// pc check) at every site. The per-site constants the slow paths need (known function, return pc, source pc, kind,
// name) sit in a static const descriptor next to the caller's other read-only data.
//
// Exact: each helper calls the very same always_inline runtime function (callGuestFn / callGuestKnown /
// callGuestKnownNoCheck / tailGuestKnown / jumpGuestFn) with the same arguments. The target is passed in ctx->pc: the
// macro stores it right before the helper call, and each of those functions' first effect on ctx->pc is that same
// store (callGuestFnBody / callGuestKnownBody / callGuestKnownNoCheck's fast path / jumpGuestFnBody), with nothing
// reading ctx->pc before it; tail jumps are always preceded by the generator's own `ctx->pc = target;` (the runtime's
// comment on PS2_TAIL_GUEST_KNOWN; checked over every site of the 52 files in round I2), so the macro's store repeats
// it. Vita only: the Mac difftest's trace hooks (PS2X_DIFFTRACE) record ctx->pc at the call event, before that store.
// What it costs: one more call level per guest call (the A9's return stack is 8 deep) and an indirect blx to the
// callee instead of a direct bl; the console's counters judge (docs/decomp_i2.md).
#pragma once

#include "sotc_decomp.h" // ps2_runtime.h, ps2_runtime_macros.h, the table's bounds (PS2_GUEST_FN)

#if PS2X_DIFFTRACE
#error "lift_outline.h (PS2X_VITA_LIFTED_OUTLINE): Vita only; the difftest's trace hooks read ctx->pc at the call event"
#endif

namespace ps2lift
{
    // One per call site, read only on the slow paths (and `known`).
    struct Site
    {
        PS2Runtime::RecompiledFunction known; // nullptr for PS2_CALL_GUEST (no known callee)
        uint32_t returnPc;
        uint32_t sourcePc;
        const char *name;
        PS2Runtime::GuestBranchKind kind;
    };
}

#if PS2X_N3_DIRECT
#define PS2LIFT_N3_FN(target, direct) (PS2_EXPECT(g_ps2DirectCalls, 1) ? (direct) : PS2_GUEST_FN(target))
#else
#define PS2LIFT_N3_FN(target, direct) PS2_GUEST_FN(target)
#endif

// noipa: one shared copy, no per-caller clones (constprop / isra) that would bring the bytes back. extern "C" inline:
// one COMDAT copy in the link, its section named .text.ps2lift_<name> (vita_hot_order.py puts them first).
#define PS2LIFT_HELPER extern "C" inline __attribute__((noinline, noipa))

PS2LIFT_HELPER bool ps2lift_call(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, const ps2lift::Site *s)
{
    const uint32_t target = ctx->pc;
    return runtime->callGuestFn(PS2_GUEST_FN(target), rdram, ctx, target, s->returnPc, s->sourcePc, s->kind, s->name);
}

PS2LIFT_HELPER bool ps2lift_callKnown(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, const ps2lift::Site *s)
{
    const uint32_t target = ctx->pc;
    return runtime->callGuestKnown(PS2_GUEST_FN(target), s->known, rdram, ctx, target, s->returnPc, s->sourcePc, s->kind, s->name);
}

PS2LIFT_HELPER bool ps2lift_callDirect(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, const ps2lift::Site *s)
{
    const uint32_t target = ctx->pc;
    return runtime->callGuestKnown(PS2LIFT_N3_FN(target, s->known), s->known, rdram, ctx, target, s->returnPc, s->sourcePc, s->kind, s->name);
}

PS2LIFT_HELPER bool ps2lift_callKnownNoChk(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, const ps2lift::Site *s)
{
    const uint32_t target = ctx->pc;
#if PS2X_N6_NOCHK
    return runtime->callGuestKnownNoCheck(PS2_GUEST_FN(target), s->known, rdram, ctx, target, s->returnPc, s->sourcePc, s->kind, s->name);
#else
    return runtime->callGuestKnown(PS2_GUEST_FN(target), s->known, rdram, ctx, target, s->returnPc, s->sourcePc, s->kind, s->name);
#endif
}

PS2LIFT_HELPER bool ps2lift_callDirectNoChk(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, const ps2lift::Site *s)
{
    const uint32_t target = ctx->pc;
#if PS2X_N6_NOCHK
    return runtime->callGuestKnownNoCheck(PS2LIFT_N3_FN(target, s->known), s->known, rdram, ctx, target, s->returnPc, s->sourcePc, s->kind, s->name);
#else
    return runtime->callGuestKnown(PS2LIFT_N3_FN(target, s->known), s->known, rdram, ctx, target, s->returnPc, s->sourcePc, s->kind, s->name);
#endif
}

PS2LIFT_HELPER void ps2lift_tailKnown(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, PS2Runtime::RecompiledFunction known)
{
    const uint32_t target = ctx->pc;
    PS2Runtime::tailGuestKnown(PS2_GUEST_FN(target), known, rdram, ctx, runtime, target);
}

PS2LIFT_HELPER void ps2lift_tailDirect(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, PS2Runtime::RecompiledFunction direct)
{
    const uint32_t target = ctx->pc;
    PS2Runtime::tailGuestKnown(PS2LIFT_N3_FN(target, direct), direct, rdram, ctx, runtime, target);
}

PS2LIFT_HELPER void ps2lift_jump(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t target)
{
    runtime->jumpGuestFn(PS2_GUEST_FN(target), rdram, ctx, target);
}

#undef PS2LIFT_HELPER

// The call macros (ps2_runtime_macros.h's names and arguments). The site's constants go into a static const descriptor.
#define PS2LIFT_SITE_CALL(helper, known, target, returnPc, sourcePc, kind, name)                                               \
    __extension__({                                                                                                       \
        static const ::ps2lift::Site ps2lift_site_ = {(known), (returnPc), (sourcePc), (name), PS2Runtime::GuestBranchKind::kind}; \
        ctx->pc = (target);                                                                                               \
        ::helper(rdram, ctx, runtime, &ps2lift_site_);                                                                    \
    })

#undef PS2_CALL_GUEST
#undef PS2_CALL_GUEST_KNOWN
#undef PS2_CALL_GUEST_DIRECT
#undef PS2_CALL_GUEST_NOCHK
#undef PS2_CALL_GUEST_DIRECT_NOCHK
#undef PS2_TAIL_GUEST_KNOWN
#undef PS2_TAIL_GUEST_DIRECT
#undef PS2_JUMP_GUEST

#define PS2_CALL_GUEST(target, returnPc, sourcePc, kind, name) \
    PS2LIFT_SITE_CALL(ps2lift_call, nullptr, target, returnPc, sourcePc, kind, name)
#define PS2_CALL_GUEST_KNOWN(target, function, returnPc, sourcePc, kind, name) \
    PS2LIFT_SITE_CALL(ps2lift_callKnown, &function, target, returnPc, sourcePc, kind, name)
#define PS2_CALL_GUEST_DIRECT(target, direct, returnPc, sourcePc, kind, name) \
    PS2LIFT_SITE_CALL(ps2lift_callDirect, &direct, target, returnPc, sourcePc, kind, name)
#define PS2_CALL_GUEST_NOCHK(target, function, returnPc, sourcePc, kind, name) \
    PS2LIFT_SITE_CALL(ps2lift_callKnownNoChk, &function, target, returnPc, sourcePc, kind, name)
#define PS2_CALL_GUEST_DIRECT_NOCHK(target, direct, returnPc, sourcePc, kind, name) \
    PS2LIFT_SITE_CALL(ps2lift_callDirectNoChk, &direct, target, returnPc, sourcePc, kind, name)
#define PS2_TAIL_GUEST_KNOWN(target, function) \
    do                                         \
    {                                          \
        ctx->pc = (target);                    \
        ::ps2lift_tailKnown(rdram, ctx, runtime, &function); \
    } while (0)
#define PS2_TAIL_GUEST_DIRECT(target, direct) \
    do                                        \
    {                                         \
        ctx->pc = (target);                   \
        ::ps2lift_tailDirect(rdram, ctx, runtime, &direct); \
    } while (0)
#define PS2_JUMP_GUEST(target) ::ps2lift_jump(rdram, ctx, runtime, (target))
