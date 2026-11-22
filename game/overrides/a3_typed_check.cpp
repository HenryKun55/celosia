// a3_typed_check.cpp: round A3's differential check of the typed functions (tools/typed_lift2.py --check-tu;
// docs/decomp_a3.md). Generated: do not edit. Compiled to code only with PS2X_A3_CHECK=1 (a check build) or
// A3_OFFLINE (the offline harness, tools/a3_harness.cpp).
#include "typed_a3_check_rt.h"
#if PS2X_A3_CHECK || defined(A3_OFFLINE)
namespace a3ck
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
#define PS2_CALL_GUEST_DIRECT_NOCHK(t, f, r, s, k, n) a3ck::refCall((t), (r), rdram, ctx, runtime)
#define PS2_CALL_GUEST_NOCHK(t, f, r, s, k, n) a3ck::refCall((t), (r), rdram, ctx, runtime)
#define PS2_CALL_GUEST_DIRECT(t, f, r, s, k, n) a3ck::refCall((t), (r), rdram, ctx, runtime)
#define PS2_CALL_GUEST_KNOWN(t, f, r, s, k, n) a3ck::refCall((t), (r), rdram, ctx, runtime)
#define PS2_TAIL_GUEST_DIRECT(t, f) (void)a3ck::refCall((t), 0u, rdram, ctx, runtime)
#define PS2_TAIL_GUEST_KNOWN(t, f) (void)a3ck::refCall((t), 0u, rdram, ctx, runtime)

namespace a3ck
{
namespace ref
{
using lift_d1::sx64;
#include "typed_a1_ref.inc"
#include "typed_a2_ref.inc"
#include "typed_a3_ref.inc"
}
namespace ty
{
#include "typed_a1.inc"
#include "typed_a2.inc"
#include "typed_a3.inc"
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
    default: ckUnknownCall(t); return true;
    }
    (void)ret;
    return true;
}
void RelocateElfInfoHeader(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint64_t p4)
{
    if (!ckBegin(0, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    if (!ckLeg([&] { ref::RelocateElfInfoHeader(rdram, &c, runtime); })) return;
    ckMid();
    uint64_t v0 = 0u;
    float f0 = 0.0f;
    uint64_t xo[4] = {Ps2GprU64(ctx, 7), Ps2GprU64(ctx, 8), Ps2GprU64(ctx, 9), Ps2GprU64(ctx, 10)};
    if (!ckLeg([&] { v0 = a1_sx64(ty::RelocateElfInfoHeader(rdram, runtime, p4, xo)); })) return;
    ckEnd(0, ctx, c, 1, v0, f0, 0x7f8u, 0x0u, Ps2GprU64(&c, 7) != xo[0] || Ps2GprU64(&c, 8) != xo[1] || Ps2GprU64(&c, 9) != xo[2] || Ps2GprU64(&c, 10) != xo[3]);
}
void LoaderSysEntryExternalIntcHandlerList(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint64_t p4, uint32_t p5)
{
    if (!ckBegin(1, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    if (!ckLeg([&] { ref::LoaderSysEntryExternalIntcHandlerList(rdram, &c, runtime); })) return;
    ckMid();
    uint64_t v0 = 0u;
    float f0 = 0.0f;
    uint64_t xo[4] = {Ps2GprU64(ctx, 6), Ps2GprU64(ctx, 7), Ps2GprU64(ctx, 8), Ps2GprU64(ctx, 9)};
    if (!ckLeg([&] { v0 = a1_sx64(ty::LoaderSysEntryExternalIntcHandlerList(rdram, runtime, p4, p5, xo)); })) return;
    ckEnd(1, ctx, c, 1, v0, f0, 0x3d8u, 0x0u, Ps2GprU64(&c, 6) != xo[0] || Ps2GprU64(&c, 7) != xo[1] || Ps2GprU64(&c, 8) != xo[2] || Ps2GprU64(&c, 9) != xo[3]);
}
void LoaderSysEntryExternalThreadList(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t p4)
{
    if (!ckBegin(2, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    if (!ckLeg([&] { ref::LoaderSysEntryExternalThreadList(rdram, &c, runtime); })) return;
    ckMid();
    uint64_t v0 = 0u;
    float f0 = 0.0f;
    uint64_t xo[4] = {Ps2GprU64(ctx, 5), Ps2GprU64(ctx, 6), Ps2GprU64(ctx, 7), Ps2GprU64(ctx, 8)};
    if (!ckLeg([&] { v0 = a1_sx64(ty::LoaderSysEntryExternalThreadList(rdram, runtime, p4, xo)); })) return;
    ckEnd(2, ctx, c, 1, v0, f0, 0x1e8u, 0x0u, Ps2GprU64(&c, 5) != xo[0] || Ps2GprU64(&c, 6) != xo[1] || Ps2GprU64(&c, 7) != xo[2] || Ps2GprU64(&c, 8) != xo[3]);
}
void LoaderSysEntryExternalSemaList(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t p4)
{
    if (!ckBegin(3, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    if (!ckLeg([&] { ref::LoaderSysEntryExternalSemaList(rdram, &c, runtime); })) return;
    ckMid();
    uint64_t v0 = 0u;
    float f0 = 0.0f;
    if (!ckLeg([&] { v0 = a1_sx64(ty::LoaderSysEntryExternalSemaList(rdram, runtime, p4)); })) return;
    ckEnd(3, ctx, c, 1, v0, f0, 0x1e8u, 0x0u, false);
}
void LoaderSysEntryExternalIopMemoryList(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint64_t p4)
{
    if (!ckBegin(4, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    if (!ckLeg([&] { ref::LoaderSysEntryExternalIopMemoryList(rdram, &c, runtime); })) return;
    ckMid();
    uint64_t v0 = 0u;
    float f0 = 0.0f;
    uint64_t xo[2] = {Ps2GprU64(ctx, 5), Ps2GprU64(ctx, 6)};
    if (!ckLeg([&] { v0 = (ty::LoaderSysEntryExternalIopMemoryList(rdram, runtime, p4, xo)); })) return;
    ckEnd(4, ctx, c, 2, v0, f0, 0x68u, 0x0u, Ps2GprU64(&c, 5) != xo[0] || Ps2GprU64(&c, 6) != xo[1]);
}
void LoaderSysDeleteExternalIntcHandlerList(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint64_t p4)
{
    if (!ckBegin(5, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    if (!ckLeg([&] { ref::LoaderSysDeleteExternalIntcHandlerList(rdram, &c, runtime); })) return;
    ckMid();
    uint64_t v0 = 0u;
    float f0 = 0.0f;
    if (!ckLeg([&] { v0 = (ty::LoaderSysDeleteExternalIntcHandlerList(rdram, runtime, p4)); })) return;
    ckEnd(5, ctx, c, 2, v0, f0, 0x68u, 0x0u, false);
}
void LoaderSysDeleteExternalThreadList(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint64_t p4)
{
    if (!ckBegin(6, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    if (!ckLeg([&] { ref::LoaderSysDeleteExternalThreadList(rdram, &c, runtime); })) return;
    ckMid();
    uint64_t v0 = 0u;
    float f0 = 0.0f;
    uint64_t xo[2] = {Ps2GprU64(ctx, 5), Ps2GprU64(ctx, 6)};
    if (!ckLeg([&] { v0 = (ty::LoaderSysDeleteExternalThreadList(rdram, runtime, p4, xo)); })) return;
    ckEnd(6, ctx, c, 2, v0, f0, 0x68u, 0x0u, Ps2GprU64(&c, 5) != xo[0] || Ps2GprU64(&c, 6) != xo[1]);
}
void LoaderSysDeleteExternalSemaList(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint64_t p4)
{
    if (!ckBegin(7, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    if (!ckLeg([&] { ref::LoaderSysDeleteExternalSemaList(rdram, &c, runtime); })) return;
    ckMid();
    uint64_t v0 = 0u;
    float f0 = 0.0f;
    uint64_t xo[2] = {Ps2GprU64(ctx, 5), Ps2GprU64(ctx, 6)};
    if (!ckLeg([&] { v0 = (ty::LoaderSysDeleteExternalSemaList(rdram, runtime, p4, xo)); })) return;
    ckEnd(7, ctx, c, 2, v0, f0, 0x68u, 0x0u, Ps2GprU64(&c, 5) != xo[0] || Ps2GprU64(&c, 6) != xo[1]);
}
void LoaderSysDeleteExternalIopMemoryList(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint64_t p4)
{
    if (!ckBegin(8, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    if (!ckLeg([&] { ref::LoaderSysDeleteExternalIopMemoryList(rdram, &c, runtime); })) return;
    ckMid();
    uint64_t v0 = 0u;
    float f0 = 0.0f;
    uint64_t xo[4] = {Ps2GprU64(ctx, 5), Ps2GprU64(ctx, 6), Ps2GprU64(ctx, 7), Ps2GprU64(ctx, 8)};
    if (!ckLeg([&] { v0 = (ty::LoaderSysDeleteExternalIopMemoryList(rdram, runtime, p4, xo)); })) return;
    ckEnd(8, ctx, c, 2, v0, f0, 0x1e8u, 0x0u, Ps2GprU64(&c, 5) != xo[0] || Ps2GprU64(&c, 6) != xo[1] || Ps2GprU64(&c, 7) != xo[2] || Ps2GprU64(&c, 8) != xo[3]);
}
void LoaderSysInitExternalIntcHandlerList(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (!ckBegin(9, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    if (!ckLeg([&] { ref::LoaderSysInitExternalIntcHandlerList(rdram, &c, runtime); })) return;
    ckMid();
    uint64_t v0 = 0u;
    float f0 = 0.0f;
    if (!ckLeg([&] { v0 = a1_sx64(ty::LoaderSysInitExternalIntcHandlerList(rdram, runtime)); })) return;
    ckEnd(9, ctx, c, 1, v0, f0, 0x18u, 0x0u, false);
}
void LoaderSysInitExternalSemaList(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (!ckBegin(10, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    if (!ckLeg([&] { ref::LoaderSysInitExternalSemaList(rdram, &c, runtime); })) return;
    ckMid();
    uint64_t v0 = 0u;
    float f0 = 0.0f;
    if (!ckLeg([&] { v0 = a1_sx64(ty::LoaderSysInitExternalSemaList(rdram, runtime)); })) return;
    ckEnd(10, ctx, c, 1, v0, f0, 0x18u, 0x0u, false);
}
void LoaderSysInitExternalThreadList(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (!ckBegin(11, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    if (!ckLeg([&] { ref::LoaderSysInitExternalThreadList(rdram, &c, runtime); })) return;
    ckMid();
    uint64_t v0 = 0u;
    float f0 = 0.0f;
    if (!ckLeg([&] { v0 = a1_sx64(ty::LoaderSysInitExternalThreadList(rdram, runtime)); })) return;
    ckEnd(11, ctx, c, 1, v0, f0, 0x18u, 0x0u, false);
}
void LoaderSysInitExternalIopMemoryList(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (!ckBegin(12, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    if (!ckLeg([&] { ref::LoaderSysInitExternalIopMemoryList(rdram, &c, runtime); })) return;
    ckMid();
    uint64_t v0 = 0u;
    float f0 = 0.0f;
    if (!ckLeg([&] { v0 = a1_sx64(ty::LoaderSysInitExternalIopMemoryList(rdram, runtime)); })) return;
    ckEnd(12, ctx, c, 1, v0, f0, 0x8u, 0x0u, false);
}
void LoaderSysGetMemoryInfo(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint64_t p4, uint64_t p28)
{
    if (!ckBegin(13, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    if (!ckLeg([&] { ref::LoaderSysGetMemoryInfo(rdram, &c, runtime); })) return;
    ckMid();
    uint64_t v0 = 0u;
    float f0 = 0.0f;
    uint64_t xo[8] = {Ps2GprU64(ctx, 4), Ps2GprU64(ctx, 5), Ps2GprU64(ctx, 6), Ps2GprU64(ctx, 7), Ps2GprU64(ctx, 8), Ps2GprU64(ctx, 9), Ps2GprU64(ctx, 10), Ps2GprU64(ctx, 11)};
    if (!ckLeg([&] { v0 = (ty::LoaderSysGetMemoryInfo(rdram, runtime, p4, p28, xo)); })) return;
    ckEnd(13, ctx, c, 2, v0, f0, 0x1ff8u, 0x0u, Ps2GprU64(&c, 4) != xo[0] || Ps2GprU64(&c, 5) != xo[1] || Ps2GprU64(&c, 6) != xo[2] || Ps2GprU64(&c, 7) != xo[3] || Ps2GprU64(&c, 8) != xo[4] || Ps2GprU64(&c, 9) != xo[5] || Ps2GprU64(&c, 10) != xo[6] || Ps2GprU64(&c, 11) != xo[7]);
}
void ScrollDisplay(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint64_t p28)
{
    if (!ckBegin(14, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    if (!ckLeg([&] { ref::ScrollDisplay(rdram, &c, runtime); })) return;
    ckMid();
    uint64_t v0 = 0u;
    float f0 = 0.0f;
    if (!ckLeg([&] { v0 = (ty::ScrollDisplay(rdram, runtime, p28)); })) return;
    ckEnd(14, ctx, c, 2, v0, f0, 0xf8u, 0x0u, false);
}
void ClearDisplay(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint64_t p28)
{
    if (!ckBegin(15, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    if (!ckLeg([&] { ref::ClearDisplay(rdram, &c, runtime); })) return;
    ckMid();
    uint64_t v0 = 0u;
    float f0 = 0.0f;
    if (!ckLeg([&] { v0 = (ty::ClearDisplay(rdram, runtime, p28)); })) return;
    ckEnd(15, ctx, c, 2, v0, f0, 0x1f8u, 0x0u, false);
}
void sub_00109640(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint64_t p4, uint32_t p5)
{
    if (!ckBegin(16, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    if (!ckLeg([&] { ref::sub_00109640(rdram, &c, runtime); })) return;
    ckMid();
    uint64_t v0 = 0u;
    float f0 = 0.0f;
    if (!ckLeg([&] { v0 = a1_sx64(ty::sub_00109640(rdram, runtime, p4, p5)); })) return;
    ckEnd(16, ctx, c, 1, v0, f0, 0x28u, 0x0u, false);
}
void sub_0010F370(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t p4, uint32_t p5, uint32_t p6)
{
    if (!ckBegin(17, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    if (!ckLeg([&] { ref::sub_0010F370(rdram, &c, runtime); })) return;
    ckMid();
    uint64_t v0 = 0u;
    float f0 = 0.0f;
    if (!ckLeg([&] { v0 = a1_sx64(ty::sub_0010F370(rdram, runtime, p4, p5, p6)); })) return;
    ckEnd(17, ctx, c, 1, v0, f0, 0xf8u, 0x0u, false);
}
void sub_0010FC08(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t p4, uint32_t p5, uint32_t p6)
{
    if (!ckBegin(18, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    if (!ckLeg([&] { ref::sub_0010FC08(rdram, &c, runtime); })) return;
    ckMid();
    uint64_t v0 = 0u;
    float f0 = 0.0f;
    if (!ckLeg([&] { v0 = a1_sx64(ty::sub_0010FC08(rdram, runtime, p4, p5, p6)); })) return;
    ckEnd(18, ctx, c, 1, v0, f0, 0xf8u, 0x0u, false);
}
void sub_0010FC40(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint64_t p4, uint64_t p5, uint64_t p6)
{
    if (!ckBegin(19, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    if (!ckLeg([&] { ref::sub_0010FC40(rdram, &c, runtime); })) return;
    ckMid();
    uint64_t v0 = 0u;
    float f0 = 0.0f;
    if (!ckLeg([&] { v0 = (ty::sub_0010FC40(rdram, runtime, p4, p5, p6)); })) return;
    ckEnd(19, ctx, c, 2, v0, f0, 0x10u, 0x0u, false);
}
void sub_0010FE50(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t p4, uint32_t p5, uint32_t p6)
{
    if (!ckBegin(20, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    if (!ckLeg([&] { ref::sub_0010FE50(rdram, &c, runtime); })) return;
    ckMid();
    uint64_t v0 = 0u;
    float f0 = 0.0f;
    if (!ckLeg([&] { v0 = a1_sx64(ty::sub_0010FE50(rdram, runtime, p4, p5, p6)); })) return;
    ckEnd(20, ctx, c, 1, v0, f0, 0xf8u, 0x0u, false);
}
void sub_0010FFC0(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t p4, uint32_t p5, uint64_t p6)
{
    if (!ckBegin(21, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    if (!ckLeg([&] { ref::sub_0010FFC0(rdram, &c, runtime); })) return;
    ckMid();
    uint64_t v0 = 0u;
    float f0 = 0.0f;
    if (!ckLeg([&] { v0 = a1_sx64(ty::sub_0010FFC0(rdram, runtime, p4, p5, p6)); })) return;
    ckEnd(21, ctx, c, 1, v0, f0, 0xb8u, 0x0u, false);
}
void sub_00110270(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t p4, uint32_t p5, uint32_t p6)
{
    if (!ckBegin(22, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    if (!ckLeg([&] { ref::sub_00110270(rdram, &c, runtime); })) return;
    ckMid();
    uint64_t v0 = 0u;
    float f0 = 0.0f;
    if (!ckLeg([&] { v0 = a1_sx64(ty::sub_00110270(rdram, runtime, p4, p5, p6)); })) return;
    ckEnd(22, ctx, c, 1, v0, f0, 0xf8u, 0x0u, false);
}
void sub_00110910(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint64_t p4)
{
    if (!ckBegin(23, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    if (!ckLeg([&] { ref::sub_00110910(rdram, &c, runtime); })) return;
    ckMid();
    uint64_t v0 = 0u;
    float f0 = 0.0f;
    uint64_t xo[6] = {Ps2GprU64(ctx, 4), Ps2GprU64(ctx, 5), Ps2GprU64(ctx, 6), Ps2GprU64(ctx, 7), Ps2GprU64(ctx, 8), Ps2GprU64(ctx, 9)};
    if (!ckLeg([&] { v0 = (ty::sub_00110910(rdram, runtime, p4, xo)); })) return;
    ckEnd(23, ctx, c, 2, v0, f0, 0x3f8u, 0x0u, Ps2GprU64(&c, 4) != xo[0] || Ps2GprU64(&c, 5) != xo[1] || Ps2GprU64(&c, 6) != xo[2] || Ps2GprU64(&c, 7) != xo[3] || Ps2GprU64(&c, 8) != xo[4] || Ps2GprU64(&c, 9) != xo[5]);
}
void iGetTimerSystemTime(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (!ckBegin(24, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    if (!ckLeg([&] { ref::iGetTimerSystemTime(rdram, &c, runtime); })) return;
    ckMid();
    uint64_t v0 = 0u;
    float f0 = 0.0f;
    uint64_t xo[4] = {Ps2GprU64(ctx, 4), Ps2GprU64(ctx, 8), Ps2GprU64(ctx, 9), Ps2GprU64(ctx, 10)};
    if (!ckLeg([&] { v0 = (ty::iGetTimerSystemTime(rdram, runtime, xo)); })) return;
    ckEnd(24, ctx, c, 2, v0, f0, 0x718u, 0x0u, Ps2GprU64(&c, 4) != xo[0] || Ps2GprU64(&c, 8) != xo[1] || Ps2GprU64(&c, 9) != xo[2] || Ps2GprU64(&c, 10) != xo[3]);
}
void iAllocTimerCounter(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (!ckBegin(25, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    if (!ckLeg([&] { ref::iAllocTimerCounter(rdram, &c, runtime); })) return;
    ckMid();
    uint64_t v0 = 0u;
    float f0 = 0.0f;
    uint64_t xo[4] = {Ps2GprU64(ctx, 4), Ps2GprU64(ctx, 5), Ps2GprU64(ctx, 6), Ps2GprU64(ctx, 7)};
    if (!ckLeg([&] { v0 = a1_sx64(ty::iAllocTimerCounter(rdram, runtime, xo)); })) return;
    ckEnd(25, ctx, c, 1, v0, f0, 0xf8u, 0x0u, Ps2GprU64(&c, 4) != xo[0] || Ps2GprU64(&c, 5) != xo[1] || Ps2GprU64(&c, 6) != xo[2] || Ps2GprU64(&c, 7) != xo[3]);
}
void iGetTimerBaseTime(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint64_t p4)
{
    if (!ckBegin(26, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    if (!ckLeg([&] { ref::iGetTimerBaseTime(rdram, &c, runtime); })) return;
    ckMid();
    uint64_t v0 = 0u;
    float f0 = 0.0f;
    uint64_t xo[1] = {Ps2GprU64(ctx, 5)};
    if (!ckLeg([&] { v0 = (ty::iGetTimerBaseTime(rdram, runtime, p4, xo)); })) return;
    ckEnd(26, ctx, c, 2, v0, f0, 0x28u, 0x0u, Ps2GprU64(&c, 5) != xo[0]);
}
void ps2__InitAlarm(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (!ckBegin(27, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    if (!ckLeg([&] { ref::ps2__InitAlarm(rdram, &c, runtime); })) return;
    ckMid();
    uint64_t v0 = 0u;
    float f0 = 0.0f;
    if (!ckLeg([&] { v0 = a1_sx64(ty::ps2__InitAlarm(rdram, runtime)); })) return;
    ckEnd(27, ctx, c, 1, v0, f0, 0x38u, 0x0u, false);
}
void sceVif1PkAddGsData(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint64_t p4, uint64_t p5)
{
    if (!ckBegin(28, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    if (!ckLeg([&] { ref::sceVif1PkAddGsData(rdram, &c, runtime); })) return;
    ckMid();
    uint64_t v0 = 0u;
    float f0 = 0.0f;
    uint64_t xo[1] = {Ps2GprU64(ctx, 5)};
    if (!ckLeg([&] { v0 = (ty::sceVif1PkAddGsData(rdram, runtime, p4, p5, xo)); })) return;
    ckEnd(28, ctx, c, 2, v0, f0, 0x28u, 0x0u, Ps2GprU64(&c, 5) != xo[0]);
}
void sceGsSetDefAlphaEnv(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint64_t p4, uint64_t p5)
{
    if (!ckBegin(29, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    if (!ckLeg([&] { ref::sceGsSetDefAlphaEnv(rdram, &c, runtime); })) return;
    ckMid();
    uint64_t v0 = 0u;
    float f0 = 0.0f;
    uint64_t xo[4] = {Ps2GprU64(ctx, 6), Ps2GprU64(ctx, 7), Ps2GprU64(ctx, 8), Ps2GprU64(ctx, 9)};
    if (!ckLeg([&] { v0 = (ty::sceGsSetDefAlphaEnv(rdram, runtime, p4, p5, xo)); })) return;
    ckEnd(29, ctx, c, 2, v0, f0, 0x3e8u, 0x0u, Ps2GprU64(&c, 6) != xo[0] || Ps2GprU64(&c, 7) != xo[1] || Ps2GprU64(&c, 8) != xo[2] || Ps2GprU64(&c, 9) != xo[3]);
}
void sceGsSetHalfOffset(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint64_t p4, uint64_t p5, uint64_t p6, uint64_t p7)
{
    if (!ckBegin(30, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    if (!ckLeg([&] { ref::sceGsSetHalfOffset(rdram, &c, runtime); })) return;
    ckMid();
    uint64_t v0 = 0u;
    float f0 = 0.0f;
    uint64_t xo[3] = {Ps2GprU64(ctx, 5), Ps2GprU64(ctx, 6), Ps2GprU64(ctx, 7)};
    if (!ckLeg([&] { v0 = (ty::sceGsSetHalfOffset(rdram, runtime, p4, p5, p6, p7, xo)); })) return;
    ckEnd(30, ctx, c, 2, v0, f0, 0xe8u, 0x0u, Ps2GprU64(&c, 5) != xo[0] || Ps2GprU64(&c, 6) != xo[1] || Ps2GprU64(&c, 7) != xo[2]);
}
void sub_001157D8(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t p4, uint32_t p5)
{
    if (!ckBegin(31, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    if (!ckLeg([&] { ref::sub_001157D8(rdram, &c, runtime); })) return;
    ckMid();
    uint64_t v0 = 0u;
    float f0 = 0.0f;
    if (!ckLeg([&] { v0 = (ty::sub_001157D8(rdram, runtime, p4, p5)); })) return;
    ckEnd(31, ctx, c, 2, v0, f0, 0x7f8u, 0x0u, false);
}
void sub_0011BAE8(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint64_t p4, uint32_t p5)
{
    if (!ckBegin(32, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    if (!ckLeg([&] { ref::sub_0011BAE8(rdram, &c, runtime); })) return;
    ckMid();
    uint64_t v0 = 0u;
    float f0 = 0.0f;
    uint64_t xo[1] = {Ps2GprU64(ctx, 6)};
    if (!ckLeg([&] { v0 = a1_sx64(ty::sub_0011BAE8(rdram, runtime, p4, p5, xo)); })) return;
    ckEnd(32, ctx, c, 1, v0, f0, 0x68u, 0x0u, Ps2GprU64(&c, 6) != xo[0]);
}
void sub_0011BBB8(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t p4)
{
    if (!ckBegin(33, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    if (!ckLeg([&] { ref::sub_0011BBB8(rdram, &c, runtime); })) return;
    ckMid();
    uint64_t v0 = 0u;
    float f0 = 0.0f;
    uint64_t xo[1] = {Ps2GprU64(ctx, 6)};
    if (!ckLeg([&] { v0 = a1_sx64(ty::sub_0011BBB8(rdram, runtime, p4, xo)); })) return;
    ckEnd(33, ctx, c, 1, v0, f0, 0x78u, 0x0u, Ps2GprU64(&c, 6) != xo[0]);
}
void sub_0011BF68(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint64_t p4)
{
    if (!ckBegin(34, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    if (!ckLeg([&] { ref::sub_0011BF68(rdram, &c, runtime); })) return;
    ckMid();
    uint64_t v0 = 0u;
    float f0 = 0.0f;
    if (!ckLeg([&] { v0 = (ty::sub_0011BF68(rdram, runtime, p4)); })) return;
    ckEnd(34, ctx, c, 2, v0, f0, 0x78u, 0x0u, false);
}
void _sceMcFatInit(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (!ckBegin(35, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    if (!ckLeg([&] { ref::_sceMcFatInit(rdram, &c, runtime); })) return;
    ckMid();
    uint64_t v0 = 0u;
    float f0 = 0.0f;
    uint64_t xo[4] = {Ps2GprU64(ctx, 6), Ps2GprU64(ctx, 7), Ps2GprU64(ctx, 8), Ps2GprU64(ctx, 10)};
    if (!ckLeg([&] { v0 = a1_sx64(ty::_sceMcFatInit(rdram, runtime, xo)); })) return;
    ckEnd(35, ctx, c, 1, v0, f0, 0x5f8u, 0x0u, Ps2GprU64(&c, 6) != xo[0] || Ps2GprU64(&c, 7) != xo[1] || Ps2GprU64(&c, 8) != xo[2] || Ps2GprU64(&c, 10) != xo[3]);
}
void _sceMcFatInit2(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t p4)
{
    if (!ckBegin(36, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    if (!ckLeg([&] { ref::_sceMcFatInit2(rdram, &c, runtime); })) return;
    ckMid();
    uint64_t v0 = 0u;
    float f0 = 0.0f;
    uint64_t xo[7] = {Ps2GprU64(ctx, 5), Ps2GprU64(ctx, 6), Ps2GprU64(ctx, 7), Ps2GprU64(ctx, 8), Ps2GprU64(ctx, 9), Ps2GprU64(ctx, 10), Ps2GprU64(ctx, 11)};
    if (!ckLeg([&] { v0 = a1_sx64(ty::_sceMcFatInit2(rdram, runtime, p4, xo)); })) return;
    ckEnd(36, ctx, c, 1, v0, f0, 0xff8u, 0x0u, Ps2GprU64(&c, 5) != xo[0] || Ps2GprU64(&c, 6) != xo[1] || Ps2GprU64(&c, 7) != xo[2] || Ps2GprU64(&c, 8) != xo[3] || Ps2GprU64(&c, 9) != xo[4] || Ps2GprU64(&c, 10) != xo[5] || Ps2GprU64(&c, 11) != xo[6]);
}
void _sceMcpInit(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (!ckBegin(37, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    if (!ckLeg([&] { ref::_sceMcpInit(rdram, &c, runtime); })) return;
    ckMid();
    uint64_t v0 = 0u;
    float f0 = 0.0f;
    if (!ckLeg([&] { v0 = a1_sx64(ty::_sceMcpInit(rdram, runtime)); })) return;
    ckEnd(37, ctx, c, 1, v0, f0, 0x38u, 0x0u, false);
}
void _sceMcpIsSpeckOutBlock(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t p4, uint64_t p5)
{
    if (!ckBegin(38, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    if (!ckLeg([&] { ref::_sceMcpIsSpeckOutBlock(rdram, &c, runtime); })) return;
    ckMid();
    uint64_t v0 = 0u;
    float f0 = 0.0f;
    if (!ckLeg([&] { v0 = a1_sx64(ty::_sceMcpIsSpeckOutBlock(rdram, runtime, p4, p5)); })) return;
    ckEnd(38, ctx, c, 1, v0, f0, 0x18u, 0x0u, false);
}
void sub_00121EC0(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (!ckBegin(39, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    if (!ckLeg([&] { ref::sub_00121EC0(rdram, &c, runtime); })) return;
    ckMid();
    uint64_t v0 = 0u;
    float f0 = 0.0f;
    if (!ckLeg([&] { v0 = (ty::sub_00121EC0(rdram, runtime)); })) return;
    ckEnd(39, ctx, c, 2, v0, f0, 0x18u, 0x0u, false);
}
void sceNetcnfifGetResult(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint64_t p4)
{
    if (!ckBegin(40, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    if (!ckLeg([&] { ref::sceNetcnfifGetResult(rdram, &c, runtime); })) return;
    ckMid();
    uint64_t v0 = 0u;
    float f0 = 0.0f;
    uint64_t xo[4] = {Ps2GprU64(ctx, 5), Ps2GprU64(ctx, 6), Ps2GprU64(ctx, 7), Ps2GprU64(ctx, 8)};
    if (!ckLeg([&] { v0 = (ty::sceNetcnfifGetResult(rdram, runtime, p4, xo)); })) return;
    ckEnd(40, ctx, c, 2, v0, f0, 0x1f8u, 0x0u, Ps2GprU64(&c, 5) != xo[0] || Ps2GprU64(&c, 6) != xo[1] || Ps2GprU64(&c, 7) != xo[2] || Ps2GprU64(&c, 8) != xo[3]);
}
void sub_00125080(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint64_t p4, uint64_t p5, uint64_t p6)
{
    if (!ckBegin(41, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    if (!ckLeg([&] { ref::sub_00125080(rdram, &c, runtime); })) return;
    ckMid();
    uint64_t v0 = 0u;
    float f0 = 0.0f;
    if (!ckLeg([&] { v0 = (ty::sub_00125080(rdram, runtime, p4, p5, p6)); })) return;
    ckEnd(41, ctx, c, 2, v0, f0, 0x1fb8u, 0x0u, false);
}
void sub_00125998(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t p4, uint32_t p5)
{
    if (!ckBegin(42, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    if (!ckLeg([&] { ref::sub_00125998(rdram, &c, runtime); })) return;
    ckMid();
    uint64_t v0 = 0u;
    float f0 = 0.0f;
    uint64_t xo[2] = {Ps2GprU64(ctx, 6), Ps2GprU64(ctx, 7)};
    if (!ckLeg([&] { v0 = a1_sx64(ty::sub_00125998(rdram, runtime, p4, p5, xo)); })) return;
    ckEnd(42, ctx, c, 1, v0, f0, 0xd8u, 0x0u, Ps2GprU64(&c, 6) != xo[0] || Ps2GprU64(&c, 7) != xo[1]);
}
void sub_001259E0(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t p4, uint32_t p5)
{
    if (!ckBegin(43, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    if (!ckLeg([&] { ref::sub_001259E0(rdram, &c, runtime); })) return;
    ckMid();
    uint64_t v0 = 0u;
    float f0 = 0.0f;
    if (!ckLeg([&] { v0 = a1_sx64(ty::sub_001259E0(rdram, runtime, p4, p5)); })) return;
    ckEnd(43, ctx, c, 1, v0, f0, 0x1d8u, 0x0u, false);
}
void sub_00125A38(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint64_t p4, uint64_t p5)
{
    if (!ckBegin(44, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    if (!ckLeg([&] { ref::sub_00125A38(rdram, &c, runtime); })) return;
    ckMid();
    uint64_t v0 = 0u;
    float f0 = 0.0f;
    uint64_t xo[7] = {Ps2GprU64(ctx, 5), Ps2GprU64(ctx, 6), Ps2GprU64(ctx, 7), Ps2GprU64(ctx, 8), Ps2GprU64(ctx, 9), Ps2GprU64(ctx, 10), Ps2GprU64(ctx, 11)};
    if (!ckLeg([&] { v0 = (ty::sub_00125A38(rdram, runtime, p4, p5, xo)); })) return;
    ckEnd(44, ctx, c, 2, v0, f0, 0x300fff8u, 0x0u, Ps2GprU64(&c, 5) != xo[0] || Ps2GprU64(&c, 6) != xo[1] || Ps2GprU64(&c, 7) != xo[2] || Ps2GprU64(&c, 8) != xo[3] || Ps2GprU64(&c, 9) != xo[4] || Ps2GprU64(&c, 10) != xo[5] || Ps2GprU64(&c, 11) != xo[6]);
}
void sub_001262B8(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t p4)
{
    if (!ckBegin(45, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    if (!ckLeg([&] { ref::sub_001262B8(rdram, &c, runtime); })) return;
    ckMid();
    uint64_t v0 = 0u;
    float f0 = 0.0f;
    uint64_t xo[2] = {Ps2GprU64(ctx, 5), Ps2GprU64(ctx, 6)};
    if (!ckLeg([&] { v0 = (ty::sub_001262B8(rdram, runtime, p4, xo)); })) return;
    ckEnd(45, ctx, c, 2, v0, f0, 0x78u, 0x0u, Ps2GprU64(&c, 5) != xo[0] || Ps2GprU64(&c, 6) != xo[1]);
}
void sub_00126388(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t p4, uint32_t p5)
{
    if (!ckBegin(46, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    if (!ckLeg([&] { ref::sub_00126388(rdram, &c, runtime); })) return;
    ckMid();
    uint64_t v0 = 0u;
    float f0 = 0.0f;
    uint64_t xo[2] = {Ps2GprU64(ctx, 6), Ps2GprU64(ctx, 7)};
    if (!ckLeg([&] { v0 = (ty::sub_00126388(rdram, runtime, p4, p5, xo)); })) return;
    ckEnd(46, ctx, c, 2, v0, f0, 0xd8u, 0x0u, Ps2GprU64(&c, 6) != xo[0] || Ps2GprU64(&c, 7) != xo[1]);
}
void ps2___unpack_d(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint64_t p4, uint64_t p5)
{
    if (!ckBegin(47, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    if (!ckLeg([&] { ref::ps2___unpack_d(rdram, &c, runtime); })) return;
    ckMid();
    uint64_t v0 = 0u;
    float f0 = 0.0f;
    uint64_t xo[2] = {Ps2GprU64(ctx, 6), Ps2GprU64(ctx, 7)};
    if (!ckLeg([&] { v0 = (ty::ps2___unpack_d(rdram, runtime, p4, p5, xo)); })) return;
    ckEnd(47, ctx, c, 2, v0, f0, 0xf8u, 0x0u, Ps2GprU64(&c, 6) != xo[0] || Ps2GprU64(&c, 7) != xo[1]);
}
void ps2__Bfree(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t p4, uint64_t p5)
{
    if (!ckBegin(48, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    if (!ckLeg([&] { ref::ps2__Bfree(rdram, &c, runtime); })) return;
    ckMid();
    uint64_t v0 = 0u;
    float f0 = 0.0f;
    uint64_t xo[1] = {Ps2GprU64(ctx, 2)};
    if (!ckLeg([&] { ty::ps2__Bfree(rdram, runtime, p4, p5, xo); })) return;
    ckEnd(48, ctx, c, 0, v0, f0, 0x1cu, 0x0u, Ps2GprU64(&c, 2) != xo[0]);
}
void ps2___mcmp(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t p4, uint32_t p5)
{
    if (!ckBegin(49, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    if (!ckLeg([&] { ref::ps2___mcmp(rdram, &c, runtime); })) return;
    ckMid();
    uint64_t v0 = 0u;
    float f0 = 0.0f;
    uint64_t xo[5] = {Ps2GprU64(ctx, 6), Ps2GprU64(ctx, 7), Ps2GprU64(ctx, 8), Ps2GprU64(ctx, 9), Ps2GprU64(ctx, 10)};
    if (!ckLeg([&] { v0 = a1_sx64(ty::ps2___mcmp(rdram, runtime, p4, p5, xo)); })) return;
    ckEnd(49, ctx, c, 1, v0, f0, 0x7f8u, 0x0u, Ps2GprU64(&c, 6) != xo[0] || Ps2GprU64(&c, 7) != xo[1] || Ps2GprU64(&c, 8) != xo[2] || Ps2GprU64(&c, 9) != xo[3] || Ps2GprU64(&c, 10) != xo[4]);
}
void _ulp(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint64_t p4, uint64_t p5)
{
    if (!ckBegin(50, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    if (!ckLeg([&] { ref::_ulp(rdram, &c, runtime); })) return;
    ckMid();
    uint64_t v0 = 0u;
    float f0 = 0.0f;
    uint64_t xo[1] = {Ps2GprU64(ctx, 6)};
    if (!ckLeg([&] { v0 = (ty::_ulp(rdram, runtime, p4, p5, xo)); })) return;
    ckEnd(50, ctx, c, 2, v0, f0, 0x78u, 0x0u, Ps2GprU64(&c, 6) != xo[0]);
}
void iosCdStStart(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint64_t p4)
{
    if (!ckBegin(51, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    if (!ckLeg([&] { ref::iosCdStStart(rdram, &c, runtime); })) return;
    ckMid();
    uint64_t v0 = 0u;
    float f0 = 0.0f;
    uint64_t xo[2] = {Ps2GprU64(ctx, 6), Ps2GprU64(ctx, 7)};
    if (!ckLeg([&] { v0 = (ty::iosCdStStart(rdram, runtime, p4, xo)); })) return;
    ckEnd(51, ctx, c, 2, v0, f0, 0xf8u, 0x0u, Ps2GprU64(&c, 6) != xo[0] || Ps2GprU64(&c, 7) != xo[1]);
}
void iosGetTCountExtension(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (!ckBegin(52, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    if (!ckLeg([&] { ref::iosGetTCountExtension(rdram, &c, runtime); })) return;
    ckMid();
    uint64_t v0 = 0u;
    float f0 = 0.0f;
    uint64_t xo[1] = {Ps2GprU64(ctx, 4)};
    if (!ckLeg([&] { v0 = (ty::iosGetTCountExtension(rdram, runtime, xo)); })) return;
    ckEnd(52, ctx, c, 2, v0, f0, 0x18u, 0x0u, Ps2GprU64(&c, 4) != xo[0]);
}
void iosResetCpuRapCounter(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint64_t p4)
{
    if (!ckBegin(53, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    if (!ckLeg([&] { ref::iosResetCpuRapCounter(rdram, &c, runtime); })) return;
    ckMid();
    uint64_t v0 = 0u;
    float f0 = 0.0f;
    uint64_t xo[2] = {Ps2GprU64(ctx, 5), Ps2GprU64(ctx, 6)};
    if (!ckLeg([&] { v0 = a1_sx64(ty::iosResetCpuRapCounter(rdram, runtime, p4, xo)); })) return;
    ckEnd(53, ctx, c, 1, v0, f0, 0x68u, 0x0u, Ps2GprU64(&c, 5) != xo[0] || Ps2GprU64(&c, 6) != xo[1]);
}
void iosPadRead(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint64_t p4, uint64_t p5)
{
    if (!ckBegin(54, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    if (!ckLeg([&] { ref::iosPadRead(rdram, &c, runtime); })) return;
    ckMid();
    uint64_t v0 = 0u;
    float f0 = 0.0f;
    uint64_t xo[4] = {Ps2GprU64(ctx, 5), Ps2GprU64(ctx, 6), Ps2GprU64(ctx, 7), Ps2GprU64(ctx, 8)};
    if (!ckLeg([&] { v0 = (ty::iosPadRead(rdram, runtime, p4, p5, xo)); })) return;
    ckEnd(54, ctx, c, 2, v0, f0, 0x1fau, 0x0u, Ps2GprU64(&c, 5) != xo[0] || Ps2GprU64(&c, 6) != xo[1] || Ps2GprU64(&c, 7) != xo[2] || Ps2GprU64(&c, 8) != xo[3]);
}
void iosMemToSprDmaAdrCopy(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t p4, uint32_t p5, uint32_t p6)
{
    if (!ckBegin(55, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    if (!ckLeg([&] { ref::iosMemToSprDmaAdrCopy(rdram, &c, runtime); })) return;
    ckMid();
    uint64_t v0 = 0u;
    float f0 = 0.0f;
    uint64_t xo[5] = {Ps2GprU64(ctx, 7), Ps2GprU64(ctx, 8), Ps2GprU64(ctx, 9), Ps2GprU64(ctx, 10), Ps2GprU64(ctx, 11)};
    if (!ckLeg([&] { v0 = a1_sx64(ty::iosMemToSprDmaAdrCopy(rdram, runtime, p4, p5, p6, xo)); })) return;
    ckEnd(55, ctx, c, 1, v0, f0, 0xf98u, 0x0u, Ps2GprU64(&c, 7) != xo[0] || Ps2GprU64(&c, 8) != xo[1] || Ps2GprU64(&c, 9) != xo[2] || Ps2GprU64(&c, 10) != xo[3] || Ps2GprU64(&c, 11) != xo[4]);
}
void Init_Shock(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (!ckBegin(56, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    if (!ckLeg([&] { ref::Init_Shock(rdram, &c, runtime); })) return;
    ckMid();
    uint64_t v0 = 0u;
    float f0 = 0.0f;
    if (!ckLeg([&] { v0 = a1_sx64(ty::Init_Shock(rdram, runtime)); })) return;
    ckEnd(56, ctx, c, 1, v0, f0, 0x1f8u, 0x0u, false);
}
void kernel_sub_001B40D0(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint64_t p4)
{
    if (!ckBegin(57, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    if (!ckLeg([&] { ref::kernel_sub_001B40D0(rdram, &c, runtime); })) return;
    ckMid();
    uint64_t v0 = 0u;
    float f0 = 0.0f;
    if (!ckLeg([&] { v0 = a1_sx64(ty::kernel_sub_001B40D0(rdram, runtime, p4)); })) return;
    ckEnd(57, ctx, c, 1, v0, f0, 0x78u, 0x0u, false);
}
void iosLoaderGetFileSize(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint64_t p4)
{
    if (!ckBegin(58, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    if (!ckLeg([&] { ref::iosLoaderGetFileSize(rdram, &c, runtime); })) return;
    ckMid();
    uint64_t v0 = 0u;
    float f0 = 0.0f;
    uint64_t xo[1] = {Ps2GprU64(ctx, 7)};
    if (!ckLeg([&] { v0 = a1_sx64(ty::iosLoaderGetFileSize(rdram, runtime, p4, xo)); })) return;
    ckEnd(58, ctx, c, 1, v0, f0, 0xe8u, 0x0u, Ps2GprU64(&c, 7) != xo[0]);
}
void initDldSys(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (!ckBegin(59, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    if (!ckLeg([&] { ref::initDldSys(rdram, &c, runtime); })) return;
    ckMid();
    uint64_t v0 = 0u;
    float f0 = 0.0f;
    uint64_t xo[1] = {Ps2GprU64(ctx, 4)};
    if (!ckLeg([&] { v0 = a1_sx64(ty::initDldSys(rdram, runtime, xo)); })) return;
    ckEnd(59, ctx, c, 1, v0, f0, 0x18u, 0x0u, Ps2GprU64(&c, 4) != xo[0]);
}
void kernel_sub_001BA690(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint64_t p4, uint64_t p5)
{
    if (!ckBegin(60, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    if (!ckLeg([&] { ref::kernel_sub_001BA690(rdram, &c, runtime); })) return;
    ckMid();
    uint64_t v0 = 0u;
    float f0 = 0.0f;
    uint64_t xo[1] = {Ps2GprU64(ctx, 2)};
    if (!ckLeg([&] { f0 = ty::kernel_sub_001BA690(rdram, runtime, p4, p5, xo); })) return;
    ckEnd(60, ctx, c, 3, v0, f0, 0x1ffeu, 0x3eu, Ps2GprU64(&c, 2) != xo[0]);
}
void iosGetTableCos(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t p4)
{
    if (!ckBegin(61, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    if (!ckLeg([&] { ref::iosGetTableCos(rdram, &c, runtime); })) return;
    ckMid();
    uint64_t v0 = 0u;
    float f0 = 0.0f;
    uint64_t xo[1] = {Ps2GprU64(ctx, 4)};
    if (!ckLeg([&] { const a1::RetVF r = ty::iosGetTableCos(rdram, runtime, p4, xo); v0 = r.v0; f0 = r.f0; })) return;
    ckEnd(61, ctx, c, 4, v0, f0, 0x1au, 0x0u, Ps2GprU64(&c, 4) != xo[0]);
}
void iosSetDCViewPortCenter(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint64_t p4, uint64_t p5, uint64_t p6, uint64_t p7, uint64_t p8, uint64_t p9, uint64_t p10)
{
    if (!ckBegin(62, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    if (!ckLeg([&] { ref::iosSetDCViewPortCenter(rdram, &c, runtime); })) return;
    ckMid();
    uint64_t v0 = 0u;
    float f0 = 0.0f;
    uint64_t xo[3] = {Ps2GprU64(ctx, 9), Ps2GprU64(ctx, 10), Ps2GprU64(ctx, 11)};
    if (!ckLeg([&] { v0 = (ty::iosSetDCViewPortCenter(rdram, runtime, p4, p5, p6, p7, p8, p9, p10, xo)); })) return;
    ckEnd(62, ctx, c, 2, v0, f0, 0x1e08u, 0x0u, Ps2GprU64(&c, 9) != xo[0] || Ps2GprU64(&c, 10) != xo[1] || Ps2GprU64(&c, 11) != xo[2]);
}
void iosUsedPacketBufferSize(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (!ckBegin(63, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    if (!ckLeg([&] { ref::iosUsedPacketBufferSize(rdram, &c, runtime); })) return;
    ckMid();
    uint64_t v0 = 0u;
    float f0 = 0.0f;
    uint64_t xo[2] = {Ps2GprU64(ctx, 4), Ps2GprU64(ctx, 5)};
    if (!ckLeg([&] { const a1::RetVF r = ty::iosUsedPacketBufferSize(rdram, runtime, xo); v0 = r.v0; f0 = r.f0; })) return;
    ckEnd(63, ctx, c, 4, v0, f0, 0x38u, 0x2u, Ps2GprU64(&c, 4) != xo[0] || Ps2GprU64(&c, 5) != xo[1]);
}
void iosAllocPacketPtrPath1(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t p4)
{
    if (!ckBegin(64, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    if (!ckLeg([&] { ref::iosAllocPacketPtrPath1(rdram, &c, runtime); })) return;
    ckMid();
    uint64_t v0 = 0u;
    float f0 = 0.0f;
    uint64_t xo[1] = {Ps2GprU64(ctx, 5)};
    if (!ckLeg([&] { v0 = a1_sx64(ty::iosAllocPacketPtrPath1(rdram, runtime, p4, xo)); })) return;
    ckEnd(64, ctx, c, 1, v0, f0, 0x28u, 0x0u, Ps2GprU64(&c, 5) != xo[0]);
}
void iosDlChainTail(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint64_t p4, uint64_t p5)
{
    if (!ckBegin(65, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    if (!ckLeg([&] { ref::iosDlChainTail(rdram, &c, runtime); })) return;
    ckMid();
    uint64_t v0 = 0u;
    float f0 = 0.0f;
    uint64_t xo[4] = {Ps2GprU64(ctx, 6), Ps2GprU64(ctx, 7), Ps2GprU64(ctx, 8), Ps2GprU64(ctx, 9)};
    if (!ckLeg([&] { v0 = (ty::iosDlChainTail(rdram, runtime, p4, p5, xo)); })) return;
    ckEnd(65, ctx, c, 2, v0, f0, 0x3f8u, 0x0u, Ps2GprU64(&c, 6) != xo[0] || Ps2GprU64(&c, 7) != xo[1] || Ps2GprU64(&c, 8) != xo[2] || Ps2GprU64(&c, 9) != xo[3]);
}
void kernel_sub_001C1538(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (!ckBegin(66, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    if (!ckLeg([&] { ref::kernel_sub_001C1538(rdram, &c, runtime); })) return;
    ckMid();
    uint64_t v0 = 0u;
    float f0 = 0.0f;
    uint64_t xo[6] = {Ps2GprU64(ctx, 10), Ps2GprU64(ctx, 11), Ps2GprU64(ctx, 12), Ps2GprU64(ctx, 13), Ps2GprU64(ctx, 14), Ps2GprU64(ctx, 15)};
    if (!ckLeg([&] { v0 = (ty::kernel_sub_001C1538(rdram, runtime, xo)); })) return;
    ckEnd(66, ctx, c, 2, v0, f0, 0xfff8u, 0x0u, Ps2GprU64(&c, 10) != xo[0] || Ps2GprU64(&c, 11) != xo[1] || Ps2GprU64(&c, 12) != xo[2] || Ps2GprU64(&c, 13) != xo[3] || Ps2GprU64(&c, 14) != xo[4] || Ps2GprU64(&c, 15) != xo[5]);
}
void iosGsSetMode(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (!ckBegin(67, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    if (!ckLeg([&] { ref::iosGsSetMode(rdram, &c, runtime); })) return;
    ckMid();
    uint64_t v0 = 0u;
    float f0 = 0.0f;
    uint64_t xo[4] = {Ps2GprU64(ctx, 8), Ps2GprU64(ctx, 9), Ps2GprU64(ctx, 10), Ps2GprU64(ctx, 11)};
    if (!ckLeg([&] { v0 = a1_sx64(ty::iosGsSetMode(rdram, runtime, xo)); })) return;
    ckEnd(67, ctx, c, 1, v0, f0, 0xff8u, 0x0u, Ps2GprU64(&c, 8) != xo[0] || Ps2GprU64(&c, 9) != xo[1] || Ps2GprU64(&c, 10) != xo[2] || Ps2GprU64(&c, 11) != xo[3]);
}
void iosChkGpDlTransExec(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (!ckBegin(68, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    if (!ckLeg([&] { ref::iosChkGpDlTransExec(rdram, &c, runtime); })) return;
    ckMid();
    uint64_t v0 = 0u;
    float f0 = 0.0f;
    uint64_t xo[1] = {Ps2GprU64(ctx, 4)};
    if (!ckLeg([&] { v0 = (ty::iosChkGpDlTransExec(rdram, runtime, xo)); })) return;
    ckEnd(68, ctx, c, 2, v0, f0, 0x18u, 0x0u, Ps2GprU64(&c, 4) != xo[0]);
}
void getTWH(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint64_t p4)
{
    if (!ckBegin(69, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    if (!ckLeg([&] { ref::getTWH(rdram, &c, runtime); })) return;
    ckMid();
    uint64_t v0 = 0u;
    float f0 = 0.0f;
    if (!ckLeg([&] { v0 = (ty::getTWH(rdram, runtime, p4)); })) return;
    ckEnd(69, ctx, c, 2, v0, f0, 0x28u, 0x0u, false);
}
void isysGetLinkNum(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (!ckBegin(70, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    if (!ckLeg([&] { ref::isysGetLinkNum(rdram, &c, runtime); })) return;
    ckMid();
    uint64_t v0 = 0u;
    float f0 = 0.0f;
    if (!ckLeg([&] { v0 = a1_sx64(ty::isysGetLinkNum(rdram, runtime)); })) return;
    ckEnd(70, ctx, c, 1, v0, f0, 0x38u, 0x0u, false);
}
void getVIntrDiffTime(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t p4)
{
    if (!ckBegin(71, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    if (!ckLeg([&] { ref::getVIntrDiffTime(rdram, &c, runtime); })) return;
    ckMid();
    uint64_t v0 = 0u;
    float f0 = 0.0f;
    uint64_t xo[1] = {Ps2GprU64(ctx, 5)};
    if (!ckLeg([&] { const a1::RetVF r = ty::getVIntrDiffTime(rdram, runtime, p4, xo); v0 = r.v0; f0 = r.f0; })) return;
    ckEnd(71, ctx, c, 4, v0, f0, 0x2au, 0x3eu, Ps2GprU64(&c, 5) != xo[0]);
}
void addVIntrTime(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t p4, float pf12)
{
    if (!ckBegin(72, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    if (!ckLeg([&] { ref::addVIntrTime(rdram, &c, runtime); })) return;
    ckMid();
    uint64_t v0 = 0u;
    float f0 = 0.0f;
    uint64_t xo[2] = {(uint64_t)a1ck::ckBits(ctx->f[12]), Ps2GprU64(ctx, 5)};
    if (!ckLeg([&] { const a1::RetVF r = ty::addVIntrTime(rdram, runtime, p4, pf12, xo); v0 = r.v0; f0 = r.f0; })) return;
    ckEnd(72, ctx, c, 4, v0, f0, 0x2au, 0x101eu, a1ck::ckBits(c.f[12]) != (uint32_t)xo[0] || Ps2GprU64(&c, 5) != xo[1]);
}
void ischedStop(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (!ckBegin(73, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    if (!ckLeg([&] { ref::ischedStop(rdram, &c, runtime); })) return;
    ckMid();
    uint64_t v0 = 0u;
    float f0 = 0.0f;
    uint64_t xo[1] = {Ps2GprU64(ctx, 5)};
    if (!ckLeg([&] { v0 = a1_sx64(ty::ischedStop(rdram, runtime, xo)); })) return;
    ckEnd(73, ctx, c, 1, v0, f0, 0x38u, 0x0u, Ps2GprU64(&c, 5) != xo[0]);
}
void get_next_letter_in_post(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t p4)
{
    if (!ckBegin(74, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    if (!ckLeg([&] { ref::get_next_letter_in_post(rdram, &c, runtime); })) return;
    ckMid();
    uint64_t v0 = 0u;
    float f0 = 0.0f;
    uint64_t xo[1] = {Ps2GprU64(ctx, 3)};
    if (!ckLeg([&] { v0 = a1_sx64(ty::get_next_letter_in_post(rdram, runtime, p4, xo)); })) return;
    ckEnd(74, ctx, c, 1, v0, f0, 0x8u, 0x0u, Ps2GprU64(&c, 3) != xo[0]);
}
void entry_new_letter_type(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t p4)
{
    if (!ckBegin(75, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    if (!ckLeg([&] { ref::entry_new_letter_type(rdram, &c, runtime); })) return;
    ckMid();
    uint64_t v0 = 0u;
    float f0 = 0.0f;
    if (!ckLeg([&] { v0 = (ty::entry_new_letter_type(rdram, runtime, p4)); })) return;
    ckEnd(75, ctx, c, 2, v0, f0, 0x28u, 0x0u, false);
}
void init_letter_type_store(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (!ckBegin(76, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    if (!ckLeg([&] { ref::init_letter_type_store(rdram, &c, runtime); })) return;
    ckMid();
    uint64_t v0 = 0u;
    float f0 = 0.0f;
    if (!ckLeg([&] { v0 = a1_sx64(ty::init_letter_type_store(rdram, runtime)); })) return;
    ckEnd(76, ctx, c, 1, v0, f0, 0x8u, 0x0u, false);
}
void kernel_sub_001C8B98(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t p4)
{
    if (!ckBegin(77, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    if (!ckLeg([&] { ref::kernel_sub_001C8B98(rdram, &c, runtime); })) return;
    ckMid();
    uint64_t v0 = 0u;
    float f0 = 0.0f;
    if (!ckLeg([&] { v0 = a1_sx64(ty::kernel_sub_001C8B98(rdram, runtime, p4)); })) return;
    ckEnd(77, ctx, c, 1, v0, f0, 0x7ff8u, 0x0u, false);
}
void iosGetFontInfo(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint64_t p4, uint64_t p5)
{
    if (!ckBegin(78, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    if (!ckLeg([&] { ref::iosGetFontInfo(rdram, &c, runtime); })) return;
    ckMid();
    uint64_t v0 = 0u;
    float f0 = 0.0f;
    uint64_t xo[3] = {Ps2GprU64(ctx, 5), Ps2GprU64(ctx, 6), Ps2GprU64(ctx, 7)};
    if (!ckLeg([&] { v0 = (ty::iosGetFontInfo(rdram, runtime, p4, p5, xo)); })) return;
    ckEnd(78, ctx, c, 2, v0, f0, 0xeau, 0x0u, Ps2GprU64(&c, 5) != xo[0] || Ps2GprU64(&c, 6) != xo[1] || Ps2GprU64(&c, 7) != xo[2]);
}
void sndSepPlayerSrh(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint64_t p4, uint64_t p5, uint64_t p6)
{
    if (!ckBegin(79, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    if (!ckLeg([&] { ref::sndSepPlayerSrh(rdram, &c, runtime); })) return;
    ckMid();
    uint64_t v0 = 0u;
    float f0 = 0.0f;
    uint64_t xo[4] = {Ps2GprU64(ctx, 7), Ps2GprU64(ctx, 8), Ps2GprU64(ctx, 9), Ps2GprU64(ctx, 10)};
    if (!ckLeg([&] { v0 = (ty::sndSepPlayerSrh(rdram, runtime, p4, p5, p6, xo)); })) return;
    ckEnd(79, ctx, c, 2, v0, f0, 0x798u, 0x0u, Ps2GprU64(&c, 7) != xo[0] || Ps2GprU64(&c, 8) != xo[1] || Ps2GprU64(&c, 9) != xo[2] || Ps2GprU64(&c, 10) != xo[3]);
}
void sndSepPlayerSrh2(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint64_t p4, uint64_t p5)
{
    if (!ckBegin(80, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    if (!ckLeg([&] { ref::sndSepPlayerSrh2(rdram, &c, runtime); })) return;
    ckMid();
    uint64_t v0 = 0u;
    float f0 = 0.0f;
    uint64_t xo[1] = {Ps2GprU64(ctx, 9)};
    if (!ckLeg([&] { v0 = (ty::sndSepPlayerSrh2(rdram, runtime, p4, p5, xo)); })) return;
    ckEnd(80, ctx, c, 2, v0, f0, 0x3d8u, 0x0u, Ps2GprU64(&c, 9) != xo[0]);
}
void sndSepSetVol(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t p4, float pf12)
{
    if (!ckBegin(81, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    if (!ckLeg([&] { ref::sndSepSetVol(rdram, &c, runtime); })) return;
    ckMid();
    uint64_t v0 = 0u;
    float f0 = 0.0f;
    uint64_t xo[2] = {Ps2GprU64(ctx, 5), Ps2GprU64(ctx, 6)};
    if (!ckLeg([&] { const a1::RetVF r = ty::sndSepSetVol(rdram, runtime, p4, pf12, xo); v0 = r.v0; f0 = r.f0; })) return;
    ckEnd(81, ctx, c, 4, v0, f0, 0x6au, 0x1002u, Ps2GprU64(&c, 5) != xo[0] || Ps2GprU64(&c, 6) != xo[1]);
}
void sndSepSetPan(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t p4, float pf12)
{
    if (!ckBegin(82, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    if (!ckLeg([&] { ref::sndSepSetPan(rdram, &c, runtime); })) return;
    ckMid();
    uint64_t v0 = 0u;
    float f0 = 0.0f;
    if (!ckLeg([&] { const a1::RetVF r = ty::sndSepSetPan(rdram, runtime, p4, pf12); v0 = r.v0; f0 = r.f0; })) return;
    ckEnd(82, ctx, c, 4, v0, f0, 0x2u, 0x1002u, false);
}
void sndSepStopTypeAll(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint64_t p4)
{
    if (!ckBegin(83, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    if (!ckLeg([&] { ref::sndSepStopTypeAll(rdram, &c, runtime); })) return;
    ckMid();
    uint64_t v0 = 0u;
    float f0 = 0.0f;
    if (!ckLeg([&] { v0 = (ty::sndSepStopTypeAll(rdram, runtime, p4)); })) return;
    ckEnd(83, ctx, c, 2, v0, f0, 0xe8u, 0x0u, false);
}
void sndVabCreate(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (!ckBegin(84, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    if (!ckLeg([&] { ref::sndVabCreate(rdram, &c, runtime); })) return;
    ckMid();
    uint64_t v0 = 0u;
    float f0 = 0.0f;
    uint64_t xo[4] = {Ps2GprU64(ctx, 7), Ps2GprU64(ctx, 8), Ps2GprU64(ctx, 9), Ps2GprU64(ctx, 10)};
    if (!ckLeg([&] { v0 = (ty::sndVabCreate(rdram, runtime, xo)); })) return;
    ckEnd(84, ctx, c, 2, v0, f0, 0x7f8u, 0x0u, Ps2GprU64(&c, 7) != xo[0] || Ps2GprU64(&c, 8) != xo[1] || Ps2GprU64(&c, 9) != xo[2] || Ps2GprU64(&c, 10) != xo[3]);
}
void sndVabInit(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (!ckBegin(85, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    if (!ckLeg([&] { ref::sndVabInit(rdram, &c, runtime); })) return;
    ckMid();
    uint64_t v0 = 0u;
    float f0 = 0.0f;
    if (!ckLeg([&] { v0 = a1_sx64(ty::sndVabInit(rdram, runtime)); })) return;
    ckEnd(85, ctx, c, 1, v0, f0, 0x38u, 0x0u, false);
}
void AddStrHashKey(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t p4, uint32_t p5)
{
    if (!ckBegin(86, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    if (!ckLeg([&] { ref::AddStrHashKey(rdram, &c, runtime); })) return;
    ckMid();
    uint64_t v0 = 0u;
    float f0 = 0.0f;
    uint64_t xo[2] = {Ps2GprU64(ctx, 6), Ps2GprU64(ctx, 7)};
    if (!ckLeg([&] { v0 = a1_sx64(ty::AddStrHashKey(rdram, runtime, p4, p5, xo)); })) return;
    ckEnd(86, ctx, c, 1, v0, f0, 0xc8u, 0x0u, Ps2GprU64(&c, 6) != xo[0] || Ps2GprU64(&c, 7) != xo[1]);
}
void Sg2CtrlEntryTickProcById(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint64_t p4, uint32_t p5)
{
    if (!ckBegin(87, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    if (!ckLeg([&] { ref::Sg2CtrlEntryTickProcById(rdram, &c, runtime); })) return;
    ckMid();
    uint64_t v0 = 0u;
    float f0 = 0.0f;
    uint64_t xo[4] = {Ps2GprU64(ctx, 4), Ps2GprU64(ctx, 6), Ps2GprU64(ctx, 7), Ps2GprU64(ctx, 8)};
    if (!ckLeg([&] { v0 = (ty::Sg2CtrlEntryTickProcById(rdram, runtime, p4, p5, xo)); })) return;
    ckEnd(87, ctx, c, 2, v0, f0, 0x1d8u, 0x0u, Ps2GprU64(&c, 4) != xo[0] || Ps2GprU64(&c, 6) != xo[1] || Ps2GprU64(&c, 7) != xo[2] || Ps2GprU64(&c, 8) != xo[3]);
}
void Sg2DspInit(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (!ckBegin(88, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    if (!ckLeg([&] { ref::Sg2DspInit(rdram, &c, runtime); })) return;
    ckMid();
    uint64_t v0 = 0u;
    float f0 = 0.0f;
    if (!ckLeg([&] { v0 = a1_sx64(ty::Sg2DspInit(rdram, runtime)); })) return;
    ckEnd(88, ctx, c, 1, v0, f0, 0x8u, 0x0u, false);
}
void ps2_____VolPerBackup(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t p4, uint64_t p5)
{
    if (!ckBegin(89, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    if (!ckLeg([&] { ref::ps2_____VolPerBackup(rdram, &c, runtime); })) return;
    ckMid();
    uint64_t v0 = 0u;
    float f0 = 0.0f;
    if (!ckLeg([&] { v0 = a1_sx64(ty::ps2_____VolPerBackup(rdram, runtime, p4, p5)); })) return;
    ckEnd(89, ctx, c, 1, v0, f0, 0x38u, 0x0u, false);
}
void kernel_sub_001CDDE8(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint64_t p4)
{
    if (!ckBegin(90, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    if (!ckLeg([&] { ref::kernel_sub_001CDDE8(rdram, &c, runtime); })) return;
    ckMid();
    uint64_t v0 = 0u;
    float f0 = 0.0f;
    uint64_t xo[5] = {Ps2GprU64(ctx, 7), Ps2GprU64(ctx, 8), Ps2GprU64(ctx, 9), Ps2GprU64(ctx, 10), Ps2GprU64(ctx, 11)};
    if (!ckLeg([&] { v0 = (ty::kernel_sub_001CDDE8(rdram, runtime, p4, xo)); })) return;
    ckEnd(90, ctx, c, 2, v0, f0, 0x100fffau, 0x0u, Ps2GprU64(&c, 7) != xo[0] || Ps2GprU64(&c, 8) != xo[1] || Ps2GprU64(&c, 9) != xo[2] || Ps2GprU64(&c, 10) != xo[3] || Ps2GprU64(&c, 11) != xo[4]);
}
void kernel_sub_001CDF38(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint64_t p4)
{
    if (!ckBegin(91, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    if (!ckLeg([&] { ref::kernel_sub_001CDF38(rdram, &c, runtime); })) return;
    ckMid();
    uint64_t v0 = 0u;
    float f0 = 0.0f;
    if (!ckLeg([&] { v0 = a1_sx64(ty::kernel_sub_001CDF38(rdram, runtime, p4)); })) return;
    ckEnd(91, ctx, c, 1, v0, f0, 0x3ffau, 0x0u, false);
}
void Sg2SepMidiOut(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint64_t p4, uint64_t p5, uint64_t p6)
{
    if (!ckBegin(92, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    if (!ckLeg([&] { ref::Sg2SepMidiOut(rdram, &c, runtime); })) return;
    ckMid();
    uint64_t v0 = 0u;
    float f0 = 0.0f;
    if (!ckLeg([&] { v0 = (ty::Sg2SepMidiOut(rdram, runtime, p4, p5, p6)); })) return;
    ckEnd(92, ctx, c, 2, v0, f0, 0xff8u, 0x0u, false);
}
void Sg2SlotGetSlotClassIdx(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (!ckBegin(93, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    if (!ckLeg([&] { ref::Sg2SlotGetSlotClassIdx(rdram, &c, runtime); })) return;
    ckMid();
    uint64_t v0 = 0u;
    float f0 = 0.0f;
    uint64_t xo[1] = {Ps2GprU64(ctx, 4)};
    if (!ckLeg([&] { v0 = (ty::Sg2SlotGetSlotClassIdx(rdram, runtime, xo)); })) return;
    ckEnd(93, ctx, c, 2, v0, f0, 0x18u, 0x0u, Ps2GprU64(&c, 4) != xo[0]);
}
void Sg2SlotBlock(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t p4, uint32_t p5)
{
    if (!ckBegin(94, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    if (!ckLeg([&] { ref::Sg2SlotBlock(rdram, &c, runtime); })) return;
    ckMid();
    uint64_t v0 = 0u;
    float f0 = 0.0f;
    uint64_t xo[7] = {Ps2GprU64(ctx, 5), Ps2GprU64(ctx, 6), Ps2GprU64(ctx, 7), Ps2GprU64(ctx, 8), Ps2GprU64(ctx, 9), Ps2GprU64(ctx, 10), Ps2GprU64(ctx, 11)};
    if (!ckLeg([&] { v0 = a1_sx64(ty::Sg2SlotBlock(rdram, runtime, p4, p5, xo)); })) return;
    ckEnd(94, ctx, c, 1, v0, f0, 0x3fe8u, 0x0u, Ps2GprU64(&c, 5) != xo[0] || Ps2GprU64(&c, 6) != xo[1] || Ps2GprU64(&c, 7) != xo[2] || Ps2GprU64(&c, 8) != xo[3] || Ps2GprU64(&c, 9) != xo[4] || Ps2GprU64(&c, 10) != xo[5] || Ps2GprU64(&c, 11) != xo[6]);
}
void Sg2SlotCheckSlotClass(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint64_t p4)
{
    if (!ckBegin(95, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    if (!ckLeg([&] { ref::Sg2SlotCheckSlotClass(rdram, &c, runtime); })) return;
    ckMid();
    uint64_t v0 = 0u;
    float f0 = 0.0f;
    uint64_t xo[1] = {Ps2GprU64(ctx, 6)};
    if (!ckLeg([&] { v0 = (ty::Sg2SlotCheckSlotClass(rdram, runtime, p4, xo)); })) return;
    ckEnd(95, ctx, c, 2, v0, f0, 0x68u, 0x0u, Ps2GprU64(&c, 6) != xo[0]);
}
void Sg2SlotFindLoop(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t p4)
{
    if (!ckBegin(96, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    if (!ckLeg([&] { ref::Sg2SlotFindLoop(rdram, &c, runtime); })) return;
    ckMid();
    uint64_t v0 = 0u;
    float f0 = 0.0f;
    if (!ckLeg([&] { v0 = (ty::Sg2SlotFindLoop(rdram, runtime, p4)); })) return;
    ckEnd(96, ctx, c, 2, v0, f0, 0x78u, 0x0u, false);
}
void Sg2SlotFindVab(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t p4, uint64_t p5)
{
    if (!ckBegin(97, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    if (!ckLeg([&] { ref::Sg2SlotFindVab(rdram, &c, runtime); })) return;
    ckMid();
    uint64_t v0 = 0u;
    float f0 = 0.0f;
    if (!ckLeg([&] { v0 = (ty::Sg2SlotFindVab(rdram, runtime, p4, p5)); })) return;
    ckEnd(97, ctx, c, 2, v0, f0, 0xd8u, 0x0u, false);
}
void Sg2SlotFindVabCh(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t p4, uint64_t p5, uint64_t p6)
{
    if (!ckBegin(98, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    if (!ckLeg([&] { ref::Sg2SlotFindVabCh(rdram, &c, runtime); })) return;
    ckMid();
    uint64_t v0 = 0u;
    float f0 = 0.0f;
    if (!ckLeg([&] { v0 = (ty::Sg2SlotFindVabCh(rdram, runtime, p4, p5, p6)); })) return;
    ckEnd(98, ctx, c, 2, v0, f0, 0x198u, 0x0u, false);
}
void Sg2SlotFindVabChNote(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t p4, uint64_t p5, uint64_t p6, uint64_t p7)
{
    if (!ckBegin(99, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    if (!ckLeg([&] { ref::Sg2SlotFindVabChNote(rdram, &c, runtime); })) return;
    ckMid();
    uint64_t v0 = 0u;
    float f0 = 0.0f;
    if (!ckLeg([&] { v0 = (ty::Sg2SlotFindVabChNote(rdram, runtime, p4, p5, p6, p7)); })) return;
    ckEnd(99, ctx, c, 2, v0, f0, 0x318u, 0x0u, false);
}
void Sg2SlotFindVabChSsaBaseNote(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t p4, uint64_t p5, uint64_t p6, uint64_t p7, uint64_t p8)
{
    if (!ckBegin(100, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    if (!ckLeg([&] { ref::Sg2SlotFindVabChSsaBaseNote(rdram, &c, runtime); })) return;
    ckMid();
    uint64_t v0 = 0u;
    float f0 = 0.0f;
    if (!ckLeg([&] { v0 = (ty::Sg2SlotFindVabChSsaBaseNote(rdram, runtime, p4, p5, p6, p7, p8)); })) return;
    ckEnd(100, ctx, c, 2, v0, f0, 0x618u, 0x0u, false);
}
void Sg2SlotFindSlotClass(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t p4, uint64_t p5)
{
    if (!ckBegin(101, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    if (!ckLeg([&] { ref::Sg2SlotFindSlotClass(rdram, &c, runtime); })) return;
    ckMid();
    uint64_t v0 = 0u;
    float f0 = 0.0f;
    uint64_t xo[3] = {Ps2GprU64(ctx, 4), Ps2GprU64(ctx, 6), Ps2GprU64(ctx, 7)};
    if (!ckLeg([&] { v0 = (ty::Sg2SlotFindSlotClass(rdram, runtime, p4, p5, xo)); })) return;
    ckEnd(101, ctx, c, 2, v0, f0, 0xd8u, 0x0u, Ps2GprU64(&c, 4) != xo[0] || Ps2GprU64(&c, 6) != xo[1] || Ps2GprU64(&c, 7) != xo[2]);
}
void Sg2SlotFindSlotClassCh(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t p4, uint64_t p5, uint64_t p6)
{
    if (!ckBegin(102, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    if (!ckLeg([&] { ref::Sg2SlotFindSlotClassCh(rdram, &c, runtime); })) return;
    ckMid();
    uint64_t v0 = 0u;
    float f0 = 0.0f;
    uint64_t xo[3] = {Ps2GprU64(ctx, 4), Ps2GprU64(ctx, 7), Ps2GprU64(ctx, 8)};
    if (!ckLeg([&] { v0 = (ty::Sg2SlotFindSlotClassCh(rdram, runtime, p4, p5, p6, xo)); })) return;
    ckEnd(102, ctx, c, 2, v0, f0, 0x198u, 0x0u, Ps2GprU64(&c, 4) != xo[0] || Ps2GprU64(&c, 7) != xo[1] || Ps2GprU64(&c, 8) != xo[2]);
}
void Sg2SlotFindSlotClassChNote(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t p4, uint64_t p5, uint64_t p6, uint64_t p7)
{
    if (!ckBegin(103, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    if (!ckLeg([&] { ref::Sg2SlotFindSlotClassChNote(rdram, &c, runtime); })) return;
    ckMid();
    uint64_t v0 = 0u;
    float f0 = 0.0f;
    if (!ckLeg([&] { v0 = (ty::Sg2SlotFindSlotClassChNote(rdram, runtime, p4, p5, p6, p7)); })) return;
    ckEnd(103, ctx, c, 2, v0, f0, 0x318u, 0x0u, false);
}
void Sg2SlotFindSlotClassChSsaBaseNote(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t p4, uint64_t p5, uint64_t p6, uint64_t p7, uint64_t p8)
{
    if (!ckBegin(104, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    if (!ckLeg([&] { ref::Sg2SlotFindSlotClassChSsaBaseNote(rdram, &c, runtime); })) return;
    ckMid();
    uint64_t v0 = 0u;
    float f0 = 0.0f;
    uint64_t xo[3] = {Ps2GprU64(ctx, 4), Ps2GprU64(ctx, 9), Ps2GprU64(ctx, 10)};
    if (!ckLeg([&] { v0 = (ty::Sg2SlotFindSlotClassChSsaBaseNote(rdram, runtime, p4, p5, p6, p7, p8, xo)); })) return;
    ckEnd(104, ctx, c, 2, v0, f0, 0x618u, 0x0u, Ps2GprU64(&c, 4) != xo[0] || Ps2GprU64(&c, 9) != xo[1] || Ps2GprU64(&c, 10) != xo[2]);
}
void Sg2SlotSetMasVol(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t p4, uint32_t p5, uint32_t p6)
{
    if (!ckBegin(105, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    if (!ckLeg([&] { ref::Sg2SlotSetMasVol(rdram, &c, runtime); })) return;
    ckMid();
    uint64_t v0 = 0u;
    float f0 = 0.0f;
    uint64_t xo[6] = {Ps2GprU64(ctx, 4), Ps2GprU64(ctx, 5), Ps2GprU64(ctx, 7), Ps2GprU64(ctx, 8), Ps2GprU64(ctx, 9), Ps2GprU64(ctx, 10)};
    if (!ckLeg([&] { v0 = a1_sx64(ty::Sg2SlotSetMasVol(rdram, runtime, p4, p5, p6, xo)); })) return;
    ckEnd(105, ctx, c, 1, v0, f0, 0x7b8u, 0x0u, Ps2GprU64(&c, 4) != xo[0] || Ps2GprU64(&c, 5) != xo[1] || Ps2GprU64(&c, 7) != xo[2] || Ps2GprU64(&c, 8) != xo[3] || Ps2GprU64(&c, 9) != xo[4] || Ps2GprU64(&c, 10) != xo[5]);
}
void Sg2SlotSetSeMasVol(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t p4, uint32_t p5)
{
    if (!ckBegin(106, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    if (!ckLeg([&] { ref::Sg2SlotSetSeMasVol(rdram, &c, runtime); })) return;
    ckMid();
    uint64_t v0 = 0u;
    float f0 = 0.0f;
    uint64_t xo[4] = {Ps2GprU64(ctx, 4), Ps2GprU64(ctx, 5), Ps2GprU64(ctx, 6), Ps2GprU64(ctx, 7)};
    if (!ckLeg([&] { v0 = a1_sx64(ty::Sg2SlotSetSeMasVol(rdram, runtime, p4, p5, xo)); })) return;
    ckEnd(106, ctx, c, 1, v0, f0, 0xf8u, 0x0u, Ps2GprU64(&c, 4) != xo[0] || Ps2GprU64(&c, 5) != xo[1] || Ps2GprU64(&c, 6) != xo[2] || Ps2GprU64(&c, 7) != xo[3]);
}
void Sg2SlotSetBaseVol(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t p4, uint32_t p5)
{
    if (!ckBegin(107, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    if (!ckLeg([&] { ref::Sg2SlotSetBaseVol(rdram, &c, runtime); })) return;
    ckMid();
    uint64_t v0 = 0u;
    float f0 = 0.0f;
    uint64_t xo[1] = {Ps2GprU64(ctx, 7)};
    if (!ckLeg([&] { v0 = a1_sx64(ty::Sg2SlotSetBaseVol(rdram, runtime, p4, p5, xo)); })) return;
    ckEnd(107, ctx, c, 1, v0, f0, 0xf8u, 0x0u, Ps2GprU64(&c, 7) != xo[0]);
}
void Sg2SlotSetVol(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t p4, uint32_t p5)
{
    if (!ckBegin(108, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    if (!ckLeg([&] { ref::Sg2SlotSetVol(rdram, &c, runtime); })) return;
    ckMid();
    uint64_t v0 = 0u;
    float f0 = 0.0f;
    uint64_t xo[1] = {Ps2GprU64(ctx, 4)};
    if (!ckLeg([&] { v0 = a1_sx64(ty::Sg2SlotSetVol(rdram, runtime, p4, p5, xo)); })) return;
    ckEnd(108, ctx, c, 1, v0, f0, 0x18u, 0x0u, Ps2GprU64(&c, 4) != xo[0]);
}
void Sg2SlotSetMasPan(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t p4, uint32_t p5)
{
    if (!ckBegin(109, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    if (!ckLeg([&] { ref::Sg2SlotSetMasPan(rdram, &c, runtime); })) return;
    ckMid();
    uint64_t v0 = 0u;
    float f0 = 0.0f;
    uint64_t xo[4] = {Ps2GprU64(ctx, 4), Ps2GprU64(ctx, 5), Ps2GprU64(ctx, 6), Ps2GprU64(ctx, 7)};
    if (!ckLeg([&] { v0 = a1_sx64(ty::Sg2SlotSetMasPan(rdram, runtime, p4, p5, xo)); })) return;
    ckEnd(109, ctx, c, 1, v0, f0, 0xf8u, 0x0u, Ps2GprU64(&c, 4) != xo[0] || Ps2GprU64(&c, 5) != xo[1] || Ps2GprU64(&c, 6) != xo[2] || Ps2GprU64(&c, 7) != xo[3]);
}
void Sg2SlotSetBasePan(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t p4, uint32_t p5)
{
    if (!ckBegin(110, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    if (!ckLeg([&] { ref::Sg2SlotSetBasePan(rdram, &c, runtime); })) return;
    ckMid();
    uint64_t v0 = 0u;
    float f0 = 0.0f;
    uint64_t xo[1] = {Ps2GprU64(ctx, 7)};
    if (!ckLeg([&] { v0 = a1_sx64(ty::Sg2SlotSetBasePan(rdram, runtime, p4, p5, xo)); })) return;
    ckEnd(110, ctx, c, 1, v0, f0, 0xf8u, 0x0u, Ps2GprU64(&c, 7) != xo[0]);
}
void Sg2SlotSetPan(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t p4, uint32_t p5)
{
    if (!ckBegin(111, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    if (!ckLeg([&] { ref::Sg2SlotSetPan(rdram, &c, runtime); })) return;
    ckMid();
    uint64_t v0 = 0u;
    float f0 = 0.0f;
    uint64_t xo[1] = {Ps2GprU64(ctx, 4)};
    if (!ckLeg([&] { v0 = a1_sx64(ty::Sg2SlotSetPan(rdram, runtime, p4, p5, xo)); })) return;
    ckEnd(111, ctx, c, 1, v0, f0, 0x18u, 0x0u, Ps2GprU64(&c, 4) != xo[0]);
}
void Sg2SlotSetMasTune(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t p4, uint32_t p5)
{
    if (!ckBegin(112, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    if (!ckLeg([&] { ref::Sg2SlotSetMasTune(rdram, &c, runtime); })) return;
    ckMid();
    uint64_t v0 = 0u;
    float f0 = 0.0f;
    uint64_t xo[1] = {Ps2GprU64(ctx, 4)};
    if (!ckLeg([&] { v0 = a1_sx64(ty::Sg2SlotSetMasTune(rdram, runtime, p4, p5, xo)); })) return;
    ckEnd(112, ctx, c, 1, v0, f0, 0x18u, 0x0u, Ps2GprU64(&c, 4) != xo[0]);
}
void Sg2SlotSetVolPhase(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t p4, uint32_t p5, uint32_t p6)
{
    if (!ckBegin(113, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    if (!ckLeg([&] { ref::Sg2SlotSetVolPhase(rdram, &c, runtime); })) return;
    ckMid();
    uint64_t v0 = 0u;
    float f0 = 0.0f;
    uint64_t xo[3] = {Ps2GprU64(ctx, 4), Ps2GprU64(ctx, 7), Ps2GprU64(ctx, 8)};
    if (!ckLeg([&] { v0 = (ty::Sg2SlotSetVolPhase(rdram, runtime, p4, p5, p6, xo)); })) return;
    ckEnd(113, ctx, c, 2, v0, f0, 0x198u, 0x0u, Ps2GprU64(&c, 4) != xo[0] || Ps2GprU64(&c, 7) != xo[1] || Ps2GprU64(&c, 8) != xo[2]);
}
void Sg2SlotSetLfoPitchTbl(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t p4, uint64_t p5)
{
    if (!ckBegin(114, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    if (!ckLeg([&] { ref::Sg2SlotSetLfoPitchTbl(rdram, &c, runtime); })) return;
    ckMid();
    uint64_t v0 = 0u;
    float f0 = 0.0f;
    uint64_t xo[2] = {Ps2GprU64(ctx, 6), Ps2GprU64(ctx, 7)};
    if (!ckLeg([&] { v0 = (ty::Sg2SlotSetLfoPitchTbl(rdram, runtime, p4, p5, xo)); })) return;
    ckEnd(114, ctx, c, 2, v0, f0, 0xf8u, 0x0u, Ps2GprU64(&c, 6) != xo[0] || Ps2GprU64(&c, 7) != xo[1]);
}
void Sg2SlotSetLfoPitchDepth(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint64_t p4, uint64_t p5)
{
    if (!ckBegin(115, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    if (!ckLeg([&] { ref::Sg2SlotSetLfoPitchDepth(rdram, &c, runtime); })) return;
    ckMid();
    uint64_t v0 = 0u;
    float f0 = 0.0f;
    uint64_t xo[5] = {Ps2GprU64(ctx, 6), Ps2GprU64(ctx, 7), Ps2GprU64(ctx, 8), Ps2GprU64(ctx, 9), Ps2GprU64(ctx, 10)};
    if (!ckLeg([&] { v0 = (ty::Sg2SlotSetLfoPitchDepth(rdram, runtime, p4, p5, xo)); })) return;
    ckEnd(115, ctx, c, 2, v0, f0, 0x7f8u, 0x0u, Ps2GprU64(&c, 6) != xo[0] || Ps2GprU64(&c, 7) != xo[1] || Ps2GprU64(&c, 8) != xo[2] || Ps2GprU64(&c, 9) != xo[3] || Ps2GprU64(&c, 10) != xo[4]);
}
void Sg2SlotSetPoltamentParam(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint64_t p4, uint64_t p5, float pf12)
{
    if (!ckBegin(116, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    if (!ckLeg([&] { ref::Sg2SlotSetPoltamentParam(rdram, &c, runtime); })) return;
    ckMid();
    uint64_t v0 = 0u;
    float f0 = 0.0f;
    uint64_t xo[1] = {(uint64_t)a1ck::ckBits(ctx->f[12])};
    if (!ckLeg([&] { v0 = a1_sx64(ty::Sg2SlotSetPoltamentParam(rdram, runtime, p4, p5, pf12, xo)); })) return;
    ckEnd(116, ctx, c, 1, v0, f0, 0x1au, 0x1000u, a1ck::ckBits(c.f[12]) != (uint32_t)xo[0]);
}
void Sg2SlotTbl(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t p4, uint64_t p5)
{
    if (!ckBegin(117, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    if (!ckLeg([&] { ref::Sg2SlotTbl(rdram, &c, runtime); })) return;
    ckMid();
    uint64_t v0 = 0u;
    float f0 = 0.0f;
    if (!ckLeg([&] { v0 = a1_sx64(ty::Sg2SlotTbl(rdram, runtime, p4, p5)); })) return;
    ckEnd(117, ctx, c, 1, v0, f0, 0x1ff8u, 0x0u, false);
}
void Sg2SmfpMidiOut(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint64_t p4, uint64_t p5, uint32_t p6)
{
    if (!ckBegin(118, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    if (!ckLeg([&] { ref::Sg2SmfpMidiOut(rdram, &c, runtime); })) return;
    ckMid();
    uint64_t v0 = 0u;
    float f0 = 0.0f;
    if (!ckLeg([&] { v0 = a1_sx64(ty::Sg2SmfpMidiOut(rdram, runtime, p4, p5, p6)); })) return;
    ckEnd(118, ctx, c, 1, v0, f0, 0x7fau, 0x0u, false);
}
void get_vabid(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (!ckBegin(119, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    if (!ckLeg([&] { ref::get_vabid(rdram, &c, runtime); })) return;
    ckMid();
    uint64_t v0 = 0u;
    float f0 = 0.0f;
    uint64_t xo[1] = {Ps2GprU64(ctx, 6)};
    if (!ckLeg([&] { v0 = (ty::get_vabid(rdram, runtime, xo)); })) return;
    ckEnd(119, ctx, c, 2, v0, f0, 0x78u, 0x0u, Ps2GprU64(&c, 6) != xo[0]);
}
void kernel_sub_001D9F08(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t p4, uint64_t p5)
{
    if (!ckBegin(120, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    if (!ckLeg([&] { ref::kernel_sub_001D9F08(rdram, &c, runtime); })) return;
    ckMid();
    uint64_t v0 = 0u;
    float f0 = 0.0f;
    uint64_t xo[2] = {Ps2GprU64(ctx, 5), Ps2GprU64(ctx, 6)};
    if (!ckLeg([&] { v0 = a1_sx64(ty::kernel_sub_001D9F08(rdram, runtime, p4, p5, xo)); })) return;
    ckEnd(120, ctx, c, 1, v0, f0, 0x68u, 0x0u, Ps2GprU64(&c, 5) != xo[0] || Ps2GprU64(&c, 6) != xo[1]);
}
void kernel_sub_001DAAB0(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint64_t p4, uint64_t p5)
{
    if (!ckBegin(121, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    if (!ckLeg([&] { ref::kernel_sub_001DAAB0(rdram, &c, runtime); })) return;
    ckMid();
    uint64_t v0 = 0u;
    float f0 = 0.0f;
    uint64_t xo[5] = {(uint64_t)a1ck::ckBits(ctx->f[0]), Ps2GprU64(ctx, 2), Ps2GprU64(ctx, 6), Ps2GprU64(ctx, 7), Ps2GprU64(ctx, 8)};
    if (!ckLeg([&] { ty::kernel_sub_001DAAB0(rdram, runtime, p4, p5, xo); })) return;
    ckEnd(121, ctx, c, 0, v0, f0, 0x1feu, 0x7u, a1ck::ckBits(c.f[0]) != (uint32_t)xo[0] || Ps2GprU64(&c, 2) != xo[1] || Ps2GprU64(&c, 6) != xo[2] || Ps2GprU64(&c, 7) != xo[3] || Ps2GprU64(&c, 8) != xo[4]);
}
void kernel_sub_001DAC18(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t p4, uint32_t p5, uint64_t p6)
{
    if (!ckBegin(122, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    if (!ckLeg([&] { ref::kernel_sub_001DAC18(rdram, &c, runtime); })) return;
    ckMid();
    uint64_t v0 = 0u;
    float f0 = 0.0f;
    uint64_t xo[3] = {(uint64_t)a1ck::ckBits(ctx->f[0]), (uint64_t)a1ck::ckBits(ctx->f[1]), Ps2GprU64(ctx, 6)};
    if (!ckLeg([&] { v0 = a1_sx64(ty::kernel_sub_001DAC18(rdram, runtime, p4, p5, p6, xo)); })) return;
    ckEnd(122, ctx, c, 1, v0, f0, 0x48u, 0x3u, a1ck::ckBits(c.f[0]) != (uint32_t)xo[0] || a1ck::ckBits(c.f[1]) != (uint32_t)xo[1] || Ps2GprU64(&c, 6) != xo[2]);
}
void kernel_sub_001DB678(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (!ckBegin(123, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    if (!ckLeg([&] { ref::kernel_sub_001DB678(rdram, &c, runtime); })) return;
    ckMid();
    uint64_t v0 = 0u;
    float f0 = 0.0f;
    uint64_t xo[1] = {Ps2GprU64(ctx, 6)};
    if (!ckLeg([&] { v0 = (ty::kernel_sub_001DB678(rdram, runtime, xo)); })) return;
    ckEnd(123, ctx, c, 2, v0, f0, 0x78u, 0x0u, Ps2GprU64(&c, 6) != xo[0]);
}
void IirFilter(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t p4, uint64_t p5, uint32_t p6)
{
    if (!ckBegin(124, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    if (!ckLeg([&] { ref::IirFilter(rdram, &c, runtime); })) return;
    ckMid();
    uint64_t v0 = 0u;
    float f0 = 0.0f;
    if (!ckLeg([&] { const a1::RetVF r = ty::IirFilter(rdram, runtime, p4, p5, p6); v0 = r.v0; f0 = r.f0; })) return;
    ckEnd(124, ctx, c, 4, v0, f0, 0x8u, 0x7ffeu, false);
}
void ps2___ieee754_fmodf(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, float pf12, float pf13)
{
    if (!ckBegin(125, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    if (!ckLeg([&] { ref::ps2___ieee754_fmodf(rdram, &c, runtime); })) return;
    ckMid();
    uint64_t v0 = 0u;
    float f0 = 0.0f;
    if (!ckLeg([&] { const a1::RetVF r = ty::ps2___ieee754_fmodf(rdram, runtime, pf12, pf13); v0 = r.v0; f0 = r.f0; })) return;
    ckEnd(125, ctx, c, 4, v0, f0, 0x7fau, 0x2u, false);
}
void GAMEUTIL_inc_or_dec_for_near_to_aim_pitch(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint64_t p4, uint64_t p5, float pf12, float pf13, float pf14, float pf15, float pf16)
{
    if (!ckBegin(126, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    if (!ckLeg([&] { ref::GAMEUTIL_inc_or_dec_for_near_to_aim_pitch(rdram, &c, runtime); })) return;
    ckMid();
    uint64_t v0 = 0u;
    float f0 = 0.0f;
    uint64_t xo[1] = {(uint64_t)a1ck::ckBits(ctx->f[0])};
    if (!ckLeg([&] { v0 = (ty::GAMEUTIL_inc_or_dec_for_near_to_aim_pitch(rdram, runtime, p4, p5, pf12, pf13, pf14, pf15, pf16, xo)); })) return;
    ckEnd(126, ctx, c, 2, v0, f0, 0xau, 0x407fu, a1ck::ckBits(c.f[0]) != (uint32_t)xo[0]);
}
void GAMEUTIL_get_interp_pitch(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, float pf12, float pf13, float pf14)
{
    if (!ckBegin(127, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    if (!ckLeg([&] { ref::GAMEUTIL_get_interp_pitch(rdram, &c, runtime); })) return;
    ckMid();
    uint64_t v0 = 0u;
    float f0 = 0.0f;
    uint64_t xo[2] = {(uint64_t)a1ck::ckBits(ctx->f[13]), Ps2GprU64(ctx, 2)};
    if (!ckLeg([&] { f0 = ty::GAMEUTIL_get_interp_pitch(rdram, runtime, pf12, pf13, pf14, xo); })) return;
    ckEnd(127, ctx, c, 3, v0, f0, 0x6u, 0x203eu, a1ck::ckBits(c.f[13]) != (uint32_t)xo[0] || Ps2GprU64(&c, 2) != xo[1]);
}
void GAMEUTIL_is_diff_nearly_equal_zero(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, float pf12, float pf13, float pf14)
{
    if (!ckBegin(128, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    if (!ckLeg([&] { ref::GAMEUTIL_is_diff_nearly_equal_zero(rdram, &c, runtime); })) return;
    ckMid();
    uint64_t v0 = 0u;
    float f0 = 0.0f;
    uint64_t xo[1] = {(uint64_t)a1ck::ckBits(ctx->f[12])};
    if (!ckLeg([&] { const a1::RetVF r = ty::GAMEUTIL_is_diff_nearly_equal_zero(rdram, runtime, pf12, pf13, pf14, xo); v0 = r.v0; f0 = r.f0; })) return;
    ckEnd(128, ctx, c, 4, v0, f0, 0x0u, 0x1000u, a1ck::ckBits(c.f[12]) != (uint32_t)xo[0]);
}
void GAMEUTIL_get_cross_position_coefficient_t_by_dist_factor(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint64_t p4, uint32_t p5, uint32_t p6, uint32_t p7, uint32_t p8, float pf12)
{
    if (!ckBegin(129, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    if (!ckLeg([&] { ref::GAMEUTIL_get_cross_position_coefficient_t_by_dist_factor(rdram, &c, runtime); })) return;
    ckMid();
    uint64_t v0 = 0u;
    float f0 = 0.0f;
    uint64_t xo[2] = {Ps2GprU64(ctx, 9), Ps2GprU64(ctx, 10)};
    if (!ckLeg([&] { const a1::RetVF r = ty::GAMEUTIL_get_cross_position_coefficient_t_by_dist_factor(rdram, runtime, p4, p5, p6, p7, p8, pf12, xo); v0 = r.v0; f0 = r.f0; })) return;
    ckEnd(129, ctx, c, 4, v0, f0, 0x61au, 0x7eu, Ps2GprU64(&c, 9) != xo[0] || Ps2GprU64(&c, 10) != xo[1]);
}
void GAMEUTIL_get_segment_num_and_data(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint64_t p4, uint64_t p5)
{
    if (!ckBegin(130, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    if (!ckLeg([&] { ref::GAMEUTIL_get_segment_num_and_data(rdram, &c, runtime); })) return;
    ckMid();
    uint64_t v0 = 0u;
    float f0 = 0.0f;
    uint64_t xo[2] = {Ps2GprU64(ctx, 5), Ps2GprU64(ctx, 6)};
    if (!ckLeg([&] { v0 = a1_sx64(ty::GAMEUTIL_get_segment_num_and_data(rdram, runtime, p4, p5, xo)); })) return;
    ckEnd(130, ctx, c, 1, v0, f0, 0x68u, 0x0u, Ps2GprU64(&c, 5) != xo[0] || Ps2GprU64(&c, 6) != xo[1]);
}
void GAMEUTIL_get_new_velocity(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, float pf12, float pf13, float pf14, float pf15, float pf16)
{
    if (!ckBegin(131, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    if (!ckLeg([&] { ref::GAMEUTIL_get_new_velocity(rdram, &c, runtime); })) return;
    ckMid();
    uint64_t v0 = 0u;
    float f0 = 0.0f;
    uint64_t xo[1] = {Ps2GprU64(ctx, 2)};
    if (!ckLeg([&] { f0 = ty::GAMEUTIL_get_new_velocity(rdram, runtime, pf12, pf13, pf14, pf15, pf16, xo); })) return;
    ckEnd(131, ctx, c, 3, v0, f0, 0xeu, 0x2002u, Ps2GprU64(&c, 2) != xo[0]);
}
void GAMEUTIL_increase_pitch(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint64_t p4, uint64_t p5, float pf12, float pf13, float pf14, float pf15)
{
    if (!ckBegin(132, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    if (!ckLeg([&] { ref::GAMEUTIL_increase_pitch(rdram, &c, runtime); })) return;
    ckMid();
    uint64_t v0 = 0u;
    float f0 = 0.0f;
    uint64_t xo[2] = {(uint64_t)a1ck::ckBits(ctx->f[14]), (uint64_t)a1ck::ckBits(ctx->f[15])};
    if (!ckLeg([&] { const a1::RetVF r = ty::GAMEUTIL_increase_pitch(rdram, runtime, p4, p5, pf12, pf13, pf14, pf15, xo); v0 = r.v0; f0 = r.f0; })) return;
    ckEnd(132, ctx, c, 4, v0, f0, 0xau, 0xc006u, a1ck::ckBits(c.f[14]) != (uint32_t)xo[0] || a1ck::ckBits(c.f[15]) != (uint32_t)xo[1]);
}
void GAMEUTIL_get_difference_yaw_and_yaw(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, float pf12, float pf13)
{
    if (!ckBegin(133, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    if (!ckLeg([&] { ref::GAMEUTIL_get_difference_yaw_and_yaw(rdram, &c, runtime); })) return;
    ckMid();
    uint64_t v0 = 0u;
    float f0 = 0.0f;
    uint64_t xo[1] = {(uint64_t)a1ck::ckBits(ctx->f[13])};
    if (!ckLeg([&] { f0 = ty::GAMEUTIL_get_difference_yaw_and_yaw(rdram, runtime, pf12, pf13, xo); })) return;
    ckEnd(133, ctx, c, 3, v0, f0, 0x2u, 0x2000u, a1ck::ckBits(c.f[13]) != (uint32_t)xo[0]);
}
void GAMEUTIL_is_reach_aim_pitch_angle(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint64_t p4, float pf12, float pf13, float pf14)
{
    if (!ckBegin(134, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    if (!ckLeg([&] { ref::GAMEUTIL_is_reach_aim_pitch_angle(rdram, &c, runtime); })) return;
    ckMid();
    uint64_t v0 = 0u;
    float f0 = 0.0f;
    uint64_t xo[1] = {(uint64_t)a1ck::ckBits(ctx->f[0])};
    if (!ckLeg([&] { v0 = a1_sx64(ty::GAMEUTIL_is_reach_aim_pitch_angle(rdram, runtime, p4, pf12, pf13, pf14, xo)); })) return;
    ckEnd(134, ctx, c, 1, v0, f0, 0xau, 0x3u, a1ck::ckBits(c.f[0]) != (uint32_t)xo[0]);
}
void GAMEUTIL_reset_interp_ratio_cntrl(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t p4)
{
    if (!ckBegin(135, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    if (!ckLeg([&] { ref::GAMEUTIL_reset_interp_ratio_cntrl(rdram, &c, runtime); })) return;
    ckMid();
    uint64_t v0 = 0u;
    float f0 = 0.0f;
    uint64_t xo[1] = {(uint64_t)a1ck::ckBits(ctx->f[0])};
    if (!ckLeg([&] { v0 = a1_sx64(ty::GAMEUTIL_reset_interp_ratio_cntrl(rdram, runtime, p4, xo)); })) return;
    ckEnd(135, ctx, c, 1, v0, f0, 0xau, 0x1u, a1ck::ckBits(c.f[0]) != (uint32_t)xo[0]);
}
void reset_double_spring_interp_cntrl3D(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t p4, uint32_t p5)
{
    if (!ckBegin(136, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    if (!ckLeg([&] { ref::reset_double_spring_interp_cntrl3D(rdram, &c, runtime); })) return;
    ckMid();
    uint64_t v0 = 0u;
    float f0 = 0.0f;
    uint64_t xo[3] = {Ps2GprU64(ctx, 6), Ps2GprU64(ctx, 7), Ps2GprU64(ctx, 8)};
    if (!ckLeg([&] { const a1::RetVF r = ty::reset_double_spring_interp_cntrl3D(rdram, runtime, p4, p5, xo); v0 = r.v0; f0 = r.f0; })) return;
    ckEnd(136, ctx, c, 4, v0, f0, 0x1d8u, 0x0u, Ps2GprU64(&c, 6) != xo[0] || Ps2GprU64(&c, 7) != xo[1] || Ps2GprU64(&c, 8) != xo[2]);
}
void endBossDeathEffect(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (!ckBegin(137, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    if (!ckLeg([&] { ref::endBossDeathEffect(rdram, &c, runtime); })) return;
    ckMid();
    uint64_t v0 = 0u;
    float f0 = 0.0f;
    uint64_t xo[1] = {Ps2GprU64(ctx, 5)};
    if (!ckLeg([&] { v0 = a1_sx64(ty::endBossDeathEffect(rdram, runtime, xo)); })) return;
    ckEnd(137, ctx, c, 1, v0, f0, 0x38u, 0x0u, Ps2GprU64(&c, 5) != xo[0]);
}
void fsEffectSetParam(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint64_t p4)
{
    if (!ckBegin(138, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    if (!ckLeg([&] { ref::fsEffectSetParam(rdram, &c, runtime); })) return;
    ckMid();
    uint64_t v0 = 0u;
    float f0 = 0.0f;
    uint64_t xo[1] = {Ps2GprU64(ctx, 5)};
    if (!ckLeg([&] { v0 = a1_sx64(ty::fsEffectSetParam(rdram, runtime, p4, xo)); })) return;
    ckEnd(138, ctx, c, 1, v0, f0, 0x28u, 0x0u, Ps2GprU64(&c, 5) != xo[0]);
}
void fsEffectManagerSetDemoParam(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t p4)
{
    if (!ckBegin(139, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    if (!ckLeg([&] { ref::fsEffectManagerSetDemoParam(rdram, &c, runtime); })) return;
    ckMid();
    uint64_t v0 = 0u;
    float f0 = 0.0f;
    uint64_t xo[4] = {Ps2GprU64(ctx, 4), Ps2GprU64(ctx, 5), Ps2GprU64(ctx, 6), Ps2GprU64(ctx, 7)};
    if (!ckLeg([&] { v0 = a1_sx64(ty::fsEffectManagerSetDemoParam(rdram, runtime, p4, xo)); })) return;
    ckEnd(139, ctx, c, 1, v0, f0, 0xf8u, 0x0u, Ps2GprU64(&c, 4) != xo[0] || Ps2GprU64(&c, 5) != xo[1] || Ps2GprU64(&c, 6) != xo[2] || Ps2GprU64(&c, 7) != xo[3]);
}
void fsEffectManagerInit(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (!ckBegin(140, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    if (!ckLeg([&] { ref::fsEffectManagerInit(rdram, &c, runtime); })) return;
    ckMid();
    uint64_t v0 = 0u;
    float f0 = 0.0f;
    if (!ckLeg([&] { v0 = (ty::fsEffectManagerInit(rdram, runtime)); })) return;
    ckEnd(140, ctx, c, 2, v0, f0, 0x1f8u, 0x0u, false);
}
void initFsEffect(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint64_t p4)
{
    if (!ckBegin(141, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    if (!ckLeg([&] { ref::initFsEffect(rdram, &c, runtime); })) return;
    ckMid();
    uint64_t v0 = 0u;
    float f0 = 0.0f;
    if (!ckLeg([&] { v0 = (ty::initFsEffect(rdram, runtime, p4)); })) return;
    ckEnd(141, ctx, c, 2, v0, f0, 0x1f8u, 0x0u, false);
}
void setFogParamDefault(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (!ckBegin(142, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    if (!ckLeg([&] { ref::setFogParamDefault(rdram, &c, runtime); })) return;
    ckMid();
    uint64_t v0 = 0u;
    float f0 = 0.0f;
    uint64_t xo[4] = {Ps2GprU64(ctx, 5), Ps2GprU64(ctx, 6), Ps2GprU64(ctx, 7), Ps2GprU64(ctx, 8)};
    if (!ckLeg([&] { v0 = (ty::setFogParamDefault(rdram, runtime, xo)); })) return;
    ckEnd(142, ctx, c, 2, v0, f0, 0x1f8u, 0x0u, Ps2GprU64(&c, 5) != xo[0] || Ps2GprU64(&c, 6) != xo[1] || Ps2GprU64(&c, 7) != xo[2] || Ps2GprU64(&c, 8) != xo[3]);
}
void glBlendFunc(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint64_t p4, uint64_t p5)
{
    if (!ckBegin(143, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    if (!ckLeg([&] { ref::glBlendFunc(rdram, &c, runtime); })) return;
    ckMid();
    uint64_t v0 = 0u;
    float f0 = 0.0f;
    uint64_t xo[5] = {Ps2GprU64(ctx, 4), Ps2GprU64(ctx, 5), Ps2GprU64(ctx, 6), Ps2GprU64(ctx, 7), Ps2GprU64(ctx, 8)};
    if (!ckLeg([&] { v0 = (ty::glBlendFunc(rdram, runtime, p4, p5, xo)); })) return;
    ckEnd(143, ctx, c, 2, v0, f0, 0x1f8u, 0x0u, Ps2GprU64(&c, 4) != xo[0] || Ps2GprU64(&c, 5) != xo[1] || Ps2GprU64(&c, 6) != xo[2] || Ps2GprU64(&c, 7) != xo[3] || Ps2GprU64(&c, 8) != xo[4]);
}
void glBlendFuncParticle(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint64_t p4)
{
    if (!ckBegin(144, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    if (!ckLeg([&] { ref::glBlendFuncParticle(rdram, &c, runtime); })) return;
    ckMid();
    uint64_t v0 = 0u;
    float f0 = 0.0f;
    uint64_t xo[4] = {Ps2GprU64(ctx, 4), Ps2GprU64(ctx, 5), Ps2GprU64(ctx, 6), Ps2GprU64(ctx, 7)};
    if (!ckLeg([&] { v0 = (ty::glBlendFuncParticle(rdram, runtime, p4, xo)); })) return;
    ckEnd(144, ctx, c, 2, v0, f0, 0xf8u, 0x0u, Ps2GprU64(&c, 4) != xo[0] || Ps2GprU64(&c, 5) != xo[1] || Ps2GprU64(&c, 6) != xo[2] || Ps2GprU64(&c, 7) != xo[3]);
}
void glColor4f(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, float pf12, float pf13, float pf14, float pf15)
{
    if (!ckBegin(145, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    if (!ckLeg([&] { ref::glColor4f(rdram, &c, runtime); })) return;
    ckMid();
    uint64_t v0 = 0u;
    float f0 = 0.0f;
    uint64_t xo[5] = {(uint64_t)a1ck::ckBits(ctx->f[12]), (uint64_t)a1ck::ckBits(ctx->f[13]), (uint64_t)a1ck::ckBits(ctx->f[14]), (uint64_t)a1ck::ckBits(ctx->f[15]), Ps2GprU64(ctx, 4)};
    if (!ckLeg([&] { const a1::RetVF r = ty::glColor4f(rdram, runtime, pf12, pf13, pf14, pf15, xo); v0 = r.v0; f0 = r.f0; })) return;
    ckEnd(145, ctx, c, 4, v0, f0, 0x1au, 0xf006u, a1ck::ckBits(c.f[12]) != (uint32_t)xo[0] || a1ck::ckBits(c.f[13]) != (uint32_t)xo[1] || a1ck::ckBits(c.f[14]) != (uint32_t)xo[2] || a1ck::ckBits(c.f[15]) != (uint32_t)xo[3] || Ps2GprU64(&c, 4) != xo[4]);
}
void glColor4fv(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t p4)
{
    if (!ckBegin(146, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    if (!ckLeg([&] { ref::glColor4fv(rdram, &c, runtime); })) return;
    ckMid();
    uint64_t v0 = 0u;
    float f0 = 0.0f;
    uint64_t xo[5] = {(uint64_t)a1ck::ckBits(ctx->f[2]), (uint64_t)a1ck::ckBits(ctx->f[3]), (uint64_t)a1ck::ckBits(ctx->f[4]), (uint64_t)a1ck::ckBits(ctx->f[5]), Ps2GprU64(ctx, 5)};
    if (!ckLeg([&] { const a1::RetVF r = ty::glColor4fv(rdram, runtime, p4, xo); v0 = r.v0; f0 = r.f0; })) return;
    ckEnd(146, ctx, c, 4, v0, f0, 0x2au, 0x3eu, a1ck::ckBits(c.f[2]) != (uint32_t)xo[0] || a1ck::ckBits(c.f[3]) != (uint32_t)xo[1] || a1ck::ckBits(c.f[4]) != (uint32_t)xo[2] || a1ck::ckBits(c.f[5]) != (uint32_t)xo[3] || Ps2GprU64(&c, 5) != xo[4]);
}
void modelGetDlLayer(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint64_t p4, uint64_t p5, uint64_t p6)
{
    if (!ckBegin(147, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    if (!ckLeg([&] { ref::modelGetDlLayer(rdram, &c, runtime); })) return;
    ckMid();
    uint64_t v0 = 0u;
    float f0 = 0.0f;
    uint64_t xo[1] = {Ps2GprU64(ctx, 5)};
    if (!ckLeg([&] { v0 = (ty::modelGetDlLayer(rdram, runtime, p4, p5, p6, xo)); })) return;
    ckEnd(147, ctx, c, 2, v0, f0, 0x38u, 0x0u, Ps2GprU64(&c, 5) != xo[0]);
}
void modelResetSurface(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t p4)
{
    if (!ckBegin(148, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    if (!ckLeg([&] { ref::modelResetSurface(rdram, &c, runtime); })) return;
    ckMid();
    uint64_t v0 = 0u;
    float f0 = 0.0f;
    uint64_t xo[1] = {Ps2GprU64(ctx, 4)};
    if (!ckLeg([&] { v0 = a1_sx64(ty::modelResetSurface(rdram, runtime, p4, xo)); })) return;
    ckEnd(148, ctx, c, 1, v0, f0, 0x18u, 0x0u, Ps2GprU64(&c, 4) != xo[0]);
}
void modelSetMatrixTransTag(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint64_t p5, uint64_t p6, uint64_t p7)
{
    if (!ckBegin(149, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    if (!ckLeg([&] { ref::modelSetMatrixTransTag(rdram, &c, runtime); })) return;
    ckMid();
    uint64_t v0 = 0u;
    float f0 = 0.0f;
    uint64_t xo[3] = {Ps2GprU64(ctx, 9), Ps2GprU64(ctx, 10), Ps2GprU64(ctx, 11)};
    if (!ckLeg([&] { v0 = (ty::modelSetMatrixTransTag(rdram, runtime, p5, p6, p7, xo)); })) return;
    ckEnd(149, ctx, c, 2, v0, f0, 0x3f78u, 0x0u, Ps2GprU64(&c, 9) != xo[0] || Ps2GprU64(&c, 10) != xo[1] || Ps2GprU64(&c, 11) != xo[2]);
}
void ptclmanBuildMatPacket(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint64_t p4, float pf12, float pf13)
{
    if (!ckBegin(150, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    if (!ckLeg([&] { ref::ptclmanBuildMatPacket(rdram, &c, runtime); })) return;
    ckMid();
    uint64_t v0 = 0u;
    float f0 = 0.0f;
    uint64_t xo[8] = {Ps2GprU64(ctx, 4), Ps2GprU64(ctx, 5), Ps2GprU64(ctx, 6), Ps2GprU64(ctx, 7), Ps2GprU64(ctx, 8), Ps2GprU64(ctx, 9), Ps2GprU64(ctx, 10), Ps2GprU64(ctx, 11)};
    if (!ckLeg([&] { const a1::RetVF r = ty::ptclmanBuildMatPacket(rdram, runtime, p4, pf12, pf13, xo); v0 = r.v0; f0 = r.f0; })) return;
    ckEnd(150, ctx, c, 4, v0, f0, 0x1ffau, 0x0u, Ps2GprU64(&c, 4) != xo[0] || Ps2GprU64(&c, 5) != xo[1] || Ps2GprU64(&c, 6) != xo[2] || Ps2GprU64(&c, 7) != xo[3] || Ps2GprU64(&c, 8) != xo[4] || Ps2GprU64(&c, 9) != xo[5] || Ps2GprU64(&c, 10) != xo[6] || Ps2GprU64(&c, 11) != xo[7]);
}
void GetMeshClass(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint64_t p4)
{
    if (!ckBegin(151, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    if (!ckLeg([&] { ref::GetMeshClass(rdram, &c, runtime); })) return;
    ckMid();
    uint64_t v0 = 0u;
    float f0 = 0.0f;
    uint64_t xo[1] = {Ps2GprU64(ctx, 4)};
    if (!ckLeg([&] { v0 = (ty::GetMeshClass(rdram, runtime, p4, xo)); })) return;
    ckEnd(151, ctx, c, 2, v0, f0, 0x18u, 0x0u, Ps2GprU64(&c, 4) != xo[0]);
}
void meshFlip(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t p4)
{
    if (!ckBegin(152, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    if (!ckLeg([&] { ref::meshFlip(rdram, &c, runtime); })) return;
    ckMid();
    uint64_t v0 = 0u;
    float f0 = 0.0f;
    uint64_t xo[1] = {Ps2GprU64(ctx, 4)};
    if (!ckLeg([&] { v0 = a1_sx64(ty::meshFlip(rdram, runtime, p4, xo)); })) return;
    ckEnd(152, ctx, c, 1, v0, f0, 0x18u, 0x0u, Ps2GprU64(&c, 4) != xo[0]);
}
void spriteSetTextureCoordinateST(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint64_t p4, float pf12, float pf13, float pf14, float pf15)
{
    if (!ckBegin(153, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    if (!ckLeg([&] { ref::spriteSetTextureCoordinateST(rdram, &c, runtime); })) return;
    ckMid();
    uint64_t v0 = 0u;
    float f0 = 0.0f;
    uint64_t xo[2] = {(uint64_t)a1ck::ckBits(ctx->f[14]), (uint64_t)a1ck::ckBits(ctx->f[15])};
    if (!ckLeg([&] { v0 = (ty::spriteSetTextureCoordinateST(rdram, runtime, p4, pf12, pf13, pf14, pf15, xo)); })) return;
    ckEnd(153, ctx, c, 2, v0, f0, 0x8u, 0xc000u, a1ck::ckBits(c.f[14]) != (uint32_t)xo[0] || a1ck::ckBits(c.f[15]) != (uint32_t)xo[1]);
}
void spriteSetBlendMode(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint64_t p4, uint64_t p5, uint64_t p6, uint64_t p7)
{
    if (!ckBegin(154, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    if (!ckLeg([&] { ref::spriteSetBlendMode(rdram, &c, runtime); })) return;
    ckMid();
    uint64_t v0 = 0u;
    float f0 = 0.0f;
    uint64_t xo[3] = {Ps2GprU64(ctx, 6), Ps2GprU64(ctx, 7), Ps2GprU64(ctx, 8)};
    if (!ckLeg([&] { v0 = (ty::spriteSetBlendMode(rdram, runtime, p4, p5, p6, p7, xo)); })) return;
    ckEnd(154, ctx, c, 2, v0, f0, 0x1e8u, 0x0u, Ps2GprU64(&c, 6) != xo[0] || Ps2GprU64(&c, 7) != xo[1] || Ps2GprU64(&c, 8) != xo[2]);
}
void texGetTexTBPSize(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t p4, uint64_t p5, uint32_t p6)
{
    if (!ckBegin(155, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    if (!ckLeg([&] { ref::texGetTexTBPSize(rdram, &c, runtime); })) return;
    ckMid();
    uint64_t v0 = 0u;
    float f0 = 0.0f;
    uint64_t xo[1] = {Ps2GprU64(ctx, 7)};
    if (!ckLeg([&] { v0 = (ty::texGetTexTBPSize(rdram, runtime, p4, p5, p6, xo)); })) return;
    ckEnd(155, ctx, c, 2, v0, f0, 0xb8u, 0x0u, Ps2GprU64(&c, 7) != xo[0]);
}
void manager_sub_010AE658(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t p4, uint64_t p5, uint32_t p6, float pf12, float pf13)
{
    if (!ckBegin(156, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    if (!ckLeg([&] { ref::manager_sub_010AE658(rdram, &c, runtime); })) return;
    ckMid();
    uint64_t v0 = 0u;
    float f0 = 0.0f;
    uint64_t xo[3] = {(uint64_t)a1ck::ckBits(ctx->f[0]), Ps2GprU64(ctx, 9), Ps2GprU64(ctx, 10)};
    if (!ckLeg([&] { v0 = (ty::manager_sub_010AE658(rdram, runtime, p4, p5, p6, pf12, pf13, xo)); })) return;
    ckEnd(156, ctx, c, 2, v0, f0, 0x7e8u, 0x3u, a1ck::ckBits(c.f[0]) != (uint32_t)xo[0] || Ps2GprU64(&c, 9) != xo[1] || Ps2GprU64(&c, 10) != xo[2]);
}
void getBlendBaseMotion(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint64_t p4)
{
    if (!ckBegin(157, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    if (!ckLeg([&] { ref::getBlendBaseMotion(rdram, &c, runtime); })) return;
    ckMid();
    uint64_t v0 = 0u;
    float f0 = 0.0f;
    uint64_t xo[4] = {Ps2GprU64(ctx, 6), Ps2GprU64(ctx, 7), Ps2GprU64(ctx, 8), Ps2GprU64(ctx, 9)};
    if (!ckLeg([&] { v0 = (ty::getBlendBaseMotion(rdram, runtime, p4, xo)); })) return;
    ckEnd(157, ctx, c, 2, v0, f0, 0x3f8u, 0x0u, Ps2GprU64(&c, 6) != xo[0] || Ps2GprU64(&c, 7) != xo[1] || Ps2GprU64(&c, 8) != xo[2] || Ps2GprU64(&c, 9) != xo[3]);
}
void GetMotionFrameDirectResource(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t p4, uint32_t p5, uint64_t p6, float pf12, float pf13)
{
    if (!ckBegin(158, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    if (!ckLeg([&] { ref::GetMotionFrameDirectResource(rdram, &c, runtime); })) return;
    ckMid();
    uint64_t v0 = 0u;
    float f0 = 0.0f;
    if (!ckLeg([&] { const a1::RetVF r = ty::GetMotionFrameDirectResource(rdram, runtime, p4, p5, p6, pf12, pf13); v0 = r.v0; f0 = r.f0; })) return;
    ckEnd(158, ctx, c, 4, v0, f0, 0x48u, 0x1006u, false);
}
void GetMotionOptionNodeParentId(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t p4, uint32_t p5)
{
    if (!ckBegin(159, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    if (!ckLeg([&] { ref::GetMotionOptionNodeParentId(rdram, &c, runtime); })) return;
    ckMid();
    uint64_t v0 = 0u;
    float f0 = 0.0f;
    if (!ckLeg([&] { v0 = a1_sx64(ty::GetMotionOptionNodeParentId(rdram, runtime, p4, p5)); })) return;
    ckEnd(159, ctx, c, 1, v0, f0, 0x28u, 0x0u, false);
}
void InitMotionBoolInfo(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t p4)
{
    if (!ckBegin(160, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    if (!ckLeg([&] { ref::InitMotionBoolInfo(rdram, &c, runtime); })) return;
    ckMid();
    uint64_t v0 = 0u;
    float f0 = 0.0f;
    uint64_t xo[1] = {Ps2GprU64(ctx, 7)};
    if (!ckLeg([&] { v0 = a1_sx64(ty::InitMotionBoolInfo(rdram, runtime, p4, xo)); })) return;
    ckEnd(160, ctx, c, 1, v0, f0, 0xe8u, 0x0u, Ps2GprU64(&c, 7) != xo[0]);
}
void ResetCharacterClassIKInfo(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t p4)
{
    if (!ckBegin(161, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    if (!ckLeg([&] { ref::ResetCharacterClassIKInfo(rdram, &c, runtime); })) return;
    ckMid();
    uint64_t v0 = 0u;
    float f0 = 0.0f;
    uint64_t xo[1] = {(uint64_t)a1ck::ckBits(ctx->f[0])};
    if (!ckLeg([&] { v0 = a1_sx64(ty::ResetCharacterClassIKInfo(rdram, runtime, p4, xo)); })) return;
    ckEnd(161, ctx, c, 1, v0, f0, 0x1au, 0x3u, a1ck::ckBits(c.f[0]) != (uint32_t)xo[0]);
}
void GetCharacterClassCurAnimationCurFrame(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t p4)
{
    if (!ckBegin(162, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    if (!ckLeg([&] { ref::GetCharacterClassCurAnimationCurFrame(rdram, &c, runtime); })) return;
    ckMid();
    uint64_t v0 = 0u;
    float f0 = 0.0f;
    uint64_t xo[1] = {Ps2GprU64(ctx, 4)};
    if (!ckLeg([&] { f0 = ty::GetCharacterClassCurAnimationCurFrame(rdram, runtime, p4, xo); })) return;
    ckEnd(162, ctx, c, 3, v0, f0, 0x10u, 0x2u, Ps2GprU64(&c, 4) != xo[0]);
}
void SetCharacterObjControlBypassFunc(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint64_t p4, uint32_t p5)
{
    if (!ckBegin(163, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    if (!ckLeg([&] { ref::SetCharacterObjControlBypassFunc(rdram, &c, runtime); })) return;
    ckMid();
    uint64_t v0 = 0u;
    float f0 = 0.0f;
    uint64_t xo[1] = {Ps2GprU64(ctx, 6)};
    if (!ckLeg([&] { v0 = a1_sx64(ty::SetCharacterObjControlBypassFunc(rdram, runtime, p4, p5, xo)); })) return;
    ckEnd(163, ctx, c, 1, v0, f0, 0x58u, 0x0u, Ps2GprU64(&c, 6) != xo[0]);
}
void SetCharacterObjParentPlaneDiscontinuousJudgeFunc(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint64_t p4, uint32_t p5)
{
    if (!ckBegin(164, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    if (!ckLeg([&] { ref::SetCharacterObjParentPlaneDiscontinuousJudgeFunc(rdram, &c, runtime); })) return;
    ckMid();
    uint64_t v0 = 0u;
    float f0 = 0.0f;
    uint64_t xo[1] = {Ps2GprU64(ctx, 6)};
    if (!ckLeg([&] { v0 = a1_sx64(ty::SetCharacterObjParentPlaneDiscontinuousJudgeFunc(rdram, runtime, p4, p5, xo)); })) return;
    ckEnd(164, ctx, c, 1, v0, f0, 0x58u, 0x0u, Ps2GprU64(&c, 6) != xo[0]);
}
void SetCharacterObjParentPointXZFitGpChangeJudgeFunc(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint64_t p4, uint32_t p5, uint32_t p6)
{
    if (!ckBegin(165, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    if (!ckLeg([&] { ref::SetCharacterObjParentPointXZFitGpChangeJudgeFunc(rdram, &c, runtime); })) return;
    ckMid();
    uint64_t v0 = 0u;
    float f0 = 0.0f;
    uint64_t xo[1] = {Ps2GprU64(ctx, 7)};
    if (!ckLeg([&] { v0 = a1_sx64(ty::SetCharacterObjParentPointXZFitGpChangeJudgeFunc(rdram, runtime, p4, p5, p6, xo)); })) return;
    ckEnd(165, ctx, c, 1, v0, f0, 0x98u, 0x0u, Ps2GprU64(&c, 7) != xo[0]);
}
void manager_sub_010C9888(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t p4, uint32_t p5)
{
    if (!ckBegin(166, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    if (!ckLeg([&] { ref::manager_sub_010C9888(rdram, &c, runtime); })) return;
    ckMid();
    uint64_t v0 = 0u;
    float f0 = 0.0f;
    uint64_t xo[1] = {(uint64_t)a1ck::ckBits(ctx->f[0])};
    if (!ckLeg([&] { v0 = (ty::manager_sub_010C9888(rdram, runtime, p4, p5, xo)); })) return;
    ckEnd(166, ctx, c, 2, v0, f0, 0x3eau, 0x7u, a1ck::ckBits(c.f[0]) != (uint32_t)xo[0]);
}
void InitScriptControlButtonCallBack(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint64_t p4)
{
    if (!ckBegin(167, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    if (!ckLeg([&] { ref::InitScriptControlButtonCallBack(rdram, &c, runtime); })) return;
    ckMid();
    uint64_t v0 = 0u;
    float f0 = 0.0f;
    uint64_t xo[3] = {Ps2GprU64(ctx, 5), Ps2GprU64(ctx, 6), Ps2GprU64(ctx, 7)};
    if (!ckLeg([&] { v0 = (ty::InitScriptControlButtonCallBack(rdram, runtime, p4, xo)); })) return;
    ckEnd(167, ctx, c, 2, v0, f0, 0xe8u, 0x0u, Ps2GprU64(&c, 5) != xo[0] || Ps2GprU64(&c, 6) != xo[1] || Ps2GprU64(&c, 7) != xo[2]);
}
void manager_sub_010CD400(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t p4, float pf12)
{
    if (!ckBegin(168, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    if (!ckLeg([&] { ref::manager_sub_010CD400(rdram, &c, runtime); })) return;
    ckMid();
    uint64_t v0 = 0u;
    float f0 = 0.0f;
    uint64_t xo[3] = {(uint64_t)a1ck::ckBits(ctx->f[0]), Ps2GprU64(ctx, 2), Ps2GprU64(ctx, 5)};
    if (!ckLeg([&] { ty::manager_sub_010CD400(rdram, runtime, p4, pf12, xo); })) return;
    ckEnd(168, ctx, c, 0, v0, f0, 0x2cu, 0x3fu, a1ck::ckBits(c.f[0]) != (uint32_t)xo[0] || Ps2GprU64(&c, 2) != xo[1] || Ps2GprU64(&c, 5) != xo[2]);
}
void RequestCharacterClassMotionChangeWithParam(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t p4, uint32_t p5, uint32_t p6, float pf12)
{
    if (!ckBegin(169, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    if (!ckLeg([&] { ref::RequestCharacterClassMotionChangeWithParam(rdram, &c, runtime); })) return;
    ckMid();
    uint64_t v0 = 0u;
    float f0 = 0.0f;
    uint64_t xo[1] = {Ps2GprU64(ctx, 5)};
    if (!ckLeg([&] { v0 = a1_sx64(ty::RequestCharacterClassMotionChangeWithParam(rdram, runtime, p4, p5, p6, pf12, xo)); })) return;
    ckEnd(169, ctx, c, 1, v0, f0, 0x28u, 0x0u, Ps2GprU64(&c, 5) != xo[0]);
}
void charrootdyn_getLimitMode(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t p4, uint32_t p5)
{
    if (!ckBegin(170, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    if (!ckLeg([&] { ref::charrootdyn_getLimitMode(rdram, &c, runtime); })) return;
    ckMid();
    uint64_t v0 = 0u;
    float f0 = 0.0f;
    uint64_t xo[1] = {(uint64_t)a1ck::ckBits(ctx->f[0])};
    if (!ckLeg([&] { v0 = a1_sx64(ty::charrootdyn_getLimitMode(rdram, runtime, p4, p5, xo)); })) return;
    ckEnd(170, ctx, c, 1, v0, f0, 0x0u, 0x1u, a1ck::ckBits(c.f[0]) != (uint32_t)xo[0]);
}
void resetExpressionLocalHeightDiff(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t p4)
{
    if (!ckBegin(171, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    if (!ckLeg([&] { ref::resetExpressionLocalHeightDiff(rdram, &c, runtime); })) return;
    ckMid();
    uint64_t v0 = 0u;
    float f0 = 0.0f;
    if (!ckLeg([&] { v0 = a1_sx64(ty::resetExpressionLocalHeightDiff(rdram, runtime, p4)); })) return;
    ckEnd(171, ctx, c, 1, v0, f0, 0x28u, 0x0u, false);
}
void GetDemoOrientNext(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint64_t p4)
{
    if (!ckBegin(172, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    if (!ckLeg([&] { ref::GetDemoOrientNext(rdram, &c, runtime); })) return;
    ckMid();
    uint64_t v0 = 0u;
    float f0 = 0.0f;
    uint64_t xo[5] = {Ps2GprU64(ctx, 5), Ps2GprU64(ctx, 6), Ps2GprU64(ctx, 7), Ps2GprU64(ctx, 8), Ps2GprU64(ctx, 9)};
    if (!ckLeg([&] { v0 = a1_sx64(ty::GetDemoOrientNext(rdram, runtime, p4, xo)); })) return;
    ckEnd(172, ctx, c, 1, v0, f0, 0x3f8u, 0x0u, Ps2GprU64(&c, 5) != xo[0] || Ps2GprU64(&c, 6) != xo[1] || Ps2GprU64(&c, 7) != xo[2] || Ps2GprU64(&c, 8) != xo[3] || Ps2GprU64(&c, 9) != xo[4]);
}
void UpdateFogCol(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t p4, uint32_t p5)
{
    if (!ckBegin(173, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    if (!ckLeg([&] { ref::UpdateFogCol(rdram, &c, runtime); })) return;
    ckMid();
    uint64_t v0 = 0u;
    float f0 = 0.0f;
    uint64_t xo[1] = {Ps2GprU64(ctx, 2)};
    if (!ckLeg([&] { f0 = ty::UpdateFogCol(rdram, runtime, p4, p5, xo); })) return;
    ckEnd(173, ctx, c, 3, v0, f0, 0xeu, 0x6u, Ps2GprU64(&c, 2) != xo[0]);
}
void IsEnvelopeDistantLightType(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint64_t p4, uint32_t p5)
{
    if (!ckBegin(174, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    if (!ckLeg([&] { ref::IsEnvelopeDistantLightType(rdram, &c, runtime); })) return;
    ckMid();
    uint64_t v0 = 0u;
    float f0 = 0.0f;
    if (!ckLeg([&] { v0 = a1_sx64(ty::IsEnvelopeDistantLightType(rdram, runtime, p4, p5)); })) return;
    ckEnd(174, ctx, c, 1, v0, f0, 0x1e8u, 0x0u, false);
}
void globalLightCalcLightNum(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t p4, uint64_t p5)
{
    if (!ckBegin(175, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    if (!ckLeg([&] { ref::globalLightCalcLightNum(rdram, &c, runtime); })) return;
    ckMid();
    uint64_t v0 = 0u;
    float f0 = 0.0f;
    uint64_t xo[1] = {Ps2GprU64(ctx, 6)};
    if (!ckLeg([&] { v0 = a1_sx64(ty::globalLightCalcLightNum(rdram, runtime, p4, p5, xo)); })) return;
    ckEnd(175, ctx, c, 1, v0, f0, 0x48u, 0x0u, Ps2GprU64(&c, 6) != xo[0]);
}
void GetImpactWorstWorkLightClass(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t p4)
{
    if (!ckBegin(176, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    if (!ckLeg([&] { ref::GetImpactWorstWorkLightClass(rdram, &c, runtime); })) return;
    ckMid();
    uint64_t v0 = 0u;
    float f0 = 0.0f;
    if (!ckLeg([&] { v0 = a1_sx64(ty::GetImpactWorstWorkLightClass(rdram, runtime, p4)); })) return;
    ckEnd(176, ctx, c, 1, v0, f0, 0x38u, 0x3u, false);
}
void GetLwsOrientNodeNum(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t p4)
{
    if (!ckBegin(177, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    if (!ckLeg([&] { ref::GetLwsOrientNodeNum(rdram, &c, runtime); })) return;
    ckMid();
    uint64_t v0 = 0u;
    float f0 = 0.0f;
    uint64_t xo[2] = {Ps2GprU64(ctx, 7), Ps2GprU64(ctx, 8)};
    if (!ckLeg([&] { v0 = a1_sx64(ty::GetLwsOrientNodeNum(rdram, runtime, p4, xo)); })) return;
    ckEnd(177, ctx, c, 1, v0, f0, 0x1f8u, 0x0u, Ps2GprU64(&c, 7) != xo[0] || Ps2GprU64(&c, 8) != xo[1]);
}
void manager_sub_010FC618(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint64_t p4)
{
    if (!ckBegin(178, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    if (!ckLeg([&] { ref::manager_sub_010FC618(rdram, &c, runtime); })) return;
    ckMid();
    uint64_t v0 = 0u;
    float f0 = 0.0f;
    uint64_t xo[5] = {Ps2GprU64(ctx, 7), Ps2GprU64(ctx, 8), Ps2GprU64(ctx, 9), Ps2GprU64(ctx, 10), Ps2GprU64(ctx, 11)};
    if (!ckLeg([&] { v0 = (ty::manager_sub_010FC618(rdram, runtime, p4, xo)); })) return;
    ckEnd(178, ctx, c, 2, v0, f0, 0xff8u, 0x0u, Ps2GprU64(&c, 7) != xo[0] || Ps2GprU64(&c, 8) != xo[1] || Ps2GprU64(&c, 9) != xo[2] || Ps2GprU64(&c, 10) != xo[3] || Ps2GprU64(&c, 11) != xo[4]);
}
void initAnimationObjResource(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t p4)
{
    if (!ckBegin(179, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    if (!ckLeg([&] { ref::initAnimationObjResource(rdram, &c, runtime); })) return;
    ckMid();
    uint64_t v0 = 0u;
    float f0 = 0.0f;
    uint64_t xo[1] = {Ps2GprU64(ctx, 6)};
    if (!ckLeg([&] { v0 = a1_sx64(ty::initAnimationObjResource(rdram, runtime, p4, xo)); })) return;
    ckEnd(179, ctx, c, 1, v0, f0, 0x78u, 0x0u, Ps2GprU64(&c, 6) != xo[0]);
}
void InitBinarySize(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (!ckBegin(180, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    if (!ckLeg([&] { ref::InitBinarySize(rdram, &c, runtime); })) return;
    ckMid();
    uint64_t v0 = 0u;
    float f0 = 0.0f;
    if (!ckLeg([&] { v0 = a1_sx64(ty::InitBinarySize(rdram, runtime)); })) return;
    ckEnd(180, ctx, c, 1, v0, f0, 0x8u, 0x0u, false);
}
void SetScriptAnimationGlobalBaseInfo(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint64_t p4)
{
    if (!ckBegin(181, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    if (!ckLeg([&] { ref::SetScriptAnimationGlobalBaseInfo(rdram, &c, runtime); })) return;
    ckMid();
    uint64_t v0 = 0u;
    float f0 = 0.0f;
    uint64_t xo[3] = {Ps2GprU64(ctx, 5), Ps2GprU64(ctx, 6), Ps2GprU64(ctx, 7)};
    if (!ckLeg([&] { v0 = (ty::SetScriptAnimationGlobalBaseInfo(rdram, runtime, p4, xo)); })) return;
    ckEnd(181, ctx, c, 2, v0, f0, 0xe8u, 0x0u, Ps2GprU64(&c, 5) != xo[0] || Ps2GprU64(&c, 6) != xo[1] || Ps2GprU64(&c, 7) != xo[2]);
}
void CopyScriptAnimationWindGeometryPointer(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t p4, uint32_t p5)
{
    if (!ckBegin(182, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    if (!ckLeg([&] { ref::CopyScriptAnimationWindGeometryPointer(rdram, &c, runtime); })) return;
    ckMid();
    uint64_t v0 = 0u;
    float f0 = 0.0f;
    uint64_t xo[1] = {Ps2GprU64(ctx, 8)};
    if (!ckLeg([&] { v0 = (ty::CopyScriptAnimationWindGeometryPointer(rdram, runtime, p4, p5, xo)); })) return;
    ckEnd(182, ctx, c, 2, v0, f0, 0x1f8u, 0x0u, Ps2GprU64(&c, 8) != xo[0]);
}
void GetSpriteMenuClass(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint64_t p4)
{
    if (!ckBegin(183, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    if (!ckLeg([&] { ref::GetSpriteMenuClass(rdram, &c, runtime); })) return;
    ckMid();
    uint64_t v0 = 0u;
    float f0 = 0.0f;
    uint64_t xo[1] = {Ps2GprU64(ctx, 4)};
    if (!ckLeg([&] { v0 = (ty::GetSpriteMenuClass(rdram, runtime, p4, xo)); })) return;
    ckEnd(183, ctx, c, 2, v0, f0, 0x18u, 0x0u, Ps2GprU64(&c, 4) != xo[0]);
}
void SpriteMenuSetPadEventCallback(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t p4, uint32_t p5, uint32_t p6)
{
    if (!ckBegin(184, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    if (!ckLeg([&] { ref::SpriteMenuSetPadEventCallback(rdram, &c, runtime); })) return;
    ckMid();
    uint64_t v0 = 0u;
    float f0 = 0.0f;
    uint64_t xo[1] = {Ps2GprU64(ctx, 7)};
    if (!ckLeg([&] { v0 = a1_sx64(ty::SpriteMenuSetPadEventCallback(rdram, runtime, p4, p5, p6, xo)); })) return;
    ckEnd(184, ctx, c, 1, v0, f0, 0x98u, 0x0u, Ps2GprU64(&c, 7) != xo[0]);
}
void SpriteSubMenuSetPadEventCallback(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t p4, uint32_t p5, uint32_t p6)
{
    if (!ckBegin(185, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    if (!ckLeg([&] { ref::SpriteSubMenuSetPadEventCallback(rdram, &c, runtime); })) return;
    ckMid();
    uint64_t v0 = 0u;
    float f0 = 0.0f;
    uint64_t xo[1] = {Ps2GprU64(ctx, 7)};
    if (!ckLeg([&] { v0 = a1_sx64(ty::SpriteSubMenuSetPadEventCallback(rdram, runtime, p4, p5, p6, xo)); })) return;
    ckEnd(185, ctx, c, 1, v0, f0, 0x98u, 0x0u, Ps2GprU64(&c, 7) != xo[0]);
}
void SpriteMenuSetDispFlag(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t p4, uint32_t p5, uint64_t p6)
{
    if (!ckBegin(186, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    if (!ckLeg([&] { ref::SpriteMenuSetDispFlag(rdram, &c, runtime); })) return;
    ckMid();
    uint64_t v0 = 0u;
    float f0 = 0.0f;
    uint64_t xo[3] = {Ps2GprU64(ctx, 4), Ps2GprU64(ctx, 5), Ps2GprU64(ctx, 7)};
    if (!ckLeg([&] { v0 = a1_sx64(ty::SpriteMenuSetDispFlag(rdram, runtime, p4, p5, p6, xo)); })) return;
    ckEnd(186, ctx, c, 1, v0, f0, 0xb8u, 0x0u, Ps2GprU64(&c, 4) != xo[0] || Ps2GprU64(&c, 5) != xo[1] || Ps2GprU64(&c, 7) != xo[2]);
}
void SpriteMenuSetDispFlagByHash(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint64_t p4, uint64_t p5, uint64_t p6)
{
    if (!ckBegin(187, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    if (!ckLeg([&] { ref::SpriteMenuSetDispFlagByHash(rdram, &c, runtime); })) return;
    ckMid();
    uint64_t v0 = 0u;
    float f0 = 0.0f;
    uint64_t xo[6] = {Ps2GprU64(ctx, 5), Ps2GprU64(ctx, 7), Ps2GprU64(ctx, 8), Ps2GprU64(ctx, 9), Ps2GprU64(ctx, 10), Ps2GprU64(ctx, 11)};
    if (!ckLeg([&] { v0 = (ty::SpriteMenuSetDispFlagByHash(rdram, runtime, p4, p5, p6, xo)); })) return;
    ckEnd(187, ctx, c, 2, v0, f0, 0xfb8u, 0x0u, Ps2GprU64(&c, 5) != xo[0] || Ps2GprU64(&c, 7) != xo[1] || Ps2GprU64(&c, 8) != xo[2] || Ps2GprU64(&c, 9) != xo[3] || Ps2GprU64(&c, 10) != xo[4] || Ps2GprU64(&c, 11) != xo[5]);
}
void SpriteMenuSetLayer(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t p4, uint64_t p5)
{
    if (!ckBegin(188, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    if (!ckLeg([&] { ref::SpriteMenuSetLayer(rdram, &c, runtime); })) return;
    ckMid();
    uint64_t v0 = 0u;
    float f0 = 0.0f;
    uint64_t xo[4] = {Ps2GprU64(ctx, 4), Ps2GprU64(ctx, 6), Ps2GprU64(ctx, 7), Ps2GprU64(ctx, 8)};
    if (!ckLeg([&] { v0 = a1_sx64(ty::SpriteMenuSetLayer(rdram, runtime, p4, p5, xo)); })) return;
    ckEnd(188, ctx, c, 1, v0, f0, 0x1d8u, 0x0u, Ps2GprU64(&c, 4) != xo[0] || Ps2GprU64(&c, 6) != xo[1] || Ps2GprU64(&c, 7) != xo[2] || Ps2GprU64(&c, 8) != xo[3]);
}
void SpriteMenuGetLayerStatus(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t p4, uint32_t p5)
{
    if (!ckBegin(189, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    if (!ckLeg([&] { ref::SpriteMenuGetLayerStatus(rdram, &c, runtime); })) return;
    ckMid();
    uint64_t v0 = 0u;
    float f0 = 0.0f;
    uint64_t xo[1] = {Ps2GprU64(ctx, 6)};
    if (!ckLeg([&] { v0 = a1_sx64(ty::SpriteMenuGetLayerStatus(rdram, runtime, p4, p5, xo)); })) return;
    ckEnd(189, ctx, c, 1, v0, f0, 0x58u, 0x0u, Ps2GprU64(&c, 6) != xo[0]);
}
void SpriteMenuGetLayerByHash(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint64_t p4, uint64_t p5)
{
    if (!ckBegin(190, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    if (!ckLeg([&] { ref::SpriteMenuGetLayerByHash(rdram, &c, runtime); })) return;
    ckMid();
    uint64_t v0 = 0u;
    float f0 = 0.0f;
    uint64_t xo[4] = {Ps2GprU64(ctx, 6), Ps2GprU64(ctx, 7), Ps2GprU64(ctx, 8), Ps2GprU64(ctx, 9)};
    if (!ckLeg([&] { v0 = (ty::SpriteMenuGetLayerByHash(rdram, runtime, p4, p5, xo)); })) return;
    ckEnd(190, ctx, c, 2, v0, f0, 0x3d8u, 0x0u, Ps2GprU64(&c, 6) != xo[0] || Ps2GprU64(&c, 7) != xo[1] || Ps2GprU64(&c, 8) != xo[2] || Ps2GprU64(&c, 9) != xo[3]);
}
void SpriteMenuGetLayerNoByHash(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint64_t p4, uint64_t p5)
{
    if (!ckBegin(191, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    if (!ckLeg([&] { ref::SpriteMenuGetLayerNoByHash(rdram, &c, runtime); })) return;
    ckMid();
    uint64_t v0 = 0u;
    float f0 = 0.0f;
    uint64_t xo[4] = {Ps2GprU64(ctx, 4), Ps2GprU64(ctx, 6), Ps2GprU64(ctx, 7), Ps2GprU64(ctx, 8)};
    if (!ckLeg([&] { v0 = (ty::SpriteMenuGetLayerNoByHash(rdram, runtime, p4, p5, xo)); })) return;
    ckEnd(191, ctx, c, 2, v0, f0, 0x1d8u, 0x0u, Ps2GprU64(&c, 4) != xo[0] || Ps2GprU64(&c, 6) != xo[1] || Ps2GprU64(&c, 7) != xo[2] || Ps2GprU64(&c, 8) != xo[3]);
}
void SpriteMenuGetMenuIdByHash(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint64_t p4, uint64_t p5, uint64_t p6)
{
    if (!ckBegin(192, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    if (!ckLeg([&] { ref::SpriteMenuGetMenuIdByHash(rdram, &c, runtime); })) return;
    ckMid();
    uint64_t v0 = 0u;
    float f0 = 0.0f;
    uint64_t xo[6] = {Ps2GprU64(ctx, 5), Ps2GprU64(ctx, 7), Ps2GprU64(ctx, 8), Ps2GprU64(ctx, 9), Ps2GprU64(ctx, 10), Ps2GprU64(ctx, 11)};
    if (!ckLeg([&] { v0 = (ty::SpriteMenuGetMenuIdByHash(rdram, runtime, p4, p5, p6, xo)); })) return;
    ckEnd(192, ctx, c, 2, v0, f0, 0xfb8u, 0x0u, Ps2GprU64(&c, 5) != xo[0] || Ps2GprU64(&c, 7) != xo[1] || Ps2GprU64(&c, 8) != xo[2] || Ps2GprU64(&c, 9) != xo[3] || Ps2GprU64(&c, 10) != xo[4] || Ps2GprU64(&c, 11) != xo[5]);
}
void SpriteMenuGetSelectMenuIdBySpriteMenuClass(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t p4)
{
    if (!ckBegin(193, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    if (!ckLeg([&] { ref::SpriteMenuGetSelectMenuIdBySpriteMenuClass(rdram, &c, runtime); })) return;
    ckMid();
    uint64_t v0 = 0u;
    float f0 = 0.0f;
    uint64_t xo[1] = {Ps2GprU64(ctx, 5)};
    if (!ckLeg([&] { v0 = a1_sx64(ty::SpriteMenuGetSelectMenuIdBySpriteMenuClass(rdram, runtime, p4, xo)); })) return;
    ckEnd(193, ctx, c, 1, v0, f0, 0x28u, 0x0u, Ps2GprU64(&c, 5) != xo[0]);
}
void SpriteMenuSetSelectMenuId(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t p4, uint32_t p5)
{
    if (!ckBegin(194, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    if (!ckLeg([&] { ref::SpriteMenuSetSelectMenuId(rdram, &c, runtime); })) return;
    ckMid();
    uint64_t v0 = 0u;
    float f0 = 0.0f;
    uint64_t xo[2] = {Ps2GprU64(ctx, 5), Ps2GprU64(ctx, 6)};
    if (!ckLeg([&] { v0 = a1_sx64(ty::SpriteMenuSetSelectMenuId(rdram, runtime, p4, p5, xo)); })) return;
    ckEnd(194, ctx, c, 1, v0, f0, 0x78u, 0x0u, Ps2GprU64(&c, 5) != xo[0] || Ps2GprU64(&c, 6) != xo[1]);
}
void SpriteMenuGetSelectMenuMode(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t p4)
{
    if (!ckBegin(195, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    if (!ckLeg([&] { ref::SpriteMenuGetSelectMenuMode(rdram, &c, runtime); })) return;
    ckMid();
    uint64_t v0 = 0u;
    float f0 = 0.0f;
    uint64_t xo[1] = {Ps2GprU64(ctx, 5)};
    if (!ckLeg([&] { v0 = a1_sx64(ty::SpriteMenuGetSelectMenuMode(rdram, runtime, p4, xo)); })) return;
    ckEnd(195, ctx, c, 1, v0, f0, 0x38u, 0x0u, Ps2GprU64(&c, 5) != xo[0]);
}
void SpriteMenuSetDefaultSelectMenu(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t p4, uint32_t p5, uint32_t p6)
{
    if (!ckBegin(196, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    if (!ckLeg([&] { ref::SpriteMenuSetDefaultSelectMenu(rdram, &c, runtime); })) return;
    ckMid();
    uint64_t v0 = 0u;
    float f0 = 0.0f;
    uint64_t xo[4] = {Ps2GprU64(ctx, 4), Ps2GprU64(ctx, 5), Ps2GprU64(ctx, 6), Ps2GprU64(ctx, 7)};
    if (!ckLeg([&] { v0 = a1_sx64(ty::SpriteMenuSetDefaultSelectMenu(rdram, runtime, p4, p5, p6, xo)); })) return;
    ckEnd(196, ctx, c, 1, v0, f0, 0xf8u, 0x0u, Ps2GprU64(&c, 4) != xo[0] || Ps2GprU64(&c, 5) != xo[1] || Ps2GprU64(&c, 6) != xo[2] || Ps2GprU64(&c, 7) != xo[3]);
}
void SpriteMenuSetNumPartsFormatByPartsParam(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t p5, uint32_t p6)
{
    if (!ckBegin(197, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    if (!ckLeg([&] { ref::SpriteMenuSetNumPartsFormatByPartsParam(rdram, &c, runtime); })) return;
    ckMid();
    uint64_t v0 = 0u;
    float f0 = 0.0f;
    uint64_t xo[1] = {Ps2GprU64(ctx, 6)};
    if (!ckLeg([&] { v0 = a1_sx64(ty::SpriteMenuSetNumPartsFormatByPartsParam(rdram, runtime, p5, p6, xo)); })) return;
    ckEnd(197, ctx, c, 1, v0, f0, 0x48u, 0x0u, Ps2GprU64(&c, 6) != xo[0]);
}
void SpriteMenuSetAnimParam(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint64_t p4, uint64_t p5, uint32_t p6, uint64_t p7, float pf12)
{
    if (!ckBegin(198, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    if (!ckLeg([&] { ref::SpriteMenuSetAnimParam(rdram, &c, runtime); })) return;
    ckMid();
    uint64_t v0 = 0u;
    float f0 = 0.0f;
    uint64_t xo[1] = {Ps2GprU64(ctx, 2)};
    if (!ckLeg([&] { ty::SpriteMenuSetAnimParam(rdram, runtime, p4, p5, p6, p7, pf12, xo); })) return;
    ckEnd(198, ctx, c, 0, v0, f0, 0x11cu, 0x0u, Ps2GprU64(&c, 2) != xo[0]);
}
void GetSpriteClass(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint64_t p4)
{
    if (!ckBegin(199, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    if (!ckLeg([&] { ref::GetSpriteClass(rdram, &c, runtime); })) return;
    ckMid();
    uint64_t v0 = 0u;
    float f0 = 0.0f;
    uint64_t xo[1] = {Ps2GprU64(ctx, 4)};
    if (!ckLeg([&] { v0 = (ty::GetSpriteClass(rdram, runtime, p4, xo)); })) return;
    ckEnd(199, ctx, c, 2, v0, f0, 0x18u, 0x0u, Ps2GprU64(&c, 4) != xo[0]);
}
void ChangeCollisionResourceAttribute(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint64_t p4, uint64_t p5, uint64_t p6, uint64_t p7, uint64_t p8, uint64_t p9, uint64_t p10)
{
    if (!ckBegin(200, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    if (!ckLeg([&] { ref::ChangeCollisionResourceAttribute(rdram, &c, runtime); })) return;
    ckMid();
    uint64_t v0 = 0u;
    float f0 = 0.0f;
    if (!ckLeg([&] { v0 = (ty::ChangeCollisionResourceAttribute(rdram, runtime, p4, p5, p6, p7, p8, p9, p10)); })) return;
    ckEnd(200, ctx, c, 2, v0, f0, 0x3a18u, 0x0u, false);
}
void ConstructorCollisionObjectJudgeCallbackData(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t p4)
{
    if (!ckBegin(201, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    if (!ckLeg([&] { ref::ConstructorCollisionObjectJudgeCallbackData(rdram, &c, runtime); })) return;
    ckMid();
    uint64_t v0 = 0u;
    float f0 = 0.0f;
    if (!ckLeg([&] { v0 = a1_sx64(ty::ConstructorCollisionObjectJudgeCallbackData(rdram, runtime, p4)); })) return;
    ckEnd(201, ctx, c, 1, v0, f0, 0x10u, 0x0u, false);
}
void CopyCollisionFinder(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint64_t p4, uint64_t p5)
{
    if (!ckBegin(202, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    if (!ckLeg([&] { ref::CopyCollisionFinder(rdram, &c, runtime); })) return;
    ckMid();
    uint64_t v0 = 0u;
    float f0 = 0.0f;
    if (!ckLeg([&] { v0 = (ty::CopyCollisionFinder(rdram, runtime, p4, p5)); })) return;
    ckEnd(202, ctx, c, 2, v0, f0, 0x48u, 0x0u, false);
}
void GetGeometryPointerObjectParentNodeId(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint64_t p4)
{
    if (!ckBegin(203, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    if (!ckLeg([&] { ref::GetGeometryPointerObjectParentNodeId(rdram, &c, runtime); })) return;
    ckMid();
    uint64_t v0 = 0u;
    float f0 = 0.0f;
    uint64_t xo[1] = {Ps2GprU64(ctx, 5)};
    if (!ckLeg([&] { v0 = a1_sx64(ty::GetGeometryPointerObjectParentNodeId(rdram, runtime, p4, xo)); })) return;
    ckEnd(203, ctx, c, 1, v0, f0, 0x38u, 0x0u, Ps2GprU64(&c, 5) != xo[0]);
}
void GetClothWorkSize(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint64_t p4)
{
    if (!ckBegin(204, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    if (!ckLeg([&] { ref::GetClothWorkSize(rdram, &c, runtime); })) return;
    ckMid();
    uint64_t v0 = 0u;
    float f0 = 0.0f;
    if (!ckLeg([&] { v0 = a1_sx64(ty::GetClothWorkSize(rdram, runtime, p4)); })) return;
    ckEnd(204, ctx, c, 1, v0, f0, 0xf8u, 0x0u, false);
}
void manager_sub_01140630(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (!ckBegin(205, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    if (!ckLeg([&] { ref::manager_sub_01140630(rdram, &c, runtime); })) return;
    ckMid();
    uint64_t v0 = 0u;
    float f0 = 0.0f;
    uint64_t xo[3] = {Ps2GprU64(ctx, 5), Ps2GprU64(ctx, 7), Ps2GprU64(ctx, 8)};
    if (!ckLeg([&] { v0 = a1_sx64(ty::manager_sub_01140630(rdram, runtime, xo)); })) return;
    ckEnd(205, ctx, c, 1, v0, f0, 0x1f8u, 0x0u, Ps2GprU64(&c, 5) != xo[0] || Ps2GprU64(&c, 7) != xo[1] || Ps2GprU64(&c, 8) != xo[2]);
}
void manager_sub_011408E0(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint64_t p4, uint32_t p5)
{
    if (!ckBegin(206, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    if (!ckLeg([&] { ref::manager_sub_011408E0(rdram, &c, runtime); })) return;
    ckMid();
    uint64_t v0 = 0u;
    float f0 = 0.0f;
    uint64_t xo[4] = {Ps2GprU64(ctx, 6), Ps2GprU64(ctx, 7), Ps2GprU64(ctx, 8), Ps2GprU64(ctx, 9)};
    if (!ckLeg([&] { v0 = (ty::manager_sub_011408E0(rdram, runtime, p4, p5, xo)); })) return;
    ckEnd(206, ctx, c, 2, v0, f0, 0x3f8u, 0x0u, Ps2GprU64(&c, 6) != xo[0] || Ps2GprU64(&c, 7) != xo[1] || Ps2GprU64(&c, 8) != xo[2] || Ps2GprU64(&c, 9) != xo[3]);
}
void manager_sub_01140E60(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t p4, uint32_t p5, uint32_t p6)
{
    if (!ckBegin(207, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    if (!ckLeg([&] { ref::manager_sub_01140E60(rdram, &c, runtime); })) return;
    ckMid();
    uint64_t v0 = 0u;
    float f0 = 0.0f;
    if (!ckLeg([&] { v0 = a1_sx64(ty::manager_sub_01140E60(rdram, runtime, p4, p5, p6)); })) return;
    ckEnd(207, ctx, c, 1, v0, f0, 0x8u, 0x0u, false);
}
void manager_sub_01140FD8(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t p4, uint32_t p5)
{
    if (!ckBegin(208, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    if (!ckLeg([&] { ref::manager_sub_01140FD8(rdram, &c, runtime); })) return;
    ckMid();
    uint64_t v0 = 0u;
    float f0 = 0.0f;
    if (!ckLeg([&] { const a1::RetVF r = ty::manager_sub_01140FD8(rdram, runtime, p4, p5); v0 = r.v0; f0 = r.f0; })) return;
    ckEnd(208, ctx, c, 4, v0, f0, 0xau, 0x2u, false);
}
void manager_sub_011418D8(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t p4, uint32_t p5, uint32_t p6)
{
    if (!ckBegin(209, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    if (!ckLeg([&] { ref::manager_sub_011418D8(rdram, &c, runtime); })) return;
    ckMid();
    uint64_t v0 = 0u;
    float f0 = 0.0f;
    uint64_t xo[1] = {(uint64_t)a1ck::ckBits(ctx->f[0])};
    if (!ckLeg([&] { v0 = a1_sx64(ty::manager_sub_011418D8(rdram, runtime, p4, p5, p6, xo)); })) return;
    ckEnd(209, ctx, c, 1, v0, f0, 0x8u, 0x1u, a1ck::ckBits(c.f[0]) != (uint32_t)xo[0]);
}
void manager_sub_01141E80(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t p4)
{
    if (!ckBegin(210, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    if (!ckLeg([&] { ref::manager_sub_01141E80(rdram, &c, runtime); })) return;
    ckMid();
    uint64_t v0 = 0u;
    float f0 = 0.0f;
    if (!ckLeg([&] { v0 = a1_sx64(ty::manager_sub_01141E80(rdram, runtime, p4)); })) return;
    ckEnd(210, ctx, c, 1, v0, f0, 0x8u, 0x0u, false);
}
void get_particle_parameter_blend_disp(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t p4, uint32_t p5, uint64_t p6, uint64_t p7, float pf12)
{
    if (!ckBegin(211, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    if (!ckLeg([&] { ref::get_particle_parameter_blend_disp(rdram, &c, runtime); })) return;
    ckMid();
    uint64_t v0 = 0u;
    float f0 = 0.0f;
    uint64_t xo[2] = {Ps2GprU64(ctx, 8), Ps2GprU64(ctx, 9)};
    if (!ckLeg([&] { const a1::RetVF r = ty::get_particle_parameter_blend_disp(rdram, runtime, p4, p5, p6, p7, pf12, xo); v0 = r.v0; f0 = r.f0; })) return;
    ckEnd(211, ctx, c, 4, v0, f0, 0x32au, 0xeu, Ps2GprU64(&c, 8) != xo[0] || Ps2GprU64(&c, 9) != xo[1]);
}
void InitTwoWayLink(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t p4, uint32_t p5)
{
    if (!ckBegin(212, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    if (!ckLeg([&] { ref::InitTwoWayLink(rdram, &c, runtime); })) return;
    ckMid();
    uint64_t v0 = 0u;
    float f0 = 0.0f;
    if (!ckLeg([&] { ty::InitTwoWayLink(rdram, runtime, p4, p5); })) return;
    ckEnd(212, ctx, c, 0, v0, f0, 0x0u, 0x0u, false);
}
void AddTwoWayLinkNext(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t p4, uint32_t p5)
{
    if (!ckBegin(213, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    if (!ckLeg([&] { ref::AddTwoWayLinkNext(rdram, &c, runtime); })) return;
    ckMid();
    uint64_t v0 = 0u;
    float f0 = 0.0f;
    if (!ckLeg([&] { v0 = a1_sx64(ty::AddTwoWayLinkNext(rdram, runtime, p4, p5)); })) return;
    ckEnd(213, ctx, c, 1, v0, f0, 0x0u, 0x0u, false);
}
void DeleteTwoWayLink(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t p4)
{
    if (!ckBegin(214, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    if (!ckLeg([&] { ref::DeleteTwoWayLink(rdram, &c, runtime); })) return;
    ckMid();
    uint64_t v0 = 0u;
    float f0 = 0.0f;
    if (!ckLeg([&] { v0 = a1_sx64(ty::DeleteTwoWayLink(rdram, runtime, p4)); })) return;
    ckEnd(214, ctx, c, 1, v0, f0, 0x8u, 0x0u, false);
}
void adpcmSetPan(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t p4, float pf12)
{
    if (!ckBegin(215, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    if (!ckLeg([&] { ref::adpcmSetPan(rdram, &c, runtime); })) return;
    ckMid();
    uint64_t v0 = 0u;
    float f0 = 0.0f;
    uint64_t xo[1] = {Ps2GprU64(ctx, 4)};
    if (!ckLeg([&] { v0 = a1_sx64(ty::adpcmSetPan(rdram, runtime, p4, pf12, xo)); })) return;
    ckEnd(215, ctx, c, 1, v0, f0, 0x12u, 0x0u, Ps2GprU64(&c, 4) != xo[0]);
}
void dsysFileInit(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (!ckBegin(216, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    if (!ckLeg([&] { ref::dsysFileInit(rdram, &c, runtime); })) return;
    ckMid();
    uint64_t v0 = 0u;
    float f0 = 0.0f;
    uint64_t xo[2] = {Ps2GprU64(ctx, 8), Ps2GprU64(ctx, 9)};
    if (!ckLeg([&] { v0 = a1_sx64(ty::dsysFileInit(rdram, runtime, xo)); })) return;
    ckEnd(216, ctx, c, 1, v0, f0, 0x3f8u, 0x0u, Ps2GprU64(&c, 8) != xo[0] || Ps2GprU64(&c, 9) != xo[1]);
}
void sheetFileGetDataAdrs(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t p4)
{
    if (!ckBegin(217, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    if (!ckLeg([&] { ref::sheetFileGetDataAdrs(rdram, &c, runtime); })) return;
    ckMid();
    uint64_t v0 = 0u;
    float f0 = 0.0f;
    uint64_t xo[1] = {Ps2GprU64(ctx, 5)};
    if (!ckLeg([&] { v0 = a1_sx64(ty::sheetFileGetDataAdrs(rdram, runtime, p4, xo)); })) return;
    ckEnd(217, ctx, c, 1, v0, f0, 0x38u, 0x0u, Ps2GprU64(&c, 5) != xo[0]);
}
void sheetMgrSheetHeaderSearchSheetDataObj(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t p4, uint64_t p5)
{
    if (!ckBegin(218, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    if (!ckLeg([&] { ref::sheetMgrSheetHeaderSearchSheetDataObj(rdram, &c, runtime); })) return;
    ckMid();
    uint64_t v0 = 0u;
    float f0 = 0.0f;
    if (!ckLeg([&] { v0 = a1_sx64(ty::sheetMgrSheetHeaderSearchSheetDataObj(rdram, runtime, p4, p5)); })) return;
    ckEnd(218, ctx, c, 1, v0, f0, 0x18u, 0x0u, false);
}
void manager_sub_011629E8(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint64_t p4)
{
    if (!ckBegin(219, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    if (!ckLeg([&] { ref::manager_sub_011629E8(rdram, &c, runtime); })) return;
    ckMid();
    uint64_t v0 = 0u;
    float f0 = 0.0f;
    uint64_t xo[1] = {Ps2GprU64(ctx, 6)};
    if (!ckLeg([&] { v0 = a1_sx64(ty::manager_sub_011629E8(rdram, runtime, p4, xo)); })) return;
    ckEnd(219, ctx, c, 1, v0, f0, 0x68u, 0x0u, Ps2GprU64(&c, 6) != xo[0]);
}
void get_hit_region_ofst_camera_region(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint64_t p4)
{
    if (!ckBegin(220, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    if (!ckLeg([&] { ref::get_hit_region_ofst_camera_region(rdram, &c, runtime); })) return;
    ckMid();
    uint64_t v0 = 0u;
    float f0 = 0.0f;
    uint64_t xo[1] = {Ps2GprU64(ctx, 2)};
    if (!ckLeg([&] { f0 = ty::get_hit_region_ofst_camera_region(rdram, runtime, p4, xo); })) return;
    ckEnd(220, ctx, c, 3, v0, f0, 0xcu, 0x0u, Ps2GprU64(&c, 2) != xo[0]);
}
void set_scissor_screen_size(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint64_t p4, uint32_t p5, uint32_t p6, uint32_t p7)
{
    if (!ckBegin(221, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    if (!ckLeg([&] { ref::set_scissor_screen_size(rdram, &c, runtime); })) return;
    ckMid();
    uint64_t v0 = 0u;
    float f0 = 0.0f;
    uint64_t xo[1] = {Ps2GprU64(ctx, 5)};
    if (!ckLeg([&] { v0 = a1_sx64(ty::set_scissor_screen_size(rdram, runtime, p4, p5, p6, p7, xo)); })) return;
    ckEnd(221, ctx, c, 1, v0, f0, 0x30u, 0x0u, Ps2GprU64(&c, 5) != xo[0]);
}
void DecreaseDamageLockTime(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t p4, float pf12)
{
    if (!ckBegin(222, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    if (!ckLeg([&] { ref::DecreaseDamageLockTime(rdram, &c, runtime); })) return;
    ckMid();
    uint64_t v0 = 0u;
    float f0 = 0.0f;
    uint64_t xo[1] = {(uint64_t)a1ck::ckBits(ctx->f[0])};
    if (!ckLeg([&] { v0 = a1_sx64(ty::DecreaseDamageLockTime(rdram, runtime, p4, pf12, xo)); })) return;
    ckEnd(222, ctx, c, 1, v0, f0, 0x0u, 0x3u, a1ck::ckBits(c.f[0]) != (uint32_t)xo[0]);
}
void GetDamagerClassDamageSheetType(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t p4)
{
    if (!ckBegin(223, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    if (!ckLeg([&] { ref::GetDamagerClassDamageSheetType(rdram, &c, runtime); })) return;
    ckMid();
    uint64_t v0 = 0u;
    float f0 = 0.0f;
    uint64_t xo[1] = {Ps2GprU64(ctx, 5)};
    if (!ckLeg([&] { v0 = a1_sx64(ty::GetDamagerClassDamageSheetType(rdram, runtime, p4, xo)); })) return;
    ckEnd(223, ctx, c, 1, v0, f0, 0x28u, 0x0u, Ps2GprU64(&c, 5) != xo[0]);
}
void ResetAnimeFrameTableInfo(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint64_t p4, uint64_t p5, uint64_t p6)
{
    if (!ckBegin(224, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    if (!ckLeg([&] { ref::ResetAnimeFrameTableInfo(rdram, &c, runtime); })) return;
    ckMid();
    uint64_t v0 = 0u;
    float f0 = 0.0f;
    uint64_t xo[3] = {Ps2GprU64(ctx, 6), Ps2GprU64(ctx, 7), Ps2GprU64(ctx, 8)};
    if (!ckLeg([&] { v0 = (ty::ResetAnimeFrameTableInfo(rdram, runtime, p4, p5, p6, xo)); })) return;
    ckEnd(224, ctx, c, 2, v0, f0, 0x1f8u, 0x0u, Ps2GprU64(&c, 6) != xo[0] || Ps2GprU64(&c, 7) != xo[1] || Ps2GprU64(&c, 8) != xo[2]);
}
void rintf(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, float pf12)
{
    if (!ckBegin(225, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    if (!ckLeg([&] { ref::rintf(rdram, &c, runtime); })) return;
    ckMid();
    uint64_t v0 = 0u;
    float f0 = 0.0f;
    uint64_t xo[7] = {(uint64_t)a1ck::ckBits(ctx->f[12]), Ps2GprU64(ctx, 4), Ps2GprU64(ctx, 5), Ps2GprU64(ctx, 6), Ps2GprU64(ctx, 7), Ps2GprU64(ctx, 8), Ps2GprU64(ctx, 9)};
    if (!ckLeg([&] { const a1::RetVF r = ty::rintf(rdram, runtime, pf12, xo); v0 = r.v0; f0 = r.f0; })) return;
    ckEnd(225, ctx, c, 4, v0, f0, 0x3fau, 0x1002u, a1ck::ckBits(c.f[12]) != (uint32_t)xo[0] || Ps2GprU64(&c, 4) != xo[1] || Ps2GprU64(&c, 5) != xo[2] || Ps2GprU64(&c, 6) != xo[3] || Ps2GprU64(&c, 7) != xo[4] || Ps2GprU64(&c, 8) != xo[5] || Ps2GprU64(&c, 9) != xo[6]);
}
void GAMEUTIL_set_parameter_look_at_vs_offset(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t p4, float pf12, float pf13, float pf14, float pf15, float pf16)
{
    if (!ckBegin(226, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    if (!ckLeg([&] { ref::GAMEUTIL_set_parameter_look_at_vs_offset(rdram, &c, runtime); })) return;
    ckMid();
    uint64_t v0 = 0u;
    float f0 = 0.0f;
    uint64_t xo[3] = {(uint64_t)a1ck::ckBits(ctx->f[12]), (uint64_t)a1ck::ckBits(ctx->f[13]), (uint64_t)a1ck::ckBits(ctx->f[14])};
    if (!ckLeg([&] { f0 = ty::GAMEUTIL_set_parameter_look_at_vs_offset(rdram, runtime, p4, pf12, pf13, pf14, pf15, pf16, xo); })) return;
    ckEnd(226, ctx, c, 3, v0, f0, 0x2u, 0x7000u, a1ck::ckBits(c.f[12]) != (uint32_t)xo[0] || a1ck::ckBits(c.f[13]) != (uint32_t)xo[1] || a1ck::ckBits(c.f[14]) != (uint32_t)xo[2]);
}
void GAMEUTIL_doit_status_manager(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t p4, uint64_t p5, uint64_t p6, float pf12)
{
    if (!ckBegin(227, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    if (!ckLeg([&] { ref::GAMEUTIL_doit_status_manager(rdram, &c, runtime); })) return;
    ckMid();
    uint64_t v0 = 0u;
    float f0 = 0.0f;
    uint64_t xo[1] = {(uint64_t)a1ck::ckBits(ctx->f[0])};
    if (!ckLeg([&] { v0 = a1_sx64(ty::GAMEUTIL_doit_status_manager(rdram, runtime, p4, p5, p6, pf12, xo)); })) return;
    ckEnd(227, ctx, c, 1, v0, f0, 0x0u, 0x1u, a1ck::ckBits(c.f[0]) != (uint32_t)xo[0]);
}
void ReleaseGame(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint64_t p4)
{
    if (!ckBegin(228, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    if (!ckLeg([&] { ref::ReleaseGame(rdram, &c, runtime); })) return;
    ckMid();
    uint64_t v0 = 0u;
    float f0 = 0.0f;
    uint64_t xo[1] = {Ps2GprU64(ctx, 4)};
    if (!ckLeg([&] { v0 = a1_sx64(ty::ReleaseGame(rdram, runtime, p4, xo)); })) return;
    ckEnd(228, ctx, c, 1, v0, f0, 0x18u, 0x0u, Ps2GprU64(&c, 4) != xo[0]);
}
void getDemoBarFade(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (!ckBegin(229, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    if (!ckLeg([&] { ref::getDemoBarFade(rdram, &c, runtime); })) return;
    ckMid();
    uint64_t v0 = 0u;
    float f0 = 0.0f;
    uint64_t xo[1] = {(uint64_t)a1ck::ckBits(ctx->f[1])};
    if (!ckLeg([&] { const a1::RetVF r = ty::getDemoBarFade(rdram, runtime, xo); v0 = r.v0; f0 = r.f0; })) return;
    ckEnd(229, ctx, c, 4, v0, f0, 0x2u, 0x2u, a1ck::ckBits(c.f[1]) != (uint32_t)xo[0]);
}
void set_base_pitch_parameter_nico_yaw_pitch_cntrl(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t p4, uint32_t p5, float pf12, float pf13, float pf14, float pf15, float pf16, float pf17)
{
    if (!ckBegin(230, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    if (!ckLeg([&] { ref::set_base_pitch_parameter_nico_yaw_pitch_cntrl(rdram, &c, runtime); })) return;
    ckMid();
    uint64_t v0 = 0u;
    float f0 = 0.0f;
    uint64_t xo[3] = {(uint64_t)a1ck::ckBits(ctx->f[14]), (uint64_t)a1ck::ckBits(ctx->f[15]), (uint64_t)a1ck::ckBits(ctx->f[16])};
    if (!ckLeg([&] { f0 = ty::set_base_pitch_parameter_nico_yaw_pitch_cntrl(rdram, runtime, p4, p5, pf12, pf13, pf14, pf15, pf16, pf17, xo); })) return;
    ckEnd(230, ctx, c, 3, v0, f0, 0x2u, 0x1f000u, a1ck::ckBits(c.f[14]) != (uint32_t)xo[0] || a1ck::ckBits(c.f[15]) != (uint32_t)xo[1] || a1ck::ckBits(c.f[16]) != (uint32_t)xo[2]);
}
void gamecore_sub_01347B28(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t p4, float pf12, float pf13)
{
    if (!ckBegin(231, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    if (!ckLeg([&] { ref::gamecore_sub_01347B28(rdram, &c, runtime); })) return;
    ckMid();
    uint64_t v0 = 0u;
    float f0 = 0.0f;
    uint64_t xo[1] = {(uint64_t)a1ck::ckBits(ctx->f[0])};
    if (!ckLeg([&] { v0 = (ty::gamecore_sub_01347B28(rdram, runtime, p4, pf12, pf13, xo)); })) return;
    ckEnd(231, ctx, c, 2, v0, f0, 0x28u, 0x3u, a1ck::ckBits(c.f[0]) != (uint32_t)xo[0]);
}
void gamecore_sub_01347B88(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t p4, uint32_t p5, uint32_t p6)
{
    if (!ckBegin(232, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    if (!ckLeg([&] { ref::gamecore_sub_01347B88(rdram, &c, runtime); })) return;
    ckMid();
    uint64_t v0 = 0u;
    float f0 = 0.0f;
    uint64_t xo[2] = {Ps2GprU64(ctx, 2), Ps2GprU64(ctx, 7)};
    if (!ckLeg([&] { f0 = ty::gamecore_sub_01347B88(rdram, runtime, p4, p5, p6, xo); })) return;
    ckEnd(232, ctx, c, 3, v0, f0, 0x8cu, 0xeu, Ps2GprU64(&c, 2) != xo[0] || Ps2GprU64(&c, 7) != xo[1]);
}
void backStageCheckPointSave(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint64_t p4)
{
    if (!ckBegin(233, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    if (!ckLeg([&] { ref::backStageCheckPointSave(rdram, &c, runtime); })) return;
    ckMid();
    uint64_t v0 = 0u;
    float f0 = 0.0f;
    uint64_t xo[5] = {Ps2GprU64(ctx, 4), Ps2GprU64(ctx, 5), Ps2GprU64(ctx, 6), Ps2GprU64(ctx, 7), Ps2GprU64(ctx, 8)};
    if (!ckLeg([&] { v0 = (ty::backStageCheckPointSave(rdram, runtime, p4, xo)); })) return;
    ckEnd(233, ctx, c, 2, v0, f0, 0x1f8u, 0x0u, Ps2GprU64(&c, 4) != xo[0] || Ps2GprU64(&c, 5) != xo[1] || Ps2GprU64(&c, 6) != xo[2] || Ps2GprU64(&c, 7) != xo[3] || Ps2GprU64(&c, 8) != xo[4]);
}
void backStageCheckPointLoad(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint64_t p4)
{
    if (!ckBegin(234, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    if (!ckLeg([&] { ref::backStageCheckPointLoad(rdram, &c, runtime); })) return;
    ckMid();
    uint64_t v0 = 0u;
    float f0 = 0.0f;
    uint64_t xo[5] = {Ps2GprU64(ctx, 4), Ps2GprU64(ctx, 5), Ps2GprU64(ctx, 6), Ps2GprU64(ctx, 7), Ps2GprU64(ctx, 8)};
    if (!ckLeg([&] { v0 = (ty::backStageCheckPointLoad(rdram, runtime, p4, xo)); })) return;
    ckEnd(234, ctx, c, 2, v0, f0, 0x1f8u, 0x0u, Ps2GprU64(&c, 4) != xo[0] || Ps2GprU64(&c, 5) != xo[1] || Ps2GprU64(&c, 6) != xo[2] || Ps2GprU64(&c, 7) != xo[3] || Ps2GprU64(&c, 8) != xo[4]);
}
void _optionWorkSet(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t p4, uint32_t p5)
{
    if (!ckBegin(235, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    if (!ckLeg([&] { ref::_optionWorkSet(rdram, &c, runtime); })) return;
    ckMid();
    uint64_t v0 = 0u;
    float f0 = 0.0f;
    uint64_t xo[1] = {Ps2GprU64(ctx, 4)};
    if (!ckLeg([&] { ty::_optionWorkSet(rdram, runtime, p4, p5, xo); })) return;
    ckEnd(235, ctx, c, 0, v0, f0, 0x12u, 0x0u, Ps2GprU64(&c, 4) != xo[0]);
}
void _gameFlagSet(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint64_t p4)
{
    if (!ckBegin(236, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    if (!ckLeg([&] { ref::_gameFlagSet(rdram, &c, runtime); })) return;
    ckMid();
    uint64_t v0 = 0u;
    float f0 = 0.0f;
    uint64_t xo[4] = {Ps2GprU64(ctx, 1), Ps2GprU64(ctx, 4), Ps2GprU64(ctx, 5), Ps2GprU64(ctx, 6)};
    if (!ckLeg([&] { v0 = (ty::_gameFlagSet(rdram, runtime, p4, xo)); })) return;
    ckEnd(236, ctx, c, 2, v0, f0, 0x7au, 0x0u, Ps2GprU64(&c, 1) != xo[0] || Ps2GprU64(&c, 4) != xo[1] || Ps2GprU64(&c, 5) != xo[2] || Ps2GprU64(&c, 6) != xo[3]);
}
void _gameFlagCls(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint64_t p4)
{
    if (!ckBegin(237, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    if (!ckLeg([&] { ref::_gameFlagCls(rdram, &c, runtime); })) return;
    ckMid();
    uint64_t v0 = 0u;
    float f0 = 0.0f;
    uint64_t xo[3] = {Ps2GprU64(ctx, 4), Ps2GprU64(ctx, 5), Ps2GprU64(ctx, 6)};
    if (!ckLeg([&] { v0 = (ty::_gameFlagCls(rdram, runtime, p4, xo)); })) return;
    ckEnd(237, ctx, c, 2, v0, f0, 0x7au, 0x0u, Ps2GprU64(&c, 4) != xo[0] || Ps2GprU64(&c, 5) != xo[1] || Ps2GprU64(&c, 6) != xo[2]);
}
void _gameFlagChk(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint64_t p4)
{
    if (!ckBegin(238, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    if (!ckLeg([&] { ref::_gameFlagChk(rdram, &c, runtime); })) return;
    ckMid();
    uint64_t v0 = 0u;
    float f0 = 0.0f;
    uint64_t xo[2] = {Ps2GprU64(ctx, 4), Ps2GprU64(ctx, 5)};
    if (!ckLeg([&] { v0 = (ty::_gameFlagChk(rdram, runtime, p4, xo)); })) return;
    ckEnd(238, ctx, c, 2, v0, f0, 0x38u, 0x0u, Ps2GprU64(&c, 4) != xo[0] || Ps2GprU64(&c, 5) != xo[1]);
}
void _gameWorkSet(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t p4, uint32_t p5)
{
    if (!ckBegin(239, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    if (!ckLeg([&] { ref::_gameWorkSet(rdram, &c, runtime); })) return;
    ckMid();
    uint64_t v0 = 0u;
    float f0 = 0.0f;
    uint64_t xo[1] = {Ps2GprU64(ctx, 1)};
    if (!ckLeg([&] { ty::_gameWorkSet(rdram, runtime, p4, p5, xo); })) return;
    ckEnd(239, ctx, c, 0, v0, f0, 0x12u, 0x0u, Ps2GprU64(&c, 1) != xo[0]);
}
void _gameFWorkSet(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t p4, float pf12)
{
    if (!ckBegin(240, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    if (!ckLeg([&] { ref::_gameFWorkSet(rdram, &c, runtime); })) return;
    ckMid();
    uint64_t v0 = 0u;
    float f0 = 0.0f;
    uint64_t xo[1] = {Ps2GprU64(ctx, 4)};
    if (!ckLeg([&] { v0 = a1_sx64(ty::_gameFWorkSet(rdram, runtime, p4, pf12, xo)); })) return;
    ckEnd(240, ctx, c, 1, v0, f0, 0x18u, 0x0u, Ps2GprU64(&c, 4) != xo[0]);
}
void _gameFWorkChk(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t p4)
{
    if (!ckBegin(241, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    if (!ckLeg([&] { ref::_gameFWorkChk(rdram, &c, runtime); })) return;
    ckMid();
    uint64_t v0 = 0u;
    float f0 = 0.0f;
    uint64_t xo[1] = {Ps2GprU64(ctx, 4)};
    if (!ckLeg([&] { const a1::RetVF r = ty::_gameFWorkChk(rdram, runtime, p4, xo); v0 = r.v0; f0 = r.f0; })) return;
    ckEnd(241, ctx, c, 4, v0, f0, 0x18u, 0x0u, Ps2GprU64(&c, 4) != xo[0]);
}
void fruitsFlagChk(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint64_t p4, uint64_t p5, uint32_t p7)
{
    if (!ckBegin(242, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    if (!ckLeg([&] { ref::fruitsFlagChk(rdram, &c, runtime); })) return;
    ckMid();
    uint64_t v0 = 0u;
    float f0 = 0.0f;
    uint64_t xo[3] = {Ps2GprU64(ctx, 6), Ps2GprU64(ctx, 7), Ps2GprU64(ctx, 8)};
    if (!ckLeg([&] { v0 = a1_sx64(ty::fruitsFlagChk(rdram, runtime, p4, p5, p7, xo)); })) return;
    ckEnd(242, ctx, c, 1, v0, f0, 0x1c8u, 0x0u, Ps2GprU64(&c, 6) != xo[0] || Ps2GprU64(&c, 7) != xo[1] || Ps2GprU64(&c, 8) != xo[2]);
}
void gameFlagCheckPointLoad(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint64_t p4, uint64_t p5, uint64_t p6, uint64_t p7, uint64_t p8, uint64_t p9)
{
    if (!ckBegin(243, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    if (!ckLeg([&] { ref::gameFlagCheckPointLoad(rdram, &c, runtime); })) return;
    ckMid();
    uint64_t v0 = 0u;
    float f0 = 0.0f;
    uint64_t xo[7] = {Ps2GprU64(ctx, 5), Ps2GprU64(ctx, 6), Ps2GprU64(ctx, 7), Ps2GprU64(ctx, 8), Ps2GprU64(ctx, 9), Ps2GprU64(ctx, 10), Ps2GprU64(ctx, 11)};
    if (!ckLeg([&] { v0 = (ty::gameFlagCheckPointLoad(rdram, runtime, p4, p5, p6, p7, p8, p9, xo)); })) return;
    ckEnd(243, ctx, c, 2, v0, f0, 0x3ff8u, 0x0u, Ps2GprU64(&c, 5) != xo[0] || Ps2GprU64(&c, 6) != xo[1] || Ps2GprU64(&c, 7) != xo[2] || Ps2GprU64(&c, 8) != xo[3] || Ps2GprU64(&c, 9) != xo[4] || Ps2GprU64(&c, 10) != xo[5] || Ps2GprU64(&c, 11) != xo[6]);
}
void tailsFlagChk(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint64_t p4, uint64_t p5, uint32_t p7)
{
    if (!ckBegin(244, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    if (!ckLeg([&] { ref::tailsFlagChk(rdram, &c, runtime); })) return;
    ckMid();
    uint64_t v0 = 0u;
    float f0 = 0.0f;
    uint64_t xo[1] = {Ps2GprU64(ctx, 8)};
    if (!ckLeg([&] { v0 = a1_sx64(ty::tailsFlagChk(rdram, runtime, p4, p5, p7, xo)); })) return;
    ckEnd(244, ctx, c, 1, v0, f0, 0x1c8u, 0x0u, Ps2GprU64(&c, 8) != xo[0]);
}
void gexsFlagChk(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint64_t p4, uint64_t p5, uint32_t p7)
{
    if (!ckBegin(245, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    if (!ckLeg([&] { ref::gexsFlagChk(rdram, &c, runtime); })) return;
    ckMid();
    uint64_t v0 = 0u;
    float f0 = 0.0f;
    uint64_t xo[3] = {Ps2GprU64(ctx, 6), Ps2GprU64(ctx, 7), Ps2GprU64(ctx, 8)};
    if (!ckLeg([&] { v0 = a1_sx64(ty::gexsFlagChk(rdram, runtime, p4, p5, p7, xo)); })) return;
    ckEnd(245, ctx, c, 1, v0, f0, 0x1c8u, 0x0u, Ps2GprU64(&c, 6) != xo[0] || Ps2GprU64(&c, 7) != xo[1] || Ps2GprU64(&c, 8) != xo[2]);
}
void _omakeGetFlagChk(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint64_t p4)
{
    if (!ckBegin(246, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    if (!ckLeg([&] { ref::_omakeGetFlagChk(rdram, &c, runtime); })) return;
    ckMid();
    uint64_t v0 = 0u;
    float f0 = 0.0f;
    uint64_t xo[1] = {Ps2GprU64(ctx, 5)};
    if (!ckLeg([&] { v0 = (ty::_omakeGetFlagChk(rdram, runtime, p4, xo)); })) return;
    ckEnd(246, ctx, c, 2, v0, f0, 0x38u, 0x0u, Ps2GprU64(&c, 5) != xo[0]);
}
void _omakeSelectFlagChk(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint64_t p4)
{
    if (!ckBegin(247, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    if (!ckLeg([&] { ref::_omakeSelectFlagChk(rdram, &c, runtime); })) return;
    ckMid();
    uint64_t v0 = 0u;
    float f0 = 0.0f;
    uint64_t xo[2] = {Ps2GprU64(ctx, 4), Ps2GprU64(ctx, 5)};
    if (!ckLeg([&] { v0 = (ty::_omakeSelectFlagChk(rdram, runtime, p4, xo)); })) return;
    ckEnd(247, ctx, c, 2, v0, f0, 0x38u, 0x0u, Ps2GprU64(&c, 4) != xo[0] || Ps2GprU64(&c, 5) != xo[1]);
}
void gameflowBossEyeCameraDisable(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint64_t p4)
{
    if (!ckBegin(248, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    if (!ckLeg([&] { ref::gameflowBossEyeCameraDisable(rdram, &c, runtime); })) return;
    ckMid();
    uint64_t v0 = 0u;
    float f0 = 0.0f;
    uint64_t xo[2] = {Ps2GprU64(ctx, 5), Ps2GprU64(ctx, 6)};
    if (!ckLeg([&] { v0 = a1_sx64(ty::gameflowBossEyeCameraDisable(rdram, runtime, p4, xo)); })) return;
    ckEnd(248, ctx, c, 1, v0, f0, 0x68u, 0x0u, Ps2GprU64(&c, 5) != xo[0] || Ps2GprU64(&c, 6) != xo[1]);
}
void gameModeSet(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint64_t p4)
{
    if (!ckBegin(249, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    if (!ckLeg([&] { ref::gameModeSet(rdram, &c, runtime); })) return;
    ckMid();
    uint64_t v0 = 0u;
    float f0 = 0.0f;
    uint64_t xo[3] = {Ps2GprU64(ctx, 4), Ps2GprU64(ctx, 5), Ps2GprU64(ctx, 6)};
    if (!ckLeg([&] { v0 = (ty::gameModeSet(rdram, runtime, p4, xo)); })) return;
    ckEnd(249, ctx, c, 2, v0, f0, 0x78u, 0x0u, Ps2GprU64(&c, 4) != xo[0] || Ps2GprU64(&c, 5) != xo[1] || Ps2GprU64(&c, 6) != xo[2]);
}
void gameModeCls(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint64_t p4)
{
    if (!ckBegin(250, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    if (!ckLeg([&] { ref::gameModeCls(rdram, &c, runtime); })) return;
    ckMid();
    uint64_t v0 = 0u;
    float f0 = 0.0f;
    uint64_t xo[7] = {Ps2GprU64(ctx, 2), Ps2GprU64(ctx, 4), Ps2GprU64(ctx, 5), Ps2GprU64(ctx, 6), Ps2GprU64(ctx, 7), Ps2GprU64(ctx, 8), Ps2GprU64(ctx, 9)};
    if (!ckLeg([&] { ty::gameModeCls(rdram, runtime, p4, xo); })) return;
    ckEnd(250, ctx, c, 0, v0, f0, 0x3fcu, 0x0u, Ps2GprU64(&c, 2) != xo[0] || Ps2GprU64(&c, 4) != xo[1] || Ps2GprU64(&c, 5) != xo[2] || Ps2GprU64(&c, 6) != xo[3] || Ps2GprU64(&c, 7) != xo[4] || Ps2GprU64(&c, 8) != xo[5] || Ps2GprU64(&c, 9) != xo[6]);
}
void gStatPlaceIdToSavePoint(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint64_t p4)
{
    if (!ckBegin(251, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    if (!ckLeg([&] { ref::gStatPlaceIdToSavePoint(rdram, &c, runtime); })) return;
    ckMid();
    uint64_t v0 = 0u;
    float f0 = 0.0f;
    uint64_t xo[3] = {Ps2GprU64(ctx, 5), Ps2GprU64(ctx, 6), Ps2GprU64(ctx, 7)};
    if (!ckLeg([&] { v0 = a1_sx64(ty::gStatPlaceIdToSavePoint(rdram, runtime, p4, xo)); })) return;
    ckEnd(251, ctx, c, 1, v0, f0, 0xe8u, 0x0u, Ps2GprU64(&c, 5) != xo[0] || Ps2GprU64(&c, 6) != xo[1] || Ps2GprU64(&c, 7) != xo[2]);
}
void gstatBootMcSetLastSaveOmakeStat(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t p4)
{
    if (!ckBegin(252, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    if (!ckLeg([&] { ref::gstatBootMcSetLastSaveOmakeStat(rdram, &c, runtime); })) return;
    ckMid();
    uint64_t v0 = 0u;
    float f0 = 0.0f;
    uint64_t xo[2] = {Ps2GprU64(ctx, 6), Ps2GprU64(ctx, 8)};
    if (!ckLeg([&] { v0 = a1_sx64(ty::gstatBootMcSetLastSaveOmakeStat(rdram, runtime, p4, xo)); })) return;
    ckEnd(252, ctx, c, 1, v0, f0, 0x1f8u, 0x0u, Ps2GprU64(&c, 6) != xo[0] || Ps2GprU64(&c, 8) != xo[1]);
}
void opflowSetDemoFrame(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, float pf12)
{
    if (!ckBegin(253, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    if (!ckLeg([&] { ref::opflowSetDemoFrame(rdram, &c, runtime); })) return;
    ckMid();
    uint64_t v0 = 0u;
    float f0 = 0.0f;
    uint64_t xo[1] = {(uint64_t)a1ck::ckBits(ctx->f[12])};
    if (!ckLeg([&] { const a1::RetVF r = ty::opflowSetDemoFrame(rdram, runtime, pf12, xo); v0 = r.v0; f0 = r.f0; })) return;
    ckEnd(253, ctx, c, 4, v0, f0, 0x2u, 0x1000u, a1ck::ckBits(c.f[12]) != (uint32_t)xo[0]);
}
void getStageChipID(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t p4, uint32_t p5, uint32_t p6)
{
    if (!ckBegin(254, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    if (!ckLeg([&] { ref::getStageChipID(rdram, &c, runtime); })) return;
    ckMid();
    uint64_t v0 = 0u;
    float f0 = 0.0f;
    uint64_t xo[1] = {Ps2GprU64(ctx, 7)};
    if (!ckLeg([&] { const a1::RetVF r = ty::getStageChipID(rdram, runtime, p4, p5, p6, xo); v0 = r.v0; f0 = r.f0; })) return;
    ckEnd(254, ctx, c, 4, v0, f0, 0x8au, 0xeu, Ps2GprU64(&c, 7) != xo[0]);
}
void gamecore_sub_01358A68(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint64_t p4)
{
    if (!ckBegin(255, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    if (!ckLeg([&] { ref::gamecore_sub_01358A68(rdram, &c, runtime); })) return;
    ckMid();
    uint64_t v0 = 0u;
    float f0 = 0.0f;
    uint64_t xo[3] = {Ps2GprU64(ctx, 6), Ps2GprU64(ctx, 7), Ps2GprU64(ctx, 8)};
    if (!ckLeg([&] { v0 = a1_sx64(ty::gamecore_sub_01358A68(rdram, runtime, p4, xo)); })) return;
    ckEnd(255, ctx, c, 1, v0, f0, 0x1f8u, 0x0u, Ps2GprU64(&c, 6) != xo[0] || Ps2GprU64(&c, 7) != xo[1] || Ps2GprU64(&c, 8) != xo[2]);
}
void stagemanager_checkstageAlreadyLoadRequestedByStrptr(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint64_t p4)
{
    if (!ckBegin(256, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    if (!ckLeg([&] { ref::stagemanager_checkstageAlreadyLoadRequestedByStrptr(rdram, &c, runtime); })) return;
    ckMid();
    uint64_t v0 = 0u;
    float f0 = 0.0f;
    uint64_t xo[3] = {Ps2GprU64(ctx, 5), Ps2GprU64(ctx, 6), Ps2GprU64(ctx, 7)};
    if (!ckLeg([&] { v0 = a1_sx64(ty::stagemanager_checkstageAlreadyLoadRequestedByStrptr(rdram, runtime, p4, xo)); })) return;
    ckEnd(256, ctx, c, 1, v0, f0, 0xf8u, 0x0u, Ps2GprU64(&c, 5) != xo[0] || Ps2GprU64(&c, 6) != xo[1] || Ps2GprU64(&c, 7) != xo[2]);
}
void gcGetStageData(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t p4)
{
    if (!ckBegin(257, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    if (!ckLeg([&] { ref::gcGetStageData(rdram, &c, runtime); })) return;
    ckMid();
    uint64_t v0 = 0u;
    float f0 = 0.0f;
    uint64_t xo[1] = {Ps2GprU64(ctx, 5)};
    if (!ckLeg([&] { v0 = a1_sx64(ty::gcGetStageData(rdram, runtime, p4, xo)); })) return;
    ckEnd(257, ctx, c, 1, v0, f0, 0x38u, 0x0u, Ps2GprU64(&c, 5) != xo[0]);
}
void stagemanager_getStageNumberWithStageDataStrptr(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint64_t p4)
{
    if (!ckBegin(258, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    if (!ckLeg([&] { ref::stagemanager_getStageNumberWithStageDataStrptr(rdram, &c, runtime); })) return;
    ckMid();
    uint64_t v0 = 0u;
    float f0 = 0.0f;
    uint64_t xo[3] = {Ps2GprU64(ctx, 4), Ps2GprU64(ctx, 5), Ps2GprU64(ctx, 6)};
    if (!ckLeg([&] { v0 = a1_sx64(ty::stagemanager_getStageNumberWithStageDataStrptr(rdram, runtime, p4, xo)); })) return;
    ckEnd(258, ctx, c, 1, v0, f0, 0x78u, 0x0u, Ps2GprU64(&c, 4) != xo[0] || Ps2GprU64(&c, 5) != xo[1] || Ps2GprU64(&c, 6) != xo[2]);
}
void PauseStageBoundaryManager(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint64_t p4)
{
    if (!ckBegin(259, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    if (!ckLeg([&] { ref::PauseStageBoundaryManager(rdram, &c, runtime); })) return;
    ckMid();
    uint64_t v0 = 0u;
    float f0 = 0.0f;
    uint64_t xo[8] = {Ps2GprU64(ctx, 4), Ps2GprU64(ctx, 5), Ps2GprU64(ctx, 6), Ps2GprU64(ctx, 7), Ps2GprU64(ctx, 8), Ps2GprU64(ctx, 9), Ps2GprU64(ctx, 10), Ps2GprU64(ctx, 11)};
    if (!ckLeg([&] { v0 = a1_sx64(ty::PauseStageBoundaryManager(rdram, runtime, p4, xo)); })) return;
    ckEnd(259, ctx, c, 1, v0, f0, 0x1ff8u, 0x0u, Ps2GprU64(&c, 4) != xo[0] || Ps2GprU64(&c, 5) != xo[1] || Ps2GprU64(&c, 6) != xo[2] || Ps2GprU64(&c, 7) != xo[3] || Ps2GprU64(&c, 8) != xo[4] || Ps2GprU64(&c, 9) != xo[5] || Ps2GprU64(&c, 10) != xo[6] || Ps2GprU64(&c, 11) != xo[7]);
}
void initBgmScript(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (!ckBegin(260, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    if (!ckLeg([&] { ref::initBgmScript(rdram, &c, runtime); })) return;
    ckMid();
    uint64_t v0 = 0u;
    float f0 = 0.0f;
    uint64_t xo[3] = {Ps2GprU64(ctx, 4), Ps2GprU64(ctx, 5), Ps2GprU64(ctx, 6)};
    if (!ckLeg([&] { v0 = (ty::initBgmScript(rdram, runtime, xo)); })) return;
    ckEnd(260, ctx, c, 2, v0, f0, 0x78u, 0x0u, Ps2GprU64(&c, 4) != xo[0] || Ps2GprU64(&c, 5) != xo[1] || Ps2GprU64(&c, 6) != xo[2]);
}
void gamecore_sub_0136A328(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint64_t p4)
{
    if (!ckBegin(261, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    if (!ckLeg([&] { ref::gamecore_sub_0136A328(rdram, &c, runtime); })) return;
    ckMid();
    uint64_t v0 = 0u;
    float f0 = 0.0f;
    if (!ckLeg([&] { v0 = a1_sx64(ty::gamecore_sub_0136A328(rdram, runtime, p4)); })) return;
    ckEnd(261, ctx, c, 1, v0, f0, 0x28u, 0x0u, false);
}
void GetBossPartsDamagePerParts(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t p4, uint32_t p5)
{
    if (!ckBegin(262, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    if (!ckLeg([&] { ref::GetBossPartsDamagePerParts(rdram, &c, runtime); })) return;
    ckMid();
    uint64_t v0 = 0u;
    float f0 = 0.0f;
    uint64_t xo[2] = {Ps2GprU64(ctx, 6), Ps2GprU64(ctx, 7)};
    if (!ckLeg([&] { const a1::RetVF r = ty::GetBossPartsDamagePerParts(rdram, runtime, p4, p5, xo); v0 = r.v0; f0 = r.f0; })) return;
    ckEnd(262, ctx, c, 4, v0, f0, 0xc8u, 0x2u, Ps2GprU64(&c, 6) != xo[0] || Ps2GprU64(&c, 7) != xo[1]);
}
void getexpressiondatapoint(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t p4, uint32_t p5)
{
    if (!ckBegin(263, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    if (!ckLeg([&] { ref::getexpressiondatapoint(rdram, &c, runtime); })) return;
    ckMid();
    uint64_t v0 = 0u;
    float f0 = 0.0f;
    uint64_t xo[1] = {Ps2GprU64(ctx, 5)};
    if (!ckLeg([&] { f0 = ty::getexpressiondatapoint(rdram, runtime, p4, p5, xo); })) return;
    ckEnd(263, ctx, c, 3, v0, f0, 0x20u, 0x0u, Ps2GprU64(&c, 5) != xo[0]);
}
void controllerAlertDisable(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint64_t p4)
{
    if (!ckBegin(264, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    if (!ckLeg([&] { ref::controllerAlertDisable(rdram, &c, runtime); })) return;
    ckMid();
    uint64_t v0 = 0u;
    float f0 = 0.0f;
    uint64_t xo[2] = {Ps2GprU64(ctx, 5), Ps2GprU64(ctx, 6)};
    if (!ckLeg([&] { v0 = a1_sx64(ty::controllerAlertDisable(rdram, runtime, p4, xo)); })) return;
    ckEnd(264, ctx, c, 1, v0, f0, 0x68u, 0x0u, Ps2GprU64(&c, 5) != xo[0] || Ps2GprU64(&c, 6) != xo[1]);
}
void gamecore_sub_01386B98(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (!ckBegin(265, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    if (!ckLeg([&] { ref::gamecore_sub_01386B98(rdram, &c, runtime); })) return;
    ckMid();
    uint64_t v0 = 0u;
    float f0 = 0.0f;
    uint64_t xo[1] = {(uint64_t)a1ck::ckBits(ctx->f[0])};
    if (!ckLeg([&] { v0 = a1_sx64(ty::gamecore_sub_01386B98(rdram, runtime, xo)); })) return;
    ckEnd(265, ctx, c, 1, v0, f0, 0x1ff8u, 0x1u, a1ck::ckBits(c.f[0]) != (uint32_t)xo[0]);
}
void fadeout_weakpoint_effect(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (!ckBegin(266, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    if (!ckLeg([&] { ref::fadeout_weakpoint_effect(rdram, &c, runtime); })) return;
    ckMid();
    uint64_t v0 = 0u;
    float f0 = 0.0f;
    if (!ckLeg([&] { v0 = a1_sx64(ty::fadeout_weakpoint_effect(rdram, runtime)); })) return;
    ckEnd(266, ctx, c, 1, v0, f0, 0x18u, 0x0u, false);
}
void cancel_fadeout_weakpoint_effect(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (!ckBegin(267, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    if (!ckLeg([&] { ref::cancel_fadeout_weakpoint_effect(rdram, &c, runtime); })) return;
    ckMid();
    uint64_t v0 = 0u;
    float f0 = 0.0f;
    if (!ckLeg([&] { v0 = a1_sx64(ty::cancel_fadeout_weakpoint_effect(rdram, runtime)); })) return;
    ckEnd(267, ctx, c, 1, v0, f0, 0x18u, 0x0u, false);
}
void set_node_data_expression(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t p4, uint64_t p5, uint32_t p6, float pf12, float pf13, float pf14)
{
    if (!ckBegin(268, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    if (!ckLeg([&] { ref::set_node_data_expression(rdram, &c, runtime); })) return;
    ckMid();
    uint64_t v0 = 0u;
    float f0 = 0.0f;
    uint64_t xo[3] = {Ps2GprU64(ctx, 2), Ps2GprU64(ctx, 4), Ps2GprU64(ctx, 7)};
    if (!ckLeg([&] { ty::set_node_data_expression(rdram, runtime, p4, p5, p6, pf12, pf13, pf14, xo); })) return;
    ckEnd(268, ctx, c, 0, v0, f0, 0x9cu, 0x0u, Ps2GprU64(&c, 2) != xo[0] || Ps2GprU64(&c, 4) != xo[1] || Ps2GprU64(&c, 7) != xo[2]);
}
void constructor_expression_change(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t p4)
{
    if (!ckBegin(269, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    if (!ckLeg([&] { ref::constructor_expression_change(rdram, &c, runtime); })) return;
    ckMid();
    uint64_t v0 = 0u;
    float f0 = 0.0f;
    if (!ckLeg([&] { v0 = a1_sx64(ty::constructor_expression_change(rdram, runtime, p4)); })) return;
    ckEnd(269, ctx, c, 1, v0, f0, 0x10u, 0x0u, false);
}
void EnableArrowWater(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t p4)
{
    if (!ckBegin(270, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    if (!ckLeg([&] { ref::EnableArrowWater(rdram, &c, runtime); })) return;
    ckMid();
    uint64_t v0 = 0u;
    float f0 = 0.0f;
    uint64_t xo[2] = {Ps2GprU64(ctx, 5), Ps2GprU64(ctx, 6)};
    if (!ckLeg([&] { v0 = a1_sx64(ty::EnableArrowWater(rdram, runtime, p4, xo)); })) return;
    ckEnd(270, ctx, c, 1, v0, f0, 0x68u, 0x0u, Ps2GprU64(&c, 5) != xo[0] || Ps2GprU64(&c, 6) != xo[1]);
}
void EndArrowWater(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint64_t p4)
{
    if (!ckBegin(271, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    if (!ckLeg([&] { ref::EndArrowWater(rdram, &c, runtime); })) return;
    ckMid();
    uint64_t v0 = 0u;
    float f0 = 0.0f;
    uint64_t xo[2] = {Ps2GprU64(ctx, 5), Ps2GprU64(ctx, 6)};
    if (!ckLeg([&] { v0 = a1_sx64(ty::EndArrowWater(rdram, runtime, p4, xo)); })) return;
    ckEnd(271, ctx, c, 1, v0, f0, 0x68u, 0x0u, Ps2GprU64(&c, 5) != xo[0] || Ps2GprU64(&c, 6) != xo[1]);
}
void gamecore_sub_01393E58(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint64_t p4, uint64_t p5, uint64_t p6, uint64_t p7, float pf12)
{
    if (!ckBegin(272, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    if (!ckLeg([&] { ref::gamecore_sub_01393E58(rdram, &c, runtime); })) return;
    ckMid();
    uint64_t v0 = 0u;
    float f0 = 0.0f;
    uint64_t xo[6] = {Ps2GprU64(ctx, 5), Ps2GprU64(ctx, 6), Ps2GprU64(ctx, 7), Ps2GprU64(ctx, 8), Ps2GprU64(ctx, 9), Ps2GprU64(ctx, 10)};
    if (!ckLeg([&] { const a1::RetVF r = ty::gamecore_sub_01393E58(rdram, runtime, p4, p5, p6, p7, pf12, xo); v0 = r.v0; f0 = r.f0; })) return;
    ckEnd(272, ctx, c, 4, v0, f0, 0x7f8u, 0x0u, Ps2GprU64(&c, 5) != xo[0] || Ps2GprU64(&c, 6) != xo[1] || Ps2GprU64(&c, 7) != xo[2] || Ps2GprU64(&c, 8) != xo[3] || Ps2GprU64(&c, 9) != xo[4] || Ps2GprU64(&c, 10) != xo[5]);
}
void gamecore_sub_0139F3B0(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint64_t p4)
{
    if (!ckBegin(273, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    if (!ckLeg([&] { ref::gamecore_sub_0139F3B0(rdram, &c, runtime); })) return;
    ckMid();
    uint64_t v0 = 0u;
    float f0 = 0.0f;
    if (!ckLeg([&] { v0 = (ty::gamecore_sub_0139F3B0(rdram, runtime, p4)); })) return;
    ckEnd(273, ctx, c, 2, v0, f0, 0xf8u, 0x0u, false);
}
void AI_GetPosition3(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, float pf12, float pf13, float pf14, float pf15, float pf16, float pf17)
{
    if (!ckBegin(274, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    if (!ckLeg([&] { ref::AI_GetPosition3(rdram, &c, runtime); })) return;
    ckMid();
    uint64_t v0 = 0u;
    float f0 = 0.0f;
    uint64_t xo[4] = {(uint64_t)a1ck::ckBits(ctx->f[13]), (uint64_t)a1ck::ckBits(ctx->f[14]), (uint64_t)a1ck::ckBits(ctx->f[16]), (uint64_t)a1ck::ckBits(ctx->f[17])};
    if (!ckLeg([&] { f0 = ty::AI_GetPosition3(rdram, runtime, pf12, pf13, pf14, pf15, pf16, pf17, xo); })) return;
    ckEnd(274, ctx, c, 3, v0, f0, 0x2u, 0x3701eu, a1ck::ckBits(c.f[13]) != (uint32_t)xo[0] || a1ck::ckBits(c.f[14]) != (uint32_t)xo[1] || a1ck::ckBits(c.f[16]) != (uint32_t)xo[2] || a1ck::ckBits(c.f[17]) != (uint32_t)xo[3]);
}
void AI_GetInterRatio(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, float pf12, float pf13, float pf14, float pf15, float pf16)
{
    if (!ckBegin(275, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    if (!ckLeg([&] { ref::AI_GetInterRatio(rdram, &c, runtime); })) return;
    ckMid();
    uint64_t v0 = 0u;
    float f0 = 0.0f;
    uint64_t xo[1] = {(uint64_t)a1ck::ckBits(ctx->f[14])};
    if (!ckLeg([&] { f0 = ty::AI_GetInterRatio(rdram, runtime, pf12, pf13, pf14, pf15, pf16, xo); })) return;
    ckEnd(275, ctx, c, 3, v0, f0, 0x0u, 0x4002u, a1ck::ckBits(c.f[14]) != (uint32_t)xo[0]);
}
void AI_GetInterRatioOutCut(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, float pf12, float pf13, float pf14, float pf15, float pf16)
{
    if (!ckBegin(276, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    if (!ckLeg([&] { ref::AI_GetInterRatioOutCut(rdram, &c, runtime); })) return;
    ckMid();
    uint64_t v0 = 0u;
    float f0 = 0.0f;
    uint64_t xo[1] = {(uint64_t)a1ck::ckBits(ctx->f[14])};
    if (!ckLeg([&] { f0 = ty::AI_GetInterRatioOutCut(rdram, runtime, pf12, pf13, pf14, pf15, pf16, xo); })) return;
    ckEnd(276, ctx, c, 3, v0, f0, 0x0u, 0x4002u, a1ck::ckBits(c.f[14]) != (uint32_t)xo[0]);
}
void Kite_CmpPathPoint(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t p4, uint32_t p5)
{
    if (!ckBegin(277, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    if (!ckLeg([&] { ref::Kite_CmpPathPoint(rdram, &c, runtime); })) return;
    ckMid();
    uint64_t v0 = 0u;
    float f0 = 0.0f;
    uint64_t xo[1] = {Ps2GprU64(ctx, 4)};
    if (!ckLeg([&] { v0 = a1_sx64(ty::Kite_CmpPathPoint(rdram, runtime, p4, p5, xo)); })) return;
    ckEnd(277, ctx, c, 1, v0, f0, 0x18u, 0x0u, Ps2GprU64(&c, 4) != xo[0]);
}
void gamecore_sub_013EF210(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint64_t p4, uint64_t p5, uint32_t p6)
{
    if (!ckBegin(278, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    if (!ckLeg([&] { ref::gamecore_sub_013EF210(rdram, &c, runtime); })) return;
    ckMid();
    uint64_t v0 = 0u;
    float f0 = 0.0f;
    if (!ckLeg([&] { const a1::RetVF r = ty::gamecore_sub_013EF210(rdram, runtime, p4, p5, p6); v0 = r.v0; f0 = r.f0; })) return;
    ckEnd(278, ctx, c, 4, v0, f0, 0x300ffb8u, 0x0u, false);
}
void getSObjNode(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint64_t p4)
{
    if (!ckBegin(279, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    if (!ckLeg([&] { ref::getSObjNode(rdram, &c, runtime); })) return;
    ckMid();
    uint64_t v0 = 0u;
    float f0 = 0.0f;
    uint64_t xo[7] = {Ps2GprU64(ctx, 5), Ps2GprU64(ctx, 6), Ps2GprU64(ctx, 7), Ps2GprU64(ctx, 8), Ps2GprU64(ctx, 9), Ps2GprU64(ctx, 10), Ps2GprU64(ctx, 11)};
    if (!ckLeg([&] { v0 = a1_sx64(ty::getSObjNode(rdram, runtime, p4, xo)); })) return;
    ckEnd(279, ctx, c, 1, v0, f0, 0xfe8u, 0x0u, Ps2GprU64(&c, 5) != xo[0] || Ps2GprU64(&c, 6) != xo[1] || Ps2GprU64(&c, 7) != xo[2] || Ps2GprU64(&c, 8) != xo[3] || Ps2GprU64(&c, 9) != xo[4] || Ps2GprU64(&c, 10) != xo[5] || Ps2GprU64(&c, 11) != xo[6]);
}
void gamecore_sub_013F1290(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t p4)
{
    if (!ckBegin(280, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    if (!ckLeg([&] { ref::gamecore_sub_013F1290(rdram, &c, runtime); })) return;
    ckMid();
    uint64_t v0 = 0u;
    float f0 = 0.0f;
    uint64_t xo[1] = {Ps2GprU64(ctx, 5)};
    if (!ckLeg([&] { v0 = a1_sx64(ty::gamecore_sub_013F1290(rdram, runtime, p4, xo)); })) return;
    ckEnd(280, ctx, c, 1, v0, f0, 0x28u, 0x0u, Ps2GprU64(&c, 5) != xo[0]);
}
void gamecore_sub_013F12F0(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t p4, uint64_t p5)
{
    if (!ckBegin(281, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    if (!ckLeg([&] { ref::gamecore_sub_013F12F0(rdram, &c, runtime); })) return;
    ckMid();
    uint64_t v0 = 0u;
    float f0 = 0.0f;
    if (!ckLeg([&] { v0 = (ty::gamecore_sub_013F12F0(rdram, runtime, p4, p5)); })) return;
    ckEnd(281, ctx, c, 2, v0, f0, 0x18u, 0x0u, false);
}
void gamecore_sub_013F1330(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint64_t p4)
{
    if (!ckBegin(282, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    if (!ckLeg([&] { ref::gamecore_sub_013F1330(rdram, &c, runtime); })) return;
    ckMid();
    uint64_t v0 = 0u;
    float f0 = 0.0f;
    uint64_t xo[1] = {Ps2GprU64(ctx, 5)};
    if (!ckLeg([&] { v0 = (ty::gamecore_sub_013F1330(rdram, runtime, p4, xo)); })) return;
    ckEnd(282, ctx, c, 2, v0, f0, 0x78u, 0x0u, Ps2GprU64(&c, 5) != xo[0]);
}
void gamecore_sub_013F1370(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t p4)
{
    if (!ckBegin(283, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    if (!ckLeg([&] { ref::gamecore_sub_013F1370(rdram, &c, runtime); })) return;
    ckMid();
    uint64_t v0 = 0u;
    float f0 = 0.0f;
    uint64_t xo[5] = {Ps2GprU64(ctx, 5), Ps2GprU64(ctx, 8), Ps2GprU64(ctx, 9), Ps2GprU64(ctx, 10), Ps2GprU64(ctx, 11)};
    if (!ckLeg([&] { v0 = (ty::gamecore_sub_013F1370(rdram, runtime, p4, xo)); })) return;
    ckEnd(283, ctx, c, 2, v0, f0, 0xff8u, 0x0u, Ps2GprU64(&c, 5) != xo[0] || Ps2GprU64(&c, 8) != xo[1] || Ps2GprU64(&c, 9) != xo[2] || Ps2GprU64(&c, 10) != xo[3] || Ps2GprU64(&c, 11) != xo[4]);
}
void gamecore_sub_013F13E0(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t p4, uint64_t p5)
{
    if (!ckBegin(284, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    if (!ckLeg([&] { ref::gamecore_sub_013F13E0(rdram, &c, runtime); })) return;
    ckMid();
    uint64_t v0 = 0u;
    float f0 = 0.0f;
    if (!ckLeg([&] { v0 = a1_sx64(ty::gamecore_sub_013F13E0(rdram, runtime, p4, p5)); })) return;
    ckEnd(284, ctx, c, 1, v0, f0, 0x18u, 0x0u, false);
}
void gamecore_sub_013F1430(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint64_t p4, uint64_t p5, uint64_t p6)
{
    if (!ckBegin(285, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    if (!ckLeg([&] { ref::gamecore_sub_013F1430(rdram, &c, runtime); })) return;
    ckMid();
    uint64_t v0 = 0u;
    float f0 = 0.0f;
    uint64_t xo[1] = {Ps2GprU64(ctx, 7)};
    if (!ckLeg([&] { v0 = (ty::gamecore_sub_013F1430(rdram, runtime, p4, p5, p6, xo)); })) return;
    ckEnd(285, ctx, c, 2, v0, f0, 0x98u, 0x0u, Ps2GprU64(&c, 7) != xo[0]);
}
void gamecore_sub_013F2558(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t p4)
{
    if (!ckBegin(286, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    if (!ckLeg([&] { ref::gamecore_sub_013F2558(rdram, &c, runtime); })) return;
    ckMid();
    uint64_t v0 = 0u;
    float f0 = 0.0f;
    uint64_t xo[1] = {(uint64_t)a1ck::ckBits(ctx->f[0])};
    if (!ckLeg([&] { v0 = a1_sx64(ty::gamecore_sub_013F2558(rdram, runtime, p4, xo)); })) return;
    ckEnd(286, ctx, c, 1, v0, f0, 0x8u, 0xfu, a1ck::ckBits(c.f[0]) != (uint32_t)xo[0]);
}
void initWeaponSpriteStatus(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (!ckBegin(287, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    if (!ckLeg([&] { ref::initWeaponSpriteStatus(rdram, &c, runtime); })) return;
    ckMid();
    uint64_t v0 = 0u;
    float f0 = 0.0f;
    if (!ckLeg([&] { const a1::RetVF r = ty::initWeaponSpriteStatus(rdram, runtime); v0 = r.v0; f0 = r.f0; })) return;
    ckEnd(287, ctx, c, 4, v0, f0, 0x1au, 0x6u, false);
}
void get_camera_collision_data_base_filter_radius(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, float pf12, float pf13, float pf14)
{
    if (!ckBegin(288, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    if (!ckLeg([&] { ref::get_camera_collision_data_base_filter_radius(rdram, &c, runtime); })) return;
    ckMid();
    uint64_t v0 = 0u;
    float f0 = 0.0f;
    uint64_t xo[1] = {(uint64_t)a1ck::ckBits(ctx->f[13])};
    if (!ckLeg([&] { f0 = ty::get_camera_collision_data_base_filter_radius(rdram, runtime, pf12, pf13, pf14, xo); })) return;
    ckEnd(288, ctx, c, 3, v0, f0, 0x2u, 0x3000u, a1ck::ckBits(c.f[13]) != (uint32_t)xo[0]);
}
void is_cliff_rise_ok_camera_place_status(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint64_t p4, uint64_t p5)
{
    if (!ckBegin(289, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    if (!ckLeg([&] { ref::is_cliff_rise_ok_camera_place_status(rdram, &c, runtime); })) return;
    ckMid();
    uint64_t v0 = 0u;
    float f0 = 0.0f;
    uint64_t xo[2] = {Ps2GprU64(ctx, 4), Ps2GprU64(ctx, 5)};
    if (!ckLeg([&] { v0 = (ty::is_cliff_rise_ok_camera_place_status(rdram, runtime, p4, p5, xo)); })) return;
    ckEnd(289, ctx, c, 2, v0, f0, 0x38u, 0x0u, Ps2GprU64(&c, 4) != xo[0] || Ps2GprU64(&c, 5) != xo[1]);
}
void act_local_vs_ofst_camera_place_status(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint64_t p4, uint64_t p5)
{
    if (!ckBegin(290, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    if (!ckLeg([&] { ref::act_local_vs_ofst_camera_place_status(rdram, &c, runtime); })) return;
    ckMid();
    uint64_t v0 = 0u;
    float f0 = 0.0f;
    uint64_t xo[2] = {Ps2GprU64(ctx, 5), Ps2GprU64(ctx, 6)};
    if (!ckLeg([&] { v0 = (ty::act_local_vs_ofst_camera_place_status(rdram, runtime, p4, p5, xo)); })) return;
    ckEnd(290, ctx, c, 2, v0, f0, 0x78u, 0x0u, Ps2GprU64(&c, 5) != xo[0] || Ps2GprU64(&c, 6) != xo[1]);
}
void gamecore_sub_014144F0(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, float pf12, float pf13, float pf14)
{
    if (!ckBegin(291, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    if (!ckLeg([&] { ref::gamecore_sub_014144F0(rdram, &c, runtime); })) return;
    ckMid();
    uint64_t v0 = 0u;
    float f0 = 0.0f;
    if (!ckLeg([&] { f0 = ty::gamecore_sub_014144F0(rdram, runtime, pf12, pf13, pf14); })) return;
    ckEnd(291, ctx, c, 3, v0, f0, 0x6u, 0x6u, false);
}
void gamecore_sub_01414E78(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t p4)
{
    if (!ckBegin(292, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    if (!ckLeg([&] { ref::gamecore_sub_01414E78(rdram, &c, runtime); })) return;
    ckMid();
    uint64_t v0 = 0u;
    float f0 = 0.0f;
    uint64_t xo[1] = {(uint64_t)a1ck::ckBits(ctx->f[0])};
    if (!ckLeg([&] { v0 = a1_sx64(ty::gamecore_sub_01414E78(rdram, runtime, p4, xo)); })) return;
    ckEnd(292, ctx, c, 1, v0, f0, 0xau, 0x3u, a1ck::ckBits(c.f[0]) != (uint32_t)xo[0]);
}
void gamecore_sub_0141B6C0(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint64_t p4, uint64_t p7)
{
    if (!ckBegin(293, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    if (!ckLeg([&] { ref::gamecore_sub_0141B6C0(rdram, &c, runtime); })) return;
    ckMid();
    uint64_t v0 = 0u;
    float f0 = 0.0f;
    if (!ckLeg([&] { v0 = (ty::gamecore_sub_0141B6C0(rdram, runtime, p4, p7)); })) return;
    ckEnd(293, ctx, c, 2, v0, f0, 0x1f8u, 0x0u, false);
}
void gamecore_sub_0141B900(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t p4, uint32_t p5)
{
    if (!ckBegin(294, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    if (!ckLeg([&] { ref::gamecore_sub_0141B900(rdram, &c, runtime); })) return;
    ckMid();
    uint64_t v0 = 0u;
    float f0 = 0.0f;
    uint64_t xo[1] = {(uint64_t)a1ck::ckBits(ctx->f[0])};
    if (!ckLeg([&] { v0 = a1_sx64(ty::gamecore_sub_0141B900(rdram, runtime, p4, p5, xo)); })) return;
    ckEnd(294, ctx, c, 1, v0, f0, 0x28u, 0x3u, a1ck::ckBits(c.f[0]) != (uint32_t)xo[0]);
}
void gamecore_sub_0141BA40(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t p4, float pf12)
{
    if (!ckBegin(295, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    if (!ckLeg([&] { ref::gamecore_sub_0141BA40(rdram, &c, runtime); })) return;
    ckMid();
    uint64_t v0 = 0u;
    float f0 = 0.0f;
    uint64_t xo[1] = {(uint64_t)a1ck::ckBits(ctx->f[0])};
    if (!ckLeg([&] { v0 = a1_sx64(ty::gamecore_sub_0141BA40(rdram, runtime, p4, pf12, xo)); })) return;
    ckEnd(295, ctx, c, 1, v0, f0, 0x0u, 0x1u, a1ck::ckBits(c.f[0]) != (uint32_t)xo[0]);
}
void gamecore_sub_0141BC78(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t p4, uint32_t p5, float pf12, float pf13)
{
    if (!ckBegin(296, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    if (!ckLeg([&] { ref::gamecore_sub_0141BC78(rdram, &c, runtime); })) return;
    ckMid();
    uint64_t v0 = 0u;
    float f0 = 0.0f;
    uint64_t xo[1] = {(uint64_t)a1ck::ckBits(ctx->f[0])};
    if (!ckLeg([&] { v0 = a1_sx64(ty::gamecore_sub_0141BC78(rdram, runtime, p4, p5, pf12, pf13, xo)); })) return;
    ckEnd(296, ctx, c, 1, v0, f0, 0x48u, 0x3u, a1ck::ckBits(c.f[0]) != (uint32_t)xo[0]);
}
void add_camera_mode_common_param_segment(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint64_t p4)
{
    if (!ckBegin(297, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    if (!ckLeg([&] { ref::add_camera_mode_common_param_segment(rdram, &c, runtime); })) return;
    ckMid();
    uint64_t v0 = 0u;
    float f0 = 0.0f;
    if (!ckLeg([&] { ty::add_camera_mode_common_param_segment(rdram, runtime, p4); })) return;
    ckEnd(297, ctx, c, 0, v0, f0, 0x7cu, 0x0u, false);
}
void add_camera_mode_manager_segment(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint64_t p4)
{
    if (!ckBegin(298, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    if (!ckLeg([&] { ref::add_camera_mode_manager_segment(rdram, &c, runtime); })) return;
    ckMid();
    uint64_t v0 = 0u;
    float f0 = 0.0f;
    if (!ckLeg([&] { ty::add_camera_mode_manager_segment(rdram, runtime, p4); })) return;
    ckEnd(298, ctx, c, 0, v0, f0, 0x7fcu, 0x0u, false);
}
void is_base_default_dir_attr_change_game_camera_param_manager(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t p4)
{
    if (!ckBegin(299, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    if (!ckLeg([&] { ref::is_base_default_dir_attr_change_game_camera_param_manager(rdram, &c, runtime); })) return;
    ckMid();
    uint64_t v0 = 0u;
    float f0 = 0.0f;
    uint64_t xo[1] = {Ps2GprU64(ctx, 6)};
    if (!ckLeg([&] { v0 = a1_sx64(ty::is_base_default_dir_attr_change_game_camera_param_manager(rdram, runtime, p4, xo)); })) return;
    ckEnd(299, ctx, c, 1, v0, f0, 0x78u, 0x0u, Ps2GprU64(&c, 6) != xo[0]);
}
void gamecore_sub_0142C7A0(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t p4, uint32_t p5, uint32_t p6)
{
    if (!ckBegin(300, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    if (!ckLeg([&] { ref::gamecore_sub_0142C7A0(rdram, &c, runtime); })) return;
    ckMid();
    uint64_t v0 = 0u;
    float f0 = 0.0f;
    uint64_t xo[1] = {(uint64_t)a1ck::ckBits(ctx->f[0])};
    if (!ckLeg([&] { v0 = a1_sx64(ty::gamecore_sub_0142C7A0(rdram, runtime, p4, p5, p6, xo)); })) return;
    ckEnd(300, ctx, c, 1, v0, f0, 0xcau, 0x3u, a1ck::ckBits(c.f[0]) != (uint32_t)xo[0]);
}
void gamecore_sub_01430248(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t p4, uint32_t p5, uint32_t p6, uint64_t p7, uint64_t p8, uint32_t p9, float pf12)
{
    if (!ckBegin(301, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    if (!ckLeg([&] { ref::gamecore_sub_01430248(rdram, &c, runtime); })) return;
    ckMid();
    uint64_t v0 = 0u;
    float f0 = 0.0f;
    uint64_t xo[5] = {(uint64_t)a1ck::ckBits(ctx->f[0]), (uint64_t)a1ck::ckBits(ctx->f[12]), Ps2GprU64(ctx, 7), Ps2GprU64(ctx, 10), Ps2GprU64(ctx, 11)};
    if (!ckLeg([&] { v0 = a1_sx64(ty::gamecore_sub_01430248(rdram, runtime, p4, p5, p6, p7, p8, p9, pf12, xo)); })) return;
    ckEnd(301, ctx, c, 1, v0, f0, 0x7c98u, 0x1001u, a1ck::ckBits(c.f[0]) != (uint32_t)xo[0] || a1ck::ckBits(c.f[12]) != (uint32_t)xo[1] || Ps2GprU64(&c, 7) != xo[2] || Ps2GprU64(&c, 10) != xo[3] || Ps2GprU64(&c, 11) != xo[4]);
}
void getPathDataAddr(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t p4, uint64_t p5)
{
    if (!ckBegin(302, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    if (!ckLeg([&] { ref::getPathDataAddr(rdram, &c, runtime); })) return;
    ckMid();
    uint64_t v0 = 0u;
    float f0 = 0.0f;
    uint64_t xo[3] = {Ps2GprU64(ctx, 6), Ps2GprU64(ctx, 7), Ps2GprU64(ctx, 8)};
    if (!ckLeg([&] { v0 = (ty::getPathDataAddr(rdram, runtime, p4, p5, xo)); })) return;
    ckEnd(302, ctx, c, 2, v0, f0, 0x1dau, 0x0u, Ps2GprU64(&c, 6) != xo[0] || Ps2GprU64(&c, 7) != xo[1] || Ps2GprU64(&c, 8) != xo[2]);
}
void gamecore_sub_01434610(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t p4, uint64_t p5, uint64_t p6)
{
    if (!ckBegin(303, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    if (!ckLeg([&] { ref::gamecore_sub_01434610(rdram, &c, runtime); })) return;
    ckMid();
    uint64_t v0 = 0u;
    float f0 = 0.0f;
    uint64_t xo[4] = {Ps2GprU64(ctx, 7), Ps2GprU64(ctx, 8), Ps2GprU64(ctx, 9), Ps2GprU64(ctx, 10)};
    if (!ckLeg([&] { v0 = a1_sx64(ty::gamecore_sub_01434610(rdram, runtime, p4, p5, p6, xo)); })) return;
    ckEnd(303, ctx, c, 1, v0, f0, 0x79au, 0x0u, Ps2GprU64(&c, 7) != xo[0] || Ps2GprU64(&c, 8) != xo[1] || Ps2GprU64(&c, 9) != xo[2] || Ps2GprU64(&c, 10) != xo[3]);
}
void calcDownSpeedRatio(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, float pf12, float pf13, float pf14)
{
    if (!ckBegin(304, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    if (!ckLeg([&] { ref::calcDownSpeedRatio(rdram, &c, runtime); })) return;
    ckMid();
    uint64_t v0 = 0u;
    float f0 = 0.0f;
    uint64_t xo[2] = {(uint64_t)a1ck::ckBits(ctx->f[12]), (uint64_t)a1ck::ckBits(ctx->f[13])};
    if (!ckLeg([&] { f0 = ty::calcDownSpeedRatio(rdram, runtime, pf12, pf13, pf14, xo); })) return;
    ckEnd(304, ctx, c, 3, v0, f0, 0x2u, 0x3006u, a1ck::ckBits(c.f[12]) != (uint32_t)xo[0] || a1ck::ckBits(c.f[13]) != (uint32_t)xo[1]);
}
void gamecore_sub_0143D6F8(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t p4)
{
    if (!ckBegin(305, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    if (!ckLeg([&] { ref::gamecore_sub_0143D6F8(rdram, &c, runtime); })) return;
    ckMid();
    uint64_t v0 = 0u;
    float f0 = 0.0f;
    uint64_t xo[2] = {Ps2GprU64(ctx, 5), Ps2GprU64(ctx, 6)};
    if (!ckLeg([&] { v0 = a1_sx64(ty::gamecore_sub_0143D6F8(rdram, runtime, p4, xo)); })) return;
    ckEnd(305, ctx, c, 1, v0, f0, 0x68u, 0x0u, Ps2GprU64(&c, 5) != xo[0] || Ps2GprU64(&c, 6) != xo[1]);
}
void setRunStopTurnAngleFastLegCtrlWork(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t p4, float pf12)
{
    if (!ckBegin(306, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    if (!ckLeg([&] { ref::setRunStopTurnAngleFastLegCtrlWork(rdram, &c, runtime); })) return;
    ckMid();
    uint64_t v0 = 0u;
    float f0 = 0.0f;
    uint64_t xo[1] = {(uint64_t)a1ck::ckBits(ctx->f[12])};
    if (!ckLeg([&] { f0 = ty::setRunStopTurnAngleFastLegCtrlWork(rdram, runtime, p4, pf12, xo); })) return;
    ckEnd(306, ctx, c, 3, v0, f0, 0x2u, 0x1000u, a1ck::ckBits(c.f[12]) != (uint32_t)xo[0]);
}
void gamecore_sub_01441020(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t p4)
{
    if (!ckBegin(307, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    if (!ckLeg([&] { ref::gamecore_sub_01441020(rdram, &c, runtime); })) return;
    ckMid();
    uint64_t v0 = 0u;
    float f0 = 0.0f;
    uint64_t xo[1] = {Ps2GprU64(ctx, 5)};
    if (!ckLeg([&] { v0 = a1_sx64(ty::gamecore_sub_01441020(rdram, runtime, p4, xo)); })) return;
    ckEnd(307, ctx, c, 1, v0, f0, 0x38u, 0x0u, Ps2GprU64(&c, 5) != xo[0]);
}
void gamecore_sub_01441210(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t p4, uint64_t p5)
{
    if (!ckBegin(308, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    if (!ckLeg([&] { ref::gamecore_sub_01441210(rdram, &c, runtime); })) return;
    ckMid();
    uint64_t v0 = 0u;
    float f0 = 0.0f;
    uint64_t xo[1] = {Ps2GprU64(ctx, 6)};
    if (!ckLeg([&] { v0 = a1_sx64(ty::gamecore_sub_01441210(rdram, runtime, p4, p5, xo)); })) return;
    ckEnd(308, ctx, c, 1, v0, f0, 0x4au, 0xfu, Ps2GprU64(&c, 6) != xo[0]);
}
void gamecore_sub_01444370(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t p4, uint64_t p5)
{
    if (!ckBegin(309, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    if (!ckLeg([&] { ref::gamecore_sub_01444370(rdram, &c, runtime); })) return;
    ckMid();
    uint64_t v0 = 0u;
    float f0 = 0.0f;
    uint64_t xo[2] = {(uint64_t)a1ck::ckBits(ctx->f[0]), Ps2GprU64(ctx, 6)};
    if (!ckLeg([&] { v0 = a1_sx64(ty::gamecore_sub_01444370(rdram, runtime, p4, p5, xo)); })) return;
    ckEnd(309, ctx, c, 1, v0, f0, 0x4au, 0x7u, a1ck::ckBits(c.f[0]) != (uint32_t)xo[0] || Ps2GprU64(&c, 6) != xo[1]);
}
void adjustAnglePI(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, float pf12)
{
    if (!ckBegin(310, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    if (!ckLeg([&] { ref::adjustAnglePI(rdram, &c, runtime); })) return;
    ckMid();
    uint64_t v0 = 0u;
    float f0 = 0.0f;
    uint64_t xo[1] = {(uint64_t)a1ck::ckBits(ctx->f[12])};
    if (!ckLeg([&] { f0 = ty::adjustAnglePI(rdram, runtime, pf12, xo); })) return;
    ckEnd(310, ctx, c, 3, v0, f0, 0x2u, 0x1002u, a1ck::ckBits(c.f[12]) != (uint32_t)xo[0]);
}
void initBeardIKWork(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t p6)
{
    if (!ckBegin(311, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    if (!ckLeg([&] { ref::initBeardIKWork(rdram, &c, runtime); })) return;
    ckMid();
    uint64_t v0 = 0u;
    float f0 = 0.0f;
    if (!ckLeg([&] { v0 = a1_sx64(ty::initBeardIKWork(rdram, runtime, p6)); })) return;
    ckEnd(311, ctx, c, 1, v0, f0, 0x40u, 0x0u, false);
}
void setRatioGazeIKWork(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t p4, float pf12, float pf13, float pf14)
{
    if (!ckBegin(312, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    if (!ckLeg([&] { ref::setRatioGazeIKWork(rdram, &c, runtime); })) return;
    ckMid();
    uint64_t v0 = 0u;
    float f0 = 0.0f;
    uint64_t xo[1] = {(uint64_t)a1ck::ckBits(ctx->f[0])};
    if (!ckLeg([&] { v0 = a1_sx64(ty::setRatioGazeIKWork(rdram, runtime, p4, pf12, pf13, pf14, xo)); })) return;
    ckEnd(312, ctx, c, 1, v0, f0, 0x2u, 0x1u, a1ck::ckBits(c.f[0]) != (uint32_t)xo[0]);
}
void setWormIKNodeLimit(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t p4, float pf12)
{
    if (!ckBegin(313, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    if (!ckLeg([&] { ref::setWormIKNodeLimit(rdram, &c, runtime); })) return;
    ckMid();
    uint64_t v0 = 0u;
    float f0 = 0.0f;
    if (!ckLeg([&] { v0 = a1_sx64(ty::setWormIKNodeLimit(rdram, runtime, p4, pf12)); })) return;
    ckEnd(313, ctx, c, 1, v0, f0, 0x10u, 0x0u, false);
}
void kernel_sub_001AD958(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint64_t p4, uint64_t p5, uint32_t p6)
{
    if (!ckBegin(314, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    if (!ckLeg([&] { ref::kernel_sub_001AD958(rdram, &c, runtime); })) return;
    ckMid();
    uint64_t v0 = 0u;
    float f0 = 0.0f;
    if (!ckLeg([&] { v0 = (ty::kernel_sub_001AD958(rdram, runtime, p4, p5, p6)); })) return;
    ckEnd(314, ctx, c, 2, v0, f0, 0x78u, 0x0u, false);
}
void Sg2CtrlInit(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (!ckBegin(315, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    if (!ckLeg([&] { ref::Sg2CtrlInit(rdram, &c, runtime); })) return;
    ckMid();
    uint64_t v0 = 0u;
    float f0 = 0.0f;
    if (!ckLeg([&] { v0 = (ty::Sg2CtrlInit(rdram, runtime)); })) return;
    ckEnd(315, ctx, c, 2, v0, f0, 0x18u, 0x0u, false);
}
void GAMEUTIL_reset_interp_value_cntrl(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint64_t p4, float pf12)
{
    if (!ckBegin(316, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    if (!ckLeg([&] { ref::GAMEUTIL_reset_interp_value_cntrl(rdram, &c, runtime); })) return;
    ckMid();
    uint64_t v0 = 0u;
    float f0 = 0.0f;
    uint64_t xo[1] = {Ps2GprU64(ctx, 2)};
    if (!ckLeg([&] { f0 = ty::GAMEUTIL_reset_interp_value_cntrl(rdram, runtime, p4, pf12, xo); })) return;
    ckEnd(316, ctx, c, 3, v0, f0, 0x4u, 0x0u, Ps2GprU64(&c, 2) != xo[0]);
}
void constructor_double_spring_interp_cntrl(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint64_t p4, float pf12, float pf20)
{
    if (!ckBegin(317, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    if (!ckLeg([&] { ref::constructor_double_spring_interp_cntrl(rdram, &c, runtime); })) return;
    ckMid();
    uint64_t v0 = 0u;
    float f0 = 0.0f;
    uint64_t xo[4] = {(uint64_t)a1ck::ckBits(ctx->f[13]), (uint64_t)a1ck::ckBits(ctx->f[14]), (uint64_t)a1ck::ckBits(ctx->f[15]), (uint64_t)a1ck::ckBits(ctx->f[20])};
    if (!ckLeg([&] { ty::constructor_double_spring_interp_cntrl(rdram, runtime, p4, pf12, pf20, xo); })) return;
    ckEnd(317, ctx, c, 0, v0, f0, 0x2u, 0x10f000u, a1ck::ckBits(c.f[13]) != (uint32_t)xo[0] || a1ck::ckBits(c.f[14]) != (uint32_t)xo[1] || a1ck::ckBits(c.f[15]) != (uint32_t)xo[2] || a1ck::ckBits(c.f[20]) != (uint32_t)xo[3]);
}
void constructor_double_spring_interp_cntrl3D(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint64_t p4, float pf12, float pf13, float pf20, float pf21)
{
    if (!ckBegin(318, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    if (!ckLeg([&] { ref::constructor_double_spring_interp_cntrl3D(rdram, &c, runtime); })) return;
    ckMid();
    uint64_t v0 = 0u;
    float f0 = 0.0f;
    uint64_t xo[8] = {(uint64_t)a1ck::ckBits(ctx->f[14]), (uint64_t)a1ck::ckBits(ctx->f[15]), (uint64_t)a1ck::ckBits(ctx->f[20]), (uint64_t)a1ck::ckBits(ctx->f[21]), Ps2GprU64(ctx, 6), Ps2GprU64(ctx, 7), Ps2GprU64(ctx, 8), Ps2GprU64(ctx, 9)};
    if (!ckLeg([&] { v0 = a1_sx64(ty::constructor_double_spring_interp_cntrl3D(rdram, runtime, p4, pf12, pf13, pf20, pf21, xo)); })) return;
    ckEnd(318, ctx, c, 1, v0, f0, 0x3fau, 0x30f000u, a1ck::ckBits(c.f[14]) != (uint32_t)xo[0] || a1ck::ckBits(c.f[15]) != (uint32_t)xo[1] || a1ck::ckBits(c.f[20]) != (uint32_t)xo[2] || a1ck::ckBits(c.f[21]) != (uint32_t)xo[3] || Ps2GprU64(&c, 6) != xo[4] || Ps2GprU64(&c, 7) != xo[5] || Ps2GprU64(&c, 8) != xo[6] || Ps2GprU64(&c, 9) != xo[7]);
}
void manager_sub_010DF5D0(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint64_t p4, uint64_t p5)
{
    if (!ckBegin(319, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    if (!ckLeg([&] { ref::manager_sub_010DF5D0(rdram, &c, runtime); })) return;
    ckMid();
    uint64_t v0 = 0u;
    float f0 = 0.0f;
    if (!ckLeg([&] { v0 = (ty::manager_sub_010DF5D0(rdram, runtime, p4, p5)); })) return;
    ckEnd(319, ctx, c, 2, v0, f0, 0x78u, 0x0u, false);
}
void SetScriptLayoutObjLetterProcMode(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint64_t p4, uint64_t p5)
{
    if (!ckBegin(320, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    if (!ckLeg([&] { ref::SetScriptLayoutObjLetterProcMode(rdram, &c, runtime); })) return;
    ckMid();
    uint64_t v0 = 0u;
    float f0 = 0.0f;
    uint64_t xo[1] = {Ps2GprU64(ctx, 6)};
    if (!ckLeg([&] { v0 = (ty::SetScriptLayoutObjLetterProcMode(rdram, runtime, p4, p5, xo)); })) return;
    ckEnd(320, ctx, c, 2, v0, f0, 0x58u, 0x0u, Ps2GprU64(&c, 6) != xo[0]);
}
void gStatGetHorseDemoStageIndex(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t p4)
{
    if (!ckBegin(321, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    if (!ckLeg([&] { ref::gStatGetHorseDemoStageIndex(rdram, &c, runtime); })) return;
    ckMid();
    uint64_t v0 = 0u;
    float f0 = 0.0f;
    if (!ckLeg([&] { v0 = (ty::gStatGetHorseDemoStageIndex(rdram, runtime, p4)); })) return;
    ckEnd(321, ctx, c, 2, v0, f0, 0x38u, 0x0u, false);
}
void getPlayerMaxGripRatio(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t p4, float pf0)
{
    if (!ckBegin(322, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    if (!ckLeg([&] { ref::getPlayerMaxGripRatio(rdram, &c, runtime); })) return;
    ckMid();
    uint64_t v0 = 0u;
    float f0 = 0.0f;
    if (!ckLeg([&] { const a1::RetVF r = ty::getPlayerMaxGripRatio(rdram, runtime, p4, pf0); v0 = r.v0; f0 = r.f0; })) return;
    ckEnd(322, ctx, c, 4, v0, f0, 0x1au, 0x2u, false);
}
void initGameObject(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t p4, uint32_t p5)
{
    if (!ckBegin(323, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    if (!ckLeg([&] { ref::initGameObject(rdram, &c, runtime); })) return;
    ckMid();
    uint64_t v0 = 0u;
    float f0 = 0.0f;
    if (!ckLeg([&] { v0 = a1_sx64(ty::initGameObject(rdram, runtime, p4, p5)); })) return;
    ckEnd(323, ctx, c, 1, v0, f0, 0x78u, 0x0u, false);
}
void gamecore_sub_013AE748(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint64_t p4)
{
    if (!ckBegin(324, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    if (!ckLeg([&] { ref::gamecore_sub_013AE748(rdram, &c, runtime); })) return;
    ckMid();
    uint64_t v0 = 0u;
    float f0 = 0.0f;
    if (!ckLeg([&] { ty::gamecore_sub_013AE748(rdram, runtime, p4); })) return;
    ckEnd(324, ctx, c, 0, v0, f0, 0x32u, 0x0u, false);
}
void gamecore_sub_013F3378(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint64_t p4, uint64_t p5, float pf12, float pf13, float pf20)
{
    if (!ckBegin(325, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    if (!ckLeg([&] { ref::gamecore_sub_013F3378(rdram, &c, runtime); })) return;
    ckMid();
    uint64_t v0 = 0u;
    float f0 = 0.0f;
    uint64_t xo[3] = {(uint64_t)a1ck::ckBits(ctx->f[12]), (uint64_t)a1ck::ckBits(ctx->f[13]), (uint64_t)a1ck::ckBits(ctx->f[20])};
    if (!ckLeg([&] { const a1::RetVF r = ty::gamecore_sub_013F3378(rdram, runtime, p4, p5, pf12, pf13, pf20, xo); v0 = r.v0; f0 = r.f0; })) return;
    ckEnd(325, ctx, c, 4, v0, f0, 0x18u, 0x10300eu, a1ck::ckBits(c.f[12]) != (uint32_t)xo[0] || a1ck::ckBits(c.f[13]) != (uint32_t)xo[1] || a1ck::ckBits(c.f[20]) != (uint32_t)xo[2]);
}
void gamecore_sub_013F3778(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint64_t p4, float pf20)
{
    if (!ckBegin(326, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    if (!ckLeg([&] { ref::gamecore_sub_013F3778(rdram, &c, runtime); })) return;
    ckMid();
    uint64_t v0 = 0u;
    float f0 = 0.0f;
    uint64_t xo[2] = {(uint64_t)a1ck::ckBits(ctx->f[12]), (uint64_t)a1ck::ckBits(ctx->f[13])};
    if (!ckLeg([&] { const a1::RetVF r = ty::gamecore_sub_013F3778(rdram, runtime, p4, pf20, xo); v0 = r.v0; f0 = r.f0; })) return;
    ckEnd(326, ctx, c, 4, v0, f0, 0x1au, 0x10301eu, a1ck::ckBits(c.f[12]) != (uint32_t)xo[0] || a1ck::ckBits(c.f[13]) != (uint32_t)xo[1]);
}
void gamecore_sub_013F3868(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint64_t p4, float pf20)
{
    if (!ckBegin(327, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    if (!ckLeg([&] { ref::gamecore_sub_013F3868(rdram, &c, runtime); })) return;
    ckMid();
    uint64_t v0 = 0u;
    float f0 = 0.0f;
    uint64_t xo[2] = {(uint64_t)a1ck::ckBits(ctx->f[12]), (uint64_t)a1ck::ckBits(ctx->f[13])};
    if (!ckLeg([&] { const a1::RetVF r = ty::gamecore_sub_013F3868(rdram, runtime, p4, pf20, xo); v0 = r.v0; f0 = r.f0; })) return;
    ckEnd(327, ctx, c, 4, v0, f0, 0x18u, 0x103002u, a1ck::ckBits(c.f[12]) != (uint32_t)xo[0] || a1ck::ckBits(c.f[13]) != (uint32_t)xo[1]);
}
void checkChangeWeaponTypeSwordToOther(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint64_t p4, uint64_t p5)
{
    if (!ckBegin(328, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    if (!ckLeg([&] { ref::checkChangeWeaponTypeSwordToOther(rdram, &c, runtime); })) return;
    ckMid();
    uint64_t v0 = 0u;
    float f0 = 0.0f;
    if (!ckLeg([&] { v0 = (ty::checkChangeWeaponTypeSwordToOther(rdram, runtime, p4, p5)); })) return;
    ckEnd(328, ctx, c, 2, v0, f0, 0x18u, 0x0u, false);
}
void initWeapon(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (!ckBegin(329, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    if (!ckLeg([&] { ref::initWeapon(rdram, &c, runtime); })) return;
    ckMid();
    uint64_t v0 = 0u;
    float f0 = 0.0f;
    if (!ckLeg([&] { const a1::RetVF r = ty::initWeapon(rdram, runtime); v0 = r.v0; f0 = r.f0; })) return;
    ckEnd(329, ctx, c, 4, v0, f0, 0x1au, 0x6u, false);
}
void gamecore_sub_014140B8(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint64_t p4, uint32_t p5, float pf0, float pf20)
{
    if (!ckBegin(330, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    if (!ckLeg([&] { ref::gamecore_sub_014140B8(rdram, &c, runtime); })) return;
    ckMid();
    uint64_t v0 = 0u;
    float f0 = 0.0f;
    uint64_t xo[1] = {(uint64_t)a1ck::ckBits(ctx->f[20])};
    if (!ckLeg([&] { f0 = ty::gamecore_sub_014140B8(rdram, runtime, p4, p5, pf0, pf20, xo); })) return;
    ckEnd(330, ctx, c, 3, v0, f0, 0x6u, 0x107006u, a1ck::ckBits(c.f[20]) != (uint32_t)xo[0]);
}
void is_view_angle_zoom_game_camera(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint64_t p4)
{
    if (!ckBegin(331, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    if (!ckLeg([&] { ref::is_view_angle_zoom_game_camera(rdram, &c, runtime); })) return;
    ckMid();
    uint64_t v0 = 0u;
    float f0 = 0.0f;
    if (!ckLeg([&] { v0 = (ty::is_view_angle_zoom_game_camera(rdram, runtime, p4)); })) return;
    ckEnd(331, ctx, c, 2, v0, f0, 0x38u, 0x0u, false);
}
void SCRConditionInvisibilityCloak(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (!ckBegin(332, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    if (!ckLeg([&] { ref::SCRConditionInvisibilityCloak(rdram, &c, runtime); })) return;
    ckMid();
    uint64_t v0 = 0u;
    float f0 = 0.0f;
    if (!ckLeg([&] { v0 = (ty::SCRConditionInvisibilityCloak(rdram, runtime)); })) return;
    ckEnd(332, ctx, c, 2, v0, f0, 0x38u, 0x0u, false);
}
void SCRConditionGetOmakeFlag(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint64_t p5)
{
    if (!ckBegin(333, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    if (!ckLeg([&] { ref::SCRConditionGetOmakeFlag(rdram, &c, runtime); })) return;
    ckMid();
    uint64_t v0 = 0u;
    float f0 = 0.0f;
    if (!ckLeg([&] { v0 = (ty::SCRConditionGetOmakeFlag(rdram, runtime, p5)); })) return;
    ckEnd(333, ctx, c, 2, v0, f0, 0x38u, 0x0u, false);
}
void eventBgmAllClose(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint64_t p5)
{
    if (!ckBegin(334, rdram, runtime)) return;
    R5900Context &c = ckCtx(ctx);
    if (!ckLeg([&] { ref::eventBgmAllClose(rdram, &c, runtime); })) return;
    ckMid();
    uint64_t v0 = 0u;
    float f0 = 0.0f;
    uint64_t xo[2] = {(uint64_t)a1ck::ckBits(ctx->f[12]), Ps2GprU64(ctx, 5)};
    if (!ckLeg([&] { v0 = a1_sx64(ty::eventBgmAllClose(rdram, runtime, p5, xo)); })) return;
    ckEnd(334, ctx, c, 1, v0, f0, 0x32u, 0x1000u, a1ck::ckBits(c.f[12]) != (uint32_t)xo[0] || Ps2GprU64(&c, 5) != xo[1]);
}
const a1ck::CkEntry kEntries[335] = {
    {"RelocateElfInfoHeader", 0x1004b8u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { RelocateElfInfoHeader(rdram, ctx, runtime, (uint64_t)Ps2GprU64(ctx, 4)); }},
    {"LoaderSysEntryExternalIntcHandlerList", 0x102730u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { LoaderSysEntryExternalIntcHandlerList(rdram, ctx, runtime, (uint64_t)Ps2GprU64(ctx, 4), (uint32_t)Ps2GprU32(ctx, 5)); }},
    {"LoaderSysEntryExternalThreadList", 0x102790u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { LoaderSysEntryExternalThreadList(rdram, ctx, runtime, (uint32_t)Ps2GprU32(ctx, 4)); }},
    {"LoaderSysEntryExternalSemaList", 0x1027e0u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { LoaderSysEntryExternalSemaList(rdram, ctx, runtime, (uint32_t)Ps2GprU32(ctx, 4)); }},
    {"LoaderSysEntryExternalIopMemoryList", 0x102830u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { LoaderSysEntryExternalIopMemoryList(rdram, ctx, runtime, (uint64_t)Ps2GprU64(ctx, 4)); }},
    {"LoaderSysDeleteExternalIntcHandlerList", 0x1028a0u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { LoaderSysDeleteExternalIntcHandlerList(rdram, ctx, runtime, (uint64_t)Ps2GprU64(ctx, 4)); }},
    {"LoaderSysDeleteExternalThreadList", 0x1028f0u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { LoaderSysDeleteExternalThreadList(rdram, ctx, runtime, (uint64_t)Ps2GprU64(ctx, 4)); }},
    {"LoaderSysDeleteExternalSemaList", 0x102930u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { LoaderSysDeleteExternalSemaList(rdram, ctx, runtime, (uint64_t)Ps2GprU64(ctx, 4)); }},
    {"LoaderSysDeleteExternalIopMemoryList", 0x102970u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { LoaderSysDeleteExternalIopMemoryList(rdram, ctx, runtime, (uint64_t)Ps2GprU64(ctx, 4)); }},
    {"LoaderSysInitExternalIntcHandlerList", 0x1029c8u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { LoaderSysInitExternalIntcHandlerList(rdram, ctx, runtime); }},
    {"LoaderSysInitExternalSemaList", 0x102a00u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { LoaderSysInitExternalSemaList(rdram, ctx, runtime); }},
    {"LoaderSysInitExternalThreadList", 0x102a40u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { LoaderSysInitExternalThreadList(rdram, ctx, runtime); }},
    {"LoaderSysInitExternalIopMemoryList", 0x102c58u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { LoaderSysInitExternalIopMemoryList(rdram, ctx, runtime); }},
    {"LoaderSysGetMemoryInfo", 0x103110u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { LoaderSysGetMemoryInfo(rdram, ctx, runtime, (uint64_t)Ps2GprU64(ctx, 4), (uint64_t)Ps2GprU64(ctx, 28)); }},
    {"ScrollDisplay", 0x106390u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { ScrollDisplay(rdram, ctx, runtime, (uint64_t)Ps2GprU64(ctx, 28)); }},
    {"ClearDisplay", 0x106430u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { ClearDisplay(rdram, ctx, runtime, (uint64_t)Ps2GprU64(ctx, 28)); }},
    {"sub_00109640", 0x109640u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { sub_00109640(rdram, ctx, runtime, (uint64_t)Ps2GprU64(ctx, 4), (uint32_t)Ps2GprU32(ctx, 5)); }},
    {"sub_0010F370", 0x10f370u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { sub_0010F370(rdram, ctx, runtime, (uint32_t)Ps2GprU32(ctx, 4), (uint32_t)Ps2GprU32(ctx, 5), (uint32_t)Ps2GprU32(ctx, 6)); }},
    {"sub_0010FC08", 0x10fc08u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { sub_0010FC08(rdram, ctx, runtime, (uint32_t)Ps2GprU32(ctx, 4), (uint32_t)Ps2GprU32(ctx, 5), (uint32_t)Ps2GprU32(ctx, 6)); }},
    {"sub_0010FC40", 0x10fc40u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { sub_0010FC40(rdram, ctx, runtime, (uint64_t)Ps2GprU64(ctx, 4), (uint64_t)Ps2GprU64(ctx, 5), (uint64_t)Ps2GprU64(ctx, 6)); }},
    {"sub_0010FE50", 0x10fe50u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { sub_0010FE50(rdram, ctx, runtime, (uint32_t)Ps2GprU32(ctx, 4), (uint32_t)Ps2GprU32(ctx, 5), (uint32_t)Ps2GprU32(ctx, 6)); }},
    {"sub_0010FFC0", 0x10ffc0u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { sub_0010FFC0(rdram, ctx, runtime, (uint32_t)Ps2GprU32(ctx, 4), (uint32_t)Ps2GprU32(ctx, 5), (uint64_t)Ps2GprU64(ctx, 6)); }},
    {"sub_00110270", 0x110270u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { sub_00110270(rdram, ctx, runtime, (uint32_t)Ps2GprU32(ctx, 4), (uint32_t)Ps2GprU32(ctx, 5), (uint32_t)Ps2GprU32(ctx, 6)); }},
    {"sub_00110910", 0x110910u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { sub_00110910(rdram, ctx, runtime, (uint64_t)Ps2GprU64(ctx, 4)); }},
    {"iGetTimerSystemTime", 0x110c78u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { iGetTimerSystemTime(rdram, ctx, runtime); }},
    {"iAllocTimerCounter", 0x110d48u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { iAllocTimerCounter(rdram, ctx, runtime); }},
    {"iGetTimerBaseTime", 0x111288u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { iGetTimerBaseTime(rdram, ctx, runtime, (uint64_t)Ps2GprU64(ctx, 4)); }},
    {"ps2__InitAlarm", 0x111700u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { ps2__InitAlarm(rdram, ctx, runtime); }},
    {"sceVif1PkAddGsData", 0x113368u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { sceVif1PkAddGsData(rdram, ctx, runtime, (uint64_t)Ps2GprU64(ctx, 4), (uint64_t)Ps2GprU64(ctx, 5)); }},
    {"sceGsSetDefAlphaEnv", 0x114020u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { sceGsSetDefAlphaEnv(rdram, ctx, runtime, (uint64_t)Ps2GprU64(ctx, 4), (uint64_t)Ps2GprU64(ctx, 5)); }},
    {"sceGsSetHalfOffset", 0x114080u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { sceGsSetHalfOffset(rdram, ctx, runtime, (uint64_t)Ps2GprU64(ctx, 4), (uint64_t)Ps2GprU64(ctx, 5), (uint64_t)Ps2GprU64(ctx, 6), (uint64_t)Ps2GprU64(ctx, 7)); }},
    {"sub_001157D8", 0x1157d8u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { sub_001157D8(rdram, ctx, runtime, (uint32_t)Ps2GprU32(ctx, 4), (uint32_t)Ps2GprU32(ctx, 5)); }},
    {"sub_0011BAE8", 0x11bae8u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { sub_0011BAE8(rdram, ctx, runtime, (uint64_t)Ps2GprU64(ctx, 4), (uint32_t)Ps2GprU32(ctx, 5)); }},
    {"sub_0011BBB8", 0x11bbb8u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { sub_0011BBB8(rdram, ctx, runtime, (uint32_t)Ps2GprU32(ctx, 4)); }},
    {"sub_0011BF68", 0x11bf68u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { sub_0011BF68(rdram, ctx, runtime, (uint64_t)Ps2GprU64(ctx, 4)); }},
    {"_sceMcFatInit", 0x11c138u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { _sceMcFatInit(rdram, ctx, runtime); }},
    {"_sceMcFatInit2", 0x11c198u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { _sceMcFatInit2(rdram, ctx, runtime, (uint32_t)Ps2GprU32(ctx, 4)); }},
    {"_sceMcpInit", 0x120438u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { _sceMcpInit(rdram, ctx, runtime); }},
    {"_sceMcpIsSpeckOutBlock", 0x120d10u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { _sceMcpIsSpeckOutBlock(rdram, ctx, runtime, (uint32_t)Ps2GprU32(ctx, 4), (uint64_t)Ps2GprU64(ctx, 5)); }},
    {"sub_00121EC0", 0x121ec0u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { sub_00121EC0(rdram, ctx, runtime); }},
    {"sceNetcnfifGetResult", 0x122378u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { sceNetcnfifGetResult(rdram, ctx, runtime, (uint64_t)Ps2GprU64(ctx, 4)); }},
    {"sub_00125080", 0x125080u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { sub_00125080(rdram, ctx, runtime, (uint64_t)Ps2GprU64(ctx, 4), (uint64_t)Ps2GprU64(ctx, 5), (uint64_t)Ps2GprU64(ctx, 6)); }},
    {"sub_00125998", 0x125998u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { sub_00125998(rdram, ctx, runtime, (uint32_t)Ps2GprU32(ctx, 4), (uint32_t)Ps2GprU32(ctx, 5)); }},
    {"sub_001259E0", 0x1259e0u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { sub_001259E0(rdram, ctx, runtime, (uint32_t)Ps2GprU32(ctx, 4), (uint32_t)Ps2GprU32(ctx, 5)); }},
    {"sub_00125A38", 0x125a38u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { sub_00125A38(rdram, ctx, runtime, (uint64_t)Ps2GprU64(ctx, 4), (uint64_t)Ps2GprU64(ctx, 5)); }},
    {"sub_001262B8", 0x1262b8u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { sub_001262B8(rdram, ctx, runtime, (uint32_t)Ps2GprU32(ctx, 4)); }},
    {"sub_00126388", 0x126388u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { sub_00126388(rdram, ctx, runtime, (uint32_t)Ps2GprU32(ctx, 4), (uint32_t)Ps2GprU32(ctx, 5)); }},
    {"ps2___unpack_d", 0x1271b0u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { ps2___unpack_d(rdram, ctx, runtime, (uint64_t)Ps2GprU64(ctx, 4), (uint64_t)Ps2GprU64(ctx, 5)); }},
    {"ps2__Bfree", 0x12e890u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { ps2__Bfree(rdram, ctx, runtime, (uint32_t)Ps2GprU32(ctx, 4), (uint64_t)Ps2GprU64(ctx, 5)); }},
    {"ps2___mcmp", 0x12f0d8u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { ps2___mcmp(rdram, ctx, runtime, (uint32_t)Ps2GprU32(ctx, 4), (uint32_t)Ps2GprU32(ctx, 5)); }},
    {"_ulp", 0x12f2d0u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { _ulp(rdram, ctx, runtime, (uint64_t)Ps2GprU64(ctx, 4), (uint64_t)Ps2GprU64(ctx, 5)); }},
    {"iosCdStStart", 0x1acdf8u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { iosCdStStart(rdram, ctx, runtime, (uint64_t)Ps2GprU64(ctx, 4)); }},
    {"iosGetTCountExtension", 0x1ad528u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { iosGetTCountExtension(rdram, ctx, runtime); }},
    {"iosResetCpuRapCounter", 0x1ad628u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { iosResetCpuRapCounter(rdram, ctx, runtime, (uint64_t)Ps2GprU64(ctx, 4)); }},
    {"iosPadRead", 0x1b27b0u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { iosPadRead(rdram, ctx, runtime, (uint64_t)Ps2GprU64(ctx, 4), (uint64_t)Ps2GprU64(ctx, 5)); }},
    {"iosMemToSprDmaAdrCopy", 0x1b2a18u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { iosMemToSprDmaAdrCopy(rdram, ctx, runtime, (uint32_t)Ps2GprU32(ctx, 4), (uint32_t)Ps2GprU32(ctx, 5), (uint32_t)Ps2GprU32(ctx, 6)); }},
    {"Init_Shock", 0x1b3c50u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { Init_Shock(rdram, ctx, runtime); }},
    {"kernel_sub_001B40D0", 0x1b40d0u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { kernel_sub_001B40D0(rdram, ctx, runtime, (uint64_t)Ps2GprU64(ctx, 4)); }},
    {"iosLoaderGetFileSize", 0x1b7500u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { iosLoaderGetFileSize(rdram, ctx, runtime, (uint64_t)Ps2GprU64(ctx, 4)); }},
    {"initDldSys", 0x1b7550u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { initDldSys(rdram, ctx, runtime); }},
    {"kernel_sub_001BA690", 0x1ba690u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { kernel_sub_001BA690(rdram, ctx, runtime, (uint64_t)Ps2GprU64(ctx, 4), (uint64_t)Ps2GprU64(ctx, 5)); }},
    {"iosGetTableCos", 0x1be570u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { iosGetTableCos(rdram, ctx, runtime, (uint32_t)Ps2GprU32(ctx, 4)); }},
    {"iosSetDCViewPortCenter", 0x1bf160u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { iosSetDCViewPortCenter(rdram, ctx, runtime, (uint64_t)Ps2GprU64(ctx, 4), (uint64_t)Ps2GprU64(ctx, 5), (uint64_t)Ps2GprU64(ctx, 6), (uint64_t)Ps2GprU64(ctx, 7), (uint64_t)Ps2GprU64(ctx, 8), (uint64_t)Ps2GprU64(ctx, 9), (uint64_t)Ps2GprU64(ctx, 10)); }},
    {"iosUsedPacketBufferSize", 0x1bfad8u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { iosUsedPacketBufferSize(rdram, ctx, runtime); }},
    {"iosAllocPacketPtrPath1", 0x1bfc08u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { iosAllocPacketPtrPath1(rdram, ctx, runtime, (uint32_t)Ps2GprU32(ctx, 4)); }},
    {"iosDlChainTail", 0x1bfcf8u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { iosDlChainTail(rdram, ctx, runtime, (uint64_t)Ps2GprU64(ctx, 4), (uint64_t)Ps2GprU64(ctx, 5)); }},
    {"kernel_sub_001C1538", 0x1c1538u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { kernel_sub_001C1538(rdram, ctx, runtime); }},
    {"iosGsSetMode", 0x1c15f8u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { iosGsSetMode(rdram, ctx, runtime); }},
    {"iosChkGpDlTransExec", 0x1c35e8u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { iosChkGpDlTransExec(rdram, ctx, runtime); }},
    {"getTWH", 0x1c5e00u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { getTWH(rdram, ctx, runtime, (uint64_t)Ps2GprU64(ctx, 4)); }},
    {"isysGetLinkNum", 0x1c7948u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { isysGetLinkNum(rdram, ctx, runtime); }},
    {"getVIntrDiffTime", 0x1c7dc8u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { getVIntrDiffTime(rdram, ctx, runtime, (uint32_t)Ps2GprU32(ctx, 4)); }},
    {"addVIntrTime", 0x1c7e30u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { addVIntrTime(rdram, ctx, runtime, (uint32_t)Ps2GprU32(ctx, 4), ctx->f[12]); }},
    {"ischedStop", 0x1c7fb0u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { ischedStop(rdram, ctx, runtime); }},
    {"get_next_letter_in_post", 0x1c8910u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { get_next_letter_in_post(rdram, ctx, runtime, (uint32_t)Ps2GprU32(ctx, 4)); }},
    {"entry_new_letter_type", 0x1c8af0u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { entry_new_letter_type(rdram, ctx, runtime, (uint32_t)Ps2GprU32(ctx, 4)); }},
    {"init_letter_type_store", 0x1c8b30u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { init_letter_type_store(rdram, ctx, runtime); }},
    {"kernel_sub_001C8B98", 0x1c8b98u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { kernel_sub_001C8B98(rdram, ctx, runtime, (uint32_t)Ps2GprU32(ctx, 4)); }},
    {"iosGetFontInfo", 0x1c9678u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { iosGetFontInfo(rdram, ctx, runtime, (uint64_t)Ps2GprU64(ctx, 4), (uint64_t)Ps2GprU64(ctx, 5)); }},
    {"sndSepPlayerSrh", 0x1cab80u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { sndSepPlayerSrh(rdram, ctx, runtime, (uint64_t)Ps2GprU64(ctx, 4), (uint64_t)Ps2GprU64(ctx, 5), (uint64_t)Ps2GprU64(ctx, 6)); }},
    {"sndSepPlayerSrh2", 0x1cac08u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { sndSepPlayerSrh2(rdram, ctx, runtime, (uint64_t)Ps2GprU64(ctx, 4), (uint64_t)Ps2GprU64(ctx, 5)); }},
    {"sndSepSetVol", 0x1cac98u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { sndSepSetVol(rdram, ctx, runtime, (uint32_t)Ps2GprU32(ctx, 4), ctx->f[12]); }},
    {"sndSepSetPan", 0x1cacd8u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { sndSepSetPan(rdram, ctx, runtime, (uint32_t)Ps2GprU32(ctx, 4), ctx->f[12]); }},
    {"sndSepStopTypeAll", 0x1cadf8u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { sndSepStopTypeAll(rdram, ctx, runtime, (uint64_t)Ps2GprU64(ctx, 4)); }},
    {"sndVabCreate", 0x1caf10u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { sndVabCreate(rdram, ctx, runtime); }},
    {"sndVabInit", 0x1cb170u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { sndVabInit(rdram, ctx, runtime); }},
    {"AddStrHashKey", 0x1cbd70u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { AddStrHashKey(rdram, ctx, runtime, (uint32_t)Ps2GprU32(ctx, 4), (uint32_t)Ps2GprU32(ctx, 5)); }},
    {"Sg2CtrlEntryTickProcById", 0x1cc0c8u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { Sg2CtrlEntryTickProcById(rdram, ctx, runtime, (uint64_t)Ps2GprU64(ctx, 4), (uint32_t)Ps2GprU32(ctx, 5)); }},
    {"Sg2DspInit", 0x1cc7f8u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { Sg2DspInit(rdram, ctx, runtime); }},
    {"ps2_____VolPerBackup", 0x1cd448u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { ps2_____VolPerBackup(rdram, ctx, runtime, (uint32_t)Ps2GprU32(ctx, 4), (uint64_t)Ps2GprU64(ctx, 5)); }},
    {"kernel_sub_001CDDE8", 0x1cdde8u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { kernel_sub_001CDDE8(rdram, ctx, runtime, (uint64_t)Ps2GprU64(ctx, 4)); }},
    {"kernel_sub_001CDF38", 0x1cdf38u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { kernel_sub_001CDF38(rdram, ctx, runtime, (uint64_t)Ps2GprU64(ctx, 4)); }},
    {"Sg2SepMidiOut", 0x1d2da0u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { Sg2SepMidiOut(rdram, ctx, runtime, (uint64_t)Ps2GprU64(ctx, 4), (uint64_t)Ps2GprU64(ctx, 5), (uint64_t)Ps2GprU64(ctx, 6)); }},
    {"Sg2SlotGetSlotClassIdx", 0x1d3b08u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { Sg2SlotGetSlotClassIdx(rdram, ctx, runtime); }},
    {"Sg2SlotBlock", 0x1d5cb8u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { Sg2SlotBlock(rdram, ctx, runtime, (uint32_t)Ps2GprU32(ctx, 4), (uint32_t)Ps2GprU32(ctx, 5)); }},
    {"Sg2SlotCheckSlotClass", 0x1d5d58u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { Sg2SlotCheckSlotClass(rdram, ctx, runtime, (uint64_t)Ps2GprU64(ctx, 4)); }},
    {"Sg2SlotFindLoop", 0x1d5db0u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { Sg2SlotFindLoop(rdram, ctx, runtime, (uint32_t)Ps2GprU32(ctx, 4)); }},
    {"Sg2SlotFindVab", 0x1d5df8u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { Sg2SlotFindVab(rdram, ctx, runtime, (uint32_t)Ps2GprU32(ctx, 4), (uint64_t)Ps2GprU64(ctx, 5)); }},
    {"Sg2SlotFindVabCh", 0x1d5e48u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { Sg2SlotFindVabCh(rdram, ctx, runtime, (uint32_t)Ps2GprU32(ctx, 4), (uint64_t)Ps2GprU64(ctx, 5), (uint64_t)Ps2GprU64(ctx, 6)); }},
    {"Sg2SlotFindVabChNote", 0x1d5ea8u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { Sg2SlotFindVabChNote(rdram, ctx, runtime, (uint32_t)Ps2GprU32(ctx, 4), (uint64_t)Ps2GprU64(ctx, 5), (uint64_t)Ps2GprU64(ctx, 6), (uint64_t)Ps2GprU64(ctx, 7)); }},
    {"Sg2SlotFindVabChSsaBaseNote", 0x1d5f10u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { Sg2SlotFindVabChSsaBaseNote(rdram, ctx, runtime, (uint32_t)Ps2GprU32(ctx, 4), (uint64_t)Ps2GprU64(ctx, 5), (uint64_t)Ps2GprU64(ctx, 6), (uint64_t)Ps2GprU64(ctx, 7), (uint64_t)Ps2GprU64(ctx, 8)); }},
    {"Sg2SlotFindSlotClass", 0x1d5f88u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { Sg2SlotFindSlotClass(rdram, ctx, runtime, (uint32_t)Ps2GprU32(ctx, 4), (uint64_t)Ps2GprU64(ctx, 5)); }},
    {"Sg2SlotFindSlotClassCh", 0x1d5fd8u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { Sg2SlotFindSlotClassCh(rdram, ctx, runtime, (uint32_t)Ps2GprU32(ctx, 4), (uint64_t)Ps2GprU64(ctx, 5), (uint64_t)Ps2GprU64(ctx, 6)); }},
    {"Sg2SlotFindSlotClassChNote", 0x1d6038u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { Sg2SlotFindSlotClassChNote(rdram, ctx, runtime, (uint32_t)Ps2GprU32(ctx, 4), (uint64_t)Ps2GprU64(ctx, 5), (uint64_t)Ps2GprU64(ctx, 6), (uint64_t)Ps2GprU64(ctx, 7)); }},
    {"Sg2SlotFindSlotClassChSsaBaseNote", 0x1d60a0u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { Sg2SlotFindSlotClassChSsaBaseNote(rdram, ctx, runtime, (uint32_t)Ps2GprU32(ctx, 4), (uint64_t)Ps2GprU64(ctx, 5), (uint64_t)Ps2GprU64(ctx, 6), (uint64_t)Ps2GprU64(ctx, 7), (uint64_t)Ps2GprU64(ctx, 8)); }},
    {"Sg2SlotSetMasVol", 0x1d62f0u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { Sg2SlotSetMasVol(rdram, ctx, runtime, (uint32_t)Ps2GprU32(ctx, 4), (uint32_t)Ps2GprU32(ctx, 5), (uint32_t)Ps2GprU32(ctx, 6)); }},
    {"Sg2SlotSetSeMasVol", 0x1d6360u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { Sg2SlotSetSeMasVol(rdram, ctx, runtime, (uint32_t)Ps2GprU32(ctx, 4), (uint32_t)Ps2GprU32(ctx, 5)); }},
    {"Sg2SlotSetBaseVol", 0x1d63b8u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { Sg2SlotSetBaseVol(rdram, ctx, runtime, (uint32_t)Ps2GprU32(ctx, 4), (uint32_t)Ps2GprU32(ctx, 5)); }},
    {"Sg2SlotSetVol", 0x1d6410u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { Sg2SlotSetVol(rdram, ctx, runtime, (uint32_t)Ps2GprU32(ctx, 4), (uint32_t)Ps2GprU32(ctx, 5)); }},
    {"Sg2SlotSetMasPan", 0x1d6440u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { Sg2SlotSetMasPan(rdram, ctx, runtime, (uint32_t)Ps2GprU32(ctx, 4), (uint32_t)Ps2GprU32(ctx, 5)); }},
    {"Sg2SlotSetBasePan", 0x1d64a0u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { Sg2SlotSetBasePan(rdram, ctx, runtime, (uint32_t)Ps2GprU32(ctx, 4), (uint32_t)Ps2GprU32(ctx, 5)); }},
    {"Sg2SlotSetPan", 0x1d64f8u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { Sg2SlotSetPan(rdram, ctx, runtime, (uint32_t)Ps2GprU32(ctx, 4), (uint32_t)Ps2GprU32(ctx, 5)); }},
    {"Sg2SlotSetMasTune", 0x1d66a0u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { Sg2SlotSetMasTune(rdram, ctx, runtime, (uint32_t)Ps2GprU32(ctx, 4), (uint32_t)Ps2GprU32(ctx, 5)); }},
    {"Sg2SlotSetVolPhase", 0x1d6720u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { Sg2SlotSetVolPhase(rdram, ctx, runtime, (uint32_t)Ps2GprU32(ctx, 4), (uint32_t)Ps2GprU32(ctx, 5), (uint32_t)Ps2GprU32(ctx, 6)); }},
    {"Sg2SlotSetLfoPitchTbl", 0x1d67a8u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { Sg2SlotSetLfoPitchTbl(rdram, ctx, runtime, (uint32_t)Ps2GprU32(ctx, 4), (uint64_t)Ps2GprU64(ctx, 5)); }},
    {"Sg2SlotSetLfoPitchDepth", 0x1d68a8u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { Sg2SlotSetLfoPitchDepth(rdram, ctx, runtime, (uint64_t)Ps2GprU64(ctx, 4), (uint64_t)Ps2GprU64(ctx, 5)); }},
    {"Sg2SlotSetPoltamentParam", 0x1d6cf0u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { Sg2SlotSetPoltamentParam(rdram, ctx, runtime, (uint64_t)Ps2GprU64(ctx, 4), (uint64_t)Ps2GprU64(ctx, 5), ctx->f[12]); }},
    {"Sg2SlotTbl", 0x1d8000u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { Sg2SlotTbl(rdram, ctx, runtime, (uint32_t)Ps2GprU32(ctx, 4), (uint64_t)Ps2GprU64(ctx, 5)); }},
    {"Sg2SmfpMidiOut", 0x1d8de0u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { Sg2SmfpMidiOut(rdram, ctx, runtime, (uint64_t)Ps2GprU64(ctx, 4), (uint64_t)Ps2GprU64(ctx, 5), (uint32_t)Ps2GprU32(ctx, 6)); }},
    {"get_vabid", 0x1d9ec8u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { get_vabid(rdram, ctx, runtime); }},
    {"kernel_sub_001D9F08", 0x1d9f08u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { kernel_sub_001D9F08(rdram, ctx, runtime, (uint32_t)Ps2GprU32(ctx, 4), (uint64_t)Ps2GprU64(ctx, 5)); }},
    {"kernel_sub_001DAAB0", 0x1daab0u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { kernel_sub_001DAAB0(rdram, ctx, runtime, (uint64_t)Ps2GprU64(ctx, 4), (uint64_t)Ps2GprU64(ctx, 5)); }},
    {"kernel_sub_001DAC18", 0x1dac18u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { kernel_sub_001DAC18(rdram, ctx, runtime, (uint32_t)Ps2GprU32(ctx, 4), (uint32_t)Ps2GprU32(ctx, 5), (uint64_t)Ps2GprU64(ctx, 6)); }},
    {"kernel_sub_001DB678", 0x1db678u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { kernel_sub_001DB678(rdram, ctx, runtime); }},
    {"IirFilter", 0x1dcb48u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { IirFilter(rdram, ctx, runtime, (uint32_t)Ps2GprU32(ctx, 4), (uint64_t)Ps2GprU64(ctx, 5), (uint32_t)Ps2GprU32(ctx, 6)); }},
    {"ps2___ieee754_fmodf", 0x1de720u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { ps2___ieee754_fmodf(rdram, ctx, runtime, ctx->f[12], ctx->f[13]); }},
    {"GAMEUTIL_inc_or_dec_for_near_to_aim_pitch", 0x107da28u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { GAMEUTIL_inc_or_dec_for_near_to_aim_pitch(rdram, ctx, runtime, (uint64_t)Ps2GprU64(ctx, 4), (uint64_t)Ps2GprU64(ctx, 5), ctx->f[12], ctx->f[13], ctx->f[14], ctx->f[15], ctx->f[16]); }},
    {"GAMEUTIL_get_interp_pitch", 0x107dd80u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { GAMEUTIL_get_interp_pitch(rdram, ctx, runtime, ctx->f[12], ctx->f[13], ctx->f[14]); }},
    {"GAMEUTIL_is_diff_nearly_equal_zero", 0x107ed30u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { GAMEUTIL_is_diff_nearly_equal_zero(rdram, ctx, runtime, ctx->f[12], ctx->f[13], ctx->f[14]); }},
    {"GAMEUTIL_get_cross_position_coefficient_t_by_dist_factor", 0x107eec8u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { GAMEUTIL_get_cross_position_coefficient_t_by_dist_factor(rdram, ctx, runtime, (uint64_t)Ps2GprU64(ctx, 4), (uint32_t)Ps2GprU32(ctx, 5), (uint32_t)Ps2GprU32(ctx, 6), (uint32_t)Ps2GprU32(ctx, 7), (uint32_t)Ps2GprU32(ctx, 8), ctx->f[12]); }},
    {"GAMEUTIL_get_segment_num_and_data", 0x107f078u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { GAMEUTIL_get_segment_num_and_data(rdram, ctx, runtime, (uint64_t)Ps2GprU64(ctx, 4), (uint64_t)Ps2GprU64(ctx, 5)); }},
    {"GAMEUTIL_get_new_velocity", 0x107fa60u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { GAMEUTIL_get_new_velocity(rdram, ctx, runtime, ctx->f[12], ctx->f[13], ctx->f[14], ctx->f[15], ctx->f[16]); }},
    {"GAMEUTIL_increase_pitch", 0x10806a8u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { GAMEUTIL_increase_pitch(rdram, ctx, runtime, (uint64_t)Ps2GprU64(ctx, 4), (uint64_t)Ps2GprU64(ctx, 5), ctx->f[12], ctx->f[13], ctx->f[14], ctx->f[15]); }},
    {"GAMEUTIL_get_difference_yaw_and_yaw", 0x1080968u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { GAMEUTIL_get_difference_yaw_and_yaw(rdram, ctx, runtime, ctx->f[12], ctx->f[13]); }},
    {"GAMEUTIL_is_reach_aim_pitch_angle", 0x1081090u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { GAMEUTIL_is_reach_aim_pitch_angle(rdram, ctx, runtime, (uint64_t)Ps2GprU64(ctx, 4), ctx->f[12], ctx->f[13], ctx->f[14]); }},
    {"GAMEUTIL_reset_interp_ratio_cntrl", 0x10842a0u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { GAMEUTIL_reset_interp_ratio_cntrl(rdram, ctx, runtime, (uint32_t)Ps2GprU32(ctx, 4)); }},
    {"reset_double_spring_interp_cntrl3D", 0x10846a8u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { reset_double_spring_interp_cntrl3D(rdram, ctx, runtime, (uint32_t)Ps2GprU32(ctx, 4), (uint32_t)Ps2GprU32(ctx, 5)); }},
    {"endBossDeathEffect", 0x10871d8u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { endBossDeathEffect(rdram, ctx, runtime); }},
    {"fsEffectSetParam", 0x1088a38u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { fsEffectSetParam(rdram, ctx, runtime, (uint64_t)Ps2GprU64(ctx, 4)); }},
    {"fsEffectManagerSetDemoParam", 0x1088d60u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { fsEffectManagerSetDemoParam(rdram, ctx, runtime, (uint32_t)Ps2GprU32(ctx, 4)); }},
    {"fsEffectManagerInit", 0x1088eb8u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { fsEffectManagerInit(rdram, ctx, runtime); }},
    {"initFsEffect", 0x1088f80u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { initFsEffect(rdram, ctx, runtime, (uint64_t)Ps2GprU64(ctx, 4)); }},
    {"setFogParamDefault", 0x1089aa0u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { setFogParamDefault(rdram, ctx, runtime); }},
    {"glBlendFunc", 0x108d150u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { glBlendFunc(rdram, ctx, runtime, (uint64_t)Ps2GprU64(ctx, 4), (uint64_t)Ps2GprU64(ctx, 5)); }},
    {"glBlendFuncParticle", 0x108d290u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { glBlendFuncParticle(rdram, ctx, runtime, (uint64_t)Ps2GprU64(ctx, 4)); }},
    {"glColor4f", 0x108e8b8u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { glColor4f(rdram, ctx, runtime, ctx->f[12], ctx->f[13], ctx->f[14], ctx->f[15]); }},
    {"glColor4fv", 0x108e938u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { glColor4fv(rdram, ctx, runtime, (uint32_t)Ps2GprU32(ctx, 4)); }},
    {"modelGetDlLayer", 0x1096a80u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { modelGetDlLayer(rdram, ctx, runtime, (uint64_t)Ps2GprU64(ctx, 4), (uint64_t)Ps2GprU64(ctx, 5), (uint64_t)Ps2GprU64(ctx, 6)); }},
    {"modelResetSurface", 0x10993c8u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { modelResetSurface(rdram, ctx, runtime, (uint32_t)Ps2GprU32(ctx, 4)); }},
    {"modelSetMatrixTransTag", 0x1099438u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { modelSetMatrixTransTag(rdram, ctx, runtime, (uint64_t)Ps2GprU64(ctx, 5), (uint64_t)Ps2GprU64(ctx, 6), (uint64_t)Ps2GprU64(ctx, 7)); }},
    {"ptclmanBuildMatPacket", 0x1099e00u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { ptclmanBuildMatPacket(rdram, ctx, runtime, (uint64_t)Ps2GprU64(ctx, 4), ctx->f[12], ctx->f[13]); }},
    {"GetMeshClass", 0x109aa48u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { GetMeshClass(rdram, ctx, runtime, (uint64_t)Ps2GprU64(ctx, 4)); }},
    {"meshFlip", 0x109abc8u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { meshFlip(rdram, ctx, runtime, (uint32_t)Ps2GprU32(ctx, 4)); }},
    {"spriteSetTextureCoordinateST", 0x10a52e0u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { spriteSetTextureCoordinateST(rdram, ctx, runtime, (uint64_t)Ps2GprU64(ctx, 4), ctx->f[12], ctx->f[13], ctx->f[14], ctx->f[15]); }},
    {"spriteSetBlendMode", 0x10a5440u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { spriteSetBlendMode(rdram, ctx, runtime, (uint64_t)Ps2GprU64(ctx, 4), (uint64_t)Ps2GprU64(ctx, 5), (uint64_t)Ps2GprU64(ctx, 6), (uint64_t)Ps2GprU64(ctx, 7)); }},
    {"texGetTexTBPSize", 0x10aacb8u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { texGetTexTBPSize(rdram, ctx, runtime, (uint32_t)Ps2GprU32(ctx, 4), (uint64_t)Ps2GprU64(ctx, 5), (uint32_t)Ps2GprU32(ctx, 6)); }},
    {"manager_sub_010AE658", 0x10ae658u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { manager_sub_010AE658(rdram, ctx, runtime, (uint32_t)Ps2GprU32(ctx, 4), (uint64_t)Ps2GprU64(ctx, 5), (uint32_t)Ps2GprU32(ctx, 6), ctx->f[12], ctx->f[13]); }},
    {"getBlendBaseMotion", 0x10b2790u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { getBlendBaseMotion(rdram, ctx, runtime, (uint64_t)Ps2GprU64(ctx, 4)); }},
    {"GetMotionFrameDirectResource", 0x10b62f0u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { GetMotionFrameDirectResource(rdram, ctx, runtime, (uint32_t)Ps2GprU32(ctx, 4), (uint32_t)Ps2GprU32(ctx, 5), (uint64_t)Ps2GprU64(ctx, 6), ctx->f[12], ctx->f[13]); }},
    {"GetMotionOptionNodeParentId", 0x10b63c0u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { GetMotionOptionNodeParentId(rdram, ctx, runtime, (uint32_t)Ps2GprU32(ctx, 4), (uint32_t)Ps2GprU32(ctx, 5)); }},
    {"InitMotionBoolInfo", 0x10b7618u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { InitMotionBoolInfo(rdram, ctx, runtime, (uint32_t)Ps2GprU32(ctx, 4)); }},
    {"ResetCharacterClassIKInfo", 0x10c1158u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { ResetCharacterClassIKInfo(rdram, ctx, runtime, (uint32_t)Ps2GprU32(ctx, 4)); }},
    {"GetCharacterClassCurAnimationCurFrame", 0x10c1a80u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { GetCharacterClassCurAnimationCurFrame(rdram, ctx, runtime, (uint32_t)Ps2GprU32(ctx, 4)); }},
    {"SetCharacterObjControlBypassFunc", 0x10c1d38u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { SetCharacterObjControlBypassFunc(rdram, ctx, runtime, (uint64_t)Ps2GprU64(ctx, 4), (uint32_t)Ps2GprU32(ctx, 5)); }},
    {"SetCharacterObjParentPlaneDiscontinuousJudgeFunc", 0x10c2910u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { SetCharacterObjParentPlaneDiscontinuousJudgeFunc(rdram, ctx, runtime, (uint64_t)Ps2GprU64(ctx, 4), (uint32_t)Ps2GprU32(ctx, 5)); }},
    {"SetCharacterObjParentPointXZFitGpChangeJudgeFunc", 0x10c2940u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { SetCharacterObjParentPointXZFitGpChangeJudgeFunc(rdram, ctx, runtime, (uint64_t)Ps2GprU64(ctx, 4), (uint32_t)Ps2GprU32(ctx, 5), (uint32_t)Ps2GprU32(ctx, 6)); }},
    {"manager_sub_010C9888", 0x10c9888u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { manager_sub_010C9888(rdram, ctx, runtime, (uint32_t)Ps2GprU32(ctx, 4), (uint32_t)Ps2GprU32(ctx, 5)); }},
    {"InitScriptControlButtonCallBack", 0x10cc4f0u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { InitScriptControlButtonCallBack(rdram, ctx, runtime, (uint64_t)Ps2GprU64(ctx, 4)); }},
    {"manager_sub_010CD400", 0x10cd400u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { manager_sub_010CD400(rdram, ctx, runtime, (uint32_t)Ps2GprU32(ctx, 4), ctx->f[12]); }},
    {"RequestCharacterClassMotionChangeWithParam", 0x10d2b18u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { RequestCharacterClassMotionChangeWithParam(rdram, ctx, runtime, (uint32_t)Ps2GprU32(ctx, 4), (uint32_t)Ps2GprU32(ctx, 5), (uint32_t)Ps2GprU32(ctx, 6), ctx->f[12]); }},
    {"charrootdyn_getLimitMode", 0x10de398u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { charrootdyn_getLimitMode(rdram, ctx, runtime, (uint32_t)Ps2GprU32(ctx, 4), (uint32_t)Ps2GprU32(ctx, 5)); }},
    {"resetExpressionLocalHeightDiff", 0x10e2170u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { resetExpressionLocalHeightDiff(rdram, ctx, runtime, (uint32_t)Ps2GprU32(ctx, 4)); }},
    {"GetDemoOrientNext", 0x10ebb90u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { GetDemoOrientNext(rdram, ctx, runtime, (uint64_t)Ps2GprU64(ctx, 4)); }},
    {"UpdateFogCol", 0x10ed440u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { UpdateFogCol(rdram, ctx, runtime, (uint32_t)Ps2GprU32(ctx, 4), (uint32_t)Ps2GprU32(ctx, 5)); }},
    {"IsEnvelopeDistantLightType", 0x10f8f90u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { IsEnvelopeDistantLightType(rdram, ctx, runtime, (uint64_t)Ps2GprU64(ctx, 4), (uint32_t)Ps2GprU32(ctx, 5)); }},
    {"globalLightCalcLightNum", 0x10f8ff0u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { globalLightCalcLightNum(rdram, ctx, runtime, (uint32_t)Ps2GprU32(ctx, 4), (uint64_t)Ps2GprU64(ctx, 5)); }},
    {"GetImpactWorstWorkLightClass", 0x10f92b0u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { GetImpactWorstWorkLightClass(rdram, ctx, runtime, (uint32_t)Ps2GprU32(ctx, 4)); }},
    {"GetLwsOrientNodeNum", 0x10fc1f0u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { GetLwsOrientNodeNum(rdram, ctx, runtime, (uint32_t)Ps2GprU32(ctx, 4)); }},
    {"manager_sub_010FC618", 0x10fc618u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { manager_sub_010FC618(rdram, ctx, runtime, (uint64_t)Ps2GprU64(ctx, 4)); }},
    {"initAnimationObjResource", 0x1100af8u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { initAnimationObjResource(rdram, ctx, runtime, (uint32_t)Ps2GprU32(ctx, 4)); }},
    {"InitBinarySize", 0x1101650u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { InitBinarySize(rdram, ctx, runtime); }},
    {"SetScriptAnimationGlobalBaseInfo", 0x110a670u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { SetScriptAnimationGlobalBaseInfo(rdram, ctx, runtime, (uint64_t)Ps2GprU64(ctx, 4)); }},
    {"CopyScriptAnimationWindGeometryPointer", 0x110a7d8u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { CopyScriptAnimationWindGeometryPointer(rdram, ctx, runtime, (uint32_t)Ps2GprU32(ctx, 4), (uint32_t)Ps2GprU32(ctx, 5)); }},
    {"GetSpriteMenuClass", 0x1111998u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { GetSpriteMenuClass(rdram, ctx, runtime, (uint64_t)Ps2GprU64(ctx, 4)); }},
    {"SpriteMenuSetPadEventCallback", 0x11119d8u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { SpriteMenuSetPadEventCallback(rdram, ctx, runtime, (uint32_t)Ps2GprU32(ctx, 4), (uint32_t)Ps2GprU32(ctx, 5), (uint32_t)Ps2GprU32(ctx, 6)); }},
    {"SpriteSubMenuSetPadEventCallback", 0x1111a08u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { SpriteSubMenuSetPadEventCallback(rdram, ctx, runtime, (uint32_t)Ps2GprU32(ctx, 4), (uint32_t)Ps2GprU32(ctx, 5), (uint32_t)Ps2GprU32(ctx, 6)); }},
    {"SpriteMenuSetDispFlag", 0x1111a38u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { SpriteMenuSetDispFlag(rdram, ctx, runtime, (uint32_t)Ps2GprU32(ctx, 4), (uint32_t)Ps2GprU32(ctx, 5), (uint64_t)Ps2GprU64(ctx, 6)); }},
    {"SpriteMenuSetDispFlagByHash", 0x1111b78u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { SpriteMenuSetDispFlagByHash(rdram, ctx, runtime, (uint64_t)Ps2GprU64(ctx, 4), (uint64_t)Ps2GprU64(ctx, 5), (uint64_t)Ps2GprU64(ctx, 6)); }},
    {"SpriteMenuSetLayer", 0x1111e20u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { SpriteMenuSetLayer(rdram, ctx, runtime, (uint32_t)Ps2GprU32(ctx, 4), (uint64_t)Ps2GprU64(ctx, 5)); }},
    {"SpriteMenuGetLayerStatus", 0x1111f60u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { SpriteMenuGetLayerStatus(rdram, ctx, runtime, (uint32_t)Ps2GprU32(ctx, 4), (uint32_t)Ps2GprU32(ctx, 5)); }},
    {"SpriteMenuGetLayerByHash", 0x1111f90u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { SpriteMenuGetLayerByHash(rdram, ctx, runtime, (uint64_t)Ps2GprU64(ctx, 4), (uint64_t)Ps2GprU64(ctx, 5)); }},
    {"SpriteMenuGetLayerNoByHash", 0x1112008u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { SpriteMenuGetLayerNoByHash(rdram, ctx, runtime, (uint64_t)Ps2GprU64(ctx, 4), (uint64_t)Ps2GprU64(ctx, 5)); }},
    {"SpriteMenuGetMenuIdByHash", 0x1112c88u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { SpriteMenuGetMenuIdByHash(rdram, ctx, runtime, (uint64_t)Ps2GprU64(ctx, 4), (uint64_t)Ps2GprU64(ctx, 5), (uint64_t)Ps2GprU64(ctx, 6)); }},
    {"SpriteMenuGetSelectMenuIdBySpriteMenuClass", 0x1112e68u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { SpriteMenuGetSelectMenuIdBySpriteMenuClass(rdram, ctx, runtime, (uint32_t)Ps2GprU32(ctx, 4)); }},
    {"SpriteMenuSetSelectMenuId", 0x1112ef0u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { SpriteMenuSetSelectMenuId(rdram, ctx, runtime, (uint32_t)Ps2GprU32(ctx, 4), (uint32_t)Ps2GprU32(ctx, 5)); }},
    {"SpriteMenuGetSelectMenuMode", 0x1113088u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { SpriteMenuGetSelectMenuMode(rdram, ctx, runtime, (uint32_t)Ps2GprU32(ctx, 4)); }},
    {"SpriteMenuSetDefaultSelectMenu", 0x1113130u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { SpriteMenuSetDefaultSelectMenu(rdram, ctx, runtime, (uint32_t)Ps2GprU32(ctx, 4), (uint32_t)Ps2GprU32(ctx, 5), (uint32_t)Ps2GprU32(ctx, 6)); }},
    {"SpriteMenuSetNumPartsFormatByPartsParam", 0x11137b8u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { SpriteMenuSetNumPartsFormatByPartsParam(rdram, ctx, runtime, (uint32_t)Ps2GprU32(ctx, 5), (uint32_t)Ps2GprU32(ctx, 6)); }},
    {"SpriteMenuSetAnimParam", 0x11137e0u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { SpriteMenuSetAnimParam(rdram, ctx, runtime, (uint64_t)Ps2GprU64(ctx, 4), (uint64_t)Ps2GprU64(ctx, 5), (uint32_t)Ps2GprU32(ctx, 6), (uint64_t)Ps2GprU64(ctx, 7), ctx->f[12]); }},
    {"GetSpriteClass", 0x1114170u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { GetSpriteClass(rdram, ctx, runtime, (uint64_t)Ps2GprU64(ctx, 4)); }},
    {"ChangeCollisionResourceAttribute", 0x1128778u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { ChangeCollisionResourceAttribute(rdram, ctx, runtime, (uint64_t)Ps2GprU64(ctx, 4), (uint64_t)Ps2GprU64(ctx, 5), (uint64_t)Ps2GprU64(ctx, 6), (uint64_t)Ps2GprU64(ctx, 7), (uint64_t)Ps2GprU64(ctx, 8), (uint64_t)Ps2GprU64(ctx, 9), (uint64_t)Ps2GprU64(ctx, 10)); }},
    {"ConstructorCollisionObjectJudgeCallbackData", 0x1128898u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { ConstructorCollisionObjectJudgeCallbackData(rdram, ctx, runtime, (uint32_t)Ps2GprU32(ctx, 4)); }},
    {"CopyCollisionFinder", 0x112a350u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { CopyCollisionFinder(rdram, ctx, runtime, (uint64_t)Ps2GprU64(ctx, 4), (uint64_t)Ps2GprU64(ctx, 5)); }},
    {"GetGeometryPointerObjectParentNodeId", 0x1132a00u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { GetGeometryPointerObjectParentNodeId(rdram, ctx, runtime, (uint64_t)Ps2GprU64(ctx, 4)); }},
    {"GetClothWorkSize", 0x1139128u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { GetClothWorkSize(rdram, ctx, runtime, (uint64_t)Ps2GprU64(ctx, 4)); }},
    {"manager_sub_01140630", 0x1140630u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { manager_sub_01140630(rdram, ctx, runtime); }},
    {"manager_sub_011408E0", 0x11408e0u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { manager_sub_011408E0(rdram, ctx, runtime, (uint64_t)Ps2GprU64(ctx, 4), (uint32_t)Ps2GprU32(ctx, 5)); }},
    {"manager_sub_01140E60", 0x1140e60u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { manager_sub_01140E60(rdram, ctx, runtime, (uint32_t)Ps2GprU32(ctx, 4), (uint32_t)Ps2GprU32(ctx, 5), (uint32_t)Ps2GprU32(ctx, 6)); }},
    {"manager_sub_01140FD8", 0x1140fd8u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { manager_sub_01140FD8(rdram, ctx, runtime, (uint32_t)Ps2GprU32(ctx, 4), (uint32_t)Ps2GprU32(ctx, 5)); }},
    {"manager_sub_011418D8", 0x11418d8u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { manager_sub_011418D8(rdram, ctx, runtime, (uint32_t)Ps2GprU32(ctx, 4), (uint32_t)Ps2GprU32(ctx, 5), (uint32_t)Ps2GprU32(ctx, 6)); }},
    {"manager_sub_01141E80", 0x1141e80u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { manager_sub_01141E80(rdram, ctx, runtime, (uint32_t)Ps2GprU32(ctx, 4)); }},
    {"get_particle_parameter_blend_disp", 0x1145ba8u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { get_particle_parameter_blend_disp(rdram, ctx, runtime, (uint32_t)Ps2GprU32(ctx, 4), (uint32_t)Ps2GprU32(ctx, 5), (uint64_t)Ps2GprU64(ctx, 6), (uint64_t)Ps2GprU64(ctx, 7), ctx->f[12]); }},
    {"InitTwoWayLink", 0x1147a78u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { InitTwoWayLink(rdram, ctx, runtime, (uint32_t)Ps2GprU32(ctx, 4), (uint32_t)Ps2GprU32(ctx, 5)); }},
    {"AddTwoWayLinkNext", 0x1147a88u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { AddTwoWayLinkNext(rdram, ctx, runtime, (uint32_t)Ps2GprU32(ctx, 4), (uint32_t)Ps2GprU32(ctx, 5)); }},
    {"DeleteTwoWayLink", 0x1147ab0u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { DeleteTwoWayLink(rdram, ctx, runtime, (uint32_t)Ps2GprU32(ctx, 4)); }},
    {"adpcmSetPan", 0x1157a98u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { adpcmSetPan(rdram, ctx, runtime, (uint32_t)Ps2GprU32(ctx, 4), ctx->f[12]); }},
    {"dsysFileInit", 0x115a060u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { dsysFileInit(rdram, ctx, runtime); }},
    {"sheetFileGetDataAdrs", 0x115cf90u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { sheetFileGetDataAdrs(rdram, ctx, runtime, (uint32_t)Ps2GprU32(ctx, 4)); }},
    {"sheetMgrSheetHeaderSearchSheetDataObj", 0x115ec60u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { sheetMgrSheetHeaderSearchSheetDataObj(rdram, ctx, runtime, (uint32_t)Ps2GprU32(ctx, 4), (uint64_t)Ps2GprU64(ctx, 5)); }},
    {"manager_sub_011629E8", 0x11629e8u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { manager_sub_011629E8(rdram, ctx, runtime, (uint64_t)Ps2GprU64(ctx, 4)); }},
    {"get_hit_region_ofst_camera_region", 0x1163848u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { get_hit_region_ofst_camera_region(rdram, ctx, runtime, (uint64_t)Ps2GprU64(ctx, 4)); }},
    {"set_scissor_screen_size", 0x11644d0u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { set_scissor_screen_size(rdram, ctx, runtime, (uint64_t)Ps2GprU64(ctx, 4), (uint32_t)Ps2GprU32(ctx, 5), (uint32_t)Ps2GprU32(ctx, 6), (uint32_t)Ps2GprU32(ctx, 7)); }},
    {"DecreaseDamageLockTime", 0x1166e50u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { DecreaseDamageLockTime(rdram, ctx, runtime, (uint32_t)Ps2GprU32(ctx, 4), ctx->f[12]); }},
    {"GetDamagerClassDamageSheetType", 0x1166e98u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { GetDamagerClassDamageSheetType(rdram, ctx, runtime, (uint32_t)Ps2GprU32(ctx, 4)); }},
    {"ResetAnimeFrameTableInfo", 0x11692a8u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { ResetAnimeFrameTableInfo(rdram, ctx, runtime, (uint64_t)Ps2GprU64(ctx, 4), (uint64_t)Ps2GprU64(ctx, 5), (uint64_t)Ps2GprU64(ctx, 6)); }},
    {"rintf", 0x116ad80u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { rintf(rdram, ctx, runtime, ctx->f[12]); }},
    {"GAMEUTIL_set_parameter_look_at_vs_offset", 0x1343eb8u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { GAMEUTIL_set_parameter_look_at_vs_offset(rdram, ctx, runtime, (uint32_t)Ps2GprU32(ctx, 4), ctx->f[12], ctx->f[13], ctx->f[14], ctx->f[15], ctx->f[16]); }},
    {"GAMEUTIL_doit_status_manager", 0x13445d8u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { GAMEUTIL_doit_status_manager(rdram, ctx, runtime, (uint32_t)Ps2GprU32(ctx, 4), (uint64_t)Ps2GprU64(ctx, 5), (uint64_t)Ps2GprU64(ctx, 6), ctx->f[12]); }},
    {"ReleaseGame", 0x1345ce0u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { ReleaseGame(rdram, ctx, runtime, (uint64_t)Ps2GprU64(ctx, 4)); }},
    {"getDemoBarFade", 0x1346300u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { getDemoBarFade(rdram, ctx, runtime); }},
    {"set_base_pitch_parameter_nico_yaw_pitch_cntrl", 0x1347028u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { set_base_pitch_parameter_nico_yaw_pitch_cntrl(rdram, ctx, runtime, (uint32_t)Ps2GprU32(ctx, 4), (uint32_t)Ps2GprU32(ctx, 5), ctx->f[12], ctx->f[13], ctx->f[14], ctx->f[15], ctx->f[16], ctx->f[17]); }},
    {"gamecore_sub_01347B28", 0x1347b28u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { gamecore_sub_01347B28(rdram, ctx, runtime, (uint32_t)Ps2GprU32(ctx, 4), ctx->f[12], ctx->f[13]); }},
    {"gamecore_sub_01347B88", 0x1347b88u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { gamecore_sub_01347B88(rdram, ctx, runtime, (uint32_t)Ps2GprU32(ctx, 4), (uint32_t)Ps2GprU32(ctx, 5), (uint32_t)Ps2GprU32(ctx, 6)); }},
    {"backStageCheckPointSave", 0x1349318u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { backStageCheckPointSave(rdram, ctx, runtime, (uint64_t)Ps2GprU64(ctx, 4)); }},
    {"backStageCheckPointLoad", 0x13493e0u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { backStageCheckPointLoad(rdram, ctx, runtime, (uint64_t)Ps2GprU64(ctx, 4)); }},
    {"_optionWorkSet", 0x134ef50u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { _optionWorkSet(rdram, ctx, runtime, (uint32_t)Ps2GprU32(ctx, 4), (uint32_t)Ps2GprU32(ctx, 5)); }},
    {"_gameFlagSet", 0x134efb0u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { _gameFlagSet(rdram, ctx, runtime, (uint64_t)Ps2GprU64(ctx, 4)); }},
    {"_gameFlagCls", 0x134eff0u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { _gameFlagCls(rdram, ctx, runtime, (uint64_t)Ps2GprU64(ctx, 4)); }},
    {"_gameFlagChk", 0x134f030u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { _gameFlagChk(rdram, ctx, runtime, (uint64_t)Ps2GprU64(ctx, 4)); }},
    {"_gameWorkSet", 0x134f0a8u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { _gameWorkSet(rdram, ctx, runtime, (uint32_t)Ps2GprU32(ctx, 4), (uint32_t)Ps2GprU32(ctx, 5)); }},
    {"_gameFWorkSet", 0x134f0c0u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { _gameFWorkSet(rdram, ctx, runtime, (uint32_t)Ps2GprU32(ctx, 4), ctx->f[12]); }},
    {"_gameFWorkChk", 0x134f120u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { _gameFWorkChk(rdram, ctx, runtime, (uint32_t)Ps2GprU32(ctx, 4)); }},
    {"fruitsFlagChk", 0x134f1b0u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { fruitsFlagChk(rdram, ctx, runtime, (uint64_t)Ps2GprU64(ctx, 4), (uint64_t)Ps2GprU64(ctx, 5), (uint32_t)Ps2GprU32(ctx, 7)); }},
    {"gameFlagCheckPointLoad", 0x134f350u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { gameFlagCheckPointLoad(rdram, ctx, runtime, (uint64_t)Ps2GprU64(ctx, 4), (uint64_t)Ps2GprU64(ctx, 5), (uint64_t)Ps2GprU64(ctx, 6), (uint64_t)Ps2GprU64(ctx, 7), (uint64_t)Ps2GprU64(ctx, 8), (uint64_t)Ps2GprU64(ctx, 9)); }},
    {"tailsFlagChk", 0x134f820u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { tailsFlagChk(rdram, ctx, runtime, (uint64_t)Ps2GprU64(ctx, 4), (uint64_t)Ps2GprU64(ctx, 5), (uint32_t)Ps2GprU32(ctx, 7)); }},
    {"gexsFlagChk", 0x134f938u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { gexsFlagChk(rdram, ctx, runtime, (uint64_t)Ps2GprU64(ctx, 4), (uint64_t)Ps2GprU64(ctx, 5), (uint32_t)Ps2GprU32(ctx, 7)); }},
    {"_omakeGetFlagChk", 0x134fa18u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { _omakeGetFlagChk(rdram, ctx, runtime, (uint64_t)Ps2GprU64(ctx, 4)); }},
    {"_omakeSelectFlagChk", 0x134fb28u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { _omakeSelectFlagChk(rdram, ctx, runtime, (uint64_t)Ps2GprU64(ctx, 4)); }},
    {"gameflowBossEyeCameraDisable", 0x1352030u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { gameflowBossEyeCameraDisable(rdram, ctx, runtime, (uint64_t)Ps2GprU64(ctx, 4)); }},
    {"gameModeSet", 0x1352b30u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { gameModeSet(rdram, ctx, runtime, (uint64_t)Ps2GprU64(ctx, 4)); }},
    {"gameModeCls", 0x1352b88u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { gameModeCls(rdram, ctx, runtime, (uint64_t)Ps2GprU64(ctx, 4)); }},
    {"gStatPlaceIdToSavePoint", 0x1353440u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { gStatPlaceIdToSavePoint(rdram, ctx, runtime, (uint64_t)Ps2GprU64(ctx, 4)); }},
    {"gstatBootMcSetLastSaveOmakeStat", 0x1353e18u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { gstatBootMcSetLastSaveOmakeStat(rdram, ctx, runtime, (uint32_t)Ps2GprU32(ctx, 4)); }},
    {"opflowSetDemoFrame", 0x13576d8u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { opflowSetDemoFrame(rdram, ctx, runtime, ctx->f[12]); }},
    {"getStageChipID", 0x1358888u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { getStageChipID(rdram, ctx, runtime, (uint32_t)Ps2GprU32(ctx, 4), (uint32_t)Ps2GprU32(ctx, 5), (uint32_t)Ps2GprU32(ctx, 6)); }},
    {"gamecore_sub_01358A68", 0x1358a68u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { gamecore_sub_01358A68(rdram, ctx, runtime, (uint64_t)Ps2GprU64(ctx, 4)); }},
    {"stagemanager_checkstageAlreadyLoadRequestedByStrptr", 0x1365ab0u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { stagemanager_checkstageAlreadyLoadRequestedByStrptr(rdram, ctx, runtime, (uint64_t)Ps2GprU64(ctx, 4)); }},
    {"gcGetStageData", 0x1365d18u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { gcGetStageData(rdram, ctx, runtime, (uint32_t)Ps2GprU32(ctx, 4)); }},
    {"stagemanager_getStageNumberWithStageDataStrptr", 0x13663d0u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { stagemanager_getStageNumberWithStageDataStrptr(rdram, ctx, runtime, (uint64_t)Ps2GprU64(ctx, 4)); }},
    {"PauseStageBoundaryManager", 0x13678e8u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { PauseStageBoundaryManager(rdram, ctx, runtime, (uint64_t)Ps2GprU64(ctx, 4)); }},
    {"initBgmScript", 0x136a2e0u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { initBgmScript(rdram, ctx, runtime); }},
    {"gamecore_sub_0136A328", 0x136a328u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { gamecore_sub_0136A328(rdram, ctx, runtime, (uint64_t)Ps2GprU64(ctx, 4)); }},
    {"GetBossPartsDamagePerParts", 0x1379270u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { GetBossPartsDamagePerParts(rdram, ctx, runtime, (uint32_t)Ps2GprU32(ctx, 4), (uint32_t)Ps2GprU32(ctx, 5)); }},
    {"getexpressiondatapoint", 0x137def8u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { getexpressiondatapoint(rdram, ctx, runtime, (uint32_t)Ps2GprU32(ctx, 4), (uint32_t)Ps2GprU32(ctx, 5)); }},
    {"controllerAlertDisable", 0x137e828u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { controllerAlertDisable(rdram, ctx, runtime, (uint64_t)Ps2GprU64(ctx, 4)); }},
    {"gamecore_sub_01386B98", 0x1386b98u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { gamecore_sub_01386B98(rdram, ctx, runtime); }},
    {"fadeout_weakpoint_effect", 0x13883b0u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { fadeout_weakpoint_effect(rdram, ctx, runtime); }},
    {"cancel_fadeout_weakpoint_effect", 0x13883f8u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { cancel_fadeout_weakpoint_effect(rdram, ctx, runtime); }},
    {"set_node_data_expression", 0x138be30u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { set_node_data_expression(rdram, ctx, runtime, (uint32_t)Ps2GprU32(ctx, 4), (uint64_t)Ps2GprU64(ctx, 5), (uint32_t)Ps2GprU32(ctx, 6), ctx->f[12], ctx->f[13], ctx->f[14]); }},
    {"constructor_expression_change", 0x138bec8u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { constructor_expression_change(rdram, ctx, runtime, (uint32_t)Ps2GprU32(ctx, 4)); }},
    {"EnableArrowWater", 0x138eab0u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { EnableArrowWater(rdram, ctx, runtime, (uint32_t)Ps2GprU32(ctx, 4)); }},
    {"EndArrowWater", 0x138eaf8u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { EndArrowWater(rdram, ctx, runtime, (uint64_t)Ps2GprU64(ctx, 4)); }},
    {"gamecore_sub_01393E58", 0x1393e58u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { gamecore_sub_01393E58(rdram, ctx, runtime, (uint64_t)Ps2GprU64(ctx, 4), (uint64_t)Ps2GprU64(ctx, 5), (uint64_t)Ps2GprU64(ctx, 6), (uint64_t)Ps2GprU64(ctx, 7), ctx->f[12]); }},
    {"gamecore_sub_0139F3B0", 0x139f3b0u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { gamecore_sub_0139F3B0(rdram, ctx, runtime, (uint64_t)Ps2GprU64(ctx, 4)); }},
    {"AI_GetPosition3", 0x13d46d8u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { AI_GetPosition3(rdram, ctx, runtime, ctx->f[12], ctx->f[13], ctx->f[14], ctx->f[15], ctx->f[16], ctx->f[17]); }},
    {"AI_GetInterRatio", 0x13d4a28u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { AI_GetInterRatio(rdram, ctx, runtime, ctx->f[12], ctx->f[13], ctx->f[14], ctx->f[15], ctx->f[16]); }},
    {"AI_GetInterRatioOutCut", 0x13d4a60u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { AI_GetInterRatioOutCut(rdram, ctx, runtime, ctx->f[12], ctx->f[13], ctx->f[14], ctx->f[15], ctx->f[16]); }},
    {"Kite_CmpPathPoint", 0x13e79c8u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { Kite_CmpPathPoint(rdram, ctx, runtime, (uint32_t)Ps2GprU32(ctx, 4), (uint32_t)Ps2GprU32(ctx, 5)); }},
    {"gamecore_sub_013EF210", 0x13ef210u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { gamecore_sub_013EF210(rdram, ctx, runtime, (uint64_t)Ps2GprU64(ctx, 4), (uint64_t)Ps2GprU64(ctx, 5), (uint32_t)Ps2GprU32(ctx, 6)); }},
    {"getSObjNode", 0x13f0d60u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { getSObjNode(rdram, ctx, runtime, (uint64_t)Ps2GprU64(ctx, 4)); }},
    {"gamecore_sub_013F1290", 0x13f1290u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { gamecore_sub_013F1290(rdram, ctx, runtime, (uint32_t)Ps2GprU32(ctx, 4)); }},
    {"gamecore_sub_013F12F0", 0x13f12f0u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { gamecore_sub_013F12F0(rdram, ctx, runtime, (uint32_t)Ps2GprU32(ctx, 4), (uint64_t)Ps2GprU64(ctx, 5)); }},
    {"gamecore_sub_013F1330", 0x13f1330u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { gamecore_sub_013F1330(rdram, ctx, runtime, (uint64_t)Ps2GprU64(ctx, 4)); }},
    {"gamecore_sub_013F1370", 0x13f1370u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { gamecore_sub_013F1370(rdram, ctx, runtime, (uint32_t)Ps2GprU32(ctx, 4)); }},
    {"gamecore_sub_013F13E0", 0x13f13e0u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { gamecore_sub_013F13E0(rdram, ctx, runtime, (uint32_t)Ps2GprU32(ctx, 4), (uint64_t)Ps2GprU64(ctx, 5)); }},
    {"gamecore_sub_013F1430", 0x13f1430u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { gamecore_sub_013F1430(rdram, ctx, runtime, (uint64_t)Ps2GprU64(ctx, 4), (uint64_t)Ps2GprU64(ctx, 5), (uint64_t)Ps2GprU64(ctx, 6)); }},
    {"gamecore_sub_013F2558", 0x13f2558u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { gamecore_sub_013F2558(rdram, ctx, runtime, (uint32_t)Ps2GprU32(ctx, 4)); }},
    {"initWeaponSpriteStatus", 0x13f95a0u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { initWeaponSpriteStatus(rdram, ctx, runtime); }},
    {"get_camera_collision_data_base_filter_radius", 0x140f418u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { get_camera_collision_data_base_filter_radius(rdram, ctx, runtime, ctx->f[12], ctx->f[13], ctx->f[14]); }},
    {"is_cliff_rise_ok_camera_place_status", 0x1413538u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { is_cliff_rise_ok_camera_place_status(rdram, ctx, runtime, (uint64_t)Ps2GprU64(ctx, 4), (uint64_t)Ps2GprU64(ctx, 5)); }},
    {"act_local_vs_ofst_camera_place_status", 0x1413608u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { act_local_vs_ofst_camera_place_status(rdram, ctx, runtime, (uint64_t)Ps2GprU64(ctx, 4), (uint64_t)Ps2GprU64(ctx, 5)); }},
    {"gamecore_sub_014144F0", 0x14144f0u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { gamecore_sub_014144F0(rdram, ctx, runtime, ctx->f[12], ctx->f[13], ctx->f[14]); }},
    {"gamecore_sub_01414E78", 0x1414e78u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { gamecore_sub_01414E78(rdram, ctx, runtime, (uint32_t)Ps2GprU32(ctx, 4)); }},
    {"gamecore_sub_0141B6C0", 0x141b6c0u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { gamecore_sub_0141B6C0(rdram, ctx, runtime, (uint64_t)Ps2GprU64(ctx, 4), (uint64_t)Ps2GprU64(ctx, 7)); }},
    {"gamecore_sub_0141B900", 0x141b900u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { gamecore_sub_0141B900(rdram, ctx, runtime, (uint32_t)Ps2GprU32(ctx, 4), (uint32_t)Ps2GprU32(ctx, 5)); }},
    {"gamecore_sub_0141BA40", 0x141ba40u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { gamecore_sub_0141BA40(rdram, ctx, runtime, (uint32_t)Ps2GprU32(ctx, 4), ctx->f[12]); }},
    {"gamecore_sub_0141BC78", 0x141bc78u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { gamecore_sub_0141BC78(rdram, ctx, runtime, (uint32_t)Ps2GprU32(ctx, 4), (uint32_t)Ps2GprU32(ctx, 5), ctx->f[12], ctx->f[13]); }},
    {"add_camera_mode_common_param_segment", 0x14255a0u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { add_camera_mode_common_param_segment(rdram, ctx, runtime, (uint64_t)Ps2GprU64(ctx, 4)); }},
    {"add_camera_mode_manager_segment", 0x14255f0u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { add_camera_mode_manager_segment(rdram, ctx, runtime, (uint64_t)Ps2GprU64(ctx, 4)); }},
    {"is_base_default_dir_attr_change_game_camera_param_manager", 0x142c1a0u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { is_base_default_dir_attr_change_game_camera_param_manager(rdram, ctx, runtime, (uint32_t)Ps2GprU32(ctx, 4)); }},
    {"gamecore_sub_0142C7A0", 0x142c7a0u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { gamecore_sub_0142C7A0(rdram, ctx, runtime, (uint32_t)Ps2GprU32(ctx, 4), (uint32_t)Ps2GprU32(ctx, 5), (uint32_t)Ps2GprU32(ctx, 6)); }},
    {"gamecore_sub_01430248", 0x1430248u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { gamecore_sub_01430248(rdram, ctx, runtime, (uint32_t)Ps2GprU32(ctx, 4), (uint32_t)Ps2GprU32(ctx, 5), (uint32_t)Ps2GprU32(ctx, 6), (uint64_t)Ps2GprU64(ctx, 7), (uint64_t)Ps2GprU64(ctx, 8), (uint32_t)Ps2GprU32(ctx, 9), ctx->f[12]); }},
    {"getPathDataAddr", 0x1433340u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { getPathDataAddr(rdram, ctx, runtime, (uint32_t)Ps2GprU32(ctx, 4), (uint64_t)Ps2GprU64(ctx, 5)); }},
    {"gamecore_sub_01434610", 0x1434610u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { gamecore_sub_01434610(rdram, ctx, runtime, (uint32_t)Ps2GprU32(ctx, 4), (uint64_t)Ps2GprU64(ctx, 5), (uint64_t)Ps2GprU64(ctx, 6)); }},
    {"calcDownSpeedRatio", 0x1437f28u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { calcDownSpeedRatio(rdram, ctx, runtime, ctx->f[12], ctx->f[13], ctx->f[14]); }},
    {"gamecore_sub_0143D6F8", 0x143d6f8u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { gamecore_sub_0143D6F8(rdram, ctx, runtime, (uint32_t)Ps2GprU32(ctx, 4)); }},
    {"setRunStopTurnAngleFastLegCtrlWork", 0x1440f48u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { setRunStopTurnAngleFastLegCtrlWork(rdram, ctx, runtime, (uint32_t)Ps2GprU32(ctx, 4), ctx->f[12]); }},
    {"gamecore_sub_01441020", 0x1441020u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { gamecore_sub_01441020(rdram, ctx, runtime, (uint32_t)Ps2GprU32(ctx, 4)); }},
    {"gamecore_sub_01441210", 0x1441210u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { gamecore_sub_01441210(rdram, ctx, runtime, (uint32_t)Ps2GprU32(ctx, 4), (uint64_t)Ps2GprU64(ctx, 5)); }},
    {"gamecore_sub_01444370", 0x1444370u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { gamecore_sub_01444370(rdram, ctx, runtime, (uint32_t)Ps2GprU32(ctx, 4), (uint64_t)Ps2GprU64(ctx, 5)); }},
    {"adjustAnglePI", 0x1446f80u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { adjustAnglePI(rdram, ctx, runtime, ctx->f[12]); }},
    {"initBeardIKWork", 0x145f0e8u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { initBeardIKWork(rdram, ctx, runtime, (uint32_t)Ps2GprU32(ctx, 6)); }},
    {"setRatioGazeIKWork", 0x1461758u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { setRatioGazeIKWork(rdram, ctx, runtime, (uint32_t)Ps2GprU32(ctx, 4), ctx->f[12], ctx->f[13], ctx->f[14]); }},
    {"setWormIKNodeLimit", 0x14647f0u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { setWormIKNodeLimit(rdram, ctx, runtime, (uint32_t)Ps2GprU32(ctx, 4), ctx->f[12]); }},
    {"kernel_sub_001AD958", 0x1ad958u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { kernel_sub_001AD958(rdram, ctx, runtime, (uint64_t)Ps2GprU64(ctx, 4), (uint64_t)Ps2GprU64(ctx, 5), (uint32_t)Ps2GprU32(ctx, 6)); }},
    {"Sg2CtrlInit", 0x1cc278u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { Sg2CtrlInit(rdram, ctx, runtime); }},
    {"GAMEUTIL_reset_interp_value_cntrl", 0x1084428u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { GAMEUTIL_reset_interp_value_cntrl(rdram, ctx, runtime, (uint64_t)Ps2GprU64(ctx, 4), ctx->f[12]); }},
    {"constructor_double_spring_interp_cntrl", 0x1084528u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { constructor_double_spring_interp_cntrl(rdram, ctx, runtime, (uint64_t)Ps2GprU64(ctx, 4), ctx->f[12], ctx->f[20]); }},
    {"constructor_double_spring_interp_cntrl3D", 0x10845f8u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { constructor_double_spring_interp_cntrl3D(rdram, ctx, runtime, (uint64_t)Ps2GprU64(ctx, 4), ctx->f[12], ctx->f[13], ctx->f[20], ctx->f[21]); }},
    {"manager_sub_010DF5D0", 0x10df5d0u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { manager_sub_010DF5D0(rdram, ctx, runtime, (uint64_t)Ps2GprU64(ctx, 4), (uint64_t)Ps2GprU64(ctx, 5)); }},
    {"SetScriptLayoutObjLetterProcMode", 0x110d0a0u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { SetScriptLayoutObjLetterProcMode(rdram, ctx, runtime, (uint64_t)Ps2GprU64(ctx, 4), (uint64_t)Ps2GprU64(ctx, 5)); }},
    {"gStatGetHorseDemoStageIndex", 0x13538e8u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { gStatGetHorseDemoStageIndex(rdram, ctx, runtime, (uint32_t)Ps2GprU32(ctx, 4)); }},
    {"getPlayerMaxGripRatio", 0x13586d0u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { getPlayerMaxGripRatio(rdram, ctx, runtime, (uint32_t)Ps2GprU32(ctx, 4), ctx->f[0]); }},
    {"initGameObject", 0x1366d20u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { initGameObject(rdram, ctx, runtime, (uint32_t)Ps2GprU32(ctx, 4), (uint32_t)Ps2GprU32(ctx, 5)); }},
    {"gamecore_sub_013AE748", 0x13ae748u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { gamecore_sub_013AE748(rdram, ctx, runtime, (uint64_t)Ps2GprU64(ctx, 4)); }},
    {"gamecore_sub_013F3378", 0x13f3378u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { gamecore_sub_013F3378(rdram, ctx, runtime, (uint64_t)Ps2GprU64(ctx, 4), (uint64_t)Ps2GprU64(ctx, 5), ctx->f[12], ctx->f[13], ctx->f[20]); }},
    {"gamecore_sub_013F3778", 0x13f3778u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { gamecore_sub_013F3778(rdram, ctx, runtime, (uint64_t)Ps2GprU64(ctx, 4), ctx->f[20]); }},
    {"gamecore_sub_013F3868", 0x13f3868u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { gamecore_sub_013F3868(rdram, ctx, runtime, (uint64_t)Ps2GprU64(ctx, 4), ctx->f[20]); }},
    {"checkChangeWeaponTypeSwordToOther", 0x13ff4a8u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { checkChangeWeaponTypeSwordToOther(rdram, ctx, runtime, (uint64_t)Ps2GprU64(ctx, 4), (uint64_t)Ps2GprU64(ctx, 5)); }},
    {"initWeapon", 0x14072f8u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { initWeapon(rdram, ctx, runtime); }},
    {"gamecore_sub_014140B8", 0x14140b8u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { gamecore_sub_014140B8(rdram, ctx, runtime, (uint64_t)Ps2GprU64(ctx, 4), (uint32_t)Ps2GprU32(ctx, 5), ctx->f[0], ctx->f[20]); }},
    {"is_view_angle_zoom_game_camera", 0x1425518u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { is_view_angle_zoom_game_camera(rdram, ctx, runtime, (uint64_t)Ps2GprU64(ctx, 4)); }},
    {"SCRConditionInvisibilityCloak", 0x1459a20u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { SCRConditionInvisibilityCloak(rdram, ctx, runtime); }},
    {"SCRConditionGetOmakeFlag", 0x1459a38u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { SCRConditionGetOmakeFlag(rdram, ctx, runtime, (uint64_t)Ps2GprU64(ctx, 5)); }},
    {"eventBgmAllClose", 0x145aeb0u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { eventBgmAllClose(rdram, ctx, runtime, (uint64_t)Ps2GprU64(ctx, 5)); }},
};
const uint32_t kEntryCount = 335u;
}
#if !defined(A3_OFFLINE)
void a3ckReport(unsigned long long frame) { a3ck::ckReport(frame); }
#endif
#endif
