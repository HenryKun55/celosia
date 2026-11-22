// a4_typed_check.cpp: round A4's differential check of the typed functions (tools/typed_lift2.py --check-tu;
// docs/decomp_a4.md). Generated: do not edit. Compiled to code only with PS2X_A4_CHECK=1 (a check build) or
// A4_OFFLINE (the offline harness, tools/a4_harness.cpp).
#include "typed_a4_check_rt.h"
#if PS2X_A4_CHECK || defined(A4_OFFLINE)
namespace a4ck
{
bool refCall(uint32_t t, uint32_t ret, uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
}
// the reference legs' calls: dispatched to the callees' lifted texts on the same logged state (no clock, no table)
#undef PS2_CALL_GUEST_DIRECT_NOCHK
#undef PS2_CALL_GUEST_NOCHK
#undef PS2_CALL_GUEST_DIRECT
#undef PS2_CALL_GUEST_KNOWN
#undef PS2_TAIL_GUEST_DIRECT
#undef PS2_TAIL_GUEST_KNOWN
#define PS2_CALL_GUEST_DIRECT_NOCHK(t, f, r, s, k, n) a4ck::refCall((t), (r), rdram, ctx, runtime)
#define PS2_CALL_GUEST_NOCHK(t, f, r, s, k, n) a4ck::refCall((t), (r), rdram, ctx, runtime)
#define PS2_CALL_GUEST_DIRECT(t, f, r, s, k, n) a4ck::refCall((t), (r), rdram, ctx, runtime)
#define PS2_CALL_GUEST_KNOWN(t, f, r, s, k, n) a4ck::refCall((t), (r), rdram, ctx, runtime)
#define PS2_TAIL_GUEST_DIRECT(t, f) (void)a4ck::refCall((t), 0u, rdram, ctx, runtime)
#define PS2_TAIL_GUEST_KNOWN(t, f) (void)a4ck::refCall((t), 0u, rdram, ctx, runtime)

namespace a4ck
{
namespace ref
{
using lift_d1::sx64;
#include "typed_a1_ref.inc"
#include "typed_a2_ref.inc"
#include "typed_a3_ref.inc"
#include "typed_a4_ref.inc"
}
namespace ty
{
#include "typed_a1.inc"
#include "typed_a2.inc"
#include "typed_a3.inc"
#include "typed_a4.inc"
}

bool refCall(uint32_t t, uint32_t ret, uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    ctx->pc = t;
    switch (t)
    {
    case 0x101a30u: ref::SetHeapStartPoint(rdram, ctx, runtime); break;
    case 0x101a48u: ref::GetHeapCurrentPoint(rdram, ctx, runtime); break;
    case 0x102fe0u: ref::LoaderSysCheckCDBootMode(rdram, ctx, runtime); break;
    case 0x1032f0u: ref::LoaderSysSendAbort(rdram, ctx, runtime); break;
    case 0x1032f8u: ref::LoaderSysFlushPrint(rdram, ctx, runtime); break;
    case 0x103ca8u: ref::LoaderSysGetBootArg(rdram, ctx, runtime); break;
    case 0x103e90u: ref::LoaderSysHookPoint(rdram, ctx, runtime); break;
    case 0x104088u: ref::initmemprintf(rdram, ctx, runtime); break;
    case 0x104e78u: ref::padSysGet(rdram, ctx, runtime); break;
    case 0x104f58u: ref::LoaderSysSetPowerOffCallBackFunc(rdram, ctx, runtime); break;
    case 0x106380u: ref::SetLocate(rdram, ctx, runtime); break;
    case 0x106b58u: ref::PutChar(rdram, ctx, runtime); break;
    case 0x107d90u: ref::sub_00107D90(rdram, ctx, runtime); break;
    case 0x109268u: ref::sub_00109268(rdram, ctx, runtime); break;
    case 0x10fbe8u: ref::ps2___sceKernlGetEhSemaId(rdram, ctx, runtime); break;
    case 0x110640u: ref::GetTimerPreScaleFactor(rdram, ctx, runtime); break;
    case 0x110998u: ref::sub_00110998(rdram, ctx, runtime); break;
    case 0x110ef0u: ref::iGetTimerUsedUnusedCounters(rdram, ctx, runtime); break;
    case 0x111c50u: ref::sceSifRemoveRebootNotifyHandler(rdram, ctx, runtime); break;
    case 0x111fc0u: ref::ioctl(rdram, ctx, runtime); break;
    case 0x112080u: ref::isatty(rdram, ctx, runtime); break;
    case 0x112358u: ref::sub_00112358(rdram, ctx, runtime); break;
    case 0x112380u: ref::sub_00112380(rdram, ctx, runtime); break;
    case 0x1123c0u: ref::sub_001123C0(rdram, ctx, runtime); break;
    case 0x1128a0u: ref::sub_001128A0(rdram, ctx, runtime); break;
    case 0x1128c0u: ref::sub_001128C0(rdram, ctx, runtime); break;
    case 0x1148b8u: ref::sub_001148B8(rdram, ctx, runtime); break;
    case 0x117a00u: ref::_sceCdSC(rdram, ctx, runtime); break;
    case 0x119aa0u: ref::sub_00119AA0(rdram, ctx, runtime); break;
    case 0x120f18u: ref::ps2__SceMcComGetSysInfo(rdram, ctx, runtime); break;
    case 0x122448u: ref::sceNetcnfifSetFNoDecode(rdram, ctx, runtime); break;
    case 0x127048u: ref::ps2___unpack_f(rdram, ctx, runtime); break;
    case 0x1270d8u: ref::ps2___pack_d(rdram, ctx, runtime); break;
    case 0x127390u: ref::ps2___pack_f(rdram, ctx, runtime); break;
    case 0x127528u: ref::ps2___errno(rdram, ctx, runtime); break;
    case 0x128b00u: ref::sub_00128B00(rdram, ctx, runtime); break;
    case 0x12d7f0u: ref::sub_0012D7F0(rdram, ctx, runtime); break;
    case 0x12e520u: ref::_localeconv_r(rdram, ctx, runtime); break;
    case 0x12eaf8u: ref::_hi0bits(rdram, ctx, runtime); break;
    case 0x12eb78u: ref::_lo0bits(rdram, ctx, runtime); break;
    case 0x12fa00u: ref::isnan(rdram, ctx, runtime); break;
    case 0x1ace28u: ref::iosCdStStop(rdram, ctx, runtime); break;
    case 0x1acf40u: ref::iosCdStSkip(rdram, ctx, runtime); break;
    case 0x1acfb8u: ref::iosCdSetErrorCallback(rdram, ctx, runtime); break;
    case 0x1acfd0u: ref::iosCdSetMode(rdram, ctx, runtime); break;
    case 0x1ad140u: ref::iosInitDebPc(rdram, ctx, runtime); break;
    case 0x1ad148u: ref::iosStopPc(rdram, ctx, runtime); break;
    case 0x1ad150u: ref::iosDebPcVsyncCounterCheck(rdram, ctx, runtime); break;
    case 0x1ad158u: ref::iosGetCnt0Pc(rdram, ctx, runtime); break;
    case 0x1ad160u: ref::iosGetCnt1Pc(rdram, ctx, runtime); break;
    case 0x1ad168u: ref::InitDebPc(rdram, ctx, runtime); break;
    case 0x1ad1f0u: ref::iosGetClock(rdram, ctx, runtime); break;
    case 0x1ad948u: ref::iosEE2IopEndChk(rdram, ctx, runtime); break;
    case 0x1ad950u: ref::iosCreateIopMgr(rdram, ctx, runtime); break;
    case 0x1b0530u: ref::iosGetMallocPtn(rdram, ctx, runtime); break;
    case 0x1b0558u: ref::iosGetMallocRootPtnRemainSize(rdram, ctx, runtime); break;
    case 0x1b06a8u: ref::addMemoryDebugCallback(rdram, ctx, runtime); break;
    case 0x1b06b8u: ref::getMemorySafetyLockFlag(rdram, ctx, runtime); break;
    case 0x1b06c8u: ref::kernel_sub_001B06C8(rdram, ctx, runtime); break;
    case 0x1b12b0u: ref::iosInitMessageSystem(rdram, ctx, runtime); break;
    case 0x1b2438u: ref::iosPadGetXZInputLwithThresholdIndepAxis(rdram, ctx, runtime); break;
    case 0x1b27a0u: ref::iosPadEntryTickFunc(rdram, ctx, runtime); break;
    case 0x1b29e8u: ref::iosCreatePrintMgr(rdram, ctx, runtime); break;
    case 0x1b29f0u: ref::iosGetPrintfMode(rdram, ctx, runtime); break;
    case 0x1b34f0u: ref::endSCEDemo(rdram, ctx, runtime); break;
    case 0x1b3c18u: ref::Init_ShockVoiceSet(rdram, ctx, runtime); break;
    case 0x1b3d10u: ref::Shock_SetShockVoiceSet(rdram, ctx, runtime); break;
    case 0x1b3d48u: ref::Init_Player(rdram, ctx, runtime); break;
    case 0x1b3d88u: ref::Init_Controler(rdram, ctx, runtime); break;
    case 0x1b40c8u: ref::kernel_sub_001B40C8(rdram, ctx, runtime); break;
    case 0x1b4118u: ref::kernel_sub_001B4118(rdram, ctx, runtime); break;
    case 0x1b5f18u: ref::IosGetMergeDataFileName(rdram, ctx, runtime); break;
    case 0x1b6798u: ref::GetSymbolByIndex(rdram, ctx, runtime); break;
    case 0x1b6828u: ref::isysGetXffWorkSize(rdram, ctx, runtime); break;
    case 0x1b6880u: ref::isysGetModuleObjMallocSize(rdram, ctx, runtime); break;
    case 0x1b69b8u: ref::GetExecModuleSize(rdram, ctx, runtime); break;
    case 0x1b69c8u: ref::getModuleSize(rdram, ctx, runtime); break;
    case 0x1b7b58u: ref::isysCreateExceptionThread(rdram, ctx, runtime); break;
    case 0x1b7b78u: ref::iosCubicBezier(rdram, ctx, runtime); break;
    case 0x1b7c20u: ref::iosFourthBezier(rdram, ctx, runtime); break;
    case 0x1b9308u: ref::iosFloatModulo(rdram, ctx, runtime); break;
    case 0x1ba428u: ref::iosIColToMvecSl(rdram, ctx, runtime); break;
    case 0x1bb868u: ref::GetCurrentQuaternion(rdram, ctx, runtime); break;
    case 0x1bb888u: ref::GetLastQuaternion(rdram, ctx, runtime); break;
    case 0x1be2e8u: ref::iosGetACos(rdram, ctx, runtime); break;
    case 0x1be4d8u: ref::iosGetASin(rdram, ctx, runtime); break;
    case 0x1be4e8u: ref::iosGetATan2(rdram, ctx, runtime); break;
    case 0x1be4f8u: ref::resetArcFuncCall(rdram, ctx, runtime); break;
    case 0x1be520u: ref::iosGetTableSin(rdram, ctx, runtime); break;
    case 0x1be8e0u: ref::iosDumpFVector(rdram, ctx, runtime); break;
    case 0x1be8e8u: ref::iosDumpIVector(rdram, ctx, runtime); break;
    case 0x1be980u: ref::iosGetScaleInverseVector(rdram, ctx, runtime); break;
    case 0x1bf0b0u: ref::iosSetDCViewPort(rdram, ctx, runtime); break;
    case 0x1bf2d8u: ref::iosGetRootDC(rdram, ctx, runtime); break;
    case 0x1bf308u: ref::iosGetCurrentDC(rdram, ctx, runtime); break;
    case 0x1bf420u: ref::iosGetDCGroup(rdram, ctx, runtime); break;
    case 0x1bf430u: ref::kernel_sub_001BF430(rdram, ctx, runtime); break;
    case 0x1bf438u: ref::kernel_sub_001BF438(rdram, ctx, runtime); break;
    case 0x1bfb80u: ref::iosDlGetTransAdrs(rdram, ctx, runtime); break;
    case 0x1bfba8u: ref::iosGetPacketPtrPath1(rdram, ctx, runtime); break;
    case 0x1bfbf0u: ref::iosSetPacketPtrPath1(rdram, ctx, runtime); break;
    case 0x1bfc30u: ref::iosDlChainHead(rdram, ctx, runtime); break;
    case 0x1bfdc0u: ref::iosDlChainHeadCurrent(rdram, ctx, runtime); break;
    case 0x1c0138u: ref::iosDlGetWorkBufId(rdram, ctx, runtime); break;
    case 0x1c0150u: ref::iosDlGetTransBufId(rdram, ctx, runtime); break;
    case 0x1c0168u: ref::iosDlGetEndAdrs(rdram, ctx, runtime); break;
    case 0x1c02c8u: ref::iosDlGetLayerNum(rdram, ctx, runtime); break;
    case 0x1c03b8u: ref::iosDlCheckAdrs(rdram, ctx, runtime); break;
    case 0x1c03f0u: ref::iosDlCheckSize(rdram, ctx, runtime); break;
    case 0x1c0480u: ref::iosAllocPacketPtrPath1UnCachedA(rdram, ctx, runtime); break;
    case 0x1c2018u: ref::getDispScreenWidth(rdram, ctx, runtime); break;
    case 0x1c2028u: ref::getDispScreenHeight(rdram, ctx, runtime); break;
    case 0x1c2038u: ref::getDrawScreenWidth(rdram, ctx, runtime); break;
    case 0x1c2048u: ref::getDrawScreenHeight(rdram, ctx, runtime); break;
    case 0x1c20e8u: ref::getDispBufferAddressTBP(rdram, ctx, runtime); break;
    case 0x1c20f0u: ref::getDispBufferAddressFBP(rdram, ctx, runtime); break;
    case 0x1c2158u: ref::getZBufferAddressTBP(rdram, ctx, runtime); break;
    case 0x1c2178u: ref::getZBufferAddressFBP(rdram, ctx, runtime); break;
    case 0x1c2208u: ref::getScreenOffsetX(rdram, ctx, runtime); break;
    case 0x1c2218u: ref::getScreenOffsetY(rdram, ctx, runtime); break;
    case 0x1c2228u: ref::getAntiAlias(rdram, ctx, runtime); break;
    case 0x1c2238u: ref::getProgressiveMode(rdram, ctx, runtime); break;
    case 0x1c28b0u: ref::iosGetField(rdram, ctx, runtime); break;
    case 0x1c2a68u: ref::iosSetSyncNoReturnFunc(rdram, ctx, runtime); break;
    case 0x1c2b98u: ref::iosWaitDMA(rdram, ctx, runtime); break;
    case 0x1c2be8u: ref::getCRTCoffsetU(rdram, ctx, runtime); break;
    case 0x1c2bf8u: ref::getCRTCoffsetV(rdram, ctx, runtime); break;
    case 0x1c3720u: ref::iosSetIStartFunc(rdram, ctx, runtime); break;
    case 0x1c3730u: ref::iosSetGpFinishFunc(rdram, ctx, runtime); break;
    case 0x1c3740u: ref::iosSetIFinishFunc(rdram, ctx, runtime); break;
    case 0x1c3750u: ref::getIntrGpCount(rdram, ctx, runtime); break;
    case 0x1c3768u: ref::iosSetDlTransFunc(rdram, ctx, runtime); break;
    case 0x1c3778u: ref::iosSetVSchedulerFunc(rdram, ctx, runtime); break;
    case 0x1c5b48u: ref::getSpriteDrawFirst(rdram, ctx, runtime); break;
    case 0x1c5b58u: ref::gsReglist(rdram, ctx, runtime); break;
    case 0x1c7540u: ref::isysSetObjImmediateDeleteFunc(rdram, ctx, runtime); break;
    case 0x1c7658u: ref::isysInitGroup(rdram, ctx, runtime); break;
    case 0x1c7680u: ref::isysObjDeleteLock(rdram, ctx, runtime); break;
    case 0x1c79a8u: ref::isysGetLinkMax(rdram, ctx, runtime); break;
    case 0x1c79b0u: ref::isysGetObjIdentifierByObj(rdram, ctx, runtime); break;
    case 0x1c7fa0u: ref::iosGetISchedulerExecCnt(rdram, ctx, runtime); break;
    case 0x1c7ff0u: ref::ischedRestart(rdram, ctx, runtime); break;
    case 0x1c8028u: ref::iosSetISchedulerSubthreadTask(rdram, ctx, runtime); break;
    case 0x1c8038u: ref::iosSetIVSyncMode(rdram, ctx, runtime); break;
    case 0x1c8048u: ref::iosGetIVSyncMode(rdram, ctx, runtime); break;
    case 0x1c8058u: ref::vschedSetNextFrameForceGarbage(rdram, ctx, runtime); break;
    case 0x1c8068u: ref::GetGarbageTime(rdram, ctx, runtime); break;
    case 0x1c84c0u: ref::init_letter(rdram, ctx, runtime); break;
    case 0x1c84d8u: ref::is_hope_letter_type(rdram, ctx, runtime); break;
    case 0x1c84e8u: ref::get_letter_contents_data(rdram, ctx, runtime); break;
    case 0x1c8500u: ref::increment_letter_refcount(rdram, ctx, runtime); break;
    case 0x1c8570u: ref::is_letter_reach_time(rdram, ctx, runtime); break;
    case 0x1c8598u: ref::is_init_letter_called(rdram, ctx, runtime); break;
    case 0x1c85a0u: ref::get_reference_letter_type(rdram, ctx, runtime); break;
    case 0x1c85a8u: ref::get_letter_receiver_obj_group(rdram, ctx, runtime); break;
    case 0x1c85b0u: ref::get_letter_receiver_data(rdram, ctx, runtime); break;
    case 0x1c88f8u: ref::get_total_letter_in_post(rdram, ctx, runtime); break;
    case 0x1c8948u: ref::kernel_sub_001C8948(rdram, ctx, runtime); break;
    case 0x1c8ad0u: ref::constructor_letter_type(rdram, ctx, runtime); break;
    case 0x1c8b68u: ref::get_letter_type(rdram, ctx, runtime); break;
    case 0x1c90d0u: ref::iosGetDataSizeOfFont(rdram, ctx, runtime); break;
    case 0x1c9160u: ref::iosGetFontDataWithJIS(rdram, ctx, runtime); break;
    case 0x1c9470u: ref::iosGetFontIDWithJIS(rdram, ctx, runtime); break;
    case 0x1c9620u: ref::iosEntryLocaleFontNoLoad(rdram, ctx, runtime); break;
    case 0x1c9708u: ref::iosGet2ByteCharCode(rdram, ctx, runtime); break;
    case 0x1c9720u: ref::_isysCreateSubMsgQueue(rdram, ctx, runtime); break;
    case 0x1ca4a0u: ref::isysGetSubThreadWorkSize(rdram, ctx, runtime); break;
    case 0x1ca5a0u: ref::isysDumpSubThreadPC(rdram, ctx, runtime); break;
    case 0x1ca5a8u: ref::isysDebugSetSubThreadSleepProtection(rdram, ctx, runtime); break;
    case 0x1cad30u: ref::sndSepSetDefaultParam(rdram, ctx, runtime); break;
    case 0x1cad90u: ref::sndSepPauseAll(rdram, ctx, runtime); break;
    case 0x1cadb8u: ref::sndSepPause(rdram, ctx, runtime); break;
    case 0x1cadd0u: ref::sndSepCont(rdram, ctx, runtime); break;
    case 0x1caf88u: ref::sndVabDelete(rdram, ctx, runtime); break;
    case 0x1cb168u: ref::sndBDClose(rdram, ctx, runtime); break;
    case 0x1cb3a0u: ref::addAdpcmTickProcFunc(rdram, ctx, runtime); break;
    case 0x1cb3b0u: ref::setPcmTickProcFunc(rdram, ctx, runtime); break;
    case 0x1cb3c0u: ref::soundGetTickProcCnt(rdram, ctx, runtime); break;
    case 0x1cb3d0u: ref::soundSetRevMode(rdram, ctx, runtime); break;
    case 0x1cb408u: ref::soundGetRevMode(rdram, ctx, runtime); break;
    case 0x1cb460u: ref::getSoundMode(rdram, ctx, runtime); break;
    case 0x1cb920u: ref::iosKeybdSetCallBackFunc(rdram, ctx, runtime); break;
    case 0x1cbc70u: ref::iosMouseSetCallBackFunc(rdram, ctx, runtime); break;
    case 0x1cbcb8u: ref::GetStrHashWorkBuffer(rdram, ctx, runtime); break;
    case 0x1cc028u: ref::kernel_sub_001CC028(rdram, ctx, runtime); break;
    case 0x1cc228u: ref::Sg2CtrlSetTickMode(rdram, ctx, runtime); break;
    case 0x1cc240u: ref::Sg2CtrlGetTickMode(rdram, ctx, runtime); break;
    case 0x1cc250u: ref::Sg2CtrlSetTickTime(rdram, ctx, runtime); break;
    case 0x1cc268u: ref::Sg2CtrlGetTickTime(rdram, ctx, runtime); break;
    case 0x1cc318u: ref::Sg2DmaInit(rdram, ctx, runtime); break;
    case 0x1cc7d8u: ref::Sg2DspGetMasterVolume(rdram, ctx, runtime); break;
    case 0x1cc898u: ref::Sg2ExStAdpcmStopMode(rdram, ctx, runtime); break;
    case 0x1cced0u: ref::Sg2ExStAdpcmSetFd(rdram, ctx, runtime); break;
    case 0x1ccee0u: ref::Sg2ExStAdpcmGetFd(rdram, ctx, runtime); break;
    case 0x1ccee8u: ref::Sg2ExStAdpcmSetLsn(rdram, ctx, runtime); break;
    case 0x1ccef8u: ref::Sg2ExStAdpcmGetLsn(rdram, ctx, runtime); break;
    case 0x1ccf00u: ref::Sg2ExStAdpcmGetTargetLsn(rdram, ctx, runtime); break;
    case 0x1ccf18u: ref::Sg2ExStAdpcmSetUserParam(rdram, ctx, runtime); break;
    case 0x1ccf28u: ref::Sg2ExStAdpcmGetUserParam(rdram, ctx, runtime); break;
    case 0x1cd6f8u: ref::Sg2ExStAdpcmLoopSecNum(rdram, ctx, runtime); break;
    case 0x1cd710u: ref::Sg2ExStAdpcmGetBuffer(rdram, ctx, runtime); break;
    case 0x1cd738u: ref::Sg2ExStAdpcmSetSignal(rdram, ctx, runtime); break;
    case 0x1cec38u: ref::Sg2ModCheckSndOn(rdram, ctx, runtime); break;
    case 0x1cec70u: ref::Sg2ModClose(rdram, ctx, runtime); break;
    case 0x1cf6e8u: ref::Sg2ModGetParam(rdram, ctx, runtime); break;
    case 0x1d0d78u: ref::kernel_sub_001D0D78(rdram, ctx, runtime); break;
    case 0x1d12a8u: ref::kernel_sub_001D12A8(rdram, ctx, runtime); break;
    case 0x1d12e0u: ref::kernel_sub_001D12E0(rdram, ctx, runtime); break;
    case 0x1d1318u: ref::kernel_sub_001D1318(rdram, ctx, runtime); break;
    case 0x1d1350u: ref::kernel_sub_001D1350(rdram, ctx, runtime); break;
    case 0x1d1358u: ref::kernel_sub_001D1358(rdram, ctx, runtime); break;
    case 0x1d13e8u: ref::kernel_sub_001D13E8(rdram, ctx, runtime); break;
    case 0x1d1470u: ref::kernel_sub_001D1470(rdram, ctx, runtime); break;
    case 0x1d14c8u: ref::kernel_sub_001D14C8(rdram, ctx, runtime); break;
    case 0x1d1520u: ref::kernel_sub_001D1520(rdram, ctx, runtime); break;
    case 0x1d1578u: ref::kernel_sub_001D1578(rdram, ctx, runtime); break;
    case 0x1d1580u: ref::kernel_sub_001D1580(rdram, ctx, runtime); break;
    case 0x1d1658u: ref::kernel_sub_001D1658(rdram, ctx, runtime); break;
    case 0x1d1660u: ref::kernel_sub_001D1660(rdram, ctx, runtime); break;
    case 0x1d1698u: ref::kernel_sub_001D1698(rdram, ctx, runtime); break;
    case 0x1d1810u: ref::kernel_sub_001D1810(rdram, ctx, runtime); break;
    case 0x1d18d8u: ref::kernel_sub_001D18D8(rdram, ctx, runtime); break;
    case 0x1d1f08u: ref::kernel_sub_001D1F08(rdram, ctx, runtime); break;
    case 0x1d1f10u: ref::kernel_sub_001D1F10(rdram, ctx, runtime); break;
    case 0x1d1f18u: ref::kernel_sub_001D1F18(rdram, ctx, runtime); break;
    case 0x1d1f20u: ref::kernel_sub_001D1F20(rdram, ctx, runtime); break;
    case 0x1d2008u: ref::kernel_sub_001D2008(rdram, ctx, runtime); break;
    case 0x1d2150u: ref::Sg2RemoteDmaTransStatus(rdram, ctx, runtime); break;
    case 0x1d2170u: ref::Sg2RemoteGetAdpStmAdr(rdram, ctx, runtime); break;
    case 0x1d2188u: ref::Sg2RemoteGetPcmStmTransOffset(rdram, ctx, runtime); break;
    case 0x1d2198u: ref::Sg2RemoteGetSlotReleaseLv(rdram, ctx, runtime); break;
    case 0x1d21b0u: ref::Sg2RemoteGetDspChangeStatus(rdram, ctx, runtime); break;
    case 0x1d2b98u: ref::Sg2SepStatus(rdram, ctx, runtime); break;
    case 0x1d3428u: ref::kernel_sub_001D3428(rdram, ctx, runtime); break;
    case 0x1d3430u: ref::kernel_sub_001D3430(rdram, ctx, runtime); break;
    case 0x1d3a48u: ref::kernel_sub_001D3A48(rdram, ctx, runtime); break;
    case 0x1d5d28u: ref::Sg2SlotFree(rdram, ctx, runtime); break;
    case 0x1d6250u: ref::Sg2SlotSetGroup(rdram, ctx, runtime); break;
    case 0x1d6270u: ref::Sg2SlotSetMute(rdram, ctx, runtime); break;
    case 0x1d6528u: ref::Sg2SlotSetBendRange(rdram, ctx, runtime); break;
    case 0x1d6548u: ref::Sg2SlotSetBendValue(rdram, ctx, runtime); break;
    case 0x1d65f0u: ref::Sg2SlotSetPause(rdram, ctx, runtime); break;
    case 0x1d6670u: ref::Sg2SlotSetDirectPitch(rdram, ctx, runtime); break;
    case 0x1d6758u: ref::Sg2SlotSetLoop(rdram, ctx, runtime); break;
    case 0x1d6780u: ref::Sg2SlotSetDumperOn(rdram, ctx, runtime); break;
    case 0x1d6950u: ref::Sg2SlotSetLfoVolTbl(rdram, ctx, runtime); break;
    case 0x1d6c30u: ref::Sg2SlotSetTimeVolParam(rdram, ctx, runtime); break;
    case 0x1d6c70u: ref::Sg2SlotSetTimePitchParam(rdram, ctx, runtime); break;
    case 0x1d6cb0u: ref::Sg2SlotSetTimePanParam(rdram, ctx, runtime); break;
    case 0x1d6d50u: ref::Sg2SlotGetVol(rdram, ctx, runtime); break;
    case 0x1d6d70u: ref::Sg2SlotGetPan(rdram, ctx, runtime); break;
    case 0x1d7458u: ref::Sg2SlotSetOutputMode(rdram, ctx, runtime); break;
    case 0x1d7470u: ref::Sg2SlotSetVolShiftDownParam(rdram, ctx, runtime); break;
    case 0x1d7488u: ref::Sg2SlotGetVolShiftDownParam(rdram, ctx, runtime); break;
    case 0x1d8250u: ref::kernel_sub_001D8250(rdram, ctx, runtime); break;
    case 0x1d88f8u: ref::Sg2SmfpStatus(rdram, ctx, runtime); break;
    case 0x1d8e78u: ref::Sg2SmfpSetSignal(rdram, ctx, runtime); break;
    case 0x1d95c8u: ref::kernel_sub_001D95C8(rdram, ctx, runtime); break;
    case 0x1d95d0u: ref::kernel_sub_001D95D0(rdram, ctx, runtime); break;
    case 0x1d97f0u: ref::kernel_sub_001D97F0(rdram, ctx, runtime); break;
    case 0x1da0a0u: ref::vab_check_alive(rdram, ctx, runtime); break;
    case 0x1da0e0u: ref::vab_get_info(rdram, ctx, runtime); break;
    case 0x1da348u: ref::vab_get_veltbl(rdram, ctx, runtime); break;
    case 0x1da380u: ref::vab_get_lfotbl_param(rdram, ctx, runtime); break;
    case 0x1da3c8u: ref::vab_set_semastervol(rdram, ctx, runtime); break;
    case 0x1da3f8u: ref::vab_get_semastervol(rdram, ctx, runtime); break;
    case 0x1db3c8u: ref::kernel_sub_001DB3C8(rdram, ctx, runtime); break;
    case 0x1db9e8u: ref::Sg2ExPcmInputGetLipFrame(rdram, ctx, runtime); break;
    case 0x1dbba0u: ref::Sg2ExPcmInputSetOutputMode(rdram, ctx, runtime); break;
    case 0x1dbd90u: ref::Sg2ExPcmInputAutoFilterCtrl(rdram, ctx, runtime); break;
    case 0x1dbfb8u: ref::Sg2ExPcmOutputSetOutputMode(rdram, ctx, runtime); break;
    case 0x1dc068u: ref::Sg2ExPcmOutputGetLipFrame(rdram, ctx, runtime); break;
    case 0x1dc1d0u: ref::Sg2ExPcmOutputAutoFilterCtrl(rdram, ctx, runtime); break;
    case 0x1dc2d0u: ref::Sg2ExPcmSetSignal(rdram, ctx, runtime); break;
    case 0x1dca28u: ref::IirInit(rdram, ctx, runtime); break;
    case 0x107d5c8u: ref::GAMEUTIL_is_inside_of_region_pitch(rdram, ctx, runtime); break;
    case 0x107ec28u: ref::GAMEUTIL_is_nearly_equal_zero(rdram, ctx, runtime); break;
    case 0x107ec70u: ref::GAMEUTIL_is_nearly_equal_zero3D(rdram, ctx, runtime); break;
    case 0x107ee48u: ref::GAMEUTIL_get_answer_of_2jihouteishiki(rdram, ctx, runtime); break;
    case 0x107fb30u: ref::GAMEUTIL_get_new_velocity_by_brake_abs(rdram, ctx, runtime); break;
    case 0x107fb60u: ref::GAMEUTIL_get_velocity_by_diff_move(rdram, ctx, runtime); break;
    case 0x107fed0u: ref::GAMEUTIL_mul_ratio(rdram, ctx, runtime); break;
    case 0x10802f0u: ref::GAMEUTIL_convert_from_lw_to_game_coord(rdram, ctx, runtime); break;
    case 0x1080b08u: ref::GAMEUTIL_get_difference_pitch_and_pitch(rdram, ctx, runtime); break;
    case 0x1080b28u: ref::GAMEUTIL_get_move_radi_from_yaw1_to_yaw2(rdram, ctx, runtime); break;
    case 0x1080b90u: ref::GAMEUTIL_get_move_radi_from_pitch1_to_pitch2(rdram, ctx, runtime); break;
    case 0x1080b98u: ref::GAMEUTIL_is_between_yaw_and_yaw(rdram, ctx, runtime); break;
    case 0x1080d58u: ref::GAMEUTIL_is_between_pitch_and_pitch(rdram, ctx, runtime); break;
    case 0x1080f50u: ref::GAMEUTIL_is_reach_aim_yaw_angle(rdram, ctx, runtime); break;
    case 0x10829f8u: ref::GAMEUTIL_is_nearly_equal_yaw(rdram, ctx, runtime); break;
    case 0x1082a50u: ref::GAMEUTIL_is_nearly_equal_pitch(rdram, ctx, runtime); break;
    case 0x1082c78u: ref::manager_sub_01082C78(rdram, ctx, runtime); break;
    case 0x1082cd8u: ref::manager_sub_01082CD8(rdram, ctx, runtime); break;
    case 0x1082da0u: ref::get_fast_get_malloc_partition(rdram, ctx, runtime); break;
    case 0x10832a8u: ref::GAMEUTIL_doit_interp_ratio_cntrl(rdram, ctx, runtime); break;
    case 0x10836c0u: ref::GAMEUTIL_get_dist_ratio(rdram, ctx, runtime); break;
    case 0x1083828u: ref::GAMEUTIL_get_interp_value(rdram, ctx, runtime); break;
    case 0x10842d0u: ref::GAMEUTIL_forward_timer_still_one_ratio_time_interp_ratio_cntrl(rdram, ctx, runtime); break;
    case 0x10842e8u: ref::GAMEUTIL_get_interp_ratio_interp_ratio_cntrl(rdram, ctx, runtime); break;
    case 0x10842f0u: ref::GAMEUTIL_get_interp_timer_interp_ratio_cntrl(rdram, ctx, runtime); break;
    case 0x10842f8u: ref::GAMEUTIL_set_act_interp_flag_interp_ratio_cntrl(rdram, ctx, runtime); break;
    case 0x1084300u: ref::GAMEUTIL_set_act_max_zero_clip_flag_interp_ratio_cntrl(rdram, ctx, runtime); break;
    case 0x1084500u: ref::GAMEUTIL_get_interp_value_interp_value_cntrl(rdram, ctx, runtime); break;
    case 0x1084508u: ref::GAMEUTIL_get_hope_value_interp_value_cntrl(rdram, ctx, runtime); break;
    case 0x1084510u: ref::set_param_double_spring_interp_common(rdram, ctx, runtime); break;
    case 0x1084588u: ref::set_rot_radi_value_flag_double_spring_interp_cntrl(rdram, ctx, runtime); break;
    case 0x1084698u: ref::set_rot_radi_value_flag_double_spring_interp_cntrl3D(rdram, ctx, runtime); break;
    case 0x1084880u: ref::constructor_aim_angle_interp_cntrl(rdram, ctx, runtime); break;
    case 0x10848a8u: ref::reset_aim_angle_interp_cntrl(rdram, ctx, runtime); break;
    case 0x1084968u: ref::set_interp_param_aim_angle_interp_cntrl(rdram, ctx, runtime); break;
    case 0x1084978u: ref::manager_sub_01084978(rdram, ctx, runtime); break;
    case 0x1087108u: ref::getNowBossEffect(rdram, ctx, runtime); break;
    case 0x1087118u: ref::setBossDeathEffect(rdram, ctx, runtime); break;
    case 0x1087200u: ref::getBossEffectBlurLevel(rdram, ctx, runtime); break;
    case 0x1087210u: ref::getBossEffectColorR(rdram, ctx, runtime); break;
    case 0x1087220u: ref::getBossEffectColorG(rdram, ctx, runtime); break;
    case 0x1087230u: ref::getBossEffectColorB(rdram, ctx, runtime); break;
    case 0x1087240u: ref::getBossEffectBlendRatio(rdram, ctx, runtime); break;
    case 0x1087250u: ref::getBossEffectOutLine(rdram, ctx, runtime); break;
    case 0x1088190u: ref::getDepthOfFieldSw(rdram, ctx, runtime); break;
    case 0x10881a0u: ref::getDepthOfFieldLevel(rdram, ctx, runtime); break;
    case 0x10881b0u: ref::getDepthOfFieldPlanes(rdram, ctx, runtime); break;
    case 0x10881c0u: ref::getDepthOfFieldNearZ(rdram, ctx, runtime); break;
    case 0x1088a78u: ref::fsEffectSetParamSheetSeg(rdram, ctx, runtime); break;
    case 0x10898f8u: ref::getFogMode(rdram, ctx, runtime); break;
    case 0x1089908u: ref::getFogNearDist(rdram, ctx, runtime); break;
    case 0x1089918u: ref::getFogFarDist(rdram, ctx, runtime); break;
    case 0x1089928u: ref::getFogNearRate(rdram, ctx, runtime); break;
    case 0x1089938u: ref::getFogFarRate(rdram, ctx, runtime); break;
    case 0x1089958u: ref::getFogOffset(rdram, ctx, runtime); break;
    case 0x1089968u: ref::getFogUnderWater(rdram, ctx, runtime); break;
    case 0x1089978u: ref::getFogColorR(rdram, ctx, runtime); break;
    case 0x1089988u: ref::getFogColorG(rdram, ctx, runtime); break;
    case 0x1089998u: ref::getFogColorB(rdram, ctx, runtime); break;
    case 0x1089a38u: ref::SeemlessInitFogTargetParamWorldLayout(rdram, ctx, runtime); break;
    case 0x1089b60u: ref::fogSetFogColorMulRatio(rdram, ctx, runtime); break;
    case 0x1089bb0u: ref::getFogFactor1(rdram, ctx, runtime); break;
    case 0x108b970u: ref::setFadeColor(rdram, ctx, runtime); break;
    case 0x108b9b8u: ref::setFadeLayer(rdram, ctx, runtime); break;
    case 0x108ba28u: ref::getCameraBlurSw(rdram, ctx, runtime); break;
    case 0x108ba38u: ref::getBlurAlsoChara(rdram, ctx, runtime); break;
    case 0x108ba48u: ref::getBlurWorkSize(rdram, ctx, runtime); break;
    case 0x108ba58u: ref::getNBlurPlanes(rdram, ctx, runtime); break;
    case 0x108ba68u: ref::getBlurIntensity(rdram, ctx, runtime); break;
    case 0x108ba78u: ref::getBlurZBase(rdram, ctx, runtime); break;
    case 0x108ba88u: ref::getBlurMinThreshold(rdram, ctx, runtime); break;
    case 0x108ba98u: ref::getBlurMaxThreshold(rdram, ctx, runtime); break;
    case 0x108baa8u: ref::getBlurShutterSpeed(rdram, ctx, runtime); break;
    case 0x108bab8u: ref::getBlurXYZRatio(rdram, ctx, runtime); break;
    case 0x108bac8u: ref::getBlurCamMoveThreshold(rdram, ctx, runtime); break;
    case 0x108e9c8u: ref::glTexCoord2f(rdram, ctx, runtime); break;
    case 0x108e9f0u: ref::glTexCoord2fv(rdram, ctx, runtime); break;
    case 0x108f3b0u: ref::glZOffset(rdram, ctx, runtime); break;
    case 0x108f608u: ref::glSetFogFactorDirect(rdram, ctx, runtime); break;
    case 0x108f7e0u: ref::gluGetBrightness(rdram, ctx, runtime); break;
    case 0x108fba8u: ref::getMBlur(rdram, ctx, runtime); break;
    case 0x108fbc8u: ref::getBrightness(rdram, ctx, runtime); break;
    case 0x10916f8u: ref::kSetTex(rdram, ctx, runtime); break;
    case 0x1092470u: ref::getSpecularity(rdram, ctx, runtime); break;
    case 0x1099590u: ref::modelDeleteDispModelClass(rdram, ctx, runtime); break;
    case 0x1099658u: ref::modelGetWorkSize(rdram, ctx, runtime); break;
    case 0x1099758u: ref::SetDisplayModelNearClippingSwitch(rdram, ctx, runtime); break;
    case 0x109a3d8u: ref::ptclmanGet(rdram, ctx, runtime); break;
    case 0x109a3f8u: ref::ptclmanPut(rdram, ctx, runtime); break;
    case 0x109a4d0u: ref::ptclCounterGet(rdram, ctx, runtime); break;
    case 0x109be88u: ref::getRefBlurLevel(rdram, ctx, runtime); break;
    case 0x109d0d8u: ref::getIsInWater(rdram, ctx, runtime); break;
    case 0x109d0e8u: ref::getRefractBlurSpeed(rdram, ctx, runtime); break;
    case 0x109d0f8u: ref::getRefractBlurSpeedRand(rdram, ctx, runtime); break;
    case 0x109d108u: ref::getRefractBlurLevel(rdram, ctx, runtime); break;
    case 0x109d118u: ref::getRefractBlurLevelRand(rdram, ctx, runtime); break;
    case 0x109d128u: ref::getRefractLengthRatio(rdram, ctx, runtime); break;
    case 0x109d138u: ref::getRefractBlendRatio(rdram, ctx, runtime); break;
    case 0x109d148u: ref::getRefractBrightness(rdram, ctx, runtime); break;
    case 0x109de78u: ref::getShadowIntensity(rdram, ctx, runtime); break;
    case 0x109dea8u: ref::GetShadowLength(rdram, ctx, runtime); break;
    case 0x109df30u: ref::shadowModelReInit(rdram, ctx, runtime); break;
    case 0x10a1728u: ref::getSolarFlareSw(rdram, ctx, runtime); break;
    case 0x10a1738u: ref::getSolarFlareBlur(rdram, ctx, runtime); break;
    case 0x10a1748u: ref::getSolarFlareColorR(rdram, ctx, runtime); break;
    case 0x10a1758u: ref::getSolarFlareColorG(rdram, ctx, runtime); break;
    case 0x10a1768u: ref::getSolarFlareColorB(rdram, ctx, runtime); break;
    case 0x10a1778u: ref::getSolarFlareSolarLevel(rdram, ctx, runtime); break;
    case 0x10a1788u: ref::getSolarGlowLevel(rdram, ctx, runtime); break;
    case 0x10a1798u: ref::getSolarFlareSamplePix(rdram, ctx, runtime); break;
    case 0x10a2040u: ref::manager_sub_010A2040(rdram, ctx, runtime); break;
    case 0x10a52d0u: ref::spriteSetPosition(rdram, ctx, runtime); break;
    case 0x10a5310u: ref::spriteSetTextureCoordinateSTFromUV(rdram, ctx, runtime); break;
    case 0x10a53b8u: ref::spriteSetTextureCoordinateUV(rdram, ctx, runtime); break;
    case 0x10a5410u: ref::spriteSetColor(rdram, ctx, runtime); break;
    case 0x10a5428u: ref::spriteSetScale(rdram, ctx, runtime); break;
    case 0x10a5438u: ref::spriteSetRotation(rdram, ctx, runtime); break;
    case 0x10a54d0u: ref::spriteSetFade(rdram, ctx, runtime); break;
    case 0x10a54d8u: ref::spriteSetPriority(rdram, ctx, runtime); break;
    case 0x10abc48u: ref::manager_sub_010ABC48(rdram, ctx, runtime); break;
    case 0x10ac400u: ref::getZFogDivide(rdram, ctx, runtime); break;
    case 0x10ac440u: ref::getZFogScissorMargin(rdram, ctx, runtime); break;
    case 0x10ac460u: ref::setZFogFar(rdram, ctx, runtime); break;
    case 0x10ac478u: ref::getZFogNear(rdram, ctx, runtime); break;
    case 0x10b6788u: ref::GetObjectAnimationParentNodeId(rdram, ctx, runtime); break;
    case 0x10b74c0u: ref::IsFullFixFormat(rdram, ctx, runtime); break;
    case 0x10b7650u: ref::GetMotionResourceRotAngle(rdram, ctx, runtime); break;
    case 0x10b92a8u: ref::constructor_motion_ik_base(rdram, ctx, runtime); break;
    case 0x10b92b0u: ref::init_motion_ik_base(rdram, ctx, runtime); break;
    case 0x10b92b8u: ref::doit_motion_ik_base(rdram, ctx, runtime); break;
    case 0x10ba140u: ref::manager_sub_010BA140(rdram, ctx, runtime); break;
    case 0x10ba9d8u: ref::UpdateAnimationClassPastSimpleInfo(rdram, ctx, runtime); break;
    case 0x10c0c78u: ref::GetCharacterClassInitParamNbOptionNodes(rdram, ctx, runtime); break;
    case 0x10c11b8u: ref::ResetCharacterClassWaterStatus(rdram, ctx, runtime); break;
    case 0x10c17b0u: ref::ResetCharacterClassMotInterpInfo(rdram, ctx, runtime); break;
    case 0x10c1a08u: ref::GetCharacterClassCurAnimationFrameRate(rdram, ctx, runtime); break;
    case 0x10c66f0u: ref::SetCharacterClassGazeAdditionChannelInfo(rdram, ctx, runtime); break;
    case 0x10cc6c8u: ref::ForceAcceptMotionControlChange(rdram, ctx, runtime); break;
    case 0x10cc778u: ref::YattsukeSetPlayerR1ON(rdram, ctx, runtime); break;
    case 0x10d2a88u: ref::RequestCharacterClassSlip(rdram, ctx, runtime); break;
    case 0x10d2aa0u: ref::RequestCharacterClassNoClimbableSlip(rdram, ctx, runtime); break;
    case 0x10d2ae0u: ref::RequestCharacterClassAdditionMotionChange(rdram, ctx, runtime); break;
    case 0x10d2b60u: ref::RequestCharacterClassMotionDirectionByGPTop(rdram, ctx, runtime); break;
    case 0x10d2b70u: ref::RequestCharacterClassMotionDirectionByGPTail(rdram, ctx, runtime); break;
    case 0x10d2b90u: ref::RequestCharacterClassMotionTurnLevel(rdram, ctx, runtime); break;
    case 0x10d2bb0u: ref::RequestCharacterClassMotionPlaySpeedUp(rdram, ctx, runtime); break;
    case 0x10d2bc0u: ref::RequestCharacterClassMotionPlaySpeedDown(rdram, ctx, runtime); break;
    case 0x10d2bd0u: ref::RequestCharacterClassMotionPlaySpeedDown2(rdram, ctx, runtime); break;
    case 0x10d2be0u: ref::RequestCharacterClassWindForceMove(rdram, ctx, runtime); break;
    case 0x10d2ce8u: ref::getTurnLimitInterp(rdram, ctx, runtime); break;
    case 0x10d2d48u: ref::getRatioFromInterp(rdram, ctx, runtime); break;
    case 0x10d37a8u: ref::SetCharacterObjMotionJumpSpeedLimit(rdram, ctx, runtime); break;
    case 0x10dc3f0u: ref::charrootdyn_getUpperEdgeEffectiveHeight(rdram, ctx, runtime); break;
    case 0x10de368u: ref::charrootdyn_getNoLimitMode(rdram, ctx, runtime); break;
    case 0x10de380u: ref::charrootdyn_getForceNoLimitMode(rdram, ctx, runtime); break;
    case 0x10df130u: ref::charrootdyn_isParentToEdge(rdram, ctx, runtime); break;
    case 0x10dfd18u: ref::manager_sub_010DFD18(rdram, ctx, runtime); break;
    case 0x10ed358u: ref::SetInitFogColClassFromStageData(rdram, ctx, runtime); break;
    case 0x10ed520u: ref::SetFogColAvailableFlag(rdram, ctx, runtime); break;
    case 0x10ed528u: ref::IsEnvelopeFogType(rdram, ctx, runtime); break;
    case 0x10ee768u: ref::GetJudgeVolumeClassWorkSize(rdram, ctx, runtime); break;
    case 0x10ee808u: ref::GetJudgeVolumeClassObjectGroup(rdram, ctx, runtime); break;
    case 0x10ee830u: ref::GetJudgeVolumeClassObjectGroupById(rdram, ctx, runtime); break;
    case 0x10f31b8u: ref::SetLayoutClassCollisionWorkAdr(rdram, ctx, runtime); break;
    case 0x10f3470u: ref::manager_sub_010F3470(rdram, ctx, runtime); break;
    case 0x10f3668u: ref::manager_sub_010F3668(rdram, ctx, runtime); break;
    case 0x10f5810u: ref::SetCameraResetSw(rdram, ctx, runtime); break;
    case 0x10f5af8u: ref::GetResetCameraFlagForDisplay(rdram, ctx, runtime); break;
    case 0x10f6358u: ref::GetLightColMan(rdram, ctx, runtime); break;
    case 0x10f6408u: ref::TaskExecLightColMan(rdram, ctx, runtime); break;
    case 0x10f7508u: ref::SetGameLightAvailable(rdram, ctx, runtime); break;
    case 0x10f7658u: ref::ForceGameLightFlagEnvelope(rdram, ctx, runtime); break;
    case 0x10f7678u: ref::IsEnvelopeGameLightType(rdram, ctx, runtime); break;
    case 0x10f78a8u: ref::SetGameLightUserParam(rdram, ctx, runtime); break;
    case 0x10f78b0u: ref::SetGameLightUserFunc(rdram, ctx, runtime); break;
    case 0x10f8b40u: ref::SetGlobalLightAvailable(rdram, ctx, runtime); break;
    case 0x10f8c90u: ref::ForceGlobalLightFlagEnvelope(rdram, ctx, runtime); break;
    case 0x10f8cb0u: ref::IsEnvelopeGlobalLightType(rdram, ctx, runtime); break;
    case 0x10f9b58u: ref::SetLwoDemoOrientManagerObjCutFinishFlag(rdram, ctx, runtime); break;
    case 0x11008a0u: ref::GetLayoutObjGroup(rdram, ctx, runtime); break;
    case 0x11008b0u: ref::GetLayoutDispObjGroup(rdram, ctx, runtime); break;
    case 0x1100c48u: ref::GetSkeletonFocusNodeDirect(rdram, ctx, runtime); break;
    case 0x11016c0u: ref::AddObjectManagerSheetWorkSize(rdram, ctx, runtime); break;
    case 0x1101718u: ref::GetAmbientLightObjGroup(rdram, ctx, runtime); break;
    case 0x1102b00u: ref::GetScriptPriorityGroup(rdram, ctx, runtime); break;
    case 0x1103080u: ref::manager_sub_01103080(rdram, ctx, runtime); break;
    case 0x110a6d8u: ref::GetScriptAnimationGlobalBaseInfo(rdram, ctx, runtime); break;
    case 0x110a790u: ref::CopyScriptAnimationWindParam(rdram, ctx, runtime); break;
    case 0x110b818u: ref::IsNonISysObjTypeRes(rdram, ctx, runtime); break;
    case 0x110cc68u: ref::SetScriptCharDefaultScriptFunc(rdram, ctx, runtime); break;
    case 0x110de28u: ref::InitSortObj(rdram, ctx, runtime); break;
    case 0x1113ae8u: ref::setDefaultMenuControlButton(rdram, ctx, runtime); break;
    case 0x1114af0u: ref::InitViewIntervalClip(rdram, ctx, runtime); break;
    case 0x11280e0u: ref::CopyCollisionEdge(rdram, ctx, runtime); break;
    case 0x1128270u: ref::MakeCollisionEdgePointer(rdram, ctx, runtime); break;
    case 0x11282f0u: ref::IsClusterCollision(rdram, ctx, runtime); break;
    case 0x11283d0u: ref::MakeGeometryPointerCollisionEdgeInfo(rdram, ctx, runtime); break;
    case 0x11287d0u: ref::GetCollisionResource(rdram, ctx, runtime); break;
    case 0x1128818u: ref::CollisionObjectJudgeCallbackExceptOneObj(rdram, ctx, runtime); break;
    case 0x11293e0u: ref::manager_sub_011293E0(rdram, ctx, runtime); break;
    case 0x1129ae8u: ref::manager_sub_01129AE8(rdram, ctx, runtime); break;
    case 0x1129b00u: ref::FlushCollisionEdgeLinkDBVWallFinder(rdram, ctx, runtime); break;
    case 0x1129b08u: ref::GetCollisionEdgeLinkDBWorkSize(rdram, ctx, runtime); break;
    case 0x112a330u: ref::GetCollisionFinderWorkSize(rdram, ctx, runtime); break;
    case 0x11328a8u: ref::InitGeometryPointer(rdram, ctx, runtime); break;
    case 0x1132cb0u: ref::SetGeometryPointerChainPos(rdram, ctx, runtime); break;
    case 0x1132d58u: ref::CheckLegalCollisionGP(rdram, ctx, runtime); break;
    case 0x1134308u: ref::GetYHeightOfPlaneXZProjection(rdram, ctx, runtime); break;
    case 0x11398f0u: ref::CheckClothChainExist(rdram, ctx, runtime); break;
    case 0x11399d8u: ref::SetClothSingleFixedChainLeafFixNodeOffset(rdram, ctx, runtime); break;
    case 0x1147ba8u: ref::GetTwoWayLinkNext(rdram, ctx, runtime); break;
    case 0x1147bb0u: ref::RelocateTwoWayLinkHeader(rdram, ctx, runtime); break;
    case 0x1147bc8u: ref::GetTwoWayLinkData(rdram, ctx, runtime); break;
    case 0x1149460u: ref::mcReqSetWaitFunc(rdram, ctx, runtime); break;
    case 0x1149cf8u: ref::mcGetProcessPer(rdram, ctx, runtime); break;
    case 0x114a300u: ref::mcGetGameClearFlag(rdram, ctx, runtime); break;
    case 0x114a320u: ref::mcGetCompleteTimeAttack(rdram, ctx, runtime); break;
    case 0x114ad38u: ref::mouseSetCursorPosition(rdram, ctx, runtime); break;
    case 0x114ae78u: ref::mouseMoveCursorPosition(rdram, ctx, runtime); break;
    case 0x114b580u: ref::setVibrationEnableWorkAdrs(rdram, ctx, runtime); break;
    case 0x114b6e0u: ref::usbdevMouseGetStatus(rdram, ctx, runtime); break;
    case 0x1155b50u: ref::windowSemiTransRate(rdram, ctx, runtime); break;
    case 0x11560f8u: ref::windowGetCurrentWindowHandle(rdram, ctx, runtime); break;
    case 0x1156338u: ref::windowSetTex(rdram, ctx, runtime); break;
    case 0x1157870u: ref::adpcmOpenChkComplete(rdram, ctx, runtime); break;
    case 0x1157940u: ref::adpcmCloseReq(rdram, ctx, runtime); break;
    case 0x1157978u: ref::adpcmCloseFadeReq(rdram, ctx, runtime); break;
    case 0x11579d0u: ref::adpcmCloseFadeSetVol(rdram, ctx, runtime); break;
    case 0x1157a00u: ref::adpcmCloseChkComplete(rdram, ctx, runtime); break;
    case 0x1157a10u: ref::adpcmDeleteAdpcmSeq(rdram, ctx, runtime); break;
    case 0x1157a40u: ref::adpcmSetVol(rdram, ctx, runtime); break;
    case 0x1157a78u: ref::adpcmSetPitch(rdram, ctx, runtime); break;
    case 0x1157b78u: ref::adpcmGamePause(rdram, ctx, runtime); break;
    case 0x11587a8u: ref::seSetVol(rdram, ctx, runtime); break;
    case 0x1159008u: ref::manager_sub_01159008(rdram, ctx, runtime); break;
    case 0x11590b0u: ref::seDelayPlay3DTest(rdram, ctx, runtime); break;
    case 0x115bab8u: ref::sheetFsAddStageTexSegIndex(rdram, ctx, runtime); break;
    case 0x115bad8u: ref::sheetFsDecStageTexSegIndex(rdram, ctx, runtime); break;
    case 0x115bb98u: ref::sheetFsSetStageTexSegTexSegDef(rdram, ctx, runtime); break;
    case 0x115bbb0u: ref::sheetFsGetStageTexSegTexSegDef(rdram, ctx, runtime); break;
    case 0x115c1e0u: ref::sheetFsGetDirectFileSize(rdram, ctx, runtime); break;
    case 0x115cfc0u: ref::sheetFileGetSize(rdram, ctx, runtime); break;
    case 0x115ec48u: ref::sheetHeaderObjSearchSheetId(rdram, ctx, runtime); break;
    case 0x115ed10u: ref::sheetMgrAddSegLoadList(rdram, ctx, runtime); break;
    case 0x115ed28u: ref::sheetMgrGetSegLoadHeader(rdram, ctx, runtime); break;
    case 0x115ed38u: ref::sheetMgrAddSegInitList(rdram, ctx, runtime); break;
    case 0x115edb0u: ref::sheetMgrGetSegInitHeader(rdram, ctx, runtime); break;
    case 0x115fa20u: ref::sheetMgrSetNoWaitMode(rdram, ctx, runtime); break;
    case 0x1161c80u: ref::get_camera_lws_animation_param(rdram, ctx, runtime); break;
    case 0x1161f70u: ref::get_boot_obj_camera_lws_animation(rdram, ctx, runtime); break;
    case 0x1161f78u: ref::is_loop_camera_lws_animation(rdram, ctx, runtime); break;
    case 0x1161fa0u: ref::is_sub_type_camera_lws_animation(rdram, ctx, runtime); break;
    case 0x1162018u: ref::reset_main_type_camera_lws_animation_update_flag(rdram, ctx, runtime); break;
    case 0x1162150u: ref::is_same_camera_lws_animation(rdram, ctx, runtime); break;
    case 0x1162208u: ref::stop_camera_lws_animation_geometry_pointer_reference(rdram, ctx, runtime); break;
    case 0x1162238u: ref::stop_camera_lws_animation_callback(rdram, ctx, runtime); break;
    case 0x11629b8u: ref::manager_sub_011629B8(rdram, ctx, runtime); break;
    case 0x1162f10u: ref::get_clip_screen(rdram, ctx, runtime); break;
    case 0x1162f18u: ref::get_transform_matrix_set(rdram, ctx, runtime); break;
    case 0x1163808u: ref::get_change_grace_time_camera_region(rdram, ctx, runtime); break;
    case 0x1164260u: ref::manager_sub_01164260(rdram, ctx, runtime); break;
    case 0x1164468u: ref::set_screen_wide_and_progressive(rdram, ctx, runtime); break;
    case 0x11644c8u: ref::set_view_angle(rdram, ctx, runtime); break;
    case 0x11644f0u: ref::get_screen_width(rdram, ctx, runtime); break;
    case 0x11644f8u: ref::get_screen_height(rdram, ctx, runtime); break;
    case 0x1164500u: ref::get_aspect_ratio(rdram, ctx, runtime); break;
    case 0x1164530u: ref::get_view_angle(rdram, ctx, runtime); break;
    case 0x1164558u: ref::get_near_clip_z_clip_screen(rdram, ctx, runtime); break;
    case 0x1164570u: ref::get_far_clip_z_clip_screen(rdram, ctx, runtime); break;
    case 0x1164588u: ref::get_z_buf_min_clip_screen(rdram, ctx, runtime); break;
    case 0x1164598u: ref::get_z_buf_max_clip_screen(rdram, ctx, runtime); break;
    case 0x1164ca8u: ref::get_scissor_clip_matrix(rdram, ctx, runtime); break;
    case 0x1164cb8u: ref::get_WV_matrix(rdram, ctx, runtime); break;
    case 0x1166de8u: ref::ResetDamageClass(rdram, ctx, runtime); break;
    case 0x1166eb8u: ref::SetDamageClassDamageTableType(rdram, ctx, runtime); break;
    case 0x1169c30u: ref::getEditWorkInitData(rdram, ctx, runtime); break;
    case 0x1169d48u: ref::getPointData(rdram, ctx, runtime); break;
    case 0x1169de0u: ref::getLineData(rdram, ctx, runtime); break;
    case 0x1169e68u: ref::getRootPointData(rdram, ctx, runtime); break;
    case 0x1169ea8u: ref::getGoalPointData(rdram, ctx, runtime); break;
    case 0x1169ee8u: ref::getSubPointData(rdram, ctx, runtime); break;
    case 0x1169ff0u: ref::getInterestPointData(rdram, ctx, runtime); break;
    case 0x116a040u: ref::getDeadPointData(rdram, ctx, runtime); break;
    case 0x116a098u: ref::getLayoutPointData(rdram, ctx, runtime); break;
    case 0x1341920u: ref::getSnapShotFormat(rdram, ctx, runtime); break;
    case 0x1342b40u: ref::GAMEUTIL_get_sight_on_reverse_cntrlLR_bool(rdram, ctx, runtime); break;
    case 0x1342b70u: ref::GAMEUTIL_get_sight_on_reverse_cntrlUD_bool(rdram, ctx, runtime); break;
    case 0x1342c70u: ref::GAMEUTIL_get_game_camera_free_mode_reverse_cntrlLR_bool(rdram, ctx, runtime); break;
    case 0x1342ca0u: ref::GAMEUTIL_get_game_camera_free_mode_reverse_cntrlUD_bool(rdram, ctx, runtime); break;
    case 0x1343b28u: ref::GAMEUTIL_set_auto_get_flag_boss_interest_position(rdram, ctx, runtime); break;
    case 0x1343cd0u: ref::GAMEUTIL_reset_move_angle(rdram, ctx, runtime); break;
    case 0x1343cd8u: ref::GAMEUTIL_set_parameter_move_angle(rdram, ctx, runtime); break;
    case 0x1343d18u: ref::GAMEUTIL_begin_move_angle(rdram, ctx, runtime); break;
    case 0x1343e10u: ref::GAMEUTIL_get_cur_velocity_move_angle(rdram, ctx, runtime); break;
    case 0x1343ee8u: ref::GAMEUTIL_set_need_dir_ratio_flag_look_at_vs_offset(rdram, ctx, runtime); break;
    case 0x1343ef0u: ref::GAMEUTIL_set_enable_side_change_angle_look_at_vs_offset(rdram, ctx, runtime); break;
    case 0x13440a8u: ref::GAMEUTIL_set_parameter_col_plane_info_filter(rdram, ctx, runtime); break;
    case 0x1344208u: ref::GAMEUTIL_set_parameter_position_filter(rdram, ctx, runtime); break;
    case 0x13445c0u: ref::GAMEUTIL_reset_status_manager(rdram, ctx, runtime); break;
    case 0x1344628u: ref::GAMEUTIL_is_status_change_status_manager(rdram, ctx, runtime); break;
    case 0x1344648u: ref::GAMEUTIL_get_status_continue_time_status_manager(rdram, ctx, runtime); break;
    case 0x1344650u: ref::GAMEUTIL_get_cur_status_status_manager(rdram, ctx, runtime); break;
    case 0x1344658u: ref::GAMEUTIL_get_pre_status_status_manager(rdram, ctx, runtime); break;
    case 0x1344660u: ref::GAMEUTIL_get_pre_changed_status_status_manager(rdram, ctx, runtime); break;
    case 0x1344668u: ref::getSightOnRevCntrlLR(rdram, ctx, runtime); break;
    case 0x1344678u: ref::getSightOnRevCntrlUD(rdram, ctx, runtime); break;
    case 0x1344698u: ref::getFreeModeRevControlLR(rdram, ctx, runtime); break;
    case 0x13446a8u: ref::getFreeModeRevControlUD(rdram, ctx, runtime); break;
    case 0x13446b8u: ref::getFreeModeRevAngleVelRatio(rdram, ctx, runtime); break;
    case 0x1345d40u: ref::GetGameTimeScaleRatio(rdram, ctx, runtime); break;
    case 0x1345e38u: ref::GetCurrentGameAnimeTimeScaleRatio(rdram, ctx, runtime); break;
    case 0x1345e48u: ref::GetGameAnimeTimeScaleRatio(rdram, ctx, runtime); break;
    case 0x1345e58u: ref::GetPlayerControlButtonContext(rdram, ctx, runtime); break;
    case 0x1345e78u: ref::RestoreDefaultGameAnimeTimeScaleRatio(rdram, ctx, runtime); break;
    case 0x1345e90u: ref::SetDefaultGameAnimeTimeScaleRatio(rdram, ctx, runtime); break;
    case 0x1345ed0u: ref::getFrameRateMode(rdram, ctx, runtime); break;
    case 0x1346028u: ref::setDefaultPlayerControlButton(rdram, ctx, runtime); break;
    case 0x1346210u: ref::getFadeValue(rdram, ctx, runtime); break;
    case 0x1346df0u: ref::set_yaw_pitch_input_border_parameter_nico_yaw_pitch_cntrl(rdram, ctx, runtime); break;
    case 0x1346fe8u: ref::set_base_yaw_parameter_nico_yaw_pitch_cntrl(rdram, ctx, runtime); break;
    case 0x1347358u: ref::set_yaw_parameter_effective_ratio_nico_yaw_pitch_cntrl(rdram, ctx, runtime); break;
    case 0x1347360u: ref::set_pitch_parameter_effective_ratio_nico_yaw_pitch_cntrl(rdram, ctx, runtime); break;
    case 0x1347598u: ref::set_temp_stop_base_rot_flag_nico_yaw_pitch_cntrl(rdram, ctx, runtime); break;
    case 0x1347838u: ref::is_enable_base_yaw_rot_nico_yaw_pitch_cntrl(rdram, ctx, runtime); break;
    case 0x1347840u: ref::is_enable_base_pitch_rot_nico_yaw_pitch_cntrl(rdram, ctx, runtime); break;
    case 0x1347848u: ref::get_base_yaw_move_nico_yaw_pitch_cntrl(rdram, ctx, runtime); break;
    case 0x1347858u: ref::set_base_yaw_nico_yaw_pitch_cntrl(rdram, ctx, runtime); break;
    case 0x1347860u: ref::set_base_pitch_nico_yaw_pitch_cntrl(rdram, ctx, runtime); break;
    case 0x1347868u: ref::get_base_yaw_nico_yaw_pitch_cntrl(rdram, ctx, runtime); break;
    case 0x1347870u: ref::get_base_pitch_nico_yaw_pitch_cntrl(rdram, ctx, runtime); break;
    case 0x13494a8u: ref::backStageGetBossSaveArea(rdram, ctx, runtime); break;
    case 0x13494b8u: ref::backStageInit(rdram, ctx, runtime); break;
    case 0x134a358u: ref::bossStatGetDeadBoss(rdram, ctx, runtime); break;
    case 0x134a778u: ref::CheckCamFinalizerReady(rdram, ctx, runtime); break;
    case 0x134a788u: ref::SetCamFinalizerReady(rdram, ctx, runtime); break;
    case 0x134a7c0u: ref::PauseCamFinalizer(rdram, ctx, runtime); break;
    case 0x134b260u: ref::CheckDemoCameraOrientFinished(rdram, ctx, runtime); break;
    case 0x134b268u: ref::getDemoEnableFlag(rdram, ctx, runtime); break;
    case 0x134c838u: ref::demoscrCheckDemoFinish(rdram, ctx, runtime); break;
    case 0x134c850u: ref::demoscrCheckDemoCutSkip(rdram, ctx, runtime); break;
    case 0x134d728u: ref::endingflowTickAdpcmSeq(rdram, ctx, runtime); break;
    case 0x134d920u: ref::gamecore_sub_0134D920(rdram, ctx, runtime); break;
    case 0x134db40u: ref::initFadeParam(rdram, ctx, runtime); break;
    case 0x134db68u: ref::setFadeParam(rdram, ctx, runtime); break;
    case 0x134db98u: ref::calcFadeParam(rdram, ctx, runtime); break;
    case 0x134dbe8u: ref::checkFadeParam(rdram, ctx, runtime); break;
    case 0x134dc20u: ref::fadeChkStat(rdram, ctx, runtime); break;
    case 0x134e390u: ref::setTreeId(rdram, ctx, runtime); break;
    case 0x134ef80u: ref::_optionWorkChk(rdram, ctx, runtime); break;
    case 0x134ef98u: ref::_optionWorkPtr(rdram, ctx, runtime); break;
    case 0x134f0d8u: ref::_gameIWorkSet(rdram, ctx, runtime); break;
    case 0x134f108u: ref::_gameWorkChk(rdram, ctx, runtime); break;
    case 0x134f138u: ref::_gameIWorkChk(rdram, ctx, runtime); break;
    case 0x134f238u: ref::setBossClearCnt(rdram, ctx, runtime); break;
    case 0x134f278u: ref::getGameClearCnt(rdram, ctx, runtime); break;
    case 0x134f2a8u: ref::getGameHardMode(rdram, ctx, runtime); break;
    case 0x134f320u: ref::setSavedClock(rdram, ctx, runtime); break;
    case 0x134f330u: ref::getSavedPlaceId(rdram, ctx, runtime); break;
    case 0x134fe00u: ref::getOptionDataAdrsAndSize(rdram, ctx, runtime); break;
    case 0x13521e0u: ref::gameflowSelectGameoverMenu(rdram, ctx, runtime); break;
    case 0x1352418u: ref::gameflowSetPauseEnable(rdram, ctx, runtime); break;
    case 0x13524a8u: ref::gameflowGameOver(rdram, ctx, runtime); break;
    case 0x13524b8u: ref::gameflowGetJumpSekiban(rdram, ctx, runtime); break;
    case 0x13524c8u: ref::gameflowSetJumpSekiban(rdram, ctx, runtime); break;
    case 0x1352770u: ref::gamecore_sub_01352770(rdram, ctx, runtime); break;
    case 0x1353570u: ref::gStatGetOmakeStatAdrs(rdram, ctx, runtime); break;
    case 0x13535d8u: ref::gStatGObjDispChk(rdram, ctx, runtime); break;
    case 0x13536d0u: ref::gStatSetGObjAdrs(rdram, ctx, runtime); break;
    case 0x1353730u: ref::gStatGetPlayerAccessoriesDamgeRaitoToPlayer(rdram, ctx, runtime); break;
    case 0x1353768u: ref::gStatGetPlayerAccessoriesDamgeRaitoToBoss(rdram, ctx, runtime); break;
    case 0x1353e00u: ref::gstatBootMcSetExistNicoData(rdram, ctx, runtime); break;
    case 0x1353ef8u: ref::gstatBootMcSetExistIcoData(rdram, ctx, runtime); break;
    case 0x1353f08u: ref::gstatBootMcChkExistIcoData(rdram, ctx, runtime); break;
    case 0x13549a0u: ref::jumphokoraflowDemoStart(rdram, ctx, runtime); break;
    case 0x13549b0u: ref::jumphokoraflowDemoEnd(rdram, ctx, runtime); break;
    case 0x13549d0u: ref::jumphokoraGetDemoInfo(rdram, ctx, runtime); break;
    case 0x1356570u: ref::gcCheckStageLoadFinish(rdram, ctx, runtime); break;
    case 0x1357648u: ref::opflowOp1End(rdram, ctx, runtime); break;
    case 0x1357660u: ref::opflowOp2Start(rdram, ctx, runtime); break;
    case 0x1357678u: ref::opflowOp2End(rdram, ctx, runtime); break;
    case 0x13576a8u: ref::opflowChkTitleDemoStart(rdram, ctx, runtime); break;
    case 0x13576c0u: ref::opflowTitleDmoeEnd(rdram, ctx, runtime); break;
    case 0x13576f8u: ref::opflowChkAbort(rdram, ctx, runtime); break;
    case 0x1357708u: ref::opflowChkLoadStart(rdram, ctx, runtime); break;
    case 0x1358248u: ref::getPlayerHp(rdram, ctx, runtime); break;
    case 0x13584e8u: ref::getPlayerGrip(rdram, ctx, runtime); break;
    case 0x1358778u: ref::initPowerWork(rdram, ctx, runtime); break;
    case 0x1358788u: ref::resetAttackPower(rdram, ctx, runtime); break;
    case 0x1358798u: ref::getAttackPowerRatio(rdram, ctx, runtime); break;
    case 0x1358840u: ref::setFullPower(rdram, ctx, runtime); break;
    case 0x1358878u: ref::gamecore_sub_01358878(rdram, ctx, runtime); break;
    case 0x135d740u: ref::getAreaSize(rdram, ctx, runtime); break;
    case 0x135d9c0u: ref::getPosOnMap(rdram, ctx, runtime); break;
    case 0x135dae8u: ref::getSeamlessStageHighModelDispDistDefault(rdram, ctx, runtime); break;
    case 0x135daf8u: ref::getSeamlessStageMiddleModelDispDistDefault(rdram, ctx, runtime); break;
    case 0x135dd58u: ref::getSeamlessStageEnv(rdram, ctx, runtime); break;
    case 0x135df88u: ref::forceLoadSeamlessStage(rdram, ctx, runtime); break;
    case 0x135df98u: ref::SetSeamlessStageControlObject(rdram, ctx, runtime); break;
    case 0x135e138u: ref::PauseSeamlessStageManager(rdram, ctx, runtime); break;
    case 0x135e280u: ref::getStageIDWithOffset(rdram, ctx, runtime); break;
    case 0x135e608u: ref::isSeamlessStage(rdram, ctx, runtime); break;
    case 0x135efb0u: ref::getSpecialEffectObjWork(rdram, ctx, runtime); break;
    case 0x1365d08u: ref::GetWorldLayoutSheetSegHeader(rdram, ctx, runtime); break;
    case 0x1366b60u: ref::stagemanager_getSegmentVariation(rdram, ctx, runtime); break;
    case 0x1366d00u: ref::initWorldLayout(rdram, ctx, runtime); break;
    case 0x136a2d0u: ref::bgmEnvTickProckMask(rdram, ctx, runtime); break;
    case 0x136a378u: ref::bossNormalHook(rdram, ctx, runtime); break;
    case 0x136a388u: ref::gamecore_sub_0136A388(rdram, ctx, runtime); break;
    case 0x136aa38u: ref::bgmPlay(rdram, ctx, runtime); break;
    case 0x136aa50u: ref::bgmStop(rdram, ctx, runtime); break;
    case 0x136aa70u: ref::bgmStopChk(rdram, ctx, runtime); break;
    case 0x136aad0u: ref::bgmChangeVol(rdram, ctx, runtime); break;
    case 0x136e4f8u: ref::gamecore_sub_0136E4F8(rdram, ctx, runtime); break;
    case 0x136e7a0u: ref::gamecore_sub_0136E7A0(rdram, ctx, runtime); break;
    case 0x13757f8u: ref::nico_IK_idDetect(rdram, ctx, runtime); break;
    case 0x137b738u: ref::gamecore_sub_0137B738(rdram, ctx, runtime); break;
    case 0x137b800u: ref::menuSelected(rdram, ctx, runtime); break;
    case 0x137c310u: ref::GetDebugDispType(rdram, ctx, runtime); break;
    case 0x137e9f0u: ref::is_LwsOrientObj(rdram, ctx, runtime); break;
    case 0x1384de0u: ref::CheckDeathTentacle(rdram, ctx, runtime); break;
    case 0x1388048u: ref::getEffectBloodWorkSize(rdram, ctx, runtime); break;
    case 0x1388060u: ref::getWeakPointEffectIntensity(rdram, ctx, runtime); break;
    case 0x1388080u: ref::getTriggerPointEffectIntensity(rdram, ctx, runtime); break;
    case 0x13885f0u: ref::effectClearDamageWork(rdram, ctx, runtime); break;
    case 0x1388600u: ref::effectGetDamageWork(rdram, ctx, runtime); break;
    case 0x13886b8u: ref::gamecore_sub_013886B8(rdram, ctx, runtime); break;
    case 0x138ea10u: ref::getFallenLeafSize(rdram, ctx, runtime); break;
    case 0x138eb48u: ref::getArrowWaterSize(rdram, ctx, runtime); break;
    case 0x138f138u: ref::gamecore_sub_0138F138(rdram, ctx, runtime); break;
    case 0x1395e00u: ref::setHorseAIStopFlag(rdram, ctx, runtime); break;
    case 0x1395e10u: ref::getHorseStandByMode(rdram, ctx, runtime); break;
    case 0x1396328u: ref::SCRFuncHorseStandByFlag(rdram, ctx, runtime); break;
    case 0x139a878u: ref::ColCont_Reset(rdram, ctx, runtime); break;
    case 0x139aa00u: ref::ColCont2_IsColStatus(rdram, ctx, runtime); break;
    case 0x139aa50u: ref::ColCont2_IsEventColStatus(rdram, ctx, runtime); break;
    case 0x139ac08u: ref::MotSpdCtrl_Reset(rdram, ctx, runtime); break;
    case 0x139ac60u: ref::MotSpdCtrl_SetParam(rdram, ctx, runtime); break;
    case 0x139e988u: ref::inInNowLoadingEffect(rdram, ctx, runtime); break;
    case 0x13a75f8u: ref::DirectTurn_Reset(rdram, ctx, runtime); break;
    case 0x13aa380u: ref::is_doing_nico_sight_on(rdram, ctx, runtime); break;
    case 0x13b53b0u: ref::getPlayerLastAttrStatus(rdram, ctx, runtime); break;
    case 0x13b53c0u: ref::getPlayerLastParentObj(rdram, ctx, runtime); break;
    case 0x13ba418u: ref::nico_setPlayerDemoAdditionClothCollisionFloorTable1(rdram, ctx, runtime); break;
    case 0x13ba458u: ref::nico_setPlayerDemoAdditionClothCollisionRightArm(rdram, ctx, runtime); break;
    case 0x13ba4b0u: ref::nico_setPlayerDemoAdditionClothCollisionRightHandAndSwordDeep(rdram, ctx, runtime); break;
    case 0x13ba508u: ref::nico_setPlayerDemoAdditionClothCollisionRightHandAndSword(rdram, ctx, runtime); break;
    case 0x13ba768u: ref::nico_resetPlayerDemoAdditionClothCollision(rdram, ctx, runtime); break;
    case 0x13cd5f8u: ref::player_initSlipEffect(rdram, ctx, runtime); break;
    case 0x13d0ab8u: ref::bossCloudStart(rdram, ctx, runtime); break;
    case 0x13d0bc0u: ref::bossDemoPlayerBreakDownAccept(rdram, ctx, runtime); break;
    case 0x13d0bd0u: ref::bossDemoPlayerBreakDownEnd(rdram, ctx, runtime); break;
    case 0x13d38a0u: ref::AI_InitAnimalOptimize(rdram, ctx, runtime); break;
    case 0x13d3a48u: ref::AI_InitAnimalWork(rdram, ctx, runtime); break;
    case 0x13d3a68u: ref::AI_SetAnimalWorkParam(rdram, ctx, runtime); break;
    case 0x13d3a70u: ref::AI_InitGexCommonWork(rdram, ctx, runtime); break;
    case 0x13d3a88u: ref::AI_InitSurpriseArrow(rdram, ctx, runtime); break;
    case 0x13d3a98u: ref::AI_SetSurprisedArea(rdram, ctx, runtime); break;
    case 0x13d3da0u: ref::AI_UpdateVelocity(rdram, ctx, runtime); break;
    case 0x13d4008u: ref::AI_GetAccelB(rdram, ctx, runtime); break;
    case 0x13d4060u: ref::AI_GetAccelA(rdram, ctx, runtime); break;
    case 0x13d40a8u: ref::AI_GetAccelA_at0(rdram, ctx, runtime); break;
    case 0x13d40f0u: ref::AI_GetAccelB_at0(rdram, ctx, runtime); break;
    case 0x13d4128u: ref::AI_GetAccelA3(rdram, ctx, runtime); break;
    case 0x13d4298u: ref::AI_GetAccelB3(rdram, ctx, runtime); break;
    case 0x13d43f0u: ref::AI_GetAccelC3(rdram, ctx, runtime); break;
    case 0x13d4540u: ref::AI_GetTimeFromAccelC3(rdram, ctx, runtime); break;
    case 0x13d45f0u: ref::AI_GetVelocity3(rdram, ctx, runtime); break;
    case 0x13d4ce0u: ref::AI_GetAreaDistScale(rdram, ctx, runtime); break;
    case 0x13d4d08u: ref::AI_GetAreaDistScale2(rdram, ctx, runtime); break;
    case 0x13d5300u: ref::AI_GetNegaVector(rdram, ctx, runtime); break;
    case 0x13e7c00u: ref::Kite_SetAnimalID_KitePathWork(rdram, ctx, runtime); break;
    case 0x13e7c08u: ref::Kite_SetEventIDList(rdram, ctx, runtime); break;
    case 0x13e7c28u: ref::Kite_InitEventCheck(rdram, ctx, runtime); break;
    case 0x13e7c40u: ref::Kite_ResetEventFrameCheck(rdram, ctx, runtime); break;
    case 0x13e7c50u: ref::Kite_StartEventCheck(rdram, ctx, runtime); break;
    case 0x13e7c68u: ref::Kite_EndEventCheck(rdram, ctx, runtime); break;
    case 0x13e7cf8u: ref::Kite_SucceedKitePathWork(rdram, ctx, runtime); break;
    case 0x13f0f90u: ref::setSmallManagerSystemLock(rdram, ctx, runtime); break;
    case 0x13f1190u: ref::gamecore_sub_013F1190(rdram, ctx, runtime); break;
    case 0x13f1938u: ref::gamecore_sub_013F1938(rdram, ctx, runtime); break;
    case 0x13f62c8u: ref::getNowExecEventScriptObj(rdram, ctx, runtime); break;
    case 0x13f62f0u: ref::setEventScriptEnable(rdram, ctx, runtime); break;
    case 0x13fa530u: ref::setPlayerStatusSwitch(rdram, ctx, runtime); break;
    case 0x13fa628u: ref::startAttackEffect(rdram, ctx, runtime); break;
    case 0x13fa930u: ref::bosslifebar_display(rdram, ctx, runtime); break;
    case 0x13fa968u: ref::bosslifebar_resethp(rdram, ctx, runtime); break;
    case 0x13fab00u: ref::PlayerSprStatusEnd(rdram, ctx, runtime); break;
    case 0x13fab68u: ref::BossSprStatusEnd(rdram, ctx, runtime); break;
    case 0x13ff148u: ref::GetSwordFlareHitPlaneAttribute(rdram, ctx, runtime); break;
    case 0x1407310u: ref::getSelectWeapon(rdram, ctx, runtime); break;
    case 0x140ae40u: ref::getSprLogWorkAddress(rdram, ctx, runtime); break;
    case 0x140ae58u: ref::getSprLogStatusAddress(rdram, ctx, runtime); break;
    case 0x140af68u: ref::initPauseMenuMap(rdram, ctx, runtime); break;
    case 0x140afb0u: ref::setLoadMapOffset(rdram, ctx, runtime); break;
    case 0x140b0c0u: ref::isInLoadWait(rdram, ctx, runtime); break;
    case 0x140b0d0u: ref::LoadWaitCancel(rdram, ctx, runtime); break;
    case 0x140ee50u: ref::acted_camera_warp(rdram, ctx, runtime); break;
    case 0x140fe00u: ref::gamecore_sub_0140FE00(rdram, ctx, runtime); break;
    case 0x140fe20u: ref::gamecore_sub_0140FE20(rdram, ctx, runtime); break;
    case 0x140fe50u: ref::gamecore_sub_0140FE50(rdram, ctx, runtime); break;
    case 0x140fe70u: ref::gamecore_sub_0140FE70(rdram, ctx, runtime); break;
    case 0x1411448u: ref::reset_time_interp_param_camera_dist_to_look_at(rdram, ctx, runtime); break;
    case 0x1411480u: ref::is_acting_rewind_camera_dist_to_look_at(rdram, ctx, runtime); break;
    case 0x1411490u: ref::get_rewind_begin_dist_to_target_camera_dist_to_look_at(rdram, ctx, runtime); break;
    case 0x14114b0u: ref::end_rewind_camera_dist_to_look_at(rdram, ctx, runtime); break;
    case 0x14114b8u: ref::is_leave_camera_dist_to_look_at(rdram, ctx, runtime); break;
    case 0x1412a60u: ref::request_end_camera_pause_rise(rdram, ctx, runtime); break;
    case 0x1412a70u: ref::is_end_camera_pause_rise(rdram, ctx, runtime); break;
    case 0x1412a80u: ref::get_begin_interp_finish_time_camera_pause_rise(rdram, ctx, runtime); break;
    case 0x1413340u: ref::get_place_status_camera_place_status(rdram, ctx, runtime); break;
    case 0x1413348u: ref::get_manage_obj_camera_place_status(rdram, ctx, runtime); break;
    case 0x1413408u: ref::get_hit_camera_region_camera_place_status(rdram, ctx, runtime); break;
    case 0x14134b8u: ref::get_on_boss_camera_col_attr_camera_place_status(rdram, ctx, runtime); break;
    case 0x14134d0u: ref::is_approach_rise_ok_camera_place_status(rdram, ctx, runtime); break;
    case 0x14135b8u: ref::is_cliff_rise_delay_camera_place_status(rdram, ctx, runtime); break;
    case 0x1413668u: ref::act_horse_look_at_ofst_decrease_camera_place_status(rdram, ctx, runtime); break;
    case 0x14136b8u: ref::act_look_at_ofst_cut_camera_place_status(rdram, ctx, runtime); break;
    case 0x1413708u: ref::gamecore_sub_01413708(rdram, ctx, runtime); break;
    case 0x1414200u: ref::CAM_get_interp_slope_pitch_ofst(rdram, ctx, runtime); break;
    case 0x14144d0u: ref::gamecore_sub_014144D0(rdram, ctx, runtime); break;
    case 0x1414e60u: ref::get_attr_camera_target_lost_angle(rdram, ctx, runtime); break;
    case 0x1415900u: ref::set_parameter_camera_target_visible_angle_manager(rdram, ctx, runtime); break;
    case 0x1415a40u: ref::end_get_yaw_pitch_camera_target_visible_angle_manager(rdram, ctx, runtime); break;
    case 0x1415ca0u: ref::gamecore_sub_01415CA0(rdram, ctx, runtime); break;
    case 0x141a108u: ref::CAM_copy_camera_base_data(rdram, ctx, runtime); break;
    case 0x141a160u: ref::CAM_interp_reset_aim_rot_common(rdram, ctx, runtime); break;
    case 0x141a658u: ref::CAM_motion_kind_is_look_around_motion_target(rdram, ctx, runtime); break;
    case 0x141a678u: ref::CAM_motion_kind_is_accept_auto_rot_motion_target(rdram, ctx, runtime); break;
    case 0x141a7b8u: ref::CAM_get_target_obj(rdram, ctx, runtime); break;
    case 0x141a7c0u: ref::CAM_get_target_riding_obj(rdram, ctx, runtime); break;
    case 0x141a7c8u: ref::CAM_get_ground_contact_obj(rdram, ctx, runtime); break;
    case 0x141aa80u: ref::CAM_is_visible_now_target_visibility(rdram, ctx, runtime); break;
    case 0x141aa90u: ref::CAM_is_return_cam_posi_target_visibility(rdram, ctx, runtime); break;
    case 0x141aad0u: ref::CAM_reset_camera_warp_cntrl(rdram, ctx, runtime); break;
    case 0x141aca8u: ref::CAM_set_invisible_warp_apply_range_camera_warp_cntrl(rdram, ctx, runtime); break;
    case 0x141acb8u: ref::CAM_act_warp_order_camera_warp_cntrl(rdram, ctx, runtime); break;
    case 0x141b010u: ref::CAM_is_acting_view_cut_avoid_cntrl(rdram, ctx, runtime); break;
    case 0x141b020u: ref::CAM_set_param_view_cut_avoid_cntrl(rdram, ctx, runtime); break;
    case 0x141b030u: ref::CAM_reset_obstacle_avoid_cntrl(rdram, ctx, runtime); break;
    case 0x141b428u: ref::gamecore_sub_0141B428(rdram, ctx, runtime); break;
    case 0x141b480u: ref::gamecore_sub_0141B480(rdram, ctx, runtime); break;
    case 0x141b978u: ref::gamecore_sub_0141B978(rdram, ctx, runtime); break;
    case 0x141b988u: ref::gamecore_sub_0141B988(rdram, ctx, runtime); break;
    case 0x141bb20u: ref::gamecore_sub_0141BB20(rdram, ctx, runtime); break;
    case 0x141bcf8u: ref::gamecore_sub_0141BCF8(rdram, ctx, runtime); break;
    case 0x141c0a8u: ref::gamecore_sub_0141C0A8(rdram, ctx, runtime); break;
    case 0x141c9b0u: ref::get_view_angle_camera_view_angle_cntrl(rdram, ctx, runtime); break;
    case 0x141cd68u: ref::get_camera_obj(rdram, ctx, runtime); break;
    case 0x141ce48u: ref::get_player_collision_group_cameraman(rdram, ctx, runtime); break;
    case 0x141ce80u: ref::is_camera_target_change(rdram, ctx, runtime); break;
    case 0x141ce98u: ref::is_camera_target_debug_move(rdram, ctx, runtime); break;
    case 0x141d370u: ref::get_main_cameraman(rdram, ctx, runtime); break;
    case 0x1424e18u: ref::reset_game_camera(rdram, ctx, runtime); break;
    case 0x1424e28u: ref::get_game_camera_cur_mode(rdram, ctx, runtime); break;
    case 0x1424e30u: ref::is_reset_or_lws_movie_game_camera(rdram, ctx, runtime); break;
    case 0x1425028u: ref::reset_game_camera_standby_flag(rdram, ctx, runtime); break;
    case 0x1425030u: ref::set_object_interest_mode_target_game_camera(rdram, ctx, runtime); break;
    case 0x1425038u: ref::get_object_interest_mode_target_game_camera(rdram, ctx, runtime); break;
    case 0x1425040u: ref::begin_object_interest_game_camera(rdram, ctx, runtime); break;
    case 0x14252d8u: ref::begin_stream_lws_movie_playing_game_camera(rdram, ctx, runtime); break;
    case 0x14252e8u: ref::end_stream_lws_movie_playing_game_camera(rdram, ctx, runtime); break;
    case 0x1425548u: ref::is_view_angle_exist_lws_anime_mode_base(rdram, ctx, runtime); break;
    case 0x1425568u: ref::get_view_angle_lws_anime_mode_base(rdram, ctx, runtime); break;
    case 0x14256a0u: ref::getBossInterestKey(rdram, ctx, runtime); break;
    case 0x14256b0u: ref::getInterestKey(rdram, ctx, runtime); break;
    case 0x14256c0u: ref::getZoomKey(rdram, ctx, runtime); break;
    case 0x14256d0u: ref::setBossInterestKey(rdram, ctx, runtime); break;
    case 0x14256e0u: ref::setInterestKey(rdram, ctx, runtime); break;
    case 0x14256f0u: ref::setZoomKey(rdram, ctx, runtime); break;
    case 0x1425700u: ref::gamecore_sub_01425700(rdram, ctx, runtime); break;
    case 0x1425788u: ref::gamecore_sub_01425788(rdram, ctx, runtime); break;
    case 0x14257a8u: ref::gamecore_sub_014257A8(rdram, ctx, runtime); break;
    case 0x1425ac0u: ref::gamecore_sub_01425AC0(rdram, ctx, runtime); break;
    case 0x14262a0u: ref::gamecore_sub_014262A0(rdram, ctx, runtime); break;
    case 0x1426b18u: ref::gamecore_sub_01426B18(rdram, ctx, runtime); break;
    case 0x1426b20u: ref::gamecore_sub_01426B20(rdram, ctx, runtime); break;
    case 0x1427230u: ref::gamecore_sub_01427230(rdram, ctx, runtime); break;
    case 0x1428cd0u: ref::gamecore_sub_01428CD0(rdram, ctx, runtime); break;
    case 0x14296f8u: ref::gamecore_sub_014296F8(rdram, ctx, runtime); break;
    case 0x1429bc0u: ref::gamecore_sub_01429BC0(rdram, ctx, runtime); break;
    case 0x142b968u: ref::gamecore_sub_0142B968(rdram, ctx, runtime); break;
    case 0x142c118u: ref::is_base_mode_manager_update_game_camera_param_manager(rdram, ctx, runtime); break;
    case 0x142c128u: ref::get_interp_time_game_camera_param_manager(rdram, ctx, runtime); break;
    case 0x142c248u: ref::act_camera_cutout_game_camera_param_manager(rdram, ctx, runtime); break;
    case 0x142c270u: ref::drop_camera_cutout_flag_game_camera_param_manager(rdram, ctx, runtime); break;
    case 0x142c278u: ref::constructor_game_camera_param_update_data_manager(rdram, ctx, runtime); break;
    case 0x142c398u: ref::is_already_set_game_camera_param_update_data_manager(rdram, ctx, runtime); break;
    case 0x142c3b8u: ref::constructor_game_camera_mode_local_parameter(rdram, ctx, runtime); break;
    case 0x142c440u: ref::gamecore_sub_0142C440(rdram, ctx, runtime); break;
    case 0x142c530u: ref::gamecore_sub_0142C530(rdram, ctx, runtime); break;
    case 0x1431c38u: ref::setBossPathColiGroup(rdram, ctx, runtime); break;
    case 0x1431c48u: ref::setHorsePathColiGroup(rdram, ctx, runtime); break;
    case 0x1431c70u: ref::getPathColiGroupByType(rdram, ctx, runtime); break;
    case 0x1431ce0u: ref::getPassageWork(rdram, ctx, runtime); break;
    case 0x1431d18u: ref::getPassageNum(rdram, ctx, runtime); break;
    case 0x1431ed0u: ref::setRealColJudgeFunc(rdram, ctx, runtime); break;
    case 0x1431ed8u: ref::getRealColJudgeFunc(rdram, ctx, runtime); break;
    case 0x1431ee0u: ref::setSelfColAndNearMinDist(rdram, ctx, runtime); break;
    case 0x1431f38u: ref::setStabilizeYPathWork(rdram, ctx, runtime); break;
    case 0x1431f40u: ref::setAngleStabilizeYPathWork(rdram, ctx, runtime); break;
    case 0x1431f48u: ref::setRayByRealColPathWork(rdram, ctx, runtime); break;
    case 0x1431f50u: ref::setPassageWideRayPathWork(rdram, ctx, runtime); break;
    case 0x1431f58u: ref::setExecRayChkPathWork(rdram, ctx, runtime); break;
    case 0x1431fa8u: ref::setForceRouteAttrPathWork(rdram, ctx, runtime); break;
    case 0x1431fb0u: ref::setRouteColCheckPathWork(rdram, ctx, runtime); break;
    case 0x1431fb8u: ref::setRouteLockPathWork(rdram, ctx, runtime); break;
    case 0x1431fc0u: ref::setPlayerWayColCheckPathWork(rdram, ctx, runtime); break;
    case 0x1431fc8u: ref::setFollowModePathWork(rdram, ctx, runtime); break;
    case 0x1432018u: ref::setUserPathMode(rdram, ctx, runtime); break;
    case 0x1432020u: ref::setPassageRangeData(rdram, ctx, runtime); break;
    case 0x1432050u: ref::setAttainRangeStruct(rdram, ctx, runtime); break;
    case 0x14320b0u: ref::setCreateTempAttain(rdram, ctx, runtime); break;
    case 0x1432700u: ref::setAttainModePathWork(rdram, ctx, runtime); break;
    case 0x1432708u: ref::setAttainRadPathWork(rdram, ctx, runtime); break;
    case 0x1432710u: ref::getPathInfo(rdram, ctx, runtime); break;
    case 0x1432740u: ref::getWayFinish(rdram, ctx, runtime); break;
    case 0x1432760u: ref::getPathStatus(rdram, ctx, runtime); break;
    case 0x1432808u: ref::getLastPassagePointIdx(rdram, ctx, runtime); break;
    case 0x1432810u: ref::getLastPassageAreaIdx(rdram, ctx, runtime); break;
    case 0x1432818u: ref::getNowPassagePointIdx(rdram, ctx, runtime); break;
    case 0x1432820u: ref::getNowPassageAreaIdx(rdram, ctx, runtime); break;
    case 0x14328a8u: ref::setSearchRadius(rdram, ctx, runtime); break;
    case 0x14328d0u: ref::setSearchPointMax(rdram, ctx, runtime); break;
    case 0x14328e8u: ref::setPathRayWideParam(rdram, ctx, runtime); break;
    case 0x1433120u: ref::resetPassageWork(rdram, ctx, runtime); break;
    case 0x1433f60u: ref::getColiGroupByUserPathWork(rdram, ctx, runtime); break;
    case 0x1434cf8u: ref::gamecore_sub_01434CF8(rdram, ctx, runtime); break;
    case 0x1434f90u: ref::gamecore_sub_01434F90(rdram, ctx, runtime); break;
    case 0x14359c8u: ref::setUserPathExMode(rdram, ctx, runtime); break;
    case 0x14359d0u: ref::setUserPathGroupID(rdram, ctx, runtime); break;
    case 0x1435a50u: ref::setGPSelectMode(rdram, ctx, runtime); break;
    case 0x1435a58u: ref::setForceInitPathEx(rdram, ctx, runtime); break;
    case 0x1435a60u: ref::setGpReferencePathEx(rdram, ctx, runtime); break;
    case 0x1435ef0u: ref::gamecore_sub_01435EF0(rdram, ctx, runtime); break;
    case 0x1437a60u: ref::_setExecBaseMoveWork(rdram, ctx, runtime); break;
    case 0x1437a68u: ref::_setStopCtrlModeBaseMoveWork(rdram, ctx, runtime); break;
    case 0x1437a70u: ref::_getStopCtrlModeBaseMoveWork(rdram, ctx, runtime); break;
    case 0x1437a78u: ref::_setBackVolumeBaseMoveWork(rdram, ctx, runtime); break;
    case 0x1437a80u: ref::_setSlideVolumeBaseMoveWork(rdram, ctx, runtime); break;
    case 0x1437b88u: ref::_setAttainFlagBaseMoveWork(rdram, ctx, runtime); break;
    case 0x1438448u: ref::gamecore_sub_01438448(rdram, ctx, runtime); break;
    case 0x143d5c8u: ref::resetRouteAttackBirdCtrlWork(rdram, ctx, runtime); break;
    case 0x143d5d8u: ref::setFlapWingBirdCtrlWork(rdram, ctx, runtime); break;
    case 0x143d5e0u: ref::setHoveringBirdCtrlWork(rdram, ctx, runtime); break;
    case 0x143d5e8u: ref::setHoverAttackBirdCtrlWork(rdram, ctx, runtime); break;
    case 0x143d660u: ref::gamecore_sub_0143D660(rdram, ctx, runtime); break;
    case 0x1440ef0u: ref::setDashFastLegCtrlWork(rdram, ctx, runtime); break;
    case 0x1440ef8u: ref::setFinalAttainWalkFastLegCtrlWork(rdram, ctx, runtime); break;
    case 0x1440f00u: ref::setAllowMaxSpeedFastLegCtrlWork(rdram, ctx, runtime); break;
    case 0x1440f18u: ref::setBackFastLegCtrlWork(rdram, ctx, runtime); break;
    case 0x1440f20u: ref::setNotFastTurnFastLegCtrlWork(rdram, ctx, runtime); break;
    case 0x1440f28u: ref::setRunStopTurnFastLegCtrlWork(rdram, ctx, runtime); break;
    case 0x1440f30u: ref::setWalkStopTurnFastLegCtrlWork(rdram, ctx, runtime); break;
    case 0x1440f38u: ref::setCurveCollisionFastLegCtrlWork(rdram, ctx, runtime); break;
    case 0x14418d0u: ref::setExecGazeCtrlWork(rdram, ctx, runtime); break;
    case 0x14418d8u: ref::setViewJudgeGazeCtrlWork(rdram, ctx, runtime); break;
    case 0x14418e0u: ref::setNextDistGazeCtrlWork(rdram, ctx, runtime); break;
    case 0x14418e8u: ref::setValidPosGazeCtrlWork(rdram, ctx, runtime); break;
    case 0x1441968u: ref::setBirdModeGazeCtrlWork(rdram, ctx, runtime); break;
    case 0x1441970u: ref::setGazeCoffGazeCtrlWork(rdram, ctx, runtime); break;
    case 0x14444d0u: ref::setDashHorseCtrlWork(rdram, ctx, runtime); break;
    case 0x14444d8u: ref::setWallStopHorseCtrlWork(rdram, ctx, runtime); break;
    case 0x14444e0u: ref::setLimitModeHorseCtrlWork(rdram, ctx, runtime); break;
    case 0x14444e8u: ref::setFinalAttainWalkHorseCtrlWork(rdram, ctx, runtime); break;
    case 0x14444f0u: ref::setAllowMaxSpeedHorseCtrlWork(rdram, ctx, runtime); break;
    case 0x1444510u: ref::setFreezeHorseCtrlWork(rdram, ctx, runtime); break;
    case 0x14460a8u: ref::chkMoveAngle(rdram, ctx, runtime); break;
    case 0x1446868u: ref::adjustAngleForLimit(rdram, ctx, runtime); break;
    case 0x1449810u: ref::setDashSlowLegCtrlWork(rdram, ctx, runtime); break;
    case 0x1449818u: ref::setStabilizeYSlowLegCtrlWork(rdram, ctx, runtime); break;
    case 0x1449820u: ref::setMoveModeSlowLegCtrlWork(rdram, ctx, runtime); break;
    case 0x1449828u: ref::setMotScaleMaxSlowLegCtrlWork(rdram, ctx, runtime); break;
    case 0x1449838u: ref::setGuardModeSlowLegCtrlWork(rdram, ctx, runtime); break;
    case 0x14498b8u: ref::gamecore_sub_014498B8(rdram, ctx, runtime); break;
    case 0x144cc60u: ref::setAttainSpeedWormCtrlWork(rdram, ctx, runtime); break;
    case 0x144cc98u: ref::setAttackParamWormCtrlWork(rdram, ctx, runtime); break;
    case 0x144cd18u: ref::setRotParamWormCtrlWork(rdram, ctx, runtime); break;
    case 0x144cd38u: ref::setChkKeepHitWormCtrlWork(rdram, ctx, runtime); break;
    case 0x144cd40u: ref::setRotColCheckWormCtrlWork(rdram, ctx, runtime); break;
    case 0x144cd50u: ref::setObstacleWormCtrlWork(rdram, ctx, runtime); break;
    case 0x144cd58u: ref::setSelfEvasionUpWormCtrlWork(rdram, ctx, runtime); break;
    case 0x144cd60u: ref::setTypeWormCtrlWork(rdram, ctx, runtime); break;
    case 0x144cd68u: ref::setFallGravCoffWormCtrlWork(rdram, ctx, runtime); break;
    case 0x144cd70u: ref::setMaxFallSpeedWormCtrlWork(rdram, ctx, runtime); break;
    case 0x144cd78u: ref::setGroundFrictionCoffWormCtrlWork(rdram, ctx, runtime); break;
    case 0x144cd80u: ref::setUnderFrictionCoffWormCtrlWork(rdram, ctx, runtime); break;
    case 0x144cd88u: ref::setWorldPitchLimitWormCtrlWork(rdram, ctx, runtime); break;
    case 0x144cde8u: ref::setColOffsetJudgeFunc(rdram, ctx, runtime); break;
    case 0x144cea8u: ref::gamecore_sub_0144CEA8(rdram, ctx, runtime); break;
    case 0x144d3a0u: ref::createArrowThrustWork(rdram, ctx, runtime); break;
    case 0x1459a50u: ref::SCRConditionChkDisplayOmakeCompleteAlert(rdram, ctx, runtime); break;
    case 0x145ae68u: ref::setScrDemoMangerFinishCallback(rdram, ctx, runtime); break;
    case 0x145c100u: ref::initInterestWork(rdram, ctx, runtime); break;
    case 0x145c118u: ref::getInterestExec(rdram, ctx, runtime); break;
    case 0x145c990u: ref::setPerceptionParam(rdram, ctx, runtime); break;
    case 0x14617b8u: ref::setGoingAngleGazeIKWork(rdram, ctx, runtime); break;
    case 0x14617f0u: ref::setSubParamGazeIKWork(rdram, ctx, runtime); break;
    case 0x1461808u: ref::setGazeModeGazeIKWork(rdram, ctx, runtime); break;
    case 0x1461820u: ref::setExecRootQuatGazeIKWork(rdram, ctx, runtime); break;
    case 0x1461838u: ref::setExecGazeIKWork(rdram, ctx, runtime); break;
    case 0x14621b8u: ref::initTailIKWork(rdram, ctx, runtime); break;
    case 0x14647e0u: ref::setWormIKBoundDynamics(rdram, ctx, runtime); break;
    case 0x1464820u: ref::setWormIKBankInterpSec(rdram, ctx, runtime); break;
    case 0x1465308u: ref::getSScriptWork(rdram, ctx, runtime); break;
    case 0x103ff8u: ref::memprintf(rdram, ctx, runtime); break;
    case 0x104040u: ref::imemprintf(rdram, ctx, runtime); break;
    case 0x105948u: ref::PutString(rdram, ctx, runtime); break;
    case 0x105990u: ref::PutStringS(rdram, ctx, runtime); break;
    case 0x108d10u: ref::sub_00108D10(rdram, ctx, runtime); break;
    case 0x116c58u: ref::sub_00116C58(rdram, ctx, runtime); break;
    case 0x127270u: ref::ps2___fpcmp_parts_d(rdram, ctx, runtime); break;
    case 0x1acf60u: ref::iosCdStSectorAlignSkip(rdram, ctx, runtime); break;
    case 0x1ad510u: ref::iosGetTCount(rdram, ctx, runtime); break;
    case 0x1ad658u: ref::iosGetCpuRapCountPar1Int(rdram, ctx, runtime); break;
    case 0x1b2a80u: ref::iosSprToMemDmaAdrCopy(rdram, ctx, runtime); break;
    case 0x1b4120u: ref::kernel_sub_001B4120(rdram, ctx, runtime); break;
    case 0x1bbc08u: ref::PopQuaternion(rdram, ctx, runtime); break;
    case 0x1be260u: ref::iosGetACosf(rdram, ctx, runtime); break;
    case 0x1c0458u: ref::iosDlGetCreateAdrs(rdram, ctx, runtime); break;
    case 0x1c7d98u: ref::getVIntrCnt(rdram, ctx, runtime); break;
    case 0x1c7db0u: ref::setVIntrBaseTime(rdram, ctx, runtime); break;
    case 0x1c91f8u: ref::iosGetFontDataWithSJIS(rdram, ctx, runtime); break;
    case 0x1c9498u: ref::iosGetFontIDWithSJIS(rdram, ctx, runtime); break;
    case 0x1d1420u: ref::kernel_sub_001D1420(rdram, ctx, runtime); break;
    case 0x1d15b8u: ref::kernel_sub_001D15B8(rdram, ctx, runtime); break;
    case 0x1da330u: ref::vab_get_splitchunk(rdram, ctx, runtime); break;
    case 0x1db418u: ref::kernel_sub_001DB418(rdram, ctx, runtime); break;
    case 0x10817c8u: ref::GAMEUTIL_is_yaw_pitch_get_direction(rdram, ctx, runtime); break;
    case 0x1082ed0u: ref::entrymanager(rdram, ctx, runtime); break;
    case 0x1084590u: ref::reset_double_spring_interp_cntrl(rdram, ctx, runtime); break;
    case 0x10871c8u: ref::setShadowEffect(rdram, ctx, runtime); break;
    case 0x1088e58u: ref::fsEffectDefault(rdram, ctx, runtime); break;
    case 0x1089118u: ref::deleteFsEffect(rdram, ctx, runtime); break;
    case 0x1089270u: ref::deleteFogEffect(rdram, ctx, runtime); break;
    case 0x1089288u: ref::manager_sub_01089288(rdram, ctx, runtime); break;
    case 0x1089b70u: ref::getFogFactor0(rdram, ctx, runtime); break;
    case 0x1099a00u: ref::ModelDispGetWorkSize(rdram, ctx, runtime); break;
    case 0x109a4c8u: ref::ptclCounterInit(rdram, ctx, runtime); break;
    case 0x109acf8u: ref::manager_sub_0109ACF8(rdram, ctx, runtime); break;
    case 0x109ad00u: ref::manager_sub_0109AD00(rdram, ctx, runtime); break;
    case 0x109deb0u: ref::shadowGetWorkSize(rdram, ctx, runtime); break;
    case 0x10ac470u: ref::zFogInit(rdram, ctx, runtime); break;
    case 0x10b92c0u: ref::get_type_parameter_segment_motion_ik_base(rdram, ctx, runtime); break;
    case 0x10ba120u: ref::request_end_motion_ik_interest(rdram, ctx, runtime); break;
    case 0x10ba7a0u: ref::getSizeGazeIKWork(rdram, ctx, runtime); break;
    case 0x10bb060u: ref::GetAnimationDefClass(rdram, ctx, runtime); break;
    case 0x10bb208u: ref::manager_sub_010BB208(rdram, ctx, runtime); break;
    case 0x10c1530u: ref::RequestCharacterMotionShiftFunc(rdram, ctx, runtime); break;
    case 0x10c1d08u: ref::SetCharacterObjControlCallBackFunc(rdram, ctx, runtime); break;
    case 0x10c29a0u: ref::SetInertiaRotInterpMode(rdram, ctx, runtime); break;
    case 0x10dc898u: ref::charrootdyn_getSlideCollisionJudgeCBArg(rdram, ctx, runtime); break;
    case 0x10ed778u: ref::manager_sub_010ED778(rdram, ctx, runtime); break;
    case 0x10ee6f8u: ref::FlushJudgeVolumeClass(rdram, ctx, runtime); break;
    case 0x10f74e8u: ref::GetGameLightClass(rdram, ctx, runtime); break;
    case 0x10f8b20u: ref::GetGlobalLightClass(rdram, ctx, runtime); break;
    case 0x10f9508u: ref::GetAmbientLightClass(rdram, ctx, runtime); break;
    case 0x10f9528u: ref::manager_sub_010F9528(rdram, ctx, runtime); break;
    case 0x1101688u: ref::createBinaryMemoryInfoWindow(rdram, ctx, runtime); break;
    case 0x11017e0u: ref::skelSegDel(rdram, ctx, runtime); break;
    case 0x11017e8u: ref::debugCreateAnimObjSegmentDisp(rdram, ctx, runtime); break;
    case 0x11017f0u: ref::debugDeleteAnimObjSegmentDisp(rdram, ctx, runtime); break;
    case 0x11017f8u: ref::debugCreateAnimObjSegmentDefDisp(rdram, ctx, runtime); break;
    case 0x1101800u: ref::debugDeleteAnimObjSegmentDefDisp(rdram, ctx, runtime); break;
    case 0x1102fd8u: ref::scriptIntrSegInit(rdram, ctx, runtime); break;
    case 0x110a838u: ref::GetScriptAnimationClass(rdram, ctx, runtime); break;
    case 0x110a8e0u: ref::SetScriptAnimationObjChildEntryGroup(rdram, ctx, runtime); break;
    case 0x110bb30u: ref::SetScariptAnimationObjMotSyncSlotId(rdram, ctx, runtime); break;
    case 0x110cc78u: ref::SetScriptCharacterObjReleaseCallBackFunc(rdram, ctx, runtime); break;
    case 0x110d868u: ref::GetScrLwsOriClass(rdram, ctx, runtime); break;
    case 0x110da90u: ref::GetScriptLwsOrientDefinfo(rdram, ctx, runtime); break;
    case 0x110dc18u: ref::manager_sub_0110DC18(rdram, ctx, runtime); break;
    case 0x110ecd0u: ref::SpriteMenuIsFadeCheckSelectLayer(rdram, ctx, runtime); break;
    case 0x11119b8u: ref::GetScriptSpriteMenuClass(rdram, ctx, runtime); break;
    case 0x1111e98u: ref::SpriteMenuSetLayerPriority(rdram, ctx, runtime); break;
    case 0x1112ba0u: ref::SpriteMenuGetSelectLayer(rdram, ctx, runtime); break;
    case 0x1112bb8u: ref::SpriteMenuGetPadEvent(rdram, ctx, runtime); break;
    case 0x1112f50u: ref::SpriteMenuGetSelectSubMenuId(rdram, ctx, runtime); break;
    case 0x11138c8u: ref::SpriteMenuIsFadeCheckLayer(rdram, ctx, runtime); break;
    case 0x1113950u: ref::SpriteMenuGetFadeByLayer(rdram, ctx, runtime); break;
    case 0x1114238u: ref::GetScriptSpriteClass(rdram, ctx, runtime); break;
    case 0x1114490u: ref::GetSubLinkClass(rdram, ctx, runtime); break;
    case 0x112a450u: ref::CheckCollisionFinderAlive(rdram, ctx, runtime); break;
    case 0x11327b0u: ref::CopyGeometryPointer(rdram, ctx, runtime); break;
    case 0x113f370u: ref::free_all_emitter_and_particle_particle_manager(rdram, ctx, runtime); break;
    case 0x113fa20u: ref::get_particle_effect_id_from_sheet(rdram, ctx, runtime); break;
    case 0x113fa70u: ref::get_particle_emitter_from_emitter_id(rdram, ctx, runtime); break;
    case 0x113fa88u: ref::get_main_particle_effect_lod_from_sheet(rdram, ctx, runtime); break;
    case 0x113fa90u: ref::get_sub_particle_effect_lod_from_sheet(rdram, ctx, runtime); break;
    case 0x113fa98u: ref::get_top_particle_effect_from_lod(rdram, ctx, runtime); break;
    case 0x11401d8u: ref::get_radiate_range_coefficient(rdram, ctx, runtime); break;
    case 0x1140730u: ref::unload_npe_file_main(rdram, ctx, runtime); break;
    case 0x1140738u: ref::unload_npe_file_sub(rdram, ctx, runtime); break;
    case 0x1140740u: ref::manager_sub_01140740(rdram, ctx, runtime); break;
    case 0x1141080u: ref::manager_sub_01141080(rdram, ctx, runtime); break;
    case 0x1142030u: ref::manager_sub_01142030(rdram, ctx, runtime); break;
    case 0x1144e00u: ref::manager_sub_01144E00(rdram, ctx, runtime); break;
    case 0x1144e38u: ref::manager_sub_01144E38(rdram, ctx, runtime); break;
    case 0x1144e50u: ref::manager_sub_01144E50(rdram, ctx, runtime); break;
    case 0x1144e58u: ref::manager_sub_01144E58(rdram, ctx, runtime); break;
    case 0x1144e70u: ref::manager_sub_01144E70(rdram, ctx, runtime); break;
    case 0x1144e88u: ref::manager_sub_01144E88(rdram, ctx, runtime); break;
    case 0x11464c0u: ref::is_eternal_wind(rdram, ctx, runtime); break;
    case 0x1146980u: ref::stop_wind_emitter_geometry_pointer_reference(rdram, ctx, runtime); break;
    case 0x11469d0u: ref::stop_wind_emitter_callback(rdram, ctx, runtime); break;
    case 0x1146a10u: ref::end_wind_emitter(rdram, ctx, runtime); break;
    case 0x1147430u: ref::add_wind_segment(rdram, ctx, runtime); break;
    case 0x1147950u: ref::RelocateModifiedManager(rdram, ctx, runtime); break;
    case 0x1147a28u: ref::GetSizeOfModifiedManagerWork(rdram, ctx, runtime); break;
    case 0x115ee68u: ref::sheetMgrChkStageIndexResolve(rdram, ctx, runtime); break;
    case 0x1162510u: ref::add_camera_lws_animation_segment(rdram, ctx, runtime); break;
    case 0x1162ef0u: ref::get_camera_position(rdram, ctx, runtime); break;
    case 0x1162ef8u: ref::get_camera_direction(rdram, ctx, runtime); break;
    case 0x1162f00u: ref::get_camera_up_direction(rdram, ctx, runtime); break;
    case 0x1162f08u: ref::get_camera_obj_layout_class(rdram, ctx, runtime); break;
    case 0x1162fd0u: ref::get_view_matrix(rdram, ctx, runtime); break;
    case 0x1163750u: ref::get_manage_object_id_camera_region(rdram, ctx, runtime); break;
    case 0x1163788u: ref::is_parent_obj_boss_camera_region(rdram, ctx, runtime); break;
    case 0x11637d0u: ref::get_priority_camera_region(rdram, ctx, runtime); break;
    case 0x1163888u: ref::get_region_type_camera_region(rdram, ctx, runtime); break;
    case 0x1164c70u: ref::get_clip_matrix(rdram, ctx, runtime); break;
    case 0x1164c78u: ref::get_clip_matrix128(rdram, ctx, runtime); break;
    case 0x1164c80u: ref::get_clip_matrix256(rdram, ctx, runtime); break;
    case 0x1164c88u: ref::get_clip_matrix_fovignore(rdram, ctx, runtime); break;
    case 0x1164c90u: ref::get_screen_matrix(rdram, ctx, runtime); break;
    case 0x1164c98u: ref::get_screen_matrix128(rdram, ctx, runtime); break;
    case 0x1164ca0u: ref::get_screen_matrix256(rdram, ctx, runtime); break;
    case 0x1164cc0u: ref::get_VC_matrix(rdram, ctx, runtime); break;
    case 0x1164cc8u: ref::get_CS_matrix(rdram, ctx, runtime); break;
    case 0x1164cd0u: ref::get_WVC_matrix(rdram, ctx, runtime); break;
    case 0x1164cd8u: ref::get_VCS_matrix(rdram, ctx, runtime); break;
    case 0x1164ce0u: ref::get_WVCS_matrix(rdram, ctx, runtime); break;
    case 0x1164ce8u: ref::get_scissor_VC_matrix(rdram, ctx, runtime); break;
    case 0x1164cf0u: ref::get_scissor_WVC_matrix(rdram, ctx, runtime); break;
    case 0x1169d18u: ref::getPointDataHeader(rdram, ctx, runtime); break;
    case 0x1169d30u: ref::getLineDataHeader(rdram, ctx, runtime); break;
    case 0x1169d88u: ref::getConnectLineArray(rdram, ctx, runtime); break;
    case 0x1169e20u: ref::getRootDataHeader(rdram, ctx, runtime); break;
    case 0x1169e38u: ref::getGoalDataHeader(rdram, ctx, runtime); break;
    case 0x1169e50u: ref::getSubDataHeader(rdram, ctx, runtime); break;
    case 0x1169f28u: ref::getChildGoalPointIdxArray(rdram, ctx, runtime); break;
    case 0x1169f80u: ref::getChildSubPointIdxArray(rdram, ctx, runtime); break;
    case 0x1169fd8u: ref::getInterestDataHeader(rdram, ctx, runtime); break;
    case 0x116a028u: ref::getDeadDataHeader(rdram, ctx, runtime); break;
    case 0x116a080u: ref::getLayoutDataHeader(rdram, ctx, runtime); break;
    case 0x1342ef8u: ref::GAMEUTIL_is_push_or_down_1p_button(rdram, ctx, runtime); break;
    case 0x1344588u: ref::GAMEUTIL_get_data_record_info(rdram, ctx, runtime); break;
    case 0x1353580u: ref::gStatSaveDataGameFlagChk(rdram, ctx, runtime); break;
    case 0x13536d8u: ref::gStatGetGObjAdrs(rdram, ctx, runtime); break;
    case 0x135ff08u: ref::getstagenumber(rdram, ctx, runtime); break;
    case 0x1366cf0u: ref::initStageLayout(rdram, ctx, runtime); break;
    case 0x1366cf8u: ref::deleteStageLayout(rdram, ctx, runtime); break;
    case 0x136a420u: ref::initAdpcmOrient(rdram, ctx, runtime); break;
    case 0x136aae8u: ref::bgmStartGameOver(rdram, ctx, runtime); break;
    case 0x136aaf8u: ref::bgmStageInit(rdram, ctx, runtime); break;
    case 0x13790a0u: ref::SCRExecBossPostProcess(rdram, ctx, runtime); break;
    case 0x13b6198u: ref::SCRPlayerReset(rdram, ctx, runtime); break;
    case 0x13b6530u: ref::gamecore_sub_013B6530(rdram, ctx, runtime); break;
    case 0x13cfb60u: ref::scrInit(rdram, ctx, runtime); break;
    case 0x13d4df8u: ref::AI_CheckNormZero(rdram, ctx, runtime); break;
    case 0x140ae18u: ref::initSavePoint(rdram, ctx, runtime); break;
    case 0x140ae38u: ref::getSprLogWorkSize(rdram, ctx, runtime); break;
    case 0x140ae50u: ref::getSprLogStatusSize(rdram, ctx, runtime); break;
    case 0x141ce38u: ref::get_camera_move_collision_group(rdram, ctx, runtime); break;
    case 0x141ce58u: ref::get_game_camera(rdram, ctx, runtime); break;
    case 0x141ce60u: ref::get_cur_camera_target(rdram, ctx, runtime); break;
    case 0x141d178u: ref::gamecore_sub_0141D178(rdram, ctx, runtime); break;
    case 0x141d180u: ref::gamecore_sub_0141D180(rdram, ctx, runtime); break;
    case 0x141d4f0u: ref::gamecore_sub_0141D4F0(rdram, ctx, runtime); break;
    case 0x1424f38u: ref::get_cur_using_lws_anime_mode_base_class(rdram, ctx, runtime); break;
    case 0x1425050u: ref::end_object_interest_game_camera(rdram, ctx, runtime); break;
    case 0x1425660u: ref::free_camera_mode_manager_segment(rdram, ctx, runtime); break;
    case 0x1425d20u: ref::gamecore_sub_01425D20(rdram, ctx, runtime); break;
    case 0x1426250u: ref::gamecore_sub_01426250(rdram, ctx, runtime); break;
    case 0x14262b0u: ref::gamecore_sub_014262B0(rdram, ctx, runtime); break;
    case 0x14262b8u: ref::gamecore_sub_014262B8(rdram, ctx, runtime); break;
    case 0x1426368u: ref::gamecore_sub_01426368(rdram, ctx, runtime); break;
    case 0x142c120u: ref::get_interp_game_camera_param_game_camera_param_manager(rdram, ctx, runtime); break;
    case 0x142c428u: ref::get_mode_common_data_filter_base_dist_to_look_at(rdram, ctx, runtime); break;
    case 0x1431c30u: ref::delISysPathObj(rdram, ctx, runtime); break;
    case 0x1431da0u: ref::setDataHeaderFromAreaIdx(rdram, ctx, runtime); break;
    case 0x14563b8u: ref::SCRConditionTrue(rdram, ctx, runtime); break;
    case 0x145d688u: ref::getSizeAimIKWork(rdram, ctx, runtime); break;
    case 0x145dfc8u: ref::getSizeAttackIKWork(rdram, ctx, runtime); break;
    case 0x145f0e0u: ref::getSizeBeardIKWork(rdram, ctx, runtime); break;
    case 0x14621b0u: ref::getSizeTailIKWork(rdram, ctx, runtime); break;
    case 0x1464aa0u: ref::getEventScriptClass(rdram, ctx, runtime); break;
    case 0x1bf400u: ref::iosInitDC(rdram, ctx, runtime); break;
    case 0x1c9828u: ref::InitSubThread(rdram, ctx, runtime); break;
    case 0x1cb430u: ref::setSoundMode(rdram, ctx, runtime); break;
    case 0x1cc300u: ref::Sg2DmaTransStatus(rdram, ctx, runtime); break;
    case 0x1da530u: ref::Sg2VabBdOpen(rdram, ctx, runtime); break;
    case 0x1080220u: ref::GAMEUTIL_get_difference_value(rdram, ctx, runtime); break;
    case 0x10c1b90u: ref::GetCalcNumObjWorkSize(rdram, ctx, runtime); break;
    case 0x1343c58u: ref::GAMEUTIL_constructor_move_angle(rdram, ctx, runtime); break;
    case 0x13461d0u: ref::setFade(rdram, ctx, runtime); break;
    case 0x13461f0u: ref::getFade(rdram, ctx, runtime); break;
    case 0x13527d8u: ref::gamecore_sub_013527D8(rdram, ctx, runtime); break;
    case 0x1357758u: ref::gamecore_sub_01357758(rdram, ctx, runtime); break;
    case 0x1384ee0u: ref::gamecore_sub_01384EE0(rdram, ctx, runtime); break;
    case 0x138be98u: ref::get_pre_type_expression(rdram, ctx, runtime); break;
    case 0x139aaf8u: ref::MotSpdCtrl_Init(rdram, ctx, runtime); break;
    case 0x13ae538u: ref::gamecore_sub_013AE538(rdram, ctx, runtime); break;
    case 0x13ae550u: ref::gamecore_sub_013AE550(rdram, ctx, runtime); break;
    case 0x13ae568u: ref::gamecore_sub_013AE568(rdram, ctx, runtime); break;
    case 0x13ae580u: ref::gamecore_sub_013AE580(rdram, ctx, runtime); break;
    case 0x13ae598u: ref::gamecore_sub_013AE598(rdram, ctx, runtime); break;
    case 0x13ae5b0u: ref::gamecore_sub_013AE5B0(rdram, ctx, runtime); break;
    case 0x13ae5c8u: ref::gamecore_sub_013AE5C8(rdram, ctx, runtime); break;
    case 0x13ae5e0u: ref::gamecore_sub_013AE5E0(rdram, ctx, runtime); break;
    case 0x13ae5f8u: ref::gamecore_sub_013AE5F8(rdram, ctx, runtime); break;
    case 0x13ae610u: ref::gamecore_sub_013AE610(rdram, ctx, runtime); break;
    case 0x13ae628u: ref::gamecore_sub_013AE628(rdram, ctx, runtime); break;
    case 0x13ae880u: ref::gamecore_sub_013AE880(rdram, ctx, runtime); break;
    case 0x13b77c8u: ref::gamecore_sub_013B77C8(rdram, ctx, runtime); break;
    case 0x14109f8u: ref::get_cur_attr_camera_default_angle(rdram, ctx, runtime); break;
    case 0x1410a90u: ref::gamecore_sub_01410A90(rdram, ctx, runtime); break;
    case 0x1414a48u: ref::get_status_camera_target_approach(rdram, ctx, runtime); break;
    case 0x141aa98u: ref::CAM_init_camera_warp_cntrl(rdram, ctx, runtime); break;
    case 0x141c9b8u: ref::gamecore_sub_0141C9B8(rdram, ctx, runtime); break;
    case 0x141d390u: ref::get_main_cameraman_camera_obj(rdram, ctx, runtime); break;
    case 0x1424fe8u: ref::request_end_pause_rise_game_camera(rdram, ctx, runtime); break;
    case 0x1425008u: ref::is_end_pause_rise_game_camera(rdram, ctx, runtime); break;
    case 0x1431c58u: ref::getPathColiGroup(rdram, ctx, runtime); break;
    case 0x1437a88u: ref::_resetBaseMoveWork(rdram, ctx, runtime); break;
    case 0x1459360u: ref::SCRConditionSwordFlareHitColAttr(rdram, ctx, runtime); break;
    case 0x1459ae8u: ref::SCRConditionHardMode(rdram, ctx, runtime); break;
    case 0x145b1c0u: ref::gamecore_sub_0145B1C0(rdram, ctx, runtime); break;
    case 0x10845b0u: ref::is_reach_src_value_double_spring_interp_cntrl(rdram, ctx, runtime); break;
    case 0x14269d0u: ref::gamecore_sub_014269D0(rdram, ctx, runtime); break;
    case 0x1004b8u: ref::RelocateElfInfoHeader(rdram, ctx, runtime); break;
    case 0x102730u: ref::LoaderSysEntryExternalIntcHandlerList(rdram, ctx, runtime); break;
    case 0x102790u: ref::LoaderSysEntryExternalThreadList(rdram, ctx, runtime); break;
    case 0x1027e0u: ref::LoaderSysEntryExternalSemaList(rdram, ctx, runtime); break;
    case 0x102830u: ref::LoaderSysEntryExternalIopMemoryList(rdram, ctx, runtime); break;
    case 0x1028a0u: ref::LoaderSysDeleteExternalIntcHandlerList(rdram, ctx, runtime); break;
    case 0x1028f0u: ref::LoaderSysDeleteExternalThreadList(rdram, ctx, runtime); break;
    case 0x102930u: ref::LoaderSysDeleteExternalSemaList(rdram, ctx, runtime); break;
    case 0x102970u: ref::LoaderSysDeleteExternalIopMemoryList(rdram, ctx, runtime); break;
    case 0x1029c8u: ref::LoaderSysInitExternalIntcHandlerList(rdram, ctx, runtime); break;
    case 0x102a00u: ref::LoaderSysInitExternalSemaList(rdram, ctx, runtime); break;
    case 0x102a40u: ref::LoaderSysInitExternalThreadList(rdram, ctx, runtime); break;
    case 0x102c58u: ref::LoaderSysInitExternalIopMemoryList(rdram, ctx, runtime); break;
    case 0x103110u: ref::LoaderSysGetMemoryInfo(rdram, ctx, runtime); break;
    case 0x106390u: ref::ScrollDisplay(rdram, ctx, runtime); break;
    case 0x106430u: ref::ClearDisplay(rdram, ctx, runtime); break;
    case 0x109640u: ref::sub_00109640(rdram, ctx, runtime); break;
    case 0x10f370u: ref::sub_0010F370(rdram, ctx, runtime); break;
    case 0x10fc08u: ref::sub_0010FC08(rdram, ctx, runtime); break;
    case 0x10fc40u: ref::sub_0010FC40(rdram, ctx, runtime); break;
    case 0x10fe50u: ref::sub_0010FE50(rdram, ctx, runtime); break;
    case 0x10ffc0u: ref::sub_0010FFC0(rdram, ctx, runtime); break;
    case 0x110270u: ref::sub_00110270(rdram, ctx, runtime); break;
    case 0x110910u: ref::sub_00110910(rdram, ctx, runtime); break;
    case 0x110c78u: ref::iGetTimerSystemTime(rdram, ctx, runtime); break;
    case 0x110d48u: ref::iAllocTimerCounter(rdram, ctx, runtime); break;
    case 0x111288u: ref::iGetTimerBaseTime(rdram, ctx, runtime); break;
    case 0x111700u: ref::ps2__InitAlarm(rdram, ctx, runtime); break;
    case 0x113368u: ref::sceVif1PkAddGsData(rdram, ctx, runtime); break;
    case 0x114020u: ref::sceGsSetDefAlphaEnv(rdram, ctx, runtime); break;
    case 0x114080u: ref::sceGsSetHalfOffset(rdram, ctx, runtime); break;
    case 0x1157d8u: ref::sub_001157D8(rdram, ctx, runtime); break;
    case 0x11bae8u: ref::sub_0011BAE8(rdram, ctx, runtime); break;
    case 0x11bbb8u: ref::sub_0011BBB8(rdram, ctx, runtime); break;
    case 0x11bf68u: ref::sub_0011BF68(rdram, ctx, runtime); break;
    case 0x11c138u: ref::_sceMcFatInit(rdram, ctx, runtime); break;
    case 0x11c198u: ref::_sceMcFatInit2(rdram, ctx, runtime); break;
    case 0x120438u: ref::_sceMcpInit(rdram, ctx, runtime); break;
    case 0x120d10u: ref::_sceMcpIsSpeckOutBlock(rdram, ctx, runtime); break;
    case 0x121ec0u: ref::sub_00121EC0(rdram, ctx, runtime); break;
    case 0x122378u: ref::sceNetcnfifGetResult(rdram, ctx, runtime); break;
    case 0x125080u: ref::sub_00125080(rdram, ctx, runtime); break;
    case 0x125998u: ref::sub_00125998(rdram, ctx, runtime); break;
    case 0x1259e0u: ref::sub_001259E0(rdram, ctx, runtime); break;
    case 0x125a38u: ref::sub_00125A38(rdram, ctx, runtime); break;
    case 0x1262b8u: ref::sub_001262B8(rdram, ctx, runtime); break;
    case 0x126388u: ref::sub_00126388(rdram, ctx, runtime); break;
    case 0x1271b0u: ref::ps2___unpack_d(rdram, ctx, runtime); break;
    case 0x12e890u: ref::ps2__Bfree(rdram, ctx, runtime); break;
    case 0x12f0d8u: ref::ps2___mcmp(rdram, ctx, runtime); break;
    case 0x12f2d0u: ref::_ulp(rdram, ctx, runtime); break;
    case 0x1acdf8u: ref::iosCdStStart(rdram, ctx, runtime); break;
    case 0x1ad528u: ref::iosGetTCountExtension(rdram, ctx, runtime); break;
    case 0x1ad628u: ref::iosResetCpuRapCounter(rdram, ctx, runtime); break;
    case 0x1b27b0u: ref::iosPadRead(rdram, ctx, runtime); break;
    case 0x1b2a18u: ref::iosMemToSprDmaAdrCopy(rdram, ctx, runtime); break;
    case 0x1b3c50u: ref::Init_Shock(rdram, ctx, runtime); break;
    case 0x1b40d0u: ref::kernel_sub_001B40D0(rdram, ctx, runtime); break;
    case 0x1b7500u: ref::iosLoaderGetFileSize(rdram, ctx, runtime); break;
    case 0x1b7550u: ref::initDldSys(rdram, ctx, runtime); break;
    case 0x1ba690u: ref::kernel_sub_001BA690(rdram, ctx, runtime); break;
    case 0x1be570u: ref::iosGetTableCos(rdram, ctx, runtime); break;
    case 0x1bf160u: ref::iosSetDCViewPortCenter(rdram, ctx, runtime); break;
    case 0x1bfad8u: ref::iosUsedPacketBufferSize(rdram, ctx, runtime); break;
    case 0x1bfc08u: ref::iosAllocPacketPtrPath1(rdram, ctx, runtime); break;
    case 0x1bfcf8u: ref::iosDlChainTail(rdram, ctx, runtime); break;
    case 0x1c1538u: ref::kernel_sub_001C1538(rdram, ctx, runtime); break;
    case 0x1c15f8u: ref::iosGsSetMode(rdram, ctx, runtime); break;
    case 0x1c35e8u: ref::iosChkGpDlTransExec(rdram, ctx, runtime); break;
    case 0x1c5e00u: ref::getTWH(rdram, ctx, runtime); break;
    case 0x1c7948u: ref::isysGetLinkNum(rdram, ctx, runtime); break;
    case 0x1c7dc8u: ref::getVIntrDiffTime(rdram, ctx, runtime); break;
    case 0x1c7e30u: ref::addVIntrTime(rdram, ctx, runtime); break;
    case 0x1c7fb0u: ref::ischedStop(rdram, ctx, runtime); break;
    case 0x1c8910u: ref::get_next_letter_in_post(rdram, ctx, runtime); break;
    case 0x1c8af0u: ref::entry_new_letter_type(rdram, ctx, runtime); break;
    case 0x1c8b30u: ref::init_letter_type_store(rdram, ctx, runtime); break;
    case 0x1c8b98u: ref::kernel_sub_001C8B98(rdram, ctx, runtime); break;
    case 0x1c9678u: ref::iosGetFontInfo(rdram, ctx, runtime); break;
    case 0x1cab80u: ref::sndSepPlayerSrh(rdram, ctx, runtime); break;
    case 0x1cac08u: ref::sndSepPlayerSrh2(rdram, ctx, runtime); break;
    case 0x1cac98u: ref::sndSepSetVol(rdram, ctx, runtime); break;
    case 0x1cacd8u: ref::sndSepSetPan(rdram, ctx, runtime); break;
    case 0x1cadf8u: ref::sndSepStopTypeAll(rdram, ctx, runtime); break;
    case 0x1caf10u: ref::sndVabCreate(rdram, ctx, runtime); break;
    case 0x1cb170u: ref::sndVabInit(rdram, ctx, runtime); break;
    case 0x1cbd70u: ref::AddStrHashKey(rdram, ctx, runtime); break;
    case 0x1cc0c8u: ref::Sg2CtrlEntryTickProcById(rdram, ctx, runtime); break;
    case 0x1cc7f8u: ref::Sg2DspInit(rdram, ctx, runtime); break;
    case 0x1cd448u: ref::ps2_____VolPerBackup(rdram, ctx, runtime); break;
    case 0x1cdde8u: ref::kernel_sub_001CDDE8(rdram, ctx, runtime); break;
    case 0x1cdf38u: ref::kernel_sub_001CDF38(rdram, ctx, runtime); break;
    case 0x1d2da0u: ref::Sg2SepMidiOut(rdram, ctx, runtime); break;
    case 0x1d3b08u: ref::Sg2SlotGetSlotClassIdx(rdram, ctx, runtime); break;
    case 0x1d5cb8u: ref::Sg2SlotBlock(rdram, ctx, runtime); break;
    case 0x1d5d58u: ref::Sg2SlotCheckSlotClass(rdram, ctx, runtime); break;
    case 0x1d5db0u: ref::Sg2SlotFindLoop(rdram, ctx, runtime); break;
    case 0x1d5df8u: ref::Sg2SlotFindVab(rdram, ctx, runtime); break;
    case 0x1d5e48u: ref::Sg2SlotFindVabCh(rdram, ctx, runtime); break;
    case 0x1d5ea8u: ref::Sg2SlotFindVabChNote(rdram, ctx, runtime); break;
    case 0x1d5f10u: ref::Sg2SlotFindVabChSsaBaseNote(rdram, ctx, runtime); break;
    case 0x1d5f88u: ref::Sg2SlotFindSlotClass(rdram, ctx, runtime); break;
    case 0x1d5fd8u: ref::Sg2SlotFindSlotClassCh(rdram, ctx, runtime); break;
    case 0x1d6038u: ref::Sg2SlotFindSlotClassChNote(rdram, ctx, runtime); break;
    case 0x1d60a0u: ref::Sg2SlotFindSlotClassChSsaBaseNote(rdram, ctx, runtime); break;
    case 0x1d62f0u: ref::Sg2SlotSetMasVol(rdram, ctx, runtime); break;
    case 0x1d6360u: ref::Sg2SlotSetSeMasVol(rdram, ctx, runtime); break;
    case 0x1d63b8u: ref::Sg2SlotSetBaseVol(rdram, ctx, runtime); break;
    case 0x1d6410u: ref::Sg2SlotSetVol(rdram, ctx, runtime); break;
    case 0x1d6440u: ref::Sg2SlotSetMasPan(rdram, ctx, runtime); break;
    case 0x1d64a0u: ref::Sg2SlotSetBasePan(rdram, ctx, runtime); break;
    case 0x1d64f8u: ref::Sg2SlotSetPan(rdram, ctx, runtime); break;
    case 0x1d66a0u: ref::Sg2SlotSetMasTune(rdram, ctx, runtime); break;
    case 0x1d6720u: ref::Sg2SlotSetVolPhase(rdram, ctx, runtime); break;
    case 0x1d67a8u: ref::Sg2SlotSetLfoPitchTbl(rdram, ctx, runtime); break;
    case 0x1d68a8u: ref::Sg2SlotSetLfoPitchDepth(rdram, ctx, runtime); break;
    case 0x1d6cf0u: ref::Sg2SlotSetPoltamentParam(rdram, ctx, runtime); break;
    case 0x1d8000u: ref::Sg2SlotTbl(rdram, ctx, runtime); break;
    case 0x1d8de0u: ref::Sg2SmfpMidiOut(rdram, ctx, runtime); break;
    case 0x1d9ec8u: ref::get_vabid(rdram, ctx, runtime); break;
    case 0x1d9f08u: ref::kernel_sub_001D9F08(rdram, ctx, runtime); break;
    case 0x1daab0u: ref::kernel_sub_001DAAB0(rdram, ctx, runtime); break;
    case 0x1dac18u: ref::kernel_sub_001DAC18(rdram, ctx, runtime); break;
    case 0x1db678u: ref::kernel_sub_001DB678(rdram, ctx, runtime); break;
    case 0x1dcb48u: ref::IirFilter(rdram, ctx, runtime); break;
    case 0x1de720u: ref::ps2___ieee754_fmodf(rdram, ctx, runtime); break;
    case 0x107da28u: ref::GAMEUTIL_inc_or_dec_for_near_to_aim_pitch(rdram, ctx, runtime); break;
    case 0x107dd80u: ref::GAMEUTIL_get_interp_pitch(rdram, ctx, runtime); break;
    case 0x107ed30u: ref::GAMEUTIL_is_diff_nearly_equal_zero(rdram, ctx, runtime); break;
    case 0x107eec8u: ref::GAMEUTIL_get_cross_position_coefficient_t_by_dist_factor(rdram, ctx, runtime); break;
    case 0x107f078u: ref::GAMEUTIL_get_segment_num_and_data(rdram, ctx, runtime); break;
    case 0x107fa60u: ref::GAMEUTIL_get_new_velocity(rdram, ctx, runtime); break;
    case 0x10806a8u: ref::GAMEUTIL_increase_pitch(rdram, ctx, runtime); break;
    case 0x1080968u: ref::GAMEUTIL_get_difference_yaw_and_yaw(rdram, ctx, runtime); break;
    case 0x1081090u: ref::GAMEUTIL_is_reach_aim_pitch_angle(rdram, ctx, runtime); break;
    case 0x10842a0u: ref::GAMEUTIL_reset_interp_ratio_cntrl(rdram, ctx, runtime); break;
    case 0x10846a8u: ref::reset_double_spring_interp_cntrl3D(rdram, ctx, runtime); break;
    case 0x10871d8u: ref::endBossDeathEffect(rdram, ctx, runtime); break;
    case 0x1088a38u: ref::fsEffectSetParam(rdram, ctx, runtime); break;
    case 0x1088d60u: ref::fsEffectManagerSetDemoParam(rdram, ctx, runtime); break;
    case 0x1088eb8u: ref::fsEffectManagerInit(rdram, ctx, runtime); break;
    case 0x1088f80u: ref::initFsEffect(rdram, ctx, runtime); break;
    case 0x1089aa0u: ref::setFogParamDefault(rdram, ctx, runtime); break;
    case 0x108d150u: ref::glBlendFunc(rdram, ctx, runtime); break;
    case 0x108d290u: ref::glBlendFuncParticle(rdram, ctx, runtime); break;
    case 0x108e8b8u: ref::glColor4f(rdram, ctx, runtime); break;
    case 0x108e938u: ref::glColor4fv(rdram, ctx, runtime); break;
    case 0x1096a80u: ref::modelGetDlLayer(rdram, ctx, runtime); break;
    case 0x10993c8u: ref::modelResetSurface(rdram, ctx, runtime); break;
    case 0x1099438u: ref::modelSetMatrixTransTag(rdram, ctx, runtime); break;
    case 0x1099e00u: ref::ptclmanBuildMatPacket(rdram, ctx, runtime); break;
    case 0x109aa48u: ref::GetMeshClass(rdram, ctx, runtime); break;
    case 0x109abc8u: ref::meshFlip(rdram, ctx, runtime); break;
    case 0x10a52e0u: ref::spriteSetTextureCoordinateST(rdram, ctx, runtime); break;
    case 0x10a5440u: ref::spriteSetBlendMode(rdram, ctx, runtime); break;
    case 0x10aacb8u: ref::texGetTexTBPSize(rdram, ctx, runtime); break;
    case 0x10ae658u: ref::manager_sub_010AE658(rdram, ctx, runtime); break;
    case 0x10b2790u: ref::getBlendBaseMotion(rdram, ctx, runtime); break;
    case 0x10b62f0u: ref::GetMotionFrameDirectResource(rdram, ctx, runtime); break;
    case 0x10b63c0u: ref::GetMotionOptionNodeParentId(rdram, ctx, runtime); break;
    case 0x10b7618u: ref::InitMotionBoolInfo(rdram, ctx, runtime); break;
    case 0x10c1158u: ref::ResetCharacterClassIKInfo(rdram, ctx, runtime); break;
    case 0x10c1a80u: ref::GetCharacterClassCurAnimationCurFrame(rdram, ctx, runtime); break;
    case 0x10c1d38u: ref::SetCharacterObjControlBypassFunc(rdram, ctx, runtime); break;
    case 0x10c2910u: ref::SetCharacterObjParentPlaneDiscontinuousJudgeFunc(rdram, ctx, runtime); break;
    case 0x10c2940u: ref::SetCharacterObjParentPointXZFitGpChangeJudgeFunc(rdram, ctx, runtime); break;
    case 0x10c9888u: ref::manager_sub_010C9888(rdram, ctx, runtime); break;
    case 0x10cc4f0u: ref::InitScriptControlButtonCallBack(rdram, ctx, runtime); break;
    case 0x10cd400u: ref::manager_sub_010CD400(rdram, ctx, runtime); break;
    case 0x10d2b18u: ref::RequestCharacterClassMotionChangeWithParam(rdram, ctx, runtime); break;
    case 0x10de398u: ref::charrootdyn_getLimitMode(rdram, ctx, runtime); break;
    case 0x10e2170u: ref::resetExpressionLocalHeightDiff(rdram, ctx, runtime); break;
    case 0x10ebb90u: ref::GetDemoOrientNext(rdram, ctx, runtime); break;
    case 0x10ed440u: ref::UpdateFogCol(rdram, ctx, runtime); break;
    case 0x10f8f90u: ref::IsEnvelopeDistantLightType(rdram, ctx, runtime); break;
    case 0x10f8ff0u: ref::globalLightCalcLightNum(rdram, ctx, runtime); break;
    case 0x10f92b0u: ref::GetImpactWorstWorkLightClass(rdram, ctx, runtime); break;
    case 0x10fc1f0u: ref::GetLwsOrientNodeNum(rdram, ctx, runtime); break;
    case 0x10fc618u: ref::manager_sub_010FC618(rdram, ctx, runtime); break;
    case 0x1100af8u: ref::initAnimationObjResource(rdram, ctx, runtime); break;
    case 0x1101650u: ref::InitBinarySize(rdram, ctx, runtime); break;
    case 0x110a670u: ref::SetScriptAnimationGlobalBaseInfo(rdram, ctx, runtime); break;
    case 0x110a7d8u: ref::CopyScriptAnimationWindGeometryPointer(rdram, ctx, runtime); break;
    case 0x1111998u: ref::GetSpriteMenuClass(rdram, ctx, runtime); break;
    case 0x11119d8u: ref::SpriteMenuSetPadEventCallback(rdram, ctx, runtime); break;
    case 0x1111a08u: ref::SpriteSubMenuSetPadEventCallback(rdram, ctx, runtime); break;
    case 0x1111a38u: ref::SpriteMenuSetDispFlag(rdram, ctx, runtime); break;
    case 0x1111b78u: ref::SpriteMenuSetDispFlagByHash(rdram, ctx, runtime); break;
    case 0x1111e20u: ref::SpriteMenuSetLayer(rdram, ctx, runtime); break;
    case 0x1111f60u: ref::SpriteMenuGetLayerStatus(rdram, ctx, runtime); break;
    case 0x1111f90u: ref::SpriteMenuGetLayerByHash(rdram, ctx, runtime); break;
    case 0x1112008u: ref::SpriteMenuGetLayerNoByHash(rdram, ctx, runtime); break;
    case 0x1112c88u: ref::SpriteMenuGetMenuIdByHash(rdram, ctx, runtime); break;
    case 0x1112e68u: ref::SpriteMenuGetSelectMenuIdBySpriteMenuClass(rdram, ctx, runtime); break;
    case 0x1112ef0u: ref::SpriteMenuSetSelectMenuId(rdram, ctx, runtime); break;
    case 0x1113088u: ref::SpriteMenuGetSelectMenuMode(rdram, ctx, runtime); break;
    case 0x1113130u: ref::SpriteMenuSetDefaultSelectMenu(rdram, ctx, runtime); break;
    case 0x11137b8u: ref::SpriteMenuSetNumPartsFormatByPartsParam(rdram, ctx, runtime); break;
    case 0x11137e0u: ref::SpriteMenuSetAnimParam(rdram, ctx, runtime); break;
    case 0x1114170u: ref::GetSpriteClass(rdram, ctx, runtime); break;
    case 0x1128778u: ref::ChangeCollisionResourceAttribute(rdram, ctx, runtime); break;
    case 0x1128898u: ref::ConstructorCollisionObjectJudgeCallbackData(rdram, ctx, runtime); break;
    case 0x112a350u: ref::CopyCollisionFinder(rdram, ctx, runtime); break;
    case 0x1132a00u: ref::GetGeometryPointerObjectParentNodeId(rdram, ctx, runtime); break;
    case 0x1139128u: ref::GetClothWorkSize(rdram, ctx, runtime); break;
    case 0x1140630u: ref::manager_sub_01140630(rdram, ctx, runtime); break;
    case 0x11408e0u: ref::manager_sub_011408E0(rdram, ctx, runtime); break;
    case 0x1140e60u: ref::manager_sub_01140E60(rdram, ctx, runtime); break;
    case 0x1140fd8u: ref::manager_sub_01140FD8(rdram, ctx, runtime); break;
    case 0x11418d8u: ref::manager_sub_011418D8(rdram, ctx, runtime); break;
    case 0x1141e80u: ref::manager_sub_01141E80(rdram, ctx, runtime); break;
    case 0x1145ba8u: ref::get_particle_parameter_blend_disp(rdram, ctx, runtime); break;
    case 0x1147a78u: ref::InitTwoWayLink(rdram, ctx, runtime); break;
    case 0x1147a88u: ref::AddTwoWayLinkNext(rdram, ctx, runtime); break;
    case 0x1147ab0u: ref::DeleteTwoWayLink(rdram, ctx, runtime); break;
    case 0x1157a98u: ref::adpcmSetPan(rdram, ctx, runtime); break;
    case 0x115a060u: ref::dsysFileInit(rdram, ctx, runtime); break;
    case 0x115cf90u: ref::sheetFileGetDataAdrs(rdram, ctx, runtime); break;
    case 0x115ec60u: ref::sheetMgrSheetHeaderSearchSheetDataObj(rdram, ctx, runtime); break;
    case 0x11629e8u: ref::manager_sub_011629E8(rdram, ctx, runtime); break;
    case 0x1163848u: ref::get_hit_region_ofst_camera_region(rdram, ctx, runtime); break;
    case 0x11644d0u: ref::set_scissor_screen_size(rdram, ctx, runtime); break;
    case 0x1166e50u: ref::DecreaseDamageLockTime(rdram, ctx, runtime); break;
    case 0x1166e98u: ref::GetDamagerClassDamageSheetType(rdram, ctx, runtime); break;
    case 0x11692a8u: ref::ResetAnimeFrameTableInfo(rdram, ctx, runtime); break;
    case 0x116ad80u: ref::rintf(rdram, ctx, runtime); break;
    case 0x1343eb8u: ref::GAMEUTIL_set_parameter_look_at_vs_offset(rdram, ctx, runtime); break;
    case 0x13445d8u: ref::GAMEUTIL_doit_status_manager(rdram, ctx, runtime); break;
    case 0x1345ce0u: ref::ReleaseGame(rdram, ctx, runtime); break;
    case 0x1346300u: ref::getDemoBarFade(rdram, ctx, runtime); break;
    case 0x1347028u: ref::set_base_pitch_parameter_nico_yaw_pitch_cntrl(rdram, ctx, runtime); break;
    case 0x1347b28u: ref::gamecore_sub_01347B28(rdram, ctx, runtime); break;
    case 0x1347b88u: ref::gamecore_sub_01347B88(rdram, ctx, runtime); break;
    case 0x1349318u: ref::backStageCheckPointSave(rdram, ctx, runtime); break;
    case 0x13493e0u: ref::backStageCheckPointLoad(rdram, ctx, runtime); break;
    case 0x134ef50u: ref::_optionWorkSet(rdram, ctx, runtime); break;
    case 0x134efb0u: ref::_gameFlagSet(rdram, ctx, runtime); break;
    case 0x134eff0u: ref::_gameFlagCls(rdram, ctx, runtime); break;
    case 0x134f030u: ref::_gameFlagChk(rdram, ctx, runtime); break;
    case 0x134f0a8u: ref::_gameWorkSet(rdram, ctx, runtime); break;
    case 0x134f0c0u: ref::_gameFWorkSet(rdram, ctx, runtime); break;
    case 0x134f120u: ref::_gameFWorkChk(rdram, ctx, runtime); break;
    case 0x134f1b0u: ref::fruitsFlagChk(rdram, ctx, runtime); break;
    case 0x134f350u: ref::gameFlagCheckPointLoad(rdram, ctx, runtime); break;
    case 0x134f820u: ref::tailsFlagChk(rdram, ctx, runtime); break;
    case 0x134f938u: ref::gexsFlagChk(rdram, ctx, runtime); break;
    case 0x134fa18u: ref::_omakeGetFlagChk(rdram, ctx, runtime); break;
    case 0x134fb28u: ref::_omakeSelectFlagChk(rdram, ctx, runtime); break;
    case 0x1352030u: ref::gameflowBossEyeCameraDisable(rdram, ctx, runtime); break;
    case 0x1352b30u: ref::gameModeSet(rdram, ctx, runtime); break;
    case 0x1352b88u: ref::gameModeCls(rdram, ctx, runtime); break;
    case 0x1353440u: ref::gStatPlaceIdToSavePoint(rdram, ctx, runtime); break;
    case 0x1353e18u: ref::gstatBootMcSetLastSaveOmakeStat(rdram, ctx, runtime); break;
    case 0x13576d8u: ref::opflowSetDemoFrame(rdram, ctx, runtime); break;
    case 0x1358888u: ref::getStageChipID(rdram, ctx, runtime); break;
    case 0x1358a68u: ref::gamecore_sub_01358A68(rdram, ctx, runtime); break;
    case 0x1365ab0u: ref::stagemanager_checkstageAlreadyLoadRequestedByStrptr(rdram, ctx, runtime); break;
    case 0x1365d18u: ref::gcGetStageData(rdram, ctx, runtime); break;
    case 0x13663d0u: ref::stagemanager_getStageNumberWithStageDataStrptr(rdram, ctx, runtime); break;
    case 0x13678e8u: ref::PauseStageBoundaryManager(rdram, ctx, runtime); break;
    case 0x136a2e0u: ref::initBgmScript(rdram, ctx, runtime); break;
    case 0x136a328u: ref::gamecore_sub_0136A328(rdram, ctx, runtime); break;
    case 0x1379270u: ref::GetBossPartsDamagePerParts(rdram, ctx, runtime); break;
    case 0x137def8u: ref::getexpressiondatapoint(rdram, ctx, runtime); break;
    case 0x137e828u: ref::controllerAlertDisable(rdram, ctx, runtime); break;
    case 0x1386b98u: ref::gamecore_sub_01386B98(rdram, ctx, runtime); break;
    case 0x13883b0u: ref::fadeout_weakpoint_effect(rdram, ctx, runtime); break;
    case 0x13883f8u: ref::cancel_fadeout_weakpoint_effect(rdram, ctx, runtime); break;
    case 0x138be30u: ref::set_node_data_expression(rdram, ctx, runtime); break;
    case 0x138bec8u: ref::constructor_expression_change(rdram, ctx, runtime); break;
    case 0x138eab0u: ref::EnableArrowWater(rdram, ctx, runtime); break;
    case 0x138eaf8u: ref::EndArrowWater(rdram, ctx, runtime); break;
    case 0x1393e58u: ref::gamecore_sub_01393E58(rdram, ctx, runtime); break;
    case 0x139f3b0u: ref::gamecore_sub_0139F3B0(rdram, ctx, runtime); break;
    case 0x13d46d8u: ref::AI_GetPosition3(rdram, ctx, runtime); break;
    case 0x13d4a28u: ref::AI_GetInterRatio(rdram, ctx, runtime); break;
    case 0x13d4a60u: ref::AI_GetInterRatioOutCut(rdram, ctx, runtime); break;
    case 0x13e79c8u: ref::Kite_CmpPathPoint(rdram, ctx, runtime); break;
    case 0x13ef210u: ref::gamecore_sub_013EF210(rdram, ctx, runtime); break;
    case 0x13f0d60u: ref::getSObjNode(rdram, ctx, runtime); break;
    case 0x13f1290u: ref::gamecore_sub_013F1290(rdram, ctx, runtime); break;
    case 0x13f12f0u: ref::gamecore_sub_013F12F0(rdram, ctx, runtime); break;
    case 0x13f1330u: ref::gamecore_sub_013F1330(rdram, ctx, runtime); break;
    case 0x13f1370u: ref::gamecore_sub_013F1370(rdram, ctx, runtime); break;
    case 0x13f13e0u: ref::gamecore_sub_013F13E0(rdram, ctx, runtime); break;
    case 0x13f1430u: ref::gamecore_sub_013F1430(rdram, ctx, runtime); break;
    case 0x13f2558u: ref::gamecore_sub_013F2558(rdram, ctx, runtime); break;
    case 0x13f95a0u: ref::initWeaponSpriteStatus(rdram, ctx, runtime); break;
    case 0x140f418u: ref::get_camera_collision_data_base_filter_radius(rdram, ctx, runtime); break;
    case 0x1413538u: ref::is_cliff_rise_ok_camera_place_status(rdram, ctx, runtime); break;
    case 0x1413608u: ref::act_local_vs_ofst_camera_place_status(rdram, ctx, runtime); break;
    case 0x14144f0u: ref::gamecore_sub_014144F0(rdram, ctx, runtime); break;
    case 0x1414e78u: ref::gamecore_sub_01414E78(rdram, ctx, runtime); break;
    case 0x141b6c0u: ref::gamecore_sub_0141B6C0(rdram, ctx, runtime); break;
    case 0x141b900u: ref::gamecore_sub_0141B900(rdram, ctx, runtime); break;
    case 0x141ba40u: ref::gamecore_sub_0141BA40(rdram, ctx, runtime); break;
    case 0x141bc78u: ref::gamecore_sub_0141BC78(rdram, ctx, runtime); break;
    case 0x14255a0u: ref::add_camera_mode_common_param_segment(rdram, ctx, runtime); break;
    case 0x14255f0u: ref::add_camera_mode_manager_segment(rdram, ctx, runtime); break;
    case 0x142c1a0u: ref::is_base_default_dir_attr_change_game_camera_param_manager(rdram, ctx, runtime); break;
    case 0x142c7a0u: ref::gamecore_sub_0142C7A0(rdram, ctx, runtime); break;
    case 0x1430248u: ref::gamecore_sub_01430248(rdram, ctx, runtime); break;
    case 0x1433340u: ref::getPathDataAddr(rdram, ctx, runtime); break;
    case 0x1434610u: ref::gamecore_sub_01434610(rdram, ctx, runtime); break;
    case 0x1437f28u: ref::calcDownSpeedRatio(rdram, ctx, runtime); break;
    case 0x143d6f8u: ref::gamecore_sub_0143D6F8(rdram, ctx, runtime); break;
    case 0x1440f48u: ref::setRunStopTurnAngleFastLegCtrlWork(rdram, ctx, runtime); break;
    case 0x1441020u: ref::gamecore_sub_01441020(rdram, ctx, runtime); break;
    case 0x1441210u: ref::gamecore_sub_01441210(rdram, ctx, runtime); break;
    case 0x1444370u: ref::gamecore_sub_01444370(rdram, ctx, runtime); break;
    case 0x1446f80u: ref::adjustAnglePI(rdram, ctx, runtime); break;
    case 0x145f0e8u: ref::initBeardIKWork(rdram, ctx, runtime); break;
    case 0x1461758u: ref::setRatioGazeIKWork(rdram, ctx, runtime); break;
    case 0x14647f0u: ref::setWormIKNodeLimit(rdram, ctx, runtime); break;
    case 0x1ad958u: ref::kernel_sub_001AD958(rdram, ctx, runtime); break;
    case 0x1cc278u: ref::Sg2CtrlInit(rdram, ctx, runtime); break;
    case 0x1084428u: ref::GAMEUTIL_reset_interp_value_cntrl(rdram, ctx, runtime); break;
    case 0x1084528u: ref::constructor_double_spring_interp_cntrl(rdram, ctx, runtime); break;
    case 0x10845f8u: ref::constructor_double_spring_interp_cntrl3D(rdram, ctx, runtime); break;
    case 0x10df5d0u: ref::manager_sub_010DF5D0(rdram, ctx, runtime); break;
    case 0x110d0a0u: ref::SetScriptLayoutObjLetterProcMode(rdram, ctx, runtime); break;
    case 0x13538e8u: ref::gStatGetHorseDemoStageIndex(rdram, ctx, runtime); break;
    case 0x13586d0u: ref::getPlayerMaxGripRatio(rdram, ctx, runtime); break;
    case 0x1366d20u: ref::initGameObject(rdram, ctx, runtime); break;
    case 0x13ae748u: ref::gamecore_sub_013AE748(rdram, ctx, runtime); break;
    case 0x13f3378u: ref::gamecore_sub_013F3378(rdram, ctx, runtime); break;
    case 0x13f3778u: ref::gamecore_sub_013F3778(rdram, ctx, runtime); break;
    case 0x13f3868u: ref::gamecore_sub_013F3868(rdram, ctx, runtime); break;
    case 0x13ff4a8u: ref::checkChangeWeaponTypeSwordToOther(rdram, ctx, runtime); break;
    case 0x14072f8u: ref::initWeapon(rdram, ctx, runtime); break;
    case 0x14140b8u: ref::gamecore_sub_014140B8(rdram, ctx, runtime); break;
    case 0x1425518u: ref::is_view_angle_zoom_game_camera(rdram, ctx, runtime); break;
    case 0x1459a20u: ref::SCRConditionInvisibilityCloak(rdram, ctx, runtime); break;
    case 0x1459a38u: ref::SCRConditionGetOmakeFlag(rdram, ctx, runtime); break;
    case 0x145aeb0u: ref::eventBgmAllClose(rdram, ctx, runtime); break;
    case 0x1b7bb8u: ref::iosCubicInterVectorXYZ(rdram, ctx, runtime); break;
    case 0x1b7c88u: ref::iosCubicBezierVectorXYZ(rdram, ctx, runtime); break;
    case 0x1b8620u: ref::iosUnitCurrentMatrix(rdram, ctx, runtime); break;
    case 0x1b8668u: ref::iosPushVu0RegistersToWork(rdram, ctx, runtime); break;
    case 0x1b8770u: ref::iosPopVu0RegistersFromWork(rdram, ctx, runtime); break;
    case 0x1b8878u: ref::iosTransCurrentMatrix(rdram, ctx, runtime); break;
    case 0x1b8898u: ref::iosSetTransCurrentMatrix(rdram, ctx, runtime); break;
    case 0x1b88a8u: ref::iosClearTransCurrentMatrix(rdram, ctx, runtime); break;
    case 0x1b8c00u: ref::iosScaleCurrentMatrix(rdram, ctx, runtime); break;
    case 0x1b8c90u: ref::iosGetCurrentMatrix(rdram, ctx, runtime); break;
    case 0x1b8ca8u: ref::iosGetCurrentMatrixTrans(rdram, ctx, runtime); break;
    case 0x1b8cb8u: ref::iosSetCurrentMatrix(rdram, ctx, runtime); break;
    case 0x1b8cd0u: ref::iosMulCurrentMatrixR(rdram, ctx, runtime); break;
    case 0x1b8d38u: ref::iosMulCurrentMatrixL(rdram, ctx, runtime); break;
    case 0x1b8d90u: ref::iosApplyCurrentMatrix(rdram, ctx, runtime); break;
    case 0x1b8db0u: ref::iosApplyRotCurrentMatrix(rdram, ctx, runtime); break;
    case 0x1b8dd0u: ref::iosRotTransPersCurrentMatrix(rdram, ctx, runtime); break;
    case 0x1b8e00u: ref::iosTransposeCurrentMatrix(rdram, ctx, runtime); break;
    case 0x1b8e50u: ref::iosTransposeRotationCurrentMatrix(rdram, ctx, runtime); break;
    case 0x1b8e80u: ref::iosInverseCurrentMatrix(rdram, ctx, runtime); break;
    case 0x1b9170u: ref::iosApplyRotMatrix(rdram, ctx, runtime); break;
    case 0x1b91a0u: ref::iosUnitMatrix(rdram, ctx, runtime); break;
    case 0x1b91c8u: ref::iosUnitRotation(rdram, ctx, runtime); break;
    case 0x1b92d0u: ref::iosScaleMatrixV(rdram, ctx, runtime); break;
    case 0x1b9468u: ref::iosMakeNormalLightMatrix(rdram, ctx, runtime); break;
    case 0x1b9560u: ref::iosScaleNormalLightMatrix(rdram, ctx, runtime); break;
    case 0x1b9598u: ref::iosMakeLightColorMatrix(rdram, ctx, runtime); break;
    case 0x1b95d0u: ref::iosTransMatrix(rdram, ctx, runtime); break;
    case 0x1b96a0u: ref::iosScaleMatrix(rdram, ctx, runtime); break;
    case 0x1b9cd0u: ref::iosGetYDirXYZFromMatrix(rdram, ctx, runtime); break;
    case 0x1b9ce8u: ref::iosGetXDirXYZFromMatrix(rdram, ctx, runtime); break;
    case 0x1b9d18u: ref::iosRotTransCurrentMatrix(rdram, ctx, runtime); break;
    case 0x1b9d78u: ref::iosMvecZero(rdram, ctx, runtime); break;
    case 0x1bb8a8u: ref::MultiCurrentQuaternion(rdram, ctx, runtime); break;
    case 0x1bb900u: ref::MultiCurrentQuaternionL(rdram, ctx, runtime); break;
    case 0x1bb958u: ref::InvertCurrentQuaternion(rdram, ctx, runtime); break;
    case 0x1bb988u: ref::SetCurrentQuaternion(rdram, ctx, runtime); break;
    case 0x1bbc28u: ref::InitQuaternionDrive(rdram, ctx, runtime); break;
    case 0x1bbe10u: ref::DivQuaternion(rdram, ctx, runtime); break;
    case 0x1bc850u: ref::SetQuaternionByCosineAxisRotateVWithNoRegularize(rdram, ctx, runtime); break;
    case 0x1bc8f8u: ref::GetYRotQuaternionFromDir(rdram, ctx, runtime); break;
    case 0x1be6d0u: ref::iosFTOI15Vector(rdram, ctx, runtime); break;
    case 0x1be6e8u: ref::iosFTOI12Vector(rdram, ctx, runtime); break;
    case 0x1be718u: ref::iosFTOI0Vector(rdram, ctx, runtime); break;
    case 0x1be730u: ref::iosITOF15Vector(rdram, ctx, runtime); break;
    case 0x1be748u: ref::iosITOF12Vector(rdram, ctx, runtime); break;
    case 0x1be760u: ref::iosITOF4Vector(rdram, ctx, runtime); break;
    case 0x1be778u: ref::iosITOF0Vector(rdram, ctx, runtime); break;
    case 0x1be790u: ref::iosGetStabXZVector(rdram, ctx, runtime); break;
    case 0x1be8b0u: ref::iosGetReflectVector(rdram, ctx, runtime); break;
    case 0x1be9e0u: ref::iosGetStabXYVector(rdram, ctx, runtime); break;
    case 0x1bea18u: ref::iosGetStabYZVector(rdram, ctx, runtime); break;
    case 0x1bea50u: ref::iosGetLengthPointToLine(rdram, ctx, runtime); break;
    case 0x1beb08u: ref::iosGetInverseVectorXYZ(rdram, ctx, runtime); break;
    case 0x1beb30u: ref::iosGetExceptAxisVector(rdram, ctx, runtime); break;
    case 0x1beb88u: ref::iosCalcFColorToBrightness(rdram, ctx, runtime); break;
    case 0x1bebc0u: ref::iosITOF15VectorXYZ(rdram, ctx, runtime); break;
    case 0x1bebd8u: ref::iosScaleColorTo255(rdram, ctx, runtime); break;
    case 0x1bebf8u: ref::iosScaleColorToONE(rdram, ctx, runtime); break;
    case 0x1c84f0u: ref::kernel_sub_001C84F0(rdram, ctx, runtime); break;
    case 0x107efb8u: ref::GAMEUTIL_get_xyz_axis_from_quaternion(rdram, ctx, runtime); break;
    case 0x107fbf0u: ref::GAMEUTIL_get_vector_velocity_by_diff_move3D(rdram, ctx, runtime); break;
    case 0x107ff08u: ref::GAMEUTIL_check_and_get_normalize_vector(rdram, ctx, runtime); break;
    case 0x107ffe0u: ref::GAMEUTIL_get_divide_vector_value(rdram, ctx, runtime); break;
    case 0x1081a78u: ref::GAMEUTIL_divide_yaw_pitch_dir(rdram, ctx, runtime); break;
    case 0x1082bd8u: ref::manager_sub_01082BD8(rdram, ctx, runtime); break;
    case 0x1083870u: ref::GAMEUTIL_get_interp_value3D(rdram, ctx, runtime); break;
    case 0x1087498u: ref::getZ(rdram, ctx, runtime); break;
    case 0x108d358u: ref::manager_sub_0108D358(rdram, ctx, runtime); break;
    case 0x108f678u: ref::iosgluGetViewCoordPosition(rdram, ctx, runtime); break;
    case 0x108f858u: ref::manager_sub_0108F858(rdram, ctx, runtime); break;
    case 0x10ad048u: ref::manager_sub_010AD048(rdram, ctx, runtime); break;
    case 0x10ba150u: ref::manager_sub_010BA150(rdram, ctx, runtime); break;
    case 0x10ba718u: ref::initGazeIKWork(rdram, ctx, runtime); break;
    case 0x10c0e48u: ref::GetCharacterClassControlFrontDirection(rdram, ctx, runtime); break;
    case 0x10c10e0u: ref::GetCharacterClassOptionNodeLocalQuaternionPos(rdram, ctx, runtime); break;
    case 0x10c1138u: ref::ResetCharacterClassFallDownInfo(rdram, ctx, runtime); break;
    case 0x10c17e0u: ref::ResetCharacterClassSlideStatus(rdram, ctx, runtime); break;
    case 0x10c17f8u: ref::ResetCharacterClassMotionInterpRootOfs(rdram, ctx, runtime); break;
    case 0x10c6d30u: ref::manager_sub_010C6D30(rdram, ctx, runtime); break;
    case 0x10d2b48u: ref::RequestCharacterClassMotionDirection(rdram, ctx, runtime); break;
    case 0x10d2ba0u: ref::RequestCharacterClassRootOffset(rdram, ctx, runtime); break;
    case 0x10dc1d8u: ref::charrootdyn_getInterpVector(rdram, ctx, runtime); break;
    case 0x10dc220u: ref::charrootdyn_getInterpPoint(rdram, ctx, runtime); break;
    case 0x10dcdc0u: ref::charrootdyn_getvelocity(rdram, ctx, runtime); break;
    case 0x10ebc80u: ref::SetDemoMotionOrientInfo(rdram, ctx, runtime); break;
    case 0x10eec00u: ref::SetJudgeVolumeInfo(rdram, ctx, runtime); break;
    case 0x10f8d30u: ref::UpdateGlobalLightInRange(rdram, ctx, runtime); break;
    case 0x1123fa0u: ref::CopyCollisionDynamics(rdram, ctx, runtime); break;
    case 0x1123fd0u: ref::FilterCopyCollisionDynamics(rdram, ctx, runtime); break;
    case 0x1129258u: ref::manager_sub_01129258(rdram, ctx, runtime); break;
    case 0x1129ea8u: ref::SetCollisionEdgeLinkDBFinderVWallDirectionStatus(rdram, ctx, runtime); break;
    case 0x1129eb8u: ref::SetCollisionEdgeLinkDBFinderCollectStatus(rdram, ctx, runtime); break;
    case 0x11328f8u: ref::SetGeometryPointerGlobalState(rdram, ctx, runtime); break;
    case 0x1132970u: ref::SetGeometryPointerGlobalStatePointer(rdram, ctx, runtime); break;
    case 0x1132c60u: ref::SetGeometryPointerNodePosState(rdram, ctx, runtime); break;
    case 0x1132c88u: ref::SetGeometryPointerNodePosStateGlobalRot(rdram, ctx, runtime); break;
    case 0x1134140u: ref::GetPointDistanceSquareFromUnnormalizedPlane(rdram, ctx, runtime); break;
    case 0x1134198u: ref::GetPointDistanceFromPlane(rdram, ctx, runtime); break;
    case 0x11341c8u: ref::MultiMatrixByPlane(rdram, ctx, runtime); break;
    case 0x1134228u: ref::CompletePlaneD(rdram, ctx, runtime); break;
    case 0x1134250u: ref::GetSTPositionOfVectorPlane(rdram, ctx, runtime); break;
    case 0x1134390u: ref::GetPlaneFittingDiffVector(rdram, ctx, runtime); break;
    case 0x1134440u: ref::GetPlaneFittingPointForUnnormalizedPlane(rdram, ctx, runtime); break;
    case 0x1134628u: ref::GetPlaneFittingVector(rdram, ctx, runtime); break;
    case 0x11346c8u: ref::GetPlaneReflectionVector(rdram, ctx, runtime); break;
    case 0x1134730u: ref::GetNormalPlane(rdram, ctx, runtime); break;
    case 0x11348e0u: ref::GetCrossPositionPlaneLine(rdram, ctx, runtime); break;
    case 0x1135b50u: ref::GetPlaneDistance(rdram, ctx, runtime); break;
    case 0x1135b78u: ref::GetDistanceSquare(rdram, ctx, runtime); break;
    case 0x1135ba0u: ref::GetDistanceSquareXZ(rdram, ctx, runtime); break;
    case 0x1135bc8u: ref::ExchangeLWVector(rdram, ctx, runtime); break;
    case 0x1142198u: ref::manager_sub_01142198(rdram, ctx, runtime); break;
    case 0x11422d8u: ref::manager_sub_011422D8(rdram, ctx, runtime); break;
    case 0x1146da8u: ref::manager_sub_01146DA8(rdram, ctx, runtime); break;
    case 0x1146eb8u: ref::manager_sub_01146EB8(rdram, ctx, runtime); break;
    case 0x1161c08u: ref::constructor_camera_lws_anime_data(rdram, ctx, runtime); break;
    case 0x1162e80u: ref::set_camera_position(rdram, ctx, runtime); break;
    case 0x1162e90u: ref::set_camera_direction(rdram, ctx, runtime); break;
    case 0x1162ec0u: ref::set_camera_up_direction(rdram, ctx, runtime); break;
    case 0x1166da0u: ref::InitDamageClass(rdram, ctx, runtime); break;
    case 0x1343b38u: ref::GAMEUTIL_set_custom_boss_interest_position(rdram, ctx, runtime); break;
    case 0x1343fe0u: ref::GAMEUTIL_constructor_col_plane_info(rdram, ctx, runtime); break;
    case 0x13441e8u: ref::GAMEUTIL_reset_position_filter(rdram, ctx, runtime); break;
    case 0x1344210u: ref::GAMEUTIL_doit_position_filter(rdram, ctx, runtime); break;
    case 0x134a7d0u: ref::SetCamFinalizerGeometry(rdram, ctx, runtime); break;
    case 0x134f150u: ref::fruitsFlagSet(rdram, ctx, runtime); break;
    case 0x137ea00u: ref::gamecore_sub_0137EA00(rdram, ctx, runtime); break;
    case 0x139a2a0u: ref::Init_TimeFVECTORInter(rdram, ctx, runtime); break;
    case 0x139a2c8u: ref::Update_TimeFVECTORInter(rdram, ctx, runtime); break;
    case 0x13a7218u: ref::CalcFootVec(rdram, ctx, runtime); break;
    case 0x13a74b0u: ref::MVHIST_Init(rdram, ctx, runtime); break;
    case 0x13aa498u: ref::get_predict_hit_position_nico_sight_on(rdram, ctx, runtime); break;
    case 0x13d3980u: ref::AI_InitAnimalLayout(rdram, ctx, runtime); break;
    case 0x13d39a0u: ref::AI_SetAnimalLayout(rdram, ctx, runtime); break;
    case 0x13d39b8u: ref::AI_CheckInAnimalLayout(rdram, ctx, runtime); break;
    case 0x13d3fa0u: ref::AI_GetMotionBasicVelocity(rdram, ctx, runtime); break;
    case 0x13d41a0u: ref::AI_GetAccelA3XYZ(rdram, ctx, runtime); break;
    case 0x13d4308u: ref::AI_GetAccelB3XYZ(rdram, ctx, runtime); break;
    case 0x13d4458u: ref::AI_GetAccelC3XYZ(rdram, ctx, runtime); break;
    case 0x13d4638u: ref::AI_GetVelocity3XYZ(rdram, ctx, runtime); break;
    case 0x13d4740u: ref::AI_GetPosition3XYZ(rdram, ctx, runtime); break;
    case 0x13d4d30u: ref::AI_GetTrgDir(rdram, ctx, runtime); break;
    case 0x13d4ef8u: ref::AI_GetSideSignalFromDirLev(rdram, ctx, runtime); break;
    case 0x13d8dc8u: ref::gamecore_sub_013D8DC8(rdram, ctx, runtime); break;
    case 0x13d8fa0u: ref::gamecore_sub_013D8FA0(rdram, ctx, runtime); break;
    case 0x13d9518u: ref::gamecore_sub_013D9518(rdram, ctx, runtime); break;
    case 0x13e79f8u: ref::Kite_InitKitePathPoint(rdram, ctx, runtime); break;
    case 0x13e7c78u: ref::Kite_SetAnimalLayout(rdram, ctx, runtime); break;
    case 0x13e7cc8u: ref::Kite_SetPathOfs(rdram, ctx, runtime); break;
    case 0x1407568u: ref::sprWeaponSetMarker(rdram, ctx, runtime); break;
    case 0x14075a0u: ref::sprWeaponGetMarkerPos(rdram, ctx, runtime); break;
    case 0x1411708u: ref::gamecore_sub_01411708(rdram, ctx, runtime); break;
    case 0x1411af8u: ref::gamecore_sub_01411AF8(rdram, ctx, runtime); break;
    case 0x1412970u: ref::constructor_camera_pause_rise(rdram, ctx, runtime); break;
    case 0x1413720u: ref::gamecore_sub_01413720(rdram, ctx, runtime); break;
    case 0x1414db0u: ref::constructor_camera_target_lost_angle(rdram, ctx, runtime); break;
    case 0x1415970u: ref::reset_camera_target_visible_angle_manager(rdram, ctx, runtime); break;
    case 0x1419b48u: ref::CAM_get_camera_position_by_look_at(rdram, ctx, runtime); break;
    case 0x1419f68u: ref::CAM_add_plane_offset_in_position(rdram, ctx, runtime); break;
    case 0x141a140u: ref::CAM_set_all_camera_base_data(rdram, ctx, runtime); break;
    case 0x141b110u: ref::CAM_constructor_base_look_at(rdram, ctx, runtime); break;
    case 0x1424c30u: ref::copy_mode_common_camera_data(rdram, ctx, runtime); break;
    case 0x1425378u: ref::get_boss_interest_position_game_camera(rdram, ctx, runtime); break;
    case 0x1432030u: ref::setAttainRangeData(rdram, ctx, runtime); break;
    case 0x1433f98u: ref::gamecore_sub_01433F98(rdram, ctx, runtime); break;
    case 0x14340f0u: ref::gamecore_sub_014340F0(rdram, ctx, runtime); break;
    case 0x143d718u: ref::gamecore_sub_0143D718(rdram, ctx, runtime); break;
    case 0x1440fe0u: ref::gamecore_sub_01440FE0(rdram, ctx, runtime); break;
    case 0x14418f0u: ref::setPosGazeCtrlWork(rdram, ctx, runtime); break;
    case 0x1444568u: ref::gamecore_sub_01444568(rdram, ctx, runtime); break;
    case 0x144cdb8u: ref::resetVelocityWormCtrlWork(rdram, ctx, runtime); break;
    case 0x145d690u: ref::initAimIKWork(rdram, ctx, runtime); break;
    case 0x145dfd0u: ref::initAttackWaistIKWork(rdram, ctx, runtime); break;
    case 0x1461850u: ref::updateGazeIKWork(rdram, ctx, runtime); break;
    case 0x1462310u: ref::ReflectIK_initWormDynamics(rdram, ctx, runtime); break;
    case 0x1083e38u: ref::GAMEUTIL_get_spring_interpolate_offset3D(rdram, ctx, runtime); break;
    case 0x10b9070u: ref::get_temp_parameter_motion_ik_base(rdram, ctx, runtime); break;
    case 0x1344040u: ref::GAMEUTIL_init_col_plane_info_filter(rdram, ctx, runtime); break;
    case 0x1381aa8u: ref::gamecore_sub_01381AA8(rdram, ctx, runtime); break;
    case 0x13dc4a8u: ref::gamecore_sub_013DC4A8(rdram, ctx, runtime); break;
    case 0x13e7900u: ref::Kite_InitSpecialPathWork(rdram, ctx, runtime); break;
    case 0x13e7ce0u: ref::Kite_SetSpecialPathLayout(rdram, ctx, runtime); break;
    case 0x1411ad0u: ref::gamecore_sub_01411AD0(rdram, ctx, runtime); break;
    case 0x14129c0u: ref::init_camera_pause_rise(rdram, ctx, runtime); break;
    case 0x1440f60u: ref::gamecore_sub_01440F60(rdram, ctx, runtime); break;
    case 0x1444518u: ref::gamecore_sub_01444518(rdram, ctx, runtime); break;
    case 0x144cdf8u: ref::gamecore_sub_0144CDF8(rdram, ctx, runtime); break;
    default: ckUnknownCall(t); return true;
    }
    (void)ret;
    return true;
}
void iosCubicInterVectorXYZ(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t p4, uint32_t p5, uint32_t p6, uint32_t p7, float pf12)
{
    if (!ckBegin(0, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    if (!ckLeg([&] { ref::iosCubicInterVectorXYZ(rdram, &c, runtime); })) return;
    ckMid();
    uint64_t v0 = 0u;
    float f0 = 0.0f;
    a4::vec4 v_vf0 = ctx->vu0_vf[0];
    a4::vec4 v_vf5 = ctx->vu0_vf[5];
    a4::vec4 v_vf6 = ctx->vu0_vf[6];
    a4::vec4 v_vf7 = ctx->vu0_vf[7];
    a4::vec4 v_vf8 = ctx->vu0_vf[8];
    a4::vec4 v_vacc = ctx->vu0_acc;
    uint64_t v_h8 = a4::hi64(ctx->r[8]);
    if (!ckLeg([&] { f0 = ty::iosCubicInterVectorXYZ(rdram, runtime, p4, p5, p6, p7, pf12, v_vf0, v_vf5, v_vf6, v_vf7, v_vf8, v_vacc, v_h8); })) return;
    R5900Context &t = ckTyCtx(ctx);
    t.vu0_vf[5] = v_vf5;
    t.vu0_vf[6] = v_vf6;
    t.vu0_vf[7] = v_vf7;
    t.vu0_vf[8] = v_vf8;
    t.vu0_acc = v_vacc;
    ckEnd(0, ctx, c, 3, v0, f0, 0x102u, 0x3eu, false, vuDiffer(t, c));
}
void iosCubicBezierVectorXYZ(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t p4, uint32_t p5, float pf12)
{
    if (!ckBegin(1, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    if (!ckLeg([&] { ref::iosCubicBezierVectorXYZ(rdram, &c, runtime); })) return;
    ckMid();
    uint64_t v0 = 0u;
    float f0 = 0.0f;
    a4::vec4 v_vf5 = ctx->vu0_vf[5];
    a4::vec4 v_vf6 = ctx->vu0_vf[6];
    uint64_t v_h2 = a4::hi64(ctx->r[2]);
    uint64_t v_h3 = a4::hi64(ctx->r[3]);
    if (!ckLeg([&] { const a1::RetVF r = ty::iosCubicBezierVectorXYZ(rdram, runtime, p4, p5, pf12, v_vf5, v_vf6, v_h2, v_h3); v0 = r.v0; f0 = r.f0; })) return;
    R5900Context &t = ckTyCtx(ctx);
    t.vu0_vf[5] = v_vf5;
    t.vu0_vf[6] = v_vf6;
    ckEnd(1, ctx, c, 4, v0, f0, 0x2au, 0x1002u, false, vuDiffer(t, c));
}
void iosUnitCurrentMatrix(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (!ckBegin(2, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    if (!ckLeg([&] { ref::iosUnitCurrentMatrix(rdram, &c, runtime); })) return;
    ckMid();
    uint64_t v0 = 0u;
    float f0 = 0.0f;
    a4::vec4 v_vf0 = ctx->vu0_vf[0];
    a4::vec4 v_vf1 = ctx->vu0_vf[1];
    a4::vec4 v_vf2 = ctx->vu0_vf[2];
    a4::vec4 v_vf3 = ctx->vu0_vf[3];
    a4::vec4 v_vf4 = ctx->vu0_vf[4];
    if (!ckLeg([&] { ty::iosUnitCurrentMatrix(rdram, runtime, v_vf0, v_vf1, v_vf2, v_vf3, v_vf4); })) return;
    R5900Context &t = ckTyCtx(ctx);
    t.vu0_vf[1] = v_vf1;
    t.vu0_vf[2] = v_vf2;
    t.vu0_vf[3] = v_vf3;
    t.vu0_vf[4] = v_vf4;
    ckEnd(2, ctx, c, 0, v0, f0, 0x0u, 0x0u, false, vuDiffer(t, c));
}
void iosPushVu0RegistersToWork(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t p4)
{
    if (!ckBegin(3, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    if (!ckLeg([&] { ref::iosPushVu0RegistersToWork(rdram, &c, runtime); })) return;
    ckMid();
    uint64_t v0 = 0u;
    float f0 = 0.0f;
    a4::vec4 v_vf1 = ctx->vu0_vf[1];
    a4::vec4 v_vf2 = ctx->vu0_vf[2];
    a4::vec4 v_vf3 = ctx->vu0_vf[3];
    a4::vec4 v_vf4 = ctx->vu0_vf[4];
    a4::vec4 v_vf5 = ctx->vu0_vf[5];
    a4::vec4 v_vf6 = ctx->vu0_vf[6];
    a4::vec4 v_vf7 = ctx->vu0_vf[7];
    a4::vec4 v_vf8 = ctx->vu0_vf[8];
    a4::vec4 v_vf9 = ctx->vu0_vf[9];
    a4::vec4 v_vf10 = ctx->vu0_vf[10];
    a4::vec4 v_vf11 = ctx->vu0_vf[11];
    a4::vec4 v_vf12 = ctx->vu0_vf[12];
    a4::vec4 v_vf13 = ctx->vu0_vf[13];
    a4::vec4 v_vf14 = ctx->vu0_vf[14];
    a4::vec4 v_vf15 = ctx->vu0_vf[15];
    a4::vec4 v_vf16 = ctx->vu0_vf[16];
    a4::vec4 v_vf17 = ctx->vu0_vf[17];
    a4::vec4 v_vf18 = ctx->vu0_vf[18];
    a4::vec4 v_vf19 = ctx->vu0_vf[19];
    a4::vec4 v_vf20 = ctx->vu0_vf[20];
    a4::vec4 v_vf21 = ctx->vu0_vf[21];
    a4::vec4 v_vf22 = ctx->vu0_vf[22];
    a4::vec4 v_vf23 = ctx->vu0_vf[23];
    a4::vec4 v_vf24 = ctx->vu0_vf[24];
    a4::vec4 v_vf25 = ctx->vu0_vf[25];
    a4::vec4 v_vf26 = ctx->vu0_vf[26];
    a4::vec4 v_vf27 = ctx->vu0_vf[27];
    a4::vec4 v_vf28 = ctx->vu0_vf[28];
    a4::vec4 v_vf29 = ctx->vu0_vf[29];
    a4::vec4 v_vf30 = ctx->vu0_vf[30];
    a4::vec4 v_vf31 = ctx->vu0_vf[31];
    if (!ckLeg([&] { v0 = a1_sx64(ty::iosPushVu0RegistersToWork(rdram, runtime, p4, v_vf1, v_vf2, v_vf3, v_vf4, v_vf5, v_vf6, v_vf7, v_vf8, v_vf9, v_vf10, v_vf11, v_vf12, v_vf13, v_vf14, v_vf15, v_vf16, v_vf17, v_vf18, v_vf19, v_vf20, v_vf21, v_vf22, v_vf23, v_vf24, v_vf25, v_vf26, v_vf27, v_vf28, v_vf29, v_vf30, v_vf31)); })) return;
    R5900Context &t = ckTyCtx(ctx);
    ckEnd(3, ctx, c, 1, v0, f0, 0x0u, 0x0u, false, vuDiffer(t, c));
}
void iosPopVu0RegistersFromWork(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t p4)
{
    if (!ckBegin(4, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    if (!ckLeg([&] { ref::iosPopVu0RegistersFromWork(rdram, &c, runtime); })) return;
    ckMid();
    uint64_t v0 = 0u;
    float f0 = 0.0f;
    a4::vec4 v_vf1 = ctx->vu0_vf[1];
    a4::vec4 v_vf2 = ctx->vu0_vf[2];
    a4::vec4 v_vf3 = ctx->vu0_vf[3];
    a4::vec4 v_vf4 = ctx->vu0_vf[4];
    a4::vec4 v_vf5 = ctx->vu0_vf[5];
    a4::vec4 v_vf6 = ctx->vu0_vf[6];
    a4::vec4 v_vf7 = ctx->vu0_vf[7];
    a4::vec4 v_vf8 = ctx->vu0_vf[8];
    a4::vec4 v_vf9 = ctx->vu0_vf[9];
    a4::vec4 v_vf10 = ctx->vu0_vf[10];
    a4::vec4 v_vf11 = ctx->vu0_vf[11];
    a4::vec4 v_vf12 = ctx->vu0_vf[12];
    a4::vec4 v_vf13 = ctx->vu0_vf[13];
    a4::vec4 v_vf14 = ctx->vu0_vf[14];
    a4::vec4 v_vf15 = ctx->vu0_vf[15];
    a4::vec4 v_vf16 = ctx->vu0_vf[16];
    a4::vec4 v_vf17 = ctx->vu0_vf[17];
    a4::vec4 v_vf18 = ctx->vu0_vf[18];
    a4::vec4 v_vf19 = ctx->vu0_vf[19];
    a4::vec4 v_vf20 = ctx->vu0_vf[20];
    a4::vec4 v_vf21 = ctx->vu0_vf[21];
    a4::vec4 v_vf22 = ctx->vu0_vf[22];
    a4::vec4 v_vf23 = ctx->vu0_vf[23];
    a4::vec4 v_vf24 = ctx->vu0_vf[24];
    a4::vec4 v_vf25 = ctx->vu0_vf[25];
    a4::vec4 v_vf26 = ctx->vu0_vf[26];
    a4::vec4 v_vf27 = ctx->vu0_vf[27];
    a4::vec4 v_vf28 = ctx->vu0_vf[28];
    a4::vec4 v_vf29 = ctx->vu0_vf[29];
    a4::vec4 v_vf30 = ctx->vu0_vf[30];
    a4::vec4 v_vf31 = ctx->vu0_vf[31];
    if (!ckLeg([&] { v0 = a1_sx64(ty::iosPopVu0RegistersFromWork(rdram, runtime, p4, v_vf1, v_vf2, v_vf3, v_vf4, v_vf5, v_vf6, v_vf7, v_vf8, v_vf9, v_vf10, v_vf11, v_vf12, v_vf13, v_vf14, v_vf15, v_vf16, v_vf17, v_vf18, v_vf19, v_vf20, v_vf21, v_vf22, v_vf23, v_vf24, v_vf25, v_vf26, v_vf27, v_vf28, v_vf29, v_vf30, v_vf31)); })) return;
    R5900Context &t = ckTyCtx(ctx);
    t.vu0_vf[1] = v_vf1;
    t.vu0_vf[2] = v_vf2;
    t.vu0_vf[3] = v_vf3;
    t.vu0_vf[4] = v_vf4;
    t.vu0_vf[5] = v_vf5;
    t.vu0_vf[6] = v_vf6;
    t.vu0_vf[7] = v_vf7;
    t.vu0_vf[8] = v_vf8;
    t.vu0_vf[9] = v_vf9;
    t.vu0_vf[10] = v_vf10;
    t.vu0_vf[11] = v_vf11;
    t.vu0_vf[12] = v_vf12;
    t.vu0_vf[13] = v_vf13;
    t.vu0_vf[14] = v_vf14;
    t.vu0_vf[15] = v_vf15;
    t.vu0_vf[16] = v_vf16;
    t.vu0_vf[17] = v_vf17;
    t.vu0_vf[18] = v_vf18;
    t.vu0_vf[19] = v_vf19;
    t.vu0_vf[20] = v_vf20;
    t.vu0_vf[21] = v_vf21;
    t.vu0_vf[22] = v_vf22;
    t.vu0_vf[23] = v_vf23;
    t.vu0_vf[24] = v_vf24;
    t.vu0_vf[25] = v_vf25;
    t.vu0_vf[26] = v_vf26;
    t.vu0_vf[27] = v_vf27;
    t.vu0_vf[28] = v_vf28;
    t.vu0_vf[29] = v_vf29;
    t.vu0_vf[30] = v_vf30;
    t.vu0_vf[31] = v_vf31;
    ckEnd(4, ctx, c, 1, v0, f0, 0x0u, 0x0u, false, vuDiffer(t, c));
}
void iosTransCurrentMatrix(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t p4)
{
    if (!ckBegin(5, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    if (!ckLeg([&] { ref::iosTransCurrentMatrix(rdram, &c, runtime); })) return;
    ckMid();
    uint64_t v0 = 0u;
    float f0 = 0.0f;
    a4::vec4 v_vf0 = ctx->vu0_vf[0];
    a4::vec4 v_vf1 = ctx->vu0_vf[1];
    a4::vec4 v_vf2 = ctx->vu0_vf[2];
    a4::vec4 v_vf3 = ctx->vu0_vf[3];
    a4::vec4 v_vf4 = ctx->vu0_vf[4];
    a4::vec4 v_vf8 = ctx->vu0_vf[8];
    a4::vec4 v_vacc = ctx->vu0_acc;
    if (!ckLeg([&] { ty::iosTransCurrentMatrix(rdram, runtime, p4, v_vf0, v_vf1, v_vf2, v_vf3, v_vf4, v_vf8, v_vacc); })) return;
    R5900Context &t = ckTyCtx(ctx);
    t.vu0_vf[4] = v_vf4;
    t.vu0_vf[8] = v_vf8;
    t.vu0_acc = v_vacc;
    ckEnd(5, ctx, c, 0, v0, f0, 0x0u, 0x0u, false, vuDiffer(t, c));
}
void iosSetTransCurrentMatrix(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t p4)
{
    if (!ckBegin(6, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    if (!ckLeg([&] { ref::iosSetTransCurrentMatrix(rdram, &c, runtime); })) return;
    ckMid();
    uint64_t v0 = 0u;
    float f0 = 0.0f;
    a4::vec4 v_vf4 = ctx->vu0_vf[4];
    a4::vec4 v_vf8 = ctx->vu0_vf[8];
    if (!ckLeg([&] { ty::iosSetTransCurrentMatrix(rdram, runtime, p4, v_vf4, v_vf8); })) return;
    R5900Context &t = ckTyCtx(ctx);
    t.vu0_vf[4] = v_vf4;
    t.vu0_vf[8] = v_vf8;
    ckEnd(6, ctx, c, 0, v0, f0, 0x0u, 0x0u, false, vuDiffer(t, c));
}
void iosClearTransCurrentMatrix(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (!ckBegin(7, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    if (!ckLeg([&] { ref::iosClearTransCurrentMatrix(rdram, &c, runtime); })) return;
    ckMid();
    uint64_t v0 = 0u;
    float f0 = 0.0f;
    a4::vec4 v_vf0 = ctx->vu0_vf[0];
    a4::vec4 v_vf4 = ctx->vu0_vf[4];
    if (!ckLeg([&] { ty::iosClearTransCurrentMatrix(rdram, runtime, v_vf0, v_vf4); })) return;
    R5900Context &t = ckTyCtx(ctx);
    t.vu0_vf[4] = v_vf4;
    ckEnd(7, ctx, c, 0, v0, f0, 0x0u, 0x0u, false, vuDiffer(t, c));
}
void iosScaleCurrentMatrix(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, float pf12, float pf13, float pf14)
{
    if (!ckBegin(8, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    if (!ckLeg([&] { ref::iosScaleCurrentMatrix(rdram, &c, runtime); })) return;
    ckMid();
    uint64_t v0 = 0u;
    float f0 = 0.0f;
    a4::vec4 v_vf0 = ctx->vu0_vf[0];
    a4::vec4 v_vf1 = ctx->vu0_vf[1];
    a4::vec4 v_vf2 = ctx->vu0_vf[2];
    a4::vec4 v_vf3 = ctx->vu0_vf[3];
    a4::vec4 v_vf4 = ctx->vu0_vf[4];
    a4::vec4 v_vf5 = ctx->vu0_vf[5];
    a4::vec4 v_vf6 = ctx->vu0_vf[6];
    a4::vec4 v_vf7 = ctx->vu0_vf[7];
    a4::vec4 v_vf14 = ctx->vu0_vf[14];
    a4::vec4 v_vf15 = ctx->vu0_vf[15];
    a4::vec4 v_vf16 = ctx->vu0_vf[16];
    a4::vec4 v_vf17 = ctx->vu0_vf[17];
    a4::vec4 v_vacc = ctx->vu0_acc;
    uint64_t v_h6 = a4::hi64(ctx->r[6]);
    uint64_t v_h7 = a4::hi64(ctx->r[7]);
    uint64_t v_h8 = a4::hi64(ctx->r[8]);
    if (!ckLeg([&] { ty::iosScaleCurrentMatrix(rdram, runtime, pf12, pf13, pf14, v_vf0, v_vf1, v_vf2, v_vf3, v_vf4, v_vf5, v_vf6, v_vf7, v_vf14, v_vf15, v_vf16, v_vf17, v_vacc, v_h6, v_h7, v_h8); })) return;
    R5900Context &t = ckTyCtx(ctx);
    t.vu0_vf[1] = v_vf1;
    t.vu0_vf[2] = v_vf2;
    t.vu0_vf[3] = v_vf3;
    t.vu0_vf[4] = v_vf4;
    t.vu0_vf[5] = v_vf5;
    t.vu0_vf[6] = v_vf6;
    t.vu0_vf[7] = v_vf7;
    t.vu0_vf[14] = v_vf14;
    t.vu0_vf[15] = v_vf15;
    t.vu0_vf[16] = v_vf16;
    t.vu0_vf[17] = v_vf17;
    t.vu0_acc = v_vacc;
    ckEnd(8, ctx, c, 0, v0, f0, 0x1c0u, 0x0u, false, vuDiffer(t, c));
}
void iosGetCurrentMatrix(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t p4)
{
    if (!ckBegin(9, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    if (!ckLeg([&] { ref::iosGetCurrentMatrix(rdram, &c, runtime); })) return;
    ckMid();
    uint64_t v0 = 0u;
    float f0 = 0.0f;
    a4::vec4 v_vf1 = ctx->vu0_vf[1];
    a4::vec4 v_vf2 = ctx->vu0_vf[2];
    a4::vec4 v_vf3 = ctx->vu0_vf[3];
    a4::vec4 v_vf4 = ctx->vu0_vf[4];
    if (!ckLeg([&] { ty::iosGetCurrentMatrix(rdram, runtime, p4, v_vf1, v_vf2, v_vf3, v_vf4); })) return;
    R5900Context &t = ckTyCtx(ctx);
    ckEnd(9, ctx, c, 0, v0, f0, 0x0u, 0x0u, false, vuDiffer(t, c));
}
void iosGetCurrentMatrixTrans(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t p4)
{
    if (!ckBegin(10, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    if (!ckLeg([&] { ref::iosGetCurrentMatrixTrans(rdram, &c, runtime); })) return;
    ckMid();
    uint64_t v0 = 0u;
    float f0 = 0.0f;
    a4::vec4 v_vf4 = ctx->vu0_vf[4];
    if (!ckLeg([&] { ty::iosGetCurrentMatrixTrans(rdram, runtime, p4, v_vf4); })) return;
    R5900Context &t = ckTyCtx(ctx);
    ckEnd(10, ctx, c, 0, v0, f0, 0x0u, 0x0u, false, vuDiffer(t, c));
}
void iosSetCurrentMatrix(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t p4)
{
    if (!ckBegin(11, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    if (!ckLeg([&] { ref::iosSetCurrentMatrix(rdram, &c, runtime); })) return;
    ckMid();
    uint64_t v0 = 0u;
    float f0 = 0.0f;
    a4::vec4 v_vf1 = ctx->vu0_vf[1];
    a4::vec4 v_vf2 = ctx->vu0_vf[2];
    a4::vec4 v_vf3 = ctx->vu0_vf[3];
    a4::vec4 v_vf4 = ctx->vu0_vf[4];
    if (!ckLeg([&] { ty::iosSetCurrentMatrix(rdram, runtime, p4, v_vf1, v_vf2, v_vf3, v_vf4); })) return;
    R5900Context &t = ckTyCtx(ctx);
    t.vu0_vf[1] = v_vf1;
    t.vu0_vf[2] = v_vf2;
    t.vu0_vf[3] = v_vf3;
    t.vu0_vf[4] = v_vf4;
    ckEnd(11, ctx, c, 0, v0, f0, 0x0u, 0x0u, false, vuDiffer(t, c));
}
void iosMulCurrentMatrixR(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t p4)
{
    if (!ckBegin(12, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    if (!ckLeg([&] { ref::iosMulCurrentMatrixR(rdram, &c, runtime); })) return;
    ckMid();
    uint64_t v0 = 0u;
    float f0 = 0.0f;
    a4::vec4 v_vf1 = ctx->vu0_vf[1];
    a4::vec4 v_vf2 = ctx->vu0_vf[2];
    a4::vec4 v_vf3 = ctx->vu0_vf[3];
    a4::vec4 v_vf4 = ctx->vu0_vf[4];
    a4::vec4 v_vf14 = ctx->vu0_vf[14];
    a4::vec4 v_vf15 = ctx->vu0_vf[15];
    a4::vec4 v_vf16 = ctx->vu0_vf[16];
    a4::vec4 v_vf17 = ctx->vu0_vf[17];
    a4::vec4 v_vacc = ctx->vu0_acc;
    if (!ckLeg([&] { ty::iosMulCurrentMatrixR(rdram, runtime, p4, v_vf1, v_vf2, v_vf3, v_vf4, v_vf14, v_vf15, v_vf16, v_vf17, v_vacc); })) return;
    R5900Context &t = ckTyCtx(ctx);
    t.vu0_vf[1] = v_vf1;
    t.vu0_vf[2] = v_vf2;
    t.vu0_vf[3] = v_vf3;
    t.vu0_vf[4] = v_vf4;
    t.vu0_vf[14] = v_vf14;
    t.vu0_vf[15] = v_vf15;
    t.vu0_vf[16] = v_vf16;
    t.vu0_vf[17] = v_vf17;
    t.vu0_acc = v_vacc;
    ckEnd(12, ctx, c, 0, v0, f0, 0x0u, 0x0u, false, vuDiffer(t, c));
}
void iosMulCurrentMatrixL(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t p4)
{
    if (!ckBegin(13, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    if (!ckLeg([&] { ref::iosMulCurrentMatrixL(rdram, &c, runtime); })) return;
    ckMid();
    uint64_t v0 = 0u;
    float f0 = 0.0f;
    a4::vec4 v_vf1 = ctx->vu0_vf[1];
    a4::vec4 v_vf2 = ctx->vu0_vf[2];
    a4::vec4 v_vf3 = ctx->vu0_vf[3];
    a4::vec4 v_vf4 = ctx->vu0_vf[4];
    a4::vec4 v_vf14 = ctx->vu0_vf[14];
    a4::vec4 v_vf15 = ctx->vu0_vf[15];
    a4::vec4 v_vf16 = ctx->vu0_vf[16];
    a4::vec4 v_vf17 = ctx->vu0_vf[17];
    a4::vec4 v_vacc = ctx->vu0_acc;
    if (!ckLeg([&] { ty::iosMulCurrentMatrixL(rdram, runtime, p4, v_vf1, v_vf2, v_vf3, v_vf4, v_vf14, v_vf15, v_vf16, v_vf17, v_vacc); })) return;
    R5900Context &t = ckTyCtx(ctx);
    t.vu0_vf[1] = v_vf1;
    t.vu0_vf[2] = v_vf2;
    t.vu0_vf[3] = v_vf3;
    t.vu0_vf[4] = v_vf4;
    t.vu0_vf[14] = v_vf14;
    t.vu0_vf[15] = v_vf15;
    t.vu0_vf[16] = v_vf16;
    t.vu0_vf[17] = v_vf17;
    t.vu0_acc = v_vacc;
    ckEnd(13, ctx, c, 0, v0, f0, 0x0u, 0x0u, false, vuDiffer(t, c));
}
void iosApplyCurrentMatrix(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t p4, uint32_t p5)
{
    if (!ckBegin(14, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    if (!ckLeg([&] { ref::iosApplyCurrentMatrix(rdram, &c, runtime); })) return;
    ckMid();
    uint64_t v0 = 0u;
    float f0 = 0.0f;
    a4::vec4 v_vf1 = ctx->vu0_vf[1];
    a4::vec4 v_vf2 = ctx->vu0_vf[2];
    a4::vec4 v_vf3 = ctx->vu0_vf[3];
    a4::vec4 v_vf4 = ctx->vu0_vf[4];
    a4::vec4 v_vf8 = ctx->vu0_vf[8];
    a4::vec4 v_vf10 = ctx->vu0_vf[10];
    a4::vec4 v_vacc = ctx->vu0_acc;
    if (!ckLeg([&] { ty::iosApplyCurrentMatrix(rdram, runtime, p4, p5, v_vf1, v_vf2, v_vf3, v_vf4, v_vf8, v_vf10, v_vacc); })) return;
    R5900Context &t = ckTyCtx(ctx);
    t.vu0_vf[8] = v_vf8;
    t.vu0_vf[10] = v_vf10;
    t.vu0_acc = v_vacc;
    ckEnd(14, ctx, c, 0, v0, f0, 0x0u, 0x0u, false, vuDiffer(t, c));
}
void iosApplyRotCurrentMatrix(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t p4, uint32_t p5)
{
    if (!ckBegin(15, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    if (!ckLeg([&] { ref::iosApplyRotCurrentMatrix(rdram, &c, runtime); })) return;
    ckMid();
    uint64_t v0 = 0u;
    float f0 = 0.0f;
    a4::vec4 v_vf0 = ctx->vu0_vf[0];
    a4::vec4 v_vf1 = ctx->vu0_vf[1];
    a4::vec4 v_vf2 = ctx->vu0_vf[2];
    a4::vec4 v_vf3 = ctx->vu0_vf[3];
    a4::vec4 v_vf8 = ctx->vu0_vf[8];
    a4::vec4 v_vf10 = ctx->vu0_vf[10];
    a4::vec4 v_vacc = ctx->vu0_acc;
    if (!ckLeg([&] { ty::iosApplyRotCurrentMatrix(rdram, runtime, p4, p5, v_vf0, v_vf1, v_vf2, v_vf3, v_vf8, v_vf10, v_vacc); })) return;
    R5900Context &t = ckTyCtx(ctx);
    t.vu0_vf[8] = v_vf8;
    t.vu0_vf[10] = v_vf10;
    t.vu0_acc = v_vacc;
    ckEnd(15, ctx, c, 0, v0, f0, 0x0u, 0x0u, false, vuDiffer(t, c));
}
void iosRotTransPersCurrentMatrix(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t p4, uint32_t p5)
{
    if (!ckBegin(16, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    if (!ckLeg([&] { ref::iosRotTransPersCurrentMatrix(rdram, &c, runtime); })) return;
    ckMid();
    uint64_t v0 = 0u;
    float f0 = 0.0f;
    a4::vec4 v_vf0 = ctx->vu0_vf[0];
    a4::vec4 v_vf1 = ctx->vu0_vf[1];
    a4::vec4 v_vf2 = ctx->vu0_vf[2];
    a4::vec4 v_vf3 = ctx->vu0_vf[3];
    a4::vec4 v_vf4 = ctx->vu0_vf[4];
    a4::vec4 v_vf8 = ctx->vu0_vf[8];
    a4::vec4 v_vf10 = ctx->vu0_vf[10];
    a4::vec4 v_vacc = ctx->vu0_acc;
    float v_vq = ctx->vu0_q;
    uint16_t v_vstat = ctx->vu0_status;
    if (!ckLeg([&] { ty::iosRotTransPersCurrentMatrix(rdram, runtime, p4, p5, v_vf0, v_vf1, v_vf2, v_vf3, v_vf4, v_vf8, v_vf10, v_vacc, v_vq, v_vstat); })) return;
    R5900Context &t = ckTyCtx(ctx);
    t.vu0_vf[8] = v_vf8;
    t.vu0_vf[10] = v_vf10;
    t.vu0_acc = v_vacc;
    t.vu0_q = v_vq;
    t.vu0_status = v_vstat;
    ckEnd(16, ctx, c, 0, v0, f0, 0x0u, 0x0u, false, vuDiffer(t, c));
}
void iosTransposeCurrentMatrix(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (!ckBegin(17, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    if (!ckLeg([&] { ref::iosTransposeCurrentMatrix(rdram, &c, runtime); })) return;
    ckMid();
    uint64_t v0 = 0u;
    float f0 = 0.0f;
    a4::vec4 v_vf0 = ctx->vu0_vf[0];
    a4::vec4 v_vf1 = ctx->vu0_vf[1];
    a4::vec4 v_vf2 = ctx->vu0_vf[2];
    a4::vec4 v_vf3 = ctx->vu0_vf[3];
    a4::vec4 v_vf4 = ctx->vu0_vf[4];
    a4::vec4 v_vf5 = ctx->vu0_vf[5];
    a4::vec4 v_vf14 = ctx->vu0_vf[14];
    a4::vec4 v_vf15 = ctx->vu0_vf[15];
    a4::vec4 v_vf16 = ctx->vu0_vf[16];
    a4::vec4 v_vf17 = ctx->vu0_vf[17];
    if (!ckLeg([&] { ty::iosTransposeCurrentMatrix(rdram, runtime, v_vf0, v_vf1, v_vf2, v_vf3, v_vf4, v_vf5, v_vf14, v_vf15, v_vf16, v_vf17); })) return;
    R5900Context &t = ckTyCtx(ctx);
    t.vu0_vf[1] = v_vf1;
    t.vu0_vf[2] = v_vf2;
    t.vu0_vf[3] = v_vf3;
    t.vu0_vf[4] = v_vf4;
    t.vu0_vf[5] = v_vf5;
    t.vu0_vf[14] = v_vf14;
    t.vu0_vf[15] = v_vf15;
    t.vu0_vf[16] = v_vf16;
    t.vu0_vf[17] = v_vf17;
    ckEnd(17, ctx, c, 0, v0, f0, 0x0u, 0x0u, false, vuDiffer(t, c));
}
void iosTransposeRotationCurrentMatrix(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (!ckBegin(18, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    if (!ckLeg([&] { ref::iosTransposeRotationCurrentMatrix(rdram, &c, runtime); })) return;
    ckMid();
    uint64_t v0 = 0u;
    float f0 = 0.0f;
    a4::vec4 v_vf0 = ctx->vu0_vf[0];
    a4::vec4 v_vf1 = ctx->vu0_vf[1];
    a4::vec4 v_vf2 = ctx->vu0_vf[2];
    a4::vec4 v_vf3 = ctx->vu0_vf[3];
    a4::vec4 v_vf5 = ctx->vu0_vf[5];
    a4::vec4 v_vf14 = ctx->vu0_vf[14];
    a4::vec4 v_vf15 = ctx->vu0_vf[15];
    a4::vec4 v_vf16 = ctx->vu0_vf[16];
    if (!ckLeg([&] { ty::iosTransposeRotationCurrentMatrix(rdram, runtime, v_vf0, v_vf1, v_vf2, v_vf3, v_vf5, v_vf14, v_vf15, v_vf16); })) return;
    R5900Context &t = ckTyCtx(ctx);
    t.vu0_vf[1] = v_vf1;
    t.vu0_vf[2] = v_vf2;
    t.vu0_vf[3] = v_vf3;
    t.vu0_vf[5] = v_vf5;
    t.vu0_vf[14] = v_vf14;
    t.vu0_vf[15] = v_vf15;
    t.vu0_vf[16] = v_vf16;
    ckEnd(18, ctx, c, 0, v0, f0, 0x0u, 0x0u, false, vuDiffer(t, c));
}
void iosInverseCurrentMatrix(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (!ckBegin(19, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    if (!ckLeg([&] { ref::iosInverseCurrentMatrix(rdram, &c, runtime); })) return;
    ckMid();
    uint64_t v0 = 0u;
    float f0 = 0.0f;
    a4::vec4 v_vf0 = ctx->vu0_vf[0];
    a4::vec4 v_vf1 = ctx->vu0_vf[1];
    a4::vec4 v_vf2 = ctx->vu0_vf[2];
    a4::vec4 v_vf3 = ctx->vu0_vf[3];
    a4::vec4 v_vf4 = ctx->vu0_vf[4];
    a4::vec4 v_vf5 = ctx->vu0_vf[5];
    a4::vec4 v_vf6 = ctx->vu0_vf[6];
    a4::vec4 v_vf14 = ctx->vu0_vf[14];
    a4::vec4 v_vf15 = ctx->vu0_vf[15];
    a4::vec4 v_vf16 = ctx->vu0_vf[16];
    a4::vec4 v_vf17 = ctx->vu0_vf[17];
    a4::vec4 v_vacc = ctx->vu0_acc;
    if (!ckLeg([&] { ty::iosInverseCurrentMatrix(rdram, runtime, v_vf0, v_vf1, v_vf2, v_vf3, v_vf4, v_vf5, v_vf6, v_vf14, v_vf15, v_vf16, v_vf17, v_vacc); })) return;
    R5900Context &t = ckTyCtx(ctx);
    t.vu0_vf[1] = v_vf1;
    t.vu0_vf[2] = v_vf2;
    t.vu0_vf[3] = v_vf3;
    t.vu0_vf[4] = v_vf4;
    t.vu0_vf[5] = v_vf5;
    t.vu0_vf[6] = v_vf6;
    t.vu0_vf[14] = v_vf14;
    t.vu0_vf[15] = v_vf15;
    t.vu0_vf[16] = v_vf16;
    t.vu0_vf[17] = v_vf17;
    t.vu0_acc = v_vacc;
    ckEnd(19, ctx, c, 0, v0, f0, 0x0u, 0x0u, false, vuDiffer(t, c));
}
void iosApplyRotMatrix(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t p4, uint32_t p5, uint32_t p6)
{
    if (!ckBegin(20, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    if (!ckLeg([&] { ref::iosApplyRotMatrix(rdram, &c, runtime); })) return;
    ckMid();
    uint64_t v0 = 0u;
    float f0 = 0.0f;
    a4::vec4 v_vf0 = ctx->vu0_vf[0];
    a4::vec4 v_vf8 = ctx->vu0_vf[8];
    a4::vec4 v_vf10 = ctx->vu0_vf[10];
    a4::vec4 v_vf14 = ctx->vu0_vf[14];
    a4::vec4 v_vf15 = ctx->vu0_vf[15];
    a4::vec4 v_vf16 = ctx->vu0_vf[16];
    a4::vec4 v_vacc = ctx->vu0_acc;
    if (!ckLeg([&] { ty::iosApplyRotMatrix(rdram, runtime, p4, p5, p6, v_vf0, v_vf8, v_vf10, v_vf14, v_vf15, v_vf16, v_vacc); })) return;
    R5900Context &t = ckTyCtx(ctx);
    t.vu0_vf[8] = v_vf8;
    t.vu0_vf[10] = v_vf10;
    t.vu0_vf[14] = v_vf14;
    t.vu0_vf[15] = v_vf15;
    t.vu0_vf[16] = v_vf16;
    t.vu0_acc = v_vacc;
    ckEnd(20, ctx, c, 0, v0, f0, 0x0u, 0x0u, false, vuDiffer(t, c));
}
void iosUnitMatrix(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t p4)
{
    if (!ckBegin(21, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    if (!ckLeg([&] { ref::iosUnitMatrix(rdram, &c, runtime); })) return;
    ckMid();
    uint64_t v0 = 0u;
    float f0 = 0.0f;
    a4::vec4 v_vf0 = ctx->vu0_vf[0];
    a4::vec4 v_vf14 = ctx->vu0_vf[14];
    a4::vec4 v_vf15 = ctx->vu0_vf[15];
    a4::vec4 v_vf16 = ctx->vu0_vf[16];
    a4::vec4 v_vf17 = ctx->vu0_vf[17];
    if (!ckLeg([&] { ty::iosUnitMatrix(rdram, runtime, p4, v_vf0, v_vf14, v_vf15, v_vf16, v_vf17); })) return;
    R5900Context &t = ckTyCtx(ctx);
    t.vu0_vf[14] = v_vf14;
    t.vu0_vf[15] = v_vf15;
    t.vu0_vf[16] = v_vf16;
    t.vu0_vf[17] = v_vf17;
    ckEnd(21, ctx, c, 0, v0, f0, 0x0u, 0x0u, false, vuDiffer(t, c));
}
void iosUnitRotation(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t p4)
{
    if (!ckBegin(22, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    if (!ckLeg([&] { ref::iosUnitRotation(rdram, &c, runtime); })) return;
    ckMid();
    uint64_t v0 = 0u;
    float f0 = 0.0f;
    a4::vec4 v_vf0 = ctx->vu0_vf[0];
    a4::vec4 v_vf14 = ctx->vu0_vf[14];
    a4::vec4 v_vf15 = ctx->vu0_vf[15];
    a4::vec4 v_vf16 = ctx->vu0_vf[16];
    a4::vec4 v_vf17 = ctx->vu0_vf[17];
    if (!ckLeg([&] { ty::iosUnitRotation(rdram, runtime, p4, v_vf0, v_vf14, v_vf15, v_vf16, v_vf17); })) return;
    R5900Context &t = ckTyCtx(ctx);
    t.vu0_vf[14] = v_vf14;
    t.vu0_vf[15] = v_vf15;
    t.vu0_vf[16] = v_vf16;
    t.vu0_vf[17] = v_vf17;
    ckEnd(22, ctx, c, 0, v0, f0, 0x0u, 0x0u, false, vuDiffer(t, c));
}
void iosScaleMatrixV(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t p4, uint32_t p5, uint32_t p6)
{
    if (!ckBegin(23, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    if (!ckLeg([&] { ref::iosScaleMatrixV(rdram, &c, runtime); })) return;
    ckMid();
    uint64_t v0 = 0u;
    float f0 = 0.0f;
    a4::vec4 v_vf8 = ctx->vu0_vf[8];
    a4::vec4 v_vf14 = ctx->vu0_vf[14];
    a4::vec4 v_vf15 = ctx->vu0_vf[15];
    a4::vec4 v_vf16 = ctx->vu0_vf[16];
    a4::vec4 v_vf17 = ctx->vu0_vf[17];
    if (!ckLeg([&] { ty::iosScaleMatrixV(rdram, runtime, p4, p5, p6, v_vf8, v_vf14, v_vf15, v_vf16, v_vf17); })) return;
    R5900Context &t = ckTyCtx(ctx);
    t.vu0_vf[8] = v_vf8;
    t.vu0_vf[14] = v_vf14;
    t.vu0_vf[15] = v_vf15;
    t.vu0_vf[16] = v_vf16;
    t.vu0_vf[17] = v_vf17;
    ckEnd(23, ctx, c, 0, v0, f0, 0x0u, 0x0u, false, vuDiffer(t, c));
}
void iosMakeNormalLightMatrix(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t p4, uint32_t p5, uint64_t p6, uint64_t p7, uint64_t p8)
{
    if (!ckBegin(24, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    if (!ckLeg([&] { ref::iosMakeNormalLightMatrix(rdram, &c, runtime); })) return;
    ckMid();
    uint64_t v0 = 0u;
    float f0 = 0.0f;
    a4::vec4 v_vf0 = ctx->vu0_vf[0];
    a4::vec4 v_vf5 = ctx->vu0_vf[5];
    a4::vec4 v_vf14 = ctx->vu0_vf[14];
    a4::vec4 v_vf15 = ctx->vu0_vf[15];
    a4::vec4 v_vf16 = ctx->vu0_vf[16];
    a4::vec4 v_vf17 = ctx->vu0_vf[17];
    a4::vec4 v_vf24 = ctx->vu0_vf[24];
    a4::vec4 v_vf25 = ctx->vu0_vf[25];
    a4::vec4 v_vf26 = ctx->vu0_vf[26];
    a4::vec4 v_vf27 = ctx->vu0_vf[27];
    float v_vq = ctx->vu0_q;
    uint16_t v_vstat = ctx->vu0_status;
    if (!ckLeg([&] { v0 = (ty::iosMakeNormalLightMatrix(rdram, runtime, p4, p5, p6, p7, p8, v_vf0, v_vf5, v_vf14, v_vf15, v_vf16, v_vf17, v_vf24, v_vf25, v_vf26, v_vf27, v_vq, v_vstat)); })) return;
    R5900Context &t = ckTyCtx(ctx);
    t.vu0_vf[5] = v_vf5;
    t.vu0_vf[14] = v_vf14;
    t.vu0_vf[15] = v_vf15;
    t.vu0_vf[16] = v_vf16;
    t.vu0_vf[17] = v_vf17;
    t.vu0_vf[24] = v_vf24;
    t.vu0_vf[25] = v_vf25;
    t.vu0_vf[26] = v_vf26;
    t.vu0_vf[27] = v_vf27;
    t.vu0_q = v_vq;
    t.vu0_status = v_vstat;
    ckEnd(24, ctx, c, 2, v0, f0, 0x408u, 0x0u, false, vuDiffer(t, c));
}
void iosScaleNormalLightMatrix(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t p4, uint32_t p5, uint32_t p6)
{
    if (!ckBegin(25, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    if (!ckLeg([&] { ref::iosScaleNormalLightMatrix(rdram, &c, runtime); })) return;
    ckMid();
    uint64_t v0 = 0u;
    float f0 = 0.0f;
    a4::vec4 v_vf24 = ctx->vu0_vf[24];
    a4::vec4 v_vf25 = ctx->vu0_vf[25];
    a4::vec4 v_vf26 = ctx->vu0_vf[26];
    a4::vec4 v_vf27 = ctx->vu0_vf[27];
    a4::vec4 v_vf28 = ctx->vu0_vf[28];
    if (!ckLeg([&] { ty::iosScaleNormalLightMatrix(rdram, runtime, p4, p5, p6, v_vf24, v_vf25, v_vf26, v_vf27, v_vf28); })) return;
    R5900Context &t = ckTyCtx(ctx);
    t.vu0_vf[24] = v_vf24;
    t.vu0_vf[25] = v_vf25;
    t.vu0_vf[26] = v_vf26;
    t.vu0_vf[27] = v_vf27;
    t.vu0_vf[28] = v_vf28;
    ckEnd(25, ctx, c, 0, v0, f0, 0x0u, 0x0u, false, vuDiffer(t, c));
}
void iosMakeLightColorMatrix(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t p4, uint32_t p5, uint64_t p6, uint64_t p7, uint64_t p8)
{
    if (!ckBegin(26, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    if (!ckLeg([&] { ref::iosMakeLightColorMatrix(rdram, &c, runtime); })) return;
    ckMid();
    uint64_t v0 = 0u;
    float f0 = 0.0f;
    uint64_t v_h6 = a4::hi64(ctx->r[6]);
    uint64_t v_h7 = a4::hi64(ctx->r[7]);
    uint64_t v_h8 = a4::hi64(ctx->r[8]);
    uint64_t v_h9 = a4::hi64(ctx->r[9]);
    if (!ckLeg([&] { v0 = (ty::iosMakeLightColorMatrix(rdram, runtime, p4, p5, p6, p7, p8, v_h6, v_h7, v_h8, v_h9)); })) return;
    R5900Context &t = ckTyCtx(ctx);
    t.r[6] = a4::mk128(v_h6, Ps2GprU64(&t, 6));
    t.r[7] = a4::mk128(v_h7, Ps2GprU64(&t, 7));
    t.r[8] = a4::mk128(v_h8, Ps2GprU64(&t, 8));
    t.r[9] = a4::mk128(v_h9, Ps2GprU64(&t, 9));
    ckEnd(26, ctx, c, 2, v0, f0, 0x7c8u, 0x0u, false, vuDiffer(t, c));
}
void iosTransMatrix(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t p4, uint32_t p5, uint64_t p6)
{
    if (!ckBegin(27, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    if (!ckLeg([&] { ref::iosTransMatrix(rdram, &c, runtime); })) return;
    ckMid();
    uint64_t v0 = 0u;
    float f0 = 0.0f;
    a4::vec4 v_vf8 = ctx->vu0_vf[8];
    a4::vec4 v_vf10 = ctx->vu0_vf[10];
    a4::vec4 v_vf14 = ctx->vu0_vf[14];
    a4::vec4 v_vf15 = ctx->vu0_vf[15];
    a4::vec4 v_vf16 = ctx->vu0_vf[16];
    a4::vec4 v_vf17 = ctx->vu0_vf[17];
    a4::vec4 v_vacc = ctx->vu0_acc;
    uint64_t v_h6 = a4::hi64(ctx->r[6]);
    uint64_t v_h7 = a4::hi64(ctx->r[7]);
    uint64_t v_h8 = a4::hi64(ctx->r[8]);
    uint64_t v_h9 = a4::hi64(ctx->r[9]);
    if (!ckLeg([&] { v0 = (ty::iosTransMatrix(rdram, runtime, p4, p5, p6, v_vf8, v_vf10, v_vf14, v_vf15, v_vf16, v_vf17, v_vacc, v_h6, v_h7, v_h8, v_h9)); })) return;
    R5900Context &t = ckTyCtx(ctx);
    t.vu0_vf[8] = v_vf8;
    t.vu0_vf[10] = v_vf10;
    t.vu0_vf[14] = v_vf14;
    t.vu0_vf[15] = v_vf15;
    t.vu0_vf[16] = v_vf16;
    t.vu0_vf[17] = v_vf17;
    t.vu0_acc = v_vacc;
    t.r[6] = a4::mk128(v_h6, Ps2GprU64(&t, 6));
    t.r[7] = a4::mk128(v_h7, Ps2GprU64(&t, 7));
    t.r[8] = a4::mk128(v_h8, Ps2GprU64(&t, 8));
    t.r[9] = a4::mk128(v_h9, Ps2GprU64(&t, 9));
    ckEnd(27, ctx, c, 2, v0, f0, 0x3d0u, 0x0u, false, vuDiffer(t, c));
}
void iosScaleMatrix(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t p4, float pf12, float pf13, float pf14)
{
    if (!ckBegin(28, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    if (!ckLeg([&] { ref::iosScaleMatrix(rdram, &c, runtime); })) return;
    ckMid();
    uint64_t v0 = 0u;
    float f0 = 0.0f;
    a4::vec4 v_vf0 = ctx->vu0_vf[0];
    a4::vec4 v_vf14 = ctx->vu0_vf[14];
    a4::vec4 v_vf15 = ctx->vu0_vf[15];
    a4::vec4 v_vf16 = ctx->vu0_vf[16];
    a4::vec4 v_vf17 = ctx->vu0_vf[17];
    if (!ckLeg([&] { ty::iosScaleMatrix(rdram, runtime, p4, pf12, pf13, pf14, v_vf0, v_vf14, v_vf15, v_vf16, v_vf17); })) return;
    R5900Context &t = ckTyCtx(ctx);
    t.vu0_vf[14] = v_vf14;
    t.vu0_vf[15] = v_vf15;
    t.vu0_vf[16] = v_vf16;
    t.vu0_vf[17] = v_vf17;
    ckEnd(28, ctx, c, 0, v0, f0, 0x0u, 0x0u, false, vuDiffer(t, c));
}
void iosGetYDirXYZFromMatrix(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t p4, uint32_t p5)
{
    if (!ckBegin(29, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    if (!ckLeg([&] { ref::iosGetYDirXYZFromMatrix(rdram, &c, runtime); })) return;
    ckMid();
    uint64_t v0 = 0u;
    float f0 = 0.0f;
    a4::vec4 v_vf4 = ctx->vu0_vf[4];
    if (!ckLeg([&] { ty::iosGetYDirXYZFromMatrix(rdram, runtime, p4, p5, v_vf4); })) return;
    R5900Context &t = ckTyCtx(ctx);
    t.vu0_vf[4] = v_vf4;
    ckEnd(29, ctx, c, 0, v0, f0, 0x0u, 0x0u, false, vuDiffer(t, c));
}
void iosGetXDirXYZFromMatrix(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t p4, uint32_t p5)
{
    if (!ckBegin(30, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    if (!ckLeg([&] { ref::iosGetXDirXYZFromMatrix(rdram, &c, runtime); })) return;
    ckMid();
    uint64_t v0 = 0u;
    float f0 = 0.0f;
    a4::vec4 v_vf4 = ctx->vu0_vf[4];
    if (!ckLeg([&] { ty::iosGetXDirXYZFromMatrix(rdram, runtime, p4, p5, v_vf4); })) return;
    R5900Context &t = ckTyCtx(ctx);
    t.vu0_vf[4] = v_vf4;
    ckEnd(30, ctx, c, 0, v0, f0, 0x0u, 0x0u, false, vuDiffer(t, c));
}
void iosRotTransCurrentMatrix(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t p4, uint32_t p5)
{
    if (!ckBegin(31, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    if (!ckLeg([&] { ref::iosRotTransCurrentMatrix(rdram, &c, runtime); })) return;
    ckMid();
    uint64_t v0 = 0u;
    float f0 = 0.0f;
    a4::vec4 v_vf0 = ctx->vu0_vf[0];
    a4::vec4 v_vf1 = ctx->vu0_vf[1];
    a4::vec4 v_vf2 = ctx->vu0_vf[2];
    a4::vec4 v_vf3 = ctx->vu0_vf[3];
    a4::vec4 v_vf4 = ctx->vu0_vf[4];
    a4::vec4 v_vf8 = ctx->vu0_vf[8];
    a4::vec4 v_vf10 = ctx->vu0_vf[10];
    a4::vec4 v_vf11 = ctx->vu0_vf[11];
    a4::vec4 v_vf12 = ctx->vu0_vf[12];
    a4::vec4 v_vf14 = ctx->vu0_vf[14];
    a4::vec4 v_vf15 = ctx->vu0_vf[15];
    a4::vec4 v_vf16 = ctx->vu0_vf[16];
    a4::vec4 v_vacc = ctx->vu0_acc;
    float v_vq = ctx->vu0_q;
    uint16_t v_vstat = ctx->vu0_status;
    uint64_t v_h7 = a4::hi64(ctx->r[7]);
    if (!ckLeg([&] { f0 = ty::iosRotTransCurrentMatrix(rdram, runtime, p4, p5, v_vf0, v_vf1, v_vf2, v_vf3, v_vf4, v_vf8, v_vf10, v_vf11, v_vf12, v_vf14, v_vf15, v_vf16, v_vacc, v_vq, v_vstat, v_h7); })) return;
    R5900Context &t = ckTyCtx(ctx);
    t.vu0_vf[8] = v_vf8;
    t.vu0_vf[10] = v_vf10;
    t.vu0_vf[11] = v_vf11;
    t.vu0_vf[12] = v_vf12;
    t.vu0_vf[14] = v_vf14;
    t.vu0_vf[15] = v_vf15;
    t.vu0_vf[16] = v_vf16;
    t.vu0_acc = v_vacc;
    t.vu0_q = v_vq;
    t.vu0_status = v_vstat;
    t.r[7] = a4::mk128(v_h7, Ps2GprU64(&t, 7));
    ckEnd(31, ctx, c, 3, v0, f0, 0x80u, 0x0u, false, vuDiffer(t, c));
}
void iosMvecZero(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t p4)
{
    if (!ckBegin(32, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    if (!ckLeg([&] { ref::iosMvecZero(rdram, &c, runtime); })) return;
    ckMid();
    uint64_t v0 = 0u;
    float f0 = 0.0f;
    uint64_t v_h2 = a4::hi64(ctx->r[2]);
    if (!ckLeg([&] { v0 = (ty::iosMvecZero(rdram, runtime, p4, v_h2)); })) return;
    R5900Context &t = ckTyCtx(ctx);
    ckEnd(32, ctx, c, 2, v0, f0, 0x0u, 0x0u, false, vuDiffer(t, c));
}
void MultiCurrentQuaternion(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t p4)
{
    if (!ckBegin(33, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    if (!ckLeg([&] { ref::MultiCurrentQuaternion(rdram, &c, runtime); })) return;
    ckMid();
    uint64_t v0 = 0u;
    float f0 = 0.0f;
    a4::vec4 v_vf11 = ctx->vu0_vf[11];
    a4::vec4 v_vf12 = ctx->vu0_vf[12];
    a4::vec4 v_vf13 = ctx->vu0_vf[13];
    a4::vec4 v_vf14 = ctx->vu0_vf[14];
    a4::vec4 v_vf15 = ctx->vu0_vf[15];
    a4::vec4 v_vf16 = ctx->vu0_vf[16];
    a4::vec4 v_vacc = ctx->vu0_acc;
    if (!ckLeg([&] { v0 = a1_sx64(ty::MultiCurrentQuaternion(rdram, runtime, p4, v_vf11, v_vf12, v_vf13, v_vf14, v_vf15, v_vf16, v_vacc)); })) return;
    R5900Context &t = ckTyCtx(ctx);
    t.vu0_vf[11] = v_vf11;
    t.vu0_vf[12] = v_vf12;
    t.vu0_vf[13] = v_vf13;
    t.vu0_vf[14] = v_vf14;
    t.vu0_vf[15] = v_vf15;
    t.vu0_vf[16] = v_vf16;
    t.vu0_acc = v_vacc;
    ckEnd(33, ctx, c, 1, v0, f0, 0xau, 0x0u, false, vuDiffer(t, c));
}
void MultiCurrentQuaternionL(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t p4)
{
    if (!ckBegin(34, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    if (!ckLeg([&] { ref::MultiCurrentQuaternionL(rdram, &c, runtime); })) return;
    ckMid();
    uint64_t v0 = 0u;
    float f0 = 0.0f;
    a4::vec4 v_vf11 = ctx->vu0_vf[11];
    a4::vec4 v_vf12 = ctx->vu0_vf[12];
    a4::vec4 v_vf13 = ctx->vu0_vf[13];
    a4::vec4 v_vf14 = ctx->vu0_vf[14];
    a4::vec4 v_vf15 = ctx->vu0_vf[15];
    a4::vec4 v_vf16 = ctx->vu0_vf[16];
    a4::vec4 v_vacc = ctx->vu0_acc;
    if (!ckLeg([&] { v0 = a1_sx64(ty::MultiCurrentQuaternionL(rdram, runtime, p4, v_vf11, v_vf12, v_vf13, v_vf14, v_vf15, v_vf16, v_vacc)); })) return;
    R5900Context &t = ckTyCtx(ctx);
    t.vu0_vf[11] = v_vf11;
    t.vu0_vf[12] = v_vf12;
    t.vu0_vf[13] = v_vf13;
    t.vu0_vf[14] = v_vf14;
    t.vu0_vf[15] = v_vf15;
    t.vu0_vf[16] = v_vf16;
    t.vu0_acc = v_vacc;
    ckEnd(34, ctx, c, 1, v0, f0, 0xau, 0x0u, false, vuDiffer(t, c));
}
void InvertCurrentQuaternion(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (!ckBegin(35, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    if (!ckLeg([&] { ref::InvertCurrentQuaternion(rdram, &c, runtime); })) return;
    ckMid();
    uint64_t v0 = 0u;
    float f0 = 0.0f;
    a4::vec4 v_vf0 = ctx->vu0_vf[0];
    a4::vec4 v_vf5 = ctx->vu0_vf[5];
    a4::vec4 v_vf11 = ctx->vu0_vf[11];
    if (!ckLeg([&] { v0 = a1_sx64(ty::InvertCurrentQuaternion(rdram, runtime, v_vf0, v_vf5, v_vf11)); })) return;
    R5900Context &t = ckTyCtx(ctx);
    t.vu0_vf[5] = v_vf5;
    t.vu0_vf[11] = v_vf11;
    ckEnd(35, ctx, c, 1, v0, f0, 0xau, 0x0u, false, vuDiffer(t, c));
}
void SetCurrentQuaternion(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t p4)
{
    if (!ckBegin(36, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    if (!ckLeg([&] { ref::SetCurrentQuaternion(rdram, &c, runtime); })) return;
    ckMid();
    uint64_t v0 = 0u;
    float f0 = 0.0f;
    uint64_t v_h5 = a4::hi64(ctx->r[5]);
    uint64_t xo[1] = {Ps2GprU64(ctx, 5)};
    if (!ckLeg([&] { v0 = a1_sx64(ty::SetCurrentQuaternion(rdram, runtime, p4, v_h5, xo)); })) return;
    R5900Context &t = ckTyCtx(ctx);
    t.r[5] = a4::mk128(v_h5, Ps2GprU64(&t, 5));
    ckEnd(36, ctx, c, 1, v0, f0, 0x2au, 0x0u, Ps2GprU64(&c, 5) != xo[0], vuDiffer(t, c));
}
void InitQuaternionDrive(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (!ckBegin(37, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    if (!ckLeg([&] { ref::InitQuaternionDrive(rdram, &c, runtime); })) return;
    ckMid();
    uint64_t v0 = 0u;
    float f0 = 0.0f;
    a4::vec4 v_vf0 = ctx->vu0_vf[0];
    if (!ckLeg([&] { v0 = a1_sx64(ty::InitQuaternionDrive(rdram, runtime, v_vf0)); })) return;
    R5900Context &t = ckTyCtx(ctx);
    ckEnd(37, ctx, c, 1, v0, f0, 0x8u, 0x0u, false, vuDiffer(t, c));
}
void DivQuaternion(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t p4, uint32_t p5, uint32_t p6)
{
    if (!ckBegin(38, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    if (!ckLeg([&] { ref::DivQuaternion(rdram, &c, runtime); })) return;
    ckMid();
    uint64_t v0 = 0u;
    float f0 = 0.0f;
    a4::vec4 v_vf0 = ctx->vu0_vf[0];
    a4::vec4 v_vf5 = ctx->vu0_vf[5];
    a4::vec4 v_vf11 = ctx->vu0_vf[11];
    a4::vec4 v_vf12 = ctx->vu0_vf[12];
    a4::vec4 v_vf13 = ctx->vu0_vf[13];
    a4::vec4 v_vf14 = ctx->vu0_vf[14];
    a4::vec4 v_vf15 = ctx->vu0_vf[15];
    a4::vec4 v_vf16 = ctx->vu0_vf[16];
    a4::vec4 v_vacc = ctx->vu0_acc;
    if (!ckLeg([&] { ty::DivQuaternion(rdram, runtime, p4, p5, p6, v_vf0, v_vf5, v_vf11, v_vf12, v_vf13, v_vf14, v_vf15, v_vf16, v_vacc); })) return;
    R5900Context &t = ckTyCtx(ctx);
    t.vu0_vf[5] = v_vf5;
    t.vu0_vf[11] = v_vf11;
    t.vu0_vf[12] = v_vf12;
    t.vu0_vf[13] = v_vf13;
    t.vu0_vf[14] = v_vf14;
    t.vu0_vf[15] = v_vf15;
    t.vu0_vf[16] = v_vf16;
    t.vu0_acc = v_vacc;
    ckEnd(38, ctx, c, 0, v0, f0, 0x0u, 0x0u, false, vuDiffer(t, c));
}
void SetQuaternionByCosineAxisRotateVWithNoRegularize(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t p4, uint32_t p5, float pf12)
{
    if (!ckBegin(39, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    if (!ckLeg([&] { ref::SetQuaternionByCosineAxisRotateVWithNoRegularize(rdram, &c, runtime); })) return;
    ckMid();
    uint64_t v0 = 0u;
    float f0 = 0.0f;
    a4::vec4 v_vf5 = ctx->vu0_vf[5];
    a4::vec4 v_vf6 = ctx->vu0_vf[6];
    uint64_t v_h2 = a4::hi64(ctx->r[2]);
    if (!ckLeg([&] { const a1::RetVF r = ty::SetQuaternionByCosineAxisRotateVWithNoRegularize(rdram, runtime, p4, p5, pf12, v_vf5, v_vf6, v_h2); v0 = r.v0; f0 = r.f0; })) return;
    R5900Context &t = ckTyCtx(ctx);
    t.vu0_vf[5] = v_vf5;
    t.vu0_vf[6] = v_vf6;
    ckEnd(39, ctx, c, 4, v0, f0, 0x2u, 0x6u, false, vuDiffer(t, c));
}
void GetYRotQuaternionFromDir(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t p4, uint32_t p5)
{
    if (!ckBegin(40, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    if (!ckLeg([&] { ref::GetYRotQuaternionFromDir(rdram, &c, runtime); })) return;
    ckMid();
    uint64_t v0 = 0u;
    float f0 = 0.0f;
    a4::vec4 v_vf5 = ctx->vu0_vf[5];
    a4::vec4 v_vf6 = ctx->vu0_vf[6];
    uint64_t v_h3 = a4::hi64(ctx->r[3]);
    if (!ckLeg([&] { const a1::RetVF r = ty::GetYRotQuaternionFromDir(rdram, runtime, p4, p5, v_vf5, v_vf6, v_h3); v0 = r.v0; f0 = r.f0; })) return;
    R5900Context &t = ckTyCtx(ctx);
    t.vu0_vf[5] = v_vf5;
    t.vu0_vf[6] = v_vf6;
    ckEnd(40, ctx, c, 4, v0, f0, 0xau, 0xeu, false, vuDiffer(t, c));
}
void iosFTOI15Vector(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t p4, uint32_t p5)
{
    if (!ckBegin(41, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    if (!ckLeg([&] { ref::iosFTOI15Vector(rdram, &c, runtime); })) return;
    ckMid();
    uint64_t v0 = 0u;
    float f0 = 0.0f;
    a4::vec4 v_vf5 = ctx->vu0_vf[5];
    a4::vec4 v_vf6 = ctx->vu0_vf[6];
    if (!ckLeg([&] { ty::iosFTOI15Vector(rdram, runtime, p4, p5, v_vf5, v_vf6); })) return;
    R5900Context &t = ckTyCtx(ctx);
    t.vu0_vf[5] = v_vf5;
    t.vu0_vf[6] = v_vf6;
    ckEnd(41, ctx, c, 0, v0, f0, 0x0u, 0x0u, false, vuDiffer(t, c));
}
void iosFTOI12Vector(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t p4, uint32_t p5)
{
    if (!ckBegin(42, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    if (!ckLeg([&] { ref::iosFTOI12Vector(rdram, &c, runtime); })) return;
    ckMid();
    uint64_t v0 = 0u;
    float f0 = 0.0f;
    a4::vec4 v_vf5 = ctx->vu0_vf[5];
    a4::vec4 v_vf6 = ctx->vu0_vf[6];
    if (!ckLeg([&] { ty::iosFTOI12Vector(rdram, runtime, p4, p5, v_vf5, v_vf6); })) return;
    R5900Context &t = ckTyCtx(ctx);
    t.vu0_vf[5] = v_vf5;
    t.vu0_vf[6] = v_vf6;
    ckEnd(42, ctx, c, 0, v0, f0, 0x0u, 0x0u, false, vuDiffer(t, c));
}
void iosFTOI0Vector(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t p4, uint32_t p5)
{
    if (!ckBegin(43, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    if (!ckLeg([&] { ref::iosFTOI0Vector(rdram, &c, runtime); })) return;
    ckMid();
    uint64_t v0 = 0u;
    float f0 = 0.0f;
    a4::vec4 v_vf5 = ctx->vu0_vf[5];
    a4::vec4 v_vf6 = ctx->vu0_vf[6];
    if (!ckLeg([&] { ty::iosFTOI0Vector(rdram, runtime, p4, p5, v_vf5, v_vf6); })) return;
    R5900Context &t = ckTyCtx(ctx);
    t.vu0_vf[5] = v_vf5;
    t.vu0_vf[6] = v_vf6;
    ckEnd(43, ctx, c, 0, v0, f0, 0x0u, 0x0u, false, vuDiffer(t, c));
}
void iosITOF15Vector(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t p4, uint32_t p5)
{
    if (!ckBegin(44, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    if (!ckLeg([&] { ref::iosITOF15Vector(rdram, &c, runtime); })) return;
    ckMid();
    uint64_t v0 = 0u;
    float f0 = 0.0f;
    a4::vec4 v_vf5 = ctx->vu0_vf[5];
    a4::vec4 v_vf6 = ctx->vu0_vf[6];
    if (!ckLeg([&] { ty::iosITOF15Vector(rdram, runtime, p4, p5, v_vf5, v_vf6); })) return;
    R5900Context &t = ckTyCtx(ctx);
    t.vu0_vf[5] = v_vf5;
    t.vu0_vf[6] = v_vf6;
    ckEnd(44, ctx, c, 0, v0, f0, 0x0u, 0x0u, false, vuDiffer(t, c));
}
void iosITOF12Vector(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t p4, uint32_t p5)
{
    if (!ckBegin(45, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    if (!ckLeg([&] { ref::iosITOF12Vector(rdram, &c, runtime); })) return;
    ckMid();
    uint64_t v0 = 0u;
    float f0 = 0.0f;
    a4::vec4 v_vf5 = ctx->vu0_vf[5];
    a4::vec4 v_vf6 = ctx->vu0_vf[6];
    if (!ckLeg([&] { ty::iosITOF12Vector(rdram, runtime, p4, p5, v_vf5, v_vf6); })) return;
    R5900Context &t = ckTyCtx(ctx);
    t.vu0_vf[5] = v_vf5;
    t.vu0_vf[6] = v_vf6;
    ckEnd(45, ctx, c, 0, v0, f0, 0x0u, 0x0u, false, vuDiffer(t, c));
}
void iosITOF4Vector(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t p4, uint32_t p5)
{
    if (!ckBegin(46, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    if (!ckLeg([&] { ref::iosITOF4Vector(rdram, &c, runtime); })) return;
    ckMid();
    uint64_t v0 = 0u;
    float f0 = 0.0f;
    a4::vec4 v_vf5 = ctx->vu0_vf[5];
    a4::vec4 v_vf6 = ctx->vu0_vf[6];
    if (!ckLeg([&] { ty::iosITOF4Vector(rdram, runtime, p4, p5, v_vf5, v_vf6); })) return;
    R5900Context &t = ckTyCtx(ctx);
    t.vu0_vf[5] = v_vf5;
    t.vu0_vf[6] = v_vf6;
    ckEnd(46, ctx, c, 0, v0, f0, 0x0u, 0x0u, false, vuDiffer(t, c));
}
void iosITOF0Vector(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t p4, uint32_t p5)
{
    if (!ckBegin(47, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    if (!ckLeg([&] { ref::iosITOF0Vector(rdram, &c, runtime); })) return;
    ckMid();
    uint64_t v0 = 0u;
    float f0 = 0.0f;
    a4::vec4 v_vf5 = ctx->vu0_vf[5];
    a4::vec4 v_vf6 = ctx->vu0_vf[6];
    if (!ckLeg([&] { ty::iosITOF0Vector(rdram, runtime, p4, p5, v_vf5, v_vf6); })) return;
    R5900Context &t = ckTyCtx(ctx);
    t.vu0_vf[5] = v_vf5;
    t.vu0_vf[6] = v_vf6;
    ckEnd(47, ctx, c, 0, v0, f0, 0x0u, 0x0u, false, vuDiffer(t, c));
}
void iosGetStabXZVector(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t p4, uint32_t p5)
{
    if (!ckBegin(48, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    if (!ckLeg([&] { ref::iosGetStabXZVector(rdram, &c, runtime); })) return;
    ckMid();
    uint64_t v0 = 0u;
    float f0 = 0.0f;
    a4::vec4 v_vf0 = ctx->vu0_vf[0];
    a4::vec4 v_vf5 = ctx->vu0_vf[5];
    a4::vec4 v_vf7 = ctx->vu0_vf[7];
    a4::vec4 v_vacc = ctx->vu0_acc;
    float v_vq = ctx->vu0_q;
    uint16_t v_vstat = ctx->vu0_status;
    uint64_t v_h2 = a4::hi64(ctx->r[2]);
    if (!ckLeg([&] { v0 = (ty::iosGetStabXZVector(rdram, runtime, p4, p5, v_vf0, v_vf5, v_vf7, v_vacc, v_vq, v_vstat, v_h2)); })) return;
    R5900Context &t = ckTyCtx(ctx);
    t.vu0_vf[5] = v_vf5;
    t.vu0_vf[7] = v_vf7;
    t.vu0_acc = v_vacc;
    t.vu0_q = v_vq;
    t.vu0_status = v_vstat;
    t.r[2] = a4::mk128(v_h2, Ps2GprU64(&t, 2));
    ckEnd(48, ctx, c, 2, v0, f0, 0x0u, 0x0u, false, vuDiffer(t, c));
}
void iosGetReflectVector(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t p4, uint32_t p5, uint32_t p6)
{
    if (!ckBegin(49, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    if (!ckLeg([&] { ref::iosGetReflectVector(rdram, &c, runtime); })) return;
    ckMid();
    uint64_t v0 = 0u;
    float f0 = 0.0f;
    a4::vec4 v_vf0 = ctx->vu0_vf[0];
    a4::vec4 v_vf4 = ctx->vu0_vf[4];
    a4::vec4 v_vf5 = ctx->vu0_vf[5];
    a4::vec4 v_vf6 = ctx->vu0_vf[6];
    a4::vec4 v_vf7 = ctx->vu0_vf[7];
    if (!ckLeg([&] { ty::iosGetReflectVector(rdram, runtime, p4, p5, p6, v_vf0, v_vf4, v_vf5, v_vf6, v_vf7); })) return;
    R5900Context &t = ckTyCtx(ctx);
    t.vu0_vf[4] = v_vf4;
    t.vu0_vf[5] = v_vf5;
    t.vu0_vf[6] = v_vf6;
    t.vu0_vf[7] = v_vf7;
    ckEnd(49, ctx, c, 0, v0, f0, 0x0u, 0x0u, false, vuDiffer(t, c));
}
void iosGetStabXYVector(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t p4, uint32_t p5)
{
    if (!ckBegin(50, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    if (!ckLeg([&] { ref::iosGetStabXYVector(rdram, &c, runtime); })) return;
    ckMid();
    uint64_t v0 = 0u;
    float f0 = 0.0f;
    a4::vec4 v_vf0 = ctx->vu0_vf[0];
    a4::vec4 v_vf5 = ctx->vu0_vf[5];
    a4::vec4 v_vf7 = ctx->vu0_vf[7];
    a4::vec4 v_vacc = ctx->vu0_acc;
    float v_vq = ctx->vu0_q;
    uint16_t v_vstat = ctx->vu0_status;
    uint64_t v_h2 = a4::hi64(ctx->r[2]);
    if (!ckLeg([&] { v0 = (ty::iosGetStabXYVector(rdram, runtime, p4, p5, v_vf0, v_vf5, v_vf7, v_vacc, v_vq, v_vstat, v_h2)); })) return;
    R5900Context &t = ckTyCtx(ctx);
    t.vu0_vf[5] = v_vf5;
    t.vu0_vf[7] = v_vf7;
    t.vu0_acc = v_vacc;
    t.vu0_q = v_vq;
    t.vu0_status = v_vstat;
    t.r[2] = a4::mk128(v_h2, Ps2GprU64(&t, 2));
    ckEnd(50, ctx, c, 2, v0, f0, 0x0u, 0x0u, false, vuDiffer(t, c));
}
void iosGetStabYZVector(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t p4, uint32_t p5)
{
    if (!ckBegin(51, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    if (!ckLeg([&] { ref::iosGetStabYZVector(rdram, &c, runtime); })) return;
    ckMid();
    uint64_t v0 = 0u;
    float f0 = 0.0f;
    a4::vec4 v_vf0 = ctx->vu0_vf[0];
    a4::vec4 v_vf5 = ctx->vu0_vf[5];
    a4::vec4 v_vf7 = ctx->vu0_vf[7];
    a4::vec4 v_vacc = ctx->vu0_acc;
    float v_vq = ctx->vu0_q;
    uint16_t v_vstat = ctx->vu0_status;
    uint64_t v_h2 = a4::hi64(ctx->r[2]);
    if (!ckLeg([&] { v0 = (ty::iosGetStabYZVector(rdram, runtime, p4, p5, v_vf0, v_vf5, v_vf7, v_vacc, v_vq, v_vstat, v_h2)); })) return;
    R5900Context &t = ckTyCtx(ctx);
    t.vu0_vf[5] = v_vf5;
    t.vu0_vf[7] = v_vf7;
    t.vu0_acc = v_vacc;
    t.vu0_q = v_vq;
    t.vu0_status = v_vstat;
    t.r[2] = a4::mk128(v_h2, Ps2GprU64(&t, 2));
    ckEnd(51, ctx, c, 2, v0, f0, 0x0u, 0x0u, false, vuDiffer(t, c));
}
void iosGetLengthPointToLine(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t p4, uint32_t p5, uint32_t p6, uint32_t p7)
{
    if (!ckBegin(52, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    if (!ckLeg([&] { ref::iosGetLengthPointToLine(rdram, &c, runtime); })) return;
    ckMid();
    uint64_t v0 = 0u;
    float f0 = 0.0f;
    a4::vec4 v_vf0 = ctx->vu0_vf[0];
    a4::vec4 v_vf5 = ctx->vu0_vf[5];
    a4::vec4 v_vf6 = ctx->vu0_vf[6];
    a4::vec4 v_vf7 = ctx->vu0_vf[7];
    a4::vec4 v_vacc = ctx->vu0_acc;
    float v_vq = ctx->vu0_q;
    uint16_t v_vstat = ctx->vu0_status;
    uint64_t v_h2 = a4::hi64(ctx->r[2]);
    if (!ckLeg([&] { const a1::RetVF r = ty::iosGetLengthPointToLine(rdram, runtime, p4, p5, p6, p7, v_vf0, v_vf5, v_vf6, v_vf7, v_vacc, v_vq, v_vstat, v_h2); v0 = r.v0; f0 = r.f0; })) return;
    R5900Context &t = ckTyCtx(ctx);
    t.vu0_vf[5] = v_vf5;
    t.vu0_vf[6] = v_vf6;
    t.vu0_vf[7] = v_vf7;
    t.vu0_acc = v_vacc;
    t.vu0_q = v_vq;
    t.vu0_status = v_vstat;
    t.r[2] = a4::mk128(v_h2, Ps2GprU64(&t, 2));
    ckEnd(52, ctx, c, 4, v0, f0, 0x0u, 0x6u, false, vuDiffer(t, c));
}
void iosGetInverseVectorXYZ(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t p4, uint32_t p5)
{
    if (!ckBegin(53, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    if (!ckLeg([&] { ref::iosGetInverseVectorXYZ(rdram, &c, runtime); })) return;
    ckMid();
    uint64_t v0 = 0u;
    float f0 = 0.0f;
    a4::vec4 v_vf5 = ctx->vu0_vf[5];
    a4::vec4 v_vf6 = ctx->vu0_vf[6];
    uint64_t v_h2 = a4::hi64(ctx->r[2]);
    if (!ckLeg([&] { const a1::RetVF r = ty::iosGetInverseVectorXYZ(rdram, runtime, p4, p5, v_vf5, v_vf6, v_h2); v0 = r.v0; f0 = r.f0; })) return;
    R5900Context &t = ckTyCtx(ctx);
    t.vu0_vf[5] = v_vf5;
    t.vu0_vf[6] = v_vf6;
    ckEnd(53, ctx, c, 4, v0, f0, 0x2u, 0x0u, false, vuDiffer(t, c));
}
void iosGetExceptAxisVector(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t p4, uint32_t p5, uint32_t p6)
{
    if (!ckBegin(54, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    if (!ckLeg([&] { ref::iosGetExceptAxisVector(rdram, &c, runtime); })) return;
    ckMid();
    uint64_t v0 = 0u;
    float f0 = 0.0f;
    a4::vec4 v_vf0 = ctx->vu0_vf[0];
    a4::vec4 v_vf5 = ctx->vu0_vf[5];
    a4::vec4 v_vf6 = ctx->vu0_vf[6];
    a4::vec4 v_vf7 = ctx->vu0_vf[7];
    a4::vec4 v_vacc = ctx->vu0_acc;
    uint64_t v_h2 = a4::hi64(ctx->r[2]);
    if (!ckLeg([&] { const a1::RetVF r = ty::iosGetExceptAxisVector(rdram, runtime, p4, p5, p6, v_vf0, v_vf5, v_vf6, v_vf7, v_vacc, v_h2); v0 = r.v0; f0 = r.f0; })) return;
    R5900Context &t = ckTyCtx(ctx);
    t.vu0_vf[5] = v_vf5;
    t.vu0_vf[6] = v_vf6;
    t.vu0_vf[7] = v_vf7;
    t.vu0_acc = v_vacc;
    t.r[2] = a4::mk128(v_h2, Ps2GprU64(&t, 2));
    ckEnd(54, ctx, c, 4, v0, f0, 0x0u, 0x0u, false, vuDiffer(t, c));
}
void iosCalcFColorToBrightness(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t p4)
{
    if (!ckBegin(55, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    if (!ckLeg([&] { ref::iosCalcFColorToBrightness(rdram, &c, runtime); })) return;
    ckMid();
    uint64_t v0 = 0u;
    float f0 = 0.0f;
    a4::vec4 v_vf5 = ctx->vu0_vf[5];
    a4::vec4 v_vf6 = ctx->vu0_vf[6];
    if (!ckLeg([&] { const a1::RetVF r = ty::iosCalcFColorToBrightness(rdram, runtime, p4, v_vf5, v_vf6); v0 = r.v0; f0 = r.f0; })) return;
    R5900Context &t = ckTyCtx(ctx);
    t.vu0_vf[5] = v_vf5;
    t.vu0_vf[6] = v_vf6;
    ckEnd(55, ctx, c, 4, v0, f0, 0x0u, 0x6u, false, vuDiffer(t, c));
}
void iosITOF15VectorXYZ(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t p4, uint32_t p5)
{
    if (!ckBegin(56, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    if (!ckLeg([&] { ref::iosITOF15VectorXYZ(rdram, &c, runtime); })) return;
    ckMid();
    uint64_t v0 = 0u;
    float f0 = 0.0f;
    a4::vec4 v_vf5 = ctx->vu0_vf[5];
    a4::vec4 v_vf6 = ctx->vu0_vf[6];
    if (!ckLeg([&] { ty::iosITOF15VectorXYZ(rdram, runtime, p4, p5, v_vf5, v_vf6); })) return;
    R5900Context &t = ckTyCtx(ctx);
    t.vu0_vf[5] = v_vf5;
    t.vu0_vf[6] = v_vf6;
    ckEnd(56, ctx, c, 0, v0, f0, 0x0u, 0x0u, false, vuDiffer(t, c));
}
void iosScaleColorTo255(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t p4, uint32_t p5)
{
    if (!ckBegin(57, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    if (!ckLeg([&] { ref::iosScaleColorTo255(rdram, &c, runtime); })) return;
    ckMid();
    uint64_t v0 = 0u;
    float f0 = 0.0f;
    a4::vec4 v_vf4 = ctx->vu0_vf[4];
    a4::vec4 v_vf5 = ctx->vu0_vf[5];
    a4::vec4 v_vf6 = ctx->vu0_vf[6];
    if (!ckLeg([&] { v0 = a1_sx64(ty::iosScaleColorTo255(rdram, runtime, p4, p5, v_vf4, v_vf5, v_vf6)); })) return;
    R5900Context &t = ckTyCtx(ctx);
    t.vu0_vf[4] = v_vf4;
    t.vu0_vf[5] = v_vf5;
    ckEnd(57, ctx, c, 1, v0, f0, 0x0u, 0x0u, false, vuDiffer(t, c));
}
void iosScaleColorToONE(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t p4, uint32_t p5)
{
    if (!ckBegin(58, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    if (!ckLeg([&] { ref::iosScaleColorToONE(rdram, &c, runtime); })) return;
    ckMid();
    uint64_t v0 = 0u;
    float f0 = 0.0f;
    a4::vec4 v_vf4 = ctx->vu0_vf[4];
    a4::vec4 v_vf5 = ctx->vu0_vf[5];
    a4::vec4 v_vf6 = ctx->vu0_vf[6];
    if (!ckLeg([&] { v0 = a1_sx64(ty::iosScaleColorToONE(rdram, runtime, p4, p5, v_vf4, v_vf5, v_vf6)); })) return;
    R5900Context &t = ckTyCtx(ctx);
    t.vu0_vf[4] = v_vf4;
    t.vu0_vf[5] = v_vf5;
    ckEnd(58, ctx, c, 1, v0, f0, 0x0u, 0x0u, false, vuDiffer(t, c));
}
void kernel_sub_001C84F0(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t p4, uint64_t p5, uint64_t p6, uint32_t p7)
{
    if (!ckBegin(59, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    if (!ckLeg([&] { ref::kernel_sub_001C84F0(rdram, &c, runtime); })) return;
    ckMid();
    uint64_t v0 = 0u;
    float f0 = 0.0f;
    uint64_t v_h5 = a4::hi64(ctx->r[5]);
    uint64_t v_h6 = a4::hi64(ctx->r[6]);
    uint64_t xo[1] = {Ps2GprU64(ctx, 5)};
    if (!ckLeg([&] { v0 = a1_sx64(ty::kernel_sub_001C84F0(rdram, runtime, p4, p5, p6, p7, v_h5, v_h6, xo)); })) return;
    R5900Context &t = ckTyCtx(ctx);
    t.r[5] = a4::mk128(v_h5, Ps2GprU64(&t, 5));
    ckEnd(59, ctx, c, 1, v0, f0, 0x20u, 0x0u, Ps2GprU64(&c, 5) != xo[0], vuDiffer(t, c));
}
void GAMEUTIL_get_xyz_axis_from_quaternion(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t p4, uint32_t p5, uint32_t p6, uint32_t p7)
{
    if (!ckBegin(60, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    if (!ckLeg([&] { ref::GAMEUTIL_get_xyz_axis_from_quaternion(rdram, &c, runtime); })) return;
    ckMid();
    uint64_t v0 = 0u;
    float f0 = 0.0f;
    a4::vec4 v_vf0 = ctx->vu0_vf[0];
    a4::vec4 v_vf4 = ctx->vu0_vf[4];
    a4::vec4 v_vf5 = ctx->vu0_vf[5];
    a4::vec4 v_vf6 = ctx->vu0_vf[6];
    a4::vec4 v_vf7 = ctx->vu0_vf[7];
    a4::vec4 v_vf8 = ctx->vu0_vf[8];
    if (!ckLeg([&] { ty::GAMEUTIL_get_xyz_axis_from_quaternion(rdram, runtime, p4, p5, p6, p7, v_vf0, v_vf4, v_vf5, v_vf6, v_vf7, v_vf8); })) return;
    R5900Context &t = ckTyCtx(ctx);
    t.vu0_vf[4] = v_vf4;
    t.vu0_vf[5] = v_vf5;
    t.vu0_vf[6] = v_vf6;
    t.vu0_vf[7] = v_vf7;
    t.vu0_vf[8] = v_vf8;
    ckEnd(60, ctx, c, 0, v0, f0, 0x0u, 0x0u, false, vuDiffer(t, c));
}
void GAMEUTIL_get_vector_velocity_by_diff_move3D(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t p4, uint32_t p5, uint32_t p6, uint32_t p7, float pf12)
{
    if (!ckBegin(61, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    if (!ckLeg([&] { ref::GAMEUTIL_get_vector_velocity_by_diff_move3D(rdram, &c, runtime); })) return;
    ckMid();
    uint64_t v0 = 0u;
    float f0 = 0.0f;
    a4::vec4 v_vf5 = ctx->vu0_vf[5];
    a4::vec4 v_vf6 = ctx->vu0_vf[6];
    uint64_t v_h2 = a4::hi64(ctx->r[2]);
    if (!ckLeg([&] { const a1::RetVF r = ty::GAMEUTIL_get_vector_velocity_by_diff_move3D(rdram, runtime, p4, p5, p6, p7, pf12, v_vf5, v_vf6, v_h2); v0 = r.v0; f0 = r.f0; })) return;
    R5900Context &t = ckTyCtx(ctx);
    t.vu0_vf[5] = v_vf5;
    t.vu0_vf[6] = v_vf6;
    t.r[2] = a4::mk128(v_h2, Ps2GprU64(&t, 2));
    ckEnd(61, ctx, c, 4, v0, f0, 0x2u, 0x0u, false, vuDiffer(t, c));
}
void GAMEUTIL_check_and_get_normalize_vector(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t p4, uint64_t p5, uint32_t p6)
{
    if (!ckBegin(62, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    if (!ckLeg([&] { ref::GAMEUTIL_check_and_get_normalize_vector(rdram, &c, runtime); })) return;
    ckMid();
    uint64_t v0 = 0u;
    float f0 = 0.0f;
    a4::vec4 v_vf0 = ctx->vu0_vf[0];
    a4::vec4 v_vf5 = ctx->vu0_vf[5];
    a4::vec4 v_vf6 = ctx->vu0_vf[6];
    a4::vec4 v_vf7 = ctx->vu0_vf[7];
    a4::vec4 v_vacc = ctx->vu0_acc;
    float v_vq = ctx->vu0_q;
    uint16_t v_vstat = ctx->vu0_status;
    uint64_t v_h2 = a4::hi64(ctx->r[2]);
    if (!ckLeg([&] { const a1::RetVF r = ty::GAMEUTIL_check_and_get_normalize_vector(rdram, runtime, p4, p5, p6, v_vf0, v_vf5, v_vf6, v_vf7, v_vacc, v_vq, v_vstat, v_h2); v0 = r.v0; f0 = r.f0; })) return;
    R5900Context &t = ckTyCtx(ctx);
    t.vu0_vf[5] = v_vf5;
    t.vu0_vf[6] = v_vf6;
    t.vu0_vf[7] = v_vf7;
    t.vu0_acc = v_vacc;
    t.vu0_q = v_vq;
    t.vu0_status = v_vstat;
    ckEnd(62, ctx, c, 4, v0, f0, 0x2u, 0x6u, false, vuDiffer(t, c));
}
void GAMEUTIL_get_divide_vector_value(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint64_t p4, uint64_t p5, uint64_t p6, uint32_t p7, uint32_t p8, uint32_t p9, uint32_t p10)
{
    if (!ckBegin(63, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    if (!ckLeg([&] { ref::GAMEUTIL_get_divide_vector_value(rdram, &c, runtime); })) return;
    ckMid();
    uint64_t v0 = 0u;
    float f0 = 0.0f;
    a4::vec4 v_vf0 = ctx->vu0_vf[0];
    a4::vec4 v_vf5 = ctx->vu0_vf[5];
    a4::vec4 v_vf6 = ctx->vu0_vf[6];
    a4::vec4 v_vf7 = ctx->vu0_vf[7];
    a4::vec4 v_vacc = ctx->vu0_acc;
    uint64_t v_h2 = a4::hi64(ctx->r[2]);
    uint64_t xo[2] = {(uint64_t)a1ck::ckBits(ctx->f[0]), Ps2GprU64(ctx, 2)};
    if (!ckLeg([&] { ty::GAMEUTIL_get_divide_vector_value(rdram, runtime, p4, p5, p6, p7, p8, p9, p10, v_vf0, v_vf5, v_vf6, v_vf7, v_vacc, v_h2, xo); })) return;
    R5900Context &t = ckTyCtx(ctx);
    t.vu0_vf[5] = v_vf5;
    t.vu0_vf[6] = v_vf6;
    t.vu0_vf[7] = v_vf7;
    t.vu0_acc = v_vacc;
    t.r[2] = a4::mk128(v_h2, Ps2GprU64(&t, 2));
    ckEnd(63, ctx, c, 0, v0, f0, 0x4u, 0x1u, a1ck::ckBits(c.f[0]) != (uint32_t)xo[0] || Ps2GprU64(&c, 2) != xo[1], vuDiffer(t, c));
}
void GAMEUTIL_divide_yaw_pitch_dir(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint64_t p4, uint64_t p5, uint32_t p6)
{
    if (!ckBegin(64, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    if (!ckLeg([&] { ref::GAMEUTIL_divide_yaw_pitch_dir(rdram, &c, runtime); })) return;
    ckMid();
    uint64_t v0 = 0u;
    float f0 = 0.0f;
    a4::vec4 v_vf0 = ctx->vu0_vf[0];
    a4::vec4 v_vf5 = ctx->vu0_vf[5];
    a4::vec4 v_vf6 = ctx->vu0_vf[6];
    a4::vec4 v_vf7 = ctx->vu0_vf[7];
    a4::vec4 v_vacc = ctx->vu0_acc;
    float v_vq = ctx->vu0_q;
    uint16_t v_vstat = ctx->vu0_status;
    uint64_t v_h2 = a4::hi64(ctx->r[2]);
    uint64_t v_h3 = a4::hi64(ctx->r[3]);
    if (!ckLeg([&] { const a1::RetVF r = ty::GAMEUTIL_divide_yaw_pitch_dir(rdram, runtime, p4, p5, p6, v_vf0, v_vf5, v_vf6, v_vf7, v_vacc, v_vq, v_vstat, v_h2, v_h3); v0 = r.v0; f0 = r.f0; })) return;
    R5900Context &t = ckTyCtx(ctx);
    t.vu0_vf[5] = v_vf5;
    t.vu0_vf[6] = v_vf6;
    t.vu0_vf[7] = v_vf7;
    t.vu0_acc = v_vacc;
    t.vu0_q = v_vq;
    t.vu0_status = v_vstat;
    t.r[2] = a4::mk128(v_h2, Ps2GprU64(&t, 2));
    t.r[3] = a4::mk128(v_h3, Ps2GprU64(&t, 3));
    ckEnd(64, ctx, c, 4, v0, f0, 0xau, 0x2u, false, vuDiffer(t, c));
}
void manager_sub_01082BD8(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, float pf12)
{
    if (!ckBegin(65, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    if (!ckLeg([&] { ref::manager_sub_01082BD8(rdram, &c, runtime); })) return;
    ckMid();
    uint64_t v0 = 0u;
    float f0 = 0.0f;
    uint64_t xo[1] = {Ps2GprU64(ctx, 2)};
    if (!ckLeg([&] { f0 = ty::manager_sub_01082BD8(rdram, runtime, pf12, xo); })) return;
    R5900Context &t = ckTyCtx(ctx);
    ckEnd(65, ctx, c, 3, v0, f0, 0x6u, 0xeu, Ps2GprU64(&c, 2) != xo[0], vuDiffer(t, c));
}
void GAMEUTIL_get_interp_value3D(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t p4, uint32_t p5, uint32_t p6, float pf12)
{
    if (!ckBegin(66, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    if (!ckLeg([&] { ref::GAMEUTIL_get_interp_value3D(rdram, &c, runtime); })) return;
    ckMid();
    uint64_t v0 = 0u;
    float f0 = 0.0f;
    a4::vec4 v_vf0 = ctx->vu0_vf[0];
    a4::vec4 v_vf5 = ctx->vu0_vf[5];
    a4::vec4 v_vf6 = ctx->vu0_vf[6];
    a4::vec4 v_vf7 = ctx->vu0_vf[7];
    a4::vec4 v_vf8 = ctx->vu0_vf[8];
    a4::vec4 v_vacc = ctx->vu0_acc;
    uint64_t v_h2 = a4::hi64(ctx->r[2]);
    uint64_t v_h8 = a4::hi64(ctx->r[8]);
    if (!ckLeg([&] { f0 = ty::GAMEUTIL_get_interp_value3D(rdram, runtime, p4, p5, p6, pf12, v_vf0, v_vf5, v_vf6, v_vf7, v_vf8, v_vacc, v_h2, v_h8); })) return;
    R5900Context &t = ckTyCtx(ctx);
    t.vu0_vf[5] = v_vf5;
    t.vu0_vf[6] = v_vf6;
    t.vu0_vf[7] = v_vf7;
    t.vu0_vf[8] = v_vf8;
    t.vu0_acc = v_vacc;
    t.r[2] = a4::mk128(v_h2, Ps2GprU64(&t, 2));
    ckEnd(66, ctx, c, 3, v0, f0, 0x106u, 0x2u, false, vuDiffer(t, c));
}
void getZ(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t p4, uint32_t p5, uint32_t p6)
{
    if (!ckBegin(67, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    if (!ckLeg([&] { ref::getZ(rdram, &c, runtime); })) return;
    ckMid();
    uint64_t v0 = 0u;
    float f0 = 0.0f;
    a4::vec4 v_vf0 = ctx->vu0_vf[0];
    a4::vec4 v_vf1 = ctx->vu0_vf[1];
    a4::vec4 v_vf24 = ctx->vu0_vf[24];
    a4::vec4 v_vf25 = ctx->vu0_vf[25];
    a4::vec4 v_vf26 = ctx->vu0_vf[26];
    a4::vec4 v_vf27 = ctx->vu0_vf[27];
    a4::vec4 v_vf28 = ctx->vu0_vf[28];
    a4::vec4 v_vacc = ctx->vu0_acc;
    if (!ckLeg([&] { ty::getZ(rdram, runtime, p4, p5, p6, v_vf0, v_vf1, v_vf24, v_vf25, v_vf26, v_vf27, v_vf28, v_vacc); })) return;
    R5900Context &t = ckTyCtx(ctx);
    t.vu0_vf[1] = v_vf1;
    t.vu0_vf[24] = v_vf24;
    t.vu0_vf[25] = v_vf25;
    t.vu0_vf[26] = v_vf26;
    t.vu0_vf[27] = v_vf27;
    t.vu0_vf[28] = v_vf28;
    t.vu0_acc = v_vacc;
    ckEnd(67, ctx, c, 0, v0, f0, 0x0u, 0x0u, false, vuDiffer(t, c));
}
void manager_sub_0108D358(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint64_t p4)
{
    if (!ckBegin(68, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    if (!ckLeg([&] { ref::manager_sub_0108D358(rdram, &c, runtime); })) return;
    ckMid();
    uint64_t v0 = 0u;
    float f0 = 0.0f;
    a4::vec4 v_vf0 = ctx->vu0_vf[0];
    a4::vec4 v_vf23 = ctx->vu0_vf[23];
    a4::vec4 v_vf24 = ctx->vu0_vf[24];
    a4::vec4 v_vf25 = ctx->vu0_vf[25];
    a4::vec4 v_vf26 = ctx->vu0_vf[26];
    a4::vec4 v_vf27 = ctx->vu0_vf[27];
    a4::vec4 v_vf28 = ctx->vu0_vf[28];
    a4::vec4 v_vf29 = ctx->vu0_vf[29];
    a4::vec4 v_vf30 = ctx->vu0_vf[30];
    a4::vec4 v_vacc = ctx->vu0_acc;
    uint64_t v_h3 = a4::hi64(ctx->r[3]);
    uint64_t v_h7 = a4::hi64(ctx->r[7]);
    uint64_t xo[8] = {(uint64_t)a1ck::ckBits(ctx->f[0]), Ps2GprU64(ctx, 5), Ps2GprU64(ctx, 6), Ps2GprU64(ctx, 7), Ps2GprU64(ctx, 8), Ps2GprU64(ctx, 9), Ps2GprU64(ctx, 10), Ps2GprU64(ctx, 11)};
    if (!ckLeg([&] { v0 = (ty::manager_sub_0108D358(rdram, runtime, p4, v_vf0, v_vf23, v_vf24, v_vf25, v_vf26, v_vf27, v_vf28, v_vf29, v_vf30, v_vacc, v_h3, v_h7, xo)); })) return;
    R5900Context &t = ckTyCtx(ctx);
    t.vu0_vf[23] = v_vf23;
    t.vu0_vf[24] = v_vf24;
    t.vu0_vf[25] = v_vf25;
    t.vu0_vf[26] = v_vf26;
    t.vu0_vf[27] = v_vf27;
    t.vu0_vf[28] = v_vf28;
    t.vu0_vf[29] = v_vf29;
    t.vu0_vf[30] = v_vf30;
    t.vu0_acc = v_vacc;
    t.r[3] = a4::mk128(v_h3, Ps2GprU64(&t, 3));
    ckEnd(68, ctx, c, 2, v0, f0, 0xffau, 0x3u, a1ck::ckBits(c.f[0]) != (uint32_t)xo[0] || Ps2GprU64(&c, 5) != xo[1] || Ps2GprU64(&c, 6) != xo[2] || Ps2GprU64(&c, 7) != xo[3] || Ps2GprU64(&c, 8) != xo[4] || Ps2GprU64(&c, 9) != xo[5] || Ps2GprU64(&c, 10) != xo[6] || Ps2GprU64(&c, 11) != xo[7], vuDiffer(t, c));
}
void iosgluGetViewCoordPosition(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t p4, uint32_t p5)
{
    if (!ckBegin(69, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    if (!ckLeg([&] { ref::iosgluGetViewCoordPosition(rdram, &c, runtime); })) return;
    ckMid();
    uint64_t v0 = 0u;
    float f0 = 0.0f;
    a4::vec4 v_vf0 = ctx->vu0_vf[0];
    a4::vec4 v_vf10 = ctx->vu0_vf[10];
    a4::vec4 v_vf11 = ctx->vu0_vf[11];
    a4::vec4 v_vf12 = ctx->vu0_vf[12];
    a4::vec4 v_vf13 = ctx->vu0_vf[13];
    a4::vec4 v_vf14 = ctx->vu0_vf[14];
    a4::vec4 v_vf15 = ctx->vu0_vf[15];
    a4::vec4 v_vf16 = ctx->vu0_vf[16];
    a4::vec4 v_vf17 = ctx->vu0_vf[17];
    a4::vec4 v_vf18 = ctx->vu0_vf[18];
    a4::vec4 v_vf19 = ctx->vu0_vf[19];
    a4::vec4 v_vf20 = ctx->vu0_vf[20];
    a4::vec4 v_vacc = ctx->vu0_acc;
    float v_vq = ctx->vu0_q;
    uint16_t v_vstat = ctx->vu0_status;
    uint64_t xo[1] = {Ps2GprU64(ctx, 6)};
    if (!ckLeg([&] { const a1::RetVF r = ty::iosgluGetViewCoordPosition(rdram, runtime, p4, p5, v_vf0, v_vf10, v_vf11, v_vf12, v_vf13, v_vf14, v_vf15, v_vf16, v_vf17, v_vf18, v_vf19, v_vf20, v_vacc, v_vq, v_vstat, xo); v0 = r.v0; f0 = r.f0; })) return;
    R5900Context &t = ckTyCtx(ctx);
    t.vu0_vf[10] = v_vf10;
    t.vu0_vf[11] = v_vf11;
    t.vu0_vf[12] = v_vf12;
    t.vu0_vf[13] = v_vf13;
    t.vu0_vf[14] = v_vf14;
    t.vu0_vf[15] = v_vf15;
    t.vu0_vf[16] = v_vf16;
    t.vu0_vf[17] = v_vf17;
    t.vu0_vf[18] = v_vf18;
    t.vu0_vf[19] = v_vf19;
    t.vu0_vf[20] = v_vf20;
    t.vu0_acc = v_vacc;
    t.vu0_q = v_vq;
    t.vu0_status = v_vstat;
    ckEnd(69, ctx, c, 4, v0, f0, 0x4au, 0x6u, Ps2GprU64(&c, 6) != xo[0], vuDiffer(t, c));
}
void manager_sub_0108F858(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t p4, uint32_t p5, uint32_t p6)
{
    if (!ckBegin(70, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    if (!ckLeg([&] { ref::manager_sub_0108F858(rdram, &c, runtime); })) return;
    ckMid();
    uint64_t v0 = 0u;
    float f0 = 0.0f;
    uint64_t v_h8 = a4::hi64(ctx->r[8]);
    uint64_t xo[5] = {Ps2GprU64(ctx, 5), Ps2GprU64(ctx, 6), Ps2GprU64(ctx, 7), Ps2GprU64(ctx, 8), Ps2GprU64(ctx, 9)};
    if (!ckLeg([&] { v0 = a1_sx64(ty::manager_sub_0108F858(rdram, runtime, p4, p5, p6, v_h8, xo)); })) return;
    R5900Context &t = ckTyCtx(ctx);
    t.r[8] = a4::mk128(v_h8, Ps2GprU64(&t, 8));
    ckEnd(70, ctx, c, 1, v0, f0, 0x3e8u, 0x0u, Ps2GprU64(&c, 5) != xo[0] || Ps2GprU64(&c, 6) != xo[1] || Ps2GprU64(&c, 7) != xo[2] || Ps2GprU64(&c, 8) != xo[3] || Ps2GprU64(&c, 9) != xo[4], vuDiffer(t, c));
}
void manager_sub_010AD048(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint64_t p4, uint32_t p5, uint32_t p6)
{
    if (!ckBegin(71, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    if (!ckLeg([&] { ref::manager_sub_010AD048(rdram, &c, runtime); })) return;
    ckMid();
    uint64_t v0 = 0u;
    float f0 = 0.0f;
    uint64_t v_h2 = a4::hi64(ctx->r[2]);
    uint64_t xo[1] = {Ps2GprU64(ctx, 7)};
    if (!ckLeg([&] { const a1::RetVF r = ty::manager_sub_010AD048(rdram, runtime, p4, p5, p6, v_h2, xo); v0 = r.v0; f0 = r.f0; })) return;
    R5900Context &t = ckTyCtx(ctx);
    t.r[2] = a4::mk128(v_h2, Ps2GprU64(&t, 2));
    ckEnd(71, ctx, c, 4, v0, f0, 0xbau, 0xeu, Ps2GprU64(&c, 7) != xo[0], vuDiffer(t, c));
}
void manager_sub_010BA150(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t p4, uint32_t p5, uint32_t p6, uint32_t p7)
{
    if (!ckBegin(72, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    if (!ckLeg([&] { ref::manager_sub_010BA150(rdram, &c, runtime); })) return;
    ckMid();
    uint64_t v0 = 0u;
    float f0 = 0.0f;
    uint64_t v_h2 = a4::hi64(ctx->r[2]);
    uint64_t xo[5] = {(uint64_t)a1ck::ckBits(ctx->f[1]), (uint64_t)a1ck::ckBits(ctx->f[2]), (uint64_t)a1ck::ckBits(ctx->f[3]), (uint64_t)a1ck::ckBits(ctx->f[4]), Ps2GprU64(ctx, 7)};
    if (!ckLeg([&] { v0 = (ty::manager_sub_010BA150(rdram, runtime, p4, p5, p6, p7, v_h2, xo)); })) return;
    R5900Context &t = ckTyCtx(ctx);
    t.r[2] = a4::mk128(v_h2, Ps2GprU64(&t, 2));
    ckEnd(72, ctx, c, 2, v0, f0, 0xbau, 0x1fu, a1ck::ckBits(c.f[1]) != (uint32_t)xo[0] || a1ck::ckBits(c.f[2]) != (uint32_t)xo[1] || a1ck::ckBits(c.f[3]) != (uint32_t)xo[2] || a1ck::ckBits(c.f[4]) != (uint32_t)xo[3] || Ps2GprU64(&c, 7) != xo[4], vuDiffer(t, c));
}
void initGazeIKWork(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t p6)
{
    if (!ckBegin(73, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    if (!ckLeg([&] { ref::initGazeIKWork(rdram, &c, runtime); })) return;
    ckMid();
    uint64_t v0 = 0u;
    float f0 = 0.0f;
    a4::vec4 v_vf0 = ctx->vu0_vf[0];
    if (!ckLeg([&] { const a1::RetVF r = ty::initGazeIKWork(rdram, runtime, p6, v_vf0); v0 = r.v0; f0 = r.f0; })) return;
    R5900Context &t = ckTyCtx(ctx);
    ckEnd(73, ctx, c, 4, v0, f0, 0xau, 0x2u, false, vuDiffer(t, c));
}
void GetCharacterClassControlFrontDirection(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t p4, uint32_t p5)
{
    if (!ckBegin(74, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    if (!ckLeg([&] { ref::GetCharacterClassControlFrontDirection(rdram, &c, runtime); })) return;
    ckMid();
    uint64_t v0 = 0u;
    float f0 = 0.0f;
    a4::vec4 v_vf0 = ctx->vu0_vf[0];
    a4::vec4 v_vf4 = ctx->vu0_vf[4];
    a4::vec4 v_vf5 = ctx->vu0_vf[5];
    a4::vec4 v_vf6 = ctx->vu0_vf[6];
    a4::vec4 v_vf7 = ctx->vu0_vf[7];
    a4::vec4 v_vf8 = ctx->vu0_vf[8];
    uint64_t xo[1] = {Ps2GprU64(ctx, 5)};
    if (!ckLeg([&] { ty::GetCharacterClassControlFrontDirection(rdram, runtime, p4, p5, v_vf0, v_vf4, v_vf5, v_vf6, v_vf7, v_vf8, xo); })) return;
    R5900Context &t = ckTyCtx(ctx);
    t.vu0_vf[4] = v_vf4;
    t.vu0_vf[5] = v_vf5;
    t.vu0_vf[6] = v_vf6;
    t.vu0_vf[7] = v_vf7;
    t.vu0_vf[8] = v_vf8;
    ckEnd(74, ctx, c, 0, v0, f0, 0x20u, 0x0u, Ps2GprU64(&c, 5) != xo[0], vuDiffer(t, c));
}
void GetCharacterClassOptionNodeLocalQuaternionPos(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t p4, uint32_t p5, uint32_t p6, uint32_t p7)
{
    if (!ckBegin(75, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    if (!ckLeg([&] { ref::GetCharacterClassOptionNodeLocalQuaternionPos(rdram, &c, runtime); })) return;
    ckMid();
    uint64_t v0 = 0u;
    float f0 = 0.0f;
    uint64_t v_h3 = a4::hi64(ctx->r[3]);
    uint64_t v_h8 = a4::hi64(ctx->r[8]);
    uint64_t xo[4] = {Ps2GprU64(ctx, 5), Ps2GprU64(ctx, 7), Ps2GprU64(ctx, 8), Ps2GprU64(ctx, 9)};
    if (!ckLeg([&] { v0 = a1_sx64(ty::GetCharacterClassOptionNodeLocalQuaternionPos(rdram, runtime, p4, p5, p6, p7, v_h3, v_h8, xo)); })) return;
    R5900Context &t = ckTyCtx(ctx);
    t.r[3] = a4::mk128(v_h3, Ps2GprU64(&t, 3));
    t.r[8] = a4::mk128(v_h8, Ps2GprU64(&t, 8));
    ckEnd(75, ctx, c, 1, v0, f0, 0x3a8u, 0x0u, Ps2GprU64(&c, 5) != xo[0] || Ps2GprU64(&c, 7) != xo[1] || Ps2GprU64(&c, 8) != xo[2] || Ps2GprU64(&c, 9) != xo[3], vuDiffer(t, c));
}
void ResetCharacterClassFallDownInfo(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t p4)
{
    if (!ckBegin(76, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    if (!ckLeg([&] { ref::ResetCharacterClassFallDownInfo(rdram, &c, runtime); })) return;
    ckMid();
    uint64_t v0 = 0u;
    float f0 = 0.0f;
    uint64_t v_h2 = a4::hi64(ctx->r[2]);
    if (!ckLeg([&] { v0 = (ty::ResetCharacterClassFallDownInfo(rdram, runtime, p4, v_h2)); })) return;
    R5900Context &t = ckTyCtx(ctx);
    t.r[2] = a4::mk128(v_h2, Ps2GprU64(&t, 2));
    ckEnd(76, ctx, c, 2, v0, f0, 0x8u, 0x0u, false, vuDiffer(t, c));
}
void ResetCharacterClassSlideStatus(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t p4)
{
    if (!ckBegin(77, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    if (!ckLeg([&] { ref::ResetCharacterClassSlideStatus(rdram, &c, runtime); })) return;
    ckMid();
    uint64_t v0 = 0u;
    float f0 = 0.0f;
    uint64_t v_h3 = a4::hi64(ctx->r[3]);
    if (!ckLeg([&] { v0 = a1_sx64(ty::ResetCharacterClassSlideStatus(rdram, runtime, p4, v_h3)); })) return;
    R5900Context &t = ckTyCtx(ctx);
    t.r[3] = a4::mk128(v_h3, Ps2GprU64(&t, 3));
    ckEnd(77, ctx, c, 1, v0, f0, 0x8u, 0x0u, false, vuDiffer(t, c));
}
void ResetCharacterClassMotionInterpRootOfs(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t p4)
{
    if (!ckBegin(78, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    if (!ckLeg([&] { ref::ResetCharacterClassMotionInterpRootOfs(rdram, &c, runtime); })) return;
    ckMid();
    uint64_t v0 = 0u;
    float f0 = 0.0f;
    uint64_t v_h2 = a4::hi64(ctx->r[2]);
    uint64_t xo[2] = {(uint64_t)a1ck::ckBits(ctx->f[0]), Ps2GprU64(ctx, 5)};
    if (!ckLeg([&] { v0 = (ty::ResetCharacterClassMotionInterpRootOfs(rdram, runtime, p4, v_h2, xo)); })) return;
    R5900Context &t = ckTyCtx(ctx);
    t.r[2] = a4::mk128(v_h2, Ps2GprU64(&t, 2));
    ckEnd(78, ctx, c, 2, v0, f0, 0x28u, 0x1u, a1ck::ckBits(c.f[0]) != (uint32_t)xo[0] || Ps2GprU64(&c, 5) != xo[1], vuDiffer(t, c));
}
void manager_sub_010C6D30(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t p4)
{
    if (!ckBegin(79, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    if (!ckLeg([&] { ref::manager_sub_010C6D30(rdram, &c, runtime); })) return;
    ckMid();
    uint64_t v0 = 0u;
    float f0 = 0.0f;
    a4::vec4 v_vf0 = ctx->vu0_vf[0];
    a4::vec4 v_vf7 = ctx->vu0_vf[7];
    a4::vec4 v_vacc = ctx->vu0_acc;
    float v_vq = ctx->vu0_q;
    uint16_t v_vstat = ctx->vu0_status;
    if (!ckLeg([&] { const a1::RetVF r = ty::manager_sub_010C6D30(rdram, runtime, p4, v_vf0, v_vf7, v_vacc, v_vq, v_vstat); v0 = r.v0; f0 = r.f0; })) return;
    R5900Context &t = ckTyCtx(ctx);
    t.vu0_vf[7] = v_vf7;
    t.vu0_acc = v_vacc;
    t.vu0_q = v_vq;
    t.vu0_status = v_vstat;
    ckEnd(79, ctx, c, 4, v0, f0, 0x0u, 0x3eu, false, vuDiffer(t, c));
}
void RequestCharacterClassMotionDirection(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t p4, uint32_t p5)
{
    if (!ckBegin(80, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    if (!ckLeg([&] { ref::RequestCharacterClassMotionDirection(rdram, &c, runtime); })) return;
    ckMid();
    uint64_t v0 = 0u;
    float f0 = 0.0f;
    uint64_t v_h2 = a4::hi64(ctx->r[2]);
    if (!ckLeg([&] { v0 = (ty::RequestCharacterClassMotionDirection(rdram, runtime, p4, p5, v_h2)); })) return;
    R5900Context &t = ckTyCtx(ctx);
    t.r[2] = a4::mk128(v_h2, Ps2GprU64(&t, 2));
    ckEnd(80, ctx, c, 2, v0, f0, 0x8u, 0x0u, false, vuDiffer(t, c));
}
void RequestCharacterClassRootOffset(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t p4, uint32_t p5)
{
    if (!ckBegin(81, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    if (!ckLeg([&] { ref::RequestCharacterClassRootOffset(rdram, &c, runtime); })) return;
    ckMid();
    uint64_t v0 = 0u;
    float f0 = 0.0f;
    uint64_t v_h2 = a4::hi64(ctx->r[2]);
    if (!ckLeg([&] { v0 = (ty::RequestCharacterClassRootOffset(rdram, runtime, p4, p5, v_h2)); })) return;
    R5900Context &t = ckTyCtx(ctx);
    t.r[2] = a4::mk128(v_h2, Ps2GprU64(&t, 2));
    ckEnd(81, ctx, c, 2, v0, f0, 0x0u, 0x0u, false, vuDiffer(t, c));
}
void charrootdyn_getInterpVector(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t p4, uint32_t p5, uint32_t p6, float pf12)
{
    if (!ckBegin(82, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    if (!ckLeg([&] { ref::charrootdyn_getInterpVector(rdram, &c, runtime); })) return;
    ckMid();
    uint64_t v0 = 0u;
    float f0 = 0.0f;
    a4::vec4 v_vf0 = ctx->vu0_vf[0];
    a4::vec4 v_vf5 = ctx->vu0_vf[5];
    a4::vec4 v_vf6 = ctx->vu0_vf[6];
    a4::vec4 v_vf7 = ctx->vu0_vf[7];
    a4::vec4 v_vf8 = ctx->vu0_vf[8];
    a4::vec4 v_vf9 = ctx->vu0_vf[9];
    a4::vec4 v_vacc = ctx->vu0_acc;
    uint64_t v_h2 = a4::hi64(ctx->r[2]);
    uint64_t v_h8 = a4::hi64(ctx->r[8]);
    uint64_t xo[2] = {Ps2GprU64(ctx, 2), Ps2GprU64(ctx, 8)};
    if (!ckLeg([&] { f0 = ty::charrootdyn_getInterpVector(rdram, runtime, p4, p5, p6, pf12, v_vf0, v_vf5, v_vf6, v_vf7, v_vf8, v_vf9, v_vacc, v_h2, v_h8, xo); })) return;
    R5900Context &t = ckTyCtx(ctx);
    t.vu0_vf[5] = v_vf5;
    t.vu0_vf[6] = v_vf6;
    t.vu0_vf[7] = v_vf7;
    t.vu0_vf[8] = v_vf8;
    t.vu0_vf[9] = v_vf9;
    t.vu0_acc = v_vacc;
    t.r[2] = a4::mk128(v_h2, Ps2GprU64(&t, 2));
    ckEnd(82, ctx, c, 3, v0, f0, 0x106u, 0x0u, Ps2GprU64(&c, 2) != xo[0] || Ps2GprU64(&c, 8) != xo[1], vuDiffer(t, c));
}
void charrootdyn_getInterpPoint(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t p4, uint32_t p5, uint32_t p6, float pf12)
{
    if (!ckBegin(83, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    if (!ckLeg([&] { ref::charrootdyn_getInterpPoint(rdram, &c, runtime); })) return;
    ckMid();
    uint64_t v0 = 0u;
    float f0 = 0.0f;
    a4::vec4 v_vf0 = ctx->vu0_vf[0];
    a4::vec4 v_vf5 = ctx->vu0_vf[5];
    a4::vec4 v_vf6 = ctx->vu0_vf[6];
    a4::vec4 v_vf7 = ctx->vu0_vf[7];
    a4::vec4 v_vf8 = ctx->vu0_vf[8];
    a4::vec4 v_vf9 = ctx->vu0_vf[9];
    a4::vec4 v_vacc = ctx->vu0_acc;
    uint64_t v_h2 = a4::hi64(ctx->r[2]);
    uint64_t v_h8 = a4::hi64(ctx->r[8]);
    uint64_t xo[1] = {Ps2GprU64(ctx, 8)};
    if (!ckLeg([&] { f0 = ty::charrootdyn_getInterpPoint(rdram, runtime, p4, p5, p6, pf12, v_vf0, v_vf5, v_vf6, v_vf7, v_vf8, v_vf9, v_vacc, v_h2, v_h8, xo); })) return;
    R5900Context &t = ckTyCtx(ctx);
    t.vu0_vf[5] = v_vf5;
    t.vu0_vf[6] = v_vf6;
    t.vu0_vf[7] = v_vf7;
    t.vu0_vf[8] = v_vf8;
    t.vu0_vf[9] = v_vf9;
    t.vu0_acc = v_vacc;
    t.r[2] = a4::mk128(v_h2, Ps2GprU64(&t, 2));
    ckEnd(83, ctx, c, 3, v0, f0, 0x106u, 0x0u, Ps2GprU64(&c, 8) != xo[0], vuDiffer(t, c));
}
void charrootdyn_getvelocity(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t p4, uint32_t p5, uint32_t p6, float pf12)
{
    if (!ckBegin(84, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    if (!ckLeg([&] { ref::charrootdyn_getvelocity(rdram, &c, runtime); })) return;
    ckMid();
    uint64_t v0 = 0u;
    float f0 = 0.0f;
    a4::vec4 v_vf5 = ctx->vu0_vf[5];
    a4::vec4 v_vf6 = ctx->vu0_vf[6];
    a4::vec4 v_vf7 = ctx->vu0_vf[7];
    uint64_t v_h2 = a4::hi64(ctx->r[2]);
    if (!ckLeg([&] { v0 = a1_sx64(ty::charrootdyn_getvelocity(rdram, runtime, p4, p5, p6, pf12, v_vf5, v_vf6, v_vf7, v_h2)); })) return;
    R5900Context &t = ckTyCtx(ctx);
    t.vu0_vf[5] = v_vf5;
    t.vu0_vf[6] = v_vf6;
    t.vu0_vf[7] = v_vf7;
    ckEnd(84, ctx, c, 1, v0, f0, 0x0u, 0x0u, false, vuDiffer(t, c));
}
void SetDemoMotionOrientInfo(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint64_t p4, uint64_t p5, uint32_t p6)
{
    if (!ckBegin(85, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    if (!ckLeg([&] { ref::SetDemoMotionOrientInfo(rdram, &c, runtime); })) return;
    ckMid();
    uint64_t v0 = 0u;
    float f0 = 0.0f;
    uint64_t v_h2 = a4::hi64(ctx->r[2]);
    uint64_t v_h3 = a4::hi64(ctx->r[3]);
    uint64_t xo[4] = {Ps2GprU64(ctx, 5), Ps2GprU64(ctx, 7), Ps2GprU64(ctx, 8), Ps2GprU64(ctx, 9)};
    if (!ckLeg([&] { v0 = (ty::SetDemoMotionOrientInfo(rdram, runtime, p4, p5, p6, v_h2, v_h3, xo)); })) return;
    R5900Context &t = ckTyCtx(ctx);
    t.r[2] = a4::mk128(v_h2, Ps2GprU64(&t, 2));
    t.r[3] = a4::mk128(v_h3, Ps2GprU64(&t, 3));
    ckEnd(85, ctx, c, 2, v0, f0, 0x3b8u, 0x0u, Ps2GprU64(&c, 5) != xo[0] || Ps2GprU64(&c, 7) != xo[1] || Ps2GprU64(&c, 8) != xo[2] || Ps2GprU64(&c, 9) != xo[3], vuDiffer(t, c));
}
void SetJudgeVolumeInfo(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t p4, uint32_t p5, uint32_t p6, uint32_t p7, uint32_t p8, uint32_t p9, uint32_t p10, float pf12, float pf13, float pf14)
{
    if (!ckBegin(86, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    if (!ckLeg([&] { ref::SetJudgeVolumeInfo(rdram, &c, runtime); })) return;
    ckMid();
    uint64_t v0 = 0u;
    float f0 = 0.0f;
    uint64_t v_h2 = a4::hi64(ctx->r[2]);
    uint64_t v_h3 = a4::hi64(ctx->r[3]);
    if (!ckLeg([&] { v0 = (ty::SetJudgeVolumeInfo(rdram, runtime, p4, p5, p6, p7, p8, p9, p10, pf12, pf13, pf14, v_h2, v_h3)); })) return;
    R5900Context &t = ckTyCtx(ctx);
    t.r[2] = a4::mk128(v_h2, Ps2GprU64(&t, 2));
    t.r[3] = a4::mk128(v_h3, Ps2GprU64(&t, 3));
    ckEnd(86, ctx, c, 2, v0, f0, 0x8u, 0x0u, false, vuDiffer(t, c));
}
void UpdateGlobalLightInRange(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t p4, uint32_t p5)
{
    if (!ckBegin(87, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    if (!ckLeg([&] { ref::UpdateGlobalLightInRange(rdram, &c, runtime); })) return;
    ckMid();
    uint64_t v0 = 0u;
    float f0 = 0.0f;
    a4::vec4 v_vf5 = ctx->vu0_vf[5];
    a4::vec4 v_vf6 = ctx->vu0_vf[6];
    uint64_t v_h2 = a4::hi64(ctx->r[2]);
    if (!ckLeg([&] { const a1::RetVF r = ty::UpdateGlobalLightInRange(rdram, runtime, p4, p5, v_vf5, v_vf6, v_h2); v0 = r.v0; f0 = r.f0; })) return;
    R5900Context &t = ckTyCtx(ctx);
    t.vu0_vf[5] = v_vf5;
    t.vu0_vf[6] = v_vf6;
    ckEnd(87, ctx, c, 4, v0, f0, 0x32u, 0x0u, false, vuDiffer(t, c));
}
void CopyCollisionDynamics(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t p4, uint32_t p5)
{
    if (!ckBegin(88, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    if (!ckLeg([&] { ref::CopyCollisionDynamics(rdram, &c, runtime); })) return;
    ckMid();
    uint64_t v0 = 0u;
    float f0 = 0.0f;
    uint64_t v_h2 = a4::hi64(ctx->r[2]);
    uint64_t v_h3 = a4::hi64(ctx->r[3]);
    if (!ckLeg([&] { v0 = (ty::CopyCollisionDynamics(rdram, runtime, p4, p5, v_h2, v_h3)); })) return;
    R5900Context &t = ckTyCtx(ctx);
    t.r[2] = a4::mk128(v_h2, Ps2GprU64(&t, 2));
    t.r[3] = a4::mk128(v_h3, Ps2GprU64(&t, 3));
    ckEnd(88, ctx, c, 2, v0, f0, 0xc8u, 0x0u, false, vuDiffer(t, c));
}
void FilterCopyCollisionDynamics(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t p4, uint32_t p5)
{
    if (!ckBegin(89, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    if (!ckLeg([&] { ref::FilterCopyCollisionDynamics(rdram, &c, runtime); })) return;
    ckMid();
    uint64_t v0 = 0u;
    float f0 = 0.0f;
    a4::vec4 v_vf0 = ctx->vu0_vf[0];
    a4::vec4 v_vf5 = ctx->vu0_vf[5];
    a4::vec4 v_vf6 = ctx->vu0_vf[6];
    a4::vec4 v_vf7 = ctx->vu0_vf[7];
    a4::vec4 v_vacc = ctx->vu0_acc;
    float v_vq = ctx->vu0_q;
    uint16_t v_vstat = ctx->vu0_status;
    uint64_t v_h3 = a4::hi64(ctx->r[3]);
    if (!ckLeg([&] { const a1::RetVF r = ty::FilterCopyCollisionDynamics(rdram, runtime, p4, p5, v_vf0, v_vf5, v_vf6, v_vf7, v_vacc, v_vq, v_vstat, v_h3); v0 = r.v0; f0 = r.f0; })) return;
    R5900Context &t = ckTyCtx(ctx);
    t.vu0_vf[5] = v_vf5;
    t.vu0_vf[6] = v_vf6;
    t.vu0_vf[7] = v_vf7;
    t.vu0_acc = v_vacc;
    t.vu0_q = v_vq;
    t.vu0_status = v_vstat;
    t.r[3] = a4::mk128(v_h3, Ps2GprU64(&t, 3));
    ckEnd(89, ctx, c, 4, v0, f0, 0x7au, 0x2u, false, vuDiffer(t, c));
}
void manager_sub_01129258(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t p4, uint32_t p5, uint32_t p6, uint32_t p7)
{
    if (!ckBegin(90, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    if (!ckLeg([&] { ref::manager_sub_01129258(rdram, &c, runtime); })) return;
    ckMid();
    uint64_t v0 = 0u;
    float f0 = 0.0f;
    a4::vec4 v_vf0 = ctx->vu0_vf[0];
    a4::vec4 v_vf5 = ctx->vu0_vf[5];
    a4::vec4 v_vf6 = ctx->vu0_vf[6];
    a4::vec4 v_vf7 = ctx->vu0_vf[7];
    a4::vec4 v_vf8 = ctx->vu0_vf[8];
    a4::vec4 v_vf9 = ctx->vu0_vf[9];
    a4::vec4 v_vacc = ctx->vu0_acc;
    uint64_t v_h2 = a4::hi64(ctx->r[2]);
    uint64_t v_h8 = a4::hi64(ctx->r[8]);
    uint64_t xo[1] = {Ps2GprU64(ctx, 8)};
    if (!ckLeg([&] { const a1::RetVF r = ty::manager_sub_01129258(rdram, runtime, p4, p5, p6, p7, v_vf0, v_vf5, v_vf6, v_vf7, v_vf8, v_vf9, v_vacc, v_h2, v_h8, xo); v0 = r.v0; f0 = r.f0; })) return;
    R5900Context &t = ckTyCtx(ctx);
    t.vu0_vf[5] = v_vf5;
    t.vu0_vf[6] = v_vf6;
    t.vu0_vf[7] = v_vf7;
    t.vu0_vf[8] = v_vf8;
    t.vu0_vf[9] = v_vf9;
    t.vu0_acc = v_vacc;
    t.r[2] = a4::mk128(v_h2, Ps2GprU64(&t, 2));
    ckEnd(90, ctx, c, 4, v0, f0, 0x102u, 0x2u, Ps2GprU64(&c, 8) != xo[0], vuDiffer(t, c));
}
void SetCollisionEdgeLinkDBFinderVWallDirectionStatus(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t p4, uint32_t p5)
{
    if (!ckBegin(91, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    if (!ckLeg([&] { ref::SetCollisionEdgeLinkDBFinderVWallDirectionStatus(rdram, &c, runtime); })) return;
    ckMid();
    uint64_t v0 = 0u;
    float f0 = 0.0f;
    uint64_t v_h2 = a4::hi64(ctx->r[2]);
    if (!ckLeg([&] { v0 = (ty::SetCollisionEdgeLinkDBFinderVWallDirectionStatus(rdram, runtime, p4, p5, v_h2)); })) return;
    R5900Context &t = ckTyCtx(ctx);
    t.r[2] = a4::mk128(v_h2, Ps2GprU64(&t, 2));
    ckEnd(91, ctx, c, 2, v0, f0, 0x0u, 0x0u, false, vuDiffer(t, c));
}
void SetCollisionEdgeLinkDBFinderCollectStatus(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t p4, uint32_t p5, uint32_t p6, uint32_t p7, uint32_t p8, float pf12)
{
    if (!ckBegin(92, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    if (!ckLeg([&] { ref::SetCollisionEdgeLinkDBFinderCollectStatus(rdram, &c, runtime); })) return;
    ckMid();
    uint64_t v0 = 0u;
    float f0 = 0.0f;
    uint64_t v_h2 = a4::hi64(ctx->r[2]);
    if (!ckLeg([&] { v0 = (ty::SetCollisionEdgeLinkDBFinderCollectStatus(rdram, runtime, p4, p5, p6, p7, p8, pf12, v_h2)); })) return;
    R5900Context &t = ckTyCtx(ctx);
    t.r[2] = a4::mk128(v_h2, Ps2GprU64(&t, 2));
    ckEnd(92, ctx, c, 2, v0, f0, 0x0u, 0x0u, false, vuDiffer(t, c));
}
void SetGeometryPointerGlobalState(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t p4, uint32_t p5, uint32_t p6)
{
    if (!ckBegin(93, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    if (!ckLeg([&] { ref::SetGeometryPointerGlobalState(rdram, &c, runtime); })) return;
    ckMid();
    uint64_t v0 = 0u;
    float f0 = 0.0f;
    uint64_t v_h2 = a4::hi64(ctx->r[2]);
    uint64_t v_h3 = a4::hi64(ctx->r[3]);
    if (!ckLeg([&] { v0 = (ty::SetGeometryPointerGlobalState(rdram, runtime, p4, p5, p6, v_h2, v_h3)); })) return;
    R5900Context &t = ckTyCtx(ctx);
    t.r[2] = a4::mk128(v_h2, Ps2GprU64(&t, 2));
    t.r[3] = a4::mk128(v_h3, Ps2GprU64(&t, 3));
    ckEnd(93, ctx, c, 2, v0, f0, 0x8u, 0x0u, false, vuDiffer(t, c));
}
void SetGeometryPointerGlobalStatePointer(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t p4, uint32_t p5, uint32_t p6, uint32_t p7, uint32_t p8)
{
    if (!ckBegin(94, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    if (!ckLeg([&] { ref::SetGeometryPointerGlobalStatePointer(rdram, &c, runtime); })) return;
    ckMid();
    uint64_t v0 = 0u;
    float f0 = 0.0f;
    uint64_t v_h2 = a4::hi64(ctx->r[2]);
    uint64_t v_h3 = a4::hi64(ctx->r[3]);
    if (!ckLeg([&] { v0 = (ty::SetGeometryPointerGlobalStatePointer(rdram, runtime, p4, p5, p6, p7, p8, v_h2, v_h3)); })) return;
    R5900Context &t = ckTyCtx(ctx);
    t.r[2] = a4::mk128(v_h2, Ps2GprU64(&t, 2));
    t.r[3] = a4::mk128(v_h3, Ps2GprU64(&t, 3));
    ckEnd(94, ctx, c, 2, v0, f0, 0x8u, 0x0u, false, vuDiffer(t, c));
}
void SetGeometryPointerNodePosState(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t p4, uint32_t p5, uint32_t p6, uint32_t p7, uint32_t p8)
{
    if (!ckBegin(95, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    if (!ckLeg([&] { ref::SetGeometryPointerNodePosState(rdram, &c, runtime); })) return;
    ckMid();
    uint64_t v0 = 0u;
    float f0 = 0.0f;
    uint64_t v_h2 = a4::hi64(ctx->r[2]);
    uint64_t v_h3 = a4::hi64(ctx->r[3]);
    if (!ckLeg([&] { v0 = (ty::SetGeometryPointerNodePosState(rdram, runtime, p4, p5, p6, p7, p8, v_h2, v_h3)); })) return;
    R5900Context &t = ckTyCtx(ctx);
    t.r[2] = a4::mk128(v_h2, Ps2GprU64(&t, 2));
    t.r[3] = a4::mk128(v_h3, Ps2GprU64(&t, 3));
    ckEnd(95, ctx, c, 2, v0, f0, 0x8u, 0x0u, false, vuDiffer(t, c));
}
void SetGeometryPointerNodePosStateGlobalRot(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t p4, uint32_t p5, uint32_t p6, uint32_t p7, uint32_t p8)
{
    if (!ckBegin(96, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    if (!ckLeg([&] { ref::SetGeometryPointerNodePosStateGlobalRot(rdram, &c, runtime); })) return;
    ckMid();
    uint64_t v0 = 0u;
    float f0 = 0.0f;
    uint64_t v_h2 = a4::hi64(ctx->r[2]);
    uint64_t v_h3 = a4::hi64(ctx->r[3]);
    if (!ckLeg([&] { v0 = (ty::SetGeometryPointerNodePosStateGlobalRot(rdram, runtime, p4, p5, p6, p7, p8, v_h2, v_h3)); })) return;
    R5900Context &t = ckTyCtx(ctx);
    t.r[2] = a4::mk128(v_h2, Ps2GprU64(&t, 2));
    t.r[3] = a4::mk128(v_h3, Ps2GprU64(&t, 3));
    ckEnd(96, ctx, c, 2, v0, f0, 0x8u, 0x0u, false, vuDiffer(t, c));
}
void GetPointDistanceSquareFromUnnormalizedPlane(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t p4, uint32_t p5)
{
    if (!ckBegin(97, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    if (!ckLeg([&] { ref::GetPointDistanceSquareFromUnnormalizedPlane(rdram, &c, runtime); })) return;
    ckMid();
    uint64_t v0 = 0u;
    float f0 = 0.0f;
    a4::vec4 v_vf0 = ctx->vu0_vf[0];
    a4::vec4 v_vf5 = ctx->vu0_vf[5];
    a4::vec4 v_vf6 = ctx->vu0_vf[6];
    a4::vec4 v_vf7 = ctx->vu0_vf[7];
    a4::vec4 v_vacc = ctx->vu0_acc;
    uint64_t v_h2 = a4::hi64(ctx->r[2]);
    if (!ckLeg([&] { const a1::RetVF r = ty::GetPointDistanceSquareFromUnnormalizedPlane(rdram, runtime, p4, p5, v_vf0, v_vf5, v_vf6, v_vf7, v_vacc, v_h2); v0 = r.v0; f0 = r.f0; })) return;
    R5900Context &t = ckTyCtx(ctx);
    t.vu0_vf[5] = v_vf5;
    t.vu0_vf[6] = v_vf6;
    t.vu0_vf[7] = v_vf7;
    t.vu0_acc = v_vacc;
    t.r[2] = a4::mk128(v_h2, Ps2GprU64(&t, 2));
    ckEnd(97, ctx, c, 4, v0, f0, 0x0u, 0x2u, false, vuDiffer(t, c));
}
void GetPointDistanceFromPlane(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t p4, uint32_t p5)
{
    if (!ckBegin(98, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    if (!ckLeg([&] { ref::GetPointDistanceFromPlane(rdram, &c, runtime); })) return;
    ckMid();
    uint64_t v0 = 0u;
    float f0 = 0.0f;
    a4::vec4 v_vf0 = ctx->vu0_vf[0];
    a4::vec4 v_vf5 = ctx->vu0_vf[5];
    a4::vec4 v_vf6 = ctx->vu0_vf[6];
    a4::vec4 v_vf7 = ctx->vu0_vf[7];
    a4::vec4 v_vacc = ctx->vu0_acc;
    uint64_t v_h2 = a4::hi64(ctx->r[2]);
    if (!ckLeg([&] { const a1::RetVF r = ty::GetPointDistanceFromPlane(rdram, runtime, p4, p5, v_vf0, v_vf5, v_vf6, v_vf7, v_vacc, v_h2); v0 = r.v0; f0 = r.f0; })) return;
    R5900Context &t = ckTyCtx(ctx);
    t.vu0_vf[5] = v_vf5;
    t.vu0_vf[6] = v_vf6;
    t.vu0_vf[7] = v_vf7;
    t.vu0_acc = v_vacc;
    t.r[2] = a4::mk128(v_h2, Ps2GprU64(&t, 2));
    ckEnd(98, ctx, c, 4, v0, f0, 0x0u, 0x0u, false, vuDiffer(t, c));
}
void MultiMatrixByPlane(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t p4, uint32_t p5, uint32_t p6)
{
    if (!ckBegin(99, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    if (!ckLeg([&] { ref::MultiMatrixByPlane(rdram, &c, runtime); })) return;
    ckMid();
    uint64_t v0 = 0u;
    float f0 = 0.0f;
    a4::vec4 v_vf0 = ctx->vu0_vf[0];
    a4::vec4 v_vf5 = ctx->vu0_vf[5];
    a4::vec4 v_vf6 = ctx->vu0_vf[6];
    a4::vec4 v_vf7 = ctx->vu0_vf[7];
    a4::vec4 v_vf8 = ctx->vu0_vf[8];
    a4::vec4 v_vf10 = ctx->vu0_vf[10];
    a4::vec4 v_vf14 = ctx->vu0_vf[14];
    a4::vec4 v_vf15 = ctx->vu0_vf[15];
    a4::vec4 v_vf16 = ctx->vu0_vf[16];
    a4::vec4 v_vf17 = ctx->vu0_vf[17];
    a4::vec4 v_vacc = ctx->vu0_acc;
    uint64_t v_h2 = a4::hi64(ctx->r[2]);
    if (!ckLeg([&] { const a1::RetVF r = ty::MultiMatrixByPlane(rdram, runtime, p4, p5, p6, v_vf0, v_vf5, v_vf6, v_vf7, v_vf8, v_vf10, v_vf14, v_vf15, v_vf16, v_vf17, v_vacc, v_h2); v0 = r.v0; f0 = r.f0; })) return;
    R5900Context &t = ckTyCtx(ctx);
    t.vu0_vf[5] = v_vf5;
    t.vu0_vf[6] = v_vf6;
    t.vu0_vf[7] = v_vf7;
    t.vu0_vf[8] = v_vf8;
    t.vu0_vf[10] = v_vf10;
    t.vu0_vf[14] = v_vf14;
    t.vu0_vf[15] = v_vf15;
    t.vu0_vf[16] = v_vf16;
    t.vu0_vf[17] = v_vf17;
    t.vu0_acc = v_vacc;
    t.r[2] = a4::mk128(v_h2, Ps2GprU64(&t, 2));
    ckEnd(99, ctx, c, 4, v0, f0, 0x20u, 0x2u, false, vuDiffer(t, c));
}
void CompletePlaneD(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t p4, uint32_t p5)
{
    if (!ckBegin(100, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    if (!ckLeg([&] { ref::CompletePlaneD(rdram, &c, runtime); })) return;
    ckMid();
    uint64_t v0 = 0u;
    float f0 = 0.0f;
    a4::vec4 v_vf0 = ctx->vu0_vf[0];
    a4::vec4 v_vf4 = ctx->vu0_vf[4];
    a4::vec4 v_vf5 = ctx->vu0_vf[5];
    a4::vec4 v_vf6 = ctx->vu0_vf[6];
    if (!ckLeg([&] { ty::CompletePlaneD(rdram, runtime, p4, p5, v_vf0, v_vf4, v_vf5, v_vf6); })) return;
    R5900Context &t = ckTyCtx(ctx);
    t.vu0_vf[4] = v_vf4;
    t.vu0_vf[5] = v_vf5;
    t.vu0_vf[6] = v_vf6;
    ckEnd(100, ctx, c, 0, v0, f0, 0x0u, 0x0u, false, vuDiffer(t, c));
}
void GetSTPositionOfVectorPlane(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t p4, uint32_t p5, uint32_t p6, uint32_t p7, uint32_t p8)
{
    if (!ckBegin(101, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    if (!ckLeg([&] { ref::GetSTPositionOfVectorPlane(rdram, &c, runtime); })) return;
    ckMid();
    uint64_t v0 = 0u;
    float f0 = 0.0f;
    a4::vec4 v_vf0 = ctx->vu0_vf[0];
    a4::vec4 v_vf5 = ctx->vu0_vf[5];
    a4::vec4 v_vf6 = ctx->vu0_vf[6];
    a4::vec4 v_vf7 = ctx->vu0_vf[7];
    a4::vec4 v_vf8 = ctx->vu0_vf[8];
    a4::vec4 v_vf9 = ctx->vu0_vf[9];
    a4::vec4 v_vf10 = ctx->vu0_vf[10];
    a4::vec4 v_vf11 = ctx->vu0_vf[11];
    a4::vec4 v_vacc = ctx->vu0_acc;
    uint64_t v_h2 = a4::hi64(ctx->r[2]);
    if (!ckLeg([&] { const a1::RetVF r = ty::GetSTPositionOfVectorPlane(rdram, runtime, p4, p5, p6, p7, p8, v_vf0, v_vf5, v_vf6, v_vf7, v_vf8, v_vf9, v_vf10, v_vf11, v_vacc, v_h2); v0 = r.v0; f0 = r.f0; })) return;
    R5900Context &t = ckTyCtx(ctx);
    t.vu0_vf[5] = v_vf5;
    t.vu0_vf[6] = v_vf6;
    t.vu0_vf[7] = v_vf7;
    t.vu0_vf[8] = v_vf8;
    t.vu0_vf[9] = v_vf9;
    t.vu0_vf[10] = v_vf10;
    t.vu0_vf[11] = v_vf11;
    t.vu0_acc = v_vacc;
    t.r[2] = a4::mk128(v_h2, Ps2GprU64(&t, 2));
    ckEnd(101, ctx, c, 4, v0, f0, 0x0u, 0xeu, false, vuDiffer(t, c));
}
void GetPlaneFittingDiffVector(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t p4, uint32_t p5, uint32_t p6)
{
    if (!ckBegin(102, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    if (!ckLeg([&] { ref::GetPlaneFittingDiffVector(rdram, &c, runtime); })) return;
    ckMid();
    uint64_t v0 = 0u;
    float f0 = 0.0f;
    a4::vec4 v_vf0 = ctx->vu0_vf[0];
    a4::vec4 v_vf5 = ctx->vu0_vf[5];
    a4::vec4 v_vf6 = ctx->vu0_vf[6];
    a4::vec4 v_vf7 = ctx->vu0_vf[7];
    a4::vec4 v_vacc = ctx->vu0_acc;
    uint64_t v_h2 = a4::hi64(ctx->r[2]);
    if (!ckLeg([&] { const a1::RetVF r = ty::GetPlaneFittingDiffVector(rdram, runtime, p4, p5, p6, v_vf0, v_vf5, v_vf6, v_vf7, v_vacc, v_h2); v0 = r.v0; f0 = r.f0; })) return;
    R5900Context &t = ckTyCtx(ctx);
    t.vu0_vf[5] = v_vf5;
    t.vu0_vf[6] = v_vf6;
    t.vu0_vf[7] = v_vf7;
    t.vu0_acc = v_vacc;
    t.r[2] = a4::mk128(v_h2, Ps2GprU64(&t, 2));
    ckEnd(102, ctx, c, 4, v0, f0, 0x0u, 0x0u, false, vuDiffer(t, c));
}
void GetPlaneFittingPointForUnnormalizedPlane(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t p4, uint32_t p5, uint32_t p6)
{
    if (!ckBegin(103, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    if (!ckLeg([&] { ref::GetPlaneFittingPointForUnnormalizedPlane(rdram, &c, runtime); })) return;
    ckMid();
    uint64_t v0 = 0u;
    float f0 = 0.0f;
    a4::vec4 v_vf0 = ctx->vu0_vf[0];
    a4::vec4 v_vf5 = ctx->vu0_vf[5];
    a4::vec4 v_vf6 = ctx->vu0_vf[6];
    a4::vec4 v_vf7 = ctx->vu0_vf[7];
    a4::vec4 v_vacc = ctx->vu0_acc;
    uint64_t v_h2 = a4::hi64(ctx->r[2]);
    if (!ckLeg([&] { const a1::RetVF r = ty::GetPlaneFittingPointForUnnormalizedPlane(rdram, runtime, p4, p5, p6, v_vf0, v_vf5, v_vf6, v_vf7, v_vacc, v_h2); v0 = r.v0; f0 = r.f0; })) return;
    R5900Context &t = ckTyCtx(ctx);
    t.vu0_vf[5] = v_vf5;
    t.vu0_vf[6] = v_vf6;
    t.vu0_vf[7] = v_vf7;
    t.vu0_acc = v_vacc;
    t.r[2] = a4::mk128(v_h2, Ps2GprU64(&t, 2));
    ckEnd(103, ctx, c, 4, v0, f0, 0x0u, 0x2u, false, vuDiffer(t, c));
}
void GetPlaneFittingVector(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t p4, uint32_t p5, uint32_t p6)
{
    if (!ckBegin(104, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    if (!ckLeg([&] { ref::GetPlaneFittingVector(rdram, &c, runtime); })) return;
    ckMid();
    uint64_t v0 = 0u;
    float f0 = 0.0f;
    a4::vec4 v_vf0 = ctx->vu0_vf[0];
    a4::vec4 v_vf5 = ctx->vu0_vf[5];
    a4::vec4 v_vf6 = ctx->vu0_vf[6];
    a4::vec4 v_vf7 = ctx->vu0_vf[7];
    a4::vec4 v_vf29 = ctx->vu0_vf[29];
    a4::vec4 v_vf30 = ctx->vu0_vf[30];
    a4::vec4 v_vf31 = ctx->vu0_vf[31];
    a4::vec4 v_vacc = ctx->vu0_acc;
    uint64_t v_h2 = a4::hi64(ctx->r[2]);
    uint64_t v_h8 = a4::hi64(ctx->r[8]);
    uint64_t xo[1] = {Ps2GprU64(ctx, 8)};
    if (!ckLeg([&] { const a1::RetVF r = ty::GetPlaneFittingVector(rdram, runtime, p4, p5, p6, v_vf0, v_vf5, v_vf6, v_vf7, v_vf29, v_vf30, v_vf31, v_vacc, v_h2, v_h8, xo); v0 = r.v0; f0 = r.f0; })) return;
    R5900Context &t = ckTyCtx(ctx);
    t.vu0_vf[5] = v_vf5;
    t.vu0_vf[6] = v_vf6;
    t.vu0_vf[7] = v_vf7;
    t.vu0_vf[29] = v_vf29;
    t.vu0_vf[30] = v_vf30;
    t.vu0_vf[31] = v_vf31;
    t.vu0_acc = v_vacc;
    t.r[2] = a4::mk128(v_h2, Ps2GprU64(&t, 2));
    ckEnd(104, ctx, c, 4, v0, f0, 0x100u, 0x0u, Ps2GprU64(&c, 8) != xo[0], vuDiffer(t, c));
}
void GetPlaneReflectionVector(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t p4, uint32_t p5, uint32_t p6)
{
    if (!ckBegin(105, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    if (!ckLeg([&] { ref::GetPlaneReflectionVector(rdram, &c, runtime); })) return;
    ckMid();
    uint64_t v0 = 0u;
    float f0 = 0.0f;
    a4::vec4 v_vf0 = ctx->vu0_vf[0];
    a4::vec4 v_vf5 = ctx->vu0_vf[5];
    a4::vec4 v_vf6 = ctx->vu0_vf[6];
    a4::vec4 v_vf7 = ctx->vu0_vf[7];
    a4::vec4 v_vacc = ctx->vu0_acc;
    uint64_t v_h2 = a4::hi64(ctx->r[2]);
    if (!ckLeg([&] { const a1::RetVF r = ty::GetPlaneReflectionVector(rdram, runtime, p4, p5, p6, v_vf0, v_vf5, v_vf6, v_vf7, v_vacc, v_h2); v0 = r.v0; f0 = r.f0; })) return;
    R5900Context &t = ckTyCtx(ctx);
    t.vu0_vf[5] = v_vf5;
    t.vu0_vf[6] = v_vf6;
    t.vu0_vf[7] = v_vf7;
    t.vu0_acc = v_vacc;
    t.r[2] = a4::mk128(v_h2, Ps2GprU64(&t, 2));
    ckEnd(105, ctx, c, 4, v0, f0, 0x0u, 0x0u, false, vuDiffer(t, c));
}
void GetNormalPlane(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t p4, uint32_t p5)
{
    if (!ckBegin(106, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    if (!ckLeg([&] { ref::GetNormalPlane(rdram, &c, runtime); })) return;
    ckMid();
    uint64_t v0 = 0u;
    float f0 = 0.0f;
    a4::vec4 v_vf0 = ctx->vu0_vf[0];
    a4::vec4 v_vf5 = ctx->vu0_vf[5];
    a4::vec4 v_vf6 = ctx->vu0_vf[6];
    a4::vec4 v_vf7 = ctx->vu0_vf[7];
    a4::vec4 v_vacc = ctx->vu0_acc;
    float v_vq = ctx->vu0_q;
    uint16_t v_vstat = ctx->vu0_status;
    uint64_t v_h2 = a4::hi64(ctx->r[2]);
    if (!ckLeg([&] { const a1::RetVF r = ty::GetNormalPlane(rdram, runtime, p4, p5, v_vf0, v_vf5, v_vf6, v_vf7, v_vacc, v_vq, v_vstat, v_h2); v0 = r.v0; f0 = r.f0; })) return;
    R5900Context &t = ckTyCtx(ctx);
    t.vu0_vf[5] = v_vf5;
    t.vu0_vf[6] = v_vf6;
    t.vu0_vf[7] = v_vf7;
    t.vu0_acc = v_vacc;
    t.vu0_q = v_vq;
    t.vu0_status = v_vstat;
    ckEnd(106, ctx, c, 4, v0, f0, 0x2u, 0x2u, false, vuDiffer(t, c));
}
void GetCrossPositionPlaneLine(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t p4, uint32_t p5, uint32_t p6, uint32_t p7)
{
    if (!ckBegin(107, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    if (!ckLeg([&] { ref::GetCrossPositionPlaneLine(rdram, &c, runtime); })) return;
    ckMid();
    uint64_t v0 = 0u;
    float f0 = 0.0f;
    a4::vec4 v_vf0 = ctx->vu0_vf[0];
    a4::vec4 v_vf5 = ctx->vu0_vf[5];
    a4::vec4 v_vf6 = ctx->vu0_vf[6];
    a4::vec4 v_vf7 = ctx->vu0_vf[7];
    a4::vec4 v_vf8 = ctx->vu0_vf[8];
    a4::vec4 v_vacc = ctx->vu0_acc;
    uint64_t v_h2 = a4::hi64(ctx->r[2]);
    uint64_t v_h8 = a4::hi64(ctx->r[8]);
    uint64_t xo[1] = {Ps2GprU64(ctx, 8)};
    if (!ckLeg([&] { const a1::RetVF r = ty::GetCrossPositionPlaneLine(rdram, runtime, p4, p5, p6, p7, v_vf0, v_vf5, v_vf6, v_vf7, v_vf8, v_vacc, v_h2, v_h8, xo); v0 = r.v0; f0 = r.f0; })) return;
    R5900Context &t = ckTyCtx(ctx);
    t.vu0_vf[5] = v_vf5;
    t.vu0_vf[6] = v_vf6;
    t.vu0_vf[7] = v_vf7;
    t.vu0_vf[8] = v_vf8;
    t.vu0_acc = v_vacc;
    t.r[2] = a4::mk128(v_h2, Ps2GprU64(&t, 2));
    ckEnd(107, ctx, c, 4, v0, f0, 0x102u, 0xeu, Ps2GprU64(&c, 8) != xo[0], vuDiffer(t, c));
}
void GetPlaneDistance(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t p4, uint32_t p5)
{
    if (!ckBegin(108, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    if (!ckLeg([&] { ref::GetPlaneDistance(rdram, &c, runtime); })) return;
    ckMid();
    uint64_t v0 = 0u;
    float f0 = 0.0f;
    a4::vec4 v_vf5 = ctx->vu0_vf[5];
    a4::vec4 v_vf6 = ctx->vu0_vf[6];
    a4::vec4 v_vf7 = ctx->vu0_vf[7];
    uint64_t v_h2 = a4::hi64(ctx->r[2]);
    if (!ckLeg([&] { const a1::RetVF r = ty::GetPlaneDistance(rdram, runtime, p4, p5, v_vf5, v_vf6, v_vf7, v_h2); v0 = r.v0; f0 = r.f0; })) return;
    R5900Context &t = ckTyCtx(ctx);
    t.vu0_vf[5] = v_vf5;
    t.vu0_vf[6] = v_vf6;
    t.vu0_vf[7] = v_vf7;
    t.r[2] = a4::mk128(v_h2, Ps2GprU64(&t, 2));
    ckEnd(108, ctx, c, 4, v0, f0, 0x0u, 0x0u, false, vuDiffer(t, c));
}
void GetDistanceSquare(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t p4, uint32_t p5)
{
    if (!ckBegin(109, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    if (!ckLeg([&] { ref::GetDistanceSquare(rdram, &c, runtime); })) return;
    ckMid();
    uint64_t v0 = 0u;
    float f0 = 0.0f;
    a4::vec4 v_vf5 = ctx->vu0_vf[5];
    a4::vec4 v_vf6 = ctx->vu0_vf[6];
    a4::vec4 v_vf7 = ctx->vu0_vf[7];
    uint64_t v_h2 = a4::hi64(ctx->r[2]);
    if (!ckLeg([&] { const a1::RetVF r = ty::GetDistanceSquare(rdram, runtime, p4, p5, v_vf5, v_vf6, v_vf7, v_h2); v0 = r.v0; f0 = r.f0; })) return;
    R5900Context &t = ckTyCtx(ctx);
    t.vu0_vf[5] = v_vf5;
    t.vu0_vf[6] = v_vf6;
    t.vu0_vf[7] = v_vf7;
    t.r[2] = a4::mk128(v_h2, Ps2GprU64(&t, 2));
    ckEnd(109, ctx, c, 4, v0, f0, 0x0u, 0x0u, false, vuDiffer(t, c));
}
void GetDistanceSquareXZ(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t p4, uint32_t p5)
{
    if (!ckBegin(110, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    if (!ckLeg([&] { ref::GetDistanceSquareXZ(rdram, &c, runtime); })) return;
    ckMid();
    uint64_t v0 = 0u;
    float f0 = 0.0f;
    a4::vec4 v_vf5 = ctx->vu0_vf[5];
    a4::vec4 v_vf6 = ctx->vu0_vf[6];
    a4::vec4 v_vf7 = ctx->vu0_vf[7];
    uint64_t v_h2 = a4::hi64(ctx->r[2]);
    if (!ckLeg([&] { const a1::RetVF r = ty::GetDistanceSquareXZ(rdram, runtime, p4, p5, v_vf5, v_vf6, v_vf7, v_h2); v0 = r.v0; f0 = r.f0; })) return;
    R5900Context &t = ckTyCtx(ctx);
    t.vu0_vf[5] = v_vf5;
    t.vu0_vf[6] = v_vf6;
    t.vu0_vf[7] = v_vf7;
    t.r[2] = a4::mk128(v_h2, Ps2GprU64(&t, 2));
    ckEnd(110, ctx, c, 4, v0, f0, 0x0u, 0x0u, false, vuDiffer(t, c));
}
void ExchangeLWVector(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t p4, uint32_t p5)
{
    if (!ckBegin(111, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    if (!ckLeg([&] { ref::ExchangeLWVector(rdram, &c, runtime); })) return;
    ckMid();
    uint64_t v0 = 0u;
    float f0 = 0.0f;
    a4::vec4 v_vf5 = ctx->vu0_vf[5];
    a4::vec4 v_vf6 = ctx->vu0_vf[6];
    uint64_t v_h2 = a4::hi64(ctx->r[2]);
    if (!ckLeg([&] { const a1::RetVF r = ty::ExchangeLWVector(rdram, runtime, p4, p5, v_vf5, v_vf6, v_h2); v0 = r.v0; f0 = r.f0; })) return;
    R5900Context &t = ckTyCtx(ctx);
    t.vu0_vf[5] = v_vf5;
    t.vu0_vf[6] = v_vf6;
    ckEnd(111, ctx, c, 4, v0, f0, 0x2u, 0x2u, false, vuDiffer(t, c));
}
void manager_sub_01142198(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t p4, uint32_t p5)
{
    if (!ckBegin(112, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    if (!ckLeg([&] { ref::manager_sub_01142198(rdram, &c, runtime); })) return;
    ckMid();
    uint64_t v0 = 0u;
    float f0 = 0.0f;
    a4::vec4 v_vf4 = ctx->vu0_vf[4];
    a4::vec4 v_vf5 = ctx->vu0_vf[5];
    a4::vec4 v_vf6 = ctx->vu0_vf[6];
    a4::vec4 v_vf7 = ctx->vu0_vf[7];
    a4::vec4 v_vf8 = ctx->vu0_vf[8];
    a4::vec4 v_vf9 = ctx->vu0_vf[9];
    a4::vec4 v_vf10 = ctx->vu0_vf[10];
    a4::vec4 v_vf11 = ctx->vu0_vf[11];
    uint64_t v_h7 = a4::hi64(ctx->r[7]);
    uint64_t v_h8 = a4::hi64(ctx->r[8]);
    uint64_t v_h9 = a4::hi64(ctx->r[9]);
    uint64_t xo[3] = {Ps2GprU64(ctx, 7), Ps2GprU64(ctx, 8), Ps2GprU64(ctx, 9)};
    if (!ckLeg([&] { const a1::RetVF r = ty::manager_sub_01142198(rdram, runtime, p4, p5, v_vf4, v_vf5, v_vf6, v_vf7, v_vf8, v_vf9, v_vf10, v_vf11, v_h7, v_h8, v_h9, xo); v0 = r.v0; f0 = r.f0; })) return;
    R5900Context &t = ckTyCtx(ctx);
    t.vu0_vf[4] = v_vf4;
    t.vu0_vf[5] = v_vf5;
    t.vu0_vf[6] = v_vf6;
    t.vu0_vf[7] = v_vf7;
    t.vu0_vf[8] = v_vf8;
    t.vu0_vf[9] = v_vf9;
    t.vu0_vf[10] = v_vf10;
    t.vu0_vf[11] = v_vf11;
    ckEnd(112, ctx, c, 4, v0, f0, 0x38au, 0xeu, Ps2GprU64(&c, 7) != xo[0] || Ps2GprU64(&c, 8) != xo[1] || Ps2GprU64(&c, 9) != xo[2], vuDiffer(t, c));
}
void manager_sub_011422D8(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t p4, uint32_t p5, uint32_t p6)
{
    if (!ckBegin(113, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    if (!ckLeg([&] { ref::manager_sub_011422D8(rdram, &c, runtime); })) return;
    ckMid();
    uint64_t v0 = 0u;
    float f0 = 0.0f;
    a4::vec4 v_vf0 = ctx->vu0_vf[0];
    a4::vec4 v_vf8 = ctx->vu0_vf[8];
    a4::vec4 v_vf9 = ctx->vu0_vf[9];
    a4::vec4 v_vf10 = ctx->vu0_vf[10];
    a4::vec4 v_vf14 = ctx->vu0_vf[14];
    a4::vec4 v_vf15 = ctx->vu0_vf[15];
    a4::vec4 v_vf16 = ctx->vu0_vf[16];
    a4::vec4 v_vf17 = ctx->vu0_vf[17];
    a4::vec4 v_vacc = ctx->vu0_acc;
    float v_vq = ctx->vu0_q;
    uint16_t v_vstat = ctx->vu0_status;
    uint64_t xo[3] = {Ps2GprU64(ctx, 6), Ps2GprU64(ctx, 7), Ps2GprU64(ctx, 8)};
    if (!ckLeg([&] { v0 = a1_sx64(ty::manager_sub_011422D8(rdram, runtime, p4, p5, p6, v_vf0, v_vf8, v_vf9, v_vf10, v_vf14, v_vf15, v_vf16, v_vf17, v_vacc, v_vq, v_vstat, xo)); })) return;
    R5900Context &t = ckTyCtx(ctx);
    t.vu0_vf[8] = v_vf8;
    t.vu0_vf[9] = v_vf9;
    t.vu0_vf[10] = v_vf10;
    t.vu0_vf[14] = v_vf14;
    t.vu0_vf[15] = v_vf15;
    t.vu0_vf[16] = v_vf16;
    t.vu0_vf[17] = v_vf17;
    t.vu0_acc = v_vacc;
    t.vu0_q = v_vq;
    t.vu0_status = v_vstat;
    ckEnd(113, ctx, c, 1, v0, f0, 0x1c8u, 0x0u, Ps2GprU64(&c, 6) != xo[0] || Ps2GprU64(&c, 7) != xo[1] || Ps2GprU64(&c, 8) != xo[2], vuDiffer(t, c));
}
void manager_sub_01146DA8(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t p4, uint32_t p5, uint32_t p6, float pf12, float pf13)
{
    if (!ckBegin(114, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    if (!ckLeg([&] { ref::manager_sub_01146DA8(rdram, &c, runtime); })) return;
    ckMid();
    uint64_t v0 = 0u;
    float f0 = 0.0f;
    a4::vec4 v_vf0 = ctx->vu0_vf[0];
    a4::vec4 v_vf5 = ctx->vu0_vf[5];
    a4::vec4 v_vf6 = ctx->vu0_vf[6];
    a4::vec4 v_vf7 = ctx->vu0_vf[7];
    a4::vec4 v_vacc = ctx->vu0_acc;
    float v_vq = ctx->vu0_q;
    uint16_t v_vstat = ctx->vu0_status;
    uint64_t v_h2 = a4::hi64(ctx->r[2]);
    uint64_t v_h3 = a4::hi64(ctx->r[3]);
    uint64_t xo[1] = {(uint64_t)a1ck::ckBits(ctx->f[0])};
    if (!ckLeg([&] { v0 = (ty::manager_sub_01146DA8(rdram, runtime, p4, p5, p6, pf12, pf13, v_vf0, v_vf5, v_vf6, v_vf7, v_vacc, v_vq, v_vstat, v_h2, v_h3, xo)); })) return;
    R5900Context &t = ckTyCtx(ctx);
    t.vu0_vf[5] = v_vf5;
    t.vu0_vf[6] = v_vf6;
    t.vu0_vf[7] = v_vf7;
    t.vu0_acc = v_vacc;
    t.vu0_q = v_vq;
    t.vu0_status = v_vstat;
    t.r[2] = a4::mk128(v_h2, Ps2GprU64(&t, 2));
    t.r[3] = a4::mk128(v_h3, Ps2GprU64(&t, 3));
    ckEnd(114, ctx, c, 2, v0, f0, 0x88u, 0x1003u, a1ck::ckBits(c.f[0]) != (uint32_t)xo[0], vuDiffer(t, c));
}
void manager_sub_01146EB8(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t p4, uint32_t p5, uint32_t p6, uint32_t p7)
{
    if (!ckBegin(115, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    if (!ckLeg([&] { ref::manager_sub_01146EB8(rdram, &c, runtime); })) return;
    ckMid();
    uint64_t v0 = 0u;
    float f0 = 0.0f;
    a4::vec4 v_vf5 = ctx->vu0_vf[5];
    a4::vec4 v_vf6 = ctx->vu0_vf[6];
    a4::vec4 v_vf7 = ctx->vu0_vf[7];
    uint64_t v_h2 = a4::hi64(ctx->r[2]);
    if (!ckLeg([&] { const a1::RetVF r = ty::manager_sub_01146EB8(rdram, runtime, p4, p5, p6, p7, v_vf5, v_vf6, v_vf7, v_h2); v0 = r.v0; f0 = r.f0; })) return;
    R5900Context &t = ckTyCtx(ctx);
    t.vu0_vf[5] = v_vf5;
    t.vu0_vf[6] = v_vf6;
    t.vu0_vf[7] = v_vf7;
    t.r[2] = a4::mk128(v_h2, Ps2GprU64(&t, 2));
    ckEnd(115, ctx, c, 4, v0, f0, 0xau, 0x1eu, false, vuDiffer(t, c));
}
void constructor_camera_lws_anime_data(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t p4)
{
    if (!ckBegin(116, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    if (!ckLeg([&] { ref::constructor_camera_lws_anime_data(rdram, &c, runtime); })) return;
    ckMid();
    uint64_t v0 = 0u;
    float f0 = 0.0f;
    uint64_t v_h2 = a4::hi64(ctx->r[2]);
    uint64_t v_h3 = a4::hi64(ctx->r[3]);
    if (!ckLeg([&] { const a1::RetVF r = ty::constructor_camera_lws_anime_data(rdram, runtime, p4, v_h2, v_h3); v0 = r.v0; f0 = r.f0; })) return;
    R5900Context &t = ckTyCtx(ctx);
    t.r[2] = a4::mk128(v_h2, Ps2GprU64(&t, 2));
    t.r[3] = a4::mk128(v_h3, Ps2GprU64(&t, 3));
    ckEnd(116, ctx, c, 4, v0, f0, 0xeau, 0x0u, false, vuDiffer(t, c));
}
void set_camera_position(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t p4, uint32_t p5)
{
    if (!ckBegin(117, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    if (!ckLeg([&] { ref::set_camera_position(rdram, &c, runtime); })) return;
    ckMid();
    uint64_t v0 = 0u;
    float f0 = 0.0f;
    uint64_t v_h2 = a4::hi64(ctx->r[2]);
    if (!ckLeg([&] { v0 = (ty::set_camera_position(rdram, runtime, p4, p5, v_h2)); })) return;
    R5900Context &t = ckTyCtx(ctx);
    t.r[2] = a4::mk128(v_h2, Ps2GprU64(&t, 2));
    ckEnd(117, ctx, c, 2, v0, f0, 0x0u, 0x0u, false, vuDiffer(t, c));
}
void set_camera_direction(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t p4, uint32_t p5)
{
    if (!ckBegin(118, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    if (!ckLeg([&] { ref::set_camera_direction(rdram, &c, runtime); })) return;
    ckMid();
    uint64_t v0 = 0u;
    float f0 = 0.0f;
    a4::vec4 v_vf0 = ctx->vu0_vf[0];
    a4::vec4 v_vf5 = ctx->vu0_vf[5];
    a4::vec4 v_vf7 = ctx->vu0_vf[7];
    a4::vec4 v_vacc = ctx->vu0_acc;
    float v_vq = ctx->vu0_q;
    uint16_t v_vstat = ctx->vu0_status;
    if (!ckLeg([&] { ty::set_camera_direction(rdram, runtime, p4, p5, v_vf0, v_vf5, v_vf7, v_vacc, v_vq, v_vstat); })) return;
    R5900Context &t = ckTyCtx(ctx);
    t.vu0_vf[5] = v_vf5;
    t.vu0_vf[7] = v_vf7;
    t.vu0_acc = v_vacc;
    t.vu0_q = v_vq;
    t.vu0_status = v_vstat;
    ckEnd(118, ctx, c, 0, v0, f0, 0x10u, 0x0u, false, vuDiffer(t, c));
}
void set_camera_up_direction(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t p4, uint32_t p5)
{
    if (!ckBegin(119, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    if (!ckLeg([&] { ref::set_camera_up_direction(rdram, &c, runtime); })) return;
    ckMid();
    uint64_t v0 = 0u;
    float f0 = 0.0f;
    a4::vec4 v_vf0 = ctx->vu0_vf[0];
    a4::vec4 v_vf5 = ctx->vu0_vf[5];
    a4::vec4 v_vf7 = ctx->vu0_vf[7];
    a4::vec4 v_vacc = ctx->vu0_acc;
    float v_vq = ctx->vu0_q;
    uint16_t v_vstat = ctx->vu0_status;
    if (!ckLeg([&] { ty::set_camera_up_direction(rdram, runtime, p4, p5, v_vf0, v_vf5, v_vf7, v_vacc, v_vq, v_vstat); })) return;
    R5900Context &t = ckTyCtx(ctx);
    t.vu0_vf[5] = v_vf5;
    t.vu0_vf[7] = v_vf7;
    t.vu0_acc = v_vacc;
    t.vu0_q = v_vq;
    t.vu0_status = v_vstat;
    ckEnd(119, ctx, c, 0, v0, f0, 0x10u, 0x0u, false, vuDiffer(t, c));
}
void InitDamageClass(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t p4)
{
    if (!ckBegin(120, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    if (!ckLeg([&] { ref::InitDamageClass(rdram, &c, runtime); })) return;
    ckMid();
    uint64_t v0 = 0u;
    float f0 = 0.0f;
    uint64_t v_h2 = a4::hi64(ctx->r[2]);
    if (!ckLeg([&] { v0 = (ty::InitDamageClass(rdram, runtime, p4, v_h2)); })) return;
    R5900Context &t = ckTyCtx(ctx);
    t.r[2] = a4::mk128(v_h2, Ps2GprU64(&t, 2));
    ckEnd(120, ctx, c, 2, v0, f0, 0x8u, 0x0u, false, vuDiffer(t, c));
}
void GAMEUTIL_set_custom_boss_interest_position(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t p4)
{
    if (!ckBegin(121, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    if (!ckLeg([&] { ref::GAMEUTIL_set_custom_boss_interest_position(rdram, &c, runtime); })) return;
    ckMid();
    uint64_t v0 = 0u;
    float f0 = 0.0f;
    uint64_t v_h3 = a4::hi64(ctx->r[3]);
    if (!ckLeg([&] { v0 = a1_sx64(ty::GAMEUTIL_set_custom_boss_interest_position(rdram, runtime, p4, v_h3)); })) return;
    R5900Context &t = ckTyCtx(ctx);
    t.r[3] = a4::mk128(v_h3, Ps2GprU64(&t, 3));
    ckEnd(121, ctx, c, 1, v0, f0, 0x8u, 0x0u, false, vuDiffer(t, c));
}
void GAMEUTIL_constructor_col_plane_info(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t p4)
{
    if (!ckBegin(122, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    if (!ckLeg([&] { ref::GAMEUTIL_constructor_col_plane_info(rdram, &c, runtime); })) return;
    ckMid();
    uint64_t v0 = 0u;
    float f0 = 0.0f;
    uint64_t v_h2 = a4::hi64(ctx->r[2]);
    uint64_t v_h3 = a4::hi64(ctx->r[3]);
    if (!ckLeg([&] { v0 = (ty::GAMEUTIL_constructor_col_plane_info(rdram, runtime, p4, v_h2, v_h3)); })) return;
    R5900Context &t = ckTyCtx(ctx);
    t.r[2] = a4::mk128(v_h2, Ps2GprU64(&t, 2));
    t.r[3] = a4::mk128(v_h3, Ps2GprU64(&t, 3));
    ckEnd(122, ctx, c, 2, v0, f0, 0x28u, 0x0u, false, vuDiffer(t, c));
}
void GAMEUTIL_reset_position_filter(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t p4, uint32_t p5)
{
    if (!ckBegin(123, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    if (!ckLeg([&] { ref::GAMEUTIL_reset_position_filter(rdram, &c, runtime); })) return;
    ckMid();
    uint64_t v0 = 0u;
    float f0 = 0.0f;
    uint64_t v_h2 = a4::hi64(ctx->r[2]);
    uint64_t v_h3 = a4::hi64(ctx->r[3]);
    if (!ckLeg([&] { v0 = (ty::GAMEUTIL_reset_position_filter(rdram, runtime, p4, p5, v_h2, v_h3)); })) return;
    R5900Context &t = ckTyCtx(ctx);
    t.r[2] = a4::mk128(v_h2, Ps2GprU64(&t, 2));
    t.r[3] = a4::mk128(v_h3, Ps2GprU64(&t, 3));
    ckEnd(123, ctx, c, 2, v0, f0, 0x48u, 0x0u, false, vuDiffer(t, c));
}
void GAMEUTIL_doit_position_filter(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t p4, uint32_t p5, uint64_t p6)
{
    if (!ckBegin(124, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    if (!ckLeg([&] { ref::GAMEUTIL_doit_position_filter(rdram, &c, runtime); })) return;
    ckMid();
    uint64_t v0 = 0u;
    float f0 = 0.0f;
    a4::vec4 v_vf5 = ctx->vu0_vf[5];
    a4::vec4 v_vf6 = ctx->vu0_vf[6];
    a4::vec4 v_vf7 = ctx->vu0_vf[7];
    uint64_t v_h2 = a4::hi64(ctx->r[2]);
    uint64_t v_h3 = a4::hi64(ctx->r[3]);
    uint64_t xo[1] = {(uint64_t)a1ck::ckBits(ctx->f[0])};
    if (!ckLeg([&] { v0 = (ty::GAMEUTIL_doit_position_filter(rdram, runtime, p4, p5, p6, v_vf5, v_vf6, v_vf7, v_h2, v_h3, xo)); })) return;
    R5900Context &t = ckTyCtx(ctx);
    t.vu0_vf[5] = v_vf5;
    t.vu0_vf[6] = v_vf6;
    t.vu0_vf[7] = v_vf7;
    t.r[2] = a4::mk128(v_h2, Ps2GprU64(&t, 2));
    t.r[3] = a4::mk128(v_h3, Ps2GprU64(&t, 3));
    ckEnd(124, ctx, c, 2, v0, f0, 0x88u, 0x3u, a1ck::ckBits(c.f[0]) != (uint32_t)xo[0], vuDiffer(t, c));
}
void SetCamFinalizerGeometry(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t p4, uint32_t p5)
{
    if (!ckBegin(125, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    if (!ckLeg([&] { ref::SetCamFinalizerGeometry(rdram, &c, runtime); })) return;
    ckMid();
    uint64_t v0 = 0u;
    float f0 = 0.0f;
    uint64_t v_h2 = a4::hi64(ctx->r[2]);
    uint64_t v_h6 = a4::hi64(ctx->r[6]);
    if (!ckLeg([&] { v0 = (ty::SetCamFinalizerGeometry(rdram, runtime, p4, p5, v_h2, v_h6)); })) return;
    R5900Context &t = ckTyCtx(ctx);
    t.r[2] = a4::mk128(v_h2, Ps2GprU64(&t, 2));
    t.r[6] = a4::mk128(v_h6, Ps2GprU64(&t, 6));
    ckEnd(125, ctx, c, 2, v0, f0, 0x48u, 0x0u, false, vuDiffer(t, c));
}
void fruitsFlagSet(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint64_t p4, uint64_t p5, uint32_t p7)
{
    if (!ckBegin(126, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    if (!ckLeg([&] { ref::fruitsFlagSet(rdram, &c, runtime); })) return;
    ckMid();
    uint64_t v0 = 0u;
    float f0 = 0.0f;
    uint64_t xo[1] = {Ps2GprU64(ctx, 7)};
    if (!ckLeg([&] { v0 = a1_sx64(ty::fruitsFlagSet(rdram, runtime, p4, p5, p7, xo)); })) return;
    R5900Context &t = ckTyCtx(ctx);
    ckEnd(126, ctx, c, 1, v0, f0, 0x1d8u, 0x0u, Ps2GprU64(&c, 7) != xo[0], vuDiffer(t, c));
}
void gamecore_sub_0137EA00(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t p4)
{
    if (!ckBegin(127, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    if (!ckLeg([&] { ref::gamecore_sub_0137EA00(rdram, &c, runtime); })) return;
    ckMid();
    uint64_t v0 = 0u;
    float f0 = 0.0f;
    a4::vec4 v_vf0 = ctx->vu0_vf[0];
    if (!ckLeg([&] { const a1::RetVF r = ty::gamecore_sub_0137EA00(rdram, runtime, p4, v_vf0); v0 = r.v0; f0 = r.f0; })) return;
    R5900Context &t = ckTyCtx(ctx);
    ckEnd(127, ctx, c, 4, v0, f0, 0x6au, 0x0u, false, vuDiffer(t, c));
}
void Init_TimeFVECTORInter(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t p4, uint32_t p5, uint32_t p6, float pf12, float pf13)
{
    if (!ckBegin(128, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    if (!ckLeg([&] { ref::Init_TimeFVECTORInter(rdram, &c, runtime); })) return;
    ckMid();
    uint64_t v0 = 0u;
    float f0 = 0.0f;
    uint64_t v_h2 = a4::hi64(ctx->r[2]);
    uint64_t v_h3 = a4::hi64(ctx->r[3]);
    if (!ckLeg([&] { v0 = (ty::Init_TimeFVECTORInter(rdram, runtime, p4, p5, p6, pf12, pf13, v_h2, v_h3)); })) return;
    R5900Context &t = ckTyCtx(ctx);
    t.r[2] = a4::mk128(v_h2, Ps2GprU64(&t, 2));
    t.r[3] = a4::mk128(v_h3, Ps2GprU64(&t, 3));
    ckEnd(128, ctx, c, 2, v0, f0, 0x8u, 0x0u, false, vuDiffer(t, c));
}
void Update_TimeFVECTORInter(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t p4)
{
    if (!ckBegin(129, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    if (!ckLeg([&] { ref::Update_TimeFVECTORInter(rdram, &c, runtime); })) return;
    ckMid();
    uint64_t v0 = 0u;
    float f0 = 0.0f;
    a4::vec4 v_vf0 = ctx->vu0_vf[0];
    a4::vec4 v_vf5 = ctx->vu0_vf[5];
    a4::vec4 v_vf6 = ctx->vu0_vf[6];
    a4::vec4 v_vf7 = ctx->vu0_vf[7];
    a4::vec4 v_vf8 = ctx->vu0_vf[8];
    a4::vec4 v_vacc = ctx->vu0_acc;
    uint64_t v_h2 = a4::hi64(ctx->r[2]);
    uint64_t v_h8 = a4::hi64(ctx->r[8]);
    if (!ckLeg([&] { ty::Update_TimeFVECTORInter(rdram, runtime, p4, v_vf0, v_vf5, v_vf6, v_vf7, v_vf8, v_vacc, v_h2, v_h8); })) return;
    R5900Context &t = ckTyCtx(ctx);
    t.vu0_vf[5] = v_vf5;
    t.vu0_vf[6] = v_vf6;
    t.vu0_vf[7] = v_vf7;
    t.vu0_vf[8] = v_vf8;
    t.vu0_acc = v_vacc;
    t.r[2] = a4::mk128(v_h2, Ps2GprU64(&t, 2));
    ckEnd(129, ctx, c, 0, v0, f0, 0x12cu, 0xfu, false, vuDiffer(t, c));
}
void CalcFootVec(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t p4, uint32_t p5, uint32_t p6, uint32_t p7)
{
    if (!ckBegin(130, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    if (!ckLeg([&] { ref::CalcFootVec(rdram, &c, runtime); })) return;
    ckMid();
    uint64_t v0 = 0u;
    float f0 = 0.0f;
    a4::vec4 v_vf4 = ctx->vu0_vf[4];
    a4::vec4 v_vf5 = ctx->vu0_vf[5];
    a4::vec4 v_vf6 = ctx->vu0_vf[6];
    a4::vec4 v_vf7 = ctx->vu0_vf[7];
    uint64_t v_h2 = a4::hi64(ctx->r[2]);
    uint64_t v_h8 = a4::hi64(ctx->r[8]);
    uint64_t xo[1] = {Ps2GprU64(ctx, 8)};
    if (!ckLeg([&] { const a1::RetVF r = ty::CalcFootVec(rdram, runtime, p4, p5, p6, p7, v_vf4, v_vf5, v_vf6, v_vf7, v_h2, v_h8, xo); v0 = r.v0; f0 = r.f0; })) return;
    R5900Context &t = ckTyCtx(ctx);
    t.vu0_vf[4] = v_vf4;
    t.vu0_vf[5] = v_vf5;
    t.vu0_vf[6] = v_vf6;
    t.vu0_vf[7] = v_vf7;
    t.r[2] = a4::mk128(v_h2, Ps2GprU64(&t, 2));
    ckEnd(130, ctx, c, 4, v0, f0, 0x100u, 0x2u, Ps2GprU64(&c, 8) != xo[0], vuDiffer(t, c));
}
void MVHIST_Init(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint64_t p4)
{
    if (!ckBegin(131, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    if (!ckLeg([&] { ref::MVHIST_Init(rdram, &c, runtime); })) return;
    ckMid();
    uint64_t v0 = 0u;
    float f0 = 0.0f;
    uint64_t xo[2] = {Ps2GprU64(ctx, 5), Ps2GprU64(ctx, 6)};
    if (!ckLeg([&] { v0 = (ty::MVHIST_Init(rdram, runtime, p4, xo)); })) return;
    R5900Context &t = ckTyCtx(ctx);
    ckEnd(131, ctx, c, 2, v0, f0, 0x68u, 0x0u, Ps2GprU64(&c, 5) != xo[0] || Ps2GprU64(&c, 6) != xo[1], vuDiffer(t, c));
}
void get_predict_hit_position_nico_sight_on(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t p4, uint32_t p5)
{
    if (!ckBegin(132, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    if (!ckLeg([&] { ref::get_predict_hit_position_nico_sight_on(rdram, &c, runtime); })) return;
    ckMid();
    uint64_t v0 = 0u;
    float f0 = 0.0f;
    uint64_t v_h2 = a4::hi64(ctx->r[2]);
    if (!ckLeg([&] { v0 = (ty::get_predict_hit_position_nico_sight_on(rdram, runtime, p4, p5, v_h2)); })) return;
    R5900Context &t = ckTyCtx(ctx);
    t.r[2] = a4::mk128(v_h2, Ps2GprU64(&t, 2));
    ckEnd(132, ctx, c, 2, v0, f0, 0x0u, 0x0u, false, vuDiffer(t, c));
}
void AI_InitAnimalLayout(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t p4)
{
    if (!ckBegin(133, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    if (!ckLeg([&] { ref::AI_InitAnimalLayout(rdram, &c, runtime); })) return;
    ckMid();
    uint64_t v0 = 0u;
    float f0 = 0.0f;
    uint64_t v_h3 = a4::hi64(ctx->r[3]);
    if (!ckLeg([&] { v0 = a1_sx64(ty::AI_InitAnimalLayout(rdram, runtime, p4, v_h3)); })) return;
    R5900Context &t = ckTyCtx(ctx);
    t.r[3] = a4::mk128(v_h3, Ps2GprU64(&t, 3));
    ckEnd(133, ctx, c, 1, v0, f0, 0x8u, 0x0u, false, vuDiffer(t, c));
}
void AI_SetAnimalLayout(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t p4, uint32_t p5, float pf12, float pf13)
{
    if (!ckBegin(134, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    if (!ckLeg([&] { ref::AI_SetAnimalLayout(rdram, &c, runtime); })) return;
    ckMid();
    uint64_t v0 = 0u;
    float f0 = 0.0f;
    uint64_t v_h2 = a4::hi64(ctx->r[2]);
    if (!ckLeg([&] { v0 = (ty::AI_SetAnimalLayout(rdram, runtime, p4, p5, pf12, pf13, v_h2)); })) return;
    R5900Context &t = ckTyCtx(ctx);
    t.r[2] = a4::mk128(v_h2, Ps2GprU64(&t, 2));
    ckEnd(134, ctx, c, 2, v0, f0, 0x0u, 0x0u, false, vuDiffer(t, c));
}
void AI_CheckInAnimalLayout(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t p4, uint32_t p5)
{
    if (!ckBegin(135, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    if (!ckLeg([&] { ref::AI_CheckInAnimalLayout(rdram, &c, runtime); })) return;
    ckMid();
    uint64_t v0 = 0u;
    float f0 = 0.0f;
    a4::vec4 v_vf0 = ctx->vu0_vf[0];
    a4::vec4 v_vf5 = ctx->vu0_vf[5];
    a4::vec4 v_vf6 = ctx->vu0_vf[6];
    a4::vec4 v_vf7 = ctx->vu0_vf[7];
    a4::vec4 v_vacc = ctx->vu0_acc;
    float v_vq = ctx->vu0_q;
    uint16_t v_vstat = ctx->vu0_status;
    if (!ckLeg([&] { const a1::RetVF r = ty::AI_CheckInAnimalLayout(rdram, runtime, p4, p5, v_vf0, v_vf5, v_vf6, v_vf7, v_vacc, v_vq, v_vstat); v0 = r.v0; f0 = r.f0; })) return;
    R5900Context &t = ckTyCtx(ctx);
    t.vu0_vf[5] = v_vf5;
    t.vu0_vf[6] = v_vf6;
    t.vu0_vf[7] = v_vf7;
    t.vu0_acc = v_vacc;
    t.vu0_q = v_vq;
    t.vu0_status = v_vstat;
    ckEnd(135, ctx, c, 4, v0, f0, 0x0u, 0x6u, false, vuDiffer(t, c));
}
void AI_GetMotionBasicVelocity(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t p4, uint32_t p5, float pf12)
{
    if (!ckBegin(136, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    if (!ckLeg([&] { ref::AI_GetMotionBasicVelocity(rdram, &c, runtime); })) return;
    ckMid();
    uint64_t v0 = 0u;
    float f0 = 0.0f;
    a4::vec4 v_vf5 = ctx->vu0_vf[5];
    a4::vec4 v_vf6 = ctx->vu0_vf[6];
    uint64_t v_h2 = a4::hi64(ctx->r[2]);
    if (!ckLeg([&] { const a1::RetVF r = ty::AI_GetMotionBasicVelocity(rdram, runtime, p4, p5, pf12, v_vf5, v_vf6, v_h2); v0 = r.v0; f0 = r.f0; })) return;
    R5900Context &t = ckTyCtx(ctx);
    t.vu0_vf[5] = v_vf5;
    t.vu0_vf[6] = v_vf6;
    ckEnd(136, ctx, c, 4, v0, f0, 0x2u, 0x2u, false, vuDiffer(t, c));
}
void AI_GetAccelA3XYZ(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t p4, uint32_t p5, uint32_t p6, uint32_t p7, uint32_t p8, uint32_t p9, float pf12)
{
    if (!ckBegin(137, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    if (!ckLeg([&] { ref::AI_GetAccelA3XYZ(rdram, &c, runtime); })) return;
    ckMid();
    uint64_t v0 = 0u;
    float f0 = 0.0f;
    a4::vec4 v_vf5 = ctx->vu0_vf[5];
    a4::vec4 v_vf6 = ctx->vu0_vf[6];
    a4::vec4 v_vf7 = ctx->vu0_vf[7];
    uint64_t v_h2 = a4::hi64(ctx->r[2]);
    if (!ckLeg([&] { const a1::RetVF r = ty::AI_GetAccelA3XYZ(rdram, runtime, p4, p5, p6, p7, p8, p9, pf12, v_vf5, v_vf6, v_vf7, v_h2); v0 = r.v0; f0 = r.f0; })) return;
    R5900Context &t = ckTyCtx(ctx);
    t.vu0_vf[5] = v_vf5;
    t.vu0_vf[6] = v_vf6;
    t.vu0_vf[7] = v_vf7;
    ckEnd(137, ctx, c, 4, v0, f0, 0x2u, 0x100eu, false, vuDiffer(t, c));
}
void AI_GetAccelB3XYZ(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t p4, uint32_t p5, uint32_t p6, uint32_t p7, uint32_t p8, uint32_t p9, float pf12)
{
    if (!ckBegin(138, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    if (!ckLeg([&] { ref::AI_GetAccelB3XYZ(rdram, &c, runtime); })) return;
    ckMid();
    uint64_t v0 = 0u;
    float f0 = 0.0f;
    a4::vec4 v_vf5 = ctx->vu0_vf[5];
    a4::vec4 v_vf6 = ctx->vu0_vf[6];
    a4::vec4 v_vf7 = ctx->vu0_vf[7];
    uint64_t v_h2 = a4::hi64(ctx->r[2]);
    if (!ckLeg([&] { const a1::RetVF r = ty::AI_GetAccelB3XYZ(rdram, runtime, p4, p5, p6, p7, p8, p9, pf12, v_vf5, v_vf6, v_vf7, v_h2); v0 = r.v0; f0 = r.f0; })) return;
    R5900Context &t = ckTyCtx(ctx);
    t.vu0_vf[5] = v_vf5;
    t.vu0_vf[6] = v_vf6;
    t.vu0_vf[7] = v_vf7;
    ckEnd(138, ctx, c, 4, v0, f0, 0x2u, 0x1002u, false, vuDiffer(t, c));
}
void AI_GetAccelC3XYZ(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t p4, uint32_t p5, uint32_t p6, uint32_t p7, uint32_t p8, uint32_t p9, float pf12)
{
    if (!ckBegin(139, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    if (!ckLeg([&] { ref::AI_GetAccelC3XYZ(rdram, &c, runtime); })) return;
    ckMid();
    uint64_t v0 = 0u;
    float f0 = 0.0f;
    a4::vec4 v_vf5 = ctx->vu0_vf[5];
    a4::vec4 v_vf6 = ctx->vu0_vf[6];
    a4::vec4 v_vf7 = ctx->vu0_vf[7];
    uint64_t v_h2 = a4::hi64(ctx->r[2]);
    if (!ckLeg([&] { const a1::RetVF r = ty::AI_GetAccelC3XYZ(rdram, runtime, p4, p5, p6, p7, p8, p9, pf12, v_vf5, v_vf6, v_vf7, v_h2); v0 = r.v0; f0 = r.f0; })) return;
    R5900Context &t = ckTyCtx(ctx);
    t.vu0_vf[5] = v_vf5;
    t.vu0_vf[6] = v_vf6;
    t.vu0_vf[7] = v_vf7;
    ckEnd(139, ctx, c, 4, v0, f0, 0x2u, 0x1000u, false, vuDiffer(t, c));
}
void AI_GetVelocity3XYZ(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t p4, uint32_t p5, uint32_t p6, uint32_t p7, uint32_t p8, float pf12)
{
    if (!ckBegin(140, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    if (!ckLeg([&] { ref::AI_GetVelocity3XYZ(rdram, &c, runtime); })) return;
    ckMid();
    uint64_t v0 = 0u;
    float f0 = 0.0f;
    a4::vec4 v_vf5 = ctx->vu0_vf[5];
    a4::vec4 v_vf6 = ctx->vu0_vf[6];
    a4::vec4 v_vf7 = ctx->vu0_vf[7];
    uint64_t v_h2 = a4::hi64(ctx->r[2]);
    if (!ckLeg([&] { const a1::RetVF r = ty::AI_GetVelocity3XYZ(rdram, runtime, p4, p5, p6, p7, p8, pf12, v_vf5, v_vf6, v_vf7, v_h2); v0 = r.v0; f0 = r.f0; })) return;
    R5900Context &t = ckTyCtx(ctx);
    t.vu0_vf[5] = v_vf5;
    t.vu0_vf[6] = v_vf6;
    t.vu0_vf[7] = v_vf7;
    ckEnd(140, ctx, c, 4, v0, f0, 0x2u, 0x6u, false, vuDiffer(t, c));
}
void AI_GetPosition3XYZ(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t p4, uint32_t p5, uint32_t p6, uint32_t p7, uint32_t p8, uint32_t p9, float pf12)
{
    if (!ckBegin(141, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    if (!ckLeg([&] { ref::AI_GetPosition3XYZ(rdram, &c, runtime); })) return;
    ckMid();
    uint64_t v0 = 0u;
    float f0 = 0.0f;
    a4::vec4 v_vf5 = ctx->vu0_vf[5];
    a4::vec4 v_vf6 = ctx->vu0_vf[6];
    a4::vec4 v_vf7 = ctx->vu0_vf[7];
    uint64_t v_h2 = a4::hi64(ctx->r[2]);
    if (!ckLeg([&] { const a1::RetVF r = ty::AI_GetPosition3XYZ(rdram, runtime, p4, p5, p6, p7, p8, p9, pf12, v_vf5, v_vf6, v_vf7, v_h2); v0 = r.v0; f0 = r.f0; })) return;
    R5900Context &t = ckTyCtx(ctx);
    t.vu0_vf[5] = v_vf5;
    t.vu0_vf[6] = v_vf6;
    t.vu0_vf[7] = v_vf7;
    ckEnd(141, ctx, c, 4, v0, f0, 0x2u, 0xeu, false, vuDiffer(t, c));
}
void AI_GetTrgDir(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t p4, uint32_t p5, float pf12)
{
    if (!ckBegin(142, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    if (!ckLeg([&] { ref::AI_GetTrgDir(rdram, &c, runtime); })) return;
    ckMid();
    uint64_t v0 = 0u;
    float f0 = 0.0f;
    a4::vec4 v_vf0 = ctx->vu0_vf[0];
    a4::vec4 v_vf5 = ctx->vu0_vf[5];
    a4::vec4 v_vf7 = ctx->vu0_vf[7];
    a4::vec4 v_vacc = ctx->vu0_acc;
    float v_vq = ctx->vu0_q;
    uint16_t v_vstat = ctx->vu0_status;
    uint64_t v_h2 = a4::hi64(ctx->r[2]);
    if (!ckLeg([&] { f0 = ty::AI_GetTrgDir(rdram, runtime, p4, p5, pf12, v_vf0, v_vf5, v_vf7, v_vacc, v_vq, v_vstat, v_h2); })) return;
    R5900Context &t = ckTyCtx(ctx);
    t.vu0_vf[5] = v_vf5;
    t.vu0_vf[7] = v_vf7;
    t.vu0_acc = v_vacc;
    t.vu0_q = v_vq;
    t.vu0_status = v_vstat;
    t.r[2] = a4::mk128(v_h2, Ps2GprU64(&t, 2));
    ckEnd(142, ctx, c, 3, v0, f0, 0x4u, 0xeu, false, vuDiffer(t, c));
}
void AI_GetSideSignalFromDirLev(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t p4, uint32_t p5)
{
    if (!ckBegin(143, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    if (!ckLeg([&] { ref::AI_GetSideSignalFromDirLev(rdram, &c, runtime); })) return;
    ckMid();
    uint64_t v0 = 0u;
    float f0 = 0.0f;
    a4::vec4 v_vf5 = ctx->vu0_vf[5];
    a4::vec4 v_vf6 = ctx->vu0_vf[6];
    a4::vec4 v_vf7 = ctx->vu0_vf[7];
    a4::vec4 v_vacc = ctx->vu0_acc;
    if (!ckLeg([&] { const a1::RetVF r = ty::AI_GetSideSignalFromDirLev(rdram, runtime, p4, p5, v_vf5, v_vf6, v_vf7, v_vacc); v0 = r.v0; f0 = r.f0; })) return;
    R5900Context &t = ckTyCtx(ctx);
    t.vu0_vf[5] = v_vf5;
    t.vu0_vf[6] = v_vf6;
    t.vu0_vf[7] = v_vf7;
    t.vu0_acc = v_vacc;
    ckEnd(143, ctx, c, 4, v0, f0, 0x0u, 0x2u, false, vuDiffer(t, c));
}
void gamecore_sub_013D8DC8(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint64_t p4)
{
    if (!ckBegin(144, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    if (!ckLeg([&] { ref::gamecore_sub_013D8DC8(rdram, &c, runtime); })) return;
    ckMid();
    uint64_t v0 = 0u;
    float f0 = 0.0f;
    a4::vec4 v_vf0 = ctx->vu0_vf[0];
    a4::vec4 v_vf5 = ctx->vu0_vf[5];
    a4::vec4 v_vf6 = ctx->vu0_vf[6];
    a4::vec4 v_vf7 = ctx->vu0_vf[7];
    a4::vec4 v_vacc = ctx->vu0_acc;
    float v_vq = ctx->vu0_q;
    uint16_t v_vstat = ctx->vu0_status;
    if (!ckLeg([&] { const a1::RetVF r = ty::gamecore_sub_013D8DC8(rdram, runtime, p4, v_vf0, v_vf5, v_vf6, v_vf7, v_vacc, v_vq, v_vstat); v0 = r.v0; f0 = r.f0; })) return;
    R5900Context &t = ckTyCtx(ctx);
    t.vu0_vf[5] = v_vf5;
    t.vu0_vf[6] = v_vf6;
    t.vu0_vf[7] = v_vf7;
    t.vu0_acc = v_vacc;
    t.vu0_q = v_vq;
    t.vu0_status = v_vstat;
    ckEnd(144, ctx, c, 4, v0, f0, 0x3au, 0x1eu, false, vuDiffer(t, c));
}
void gamecore_sub_013D8FA0(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint64_t p4)
{
    if (!ckBegin(145, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    if (!ckLeg([&] { ref::gamecore_sub_013D8FA0(rdram, &c, runtime); })) return;
    ckMid();
    uint64_t v0 = 0u;
    float f0 = 0.0f;
    a4::vec4 v_vf0 = ctx->vu0_vf[0];
    a4::vec4 v_vf5 = ctx->vu0_vf[5];
    a4::vec4 v_vf6 = ctx->vu0_vf[6];
    a4::vec4 v_vf7 = ctx->vu0_vf[7];
    a4::vec4 v_vacc = ctx->vu0_acc;
    float v_vq = ctx->vu0_q;
    uint16_t v_vstat = ctx->vu0_status;
    uint64_t xo[1] = {(uint64_t)a1ck::ckBits(ctx->f[0])};
    if (!ckLeg([&] { v0 = a1_sx64(ty::gamecore_sub_013D8FA0(rdram, runtime, p4, v_vf0, v_vf5, v_vf6, v_vf7, v_vacc, v_vq, v_vstat, xo)); })) return;
    R5900Context &t = ckTyCtx(ctx);
    t.vu0_vf[5] = v_vf5;
    t.vu0_vf[6] = v_vf6;
    t.vu0_vf[7] = v_vf7;
    t.vu0_acc = v_vacc;
    t.vu0_q = v_vq;
    t.vu0_status = v_vstat;
    ckEnd(145, ctx, c, 1, v0, f0, 0x3au, 0xfu, a1ck::ckBits(c.f[0]) != (uint32_t)xo[0], vuDiffer(t, c));
}
void gamecore_sub_013D9518(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint64_t p4)
{
    if (!ckBegin(146, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    if (!ckLeg([&] { ref::gamecore_sub_013D9518(rdram, &c, runtime); })) return;
    ckMid();
    uint64_t v0 = 0u;
    float f0 = 0.0f;
    a4::vec4 v_vf0 = ctx->vu0_vf[0];
    a4::vec4 v_vf5 = ctx->vu0_vf[5];
    a4::vec4 v_vf6 = ctx->vu0_vf[6];
    a4::vec4 v_vf7 = ctx->vu0_vf[7];
    a4::vec4 v_vacc = ctx->vu0_acc;
    float v_vq = ctx->vu0_q;
    uint16_t v_vstat = ctx->vu0_status;
    uint64_t xo[1] = {(uint64_t)a1ck::ckBits(ctx->f[0])};
    if (!ckLeg([&] { v0 = (ty::gamecore_sub_013D9518(rdram, runtime, p4, v_vf0, v_vf5, v_vf6, v_vf7, v_vacc, v_vq, v_vstat, xo)); })) return;
    R5900Context &t = ckTyCtx(ctx);
    t.vu0_vf[5] = v_vf5;
    t.vu0_vf[6] = v_vf6;
    t.vu0_vf[7] = v_vf7;
    t.vu0_acc = v_vacc;
    t.vu0_q = v_vq;
    t.vu0_status = v_vstat;
    ckEnd(146, ctx, c, 2, v0, f0, 0x7au, 0x7u, a1ck::ckBits(c.f[0]) != (uint32_t)xo[0], vuDiffer(t, c));
}
void Kite_InitKitePathPoint(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t p4)
{
    if (!ckBegin(147, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    if (!ckLeg([&] { ref::Kite_InitKitePathPoint(rdram, &c, runtime); })) return;
    ckMid();
    uint64_t v0 = 0u;
    float f0 = 0.0f;
    a4::vec4 v_vf0 = ctx->vu0_vf[0];
    if (!ckLeg([&] { v0 = a1_sx64(ty::Kite_InitKitePathPoint(rdram, runtime, p4, v_vf0)); })) return;
    R5900Context &t = ckTyCtx(ctx);
    ckEnd(147, ctx, c, 1, v0, f0, 0x0u, 0x0u, false, vuDiffer(t, c));
}
void Kite_SetAnimalLayout(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t p4, uint32_t p5, float pf12, float pf13, float pf14)
{
    if (!ckBegin(148, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    if (!ckLeg([&] { ref::Kite_SetAnimalLayout(rdram, &c, runtime); })) return;
    ckMid();
    uint64_t v0 = 0u;
    float f0 = 0.0f;
    uint64_t v_h2 = a4::hi64(ctx->r[2]);
    uint64_t xo[1] = {(uint64_t)a1ck::ckBits(ctx->f[14])};
    if (!ckLeg([&] { const a1::RetVF r = ty::Kite_SetAnimalLayout(rdram, runtime, p4, p5, pf12, pf13, pf14, v_h2, xo); v0 = r.v0; f0 = r.f0; })) return;
    R5900Context &t = ckTyCtx(ctx);
    t.r[2] = a4::mk128(v_h2, Ps2GprU64(&t, 2));
    ckEnd(148, ctx, c, 4, v0, f0, 0x0u, 0x4002u, a1ck::ckBits(c.f[14]) != (uint32_t)xo[0], vuDiffer(t, c));
}
void Kite_SetPathOfs(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t p4, uint32_t p5)
{
    if (!ckBegin(149, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    if (!ckLeg([&] { ref::Kite_SetPathOfs(rdram, &c, runtime); })) return;
    ckMid();
    uint64_t v0 = 0u;
    float f0 = 0.0f;
    uint64_t v_h2 = a4::hi64(ctx->r[2]);
    uint64_t v_h3 = a4::hi64(ctx->r[3]);
    if (!ckLeg([&] { v0 = (ty::Kite_SetPathOfs(rdram, runtime, p4, p5, v_h2, v_h3)); })) return;
    R5900Context &t = ckTyCtx(ctx);
    t.r[2] = a4::mk128(v_h2, Ps2GprU64(&t, 2));
    t.r[3] = a4::mk128(v_h3, Ps2GprU64(&t, 3));
    ckEnd(149, ctx, c, 2, v0, f0, 0x8u, 0x0u, false, vuDiffer(t, c));
}
void sprWeaponSetMarker(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint64_t p4, uint32_t p5, uint64_t p6)
{
    if (!ckBegin(150, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    if (!ckLeg([&] { ref::sprWeaponSetMarker(rdram, &c, runtime); })) return;
    ckMid();
    uint64_t v0 = 0u;
    float f0 = 0.0f;
    uint64_t v_h2 = a4::hi64(ctx->r[2]);
    uint64_t v_h4 = a4::hi64(ctx->r[4]);
    if (!ckLeg([&] { v0 = (ty::sprWeaponSetMarker(rdram, runtime, p4, p5, p6, v_h2, v_h4)); })) return;
    R5900Context &t = ckTyCtx(ctx);
    t.r[2] = a4::mk128(v_h2, Ps2GprU64(&t, 2));
    t.r[4] = a4::mk128(v_h4, Ps2GprU64(&t, 4));
    ckEnd(150, ctx, c, 2, v0, f0, 0x18u, 0x0u, false, vuDiffer(t, c));
}
void sprWeaponGetMarkerPos(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t p4)
{
    if (!ckBegin(151, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    if (!ckLeg([&] { ref::sprWeaponGetMarkerPos(rdram, &c, runtime); })) return;
    ckMid();
    uint64_t v0 = 0u;
    float f0 = 0.0f;
    uint64_t v_h3 = a4::hi64(ctx->r[3]);
    if (!ckLeg([&] { v0 = a1_sx64(ty::sprWeaponGetMarkerPos(rdram, runtime, p4, v_h3)); })) return;
    R5900Context &t = ckTyCtx(ctx);
    t.r[3] = a4::mk128(v_h3, Ps2GprU64(&t, 3));
    ckEnd(151, ctx, c, 1, v0, f0, 0x8u, 0x0u, false, vuDiffer(t, c));
}
void gamecore_sub_01411708(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t p4, uint32_t p5)
{
    if (!ckBegin(152, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    if (!ckLeg([&] { ref::gamecore_sub_01411708(rdram, &c, runtime); })) return;
    ckMid();
    uint64_t v0 = 0u;
    float f0 = 0.0f;
    uint64_t v_h2 = a4::hi64(ctx->r[2]);
    uint64_t v_h3 = a4::hi64(ctx->r[3]);
    if (!ckLeg([&] { v0 = (ty::gamecore_sub_01411708(rdram, runtime, p4, p5, v_h2, v_h3)); })) return;
    R5900Context &t = ckTyCtx(ctx);
    t.r[2] = a4::mk128(v_h2, Ps2GprU64(&t, 2));
    t.r[3] = a4::mk128(v_h3, Ps2GprU64(&t, 3));
    ckEnd(152, ctx, c, 2, v0, f0, 0x28u, 0x0u, false, vuDiffer(t, c));
}
void gamecore_sub_01411AF8(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t p4)
{
    if (!ckBegin(153, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    if (!ckLeg([&] { ref::gamecore_sub_01411AF8(rdram, &c, runtime); })) return;
    ckMid();
    uint64_t v0 = 0u;
    float f0 = 0.0f;
    uint64_t v_h3 = a4::hi64(ctx->r[3]);
    if (!ckLeg([&] { v0 = a1_sx64(ty::gamecore_sub_01411AF8(rdram, runtime, p4, v_h3)); })) return;
    R5900Context &t = ckTyCtx(ctx);
    t.r[3] = a4::mk128(v_h3, Ps2GprU64(&t, 3));
    ckEnd(153, ctx, c, 1, v0, f0, 0x8u, 0x0u, false, vuDiffer(t, c));
}
void constructor_camera_pause_rise(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t p4)
{
    if (!ckBegin(154, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    if (!ckLeg([&] { ref::constructor_camera_pause_rise(rdram, &c, runtime); })) return;
    ckMid();
    uint64_t v0 = 0u;
    float f0 = 0.0f;
    uint64_t v_h2 = a4::hi64(ctx->r[2]);
    uint64_t v_h3 = a4::hi64(ctx->r[3]);
    uint64_t xo[2] = {Ps2GprU64(ctx, 5), Ps2GprU64(ctx, 6)};
    if (!ckLeg([&] { const a1::RetVF r = ty::constructor_camera_pause_rise(rdram, runtime, p4, v_h2, v_h3, xo); v0 = r.v0; f0 = r.f0; })) return;
    R5900Context &t = ckTyCtx(ctx);
    t.r[2] = a4::mk128(v_h2, Ps2GprU64(&t, 2));
    t.r[3] = a4::mk128(v_h3, Ps2GprU64(&t, 3));
    ckEnd(154, ctx, c, 4, v0, f0, 0x6au, 0x2u, Ps2GprU64(&c, 5) != xo[0] || Ps2GprU64(&c, 6) != xo[1], vuDiffer(t, c));
}
void gamecore_sub_01413720(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t p4)
{
    if (!ckBegin(155, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    if (!ckLeg([&] { ref::gamecore_sub_01413720(rdram, &c, runtime); })) return;
    ckMid();
    uint64_t v0 = 0u;
    float f0 = 0.0f;
    uint64_t v_h2 = a4::hi64(ctx->r[2]);
    uint64_t v_h3 = a4::hi64(ctx->r[3]);
    uint64_t v_h5 = a4::hi64(ctx->r[5]);
    if (!ckLeg([&] { const a1::RetVF r = ty::gamecore_sub_01413720(rdram, runtime, p4, v_h2, v_h3, v_h5); v0 = r.v0; f0 = r.f0; })) return;
    R5900Context &t = ckTyCtx(ctx);
    t.r[2] = a4::mk128(v_h2, Ps2GprU64(&t, 2));
    t.r[3] = a4::mk128(v_h3, Ps2GprU64(&t, 3));
    t.r[5] = a4::mk128(v_h5, Ps2GprU64(&t, 5));
    ckEnd(155, ctx, c, 4, v0, f0, 0x6au, 0x0u, false, vuDiffer(t, c));
}
void constructor_camera_target_lost_angle(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t p4)
{
    if (!ckBegin(156, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    if (!ckLeg([&] { ref::constructor_camera_target_lost_angle(rdram, &c, runtime); })) return;
    ckMid();
    uint64_t v0 = 0u;
    float f0 = 0.0f;
    uint64_t v_h2 = a4::hi64(ctx->r[2]);
    uint64_t v_h5 = a4::hi64(ctx->r[5]);
    if (!ckLeg([&] { const a1::RetVF r = ty::constructor_camera_target_lost_angle(rdram, runtime, p4, v_h2, v_h5); v0 = r.v0; f0 = r.f0; })) return;
    R5900Context &t = ckTyCtx(ctx);
    t.r[2] = a4::mk128(v_h2, Ps2GprU64(&t, 2));
    t.r[5] = a4::mk128(v_h5, Ps2GprU64(&t, 5));
    ckEnd(156, ctx, c, 4, v0, f0, 0x28u, 0x0u, false, vuDiffer(t, c));
}
void reset_camera_target_visible_angle_manager(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t p4)
{
    if (!ckBegin(157, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    if (!ckLeg([&] { ref::reset_camera_target_visible_angle_manager(rdram, &c, runtime); })) return;
    ckMid();
    uint64_t v0 = 0u;
    float f0 = 0.0f;
    uint64_t v_h2 = a4::hi64(ctx->r[2]);
    uint64_t v_h3 = a4::hi64(ctx->r[3]);
    if (!ckLeg([&] { const a1::RetVF r = ty::reset_camera_target_visible_angle_manager(rdram, runtime, p4, v_h2, v_h3); v0 = r.v0; f0 = r.f0; })) return;
    R5900Context &t = ckTyCtx(ctx);
    t.r[2] = a4::mk128(v_h2, Ps2GprU64(&t, 2));
    t.r[3] = a4::mk128(v_h3, Ps2GprU64(&t, 3));
    ckEnd(157, ctx, c, 4, v0, f0, 0xe8u, 0x0u, false, vuDiffer(t, c));
}
void CAM_get_camera_position_by_look_at(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t p4, uint32_t p5, uint32_t p6, float pf12)
{
    if (!ckBegin(158, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    if (!ckLeg([&] { ref::CAM_get_camera_position_by_look_at(rdram, &c, runtime); })) return;
    ckMid();
    uint64_t v0 = 0u;
    float f0 = 0.0f;
    a4::vec4 v_vf5 = ctx->vu0_vf[5];
    a4::vec4 v_vf6 = ctx->vu0_vf[6];
    uint64_t v_h2 = a4::hi64(ctx->r[2]);
    uint64_t xo[1] = {(uint64_t)a1ck::ckBits(ctx->f[12])};
    if (!ckLeg([&] { v0 = a1_sx64(ty::CAM_get_camera_position_by_look_at(rdram, runtime, p4, p5, p6, pf12, v_vf5, v_vf6, v_h2, xo)); })) return;
    R5900Context &t = ckTyCtx(ctx);
    t.vu0_vf[5] = v_vf5;
    t.vu0_vf[6] = v_vf6;
    ckEnd(158, ctx, c, 1, v0, f0, 0x0u, 0x1000u, a1ck::ckBits(c.f[12]) != (uint32_t)xo[0], vuDiffer(t, c));
}
void CAM_add_plane_offset_in_position(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t p4, uint32_t p5, uint32_t p6, float pf12)
{
    if (!ckBegin(159, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    if (!ckLeg([&] { ref::CAM_add_plane_offset_in_position(rdram, &c, runtime); })) return;
    ckMid();
    uint64_t v0 = 0u;
    float f0 = 0.0f;
    a4::vec4 v_vf5 = ctx->vu0_vf[5];
    a4::vec4 v_vf6 = ctx->vu0_vf[6];
    uint64_t v_h2 = a4::hi64(ctx->r[2]);
    if (!ckLeg([&] { v0 = a1_sx64(ty::CAM_add_plane_offset_in_position(rdram, runtime, p4, p5, p6, pf12, v_vf5, v_vf6, v_h2)); })) return;
    R5900Context &t = ckTyCtx(ctx);
    t.vu0_vf[5] = v_vf5;
    t.vu0_vf[6] = v_vf6;
    ckEnd(159, ctx, c, 1, v0, f0, 0x0u, 0x0u, false, vuDiffer(t, c));
}
void CAM_set_all_camera_base_data(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t p4, uint32_t p5, uint32_t p6, float pf12, float pf13, float pf14)
{
    if (!ckBegin(160, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    if (!ckLeg([&] { ref::CAM_set_all_camera_base_data(rdram, &c, runtime); })) return;
    ckMid();
    uint64_t v0 = 0u;
    float f0 = 0.0f;
    uint64_t v_h2 = a4::hi64(ctx->r[2]);
    uint64_t v_h3 = a4::hi64(ctx->r[3]);
    if (!ckLeg([&] { v0 = (ty::CAM_set_all_camera_base_data(rdram, runtime, p4, p5, p6, pf12, pf13, pf14, v_h2, v_h3)); })) return;
    R5900Context &t = ckTyCtx(ctx);
    t.r[2] = a4::mk128(v_h2, Ps2GprU64(&t, 2));
    t.r[3] = a4::mk128(v_h3, Ps2GprU64(&t, 3));
    ckEnd(160, ctx, c, 2, v0, f0, 0x8u, 0x0u, false, vuDiffer(t, c));
}
void CAM_constructor_base_look_at(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t p4)
{
    if (!ckBegin(161, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    if (!ckLeg([&] { ref::CAM_constructor_base_look_at(rdram, &c, runtime); })) return;
    ckMid();
    uint64_t v0 = 0u;
    float f0 = 0.0f;
    uint64_t v_h2 = a4::hi64(ctx->r[2]);
    uint64_t v_h3 = a4::hi64(ctx->r[3]);
    uint64_t xo[1] = {Ps2GprU64(ctx, 6)};
    if (!ckLeg([&] { const a1::RetVF r = ty::CAM_constructor_base_look_at(rdram, runtime, p4, v_h2, v_h3, xo); v0 = r.v0; f0 = r.f0; })) return;
    R5900Context &t = ckTyCtx(ctx);
    t.r[2] = a4::mk128(v_h2, Ps2GprU64(&t, 2));
    t.r[3] = a4::mk128(v_h3, Ps2GprU64(&t, 3));
    ckEnd(161, ctx, c, 4, v0, f0, 0x68u, 0x0u, Ps2GprU64(&c, 6) != xo[0], vuDiffer(t, c));
}
void copy_mode_common_camera_data(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t p4, uint32_t p5)
{
    if (!ckBegin(162, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    if (!ckLeg([&] { ref::copy_mode_common_camera_data(rdram, &c, runtime); })) return;
    ckMid();
    uint64_t v0 = 0u;
    float f0 = 0.0f;
    uint64_t v_h2 = a4::hi64(ctx->r[2]);
    uint64_t v_h3 = a4::hi64(ctx->r[3]);
    if (!ckLeg([&] { const a1::RetVF r = ty::copy_mode_common_camera_data(rdram, runtime, p4, p5, v_h2, v_h3); v0 = r.v0; f0 = r.f0; })) return;
    R5900Context &t = ckTyCtx(ctx);
    t.r[2] = a4::mk128(v_h2, Ps2GprU64(&t, 2));
    t.r[3] = a4::mk128(v_h3, Ps2GprU64(&t, 3));
    ckEnd(162, ctx, c, 4, v0, f0, 0x8u, 0x2u, false, vuDiffer(t, c));
}
void get_boss_interest_position_game_camera(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t p4, uint32_t p5)
{
    if (!ckBegin(163, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    if (!ckLeg([&] { ref::get_boss_interest_position_game_camera(rdram, &c, runtime); })) return;
    ckMid();
    uint64_t v0 = 0u;
    float f0 = 0.0f;
    uint64_t v_h3 = a4::hi64(ctx->r[3]);
    if (!ckLeg([&] { v0 = a1_sx64(ty::get_boss_interest_position_game_camera(rdram, runtime, p4, p5, v_h3)); })) return;
    R5900Context &t = ckTyCtx(ctx);
    t.r[3] = a4::mk128(v_h3, Ps2GprU64(&t, 3));
    ckEnd(163, ctx, c, 1, v0, f0, 0x8u, 0x0u, false, vuDiffer(t, c));
}
void setAttainRangeData(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t p4, uint32_t p5, float pf12, float pf13, float pf14, float pf15)
{
    if (!ckBegin(164, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    if (!ckLeg([&] { ref::setAttainRangeData(rdram, &c, runtime); })) return;
    ckMid();
    uint64_t v0 = 0u;
    float f0 = 0.0f;
    uint64_t v_h2 = a4::hi64(ctx->r[2]);
    if (!ckLeg([&] { v0 = (ty::setAttainRangeData(rdram, runtime, p4, p5, pf12, pf13, pf14, pf15, v_h2)); })) return;
    R5900Context &t = ckTyCtx(ctx);
    t.r[2] = a4::mk128(v_h2, Ps2GprU64(&t, 2));
    ckEnd(164, ctx, c, 2, v0, f0, 0x0u, 0x0u, false, vuDiffer(t, c));
}
void gamecore_sub_01433F98(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint64_t p4, uint32_t p5, uint32_t p6, uint32_t p7, uint32_t p8)
{
    if (!ckBegin(165, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    if (!ckLeg([&] { ref::gamecore_sub_01433F98(rdram, &c, runtime); })) return;
    ckMid();
    uint64_t v0 = 0u;
    float f0 = 0.0f;
    uint64_t v_h3 = a4::hi64(ctx->r[3]);
    uint64_t xo[2] = {Ps2GprU64(ctx, 9), Ps2GprU64(ctx, 10)};
    if (!ckLeg([&] { v0 = (ty::gamecore_sub_01433F98(rdram, runtime, p4, p5, p6, p7, p8, v_h3, xo)); })) return;
    R5900Context &t = ckTyCtx(ctx);
    t.r[3] = a4::mk128(v_h3, Ps2GprU64(&t, 3));
    ckEnd(165, ctx, c, 2, v0, f0, 0x618u, 0x0u, Ps2GprU64(&c, 9) != xo[0] || Ps2GprU64(&c, 10) != xo[1], vuDiffer(t, c));
}
void gamecore_sub_014340F0(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t p4)
{
    if (!ckBegin(166, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    if (!ckLeg([&] { ref::gamecore_sub_014340F0(rdram, &c, runtime); })) return;
    ckMid();
    uint64_t v0 = 0u;
    float f0 = 0.0f;
    uint64_t v_h2 = a4::hi64(ctx->r[2]);
    if (!ckLeg([&] { v0 = (ty::gamecore_sub_014340F0(rdram, runtime, p4, v_h2)); })) return;
    R5900Context &t = ckTyCtx(ctx);
    t.r[2] = a4::mk128(v_h2, Ps2GprU64(&t, 2));
    ckEnd(166, ctx, c, 2, v0, f0, 0x8u, 0x0u, false, vuDiffer(t, c));
}
void gamecore_sub_0143D718(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t p4)
{
    if (!ckBegin(167, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    if (!ckLeg([&] { ref::gamecore_sub_0143D718(rdram, &c, runtime); })) return;
    ckMid();
    uint64_t v0 = 0u;
    float f0 = 0.0f;
    a4::vec4 v_vf0 = ctx->vu0_vf[0];
    if (!ckLeg([&] { v0 = a1_sx64(ty::gamecore_sub_0143D718(rdram, runtime, p4, v_vf0)); })) return;
    R5900Context &t = ckTyCtx(ctx);
    ckEnd(167, ctx, c, 1, v0, f0, 0x10u, 0x0u, false, vuDiffer(t, c));
}
void gamecore_sub_01440FE0(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t p4)
{
    if (!ckBegin(168, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    if (!ckLeg([&] { ref::gamecore_sub_01440FE0(rdram, &c, runtime); })) return;
    ckMid();
    uint64_t v0 = 0u;
    float f0 = 0.0f;
    uint64_t v_h2 = a4::hi64(ctx->r[2]);
    if (!ckLeg([&] { const a1::RetVF r = ty::gamecore_sub_01440FE0(rdram, runtime, p4, v_h2); v0 = r.v0; f0 = r.f0; })) return;
    R5900Context &t = ckTyCtx(ctx);
    t.r[2] = a4::mk128(v_h2, Ps2GprU64(&t, 2));
    ckEnd(168, ctx, c, 4, v0, f0, 0x28u, 0x2u, false, vuDiffer(t, c));
}
void setPosGazeCtrlWork(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t p4, uint32_t p5)
{
    if (!ckBegin(169, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    if (!ckLeg([&] { ref::setPosGazeCtrlWork(rdram, &c, runtime); })) return;
    ckMid();
    uint64_t v0 = 0u;
    float f0 = 0.0f;
    uint64_t v_h2 = a4::hi64(ctx->r[2]);
    if (!ckLeg([&] { v0 = (ty::setPosGazeCtrlWork(rdram, runtime, p4, p5, v_h2)); })) return;
    R5900Context &t = ckTyCtx(ctx);
    t.r[2] = a4::mk128(v_h2, Ps2GprU64(&t, 2));
    ckEnd(169, ctx, c, 2, v0, f0, 0x0u, 0x0u, false, vuDiffer(t, c));
}
void gamecore_sub_01444568(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t p4)
{
    if (!ckBegin(170, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    if (!ckLeg([&] { ref::gamecore_sub_01444568(rdram, &c, runtime); })) return;
    ckMid();
    uint64_t v0 = 0u;
    float f0 = 0.0f;
    uint64_t v_h2 = a4::hi64(ctx->r[2]);
    if (!ckLeg([&] { const a1::RetVF r = ty::gamecore_sub_01444568(rdram, runtime, p4, v_h2); v0 = r.v0; f0 = r.f0; })) return;
    R5900Context &t = ckTyCtx(ctx);
    t.r[2] = a4::mk128(v_h2, Ps2GprU64(&t, 2));
    ckEnd(170, ctx, c, 4, v0, f0, 0x28u, 0x2u, false, vuDiffer(t, c));
}
void resetVelocityWormCtrlWork(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t p5)
{
    if (!ckBegin(171, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    if (!ckLeg([&] { ref::resetVelocityWormCtrlWork(rdram, &c, runtime); })) return;
    ckMid();
    uint64_t v0 = 0u;
    float f0 = 0.0f;
    a4::vec4 v_vf0 = ctx->vu0_vf[0];
    if (!ckLeg([&] { v0 = a1_sx64(ty::resetVelocityWormCtrlWork(rdram, runtime, p5, v_vf0)); })) return;
    R5900Context &t = ckTyCtx(ctx);
    ckEnd(171, ctx, c, 1, v0, f0, 0x0u, 0x0u, false, vuDiffer(t, c));
}
void initAimIKWork(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t p6)
{
    if (!ckBegin(172, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    if (!ckLeg([&] { ref::initAimIKWork(rdram, &c, runtime); })) return;
    ckMid();
    uint64_t v0 = 0u;
    float f0 = 0.0f;
    a4::vec4 v_vf0 = ctx->vu0_vf[0];
    if (!ckLeg([&] { const a1::RetVF r = ty::initAimIKWork(rdram, runtime, p6, v_vf0); v0 = r.v0; f0 = r.f0; })) return;
    R5900Context &t = ckTyCtx(ctx);
    ckEnd(172, ctx, c, 4, v0, f0, 0x2u, 0x0u, false, vuDiffer(t, c));
}
void initAttackWaistIKWork(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t p6)
{
    if (!ckBegin(173, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    if (!ckLeg([&] { ref::initAttackWaistIKWork(rdram, &c, runtime); })) return;
    ckMid();
    uint64_t v0 = 0u;
    float f0 = 0.0f;
    a4::vec4 v_vf0 = ctx->vu0_vf[0];
    if (!ckLeg([&] { const a1::RetVF r = ty::initAttackWaistIKWork(rdram, runtime, p6, v_vf0); v0 = r.v0; f0 = r.f0; })) return;
    R5900Context &t = ckTyCtx(ctx);
    ckEnd(173, ctx, c, 4, v0, f0, 0x2u, 0x0u, false, vuDiffer(t, c));
}
void updateGazeIKWork(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t p4, uint32_t p5)
{
    if (!ckBegin(174, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    if (!ckLeg([&] { ref::updateGazeIKWork(rdram, &c, runtime); })) return;
    ckMid();
    uint64_t v0 = 0u;
    float f0 = 0.0f;
    uint64_t v_h2 = a4::hi64(ctx->r[2]);
    uint64_t xo[2] = {(uint64_t)a1ck::ckBits(ctx->f[0]), Ps2GprU64(ctx, 2)};
    if (!ckLeg([&] { ty::updateGazeIKWork(rdram, runtime, p4, p5, v_h2, xo); })) return;
    R5900Context &t = ckTyCtx(ctx);
    t.r[2] = a4::mk128(v_h2, Ps2GprU64(&t, 2));
    ckEnd(174, ctx, c, 0, v0, f0, 0xeu, 0x1u, a1ck::ckBits(c.f[0]) != (uint32_t)xo[0] || Ps2GprU64(&c, 2) != xo[1], vuDiffer(t, c));
}
void ReflectIK_initWormDynamics(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t p4, uint32_t p5, uint32_t p6, uint32_t p7, float pf12)
{
    if (!ckBegin(175, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    if (!ckLeg([&] { ref::ReflectIK_initWormDynamics(rdram, &c, runtime); })) return;
    ckMid();
    uint64_t v0 = 0u;
    float f0 = 0.0f;
    uint64_t v_h2 = a4::hi64(ctx->r[2]);
    uint64_t v_h3 = a4::hi64(ctx->r[3]);
    if (!ckLeg([&] { v0 = (ty::ReflectIK_initWormDynamics(rdram, runtime, p4, p5, p6, p7, pf12, v_h2, v_h3)); })) return;
    R5900Context &t = ckTyCtx(ctx);
    t.r[2] = a4::mk128(v_h2, Ps2GprU64(&t, 2));
    t.r[3] = a4::mk128(v_h3, Ps2GprU64(&t, 3));
    ckEnd(175, ctx, c, 2, v0, f0, 0x8u, 0x0u, false, vuDiffer(t, c));
}
void GAMEUTIL_get_spring_interpolate_offset3D(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint64_t p4, uint32_t p5, uint32_t p6, float pf12, float pf13, float pf14, float pf15, float pf20, float pf21, float pf22, float pf23, float pf24, float pf25, float pf26)
{
    if (!ckBegin(176, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    if (!ckLeg([&] { ref::GAMEUTIL_get_spring_interpolate_offset3D(rdram, &c, runtime); })) return;
    ckMid();
    uint64_t v0 = 0u;
    float f0 = 0.0f;
    a4::vec4 v_vf0 = ctx->vu0_vf[0];
    a4::vec4 v_vf5 = ctx->vu0_vf[5];
    a4::vec4 v_vf6 = ctx->vu0_vf[6];
    a4::vec4 v_vf7 = ctx->vu0_vf[7];
    a4::vec4 v_vacc = ctx->vu0_acc;
    float v_vq = ctx->vu0_q;
    uint16_t v_vstat = ctx->vu0_status;
    uint64_t v_h2 = a4::hi64(ctx->r[2]);
    uint64_t v_h3 = a4::hi64(ctx->r[3]);
    if (!ckLeg([&] { const a1::RetVF r = ty::GAMEUTIL_get_spring_interpolate_offset3D(rdram, runtime, p4, p5, p6, pf12, pf13, pf14, pf15, pf20, pf21, pf22, pf23, pf24, pf25, pf26, v_vf0, v_vf5, v_vf6, v_vf7, v_vacc, v_vq, v_vstat, v_h2, v_h3); v0 = r.v0; f0 = r.f0; })) return;
    R5900Context &t = ckTyCtx(ctx);
    t.vu0_vf[5] = v_vf5;
    t.vu0_vf[6] = v_vf6;
    t.vu0_vf[7] = v_vf7;
    t.vu0_acc = v_vacc;
    t.vu0_q = v_vq;
    t.vu0_status = v_vstat;
    t.r[3] = a4::mk128(v_h3, Ps2GprU64(&t, 3));
    ckEnd(176, ctx, c, 4, v0, f0, 0x8u, 0x7f03006u, false, vuDiffer(t, c));
}
void get_temp_parameter_motion_ik_base(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint64_t p4, uint32_t p5)
{
    if (!ckBegin(177, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    if (!ckLeg([&] { ref::get_temp_parameter_motion_ik_base(rdram, &c, runtime); })) return;
    ckMid();
    uint64_t v0 = 0u;
    float f0 = 0.0f;
    a4::vec4 v_vf0 = ctx->vu0_vf[0];
    a4::vec4 v_vf5 = ctx->vu0_vf[5];
    a4::vec4 v_vf11 = ctx->vu0_vf[11];
    a4::vec4 v_vf12 = ctx->vu0_vf[12];
    a4::vec4 v_vf13 = ctx->vu0_vf[13];
    a4::vec4 v_vf14 = ctx->vu0_vf[14];
    a4::vec4 v_vf15 = ctx->vu0_vf[15];
    a4::vec4 v_vf16 = ctx->vu0_vf[16];
    a4::vec4 v_vacc = ctx->vu0_acc;
    uint64_t v_h2 = a4::hi64(ctx->r[2]);
    uint64_t xo[1] = {(uint64_t)a1ck::ckBits(ctx->f[3])};
    if (!ckLeg([&] { const a1::RetVF r = ty::get_temp_parameter_motion_ik_base(rdram, runtime, p4, p5, v_vf0, v_vf5, v_vf11, v_vf12, v_vf13, v_vf14, v_vf15, v_vf16, v_vacc, v_h2, xo); v0 = r.v0; f0 = r.f0; })) return;
    R5900Context &t = ckTyCtx(ctx);
    t.vu0_vf[5] = v_vf5;
    t.vu0_vf[11] = v_vf11;
    t.vu0_vf[12] = v_vf12;
    t.vu0_vf[13] = v_vf13;
    t.vu0_vf[14] = v_vf14;
    t.vu0_vf[15] = v_vf15;
    t.vu0_vf[16] = v_vf16;
    t.vu0_acc = v_vacc;
    t.r[2] = a4::mk128(v_h2, Ps2GprU64(&t, 2));
    ckEnd(177, ctx, c, 4, v0, f0, 0xfau, 0xeu, a1ck::ckBits(c.f[3]) != (uint32_t)xo[0], vuDiffer(t, c));
}
void GAMEUTIL_init_col_plane_info_filter(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint64_t p4, float pf12, float pf13)
{
    if (!ckBegin(178, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    if (!ckLeg([&] { ref::GAMEUTIL_init_col_plane_info_filter(rdram, &c, runtime); })) return;
    ckMid();
    uint64_t v0 = 0u;
    float f0 = 0.0f;
    uint64_t v_h2 = a4::hi64(ctx->r[2]);
    uint64_t v_h3 = a4::hi64(ctx->r[3]);
    uint64_t xo[1] = {Ps2GprU64(ctx, 2)};
    if (!ckLeg([&] { ty::GAMEUTIL_init_col_plane_info_filter(rdram, runtime, p4, pf12, pf13, v_h2, v_h3, xo); })) return;
    R5900Context &t = ckTyCtx(ctx);
    t.r[2] = a4::mk128(v_h2, Ps2GprU64(&t, 2));
    t.r[3] = a4::mk128(v_h3, Ps2GprU64(&t, 3));
    ckEnd(178, ctx, c, 0, v0, f0, 0x6cu, 0x0u, Ps2GprU64(&c, 2) != xo[0], vuDiffer(t, c));
}
void gamecore_sub_01381AA8(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint64_t p4)
{
    if (!ckBegin(179, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    if (!ckLeg([&] { ref::gamecore_sub_01381AA8(rdram, &c, runtime); })) return;
    ckMid();
    uint64_t v0 = 0u;
    float f0 = 0.0f;
    a4::vec4 v_vf5 = ctx->vu0_vf[5];
    a4::vec4 v_vf6 = ctx->vu0_vf[6];
    a4::vec4 v_vf7 = ctx->vu0_vf[7];
    uint64_t v_h2 = a4::hi64(ctx->r[2]);
    uint64_t xo[7] = {(uint64_t)a1ck::ckBits(ctx->f[12]), Ps2GprU64(ctx, 4), Ps2GprU64(ctx, 5), Ps2GprU64(ctx, 6), Ps2GprU64(ctx, 7), Ps2GprU64(ctx, 8), Ps2GprU64(ctx, 9)};
    if (!ckLeg([&] { const a1::RetVF r = ty::gamecore_sub_01381AA8(rdram, runtime, p4, v_vf5, v_vf6, v_vf7, v_h2, xo); v0 = r.v0; f0 = r.f0; })) return;
    R5900Context &t = ckTyCtx(ctx);
    t.vu0_vf[5] = v_vf5;
    t.vu0_vf[6] = v_vf6;
    t.vu0_vf[7] = v_vf7;
    ckEnd(179, ctx, c, 4, v0, f0, 0x3f2u, 0x100eu, a1ck::ckBits(c.f[12]) != (uint32_t)xo[0] || Ps2GprU64(&c, 4) != xo[1] || Ps2GprU64(&c, 5) != xo[2] || Ps2GprU64(&c, 6) != xo[3] || Ps2GprU64(&c, 7) != xo[4] || Ps2GprU64(&c, 8) != xo[5] || Ps2GprU64(&c, 9) != xo[6], vuDiffer(t, c));
}
void gamecore_sub_013DC4A8(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint64_t p5, uint64_t p8, float pf20)
{
    if (!ckBegin(180, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    if (!ckLeg([&] { ref::gamecore_sub_013DC4A8(rdram, &c, runtime); })) return;
    ckMid();
    uint64_t v0 = 0u;
    float f0 = 0.0f;
    a4::vec4 v_vf5 = ctx->vu0_vf[5];
    a4::vec4 v_vf6 = ctx->vu0_vf[6];
    a4::vec4 v_vf7 = ctx->vu0_vf[7];
    uint64_t v_h2 = a4::hi64(ctx->r[2]);
    uint64_t xo[1] = {(uint64_t)a1ck::ckBits(ctx->f[20])};
    if (!ckLeg([&] { const a1::RetVF r = ty::gamecore_sub_013DC4A8(rdram, runtime, p5, p8, pf20, v_vf5, v_vf6, v_vf7, v_h2, xo); v0 = r.v0; f0 = r.f0; })) return;
    R5900Context &t = ckTyCtx(ctx);
    t.vu0_vf[5] = v_vf5;
    t.vu0_vf[6] = v_vf6;
    t.vu0_vf[7] = v_vf7;
    ckEnd(180, ctx, c, 4, v0, f0, 0x3f2u, 0x13f01eu, a1ck::ckBits(c.f[20]) != (uint32_t)xo[0], vuDiffer(t, c));
}
void Kite_InitSpecialPathWork(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t p4)
{
    if (!ckBegin(181, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    if (!ckLeg([&] { ref::Kite_InitSpecialPathWork(rdram, &c, runtime); })) return;
    ckMid();
    uint64_t v0 = 0u;
    float f0 = 0.0f;
    uint64_t v_h3 = a4::hi64(ctx->r[3]);
    if (!ckLeg([&] { v0 = a1_sx64(ty::Kite_InitSpecialPathWork(rdram, runtime, p4, v_h3)); })) return;
    R5900Context &t = ckTyCtx(ctx);
    t.r[3] = a4::mk128(v_h3, Ps2GprU64(&t, 3));
    ckEnd(181, ctx, c, 1, v0, f0, 0x8u, 0x0u, false, vuDiffer(t, c));
}
void Kite_SetSpecialPathLayout(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t p4, uint32_t p5, float pf12, float pf13)
{
    if (!ckBegin(182, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    if (!ckLeg([&] { ref::Kite_SetSpecialPathLayout(rdram, &c, runtime); })) return;
    ckMid();
    uint64_t v0 = 0u;
    float f0 = 0.0f;
    uint64_t v_h2 = a4::hi64(ctx->r[2]);
    if (!ckLeg([&] { v0 = (ty::Kite_SetSpecialPathLayout(rdram, runtime, p4, p5, pf12, pf13, v_h2)); })) return;
    R5900Context &t = ckTyCtx(ctx);
    t.r[2] = a4::mk128(v_h2, Ps2GprU64(&t, 2));
    ckEnd(182, ctx, c, 2, v0, f0, 0x10u, 0x0u, false, vuDiffer(t, c));
}
void gamecore_sub_01411AD0(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint64_t p4)
{
    if (!ckBegin(183, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    if (!ckLeg([&] { ref::gamecore_sub_01411AD0(rdram, &c, runtime); })) return;
    ckMid();
    uint64_t v0 = 0u;
    float f0 = 0.0f;
    uint64_t v_h3 = a4::hi64(ctx->r[3]);
    if (!ckLeg([&] { v0 = (ty::gamecore_sub_01411AD0(rdram, runtime, p4, v_h3)); })) return;
    R5900Context &t = ckTyCtx(ctx);
    t.r[3] = a4::mk128(v_h3, Ps2GprU64(&t, 3));
    ckEnd(183, ctx, c, 2, v0, f0, 0x8u, 0x0u, false, vuDiffer(t, c));
}
void init_camera_pause_rise(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint64_t p4, uint64_t p5)
{
    if (!ckBegin(184, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    if (!ckLeg([&] { ref::init_camera_pause_rise(rdram, &c, runtime); })) return;
    ckMid();
    uint64_t v0 = 0u;
    float f0 = 0.0f;
    a4::vec4 v_vf5 = ctx->vu0_vf[5];
    a4::vec4 v_vf6 = ctx->vu0_vf[6];
    a4::vec4 v_vf7 = ctx->vu0_vf[7];
    float v_vq = ctx->vu0_q;
    uint16_t v_vstat = ctx->vu0_status;
    uint64_t v_h2 = a4::hi64(ctx->r[2]);
    uint64_t v_h3 = a4::hi64(ctx->r[3]);
    if (!ckLeg([&] { const a1::RetVF r = ty::init_camera_pause_rise(rdram, runtime, p4, p5, v_vf5, v_vf6, v_vf7, v_vq, v_vstat, v_h2, v_h3); v0 = r.v0; f0 = r.f0; })) return;
    R5900Context &t = ckTyCtx(ctx);
    t.vu0_vf[5] = v_vf5;
    t.vu0_vf[6] = v_vf6;
    t.vu0_vf[7] = v_vf7;
    t.vu0_q = v_vq;
    t.vu0_status = v_vstat;
    t.r[2] = a4::mk128(v_h2, Ps2GprU64(&t, 2));
    t.r[3] = a4::mk128(v_h3, Ps2GprU64(&t, 3));
    ckEnd(184, ctx, c, 4, v0, f0, 0x78u, 0x6u, false, vuDiffer(t, c));
}
void gamecore_sub_01440F60(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint64_t p5)
{
    if (!ckBegin(185, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    if (!ckLeg([&] { ref::gamecore_sub_01440F60(rdram, &c, runtime); })) return;
    ckMid();
    uint64_t v0 = 0u;
    float f0 = 0.0f;
    a4::vec4 v_vf0 = ctx->vu0_vf[0];
    uint64_t v_h2 = a4::hi64(ctx->r[2]);
    if (!ckLeg([&] { const a1::RetVF r = ty::gamecore_sub_01440F60(rdram, runtime, p5, v_vf0, v_h2); v0 = r.v0; f0 = r.f0; })) return;
    R5900Context &t = ckTyCtx(ctx);
    t.r[2] = a4::mk128(v_h2, Ps2GprU64(&t, 2));
    ckEnd(185, ctx, c, 4, v0, f0, 0x3au, 0x6u, false, vuDiffer(t, c));
}
void gamecore_sub_01444518(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint64_t p5)
{
    if (!ckBegin(186, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    if (!ckLeg([&] { ref::gamecore_sub_01444518(rdram, &c, runtime); })) return;
    ckMid();
    uint64_t v0 = 0u;
    float f0 = 0.0f;
    uint64_t v_h2 = a4::hi64(ctx->r[2]);
    if (!ckLeg([&] { const a1::RetVF r = ty::gamecore_sub_01444518(rdram, runtime, p5, v_h2); v0 = r.v0; f0 = r.f0; })) return;
    R5900Context &t = ckTyCtx(ctx);
    t.r[2] = a4::mk128(v_h2, Ps2GprU64(&t, 2));
    ckEnd(186, ctx, c, 4, v0, f0, 0x3au, 0x2u, false, vuDiffer(t, c));
}
void gamecore_sub_0144CDF8(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint64_t p4, uint64_t p5)
{
    if (!ckBegin(187, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    if (!ckLeg([&] { ref::gamecore_sub_0144CDF8(rdram, &c, runtime); })) return;
    ckMid();
    uint64_t v0 = 0u;
    float f0 = 0.0f;
    a4::vec4 v_vf0 = ctx->vu0_vf[0];
    if (!ckLeg([&] { const a1::RetVF r = ty::gamecore_sub_0144CDF8(rdram, runtime, p4, p5, v_vf0); v0 = r.v0; f0 = r.f0; })) return;
    R5900Context &t = ckTyCtx(ctx);
    ckEnd(187, ctx, c, 4, v0, f0, 0x32u, 0x7eu, false, vuDiffer(t, c));
}
const a1ck::CkEntry kEntries[188] = {
    {"iosCubicInterVectorXYZ", 0x1b7bb8u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { iosCubicInterVectorXYZ(rdram, ctx, runtime, (uint32_t)Ps2GprU32(ctx, 4), (uint32_t)Ps2GprU32(ctx, 5), (uint32_t)Ps2GprU32(ctx, 6), (uint32_t)Ps2GprU32(ctx, 7), ctx->f[12]); }},
    {"iosCubicBezierVectorXYZ", 0x1b7c88u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { iosCubicBezierVectorXYZ(rdram, ctx, runtime, (uint32_t)Ps2GprU32(ctx, 4), (uint32_t)Ps2GprU32(ctx, 5), ctx->f[12]); }},
    {"iosUnitCurrentMatrix", 0x1b8620u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { iosUnitCurrentMatrix(rdram, ctx, runtime); }},
    {"iosPushVu0RegistersToWork", 0x1b8668u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { iosPushVu0RegistersToWork(rdram, ctx, runtime, (uint32_t)Ps2GprU32(ctx, 4)); }},
    {"iosPopVu0RegistersFromWork", 0x1b8770u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { iosPopVu0RegistersFromWork(rdram, ctx, runtime, (uint32_t)Ps2GprU32(ctx, 4)); }},
    {"iosTransCurrentMatrix", 0x1b8878u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { iosTransCurrentMatrix(rdram, ctx, runtime, (uint32_t)Ps2GprU32(ctx, 4)); }},
    {"iosSetTransCurrentMatrix", 0x1b8898u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { iosSetTransCurrentMatrix(rdram, ctx, runtime, (uint32_t)Ps2GprU32(ctx, 4)); }},
    {"iosClearTransCurrentMatrix", 0x1b88a8u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { iosClearTransCurrentMatrix(rdram, ctx, runtime); }},
    {"iosScaleCurrentMatrix", 0x1b8c00u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { iosScaleCurrentMatrix(rdram, ctx, runtime, ctx->f[12], ctx->f[13], ctx->f[14]); }},
    {"iosGetCurrentMatrix", 0x1b8c90u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { iosGetCurrentMatrix(rdram, ctx, runtime, (uint32_t)Ps2GprU32(ctx, 4)); }},
    {"iosGetCurrentMatrixTrans", 0x1b8ca8u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { iosGetCurrentMatrixTrans(rdram, ctx, runtime, (uint32_t)Ps2GprU32(ctx, 4)); }},
    {"iosSetCurrentMatrix", 0x1b8cb8u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { iosSetCurrentMatrix(rdram, ctx, runtime, (uint32_t)Ps2GprU32(ctx, 4)); }},
    {"iosMulCurrentMatrixR", 0x1b8cd0u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { iosMulCurrentMatrixR(rdram, ctx, runtime, (uint32_t)Ps2GprU32(ctx, 4)); }},
    {"iosMulCurrentMatrixL", 0x1b8d38u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { iosMulCurrentMatrixL(rdram, ctx, runtime, (uint32_t)Ps2GprU32(ctx, 4)); }},
    {"iosApplyCurrentMatrix", 0x1b8d90u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { iosApplyCurrentMatrix(rdram, ctx, runtime, (uint32_t)Ps2GprU32(ctx, 4), (uint32_t)Ps2GprU32(ctx, 5)); }},
    {"iosApplyRotCurrentMatrix", 0x1b8db0u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { iosApplyRotCurrentMatrix(rdram, ctx, runtime, (uint32_t)Ps2GprU32(ctx, 4), (uint32_t)Ps2GprU32(ctx, 5)); }},
    {"iosRotTransPersCurrentMatrix", 0x1b8dd0u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { iosRotTransPersCurrentMatrix(rdram, ctx, runtime, (uint32_t)Ps2GprU32(ctx, 4), (uint32_t)Ps2GprU32(ctx, 5)); }},
    {"iosTransposeCurrentMatrix", 0x1b8e00u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { iosTransposeCurrentMatrix(rdram, ctx, runtime); }},
    {"iosTransposeRotationCurrentMatrix", 0x1b8e50u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { iosTransposeRotationCurrentMatrix(rdram, ctx, runtime); }},
    {"iosInverseCurrentMatrix", 0x1b8e80u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { iosInverseCurrentMatrix(rdram, ctx, runtime); }},
    {"iosApplyRotMatrix", 0x1b9170u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { iosApplyRotMatrix(rdram, ctx, runtime, (uint32_t)Ps2GprU32(ctx, 4), (uint32_t)Ps2GprU32(ctx, 5), (uint32_t)Ps2GprU32(ctx, 6)); }},
    {"iosUnitMatrix", 0x1b91a0u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { iosUnitMatrix(rdram, ctx, runtime, (uint32_t)Ps2GprU32(ctx, 4)); }},
    {"iosUnitRotation", 0x1b91c8u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { iosUnitRotation(rdram, ctx, runtime, (uint32_t)Ps2GprU32(ctx, 4)); }},
    {"iosScaleMatrixV", 0x1b92d0u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { iosScaleMatrixV(rdram, ctx, runtime, (uint32_t)Ps2GprU32(ctx, 4), (uint32_t)Ps2GprU32(ctx, 5), (uint32_t)Ps2GprU32(ctx, 6)); }},
    {"iosMakeNormalLightMatrix", 0x1b9468u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { iosMakeNormalLightMatrix(rdram, ctx, runtime, (uint32_t)Ps2GprU32(ctx, 4), (uint32_t)Ps2GprU32(ctx, 5), (uint64_t)Ps2GprU64(ctx, 6), (uint64_t)Ps2GprU64(ctx, 7), (uint64_t)Ps2GprU64(ctx, 8)); }},
    {"iosScaleNormalLightMatrix", 0x1b9560u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { iosScaleNormalLightMatrix(rdram, ctx, runtime, (uint32_t)Ps2GprU32(ctx, 4), (uint32_t)Ps2GprU32(ctx, 5), (uint32_t)Ps2GprU32(ctx, 6)); }},
    {"iosMakeLightColorMatrix", 0x1b9598u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { iosMakeLightColorMatrix(rdram, ctx, runtime, (uint32_t)Ps2GprU32(ctx, 4), (uint32_t)Ps2GprU32(ctx, 5), (uint64_t)Ps2GprU64(ctx, 6), (uint64_t)Ps2GprU64(ctx, 7), (uint64_t)Ps2GprU64(ctx, 8)); }},
    {"iosTransMatrix", 0x1b95d0u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { iosTransMatrix(rdram, ctx, runtime, (uint32_t)Ps2GprU32(ctx, 4), (uint32_t)Ps2GprU32(ctx, 5), (uint64_t)Ps2GprU64(ctx, 6)); }},
    {"iosScaleMatrix", 0x1b96a0u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { iosScaleMatrix(rdram, ctx, runtime, (uint32_t)Ps2GprU32(ctx, 4), ctx->f[12], ctx->f[13], ctx->f[14]); }},
    {"iosGetYDirXYZFromMatrix", 0x1b9cd0u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { iosGetYDirXYZFromMatrix(rdram, ctx, runtime, (uint32_t)Ps2GprU32(ctx, 4), (uint32_t)Ps2GprU32(ctx, 5)); }},
    {"iosGetXDirXYZFromMatrix", 0x1b9ce8u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { iosGetXDirXYZFromMatrix(rdram, ctx, runtime, (uint32_t)Ps2GprU32(ctx, 4), (uint32_t)Ps2GprU32(ctx, 5)); }},
    {"iosRotTransCurrentMatrix", 0x1b9d18u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { iosRotTransCurrentMatrix(rdram, ctx, runtime, (uint32_t)Ps2GprU32(ctx, 4), (uint32_t)Ps2GprU32(ctx, 5)); }},
    {"iosMvecZero", 0x1b9d78u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { iosMvecZero(rdram, ctx, runtime, (uint32_t)Ps2GprU32(ctx, 4)); }},
    {"MultiCurrentQuaternion", 0x1bb8a8u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { MultiCurrentQuaternion(rdram, ctx, runtime, (uint32_t)Ps2GprU32(ctx, 4)); }},
    {"MultiCurrentQuaternionL", 0x1bb900u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { MultiCurrentQuaternionL(rdram, ctx, runtime, (uint32_t)Ps2GprU32(ctx, 4)); }},
    {"InvertCurrentQuaternion", 0x1bb958u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { InvertCurrentQuaternion(rdram, ctx, runtime); }},
    {"SetCurrentQuaternion", 0x1bb988u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { SetCurrentQuaternion(rdram, ctx, runtime, (uint32_t)Ps2GprU32(ctx, 4)); }},
    {"InitQuaternionDrive", 0x1bbc28u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { InitQuaternionDrive(rdram, ctx, runtime); }},
    {"DivQuaternion", 0x1bbe10u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { DivQuaternion(rdram, ctx, runtime, (uint32_t)Ps2GprU32(ctx, 4), (uint32_t)Ps2GprU32(ctx, 5), (uint32_t)Ps2GprU32(ctx, 6)); }},
    {"SetQuaternionByCosineAxisRotateVWithNoRegularize", 0x1bc850u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { SetQuaternionByCosineAxisRotateVWithNoRegularize(rdram, ctx, runtime, (uint32_t)Ps2GprU32(ctx, 4), (uint32_t)Ps2GprU32(ctx, 5), ctx->f[12]); }},
    {"GetYRotQuaternionFromDir", 0x1bc8f8u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { GetYRotQuaternionFromDir(rdram, ctx, runtime, (uint32_t)Ps2GprU32(ctx, 4), (uint32_t)Ps2GprU32(ctx, 5)); }},
    {"iosFTOI15Vector", 0x1be6d0u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { iosFTOI15Vector(rdram, ctx, runtime, (uint32_t)Ps2GprU32(ctx, 4), (uint32_t)Ps2GprU32(ctx, 5)); }},
    {"iosFTOI12Vector", 0x1be6e8u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { iosFTOI12Vector(rdram, ctx, runtime, (uint32_t)Ps2GprU32(ctx, 4), (uint32_t)Ps2GprU32(ctx, 5)); }},
    {"iosFTOI0Vector", 0x1be718u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { iosFTOI0Vector(rdram, ctx, runtime, (uint32_t)Ps2GprU32(ctx, 4), (uint32_t)Ps2GprU32(ctx, 5)); }},
    {"iosITOF15Vector", 0x1be730u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { iosITOF15Vector(rdram, ctx, runtime, (uint32_t)Ps2GprU32(ctx, 4), (uint32_t)Ps2GprU32(ctx, 5)); }},
    {"iosITOF12Vector", 0x1be748u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { iosITOF12Vector(rdram, ctx, runtime, (uint32_t)Ps2GprU32(ctx, 4), (uint32_t)Ps2GprU32(ctx, 5)); }},
    {"iosITOF4Vector", 0x1be760u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { iosITOF4Vector(rdram, ctx, runtime, (uint32_t)Ps2GprU32(ctx, 4), (uint32_t)Ps2GprU32(ctx, 5)); }},
    {"iosITOF0Vector", 0x1be778u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { iosITOF0Vector(rdram, ctx, runtime, (uint32_t)Ps2GprU32(ctx, 4), (uint32_t)Ps2GprU32(ctx, 5)); }},
    {"iosGetStabXZVector", 0x1be790u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { iosGetStabXZVector(rdram, ctx, runtime, (uint32_t)Ps2GprU32(ctx, 4), (uint32_t)Ps2GprU32(ctx, 5)); }},
    {"iosGetReflectVector", 0x1be8b0u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { iosGetReflectVector(rdram, ctx, runtime, (uint32_t)Ps2GprU32(ctx, 4), (uint32_t)Ps2GprU32(ctx, 5), (uint32_t)Ps2GprU32(ctx, 6)); }},
    {"iosGetStabXYVector", 0x1be9e0u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { iosGetStabXYVector(rdram, ctx, runtime, (uint32_t)Ps2GprU32(ctx, 4), (uint32_t)Ps2GprU32(ctx, 5)); }},
    {"iosGetStabYZVector", 0x1bea18u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { iosGetStabYZVector(rdram, ctx, runtime, (uint32_t)Ps2GprU32(ctx, 4), (uint32_t)Ps2GprU32(ctx, 5)); }},
    {"iosGetLengthPointToLine", 0x1bea50u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { iosGetLengthPointToLine(rdram, ctx, runtime, (uint32_t)Ps2GprU32(ctx, 4), (uint32_t)Ps2GprU32(ctx, 5), (uint32_t)Ps2GprU32(ctx, 6), (uint32_t)Ps2GprU32(ctx, 7)); }},
    {"iosGetInverseVectorXYZ", 0x1beb08u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { iosGetInverseVectorXYZ(rdram, ctx, runtime, (uint32_t)Ps2GprU32(ctx, 4), (uint32_t)Ps2GprU32(ctx, 5)); }},
    {"iosGetExceptAxisVector", 0x1beb30u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { iosGetExceptAxisVector(rdram, ctx, runtime, (uint32_t)Ps2GprU32(ctx, 4), (uint32_t)Ps2GprU32(ctx, 5), (uint32_t)Ps2GprU32(ctx, 6)); }},
    {"iosCalcFColorToBrightness", 0x1beb88u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { iosCalcFColorToBrightness(rdram, ctx, runtime, (uint32_t)Ps2GprU32(ctx, 4)); }},
    {"iosITOF15VectorXYZ", 0x1bebc0u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { iosITOF15VectorXYZ(rdram, ctx, runtime, (uint32_t)Ps2GprU32(ctx, 4), (uint32_t)Ps2GprU32(ctx, 5)); }},
    {"iosScaleColorTo255", 0x1bebd8u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { iosScaleColorTo255(rdram, ctx, runtime, (uint32_t)Ps2GprU32(ctx, 4), (uint32_t)Ps2GprU32(ctx, 5)); }},
    {"iosScaleColorToONE", 0x1bebf8u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { iosScaleColorToONE(rdram, ctx, runtime, (uint32_t)Ps2GprU32(ctx, 4), (uint32_t)Ps2GprU32(ctx, 5)); }},
    {"kernel_sub_001C84F0", 0x1c84f0u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { kernel_sub_001C84F0(rdram, ctx, runtime, (uint32_t)Ps2GprU32(ctx, 4), (uint64_t)Ps2GprU64(ctx, 5), (uint64_t)Ps2GprU64(ctx, 6), (uint32_t)Ps2GprU32(ctx, 7)); }},
    {"GAMEUTIL_get_xyz_axis_from_quaternion", 0x107efb8u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { GAMEUTIL_get_xyz_axis_from_quaternion(rdram, ctx, runtime, (uint32_t)Ps2GprU32(ctx, 4), (uint32_t)Ps2GprU32(ctx, 5), (uint32_t)Ps2GprU32(ctx, 6), (uint32_t)Ps2GprU32(ctx, 7)); }},
    {"GAMEUTIL_get_vector_velocity_by_diff_move3D", 0x107fbf0u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { GAMEUTIL_get_vector_velocity_by_diff_move3D(rdram, ctx, runtime, (uint32_t)Ps2GprU32(ctx, 4), (uint32_t)Ps2GprU32(ctx, 5), (uint32_t)Ps2GprU32(ctx, 6), (uint32_t)Ps2GprU32(ctx, 7), ctx->f[12]); }},
    {"GAMEUTIL_check_and_get_normalize_vector", 0x107ff08u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { GAMEUTIL_check_and_get_normalize_vector(rdram, ctx, runtime, (uint32_t)Ps2GprU32(ctx, 4), (uint64_t)Ps2GprU64(ctx, 5), (uint32_t)Ps2GprU32(ctx, 6)); }},
    {"GAMEUTIL_get_divide_vector_value", 0x107ffe0u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { GAMEUTIL_get_divide_vector_value(rdram, ctx, runtime, (uint64_t)Ps2GprU64(ctx, 4), (uint64_t)Ps2GprU64(ctx, 5), (uint64_t)Ps2GprU64(ctx, 6), (uint32_t)Ps2GprU32(ctx, 7), (uint32_t)Ps2GprU32(ctx, 8), (uint32_t)Ps2GprU32(ctx, 9), (uint32_t)Ps2GprU32(ctx, 10)); }},
    {"GAMEUTIL_divide_yaw_pitch_dir", 0x1081a78u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { GAMEUTIL_divide_yaw_pitch_dir(rdram, ctx, runtime, (uint64_t)Ps2GprU64(ctx, 4), (uint64_t)Ps2GprU64(ctx, 5), (uint32_t)Ps2GprU32(ctx, 6)); }},
    {"manager_sub_01082BD8", 0x1082bd8u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { manager_sub_01082BD8(rdram, ctx, runtime, ctx->f[12]); }},
    {"GAMEUTIL_get_interp_value3D", 0x1083870u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { GAMEUTIL_get_interp_value3D(rdram, ctx, runtime, (uint32_t)Ps2GprU32(ctx, 4), (uint32_t)Ps2GprU32(ctx, 5), (uint32_t)Ps2GprU32(ctx, 6), ctx->f[12]); }},
    {"getZ", 0x1087498u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { getZ(rdram, ctx, runtime, (uint32_t)Ps2GprU32(ctx, 4), (uint32_t)Ps2GprU32(ctx, 5), (uint32_t)Ps2GprU32(ctx, 6)); }},
    {"manager_sub_0108D358", 0x108d358u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { manager_sub_0108D358(rdram, ctx, runtime, (uint64_t)Ps2GprU64(ctx, 4)); }},
    {"iosgluGetViewCoordPosition", 0x108f678u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { iosgluGetViewCoordPosition(rdram, ctx, runtime, (uint32_t)Ps2GprU32(ctx, 4), (uint32_t)Ps2GprU32(ctx, 5)); }},
    {"manager_sub_0108F858", 0x108f858u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { manager_sub_0108F858(rdram, ctx, runtime, (uint32_t)Ps2GprU32(ctx, 4), (uint32_t)Ps2GprU32(ctx, 5), (uint32_t)Ps2GprU32(ctx, 6)); }},
    {"manager_sub_010AD048", 0x10ad048u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { manager_sub_010AD048(rdram, ctx, runtime, (uint64_t)Ps2GprU64(ctx, 4), (uint32_t)Ps2GprU32(ctx, 5), (uint32_t)Ps2GprU32(ctx, 6)); }},
    {"manager_sub_010BA150", 0x10ba150u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { manager_sub_010BA150(rdram, ctx, runtime, (uint32_t)Ps2GprU32(ctx, 4), (uint32_t)Ps2GprU32(ctx, 5), (uint32_t)Ps2GprU32(ctx, 6), (uint32_t)Ps2GprU32(ctx, 7)); }},
    {"initGazeIKWork", 0x10ba718u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { initGazeIKWork(rdram, ctx, runtime, (uint32_t)Ps2GprU32(ctx, 6)); }},
    {"GetCharacterClassControlFrontDirection", 0x10c0e48u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { GetCharacterClassControlFrontDirection(rdram, ctx, runtime, (uint32_t)Ps2GprU32(ctx, 4), (uint32_t)Ps2GprU32(ctx, 5)); }},
    {"GetCharacterClassOptionNodeLocalQuaternionPos", 0x10c10e0u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { GetCharacterClassOptionNodeLocalQuaternionPos(rdram, ctx, runtime, (uint32_t)Ps2GprU32(ctx, 4), (uint32_t)Ps2GprU32(ctx, 5), (uint32_t)Ps2GprU32(ctx, 6), (uint32_t)Ps2GprU32(ctx, 7)); }},
    {"ResetCharacterClassFallDownInfo", 0x10c1138u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { ResetCharacterClassFallDownInfo(rdram, ctx, runtime, (uint32_t)Ps2GprU32(ctx, 4)); }},
    {"ResetCharacterClassSlideStatus", 0x10c17e0u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { ResetCharacterClassSlideStatus(rdram, ctx, runtime, (uint32_t)Ps2GprU32(ctx, 4)); }},
    {"ResetCharacterClassMotionInterpRootOfs", 0x10c17f8u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { ResetCharacterClassMotionInterpRootOfs(rdram, ctx, runtime, (uint32_t)Ps2GprU32(ctx, 4)); }},
    {"manager_sub_010C6D30", 0x10c6d30u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { manager_sub_010C6D30(rdram, ctx, runtime, (uint32_t)Ps2GprU32(ctx, 4)); }},
    {"RequestCharacterClassMotionDirection", 0x10d2b48u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { RequestCharacterClassMotionDirection(rdram, ctx, runtime, (uint32_t)Ps2GprU32(ctx, 4), (uint32_t)Ps2GprU32(ctx, 5)); }},
    {"RequestCharacterClassRootOffset", 0x10d2ba0u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { RequestCharacterClassRootOffset(rdram, ctx, runtime, (uint32_t)Ps2GprU32(ctx, 4), (uint32_t)Ps2GprU32(ctx, 5)); }},
    {"charrootdyn_getInterpVector", 0x10dc1d8u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { charrootdyn_getInterpVector(rdram, ctx, runtime, (uint32_t)Ps2GprU32(ctx, 4), (uint32_t)Ps2GprU32(ctx, 5), (uint32_t)Ps2GprU32(ctx, 6), ctx->f[12]); }},
    {"charrootdyn_getInterpPoint", 0x10dc220u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { charrootdyn_getInterpPoint(rdram, ctx, runtime, (uint32_t)Ps2GprU32(ctx, 4), (uint32_t)Ps2GprU32(ctx, 5), (uint32_t)Ps2GprU32(ctx, 6), ctx->f[12]); }},
    {"charrootdyn_getvelocity", 0x10dcdc0u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { charrootdyn_getvelocity(rdram, ctx, runtime, (uint32_t)Ps2GprU32(ctx, 4), (uint32_t)Ps2GprU32(ctx, 5), (uint32_t)Ps2GprU32(ctx, 6), ctx->f[12]); }},
    {"SetDemoMotionOrientInfo", 0x10ebc80u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { SetDemoMotionOrientInfo(rdram, ctx, runtime, (uint64_t)Ps2GprU64(ctx, 4), (uint64_t)Ps2GprU64(ctx, 5), (uint32_t)Ps2GprU32(ctx, 6)); }},
    {"SetJudgeVolumeInfo", 0x10eec00u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { SetJudgeVolumeInfo(rdram, ctx, runtime, (uint32_t)Ps2GprU32(ctx, 4), (uint32_t)Ps2GprU32(ctx, 5), (uint32_t)Ps2GprU32(ctx, 6), (uint32_t)Ps2GprU32(ctx, 7), (uint32_t)Ps2GprU32(ctx, 8), (uint32_t)Ps2GprU32(ctx, 9), (uint32_t)Ps2GprU32(ctx, 10), ctx->f[12], ctx->f[13], ctx->f[14]); }},
    {"UpdateGlobalLightInRange", 0x10f8d30u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { UpdateGlobalLightInRange(rdram, ctx, runtime, (uint32_t)Ps2GprU32(ctx, 4), (uint32_t)Ps2GprU32(ctx, 5)); }},
    {"CopyCollisionDynamics", 0x1123fa0u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { CopyCollisionDynamics(rdram, ctx, runtime, (uint32_t)Ps2GprU32(ctx, 4), (uint32_t)Ps2GprU32(ctx, 5)); }},
    {"FilterCopyCollisionDynamics", 0x1123fd0u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { FilterCopyCollisionDynamics(rdram, ctx, runtime, (uint32_t)Ps2GprU32(ctx, 4), (uint32_t)Ps2GprU32(ctx, 5)); }},
    {"manager_sub_01129258", 0x1129258u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { manager_sub_01129258(rdram, ctx, runtime, (uint32_t)Ps2GprU32(ctx, 4), (uint32_t)Ps2GprU32(ctx, 5), (uint32_t)Ps2GprU32(ctx, 6), (uint32_t)Ps2GprU32(ctx, 7)); }},
    {"SetCollisionEdgeLinkDBFinderVWallDirectionStatus", 0x1129ea8u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { SetCollisionEdgeLinkDBFinderVWallDirectionStatus(rdram, ctx, runtime, (uint32_t)Ps2GprU32(ctx, 4), (uint32_t)Ps2GprU32(ctx, 5)); }},
    {"SetCollisionEdgeLinkDBFinderCollectStatus", 0x1129eb8u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { SetCollisionEdgeLinkDBFinderCollectStatus(rdram, ctx, runtime, (uint32_t)Ps2GprU32(ctx, 4), (uint32_t)Ps2GprU32(ctx, 5), (uint32_t)Ps2GprU32(ctx, 6), (uint32_t)Ps2GprU32(ctx, 7), (uint32_t)Ps2GprU32(ctx, 8), ctx->f[12]); }},
    {"SetGeometryPointerGlobalState", 0x11328f8u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { SetGeometryPointerGlobalState(rdram, ctx, runtime, (uint32_t)Ps2GprU32(ctx, 4), (uint32_t)Ps2GprU32(ctx, 5), (uint32_t)Ps2GprU32(ctx, 6)); }},
    {"SetGeometryPointerGlobalStatePointer", 0x1132970u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { SetGeometryPointerGlobalStatePointer(rdram, ctx, runtime, (uint32_t)Ps2GprU32(ctx, 4), (uint32_t)Ps2GprU32(ctx, 5), (uint32_t)Ps2GprU32(ctx, 6), (uint32_t)Ps2GprU32(ctx, 7), (uint32_t)Ps2GprU32(ctx, 8)); }},
    {"SetGeometryPointerNodePosState", 0x1132c60u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { SetGeometryPointerNodePosState(rdram, ctx, runtime, (uint32_t)Ps2GprU32(ctx, 4), (uint32_t)Ps2GprU32(ctx, 5), (uint32_t)Ps2GprU32(ctx, 6), (uint32_t)Ps2GprU32(ctx, 7), (uint32_t)Ps2GprU32(ctx, 8)); }},
    {"SetGeometryPointerNodePosStateGlobalRot", 0x1132c88u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { SetGeometryPointerNodePosStateGlobalRot(rdram, ctx, runtime, (uint32_t)Ps2GprU32(ctx, 4), (uint32_t)Ps2GprU32(ctx, 5), (uint32_t)Ps2GprU32(ctx, 6), (uint32_t)Ps2GprU32(ctx, 7), (uint32_t)Ps2GprU32(ctx, 8)); }},
    {"GetPointDistanceSquareFromUnnormalizedPlane", 0x1134140u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { GetPointDistanceSquareFromUnnormalizedPlane(rdram, ctx, runtime, (uint32_t)Ps2GprU32(ctx, 4), (uint32_t)Ps2GprU32(ctx, 5)); }},
    {"GetPointDistanceFromPlane", 0x1134198u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { GetPointDistanceFromPlane(rdram, ctx, runtime, (uint32_t)Ps2GprU32(ctx, 4), (uint32_t)Ps2GprU32(ctx, 5)); }},
    {"MultiMatrixByPlane", 0x11341c8u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { MultiMatrixByPlane(rdram, ctx, runtime, (uint32_t)Ps2GprU32(ctx, 4), (uint32_t)Ps2GprU32(ctx, 5), (uint32_t)Ps2GprU32(ctx, 6)); }},
    {"CompletePlaneD", 0x1134228u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { CompletePlaneD(rdram, ctx, runtime, (uint32_t)Ps2GprU32(ctx, 4), (uint32_t)Ps2GprU32(ctx, 5)); }},
    {"GetSTPositionOfVectorPlane", 0x1134250u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { GetSTPositionOfVectorPlane(rdram, ctx, runtime, (uint32_t)Ps2GprU32(ctx, 4), (uint32_t)Ps2GprU32(ctx, 5), (uint32_t)Ps2GprU32(ctx, 6), (uint32_t)Ps2GprU32(ctx, 7), (uint32_t)Ps2GprU32(ctx, 8)); }},
    {"GetPlaneFittingDiffVector", 0x1134390u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { GetPlaneFittingDiffVector(rdram, ctx, runtime, (uint32_t)Ps2GprU32(ctx, 4), (uint32_t)Ps2GprU32(ctx, 5), (uint32_t)Ps2GprU32(ctx, 6)); }},
    {"GetPlaneFittingPointForUnnormalizedPlane", 0x1134440u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { GetPlaneFittingPointForUnnormalizedPlane(rdram, ctx, runtime, (uint32_t)Ps2GprU32(ctx, 4), (uint32_t)Ps2GprU32(ctx, 5), (uint32_t)Ps2GprU32(ctx, 6)); }},
    {"GetPlaneFittingVector", 0x1134628u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { GetPlaneFittingVector(rdram, ctx, runtime, (uint32_t)Ps2GprU32(ctx, 4), (uint32_t)Ps2GprU32(ctx, 5), (uint32_t)Ps2GprU32(ctx, 6)); }},
    {"GetPlaneReflectionVector", 0x11346c8u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { GetPlaneReflectionVector(rdram, ctx, runtime, (uint32_t)Ps2GprU32(ctx, 4), (uint32_t)Ps2GprU32(ctx, 5), (uint32_t)Ps2GprU32(ctx, 6)); }},
    {"GetNormalPlane", 0x1134730u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { GetNormalPlane(rdram, ctx, runtime, (uint32_t)Ps2GprU32(ctx, 4), (uint32_t)Ps2GprU32(ctx, 5)); }},
    {"GetCrossPositionPlaneLine", 0x11348e0u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { GetCrossPositionPlaneLine(rdram, ctx, runtime, (uint32_t)Ps2GprU32(ctx, 4), (uint32_t)Ps2GprU32(ctx, 5), (uint32_t)Ps2GprU32(ctx, 6), (uint32_t)Ps2GprU32(ctx, 7)); }},
    {"GetPlaneDistance", 0x1135b50u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { GetPlaneDistance(rdram, ctx, runtime, (uint32_t)Ps2GprU32(ctx, 4), (uint32_t)Ps2GprU32(ctx, 5)); }},
    {"GetDistanceSquare", 0x1135b78u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { GetDistanceSquare(rdram, ctx, runtime, (uint32_t)Ps2GprU32(ctx, 4), (uint32_t)Ps2GprU32(ctx, 5)); }},
    {"GetDistanceSquareXZ", 0x1135ba0u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { GetDistanceSquareXZ(rdram, ctx, runtime, (uint32_t)Ps2GprU32(ctx, 4), (uint32_t)Ps2GprU32(ctx, 5)); }},
    {"ExchangeLWVector", 0x1135bc8u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { ExchangeLWVector(rdram, ctx, runtime, (uint32_t)Ps2GprU32(ctx, 4), (uint32_t)Ps2GprU32(ctx, 5)); }},
    {"manager_sub_01142198", 0x1142198u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { manager_sub_01142198(rdram, ctx, runtime, (uint32_t)Ps2GprU32(ctx, 4), (uint32_t)Ps2GprU32(ctx, 5)); }},
    {"manager_sub_011422D8", 0x11422d8u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { manager_sub_011422D8(rdram, ctx, runtime, (uint32_t)Ps2GprU32(ctx, 4), (uint32_t)Ps2GprU32(ctx, 5), (uint32_t)Ps2GprU32(ctx, 6)); }},
    {"manager_sub_01146DA8", 0x1146da8u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { manager_sub_01146DA8(rdram, ctx, runtime, (uint32_t)Ps2GprU32(ctx, 4), (uint32_t)Ps2GprU32(ctx, 5), (uint32_t)Ps2GprU32(ctx, 6), ctx->f[12], ctx->f[13]); }},
    {"manager_sub_01146EB8", 0x1146eb8u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { manager_sub_01146EB8(rdram, ctx, runtime, (uint32_t)Ps2GprU32(ctx, 4), (uint32_t)Ps2GprU32(ctx, 5), (uint32_t)Ps2GprU32(ctx, 6), (uint32_t)Ps2GprU32(ctx, 7)); }},
    {"constructor_camera_lws_anime_data", 0x1161c08u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { constructor_camera_lws_anime_data(rdram, ctx, runtime, (uint32_t)Ps2GprU32(ctx, 4)); }},
    {"set_camera_position", 0x1162e80u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { set_camera_position(rdram, ctx, runtime, (uint32_t)Ps2GprU32(ctx, 4), (uint32_t)Ps2GprU32(ctx, 5)); }},
    {"set_camera_direction", 0x1162e90u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { set_camera_direction(rdram, ctx, runtime, (uint32_t)Ps2GprU32(ctx, 4), (uint32_t)Ps2GprU32(ctx, 5)); }},
    {"set_camera_up_direction", 0x1162ec0u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { set_camera_up_direction(rdram, ctx, runtime, (uint32_t)Ps2GprU32(ctx, 4), (uint32_t)Ps2GprU32(ctx, 5)); }},
    {"InitDamageClass", 0x1166da0u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { InitDamageClass(rdram, ctx, runtime, (uint32_t)Ps2GprU32(ctx, 4)); }},
    {"GAMEUTIL_set_custom_boss_interest_position", 0x1343b38u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { GAMEUTIL_set_custom_boss_interest_position(rdram, ctx, runtime, (uint32_t)Ps2GprU32(ctx, 4)); }},
    {"GAMEUTIL_constructor_col_plane_info", 0x1343fe0u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { GAMEUTIL_constructor_col_plane_info(rdram, ctx, runtime, (uint32_t)Ps2GprU32(ctx, 4)); }},
    {"GAMEUTIL_reset_position_filter", 0x13441e8u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { GAMEUTIL_reset_position_filter(rdram, ctx, runtime, (uint32_t)Ps2GprU32(ctx, 4), (uint32_t)Ps2GprU32(ctx, 5)); }},
    {"GAMEUTIL_doit_position_filter", 0x1344210u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { GAMEUTIL_doit_position_filter(rdram, ctx, runtime, (uint32_t)Ps2GprU32(ctx, 4), (uint32_t)Ps2GprU32(ctx, 5), (uint64_t)Ps2GprU64(ctx, 6)); }},
    {"SetCamFinalizerGeometry", 0x134a7d0u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { SetCamFinalizerGeometry(rdram, ctx, runtime, (uint32_t)Ps2GprU32(ctx, 4), (uint32_t)Ps2GprU32(ctx, 5)); }},
    {"fruitsFlagSet", 0x134f150u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { fruitsFlagSet(rdram, ctx, runtime, (uint64_t)Ps2GprU64(ctx, 4), (uint64_t)Ps2GprU64(ctx, 5), (uint32_t)Ps2GprU32(ctx, 7)); }},
    {"gamecore_sub_0137EA00", 0x137ea00u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { gamecore_sub_0137EA00(rdram, ctx, runtime, (uint32_t)Ps2GprU32(ctx, 4)); }},
    {"Init_TimeFVECTORInter", 0x139a2a0u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { Init_TimeFVECTORInter(rdram, ctx, runtime, (uint32_t)Ps2GprU32(ctx, 4), (uint32_t)Ps2GprU32(ctx, 5), (uint32_t)Ps2GprU32(ctx, 6), ctx->f[12], ctx->f[13]); }},
    {"Update_TimeFVECTORInter", 0x139a2c8u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { Update_TimeFVECTORInter(rdram, ctx, runtime, (uint32_t)Ps2GprU32(ctx, 4)); }},
    {"CalcFootVec", 0x13a7218u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { CalcFootVec(rdram, ctx, runtime, (uint32_t)Ps2GprU32(ctx, 4), (uint32_t)Ps2GprU32(ctx, 5), (uint32_t)Ps2GprU32(ctx, 6), (uint32_t)Ps2GprU32(ctx, 7)); }},
    {"MVHIST_Init", 0x13a74b0u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { MVHIST_Init(rdram, ctx, runtime, (uint64_t)Ps2GprU64(ctx, 4)); }},
    {"get_predict_hit_position_nico_sight_on", 0x13aa498u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { get_predict_hit_position_nico_sight_on(rdram, ctx, runtime, (uint32_t)Ps2GprU32(ctx, 4), (uint32_t)Ps2GprU32(ctx, 5)); }},
    {"AI_InitAnimalLayout", 0x13d3980u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { AI_InitAnimalLayout(rdram, ctx, runtime, (uint32_t)Ps2GprU32(ctx, 4)); }},
    {"AI_SetAnimalLayout", 0x13d39a0u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { AI_SetAnimalLayout(rdram, ctx, runtime, (uint32_t)Ps2GprU32(ctx, 4), (uint32_t)Ps2GprU32(ctx, 5), ctx->f[12], ctx->f[13]); }},
    {"AI_CheckInAnimalLayout", 0x13d39b8u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { AI_CheckInAnimalLayout(rdram, ctx, runtime, (uint32_t)Ps2GprU32(ctx, 4), (uint32_t)Ps2GprU32(ctx, 5)); }},
    {"AI_GetMotionBasicVelocity", 0x13d3fa0u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { AI_GetMotionBasicVelocity(rdram, ctx, runtime, (uint32_t)Ps2GprU32(ctx, 4), (uint32_t)Ps2GprU32(ctx, 5), ctx->f[12]); }},
    {"AI_GetAccelA3XYZ", 0x13d41a0u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { AI_GetAccelA3XYZ(rdram, ctx, runtime, (uint32_t)Ps2GprU32(ctx, 4), (uint32_t)Ps2GprU32(ctx, 5), (uint32_t)Ps2GprU32(ctx, 6), (uint32_t)Ps2GprU32(ctx, 7), (uint32_t)Ps2GprU32(ctx, 8), (uint32_t)Ps2GprU32(ctx, 9), ctx->f[12]); }},
    {"AI_GetAccelB3XYZ", 0x13d4308u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { AI_GetAccelB3XYZ(rdram, ctx, runtime, (uint32_t)Ps2GprU32(ctx, 4), (uint32_t)Ps2GprU32(ctx, 5), (uint32_t)Ps2GprU32(ctx, 6), (uint32_t)Ps2GprU32(ctx, 7), (uint32_t)Ps2GprU32(ctx, 8), (uint32_t)Ps2GprU32(ctx, 9), ctx->f[12]); }},
    {"AI_GetAccelC3XYZ", 0x13d4458u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { AI_GetAccelC3XYZ(rdram, ctx, runtime, (uint32_t)Ps2GprU32(ctx, 4), (uint32_t)Ps2GprU32(ctx, 5), (uint32_t)Ps2GprU32(ctx, 6), (uint32_t)Ps2GprU32(ctx, 7), (uint32_t)Ps2GprU32(ctx, 8), (uint32_t)Ps2GprU32(ctx, 9), ctx->f[12]); }},
    {"AI_GetVelocity3XYZ", 0x13d4638u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { AI_GetVelocity3XYZ(rdram, ctx, runtime, (uint32_t)Ps2GprU32(ctx, 4), (uint32_t)Ps2GprU32(ctx, 5), (uint32_t)Ps2GprU32(ctx, 6), (uint32_t)Ps2GprU32(ctx, 7), (uint32_t)Ps2GprU32(ctx, 8), ctx->f[12]); }},
    {"AI_GetPosition3XYZ", 0x13d4740u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { AI_GetPosition3XYZ(rdram, ctx, runtime, (uint32_t)Ps2GprU32(ctx, 4), (uint32_t)Ps2GprU32(ctx, 5), (uint32_t)Ps2GprU32(ctx, 6), (uint32_t)Ps2GprU32(ctx, 7), (uint32_t)Ps2GprU32(ctx, 8), (uint32_t)Ps2GprU32(ctx, 9), ctx->f[12]); }},
    {"AI_GetTrgDir", 0x13d4d30u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { AI_GetTrgDir(rdram, ctx, runtime, (uint32_t)Ps2GprU32(ctx, 4), (uint32_t)Ps2GprU32(ctx, 5), ctx->f[12]); }},
    {"AI_GetSideSignalFromDirLev", 0x13d4ef8u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { AI_GetSideSignalFromDirLev(rdram, ctx, runtime, (uint32_t)Ps2GprU32(ctx, 4), (uint32_t)Ps2GprU32(ctx, 5)); }},
    {"gamecore_sub_013D8DC8", 0x13d8dc8u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { gamecore_sub_013D8DC8(rdram, ctx, runtime, (uint64_t)Ps2GprU64(ctx, 4)); }},
    {"gamecore_sub_013D8FA0", 0x13d8fa0u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { gamecore_sub_013D8FA0(rdram, ctx, runtime, (uint64_t)Ps2GprU64(ctx, 4)); }},
    {"gamecore_sub_013D9518", 0x13d9518u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { gamecore_sub_013D9518(rdram, ctx, runtime, (uint64_t)Ps2GprU64(ctx, 4)); }},
    {"Kite_InitKitePathPoint", 0x13e79f8u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { Kite_InitKitePathPoint(rdram, ctx, runtime, (uint32_t)Ps2GprU32(ctx, 4)); }},
    {"Kite_SetAnimalLayout", 0x13e7c78u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { Kite_SetAnimalLayout(rdram, ctx, runtime, (uint32_t)Ps2GprU32(ctx, 4), (uint32_t)Ps2GprU32(ctx, 5), ctx->f[12], ctx->f[13], ctx->f[14]); }},
    {"Kite_SetPathOfs", 0x13e7cc8u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { Kite_SetPathOfs(rdram, ctx, runtime, (uint32_t)Ps2GprU32(ctx, 4), (uint32_t)Ps2GprU32(ctx, 5)); }},
    {"sprWeaponSetMarker", 0x1407568u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { sprWeaponSetMarker(rdram, ctx, runtime, (uint64_t)Ps2GprU64(ctx, 4), (uint32_t)Ps2GprU32(ctx, 5), (uint64_t)Ps2GprU64(ctx, 6)); }},
    {"sprWeaponGetMarkerPos", 0x14075a0u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { sprWeaponGetMarkerPos(rdram, ctx, runtime, (uint32_t)Ps2GprU32(ctx, 4)); }},
    {"gamecore_sub_01411708", 0x1411708u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { gamecore_sub_01411708(rdram, ctx, runtime, (uint32_t)Ps2GprU32(ctx, 4), (uint32_t)Ps2GprU32(ctx, 5)); }},
    {"gamecore_sub_01411AF8", 0x1411af8u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { gamecore_sub_01411AF8(rdram, ctx, runtime, (uint32_t)Ps2GprU32(ctx, 4)); }},
    {"constructor_camera_pause_rise", 0x1412970u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { constructor_camera_pause_rise(rdram, ctx, runtime, (uint32_t)Ps2GprU32(ctx, 4)); }},
    {"gamecore_sub_01413720", 0x1413720u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { gamecore_sub_01413720(rdram, ctx, runtime, (uint32_t)Ps2GprU32(ctx, 4)); }},
    {"constructor_camera_target_lost_angle", 0x1414db0u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { constructor_camera_target_lost_angle(rdram, ctx, runtime, (uint32_t)Ps2GprU32(ctx, 4)); }},
    {"reset_camera_target_visible_angle_manager", 0x1415970u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { reset_camera_target_visible_angle_manager(rdram, ctx, runtime, (uint32_t)Ps2GprU32(ctx, 4)); }},
    {"CAM_get_camera_position_by_look_at", 0x1419b48u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { CAM_get_camera_position_by_look_at(rdram, ctx, runtime, (uint32_t)Ps2GprU32(ctx, 4), (uint32_t)Ps2GprU32(ctx, 5), (uint32_t)Ps2GprU32(ctx, 6), ctx->f[12]); }},
    {"CAM_add_plane_offset_in_position", 0x1419f68u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { CAM_add_plane_offset_in_position(rdram, ctx, runtime, (uint32_t)Ps2GprU32(ctx, 4), (uint32_t)Ps2GprU32(ctx, 5), (uint32_t)Ps2GprU32(ctx, 6), ctx->f[12]); }},
    {"CAM_set_all_camera_base_data", 0x141a140u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { CAM_set_all_camera_base_data(rdram, ctx, runtime, (uint32_t)Ps2GprU32(ctx, 4), (uint32_t)Ps2GprU32(ctx, 5), (uint32_t)Ps2GprU32(ctx, 6), ctx->f[12], ctx->f[13], ctx->f[14]); }},
    {"CAM_constructor_base_look_at", 0x141b110u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { CAM_constructor_base_look_at(rdram, ctx, runtime, (uint32_t)Ps2GprU32(ctx, 4)); }},
    {"copy_mode_common_camera_data", 0x1424c30u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { copy_mode_common_camera_data(rdram, ctx, runtime, (uint32_t)Ps2GprU32(ctx, 4), (uint32_t)Ps2GprU32(ctx, 5)); }},
    {"get_boss_interest_position_game_camera", 0x1425378u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { get_boss_interest_position_game_camera(rdram, ctx, runtime, (uint32_t)Ps2GprU32(ctx, 4), (uint32_t)Ps2GprU32(ctx, 5)); }},
    {"setAttainRangeData", 0x1432030u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { setAttainRangeData(rdram, ctx, runtime, (uint32_t)Ps2GprU32(ctx, 4), (uint32_t)Ps2GprU32(ctx, 5), ctx->f[12], ctx->f[13], ctx->f[14], ctx->f[15]); }},
    {"gamecore_sub_01433F98", 0x1433f98u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { gamecore_sub_01433F98(rdram, ctx, runtime, (uint64_t)Ps2GprU64(ctx, 4), (uint32_t)Ps2GprU32(ctx, 5), (uint32_t)Ps2GprU32(ctx, 6), (uint32_t)Ps2GprU32(ctx, 7), (uint32_t)Ps2GprU32(ctx, 8)); }},
    {"gamecore_sub_014340F0", 0x14340f0u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { gamecore_sub_014340F0(rdram, ctx, runtime, (uint32_t)Ps2GprU32(ctx, 4)); }},
    {"gamecore_sub_0143D718", 0x143d718u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { gamecore_sub_0143D718(rdram, ctx, runtime, (uint32_t)Ps2GprU32(ctx, 4)); }},
    {"gamecore_sub_01440FE0", 0x1440fe0u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { gamecore_sub_01440FE0(rdram, ctx, runtime, (uint32_t)Ps2GprU32(ctx, 4)); }},
    {"setPosGazeCtrlWork", 0x14418f0u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { setPosGazeCtrlWork(rdram, ctx, runtime, (uint32_t)Ps2GprU32(ctx, 4), (uint32_t)Ps2GprU32(ctx, 5)); }},
    {"gamecore_sub_01444568", 0x1444568u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { gamecore_sub_01444568(rdram, ctx, runtime, (uint32_t)Ps2GprU32(ctx, 4)); }},
    {"resetVelocityWormCtrlWork", 0x144cdb8u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { resetVelocityWormCtrlWork(rdram, ctx, runtime, (uint32_t)Ps2GprU32(ctx, 5)); }},
    {"initAimIKWork", 0x145d690u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { initAimIKWork(rdram, ctx, runtime, (uint32_t)Ps2GprU32(ctx, 6)); }},
    {"initAttackWaistIKWork", 0x145dfd0u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { initAttackWaistIKWork(rdram, ctx, runtime, (uint32_t)Ps2GprU32(ctx, 6)); }},
    {"updateGazeIKWork", 0x1461850u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { updateGazeIKWork(rdram, ctx, runtime, (uint32_t)Ps2GprU32(ctx, 4), (uint32_t)Ps2GprU32(ctx, 5)); }},
    {"ReflectIK_initWormDynamics", 0x1462310u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { ReflectIK_initWormDynamics(rdram, ctx, runtime, (uint32_t)Ps2GprU32(ctx, 4), (uint32_t)Ps2GprU32(ctx, 5), (uint32_t)Ps2GprU32(ctx, 6), (uint32_t)Ps2GprU32(ctx, 7), ctx->f[12]); }},
    {"GAMEUTIL_get_spring_interpolate_offset3D", 0x1083e38u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { GAMEUTIL_get_spring_interpolate_offset3D(rdram, ctx, runtime, (uint64_t)Ps2GprU64(ctx, 4), (uint32_t)Ps2GprU32(ctx, 5), (uint32_t)Ps2GprU32(ctx, 6), ctx->f[12], ctx->f[13], ctx->f[14], ctx->f[15], ctx->f[20], ctx->f[21], ctx->f[22], ctx->f[23], ctx->f[24], ctx->f[25], ctx->f[26]); }},
    {"get_temp_parameter_motion_ik_base", 0x10b9070u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { get_temp_parameter_motion_ik_base(rdram, ctx, runtime, (uint64_t)Ps2GprU64(ctx, 4), (uint32_t)Ps2GprU32(ctx, 5)); }},
    {"GAMEUTIL_init_col_plane_info_filter", 0x1344040u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { GAMEUTIL_init_col_plane_info_filter(rdram, ctx, runtime, (uint64_t)Ps2GprU64(ctx, 4), ctx->f[12], ctx->f[13]); }},
    {"gamecore_sub_01381AA8", 0x1381aa8u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { gamecore_sub_01381AA8(rdram, ctx, runtime, (uint64_t)Ps2GprU64(ctx, 4)); }},
    {"gamecore_sub_013DC4A8", 0x13dc4a8u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { gamecore_sub_013DC4A8(rdram, ctx, runtime, (uint64_t)Ps2GprU64(ctx, 5), (uint64_t)Ps2GprU64(ctx, 8), ctx->f[20]); }},
    {"Kite_InitSpecialPathWork", 0x13e7900u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { Kite_InitSpecialPathWork(rdram, ctx, runtime, (uint32_t)Ps2GprU32(ctx, 4)); }},
    {"Kite_SetSpecialPathLayout", 0x13e7ce0u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { Kite_SetSpecialPathLayout(rdram, ctx, runtime, (uint32_t)Ps2GprU32(ctx, 4), (uint32_t)Ps2GprU32(ctx, 5), ctx->f[12], ctx->f[13]); }},
    {"gamecore_sub_01411AD0", 0x1411ad0u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { gamecore_sub_01411AD0(rdram, ctx, runtime, (uint64_t)Ps2GprU64(ctx, 4)); }},
    {"init_camera_pause_rise", 0x14129c0u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { init_camera_pause_rise(rdram, ctx, runtime, (uint64_t)Ps2GprU64(ctx, 4), (uint64_t)Ps2GprU64(ctx, 5)); }},
    {"gamecore_sub_01440F60", 0x1440f60u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { gamecore_sub_01440F60(rdram, ctx, runtime, (uint64_t)Ps2GprU64(ctx, 5)); }},
    {"gamecore_sub_01444518", 0x1444518u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { gamecore_sub_01444518(rdram, ctx, runtime, (uint64_t)Ps2GprU64(ctx, 5)); }},
    {"gamecore_sub_0144CDF8", 0x144cdf8u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { gamecore_sub_0144CDF8(rdram, ctx, runtime, (uint64_t)Ps2GprU64(ctx, 4), (uint64_t)Ps2GprU64(ctx, 5)); }},
};
const uint32_t kEntryCount = 188u;
}
#if !defined(A4_OFFLINE)
void a4ckReport(unsigned long long frame) { a4ck::ckReport(frame); }
#endif
#endif
