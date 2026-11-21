// a1_typed_check.cpp: round A1's differential check of the typed functions (tools/typed_lift.py --check-tu;
// docs/decomp_a1.md section 4). Generated: do not edit. Compiled to code only with PS2X_A1_CHECK=1 (a check build)
// or A1_OFFLINE (the offline harness, tools/a1_harness.cpp).
#include "typed_a1_check_rt.h"
#if PS2X_A1_CHECK || defined(A1_OFFLINE)
namespace a1ck
{
namespace ref
{
using lift_d1::sx64;
#include "typed_a1_ref.inc"
}
namespace ty
{
#include "typed_a1.inc"
}
}

namespace a1ck
{
void SetHeapStartPoint(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t p4, uint32_t p28)
{
    if (!ckBegin(0, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    ref::SetHeapStartPoint(rdram, &c, runtime);
    ckMid();
    const uint64_t r = a1_sx64(ty::SetHeapStartPoint(rdram, runtime, p4, p28));
    ckEnd(0, ctx, c, 1, r, 0.0f, 0x10u, 0x0u);
}
void GetHeapCurrentPoint(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t p28)
{
    if (!ckBegin(1, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    ref::GetHeapCurrentPoint(rdram, &c, runtime);
    ckMid();
    const uint64_t r = a1_sx64(ty::GetHeapCurrentPoint(rdram, runtime, p28));
    ckEnd(1, ctx, c, 1, r, 0.0f, 0x0u, 0x0u);
}
void LoaderSysCheckCDBootMode(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (!ckBegin(2, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    ref::LoaderSysCheckCDBootMode(rdram, &c, runtime);
    ckMid();
    const uint64_t r = a1_sx64(ty::LoaderSysCheckCDBootMode(rdram, runtime));
    ckEnd(2, ctx, c, 1, r, 0.0f, 0x0u, 0x0u);
}
void LoaderSysSendAbort(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (!ckBegin(3, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    ref::LoaderSysSendAbort(rdram, &c, runtime);
    ckMid();
    ty::LoaderSysSendAbort(rdram, runtime);
    ckEnd(3, ctx, c, 0, 0u, 0.0f, 0x0u, 0x0u);
}
void LoaderSysFlushPrint(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (!ckBegin(4, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    ref::LoaderSysFlushPrint(rdram, &c, runtime);
    ckMid();
    ty::LoaderSysFlushPrint(rdram, runtime);
    ckEnd(4, ctx, c, 0, 0u, 0.0f, 0x0u, 0x0u);
}
void LoaderSysGetBootArg(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (!ckBegin(5, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    ref::LoaderSysGetBootArg(rdram, &c, runtime);
    ckMid();
    const uint64_t r = a1_sx64(ty::LoaderSysGetBootArg(rdram, runtime));
    ckEnd(5, ctx, c, 1, r, 0.0f, 0x0u, 0x0u);
}
void LoaderSysHookPoint(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (!ckBegin(6, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    ref::LoaderSysHookPoint(rdram, &c, runtime);
    ckMid();
    ty::LoaderSysHookPoint(rdram, runtime);
    ckEnd(6, ctx, c, 0, 0u, 0.0f, 0x0u, 0x0u);
}
void initmemprintf(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (!ckBegin(7, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    ref::initmemprintf(rdram, &c, runtime);
    ckMid();
    ty::initmemprintf(rdram, runtime);
    ckEnd(7, ctx, c, 0, 0u, 0.0f, 0x0u, 0x0u);
}
void padSysGet(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t p4)
{
    if (!ckBegin(8, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    ref::padSysGet(rdram, &c, runtime);
    ckMid();
    const uint64_t r = a1_sx64(ty::padSysGet(rdram, runtime, p4));
    ckEnd(8, ctx, c, 1, r, 0.0f, 0x2u, 0x0u);
}
void LoaderSysSetPowerOffCallBackFunc(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t p4, uint32_t p28)
{
    if (!ckBegin(9, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    ref::LoaderSysSetPowerOffCallBackFunc(rdram, &c, runtime);
    ckMid();
    ty::LoaderSysSetPowerOffCallBackFunc(rdram, runtime, p4, p28);
    ckEnd(9, ctx, c, 0, 0u, 0.0f, 0x0u, 0x0u);
}
void SetLocate(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t p4, uint32_t p5, uint32_t p28)
{
    if (!ckBegin(10, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    ref::SetLocate(rdram, &c, runtime);
    ckMid();
    ty::SetLocate(rdram, runtime, p4, p5, p28);
    ckEnd(10, ctx, c, 0, 0u, 0.0f, 0x0u, 0x0u);
}
void PutChar(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t p4, uint32_t p5, uint32_t p28)
{
    if (!ckBegin(11, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    ref::PutChar(rdram, &c, runtime);
    ckMid();
    const uint64_t r = a1_sx64(ty::PutChar(rdram, runtime, p4, p5, p28));
    ckEnd(11, ctx, c, 1, r, 0.0f, 0x4au, 0x0u);
}
void sub_00107D90(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint64_t p4)
{
    if (!ckBegin(12, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    ref::sub_00107D90(rdram, &c, runtime);
    ckMid();
    const uint64_t r = (ty::sub_00107D90(rdram, runtime, p4));
    ckEnd(12, ctx, c, 2, r, 0.0f, 0x78u, 0x0u);
}
void sub_00109268(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t p4)
{
    if (!ckBegin(13, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    ref::sub_00109268(rdram, &c, runtime);
    ckMid();
    const uint64_t r = a1_sx64(ty::sub_00109268(rdram, runtime, p4));
    ckEnd(13, ctx, c, 1, r, 0.0f, 0x8u, 0x0u);
}
void ps2___sceKernlGetEhSemaId(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (!ckBegin(14, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    ref::ps2___sceKernlGetEhSemaId(rdram, &c, runtime);
    ckMid();
    const uint64_t r = a1_sx64(ty::ps2___sceKernlGetEhSemaId(rdram, runtime));
    ckEnd(14, ctx, c, 1, r, 0.0f, 0x8u, 0x0u);
}
void GetTimerPreScaleFactor(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (!ckBegin(15, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    ref::GetTimerPreScaleFactor(rdram, &c, runtime);
    ckMid();
    const uint64_t r = a1_sx64(ty::GetTimerPreScaleFactor(rdram, runtime));
    ckEnd(15, ctx, c, 1, r, 0.0f, 0x8u, 0x0u);
}
void sub_00110998(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t p4)
{
    if (!ckBegin(16, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    ref::sub_00110998(rdram, &c, runtime);
    ckMid();
    const uint64_t r = a1_sx64(ty::sub_00110998(rdram, runtime, p4));
    ckEnd(16, ctx, c, 1, r, 0.0f, 0x8u, 0x0u);
}
void iGetTimerUsedUnusedCounters(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint64_t p4, uint64_t p5)
{
    if (!ckBegin(17, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    ref::iGetTimerUsedUnusedCounters(rdram, &c, runtime);
    ckMid();
    const uint64_t r = a1_sx64(ty::iGetTimerUsedUnusedCounters(rdram, runtime, p4, p5));
    ckEnd(17, ctx, c, 1, r, 0.0f, 0x8u, 0x0u);
}
void sceSifRemoveRebootNotifyHandler(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint64_t p4)
{
    if (!ckBegin(18, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    ref::sceSifRemoveRebootNotifyHandler(rdram, &c, runtime);
    ckMid();
    const uint64_t r = a1_sx64(ty::sceSifRemoveRebootNotifyHandler(rdram, runtime, p4));
    ckEnd(18, ctx, c, 1, r, 0.0f, 0x38u, 0x0u);
}
void ioctl(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (!ckBegin(19, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    ref::ioctl(rdram, &c, runtime);
    ckMid();
    const uint64_t r = a1_sx64(ty::ioctl(rdram, runtime));
    ckEnd(19, ctx, c, 1, r, 0.0f, 0x0u, 0x0u);
}
void isatty(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (!ckBegin(20, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    ref::isatty(rdram, &c, runtime);
    ckMid();
    const uint64_t r = a1_sx64(ty::isatty(rdram, runtime));
    ckEnd(20, ctx, c, 1, r, 0.0f, 0x0u, 0x0u);
}
void sub_00112358(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t p4)
{
    if (!ckBegin(21, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    ref::sub_00112358(rdram, &c, runtime);
    ckMid();
    const uint64_t r = a1_sx64(ty::sub_00112358(rdram, runtime, p4));
    ckEnd(21, ctx, c, 1, r, 0.0f, 0x18u, 0x0u);
}
void sub_00112380(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint64_t p4)
{
    if (!ckBegin(22, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    ref::sub_00112380(rdram, &c, runtime);
    ckMid();
    const uint64_t r = a1_sx64(ty::sub_00112380(rdram, runtime, p4));
    ckEnd(22, ctx, c, 1, r, 0.0f, 0x38u, 0x0u);
}
void sub_001123C0(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint64_t p4)
{
    if (!ckBegin(23, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    ref::sub_001123C0(rdram, &c, runtime);
    ckMid();
    const uint64_t r = a1_sx64(ty::sub_001123C0(rdram, runtime, p4));
    ckEnd(23, ctx, c, 1, r, 0.0f, 0x38u, 0x0u);
}
void sub_001128A0(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t p4, uint32_t p5)
{
    if (!ckBegin(24, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    ref::sub_001128A0(rdram, &c, runtime);
    ckMid();
    const uint64_t r = a1_sx64(ty::sub_001128A0(rdram, runtime, p4, p5));
    ckEnd(24, ctx, c, 1, r, 0.0f, 0x48u, 0x0u);
}
void sub_001128C0(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t p4, uint32_t p5)
{
    if (!ckBegin(25, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    ref::sub_001128C0(rdram, &c, runtime);
    ckMid();
    const uint64_t r = a1_sx64(ty::sub_001128C0(rdram, runtime, p4, p5));
    ckEnd(25, ctx, c, 1, r, 0.0f, 0x0u, 0x0u);
}
void sub_001148B8(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint64_t p4)
{
    if (!ckBegin(26, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    ref::sub_001148B8(rdram, &c, runtime);
    ckMid();
    const uint64_t r = (ty::sub_001148B8(rdram, runtime, p4));
    ckEnd(26, ctx, c, 2, r, 0.0f, 0x18u, 0x0u);
}
void _sceCdSC(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t p5, uint64_t p4)
{
    if (!ckBegin(27, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    ref::_sceCdSC(rdram, &c, runtime);
    ckMid();
    const uint64_t r = (ty::_sceCdSC(rdram, runtime, p5, p4));
    ckEnd(27, ctx, c, 2, r, 0.0f, 0x0u, 0x0u);
}
void sub_00119AA0(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t p4)
{
    if (!ckBegin(28, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    ref::sub_00119AA0(rdram, &c, runtime);
    ckMid();
    const uint64_t r = a1_sx64(ty::sub_00119AA0(rdram, runtime, p4));
    ckEnd(28, ctx, c, 1, r, 0.0f, 0x8u, 0x0u);
}
void ps2__SceMcComGetSysInfo(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t p4)
{
    if (!ckBegin(29, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    ref::ps2__SceMcComGetSysInfo(rdram, &c, runtime);
    ckMid();
    const uint64_t r = a1_sx64(ty::ps2__SceMcComGetSysInfo(rdram, runtime, p4));
    ckEnd(29, ctx, c, 1, r, 0.0f, 0x18u, 0x0u);
}
void sceNetcnfifSetFNoDecode(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t p4)
{
    if (!ckBegin(30, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    ref::sceNetcnfifSetFNoDecode(rdram, &c, runtime);
    ckMid();
    const uint64_t r = (ty::sceNetcnfifSetFNoDecode(rdram, runtime, p4));
    ckEnd(30, ctx, c, 2, r, 0.0f, 0x8u, 0x0u);
}
void ps2___unpack_f(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t p4, uint64_t p5)
{
    if (!ckBegin(31, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    ref::ps2___unpack_f(rdram, &c, runtime);
    ckMid();
    const uint64_t r = a1_sx64(ty::ps2___unpack_f(rdram, runtime, p4, p5));
    ckEnd(31, ctx, c, 1, r, 0.0f, 0x78u, 0x0u);
}
void ps2___pack_d(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint64_t p4, uint64_t p6)
{
    if (!ckBegin(32, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    ref::ps2___pack_d(rdram, &c, runtime);
    ckMid();
    const uint64_t r = (ty::ps2___pack_d(rdram, runtime, p4, p6));
    ckEnd(32, ctx, c, 2, r, 0.0f, 0x1f8u, 0x0u);
}
void ps2___pack_f(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint64_t p4, uint64_t p6)
{
    if (!ckBegin(33, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    ref::ps2___pack_f(rdram, &c, runtime);
    ckMid();
    const a1::RetVF r = ty::ps2___pack_f(rdram, runtime, p4, p6);
    ckEnd(33, ctx, c, 4, r.v0, r.f0, 0x1f8u, 0x0u);
}
void ps2___errno(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (!ckBegin(34, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    ref::ps2___errno(rdram, &c, runtime);
    ckMid();
    const uint64_t r = a1_sx64(ty::ps2___errno(rdram, runtime));
    ckEnd(34, ctx, c, 1, r, 0.0f, 0x8u, 0x0u);
}
void sub_00128B00(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (!ckBegin(35, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    ref::sub_00128B00(rdram, &c, runtime);
    ckMid();
    const uint64_t r = (ty::sub_00128B00(rdram, runtime));
    ckEnd(35, ctx, c, 2, r, 0.0f, 0x0u, 0x0u);
}
void sub_0012D7F0(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t p4, uint32_t p5, uint32_t p6, uint32_t p7)
{
    if (!ckBegin(36, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    ref::sub_0012D7F0(rdram, &c, runtime);
    ckMid();
    const uint64_t r = a1_sx64(ty::sub_0012D7F0(rdram, runtime, p4, p5, p6, p7));
    ckEnd(36, ctx, c, 1, r, 0.0f, 0x308u, 0x0u);
}
void _localeconv_r(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (!ckBegin(37, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    ref::_localeconv_r(rdram, &c, runtime);
    ckMid();
    const uint64_t r = a1_sx64(ty::_localeconv_r(rdram, runtime));
    ckEnd(37, ctx, c, 1, r, 0.0f, 0x0u, 0x0u);
}
void _hi0bits(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint64_t p4)
{
    if (!ckBegin(38, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    ref::_hi0bits(rdram, &c, runtime);
    ckMid();
    const uint64_t r = (ty::_hi0bits(rdram, runtime, p4));
    ckEnd(38, ctx, c, 2, r, 0.0f, 0x38u, 0x0u);
}
void _lo0bits(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t p4)
{
    if (!ckBegin(39, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    ref::_lo0bits(rdram, &c, runtime);
    ckMid();
    const uint64_t r = a1_sx64(ty::_lo0bits(rdram, runtime, p4));
    ckEnd(39, ctx, c, 1, r, 0.0f, 0x28u, 0x0u);
}
void isnan(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint64_t p4)
{
    if (!ckBegin(40, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    ref::isnan(rdram, &c, runtime);
    ckMid();
    const uint64_t r = (ty::isnan(rdram, runtime, p4));
    ckEnd(40, ctx, c, 2, r, 0.0f, 0x78u, 0x0u);
}
void iosCdStStop(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (!ckBegin(41, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    ref::iosCdStStop(rdram, &c, runtime);
    ckMid();
    ty::iosCdStStop(rdram, runtime);
    ckEnd(41, ctx, c, 0, 0u, 0.0f, 0x0u, 0x0u);
}
void iosCdStSkip(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint64_t p4)
{
    if (!ckBegin(42, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    ref::iosCdStSkip(rdram, &c, runtime);
    ckMid();
    const uint64_t r = (ty::iosCdStSkip(rdram, runtime, p4));
    ckEnd(42, ctx, c, 2, r, 0.0f, 0x18u, 0x0u);
}
void iosCdSetErrorCallback(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t p4, uint32_t p5)
{
    if (!ckBegin(43, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    ref::iosCdSetErrorCallback(rdram, &c, runtime);
    ckMid();
    const uint64_t r = a1_sx64(ty::iosCdSetErrorCallback(rdram, runtime, p4, p5));
    ckEnd(43, ctx, c, 1, r, 0.0f, 0x8u, 0x0u);
}
void iosCdSetMode(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint64_t p4)
{
    if (!ckBegin(44, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    ref::iosCdSetMode(rdram, &c, runtime);
    ckMid();
    const uint64_t r = a1_sx64(ty::iosCdSetMode(rdram, runtime, p4));
    ckEnd(44, ctx, c, 1, r, 0.0f, 0x28u, 0x0u);
}
void iosInitDebPc(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (!ckBegin(45, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    ref::iosInitDebPc(rdram, &c, runtime);
    ckMid();
    ty::iosInitDebPc(rdram, runtime);
    ckEnd(45, ctx, c, 0, 0u, 0.0f, 0x0u, 0x0u);
}
void iosStopPc(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (!ckBegin(46, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    ref::iosStopPc(rdram, &c, runtime);
    ckMid();
    ty::iosStopPc(rdram, runtime);
    ckEnd(46, ctx, c, 0, 0u, 0.0f, 0x0u, 0x0u);
}
void iosDebPcVsyncCounterCheck(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (!ckBegin(47, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    ref::iosDebPcVsyncCounterCheck(rdram, &c, runtime);
    ckMid();
    ty::iosDebPcVsyncCounterCheck(rdram, runtime);
    ckEnd(47, ctx, c, 0, 0u, 0.0f, 0x0u, 0x0u);
}
void iosGetCnt0Pc(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (!ckBegin(48, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    ref::iosGetCnt0Pc(rdram, &c, runtime);
    ckMid();
    const uint64_t r = (ty::iosGetCnt0Pc(rdram, runtime));
    ckEnd(48, ctx, c, 2, r, 0.0f, 0x0u, 0x0u);
}
void iosGetCnt1Pc(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (!ckBegin(49, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    ref::iosGetCnt1Pc(rdram, &c, runtime);
    ckMid();
    const uint64_t r = (ty::iosGetCnt1Pc(rdram, runtime));
    ckEnd(49, ctx, c, 2, r, 0.0f, 0x0u, 0x0u);
}
void InitDebPc(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (!ckBegin(50, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    ref::InitDebPc(rdram, &c, runtime);
    ckMid();
    ty::InitDebPc(rdram, runtime);
    ckEnd(50, ctx, c, 0, 0u, 0.0f, 0x0u, 0x0u);
}
void iosGetClock(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t p4)
{
    if (!ckBegin(51, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    ref::iosGetClock(rdram, &c, runtime);
    ckMid();
    const uint64_t r = a1_sx64(ty::iosGetClock(rdram, runtime, p4));
    ckEnd(51, ctx, c, 1, r, 0.0f, 0x8u, 0x0u);
}
void iosEE2IopEndChk(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (!ckBegin(52, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    ref::iosEE2IopEndChk(rdram, &c, runtime);
    ckMid();
    const uint64_t r = a1_sx64(ty::iosEE2IopEndChk(rdram, runtime));
    ckEnd(52, ctx, c, 1, r, 0.0f, 0x0u, 0x0u);
}
void iosCreateIopMgr(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (!ckBegin(53, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    ref::iosCreateIopMgr(rdram, &c, runtime);
    ckMid();
    ty::iosCreateIopMgr(rdram, runtime);
    ckEnd(53, ctx, c, 0, 0u, 0.0f, 0x0u, 0x0u);
}
void iosGetMallocPtn(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t p4)
{
    if (!ckBegin(54, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    ref::iosGetMallocPtn(rdram, &c, runtime);
    ckMid();
    const uint64_t r = a1_sx64(ty::iosGetMallocPtn(rdram, runtime, p4));
    ckEnd(54, ctx, c, 1, r, 0.0f, 0x38u, 0x0u);
}
void iosGetMallocRootPtnRemainSize(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (!ckBegin(55, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    ref::iosGetMallocRootPtnRemainSize(rdram, &c, runtime);
    ckMid();
    const uint64_t r = a1_sx64(ty::iosGetMallocRootPtnRemainSize(rdram, runtime));
    ckEnd(55, ctx, c, 1, r, 0.0f, 0x8u, 0x0u);
}
void addMemoryDebugCallback(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t p4)
{
    if (!ckBegin(56, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    ref::addMemoryDebugCallback(rdram, &c, runtime);
    ckMid();
    const uint64_t r = a1_sx64(ty::addMemoryDebugCallback(rdram, runtime, p4));
    ckEnd(56, ctx, c, 1, r, 0.0f, 0x0u, 0x0u);
}
void getMemorySafetyLockFlag(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (!ckBegin(57, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    ref::getMemorySafetyLockFlag(rdram, &c, runtime);
    ckMid();
    const uint64_t r = a1_sx64(ty::getMemorySafetyLockFlag(rdram, runtime));
    ckEnd(57, ctx, c, 1, r, 0.0f, 0x0u, 0x0u);
}
void kernel_sub_001B06C8(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (!ckBegin(58, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    ref::kernel_sub_001B06C8(rdram, &c, runtime);
    ckMid();
    ty::kernel_sub_001B06C8(rdram, runtime);
    ckEnd(58, ctx, c, 0, 0u, 0.0f, 0x0u, 0x0u);
}
void iosInitMessageSystem(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (!ckBegin(59, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    ref::iosInitMessageSystem(rdram, &c, runtime);
    ckMid();
    ty::iosInitMessageSystem(rdram, runtime);
    ckEnd(59, ctx, c, 0, 0u, 0.0f, 0x0u, 0x0u);
}
void iosPadGetXZInputLwithThresholdIndepAxis(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t p4, uint32_t p5, float pf12)
{
    if (!ckBegin(60, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    ref::iosPadGetXZInputLwithThresholdIndepAxis(rdram, &c, runtime);
    ckMid();
    const a1::RetVF r = ty::iosPadGetXZInputLwithThresholdIndepAxis(rdram, runtime, p4, p5, pf12);
    ckEnd(60, ctx, c, 4, r.v0, r.f0, 0x2u, 0x1eu);
}
void iosPadEntryTickFunc(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t p4)
{
    if (!ckBegin(61, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    ref::iosPadEntryTickFunc(rdram, &c, runtime);
    ckMid();
    const uint64_t r = a1_sx64(ty::iosPadEntryTickFunc(rdram, runtime, p4));
    ckEnd(61, ctx, c, 1, r, 0.0f, 0x0u, 0x0u);
}
void iosCreatePrintMgr(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (!ckBegin(62, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    ref::iosCreatePrintMgr(rdram, &c, runtime);
    ckMid();
    ty::iosCreatePrintMgr(rdram, runtime);
    ckEnd(62, ctx, c, 0, 0u, 0.0f, 0x0u, 0x0u);
}
void iosGetPrintfMode(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (!ckBegin(63, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    ref::iosGetPrintfMode(rdram, &c, runtime);
    ckMid();
    const uint64_t r = a1_sx64(ty::iosGetPrintfMode(rdram, runtime));
    ckEnd(63, ctx, c, 1, r, 0.0f, 0x0u, 0x0u);
}
void endSCEDemo(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (!ckBegin(64, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    ref::endSCEDemo(rdram, &c, runtime);
    ckMid();
    ty::endSCEDemo(rdram, runtime);
    ckEnd(64, ctx, c, 0, 0u, 0.0f, 0x0u, 0x0u);
}
void Init_ShockVoiceSet(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t p4, uint32_t p5)
{
    if (!ckBegin(65, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    ref::Init_ShockVoiceSet(rdram, &c, runtime);
    ckMid();
    const uint64_t r = a1_sx64(ty::Init_ShockVoiceSet(rdram, runtime, p4, p5));
    ckEnd(65, ctx, c, 1, r, 0.0f, 0x28u, 0x0u);
}
void Shock_SetShockVoiceSet(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t p5, uint64_t p4)
{
    if (!ckBegin(66, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    ref::Shock_SetShockVoiceSet(rdram, &c, runtime);
    ckMid();
    const uint64_t r = (ty::Shock_SetShockVoiceSet(rdram, runtime, p5, p4));
    ckEnd(66, ctx, c, 2, r, 0.0f, 0xc8u, 0x0u);
}
void Init_Player(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t p4)
{
    if (!ckBegin(67, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    ref::Init_Player(rdram, &c, runtime);
    ckMid();
    const uint64_t r = a1_sx64(ty::Init_Player(rdram, runtime, p4));
    ckEnd(67, ctx, c, 1, r, 0.0f, 0x68u, 0x0u);
}
void Init_Controler(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t p4)
{
    if (!ckBegin(68, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    ref::Init_Controler(rdram, &c, runtime);
    ckMid();
    ty::Init_Controler(rdram, runtime, p4);
    ckEnd(68, ctx, c, 0, 0u, 0.0f, 0x0u, 0x0u);
}
void kernel_sub_001B40C8(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (!ckBegin(69, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    ref::kernel_sub_001B40C8(rdram, &c, runtime);
    ckMid();
    const uint64_t r = (ty::kernel_sub_001B40C8(rdram, runtime));
    ckEnd(69, ctx, c, 2, r, 0.0f, 0x0u, 0x0u);
}
void kernel_sub_001B4118(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t p4)
{
    if (!ckBegin(70, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    ref::kernel_sub_001B4118(rdram, &c, runtime);
    ckMid();
    ty::kernel_sub_001B4118(rdram, runtime, p4);
    ckEnd(70, ctx, c, 0, 0u, 0.0f, 0x0u, 0x0u);
}
void IosGetMergeDataFileName(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (!ckBegin(71, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    ref::IosGetMergeDataFileName(rdram, &c, runtime);
    ckMid();
    const uint64_t r = a1_sx64(ty::IosGetMergeDataFileName(rdram, runtime));
    ckEnd(71, ctx, c, 1, r, 0.0f, 0x0u, 0x0u);
}
void GetSymbolByIndex(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t p4, uint32_t p5)
{
    if (!ckBegin(72, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    ref::GetSymbolByIndex(rdram, &c, runtime);
    ckMid();
    const uint64_t r = a1_sx64(ty::GetSymbolByIndex(rdram, runtime, p4, p5));
    ckEnd(72, ctx, c, 1, r, 0.0f, 0x78u, 0x0u);
}
void isysGetXffWorkSize(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t p4)
{
    if (!ckBegin(73, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    ref::isysGetXffWorkSize(rdram, &c, runtime);
    ckMid();
    const uint64_t r = a1_sx64(ty::isysGetXffWorkSize(rdram, runtime, p4));
    ckEnd(73, ctx, c, 1, r, 0.0f, 0x78u, 0x0u);
}
void isysGetModuleObjMallocSize(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t p4)
{
    if (!ckBegin(74, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    ref::isysGetModuleObjMallocSize(rdram, &c, runtime);
    ckMid();
    const uint64_t r = a1_sx64(ty::isysGetModuleObjMallocSize(rdram, runtime, p4));
    ckEnd(74, ctx, c, 1, r, 0.0f, 0x18u, 0x0u);
}
void GetExecModuleSize(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (!ckBegin(75, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    ref::GetExecModuleSize(rdram, &c, runtime);
    ckMid();
    const uint64_t r = a1_sx64(ty::GetExecModuleSize(rdram, runtime));
    ckEnd(75, ctx, c, 1, r, 0.0f, 0x8u, 0x0u);
}
void getModuleSize(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t p4)
{
    if (!ckBegin(76, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    ref::getModuleSize(rdram, &c, runtime);
    ckMid();
    const uint64_t r = a1_sx64(ty::getModuleSize(rdram, runtime, p4));
    ckEnd(76, ctx, c, 1, r, 0.0f, 0x8u, 0x0u);
}
void isysCreateExceptionThread(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (!ckBegin(77, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    ref::isysCreateExceptionThread(rdram, &c, runtime);
    ckMid();
    ty::isysCreateExceptionThread(rdram, runtime);
    ckEnd(77, ctx, c, 0, 0u, 0.0f, 0x0u, 0x0u);
}
void iosCubicBezier(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t p4, float pf12)
{
    if (!ckBegin(78, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    ref::iosCubicBezier(rdram, &c, runtime);
    ckMid();
    const float r = ty::iosCubicBezier(rdram, runtime, p4, pf12);
    ckEnd(78, ctx, c, 3, 0u, r, 0x2u, 0x3eu);
}
void iosFourthBezier(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t p4, float pf12)
{
    if (!ckBegin(79, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    ref::iosFourthBezier(rdram, &c, runtime);
    ckMid();
    const float r = ty::iosFourthBezier(rdram, runtime, p4, pf12);
    ckEnd(79, ctx, c, 3, 0u, r, 0x2u, 0x3feu);
}
void iosFloatModulo(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, float pf12, float pf13)
{
    if (!ckBegin(80, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    ref::iosFloatModulo(rdram, &c, runtime);
    ckMid();
    const float r = ty::iosFloatModulo(rdram, runtime, pf12, pf13);
    ckEnd(80, ctx, c, 3, 0u, r, 0x4u, 0x2u);
}
void iosIColToMvecSl(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t p4, uint32_t p5)
{
    if (!ckBegin(81, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    ref::iosIColToMvecSl(rdram, &c, runtime);
    ckMid();
    const uint64_t r = a1_sx64(ty::iosIColToMvecSl(rdram, runtime, p4, p5));
    ckEnd(81, ctx, c, 1, r, 0.0f, 0x68u, 0x0u);
}
void GetCurrentQuaternion(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (!ckBegin(82, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    ref::GetCurrentQuaternion(rdram, &c, runtime);
    ckMid();
    const uint64_t r = a1_sx64(ty::GetCurrentQuaternion(rdram, runtime));
    ckEnd(82, ctx, c, 1, r, 0.0f, 0xau, 0x0u);
}
void GetLastQuaternion(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (!ckBegin(83, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    ref::GetLastQuaternion(rdram, &c, runtime);
    ckMid();
    const uint64_t r = a1_sx64(ty::GetLastQuaternion(rdram, runtime));
    ckEnd(83, ctx, c, 1, r, 0.0f, 0x18u, 0x0u);
}
void iosGetACos(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t p4)
{
    if (!ckBegin(84, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    ref::iosGetACos(rdram, &c, runtime);
    ckMid();
    const float r = ty::iosGetACos(rdram, runtime, p4);
    ckEnd(84, ctx, c, 3, 0u, r, 0x2u, 0xfeu);
}
void iosGetASin(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (!ckBegin(85, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    ref::iosGetASin(rdram, &c, runtime);
    ckMid();
    const float r = ty::iosGetASin(rdram, runtime);
    ckEnd(85, ctx, c, 3, 0u, r, 0x0u, 0x0u);
}
void iosGetATan2(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (!ckBegin(86, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    ref::iosGetATan2(rdram, &c, runtime);
    ckMid();
    const float r = ty::iosGetATan2(rdram, runtime);
    ckEnd(86, ctx, c, 3, 0u, r, 0x0u, 0x0u);
}
void resetArcFuncCall(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (!ckBegin(87, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    ref::resetArcFuncCall(rdram, &c, runtime);
    ckMid();
    const uint64_t r = a1_sx64(ty::resetArcFuncCall(rdram, runtime));
    ckEnd(87, ctx, c, 1, r, 0.0f, 0x38u, 0x0u);
}
void iosGetTableSin(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t p4)
{
    if (!ckBegin(88, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    ref::iosGetTableSin(rdram, &c, runtime);
    ckMid();
    const a1::RetVF r = ty::iosGetTableSin(rdram, runtime, p4);
    ckEnd(88, ctx, c, 4, r.v0, r.f0, 0x1au, 0x0u);
}
void iosDumpFVector(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (!ckBegin(89, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    ref::iosDumpFVector(rdram, &c, runtime);
    ckMid();
    ty::iosDumpFVector(rdram, runtime);
    ckEnd(89, ctx, c, 0, 0u, 0.0f, 0x0u, 0x0u);
}
void iosDumpIVector(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (!ckBegin(90, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    ref::iosDumpIVector(rdram, &c, runtime);
    ckMid();
    ty::iosDumpIVector(rdram, runtime);
    ckEnd(90, ctx, c, 0, 0u, 0.0f, 0x0u, 0x0u);
}
void iosGetScaleInverseVector(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t p4, uint32_t p5)
{
    if (!ckBegin(91, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    ref::iosGetScaleInverseVector(rdram, &c, runtime);
    ckMid();
    const float r = ty::iosGetScaleInverseVector(rdram, runtime, p4, p5);
    ckEnd(91, ctx, c, 3, 0u, r, 0x2u, 0x6u);
}
void iosSetDCViewPort(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint64_t p4, uint64_t p5, uint64_t p6, uint64_t p7, uint64_t p8)
{
    if (!ckBegin(92, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    ref::iosSetDCViewPort(rdram, &c, runtime);
    ckMid();
    const uint64_t r = (ty::iosSetDCViewPort(rdram, runtime, p4, p5, p6, p7, p8));
    ckEnd(92, ctx, c, 2, r, 0.0f, 0x1e08u, 0x0u);
}
void iosGetRootDC(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (!ckBegin(93, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    ref::iosGetRootDC(rdram, &c, runtime);
    ckMid();
    const uint64_t r = a1_sx64(ty::iosGetRootDC(rdram, runtime));
    ckEnd(93, ctx, c, 1, r, 0.0f, 0x8u, 0x0u);
}
void iosGetCurrentDC(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (!ckBegin(94, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    ref::iosGetCurrentDC(rdram, &c, runtime);
    ckMid();
    const uint64_t r = a1_sx64(ty::iosGetCurrentDC(rdram, runtime));
    ckEnd(94, ctx, c, 1, r, 0.0f, 0x8u, 0x0u);
}
void iosGetDCGroup(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (!ckBegin(95, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    ref::iosGetDCGroup(rdram, &c, runtime);
    ckMid();
    const uint64_t r = a1_sx64(ty::iosGetDCGroup(rdram, runtime));
    ckEnd(95, ctx, c, 1, r, 0.0f, 0x0u, 0x0u);
}
void kernel_sub_001BF430(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (!ckBegin(96, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    ref::kernel_sub_001BF430(rdram, &c, runtime);
    ckMid();
    ty::kernel_sub_001BF430(rdram, runtime);
    ckEnd(96, ctx, c, 0, 0u, 0.0f, 0x0u, 0x0u);
}
void kernel_sub_001BF438(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (!ckBegin(97, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    ref::kernel_sub_001BF438(rdram, &c, runtime);
    ckMid();
    ty::kernel_sub_001BF438(rdram, runtime);
    ckEnd(97, ctx, c, 0, 0u, 0.0f, 0x0u, 0x0u);
}
void iosDlGetTransAdrs(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (!ckBegin(98, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    ref::iosDlGetTransAdrs(rdram, &c, runtime);
    ckMid();
    const uint64_t r = a1_sx64(ty::iosDlGetTransAdrs(rdram, runtime));
    ckEnd(98, ctx, c, 1, r, 0.0f, 0x8u, 0x0u);
}
void iosGetPacketPtrPath1(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (!ckBegin(99, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    ref::iosGetPacketPtrPath1(rdram, &c, runtime);
    ckMid();
    const uint64_t r = a1_sx64(ty::iosGetPacketPtrPath1(rdram, runtime));
    ckEnd(99, ctx, c, 1, r, 0.0f, 0x38u, 0x0u);
}
void iosSetPacketPtrPath1(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t p4)
{
    if (!ckBegin(100, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    ref::iosSetPacketPtrPath1(rdram, &c, runtime);
    ckMid();
    const uint64_t r = a1_sx64(ty::iosSetPacketPtrPath1(rdram, runtime, p4));
    ckEnd(100, ctx, c, 1, r, 0.0f, 0x0u, 0x0u);
}
void iosDlChainHead(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint64_t p4, uint64_t p5)
{
    if (!ckBegin(101, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    ref::iosDlChainHead(rdram, &c, runtime);
    ckMid();
    const uint64_t r = (ty::iosDlChainHead(rdram, runtime, p4, p5));
    ckEnd(101, ctx, c, 2, r, 0.0f, 0x3f8u, 0x0u);
}
void iosDlChainHeadCurrent(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint64_t p4)
{
    if (!ckBegin(102, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    ref::iosDlChainHeadCurrent(rdram, &c, runtime);
    ckMid();
    const uint64_t r = (ty::iosDlChainHeadCurrent(rdram, runtime, p4));
    ckEnd(102, ctx, c, 2, r, 0.0f, 0x3f8u, 0x0u);
}
void iosDlGetWorkBufId(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (!ckBegin(103, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    ref::iosDlGetWorkBufId(rdram, &c, runtime);
    ckMid();
    const uint64_t r = a1_sx64(ty::iosDlGetWorkBufId(rdram, runtime));
    ckEnd(103, ctx, c, 1, r, 0.0f, 0x8u, 0x0u);
}
void iosDlGetTransBufId(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (!ckBegin(104, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    ref::iosDlGetTransBufId(rdram, &c, runtime);
    ckMid();
    const uint64_t r = a1_sx64(ty::iosDlGetTransBufId(rdram, runtime));
    ckEnd(104, ctx, c, 1, r, 0.0f, 0x8u, 0x0u);
}
void iosDlGetEndAdrs(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (!ckBegin(105, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    ref::iosDlGetEndAdrs(rdram, &c, runtime);
    ckMid();
    const uint64_t r = a1_sx64(ty::iosDlGetEndAdrs(rdram, runtime));
    ckEnd(105, ctx, c, 1, r, 0.0f, 0x38u, 0x0u);
}
void iosDlGetLayerNum(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (!ckBegin(106, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    ref::iosDlGetLayerNum(rdram, &c, runtime);
    ckMid();
    const uint64_t r = a1_sx64(ty::iosDlGetLayerNum(rdram, runtime));
    ckEnd(106, ctx, c, 1, r, 0.0f, 0x8u, 0x0u);
}
void iosDlCheckAdrs(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint64_t p4)
{
    if (!ckBegin(107, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    ref::iosDlCheckAdrs(rdram, &c, runtime);
    ckMid();
    const uint64_t r = a1_sx64(ty::iosDlCheckAdrs(rdram, runtime, p4));
    ckEnd(107, ctx, c, 1, r, 0.0f, 0x68u, 0x0u);
}
void iosDlCheckSize(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (!ckBegin(108, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    ref::iosDlCheckSize(rdram, &c, runtime);
    ckMid();
    const uint64_t r = a1_sx64(ty::iosDlCheckSize(rdram, runtime));
    ckEnd(108, ctx, c, 1, r, 0.0f, 0xf8u, 0x0u);
}
void iosAllocPacketPtrPath1UnCachedA(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t p4)
{
    if (!ckBegin(109, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    ref::iosAllocPacketPtrPath1UnCachedA(rdram, &c, runtime);
    ckMid();
    const uint64_t r = a1_sx64(ty::iosAllocPacketPtrPath1UnCachedA(rdram, runtime, p4));
    ckEnd(109, ctx, c, 1, r, 0.0f, 0x68u, 0x0u);
}
void getDispScreenWidth(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (!ckBegin(110, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    ref::getDispScreenWidth(rdram, &c, runtime);
    ckMid();
    const uint64_t r = a1_sx64(ty::getDispScreenWidth(rdram, runtime));
    ckEnd(110, ctx, c, 1, r, 0.0f, 0x8u, 0x0u);
}
void getDispScreenHeight(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (!ckBegin(111, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    ref::getDispScreenHeight(rdram, &c, runtime);
    ckMid();
    const uint64_t r = a1_sx64(ty::getDispScreenHeight(rdram, runtime));
    ckEnd(111, ctx, c, 1, r, 0.0f, 0x8u, 0x0u);
}
void getDrawScreenWidth(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (!ckBegin(112, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    ref::getDrawScreenWidth(rdram, &c, runtime);
    ckMid();
    const uint64_t r = a1_sx64(ty::getDrawScreenWidth(rdram, runtime));
    ckEnd(112, ctx, c, 1, r, 0.0f, 0x8u, 0x0u);
}
void getDrawScreenHeight(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (!ckBegin(113, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    ref::getDrawScreenHeight(rdram, &c, runtime);
    ckMid();
    const uint64_t r = a1_sx64(ty::getDrawScreenHeight(rdram, runtime));
    ckEnd(113, ctx, c, 1, r, 0.0f, 0x8u, 0x0u);
}
void getDispBufferAddressTBP(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (!ckBegin(114, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    ref::getDispBufferAddressTBP(rdram, &c, runtime);
    ckMid();
    const uint64_t r = (ty::getDispBufferAddressTBP(rdram, runtime));
    ckEnd(114, ctx, c, 2, r, 0.0f, 0x0u, 0x0u);
}
void getDispBufferAddressFBP(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (!ckBegin(115, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    ref::getDispBufferAddressFBP(rdram, &c, runtime);
    ckMid();
    const uint64_t r = (ty::getDispBufferAddressFBP(rdram, runtime));
    ckEnd(115, ctx, c, 2, r, 0.0f, 0x0u, 0x0u);
}
void getZBufferAddressTBP(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (!ckBegin(116, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    ref::getZBufferAddressTBP(rdram, &c, runtime);
    ckMid();
    const uint64_t r = a1_sx64(ty::getZBufferAddressTBP(rdram, runtime));
    ckEnd(116, ctx, c, 1, r, 0.0f, 0x18u, 0x0u);
}
void getZBufferAddressFBP(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (!ckBegin(117, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    ref::getZBufferAddressFBP(rdram, &c, runtime);
    ckMid();
    const uint64_t r = a1_sx64(ty::getZBufferAddressFBP(rdram, runtime));
    ckEnd(117, ctx, c, 1, r, 0.0f, 0x18u, 0x0u);
}
void getScreenOffsetX(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (!ckBegin(118, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    ref::getScreenOffsetX(rdram, &c, runtime);
    ckMid();
    const uint64_t r = a1_sx64(ty::getScreenOffsetX(rdram, runtime));
    ckEnd(118, ctx, c, 1, r, 0.0f, 0x0u, 0x0u);
}
void getScreenOffsetY(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (!ckBegin(119, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    ref::getScreenOffsetY(rdram, &c, runtime);
    ckMid();
    const uint64_t r = a1_sx64(ty::getScreenOffsetY(rdram, runtime));
    ckEnd(119, ctx, c, 1, r, 0.0f, 0x0u, 0x0u);
}
void getAntiAlias(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (!ckBegin(120, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    ref::getAntiAlias(rdram, &c, runtime);
    ckMid();
    const uint64_t r = a1_sx64(ty::getAntiAlias(rdram, runtime));
    ckEnd(120, ctx, c, 1, r, 0.0f, 0x0u, 0x0u);
}
void getProgressiveMode(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (!ckBegin(121, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    ref::getProgressiveMode(rdram, &c, runtime);
    ckMid();
    const uint64_t r = a1_sx64(ty::getProgressiveMode(rdram, runtime));
    ckEnd(121, ctx, c, 1, r, 0.0f, 0x0u, 0x0u);
}
void iosGetField(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (!ckBegin(122, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    ref::iosGetField(rdram, &c, runtime);
    ckMid();
    const uint64_t r = (ty::iosGetField(rdram, runtime));
    ckEnd(122, ctx, c, 2, r, 0.0f, 0x8u, 0x0u);
}
void iosSetSyncNoReturnFunc(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t p4)
{
    if (!ckBegin(123, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    ref::iosSetSyncNoReturnFunc(rdram, &c, runtime);
    ckMid();
    const uint64_t r = a1_sx64(ty::iosSetSyncNoReturnFunc(rdram, runtime, p4));
    ckEnd(123, ctx, c, 1, r, 0.0f, 0x8u, 0x0u);
}
void iosWaitDMA(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (!ckBegin(124, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    ref::iosWaitDMA(rdram, &c, runtime);
    ckMid();
    const uint64_t r = (ty::iosWaitDMA(rdram, runtime));
    ckEnd(124, ctx, c, 2, r, 0.0f, 0x0u, 0x0u);
}
void getCRTCoffsetU(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (!ckBegin(125, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    ref::getCRTCoffsetU(rdram, &c, runtime);
    ckMid();
    const uint64_t r = a1_sx64(ty::getCRTCoffsetU(rdram, runtime));
    ckEnd(125, ctx, c, 1, r, 0.0f, 0x0u, 0x0u);
}
void getCRTCoffsetV(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (!ckBegin(126, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    ref::getCRTCoffsetV(rdram, &c, runtime);
    ckMid();
    const uint64_t r = a1_sx64(ty::getCRTCoffsetV(rdram, runtime));
    ckEnd(126, ctx, c, 1, r, 0.0f, 0x0u, 0x0u);
}
void iosSetIStartFunc(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t p4)
{
    if (!ckBegin(127, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    ref::iosSetIStartFunc(rdram, &c, runtime);
    ckMid();
    const uint64_t r = a1_sx64(ty::iosSetIStartFunc(rdram, runtime, p4));
    ckEnd(127, ctx, c, 1, r, 0.0f, 0x8u, 0x0u);
}
void iosSetGpFinishFunc(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t p4)
{
    if (!ckBegin(128, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    ref::iosSetGpFinishFunc(rdram, &c, runtime);
    ckMid();
    const uint64_t r = a1_sx64(ty::iosSetGpFinishFunc(rdram, runtime, p4));
    ckEnd(128, ctx, c, 1, r, 0.0f, 0x8u, 0x0u);
}
void iosSetIFinishFunc(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t p4)
{
    if (!ckBegin(129, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    ref::iosSetIFinishFunc(rdram, &c, runtime);
    ckMid();
    const uint64_t r = a1_sx64(ty::iosSetIFinishFunc(rdram, runtime, p4));
    ckEnd(129, ctx, c, 1, r, 0.0f, 0x8u, 0x0u);
}
void getIntrGpCount(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (!ckBegin(130, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    ref::getIntrGpCount(rdram, &c, runtime);
    ckMid();
    const uint64_t r = (ty::getIntrGpCount(rdram, runtime));
    ckEnd(130, ctx, c, 2, r, 0.0f, 0x8u, 0x0u);
}
void iosSetDlTransFunc(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t p4)
{
    if (!ckBegin(131, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    ref::iosSetDlTransFunc(rdram, &c, runtime);
    ckMid();
    const uint64_t r = a1_sx64(ty::iosSetDlTransFunc(rdram, runtime, p4));
    ckEnd(131, ctx, c, 1, r, 0.0f, 0x8u, 0x0u);
}
void iosSetVSchedulerFunc(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t p4)
{
    if (!ckBegin(132, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    ref::iosSetVSchedulerFunc(rdram, &c, runtime);
    ckMid();
    const uint64_t r = a1_sx64(ty::iosSetVSchedulerFunc(rdram, runtime, p4));
    ckEnd(132, ctx, c, 1, r, 0.0f, 0x8u, 0x0u);
}
void getSpriteDrawFirst(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (!ckBegin(133, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    ref::getSpriteDrawFirst(rdram, &c, runtime);
    ckMid();
    const uint64_t r = a1_sx64(ty::getSpriteDrawFirst(rdram, runtime));
    ckEnd(133, ctx, c, 1, r, 0.0f, 0x0u, 0x0u);
}
void gsReglist(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (!ckBegin(134, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    ref::gsReglist(rdram, &c, runtime);
    ckMid();
    ty::gsReglist(rdram, runtime);
    ckEnd(134, ctx, c, 0, 0u, 0.0f, 0x0u, 0x0u);
}
void isysSetObjImmediateDeleteFunc(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t p4, uint32_t p5)
{
    if (!ckBegin(135, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    ref::isysSetObjImmediateDeleteFunc(rdram, &c, runtime);
    ckMid();
    ty::isysSetObjImmediateDeleteFunc(rdram, runtime, p4, p5);
    ckEnd(135, ctx, c, 0, 0u, 0.0f, 0x0u, 0x0u);
}
void isysInitGroup(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t p4)
{
    if (!ckBegin(136, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    ref::isysInitGroup(rdram, &c, runtime);
    ckMid();
    ty::isysInitGroup(rdram, runtime, p4);
    ckEnd(136, ctx, c, 0, 0u, 0.0f, 0x0u, 0x0u);
}
void isysObjDeleteLock(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t p4)
{
    if (!ckBegin(137, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    ref::isysObjDeleteLock(rdram, &c, runtime);
    ckMid();
    const uint64_t r = a1_sx64(ty::isysObjDeleteLock(rdram, runtime, p4));
    ckEnd(137, ctx, c, 1, r, 0.0f, 0x28u, 0x0u);
}
void isysGetLinkMax(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (!ckBegin(138, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    ref::isysGetLinkMax(rdram, &c, runtime);
    ckMid();
    const uint64_t r = a1_sx64(ty::isysGetLinkMax(rdram, runtime));
    ckEnd(138, ctx, c, 1, r, 0.0f, 0x0u, 0x0u);
}
void isysGetObjIdentifierByObj(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t p4)
{
    if (!ckBegin(139, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    ref::isysGetObjIdentifierByObj(rdram, &c, runtime);
    ckMid();
    const uint64_t r = a1_sx64(ty::isysGetObjIdentifierByObj(rdram, runtime, p4));
    ckEnd(139, ctx, c, 1, r, 0.0f, 0x8u, 0x0u);
}
void iosGetISchedulerExecCnt(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (!ckBegin(140, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    ref::iosGetISchedulerExecCnt(rdram, &c, runtime);
    ckMid();
    const uint64_t r = a1_sx64(ty::iosGetISchedulerExecCnt(rdram, runtime));
    ckEnd(140, ctx, c, 1, r, 0.0f, 0x8u, 0x0u);
}
void ischedRestart(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (!ckBegin(141, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    ref::ischedRestart(rdram, &c, runtime);
    ckMid();
    const uint64_t r = a1_sx64(ty::ischedRestart(rdram, runtime));
    ckEnd(141, ctx, c, 1, r, 0.0f, 0x18u, 0x0u);
}
void iosSetISchedulerSubthreadTask(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t p4)
{
    if (!ckBegin(142, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    ref::iosSetISchedulerSubthreadTask(rdram, &c, runtime);
    ckMid();
    const uint64_t r = a1_sx64(ty::iosSetISchedulerSubthreadTask(rdram, runtime, p4));
    ckEnd(142, ctx, c, 1, r, 0.0f, 0x0u, 0x0u);
}
void iosSetIVSyncMode(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t p4)
{
    if (!ckBegin(143, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    ref::iosSetIVSyncMode(rdram, &c, runtime);
    ckMid();
    const uint64_t r = a1_sx64(ty::iosSetIVSyncMode(rdram, runtime, p4));
    ckEnd(143, ctx, c, 1, r, 0.0f, 0x0u, 0x0u);
}
void iosGetIVSyncMode(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (!ckBegin(144, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    ref::iosGetIVSyncMode(rdram, &c, runtime);
    ckMid();
    const uint64_t r = a1_sx64(ty::iosGetIVSyncMode(rdram, runtime));
    ckEnd(144, ctx, c, 1, r, 0.0f, 0x8u, 0x0u);
}
void vschedSetNextFrameForceGarbage(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (!ckBegin(145, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    ref::vschedSetNextFrameForceGarbage(rdram, &c, runtime);
    ckMid();
    const uint64_t r = a1_sx64(ty::vschedSetNextFrameForceGarbage(rdram, runtime));
    ckEnd(145, ctx, c, 1, r, 0.0f, 0x8u, 0x0u);
}
void GetGarbageTime(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (!ckBegin(146, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    ref::GetGarbageTime(rdram, &c, runtime);
    ckMid();
    const uint64_t r = (ty::GetGarbageTime(rdram, runtime));
    ckEnd(146, ctx, c, 2, r, 0.0f, 0x8u, 0x0u);
}
void init_letter(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t p4, uint32_t p5, uint32_t p6)
{
    if (!ckBegin(147, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    ref::init_letter(rdram, &c, runtime);
    ckMid();
    const uint64_t r = a1_sx64(ty::init_letter(rdram, runtime, p4, p5, p6));
    ckEnd(147, ctx, c, 1, r, 0.0f, 0x0u, 0x0u);
}
void is_hope_letter_type(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t p4, uint64_t p5)
{
    if (!ckBegin(148, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    ref::is_hope_letter_type(rdram, &c, runtime);
    ckMid();
    const uint64_t r = (ty::is_hope_letter_type(rdram, runtime, p4, p5));
    ckEnd(148, ctx, c, 2, r, 0.0f, 0x0u, 0x0u);
}
void get_letter_contents_data(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t p4)
{
    if (!ckBegin(149, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    ref::get_letter_contents_data(rdram, &c, runtime);
    ckMid();
    const uint64_t r = a1_sx64(ty::get_letter_contents_data(rdram, runtime, p4));
    ckEnd(149, ctx, c, 1, r, 0.0f, 0x0u, 0x0u);
}
void increment_letter_refcount(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t p4)
{
    if (!ckBegin(150, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    ref::increment_letter_refcount(rdram, &c, runtime);
    ckMid();
    const uint64_t r = a1_sx64(ty::increment_letter_refcount(rdram, runtime, p4));
    ckEnd(150, ctx, c, 1, r, 0.0f, 0x0u, 0x0u);
}
void is_letter_reach_time(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t p4, float pf12)
{
    if (!ckBegin(151, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    ref::is_letter_reach_time(rdram, &c, runtime);
    ckMid();
    const a1::RetVF r = ty::is_letter_reach_time(rdram, runtime, p4, pf12);
    ckEnd(151, ctx, c, 4, r.v0, r.f0, 0x0u, 0x2u);
}
void is_init_letter_called(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t p4)
{
    if (!ckBegin(152, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    ref::is_init_letter_called(rdram, &c, runtime);
    ckMid();
    const uint64_t r = a1_sx64(ty::is_init_letter_called(rdram, runtime, p4));
    ckEnd(152, ctx, c, 1, r, 0.0f, 0x0u, 0x0u);
}
void get_reference_letter_type(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t p4)
{
    if (!ckBegin(153, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    ref::get_reference_letter_type(rdram, &c, runtime);
    ckMid();
    const uint64_t r = a1_sx64(ty::get_reference_letter_type(rdram, runtime, p4));
    ckEnd(153, ctx, c, 1, r, 0.0f, 0x0u, 0x0u);
}
void get_letter_receiver_obj_group(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t p4)
{
    if (!ckBegin(154, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    ref::get_letter_receiver_obj_group(rdram, &c, runtime);
    ckMid();
    const uint64_t r = a1_sx64(ty::get_letter_receiver_obj_group(rdram, runtime, p4));
    ckEnd(154, ctx, c, 1, r, 0.0f, 0x0u, 0x0u);
}
void get_letter_receiver_data(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t p4)
{
    if (!ckBegin(155, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    ref::get_letter_receiver_data(rdram, &c, runtime);
    ckMid();
    const uint64_t r = a1_sx64(ty::get_letter_receiver_data(rdram, runtime, p4));
    ckEnd(155, ctx, c, 1, r, 0.0f, 0x0u, 0x0u);
}
void get_total_letter_in_post(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t p4)
{
    if (!ckBegin(156, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    ref::get_total_letter_in_post(rdram, &c, runtime);
    ckMid();
    const uint64_t r = a1_sx64(ty::get_total_letter_in_post(rdram, runtime, p4));
    ckEnd(156, ctx, c, 1, r, 0.0f, 0x0u, 0x0u);
}
void kernel_sub_001C8948(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t p4)
{
    if (!ckBegin(157, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    ref::kernel_sub_001C8948(rdram, &c, runtime);
    ckMid();
    ty::kernel_sub_001C8948(rdram, runtime, p4);
    ckEnd(157, ctx, c, 0, 0u, 0.0f, 0x0u, 0x0u);
}
void constructor_letter_type(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t p4)
{
    if (!ckBegin(158, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    ref::constructor_letter_type(rdram, &c, runtime);
    ckMid();
    const uint64_t r = a1_sx64(ty::constructor_letter_type(rdram, runtime, p4));
    ckEnd(158, ctx, c, 1, r, 0.0f, 0x0u, 0x0u);
}
void get_letter_type(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint64_t p4)
{
    if (!ckBegin(159, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    ref::get_letter_type(rdram, &c, runtime);
    ckMid();
    const uint64_t r = a1_sx64(ty::get_letter_type(rdram, runtime, p4));
    ckEnd(159, ctx, c, 1, r, 0.0f, 0x12u, 0x0u);
}
void iosGetDataSizeOfFont(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t p4)
{
    if (!ckBegin(160, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    ref::iosGetDataSizeOfFont(rdram, &c, runtime);
    ckMid();
    const uint64_t r = a1_sx64(ty::iosGetDataSizeOfFont(rdram, runtime, p4));
    ckEnd(160, ctx, c, 1, r, 0.0f, 0x10u, 0x0u);
}
void iosGetFontDataWithJIS(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint64_t p4, uint64_t p5, uint64_t p6)
{
    if (!ckBegin(161, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    ref::iosGetFontDataWithJIS(rdram, &c, runtime);
    ckMid();
    const uint64_t r = (ty::iosGetFontDataWithJIS(rdram, runtime, p4, p5, p6));
    ckEnd(161, ctx, c, 2, r, 0.0f, 0x1e8u, 0x0u);
}
void iosGetFontIDWithJIS(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t p4)
{
    if (!ckBegin(162, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    ref::iosGetFontIDWithJIS(rdram, &c, runtime);
    ckMid();
    const uint64_t r = a1_sx64(ty::iosGetFontIDWithJIS(rdram, runtime, p4));
    ckEnd(162, ctx, c, 1, r, 0.0f, 0x18u, 0x0u);
}
void iosEntryLocaleFontNoLoad(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint64_t p4, uint64_t p5)
{
    if (!ckBegin(163, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    ref::iosEntryLocaleFontNoLoad(rdram, &c, runtime);
    ckMid();
    const uint64_t r = (ty::iosEntryLocaleFontNoLoad(rdram, runtime, p4, p5));
    ckEnd(163, ctx, c, 2, r, 0.0f, 0x3c8u, 0x0u);
}
void iosGet2ByteCharCode(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t p4)
{
    if (!ckBegin(164, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    ref::iosGet2ByteCharCode(rdram, &c, runtime);
    ckMid();
    const uint64_t r = a1_sx64(ty::iosGet2ByteCharCode(rdram, runtime, p4));
    ckEnd(164, ctx, c, 1, r, 0.0f, 0x8u, 0x0u);
}
void _isysCreateSubMsgQueue(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t p4, uint32_t p5, uint32_t p6, uint32_t p7)
{
    if (!ckBegin(165, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    ref::_isysCreateSubMsgQueue(rdram, &c, runtime);
    ckMid();
    ty::_isysCreateSubMsgQueue(rdram, runtime, p4, p5, p6, p7);
    ckEnd(165, ctx, c, 0, 0u, 0.0f, 0x0u, 0x0u);
}
void isysGetSubThreadWorkSize(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t p4)
{
    if (!ckBegin(166, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    ref::isysGetSubThreadWorkSize(rdram, &c, runtime);
    ckMid();
    const uint64_t r = a1_sx64(ty::isysGetSubThreadWorkSize(rdram, runtime, p4));
    ckEnd(166, ctx, c, 1, r, 0.0f, 0x0u, 0x0u);
}
void isysDumpSubThreadPC(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (!ckBegin(167, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    ref::isysDumpSubThreadPC(rdram, &c, runtime);
    ckMid();
    ty::isysDumpSubThreadPC(rdram, runtime);
    ckEnd(167, ctx, c, 0, 0u, 0.0f, 0x0u, 0x0u);
}
void isysDebugSetSubThreadSleepProtection(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t p4)
{
    if (!ckBegin(168, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    ref::isysDebugSetSubThreadSleepProtection(rdram, &c, runtime);
    ckMid();
    const uint64_t r = a1_sx64(ty::isysDebugSetSubThreadSleepProtection(rdram, runtime, p4));
    ckEnd(168, ctx, c, 1, r, 0.0f, 0x0u, 0x0u);
}
void sndSepSetDefaultParam(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t p4)
{
    if (!ckBegin(169, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    ref::sndSepSetDefaultParam(rdram, &c, runtime);
    ckMid();
    const a1::RetVF r = ty::sndSepSetDefaultParam(rdram, runtime, p4);
    ckEnd(169, ctx, c, 4, r.v0, r.f0, 0x6au, 0xeu);
}
void sndSepPauseAll(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint64_t p4)
{
    if (!ckBegin(170, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    ref::sndSepPauseAll(rdram, &c, runtime);
    ckMid();
    const uint64_t r = a1_sx64(ty::sndSepPauseAll(rdram, runtime, p4));
    ckEnd(170, ctx, c, 1, r, 0.0f, 0x8u, 0x0u);
}
void sndSepPause(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint64_t p4)
{
    if (!ckBegin(171, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    ref::sndSepPause(rdram, &c, runtime);
    ckMid();
    const uint64_t r = (ty::sndSepPause(rdram, runtime, p4));
    ckEnd(171, ctx, c, 2, r, 0.0f, 0x8u, 0x0u);
}
void sndSepCont(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint64_t p4)
{
    if (!ckBegin(172, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    ref::sndSepCont(rdram, &c, runtime);
    ckMid();
    const uint64_t r = (ty::sndSepCont(rdram, runtime, p4));
    ckEnd(172, ctx, c, 2, r, 0.0f, 0x8u, 0x0u);
}
void sndVabDelete(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t p4)
{
    if (!ckBegin(173, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    ref::sndVabDelete(rdram, &c, runtime);
    ckMid();
    const uint64_t r = a1_sx64(ty::sndVabDelete(rdram, runtime, p4));
    ckEnd(173, ctx, c, 1, r, 0.0f, 0x8u, 0x0u);
}
void sndBDClose(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (!ckBegin(174, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    ref::sndBDClose(rdram, &c, runtime);
    ckMid();
    ty::sndBDClose(rdram, runtime);
    ckEnd(174, ctx, c, 0, 0u, 0.0f, 0x0u, 0x0u);
}
void addAdpcmTickProcFunc(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t p4)
{
    if (!ckBegin(175, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    ref::addAdpcmTickProcFunc(rdram, &c, runtime);
    ckMid();
    const uint64_t r = a1_sx64(ty::addAdpcmTickProcFunc(rdram, runtime, p4));
    ckEnd(175, ctx, c, 1, r, 0.0f, 0x0u, 0x0u);
}
void setPcmTickProcFunc(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t p4)
{
    if (!ckBegin(176, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    ref::setPcmTickProcFunc(rdram, &c, runtime);
    ckMid();
    const uint64_t r = a1_sx64(ty::setPcmTickProcFunc(rdram, runtime, p4));
    ckEnd(176, ctx, c, 1, r, 0.0f, 0x0u, 0x0u);
}
void soundGetTickProcCnt(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (!ckBegin(177, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    ref::soundGetTickProcCnt(rdram, &c, runtime);
    ckMid();
    const uint64_t r = a1_sx64(ty::soundGetTickProcCnt(rdram, runtime));
    ckEnd(177, ctx, c, 1, r, 0.0f, 0x8u, 0x0u);
}
void soundSetRevMode(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint64_t p4, uint64_t p5)
{
    if (!ckBegin(178, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    ref::soundSetRevMode(rdram, &c, runtime);
    ckMid();
    const uint64_t r = (ty::soundSetRevMode(rdram, runtime, p4, p5));
    ckEnd(178, ctx, c, 2, r, 0.0f, 0x18u, 0x0u);
}
void soundGetRevMode(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (!ckBegin(179, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    ref::soundGetRevMode(rdram, &c, runtime);
    ckMid();
    const uint64_t r = a1_sx64(ty::soundGetRevMode(rdram, runtime));
    ckEnd(179, ctx, c, 1, r, 0.0f, 0x8u, 0x0u);
}
void getSoundMode(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (!ckBegin(180, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    ref::getSoundMode(rdram, &c, runtime);
    ckMid();
    const uint64_t r = a1_sx64(ty::getSoundMode(rdram, runtime));
    ckEnd(180, ctx, c, 1, r, 0.0f, 0x8u, 0x0u);
}
void iosKeybdSetCallBackFunc(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t p4)
{
    if (!ckBegin(181, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    ref::iosKeybdSetCallBackFunc(rdram, &c, runtime);
    ckMid();
    const uint64_t r = a1_sx64(ty::iosKeybdSetCallBackFunc(rdram, runtime, p4));
    ckEnd(181, ctx, c, 1, r, 0.0f, 0x8u, 0x0u);
}
void iosMouseSetCallBackFunc(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t p4)
{
    if (!ckBegin(182, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    ref::iosMouseSetCallBackFunc(rdram, &c, runtime);
    ckMid();
    const uint64_t r = a1_sx64(ty::iosMouseSetCallBackFunc(rdram, runtime, p4));
    ckEnd(182, ctx, c, 1, r, 0.0f, 0x8u, 0x0u);
}
void GetStrHashWorkBuffer(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t p4, uint32_t p5)
{
    if (!ckBegin(183, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    ref::GetStrHashWorkBuffer(rdram, &c, runtime);
    ckMid();
    const uint64_t r = a1_sx64(ty::GetStrHashWorkBuffer(rdram, runtime, p4, p5));
    ckEnd(183, ctx, c, 1, r, 0.0f, 0x20u, 0x0u);
}
void kernel_sub_001CC028(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (!ckBegin(184, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    ref::kernel_sub_001CC028(rdram, &c, runtime);
    ckMid();
    ty::kernel_sub_001CC028(rdram, runtime);
    ckEnd(184, ctx, c, 0, 0u, 0.0f, 0x0u, 0x0u);
}
void Sg2CtrlSetTickMode(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t p4)
{
    if (!ckBegin(185, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    ref::Sg2CtrlSetTickMode(rdram, &c, runtime);
    ckMid();
    const uint64_t r = a1_sx64(ty::Sg2CtrlSetTickMode(rdram, runtime, p4));
    ckEnd(185, ctx, c, 1, r, 0.0f, 0x8u, 0x0u);
}
void Sg2CtrlGetTickMode(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (!ckBegin(186, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    ref::Sg2CtrlGetTickMode(rdram, &c, runtime);
    ckMid();
    const uint64_t r = a1_sx64(ty::Sg2CtrlGetTickMode(rdram, runtime));
    ckEnd(186, ctx, c, 1, r, 0.0f, 0x8u, 0x0u);
}
void Sg2CtrlSetTickTime(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t p4)
{
    if (!ckBegin(187, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    ref::Sg2CtrlSetTickTime(rdram, &c, runtime);
    ckMid();
    const uint64_t r = a1_sx64(ty::Sg2CtrlSetTickTime(rdram, runtime, p4));
    ckEnd(187, ctx, c, 1, r, 0.0f, 0x8u, 0x0u);
}
void Sg2CtrlGetTickTime(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (!ckBegin(188, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    ref::Sg2CtrlGetTickTime(rdram, &c, runtime);
    ckMid();
    const uint64_t r = a1_sx64(ty::Sg2CtrlGetTickTime(rdram, runtime));
    ckEnd(188, ctx, c, 1, r, 0.0f, 0x8u, 0x0u);
}
void Sg2DmaInit(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (!ckBegin(189, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    ref::Sg2DmaInit(rdram, &c, runtime);
    ckMid();
    const uint64_t r = (ty::Sg2DmaInit(rdram, runtime));
    ckEnd(189, ctx, c, 2, r, 0.0f, 0x0u, 0x0u);
}
void Sg2DspGetMasterVolume(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t p4)
{
    if (!ckBegin(190, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    ref::Sg2DspGetMasterVolume(rdram, &c, runtime);
    ckMid();
    const uint64_t r = a1_sx64(ty::Sg2DspGetMasterVolume(rdram, runtime, p4));
    ckEnd(190, ctx, c, 1, r, 0.0f, 0x8u, 0x0u);
}
void Sg2ExStAdpcmStopMode(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t p4)
{
    if (!ckBegin(191, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    ref::Sg2ExStAdpcmStopMode(rdram, &c, runtime);
    ckMid();
    const uint64_t r = a1_sx64(ty::Sg2ExStAdpcmStopMode(rdram, runtime, p4));
    ckEnd(191, ctx, c, 1, r, 0.0f, 0x0u, 0x0u);
}
void Sg2ExStAdpcmSetFd(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t p4, uint32_t p5)
{
    if (!ckBegin(192, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    ref::Sg2ExStAdpcmSetFd(rdram, &c, runtime);
    ckMid();
    const uint64_t r = (ty::Sg2ExStAdpcmSetFd(rdram, runtime, p4, p5));
    ckEnd(192, ctx, c, 2, r, 0.0f, 0x0u, 0x0u);
}
void Sg2ExStAdpcmGetFd(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t p4)
{
    if (!ckBegin(193, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    ref::Sg2ExStAdpcmGetFd(rdram, &c, runtime);
    ckMid();
    const uint64_t r = a1_sx64(ty::Sg2ExStAdpcmGetFd(rdram, runtime, p4));
    ckEnd(193, ctx, c, 1, r, 0.0f, 0x0u, 0x0u);
}
void Sg2ExStAdpcmSetLsn(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t p4, uint32_t p5)
{
    if (!ckBegin(194, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    ref::Sg2ExStAdpcmSetLsn(rdram, &c, runtime);
    ckMid();
    const uint64_t r = (ty::Sg2ExStAdpcmSetLsn(rdram, runtime, p4, p5));
    ckEnd(194, ctx, c, 2, r, 0.0f, 0x0u, 0x0u);
}
void Sg2ExStAdpcmGetLsn(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t p4)
{
    if (!ckBegin(195, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    ref::Sg2ExStAdpcmGetLsn(rdram, &c, runtime);
    ckMid();
    const uint64_t r = a1_sx64(ty::Sg2ExStAdpcmGetLsn(rdram, runtime, p4));
    ckEnd(195, ctx, c, 1, r, 0.0f, 0x0u, 0x0u);
}
void Sg2ExStAdpcmGetTargetLsn(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t p4)
{
    if (!ckBegin(196, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    ref::Sg2ExStAdpcmGetTargetLsn(rdram, &c, runtime);
    ckMid();
    const uint64_t r = a1_sx64(ty::Sg2ExStAdpcmGetTargetLsn(rdram, runtime, p4));
    ckEnd(196, ctx, c, 1, r, 0.0f, 0x8u, 0x0u);
}
void Sg2ExStAdpcmSetUserParam(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t p4, uint32_t p5)
{
    if (!ckBegin(197, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    ref::Sg2ExStAdpcmSetUserParam(rdram, &c, runtime);
    ckMid();
    const uint64_t r = (ty::Sg2ExStAdpcmSetUserParam(rdram, runtime, p4, p5));
    ckEnd(197, ctx, c, 2, r, 0.0f, 0x0u, 0x0u);
}
void Sg2ExStAdpcmGetUserParam(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t p4)
{
    if (!ckBegin(198, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    ref::Sg2ExStAdpcmGetUserParam(rdram, &c, runtime);
    ckMid();
    const uint64_t r = a1_sx64(ty::Sg2ExStAdpcmGetUserParam(rdram, runtime, p4));
    ckEnd(198, ctx, c, 1, r, 0.0f, 0x0u, 0x0u);
}
void Sg2ExStAdpcmLoopSecNum(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t p4, uint32_t p5, uint32_t p6)
{
    if (!ckBegin(199, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    ref::Sg2ExStAdpcmLoopSecNum(rdram, &c, runtime);
    ckMid();
    const uint64_t r = (ty::Sg2ExStAdpcmLoopSecNum(rdram, runtime, p4, p5, p6));
    ckEnd(199, ctx, c, 2, r, 0.0f, 0x28u, 0x0u);
}
void Sg2ExStAdpcmGetBuffer(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t p4, uint32_t p5, uint32_t p6, uint32_t p7, uint32_t p8)
{
    if (!ckBegin(200, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    ref::Sg2ExStAdpcmGetBuffer(rdram, &c, runtime);
    ckMid();
    const uint64_t r = (ty::Sg2ExStAdpcmGetBuffer(rdram, runtime, p4, p5, p6, p7, p8));
    ckEnd(200, ctx, c, 2, r, 0.0f, 0x28u, 0x0u);
}
void Sg2ExStAdpcmSetSignal(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t p4)
{
    if (!ckBegin(201, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    ref::Sg2ExStAdpcmSetSignal(rdram, &c, runtime);
    ckMid();
    const uint64_t r = a1_sx64(ty::Sg2ExStAdpcmSetSignal(rdram, runtime, p4));
    ckEnd(201, ctx, c, 1, r, 0.0f, 0x0u, 0x0u);
}
void Sg2ModCheckSndOn(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint64_t p4)
{
    if (!ckBegin(202, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    ref::Sg2ModCheckSndOn(rdram, &c, runtime);
    ckMid();
    const uint64_t r = a1_sx64(ty::Sg2ModCheckSndOn(rdram, runtime, p4));
    ckEnd(202, ctx, c, 1, r, 0.0f, 0x8u, 0x0u);
}
void Sg2ModClose(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint64_t p4)
{
    if (!ckBegin(203, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    ref::Sg2ModClose(rdram, &c, runtime);
    ckMid();
    const uint64_t r = a1_sx64(ty::Sg2ModClose(rdram, runtime, p4));
    ckEnd(203, ctx, c, 1, r, 0.0f, 0x1au, 0x0u);
}
void Sg2ModGetParam(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t p6, uint32_t p7, uint64_t p4, uint64_t p5)
{
    if (!ckBegin(204, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    ref::Sg2ModGetParam(rdram, &c, runtime);
    ckMid();
    const uint64_t r = a1_sx64(ty::Sg2ModGetParam(rdram, runtime, p6, p7, p4, p5));
    ckEnd(204, ctx, c, 1, r, 0.0f, 0x338u, 0x0u);
}
void kernel_sub_001D0D78(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (!ckBegin(205, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    ref::kernel_sub_001D0D78(rdram, &c, runtime);
    ckMid();
    const uint64_t r = a1_sx64(ty::kernel_sub_001D0D78(rdram, runtime));
    ckEnd(205, ctx, c, 1, r, 0.0f, 0x78u, 0x0u);
}
void kernel_sub_001D12A8(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (!ckBegin(206, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    ref::kernel_sub_001D12A8(rdram, &c, runtime);
    ckMid();
    const uint64_t r = a1_sx64(ty::kernel_sub_001D12A8(rdram, runtime));
    ckEnd(206, ctx, c, 1, r, 0.0f, 0x78u, 0x0u);
}
void kernel_sub_001D12E0(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (!ckBegin(207, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    ref::kernel_sub_001D12E0(rdram, &c, runtime);
    ckMid();
    const uint64_t r = a1_sx64(ty::kernel_sub_001D12E0(rdram, runtime));
    ckEnd(207, ctx, c, 1, r, 0.0f, 0x78u, 0x0u);
}
void kernel_sub_001D1318(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (!ckBegin(208, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    ref::kernel_sub_001D1318(rdram, &c, runtime);
    ckMid();
    const uint64_t r = a1_sx64(ty::kernel_sub_001D1318(rdram, runtime));
    ckEnd(208, ctx, c, 1, r, 0.0f, 0x78u, 0x0u);
}
void kernel_sub_001D1350(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (!ckBegin(209, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    ref::kernel_sub_001D1350(rdram, &c, runtime);
    ckMid();
    ty::kernel_sub_001D1350(rdram, runtime);
    ckEnd(209, ctx, c, 0, 0u, 0.0f, 0x0u, 0x0u);
}
void kernel_sub_001D1358(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (!ckBegin(210, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    ref::kernel_sub_001D1358(rdram, &c, runtime);
    ckMid();
    ty::kernel_sub_001D1358(rdram, runtime);
    ckEnd(210, ctx, c, 0, 0u, 0.0f, 0x0u, 0x0u);
}
void kernel_sub_001D13E8(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (!ckBegin(211, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    ref::kernel_sub_001D13E8(rdram, &c, runtime);
    ckMid();
    const uint64_t r = a1_sx64(ty::kernel_sub_001D13E8(rdram, runtime));
    ckEnd(211, ctx, c, 1, r, 0.0f, 0x78u, 0x0u);
}
void kernel_sub_001D1470(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (!ckBegin(212, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    ref::kernel_sub_001D1470(rdram, &c, runtime);
    ckMid();
    const uint64_t r = a1_sx64(ty::kernel_sub_001D1470(rdram, runtime));
    ckEnd(212, ctx, c, 1, r, 0.0f, 0x78u, 0x0u);
}
void kernel_sub_001D14C8(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (!ckBegin(213, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    ref::kernel_sub_001D14C8(rdram, &c, runtime);
    ckMid();
    const uint64_t r = (ty::kernel_sub_001D14C8(rdram, runtime));
    ckEnd(213, ctx, c, 2, r, 0.0f, 0x78u, 0x0u);
}
void kernel_sub_001D1520(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (!ckBegin(214, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    ref::kernel_sub_001D1520(rdram, &c, runtime);
    ckMid();
    const uint64_t r = a1_sx64(ty::kernel_sub_001D1520(rdram, runtime));
    ckEnd(214, ctx, c, 1, r, 0.0f, 0xf8u, 0x0u);
}
void kernel_sub_001D1578(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (!ckBegin(215, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    ref::kernel_sub_001D1578(rdram, &c, runtime);
    ckMid();
    ty::kernel_sub_001D1578(rdram, runtime);
    ckEnd(215, ctx, c, 0, 0u, 0.0f, 0x0u, 0x0u);
}
void kernel_sub_001D1580(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (!ckBegin(216, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    ref::kernel_sub_001D1580(rdram, &c, runtime);
    ckMid();
    const uint64_t r = a1_sx64(ty::kernel_sub_001D1580(rdram, runtime));
    ckEnd(216, ctx, c, 1, r, 0.0f, 0x78u, 0x0u);
}
void kernel_sub_001D1658(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (!ckBegin(217, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    ref::kernel_sub_001D1658(rdram, &c, runtime);
    ckMid();
    ty::kernel_sub_001D1658(rdram, runtime);
    ckEnd(217, ctx, c, 0, 0u, 0.0f, 0x0u, 0x0u);
}
void kernel_sub_001D1660(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (!ckBegin(218, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    ref::kernel_sub_001D1660(rdram, &c, runtime);
    ckMid();
    const uint64_t r = a1_sx64(ty::kernel_sub_001D1660(rdram, runtime));
    ckEnd(218, ctx, c, 1, r, 0.0f, 0x78u, 0x0u);
}
void kernel_sub_001D1698(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (!ckBegin(219, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    ref::kernel_sub_001D1698(rdram, &c, runtime);
    ckMid();
    const uint64_t r = a1_sx64(ty::kernel_sub_001D1698(rdram, runtime));
    ckEnd(219, ctx, c, 1, r, 0.0f, 0x78u, 0x0u);
}
void kernel_sub_001D1810(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (!ckBegin(220, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    ref::kernel_sub_001D1810(rdram, &c, runtime);
    ckMid();
    ty::kernel_sub_001D1810(rdram, runtime);
    ckEnd(220, ctx, c, 0, 0u, 0.0f, 0x0u, 0x0u);
}
void kernel_sub_001D18D8(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (!ckBegin(221, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    ref::kernel_sub_001D18D8(rdram, &c, runtime);
    ckMid();
    ty::kernel_sub_001D18D8(rdram, runtime);
    ckEnd(221, ctx, c, 0, 0u, 0.0f, 0x0u, 0x0u);
}
void kernel_sub_001D1F08(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (!ckBegin(222, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    ref::kernel_sub_001D1F08(rdram, &c, runtime);
    ckMid();
    ty::kernel_sub_001D1F08(rdram, runtime);
    ckEnd(222, ctx, c, 0, 0u, 0.0f, 0x0u, 0x0u);
}
void kernel_sub_001D1F10(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (!ckBegin(223, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    ref::kernel_sub_001D1F10(rdram, &c, runtime);
    ckMid();
    ty::kernel_sub_001D1F10(rdram, runtime);
    ckEnd(223, ctx, c, 0, 0u, 0.0f, 0x0u, 0x0u);
}
void kernel_sub_001D1F18(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (!ckBegin(224, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    ref::kernel_sub_001D1F18(rdram, &c, runtime);
    ckMid();
    ty::kernel_sub_001D1F18(rdram, runtime);
    ckEnd(224, ctx, c, 0, 0u, 0.0f, 0x0u, 0x0u);
}
void kernel_sub_001D1F20(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (!ckBegin(225, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    ref::kernel_sub_001D1F20(rdram, &c, runtime);
    ckMid();
    ty::kernel_sub_001D1F20(rdram, runtime);
    ckEnd(225, ctx, c, 0, 0u, 0.0f, 0x0u, 0x0u);
}
void kernel_sub_001D2008(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (!ckBegin(226, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    ref::kernel_sub_001D2008(rdram, &c, runtime);
    ckMid();
    ty::kernel_sub_001D2008(rdram, runtime);
    ckEnd(226, ctx, c, 0, 0u, 0.0f, 0x0u, 0x0u);
}
void Sg2RemoteDmaTransStatus(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t p4)
{
    if (!ckBegin(227, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    ref::Sg2RemoteDmaTransStatus(rdram, &c, runtime);
    ckMid();
    const uint64_t r = a1_sx64(ty::Sg2RemoteDmaTransStatus(rdram, runtime, p4));
    ckEnd(227, ctx, c, 1, r, 0.0f, 0x8u, 0x0u);
}
void Sg2RemoteGetAdpStmAdr(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t p4)
{
    if (!ckBegin(228, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    ref::Sg2RemoteGetAdpStmAdr(rdram, &c, runtime);
    ckMid();
    const uint64_t r = a1_sx64(ty::Sg2RemoteGetAdpStmAdr(rdram, runtime, p4));
    ckEnd(228, ctx, c, 1, r, 0.0f, 0x10u, 0x0u);
}
void Sg2RemoteGetPcmStmTransOffset(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (!ckBegin(229, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    ref::Sg2RemoteGetPcmStmTransOffset(rdram, &c, runtime);
    ckMid();
    const uint64_t r = a1_sx64(ty::Sg2RemoteGetPcmStmTransOffset(rdram, runtime));
    ckEnd(229, ctx, c, 1, r, 0.0f, 0x8u, 0x0u);
}
void Sg2RemoteGetSlotReleaseLv(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t p4)
{
    if (!ckBegin(230, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    ref::Sg2RemoteGetSlotReleaseLv(rdram, &c, runtime);
    ckMid();
    const uint64_t r = a1_sx64(ty::Sg2RemoteGetSlotReleaseLv(rdram, runtime, p4));
    ckEnd(230, ctx, c, 1, r, 0.0f, 0x10u, 0x0u);
}
void Sg2RemoteGetDspChangeStatus(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t p4)
{
    if (!ckBegin(231, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    ref::Sg2RemoteGetDspChangeStatus(rdram, &c, runtime);
    ckMid();
    const uint64_t r = a1_sx64(ty::Sg2RemoteGetDspChangeStatus(rdram, runtime, p4));
    ckEnd(231, ctx, c, 1, r, 0.0f, 0x10u, 0x0u);
}
void Sg2SepStatus(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint64_t p4)
{
    if (!ckBegin(232, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    ref::Sg2SepStatus(rdram, &c, runtime);
    ckMid();
    const uint64_t r = a1_sx64(ty::Sg2SepStatus(rdram, runtime, p4));
    ckEnd(232, ctx, c, 1, r, 0.0f, 0x8u, 0x0u);
}
void kernel_sub_001D3428(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (!ckBegin(233, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    ref::kernel_sub_001D3428(rdram, &c, runtime);
    ckMid();
    ty::kernel_sub_001D3428(rdram, runtime);
    ckEnd(233, ctx, c, 0, 0u, 0.0f, 0x0u, 0x0u);
}
void kernel_sub_001D3430(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t p4)
{
    if (!ckBegin(234, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    ref::kernel_sub_001D3430(rdram, &c, runtime);
    ckMid();
    const uint64_t r = a1_sx64(ty::kernel_sub_001D3430(rdram, runtime, p4));
    ckEnd(234, ctx, c, 1, r, 0.0f, 0xffau, 0x0u);
}
void kernel_sub_001D3A48(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint64_t p4)
{
    if (!ckBegin(235, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    ref::kernel_sub_001D3A48(rdram, &c, runtime);
    ckMid();
    const uint64_t r = a1_sx64(ty::kernel_sub_001D3A48(rdram, runtime, p4));
    ckEnd(235, ctx, c, 1, r, 0.0f, 0x3fau, 0x0u);
}
void Sg2SlotFree(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t p4)
{
    if (!ckBegin(236, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    ref::Sg2SlotFree(rdram, &c, runtime);
    ckMid();
    const uint64_t r = a1_sx64(ty::Sg2SlotFree(rdram, runtime, p4));
    ckEnd(236, ctx, c, 1, r, 0.0f, 0x18u, 0x0u);
}
void Sg2SlotSetGroup(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t p4, uint32_t p5)
{
    if (!ckBegin(237, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    ref::Sg2SlotSetGroup(rdram, &c, runtime);
    ckMid();
    const uint64_t r = a1_sx64(ty::Sg2SlotSetGroup(rdram, runtime, p4, p5));
    ckEnd(237, ctx, c, 1, r, 0.0f, 0x2u, 0x0u);
}
void Sg2SlotSetMute(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint64_t p4, uint64_t p5)
{
    if (!ckBegin(238, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    ref::Sg2SlotSetMute(rdram, &c, runtime);
    ckMid();
    const uint64_t r = (ty::Sg2SlotSetMute(rdram, runtime, p4, p5));
    ckEnd(238, ctx, c, 2, r, 0.0f, 0xfau, 0x0u);
}
void Sg2SlotSetBendRange(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t p4, uint32_t p5)
{
    if (!ckBegin(239, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    ref::Sg2SlotSetBendRange(rdram, &c, runtime);
    ckMid();
    const uint64_t r = a1_sx64(ty::Sg2SlotSetBendRange(rdram, runtime, p4, p5));
    ckEnd(239, ctx, c, 1, r, 0.0f, 0x2u, 0x0u);
}
void Sg2SlotSetBendValue(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t p4, uint32_t p5)
{
    if (!ckBegin(240, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    ref::Sg2SlotSetBendValue(rdram, &c, runtime);
    ckMid();
    const uint64_t r = a1_sx64(ty::Sg2SlotSetBendValue(rdram, runtime, p4, p5));
    ckEnd(240, ctx, c, 1, r, 0.0f, 0x18u, 0x0u);
}
void Sg2SlotSetPause(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint64_t p4, uint64_t p5)
{
    if (!ckBegin(241, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    ref::Sg2SlotSetPause(rdram, &c, runtime);
    ckMid();
    const uint64_t r = (ty::Sg2SlotSetPause(rdram, runtime, p4, p5));
    ckEnd(241, ctx, c, 2, r, 0.0f, 0xfau, 0x0u);
}
void Sg2SlotSetDirectPitch(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t p4, uint32_t p5)
{
    if (!ckBegin(242, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    ref::Sg2SlotSetDirectPitch(rdram, &c, runtime);
    ckMid();
    const uint64_t r = a1_sx64(ty::Sg2SlotSetDirectPitch(rdram, runtime, p4, p5));
    ckEnd(242, ctx, c, 1, r, 0.0f, 0x18u, 0x0u);
}
void Sg2SlotSetLoop(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t p4)
{
    if (!ckBegin(243, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    ref::Sg2SlotSetLoop(rdram, &c, runtime);
    ckMid();
    const uint64_t r = a1_sx64(ty::Sg2SlotSetLoop(rdram, runtime, p4));
    ckEnd(243, ctx, c, 1, r, 0.0f, 0xau, 0x0u);
}
void Sg2SlotSetDumperOn(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t p4)
{
    if (!ckBegin(244, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    ref::Sg2SlotSetDumperOn(rdram, &c, runtime);
    ckMid();
    const uint64_t r = a1_sx64(ty::Sg2SlotSetDumperOn(rdram, runtime, p4));
    ckEnd(244, ctx, c, 1, r, 0.0f, 0xau, 0x0u);
}
void Sg2SlotSetLfoVolTbl(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t p4, uint64_t p5)
{
    if (!ckBegin(245, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    ref::Sg2SlotSetLfoVolTbl(rdram, &c, runtime);
    ckMid();
    const uint64_t r = a1_sx64(ty::Sg2SlotSetLfoVolTbl(rdram, runtime, p4, p5));
    ckEnd(245, ctx, c, 1, r, 0.0f, 0xf8u, 0x0u);
}
void Sg2SlotSetTimeVolParam(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t p4, float pf12, float pf13)
{
    if (!ckBegin(246, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    ref::Sg2SlotSetTimeVolParam(rdram, &c, runtime);
    ckMid();
    const uint64_t r = a1_sx64(ty::Sg2SlotSetTimeVolParam(rdram, runtime, p4, pf12, pf13));
    ckEnd(246, ctx, c, 1, r, 0.0f, 0x78u, 0x0u);
}
void Sg2SlotSetTimePitchParam(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t p4, float pf12, float pf13)
{
    if (!ckBegin(247, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    ref::Sg2SlotSetTimePitchParam(rdram, &c, runtime);
    ckMid();
    const uint64_t r = a1_sx64(ty::Sg2SlotSetTimePitchParam(rdram, runtime, p4, pf12, pf13));
    ckEnd(247, ctx, c, 1, r, 0.0f, 0x78u, 0x0u);
}
void Sg2SlotSetTimePanParam(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t p4, float pf12, float pf13)
{
    if (!ckBegin(248, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    ref::Sg2SlotSetTimePanParam(rdram, &c, runtime);
    ckMid();
    const uint64_t r = a1_sx64(ty::Sg2SlotSetTimePanParam(rdram, runtime, p4, pf12, pf13));
    ckEnd(248, ctx, c, 1, r, 0.0f, 0x78u, 0x0u);
}
void Sg2SlotGetVol(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t p4)
{
    if (!ckBegin(249, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    ref::Sg2SlotGetVol(rdram, &c, runtime);
    ckMid();
    const uint64_t r = a1_sx64(ty::Sg2SlotGetVol(rdram, runtime, p4));
    ckEnd(249, ctx, c, 1, r, 0.0f, 0x8u, 0x0u);
}
void Sg2SlotGetPan(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t p4)
{
    if (!ckBegin(250, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    ref::Sg2SlotGetPan(rdram, &c, runtime);
    ckMid();
    const uint64_t r = a1_sx64(ty::Sg2SlotGetPan(rdram, runtime, p4));
    ckEnd(250, ctx, c, 1, r, 0.0f, 0x8u, 0x0u);
}
void Sg2SlotSetOutputMode(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t p4)
{
    if (!ckBegin(251, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    ref::Sg2SlotSetOutputMode(rdram, &c, runtime);
    ckMid();
    const uint64_t r = a1_sx64(ty::Sg2SlotSetOutputMode(rdram, runtime, p4));
    ckEnd(251, ctx, c, 1, r, 0.0f, 0x8u, 0x0u);
}
void Sg2SlotSetVolShiftDownParam(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t p4)
{
    if (!ckBegin(252, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    ref::Sg2SlotSetVolShiftDownParam(rdram, &c, runtime);
    ckMid();
    const uint64_t r = a1_sx64(ty::Sg2SlotSetVolShiftDownParam(rdram, runtime, p4));
    ckEnd(252, ctx, c, 1, r, 0.0f, 0x8u, 0x0u);
}
void Sg2SlotGetVolShiftDownParam(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (!ckBegin(253, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    ref::Sg2SlotGetVolShiftDownParam(rdram, &c, runtime);
    ckMid();
    const uint64_t r = a1_sx64(ty::Sg2SlotGetVolShiftDownParam(rdram, runtime));
    ckEnd(253, ctx, c, 1, r, 0.0f, 0x8u, 0x0u);
}
void kernel_sub_001D8250(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t p4)
{
    if (!ckBegin(254, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    ref::kernel_sub_001D8250(rdram, &c, runtime);
    ckMid();
    const uint64_t r = (ty::kernel_sub_001D8250(rdram, runtime, p4));
    ckEnd(254, ctx, c, 2, r, 0.0f, 0x8u, 0x0u);
}
void Sg2SmfpStatus(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint64_t p4)
{
    if (!ckBegin(255, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    ref::Sg2SmfpStatus(rdram, &c, runtime);
    ckMid();
    const uint64_t r = a1_sx64(ty::Sg2SmfpStatus(rdram, runtime, p4));
    ckEnd(255, ctx, c, 1, r, 0.0f, 0x2u, 0x0u);
}
void Sg2SmfpSetSignal(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t p4)
{
    if (!ckBegin(256, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    ref::Sg2SmfpSetSignal(rdram, &c, runtime);
    ckMid();
    const uint64_t r = a1_sx64(ty::Sg2SmfpSetSignal(rdram, runtime, p4));
    ckEnd(256, ctx, c, 1, r, 0.0f, 0x0u, 0x0u);
}
void kernel_sub_001D95C8(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (!ckBegin(257, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    ref::kernel_sub_001D95C8(rdram, &c, runtime);
    ckMid();
    ty::kernel_sub_001D95C8(rdram, runtime);
    ckEnd(257, ctx, c, 0, 0u, 0.0f, 0x0u, 0x0u);
}
void kernel_sub_001D95D0(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t p4)
{
    if (!ckBegin(258, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    ref::kernel_sub_001D95D0(rdram, &c, runtime);
    ckMid();
    const uint64_t r = a1_sx64(ty::kernel_sub_001D95D0(rdram, runtime, p4));
    ckEnd(258, ctx, c, 1, r, 0.0f, 0x3fau, 0x0u);
}
void kernel_sub_001D97F0(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t p4)
{
    if (!ckBegin(259, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    ref::kernel_sub_001D97F0(rdram, &c, runtime);
    ckMid();
    const uint64_t r = a1_sx64(ty::kernel_sub_001D97F0(rdram, runtime, p4));
    ckEnd(259, ctx, c, 1, r, 0.0f, 0xfau, 0x0u);
}
void vab_check_alive(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint64_t p4)
{
    if (!ckBegin(260, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    ref::vab_check_alive(rdram, &c, runtime);
    ckMid();
    const uint64_t r = (ty::vab_check_alive(rdram, runtime, p4));
    ckEnd(260, ctx, c, 2, r, 0.0f, 0x8u, 0x0u);
}
void vab_get_info(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t p4, uint32_t p5, uint32_t p6)
{
    if (!ckBegin(261, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    ref::vab_get_info(rdram, &c, runtime);
    ckMid();
    const uint64_t r = (ty::vab_get_info(rdram, runtime, p4, p5, p6));
    ckEnd(261, ctx, c, 2, r, 0.0f, 0x1b8u, 0x0u);
}
void vab_get_veltbl(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t p4, uint32_t p5)
{
    if (!ckBegin(262, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    ref::vab_get_veltbl(rdram, &c, runtime);
    ckMid();
    const uint64_t r = a1_sx64(ty::vab_get_veltbl(rdram, runtime, p4, p5));
    ckEnd(262, ctx, c, 1, r, 0.0f, 0x58u, 0x0u);
}
void vab_get_lfotbl_param(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t p4, uint32_t p5, uint32_t p6)
{
    if (!ckBegin(263, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    ref::vab_get_lfotbl_param(rdram, &c, runtime);
    ckMid();
    const uint64_t r = a1_sx64(ty::vab_get_lfotbl_param(rdram, runtime, p4, p5, p6));
    ckEnd(263, ctx, c, 1, r, 0.0f, 0x198u, 0x0u);
}
void vab_set_semastervol(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t p4, uint32_t p5)
{
    if (!ckBegin(264, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    ref::vab_set_semastervol(rdram, &c, runtime);
    ckMid();
    const uint64_t r = a1_sx64(ty::vab_set_semastervol(rdram, runtime, p4, p5));
    ckEnd(264, ctx, c, 1, r, 0.0f, 0xau, 0x0u);
}
void vab_get_semastervol(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t p4)
{
    if (!ckBegin(265, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    ref::vab_get_semastervol(rdram, &c, runtime);
    ckMid();
    const uint64_t r = a1_sx64(ty::vab_get_semastervol(rdram, runtime, p4));
    ckEnd(265, ctx, c, 1, r, 0.0f, 0x8u, 0x0u);
}
void kernel_sub_001DB3C8(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t p4)
{
    if (!ckBegin(266, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    ref::kernel_sub_001DB3C8(rdram, &c, runtime);
    ckMid();
    const uint64_t r = a1_sx64(ty::kernel_sub_001DB3C8(rdram, runtime, p4));
    ckEnd(266, ctx, c, 1, r, 0.0f, 0x8u, 0x0u);
}
void Sg2ExPcmInputGetLipFrame(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint64_t p4)
{
    if (!ckBegin(267, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    ref::Sg2ExPcmInputGetLipFrame(rdram, &c, runtime);
    ckMid();
    const uint64_t r = a1_sx64(ty::Sg2ExPcmInputGetLipFrame(rdram, runtime, p4));
    ckEnd(267, ctx, c, 1, r, 0.0f, 0x8u, 0x0u);
}
void Sg2ExPcmInputSetOutputMode(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t p5, uint64_t p4)
{
    if (!ckBegin(268, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    ref::Sg2ExPcmInputSetOutputMode(rdram, &c, runtime);
    ckMid();
    const uint64_t r = (ty::Sg2ExPcmInputSetOutputMode(rdram, runtime, p5, p4));
    ckEnd(268, ctx, c, 2, r, 0.0f, 0x2u, 0x0u);
}
void Sg2ExPcmInputAutoFilterCtrl(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t p5, uint32_t p6, uint64_t p4)
{
    if (!ckBegin(269, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    ref::Sg2ExPcmInputAutoFilterCtrl(rdram, &c, runtime);
    ckMid();
    const uint64_t r = a1_sx64(ty::Sg2ExPcmInputAutoFilterCtrl(rdram, runtime, p5, p6, p4));
    ckEnd(269, ctx, c, 1, r, 0.0f, 0xau, 0x0u);
}
void Sg2ExPcmOutputSetOutputMode(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t p4)
{
    if (!ckBegin(270, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    ref::Sg2ExPcmOutputSetOutputMode(rdram, &c, runtime);
    ckMid();
    const uint64_t r = a1_sx64(ty::Sg2ExPcmOutputSetOutputMode(rdram, runtime, p4));
    ckEnd(270, ctx, c, 1, r, 0.0f, 0x0u, 0x0u);
}
void Sg2ExPcmOutputGetLipFrame(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (!ckBegin(271, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    ref::Sg2ExPcmOutputGetLipFrame(rdram, &c, runtime);
    ckMid();
    const uint64_t r = a1_sx64(ty::Sg2ExPcmOutputGetLipFrame(rdram, runtime));
    ckEnd(271, ctx, c, 1, r, 0.0f, 0x8u, 0x0u);
}
void Sg2ExPcmOutputAutoFilterCtrl(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t p4, uint32_t p5)
{
    if (!ckBegin(272, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    ref::Sg2ExPcmOutputAutoFilterCtrl(rdram, &c, runtime);
    ckMid();
    const uint64_t r = a1_sx64(ty::Sg2ExPcmOutputAutoFilterCtrl(rdram, runtime, p4, p5));
    ckEnd(272, ctx, c, 1, r, 0.0f, 0x2u, 0x0u);
}
void Sg2ExPcmSetSignal(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t p4)
{
    if (!ckBegin(273, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    ref::Sg2ExPcmSetSignal(rdram, &c, runtime);
    ckMid();
    const uint64_t r = a1_sx64(ty::Sg2ExPcmSetSignal(rdram, runtime, p4));
    ckEnd(273, ctx, c, 1, r, 0.0f, 0x0u, 0x0u);
}
void IirInit(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t p4)
{
    if (!ckBegin(274, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    ref::IirInit(rdram, &c, runtime);
    ckMid();
    ty::IirInit(rdram, runtime, p4);
    ckEnd(274, ctx, c, 0, 0u, 0.0f, 0x0u, 0x0u);
}
void GAMEUTIL_is_inside_of_region_pitch(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint64_t p4, uint64_t p5, float pf12, float pf13, float pf14)
{
    if (!ckBegin(275, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    ref::GAMEUTIL_is_inside_of_region_pitch(rdram, &c, runtime);
    ckMid();
    const a1::RetVF r = ty::GAMEUTIL_is_inside_of_region_pitch(rdram, runtime, p4, p5, pf12, pf13, pf14);
    ckEnd(275, ctx, c, 4, r.v0, r.f0, 0x2u, 0x403eu);
}
void GAMEUTIL_is_nearly_equal_zero(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, float pf12, float pf13)
{
    if (!ckBegin(276, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    ref::GAMEUTIL_is_nearly_equal_zero(rdram, &c, runtime);
    ckMid();
    const a1::RetVF r = ty::GAMEUTIL_is_nearly_equal_zero(rdram, runtime, pf12, pf13);
    ckEnd(276, ctx, c, 4, r.v0, r.f0, 0x0u, 0x0u);
}
void GAMEUTIL_is_nearly_equal_zero3D(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t p4, float pf12)
{
    if (!ckBegin(277, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    ref::GAMEUTIL_is_nearly_equal_zero3D(rdram, &c, runtime);
    ckMid();
    const a1::RetVF r = ty::GAMEUTIL_is_nearly_equal_zero3D(rdram, runtime, p4, pf12);
    ckEnd(277, ctx, c, 4, r.v0, r.f0, 0x0u, 0x2u);
}
void GAMEUTIL_get_answer_of_2jihouteishiki(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t p4, uint32_t p5, float pf12, float pf13, float pf14)
{
    if (!ckBegin(278, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    ref::GAMEUTIL_get_answer_of_2jihouteishiki(rdram, &c, runtime);
    ckMid();
    const a1::RetVF r = ty::GAMEUTIL_get_answer_of_2jihouteishiki(rdram, runtime, p4, p5, pf12, pf13, pf14);
    ckEnd(278, ctx, c, 4, r.v0, r.f0, 0x2u, 0xeu);
}
void GAMEUTIL_get_new_velocity_by_brake_abs(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, float pf12, float pf13, float pf14, float pf15)
{
    if (!ckBegin(279, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    ref::GAMEUTIL_get_new_velocity_by_brake_abs(rdram, &c, runtime);
    ckMid();
    const float r = ty::GAMEUTIL_get_new_velocity_by_brake_abs(rdram, runtime, pf12, pf13, pf14, pf15);
    ckEnd(279, ctx, c, 3, 0u, r, 0x0u, 0x0u);
}
void GAMEUTIL_get_velocity_by_diff_move(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint64_t p4, float pf12, float pf13, float pf14, float pf15)
{
    if (!ckBegin(280, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    ref::GAMEUTIL_get_velocity_by_diff_move(rdram, &c, runtime);
    ckMid();
    const float r = ty::GAMEUTIL_get_velocity_by_diff_move(rdram, runtime, p4, pf12, pf13, pf14, pf15);
    ckEnd(280, ctx, c, 3, 0u, r, 0x0u, 0x2u);
}
void GAMEUTIL_mul_ratio(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint64_t p4, float pf12, float pf13)
{
    if (!ckBegin(281, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    ref::GAMEUTIL_mul_ratio(rdram, &c, runtime);
    ckMid();
    const float r = ty::GAMEUTIL_mul_ratio(rdram, runtime, p4, pf12, pf13);
    ckEnd(281, ctx, c, 3, 0u, r, 0x2u, 0x2u);
}
void GAMEUTIL_convert_from_lw_to_game_coord(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t p4)
{
    if (!ckBegin(282, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    ref::GAMEUTIL_convert_from_lw_to_game_coord(rdram, &c, runtime);
    ckMid();
    const float r = ty::GAMEUTIL_convert_from_lw_to_game_coord(rdram, runtime, p4);
    ckEnd(282, ctx, c, 3, 0u, r, 0x0u, 0x2u);
}
void GAMEUTIL_get_difference_pitch_and_pitch(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, float pf12, float pf13)
{
    if (!ckBegin(283, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    ref::GAMEUTIL_get_difference_pitch_and_pitch(rdram, &c, runtime);
    ckMid();
    const float r = ty::GAMEUTIL_get_difference_pitch_and_pitch(rdram, runtime, pf12, pf13);
    ckEnd(283, ctx, c, 3, 0u, r, 0x0u, 0x2000u);
}
void GAMEUTIL_get_move_radi_from_yaw1_to_yaw2(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, float pf12, float pf13)
{
    if (!ckBegin(284, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    ref::GAMEUTIL_get_move_radi_from_yaw1_to_yaw2(rdram, &c, runtime);
    ckMid();
    const a1::RetVF r = ty::GAMEUTIL_get_move_radi_from_yaw1_to_yaw2(rdram, runtime, pf12, pf13);
    ckEnd(284, ctx, c, 4, r.v0, r.f0, 0x2u, 0x2u);
}
void GAMEUTIL_get_move_radi_from_pitch1_to_pitch2(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, float pf12, float pf13)
{
    if (!ckBegin(285, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    ref::GAMEUTIL_get_move_radi_from_pitch1_to_pitch2(rdram, &c, runtime);
    ckMid();
    const float r = ty::GAMEUTIL_get_move_radi_from_pitch1_to_pitch2(rdram, runtime, pf12, pf13);
    ckEnd(285, ctx, c, 3, 0u, r, 0x0u, 0x0u);
}
void GAMEUTIL_is_between_yaw_and_yaw(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, float pf12, float pf13, float pf14)
{
    if (!ckBegin(286, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    ref::GAMEUTIL_is_between_yaw_and_yaw(rdram, &c, runtime);
    ckMid();
    const a1::RetVF r = ty::GAMEUTIL_is_between_yaw_and_yaw(rdram, runtime, pf12, pf13, pf14);
    ckEnd(286, ctx, c, 4, r.v0, r.f0, 0x2u, 0x2u);
}
void GAMEUTIL_is_between_pitch_and_pitch(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, float pf12, float pf13, float pf14)
{
    if (!ckBegin(287, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    ref::GAMEUTIL_is_between_pitch_and_pitch(rdram, &c, runtime);
    ckMid();
    const uint64_t r = (ty::GAMEUTIL_is_between_pitch_and_pitch(rdram, runtime, pf12, pf13, pf14));
    ckEnd(287, ctx, c, 2, r, 0.0f, 0x0u, 0x0u);
}
void GAMEUTIL_is_reach_aim_yaw_angle(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint64_t p4, float pf12, float pf13, float pf14)
{
    if (!ckBegin(288, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    ref::GAMEUTIL_is_reach_aim_yaw_angle(rdram, &c, runtime);
    ckMid();
    const a1::RetVF r = ty::GAMEUTIL_is_reach_aim_yaw_angle(rdram, runtime, p4, pf12, pf13, pf14);
    ckEnd(288, ctx, c, 4, r.v0, r.f0, 0xau, 0x6u);
}
void GAMEUTIL_is_nearly_equal_yaw(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, float pf12, float pf13, float pf14)
{
    if (!ckBegin(289, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    ref::GAMEUTIL_is_nearly_equal_yaw(rdram, &c, runtime);
    ckMid();
    const a1::RetVF r = ty::GAMEUTIL_is_nearly_equal_yaw(rdram, runtime, pf12, pf13, pf14);
    ckEnd(289, ctx, c, 4, r.v0, r.f0, 0x2u, 0x2000u);
}
void GAMEUTIL_is_nearly_equal_pitch(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, float pf12, float pf13, float pf14)
{
    if (!ckBegin(290, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    ref::GAMEUTIL_is_nearly_equal_pitch(rdram, &c, runtime);
    ckMid();
    const a1::RetVF r = ty::GAMEUTIL_is_nearly_equal_pitch(rdram, runtime, pf12, pf13, pf14);
    ckEnd(290, ctx, c, 4, r.v0, r.f0, 0x0u, 0x2000u);
}
void manager_sub_01082C78(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, float pf12)
{
    if (!ckBegin(291, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    ref::manager_sub_01082C78(rdram, &c, runtime);
    ckMid();
    const a1::RetVF r = ty::manager_sub_01082C78(rdram, runtime, pf12);
    ckEnd(291, ctx, c, 4, r.v0, r.f0, 0x2u, 0x2u);
}
void manager_sub_01082CD8(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, float pf12, float pf13, float pf14)
{
    if (!ckBegin(292, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    ref::manager_sub_01082CD8(rdram, &c, runtime);
    ckMid();
    const a1::RetVF r = ty::manager_sub_01082CD8(rdram, runtime, pf12, pf13, pf14);
    ckEnd(292, ctx, c, 4, r.v0, r.f0, 0x2u, 0x6u);
}
void get_fast_get_malloc_partition(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (!ckBegin(293, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    ref::get_fast_get_malloc_partition(rdram, &c, runtime);
    ckMid();
    const uint64_t r = a1_sx64(ty::get_fast_get_malloc_partition(rdram, runtime));
    ckEnd(293, ctx, c, 1, r, 0.0f, 0x8u, 0x0u);
}
void GAMEUTIL_doit_interp_ratio_cntrl(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t p4, float pf12)
{
    if (!ckBegin(294, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    ref::GAMEUTIL_doit_interp_ratio_cntrl(rdram, &c, runtime);
    ckMid();
    const a1::RetVF r = ty::GAMEUTIL_doit_interp_ratio_cntrl(rdram, runtime, p4, pf12);
    ckEnd(294, ctx, c, 4, r.v0, r.f0, 0xau, 0xeu);
}
void GAMEUTIL_get_dist_ratio(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, float pf12, float pf13, float pf14)
{
    if (!ckBegin(295, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    ref::GAMEUTIL_get_dist_ratio(rdram, &c, runtime);
    ckMid();
    const float r = ty::GAMEUTIL_get_dist_ratio(rdram, runtime, pf12, pf13, pf14);
    ckEnd(295, ctx, c, 3, 0u, r, 0x2u, 0x2u);
}
void GAMEUTIL_get_interp_value(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, float pf12, float pf13, float pf14)
{
    if (!ckBegin(296, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    ref::GAMEUTIL_get_interp_value(rdram, &c, runtime);
    ckMid();
    const float r = ty::GAMEUTIL_get_interp_value(rdram, runtime, pf12, pf13, pf14);
    ckEnd(296, ctx, c, 3, 0u, r, 0x2u, 0x2u);
}
void GAMEUTIL_forward_timer_still_one_ratio_time_interp_ratio_cntrl(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t p4)
{
    if (!ckBegin(297, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    ref::GAMEUTIL_forward_timer_still_one_ratio_time_interp_ratio_cntrl(rdram, &c, runtime);
    ckMid();
    const float r = ty::GAMEUTIL_forward_timer_still_one_ratio_time_interp_ratio_cntrl(rdram, runtime, p4);
    ckEnd(297, ctx, c, 3, 0u, r, 0x0u, 0x2u);
}
void GAMEUTIL_get_interp_ratio_interp_ratio_cntrl(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t p4)
{
    if (!ckBegin(298, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    ref::GAMEUTIL_get_interp_ratio_interp_ratio_cntrl(rdram, &c, runtime);
    ckMid();
    const float r = ty::GAMEUTIL_get_interp_ratio_interp_ratio_cntrl(rdram, runtime, p4);
    ckEnd(298, ctx, c, 3, 0u, r, 0x0u, 0x0u);
}
void GAMEUTIL_get_interp_timer_interp_ratio_cntrl(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t p4)
{
    if (!ckBegin(299, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    ref::GAMEUTIL_get_interp_timer_interp_ratio_cntrl(rdram, &c, runtime);
    ckMid();
    const float r = ty::GAMEUTIL_get_interp_timer_interp_ratio_cntrl(rdram, runtime, p4);
    ckEnd(299, ctx, c, 3, 0u, r, 0x0u, 0x0u);
}
void GAMEUTIL_set_act_interp_flag_interp_ratio_cntrl(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t p4, uint32_t p5)
{
    if (!ckBegin(300, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    ref::GAMEUTIL_set_act_interp_flag_interp_ratio_cntrl(rdram, &c, runtime);
    ckMid();
    ty::GAMEUTIL_set_act_interp_flag_interp_ratio_cntrl(rdram, runtime, p4, p5);
    ckEnd(300, ctx, c, 0, 0u, 0.0f, 0x0u, 0x0u);
}
void GAMEUTIL_set_act_max_zero_clip_flag_interp_ratio_cntrl(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t p4, uint32_t p5)
{
    if (!ckBegin(301, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    ref::GAMEUTIL_set_act_max_zero_clip_flag_interp_ratio_cntrl(rdram, &c, runtime);
    ckMid();
    ty::GAMEUTIL_set_act_max_zero_clip_flag_interp_ratio_cntrl(rdram, runtime, p4, p5);
    ckEnd(301, ctx, c, 0, 0u, 0.0f, 0x0u, 0x0u);
}
void GAMEUTIL_get_interp_value_interp_value_cntrl(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t p4)
{
    if (!ckBegin(302, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    ref::GAMEUTIL_get_interp_value_interp_value_cntrl(rdram, &c, runtime);
    ckMid();
    const float r = ty::GAMEUTIL_get_interp_value_interp_value_cntrl(rdram, runtime, p4);
    ckEnd(302, ctx, c, 3, 0u, r, 0x0u, 0x0u);
}
void GAMEUTIL_get_hope_value_interp_value_cntrl(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t p4)
{
    if (!ckBegin(303, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    ref::GAMEUTIL_get_hope_value_interp_value_cntrl(rdram, &c, runtime);
    ckMid();
    const float r = ty::GAMEUTIL_get_hope_value_interp_value_cntrl(rdram, runtime, p4);
    ckEnd(303, ctx, c, 3, 0u, r, 0x0u, 0x0u);
}
void set_param_double_spring_interp_common(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t p4, float pf12, float pf13, float pf14, float pf15)
{
    if (!ckBegin(304, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    ref::set_param_double_spring_interp_common(rdram, &c, runtime);
    ckMid();
    ty::set_param_double_spring_interp_common(rdram, runtime, p4, pf12, pf13, pf14, pf15);
    ckEnd(304, ctx, c, 0, 0u, 0.0f, 0x0u, 0x0u);
}
void set_rot_radi_value_flag_double_spring_interp_cntrl(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t p4, uint32_t p5)
{
    if (!ckBegin(305, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    ref::set_rot_radi_value_flag_double_spring_interp_cntrl(rdram, &c, runtime);
    ckMid();
    ty::set_rot_radi_value_flag_double_spring_interp_cntrl(rdram, runtime, p4, p5);
    ckEnd(305, ctx, c, 0, 0u, 0.0f, 0x0u, 0x0u);
}
void set_rot_radi_value_flag_double_spring_interp_cntrl3D(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t p4, uint32_t p5, uint32_t p6, uint32_t p7)
{
    if (!ckBegin(306, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    ref::set_rot_radi_value_flag_double_spring_interp_cntrl3D(rdram, &c, runtime);
    ckMid();
    ty::set_rot_radi_value_flag_double_spring_interp_cntrl3D(rdram, runtime, p4, p5, p6, p7);
    ckEnd(306, ctx, c, 0, 0u, 0.0f, 0x0u, 0x0u);
}
void constructor_aim_angle_interp_cntrl(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t p4, uint32_t p5)
{
    if (!ckBegin(307, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    ref::constructor_aim_angle_interp_cntrl(rdram, &c, runtime);
    ckMid();
    const float r = ty::constructor_aim_angle_interp_cntrl(rdram, runtime, p4, p5);
    ckEnd(307, ctx, c, 3, 0u, r, 0x2u, 0x2u);
}
void reset_aim_angle_interp_cntrl(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t p4, float pf12)
{
    if (!ckBegin(308, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    ref::reset_aim_angle_interp_cntrl(rdram, &c, runtime);
    ckMid();
    const float r = ty::reset_aim_angle_interp_cntrl(rdram, runtime, p4, pf12);
    ckEnd(308, ctx, c, 3, 0u, r, 0x0u, 0x0u);
}
void set_interp_param_aim_angle_interp_cntrl(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t p4, float pf12, float pf13)
{
    if (!ckBegin(309, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    ref::set_interp_param_aim_angle_interp_cntrl(rdram, &c, runtime);
    ckMid();
    ty::set_interp_param_aim_angle_interp_cntrl(rdram, runtime, p4, pf12, pf13);
    ckEnd(309, ctx, c, 0, 0u, 0.0f, 0x0u, 0x0u);
}
void manager_sub_01084978(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t p4, float pf12, float pf13)
{
    if (!ckBegin(310, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    ref::manager_sub_01084978(rdram, &c, runtime);
    ckMid();
    const a1::RetVF r = ty::manager_sub_01084978(rdram, runtime, p4, pf12, pf13);
    ckEnd(310, ctx, c, 4, r.v0, r.f0, 0x0u, 0x2u);
}
void getNowBossEffect(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (!ckBegin(311, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    ref::getNowBossEffect(rdram, &c, runtime);
    ckMid();
    const uint64_t r = a1_sx64(ty::getNowBossEffect(rdram, runtime));
    ckEnd(311, ctx, c, 1, r, 0.0f, 0x8u, 0x0u);
}
void setBossDeathEffect(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint64_t p4, uint64_t p5, float pf12)
{
    if (!ckBegin(312, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    ref::setBossDeathEffect(rdram, &c, runtime);
    ckMid();
    const a1::RetVF r = ty::setBossDeathEffect(rdram, runtime, p4, p5, pf12);
    ckEnd(312, ctx, c, 4, r.v0, r.f0, 0x3dau, 0x2u);
}
void getBossEffectBlurLevel(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (!ckBegin(313, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    ref::getBossEffectBlurLevel(rdram, &c, runtime);
    ckMid();
    const uint64_t r = a1_sx64(ty::getBossEffectBlurLevel(rdram, runtime));
    ckEnd(313, ctx, c, 1, r, 0.0f, 0x0u, 0x0u);
}
void getBossEffectColorR(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (!ckBegin(314, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    ref::getBossEffectColorR(rdram, &c, runtime);
    ckMid();
    const uint64_t r = a1_sx64(ty::getBossEffectColorR(rdram, runtime));
    ckEnd(314, ctx, c, 1, r, 0.0f, 0x0u, 0x0u);
}
void getBossEffectColorG(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (!ckBegin(315, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    ref::getBossEffectColorG(rdram, &c, runtime);
    ckMid();
    const uint64_t r = a1_sx64(ty::getBossEffectColorG(rdram, runtime));
    ckEnd(315, ctx, c, 1, r, 0.0f, 0x0u, 0x0u);
}
void getBossEffectColorB(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (!ckBegin(316, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    ref::getBossEffectColorB(rdram, &c, runtime);
    ckMid();
    const uint64_t r = a1_sx64(ty::getBossEffectColorB(rdram, runtime));
    ckEnd(316, ctx, c, 1, r, 0.0f, 0x0u, 0x0u);
}
void getBossEffectBlendRatio(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (!ckBegin(317, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    ref::getBossEffectBlendRatio(rdram, &c, runtime);
    ckMid();
    const uint64_t r = a1_sx64(ty::getBossEffectBlendRatio(rdram, runtime));
    ckEnd(317, ctx, c, 1, r, 0.0f, 0x0u, 0x0u);
}
void getBossEffectOutLine(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (!ckBegin(318, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    ref::getBossEffectOutLine(rdram, &c, runtime);
    ckMid();
    const uint64_t r = a1_sx64(ty::getBossEffectOutLine(rdram, runtime));
    ckEnd(318, ctx, c, 1, r, 0.0f, 0x0u, 0x0u);
}
void getDepthOfFieldSw(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (!ckBegin(319, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    ref::getDepthOfFieldSw(rdram, &c, runtime);
    ckMid();
    const uint64_t r = a1_sx64(ty::getDepthOfFieldSw(rdram, runtime));
    ckEnd(319, ctx, c, 1, r, 0.0f, 0x0u, 0x0u);
}
void getDepthOfFieldLevel(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (!ckBegin(320, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    ref::getDepthOfFieldLevel(rdram, &c, runtime);
    ckMid();
    const uint64_t r = a1_sx64(ty::getDepthOfFieldLevel(rdram, runtime));
    ckEnd(320, ctx, c, 1, r, 0.0f, 0x0u, 0x0u);
}
void getDepthOfFieldPlanes(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (!ckBegin(321, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    ref::getDepthOfFieldPlanes(rdram, &c, runtime);
    ckMid();
    const uint64_t r = a1_sx64(ty::getDepthOfFieldPlanes(rdram, runtime));
    ckEnd(321, ctx, c, 1, r, 0.0f, 0x0u, 0x0u);
}
void getDepthOfFieldNearZ(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (!ckBegin(322, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    ref::getDepthOfFieldNearZ(rdram, &c, runtime);
    ckMid();
    const uint64_t r = a1_sx64(ty::getDepthOfFieldNearZ(rdram, runtime));
    ckEnd(322, ctx, c, 1, r, 0.0f, 0x0u, 0x0u);
}
void fsEffectSetParamSheetSeg(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint64_t p4)
{
    if (!ckBegin(323, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    ref::fsEffectSetParamSheetSeg(rdram, &c, runtime);
    ckMid();
    const uint64_t r = a1_sx64(ty::fsEffectSetParamSheetSeg(rdram, runtime, p4));
    ckEnd(323, ctx, c, 1, r, 0.0f, 0x78u, 0x0u);
}
void getFogMode(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (!ckBegin(324, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    ref::getFogMode(rdram, &c, runtime);
    ckMid();
    const uint64_t r = a1_sx64(ty::getFogMode(rdram, runtime));
    ckEnd(324, ctx, c, 1, r, 0.0f, 0x0u, 0x0u);
}
void getFogNearDist(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (!ckBegin(325, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    ref::getFogNearDist(rdram, &c, runtime);
    ckMid();
    const uint64_t r = a1_sx64(ty::getFogNearDist(rdram, runtime));
    ckEnd(325, ctx, c, 1, r, 0.0f, 0x0u, 0x0u);
}
void getFogFarDist(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (!ckBegin(326, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    ref::getFogFarDist(rdram, &c, runtime);
    ckMid();
    const uint64_t r = a1_sx64(ty::getFogFarDist(rdram, runtime));
    ckEnd(326, ctx, c, 1, r, 0.0f, 0x0u, 0x0u);
}
void getFogNearRate(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (!ckBegin(327, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    ref::getFogNearRate(rdram, &c, runtime);
    ckMid();
    const uint64_t r = a1_sx64(ty::getFogNearRate(rdram, runtime));
    ckEnd(327, ctx, c, 1, r, 0.0f, 0x0u, 0x0u);
}
void getFogFarRate(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (!ckBegin(328, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    ref::getFogFarRate(rdram, &c, runtime);
    ckMid();
    const uint64_t r = a1_sx64(ty::getFogFarRate(rdram, runtime));
    ckEnd(328, ctx, c, 1, r, 0.0f, 0x0u, 0x0u);
}
void getFogOffset(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (!ckBegin(329, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    ref::getFogOffset(rdram, &c, runtime);
    ckMid();
    const uint64_t r = a1_sx64(ty::getFogOffset(rdram, runtime));
    ckEnd(329, ctx, c, 1, r, 0.0f, 0x0u, 0x0u);
}
void getFogUnderWater(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (!ckBegin(330, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    ref::getFogUnderWater(rdram, &c, runtime);
    ckMid();
    const uint64_t r = a1_sx64(ty::getFogUnderWater(rdram, runtime));
    ckEnd(330, ctx, c, 1, r, 0.0f, 0x0u, 0x0u);
}
void getFogColorR(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (!ckBegin(331, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    ref::getFogColorR(rdram, &c, runtime);
    ckMid();
    const uint64_t r = a1_sx64(ty::getFogColorR(rdram, runtime));
    ckEnd(331, ctx, c, 1, r, 0.0f, 0x0u, 0x0u);
}
void getFogColorG(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (!ckBegin(332, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    ref::getFogColorG(rdram, &c, runtime);
    ckMid();
    const uint64_t r = a1_sx64(ty::getFogColorG(rdram, runtime));
    ckEnd(332, ctx, c, 1, r, 0.0f, 0x0u, 0x0u);
}
void getFogColorB(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (!ckBegin(333, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    ref::getFogColorB(rdram, &c, runtime);
    ckMid();
    const uint64_t r = a1_sx64(ty::getFogColorB(rdram, runtime));
    ckEnd(333, ctx, c, 1, r, 0.0f, 0x0u, 0x0u);
}
void SeemlessInitFogTargetParamWorldLayout(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t p4)
{
    if (!ckBegin(334, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    ref::SeemlessInitFogTargetParamWorldLayout(rdram, &c, runtime);
    ckMid();
    const a1::RetVF r = ty::SeemlessInitFogTargetParamWorldLayout(rdram, runtime, p4);
    ckEnd(334, ctx, c, 4, r.v0, r.f0, 0xeau, 0x6u);
}
void fogSetFogColorMulRatio(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, float pf12)
{
    if (!ckBegin(335, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    ref::fogSetFogColorMulRatio(rdram, &c, runtime);
    ckMid();
    const uint64_t r = a1_sx64(ty::fogSetFogColorMulRatio(rdram, runtime, pf12));
    ckEnd(335, ctx, c, 1, r, 0.0f, 0x0u, 0x0u);
}
void getFogFactor1(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (!ckBegin(336, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    ref::getFogFactor1(rdram, &c, runtime);
    ckMid();
    const a1::RetVF r = ty::getFogFactor1(rdram, runtime);
    ckEnd(336, ctx, c, 4, r.v0, r.f0, 0x0u, 0x3eu);
}
void setFadeColor(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint64_t p4)
{
    if (!ckBegin(337, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    ref::setFadeColor(rdram, &c, runtime);
    ckMid();
    const uint64_t r = a1_sx64(ty::setFadeColor(rdram, runtime, p4));
    ckEnd(337, ctx, c, 1, r, 0.0f, 0x8u, 0x0u);
}
void setFadeLayer(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t p4)
{
    if (!ckBegin(338, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    ref::setFadeLayer(rdram, &c, runtime);
    ckMid();
    const uint64_t r = a1_sx64(ty::setFadeLayer(rdram, runtime, p4));
    ckEnd(338, ctx, c, 1, r, 0.0f, 0x0u, 0x0u);
}
void getCameraBlurSw(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (!ckBegin(339, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    ref::getCameraBlurSw(rdram, &c, runtime);
    ckMid();
    const uint64_t r = a1_sx64(ty::getCameraBlurSw(rdram, runtime));
    ckEnd(339, ctx, c, 1, r, 0.0f, 0x0u, 0x0u);
}
void getBlurAlsoChara(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (!ckBegin(340, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    ref::getBlurAlsoChara(rdram, &c, runtime);
    ckMid();
    const uint64_t r = a1_sx64(ty::getBlurAlsoChara(rdram, runtime));
    ckEnd(340, ctx, c, 1, r, 0.0f, 0x0u, 0x0u);
}
void getBlurWorkSize(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (!ckBegin(341, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    ref::getBlurWorkSize(rdram, &c, runtime);
    ckMid();
    const uint64_t r = a1_sx64(ty::getBlurWorkSize(rdram, runtime));
    ckEnd(341, ctx, c, 1, r, 0.0f, 0x0u, 0x0u);
}
void getNBlurPlanes(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (!ckBegin(342, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    ref::getNBlurPlanes(rdram, &c, runtime);
    ckMid();
    const uint64_t r = a1_sx64(ty::getNBlurPlanes(rdram, runtime));
    ckEnd(342, ctx, c, 1, r, 0.0f, 0x0u, 0x0u);
}
void getBlurIntensity(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (!ckBegin(343, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    ref::getBlurIntensity(rdram, &c, runtime);
    ckMid();
    const uint64_t r = a1_sx64(ty::getBlurIntensity(rdram, runtime));
    ckEnd(343, ctx, c, 1, r, 0.0f, 0x0u, 0x0u);
}
void getBlurZBase(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (!ckBegin(344, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    ref::getBlurZBase(rdram, &c, runtime);
    ckMid();
    const uint64_t r = a1_sx64(ty::getBlurZBase(rdram, runtime));
    ckEnd(344, ctx, c, 1, r, 0.0f, 0x0u, 0x0u);
}
void getBlurMinThreshold(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (!ckBegin(345, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    ref::getBlurMinThreshold(rdram, &c, runtime);
    ckMid();
    const uint64_t r = a1_sx64(ty::getBlurMinThreshold(rdram, runtime));
    ckEnd(345, ctx, c, 1, r, 0.0f, 0x0u, 0x0u);
}
void getBlurMaxThreshold(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (!ckBegin(346, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    ref::getBlurMaxThreshold(rdram, &c, runtime);
    ckMid();
    const uint64_t r = a1_sx64(ty::getBlurMaxThreshold(rdram, runtime));
    ckEnd(346, ctx, c, 1, r, 0.0f, 0x0u, 0x0u);
}
void getBlurShutterSpeed(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (!ckBegin(347, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    ref::getBlurShutterSpeed(rdram, &c, runtime);
    ckMid();
    const uint64_t r = a1_sx64(ty::getBlurShutterSpeed(rdram, runtime));
    ckEnd(347, ctx, c, 1, r, 0.0f, 0x0u, 0x0u);
}
void getBlurXYZRatio(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (!ckBegin(348, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    ref::getBlurXYZRatio(rdram, &c, runtime);
    ckMid();
    const uint64_t r = a1_sx64(ty::getBlurXYZRatio(rdram, runtime));
    ckEnd(348, ctx, c, 1, r, 0.0f, 0x0u, 0x0u);
}
void getBlurCamMoveThreshold(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (!ckBegin(349, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    ref::getBlurCamMoveThreshold(rdram, &c, runtime);
    ckMid();
    const uint64_t r = a1_sx64(ty::getBlurCamMoveThreshold(rdram, runtime));
    ckEnd(349, ctx, c, 1, r, 0.0f, 0x0u, 0x0u);
}
void glTexCoord2f(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, float pf12, float pf13)
{
    if (!ckBegin(350, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    ref::glTexCoord2f(rdram, &c, runtime);
    ckMid();
    const a1::RetVF r = ty::glTexCoord2f(rdram, runtime, pf12, pf13);
    ckEnd(350, ctx, c, 4, r.v0, r.f0, 0x2u, 0x0u);
}
void glTexCoord2fv(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t p4)
{
    if (!ckBegin(351, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    ref::glTexCoord2fv(rdram, &c, runtime);
    ckMid();
    const a1::RetVF r = ty::glTexCoord2fv(rdram, runtime, p4);
    ckEnd(351, ctx, c, 4, r.v0, r.f0, 0x2u, 0x6u);
}
void glZOffset(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, float pf12)
{
    if (!ckBegin(352, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    ref::glZOffset(rdram, &c, runtime);
    ckMid();
    const uint64_t r = a1_sx64(ty::glZOffset(rdram, runtime, pf12));
    ckEnd(352, ctx, c, 1, r, 0.0f, 0x0u, 0x0u);
}
void glSetFogFactorDirect(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t p4)
{
    if (!ckBegin(353, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    ref::glSetFogFactorDirect(rdram, &c, runtime);
    ckMid();
    const uint64_t r = a1_sx64(ty::glSetFogFactorDirect(rdram, runtime, p4));
    ckEnd(353, ctx, c, 1, r, 0.0f, 0x0u, 0x0u);
}
void gluGetBrightness(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t p4)
{
    if (!ckBegin(354, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    ref::gluGetBrightness(rdram, &c, runtime);
    ckMid();
    const float r = ty::gluGetBrightness(rdram, runtime, p4);
    ckEnd(354, ctx, c, 3, 0u, r, 0x2u, 0x1eu);
}
void getMBlur(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (!ckBegin(355, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    ref::getMBlur(rdram, &c, runtime);
    ckMid();
    const uint64_t r = a1_sx64(ty::getMBlur(rdram, runtime));
    ckEnd(355, ctx, c, 1, r, 0.0f, 0x0u, 0x0u);
}
void getBrightness(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (!ckBegin(356, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    ref::getBrightness(rdram, &c, runtime);
    ckMid();
    const uint64_t r = a1_sx64(ty::getBrightness(rdram, runtime));
    ckEnd(356, ctx, c, 1, r, 0.0f, 0x0u, 0x0u);
}
void kSetTex(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t p4)
{
    if (!ckBegin(357, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    ref::kSetTex(rdram, &c, runtime);
    ckMid();
    const uint64_t r = a1_sx64(ty::kSetTex(rdram, runtime, p4));
    ckEnd(357, ctx, c, 1, r, 0.0f, 0x0u, 0x0u);
}
void getSpecularity(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (!ckBegin(358, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    ref::getSpecularity(rdram, &c, runtime);
    ckMid();
    const uint64_t r = a1_sx64(ty::getSpecularity(rdram, runtime));
    ckEnd(358, ctx, c, 1, r, 0.0f, 0x0u, 0x0u);
}
void modelDeleteDispModelClass(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint64_t p4)
{
    if (!ckBegin(359, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    ref::modelDeleteDispModelClass(rdram, &c, runtime);
    ckMid();
    const uint64_t r = (ty::modelDeleteDispModelClass(rdram, runtime, p4));
    ckEnd(359, ctx, c, 2, r, 0.0f, 0x38u, 0x0u);
}
void modelGetWorkSize(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t p4)
{
    if (!ckBegin(360, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    ref::modelGetWorkSize(rdram, &c, runtime);
    ckMid();
    const uint64_t r = a1_sx64(ty::modelGetWorkSize(rdram, runtime, p4));
    ckEnd(360, ctx, c, 1, r, 0.0f, 0x28u, 0x0u);
}
void SetDisplayModelNearClippingSwitch(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t p4)
{
    if (!ckBegin(361, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    ref::SetDisplayModelNearClippingSwitch(rdram, &c, runtime);
    ckMid();
    const uint64_t r = a1_sx64(ty::SetDisplayModelNearClippingSwitch(rdram, runtime, p4));
    ckEnd(361, ctx, c, 1, r, 0.0f, 0x0u, 0x0u);
}
void ptclmanGet(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (!ckBegin(362, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    ref::ptclmanGet(rdram, &c, runtime);
    ckMid();
    const uint64_t r = a1_sx64(ty::ptclmanGet(rdram, runtime));
    ckEnd(362, ctx, c, 1, r, 0.0f, 0x18u, 0x0u);
}
void ptclmanPut(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t p4)
{
    if (!ckBegin(363, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    ref::ptclmanPut(rdram, &c, runtime);
    ckMid();
    const uint64_t r = a1_sx64(ty::ptclmanPut(rdram, runtime, p4));
    ckEnd(363, ctx, c, 1, r, 0.0f, 0x8u, 0x0u);
}
void ptclCounterGet(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (!ckBegin(364, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    ref::ptclCounterGet(rdram, &c, runtime);
    ckMid();
    const uint64_t r = (ty::ptclCounterGet(rdram, runtime));
    ckEnd(364, ctx, c, 2, r, 0.0f, 0x0u, 0x0u);
}
void getRefBlurLevel(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (!ckBegin(365, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    ref::getRefBlurLevel(rdram, &c, runtime);
    ckMid();
    const uint64_t r = a1_sx64(ty::getRefBlurLevel(rdram, runtime));
    ckEnd(365, ctx, c, 1, r, 0.0f, 0x0u, 0x0u);
}
void getIsInWater(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (!ckBegin(366, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    ref::getIsInWater(rdram, &c, runtime);
    ckMid();
    const uint64_t r = a1_sx64(ty::getIsInWater(rdram, runtime));
    ckEnd(366, ctx, c, 1, r, 0.0f, 0x0u, 0x0u);
}
void getRefractBlurSpeed(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (!ckBegin(367, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    ref::getRefractBlurSpeed(rdram, &c, runtime);
    ckMid();
    const uint64_t r = a1_sx64(ty::getRefractBlurSpeed(rdram, runtime));
    ckEnd(367, ctx, c, 1, r, 0.0f, 0x0u, 0x0u);
}
void getRefractBlurSpeedRand(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (!ckBegin(368, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    ref::getRefractBlurSpeedRand(rdram, &c, runtime);
    ckMid();
    const uint64_t r = a1_sx64(ty::getRefractBlurSpeedRand(rdram, runtime));
    ckEnd(368, ctx, c, 1, r, 0.0f, 0x0u, 0x0u);
}
void getRefractBlurLevel(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (!ckBegin(369, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    ref::getRefractBlurLevel(rdram, &c, runtime);
    ckMid();
    const uint64_t r = a1_sx64(ty::getRefractBlurLevel(rdram, runtime));
    ckEnd(369, ctx, c, 1, r, 0.0f, 0x0u, 0x0u);
}
void getRefractBlurLevelRand(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (!ckBegin(370, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    ref::getRefractBlurLevelRand(rdram, &c, runtime);
    ckMid();
    const uint64_t r = a1_sx64(ty::getRefractBlurLevelRand(rdram, runtime));
    ckEnd(370, ctx, c, 1, r, 0.0f, 0x0u, 0x0u);
}
void getRefractLengthRatio(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (!ckBegin(371, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    ref::getRefractLengthRatio(rdram, &c, runtime);
    ckMid();
    const uint64_t r = a1_sx64(ty::getRefractLengthRatio(rdram, runtime));
    ckEnd(371, ctx, c, 1, r, 0.0f, 0x0u, 0x0u);
}
void getRefractBlendRatio(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (!ckBegin(372, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    ref::getRefractBlendRatio(rdram, &c, runtime);
    ckMid();
    const uint64_t r = a1_sx64(ty::getRefractBlendRatio(rdram, runtime));
    ckEnd(372, ctx, c, 1, r, 0.0f, 0x0u, 0x0u);
}
void getRefractBrightness(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (!ckBegin(373, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    ref::getRefractBrightness(rdram, &c, runtime);
    ckMid();
    const uint64_t r = a1_sx64(ty::getRefractBrightness(rdram, runtime));
    ckEnd(373, ctx, c, 1, r, 0.0f, 0x0u, 0x0u);
}
void getShadowIntensity(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (!ckBegin(374, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    ref::getShadowIntensity(rdram, &c, runtime);
    ckMid();
    const uint64_t r = a1_sx64(ty::getShadowIntensity(rdram, runtime));
    ckEnd(374, ctx, c, 1, r, 0.0f, 0x0u, 0x0u);
}
void GetShadowLength(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t p4)
{
    if (!ckBegin(375, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    ref::GetShadowLength(rdram, &c, runtime);
    ckMid();
    const float r = ty::GetShadowLength(rdram, runtime, p4);
    ckEnd(375, ctx, c, 3, 0u, r, 0x0u, 0x0u);
}
void shadowModelReInit(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint64_t p4)
{
    if (!ckBegin(376, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    ref::shadowModelReInit(rdram, &c, runtime);
    ckMid();
    const uint64_t r = (ty::shadowModelReInit(rdram, runtime, p4));
    ckEnd(376, ctx, c, 2, r, 0.0f, 0x68u, 0x0u);
}
void getSolarFlareSw(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (!ckBegin(377, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    ref::getSolarFlareSw(rdram, &c, runtime);
    ckMid();
    const uint64_t r = a1_sx64(ty::getSolarFlareSw(rdram, runtime));
    ckEnd(377, ctx, c, 1, r, 0.0f, 0x0u, 0x0u);
}
void getSolarFlareBlur(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (!ckBegin(378, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    ref::getSolarFlareBlur(rdram, &c, runtime);
    ckMid();
    const uint64_t r = a1_sx64(ty::getSolarFlareBlur(rdram, runtime));
    ckEnd(378, ctx, c, 1, r, 0.0f, 0x0u, 0x0u);
}
void getSolarFlareColorR(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (!ckBegin(379, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    ref::getSolarFlareColorR(rdram, &c, runtime);
    ckMid();
    const uint64_t r = a1_sx64(ty::getSolarFlareColorR(rdram, runtime));
    ckEnd(379, ctx, c, 1, r, 0.0f, 0x0u, 0x0u);
}
void getSolarFlareColorG(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (!ckBegin(380, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    ref::getSolarFlareColorG(rdram, &c, runtime);
    ckMid();
    const uint64_t r = a1_sx64(ty::getSolarFlareColorG(rdram, runtime));
    ckEnd(380, ctx, c, 1, r, 0.0f, 0x0u, 0x0u);
}
void getSolarFlareColorB(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (!ckBegin(381, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    ref::getSolarFlareColorB(rdram, &c, runtime);
    ckMid();
    const uint64_t r = a1_sx64(ty::getSolarFlareColorB(rdram, runtime));
    ckEnd(381, ctx, c, 1, r, 0.0f, 0x0u, 0x0u);
}
void getSolarFlareSolarLevel(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (!ckBegin(382, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    ref::getSolarFlareSolarLevel(rdram, &c, runtime);
    ckMid();
    const uint64_t r = a1_sx64(ty::getSolarFlareSolarLevel(rdram, runtime));
    ckEnd(382, ctx, c, 1, r, 0.0f, 0x0u, 0x0u);
}
void getSolarGlowLevel(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (!ckBegin(383, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    ref::getSolarGlowLevel(rdram, &c, runtime);
    ckMid();
    const uint64_t r = a1_sx64(ty::getSolarGlowLevel(rdram, runtime));
    ckEnd(383, ctx, c, 1, r, 0.0f, 0x0u, 0x0u);
}
void getSolarFlareSamplePix(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (!ckBegin(384, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    ref::getSolarFlareSamplePix(rdram, &c, runtime);
    ckMid();
    const uint64_t r = a1_sx64(ty::getSolarFlareSamplePix(rdram, runtime));
    ckEnd(384, ctx, c, 1, r, 0.0f, 0x0u, 0x0u);
}
void manager_sub_010A2040(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t p4)
{
    if (!ckBegin(385, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    ref::manager_sub_010A2040(rdram, &c, runtime);
    ckMid();
    const a1::RetVF r = ty::manager_sub_010A2040(rdram, runtime, p4);
    ckEnd(385, ctx, c, 4, r.v0, r.f0, 0x2u, 0x3ffeu);
}
void spriteSetPosition(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t p4, float pf12, float pf13)
{
    if (!ckBegin(386, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    ref::spriteSetPosition(rdram, &c, runtime);
    ckMid();
    ty::spriteSetPosition(rdram, runtime, p4, pf12, pf13);
    ckEnd(386, ctx, c, 0, 0u, 0.0f, 0x0u, 0x0u);
}
void spriteSetTextureCoordinateSTFromUV(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint64_t p4, float pf12, float pf13, float pf14, float pf15)
{
    if (!ckBegin(387, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    ref::spriteSetTextureCoordinateSTFromUV(rdram, &c, runtime);
    ckMid();
    const a1::RetVF r = ty::spriteSetTextureCoordinateSTFromUV(rdram, runtime, p4, pf12, pf13, pf14, pf15);
    ckEnd(387, ctx, c, 4, r.v0, r.f0, 0xeau, 0xf00eu);
}
void spriteSetTextureCoordinateUV(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint64_t p4, float pf12, float pf13, float pf14, float pf15)
{
    if (!ckBegin(388, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    ref::spriteSetTextureCoordinateUV(rdram, &c, runtime);
    ckMid();
    const a1::RetVF r = ty::spriteSetTextureCoordinateUV(rdram, runtime, p4, pf12, pf13, pf14, pf15);
    ckEnd(388, ctx, c, 4, r.v0, r.f0, 0x8u, 0x6u);
}
void spriteSetColor(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t p4, uint32_t p5, uint32_t p6, uint32_t p7, uint32_t p8)
{
    if (!ckBegin(389, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    ref::spriteSetColor(rdram, &c, runtime);
    ckMid();
    ty::spriteSetColor(rdram, runtime, p4, p5, p6, p7, p8);
    ckEnd(389, ctx, c, 0, 0u, 0.0f, 0x0u, 0x0u);
}
void spriteSetScale(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t p4, float pf12, float pf13)
{
    if (!ckBegin(390, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    ref::spriteSetScale(rdram, &c, runtime);
    ckMid();
    ty::spriteSetScale(rdram, runtime, p4, pf12, pf13);
    ckEnd(390, ctx, c, 0, 0u, 0.0f, 0x0u, 0x0u);
}
void spriteSetRotation(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t p4, float pf12)
{
    if (!ckBegin(391, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    ref::spriteSetRotation(rdram, &c, runtime);
    ckMid();
    ty::spriteSetRotation(rdram, runtime, p4, pf12);
    ckEnd(391, ctx, c, 0, 0u, 0.0f, 0x0u, 0x0u);
}
void spriteSetFade(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t p4, float pf12)
{
    if (!ckBegin(392, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    ref::spriteSetFade(rdram, &c, runtime);
    ckMid();
    ty::spriteSetFade(rdram, runtime, p4, pf12);
    ckEnd(392, ctx, c, 0, 0u, 0.0f, 0x0u, 0x0u);
}
void spriteSetPriority(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t p4, uint32_t p5)
{
    if (!ckBegin(393, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    ref::spriteSetPriority(rdram, &c, runtime);
    ckMid();
    ty::spriteSetPriority(rdram, runtime, p4, p5);
    ckEnd(393, ctx, c, 0, 0u, 0.0f, 0x0u, 0x0u);
}
void manager_sub_010ABC48(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint64_t p4)
{
    if (!ckBegin(394, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    ref::manager_sub_010ABC48(rdram, &c, runtime);
    ckMid();
    const uint64_t r = (ty::manager_sub_010ABC48(rdram, runtime, p4));
    ckEnd(394, ctx, c, 2, r, 0.0f, 0x1ff8u, 0x0u);
}
void getZFogDivide(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (!ckBegin(395, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    ref::getZFogDivide(rdram, &c, runtime);
    ckMid();
    const uint64_t r = a1_sx64(ty::getZFogDivide(rdram, runtime));
    ckEnd(395, ctx, c, 1, r, 0.0f, 0x0u, 0x0u);
}
void getZFogScissorMargin(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (!ckBegin(396, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    ref::getZFogScissorMargin(rdram, &c, runtime);
    ckMid();
    const uint64_t r = a1_sx64(ty::getZFogScissorMargin(rdram, runtime));
    ckEnd(396, ctx, c, 1, r, 0.0f, 0x0u, 0x0u);
}
void setZFogFar(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, float pf12)
{
    if (!ckBegin(397, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    ref::setZFogFar(rdram, &c, runtime);
    ckMid();
    const uint64_t r = a1_sx64(ty::setZFogFar(rdram, runtime, pf12));
    ckEnd(397, ctx, c, 1, r, 0.0f, 0x0u, 0x0u);
}
void getZFogNear(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (!ckBegin(398, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    ref::getZFogNear(rdram, &c, runtime);
    ckMid();
    const a1::RetVF r = ty::getZFogNear(rdram, runtime);
    ckEnd(398, ctx, c, 4, r.v0, r.f0, 0x18u, 0x6u);
}
void GetObjectAnimationParentNodeId(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t p4, uint32_t p5)
{
    if (!ckBegin(399, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    ref::GetObjectAnimationParentNodeId(rdram, &c, runtime);
    ckMid();
    const uint64_t r = a1_sx64(ty::GetObjectAnimationParentNodeId(rdram, runtime, p4, p5));
    ckEnd(399, ctx, c, 1, r, 0.0f, 0x28u, 0x0u);
}
void IsFullFixFormat(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint64_t p4)
{
    if (!ckBegin(400, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    ref::IsFullFixFormat(rdram, &c, runtime);
    ckMid();
    const uint64_t r = (ty::IsFullFixFormat(rdram, runtime, p4));
    ckEnd(400, ctx, c, 2, r, 0.0f, 0x8u, 0x0u);
}
void GetMotionResourceRotAngle(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint64_t p4)
{
    if (!ckBegin(401, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    ref::GetMotionResourceRotAngle(rdram, &c, runtime);
    ckMid();
    const uint64_t r = (ty::GetMotionResourceRotAngle(rdram, runtime, p4));
    ckEnd(401, ctx, c, 2, r, 0.0f, 0x28u, 0x0u);
}
void constructor_motion_ik_base(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t p4)
{
    if (!ckBegin(402, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    ref::constructor_motion_ik_base(rdram, &c, runtime);
    ckMid();
    ty::constructor_motion_ik_base(rdram, runtime, p4);
    ckEnd(402, ctx, c, 0, 0u, 0.0f, 0x0u, 0x0u);
}
void init_motion_ik_base(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t p4)
{
    if (!ckBegin(403, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    ref::init_motion_ik_base(rdram, &c, runtime);
    ckMid();
    ty::init_motion_ik_base(rdram, runtime, p4);
    ckEnd(403, ctx, c, 0, 0u, 0.0f, 0x0u, 0x0u);
}
void doit_motion_ik_base(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t p4, uint32_t p5)
{
    if (!ckBegin(404, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    ref::doit_motion_ik_base(rdram, &c, runtime);
    ckMid();
    ty::doit_motion_ik_base(rdram, runtime, p4, p5);
    ckEnd(404, ctx, c, 0, 0u, 0.0f, 0x0u, 0x0u);
}
void manager_sub_010BA140(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t p4)
{
    if (!ckBegin(405, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    ref::manager_sub_010BA140(rdram, &c, runtime);
    ckMid();
    ty::manager_sub_010BA140(rdram, runtime, p4);
    ckEnd(405, ctx, c, 0, 0u, 0.0f, 0x0u, 0x0u);
}
void UpdateAnimationClassPastSimpleInfo(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t p4)
{
    if (!ckBegin(406, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    ref::UpdateAnimationClassPastSimpleInfo(rdram, &c, runtime);
    ckMid();
    const float r = ty::UpdateAnimationClassPastSimpleInfo(rdram, runtime, p4);
    ckEnd(406, ctx, c, 3, 0u, r, 0x0u, 0x0u);
}
void GetCharacterClassInitParamNbOptionNodes(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t p4)
{
    if (!ckBegin(407, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    ref::GetCharacterClassInitParamNbOptionNodes(rdram, &c, runtime);
    ckMid();
    const uint64_t r = a1_sx64(ty::GetCharacterClassInitParamNbOptionNodes(rdram, runtime, p4));
    ckEnd(407, ctx, c, 1, r, 0.0f, 0x8u, 0x0u);
}
void ResetCharacterClassWaterStatus(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t p4)
{
    if (!ckBegin(408, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    ref::ResetCharacterClassWaterStatus(rdram, &c, runtime);
    ckMid();
    ty::ResetCharacterClassWaterStatus(rdram, runtime, p4);
    ckEnd(408, ctx, c, 0, 0u, 0.0f, 0x0u, 0x0u);
}
void ResetCharacterClassMotInterpInfo(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t p4)
{
    if (!ckBegin(409, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    ref::ResetCharacterClassMotInterpInfo(rdram, &c, runtime);
    ckMid();
    const a1::RetVF r = ty::ResetCharacterClassMotInterpInfo(rdram, runtime, p4);
    ckEnd(409, ctx, c, 4, r.v0, r.f0, 0x2u, 0x2u);
}
void GetCharacterClassCurAnimationFrameRate(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t p4)
{
    if (!ckBegin(410, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    ref::GetCharacterClassCurAnimationFrameRate(rdram, &c, runtime);
    ckMid();
    const uint64_t r = (ty::GetCharacterClassCurAnimationFrameRate(rdram, runtime, p4));
    ckEnd(410, ctx, c, 2, r, 0.0f, 0x8u, 0x0u);
}
void SetCharacterClassGazeAdditionChannelInfo(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t p4, uint32_t p5, float pf12, float pf13)
{
    if (!ckBegin(411, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    ref::SetCharacterClassGazeAdditionChannelInfo(rdram, &c, runtime);
    ckMid();
    const uint64_t r = a1_sx64(ty::SetCharacterClassGazeAdditionChannelInfo(rdram, runtime, p4, p5, pf12, pf13));
    ckEnd(411, ctx, c, 1, r, 0.0f, 0x8u, 0x0u);
}
void ForceAcceptMotionControlChange(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t p4, uint64_t p5)
{
    if (!ckBegin(412, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    ref::ForceAcceptMotionControlChange(rdram, &c, runtime);
    ckMid();
    const uint64_t r = a1_sx64(ty::ForceAcceptMotionControlChange(rdram, runtime, p4, p5));
    ckEnd(412, ctx, c, 1, r, 0.0f, 0x0u, 0x0u);
}
void YattsukeSetPlayerR1ON(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t p4)
{
    if (!ckBegin(413, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    ref::YattsukeSetPlayerR1ON(rdram, &c, runtime);
    ckMid();
    const uint64_t r = a1_sx64(ty::YattsukeSetPlayerR1ON(rdram, runtime, p4));
    ckEnd(413, ctx, c, 1, r, 0.0f, 0x0u, 0x0u);
}
void RequestCharacterClassSlip(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t p4)
{
    if (!ckBegin(414, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    ref::RequestCharacterClassSlip(rdram, &c, runtime);
    ckMid();
    const uint64_t r = a1_sx64(ty::RequestCharacterClassSlip(rdram, runtime, p4));
    ckEnd(414, ctx, c, 1, r, 0.0f, 0x0u, 0x0u);
}
void RequestCharacterClassNoClimbableSlip(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t p4)
{
    if (!ckBegin(415, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    ref::RequestCharacterClassNoClimbableSlip(rdram, &c, runtime);
    ckMid();
    const uint64_t r = a1_sx64(ty::RequestCharacterClassNoClimbableSlip(rdram, runtime, p4));
    ckEnd(415, ctx, c, 1, r, 0.0f, 0x0u, 0x0u);
}
void RequestCharacterClassAdditionMotionChange(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t p4, uint32_t p5)
{
    if (!ckBegin(416, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    ref::RequestCharacterClassAdditionMotionChange(rdram, &c, runtime);
    ckMid();
    const uint64_t r = a1_sx64(ty::RequestCharacterClassAdditionMotionChange(rdram, runtime, p4, p5));
    ckEnd(416, ctx, c, 1, r, 0.0f, 0x48u, 0x0u);
}
void RequestCharacterClassMotionDirectionByGPTop(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t p4)
{
    if (!ckBegin(417, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    ref::RequestCharacterClassMotionDirectionByGPTop(rdram, &c, runtime);
    ckMid();
    const uint64_t r = a1_sx64(ty::RequestCharacterClassMotionDirectionByGPTop(rdram, runtime, p4));
    ckEnd(417, ctx, c, 1, r, 0.0f, 0x0u, 0x0u);
}
void RequestCharacterClassMotionDirectionByGPTail(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t p4)
{
    if (!ckBegin(418, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    ref::RequestCharacterClassMotionDirectionByGPTail(rdram, &c, runtime);
    ckMid();
    const uint64_t r = a1_sx64(ty::RequestCharacterClassMotionDirectionByGPTail(rdram, runtime, p4));
    ckEnd(418, ctx, c, 1, r, 0.0f, 0x0u, 0x0u);
}
void RequestCharacterClassMotionTurnLevel(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t p4, float pf12)
{
    if (!ckBegin(419, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    ref::RequestCharacterClassMotionTurnLevel(rdram, &c, runtime);
    ckMid();
    const uint64_t r = a1_sx64(ty::RequestCharacterClassMotionTurnLevel(rdram, runtime, p4, pf12));
    ckEnd(419, ctx, c, 1, r, 0.0f, 0x0u, 0x0u);
}
void RequestCharacterClassMotionPlaySpeedUp(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t p4)
{
    if (!ckBegin(420, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    ref::RequestCharacterClassMotionPlaySpeedUp(rdram, &c, runtime);
    ckMid();
    const uint64_t r = a1_sx64(ty::RequestCharacterClassMotionPlaySpeedUp(rdram, runtime, p4));
    ckEnd(420, ctx, c, 1, r, 0.0f, 0x0u, 0x0u);
}
void RequestCharacterClassMotionPlaySpeedDown(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t p4)
{
    if (!ckBegin(421, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    ref::RequestCharacterClassMotionPlaySpeedDown(rdram, &c, runtime);
    ckMid();
    const uint64_t r = a1_sx64(ty::RequestCharacterClassMotionPlaySpeedDown(rdram, runtime, p4));
    ckEnd(421, ctx, c, 1, r, 0.0f, 0x0u, 0x0u);
}
void RequestCharacterClassMotionPlaySpeedDown2(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t p4)
{
    if (!ckBegin(422, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    ref::RequestCharacterClassMotionPlaySpeedDown2(rdram, &c, runtime);
    ckMid();
    const uint64_t r = a1_sx64(ty::RequestCharacterClassMotionPlaySpeedDown2(rdram, runtime, p4));
    ckEnd(422, ctx, c, 1, r, 0.0f, 0x0u, 0x0u);
}
void RequestCharacterClassWindForceMove(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t p4)
{
    if (!ckBegin(423, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    ref::RequestCharacterClassWindForceMove(rdram, &c, runtime);
    ckMid();
    const uint64_t r = a1_sx64(ty::RequestCharacterClassWindForceMove(rdram, runtime, p4));
    ckEnd(423, ctx, c, 1, r, 0.0f, 0x0u, 0x0u);
}
void getTurnLimitInterp(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, float pf12, float pf13, float pf14, float pf15)
{
    if (!ckBegin(424, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    ref::getTurnLimitInterp(rdram, &c, runtime);
    ckMid();
    const float r = ty::getTurnLimitInterp(rdram, runtime, pf12, pf13, pf14, pf15);
    ckEnd(424, ctx, c, 3, 0u, r, 0x2u, 0x4002u);
}
void getRatioFromInterp(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, float pf12, float pf13)
{
    if (!ckBegin(425, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    ref::getRatioFromInterp(rdram, &c, runtime);
    ckMid();
    const float r = ty::getRatioFromInterp(rdram, runtime, pf12, pf13);
    ckEnd(425, ctx, c, 3, 0u, r, 0x2u, 0x2u);
}
void SetCharacterObjMotionJumpSpeedLimit(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t p5)
{
    if (!ckBegin(426, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    ref::SetCharacterObjMotionJumpSpeedLimit(rdram, &c, runtime);
    ckMid();
    const uint64_t r = a1_sx64(ty::SetCharacterObjMotionJumpSpeedLimit(rdram, runtime, p5));
    ckEnd(426, ctx, c, 1, r, 0.0f, 0x18u, 0x0u);
}
void charrootdyn_getUpperEdgeEffectiveHeight(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t p4)
{
    if (!ckBegin(427, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    ref::charrootdyn_getUpperEdgeEffectiveHeight(rdram, &c, runtime);
    ckMid();
    ty::charrootdyn_getUpperEdgeEffectiveHeight(rdram, runtime, p4);
    ckEnd(427, ctx, c, 0, 0u, 0.0f, 0x0u, 0x0u);
}
void charrootdyn_getNoLimitMode(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t p4, uint32_t p5)
{
    if (!ckBegin(428, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    ref::charrootdyn_getNoLimitMode(rdram, &c, runtime);
    ckMid();
    const uint64_t r = a1_sx64(ty::charrootdyn_getNoLimitMode(rdram, runtime, p4, p5));
    ckEnd(428, ctx, c, 1, r, 0.0f, 0x0u, 0x0u);
}
void charrootdyn_getForceNoLimitMode(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t p4)
{
    if (!ckBegin(429, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    ref::charrootdyn_getForceNoLimitMode(rdram, &c, runtime);
    ckMid();
    const uint64_t r = a1_sx64(ty::charrootdyn_getForceNoLimitMode(rdram, runtime, p4));
    ckEnd(429, ctx, c, 1, r, 0.0f, 0x0u, 0x0u);
}
void charrootdyn_isParentToEdge(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t p4)
{
    if (!ckBegin(430, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    ref::charrootdyn_isParentToEdge(rdram, &c, runtime);
    ckMid();
    const uint64_t r = (ty::charrootdyn_isParentToEdge(rdram, runtime, p4));
    ckEnd(430, ctx, c, 2, r, 0.0f, 0x8u, 0x0u);
}
void manager_sub_010DFD18(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t p6, uint64_t p5)
{
    if (!ckBegin(431, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    ref::manager_sub_010DFD18(rdram, &c, runtime);
    ckMid();
    const uint64_t r = a1_sx64(ty::manager_sub_010DFD18(rdram, runtime, p6, p5));
    ckEnd(431, ctx, c, 1, r, 0.0f, 0x0u, 0x0u);
}
void SetInitFogColClassFromStageData(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t p4)
{
    if (!ckBegin(432, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    ref::SetInitFogColClassFromStageData(rdram, &c, runtime);
    ckMid();
    const a1::RetVF r = ty::SetInitFogColClassFromStageData(rdram, runtime, p4);
    ckEnd(432, ctx, c, 4, r.v0, r.f0, 0x28u, 0x2u);
}
void SetFogColAvailableFlag(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t p4, uint32_t p5)
{
    if (!ckBegin(433, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    ref::SetFogColAvailableFlag(rdram, &c, runtime);
    ckMid();
    ty::SetFogColAvailableFlag(rdram, runtime, p4, p5);
    ckEnd(433, ctx, c, 0, 0u, 0.0f, 0x0u, 0x0u);
}
void IsEnvelopeFogType(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t p4)
{
    if (!ckBegin(434, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    ref::IsEnvelopeFogType(rdram, &c, runtime);
    ckMid();
    const uint64_t r = a1_sx64(ty::IsEnvelopeFogType(rdram, runtime, p4));
    ckEnd(434, ctx, c, 1, r, 0.0f, 0x0u, 0x0u);
}
void GetJudgeVolumeClassWorkSize(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint64_t p4)
{
    if (!ckBegin(435, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    ref::GetJudgeVolumeClassWorkSize(rdram, &c, runtime);
    ckMid();
    const uint64_t r = a1_sx64(ty::GetJudgeVolumeClassWorkSize(rdram, runtime, p4));
    ckEnd(435, ctx, c, 1, r, 0.0f, 0x18u, 0x0u);
}
void GetJudgeVolumeClassObjectGroup(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t p4, uint32_t p5)
{
    if (!ckBegin(436, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    ref::GetJudgeVolumeClassObjectGroup(rdram, &c, runtime);
    ckMid();
    const uint64_t r = a1_sx64(ty::GetJudgeVolumeClassObjectGroup(rdram, runtime, p4, p5));
    ckEnd(436, ctx, c, 1, r, 0.0f, 0x8u, 0x0u);
}
void GetJudgeVolumeClassObjectGroupById(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t p4, uint64_t p5)
{
    if (!ckBegin(437, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    ref::GetJudgeVolumeClassObjectGroupById(rdram, &c, runtime);
    ckMid();
    const uint64_t r = (ty::GetJudgeVolumeClassObjectGroupById(rdram, runtime, p4, p5));
    ckEnd(437, ctx, c, 2, r, 0.0f, 0x8u, 0x0u);
}
void SetLayoutClassCollisionWorkAdr(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t p4, uint32_t p5)
{
    if (!ckBegin(438, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    ref::SetLayoutClassCollisionWorkAdr(rdram, &c, runtime);
    ckMid();
    ty::SetLayoutClassCollisionWorkAdr(rdram, runtime, p4, p5);
    ckEnd(438, ctx, c, 0, 0u, 0.0f, 0x0u, 0x0u);
}
void manager_sub_010F3470(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t p4)
{
    if (!ckBegin(439, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    ref::manager_sub_010F3470(rdram, &c, runtime);
    ckMid();
    const uint64_t r = a1_sx64(ty::manager_sub_010F3470(rdram, runtime, p4));
    ckEnd(439, ctx, c, 1, r, 0.0f, 0x8u, 0x0u);
}
void manager_sub_010F3668(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t p4)
{
    if (!ckBegin(440, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    ref::manager_sub_010F3668(rdram, &c, runtime);
    ckMid();
    ty::manager_sub_010F3668(rdram, runtime, p4);
    ckEnd(440, ctx, c, 0, 0u, 0.0f, 0x0u, 0x0u);
}
void SetCameraResetSw(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t p4)
{
    if (!ckBegin(441, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    ref::SetCameraResetSw(rdram, &c, runtime);
    ckMid();
    const uint64_t r = a1_sx64(ty::SetCameraResetSw(rdram, runtime, p4));
    ckEnd(441, ctx, c, 1, r, 0.0f, 0x0u, 0x0u);
}
void GetResetCameraFlagForDisplay(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (!ckBegin(442, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    ref::GetResetCameraFlagForDisplay(rdram, &c, runtime);
    ckMid();
    const uint64_t r = a1_sx64(ty::GetResetCameraFlagForDisplay(rdram, runtime));
    ckEnd(442, ctx, c, 1, r, 0.0f, 0x8u, 0x0u);
}
void GetLightColMan(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (!ckBegin(443, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    ref::GetLightColMan(rdram, &c, runtime);
    ckMid();
    const uint64_t r = a1_sx64(ty::GetLightColMan(rdram, runtime));
    ckEnd(443, ctx, c, 1, r, 0.0f, 0x8u, 0x0u);
}
void TaskExecLightColMan(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (!ckBegin(444, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    ref::TaskExecLightColMan(rdram, &c, runtime);
    ckMid();
    const uint64_t r = a1_sx64(ty::TaskExecLightColMan(rdram, runtime));
    ckEnd(444, ctx, c, 1, r, 0.0f, 0x8u, 0x0u);
}
void SetGameLightAvailable(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t p4, uint32_t p5)
{
    if (!ckBegin(445, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    ref::SetGameLightAvailable(rdram, &c, runtime);
    ckMid();
    ty::SetGameLightAvailable(rdram, runtime, p4, p5);
    ckEnd(445, ctx, c, 0, 0u, 0.0f, 0x0u, 0x0u);
}
void ForceGameLightFlagEnvelope(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t p4)
{
    if (!ckBegin(446, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    ref::ForceGameLightFlagEnvelope(rdram, &c, runtime);
    ckMid();
    const uint64_t r = a1_sx64(ty::ForceGameLightFlagEnvelope(rdram, runtime, p4));
    ckEnd(446, ctx, c, 1, r, 0.0f, 0x28u, 0x0u);
}
void IsEnvelopeGameLightType(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t p4)
{
    if (!ckBegin(447, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    ref::IsEnvelopeGameLightType(rdram, &c, runtime);
    ckMid();
    const uint64_t r = a1_sx64(ty::IsEnvelopeGameLightType(rdram, runtime, p4));
    ckEnd(447, ctx, c, 1, r, 0.0f, 0x8u, 0x0u);
}
void SetGameLightUserParam(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t p4, uint32_t p5)
{
    if (!ckBegin(448, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    ref::SetGameLightUserParam(rdram, &c, runtime);
    ckMid();
    ty::SetGameLightUserParam(rdram, runtime, p4, p5);
    ckEnd(448, ctx, c, 0, 0u, 0.0f, 0x0u, 0x0u);
}
void SetGameLightUserFunc(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t p4, uint32_t p5)
{
    if (!ckBegin(449, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    ref::SetGameLightUserFunc(rdram, &c, runtime);
    ckMid();
    ty::SetGameLightUserFunc(rdram, runtime, p4, p5);
    ckEnd(449, ctx, c, 0, 0u, 0.0f, 0x0u, 0x0u);
}
void SetGlobalLightAvailable(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t p5, uint64_t p4)
{
    if (!ckBegin(450, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    ref::SetGlobalLightAvailable(rdram, &c, runtime);
    ckMid();
    ty::SetGlobalLightAvailable(rdram, runtime, p5, p4);
    ckEnd(450, ctx, c, 0, 0u, 0.0f, 0x0u, 0x0u);
}
void ForceGlobalLightFlagEnvelope(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t p4)
{
    if (!ckBegin(451, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    ref::ForceGlobalLightFlagEnvelope(rdram, &c, runtime);
    ckMid();
    const uint64_t r = a1_sx64(ty::ForceGlobalLightFlagEnvelope(rdram, runtime, p4));
    ckEnd(451, ctx, c, 1, r, 0.0f, 0x28u, 0x0u);
}
void IsEnvelopeGlobalLightType(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t p4)
{
    if (!ckBegin(452, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    ref::IsEnvelopeGlobalLightType(rdram, &c, runtime);
    ckMid();
    const uint64_t r = a1_sx64(ty::IsEnvelopeGlobalLightType(rdram, runtime, p4));
    ckEnd(452, ctx, c, 1, r, 0.0f, 0x8u, 0x0u);
}
void SetLwoDemoOrientManagerObjCutFinishFlag(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t p4, uint32_t p5)
{
    if (!ckBegin(453, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    ref::SetLwoDemoOrientManagerObjCutFinishFlag(rdram, &c, runtime);
    ckMid();
    ty::SetLwoDemoOrientManagerObjCutFinishFlag(rdram, runtime, p4, p5);
    ckEnd(453, ctx, c, 0, 0u, 0.0f, 0x0u, 0x0u);
}
void GetLayoutObjGroup(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (!ckBegin(454, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    ref::GetLayoutObjGroup(rdram, &c, runtime);
    ckMid();
    const uint64_t r = a1_sx64(ty::GetLayoutObjGroup(rdram, runtime));
    ckEnd(454, ctx, c, 1, r, 0.0f, 0x8u, 0x0u);
}
void GetLayoutDispObjGroup(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (!ckBegin(455, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    ref::GetLayoutDispObjGroup(rdram, &c, runtime);
    ckMid();
    const uint64_t r = a1_sx64(ty::GetLayoutDispObjGroup(rdram, runtime));
    ckEnd(455, ctx, c, 1, r, 0.0f, 0x8u, 0x0u);
}
void GetSkeletonFocusNodeDirect(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint64_t p4)
{
    if (!ckBegin(456, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    ref::GetSkeletonFocusNodeDirect(rdram, &c, runtime);
    ckMid();
    const uint64_t r = a1_sx64(ty::GetSkeletonFocusNodeDirect(rdram, runtime, p4));
    ckEnd(456, ctx, c, 1, r, 0.0f, 0x0u, 0x0u);
}
void AddObjectManagerSheetWorkSize(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t p4)
{
    if (!ckBegin(457, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    ref::AddObjectManagerSheetWorkSize(rdram, &c, runtime);
    ckMid();
    const uint64_t r = a1_sx64(ty::AddObjectManagerSheetWorkSize(rdram, runtime, p4));
    ckEnd(457, ctx, c, 1, r, 0.0f, 0x8u, 0x0u);
}
void GetAmbientLightObjGroup(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (!ckBegin(458, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    ref::GetAmbientLightObjGroup(rdram, &c, runtime);
    ckMid();
    const uint64_t r = a1_sx64(ty::GetAmbientLightObjGroup(rdram, runtime));
    ckEnd(458, ctx, c, 1, r, 0.0f, 0x8u, 0x0u);
}
void GetScriptPriorityGroup(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t p4)
{
    if (!ckBegin(459, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    ref::GetScriptPriorityGroup(rdram, &c, runtime);
    ckMid();
    const uint64_t r = a1_sx64(ty::GetScriptPriorityGroup(rdram, runtime, p4));
    ckEnd(459, ctx, c, 1, r, 0.0f, 0x2u, 0x0u);
}
void manager_sub_01103080(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint64_t p4)
{
    if (!ckBegin(460, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    ref::manager_sub_01103080(rdram, &c, runtime);
    ckMid();
    const uint64_t r = (ty::manager_sub_01103080(rdram, runtime, p4));
    ckEnd(460, ctx, c, 2, r, 0.0f, 0x18u, 0x0u);
}
void GetScriptAnimationGlobalBaseInfo(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (!ckBegin(461, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    ref::GetScriptAnimationGlobalBaseInfo(rdram, &c, runtime);
    ckMid();
    const uint64_t r = a1_sx64(ty::GetScriptAnimationGlobalBaseInfo(rdram, runtime));
    ckEnd(461, ctx, c, 1, r, 0.0f, 0x0u, 0x0u);
}
void CopyScriptAnimationWindParam(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t p4, uint32_t p5)
{
    if (!ckBegin(462, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    ref::CopyScriptAnimationWindParam(rdram, &c, runtime);
    ckMid();
    const uint64_t r = (ty::CopyScriptAnimationWindParam(rdram, runtime, p4, p5));
    ckEnd(462, ctx, c, 2, r, 0.0f, 0x8u, 0x0u);
}
void IsNonISysObjTypeRes(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint64_t p4)
{
    if (!ckBegin(463, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    ref::IsNonISysObjTypeRes(rdram, &c, runtime);
    ckMid();
    const uint64_t r = (ty::IsNonISysObjTypeRes(rdram, runtime, p4));
    ckEnd(463, ctx, c, 2, r, 0.0f, 0x8u, 0x0u);
}
void SetScriptCharDefaultScriptFunc(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t p4)
{
    if (!ckBegin(464, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    ref::SetScriptCharDefaultScriptFunc(rdram, &c, runtime);
    ckMid();
    const uint64_t r = a1_sx64(ty::SetScriptCharDefaultScriptFunc(rdram, runtime, p4));
    ckEnd(464, ctx, c, 1, r, 0.0f, 0x0u, 0x0u);
}
void InitSortObj(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t p4, uint32_t p5)
{
    if (!ckBegin(465, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    ref::InitSortObj(rdram, &c, runtime);
    ckMid();
    ty::InitSortObj(rdram, runtime, p4, p5);
    ckEnd(465, ctx, c, 0, 0u, 0.0f, 0x0u, 0x0u);
}
void setDefaultMenuControlButton(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (!ckBegin(466, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    ref::setDefaultMenuControlButton(rdram, &c, runtime);
    ckMid();
    const uint64_t r = a1_sx64(ty::setDefaultMenuControlButton(rdram, runtime));
    ckEnd(466, ctx, c, 1, r, 0.0f, 0x18u, 0x0u);
}
void InitViewIntervalClip(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t p4)
{
    if (!ckBegin(467, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    ref::InitViewIntervalClip(rdram, &c, runtime);
    ckMid();
    const float r = ty::InitViewIntervalClip(rdram, runtime, p4);
    ckEnd(467, ctx, c, 3, 0u, r, 0x2u, 0x0u);
}
void CopyCollisionEdge(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t p4, uint32_t p5)
{
    if (!ckBegin(468, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    ref::CopyCollisionEdge(rdram, &c, runtime);
    ckMid();
    const uint64_t r = a1_sx64(ty::CopyCollisionEdge(rdram, runtime, p4, p5));
    ckEnd(468, ctx, c, 1, r, 0.0f, 0x8u, 0x0u);
}
void MakeCollisionEdgePointer(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t p4, uint32_t p5, uint32_t p6)
{
    if (!ckBegin(469, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    ref::MakeCollisionEdgePointer(rdram, &c, runtime);
    ckMid();
    const uint64_t r = a1_sx64(ty::MakeCollisionEdgePointer(rdram, runtime, p4, p5, p6));
    ckEnd(469, ctx, c, 1, r, 0.0f, 0x8u, 0x0u);
}
void IsClusterCollision(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t p4)
{
    if (!ckBegin(470, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    ref::IsClusterCollision(rdram, &c, runtime);
    ckMid();
    const uint64_t r = a1_sx64(ty::IsClusterCollision(rdram, runtime, p4));
    ckEnd(470, ctx, c, 1, r, 0.0f, 0x0u, 0x0u);
}
void MakeGeometryPointerCollisionEdgeInfo(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t p4, uint32_t p5, uint32_t p6, float pf12)
{
    if (!ckBegin(471, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    ref::MakeGeometryPointerCollisionEdgeInfo(rdram, &c, runtime);
    ckMid();
    const uint64_t r = a1_sx64(ty::MakeGeometryPointerCollisionEdgeInfo(rdram, runtime, p4, p5, p6, pf12));
    ckEnd(471, ctx, c, 1, r, 0.0f, 0x88u, 0x0u);
}
void GetCollisionResource(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t p4)
{
    if (!ckBegin(472, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    ref::GetCollisionResource(rdram, &c, runtime);
    ckMid();
    const uint64_t r = a1_sx64(ty::GetCollisionResource(rdram, runtime, p4));
    ckEnd(472, ctx, c, 1, r, 0.0f, 0x8u, 0x0u);
}
void CollisionObjectJudgeCallbackExceptOneObj(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint64_t p4, uint64_t p5)
{
    if (!ckBegin(473, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    ref::CollisionObjectJudgeCallbackExceptOneObj(rdram, &c, runtime);
    ckMid();
    const uint64_t r = (ty::CollisionObjectJudgeCallbackExceptOneObj(rdram, runtime, p4, p5));
    ckEnd(473, ctx, c, 2, r, 0.0f, 0x0u, 0x0u);
}
void manager_sub_011293E0(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t p7, float pf12)
{
    if (!ckBegin(474, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    ref::manager_sub_011293E0(rdram, &c, runtime);
    ckMid();
    const a1::RetVF r = ty::manager_sub_011293E0(rdram, runtime, p7, pf12);
    ckEnd(474, ctx, c, 4, r.v0, r.f0, 0x0u, 0x0u);
}
void manager_sub_01129AE8(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t p4)
{
    if (!ckBegin(475, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    ref::manager_sub_01129AE8(rdram, &c, runtime);
    ckMid();
    ty::manager_sub_01129AE8(rdram, runtime, p4);
    ckEnd(475, ctx, c, 0, 0u, 0.0f, 0x0u, 0x0u);
}
void FlushCollisionEdgeLinkDBVWallFinder(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t p4)
{
    if (!ckBegin(476, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    ref::FlushCollisionEdgeLinkDBVWallFinder(rdram, &c, runtime);
    ckMid();
    ty::FlushCollisionEdgeLinkDBVWallFinder(rdram, runtime, p4);
    ckEnd(476, ctx, c, 0, 0u, 0.0f, 0x0u, 0x0u);
}
void GetCollisionEdgeLinkDBWorkSize(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint64_t p4)
{
    if (!ckBegin(477, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    ref::GetCollisionEdgeLinkDBWorkSize(rdram, &c, runtime);
    ckMid();
    const uint64_t r = a1_sx64(ty::GetCollisionEdgeLinkDBWorkSize(rdram, runtime, p4));
    ckEnd(477, ctx, c, 1, r, 0.0f, 0x0u, 0x0u);
}
void GetCollisionFinderWorkSize(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t p4)
{
    if (!ckBegin(478, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    ref::GetCollisionFinderWorkSize(rdram, &c, runtime);
    ckMid();
    const uint64_t r = a1_sx64(ty::GetCollisionFinderWorkSize(rdram, runtime, p4));
    ckEnd(478, ctx, c, 1, r, 0.0f, 0x8u, 0x0u);
}
void InitGeometryPointer(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t p4)
{
    if (!ckBegin(479, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    ref::InitGeometryPointer(rdram, &c, runtime);
    ckMid();
    const uint64_t r = (ty::InitGeometryPointer(rdram, runtime, p4));
    ckEnd(479, ctx, c, 2, r, 0.0f, 0x28u, 0x0u);
}
void SetGeometryPointerChainPos(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t p4, uint32_t p5, uint32_t p6, float pf12)
{
    if (!ckBegin(480, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    ref::SetGeometryPointerChainPos(rdram, &c, runtime);
    ckMid();
    const uint64_t r = a1_sx64(ty::SetGeometryPointerChainPos(rdram, runtime, p4, p5, p6, pf12));
    ckEnd(480, ctx, c, 1, r, 0.0f, 0x0u, 0x0u);
}
void CheckLegalCollisionGP(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t p4)
{
    if (!ckBegin(481, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    ref::CheckLegalCollisionGP(rdram, &c, runtime);
    ckMid();
    const uint64_t r = a1_sx64(ty::CheckLegalCollisionGP(rdram, runtime, p4));
    ckEnd(481, ctx, c, 1, r, 0.0f, 0x8u, 0x0u);
}
void GetYHeightOfPlaneXZProjection(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t p4, uint32_t p5)
{
    if (!ckBegin(482, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    ref::GetYHeightOfPlaneXZProjection(rdram, &c, runtime);
    ckMid();
    const float r = ty::GetYHeightOfPlaneXZProjection(rdram, runtime, p4, p5);
    ckEnd(482, ctx, c, 3, 0u, r, 0x0u, 0x1eu);
}
void CheckClothChainExist(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t p4)
{
    if (!ckBegin(483, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    ref::CheckClothChainExist(rdram, &c, runtime);
    ckMid();
    const uint64_t r = a1_sx64(ty::CheckClothChainExist(rdram, runtime, p4));
    ckEnd(483, ctx, c, 1, r, 0.0f, 0x0u, 0x0u);
}
void SetClothSingleFixedChainLeafFixNodeOffset(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t p4, uint32_t p5)
{
    if (!ckBegin(484, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    ref::SetClothSingleFixedChainLeafFixNodeOffset(rdram, &c, runtime);
    ckMid();
    ty::SetClothSingleFixedChainLeafFixNodeOffset(rdram, runtime, p4, p5);
    ckEnd(484, ctx, c, 0, 0u, 0.0f, 0x0u, 0x0u);
}
void GetTwoWayLinkNext(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t p4)
{
    if (!ckBegin(485, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    ref::GetTwoWayLinkNext(rdram, &c, runtime);
    ckMid();
    const uint64_t r = a1_sx64(ty::GetTwoWayLinkNext(rdram, runtime, p4));
    ckEnd(485, ctx, c, 1, r, 0.0f, 0x0u, 0x0u);
}
void RelocateTwoWayLinkHeader(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t p4)
{
    if (!ckBegin(486, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    ref::RelocateTwoWayLinkHeader(rdram, &c, runtime);
    ckMid();
    const uint64_t r = a1_sx64(ty::RelocateTwoWayLinkHeader(rdram, runtime, p4));
    ckEnd(486, ctx, c, 1, r, 0.0f, 0x0u, 0x0u);
}
void GetTwoWayLinkData(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t p4)
{
    if (!ckBegin(487, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    ref::GetTwoWayLinkData(rdram, &c, runtime);
    ckMid();
    const uint64_t r = a1_sx64(ty::GetTwoWayLinkData(rdram, runtime, p4));
    ckEnd(487, ctx, c, 1, r, 0.0f, 0x0u, 0x0u);
}
void mcReqSetWaitFunc(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t p4)
{
    if (!ckBegin(488, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    ref::mcReqSetWaitFunc(rdram, &c, runtime);
    ckMid();
    const uint64_t r = a1_sx64(ty::mcReqSetWaitFunc(rdram, runtime, p4));
    ckEnd(488, ctx, c, 1, r, 0.0f, 0x0u, 0x0u);
}
void mcGetProcessPer(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (!ckBegin(489, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    ref::mcGetProcessPer(rdram, &c, runtime);
    ckMid();
    const a1::RetVF r = ty::mcGetProcessPer(rdram, runtime);
    ckEnd(489, ctx, c, 4, r.v0, r.f0, 0x0u, 0x0u);
}
void mcGetGameClearFlag(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (!ckBegin(490, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    ref::mcGetGameClearFlag(rdram, &c, runtime);
    ckMid();
    const uint64_t r = a1_sx64(ty::mcGetGameClearFlag(rdram, runtime));
    ckEnd(490, ctx, c, 1, r, 0.0f, 0x8u, 0x0u);
}
void mcGetCompleteTimeAttack(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t p4, uint32_t p5)
{
    if (!ckBegin(491, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    ref::mcGetCompleteTimeAttack(rdram, &c, runtime);
    ckMid();
    const uint64_t r = a1_sx64(ty::mcGetCompleteTimeAttack(rdram, runtime, p4, p5));
    ckEnd(491, ctx, c, 1, r, 0.0f, 0x48u, 0x0u);
}
void mouseSetCursorPosition(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, float pf12, float pf13)
{
    if (!ckBegin(492, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    ref::mouseSetCursorPosition(rdram, &c, runtime);
    ckMid();
    const uint64_t r = a1_sx64(ty::mouseSetCursorPosition(rdram, runtime, pf12, pf13));
    ckEnd(492, ctx, c, 1, r, 0.0f, 0x0u, 0x0u);
}
void mouseMoveCursorPosition(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, float pf12, float pf13)
{
    if (!ckBegin(493, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    ref::mouseMoveCursorPosition(rdram, &c, runtime);
    ckMid();
    const a1::RetVF r = ty::mouseMoveCursorPosition(rdram, runtime, pf12, pf13);
    ckEnd(493, ctx, c, 4, r.v0, r.f0, 0x0u, 0x2u);
}
void setVibrationEnableWorkAdrs(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t p4)
{
    if (!ckBegin(494, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    ref::setVibrationEnableWorkAdrs(rdram, &c, runtime);
    ckMid();
    const uint64_t r = a1_sx64(ty::setVibrationEnableWorkAdrs(rdram, runtime, p4));
    ckEnd(494, ctx, c, 1, r, 0.0f, 0x0u, 0x0u);
}
void usbdevMouseGetStatus(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (!ckBegin(495, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    ref::usbdevMouseGetStatus(rdram, &c, runtime);
    ckMid();
    const uint64_t r = a1_sx64(ty::usbdevMouseGetStatus(rdram, runtime));
    ckEnd(495, ctx, c, 1, r, 0.0f, 0x0u, 0x0u);
}
void windowSemiTransRate(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (!ckBegin(496, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    ref::windowSemiTransRate(rdram, &c, runtime);
    ckMid();
    const uint64_t r = a1_sx64(ty::windowSemiTransRate(rdram, runtime));
    ckEnd(496, ctx, c, 1, r, 0.0f, 0x0u, 0x0u);
}
void windowGetCurrentWindowHandle(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (!ckBegin(497, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    ref::windowGetCurrentWindowHandle(rdram, &c, runtime);
    ckMid();
    const uint64_t r = a1_sx64(ty::windowGetCurrentWindowHandle(rdram, runtime));
    ckEnd(497, ctx, c, 1, r, 0.0f, 0x8u, 0x0u);
}
void windowSetTex(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t p4)
{
    if (!ckBegin(498, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    ref::windowSetTex(rdram, &c, runtime);
    ckMid();
    const uint64_t r = a1_sx64(ty::windowSetTex(rdram, runtime, p4));
    ckEnd(498, ctx, c, 1, r, 0.0f, 0x0u, 0x0u);
}
void adpcmOpenChkComplete(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint64_t p4)
{
    if (!ckBegin(499, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    ref::adpcmOpenChkComplete(rdram, &c, runtime);
    ckMid();
    const uint64_t r = (ty::adpcmOpenChkComplete(rdram, runtime, p4));
    ckEnd(499, ctx, c, 2, r, 0.0f, 0x10u, 0x0u);
}
void adpcmCloseReq(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint64_t p4, uint64_t p5)
{
    if (!ckBegin(500, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    ref::adpcmCloseReq(rdram, &c, runtime);
    ckMid();
    const uint64_t r = (ty::adpcmCloseReq(rdram, runtime, p4, p5));
    ckEnd(500, ctx, c, 2, r, 0.0f, 0x1au, 0x0u);
}
void adpcmCloseFadeReq(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint64_t p4, uint64_t p5, float pf12)
{
    if (!ckBegin(501, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    ref::adpcmCloseFadeReq(rdram, &c, runtime);
    ckMid();
    const uint64_t r = (ty::adpcmCloseFadeReq(rdram, runtime, p4, p5, pf12));
    ckEnd(501, ctx, c, 2, r, 0.0f, 0x1au, 0x0u);
}
void adpcmCloseFadeSetVol(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t p4, float pf12)
{
    if (!ckBegin(502, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    ref::adpcmCloseFadeSetVol(rdram, &c, runtime);
    ckMid();
    const a1::RetVF r = ty::adpcmCloseFadeSetVol(rdram, runtime, p4, pf12);
    ckEnd(502, ctx, c, 4, r.v0, r.f0, 0x12u, 0x2u);
}
void adpcmCloseChkComplete(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint64_t p4)
{
    if (!ckBegin(503, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    ref::adpcmCloseChkComplete(rdram, &c, runtime);
    ckMid();
    const uint64_t r = (ty::adpcmCloseChkComplete(rdram, runtime, p4));
    ckEnd(503, ctx, c, 2, r, 0.0f, 0x0u, 0x0u);
}
void adpcmDeleteAdpcmSeq(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint64_t p4)
{
    if (!ckBegin(504, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    ref::adpcmDeleteAdpcmSeq(rdram, &c, runtime);
    ckMid();
    const uint64_t r = (ty::adpcmDeleteAdpcmSeq(rdram, runtime, p4));
    ckEnd(504, ctx, c, 2, r, 0.0f, 0x1au, 0x0u);
}
void adpcmSetVol(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint64_t p4, float pf12, float pf13)
{
    if (!ckBegin(505, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    ref::adpcmSetVol(rdram, &c, runtime);
    ckMid();
    const uint64_t r = (ty::adpcmSetVol(rdram, runtime, p4, pf12, pf13));
    ckEnd(505, ctx, c, 2, r, 0.0f, 0x12u, 0x0u);
}
void adpcmSetPitch(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t p4, float pf12, float pf13)
{
    if (!ckBegin(506, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    ref::adpcmSetPitch(rdram, &c, runtime);
    ckMid();
    const uint64_t r = a1_sx64(ty::adpcmSetPitch(rdram, runtime, p4, pf12, pf13));
    ckEnd(506, ctx, c, 1, r, 0.0f, 0x12u, 0x0u);
}
void adpcmGamePause(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t p4)
{
    if (!ckBegin(507, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    ref::adpcmGamePause(rdram, &c, runtime);
    ckMid();
    const uint64_t r = a1_sx64(ty::adpcmGamePause(rdram, runtime, p4));
    ckEnd(507, ctx, c, 1, r, 0.0f, 0x0u, 0x0u);
}
void seSetVol(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t p4, float pf12)
{
    if (!ckBegin(508, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    ref::seSetVol(rdram, &c, runtime);
    ckMid();
    ty::seSetVol(rdram, runtime, p4, pf12);
    ckEnd(508, ctx, c, 0, 0u, 0.0f, 0x0u, 0x0u);
}
void manager_sub_01159008(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t p4)
{
    if (!ckBegin(509, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    ref::manager_sub_01159008(rdram, &c, runtime);
    ckMid();
    const uint64_t r = (ty::manager_sub_01159008(rdram, runtime, p4));
    ckEnd(509, ctx, c, 2, r, 0.0f, 0xf8u, 0x0u);
}
void seDelayPlay3DTest(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (!ckBegin(510, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    ref::seDelayPlay3DTest(rdram, &c, runtime);
    ckMid();
    const uint64_t r = (ty::seDelayPlay3DTest(rdram, runtime));
    ckEnd(510, ctx, c, 2, r, 0.0f, 0x0u, 0x0u);
}
void sheetFsAddStageTexSegIndex(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t p4)
{
    if (!ckBegin(511, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    ref::sheetFsAddStageTexSegIndex(rdram, &c, runtime);
    ckMid();
    const uint64_t r = a1_sx64(ty::sheetFsAddStageTexSegIndex(rdram, runtime, p4));
    ckEnd(511, ctx, c, 1, r, 0.0f, 0x18u, 0x0u);
}
void sheetFsDecStageTexSegIndex(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t p4)
{
    if (!ckBegin(512, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    ref::sheetFsDecStageTexSegIndex(rdram, &c, runtime);
    ckMid();
    const uint64_t r = a1_sx64(ty::sheetFsDecStageTexSegIndex(rdram, runtime, p4));
    ckEnd(512, ctx, c, 1, r, 0.0f, 0x18u, 0x0u);
}
void sheetFsSetStageTexSegTexSegDef(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t p4, uint32_t p5)
{
    if (!ckBegin(513, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    ref::sheetFsSetStageTexSegTexSegDef(rdram, &c, runtime);
    ckMid();
    const uint64_t r = a1_sx64(ty::sheetFsSetStageTexSegTexSegDef(rdram, runtime, p4, p5));
    ckEnd(513, ctx, c, 1, r, 0.0f, 0x18u, 0x0u);
}
void sheetFsGetStageTexSegTexSegDef(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t p4)
{
    if (!ckBegin(514, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    ref::sheetFsGetStageTexSegTexSegDef(rdram, &c, runtime);
    ckMid();
    const uint64_t r = a1_sx64(ty::sheetFsGetStageTexSegTexSegDef(rdram, runtime, p4));
    ckEnd(514, ctx, c, 1, r, 0.0f, 0x18u, 0x0u);
}
void sheetFsGetDirectFileSize(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t p4)
{
    if (!ckBegin(515, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    ref::sheetFsGetDirectFileSize(rdram, &c, runtime);
    ckMid();
    const uint64_t r = a1_sx64(ty::sheetFsGetDirectFileSize(rdram, runtime, p4));
    ckEnd(515, ctx, c, 1, r, 0.0f, 0x18u, 0x0u);
}
void sheetFileGetSize(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t p4)
{
    if (!ckBegin(516, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    ref::sheetFileGetSize(rdram, &c, runtime);
    ckMid();
    const uint64_t r = a1_sx64(ty::sheetFileGetSize(rdram, runtime, p4));
    ckEnd(516, ctx, c, 1, r, 0.0f, 0x0u, 0x0u);
}
void sheetHeaderObjSearchSheetId(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t p4)
{
    if (!ckBegin(517, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    ref::sheetHeaderObjSearchSheetId(rdram, &c, runtime);
    ckMid();
    const uint64_t r = a1_sx64(ty::sheetHeaderObjSearchSheetId(rdram, runtime, p4));
    ckEnd(517, ctx, c, 1, r, 0.0f, 0x18u, 0x0u);
}
void sheetMgrAddSegLoadList(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t p4)
{
    if (!ckBegin(518, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    ref::sheetMgrAddSegLoadList(rdram, &c, runtime);
    ckMid();
    const uint64_t r = a1_sx64(ty::sheetMgrAddSegLoadList(rdram, runtime, p4));
    ckEnd(518, ctx, c, 1, r, 0.0f, 0x8u, 0x0u);
}
void sheetMgrGetSegLoadHeader(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (!ckBegin(519, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    ref::sheetMgrGetSegLoadHeader(rdram, &c, runtime);
    ckMid();
    const uint64_t r = a1_sx64(ty::sheetMgrGetSegLoadHeader(rdram, runtime));
    ckEnd(519, ctx, c, 1, r, 0.0f, 0x8u, 0x0u);
}
void sheetMgrAddSegInitList(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t p4)
{
    if (!ckBegin(520, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    ref::sheetMgrAddSegInitList(rdram, &c, runtime);
    ckMid();
    const uint64_t r = a1_sx64(ty::sheetMgrAddSegInitList(rdram, runtime, p4));
    ckEnd(520, ctx, c, 1, r, 0.0f, 0x8u, 0x0u);
}
void sheetMgrGetSegInitHeader(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (!ckBegin(521, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    ref::sheetMgrGetSegInitHeader(rdram, &c, runtime);
    ckMid();
    const uint64_t r = a1_sx64(ty::sheetMgrGetSegInitHeader(rdram, runtime));
    ckEnd(521, ctx, c, 1, r, 0.0f, 0x8u, 0x0u);
}
void sheetMgrSetNoWaitMode(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t p4)
{
    if (!ckBegin(522, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    ref::sheetMgrSetNoWaitMode(rdram, &c, runtime);
    ckMid();
    const uint64_t r = a1_sx64(ty::sheetMgrSetNoWaitMode(rdram, runtime, p4));
    ckEnd(522, ctx, c, 1, r, 0.0f, 0x0u, 0x0u);
}
void get_camera_lws_animation_param(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t p4)
{
    if (!ckBegin(523, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    ref::get_camera_lws_animation_param(rdram, &c, runtime);
    ckMid();
    const uint64_t r = a1_sx64(ty::get_camera_lws_animation_param(rdram, runtime, p4));
    ckEnd(523, ctx, c, 1, r, 0.0f, 0x0u, 0x0u);
}
void get_boot_obj_camera_lws_animation(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t p4)
{
    if (!ckBegin(524, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    ref::get_boot_obj_camera_lws_animation(rdram, &c, runtime);
    ckMid();
    const uint64_t r = a1_sx64(ty::get_boot_obj_camera_lws_animation(rdram, runtime, p4));
    ckEnd(524, ctx, c, 1, r, 0.0f, 0x0u, 0x0u);
}
void is_loop_camera_lws_animation(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t p4)
{
    if (!ckBegin(525, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    ref::is_loop_camera_lws_animation(rdram, &c, runtime);
    ckMid();
    const uint64_t r = a1_sx64(ty::is_loop_camera_lws_animation(rdram, runtime, p4));
    ckEnd(525, ctx, c, 1, r, 0.0f, 0x0u, 0x0u);
}
void is_sub_type_camera_lws_animation(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t p4)
{
    if (!ckBegin(526, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    ref::is_sub_type_camera_lws_animation(rdram, &c, runtime);
    ckMid();
    const uint64_t r = a1_sx64(ty::is_sub_type_camera_lws_animation(rdram, runtime, p4));
    ckEnd(526, ctx, c, 1, r, 0.0f, 0x0u, 0x0u);
}
void reset_main_type_camera_lws_animation_update_flag(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (!ckBegin(527, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    ref::reset_main_type_camera_lws_animation_update_flag(rdram, &c, runtime);
    ckMid();
    const uint64_t r = a1_sx64(ty::reset_main_type_camera_lws_animation_update_flag(rdram, runtime));
    ckEnd(527, ctx, c, 1, r, 0.0f, 0x0u, 0x0u);
}
void is_same_camera_lws_animation(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint64_t p4, uint64_t p5)
{
    if (!ckBegin(528, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    ref::is_same_camera_lws_animation(rdram, &c, runtime);
    ckMid();
    const uint64_t r = (ty::is_same_camera_lws_animation(rdram, runtime, p4, p5));
    ckEnd(528, ctx, c, 2, r, 0.0f, 0x18u, 0x0u);
}
void stop_camera_lws_animation_geometry_pointer_reference(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint64_t p4)
{
    if (!ckBegin(529, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    ref::stop_camera_lws_animation_geometry_pointer_reference(rdram, &c, runtime);
    ckMid();
    const uint64_t r = a1_sx64(ty::stop_camera_lws_animation_geometry_pointer_reference(rdram, runtime, p4));
    ckEnd(529, ctx, c, 1, r, 0.0f, 0xau, 0x0u);
}
void stop_camera_lws_animation_callback(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint64_t p4)
{
    if (!ckBegin(530, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    ref::stop_camera_lws_animation_callback(rdram, &c, runtime);
    ckMid();
    const uint64_t r = a1_sx64(ty::stop_camera_lws_animation_callback(rdram, runtime, p4));
    ckEnd(530, ctx, c, 1, r, 0.0f, 0xau, 0x0u);
}
void manager_sub_011629B8(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t p4)
{
    if (!ckBegin(531, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    ref::manager_sub_011629B8(rdram, &c, runtime);
    ckMid();
    const uint64_t r = a1_sx64(ty::manager_sub_011629B8(rdram, runtime, p4));
    ckEnd(531, ctx, c, 1, r, 0.0f, 0x0u, 0x3u);
}
void get_clip_screen(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t p4)
{
    if (!ckBegin(532, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    ref::get_clip_screen(rdram, &c, runtime);
    ckMid();
    const uint64_t r = a1_sx64(ty::get_clip_screen(rdram, runtime, p4));
    ckEnd(532, ctx, c, 1, r, 0.0f, 0x0u, 0x0u);
}
void get_transform_matrix_set(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t p4)
{
    if (!ckBegin(533, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    ref::get_transform_matrix_set(rdram, &c, runtime);
    ckMid();
    const uint64_t r = a1_sx64(ty::get_transform_matrix_set(rdram, runtime, p4));
    ckEnd(533, ctx, c, 1, r, 0.0f, 0x0u, 0x0u);
}
void get_change_grace_time_camera_region(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint64_t p4)
{
    if (!ckBegin(534, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    ref::get_change_grace_time_camera_region(rdram, &c, runtime);
    ckMid();
    const float r = ty::get_change_grace_time_camera_region(rdram, runtime, p4);
    ckEnd(534, ctx, c, 3, 0u, r, 0xcu, 0x0u);
}
void manager_sub_01164260(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint64_t p4)
{
    if (!ckBegin(535, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    ref::manager_sub_01164260(rdram, &c, runtime);
    ckMid();
    const uint64_t r = (ty::manager_sub_01164260(rdram, runtime, p4));
    ckEnd(535, ctx, c, 2, r, 0.0f, 0x8u, 0x0u);
}
void set_screen_wide_and_progressive(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint64_t p4, uint64_t p5)
{
    if (!ckBegin(536, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    ref::set_screen_wide_and_progressive(rdram, &c, runtime);
    ckMid();
    const a1::RetVF r = ty::set_screen_wide_and_progressive(rdram, runtime, p4, p5);
    ckEnd(536, ctx, c, 4, r.v0, r.f0, 0x2u, 0x0u);
}
void set_view_angle(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t p4, float pf12)
{
    if (!ckBegin(537, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    ref::set_view_angle(rdram, &c, runtime);
    ckMid();
    ty::set_view_angle(rdram, runtime, p4, pf12);
    ckEnd(537, ctx, c, 0, 0u, 0.0f, 0x0u, 0x0u);
}
void get_screen_width(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t p4)
{
    if (!ckBegin(538, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    ref::get_screen_width(rdram, &c, runtime);
    ckMid();
    const uint64_t r = a1_sx64(ty::get_screen_width(rdram, runtime, p4));
    ckEnd(538, ctx, c, 1, r, 0.0f, 0x0u, 0x0u);
}
void get_screen_height(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t p4)
{
    if (!ckBegin(539, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    ref::get_screen_height(rdram, &c, runtime);
    ckMid();
    const uint64_t r = a1_sx64(ty::get_screen_height(rdram, runtime, p4));
    ckEnd(539, ctx, c, 1, r, 0.0f, 0x0u, 0x0u);
}
void get_aspect_ratio(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t p4)
{
    if (!ckBegin(540, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    ref::get_aspect_ratio(rdram, &c, runtime);
    ckMid();
    const a1::RetVF r = ty::get_aspect_ratio(rdram, runtime, p4);
    ckEnd(540, ctx, c, 4, r.v0, r.f0, 0x8u, 0x6u);
}
void get_view_angle(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t p4)
{
    if (!ckBegin(541, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    ref::get_view_angle(rdram, &c, runtime);
    ckMid();
    const float r = ty::get_view_angle(rdram, runtime, p4);
    ckEnd(541, ctx, c, 3, 0u, r, 0x0u, 0x0u);
}
void get_near_clip_z_clip_screen(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (!ckBegin(542, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    ref::get_near_clip_z_clip_screen(rdram, &c, runtime);
    ckMid();
    const float r = ty::get_near_clip_z_clip_screen(rdram, runtime);
    ckEnd(542, ctx, c, 3, 0u, r, 0x2u, 0x0u);
}
void get_far_clip_z_clip_screen(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (!ckBegin(543, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    ref::get_far_clip_z_clip_screen(rdram, &c, runtime);
    ckMid();
    const float r = ty::get_far_clip_z_clip_screen(rdram, runtime);
    ckEnd(543, ctx, c, 3, 0u, r, 0x2u, 0x0u);
}
void get_z_buf_min_clip_screen(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (!ckBegin(544, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    ref::get_z_buf_min_clip_screen(rdram, &c, runtime);
    ckMid();
    const float r = ty::get_z_buf_min_clip_screen(rdram, runtime);
    ckEnd(544, ctx, c, 3, 0u, r, 0x2u, 0x0u);
}
void get_z_buf_max_clip_screen(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (!ckBegin(545, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    ref::get_z_buf_max_clip_screen(rdram, &c, runtime);
    ckMid();
    const float r = ty::get_z_buf_max_clip_screen(rdram, runtime);
    ckEnd(545, ctx, c, 3, 0u, r, 0x2u, 0x0u);
}
void get_scissor_clip_matrix(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t p4, uint32_t p5)
{
    if (!ckBegin(546, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    ref::get_scissor_clip_matrix(rdram, &c, runtime);
    ckMid();
    const uint64_t r = a1_sx64(ty::get_scissor_clip_matrix(rdram, runtime, p4, p5));
    ckEnd(546, ctx, c, 1, r, 0.0f, 0x20u, 0x0u);
}
void get_WV_matrix(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint64_t p4)
{
    if (!ckBegin(547, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    ref::get_WV_matrix(rdram, &c, runtime);
    ckMid();
    const uint64_t r = (ty::get_WV_matrix(rdram, runtime, p4));
    ckEnd(547, ctx, c, 2, r, 0.0f, 0x0u, 0x0u);
}
void ResetDamageClass(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t p4)
{
    if (!ckBegin(548, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    ref::ResetDamageClass(rdram, &c, runtime);
    ckMid();
    const uint64_t r = a1_sx64(ty::ResetDamageClass(rdram, runtime, p4));
    ckEnd(548, ctx, c, 1, r, 0.0f, 0x0u, 0x0u);
}
void SetDamageClassDamageTableType(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint64_t p4)
{
    if (!ckBegin(549, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    ref::SetDamageClassDamageTableType(rdram, &c, runtime);
    ckMid();
    const uint64_t r = (ty::SetDamageClassDamageTableType(rdram, runtime, p4));
    ckEnd(549, ctx, c, 2, r, 0.0f, 0x8u, 0x0u);
}
void getEditWorkInitData(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint64_t p4)
{
    if (!ckBegin(550, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    ref::getEditWorkInitData(rdram, &c, runtime);
    ckMid();
    const uint64_t r = (ty::getEditWorkInitData(rdram, runtime, p4));
    ckEnd(550, ctx, c, 2, r, 0.0f, 0x0u, 0x0u);
}
void getPointData(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint64_t p4, uint64_t p5)
{
    if (!ckBegin(551, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    ref::getPointData(rdram, &c, runtime);
    ckMid();
    const uint64_t r = (ty::getPointData(rdram, runtime, p4, p5));
    ckEnd(551, ctx, c, 2, r, 0.0f, 0x8u, 0x0u);
}
void getLineData(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint64_t p4, uint64_t p5)
{
    if (!ckBegin(552, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    ref::getLineData(rdram, &c, runtime);
    ckMid();
    const uint64_t r = (ty::getLineData(rdram, runtime, p4, p5));
    ckEnd(552, ctx, c, 2, r, 0.0f, 0x8u, 0x0u);
}
void getRootPointData(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint64_t p4, uint64_t p5)
{
    if (!ckBegin(553, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    ref::getRootPointData(rdram, &c, runtime);
    ckMid();
    const uint64_t r = (ty::getRootPointData(rdram, runtime, p4, p5));
    ckEnd(553, ctx, c, 2, r, 0.0f, 0x8u, 0x0u);
}
void getGoalPointData(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint64_t p4, uint64_t p5)
{
    if (!ckBegin(554, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    ref::getGoalPointData(rdram, &c, runtime);
    ckMid();
    const uint64_t r = (ty::getGoalPointData(rdram, runtime, p4, p5));
    ckEnd(554, ctx, c, 2, r, 0.0f, 0x8u, 0x0u);
}
void getSubPointData(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint64_t p4, uint64_t p5)
{
    if (!ckBegin(555, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    ref::getSubPointData(rdram, &c, runtime);
    ckMid();
    const uint64_t r = (ty::getSubPointData(rdram, runtime, p4, p5));
    ckEnd(555, ctx, c, 2, r, 0.0f, 0x8u, 0x0u);
}
void getInterestPointData(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint64_t p4, uint64_t p5)
{
    if (!ckBegin(556, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    ref::getInterestPointData(rdram, &c, runtime);
    ckMid();
    const uint64_t r = (ty::getInterestPointData(rdram, runtime, p4, p5));
    ckEnd(556, ctx, c, 2, r, 0.0f, 0x8u, 0x0u);
}
void getDeadPointData(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint64_t p4, uint64_t p5)
{
    if (!ckBegin(557, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    ref::getDeadPointData(rdram, &c, runtime);
    ckMid();
    const uint64_t r = (ty::getDeadPointData(rdram, runtime, p4, p5));
    ckEnd(557, ctx, c, 2, r, 0.0f, 0x8u, 0x0u);
}
void getLayoutPointData(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint64_t p4, uint64_t p5)
{
    if (!ckBegin(558, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    ref::getLayoutPointData(rdram, &c, runtime);
    ckMid();
    const uint64_t r = (ty::getLayoutPointData(rdram, runtime, p4, p5));
    ckEnd(558, ctx, c, 2, r, 0.0f, 0x8u, 0x0u);
}
void getSnapShotFormat(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (!ckBegin(559, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    ref::getSnapShotFormat(rdram, &c, runtime);
    ckMid();
    const uint64_t r = a1_sx64(ty::getSnapShotFormat(rdram, runtime));
    ckEnd(559, ctx, c, 1, r, 0.0f, 0x0u, 0x0u);
}
void GAMEUTIL_get_sight_on_reverse_cntrlLR_bool(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (!ckBegin(560, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    ref::GAMEUTIL_get_sight_on_reverse_cntrlLR_bool(rdram, &c, runtime);
    ckMid();
    const uint64_t r = a1_sx64(ty::GAMEUTIL_get_sight_on_reverse_cntrlLR_bool(rdram, runtime));
    ckEnd(560, ctx, c, 1, r, 0.0f, 0x8u, 0x0u);
}
void GAMEUTIL_get_sight_on_reverse_cntrlUD_bool(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (!ckBegin(561, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    ref::GAMEUTIL_get_sight_on_reverse_cntrlUD_bool(rdram, &c, runtime);
    ckMid();
    const uint64_t r = a1_sx64(ty::GAMEUTIL_get_sight_on_reverse_cntrlUD_bool(rdram, runtime));
    ckEnd(561, ctx, c, 1, r, 0.0f, 0x8u, 0x0u);
}
void GAMEUTIL_get_game_camera_free_mode_reverse_cntrlLR_bool(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (!ckBegin(562, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    ref::GAMEUTIL_get_game_camera_free_mode_reverse_cntrlLR_bool(rdram, &c, runtime);
    ckMid();
    const uint64_t r = a1_sx64(ty::GAMEUTIL_get_game_camera_free_mode_reverse_cntrlLR_bool(rdram, runtime));
    ckEnd(562, ctx, c, 1, r, 0.0f, 0x8u, 0x0u);
}
void GAMEUTIL_get_game_camera_free_mode_reverse_cntrlUD_bool(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (!ckBegin(563, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    ref::GAMEUTIL_get_game_camera_free_mode_reverse_cntrlUD_bool(rdram, &c, runtime);
    ckMid();
    const uint64_t r = a1_sx64(ty::GAMEUTIL_get_game_camera_free_mode_reverse_cntrlUD_bool(rdram, runtime));
    ckEnd(563, ctx, c, 1, r, 0.0f, 0x8u, 0x0u);
}
void GAMEUTIL_set_auto_get_flag_boss_interest_position(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t p4)
{
    if (!ckBegin(564, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    ref::GAMEUTIL_set_auto_get_flag_boss_interest_position(rdram, &c, runtime);
    ckMid();
    const uint64_t r = a1_sx64(ty::GAMEUTIL_set_auto_get_flag_boss_interest_position(rdram, runtime, p4));
    ckEnd(564, ctx, c, 1, r, 0.0f, 0x0u, 0x0u);
}
void GAMEUTIL_reset_move_angle(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t p4)
{
    if (!ckBegin(565, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    ref::GAMEUTIL_reset_move_angle(rdram, &c, runtime);
    ckMid();
    ty::GAMEUTIL_reset_move_angle(rdram, runtime, p4);
    ckEnd(565, ctx, c, 0, 0u, 0.0f, 0x0u, 0x0u);
}
void GAMEUTIL_set_parameter_move_angle(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t p4, float pf12, float pf13, float pf14, float pf15, float pf16, float pf17)
{
    if (!ckBegin(566, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    ref::GAMEUTIL_set_parameter_move_angle(rdram, &c, runtime);
    ckMid();
    const float r = ty::GAMEUTIL_set_parameter_move_angle(rdram, runtime, p4, pf12, pf13, pf14, pf15, pf16, pf17);
    ckEnd(566, ctx, c, 3, 0u, r, 0x2u, 0x37000u);
}
void GAMEUTIL_begin_move_angle(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t p4)
{
    if (!ckBegin(567, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    ref::GAMEUTIL_begin_move_angle(rdram, &c, runtime);
    ckMid();
    ty::GAMEUTIL_begin_move_angle(rdram, runtime, p4);
    ckEnd(567, ctx, c, 0, 0u, 0.0f, 0x0u, 0x0u);
}
void GAMEUTIL_get_cur_velocity_move_angle(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t p4)
{
    if (!ckBegin(568, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    ref::GAMEUTIL_get_cur_velocity_move_angle(rdram, &c, runtime);
    ckMid();
    const float r = ty::GAMEUTIL_get_cur_velocity_move_angle(rdram, runtime, p4);
    ckEnd(568, ctx, c, 3, 0u, r, 0x0u, 0x0u);
}
void GAMEUTIL_set_need_dir_ratio_flag_look_at_vs_offset(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t p4, uint32_t p5)
{
    if (!ckBegin(569, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    ref::GAMEUTIL_set_need_dir_ratio_flag_look_at_vs_offset(rdram, &c, runtime);
    ckMid();
    ty::GAMEUTIL_set_need_dir_ratio_flag_look_at_vs_offset(rdram, runtime, p4, p5);
    ckEnd(569, ctx, c, 0, 0u, 0.0f, 0x0u, 0x0u);
}
void GAMEUTIL_set_enable_side_change_angle_look_at_vs_offset(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t p4, float pf12)
{
    if (!ckBegin(570, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    ref::GAMEUTIL_set_enable_side_change_angle_look_at_vs_offset(rdram, &c, runtime);
    ckMid();
    ty::GAMEUTIL_set_enable_side_change_angle_look_at_vs_offset(rdram, runtime, p4, pf12);
    ckEnd(570, ctx, c, 0, 0u, 0.0f, 0x0u, 0x0u);
}
void GAMEUTIL_set_parameter_col_plane_info_filter(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t p4, float pf12, float pf13)
{
    if (!ckBegin(571, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    ref::GAMEUTIL_set_parameter_col_plane_info_filter(rdram, &c, runtime);
    ckMid();
    ty::GAMEUTIL_set_parameter_col_plane_info_filter(rdram, runtime, p4, pf12, pf13);
    ckEnd(571, ctx, c, 0, 0u, 0.0f, 0x0u, 0x0u);
}
void GAMEUTIL_set_parameter_position_filter(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t p4, float pf12)
{
    if (!ckBegin(572, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    ref::GAMEUTIL_set_parameter_position_filter(rdram, &c, runtime);
    ckMid();
    ty::GAMEUTIL_set_parameter_position_filter(rdram, runtime, p4, pf12);
    ckEnd(572, ctx, c, 0, 0u, 0.0f, 0x0u, 0x0u);
}
void GAMEUTIL_reset_status_manager(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t p4, uint32_t p5)
{
    if (!ckBegin(573, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    ref::GAMEUTIL_reset_status_manager(rdram, &c, runtime);
    ckMid();
    ty::GAMEUTIL_reset_status_manager(rdram, runtime, p4, p5);
    ckEnd(573, ctx, c, 0, 0u, 0.0f, 0x0u, 0x0u);
}
void GAMEUTIL_is_status_change_status_manager(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t p4)
{
    if (!ckBegin(574, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    ref::GAMEUTIL_is_status_change_status_manager(rdram, &c, runtime);
    ckMid();
    const a1::RetVF r = ty::GAMEUTIL_is_status_change_status_manager(rdram, runtime, p4);
    ckEnd(574, ctx, c, 4, r.v0, r.f0, 0x0u, 0x2u);
}
void GAMEUTIL_get_status_continue_time_status_manager(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t p4)
{
    if (!ckBegin(575, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    ref::GAMEUTIL_get_status_continue_time_status_manager(rdram, &c, runtime);
    ckMid();
    const float r = ty::GAMEUTIL_get_status_continue_time_status_manager(rdram, runtime, p4);
    ckEnd(575, ctx, c, 3, 0u, r, 0x0u, 0x0u);
}
void GAMEUTIL_get_cur_status_status_manager(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t p4)
{
    if (!ckBegin(576, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    ref::GAMEUTIL_get_cur_status_status_manager(rdram, &c, runtime);
    ckMid();
    const uint64_t r = a1_sx64(ty::GAMEUTIL_get_cur_status_status_manager(rdram, runtime, p4));
    ckEnd(576, ctx, c, 1, r, 0.0f, 0x0u, 0x0u);
}
void GAMEUTIL_get_pre_status_status_manager(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t p4)
{
    if (!ckBegin(577, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    ref::GAMEUTIL_get_pre_status_status_manager(rdram, &c, runtime);
    ckMid();
    const uint64_t r = a1_sx64(ty::GAMEUTIL_get_pre_status_status_manager(rdram, runtime, p4));
    ckEnd(577, ctx, c, 1, r, 0.0f, 0x0u, 0x0u);
}
void GAMEUTIL_get_pre_changed_status_status_manager(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t p4)
{
    if (!ckBegin(578, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    ref::GAMEUTIL_get_pre_changed_status_status_manager(rdram, &c, runtime);
    ckMid();
    const uint64_t r = a1_sx64(ty::GAMEUTIL_get_pre_changed_status_status_manager(rdram, runtime, p4));
    ckEnd(578, ctx, c, 1, r, 0.0f, 0x0u, 0x0u);
}
void getSightOnRevCntrlLR(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (!ckBegin(579, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    ref::getSightOnRevCntrlLR(rdram, &c, runtime);
    ckMid();
    const uint64_t r = a1_sx64(ty::getSightOnRevCntrlLR(rdram, runtime));
    ckEnd(579, ctx, c, 1, r, 0.0f, 0x0u, 0x0u);
}
void getSightOnRevCntrlUD(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (!ckBegin(580, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    ref::getSightOnRevCntrlUD(rdram, &c, runtime);
    ckMid();
    const uint64_t r = a1_sx64(ty::getSightOnRevCntrlUD(rdram, runtime));
    ckEnd(580, ctx, c, 1, r, 0.0f, 0x0u, 0x0u);
}
void getFreeModeRevControlLR(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (!ckBegin(581, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    ref::getFreeModeRevControlLR(rdram, &c, runtime);
    ckMid();
    const uint64_t r = a1_sx64(ty::getFreeModeRevControlLR(rdram, runtime));
    ckEnd(581, ctx, c, 1, r, 0.0f, 0x0u, 0x0u);
}
void getFreeModeRevControlUD(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (!ckBegin(582, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    ref::getFreeModeRevControlUD(rdram, &c, runtime);
    ckMid();
    const uint64_t r = a1_sx64(ty::getFreeModeRevControlUD(rdram, runtime));
    ckEnd(582, ctx, c, 1, r, 0.0f, 0x0u, 0x0u);
}
void getFreeModeRevAngleVelRatio(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (!ckBegin(583, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    ref::getFreeModeRevAngleVelRatio(rdram, &c, runtime);
    ckMid();
    const uint64_t r = a1_sx64(ty::getFreeModeRevAngleVelRatio(rdram, runtime));
    ckEnd(583, ctx, c, 1, r, 0.0f, 0x0u, 0x0u);
}
void GetGameTimeScaleRatio(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (!ckBegin(584, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    ref::GetGameTimeScaleRatio(rdram, &c, runtime);
    ckMid();
    const a1::RetVF r = ty::GetGameTimeScaleRatio(rdram, runtime);
    ckEnd(584, ctx, c, 4, r.v0, r.f0, 0x0u, 0x0u);
}
void GetCurrentGameAnimeTimeScaleRatio(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (!ckBegin(585, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    ref::GetCurrentGameAnimeTimeScaleRatio(rdram, &c, runtime);
    ckMid();
    const a1::RetVF r = ty::GetCurrentGameAnimeTimeScaleRatio(rdram, runtime);
    ckEnd(585, ctx, c, 4, r.v0, r.f0, 0x0u, 0x0u);
}
void GetGameAnimeTimeScaleRatio(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (!ckBegin(586, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    ref::GetGameAnimeTimeScaleRatio(rdram, &c, runtime);
    ckMid();
    const a1::RetVF r = ty::GetGameAnimeTimeScaleRatio(rdram, runtime);
    ckEnd(586, ctx, c, 4, r.v0, r.f0, 0x0u, 0x0u);
}
void GetPlayerControlButtonContext(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (!ckBegin(587, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    ref::GetPlayerControlButtonContext(rdram, &c, runtime);
    ckMid();
    const uint64_t r = a1_sx64(ty::GetPlayerControlButtonContext(rdram, runtime));
    ckEnd(587, ctx, c, 1, r, 0.0f, 0x0u, 0x0u);
}
void RestoreDefaultGameAnimeTimeScaleRatio(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (!ckBegin(588, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    ref::RestoreDefaultGameAnimeTimeScaleRatio(rdram, &c, runtime);
    ckMid();
    const a1::RetVF r = ty::RestoreDefaultGameAnimeTimeScaleRatio(rdram, runtime);
    ckEnd(588, ctx, c, 4, r.v0, r.f0, 0x2u, 0x0u);
}
void SetDefaultGameAnimeTimeScaleRatio(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, float pf12)
{
    if (!ckBegin(589, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    ref::SetDefaultGameAnimeTimeScaleRatio(rdram, &c, runtime);
    ckMid();
    const uint64_t r = a1_sx64(ty::SetDefaultGameAnimeTimeScaleRatio(rdram, runtime, pf12));
    ckEnd(589, ctx, c, 1, r, 0.0f, 0x0u, 0x0u);
}
void getFrameRateMode(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (!ckBegin(590, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    ref::getFrameRateMode(rdram, &c, runtime);
    ckMid();
    const uint64_t r = a1_sx64(ty::getFrameRateMode(rdram, runtime));
    ckEnd(590, ctx, c, 1, r, 0.0f, 0x0u, 0x0u);
}
void setDefaultPlayerControlButton(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint64_t p4)
{
    if (!ckBegin(591, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    ref::setDefaultPlayerControlButton(rdram, &c, runtime);
    ckMid();
    const uint64_t r = (ty::setDefaultPlayerControlButton(rdram, runtime, p4));
    ckEnd(591, ctx, c, 2, r, 0.0f, 0xf8u, 0x0u);
}
void getFadeValue(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (!ckBegin(592, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    ref::getFadeValue(rdram, &c, runtime);
    ckMid();
    const a1::RetVF r = ty::getFadeValue(rdram, runtime);
    ckEnd(592, ctx, c, 4, r.v0, r.f0, 0x0u, 0x0u);
}
void set_yaw_pitch_input_border_parameter_nico_yaw_pitch_cntrl(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t p4, float pf12, float pf13, float pf14, float pf15)
{
    if (!ckBegin(593, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    ref::set_yaw_pitch_input_border_parameter_nico_yaw_pitch_cntrl(rdram, &c, runtime);
    ckMid();
    ty::set_yaw_pitch_input_border_parameter_nico_yaw_pitch_cntrl(rdram, runtime, p4, pf12, pf13, pf14, pf15);
    ckEnd(593, ctx, c, 0, 0u, 0.0f, 0x0u, 0x0u);
}
void set_base_yaw_parameter_nico_yaw_pitch_cntrl(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t p4, uint32_t p5, float pf12, float pf13, float pf14, float pf15, float pf16, float pf17)
{
    if (!ckBegin(594, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    ref::set_base_yaw_parameter_nico_yaw_pitch_cntrl(rdram, &c, runtime);
    ckMid();
    const float r = ty::set_base_yaw_parameter_nico_yaw_pitch_cntrl(rdram, runtime, p4, p5, pf12, pf13, pf14, pf15, pf16, pf17);
    ckEnd(594, ctx, c, 3, 0u, r, 0x2u, 0x1f000u);
}
void set_yaw_parameter_effective_ratio_nico_yaw_pitch_cntrl(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t p4, float pf12)
{
    if (!ckBegin(595, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    ref::set_yaw_parameter_effective_ratio_nico_yaw_pitch_cntrl(rdram, &c, runtime);
    ckMid();
    ty::set_yaw_parameter_effective_ratio_nico_yaw_pitch_cntrl(rdram, runtime, p4, pf12);
    ckEnd(595, ctx, c, 0, 0u, 0.0f, 0x0u, 0x0u);
}
void set_pitch_parameter_effective_ratio_nico_yaw_pitch_cntrl(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t p4, float pf12)
{
    if (!ckBegin(596, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    ref::set_pitch_parameter_effective_ratio_nico_yaw_pitch_cntrl(rdram, &c, runtime);
    ckMid();
    ty::set_pitch_parameter_effective_ratio_nico_yaw_pitch_cntrl(rdram, runtime, p4, pf12);
    ckEnd(596, ctx, c, 0, 0u, 0.0f, 0x0u, 0x0u);
}
void set_temp_stop_base_rot_flag_nico_yaw_pitch_cntrl(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t p4, uint32_t p5)
{
    if (!ckBegin(597, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    ref::set_temp_stop_base_rot_flag_nico_yaw_pitch_cntrl(rdram, &c, runtime);
    ckMid();
    ty::set_temp_stop_base_rot_flag_nico_yaw_pitch_cntrl(rdram, runtime, p4, p5);
    ckEnd(597, ctx, c, 0, 0u, 0.0f, 0x0u, 0x0u);
}
void is_enable_base_yaw_rot_nico_yaw_pitch_cntrl(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t p4)
{
    if (!ckBegin(598, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    ref::is_enable_base_yaw_rot_nico_yaw_pitch_cntrl(rdram, &c, runtime);
    ckMid();
    const uint64_t r = a1_sx64(ty::is_enable_base_yaw_rot_nico_yaw_pitch_cntrl(rdram, runtime, p4));
    ckEnd(598, ctx, c, 1, r, 0.0f, 0x0u, 0x0u);
}
void is_enable_base_pitch_rot_nico_yaw_pitch_cntrl(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t p4)
{
    if (!ckBegin(599, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    ref::is_enable_base_pitch_rot_nico_yaw_pitch_cntrl(rdram, &c, runtime);
    ckMid();
    const uint64_t r = a1_sx64(ty::is_enable_base_pitch_rot_nico_yaw_pitch_cntrl(rdram, runtime, p4));
    ckEnd(599, ctx, c, 1, r, 0.0f, 0x0u, 0x0u);
}
void get_base_yaw_move_nico_yaw_pitch_cntrl(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t p4)
{
    if (!ckBegin(600, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    ref::get_base_yaw_move_nico_yaw_pitch_cntrl(rdram, &c, runtime);
    ckMid();
    const float r = ty::get_base_yaw_move_nico_yaw_pitch_cntrl(rdram, runtime, p4);
    ckEnd(600, ctx, c, 3, 0u, r, 0x0u, 0x0u);
}
void set_base_yaw_nico_yaw_pitch_cntrl(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t p4, float pf12)
{
    if (!ckBegin(601, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    ref::set_base_yaw_nico_yaw_pitch_cntrl(rdram, &c, runtime);
    ckMid();
    ty::set_base_yaw_nico_yaw_pitch_cntrl(rdram, runtime, p4, pf12);
    ckEnd(601, ctx, c, 0, 0u, 0.0f, 0x0u, 0x0u);
}
void set_base_pitch_nico_yaw_pitch_cntrl(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t p4, float pf12)
{
    if (!ckBegin(602, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    ref::set_base_pitch_nico_yaw_pitch_cntrl(rdram, &c, runtime);
    ckMid();
    ty::set_base_pitch_nico_yaw_pitch_cntrl(rdram, runtime, p4, pf12);
    ckEnd(602, ctx, c, 0, 0u, 0.0f, 0x0u, 0x0u);
}
void get_base_yaw_nico_yaw_pitch_cntrl(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t p4)
{
    if (!ckBegin(603, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    ref::get_base_yaw_nico_yaw_pitch_cntrl(rdram, &c, runtime);
    ckMid();
    const float r = ty::get_base_yaw_nico_yaw_pitch_cntrl(rdram, runtime, p4);
    ckEnd(603, ctx, c, 3, 0u, r, 0x0u, 0x0u);
}
void get_base_pitch_nico_yaw_pitch_cntrl(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t p4)
{
    if (!ckBegin(604, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    ref::get_base_pitch_nico_yaw_pitch_cntrl(rdram, &c, runtime);
    ckMid();
    const float r = ty::get_base_pitch_nico_yaw_pitch_cntrl(rdram, runtime, p4);
    ckEnd(604, ctx, c, 3, 0u, r, 0x0u, 0x0u);
}
void backStageGetBossSaveArea(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (!ckBegin(605, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    ref::backStageGetBossSaveArea(rdram, &c, runtime);
    ckMid();
    const uint64_t r = a1_sx64(ty::backStageGetBossSaveArea(rdram, runtime));
    ckEnd(605, ctx, c, 1, r, 0.0f, 0x0u, 0x0u);
}
void backStageInit(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (!ckBegin(606, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    ref::backStageInit(rdram, &c, runtime);
    ckMid();
    const uint64_t r = a1_sx64(ty::backStageInit(rdram, runtime));
    ckEnd(606, ctx, c, 1, r, 0.0f, 0x8u, 0x0u);
}
void bossStatGetDeadBoss(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (!ckBegin(607, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    ref::bossStatGetDeadBoss(rdram, &c, runtime);
    ckMid();
    const uint64_t r = a1_sx64(ty::bossStatGetDeadBoss(rdram, runtime));
    ckEnd(607, ctx, c, 1, r, 0.0f, 0x8u, 0x0u);
}
void CheckCamFinalizerReady(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (!ckBegin(608, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    ref::CheckCamFinalizerReady(rdram, &c, runtime);
    ckMid();
    const uint64_t r = a1_sx64(ty::CheckCamFinalizerReady(rdram, runtime));
    ckEnd(608, ctx, c, 1, r, 0.0f, 0x8u, 0x0u);
}
void SetCamFinalizerReady(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t p4)
{
    if (!ckBegin(609, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    ref::SetCamFinalizerReady(rdram, &c, runtime);
    ckMid();
    const uint64_t r = a1_sx64(ty::SetCamFinalizerReady(rdram, runtime, p4));
    ckEnd(609, ctx, c, 1, r, 0.0f, 0x0u, 0x0u);
}
void PauseCamFinalizer(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t p4)
{
    if (!ckBegin(610, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    ref::PauseCamFinalizer(rdram, &c, runtime);
    ckMid();
    const uint64_t r = a1_sx64(ty::PauseCamFinalizer(rdram, runtime, p4));
    ckEnd(610, ctx, c, 1, r, 0.0f, 0x0u, 0x0u);
}
void CheckDemoCameraOrientFinished(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t p4)
{
    if (!ckBegin(611, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    ref::CheckDemoCameraOrientFinished(rdram, &c, runtime);
    ckMid();
    const uint64_t r = a1_sx64(ty::CheckDemoCameraOrientFinished(rdram, runtime, p4));
    ckEnd(611, ctx, c, 1, r, 0.0f, 0x0u, 0x0u);
}
void getDemoEnableFlag(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (!ckBegin(612, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    ref::getDemoEnableFlag(rdram, &c, runtime);
    ckMid();
    const uint64_t r = a1_sx64(ty::getDemoEnableFlag(rdram, runtime));
    ckEnd(612, ctx, c, 1, r, 0.0f, 0x0u, 0x0u);
}
void demoscrCheckDemoFinish(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (!ckBegin(613, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    ref::demoscrCheckDemoFinish(rdram, &c, runtime);
    ckMid();
    const uint64_t r = a1_sx64(ty::demoscrCheckDemoFinish(rdram, runtime));
    ckEnd(613, ctx, c, 1, r, 0.0f, 0x8u, 0x0u);
}
void demoscrCheckDemoCutSkip(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (!ckBegin(614, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    ref::demoscrCheckDemoCutSkip(rdram, &c, runtime);
    ckMid();
    const uint64_t r = (ty::demoscrCheckDemoCutSkip(rdram, runtime));
    ckEnd(614, ctx, c, 2, r, 0.0f, 0x0u, 0x0u);
}
void endingflowTickAdpcmSeq(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t p4)
{
    if (!ckBegin(615, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    ref::endingflowTickAdpcmSeq(rdram, &c, runtime);
    ckMid();
    const uint64_t r = a1_sx64(ty::endingflowTickAdpcmSeq(rdram, runtime, p4));
    ckEnd(615, ctx, c, 1, r, 0.0f, 0x0u, 0x0u);
}
void gamecore_sub_0134D920(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t p8)
{
    if (!ckBegin(616, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    ref::gamecore_sub_0134D920(rdram, &c, runtime);
    ckMid();
    const uint64_t r = a1_sx64(ty::gamecore_sub_0134D920(rdram, runtime, p8));
    ckEnd(616, ctx, c, 1, r, 0.0f, 0x8u, 0x0u);
}
void initFadeParam(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t p4, float pf12)
{
    if (!ckBegin(617, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    ref::initFadeParam(rdram, &c, runtime);
    ckMid();
    const a1::RetVF r = ty::initFadeParam(rdram, runtime, p4, pf12);
    ckEnd(617, ctx, c, 4, r.v0, r.f0, 0x0u, 0x2u);
}
void setFadeParam(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t p4, uint32_t p5, float pf12, float pf13)
{
    if (!ckBegin(618, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    ref::setFadeParam(rdram, &c, runtime);
    ckMid();
    const a1::RetVF r = ty::setFadeParam(rdram, runtime, p4, p5, pf12, pf13);
    ckEnd(618, ctx, c, 4, r.v0, r.f0, 0x0u, 0x6u);
}
void calcFadeParam(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t p4)
{
    if (!ckBegin(619, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    ref::calcFadeParam(rdram, &c, runtime);
    ckMid();
    const a1::RetVF r = ty::calcFadeParam(rdram, runtime, p4);
    ckEnd(619, ctx, c, 4, r.v0, r.f0, 0x0u, 0x1eu);
}
void checkFadeParam(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t p4)
{
    if (!ckBegin(620, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    ref::checkFadeParam(rdram, &c, runtime);
    ckMid();
    const a1::RetVF r = ty::checkFadeParam(rdram, runtime, p4);
    ckEnd(620, ctx, c, 4, r.v0, r.f0, 0x0u, 0x2u);
}
void fadeChkStat(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint64_t p4)
{
    if (!ckBegin(621, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    ref::fadeChkStat(rdram, &c, runtime);
    ckMid();
    const uint64_t r = (ty::fadeChkStat(rdram, runtime, p4));
    ckEnd(621, ctx, c, 2, r, 0.0f, 0x8u, 0x0u);
}
void setTreeId(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t p4)
{
    if (!ckBegin(622, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    ref::setTreeId(rdram, &c, runtime);
    ckMid();
    const uint64_t r = a1_sx64(ty::setTreeId(rdram, runtime, p4));
    ckEnd(622, ctx, c, 1, r, 0.0f, 0x8u, 0x0u);
}
void _optionWorkChk(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t p4)
{
    if (!ckBegin(623, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    ref::_optionWorkChk(rdram, &c, runtime);
    ckMid();
    const uint64_t r = a1_sx64(ty::_optionWorkChk(rdram, runtime, p4));
    ckEnd(623, ctx, c, 1, r, 0.0f, 0x10u, 0x0u);
}
void _optionWorkPtr(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t p4)
{
    if (!ckBegin(624, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    ref::_optionWorkPtr(rdram, &c, runtime);
    ckMid();
    const uint64_t r = a1_sx64(ty::_optionWorkPtr(rdram, runtime, p4));
    ckEnd(624, ctx, c, 1, r, 0.0f, 0x2u, 0x0u);
}
void _gameIWorkSet(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t p4, uint32_t p5)
{
    if (!ckBegin(625, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    ref::_gameIWorkSet(rdram, &c, runtime);
    ckMid();
    const uint64_t r = a1_sx64(ty::_gameIWorkSet(rdram, runtime, p4, p5));
    ckEnd(625, ctx, c, 1, r, 0.0f, 0x18u, 0x0u);
}
void _gameWorkChk(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t p4)
{
    if (!ckBegin(626, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    ref::_gameWorkChk(rdram, &c, runtime);
    ckMid();
    const uint64_t r = a1_sx64(ty::_gameWorkChk(rdram, runtime, p4));
    ckEnd(626, ctx, c, 1, r, 0.0f, 0x10u, 0x0u);
}
void _gameIWorkChk(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t p4)
{
    if (!ckBegin(627, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    ref::_gameIWorkChk(rdram, &c, runtime);
    ckMid();
    const uint64_t r = a1_sx64(ty::_gameIWorkChk(rdram, runtime, p4));
    ckEnd(627, ctx, c, 1, r, 0.0f, 0x18u, 0x0u);
}
void setBossClearCnt(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t p4)
{
    if (!ckBegin(628, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    ref::setBossClearCnt(rdram, &c, runtime);
    ckMid();
    const uint64_t r = a1_sx64(ty::setBossClearCnt(rdram, runtime, p4));
    ckEnd(628, ctx, c, 1, r, 0.0f, 0x0u, 0x0u);
}
void getGameClearCnt(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (!ckBegin(629, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    ref::getGameClearCnt(rdram, &c, runtime);
    ckMid();
    const uint64_t r = a1_sx64(ty::getGameClearCnt(rdram, runtime));
    ckEnd(629, ctx, c, 1, r, 0.0f, 0x8u, 0x0u);
}
void getGameHardMode(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (!ckBegin(630, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    ref::getGameHardMode(rdram, &c, runtime);
    ckMid();
    const uint64_t r = a1_sx64(ty::getGameHardMode(rdram, runtime));
    ckEnd(630, ctx, c, 1, r, 0.0f, 0x8u, 0x0u);
}
void setSavedClock(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t p4)
{
    if (!ckBegin(631, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    ref::setSavedClock(rdram, &c, runtime);
    ckMid();
    const uint64_t r = a1_sx64(ty::setSavedClock(rdram, runtime, p4));
    ckEnd(631, ctx, c, 1, r, 0.0f, 0x8u, 0x0u);
}
void getSavedPlaceId(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (!ckBegin(632, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    ref::getSavedPlaceId(rdram, &c, runtime);
    ckMid();
    const uint64_t r = a1_sx64(ty::getSavedPlaceId(rdram, runtime));
    ckEnd(632, ctx, c, 1, r, 0.0f, 0x8u, 0x0u);
}
void getOptionDataAdrsAndSize(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t p4)
{
    if (!ckBegin(633, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    ref::getOptionDataAdrsAndSize(rdram, &c, runtime);
    ckMid();
    const uint64_t r = a1_sx64(ty::getOptionDataAdrsAndSize(rdram, runtime, p4));
    ckEnd(633, ctx, c, 1, r, 0.0f, 0x8u, 0x0u);
}
void gameflowSelectGameoverMenu(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t p4)
{
    if (!ckBegin(634, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    ref::gameflowSelectGameoverMenu(rdram, &c, runtime);
    ckMid();
    const uint64_t r = a1_sx64(ty::gameflowSelectGameoverMenu(rdram, runtime, p4));
    ckEnd(634, ctx, c, 1, r, 0.0f, 0x0u, 0x0u);
}
void gameflowSetPauseEnable(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint64_t p4)
{
    if (!ckBegin(635, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    ref::gameflowSetPauseEnable(rdram, &c, runtime);
    ckMid();
    const uint64_t r = a1_sx64(ty::gameflowSetPauseEnable(rdram, runtime, p4));
    ckEnd(635, ctx, c, 1, r, 0.0f, 0x8u, 0x0u);
}
void gameflowGameOver(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (!ckBegin(636, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    ref::gameflowGameOver(rdram, &c, runtime);
    ckMid();
    const uint64_t r = a1_sx64(ty::gameflowGameOver(rdram, runtime));
    ckEnd(636, ctx, c, 1, r, 0.0f, 0x8u, 0x0u);
}
void gameflowGetJumpSekiban(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (!ckBegin(637, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    ref::gameflowGetJumpSekiban(rdram, &c, runtime);
    ckMid();
    const uint64_t r = a1_sx64(ty::gameflowGetJumpSekiban(rdram, runtime));
    ckEnd(637, ctx, c, 1, r, 0.0f, 0x8u, 0x0u);
}
void gameflowSetJumpSekiban(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t p4)
{
    if (!ckBegin(638, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    ref::gameflowSetJumpSekiban(rdram, &c, runtime);
    ckMid();
    const uint64_t r = a1_sx64(ty::gameflowSetJumpSekiban(rdram, runtime, p4));
    ckEnd(638, ctx, c, 1, r, 0.0f, 0x0u, 0x0u);
}
void gamecore_sub_01352770(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (!ckBegin(639, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    ref::gamecore_sub_01352770(rdram, &c, runtime);
    ckMid();
    const uint64_t r = a1_sx64(ty::gamecore_sub_01352770(rdram, runtime));
    ckEnd(639, ctx, c, 1, r, 0.0f, 0x8u, 0x0u);
}
void gStatGetOmakeStatAdrs(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (!ckBegin(640, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    ref::gStatGetOmakeStatAdrs(rdram, &c, runtime);
    ckMid();
    const uint64_t r = a1_sx64(ty::gStatGetOmakeStatAdrs(rdram, runtime));
    ckEnd(640, ctx, c, 1, r, 0.0f, 0x0u, 0x0u);
}
void gStatGObjDispChk(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t p4)
{
    if (!ckBegin(641, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    ref::gStatGObjDispChk(rdram, &c, runtime);
    ckMid();
    const uint64_t r = a1_sx64(ty::gStatGObjDispChk(rdram, runtime, p4));
    ckEnd(641, ctx, c, 1, r, 0.0f, 0x0u, 0x0u);
}
void gStatSetGObjAdrs(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t p4, uint32_t p5)
{
    if (!ckBegin(642, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    ref::gStatSetGObjAdrs(rdram, &c, runtime);
    ckMid();
    ty::gStatSetGObjAdrs(rdram, runtime, p4, p5);
    ckEnd(642, ctx, c, 0, 0u, 0.0f, 0x0u, 0x0u);
}
void gStatGetPlayerAccessoriesDamgeRaitoToPlayer(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (!ckBegin(643, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    ref::gStatGetPlayerAccessoriesDamgeRaitoToPlayer(rdram, &c, runtime);
    ckMid();
    const a1::RetVF r = ty::gStatGetPlayerAccessoriesDamgeRaitoToPlayer(rdram, runtime);
    ckEnd(643, ctx, c, 4, r.v0, r.f0, 0x2u, 0x2u);
}
void gStatGetPlayerAccessoriesDamgeRaitoToBoss(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (!ckBegin(644, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    ref::gStatGetPlayerAccessoriesDamgeRaitoToBoss(rdram, &c, runtime);
    ckMid();
    const a1::RetVF r = ty::gStatGetPlayerAccessoriesDamgeRaitoToBoss(rdram, runtime);
    ckEnd(644, ctx, c, 4, r.v0, r.f0, 0x2u, 0x2u);
}
void gstatBootMcSetExistNicoData(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t p4, uint32_t p5)
{
    if (!ckBegin(645, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    ref::gstatBootMcSetExistNicoData(rdram, &c, runtime);
    ckMid();
    const uint64_t r = a1_sx64(ty::gstatBootMcSetExistNicoData(rdram, runtime, p4, p5));
    ckEnd(645, ctx, c, 1, r, 0.0f, 0x8u, 0x0u);
}
void gstatBootMcSetExistIcoData(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t p4)
{
    if (!ckBegin(646, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    ref::gstatBootMcSetExistIcoData(rdram, &c, runtime);
    ckMid();
    const uint64_t r = a1_sx64(ty::gstatBootMcSetExistIcoData(rdram, runtime, p4));
    ckEnd(646, ctx, c, 1, r, 0.0f, 0x0u, 0x0u);
}
void gstatBootMcChkExistIcoData(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (!ckBegin(647, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    ref::gstatBootMcChkExistIcoData(rdram, &c, runtime);
    ckMid();
    const uint64_t r = a1_sx64(ty::gstatBootMcChkExistIcoData(rdram, runtime));
    ckEnd(647, ctx, c, 1, r, 0.0f, 0x8u, 0x0u);
}
void jumphokoraflowDemoStart(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (!ckBegin(648, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    ref::jumphokoraflowDemoStart(rdram, &c, runtime);
    ckMid();
    const uint64_t r = a1_sx64(ty::jumphokoraflowDemoStart(rdram, runtime));
    ckEnd(648, ctx, c, 1, r, 0.0f, 0x8u, 0x0u);
}
void jumphokoraflowDemoEnd(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t p4)
{
    if (!ckBegin(649, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    ref::jumphokoraflowDemoEnd(rdram, &c, runtime);
    ckMid();
    const uint64_t r = a1_sx64(ty::jumphokoraflowDemoEnd(rdram, runtime, p4));
    ckEnd(649, ctx, c, 1, r, 0.0f, 0x8u, 0x0u);
}
void jumphokoraGetDemoInfo(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t p4, uint32_t p5)
{
    if (!ckBegin(650, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    ref::jumphokoraGetDemoInfo(rdram, &c, runtime);
    ckMid();
    const uint64_t r = a1_sx64(ty::jumphokoraGetDemoInfo(rdram, runtime, p4, p5));
    ckEnd(650, ctx, c, 1, r, 0.0f, 0x48u, 0x0u);
}
void gcCheckStageLoadFinish(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (!ckBegin(651, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    ref::gcCheckStageLoadFinish(rdram, &c, runtime);
    ckMid();
    const uint64_t r = a1_sx64(ty::gcCheckStageLoadFinish(rdram, runtime));
    ckEnd(651, ctx, c, 1, r, 0.0f, 0x8u, 0x0u);
}
void opflowOp1End(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (!ckBegin(652, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    ref::opflowOp1End(rdram, &c, runtime);
    ckMid();
    const uint64_t r = a1_sx64(ty::opflowOp1End(rdram, runtime));
    ckEnd(652, ctx, c, 1, r, 0.0f, 0x8u, 0x0u);
}
void opflowOp2Start(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (!ckBegin(653, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    ref::opflowOp2Start(rdram, &c, runtime);
    ckMid();
    const uint64_t r = a1_sx64(ty::opflowOp2Start(rdram, runtime));
    ckEnd(653, ctx, c, 1, r, 0.0f, 0x8u, 0x0u);
}
void opflowOp2End(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t p4)
{
    if (!ckBegin(654, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    ref::opflowOp2End(rdram, &c, runtime);
    ckMid();
    const uint64_t r = a1_sx64(ty::opflowOp2End(rdram, runtime, p4));
    ckEnd(654, ctx, c, 1, r, 0.0f, 0x38u, 0x0u);
}
void opflowChkTitleDemoStart(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (!ckBegin(655, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    ref::opflowChkTitleDemoStart(rdram, &c, runtime);
    ckMid();
    const uint64_t r = a1_sx64(ty::opflowChkTitleDemoStart(rdram, runtime));
    ckEnd(655, ctx, c, 1, r, 0.0f, 0x8u, 0x0u);
}
void opflowTitleDmoeEnd(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (!ckBegin(656, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    ref::opflowTitleDmoeEnd(rdram, &c, runtime);
    ckMid();
    const uint64_t r = a1_sx64(ty::opflowTitleDmoeEnd(rdram, runtime));
    ckEnd(656, ctx, c, 1, r, 0.0f, 0x8u, 0x0u);
}
void opflowChkAbort(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (!ckBegin(657, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    ref::opflowChkAbort(rdram, &c, runtime);
    ckMid();
    const uint64_t r = a1_sx64(ty::opflowChkAbort(rdram, runtime));
    ckEnd(657, ctx, c, 1, r, 0.0f, 0x8u, 0x0u);
}
void opflowChkLoadStart(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (!ckBegin(658, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    ref::opflowChkLoadStart(rdram, &c, runtime);
    ckMid();
    const uint64_t r = a1_sx64(ty::opflowChkLoadStart(rdram, runtime));
    ckEnd(658, ctx, c, 1, r, 0.0f, 0x8u, 0x0u);
}
void getPlayerHp(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (!ckBegin(659, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    ref::getPlayerHp(rdram, &c, runtime);
    ckMid();
    const a1::RetVF r = ty::getPlayerHp(rdram, runtime);
    ckEnd(659, ctx, c, 4, r.v0, r.f0, 0x0u, 0x0u);
}
void getPlayerGrip(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (!ckBegin(660, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    ref::getPlayerGrip(rdram, &c, runtime);
    ckMid();
    const a1::RetVF r = ty::getPlayerGrip(rdram, runtime);
    ckEnd(660, ctx, c, 4, r.v0, r.f0, 0x0u, 0x0u);
}
void initPowerWork(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (!ckBegin(661, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    ref::initPowerWork(rdram, &c, runtime);
    ckMid();
    const uint64_t r = a1_sx64(ty::initPowerWork(rdram, runtime));
    ckEnd(661, ctx, c, 1, r, 0.0f, 0x0u, 0x0u);
}
void resetAttackPower(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (!ckBegin(662, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    ref::resetAttackPower(rdram, &c, runtime);
    ckMid();
    const uint64_t r = a1_sx64(ty::resetAttackPower(rdram, runtime));
    ckEnd(662, ctx, c, 1, r, 0.0f, 0x0u, 0x0u);
}
void getAttackPowerRatio(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (!ckBegin(663, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    ref::getAttackPowerRatio(rdram, &c, runtime);
    ckMid();
    const a1::RetVF r = ty::getAttackPowerRatio(rdram, runtime);
    ckEnd(663, ctx, c, 4, r.v0, r.f0, 0x2u, 0x2u);
}
void setFullPower(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (!ckBegin(664, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    ref::setFullPower(rdram, &c, runtime);
    ckMid();
    const a1::RetVF r = ty::setFullPower(rdram, runtime);
    ckEnd(664, ctx, c, 4, r.v0, r.f0, 0x2u, 0x0u);
}
void gamecore_sub_01358878(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (!ckBegin(665, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    ref::gamecore_sub_01358878(rdram, &c, runtime);
    ckMid();
    const uint64_t r = a1_sx64(ty::gamecore_sub_01358878(rdram, runtime));
    ckEnd(665, ctx, c, 1, r, 0.0f, 0x0u, 0x0u);
}
void getAreaSize(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (!ckBegin(666, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    ref::getAreaSize(rdram, &c, runtime);
    ckMid();
    const a1::RetVF r = ty::getAreaSize(rdram, runtime);
    ckEnd(666, ctx, c, 4, r.v0, r.f0, 0x0u, 0x2u);
}
void getPosOnMap(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t p4, uint32_t p5, uint32_t p6)
{
    if (!ckBegin(667, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    ref::getPosOnMap(rdram, &c, runtime);
    ckMid();
    const a1::RetVF r = ty::getPosOnMap(rdram, runtime, p4, p5, p6);
    ckEnd(667, ctx, c, 4, r.v0, r.f0, 0x0u, 0x6u);
}
void getSeamlessStageHighModelDispDistDefault(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (!ckBegin(668, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    ref::getSeamlessStageHighModelDispDistDefault(rdram, &c, runtime);
    ckMid();
    const uint64_t r = a1_sx64(ty::getSeamlessStageHighModelDispDistDefault(rdram, runtime));
    ckEnd(668, ctx, c, 1, r, 0.0f, 0x0u, 0x0u);
}
void getSeamlessStageMiddleModelDispDistDefault(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (!ckBegin(669, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    ref::getSeamlessStageMiddleModelDispDistDefault(rdram, &c, runtime);
    ckMid();
    const uint64_t r = a1_sx64(ty::getSeamlessStageMiddleModelDispDistDefault(rdram, runtime));
    ckEnd(669, ctx, c, 1, r, 0.0f, 0x0u, 0x0u);
}
void getSeamlessStageEnv(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (!ckBegin(670, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    ref::getSeamlessStageEnv(rdram, &c, runtime);
    ckMid();
    const uint64_t r = a1_sx64(ty::getSeamlessStageEnv(rdram, runtime));
    ckEnd(670, ctx, c, 1, r, 0.0f, 0x0u, 0x0u);
}
void forceLoadSeamlessStage(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (!ckBegin(671, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    ref::forceLoadSeamlessStage(rdram, &c, runtime);
    ckMid();
    const uint64_t r = a1_sx64(ty::forceLoadSeamlessStage(rdram, runtime));
    ckEnd(671, ctx, c, 1, r, 0.0f, 0x8u, 0x0u);
}
void SetSeamlessStageControlObject(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t p4)
{
    if (!ckBegin(672, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    ref::SetSeamlessStageControlObject(rdram, &c, runtime);
    ckMid();
    const uint64_t r = a1_sx64(ty::SetSeamlessStageControlObject(rdram, runtime, p4));
    ckEnd(672, ctx, c, 1, r, 0.0f, 0x0u, 0x0u);
}
void PauseSeamlessStageManager(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint64_t p4)
{
    if (!ckBegin(673, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    ref::PauseSeamlessStageManager(rdram, &c, runtime);
    ckMid();
    const uint64_t r = a1_sx64(ty::PauseSeamlessStageManager(rdram, runtime, p4));
    ckEnd(673, ctx, c, 1, r, 0.0f, 0x8u, 0x0u);
}
void getStageIDWithOffset(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t p4, uint32_t p5, uint32_t p6, uint32_t p7, uint32_t p8)
{
    if (!ckBegin(674, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    ref::getStageIDWithOffset(rdram, &c, runtime);
    ckMid();
    const a1::RetVF r = ty::getStageIDWithOffset(rdram, runtime, p4, p5, p6, p7, p8);
    ckEnd(674, ctx, c, 4, r.v0, r.f0, 0x0u, 0xfeu);
}
void isSeamlessStage(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (!ckBegin(675, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    ref::isSeamlessStage(rdram, &c, runtime);
    ckMid();
    const uint64_t r = a1_sx64(ty::isSeamlessStage(rdram, runtime));
    ckEnd(675, ctx, c, 1, r, 0.0f, 0x8u, 0x0u);
}
void getSpecialEffectObjWork(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t p4)
{
    if (!ckBegin(676, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    ref::getSpecialEffectObjWork(rdram, &c, runtime);
    ckMid();
    const uint64_t r = a1_sx64(ty::getSpecialEffectObjWork(rdram, runtime, p4));
    ckEnd(676, ctx, c, 1, r, 0.0f, 0x0u, 0x0u);
}
void GetWorldLayoutSheetSegHeader(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (!ckBegin(677, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    ref::GetWorldLayoutSheetSegHeader(rdram, &c, runtime);
    ckMid();
    const uint64_t r = a1_sx64(ty::GetWorldLayoutSheetSegHeader(rdram, runtime));
    ckEnd(677, ctx, c, 1, r, 0.0f, 0x8u, 0x0u);
}
void stagemanager_getSegmentVariation(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (!ckBegin(678, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    ref::stagemanager_getSegmentVariation(rdram, &c, runtime);
    ckMid();
    const uint64_t r = a1_sx64(ty::stagemanager_getSegmentVariation(rdram, runtime));
    ckEnd(678, ctx, c, 1, r, 0.0f, 0x8u, 0x0u);
}
void initWorldLayout(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t p4)
{
    if (!ckBegin(679, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    ref::initWorldLayout(rdram, &c, runtime);
    ckMid();
    const uint64_t r = a1_sx64(ty::initWorldLayout(rdram, runtime, p4));
    ckEnd(679, ctx, c, 1, r, 0.0f, 0x0u, 0x0u);
}
void bgmEnvTickProckMask(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t p4)
{
    if (!ckBegin(680, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    ref::bgmEnvTickProckMask(rdram, &c, runtime);
    ckMid();
    const uint64_t r = a1_sx64(ty::bgmEnvTickProckMask(rdram, runtime, p4));
    ckEnd(680, ctx, c, 1, r, 0.0f, 0x0u, 0x0u);
}
void bossNormalHook(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t p8)
{
    if (!ckBegin(681, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    ref::bossNormalHook(rdram, &c, runtime);
    ckMid();
    const uint64_t r = a1_sx64(ty::bossNormalHook(rdram, runtime, p8));
    ckEnd(681, ctx, c, 1, r, 0.0f, 0x0u, 0x0u);
}
void gamecore_sub_0136A388(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint64_t p4)
{
    if (!ckBegin(682, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    ref::gamecore_sub_0136A388(rdram, &c, runtime);
    ckMid();
    const uint64_t r = a1_sx64(ty::gamecore_sub_0136A388(rdram, runtime, p4));
    ckEnd(682, ctx, c, 1, r, 0.0f, 0x8u, 0x0u);
}
void bgmPlay(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t p4, float pf12)
{
    if (!ckBegin(683, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    ref::bgmPlay(rdram, &c, runtime);
    ckMid();
    const uint64_t r = a1_sx64(ty::bgmPlay(rdram, runtime, p4, pf12));
    ckEnd(683, ctx, c, 1, r, 0.0f, 0x0u, 0x0u);
}
void bgmStop(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t p4, uint64_t p5, float pf12)
{
    if (!ckBegin(684, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    ref::bgmStop(rdram, &c, runtime);
    ckMid();
    const uint64_t r = a1_sx64(ty::bgmStop(rdram, runtime, p4, p5, pf12));
    ckEnd(684, ctx, c, 1, r, 0.0f, 0x0u, 0x0u);
}
void bgmStopChk(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint64_t p4)
{
    if (!ckBegin(685, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    ref::bgmStopChk(rdram, &c, runtime);
    ckMid();
    const uint64_t r = (ty::bgmStopChk(rdram, runtime, p4));
    ckEnd(685, ctx, c, 2, r, 0.0f, 0x0u, 0x0u);
}
void bgmChangeVol(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t p4, float pf12, float pf13)
{
    if (!ckBegin(686, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    ref::bgmChangeVol(rdram, &c, runtime);
    ckMid();
    const uint64_t r = a1_sx64(ty::bgmChangeVol(rdram, runtime, p4, pf12, pf13));
    ckEnd(686, ctx, c, 1, r, 0.0f, 0x0u, 0x0u);
}
void gamecore_sub_0136E4F8(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t p4)
{
    if (!ckBegin(687, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    ref::gamecore_sub_0136E4F8(rdram, &c, runtime);
    ckMid();
    const uint64_t r = a1_sx64(ty::gamecore_sub_0136E4F8(rdram, runtime, p4));
    ckEnd(687, ctx, c, 1, r, 0.0f, 0x0u, 0x0u);
}
void gamecore_sub_0136E7A0(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t p5)
{
    if (!ckBegin(688, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    ref::gamecore_sub_0136E7A0(rdram, &c, runtime);
    ckMid();
    ty::gamecore_sub_0136E7A0(rdram, runtime, p5);
    ckEnd(688, ctx, c, 0, 0u, 0.0f, 0x0u, 0x0u);
}
void nico_IK_idDetect(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t p4)
{
    if (!ckBegin(689, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    ref::nico_IK_idDetect(rdram, &c, runtime);
    ckMid();
    const uint64_t r = a1_sx64(ty::nico_IK_idDetect(rdram, runtime, p4));
    ckEnd(689, ctx, c, 1, r, 0.0f, 0x18u, 0x0u);
}
void gamecore_sub_0137B738(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint64_t p4, uint64_t p5)
{
    if (!ckBegin(690, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    ref::gamecore_sub_0137B738(rdram, &c, runtime);
    ckMid();
    const uint64_t r = (ty::gamecore_sub_0137B738(rdram, runtime, p4, p5));
    ckEnd(690, ctx, c, 2, r, 0.0f, 0x0u, 0x0u);
}
void menuSelected(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (!ckBegin(691, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    ref::menuSelected(rdram, &c, runtime);
    ckMid();
    const uint64_t r = a1_sx64(ty::menuSelected(rdram, runtime));
    ckEnd(691, ctx, c, 1, r, 0.0f, 0x8u, 0x0u);
}
void GetDebugDispType(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (!ckBegin(692, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    ref::GetDebugDispType(rdram, &c, runtime);
    ckMid();
    const uint64_t r = (ty::GetDebugDispType(rdram, runtime));
    ckEnd(692, ctx, c, 2, r, 0.0f, 0x0u, 0x0u);
}
void is_LwsOrientObj(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t p4)
{
    if (!ckBegin(693, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    ref::is_LwsOrientObj(rdram, &c, runtime);
    ckMid();
    const uint64_t r = a1_sx64(ty::is_LwsOrientObj(rdram, runtime, p4));
    ckEnd(693, ctx, c, 1, r, 0.0f, 0x0u, 0x0u);
}
void CheckDeathTentacle(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (!ckBegin(694, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    ref::CheckDeathTentacle(rdram, &c, runtime);
    ckMid();
    const uint64_t r = a1_sx64(ty::CheckDeathTentacle(rdram, runtime));
    ckEnd(694, ctx, c, 1, r, 0.0f, 0x8u, 0x0u);
}
void getEffectBloodWorkSize(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t p4)
{
    if (!ckBegin(695, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    ref::getEffectBloodWorkSize(rdram, &c, runtime);
    ckMid();
    const uint64_t r = a1_sx64(ty::getEffectBloodWorkSize(rdram, runtime, p4));
    ckEnd(695, ctx, c, 1, r, 0.0f, 0x0u, 0x0u);
}
void getWeakPointEffectIntensity(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t p4)
{
    if (!ckBegin(696, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    ref::getWeakPointEffectIntensity(rdram, &c, runtime);
    ckMid();
    const a1::RetVF r = ty::getWeakPointEffectIntensity(rdram, runtime, p4);
    ckEnd(696, ctx, c, 4, r.v0, r.f0, 0x2u, 0x0u);
}
void getTriggerPointEffectIntensity(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t p4)
{
    if (!ckBegin(697, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    ref::getTriggerPointEffectIntensity(rdram, &c, runtime);
    ckMid();
    const a1::RetVF r = ty::getTriggerPointEffectIntensity(rdram, runtime, p4);
    ckEnd(697, ctx, c, 4, r.v0, r.f0, 0x12u, 0x0u);
}
void effectClearDamageWork(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (!ckBegin(698, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    ref::effectClearDamageWork(rdram, &c, runtime);
    ckMid();
    const uint64_t r = a1_sx64(ty::effectClearDamageWork(rdram, runtime));
    ckEnd(698, ctx, c, 1, r, 0.0f, 0x0u, 0x0u);
}
void effectGetDamageWork(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t p4)
{
    if (!ckBegin(699, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    ref::effectGetDamageWork(rdram, &c, runtime);
    ckMid();
    const uint64_t r = a1_sx64(ty::effectGetDamageWork(rdram, runtime, p4));
    ckEnd(699, ctx, c, 1, r, 0.0f, 0x8u, 0x0u);
}
void gamecore_sub_013886B8(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t p4)
{
    if (!ckBegin(700, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    ref::gamecore_sub_013886B8(rdram, &c, runtime);
    ckMid();
    const a1::RetVF r = ty::gamecore_sub_013886B8(rdram, runtime, p4);
    ckEnd(700, ctx, c, 4, r.v0, r.f0, 0x0u, 0x6u);
}
void getFallenLeafSize(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint64_t p4)
{
    if (!ckBegin(701, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    ref::getFallenLeafSize(rdram, &c, runtime);
    ckMid();
    const uint64_t r = a1_sx64(ty::getFallenLeafSize(rdram, runtime, p4));
    ckEnd(701, ctx, c, 1, r, 0.0f, 0xf8u, 0x0u);
}
void getArrowWaterSize(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t p4)
{
    if (!ckBegin(702, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    ref::getArrowWaterSize(rdram, &c, runtime);
    ckMid();
    const uint64_t r = a1_sx64(ty::getArrowWaterSize(rdram, runtime, p4));
    ckEnd(702, ctx, c, 1, r, 0.0f, 0x78u, 0x0u);
}
void gamecore_sub_0138F138(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t p6, uint64_t p7)
{
    if (!ckBegin(703, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    ref::gamecore_sub_0138F138(rdram, &c, runtime);
    ckMid();
    ty::gamecore_sub_0138F138(rdram, runtime, p6, p7);
    ckEnd(703, ctx, c, 0, 0u, 0.0f, 0x4cu, 0x0u);
}
void setHorseAIStopFlag(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t p4)
{
    if (!ckBegin(704, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    ref::setHorseAIStopFlag(rdram, &c, runtime);
    ckMid();
    const uint64_t r = a1_sx64(ty::setHorseAIStopFlag(rdram, runtime, p4));
    ckEnd(704, ctx, c, 1, r, 0.0f, 0x0u, 0x0u);
}
void getHorseStandByMode(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (!ckBegin(705, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    ref::getHorseStandByMode(rdram, &c, runtime);
    ckMid();
    const uint64_t r = a1_sx64(ty::getHorseStandByMode(rdram, runtime));
    ckEnd(705, ctx, c, 1, r, 0.0f, 0x8u, 0x0u);
}
void SCRFuncHorseStandByFlag(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t p5)
{
    if (!ckBegin(706, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    ref::SCRFuncHorseStandByFlag(rdram, &c, runtime);
    ckMid();
    const uint64_t r = a1_sx64(ty::SCRFuncHorseStandByFlag(rdram, runtime, p5));
    ckEnd(706, ctx, c, 1, r, 0.0f, 0x0u, 0x0u);
}
void ColCont_Reset(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t p4)
{
    if (!ckBegin(707, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    ref::ColCont_Reset(rdram, &c, runtime);
    ckMid();
    ty::ColCont_Reset(rdram, runtime, p4);
    ckEnd(707, ctx, c, 0, 0u, 0.0f, 0x0u, 0x0u);
}
void ColCont2_IsColStatus(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t p4, uint64_t p5, uint64_t p6)
{
    if (!ckBegin(708, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    ref::ColCont2_IsColStatus(rdram, &c, runtime);
    ckMid();
    const uint64_t r = (ty::ColCont2_IsColStatus(rdram, runtime, p4, p5, p6));
    ckEnd(708, ctx, c, 2, r, 0.0f, 0x8u, 0x0u);
}
void ColCont2_IsEventColStatus(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t p4, uint64_t p5, uint64_t p6)
{
    if (!ckBegin(709, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    ref::ColCont2_IsEventColStatus(rdram, &c, runtime);
    ckMid();
    const uint64_t r = (ty::ColCont2_IsEventColStatus(rdram, runtime, p4, p5, p6));
    ckEnd(709, ctx, c, 2, r, 0.0f, 0x8u, 0x0u);
}
void MotSpdCtrl_Reset(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t p4)
{
    if (!ckBegin(710, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    ref::MotSpdCtrl_Reset(rdram, &c, runtime);
    ckMid();
    ty::MotSpdCtrl_Reset(rdram, runtime, p4);
    ckEnd(710, ctx, c, 0, 0u, 0.0f, 0x0u, 0x0u);
}
void MotSpdCtrl_SetParam(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t p4, uint32_t p5)
{
    if (!ckBegin(711, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    ref::MotSpdCtrl_SetParam(rdram, &c, runtime);
    ckMid();
    ty::MotSpdCtrl_SetParam(rdram, runtime, p4, p5);
    ckEnd(711, ctx, c, 0, 0u, 0.0f, 0x0u, 0x0u);
}
void inInNowLoadingEffect(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (!ckBegin(712, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    ref::inInNowLoadingEffect(rdram, &c, runtime);
    ckMid();
    const uint64_t r = a1_sx64(ty::inInNowLoadingEffect(rdram, runtime));
    ckEnd(712, ctx, c, 1, r, 0.0f, 0x8u, 0x0u);
}
void DirectTurn_Reset(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t p4)
{
    if (!ckBegin(713, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    ref::DirectTurn_Reset(rdram, &c, runtime);
    ckMid();
    ty::DirectTurn_Reset(rdram, runtime, p4);
    ckEnd(713, ctx, c, 0, 0u, 0.0f, 0x0u, 0x0u);
}
void is_doing_nico_sight_on(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t p4)
{
    if (!ckBegin(714, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    ref::is_doing_nico_sight_on(rdram, &c, runtime);
    ckMid();
    const uint64_t r = a1_sx64(ty::is_doing_nico_sight_on(rdram, runtime, p4));
    ckEnd(714, ctx, c, 1, r, 0.0f, 0x0u, 0x0u);
}
void getPlayerLastAttrStatus(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (!ckBegin(715, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    ref::getPlayerLastAttrStatus(rdram, &c, runtime);
    ckMid();
    const uint64_t r = (ty::getPlayerLastAttrStatus(rdram, runtime));
    ckEnd(715, ctx, c, 2, r, 0.0f, 0x8u, 0x0u);
}
void getPlayerLastParentObj(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (!ckBegin(716, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    ref::getPlayerLastParentObj(rdram, &c, runtime);
    ckMid();
    const uint64_t r = a1_sx64(ty::getPlayerLastParentObj(rdram, runtime));
    ckEnd(716, ctx, c, 1, r, 0.0f, 0x8u, 0x0u);
}
void nico_setPlayerDemoAdditionClothCollisionFloorTable1(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (!ckBegin(717, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    ref::nico_setPlayerDemoAdditionClothCollisionFloorTable1(rdram, &c, runtime);
    ckMid();
    const uint64_t r = a1_sx64(ty::nico_setPlayerDemoAdditionClothCollisionFloorTable1(rdram, runtime));
    ckEnd(717, ctx, c, 1, r, 0.0f, 0x1f8u, 0x0u);
}
void nico_setPlayerDemoAdditionClothCollisionRightArm(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t p4)
{
    if (!ckBegin(718, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    ref::nico_setPlayerDemoAdditionClothCollisionRightArm(rdram, &c, runtime);
    ckMid();
    const a1::RetVF r = ty::nico_setPlayerDemoAdditionClothCollisionRightArm(rdram, runtime, p4);
    ckEnd(718, ctx, c, 4, r.v0, r.f0, 0x7eau, 0x0u);
}
void nico_setPlayerDemoAdditionClothCollisionRightHandAndSwordDeep(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t p4)
{
    if (!ckBegin(719, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    ref::nico_setPlayerDemoAdditionClothCollisionRightHandAndSwordDeep(rdram, &c, runtime);
    ckMid();
    const a1::RetVF r = ty::nico_setPlayerDemoAdditionClothCollisionRightHandAndSwordDeep(rdram, runtime, p4);
    ckEnd(719, ctx, c, 4, r.v0, r.f0, 0x7eau, 0x0u);
}
void nico_setPlayerDemoAdditionClothCollisionRightHandAndSword(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t p4)
{
    if (!ckBegin(720, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    ref::nico_setPlayerDemoAdditionClothCollisionRightHandAndSword(rdram, &c, runtime);
    ckMid();
    const a1::RetVF r = ty::nico_setPlayerDemoAdditionClothCollisionRightHandAndSword(rdram, runtime, p4);
    ckEnd(720, ctx, c, 4, r.v0, r.f0, 0x7eau, 0x0u);
}
void nico_resetPlayerDemoAdditionClothCollision(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (!ckBegin(721, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    ref::nico_resetPlayerDemoAdditionClothCollision(rdram, &c, runtime);
    ckMid();
    const uint64_t r = a1_sx64(ty::nico_resetPlayerDemoAdditionClothCollision(rdram, runtime));
    ckEnd(721, ctx, c, 1, r, 0.0f, 0x38u, 0x0u);
}
void player_initSlipEffect(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t p5)
{
    if (!ckBegin(722, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    ref::player_initSlipEffect(rdram, &c, runtime);
    ckMid();
    const uint64_t r = a1_sx64(ty::player_initSlipEffect(rdram, runtime, p5));
    ckEnd(722, ctx, c, 1, r, 0.0f, 0x18u, 0x0u);
}
void bossCloudStart(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (!ckBegin(723, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    ref::bossCloudStart(rdram, &c, runtime);
    ckMid();
    const uint64_t r = a1_sx64(ty::bossCloudStart(rdram, runtime));
    ckEnd(723, ctx, c, 1, r, 0.0f, 0x8u, 0x0u);
}
void bossDemoPlayerBreakDownAccept(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (!ckBegin(724, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    ref::bossDemoPlayerBreakDownAccept(rdram, &c, runtime);
    ckMid();
    const uint64_t r = a1_sx64(ty::bossDemoPlayerBreakDownAccept(rdram, runtime));
    ckEnd(724, ctx, c, 1, r, 0.0f, 0x8u, 0x0u);
}
void bossDemoPlayerBreakDownEnd(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (!ckBegin(725, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    ref::bossDemoPlayerBreakDownEnd(rdram, &c, runtime);
    ckMid();
    const uint64_t r = a1_sx64(ty::bossDemoPlayerBreakDownEnd(rdram, runtime));
    ckEnd(725, ctx, c, 1, r, 0.0f, 0x8u, 0x0u);
}
void AI_InitAnimalOptimize(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t p4)
{
    if (!ckBegin(726, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    ref::AI_InitAnimalOptimize(rdram, &c, runtime);
    ckMid();
    const a1::RetVF r = ty::AI_InitAnimalOptimize(rdram, runtime, p4);
    ckEnd(726, ctx, c, 4, r.v0, r.f0, 0x2u, 0x0u);
}
void AI_InitAnimalWork(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t p4, uint32_t p5)
{
    if (!ckBegin(727, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    ref::AI_InitAnimalWork(rdram, &c, runtime);
    ckMid();
    const uint64_t r = a1_sx64(ty::AI_InitAnimalWork(rdram, runtime, p4, p5));
    ckEnd(727, ctx, c, 1, r, 0.0f, 0x0u, 0x0u);
}
void AI_SetAnimalWorkParam(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t p4, uint32_t p5)
{
    if (!ckBegin(728, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    ref::AI_SetAnimalWorkParam(rdram, &c, runtime);
    ckMid();
    ty::AI_SetAnimalWorkParam(rdram, runtime, p4, p5);
    ckEnd(728, ctx, c, 0, 0u, 0.0f, 0x0u, 0x0u);
}
void AI_InitGexCommonWork(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t p4)
{
    if (!ckBegin(729, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    ref::AI_InitGexCommonWork(rdram, &c, runtime);
    ckMid();
    ty::AI_InitGexCommonWork(rdram, runtime, p4);
    ckEnd(729, ctx, c, 0, 0u, 0.0f, 0x0u, 0x0u);
}
void AI_InitSurpriseArrow(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t p4)
{
    if (!ckBegin(730, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    ref::AI_InitSurpriseArrow(rdram, &c, runtime);
    ckMid();
    ty::AI_InitSurpriseArrow(rdram, runtime, p4);
    ckEnd(730, ctx, c, 0, 0u, 0.0f, 0x0u, 0x0u);
}
void AI_SetSurprisedArea(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t p4, float pf12)
{
    if (!ckBegin(731, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    ref::AI_SetSurprisedArea(rdram, &c, runtime);
    ckMid();
    ty::AI_SetSurprisedArea(rdram, runtime, p4, pf12);
    ckEnd(731, ctx, c, 0, 0u, 0.0f, 0x0u, 0x0u);
}
void AI_UpdateVelocity(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t p4, float pf12, float pf13)
{
    if (!ckBegin(732, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    ref::AI_UpdateVelocity(rdram, &c, runtime);
    ckMid();
    const a1::RetVF r = ty::AI_UpdateVelocity(rdram, runtime, p4, pf12, pf13);
    ckEnd(732, ctx, c, 4, r.v0, r.f0, 0x8u, 0x2u);
}
void AI_GetAccelB(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, float pf12, float pf13, float pf14, float pf15)
{
    if (!ckBegin(733, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    ref::AI_GetAccelB(rdram, &c, runtime);
    ckMid();
    const float r = ty::AI_GetAccelB(rdram, runtime, pf12, pf13, pf14, pf15);
    ckEnd(733, ctx, c, 3, 0u, r, 0x2u, 0xeu);
}
void AI_GetAccelA(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, float pf12, float pf13, float pf14, float pf15)
{
    if (!ckBegin(734, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    ref::AI_GetAccelA(rdram, &c, runtime);
    ckMid();
    const float r = ty::AI_GetAccelA(rdram, runtime, pf12, pf13, pf14, pf15);
    ckEnd(734, ctx, c, 3, 0u, r, 0x2u, 0x2002u);
}
void AI_GetAccelA_at0(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, float pf12, float pf13, float pf14)
{
    if (!ckBegin(735, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    ref::AI_GetAccelA_at0(rdram, &c, runtime);
    ckMid();
    const float r = ty::AI_GetAccelA_at0(rdram, runtime, pf12, pf13, pf14);
    ckEnd(735, ctx, c, 3, 0u, r, 0x2u, 0x2002u);
}
void AI_GetAccelB_at0(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, float pf12, float pf13, float pf14)
{
    if (!ckBegin(736, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    ref::AI_GetAccelB_at0(rdram, &c, runtime);
    ckMid();
    const float r = ty::AI_GetAccelB_at0(rdram, runtime, pf12, pf13, pf14);
    ckEnd(736, ctx, c, 3, 0u, r, 0x2u, 0x2000u);
}
void AI_GetAccelA3(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, float pf12, float pf13, float pf14, float pf15, float pf16, float pf17)
{
    if (!ckBegin(737, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    ref::AI_GetAccelA3(rdram, &c, runtime);
    ckMid();
    const float r = ty::AI_GetAccelA3(rdram, runtime, pf12, pf13, pf14, pf15, pf16, pf17);
    ckEnd(737, ctx, c, 3, 0u, r, 0x2u, 0x1401eu);
}
void AI_GetAccelB3(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, float pf12, float pf13, float pf14, float pf15, float pf16, float pf17)
{
    if (!ckBegin(738, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    ref::AI_GetAccelB3(rdram, &c, runtime);
    ckMid();
    const float r = ty::AI_GetAccelB3(rdram, runtime, pf12, pf13, pf14, pf15, pf16, pf17);
    ckEnd(738, ctx, c, 3, 0u, r, 0x2u, 0x1801eu);
}
void AI_GetAccelC3(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, float pf12, float pf13, float pf14, float pf15, float pf16, float pf17)
{
    if (!ckBegin(739, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    ref::AI_GetAccelC3(rdram, &c, runtime);
    ckMid();
    const float r = ty::AI_GetAccelC3(rdram, runtime, pf12, pf13, pf14, pf15, pf16, pf17);
    ckEnd(739, ctx, c, 3, 0u, r, 0x2u, 0x2a006u);
}
void AI_GetTimeFromAccelC3(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, float pf12, float pf13, float pf14, float pf15)
{
    if (!ckBegin(740, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    ref::AI_GetTimeFromAccelC3(rdram, &c, runtime);
    ckMid();
    const float r = ty::AI_GetTimeFromAccelC3(rdram, runtime, pf12, pf13, pf14, pf15);
    ckEnd(740, ctx, c, 3, 0u, r, 0x2u, 0xe006u);
}
void AI_GetVelocity3(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, float pf12, float pf13, float pf14, float pf15, float pf16)
{
    if (!ckBegin(741, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    ref::AI_GetVelocity3(rdram, &c, runtime);
    ckMid();
    const float r = ty::AI_GetVelocity3(rdram, runtime, pf12, pf13, pf14, pf15, pf16);
    ckEnd(741, ctx, c, 3, 0u, r, 0x2u, 0x7002u);
}
void AI_GetAreaDistScale(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, float pf12, float pf13)
{
    if (!ckBegin(742, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    ref::AI_GetAreaDistScale(rdram, &c, runtime);
    ckMid();
    const float r = ty::AI_GetAreaDistScale(rdram, runtime, pf12, pf13);
    ckEnd(742, ctx, c, 3, 0u, r, 0x0u, 0x0u);
}
void AI_GetAreaDistScale2(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, float pf12, float pf13)
{
    if (!ckBegin(743, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    ref::AI_GetAreaDistScale2(rdram, &c, runtime);
    ckMid();
    const float r = ty::AI_GetAreaDistScale2(rdram, runtime, pf12, pf13);
    ckEnd(743, ctx, c, 3, 0u, r, 0x0u, 0x0u);
}
void AI_GetNegaVector(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t p4, uint32_t p5)
{
    if (!ckBegin(744, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    ref::AI_GetNegaVector(rdram, &c, runtime);
    ckMid();
    const float r = ty::AI_GetNegaVector(rdram, runtime, p4, p5);
    ckEnd(744, ctx, c, 3, 0u, r, 0x0u, 0x2u);
}
void Kite_SetAnimalID_KitePathWork(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t p4, uint32_t p5)
{
    if (!ckBegin(745, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    ref::Kite_SetAnimalID_KitePathWork(rdram, &c, runtime);
    ckMid();
    ty::Kite_SetAnimalID_KitePathWork(rdram, runtime, p4, p5);
    ckEnd(745, ctx, c, 0, 0u, 0.0f, 0x0u, 0x0u);
}
void Kite_SetEventIDList(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t p4, uint32_t p5, uint64_t p6)
{
    if (!ckBegin(746, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    ref::Kite_SetEventIDList(rdram, &c, runtime);
    ckMid();
    const uint64_t r = a1_sx64(ty::Kite_SetEventIDList(rdram, runtime, p4, p5, p6));
    ckEnd(746, ctx, c, 1, r, 0.0f, 0x40u, 0x0u);
}
void Kite_InitEventCheck(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t p4)
{
    if (!ckBegin(747, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    ref::Kite_InitEventCheck(rdram, &c, runtime);
    ckMid();
    const uint64_t r = a1_sx64(ty::Kite_InitEventCheck(rdram, runtime, p4));
    ckEnd(747, ctx, c, 1, r, 0.0f, 0x0u, 0x0u);
}
void Kite_ResetEventFrameCheck(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t p4)
{
    if (!ckBegin(748, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    ref::Kite_ResetEventFrameCheck(rdram, &c, runtime);
    ckMid();
    ty::Kite_ResetEventFrameCheck(rdram, runtime, p4);
    ckEnd(748, ctx, c, 0, 0u, 0.0f, 0x0u, 0x0u);
}
void Kite_StartEventCheck(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t p4, uint32_t p5)
{
    if (!ckBegin(749, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    ref::Kite_StartEventCheck(rdram, &c, runtime);
    ckMid();
    const uint64_t r = a1_sx64(ty::Kite_StartEventCheck(rdram, runtime, p4, p5));
    ckEnd(749, ctx, c, 1, r, 0.0f, 0x0u, 0x0u);
}
void Kite_EndEventCheck(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t p4)
{
    if (!ckBegin(750, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    ref::Kite_EndEventCheck(rdram, &c, runtime);
    ckMid();
    const uint64_t r = a1_sx64(ty::Kite_EndEventCheck(rdram, runtime, p4));
    ckEnd(750, ctx, c, 1, r, 0.0f, 0x0u, 0x0u);
}
void Kite_SucceedKitePathWork(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t p4, uint32_t p5)
{
    if (!ckBegin(751, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    ref::Kite_SucceedKitePathWork(rdram, &c, runtime);
    ckMid();
    const uint64_t r = (ty::Kite_SucceedKitePathWork(rdram, runtime, p4, p5));
    ckEnd(751, ctx, c, 2, r, 0.0f, 0x8u, 0x0u);
}
void setSmallManagerSystemLock(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t p4)
{
    if (!ckBegin(752, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    ref::setSmallManagerSystemLock(rdram, &c, runtime);
    ckMid();
    const uint64_t r = a1_sx64(ty::setSmallManagerSystemLock(rdram, runtime, p4));
    ckEnd(752, ctx, c, 1, r, 0.0f, 0x0u, 0x0u);
}
void gamecore_sub_013F1190(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t p4)
{
    if (!ckBegin(753, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    ref::gamecore_sub_013F1190(rdram, &c, runtime);
    ckMid();
    const uint64_t r = a1_sx64(ty::gamecore_sub_013F1190(rdram, runtime, p4));
    ckEnd(753, ctx, c, 1, r, 0.0f, 0x0u, 0x0u);
}
void gamecore_sub_013F1938(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t p4, uint32_t p5)
{
    if (!ckBegin(754, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    ref::gamecore_sub_013F1938(rdram, &c, runtime);
    ckMid();
    const a1::RetVF r = ty::gamecore_sub_013F1938(rdram, runtime, p4, p5);
    ckEnd(754, ctx, c, 4, r.v0, r.f0, 0xau, 0x6u);
}
void getNowExecEventScriptObj(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (!ckBegin(755, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    ref::getNowExecEventScriptObj(rdram, &c, runtime);
    ckMid();
    const uint64_t r = a1_sx64(ty::getNowExecEventScriptObj(rdram, runtime));
    ckEnd(755, ctx, c, 1, r, 0.0f, 0x8u, 0x0u);
}
void setEventScriptEnable(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t p4)
{
    if (!ckBegin(756, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    ref::setEventScriptEnable(rdram, &c, runtime);
    ckMid();
    const uint64_t r = a1_sx64(ty::setEventScriptEnable(rdram, runtime, p4));
    ckEnd(756, ctx, c, 1, r, 0.0f, 0x0u, 0x0u);
}
void setPlayerStatusSwitch(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t p4)
{
    if (!ckBegin(757, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    ref::setPlayerStatusSwitch(rdram, &c, runtime);
    ckMid();
    const uint64_t r = a1_sx64(ty::setPlayerStatusSwitch(rdram, runtime, p4));
    ckEnd(757, ctx, c, 1, r, 0.0f, 0x0u, 0x0u);
}
void startAttackEffect(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, float pf12)
{
    if (!ckBegin(758, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    ref::startAttackEffect(rdram, &c, runtime);
    ckMid();
    const a1::RetVF r = ty::startAttackEffect(rdram, runtime, pf12);
    ckEnd(758, ctx, c, 4, r.v0, r.f0, 0xau, 0x2u);
}
void bosslifebar_display(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (!ckBegin(759, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    ref::bosslifebar_display(rdram, &c, runtime);
    ckMid();
    const a1::RetVF r = ty::bosslifebar_display(rdram, runtime);
    ckEnd(759, ctx, c, 4, r.v0, r.f0, 0x8u, 0x2u);
}
void bosslifebar_resethp(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, float pf12, float pf13)
{
    if (!ckBegin(760, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    ref::bosslifebar_resethp(rdram, &c, runtime);
    ckMid();
    const a1::RetVF r = ty::bosslifebar_resethp(rdram, runtime, pf12, pf13);
    ckEnd(760, ctx, c, 4, r.v0, r.f0, 0xau, 0x3000u);
}
void PlayerSprStatusEnd(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (!ckBegin(761, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    ref::PlayerSprStatusEnd(rdram, &c, runtime);
    ckMid();
    const uint64_t r = a1_sx64(ty::PlayerSprStatusEnd(rdram, runtime));
    ckEnd(761, ctx, c, 1, r, 0.0f, 0x0u, 0x0u);
}
void BossSprStatusEnd(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (!ckBegin(762, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    ref::BossSprStatusEnd(rdram, &c, runtime);
    ckMid();
    const uint64_t r = a1_sx64(ty::BossSprStatusEnd(rdram, runtime));
    ckEnd(762, ctx, c, 1, r, 0.0f, 0x0u, 0x0u);
}
void GetSwordFlareHitPlaneAttribute(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (!ckBegin(763, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    ref::GetSwordFlareHitPlaneAttribute(rdram, &c, runtime);
    ckMid();
    const uint64_t r = (ty::GetSwordFlareHitPlaneAttribute(rdram, runtime));
    ckEnd(763, ctx, c, 2, r, 0.0f, 0x8u, 0x0u);
}
void getSelectWeapon(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (!ckBegin(764, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    ref::getSelectWeapon(rdram, &c, runtime);
    ckMid();
    const uint64_t r = a1_sx64(ty::getSelectWeapon(rdram, runtime));
    ckEnd(764, ctx, c, 1, r, 0.0f, 0x8u, 0x0u);
}
void getSprLogWorkAddress(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (!ckBegin(765, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    ref::getSprLogWorkAddress(rdram, &c, runtime);
    ckMid();
    const uint64_t r = a1_sx64(ty::getSprLogWorkAddress(rdram, runtime));
    ckEnd(765, ctx, c, 1, r, 0.0f, 0x0u, 0x0u);
}
void getSprLogStatusAddress(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (!ckBegin(766, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    ref::getSprLogStatusAddress(rdram, &c, runtime);
    ckMid();
    const uint64_t r = a1_sx64(ty::getSprLogStatusAddress(rdram, runtime));
    ckEnd(766, ctx, c, 1, r, 0.0f, 0x0u, 0x0u);
}
void initPauseMenuMap(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (!ckBegin(767, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    ref::initPauseMenuMap(rdram, &c, runtime);
    ckMid();
    const uint64_t r = a1_sx64(ty::initPauseMenuMap(rdram, runtime));
    ckEnd(767, ctx, c, 1, r, 0.0f, 0x0u, 0x0u);
}
void setLoadMapOffset(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t p4, uint32_t p5, float pf12, float pf13)
{
    if (!ckBegin(768, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    ref::setLoadMapOffset(rdram, &c, runtime);
    ckMid();
    const uint64_t r = a1_sx64(ty::setLoadMapOffset(rdram, runtime, p4, p5, pf12, pf13));
    ckEnd(768, ctx, c, 1, r, 0.0f, 0xc8u, 0x0u);
}
void isInLoadWait(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (!ckBegin(769, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    ref::isInLoadWait(rdram, &c, runtime);
    ckMid();
    const uint64_t r = a1_sx64(ty::isInLoadWait(rdram, runtime));
    ckEnd(769, ctx, c, 1, r, 0.0f, 0x8u, 0x0u);
}
void LoadWaitCancel(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t p4)
{
    if (!ckBegin(770, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    ref::LoadWaitCancel(rdram, &c, runtime);
    ckMid();
    const uint64_t r = a1_sx64(ty::LoadWaitCancel(rdram, runtime, p4));
    ckEnd(770, ctx, c, 1, r, 0.0f, 0x8u, 0x0u);
}
void acted_camera_warp(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint64_t p4)
{
    if (!ckBegin(771, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    ref::acted_camera_warp(rdram, &c, runtime);
    ckMid();
    const uint64_t r = (ty::acted_camera_warp(rdram, runtime, p4));
    ckEnd(771, ctx, c, 2, r, 0.0f, 0x0u, 0x0u);
}
void gamecore_sub_0140FE00(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t p4, uint32_t p5, uint32_t p6)
{
    if (!ckBegin(772, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    ref::gamecore_sub_0140FE00(rdram, &c, runtime);
    ckMid();
    const float r = ty::gamecore_sub_0140FE00(rdram, runtime, p4, p5, p6);
    ckEnd(772, ctx, c, 3, 0u, r, 0x60u, 0x2u);
}
void gamecore_sub_0140FE20(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t p4, uint32_t p5, uint32_t p6, uint32_t p7)
{
    if (!ckBegin(773, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    ref::gamecore_sub_0140FE20(rdram, &c, runtime);
    ckMid();
    const float r = ty::gamecore_sub_0140FE20(rdram, runtime, p4, p5, p6, p7);
    ckEnd(773, ctx, c, 3, 0u, r, 0xb0u, 0x6u);
}
void gamecore_sub_0140FE50(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t p4, uint32_t p5, uint32_t p6)
{
    if (!ckBegin(774, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    ref::gamecore_sub_0140FE50(rdram, &c, runtime);
    ckMid();
    const float r = ty::gamecore_sub_0140FE50(rdram, runtime, p4, p5, p6);
    ckEnd(774, ctx, c, 3, 0u, r, 0x60u, 0x2u);
}
void gamecore_sub_0140FE70(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, float pf12)
{
    if (!ckBegin(775, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    ref::gamecore_sub_0140FE70(rdram, &c, runtime);
    ckMid();
    const a1::RetVF r = ty::gamecore_sub_0140FE70(rdram, runtime, pf12);
    ckEnd(775, ctx, c, 4, r.v0, r.f0, 0x2u, 0x0u);
}
void reset_time_interp_param_camera_dist_to_look_at(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t p4)
{
    if (!ckBegin(776, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    ref::reset_time_interp_param_camera_dist_to_look_at(rdram, &c, runtime);
    ckMid();
    const float r = ty::reset_time_interp_param_camera_dist_to_look_at(rdram, runtime, p4);
    ckEnd(776, ctx, c, 3, 0u, r, 0x2u, 0x0u);
}
void is_acting_rewind_camera_dist_to_look_at(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t p4)
{
    if (!ckBegin(777, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    ref::is_acting_rewind_camera_dist_to_look_at(rdram, &c, runtime);
    ckMid();
    const uint64_t r = a1_sx64(ty::is_acting_rewind_camera_dist_to_look_at(rdram, runtime, p4));
    ckEnd(777, ctx, c, 1, r, 0.0f, 0x0u, 0x0u);
}
void get_rewind_begin_dist_to_target_camera_dist_to_look_at(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t p4, float pf12)
{
    if (!ckBegin(778, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    ref::get_rewind_begin_dist_to_target_camera_dist_to_look_at(rdram, &c, runtime);
    ckMid();
    const a1::RetVF r = ty::get_rewind_begin_dist_to_target_camera_dist_to_look_at(rdram, runtime, p4, pf12);
    ckEnd(778, ctx, c, 4, r.v0, r.f0, 0x0u, 0x0u);
}
void end_rewind_camera_dist_to_look_at(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t p4)
{
    if (!ckBegin(779, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    ref::end_rewind_camera_dist_to_look_at(rdram, &c, runtime);
    ckMid();
    ty::end_rewind_camera_dist_to_look_at(rdram, runtime, p4);
    ckEnd(779, ctx, c, 0, 0u, 0.0f, 0x0u, 0x0u);
}
void is_leave_camera_dist_to_look_at(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t p4)
{
    if (!ckBegin(780, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    ref::is_leave_camera_dist_to_look_at(rdram, &c, runtime);
    ckMid();
    const uint64_t r = a1_sx64(ty::is_leave_camera_dist_to_look_at(rdram, runtime, p4));
    ckEnd(780, ctx, c, 1, r, 0.0f, 0x8u, 0x0u);
}
void request_end_camera_pause_rise(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t p4)
{
    if (!ckBegin(781, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    ref::request_end_camera_pause_rise(rdram, &c, runtime);
    ckMid();
    const uint64_t r = a1_sx64(ty::request_end_camera_pause_rise(rdram, runtime, p4));
    ckEnd(781, ctx, c, 1, r, 0.0f, 0x0u, 0x0u);
}
void is_end_camera_pause_rise(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t p4)
{
    if (!ckBegin(782, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    ref::is_end_camera_pause_rise(rdram, &c, runtime);
    ckMid();
    const uint64_t r = a1_sx64(ty::is_end_camera_pause_rise(rdram, runtime, p4));
    ckEnd(782, ctx, c, 1, r, 0.0f, 0x0u, 0x0u);
}
void get_begin_interp_finish_time_camera_pause_rise(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (!ckBegin(783, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    ref::get_begin_interp_finish_time_camera_pause_rise(rdram, &c, runtime);
    ckMid();
    const a1::RetVF r = ty::get_begin_interp_finish_time_camera_pause_rise(rdram, runtime);
    ckEnd(783, ctx, c, 4, r.v0, r.f0, 0x0u, 0x0u);
}
void get_place_status_camera_place_status(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t p4)
{
    if (!ckBegin(784, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    ref::get_place_status_camera_place_status(rdram, &c, runtime);
    ckMid();
    const uint64_t r = a1_sx64(ty::get_place_status_camera_place_status(rdram, runtime, p4));
    ckEnd(784, ctx, c, 1, r, 0.0f, 0x0u, 0x0u);
}
void get_manage_obj_camera_place_status(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t p4)
{
    if (!ckBegin(785, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    ref::get_manage_obj_camera_place_status(rdram, &c, runtime);
    ckMid();
    const uint64_t r = a1_sx64(ty::get_manage_obj_camera_place_status(rdram, runtime, p4));
    ckEnd(785, ctx, c, 1, r, 0.0f, 0x0u, 0x0u);
}
void get_hit_camera_region_camera_place_status(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t p4)
{
    if (!ckBegin(786, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    ref::get_hit_camera_region_camera_place_status(rdram, &c, runtime);
    ckMid();
    const uint64_t r = a1_sx64(ty::get_hit_camera_region_camera_place_status(rdram, runtime, p4));
    ckEnd(786, ctx, c, 1, r, 0.0f, 0x0u, 0x0u);
}
void get_on_boss_camera_col_attr_camera_place_status(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t p4, uint32_t p5)
{
    if (!ckBegin(787, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    ref::get_on_boss_camera_col_attr_camera_place_status(rdram, &c, runtime);
    ckMid();
    const uint64_t r = (ty::get_on_boss_camera_col_attr_camera_place_status(rdram, runtime, p4, p5));
    ckEnd(787, ctx, c, 2, r, 0.0f, 0x20u, 0x0u);
}
void is_approach_rise_ok_camera_place_status(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint64_t p4)
{
    if (!ckBegin(788, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    ref::is_approach_rise_ok_camera_place_status(rdram, &c, runtime);
    ckMid();
    const uint64_t r = (ty::is_approach_rise_ok_camera_place_status(rdram, runtime, p4));
    ckEnd(788, ctx, c, 2, r, 0.0f, 0x18u, 0x0u);
}
void is_cliff_rise_delay_camera_place_status(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint64_t p4)
{
    if (!ckBegin(789, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    ref::is_cliff_rise_delay_camera_place_status(rdram, &c, runtime);
    ckMid();
    const uint64_t r = (ty::is_cliff_rise_delay_camera_place_status(rdram, runtime, p4));
    ckEnd(789, ctx, c, 2, r, 0.0f, 0x18u, 0x0u);
}
void act_horse_look_at_ofst_decrease_camera_place_status(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint64_t p4)
{
    if (!ckBegin(790, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    ref::act_horse_look_at_ofst_decrease_camera_place_status(rdram, &c, runtime);
    ckMid();
    const uint64_t r = (ty::act_horse_look_at_ofst_decrease_camera_place_status(rdram, runtime, p4));
    ckEnd(790, ctx, c, 2, r, 0.0f, 0x18u, 0x0u);
}
void act_look_at_ofst_cut_camera_place_status(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint64_t p4)
{
    if (!ckBegin(791, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    ref::act_look_at_ofst_cut_camera_place_status(rdram, &c, runtime);
    ckMid();
    const uint64_t r = (ty::act_look_at_ofst_cut_camera_place_status(rdram, runtime, p4));
    ckEnd(791, ctx, c, 2, r, 0.0f, 0x18u, 0x0u);
}
void gamecore_sub_01413708(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t p4)
{
    if (!ckBegin(792, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    ref::gamecore_sub_01413708(rdram, &c, runtime);
    ckMid();
    const uint64_t r = a1_sx64(ty::gamecore_sub_01413708(rdram, runtime, p4));
    ckEnd(792, ctx, c, 1, r, 0.0f, 0x0u, 0x0u);
}
void CAM_get_interp_slope_pitch_ofst(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t p4)
{
    if (!ckBegin(793, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    ref::CAM_get_interp_slope_pitch_ofst(rdram, &c, runtime);
    ckMid();
    const float r = ty::CAM_get_interp_slope_pitch_ofst(rdram, runtime, p4);
    ckEnd(793, ctx, c, 3, 0u, r, 0x0u, 0x0u);
}
void gamecore_sub_014144D0(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, float pf12, float pf13, float pf14)
{
    if (!ckBegin(794, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    ref::gamecore_sub_014144D0(rdram, &c, runtime);
    ckMid();
    const float r = ty::gamecore_sub_014144D0(rdram, runtime, pf12, pf13, pf14);
    ckEnd(794, ctx, c, 3, 0u, r, 0x0u, 0x1002u);
}
void get_attr_camera_target_lost_angle(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t p4)
{
    if (!ckBegin(795, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    ref::get_attr_camera_target_lost_angle(rdram, &c, runtime);
    ckMid();
    const uint64_t r = a1_sx64(ty::get_attr_camera_target_lost_angle(rdram, runtime, p4));
    ckEnd(795, ctx, c, 1, r, 0.0f, 0x0u, 0x0u);
}
void set_parameter_camera_target_visible_angle_manager(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t p4, uint32_t p5, float pf12, float pf13)
{
    if (!ckBegin(796, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    ref::set_parameter_camera_target_visible_angle_manager(rdram, &c, runtime);
    ckMid();
    const a1::RetVF r = ty::set_parameter_camera_target_visible_angle_manager(rdram, runtime, p4, p5, pf12, pf13);
    ckEnd(796, ctx, c, 4, r.v0, r.f0, 0x4au, 0x2u);
}
void end_get_yaw_pitch_camera_target_visible_angle_manager(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t p4)
{
    if (!ckBegin(797, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    ref::end_get_yaw_pitch_camera_target_visible_angle_manager(rdram, &c, runtime);
    ckMid();
    ty::end_get_yaw_pitch_camera_target_visible_angle_manager(rdram, runtime, p4);
    ckEnd(797, ctx, c, 0, 0u, 0.0f, 0x0u, 0x0u);
}
void gamecore_sub_01415CA0(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t p4, uint32_t p5)
{
    if (!ckBegin(798, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    ref::gamecore_sub_01415CA0(rdram, &c, runtime);
    ckMid();
    const a1::RetVF r = ty::gamecore_sub_01415CA0(rdram, runtime, p4, p5);
    ckEnd(798, ctx, c, 4, r.v0, r.f0, 0x48u, 0x2u);
}
void CAM_copy_camera_base_data(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t p4, uint32_t p5)
{
    if (!ckBegin(799, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    ref::CAM_copy_camera_base_data(rdram, &c, runtime);
    ckMid();
    const uint64_t r = (ty::CAM_copy_camera_base_data(rdram, runtime, p4, p5));
    ckEnd(799, ctx, c, 2, r, 0.0f, 0x8u, 0x0u);
}
void CAM_interp_reset_aim_rot_common(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t p4)
{
    if (!ckBegin(800, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    ref::CAM_interp_reset_aim_rot_common(rdram, &c, runtime);
    ckMid();
    const uint64_t r = a1_sx64(ty::CAM_interp_reset_aim_rot_common(rdram, runtime, p4));
    ckEnd(800, ctx, c, 1, r, 0.0f, 0x0u, 0x0u);
}
void CAM_motion_kind_is_look_around_motion_target(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t p4)
{
    if (!ckBegin(801, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    ref::CAM_motion_kind_is_look_around_motion_target(rdram, &c, runtime);
    ckMid();
    const uint64_t r = a1_sx64(ty::CAM_motion_kind_is_look_around_motion_target(rdram, runtime, p4));
    ckEnd(801, ctx, c, 1, r, 0.0f, 0x0u, 0x0u);
}
void CAM_motion_kind_is_accept_auto_rot_motion_target(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t p4)
{
    if (!ckBegin(802, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    ref::CAM_motion_kind_is_accept_auto_rot_motion_target(rdram, &c, runtime);
    ckMid();
    const uint64_t r = a1_sx64(ty::CAM_motion_kind_is_accept_auto_rot_motion_target(rdram, runtime, p4));
    ckEnd(802, ctx, c, 1, r, 0.0f, 0x0u, 0x0u);
}
void CAM_get_target_obj(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t p4)
{
    if (!ckBegin(803, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    ref::CAM_get_target_obj(rdram, &c, runtime);
    ckMid();
    const uint64_t r = a1_sx64(ty::CAM_get_target_obj(rdram, runtime, p4));
    ckEnd(803, ctx, c, 1, r, 0.0f, 0x0u, 0x0u);
}
void CAM_get_target_riding_obj(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t p4)
{
    if (!ckBegin(804, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    ref::CAM_get_target_riding_obj(rdram, &c, runtime);
    ckMid();
    const uint64_t r = a1_sx64(ty::CAM_get_target_riding_obj(rdram, runtime, p4));
    ckEnd(804, ctx, c, 1, r, 0.0f, 0x0u, 0x0u);
}
void CAM_get_ground_contact_obj(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t p4)
{
    if (!ckBegin(805, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    ref::CAM_get_ground_contact_obj(rdram, &c, runtime);
    ckMid();
    const uint64_t r = (ty::CAM_get_ground_contact_obj(rdram, runtime, p4));
    ckEnd(805, ctx, c, 2, r, 0.0f, 0x8u, 0x0u);
}
void CAM_is_visible_now_target_visibility(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t p4)
{
    if (!ckBegin(806, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    ref::CAM_is_visible_now_target_visibility(rdram, &c, runtime);
    ckMid();
    const uint64_t r = a1_sx64(ty::CAM_is_visible_now_target_visibility(rdram, runtime, p4));
    ckEnd(806, ctx, c, 1, r, 0.0f, 0x0u, 0x0u);
}
void CAM_is_return_cam_posi_target_visibility(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t p4)
{
    if (!ckBegin(807, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    ref::CAM_is_return_cam_posi_target_visibility(rdram, &c, runtime);
    ckMid();
    const uint64_t r = a1_sx64(ty::CAM_is_return_cam_posi_target_visibility(rdram, runtime, p4));
    ckEnd(807, ctx, c, 1, r, 0.0f, 0x0u, 0x0u);
}
void CAM_reset_camera_warp_cntrl(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t p4)
{
    if (!ckBegin(808, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    ref::CAM_reset_camera_warp_cntrl(rdram, &c, runtime);
    ckMid();
    const a1::RetVF r = ty::CAM_reset_camera_warp_cntrl(rdram, runtime, p4);
    ckEnd(808, ctx, c, 4, r.v0, r.f0, 0x2u, 0x2u);
}
void CAM_set_invisible_warp_apply_range_camera_warp_cntrl(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t p4, float pf12, float pf13)
{
    if (!ckBegin(809, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    ref::CAM_set_invisible_warp_apply_range_camera_warp_cntrl(rdram, &c, runtime);
    ckMid();
    ty::CAM_set_invisible_warp_apply_range_camera_warp_cntrl(rdram, runtime, p4, pf12, pf13);
    ckEnd(809, ctx, c, 0, 0u, 0.0f, 0x0u, 0x0u);
}
void CAM_act_warp_order_camera_warp_cntrl(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t p4)
{
    if (!ckBegin(810, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    ref::CAM_act_warp_order_camera_warp_cntrl(rdram, &c, runtime);
    ckMid();
    const uint64_t r = a1_sx64(ty::CAM_act_warp_order_camera_warp_cntrl(rdram, runtime, p4));
    ckEnd(810, ctx, c, 1, r, 0.0f, 0x0u, 0x0u);
}
void CAM_is_acting_view_cut_avoid_cntrl(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t p4)
{
    if (!ckBegin(811, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    ref::CAM_is_acting_view_cut_avoid_cntrl(rdram, &c, runtime);
    ckMid();
    const uint64_t r = a1_sx64(ty::CAM_is_acting_view_cut_avoid_cntrl(rdram, runtime, p4));
    ckEnd(811, ctx, c, 1, r, 0.0f, 0x0u, 0x0u);
}
void CAM_set_param_view_cut_avoid_cntrl(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t p4, float pf12, float pf13)
{
    if (!ckBegin(812, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    ref::CAM_set_param_view_cut_avoid_cntrl(rdram, &c, runtime);
    ckMid();
    ty::CAM_set_param_view_cut_avoid_cntrl(rdram, runtime, p4, pf12, pf13);
    ckEnd(812, ctx, c, 0, 0u, 0.0f, 0x0u, 0x0u);
}
void CAM_reset_obstacle_avoid_cntrl(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t p4)
{
    if (!ckBegin(813, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    ref::CAM_reset_obstacle_avoid_cntrl(rdram, &c, runtime);
    ckMid();
    ty::CAM_reset_obstacle_avoid_cntrl(rdram, runtime, p4);
    ckEnd(813, ctx, c, 0, 0u, 0.0f, 0x0u, 0x0u);
}
void gamecore_sub_0141B428(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, float pf12, float pf13)
{
    if (!ckBegin(814, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    ref::gamecore_sub_0141B428(rdram, &c, runtime);
    ckMid();
    const float r = ty::gamecore_sub_0141B428(rdram, runtime, pf12, pf13);
    ckEnd(814, ctx, c, 3, 0u, r, 0x2u, 0x0u);
}
void gamecore_sub_0141B480(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t p4)
{
    if (!ckBegin(815, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    ref::gamecore_sub_0141B480(rdram, &c, runtime);
    ckMid();
    ty::gamecore_sub_0141B480(rdram, runtime, p4);
    ckEnd(815, ctx, c, 0, 0u, 0.0f, 0x0u, 0x0u);
}
void gamecore_sub_0141B978(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t p4)
{
    if (!ckBegin(816, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    ref::gamecore_sub_0141B978(rdram, &c, runtime);
    ckMid();
    const uint64_t r = a1_sx64(ty::gamecore_sub_0141B978(rdram, runtime, p4));
    ckEnd(816, ctx, c, 1, r, 0.0f, 0x0u, 0x0u);
}
void gamecore_sub_0141B988(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t p4)
{
    if (!ckBegin(817, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    ref::gamecore_sub_0141B988(rdram, &c, runtime);
    ckMid();
    const uint64_t r = a1_sx64(ty::gamecore_sub_0141B988(rdram, runtime, p4));
    ckEnd(817, ctx, c, 1, r, 0.0f, 0x0u, 0x0u);
}
void gamecore_sub_0141BB20(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t p4, uint32_t p5, float pf12)
{
    if (!ckBegin(818, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    ref::gamecore_sub_0141BB20(rdram, &c, runtime);
    ckMid();
    const a1::RetVF r = ty::gamecore_sub_0141BB20(rdram, runtime, p4, p5, pf12);
    ckEnd(818, ctx, c, 4, r.v0, r.f0, 0xau, 0x6u);
}
void gamecore_sub_0141BCF8(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t p5)
{
    if (!ckBegin(819, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    ref::gamecore_sub_0141BCF8(rdram, &c, runtime);
    ckMid();
    const uint64_t r = a1_sx64(ty::gamecore_sub_0141BCF8(rdram, runtime, p5));
    ckEnd(819, ctx, c, 1, r, 0.0f, 0x0u, 0x0u);
}
void gamecore_sub_0141C0A8(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, float pf12, float pf13, float pf14)
{
    if (!ckBegin(820, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    ref::gamecore_sub_0141C0A8(rdram, &c, runtime);
    ckMid();
    const float r = ty::gamecore_sub_0141C0A8(rdram, runtime, pf12, pf13, pf14);
    ckEnd(820, ctx, c, 3, 0u, r, 0x0u, 0x0u);
}
void get_view_angle_camera_view_angle_cntrl(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t p4)
{
    if (!ckBegin(821, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    ref::get_view_angle_camera_view_angle_cntrl(rdram, &c, runtime);
    ckMid();
    const float r = ty::get_view_angle_camera_view_angle_cntrl(rdram, runtime, p4);
    ckEnd(821, ctx, c, 3, 0u, r, 0x0u, 0x0u);
}
void get_camera_obj(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t p4)
{
    if (!ckBegin(822, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    ref::get_camera_obj(rdram, &c, runtime);
    ckMid();
    const uint64_t r = a1_sx64(ty::get_camera_obj(rdram, runtime, p4));
    ckEnd(822, ctx, c, 1, r, 0.0f, 0x0u, 0x0u);
}
void get_player_collision_group_cameraman(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t p4)
{
    if (!ckBegin(823, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    ref::get_player_collision_group_cameraman(rdram, &c, runtime);
    ckMid();
    const uint64_t r = a1_sx64(ty::get_player_collision_group_cameraman(rdram, runtime, p4));
    ckEnd(823, ctx, c, 1, r, 0.0f, 0x0u, 0x0u);
}
void is_camera_target_change(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t p4)
{
    if (!ckBegin(824, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    ref::is_camera_target_change(rdram, &c, runtime);
    ckMid();
    const uint64_t r = a1_sx64(ty::is_camera_target_change(rdram, runtime, p4));
    ckEnd(824, ctx, c, 1, r, 0.0f, 0x8u, 0x0u);
}
void is_camera_target_debug_move(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (!ckBegin(825, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    ref::is_camera_target_debug_move(rdram, &c, runtime);
    ckMid();
    const uint64_t r = (ty::is_camera_target_debug_move(rdram, runtime));
    ckEnd(825, ctx, c, 2, r, 0.0f, 0x0u, 0x0u);
}
void get_main_cameraman(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (!ckBegin(826, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    ref::get_main_cameraman(rdram, &c, runtime);
    ckMid();
    const uint64_t r = a1_sx64(ty::get_main_cameraman(rdram, runtime));
    ckEnd(826, ctx, c, 1, r, 0.0f, 0x8u, 0x0u);
}
void reset_game_camera(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t p4)
{
    if (!ckBegin(827, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    ref::reset_game_camera(rdram, &c, runtime);
    ckMid();
    const uint64_t r = a1_sx64(ty::reset_game_camera(rdram, runtime, p4));
    ckEnd(827, ctx, c, 1, r, 0.0f, 0x0u, 0x0u);
}
void get_game_camera_cur_mode(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t p4)
{
    if (!ckBegin(828, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    ref::get_game_camera_cur_mode(rdram, &c, runtime);
    ckMid();
    const uint64_t r = a1_sx64(ty::get_game_camera_cur_mode(rdram, runtime, p4));
    ckEnd(828, ctx, c, 1, r, 0.0f, 0x0u, 0x0u);
}
void is_reset_or_lws_movie_game_camera(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t p4)
{
    if (!ckBegin(829, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    ref::is_reset_or_lws_movie_game_camera(rdram, &c, runtime);
    ckMid();
    const uint64_t r = (ty::is_reset_or_lws_movie_game_camera(rdram, runtime, p4));
    ckEnd(829, ctx, c, 2, r, 0.0f, 0x18u, 0x0u);
}
void reset_game_camera_standby_flag(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t p4)
{
    if (!ckBegin(830, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    ref::reset_game_camera_standby_flag(rdram, &c, runtime);
    ckMid();
    ty::reset_game_camera_standby_flag(rdram, runtime, p4);
    ckEnd(830, ctx, c, 0, 0u, 0.0f, 0x0u, 0x0u);
}
void set_object_interest_mode_target_game_camera(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t p4, uint32_t p5)
{
    if (!ckBegin(831, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    ref::set_object_interest_mode_target_game_camera(rdram, &c, runtime);
    ckMid();
    ty::set_object_interest_mode_target_game_camera(rdram, runtime, p4, p5);
    ckEnd(831, ctx, c, 0, 0u, 0.0f, 0x0u, 0x0u);
}
void get_object_interest_mode_target_game_camera(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t p4)
{
    if (!ckBegin(832, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    ref::get_object_interest_mode_target_game_camera(rdram, &c, runtime);
    ckMid();
    const uint64_t r = a1_sx64(ty::get_object_interest_mode_target_game_camera(rdram, runtime, p4));
    ckEnd(832, ctx, c, 1, r, 0.0f, 0x0u, 0x0u);
}
void begin_object_interest_game_camera(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t p4)
{
    if (!ckBegin(833, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    ref::begin_object_interest_game_camera(rdram, &c, runtime);
    ckMid();
    const uint64_t r = a1_sx64(ty::begin_object_interest_game_camera(rdram, runtime, p4));
    ckEnd(833, ctx, c, 1, r, 0.0f, 0x0u, 0x0u);
}
void begin_stream_lws_movie_playing_game_camera(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t p4)
{
    if (!ckBegin(834, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    ref::begin_stream_lws_movie_playing_game_camera(rdram, &c, runtime);
    ckMid();
    const uint64_t r = a1_sx64(ty::begin_stream_lws_movie_playing_game_camera(rdram, runtime, p4));
    ckEnd(834, ctx, c, 1, r, 0.0f, 0x0u, 0x0u);
}
void end_stream_lws_movie_playing_game_camera(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t p4)
{
    if (!ckBegin(835, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    ref::end_stream_lws_movie_playing_game_camera(rdram, &c, runtime);
    ckMid();
    ty::end_stream_lws_movie_playing_game_camera(rdram, runtime, p4);
    ckEnd(835, ctx, c, 0, 0u, 0.0f, 0x0u, 0x0u);
}
void is_view_angle_exist_lws_anime_mode_base(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t p4)
{
    if (!ckBegin(836, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    ref::is_view_angle_exist_lws_anime_mode_base(rdram, &c, runtime);
    ckMid();
    const a1::RetVF r = ty::is_view_angle_exist_lws_anime_mode_base(rdram, runtime, p4);
    ckEnd(836, ctx, c, 4, r.v0, r.f0, 0x0u, 0x2u);
}
void get_view_angle_lws_anime_mode_base(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t p4)
{
    if (!ckBegin(837, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    ref::get_view_angle_lws_anime_mode_base(rdram, &c, runtime);
    ckMid();
    const a1::RetVF r = ty::get_view_angle_lws_anime_mode_base(rdram, runtime, p4);
    ckEnd(837, ctx, c, 4, r.v0, r.f0, 0xau, 0x2u);
}
void getBossInterestKey(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (!ckBegin(838, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    ref::getBossInterestKey(rdram, &c, runtime);
    ckMid();
    const uint64_t r = a1_sx64(ty::getBossInterestKey(rdram, runtime));
    ckEnd(838, ctx, c, 1, r, 0.0f, 0x8u, 0x0u);
}
void getInterestKey(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (!ckBegin(839, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    ref::getInterestKey(rdram, &c, runtime);
    ckMid();
    const uint64_t r = a1_sx64(ty::getInterestKey(rdram, runtime));
    ckEnd(839, ctx, c, 1, r, 0.0f, 0x8u, 0x0u);
}
void getZoomKey(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (!ckBegin(840, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    ref::getZoomKey(rdram, &c, runtime);
    ckMid();
    const uint64_t r = a1_sx64(ty::getZoomKey(rdram, runtime));
    ckEnd(840, ctx, c, 1, r, 0.0f, 0x8u, 0x0u);
}
void setBossInterestKey(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t p4)
{
    if (!ckBegin(841, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    ref::setBossInterestKey(rdram, &c, runtime);
    ckMid();
    const uint64_t r = a1_sx64(ty::setBossInterestKey(rdram, runtime, p4));
    ckEnd(841, ctx, c, 1, r, 0.0f, 0x0u, 0x0u);
}
void setInterestKey(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t p4)
{
    if (!ckBegin(842, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    ref::setInterestKey(rdram, &c, runtime);
    ckMid();
    const uint64_t r = a1_sx64(ty::setInterestKey(rdram, runtime, p4));
    ckEnd(842, ctx, c, 1, r, 0.0f, 0x0u, 0x0u);
}
void setZoomKey(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t p4)
{
    if (!ckBegin(843, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    ref::setZoomKey(rdram, &c, runtime);
    ckMid();
    const uint64_t r = a1_sx64(ty::setZoomKey(rdram, runtime, p4));
    ckEnd(843, ctx, c, 1, r, 0.0f, 0x0u, 0x0u);
}
void gamecore_sub_01425700(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t p4, uint64_t p5)
{
    if (!ckBegin(844, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    ref::gamecore_sub_01425700(rdram, &c, runtime);
    ckMid();
    const uint64_t r = (ty::gamecore_sub_01425700(rdram, runtime, p4, p5));
    ckEnd(844, ctx, c, 2, r, 0.0f, 0x18u, 0x0u);
}
void gamecore_sub_01425788(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t p4)
{
    if (!ckBegin(845, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    ref::gamecore_sub_01425788(rdram, &c, runtime);
    ckMid();
    const a1::RetVF r = ty::gamecore_sub_01425788(rdram, runtime, p4);
    ckEnd(845, ctx, c, 4, r.v0, r.f0, 0x0u, 0x2u);
}
void gamecore_sub_014257A8(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t p4)
{
    if (!ckBegin(846, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    ref::gamecore_sub_014257A8(rdram, &c, runtime);
    ckMid();
    ty::gamecore_sub_014257A8(rdram, runtime, p4);
    ckEnd(846, ctx, c, 0, 0u, 0.0f, 0x0u, 0x0u);
}
void gamecore_sub_01425AC0(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t p4, uint32_t p5)
{
    if (!ckBegin(847, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    ref::gamecore_sub_01425AC0(rdram, &c, runtime);
    ckMid();
    const uint64_t r = a1_sx64(ty::gamecore_sub_01425AC0(rdram, runtime, p4, p5));
    ckEnd(847, ctx, c, 1, r, 0.0f, 0x3e8u, 0x0u);
}
void gamecore_sub_014262A0(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t p5)
{
    if (!ckBegin(848, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    ref::gamecore_sub_014262A0(rdram, &c, runtime);
    ckMid();
    ty::gamecore_sub_014262A0(rdram, runtime, p5);
    ckEnd(848, ctx, c, 0, 0u, 0.0f, 0x0u, 0x0u);
}
void gamecore_sub_01426B18(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t p4)
{
    if (!ckBegin(849, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    ref::gamecore_sub_01426B18(rdram, &c, runtime);
    ckMid();
    const float r = ty::gamecore_sub_01426B18(rdram, runtime, p4);
    ckEnd(849, ctx, c, 3, 0u, r, 0x0u, 0x0u);
}
void gamecore_sub_01426B20(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t p4)
{
    if (!ckBegin(850, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    ref::gamecore_sub_01426B20(rdram, &c, runtime);
    ckMid();
    const float r = ty::gamecore_sub_01426B20(rdram, runtime, p4);
    ckEnd(850, ctx, c, 3, 0u, r, 0x0u, 0x0u);
}
void gamecore_sub_01427230(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint64_t p4)
{
    if (!ckBegin(851, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    ref::gamecore_sub_01427230(rdram, &c, runtime);
    ckMid();
    const uint64_t r = (ty::gamecore_sub_01427230(rdram, runtime, p4));
    ckEnd(851, ctx, c, 2, r, 0.0f, 0x8u, 0x0u);
}
void gamecore_sub_01428CD0(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t p4, uint32_t p5, uint32_t p6, float pf12, float pf13)
{
    if (!ckBegin(852, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    ref::gamecore_sub_01428CD0(rdram, &c, runtime);
    ckMid();
    const a1::RetVF r = ty::gamecore_sub_01428CD0(rdram, runtime, p4, p5, p6, pf12, pf13);
    ckEnd(852, ctx, c, 4, r.v0, r.f0, 0x8u, 0xeu);
}
void gamecore_sub_014296F8(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t p4, uint32_t p5, uint32_t p6, float pf12, float pf13)
{
    if (!ckBegin(853, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    ref::gamecore_sub_014296F8(rdram, &c, runtime);
    ckMid();
    const a1::RetVF r = ty::gamecore_sub_014296F8(rdram, runtime, p4, p5, p6, pf12, pf13);
    ckEnd(853, ctx, c, 4, r.v0, r.f0, 0xau, 0xeu);
}
void gamecore_sub_01429BC0(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t p4, uint32_t p5, uint32_t p6, float pf12, float pf13)
{
    if (!ckBegin(854, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    ref::gamecore_sub_01429BC0(rdram, &c, runtime);
    ckMid();
    const float r = ty::gamecore_sub_01429BC0(rdram, runtime, p4, p5, p6, pf12, pf13);
    ckEnd(854, ctx, c, 3, 0u, r, 0x2u, 0x7eu);
}
void gamecore_sub_0142B968(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t p4, uint32_t p5, uint32_t p6, float pf12, float pf13)
{
    if (!ckBegin(855, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    ref::gamecore_sub_0142B968(rdram, &c, runtime);
    ckMid();
    const float r = ty::gamecore_sub_0142B968(rdram, runtime, p4, p5, p6, pf12, pf13);
    ckEnd(855, ctx, c, 3, 0u, r, 0x2u, 0xeu);
}
void is_base_mode_manager_update_game_camera_param_manager(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t p4)
{
    if (!ckBegin(856, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    ref::is_base_mode_manager_update_game_camera_param_manager(rdram, &c, runtime);
    ckMid();
    const uint64_t r = a1_sx64(ty::is_base_mode_manager_update_game_camera_param_manager(rdram, runtime, p4));
    ckEnd(856, ctx, c, 1, r, 0.0f, 0x0u, 0x0u);
}
void get_interp_time_game_camera_param_manager(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t p4)
{
    if (!ckBegin(857, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    ref::get_interp_time_game_camera_param_manager(rdram, &c, runtime);
    ckMid();
    const float r = ty::get_interp_time_game_camera_param_manager(rdram, runtime, p4);
    ckEnd(857, ctx, c, 3, 0u, r, 0x0u, 0x2u);
}
void act_camera_cutout_game_camera_param_manager(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t p4, uint32_t p5)
{
    if (!ckBegin(858, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    ref::act_camera_cutout_game_camera_param_manager(rdram, &c, runtime);
    ckMid();
    const uint64_t r = a1_sx64(ty::act_camera_cutout_game_camera_param_manager(rdram, runtime, p4, p5));
    ckEnd(858, ctx, c, 1, r, 0.0f, 0x18u, 0x0u);
}
void drop_camera_cutout_flag_game_camera_param_manager(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t p4)
{
    if (!ckBegin(859, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    ref::drop_camera_cutout_flag_game_camera_param_manager(rdram, &c, runtime);
    ckMid();
    ty::drop_camera_cutout_flag_game_camera_param_manager(rdram, runtime, p4);
    ckEnd(859, ctx, c, 0, 0u, 0.0f, 0x0u, 0x0u);
}
void constructor_game_camera_param_update_data_manager(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t p4)
{
    if (!ckBegin(860, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    ref::constructor_game_camera_param_update_data_manager(rdram, &c, runtime);
    ckMid();
    ty::constructor_game_camera_param_update_data_manager(rdram, runtime, p4);
    ckEnd(860, ctx, c, 0, 0u, 0.0f, 0x0u, 0x0u);
}
void is_already_set_game_camera_param_update_data_manager(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t p4, uint32_t p5)
{
    if (!ckBegin(861, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    ref::is_already_set_game_camera_param_update_data_manager(rdram, &c, runtime);
    ckMid();
    const uint64_t r = a1_sx64(ty::is_already_set_game_camera_param_update_data_manager(rdram, runtime, p4, p5));
    ckEnd(861, ctx, c, 1, r, 0.0f, 0x28u, 0x0u);
}
void constructor_game_camera_mode_local_parameter(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t p4)
{
    if (!ckBegin(862, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    ref::constructor_game_camera_mode_local_parameter(rdram, &c, runtime);
    ckMid();
    const float r = ty::constructor_game_camera_mode_local_parameter(rdram, runtime, p4);
    ckEnd(862, ctx, c, 3, 0u, r, 0x2u, 0x1eu);
}
void gamecore_sub_0142C440(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t p4, uint32_t p5, uint32_t p6, float pf12, float pf13)
{
    if (!ckBegin(863, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    ref::gamecore_sub_0142C440(rdram, &c, runtime);
    ckMid();
    const float r = ty::gamecore_sub_0142C440(rdram, runtime, p4, p5, p6, pf12, pf13);
    ckEnd(863, ctx, c, 3, 0u, r, 0x2u, 0x3eu);
}
void gamecore_sub_0142C530(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t p4, uint64_t p5)
{
    if (!ckBegin(864, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    ref::gamecore_sub_0142C530(rdram, &c, runtime);
    ckMid();
    const uint64_t r = (ty::gamecore_sub_0142C530(rdram, runtime, p4, p5));
    ckEnd(864, ctx, c, 2, r, 0.0f, 0x78u, 0x0u);
}
void setBossPathColiGroup(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t p4)
{
    if (!ckBegin(865, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    ref::setBossPathColiGroup(rdram, &c, runtime);
    ckMid();
    const uint64_t r = a1_sx64(ty::setBossPathColiGroup(rdram, runtime, p4));
    ckEnd(865, ctx, c, 1, r, 0.0f, 0x0u, 0x0u);
}
void setHorsePathColiGroup(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t p4)
{
    if (!ckBegin(866, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    ref::setHorsePathColiGroup(rdram, &c, runtime);
    ckMid();
    const uint64_t r = a1_sx64(ty::setHorsePathColiGroup(rdram, runtime, p4));
    ckEnd(866, ctx, c, 1, r, 0.0f, 0x0u, 0x0u);
}
void getPathColiGroupByType(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint64_t p4)
{
    if (!ckBegin(867, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    ref::getPathColiGroupByType(rdram, &c, runtime);
    ckMid();
    const uint64_t r = a1_sx64(ty::getPathColiGroupByType(rdram, runtime, p4));
    ckEnd(867, ctx, c, 1, r, 0.0f, 0x8u, 0x0u);
}
void getPassageWork(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t p4, uint64_t p5)
{
    if (!ckBegin(868, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    ref::getPassageWork(rdram, &c, runtime);
    ckMid();
    const uint64_t r = (ty::getPassageWork(rdram, runtime, p4, p5));
    ckEnd(868, ctx, c, 2, r, 0.0f, 0x0u, 0x0u);
}
void getPassageNum(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t p4)
{
    if (!ckBegin(869, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    ref::getPassageNum(rdram, &c, runtime);
    ckMid();
    const uint64_t r = a1_sx64(ty::getPassageNum(rdram, runtime, p4));
    ckEnd(869, ctx, c, 1, r, 0.0f, 0x0u, 0x0u);
}
void setRealColJudgeFunc(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t p4, uint32_t p5)
{
    if (!ckBegin(870, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    ref::setRealColJudgeFunc(rdram, &c, runtime);
    ckMid();
    ty::setRealColJudgeFunc(rdram, runtime, p4, p5);
    ckEnd(870, ctx, c, 0, 0u, 0.0f, 0x0u, 0x0u);
}
void getRealColJudgeFunc(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t p4)
{
    if (!ckBegin(871, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    ref::getRealColJudgeFunc(rdram, &c, runtime);
    ckMid();
    const uint64_t r = a1_sx64(ty::getRealColJudgeFunc(rdram, runtime, p4));
    ckEnd(871, ctx, c, 1, r, 0.0f, 0x0u, 0x0u);
}
void setSelfColAndNearMinDist(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t p4, uint32_t p5, float pf12)
{
    if (!ckBegin(872, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    ref::setSelfColAndNearMinDist(rdram, &c, runtime);
    ckMid();
    ty::setSelfColAndNearMinDist(rdram, runtime, p4, p5, pf12);
    ckEnd(872, ctx, c, 0, 0u, 0.0f, 0x0u, 0x0u);
}
void setStabilizeYPathWork(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t p4, uint32_t p5)
{
    if (!ckBegin(873, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    ref::setStabilizeYPathWork(rdram, &c, runtime);
    ckMid();
    ty::setStabilizeYPathWork(rdram, runtime, p4, p5);
    ckEnd(873, ctx, c, 0, 0u, 0.0f, 0x0u, 0x0u);
}
void setAngleStabilizeYPathWork(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t p4, uint32_t p5)
{
    if (!ckBegin(874, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    ref::setAngleStabilizeYPathWork(rdram, &c, runtime);
    ckMid();
    ty::setAngleStabilizeYPathWork(rdram, runtime, p4, p5);
    ckEnd(874, ctx, c, 0, 0u, 0.0f, 0x0u, 0x0u);
}
void setRayByRealColPathWork(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t p4, uint32_t p5)
{
    if (!ckBegin(875, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    ref::setRayByRealColPathWork(rdram, &c, runtime);
    ckMid();
    ty::setRayByRealColPathWork(rdram, runtime, p4, p5);
    ckEnd(875, ctx, c, 0, 0u, 0.0f, 0x0u, 0x0u);
}
void setPassageWideRayPathWork(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t p4, uint32_t p5)
{
    if (!ckBegin(876, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    ref::setPassageWideRayPathWork(rdram, &c, runtime);
    ckMid();
    ty::setPassageWideRayPathWork(rdram, runtime, p4, p5);
    ckEnd(876, ctx, c, 0, 0u, 0.0f, 0x0u, 0x0u);
}
void setExecRayChkPathWork(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t p4, uint32_t p5)
{
    if (!ckBegin(877, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    ref::setExecRayChkPathWork(rdram, &c, runtime);
    ckMid();
    ty::setExecRayChkPathWork(rdram, runtime, p4, p5);
    ckEnd(877, ctx, c, 0, 0u, 0.0f, 0x0u, 0x0u);
}
void setForceRouteAttrPathWork(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t p4, uint32_t p5)
{
    if (!ckBegin(878, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    ref::setForceRouteAttrPathWork(rdram, &c, runtime);
    ckMid();
    ty::setForceRouteAttrPathWork(rdram, runtime, p4, p5);
    ckEnd(878, ctx, c, 0, 0u, 0.0f, 0x0u, 0x0u);
}
void setRouteColCheckPathWork(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t p4, uint32_t p5)
{
    if (!ckBegin(879, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    ref::setRouteColCheckPathWork(rdram, &c, runtime);
    ckMid();
    ty::setRouteColCheckPathWork(rdram, runtime, p4, p5);
    ckEnd(879, ctx, c, 0, 0u, 0.0f, 0x0u, 0x0u);
}
void setRouteLockPathWork(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t p4, uint32_t p5)
{
    if (!ckBegin(880, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    ref::setRouteLockPathWork(rdram, &c, runtime);
    ckMid();
    ty::setRouteLockPathWork(rdram, runtime, p4, p5);
    ckEnd(880, ctx, c, 0, 0u, 0.0f, 0x0u, 0x0u);
}
void setPlayerWayColCheckPathWork(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t p4, uint32_t p5)
{
    if (!ckBegin(881, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    ref::setPlayerWayColCheckPathWork(rdram, &c, runtime);
    ckMid();
    ty::setPlayerWayColCheckPathWork(rdram, runtime, p4, p5);
    ckEnd(881, ctx, c, 0, 0u, 0.0f, 0x0u, 0x0u);
}
void setFollowModePathWork(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t p4, uint32_t p5)
{
    if (!ckBegin(882, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    ref::setFollowModePathWork(rdram, &c, runtime);
    ckMid();
    ty::setFollowModePathWork(rdram, runtime, p4, p5);
    ckEnd(882, ctx, c, 0, 0u, 0.0f, 0x0u, 0x0u);
}
void setUserPathMode(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t p4, uint32_t p5)
{
    if (!ckBegin(883, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    ref::setUserPathMode(rdram, &c, runtime);
    ckMid();
    ty::setUserPathMode(rdram, runtime, p4, p5);
    ckEnd(883, ctx, c, 0, 0u, 0.0f, 0x0u, 0x0u);
}
void setPassageRangeData(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t p4, float pf12, float pf13)
{
    if (!ckBegin(884, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    ref::setPassageRangeData(rdram, &c, runtime);
    ckMid();
    ty::setPassageRangeData(rdram, runtime, p4, pf12, pf13);
    ckEnd(884, ctx, c, 0, 0u, 0.0f, 0x0u, 0x0u);
}
void setAttainRangeStruct(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t p4, uint32_t p5)
{
    if (!ckBegin(885, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    ref::setAttainRangeStruct(rdram, &c, runtime);
    ckMid();
    const uint64_t r = (ty::setAttainRangeStruct(rdram, runtime, p4, p5));
    ckEnd(885, ctx, c, 2, r, 0.0f, 0x8u, 0x0u);
}
void setCreateTempAttain(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t p4, uint32_t p5)
{
    if (!ckBegin(886, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    ref::setCreateTempAttain(rdram, &c, runtime);
    ckMid();
    ty::setCreateTempAttain(rdram, runtime, p4, p5);
    ckEnd(886, ctx, c, 0, 0u, 0.0f, 0x0u, 0x0u);
}
void setAttainModePathWork(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t p4, uint32_t p5)
{
    if (!ckBegin(887, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    ref::setAttainModePathWork(rdram, &c, runtime);
    ckMid();
    ty::setAttainModePathWork(rdram, runtime, p4, p5);
    ckEnd(887, ctx, c, 0, 0u, 0.0f, 0x0u, 0x0u);
}
void setAttainRadPathWork(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t p4, float pf12)
{
    if (!ckBegin(888, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    ref::setAttainRadPathWork(rdram, &c, runtime);
    ckMid();
    ty::setAttainRadPathWork(rdram, runtime, p4, pf12);
    ckEnd(888, ctx, c, 0, 0u, 0.0f, 0x0u, 0x0u);
}
void getPathInfo(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t p4)
{
    if (!ckBegin(889, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    ref::getPathInfo(rdram, &c, runtime);
    ckMid();
    const uint64_t r = a1_sx64(ty::getPathInfo(rdram, runtime, p4));
    ckEnd(889, ctx, c, 1, r, 0.0f, 0xau, 0x0u);
}
void getWayFinish(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t p4)
{
    if (!ckBegin(890, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    ref::getWayFinish(rdram, &c, runtime);
    ckMid();
    const uint64_t r = (ty::getWayFinish(rdram, runtime, p4));
    ckEnd(890, ctx, c, 2, r, 0.0f, 0x8u, 0x0u);
}
void getPathStatus(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t p4)
{
    if (!ckBegin(891, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    ref::getPathStatus(rdram, &c, runtime);
    ckMid();
    const uint64_t r = a1_sx64(ty::getPathStatus(rdram, runtime, p4));
    ckEnd(891, ctx, c, 1, r, 0.0f, 0x28u, 0x0u);
}
void getLastPassagePointIdx(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t p4)
{
    if (!ckBegin(892, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    ref::getLastPassagePointIdx(rdram, &c, runtime);
    ckMid();
    const uint64_t r = a1_sx64(ty::getLastPassagePointIdx(rdram, runtime, p4));
    ckEnd(892, ctx, c, 1, r, 0.0f, 0x0u, 0x0u);
}
void getLastPassageAreaIdx(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t p4)
{
    if (!ckBegin(893, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    ref::getLastPassageAreaIdx(rdram, &c, runtime);
    ckMid();
    const uint64_t r = a1_sx64(ty::getLastPassageAreaIdx(rdram, runtime, p4));
    ckEnd(893, ctx, c, 1, r, 0.0f, 0x0u, 0x0u);
}
void getNowPassagePointIdx(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t p4)
{
    if (!ckBegin(894, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    ref::getNowPassagePointIdx(rdram, &c, runtime);
    ckMid();
    const uint64_t r = a1_sx64(ty::getNowPassagePointIdx(rdram, runtime, p4));
    ckEnd(894, ctx, c, 1, r, 0.0f, 0x0u, 0x0u);
}
void getNowPassageAreaIdx(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t p4)
{
    if (!ckBegin(895, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    ref::getNowPassageAreaIdx(rdram, &c, runtime);
    ckMid();
    const uint64_t r = a1_sx64(ty::getNowPassageAreaIdx(rdram, runtime, p4));
    ckEnd(895, ctx, c, 1, r, 0.0f, 0x0u, 0x0u);
}
void setSearchRadius(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t p4, float pf12)
{
    if (!ckBegin(896, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    ref::setSearchRadius(rdram, &c, runtime);
    ckMid();
    const a1::RetVF r = ty::setSearchRadius(rdram, runtime, p4, pf12);
    ckEnd(896, ctx, c, 4, r.v0, r.f0, 0x0u, 0x0u);
}
void setSearchPointMax(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t p4, uint64_t p5)
{
    if (!ckBegin(897, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    ref::setSearchPointMax(rdram, &c, runtime);
    ckMid();
    const uint64_t r = a1_sx64(ty::setSearchPointMax(rdram, runtime, p4, p5));
    ckEnd(897, ctx, c, 1, r, 0.0f, 0x0u, 0x0u);
}
void setPathRayWideParam(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t p4, uint32_t p5, float pf12)
{
    if (!ckBegin(898, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    ref::setPathRayWideParam(rdram, &c, runtime);
    ckMid();
    ty::setPathRayWideParam(rdram, runtime, p4, p5, pf12);
    ckEnd(898, ctx, c, 0, 0u, 0.0f, 0x0u, 0x0u);
}
void resetPassageWork(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t p4)
{
    if (!ckBegin(899, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    ref::resetPassageWork(rdram, &c, runtime);
    ckMid();
    ty::resetPassageWork(rdram, runtime, p4);
    ckEnd(899, ctx, c, 0, 0u, 0.0f, 0x0u, 0x0u);
}
void getColiGroupByUserPathWork(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t p4)
{
    if (!ckBegin(900, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    ref::getColiGroupByUserPathWork(rdram, &c, runtime);
    ckMid();
    const uint64_t r = a1_sx64(ty::getColiGroupByUserPathWork(rdram, runtime, p4));
    ckEnd(900, ctx, c, 1, r, 0.0f, 0x8u, 0x0u);
}
void gamecore_sub_01434CF8(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint64_t p4)
{
    if (!ckBegin(901, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    ref::gamecore_sub_01434CF8(rdram, &c, runtime);
    ckMid();
    const uint64_t r = (ty::gamecore_sub_01434CF8(rdram, runtime, p4));
    ckEnd(901, ctx, c, 2, r, 0.0f, 0x38u, 0x0u);
}
void gamecore_sub_01434F90(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t p4)
{
    if (!ckBegin(902, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    ref::gamecore_sub_01434F90(rdram, &c, runtime);
    ckMid();
    const a1::RetVF r = ty::gamecore_sub_01434F90(rdram, runtime, p4);
    ckEnd(902, ctx, c, 4, r.v0, r.f0, 0x8u, 0x2u);
}
void setUserPathExMode(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t p4, uint32_t p5)
{
    if (!ckBegin(903, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    ref::setUserPathExMode(rdram, &c, runtime);
    ckMid();
    ty::setUserPathExMode(rdram, runtime, p4, p5);
    ckEnd(903, ctx, c, 0, 0u, 0.0f, 0x0u, 0x0u);
}
void setUserPathGroupID(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t p4, uint32_t p5, uint32_t p6)
{
    if (!ckBegin(904, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    ref::setUserPathGroupID(rdram, &c, runtime);
    ckMid();
    ty::setUserPathGroupID(rdram, runtime, p4, p5, p6);
    ckEnd(904, ctx, c, 0, 0u, 0.0f, 0x0u, 0x0u);
}
void setGPSelectMode(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t p4, uint32_t p5)
{
    if (!ckBegin(905, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    ref::setGPSelectMode(rdram, &c, runtime);
    ckMid();
    ty::setGPSelectMode(rdram, runtime, p4, p5);
    ckEnd(905, ctx, c, 0, 0u, 0.0f, 0x0u, 0x0u);
}
void setForceInitPathEx(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t p4, uint32_t p5)
{
    if (!ckBegin(906, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    ref::setForceInitPathEx(rdram, &c, runtime);
    ckMid();
    ty::setForceInitPathEx(rdram, runtime, p4, p5);
    ckEnd(906, ctx, c, 0, 0u, 0.0f, 0x0u, 0x0u);
}
void setGpReferencePathEx(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t p4, uint32_t p5)
{
    if (!ckBegin(907, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    ref::setGpReferencePathEx(rdram, &c, runtime);
    ckMid();
    ty::setGpReferencePathEx(rdram, runtime, p4, p5);
    ckEnd(907, ctx, c, 0, 0u, 0.0f, 0x0u, 0x0u);
}
void gamecore_sub_01435EF0(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t p4)
{
    if (!ckBegin(908, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    ref::gamecore_sub_01435EF0(rdram, &c, runtime);
    ckMid();
    const uint64_t r = a1_sx64(ty::gamecore_sub_01435EF0(rdram, runtime, p4));
    ckEnd(908, ctx, c, 1, r, 0.0f, 0x0u, 0x0u);
}
void _setExecBaseMoveWork(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t p4, uint32_t p5)
{
    if (!ckBegin(909, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    ref::_setExecBaseMoveWork(rdram, &c, runtime);
    ckMid();
    ty::_setExecBaseMoveWork(rdram, runtime, p4, p5);
    ckEnd(909, ctx, c, 0, 0u, 0.0f, 0x0u, 0x0u);
}
void _setStopCtrlModeBaseMoveWork(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t p4, uint32_t p5)
{
    if (!ckBegin(910, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    ref::_setStopCtrlModeBaseMoveWork(rdram, &c, runtime);
    ckMid();
    ty::_setStopCtrlModeBaseMoveWork(rdram, runtime, p4, p5);
    ckEnd(910, ctx, c, 0, 0u, 0.0f, 0x0u, 0x0u);
}
void _getStopCtrlModeBaseMoveWork(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t p4)
{
    if (!ckBegin(911, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    ref::_getStopCtrlModeBaseMoveWork(rdram, &c, runtime);
    ckMid();
    const uint64_t r = a1_sx64(ty::_getStopCtrlModeBaseMoveWork(rdram, runtime, p4));
    ckEnd(911, ctx, c, 1, r, 0.0f, 0x0u, 0x0u);
}
void _setBackVolumeBaseMoveWork(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t p4, uint32_t p5)
{
    if (!ckBegin(912, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    ref::_setBackVolumeBaseMoveWork(rdram, &c, runtime);
    ckMid();
    ty::_setBackVolumeBaseMoveWork(rdram, runtime, p4, p5);
    ckEnd(912, ctx, c, 0, 0u, 0.0f, 0x0u, 0x0u);
}
void _setSlideVolumeBaseMoveWork(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t p4, uint32_t p5)
{
    if (!ckBegin(913, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    ref::_setSlideVolumeBaseMoveWork(rdram, &c, runtime);
    ckMid();
    ty::_setSlideVolumeBaseMoveWork(rdram, runtime, p4, p5);
    ckEnd(913, ctx, c, 0, 0u, 0.0f, 0x0u, 0x0u);
}
void _setAttainFlagBaseMoveWork(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t p4, uint32_t p5)
{
    if (!ckBegin(914, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    ref::_setAttainFlagBaseMoveWork(rdram, &c, runtime);
    ckMid();
    ty::_setAttainFlagBaseMoveWork(rdram, runtime, p4, p5);
    ckEnd(914, ctx, c, 0, 0u, 0.0f, 0x0u, 0x0u);
}
void gamecore_sub_01438448(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t p4)
{
    if (!ckBegin(915, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    ref::gamecore_sub_01438448(rdram, &c, runtime);
    ckMid();
    const a1::RetVF r = ty::gamecore_sub_01438448(rdram, runtime, p4);
    ckEnd(915, ctx, c, 4, r.v0, r.f0, 0x2u, 0x2u);
}
void resetRouteAttackBirdCtrlWork(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t p4)
{
    if (!ckBegin(916, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    ref::resetRouteAttackBirdCtrlWork(rdram, &c, runtime);
    ckMid();
    ty::resetRouteAttackBirdCtrlWork(rdram, runtime, p4);
    ckEnd(916, ctx, c, 0, 0u, 0.0f, 0x0u, 0x0u);
}
void setFlapWingBirdCtrlWork(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t p4, uint32_t p5)
{
    if (!ckBegin(917, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    ref::setFlapWingBirdCtrlWork(rdram, &c, runtime);
    ckMid();
    ty::setFlapWingBirdCtrlWork(rdram, runtime, p4, p5);
    ckEnd(917, ctx, c, 0, 0u, 0.0f, 0x0u, 0x0u);
}
void setHoveringBirdCtrlWork(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t p4, uint32_t p5)
{
    if (!ckBegin(918, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    ref::setHoveringBirdCtrlWork(rdram, &c, runtime);
    ckMid();
    ty::setHoveringBirdCtrlWork(rdram, runtime, p4, p5);
    ckEnd(918, ctx, c, 0, 0u, 0.0f, 0x0u, 0x0u);
}
void setHoverAttackBirdCtrlWork(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t p4, uint32_t p5)
{
    if (!ckBegin(919, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    ref::setHoverAttackBirdCtrlWork(rdram, &c, runtime);
    ckMid();
    ty::setHoverAttackBirdCtrlWork(rdram, runtime, p4, p5);
    ckEnd(919, ctx, c, 0, 0u, 0.0f, 0x0u, 0x0u);
}
void gamecore_sub_0143D660(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t p4)
{
    if (!ckBegin(920, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    ref::gamecore_sub_0143D660(rdram, &c, runtime);
    ckMid();
    const a1::RetVF r = ty::gamecore_sub_0143D660(rdram, runtime, p4);
    ckEnd(920, ctx, c, 4, r.v0, r.f0, 0x2u, 0x2u);
}
void setDashFastLegCtrlWork(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t p4, uint32_t p5)
{
    if (!ckBegin(921, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    ref::setDashFastLegCtrlWork(rdram, &c, runtime);
    ckMid();
    ty::setDashFastLegCtrlWork(rdram, runtime, p4, p5);
    ckEnd(921, ctx, c, 0, 0u, 0.0f, 0x0u, 0x0u);
}
void setFinalAttainWalkFastLegCtrlWork(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t p4, uint32_t p5)
{
    if (!ckBegin(922, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    ref::setFinalAttainWalkFastLegCtrlWork(rdram, &c, runtime);
    ckMid();
    ty::setFinalAttainWalkFastLegCtrlWork(rdram, runtime, p4, p5);
    ckEnd(922, ctx, c, 0, 0u, 0.0f, 0x0u, 0x0u);
}
void setAllowMaxSpeedFastLegCtrlWork(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t p4, float pf12)
{
    if (!ckBegin(923, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    ref::setAllowMaxSpeedFastLegCtrlWork(rdram, &c, runtime);
    ckMid();
    const float r = ty::setAllowMaxSpeedFastLegCtrlWork(rdram, runtime, p4, pf12);
    ckEnd(923, ctx, c, 3, 0u, r, 0x0u, 0x0u);
}
void setBackFastLegCtrlWork(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t p4, uint32_t p5)
{
    if (!ckBegin(924, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    ref::setBackFastLegCtrlWork(rdram, &c, runtime);
    ckMid();
    ty::setBackFastLegCtrlWork(rdram, runtime, p4, p5);
    ckEnd(924, ctx, c, 0, 0u, 0.0f, 0x0u, 0x0u);
}
void setNotFastTurnFastLegCtrlWork(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t p4, uint32_t p5)
{
    if (!ckBegin(925, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    ref::setNotFastTurnFastLegCtrlWork(rdram, &c, runtime);
    ckMid();
    ty::setNotFastTurnFastLegCtrlWork(rdram, runtime, p4, p5);
    ckEnd(925, ctx, c, 0, 0u, 0.0f, 0x0u, 0x0u);
}
void setRunStopTurnFastLegCtrlWork(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t p4, uint32_t p5)
{
    if (!ckBegin(926, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    ref::setRunStopTurnFastLegCtrlWork(rdram, &c, runtime);
    ckMid();
    ty::setRunStopTurnFastLegCtrlWork(rdram, runtime, p4, p5);
    ckEnd(926, ctx, c, 0, 0u, 0.0f, 0x0u, 0x0u);
}
void setWalkStopTurnFastLegCtrlWork(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t p4, uint32_t p5)
{
    if (!ckBegin(927, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    ref::setWalkStopTurnFastLegCtrlWork(rdram, &c, runtime);
    ckMid();
    ty::setWalkStopTurnFastLegCtrlWork(rdram, runtime, p4, p5);
    ckEnd(927, ctx, c, 0, 0u, 0.0f, 0x0u, 0x0u);
}
void setCurveCollisionFastLegCtrlWork(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t p4, uint32_t p5)
{
    if (!ckBegin(928, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    ref::setCurveCollisionFastLegCtrlWork(rdram, &c, runtime);
    ckMid();
    ty::setCurveCollisionFastLegCtrlWork(rdram, runtime, p4, p5);
    ckEnd(928, ctx, c, 0, 0u, 0.0f, 0x0u, 0x0u);
}
void setExecGazeCtrlWork(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t p4, uint32_t p5)
{
    if (!ckBegin(929, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    ref::setExecGazeCtrlWork(rdram, &c, runtime);
    ckMid();
    ty::setExecGazeCtrlWork(rdram, runtime, p4, p5);
    ckEnd(929, ctx, c, 0, 0u, 0.0f, 0x0u, 0x0u);
}
void setViewJudgeGazeCtrlWork(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t p4, uint32_t p5)
{
    if (!ckBegin(930, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    ref::setViewJudgeGazeCtrlWork(rdram, &c, runtime);
    ckMid();
    ty::setViewJudgeGazeCtrlWork(rdram, runtime, p4, p5);
    ckEnd(930, ctx, c, 0, 0u, 0.0f, 0x0u, 0x0u);
}
void setNextDistGazeCtrlWork(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t p4, float pf12)
{
    if (!ckBegin(931, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    ref::setNextDistGazeCtrlWork(rdram, &c, runtime);
    ckMid();
    ty::setNextDistGazeCtrlWork(rdram, runtime, p4, pf12);
    ckEnd(931, ctx, c, 0, 0u, 0.0f, 0x0u, 0x0u);
}
void setValidPosGazeCtrlWork(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t p4, uint32_t p5)
{
    if (!ckBegin(932, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    ref::setValidPosGazeCtrlWork(rdram, &c, runtime);
    ckMid();
    ty::setValidPosGazeCtrlWork(rdram, runtime, p4, p5);
    ckEnd(932, ctx, c, 0, 0u, 0.0f, 0x0u, 0x0u);
}
void setBirdModeGazeCtrlWork(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t p4, uint32_t p5)
{
    if (!ckBegin(933, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    ref::setBirdModeGazeCtrlWork(rdram, &c, runtime);
    ckMid();
    ty::setBirdModeGazeCtrlWork(rdram, runtime, p4, p5);
    ckEnd(933, ctx, c, 0, 0u, 0.0f, 0x0u, 0x0u);
}
void setGazeCoffGazeCtrlWork(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t p4, float pf12)
{
    if (!ckBegin(934, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    ref::setGazeCoffGazeCtrlWork(rdram, &c, runtime);
    ckMid();
    ty::setGazeCoffGazeCtrlWork(rdram, runtime, p4, pf12);
    ckEnd(934, ctx, c, 0, 0u, 0.0f, 0x0u, 0x0u);
}
void setDashHorseCtrlWork(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t p4, uint32_t p5)
{
    if (!ckBegin(935, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    ref::setDashHorseCtrlWork(rdram, &c, runtime);
    ckMid();
    ty::setDashHorseCtrlWork(rdram, runtime, p4, p5);
    ckEnd(935, ctx, c, 0, 0u, 0.0f, 0x0u, 0x0u);
}
void setWallStopHorseCtrlWork(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t p4, uint32_t p5)
{
    if (!ckBegin(936, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    ref::setWallStopHorseCtrlWork(rdram, &c, runtime);
    ckMid();
    ty::setWallStopHorseCtrlWork(rdram, runtime, p4, p5);
    ckEnd(936, ctx, c, 0, 0u, 0.0f, 0x0u, 0x0u);
}
void setLimitModeHorseCtrlWork(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t p4, uint32_t p5)
{
    if (!ckBegin(937, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    ref::setLimitModeHorseCtrlWork(rdram, &c, runtime);
    ckMid();
    ty::setLimitModeHorseCtrlWork(rdram, runtime, p4, p5);
    ckEnd(937, ctx, c, 0, 0u, 0.0f, 0x0u, 0x0u);
}
void setFinalAttainWalkHorseCtrlWork(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t p4, uint32_t p5)
{
    if (!ckBegin(938, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    ref::setFinalAttainWalkHorseCtrlWork(rdram, &c, runtime);
    ckMid();
    ty::setFinalAttainWalkHorseCtrlWork(rdram, runtime, p4, p5);
    ckEnd(938, ctx, c, 0, 0u, 0.0f, 0x0u, 0x0u);
}
void setAllowMaxSpeedHorseCtrlWork(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t p4, float pf12)
{
    if (!ckBegin(939, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    ref::setAllowMaxSpeedHorseCtrlWork(rdram, &c, runtime);
    ckMid();
    const a1::RetVF r = ty::setAllowMaxSpeedHorseCtrlWork(rdram, runtime, p4, pf12);
    ckEnd(939, ctx, c, 4, r.v0, r.f0, 0x0u, 0x0u);
}
void setFreezeHorseCtrlWork(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t p4, uint32_t p5)
{
    if (!ckBegin(940, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    ref::setFreezeHorseCtrlWork(rdram, &c, runtime);
    ckMid();
    ty::setFreezeHorseCtrlWork(rdram, runtime, p4, p5);
    ckEnd(940, ctx, c, 0, 0u, 0.0f, 0x0u, 0x0u);
}
void chkMoveAngle(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, float pf12, float pf13)
{
    if (!ckBegin(941, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    ref::chkMoveAngle(rdram, &c, runtime);
    ckMid();
    const a1::RetVF r = ty::chkMoveAngle(rdram, runtime, pf12, pf13);
    ckEnd(941, ctx, c, 4, r.v0, r.f0, 0x2u, 0x2u);
}
void adjustAngleForLimit(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, float pf12, float pf13)
{
    if (!ckBegin(942, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    ref::adjustAngleForLimit(rdram, &c, runtime);
    ckMid();
    const float r = ty::adjustAngleForLimit(rdram, runtime, pf12, pf13);
    ckEnd(942, ctx, c, 3, 0u, r, 0x0u, 0x0u);
}
void setDashSlowLegCtrlWork(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t p4, uint32_t p5)
{
    if (!ckBegin(943, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    ref::setDashSlowLegCtrlWork(rdram, &c, runtime);
    ckMid();
    ty::setDashSlowLegCtrlWork(rdram, runtime, p4, p5);
    ckEnd(943, ctx, c, 0, 0u, 0.0f, 0x0u, 0x0u);
}
void setStabilizeYSlowLegCtrlWork(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t p4, uint32_t p5)
{
    if (!ckBegin(944, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    ref::setStabilizeYSlowLegCtrlWork(rdram, &c, runtime);
    ckMid();
    ty::setStabilizeYSlowLegCtrlWork(rdram, runtime, p4, p5);
    ckEnd(944, ctx, c, 0, 0u, 0.0f, 0x0u, 0x0u);
}
void setMoveModeSlowLegCtrlWork(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t p4, uint32_t p5)
{
    if (!ckBegin(945, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    ref::setMoveModeSlowLegCtrlWork(rdram, &c, runtime);
    ckMid();
    ty::setMoveModeSlowLegCtrlWork(rdram, runtime, p4, p5);
    ckEnd(945, ctx, c, 0, 0u, 0.0f, 0x0u, 0x0u);
}
void setMotScaleMaxSlowLegCtrlWork(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t p4, float pf12)
{
    if (!ckBegin(946, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    ref::setMotScaleMaxSlowLegCtrlWork(rdram, &c, runtime);
    ckMid();
    ty::setMotScaleMaxSlowLegCtrlWork(rdram, runtime, p4, pf12);
    ckEnd(946, ctx, c, 0, 0u, 0.0f, 0x0u, 0x0u);
}
void setGuardModeSlowLegCtrlWork(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t p4, uint32_t p5)
{
    if (!ckBegin(947, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    ref::setGuardModeSlowLegCtrlWork(rdram, &c, runtime);
    ckMid();
    ty::setGuardModeSlowLegCtrlWork(rdram, runtime, p4, p5);
    ckEnd(947, ctx, c, 0, 0u, 0.0f, 0x0u, 0x0u);
}
void gamecore_sub_014498B8(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t p4)
{
    if (!ckBegin(948, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    ref::gamecore_sub_014498B8(rdram, &c, runtime);
    ckMid();
    ty::gamecore_sub_014498B8(rdram, runtime, p4);
    ckEnd(948, ctx, c, 0, 0u, 0.0f, 0x0u, 0x0u);
}
void setAttainSpeedWormCtrlWork(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t p4, float pf12)
{
    if (!ckBegin(949, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    ref::setAttainSpeedWormCtrlWork(rdram, &c, runtime);
    ckMid();
    const a1::RetVF r = ty::setAttainSpeedWormCtrlWork(rdram, runtime, p4, pf12);
    ckEnd(949, ctx, c, 4, r.v0, r.f0, 0x0u, 0x0u);
}
void setAttackParamWormCtrlWork(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t p4, float pf12)
{
    if (!ckBegin(950, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    ref::setAttackParamWormCtrlWork(rdram, &c, runtime);
    ckMid();
    ty::setAttackParamWormCtrlWork(rdram, runtime, p4, pf12);
    ckEnd(950, ctx, c, 0, 0u, 0.0f, 0x0u, 0x0u);
}
void setRotParamWormCtrlWork(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t p4, float pf12, float pf13)
{
    if (!ckBegin(951, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    ref::setRotParamWormCtrlWork(rdram, &c, runtime);
    ckMid();
    const float r = ty::setRotParamWormCtrlWork(rdram, runtime, p4, pf12, pf13);
    ckEnd(951, ctx, c, 3, 0u, r, 0x2u, 0x2000u);
}
void setChkKeepHitWormCtrlWork(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t p4, uint32_t p5)
{
    if (!ckBegin(952, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    ref::setChkKeepHitWormCtrlWork(rdram, &c, runtime);
    ckMid();
    ty::setChkKeepHitWormCtrlWork(rdram, runtime, p4, p5);
    ckEnd(952, ctx, c, 0, 0u, 0.0f, 0x0u, 0x0u);
}
void setRotColCheckWormCtrlWork(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t p4, uint32_t p5)
{
    if (!ckBegin(953, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    ref::setRotColCheckWormCtrlWork(rdram, &c, runtime);
    ckMid();
    ty::setRotColCheckWormCtrlWork(rdram, runtime, p4, p5);
    ckEnd(953, ctx, c, 0, 0u, 0.0f, 0x0u, 0x0u);
}
void setObstacleWormCtrlWork(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t p4, uint32_t p5)
{
    if (!ckBegin(954, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    ref::setObstacleWormCtrlWork(rdram, &c, runtime);
    ckMid();
    ty::setObstacleWormCtrlWork(rdram, runtime, p4, p5);
    ckEnd(954, ctx, c, 0, 0u, 0.0f, 0x0u, 0x0u);
}
void setSelfEvasionUpWormCtrlWork(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t p4, uint32_t p5)
{
    if (!ckBegin(955, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    ref::setSelfEvasionUpWormCtrlWork(rdram, &c, runtime);
    ckMid();
    ty::setSelfEvasionUpWormCtrlWork(rdram, runtime, p4, p5);
    ckEnd(955, ctx, c, 0, 0u, 0.0f, 0x0u, 0x0u);
}
void setTypeWormCtrlWork(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t p4, uint32_t p5)
{
    if (!ckBegin(956, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    ref::setTypeWormCtrlWork(rdram, &c, runtime);
    ckMid();
    ty::setTypeWormCtrlWork(rdram, runtime, p4, p5);
    ckEnd(956, ctx, c, 0, 0u, 0.0f, 0x0u, 0x0u);
}
void setFallGravCoffWormCtrlWork(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t p4, float pf12)
{
    if (!ckBegin(957, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    ref::setFallGravCoffWormCtrlWork(rdram, &c, runtime);
    ckMid();
    ty::setFallGravCoffWormCtrlWork(rdram, runtime, p4, pf12);
    ckEnd(957, ctx, c, 0, 0u, 0.0f, 0x0u, 0x0u);
}
void setMaxFallSpeedWormCtrlWork(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t p4, float pf12)
{
    if (!ckBegin(958, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    ref::setMaxFallSpeedWormCtrlWork(rdram, &c, runtime);
    ckMid();
    ty::setMaxFallSpeedWormCtrlWork(rdram, runtime, p4, pf12);
    ckEnd(958, ctx, c, 0, 0u, 0.0f, 0x0u, 0x0u);
}
void setGroundFrictionCoffWormCtrlWork(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t p4, float pf12)
{
    if (!ckBegin(959, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    ref::setGroundFrictionCoffWormCtrlWork(rdram, &c, runtime);
    ckMid();
    ty::setGroundFrictionCoffWormCtrlWork(rdram, runtime, p4, pf12);
    ckEnd(959, ctx, c, 0, 0u, 0.0f, 0x0u, 0x0u);
}
void setUnderFrictionCoffWormCtrlWork(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t p4, float pf12)
{
    if (!ckBegin(960, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    ref::setUnderFrictionCoffWormCtrlWork(rdram, &c, runtime);
    ckMid();
    ty::setUnderFrictionCoffWormCtrlWork(rdram, runtime, p4, pf12);
    ckEnd(960, ctx, c, 0, 0u, 0.0f, 0x0u, 0x0u);
}
void setWorldPitchLimitWormCtrlWork(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t p4, float pf12, float pf13)
{
    if (!ckBegin(961, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    ref::setWorldPitchLimitWormCtrlWork(rdram, &c, runtime);
    ckMid();
    const float r = ty::setWorldPitchLimitWormCtrlWork(rdram, runtime, p4, pf12, pf13);
    ckEnd(961, ctx, c, 3, 0u, r, 0x2u, 0x3000u);
}
void setColOffsetJudgeFunc(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t p4, uint32_t p5)
{
    if (!ckBegin(962, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    ref::setColOffsetJudgeFunc(rdram, &c, runtime);
    ckMid();
    ty::setColOffsetJudgeFunc(rdram, runtime, p4, p5);
    ckEnd(962, ctx, c, 0, 0u, 0.0f, 0x0u, 0x0u);
}
void gamecore_sub_0144CEA8(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t p4)
{
    if (!ckBegin(963, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    ref::gamecore_sub_0144CEA8(rdram, &c, runtime);
    ckMid();
    const a1::RetVF r = ty::gamecore_sub_0144CEA8(rdram, runtime, p4);
    ckEnd(963, ctx, c, 4, r.v0, r.f0, 0x2u, 0x2u);
}
void createArrowThrustWork(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t p5, uint32_t p6)
{
    if (!ckBegin(964, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    ref::createArrowThrustWork(rdram, &c, runtime);
    ckMid();
    ty::createArrowThrustWork(rdram, runtime, p5, p6);
    ckEnd(964, ctx, c, 0, 0u, 0.0f, 0x0u, 0x0u);
}
void SCRConditionChkDisplayOmakeCompleteAlert(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint64_t p5)
{
    if (!ckBegin(965, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    ref::SCRConditionChkDisplayOmakeCompleteAlert(rdram, &c, runtime);
    ckMid();
    const uint64_t r = a1_sx64(ty::SCRConditionChkDisplayOmakeCompleteAlert(rdram, runtime, p5));
    ckEnd(965, ctx, c, 1, r, 0.0f, 0x0u, 0x0u);
}
void setScrDemoMangerFinishCallback(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t p4, uint32_t p5)
{
    if (!ckBegin(966, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    ref::setScrDemoMangerFinishCallback(rdram, &c, runtime);
    ckMid();
    const uint64_t r = a1_sx64(ty::setScrDemoMangerFinishCallback(rdram, runtime, p4, p5));
    ckEnd(966, ctx, c, 1, r, 0.0f, 0x8u, 0x0u);
}
void initInterestWork(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t p5)
{
    if (!ckBegin(967, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    ref::initInterestWork(rdram, &c, runtime);
    ckMid();
    const uint64_t r = a1_sx64(ty::initInterestWork(rdram, runtime, p5));
    ckEnd(967, ctx, c, 1, r, 0.0f, 0x0u, 0x0u);
}
void getInterestExec(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t p4)
{
    if (!ckBegin(968, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    ref::getInterestExec(rdram, &c, runtime);
    ckMid();
    const uint64_t r = a1_sx64(ty::getInterestExec(rdram, runtime, p4));
    ckEnd(968, ctx, c, 1, r, 0.0f, 0x0u, 0x0u);
}
void setPerceptionParam(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t p4, uint32_t p5)
{
    if (!ckBegin(969, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    ref::setPerceptionParam(rdram, &c, runtime);
    ckMid();
    ty::setPerceptionParam(rdram, runtime, p4, p5);
    ckEnd(969, ctx, c, 0, 0u, 0.0f, 0x0u, 0x0u);
}
void setGoingAngleGazeIKWork(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t p4, float pf12)
{
    if (!ckBegin(970, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    ref::setGoingAngleGazeIKWork(rdram, &c, runtime);
    ckMid();
    const uint64_t r = a1_sx64(ty::setGoingAngleGazeIKWork(rdram, runtime, p4, pf12));
    ckEnd(970, ctx, c, 1, r, 0.0f, 0x0u, 0x0u);
}
void setSubParamGazeIKWork(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t p4, uint32_t p5)
{
    if (!ckBegin(971, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    ref::setSubParamGazeIKWork(rdram, &c, runtime);
    ckMid();
    const uint64_t r = a1_sx64(ty::setSubParamGazeIKWork(rdram, runtime, p4, p5));
    ckEnd(971, ctx, c, 1, r, 0.0f, 0x0u, 0x0u);
}
void setGazeModeGazeIKWork(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t p4, uint32_t p5)
{
    if (!ckBegin(972, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    ref::setGazeModeGazeIKWork(rdram, &c, runtime);
    ckMid();
    const uint64_t r = a1_sx64(ty::setGazeModeGazeIKWork(rdram, runtime, p4, p5));
    ckEnd(972, ctx, c, 1, r, 0.0f, 0x0u, 0x0u);
}
void setExecRootQuatGazeIKWork(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t p4, uint32_t p5)
{
    if (!ckBegin(973, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    ref::setExecRootQuatGazeIKWork(rdram, &c, runtime);
    ckMid();
    const uint64_t r = a1_sx64(ty::setExecRootQuatGazeIKWork(rdram, runtime, p4, p5));
    ckEnd(973, ctx, c, 1, r, 0.0f, 0x0u, 0x0u);
}
void setExecGazeIKWork(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t p4, uint32_t p5)
{
    if (!ckBegin(974, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    ref::setExecGazeIKWork(rdram, &c, runtime);
    ckMid();
    const uint64_t r = a1_sx64(ty::setExecGazeIKWork(rdram, runtime, p4, p5));
    ckEnd(974, ctx, c, 1, r, 0.0f, 0x0u, 0x0u);
}
void initTailIKWork(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t p6)
{
    if (!ckBegin(975, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    ref::initTailIKWork(rdram, &c, runtime);
    ckMid();
    ty::initTailIKWork(rdram, runtime, p6);
    ckEnd(975, ctx, c, 0, 0u, 0.0f, 0x0u, 0x0u);
}
void setWormIKBoundDynamics(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t p4, float pf12, float pf13, float pf14)
{
    if (!ckBegin(976, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    ref::setWormIKBoundDynamics(rdram, &c, runtime);
    ckMid();
    ty::setWormIKBoundDynamics(rdram, runtime, p4, pf12, pf13, pf14);
    ckEnd(976, ctx, c, 0, 0u, 0.0f, 0x0u, 0x0u);
}
void setWormIKBankInterpSec(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t p4, float pf12)
{
    if (!ckBegin(977, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    ref::setWormIKBankInterpSec(rdram, &c, runtime);
    ckMid();
    ty::setWormIKBankInterpSec(rdram, runtime, p4, pf12);
    ckEnd(977, ctx, c, 0, 0u, 0.0f, 0x0u, 0x0u);
}
void getSScriptWork(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (!ckBegin(978, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    ref::getSScriptWork(rdram, &c, runtime);
    ckMid();
    const uint64_t r = a1_sx64(ty::getSScriptWork(rdram, runtime));
    ckEnd(978, ctx, c, 1, r, 0.0f, 0x0u, 0x0u);
}
const CkEntry kEntries[979] = {
    {"SetHeapStartPoint", 0x101a30u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { SetHeapStartPoint(rdram, ctx, runtime, (uint32_t)Ps2GprU32(ctx, 4), (uint32_t)Ps2GprU32(ctx, 28)); }},
    {"GetHeapCurrentPoint", 0x101a48u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { GetHeapCurrentPoint(rdram, ctx, runtime, (uint32_t)Ps2GprU32(ctx, 28)); }},
    {"LoaderSysCheckCDBootMode", 0x102fe0u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { LoaderSysCheckCDBootMode(rdram, ctx, runtime); }},
    {"LoaderSysSendAbort", 0x1032f0u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { LoaderSysSendAbort(rdram, ctx, runtime); }},
    {"LoaderSysFlushPrint", 0x1032f8u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { LoaderSysFlushPrint(rdram, ctx, runtime); }},
    {"LoaderSysGetBootArg", 0x103ca8u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { LoaderSysGetBootArg(rdram, ctx, runtime); }},
    {"LoaderSysHookPoint", 0x103e90u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { LoaderSysHookPoint(rdram, ctx, runtime); }},
    {"initmemprintf", 0x104088u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { initmemprintf(rdram, ctx, runtime); }},
    {"padSysGet", 0x104e78u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { padSysGet(rdram, ctx, runtime, (uint32_t)Ps2GprU32(ctx, 4)); }},
    {"LoaderSysSetPowerOffCallBackFunc", 0x104f58u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { LoaderSysSetPowerOffCallBackFunc(rdram, ctx, runtime, (uint32_t)Ps2GprU32(ctx, 4), (uint32_t)Ps2GprU32(ctx, 28)); }},
    {"SetLocate", 0x106380u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { SetLocate(rdram, ctx, runtime, (uint32_t)Ps2GprU32(ctx, 4), (uint32_t)Ps2GprU32(ctx, 5), (uint32_t)Ps2GprU32(ctx, 28)); }},
    {"PutChar", 0x106b58u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { PutChar(rdram, ctx, runtime, (uint32_t)Ps2GprU32(ctx, 4), (uint32_t)Ps2GprU32(ctx, 5), (uint32_t)Ps2GprU32(ctx, 28)); }},
    {"sub_00107D90", 0x107d90u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { sub_00107D90(rdram, ctx, runtime, (uint64_t)Ps2GprU64(ctx, 4)); }},
    {"sub_00109268", 0x109268u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { sub_00109268(rdram, ctx, runtime, (uint32_t)Ps2GprU32(ctx, 4)); }},
    {"ps2___sceKernlGetEhSemaId", 0x10fbe8u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { ps2___sceKernlGetEhSemaId(rdram, ctx, runtime); }},
    {"GetTimerPreScaleFactor", 0x110640u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { GetTimerPreScaleFactor(rdram, ctx, runtime); }},
    {"sub_00110998", 0x110998u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { sub_00110998(rdram, ctx, runtime, (uint32_t)Ps2GprU32(ctx, 4)); }},
    {"iGetTimerUsedUnusedCounters", 0x110ef0u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { iGetTimerUsedUnusedCounters(rdram, ctx, runtime, (uint64_t)Ps2GprU64(ctx, 4), (uint64_t)Ps2GprU64(ctx, 5)); }},
    {"sceSifRemoveRebootNotifyHandler", 0x111c50u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { sceSifRemoveRebootNotifyHandler(rdram, ctx, runtime, (uint64_t)Ps2GprU64(ctx, 4)); }},
    {"ioctl", 0x111fc0u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { ioctl(rdram, ctx, runtime); }},
    {"isatty", 0x112080u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { isatty(rdram, ctx, runtime); }},
    {"sub_00112358", 0x112358u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { sub_00112358(rdram, ctx, runtime, (uint32_t)Ps2GprU32(ctx, 4)); }},
    {"sub_00112380", 0x112380u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { sub_00112380(rdram, ctx, runtime, (uint64_t)Ps2GprU64(ctx, 4)); }},
    {"sub_001123C0", 0x1123c0u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { sub_001123C0(rdram, ctx, runtime, (uint64_t)Ps2GprU64(ctx, 4)); }},
    {"sub_001128A0", 0x1128a0u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { sub_001128A0(rdram, ctx, runtime, (uint32_t)Ps2GprU32(ctx, 4), (uint32_t)Ps2GprU32(ctx, 5)); }},
    {"sub_001128C0", 0x1128c0u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { sub_001128C0(rdram, ctx, runtime, (uint32_t)Ps2GprU32(ctx, 4), (uint32_t)Ps2GprU32(ctx, 5)); }},
    {"sub_001148B8", 0x1148b8u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { sub_001148B8(rdram, ctx, runtime, (uint64_t)Ps2GprU64(ctx, 4)); }},
    {"_sceCdSC", 0x117a00u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { _sceCdSC(rdram, ctx, runtime, (uint32_t)Ps2GprU32(ctx, 5), (uint64_t)Ps2GprU64(ctx, 4)); }},
    {"sub_00119AA0", 0x119aa0u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { sub_00119AA0(rdram, ctx, runtime, (uint32_t)Ps2GprU32(ctx, 4)); }},
    {"ps2__SceMcComGetSysInfo", 0x120f18u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { ps2__SceMcComGetSysInfo(rdram, ctx, runtime, (uint32_t)Ps2GprU32(ctx, 4)); }},
    {"sceNetcnfifSetFNoDecode", 0x122448u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { sceNetcnfifSetFNoDecode(rdram, ctx, runtime, (uint32_t)Ps2GprU32(ctx, 4)); }},
    {"ps2___unpack_f", 0x127048u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { ps2___unpack_f(rdram, ctx, runtime, (uint32_t)Ps2GprU32(ctx, 4), (uint64_t)Ps2GprU64(ctx, 5)); }},
    {"ps2___pack_d", 0x1270d8u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { ps2___pack_d(rdram, ctx, runtime, (uint64_t)Ps2GprU64(ctx, 4), (uint64_t)Ps2GprU64(ctx, 6)); }},
    {"ps2___pack_f", 0x127390u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { ps2___pack_f(rdram, ctx, runtime, (uint64_t)Ps2GprU64(ctx, 4), (uint64_t)Ps2GprU64(ctx, 6)); }},
    {"ps2___errno", 0x127528u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { ps2___errno(rdram, ctx, runtime); }},
    {"sub_00128B00", 0x128b00u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { sub_00128B00(rdram, ctx, runtime); }},
    {"sub_0012D7F0", 0x12d7f0u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { sub_0012D7F0(rdram, ctx, runtime, (uint32_t)Ps2GprU32(ctx, 4), (uint32_t)Ps2GprU32(ctx, 5), (uint32_t)Ps2GprU32(ctx, 6), (uint32_t)Ps2GprU32(ctx, 7)); }},
    {"_localeconv_r", 0x12e520u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { _localeconv_r(rdram, ctx, runtime); }},
    {"_hi0bits", 0x12eaf8u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { _hi0bits(rdram, ctx, runtime, (uint64_t)Ps2GprU64(ctx, 4)); }},
    {"_lo0bits", 0x12eb78u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { _lo0bits(rdram, ctx, runtime, (uint32_t)Ps2GprU32(ctx, 4)); }},
    {"isnan", 0x12fa00u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { isnan(rdram, ctx, runtime, (uint64_t)Ps2GprU64(ctx, 4)); }},
    {"iosCdStStop", 0x1ace28u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { iosCdStStop(rdram, ctx, runtime); }},
    {"iosCdStSkip", 0x1acf40u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { iosCdStSkip(rdram, ctx, runtime, (uint64_t)Ps2GprU64(ctx, 4)); }},
    {"iosCdSetErrorCallback", 0x1acfb8u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { iosCdSetErrorCallback(rdram, ctx, runtime, (uint32_t)Ps2GprU32(ctx, 4), (uint32_t)Ps2GprU32(ctx, 5)); }},
    {"iosCdSetMode", 0x1acfd0u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { iosCdSetMode(rdram, ctx, runtime, (uint64_t)Ps2GprU64(ctx, 4)); }},
    {"iosInitDebPc", 0x1ad140u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { iosInitDebPc(rdram, ctx, runtime); }},
    {"iosStopPc", 0x1ad148u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { iosStopPc(rdram, ctx, runtime); }},
    {"iosDebPcVsyncCounterCheck", 0x1ad150u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { iosDebPcVsyncCounterCheck(rdram, ctx, runtime); }},
    {"iosGetCnt0Pc", 0x1ad158u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { iosGetCnt0Pc(rdram, ctx, runtime); }},
    {"iosGetCnt1Pc", 0x1ad160u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { iosGetCnt1Pc(rdram, ctx, runtime); }},
    {"InitDebPc", 0x1ad168u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { InitDebPc(rdram, ctx, runtime); }},
    {"iosGetClock", 0x1ad1f0u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { iosGetClock(rdram, ctx, runtime, (uint32_t)Ps2GprU32(ctx, 4)); }},
    {"iosEE2IopEndChk", 0x1ad948u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { iosEE2IopEndChk(rdram, ctx, runtime); }},
    {"iosCreateIopMgr", 0x1ad950u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { iosCreateIopMgr(rdram, ctx, runtime); }},
    {"iosGetMallocPtn", 0x1b0530u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { iosGetMallocPtn(rdram, ctx, runtime, (uint32_t)Ps2GprU32(ctx, 4)); }},
    {"iosGetMallocRootPtnRemainSize", 0x1b0558u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { iosGetMallocRootPtnRemainSize(rdram, ctx, runtime); }},
    {"addMemoryDebugCallback", 0x1b06a8u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { addMemoryDebugCallback(rdram, ctx, runtime, (uint32_t)Ps2GprU32(ctx, 4)); }},
    {"getMemorySafetyLockFlag", 0x1b06b8u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { getMemorySafetyLockFlag(rdram, ctx, runtime); }},
    {"kernel_sub_001B06C8", 0x1b06c8u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { kernel_sub_001B06C8(rdram, ctx, runtime); }},
    {"iosInitMessageSystem", 0x1b12b0u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { iosInitMessageSystem(rdram, ctx, runtime); }},
    {"iosPadGetXZInputLwithThresholdIndepAxis", 0x1b2438u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { iosPadGetXZInputLwithThresholdIndepAxis(rdram, ctx, runtime, (uint32_t)Ps2GprU32(ctx, 4), (uint32_t)Ps2GprU32(ctx, 5), ctx->f[12]); }},
    {"iosPadEntryTickFunc", 0x1b27a0u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { iosPadEntryTickFunc(rdram, ctx, runtime, (uint32_t)Ps2GprU32(ctx, 4)); }},
    {"iosCreatePrintMgr", 0x1b29e8u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { iosCreatePrintMgr(rdram, ctx, runtime); }},
    {"iosGetPrintfMode", 0x1b29f0u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { iosGetPrintfMode(rdram, ctx, runtime); }},
    {"endSCEDemo", 0x1b34f0u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { endSCEDemo(rdram, ctx, runtime); }},
    {"Init_ShockVoiceSet", 0x1b3c18u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { Init_ShockVoiceSet(rdram, ctx, runtime, (uint32_t)Ps2GprU32(ctx, 4), (uint32_t)Ps2GprU32(ctx, 5)); }},
    {"Shock_SetShockVoiceSet", 0x1b3d10u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { Shock_SetShockVoiceSet(rdram, ctx, runtime, (uint32_t)Ps2GprU32(ctx, 5), (uint64_t)Ps2GprU64(ctx, 4)); }},
    {"Init_Player", 0x1b3d48u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { Init_Player(rdram, ctx, runtime, (uint32_t)Ps2GprU32(ctx, 4)); }},
    {"Init_Controler", 0x1b3d88u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { Init_Controler(rdram, ctx, runtime, (uint32_t)Ps2GprU32(ctx, 4)); }},
    {"kernel_sub_001B40C8", 0x1b40c8u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { kernel_sub_001B40C8(rdram, ctx, runtime); }},
    {"kernel_sub_001B4118", 0x1b4118u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { kernel_sub_001B4118(rdram, ctx, runtime, (uint32_t)Ps2GprU32(ctx, 4)); }},
    {"IosGetMergeDataFileName", 0x1b5f18u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { IosGetMergeDataFileName(rdram, ctx, runtime); }},
    {"GetSymbolByIndex", 0x1b6798u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { GetSymbolByIndex(rdram, ctx, runtime, (uint32_t)Ps2GprU32(ctx, 4), (uint32_t)Ps2GprU32(ctx, 5)); }},
    {"isysGetXffWorkSize", 0x1b6828u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { isysGetXffWorkSize(rdram, ctx, runtime, (uint32_t)Ps2GprU32(ctx, 4)); }},
    {"isysGetModuleObjMallocSize", 0x1b6880u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { isysGetModuleObjMallocSize(rdram, ctx, runtime, (uint32_t)Ps2GprU32(ctx, 4)); }},
    {"GetExecModuleSize", 0x1b69b8u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { GetExecModuleSize(rdram, ctx, runtime); }},
    {"getModuleSize", 0x1b69c8u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { getModuleSize(rdram, ctx, runtime, (uint32_t)Ps2GprU32(ctx, 4)); }},
    {"isysCreateExceptionThread", 0x1b7b58u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { isysCreateExceptionThread(rdram, ctx, runtime); }},
    {"iosCubicBezier", 0x1b7b78u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { iosCubicBezier(rdram, ctx, runtime, (uint32_t)Ps2GprU32(ctx, 4), ctx->f[12]); }},
    {"iosFourthBezier", 0x1b7c20u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { iosFourthBezier(rdram, ctx, runtime, (uint32_t)Ps2GprU32(ctx, 4), ctx->f[12]); }},
    {"iosFloatModulo", 0x1b9308u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { iosFloatModulo(rdram, ctx, runtime, ctx->f[12], ctx->f[13]); }},
    {"iosIColToMvecSl", 0x1ba428u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { iosIColToMvecSl(rdram, ctx, runtime, (uint32_t)Ps2GprU32(ctx, 4), (uint32_t)Ps2GprU32(ctx, 5)); }},
    {"GetCurrentQuaternion", 0x1bb868u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { GetCurrentQuaternion(rdram, ctx, runtime); }},
    {"GetLastQuaternion", 0x1bb888u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { GetLastQuaternion(rdram, ctx, runtime); }},
    {"iosGetACos", 0x1be2e8u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { iosGetACos(rdram, ctx, runtime, (uint32_t)Ps2GprU32(ctx, 4)); }},
    {"iosGetASin", 0x1be4d8u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { iosGetASin(rdram, ctx, runtime); }},
    {"iosGetATan2", 0x1be4e8u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { iosGetATan2(rdram, ctx, runtime); }},
    {"resetArcFuncCall", 0x1be4f8u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { resetArcFuncCall(rdram, ctx, runtime); }},
    {"iosGetTableSin", 0x1be520u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { iosGetTableSin(rdram, ctx, runtime, (uint32_t)Ps2GprU32(ctx, 4)); }},
    {"iosDumpFVector", 0x1be8e0u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { iosDumpFVector(rdram, ctx, runtime); }},
    {"iosDumpIVector", 0x1be8e8u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { iosDumpIVector(rdram, ctx, runtime); }},
    {"iosGetScaleInverseVector", 0x1be980u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { iosGetScaleInverseVector(rdram, ctx, runtime, (uint32_t)Ps2GprU32(ctx, 4), (uint32_t)Ps2GprU32(ctx, 5)); }},
    {"iosSetDCViewPort", 0x1bf0b0u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { iosSetDCViewPort(rdram, ctx, runtime, (uint64_t)Ps2GprU64(ctx, 4), (uint64_t)Ps2GprU64(ctx, 5), (uint64_t)Ps2GprU64(ctx, 6), (uint64_t)Ps2GprU64(ctx, 7), (uint64_t)Ps2GprU64(ctx, 8)); }},
    {"iosGetRootDC", 0x1bf2d8u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { iosGetRootDC(rdram, ctx, runtime); }},
    {"iosGetCurrentDC", 0x1bf308u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { iosGetCurrentDC(rdram, ctx, runtime); }},
    {"iosGetDCGroup", 0x1bf420u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { iosGetDCGroup(rdram, ctx, runtime); }},
    {"kernel_sub_001BF430", 0x1bf430u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { kernel_sub_001BF430(rdram, ctx, runtime); }},
    {"kernel_sub_001BF438", 0x1bf438u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { kernel_sub_001BF438(rdram, ctx, runtime); }},
    {"iosDlGetTransAdrs", 0x1bfb80u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { iosDlGetTransAdrs(rdram, ctx, runtime); }},
    {"iosGetPacketPtrPath1", 0x1bfba8u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { iosGetPacketPtrPath1(rdram, ctx, runtime); }},
    {"iosSetPacketPtrPath1", 0x1bfbf0u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { iosSetPacketPtrPath1(rdram, ctx, runtime, (uint32_t)Ps2GprU32(ctx, 4)); }},
    {"iosDlChainHead", 0x1bfc30u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { iosDlChainHead(rdram, ctx, runtime, (uint64_t)Ps2GprU64(ctx, 4), (uint64_t)Ps2GprU64(ctx, 5)); }},
    {"iosDlChainHeadCurrent", 0x1bfdc0u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { iosDlChainHeadCurrent(rdram, ctx, runtime, (uint64_t)Ps2GprU64(ctx, 4)); }},
    {"iosDlGetWorkBufId", 0x1c0138u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { iosDlGetWorkBufId(rdram, ctx, runtime); }},
    {"iosDlGetTransBufId", 0x1c0150u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { iosDlGetTransBufId(rdram, ctx, runtime); }},
    {"iosDlGetEndAdrs", 0x1c0168u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { iosDlGetEndAdrs(rdram, ctx, runtime); }},
    {"iosDlGetLayerNum", 0x1c02c8u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { iosDlGetLayerNum(rdram, ctx, runtime); }},
    {"iosDlCheckAdrs", 0x1c03b8u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { iosDlCheckAdrs(rdram, ctx, runtime, (uint64_t)Ps2GprU64(ctx, 4)); }},
    {"iosDlCheckSize", 0x1c03f0u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { iosDlCheckSize(rdram, ctx, runtime); }},
    {"iosAllocPacketPtrPath1UnCachedA", 0x1c0480u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { iosAllocPacketPtrPath1UnCachedA(rdram, ctx, runtime, (uint32_t)Ps2GprU32(ctx, 4)); }},
    {"getDispScreenWidth", 0x1c2018u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { getDispScreenWidth(rdram, ctx, runtime); }},
    {"getDispScreenHeight", 0x1c2028u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { getDispScreenHeight(rdram, ctx, runtime); }},
    {"getDrawScreenWidth", 0x1c2038u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { getDrawScreenWidth(rdram, ctx, runtime); }},
    {"getDrawScreenHeight", 0x1c2048u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { getDrawScreenHeight(rdram, ctx, runtime); }},
    {"getDispBufferAddressTBP", 0x1c20e8u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { getDispBufferAddressTBP(rdram, ctx, runtime); }},
    {"getDispBufferAddressFBP", 0x1c20f0u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { getDispBufferAddressFBP(rdram, ctx, runtime); }},
    {"getZBufferAddressTBP", 0x1c2158u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { getZBufferAddressTBP(rdram, ctx, runtime); }},
    {"getZBufferAddressFBP", 0x1c2178u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { getZBufferAddressFBP(rdram, ctx, runtime); }},
    {"getScreenOffsetX", 0x1c2208u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { getScreenOffsetX(rdram, ctx, runtime); }},
    {"getScreenOffsetY", 0x1c2218u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { getScreenOffsetY(rdram, ctx, runtime); }},
    {"getAntiAlias", 0x1c2228u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { getAntiAlias(rdram, ctx, runtime); }},
    {"getProgressiveMode", 0x1c2238u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { getProgressiveMode(rdram, ctx, runtime); }},
    {"iosGetField", 0x1c28b0u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { iosGetField(rdram, ctx, runtime); }},
    {"iosSetSyncNoReturnFunc", 0x1c2a68u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { iosSetSyncNoReturnFunc(rdram, ctx, runtime, (uint32_t)Ps2GprU32(ctx, 4)); }},
    {"iosWaitDMA", 0x1c2b98u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { iosWaitDMA(rdram, ctx, runtime); }},
    {"getCRTCoffsetU", 0x1c2be8u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { getCRTCoffsetU(rdram, ctx, runtime); }},
    {"getCRTCoffsetV", 0x1c2bf8u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { getCRTCoffsetV(rdram, ctx, runtime); }},
    {"iosSetIStartFunc", 0x1c3720u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { iosSetIStartFunc(rdram, ctx, runtime, (uint32_t)Ps2GprU32(ctx, 4)); }},
    {"iosSetGpFinishFunc", 0x1c3730u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { iosSetGpFinishFunc(rdram, ctx, runtime, (uint32_t)Ps2GprU32(ctx, 4)); }},
    {"iosSetIFinishFunc", 0x1c3740u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { iosSetIFinishFunc(rdram, ctx, runtime, (uint32_t)Ps2GprU32(ctx, 4)); }},
    {"getIntrGpCount", 0x1c3750u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { getIntrGpCount(rdram, ctx, runtime); }},
    {"iosSetDlTransFunc", 0x1c3768u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { iosSetDlTransFunc(rdram, ctx, runtime, (uint32_t)Ps2GprU32(ctx, 4)); }},
    {"iosSetVSchedulerFunc", 0x1c3778u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { iosSetVSchedulerFunc(rdram, ctx, runtime, (uint32_t)Ps2GprU32(ctx, 4)); }},
    {"getSpriteDrawFirst", 0x1c5b48u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { getSpriteDrawFirst(rdram, ctx, runtime); }},
    {"gsReglist", 0x1c5b58u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { gsReglist(rdram, ctx, runtime); }},
    {"isysSetObjImmediateDeleteFunc", 0x1c7540u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { isysSetObjImmediateDeleteFunc(rdram, ctx, runtime, (uint32_t)Ps2GprU32(ctx, 4), (uint32_t)Ps2GprU32(ctx, 5)); }},
    {"isysInitGroup", 0x1c7658u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { isysInitGroup(rdram, ctx, runtime, (uint32_t)Ps2GprU32(ctx, 4)); }},
    {"isysObjDeleteLock", 0x1c7680u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { isysObjDeleteLock(rdram, ctx, runtime, (uint32_t)Ps2GprU32(ctx, 4)); }},
    {"isysGetLinkMax", 0x1c79a8u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { isysGetLinkMax(rdram, ctx, runtime); }},
    {"isysGetObjIdentifierByObj", 0x1c79b0u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { isysGetObjIdentifierByObj(rdram, ctx, runtime, (uint32_t)Ps2GprU32(ctx, 4)); }},
    {"iosGetISchedulerExecCnt", 0x1c7fa0u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { iosGetISchedulerExecCnt(rdram, ctx, runtime); }},
    {"ischedRestart", 0x1c7ff0u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { ischedRestart(rdram, ctx, runtime); }},
    {"iosSetISchedulerSubthreadTask", 0x1c8028u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { iosSetISchedulerSubthreadTask(rdram, ctx, runtime, (uint32_t)Ps2GprU32(ctx, 4)); }},
    {"iosSetIVSyncMode", 0x1c8038u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { iosSetIVSyncMode(rdram, ctx, runtime, (uint32_t)Ps2GprU32(ctx, 4)); }},
    {"iosGetIVSyncMode", 0x1c8048u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { iosGetIVSyncMode(rdram, ctx, runtime); }},
    {"vschedSetNextFrameForceGarbage", 0x1c8058u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { vschedSetNextFrameForceGarbage(rdram, ctx, runtime); }},
    {"GetGarbageTime", 0x1c8068u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { GetGarbageTime(rdram, ctx, runtime); }},
    {"init_letter", 0x1c84c0u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { init_letter(rdram, ctx, runtime, (uint32_t)Ps2GprU32(ctx, 4), (uint32_t)Ps2GprU32(ctx, 5), (uint32_t)Ps2GprU32(ctx, 6)); }},
    {"is_hope_letter_type", 0x1c84d8u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { is_hope_letter_type(rdram, ctx, runtime, (uint32_t)Ps2GprU32(ctx, 4), (uint64_t)Ps2GprU64(ctx, 5)); }},
    {"get_letter_contents_data", 0x1c84e8u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { get_letter_contents_data(rdram, ctx, runtime, (uint32_t)Ps2GprU32(ctx, 4)); }},
    {"increment_letter_refcount", 0x1c8500u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { increment_letter_refcount(rdram, ctx, runtime, (uint32_t)Ps2GprU32(ctx, 4)); }},
    {"is_letter_reach_time", 0x1c8570u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { is_letter_reach_time(rdram, ctx, runtime, (uint32_t)Ps2GprU32(ctx, 4), ctx->f[12]); }},
    {"is_init_letter_called", 0x1c8598u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { is_init_letter_called(rdram, ctx, runtime, (uint32_t)Ps2GprU32(ctx, 4)); }},
    {"get_reference_letter_type", 0x1c85a0u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { get_reference_letter_type(rdram, ctx, runtime, (uint32_t)Ps2GprU32(ctx, 4)); }},
    {"get_letter_receiver_obj_group", 0x1c85a8u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { get_letter_receiver_obj_group(rdram, ctx, runtime, (uint32_t)Ps2GprU32(ctx, 4)); }},
    {"get_letter_receiver_data", 0x1c85b0u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { get_letter_receiver_data(rdram, ctx, runtime, (uint32_t)Ps2GprU32(ctx, 4)); }},
    {"get_total_letter_in_post", 0x1c88f8u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { get_total_letter_in_post(rdram, ctx, runtime, (uint32_t)Ps2GprU32(ctx, 4)); }},
    {"kernel_sub_001C8948", 0x1c8948u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { kernel_sub_001C8948(rdram, ctx, runtime, (uint32_t)Ps2GprU32(ctx, 4)); }},
    {"constructor_letter_type", 0x1c8ad0u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { constructor_letter_type(rdram, ctx, runtime, (uint32_t)Ps2GprU32(ctx, 4)); }},
    {"get_letter_type", 0x1c8b68u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { get_letter_type(rdram, ctx, runtime, (uint64_t)Ps2GprU64(ctx, 4)); }},
    {"iosGetDataSizeOfFont", 0x1c90d0u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { iosGetDataSizeOfFont(rdram, ctx, runtime, (uint32_t)Ps2GprU32(ctx, 4)); }},
    {"iosGetFontDataWithJIS", 0x1c9160u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { iosGetFontDataWithJIS(rdram, ctx, runtime, (uint64_t)Ps2GprU64(ctx, 4), (uint64_t)Ps2GprU64(ctx, 5), (uint64_t)Ps2GprU64(ctx, 6)); }},
    {"iosGetFontIDWithJIS", 0x1c9470u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { iosGetFontIDWithJIS(rdram, ctx, runtime, (uint32_t)Ps2GprU32(ctx, 4)); }},
    {"iosEntryLocaleFontNoLoad", 0x1c9620u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { iosEntryLocaleFontNoLoad(rdram, ctx, runtime, (uint64_t)Ps2GprU64(ctx, 4), (uint64_t)Ps2GprU64(ctx, 5)); }},
    {"iosGet2ByteCharCode", 0x1c9708u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { iosGet2ByteCharCode(rdram, ctx, runtime, (uint32_t)Ps2GprU32(ctx, 4)); }},
    {"_isysCreateSubMsgQueue", 0x1c9720u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { _isysCreateSubMsgQueue(rdram, ctx, runtime, (uint32_t)Ps2GprU32(ctx, 4), (uint32_t)Ps2GprU32(ctx, 5), (uint32_t)Ps2GprU32(ctx, 6), (uint32_t)Ps2GprU32(ctx, 7)); }},
    {"isysGetSubThreadWorkSize", 0x1ca4a0u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { isysGetSubThreadWorkSize(rdram, ctx, runtime, (uint32_t)Ps2GprU32(ctx, 4)); }},
    {"isysDumpSubThreadPC", 0x1ca5a0u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { isysDumpSubThreadPC(rdram, ctx, runtime); }},
    {"isysDebugSetSubThreadSleepProtection", 0x1ca5a8u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { isysDebugSetSubThreadSleepProtection(rdram, ctx, runtime, (uint32_t)Ps2GprU32(ctx, 4)); }},
    {"sndSepSetDefaultParam", 0x1cad30u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { sndSepSetDefaultParam(rdram, ctx, runtime, (uint32_t)Ps2GprU32(ctx, 4)); }},
    {"sndSepPauseAll", 0x1cad90u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { sndSepPauseAll(rdram, ctx, runtime, (uint64_t)Ps2GprU64(ctx, 4)); }},
    {"sndSepPause", 0x1cadb8u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { sndSepPause(rdram, ctx, runtime, (uint64_t)Ps2GprU64(ctx, 4)); }},
    {"sndSepCont", 0x1cadd0u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { sndSepCont(rdram, ctx, runtime, (uint64_t)Ps2GprU64(ctx, 4)); }},
    {"sndVabDelete", 0x1caf88u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { sndVabDelete(rdram, ctx, runtime, (uint32_t)Ps2GprU32(ctx, 4)); }},
    {"sndBDClose", 0x1cb168u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { sndBDClose(rdram, ctx, runtime); }},
    {"addAdpcmTickProcFunc", 0x1cb3a0u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { addAdpcmTickProcFunc(rdram, ctx, runtime, (uint32_t)Ps2GprU32(ctx, 4)); }},
    {"setPcmTickProcFunc", 0x1cb3b0u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { setPcmTickProcFunc(rdram, ctx, runtime, (uint32_t)Ps2GprU32(ctx, 4)); }},
    {"soundGetTickProcCnt", 0x1cb3c0u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { soundGetTickProcCnt(rdram, ctx, runtime); }},
    {"soundSetRevMode", 0x1cb3d0u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { soundSetRevMode(rdram, ctx, runtime, (uint64_t)Ps2GprU64(ctx, 4), (uint64_t)Ps2GprU64(ctx, 5)); }},
    {"soundGetRevMode", 0x1cb408u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { soundGetRevMode(rdram, ctx, runtime); }},
    {"getSoundMode", 0x1cb460u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { getSoundMode(rdram, ctx, runtime); }},
    {"iosKeybdSetCallBackFunc", 0x1cb920u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { iosKeybdSetCallBackFunc(rdram, ctx, runtime, (uint32_t)Ps2GprU32(ctx, 4)); }},
    {"iosMouseSetCallBackFunc", 0x1cbc70u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { iosMouseSetCallBackFunc(rdram, ctx, runtime, (uint32_t)Ps2GprU32(ctx, 4)); }},
    {"GetStrHashWorkBuffer", 0x1cbcb8u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { GetStrHashWorkBuffer(rdram, ctx, runtime, (uint32_t)Ps2GprU32(ctx, 4), (uint32_t)Ps2GprU32(ctx, 5)); }},
    {"kernel_sub_001CC028", 0x1cc028u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { kernel_sub_001CC028(rdram, ctx, runtime); }},
    {"Sg2CtrlSetTickMode", 0x1cc228u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { Sg2CtrlSetTickMode(rdram, ctx, runtime, (uint32_t)Ps2GprU32(ctx, 4)); }},
    {"Sg2CtrlGetTickMode", 0x1cc240u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { Sg2CtrlGetTickMode(rdram, ctx, runtime); }},
    {"Sg2CtrlSetTickTime", 0x1cc250u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { Sg2CtrlSetTickTime(rdram, ctx, runtime, (uint32_t)Ps2GprU32(ctx, 4)); }},
    {"Sg2CtrlGetTickTime", 0x1cc268u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { Sg2CtrlGetTickTime(rdram, ctx, runtime); }},
    {"Sg2DmaInit", 0x1cc318u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { Sg2DmaInit(rdram, ctx, runtime); }},
    {"Sg2DspGetMasterVolume", 0x1cc7d8u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { Sg2DspGetMasterVolume(rdram, ctx, runtime, (uint32_t)Ps2GprU32(ctx, 4)); }},
    {"Sg2ExStAdpcmStopMode", 0x1cc898u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { Sg2ExStAdpcmStopMode(rdram, ctx, runtime, (uint32_t)Ps2GprU32(ctx, 4)); }},
    {"Sg2ExStAdpcmSetFd", 0x1cced0u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { Sg2ExStAdpcmSetFd(rdram, ctx, runtime, (uint32_t)Ps2GprU32(ctx, 4), (uint32_t)Ps2GprU32(ctx, 5)); }},
    {"Sg2ExStAdpcmGetFd", 0x1ccee0u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { Sg2ExStAdpcmGetFd(rdram, ctx, runtime, (uint32_t)Ps2GprU32(ctx, 4)); }},
    {"Sg2ExStAdpcmSetLsn", 0x1ccee8u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { Sg2ExStAdpcmSetLsn(rdram, ctx, runtime, (uint32_t)Ps2GprU32(ctx, 4), (uint32_t)Ps2GprU32(ctx, 5)); }},
    {"Sg2ExStAdpcmGetLsn", 0x1ccef8u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { Sg2ExStAdpcmGetLsn(rdram, ctx, runtime, (uint32_t)Ps2GprU32(ctx, 4)); }},
    {"Sg2ExStAdpcmGetTargetLsn", 0x1ccf00u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { Sg2ExStAdpcmGetTargetLsn(rdram, ctx, runtime, (uint32_t)Ps2GprU32(ctx, 4)); }},
    {"Sg2ExStAdpcmSetUserParam", 0x1ccf18u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { Sg2ExStAdpcmSetUserParam(rdram, ctx, runtime, (uint32_t)Ps2GprU32(ctx, 4), (uint32_t)Ps2GprU32(ctx, 5)); }},
    {"Sg2ExStAdpcmGetUserParam", 0x1ccf28u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { Sg2ExStAdpcmGetUserParam(rdram, ctx, runtime, (uint32_t)Ps2GprU32(ctx, 4)); }},
    {"Sg2ExStAdpcmLoopSecNum", 0x1cd6f8u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { Sg2ExStAdpcmLoopSecNum(rdram, ctx, runtime, (uint32_t)Ps2GprU32(ctx, 4), (uint32_t)Ps2GprU32(ctx, 5), (uint32_t)Ps2GprU32(ctx, 6)); }},
    {"Sg2ExStAdpcmGetBuffer", 0x1cd710u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { Sg2ExStAdpcmGetBuffer(rdram, ctx, runtime, (uint32_t)Ps2GprU32(ctx, 4), (uint32_t)Ps2GprU32(ctx, 5), (uint32_t)Ps2GprU32(ctx, 6), (uint32_t)Ps2GprU32(ctx, 7), (uint32_t)Ps2GprU32(ctx, 8)); }},
    {"Sg2ExStAdpcmSetSignal", 0x1cd738u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { Sg2ExStAdpcmSetSignal(rdram, ctx, runtime, (uint32_t)Ps2GprU32(ctx, 4)); }},
    {"Sg2ModCheckSndOn", 0x1cec38u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { Sg2ModCheckSndOn(rdram, ctx, runtime, (uint64_t)Ps2GprU64(ctx, 4)); }},
    {"Sg2ModClose", 0x1cec70u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { Sg2ModClose(rdram, ctx, runtime, (uint64_t)Ps2GprU64(ctx, 4)); }},
    {"Sg2ModGetParam", 0x1cf6e8u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { Sg2ModGetParam(rdram, ctx, runtime, (uint32_t)Ps2GprU32(ctx, 6), (uint32_t)Ps2GprU32(ctx, 7), (uint64_t)Ps2GprU64(ctx, 4), (uint64_t)Ps2GprU64(ctx, 5)); }},
    {"kernel_sub_001D0D78", 0x1d0d78u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { kernel_sub_001D0D78(rdram, ctx, runtime); }},
    {"kernel_sub_001D12A8", 0x1d12a8u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { kernel_sub_001D12A8(rdram, ctx, runtime); }},
    {"kernel_sub_001D12E0", 0x1d12e0u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { kernel_sub_001D12E0(rdram, ctx, runtime); }},
    {"kernel_sub_001D1318", 0x1d1318u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { kernel_sub_001D1318(rdram, ctx, runtime); }},
    {"kernel_sub_001D1350", 0x1d1350u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { kernel_sub_001D1350(rdram, ctx, runtime); }},
    {"kernel_sub_001D1358", 0x1d1358u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { kernel_sub_001D1358(rdram, ctx, runtime); }},
    {"kernel_sub_001D13E8", 0x1d13e8u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { kernel_sub_001D13E8(rdram, ctx, runtime); }},
    {"kernel_sub_001D1470", 0x1d1470u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { kernel_sub_001D1470(rdram, ctx, runtime); }},
    {"kernel_sub_001D14C8", 0x1d14c8u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { kernel_sub_001D14C8(rdram, ctx, runtime); }},
    {"kernel_sub_001D1520", 0x1d1520u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { kernel_sub_001D1520(rdram, ctx, runtime); }},
    {"kernel_sub_001D1578", 0x1d1578u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { kernel_sub_001D1578(rdram, ctx, runtime); }},
    {"kernel_sub_001D1580", 0x1d1580u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { kernel_sub_001D1580(rdram, ctx, runtime); }},
    {"kernel_sub_001D1658", 0x1d1658u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { kernel_sub_001D1658(rdram, ctx, runtime); }},
    {"kernel_sub_001D1660", 0x1d1660u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { kernel_sub_001D1660(rdram, ctx, runtime); }},
    {"kernel_sub_001D1698", 0x1d1698u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { kernel_sub_001D1698(rdram, ctx, runtime); }},
    {"kernel_sub_001D1810", 0x1d1810u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { kernel_sub_001D1810(rdram, ctx, runtime); }},
    {"kernel_sub_001D18D8", 0x1d18d8u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { kernel_sub_001D18D8(rdram, ctx, runtime); }},
    {"kernel_sub_001D1F08", 0x1d1f08u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { kernel_sub_001D1F08(rdram, ctx, runtime); }},
    {"kernel_sub_001D1F10", 0x1d1f10u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { kernel_sub_001D1F10(rdram, ctx, runtime); }},
    {"kernel_sub_001D1F18", 0x1d1f18u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { kernel_sub_001D1F18(rdram, ctx, runtime); }},
    {"kernel_sub_001D1F20", 0x1d1f20u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { kernel_sub_001D1F20(rdram, ctx, runtime); }},
    {"kernel_sub_001D2008", 0x1d2008u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { kernel_sub_001D2008(rdram, ctx, runtime); }},
    {"Sg2RemoteDmaTransStatus", 0x1d2150u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { Sg2RemoteDmaTransStatus(rdram, ctx, runtime, (uint32_t)Ps2GprU32(ctx, 4)); }},
    {"Sg2RemoteGetAdpStmAdr", 0x1d2170u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { Sg2RemoteGetAdpStmAdr(rdram, ctx, runtime, (uint32_t)Ps2GprU32(ctx, 4)); }},
    {"Sg2RemoteGetPcmStmTransOffset", 0x1d2188u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { Sg2RemoteGetPcmStmTransOffset(rdram, ctx, runtime); }},
    {"Sg2RemoteGetSlotReleaseLv", 0x1d2198u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { Sg2RemoteGetSlotReleaseLv(rdram, ctx, runtime, (uint32_t)Ps2GprU32(ctx, 4)); }},
    {"Sg2RemoteGetDspChangeStatus", 0x1d21b0u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { Sg2RemoteGetDspChangeStatus(rdram, ctx, runtime, (uint32_t)Ps2GprU32(ctx, 4)); }},
    {"Sg2SepStatus", 0x1d2b98u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { Sg2SepStatus(rdram, ctx, runtime, (uint64_t)Ps2GprU64(ctx, 4)); }},
    {"kernel_sub_001D3428", 0x1d3428u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { kernel_sub_001D3428(rdram, ctx, runtime); }},
    {"kernel_sub_001D3430", 0x1d3430u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { kernel_sub_001D3430(rdram, ctx, runtime, (uint32_t)Ps2GprU32(ctx, 4)); }},
    {"kernel_sub_001D3A48", 0x1d3a48u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { kernel_sub_001D3A48(rdram, ctx, runtime, (uint64_t)Ps2GprU64(ctx, 4)); }},
    {"Sg2SlotFree", 0x1d5d28u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { Sg2SlotFree(rdram, ctx, runtime, (uint32_t)Ps2GprU32(ctx, 4)); }},
    {"Sg2SlotSetGroup", 0x1d6250u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { Sg2SlotSetGroup(rdram, ctx, runtime, (uint32_t)Ps2GprU32(ctx, 4), (uint32_t)Ps2GprU32(ctx, 5)); }},
    {"Sg2SlotSetMute", 0x1d6270u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { Sg2SlotSetMute(rdram, ctx, runtime, (uint64_t)Ps2GprU64(ctx, 4), (uint64_t)Ps2GprU64(ctx, 5)); }},
    {"Sg2SlotSetBendRange", 0x1d6528u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { Sg2SlotSetBendRange(rdram, ctx, runtime, (uint32_t)Ps2GprU32(ctx, 4), (uint32_t)Ps2GprU32(ctx, 5)); }},
    {"Sg2SlotSetBendValue", 0x1d6548u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { Sg2SlotSetBendValue(rdram, ctx, runtime, (uint32_t)Ps2GprU32(ctx, 4), (uint32_t)Ps2GprU32(ctx, 5)); }},
    {"Sg2SlotSetPause", 0x1d65f0u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { Sg2SlotSetPause(rdram, ctx, runtime, (uint64_t)Ps2GprU64(ctx, 4), (uint64_t)Ps2GprU64(ctx, 5)); }},
    {"Sg2SlotSetDirectPitch", 0x1d6670u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { Sg2SlotSetDirectPitch(rdram, ctx, runtime, (uint32_t)Ps2GprU32(ctx, 4), (uint32_t)Ps2GprU32(ctx, 5)); }},
    {"Sg2SlotSetLoop", 0x1d6758u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { Sg2SlotSetLoop(rdram, ctx, runtime, (uint32_t)Ps2GprU32(ctx, 4)); }},
    {"Sg2SlotSetDumperOn", 0x1d6780u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { Sg2SlotSetDumperOn(rdram, ctx, runtime, (uint32_t)Ps2GprU32(ctx, 4)); }},
    {"Sg2SlotSetLfoVolTbl", 0x1d6950u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { Sg2SlotSetLfoVolTbl(rdram, ctx, runtime, (uint32_t)Ps2GprU32(ctx, 4), (uint64_t)Ps2GprU64(ctx, 5)); }},
    {"Sg2SlotSetTimeVolParam", 0x1d6c30u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { Sg2SlotSetTimeVolParam(rdram, ctx, runtime, (uint32_t)Ps2GprU32(ctx, 4), ctx->f[12], ctx->f[13]); }},
    {"Sg2SlotSetTimePitchParam", 0x1d6c70u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { Sg2SlotSetTimePitchParam(rdram, ctx, runtime, (uint32_t)Ps2GprU32(ctx, 4), ctx->f[12], ctx->f[13]); }},
    {"Sg2SlotSetTimePanParam", 0x1d6cb0u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { Sg2SlotSetTimePanParam(rdram, ctx, runtime, (uint32_t)Ps2GprU32(ctx, 4), ctx->f[12], ctx->f[13]); }},
    {"Sg2SlotGetVol", 0x1d6d50u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { Sg2SlotGetVol(rdram, ctx, runtime, (uint32_t)Ps2GprU32(ctx, 4)); }},
    {"Sg2SlotGetPan", 0x1d6d70u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { Sg2SlotGetPan(rdram, ctx, runtime, (uint32_t)Ps2GprU32(ctx, 4)); }},
    {"Sg2SlotSetOutputMode", 0x1d7458u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { Sg2SlotSetOutputMode(rdram, ctx, runtime, (uint32_t)Ps2GprU32(ctx, 4)); }},
    {"Sg2SlotSetVolShiftDownParam", 0x1d7470u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { Sg2SlotSetVolShiftDownParam(rdram, ctx, runtime, (uint32_t)Ps2GprU32(ctx, 4)); }},
    {"Sg2SlotGetVolShiftDownParam", 0x1d7488u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { Sg2SlotGetVolShiftDownParam(rdram, ctx, runtime); }},
    {"kernel_sub_001D8250", 0x1d8250u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { kernel_sub_001D8250(rdram, ctx, runtime, (uint32_t)Ps2GprU32(ctx, 4)); }},
    {"Sg2SmfpStatus", 0x1d88f8u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { Sg2SmfpStatus(rdram, ctx, runtime, (uint64_t)Ps2GprU64(ctx, 4)); }},
    {"Sg2SmfpSetSignal", 0x1d8e78u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { Sg2SmfpSetSignal(rdram, ctx, runtime, (uint32_t)Ps2GprU32(ctx, 4)); }},
    {"kernel_sub_001D95C8", 0x1d95c8u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { kernel_sub_001D95C8(rdram, ctx, runtime); }},
    {"kernel_sub_001D95D0", 0x1d95d0u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { kernel_sub_001D95D0(rdram, ctx, runtime, (uint32_t)Ps2GprU32(ctx, 4)); }},
    {"kernel_sub_001D97F0", 0x1d97f0u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { kernel_sub_001D97F0(rdram, ctx, runtime, (uint32_t)Ps2GprU32(ctx, 4)); }},
    {"vab_check_alive", 0x1da0a0u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { vab_check_alive(rdram, ctx, runtime, (uint64_t)Ps2GprU64(ctx, 4)); }},
    {"vab_get_info", 0x1da0e0u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { vab_get_info(rdram, ctx, runtime, (uint32_t)Ps2GprU32(ctx, 4), (uint32_t)Ps2GprU32(ctx, 5), (uint32_t)Ps2GprU32(ctx, 6)); }},
    {"vab_get_veltbl", 0x1da348u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { vab_get_veltbl(rdram, ctx, runtime, (uint32_t)Ps2GprU32(ctx, 4), (uint32_t)Ps2GprU32(ctx, 5)); }},
    {"vab_get_lfotbl_param", 0x1da380u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { vab_get_lfotbl_param(rdram, ctx, runtime, (uint32_t)Ps2GprU32(ctx, 4), (uint32_t)Ps2GprU32(ctx, 5), (uint32_t)Ps2GprU32(ctx, 6)); }},
    {"vab_set_semastervol", 0x1da3c8u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { vab_set_semastervol(rdram, ctx, runtime, (uint32_t)Ps2GprU32(ctx, 4), (uint32_t)Ps2GprU32(ctx, 5)); }},
    {"vab_get_semastervol", 0x1da3f8u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { vab_get_semastervol(rdram, ctx, runtime, (uint32_t)Ps2GprU32(ctx, 4)); }},
    {"kernel_sub_001DB3C8", 0x1db3c8u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { kernel_sub_001DB3C8(rdram, ctx, runtime, (uint32_t)Ps2GprU32(ctx, 4)); }},
    {"Sg2ExPcmInputGetLipFrame", 0x1db9e8u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { Sg2ExPcmInputGetLipFrame(rdram, ctx, runtime, (uint64_t)Ps2GprU64(ctx, 4)); }},
    {"Sg2ExPcmInputSetOutputMode", 0x1dbba0u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { Sg2ExPcmInputSetOutputMode(rdram, ctx, runtime, (uint32_t)Ps2GprU32(ctx, 5), (uint64_t)Ps2GprU64(ctx, 4)); }},
    {"Sg2ExPcmInputAutoFilterCtrl", 0x1dbd90u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { Sg2ExPcmInputAutoFilterCtrl(rdram, ctx, runtime, (uint32_t)Ps2GprU32(ctx, 5), (uint32_t)Ps2GprU32(ctx, 6), (uint64_t)Ps2GprU64(ctx, 4)); }},
    {"Sg2ExPcmOutputSetOutputMode", 0x1dbfb8u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { Sg2ExPcmOutputSetOutputMode(rdram, ctx, runtime, (uint32_t)Ps2GprU32(ctx, 4)); }},
    {"Sg2ExPcmOutputGetLipFrame", 0x1dc068u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { Sg2ExPcmOutputGetLipFrame(rdram, ctx, runtime); }},
    {"Sg2ExPcmOutputAutoFilterCtrl", 0x1dc1d0u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { Sg2ExPcmOutputAutoFilterCtrl(rdram, ctx, runtime, (uint32_t)Ps2GprU32(ctx, 4), (uint32_t)Ps2GprU32(ctx, 5)); }},
    {"Sg2ExPcmSetSignal", 0x1dc2d0u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { Sg2ExPcmSetSignal(rdram, ctx, runtime, (uint32_t)Ps2GprU32(ctx, 4)); }},
    {"IirInit", 0x1dca28u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { IirInit(rdram, ctx, runtime, (uint32_t)Ps2GprU32(ctx, 4)); }},
    {"GAMEUTIL_is_inside_of_region_pitch", 0x107d5c8u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { GAMEUTIL_is_inside_of_region_pitch(rdram, ctx, runtime, (uint64_t)Ps2GprU64(ctx, 4), (uint64_t)Ps2GprU64(ctx, 5), ctx->f[12], ctx->f[13], ctx->f[14]); }},
    {"GAMEUTIL_is_nearly_equal_zero", 0x107ec28u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { GAMEUTIL_is_nearly_equal_zero(rdram, ctx, runtime, ctx->f[12], ctx->f[13]); }},
    {"GAMEUTIL_is_nearly_equal_zero3D", 0x107ec70u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { GAMEUTIL_is_nearly_equal_zero3D(rdram, ctx, runtime, (uint32_t)Ps2GprU32(ctx, 4), ctx->f[12]); }},
    {"GAMEUTIL_get_answer_of_2jihouteishiki", 0x107ee48u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { GAMEUTIL_get_answer_of_2jihouteishiki(rdram, ctx, runtime, (uint32_t)Ps2GprU32(ctx, 4), (uint32_t)Ps2GprU32(ctx, 5), ctx->f[12], ctx->f[13], ctx->f[14]); }},
    {"GAMEUTIL_get_new_velocity_by_brake_abs", 0x107fb30u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { GAMEUTIL_get_new_velocity_by_brake_abs(rdram, ctx, runtime, ctx->f[12], ctx->f[13], ctx->f[14], ctx->f[15]); }},
    {"GAMEUTIL_get_velocity_by_diff_move", 0x107fb60u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { GAMEUTIL_get_velocity_by_diff_move(rdram, ctx, runtime, (uint64_t)Ps2GprU64(ctx, 4), ctx->f[12], ctx->f[13], ctx->f[14], ctx->f[15]); }},
    {"GAMEUTIL_mul_ratio", 0x107fed0u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { GAMEUTIL_mul_ratio(rdram, ctx, runtime, (uint64_t)Ps2GprU64(ctx, 4), ctx->f[12], ctx->f[13]); }},
    {"GAMEUTIL_convert_from_lw_to_game_coord", 0x10802f0u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { GAMEUTIL_convert_from_lw_to_game_coord(rdram, ctx, runtime, (uint32_t)Ps2GprU32(ctx, 4)); }},
    {"GAMEUTIL_get_difference_pitch_and_pitch", 0x1080b08u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { GAMEUTIL_get_difference_pitch_and_pitch(rdram, ctx, runtime, ctx->f[12], ctx->f[13]); }},
    {"GAMEUTIL_get_move_radi_from_yaw1_to_yaw2", 0x1080b28u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { GAMEUTIL_get_move_radi_from_yaw1_to_yaw2(rdram, ctx, runtime, ctx->f[12], ctx->f[13]); }},
    {"GAMEUTIL_get_move_radi_from_pitch1_to_pitch2", 0x1080b90u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { GAMEUTIL_get_move_radi_from_pitch1_to_pitch2(rdram, ctx, runtime, ctx->f[12], ctx->f[13]); }},
    {"GAMEUTIL_is_between_yaw_and_yaw", 0x1080b98u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { GAMEUTIL_is_between_yaw_and_yaw(rdram, ctx, runtime, ctx->f[12], ctx->f[13], ctx->f[14]); }},
    {"GAMEUTIL_is_between_pitch_and_pitch", 0x1080d58u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { GAMEUTIL_is_between_pitch_and_pitch(rdram, ctx, runtime, ctx->f[12], ctx->f[13], ctx->f[14]); }},
    {"GAMEUTIL_is_reach_aim_yaw_angle", 0x1080f50u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { GAMEUTIL_is_reach_aim_yaw_angle(rdram, ctx, runtime, (uint64_t)Ps2GprU64(ctx, 4), ctx->f[12], ctx->f[13], ctx->f[14]); }},
    {"GAMEUTIL_is_nearly_equal_yaw", 0x10829f8u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { GAMEUTIL_is_nearly_equal_yaw(rdram, ctx, runtime, ctx->f[12], ctx->f[13], ctx->f[14]); }},
    {"GAMEUTIL_is_nearly_equal_pitch", 0x1082a50u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { GAMEUTIL_is_nearly_equal_pitch(rdram, ctx, runtime, ctx->f[12], ctx->f[13], ctx->f[14]); }},
    {"manager_sub_01082C78", 0x1082c78u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { manager_sub_01082C78(rdram, ctx, runtime, ctx->f[12]); }},
    {"manager_sub_01082CD8", 0x1082cd8u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { manager_sub_01082CD8(rdram, ctx, runtime, ctx->f[12], ctx->f[13], ctx->f[14]); }},
    {"get_fast_get_malloc_partition", 0x1082da0u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { get_fast_get_malloc_partition(rdram, ctx, runtime); }},
    {"GAMEUTIL_doit_interp_ratio_cntrl", 0x10832a8u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { GAMEUTIL_doit_interp_ratio_cntrl(rdram, ctx, runtime, (uint32_t)Ps2GprU32(ctx, 4), ctx->f[12]); }},
    {"GAMEUTIL_get_dist_ratio", 0x10836c0u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { GAMEUTIL_get_dist_ratio(rdram, ctx, runtime, ctx->f[12], ctx->f[13], ctx->f[14]); }},
    {"GAMEUTIL_get_interp_value", 0x1083828u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { GAMEUTIL_get_interp_value(rdram, ctx, runtime, ctx->f[12], ctx->f[13], ctx->f[14]); }},
    {"GAMEUTIL_forward_timer_still_one_ratio_time_interp_ratio_cntrl", 0x10842d0u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { GAMEUTIL_forward_timer_still_one_ratio_time_interp_ratio_cntrl(rdram, ctx, runtime, (uint32_t)Ps2GprU32(ctx, 4)); }},
    {"GAMEUTIL_get_interp_ratio_interp_ratio_cntrl", 0x10842e8u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { GAMEUTIL_get_interp_ratio_interp_ratio_cntrl(rdram, ctx, runtime, (uint32_t)Ps2GprU32(ctx, 4)); }},
    {"GAMEUTIL_get_interp_timer_interp_ratio_cntrl", 0x10842f0u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { GAMEUTIL_get_interp_timer_interp_ratio_cntrl(rdram, ctx, runtime, (uint32_t)Ps2GprU32(ctx, 4)); }},
    {"GAMEUTIL_set_act_interp_flag_interp_ratio_cntrl", 0x10842f8u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { GAMEUTIL_set_act_interp_flag_interp_ratio_cntrl(rdram, ctx, runtime, (uint32_t)Ps2GprU32(ctx, 4), (uint32_t)Ps2GprU32(ctx, 5)); }},
    {"GAMEUTIL_set_act_max_zero_clip_flag_interp_ratio_cntrl", 0x1084300u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { GAMEUTIL_set_act_max_zero_clip_flag_interp_ratio_cntrl(rdram, ctx, runtime, (uint32_t)Ps2GprU32(ctx, 4), (uint32_t)Ps2GprU32(ctx, 5)); }},
    {"GAMEUTIL_get_interp_value_interp_value_cntrl", 0x1084500u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { GAMEUTIL_get_interp_value_interp_value_cntrl(rdram, ctx, runtime, (uint32_t)Ps2GprU32(ctx, 4)); }},
    {"GAMEUTIL_get_hope_value_interp_value_cntrl", 0x1084508u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { GAMEUTIL_get_hope_value_interp_value_cntrl(rdram, ctx, runtime, (uint32_t)Ps2GprU32(ctx, 4)); }},
    {"set_param_double_spring_interp_common", 0x1084510u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { set_param_double_spring_interp_common(rdram, ctx, runtime, (uint32_t)Ps2GprU32(ctx, 4), ctx->f[12], ctx->f[13], ctx->f[14], ctx->f[15]); }},
    {"set_rot_radi_value_flag_double_spring_interp_cntrl", 0x1084588u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { set_rot_radi_value_flag_double_spring_interp_cntrl(rdram, ctx, runtime, (uint32_t)Ps2GprU32(ctx, 4), (uint32_t)Ps2GprU32(ctx, 5)); }},
    {"set_rot_radi_value_flag_double_spring_interp_cntrl3D", 0x1084698u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { set_rot_radi_value_flag_double_spring_interp_cntrl3D(rdram, ctx, runtime, (uint32_t)Ps2GprU32(ctx, 4), (uint32_t)Ps2GprU32(ctx, 5), (uint32_t)Ps2GprU32(ctx, 6), (uint32_t)Ps2GprU32(ctx, 7)); }},
    {"constructor_aim_angle_interp_cntrl", 0x1084880u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { constructor_aim_angle_interp_cntrl(rdram, ctx, runtime, (uint32_t)Ps2GprU32(ctx, 4), (uint32_t)Ps2GprU32(ctx, 5)); }},
    {"reset_aim_angle_interp_cntrl", 0x10848a8u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { reset_aim_angle_interp_cntrl(rdram, ctx, runtime, (uint32_t)Ps2GprU32(ctx, 4), ctx->f[12]); }},
    {"set_interp_param_aim_angle_interp_cntrl", 0x1084968u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { set_interp_param_aim_angle_interp_cntrl(rdram, ctx, runtime, (uint32_t)Ps2GprU32(ctx, 4), ctx->f[12], ctx->f[13]); }},
    {"manager_sub_01084978", 0x1084978u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { manager_sub_01084978(rdram, ctx, runtime, (uint32_t)Ps2GprU32(ctx, 4), ctx->f[12], ctx->f[13]); }},
    {"getNowBossEffect", 0x1087108u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { getNowBossEffect(rdram, ctx, runtime); }},
    {"setBossDeathEffect", 0x1087118u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { setBossDeathEffect(rdram, ctx, runtime, (uint64_t)Ps2GprU64(ctx, 4), (uint64_t)Ps2GprU64(ctx, 5), ctx->f[12]); }},
    {"getBossEffectBlurLevel", 0x1087200u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { getBossEffectBlurLevel(rdram, ctx, runtime); }},
    {"getBossEffectColorR", 0x1087210u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { getBossEffectColorR(rdram, ctx, runtime); }},
    {"getBossEffectColorG", 0x1087220u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { getBossEffectColorG(rdram, ctx, runtime); }},
    {"getBossEffectColorB", 0x1087230u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { getBossEffectColorB(rdram, ctx, runtime); }},
    {"getBossEffectBlendRatio", 0x1087240u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { getBossEffectBlendRatio(rdram, ctx, runtime); }},
    {"getBossEffectOutLine", 0x1087250u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { getBossEffectOutLine(rdram, ctx, runtime); }},
    {"getDepthOfFieldSw", 0x1088190u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { getDepthOfFieldSw(rdram, ctx, runtime); }},
    {"getDepthOfFieldLevel", 0x10881a0u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { getDepthOfFieldLevel(rdram, ctx, runtime); }},
    {"getDepthOfFieldPlanes", 0x10881b0u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { getDepthOfFieldPlanes(rdram, ctx, runtime); }},
    {"getDepthOfFieldNearZ", 0x10881c0u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { getDepthOfFieldNearZ(rdram, ctx, runtime); }},
    {"fsEffectSetParamSheetSeg", 0x1088a78u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { fsEffectSetParamSheetSeg(rdram, ctx, runtime, (uint64_t)Ps2GprU64(ctx, 4)); }},
    {"getFogMode", 0x10898f8u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { getFogMode(rdram, ctx, runtime); }},
    {"getFogNearDist", 0x1089908u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { getFogNearDist(rdram, ctx, runtime); }},
    {"getFogFarDist", 0x1089918u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { getFogFarDist(rdram, ctx, runtime); }},
    {"getFogNearRate", 0x1089928u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { getFogNearRate(rdram, ctx, runtime); }},
    {"getFogFarRate", 0x1089938u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { getFogFarRate(rdram, ctx, runtime); }},
    {"getFogOffset", 0x1089958u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { getFogOffset(rdram, ctx, runtime); }},
    {"getFogUnderWater", 0x1089968u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { getFogUnderWater(rdram, ctx, runtime); }},
    {"getFogColorR", 0x1089978u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { getFogColorR(rdram, ctx, runtime); }},
    {"getFogColorG", 0x1089988u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { getFogColorG(rdram, ctx, runtime); }},
    {"getFogColorB", 0x1089998u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { getFogColorB(rdram, ctx, runtime); }},
    {"SeemlessInitFogTargetParamWorldLayout", 0x1089a38u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { SeemlessInitFogTargetParamWorldLayout(rdram, ctx, runtime, (uint32_t)Ps2GprU32(ctx, 4)); }},
    {"fogSetFogColorMulRatio", 0x1089b60u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { fogSetFogColorMulRatio(rdram, ctx, runtime, ctx->f[12]); }},
    {"getFogFactor1", 0x1089bb0u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { getFogFactor1(rdram, ctx, runtime); }},
    {"setFadeColor", 0x108b970u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { setFadeColor(rdram, ctx, runtime, (uint64_t)Ps2GprU64(ctx, 4)); }},
    {"setFadeLayer", 0x108b9b8u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { setFadeLayer(rdram, ctx, runtime, (uint32_t)Ps2GprU32(ctx, 4)); }},
    {"getCameraBlurSw", 0x108ba28u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { getCameraBlurSw(rdram, ctx, runtime); }},
    {"getBlurAlsoChara", 0x108ba38u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { getBlurAlsoChara(rdram, ctx, runtime); }},
    {"getBlurWorkSize", 0x108ba48u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { getBlurWorkSize(rdram, ctx, runtime); }},
    {"getNBlurPlanes", 0x108ba58u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { getNBlurPlanes(rdram, ctx, runtime); }},
    {"getBlurIntensity", 0x108ba68u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { getBlurIntensity(rdram, ctx, runtime); }},
    {"getBlurZBase", 0x108ba78u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { getBlurZBase(rdram, ctx, runtime); }},
    {"getBlurMinThreshold", 0x108ba88u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { getBlurMinThreshold(rdram, ctx, runtime); }},
    {"getBlurMaxThreshold", 0x108ba98u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { getBlurMaxThreshold(rdram, ctx, runtime); }},
    {"getBlurShutterSpeed", 0x108baa8u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { getBlurShutterSpeed(rdram, ctx, runtime); }},
    {"getBlurXYZRatio", 0x108bab8u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { getBlurXYZRatio(rdram, ctx, runtime); }},
    {"getBlurCamMoveThreshold", 0x108bac8u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { getBlurCamMoveThreshold(rdram, ctx, runtime); }},
    {"glTexCoord2f", 0x108e9c8u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { glTexCoord2f(rdram, ctx, runtime, ctx->f[12], ctx->f[13]); }},
    {"glTexCoord2fv", 0x108e9f0u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { glTexCoord2fv(rdram, ctx, runtime, (uint32_t)Ps2GprU32(ctx, 4)); }},
    {"glZOffset", 0x108f3b0u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { glZOffset(rdram, ctx, runtime, ctx->f[12]); }},
    {"glSetFogFactorDirect", 0x108f608u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { glSetFogFactorDirect(rdram, ctx, runtime, (uint32_t)Ps2GprU32(ctx, 4)); }},
    {"gluGetBrightness", 0x108f7e0u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { gluGetBrightness(rdram, ctx, runtime, (uint32_t)Ps2GprU32(ctx, 4)); }},
    {"getMBlur", 0x108fba8u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { getMBlur(rdram, ctx, runtime); }},
    {"getBrightness", 0x108fbc8u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { getBrightness(rdram, ctx, runtime); }},
    {"kSetTex", 0x10916f8u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { kSetTex(rdram, ctx, runtime, (uint32_t)Ps2GprU32(ctx, 4)); }},
    {"getSpecularity", 0x1092470u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { getSpecularity(rdram, ctx, runtime); }},
    {"modelDeleteDispModelClass", 0x1099590u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { modelDeleteDispModelClass(rdram, ctx, runtime, (uint64_t)Ps2GprU64(ctx, 4)); }},
    {"modelGetWorkSize", 0x1099658u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { modelGetWorkSize(rdram, ctx, runtime, (uint32_t)Ps2GprU32(ctx, 4)); }},
    {"SetDisplayModelNearClippingSwitch", 0x1099758u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { SetDisplayModelNearClippingSwitch(rdram, ctx, runtime, (uint32_t)Ps2GprU32(ctx, 4)); }},
    {"ptclmanGet", 0x109a3d8u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { ptclmanGet(rdram, ctx, runtime); }},
    {"ptclmanPut", 0x109a3f8u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { ptclmanPut(rdram, ctx, runtime, (uint32_t)Ps2GprU32(ctx, 4)); }},
    {"ptclCounterGet", 0x109a4d0u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { ptclCounterGet(rdram, ctx, runtime); }},
    {"getRefBlurLevel", 0x109be88u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { getRefBlurLevel(rdram, ctx, runtime); }},
    {"getIsInWater", 0x109d0d8u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { getIsInWater(rdram, ctx, runtime); }},
    {"getRefractBlurSpeed", 0x109d0e8u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { getRefractBlurSpeed(rdram, ctx, runtime); }},
    {"getRefractBlurSpeedRand", 0x109d0f8u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { getRefractBlurSpeedRand(rdram, ctx, runtime); }},
    {"getRefractBlurLevel", 0x109d108u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { getRefractBlurLevel(rdram, ctx, runtime); }},
    {"getRefractBlurLevelRand", 0x109d118u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { getRefractBlurLevelRand(rdram, ctx, runtime); }},
    {"getRefractLengthRatio", 0x109d128u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { getRefractLengthRatio(rdram, ctx, runtime); }},
    {"getRefractBlendRatio", 0x109d138u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { getRefractBlendRatio(rdram, ctx, runtime); }},
    {"getRefractBrightness", 0x109d148u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { getRefractBrightness(rdram, ctx, runtime); }},
    {"getShadowIntensity", 0x109de78u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { getShadowIntensity(rdram, ctx, runtime); }},
    {"GetShadowLength", 0x109dea8u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { GetShadowLength(rdram, ctx, runtime, (uint32_t)Ps2GprU32(ctx, 4)); }},
    {"shadowModelReInit", 0x109df30u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { shadowModelReInit(rdram, ctx, runtime, (uint64_t)Ps2GprU64(ctx, 4)); }},
    {"getSolarFlareSw", 0x10a1728u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { getSolarFlareSw(rdram, ctx, runtime); }},
    {"getSolarFlareBlur", 0x10a1738u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { getSolarFlareBlur(rdram, ctx, runtime); }},
    {"getSolarFlareColorR", 0x10a1748u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { getSolarFlareColorR(rdram, ctx, runtime); }},
    {"getSolarFlareColorG", 0x10a1758u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { getSolarFlareColorG(rdram, ctx, runtime); }},
    {"getSolarFlareColorB", 0x10a1768u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { getSolarFlareColorB(rdram, ctx, runtime); }},
    {"getSolarFlareSolarLevel", 0x10a1778u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { getSolarFlareSolarLevel(rdram, ctx, runtime); }},
    {"getSolarGlowLevel", 0x10a1788u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { getSolarGlowLevel(rdram, ctx, runtime); }},
    {"getSolarFlareSamplePix", 0x10a1798u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { getSolarFlareSamplePix(rdram, ctx, runtime); }},
    {"manager_sub_010A2040", 0x10a2040u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { manager_sub_010A2040(rdram, ctx, runtime, (uint32_t)Ps2GprU32(ctx, 4)); }},
    {"spriteSetPosition", 0x10a52d0u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { spriteSetPosition(rdram, ctx, runtime, (uint32_t)Ps2GprU32(ctx, 4), ctx->f[12], ctx->f[13]); }},
    {"spriteSetTextureCoordinateSTFromUV", 0x10a5310u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { spriteSetTextureCoordinateSTFromUV(rdram, ctx, runtime, (uint64_t)Ps2GprU64(ctx, 4), ctx->f[12], ctx->f[13], ctx->f[14], ctx->f[15]); }},
    {"spriteSetTextureCoordinateUV", 0x10a53b8u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { spriteSetTextureCoordinateUV(rdram, ctx, runtime, (uint64_t)Ps2GprU64(ctx, 4), ctx->f[12], ctx->f[13], ctx->f[14], ctx->f[15]); }},
    {"spriteSetColor", 0x10a5410u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { spriteSetColor(rdram, ctx, runtime, (uint32_t)Ps2GprU32(ctx, 4), (uint32_t)Ps2GprU32(ctx, 5), (uint32_t)Ps2GprU32(ctx, 6), (uint32_t)Ps2GprU32(ctx, 7), (uint32_t)Ps2GprU32(ctx, 8)); }},
    {"spriteSetScale", 0x10a5428u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { spriteSetScale(rdram, ctx, runtime, (uint32_t)Ps2GprU32(ctx, 4), ctx->f[12], ctx->f[13]); }},
    {"spriteSetRotation", 0x10a5438u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { spriteSetRotation(rdram, ctx, runtime, (uint32_t)Ps2GprU32(ctx, 4), ctx->f[12]); }},
    {"spriteSetFade", 0x10a54d0u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { spriteSetFade(rdram, ctx, runtime, (uint32_t)Ps2GprU32(ctx, 4), ctx->f[12]); }},
    {"spriteSetPriority", 0x10a54d8u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { spriteSetPriority(rdram, ctx, runtime, (uint32_t)Ps2GprU32(ctx, 4), (uint32_t)Ps2GprU32(ctx, 5)); }},
    {"manager_sub_010ABC48", 0x10abc48u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { manager_sub_010ABC48(rdram, ctx, runtime, (uint64_t)Ps2GprU64(ctx, 4)); }},
    {"getZFogDivide", 0x10ac400u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { getZFogDivide(rdram, ctx, runtime); }},
    {"getZFogScissorMargin", 0x10ac440u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { getZFogScissorMargin(rdram, ctx, runtime); }},
    {"setZFogFar", 0x10ac460u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { setZFogFar(rdram, ctx, runtime, ctx->f[12]); }},
    {"getZFogNear", 0x10ac478u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { getZFogNear(rdram, ctx, runtime); }},
    {"GetObjectAnimationParentNodeId", 0x10b6788u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { GetObjectAnimationParentNodeId(rdram, ctx, runtime, (uint32_t)Ps2GprU32(ctx, 4), (uint32_t)Ps2GprU32(ctx, 5)); }},
    {"IsFullFixFormat", 0x10b74c0u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { IsFullFixFormat(rdram, ctx, runtime, (uint64_t)Ps2GprU64(ctx, 4)); }},
    {"GetMotionResourceRotAngle", 0x10b7650u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { GetMotionResourceRotAngle(rdram, ctx, runtime, (uint64_t)Ps2GprU64(ctx, 4)); }},
    {"constructor_motion_ik_base", 0x10b92a8u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { constructor_motion_ik_base(rdram, ctx, runtime, (uint32_t)Ps2GprU32(ctx, 4)); }},
    {"init_motion_ik_base", 0x10b92b0u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { init_motion_ik_base(rdram, ctx, runtime, (uint32_t)Ps2GprU32(ctx, 4)); }},
    {"doit_motion_ik_base", 0x10b92b8u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { doit_motion_ik_base(rdram, ctx, runtime, (uint32_t)Ps2GprU32(ctx, 4), (uint32_t)Ps2GprU32(ctx, 5)); }},
    {"manager_sub_010BA140", 0x10ba140u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { manager_sub_010BA140(rdram, ctx, runtime, (uint32_t)Ps2GprU32(ctx, 4)); }},
    {"UpdateAnimationClassPastSimpleInfo", 0x10ba9d8u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { UpdateAnimationClassPastSimpleInfo(rdram, ctx, runtime, (uint32_t)Ps2GprU32(ctx, 4)); }},
    {"GetCharacterClassInitParamNbOptionNodes", 0x10c0c78u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { GetCharacterClassInitParamNbOptionNodes(rdram, ctx, runtime, (uint32_t)Ps2GprU32(ctx, 4)); }},
    {"ResetCharacterClassWaterStatus", 0x10c11b8u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { ResetCharacterClassWaterStatus(rdram, ctx, runtime, (uint32_t)Ps2GprU32(ctx, 4)); }},
    {"ResetCharacterClassMotInterpInfo", 0x10c17b0u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { ResetCharacterClassMotInterpInfo(rdram, ctx, runtime, (uint32_t)Ps2GprU32(ctx, 4)); }},
    {"GetCharacterClassCurAnimationFrameRate", 0x10c1a08u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { GetCharacterClassCurAnimationFrameRate(rdram, ctx, runtime, (uint32_t)Ps2GprU32(ctx, 4)); }},
    {"SetCharacterClassGazeAdditionChannelInfo", 0x10c66f0u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { SetCharacterClassGazeAdditionChannelInfo(rdram, ctx, runtime, (uint32_t)Ps2GprU32(ctx, 4), (uint32_t)Ps2GprU32(ctx, 5), ctx->f[12], ctx->f[13]); }},
    {"ForceAcceptMotionControlChange", 0x10cc6c8u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { ForceAcceptMotionControlChange(rdram, ctx, runtime, (uint32_t)Ps2GprU32(ctx, 4), (uint64_t)Ps2GprU64(ctx, 5)); }},
    {"YattsukeSetPlayerR1ON", 0x10cc778u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { YattsukeSetPlayerR1ON(rdram, ctx, runtime, (uint32_t)Ps2GprU32(ctx, 4)); }},
    {"RequestCharacterClassSlip", 0x10d2a88u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { RequestCharacterClassSlip(rdram, ctx, runtime, (uint32_t)Ps2GprU32(ctx, 4)); }},
    {"RequestCharacterClassNoClimbableSlip", 0x10d2aa0u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { RequestCharacterClassNoClimbableSlip(rdram, ctx, runtime, (uint32_t)Ps2GprU32(ctx, 4)); }},
    {"RequestCharacterClassAdditionMotionChange", 0x10d2ae0u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { RequestCharacterClassAdditionMotionChange(rdram, ctx, runtime, (uint32_t)Ps2GprU32(ctx, 4), (uint32_t)Ps2GprU32(ctx, 5)); }},
    {"RequestCharacterClassMotionDirectionByGPTop", 0x10d2b60u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { RequestCharacterClassMotionDirectionByGPTop(rdram, ctx, runtime, (uint32_t)Ps2GprU32(ctx, 4)); }},
    {"RequestCharacterClassMotionDirectionByGPTail", 0x10d2b70u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { RequestCharacterClassMotionDirectionByGPTail(rdram, ctx, runtime, (uint32_t)Ps2GprU32(ctx, 4)); }},
    {"RequestCharacterClassMotionTurnLevel", 0x10d2b90u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { RequestCharacterClassMotionTurnLevel(rdram, ctx, runtime, (uint32_t)Ps2GprU32(ctx, 4), ctx->f[12]); }},
    {"RequestCharacterClassMotionPlaySpeedUp", 0x10d2bb0u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { RequestCharacterClassMotionPlaySpeedUp(rdram, ctx, runtime, (uint32_t)Ps2GprU32(ctx, 4)); }},
    {"RequestCharacterClassMotionPlaySpeedDown", 0x10d2bc0u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { RequestCharacterClassMotionPlaySpeedDown(rdram, ctx, runtime, (uint32_t)Ps2GprU32(ctx, 4)); }},
    {"RequestCharacterClassMotionPlaySpeedDown2", 0x10d2bd0u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { RequestCharacterClassMotionPlaySpeedDown2(rdram, ctx, runtime, (uint32_t)Ps2GprU32(ctx, 4)); }},
    {"RequestCharacterClassWindForceMove", 0x10d2be0u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { RequestCharacterClassWindForceMove(rdram, ctx, runtime, (uint32_t)Ps2GprU32(ctx, 4)); }},
    {"getTurnLimitInterp", 0x10d2ce8u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { getTurnLimitInterp(rdram, ctx, runtime, ctx->f[12], ctx->f[13], ctx->f[14], ctx->f[15]); }},
    {"getRatioFromInterp", 0x10d2d48u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { getRatioFromInterp(rdram, ctx, runtime, ctx->f[12], ctx->f[13]); }},
    {"SetCharacterObjMotionJumpSpeedLimit", 0x10d37a8u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { SetCharacterObjMotionJumpSpeedLimit(rdram, ctx, runtime, (uint32_t)Ps2GprU32(ctx, 5)); }},
    {"charrootdyn_getUpperEdgeEffectiveHeight", 0x10dc3f0u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { charrootdyn_getUpperEdgeEffectiveHeight(rdram, ctx, runtime, (uint32_t)Ps2GprU32(ctx, 4)); }},
    {"charrootdyn_getNoLimitMode", 0x10de368u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { charrootdyn_getNoLimitMode(rdram, ctx, runtime, (uint32_t)Ps2GprU32(ctx, 4), (uint32_t)Ps2GprU32(ctx, 5)); }},
    {"charrootdyn_getForceNoLimitMode", 0x10de380u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { charrootdyn_getForceNoLimitMode(rdram, ctx, runtime, (uint32_t)Ps2GprU32(ctx, 4)); }},
    {"charrootdyn_isParentToEdge", 0x10df130u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { charrootdyn_isParentToEdge(rdram, ctx, runtime, (uint32_t)Ps2GprU32(ctx, 4)); }},
    {"manager_sub_010DFD18", 0x10dfd18u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { manager_sub_010DFD18(rdram, ctx, runtime, (uint32_t)Ps2GprU32(ctx, 6), (uint64_t)Ps2GprU64(ctx, 5)); }},
    {"SetInitFogColClassFromStageData", 0x10ed358u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { SetInitFogColClassFromStageData(rdram, ctx, runtime, (uint32_t)Ps2GprU32(ctx, 4)); }},
    {"SetFogColAvailableFlag", 0x10ed520u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { SetFogColAvailableFlag(rdram, ctx, runtime, (uint32_t)Ps2GprU32(ctx, 4), (uint32_t)Ps2GprU32(ctx, 5)); }},
    {"IsEnvelopeFogType", 0x10ed528u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { IsEnvelopeFogType(rdram, ctx, runtime, (uint32_t)Ps2GprU32(ctx, 4)); }},
    {"GetJudgeVolumeClassWorkSize", 0x10ee768u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { GetJudgeVolumeClassWorkSize(rdram, ctx, runtime, (uint64_t)Ps2GprU64(ctx, 4)); }},
    {"GetJudgeVolumeClassObjectGroup", 0x10ee808u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { GetJudgeVolumeClassObjectGroup(rdram, ctx, runtime, (uint32_t)Ps2GprU32(ctx, 4), (uint32_t)Ps2GprU32(ctx, 5)); }},
    {"GetJudgeVolumeClassObjectGroupById", 0x10ee830u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { GetJudgeVolumeClassObjectGroupById(rdram, ctx, runtime, (uint32_t)Ps2GprU32(ctx, 4), (uint64_t)Ps2GprU64(ctx, 5)); }},
    {"SetLayoutClassCollisionWorkAdr", 0x10f31b8u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { SetLayoutClassCollisionWorkAdr(rdram, ctx, runtime, (uint32_t)Ps2GprU32(ctx, 4), (uint32_t)Ps2GprU32(ctx, 5)); }},
    {"manager_sub_010F3470", 0x10f3470u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { manager_sub_010F3470(rdram, ctx, runtime, (uint32_t)Ps2GprU32(ctx, 4)); }},
    {"manager_sub_010F3668", 0x10f3668u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { manager_sub_010F3668(rdram, ctx, runtime, (uint32_t)Ps2GprU32(ctx, 4)); }},
    {"SetCameraResetSw", 0x10f5810u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { SetCameraResetSw(rdram, ctx, runtime, (uint32_t)Ps2GprU32(ctx, 4)); }},
    {"GetResetCameraFlagForDisplay", 0x10f5af8u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { GetResetCameraFlagForDisplay(rdram, ctx, runtime); }},
    {"GetLightColMan", 0x10f6358u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { GetLightColMan(rdram, ctx, runtime); }},
    {"TaskExecLightColMan", 0x10f6408u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { TaskExecLightColMan(rdram, ctx, runtime); }},
    {"SetGameLightAvailable", 0x10f7508u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { SetGameLightAvailable(rdram, ctx, runtime, (uint32_t)Ps2GprU32(ctx, 4), (uint32_t)Ps2GprU32(ctx, 5)); }},
    {"ForceGameLightFlagEnvelope", 0x10f7658u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { ForceGameLightFlagEnvelope(rdram, ctx, runtime, (uint32_t)Ps2GprU32(ctx, 4)); }},
    {"IsEnvelopeGameLightType", 0x10f7678u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { IsEnvelopeGameLightType(rdram, ctx, runtime, (uint32_t)Ps2GprU32(ctx, 4)); }},
    {"SetGameLightUserParam", 0x10f78a8u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { SetGameLightUserParam(rdram, ctx, runtime, (uint32_t)Ps2GprU32(ctx, 4), (uint32_t)Ps2GprU32(ctx, 5)); }},
    {"SetGameLightUserFunc", 0x10f78b0u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { SetGameLightUserFunc(rdram, ctx, runtime, (uint32_t)Ps2GprU32(ctx, 4), (uint32_t)Ps2GprU32(ctx, 5)); }},
    {"SetGlobalLightAvailable", 0x10f8b40u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { SetGlobalLightAvailable(rdram, ctx, runtime, (uint32_t)Ps2GprU32(ctx, 5), (uint64_t)Ps2GprU64(ctx, 4)); }},
    {"ForceGlobalLightFlagEnvelope", 0x10f8c90u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { ForceGlobalLightFlagEnvelope(rdram, ctx, runtime, (uint32_t)Ps2GprU32(ctx, 4)); }},
    {"IsEnvelopeGlobalLightType", 0x10f8cb0u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { IsEnvelopeGlobalLightType(rdram, ctx, runtime, (uint32_t)Ps2GprU32(ctx, 4)); }},
    {"SetLwoDemoOrientManagerObjCutFinishFlag", 0x10f9b58u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { SetLwoDemoOrientManagerObjCutFinishFlag(rdram, ctx, runtime, (uint32_t)Ps2GprU32(ctx, 4), (uint32_t)Ps2GprU32(ctx, 5)); }},
    {"GetLayoutObjGroup", 0x11008a0u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { GetLayoutObjGroup(rdram, ctx, runtime); }},
    {"GetLayoutDispObjGroup", 0x11008b0u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { GetLayoutDispObjGroup(rdram, ctx, runtime); }},
    {"GetSkeletonFocusNodeDirect", 0x1100c48u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { GetSkeletonFocusNodeDirect(rdram, ctx, runtime, (uint64_t)Ps2GprU64(ctx, 4)); }},
    {"AddObjectManagerSheetWorkSize", 0x11016c0u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { AddObjectManagerSheetWorkSize(rdram, ctx, runtime, (uint32_t)Ps2GprU32(ctx, 4)); }},
    {"GetAmbientLightObjGroup", 0x1101718u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { GetAmbientLightObjGroup(rdram, ctx, runtime); }},
    {"GetScriptPriorityGroup", 0x1102b00u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { GetScriptPriorityGroup(rdram, ctx, runtime, (uint32_t)Ps2GprU32(ctx, 4)); }},
    {"manager_sub_01103080", 0x1103080u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { manager_sub_01103080(rdram, ctx, runtime, (uint64_t)Ps2GprU64(ctx, 4)); }},
    {"GetScriptAnimationGlobalBaseInfo", 0x110a6d8u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { GetScriptAnimationGlobalBaseInfo(rdram, ctx, runtime); }},
    {"CopyScriptAnimationWindParam", 0x110a790u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { CopyScriptAnimationWindParam(rdram, ctx, runtime, (uint32_t)Ps2GprU32(ctx, 4), (uint32_t)Ps2GprU32(ctx, 5)); }},
    {"IsNonISysObjTypeRes", 0x110b818u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { IsNonISysObjTypeRes(rdram, ctx, runtime, (uint64_t)Ps2GprU64(ctx, 4)); }},
    {"SetScriptCharDefaultScriptFunc", 0x110cc68u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { SetScriptCharDefaultScriptFunc(rdram, ctx, runtime, (uint32_t)Ps2GprU32(ctx, 4)); }},
    {"InitSortObj", 0x110de28u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { InitSortObj(rdram, ctx, runtime, (uint32_t)Ps2GprU32(ctx, 4), (uint32_t)Ps2GprU32(ctx, 5)); }},
    {"setDefaultMenuControlButton", 0x1113ae8u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { setDefaultMenuControlButton(rdram, ctx, runtime); }},
    {"InitViewIntervalClip", 0x1114af0u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { InitViewIntervalClip(rdram, ctx, runtime, (uint32_t)Ps2GprU32(ctx, 4)); }},
    {"CopyCollisionEdge", 0x11280e0u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { CopyCollisionEdge(rdram, ctx, runtime, (uint32_t)Ps2GprU32(ctx, 4), (uint32_t)Ps2GprU32(ctx, 5)); }},
    {"MakeCollisionEdgePointer", 0x1128270u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { MakeCollisionEdgePointer(rdram, ctx, runtime, (uint32_t)Ps2GprU32(ctx, 4), (uint32_t)Ps2GprU32(ctx, 5), (uint32_t)Ps2GprU32(ctx, 6)); }},
    {"IsClusterCollision", 0x11282f0u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { IsClusterCollision(rdram, ctx, runtime, (uint32_t)Ps2GprU32(ctx, 4)); }},
    {"MakeGeometryPointerCollisionEdgeInfo", 0x11283d0u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { MakeGeometryPointerCollisionEdgeInfo(rdram, ctx, runtime, (uint32_t)Ps2GprU32(ctx, 4), (uint32_t)Ps2GprU32(ctx, 5), (uint32_t)Ps2GprU32(ctx, 6), ctx->f[12]); }},
    {"GetCollisionResource", 0x11287d0u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { GetCollisionResource(rdram, ctx, runtime, (uint32_t)Ps2GprU32(ctx, 4)); }},
    {"CollisionObjectJudgeCallbackExceptOneObj", 0x1128818u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { CollisionObjectJudgeCallbackExceptOneObj(rdram, ctx, runtime, (uint64_t)Ps2GprU64(ctx, 4), (uint64_t)Ps2GprU64(ctx, 5)); }},
    {"manager_sub_011293E0", 0x11293e0u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { manager_sub_011293E0(rdram, ctx, runtime, (uint32_t)Ps2GprU32(ctx, 7), ctx->f[12]); }},
    {"manager_sub_01129AE8", 0x1129ae8u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { manager_sub_01129AE8(rdram, ctx, runtime, (uint32_t)Ps2GprU32(ctx, 4)); }},
    {"FlushCollisionEdgeLinkDBVWallFinder", 0x1129b00u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { FlushCollisionEdgeLinkDBVWallFinder(rdram, ctx, runtime, (uint32_t)Ps2GprU32(ctx, 4)); }},
    {"GetCollisionEdgeLinkDBWorkSize", 0x1129b08u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { GetCollisionEdgeLinkDBWorkSize(rdram, ctx, runtime, (uint64_t)Ps2GprU64(ctx, 4)); }},
    {"GetCollisionFinderWorkSize", 0x112a330u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { GetCollisionFinderWorkSize(rdram, ctx, runtime, (uint32_t)Ps2GprU32(ctx, 4)); }},
    {"InitGeometryPointer", 0x11328a8u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { InitGeometryPointer(rdram, ctx, runtime, (uint32_t)Ps2GprU32(ctx, 4)); }},
    {"SetGeometryPointerChainPos", 0x1132cb0u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { SetGeometryPointerChainPos(rdram, ctx, runtime, (uint32_t)Ps2GprU32(ctx, 4), (uint32_t)Ps2GprU32(ctx, 5), (uint32_t)Ps2GprU32(ctx, 6), ctx->f[12]); }},
    {"CheckLegalCollisionGP", 0x1132d58u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { CheckLegalCollisionGP(rdram, ctx, runtime, (uint32_t)Ps2GprU32(ctx, 4)); }},
    {"GetYHeightOfPlaneXZProjection", 0x1134308u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { GetYHeightOfPlaneXZProjection(rdram, ctx, runtime, (uint32_t)Ps2GprU32(ctx, 4), (uint32_t)Ps2GprU32(ctx, 5)); }},
    {"CheckClothChainExist", 0x11398f0u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { CheckClothChainExist(rdram, ctx, runtime, (uint32_t)Ps2GprU32(ctx, 4)); }},
    {"SetClothSingleFixedChainLeafFixNodeOffset", 0x11399d8u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { SetClothSingleFixedChainLeafFixNodeOffset(rdram, ctx, runtime, (uint32_t)Ps2GprU32(ctx, 4), (uint32_t)Ps2GprU32(ctx, 5)); }},
    {"GetTwoWayLinkNext", 0x1147ba8u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { GetTwoWayLinkNext(rdram, ctx, runtime, (uint32_t)Ps2GprU32(ctx, 4)); }},
    {"RelocateTwoWayLinkHeader", 0x1147bb0u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { RelocateTwoWayLinkHeader(rdram, ctx, runtime, (uint32_t)Ps2GprU32(ctx, 4)); }},
    {"GetTwoWayLinkData", 0x1147bc8u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { GetTwoWayLinkData(rdram, ctx, runtime, (uint32_t)Ps2GprU32(ctx, 4)); }},
    {"mcReqSetWaitFunc", 0x1149460u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { mcReqSetWaitFunc(rdram, ctx, runtime, (uint32_t)Ps2GprU32(ctx, 4)); }},
    {"mcGetProcessPer", 0x1149cf8u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { mcGetProcessPer(rdram, ctx, runtime); }},
    {"mcGetGameClearFlag", 0x114a300u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { mcGetGameClearFlag(rdram, ctx, runtime); }},
    {"mcGetCompleteTimeAttack", 0x114a320u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { mcGetCompleteTimeAttack(rdram, ctx, runtime, (uint32_t)Ps2GprU32(ctx, 4), (uint32_t)Ps2GprU32(ctx, 5)); }},
    {"mouseSetCursorPosition", 0x114ad38u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { mouseSetCursorPosition(rdram, ctx, runtime, ctx->f[12], ctx->f[13]); }},
    {"mouseMoveCursorPosition", 0x114ae78u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { mouseMoveCursorPosition(rdram, ctx, runtime, ctx->f[12], ctx->f[13]); }},
    {"setVibrationEnableWorkAdrs", 0x114b580u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { setVibrationEnableWorkAdrs(rdram, ctx, runtime, (uint32_t)Ps2GprU32(ctx, 4)); }},
    {"usbdevMouseGetStatus", 0x114b6e0u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { usbdevMouseGetStatus(rdram, ctx, runtime); }},
    {"windowSemiTransRate", 0x1155b50u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { windowSemiTransRate(rdram, ctx, runtime); }},
    {"windowGetCurrentWindowHandle", 0x11560f8u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { windowGetCurrentWindowHandle(rdram, ctx, runtime); }},
    {"windowSetTex", 0x1156338u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { windowSetTex(rdram, ctx, runtime, (uint32_t)Ps2GprU32(ctx, 4)); }},
    {"adpcmOpenChkComplete", 0x1157870u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { adpcmOpenChkComplete(rdram, ctx, runtime, (uint64_t)Ps2GprU64(ctx, 4)); }},
    {"adpcmCloseReq", 0x1157940u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { adpcmCloseReq(rdram, ctx, runtime, (uint64_t)Ps2GprU64(ctx, 4), (uint64_t)Ps2GprU64(ctx, 5)); }},
    {"adpcmCloseFadeReq", 0x1157978u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { adpcmCloseFadeReq(rdram, ctx, runtime, (uint64_t)Ps2GprU64(ctx, 4), (uint64_t)Ps2GprU64(ctx, 5), ctx->f[12]); }},
    {"adpcmCloseFadeSetVol", 0x11579d0u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { adpcmCloseFadeSetVol(rdram, ctx, runtime, (uint32_t)Ps2GprU32(ctx, 4), ctx->f[12]); }},
    {"adpcmCloseChkComplete", 0x1157a00u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { adpcmCloseChkComplete(rdram, ctx, runtime, (uint64_t)Ps2GprU64(ctx, 4)); }},
    {"adpcmDeleteAdpcmSeq", 0x1157a10u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { adpcmDeleteAdpcmSeq(rdram, ctx, runtime, (uint64_t)Ps2GprU64(ctx, 4)); }},
    {"adpcmSetVol", 0x1157a40u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { adpcmSetVol(rdram, ctx, runtime, (uint64_t)Ps2GprU64(ctx, 4), ctx->f[12], ctx->f[13]); }},
    {"adpcmSetPitch", 0x1157a78u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { adpcmSetPitch(rdram, ctx, runtime, (uint32_t)Ps2GprU32(ctx, 4), ctx->f[12], ctx->f[13]); }},
    {"adpcmGamePause", 0x1157b78u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { adpcmGamePause(rdram, ctx, runtime, (uint32_t)Ps2GprU32(ctx, 4)); }},
    {"seSetVol", 0x11587a8u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { seSetVol(rdram, ctx, runtime, (uint32_t)Ps2GprU32(ctx, 4), ctx->f[12]); }},
    {"manager_sub_01159008", 0x1159008u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { manager_sub_01159008(rdram, ctx, runtime, (uint32_t)Ps2GprU32(ctx, 4)); }},
    {"seDelayPlay3DTest", 0x11590b0u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { seDelayPlay3DTest(rdram, ctx, runtime); }},
    {"sheetFsAddStageTexSegIndex", 0x115bab8u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { sheetFsAddStageTexSegIndex(rdram, ctx, runtime, (uint32_t)Ps2GprU32(ctx, 4)); }},
    {"sheetFsDecStageTexSegIndex", 0x115bad8u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { sheetFsDecStageTexSegIndex(rdram, ctx, runtime, (uint32_t)Ps2GprU32(ctx, 4)); }},
    {"sheetFsSetStageTexSegTexSegDef", 0x115bb98u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { sheetFsSetStageTexSegTexSegDef(rdram, ctx, runtime, (uint32_t)Ps2GprU32(ctx, 4), (uint32_t)Ps2GprU32(ctx, 5)); }},
    {"sheetFsGetStageTexSegTexSegDef", 0x115bbb0u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { sheetFsGetStageTexSegTexSegDef(rdram, ctx, runtime, (uint32_t)Ps2GprU32(ctx, 4)); }},
    {"sheetFsGetDirectFileSize", 0x115c1e0u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { sheetFsGetDirectFileSize(rdram, ctx, runtime, (uint32_t)Ps2GprU32(ctx, 4)); }},
    {"sheetFileGetSize", 0x115cfc0u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { sheetFileGetSize(rdram, ctx, runtime, (uint32_t)Ps2GprU32(ctx, 4)); }},
    {"sheetHeaderObjSearchSheetId", 0x115ec48u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { sheetHeaderObjSearchSheetId(rdram, ctx, runtime, (uint32_t)Ps2GprU32(ctx, 4)); }},
    {"sheetMgrAddSegLoadList", 0x115ed10u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { sheetMgrAddSegLoadList(rdram, ctx, runtime, (uint32_t)Ps2GprU32(ctx, 4)); }},
    {"sheetMgrGetSegLoadHeader", 0x115ed28u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { sheetMgrGetSegLoadHeader(rdram, ctx, runtime); }},
    {"sheetMgrAddSegInitList", 0x115ed38u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { sheetMgrAddSegInitList(rdram, ctx, runtime, (uint32_t)Ps2GprU32(ctx, 4)); }},
    {"sheetMgrGetSegInitHeader", 0x115edb0u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { sheetMgrGetSegInitHeader(rdram, ctx, runtime); }},
    {"sheetMgrSetNoWaitMode", 0x115fa20u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { sheetMgrSetNoWaitMode(rdram, ctx, runtime, (uint32_t)Ps2GprU32(ctx, 4)); }},
    {"get_camera_lws_animation_param", 0x1161c80u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { get_camera_lws_animation_param(rdram, ctx, runtime, (uint32_t)Ps2GprU32(ctx, 4)); }},
    {"get_boot_obj_camera_lws_animation", 0x1161f70u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { get_boot_obj_camera_lws_animation(rdram, ctx, runtime, (uint32_t)Ps2GprU32(ctx, 4)); }},
    {"is_loop_camera_lws_animation", 0x1161f78u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { is_loop_camera_lws_animation(rdram, ctx, runtime, (uint32_t)Ps2GprU32(ctx, 4)); }},
    {"is_sub_type_camera_lws_animation", 0x1161fa0u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { is_sub_type_camera_lws_animation(rdram, ctx, runtime, (uint32_t)Ps2GprU32(ctx, 4)); }},
    {"reset_main_type_camera_lws_animation_update_flag", 0x1162018u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { reset_main_type_camera_lws_animation_update_flag(rdram, ctx, runtime); }},
    {"is_same_camera_lws_animation", 0x1162150u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { is_same_camera_lws_animation(rdram, ctx, runtime, (uint64_t)Ps2GprU64(ctx, 4), (uint64_t)Ps2GprU64(ctx, 5)); }},
    {"stop_camera_lws_animation_geometry_pointer_reference", 0x1162208u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { stop_camera_lws_animation_geometry_pointer_reference(rdram, ctx, runtime, (uint64_t)Ps2GprU64(ctx, 4)); }},
    {"stop_camera_lws_animation_callback", 0x1162238u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { stop_camera_lws_animation_callback(rdram, ctx, runtime, (uint64_t)Ps2GprU64(ctx, 4)); }},
    {"manager_sub_011629B8", 0x11629b8u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { manager_sub_011629B8(rdram, ctx, runtime, (uint32_t)Ps2GprU32(ctx, 4)); }},
    {"get_clip_screen", 0x1162f10u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { get_clip_screen(rdram, ctx, runtime, (uint32_t)Ps2GprU32(ctx, 4)); }},
    {"get_transform_matrix_set", 0x1162f18u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { get_transform_matrix_set(rdram, ctx, runtime, (uint32_t)Ps2GprU32(ctx, 4)); }},
    {"get_change_grace_time_camera_region", 0x1163808u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { get_change_grace_time_camera_region(rdram, ctx, runtime, (uint64_t)Ps2GprU64(ctx, 4)); }},
    {"manager_sub_01164260", 0x1164260u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { manager_sub_01164260(rdram, ctx, runtime, (uint64_t)Ps2GprU64(ctx, 4)); }},
    {"set_screen_wide_and_progressive", 0x1164468u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { set_screen_wide_and_progressive(rdram, ctx, runtime, (uint64_t)Ps2GprU64(ctx, 4), (uint64_t)Ps2GprU64(ctx, 5)); }},
    {"set_view_angle", 0x11644c8u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { set_view_angle(rdram, ctx, runtime, (uint32_t)Ps2GprU32(ctx, 4), ctx->f[12]); }},
    {"get_screen_width", 0x11644f0u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { get_screen_width(rdram, ctx, runtime, (uint32_t)Ps2GprU32(ctx, 4)); }},
    {"get_screen_height", 0x11644f8u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { get_screen_height(rdram, ctx, runtime, (uint32_t)Ps2GprU32(ctx, 4)); }},
    {"get_aspect_ratio", 0x1164500u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { get_aspect_ratio(rdram, ctx, runtime, (uint32_t)Ps2GprU32(ctx, 4)); }},
    {"get_view_angle", 0x1164530u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { get_view_angle(rdram, ctx, runtime, (uint32_t)Ps2GprU32(ctx, 4)); }},
    {"get_near_clip_z_clip_screen", 0x1164558u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { get_near_clip_z_clip_screen(rdram, ctx, runtime); }},
    {"get_far_clip_z_clip_screen", 0x1164570u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { get_far_clip_z_clip_screen(rdram, ctx, runtime); }},
    {"get_z_buf_min_clip_screen", 0x1164588u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { get_z_buf_min_clip_screen(rdram, ctx, runtime); }},
    {"get_z_buf_max_clip_screen", 0x1164598u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { get_z_buf_max_clip_screen(rdram, ctx, runtime); }},
    {"get_scissor_clip_matrix", 0x1164ca8u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { get_scissor_clip_matrix(rdram, ctx, runtime, (uint32_t)Ps2GprU32(ctx, 4), (uint32_t)Ps2GprU32(ctx, 5)); }},
    {"get_WV_matrix", 0x1164cb8u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { get_WV_matrix(rdram, ctx, runtime, (uint64_t)Ps2GprU64(ctx, 4)); }},
    {"ResetDamageClass", 0x1166de8u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { ResetDamageClass(rdram, ctx, runtime, (uint32_t)Ps2GprU32(ctx, 4)); }},
    {"SetDamageClassDamageTableType", 0x1166eb8u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { SetDamageClassDamageTableType(rdram, ctx, runtime, (uint64_t)Ps2GprU64(ctx, 4)); }},
    {"getEditWorkInitData", 0x1169c30u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { getEditWorkInitData(rdram, ctx, runtime, (uint64_t)Ps2GprU64(ctx, 4)); }},
    {"getPointData", 0x1169d48u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { getPointData(rdram, ctx, runtime, (uint64_t)Ps2GprU64(ctx, 4), (uint64_t)Ps2GprU64(ctx, 5)); }},
    {"getLineData", 0x1169de0u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { getLineData(rdram, ctx, runtime, (uint64_t)Ps2GprU64(ctx, 4), (uint64_t)Ps2GprU64(ctx, 5)); }},
    {"getRootPointData", 0x1169e68u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { getRootPointData(rdram, ctx, runtime, (uint64_t)Ps2GprU64(ctx, 4), (uint64_t)Ps2GprU64(ctx, 5)); }},
    {"getGoalPointData", 0x1169ea8u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { getGoalPointData(rdram, ctx, runtime, (uint64_t)Ps2GprU64(ctx, 4), (uint64_t)Ps2GprU64(ctx, 5)); }},
    {"getSubPointData", 0x1169ee8u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { getSubPointData(rdram, ctx, runtime, (uint64_t)Ps2GprU64(ctx, 4), (uint64_t)Ps2GprU64(ctx, 5)); }},
    {"getInterestPointData", 0x1169ff0u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { getInterestPointData(rdram, ctx, runtime, (uint64_t)Ps2GprU64(ctx, 4), (uint64_t)Ps2GprU64(ctx, 5)); }},
    {"getDeadPointData", 0x116a040u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { getDeadPointData(rdram, ctx, runtime, (uint64_t)Ps2GprU64(ctx, 4), (uint64_t)Ps2GprU64(ctx, 5)); }},
    {"getLayoutPointData", 0x116a098u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { getLayoutPointData(rdram, ctx, runtime, (uint64_t)Ps2GprU64(ctx, 4), (uint64_t)Ps2GprU64(ctx, 5)); }},
    {"getSnapShotFormat", 0x1341920u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { getSnapShotFormat(rdram, ctx, runtime); }},
    {"GAMEUTIL_get_sight_on_reverse_cntrlLR_bool", 0x1342b40u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { GAMEUTIL_get_sight_on_reverse_cntrlLR_bool(rdram, ctx, runtime); }},
    {"GAMEUTIL_get_sight_on_reverse_cntrlUD_bool", 0x1342b70u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { GAMEUTIL_get_sight_on_reverse_cntrlUD_bool(rdram, ctx, runtime); }},
    {"GAMEUTIL_get_game_camera_free_mode_reverse_cntrlLR_bool", 0x1342c70u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { GAMEUTIL_get_game_camera_free_mode_reverse_cntrlLR_bool(rdram, ctx, runtime); }},
    {"GAMEUTIL_get_game_camera_free_mode_reverse_cntrlUD_bool", 0x1342ca0u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { GAMEUTIL_get_game_camera_free_mode_reverse_cntrlUD_bool(rdram, ctx, runtime); }},
    {"GAMEUTIL_set_auto_get_flag_boss_interest_position", 0x1343b28u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { GAMEUTIL_set_auto_get_flag_boss_interest_position(rdram, ctx, runtime, (uint32_t)Ps2GprU32(ctx, 4)); }},
    {"GAMEUTIL_reset_move_angle", 0x1343cd0u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { GAMEUTIL_reset_move_angle(rdram, ctx, runtime, (uint32_t)Ps2GprU32(ctx, 4)); }},
    {"GAMEUTIL_set_parameter_move_angle", 0x1343cd8u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { GAMEUTIL_set_parameter_move_angle(rdram, ctx, runtime, (uint32_t)Ps2GprU32(ctx, 4), ctx->f[12], ctx->f[13], ctx->f[14], ctx->f[15], ctx->f[16], ctx->f[17]); }},
    {"GAMEUTIL_begin_move_angle", 0x1343d18u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { GAMEUTIL_begin_move_angle(rdram, ctx, runtime, (uint32_t)Ps2GprU32(ctx, 4)); }},
    {"GAMEUTIL_get_cur_velocity_move_angle", 0x1343e10u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { GAMEUTIL_get_cur_velocity_move_angle(rdram, ctx, runtime, (uint32_t)Ps2GprU32(ctx, 4)); }},
    {"GAMEUTIL_set_need_dir_ratio_flag_look_at_vs_offset", 0x1343ee8u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { GAMEUTIL_set_need_dir_ratio_flag_look_at_vs_offset(rdram, ctx, runtime, (uint32_t)Ps2GprU32(ctx, 4), (uint32_t)Ps2GprU32(ctx, 5)); }},
    {"GAMEUTIL_set_enable_side_change_angle_look_at_vs_offset", 0x1343ef0u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { GAMEUTIL_set_enable_side_change_angle_look_at_vs_offset(rdram, ctx, runtime, (uint32_t)Ps2GprU32(ctx, 4), ctx->f[12]); }},
    {"GAMEUTIL_set_parameter_col_plane_info_filter", 0x13440a8u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { GAMEUTIL_set_parameter_col_plane_info_filter(rdram, ctx, runtime, (uint32_t)Ps2GprU32(ctx, 4), ctx->f[12], ctx->f[13]); }},
    {"GAMEUTIL_set_parameter_position_filter", 0x1344208u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { GAMEUTIL_set_parameter_position_filter(rdram, ctx, runtime, (uint32_t)Ps2GprU32(ctx, 4), ctx->f[12]); }},
    {"GAMEUTIL_reset_status_manager", 0x13445c0u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { GAMEUTIL_reset_status_manager(rdram, ctx, runtime, (uint32_t)Ps2GprU32(ctx, 4), (uint32_t)Ps2GprU32(ctx, 5)); }},
    {"GAMEUTIL_is_status_change_status_manager", 0x1344628u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { GAMEUTIL_is_status_change_status_manager(rdram, ctx, runtime, (uint32_t)Ps2GprU32(ctx, 4)); }},
    {"GAMEUTIL_get_status_continue_time_status_manager", 0x1344648u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { GAMEUTIL_get_status_continue_time_status_manager(rdram, ctx, runtime, (uint32_t)Ps2GprU32(ctx, 4)); }},
    {"GAMEUTIL_get_cur_status_status_manager", 0x1344650u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { GAMEUTIL_get_cur_status_status_manager(rdram, ctx, runtime, (uint32_t)Ps2GprU32(ctx, 4)); }},
    {"GAMEUTIL_get_pre_status_status_manager", 0x1344658u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { GAMEUTIL_get_pre_status_status_manager(rdram, ctx, runtime, (uint32_t)Ps2GprU32(ctx, 4)); }},
    {"GAMEUTIL_get_pre_changed_status_status_manager", 0x1344660u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { GAMEUTIL_get_pre_changed_status_status_manager(rdram, ctx, runtime, (uint32_t)Ps2GprU32(ctx, 4)); }},
    {"getSightOnRevCntrlLR", 0x1344668u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { getSightOnRevCntrlLR(rdram, ctx, runtime); }},
    {"getSightOnRevCntrlUD", 0x1344678u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { getSightOnRevCntrlUD(rdram, ctx, runtime); }},
    {"getFreeModeRevControlLR", 0x1344698u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { getFreeModeRevControlLR(rdram, ctx, runtime); }},
    {"getFreeModeRevControlUD", 0x13446a8u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { getFreeModeRevControlUD(rdram, ctx, runtime); }},
    {"getFreeModeRevAngleVelRatio", 0x13446b8u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { getFreeModeRevAngleVelRatio(rdram, ctx, runtime); }},
    {"GetGameTimeScaleRatio", 0x1345d40u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { GetGameTimeScaleRatio(rdram, ctx, runtime); }},
    {"GetCurrentGameAnimeTimeScaleRatio", 0x1345e38u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { GetCurrentGameAnimeTimeScaleRatio(rdram, ctx, runtime); }},
    {"GetGameAnimeTimeScaleRatio", 0x1345e48u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { GetGameAnimeTimeScaleRatio(rdram, ctx, runtime); }},
    {"GetPlayerControlButtonContext", 0x1345e58u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { GetPlayerControlButtonContext(rdram, ctx, runtime); }},
    {"RestoreDefaultGameAnimeTimeScaleRatio", 0x1345e78u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { RestoreDefaultGameAnimeTimeScaleRatio(rdram, ctx, runtime); }},
    {"SetDefaultGameAnimeTimeScaleRatio", 0x1345e90u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { SetDefaultGameAnimeTimeScaleRatio(rdram, ctx, runtime, ctx->f[12]); }},
    {"getFrameRateMode", 0x1345ed0u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { getFrameRateMode(rdram, ctx, runtime); }},
    {"setDefaultPlayerControlButton", 0x1346028u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { setDefaultPlayerControlButton(rdram, ctx, runtime, (uint64_t)Ps2GprU64(ctx, 4)); }},
    {"getFadeValue", 0x1346210u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { getFadeValue(rdram, ctx, runtime); }},
    {"set_yaw_pitch_input_border_parameter_nico_yaw_pitch_cntrl", 0x1346df0u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { set_yaw_pitch_input_border_parameter_nico_yaw_pitch_cntrl(rdram, ctx, runtime, (uint32_t)Ps2GprU32(ctx, 4), ctx->f[12], ctx->f[13], ctx->f[14], ctx->f[15]); }},
    {"set_base_yaw_parameter_nico_yaw_pitch_cntrl", 0x1346fe8u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { set_base_yaw_parameter_nico_yaw_pitch_cntrl(rdram, ctx, runtime, (uint32_t)Ps2GprU32(ctx, 4), (uint32_t)Ps2GprU32(ctx, 5), ctx->f[12], ctx->f[13], ctx->f[14], ctx->f[15], ctx->f[16], ctx->f[17]); }},
    {"set_yaw_parameter_effective_ratio_nico_yaw_pitch_cntrl", 0x1347358u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { set_yaw_parameter_effective_ratio_nico_yaw_pitch_cntrl(rdram, ctx, runtime, (uint32_t)Ps2GprU32(ctx, 4), ctx->f[12]); }},
    {"set_pitch_parameter_effective_ratio_nico_yaw_pitch_cntrl", 0x1347360u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { set_pitch_parameter_effective_ratio_nico_yaw_pitch_cntrl(rdram, ctx, runtime, (uint32_t)Ps2GprU32(ctx, 4), ctx->f[12]); }},
    {"set_temp_stop_base_rot_flag_nico_yaw_pitch_cntrl", 0x1347598u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { set_temp_stop_base_rot_flag_nico_yaw_pitch_cntrl(rdram, ctx, runtime, (uint32_t)Ps2GprU32(ctx, 4), (uint32_t)Ps2GprU32(ctx, 5)); }},
    {"is_enable_base_yaw_rot_nico_yaw_pitch_cntrl", 0x1347838u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { is_enable_base_yaw_rot_nico_yaw_pitch_cntrl(rdram, ctx, runtime, (uint32_t)Ps2GprU32(ctx, 4)); }},
    {"is_enable_base_pitch_rot_nico_yaw_pitch_cntrl", 0x1347840u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { is_enable_base_pitch_rot_nico_yaw_pitch_cntrl(rdram, ctx, runtime, (uint32_t)Ps2GprU32(ctx, 4)); }},
    {"get_base_yaw_move_nico_yaw_pitch_cntrl", 0x1347848u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { get_base_yaw_move_nico_yaw_pitch_cntrl(rdram, ctx, runtime, (uint32_t)Ps2GprU32(ctx, 4)); }},
    {"set_base_yaw_nico_yaw_pitch_cntrl", 0x1347858u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { set_base_yaw_nico_yaw_pitch_cntrl(rdram, ctx, runtime, (uint32_t)Ps2GprU32(ctx, 4), ctx->f[12]); }},
    {"set_base_pitch_nico_yaw_pitch_cntrl", 0x1347860u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { set_base_pitch_nico_yaw_pitch_cntrl(rdram, ctx, runtime, (uint32_t)Ps2GprU32(ctx, 4), ctx->f[12]); }},
    {"get_base_yaw_nico_yaw_pitch_cntrl", 0x1347868u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { get_base_yaw_nico_yaw_pitch_cntrl(rdram, ctx, runtime, (uint32_t)Ps2GprU32(ctx, 4)); }},
    {"get_base_pitch_nico_yaw_pitch_cntrl", 0x1347870u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { get_base_pitch_nico_yaw_pitch_cntrl(rdram, ctx, runtime, (uint32_t)Ps2GprU32(ctx, 4)); }},
    {"backStageGetBossSaveArea", 0x13494a8u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { backStageGetBossSaveArea(rdram, ctx, runtime); }},
    {"backStageInit", 0x13494b8u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { backStageInit(rdram, ctx, runtime); }},
    {"bossStatGetDeadBoss", 0x134a358u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { bossStatGetDeadBoss(rdram, ctx, runtime); }},
    {"CheckCamFinalizerReady", 0x134a778u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { CheckCamFinalizerReady(rdram, ctx, runtime); }},
    {"SetCamFinalizerReady", 0x134a788u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { SetCamFinalizerReady(rdram, ctx, runtime, (uint32_t)Ps2GprU32(ctx, 4)); }},
    {"PauseCamFinalizer", 0x134a7c0u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { PauseCamFinalizer(rdram, ctx, runtime, (uint32_t)Ps2GprU32(ctx, 4)); }},
    {"CheckDemoCameraOrientFinished", 0x134b260u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { CheckDemoCameraOrientFinished(rdram, ctx, runtime, (uint32_t)Ps2GprU32(ctx, 4)); }},
    {"getDemoEnableFlag", 0x134b268u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { getDemoEnableFlag(rdram, ctx, runtime); }},
    {"demoscrCheckDemoFinish", 0x134c838u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { demoscrCheckDemoFinish(rdram, ctx, runtime); }},
    {"demoscrCheckDemoCutSkip", 0x134c850u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { demoscrCheckDemoCutSkip(rdram, ctx, runtime); }},
    {"endingflowTickAdpcmSeq", 0x134d728u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { endingflowTickAdpcmSeq(rdram, ctx, runtime, (uint32_t)Ps2GprU32(ctx, 4)); }},
    {"gamecore_sub_0134D920", 0x134d920u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { gamecore_sub_0134D920(rdram, ctx, runtime, (uint32_t)Ps2GprU32(ctx, 8)); }},
    {"initFadeParam", 0x134db40u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { initFadeParam(rdram, ctx, runtime, (uint32_t)Ps2GprU32(ctx, 4), ctx->f[12]); }},
    {"setFadeParam", 0x134db68u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { setFadeParam(rdram, ctx, runtime, (uint32_t)Ps2GprU32(ctx, 4), (uint32_t)Ps2GprU32(ctx, 5), ctx->f[12], ctx->f[13]); }},
    {"calcFadeParam", 0x134db98u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { calcFadeParam(rdram, ctx, runtime, (uint32_t)Ps2GprU32(ctx, 4)); }},
    {"checkFadeParam", 0x134dbe8u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { checkFadeParam(rdram, ctx, runtime, (uint32_t)Ps2GprU32(ctx, 4)); }},
    {"fadeChkStat", 0x134dc20u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { fadeChkStat(rdram, ctx, runtime, (uint64_t)Ps2GprU64(ctx, 4)); }},
    {"setTreeId", 0x134e390u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { setTreeId(rdram, ctx, runtime, (uint32_t)Ps2GprU32(ctx, 4)); }},
    {"_optionWorkChk", 0x134ef80u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { _optionWorkChk(rdram, ctx, runtime, (uint32_t)Ps2GprU32(ctx, 4)); }},
    {"_optionWorkPtr", 0x134ef98u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { _optionWorkPtr(rdram, ctx, runtime, (uint32_t)Ps2GprU32(ctx, 4)); }},
    {"_gameIWorkSet", 0x134f0d8u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { _gameIWorkSet(rdram, ctx, runtime, (uint32_t)Ps2GprU32(ctx, 4), (uint32_t)Ps2GprU32(ctx, 5)); }},
    {"_gameWorkChk", 0x134f108u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { _gameWorkChk(rdram, ctx, runtime, (uint32_t)Ps2GprU32(ctx, 4)); }},
    {"_gameIWorkChk", 0x134f138u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { _gameIWorkChk(rdram, ctx, runtime, (uint32_t)Ps2GprU32(ctx, 4)); }},
    {"setBossClearCnt", 0x134f238u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { setBossClearCnt(rdram, ctx, runtime, (uint32_t)Ps2GprU32(ctx, 4)); }},
    {"getGameClearCnt", 0x134f278u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { getGameClearCnt(rdram, ctx, runtime); }},
    {"getGameHardMode", 0x134f2a8u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { getGameHardMode(rdram, ctx, runtime); }},
    {"setSavedClock", 0x134f320u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { setSavedClock(rdram, ctx, runtime, (uint32_t)Ps2GprU32(ctx, 4)); }},
    {"getSavedPlaceId", 0x134f330u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { getSavedPlaceId(rdram, ctx, runtime); }},
    {"getOptionDataAdrsAndSize", 0x134fe00u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { getOptionDataAdrsAndSize(rdram, ctx, runtime, (uint32_t)Ps2GprU32(ctx, 4)); }},
    {"gameflowSelectGameoverMenu", 0x13521e0u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { gameflowSelectGameoverMenu(rdram, ctx, runtime, (uint32_t)Ps2GprU32(ctx, 4)); }},
    {"gameflowSetPauseEnable", 0x1352418u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { gameflowSetPauseEnable(rdram, ctx, runtime, (uint64_t)Ps2GprU64(ctx, 4)); }},
    {"gameflowGameOver", 0x13524a8u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { gameflowGameOver(rdram, ctx, runtime); }},
    {"gameflowGetJumpSekiban", 0x13524b8u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { gameflowGetJumpSekiban(rdram, ctx, runtime); }},
    {"gameflowSetJumpSekiban", 0x13524c8u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { gameflowSetJumpSekiban(rdram, ctx, runtime, (uint32_t)Ps2GprU32(ctx, 4)); }},
    {"gamecore_sub_01352770", 0x1352770u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { gamecore_sub_01352770(rdram, ctx, runtime); }},
    {"gStatGetOmakeStatAdrs", 0x1353570u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { gStatGetOmakeStatAdrs(rdram, ctx, runtime); }},
    {"gStatGObjDispChk", 0x13535d8u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { gStatGObjDispChk(rdram, ctx, runtime, (uint32_t)Ps2GprU32(ctx, 4)); }},
    {"gStatSetGObjAdrs", 0x13536d0u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { gStatSetGObjAdrs(rdram, ctx, runtime, (uint32_t)Ps2GprU32(ctx, 4), (uint32_t)Ps2GprU32(ctx, 5)); }},
    {"gStatGetPlayerAccessoriesDamgeRaitoToPlayer", 0x1353730u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { gStatGetPlayerAccessoriesDamgeRaitoToPlayer(rdram, ctx, runtime); }},
    {"gStatGetPlayerAccessoriesDamgeRaitoToBoss", 0x1353768u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { gStatGetPlayerAccessoriesDamgeRaitoToBoss(rdram, ctx, runtime); }},
    {"gstatBootMcSetExistNicoData", 0x1353e00u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { gstatBootMcSetExistNicoData(rdram, ctx, runtime, (uint32_t)Ps2GprU32(ctx, 4), (uint32_t)Ps2GprU32(ctx, 5)); }},
    {"gstatBootMcSetExistIcoData", 0x1353ef8u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { gstatBootMcSetExistIcoData(rdram, ctx, runtime, (uint32_t)Ps2GprU32(ctx, 4)); }},
    {"gstatBootMcChkExistIcoData", 0x1353f08u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { gstatBootMcChkExistIcoData(rdram, ctx, runtime); }},
    {"jumphokoraflowDemoStart", 0x13549a0u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { jumphokoraflowDemoStart(rdram, ctx, runtime); }},
    {"jumphokoraflowDemoEnd", 0x13549b0u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { jumphokoraflowDemoEnd(rdram, ctx, runtime, (uint32_t)Ps2GprU32(ctx, 4)); }},
    {"jumphokoraGetDemoInfo", 0x13549d0u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { jumphokoraGetDemoInfo(rdram, ctx, runtime, (uint32_t)Ps2GprU32(ctx, 4), (uint32_t)Ps2GprU32(ctx, 5)); }},
    {"gcCheckStageLoadFinish", 0x1356570u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { gcCheckStageLoadFinish(rdram, ctx, runtime); }},
    {"opflowOp1End", 0x1357648u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { opflowOp1End(rdram, ctx, runtime); }},
    {"opflowOp2Start", 0x1357660u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { opflowOp2Start(rdram, ctx, runtime); }},
    {"opflowOp2End", 0x1357678u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { opflowOp2End(rdram, ctx, runtime, (uint32_t)Ps2GprU32(ctx, 4)); }},
    {"opflowChkTitleDemoStart", 0x13576a8u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { opflowChkTitleDemoStart(rdram, ctx, runtime); }},
    {"opflowTitleDmoeEnd", 0x13576c0u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { opflowTitleDmoeEnd(rdram, ctx, runtime); }},
    {"opflowChkAbort", 0x13576f8u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { opflowChkAbort(rdram, ctx, runtime); }},
    {"opflowChkLoadStart", 0x1357708u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { opflowChkLoadStart(rdram, ctx, runtime); }},
    {"getPlayerHp", 0x1358248u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { getPlayerHp(rdram, ctx, runtime); }},
    {"getPlayerGrip", 0x13584e8u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { getPlayerGrip(rdram, ctx, runtime); }},
    {"initPowerWork", 0x1358778u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { initPowerWork(rdram, ctx, runtime); }},
    {"resetAttackPower", 0x1358788u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { resetAttackPower(rdram, ctx, runtime); }},
    {"getAttackPowerRatio", 0x1358798u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { getAttackPowerRatio(rdram, ctx, runtime); }},
    {"setFullPower", 0x1358840u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { setFullPower(rdram, ctx, runtime); }},
    {"gamecore_sub_01358878", 0x1358878u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { gamecore_sub_01358878(rdram, ctx, runtime); }},
    {"getAreaSize", 0x135d740u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { getAreaSize(rdram, ctx, runtime); }},
    {"getPosOnMap", 0x135d9c0u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { getPosOnMap(rdram, ctx, runtime, (uint32_t)Ps2GprU32(ctx, 4), (uint32_t)Ps2GprU32(ctx, 5), (uint32_t)Ps2GprU32(ctx, 6)); }},
    {"getSeamlessStageHighModelDispDistDefault", 0x135dae8u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { getSeamlessStageHighModelDispDistDefault(rdram, ctx, runtime); }},
    {"getSeamlessStageMiddleModelDispDistDefault", 0x135daf8u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { getSeamlessStageMiddleModelDispDistDefault(rdram, ctx, runtime); }},
    {"getSeamlessStageEnv", 0x135dd58u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { getSeamlessStageEnv(rdram, ctx, runtime); }},
    {"forceLoadSeamlessStage", 0x135df88u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { forceLoadSeamlessStage(rdram, ctx, runtime); }},
    {"SetSeamlessStageControlObject", 0x135df98u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { SetSeamlessStageControlObject(rdram, ctx, runtime, (uint32_t)Ps2GprU32(ctx, 4)); }},
    {"PauseSeamlessStageManager", 0x135e138u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { PauseSeamlessStageManager(rdram, ctx, runtime, (uint64_t)Ps2GprU64(ctx, 4)); }},
    {"getStageIDWithOffset", 0x135e280u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { getStageIDWithOffset(rdram, ctx, runtime, (uint32_t)Ps2GprU32(ctx, 4), (uint32_t)Ps2GprU32(ctx, 5), (uint32_t)Ps2GprU32(ctx, 6), (uint32_t)Ps2GprU32(ctx, 7), (uint32_t)Ps2GprU32(ctx, 8)); }},
    {"isSeamlessStage", 0x135e608u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { isSeamlessStage(rdram, ctx, runtime); }},
    {"getSpecialEffectObjWork", 0x135efb0u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { getSpecialEffectObjWork(rdram, ctx, runtime, (uint32_t)Ps2GprU32(ctx, 4)); }},
    {"GetWorldLayoutSheetSegHeader", 0x1365d08u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { GetWorldLayoutSheetSegHeader(rdram, ctx, runtime); }},
    {"stagemanager_getSegmentVariation", 0x1366b60u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { stagemanager_getSegmentVariation(rdram, ctx, runtime); }},
    {"initWorldLayout", 0x1366d00u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { initWorldLayout(rdram, ctx, runtime, (uint32_t)Ps2GprU32(ctx, 4)); }},
    {"bgmEnvTickProckMask", 0x136a2d0u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { bgmEnvTickProckMask(rdram, ctx, runtime, (uint32_t)Ps2GprU32(ctx, 4)); }},
    {"bossNormalHook", 0x136a378u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { bossNormalHook(rdram, ctx, runtime, (uint32_t)Ps2GprU32(ctx, 8)); }},
    {"gamecore_sub_0136A388", 0x136a388u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { gamecore_sub_0136A388(rdram, ctx, runtime, (uint64_t)Ps2GprU64(ctx, 4)); }},
    {"bgmPlay", 0x136aa38u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { bgmPlay(rdram, ctx, runtime, (uint32_t)Ps2GprU32(ctx, 4), ctx->f[12]); }},
    {"bgmStop", 0x136aa50u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { bgmStop(rdram, ctx, runtime, (uint32_t)Ps2GprU32(ctx, 4), (uint64_t)Ps2GprU64(ctx, 5), ctx->f[12]); }},
    {"bgmStopChk", 0x136aa70u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { bgmStopChk(rdram, ctx, runtime, (uint64_t)Ps2GprU64(ctx, 4)); }},
    {"bgmChangeVol", 0x136aad0u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { bgmChangeVol(rdram, ctx, runtime, (uint32_t)Ps2GprU32(ctx, 4), ctx->f[12], ctx->f[13]); }},
    {"gamecore_sub_0136E4F8", 0x136e4f8u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { gamecore_sub_0136E4F8(rdram, ctx, runtime, (uint32_t)Ps2GprU32(ctx, 4)); }},
    {"gamecore_sub_0136E7A0", 0x136e7a0u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { gamecore_sub_0136E7A0(rdram, ctx, runtime, (uint32_t)Ps2GprU32(ctx, 5)); }},
    {"nico_IK_idDetect", 0x13757f8u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { nico_IK_idDetect(rdram, ctx, runtime, (uint32_t)Ps2GprU32(ctx, 4)); }},
    {"gamecore_sub_0137B738", 0x137b738u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { gamecore_sub_0137B738(rdram, ctx, runtime, (uint64_t)Ps2GprU64(ctx, 4), (uint64_t)Ps2GprU64(ctx, 5)); }},
    {"menuSelected", 0x137b800u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { menuSelected(rdram, ctx, runtime); }},
    {"GetDebugDispType", 0x137c310u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { GetDebugDispType(rdram, ctx, runtime); }},
    {"is_LwsOrientObj", 0x137e9f0u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { is_LwsOrientObj(rdram, ctx, runtime, (uint32_t)Ps2GprU32(ctx, 4)); }},
    {"CheckDeathTentacle", 0x1384de0u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { CheckDeathTentacle(rdram, ctx, runtime); }},
    {"getEffectBloodWorkSize", 0x1388048u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { getEffectBloodWorkSize(rdram, ctx, runtime, (uint32_t)Ps2GprU32(ctx, 4)); }},
    {"getWeakPointEffectIntensity", 0x1388060u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { getWeakPointEffectIntensity(rdram, ctx, runtime, (uint32_t)Ps2GprU32(ctx, 4)); }},
    {"getTriggerPointEffectIntensity", 0x1388080u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { getTriggerPointEffectIntensity(rdram, ctx, runtime, (uint32_t)Ps2GprU32(ctx, 4)); }},
    {"effectClearDamageWork", 0x13885f0u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { effectClearDamageWork(rdram, ctx, runtime); }},
    {"effectGetDamageWork", 0x1388600u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { effectGetDamageWork(rdram, ctx, runtime, (uint32_t)Ps2GprU32(ctx, 4)); }},
    {"gamecore_sub_013886B8", 0x13886b8u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { gamecore_sub_013886B8(rdram, ctx, runtime, (uint32_t)Ps2GprU32(ctx, 4)); }},
    {"getFallenLeafSize", 0x138ea10u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { getFallenLeafSize(rdram, ctx, runtime, (uint64_t)Ps2GprU64(ctx, 4)); }},
    {"getArrowWaterSize", 0x138eb48u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { getArrowWaterSize(rdram, ctx, runtime, (uint32_t)Ps2GprU32(ctx, 4)); }},
    {"gamecore_sub_0138F138", 0x138f138u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { gamecore_sub_0138F138(rdram, ctx, runtime, (uint32_t)Ps2GprU32(ctx, 6), (uint64_t)Ps2GprU64(ctx, 7)); }},
    {"setHorseAIStopFlag", 0x1395e00u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { setHorseAIStopFlag(rdram, ctx, runtime, (uint32_t)Ps2GprU32(ctx, 4)); }},
    {"getHorseStandByMode", 0x1395e10u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { getHorseStandByMode(rdram, ctx, runtime); }},
    {"SCRFuncHorseStandByFlag", 0x1396328u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { SCRFuncHorseStandByFlag(rdram, ctx, runtime, (uint32_t)Ps2GprU32(ctx, 5)); }},
    {"ColCont_Reset", 0x139a878u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { ColCont_Reset(rdram, ctx, runtime, (uint32_t)Ps2GprU32(ctx, 4)); }},
    {"ColCont2_IsColStatus", 0x139aa00u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { ColCont2_IsColStatus(rdram, ctx, runtime, (uint32_t)Ps2GprU32(ctx, 4), (uint64_t)Ps2GprU64(ctx, 5), (uint64_t)Ps2GprU64(ctx, 6)); }},
    {"ColCont2_IsEventColStatus", 0x139aa50u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { ColCont2_IsEventColStatus(rdram, ctx, runtime, (uint32_t)Ps2GprU32(ctx, 4), (uint64_t)Ps2GprU64(ctx, 5), (uint64_t)Ps2GprU64(ctx, 6)); }},
    {"MotSpdCtrl_Reset", 0x139ac08u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { MotSpdCtrl_Reset(rdram, ctx, runtime, (uint32_t)Ps2GprU32(ctx, 4)); }},
    {"MotSpdCtrl_SetParam", 0x139ac60u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { MotSpdCtrl_SetParam(rdram, ctx, runtime, (uint32_t)Ps2GprU32(ctx, 4), (uint32_t)Ps2GprU32(ctx, 5)); }},
    {"inInNowLoadingEffect", 0x139e988u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { inInNowLoadingEffect(rdram, ctx, runtime); }},
    {"DirectTurn_Reset", 0x13a75f8u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { DirectTurn_Reset(rdram, ctx, runtime, (uint32_t)Ps2GprU32(ctx, 4)); }},
    {"is_doing_nico_sight_on", 0x13aa380u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { is_doing_nico_sight_on(rdram, ctx, runtime, (uint32_t)Ps2GprU32(ctx, 4)); }},
    {"getPlayerLastAttrStatus", 0x13b53b0u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { getPlayerLastAttrStatus(rdram, ctx, runtime); }},
    {"getPlayerLastParentObj", 0x13b53c0u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { getPlayerLastParentObj(rdram, ctx, runtime); }},
    {"nico_setPlayerDemoAdditionClothCollisionFloorTable1", 0x13ba418u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { nico_setPlayerDemoAdditionClothCollisionFloorTable1(rdram, ctx, runtime); }},
    {"nico_setPlayerDemoAdditionClothCollisionRightArm", 0x13ba458u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { nico_setPlayerDemoAdditionClothCollisionRightArm(rdram, ctx, runtime, (uint32_t)Ps2GprU32(ctx, 4)); }},
    {"nico_setPlayerDemoAdditionClothCollisionRightHandAndSwordDeep", 0x13ba4b0u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { nico_setPlayerDemoAdditionClothCollisionRightHandAndSwordDeep(rdram, ctx, runtime, (uint32_t)Ps2GprU32(ctx, 4)); }},
    {"nico_setPlayerDemoAdditionClothCollisionRightHandAndSword", 0x13ba508u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { nico_setPlayerDemoAdditionClothCollisionRightHandAndSword(rdram, ctx, runtime, (uint32_t)Ps2GprU32(ctx, 4)); }},
    {"nico_resetPlayerDemoAdditionClothCollision", 0x13ba768u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { nico_resetPlayerDemoAdditionClothCollision(rdram, ctx, runtime); }},
    {"player_initSlipEffect", 0x13cd5f8u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { player_initSlipEffect(rdram, ctx, runtime, (uint32_t)Ps2GprU32(ctx, 5)); }},
    {"bossCloudStart", 0x13d0ab8u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { bossCloudStart(rdram, ctx, runtime); }},
    {"bossDemoPlayerBreakDownAccept", 0x13d0bc0u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { bossDemoPlayerBreakDownAccept(rdram, ctx, runtime); }},
    {"bossDemoPlayerBreakDownEnd", 0x13d0bd0u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { bossDemoPlayerBreakDownEnd(rdram, ctx, runtime); }},
    {"AI_InitAnimalOptimize", 0x13d38a0u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { AI_InitAnimalOptimize(rdram, ctx, runtime, (uint32_t)Ps2GprU32(ctx, 4)); }},
    {"AI_InitAnimalWork", 0x13d3a48u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { AI_InitAnimalWork(rdram, ctx, runtime, (uint32_t)Ps2GprU32(ctx, 4), (uint32_t)Ps2GprU32(ctx, 5)); }},
    {"AI_SetAnimalWorkParam", 0x13d3a68u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { AI_SetAnimalWorkParam(rdram, ctx, runtime, (uint32_t)Ps2GprU32(ctx, 4), (uint32_t)Ps2GprU32(ctx, 5)); }},
    {"AI_InitGexCommonWork", 0x13d3a70u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { AI_InitGexCommonWork(rdram, ctx, runtime, (uint32_t)Ps2GprU32(ctx, 4)); }},
    {"AI_InitSurpriseArrow", 0x13d3a88u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { AI_InitSurpriseArrow(rdram, ctx, runtime, (uint32_t)Ps2GprU32(ctx, 4)); }},
    {"AI_SetSurprisedArea", 0x13d3a98u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { AI_SetSurprisedArea(rdram, ctx, runtime, (uint32_t)Ps2GprU32(ctx, 4), ctx->f[12]); }},
    {"AI_UpdateVelocity", 0x13d3da0u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { AI_UpdateVelocity(rdram, ctx, runtime, (uint32_t)Ps2GprU32(ctx, 4), ctx->f[12], ctx->f[13]); }},
    {"AI_GetAccelB", 0x13d4008u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { AI_GetAccelB(rdram, ctx, runtime, ctx->f[12], ctx->f[13], ctx->f[14], ctx->f[15]); }},
    {"AI_GetAccelA", 0x13d4060u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { AI_GetAccelA(rdram, ctx, runtime, ctx->f[12], ctx->f[13], ctx->f[14], ctx->f[15]); }},
    {"AI_GetAccelA_at0", 0x13d40a8u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { AI_GetAccelA_at0(rdram, ctx, runtime, ctx->f[12], ctx->f[13], ctx->f[14]); }},
    {"AI_GetAccelB_at0", 0x13d40f0u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { AI_GetAccelB_at0(rdram, ctx, runtime, ctx->f[12], ctx->f[13], ctx->f[14]); }},
    {"AI_GetAccelA3", 0x13d4128u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { AI_GetAccelA3(rdram, ctx, runtime, ctx->f[12], ctx->f[13], ctx->f[14], ctx->f[15], ctx->f[16], ctx->f[17]); }},
    {"AI_GetAccelB3", 0x13d4298u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { AI_GetAccelB3(rdram, ctx, runtime, ctx->f[12], ctx->f[13], ctx->f[14], ctx->f[15], ctx->f[16], ctx->f[17]); }},
    {"AI_GetAccelC3", 0x13d43f0u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { AI_GetAccelC3(rdram, ctx, runtime, ctx->f[12], ctx->f[13], ctx->f[14], ctx->f[15], ctx->f[16], ctx->f[17]); }},
    {"AI_GetTimeFromAccelC3", 0x13d4540u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { AI_GetTimeFromAccelC3(rdram, ctx, runtime, ctx->f[12], ctx->f[13], ctx->f[14], ctx->f[15]); }},
    {"AI_GetVelocity3", 0x13d45f0u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { AI_GetVelocity3(rdram, ctx, runtime, ctx->f[12], ctx->f[13], ctx->f[14], ctx->f[15], ctx->f[16]); }},
    {"AI_GetAreaDistScale", 0x13d4ce0u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { AI_GetAreaDistScale(rdram, ctx, runtime, ctx->f[12], ctx->f[13]); }},
    {"AI_GetAreaDistScale2", 0x13d4d08u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { AI_GetAreaDistScale2(rdram, ctx, runtime, ctx->f[12], ctx->f[13]); }},
    {"AI_GetNegaVector", 0x13d5300u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { AI_GetNegaVector(rdram, ctx, runtime, (uint32_t)Ps2GprU32(ctx, 4), (uint32_t)Ps2GprU32(ctx, 5)); }},
    {"Kite_SetAnimalID_KitePathWork", 0x13e7c00u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { Kite_SetAnimalID_KitePathWork(rdram, ctx, runtime, (uint32_t)Ps2GprU32(ctx, 4), (uint32_t)Ps2GprU32(ctx, 5)); }},
    {"Kite_SetEventIDList", 0x13e7c08u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { Kite_SetEventIDList(rdram, ctx, runtime, (uint32_t)Ps2GprU32(ctx, 4), (uint32_t)Ps2GprU32(ctx, 5), (uint64_t)Ps2GprU64(ctx, 6)); }},
    {"Kite_InitEventCheck", 0x13e7c28u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { Kite_InitEventCheck(rdram, ctx, runtime, (uint32_t)Ps2GprU32(ctx, 4)); }},
    {"Kite_ResetEventFrameCheck", 0x13e7c40u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { Kite_ResetEventFrameCheck(rdram, ctx, runtime, (uint32_t)Ps2GprU32(ctx, 4)); }},
    {"Kite_StartEventCheck", 0x13e7c50u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { Kite_StartEventCheck(rdram, ctx, runtime, (uint32_t)Ps2GprU32(ctx, 4), (uint32_t)Ps2GprU32(ctx, 5)); }},
    {"Kite_EndEventCheck", 0x13e7c68u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { Kite_EndEventCheck(rdram, ctx, runtime, (uint32_t)Ps2GprU32(ctx, 4)); }},
    {"Kite_SucceedKitePathWork", 0x13e7cf8u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { Kite_SucceedKitePathWork(rdram, ctx, runtime, (uint32_t)Ps2GprU32(ctx, 4), (uint32_t)Ps2GprU32(ctx, 5)); }},
    {"setSmallManagerSystemLock", 0x13f0f90u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { setSmallManagerSystemLock(rdram, ctx, runtime, (uint32_t)Ps2GprU32(ctx, 4)); }},
    {"gamecore_sub_013F1190", 0x13f1190u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { gamecore_sub_013F1190(rdram, ctx, runtime, (uint32_t)Ps2GprU32(ctx, 4)); }},
    {"gamecore_sub_013F1938", 0x13f1938u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { gamecore_sub_013F1938(rdram, ctx, runtime, (uint32_t)Ps2GprU32(ctx, 4), (uint32_t)Ps2GprU32(ctx, 5)); }},
    {"getNowExecEventScriptObj", 0x13f62c8u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { getNowExecEventScriptObj(rdram, ctx, runtime); }},
    {"setEventScriptEnable", 0x13f62f0u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { setEventScriptEnable(rdram, ctx, runtime, (uint32_t)Ps2GprU32(ctx, 4)); }},
    {"setPlayerStatusSwitch", 0x13fa530u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { setPlayerStatusSwitch(rdram, ctx, runtime, (uint32_t)Ps2GprU32(ctx, 4)); }},
    {"startAttackEffect", 0x13fa628u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { startAttackEffect(rdram, ctx, runtime, ctx->f[12]); }},
    {"bosslifebar_display", 0x13fa930u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { bosslifebar_display(rdram, ctx, runtime); }},
    {"bosslifebar_resethp", 0x13fa968u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { bosslifebar_resethp(rdram, ctx, runtime, ctx->f[12], ctx->f[13]); }},
    {"PlayerSprStatusEnd", 0x13fab00u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { PlayerSprStatusEnd(rdram, ctx, runtime); }},
    {"BossSprStatusEnd", 0x13fab68u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { BossSprStatusEnd(rdram, ctx, runtime); }},
    {"GetSwordFlareHitPlaneAttribute", 0x13ff148u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { GetSwordFlareHitPlaneAttribute(rdram, ctx, runtime); }},
    {"getSelectWeapon", 0x1407310u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { getSelectWeapon(rdram, ctx, runtime); }},
    {"getSprLogWorkAddress", 0x140ae40u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { getSprLogWorkAddress(rdram, ctx, runtime); }},
    {"getSprLogStatusAddress", 0x140ae58u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { getSprLogStatusAddress(rdram, ctx, runtime); }},
    {"initPauseMenuMap", 0x140af68u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { initPauseMenuMap(rdram, ctx, runtime); }},
    {"setLoadMapOffset", 0x140afb0u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { setLoadMapOffset(rdram, ctx, runtime, (uint32_t)Ps2GprU32(ctx, 4), (uint32_t)Ps2GprU32(ctx, 5), ctx->f[12], ctx->f[13]); }},
    {"isInLoadWait", 0x140b0c0u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { isInLoadWait(rdram, ctx, runtime); }},
    {"LoadWaitCancel", 0x140b0d0u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { LoadWaitCancel(rdram, ctx, runtime, (uint32_t)Ps2GprU32(ctx, 4)); }},
    {"acted_camera_warp", 0x140ee50u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { acted_camera_warp(rdram, ctx, runtime, (uint64_t)Ps2GprU64(ctx, 4)); }},
    {"gamecore_sub_0140FE00", 0x140fe00u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { gamecore_sub_0140FE00(rdram, ctx, runtime, (uint32_t)Ps2GprU32(ctx, 4), (uint32_t)Ps2GprU32(ctx, 5), (uint32_t)Ps2GprU32(ctx, 6)); }},
    {"gamecore_sub_0140FE20", 0x140fe20u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { gamecore_sub_0140FE20(rdram, ctx, runtime, (uint32_t)Ps2GprU32(ctx, 4), (uint32_t)Ps2GprU32(ctx, 5), (uint32_t)Ps2GprU32(ctx, 6), (uint32_t)Ps2GprU32(ctx, 7)); }},
    {"gamecore_sub_0140FE50", 0x140fe50u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { gamecore_sub_0140FE50(rdram, ctx, runtime, (uint32_t)Ps2GprU32(ctx, 4), (uint32_t)Ps2GprU32(ctx, 5), (uint32_t)Ps2GprU32(ctx, 6)); }},
    {"gamecore_sub_0140FE70", 0x140fe70u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { gamecore_sub_0140FE70(rdram, ctx, runtime, ctx->f[12]); }},
    {"reset_time_interp_param_camera_dist_to_look_at", 0x1411448u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { reset_time_interp_param_camera_dist_to_look_at(rdram, ctx, runtime, (uint32_t)Ps2GprU32(ctx, 4)); }},
    {"is_acting_rewind_camera_dist_to_look_at", 0x1411480u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { is_acting_rewind_camera_dist_to_look_at(rdram, ctx, runtime, (uint32_t)Ps2GprU32(ctx, 4)); }},
    {"get_rewind_begin_dist_to_target_camera_dist_to_look_at", 0x1411490u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { get_rewind_begin_dist_to_target_camera_dist_to_look_at(rdram, ctx, runtime, (uint32_t)Ps2GprU32(ctx, 4), ctx->f[12]); }},
    {"end_rewind_camera_dist_to_look_at", 0x14114b0u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { end_rewind_camera_dist_to_look_at(rdram, ctx, runtime, (uint32_t)Ps2GprU32(ctx, 4)); }},
    {"is_leave_camera_dist_to_look_at", 0x14114b8u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { is_leave_camera_dist_to_look_at(rdram, ctx, runtime, (uint32_t)Ps2GprU32(ctx, 4)); }},
    {"request_end_camera_pause_rise", 0x1412a60u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { request_end_camera_pause_rise(rdram, ctx, runtime, (uint32_t)Ps2GprU32(ctx, 4)); }},
    {"is_end_camera_pause_rise", 0x1412a70u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { is_end_camera_pause_rise(rdram, ctx, runtime, (uint32_t)Ps2GprU32(ctx, 4)); }},
    {"get_begin_interp_finish_time_camera_pause_rise", 0x1412a80u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { get_begin_interp_finish_time_camera_pause_rise(rdram, ctx, runtime); }},
    {"get_place_status_camera_place_status", 0x1413340u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { get_place_status_camera_place_status(rdram, ctx, runtime, (uint32_t)Ps2GprU32(ctx, 4)); }},
    {"get_manage_obj_camera_place_status", 0x1413348u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { get_manage_obj_camera_place_status(rdram, ctx, runtime, (uint32_t)Ps2GprU32(ctx, 4)); }},
    {"get_hit_camera_region_camera_place_status", 0x1413408u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { get_hit_camera_region_camera_place_status(rdram, ctx, runtime, (uint32_t)Ps2GprU32(ctx, 4)); }},
    {"get_on_boss_camera_col_attr_camera_place_status", 0x14134b8u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { get_on_boss_camera_col_attr_camera_place_status(rdram, ctx, runtime, (uint32_t)Ps2GprU32(ctx, 4), (uint32_t)Ps2GprU32(ctx, 5)); }},
    {"is_approach_rise_ok_camera_place_status", 0x14134d0u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { is_approach_rise_ok_camera_place_status(rdram, ctx, runtime, (uint64_t)Ps2GprU64(ctx, 4)); }},
    {"is_cliff_rise_delay_camera_place_status", 0x14135b8u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { is_cliff_rise_delay_camera_place_status(rdram, ctx, runtime, (uint64_t)Ps2GprU64(ctx, 4)); }},
    {"act_horse_look_at_ofst_decrease_camera_place_status", 0x1413668u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { act_horse_look_at_ofst_decrease_camera_place_status(rdram, ctx, runtime, (uint64_t)Ps2GprU64(ctx, 4)); }},
    {"act_look_at_ofst_cut_camera_place_status", 0x14136b8u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { act_look_at_ofst_cut_camera_place_status(rdram, ctx, runtime, (uint64_t)Ps2GprU64(ctx, 4)); }},
    {"gamecore_sub_01413708", 0x1413708u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { gamecore_sub_01413708(rdram, ctx, runtime, (uint32_t)Ps2GprU32(ctx, 4)); }},
    {"CAM_get_interp_slope_pitch_ofst", 0x1414200u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { CAM_get_interp_slope_pitch_ofst(rdram, ctx, runtime, (uint32_t)Ps2GprU32(ctx, 4)); }},
    {"gamecore_sub_014144D0", 0x14144d0u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { gamecore_sub_014144D0(rdram, ctx, runtime, ctx->f[12], ctx->f[13], ctx->f[14]); }},
    {"get_attr_camera_target_lost_angle", 0x1414e60u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { get_attr_camera_target_lost_angle(rdram, ctx, runtime, (uint32_t)Ps2GprU32(ctx, 4)); }},
    {"set_parameter_camera_target_visible_angle_manager", 0x1415900u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { set_parameter_camera_target_visible_angle_manager(rdram, ctx, runtime, (uint32_t)Ps2GprU32(ctx, 4), (uint32_t)Ps2GprU32(ctx, 5), ctx->f[12], ctx->f[13]); }},
    {"end_get_yaw_pitch_camera_target_visible_angle_manager", 0x1415a40u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { end_get_yaw_pitch_camera_target_visible_angle_manager(rdram, ctx, runtime, (uint32_t)Ps2GprU32(ctx, 4)); }},
    {"gamecore_sub_01415CA0", 0x1415ca0u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { gamecore_sub_01415CA0(rdram, ctx, runtime, (uint32_t)Ps2GprU32(ctx, 4), (uint32_t)Ps2GprU32(ctx, 5)); }},
    {"CAM_copy_camera_base_data", 0x141a108u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { CAM_copy_camera_base_data(rdram, ctx, runtime, (uint32_t)Ps2GprU32(ctx, 4), (uint32_t)Ps2GprU32(ctx, 5)); }},
    {"CAM_interp_reset_aim_rot_common", 0x141a160u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { CAM_interp_reset_aim_rot_common(rdram, ctx, runtime, (uint32_t)Ps2GprU32(ctx, 4)); }},
    {"CAM_motion_kind_is_look_around_motion_target", 0x141a658u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { CAM_motion_kind_is_look_around_motion_target(rdram, ctx, runtime, (uint32_t)Ps2GprU32(ctx, 4)); }},
    {"CAM_motion_kind_is_accept_auto_rot_motion_target", 0x141a678u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { CAM_motion_kind_is_accept_auto_rot_motion_target(rdram, ctx, runtime, (uint32_t)Ps2GprU32(ctx, 4)); }},
    {"CAM_get_target_obj", 0x141a7b8u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { CAM_get_target_obj(rdram, ctx, runtime, (uint32_t)Ps2GprU32(ctx, 4)); }},
    {"CAM_get_target_riding_obj", 0x141a7c0u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { CAM_get_target_riding_obj(rdram, ctx, runtime, (uint32_t)Ps2GprU32(ctx, 4)); }},
    {"CAM_get_ground_contact_obj", 0x141a7c8u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { CAM_get_ground_contact_obj(rdram, ctx, runtime, (uint32_t)Ps2GprU32(ctx, 4)); }},
    {"CAM_is_visible_now_target_visibility", 0x141aa80u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { CAM_is_visible_now_target_visibility(rdram, ctx, runtime, (uint32_t)Ps2GprU32(ctx, 4)); }},
    {"CAM_is_return_cam_posi_target_visibility", 0x141aa90u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { CAM_is_return_cam_posi_target_visibility(rdram, ctx, runtime, (uint32_t)Ps2GprU32(ctx, 4)); }},
    {"CAM_reset_camera_warp_cntrl", 0x141aad0u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { CAM_reset_camera_warp_cntrl(rdram, ctx, runtime, (uint32_t)Ps2GprU32(ctx, 4)); }},
    {"CAM_set_invisible_warp_apply_range_camera_warp_cntrl", 0x141aca8u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { CAM_set_invisible_warp_apply_range_camera_warp_cntrl(rdram, ctx, runtime, (uint32_t)Ps2GprU32(ctx, 4), ctx->f[12], ctx->f[13]); }},
    {"CAM_act_warp_order_camera_warp_cntrl", 0x141acb8u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { CAM_act_warp_order_camera_warp_cntrl(rdram, ctx, runtime, (uint32_t)Ps2GprU32(ctx, 4)); }},
    {"CAM_is_acting_view_cut_avoid_cntrl", 0x141b010u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { CAM_is_acting_view_cut_avoid_cntrl(rdram, ctx, runtime, (uint32_t)Ps2GprU32(ctx, 4)); }},
    {"CAM_set_param_view_cut_avoid_cntrl", 0x141b020u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { CAM_set_param_view_cut_avoid_cntrl(rdram, ctx, runtime, (uint32_t)Ps2GprU32(ctx, 4), ctx->f[12], ctx->f[13]); }},
    {"CAM_reset_obstacle_avoid_cntrl", 0x141b030u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { CAM_reset_obstacle_avoid_cntrl(rdram, ctx, runtime, (uint32_t)Ps2GprU32(ctx, 4)); }},
    {"gamecore_sub_0141B428", 0x141b428u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { gamecore_sub_0141B428(rdram, ctx, runtime, ctx->f[12], ctx->f[13]); }},
    {"gamecore_sub_0141B480", 0x141b480u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { gamecore_sub_0141B480(rdram, ctx, runtime, (uint32_t)Ps2GprU32(ctx, 4)); }},
    {"gamecore_sub_0141B978", 0x141b978u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { gamecore_sub_0141B978(rdram, ctx, runtime, (uint32_t)Ps2GprU32(ctx, 4)); }},
    {"gamecore_sub_0141B988", 0x141b988u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { gamecore_sub_0141B988(rdram, ctx, runtime, (uint32_t)Ps2GprU32(ctx, 4)); }},
    {"gamecore_sub_0141BB20", 0x141bb20u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { gamecore_sub_0141BB20(rdram, ctx, runtime, (uint32_t)Ps2GprU32(ctx, 4), (uint32_t)Ps2GprU32(ctx, 5), ctx->f[12]); }},
    {"gamecore_sub_0141BCF8", 0x141bcf8u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { gamecore_sub_0141BCF8(rdram, ctx, runtime, (uint32_t)Ps2GprU32(ctx, 5)); }},
    {"gamecore_sub_0141C0A8", 0x141c0a8u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { gamecore_sub_0141C0A8(rdram, ctx, runtime, ctx->f[12], ctx->f[13], ctx->f[14]); }},
    {"get_view_angle_camera_view_angle_cntrl", 0x141c9b0u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { get_view_angle_camera_view_angle_cntrl(rdram, ctx, runtime, (uint32_t)Ps2GprU32(ctx, 4)); }},
    {"get_camera_obj", 0x141cd68u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { get_camera_obj(rdram, ctx, runtime, (uint32_t)Ps2GprU32(ctx, 4)); }},
    {"get_player_collision_group_cameraman", 0x141ce48u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { get_player_collision_group_cameraman(rdram, ctx, runtime, (uint32_t)Ps2GprU32(ctx, 4)); }},
    {"is_camera_target_change", 0x141ce80u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { is_camera_target_change(rdram, ctx, runtime, (uint32_t)Ps2GprU32(ctx, 4)); }},
    {"is_camera_target_debug_move", 0x141ce98u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { is_camera_target_debug_move(rdram, ctx, runtime); }},
    {"get_main_cameraman", 0x141d370u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { get_main_cameraman(rdram, ctx, runtime); }},
    {"reset_game_camera", 0x1424e18u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { reset_game_camera(rdram, ctx, runtime, (uint32_t)Ps2GprU32(ctx, 4)); }},
    {"get_game_camera_cur_mode", 0x1424e28u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { get_game_camera_cur_mode(rdram, ctx, runtime, (uint32_t)Ps2GprU32(ctx, 4)); }},
    {"is_reset_or_lws_movie_game_camera", 0x1424e30u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { is_reset_or_lws_movie_game_camera(rdram, ctx, runtime, (uint32_t)Ps2GprU32(ctx, 4)); }},
    {"reset_game_camera_standby_flag", 0x1425028u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { reset_game_camera_standby_flag(rdram, ctx, runtime, (uint32_t)Ps2GprU32(ctx, 4)); }},
    {"set_object_interest_mode_target_game_camera", 0x1425030u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { set_object_interest_mode_target_game_camera(rdram, ctx, runtime, (uint32_t)Ps2GprU32(ctx, 4), (uint32_t)Ps2GprU32(ctx, 5)); }},
    {"get_object_interest_mode_target_game_camera", 0x1425038u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { get_object_interest_mode_target_game_camera(rdram, ctx, runtime, (uint32_t)Ps2GprU32(ctx, 4)); }},
    {"begin_object_interest_game_camera", 0x1425040u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { begin_object_interest_game_camera(rdram, ctx, runtime, (uint32_t)Ps2GprU32(ctx, 4)); }},
    {"begin_stream_lws_movie_playing_game_camera", 0x14252d8u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { begin_stream_lws_movie_playing_game_camera(rdram, ctx, runtime, (uint32_t)Ps2GprU32(ctx, 4)); }},
    {"end_stream_lws_movie_playing_game_camera", 0x14252e8u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { end_stream_lws_movie_playing_game_camera(rdram, ctx, runtime, (uint32_t)Ps2GprU32(ctx, 4)); }},
    {"is_view_angle_exist_lws_anime_mode_base", 0x1425548u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { is_view_angle_exist_lws_anime_mode_base(rdram, ctx, runtime, (uint32_t)Ps2GprU32(ctx, 4)); }},
    {"get_view_angle_lws_anime_mode_base", 0x1425568u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { get_view_angle_lws_anime_mode_base(rdram, ctx, runtime, (uint32_t)Ps2GprU32(ctx, 4)); }},
    {"getBossInterestKey", 0x14256a0u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { getBossInterestKey(rdram, ctx, runtime); }},
    {"getInterestKey", 0x14256b0u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { getInterestKey(rdram, ctx, runtime); }},
    {"getZoomKey", 0x14256c0u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { getZoomKey(rdram, ctx, runtime); }},
    {"setBossInterestKey", 0x14256d0u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { setBossInterestKey(rdram, ctx, runtime, (uint32_t)Ps2GprU32(ctx, 4)); }},
    {"setInterestKey", 0x14256e0u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { setInterestKey(rdram, ctx, runtime, (uint32_t)Ps2GprU32(ctx, 4)); }},
    {"setZoomKey", 0x14256f0u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { setZoomKey(rdram, ctx, runtime, (uint32_t)Ps2GprU32(ctx, 4)); }},
    {"gamecore_sub_01425700", 0x1425700u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { gamecore_sub_01425700(rdram, ctx, runtime, (uint32_t)Ps2GprU32(ctx, 4), (uint64_t)Ps2GprU64(ctx, 5)); }},
    {"gamecore_sub_01425788", 0x1425788u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { gamecore_sub_01425788(rdram, ctx, runtime, (uint32_t)Ps2GprU32(ctx, 4)); }},
    {"gamecore_sub_014257A8", 0x14257a8u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { gamecore_sub_014257A8(rdram, ctx, runtime, (uint32_t)Ps2GprU32(ctx, 4)); }},
    {"gamecore_sub_01425AC0", 0x1425ac0u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { gamecore_sub_01425AC0(rdram, ctx, runtime, (uint32_t)Ps2GprU32(ctx, 4), (uint32_t)Ps2GprU32(ctx, 5)); }},
    {"gamecore_sub_014262A0", 0x14262a0u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { gamecore_sub_014262A0(rdram, ctx, runtime, (uint32_t)Ps2GprU32(ctx, 5)); }},
    {"gamecore_sub_01426B18", 0x1426b18u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { gamecore_sub_01426B18(rdram, ctx, runtime, (uint32_t)Ps2GprU32(ctx, 4)); }},
    {"gamecore_sub_01426B20", 0x1426b20u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { gamecore_sub_01426B20(rdram, ctx, runtime, (uint32_t)Ps2GprU32(ctx, 4)); }},
    {"gamecore_sub_01427230", 0x1427230u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { gamecore_sub_01427230(rdram, ctx, runtime, (uint64_t)Ps2GprU64(ctx, 4)); }},
    {"gamecore_sub_01428CD0", 0x1428cd0u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { gamecore_sub_01428CD0(rdram, ctx, runtime, (uint32_t)Ps2GprU32(ctx, 4), (uint32_t)Ps2GprU32(ctx, 5), (uint32_t)Ps2GprU32(ctx, 6), ctx->f[12], ctx->f[13]); }},
    {"gamecore_sub_014296F8", 0x14296f8u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { gamecore_sub_014296F8(rdram, ctx, runtime, (uint32_t)Ps2GprU32(ctx, 4), (uint32_t)Ps2GprU32(ctx, 5), (uint32_t)Ps2GprU32(ctx, 6), ctx->f[12], ctx->f[13]); }},
    {"gamecore_sub_01429BC0", 0x1429bc0u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { gamecore_sub_01429BC0(rdram, ctx, runtime, (uint32_t)Ps2GprU32(ctx, 4), (uint32_t)Ps2GprU32(ctx, 5), (uint32_t)Ps2GprU32(ctx, 6), ctx->f[12], ctx->f[13]); }},
    {"gamecore_sub_0142B968", 0x142b968u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { gamecore_sub_0142B968(rdram, ctx, runtime, (uint32_t)Ps2GprU32(ctx, 4), (uint32_t)Ps2GprU32(ctx, 5), (uint32_t)Ps2GprU32(ctx, 6), ctx->f[12], ctx->f[13]); }},
    {"is_base_mode_manager_update_game_camera_param_manager", 0x142c118u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { is_base_mode_manager_update_game_camera_param_manager(rdram, ctx, runtime, (uint32_t)Ps2GprU32(ctx, 4)); }},
    {"get_interp_time_game_camera_param_manager", 0x142c128u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { get_interp_time_game_camera_param_manager(rdram, ctx, runtime, (uint32_t)Ps2GprU32(ctx, 4)); }},
    {"act_camera_cutout_game_camera_param_manager", 0x142c248u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { act_camera_cutout_game_camera_param_manager(rdram, ctx, runtime, (uint32_t)Ps2GprU32(ctx, 4), (uint32_t)Ps2GprU32(ctx, 5)); }},
    {"drop_camera_cutout_flag_game_camera_param_manager", 0x142c270u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { drop_camera_cutout_flag_game_camera_param_manager(rdram, ctx, runtime, (uint32_t)Ps2GprU32(ctx, 4)); }},
    {"constructor_game_camera_param_update_data_manager", 0x142c278u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { constructor_game_camera_param_update_data_manager(rdram, ctx, runtime, (uint32_t)Ps2GprU32(ctx, 4)); }},
    {"is_already_set_game_camera_param_update_data_manager", 0x142c398u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { is_already_set_game_camera_param_update_data_manager(rdram, ctx, runtime, (uint32_t)Ps2GprU32(ctx, 4), (uint32_t)Ps2GprU32(ctx, 5)); }},
    {"constructor_game_camera_mode_local_parameter", 0x142c3b8u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { constructor_game_camera_mode_local_parameter(rdram, ctx, runtime, (uint32_t)Ps2GprU32(ctx, 4)); }},
    {"gamecore_sub_0142C440", 0x142c440u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { gamecore_sub_0142C440(rdram, ctx, runtime, (uint32_t)Ps2GprU32(ctx, 4), (uint32_t)Ps2GprU32(ctx, 5), (uint32_t)Ps2GprU32(ctx, 6), ctx->f[12], ctx->f[13]); }},
    {"gamecore_sub_0142C530", 0x142c530u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { gamecore_sub_0142C530(rdram, ctx, runtime, (uint32_t)Ps2GprU32(ctx, 4), (uint64_t)Ps2GprU64(ctx, 5)); }},
    {"setBossPathColiGroup", 0x1431c38u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { setBossPathColiGroup(rdram, ctx, runtime, (uint32_t)Ps2GprU32(ctx, 4)); }},
    {"setHorsePathColiGroup", 0x1431c48u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { setHorsePathColiGroup(rdram, ctx, runtime, (uint32_t)Ps2GprU32(ctx, 4)); }},
    {"getPathColiGroupByType", 0x1431c70u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { getPathColiGroupByType(rdram, ctx, runtime, (uint64_t)Ps2GprU64(ctx, 4)); }},
    {"getPassageWork", 0x1431ce0u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { getPassageWork(rdram, ctx, runtime, (uint32_t)Ps2GprU32(ctx, 4), (uint64_t)Ps2GprU64(ctx, 5)); }},
    {"getPassageNum", 0x1431d18u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { getPassageNum(rdram, ctx, runtime, (uint32_t)Ps2GprU32(ctx, 4)); }},
    {"setRealColJudgeFunc", 0x1431ed0u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { setRealColJudgeFunc(rdram, ctx, runtime, (uint32_t)Ps2GprU32(ctx, 4), (uint32_t)Ps2GprU32(ctx, 5)); }},
    {"getRealColJudgeFunc", 0x1431ed8u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { getRealColJudgeFunc(rdram, ctx, runtime, (uint32_t)Ps2GprU32(ctx, 4)); }},
    {"setSelfColAndNearMinDist", 0x1431ee0u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { setSelfColAndNearMinDist(rdram, ctx, runtime, (uint32_t)Ps2GprU32(ctx, 4), (uint32_t)Ps2GprU32(ctx, 5), ctx->f[12]); }},
    {"setStabilizeYPathWork", 0x1431f38u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { setStabilizeYPathWork(rdram, ctx, runtime, (uint32_t)Ps2GprU32(ctx, 4), (uint32_t)Ps2GprU32(ctx, 5)); }},
    {"setAngleStabilizeYPathWork", 0x1431f40u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { setAngleStabilizeYPathWork(rdram, ctx, runtime, (uint32_t)Ps2GprU32(ctx, 4), (uint32_t)Ps2GprU32(ctx, 5)); }},
    {"setRayByRealColPathWork", 0x1431f48u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { setRayByRealColPathWork(rdram, ctx, runtime, (uint32_t)Ps2GprU32(ctx, 4), (uint32_t)Ps2GprU32(ctx, 5)); }},
    {"setPassageWideRayPathWork", 0x1431f50u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { setPassageWideRayPathWork(rdram, ctx, runtime, (uint32_t)Ps2GprU32(ctx, 4), (uint32_t)Ps2GprU32(ctx, 5)); }},
    {"setExecRayChkPathWork", 0x1431f58u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { setExecRayChkPathWork(rdram, ctx, runtime, (uint32_t)Ps2GprU32(ctx, 4), (uint32_t)Ps2GprU32(ctx, 5)); }},
    {"setForceRouteAttrPathWork", 0x1431fa8u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { setForceRouteAttrPathWork(rdram, ctx, runtime, (uint32_t)Ps2GprU32(ctx, 4), (uint32_t)Ps2GprU32(ctx, 5)); }},
    {"setRouteColCheckPathWork", 0x1431fb0u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { setRouteColCheckPathWork(rdram, ctx, runtime, (uint32_t)Ps2GprU32(ctx, 4), (uint32_t)Ps2GprU32(ctx, 5)); }},
    {"setRouteLockPathWork", 0x1431fb8u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { setRouteLockPathWork(rdram, ctx, runtime, (uint32_t)Ps2GprU32(ctx, 4), (uint32_t)Ps2GprU32(ctx, 5)); }},
    {"setPlayerWayColCheckPathWork", 0x1431fc0u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { setPlayerWayColCheckPathWork(rdram, ctx, runtime, (uint32_t)Ps2GprU32(ctx, 4), (uint32_t)Ps2GprU32(ctx, 5)); }},
    {"setFollowModePathWork", 0x1431fc8u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { setFollowModePathWork(rdram, ctx, runtime, (uint32_t)Ps2GprU32(ctx, 4), (uint32_t)Ps2GprU32(ctx, 5)); }},
    {"setUserPathMode", 0x1432018u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { setUserPathMode(rdram, ctx, runtime, (uint32_t)Ps2GprU32(ctx, 4), (uint32_t)Ps2GprU32(ctx, 5)); }},
    {"setPassageRangeData", 0x1432020u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { setPassageRangeData(rdram, ctx, runtime, (uint32_t)Ps2GprU32(ctx, 4), ctx->f[12], ctx->f[13]); }},
    {"setAttainRangeStruct", 0x1432050u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { setAttainRangeStruct(rdram, ctx, runtime, (uint32_t)Ps2GprU32(ctx, 4), (uint32_t)Ps2GprU32(ctx, 5)); }},
    {"setCreateTempAttain", 0x14320b0u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { setCreateTempAttain(rdram, ctx, runtime, (uint32_t)Ps2GprU32(ctx, 4), (uint32_t)Ps2GprU32(ctx, 5)); }},
    {"setAttainModePathWork", 0x1432700u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { setAttainModePathWork(rdram, ctx, runtime, (uint32_t)Ps2GprU32(ctx, 4), (uint32_t)Ps2GprU32(ctx, 5)); }},
    {"setAttainRadPathWork", 0x1432708u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { setAttainRadPathWork(rdram, ctx, runtime, (uint32_t)Ps2GprU32(ctx, 4), ctx->f[12]); }},
    {"getPathInfo", 0x1432710u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { getPathInfo(rdram, ctx, runtime, (uint32_t)Ps2GprU32(ctx, 4)); }},
    {"getWayFinish", 0x1432740u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { getWayFinish(rdram, ctx, runtime, (uint32_t)Ps2GprU32(ctx, 4)); }},
    {"getPathStatus", 0x1432760u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { getPathStatus(rdram, ctx, runtime, (uint32_t)Ps2GprU32(ctx, 4)); }},
    {"getLastPassagePointIdx", 0x1432808u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { getLastPassagePointIdx(rdram, ctx, runtime, (uint32_t)Ps2GprU32(ctx, 4)); }},
    {"getLastPassageAreaIdx", 0x1432810u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { getLastPassageAreaIdx(rdram, ctx, runtime, (uint32_t)Ps2GprU32(ctx, 4)); }},
    {"getNowPassagePointIdx", 0x1432818u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { getNowPassagePointIdx(rdram, ctx, runtime, (uint32_t)Ps2GprU32(ctx, 4)); }},
    {"getNowPassageAreaIdx", 0x1432820u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { getNowPassageAreaIdx(rdram, ctx, runtime, (uint32_t)Ps2GprU32(ctx, 4)); }},
    {"setSearchRadius", 0x14328a8u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { setSearchRadius(rdram, ctx, runtime, (uint32_t)Ps2GprU32(ctx, 4), ctx->f[12]); }},
    {"setSearchPointMax", 0x14328d0u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { setSearchPointMax(rdram, ctx, runtime, (uint32_t)Ps2GprU32(ctx, 4), (uint64_t)Ps2GprU64(ctx, 5)); }},
    {"setPathRayWideParam", 0x14328e8u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { setPathRayWideParam(rdram, ctx, runtime, (uint32_t)Ps2GprU32(ctx, 4), (uint32_t)Ps2GprU32(ctx, 5), ctx->f[12]); }},
    {"resetPassageWork", 0x1433120u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { resetPassageWork(rdram, ctx, runtime, (uint32_t)Ps2GprU32(ctx, 4)); }},
    {"getColiGroupByUserPathWork", 0x1433f60u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { getColiGroupByUserPathWork(rdram, ctx, runtime, (uint32_t)Ps2GprU32(ctx, 4)); }},
    {"gamecore_sub_01434CF8", 0x1434cf8u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { gamecore_sub_01434CF8(rdram, ctx, runtime, (uint64_t)Ps2GprU64(ctx, 4)); }},
    {"gamecore_sub_01434F90", 0x1434f90u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { gamecore_sub_01434F90(rdram, ctx, runtime, (uint32_t)Ps2GprU32(ctx, 4)); }},
    {"setUserPathExMode", 0x14359c8u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { setUserPathExMode(rdram, ctx, runtime, (uint32_t)Ps2GprU32(ctx, 4), (uint32_t)Ps2GprU32(ctx, 5)); }},
    {"setUserPathGroupID", 0x14359d0u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { setUserPathGroupID(rdram, ctx, runtime, (uint32_t)Ps2GprU32(ctx, 4), (uint32_t)Ps2GprU32(ctx, 5), (uint32_t)Ps2GprU32(ctx, 6)); }},
    {"setGPSelectMode", 0x1435a50u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { setGPSelectMode(rdram, ctx, runtime, (uint32_t)Ps2GprU32(ctx, 4), (uint32_t)Ps2GprU32(ctx, 5)); }},
    {"setForceInitPathEx", 0x1435a58u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { setForceInitPathEx(rdram, ctx, runtime, (uint32_t)Ps2GprU32(ctx, 4), (uint32_t)Ps2GprU32(ctx, 5)); }},
    {"setGpReferencePathEx", 0x1435a60u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { setGpReferencePathEx(rdram, ctx, runtime, (uint32_t)Ps2GprU32(ctx, 4), (uint32_t)Ps2GprU32(ctx, 5)); }},
    {"gamecore_sub_01435EF0", 0x1435ef0u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { gamecore_sub_01435EF0(rdram, ctx, runtime, (uint32_t)Ps2GprU32(ctx, 4)); }},
    {"_setExecBaseMoveWork", 0x1437a60u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { _setExecBaseMoveWork(rdram, ctx, runtime, (uint32_t)Ps2GprU32(ctx, 4), (uint32_t)Ps2GprU32(ctx, 5)); }},
    {"_setStopCtrlModeBaseMoveWork", 0x1437a68u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { _setStopCtrlModeBaseMoveWork(rdram, ctx, runtime, (uint32_t)Ps2GprU32(ctx, 4), (uint32_t)Ps2GprU32(ctx, 5)); }},
    {"_getStopCtrlModeBaseMoveWork", 0x1437a70u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { _getStopCtrlModeBaseMoveWork(rdram, ctx, runtime, (uint32_t)Ps2GprU32(ctx, 4)); }},
    {"_setBackVolumeBaseMoveWork", 0x1437a78u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { _setBackVolumeBaseMoveWork(rdram, ctx, runtime, (uint32_t)Ps2GprU32(ctx, 4), (uint32_t)Ps2GprU32(ctx, 5)); }},
    {"_setSlideVolumeBaseMoveWork", 0x1437a80u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { _setSlideVolumeBaseMoveWork(rdram, ctx, runtime, (uint32_t)Ps2GprU32(ctx, 4), (uint32_t)Ps2GprU32(ctx, 5)); }},
    {"_setAttainFlagBaseMoveWork", 0x1437b88u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { _setAttainFlagBaseMoveWork(rdram, ctx, runtime, (uint32_t)Ps2GprU32(ctx, 4), (uint32_t)Ps2GprU32(ctx, 5)); }},
    {"gamecore_sub_01438448", 0x1438448u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { gamecore_sub_01438448(rdram, ctx, runtime, (uint32_t)Ps2GprU32(ctx, 4)); }},
    {"resetRouteAttackBirdCtrlWork", 0x143d5c8u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { resetRouteAttackBirdCtrlWork(rdram, ctx, runtime, (uint32_t)Ps2GprU32(ctx, 4)); }},
    {"setFlapWingBirdCtrlWork", 0x143d5d8u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { setFlapWingBirdCtrlWork(rdram, ctx, runtime, (uint32_t)Ps2GprU32(ctx, 4), (uint32_t)Ps2GprU32(ctx, 5)); }},
    {"setHoveringBirdCtrlWork", 0x143d5e0u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { setHoveringBirdCtrlWork(rdram, ctx, runtime, (uint32_t)Ps2GprU32(ctx, 4), (uint32_t)Ps2GprU32(ctx, 5)); }},
    {"setHoverAttackBirdCtrlWork", 0x143d5e8u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { setHoverAttackBirdCtrlWork(rdram, ctx, runtime, (uint32_t)Ps2GprU32(ctx, 4), (uint32_t)Ps2GprU32(ctx, 5)); }},
    {"gamecore_sub_0143D660", 0x143d660u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { gamecore_sub_0143D660(rdram, ctx, runtime, (uint32_t)Ps2GprU32(ctx, 4)); }},
    {"setDashFastLegCtrlWork", 0x1440ef0u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { setDashFastLegCtrlWork(rdram, ctx, runtime, (uint32_t)Ps2GprU32(ctx, 4), (uint32_t)Ps2GprU32(ctx, 5)); }},
    {"setFinalAttainWalkFastLegCtrlWork", 0x1440ef8u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { setFinalAttainWalkFastLegCtrlWork(rdram, ctx, runtime, (uint32_t)Ps2GprU32(ctx, 4), (uint32_t)Ps2GprU32(ctx, 5)); }},
    {"setAllowMaxSpeedFastLegCtrlWork", 0x1440f00u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { setAllowMaxSpeedFastLegCtrlWork(rdram, ctx, runtime, (uint32_t)Ps2GprU32(ctx, 4), ctx->f[12]); }},
    {"setBackFastLegCtrlWork", 0x1440f18u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { setBackFastLegCtrlWork(rdram, ctx, runtime, (uint32_t)Ps2GprU32(ctx, 4), (uint32_t)Ps2GprU32(ctx, 5)); }},
    {"setNotFastTurnFastLegCtrlWork", 0x1440f20u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { setNotFastTurnFastLegCtrlWork(rdram, ctx, runtime, (uint32_t)Ps2GprU32(ctx, 4), (uint32_t)Ps2GprU32(ctx, 5)); }},
    {"setRunStopTurnFastLegCtrlWork", 0x1440f28u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { setRunStopTurnFastLegCtrlWork(rdram, ctx, runtime, (uint32_t)Ps2GprU32(ctx, 4), (uint32_t)Ps2GprU32(ctx, 5)); }},
    {"setWalkStopTurnFastLegCtrlWork", 0x1440f30u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { setWalkStopTurnFastLegCtrlWork(rdram, ctx, runtime, (uint32_t)Ps2GprU32(ctx, 4), (uint32_t)Ps2GprU32(ctx, 5)); }},
    {"setCurveCollisionFastLegCtrlWork", 0x1440f38u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { setCurveCollisionFastLegCtrlWork(rdram, ctx, runtime, (uint32_t)Ps2GprU32(ctx, 4), (uint32_t)Ps2GprU32(ctx, 5)); }},
    {"setExecGazeCtrlWork", 0x14418d0u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { setExecGazeCtrlWork(rdram, ctx, runtime, (uint32_t)Ps2GprU32(ctx, 4), (uint32_t)Ps2GprU32(ctx, 5)); }},
    {"setViewJudgeGazeCtrlWork", 0x14418d8u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { setViewJudgeGazeCtrlWork(rdram, ctx, runtime, (uint32_t)Ps2GprU32(ctx, 4), (uint32_t)Ps2GprU32(ctx, 5)); }},
    {"setNextDistGazeCtrlWork", 0x14418e0u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { setNextDistGazeCtrlWork(rdram, ctx, runtime, (uint32_t)Ps2GprU32(ctx, 4), ctx->f[12]); }},
    {"setValidPosGazeCtrlWork", 0x14418e8u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { setValidPosGazeCtrlWork(rdram, ctx, runtime, (uint32_t)Ps2GprU32(ctx, 4), (uint32_t)Ps2GprU32(ctx, 5)); }},
    {"setBirdModeGazeCtrlWork", 0x1441968u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { setBirdModeGazeCtrlWork(rdram, ctx, runtime, (uint32_t)Ps2GprU32(ctx, 4), (uint32_t)Ps2GprU32(ctx, 5)); }},
    {"setGazeCoffGazeCtrlWork", 0x1441970u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { setGazeCoffGazeCtrlWork(rdram, ctx, runtime, (uint32_t)Ps2GprU32(ctx, 4), ctx->f[12]); }},
    {"setDashHorseCtrlWork", 0x14444d0u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { setDashHorseCtrlWork(rdram, ctx, runtime, (uint32_t)Ps2GprU32(ctx, 4), (uint32_t)Ps2GprU32(ctx, 5)); }},
    {"setWallStopHorseCtrlWork", 0x14444d8u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { setWallStopHorseCtrlWork(rdram, ctx, runtime, (uint32_t)Ps2GprU32(ctx, 4), (uint32_t)Ps2GprU32(ctx, 5)); }},
    {"setLimitModeHorseCtrlWork", 0x14444e0u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { setLimitModeHorseCtrlWork(rdram, ctx, runtime, (uint32_t)Ps2GprU32(ctx, 4), (uint32_t)Ps2GprU32(ctx, 5)); }},
    {"setFinalAttainWalkHorseCtrlWork", 0x14444e8u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { setFinalAttainWalkHorseCtrlWork(rdram, ctx, runtime, (uint32_t)Ps2GprU32(ctx, 4), (uint32_t)Ps2GprU32(ctx, 5)); }},
    {"setAllowMaxSpeedHorseCtrlWork", 0x14444f0u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { setAllowMaxSpeedHorseCtrlWork(rdram, ctx, runtime, (uint32_t)Ps2GprU32(ctx, 4), ctx->f[12]); }},
    {"setFreezeHorseCtrlWork", 0x1444510u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { setFreezeHorseCtrlWork(rdram, ctx, runtime, (uint32_t)Ps2GprU32(ctx, 4), (uint32_t)Ps2GprU32(ctx, 5)); }},
    {"chkMoveAngle", 0x14460a8u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { chkMoveAngle(rdram, ctx, runtime, ctx->f[12], ctx->f[13]); }},
    {"adjustAngleForLimit", 0x1446868u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { adjustAngleForLimit(rdram, ctx, runtime, ctx->f[12], ctx->f[13]); }},
    {"setDashSlowLegCtrlWork", 0x1449810u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { setDashSlowLegCtrlWork(rdram, ctx, runtime, (uint32_t)Ps2GprU32(ctx, 4), (uint32_t)Ps2GprU32(ctx, 5)); }},
    {"setStabilizeYSlowLegCtrlWork", 0x1449818u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { setStabilizeYSlowLegCtrlWork(rdram, ctx, runtime, (uint32_t)Ps2GprU32(ctx, 4), (uint32_t)Ps2GprU32(ctx, 5)); }},
    {"setMoveModeSlowLegCtrlWork", 0x1449820u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { setMoveModeSlowLegCtrlWork(rdram, ctx, runtime, (uint32_t)Ps2GprU32(ctx, 4), (uint32_t)Ps2GprU32(ctx, 5)); }},
    {"setMotScaleMaxSlowLegCtrlWork", 0x1449828u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { setMotScaleMaxSlowLegCtrlWork(rdram, ctx, runtime, (uint32_t)Ps2GprU32(ctx, 4), ctx->f[12]); }},
    {"setGuardModeSlowLegCtrlWork", 0x1449838u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { setGuardModeSlowLegCtrlWork(rdram, ctx, runtime, (uint32_t)Ps2GprU32(ctx, 4), (uint32_t)Ps2GprU32(ctx, 5)); }},
    {"gamecore_sub_014498B8", 0x14498b8u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { gamecore_sub_014498B8(rdram, ctx, runtime, (uint32_t)Ps2GprU32(ctx, 4)); }},
    {"setAttainSpeedWormCtrlWork", 0x144cc60u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { setAttainSpeedWormCtrlWork(rdram, ctx, runtime, (uint32_t)Ps2GprU32(ctx, 4), ctx->f[12]); }},
    {"setAttackParamWormCtrlWork", 0x144cc98u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { setAttackParamWormCtrlWork(rdram, ctx, runtime, (uint32_t)Ps2GprU32(ctx, 4), ctx->f[12]); }},
    {"setRotParamWormCtrlWork", 0x144cd18u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { setRotParamWormCtrlWork(rdram, ctx, runtime, (uint32_t)Ps2GprU32(ctx, 4), ctx->f[12], ctx->f[13]); }},
    {"setChkKeepHitWormCtrlWork", 0x144cd38u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { setChkKeepHitWormCtrlWork(rdram, ctx, runtime, (uint32_t)Ps2GprU32(ctx, 4), (uint32_t)Ps2GprU32(ctx, 5)); }},
    {"setRotColCheckWormCtrlWork", 0x144cd40u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { setRotColCheckWormCtrlWork(rdram, ctx, runtime, (uint32_t)Ps2GprU32(ctx, 4), (uint32_t)Ps2GprU32(ctx, 5)); }},
    {"setObstacleWormCtrlWork", 0x144cd50u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { setObstacleWormCtrlWork(rdram, ctx, runtime, (uint32_t)Ps2GprU32(ctx, 4), (uint32_t)Ps2GprU32(ctx, 5)); }},
    {"setSelfEvasionUpWormCtrlWork", 0x144cd58u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { setSelfEvasionUpWormCtrlWork(rdram, ctx, runtime, (uint32_t)Ps2GprU32(ctx, 4), (uint32_t)Ps2GprU32(ctx, 5)); }},
    {"setTypeWormCtrlWork", 0x144cd60u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { setTypeWormCtrlWork(rdram, ctx, runtime, (uint32_t)Ps2GprU32(ctx, 4), (uint32_t)Ps2GprU32(ctx, 5)); }},
    {"setFallGravCoffWormCtrlWork", 0x144cd68u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { setFallGravCoffWormCtrlWork(rdram, ctx, runtime, (uint32_t)Ps2GprU32(ctx, 4), ctx->f[12]); }},
    {"setMaxFallSpeedWormCtrlWork", 0x144cd70u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { setMaxFallSpeedWormCtrlWork(rdram, ctx, runtime, (uint32_t)Ps2GprU32(ctx, 4), ctx->f[12]); }},
    {"setGroundFrictionCoffWormCtrlWork", 0x144cd78u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { setGroundFrictionCoffWormCtrlWork(rdram, ctx, runtime, (uint32_t)Ps2GprU32(ctx, 4), ctx->f[12]); }},
    {"setUnderFrictionCoffWormCtrlWork", 0x144cd80u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { setUnderFrictionCoffWormCtrlWork(rdram, ctx, runtime, (uint32_t)Ps2GprU32(ctx, 4), ctx->f[12]); }},
    {"setWorldPitchLimitWormCtrlWork", 0x144cd88u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { setWorldPitchLimitWormCtrlWork(rdram, ctx, runtime, (uint32_t)Ps2GprU32(ctx, 4), ctx->f[12], ctx->f[13]); }},
    {"setColOffsetJudgeFunc", 0x144cde8u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { setColOffsetJudgeFunc(rdram, ctx, runtime, (uint32_t)Ps2GprU32(ctx, 4), (uint32_t)Ps2GprU32(ctx, 5)); }},
    {"gamecore_sub_0144CEA8", 0x144cea8u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { gamecore_sub_0144CEA8(rdram, ctx, runtime, (uint32_t)Ps2GprU32(ctx, 4)); }},
    {"createArrowThrustWork", 0x144d3a0u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { createArrowThrustWork(rdram, ctx, runtime, (uint32_t)Ps2GprU32(ctx, 5), (uint32_t)Ps2GprU32(ctx, 6)); }},
    {"SCRConditionChkDisplayOmakeCompleteAlert", 0x1459a50u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { SCRConditionChkDisplayOmakeCompleteAlert(rdram, ctx, runtime, (uint64_t)Ps2GprU64(ctx, 5)); }},
    {"setScrDemoMangerFinishCallback", 0x145ae68u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { setScrDemoMangerFinishCallback(rdram, ctx, runtime, (uint32_t)Ps2GprU32(ctx, 4), (uint32_t)Ps2GprU32(ctx, 5)); }},
    {"initInterestWork", 0x145c100u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { initInterestWork(rdram, ctx, runtime, (uint32_t)Ps2GprU32(ctx, 5)); }},
    {"getInterestExec", 0x145c118u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { getInterestExec(rdram, ctx, runtime, (uint32_t)Ps2GprU32(ctx, 4)); }},
    {"setPerceptionParam", 0x145c990u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { setPerceptionParam(rdram, ctx, runtime, (uint32_t)Ps2GprU32(ctx, 4), (uint32_t)Ps2GprU32(ctx, 5)); }},
    {"setGoingAngleGazeIKWork", 0x14617b8u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { setGoingAngleGazeIKWork(rdram, ctx, runtime, (uint32_t)Ps2GprU32(ctx, 4), ctx->f[12]); }},
    {"setSubParamGazeIKWork", 0x14617f0u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { setSubParamGazeIKWork(rdram, ctx, runtime, (uint32_t)Ps2GprU32(ctx, 4), (uint32_t)Ps2GprU32(ctx, 5)); }},
    {"setGazeModeGazeIKWork", 0x1461808u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { setGazeModeGazeIKWork(rdram, ctx, runtime, (uint32_t)Ps2GprU32(ctx, 4), (uint32_t)Ps2GprU32(ctx, 5)); }},
    {"setExecRootQuatGazeIKWork", 0x1461820u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { setExecRootQuatGazeIKWork(rdram, ctx, runtime, (uint32_t)Ps2GprU32(ctx, 4), (uint32_t)Ps2GprU32(ctx, 5)); }},
    {"setExecGazeIKWork", 0x1461838u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { setExecGazeIKWork(rdram, ctx, runtime, (uint32_t)Ps2GprU32(ctx, 4), (uint32_t)Ps2GprU32(ctx, 5)); }},
    {"initTailIKWork", 0x14621b8u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { initTailIKWork(rdram, ctx, runtime, (uint32_t)Ps2GprU32(ctx, 6)); }},
    {"setWormIKBoundDynamics", 0x14647e0u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { setWormIKBoundDynamics(rdram, ctx, runtime, (uint32_t)Ps2GprU32(ctx, 4), ctx->f[12], ctx->f[13], ctx->f[14]); }},
    {"setWormIKBankInterpSec", 0x1464820u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { setWormIKBankInterpSec(rdram, ctx, runtime, (uint32_t)Ps2GprU32(ctx, 4), ctx->f[12]); }},
    {"getSScriptWork", 0x1465308u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { getSScriptWork(rdram, ctx, runtime); }},
};
const uint32_t kEntryCount = 979u;
}
#if !defined(A1_OFFLINE)
// in-run: an address outside main RAM read through the runtime (the same slow path as the typed code's)
static uint64_t a1ckSlowRead(uint8_t *ram, PS2Runtime *rt, uint32_t a, uint32_t n)
{
    return n == 1 ? a1m::slowRead8(ram, rt, a) : n == 2 ? a1m::slowRead16(ram, rt, a) : n == 4 ? a1m::slowRead32(ram, rt, a) : a1m::slowRead64(ram, rt, a);
}
static const bool a1ckInit = (a1ck::g_slowRead = &a1ckSlowRead, true);
void a1ckReport(unsigned long long frame) { a1ck::ckReport(frame); }
#endif
#endif
