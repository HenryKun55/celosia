// a2_typed_check.cpp: round A2's differential check of the typed functions (tools/typed_lift2.py --check-tu;
// docs/decomp_a2.md). Generated: do not edit. Compiled to code only with PS2X_A2_CHECK=1 (a check build) or
// A2_OFFLINE (the offline harness, tools/a2_harness.cpp).
#include "typed_a2_check_rt.h"
#if PS2X_A2_CHECK || defined(A2_OFFLINE)
namespace a2ck
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
#define PS2_CALL_GUEST_DIRECT_NOCHK(t, f, r, s, k, n) a2ck::refCall((t), (r), rdram, ctx, runtime)
#define PS2_CALL_GUEST_NOCHK(t, f, r, s, k, n) a2ck::refCall((t), (r), rdram, ctx, runtime)
#define PS2_CALL_GUEST_DIRECT(t, f, r, s, k, n) a2ck::refCall((t), (r), rdram, ctx, runtime)
#define PS2_CALL_GUEST_KNOWN(t, f, r, s, k, n) a2ck::refCall((t), (r), rdram, ctx, runtime)
#define PS2_TAIL_GUEST_DIRECT(t, f) (void)a2ck::refCall((t), 0u, rdram, ctx, runtime)
#define PS2_TAIL_GUEST_KNOWN(t, f) (void)a2ck::refCall((t), 0u, rdram, ctx, runtime)

namespace a2ck
{
namespace ref
{
using lift_d1::sx64;
#include "typed_a1_ref.inc"
#include "typed_a2_ref.inc"
}
namespace ty
{
#include "typed_a1.inc"
#include "typed_a2.inc"
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
    default: ckUnknownCall(t); return true;
    }
    (void)ret;
    return true;
}
void memprintf(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, float pf12, float pf13, float pf14, float pf15, float pf16, float pf17, float pf18, float pf19)
{
    if (!ckBegin(0, rdram, runtime)) return;
    R5900Context &c = a1ck::ckCtx(ctx);
    ref::memprintf(rdram, &c, runtime);
    a1ck::ckMid();
    ty::memprintf(rdram, runtime, pf12, pf13, pf14, pf15, pf16, pf17, pf18, pf19);
    ckEnd(0, ctx, c, 0, 0u, 0.0f, 0x0u, 0x0u);
}
void imemprintf(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, float pf12, float pf13, float pf14, float pf15, float pf16, float pf17, float pf18, float pf19)
{
    if (!ckBegin(1, rdram, runtime)) return;
    R5900Context &c = a1ck::ckCtx(ctx);
    ref::imemprintf(rdram, &c, runtime);
    a1ck::ckMid();
    ty::imemprintf(rdram, runtime, pf12, pf13, pf14, pf15, pf16, pf17, pf18, pf19);
    ckEnd(1, ctx, c, 0, 0u, 0.0f, 0x0u, 0x0u);
}
void PutString(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, float pf12, float pf13, float pf14, float pf15, float pf16, float pf17, float pf18, float pf19)
{
    if (!ckBegin(2, rdram, runtime)) return;
    R5900Context &c = a1ck::ckCtx(ctx);
    ref::PutString(rdram, &c, runtime);
    a1ck::ckMid();
    ty::PutString(rdram, runtime, pf12, pf13, pf14, pf15, pf16, pf17, pf18, pf19);
    ckEnd(2, ctx, c, 0, 0u, 0.0f, 0x0u, 0x0u);
}
void PutStringS(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, float pf12, float pf13, float pf14, float pf15, float pf16, float pf17, float pf18, float pf19)
{
    if (!ckBegin(3, rdram, runtime)) return;
    R5900Context &c = a1ck::ckCtx(ctx);
    ref::PutStringS(rdram, &c, runtime);
    a1ck::ckMid();
    ty::PutStringS(rdram, runtime, pf12, pf13, pf14, pf15, pf16, pf17, pf18, pf19);
    ckEnd(3, ctx, c, 0, 0u, 0.0f, 0x0u, 0x0u);
}
void sub_00108D10(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint64_t p4, uint32_t p5, uint64_t p6)
{
    if (!ckBegin(4, rdram, runtime)) return;
    R5900Context &c = a1ck::ckCtx(ctx);
    ref::sub_00108D10(rdram, &c, runtime);
    a1ck::ckMid();
    const uint64_t r = (ty::sub_00108D10(rdram, runtime, p4, p5, p6));
    ckEnd(4, ctx, c, 2, r, 0.0f, 0xd8u, 0x0u);
}
void sub_00116C58(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (!ckBegin(5, rdram, runtime)) return;
    R5900Context &c = a1ck::ckCtx(ctx);
    ref::sub_00116C58(rdram, &c, runtime);
    a1ck::ckMid();
    ty::sub_00116C58(rdram, runtime);
    ckEnd(5, ctx, c, 0, 0u, 0.0f, 0x0u, 0x0u);
}
void ps2___fpcmp_parts_d(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint64_t p4, uint32_t p5)
{
    if (!ckBegin(6, rdram, runtime)) return;
    R5900Context &c = a1ck::ckCtx(ctx);
    ref::ps2___fpcmp_parts_d(rdram, &c, runtime);
    a1ck::ckMid();
    const uint64_t r = (ty::ps2___fpcmp_parts_d(rdram, runtime, p4, p5));
    ckEnd(6, ctx, c, 2, r, 0.0f, 0xd8u, 0x0u);
}
void iosCdStSectorAlignSkip(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (!ckBegin(7, rdram, runtime)) return;
    R5900Context &c = a1ck::ckCtx(ctx);
    ref::iosCdStSectorAlignSkip(rdram, &c, runtime);
    a1ck::ckMid();
    const uint64_t r = (ty::iosCdStSectorAlignSkip(rdram, runtime));
    ckEnd(7, ctx, c, 2, r, 0.0f, 0x78u, 0x0u);
}
void iosGetTCount(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (!ckBegin(8, rdram, runtime)) return;
    R5900Context &c = a1ck::ckCtx(ctx);
    ref::iosGetTCount(rdram, &c, runtime);
    a1ck::ckMid();
    const uint64_t r = (ty::iosGetTCount(rdram, runtime));
    ckEnd(8, ctx, c, 2, r, 0.0f, 0x8u, 0x0u);
}
void iosGetCpuRapCountPar1Int(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (!ckBegin(9, rdram, runtime)) return;
    R5900Context &c = a1ck::ckCtx(ctx);
    ref::iosGetCpuRapCountPar1Int(rdram, &c, runtime);
    a1ck::ckMid();
    const a1::RetVF r = ty::iosGetCpuRapCountPar1Int(rdram, runtime);
    ckEnd(9, ctx, c, 4, r.v0, r.f0, 0x1au, 0x2u);
}
void iosSprToMemDmaAdrCopy(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t p4, uint32_t p5, uint32_t p6)
{
    if (!ckBegin(10, rdram, runtime)) return;
    R5900Context &c = a1ck::ckCtx(ctx);
    ref::iosSprToMemDmaAdrCopy(rdram, &c, runtime);
    a1ck::ckMid();
    const uint64_t r = a1_sx64(ty::iosSprToMemDmaAdrCopy(rdram, runtime, p4, p5, p6));
    ckEnd(10, ctx, c, 1, r, 0.0f, 0xf88u, 0x0u);
}
void kernel_sub_001B4120(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, float pf12, float pf13, float pf14, float pf15, float pf16, float pf17, float pf18, float pf19)
{
    if (!ckBegin(11, rdram, runtime)) return;
    R5900Context &c = a1ck::ckCtx(ctx);
    ref::kernel_sub_001B4120(rdram, &c, runtime);
    a1ck::ckMid();
    const uint64_t r = (ty::kernel_sub_001B4120(rdram, runtime, pf12, pf13, pf14, pf15, pf16, pf17, pf18, pf19));
    ckEnd(11, ctx, c, 2, r, 0.0f, 0x0u, 0x0u);
}
void PopQuaternion(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (!ckBegin(12, rdram, runtime)) return;
    R5900Context &c = a1ck::ckCtx(ctx);
    ref::PopQuaternion(rdram, &c, runtime);
    a1ck::ckMid();
    const uint64_t r = a1_sx64(ty::PopQuaternion(rdram, runtime));
    ckEnd(12, ctx, c, 1, r, 0.0f, 0x18u, 0x0u);
}
void iosGetACosf(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, float pf12)
{
    if (!ckBegin(13, rdram, runtime)) return;
    R5900Context &c = a1ck::ckCtx(ctx);
    ref::iosGetACosf(rdram, &c, runtime);
    a1ck::ckMid();
    const float r = ty::iosGetACosf(rdram, runtime, pf12);
    ckEnd(13, ctx, c, 3, 0u, r, 0x2u, 0x7eu);
}
void iosDlGetCreateAdrs(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (!ckBegin(14, rdram, runtime)) return;
    R5900Context &c = a1ck::ckCtx(ctx);
    ref::iosDlGetCreateAdrs(rdram, &c, runtime);
    a1ck::ckMid();
    const uint64_t r = a1_sx64(ty::iosDlGetCreateAdrs(rdram, runtime));
    ckEnd(14, ctx, c, 1, r, 0.0f, 0x18u, 0x0u);
}
void getVIntrCnt(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (!ckBegin(15, rdram, runtime)) return;
    R5900Context &c = a1ck::ckCtx(ctx);
    ref::getVIntrCnt(rdram, &c, runtime);
    a1ck::ckMid();
    const uint64_t r = a1_sx64(ty::getVIntrCnt(rdram, runtime));
    ckEnd(15, ctx, c, 1, r, 0.0f, 0x8u, 0x0u);
}
void setVIntrBaseTime(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t p4)
{
    if (!ckBegin(16, rdram, runtime)) return;
    R5900Context &c = a1ck::ckCtx(ctx);
    ref::setVIntrBaseTime(rdram, &c, runtime);
    a1ck::ckMid();
    const uint64_t r = a1_sx64(ty::setVIntrBaseTime(rdram, runtime, p4));
    ckEnd(16, ctx, c, 1, r, 0.0f, 0x8u, 0x0u);
}
void iosGetFontDataWithSJIS(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint64_t p4, uint64_t p5, uint64_t p6)
{
    if (!ckBegin(17, rdram, runtime)) return;
    R5900Context &c = a1ck::ckCtx(ctx);
    ref::iosGetFontDataWithSJIS(rdram, &c, runtime);
    a1ck::ckMid();
    const uint64_t r = (ty::iosGetFontDataWithSJIS(rdram, runtime, p4, p5, p6));
    ckEnd(17, ctx, c, 2, r, 0.0f, 0x1ff8u, 0x0u);
}
void iosGetFontIDWithSJIS(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint64_t p4)
{
    if (!ckBegin(18, rdram, runtime)) return;
    R5900Context &c = a1ck::ckCtx(ctx);
    ref::iosGetFontIDWithSJIS(rdram, &c, runtime);
    a1ck::ckMid();
    const uint64_t r = (ty::iosGetFontIDWithSJIS(rdram, runtime, p4));
    ckEnd(18, ctx, c, 2, r, 0.0f, 0x3f8u, 0x0u);
}
void kernel_sub_001D1420(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (!ckBegin(19, rdram, runtime)) return;
    R5900Context &c = a1ck::ckCtx(ctx);
    ref::kernel_sub_001D1420(rdram, &c, runtime);
    a1ck::ckMid();
    const uint64_t r = a1_sx64(ty::kernel_sub_001D1420(rdram, runtime));
    ckEnd(19, ctx, c, 1, r, 0.0f, 0xf8u, 0x0u);
}
void kernel_sub_001D15B8(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (!ckBegin(20, rdram, runtime)) return;
    R5900Context &c = a1ck::ckCtx(ctx);
    ref::kernel_sub_001D15B8(rdram, &c, runtime);
    a1ck::ckMid();
    const uint64_t r = (ty::kernel_sub_001D15B8(rdram, runtime));
    ckEnd(20, ctx, c, 2, r, 0.0f, 0x78u, 0x0u);
}
void vab_get_splitchunk(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint64_t p4)
{
    if (!ckBegin(21, rdram, runtime)) return;
    R5900Context &c = a1ck::ckCtx(ctx);
    ref::vab_get_splitchunk(rdram, &c, runtime);
    a1ck::ckMid();
    const uint64_t r = (ty::vab_get_splitchunk(rdram, runtime, p4));
    ckEnd(21, ctx, c, 2, r, 0.0f, 0x8u, 0x0u);
}
void kernel_sub_001DB418(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t p4, uint32_t p5)
{
    if (!ckBegin(22, rdram, runtime)) return;
    R5900Context &c = a1ck::ckCtx(ctx);
    ref::kernel_sub_001DB418(rdram, &c, runtime);
    a1ck::ckMid();
    const uint64_t r = (ty::kernel_sub_001DB418(rdram, runtime, p4, p5));
    ckEnd(22, ctx, c, 2, r, 0.0f, 0x28u, 0x0u);
}
void GAMEUTIL_is_yaw_pitch_get_direction(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t p4)
{
    if (!ckBegin(23, rdram, runtime)) return;
    R5900Context &c = a1ck::ckCtx(ctx);
    ref::GAMEUTIL_is_yaw_pitch_get_direction(rdram, &c, runtime);
    a1ck::ckMid();
    const a1::RetVF r = ty::GAMEUTIL_is_yaw_pitch_get_direction(rdram, runtime, p4);
    ckEnd(23, ctx, c, 4, r.v0, r.f0, 0x2u, 0x2u);
}
void entrymanager(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (!ckBegin(24, rdram, runtime)) return;
    R5900Context &c = a1ck::ckCtx(ctx);
    ref::entrymanager(rdram, &c, runtime);
    a1ck::ckMid();
    ty::entrymanager(rdram, runtime);
    ckEnd(24, ctx, c, 0, 0u, 0.0f, 0x0u, 0x0u);
}
void reset_double_spring_interp_cntrl(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t p4, float pf12)
{
    if (!ckBegin(25, rdram, runtime)) return;
    R5900Context &c = a1ck::ckCtx(ctx);
    ref::reset_double_spring_interp_cntrl(rdram, &c, runtime);
    a1ck::ckMid();
    const a1::RetVF r = ty::reset_double_spring_interp_cntrl(rdram, runtime, p4, pf12);
    ckEnd(25, ctx, c, 4, r.v0, r.f0, 0x0u, 0x0u);
}
void setShadowEffect(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (!ckBegin(26, rdram, runtime)) return;
    R5900Context &c = a1ck::ckCtx(ctx);
    ref::setShadowEffect(rdram, &c, runtime);
    a1ck::ckMid();
    const uint64_t r = a1_sx64(ty::setShadowEffect(rdram, runtime));
    ckEnd(26, ctx, c, 1, r, 0.0f, 0x8u, 0x0u);
}
void fsEffectDefault(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (!ckBegin(27, rdram, runtime)) return;
    R5900Context &c = a1ck::ckCtx(ctx);
    ref::fsEffectDefault(rdram, &c, runtime);
    a1ck::ckMid();
    const uint64_t r = a1_sx64(ty::fsEffectDefault(rdram, runtime));
    ckEnd(27, ctx, c, 1, r, 0.0f, 0x38u, 0x0u);
}
void deleteFsEffect(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (!ckBegin(28, rdram, runtime)) return;
    R5900Context &c = a1ck::ckCtx(ctx);
    ref::deleteFsEffect(rdram, &c, runtime);
    a1ck::ckMid();
    ty::deleteFsEffect(rdram, runtime);
    ckEnd(28, ctx, c, 0, 0u, 0.0f, 0x0u, 0x0u);
}
void deleteFogEffect(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (!ckBegin(29, rdram, runtime)) return;
    R5900Context &c = a1ck::ckCtx(ctx);
    ref::deleteFogEffect(rdram, &c, runtime);
    a1ck::ckMid();
    ty::deleteFogEffect(rdram, runtime);
    ckEnd(29, ctx, c, 0, 0u, 0.0f, 0x0u, 0x0u);
}
void manager_sub_01089288(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (!ckBegin(30, rdram, runtime)) return;
    R5900Context &c = a1ck::ckCtx(ctx);
    ref::manager_sub_01089288(rdram, &c, runtime);
    a1ck::ckMid();
    const uint64_t r = a1_sx64(ty::manager_sub_01089288(rdram, runtime));
    ckEnd(30, ctx, c, 1, r, 0.0f, 0x38u, 0x0u);
}
void getFogFactor0(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (!ckBegin(31, rdram, runtime)) return;
    R5900Context &c = a1ck::ckCtx(ctx);
    ref::getFogFactor0(rdram, &c, runtime);
    a1ck::ckMid();
    const a1::RetVF r = ty::getFogFactor0(rdram, runtime);
    ckEnd(31, ctx, c, 4, r.v0, r.f0, 0x0u, 0x1eu);
}
void ModelDispGetWorkSize(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (!ckBegin(32, rdram, runtime)) return;
    R5900Context &c = a1ck::ckCtx(ctx);
    ref::ModelDispGetWorkSize(rdram, &c, runtime);
    a1ck::ckMid();
    const uint64_t r = a1_sx64(ty::ModelDispGetWorkSize(rdram, runtime));
    ckEnd(32, ctx, c, 1, r, 0.0f, 0x0u, 0x0u);
}
void ptclCounterInit(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (!ckBegin(33, rdram, runtime)) return;
    R5900Context &c = a1ck::ckCtx(ctx);
    ref::ptclCounterInit(rdram, &c, runtime);
    a1ck::ckMid();
    ty::ptclCounterInit(rdram, runtime);
    ckEnd(33, ctx, c, 0, 0u, 0.0f, 0x0u, 0x0u);
}
void manager_sub_0109ACF8(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (!ckBegin(34, rdram, runtime)) return;
    R5900Context &c = a1ck::ckCtx(ctx);
    ref::manager_sub_0109ACF8(rdram, &c, runtime);
    a1ck::ckMid();
    ty::manager_sub_0109ACF8(rdram, runtime);
    ckEnd(34, ctx, c, 0, 0u, 0.0f, 0x0u, 0x0u);
}
void manager_sub_0109AD00(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (!ckBegin(35, rdram, runtime)) return;
    R5900Context &c = a1ck::ckCtx(ctx);
    ref::manager_sub_0109AD00(rdram, &c, runtime);
    a1ck::ckMid();
    ty::manager_sub_0109AD00(rdram, runtime);
    ckEnd(35, ctx, c, 0, 0u, 0.0f, 0x0u, 0x0u);
}
void shadowGetWorkSize(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (!ckBegin(36, rdram, runtime)) return;
    R5900Context &c = a1ck::ckCtx(ctx);
    ref::shadowGetWorkSize(rdram, &c, runtime);
    a1ck::ckMid();
    const uint64_t r = a1_sx64(ty::shadowGetWorkSize(rdram, runtime));
    ckEnd(36, ctx, c, 1, r, 0.0f, 0x0u, 0x0u);
}
void zFogInit(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (!ckBegin(37, rdram, runtime)) return;
    R5900Context &c = a1ck::ckCtx(ctx);
    ref::zFogInit(rdram, &c, runtime);
    a1ck::ckMid();
    ty::zFogInit(rdram, runtime);
    ckEnd(37, ctx, c, 0, 0u, 0.0f, 0x0u, 0x0u);
}
void get_type_parameter_segment_motion_ik_base(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t p4)
{
    if (!ckBegin(38, rdram, runtime)) return;
    R5900Context &c = a1ck::ckCtx(ctx);
    ref::get_type_parameter_segment_motion_ik_base(rdram, &c, runtime);
    a1ck::ckMid();
    const uint64_t r = a1_sx64(ty::get_type_parameter_segment_motion_ik_base(rdram, runtime, p4));
    ckEnd(38, ctx, c, 1, r, 0.0f, 0x78u, 0x0u);
}
void request_end_motion_ik_interest(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t p4)
{
    if (!ckBegin(39, rdram, runtime)) return;
    R5900Context &c = a1ck::ckCtx(ctx);
    ref::request_end_motion_ik_interest(rdram, &c, runtime);
    a1ck::ckMid();
    const uint64_t r = (ty::request_end_motion_ik_interest(rdram, runtime, p4));
    ckEnd(39, ctx, c, 2, r, 0.0f, 0x8u, 0x0u);
}
void getSizeGazeIKWork(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (!ckBegin(40, rdram, runtime)) return;
    R5900Context &c = a1ck::ckCtx(ctx);
    ref::getSizeGazeIKWork(rdram, &c, runtime);
    a1ck::ckMid();
    const uint64_t r = a1_sx64(ty::getSizeGazeIKWork(rdram, runtime));
    ckEnd(40, ctx, c, 1, r, 0.0f, 0x0u, 0x0u);
}
void GetAnimationDefClass(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint64_t p4)
{
    if (!ckBegin(41, rdram, runtime)) return;
    R5900Context &c = a1ck::ckCtx(ctx);
    ref::GetAnimationDefClass(rdram, &c, runtime);
    a1ck::ckMid();
    const uint64_t r = (ty::GetAnimationDefClass(rdram, runtime, p4));
    ckEnd(41, ctx, c, 2, r, 0.0f, 0x18u, 0x0u);
}
void manager_sub_010BB208(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (!ckBegin(42, rdram, runtime)) return;
    R5900Context &c = a1ck::ckCtx(ctx);
    ref::manager_sub_010BB208(rdram, &c, runtime);
    a1ck::ckMid();
    ty::manager_sub_010BB208(rdram, runtime);
    ckEnd(42, ctx, c, 0, 0u, 0.0f, 0x0u, 0x0u);
}
void RequestCharacterMotionShiftFunc(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint64_t p4, uint32_t p5)
{
    if (!ckBegin(43, rdram, runtime)) return;
    R5900Context &c = a1ck::ckCtx(ctx);
    ref::RequestCharacterMotionShiftFunc(rdram, &c, runtime);
    a1ck::ckMid();
    const uint64_t r = a1_sx64(ty::RequestCharacterMotionShiftFunc(rdram, runtime, p4, p5));
    ckEnd(43, ctx, c, 1, r, 0.0f, 0x58u, 0x0u);
}
void SetCharacterObjControlCallBackFunc(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint64_t p4, uint32_t p5)
{
    if (!ckBegin(44, rdram, runtime)) return;
    R5900Context &c = a1ck::ckCtx(ctx);
    ref::SetCharacterObjControlCallBackFunc(rdram, &c, runtime);
    a1ck::ckMid();
    const uint64_t r = a1_sx64(ty::SetCharacterObjControlCallBackFunc(rdram, runtime, p4, p5));
    ckEnd(44, ctx, c, 1, r, 0.0f, 0x58u, 0x0u);
}
void SetInertiaRotInterpMode(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint64_t p4)
{
    if (!ckBegin(45, rdram, runtime)) return;
    R5900Context &c = a1ck::ckCtx(ctx);
    ref::SetInertiaRotInterpMode(rdram, &c, runtime);
    a1ck::ckMid();
    const uint64_t r = (ty::SetInertiaRotInterpMode(rdram, runtime, p4));
    ckEnd(45, ctx, c, 2, r, 0.0f, 0xf8u, 0x0u);
}
void charrootdyn_getSlideCollisionJudgeCBArg(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint64_t p4, uint32_t p5, uint64_t p6)
{
    if (!ckBegin(46, rdram, runtime)) return;
    R5900Context &c = a1ck::ckCtx(ctx);
    ref::charrootdyn_getSlideCollisionJudgeCBArg(rdram, &c, runtime);
    a1ck::ckMid();
    const uint64_t r = (ty::charrootdyn_getSlideCollisionJudgeCBArg(rdram, runtime, p4, p5, p6));
    ckEnd(46, ctx, c, 2, r, 0.0f, 0x48u, 0x0u);
}
void manager_sub_010ED778(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (!ckBegin(47, rdram, runtime)) return;
    R5900Context &c = a1ck::ckCtx(ctx);
    ref::manager_sub_010ED778(rdram, &c, runtime);
    a1ck::ckMid();
    ty::manager_sub_010ED778(rdram, runtime);
    ckEnd(47, ctx, c, 0, 0u, 0.0f, 0x0u, 0x0u);
}
void FlushJudgeVolumeClass(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (!ckBegin(48, rdram, runtime)) return;
    R5900Context &c = a1ck::ckCtx(ctx);
    ref::FlushJudgeVolumeClass(rdram, &c, runtime);
    a1ck::ckMid();
    ty::FlushJudgeVolumeClass(rdram, runtime);
    ckEnd(48, ctx, c, 0, 0u, 0.0f, 0x0u, 0x0u);
}
void GetGameLightClass(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint64_t p4)
{
    if (!ckBegin(49, rdram, runtime)) return;
    R5900Context &c = a1ck::ckCtx(ctx);
    ref::GetGameLightClass(rdram, &c, runtime);
    a1ck::ckMid();
    const uint64_t r = (ty::GetGameLightClass(rdram, runtime, p4));
    ckEnd(49, ctx, c, 2, r, 0.0f, 0x18u, 0x0u);
}
void GetGlobalLightClass(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint64_t p4)
{
    if (!ckBegin(50, rdram, runtime)) return;
    R5900Context &c = a1ck::ckCtx(ctx);
    ref::GetGlobalLightClass(rdram, &c, runtime);
    a1ck::ckMid();
    const uint64_t r = (ty::GetGlobalLightClass(rdram, runtime, p4));
    ckEnd(50, ctx, c, 2, r, 0.0f, 0x18u, 0x0u);
}
void GetAmbientLightClass(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint64_t p4)
{
    if (!ckBegin(51, rdram, runtime)) return;
    R5900Context &c = a1ck::ckCtx(ctx);
    ref::GetAmbientLightClass(rdram, &c, runtime);
    a1ck::ckMid();
    const uint64_t r = (ty::GetAmbientLightClass(rdram, runtime, p4));
    ckEnd(51, ctx, c, 2, r, 0.0f, 0x18u, 0x0u);
}
void manager_sub_010F9528(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (!ckBegin(52, rdram, runtime)) return;
    R5900Context &c = a1ck::ckCtx(ctx);
    ref::manager_sub_010F9528(rdram, &c, runtime);
    a1ck::ckMid();
    ty::manager_sub_010F9528(rdram, runtime);
    ckEnd(52, ctx, c, 0, 0u, 0.0f, 0x0u, 0x0u);
}
void createBinaryMemoryInfoWindow(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (!ckBegin(53, rdram, runtime)) return;
    R5900Context &c = a1ck::ckCtx(ctx);
    ref::createBinaryMemoryInfoWindow(rdram, &c, runtime);
    a1ck::ckMid();
    ty::createBinaryMemoryInfoWindow(rdram, runtime);
    ckEnd(53, ctx, c, 0, 0u, 0.0f, 0x0u, 0x0u);
}
void skelSegDel(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (!ckBegin(54, rdram, runtime)) return;
    R5900Context &c = a1ck::ckCtx(ctx);
    ref::skelSegDel(rdram, &c, runtime);
    a1ck::ckMid();
    ty::skelSegDel(rdram, runtime);
    ckEnd(54, ctx, c, 0, 0u, 0.0f, 0x0u, 0x0u);
}
void debugCreateAnimObjSegmentDisp(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (!ckBegin(55, rdram, runtime)) return;
    R5900Context &c = a1ck::ckCtx(ctx);
    ref::debugCreateAnimObjSegmentDisp(rdram, &c, runtime);
    a1ck::ckMid();
    ty::debugCreateAnimObjSegmentDisp(rdram, runtime);
    ckEnd(55, ctx, c, 0, 0u, 0.0f, 0x0u, 0x0u);
}
void debugDeleteAnimObjSegmentDisp(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (!ckBegin(56, rdram, runtime)) return;
    R5900Context &c = a1ck::ckCtx(ctx);
    ref::debugDeleteAnimObjSegmentDisp(rdram, &c, runtime);
    a1ck::ckMid();
    ty::debugDeleteAnimObjSegmentDisp(rdram, runtime);
    ckEnd(56, ctx, c, 0, 0u, 0.0f, 0x0u, 0x0u);
}
void debugCreateAnimObjSegmentDefDisp(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (!ckBegin(57, rdram, runtime)) return;
    R5900Context &c = a1ck::ckCtx(ctx);
    ref::debugCreateAnimObjSegmentDefDisp(rdram, &c, runtime);
    a1ck::ckMid();
    ty::debugCreateAnimObjSegmentDefDisp(rdram, runtime);
    ckEnd(57, ctx, c, 0, 0u, 0.0f, 0x0u, 0x0u);
}
void debugDeleteAnimObjSegmentDefDisp(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (!ckBegin(58, rdram, runtime)) return;
    R5900Context &c = a1ck::ckCtx(ctx);
    ref::debugDeleteAnimObjSegmentDefDisp(rdram, &c, runtime);
    a1ck::ckMid();
    ty::debugDeleteAnimObjSegmentDefDisp(rdram, runtime);
    ckEnd(58, ctx, c, 0, 0u, 0.0f, 0x0u, 0x0u);
}
void scriptIntrSegInit(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (!ckBegin(59, rdram, runtime)) return;
    R5900Context &c = a1ck::ckCtx(ctx);
    ref::scriptIntrSegInit(rdram, &c, runtime);
    a1ck::ckMid();
    ty::scriptIntrSegInit(rdram, runtime);
    ckEnd(59, ctx, c, 0, 0u, 0.0f, 0x0u, 0x0u);
}
void GetScriptAnimationClass(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint64_t p4)
{
    if (!ckBegin(60, rdram, runtime)) return;
    R5900Context &c = a1ck::ckCtx(ctx);
    ref::GetScriptAnimationClass(rdram, &c, runtime);
    a1ck::ckMid();
    const uint64_t r = (ty::GetScriptAnimationClass(rdram, runtime, p4));
    ckEnd(60, ctx, c, 2, r, 0.0f, 0x18u, 0x0u);
}
void SetScriptAnimationObjChildEntryGroup(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t p4, uint32_t p5)
{
    if (!ckBegin(61, rdram, runtime)) return;
    R5900Context &c = a1ck::ckCtx(ctx);
    ref::SetScriptAnimationObjChildEntryGroup(rdram, &c, runtime);
    a1ck::ckMid();
    const uint64_t r = a1_sx64(ty::SetScriptAnimationObjChildEntryGroup(rdram, runtime, p4, p5));
    ckEnd(61, ctx, c, 1, r, 0.0f, 0x10u, 0x0u);
}
void SetScariptAnimationObjMotSyncSlotId(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t p4, uint32_t p5)
{
    if (!ckBegin(62, rdram, runtime)) return;
    R5900Context &c = a1ck::ckCtx(ctx);
    ref::SetScariptAnimationObjMotSyncSlotId(rdram, &c, runtime);
    a1ck::ckMid();
    const uint64_t r = a1_sx64(ty::SetScariptAnimationObjMotSyncSlotId(rdram, runtime, p4, p5));
    ckEnd(62, ctx, c, 1, r, 0.0f, 0x10u, 0x0u);
}
void SetScriptCharacterObjReleaseCallBackFunc(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t p4, uint32_t p5, uint32_t p6)
{
    if (!ckBegin(63, rdram, runtime)) return;
    R5900Context &c = a1ck::ckCtx(ctx);
    ref::SetScriptCharacterObjReleaseCallBackFunc(rdram, &c, runtime);
    a1ck::ckMid();
    const uint64_t r = a1_sx64(ty::SetScriptCharacterObjReleaseCallBackFunc(rdram, runtime, p4, p5, p6));
    ckEnd(63, ctx, c, 1, r, 0.0f, 0x10u, 0x0u);
}
void GetScrLwsOriClass(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint64_t p4)
{
    if (!ckBegin(64, rdram, runtime)) return;
    R5900Context &c = a1ck::ckCtx(ctx);
    ref::GetScrLwsOriClass(rdram, &c, runtime);
    a1ck::ckMid();
    const uint64_t r = (ty::GetScrLwsOriClass(rdram, runtime, p4));
    ckEnd(64, ctx, c, 2, r, 0.0f, 0x18u, 0x0u);
}
void GetScriptLwsOrientDefinfo(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint64_t p4)
{
    if (!ckBegin(65, rdram, runtime)) return;
    R5900Context &c = a1ck::ckCtx(ctx);
    ref::GetScriptLwsOrientDefinfo(rdram, &c, runtime);
    a1ck::ckMid();
    const uint64_t r = (ty::GetScriptLwsOrientDefinfo(rdram, runtime, p4));
    ckEnd(65, ctx, c, 2, r, 0.0f, 0x18u, 0x0u);
}
void manager_sub_0110DC18(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (!ckBegin(66, rdram, runtime)) return;
    R5900Context &c = a1ck::ckCtx(ctx);
    ref::manager_sub_0110DC18(rdram, &c, runtime);
    a1ck::ckMid();
    ty::manager_sub_0110DC18(rdram, runtime);
    ckEnd(66, ctx, c, 0, 0u, 0.0f, 0x0u, 0x0u);
}
void SpriteMenuIsFadeCheckSelectLayer(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t p4)
{
    if (!ckBegin(67, rdram, runtime)) return;
    R5900Context &c = a1ck::ckCtx(ctx);
    ref::SpriteMenuIsFadeCheckSelectLayer(rdram, &c, runtime);
    a1ck::ckMid();
    const a1::RetVF r = ty::SpriteMenuIsFadeCheckSelectLayer(rdram, runtime, p4);
    ckEnd(67, ctx, c, 4, r.v0, r.f0, 0x78u, 0x2u);
}
void GetScriptSpriteMenuClass(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint64_t p4)
{
    if (!ckBegin(68, rdram, runtime)) return;
    R5900Context &c = a1ck::ckCtx(ctx);
    ref::GetScriptSpriteMenuClass(rdram, &c, runtime);
    a1ck::ckMid();
    const uint64_t r = (ty::GetScriptSpriteMenuClass(rdram, runtime, p4));
    ckEnd(68, ctx, c, 2, r, 0.0f, 0x18u, 0x0u);
}
void SpriteMenuSetLayerPriority(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t p4, uint32_t p5, uint32_t p6)
{
    if (!ckBegin(69, rdram, runtime)) return;
    R5900Context &c = a1ck::ckCtx(ctx);
    ref::SpriteMenuSetLayerPriority(rdram, &c, runtime);
    a1ck::ckMid();
    const uint64_t r = a1_sx64(ty::SpriteMenuSetLayerPriority(rdram, runtime, p4, p5, p6));
    ckEnd(69, ctx, c, 1, r, 0.0f, 0xf8u, 0x0u);
}
void SpriteMenuGetSelectLayer(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t p4)
{
    if (!ckBegin(70, rdram, runtime)) return;
    R5900Context &c = a1ck::ckCtx(ctx);
    ref::SpriteMenuGetSelectLayer(rdram, &c, runtime);
    a1ck::ckMid();
    const uint64_t r = a1_sx64(ty::SpriteMenuGetSelectLayer(rdram, runtime, p4));
    ckEnd(70, ctx, c, 1, r, 0.0f, 0x10u, 0x0u);
}
void SpriteMenuGetPadEvent(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t p4)
{
    if (!ckBegin(71, rdram, runtime)) return;
    R5900Context &c = a1ck::ckCtx(ctx);
    ref::SpriteMenuGetPadEvent(rdram, &c, runtime);
    a1ck::ckMid();
    const uint64_t r = a1_sx64(ty::SpriteMenuGetPadEvent(rdram, runtime, p4));
    ckEnd(71, ctx, c, 1, r, 0.0f, 0x10u, 0x0u);
}
void SpriteMenuGetSelectSubMenuId(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t p4)
{
    if (!ckBegin(72, rdram, runtime)) return;
    R5900Context &c = a1ck::ckCtx(ctx);
    ref::SpriteMenuGetSelectSubMenuId(rdram, &c, runtime);
    a1ck::ckMid();
    const uint64_t r = a1_sx64(ty::SpriteMenuGetSelectSubMenuId(rdram, runtime, p4));
    ckEnd(72, ctx, c, 1, r, 0.0f, 0x38u, 0x0u);
}
void SpriteMenuIsFadeCheckLayer(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t p4, uint32_t p5)
{
    if (!ckBegin(73, rdram, runtime)) return;
    R5900Context &c = a1ck::ckCtx(ctx);
    ref::SpriteMenuIsFadeCheckLayer(rdram, &c, runtime);
    a1ck::ckMid();
    const a1::RetVF r = ty::SpriteMenuIsFadeCheckLayer(rdram, runtime, p4, p5);
    ckEnd(73, ctx, c, 4, r.v0, r.f0, 0x78u, 0x2u);
}
void SpriteMenuGetFadeByLayer(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t p4, uint32_t p5)
{
    if (!ckBegin(74, rdram, runtime)) return;
    R5900Context &c = a1ck::ckCtx(ctx);
    ref::SpriteMenuGetFadeByLayer(rdram, &c, runtime);
    a1ck::ckMid();
    const a1::RetVF r = ty::SpriteMenuGetFadeByLayer(rdram, runtime, p4, p5);
    ckEnd(74, ctx, c, 4, r.v0, r.f0, 0x78u, 0x0u);
}
void GetScriptSpriteClass(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint64_t p4)
{
    if (!ckBegin(75, rdram, runtime)) return;
    R5900Context &c = a1ck::ckCtx(ctx);
    ref::GetScriptSpriteClass(rdram, &c, runtime);
    a1ck::ckMid();
    const uint64_t r = (ty::GetScriptSpriteClass(rdram, runtime, p4));
    ckEnd(75, ctx, c, 2, r, 0.0f, 0x18u, 0x0u);
}
void GetSubLinkClass(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint64_t p4)
{
    if (!ckBegin(76, rdram, runtime)) return;
    R5900Context &c = a1ck::ckCtx(ctx);
    ref::GetSubLinkClass(rdram, &c, runtime);
    a1ck::ckMid();
    const uint64_t r = (ty::GetSubLinkClass(rdram, runtime, p4));
    ckEnd(76, ctx, c, 2, r, 0.0f, 0x18u, 0x0u);
}
void CheckCollisionFinderAlive(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t p4)
{
    if (!ckBegin(77, rdram, runtime)) return;
    R5900Context &c = a1ck::ckCtx(ctx);
    ref::CheckCollisionFinderAlive(rdram, &c, runtime);
    a1ck::ckMid();
    const uint64_t r = a1_sx64(ty::CheckCollisionFinderAlive(rdram, runtime, p4));
    ckEnd(77, ctx, c, 1, r, 0.0f, 0x8u, 0x0u);
}
void CopyGeometryPointer(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t p4, uint32_t p5)
{
    if (!ckBegin(78, rdram, runtime)) return;
    R5900Context &c = a1ck::ckCtx(ctx);
    ref::CopyGeometryPointer(rdram, &c, runtime);
    a1ck::ckMid();
    const uint64_t r = (ty::CopyGeometryPointer(rdram, runtime, p4, p5));
    ckEnd(78, ctx, c, 2, r, 0.0f, 0x8u, 0x0u);
}
void free_all_emitter_and_particle_particle_manager(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (!ckBegin(79, rdram, runtime)) return;
    R5900Context &c = a1ck::ckCtx(ctx);
    ref::free_all_emitter_and_particle_particle_manager(rdram, &c, runtime);
    a1ck::ckMid();
    ty::free_all_emitter_and_particle_particle_manager(rdram, runtime);
    ckEnd(79, ctx, c, 0, 0u, 0.0f, 0x0u, 0x0u);
}
void get_particle_effect_id_from_sheet(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint64_t p4)
{
    if (!ckBegin(80, rdram, runtime)) return;
    R5900Context &c = a1ck::ckCtx(ctx);
    ref::get_particle_effect_id_from_sheet(rdram, &c, runtime);
    a1ck::ckMid();
    const uint64_t r = (ty::get_particle_effect_id_from_sheet(rdram, runtime, p4));
    ckEnd(80, ctx, c, 2, r, 0.0f, 0x0u, 0x0u);
}
void get_particle_emitter_from_emitter_id(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint64_t p4)
{
    if (!ckBegin(81, rdram, runtime)) return;
    R5900Context &c = a1ck::ckCtx(ctx);
    ref::get_particle_emitter_from_emitter_id(rdram, &c, runtime);
    a1ck::ckMid();
    const uint64_t r = (ty::get_particle_emitter_from_emitter_id(rdram, runtime, p4));
    ckEnd(81, ctx, c, 2, r, 0.0f, 0x8u, 0x0u);
}
void get_main_particle_effect_lod_from_sheet(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t p4)
{
    if (!ckBegin(82, rdram, runtime)) return;
    R5900Context &c = a1ck::ckCtx(ctx);
    ref::get_main_particle_effect_lod_from_sheet(rdram, &c, runtime);
    a1ck::ckMid();
    const uint64_t r = a1_sx64(ty::get_main_particle_effect_lod_from_sheet(rdram, runtime, p4));
    ckEnd(82, ctx, c, 1, r, 0.0f, 0x0u, 0x0u);
}
void get_sub_particle_effect_lod_from_sheet(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t p4)
{
    if (!ckBegin(83, rdram, runtime)) return;
    R5900Context &c = a1ck::ckCtx(ctx);
    ref::get_sub_particle_effect_lod_from_sheet(rdram, &c, runtime);
    a1ck::ckMid();
    const uint64_t r = a1_sx64(ty::get_sub_particle_effect_lod_from_sheet(rdram, runtime, p4));
    ckEnd(83, ctx, c, 1, r, 0.0f, 0x0u, 0x0u);
}
void get_top_particle_effect_from_lod(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t p4)
{
    if (!ckBegin(84, rdram, runtime)) return;
    R5900Context &c = a1ck::ckCtx(ctx);
    ref::get_top_particle_effect_from_lod(rdram, &c, runtime);
    a1ck::ckMid();
    const uint64_t r = a1_sx64(ty::get_top_particle_effect_from_lod(rdram, runtime, p4));
    ckEnd(84, ctx, c, 1, r, 0.0f, 0x0u, 0x0u);
}
void get_radiate_range_coefficient(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, float pf12)
{
    if (!ckBegin(85, rdram, runtime)) return;
    R5900Context &c = a1ck::ckCtx(ctx);
    ref::get_radiate_range_coefficient(rdram, &c, runtime);
    a1ck::ckMid();
    const float r = ty::get_radiate_range_coefficient(rdram, runtime, pf12);
    ckEnd(85, ctx, c, 3, 0u, r, 0x2u, 0x2u);
}
void unload_npe_file_main(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (!ckBegin(86, rdram, runtime)) return;
    R5900Context &c = a1ck::ckCtx(ctx);
    ref::unload_npe_file_main(rdram, &c, runtime);
    a1ck::ckMid();
    ty::unload_npe_file_main(rdram, runtime);
    ckEnd(86, ctx, c, 0, 0u, 0.0f, 0x0u, 0x0u);
}
void unload_npe_file_sub(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (!ckBegin(87, rdram, runtime)) return;
    R5900Context &c = a1ck::ckCtx(ctx);
    ref::unload_npe_file_sub(rdram, &c, runtime);
    a1ck::ckMid();
    ty::unload_npe_file_sub(rdram, runtime);
    ckEnd(87, ctx, c, 0, 0u, 0.0f, 0x0u, 0x0u);
}
void manager_sub_01140740(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint64_t p4)
{
    if (!ckBegin(88, rdram, runtime)) return;
    R5900Context &c = a1ck::ckCtx(ctx);
    ref::manager_sub_01140740(rdram, &c, runtime);
    a1ck::ckMid();
    const uint64_t r = (ty::manager_sub_01140740(rdram, runtime, p4));
    ckEnd(88, ctx, c, 2, r, 0.0f, 0x0u, 0x0u);
}
void manager_sub_01141080(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t p4)
{
    if (!ckBegin(89, rdram, runtime)) return;
    R5900Context &c = a1ck::ckCtx(ctx);
    ref::manager_sub_01141080(rdram, &c, runtime);
    a1ck::ckMid();
    const uint64_t r = a1_sx64(ty::manager_sub_01141080(rdram, runtime, p4));
    ckEnd(89, ctx, c, 1, r, 0.0f, 0x0u, 0x0u);
}
void manager_sub_01142030(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t p4)
{
    if (!ckBegin(90, rdram, runtime)) return;
    R5900Context &c = a1ck::ckCtx(ctx);
    ref::manager_sub_01142030(rdram, &c, runtime);
    a1ck::ckMid();
    const float r = ty::manager_sub_01142030(rdram, runtime, p4);
    ckEnd(90, ctx, c, 3, 0u, r, 0x2u, 0x6u);
}
void manager_sub_01144E00(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t p4)
{
    if (!ckBegin(91, rdram, runtime)) return;
    R5900Context &c = a1ck::ckCtx(ctx);
    ref::manager_sub_01144E00(rdram, &c, runtime);
    a1ck::ckMid();
    const float r = ty::manager_sub_01144E00(rdram, runtime, p4);
    ckEnd(91, ctx, c, 3, 0u, r, 0x2u, 0x2u);
}
void manager_sub_01144E38(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t p4)
{
    if (!ckBegin(92, rdram, runtime)) return;
    R5900Context &c = a1ck::ckCtx(ctx);
    ref::manager_sub_01144E38(rdram, &c, runtime);
    a1ck::ckMid();
    const float r = ty::manager_sub_01144E38(rdram, runtime, p4);
    ckEnd(92, ctx, c, 3, 0u, r, 0x2u, 0x0u);
}
void manager_sub_01144E50(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t p4)
{
    if (!ckBegin(93, rdram, runtime)) return;
    R5900Context &c = a1ck::ckCtx(ctx);
    ref::manager_sub_01144E50(rdram, &c, runtime);
    a1ck::ckMid();
    ty::manager_sub_01144E50(rdram, runtime, p4);
    ckEnd(93, ctx, c, 0, 0u, 0.0f, 0x0u, 0x0u);
}
void manager_sub_01144E58(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t p4)
{
    if (!ckBegin(94, rdram, runtime)) return;
    R5900Context &c = a1ck::ckCtx(ctx);
    ref::manager_sub_01144E58(rdram, &c, runtime);
    a1ck::ckMid();
    const float r = ty::manager_sub_01144E58(rdram, runtime, p4);
    ckEnd(94, ctx, c, 3, 0u, r, 0x2u, 0x0u);
}
void manager_sub_01144E70(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t p4)
{
    if (!ckBegin(95, rdram, runtime)) return;
    R5900Context &c = a1ck::ckCtx(ctx);
    ref::manager_sub_01144E70(rdram, &c, runtime);
    a1ck::ckMid();
    const uint64_t r = a1_sx64(ty::manager_sub_01144E70(rdram, runtime, p4));
    ckEnd(95, ctx, c, 1, r, 0.0f, 0x0u, 0x0u);
}
void manager_sub_01144E88(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t p4)
{
    if (!ckBegin(96, rdram, runtime)) return;
    R5900Context &c = a1ck::ckCtx(ctx);
    ref::manager_sub_01144E88(rdram, &c, runtime);
    a1ck::ckMid();
    ty::manager_sub_01144E88(rdram, runtime, p4);
    ckEnd(96, ctx, c, 0, 0u, 0.0f, 0x0u, 0x0u);
}
void is_eternal_wind(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t p4)
{
    if (!ckBegin(97, rdram, runtime)) return;
    R5900Context &c = a1ck::ckCtx(ctx);
    ref::is_eternal_wind(rdram, &c, runtime);
    a1ck::ckMid();
    const a1::RetVF r = ty::is_eternal_wind(rdram, runtime, p4);
    ckEnd(97, ctx, c, 4, r.v0, r.f0, 0x0u, 0x2u);
}
void stop_wind_emitter_geometry_pointer_reference(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint64_t p4)
{
    if (!ckBegin(98, rdram, runtime)) return;
    R5900Context &c = a1ck::ckCtx(ctx);
    ref::stop_wind_emitter_geometry_pointer_reference(rdram, &c, runtime);
    a1ck::ckMid();
    const uint64_t r = a1_sx64(ty::stop_wind_emitter_geometry_pointer_reference(rdram, runtime, p4));
    ckEnd(98, ctx, c, 1, r, 0.0f, 0x18u, 0x0u);
}
void stop_wind_emitter_callback(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint64_t p4)
{
    if (!ckBegin(99, rdram, runtime)) return;
    R5900Context &c = a1ck::ckCtx(ctx);
    ref::stop_wind_emitter_callback(rdram, &c, runtime);
    a1ck::ckMid();
    const uint64_t r = a1_sx64(ty::stop_wind_emitter_callback(rdram, runtime, p4));
    ckEnd(99, ctx, c, 1, r, 0.0f, 0x18u, 0x0u);
}
void end_wind_emitter(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint64_t p4)
{
    if (!ckBegin(100, rdram, runtime)) return;
    R5900Context &c = a1ck::ckCtx(ctx);
    ref::end_wind_emitter(rdram, &c, runtime);
    a1ck::ckMid();
    const uint64_t r = a1_sx64(ty::end_wind_emitter(rdram, runtime, p4));
    ckEnd(100, ctx, c, 1, r, 0.0f, 0x18u, 0x0u);
}
void add_wind_segment(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (!ckBegin(101, rdram, runtime)) return;
    R5900Context &c = a1ck::ckCtx(ctx);
    ref::add_wind_segment(rdram, &c, runtime);
    a1ck::ckMid();
    ty::add_wind_segment(rdram, runtime);
    ckEnd(101, ctx, c, 0, 0u, 0.0f, 0x0u, 0x0u);
}
void RelocateModifiedManager(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint64_t p4, uint64_t p5, uint32_t p6)
{
    if (!ckBegin(102, rdram, runtime)) return;
    R5900Context &c = a1ck::ckCtx(ctx);
    ref::RelocateModifiedManager(rdram, &c, runtime);
    a1ck::ckMid();
    const uint64_t r = (ty::RelocateModifiedManager(rdram, runtime, p4, p5, p6));
    ckEnd(102, ctx, c, 2, r, 0.0f, 0x3d8u, 0x0u);
}
void GetSizeOfModifiedManagerWork(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint64_t p4)
{
    if (!ckBegin(103, rdram, runtime)) return;
    R5900Context &c = a1ck::ckCtx(ctx);
    ref::GetSizeOfModifiedManagerWork(rdram, &c, runtime);
    a1ck::ckMid();
    const uint64_t r = (ty::GetSizeOfModifiedManagerWork(rdram, runtime, p4));
    ckEnd(103, ctx, c, 2, r, 0.0f, 0x78u, 0x0u);
}
void sheetMgrChkStageIndexResolve(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint64_t p4)
{
    if (!ckBegin(104, rdram, runtime)) return;
    R5900Context &c = a1ck::ckCtx(ctx);
    ref::sheetMgrChkStageIndexResolve(rdram, &c, runtime);
    a1ck::ckMid();
    const uint64_t r = (ty::sheetMgrChkStageIndexResolve(rdram, runtime, p4));
    ckEnd(104, ctx, c, 2, r, 0.0f, 0xf8u, 0x0u);
}
void add_camera_lws_animation_segment(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (!ckBegin(105, rdram, runtime)) return;
    R5900Context &c = a1ck::ckCtx(ctx);
    ref::add_camera_lws_animation_segment(rdram, &c, runtime);
    a1ck::ckMid();
    ty::add_camera_lws_animation_segment(rdram, runtime);
    ckEnd(105, ctx, c, 0, 0u, 0.0f, 0x0u, 0x0u);
}
void get_camera_position(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t p4)
{
    if (!ckBegin(106, rdram, runtime)) return;
    R5900Context &c = a1ck::ckCtx(ctx);
    ref::get_camera_position(rdram, &c, runtime);
    a1ck::ckMid();
    const uint64_t r = a1_sx64(ty::get_camera_position(rdram, runtime, p4));
    ckEnd(106, ctx, c, 1, r, 0.0f, 0x0u, 0x0u);
}
void get_camera_direction(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t p4)
{
    if (!ckBegin(107, rdram, runtime)) return;
    R5900Context &c = a1ck::ckCtx(ctx);
    ref::get_camera_direction(rdram, &c, runtime);
    a1ck::ckMid();
    const uint64_t r = a1_sx64(ty::get_camera_direction(rdram, runtime, p4));
    ckEnd(107, ctx, c, 1, r, 0.0f, 0x0u, 0x0u);
}
void get_camera_up_direction(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t p4)
{
    if (!ckBegin(108, rdram, runtime)) return;
    R5900Context &c = a1ck::ckCtx(ctx);
    ref::get_camera_up_direction(rdram, &c, runtime);
    a1ck::ckMid();
    const uint64_t r = a1_sx64(ty::get_camera_up_direction(rdram, runtime, p4));
    ckEnd(108, ctx, c, 1, r, 0.0f, 0x0u, 0x0u);
}
void get_camera_obj_layout_class(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t p4)
{
    if (!ckBegin(109, rdram, runtime)) return;
    R5900Context &c = a1ck::ckCtx(ctx);
    ref::get_camera_obj_layout_class(rdram, &c, runtime);
    a1ck::ckMid();
    const uint64_t r = a1_sx64(ty::get_camera_obj_layout_class(rdram, runtime, p4));
    ckEnd(109, ctx, c, 1, r, 0.0f, 0x0u, 0x0u);
}
void get_view_matrix(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t p4)
{
    if (!ckBegin(110, rdram, runtime)) return;
    R5900Context &c = a1ck::ckCtx(ctx);
    ref::get_view_matrix(rdram, &c, runtime);
    a1ck::ckMid();
    const uint64_t r = a1_sx64(ty::get_view_matrix(rdram, runtime, p4));
    ckEnd(110, ctx, c, 1, r, 0.0f, 0x0u, 0x0u);
}
void get_manage_object_id_camera_region(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint64_t p4)
{
    if (!ckBegin(111, rdram, runtime)) return;
    R5900Context &c = a1ck::ckCtx(ctx);
    ref::get_manage_object_id_camera_region(rdram, &c, runtime);
    a1ck::ckMid();
    const uint64_t r = a1_sx64(ty::get_manage_object_id_camera_region(rdram, runtime, p4));
    ckEnd(111, ctx, c, 1, r, 0.0f, 0x8u, 0x0u);
}
void is_parent_obj_boss_camera_region(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint64_t p4)
{
    if (!ckBegin(112, rdram, runtime)) return;
    R5900Context &c = a1ck::ckCtx(ctx);
    ref::is_parent_obj_boss_camera_region(rdram, &c, runtime);
    a1ck::ckMid();
    const uint64_t r = a1_sx64(ty::is_parent_obj_boss_camera_region(rdram, runtime, p4));
    ckEnd(112, ctx, c, 1, r, 0.0f, 0x18u, 0x0u);
}
void get_priority_camera_region(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint64_t p4)
{
    if (!ckBegin(113, rdram, runtime)) return;
    R5900Context &c = a1ck::ckCtx(ctx);
    ref::get_priority_camera_region(rdram, &c, runtime);
    a1ck::ckMid();
    const uint64_t r = a1_sx64(ty::get_priority_camera_region(rdram, runtime, p4));
    ckEnd(113, ctx, c, 1, r, 0.0f, 0x8u, 0x0u);
}
void get_region_type_camera_region(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint64_t p4)
{
    if (!ckBegin(114, rdram, runtime)) return;
    R5900Context &c = a1ck::ckCtx(ctx);
    ref::get_region_type_camera_region(rdram, &c, runtime);
    a1ck::ckMid();
    const uint64_t r = a1_sx64(ty::get_region_type_camera_region(rdram, runtime, p4));
    ckEnd(114, ctx, c, 1, r, 0.0f, 0x8u, 0x0u);
}
void get_clip_matrix(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t p4)
{
    if (!ckBegin(115, rdram, runtime)) return;
    R5900Context &c = a1ck::ckCtx(ctx);
    ref::get_clip_matrix(rdram, &c, runtime);
    a1ck::ckMid();
    const uint64_t r = a1_sx64(ty::get_clip_matrix(rdram, runtime, p4));
    ckEnd(115, ctx, c, 1, r, 0.0f, 0x0u, 0x0u);
}
void get_clip_matrix128(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t p4)
{
    if (!ckBegin(116, rdram, runtime)) return;
    R5900Context &c = a1ck::ckCtx(ctx);
    ref::get_clip_matrix128(rdram, &c, runtime);
    a1ck::ckMid();
    const uint64_t r = a1_sx64(ty::get_clip_matrix128(rdram, runtime, p4));
    ckEnd(116, ctx, c, 1, r, 0.0f, 0x0u, 0x0u);
}
void get_clip_matrix256(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t p4)
{
    if (!ckBegin(117, rdram, runtime)) return;
    R5900Context &c = a1ck::ckCtx(ctx);
    ref::get_clip_matrix256(rdram, &c, runtime);
    a1ck::ckMid();
    const uint64_t r = a1_sx64(ty::get_clip_matrix256(rdram, runtime, p4));
    ckEnd(117, ctx, c, 1, r, 0.0f, 0x0u, 0x0u);
}
void get_clip_matrix_fovignore(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t p4)
{
    if (!ckBegin(118, rdram, runtime)) return;
    R5900Context &c = a1ck::ckCtx(ctx);
    ref::get_clip_matrix_fovignore(rdram, &c, runtime);
    a1ck::ckMid();
    const uint64_t r = a1_sx64(ty::get_clip_matrix_fovignore(rdram, runtime, p4));
    ckEnd(118, ctx, c, 1, r, 0.0f, 0x0u, 0x0u);
}
void get_screen_matrix(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t p4)
{
    if (!ckBegin(119, rdram, runtime)) return;
    R5900Context &c = a1ck::ckCtx(ctx);
    ref::get_screen_matrix(rdram, &c, runtime);
    a1ck::ckMid();
    const uint64_t r = a1_sx64(ty::get_screen_matrix(rdram, runtime, p4));
    ckEnd(119, ctx, c, 1, r, 0.0f, 0x0u, 0x0u);
}
void get_screen_matrix128(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t p4)
{
    if (!ckBegin(120, rdram, runtime)) return;
    R5900Context &c = a1ck::ckCtx(ctx);
    ref::get_screen_matrix128(rdram, &c, runtime);
    a1ck::ckMid();
    const uint64_t r = a1_sx64(ty::get_screen_matrix128(rdram, runtime, p4));
    ckEnd(120, ctx, c, 1, r, 0.0f, 0x0u, 0x0u);
}
void get_screen_matrix256(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t p4)
{
    if (!ckBegin(121, rdram, runtime)) return;
    R5900Context &c = a1ck::ckCtx(ctx);
    ref::get_screen_matrix256(rdram, &c, runtime);
    a1ck::ckMid();
    const uint64_t r = a1_sx64(ty::get_screen_matrix256(rdram, runtime, p4));
    ckEnd(121, ctx, c, 1, r, 0.0f, 0x0u, 0x0u);
}
void get_VC_matrix(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t p4)
{
    if (!ckBegin(122, rdram, runtime)) return;
    R5900Context &c = a1ck::ckCtx(ctx);
    ref::get_VC_matrix(rdram, &c, runtime);
    a1ck::ckMid();
    const uint64_t r = a1_sx64(ty::get_VC_matrix(rdram, runtime, p4));
    ckEnd(122, ctx, c, 1, r, 0.0f, 0x0u, 0x0u);
}
void get_CS_matrix(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t p4)
{
    if (!ckBegin(123, rdram, runtime)) return;
    R5900Context &c = a1ck::ckCtx(ctx);
    ref::get_CS_matrix(rdram, &c, runtime);
    a1ck::ckMid();
    const uint64_t r = a1_sx64(ty::get_CS_matrix(rdram, runtime, p4));
    ckEnd(123, ctx, c, 1, r, 0.0f, 0x0u, 0x0u);
}
void get_WVC_matrix(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t p4)
{
    if (!ckBegin(124, rdram, runtime)) return;
    R5900Context &c = a1ck::ckCtx(ctx);
    ref::get_WVC_matrix(rdram, &c, runtime);
    a1ck::ckMid();
    const uint64_t r = a1_sx64(ty::get_WVC_matrix(rdram, runtime, p4));
    ckEnd(124, ctx, c, 1, r, 0.0f, 0x0u, 0x0u);
}
void get_VCS_matrix(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t p4)
{
    if (!ckBegin(125, rdram, runtime)) return;
    R5900Context &c = a1ck::ckCtx(ctx);
    ref::get_VCS_matrix(rdram, &c, runtime);
    a1ck::ckMid();
    const uint64_t r = a1_sx64(ty::get_VCS_matrix(rdram, runtime, p4));
    ckEnd(125, ctx, c, 1, r, 0.0f, 0x0u, 0x0u);
}
void get_WVCS_matrix(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t p4)
{
    if (!ckBegin(126, rdram, runtime)) return;
    R5900Context &c = a1ck::ckCtx(ctx);
    ref::get_WVCS_matrix(rdram, &c, runtime);
    a1ck::ckMid();
    const uint64_t r = a1_sx64(ty::get_WVCS_matrix(rdram, runtime, p4));
    ckEnd(126, ctx, c, 1, r, 0.0f, 0x0u, 0x0u);
}
void get_scissor_VC_matrix(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t p4)
{
    if (!ckBegin(127, rdram, runtime)) return;
    R5900Context &c = a1ck::ckCtx(ctx);
    ref::get_scissor_VC_matrix(rdram, &c, runtime);
    a1ck::ckMid();
    const uint64_t r = a1_sx64(ty::get_scissor_VC_matrix(rdram, runtime, p4));
    ckEnd(127, ctx, c, 1, r, 0.0f, 0x0u, 0x0u);
}
void get_scissor_WVC_matrix(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t p4)
{
    if (!ckBegin(128, rdram, runtime)) return;
    R5900Context &c = a1ck::ckCtx(ctx);
    ref::get_scissor_WVC_matrix(rdram, &c, runtime);
    a1ck::ckMid();
    const uint64_t r = a1_sx64(ty::get_scissor_WVC_matrix(rdram, runtime, p4));
    ckEnd(128, ctx, c, 1, r, 0.0f, 0x0u, 0x0u);
}
void getPointDataHeader(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint64_t p4)
{
    if (!ckBegin(129, rdram, runtime)) return;
    R5900Context &c = a1ck::ckCtx(ctx);
    ref::getPointDataHeader(rdram, &c, runtime);
    a1ck::ckMid();
    const uint64_t r = (ty::getPointDataHeader(rdram, runtime, p4));
    ckEnd(129, ctx, c, 2, r, 0.0f, 0x8u, 0x0u);
}
void getLineDataHeader(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint64_t p4)
{
    if (!ckBegin(130, rdram, runtime)) return;
    R5900Context &c = a1ck::ckCtx(ctx);
    ref::getLineDataHeader(rdram, &c, runtime);
    a1ck::ckMid();
    const uint64_t r = (ty::getLineDataHeader(rdram, runtime, p4));
    ckEnd(130, ctx, c, 2, r, 0.0f, 0x8u, 0x0u);
}
void getConnectLineArray(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint64_t p4, uint64_t p5)
{
    if (!ckBegin(131, rdram, runtime)) return;
    R5900Context &c = a1ck::ckCtx(ctx);
    ref::getConnectLineArray(rdram, &c, runtime);
    a1ck::ckMid();
    const uint64_t r = a1_sx64(ty::getConnectLineArray(rdram, runtime, p4, p5));
    ckEnd(131, ctx, c, 1, r, 0.0f, 0x48u, 0x0u);
}
void getRootDataHeader(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint64_t p4)
{
    if (!ckBegin(132, rdram, runtime)) return;
    R5900Context &c = a1ck::ckCtx(ctx);
    ref::getRootDataHeader(rdram, &c, runtime);
    a1ck::ckMid();
    const uint64_t r = (ty::getRootDataHeader(rdram, runtime, p4));
    ckEnd(132, ctx, c, 2, r, 0.0f, 0x8u, 0x0u);
}
void getGoalDataHeader(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint64_t p4)
{
    if (!ckBegin(133, rdram, runtime)) return;
    R5900Context &c = a1ck::ckCtx(ctx);
    ref::getGoalDataHeader(rdram, &c, runtime);
    a1ck::ckMid();
    const uint64_t r = (ty::getGoalDataHeader(rdram, runtime, p4));
    ckEnd(133, ctx, c, 2, r, 0.0f, 0x8u, 0x0u);
}
void getSubDataHeader(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint64_t p4)
{
    if (!ckBegin(134, rdram, runtime)) return;
    R5900Context &c = a1ck::ckCtx(ctx);
    ref::getSubDataHeader(rdram, &c, runtime);
    a1ck::ckMid();
    const uint64_t r = (ty::getSubDataHeader(rdram, runtime, p4));
    ckEnd(134, ctx, c, 2, r, 0.0f, 0x8u, 0x0u);
}
void getChildGoalPointIdxArray(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint64_t p4, uint64_t p5)
{
    if (!ckBegin(135, rdram, runtime)) return;
    R5900Context &c = a1ck::ckCtx(ctx);
    ref::getChildGoalPointIdxArray(rdram, &c, runtime);
    a1ck::ckMid();
    const uint64_t r = a1_sx64(ty::getChildGoalPointIdxArray(rdram, runtime, p4, p5));
    ckEnd(135, ctx, c, 1, r, 0.0f, 0x48u, 0x0u);
}
void getChildSubPointIdxArray(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint64_t p4, uint64_t p5)
{
    if (!ckBegin(136, rdram, runtime)) return;
    R5900Context &c = a1ck::ckCtx(ctx);
    ref::getChildSubPointIdxArray(rdram, &c, runtime);
    a1ck::ckMid();
    const uint64_t r = a1_sx64(ty::getChildSubPointIdxArray(rdram, runtime, p4, p5));
    ckEnd(136, ctx, c, 1, r, 0.0f, 0x48u, 0x0u);
}
void getInterestDataHeader(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint64_t p4)
{
    if (!ckBegin(137, rdram, runtime)) return;
    R5900Context &c = a1ck::ckCtx(ctx);
    ref::getInterestDataHeader(rdram, &c, runtime);
    a1ck::ckMid();
    const uint64_t r = (ty::getInterestDataHeader(rdram, runtime, p4));
    ckEnd(137, ctx, c, 2, r, 0.0f, 0x8u, 0x0u);
}
void getDeadDataHeader(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint64_t p4)
{
    if (!ckBegin(138, rdram, runtime)) return;
    R5900Context &c = a1ck::ckCtx(ctx);
    ref::getDeadDataHeader(rdram, &c, runtime);
    a1ck::ckMid();
    const uint64_t r = (ty::getDeadDataHeader(rdram, runtime, p4));
    ckEnd(138, ctx, c, 2, r, 0.0f, 0x8u, 0x0u);
}
void getLayoutDataHeader(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint64_t p4)
{
    if (!ckBegin(139, rdram, runtime)) return;
    R5900Context &c = a1ck::ckCtx(ctx);
    ref::getLayoutDataHeader(rdram, &c, runtime);
    a1ck::ckMid();
    const uint64_t r = (ty::getLayoutDataHeader(rdram, runtime, p4));
    ckEnd(139, ctx, c, 2, r, 0.0f, 0x8u, 0x0u);
}
void GAMEUTIL_is_push_or_down_1p_button(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint64_t p4, uint32_t p5)
{
    if (!ckBegin(140, rdram, runtime)) return;
    R5900Context &c = a1ck::ckCtx(ctx);
    ref::GAMEUTIL_is_push_or_down_1p_button(rdram, &c, runtime);
    a1ck::ckMid();
    const uint64_t r = a1_sx64(ty::GAMEUTIL_is_push_or_down_1p_button(rdram, runtime, p4, p5));
    ckEnd(140, ctx, c, 1, r, 0.0f, 0x18u, 0x0u);
}
void GAMEUTIL_get_data_record_info(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t p4, uint64_t p5)
{
    if (!ckBegin(141, rdram, runtime)) return;
    R5900Context &c = a1ck::ckCtx(ctx);
    ref::GAMEUTIL_get_data_record_info(rdram, &c, runtime);
    a1ck::ckMid();
    const uint64_t r = a1_sx64(ty::GAMEUTIL_get_data_record_info(rdram, runtime, p4, p5));
    ckEnd(141, ctx, c, 1, r, 0.0f, 0x8u, 0x0u);
}
void gStatSaveDataGameFlagChk(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint64_t p4)
{
    if (!ckBegin(142, rdram, runtime)) return;
    R5900Context &c = a1ck::ckCtx(ctx);
    ref::gStatSaveDataGameFlagChk(rdram, &c, runtime);
    a1ck::ckMid();
    const uint64_t r = (ty::gStatSaveDataGameFlagChk(rdram, runtime, p4));
    ckEnd(142, ctx, c, 2, r, 0.0f, 0x38u, 0x0u);
}
void gStatGetGObjAdrs(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t p4)
{
    if (!ckBegin(143, rdram, runtime)) return;
    R5900Context &c = a1ck::ckCtx(ctx);
    ref::gStatGetGObjAdrs(rdram, &c, runtime);
    a1ck::ckMid();
    const uint64_t r = a1_sx64(ty::gStatGetGObjAdrs(rdram, runtime, p4));
    ckEnd(143, ctx, c, 1, r, 0.0f, 0x0u, 0x0u);
}
void getstagenumber(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint64_t p4)
{
    if (!ckBegin(144, rdram, runtime)) return;
    R5900Context &c = a1ck::ckCtx(ctx);
    ref::getstagenumber(rdram, &c, runtime);
    a1ck::ckMid();
    const uint64_t r = (ty::getstagenumber(rdram, runtime, p4));
    ckEnd(144, ctx, c, 2, r, 0.0f, 0x1f8u, 0x0u);
}
void initStageLayout(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (!ckBegin(145, rdram, runtime)) return;
    R5900Context &c = a1ck::ckCtx(ctx);
    ref::initStageLayout(rdram, &c, runtime);
    a1ck::ckMid();
    ty::initStageLayout(rdram, runtime);
    ckEnd(145, ctx, c, 0, 0u, 0.0f, 0x0u, 0x0u);
}
void deleteStageLayout(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (!ckBegin(146, rdram, runtime)) return;
    R5900Context &c = a1ck::ckCtx(ctx);
    ref::deleteStageLayout(rdram, &c, runtime);
    a1ck::ckMid();
    ty::deleteStageLayout(rdram, runtime);
    ckEnd(146, ctx, c, 0, 0u, 0.0f, 0x0u, 0x0u);
}
void initAdpcmOrient(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (!ckBegin(147, rdram, runtime)) return;
    R5900Context &c = a1ck::ckCtx(ctx);
    ref::initAdpcmOrient(rdram, &c, runtime);
    a1ck::ckMid();
    ty::initAdpcmOrient(rdram, runtime);
    ckEnd(147, ctx, c, 0, 0u, 0.0f, 0x0u, 0x0u);
}
void bgmStartGameOver(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (!ckBegin(148, rdram, runtime)) return;
    R5900Context &c = a1ck::ckCtx(ctx);
    ref::bgmStartGameOver(rdram, &c, runtime);
    a1ck::ckMid();
    ty::bgmStartGameOver(rdram, runtime);
    ckEnd(148, ctx, c, 0, 0u, 0.0f, 0x0u, 0x0u);
}
void bgmStageInit(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (!ckBegin(149, rdram, runtime)) return;
    R5900Context &c = a1ck::ckCtx(ctx);
    ref::bgmStageInit(rdram, &c, runtime);
    a1ck::ckMid();
    ty::bgmStageInit(rdram, runtime);
    ckEnd(149, ctx, c, 0, 0u, 0.0f, 0x0u, 0x0u);
}
void SCRExecBossPostProcess(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (!ckBegin(150, rdram, runtime)) return;
    R5900Context &c = a1ck::ckCtx(ctx);
    ref::SCRExecBossPostProcess(rdram, &c, runtime);
    a1ck::ckMid();
    ty::SCRExecBossPostProcess(rdram, runtime);
    ckEnd(150, ctx, c, 0, 0u, 0.0f, 0x0u, 0x0u);
}
void SCRPlayerReset(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (!ckBegin(151, rdram, runtime)) return;
    R5900Context &c = a1ck::ckCtx(ctx);
    ref::SCRPlayerReset(rdram, &c, runtime);
    a1ck::ckMid();
    ty::SCRPlayerReset(rdram, runtime);
    ckEnd(151, ctx, c, 0, 0u, 0.0f, 0x0u, 0x0u);
}
void gamecore_sub_013B6530(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (!ckBegin(152, rdram, runtime)) return;
    R5900Context &c = a1ck::ckCtx(ctx);
    ref::gamecore_sub_013B6530(rdram, &c, runtime);
    a1ck::ckMid();
    ty::gamecore_sub_013B6530(rdram, runtime);
    ckEnd(152, ctx, c, 0, 0u, 0.0f, 0x0u, 0x0u);
}
void scrInit(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (!ckBegin(153, rdram, runtime)) return;
    R5900Context &c = a1ck::ckCtx(ctx);
    ref::scrInit(rdram, &c, runtime);
    a1ck::ckMid();
    ty::scrInit(rdram, runtime);
    ckEnd(153, ctx, c, 0, 0u, 0.0f, 0x0u, 0x0u);
}
void AI_CheckNormZero(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t p4)
{
    if (!ckBegin(154, rdram, runtime)) return;
    R5900Context &c = a1ck::ckCtx(ctx);
    ref::AI_CheckNormZero(rdram, &c, runtime);
    a1ck::ckMid();
    const a1::RetVF r = ty::AI_CheckNormZero(rdram, runtime, p4);
    ckEnd(154, ctx, c, 4, r.v0, r.f0, 0x2u, 0x2u);
}
void initSavePoint(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (!ckBegin(155, rdram, runtime)) return;
    R5900Context &c = a1ck::ckCtx(ctx);
    ref::initSavePoint(rdram, &c, runtime);
    a1ck::ckMid();
    ty::initSavePoint(rdram, runtime);
    ckEnd(155, ctx, c, 0, 0u, 0.0f, 0x0u, 0x0u);
}
void getSprLogWorkSize(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (!ckBegin(156, rdram, runtime)) return;
    R5900Context &c = a1ck::ckCtx(ctx);
    ref::getSprLogWorkSize(rdram, &c, runtime);
    a1ck::ckMid();
    const uint64_t r = a1_sx64(ty::getSprLogWorkSize(rdram, runtime));
    ckEnd(156, ctx, c, 1, r, 0.0f, 0x0u, 0x0u);
}
void getSprLogStatusSize(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (!ckBegin(157, rdram, runtime)) return;
    R5900Context &c = a1ck::ckCtx(ctx);
    ref::getSprLogStatusSize(rdram, &c, runtime);
    a1ck::ckMid();
    const uint64_t r = a1_sx64(ty::getSprLogStatusSize(rdram, runtime));
    ckEnd(157, ctx, c, 1, r, 0.0f, 0x0u, 0x0u);
}
void get_camera_move_collision_group(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t p4)
{
    if (!ckBegin(158, rdram, runtime)) return;
    R5900Context &c = a1ck::ckCtx(ctx);
    ref::get_camera_move_collision_group(rdram, &c, runtime);
    a1ck::ckMid();
    const uint64_t r = a1_sx64(ty::get_camera_move_collision_group(rdram, runtime, p4));
    ckEnd(158, ctx, c, 1, r, 0.0f, 0x0u, 0x0u);
}
void get_game_camera(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t p4)
{
    if (!ckBegin(159, rdram, runtime)) return;
    R5900Context &c = a1ck::ckCtx(ctx);
    ref::get_game_camera(rdram, &c, runtime);
    a1ck::ckMid();
    const uint64_t r = a1_sx64(ty::get_game_camera(rdram, runtime, p4));
    ckEnd(159, ctx, c, 1, r, 0.0f, 0x0u, 0x0u);
}
void get_cur_camera_target(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t p4)
{
    if (!ckBegin(160, rdram, runtime)) return;
    R5900Context &c = a1ck::ckCtx(ctx);
    ref::get_cur_camera_target(rdram, &c, runtime);
    a1ck::ckMid();
    const uint64_t r = a1_sx64(ty::get_cur_camera_target(rdram, runtime, p4));
    ckEnd(160, ctx, c, 1, r, 0.0f, 0x0u, 0x0u);
}
void gamecore_sub_0141D178(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (!ckBegin(161, rdram, runtime)) return;
    R5900Context &c = a1ck::ckCtx(ctx);
    ref::gamecore_sub_0141D178(rdram, &c, runtime);
    a1ck::ckMid();
    ty::gamecore_sub_0141D178(rdram, runtime);
    ckEnd(161, ctx, c, 0, 0u, 0.0f, 0x0u, 0x0u);
}
void gamecore_sub_0141D180(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (!ckBegin(162, rdram, runtime)) return;
    R5900Context &c = a1ck::ckCtx(ctx);
    ref::gamecore_sub_0141D180(rdram, &c, runtime);
    a1ck::ckMid();
    ty::gamecore_sub_0141D180(rdram, runtime);
    ckEnd(162, ctx, c, 0, 0u, 0.0f, 0x0u, 0x0u);
}
void gamecore_sub_0141D4F0(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (!ckBegin(163, rdram, runtime)) return;
    R5900Context &c = a1ck::ckCtx(ctx);
    ref::gamecore_sub_0141D4F0(rdram, &c, runtime);
    a1ck::ckMid();
    ty::gamecore_sub_0141D4F0(rdram, runtime);
    ckEnd(163, ctx, c, 0, 0u, 0.0f, 0x0u, 0x0u);
}
void get_cur_using_lws_anime_mode_base_class(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint64_t p4)
{
    if (!ckBegin(164, rdram, runtime)) return;
    R5900Context &c = a1ck::ckCtx(ctx);
    ref::get_cur_using_lws_anime_mode_base_class(rdram, &c, runtime);
    a1ck::ckMid();
    const uint64_t r = (ty::get_cur_using_lws_anime_mode_base_class(rdram, runtime, p4));
    ckEnd(164, ctx, c, 2, r, 0.0f, 0x38u, 0x0u);
}
void end_object_interest_game_camera(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (!ckBegin(165, rdram, runtime)) return;
    R5900Context &c = a1ck::ckCtx(ctx);
    ref::end_object_interest_game_camera(rdram, &c, runtime);
    a1ck::ckMid();
    ty::end_object_interest_game_camera(rdram, runtime);
    ckEnd(165, ctx, c, 0, 0u, 0.0f, 0x0u, 0x0u);
}
void free_camera_mode_manager_segment(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (!ckBegin(166, rdram, runtime)) return;
    R5900Context &c = a1ck::ckCtx(ctx);
    ref::free_camera_mode_manager_segment(rdram, &c, runtime);
    a1ck::ckMid();
    ty::free_camera_mode_manager_segment(rdram, runtime);
    ckEnd(166, ctx, c, 0, 0u, 0.0f, 0x0u, 0x0u);
}
void gamecore_sub_01425D20(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint64_t p4)
{
    if (!ckBegin(167, rdram, runtime)) return;
    R5900Context &c = a1ck::ckCtx(ctx);
    ref::gamecore_sub_01425D20(rdram, &c, runtime);
    a1ck::ckMid();
    const uint64_t r = a1_sx64(ty::gamecore_sub_01425D20(rdram, runtime, p4));
    ckEnd(167, ctx, c, 1, r, 0.0f, 0x78u, 0x0u);
}
void gamecore_sub_01426250(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (!ckBegin(168, rdram, runtime)) return;
    R5900Context &c = a1ck::ckCtx(ctx);
    ref::gamecore_sub_01426250(rdram, &c, runtime);
    a1ck::ckMid();
    ty::gamecore_sub_01426250(rdram, runtime);
    ckEnd(168, ctx, c, 0, 0u, 0.0f, 0x0u, 0x0u);
}
void gamecore_sub_014262B0(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (!ckBegin(169, rdram, runtime)) return;
    R5900Context &c = a1ck::ckCtx(ctx);
    ref::gamecore_sub_014262B0(rdram, &c, runtime);
    a1ck::ckMid();
    ty::gamecore_sub_014262B0(rdram, runtime);
    ckEnd(169, ctx, c, 0, 0u, 0.0f, 0x0u, 0x0u);
}
void gamecore_sub_014262B8(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (!ckBegin(170, rdram, runtime)) return;
    R5900Context &c = a1ck::ckCtx(ctx);
    ref::gamecore_sub_014262B8(rdram, &c, runtime);
    a1ck::ckMid();
    ty::gamecore_sub_014262B8(rdram, runtime);
    ckEnd(170, ctx, c, 0, 0u, 0.0f, 0x0u, 0x0u);
}
void gamecore_sub_01426368(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (!ckBegin(171, rdram, runtime)) return;
    R5900Context &c = a1ck::ckCtx(ctx);
    ref::gamecore_sub_01426368(rdram, &c, runtime);
    a1ck::ckMid();
    ty::gamecore_sub_01426368(rdram, runtime);
    ckEnd(171, ctx, c, 0, 0u, 0.0f, 0x0u, 0x0u);
}
void get_interp_game_camera_param_game_camera_param_manager(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t p4)
{
    if (!ckBegin(172, rdram, runtime)) return;
    R5900Context &c = a1ck::ckCtx(ctx);
    ref::get_interp_game_camera_param_game_camera_param_manager(rdram, &c, runtime);
    a1ck::ckMid();
    const uint64_t r = a1_sx64(ty::get_interp_game_camera_param_game_camera_param_manager(rdram, runtime, p4));
    ckEnd(172, ctx, c, 1, r, 0.0f, 0x0u, 0x0u);
}
void get_mode_common_data_filter_base_dist_to_look_at(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t p4)
{
    if (!ckBegin(173, rdram, runtime)) return;
    R5900Context &c = a1ck::ckCtx(ctx);
    ref::get_mode_common_data_filter_base_dist_to_look_at(rdram, &c, runtime);
    a1ck::ckMid();
    const a1::RetVF r = ty::get_mode_common_data_filter_base_dist_to_look_at(rdram, runtime, p4);
    ckEnd(173, ctx, c, 4, r.v0, r.f0, 0x0u, 0x0u);
}
void delISysPathObj(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (!ckBegin(174, rdram, runtime)) return;
    R5900Context &c = a1ck::ckCtx(ctx);
    ref::delISysPathObj(rdram, &c, runtime);
    a1ck::ckMid();
    ty::delISysPathObj(rdram, runtime);
    ckEnd(174, ctx, c, 0, 0u, 0.0f, 0x0u, 0x0u);
}
void setDataHeaderFromAreaIdx(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (!ckBegin(175, rdram, runtime)) return;
    R5900Context &c = a1ck::ckCtx(ctx);
    ref::setDataHeaderFromAreaIdx(rdram, &c, runtime);
    a1ck::ckMid();
    const uint64_t r = a1_sx64(ty::setDataHeaderFromAreaIdx(rdram, runtime));
    ckEnd(175, ctx, c, 1, r, 0.0f, 0x0u, 0x0u);
}
void SCRConditionTrue(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (!ckBegin(176, rdram, runtime)) return;
    R5900Context &c = a1ck::ckCtx(ctx);
    ref::SCRConditionTrue(rdram, &c, runtime);
    a1ck::ckMid();
    const uint64_t r = a1_sx64(ty::SCRConditionTrue(rdram, runtime));
    ckEnd(176, ctx, c, 1, r, 0.0f, 0x0u, 0x0u);
}
void getSizeAimIKWork(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (!ckBegin(177, rdram, runtime)) return;
    R5900Context &c = a1ck::ckCtx(ctx);
    ref::getSizeAimIKWork(rdram, &c, runtime);
    a1ck::ckMid();
    const uint64_t r = a1_sx64(ty::getSizeAimIKWork(rdram, runtime));
    ckEnd(177, ctx, c, 1, r, 0.0f, 0x0u, 0x0u);
}
void getSizeAttackIKWork(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (!ckBegin(178, rdram, runtime)) return;
    R5900Context &c = a1ck::ckCtx(ctx);
    ref::getSizeAttackIKWork(rdram, &c, runtime);
    a1ck::ckMid();
    const uint64_t r = a1_sx64(ty::getSizeAttackIKWork(rdram, runtime));
    ckEnd(178, ctx, c, 1, r, 0.0f, 0x0u, 0x0u);
}
void getSizeBeardIKWork(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (!ckBegin(179, rdram, runtime)) return;
    R5900Context &c = a1ck::ckCtx(ctx);
    ref::getSizeBeardIKWork(rdram, &c, runtime);
    a1ck::ckMid();
    const uint64_t r = a1_sx64(ty::getSizeBeardIKWork(rdram, runtime));
    ckEnd(179, ctx, c, 1, r, 0.0f, 0x0u, 0x0u);
}
void getSizeTailIKWork(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (!ckBegin(180, rdram, runtime)) return;
    R5900Context &c = a1ck::ckCtx(ctx);
    ref::getSizeTailIKWork(rdram, &c, runtime);
    a1ck::ckMid();
    const uint64_t r = a1_sx64(ty::getSizeTailIKWork(rdram, runtime));
    ckEnd(180, ctx, c, 1, r, 0.0f, 0x0u, 0x0u);
}
void getEventScriptClass(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint64_t p4)
{
    if (!ckBegin(181, rdram, runtime)) return;
    R5900Context &c = a1ck::ckCtx(ctx);
    ref::getEventScriptClass(rdram, &c, runtime);
    a1ck::ckMid();
    const uint64_t r = (ty::getEventScriptClass(rdram, runtime, p4));
    ckEnd(181, ctx, c, 2, r, 0.0f, 0x18u, 0x0u);
}
void iosInitDC(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (!ckBegin(182, rdram, runtime)) return;
    R5900Context &c = a1ck::ckCtx(ctx);
    ref::iosInitDC(rdram, &c, runtime);
    a1ck::ckMid();
    ty::iosInitDC(rdram, runtime);
    ckEnd(182, ctx, c, 0, 0u, 0.0f, 0x10u, 0x0u);
}
void InitSubThread(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (!ckBegin(183, rdram, runtime)) return;
    R5900Context &c = a1ck::ckCtx(ctx);
    ref::InitSubThread(rdram, &c, runtime);
    a1ck::ckMid();
    ty::InitSubThread(rdram, runtime);
    ckEnd(183, ctx, c, 0, 0u, 0.0f, 0x10u, 0x0u);
}
void setSoundMode(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint64_t p4)
{
    if (!ckBegin(184, rdram, runtime)) return;
    R5900Context &c = a1ck::ckCtx(ctx);
    ref::setSoundMode(rdram, &c, runtime);
    a1ck::ckMid();
    const uint64_t r = a1_sx64(ty::setSoundMode(rdram, runtime, p4));
    ckEnd(184, ctx, c, 1, r, 0.0f, 0x38u, 0x0u);
}
void Sg2DmaTransStatus(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t p4)
{
    if (!ckBegin(185, rdram, runtime)) return;
    R5900Context &c = a1ck::ckCtx(ctx);
    ref::Sg2DmaTransStatus(rdram, &c, runtime);
    a1ck::ckMid();
    const uint64_t r = a1_sx64(ty::Sg2DmaTransStatus(rdram, runtime, p4));
    ckEnd(185, ctx, c, 1, r, 0.0f, 0x8u, 0x0u);
}
void Sg2VabBdOpen(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint64_t p4, uint64_t p5)
{
    if (!ckBegin(186, rdram, runtime)) return;
    R5900Context &c = a1ck::ckCtx(ctx);
    ref::Sg2VabBdOpen(rdram, &c, runtime);
    a1ck::ckMid();
    const uint64_t r = (ty::Sg2VabBdOpen(rdram, runtime, p4, p5));
    ckEnd(186, ctx, c, 2, r, 0.0f, 0x8u, 0x0u);
}
void GAMEUTIL_get_difference_value(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint64_t p4, float pf12, float pf13)
{
    if (!ckBegin(187, rdram, runtime)) return;
    R5900Context &c = a1ck::ckCtx(ctx);
    ref::GAMEUTIL_get_difference_value(rdram, &c, runtime);
    a1ck::ckMid();
    const a1::RetVF r = ty::GAMEUTIL_get_difference_value(rdram, runtime, p4, pf12, pf13);
    ckEnd(187, ctx, c, 4, r.v0, r.f0, 0x2u, 0x2u);
}
void GetCalcNumObjWorkSize(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint64_t p4, uint64_t p5, uint64_t p6, uint64_t p7, uint64_t p8)
{
    if (!ckBegin(188, rdram, runtime)) return;
    R5900Context &c = a1ck::ckCtx(ctx);
    ref::GetCalcNumObjWorkSize(rdram, &c, runtime);
    a1ck::ckMid();
    const uint64_t r = a1_sx64(ty::GetCalcNumObjWorkSize(rdram, runtime, p4, p5, p6, p7, p8));
    ckEnd(188, ctx, c, 1, r, 0.0f, 0x218u, 0x0u);
}
void GAMEUTIL_constructor_move_angle(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint64_t p4)
{
    if (!ckBegin(189, rdram, runtime)) return;
    R5900Context &c = a1ck::ckCtx(ctx);
    ref::GAMEUTIL_constructor_move_angle(rdram, &c, runtime);
    a1ck::ckMid();
    const float r = ty::GAMEUTIL_constructor_move_angle(rdram, runtime, p4);
    ckEnd(189, ctx, c, 3, 0u, r, 0x2u, 0x1eu);
}
void setFade(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, float pf12, float pf13)
{
    if (!ckBegin(190, rdram, runtime)) return;
    R5900Context &c = a1ck::ckCtx(ctx);
    ref::setFade(rdram, &c, runtime);
    a1ck::ckMid();
    const a1::RetVF r = ty::setFade(rdram, runtime, pf12, pf13);
    ckEnd(190, ctx, c, 4, r.v0, r.f0, 0x30u, 0x6u);
}
void getFade(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (!ckBegin(191, rdram, runtime)) return;
    R5900Context &c = a1ck::ckCtx(ctx);
    ref::getFade(rdram, &c, runtime);
    a1ck::ckMid();
    const a1::RetVF r = ty::getFade(rdram, runtime);
    ckEnd(191, ctx, c, 4, r.v0, r.f0, 0x10u, 0x2u);
}
void gamecore_sub_013527D8(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t p4)
{
    if (!ckBegin(192, rdram, runtime)) return;
    R5900Context &c = a1ck::ckCtx(ctx);
    ref::gamecore_sub_013527D8(rdram, &c, runtime);
    a1ck::ckMid();
    const uint64_t r = a1_sx64(ty::gamecore_sub_013527D8(rdram, runtime, p4));
    ckEnd(192, ctx, c, 1, r, 0.0f, 0x22u, 0x1000u);
}
void gamecore_sub_01357758(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (!ckBegin(193, rdram, runtime)) return;
    R5900Context &c = a1ck::ckCtx(ctx);
    ref::gamecore_sub_01357758(rdram, &c, runtime);
    a1ck::ckMid();
    const uint64_t r = a1_sx64(ty::gamecore_sub_01357758(rdram, runtime));
    ckEnd(193, ctx, c, 1, r, 0.0f, 0x32u, 0x1000u);
}
void gamecore_sub_01384EE0(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (!ckBegin(194, rdram, runtime)) return;
    R5900Context &c = a1ck::ckCtx(ctx);
    ref::gamecore_sub_01384EE0(rdram, &c, runtime);
    a1ck::ckMid();
    const uint64_t r = a1_sx64(ty::gamecore_sub_01384EE0(rdram, runtime));
    ckEnd(194, ctx, c, 1, r, 0.0f, 0x10u, 0x0u);
}
void get_pre_type_expression(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t p4)
{
    if (!ckBegin(195, rdram, runtime)) return;
    R5900Context &c = a1ck::ckCtx(ctx);
    ref::get_pre_type_expression(rdram, &c, runtime);
    a1ck::ckMid();
    const uint64_t r = a1_sx64(ty::get_pre_type_expression(rdram, runtime, p4));
    ckEnd(195, ctx, c, 1, r, 0.0f, 0x10u, 0x0u);
}
void MotSpdCtrl_Init(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint64_t p4, uint64_t p5)
{
    if (!ckBegin(196, rdram, runtime)) return;
    R5900Context &c = a1ck::ckCtx(ctx);
    ref::MotSpdCtrl_Init(rdram, &c, runtime);
    a1ck::ckMid();
    ty::MotSpdCtrl_Init(rdram, runtime, p4, p5);
    ckEnd(196, ctx, c, 0, 0u, 0.0f, 0x0u, 0x0u);
}
void gamecore_sub_013AE538(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (!ckBegin(197, rdram, runtime)) return;
    R5900Context &c = a1ck::ckCtx(ctx);
    ref::gamecore_sub_013AE538(rdram, &c, runtime);
    a1ck::ckMid();
    const uint64_t r = a1_sx64(ty::gamecore_sub_013AE538(rdram, runtime));
    ckEnd(197, ctx, c, 1, r, 0.0f, 0x10u, 0x0u);
}
void gamecore_sub_013AE550(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (!ckBegin(198, rdram, runtime)) return;
    R5900Context &c = a1ck::ckCtx(ctx);
    ref::gamecore_sub_013AE550(rdram, &c, runtime);
    a1ck::ckMid();
    const uint64_t r = a1_sx64(ty::gamecore_sub_013AE550(rdram, runtime));
    ckEnd(198, ctx, c, 1, r, 0.0f, 0x10u, 0x0u);
}
void gamecore_sub_013AE568(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (!ckBegin(199, rdram, runtime)) return;
    R5900Context &c = a1ck::ckCtx(ctx);
    ref::gamecore_sub_013AE568(rdram, &c, runtime);
    a1ck::ckMid();
    const uint64_t r = a1_sx64(ty::gamecore_sub_013AE568(rdram, runtime));
    ckEnd(199, ctx, c, 1, r, 0.0f, 0x10u, 0x0u);
}
void gamecore_sub_013AE580(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (!ckBegin(200, rdram, runtime)) return;
    R5900Context &c = a1ck::ckCtx(ctx);
    ref::gamecore_sub_013AE580(rdram, &c, runtime);
    a1ck::ckMid();
    const uint64_t r = a1_sx64(ty::gamecore_sub_013AE580(rdram, runtime));
    ckEnd(200, ctx, c, 1, r, 0.0f, 0x10u, 0x0u);
}
void gamecore_sub_013AE598(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (!ckBegin(201, rdram, runtime)) return;
    R5900Context &c = a1ck::ckCtx(ctx);
    ref::gamecore_sub_013AE598(rdram, &c, runtime);
    a1ck::ckMid();
    const uint64_t r = a1_sx64(ty::gamecore_sub_013AE598(rdram, runtime));
    ckEnd(201, ctx, c, 1, r, 0.0f, 0x10u, 0x0u);
}
void gamecore_sub_013AE5B0(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (!ckBegin(202, rdram, runtime)) return;
    R5900Context &c = a1ck::ckCtx(ctx);
    ref::gamecore_sub_013AE5B0(rdram, &c, runtime);
    a1ck::ckMid();
    const uint64_t r = a1_sx64(ty::gamecore_sub_013AE5B0(rdram, runtime));
    ckEnd(202, ctx, c, 1, r, 0.0f, 0x10u, 0x0u);
}
void gamecore_sub_013AE5C8(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (!ckBegin(203, rdram, runtime)) return;
    R5900Context &c = a1ck::ckCtx(ctx);
    ref::gamecore_sub_013AE5C8(rdram, &c, runtime);
    a1ck::ckMid();
    const uint64_t r = a1_sx64(ty::gamecore_sub_013AE5C8(rdram, runtime));
    ckEnd(203, ctx, c, 1, r, 0.0f, 0x10u, 0x0u);
}
void gamecore_sub_013AE5E0(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (!ckBegin(204, rdram, runtime)) return;
    R5900Context &c = a1ck::ckCtx(ctx);
    ref::gamecore_sub_013AE5E0(rdram, &c, runtime);
    a1ck::ckMid();
    const uint64_t r = a1_sx64(ty::gamecore_sub_013AE5E0(rdram, runtime));
    ckEnd(204, ctx, c, 1, r, 0.0f, 0x10u, 0x0u);
}
void gamecore_sub_013AE5F8(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (!ckBegin(205, rdram, runtime)) return;
    R5900Context &c = a1ck::ckCtx(ctx);
    ref::gamecore_sub_013AE5F8(rdram, &c, runtime);
    a1ck::ckMid();
    const uint64_t r = a1_sx64(ty::gamecore_sub_013AE5F8(rdram, runtime));
    ckEnd(205, ctx, c, 1, r, 0.0f, 0x10u, 0x0u);
}
void gamecore_sub_013AE610(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (!ckBegin(206, rdram, runtime)) return;
    R5900Context &c = a1ck::ckCtx(ctx);
    ref::gamecore_sub_013AE610(rdram, &c, runtime);
    a1ck::ckMid();
    const uint64_t r = a1_sx64(ty::gamecore_sub_013AE610(rdram, runtime));
    ckEnd(206, ctx, c, 1, r, 0.0f, 0x10u, 0x0u);
}
void gamecore_sub_013AE628(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (!ckBegin(207, rdram, runtime)) return;
    R5900Context &c = a1ck::ckCtx(ctx);
    ref::gamecore_sub_013AE628(rdram, &c, runtime);
    a1ck::ckMid();
    const uint64_t r = a1_sx64(ty::gamecore_sub_013AE628(rdram, runtime));
    ckEnd(207, ctx, c, 1, r, 0.0f, 0x10u, 0x0u);
}
void gamecore_sub_013AE880(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (!ckBegin(208, rdram, runtime)) return;
    R5900Context &c = a1ck::ckCtx(ctx);
    ref::gamecore_sub_013AE880(rdram, &c, runtime);
    a1ck::ckMid();
    const uint64_t r = a1_sx64(ty::gamecore_sub_013AE880(rdram, runtime));
    ckEnd(208, ctx, c, 1, r, 0.0f, 0x10u, 0x0u);
}
void gamecore_sub_013B77C8(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (!ckBegin(209, rdram, runtime)) return;
    R5900Context &c = a1ck::ckCtx(ctx);
    ref::gamecore_sub_013B77C8(rdram, &c, runtime);
    a1ck::ckMid();
    const a1::RetVF r = ty::gamecore_sub_013B77C8(rdram, runtime);
    ckEnd(209, ctx, c, 4, r.v0, r.f0, 0x2u, 0x0u);
}
void get_cur_attr_camera_default_angle(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t p4)
{
    if (!ckBegin(210, rdram, runtime)) return;
    R5900Context &c = a1ck::ckCtx(ctx);
    ref::get_cur_attr_camera_default_angle(rdram, &c, runtime);
    a1ck::ckMid();
    const uint64_t r = a1_sx64(ty::get_cur_attr_camera_default_angle(rdram, runtime, p4));
    ckEnd(210, ctx, c, 1, r, 0.0f, 0x0u, 0x0u);
}
void gamecore_sub_01410A90(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t p4)
{
    if (!ckBegin(211, rdram, runtime)) return;
    R5900Context &c = a1ck::ckCtx(ctx);
    ref::gamecore_sub_01410A90(rdram, &c, runtime);
    a1ck::ckMid();
    ty::gamecore_sub_01410A90(rdram, runtime, p4);
    ckEnd(211, ctx, c, 0, 0u, 0.0f, 0x34u, 0x0u);
}
void get_status_camera_target_approach(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t p4)
{
    if (!ckBegin(212, rdram, runtime)) return;
    R5900Context &c = a1ck::ckCtx(ctx);
    ref::get_status_camera_target_approach(rdram, &c, runtime);
    a1ck::ckMid();
    const uint64_t r = a1_sx64(ty::get_status_camera_target_approach(rdram, runtime, p4));
    ckEnd(212, ctx, c, 1, r, 0.0f, 0x0u, 0x0u);
}
void CAM_init_camera_warp_cntrl(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint64_t p4, uint32_t p5, uint32_t p6, uint32_t p7)
{
    if (!ckBegin(213, rdram, runtime)) return;
    R5900Context &c = a1ck::ckCtx(ctx);
    ref::CAM_init_camera_warp_cntrl(rdram, &c, runtime);
    a1ck::ckMid();
    const a1::RetVF r = ty::CAM_init_camera_warp_cntrl(rdram, runtime, p4, p5, p6, p7);
    ckEnd(213, ctx, c, 4, r.v0, r.f0, 0x2u, 0x2u);
}
void gamecore_sub_0141C9B8(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint64_t p4, uint32_t p5, uint64_t p6)
{
    if (!ckBegin(214, rdram, runtime)) return;
    R5900Context &c = a1ck::ckCtx(ctx);
    ref::gamecore_sub_0141C9B8(rdram, &c, runtime);
    a1ck::ckMid();
    const a1::RetVF r = ty::gamecore_sub_0141C9B8(rdram, runtime, p4, p5, p6);
    ckEnd(214, ctx, c, 4, r.v0, r.f0, 0x1au, 0x2u);
}
void get_main_cameraman_camera_obj(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (!ckBegin(215, rdram, runtime)) return;
    R5900Context &c = a1ck::ckCtx(ctx);
    ref::get_main_cameraman_camera_obj(rdram, &c, runtime);
    a1ck::ckMid();
    const uint64_t r = a1_sx64(ty::get_main_cameraman_camera_obj(rdram, runtime));
    ckEnd(215, ctx, c, 1, r, 0.0f, 0x18u, 0x0u);
}
void request_end_pause_rise_game_camera(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint64_t p4, uint64_t p5)
{
    if (!ckBegin(216, rdram, runtime)) return;
    R5900Context &c = a1ck::ckCtx(ctx);
    ref::request_end_pause_rise_game_camera(rdram, &c, runtime);
    a1ck::ckMid();
    const uint64_t r = a1_sx64(ty::request_end_pause_rise_game_camera(rdram, runtime, p4, p5));
    ckEnd(216, ctx, c, 1, r, 0.0f, 0x70u, 0x0u);
}
void is_end_pause_rise_game_camera(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t p4)
{
    if (!ckBegin(217, rdram, runtime)) return;
    R5900Context &c = a1ck::ckCtx(ctx);
    ref::is_end_pause_rise_game_camera(rdram, &c, runtime);
    a1ck::ckMid();
    const uint64_t r = a1_sx64(ty::is_end_pause_rise_game_camera(rdram, runtime, p4));
    ckEnd(217, ctx, c, 1, r, 0.0f, 0x10u, 0x0u);
}
void getPathColiGroup(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t p4)
{
    if (!ckBegin(218, rdram, runtime)) return;
    R5900Context &c = a1ck::ckCtx(ctx);
    ref::getPathColiGroup(rdram, &c, runtime);
    a1ck::ckMid();
    const uint64_t r = a1_sx64(ty::getPathColiGroup(rdram, runtime, p4));
    ckEnd(218, ctx, c, 1, r, 0.0f, 0x18u, 0x0u);
}
void _resetBaseMoveWork(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint64_t p5)
{
    if (!ckBegin(219, rdram, runtime)) return;
    R5900Context &c = a1ck::ckCtx(ctx);
    ref::_resetBaseMoveWork(rdram, &c, runtime);
    a1ck::ckMid();
    const a1::RetVF r = ty::_resetBaseMoveWork(rdram, runtime, p5);
    ckEnd(219, ctx, c, 4, r.v0, r.f0, 0x12u, 0x2u);
}
void SCRConditionSwordFlareHitColAttr(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint64_t p8, uint64_t p9)
{
    if (!ckBegin(220, rdram, runtime)) return;
    R5900Context &c = a1ck::ckCtx(ctx);
    ref::SCRConditionSwordFlareHitColAttr(rdram, &c, runtime);
    a1ck::ckMid();
    const uint64_t r = (ty::SCRConditionSwordFlareHitColAttr(rdram, runtime, p8, p9));
    ckEnd(220, ctx, c, 2, r, 0.0f, 0x8u, 0x0u);
}
void SCRConditionHardMode(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (!ckBegin(221, rdram, runtime)) return;
    R5900Context &c = a1ck::ckCtx(ctx);
    ref::SCRConditionHardMode(rdram, &c, runtime);
    a1ck::ckMid();
    const uint64_t r = a1_sx64(ty::SCRConditionHardMode(rdram, runtime));
    ckEnd(221, ctx, c, 1, r, 0.0f, 0x8u, 0x0u);
}
void gamecore_sub_0145B1C0(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint64_t p5, uint64_t p8)
{
    if (!ckBegin(222, rdram, runtime)) return;
    R5900Context &c = a1ck::ckCtx(ctx);
    ref::gamecore_sub_0145B1C0(rdram, &c, runtime);
    a1ck::ckMid();
    const uint64_t r = a1_sx64(ty::gamecore_sub_0145B1C0(rdram, runtime, p5, p8));
    ckEnd(222, ctx, c, 1, r, 0.0f, 0x32u, 0x1000u);
}
void is_reach_src_value_double_spring_interp_cntrl(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint64_t p4, float pf0, float pf12)
{
    if (!ckBegin(223, rdram, runtime)) return;
    R5900Context &c = a1ck::ckCtx(ctx);
    ref::is_reach_src_value_double_spring_interp_cntrl(rdram, &c, runtime);
    a1ck::ckMid();
    const a1::RetVF r = ty::is_reach_src_value_double_spring_interp_cntrl(rdram, runtime, p4, pf0, pf12);
    ckEnd(223, ctx, c, 4, r.v0, r.f0, 0x12u, 0x3002u);
}
void gamecore_sub_014269D0(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint64_t p4)
{
    if (!ckBegin(224, rdram, runtime)) return;
    R5900Context &c = a1ck::ckCtx(ctx);
    ref::gamecore_sub_014269D0(rdram, &c, runtime);
    a1ck::ckMid();
    const a1::RetVF r = ty::gamecore_sub_014269D0(rdram, runtime, p4);
    ckEnd(224, ctx, c, 4, r.v0, r.f0, 0xf2u, 0x2u);
}
const a1ck::CkEntry kEntries[225] = {
    {"memprintf", 0x103ff8u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { memprintf(rdram, ctx, runtime, ctx->f[12], ctx->f[13], ctx->f[14], ctx->f[15], ctx->f[16], ctx->f[17], ctx->f[18], ctx->f[19]); }},
    {"imemprintf", 0x104040u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { imemprintf(rdram, ctx, runtime, ctx->f[12], ctx->f[13], ctx->f[14], ctx->f[15], ctx->f[16], ctx->f[17], ctx->f[18], ctx->f[19]); }},
    {"PutString", 0x105948u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { PutString(rdram, ctx, runtime, ctx->f[12], ctx->f[13], ctx->f[14], ctx->f[15], ctx->f[16], ctx->f[17], ctx->f[18], ctx->f[19]); }},
    {"PutStringS", 0x105990u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { PutStringS(rdram, ctx, runtime, ctx->f[12], ctx->f[13], ctx->f[14], ctx->f[15], ctx->f[16], ctx->f[17], ctx->f[18], ctx->f[19]); }},
    {"sub_00108D10", 0x108d10u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { sub_00108D10(rdram, ctx, runtime, (uint64_t)Ps2GprU64(ctx, 4), (uint32_t)Ps2GprU32(ctx, 5), (uint64_t)Ps2GprU64(ctx, 6)); }},
    {"sub_00116C58", 0x116c58u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { sub_00116C58(rdram, ctx, runtime); }},
    {"ps2___fpcmp_parts_d", 0x127270u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { ps2___fpcmp_parts_d(rdram, ctx, runtime, (uint64_t)Ps2GprU64(ctx, 4), (uint32_t)Ps2GprU32(ctx, 5)); }},
    {"iosCdStSectorAlignSkip", 0x1acf60u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { iosCdStSectorAlignSkip(rdram, ctx, runtime); }},
    {"iosGetTCount", 0x1ad510u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { iosGetTCount(rdram, ctx, runtime); }},
    {"iosGetCpuRapCountPar1Int", 0x1ad658u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { iosGetCpuRapCountPar1Int(rdram, ctx, runtime); }},
    {"iosSprToMemDmaAdrCopy", 0x1b2a80u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { iosSprToMemDmaAdrCopy(rdram, ctx, runtime, (uint32_t)Ps2GprU32(ctx, 4), (uint32_t)Ps2GprU32(ctx, 5), (uint32_t)Ps2GprU32(ctx, 6)); }},
    {"kernel_sub_001B4120", 0x1b4120u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { kernel_sub_001B4120(rdram, ctx, runtime, ctx->f[12], ctx->f[13], ctx->f[14], ctx->f[15], ctx->f[16], ctx->f[17], ctx->f[18], ctx->f[19]); }},
    {"PopQuaternion", 0x1bbc08u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { PopQuaternion(rdram, ctx, runtime); }},
    {"iosGetACosf", 0x1be260u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { iosGetACosf(rdram, ctx, runtime, ctx->f[12]); }},
    {"iosDlGetCreateAdrs", 0x1c0458u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { iosDlGetCreateAdrs(rdram, ctx, runtime); }},
    {"getVIntrCnt", 0x1c7d98u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { getVIntrCnt(rdram, ctx, runtime); }},
    {"setVIntrBaseTime", 0x1c7db0u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { setVIntrBaseTime(rdram, ctx, runtime, (uint32_t)Ps2GprU32(ctx, 4)); }},
    {"iosGetFontDataWithSJIS", 0x1c91f8u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { iosGetFontDataWithSJIS(rdram, ctx, runtime, (uint64_t)Ps2GprU64(ctx, 4), (uint64_t)Ps2GprU64(ctx, 5), (uint64_t)Ps2GprU64(ctx, 6)); }},
    {"iosGetFontIDWithSJIS", 0x1c9498u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { iosGetFontIDWithSJIS(rdram, ctx, runtime, (uint64_t)Ps2GprU64(ctx, 4)); }},
    {"kernel_sub_001D1420", 0x1d1420u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { kernel_sub_001D1420(rdram, ctx, runtime); }},
    {"kernel_sub_001D15B8", 0x1d15b8u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { kernel_sub_001D15B8(rdram, ctx, runtime); }},
    {"vab_get_splitchunk", 0x1da330u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { vab_get_splitchunk(rdram, ctx, runtime, (uint64_t)Ps2GprU64(ctx, 4)); }},
    {"kernel_sub_001DB418", 0x1db418u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { kernel_sub_001DB418(rdram, ctx, runtime, (uint32_t)Ps2GprU32(ctx, 4), (uint32_t)Ps2GprU32(ctx, 5)); }},
    {"GAMEUTIL_is_yaw_pitch_get_direction", 0x10817c8u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { GAMEUTIL_is_yaw_pitch_get_direction(rdram, ctx, runtime, (uint32_t)Ps2GprU32(ctx, 4)); }},
    {"entrymanager", 0x1082ed0u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { entrymanager(rdram, ctx, runtime); }},
    {"reset_double_spring_interp_cntrl", 0x1084590u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { reset_double_spring_interp_cntrl(rdram, ctx, runtime, (uint32_t)Ps2GprU32(ctx, 4), ctx->f[12]); }},
    {"setShadowEffect", 0x10871c8u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { setShadowEffect(rdram, ctx, runtime); }},
    {"fsEffectDefault", 0x1088e58u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { fsEffectDefault(rdram, ctx, runtime); }},
    {"deleteFsEffect", 0x1089118u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { deleteFsEffect(rdram, ctx, runtime); }},
    {"deleteFogEffect", 0x1089270u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { deleteFogEffect(rdram, ctx, runtime); }},
    {"manager_sub_01089288", 0x1089288u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { manager_sub_01089288(rdram, ctx, runtime); }},
    {"getFogFactor0", 0x1089b70u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { getFogFactor0(rdram, ctx, runtime); }},
    {"ModelDispGetWorkSize", 0x1099a00u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { ModelDispGetWorkSize(rdram, ctx, runtime); }},
    {"ptclCounterInit", 0x109a4c8u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { ptclCounterInit(rdram, ctx, runtime); }},
    {"manager_sub_0109ACF8", 0x109acf8u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { manager_sub_0109ACF8(rdram, ctx, runtime); }},
    {"manager_sub_0109AD00", 0x109ad00u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { manager_sub_0109AD00(rdram, ctx, runtime); }},
    {"shadowGetWorkSize", 0x109deb0u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { shadowGetWorkSize(rdram, ctx, runtime); }},
    {"zFogInit", 0x10ac470u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { zFogInit(rdram, ctx, runtime); }},
    {"get_type_parameter_segment_motion_ik_base", 0x10b92c0u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { get_type_parameter_segment_motion_ik_base(rdram, ctx, runtime, (uint32_t)Ps2GprU32(ctx, 4)); }},
    {"request_end_motion_ik_interest", 0x10ba120u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { request_end_motion_ik_interest(rdram, ctx, runtime, (uint32_t)Ps2GprU32(ctx, 4)); }},
    {"getSizeGazeIKWork", 0x10ba7a0u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { getSizeGazeIKWork(rdram, ctx, runtime); }},
    {"GetAnimationDefClass", 0x10bb060u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { GetAnimationDefClass(rdram, ctx, runtime, (uint64_t)Ps2GprU64(ctx, 4)); }},
    {"manager_sub_010BB208", 0x10bb208u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { manager_sub_010BB208(rdram, ctx, runtime); }},
    {"RequestCharacterMotionShiftFunc", 0x10c1530u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { RequestCharacterMotionShiftFunc(rdram, ctx, runtime, (uint64_t)Ps2GprU64(ctx, 4), (uint32_t)Ps2GprU32(ctx, 5)); }},
    {"SetCharacterObjControlCallBackFunc", 0x10c1d08u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { SetCharacterObjControlCallBackFunc(rdram, ctx, runtime, (uint64_t)Ps2GprU64(ctx, 4), (uint32_t)Ps2GprU32(ctx, 5)); }},
    {"SetInertiaRotInterpMode", 0x10c29a0u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { SetInertiaRotInterpMode(rdram, ctx, runtime, (uint64_t)Ps2GprU64(ctx, 4)); }},
    {"charrootdyn_getSlideCollisionJudgeCBArg", 0x10dc898u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { charrootdyn_getSlideCollisionJudgeCBArg(rdram, ctx, runtime, (uint64_t)Ps2GprU64(ctx, 4), (uint32_t)Ps2GprU32(ctx, 5), (uint64_t)Ps2GprU64(ctx, 6)); }},
    {"manager_sub_010ED778", 0x10ed778u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { manager_sub_010ED778(rdram, ctx, runtime); }},
    {"FlushJudgeVolumeClass", 0x10ee6f8u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { FlushJudgeVolumeClass(rdram, ctx, runtime); }},
    {"GetGameLightClass", 0x10f74e8u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { GetGameLightClass(rdram, ctx, runtime, (uint64_t)Ps2GprU64(ctx, 4)); }},
    {"GetGlobalLightClass", 0x10f8b20u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { GetGlobalLightClass(rdram, ctx, runtime, (uint64_t)Ps2GprU64(ctx, 4)); }},
    {"GetAmbientLightClass", 0x10f9508u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { GetAmbientLightClass(rdram, ctx, runtime, (uint64_t)Ps2GprU64(ctx, 4)); }},
    {"manager_sub_010F9528", 0x10f9528u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { manager_sub_010F9528(rdram, ctx, runtime); }},
    {"createBinaryMemoryInfoWindow", 0x1101688u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { createBinaryMemoryInfoWindow(rdram, ctx, runtime); }},
    {"skelSegDel", 0x11017e0u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { skelSegDel(rdram, ctx, runtime); }},
    {"debugCreateAnimObjSegmentDisp", 0x11017e8u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { debugCreateAnimObjSegmentDisp(rdram, ctx, runtime); }},
    {"debugDeleteAnimObjSegmentDisp", 0x11017f0u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { debugDeleteAnimObjSegmentDisp(rdram, ctx, runtime); }},
    {"debugCreateAnimObjSegmentDefDisp", 0x11017f8u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { debugCreateAnimObjSegmentDefDisp(rdram, ctx, runtime); }},
    {"debugDeleteAnimObjSegmentDefDisp", 0x1101800u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { debugDeleteAnimObjSegmentDefDisp(rdram, ctx, runtime); }},
    {"scriptIntrSegInit", 0x1102fd8u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { scriptIntrSegInit(rdram, ctx, runtime); }},
    {"GetScriptAnimationClass", 0x110a838u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { GetScriptAnimationClass(rdram, ctx, runtime, (uint64_t)Ps2GprU64(ctx, 4)); }},
    {"SetScriptAnimationObjChildEntryGroup", 0x110a8e0u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { SetScriptAnimationObjChildEntryGroup(rdram, ctx, runtime, (uint32_t)Ps2GprU32(ctx, 4), (uint32_t)Ps2GprU32(ctx, 5)); }},
    {"SetScariptAnimationObjMotSyncSlotId", 0x110bb30u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { SetScariptAnimationObjMotSyncSlotId(rdram, ctx, runtime, (uint32_t)Ps2GprU32(ctx, 4), (uint32_t)Ps2GprU32(ctx, 5)); }},
    {"SetScriptCharacterObjReleaseCallBackFunc", 0x110cc78u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { SetScriptCharacterObjReleaseCallBackFunc(rdram, ctx, runtime, (uint32_t)Ps2GprU32(ctx, 4), (uint32_t)Ps2GprU32(ctx, 5), (uint32_t)Ps2GprU32(ctx, 6)); }},
    {"GetScrLwsOriClass", 0x110d868u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { GetScrLwsOriClass(rdram, ctx, runtime, (uint64_t)Ps2GprU64(ctx, 4)); }},
    {"GetScriptLwsOrientDefinfo", 0x110da90u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { GetScriptLwsOrientDefinfo(rdram, ctx, runtime, (uint64_t)Ps2GprU64(ctx, 4)); }},
    {"manager_sub_0110DC18", 0x110dc18u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { manager_sub_0110DC18(rdram, ctx, runtime); }},
    {"SpriteMenuIsFadeCheckSelectLayer", 0x110ecd0u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { SpriteMenuIsFadeCheckSelectLayer(rdram, ctx, runtime, (uint32_t)Ps2GprU32(ctx, 4)); }},
    {"GetScriptSpriteMenuClass", 0x11119b8u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { GetScriptSpriteMenuClass(rdram, ctx, runtime, (uint64_t)Ps2GprU64(ctx, 4)); }},
    {"SpriteMenuSetLayerPriority", 0x1111e98u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { SpriteMenuSetLayerPriority(rdram, ctx, runtime, (uint32_t)Ps2GprU32(ctx, 4), (uint32_t)Ps2GprU32(ctx, 5), (uint32_t)Ps2GprU32(ctx, 6)); }},
    {"SpriteMenuGetSelectLayer", 0x1112ba0u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { SpriteMenuGetSelectLayer(rdram, ctx, runtime, (uint32_t)Ps2GprU32(ctx, 4)); }},
    {"SpriteMenuGetPadEvent", 0x1112bb8u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { SpriteMenuGetPadEvent(rdram, ctx, runtime, (uint32_t)Ps2GprU32(ctx, 4)); }},
    {"SpriteMenuGetSelectSubMenuId", 0x1112f50u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { SpriteMenuGetSelectSubMenuId(rdram, ctx, runtime, (uint32_t)Ps2GprU32(ctx, 4)); }},
    {"SpriteMenuIsFadeCheckLayer", 0x11138c8u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { SpriteMenuIsFadeCheckLayer(rdram, ctx, runtime, (uint32_t)Ps2GprU32(ctx, 4), (uint32_t)Ps2GprU32(ctx, 5)); }},
    {"SpriteMenuGetFadeByLayer", 0x1113950u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { SpriteMenuGetFadeByLayer(rdram, ctx, runtime, (uint32_t)Ps2GprU32(ctx, 4), (uint32_t)Ps2GprU32(ctx, 5)); }},
    {"GetScriptSpriteClass", 0x1114238u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { GetScriptSpriteClass(rdram, ctx, runtime, (uint64_t)Ps2GprU64(ctx, 4)); }},
    {"GetSubLinkClass", 0x1114490u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { GetSubLinkClass(rdram, ctx, runtime, (uint64_t)Ps2GprU64(ctx, 4)); }},
    {"CheckCollisionFinderAlive", 0x112a450u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { CheckCollisionFinderAlive(rdram, ctx, runtime, (uint32_t)Ps2GprU32(ctx, 4)); }},
    {"CopyGeometryPointer", 0x11327b0u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { CopyGeometryPointer(rdram, ctx, runtime, (uint32_t)Ps2GprU32(ctx, 4), (uint32_t)Ps2GprU32(ctx, 5)); }},
    {"free_all_emitter_and_particle_particle_manager", 0x113f370u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { free_all_emitter_and_particle_particle_manager(rdram, ctx, runtime); }},
    {"get_particle_effect_id_from_sheet", 0x113fa20u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { get_particle_effect_id_from_sheet(rdram, ctx, runtime, (uint64_t)Ps2GprU64(ctx, 4)); }},
    {"get_particle_emitter_from_emitter_id", 0x113fa70u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { get_particle_emitter_from_emitter_id(rdram, ctx, runtime, (uint64_t)Ps2GprU64(ctx, 4)); }},
    {"get_main_particle_effect_lod_from_sheet", 0x113fa88u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { get_main_particle_effect_lod_from_sheet(rdram, ctx, runtime, (uint32_t)Ps2GprU32(ctx, 4)); }},
    {"get_sub_particle_effect_lod_from_sheet", 0x113fa90u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { get_sub_particle_effect_lod_from_sheet(rdram, ctx, runtime, (uint32_t)Ps2GprU32(ctx, 4)); }},
    {"get_top_particle_effect_from_lod", 0x113fa98u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { get_top_particle_effect_from_lod(rdram, ctx, runtime, (uint32_t)Ps2GprU32(ctx, 4)); }},
    {"get_radiate_range_coefficient", 0x11401d8u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { get_radiate_range_coefficient(rdram, ctx, runtime, ctx->f[12]); }},
    {"unload_npe_file_main", 0x1140730u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { unload_npe_file_main(rdram, ctx, runtime); }},
    {"unload_npe_file_sub", 0x1140738u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { unload_npe_file_sub(rdram, ctx, runtime); }},
    {"manager_sub_01140740", 0x1140740u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { manager_sub_01140740(rdram, ctx, runtime, (uint64_t)Ps2GprU64(ctx, 4)); }},
    {"manager_sub_01141080", 0x1141080u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { manager_sub_01141080(rdram, ctx, runtime, (uint32_t)Ps2GprU32(ctx, 4)); }},
    {"manager_sub_01142030", 0x1142030u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { manager_sub_01142030(rdram, ctx, runtime, (uint32_t)Ps2GprU32(ctx, 4)); }},
    {"manager_sub_01144E00", 0x1144e00u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { manager_sub_01144E00(rdram, ctx, runtime, (uint32_t)Ps2GprU32(ctx, 4)); }},
    {"manager_sub_01144E38", 0x1144e38u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { manager_sub_01144E38(rdram, ctx, runtime, (uint32_t)Ps2GprU32(ctx, 4)); }},
    {"manager_sub_01144E50", 0x1144e50u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { manager_sub_01144E50(rdram, ctx, runtime, (uint32_t)Ps2GprU32(ctx, 4)); }},
    {"manager_sub_01144E58", 0x1144e58u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { manager_sub_01144E58(rdram, ctx, runtime, (uint32_t)Ps2GprU32(ctx, 4)); }},
    {"manager_sub_01144E70", 0x1144e70u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { manager_sub_01144E70(rdram, ctx, runtime, (uint32_t)Ps2GprU32(ctx, 4)); }},
    {"manager_sub_01144E88", 0x1144e88u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { manager_sub_01144E88(rdram, ctx, runtime, (uint32_t)Ps2GprU32(ctx, 4)); }},
    {"is_eternal_wind", 0x11464c0u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { is_eternal_wind(rdram, ctx, runtime, (uint32_t)Ps2GprU32(ctx, 4)); }},
    {"stop_wind_emitter_geometry_pointer_reference", 0x1146980u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { stop_wind_emitter_geometry_pointer_reference(rdram, ctx, runtime, (uint64_t)Ps2GprU64(ctx, 4)); }},
    {"stop_wind_emitter_callback", 0x11469d0u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { stop_wind_emitter_callback(rdram, ctx, runtime, (uint64_t)Ps2GprU64(ctx, 4)); }},
    {"end_wind_emitter", 0x1146a10u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { end_wind_emitter(rdram, ctx, runtime, (uint64_t)Ps2GprU64(ctx, 4)); }},
    {"add_wind_segment", 0x1147430u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { add_wind_segment(rdram, ctx, runtime); }},
    {"RelocateModifiedManager", 0x1147950u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { RelocateModifiedManager(rdram, ctx, runtime, (uint64_t)Ps2GprU64(ctx, 4), (uint64_t)Ps2GprU64(ctx, 5), (uint32_t)Ps2GprU32(ctx, 6)); }},
    {"GetSizeOfModifiedManagerWork", 0x1147a28u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { GetSizeOfModifiedManagerWork(rdram, ctx, runtime, (uint64_t)Ps2GprU64(ctx, 4)); }},
    {"sheetMgrChkStageIndexResolve", 0x115ee68u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { sheetMgrChkStageIndexResolve(rdram, ctx, runtime, (uint64_t)Ps2GprU64(ctx, 4)); }},
    {"add_camera_lws_animation_segment", 0x1162510u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { add_camera_lws_animation_segment(rdram, ctx, runtime); }},
    {"get_camera_position", 0x1162ef0u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { get_camera_position(rdram, ctx, runtime, (uint32_t)Ps2GprU32(ctx, 4)); }},
    {"get_camera_direction", 0x1162ef8u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { get_camera_direction(rdram, ctx, runtime, (uint32_t)Ps2GprU32(ctx, 4)); }},
    {"get_camera_up_direction", 0x1162f00u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { get_camera_up_direction(rdram, ctx, runtime, (uint32_t)Ps2GprU32(ctx, 4)); }},
    {"get_camera_obj_layout_class", 0x1162f08u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { get_camera_obj_layout_class(rdram, ctx, runtime, (uint32_t)Ps2GprU32(ctx, 4)); }},
    {"get_view_matrix", 0x1162fd0u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { get_view_matrix(rdram, ctx, runtime, (uint32_t)Ps2GprU32(ctx, 4)); }},
    {"get_manage_object_id_camera_region", 0x1163750u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { get_manage_object_id_camera_region(rdram, ctx, runtime, (uint64_t)Ps2GprU64(ctx, 4)); }},
    {"is_parent_obj_boss_camera_region", 0x1163788u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { is_parent_obj_boss_camera_region(rdram, ctx, runtime, (uint64_t)Ps2GprU64(ctx, 4)); }},
    {"get_priority_camera_region", 0x11637d0u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { get_priority_camera_region(rdram, ctx, runtime, (uint64_t)Ps2GprU64(ctx, 4)); }},
    {"get_region_type_camera_region", 0x1163888u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { get_region_type_camera_region(rdram, ctx, runtime, (uint64_t)Ps2GprU64(ctx, 4)); }},
    {"get_clip_matrix", 0x1164c70u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { get_clip_matrix(rdram, ctx, runtime, (uint32_t)Ps2GprU32(ctx, 4)); }},
    {"get_clip_matrix128", 0x1164c78u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { get_clip_matrix128(rdram, ctx, runtime, (uint32_t)Ps2GprU32(ctx, 4)); }},
    {"get_clip_matrix256", 0x1164c80u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { get_clip_matrix256(rdram, ctx, runtime, (uint32_t)Ps2GprU32(ctx, 4)); }},
    {"get_clip_matrix_fovignore", 0x1164c88u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { get_clip_matrix_fovignore(rdram, ctx, runtime, (uint32_t)Ps2GprU32(ctx, 4)); }},
    {"get_screen_matrix", 0x1164c90u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { get_screen_matrix(rdram, ctx, runtime, (uint32_t)Ps2GprU32(ctx, 4)); }},
    {"get_screen_matrix128", 0x1164c98u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { get_screen_matrix128(rdram, ctx, runtime, (uint32_t)Ps2GprU32(ctx, 4)); }},
    {"get_screen_matrix256", 0x1164ca0u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { get_screen_matrix256(rdram, ctx, runtime, (uint32_t)Ps2GprU32(ctx, 4)); }},
    {"get_VC_matrix", 0x1164cc0u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { get_VC_matrix(rdram, ctx, runtime, (uint32_t)Ps2GprU32(ctx, 4)); }},
    {"get_CS_matrix", 0x1164cc8u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { get_CS_matrix(rdram, ctx, runtime, (uint32_t)Ps2GprU32(ctx, 4)); }},
    {"get_WVC_matrix", 0x1164cd0u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { get_WVC_matrix(rdram, ctx, runtime, (uint32_t)Ps2GprU32(ctx, 4)); }},
    {"get_VCS_matrix", 0x1164cd8u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { get_VCS_matrix(rdram, ctx, runtime, (uint32_t)Ps2GprU32(ctx, 4)); }},
    {"get_WVCS_matrix", 0x1164ce0u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { get_WVCS_matrix(rdram, ctx, runtime, (uint32_t)Ps2GprU32(ctx, 4)); }},
    {"get_scissor_VC_matrix", 0x1164ce8u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { get_scissor_VC_matrix(rdram, ctx, runtime, (uint32_t)Ps2GprU32(ctx, 4)); }},
    {"get_scissor_WVC_matrix", 0x1164cf0u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { get_scissor_WVC_matrix(rdram, ctx, runtime, (uint32_t)Ps2GprU32(ctx, 4)); }},
    {"getPointDataHeader", 0x1169d18u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { getPointDataHeader(rdram, ctx, runtime, (uint64_t)Ps2GprU64(ctx, 4)); }},
    {"getLineDataHeader", 0x1169d30u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { getLineDataHeader(rdram, ctx, runtime, (uint64_t)Ps2GprU64(ctx, 4)); }},
    {"getConnectLineArray", 0x1169d88u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { getConnectLineArray(rdram, ctx, runtime, (uint64_t)Ps2GprU64(ctx, 4), (uint64_t)Ps2GprU64(ctx, 5)); }},
    {"getRootDataHeader", 0x1169e20u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { getRootDataHeader(rdram, ctx, runtime, (uint64_t)Ps2GprU64(ctx, 4)); }},
    {"getGoalDataHeader", 0x1169e38u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { getGoalDataHeader(rdram, ctx, runtime, (uint64_t)Ps2GprU64(ctx, 4)); }},
    {"getSubDataHeader", 0x1169e50u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { getSubDataHeader(rdram, ctx, runtime, (uint64_t)Ps2GprU64(ctx, 4)); }},
    {"getChildGoalPointIdxArray", 0x1169f28u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { getChildGoalPointIdxArray(rdram, ctx, runtime, (uint64_t)Ps2GprU64(ctx, 4), (uint64_t)Ps2GprU64(ctx, 5)); }},
    {"getChildSubPointIdxArray", 0x1169f80u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { getChildSubPointIdxArray(rdram, ctx, runtime, (uint64_t)Ps2GprU64(ctx, 4), (uint64_t)Ps2GprU64(ctx, 5)); }},
    {"getInterestDataHeader", 0x1169fd8u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { getInterestDataHeader(rdram, ctx, runtime, (uint64_t)Ps2GprU64(ctx, 4)); }},
    {"getDeadDataHeader", 0x116a028u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { getDeadDataHeader(rdram, ctx, runtime, (uint64_t)Ps2GprU64(ctx, 4)); }},
    {"getLayoutDataHeader", 0x116a080u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { getLayoutDataHeader(rdram, ctx, runtime, (uint64_t)Ps2GprU64(ctx, 4)); }},
    {"GAMEUTIL_is_push_or_down_1p_button", 0x1342ef8u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { GAMEUTIL_is_push_or_down_1p_button(rdram, ctx, runtime, (uint64_t)Ps2GprU64(ctx, 4), (uint32_t)Ps2GprU32(ctx, 5)); }},
    {"GAMEUTIL_get_data_record_info", 0x1344588u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { GAMEUTIL_get_data_record_info(rdram, ctx, runtime, (uint32_t)Ps2GprU32(ctx, 4), (uint64_t)Ps2GprU64(ctx, 5)); }},
    {"gStatSaveDataGameFlagChk", 0x1353580u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { gStatSaveDataGameFlagChk(rdram, ctx, runtime, (uint64_t)Ps2GprU64(ctx, 4)); }},
    {"gStatGetGObjAdrs", 0x13536d8u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { gStatGetGObjAdrs(rdram, ctx, runtime, (uint32_t)Ps2GprU32(ctx, 4)); }},
    {"getstagenumber", 0x135ff08u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { getstagenumber(rdram, ctx, runtime, (uint64_t)Ps2GprU64(ctx, 4)); }},
    {"initStageLayout", 0x1366cf0u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { initStageLayout(rdram, ctx, runtime); }},
    {"deleteStageLayout", 0x1366cf8u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { deleteStageLayout(rdram, ctx, runtime); }},
    {"initAdpcmOrient", 0x136a420u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { initAdpcmOrient(rdram, ctx, runtime); }},
    {"bgmStartGameOver", 0x136aae8u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { bgmStartGameOver(rdram, ctx, runtime); }},
    {"bgmStageInit", 0x136aaf8u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { bgmStageInit(rdram, ctx, runtime); }},
    {"SCRExecBossPostProcess", 0x13790a0u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { SCRExecBossPostProcess(rdram, ctx, runtime); }},
    {"SCRPlayerReset", 0x13b6198u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { SCRPlayerReset(rdram, ctx, runtime); }},
    {"gamecore_sub_013B6530", 0x13b6530u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { gamecore_sub_013B6530(rdram, ctx, runtime); }},
    {"scrInit", 0x13cfb60u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { scrInit(rdram, ctx, runtime); }},
    {"AI_CheckNormZero", 0x13d4df8u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { AI_CheckNormZero(rdram, ctx, runtime, (uint32_t)Ps2GprU32(ctx, 4)); }},
    {"initSavePoint", 0x140ae18u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { initSavePoint(rdram, ctx, runtime); }},
    {"getSprLogWorkSize", 0x140ae38u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { getSprLogWorkSize(rdram, ctx, runtime); }},
    {"getSprLogStatusSize", 0x140ae50u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { getSprLogStatusSize(rdram, ctx, runtime); }},
    {"get_camera_move_collision_group", 0x141ce38u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { get_camera_move_collision_group(rdram, ctx, runtime, (uint32_t)Ps2GprU32(ctx, 4)); }},
    {"get_game_camera", 0x141ce58u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { get_game_camera(rdram, ctx, runtime, (uint32_t)Ps2GprU32(ctx, 4)); }},
    {"get_cur_camera_target", 0x141ce60u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { get_cur_camera_target(rdram, ctx, runtime, (uint32_t)Ps2GprU32(ctx, 4)); }},
    {"gamecore_sub_0141D178", 0x141d178u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { gamecore_sub_0141D178(rdram, ctx, runtime); }},
    {"gamecore_sub_0141D180", 0x141d180u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { gamecore_sub_0141D180(rdram, ctx, runtime); }},
    {"gamecore_sub_0141D4F0", 0x141d4f0u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { gamecore_sub_0141D4F0(rdram, ctx, runtime); }},
    {"get_cur_using_lws_anime_mode_base_class", 0x1424f38u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { get_cur_using_lws_anime_mode_base_class(rdram, ctx, runtime, (uint64_t)Ps2GprU64(ctx, 4)); }},
    {"end_object_interest_game_camera", 0x1425050u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { end_object_interest_game_camera(rdram, ctx, runtime); }},
    {"free_camera_mode_manager_segment", 0x1425660u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { free_camera_mode_manager_segment(rdram, ctx, runtime); }},
    {"gamecore_sub_01425D20", 0x1425d20u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { gamecore_sub_01425D20(rdram, ctx, runtime, (uint64_t)Ps2GprU64(ctx, 4)); }},
    {"gamecore_sub_01426250", 0x1426250u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { gamecore_sub_01426250(rdram, ctx, runtime); }},
    {"gamecore_sub_014262B0", 0x14262b0u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { gamecore_sub_014262B0(rdram, ctx, runtime); }},
    {"gamecore_sub_014262B8", 0x14262b8u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { gamecore_sub_014262B8(rdram, ctx, runtime); }},
    {"gamecore_sub_01426368", 0x1426368u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { gamecore_sub_01426368(rdram, ctx, runtime); }},
    {"get_interp_game_camera_param_game_camera_param_manager", 0x142c120u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { get_interp_game_camera_param_game_camera_param_manager(rdram, ctx, runtime, (uint32_t)Ps2GprU32(ctx, 4)); }},
    {"get_mode_common_data_filter_base_dist_to_look_at", 0x142c428u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { get_mode_common_data_filter_base_dist_to_look_at(rdram, ctx, runtime, (uint32_t)Ps2GprU32(ctx, 4)); }},
    {"delISysPathObj", 0x1431c30u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { delISysPathObj(rdram, ctx, runtime); }},
    {"setDataHeaderFromAreaIdx", 0x1431da0u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { setDataHeaderFromAreaIdx(rdram, ctx, runtime); }},
    {"SCRConditionTrue", 0x14563b8u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { SCRConditionTrue(rdram, ctx, runtime); }},
    {"getSizeAimIKWork", 0x145d688u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { getSizeAimIKWork(rdram, ctx, runtime); }},
    {"getSizeAttackIKWork", 0x145dfc8u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { getSizeAttackIKWork(rdram, ctx, runtime); }},
    {"getSizeBeardIKWork", 0x145f0e0u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { getSizeBeardIKWork(rdram, ctx, runtime); }},
    {"getSizeTailIKWork", 0x14621b0u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { getSizeTailIKWork(rdram, ctx, runtime); }},
    {"getEventScriptClass", 0x1464aa0u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { getEventScriptClass(rdram, ctx, runtime, (uint64_t)Ps2GprU64(ctx, 4)); }},
    {"iosInitDC", 0x1bf400u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { iosInitDC(rdram, ctx, runtime); }},
    {"InitSubThread", 0x1c9828u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { InitSubThread(rdram, ctx, runtime); }},
    {"setSoundMode", 0x1cb430u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { setSoundMode(rdram, ctx, runtime, (uint64_t)Ps2GprU64(ctx, 4)); }},
    {"Sg2DmaTransStatus", 0x1cc300u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { Sg2DmaTransStatus(rdram, ctx, runtime, (uint32_t)Ps2GprU32(ctx, 4)); }},
    {"Sg2VabBdOpen", 0x1da530u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { Sg2VabBdOpen(rdram, ctx, runtime, (uint64_t)Ps2GprU64(ctx, 4), (uint64_t)Ps2GprU64(ctx, 5)); }},
    {"GAMEUTIL_get_difference_value", 0x1080220u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { GAMEUTIL_get_difference_value(rdram, ctx, runtime, (uint64_t)Ps2GprU64(ctx, 4), ctx->f[12], ctx->f[13]); }},
    {"GetCalcNumObjWorkSize", 0x10c1b90u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { GetCalcNumObjWorkSize(rdram, ctx, runtime, (uint64_t)Ps2GprU64(ctx, 4), (uint64_t)Ps2GprU64(ctx, 5), (uint64_t)Ps2GprU64(ctx, 6), (uint64_t)Ps2GprU64(ctx, 7), (uint64_t)Ps2GprU64(ctx, 8)); }},
    {"GAMEUTIL_constructor_move_angle", 0x1343c58u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { GAMEUTIL_constructor_move_angle(rdram, ctx, runtime, (uint64_t)Ps2GprU64(ctx, 4)); }},
    {"setFade", 0x13461d0u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { setFade(rdram, ctx, runtime, ctx->f[12], ctx->f[13]); }},
    {"getFade", 0x13461f0u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { getFade(rdram, ctx, runtime); }},
    {"gamecore_sub_013527D8", 0x13527d8u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { gamecore_sub_013527D8(rdram, ctx, runtime, (uint32_t)Ps2GprU32(ctx, 4)); }},
    {"gamecore_sub_01357758", 0x1357758u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { gamecore_sub_01357758(rdram, ctx, runtime); }},
    {"gamecore_sub_01384EE0", 0x1384ee0u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { gamecore_sub_01384EE0(rdram, ctx, runtime); }},
    {"get_pre_type_expression", 0x138be98u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { get_pre_type_expression(rdram, ctx, runtime, (uint32_t)Ps2GprU32(ctx, 4)); }},
    {"MotSpdCtrl_Init", 0x139aaf8u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { MotSpdCtrl_Init(rdram, ctx, runtime, (uint64_t)Ps2GprU64(ctx, 4), (uint64_t)Ps2GprU64(ctx, 5)); }},
    {"gamecore_sub_013AE538", 0x13ae538u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { gamecore_sub_013AE538(rdram, ctx, runtime); }},
    {"gamecore_sub_013AE550", 0x13ae550u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { gamecore_sub_013AE550(rdram, ctx, runtime); }},
    {"gamecore_sub_013AE568", 0x13ae568u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { gamecore_sub_013AE568(rdram, ctx, runtime); }},
    {"gamecore_sub_013AE580", 0x13ae580u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { gamecore_sub_013AE580(rdram, ctx, runtime); }},
    {"gamecore_sub_013AE598", 0x13ae598u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { gamecore_sub_013AE598(rdram, ctx, runtime); }},
    {"gamecore_sub_013AE5B0", 0x13ae5b0u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { gamecore_sub_013AE5B0(rdram, ctx, runtime); }},
    {"gamecore_sub_013AE5C8", 0x13ae5c8u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { gamecore_sub_013AE5C8(rdram, ctx, runtime); }},
    {"gamecore_sub_013AE5E0", 0x13ae5e0u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { gamecore_sub_013AE5E0(rdram, ctx, runtime); }},
    {"gamecore_sub_013AE5F8", 0x13ae5f8u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { gamecore_sub_013AE5F8(rdram, ctx, runtime); }},
    {"gamecore_sub_013AE610", 0x13ae610u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { gamecore_sub_013AE610(rdram, ctx, runtime); }},
    {"gamecore_sub_013AE628", 0x13ae628u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { gamecore_sub_013AE628(rdram, ctx, runtime); }},
    {"gamecore_sub_013AE880", 0x13ae880u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { gamecore_sub_013AE880(rdram, ctx, runtime); }},
    {"gamecore_sub_013B77C8", 0x13b77c8u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { gamecore_sub_013B77C8(rdram, ctx, runtime); }},
    {"get_cur_attr_camera_default_angle", 0x14109f8u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { get_cur_attr_camera_default_angle(rdram, ctx, runtime, (uint32_t)Ps2GprU32(ctx, 4)); }},
    {"gamecore_sub_01410A90", 0x1410a90u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { gamecore_sub_01410A90(rdram, ctx, runtime, (uint32_t)Ps2GprU32(ctx, 4)); }},
    {"get_status_camera_target_approach", 0x1414a48u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { get_status_camera_target_approach(rdram, ctx, runtime, (uint32_t)Ps2GprU32(ctx, 4)); }},
    {"CAM_init_camera_warp_cntrl", 0x141aa98u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { CAM_init_camera_warp_cntrl(rdram, ctx, runtime, (uint64_t)Ps2GprU64(ctx, 4), (uint32_t)Ps2GprU32(ctx, 5), (uint32_t)Ps2GprU32(ctx, 6), (uint32_t)Ps2GprU32(ctx, 7)); }},
    {"gamecore_sub_0141C9B8", 0x141c9b8u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { gamecore_sub_0141C9B8(rdram, ctx, runtime, (uint64_t)Ps2GprU64(ctx, 4), (uint32_t)Ps2GprU32(ctx, 5), (uint64_t)Ps2GprU64(ctx, 6)); }},
    {"get_main_cameraman_camera_obj", 0x141d390u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { get_main_cameraman_camera_obj(rdram, ctx, runtime); }},
    {"request_end_pause_rise_game_camera", 0x1424fe8u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { request_end_pause_rise_game_camera(rdram, ctx, runtime, (uint64_t)Ps2GprU64(ctx, 4), (uint64_t)Ps2GprU64(ctx, 5)); }},
    {"is_end_pause_rise_game_camera", 0x1425008u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { is_end_pause_rise_game_camera(rdram, ctx, runtime, (uint32_t)Ps2GprU32(ctx, 4)); }},
    {"getPathColiGroup", 0x1431c58u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { getPathColiGroup(rdram, ctx, runtime, (uint32_t)Ps2GprU32(ctx, 4)); }},
    {"_resetBaseMoveWork", 0x1437a88u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { _resetBaseMoveWork(rdram, ctx, runtime, (uint64_t)Ps2GprU64(ctx, 5)); }},
    {"SCRConditionSwordFlareHitColAttr", 0x1459360u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { SCRConditionSwordFlareHitColAttr(rdram, ctx, runtime, (uint64_t)Ps2GprU64(ctx, 8), (uint64_t)Ps2GprU64(ctx, 9)); }},
    {"SCRConditionHardMode", 0x1459ae8u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { SCRConditionHardMode(rdram, ctx, runtime); }},
    {"gamecore_sub_0145B1C0", 0x145b1c0u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { gamecore_sub_0145B1C0(rdram, ctx, runtime, (uint64_t)Ps2GprU64(ctx, 5), (uint64_t)Ps2GprU64(ctx, 8)); }},
    {"is_reach_src_value_double_spring_interp_cntrl", 0x10845b0u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { is_reach_src_value_double_spring_interp_cntrl(rdram, ctx, runtime, (uint64_t)Ps2GprU64(ctx, 4), ctx->f[0], ctx->f[12]); }},
    {"gamecore_sub_014269D0", 0x14269d0u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { gamecore_sub_014269D0(rdram, ctx, runtime, (uint64_t)Ps2GprU64(ctx, 4)); }},
};
const uint32_t kEntryCount = 225u;
}
#if !defined(A2_OFFLINE)
void a2ckReport(unsigned long long frame) { a2ck::ckReport(frame); }
#endif
#endif
